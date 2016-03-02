// Copyright(c) 2015-2016, NVIDIA CORPORATION. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#ifndef VK_CPP_H_
#define VK_CPP_H_

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <vulkan/vulkan.h>
#ifdef VKCPP_ENHANCED_MODE
# include <vector>
#endif /*VKCPP_ENHANCED_MODE*/

static_assert( VK_MAKE_VERSION(1, 0, 4) == VK_API_VERSION, "Wrong VK_API_VERSION!" );

// 32-bit vulkan is not typesafe for handles, so don't allow copy constructors on this platform by default.
// To enable this feature on 32-bit platforms please define VK_CPP_TYPESAFE_CONVERSION
#if defined(__LP64__) || defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
#define VK_CPP_TYPESAFE_CONVERSION 1
#endif

namespace vk
{
  enum class Result;

  class Exception : public std::runtime_error
  {
  public:
    Exception(Result result, std::string const& what)
      : std::runtime_error(what)
      , m_error(result)
    {}

  private:
    Result m_error;
  };

  template <typename BitType, typename MaskType = uint32_t>
  class Flags
  {
  public:
    Flags()
      : m_mask(0)
    {
    }

    Flags(BitType bit)
      : m_mask(static_cast<uint32_t>(bit))
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

    bool operator==(Flags<BitType> const& rhs) const
    {
      return m_mask == rhs.m_mask;
    }

    bool operator!=(Flags<BitType> const& rhs) const
    {
      return m_mask != rhs.m_mask;
    }

    operator bool() const
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

  typedef uint32_t SampleMask;
  typedef uint32_t Bool32;
  typedef uint64_t DeviceSize;
  enum class FramebufferCreateFlagBits
  {
  };

  typedef Flags<FramebufferCreateFlagBits, VkFramebufferCreateFlags> FramebufferCreateFlags;

  inline FramebufferCreateFlags operator|( FramebufferCreateFlagBits bit0, FramebufferCreateFlagBits bit1 )
  {
    return FramebufferCreateFlags( bit0 ) | bit1;
  }

  enum class QueryPoolCreateFlagBits
  {
  };

  typedef Flags<QueryPoolCreateFlagBits, VkQueryPoolCreateFlags> QueryPoolCreateFlags;

  inline QueryPoolCreateFlags operator|( QueryPoolCreateFlagBits bit0, QueryPoolCreateFlagBits bit1 )
  {
    return QueryPoolCreateFlags( bit0 ) | bit1;
  }

  enum class RenderPassCreateFlagBits
  {
  };

  typedef Flags<RenderPassCreateFlagBits, VkRenderPassCreateFlags> RenderPassCreateFlags;

  inline RenderPassCreateFlags operator|( RenderPassCreateFlagBits bit0, RenderPassCreateFlagBits bit1 )
  {
    return RenderPassCreateFlags( bit0 ) | bit1;
  }

  enum class SamplerCreateFlagBits
  {
  };

  typedef Flags<SamplerCreateFlagBits, VkSamplerCreateFlags> SamplerCreateFlags;

  inline SamplerCreateFlags operator|( SamplerCreateFlagBits bit0, SamplerCreateFlagBits bit1 )
  {
    return SamplerCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineLayoutCreateFlagBits
  {
  };

  typedef Flags<PipelineLayoutCreateFlagBits, VkPipelineLayoutCreateFlags> PipelineLayoutCreateFlags;

  inline PipelineLayoutCreateFlags operator|( PipelineLayoutCreateFlagBits bit0, PipelineLayoutCreateFlagBits bit1 )
  {
    return PipelineLayoutCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineCacheCreateFlagBits
  {
  };

  typedef Flags<PipelineCacheCreateFlagBits, VkPipelineCacheCreateFlags> PipelineCacheCreateFlags;

  inline PipelineCacheCreateFlags operator|( PipelineCacheCreateFlagBits bit0, PipelineCacheCreateFlagBits bit1 )
  {
    return PipelineCacheCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineDepthStencilStateCreateFlagBits
  {
  };

  typedef Flags<PipelineDepthStencilStateCreateFlagBits, VkPipelineDepthStencilStateCreateFlags> PipelineDepthStencilStateCreateFlags;

  inline PipelineDepthStencilStateCreateFlags operator|( PipelineDepthStencilStateCreateFlagBits bit0, PipelineDepthStencilStateCreateFlagBits bit1 )
  {
    return PipelineDepthStencilStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineDynamicStateCreateFlagBits
  {
  };

  typedef Flags<PipelineDynamicStateCreateFlagBits, VkPipelineDynamicStateCreateFlags> PipelineDynamicStateCreateFlags;

  inline PipelineDynamicStateCreateFlags operator|( PipelineDynamicStateCreateFlagBits bit0, PipelineDynamicStateCreateFlagBits bit1 )
  {
    return PipelineDynamicStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineColorBlendStateCreateFlagBits
  {
  };

  typedef Flags<PipelineColorBlendStateCreateFlagBits, VkPipelineColorBlendStateCreateFlags> PipelineColorBlendStateCreateFlags;

  inline PipelineColorBlendStateCreateFlags operator|( PipelineColorBlendStateCreateFlagBits bit0, PipelineColorBlendStateCreateFlagBits bit1 )
  {
    return PipelineColorBlendStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineMultisampleStateCreateFlagBits
  {
  };

  typedef Flags<PipelineMultisampleStateCreateFlagBits, VkPipelineMultisampleStateCreateFlags> PipelineMultisampleStateCreateFlags;

  inline PipelineMultisampleStateCreateFlags operator|( PipelineMultisampleStateCreateFlagBits bit0, PipelineMultisampleStateCreateFlagBits bit1 )
  {
    return PipelineMultisampleStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineRasterizationStateCreateFlagBits
  {
  };

  typedef Flags<PipelineRasterizationStateCreateFlagBits, VkPipelineRasterizationStateCreateFlags> PipelineRasterizationStateCreateFlags;

  inline PipelineRasterizationStateCreateFlags operator|( PipelineRasterizationStateCreateFlagBits bit0, PipelineRasterizationStateCreateFlagBits bit1 )
  {
    return PipelineRasterizationStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineViewportStateCreateFlagBits
  {
  };

  typedef Flags<PipelineViewportStateCreateFlagBits, VkPipelineViewportStateCreateFlags> PipelineViewportStateCreateFlags;

  inline PipelineViewportStateCreateFlags operator|( PipelineViewportStateCreateFlagBits bit0, PipelineViewportStateCreateFlagBits bit1 )
  {
    return PipelineViewportStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineTessellationStateCreateFlagBits
  {
  };

  typedef Flags<PipelineTessellationStateCreateFlagBits, VkPipelineTessellationStateCreateFlags> PipelineTessellationStateCreateFlags;

  inline PipelineTessellationStateCreateFlags operator|( PipelineTessellationStateCreateFlagBits bit0, PipelineTessellationStateCreateFlagBits bit1 )
  {
    return PipelineTessellationStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineInputAssemblyStateCreateFlagBits
  {
  };

  typedef Flags<PipelineInputAssemblyStateCreateFlagBits, VkPipelineInputAssemblyStateCreateFlags> PipelineInputAssemblyStateCreateFlags;

  inline PipelineInputAssemblyStateCreateFlags operator|( PipelineInputAssemblyStateCreateFlagBits bit0, PipelineInputAssemblyStateCreateFlagBits bit1 )
  {
    return PipelineInputAssemblyStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineVertexInputStateCreateFlagBits
  {
  };

  typedef Flags<PipelineVertexInputStateCreateFlagBits, VkPipelineVertexInputStateCreateFlags> PipelineVertexInputStateCreateFlags;

  inline PipelineVertexInputStateCreateFlags operator|( PipelineVertexInputStateCreateFlagBits bit0, PipelineVertexInputStateCreateFlagBits bit1 )
  {
    return PipelineVertexInputStateCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineShaderStageCreateFlagBits
  {
  };

  typedef Flags<PipelineShaderStageCreateFlagBits, VkPipelineShaderStageCreateFlags> PipelineShaderStageCreateFlags;

  inline PipelineShaderStageCreateFlags operator|( PipelineShaderStageCreateFlagBits bit0, PipelineShaderStageCreateFlagBits bit1 )
  {
    return PipelineShaderStageCreateFlags( bit0 ) | bit1;
  }

  enum class DescriptorSetLayoutCreateFlagBits
  {
  };

  typedef Flags<DescriptorSetLayoutCreateFlagBits, VkDescriptorSetLayoutCreateFlags> DescriptorSetLayoutCreateFlags;

  inline DescriptorSetLayoutCreateFlags operator|( DescriptorSetLayoutCreateFlagBits bit0, DescriptorSetLayoutCreateFlagBits bit1 )
  {
    return DescriptorSetLayoutCreateFlags( bit0 ) | bit1;
  }

  enum class BufferViewCreateFlagBits
  {
  };

  typedef Flags<BufferViewCreateFlagBits, VkBufferViewCreateFlags> BufferViewCreateFlags;

  inline BufferViewCreateFlags operator|( BufferViewCreateFlagBits bit0, BufferViewCreateFlagBits bit1 )
  {
    return BufferViewCreateFlags( bit0 ) | bit1;
  }

  enum class InstanceCreateFlagBits
  {
  };

  typedef Flags<InstanceCreateFlagBits, VkInstanceCreateFlags> InstanceCreateFlags;

  inline InstanceCreateFlags operator|( InstanceCreateFlagBits bit0, InstanceCreateFlagBits bit1 )
  {
    return InstanceCreateFlags( bit0 ) | bit1;
  }

  enum class DeviceCreateFlagBits
  {
  };

  typedef Flags<DeviceCreateFlagBits, VkDeviceCreateFlags> DeviceCreateFlags;

  inline DeviceCreateFlags operator|( DeviceCreateFlagBits bit0, DeviceCreateFlagBits bit1 )
  {
    return DeviceCreateFlags( bit0 ) | bit1;
  }

  enum class DeviceQueueCreateFlagBits
  {
  };

  typedef Flags<DeviceQueueCreateFlagBits, VkDeviceQueueCreateFlags> DeviceQueueCreateFlags;

  inline DeviceQueueCreateFlags operator|( DeviceQueueCreateFlagBits bit0, DeviceQueueCreateFlagBits bit1 )
  {
    return DeviceQueueCreateFlags( bit0 ) | bit1;
  }

  enum class ImageViewCreateFlagBits
  {
  };

  typedef Flags<ImageViewCreateFlagBits, VkImageViewCreateFlags> ImageViewCreateFlags;

  inline ImageViewCreateFlags operator|( ImageViewCreateFlagBits bit0, ImageViewCreateFlagBits bit1 )
  {
    return ImageViewCreateFlags( bit0 ) | bit1;
  }

  enum class SemaphoreCreateFlagBits
  {
  };

  typedef Flags<SemaphoreCreateFlagBits, VkSemaphoreCreateFlags> SemaphoreCreateFlags;

  inline SemaphoreCreateFlags operator|( SemaphoreCreateFlagBits bit0, SemaphoreCreateFlagBits bit1 )
  {
    return SemaphoreCreateFlags( bit0 ) | bit1;
  }

  enum class ShaderModuleCreateFlagBits
  {
  };

  typedef Flags<ShaderModuleCreateFlagBits, VkShaderModuleCreateFlags> ShaderModuleCreateFlags;

  inline ShaderModuleCreateFlags operator|( ShaderModuleCreateFlagBits bit0, ShaderModuleCreateFlagBits bit1 )
  {
    return ShaderModuleCreateFlags( bit0 ) | bit1;
  }

  enum class EventCreateFlagBits
  {
  };

  typedef Flags<EventCreateFlagBits, VkEventCreateFlags> EventCreateFlags;

  inline EventCreateFlags operator|( EventCreateFlagBits bit0, EventCreateFlagBits bit1 )
  {
    return EventCreateFlags( bit0 ) | bit1;
  }

  enum class MemoryMapFlagBits
  {
  };

  typedef Flags<MemoryMapFlagBits, VkMemoryMapFlags> MemoryMapFlags;

  inline MemoryMapFlags operator|( MemoryMapFlagBits bit0, MemoryMapFlagBits bit1 )
  {
    return MemoryMapFlags( bit0 ) | bit1;
  }

  enum class SubpassDescriptionFlagBits
  {
  };

  typedef Flags<SubpassDescriptionFlagBits, VkSubpassDescriptionFlags> SubpassDescriptionFlags;

  inline SubpassDescriptionFlags operator|( SubpassDescriptionFlagBits bit0, SubpassDescriptionFlagBits bit1 )
  {
    return SubpassDescriptionFlags( bit0 ) | bit1;
  }

  enum class DescriptorPoolResetFlagBits
  {
  };

  typedef Flags<DescriptorPoolResetFlagBits, VkDescriptorPoolResetFlags> DescriptorPoolResetFlags;

  inline DescriptorPoolResetFlags operator|( DescriptorPoolResetFlagBits bit0, DescriptorPoolResetFlagBits bit1 )
  {
    return DescriptorPoolResetFlags( bit0 ) | bit1;
  }

  enum class SwapchainCreateFlagBitsKHR
  {
  };

  typedef Flags<SwapchainCreateFlagBitsKHR, VkSwapchainCreateFlagsKHR> SwapchainCreateFlagsKHR;

  inline SwapchainCreateFlagsKHR operator|( SwapchainCreateFlagBitsKHR bit0, SwapchainCreateFlagBitsKHR bit1 )
  {
    return SwapchainCreateFlagsKHR( bit0 ) | bit1;
  }

  enum class DisplayModeCreateFlagBitsKHR
  {
  };

  typedef Flags<DisplayModeCreateFlagBitsKHR, VkDisplayModeCreateFlagsKHR> DisplayModeCreateFlagsKHR;

  inline DisplayModeCreateFlagsKHR operator|( DisplayModeCreateFlagBitsKHR bit0, DisplayModeCreateFlagBitsKHR bit1 )
  {
    return DisplayModeCreateFlagsKHR( bit0 ) | bit1;
  }

  enum class DisplaySurfaceCreateFlagBitsKHR
  {
  };

  typedef Flags<DisplaySurfaceCreateFlagBitsKHR, VkDisplaySurfaceCreateFlagsKHR> DisplaySurfaceCreateFlagsKHR;

  inline DisplaySurfaceCreateFlagsKHR operator|( DisplaySurfaceCreateFlagBitsKHR bit0, DisplaySurfaceCreateFlagBitsKHR bit1 )
  {
    return DisplaySurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  enum class AndroidSurfaceCreateFlagBitsKHR
  {
  };
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  typedef Flags<AndroidSurfaceCreateFlagBitsKHR, VkAndroidSurfaceCreateFlagsKHR> AndroidSurfaceCreateFlagsKHR;

  inline AndroidSurfaceCreateFlagsKHR operator|( AndroidSurfaceCreateFlagBitsKHR bit0, AndroidSurfaceCreateFlagBitsKHR bit1 )
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
  typedef Flags<MirSurfaceCreateFlagBitsKHR, VkMirSurfaceCreateFlagsKHR> MirSurfaceCreateFlagsKHR;

  inline MirSurfaceCreateFlagsKHR operator|( MirSurfaceCreateFlagBitsKHR bit0, MirSurfaceCreateFlagBitsKHR bit1 )
  {
    return MirSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }
#endif /*VK_USE_PLATFORM_MIR_KHR*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  enum class WaylandSurfaceCreateFlagBitsKHR
  {
  };
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  typedef Flags<WaylandSurfaceCreateFlagBitsKHR, VkWaylandSurfaceCreateFlagsKHR> WaylandSurfaceCreateFlagsKHR;

  inline WaylandSurfaceCreateFlagsKHR operator|( WaylandSurfaceCreateFlagBitsKHR bit0, WaylandSurfaceCreateFlagBitsKHR bit1 )
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
  typedef Flags<Win32SurfaceCreateFlagBitsKHR, VkWin32SurfaceCreateFlagsKHR> Win32SurfaceCreateFlagsKHR;

  inline Win32SurfaceCreateFlagsKHR operator|( Win32SurfaceCreateFlagBitsKHR bit0, Win32SurfaceCreateFlagBitsKHR bit1 )
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
  typedef Flags<XlibSurfaceCreateFlagBitsKHR, VkXlibSurfaceCreateFlagsKHR> XlibSurfaceCreateFlagsKHR;

  inline XlibSurfaceCreateFlagsKHR operator|( XlibSurfaceCreateFlagBitsKHR bit0, XlibSurfaceCreateFlagBitsKHR bit1 )
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
  typedef Flags<XcbSurfaceCreateFlagBitsKHR, VkXcbSurfaceCreateFlagsKHR> XcbSurfaceCreateFlagsKHR;

  inline XcbSurfaceCreateFlagsKHR operator|( XcbSurfaceCreateFlagBitsKHR bit0, XcbSurfaceCreateFlagBitsKHR bit1 )
  {
    return XcbSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }
#endif /*VK_USE_PLATFORM_XCB_KHR*/

  class DeviceMemory
  {
  public:
    DeviceMemory()
      : m_deviceMemory(VK_NULL_HANDLE)
    {}

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    DeviceMemory(VkDeviceMemory deviceMemory)
       : m_deviceMemory(deviceMemory)
    {}

    DeviceMemory& operator=(VkDeviceMemory deviceMemory)
    {
      m_deviceMemory = deviceMemory;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    CommandPool(VkCommandPool commandPool)
       : m_commandPool(commandPool)
    {}

    CommandPool& operator=(VkCommandPool commandPool)
    {
      m_commandPool = commandPool;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    Buffer(VkBuffer buffer)
       : m_buffer(buffer)
    {}

    Buffer& operator=(VkBuffer buffer)
    {
      m_buffer = buffer;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    BufferView(VkBufferView bufferView)
       : m_bufferView(bufferView)
    {}

    BufferView& operator=(VkBufferView bufferView)
    {
      m_bufferView = bufferView;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    Image(VkImage image)
       : m_image(image)
    {}

    Image& operator=(VkImage image)
    {
      m_image = image;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    ImageView(VkImageView imageView)
       : m_imageView(imageView)
    {}

    ImageView& operator=(VkImageView imageView)
    {
      m_imageView = imageView;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    ShaderModule(VkShaderModule shaderModule)
       : m_shaderModule(shaderModule)
    {}

    ShaderModule& operator=(VkShaderModule shaderModule)
    {
      m_shaderModule = shaderModule;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    Pipeline(VkPipeline pipeline)
       : m_pipeline(pipeline)
    {}

    Pipeline& operator=(VkPipeline pipeline)
    {
      m_pipeline = pipeline;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    PipelineLayout(VkPipelineLayout pipelineLayout)
       : m_pipelineLayout(pipelineLayout)
    {}

    PipelineLayout& operator=(VkPipelineLayout pipelineLayout)
    {
      m_pipelineLayout = pipelineLayout;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    Sampler(VkSampler sampler)
       : m_sampler(sampler)
    {}

    Sampler& operator=(VkSampler sampler)
    {
      m_sampler = sampler;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    DescriptorSet(VkDescriptorSet descriptorSet)
       : m_descriptorSet(descriptorSet)
    {}

    DescriptorSet& operator=(VkDescriptorSet descriptorSet)
    {
      m_descriptorSet = descriptorSet;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    DescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout)
       : m_descriptorSetLayout(descriptorSetLayout)
    {}

    DescriptorSetLayout& operator=(VkDescriptorSetLayout descriptorSetLayout)
    {
      m_descriptorSetLayout = descriptorSetLayout;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    DescriptorPool(VkDescriptorPool descriptorPool)
       : m_descriptorPool(descriptorPool)
    {}

    DescriptorPool& operator=(VkDescriptorPool descriptorPool)
    {
      m_descriptorPool = descriptorPool;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    Fence(VkFence fence)
       : m_fence(fence)
    {}

    Fence& operator=(VkFence fence)
    {
      m_fence = fence;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    Semaphore(VkSemaphore semaphore)
       : m_semaphore(semaphore)
    {}

    Semaphore& operator=(VkSemaphore semaphore)
    {
      m_semaphore = semaphore;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    Event(VkEvent event)
       : m_event(event)
    {}

    Event& operator=(VkEvent event)
    {
      m_event = event;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    QueryPool(VkQueryPool queryPool)
       : m_queryPool(queryPool)
    {}

    QueryPool& operator=(VkQueryPool queryPool)
    {
      m_queryPool = queryPool;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    Framebuffer(VkFramebuffer framebuffer)
       : m_framebuffer(framebuffer)
    {}

    Framebuffer& operator=(VkFramebuffer framebuffer)
    {
      m_framebuffer = framebuffer;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    RenderPass(VkRenderPass renderPass)
       : m_renderPass(renderPass)
    {}

    RenderPass& operator=(VkRenderPass renderPass)
    {
      m_renderPass = renderPass;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    PipelineCache(VkPipelineCache pipelineCache)
       : m_pipelineCache(pipelineCache)
    {}

    PipelineCache& operator=(VkPipelineCache pipelineCache)
    {
      m_pipelineCache = pipelineCache;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

  class DisplayKHR
  {
  public:
    DisplayKHR()
      : m_displayKHR(VK_NULL_HANDLE)
    {}

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    DisplayKHR(VkDisplayKHR displayKHR)
       : m_displayKHR(displayKHR)
    {}

    DisplayKHR& operator=(VkDisplayKHR displayKHR)
    {
      m_displayKHR = displayKHR;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    DisplayModeKHR(VkDisplayModeKHR displayModeKHR)
       : m_displayModeKHR(displayModeKHR)
    {}

    DisplayModeKHR& operator=(VkDisplayModeKHR displayModeKHR)
    {
      m_displayModeKHR = displayModeKHR;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    SurfaceKHR(VkSurfaceKHR surfaceKHR)
       : m_surfaceKHR(surfaceKHR)
    {}

    SurfaceKHR& operator=(VkSurfaceKHR surfaceKHR)
    {
      m_surfaceKHR = surfaceKHR;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    SwapchainKHR(VkSwapchainKHR swapchainKHR)
       : m_swapchainKHR(swapchainKHR)
    {}

    SwapchainKHR& operator=(VkSwapchainKHR swapchainKHR)
    {
      m_swapchainKHR = swapchainKHR;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    DebugReportCallbackEXT(VkDebugReportCallbackEXT debugReportCallbackEXT)
       : m_debugReportCallbackEXT(debugReportCallbackEXT)
    {}

    DebugReportCallbackEXT& operator=(VkDebugReportCallbackEXT debugReportCallbackEXT)
    {
      m_debugReportCallbackEXT = debugReportCallbackEXT;
      return *this;
    }
#endif

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

  class Offset2D
  {
  public:
    Offset2D()
      : Offset2D( 0, 0 )
    {}

    Offset2D( int32_t x, int32_t y)
    {
      m_offset2D.x = x;
      m_offset2D.y = y;
    }

    Offset2D(VkOffset2D const & rhs)
      : m_offset2D(rhs)
    {
    }

    Offset2D& operator=(VkOffset2D const & rhs)
    {
      m_offset2D = rhs;
      return *this;
    }

    const int32_t& x() const
    {
      return m_offset2D.x;
    }

    int32_t& x()
    {
      return m_offset2D.x;
    }

    Offset2D& x( int32_t x )
    {
      m_offset2D.x = x;
      return *this;
    }

    const int32_t& y() const
    {
      return m_offset2D.y;
    }

    int32_t& y()
    {
      return m_offset2D.y;
    }

    Offset2D& y( int32_t y )
    {
      m_offset2D.y = y;
      return *this;
    }

    static Offset2D& null()
    {
      return *((Offset2D*)(nullptr));
    }

    operator const VkOffset2D&() const
    {
      return m_offset2D;
    }

  private:
    VkOffset2D m_offset2D;
  };
  static_assert( sizeof( Offset2D ) == sizeof( VkOffset2D ), "struct and wrapper have different size!" );

  class Offset3D
  {
  public:
    Offset3D()
      : Offset3D( 0, 0, 0 )
    {}

    Offset3D( int32_t x, int32_t y, int32_t z)
    {
      m_offset3D.x = x;
      m_offset3D.y = y;
      m_offset3D.z = z;
    }

    Offset3D(VkOffset3D const & rhs)
      : m_offset3D(rhs)
    {
    }

    Offset3D& operator=(VkOffset3D const & rhs)
    {
      m_offset3D = rhs;
      return *this;
    }

    const int32_t& x() const
    {
      return m_offset3D.x;
    }

    int32_t& x()
    {
      return m_offset3D.x;
    }

    Offset3D& x( int32_t x )
    {
      m_offset3D.x = x;
      return *this;
    }

    const int32_t& y() const
    {
      return m_offset3D.y;
    }

    int32_t& y()
    {
      return m_offset3D.y;
    }

    Offset3D& y( int32_t y )
    {
      m_offset3D.y = y;
      return *this;
    }

    const int32_t& z() const
    {
      return m_offset3D.z;
    }

    int32_t& z()
    {
      return m_offset3D.z;
    }

    Offset3D& z( int32_t z )
    {
      m_offset3D.z = z;
      return *this;
    }

    static Offset3D& null()
    {
      return *((Offset3D*)(nullptr));
    }

    operator const VkOffset3D&() const
    {
      return m_offset3D;
    }

  private:
    VkOffset3D m_offset3D;
  };
  static_assert( sizeof( Offset3D ) == sizeof( VkOffset3D ), "struct and wrapper have different size!" );

  class Extent2D
  {
  public:
    Extent2D()
      : Extent2D( 0, 0 )
    {}

    Extent2D( uint32_t width, uint32_t height)
    {
      m_extent2D.width = width;
      m_extent2D.height = height;
    }

    Extent2D(VkExtent2D const & rhs)
      : m_extent2D(rhs)
    {
    }

    Extent2D& operator=(VkExtent2D const & rhs)
    {
      m_extent2D = rhs;
      return *this;
    }

    const uint32_t& width() const
    {
      return m_extent2D.width;
    }

    uint32_t& width()
    {
      return m_extent2D.width;
    }

    Extent2D& width( uint32_t width )
    {
      m_extent2D.width = width;
      return *this;
    }

    const uint32_t& height() const
    {
      return m_extent2D.height;
    }

    uint32_t& height()
    {
      return m_extent2D.height;
    }

    Extent2D& height( uint32_t height )
    {
      m_extent2D.height = height;
      return *this;
    }

    static Extent2D& null()
    {
      return *((Extent2D*)(nullptr));
    }

    operator const VkExtent2D&() const
    {
      return m_extent2D;
    }

  private:
    VkExtent2D m_extent2D;
  };
  static_assert( sizeof( Extent2D ) == sizeof( VkExtent2D ), "struct and wrapper have different size!" );

  class Extent3D
  {
  public:
    Extent3D()
      : Extent3D( 0, 0, 0 )
    {}

    Extent3D( uint32_t width, uint32_t height, uint32_t depth)
    {
      m_extent3D.width = width;
      m_extent3D.height = height;
      m_extent3D.depth = depth;
    }

    Extent3D(VkExtent3D const & rhs)
      : m_extent3D(rhs)
    {
    }

    Extent3D& operator=(VkExtent3D const & rhs)
    {
      m_extent3D = rhs;
      return *this;
    }

    const uint32_t& width() const
    {
      return m_extent3D.width;
    }

    uint32_t& width()
    {
      return m_extent3D.width;
    }

    Extent3D& width( uint32_t width )
    {
      m_extent3D.width = width;
      return *this;
    }

    const uint32_t& height() const
    {
      return m_extent3D.height;
    }

    uint32_t& height()
    {
      return m_extent3D.height;
    }

    Extent3D& height( uint32_t height )
    {
      m_extent3D.height = height;
      return *this;
    }

    const uint32_t& depth() const
    {
      return m_extent3D.depth;
    }

    uint32_t& depth()
    {
      return m_extent3D.depth;
    }

    Extent3D& depth( uint32_t depth )
    {
      m_extent3D.depth = depth;
      return *this;
    }

    static Extent3D& null()
    {
      return *((Extent3D*)(nullptr));
    }

    operator const VkExtent3D&() const
    {
      return m_extent3D;
    }

  private:
    VkExtent3D m_extent3D;
  };
  static_assert( sizeof( Extent3D ) == sizeof( VkExtent3D ), "struct and wrapper have different size!" );

  class Viewport
  {
  public:
    Viewport()
      : Viewport( 0, 0, 0, 0, 0, 0 )
    {}

    Viewport( float x, float y, float width, float height, float minDepth, float maxDepth)
    {
      m_viewport.x = x;
      m_viewport.y = y;
      m_viewport.width = width;
      m_viewport.height = height;
      m_viewport.minDepth = minDepth;
      m_viewport.maxDepth = maxDepth;
    }

    Viewport(VkViewport const & rhs)
      : m_viewport(rhs)
    {
    }

    Viewport& operator=(VkViewport const & rhs)
    {
      m_viewport = rhs;
      return *this;
    }

    const float& x() const
    {
      return m_viewport.x;
    }

    float& x()
    {
      return m_viewport.x;
    }

    Viewport& x( float x )
    {
      m_viewport.x = x;
      return *this;
    }

    const float& y() const
    {
      return m_viewport.y;
    }

    float& y()
    {
      return m_viewport.y;
    }

    Viewport& y( float y )
    {
      m_viewport.y = y;
      return *this;
    }

    const float& width() const
    {
      return m_viewport.width;
    }

    float& width()
    {
      return m_viewport.width;
    }

    Viewport& width( float width )
    {
      m_viewport.width = width;
      return *this;
    }

    const float& height() const
    {
      return m_viewport.height;
    }

    float& height()
    {
      return m_viewport.height;
    }

    Viewport& height( float height )
    {
      m_viewport.height = height;
      return *this;
    }

    const float& minDepth() const
    {
      return m_viewport.minDepth;
    }

    float& minDepth()
    {
      return m_viewport.minDepth;
    }

    Viewport& minDepth( float minDepth )
    {
      m_viewport.minDepth = minDepth;
      return *this;
    }

    const float& maxDepth() const
    {
      return m_viewport.maxDepth;
    }

    float& maxDepth()
    {
      return m_viewport.maxDepth;
    }

    Viewport& maxDepth( float maxDepth )
    {
      m_viewport.maxDepth = maxDepth;
      return *this;
    }

    static Viewport& null()
    {
      return *((Viewport*)(nullptr));
    }

    operator const VkViewport&() const
    {
      return m_viewport;
    }

  private:
    VkViewport m_viewport;
  };
  static_assert( sizeof( Viewport ) == sizeof( VkViewport ), "struct and wrapper have different size!" );

  class Rect2D
  {
  public:
    Rect2D()
      : Rect2D( Offset2D(), Extent2D() )
    {}

    Rect2D( Offset2D offset, Extent2D extent)
    {
      m_rect2D.offset = static_cast<VkOffset2D>( offset );
      m_rect2D.extent = static_cast<VkExtent2D>( extent );
    }

    Rect2D(VkRect2D const & rhs)
      : m_rect2D(rhs)
    {
    }

    Rect2D& operator=(VkRect2D const & rhs)
    {
      m_rect2D = rhs;
      return *this;
    }

    const Offset2D& offset() const
    {
      return reinterpret_cast<const Offset2D&>( m_rect2D.offset );
    }

    Offset2D& offset()
    {
      return reinterpret_cast<Offset2D&>( m_rect2D.offset );
    }

    Rect2D& offset( Offset2D offset )
    {
      m_rect2D.offset = static_cast<VkOffset2D>( offset );
      return *this;
    }

    const Extent2D& extent() const
    {
      return reinterpret_cast<const Extent2D&>( m_rect2D.extent );
    }

    Extent2D& extent()
    {
      return reinterpret_cast<Extent2D&>( m_rect2D.extent );
    }

    Rect2D& extent( Extent2D extent )
    {
      m_rect2D.extent = static_cast<VkExtent2D>( extent );
      return *this;
    }

    static Rect2D& null()
    {
      return *((Rect2D*)(nullptr));
    }

    operator const VkRect2D&() const
    {
      return m_rect2D;
    }

  private:
    VkRect2D m_rect2D;
  };
  static_assert( sizeof( Rect2D ) == sizeof( VkRect2D ), "struct and wrapper have different size!" );

  class ClearRect
  {
  public:
    ClearRect()
      : ClearRect( Rect2D(), 0, 0 )
    {}

    ClearRect( Rect2D rect, uint32_t baseArrayLayer, uint32_t layerCount)
    {
      m_clearRect.rect = static_cast<VkRect2D>( rect );
      m_clearRect.baseArrayLayer = baseArrayLayer;
      m_clearRect.layerCount = layerCount;
    }

    ClearRect(VkClearRect const & rhs)
      : m_clearRect(rhs)
    {
    }

    ClearRect& operator=(VkClearRect const & rhs)
    {
      m_clearRect = rhs;
      return *this;
    }

    const Rect2D& rect() const
    {
      return reinterpret_cast<const Rect2D&>( m_clearRect.rect );
    }

    Rect2D& rect()
    {
      return reinterpret_cast<Rect2D&>( m_clearRect.rect );
    }

    ClearRect& rect( Rect2D rect )
    {
      m_clearRect.rect = static_cast<VkRect2D>( rect );
      return *this;
    }

    const uint32_t& baseArrayLayer() const
    {
      return m_clearRect.baseArrayLayer;
    }

    uint32_t& baseArrayLayer()
    {
      return m_clearRect.baseArrayLayer;
    }

    ClearRect& baseArrayLayer( uint32_t baseArrayLayer )
    {
      m_clearRect.baseArrayLayer = baseArrayLayer;
      return *this;
    }

    const uint32_t& layerCount() const
    {
      return m_clearRect.layerCount;
    }

    uint32_t& layerCount()
    {
      return m_clearRect.layerCount;
    }

    ClearRect& layerCount( uint32_t layerCount )
    {
      m_clearRect.layerCount = layerCount;
      return *this;
    }

    static ClearRect& null()
    {
      return *((ClearRect*)(nullptr));
    }

    operator const VkClearRect&() const
    {
      return m_clearRect;
    }

  private:
    VkClearRect m_clearRect;
  };
  static_assert( sizeof( ClearRect ) == sizeof( VkClearRect ), "struct and wrapper have different size!" );

  class ExtensionProperties
  {
  public:
    const char* extensionName() const
    {
      return reinterpret_cast<const char*>( m_extensionProperties.extensionName );
    }

    const uint32_t& specVersion() const
    {
      return m_extensionProperties.specVersion;
    }

    static ExtensionProperties& null()
    {
      return *((ExtensionProperties*)(nullptr));
    }

    operator const VkExtensionProperties&() const
    {
      return m_extensionProperties;
    }

  private:
    VkExtensionProperties m_extensionProperties;
  };
  static_assert( sizeof( ExtensionProperties ) == sizeof( VkExtensionProperties ), "struct and wrapper have different size!" );

  class LayerProperties
  {
  public:
    const char* layerName() const
    {
      return reinterpret_cast<const char*>( m_layerProperties.layerName );
    }

    const uint32_t& specVersion() const
    {
      return m_layerProperties.specVersion;
    }

    const uint32_t& implementationVersion() const
    {
      return m_layerProperties.implementationVersion;
    }

    const char* description() const
    {
      return reinterpret_cast<const char*>( m_layerProperties.description );
    }

    static LayerProperties& null()
    {
      return *((LayerProperties*)(nullptr));
    }

    operator const VkLayerProperties&() const
    {
      return m_layerProperties;
    }

  private:
    VkLayerProperties m_layerProperties;
  };
  static_assert( sizeof( LayerProperties ) == sizeof( VkLayerProperties ), "struct and wrapper have different size!" );

  class AllocationCallbacks
  {
  public:
    AllocationCallbacks( void* pUserData, PFN_vkAllocationFunction pfnAllocation, PFN_vkReallocationFunction pfnReallocation, PFN_vkFreeFunction pfnFree, PFN_vkInternalAllocationNotification pfnInternalAllocation, PFN_vkInternalFreeNotification pfnInternalFree)
    {
      m_allocationCallbacks.pUserData = pUserData;
      m_allocationCallbacks.pfnAllocation = pfnAllocation;
      m_allocationCallbacks.pfnReallocation = pfnReallocation;
      m_allocationCallbacks.pfnFree = pfnFree;
      m_allocationCallbacks.pfnInternalAllocation = pfnInternalAllocation;
      m_allocationCallbacks.pfnInternalFree = pfnInternalFree;
    }

    AllocationCallbacks(VkAllocationCallbacks const & rhs)
      : m_allocationCallbacks(rhs)
    {
    }

    AllocationCallbacks& operator=(VkAllocationCallbacks const & rhs)
    {
      m_allocationCallbacks = rhs;
      return *this;
    }

    const void* pUserData() const
    {
      return reinterpret_cast<const void*>( m_allocationCallbacks.pUserData );
    }

    void* pUserData()
    {
      return reinterpret_cast<void*>( m_allocationCallbacks.pUserData );
    }

    AllocationCallbacks& pUserData( void* pUserData )
    {
      m_allocationCallbacks.pUserData = pUserData;
      return *this;
    }

    const PFN_vkAllocationFunction& pfnAllocation() const
    {
      return m_allocationCallbacks.pfnAllocation;
    }

    PFN_vkAllocationFunction& pfnAllocation()
    {
      return m_allocationCallbacks.pfnAllocation;
    }

    AllocationCallbacks& pfnAllocation( PFN_vkAllocationFunction pfnAllocation )
    {
      m_allocationCallbacks.pfnAllocation = pfnAllocation;
      return *this;
    }

    const PFN_vkReallocationFunction& pfnReallocation() const
    {
      return m_allocationCallbacks.pfnReallocation;
    }

    PFN_vkReallocationFunction& pfnReallocation()
    {
      return m_allocationCallbacks.pfnReallocation;
    }

    AllocationCallbacks& pfnReallocation( PFN_vkReallocationFunction pfnReallocation )
    {
      m_allocationCallbacks.pfnReallocation = pfnReallocation;
      return *this;
    }

    const PFN_vkFreeFunction& pfnFree() const
    {
      return m_allocationCallbacks.pfnFree;
    }

    PFN_vkFreeFunction& pfnFree()
    {
      return m_allocationCallbacks.pfnFree;
    }

    AllocationCallbacks& pfnFree( PFN_vkFreeFunction pfnFree )
    {
      m_allocationCallbacks.pfnFree = pfnFree;
      return *this;
    }

    const PFN_vkInternalAllocationNotification& pfnInternalAllocation() const
    {
      return m_allocationCallbacks.pfnInternalAllocation;
    }

    PFN_vkInternalAllocationNotification& pfnInternalAllocation()
    {
      return m_allocationCallbacks.pfnInternalAllocation;
    }

    AllocationCallbacks& pfnInternalAllocation( PFN_vkInternalAllocationNotification pfnInternalAllocation )
    {
      m_allocationCallbacks.pfnInternalAllocation = pfnInternalAllocation;
      return *this;
    }

    const PFN_vkInternalFreeNotification& pfnInternalFree() const
    {
      return m_allocationCallbacks.pfnInternalFree;
    }

    PFN_vkInternalFreeNotification& pfnInternalFree()
    {
      return m_allocationCallbacks.pfnInternalFree;
    }

    AllocationCallbacks& pfnInternalFree( PFN_vkInternalFreeNotification pfnInternalFree )
    {
      m_allocationCallbacks.pfnInternalFree = pfnInternalFree;
      return *this;
    }

    static AllocationCallbacks& null()
    {
      return *((AllocationCallbacks*)(nullptr));
    }

    operator const VkAllocationCallbacks&() const
    {
      return m_allocationCallbacks;
    }

  private:
    VkAllocationCallbacks m_allocationCallbacks;
  };
  static_assert( sizeof( AllocationCallbacks ) == sizeof( VkAllocationCallbacks ), "struct and wrapper have different size!" );

  class MemoryRequirements
  {
  public:
    const DeviceSize& size() const
    {
      return m_memoryRequirements.size;
    }

    const DeviceSize& alignment() const
    {
      return m_memoryRequirements.alignment;
    }

    const uint32_t& memoryTypeBits() const
    {
      return m_memoryRequirements.memoryTypeBits;
    }

    static MemoryRequirements& null()
    {
      return *((MemoryRequirements*)(nullptr));
    }

    operator const VkMemoryRequirements&() const
    {
      return m_memoryRequirements;
    }

  private:
    VkMemoryRequirements m_memoryRequirements;
  };
  static_assert( sizeof( MemoryRequirements ) == sizeof( VkMemoryRequirements ), "struct and wrapper have different size!" );

  class DescriptorBufferInfo
  {
  public:
    DescriptorBufferInfo()
      : DescriptorBufferInfo( Buffer(), 0, 0 )
    {}

    DescriptorBufferInfo( Buffer buffer, DeviceSize offset, DeviceSize range)
    {
      m_descriptorBufferInfo.buffer = static_cast<VkBuffer>( buffer );
      m_descriptorBufferInfo.offset = offset;
      m_descriptorBufferInfo.range = range;
    }

    DescriptorBufferInfo(VkDescriptorBufferInfo const & rhs)
      : m_descriptorBufferInfo(rhs)
    {
    }

    DescriptorBufferInfo& operator=(VkDescriptorBufferInfo const & rhs)
    {
      m_descriptorBufferInfo = rhs;
      return *this;
    }

    const Buffer& buffer() const
    {
      return reinterpret_cast<const Buffer&>( m_descriptorBufferInfo.buffer );
    }

    Buffer& buffer()
    {
      return reinterpret_cast<Buffer&>( m_descriptorBufferInfo.buffer );
    }

    DescriptorBufferInfo& buffer( Buffer buffer )
    {
      m_descriptorBufferInfo.buffer = static_cast<VkBuffer>( buffer );
      return *this;
    }

    const DeviceSize& offset() const
    {
      return m_descriptorBufferInfo.offset;
    }

    DeviceSize& offset()
    {
      return m_descriptorBufferInfo.offset;
    }

    DescriptorBufferInfo& offset( DeviceSize offset )
    {
      m_descriptorBufferInfo.offset = offset;
      return *this;
    }

    const DeviceSize& range() const
    {
      return m_descriptorBufferInfo.range;
    }

    DeviceSize& range()
    {
      return m_descriptorBufferInfo.range;
    }

    DescriptorBufferInfo& range( DeviceSize range )
    {
      m_descriptorBufferInfo.range = range;
      return *this;
    }

    static DescriptorBufferInfo& null()
    {
      return *((DescriptorBufferInfo*)(nullptr));
    }

    operator const VkDescriptorBufferInfo&() const
    {
      return m_descriptorBufferInfo;
    }

  private:
    VkDescriptorBufferInfo m_descriptorBufferInfo;
  };
  static_assert( sizeof( DescriptorBufferInfo ) == sizeof( VkDescriptorBufferInfo ), "struct and wrapper have different size!" );

  class SubresourceLayout
  {
  public:
    SubresourceLayout()
      : SubresourceLayout( 0, 0, 0, 0, 0 )
    {}

    SubresourceLayout( DeviceSize offset, DeviceSize size, DeviceSize rowPitch, DeviceSize arrayPitch, DeviceSize depthPitch)
    {
      m_subresourceLayout.offset = offset;
      m_subresourceLayout.size = size;
      m_subresourceLayout.rowPitch = rowPitch;
      m_subresourceLayout.arrayPitch = arrayPitch;
      m_subresourceLayout.depthPitch = depthPitch;
    }

    SubresourceLayout(VkSubresourceLayout const & rhs)
      : m_subresourceLayout(rhs)
    {
    }

    SubresourceLayout& operator=(VkSubresourceLayout const & rhs)
    {
      m_subresourceLayout = rhs;
      return *this;
    }

    const DeviceSize& offset() const
    {
      return m_subresourceLayout.offset;
    }

    DeviceSize& offset()
    {
      return m_subresourceLayout.offset;
    }

    SubresourceLayout& offset( DeviceSize offset )
    {
      m_subresourceLayout.offset = offset;
      return *this;
    }

    const DeviceSize& size() const
    {
      return m_subresourceLayout.size;
    }

    DeviceSize& size()
    {
      return m_subresourceLayout.size;
    }

    SubresourceLayout& size( DeviceSize size )
    {
      m_subresourceLayout.size = size;
      return *this;
    }

    const DeviceSize& rowPitch() const
    {
      return m_subresourceLayout.rowPitch;
    }

    DeviceSize& rowPitch()
    {
      return m_subresourceLayout.rowPitch;
    }

    SubresourceLayout& rowPitch( DeviceSize rowPitch )
    {
      m_subresourceLayout.rowPitch = rowPitch;
      return *this;
    }

    const DeviceSize& arrayPitch() const
    {
      return m_subresourceLayout.arrayPitch;
    }

    DeviceSize& arrayPitch()
    {
      return m_subresourceLayout.arrayPitch;
    }

    SubresourceLayout& arrayPitch( DeviceSize arrayPitch )
    {
      m_subresourceLayout.arrayPitch = arrayPitch;
      return *this;
    }

    const DeviceSize& depthPitch() const
    {
      return m_subresourceLayout.depthPitch;
    }

    DeviceSize& depthPitch()
    {
      return m_subresourceLayout.depthPitch;
    }

    SubresourceLayout& depthPitch( DeviceSize depthPitch )
    {
      m_subresourceLayout.depthPitch = depthPitch;
      return *this;
    }

    static SubresourceLayout& null()
    {
      return *((SubresourceLayout*)(nullptr));
    }

    operator const VkSubresourceLayout&() const
    {
      return m_subresourceLayout;
    }

  private:
    VkSubresourceLayout m_subresourceLayout;
  };
  static_assert( sizeof( SubresourceLayout ) == sizeof( VkSubresourceLayout ), "struct and wrapper have different size!" );

  class BufferCopy
  {
  public:
    BufferCopy()
      : BufferCopy( 0, 0, 0 )
    {}

    BufferCopy( DeviceSize srcOffset, DeviceSize dstOffset, DeviceSize size)
    {
      m_bufferCopy.srcOffset = srcOffset;
      m_bufferCopy.dstOffset = dstOffset;
      m_bufferCopy.size = size;
    }

    BufferCopy(VkBufferCopy const & rhs)
      : m_bufferCopy(rhs)
    {
    }

    BufferCopy& operator=(VkBufferCopy const & rhs)
    {
      m_bufferCopy = rhs;
      return *this;
    }

    const DeviceSize& srcOffset() const
    {
      return m_bufferCopy.srcOffset;
    }

    DeviceSize& srcOffset()
    {
      return m_bufferCopy.srcOffset;
    }

    BufferCopy& srcOffset( DeviceSize srcOffset )
    {
      m_bufferCopy.srcOffset = srcOffset;
      return *this;
    }

    const DeviceSize& dstOffset() const
    {
      return m_bufferCopy.dstOffset;
    }

    DeviceSize& dstOffset()
    {
      return m_bufferCopy.dstOffset;
    }

    BufferCopy& dstOffset( DeviceSize dstOffset )
    {
      m_bufferCopy.dstOffset = dstOffset;
      return *this;
    }

    const DeviceSize& size() const
    {
      return m_bufferCopy.size;
    }

    DeviceSize& size()
    {
      return m_bufferCopy.size;
    }

    BufferCopy& size( DeviceSize size )
    {
      m_bufferCopy.size = size;
      return *this;
    }

    static BufferCopy& null()
    {
      return *((BufferCopy*)(nullptr));
    }

    operator const VkBufferCopy&() const
    {
      return m_bufferCopy;
    }

  private:
    VkBufferCopy m_bufferCopy;
  };
  static_assert( sizeof( BufferCopy ) == sizeof( VkBufferCopy ), "struct and wrapper have different size!" );

  class SpecializationMapEntry
  {
  public:
    SpecializationMapEntry()
      : SpecializationMapEntry( 0, 0, 0 )
    {}

    SpecializationMapEntry( uint32_t constantID, uint32_t offset, size_t size)
    {
      m_specializationMapEntry.constantID = constantID;
      m_specializationMapEntry.offset = offset;
      m_specializationMapEntry.size = size;
    }

    SpecializationMapEntry(VkSpecializationMapEntry const & rhs)
      : m_specializationMapEntry(rhs)
    {
    }

    SpecializationMapEntry& operator=(VkSpecializationMapEntry const & rhs)
    {
      m_specializationMapEntry = rhs;
      return *this;
    }

    const uint32_t& constantID() const
    {
      return m_specializationMapEntry.constantID;
    }

    uint32_t& constantID()
    {
      return m_specializationMapEntry.constantID;
    }

    SpecializationMapEntry& constantID( uint32_t constantID )
    {
      m_specializationMapEntry.constantID = constantID;
      return *this;
    }

    const uint32_t& offset() const
    {
      return m_specializationMapEntry.offset;
    }

    uint32_t& offset()
    {
      return m_specializationMapEntry.offset;
    }

    SpecializationMapEntry& offset( uint32_t offset )
    {
      m_specializationMapEntry.offset = offset;
      return *this;
    }

    const size_t& size() const
    {
      return m_specializationMapEntry.size;
    }

    size_t& size()
    {
      return m_specializationMapEntry.size;
    }

    SpecializationMapEntry& size( size_t size )
    {
      m_specializationMapEntry.size = size;
      return *this;
    }

    static SpecializationMapEntry& null()
    {
      return *((SpecializationMapEntry*)(nullptr));
    }

    operator const VkSpecializationMapEntry&() const
    {
      return m_specializationMapEntry;
    }

  private:
    VkSpecializationMapEntry m_specializationMapEntry;
  };
  static_assert( sizeof( SpecializationMapEntry ) == sizeof( VkSpecializationMapEntry ), "struct and wrapper have different size!" );

  class SpecializationInfo
  {
  public:
    SpecializationInfo()
      : SpecializationInfo( 0, nullptr, 0, nullptr )
    {}

    SpecializationInfo( uint32_t mapEntryCount, const SpecializationMapEntry* pMapEntries, size_t dataSize, const void* pData)
    {
      m_specializationInfo.mapEntryCount = mapEntryCount;
      m_specializationInfo.pMapEntries = reinterpret_cast<const VkSpecializationMapEntry*>( pMapEntries );
      m_specializationInfo.dataSize = dataSize;
      m_specializationInfo.pData = pData;
    }

    SpecializationInfo(VkSpecializationInfo const & rhs)
      : m_specializationInfo(rhs)
    {
    }

    SpecializationInfo& operator=(VkSpecializationInfo const & rhs)
    {
      m_specializationInfo = rhs;
      return *this;
    }

    const uint32_t& mapEntryCount() const
    {
      return m_specializationInfo.mapEntryCount;
    }

    uint32_t& mapEntryCount()
    {
      return m_specializationInfo.mapEntryCount;
    }

    SpecializationInfo& mapEntryCount( uint32_t mapEntryCount )
    {
      m_specializationInfo.mapEntryCount = mapEntryCount;
      return *this;
    }

    const SpecializationMapEntry* pMapEntries() const
    {
      return reinterpret_cast<const SpecializationMapEntry*>( m_specializationInfo.pMapEntries );
    }

    const SpecializationMapEntry* pMapEntries()
    {
      return reinterpret_cast<const SpecializationMapEntry*>( m_specializationInfo.pMapEntries );
    }

    SpecializationInfo& pMapEntries( const SpecializationMapEntry* pMapEntries )
    {
      m_specializationInfo.pMapEntries = reinterpret_cast<const VkSpecializationMapEntry*>( pMapEntries );
      return *this;
    }

    const size_t& dataSize() const
    {
      return m_specializationInfo.dataSize;
    }

    size_t& dataSize()
    {
      return m_specializationInfo.dataSize;
    }

    SpecializationInfo& dataSize( size_t dataSize )
    {
      m_specializationInfo.dataSize = dataSize;
      return *this;
    }

    const void* pData() const
    {
      return reinterpret_cast<const void*>( m_specializationInfo.pData );
    }

    const void* pData()
    {
      return reinterpret_cast<const void*>( m_specializationInfo.pData );
    }

    SpecializationInfo& pData( const void* pData )
    {
      m_specializationInfo.pData = pData;
      return *this;
    }

    static SpecializationInfo& null()
    {
      return *((SpecializationInfo*)(nullptr));
    }

    operator const VkSpecializationInfo&() const
    {
      return m_specializationInfo;
    }

  private:
    VkSpecializationInfo m_specializationInfo;
  };
  static_assert( sizeof( SpecializationInfo ) == sizeof( VkSpecializationInfo ), "struct and wrapper have different size!" );

  class ClearColorValue
  {
  public:
    ClearColorValue( const std::array<float,4>& float32 = { 0 } )
    {
      memcpy( &m_clearColorValue.float32, float32.data(), 4 * sizeof( float ) );
    }

    const float* float32() const
    {
      return reinterpret_cast<const float*>( m_clearColorValue.float32 );
    }

    float* float32()
    {
      return reinterpret_cast<float*>( m_clearColorValue.float32 );
    }

    ClearColorValue& float32( std::array<float,4> float32 )
    {
      memcpy( &m_clearColorValue.float32, float32.data(), 4 * sizeof( float ) );
      return *this;
    }

    ClearColorValue( const std::array<int32_t,4>& int32 )
    {
      memcpy( &m_clearColorValue.int32, int32.data(), 4 * sizeof( int32_t ) );
    }

    const int32_t* int32() const
    {
      return reinterpret_cast<const int32_t*>( m_clearColorValue.int32 );
    }

    int32_t* int32()
    {
      return reinterpret_cast<int32_t*>( m_clearColorValue.int32 );
    }

    ClearColorValue& int32( std::array<int32_t,4> int32 )
    {
      memcpy( &m_clearColorValue.int32, int32.data(), 4 * sizeof( int32_t ) );
      return *this;
    }

    ClearColorValue( const std::array<uint32_t,4>& uint32 )
    {
      memcpy( &m_clearColorValue.uint32, uint32.data(), 4 * sizeof( uint32_t ) );
    }

    const uint32_t* uint32() const
    {
      return reinterpret_cast<const uint32_t*>( m_clearColorValue.uint32 );
    }

    uint32_t* uint32()
    {
      return reinterpret_cast<uint32_t*>( m_clearColorValue.uint32 );
    }

    ClearColorValue& uint32( std::array<uint32_t,4> uint32 )
    {
      memcpy( &m_clearColorValue.uint32, uint32.data(), 4 * sizeof( uint32_t ) );
      return *this;
    }

    operator VkClearColorValue const& () const
    {
      return m_clearColorValue;
    }

  private:
    VkClearColorValue m_clearColorValue;
  };

  class ClearDepthStencilValue
  {
  public:
    ClearDepthStencilValue()
      : ClearDepthStencilValue( 0, 0 )
    {}

    ClearDepthStencilValue( float depth, uint32_t stencil)
    {
      m_clearDepthStencilValue.depth = depth;
      m_clearDepthStencilValue.stencil = stencil;
    }

    ClearDepthStencilValue(VkClearDepthStencilValue const & rhs)
      : m_clearDepthStencilValue(rhs)
    {
    }

    ClearDepthStencilValue& operator=(VkClearDepthStencilValue const & rhs)
    {
      m_clearDepthStencilValue = rhs;
      return *this;
    }

    const float& depth() const
    {
      return m_clearDepthStencilValue.depth;
    }

    float& depth()
    {
      return m_clearDepthStencilValue.depth;
    }

    ClearDepthStencilValue& depth( float depth )
    {
      m_clearDepthStencilValue.depth = depth;
      return *this;
    }

    const uint32_t& stencil() const
    {
      return m_clearDepthStencilValue.stencil;
    }

    uint32_t& stencil()
    {
      return m_clearDepthStencilValue.stencil;
    }

    ClearDepthStencilValue& stencil( uint32_t stencil )
    {
      m_clearDepthStencilValue.stencil = stencil;
      return *this;
    }

    static ClearDepthStencilValue& null()
    {
      return *((ClearDepthStencilValue*)(nullptr));
    }

    operator const VkClearDepthStencilValue&() const
    {
      return m_clearDepthStencilValue;
    }

  private:
    VkClearDepthStencilValue m_clearDepthStencilValue;
  };
  static_assert( sizeof( ClearDepthStencilValue ) == sizeof( VkClearDepthStencilValue ), "struct and wrapper have different size!" );

  class ClearValue
  {
  public:
    ClearValue( ClearColorValue color = ClearColorValue() )
    {
      m_clearValue.color = color;
    }

    const ClearColorValue& color() const
    {
      return reinterpret_cast<const ClearColorValue&>( m_clearValue.color );
    }

    ClearColorValue& color()
    {
      return reinterpret_cast<ClearColorValue&>( m_clearValue.color );
    }

    ClearValue& color( ClearColorValue color )
    {
      m_clearValue.color = static_cast<VkClearColorValue>( color );
      return *this;
    }

    ClearValue( ClearDepthStencilValue depthStencil )
    {
      m_clearValue.depthStencil = depthStencil;
    }

    const ClearDepthStencilValue& depthStencil() const
    {
      return reinterpret_cast<const ClearDepthStencilValue&>( m_clearValue.depthStencil );
    }

    ClearDepthStencilValue& depthStencil()
    {
      return reinterpret_cast<ClearDepthStencilValue&>( m_clearValue.depthStencil );
    }

    ClearValue& depthStencil( ClearDepthStencilValue depthStencil )
    {
      m_clearValue.depthStencil = static_cast<VkClearDepthStencilValue>( depthStencil );
      return *this;
    }

    operator VkClearValue const& () const
    {
      return m_clearValue;
    }

  private:
    VkClearValue m_clearValue;
  };

  class PhysicalDeviceFeatures
  {
  public:
    PhysicalDeviceFeatures()
      : PhysicalDeviceFeatures( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
    {}

    PhysicalDeviceFeatures( Bool32 robustBufferAccess, Bool32 fullDrawIndexUint32, Bool32 imageCubeArray, Bool32 independentBlend, Bool32 geometryShader, Bool32 tessellationShader, Bool32 sampleRateShading, Bool32 dualSrcBlend, Bool32 logicOp, Bool32 multiDrawIndirect, Bool32 drawIndirectFirstInstance, Bool32 depthClamp, Bool32 depthBiasClamp, Bool32 fillModeNonSolid, Bool32 depthBounds, Bool32 wideLines, Bool32 largePoints, Bool32 alphaToOne, Bool32 multiViewport, Bool32 samplerAnisotropy, Bool32 textureCompressionETC2, Bool32 textureCompressionASTC_LDR, Bool32 textureCompressionBC, Bool32 occlusionQueryPrecise, Bool32 pipelineStatisticsQuery, Bool32 vertexPipelineStoresAndAtomics, Bool32 fragmentStoresAndAtomics, Bool32 shaderTessellationAndGeometryPointSize, Bool32 shaderImageGatherExtended, Bool32 shaderStorageImageExtendedFormats, Bool32 shaderStorageImageMultisample, Bool32 shaderStorageImageReadWithoutFormat, Bool32 shaderStorageImageWriteWithoutFormat, Bool32 shaderUniformBufferArrayDynamicIndexing, Bool32 shaderSampledImageArrayDynamicIndexing, Bool32 shaderStorageBufferArrayDynamicIndexing, Bool32 shaderStorageImageArrayDynamicIndexing, Bool32 shaderClipDistance, Bool32 shaderCullDistance, Bool32 shaderFloat64, Bool32 shaderInt64, Bool32 shaderInt16, Bool32 shaderResourceResidency, Bool32 shaderResourceMinLod, Bool32 sparseBinding, Bool32 sparseResidencyBuffer, Bool32 sparseResidencyImage2D, Bool32 sparseResidencyImage3D, Bool32 sparseResidency2Samples, Bool32 sparseResidency4Samples, Bool32 sparseResidency8Samples, Bool32 sparseResidency16Samples, Bool32 sparseResidencyAliased, Bool32 variableMultisampleRate, Bool32 inheritedQueries)
    {
      m_physicalDeviceFeatures.robustBufferAccess = robustBufferAccess;
      m_physicalDeviceFeatures.fullDrawIndexUint32 = fullDrawIndexUint32;
      m_physicalDeviceFeatures.imageCubeArray = imageCubeArray;
      m_physicalDeviceFeatures.independentBlend = independentBlend;
      m_physicalDeviceFeatures.geometryShader = geometryShader;
      m_physicalDeviceFeatures.tessellationShader = tessellationShader;
      m_physicalDeviceFeatures.sampleRateShading = sampleRateShading;
      m_physicalDeviceFeatures.dualSrcBlend = dualSrcBlend;
      m_physicalDeviceFeatures.logicOp = logicOp;
      m_physicalDeviceFeatures.multiDrawIndirect = multiDrawIndirect;
      m_physicalDeviceFeatures.drawIndirectFirstInstance = drawIndirectFirstInstance;
      m_physicalDeviceFeatures.depthClamp = depthClamp;
      m_physicalDeviceFeatures.depthBiasClamp = depthBiasClamp;
      m_physicalDeviceFeatures.fillModeNonSolid = fillModeNonSolid;
      m_physicalDeviceFeatures.depthBounds = depthBounds;
      m_physicalDeviceFeatures.wideLines = wideLines;
      m_physicalDeviceFeatures.largePoints = largePoints;
      m_physicalDeviceFeatures.alphaToOne = alphaToOne;
      m_physicalDeviceFeatures.multiViewport = multiViewport;
      m_physicalDeviceFeatures.samplerAnisotropy = samplerAnisotropy;
      m_physicalDeviceFeatures.textureCompressionETC2 = textureCompressionETC2;
      m_physicalDeviceFeatures.textureCompressionASTC_LDR = textureCompressionASTC_LDR;
      m_physicalDeviceFeatures.textureCompressionBC = textureCompressionBC;
      m_physicalDeviceFeatures.occlusionQueryPrecise = occlusionQueryPrecise;
      m_physicalDeviceFeatures.pipelineStatisticsQuery = pipelineStatisticsQuery;
      m_physicalDeviceFeatures.vertexPipelineStoresAndAtomics = vertexPipelineStoresAndAtomics;
      m_physicalDeviceFeatures.fragmentStoresAndAtomics = fragmentStoresAndAtomics;
      m_physicalDeviceFeatures.shaderTessellationAndGeometryPointSize = shaderTessellationAndGeometryPointSize;
      m_physicalDeviceFeatures.shaderImageGatherExtended = shaderImageGatherExtended;
      m_physicalDeviceFeatures.shaderStorageImageExtendedFormats = shaderStorageImageExtendedFormats;
      m_physicalDeviceFeatures.shaderStorageImageMultisample = shaderStorageImageMultisample;
      m_physicalDeviceFeatures.shaderStorageImageReadWithoutFormat = shaderStorageImageReadWithoutFormat;
      m_physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat = shaderStorageImageWriteWithoutFormat;
      m_physicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing = shaderUniformBufferArrayDynamicIndexing;
      m_physicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing = shaderSampledImageArrayDynamicIndexing;
      m_physicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing = shaderStorageBufferArrayDynamicIndexing;
      m_physicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing = shaderStorageImageArrayDynamicIndexing;
      m_physicalDeviceFeatures.shaderClipDistance = shaderClipDistance;
      m_physicalDeviceFeatures.shaderCullDistance = shaderCullDistance;
      m_physicalDeviceFeatures.shaderFloat64 = shaderFloat64;
      m_physicalDeviceFeatures.shaderInt64 = shaderInt64;
      m_physicalDeviceFeatures.shaderInt16 = shaderInt16;
      m_physicalDeviceFeatures.shaderResourceResidency = shaderResourceResidency;
      m_physicalDeviceFeatures.shaderResourceMinLod = shaderResourceMinLod;
      m_physicalDeviceFeatures.sparseBinding = sparseBinding;
      m_physicalDeviceFeatures.sparseResidencyBuffer = sparseResidencyBuffer;
      m_physicalDeviceFeatures.sparseResidencyImage2D = sparseResidencyImage2D;
      m_physicalDeviceFeatures.sparseResidencyImage3D = sparseResidencyImage3D;
      m_physicalDeviceFeatures.sparseResidency2Samples = sparseResidency2Samples;
      m_physicalDeviceFeatures.sparseResidency4Samples = sparseResidency4Samples;
      m_physicalDeviceFeatures.sparseResidency8Samples = sparseResidency8Samples;
      m_physicalDeviceFeatures.sparseResidency16Samples = sparseResidency16Samples;
      m_physicalDeviceFeatures.sparseResidencyAliased = sparseResidencyAliased;
      m_physicalDeviceFeatures.variableMultisampleRate = variableMultisampleRate;
      m_physicalDeviceFeatures.inheritedQueries = inheritedQueries;
    }

    PhysicalDeviceFeatures(VkPhysicalDeviceFeatures const & rhs)
      : m_physicalDeviceFeatures(rhs)
    {
    }

    PhysicalDeviceFeatures& operator=(VkPhysicalDeviceFeatures const & rhs)
    {
      m_physicalDeviceFeatures = rhs;
      return *this;
    }

    const Bool32& robustBufferAccess() const
    {
      return m_physicalDeviceFeatures.robustBufferAccess;
    }

    Bool32& robustBufferAccess()
    {
      return m_physicalDeviceFeatures.robustBufferAccess;
    }

    PhysicalDeviceFeatures& robustBufferAccess( Bool32 robustBufferAccess )
    {
      m_physicalDeviceFeatures.robustBufferAccess = robustBufferAccess;
      return *this;
    }

    const Bool32& fullDrawIndexUint32() const
    {
      return m_physicalDeviceFeatures.fullDrawIndexUint32;
    }

    Bool32& fullDrawIndexUint32()
    {
      return m_physicalDeviceFeatures.fullDrawIndexUint32;
    }

    PhysicalDeviceFeatures& fullDrawIndexUint32( Bool32 fullDrawIndexUint32 )
    {
      m_physicalDeviceFeatures.fullDrawIndexUint32 = fullDrawIndexUint32;
      return *this;
    }

    const Bool32& imageCubeArray() const
    {
      return m_physicalDeviceFeatures.imageCubeArray;
    }

    Bool32& imageCubeArray()
    {
      return m_physicalDeviceFeatures.imageCubeArray;
    }

    PhysicalDeviceFeatures& imageCubeArray( Bool32 imageCubeArray )
    {
      m_physicalDeviceFeatures.imageCubeArray = imageCubeArray;
      return *this;
    }

    const Bool32& independentBlend() const
    {
      return m_physicalDeviceFeatures.independentBlend;
    }

    Bool32& independentBlend()
    {
      return m_physicalDeviceFeatures.independentBlend;
    }

    PhysicalDeviceFeatures& independentBlend( Bool32 independentBlend )
    {
      m_physicalDeviceFeatures.independentBlend = independentBlend;
      return *this;
    }

    const Bool32& geometryShader() const
    {
      return m_physicalDeviceFeatures.geometryShader;
    }

    Bool32& geometryShader()
    {
      return m_physicalDeviceFeatures.geometryShader;
    }

    PhysicalDeviceFeatures& geometryShader( Bool32 geometryShader )
    {
      m_physicalDeviceFeatures.geometryShader = geometryShader;
      return *this;
    }

    const Bool32& tessellationShader() const
    {
      return m_physicalDeviceFeatures.tessellationShader;
    }

    Bool32& tessellationShader()
    {
      return m_physicalDeviceFeatures.tessellationShader;
    }

    PhysicalDeviceFeatures& tessellationShader( Bool32 tessellationShader )
    {
      m_physicalDeviceFeatures.tessellationShader = tessellationShader;
      return *this;
    }

    const Bool32& sampleRateShading() const
    {
      return m_physicalDeviceFeatures.sampleRateShading;
    }

    Bool32& sampleRateShading()
    {
      return m_physicalDeviceFeatures.sampleRateShading;
    }

    PhysicalDeviceFeatures& sampleRateShading( Bool32 sampleRateShading )
    {
      m_physicalDeviceFeatures.sampleRateShading = sampleRateShading;
      return *this;
    }

    const Bool32& dualSrcBlend() const
    {
      return m_physicalDeviceFeatures.dualSrcBlend;
    }

    Bool32& dualSrcBlend()
    {
      return m_physicalDeviceFeatures.dualSrcBlend;
    }

    PhysicalDeviceFeatures& dualSrcBlend( Bool32 dualSrcBlend )
    {
      m_physicalDeviceFeatures.dualSrcBlend = dualSrcBlend;
      return *this;
    }

    const Bool32& logicOp() const
    {
      return m_physicalDeviceFeatures.logicOp;
    }

    Bool32& logicOp()
    {
      return m_physicalDeviceFeatures.logicOp;
    }

    PhysicalDeviceFeatures& logicOp( Bool32 logicOp )
    {
      m_physicalDeviceFeatures.logicOp = logicOp;
      return *this;
    }

    const Bool32& multiDrawIndirect() const
    {
      return m_physicalDeviceFeatures.multiDrawIndirect;
    }

    Bool32& multiDrawIndirect()
    {
      return m_physicalDeviceFeatures.multiDrawIndirect;
    }

    PhysicalDeviceFeatures& multiDrawIndirect( Bool32 multiDrawIndirect )
    {
      m_physicalDeviceFeatures.multiDrawIndirect = multiDrawIndirect;
      return *this;
    }

    const Bool32& drawIndirectFirstInstance() const
    {
      return m_physicalDeviceFeatures.drawIndirectFirstInstance;
    }

    Bool32& drawIndirectFirstInstance()
    {
      return m_physicalDeviceFeatures.drawIndirectFirstInstance;
    }

    PhysicalDeviceFeatures& drawIndirectFirstInstance( Bool32 drawIndirectFirstInstance )
    {
      m_physicalDeviceFeatures.drawIndirectFirstInstance = drawIndirectFirstInstance;
      return *this;
    }

    const Bool32& depthClamp() const
    {
      return m_physicalDeviceFeatures.depthClamp;
    }

    Bool32& depthClamp()
    {
      return m_physicalDeviceFeatures.depthClamp;
    }

    PhysicalDeviceFeatures& depthClamp( Bool32 depthClamp )
    {
      m_physicalDeviceFeatures.depthClamp = depthClamp;
      return *this;
    }

    const Bool32& depthBiasClamp() const
    {
      return m_physicalDeviceFeatures.depthBiasClamp;
    }

    Bool32& depthBiasClamp()
    {
      return m_physicalDeviceFeatures.depthBiasClamp;
    }

    PhysicalDeviceFeatures& depthBiasClamp( Bool32 depthBiasClamp )
    {
      m_physicalDeviceFeatures.depthBiasClamp = depthBiasClamp;
      return *this;
    }

    const Bool32& fillModeNonSolid() const
    {
      return m_physicalDeviceFeatures.fillModeNonSolid;
    }

    Bool32& fillModeNonSolid()
    {
      return m_physicalDeviceFeatures.fillModeNonSolid;
    }

    PhysicalDeviceFeatures& fillModeNonSolid( Bool32 fillModeNonSolid )
    {
      m_physicalDeviceFeatures.fillModeNonSolid = fillModeNonSolid;
      return *this;
    }

    const Bool32& depthBounds() const
    {
      return m_physicalDeviceFeatures.depthBounds;
    }

    Bool32& depthBounds()
    {
      return m_physicalDeviceFeatures.depthBounds;
    }

    PhysicalDeviceFeatures& depthBounds( Bool32 depthBounds )
    {
      m_physicalDeviceFeatures.depthBounds = depthBounds;
      return *this;
    }

    const Bool32& wideLines() const
    {
      return m_physicalDeviceFeatures.wideLines;
    }

    Bool32& wideLines()
    {
      return m_physicalDeviceFeatures.wideLines;
    }

    PhysicalDeviceFeatures& wideLines( Bool32 wideLines )
    {
      m_physicalDeviceFeatures.wideLines = wideLines;
      return *this;
    }

    const Bool32& largePoints() const
    {
      return m_physicalDeviceFeatures.largePoints;
    }

    Bool32& largePoints()
    {
      return m_physicalDeviceFeatures.largePoints;
    }

    PhysicalDeviceFeatures& largePoints( Bool32 largePoints )
    {
      m_physicalDeviceFeatures.largePoints = largePoints;
      return *this;
    }

    const Bool32& alphaToOne() const
    {
      return m_physicalDeviceFeatures.alphaToOne;
    }

    Bool32& alphaToOne()
    {
      return m_physicalDeviceFeatures.alphaToOne;
    }

    PhysicalDeviceFeatures& alphaToOne( Bool32 alphaToOne )
    {
      m_physicalDeviceFeatures.alphaToOne = alphaToOne;
      return *this;
    }

    const Bool32& multiViewport() const
    {
      return m_physicalDeviceFeatures.multiViewport;
    }

    Bool32& multiViewport()
    {
      return m_physicalDeviceFeatures.multiViewport;
    }

    PhysicalDeviceFeatures& multiViewport( Bool32 multiViewport )
    {
      m_physicalDeviceFeatures.multiViewport = multiViewport;
      return *this;
    }

    const Bool32& samplerAnisotropy() const
    {
      return m_physicalDeviceFeatures.samplerAnisotropy;
    }

    Bool32& samplerAnisotropy()
    {
      return m_physicalDeviceFeatures.samplerAnisotropy;
    }

    PhysicalDeviceFeatures& samplerAnisotropy( Bool32 samplerAnisotropy )
    {
      m_physicalDeviceFeatures.samplerAnisotropy = samplerAnisotropy;
      return *this;
    }

    const Bool32& textureCompressionETC2() const
    {
      return m_physicalDeviceFeatures.textureCompressionETC2;
    }

    Bool32& textureCompressionETC2()
    {
      return m_physicalDeviceFeatures.textureCompressionETC2;
    }

    PhysicalDeviceFeatures& textureCompressionETC2( Bool32 textureCompressionETC2 )
    {
      m_physicalDeviceFeatures.textureCompressionETC2 = textureCompressionETC2;
      return *this;
    }

    const Bool32& textureCompressionASTC_LDR() const
    {
      return m_physicalDeviceFeatures.textureCompressionASTC_LDR;
    }

    Bool32& textureCompressionASTC_LDR()
    {
      return m_physicalDeviceFeatures.textureCompressionASTC_LDR;
    }

    PhysicalDeviceFeatures& textureCompressionASTC_LDR( Bool32 textureCompressionASTC_LDR )
    {
      m_physicalDeviceFeatures.textureCompressionASTC_LDR = textureCompressionASTC_LDR;
      return *this;
    }

    const Bool32& textureCompressionBC() const
    {
      return m_physicalDeviceFeatures.textureCompressionBC;
    }

    Bool32& textureCompressionBC()
    {
      return m_physicalDeviceFeatures.textureCompressionBC;
    }

    PhysicalDeviceFeatures& textureCompressionBC( Bool32 textureCompressionBC )
    {
      m_physicalDeviceFeatures.textureCompressionBC = textureCompressionBC;
      return *this;
    }

    const Bool32& occlusionQueryPrecise() const
    {
      return m_physicalDeviceFeatures.occlusionQueryPrecise;
    }

    Bool32& occlusionQueryPrecise()
    {
      return m_physicalDeviceFeatures.occlusionQueryPrecise;
    }

    PhysicalDeviceFeatures& occlusionQueryPrecise( Bool32 occlusionQueryPrecise )
    {
      m_physicalDeviceFeatures.occlusionQueryPrecise = occlusionQueryPrecise;
      return *this;
    }

    const Bool32& pipelineStatisticsQuery() const
    {
      return m_physicalDeviceFeatures.pipelineStatisticsQuery;
    }

    Bool32& pipelineStatisticsQuery()
    {
      return m_physicalDeviceFeatures.pipelineStatisticsQuery;
    }

    PhysicalDeviceFeatures& pipelineStatisticsQuery( Bool32 pipelineStatisticsQuery )
    {
      m_physicalDeviceFeatures.pipelineStatisticsQuery = pipelineStatisticsQuery;
      return *this;
    }

    const Bool32& vertexPipelineStoresAndAtomics() const
    {
      return m_physicalDeviceFeatures.vertexPipelineStoresAndAtomics;
    }

    Bool32& vertexPipelineStoresAndAtomics()
    {
      return m_physicalDeviceFeatures.vertexPipelineStoresAndAtomics;
    }

    PhysicalDeviceFeatures& vertexPipelineStoresAndAtomics( Bool32 vertexPipelineStoresAndAtomics )
    {
      m_physicalDeviceFeatures.vertexPipelineStoresAndAtomics = vertexPipelineStoresAndAtomics;
      return *this;
    }

    const Bool32& fragmentStoresAndAtomics() const
    {
      return m_physicalDeviceFeatures.fragmentStoresAndAtomics;
    }

    Bool32& fragmentStoresAndAtomics()
    {
      return m_physicalDeviceFeatures.fragmentStoresAndAtomics;
    }

    PhysicalDeviceFeatures& fragmentStoresAndAtomics( Bool32 fragmentStoresAndAtomics )
    {
      m_physicalDeviceFeatures.fragmentStoresAndAtomics = fragmentStoresAndAtomics;
      return *this;
    }

    const Bool32& shaderTessellationAndGeometryPointSize() const
    {
      return m_physicalDeviceFeatures.shaderTessellationAndGeometryPointSize;
    }

    Bool32& shaderTessellationAndGeometryPointSize()
    {
      return m_physicalDeviceFeatures.shaderTessellationAndGeometryPointSize;
    }

    PhysicalDeviceFeatures& shaderTessellationAndGeometryPointSize( Bool32 shaderTessellationAndGeometryPointSize )
    {
      m_physicalDeviceFeatures.shaderTessellationAndGeometryPointSize = shaderTessellationAndGeometryPointSize;
      return *this;
    }

    const Bool32& shaderImageGatherExtended() const
    {
      return m_physicalDeviceFeatures.shaderImageGatherExtended;
    }

    Bool32& shaderImageGatherExtended()
    {
      return m_physicalDeviceFeatures.shaderImageGatherExtended;
    }

    PhysicalDeviceFeatures& shaderImageGatherExtended( Bool32 shaderImageGatherExtended )
    {
      m_physicalDeviceFeatures.shaderImageGatherExtended = shaderImageGatherExtended;
      return *this;
    }

    const Bool32& shaderStorageImageExtendedFormats() const
    {
      return m_physicalDeviceFeatures.shaderStorageImageExtendedFormats;
    }

    Bool32& shaderStorageImageExtendedFormats()
    {
      return m_physicalDeviceFeatures.shaderStorageImageExtendedFormats;
    }

    PhysicalDeviceFeatures& shaderStorageImageExtendedFormats( Bool32 shaderStorageImageExtendedFormats )
    {
      m_physicalDeviceFeatures.shaderStorageImageExtendedFormats = shaderStorageImageExtendedFormats;
      return *this;
    }

    const Bool32& shaderStorageImageMultisample() const
    {
      return m_physicalDeviceFeatures.shaderStorageImageMultisample;
    }

    Bool32& shaderStorageImageMultisample()
    {
      return m_physicalDeviceFeatures.shaderStorageImageMultisample;
    }

    PhysicalDeviceFeatures& shaderStorageImageMultisample( Bool32 shaderStorageImageMultisample )
    {
      m_physicalDeviceFeatures.shaderStorageImageMultisample = shaderStorageImageMultisample;
      return *this;
    }

    const Bool32& shaderStorageImageReadWithoutFormat() const
    {
      return m_physicalDeviceFeatures.shaderStorageImageReadWithoutFormat;
    }

    Bool32& shaderStorageImageReadWithoutFormat()
    {
      return m_physicalDeviceFeatures.shaderStorageImageReadWithoutFormat;
    }

    PhysicalDeviceFeatures& shaderStorageImageReadWithoutFormat( Bool32 shaderStorageImageReadWithoutFormat )
    {
      m_physicalDeviceFeatures.shaderStorageImageReadWithoutFormat = shaderStorageImageReadWithoutFormat;
      return *this;
    }

    const Bool32& shaderStorageImageWriteWithoutFormat() const
    {
      return m_physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat;
    }

    Bool32& shaderStorageImageWriteWithoutFormat()
    {
      return m_physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat;
    }

    PhysicalDeviceFeatures& shaderStorageImageWriteWithoutFormat( Bool32 shaderStorageImageWriteWithoutFormat )
    {
      m_physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat = shaderStorageImageWriteWithoutFormat;
      return *this;
    }

    const Bool32& shaderUniformBufferArrayDynamicIndexing() const
    {
      return m_physicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing;
    }

    Bool32& shaderUniformBufferArrayDynamicIndexing()
    {
      return m_physicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing;
    }

    PhysicalDeviceFeatures& shaderUniformBufferArrayDynamicIndexing( Bool32 shaderUniformBufferArrayDynamicIndexing )
    {
      m_physicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing = shaderUniformBufferArrayDynamicIndexing;
      return *this;
    }

    const Bool32& shaderSampledImageArrayDynamicIndexing() const
    {
      return m_physicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing;
    }

    Bool32& shaderSampledImageArrayDynamicIndexing()
    {
      return m_physicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing;
    }

    PhysicalDeviceFeatures& shaderSampledImageArrayDynamicIndexing( Bool32 shaderSampledImageArrayDynamicIndexing )
    {
      m_physicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing = shaderSampledImageArrayDynamicIndexing;
      return *this;
    }

    const Bool32& shaderStorageBufferArrayDynamicIndexing() const
    {
      return m_physicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing;
    }

    Bool32& shaderStorageBufferArrayDynamicIndexing()
    {
      return m_physicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing;
    }

    PhysicalDeviceFeatures& shaderStorageBufferArrayDynamicIndexing( Bool32 shaderStorageBufferArrayDynamicIndexing )
    {
      m_physicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing = shaderStorageBufferArrayDynamicIndexing;
      return *this;
    }

    const Bool32& shaderStorageImageArrayDynamicIndexing() const
    {
      return m_physicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing;
    }

    Bool32& shaderStorageImageArrayDynamicIndexing()
    {
      return m_physicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing;
    }

    PhysicalDeviceFeatures& shaderStorageImageArrayDynamicIndexing( Bool32 shaderStorageImageArrayDynamicIndexing )
    {
      m_physicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing = shaderStorageImageArrayDynamicIndexing;
      return *this;
    }

    const Bool32& shaderClipDistance() const
    {
      return m_physicalDeviceFeatures.shaderClipDistance;
    }

    Bool32& shaderClipDistance()
    {
      return m_physicalDeviceFeatures.shaderClipDistance;
    }

    PhysicalDeviceFeatures& shaderClipDistance( Bool32 shaderClipDistance )
    {
      m_physicalDeviceFeatures.shaderClipDistance = shaderClipDistance;
      return *this;
    }

    const Bool32& shaderCullDistance() const
    {
      return m_physicalDeviceFeatures.shaderCullDistance;
    }

    Bool32& shaderCullDistance()
    {
      return m_physicalDeviceFeatures.shaderCullDistance;
    }

    PhysicalDeviceFeatures& shaderCullDistance( Bool32 shaderCullDistance )
    {
      m_physicalDeviceFeatures.shaderCullDistance = shaderCullDistance;
      return *this;
    }

    const Bool32& shaderFloat64() const
    {
      return m_physicalDeviceFeatures.shaderFloat64;
    }

    Bool32& shaderFloat64()
    {
      return m_physicalDeviceFeatures.shaderFloat64;
    }

    PhysicalDeviceFeatures& shaderFloat64( Bool32 shaderFloat64 )
    {
      m_physicalDeviceFeatures.shaderFloat64 = shaderFloat64;
      return *this;
    }

    const Bool32& shaderInt64() const
    {
      return m_physicalDeviceFeatures.shaderInt64;
    }

    Bool32& shaderInt64()
    {
      return m_physicalDeviceFeatures.shaderInt64;
    }

    PhysicalDeviceFeatures& shaderInt64( Bool32 shaderInt64 )
    {
      m_physicalDeviceFeatures.shaderInt64 = shaderInt64;
      return *this;
    }

    const Bool32& shaderInt16() const
    {
      return m_physicalDeviceFeatures.shaderInt16;
    }

    Bool32& shaderInt16()
    {
      return m_physicalDeviceFeatures.shaderInt16;
    }

    PhysicalDeviceFeatures& shaderInt16( Bool32 shaderInt16 )
    {
      m_physicalDeviceFeatures.shaderInt16 = shaderInt16;
      return *this;
    }

    const Bool32& shaderResourceResidency() const
    {
      return m_physicalDeviceFeatures.shaderResourceResidency;
    }

    Bool32& shaderResourceResidency()
    {
      return m_physicalDeviceFeatures.shaderResourceResidency;
    }

    PhysicalDeviceFeatures& shaderResourceResidency( Bool32 shaderResourceResidency )
    {
      m_physicalDeviceFeatures.shaderResourceResidency = shaderResourceResidency;
      return *this;
    }

    const Bool32& shaderResourceMinLod() const
    {
      return m_physicalDeviceFeatures.shaderResourceMinLod;
    }

    Bool32& shaderResourceMinLod()
    {
      return m_physicalDeviceFeatures.shaderResourceMinLod;
    }

    PhysicalDeviceFeatures& shaderResourceMinLod( Bool32 shaderResourceMinLod )
    {
      m_physicalDeviceFeatures.shaderResourceMinLod = shaderResourceMinLod;
      return *this;
    }

    const Bool32& sparseBinding() const
    {
      return m_physicalDeviceFeatures.sparseBinding;
    }

    Bool32& sparseBinding()
    {
      return m_physicalDeviceFeatures.sparseBinding;
    }

    PhysicalDeviceFeatures& sparseBinding( Bool32 sparseBinding )
    {
      m_physicalDeviceFeatures.sparseBinding = sparseBinding;
      return *this;
    }

    const Bool32& sparseResidencyBuffer() const
    {
      return m_physicalDeviceFeatures.sparseResidencyBuffer;
    }

    Bool32& sparseResidencyBuffer()
    {
      return m_physicalDeviceFeatures.sparseResidencyBuffer;
    }

    PhysicalDeviceFeatures& sparseResidencyBuffer( Bool32 sparseResidencyBuffer )
    {
      m_physicalDeviceFeatures.sparseResidencyBuffer = sparseResidencyBuffer;
      return *this;
    }

    const Bool32& sparseResidencyImage2D() const
    {
      return m_physicalDeviceFeatures.sparseResidencyImage2D;
    }

    Bool32& sparseResidencyImage2D()
    {
      return m_physicalDeviceFeatures.sparseResidencyImage2D;
    }

    PhysicalDeviceFeatures& sparseResidencyImage2D( Bool32 sparseResidencyImage2D )
    {
      m_physicalDeviceFeatures.sparseResidencyImage2D = sparseResidencyImage2D;
      return *this;
    }

    const Bool32& sparseResidencyImage3D() const
    {
      return m_physicalDeviceFeatures.sparseResidencyImage3D;
    }

    Bool32& sparseResidencyImage3D()
    {
      return m_physicalDeviceFeatures.sparseResidencyImage3D;
    }

    PhysicalDeviceFeatures& sparseResidencyImage3D( Bool32 sparseResidencyImage3D )
    {
      m_physicalDeviceFeatures.sparseResidencyImage3D = sparseResidencyImage3D;
      return *this;
    }

    const Bool32& sparseResidency2Samples() const
    {
      return m_physicalDeviceFeatures.sparseResidency2Samples;
    }

    Bool32& sparseResidency2Samples()
    {
      return m_physicalDeviceFeatures.sparseResidency2Samples;
    }

    PhysicalDeviceFeatures& sparseResidency2Samples( Bool32 sparseResidency2Samples )
    {
      m_physicalDeviceFeatures.sparseResidency2Samples = sparseResidency2Samples;
      return *this;
    }

    const Bool32& sparseResidency4Samples() const
    {
      return m_physicalDeviceFeatures.sparseResidency4Samples;
    }

    Bool32& sparseResidency4Samples()
    {
      return m_physicalDeviceFeatures.sparseResidency4Samples;
    }

    PhysicalDeviceFeatures& sparseResidency4Samples( Bool32 sparseResidency4Samples )
    {
      m_physicalDeviceFeatures.sparseResidency4Samples = sparseResidency4Samples;
      return *this;
    }

    const Bool32& sparseResidency8Samples() const
    {
      return m_physicalDeviceFeatures.sparseResidency8Samples;
    }

    Bool32& sparseResidency8Samples()
    {
      return m_physicalDeviceFeatures.sparseResidency8Samples;
    }

    PhysicalDeviceFeatures& sparseResidency8Samples( Bool32 sparseResidency8Samples )
    {
      m_physicalDeviceFeatures.sparseResidency8Samples = sparseResidency8Samples;
      return *this;
    }

    const Bool32& sparseResidency16Samples() const
    {
      return m_physicalDeviceFeatures.sparseResidency16Samples;
    }

    Bool32& sparseResidency16Samples()
    {
      return m_physicalDeviceFeatures.sparseResidency16Samples;
    }

    PhysicalDeviceFeatures& sparseResidency16Samples( Bool32 sparseResidency16Samples )
    {
      m_physicalDeviceFeatures.sparseResidency16Samples = sparseResidency16Samples;
      return *this;
    }

    const Bool32& sparseResidencyAliased() const
    {
      return m_physicalDeviceFeatures.sparseResidencyAliased;
    }

    Bool32& sparseResidencyAliased()
    {
      return m_physicalDeviceFeatures.sparseResidencyAliased;
    }

    PhysicalDeviceFeatures& sparseResidencyAliased( Bool32 sparseResidencyAliased )
    {
      m_physicalDeviceFeatures.sparseResidencyAliased = sparseResidencyAliased;
      return *this;
    }

    const Bool32& variableMultisampleRate() const
    {
      return m_physicalDeviceFeatures.variableMultisampleRate;
    }

    Bool32& variableMultisampleRate()
    {
      return m_physicalDeviceFeatures.variableMultisampleRate;
    }

    PhysicalDeviceFeatures& variableMultisampleRate( Bool32 variableMultisampleRate )
    {
      m_physicalDeviceFeatures.variableMultisampleRate = variableMultisampleRate;
      return *this;
    }

    const Bool32& inheritedQueries() const
    {
      return m_physicalDeviceFeatures.inheritedQueries;
    }

    Bool32& inheritedQueries()
    {
      return m_physicalDeviceFeatures.inheritedQueries;
    }

    PhysicalDeviceFeatures& inheritedQueries( Bool32 inheritedQueries )
    {
      m_physicalDeviceFeatures.inheritedQueries = inheritedQueries;
      return *this;
    }

    static PhysicalDeviceFeatures& null()
    {
      return *((PhysicalDeviceFeatures*)(nullptr));
    }

    operator const VkPhysicalDeviceFeatures&() const
    {
      return m_physicalDeviceFeatures;
    }

  private:
    VkPhysicalDeviceFeatures m_physicalDeviceFeatures;
  };
  static_assert( sizeof( PhysicalDeviceFeatures ) == sizeof( VkPhysicalDeviceFeatures ), "struct and wrapper have different size!" );

  class PhysicalDeviceSparseProperties
  {
  public:
    const Bool32& residencyStandard2DBlockShape() const
    {
      return m_physicalDeviceSparseProperties.residencyStandard2DBlockShape;
    }

    const Bool32& residencyStandard2DMultisampleBlockShape() const
    {
      return m_physicalDeviceSparseProperties.residencyStandard2DMultisampleBlockShape;
    }

    const Bool32& residencyStandard3DBlockShape() const
    {
      return m_physicalDeviceSparseProperties.residencyStandard3DBlockShape;
    }

    const Bool32& residencyAlignedMipSize() const
    {
      return m_physicalDeviceSparseProperties.residencyAlignedMipSize;
    }

    const Bool32& residencyNonResidentStrict() const
    {
      return m_physicalDeviceSparseProperties.residencyNonResidentStrict;
    }

    static PhysicalDeviceSparseProperties& null()
    {
      return *((PhysicalDeviceSparseProperties*)(nullptr));
    }

    operator const VkPhysicalDeviceSparseProperties&() const
    {
      return m_physicalDeviceSparseProperties;
    }

  private:
    VkPhysicalDeviceSparseProperties m_physicalDeviceSparseProperties;
  };
  static_assert( sizeof( PhysicalDeviceSparseProperties ) == sizeof( VkPhysicalDeviceSparseProperties ), "struct and wrapper have different size!" );

  class DrawIndirectCommand
  {
  public:
    DrawIndirectCommand()
      : DrawIndirectCommand( 0, 0, 0, 0 )
    {}

    DrawIndirectCommand( uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
    {
      m_drawIndirectCommand.vertexCount = vertexCount;
      m_drawIndirectCommand.instanceCount = instanceCount;
      m_drawIndirectCommand.firstVertex = firstVertex;
      m_drawIndirectCommand.firstInstance = firstInstance;
    }

    DrawIndirectCommand(VkDrawIndirectCommand const & rhs)
      : m_drawIndirectCommand(rhs)
    {
    }

    DrawIndirectCommand& operator=(VkDrawIndirectCommand const & rhs)
    {
      m_drawIndirectCommand = rhs;
      return *this;
    }

    const uint32_t& vertexCount() const
    {
      return m_drawIndirectCommand.vertexCount;
    }

    uint32_t& vertexCount()
    {
      return m_drawIndirectCommand.vertexCount;
    }

    DrawIndirectCommand& vertexCount( uint32_t vertexCount )
    {
      m_drawIndirectCommand.vertexCount = vertexCount;
      return *this;
    }

    const uint32_t& instanceCount() const
    {
      return m_drawIndirectCommand.instanceCount;
    }

    uint32_t& instanceCount()
    {
      return m_drawIndirectCommand.instanceCount;
    }

    DrawIndirectCommand& instanceCount( uint32_t instanceCount )
    {
      m_drawIndirectCommand.instanceCount = instanceCount;
      return *this;
    }

    const uint32_t& firstVertex() const
    {
      return m_drawIndirectCommand.firstVertex;
    }

    uint32_t& firstVertex()
    {
      return m_drawIndirectCommand.firstVertex;
    }

    DrawIndirectCommand& firstVertex( uint32_t firstVertex )
    {
      m_drawIndirectCommand.firstVertex = firstVertex;
      return *this;
    }

    const uint32_t& firstInstance() const
    {
      return m_drawIndirectCommand.firstInstance;
    }

    uint32_t& firstInstance()
    {
      return m_drawIndirectCommand.firstInstance;
    }

    DrawIndirectCommand& firstInstance( uint32_t firstInstance )
    {
      m_drawIndirectCommand.firstInstance = firstInstance;
      return *this;
    }

    static DrawIndirectCommand& null()
    {
      return *((DrawIndirectCommand*)(nullptr));
    }

    operator const VkDrawIndirectCommand&() const
    {
      return m_drawIndirectCommand;
    }

  private:
    VkDrawIndirectCommand m_drawIndirectCommand;
  };
  static_assert( sizeof( DrawIndirectCommand ) == sizeof( VkDrawIndirectCommand ), "struct and wrapper have different size!" );

  class DrawIndexedIndirectCommand
  {
  public:
    DrawIndexedIndirectCommand()
      : DrawIndexedIndirectCommand( 0, 0, 0, 0, 0 )
    {}

    DrawIndexedIndirectCommand( uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
    {
      m_drawIndexedIndirectCommand.indexCount = indexCount;
      m_drawIndexedIndirectCommand.instanceCount = instanceCount;
      m_drawIndexedIndirectCommand.firstIndex = firstIndex;
      m_drawIndexedIndirectCommand.vertexOffset = vertexOffset;
      m_drawIndexedIndirectCommand.firstInstance = firstInstance;
    }

    DrawIndexedIndirectCommand(VkDrawIndexedIndirectCommand const & rhs)
      : m_drawIndexedIndirectCommand(rhs)
    {
    }

    DrawIndexedIndirectCommand& operator=(VkDrawIndexedIndirectCommand const & rhs)
    {
      m_drawIndexedIndirectCommand = rhs;
      return *this;
    }

    const uint32_t& indexCount() const
    {
      return m_drawIndexedIndirectCommand.indexCount;
    }

    uint32_t& indexCount()
    {
      return m_drawIndexedIndirectCommand.indexCount;
    }

    DrawIndexedIndirectCommand& indexCount( uint32_t indexCount )
    {
      m_drawIndexedIndirectCommand.indexCount = indexCount;
      return *this;
    }

    const uint32_t& instanceCount() const
    {
      return m_drawIndexedIndirectCommand.instanceCount;
    }

    uint32_t& instanceCount()
    {
      return m_drawIndexedIndirectCommand.instanceCount;
    }

    DrawIndexedIndirectCommand& instanceCount( uint32_t instanceCount )
    {
      m_drawIndexedIndirectCommand.instanceCount = instanceCount;
      return *this;
    }

    const uint32_t& firstIndex() const
    {
      return m_drawIndexedIndirectCommand.firstIndex;
    }

    uint32_t& firstIndex()
    {
      return m_drawIndexedIndirectCommand.firstIndex;
    }

    DrawIndexedIndirectCommand& firstIndex( uint32_t firstIndex )
    {
      m_drawIndexedIndirectCommand.firstIndex = firstIndex;
      return *this;
    }

    const int32_t& vertexOffset() const
    {
      return m_drawIndexedIndirectCommand.vertexOffset;
    }

    int32_t& vertexOffset()
    {
      return m_drawIndexedIndirectCommand.vertexOffset;
    }

    DrawIndexedIndirectCommand& vertexOffset( int32_t vertexOffset )
    {
      m_drawIndexedIndirectCommand.vertexOffset = vertexOffset;
      return *this;
    }

    const uint32_t& firstInstance() const
    {
      return m_drawIndexedIndirectCommand.firstInstance;
    }

    uint32_t& firstInstance()
    {
      return m_drawIndexedIndirectCommand.firstInstance;
    }

    DrawIndexedIndirectCommand& firstInstance( uint32_t firstInstance )
    {
      m_drawIndexedIndirectCommand.firstInstance = firstInstance;
      return *this;
    }

    static DrawIndexedIndirectCommand& null()
    {
      return *((DrawIndexedIndirectCommand*)(nullptr));
    }

    operator const VkDrawIndexedIndirectCommand&() const
    {
      return m_drawIndexedIndirectCommand;
    }

  private:
    VkDrawIndexedIndirectCommand m_drawIndexedIndirectCommand;
  };
  static_assert( sizeof( DrawIndexedIndirectCommand ) == sizeof( VkDrawIndexedIndirectCommand ), "struct and wrapper have different size!" );

  class DispatchIndirectCommand
  {
  public:
    DispatchIndirectCommand()
      : DispatchIndirectCommand( 0, 0, 0 )
    {}

    DispatchIndirectCommand( uint32_t x, uint32_t y, uint32_t z)
    {
      m_dispatchIndirectCommand.x = x;
      m_dispatchIndirectCommand.y = y;
      m_dispatchIndirectCommand.z = z;
    }

    DispatchIndirectCommand(VkDispatchIndirectCommand const & rhs)
      : m_dispatchIndirectCommand(rhs)
    {
    }

    DispatchIndirectCommand& operator=(VkDispatchIndirectCommand const & rhs)
    {
      m_dispatchIndirectCommand = rhs;
      return *this;
    }

    const uint32_t& x() const
    {
      return m_dispatchIndirectCommand.x;
    }

    uint32_t& x()
    {
      return m_dispatchIndirectCommand.x;
    }

    DispatchIndirectCommand& x( uint32_t x )
    {
      m_dispatchIndirectCommand.x = x;
      return *this;
    }

    const uint32_t& y() const
    {
      return m_dispatchIndirectCommand.y;
    }

    uint32_t& y()
    {
      return m_dispatchIndirectCommand.y;
    }

    DispatchIndirectCommand& y( uint32_t y )
    {
      m_dispatchIndirectCommand.y = y;
      return *this;
    }

    const uint32_t& z() const
    {
      return m_dispatchIndirectCommand.z;
    }

    uint32_t& z()
    {
      return m_dispatchIndirectCommand.z;
    }

    DispatchIndirectCommand& z( uint32_t z )
    {
      m_dispatchIndirectCommand.z = z;
      return *this;
    }

    static DispatchIndirectCommand& null()
    {
      return *((DispatchIndirectCommand*)(nullptr));
    }

    operator const VkDispatchIndirectCommand&() const
    {
      return m_dispatchIndirectCommand;
    }

  private:
    VkDispatchIndirectCommand m_dispatchIndirectCommand;
  };
  static_assert( sizeof( DispatchIndirectCommand ) == sizeof( VkDispatchIndirectCommand ), "struct and wrapper have different size!" );

  class DisplayPlanePropertiesKHR
  {
  public:
    DisplayPlanePropertiesKHR()
      : DisplayPlanePropertiesKHR( DisplayKHR(), 0 )
    {}

    DisplayPlanePropertiesKHR( DisplayKHR currentDisplay, uint32_t currentStackIndex)
    {
      m_displayPlanePropertiesKHR.currentDisplay = static_cast<VkDisplayKHR>( currentDisplay );
      m_displayPlanePropertiesKHR.currentStackIndex = currentStackIndex;
    }

    DisplayPlanePropertiesKHR(VkDisplayPlanePropertiesKHR const & rhs)
      : m_displayPlanePropertiesKHR(rhs)
    {
    }

    DisplayPlanePropertiesKHR& operator=(VkDisplayPlanePropertiesKHR const & rhs)
    {
      m_displayPlanePropertiesKHR = rhs;
      return *this;
    }

    const DisplayKHR& currentDisplay() const
    {
      return reinterpret_cast<const DisplayKHR&>( m_displayPlanePropertiesKHR.currentDisplay );
    }

    DisplayKHR& currentDisplay()
    {
      return reinterpret_cast<DisplayKHR&>( m_displayPlanePropertiesKHR.currentDisplay );
    }

    DisplayPlanePropertiesKHR& currentDisplay( DisplayKHR currentDisplay )
    {
      m_displayPlanePropertiesKHR.currentDisplay = static_cast<VkDisplayKHR>( currentDisplay );
      return *this;
    }

    const uint32_t& currentStackIndex() const
    {
      return m_displayPlanePropertiesKHR.currentStackIndex;
    }

    uint32_t& currentStackIndex()
    {
      return m_displayPlanePropertiesKHR.currentStackIndex;
    }

    DisplayPlanePropertiesKHR& currentStackIndex( uint32_t currentStackIndex )
    {
      m_displayPlanePropertiesKHR.currentStackIndex = currentStackIndex;
      return *this;
    }

    static DisplayPlanePropertiesKHR& null()
    {
      return *((DisplayPlanePropertiesKHR*)(nullptr));
    }

    operator const VkDisplayPlanePropertiesKHR&() const
    {
      return m_displayPlanePropertiesKHR;
    }

  private:
    VkDisplayPlanePropertiesKHR m_displayPlanePropertiesKHR;
  };
  static_assert( sizeof( DisplayPlanePropertiesKHR ) == sizeof( VkDisplayPlanePropertiesKHR ), "struct and wrapper have different size!" );

  class DisplayModeParametersKHR
  {
  public:
    DisplayModeParametersKHR()
      : DisplayModeParametersKHR( Extent2D(), 0 )
    {}

    DisplayModeParametersKHR( Extent2D visibleRegion, uint32_t refreshRate)
    {
      m_displayModeParametersKHR.visibleRegion = static_cast<VkExtent2D>( visibleRegion );
      m_displayModeParametersKHR.refreshRate = refreshRate;
    }

    DisplayModeParametersKHR(VkDisplayModeParametersKHR const & rhs)
      : m_displayModeParametersKHR(rhs)
    {
    }

    DisplayModeParametersKHR& operator=(VkDisplayModeParametersKHR const & rhs)
    {
      m_displayModeParametersKHR = rhs;
      return *this;
    }

    const Extent2D& visibleRegion() const
    {
      return reinterpret_cast<const Extent2D&>( m_displayModeParametersKHR.visibleRegion );
    }

    Extent2D& visibleRegion()
    {
      return reinterpret_cast<Extent2D&>( m_displayModeParametersKHR.visibleRegion );
    }

    DisplayModeParametersKHR& visibleRegion( Extent2D visibleRegion )
    {
      m_displayModeParametersKHR.visibleRegion = static_cast<VkExtent2D>( visibleRegion );
      return *this;
    }

    const uint32_t& refreshRate() const
    {
      return m_displayModeParametersKHR.refreshRate;
    }

    uint32_t& refreshRate()
    {
      return m_displayModeParametersKHR.refreshRate;
    }

    DisplayModeParametersKHR& refreshRate( uint32_t refreshRate )
    {
      m_displayModeParametersKHR.refreshRate = refreshRate;
      return *this;
    }

    static DisplayModeParametersKHR& null()
    {
      return *((DisplayModeParametersKHR*)(nullptr));
    }

    operator const VkDisplayModeParametersKHR&() const
    {
      return m_displayModeParametersKHR;
    }

  private:
    VkDisplayModeParametersKHR m_displayModeParametersKHR;
  };
  static_assert( sizeof( DisplayModeParametersKHR ) == sizeof( VkDisplayModeParametersKHR ), "struct and wrapper have different size!" );

  class DisplayModePropertiesKHR
  {
  public:
    DisplayModePropertiesKHR()
      : DisplayModePropertiesKHR( DisplayModeKHR(), DisplayModeParametersKHR() )
    {}

    DisplayModePropertiesKHR( DisplayModeKHR displayMode, DisplayModeParametersKHR parameters)
    {
      m_displayModePropertiesKHR.displayMode = static_cast<VkDisplayModeKHR>( displayMode );
      m_displayModePropertiesKHR.parameters = static_cast<VkDisplayModeParametersKHR>( parameters );
    }

    DisplayModePropertiesKHR(VkDisplayModePropertiesKHR const & rhs)
      : m_displayModePropertiesKHR(rhs)
    {
    }

    DisplayModePropertiesKHR& operator=(VkDisplayModePropertiesKHR const & rhs)
    {
      m_displayModePropertiesKHR = rhs;
      return *this;
    }

    const DisplayModeKHR& displayMode() const
    {
      return reinterpret_cast<const DisplayModeKHR&>( m_displayModePropertiesKHR.displayMode );
    }

    DisplayModeKHR& displayMode()
    {
      return reinterpret_cast<DisplayModeKHR&>( m_displayModePropertiesKHR.displayMode );
    }

    DisplayModePropertiesKHR& displayMode( DisplayModeKHR displayMode )
    {
      m_displayModePropertiesKHR.displayMode = static_cast<VkDisplayModeKHR>( displayMode );
      return *this;
    }

    const DisplayModeParametersKHR& parameters() const
    {
      return reinterpret_cast<const DisplayModeParametersKHR&>( m_displayModePropertiesKHR.parameters );
    }

    DisplayModeParametersKHR& parameters()
    {
      return reinterpret_cast<DisplayModeParametersKHR&>( m_displayModePropertiesKHR.parameters );
    }

    DisplayModePropertiesKHR& parameters( DisplayModeParametersKHR parameters )
    {
      m_displayModePropertiesKHR.parameters = static_cast<VkDisplayModeParametersKHR>( parameters );
      return *this;
    }

    static DisplayModePropertiesKHR& null()
    {
      return *((DisplayModePropertiesKHR*)(nullptr));
    }

    operator const VkDisplayModePropertiesKHR&() const
    {
      return m_displayModePropertiesKHR;
    }

  private:
    VkDisplayModePropertiesKHR m_displayModePropertiesKHR;
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

  class DescriptorImageInfo
  {
  public:
    DescriptorImageInfo()
      : DescriptorImageInfo( Sampler(), ImageView(), ImageLayout::eUndefined )
    {}

    DescriptorImageInfo( Sampler sampler, ImageView imageView, ImageLayout imageLayout)
    {
      m_descriptorImageInfo.sampler = static_cast<VkSampler>( sampler );
      m_descriptorImageInfo.imageView = static_cast<VkImageView>( imageView );
      m_descriptorImageInfo.imageLayout = static_cast<VkImageLayout>( imageLayout );
    }

    DescriptorImageInfo(VkDescriptorImageInfo const & rhs)
      : m_descriptorImageInfo(rhs)
    {
    }

    DescriptorImageInfo& operator=(VkDescriptorImageInfo const & rhs)
    {
      m_descriptorImageInfo = rhs;
      return *this;
    }

    const Sampler& sampler() const
    {
      return reinterpret_cast<const Sampler&>( m_descriptorImageInfo.sampler );
    }

    Sampler& sampler()
    {
      return reinterpret_cast<Sampler&>( m_descriptorImageInfo.sampler );
    }

    DescriptorImageInfo& sampler( Sampler sampler )
    {
      m_descriptorImageInfo.sampler = static_cast<VkSampler>( sampler );
      return *this;
    }

    const ImageView& imageView() const
    {
      return reinterpret_cast<const ImageView&>( m_descriptorImageInfo.imageView );
    }

    ImageView& imageView()
    {
      return reinterpret_cast<ImageView&>( m_descriptorImageInfo.imageView );
    }

    DescriptorImageInfo& imageView( ImageView imageView )
    {
      m_descriptorImageInfo.imageView = static_cast<VkImageView>( imageView );
      return *this;
    }

    const ImageLayout& imageLayout() const
    {
      return reinterpret_cast<const ImageLayout&>( m_descriptorImageInfo.imageLayout );
    }

    ImageLayout& imageLayout()
    {
      return reinterpret_cast<ImageLayout&>( m_descriptorImageInfo.imageLayout );
    }

    DescriptorImageInfo& imageLayout( ImageLayout imageLayout )
    {
      m_descriptorImageInfo.imageLayout = static_cast<VkImageLayout>( imageLayout );
      return *this;
    }

    static DescriptorImageInfo& null()
    {
      return *((DescriptorImageInfo*)(nullptr));
    }

    operator const VkDescriptorImageInfo&() const
    {
      return m_descriptorImageInfo;
    }

  private:
    VkDescriptorImageInfo m_descriptorImageInfo;
  };
  static_assert( sizeof( DescriptorImageInfo ) == sizeof( VkDescriptorImageInfo ), "struct and wrapper have different size!" );

  class AttachmentReference
  {
  public:
    AttachmentReference()
      : AttachmentReference( 0, ImageLayout::eUndefined )
    {}

    AttachmentReference( uint32_t attachment, ImageLayout layout)
    {
      m_attachmentReference.attachment = attachment;
      m_attachmentReference.layout = static_cast<VkImageLayout>( layout );
    }

    AttachmentReference(VkAttachmentReference const & rhs)
      : m_attachmentReference(rhs)
    {
    }

    AttachmentReference& operator=(VkAttachmentReference const & rhs)
    {
      m_attachmentReference = rhs;
      return *this;
    }

    const uint32_t& attachment() const
    {
      return m_attachmentReference.attachment;
    }

    uint32_t& attachment()
    {
      return m_attachmentReference.attachment;
    }

    AttachmentReference& attachment( uint32_t attachment )
    {
      m_attachmentReference.attachment = attachment;
      return *this;
    }

    const ImageLayout& layout() const
    {
      return reinterpret_cast<const ImageLayout&>( m_attachmentReference.layout );
    }

    ImageLayout& layout()
    {
      return reinterpret_cast<ImageLayout&>( m_attachmentReference.layout );
    }

    AttachmentReference& layout( ImageLayout layout )
    {
      m_attachmentReference.layout = static_cast<VkImageLayout>( layout );
      return *this;
    }

    static AttachmentReference& null()
    {
      return *((AttachmentReference*)(nullptr));
    }

    operator const VkAttachmentReference&() const
    {
      return m_attachmentReference;
    }

  private:
    VkAttachmentReference m_attachmentReference;
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

  class ComponentMapping
  {
  public:
    ComponentMapping()
      : ComponentMapping( ComponentSwizzle::eIdentity, ComponentSwizzle::eIdentity, ComponentSwizzle::eIdentity, ComponentSwizzle::eIdentity )
    {}

    ComponentMapping( ComponentSwizzle r, ComponentSwizzle g, ComponentSwizzle b, ComponentSwizzle a)
    {
      m_componentMapping.r = static_cast<VkComponentSwizzle>( r );
      m_componentMapping.g = static_cast<VkComponentSwizzle>( g );
      m_componentMapping.b = static_cast<VkComponentSwizzle>( b );
      m_componentMapping.a = static_cast<VkComponentSwizzle>( a );
    }

    ComponentMapping(VkComponentMapping const & rhs)
      : m_componentMapping(rhs)
    {
    }

    ComponentMapping& operator=(VkComponentMapping const & rhs)
    {
      m_componentMapping = rhs;
      return *this;
    }

    const ComponentSwizzle& r() const
    {
      return reinterpret_cast<const ComponentSwizzle&>( m_componentMapping.r );
    }

    ComponentSwizzle& r()
    {
      return reinterpret_cast<ComponentSwizzle&>( m_componentMapping.r );
    }

    ComponentMapping& r( ComponentSwizzle r )
    {
      m_componentMapping.r = static_cast<VkComponentSwizzle>( r );
      return *this;
    }

    const ComponentSwizzle& g() const
    {
      return reinterpret_cast<const ComponentSwizzle&>( m_componentMapping.g );
    }

    ComponentSwizzle& g()
    {
      return reinterpret_cast<ComponentSwizzle&>( m_componentMapping.g );
    }

    ComponentMapping& g( ComponentSwizzle g )
    {
      m_componentMapping.g = static_cast<VkComponentSwizzle>( g );
      return *this;
    }

    const ComponentSwizzle& b() const
    {
      return reinterpret_cast<const ComponentSwizzle&>( m_componentMapping.b );
    }

    ComponentSwizzle& b()
    {
      return reinterpret_cast<ComponentSwizzle&>( m_componentMapping.b );
    }

    ComponentMapping& b( ComponentSwizzle b )
    {
      m_componentMapping.b = static_cast<VkComponentSwizzle>( b );
      return *this;
    }

    const ComponentSwizzle& a() const
    {
      return reinterpret_cast<const ComponentSwizzle&>( m_componentMapping.a );
    }

    ComponentSwizzle& a()
    {
      return reinterpret_cast<ComponentSwizzle&>( m_componentMapping.a );
    }

    ComponentMapping& a( ComponentSwizzle a )
    {
      m_componentMapping.a = static_cast<VkComponentSwizzle>( a );
      return *this;
    }

    static ComponentMapping& null()
    {
      return *((ComponentMapping*)(nullptr));
    }

    operator const VkComponentMapping&() const
    {
      return m_componentMapping;
    }

  private:
    VkComponentMapping m_componentMapping;
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

  class DescriptorPoolSize
  {
  public:
    DescriptorPoolSize()
      : DescriptorPoolSize( DescriptorType::eSampler, 0 )
    {}

    DescriptorPoolSize( DescriptorType type, uint32_t descriptorCount)
    {
      m_descriptorPoolSize.type = static_cast<VkDescriptorType>( type );
      m_descriptorPoolSize.descriptorCount = descriptorCount;
    }

    DescriptorPoolSize(VkDescriptorPoolSize const & rhs)
      : m_descriptorPoolSize(rhs)
    {
    }

    DescriptorPoolSize& operator=(VkDescriptorPoolSize const & rhs)
    {
      m_descriptorPoolSize = rhs;
      return *this;
    }

    const DescriptorType& type() const
    {
      return reinterpret_cast<const DescriptorType&>( m_descriptorPoolSize.type );
    }

    DescriptorType& type()
    {
      return reinterpret_cast<DescriptorType&>( m_descriptorPoolSize.type );
    }

    DescriptorPoolSize& type( DescriptorType type )
    {
      m_descriptorPoolSize.type = static_cast<VkDescriptorType>( type );
      return *this;
    }

    const uint32_t& descriptorCount() const
    {
      return m_descriptorPoolSize.descriptorCount;
    }

    uint32_t& descriptorCount()
    {
      return m_descriptorPoolSize.descriptorCount;
    }

    DescriptorPoolSize& descriptorCount( uint32_t descriptorCount )
    {
      m_descriptorPoolSize.descriptorCount = descriptorCount;
      return *this;
    }

    static DescriptorPoolSize& null()
    {
      return *((DescriptorPoolSize*)(nullptr));
    }

    operator const VkDescriptorPoolSize&() const
    {
      return m_descriptorPoolSize;
    }

  private:
    VkDescriptorPoolSize m_descriptorPoolSize;
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

  class SubpassDescription
  {
  public:
    SubpassDescription()
      : SubpassDescription( SubpassDescriptionFlags(), PipelineBindPoint::eGraphics, 0, nullptr, 0, nullptr, nullptr, nullptr, 0, nullptr )
    {}

    SubpassDescription( SubpassDescriptionFlags flags, PipelineBindPoint pipelineBindPoint, uint32_t inputAttachmentCount, const AttachmentReference* pInputAttachments, uint32_t colorAttachmentCount, const AttachmentReference* pColorAttachments, const AttachmentReference* pResolveAttachments, const AttachmentReference* pDepthStencilAttachment, uint32_t preserveAttachmentCount, const uint32_t* pPreserveAttachments)
    {
      m_subpassDescription.flags = static_cast<VkSubpassDescriptionFlags>( flags );
      m_subpassDescription.pipelineBindPoint = static_cast<VkPipelineBindPoint>( pipelineBindPoint );
      m_subpassDescription.inputAttachmentCount = inputAttachmentCount;
      m_subpassDescription.pInputAttachments = reinterpret_cast<const VkAttachmentReference*>( pInputAttachments );
      m_subpassDescription.colorAttachmentCount = colorAttachmentCount;
      m_subpassDescription.pColorAttachments = reinterpret_cast<const VkAttachmentReference*>( pColorAttachments );
      m_subpassDescription.pResolveAttachments = reinterpret_cast<const VkAttachmentReference*>( pResolveAttachments );
      m_subpassDescription.pDepthStencilAttachment = reinterpret_cast<const VkAttachmentReference*>( pDepthStencilAttachment );
      m_subpassDescription.preserveAttachmentCount = preserveAttachmentCount;
      m_subpassDescription.pPreserveAttachments = pPreserveAttachments;
    }

    SubpassDescription(VkSubpassDescription const & rhs)
      : m_subpassDescription(rhs)
    {
    }

    SubpassDescription& operator=(VkSubpassDescription const & rhs)
    {
      m_subpassDescription = rhs;
      return *this;
    }

    const SubpassDescriptionFlags& flags() const
    {
      return reinterpret_cast<const SubpassDescriptionFlags&>( m_subpassDescription.flags );
    }

    SubpassDescriptionFlags& flags()
    {
      return reinterpret_cast<SubpassDescriptionFlags&>( m_subpassDescription.flags );
    }

    SubpassDescription& flags( SubpassDescriptionFlags flags )
    {
      m_subpassDescription.flags = static_cast<VkSubpassDescriptionFlags>( flags );
      return *this;
    }

    const PipelineBindPoint& pipelineBindPoint() const
    {
      return reinterpret_cast<const PipelineBindPoint&>( m_subpassDescription.pipelineBindPoint );
    }

    PipelineBindPoint& pipelineBindPoint()
    {
      return reinterpret_cast<PipelineBindPoint&>( m_subpassDescription.pipelineBindPoint );
    }

    SubpassDescription& pipelineBindPoint( PipelineBindPoint pipelineBindPoint )
    {
      m_subpassDescription.pipelineBindPoint = static_cast<VkPipelineBindPoint>( pipelineBindPoint );
      return *this;
    }

    const uint32_t& inputAttachmentCount() const
    {
      return m_subpassDescription.inputAttachmentCount;
    }

    uint32_t& inputAttachmentCount()
    {
      return m_subpassDescription.inputAttachmentCount;
    }

    SubpassDescription& inputAttachmentCount( uint32_t inputAttachmentCount )
    {
      m_subpassDescription.inputAttachmentCount = inputAttachmentCount;
      return *this;
    }

    const AttachmentReference* pInputAttachments() const
    {
      return reinterpret_cast<const AttachmentReference*>( m_subpassDescription.pInputAttachments );
    }

    const AttachmentReference* pInputAttachments()
    {
      return reinterpret_cast<const AttachmentReference*>( m_subpassDescription.pInputAttachments );
    }

    SubpassDescription& pInputAttachments( const AttachmentReference* pInputAttachments )
    {
      m_subpassDescription.pInputAttachments = reinterpret_cast<const VkAttachmentReference*>( pInputAttachments );
      return *this;
    }

    const uint32_t& colorAttachmentCount() const
    {
      return m_subpassDescription.colorAttachmentCount;
    }

    uint32_t& colorAttachmentCount()
    {
      return m_subpassDescription.colorAttachmentCount;
    }

    SubpassDescription& colorAttachmentCount( uint32_t colorAttachmentCount )
    {
      m_subpassDescription.colorAttachmentCount = colorAttachmentCount;
      return *this;
    }

    const AttachmentReference* pColorAttachments() const
    {
      return reinterpret_cast<const AttachmentReference*>( m_subpassDescription.pColorAttachments );
    }

    const AttachmentReference* pColorAttachments()
    {
      return reinterpret_cast<const AttachmentReference*>( m_subpassDescription.pColorAttachments );
    }

    SubpassDescription& pColorAttachments( const AttachmentReference* pColorAttachments )
    {
      m_subpassDescription.pColorAttachments = reinterpret_cast<const VkAttachmentReference*>( pColorAttachments );
      return *this;
    }

    const AttachmentReference* pResolveAttachments() const
    {
      return reinterpret_cast<const AttachmentReference*>( m_subpassDescription.pResolveAttachments );
    }

    const AttachmentReference* pResolveAttachments()
    {
      return reinterpret_cast<const AttachmentReference*>( m_subpassDescription.pResolveAttachments );
    }

    SubpassDescription& pResolveAttachments( const AttachmentReference* pResolveAttachments )
    {
      m_subpassDescription.pResolveAttachments = reinterpret_cast<const VkAttachmentReference*>( pResolveAttachments );
      return *this;
    }

    const AttachmentReference* pDepthStencilAttachment() const
    {
      return reinterpret_cast<const AttachmentReference*>( m_subpassDescription.pDepthStencilAttachment );
    }

    const AttachmentReference* pDepthStencilAttachment()
    {
      return reinterpret_cast<const AttachmentReference*>( m_subpassDescription.pDepthStencilAttachment );
    }

    SubpassDescription& pDepthStencilAttachment( const AttachmentReference* pDepthStencilAttachment )
    {
      m_subpassDescription.pDepthStencilAttachment = reinterpret_cast<const VkAttachmentReference*>( pDepthStencilAttachment );
      return *this;
    }

    const uint32_t& preserveAttachmentCount() const
    {
      return m_subpassDescription.preserveAttachmentCount;
    }

    uint32_t& preserveAttachmentCount()
    {
      return m_subpassDescription.preserveAttachmentCount;
    }

    SubpassDescription& preserveAttachmentCount( uint32_t preserveAttachmentCount )
    {
      m_subpassDescription.preserveAttachmentCount = preserveAttachmentCount;
      return *this;
    }

    const uint32_t* pPreserveAttachments() const
    {
      return reinterpret_cast<const uint32_t*>( m_subpassDescription.pPreserveAttachments );
    }

    const uint32_t* pPreserveAttachments()
    {
      return reinterpret_cast<const uint32_t*>( m_subpassDescription.pPreserveAttachments );
    }

    SubpassDescription& pPreserveAttachments( const uint32_t* pPreserveAttachments )
    {
      m_subpassDescription.pPreserveAttachments = pPreserveAttachments;
      return *this;
    }

    static SubpassDescription& null()
    {
      return *((SubpassDescription*)(nullptr));
    }

    operator const VkSubpassDescription&() const
    {
      return m_subpassDescription;
    }

  private:
    VkSubpassDescription m_subpassDescription;
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
    eLinear = VK_FILTER_LINEAR
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

  typedef Flags<CullModeFlagBits, VkCullModeFlags> CullModeFlags;

  inline CullModeFlags operator|( CullModeFlagBits bit0, CullModeFlagBits bit1 )
  {
    return CullModeFlags( bit0 ) | bit1;
  }

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

  class StencilOpState
  {
  public:
    StencilOpState()
      : StencilOpState( StencilOp::eKeep, StencilOp::eKeep, StencilOp::eKeep, CompareOp::eNever, 0, 0, 0 )
    {}

    StencilOpState( StencilOp failOp, StencilOp passOp, StencilOp depthFailOp, CompareOp compareOp, uint32_t compareMask, uint32_t writeMask, uint32_t reference)
    {
      m_stencilOpState.failOp = static_cast<VkStencilOp>( failOp );
      m_stencilOpState.passOp = static_cast<VkStencilOp>( passOp );
      m_stencilOpState.depthFailOp = static_cast<VkStencilOp>( depthFailOp );
      m_stencilOpState.compareOp = static_cast<VkCompareOp>( compareOp );
      m_stencilOpState.compareMask = compareMask;
      m_stencilOpState.writeMask = writeMask;
      m_stencilOpState.reference = reference;
    }

    StencilOpState(VkStencilOpState const & rhs)
      : m_stencilOpState(rhs)
    {
    }

    StencilOpState& operator=(VkStencilOpState const & rhs)
    {
      m_stencilOpState = rhs;
      return *this;
    }

    const StencilOp& failOp() const
    {
      return reinterpret_cast<const StencilOp&>( m_stencilOpState.failOp );
    }

    StencilOp& failOp()
    {
      return reinterpret_cast<StencilOp&>( m_stencilOpState.failOp );
    }

    StencilOpState& failOp( StencilOp failOp )
    {
      m_stencilOpState.failOp = static_cast<VkStencilOp>( failOp );
      return *this;
    }

    const StencilOp& passOp() const
    {
      return reinterpret_cast<const StencilOp&>( m_stencilOpState.passOp );
    }

    StencilOp& passOp()
    {
      return reinterpret_cast<StencilOp&>( m_stencilOpState.passOp );
    }

    StencilOpState& passOp( StencilOp passOp )
    {
      m_stencilOpState.passOp = static_cast<VkStencilOp>( passOp );
      return *this;
    }

    const StencilOp& depthFailOp() const
    {
      return reinterpret_cast<const StencilOp&>( m_stencilOpState.depthFailOp );
    }

    StencilOp& depthFailOp()
    {
      return reinterpret_cast<StencilOp&>( m_stencilOpState.depthFailOp );
    }

    StencilOpState& depthFailOp( StencilOp depthFailOp )
    {
      m_stencilOpState.depthFailOp = static_cast<VkStencilOp>( depthFailOp );
      return *this;
    }

    const CompareOp& compareOp() const
    {
      return reinterpret_cast<const CompareOp&>( m_stencilOpState.compareOp );
    }

    CompareOp& compareOp()
    {
      return reinterpret_cast<CompareOp&>( m_stencilOpState.compareOp );
    }

    StencilOpState& compareOp( CompareOp compareOp )
    {
      m_stencilOpState.compareOp = static_cast<VkCompareOp>( compareOp );
      return *this;
    }

    const uint32_t& compareMask() const
    {
      return m_stencilOpState.compareMask;
    }

    uint32_t& compareMask()
    {
      return m_stencilOpState.compareMask;
    }

    StencilOpState& compareMask( uint32_t compareMask )
    {
      m_stencilOpState.compareMask = compareMask;
      return *this;
    }

    const uint32_t& writeMask() const
    {
      return m_stencilOpState.writeMask;
    }

    uint32_t& writeMask()
    {
      return m_stencilOpState.writeMask;
    }

    StencilOpState& writeMask( uint32_t writeMask )
    {
      m_stencilOpState.writeMask = writeMask;
      return *this;
    }

    const uint32_t& reference() const
    {
      return m_stencilOpState.reference;
    }

    uint32_t& reference()
    {
      return m_stencilOpState.reference;
    }

    StencilOpState& reference( uint32_t reference )
    {
      m_stencilOpState.reference = reference;
      return *this;
    }

    static StencilOpState& null()
    {
      return *((StencilOpState*)(nullptr));
    }

    operator const VkStencilOpState&() const
    {
      return m_stencilOpState;
    }

  private:
    VkStencilOpState m_stencilOpState;
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

  class VertexInputBindingDescription
  {
  public:
    VertexInputBindingDescription()
      : VertexInputBindingDescription( 0, 0, VertexInputRate::eVertex )
    {}

    VertexInputBindingDescription( uint32_t binding, uint32_t stride, VertexInputRate inputRate)
    {
      m_vertexInputBindingDescription.binding = binding;
      m_vertexInputBindingDescription.stride = stride;
      m_vertexInputBindingDescription.inputRate = static_cast<VkVertexInputRate>( inputRate );
    }

    VertexInputBindingDescription(VkVertexInputBindingDescription const & rhs)
      : m_vertexInputBindingDescription(rhs)
    {
    }

    VertexInputBindingDescription& operator=(VkVertexInputBindingDescription const & rhs)
    {
      m_vertexInputBindingDescription = rhs;
      return *this;
    }

    const uint32_t& binding() const
    {
      return m_vertexInputBindingDescription.binding;
    }

    uint32_t& binding()
    {
      return m_vertexInputBindingDescription.binding;
    }

    VertexInputBindingDescription& binding( uint32_t binding )
    {
      m_vertexInputBindingDescription.binding = binding;
      return *this;
    }

    const uint32_t& stride() const
    {
      return m_vertexInputBindingDescription.stride;
    }

    uint32_t& stride()
    {
      return m_vertexInputBindingDescription.stride;
    }

    VertexInputBindingDescription& stride( uint32_t stride )
    {
      m_vertexInputBindingDescription.stride = stride;
      return *this;
    }

    const VertexInputRate& inputRate() const
    {
      return reinterpret_cast<const VertexInputRate&>( m_vertexInputBindingDescription.inputRate );
    }

    VertexInputRate& inputRate()
    {
      return reinterpret_cast<VertexInputRate&>( m_vertexInputBindingDescription.inputRate );
    }

    VertexInputBindingDescription& inputRate( VertexInputRate inputRate )
    {
      m_vertexInputBindingDescription.inputRate = static_cast<VkVertexInputRate>( inputRate );
      return *this;
    }

    static VertexInputBindingDescription& null()
    {
      return *((VertexInputBindingDescription*)(nullptr));
    }

    operator const VkVertexInputBindingDescription&() const
    {
      return m_vertexInputBindingDescription;
    }

  private:
    VkVertexInputBindingDescription m_vertexInputBindingDescription;
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
    eAstc12x12SrgbBlock = VK_FORMAT_ASTC_12x12_SRGB_BLOCK
  };

  class VertexInputAttributeDescription
  {
  public:
    VertexInputAttributeDescription()
      : VertexInputAttributeDescription( 0, 0, Format::eUndefined, 0 )
    {}

    VertexInputAttributeDescription( uint32_t location, uint32_t binding, Format format, uint32_t offset)
    {
      m_vertexInputAttributeDescription.location = location;
      m_vertexInputAttributeDescription.binding = binding;
      m_vertexInputAttributeDescription.format = static_cast<VkFormat>( format );
      m_vertexInputAttributeDescription.offset = offset;
    }

    VertexInputAttributeDescription(VkVertexInputAttributeDescription const & rhs)
      : m_vertexInputAttributeDescription(rhs)
    {
    }

    VertexInputAttributeDescription& operator=(VkVertexInputAttributeDescription const & rhs)
    {
      m_vertexInputAttributeDescription = rhs;
      return *this;
    }

    const uint32_t& location() const
    {
      return m_vertexInputAttributeDescription.location;
    }

    uint32_t& location()
    {
      return m_vertexInputAttributeDescription.location;
    }

    VertexInputAttributeDescription& location( uint32_t location )
    {
      m_vertexInputAttributeDescription.location = location;
      return *this;
    }

    const uint32_t& binding() const
    {
      return m_vertexInputAttributeDescription.binding;
    }

    uint32_t& binding()
    {
      return m_vertexInputAttributeDescription.binding;
    }

    VertexInputAttributeDescription& binding( uint32_t binding )
    {
      m_vertexInputAttributeDescription.binding = binding;
      return *this;
    }

    const Format& format() const
    {
      return reinterpret_cast<const Format&>( m_vertexInputAttributeDescription.format );
    }

    Format& format()
    {
      return reinterpret_cast<Format&>( m_vertexInputAttributeDescription.format );
    }

    VertexInputAttributeDescription& format( Format format )
    {
      m_vertexInputAttributeDescription.format = static_cast<VkFormat>( format );
      return *this;
    }

    const uint32_t& offset() const
    {
      return m_vertexInputAttributeDescription.offset;
    }

    uint32_t& offset()
    {
      return m_vertexInputAttributeDescription.offset;
    }

    VertexInputAttributeDescription& offset( uint32_t offset )
    {
      m_vertexInputAttributeDescription.offset = offset;
      return *this;
    }

    static VertexInputAttributeDescription& null()
    {
      return *((VertexInputAttributeDescription*)(nullptr));
    }

    operator const VkVertexInputAttributeDescription&() const
    {
      return m_vertexInputAttributeDescription;
    }

  private:
    VkVertexInputAttributeDescription m_vertexInputAttributeDescription;
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
    eDebugReportCallbackCreateInfoEXT = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT
  };

  class ApplicationInfo
  {
  public:
    ApplicationInfo()
      : ApplicationInfo( nullptr, 0, nullptr, 0, 0 )
    {}

    ApplicationInfo( const char* pApplicationName, uint32_t applicationVersion, const char* pEngineName, uint32_t engineVersion, uint32_t apiVersion)
    {
      m_applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      m_applicationInfo.pNext = nullptr;
      m_applicationInfo.pApplicationName = pApplicationName;
      m_applicationInfo.applicationVersion = applicationVersion;
      m_applicationInfo.pEngineName = pEngineName;
      m_applicationInfo.engineVersion = engineVersion;
      m_applicationInfo.apiVersion = apiVersion;
    }

    ApplicationInfo(VkApplicationInfo const & rhs)
      : m_applicationInfo(rhs)
    {
    }

    ApplicationInfo& operator=(VkApplicationInfo const & rhs)
    {
      m_applicationInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_applicationInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_applicationInfo.sType );
    }

    ApplicationInfo& sType( StructureType sType )
    {
      m_applicationInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_applicationInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_applicationInfo.pNext );
    }

    ApplicationInfo& pNext( const void* pNext )
    {
      m_applicationInfo.pNext = pNext;
      return *this;
    }

    const char* pApplicationName() const
    {
      return reinterpret_cast<const char*>( m_applicationInfo.pApplicationName );
    }

    const char* pApplicationName()
    {
      return reinterpret_cast<const char*>( m_applicationInfo.pApplicationName );
    }

    ApplicationInfo& pApplicationName( const char* pApplicationName )
    {
      m_applicationInfo.pApplicationName = pApplicationName;
      return *this;
    }

    const uint32_t& applicationVersion() const
    {
      return m_applicationInfo.applicationVersion;
    }

    uint32_t& applicationVersion()
    {
      return m_applicationInfo.applicationVersion;
    }

    ApplicationInfo& applicationVersion( uint32_t applicationVersion )
    {
      m_applicationInfo.applicationVersion = applicationVersion;
      return *this;
    }

    const char* pEngineName() const
    {
      return reinterpret_cast<const char*>( m_applicationInfo.pEngineName );
    }

    const char* pEngineName()
    {
      return reinterpret_cast<const char*>( m_applicationInfo.pEngineName );
    }

    ApplicationInfo& pEngineName( const char* pEngineName )
    {
      m_applicationInfo.pEngineName = pEngineName;
      return *this;
    }

    const uint32_t& engineVersion() const
    {
      return m_applicationInfo.engineVersion;
    }

    uint32_t& engineVersion()
    {
      return m_applicationInfo.engineVersion;
    }

    ApplicationInfo& engineVersion( uint32_t engineVersion )
    {
      m_applicationInfo.engineVersion = engineVersion;
      return *this;
    }

    const uint32_t& apiVersion() const
    {
      return m_applicationInfo.apiVersion;
    }

    uint32_t& apiVersion()
    {
      return m_applicationInfo.apiVersion;
    }

    ApplicationInfo& apiVersion( uint32_t apiVersion )
    {
      m_applicationInfo.apiVersion = apiVersion;
      return *this;
    }

    static ApplicationInfo& null()
    {
      return *((ApplicationInfo*)(nullptr));
    }

    operator const VkApplicationInfo&() const
    {
      return m_applicationInfo;
    }

  private:
    VkApplicationInfo m_applicationInfo;
  };
  static_assert( sizeof( ApplicationInfo ) == sizeof( VkApplicationInfo ), "struct and wrapper have different size!" );

  class DeviceQueueCreateInfo
  {
  public:
    DeviceQueueCreateInfo()
      : DeviceQueueCreateInfo( DeviceQueueCreateFlags(), 0, 0, nullptr )
    {}

    DeviceQueueCreateInfo( DeviceQueueCreateFlags flags, uint32_t queueFamilyIndex, uint32_t queueCount, const float* pQueuePriorities)
    {
      m_deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      m_deviceQueueCreateInfo.pNext = nullptr;
      m_deviceQueueCreateInfo.flags = static_cast<VkDeviceQueueCreateFlags>( flags );
      m_deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
      m_deviceQueueCreateInfo.queueCount = queueCount;
      m_deviceQueueCreateInfo.pQueuePriorities = pQueuePriorities;
    }

    DeviceQueueCreateInfo(VkDeviceQueueCreateInfo const & rhs)
      : m_deviceQueueCreateInfo(rhs)
    {
    }

    DeviceQueueCreateInfo& operator=(VkDeviceQueueCreateInfo const & rhs)
    {
      m_deviceQueueCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_deviceQueueCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_deviceQueueCreateInfo.sType );
    }

    DeviceQueueCreateInfo& sType( StructureType sType )
    {
      m_deviceQueueCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_deviceQueueCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_deviceQueueCreateInfo.pNext );
    }

    DeviceQueueCreateInfo& pNext( const void* pNext )
    {
      m_deviceQueueCreateInfo.pNext = pNext;
      return *this;
    }

    const DeviceQueueCreateFlags& flags() const
    {
      return reinterpret_cast<const DeviceQueueCreateFlags&>( m_deviceQueueCreateInfo.flags );
    }

    DeviceQueueCreateFlags& flags()
    {
      return reinterpret_cast<DeviceQueueCreateFlags&>( m_deviceQueueCreateInfo.flags );
    }

    DeviceQueueCreateInfo& flags( DeviceQueueCreateFlags flags )
    {
      m_deviceQueueCreateInfo.flags = static_cast<VkDeviceQueueCreateFlags>( flags );
      return *this;
    }

    const uint32_t& queueFamilyIndex() const
    {
      return m_deviceQueueCreateInfo.queueFamilyIndex;
    }

    uint32_t& queueFamilyIndex()
    {
      return m_deviceQueueCreateInfo.queueFamilyIndex;
    }

    DeviceQueueCreateInfo& queueFamilyIndex( uint32_t queueFamilyIndex )
    {
      m_deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
      return *this;
    }

    const uint32_t& queueCount() const
    {
      return m_deviceQueueCreateInfo.queueCount;
    }

    uint32_t& queueCount()
    {
      return m_deviceQueueCreateInfo.queueCount;
    }

    DeviceQueueCreateInfo& queueCount( uint32_t queueCount )
    {
      m_deviceQueueCreateInfo.queueCount = queueCount;
      return *this;
    }

    const float* pQueuePriorities() const
    {
      return reinterpret_cast<const float*>( m_deviceQueueCreateInfo.pQueuePriorities );
    }

    const float* pQueuePriorities()
    {
      return reinterpret_cast<const float*>( m_deviceQueueCreateInfo.pQueuePriorities );
    }

    DeviceQueueCreateInfo& pQueuePriorities( const float* pQueuePriorities )
    {
      m_deviceQueueCreateInfo.pQueuePriorities = pQueuePriorities;
      return *this;
    }

    static DeviceQueueCreateInfo& null()
    {
      return *((DeviceQueueCreateInfo*)(nullptr));
    }

    operator const VkDeviceQueueCreateInfo&() const
    {
      return m_deviceQueueCreateInfo;
    }

  private:
    VkDeviceQueueCreateInfo m_deviceQueueCreateInfo;
  };
  static_assert( sizeof( DeviceQueueCreateInfo ) == sizeof( VkDeviceQueueCreateInfo ), "struct and wrapper have different size!" );

  class DeviceCreateInfo
  {
  public:
    DeviceCreateInfo()
      : DeviceCreateInfo( DeviceCreateFlags(), 0, nullptr, 0, nullptr, 0, nullptr, nullptr )
    {}

    DeviceCreateInfo( DeviceCreateFlags flags, uint32_t queueCreateInfoCount, const DeviceQueueCreateInfo* pQueueCreateInfos, uint32_t enabledLayerCount, const char* const* ppEnabledLayerNames, uint32_t enabledExtensionCount, const char* const* ppEnabledExtensionNames, const PhysicalDeviceFeatures* pEnabledFeatures)
    {
      m_deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
      m_deviceCreateInfo.pNext = nullptr;
      m_deviceCreateInfo.flags = static_cast<VkDeviceCreateFlags>( flags );
      m_deviceCreateInfo.queueCreateInfoCount = queueCreateInfoCount;
      m_deviceCreateInfo.pQueueCreateInfos = reinterpret_cast<const VkDeviceQueueCreateInfo*>( pQueueCreateInfos );
      m_deviceCreateInfo.enabledLayerCount = enabledLayerCount;
      m_deviceCreateInfo.ppEnabledLayerNames = ppEnabledLayerNames;
      m_deviceCreateInfo.enabledExtensionCount = enabledExtensionCount;
      m_deviceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames;
      m_deviceCreateInfo.pEnabledFeatures = reinterpret_cast<const VkPhysicalDeviceFeatures*>( pEnabledFeatures );
    }

    DeviceCreateInfo(VkDeviceCreateInfo const & rhs)
      : m_deviceCreateInfo(rhs)
    {
    }

    DeviceCreateInfo& operator=(VkDeviceCreateInfo const & rhs)
    {
      m_deviceCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_deviceCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_deviceCreateInfo.sType );
    }

    DeviceCreateInfo& sType( StructureType sType )
    {
      m_deviceCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_deviceCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_deviceCreateInfo.pNext );
    }

    DeviceCreateInfo& pNext( const void* pNext )
    {
      m_deviceCreateInfo.pNext = pNext;
      return *this;
    }

    const DeviceCreateFlags& flags() const
    {
      return reinterpret_cast<const DeviceCreateFlags&>( m_deviceCreateInfo.flags );
    }

    DeviceCreateFlags& flags()
    {
      return reinterpret_cast<DeviceCreateFlags&>( m_deviceCreateInfo.flags );
    }

    DeviceCreateInfo& flags( DeviceCreateFlags flags )
    {
      m_deviceCreateInfo.flags = static_cast<VkDeviceCreateFlags>( flags );
      return *this;
    }

    const uint32_t& queueCreateInfoCount() const
    {
      return m_deviceCreateInfo.queueCreateInfoCount;
    }

    uint32_t& queueCreateInfoCount()
    {
      return m_deviceCreateInfo.queueCreateInfoCount;
    }

    DeviceCreateInfo& queueCreateInfoCount( uint32_t queueCreateInfoCount )
    {
      m_deviceCreateInfo.queueCreateInfoCount = queueCreateInfoCount;
      return *this;
    }

    const DeviceQueueCreateInfo* pQueueCreateInfos() const
    {
      return reinterpret_cast<const DeviceQueueCreateInfo*>( m_deviceCreateInfo.pQueueCreateInfos );
    }

    const DeviceQueueCreateInfo* pQueueCreateInfos()
    {
      return reinterpret_cast<const DeviceQueueCreateInfo*>( m_deviceCreateInfo.pQueueCreateInfos );
    }

    DeviceCreateInfo& pQueueCreateInfos( const DeviceQueueCreateInfo* pQueueCreateInfos )
    {
      m_deviceCreateInfo.pQueueCreateInfos = reinterpret_cast<const VkDeviceQueueCreateInfo*>( pQueueCreateInfos );
      return *this;
    }

    const uint32_t& enabledLayerCount() const
    {
      return m_deviceCreateInfo.enabledLayerCount;
    }

    uint32_t& enabledLayerCount()
    {
      return m_deviceCreateInfo.enabledLayerCount;
    }

    DeviceCreateInfo& enabledLayerCount( uint32_t enabledLayerCount )
    {
      m_deviceCreateInfo.enabledLayerCount = enabledLayerCount;
      return *this;
    }

    const char* const* ppEnabledLayerNames() const
    {
      return reinterpret_cast<const char* const*>( m_deviceCreateInfo.ppEnabledLayerNames );
    }

    const char* const* ppEnabledLayerNames()
    {
      return reinterpret_cast<const char* const*>( m_deviceCreateInfo.ppEnabledLayerNames );
    }

    DeviceCreateInfo& ppEnabledLayerNames( const char* const* ppEnabledLayerNames )
    {
      m_deviceCreateInfo.ppEnabledLayerNames = ppEnabledLayerNames;
      return *this;
    }

    const uint32_t& enabledExtensionCount() const
    {
      return m_deviceCreateInfo.enabledExtensionCount;
    }

    uint32_t& enabledExtensionCount()
    {
      return m_deviceCreateInfo.enabledExtensionCount;
    }

    DeviceCreateInfo& enabledExtensionCount( uint32_t enabledExtensionCount )
    {
      m_deviceCreateInfo.enabledExtensionCount = enabledExtensionCount;
      return *this;
    }

    const char* const* ppEnabledExtensionNames() const
    {
      return reinterpret_cast<const char* const*>( m_deviceCreateInfo.ppEnabledExtensionNames );
    }

    const char* const* ppEnabledExtensionNames()
    {
      return reinterpret_cast<const char* const*>( m_deviceCreateInfo.ppEnabledExtensionNames );
    }

    DeviceCreateInfo& ppEnabledExtensionNames( const char* const* ppEnabledExtensionNames )
    {
      m_deviceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames;
      return *this;
    }

    const PhysicalDeviceFeatures* pEnabledFeatures() const
    {
      return reinterpret_cast<const PhysicalDeviceFeatures*>( m_deviceCreateInfo.pEnabledFeatures );
    }

    const PhysicalDeviceFeatures* pEnabledFeatures()
    {
      return reinterpret_cast<const PhysicalDeviceFeatures*>( m_deviceCreateInfo.pEnabledFeatures );
    }

    DeviceCreateInfo& pEnabledFeatures( const PhysicalDeviceFeatures* pEnabledFeatures )
    {
      m_deviceCreateInfo.pEnabledFeatures = reinterpret_cast<const VkPhysicalDeviceFeatures*>( pEnabledFeatures );
      return *this;
    }

    static DeviceCreateInfo& null()
    {
      return *((DeviceCreateInfo*)(nullptr));
    }

    operator const VkDeviceCreateInfo&() const
    {
      return m_deviceCreateInfo;
    }

  private:
    VkDeviceCreateInfo m_deviceCreateInfo;
  };
  static_assert( sizeof( DeviceCreateInfo ) == sizeof( VkDeviceCreateInfo ), "struct and wrapper have different size!" );

  class InstanceCreateInfo
  {
  public:
    InstanceCreateInfo()
      : InstanceCreateInfo( InstanceCreateFlags(), nullptr, 0, nullptr, 0, nullptr )
    {}

    InstanceCreateInfo( InstanceCreateFlags flags, const ApplicationInfo* pApplicationInfo, uint32_t enabledLayerCount, const char* const* ppEnabledLayerNames, uint32_t enabledExtensionCount, const char* const* ppEnabledExtensionNames)
    {
      m_instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      m_instanceCreateInfo.pNext = nullptr;
      m_instanceCreateInfo.flags = static_cast<VkInstanceCreateFlags>( flags );
      m_instanceCreateInfo.pApplicationInfo = reinterpret_cast<const VkApplicationInfo*>( pApplicationInfo );
      m_instanceCreateInfo.enabledLayerCount = enabledLayerCount;
      m_instanceCreateInfo.ppEnabledLayerNames = ppEnabledLayerNames;
      m_instanceCreateInfo.enabledExtensionCount = enabledExtensionCount;
      m_instanceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames;
    }

    InstanceCreateInfo(VkInstanceCreateInfo const & rhs)
      : m_instanceCreateInfo(rhs)
    {
    }

    InstanceCreateInfo& operator=(VkInstanceCreateInfo const & rhs)
    {
      m_instanceCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_instanceCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_instanceCreateInfo.sType );
    }

    InstanceCreateInfo& sType( StructureType sType )
    {
      m_instanceCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_instanceCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_instanceCreateInfo.pNext );
    }

    InstanceCreateInfo& pNext( const void* pNext )
    {
      m_instanceCreateInfo.pNext = pNext;
      return *this;
    }

    const InstanceCreateFlags& flags() const
    {
      return reinterpret_cast<const InstanceCreateFlags&>( m_instanceCreateInfo.flags );
    }

    InstanceCreateFlags& flags()
    {
      return reinterpret_cast<InstanceCreateFlags&>( m_instanceCreateInfo.flags );
    }

    InstanceCreateInfo& flags( InstanceCreateFlags flags )
    {
      m_instanceCreateInfo.flags = static_cast<VkInstanceCreateFlags>( flags );
      return *this;
    }

    const ApplicationInfo* pApplicationInfo() const
    {
      return reinterpret_cast<const ApplicationInfo*>( m_instanceCreateInfo.pApplicationInfo );
    }

    const ApplicationInfo* pApplicationInfo()
    {
      return reinterpret_cast<const ApplicationInfo*>( m_instanceCreateInfo.pApplicationInfo );
    }

    InstanceCreateInfo& pApplicationInfo( const ApplicationInfo* pApplicationInfo )
    {
      m_instanceCreateInfo.pApplicationInfo = reinterpret_cast<const VkApplicationInfo*>( pApplicationInfo );
      return *this;
    }

    const uint32_t& enabledLayerCount() const
    {
      return m_instanceCreateInfo.enabledLayerCount;
    }

    uint32_t& enabledLayerCount()
    {
      return m_instanceCreateInfo.enabledLayerCount;
    }

    InstanceCreateInfo& enabledLayerCount( uint32_t enabledLayerCount )
    {
      m_instanceCreateInfo.enabledLayerCount = enabledLayerCount;
      return *this;
    }

    const char* const* ppEnabledLayerNames() const
    {
      return reinterpret_cast<const char* const*>( m_instanceCreateInfo.ppEnabledLayerNames );
    }

    const char* const* ppEnabledLayerNames()
    {
      return reinterpret_cast<const char* const*>( m_instanceCreateInfo.ppEnabledLayerNames );
    }

    InstanceCreateInfo& ppEnabledLayerNames( const char* const* ppEnabledLayerNames )
    {
      m_instanceCreateInfo.ppEnabledLayerNames = ppEnabledLayerNames;
      return *this;
    }

    const uint32_t& enabledExtensionCount() const
    {
      return m_instanceCreateInfo.enabledExtensionCount;
    }

    uint32_t& enabledExtensionCount()
    {
      return m_instanceCreateInfo.enabledExtensionCount;
    }

    InstanceCreateInfo& enabledExtensionCount( uint32_t enabledExtensionCount )
    {
      m_instanceCreateInfo.enabledExtensionCount = enabledExtensionCount;
      return *this;
    }

    const char* const* ppEnabledExtensionNames() const
    {
      return reinterpret_cast<const char* const*>( m_instanceCreateInfo.ppEnabledExtensionNames );
    }

    const char* const* ppEnabledExtensionNames()
    {
      return reinterpret_cast<const char* const*>( m_instanceCreateInfo.ppEnabledExtensionNames );
    }

    InstanceCreateInfo& ppEnabledExtensionNames( const char* const* ppEnabledExtensionNames )
    {
      m_instanceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames;
      return *this;
    }

    static InstanceCreateInfo& null()
    {
      return *((InstanceCreateInfo*)(nullptr));
    }

    operator const VkInstanceCreateInfo&() const
    {
      return m_instanceCreateInfo;
    }

  private:
    VkInstanceCreateInfo m_instanceCreateInfo;
  };
  static_assert( sizeof( InstanceCreateInfo ) == sizeof( VkInstanceCreateInfo ), "struct and wrapper have different size!" );

  class MemoryAllocateInfo
  {
  public:
    MemoryAllocateInfo()
      : MemoryAllocateInfo( 0, 0 )
    {}

    MemoryAllocateInfo( DeviceSize allocationSize, uint32_t memoryTypeIndex)
    {
      m_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      m_memoryAllocateInfo.pNext = nullptr;
      m_memoryAllocateInfo.allocationSize = allocationSize;
      m_memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;
    }

    MemoryAllocateInfo(VkMemoryAllocateInfo const & rhs)
      : m_memoryAllocateInfo(rhs)
    {
    }

    MemoryAllocateInfo& operator=(VkMemoryAllocateInfo const & rhs)
    {
      m_memoryAllocateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_memoryAllocateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_memoryAllocateInfo.sType );
    }

    MemoryAllocateInfo& sType( StructureType sType )
    {
      m_memoryAllocateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_memoryAllocateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_memoryAllocateInfo.pNext );
    }

    MemoryAllocateInfo& pNext( const void* pNext )
    {
      m_memoryAllocateInfo.pNext = pNext;
      return *this;
    }

    const DeviceSize& allocationSize() const
    {
      return m_memoryAllocateInfo.allocationSize;
    }

    DeviceSize& allocationSize()
    {
      return m_memoryAllocateInfo.allocationSize;
    }

    MemoryAllocateInfo& allocationSize( DeviceSize allocationSize )
    {
      m_memoryAllocateInfo.allocationSize = allocationSize;
      return *this;
    }

    const uint32_t& memoryTypeIndex() const
    {
      return m_memoryAllocateInfo.memoryTypeIndex;
    }

    uint32_t& memoryTypeIndex()
    {
      return m_memoryAllocateInfo.memoryTypeIndex;
    }

    MemoryAllocateInfo& memoryTypeIndex( uint32_t memoryTypeIndex )
    {
      m_memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;
      return *this;
    }

    static MemoryAllocateInfo& null()
    {
      return *((MemoryAllocateInfo*)(nullptr));
    }

    operator const VkMemoryAllocateInfo&() const
    {
      return m_memoryAllocateInfo;
    }

  private:
    VkMemoryAllocateInfo m_memoryAllocateInfo;
  };
  static_assert( sizeof( MemoryAllocateInfo ) == sizeof( VkMemoryAllocateInfo ), "struct and wrapper have different size!" );

  class MappedMemoryRange
  {
  public:
    MappedMemoryRange()
      : MappedMemoryRange( DeviceMemory(), 0, 0 )
    {}

    MappedMemoryRange( DeviceMemory memory, DeviceSize offset, DeviceSize size)
    {
      m_mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
      m_mappedMemoryRange.pNext = nullptr;
      m_mappedMemoryRange.memory = static_cast<VkDeviceMemory>( memory );
      m_mappedMemoryRange.offset = offset;
      m_mappedMemoryRange.size = size;
    }

    MappedMemoryRange(VkMappedMemoryRange const & rhs)
      : m_mappedMemoryRange(rhs)
    {
    }

    MappedMemoryRange& operator=(VkMappedMemoryRange const & rhs)
    {
      m_mappedMemoryRange = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_mappedMemoryRange.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_mappedMemoryRange.sType );
    }

    MappedMemoryRange& sType( StructureType sType )
    {
      m_mappedMemoryRange.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_mappedMemoryRange.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_mappedMemoryRange.pNext );
    }

    MappedMemoryRange& pNext( const void* pNext )
    {
      m_mappedMemoryRange.pNext = pNext;
      return *this;
    }

    const DeviceMemory& memory() const
    {
      return reinterpret_cast<const DeviceMemory&>( m_mappedMemoryRange.memory );
    }

    DeviceMemory& memory()
    {
      return reinterpret_cast<DeviceMemory&>( m_mappedMemoryRange.memory );
    }

    MappedMemoryRange& memory( DeviceMemory memory )
    {
      m_mappedMemoryRange.memory = static_cast<VkDeviceMemory>( memory );
      return *this;
    }

    const DeviceSize& offset() const
    {
      return m_mappedMemoryRange.offset;
    }

    DeviceSize& offset()
    {
      return m_mappedMemoryRange.offset;
    }

    MappedMemoryRange& offset( DeviceSize offset )
    {
      m_mappedMemoryRange.offset = offset;
      return *this;
    }

    const DeviceSize& size() const
    {
      return m_mappedMemoryRange.size;
    }

    DeviceSize& size()
    {
      return m_mappedMemoryRange.size;
    }

    MappedMemoryRange& size( DeviceSize size )
    {
      m_mappedMemoryRange.size = size;
      return *this;
    }

    static MappedMemoryRange& null()
    {
      return *((MappedMemoryRange*)(nullptr));
    }

    operator const VkMappedMemoryRange&() const
    {
      return m_mappedMemoryRange;
    }

  private:
    VkMappedMemoryRange m_mappedMemoryRange;
  };
  static_assert( sizeof( MappedMemoryRange ) == sizeof( VkMappedMemoryRange ), "struct and wrapper have different size!" );

  class WriteDescriptorSet
  {
  public:
    WriteDescriptorSet()
      : WriteDescriptorSet( DescriptorSet(), 0, 0, 0, DescriptorType::eSampler, nullptr, nullptr, nullptr )
    {}

    WriteDescriptorSet( DescriptorSet dstSet, uint32_t dstBinding, uint32_t dstArrayElement, uint32_t descriptorCount, DescriptorType descriptorType, const DescriptorImageInfo* pImageInfo, const DescriptorBufferInfo* pBufferInfo, const BufferView* pTexelBufferView)
    {
      m_writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      m_writeDescriptorSet.pNext = nullptr;
      m_writeDescriptorSet.dstSet = static_cast<VkDescriptorSet>( dstSet );
      m_writeDescriptorSet.dstBinding = dstBinding;
      m_writeDescriptorSet.dstArrayElement = dstArrayElement;
      m_writeDescriptorSet.descriptorCount = descriptorCount;
      m_writeDescriptorSet.descriptorType = static_cast<VkDescriptorType>( descriptorType );
      m_writeDescriptorSet.pImageInfo = reinterpret_cast<const VkDescriptorImageInfo*>( pImageInfo );
      m_writeDescriptorSet.pBufferInfo = reinterpret_cast<const VkDescriptorBufferInfo*>( pBufferInfo );
      m_writeDescriptorSet.pTexelBufferView = reinterpret_cast<const VkBufferView*>( pTexelBufferView );
    }

    WriteDescriptorSet(VkWriteDescriptorSet const & rhs)
      : m_writeDescriptorSet(rhs)
    {
    }

    WriteDescriptorSet& operator=(VkWriteDescriptorSet const & rhs)
    {
      m_writeDescriptorSet = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_writeDescriptorSet.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_writeDescriptorSet.sType );
    }

    WriteDescriptorSet& sType( StructureType sType )
    {
      m_writeDescriptorSet.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_writeDescriptorSet.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_writeDescriptorSet.pNext );
    }

    WriteDescriptorSet& pNext( const void* pNext )
    {
      m_writeDescriptorSet.pNext = pNext;
      return *this;
    }

    const DescriptorSet& dstSet() const
    {
      return reinterpret_cast<const DescriptorSet&>( m_writeDescriptorSet.dstSet );
    }

    DescriptorSet& dstSet()
    {
      return reinterpret_cast<DescriptorSet&>( m_writeDescriptorSet.dstSet );
    }

    WriteDescriptorSet& dstSet( DescriptorSet dstSet )
    {
      m_writeDescriptorSet.dstSet = static_cast<VkDescriptorSet>( dstSet );
      return *this;
    }

    const uint32_t& dstBinding() const
    {
      return m_writeDescriptorSet.dstBinding;
    }

    uint32_t& dstBinding()
    {
      return m_writeDescriptorSet.dstBinding;
    }

    WriteDescriptorSet& dstBinding( uint32_t dstBinding )
    {
      m_writeDescriptorSet.dstBinding = dstBinding;
      return *this;
    }

    const uint32_t& dstArrayElement() const
    {
      return m_writeDescriptorSet.dstArrayElement;
    }

    uint32_t& dstArrayElement()
    {
      return m_writeDescriptorSet.dstArrayElement;
    }

    WriteDescriptorSet& dstArrayElement( uint32_t dstArrayElement )
    {
      m_writeDescriptorSet.dstArrayElement = dstArrayElement;
      return *this;
    }

    const uint32_t& descriptorCount() const
    {
      return m_writeDescriptorSet.descriptorCount;
    }

    uint32_t& descriptorCount()
    {
      return m_writeDescriptorSet.descriptorCount;
    }

    WriteDescriptorSet& descriptorCount( uint32_t descriptorCount )
    {
      m_writeDescriptorSet.descriptorCount = descriptorCount;
      return *this;
    }

    const DescriptorType& descriptorType() const
    {
      return reinterpret_cast<const DescriptorType&>( m_writeDescriptorSet.descriptorType );
    }

    DescriptorType& descriptorType()
    {
      return reinterpret_cast<DescriptorType&>( m_writeDescriptorSet.descriptorType );
    }

    WriteDescriptorSet& descriptorType( DescriptorType descriptorType )
    {
      m_writeDescriptorSet.descriptorType = static_cast<VkDescriptorType>( descriptorType );
      return *this;
    }

    const DescriptorImageInfo* pImageInfo() const
    {
      return reinterpret_cast<const DescriptorImageInfo*>( m_writeDescriptorSet.pImageInfo );
    }

    const DescriptorImageInfo* pImageInfo()
    {
      return reinterpret_cast<const DescriptorImageInfo*>( m_writeDescriptorSet.pImageInfo );
    }

    WriteDescriptorSet& pImageInfo( const DescriptorImageInfo* pImageInfo )
    {
      m_writeDescriptorSet.pImageInfo = reinterpret_cast<const VkDescriptorImageInfo*>( pImageInfo );
      return *this;
    }

    const DescriptorBufferInfo* pBufferInfo() const
    {
      return reinterpret_cast<const DescriptorBufferInfo*>( m_writeDescriptorSet.pBufferInfo );
    }

    const DescriptorBufferInfo* pBufferInfo()
    {
      return reinterpret_cast<const DescriptorBufferInfo*>( m_writeDescriptorSet.pBufferInfo );
    }

    WriteDescriptorSet& pBufferInfo( const DescriptorBufferInfo* pBufferInfo )
    {
      m_writeDescriptorSet.pBufferInfo = reinterpret_cast<const VkDescriptorBufferInfo*>( pBufferInfo );
      return *this;
    }

    const BufferView* pTexelBufferView() const
    {
      return reinterpret_cast<const BufferView*>( m_writeDescriptorSet.pTexelBufferView );
    }

    const BufferView* pTexelBufferView()
    {
      return reinterpret_cast<const BufferView*>( m_writeDescriptorSet.pTexelBufferView );
    }

    WriteDescriptorSet& pTexelBufferView( const BufferView* pTexelBufferView )
    {
      m_writeDescriptorSet.pTexelBufferView = reinterpret_cast<const VkBufferView*>( pTexelBufferView );
      return *this;
    }

    static WriteDescriptorSet& null()
    {
      return *((WriteDescriptorSet*)(nullptr));
    }

    operator const VkWriteDescriptorSet&() const
    {
      return m_writeDescriptorSet;
    }

  private:
    VkWriteDescriptorSet m_writeDescriptorSet;
  };
  static_assert( sizeof( WriteDescriptorSet ) == sizeof( VkWriteDescriptorSet ), "struct and wrapper have different size!" );

  class CopyDescriptorSet
  {
  public:
    CopyDescriptorSet()
      : CopyDescriptorSet( DescriptorSet(), 0, 0, DescriptorSet(), 0, 0, 0 )
    {}

    CopyDescriptorSet( DescriptorSet srcSet, uint32_t srcBinding, uint32_t srcArrayElement, DescriptorSet dstSet, uint32_t dstBinding, uint32_t dstArrayElement, uint32_t descriptorCount)
    {
      m_copyDescriptorSet.sType = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET;
      m_copyDescriptorSet.pNext = nullptr;
      m_copyDescriptorSet.srcSet = static_cast<VkDescriptorSet>( srcSet );
      m_copyDescriptorSet.srcBinding = srcBinding;
      m_copyDescriptorSet.srcArrayElement = srcArrayElement;
      m_copyDescriptorSet.dstSet = static_cast<VkDescriptorSet>( dstSet );
      m_copyDescriptorSet.dstBinding = dstBinding;
      m_copyDescriptorSet.dstArrayElement = dstArrayElement;
      m_copyDescriptorSet.descriptorCount = descriptorCount;
    }

    CopyDescriptorSet(VkCopyDescriptorSet const & rhs)
      : m_copyDescriptorSet(rhs)
    {
    }

    CopyDescriptorSet& operator=(VkCopyDescriptorSet const & rhs)
    {
      m_copyDescriptorSet = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_copyDescriptorSet.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_copyDescriptorSet.sType );
    }

    CopyDescriptorSet& sType( StructureType sType )
    {
      m_copyDescriptorSet.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_copyDescriptorSet.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_copyDescriptorSet.pNext );
    }

    CopyDescriptorSet& pNext( const void* pNext )
    {
      m_copyDescriptorSet.pNext = pNext;
      return *this;
    }

    const DescriptorSet& srcSet() const
    {
      return reinterpret_cast<const DescriptorSet&>( m_copyDescriptorSet.srcSet );
    }

    DescriptorSet& srcSet()
    {
      return reinterpret_cast<DescriptorSet&>( m_copyDescriptorSet.srcSet );
    }

    CopyDescriptorSet& srcSet( DescriptorSet srcSet )
    {
      m_copyDescriptorSet.srcSet = static_cast<VkDescriptorSet>( srcSet );
      return *this;
    }

    const uint32_t& srcBinding() const
    {
      return m_copyDescriptorSet.srcBinding;
    }

    uint32_t& srcBinding()
    {
      return m_copyDescriptorSet.srcBinding;
    }

    CopyDescriptorSet& srcBinding( uint32_t srcBinding )
    {
      m_copyDescriptorSet.srcBinding = srcBinding;
      return *this;
    }

    const uint32_t& srcArrayElement() const
    {
      return m_copyDescriptorSet.srcArrayElement;
    }

    uint32_t& srcArrayElement()
    {
      return m_copyDescriptorSet.srcArrayElement;
    }

    CopyDescriptorSet& srcArrayElement( uint32_t srcArrayElement )
    {
      m_copyDescriptorSet.srcArrayElement = srcArrayElement;
      return *this;
    }

    const DescriptorSet& dstSet() const
    {
      return reinterpret_cast<const DescriptorSet&>( m_copyDescriptorSet.dstSet );
    }

    DescriptorSet& dstSet()
    {
      return reinterpret_cast<DescriptorSet&>( m_copyDescriptorSet.dstSet );
    }

    CopyDescriptorSet& dstSet( DescriptorSet dstSet )
    {
      m_copyDescriptorSet.dstSet = static_cast<VkDescriptorSet>( dstSet );
      return *this;
    }

    const uint32_t& dstBinding() const
    {
      return m_copyDescriptorSet.dstBinding;
    }

    uint32_t& dstBinding()
    {
      return m_copyDescriptorSet.dstBinding;
    }

    CopyDescriptorSet& dstBinding( uint32_t dstBinding )
    {
      m_copyDescriptorSet.dstBinding = dstBinding;
      return *this;
    }

    const uint32_t& dstArrayElement() const
    {
      return m_copyDescriptorSet.dstArrayElement;
    }

    uint32_t& dstArrayElement()
    {
      return m_copyDescriptorSet.dstArrayElement;
    }

    CopyDescriptorSet& dstArrayElement( uint32_t dstArrayElement )
    {
      m_copyDescriptorSet.dstArrayElement = dstArrayElement;
      return *this;
    }

    const uint32_t& descriptorCount() const
    {
      return m_copyDescriptorSet.descriptorCount;
    }

    uint32_t& descriptorCount()
    {
      return m_copyDescriptorSet.descriptorCount;
    }

    CopyDescriptorSet& descriptorCount( uint32_t descriptorCount )
    {
      m_copyDescriptorSet.descriptorCount = descriptorCount;
      return *this;
    }

    static CopyDescriptorSet& null()
    {
      return *((CopyDescriptorSet*)(nullptr));
    }

    operator const VkCopyDescriptorSet&() const
    {
      return m_copyDescriptorSet;
    }

  private:
    VkCopyDescriptorSet m_copyDescriptorSet;
  };
  static_assert( sizeof( CopyDescriptorSet ) == sizeof( VkCopyDescriptorSet ), "struct and wrapper have different size!" );

  class BufferViewCreateInfo
  {
  public:
    BufferViewCreateInfo()
      : BufferViewCreateInfo( BufferViewCreateFlags(), Buffer(), Format::eUndefined, 0, 0 )
    {}

    BufferViewCreateInfo( BufferViewCreateFlags flags, Buffer buffer, Format format, DeviceSize offset, DeviceSize range)
    {
      m_bufferViewCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
      m_bufferViewCreateInfo.pNext = nullptr;
      m_bufferViewCreateInfo.flags = static_cast<VkBufferViewCreateFlags>( flags );
      m_bufferViewCreateInfo.buffer = static_cast<VkBuffer>( buffer );
      m_bufferViewCreateInfo.format = static_cast<VkFormat>( format );
      m_bufferViewCreateInfo.offset = offset;
      m_bufferViewCreateInfo.range = range;
    }

    BufferViewCreateInfo(VkBufferViewCreateInfo const & rhs)
      : m_bufferViewCreateInfo(rhs)
    {
    }

    BufferViewCreateInfo& operator=(VkBufferViewCreateInfo const & rhs)
    {
      m_bufferViewCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_bufferViewCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_bufferViewCreateInfo.sType );
    }

    BufferViewCreateInfo& sType( StructureType sType )
    {
      m_bufferViewCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_bufferViewCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_bufferViewCreateInfo.pNext );
    }

    BufferViewCreateInfo& pNext( const void* pNext )
    {
      m_bufferViewCreateInfo.pNext = pNext;
      return *this;
    }

    const BufferViewCreateFlags& flags() const
    {
      return reinterpret_cast<const BufferViewCreateFlags&>( m_bufferViewCreateInfo.flags );
    }

    BufferViewCreateFlags& flags()
    {
      return reinterpret_cast<BufferViewCreateFlags&>( m_bufferViewCreateInfo.flags );
    }

    BufferViewCreateInfo& flags( BufferViewCreateFlags flags )
    {
      m_bufferViewCreateInfo.flags = static_cast<VkBufferViewCreateFlags>( flags );
      return *this;
    }

    const Buffer& buffer() const
    {
      return reinterpret_cast<const Buffer&>( m_bufferViewCreateInfo.buffer );
    }

    Buffer& buffer()
    {
      return reinterpret_cast<Buffer&>( m_bufferViewCreateInfo.buffer );
    }

    BufferViewCreateInfo& buffer( Buffer buffer )
    {
      m_bufferViewCreateInfo.buffer = static_cast<VkBuffer>( buffer );
      return *this;
    }

    const Format& format() const
    {
      return reinterpret_cast<const Format&>( m_bufferViewCreateInfo.format );
    }

    Format& format()
    {
      return reinterpret_cast<Format&>( m_bufferViewCreateInfo.format );
    }

    BufferViewCreateInfo& format( Format format )
    {
      m_bufferViewCreateInfo.format = static_cast<VkFormat>( format );
      return *this;
    }

    const DeviceSize& offset() const
    {
      return m_bufferViewCreateInfo.offset;
    }

    DeviceSize& offset()
    {
      return m_bufferViewCreateInfo.offset;
    }

    BufferViewCreateInfo& offset( DeviceSize offset )
    {
      m_bufferViewCreateInfo.offset = offset;
      return *this;
    }

    const DeviceSize& range() const
    {
      return m_bufferViewCreateInfo.range;
    }

    DeviceSize& range()
    {
      return m_bufferViewCreateInfo.range;
    }

    BufferViewCreateInfo& range( DeviceSize range )
    {
      m_bufferViewCreateInfo.range = range;
      return *this;
    }

    static BufferViewCreateInfo& null()
    {
      return *((BufferViewCreateInfo*)(nullptr));
    }

    operator const VkBufferViewCreateInfo&() const
    {
      return m_bufferViewCreateInfo;
    }

  private:
    VkBufferViewCreateInfo m_bufferViewCreateInfo;
  };
  static_assert( sizeof( BufferViewCreateInfo ) == sizeof( VkBufferViewCreateInfo ), "struct and wrapper have different size!" );

  class ShaderModuleCreateInfo
  {
  public:
    ShaderModuleCreateInfo()
      : ShaderModuleCreateInfo( ShaderModuleCreateFlags(), 0, nullptr )
    {}

    ShaderModuleCreateInfo( ShaderModuleCreateFlags flags, size_t codeSize, const uint32_t* pCode)
    {
      m_shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
      m_shaderModuleCreateInfo.pNext = nullptr;
      m_shaderModuleCreateInfo.flags = static_cast<VkShaderModuleCreateFlags>( flags );
      m_shaderModuleCreateInfo.codeSize = codeSize;
      m_shaderModuleCreateInfo.pCode = pCode;
    }

    ShaderModuleCreateInfo(VkShaderModuleCreateInfo const & rhs)
      : m_shaderModuleCreateInfo(rhs)
    {
    }

    ShaderModuleCreateInfo& operator=(VkShaderModuleCreateInfo const & rhs)
    {
      m_shaderModuleCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_shaderModuleCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_shaderModuleCreateInfo.sType );
    }

    ShaderModuleCreateInfo& sType( StructureType sType )
    {
      m_shaderModuleCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_shaderModuleCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_shaderModuleCreateInfo.pNext );
    }

    ShaderModuleCreateInfo& pNext( const void* pNext )
    {
      m_shaderModuleCreateInfo.pNext = pNext;
      return *this;
    }

    const ShaderModuleCreateFlags& flags() const
    {
      return reinterpret_cast<const ShaderModuleCreateFlags&>( m_shaderModuleCreateInfo.flags );
    }

    ShaderModuleCreateFlags& flags()
    {
      return reinterpret_cast<ShaderModuleCreateFlags&>( m_shaderModuleCreateInfo.flags );
    }

    ShaderModuleCreateInfo& flags( ShaderModuleCreateFlags flags )
    {
      m_shaderModuleCreateInfo.flags = static_cast<VkShaderModuleCreateFlags>( flags );
      return *this;
    }

    const size_t& codeSize() const
    {
      return m_shaderModuleCreateInfo.codeSize;
    }

    size_t& codeSize()
    {
      return m_shaderModuleCreateInfo.codeSize;
    }

    ShaderModuleCreateInfo& codeSize( size_t codeSize )
    {
      m_shaderModuleCreateInfo.codeSize = codeSize;
      return *this;
    }

    const uint32_t* pCode() const
    {
      return reinterpret_cast<const uint32_t*>( m_shaderModuleCreateInfo.pCode );
    }

    const uint32_t* pCode()
    {
      return reinterpret_cast<const uint32_t*>( m_shaderModuleCreateInfo.pCode );
    }

    ShaderModuleCreateInfo& pCode( const uint32_t* pCode )
    {
      m_shaderModuleCreateInfo.pCode = pCode;
      return *this;
    }

    static ShaderModuleCreateInfo& null()
    {
      return *((ShaderModuleCreateInfo*)(nullptr));
    }

    operator const VkShaderModuleCreateInfo&() const
    {
      return m_shaderModuleCreateInfo;
    }

  private:
    VkShaderModuleCreateInfo m_shaderModuleCreateInfo;
  };
  static_assert( sizeof( ShaderModuleCreateInfo ) == sizeof( VkShaderModuleCreateInfo ), "struct and wrapper have different size!" );

  class DescriptorSetAllocateInfo
  {
  public:
    DescriptorSetAllocateInfo()
      : DescriptorSetAllocateInfo( DescriptorPool(), 0, nullptr )
    {}

    DescriptorSetAllocateInfo( DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSetLayout* pSetLayouts)
    {
      m_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      m_descriptorSetAllocateInfo.pNext = nullptr;
      m_descriptorSetAllocateInfo.descriptorPool = static_cast<VkDescriptorPool>( descriptorPool );
      m_descriptorSetAllocateInfo.descriptorSetCount = descriptorSetCount;
      m_descriptorSetAllocateInfo.pSetLayouts = reinterpret_cast<const VkDescriptorSetLayout*>( pSetLayouts );
    }

    DescriptorSetAllocateInfo(VkDescriptorSetAllocateInfo const & rhs)
      : m_descriptorSetAllocateInfo(rhs)
    {
    }

    DescriptorSetAllocateInfo& operator=(VkDescriptorSetAllocateInfo const & rhs)
    {
      m_descriptorSetAllocateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_descriptorSetAllocateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_descriptorSetAllocateInfo.sType );
    }

    DescriptorSetAllocateInfo& sType( StructureType sType )
    {
      m_descriptorSetAllocateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_descriptorSetAllocateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_descriptorSetAllocateInfo.pNext );
    }

    DescriptorSetAllocateInfo& pNext( const void* pNext )
    {
      m_descriptorSetAllocateInfo.pNext = pNext;
      return *this;
    }

    const DescriptorPool& descriptorPool() const
    {
      return reinterpret_cast<const DescriptorPool&>( m_descriptorSetAllocateInfo.descriptorPool );
    }

    DescriptorPool& descriptorPool()
    {
      return reinterpret_cast<DescriptorPool&>( m_descriptorSetAllocateInfo.descriptorPool );
    }

    DescriptorSetAllocateInfo& descriptorPool( DescriptorPool descriptorPool )
    {
      m_descriptorSetAllocateInfo.descriptorPool = static_cast<VkDescriptorPool>( descriptorPool );
      return *this;
    }

    const uint32_t& descriptorSetCount() const
    {
      return m_descriptorSetAllocateInfo.descriptorSetCount;
    }

    uint32_t& descriptorSetCount()
    {
      return m_descriptorSetAllocateInfo.descriptorSetCount;
    }

    DescriptorSetAllocateInfo& descriptorSetCount( uint32_t descriptorSetCount )
    {
      m_descriptorSetAllocateInfo.descriptorSetCount = descriptorSetCount;
      return *this;
    }

    const DescriptorSetLayout* pSetLayouts() const
    {
      return reinterpret_cast<const DescriptorSetLayout*>( m_descriptorSetAllocateInfo.pSetLayouts );
    }

    const DescriptorSetLayout* pSetLayouts()
    {
      return reinterpret_cast<const DescriptorSetLayout*>( m_descriptorSetAllocateInfo.pSetLayouts );
    }

    DescriptorSetAllocateInfo& pSetLayouts( const DescriptorSetLayout* pSetLayouts )
    {
      m_descriptorSetAllocateInfo.pSetLayouts = reinterpret_cast<const VkDescriptorSetLayout*>( pSetLayouts );
      return *this;
    }

    static DescriptorSetAllocateInfo& null()
    {
      return *((DescriptorSetAllocateInfo*)(nullptr));
    }

    operator const VkDescriptorSetAllocateInfo&() const
    {
      return m_descriptorSetAllocateInfo;
    }

  private:
    VkDescriptorSetAllocateInfo m_descriptorSetAllocateInfo;
  };
  static_assert( sizeof( DescriptorSetAllocateInfo ) == sizeof( VkDescriptorSetAllocateInfo ), "struct and wrapper have different size!" );

  class PipelineVertexInputStateCreateInfo
  {
  public:
    PipelineVertexInputStateCreateInfo()
      : PipelineVertexInputStateCreateInfo( PipelineVertexInputStateCreateFlags(), 0, nullptr, 0, nullptr )
    {}

    PipelineVertexInputStateCreateInfo( PipelineVertexInputStateCreateFlags flags, uint32_t vertexBindingDescriptionCount, const VertexInputBindingDescription* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VertexInputAttributeDescription* pVertexAttributeDescriptions)
    {
      m_pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
      m_pipelineVertexInputStateCreateInfo.pNext = nullptr;
      m_pipelineVertexInputStateCreateInfo.flags = static_cast<VkPipelineVertexInputStateCreateFlags>( flags );
      m_pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexBindingDescriptionCount;
      m_pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = reinterpret_cast<const VkVertexInputBindingDescription*>( pVertexBindingDescriptions );
      m_pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexAttributeDescriptionCount;
      m_pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = reinterpret_cast<const VkVertexInputAttributeDescription*>( pVertexAttributeDescriptions );
    }

    PipelineVertexInputStateCreateInfo(VkPipelineVertexInputStateCreateInfo const & rhs)
      : m_pipelineVertexInputStateCreateInfo(rhs)
    {
    }

    PipelineVertexInputStateCreateInfo& operator=(VkPipelineVertexInputStateCreateInfo const & rhs)
    {
      m_pipelineVertexInputStateCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineVertexInputStateCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_pipelineVertexInputStateCreateInfo.sType );
    }

    PipelineVertexInputStateCreateInfo& sType( StructureType sType )
    {
      m_pipelineVertexInputStateCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_pipelineVertexInputStateCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_pipelineVertexInputStateCreateInfo.pNext );
    }

    PipelineVertexInputStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineVertexInputStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineVertexInputStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineVertexInputStateCreateFlags&>( m_pipelineVertexInputStateCreateInfo.flags );
    }

    PipelineVertexInputStateCreateFlags& flags()
    {
      return reinterpret_cast<PipelineVertexInputStateCreateFlags&>( m_pipelineVertexInputStateCreateInfo.flags );
    }

    PipelineVertexInputStateCreateInfo& flags( PipelineVertexInputStateCreateFlags flags )
    {
      m_pipelineVertexInputStateCreateInfo.flags = static_cast<VkPipelineVertexInputStateCreateFlags>( flags );
      return *this;
    }

    const uint32_t& vertexBindingDescriptionCount() const
    {
      return m_pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount;
    }

    uint32_t& vertexBindingDescriptionCount()
    {
      return m_pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount;
    }

    PipelineVertexInputStateCreateInfo& vertexBindingDescriptionCount( uint32_t vertexBindingDescriptionCount )
    {
      m_pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexBindingDescriptionCount;
      return *this;
    }

    const VertexInputBindingDescription* pVertexBindingDescriptions() const
    {
      return reinterpret_cast<const VertexInputBindingDescription*>( m_pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions );
    }

    const VertexInputBindingDescription* pVertexBindingDescriptions()
    {
      return reinterpret_cast<const VertexInputBindingDescription*>( m_pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions );
    }

    PipelineVertexInputStateCreateInfo& pVertexBindingDescriptions( const VertexInputBindingDescription* pVertexBindingDescriptions )
    {
      m_pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = reinterpret_cast<const VkVertexInputBindingDescription*>( pVertexBindingDescriptions );
      return *this;
    }

    const uint32_t& vertexAttributeDescriptionCount() const
    {
      return m_pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount;
    }

    uint32_t& vertexAttributeDescriptionCount()
    {
      return m_pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount;
    }

    PipelineVertexInputStateCreateInfo& vertexAttributeDescriptionCount( uint32_t vertexAttributeDescriptionCount )
    {
      m_pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexAttributeDescriptionCount;
      return *this;
    }

    const VertexInputAttributeDescription* pVertexAttributeDescriptions() const
    {
      return reinterpret_cast<const VertexInputAttributeDescription*>( m_pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions );
    }

    const VertexInputAttributeDescription* pVertexAttributeDescriptions()
    {
      return reinterpret_cast<const VertexInputAttributeDescription*>( m_pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions );
    }

    PipelineVertexInputStateCreateInfo& pVertexAttributeDescriptions( const VertexInputAttributeDescription* pVertexAttributeDescriptions )
    {
      m_pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = reinterpret_cast<const VkVertexInputAttributeDescription*>( pVertexAttributeDescriptions );
      return *this;
    }

    static PipelineVertexInputStateCreateInfo& null()
    {
      return *((PipelineVertexInputStateCreateInfo*)(nullptr));
    }

    operator const VkPipelineVertexInputStateCreateInfo&() const
    {
      return m_pipelineVertexInputStateCreateInfo;
    }

  private:
    VkPipelineVertexInputStateCreateInfo m_pipelineVertexInputStateCreateInfo;
  };
  static_assert( sizeof( PipelineVertexInputStateCreateInfo ) == sizeof( VkPipelineVertexInputStateCreateInfo ), "struct and wrapper have different size!" );

  class PipelineInputAssemblyStateCreateInfo
  {
  public:
    PipelineInputAssemblyStateCreateInfo()
      : PipelineInputAssemblyStateCreateInfo( PipelineInputAssemblyStateCreateFlags(), PrimitiveTopology::ePointList, 0 )
    {}

    PipelineInputAssemblyStateCreateInfo( PipelineInputAssemblyStateCreateFlags flags, PrimitiveTopology topology, Bool32 primitiveRestartEnable)
    {
      m_pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
      m_pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
      m_pipelineInputAssemblyStateCreateInfo.flags = static_cast<VkPipelineInputAssemblyStateCreateFlags>( flags );
      m_pipelineInputAssemblyStateCreateInfo.topology = static_cast<VkPrimitiveTopology>( topology );
      m_pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
    }

    PipelineInputAssemblyStateCreateInfo(VkPipelineInputAssemblyStateCreateInfo const & rhs)
      : m_pipelineInputAssemblyStateCreateInfo(rhs)
    {
    }

    PipelineInputAssemblyStateCreateInfo& operator=(VkPipelineInputAssemblyStateCreateInfo const & rhs)
    {
      m_pipelineInputAssemblyStateCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineInputAssemblyStateCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_pipelineInputAssemblyStateCreateInfo.sType );
    }

    PipelineInputAssemblyStateCreateInfo& sType( StructureType sType )
    {
      m_pipelineInputAssemblyStateCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_pipelineInputAssemblyStateCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_pipelineInputAssemblyStateCreateInfo.pNext );
    }

    PipelineInputAssemblyStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineInputAssemblyStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineInputAssemblyStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineInputAssemblyStateCreateFlags&>( m_pipelineInputAssemblyStateCreateInfo.flags );
    }

    PipelineInputAssemblyStateCreateFlags& flags()
    {
      return reinterpret_cast<PipelineInputAssemblyStateCreateFlags&>( m_pipelineInputAssemblyStateCreateInfo.flags );
    }

    PipelineInputAssemblyStateCreateInfo& flags( PipelineInputAssemblyStateCreateFlags flags )
    {
      m_pipelineInputAssemblyStateCreateInfo.flags = static_cast<VkPipelineInputAssemblyStateCreateFlags>( flags );
      return *this;
    }

    const PrimitiveTopology& topology() const
    {
      return reinterpret_cast<const PrimitiveTopology&>( m_pipelineInputAssemblyStateCreateInfo.topology );
    }

    PrimitiveTopology& topology()
    {
      return reinterpret_cast<PrimitiveTopology&>( m_pipelineInputAssemblyStateCreateInfo.topology );
    }

    PipelineInputAssemblyStateCreateInfo& topology( PrimitiveTopology topology )
    {
      m_pipelineInputAssemblyStateCreateInfo.topology = static_cast<VkPrimitiveTopology>( topology );
      return *this;
    }

    const Bool32& primitiveRestartEnable() const
    {
      return m_pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable;
    }

    Bool32& primitiveRestartEnable()
    {
      return m_pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable;
    }

    PipelineInputAssemblyStateCreateInfo& primitiveRestartEnable( Bool32 primitiveRestartEnable )
    {
      m_pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
      return *this;
    }

    static PipelineInputAssemblyStateCreateInfo& null()
    {
      return *((PipelineInputAssemblyStateCreateInfo*)(nullptr));
    }

    operator const VkPipelineInputAssemblyStateCreateInfo&() const
    {
      return m_pipelineInputAssemblyStateCreateInfo;
    }

  private:
    VkPipelineInputAssemblyStateCreateInfo m_pipelineInputAssemblyStateCreateInfo;
  };
  static_assert( sizeof( PipelineInputAssemblyStateCreateInfo ) == sizeof( VkPipelineInputAssemblyStateCreateInfo ), "struct and wrapper have different size!" );

  class PipelineTessellationStateCreateInfo
  {
  public:
    PipelineTessellationStateCreateInfo()
      : PipelineTessellationStateCreateInfo( PipelineTessellationStateCreateFlags(), 0 )
    {}

    PipelineTessellationStateCreateInfo( PipelineTessellationStateCreateFlags flags, uint32_t patchControlPoints)
    {
      m_pipelineTessellationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
      m_pipelineTessellationStateCreateInfo.pNext = nullptr;
      m_pipelineTessellationStateCreateInfo.flags = static_cast<VkPipelineTessellationStateCreateFlags>( flags );
      m_pipelineTessellationStateCreateInfo.patchControlPoints = patchControlPoints;
    }

    PipelineTessellationStateCreateInfo(VkPipelineTessellationStateCreateInfo const & rhs)
      : m_pipelineTessellationStateCreateInfo(rhs)
    {
    }

    PipelineTessellationStateCreateInfo& operator=(VkPipelineTessellationStateCreateInfo const & rhs)
    {
      m_pipelineTessellationStateCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineTessellationStateCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_pipelineTessellationStateCreateInfo.sType );
    }

    PipelineTessellationStateCreateInfo& sType( StructureType sType )
    {
      m_pipelineTessellationStateCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_pipelineTessellationStateCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_pipelineTessellationStateCreateInfo.pNext );
    }

    PipelineTessellationStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineTessellationStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineTessellationStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineTessellationStateCreateFlags&>( m_pipelineTessellationStateCreateInfo.flags );
    }

    PipelineTessellationStateCreateFlags& flags()
    {
      return reinterpret_cast<PipelineTessellationStateCreateFlags&>( m_pipelineTessellationStateCreateInfo.flags );
    }

    PipelineTessellationStateCreateInfo& flags( PipelineTessellationStateCreateFlags flags )
    {
      m_pipelineTessellationStateCreateInfo.flags = static_cast<VkPipelineTessellationStateCreateFlags>( flags );
      return *this;
    }

    const uint32_t& patchControlPoints() const
    {
      return m_pipelineTessellationStateCreateInfo.patchControlPoints;
    }

    uint32_t& patchControlPoints()
    {
      return m_pipelineTessellationStateCreateInfo.patchControlPoints;
    }

    PipelineTessellationStateCreateInfo& patchControlPoints( uint32_t patchControlPoints )
    {
      m_pipelineTessellationStateCreateInfo.patchControlPoints = patchControlPoints;
      return *this;
    }

    static PipelineTessellationStateCreateInfo& null()
    {
      return *((PipelineTessellationStateCreateInfo*)(nullptr));
    }

    operator const VkPipelineTessellationStateCreateInfo&() const
    {
      return m_pipelineTessellationStateCreateInfo;
    }

  private:
    VkPipelineTessellationStateCreateInfo m_pipelineTessellationStateCreateInfo;
  };
  static_assert( sizeof( PipelineTessellationStateCreateInfo ) == sizeof( VkPipelineTessellationStateCreateInfo ), "struct and wrapper have different size!" );

  class PipelineViewportStateCreateInfo
  {
  public:
    PipelineViewportStateCreateInfo()
      : PipelineViewportStateCreateInfo( PipelineViewportStateCreateFlags(), 0, nullptr, 0, nullptr )
    {}

    PipelineViewportStateCreateInfo( PipelineViewportStateCreateFlags flags, uint32_t viewportCount, const Viewport* pViewports, uint32_t scissorCount, const Rect2D* pScissors)
    {
      m_pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
      m_pipelineViewportStateCreateInfo.pNext = nullptr;
      m_pipelineViewportStateCreateInfo.flags = static_cast<VkPipelineViewportStateCreateFlags>( flags );
      m_pipelineViewportStateCreateInfo.viewportCount = viewportCount;
      m_pipelineViewportStateCreateInfo.pViewports = reinterpret_cast<const VkViewport*>( pViewports );
      m_pipelineViewportStateCreateInfo.scissorCount = scissorCount;
      m_pipelineViewportStateCreateInfo.pScissors = reinterpret_cast<const VkRect2D*>( pScissors );
    }

    PipelineViewportStateCreateInfo(VkPipelineViewportStateCreateInfo const & rhs)
      : m_pipelineViewportStateCreateInfo(rhs)
    {
    }

    PipelineViewportStateCreateInfo& operator=(VkPipelineViewportStateCreateInfo const & rhs)
    {
      m_pipelineViewportStateCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineViewportStateCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_pipelineViewportStateCreateInfo.sType );
    }

    PipelineViewportStateCreateInfo& sType( StructureType sType )
    {
      m_pipelineViewportStateCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_pipelineViewportStateCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_pipelineViewportStateCreateInfo.pNext );
    }

    PipelineViewportStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineViewportStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineViewportStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineViewportStateCreateFlags&>( m_pipelineViewportStateCreateInfo.flags );
    }

    PipelineViewportStateCreateFlags& flags()
    {
      return reinterpret_cast<PipelineViewportStateCreateFlags&>( m_pipelineViewportStateCreateInfo.flags );
    }

    PipelineViewportStateCreateInfo& flags( PipelineViewportStateCreateFlags flags )
    {
      m_pipelineViewportStateCreateInfo.flags = static_cast<VkPipelineViewportStateCreateFlags>( flags );
      return *this;
    }

    const uint32_t& viewportCount() const
    {
      return m_pipelineViewportStateCreateInfo.viewportCount;
    }

    uint32_t& viewportCount()
    {
      return m_pipelineViewportStateCreateInfo.viewportCount;
    }

    PipelineViewportStateCreateInfo& viewportCount( uint32_t viewportCount )
    {
      m_pipelineViewportStateCreateInfo.viewportCount = viewportCount;
      return *this;
    }

    const Viewport* pViewports() const
    {
      return reinterpret_cast<const Viewport*>( m_pipelineViewportStateCreateInfo.pViewports );
    }

    const Viewport* pViewports()
    {
      return reinterpret_cast<const Viewport*>( m_pipelineViewportStateCreateInfo.pViewports );
    }

    PipelineViewportStateCreateInfo& pViewports( const Viewport* pViewports )
    {
      m_pipelineViewportStateCreateInfo.pViewports = reinterpret_cast<const VkViewport*>( pViewports );
      return *this;
    }

    const uint32_t& scissorCount() const
    {
      return m_pipelineViewportStateCreateInfo.scissorCount;
    }

    uint32_t& scissorCount()
    {
      return m_pipelineViewportStateCreateInfo.scissorCount;
    }

    PipelineViewportStateCreateInfo& scissorCount( uint32_t scissorCount )
    {
      m_pipelineViewportStateCreateInfo.scissorCount = scissorCount;
      return *this;
    }

    const Rect2D* pScissors() const
    {
      return reinterpret_cast<const Rect2D*>( m_pipelineViewportStateCreateInfo.pScissors );
    }

    const Rect2D* pScissors()
    {
      return reinterpret_cast<const Rect2D*>( m_pipelineViewportStateCreateInfo.pScissors );
    }

    PipelineViewportStateCreateInfo& pScissors( const Rect2D* pScissors )
    {
      m_pipelineViewportStateCreateInfo.pScissors = reinterpret_cast<const VkRect2D*>( pScissors );
      return *this;
    }

    static PipelineViewportStateCreateInfo& null()
    {
      return *((PipelineViewportStateCreateInfo*)(nullptr));
    }

    operator const VkPipelineViewportStateCreateInfo&() const
    {
      return m_pipelineViewportStateCreateInfo;
    }

  private:
    VkPipelineViewportStateCreateInfo m_pipelineViewportStateCreateInfo;
  };
  static_assert( sizeof( PipelineViewportStateCreateInfo ) == sizeof( VkPipelineViewportStateCreateInfo ), "struct and wrapper have different size!" );

  class PipelineRasterizationStateCreateInfo
  {
  public:
    PipelineRasterizationStateCreateInfo()
      : PipelineRasterizationStateCreateInfo( PipelineRasterizationStateCreateFlags(), 0, 0, PolygonMode::eFill, CullModeFlags(), FrontFace::eCounterClockwise, 0, 0, 0, 0, 0 )
    {}

    PipelineRasterizationStateCreateInfo( PipelineRasterizationStateCreateFlags flags, Bool32 depthClampEnable, Bool32 rasterizerDiscardEnable, PolygonMode polygonMode, CullModeFlags cullMode, FrontFace frontFace, Bool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth)
    {
      m_pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
      m_pipelineRasterizationStateCreateInfo.pNext = nullptr;
      m_pipelineRasterizationStateCreateInfo.flags = static_cast<VkPipelineRasterizationStateCreateFlags>( flags );
      m_pipelineRasterizationStateCreateInfo.depthClampEnable = depthClampEnable;
      m_pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = rasterizerDiscardEnable;
      m_pipelineRasterizationStateCreateInfo.polygonMode = static_cast<VkPolygonMode>( polygonMode );
      m_pipelineRasterizationStateCreateInfo.cullMode = static_cast<VkCullModeFlags>( cullMode );
      m_pipelineRasterizationStateCreateInfo.frontFace = static_cast<VkFrontFace>( frontFace );
      m_pipelineRasterizationStateCreateInfo.depthBiasEnable = depthBiasEnable;
      m_pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = depthBiasConstantFactor;
      m_pipelineRasterizationStateCreateInfo.depthBiasClamp = depthBiasClamp;
      m_pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = depthBiasSlopeFactor;
      m_pipelineRasterizationStateCreateInfo.lineWidth = lineWidth;
    }

    PipelineRasterizationStateCreateInfo(VkPipelineRasterizationStateCreateInfo const & rhs)
      : m_pipelineRasterizationStateCreateInfo(rhs)
    {
    }

    PipelineRasterizationStateCreateInfo& operator=(VkPipelineRasterizationStateCreateInfo const & rhs)
    {
      m_pipelineRasterizationStateCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineRasterizationStateCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_pipelineRasterizationStateCreateInfo.sType );
    }

    PipelineRasterizationStateCreateInfo& sType( StructureType sType )
    {
      m_pipelineRasterizationStateCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_pipelineRasterizationStateCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_pipelineRasterizationStateCreateInfo.pNext );
    }

    PipelineRasterizationStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineRasterizationStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineRasterizationStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineRasterizationStateCreateFlags&>( m_pipelineRasterizationStateCreateInfo.flags );
    }

    PipelineRasterizationStateCreateFlags& flags()
    {
      return reinterpret_cast<PipelineRasterizationStateCreateFlags&>( m_pipelineRasterizationStateCreateInfo.flags );
    }

    PipelineRasterizationStateCreateInfo& flags( PipelineRasterizationStateCreateFlags flags )
    {
      m_pipelineRasterizationStateCreateInfo.flags = static_cast<VkPipelineRasterizationStateCreateFlags>( flags );
      return *this;
    }

    const Bool32& depthClampEnable() const
    {
      return m_pipelineRasterizationStateCreateInfo.depthClampEnable;
    }

    Bool32& depthClampEnable()
    {
      return m_pipelineRasterizationStateCreateInfo.depthClampEnable;
    }

    PipelineRasterizationStateCreateInfo& depthClampEnable( Bool32 depthClampEnable )
    {
      m_pipelineRasterizationStateCreateInfo.depthClampEnable = depthClampEnable;
      return *this;
    }

    const Bool32& rasterizerDiscardEnable() const
    {
      return m_pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable;
    }

    Bool32& rasterizerDiscardEnable()
    {
      return m_pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable;
    }

    PipelineRasterizationStateCreateInfo& rasterizerDiscardEnable( Bool32 rasterizerDiscardEnable )
    {
      m_pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = rasterizerDiscardEnable;
      return *this;
    }

    const PolygonMode& polygonMode() const
    {
      return reinterpret_cast<const PolygonMode&>( m_pipelineRasterizationStateCreateInfo.polygonMode );
    }

    PolygonMode& polygonMode()
    {
      return reinterpret_cast<PolygonMode&>( m_pipelineRasterizationStateCreateInfo.polygonMode );
    }

    PipelineRasterizationStateCreateInfo& polygonMode( PolygonMode polygonMode )
    {
      m_pipelineRasterizationStateCreateInfo.polygonMode = static_cast<VkPolygonMode>( polygonMode );
      return *this;
    }

    const CullModeFlags& cullMode() const
    {
      return reinterpret_cast<const CullModeFlags&>( m_pipelineRasterizationStateCreateInfo.cullMode );
    }

    CullModeFlags& cullMode()
    {
      return reinterpret_cast<CullModeFlags&>( m_pipelineRasterizationStateCreateInfo.cullMode );
    }

    PipelineRasterizationStateCreateInfo& cullMode( CullModeFlags cullMode )
    {
      m_pipelineRasterizationStateCreateInfo.cullMode = static_cast<VkCullModeFlags>( cullMode );
      return *this;
    }

    const FrontFace& frontFace() const
    {
      return reinterpret_cast<const FrontFace&>( m_pipelineRasterizationStateCreateInfo.frontFace );
    }

    FrontFace& frontFace()
    {
      return reinterpret_cast<FrontFace&>( m_pipelineRasterizationStateCreateInfo.frontFace );
    }

    PipelineRasterizationStateCreateInfo& frontFace( FrontFace frontFace )
    {
      m_pipelineRasterizationStateCreateInfo.frontFace = static_cast<VkFrontFace>( frontFace );
      return *this;
    }

    const Bool32& depthBiasEnable() const
    {
      return m_pipelineRasterizationStateCreateInfo.depthBiasEnable;
    }

    Bool32& depthBiasEnable()
    {
      return m_pipelineRasterizationStateCreateInfo.depthBiasEnable;
    }

    PipelineRasterizationStateCreateInfo& depthBiasEnable( Bool32 depthBiasEnable )
    {
      m_pipelineRasterizationStateCreateInfo.depthBiasEnable = depthBiasEnable;
      return *this;
    }

    const float& depthBiasConstantFactor() const
    {
      return m_pipelineRasterizationStateCreateInfo.depthBiasConstantFactor;
    }

    float& depthBiasConstantFactor()
    {
      return m_pipelineRasterizationStateCreateInfo.depthBiasConstantFactor;
    }

    PipelineRasterizationStateCreateInfo& depthBiasConstantFactor( float depthBiasConstantFactor )
    {
      m_pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = depthBiasConstantFactor;
      return *this;
    }

    const float& depthBiasClamp() const
    {
      return m_pipelineRasterizationStateCreateInfo.depthBiasClamp;
    }

    float& depthBiasClamp()
    {
      return m_pipelineRasterizationStateCreateInfo.depthBiasClamp;
    }

    PipelineRasterizationStateCreateInfo& depthBiasClamp( float depthBiasClamp )
    {
      m_pipelineRasterizationStateCreateInfo.depthBiasClamp = depthBiasClamp;
      return *this;
    }

    const float& depthBiasSlopeFactor() const
    {
      return m_pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor;
    }

    float& depthBiasSlopeFactor()
    {
      return m_pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor;
    }

    PipelineRasterizationStateCreateInfo& depthBiasSlopeFactor( float depthBiasSlopeFactor )
    {
      m_pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = depthBiasSlopeFactor;
      return *this;
    }

    const float& lineWidth() const
    {
      return m_pipelineRasterizationStateCreateInfo.lineWidth;
    }

    float& lineWidth()
    {
      return m_pipelineRasterizationStateCreateInfo.lineWidth;
    }

    PipelineRasterizationStateCreateInfo& lineWidth( float lineWidth )
    {
      m_pipelineRasterizationStateCreateInfo.lineWidth = lineWidth;
      return *this;
    }

    static PipelineRasterizationStateCreateInfo& null()
    {
      return *((PipelineRasterizationStateCreateInfo*)(nullptr));
    }

    operator const VkPipelineRasterizationStateCreateInfo&() const
    {
      return m_pipelineRasterizationStateCreateInfo;
    }

  private:
    VkPipelineRasterizationStateCreateInfo m_pipelineRasterizationStateCreateInfo;
  };
  static_assert( sizeof( PipelineRasterizationStateCreateInfo ) == sizeof( VkPipelineRasterizationStateCreateInfo ), "struct and wrapper have different size!" );

  class PipelineDepthStencilStateCreateInfo
  {
  public:
    PipelineDepthStencilStateCreateInfo()
      : PipelineDepthStencilStateCreateInfo( PipelineDepthStencilStateCreateFlags(), 0, 0, CompareOp::eNever, 0, 0, StencilOpState(), StencilOpState(), 0, 0 )
    {}

    PipelineDepthStencilStateCreateInfo( PipelineDepthStencilStateCreateFlags flags, Bool32 depthTestEnable, Bool32 depthWriteEnable, CompareOp depthCompareOp, Bool32 depthBoundsTestEnable, Bool32 stencilTestEnable, StencilOpState front, StencilOpState back, float minDepthBounds, float maxDepthBounds)
    {
      m_pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
      m_pipelineDepthStencilStateCreateInfo.pNext = nullptr;
      m_pipelineDepthStencilStateCreateInfo.flags = static_cast<VkPipelineDepthStencilStateCreateFlags>( flags );
      m_pipelineDepthStencilStateCreateInfo.depthTestEnable = depthTestEnable;
      m_pipelineDepthStencilStateCreateInfo.depthWriteEnable = depthWriteEnable;
      m_pipelineDepthStencilStateCreateInfo.depthCompareOp = static_cast<VkCompareOp>( depthCompareOp );
      m_pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = depthBoundsTestEnable;
      m_pipelineDepthStencilStateCreateInfo.stencilTestEnable = stencilTestEnable;
      m_pipelineDepthStencilStateCreateInfo.front = static_cast<VkStencilOpState>( front );
      m_pipelineDepthStencilStateCreateInfo.back = static_cast<VkStencilOpState>( back );
      m_pipelineDepthStencilStateCreateInfo.minDepthBounds = minDepthBounds;
      m_pipelineDepthStencilStateCreateInfo.maxDepthBounds = maxDepthBounds;
    }

    PipelineDepthStencilStateCreateInfo(VkPipelineDepthStencilStateCreateInfo const & rhs)
      : m_pipelineDepthStencilStateCreateInfo(rhs)
    {
    }

    PipelineDepthStencilStateCreateInfo& operator=(VkPipelineDepthStencilStateCreateInfo const & rhs)
    {
      m_pipelineDepthStencilStateCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineDepthStencilStateCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_pipelineDepthStencilStateCreateInfo.sType );
    }

    PipelineDepthStencilStateCreateInfo& sType( StructureType sType )
    {
      m_pipelineDepthStencilStateCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_pipelineDepthStencilStateCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_pipelineDepthStencilStateCreateInfo.pNext );
    }

    PipelineDepthStencilStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineDepthStencilStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineDepthStencilStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineDepthStencilStateCreateFlags&>( m_pipelineDepthStencilStateCreateInfo.flags );
    }

    PipelineDepthStencilStateCreateFlags& flags()
    {
      return reinterpret_cast<PipelineDepthStencilStateCreateFlags&>( m_pipelineDepthStencilStateCreateInfo.flags );
    }

    PipelineDepthStencilStateCreateInfo& flags( PipelineDepthStencilStateCreateFlags flags )
    {
      m_pipelineDepthStencilStateCreateInfo.flags = static_cast<VkPipelineDepthStencilStateCreateFlags>( flags );
      return *this;
    }

    const Bool32& depthTestEnable() const
    {
      return m_pipelineDepthStencilStateCreateInfo.depthTestEnable;
    }

    Bool32& depthTestEnable()
    {
      return m_pipelineDepthStencilStateCreateInfo.depthTestEnable;
    }

    PipelineDepthStencilStateCreateInfo& depthTestEnable( Bool32 depthTestEnable )
    {
      m_pipelineDepthStencilStateCreateInfo.depthTestEnable = depthTestEnable;
      return *this;
    }

    const Bool32& depthWriteEnable() const
    {
      return m_pipelineDepthStencilStateCreateInfo.depthWriteEnable;
    }

    Bool32& depthWriteEnable()
    {
      return m_pipelineDepthStencilStateCreateInfo.depthWriteEnable;
    }

    PipelineDepthStencilStateCreateInfo& depthWriteEnable( Bool32 depthWriteEnable )
    {
      m_pipelineDepthStencilStateCreateInfo.depthWriteEnable = depthWriteEnable;
      return *this;
    }

    const CompareOp& depthCompareOp() const
    {
      return reinterpret_cast<const CompareOp&>( m_pipelineDepthStencilStateCreateInfo.depthCompareOp );
    }

    CompareOp& depthCompareOp()
    {
      return reinterpret_cast<CompareOp&>( m_pipelineDepthStencilStateCreateInfo.depthCompareOp );
    }

    PipelineDepthStencilStateCreateInfo& depthCompareOp( CompareOp depthCompareOp )
    {
      m_pipelineDepthStencilStateCreateInfo.depthCompareOp = static_cast<VkCompareOp>( depthCompareOp );
      return *this;
    }

    const Bool32& depthBoundsTestEnable() const
    {
      return m_pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable;
    }

    Bool32& depthBoundsTestEnable()
    {
      return m_pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable;
    }

    PipelineDepthStencilStateCreateInfo& depthBoundsTestEnable( Bool32 depthBoundsTestEnable )
    {
      m_pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = depthBoundsTestEnable;
      return *this;
    }

    const Bool32& stencilTestEnable() const
    {
      return m_pipelineDepthStencilStateCreateInfo.stencilTestEnable;
    }

    Bool32& stencilTestEnable()
    {
      return m_pipelineDepthStencilStateCreateInfo.stencilTestEnable;
    }

    PipelineDepthStencilStateCreateInfo& stencilTestEnable( Bool32 stencilTestEnable )
    {
      m_pipelineDepthStencilStateCreateInfo.stencilTestEnable = stencilTestEnable;
      return *this;
    }

    const StencilOpState& front() const
    {
      return reinterpret_cast<const StencilOpState&>( m_pipelineDepthStencilStateCreateInfo.front );
    }

    StencilOpState& front()
    {
      return reinterpret_cast<StencilOpState&>( m_pipelineDepthStencilStateCreateInfo.front );
    }

    PipelineDepthStencilStateCreateInfo& front( StencilOpState front )
    {
      m_pipelineDepthStencilStateCreateInfo.front = static_cast<VkStencilOpState>( front );
      return *this;
    }

    const StencilOpState& back() const
    {
      return reinterpret_cast<const StencilOpState&>( m_pipelineDepthStencilStateCreateInfo.back );
    }

    StencilOpState& back()
    {
      return reinterpret_cast<StencilOpState&>( m_pipelineDepthStencilStateCreateInfo.back );
    }

    PipelineDepthStencilStateCreateInfo& back( StencilOpState back )
    {
      m_pipelineDepthStencilStateCreateInfo.back = static_cast<VkStencilOpState>( back );
      return *this;
    }

    const float& minDepthBounds() const
    {
      return m_pipelineDepthStencilStateCreateInfo.minDepthBounds;
    }

    float& minDepthBounds()
    {
      return m_pipelineDepthStencilStateCreateInfo.minDepthBounds;
    }

    PipelineDepthStencilStateCreateInfo& minDepthBounds( float minDepthBounds )
    {
      m_pipelineDepthStencilStateCreateInfo.minDepthBounds = minDepthBounds;
      return *this;
    }

    const float& maxDepthBounds() const
    {
      return m_pipelineDepthStencilStateCreateInfo.maxDepthBounds;
    }

    float& maxDepthBounds()
    {
      return m_pipelineDepthStencilStateCreateInfo.maxDepthBounds;
    }

    PipelineDepthStencilStateCreateInfo& maxDepthBounds( float maxDepthBounds )
    {
      m_pipelineDepthStencilStateCreateInfo.maxDepthBounds = maxDepthBounds;
      return *this;
    }

    static PipelineDepthStencilStateCreateInfo& null()
    {
      return *((PipelineDepthStencilStateCreateInfo*)(nullptr));
    }

    operator const VkPipelineDepthStencilStateCreateInfo&() const
    {
      return m_pipelineDepthStencilStateCreateInfo;
    }

  private:
    VkPipelineDepthStencilStateCreateInfo m_pipelineDepthStencilStateCreateInfo;
  };
  static_assert( sizeof( PipelineDepthStencilStateCreateInfo ) == sizeof( VkPipelineDepthStencilStateCreateInfo ), "struct and wrapper have different size!" );

  class PipelineCacheCreateInfo
  {
  public:
    PipelineCacheCreateInfo()
      : PipelineCacheCreateInfo( PipelineCacheCreateFlags(), 0, nullptr )
    {}

    PipelineCacheCreateInfo( PipelineCacheCreateFlags flags, size_t initialDataSize, const void* pInitialData)
    {
      m_pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
      m_pipelineCacheCreateInfo.pNext = nullptr;
      m_pipelineCacheCreateInfo.flags = static_cast<VkPipelineCacheCreateFlags>( flags );
      m_pipelineCacheCreateInfo.initialDataSize = initialDataSize;
      m_pipelineCacheCreateInfo.pInitialData = pInitialData;
    }

    PipelineCacheCreateInfo(VkPipelineCacheCreateInfo const & rhs)
      : m_pipelineCacheCreateInfo(rhs)
    {
    }

    PipelineCacheCreateInfo& operator=(VkPipelineCacheCreateInfo const & rhs)
    {
      m_pipelineCacheCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineCacheCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_pipelineCacheCreateInfo.sType );
    }

    PipelineCacheCreateInfo& sType( StructureType sType )
    {
      m_pipelineCacheCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_pipelineCacheCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_pipelineCacheCreateInfo.pNext );
    }

    PipelineCacheCreateInfo& pNext( const void* pNext )
    {
      m_pipelineCacheCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineCacheCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineCacheCreateFlags&>( m_pipelineCacheCreateInfo.flags );
    }

    PipelineCacheCreateFlags& flags()
    {
      return reinterpret_cast<PipelineCacheCreateFlags&>( m_pipelineCacheCreateInfo.flags );
    }

    PipelineCacheCreateInfo& flags( PipelineCacheCreateFlags flags )
    {
      m_pipelineCacheCreateInfo.flags = static_cast<VkPipelineCacheCreateFlags>( flags );
      return *this;
    }

    const size_t& initialDataSize() const
    {
      return m_pipelineCacheCreateInfo.initialDataSize;
    }

    size_t& initialDataSize()
    {
      return m_pipelineCacheCreateInfo.initialDataSize;
    }

    PipelineCacheCreateInfo& initialDataSize( size_t initialDataSize )
    {
      m_pipelineCacheCreateInfo.initialDataSize = initialDataSize;
      return *this;
    }

    const void* pInitialData() const
    {
      return reinterpret_cast<const void*>( m_pipelineCacheCreateInfo.pInitialData );
    }

    const void* pInitialData()
    {
      return reinterpret_cast<const void*>( m_pipelineCacheCreateInfo.pInitialData );
    }

    PipelineCacheCreateInfo& pInitialData( const void* pInitialData )
    {
      m_pipelineCacheCreateInfo.pInitialData = pInitialData;
      return *this;
    }

    static PipelineCacheCreateInfo& null()
    {
      return *((PipelineCacheCreateInfo*)(nullptr));
    }

    operator const VkPipelineCacheCreateInfo&() const
    {
      return m_pipelineCacheCreateInfo;
    }

  private:
    VkPipelineCacheCreateInfo m_pipelineCacheCreateInfo;
  };
  static_assert( sizeof( PipelineCacheCreateInfo ) == sizeof( VkPipelineCacheCreateInfo ), "struct and wrapper have different size!" );

  class SamplerCreateInfo
  {
  public:
    SamplerCreateInfo()
      : SamplerCreateInfo( SamplerCreateFlags(), Filter::eNearest, Filter::eNearest, SamplerMipmapMode::eNearest, SamplerAddressMode::eRepeat, SamplerAddressMode::eRepeat, SamplerAddressMode::eRepeat, 0, 0, 0, 0, CompareOp::eNever, 0, 0, BorderColor::eFloatTransparentBlack, 0 )
    {}

    SamplerCreateInfo( SamplerCreateFlags flags, Filter magFilter, Filter minFilter, SamplerMipmapMode mipmapMode, SamplerAddressMode addressModeU, SamplerAddressMode addressModeV, SamplerAddressMode addressModeW, float mipLodBias, Bool32 anisotropyEnable, float maxAnisotropy, Bool32 compareEnable, CompareOp compareOp, float minLod, float maxLod, BorderColor borderColor, Bool32 unnormalizedCoordinates)
    {
      m_samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
      m_samplerCreateInfo.pNext = nullptr;
      m_samplerCreateInfo.flags = static_cast<VkSamplerCreateFlags>( flags );
      m_samplerCreateInfo.magFilter = static_cast<VkFilter>( magFilter );
      m_samplerCreateInfo.minFilter = static_cast<VkFilter>( minFilter );
      m_samplerCreateInfo.mipmapMode = static_cast<VkSamplerMipmapMode>( mipmapMode );
      m_samplerCreateInfo.addressModeU = static_cast<VkSamplerAddressMode>( addressModeU );
      m_samplerCreateInfo.addressModeV = static_cast<VkSamplerAddressMode>( addressModeV );
      m_samplerCreateInfo.addressModeW = static_cast<VkSamplerAddressMode>( addressModeW );
      m_samplerCreateInfo.mipLodBias = mipLodBias;
      m_samplerCreateInfo.anisotropyEnable = anisotropyEnable;
      m_samplerCreateInfo.maxAnisotropy = maxAnisotropy;
      m_samplerCreateInfo.compareEnable = compareEnable;
      m_samplerCreateInfo.compareOp = static_cast<VkCompareOp>( compareOp );
      m_samplerCreateInfo.minLod = minLod;
      m_samplerCreateInfo.maxLod = maxLod;
      m_samplerCreateInfo.borderColor = static_cast<VkBorderColor>( borderColor );
      m_samplerCreateInfo.unnormalizedCoordinates = unnormalizedCoordinates;
    }

    SamplerCreateInfo(VkSamplerCreateInfo const & rhs)
      : m_samplerCreateInfo(rhs)
    {
    }

    SamplerCreateInfo& operator=(VkSamplerCreateInfo const & rhs)
    {
      m_samplerCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_samplerCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_samplerCreateInfo.sType );
    }

    SamplerCreateInfo& sType( StructureType sType )
    {
      m_samplerCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_samplerCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_samplerCreateInfo.pNext );
    }

    SamplerCreateInfo& pNext( const void* pNext )
    {
      m_samplerCreateInfo.pNext = pNext;
      return *this;
    }

    const SamplerCreateFlags& flags() const
    {
      return reinterpret_cast<const SamplerCreateFlags&>( m_samplerCreateInfo.flags );
    }

    SamplerCreateFlags& flags()
    {
      return reinterpret_cast<SamplerCreateFlags&>( m_samplerCreateInfo.flags );
    }

    SamplerCreateInfo& flags( SamplerCreateFlags flags )
    {
      m_samplerCreateInfo.flags = static_cast<VkSamplerCreateFlags>( flags );
      return *this;
    }

    const Filter& magFilter() const
    {
      return reinterpret_cast<const Filter&>( m_samplerCreateInfo.magFilter );
    }

    Filter& magFilter()
    {
      return reinterpret_cast<Filter&>( m_samplerCreateInfo.magFilter );
    }

    SamplerCreateInfo& magFilter( Filter magFilter )
    {
      m_samplerCreateInfo.magFilter = static_cast<VkFilter>( magFilter );
      return *this;
    }

    const Filter& minFilter() const
    {
      return reinterpret_cast<const Filter&>( m_samplerCreateInfo.minFilter );
    }

    Filter& minFilter()
    {
      return reinterpret_cast<Filter&>( m_samplerCreateInfo.minFilter );
    }

    SamplerCreateInfo& minFilter( Filter minFilter )
    {
      m_samplerCreateInfo.minFilter = static_cast<VkFilter>( minFilter );
      return *this;
    }

    const SamplerMipmapMode& mipmapMode() const
    {
      return reinterpret_cast<const SamplerMipmapMode&>( m_samplerCreateInfo.mipmapMode );
    }

    SamplerMipmapMode& mipmapMode()
    {
      return reinterpret_cast<SamplerMipmapMode&>( m_samplerCreateInfo.mipmapMode );
    }

    SamplerCreateInfo& mipmapMode( SamplerMipmapMode mipmapMode )
    {
      m_samplerCreateInfo.mipmapMode = static_cast<VkSamplerMipmapMode>( mipmapMode );
      return *this;
    }

    const SamplerAddressMode& addressModeU() const
    {
      return reinterpret_cast<const SamplerAddressMode&>( m_samplerCreateInfo.addressModeU );
    }

    SamplerAddressMode& addressModeU()
    {
      return reinterpret_cast<SamplerAddressMode&>( m_samplerCreateInfo.addressModeU );
    }

    SamplerCreateInfo& addressModeU( SamplerAddressMode addressModeU )
    {
      m_samplerCreateInfo.addressModeU = static_cast<VkSamplerAddressMode>( addressModeU );
      return *this;
    }

    const SamplerAddressMode& addressModeV() const
    {
      return reinterpret_cast<const SamplerAddressMode&>( m_samplerCreateInfo.addressModeV );
    }

    SamplerAddressMode& addressModeV()
    {
      return reinterpret_cast<SamplerAddressMode&>( m_samplerCreateInfo.addressModeV );
    }

    SamplerCreateInfo& addressModeV( SamplerAddressMode addressModeV )
    {
      m_samplerCreateInfo.addressModeV = static_cast<VkSamplerAddressMode>( addressModeV );
      return *this;
    }

    const SamplerAddressMode& addressModeW() const
    {
      return reinterpret_cast<const SamplerAddressMode&>( m_samplerCreateInfo.addressModeW );
    }

    SamplerAddressMode& addressModeW()
    {
      return reinterpret_cast<SamplerAddressMode&>( m_samplerCreateInfo.addressModeW );
    }

    SamplerCreateInfo& addressModeW( SamplerAddressMode addressModeW )
    {
      m_samplerCreateInfo.addressModeW = static_cast<VkSamplerAddressMode>( addressModeW );
      return *this;
    }

    const float& mipLodBias() const
    {
      return m_samplerCreateInfo.mipLodBias;
    }

    float& mipLodBias()
    {
      return m_samplerCreateInfo.mipLodBias;
    }

    SamplerCreateInfo& mipLodBias( float mipLodBias )
    {
      m_samplerCreateInfo.mipLodBias = mipLodBias;
      return *this;
    }

    const Bool32& anisotropyEnable() const
    {
      return m_samplerCreateInfo.anisotropyEnable;
    }

    Bool32& anisotropyEnable()
    {
      return m_samplerCreateInfo.anisotropyEnable;
    }

    SamplerCreateInfo& anisotropyEnable( Bool32 anisotropyEnable )
    {
      m_samplerCreateInfo.anisotropyEnable = anisotropyEnable;
      return *this;
    }

    const float& maxAnisotropy() const
    {
      return m_samplerCreateInfo.maxAnisotropy;
    }

    float& maxAnisotropy()
    {
      return m_samplerCreateInfo.maxAnisotropy;
    }

    SamplerCreateInfo& maxAnisotropy( float maxAnisotropy )
    {
      m_samplerCreateInfo.maxAnisotropy = maxAnisotropy;
      return *this;
    }

    const Bool32& compareEnable() const
    {
      return m_samplerCreateInfo.compareEnable;
    }

    Bool32& compareEnable()
    {
      return m_samplerCreateInfo.compareEnable;
    }

    SamplerCreateInfo& compareEnable( Bool32 compareEnable )
    {
      m_samplerCreateInfo.compareEnable = compareEnable;
      return *this;
    }

    const CompareOp& compareOp() const
    {
      return reinterpret_cast<const CompareOp&>( m_samplerCreateInfo.compareOp );
    }

    CompareOp& compareOp()
    {
      return reinterpret_cast<CompareOp&>( m_samplerCreateInfo.compareOp );
    }

    SamplerCreateInfo& compareOp( CompareOp compareOp )
    {
      m_samplerCreateInfo.compareOp = static_cast<VkCompareOp>( compareOp );
      return *this;
    }

    const float& minLod() const
    {
      return m_samplerCreateInfo.minLod;
    }

    float& minLod()
    {
      return m_samplerCreateInfo.minLod;
    }

    SamplerCreateInfo& minLod( float minLod )
    {
      m_samplerCreateInfo.minLod = minLod;
      return *this;
    }

    const float& maxLod() const
    {
      return m_samplerCreateInfo.maxLod;
    }

    float& maxLod()
    {
      return m_samplerCreateInfo.maxLod;
    }

    SamplerCreateInfo& maxLod( float maxLod )
    {
      m_samplerCreateInfo.maxLod = maxLod;
      return *this;
    }

    const BorderColor& borderColor() const
    {
      return reinterpret_cast<const BorderColor&>( m_samplerCreateInfo.borderColor );
    }

    BorderColor& borderColor()
    {
      return reinterpret_cast<BorderColor&>( m_samplerCreateInfo.borderColor );
    }

    SamplerCreateInfo& borderColor( BorderColor borderColor )
    {
      m_samplerCreateInfo.borderColor = static_cast<VkBorderColor>( borderColor );
      return *this;
    }

    const Bool32& unnormalizedCoordinates() const
    {
      return m_samplerCreateInfo.unnormalizedCoordinates;
    }

    Bool32& unnormalizedCoordinates()
    {
      return m_samplerCreateInfo.unnormalizedCoordinates;
    }

    SamplerCreateInfo& unnormalizedCoordinates( Bool32 unnormalizedCoordinates )
    {
      m_samplerCreateInfo.unnormalizedCoordinates = unnormalizedCoordinates;
      return *this;
    }

    static SamplerCreateInfo& null()
    {
      return *((SamplerCreateInfo*)(nullptr));
    }

    operator const VkSamplerCreateInfo&() const
    {
      return m_samplerCreateInfo;
    }

  private:
    VkSamplerCreateInfo m_samplerCreateInfo;
  };
  static_assert( sizeof( SamplerCreateInfo ) == sizeof( VkSamplerCreateInfo ), "struct and wrapper have different size!" );

  class CommandBufferAllocateInfo
  {
  public:
    CommandBufferAllocateInfo()
      : CommandBufferAllocateInfo( CommandPool(), CommandBufferLevel::ePrimary, 0 )
    {}

    CommandBufferAllocateInfo( CommandPool commandPool, CommandBufferLevel level, uint32_t commandBufferCount)
    {
      m_commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      m_commandBufferAllocateInfo.pNext = nullptr;
      m_commandBufferAllocateInfo.commandPool = static_cast<VkCommandPool>( commandPool );
      m_commandBufferAllocateInfo.level = static_cast<VkCommandBufferLevel>( level );
      m_commandBufferAllocateInfo.commandBufferCount = commandBufferCount;
    }

    CommandBufferAllocateInfo(VkCommandBufferAllocateInfo const & rhs)
      : m_commandBufferAllocateInfo(rhs)
    {
    }

    CommandBufferAllocateInfo& operator=(VkCommandBufferAllocateInfo const & rhs)
    {
      m_commandBufferAllocateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_commandBufferAllocateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_commandBufferAllocateInfo.sType );
    }

    CommandBufferAllocateInfo& sType( StructureType sType )
    {
      m_commandBufferAllocateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_commandBufferAllocateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_commandBufferAllocateInfo.pNext );
    }

    CommandBufferAllocateInfo& pNext( const void* pNext )
    {
      m_commandBufferAllocateInfo.pNext = pNext;
      return *this;
    }

    const CommandPool& commandPool() const
    {
      return reinterpret_cast<const CommandPool&>( m_commandBufferAllocateInfo.commandPool );
    }

    CommandPool& commandPool()
    {
      return reinterpret_cast<CommandPool&>( m_commandBufferAllocateInfo.commandPool );
    }

    CommandBufferAllocateInfo& commandPool( CommandPool commandPool )
    {
      m_commandBufferAllocateInfo.commandPool = static_cast<VkCommandPool>( commandPool );
      return *this;
    }

    const CommandBufferLevel& level() const
    {
      return reinterpret_cast<const CommandBufferLevel&>( m_commandBufferAllocateInfo.level );
    }

    CommandBufferLevel& level()
    {
      return reinterpret_cast<CommandBufferLevel&>( m_commandBufferAllocateInfo.level );
    }

    CommandBufferAllocateInfo& level( CommandBufferLevel level )
    {
      m_commandBufferAllocateInfo.level = static_cast<VkCommandBufferLevel>( level );
      return *this;
    }

    const uint32_t& commandBufferCount() const
    {
      return m_commandBufferAllocateInfo.commandBufferCount;
    }

    uint32_t& commandBufferCount()
    {
      return m_commandBufferAllocateInfo.commandBufferCount;
    }

    CommandBufferAllocateInfo& commandBufferCount( uint32_t commandBufferCount )
    {
      m_commandBufferAllocateInfo.commandBufferCount = commandBufferCount;
      return *this;
    }

    static CommandBufferAllocateInfo& null()
    {
      return *((CommandBufferAllocateInfo*)(nullptr));
    }

    operator const VkCommandBufferAllocateInfo&() const
    {
      return m_commandBufferAllocateInfo;
    }

  private:
    VkCommandBufferAllocateInfo m_commandBufferAllocateInfo;
  };
  static_assert( sizeof( CommandBufferAllocateInfo ) == sizeof( VkCommandBufferAllocateInfo ), "struct and wrapper have different size!" );

  class RenderPassBeginInfo
  {
  public:
    RenderPassBeginInfo()
      : RenderPassBeginInfo( RenderPass(), Framebuffer(), Rect2D(), 0, nullptr )
    {}

    RenderPassBeginInfo( RenderPass renderPass, Framebuffer framebuffer, Rect2D renderArea, uint32_t clearValueCount, const ClearValue* pClearValues)
    {
      m_renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      m_renderPassBeginInfo.pNext = nullptr;
      m_renderPassBeginInfo.renderPass = static_cast<VkRenderPass>( renderPass );
      m_renderPassBeginInfo.framebuffer = static_cast<VkFramebuffer>( framebuffer );
      m_renderPassBeginInfo.renderArea = static_cast<VkRect2D>( renderArea );
      m_renderPassBeginInfo.clearValueCount = clearValueCount;
      m_renderPassBeginInfo.pClearValues = reinterpret_cast<const VkClearValue*>( pClearValues );
    }

    RenderPassBeginInfo(VkRenderPassBeginInfo const & rhs)
      : m_renderPassBeginInfo(rhs)
    {
    }

    RenderPassBeginInfo& operator=(VkRenderPassBeginInfo const & rhs)
    {
      m_renderPassBeginInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_renderPassBeginInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_renderPassBeginInfo.sType );
    }

    RenderPassBeginInfo& sType( StructureType sType )
    {
      m_renderPassBeginInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_renderPassBeginInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_renderPassBeginInfo.pNext );
    }

    RenderPassBeginInfo& pNext( const void* pNext )
    {
      m_renderPassBeginInfo.pNext = pNext;
      return *this;
    }

    const RenderPass& renderPass() const
    {
      return reinterpret_cast<const RenderPass&>( m_renderPassBeginInfo.renderPass );
    }

    RenderPass& renderPass()
    {
      return reinterpret_cast<RenderPass&>( m_renderPassBeginInfo.renderPass );
    }

    RenderPassBeginInfo& renderPass( RenderPass renderPass )
    {
      m_renderPassBeginInfo.renderPass = static_cast<VkRenderPass>( renderPass );
      return *this;
    }

    const Framebuffer& framebuffer() const
    {
      return reinterpret_cast<const Framebuffer&>( m_renderPassBeginInfo.framebuffer );
    }

    Framebuffer& framebuffer()
    {
      return reinterpret_cast<Framebuffer&>( m_renderPassBeginInfo.framebuffer );
    }

    RenderPassBeginInfo& framebuffer( Framebuffer framebuffer )
    {
      m_renderPassBeginInfo.framebuffer = static_cast<VkFramebuffer>( framebuffer );
      return *this;
    }

    const Rect2D& renderArea() const
    {
      return reinterpret_cast<const Rect2D&>( m_renderPassBeginInfo.renderArea );
    }

    Rect2D& renderArea()
    {
      return reinterpret_cast<Rect2D&>( m_renderPassBeginInfo.renderArea );
    }

    RenderPassBeginInfo& renderArea( Rect2D renderArea )
    {
      m_renderPassBeginInfo.renderArea = static_cast<VkRect2D>( renderArea );
      return *this;
    }

    const uint32_t& clearValueCount() const
    {
      return m_renderPassBeginInfo.clearValueCount;
    }

    uint32_t& clearValueCount()
    {
      return m_renderPassBeginInfo.clearValueCount;
    }

    RenderPassBeginInfo& clearValueCount( uint32_t clearValueCount )
    {
      m_renderPassBeginInfo.clearValueCount = clearValueCount;
      return *this;
    }

    const ClearValue* pClearValues() const
    {
      return reinterpret_cast<const ClearValue*>( m_renderPassBeginInfo.pClearValues );
    }

    const ClearValue* pClearValues()
    {
      return reinterpret_cast<const ClearValue*>( m_renderPassBeginInfo.pClearValues );
    }

    RenderPassBeginInfo& pClearValues( const ClearValue* pClearValues )
    {
      m_renderPassBeginInfo.pClearValues = reinterpret_cast<const VkClearValue*>( pClearValues );
      return *this;
    }

    static RenderPassBeginInfo& null()
    {
      return *((RenderPassBeginInfo*)(nullptr));
    }

    operator const VkRenderPassBeginInfo&() const
    {
      return m_renderPassBeginInfo;
    }

  private:
    VkRenderPassBeginInfo m_renderPassBeginInfo;
  };
  static_assert( sizeof( RenderPassBeginInfo ) == sizeof( VkRenderPassBeginInfo ), "struct and wrapper have different size!" );

  class EventCreateInfo
  {
  public:
    EventCreateInfo()
      : EventCreateInfo( EventCreateFlags() )
    {}

    EventCreateInfo( EventCreateFlags flags)
    {
      m_eventCreateInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
      m_eventCreateInfo.pNext = nullptr;
      m_eventCreateInfo.flags = static_cast<VkEventCreateFlags>( flags );
    }

    EventCreateInfo(VkEventCreateInfo const & rhs)
      : m_eventCreateInfo(rhs)
    {
    }

    EventCreateInfo& operator=(VkEventCreateInfo const & rhs)
    {
      m_eventCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_eventCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_eventCreateInfo.sType );
    }

    EventCreateInfo& sType( StructureType sType )
    {
      m_eventCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_eventCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_eventCreateInfo.pNext );
    }

    EventCreateInfo& pNext( const void* pNext )
    {
      m_eventCreateInfo.pNext = pNext;
      return *this;
    }

    const EventCreateFlags& flags() const
    {
      return reinterpret_cast<const EventCreateFlags&>( m_eventCreateInfo.flags );
    }

    EventCreateFlags& flags()
    {
      return reinterpret_cast<EventCreateFlags&>( m_eventCreateInfo.flags );
    }

    EventCreateInfo& flags( EventCreateFlags flags )
    {
      m_eventCreateInfo.flags = static_cast<VkEventCreateFlags>( flags );
      return *this;
    }

    static EventCreateInfo& null()
    {
      return *((EventCreateInfo*)(nullptr));
    }

    operator const VkEventCreateInfo&() const
    {
      return m_eventCreateInfo;
    }

  private:
    VkEventCreateInfo m_eventCreateInfo;
  };
  static_assert( sizeof( EventCreateInfo ) == sizeof( VkEventCreateInfo ), "struct and wrapper have different size!" );

  class SemaphoreCreateInfo
  {
  public:
    SemaphoreCreateInfo()
      : SemaphoreCreateInfo( SemaphoreCreateFlags() )
    {}

    SemaphoreCreateInfo( SemaphoreCreateFlags flags)
    {
      m_semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
      m_semaphoreCreateInfo.pNext = nullptr;
      m_semaphoreCreateInfo.flags = static_cast<VkSemaphoreCreateFlags>( flags );
    }

    SemaphoreCreateInfo(VkSemaphoreCreateInfo const & rhs)
      : m_semaphoreCreateInfo(rhs)
    {
    }

    SemaphoreCreateInfo& operator=(VkSemaphoreCreateInfo const & rhs)
    {
      m_semaphoreCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_semaphoreCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_semaphoreCreateInfo.sType );
    }

    SemaphoreCreateInfo& sType( StructureType sType )
    {
      m_semaphoreCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_semaphoreCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_semaphoreCreateInfo.pNext );
    }

    SemaphoreCreateInfo& pNext( const void* pNext )
    {
      m_semaphoreCreateInfo.pNext = pNext;
      return *this;
    }

    const SemaphoreCreateFlags& flags() const
    {
      return reinterpret_cast<const SemaphoreCreateFlags&>( m_semaphoreCreateInfo.flags );
    }

    SemaphoreCreateFlags& flags()
    {
      return reinterpret_cast<SemaphoreCreateFlags&>( m_semaphoreCreateInfo.flags );
    }

    SemaphoreCreateInfo& flags( SemaphoreCreateFlags flags )
    {
      m_semaphoreCreateInfo.flags = static_cast<VkSemaphoreCreateFlags>( flags );
      return *this;
    }

    static SemaphoreCreateInfo& null()
    {
      return *((SemaphoreCreateInfo*)(nullptr));
    }

    operator const VkSemaphoreCreateInfo&() const
    {
      return m_semaphoreCreateInfo;
    }

  private:
    VkSemaphoreCreateInfo m_semaphoreCreateInfo;
  };
  static_assert( sizeof( SemaphoreCreateInfo ) == sizeof( VkSemaphoreCreateInfo ), "struct and wrapper have different size!" );

  class FramebufferCreateInfo
  {
  public:
    FramebufferCreateInfo()
      : FramebufferCreateInfo( FramebufferCreateFlags(), RenderPass(), 0, nullptr, 0, 0, 0 )
    {}

    FramebufferCreateInfo( FramebufferCreateFlags flags, RenderPass renderPass, uint32_t attachmentCount, const ImageView* pAttachments, uint32_t width, uint32_t height, uint32_t layers)
    {
      m_framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      m_framebufferCreateInfo.pNext = nullptr;
      m_framebufferCreateInfo.flags = static_cast<VkFramebufferCreateFlags>( flags );
      m_framebufferCreateInfo.renderPass = static_cast<VkRenderPass>( renderPass );
      m_framebufferCreateInfo.attachmentCount = attachmentCount;
      m_framebufferCreateInfo.pAttachments = reinterpret_cast<const VkImageView*>( pAttachments );
      m_framebufferCreateInfo.width = width;
      m_framebufferCreateInfo.height = height;
      m_framebufferCreateInfo.layers = layers;
    }

    FramebufferCreateInfo(VkFramebufferCreateInfo const & rhs)
      : m_framebufferCreateInfo(rhs)
    {
    }

    FramebufferCreateInfo& operator=(VkFramebufferCreateInfo const & rhs)
    {
      m_framebufferCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_framebufferCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_framebufferCreateInfo.sType );
    }

    FramebufferCreateInfo& sType( StructureType sType )
    {
      m_framebufferCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_framebufferCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_framebufferCreateInfo.pNext );
    }

    FramebufferCreateInfo& pNext( const void* pNext )
    {
      m_framebufferCreateInfo.pNext = pNext;
      return *this;
    }

    const FramebufferCreateFlags& flags() const
    {
      return reinterpret_cast<const FramebufferCreateFlags&>( m_framebufferCreateInfo.flags );
    }

    FramebufferCreateFlags& flags()
    {
      return reinterpret_cast<FramebufferCreateFlags&>( m_framebufferCreateInfo.flags );
    }

    FramebufferCreateInfo& flags( FramebufferCreateFlags flags )
    {
      m_framebufferCreateInfo.flags = static_cast<VkFramebufferCreateFlags>( flags );
      return *this;
    }

    const RenderPass& renderPass() const
    {
      return reinterpret_cast<const RenderPass&>( m_framebufferCreateInfo.renderPass );
    }

    RenderPass& renderPass()
    {
      return reinterpret_cast<RenderPass&>( m_framebufferCreateInfo.renderPass );
    }

    FramebufferCreateInfo& renderPass( RenderPass renderPass )
    {
      m_framebufferCreateInfo.renderPass = static_cast<VkRenderPass>( renderPass );
      return *this;
    }

    const uint32_t& attachmentCount() const
    {
      return m_framebufferCreateInfo.attachmentCount;
    }

    uint32_t& attachmentCount()
    {
      return m_framebufferCreateInfo.attachmentCount;
    }

    FramebufferCreateInfo& attachmentCount( uint32_t attachmentCount )
    {
      m_framebufferCreateInfo.attachmentCount = attachmentCount;
      return *this;
    }

    const ImageView* pAttachments() const
    {
      return reinterpret_cast<const ImageView*>( m_framebufferCreateInfo.pAttachments );
    }

    const ImageView* pAttachments()
    {
      return reinterpret_cast<const ImageView*>( m_framebufferCreateInfo.pAttachments );
    }

    FramebufferCreateInfo& pAttachments( const ImageView* pAttachments )
    {
      m_framebufferCreateInfo.pAttachments = reinterpret_cast<const VkImageView*>( pAttachments );
      return *this;
    }

    const uint32_t& width() const
    {
      return m_framebufferCreateInfo.width;
    }

    uint32_t& width()
    {
      return m_framebufferCreateInfo.width;
    }

    FramebufferCreateInfo& width( uint32_t width )
    {
      m_framebufferCreateInfo.width = width;
      return *this;
    }

    const uint32_t& height() const
    {
      return m_framebufferCreateInfo.height;
    }

    uint32_t& height()
    {
      return m_framebufferCreateInfo.height;
    }

    FramebufferCreateInfo& height( uint32_t height )
    {
      m_framebufferCreateInfo.height = height;
      return *this;
    }

    const uint32_t& layers() const
    {
      return m_framebufferCreateInfo.layers;
    }

    uint32_t& layers()
    {
      return m_framebufferCreateInfo.layers;
    }

    FramebufferCreateInfo& layers( uint32_t layers )
    {
      m_framebufferCreateInfo.layers = layers;
      return *this;
    }

    static FramebufferCreateInfo& null()
    {
      return *((FramebufferCreateInfo*)(nullptr));
    }

    operator const VkFramebufferCreateInfo&() const
    {
      return m_framebufferCreateInfo;
    }

  private:
    VkFramebufferCreateInfo m_framebufferCreateInfo;
  };
  static_assert( sizeof( FramebufferCreateInfo ) == sizeof( VkFramebufferCreateInfo ), "struct and wrapper have different size!" );

  class DisplayModeCreateInfoKHR
  {
  public:
    DisplayModeCreateInfoKHR()
      : DisplayModeCreateInfoKHR( DisplayModeCreateFlagsKHR(), DisplayModeParametersKHR() )
    {}

    DisplayModeCreateInfoKHR( DisplayModeCreateFlagsKHR flags, DisplayModeParametersKHR parameters)
    {
      m_displayModeCreateInfoKHR.sType = VK_STRUCTURE_TYPE_DISPLAY_MODE_CREATE_INFO_KHR;
      m_displayModeCreateInfoKHR.pNext = nullptr;
      m_displayModeCreateInfoKHR.flags = static_cast<VkDisplayModeCreateFlagsKHR>( flags );
      m_displayModeCreateInfoKHR.parameters = static_cast<VkDisplayModeParametersKHR>( parameters );
    }

    DisplayModeCreateInfoKHR(VkDisplayModeCreateInfoKHR const & rhs)
      : m_displayModeCreateInfoKHR(rhs)
    {
    }

    DisplayModeCreateInfoKHR& operator=(VkDisplayModeCreateInfoKHR const & rhs)
    {
      m_displayModeCreateInfoKHR = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_displayModeCreateInfoKHR.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_displayModeCreateInfoKHR.sType );
    }

    DisplayModeCreateInfoKHR& sType( StructureType sType )
    {
      m_displayModeCreateInfoKHR.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_displayModeCreateInfoKHR.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_displayModeCreateInfoKHR.pNext );
    }

    DisplayModeCreateInfoKHR& pNext( const void* pNext )
    {
      m_displayModeCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const DisplayModeCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const DisplayModeCreateFlagsKHR&>( m_displayModeCreateInfoKHR.flags );
    }

    DisplayModeCreateFlagsKHR& flags()
    {
      return reinterpret_cast<DisplayModeCreateFlagsKHR&>( m_displayModeCreateInfoKHR.flags );
    }

    DisplayModeCreateInfoKHR& flags( DisplayModeCreateFlagsKHR flags )
    {
      m_displayModeCreateInfoKHR.flags = static_cast<VkDisplayModeCreateFlagsKHR>( flags );
      return *this;
    }

    const DisplayModeParametersKHR& parameters() const
    {
      return reinterpret_cast<const DisplayModeParametersKHR&>( m_displayModeCreateInfoKHR.parameters );
    }

    DisplayModeParametersKHR& parameters()
    {
      return reinterpret_cast<DisplayModeParametersKHR&>( m_displayModeCreateInfoKHR.parameters );
    }

    DisplayModeCreateInfoKHR& parameters( DisplayModeParametersKHR parameters )
    {
      m_displayModeCreateInfoKHR.parameters = static_cast<VkDisplayModeParametersKHR>( parameters );
      return *this;
    }

    static DisplayModeCreateInfoKHR& null()
    {
      return *((DisplayModeCreateInfoKHR*)(nullptr));
    }

    operator const VkDisplayModeCreateInfoKHR&() const
    {
      return m_displayModeCreateInfoKHR;
    }

  private:
    VkDisplayModeCreateInfoKHR m_displayModeCreateInfoKHR;
  };
  static_assert( sizeof( DisplayModeCreateInfoKHR ) == sizeof( VkDisplayModeCreateInfoKHR ), "struct and wrapper have different size!" );

  class DisplayPresentInfoKHR
  {
  public:
    DisplayPresentInfoKHR()
      : DisplayPresentInfoKHR( Rect2D(), Rect2D(), 0 )
    {}

    DisplayPresentInfoKHR( Rect2D srcRect, Rect2D dstRect, Bool32 persistent)
    {
      m_displayPresentInfoKHR.sType = VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR;
      m_displayPresentInfoKHR.pNext = nullptr;
      m_displayPresentInfoKHR.srcRect = static_cast<VkRect2D>( srcRect );
      m_displayPresentInfoKHR.dstRect = static_cast<VkRect2D>( dstRect );
      m_displayPresentInfoKHR.persistent = persistent;
    }

    DisplayPresentInfoKHR(VkDisplayPresentInfoKHR const & rhs)
      : m_displayPresentInfoKHR(rhs)
    {
    }

    DisplayPresentInfoKHR& operator=(VkDisplayPresentInfoKHR const & rhs)
    {
      m_displayPresentInfoKHR = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_displayPresentInfoKHR.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_displayPresentInfoKHR.sType );
    }

    DisplayPresentInfoKHR& sType( StructureType sType )
    {
      m_displayPresentInfoKHR.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_displayPresentInfoKHR.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_displayPresentInfoKHR.pNext );
    }

    DisplayPresentInfoKHR& pNext( const void* pNext )
    {
      m_displayPresentInfoKHR.pNext = pNext;
      return *this;
    }

    const Rect2D& srcRect() const
    {
      return reinterpret_cast<const Rect2D&>( m_displayPresentInfoKHR.srcRect );
    }

    Rect2D& srcRect()
    {
      return reinterpret_cast<Rect2D&>( m_displayPresentInfoKHR.srcRect );
    }

    DisplayPresentInfoKHR& srcRect( Rect2D srcRect )
    {
      m_displayPresentInfoKHR.srcRect = static_cast<VkRect2D>( srcRect );
      return *this;
    }

    const Rect2D& dstRect() const
    {
      return reinterpret_cast<const Rect2D&>( m_displayPresentInfoKHR.dstRect );
    }

    Rect2D& dstRect()
    {
      return reinterpret_cast<Rect2D&>( m_displayPresentInfoKHR.dstRect );
    }

    DisplayPresentInfoKHR& dstRect( Rect2D dstRect )
    {
      m_displayPresentInfoKHR.dstRect = static_cast<VkRect2D>( dstRect );
      return *this;
    }

    const Bool32& persistent() const
    {
      return m_displayPresentInfoKHR.persistent;
    }

    Bool32& persistent()
    {
      return m_displayPresentInfoKHR.persistent;
    }

    DisplayPresentInfoKHR& persistent( Bool32 persistent )
    {
      m_displayPresentInfoKHR.persistent = persistent;
      return *this;
    }

    static DisplayPresentInfoKHR& null()
    {
      return *((DisplayPresentInfoKHR*)(nullptr));
    }

    operator const VkDisplayPresentInfoKHR&() const
    {
      return m_displayPresentInfoKHR;
    }

  private:
    VkDisplayPresentInfoKHR m_displayPresentInfoKHR;
  };
  static_assert( sizeof( DisplayPresentInfoKHR ) == sizeof( VkDisplayPresentInfoKHR ), "struct and wrapper have different size!" );

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  class AndroidSurfaceCreateInfoKHR
  {
  public:
    AndroidSurfaceCreateInfoKHR()
      : AndroidSurfaceCreateInfoKHR( AndroidSurfaceCreateFlagsKHR(), nullptr )
    {}

    AndroidSurfaceCreateInfoKHR( AndroidSurfaceCreateFlagsKHR flags, ANativeWindow* window)
    {
      m_androidSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
      m_androidSurfaceCreateInfoKHR.pNext = nullptr;
      m_androidSurfaceCreateInfoKHR.flags = static_cast<VkAndroidSurfaceCreateFlagsKHR>( flags );
      m_androidSurfaceCreateInfoKHR.window = window;
    }

    AndroidSurfaceCreateInfoKHR(VkAndroidSurfaceCreateInfoKHR const & rhs)
      : m_androidSurfaceCreateInfoKHR(rhs)
    {
    }

    AndroidSurfaceCreateInfoKHR& operator=(VkAndroidSurfaceCreateInfoKHR const & rhs)
    {
      m_androidSurfaceCreateInfoKHR = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_androidSurfaceCreateInfoKHR.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_androidSurfaceCreateInfoKHR.sType );
    }

    AndroidSurfaceCreateInfoKHR& sType( StructureType sType )
    {
      m_androidSurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_androidSurfaceCreateInfoKHR.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_androidSurfaceCreateInfoKHR.pNext );
    }

    AndroidSurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_androidSurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const AndroidSurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const AndroidSurfaceCreateFlagsKHR&>( m_androidSurfaceCreateInfoKHR.flags );
    }

    AndroidSurfaceCreateFlagsKHR& flags()
    {
      return reinterpret_cast<AndroidSurfaceCreateFlagsKHR&>( m_androidSurfaceCreateInfoKHR.flags );
    }

    AndroidSurfaceCreateInfoKHR& flags( AndroidSurfaceCreateFlagsKHR flags )
    {
      m_androidSurfaceCreateInfoKHR.flags = static_cast<VkAndroidSurfaceCreateFlagsKHR>( flags );
      return *this;
    }

    const ANativeWindow* window() const
    {
      return reinterpret_cast<const ANativeWindow*>( m_androidSurfaceCreateInfoKHR.window );
    }

    ANativeWindow* window()
    {
      return reinterpret_cast<ANativeWindow*>( m_androidSurfaceCreateInfoKHR.window );
    }

    AndroidSurfaceCreateInfoKHR& window( ANativeWindow* window )
    {
      m_androidSurfaceCreateInfoKHR.window = window;
      return *this;
    }

    static AndroidSurfaceCreateInfoKHR& null()
    {
      return *((AndroidSurfaceCreateInfoKHR*)(nullptr));
    }

    operator const VkAndroidSurfaceCreateInfoKHR&() const
    {
      return m_androidSurfaceCreateInfoKHR;
    }

  private:
    VkAndroidSurfaceCreateInfoKHR m_androidSurfaceCreateInfoKHR;
  };
  static_assert( sizeof( AndroidSurfaceCreateInfoKHR ) == sizeof( VkAndroidSurfaceCreateInfoKHR ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#ifdef VK_USE_PLATFORM_MIR_KHR
  class MirSurfaceCreateInfoKHR
  {
  public:
    MirSurfaceCreateInfoKHR()
      : MirSurfaceCreateInfoKHR( MirSurfaceCreateFlagsKHR(), nullptr, nullptr )
    {}

    MirSurfaceCreateInfoKHR( MirSurfaceCreateFlagsKHR flags, MirConnection* connection, MirSurface* mirSurface)
    {
      m_mirSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR;
      m_mirSurfaceCreateInfoKHR.pNext = nullptr;
      m_mirSurfaceCreateInfoKHR.flags = static_cast<VkMirSurfaceCreateFlagsKHR>( flags );
      m_mirSurfaceCreateInfoKHR.connection = connection;
      m_mirSurfaceCreateInfoKHR.mirSurface = mirSurface;
    }

    MirSurfaceCreateInfoKHR(VkMirSurfaceCreateInfoKHR const & rhs)
      : m_mirSurfaceCreateInfoKHR(rhs)
    {
    }

    MirSurfaceCreateInfoKHR& operator=(VkMirSurfaceCreateInfoKHR const & rhs)
    {
      m_mirSurfaceCreateInfoKHR = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_mirSurfaceCreateInfoKHR.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_mirSurfaceCreateInfoKHR.sType );
    }

    MirSurfaceCreateInfoKHR& sType( StructureType sType )
    {
      m_mirSurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_mirSurfaceCreateInfoKHR.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_mirSurfaceCreateInfoKHR.pNext );
    }

    MirSurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_mirSurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const MirSurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const MirSurfaceCreateFlagsKHR&>( m_mirSurfaceCreateInfoKHR.flags );
    }

    MirSurfaceCreateFlagsKHR& flags()
    {
      return reinterpret_cast<MirSurfaceCreateFlagsKHR&>( m_mirSurfaceCreateInfoKHR.flags );
    }

    MirSurfaceCreateInfoKHR& flags( MirSurfaceCreateFlagsKHR flags )
    {
      m_mirSurfaceCreateInfoKHR.flags = static_cast<VkMirSurfaceCreateFlagsKHR>( flags );
      return *this;
    }

    const MirConnection* connection() const
    {
      return reinterpret_cast<const MirConnection*>( m_mirSurfaceCreateInfoKHR.connection );
    }

    MirConnection* connection()
    {
      return reinterpret_cast<MirConnection*>( m_mirSurfaceCreateInfoKHR.connection );
    }

    MirSurfaceCreateInfoKHR& connection( MirConnection* connection )
    {
      m_mirSurfaceCreateInfoKHR.connection = connection;
      return *this;
    }

    const MirSurface* mirSurface() const
    {
      return reinterpret_cast<const MirSurface*>( m_mirSurfaceCreateInfoKHR.mirSurface );
    }

    MirSurface* mirSurface()
    {
      return reinterpret_cast<MirSurface*>( m_mirSurfaceCreateInfoKHR.mirSurface );
    }

    MirSurfaceCreateInfoKHR& mirSurface( MirSurface* mirSurface )
    {
      m_mirSurfaceCreateInfoKHR.mirSurface = mirSurface;
      return *this;
    }

    static MirSurfaceCreateInfoKHR& null()
    {
      return *((MirSurfaceCreateInfoKHR*)(nullptr));
    }

    operator const VkMirSurfaceCreateInfoKHR&() const
    {
      return m_mirSurfaceCreateInfoKHR;
    }

  private:
    VkMirSurfaceCreateInfoKHR m_mirSurfaceCreateInfoKHR;
  };
  static_assert( sizeof( MirSurfaceCreateInfoKHR ) == sizeof( VkMirSurfaceCreateInfoKHR ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_MIR_KHR*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  class WaylandSurfaceCreateInfoKHR
  {
  public:
    WaylandSurfaceCreateInfoKHR()
      : WaylandSurfaceCreateInfoKHR( WaylandSurfaceCreateFlagsKHR(), nullptr, nullptr )
    {}

    WaylandSurfaceCreateInfoKHR( WaylandSurfaceCreateFlagsKHR flags, struct wl_display* display, struct wl_surface* surface)
    {
      m_waylandSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
      m_waylandSurfaceCreateInfoKHR.pNext = nullptr;
      m_waylandSurfaceCreateInfoKHR.flags = static_cast<VkWaylandSurfaceCreateFlagsKHR>( flags );
      m_waylandSurfaceCreateInfoKHR.display = display;
      m_waylandSurfaceCreateInfoKHR.surface = surface;
    }

    WaylandSurfaceCreateInfoKHR(VkWaylandSurfaceCreateInfoKHR const & rhs)
      : m_waylandSurfaceCreateInfoKHR(rhs)
    {
    }

    WaylandSurfaceCreateInfoKHR& operator=(VkWaylandSurfaceCreateInfoKHR const & rhs)
    {
      m_waylandSurfaceCreateInfoKHR = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_waylandSurfaceCreateInfoKHR.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_waylandSurfaceCreateInfoKHR.sType );
    }

    WaylandSurfaceCreateInfoKHR& sType( StructureType sType )
    {
      m_waylandSurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_waylandSurfaceCreateInfoKHR.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_waylandSurfaceCreateInfoKHR.pNext );
    }

    WaylandSurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_waylandSurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const WaylandSurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const WaylandSurfaceCreateFlagsKHR&>( m_waylandSurfaceCreateInfoKHR.flags );
    }

    WaylandSurfaceCreateFlagsKHR& flags()
    {
      return reinterpret_cast<WaylandSurfaceCreateFlagsKHR&>( m_waylandSurfaceCreateInfoKHR.flags );
    }

    WaylandSurfaceCreateInfoKHR& flags( WaylandSurfaceCreateFlagsKHR flags )
    {
      m_waylandSurfaceCreateInfoKHR.flags = static_cast<VkWaylandSurfaceCreateFlagsKHR>( flags );
      return *this;
    }

    const struct wl_display* display() const
    {
      return reinterpret_cast<const struct wl_display*>( m_waylandSurfaceCreateInfoKHR.display );
    }

    struct wl_display* display()
    {
      return reinterpret_cast<struct wl_display*>( m_waylandSurfaceCreateInfoKHR.display );
    }

    WaylandSurfaceCreateInfoKHR& display( struct wl_display* display )
    {
      m_waylandSurfaceCreateInfoKHR.display = display;
      return *this;
    }

    const struct wl_surface* surface() const
    {
      return reinterpret_cast<const struct wl_surface*>( m_waylandSurfaceCreateInfoKHR.surface );
    }

    struct wl_surface* surface()
    {
      return reinterpret_cast<struct wl_surface*>( m_waylandSurfaceCreateInfoKHR.surface );
    }

    WaylandSurfaceCreateInfoKHR& surface( struct wl_surface* surface )
    {
      m_waylandSurfaceCreateInfoKHR.surface = surface;
      return *this;
    }

    static WaylandSurfaceCreateInfoKHR& null()
    {
      return *((WaylandSurfaceCreateInfoKHR*)(nullptr));
    }

    operator const VkWaylandSurfaceCreateInfoKHR&() const
    {
      return m_waylandSurfaceCreateInfoKHR;
    }

  private:
    VkWaylandSurfaceCreateInfoKHR m_waylandSurfaceCreateInfoKHR;
  };
  static_assert( sizeof( WaylandSurfaceCreateInfoKHR ) == sizeof( VkWaylandSurfaceCreateInfoKHR ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
  class Win32SurfaceCreateInfoKHR
  {
  public:
    Win32SurfaceCreateInfoKHR()
      : Win32SurfaceCreateInfoKHR( Win32SurfaceCreateFlagsKHR(), 0, 0 )
    {}

    Win32SurfaceCreateInfoKHR( Win32SurfaceCreateFlagsKHR flags, HINSTANCE hinstance, HWND hwnd)
    {
      m_win32SurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
      m_win32SurfaceCreateInfoKHR.pNext = nullptr;
      m_win32SurfaceCreateInfoKHR.flags = static_cast<VkWin32SurfaceCreateFlagsKHR>( flags );
      m_win32SurfaceCreateInfoKHR.hinstance = hinstance;
      m_win32SurfaceCreateInfoKHR.hwnd = hwnd;
    }

    Win32SurfaceCreateInfoKHR(VkWin32SurfaceCreateInfoKHR const & rhs)
      : m_win32SurfaceCreateInfoKHR(rhs)
    {
    }

    Win32SurfaceCreateInfoKHR& operator=(VkWin32SurfaceCreateInfoKHR const & rhs)
    {
      m_win32SurfaceCreateInfoKHR = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_win32SurfaceCreateInfoKHR.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_win32SurfaceCreateInfoKHR.sType );
    }

    Win32SurfaceCreateInfoKHR& sType( StructureType sType )
    {
      m_win32SurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_win32SurfaceCreateInfoKHR.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_win32SurfaceCreateInfoKHR.pNext );
    }

    Win32SurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_win32SurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const Win32SurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const Win32SurfaceCreateFlagsKHR&>( m_win32SurfaceCreateInfoKHR.flags );
    }

    Win32SurfaceCreateFlagsKHR& flags()
    {
      return reinterpret_cast<Win32SurfaceCreateFlagsKHR&>( m_win32SurfaceCreateInfoKHR.flags );
    }

    Win32SurfaceCreateInfoKHR& flags( Win32SurfaceCreateFlagsKHR flags )
    {
      m_win32SurfaceCreateInfoKHR.flags = static_cast<VkWin32SurfaceCreateFlagsKHR>( flags );
      return *this;
    }

    const HINSTANCE& hinstance() const
    {
      return m_win32SurfaceCreateInfoKHR.hinstance;
    }

    HINSTANCE& hinstance()
    {
      return m_win32SurfaceCreateInfoKHR.hinstance;
    }

    Win32SurfaceCreateInfoKHR& hinstance( HINSTANCE hinstance )
    {
      m_win32SurfaceCreateInfoKHR.hinstance = hinstance;
      return *this;
    }

    const HWND& hwnd() const
    {
      return m_win32SurfaceCreateInfoKHR.hwnd;
    }

    HWND& hwnd()
    {
      return m_win32SurfaceCreateInfoKHR.hwnd;
    }

    Win32SurfaceCreateInfoKHR& hwnd( HWND hwnd )
    {
      m_win32SurfaceCreateInfoKHR.hwnd = hwnd;
      return *this;
    }

    static Win32SurfaceCreateInfoKHR& null()
    {
      return *((Win32SurfaceCreateInfoKHR*)(nullptr));
    }

    operator const VkWin32SurfaceCreateInfoKHR&() const
    {
      return m_win32SurfaceCreateInfoKHR;
    }

  private:
    VkWin32SurfaceCreateInfoKHR m_win32SurfaceCreateInfoKHR;
  };
  static_assert( sizeof( Win32SurfaceCreateInfoKHR ) == sizeof( VkWin32SurfaceCreateInfoKHR ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
  class XlibSurfaceCreateInfoKHR
  {
  public:
    XlibSurfaceCreateInfoKHR()
      : XlibSurfaceCreateInfoKHR( XlibSurfaceCreateFlagsKHR(), nullptr, 0 )
    {}

    XlibSurfaceCreateInfoKHR( XlibSurfaceCreateFlagsKHR flags, Display* dpy, Window window)
    {
      m_xlibSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
      m_xlibSurfaceCreateInfoKHR.pNext = nullptr;
      m_xlibSurfaceCreateInfoKHR.flags = static_cast<VkXlibSurfaceCreateFlagsKHR>( flags );
      m_xlibSurfaceCreateInfoKHR.dpy = dpy;
      m_xlibSurfaceCreateInfoKHR.window = window;
    }

    XlibSurfaceCreateInfoKHR(VkXlibSurfaceCreateInfoKHR const & rhs)
      : m_xlibSurfaceCreateInfoKHR(rhs)
    {
    }

    XlibSurfaceCreateInfoKHR& operator=(VkXlibSurfaceCreateInfoKHR const & rhs)
    {
      m_xlibSurfaceCreateInfoKHR = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_xlibSurfaceCreateInfoKHR.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_xlibSurfaceCreateInfoKHR.sType );
    }

    XlibSurfaceCreateInfoKHR& sType( StructureType sType )
    {
      m_xlibSurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_xlibSurfaceCreateInfoKHR.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_xlibSurfaceCreateInfoKHR.pNext );
    }

    XlibSurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_xlibSurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const XlibSurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const XlibSurfaceCreateFlagsKHR&>( m_xlibSurfaceCreateInfoKHR.flags );
    }

    XlibSurfaceCreateFlagsKHR& flags()
    {
      return reinterpret_cast<XlibSurfaceCreateFlagsKHR&>( m_xlibSurfaceCreateInfoKHR.flags );
    }

    XlibSurfaceCreateInfoKHR& flags( XlibSurfaceCreateFlagsKHR flags )
    {
      m_xlibSurfaceCreateInfoKHR.flags = static_cast<VkXlibSurfaceCreateFlagsKHR>( flags );
      return *this;
    }

    const Display* dpy() const
    {
      return reinterpret_cast<const Display*>( m_xlibSurfaceCreateInfoKHR.dpy );
    }

    Display* dpy()
    {
      return reinterpret_cast<Display*>( m_xlibSurfaceCreateInfoKHR.dpy );
    }

    XlibSurfaceCreateInfoKHR& dpy( Display* dpy )
    {
      m_xlibSurfaceCreateInfoKHR.dpy = dpy;
      return *this;
    }

    const Window& window() const
    {
      return m_xlibSurfaceCreateInfoKHR.window;
    }

    Window& window()
    {
      return m_xlibSurfaceCreateInfoKHR.window;
    }

    XlibSurfaceCreateInfoKHR& window( Window window )
    {
      m_xlibSurfaceCreateInfoKHR.window = window;
      return *this;
    }

    static XlibSurfaceCreateInfoKHR& null()
    {
      return *((XlibSurfaceCreateInfoKHR*)(nullptr));
    }

    operator const VkXlibSurfaceCreateInfoKHR&() const
    {
      return m_xlibSurfaceCreateInfoKHR;
    }

  private:
    VkXlibSurfaceCreateInfoKHR m_xlibSurfaceCreateInfoKHR;
  };
  static_assert( sizeof( XlibSurfaceCreateInfoKHR ) == sizeof( VkXlibSurfaceCreateInfoKHR ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
  class XcbSurfaceCreateInfoKHR
  {
  public:
    XcbSurfaceCreateInfoKHR()
      : XcbSurfaceCreateInfoKHR( XcbSurfaceCreateFlagsKHR(), nullptr, 0 )
    {}

    XcbSurfaceCreateInfoKHR( XcbSurfaceCreateFlagsKHR flags, xcb_connection_t* connection, xcb_window_t window)
    {
      m_xcbSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
      m_xcbSurfaceCreateInfoKHR.pNext = nullptr;
      m_xcbSurfaceCreateInfoKHR.flags = static_cast<VkXcbSurfaceCreateFlagsKHR>( flags );
      m_xcbSurfaceCreateInfoKHR.connection = connection;
      m_xcbSurfaceCreateInfoKHR.window = window;
    }

    XcbSurfaceCreateInfoKHR(VkXcbSurfaceCreateInfoKHR const & rhs)
      : m_xcbSurfaceCreateInfoKHR(rhs)
    {
    }

    XcbSurfaceCreateInfoKHR& operator=(VkXcbSurfaceCreateInfoKHR const & rhs)
    {
      m_xcbSurfaceCreateInfoKHR = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_xcbSurfaceCreateInfoKHR.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_xcbSurfaceCreateInfoKHR.sType );
    }

    XcbSurfaceCreateInfoKHR& sType( StructureType sType )
    {
      m_xcbSurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_xcbSurfaceCreateInfoKHR.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_xcbSurfaceCreateInfoKHR.pNext );
    }

    XcbSurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_xcbSurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const XcbSurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const XcbSurfaceCreateFlagsKHR&>( m_xcbSurfaceCreateInfoKHR.flags );
    }

    XcbSurfaceCreateFlagsKHR& flags()
    {
      return reinterpret_cast<XcbSurfaceCreateFlagsKHR&>( m_xcbSurfaceCreateInfoKHR.flags );
    }

    XcbSurfaceCreateInfoKHR& flags( XcbSurfaceCreateFlagsKHR flags )
    {
      m_xcbSurfaceCreateInfoKHR.flags = static_cast<VkXcbSurfaceCreateFlagsKHR>( flags );
      return *this;
    }

    const xcb_connection_t* connection() const
    {
      return reinterpret_cast<const xcb_connection_t*>( m_xcbSurfaceCreateInfoKHR.connection );
    }

    xcb_connection_t* connection()
    {
      return reinterpret_cast<xcb_connection_t*>( m_xcbSurfaceCreateInfoKHR.connection );
    }

    XcbSurfaceCreateInfoKHR& connection( xcb_connection_t* connection )
    {
      m_xcbSurfaceCreateInfoKHR.connection = connection;
      return *this;
    }

    const xcb_window_t& window() const
    {
      return m_xcbSurfaceCreateInfoKHR.window;
    }

    xcb_window_t& window()
    {
      return m_xcbSurfaceCreateInfoKHR.window;
    }

    XcbSurfaceCreateInfoKHR& window( xcb_window_t window )
    {
      m_xcbSurfaceCreateInfoKHR.window = window;
      return *this;
    }

    static XcbSurfaceCreateInfoKHR& null()
    {
      return *((XcbSurfaceCreateInfoKHR*)(nullptr));
    }

    operator const VkXcbSurfaceCreateInfoKHR&() const
    {
      return m_xcbSurfaceCreateInfoKHR;
    }

  private:
    VkXcbSurfaceCreateInfoKHR m_xcbSurfaceCreateInfoKHR;
  };
  static_assert( sizeof( XcbSurfaceCreateInfoKHR ) == sizeof( VkXcbSurfaceCreateInfoKHR ), "struct and wrapper have different size!" );
#endif /*VK_USE_PLATFORM_XCB_KHR*/

  enum class SubpassContents
  {
    eInline = VK_SUBPASS_CONTENTS_INLINE,
    eSecondaryCommandBuffers = VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS
  };

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
    eErrorSurfaceLostKHR = VK_ERROR_SURFACE_LOST_KHR,
    eErrorNativeWindowInUseKHR = VK_ERROR_NATIVE_WINDOW_IN_USE_KHR,
    eSuboptimalKHR = VK_SUBOPTIMAL_KHR,
    eErrorOutOfDateKHR = VK_ERROR_OUT_OF_DATE_KHR,
    eErrorIncompatibleDisplayKHR = VK_ERROR_INCOMPATIBLE_DISPLAY_KHR,
    eErrorValidationFailedEXT = VK_ERROR_VALIDATION_FAILED_EXT
  };

  class PresentInfoKHR
  {
  public:
    PresentInfoKHR()
      : PresentInfoKHR( 0, nullptr, 0, nullptr, nullptr, nullptr )
    {}

    PresentInfoKHR( uint32_t waitSemaphoreCount, const Semaphore* pWaitSemaphores, uint32_t swapchainCount, const SwapchainKHR* pSwapchains, const uint32_t* pImageIndices, Result* pResults)
    {
      m_presentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
      m_presentInfoKHR.pNext = nullptr;
      m_presentInfoKHR.waitSemaphoreCount = waitSemaphoreCount;
      m_presentInfoKHR.pWaitSemaphores = reinterpret_cast<const VkSemaphore*>( pWaitSemaphores );
      m_presentInfoKHR.swapchainCount = swapchainCount;
      m_presentInfoKHR.pSwapchains = reinterpret_cast<const VkSwapchainKHR*>( pSwapchains );
      m_presentInfoKHR.pImageIndices = pImageIndices;
      m_presentInfoKHR.pResults = reinterpret_cast<VkResult*>( pResults );
    }

    PresentInfoKHR(VkPresentInfoKHR const & rhs)
      : m_presentInfoKHR(rhs)
    {
    }

    PresentInfoKHR& operator=(VkPresentInfoKHR const & rhs)
    {
      m_presentInfoKHR = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_presentInfoKHR.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_presentInfoKHR.sType );
    }

    PresentInfoKHR& sType( StructureType sType )
    {
      m_presentInfoKHR.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_presentInfoKHR.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_presentInfoKHR.pNext );
    }

    PresentInfoKHR& pNext( const void* pNext )
    {
      m_presentInfoKHR.pNext = pNext;
      return *this;
    }

    const uint32_t& waitSemaphoreCount() const
    {
      return m_presentInfoKHR.waitSemaphoreCount;
    }

    uint32_t& waitSemaphoreCount()
    {
      return m_presentInfoKHR.waitSemaphoreCount;
    }

    PresentInfoKHR& waitSemaphoreCount( uint32_t waitSemaphoreCount )
    {
      m_presentInfoKHR.waitSemaphoreCount = waitSemaphoreCount;
      return *this;
    }

    const Semaphore* pWaitSemaphores() const
    {
      return reinterpret_cast<const Semaphore*>( m_presentInfoKHR.pWaitSemaphores );
    }

    const Semaphore* pWaitSemaphores()
    {
      return reinterpret_cast<const Semaphore*>( m_presentInfoKHR.pWaitSemaphores );
    }

    PresentInfoKHR& pWaitSemaphores( const Semaphore* pWaitSemaphores )
    {
      m_presentInfoKHR.pWaitSemaphores = reinterpret_cast<const VkSemaphore*>( pWaitSemaphores );
      return *this;
    }

    const uint32_t& swapchainCount() const
    {
      return m_presentInfoKHR.swapchainCount;
    }

    uint32_t& swapchainCount()
    {
      return m_presentInfoKHR.swapchainCount;
    }

    PresentInfoKHR& swapchainCount( uint32_t swapchainCount )
    {
      m_presentInfoKHR.swapchainCount = swapchainCount;
      return *this;
    }

    const SwapchainKHR* pSwapchains() const
    {
      return reinterpret_cast<const SwapchainKHR*>( m_presentInfoKHR.pSwapchains );
    }

    const SwapchainKHR* pSwapchains()
    {
      return reinterpret_cast<const SwapchainKHR*>( m_presentInfoKHR.pSwapchains );
    }

    PresentInfoKHR& pSwapchains( const SwapchainKHR* pSwapchains )
    {
      m_presentInfoKHR.pSwapchains = reinterpret_cast<const VkSwapchainKHR*>( pSwapchains );
      return *this;
    }

    const uint32_t* pImageIndices() const
    {
      return reinterpret_cast<const uint32_t*>( m_presentInfoKHR.pImageIndices );
    }

    const uint32_t* pImageIndices()
    {
      return reinterpret_cast<const uint32_t*>( m_presentInfoKHR.pImageIndices );
    }

    PresentInfoKHR& pImageIndices( const uint32_t* pImageIndices )
    {
      m_presentInfoKHR.pImageIndices = pImageIndices;
      return *this;
    }

    const Result* pResults() const
    {
      return reinterpret_cast<const Result*>( m_presentInfoKHR.pResults );
    }

    Result* pResults()
    {
      return reinterpret_cast<Result*>( m_presentInfoKHR.pResults );
    }

    PresentInfoKHR& pResults( Result* pResults )
    {
      m_presentInfoKHR.pResults = reinterpret_cast<VkResult*>( pResults );
      return *this;
    }

    static PresentInfoKHR& null()
    {
      return *((PresentInfoKHR*)(nullptr));
    }

    operator const VkPresentInfoKHR&() const
    {
      return m_presentInfoKHR;
    }

  private:
    VkPresentInfoKHR m_presentInfoKHR;
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

  class PipelineDynamicStateCreateInfo
  {
  public:
    PipelineDynamicStateCreateInfo()
      : PipelineDynamicStateCreateInfo( PipelineDynamicStateCreateFlags(), 0, nullptr )
    {}

    PipelineDynamicStateCreateInfo( PipelineDynamicStateCreateFlags flags, uint32_t dynamicStateCount, const DynamicState* pDynamicStates)
    {
      m_pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
      m_pipelineDynamicStateCreateInfo.pNext = nullptr;
      m_pipelineDynamicStateCreateInfo.flags = static_cast<VkPipelineDynamicStateCreateFlags>( flags );
      m_pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStateCount;
      m_pipelineDynamicStateCreateInfo.pDynamicStates = reinterpret_cast<const VkDynamicState*>( pDynamicStates );
    }

    PipelineDynamicStateCreateInfo(VkPipelineDynamicStateCreateInfo const & rhs)
      : m_pipelineDynamicStateCreateInfo(rhs)
    {
    }

    PipelineDynamicStateCreateInfo& operator=(VkPipelineDynamicStateCreateInfo const & rhs)
    {
      m_pipelineDynamicStateCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineDynamicStateCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_pipelineDynamicStateCreateInfo.sType );
    }

    PipelineDynamicStateCreateInfo& sType( StructureType sType )
    {
      m_pipelineDynamicStateCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_pipelineDynamicStateCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_pipelineDynamicStateCreateInfo.pNext );
    }

    PipelineDynamicStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineDynamicStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineDynamicStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineDynamicStateCreateFlags&>( m_pipelineDynamicStateCreateInfo.flags );
    }

    PipelineDynamicStateCreateFlags& flags()
    {
      return reinterpret_cast<PipelineDynamicStateCreateFlags&>( m_pipelineDynamicStateCreateInfo.flags );
    }

    PipelineDynamicStateCreateInfo& flags( PipelineDynamicStateCreateFlags flags )
    {
      m_pipelineDynamicStateCreateInfo.flags = static_cast<VkPipelineDynamicStateCreateFlags>( flags );
      return *this;
    }

    const uint32_t& dynamicStateCount() const
    {
      return m_pipelineDynamicStateCreateInfo.dynamicStateCount;
    }

    uint32_t& dynamicStateCount()
    {
      return m_pipelineDynamicStateCreateInfo.dynamicStateCount;
    }

    PipelineDynamicStateCreateInfo& dynamicStateCount( uint32_t dynamicStateCount )
    {
      m_pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStateCount;
      return *this;
    }

    const DynamicState* pDynamicStates() const
    {
      return reinterpret_cast<const DynamicState*>( m_pipelineDynamicStateCreateInfo.pDynamicStates );
    }

    const DynamicState* pDynamicStates()
    {
      return reinterpret_cast<const DynamicState*>( m_pipelineDynamicStateCreateInfo.pDynamicStates );
    }

    PipelineDynamicStateCreateInfo& pDynamicStates( const DynamicState* pDynamicStates )
    {
      m_pipelineDynamicStateCreateInfo.pDynamicStates = reinterpret_cast<const VkDynamicState*>( pDynamicStates );
      return *this;
    }

    static PipelineDynamicStateCreateInfo& null()
    {
      return *((PipelineDynamicStateCreateInfo*)(nullptr));
    }

    operator const VkPipelineDynamicStateCreateInfo&() const
    {
      return m_pipelineDynamicStateCreateInfo;
    }

  private:
    VkPipelineDynamicStateCreateInfo m_pipelineDynamicStateCreateInfo;
  };
  static_assert( sizeof( PipelineDynamicStateCreateInfo ) == sizeof( VkPipelineDynamicStateCreateInfo ), "struct and wrapper have different size!" );

  enum class QueueFlagBits
  {
    eGraphics = VK_QUEUE_GRAPHICS_BIT,
    eCompute = VK_QUEUE_COMPUTE_BIT,
    eTransfer = VK_QUEUE_TRANSFER_BIT,
    eSparseBinding = VK_QUEUE_SPARSE_BINDING_BIT
  };

  typedef Flags<QueueFlagBits, VkQueueFlags> QueueFlags;

  inline QueueFlags operator|( QueueFlagBits bit0, QueueFlagBits bit1 )
  {
    return QueueFlags( bit0 ) | bit1;
  }

  class QueueFamilyProperties
  {
  public:
    const QueueFlags& queueFlags() const
    {
      return reinterpret_cast<const QueueFlags&>( m_queueFamilyProperties.queueFlags );
    }

    const uint32_t& queueCount() const
    {
      return m_queueFamilyProperties.queueCount;
    }

    const uint32_t& timestampValidBits() const
    {
      return m_queueFamilyProperties.timestampValidBits;
    }

    const Extent3D& minImageTransferGranularity() const
    {
      return reinterpret_cast<const Extent3D&>( m_queueFamilyProperties.minImageTransferGranularity );
    }

    static QueueFamilyProperties& null()
    {
      return *((QueueFamilyProperties*)(nullptr));
    }

    operator const VkQueueFamilyProperties&() const
    {
      return m_queueFamilyProperties;
    }

  private:
    VkQueueFamilyProperties m_queueFamilyProperties;
  };
  static_assert( sizeof( QueueFamilyProperties ) == sizeof( VkQueueFamilyProperties ), "struct and wrapper have different size!" );

  enum class MemoryPropertyFlagBits
  {
    eDeviceLocal = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    eHostVisible = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
    eHostCoherent = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    eHostCached = VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
    eLazilyAllocated = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT
  };

  typedef Flags<MemoryPropertyFlagBits, VkMemoryPropertyFlags> MemoryPropertyFlags;

  inline MemoryPropertyFlags operator|( MemoryPropertyFlagBits bit0, MemoryPropertyFlagBits bit1 )
  {
    return MemoryPropertyFlags( bit0 ) | bit1;
  }

  class MemoryType
  {
  public:
    const MemoryPropertyFlags& propertyFlags() const
    {
      return reinterpret_cast<const MemoryPropertyFlags&>( m_memoryType.propertyFlags );
    }

    const uint32_t& heapIndex() const
    {
      return m_memoryType.heapIndex;
    }

    static MemoryType& null()
    {
      return *((MemoryType*)(nullptr));
    }

    operator const VkMemoryType&() const
    {
      return m_memoryType;
    }

  private:
    VkMemoryType m_memoryType;
  };
  static_assert( sizeof( MemoryType ) == sizeof( VkMemoryType ), "struct and wrapper have different size!" );

  enum class MemoryHeapFlagBits
  {
    eDeviceLocal = VK_MEMORY_HEAP_DEVICE_LOCAL_BIT
  };

  typedef Flags<MemoryHeapFlagBits, VkMemoryHeapFlags> MemoryHeapFlags;

  inline MemoryHeapFlags operator|( MemoryHeapFlagBits bit0, MemoryHeapFlagBits bit1 )
  {
    return MemoryHeapFlags( bit0 ) | bit1;
  }

  class MemoryHeap
  {
  public:
    const DeviceSize& size() const
    {
      return m_memoryHeap.size;
    }

    const MemoryHeapFlags& flags() const
    {
      return reinterpret_cast<const MemoryHeapFlags&>( m_memoryHeap.flags );
    }

    static MemoryHeap& null()
    {
      return *((MemoryHeap*)(nullptr));
    }

    operator const VkMemoryHeap&() const
    {
      return m_memoryHeap;
    }

  private:
    VkMemoryHeap m_memoryHeap;
  };
  static_assert( sizeof( MemoryHeap ) == sizeof( VkMemoryHeap ), "struct and wrapper have different size!" );

  class PhysicalDeviceMemoryProperties
  {
  public:
    const uint32_t& memoryTypeCount() const
    {
      return m_physicalDeviceMemoryProperties.memoryTypeCount;
    }

    const MemoryType* memoryTypes() const
    {
      return reinterpret_cast<const MemoryType*>( m_physicalDeviceMemoryProperties.memoryTypes );
    }

    const uint32_t& memoryHeapCount() const
    {
      return m_physicalDeviceMemoryProperties.memoryHeapCount;
    }

    const MemoryHeap* memoryHeaps() const
    {
      return reinterpret_cast<const MemoryHeap*>( m_physicalDeviceMemoryProperties.memoryHeaps );
    }

    static PhysicalDeviceMemoryProperties& null()
    {
      return *((PhysicalDeviceMemoryProperties*)(nullptr));
    }

    operator const VkPhysicalDeviceMemoryProperties&() const
    {
      return m_physicalDeviceMemoryProperties;
    }

  private:
    VkPhysicalDeviceMemoryProperties m_physicalDeviceMemoryProperties;
  };
  static_assert( sizeof( PhysicalDeviceMemoryProperties ) == sizeof( VkPhysicalDeviceMemoryProperties ), "struct and wrapper have different size!" );

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
    eMemoryWrite = VK_ACCESS_MEMORY_WRITE_BIT
  };

  typedef Flags<AccessFlagBits, VkAccessFlags> AccessFlags;

  inline AccessFlags operator|( AccessFlagBits bit0, AccessFlagBits bit1 )
  {
    return AccessFlags( bit0 ) | bit1;
  }

  class MemoryBarrier
  {
  public:
    MemoryBarrier()
      : MemoryBarrier( AccessFlags(), AccessFlags() )
    {}

    MemoryBarrier( AccessFlags srcAccessMask, AccessFlags dstAccessMask)
    {
      m_memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
      m_memoryBarrier.pNext = nullptr;
      m_memoryBarrier.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask );
      m_memoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask );
    }

    MemoryBarrier(VkMemoryBarrier const & rhs)
      : m_memoryBarrier(rhs)
    {
    }

    MemoryBarrier& operator=(VkMemoryBarrier const & rhs)
    {
      m_memoryBarrier = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_memoryBarrier.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_memoryBarrier.sType );
    }

    MemoryBarrier& sType( StructureType sType )
    {
      m_memoryBarrier.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_memoryBarrier.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_memoryBarrier.pNext );
    }

    MemoryBarrier& pNext( const void* pNext )
    {
      m_memoryBarrier.pNext = pNext;
      return *this;
    }

    const AccessFlags& srcAccessMask() const
    {
      return reinterpret_cast<const AccessFlags&>( m_memoryBarrier.srcAccessMask );
    }

    AccessFlags& srcAccessMask()
    {
      return reinterpret_cast<AccessFlags&>( m_memoryBarrier.srcAccessMask );
    }

    MemoryBarrier& srcAccessMask( AccessFlags srcAccessMask )
    {
      m_memoryBarrier.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask );
      return *this;
    }

    const AccessFlags& dstAccessMask() const
    {
      return reinterpret_cast<const AccessFlags&>( m_memoryBarrier.dstAccessMask );
    }

    AccessFlags& dstAccessMask()
    {
      return reinterpret_cast<AccessFlags&>( m_memoryBarrier.dstAccessMask );
    }

    MemoryBarrier& dstAccessMask( AccessFlags dstAccessMask )
    {
      m_memoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask );
      return *this;
    }

    static MemoryBarrier& null()
    {
      return *((MemoryBarrier*)(nullptr));
    }

    operator const VkMemoryBarrier&() const
    {
      return m_memoryBarrier;
    }

  private:
    VkMemoryBarrier m_memoryBarrier;
  };
  static_assert( sizeof( MemoryBarrier ) == sizeof( VkMemoryBarrier ), "struct and wrapper have different size!" );

  class BufferMemoryBarrier
  {
  public:
    BufferMemoryBarrier()
      : BufferMemoryBarrier( AccessFlags(), AccessFlags(), 0, 0, Buffer(), 0, 0 )
    {}

    BufferMemoryBarrier( AccessFlags srcAccessMask, AccessFlags dstAccessMask, uint32_t srcQueueFamilyIndex, uint32_t dstQueueFamilyIndex, Buffer buffer, DeviceSize offset, DeviceSize size)
    {
      m_bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
      m_bufferMemoryBarrier.pNext = nullptr;
      m_bufferMemoryBarrier.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask );
      m_bufferMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask );
      m_bufferMemoryBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex;
      m_bufferMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
      m_bufferMemoryBarrier.buffer = static_cast<VkBuffer>( buffer );
      m_bufferMemoryBarrier.offset = offset;
      m_bufferMemoryBarrier.size = size;
    }

    BufferMemoryBarrier(VkBufferMemoryBarrier const & rhs)
      : m_bufferMemoryBarrier(rhs)
    {
    }

    BufferMemoryBarrier& operator=(VkBufferMemoryBarrier const & rhs)
    {
      m_bufferMemoryBarrier = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_bufferMemoryBarrier.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_bufferMemoryBarrier.sType );
    }

    BufferMemoryBarrier& sType( StructureType sType )
    {
      m_bufferMemoryBarrier.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_bufferMemoryBarrier.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_bufferMemoryBarrier.pNext );
    }

    BufferMemoryBarrier& pNext( const void* pNext )
    {
      m_bufferMemoryBarrier.pNext = pNext;
      return *this;
    }

    const AccessFlags& srcAccessMask() const
    {
      return reinterpret_cast<const AccessFlags&>( m_bufferMemoryBarrier.srcAccessMask );
    }

    AccessFlags& srcAccessMask()
    {
      return reinterpret_cast<AccessFlags&>( m_bufferMemoryBarrier.srcAccessMask );
    }

    BufferMemoryBarrier& srcAccessMask( AccessFlags srcAccessMask )
    {
      m_bufferMemoryBarrier.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask );
      return *this;
    }

    const AccessFlags& dstAccessMask() const
    {
      return reinterpret_cast<const AccessFlags&>( m_bufferMemoryBarrier.dstAccessMask );
    }

    AccessFlags& dstAccessMask()
    {
      return reinterpret_cast<AccessFlags&>( m_bufferMemoryBarrier.dstAccessMask );
    }

    BufferMemoryBarrier& dstAccessMask( AccessFlags dstAccessMask )
    {
      m_bufferMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask );
      return *this;
    }

    const uint32_t& srcQueueFamilyIndex() const
    {
      return m_bufferMemoryBarrier.srcQueueFamilyIndex;
    }

    uint32_t& srcQueueFamilyIndex()
    {
      return m_bufferMemoryBarrier.srcQueueFamilyIndex;
    }

    BufferMemoryBarrier& srcQueueFamilyIndex( uint32_t srcQueueFamilyIndex )
    {
      m_bufferMemoryBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex;
      return *this;
    }

    const uint32_t& dstQueueFamilyIndex() const
    {
      return m_bufferMemoryBarrier.dstQueueFamilyIndex;
    }

    uint32_t& dstQueueFamilyIndex()
    {
      return m_bufferMemoryBarrier.dstQueueFamilyIndex;
    }

    BufferMemoryBarrier& dstQueueFamilyIndex( uint32_t dstQueueFamilyIndex )
    {
      m_bufferMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
      return *this;
    }

    const Buffer& buffer() const
    {
      return reinterpret_cast<const Buffer&>( m_bufferMemoryBarrier.buffer );
    }

    Buffer& buffer()
    {
      return reinterpret_cast<Buffer&>( m_bufferMemoryBarrier.buffer );
    }

    BufferMemoryBarrier& buffer( Buffer buffer )
    {
      m_bufferMemoryBarrier.buffer = static_cast<VkBuffer>( buffer );
      return *this;
    }

    const DeviceSize& offset() const
    {
      return m_bufferMemoryBarrier.offset;
    }

    DeviceSize& offset()
    {
      return m_bufferMemoryBarrier.offset;
    }

    BufferMemoryBarrier& offset( DeviceSize offset )
    {
      m_bufferMemoryBarrier.offset = offset;
      return *this;
    }

    const DeviceSize& size() const
    {
      return m_bufferMemoryBarrier.size;
    }

    DeviceSize& size()
    {
      return m_bufferMemoryBarrier.size;
    }

    BufferMemoryBarrier& size( DeviceSize size )
    {
      m_bufferMemoryBarrier.size = size;
      return *this;
    }

    static BufferMemoryBarrier& null()
    {
      return *((BufferMemoryBarrier*)(nullptr));
    }

    operator const VkBufferMemoryBarrier&() const
    {
      return m_bufferMemoryBarrier;
    }

  private:
    VkBufferMemoryBarrier m_bufferMemoryBarrier;
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

  typedef Flags<BufferUsageFlagBits, VkBufferUsageFlags> BufferUsageFlags;

  inline BufferUsageFlags operator|( BufferUsageFlagBits bit0, BufferUsageFlagBits bit1 )
  {
    return BufferUsageFlags( bit0 ) | bit1;
  }

  enum class BufferCreateFlagBits
  {
    eSparseBinding = VK_BUFFER_CREATE_SPARSE_BINDING_BIT,
    eSparseResidency = VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT,
    eSparseAliased = VK_BUFFER_CREATE_SPARSE_ALIASED_BIT
  };

  typedef Flags<BufferCreateFlagBits, VkBufferCreateFlags> BufferCreateFlags;

  inline BufferCreateFlags operator|( BufferCreateFlagBits bit0, BufferCreateFlagBits bit1 )
  {
    return BufferCreateFlags( bit0 ) | bit1;
  }

  class BufferCreateInfo
  {
  public:
    BufferCreateInfo()
      : BufferCreateInfo( BufferCreateFlags(), 0, BufferUsageFlags(), SharingMode::eExclusive, 0, nullptr )
    {}

    BufferCreateInfo( BufferCreateFlags flags, DeviceSize size, BufferUsageFlags usage, SharingMode sharingMode, uint32_t queueFamilyIndexCount, const uint32_t* pQueueFamilyIndices)
    {
      m_bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      m_bufferCreateInfo.pNext = nullptr;
      m_bufferCreateInfo.flags = static_cast<VkBufferCreateFlags>( flags );
      m_bufferCreateInfo.size = size;
      m_bufferCreateInfo.usage = static_cast<VkBufferUsageFlags>( usage );
      m_bufferCreateInfo.sharingMode = static_cast<VkSharingMode>( sharingMode );
      m_bufferCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount;
      m_bufferCreateInfo.pQueueFamilyIndices = pQueueFamilyIndices;
    }

    BufferCreateInfo(VkBufferCreateInfo const & rhs)
      : m_bufferCreateInfo(rhs)
    {
    }

    BufferCreateInfo& operator=(VkBufferCreateInfo const & rhs)
    {
      m_bufferCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_bufferCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_bufferCreateInfo.sType );
    }

    BufferCreateInfo& sType( StructureType sType )
    {
      m_bufferCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_bufferCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_bufferCreateInfo.pNext );
    }

    BufferCreateInfo& pNext( const void* pNext )
    {
      m_bufferCreateInfo.pNext = pNext;
      return *this;
    }

    const BufferCreateFlags& flags() const
    {
      return reinterpret_cast<const BufferCreateFlags&>( m_bufferCreateInfo.flags );
    }

    BufferCreateFlags& flags()
    {
      return reinterpret_cast<BufferCreateFlags&>( m_bufferCreateInfo.flags );
    }

    BufferCreateInfo& flags( BufferCreateFlags flags )
    {
      m_bufferCreateInfo.flags = static_cast<VkBufferCreateFlags>( flags );
      return *this;
    }

    const DeviceSize& size() const
    {
      return m_bufferCreateInfo.size;
    }

    DeviceSize& size()
    {
      return m_bufferCreateInfo.size;
    }

    BufferCreateInfo& size( DeviceSize size )
    {
      m_bufferCreateInfo.size = size;
      return *this;
    }

    const BufferUsageFlags& usage() const
    {
      return reinterpret_cast<const BufferUsageFlags&>( m_bufferCreateInfo.usage );
    }

    BufferUsageFlags& usage()
    {
      return reinterpret_cast<BufferUsageFlags&>( m_bufferCreateInfo.usage );
    }

    BufferCreateInfo& usage( BufferUsageFlags usage )
    {
      m_bufferCreateInfo.usage = static_cast<VkBufferUsageFlags>( usage );
      return *this;
    }

    const SharingMode& sharingMode() const
    {
      return reinterpret_cast<const SharingMode&>( m_bufferCreateInfo.sharingMode );
    }

    SharingMode& sharingMode()
    {
      return reinterpret_cast<SharingMode&>( m_bufferCreateInfo.sharingMode );
    }

    BufferCreateInfo& sharingMode( SharingMode sharingMode )
    {
      m_bufferCreateInfo.sharingMode = static_cast<VkSharingMode>( sharingMode );
      return *this;
    }

    const uint32_t& queueFamilyIndexCount() const
    {
      return m_bufferCreateInfo.queueFamilyIndexCount;
    }

    uint32_t& queueFamilyIndexCount()
    {
      return m_bufferCreateInfo.queueFamilyIndexCount;
    }

    BufferCreateInfo& queueFamilyIndexCount( uint32_t queueFamilyIndexCount )
    {
      m_bufferCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount;
      return *this;
    }

    const uint32_t* pQueueFamilyIndices() const
    {
      return reinterpret_cast<const uint32_t*>( m_bufferCreateInfo.pQueueFamilyIndices );
    }

    const uint32_t* pQueueFamilyIndices()
    {
      return reinterpret_cast<const uint32_t*>( m_bufferCreateInfo.pQueueFamilyIndices );
    }

    BufferCreateInfo& pQueueFamilyIndices( const uint32_t* pQueueFamilyIndices )
    {
      m_bufferCreateInfo.pQueueFamilyIndices = pQueueFamilyIndices;
      return *this;
    }

    static BufferCreateInfo& null()
    {
      return *((BufferCreateInfo*)(nullptr));
    }

    operator const VkBufferCreateInfo&() const
    {
      return m_bufferCreateInfo;
    }

  private:
    VkBufferCreateInfo m_bufferCreateInfo;
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

  typedef Flags<ShaderStageFlagBits, VkShaderStageFlags> ShaderStageFlags;

  inline ShaderStageFlags operator|( ShaderStageFlagBits bit0, ShaderStageFlagBits bit1 )
  {
    return ShaderStageFlags( bit0 ) | bit1;
  }

  class DescriptorSetLayoutBinding
  {
  public:
    DescriptorSetLayoutBinding()
      : DescriptorSetLayoutBinding( 0, DescriptorType::eSampler, 0, ShaderStageFlags(), nullptr )
    {}

    DescriptorSetLayoutBinding( uint32_t binding, DescriptorType descriptorType, uint32_t descriptorCount, ShaderStageFlags stageFlags, const Sampler* pImmutableSamplers)
    {
      m_descriptorSetLayoutBinding.binding = binding;
      m_descriptorSetLayoutBinding.descriptorType = static_cast<VkDescriptorType>( descriptorType );
      m_descriptorSetLayoutBinding.descriptorCount = descriptorCount;
      m_descriptorSetLayoutBinding.stageFlags = static_cast<VkShaderStageFlags>( stageFlags );
      m_descriptorSetLayoutBinding.pImmutableSamplers = reinterpret_cast<const VkSampler*>( pImmutableSamplers );
    }

    DescriptorSetLayoutBinding(VkDescriptorSetLayoutBinding const & rhs)
      : m_descriptorSetLayoutBinding(rhs)
    {
    }

    DescriptorSetLayoutBinding& operator=(VkDescriptorSetLayoutBinding const & rhs)
    {
      m_descriptorSetLayoutBinding = rhs;
      return *this;
    }

    const uint32_t& binding() const
    {
      return m_descriptorSetLayoutBinding.binding;
    }

    uint32_t& binding()
    {
      return m_descriptorSetLayoutBinding.binding;
    }

    DescriptorSetLayoutBinding& binding( uint32_t binding )
    {
      m_descriptorSetLayoutBinding.binding = binding;
      return *this;
    }

    const DescriptorType& descriptorType() const
    {
      return reinterpret_cast<const DescriptorType&>( m_descriptorSetLayoutBinding.descriptorType );
    }

    DescriptorType& descriptorType()
    {
      return reinterpret_cast<DescriptorType&>( m_descriptorSetLayoutBinding.descriptorType );
    }

    DescriptorSetLayoutBinding& descriptorType( DescriptorType descriptorType )
    {
      m_descriptorSetLayoutBinding.descriptorType = static_cast<VkDescriptorType>( descriptorType );
      return *this;
    }

    const uint32_t& descriptorCount() const
    {
      return m_descriptorSetLayoutBinding.descriptorCount;
    }

    uint32_t& descriptorCount()
    {
      return m_descriptorSetLayoutBinding.descriptorCount;
    }

    DescriptorSetLayoutBinding& descriptorCount( uint32_t descriptorCount )
    {
      m_descriptorSetLayoutBinding.descriptorCount = descriptorCount;
      return *this;
    }

    const ShaderStageFlags& stageFlags() const
    {
      return reinterpret_cast<const ShaderStageFlags&>( m_descriptorSetLayoutBinding.stageFlags );
    }

    ShaderStageFlags& stageFlags()
    {
      return reinterpret_cast<ShaderStageFlags&>( m_descriptorSetLayoutBinding.stageFlags );
    }

    DescriptorSetLayoutBinding& stageFlags( ShaderStageFlags stageFlags )
    {
      m_descriptorSetLayoutBinding.stageFlags = static_cast<VkShaderStageFlags>( stageFlags );
      return *this;
    }

    const Sampler* pImmutableSamplers() const
    {
      return reinterpret_cast<const Sampler*>( m_descriptorSetLayoutBinding.pImmutableSamplers );
    }

    const Sampler* pImmutableSamplers()
    {
      return reinterpret_cast<const Sampler*>( m_descriptorSetLayoutBinding.pImmutableSamplers );
    }

    DescriptorSetLayoutBinding& pImmutableSamplers( const Sampler* pImmutableSamplers )
    {
      m_descriptorSetLayoutBinding.pImmutableSamplers = reinterpret_cast<const VkSampler*>( pImmutableSamplers );
      return *this;
    }

    static DescriptorSetLayoutBinding& null()
    {
      return *((DescriptorSetLayoutBinding*)(nullptr));
    }

    operator const VkDescriptorSetLayoutBinding&() const
    {
      return m_descriptorSetLayoutBinding;
    }

  private:
    VkDescriptorSetLayoutBinding m_descriptorSetLayoutBinding;
  };
  static_assert( sizeof( DescriptorSetLayoutBinding ) == sizeof( VkDescriptorSetLayoutBinding ), "struct and wrapper have different size!" );

  class DescriptorSetLayoutCreateInfo
  {
  public:
    DescriptorSetLayoutCreateInfo()
      : DescriptorSetLayoutCreateInfo( DescriptorSetLayoutCreateFlags(), 0, nullptr )
    {}

    DescriptorSetLayoutCreateInfo( DescriptorSetLayoutCreateFlags flags, uint32_t bindingCount, const DescriptorSetLayoutBinding* pBindings)
    {
      m_descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      m_descriptorSetLayoutCreateInfo.pNext = nullptr;
      m_descriptorSetLayoutCreateInfo.flags = static_cast<VkDescriptorSetLayoutCreateFlags>( flags );
      m_descriptorSetLayoutCreateInfo.bindingCount = bindingCount;
      m_descriptorSetLayoutCreateInfo.pBindings = reinterpret_cast<const VkDescriptorSetLayoutBinding*>( pBindings );
    }

    DescriptorSetLayoutCreateInfo(VkDescriptorSetLayoutCreateInfo const & rhs)
      : m_descriptorSetLayoutCreateInfo(rhs)
    {
    }

    DescriptorSetLayoutCreateInfo& operator=(VkDescriptorSetLayoutCreateInfo const & rhs)
    {
      m_descriptorSetLayoutCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_descriptorSetLayoutCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_descriptorSetLayoutCreateInfo.sType );
    }

    DescriptorSetLayoutCreateInfo& sType( StructureType sType )
    {
      m_descriptorSetLayoutCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_descriptorSetLayoutCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_descriptorSetLayoutCreateInfo.pNext );
    }

    DescriptorSetLayoutCreateInfo& pNext( const void* pNext )
    {
      m_descriptorSetLayoutCreateInfo.pNext = pNext;
      return *this;
    }

    const DescriptorSetLayoutCreateFlags& flags() const
    {
      return reinterpret_cast<const DescriptorSetLayoutCreateFlags&>( m_descriptorSetLayoutCreateInfo.flags );
    }

    DescriptorSetLayoutCreateFlags& flags()
    {
      return reinterpret_cast<DescriptorSetLayoutCreateFlags&>( m_descriptorSetLayoutCreateInfo.flags );
    }

    DescriptorSetLayoutCreateInfo& flags( DescriptorSetLayoutCreateFlags flags )
    {
      m_descriptorSetLayoutCreateInfo.flags = static_cast<VkDescriptorSetLayoutCreateFlags>( flags );
      return *this;
    }

    const uint32_t& bindingCount() const
    {
      return m_descriptorSetLayoutCreateInfo.bindingCount;
    }

    uint32_t& bindingCount()
    {
      return m_descriptorSetLayoutCreateInfo.bindingCount;
    }

    DescriptorSetLayoutCreateInfo& bindingCount( uint32_t bindingCount )
    {
      m_descriptorSetLayoutCreateInfo.bindingCount = bindingCount;
      return *this;
    }

    const DescriptorSetLayoutBinding* pBindings() const
    {
      return reinterpret_cast<const DescriptorSetLayoutBinding*>( m_descriptorSetLayoutCreateInfo.pBindings );
    }

    const DescriptorSetLayoutBinding* pBindings()
    {
      return reinterpret_cast<const DescriptorSetLayoutBinding*>( m_descriptorSetLayoutCreateInfo.pBindings );
    }

    DescriptorSetLayoutCreateInfo& pBindings( const DescriptorSetLayoutBinding* pBindings )
    {
      m_descriptorSetLayoutCreateInfo.pBindings = reinterpret_cast<const VkDescriptorSetLayoutBinding*>( pBindings );
      return *this;
    }

    static DescriptorSetLayoutCreateInfo& null()
    {
      return *((DescriptorSetLayoutCreateInfo*)(nullptr));
    }

    operator const VkDescriptorSetLayoutCreateInfo&() const
    {
      return m_descriptorSetLayoutCreateInfo;
    }

  private:
    VkDescriptorSetLayoutCreateInfo m_descriptorSetLayoutCreateInfo;
  };
  static_assert( sizeof( DescriptorSetLayoutCreateInfo ) == sizeof( VkDescriptorSetLayoutCreateInfo ), "struct and wrapper have different size!" );

  class PipelineShaderStageCreateInfo
  {
  public:
    PipelineShaderStageCreateInfo()
      : PipelineShaderStageCreateInfo( PipelineShaderStageCreateFlags(), ShaderStageFlagBits::eVertex, ShaderModule(), nullptr, nullptr )
    {}

    PipelineShaderStageCreateInfo( PipelineShaderStageCreateFlags flags, ShaderStageFlagBits stage, ShaderModule module, const char* pName, const SpecializationInfo* pSpecializationInfo)
    {
      m_pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      m_pipelineShaderStageCreateInfo.pNext = nullptr;
      m_pipelineShaderStageCreateInfo.flags = static_cast<VkPipelineShaderStageCreateFlags>( flags );
      m_pipelineShaderStageCreateInfo.stage = static_cast<VkShaderStageFlagBits>( stage );
      m_pipelineShaderStageCreateInfo.module = static_cast<VkShaderModule>( module );
      m_pipelineShaderStageCreateInfo.pName = pName;
      m_pipelineShaderStageCreateInfo.pSpecializationInfo = reinterpret_cast<const VkSpecializationInfo*>( pSpecializationInfo );
    }

    PipelineShaderStageCreateInfo(VkPipelineShaderStageCreateInfo const & rhs)
      : m_pipelineShaderStageCreateInfo(rhs)
    {
    }

    PipelineShaderStageCreateInfo& operator=(VkPipelineShaderStageCreateInfo const & rhs)
    {
      m_pipelineShaderStageCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineShaderStageCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_pipelineShaderStageCreateInfo.sType );
    }

    PipelineShaderStageCreateInfo& sType( StructureType sType )
    {
      m_pipelineShaderStageCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_pipelineShaderStageCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_pipelineShaderStageCreateInfo.pNext );
    }

    PipelineShaderStageCreateInfo& pNext( const void* pNext )
    {
      m_pipelineShaderStageCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineShaderStageCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineShaderStageCreateFlags&>( m_pipelineShaderStageCreateInfo.flags );
    }

    PipelineShaderStageCreateFlags& flags()
    {
      return reinterpret_cast<PipelineShaderStageCreateFlags&>( m_pipelineShaderStageCreateInfo.flags );
    }

    PipelineShaderStageCreateInfo& flags( PipelineShaderStageCreateFlags flags )
    {
      m_pipelineShaderStageCreateInfo.flags = static_cast<VkPipelineShaderStageCreateFlags>( flags );
      return *this;
    }

    const ShaderStageFlagBits& stage() const
    {
      return reinterpret_cast<const ShaderStageFlagBits&>( m_pipelineShaderStageCreateInfo.stage );
    }

    ShaderStageFlagBits& stage()
    {
      return reinterpret_cast<ShaderStageFlagBits&>( m_pipelineShaderStageCreateInfo.stage );
    }

    PipelineShaderStageCreateInfo& stage( ShaderStageFlagBits stage )
    {
      m_pipelineShaderStageCreateInfo.stage = static_cast<VkShaderStageFlagBits>( stage );
      return *this;
    }

    const ShaderModule& module() const
    {
      return reinterpret_cast<const ShaderModule&>( m_pipelineShaderStageCreateInfo.module );
    }

    ShaderModule& module()
    {
      return reinterpret_cast<ShaderModule&>( m_pipelineShaderStageCreateInfo.module );
    }

    PipelineShaderStageCreateInfo& module( ShaderModule module )
    {
      m_pipelineShaderStageCreateInfo.module = static_cast<VkShaderModule>( module );
      return *this;
    }

    const char* pName() const
    {
      return reinterpret_cast<const char*>( m_pipelineShaderStageCreateInfo.pName );
    }

    const char* pName()
    {
      return reinterpret_cast<const char*>( m_pipelineShaderStageCreateInfo.pName );
    }

    PipelineShaderStageCreateInfo& pName( const char* pName )
    {
      m_pipelineShaderStageCreateInfo.pName = pName;
      return *this;
    }

    const SpecializationInfo* pSpecializationInfo() const
    {
      return reinterpret_cast<const SpecializationInfo*>( m_pipelineShaderStageCreateInfo.pSpecializationInfo );
    }

    const SpecializationInfo* pSpecializationInfo()
    {
      return reinterpret_cast<const SpecializationInfo*>( m_pipelineShaderStageCreateInfo.pSpecializationInfo );
    }

    PipelineShaderStageCreateInfo& pSpecializationInfo( const SpecializationInfo* pSpecializationInfo )
    {
      m_pipelineShaderStageCreateInfo.pSpecializationInfo = reinterpret_cast<const VkSpecializationInfo*>( pSpecializationInfo );
      return *this;
    }

    static PipelineShaderStageCreateInfo& null()
    {
      return *((PipelineShaderStageCreateInfo*)(nullptr));
    }

    operator const VkPipelineShaderStageCreateInfo&() const
    {
      return m_pipelineShaderStageCreateInfo;
    }

  private:
    VkPipelineShaderStageCreateInfo m_pipelineShaderStageCreateInfo;
  };
  static_assert( sizeof( PipelineShaderStageCreateInfo ) == sizeof( VkPipelineShaderStageCreateInfo ), "struct and wrapper have different size!" );

  class PushConstantRange
  {
  public:
    PushConstantRange()
      : PushConstantRange( ShaderStageFlags(), 0, 0 )
    {}

    PushConstantRange( ShaderStageFlags stageFlags, uint32_t offset, uint32_t size)
    {
      m_pushConstantRange.stageFlags = static_cast<VkShaderStageFlags>( stageFlags );
      m_pushConstantRange.offset = offset;
      m_pushConstantRange.size = size;
    }

    PushConstantRange(VkPushConstantRange const & rhs)
      : m_pushConstantRange(rhs)
    {
    }

    PushConstantRange& operator=(VkPushConstantRange const & rhs)
    {
      m_pushConstantRange = rhs;
      return *this;
    }

    const ShaderStageFlags& stageFlags() const
    {
      return reinterpret_cast<const ShaderStageFlags&>( m_pushConstantRange.stageFlags );
    }

    ShaderStageFlags& stageFlags()
    {
      return reinterpret_cast<ShaderStageFlags&>( m_pushConstantRange.stageFlags );
    }

    PushConstantRange& stageFlags( ShaderStageFlags stageFlags )
    {
      m_pushConstantRange.stageFlags = static_cast<VkShaderStageFlags>( stageFlags );
      return *this;
    }

    const uint32_t& offset() const
    {
      return m_pushConstantRange.offset;
    }

    uint32_t& offset()
    {
      return m_pushConstantRange.offset;
    }

    PushConstantRange& offset( uint32_t offset )
    {
      m_pushConstantRange.offset = offset;
      return *this;
    }

    const uint32_t& size() const
    {
      return m_pushConstantRange.size;
    }

    uint32_t& size()
    {
      return m_pushConstantRange.size;
    }

    PushConstantRange& size( uint32_t size )
    {
      m_pushConstantRange.size = size;
      return *this;
    }

    static PushConstantRange& null()
    {
      return *((PushConstantRange*)(nullptr));
    }

    operator const VkPushConstantRange&() const
    {
      return m_pushConstantRange;
    }

  private:
    VkPushConstantRange m_pushConstantRange;
  };
  static_assert( sizeof( PushConstantRange ) == sizeof( VkPushConstantRange ), "struct and wrapper have different size!" );

  class PipelineLayoutCreateInfo
  {
  public:
    PipelineLayoutCreateInfo()
      : PipelineLayoutCreateInfo( PipelineLayoutCreateFlags(), 0, nullptr, 0, nullptr )
    {}

    PipelineLayoutCreateInfo( PipelineLayoutCreateFlags flags, uint32_t setLayoutCount, const DescriptorSetLayout* pSetLayouts, uint32_t pushConstantRangeCount, const PushConstantRange* pPushConstantRanges)
    {
      m_pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      m_pipelineLayoutCreateInfo.pNext = nullptr;
      m_pipelineLayoutCreateInfo.flags = static_cast<VkPipelineLayoutCreateFlags>( flags );
      m_pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount;
      m_pipelineLayoutCreateInfo.pSetLayouts = reinterpret_cast<const VkDescriptorSetLayout*>( pSetLayouts );
      m_pipelineLayoutCreateInfo.pushConstantRangeCount = pushConstantRangeCount;
      m_pipelineLayoutCreateInfo.pPushConstantRanges = reinterpret_cast<const VkPushConstantRange*>( pPushConstantRanges );
    }

    PipelineLayoutCreateInfo(VkPipelineLayoutCreateInfo const & rhs)
      : m_pipelineLayoutCreateInfo(rhs)
    {
    }

    PipelineLayoutCreateInfo& operator=(VkPipelineLayoutCreateInfo const & rhs)
    {
      m_pipelineLayoutCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineLayoutCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_pipelineLayoutCreateInfo.sType );
    }

    PipelineLayoutCreateInfo& sType( StructureType sType )
    {
      m_pipelineLayoutCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_pipelineLayoutCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_pipelineLayoutCreateInfo.pNext );
    }

    PipelineLayoutCreateInfo& pNext( const void* pNext )
    {
      m_pipelineLayoutCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineLayoutCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineLayoutCreateFlags&>( m_pipelineLayoutCreateInfo.flags );
    }

    PipelineLayoutCreateFlags& flags()
    {
      return reinterpret_cast<PipelineLayoutCreateFlags&>( m_pipelineLayoutCreateInfo.flags );
    }

    PipelineLayoutCreateInfo& flags( PipelineLayoutCreateFlags flags )
    {
      m_pipelineLayoutCreateInfo.flags = static_cast<VkPipelineLayoutCreateFlags>( flags );
      return *this;
    }

    const uint32_t& setLayoutCount() const
    {
      return m_pipelineLayoutCreateInfo.setLayoutCount;
    }

    uint32_t& setLayoutCount()
    {
      return m_pipelineLayoutCreateInfo.setLayoutCount;
    }

    PipelineLayoutCreateInfo& setLayoutCount( uint32_t setLayoutCount )
    {
      m_pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount;
      return *this;
    }

    const DescriptorSetLayout* pSetLayouts() const
    {
      return reinterpret_cast<const DescriptorSetLayout*>( m_pipelineLayoutCreateInfo.pSetLayouts );
    }

    const DescriptorSetLayout* pSetLayouts()
    {
      return reinterpret_cast<const DescriptorSetLayout*>( m_pipelineLayoutCreateInfo.pSetLayouts );
    }

    PipelineLayoutCreateInfo& pSetLayouts( const DescriptorSetLayout* pSetLayouts )
    {
      m_pipelineLayoutCreateInfo.pSetLayouts = reinterpret_cast<const VkDescriptorSetLayout*>( pSetLayouts );
      return *this;
    }

    const uint32_t& pushConstantRangeCount() const
    {
      return m_pipelineLayoutCreateInfo.pushConstantRangeCount;
    }

    uint32_t& pushConstantRangeCount()
    {
      return m_pipelineLayoutCreateInfo.pushConstantRangeCount;
    }

    PipelineLayoutCreateInfo& pushConstantRangeCount( uint32_t pushConstantRangeCount )
    {
      m_pipelineLayoutCreateInfo.pushConstantRangeCount = pushConstantRangeCount;
      return *this;
    }

    const PushConstantRange* pPushConstantRanges() const
    {
      return reinterpret_cast<const PushConstantRange*>( m_pipelineLayoutCreateInfo.pPushConstantRanges );
    }

    const PushConstantRange* pPushConstantRanges()
    {
      return reinterpret_cast<const PushConstantRange*>( m_pipelineLayoutCreateInfo.pPushConstantRanges );
    }

    PipelineLayoutCreateInfo& pPushConstantRanges( const PushConstantRange* pPushConstantRanges )
    {
      m_pipelineLayoutCreateInfo.pPushConstantRanges = reinterpret_cast<const VkPushConstantRange*>( pPushConstantRanges );
      return *this;
    }

    static PipelineLayoutCreateInfo& null()
    {
      return *((PipelineLayoutCreateInfo*)(nullptr));
    }

    operator const VkPipelineLayoutCreateInfo&() const
    {
      return m_pipelineLayoutCreateInfo;
    }

  private:
    VkPipelineLayoutCreateInfo m_pipelineLayoutCreateInfo;
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

  typedef Flags<ImageUsageFlagBits, VkImageUsageFlags> ImageUsageFlags;

  inline ImageUsageFlags operator|( ImageUsageFlagBits bit0, ImageUsageFlagBits bit1 )
  {
    return ImageUsageFlags( bit0 ) | bit1;
  }

  enum class ImageCreateFlagBits
  {
    eSparseBinding = VK_IMAGE_CREATE_SPARSE_BINDING_BIT,
    eSparseResidency = VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT,
    eSparseAliased = VK_IMAGE_CREATE_SPARSE_ALIASED_BIT,
    eMutableFormat = VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT,
    eCubeCompatible = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT
  };

  typedef Flags<ImageCreateFlagBits, VkImageCreateFlags> ImageCreateFlags;

  inline ImageCreateFlags operator|( ImageCreateFlagBits bit0, ImageCreateFlagBits bit1 )
  {
    return ImageCreateFlags( bit0 ) | bit1;
  }

  enum class PipelineCreateFlagBits
  {
    eDisableOptimization = VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT,
    eAllowDerivatives = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT,
    eDerivative = VK_PIPELINE_CREATE_DERIVATIVE_BIT
  };

  typedef Flags<PipelineCreateFlagBits, VkPipelineCreateFlags> PipelineCreateFlags;

  inline PipelineCreateFlags operator|( PipelineCreateFlagBits bit0, PipelineCreateFlagBits bit1 )
  {
    return PipelineCreateFlags( bit0 ) | bit1;
  }

  class ComputePipelineCreateInfo
  {
  public:
    ComputePipelineCreateInfo()
      : ComputePipelineCreateInfo( PipelineCreateFlags(), PipelineShaderStageCreateInfo(), PipelineLayout(), Pipeline(), 0 )
    {}

    ComputePipelineCreateInfo( PipelineCreateFlags flags, PipelineShaderStageCreateInfo stage, PipelineLayout layout, Pipeline basePipelineHandle, int32_t basePipelineIndex)
    {
      m_computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
      m_computePipelineCreateInfo.pNext = nullptr;
      m_computePipelineCreateInfo.flags = static_cast<VkPipelineCreateFlags>( flags );
      m_computePipelineCreateInfo.stage = static_cast<VkPipelineShaderStageCreateInfo>( stage );
      m_computePipelineCreateInfo.layout = static_cast<VkPipelineLayout>( layout );
      m_computePipelineCreateInfo.basePipelineHandle = static_cast<VkPipeline>( basePipelineHandle );
      m_computePipelineCreateInfo.basePipelineIndex = basePipelineIndex;
    }

    ComputePipelineCreateInfo(VkComputePipelineCreateInfo const & rhs)
      : m_computePipelineCreateInfo(rhs)
    {
    }

    ComputePipelineCreateInfo& operator=(VkComputePipelineCreateInfo const & rhs)
    {
      m_computePipelineCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_computePipelineCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_computePipelineCreateInfo.sType );
    }

    ComputePipelineCreateInfo& sType( StructureType sType )
    {
      m_computePipelineCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_computePipelineCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_computePipelineCreateInfo.pNext );
    }

    ComputePipelineCreateInfo& pNext( const void* pNext )
    {
      m_computePipelineCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineCreateFlags&>( m_computePipelineCreateInfo.flags );
    }

    PipelineCreateFlags& flags()
    {
      return reinterpret_cast<PipelineCreateFlags&>( m_computePipelineCreateInfo.flags );
    }

    ComputePipelineCreateInfo& flags( PipelineCreateFlags flags )
    {
      m_computePipelineCreateInfo.flags = static_cast<VkPipelineCreateFlags>( flags );
      return *this;
    }

    const PipelineShaderStageCreateInfo& stage() const
    {
      return reinterpret_cast<const PipelineShaderStageCreateInfo&>( m_computePipelineCreateInfo.stage );
    }

    PipelineShaderStageCreateInfo& stage()
    {
      return reinterpret_cast<PipelineShaderStageCreateInfo&>( m_computePipelineCreateInfo.stage );
    }

    ComputePipelineCreateInfo& stage( PipelineShaderStageCreateInfo stage )
    {
      m_computePipelineCreateInfo.stage = static_cast<VkPipelineShaderStageCreateInfo>( stage );
      return *this;
    }

    const PipelineLayout& layout() const
    {
      return reinterpret_cast<const PipelineLayout&>( m_computePipelineCreateInfo.layout );
    }

    PipelineLayout& layout()
    {
      return reinterpret_cast<PipelineLayout&>( m_computePipelineCreateInfo.layout );
    }

    ComputePipelineCreateInfo& layout( PipelineLayout layout )
    {
      m_computePipelineCreateInfo.layout = static_cast<VkPipelineLayout>( layout );
      return *this;
    }

    const Pipeline& basePipelineHandle() const
    {
      return reinterpret_cast<const Pipeline&>( m_computePipelineCreateInfo.basePipelineHandle );
    }

    Pipeline& basePipelineHandle()
    {
      return reinterpret_cast<Pipeline&>( m_computePipelineCreateInfo.basePipelineHandle );
    }

    ComputePipelineCreateInfo& basePipelineHandle( Pipeline basePipelineHandle )
    {
      m_computePipelineCreateInfo.basePipelineHandle = static_cast<VkPipeline>( basePipelineHandle );
      return *this;
    }

    const int32_t& basePipelineIndex() const
    {
      return m_computePipelineCreateInfo.basePipelineIndex;
    }

    int32_t& basePipelineIndex()
    {
      return m_computePipelineCreateInfo.basePipelineIndex;
    }

    ComputePipelineCreateInfo& basePipelineIndex( int32_t basePipelineIndex )
    {
      m_computePipelineCreateInfo.basePipelineIndex = basePipelineIndex;
      return *this;
    }

    static ComputePipelineCreateInfo& null()
    {
      return *((ComputePipelineCreateInfo*)(nullptr));
    }

    operator const VkComputePipelineCreateInfo&() const
    {
      return m_computePipelineCreateInfo;
    }

  private:
    VkComputePipelineCreateInfo m_computePipelineCreateInfo;
  };
  static_assert( sizeof( ComputePipelineCreateInfo ) == sizeof( VkComputePipelineCreateInfo ), "struct and wrapper have different size!" );

  enum class ColorComponentFlagBits
  {
    eR = VK_COLOR_COMPONENT_R_BIT,
    eG = VK_COLOR_COMPONENT_G_BIT,
    eB = VK_COLOR_COMPONENT_B_BIT,
    eA = VK_COLOR_COMPONENT_A_BIT
  };

  typedef Flags<ColorComponentFlagBits, VkColorComponentFlags> ColorComponentFlags;

  inline ColorComponentFlags operator|( ColorComponentFlagBits bit0, ColorComponentFlagBits bit1 )
  {
    return ColorComponentFlags( bit0 ) | bit1;
  }

  class PipelineColorBlendAttachmentState
  {
  public:
    PipelineColorBlendAttachmentState()
      : PipelineColorBlendAttachmentState( 0, BlendFactor::eZero, BlendFactor::eZero, BlendOp::eAdd, BlendFactor::eZero, BlendFactor::eZero, BlendOp::eAdd, ColorComponentFlags() )
    {}

    PipelineColorBlendAttachmentState( Bool32 blendEnable, BlendFactor srcColorBlendFactor, BlendFactor dstColorBlendFactor, BlendOp colorBlendOp, BlendFactor srcAlphaBlendFactor, BlendFactor dstAlphaBlendFactor, BlendOp alphaBlendOp, ColorComponentFlags colorWriteMask)
    {
      m_pipelineColorBlendAttachmentState.blendEnable = blendEnable;
      m_pipelineColorBlendAttachmentState.srcColorBlendFactor = static_cast<VkBlendFactor>( srcColorBlendFactor );
      m_pipelineColorBlendAttachmentState.dstColorBlendFactor = static_cast<VkBlendFactor>( dstColorBlendFactor );
      m_pipelineColorBlendAttachmentState.colorBlendOp = static_cast<VkBlendOp>( colorBlendOp );
      m_pipelineColorBlendAttachmentState.srcAlphaBlendFactor = static_cast<VkBlendFactor>( srcAlphaBlendFactor );
      m_pipelineColorBlendAttachmentState.dstAlphaBlendFactor = static_cast<VkBlendFactor>( dstAlphaBlendFactor );
      m_pipelineColorBlendAttachmentState.alphaBlendOp = static_cast<VkBlendOp>( alphaBlendOp );
      m_pipelineColorBlendAttachmentState.colorWriteMask = static_cast<VkColorComponentFlags>( colorWriteMask );
    }

    PipelineColorBlendAttachmentState(VkPipelineColorBlendAttachmentState const & rhs)
      : m_pipelineColorBlendAttachmentState(rhs)
    {
    }

    PipelineColorBlendAttachmentState& operator=(VkPipelineColorBlendAttachmentState const & rhs)
    {
      m_pipelineColorBlendAttachmentState = rhs;
      return *this;
    }

    const Bool32& blendEnable() const
    {
      return m_pipelineColorBlendAttachmentState.blendEnable;
    }

    Bool32& blendEnable()
    {
      return m_pipelineColorBlendAttachmentState.blendEnable;
    }

    PipelineColorBlendAttachmentState& blendEnable( Bool32 blendEnable )
    {
      m_pipelineColorBlendAttachmentState.blendEnable = blendEnable;
      return *this;
    }

    const BlendFactor& srcColorBlendFactor() const
    {
      return reinterpret_cast<const BlendFactor&>( m_pipelineColorBlendAttachmentState.srcColorBlendFactor );
    }

    BlendFactor& srcColorBlendFactor()
    {
      return reinterpret_cast<BlendFactor&>( m_pipelineColorBlendAttachmentState.srcColorBlendFactor );
    }

    PipelineColorBlendAttachmentState& srcColorBlendFactor( BlendFactor srcColorBlendFactor )
    {
      m_pipelineColorBlendAttachmentState.srcColorBlendFactor = static_cast<VkBlendFactor>( srcColorBlendFactor );
      return *this;
    }

    const BlendFactor& dstColorBlendFactor() const
    {
      return reinterpret_cast<const BlendFactor&>( m_pipelineColorBlendAttachmentState.dstColorBlendFactor );
    }

    BlendFactor& dstColorBlendFactor()
    {
      return reinterpret_cast<BlendFactor&>( m_pipelineColorBlendAttachmentState.dstColorBlendFactor );
    }

    PipelineColorBlendAttachmentState& dstColorBlendFactor( BlendFactor dstColorBlendFactor )
    {
      m_pipelineColorBlendAttachmentState.dstColorBlendFactor = static_cast<VkBlendFactor>( dstColorBlendFactor );
      return *this;
    }

    const BlendOp& colorBlendOp() const
    {
      return reinterpret_cast<const BlendOp&>( m_pipelineColorBlendAttachmentState.colorBlendOp );
    }

    BlendOp& colorBlendOp()
    {
      return reinterpret_cast<BlendOp&>( m_pipelineColorBlendAttachmentState.colorBlendOp );
    }

    PipelineColorBlendAttachmentState& colorBlendOp( BlendOp colorBlendOp )
    {
      m_pipelineColorBlendAttachmentState.colorBlendOp = static_cast<VkBlendOp>( colorBlendOp );
      return *this;
    }

    const BlendFactor& srcAlphaBlendFactor() const
    {
      return reinterpret_cast<const BlendFactor&>( m_pipelineColorBlendAttachmentState.srcAlphaBlendFactor );
    }

    BlendFactor& srcAlphaBlendFactor()
    {
      return reinterpret_cast<BlendFactor&>( m_pipelineColorBlendAttachmentState.srcAlphaBlendFactor );
    }

    PipelineColorBlendAttachmentState& srcAlphaBlendFactor( BlendFactor srcAlphaBlendFactor )
    {
      m_pipelineColorBlendAttachmentState.srcAlphaBlendFactor = static_cast<VkBlendFactor>( srcAlphaBlendFactor );
      return *this;
    }

    const BlendFactor& dstAlphaBlendFactor() const
    {
      return reinterpret_cast<const BlendFactor&>( m_pipelineColorBlendAttachmentState.dstAlphaBlendFactor );
    }

    BlendFactor& dstAlphaBlendFactor()
    {
      return reinterpret_cast<BlendFactor&>( m_pipelineColorBlendAttachmentState.dstAlphaBlendFactor );
    }

    PipelineColorBlendAttachmentState& dstAlphaBlendFactor( BlendFactor dstAlphaBlendFactor )
    {
      m_pipelineColorBlendAttachmentState.dstAlphaBlendFactor = static_cast<VkBlendFactor>( dstAlphaBlendFactor );
      return *this;
    }

    const BlendOp& alphaBlendOp() const
    {
      return reinterpret_cast<const BlendOp&>( m_pipelineColorBlendAttachmentState.alphaBlendOp );
    }

    BlendOp& alphaBlendOp()
    {
      return reinterpret_cast<BlendOp&>( m_pipelineColorBlendAttachmentState.alphaBlendOp );
    }

    PipelineColorBlendAttachmentState& alphaBlendOp( BlendOp alphaBlendOp )
    {
      m_pipelineColorBlendAttachmentState.alphaBlendOp = static_cast<VkBlendOp>( alphaBlendOp );
      return *this;
    }

    const ColorComponentFlags& colorWriteMask() const
    {
      return reinterpret_cast<const ColorComponentFlags&>( m_pipelineColorBlendAttachmentState.colorWriteMask );
    }

    ColorComponentFlags& colorWriteMask()
    {
      return reinterpret_cast<ColorComponentFlags&>( m_pipelineColorBlendAttachmentState.colorWriteMask );
    }

    PipelineColorBlendAttachmentState& colorWriteMask( ColorComponentFlags colorWriteMask )
    {
      m_pipelineColorBlendAttachmentState.colorWriteMask = static_cast<VkColorComponentFlags>( colorWriteMask );
      return *this;
    }

    static PipelineColorBlendAttachmentState& null()
    {
      return *((PipelineColorBlendAttachmentState*)(nullptr));
    }

    operator const VkPipelineColorBlendAttachmentState&() const
    {
      return m_pipelineColorBlendAttachmentState;
    }

  private:
    VkPipelineColorBlendAttachmentState m_pipelineColorBlendAttachmentState;
  };
  static_assert( sizeof( PipelineColorBlendAttachmentState ) == sizeof( VkPipelineColorBlendAttachmentState ), "struct and wrapper have different size!" );

  class PipelineColorBlendStateCreateInfo
  {
  public:
    PipelineColorBlendStateCreateInfo()
      : PipelineColorBlendStateCreateInfo( PipelineColorBlendStateCreateFlags(), 0, LogicOp::eClear, 0, nullptr, { 0 } )
    {}

    PipelineColorBlendStateCreateInfo( PipelineColorBlendStateCreateFlags flags, Bool32 logicOpEnable, LogicOp logicOp, uint32_t attachmentCount, const PipelineColorBlendAttachmentState* pAttachments, std::array<float,4> const& blendConstants)
    {
      m_pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      m_pipelineColorBlendStateCreateInfo.pNext = nullptr;
      m_pipelineColorBlendStateCreateInfo.flags = static_cast<VkPipelineColorBlendStateCreateFlags>( flags );
      m_pipelineColorBlendStateCreateInfo.logicOpEnable = logicOpEnable;
      m_pipelineColorBlendStateCreateInfo.logicOp = static_cast<VkLogicOp>( logicOp );
      m_pipelineColorBlendStateCreateInfo.attachmentCount = attachmentCount;
      m_pipelineColorBlendStateCreateInfo.pAttachments = reinterpret_cast<const VkPipelineColorBlendAttachmentState*>( pAttachments );
      memcpy( &m_pipelineColorBlendStateCreateInfo.blendConstants, blendConstants.data(), 4 * sizeof( float ) );
    }

    PipelineColorBlendStateCreateInfo(VkPipelineColorBlendStateCreateInfo const & rhs)
      : m_pipelineColorBlendStateCreateInfo(rhs)
    {
    }

    PipelineColorBlendStateCreateInfo& operator=(VkPipelineColorBlendStateCreateInfo const & rhs)
    {
      m_pipelineColorBlendStateCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineColorBlendStateCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_pipelineColorBlendStateCreateInfo.sType );
    }

    PipelineColorBlendStateCreateInfo& sType( StructureType sType )
    {
      m_pipelineColorBlendStateCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_pipelineColorBlendStateCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_pipelineColorBlendStateCreateInfo.pNext );
    }

    PipelineColorBlendStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineColorBlendStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineColorBlendStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineColorBlendStateCreateFlags&>( m_pipelineColorBlendStateCreateInfo.flags );
    }

    PipelineColorBlendStateCreateFlags& flags()
    {
      return reinterpret_cast<PipelineColorBlendStateCreateFlags&>( m_pipelineColorBlendStateCreateInfo.flags );
    }

    PipelineColorBlendStateCreateInfo& flags( PipelineColorBlendStateCreateFlags flags )
    {
      m_pipelineColorBlendStateCreateInfo.flags = static_cast<VkPipelineColorBlendStateCreateFlags>( flags );
      return *this;
    }

    const Bool32& logicOpEnable() const
    {
      return m_pipelineColorBlendStateCreateInfo.logicOpEnable;
    }

    Bool32& logicOpEnable()
    {
      return m_pipelineColorBlendStateCreateInfo.logicOpEnable;
    }

    PipelineColorBlendStateCreateInfo& logicOpEnable( Bool32 logicOpEnable )
    {
      m_pipelineColorBlendStateCreateInfo.logicOpEnable = logicOpEnable;
      return *this;
    }

    const LogicOp& logicOp() const
    {
      return reinterpret_cast<const LogicOp&>( m_pipelineColorBlendStateCreateInfo.logicOp );
    }

    LogicOp& logicOp()
    {
      return reinterpret_cast<LogicOp&>( m_pipelineColorBlendStateCreateInfo.logicOp );
    }

    PipelineColorBlendStateCreateInfo& logicOp( LogicOp logicOp )
    {
      m_pipelineColorBlendStateCreateInfo.logicOp = static_cast<VkLogicOp>( logicOp );
      return *this;
    }

    const uint32_t& attachmentCount() const
    {
      return m_pipelineColorBlendStateCreateInfo.attachmentCount;
    }

    uint32_t& attachmentCount()
    {
      return m_pipelineColorBlendStateCreateInfo.attachmentCount;
    }

    PipelineColorBlendStateCreateInfo& attachmentCount( uint32_t attachmentCount )
    {
      m_pipelineColorBlendStateCreateInfo.attachmentCount = attachmentCount;
      return *this;
    }

    const PipelineColorBlendAttachmentState* pAttachments() const
    {
      return reinterpret_cast<const PipelineColorBlendAttachmentState*>( m_pipelineColorBlendStateCreateInfo.pAttachments );
    }

    const PipelineColorBlendAttachmentState* pAttachments()
    {
      return reinterpret_cast<const PipelineColorBlendAttachmentState*>( m_pipelineColorBlendStateCreateInfo.pAttachments );
    }

    PipelineColorBlendStateCreateInfo& pAttachments( const PipelineColorBlendAttachmentState* pAttachments )
    {
      m_pipelineColorBlendStateCreateInfo.pAttachments = reinterpret_cast<const VkPipelineColorBlendAttachmentState*>( pAttachments );
      return *this;
    }

    const float* blendConstants() const
    {
      return reinterpret_cast<const float*>( m_pipelineColorBlendStateCreateInfo.blendConstants );
    }

    float* blendConstants()
    {
      return reinterpret_cast<float*>( m_pipelineColorBlendStateCreateInfo.blendConstants );
    }

    PipelineColorBlendStateCreateInfo& blendConstants( std::array<float,4> blendConstants )
    {
      memcpy( &m_pipelineColorBlendStateCreateInfo.blendConstants, blendConstants.data(), 4 * sizeof( float ) );
      return *this;
    }

    static PipelineColorBlendStateCreateInfo& null()
    {
      return *((PipelineColorBlendStateCreateInfo*)(nullptr));
    }

    operator const VkPipelineColorBlendStateCreateInfo&() const
    {
      return m_pipelineColorBlendStateCreateInfo;
    }

  private:
    VkPipelineColorBlendStateCreateInfo m_pipelineColorBlendStateCreateInfo;
  };
  static_assert( sizeof( PipelineColorBlendStateCreateInfo ) == sizeof( VkPipelineColorBlendStateCreateInfo ), "struct and wrapper have different size!" );

  enum class FenceCreateFlagBits
  {
    eSignaled = VK_FENCE_CREATE_SIGNALED_BIT
  };

  typedef Flags<FenceCreateFlagBits, VkFenceCreateFlags> FenceCreateFlags;

  inline FenceCreateFlags operator|( FenceCreateFlagBits bit0, FenceCreateFlagBits bit1 )
  {
    return FenceCreateFlags( bit0 ) | bit1;
  }

  class FenceCreateInfo
  {
  public:
    FenceCreateInfo()
      : FenceCreateInfo( FenceCreateFlags() )
    {}

    FenceCreateInfo( FenceCreateFlags flags)
    {
      m_fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      m_fenceCreateInfo.pNext = nullptr;
      m_fenceCreateInfo.flags = static_cast<VkFenceCreateFlags>( flags );
    }

    FenceCreateInfo(VkFenceCreateInfo const & rhs)
      : m_fenceCreateInfo(rhs)
    {
    }

    FenceCreateInfo& operator=(VkFenceCreateInfo const & rhs)
    {
      m_fenceCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_fenceCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_fenceCreateInfo.sType );
    }

    FenceCreateInfo& sType( StructureType sType )
    {
      m_fenceCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_fenceCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_fenceCreateInfo.pNext );
    }

    FenceCreateInfo& pNext( const void* pNext )
    {
      m_fenceCreateInfo.pNext = pNext;
      return *this;
    }

    const FenceCreateFlags& flags() const
    {
      return reinterpret_cast<const FenceCreateFlags&>( m_fenceCreateInfo.flags );
    }

    FenceCreateFlags& flags()
    {
      return reinterpret_cast<FenceCreateFlags&>( m_fenceCreateInfo.flags );
    }

    FenceCreateInfo& flags( FenceCreateFlags flags )
    {
      m_fenceCreateInfo.flags = static_cast<VkFenceCreateFlags>( flags );
      return *this;
    }

    static FenceCreateInfo& null()
    {
      return *((FenceCreateInfo*)(nullptr));
    }

    operator const VkFenceCreateInfo&() const
    {
      return m_fenceCreateInfo;
    }

  private:
    VkFenceCreateInfo m_fenceCreateInfo;
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
    eSampledImageFilterLinear = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT
  };

  typedef Flags<FormatFeatureFlagBits, VkFormatFeatureFlags> FormatFeatureFlags;

  inline FormatFeatureFlags operator|( FormatFeatureFlagBits bit0, FormatFeatureFlagBits bit1 )
  {
    return FormatFeatureFlags( bit0 ) | bit1;
  }

  class FormatProperties
  {
  public:
    const FormatFeatureFlags& linearTilingFeatures() const
    {
      return reinterpret_cast<const FormatFeatureFlags&>( m_formatProperties.linearTilingFeatures );
    }

    const FormatFeatureFlags& optimalTilingFeatures() const
    {
      return reinterpret_cast<const FormatFeatureFlags&>( m_formatProperties.optimalTilingFeatures );
    }

    const FormatFeatureFlags& bufferFeatures() const
    {
      return reinterpret_cast<const FormatFeatureFlags&>( m_formatProperties.bufferFeatures );
    }

    static FormatProperties& null()
    {
      return *((FormatProperties*)(nullptr));
    }

    operator const VkFormatProperties&() const
    {
      return m_formatProperties;
    }

  private:
    VkFormatProperties m_formatProperties;
  };
  static_assert( sizeof( FormatProperties ) == sizeof( VkFormatProperties ), "struct and wrapper have different size!" );

  enum class QueryControlFlagBits
  {
    ePrecise = VK_QUERY_CONTROL_PRECISE_BIT
  };

  typedef Flags<QueryControlFlagBits, VkQueryControlFlags> QueryControlFlags;

  inline QueryControlFlags operator|( QueryControlFlagBits bit0, QueryControlFlagBits bit1 )
  {
    return QueryControlFlags( bit0 ) | bit1;
  }

  enum class QueryResultFlagBits
  {
    e64 = VK_QUERY_RESULT_64_BIT,
    eWait = VK_QUERY_RESULT_WAIT_BIT,
    eWithAvailability = VK_QUERY_RESULT_WITH_AVAILABILITY_BIT,
    ePartial = VK_QUERY_RESULT_PARTIAL_BIT
  };

  typedef Flags<QueryResultFlagBits, VkQueryResultFlags> QueryResultFlags;

  inline QueryResultFlags operator|( QueryResultFlagBits bit0, QueryResultFlagBits bit1 )
  {
    return QueryResultFlags( bit0 ) | bit1;
  }

  enum class CommandBufferUsageFlagBits
  {
    eOneTimeSubmit = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    eRenderPassContinue = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
    eSimultaneousUse = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
  };

  typedef Flags<CommandBufferUsageFlagBits, VkCommandBufferUsageFlags> CommandBufferUsageFlags;

  inline CommandBufferUsageFlags operator|( CommandBufferUsageFlagBits bit0, CommandBufferUsageFlagBits bit1 )
  {
    return CommandBufferUsageFlags( bit0 ) | bit1;
  }

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

  typedef Flags<QueryPipelineStatisticFlagBits, VkQueryPipelineStatisticFlags> QueryPipelineStatisticFlags;

  inline QueryPipelineStatisticFlags operator|( QueryPipelineStatisticFlagBits bit0, QueryPipelineStatisticFlagBits bit1 )
  {
    return QueryPipelineStatisticFlags( bit0 ) | bit1;
  }

  class CommandBufferInheritanceInfo
  {
  public:
    CommandBufferInheritanceInfo()
      : CommandBufferInheritanceInfo( RenderPass(), 0, Framebuffer(), 0, QueryControlFlags(), QueryPipelineStatisticFlags() )
    {}

    CommandBufferInheritanceInfo( RenderPass renderPass, uint32_t subpass, Framebuffer framebuffer, Bool32 occlusionQueryEnable, QueryControlFlags queryFlags, QueryPipelineStatisticFlags pipelineStatistics)
    {
      m_commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
      m_commandBufferInheritanceInfo.pNext = nullptr;
      m_commandBufferInheritanceInfo.renderPass = static_cast<VkRenderPass>( renderPass );
      m_commandBufferInheritanceInfo.subpass = subpass;
      m_commandBufferInheritanceInfo.framebuffer = static_cast<VkFramebuffer>( framebuffer );
      m_commandBufferInheritanceInfo.occlusionQueryEnable = occlusionQueryEnable;
      m_commandBufferInheritanceInfo.queryFlags = static_cast<VkQueryControlFlags>( queryFlags );
      m_commandBufferInheritanceInfo.pipelineStatistics = static_cast<VkQueryPipelineStatisticFlags>( pipelineStatistics );
    }

    CommandBufferInheritanceInfo(VkCommandBufferInheritanceInfo const & rhs)
      : m_commandBufferInheritanceInfo(rhs)
    {
    }

    CommandBufferInheritanceInfo& operator=(VkCommandBufferInheritanceInfo const & rhs)
    {
      m_commandBufferInheritanceInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_commandBufferInheritanceInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_commandBufferInheritanceInfo.sType );
    }

    CommandBufferInheritanceInfo& sType( StructureType sType )
    {
      m_commandBufferInheritanceInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_commandBufferInheritanceInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_commandBufferInheritanceInfo.pNext );
    }

    CommandBufferInheritanceInfo& pNext( const void* pNext )
    {
      m_commandBufferInheritanceInfo.pNext = pNext;
      return *this;
    }

    const RenderPass& renderPass() const
    {
      return reinterpret_cast<const RenderPass&>( m_commandBufferInheritanceInfo.renderPass );
    }

    RenderPass& renderPass()
    {
      return reinterpret_cast<RenderPass&>( m_commandBufferInheritanceInfo.renderPass );
    }

    CommandBufferInheritanceInfo& renderPass( RenderPass renderPass )
    {
      m_commandBufferInheritanceInfo.renderPass = static_cast<VkRenderPass>( renderPass );
      return *this;
    }

    const uint32_t& subpass() const
    {
      return m_commandBufferInheritanceInfo.subpass;
    }

    uint32_t& subpass()
    {
      return m_commandBufferInheritanceInfo.subpass;
    }

    CommandBufferInheritanceInfo& subpass( uint32_t subpass )
    {
      m_commandBufferInheritanceInfo.subpass = subpass;
      return *this;
    }

    const Framebuffer& framebuffer() const
    {
      return reinterpret_cast<const Framebuffer&>( m_commandBufferInheritanceInfo.framebuffer );
    }

    Framebuffer& framebuffer()
    {
      return reinterpret_cast<Framebuffer&>( m_commandBufferInheritanceInfo.framebuffer );
    }

    CommandBufferInheritanceInfo& framebuffer( Framebuffer framebuffer )
    {
      m_commandBufferInheritanceInfo.framebuffer = static_cast<VkFramebuffer>( framebuffer );
      return *this;
    }

    const Bool32& occlusionQueryEnable() const
    {
      return m_commandBufferInheritanceInfo.occlusionQueryEnable;
    }

    Bool32& occlusionQueryEnable()
    {
      return m_commandBufferInheritanceInfo.occlusionQueryEnable;
    }

    CommandBufferInheritanceInfo& occlusionQueryEnable( Bool32 occlusionQueryEnable )
    {
      m_commandBufferInheritanceInfo.occlusionQueryEnable = occlusionQueryEnable;
      return *this;
    }

    const QueryControlFlags& queryFlags() const
    {
      return reinterpret_cast<const QueryControlFlags&>( m_commandBufferInheritanceInfo.queryFlags );
    }

    QueryControlFlags& queryFlags()
    {
      return reinterpret_cast<QueryControlFlags&>( m_commandBufferInheritanceInfo.queryFlags );
    }

    CommandBufferInheritanceInfo& queryFlags( QueryControlFlags queryFlags )
    {
      m_commandBufferInheritanceInfo.queryFlags = static_cast<VkQueryControlFlags>( queryFlags );
      return *this;
    }

    const QueryPipelineStatisticFlags& pipelineStatistics() const
    {
      return reinterpret_cast<const QueryPipelineStatisticFlags&>( m_commandBufferInheritanceInfo.pipelineStatistics );
    }

    QueryPipelineStatisticFlags& pipelineStatistics()
    {
      return reinterpret_cast<QueryPipelineStatisticFlags&>( m_commandBufferInheritanceInfo.pipelineStatistics );
    }

    CommandBufferInheritanceInfo& pipelineStatistics( QueryPipelineStatisticFlags pipelineStatistics )
    {
      m_commandBufferInheritanceInfo.pipelineStatistics = static_cast<VkQueryPipelineStatisticFlags>( pipelineStatistics );
      return *this;
    }

    static CommandBufferInheritanceInfo& null()
    {
      return *((CommandBufferInheritanceInfo*)(nullptr));
    }

    operator const VkCommandBufferInheritanceInfo&() const
    {
      return m_commandBufferInheritanceInfo;
    }

  private:
    VkCommandBufferInheritanceInfo m_commandBufferInheritanceInfo;
  };
  static_assert( sizeof( CommandBufferInheritanceInfo ) == sizeof( VkCommandBufferInheritanceInfo ), "struct and wrapper have different size!" );

  class CommandBufferBeginInfo
  {
  public:
    CommandBufferBeginInfo()
      : CommandBufferBeginInfo( CommandBufferUsageFlags(), nullptr )
    {}

    CommandBufferBeginInfo( CommandBufferUsageFlags flags, const CommandBufferInheritanceInfo* pInheritanceInfo)
    {
      m_commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      m_commandBufferBeginInfo.pNext = nullptr;
      m_commandBufferBeginInfo.flags = static_cast<VkCommandBufferUsageFlags>( flags );
      m_commandBufferBeginInfo.pInheritanceInfo = reinterpret_cast<const VkCommandBufferInheritanceInfo*>( pInheritanceInfo );
    }

    CommandBufferBeginInfo(VkCommandBufferBeginInfo const & rhs)
      : m_commandBufferBeginInfo(rhs)
    {
    }

    CommandBufferBeginInfo& operator=(VkCommandBufferBeginInfo const & rhs)
    {
      m_commandBufferBeginInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_commandBufferBeginInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_commandBufferBeginInfo.sType );
    }

    CommandBufferBeginInfo& sType( StructureType sType )
    {
      m_commandBufferBeginInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_commandBufferBeginInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_commandBufferBeginInfo.pNext );
    }

    CommandBufferBeginInfo& pNext( const void* pNext )
    {
      m_commandBufferBeginInfo.pNext = pNext;
      return *this;
    }

    const CommandBufferUsageFlags& flags() const
    {
      return reinterpret_cast<const CommandBufferUsageFlags&>( m_commandBufferBeginInfo.flags );
    }

    CommandBufferUsageFlags& flags()
    {
      return reinterpret_cast<CommandBufferUsageFlags&>( m_commandBufferBeginInfo.flags );
    }

    CommandBufferBeginInfo& flags( CommandBufferUsageFlags flags )
    {
      m_commandBufferBeginInfo.flags = static_cast<VkCommandBufferUsageFlags>( flags );
      return *this;
    }

    const CommandBufferInheritanceInfo* pInheritanceInfo() const
    {
      return reinterpret_cast<const CommandBufferInheritanceInfo*>( m_commandBufferBeginInfo.pInheritanceInfo );
    }

    const CommandBufferInheritanceInfo* pInheritanceInfo()
    {
      return reinterpret_cast<const CommandBufferInheritanceInfo*>( m_commandBufferBeginInfo.pInheritanceInfo );
    }

    CommandBufferBeginInfo& pInheritanceInfo( const CommandBufferInheritanceInfo* pInheritanceInfo )
    {
      m_commandBufferBeginInfo.pInheritanceInfo = reinterpret_cast<const VkCommandBufferInheritanceInfo*>( pInheritanceInfo );
      return *this;
    }

    static CommandBufferBeginInfo& null()
    {
      return *((CommandBufferBeginInfo*)(nullptr));
    }

    operator const VkCommandBufferBeginInfo&() const
    {
      return m_commandBufferBeginInfo;
    }

  private:
    VkCommandBufferBeginInfo m_commandBufferBeginInfo;
  };
  static_assert( sizeof( CommandBufferBeginInfo ) == sizeof( VkCommandBufferBeginInfo ), "struct and wrapper have different size!" );

  class QueryPoolCreateInfo
  {
  public:
    QueryPoolCreateInfo()
      : QueryPoolCreateInfo( QueryPoolCreateFlags(), QueryType::eOcclusion, 0, QueryPipelineStatisticFlags() )
    {}

    QueryPoolCreateInfo( QueryPoolCreateFlags flags, QueryType queryType, uint32_t queryCount, QueryPipelineStatisticFlags pipelineStatistics)
    {
      m_queryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
      m_queryPoolCreateInfo.pNext = nullptr;
      m_queryPoolCreateInfo.flags = static_cast<VkQueryPoolCreateFlags>( flags );
      m_queryPoolCreateInfo.queryType = static_cast<VkQueryType>( queryType );
      m_queryPoolCreateInfo.queryCount = queryCount;
      m_queryPoolCreateInfo.pipelineStatistics = static_cast<VkQueryPipelineStatisticFlags>( pipelineStatistics );
    }

    QueryPoolCreateInfo(VkQueryPoolCreateInfo const & rhs)
      : m_queryPoolCreateInfo(rhs)
    {
    }

    QueryPoolCreateInfo& operator=(VkQueryPoolCreateInfo const & rhs)
    {
      m_queryPoolCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_queryPoolCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_queryPoolCreateInfo.sType );
    }

    QueryPoolCreateInfo& sType( StructureType sType )
    {
      m_queryPoolCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_queryPoolCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_queryPoolCreateInfo.pNext );
    }

    QueryPoolCreateInfo& pNext( const void* pNext )
    {
      m_queryPoolCreateInfo.pNext = pNext;
      return *this;
    }

    const QueryPoolCreateFlags& flags() const
    {
      return reinterpret_cast<const QueryPoolCreateFlags&>( m_queryPoolCreateInfo.flags );
    }

    QueryPoolCreateFlags& flags()
    {
      return reinterpret_cast<QueryPoolCreateFlags&>( m_queryPoolCreateInfo.flags );
    }

    QueryPoolCreateInfo& flags( QueryPoolCreateFlags flags )
    {
      m_queryPoolCreateInfo.flags = static_cast<VkQueryPoolCreateFlags>( flags );
      return *this;
    }

    const QueryType& queryType() const
    {
      return reinterpret_cast<const QueryType&>( m_queryPoolCreateInfo.queryType );
    }

    QueryType& queryType()
    {
      return reinterpret_cast<QueryType&>( m_queryPoolCreateInfo.queryType );
    }

    QueryPoolCreateInfo& queryType( QueryType queryType )
    {
      m_queryPoolCreateInfo.queryType = static_cast<VkQueryType>( queryType );
      return *this;
    }

    const uint32_t& queryCount() const
    {
      return m_queryPoolCreateInfo.queryCount;
    }

    uint32_t& queryCount()
    {
      return m_queryPoolCreateInfo.queryCount;
    }

    QueryPoolCreateInfo& queryCount( uint32_t queryCount )
    {
      m_queryPoolCreateInfo.queryCount = queryCount;
      return *this;
    }

    const QueryPipelineStatisticFlags& pipelineStatistics() const
    {
      return reinterpret_cast<const QueryPipelineStatisticFlags&>( m_queryPoolCreateInfo.pipelineStatistics );
    }

    QueryPipelineStatisticFlags& pipelineStatistics()
    {
      return reinterpret_cast<QueryPipelineStatisticFlags&>( m_queryPoolCreateInfo.pipelineStatistics );
    }

    QueryPoolCreateInfo& pipelineStatistics( QueryPipelineStatisticFlags pipelineStatistics )
    {
      m_queryPoolCreateInfo.pipelineStatistics = static_cast<VkQueryPipelineStatisticFlags>( pipelineStatistics );
      return *this;
    }

    static QueryPoolCreateInfo& null()
    {
      return *((QueryPoolCreateInfo*)(nullptr));
    }

    operator const VkQueryPoolCreateInfo&() const
    {
      return m_queryPoolCreateInfo;
    }

  private:
    VkQueryPoolCreateInfo m_queryPoolCreateInfo;
  };
  static_assert( sizeof( QueryPoolCreateInfo ) == sizeof( VkQueryPoolCreateInfo ), "struct and wrapper have different size!" );

  enum class ImageAspectFlagBits
  {
    eColor = VK_IMAGE_ASPECT_COLOR_BIT,
    eDepth = VK_IMAGE_ASPECT_DEPTH_BIT,
    eStencil = VK_IMAGE_ASPECT_STENCIL_BIT,
    eMetadata = VK_IMAGE_ASPECT_METADATA_BIT
  };

  typedef Flags<ImageAspectFlagBits, VkImageAspectFlags> ImageAspectFlags;

  inline ImageAspectFlags operator|( ImageAspectFlagBits bit0, ImageAspectFlagBits bit1 )
  {
    return ImageAspectFlags( bit0 ) | bit1;
  }

  class ImageSubresource
  {
  public:
    ImageSubresource()
      : ImageSubresource( ImageAspectFlags(), 0, 0 )
    {}

    ImageSubresource( ImageAspectFlags aspectMask, uint32_t mipLevel, uint32_t arrayLayer)
    {
      m_imageSubresource.aspectMask = static_cast<VkImageAspectFlags>( aspectMask );
      m_imageSubresource.mipLevel = mipLevel;
      m_imageSubresource.arrayLayer = arrayLayer;
    }

    ImageSubresource(VkImageSubresource const & rhs)
      : m_imageSubresource(rhs)
    {
    }

    ImageSubresource& operator=(VkImageSubresource const & rhs)
    {
      m_imageSubresource = rhs;
      return *this;
    }

    const ImageAspectFlags& aspectMask() const
    {
      return reinterpret_cast<const ImageAspectFlags&>( m_imageSubresource.aspectMask );
    }

    ImageAspectFlags& aspectMask()
    {
      return reinterpret_cast<ImageAspectFlags&>( m_imageSubresource.aspectMask );
    }

    ImageSubresource& aspectMask( ImageAspectFlags aspectMask )
    {
      m_imageSubresource.aspectMask = static_cast<VkImageAspectFlags>( aspectMask );
      return *this;
    }

    const uint32_t& mipLevel() const
    {
      return m_imageSubresource.mipLevel;
    }

    uint32_t& mipLevel()
    {
      return m_imageSubresource.mipLevel;
    }

    ImageSubresource& mipLevel( uint32_t mipLevel )
    {
      m_imageSubresource.mipLevel = mipLevel;
      return *this;
    }

    const uint32_t& arrayLayer() const
    {
      return m_imageSubresource.arrayLayer;
    }

    uint32_t& arrayLayer()
    {
      return m_imageSubresource.arrayLayer;
    }

    ImageSubresource& arrayLayer( uint32_t arrayLayer )
    {
      m_imageSubresource.arrayLayer = arrayLayer;
      return *this;
    }

    static ImageSubresource& null()
    {
      return *((ImageSubresource*)(nullptr));
    }

    operator const VkImageSubresource&() const
    {
      return m_imageSubresource;
    }

  private:
    VkImageSubresource m_imageSubresource;
  };
  static_assert( sizeof( ImageSubresource ) == sizeof( VkImageSubresource ), "struct and wrapper have different size!" );

  class ImageSubresourceLayers
  {
  public:
    ImageSubresourceLayers()
      : ImageSubresourceLayers( ImageAspectFlags(), 0, 0, 0 )
    {}

    ImageSubresourceLayers( ImageAspectFlags aspectMask, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount)
    {
      m_imageSubresourceLayers.aspectMask = static_cast<VkImageAspectFlags>( aspectMask );
      m_imageSubresourceLayers.mipLevel = mipLevel;
      m_imageSubresourceLayers.baseArrayLayer = baseArrayLayer;
      m_imageSubresourceLayers.layerCount = layerCount;
    }

    ImageSubresourceLayers(VkImageSubresourceLayers const & rhs)
      : m_imageSubresourceLayers(rhs)
    {
    }

    ImageSubresourceLayers& operator=(VkImageSubresourceLayers const & rhs)
    {
      m_imageSubresourceLayers = rhs;
      return *this;
    }

    const ImageAspectFlags& aspectMask() const
    {
      return reinterpret_cast<const ImageAspectFlags&>( m_imageSubresourceLayers.aspectMask );
    }

    ImageAspectFlags& aspectMask()
    {
      return reinterpret_cast<ImageAspectFlags&>( m_imageSubresourceLayers.aspectMask );
    }

    ImageSubresourceLayers& aspectMask( ImageAspectFlags aspectMask )
    {
      m_imageSubresourceLayers.aspectMask = static_cast<VkImageAspectFlags>( aspectMask );
      return *this;
    }

    const uint32_t& mipLevel() const
    {
      return m_imageSubresourceLayers.mipLevel;
    }

    uint32_t& mipLevel()
    {
      return m_imageSubresourceLayers.mipLevel;
    }

    ImageSubresourceLayers& mipLevel( uint32_t mipLevel )
    {
      m_imageSubresourceLayers.mipLevel = mipLevel;
      return *this;
    }

    const uint32_t& baseArrayLayer() const
    {
      return m_imageSubresourceLayers.baseArrayLayer;
    }

    uint32_t& baseArrayLayer()
    {
      return m_imageSubresourceLayers.baseArrayLayer;
    }

    ImageSubresourceLayers& baseArrayLayer( uint32_t baseArrayLayer )
    {
      m_imageSubresourceLayers.baseArrayLayer = baseArrayLayer;
      return *this;
    }

    const uint32_t& layerCount() const
    {
      return m_imageSubresourceLayers.layerCount;
    }

    uint32_t& layerCount()
    {
      return m_imageSubresourceLayers.layerCount;
    }

    ImageSubresourceLayers& layerCount( uint32_t layerCount )
    {
      m_imageSubresourceLayers.layerCount = layerCount;
      return *this;
    }

    static ImageSubresourceLayers& null()
    {
      return *((ImageSubresourceLayers*)(nullptr));
    }

    operator const VkImageSubresourceLayers&() const
    {
      return m_imageSubresourceLayers;
    }

  private:
    VkImageSubresourceLayers m_imageSubresourceLayers;
  };
  static_assert( sizeof( ImageSubresourceLayers ) == sizeof( VkImageSubresourceLayers ), "struct and wrapper have different size!" );

  class ImageSubresourceRange
  {
  public:
    ImageSubresourceRange()
      : ImageSubresourceRange( ImageAspectFlags(), 0, 0, 0, 0 )
    {}

    ImageSubresourceRange( ImageAspectFlags aspectMask, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
    {
      m_imageSubresourceRange.aspectMask = static_cast<VkImageAspectFlags>( aspectMask );
      m_imageSubresourceRange.baseMipLevel = baseMipLevel;
      m_imageSubresourceRange.levelCount = levelCount;
      m_imageSubresourceRange.baseArrayLayer = baseArrayLayer;
      m_imageSubresourceRange.layerCount = layerCount;
    }

    ImageSubresourceRange(VkImageSubresourceRange const & rhs)
      : m_imageSubresourceRange(rhs)
    {
    }

    ImageSubresourceRange& operator=(VkImageSubresourceRange const & rhs)
    {
      m_imageSubresourceRange = rhs;
      return *this;
    }

    const ImageAspectFlags& aspectMask() const
    {
      return reinterpret_cast<const ImageAspectFlags&>( m_imageSubresourceRange.aspectMask );
    }

    ImageAspectFlags& aspectMask()
    {
      return reinterpret_cast<ImageAspectFlags&>( m_imageSubresourceRange.aspectMask );
    }

    ImageSubresourceRange& aspectMask( ImageAspectFlags aspectMask )
    {
      m_imageSubresourceRange.aspectMask = static_cast<VkImageAspectFlags>( aspectMask );
      return *this;
    }

    const uint32_t& baseMipLevel() const
    {
      return m_imageSubresourceRange.baseMipLevel;
    }

    uint32_t& baseMipLevel()
    {
      return m_imageSubresourceRange.baseMipLevel;
    }

    ImageSubresourceRange& baseMipLevel( uint32_t baseMipLevel )
    {
      m_imageSubresourceRange.baseMipLevel = baseMipLevel;
      return *this;
    }

    const uint32_t& levelCount() const
    {
      return m_imageSubresourceRange.levelCount;
    }

    uint32_t& levelCount()
    {
      return m_imageSubresourceRange.levelCount;
    }

    ImageSubresourceRange& levelCount( uint32_t levelCount )
    {
      m_imageSubresourceRange.levelCount = levelCount;
      return *this;
    }

    const uint32_t& baseArrayLayer() const
    {
      return m_imageSubresourceRange.baseArrayLayer;
    }

    uint32_t& baseArrayLayer()
    {
      return m_imageSubresourceRange.baseArrayLayer;
    }

    ImageSubresourceRange& baseArrayLayer( uint32_t baseArrayLayer )
    {
      m_imageSubresourceRange.baseArrayLayer = baseArrayLayer;
      return *this;
    }

    const uint32_t& layerCount() const
    {
      return m_imageSubresourceRange.layerCount;
    }

    uint32_t& layerCount()
    {
      return m_imageSubresourceRange.layerCount;
    }

    ImageSubresourceRange& layerCount( uint32_t layerCount )
    {
      m_imageSubresourceRange.layerCount = layerCount;
      return *this;
    }

    static ImageSubresourceRange& null()
    {
      return *((ImageSubresourceRange*)(nullptr));
    }

    operator const VkImageSubresourceRange&() const
    {
      return m_imageSubresourceRange;
    }

  private:
    VkImageSubresourceRange m_imageSubresourceRange;
  };
  static_assert( sizeof( ImageSubresourceRange ) == sizeof( VkImageSubresourceRange ), "struct and wrapper have different size!" );

  class ImageMemoryBarrier
  {
  public:
    ImageMemoryBarrier()
      : ImageMemoryBarrier( AccessFlags(), AccessFlags(), ImageLayout::eUndefined, ImageLayout::eUndefined, 0, 0, Image(), ImageSubresourceRange() )
    {}

    ImageMemoryBarrier( AccessFlags srcAccessMask, AccessFlags dstAccessMask, ImageLayout oldLayout, ImageLayout newLayout, uint32_t srcQueueFamilyIndex, uint32_t dstQueueFamilyIndex, Image image, ImageSubresourceRange subresourceRange)
    {
      m_imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      m_imageMemoryBarrier.pNext = nullptr;
      m_imageMemoryBarrier.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask );
      m_imageMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask );
      m_imageMemoryBarrier.oldLayout = static_cast<VkImageLayout>( oldLayout );
      m_imageMemoryBarrier.newLayout = static_cast<VkImageLayout>( newLayout );
      m_imageMemoryBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex;
      m_imageMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
      m_imageMemoryBarrier.image = static_cast<VkImage>( image );
      m_imageMemoryBarrier.subresourceRange = static_cast<VkImageSubresourceRange>( subresourceRange );
    }

    ImageMemoryBarrier(VkImageMemoryBarrier const & rhs)
      : m_imageMemoryBarrier(rhs)
    {
    }

    ImageMemoryBarrier& operator=(VkImageMemoryBarrier const & rhs)
    {
      m_imageMemoryBarrier = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_imageMemoryBarrier.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_imageMemoryBarrier.sType );
    }

    ImageMemoryBarrier& sType( StructureType sType )
    {
      m_imageMemoryBarrier.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_imageMemoryBarrier.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_imageMemoryBarrier.pNext );
    }

    ImageMemoryBarrier& pNext( const void* pNext )
    {
      m_imageMemoryBarrier.pNext = pNext;
      return *this;
    }

    const AccessFlags& srcAccessMask() const
    {
      return reinterpret_cast<const AccessFlags&>( m_imageMemoryBarrier.srcAccessMask );
    }

    AccessFlags& srcAccessMask()
    {
      return reinterpret_cast<AccessFlags&>( m_imageMemoryBarrier.srcAccessMask );
    }

    ImageMemoryBarrier& srcAccessMask( AccessFlags srcAccessMask )
    {
      m_imageMemoryBarrier.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask );
      return *this;
    }

    const AccessFlags& dstAccessMask() const
    {
      return reinterpret_cast<const AccessFlags&>( m_imageMemoryBarrier.dstAccessMask );
    }

    AccessFlags& dstAccessMask()
    {
      return reinterpret_cast<AccessFlags&>( m_imageMemoryBarrier.dstAccessMask );
    }

    ImageMemoryBarrier& dstAccessMask( AccessFlags dstAccessMask )
    {
      m_imageMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask );
      return *this;
    }

    const ImageLayout& oldLayout() const
    {
      return reinterpret_cast<const ImageLayout&>( m_imageMemoryBarrier.oldLayout );
    }

    ImageLayout& oldLayout()
    {
      return reinterpret_cast<ImageLayout&>( m_imageMemoryBarrier.oldLayout );
    }

    ImageMemoryBarrier& oldLayout( ImageLayout oldLayout )
    {
      m_imageMemoryBarrier.oldLayout = static_cast<VkImageLayout>( oldLayout );
      return *this;
    }

    const ImageLayout& newLayout() const
    {
      return reinterpret_cast<const ImageLayout&>( m_imageMemoryBarrier.newLayout );
    }

    ImageLayout& newLayout()
    {
      return reinterpret_cast<ImageLayout&>( m_imageMemoryBarrier.newLayout );
    }

    ImageMemoryBarrier& newLayout( ImageLayout newLayout )
    {
      m_imageMemoryBarrier.newLayout = static_cast<VkImageLayout>( newLayout );
      return *this;
    }

    const uint32_t& srcQueueFamilyIndex() const
    {
      return m_imageMemoryBarrier.srcQueueFamilyIndex;
    }

    uint32_t& srcQueueFamilyIndex()
    {
      return m_imageMemoryBarrier.srcQueueFamilyIndex;
    }

    ImageMemoryBarrier& srcQueueFamilyIndex( uint32_t srcQueueFamilyIndex )
    {
      m_imageMemoryBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex;
      return *this;
    }

    const uint32_t& dstQueueFamilyIndex() const
    {
      return m_imageMemoryBarrier.dstQueueFamilyIndex;
    }

    uint32_t& dstQueueFamilyIndex()
    {
      return m_imageMemoryBarrier.dstQueueFamilyIndex;
    }

    ImageMemoryBarrier& dstQueueFamilyIndex( uint32_t dstQueueFamilyIndex )
    {
      m_imageMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
      return *this;
    }

    const Image& image() const
    {
      return reinterpret_cast<const Image&>( m_imageMemoryBarrier.image );
    }

    Image& image()
    {
      return reinterpret_cast<Image&>( m_imageMemoryBarrier.image );
    }

    ImageMemoryBarrier& image( Image image )
    {
      m_imageMemoryBarrier.image = static_cast<VkImage>( image );
      return *this;
    }

    const ImageSubresourceRange& subresourceRange() const
    {
      return reinterpret_cast<const ImageSubresourceRange&>( m_imageMemoryBarrier.subresourceRange );
    }

    ImageSubresourceRange& subresourceRange()
    {
      return reinterpret_cast<ImageSubresourceRange&>( m_imageMemoryBarrier.subresourceRange );
    }

    ImageMemoryBarrier& subresourceRange( ImageSubresourceRange subresourceRange )
    {
      m_imageMemoryBarrier.subresourceRange = static_cast<VkImageSubresourceRange>( subresourceRange );
      return *this;
    }

    static ImageMemoryBarrier& null()
    {
      return *((ImageMemoryBarrier*)(nullptr));
    }

    operator const VkImageMemoryBarrier&() const
    {
      return m_imageMemoryBarrier;
    }

  private:
    VkImageMemoryBarrier m_imageMemoryBarrier;
  };
  static_assert( sizeof( ImageMemoryBarrier ) == sizeof( VkImageMemoryBarrier ), "struct and wrapper have different size!" );

  class ImageViewCreateInfo
  {
  public:
    ImageViewCreateInfo()
      : ImageViewCreateInfo( ImageViewCreateFlags(), Image(), ImageViewType::e1D, Format::eUndefined, ComponentMapping(), ImageSubresourceRange() )
    {}

    ImageViewCreateInfo( ImageViewCreateFlags flags, Image image, ImageViewType viewType, Format format, ComponentMapping components, ImageSubresourceRange subresourceRange)
    {
      m_imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      m_imageViewCreateInfo.pNext = nullptr;
      m_imageViewCreateInfo.flags = static_cast<VkImageViewCreateFlags>( flags );
      m_imageViewCreateInfo.image = static_cast<VkImage>( image );
      m_imageViewCreateInfo.viewType = static_cast<VkImageViewType>( viewType );
      m_imageViewCreateInfo.format = static_cast<VkFormat>( format );
      m_imageViewCreateInfo.components = static_cast<VkComponentMapping>( components );
      m_imageViewCreateInfo.subresourceRange = static_cast<VkImageSubresourceRange>( subresourceRange );
    }

    ImageViewCreateInfo(VkImageViewCreateInfo const & rhs)
      : m_imageViewCreateInfo(rhs)
    {
    }

    ImageViewCreateInfo& operator=(VkImageViewCreateInfo const & rhs)
    {
      m_imageViewCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_imageViewCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_imageViewCreateInfo.sType );
    }

    ImageViewCreateInfo& sType( StructureType sType )
    {
      m_imageViewCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_imageViewCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_imageViewCreateInfo.pNext );
    }

    ImageViewCreateInfo& pNext( const void* pNext )
    {
      m_imageViewCreateInfo.pNext = pNext;
      return *this;
    }

    const ImageViewCreateFlags& flags() const
    {
      return reinterpret_cast<const ImageViewCreateFlags&>( m_imageViewCreateInfo.flags );
    }

    ImageViewCreateFlags& flags()
    {
      return reinterpret_cast<ImageViewCreateFlags&>( m_imageViewCreateInfo.flags );
    }

    ImageViewCreateInfo& flags( ImageViewCreateFlags flags )
    {
      m_imageViewCreateInfo.flags = static_cast<VkImageViewCreateFlags>( flags );
      return *this;
    }

    const Image& image() const
    {
      return reinterpret_cast<const Image&>( m_imageViewCreateInfo.image );
    }

    Image& image()
    {
      return reinterpret_cast<Image&>( m_imageViewCreateInfo.image );
    }

    ImageViewCreateInfo& image( Image image )
    {
      m_imageViewCreateInfo.image = static_cast<VkImage>( image );
      return *this;
    }

    const ImageViewType& viewType() const
    {
      return reinterpret_cast<const ImageViewType&>( m_imageViewCreateInfo.viewType );
    }

    ImageViewType& viewType()
    {
      return reinterpret_cast<ImageViewType&>( m_imageViewCreateInfo.viewType );
    }

    ImageViewCreateInfo& viewType( ImageViewType viewType )
    {
      m_imageViewCreateInfo.viewType = static_cast<VkImageViewType>( viewType );
      return *this;
    }

    const Format& format() const
    {
      return reinterpret_cast<const Format&>( m_imageViewCreateInfo.format );
    }

    Format& format()
    {
      return reinterpret_cast<Format&>( m_imageViewCreateInfo.format );
    }

    ImageViewCreateInfo& format( Format format )
    {
      m_imageViewCreateInfo.format = static_cast<VkFormat>( format );
      return *this;
    }

    const ComponentMapping& components() const
    {
      return reinterpret_cast<const ComponentMapping&>( m_imageViewCreateInfo.components );
    }

    ComponentMapping& components()
    {
      return reinterpret_cast<ComponentMapping&>( m_imageViewCreateInfo.components );
    }

    ImageViewCreateInfo& components( ComponentMapping components )
    {
      m_imageViewCreateInfo.components = static_cast<VkComponentMapping>( components );
      return *this;
    }

    const ImageSubresourceRange& subresourceRange() const
    {
      return reinterpret_cast<const ImageSubresourceRange&>( m_imageViewCreateInfo.subresourceRange );
    }

    ImageSubresourceRange& subresourceRange()
    {
      return reinterpret_cast<ImageSubresourceRange&>( m_imageViewCreateInfo.subresourceRange );
    }

    ImageViewCreateInfo& subresourceRange( ImageSubresourceRange subresourceRange )
    {
      m_imageViewCreateInfo.subresourceRange = static_cast<VkImageSubresourceRange>( subresourceRange );
      return *this;
    }

    static ImageViewCreateInfo& null()
    {
      return *((ImageViewCreateInfo*)(nullptr));
    }

    operator const VkImageViewCreateInfo&() const
    {
      return m_imageViewCreateInfo;
    }

  private:
    VkImageViewCreateInfo m_imageViewCreateInfo;
  };
  static_assert( sizeof( ImageViewCreateInfo ) == sizeof( VkImageViewCreateInfo ), "struct and wrapper have different size!" );

  class ImageCopy
  {
  public:
    ImageCopy()
      : ImageCopy( ImageSubresourceLayers(), Offset3D(), ImageSubresourceLayers(), Offset3D(), Extent3D() )
    {}

    ImageCopy( ImageSubresourceLayers srcSubresource, Offset3D srcOffset, ImageSubresourceLayers dstSubresource, Offset3D dstOffset, Extent3D extent)
    {
      m_imageCopy.srcSubresource = static_cast<VkImageSubresourceLayers>( srcSubresource );
      m_imageCopy.srcOffset = static_cast<VkOffset3D>( srcOffset );
      m_imageCopy.dstSubresource = static_cast<VkImageSubresourceLayers>( dstSubresource );
      m_imageCopy.dstOffset = static_cast<VkOffset3D>( dstOffset );
      m_imageCopy.extent = static_cast<VkExtent3D>( extent );
    }

    ImageCopy(VkImageCopy const & rhs)
      : m_imageCopy(rhs)
    {
    }

    ImageCopy& operator=(VkImageCopy const & rhs)
    {
      m_imageCopy = rhs;
      return *this;
    }

    const ImageSubresourceLayers& srcSubresource() const
    {
      return reinterpret_cast<const ImageSubresourceLayers&>( m_imageCopy.srcSubresource );
    }

    ImageSubresourceLayers& srcSubresource()
    {
      return reinterpret_cast<ImageSubresourceLayers&>( m_imageCopy.srcSubresource );
    }

    ImageCopy& srcSubresource( ImageSubresourceLayers srcSubresource )
    {
      m_imageCopy.srcSubresource = static_cast<VkImageSubresourceLayers>( srcSubresource );
      return *this;
    }

    const Offset3D& srcOffset() const
    {
      return reinterpret_cast<const Offset3D&>( m_imageCopy.srcOffset );
    }

    Offset3D& srcOffset()
    {
      return reinterpret_cast<Offset3D&>( m_imageCopy.srcOffset );
    }

    ImageCopy& srcOffset( Offset3D srcOffset )
    {
      m_imageCopy.srcOffset = static_cast<VkOffset3D>( srcOffset );
      return *this;
    }

    const ImageSubresourceLayers& dstSubresource() const
    {
      return reinterpret_cast<const ImageSubresourceLayers&>( m_imageCopy.dstSubresource );
    }

    ImageSubresourceLayers& dstSubresource()
    {
      return reinterpret_cast<ImageSubresourceLayers&>( m_imageCopy.dstSubresource );
    }

    ImageCopy& dstSubresource( ImageSubresourceLayers dstSubresource )
    {
      m_imageCopy.dstSubresource = static_cast<VkImageSubresourceLayers>( dstSubresource );
      return *this;
    }

    const Offset3D& dstOffset() const
    {
      return reinterpret_cast<const Offset3D&>( m_imageCopy.dstOffset );
    }

    Offset3D& dstOffset()
    {
      return reinterpret_cast<Offset3D&>( m_imageCopy.dstOffset );
    }

    ImageCopy& dstOffset( Offset3D dstOffset )
    {
      m_imageCopy.dstOffset = static_cast<VkOffset3D>( dstOffset );
      return *this;
    }

    const Extent3D& extent() const
    {
      return reinterpret_cast<const Extent3D&>( m_imageCopy.extent );
    }

    Extent3D& extent()
    {
      return reinterpret_cast<Extent3D&>( m_imageCopy.extent );
    }

    ImageCopy& extent( Extent3D extent )
    {
      m_imageCopy.extent = static_cast<VkExtent3D>( extent );
      return *this;
    }

    static ImageCopy& null()
    {
      return *((ImageCopy*)(nullptr));
    }

    operator const VkImageCopy&() const
    {
      return m_imageCopy;
    }

  private:
    VkImageCopy m_imageCopy;
  };
  static_assert( sizeof( ImageCopy ) == sizeof( VkImageCopy ), "struct and wrapper have different size!" );

  class ImageBlit
  {
  public:
    ImageBlit()
      : ImageBlit( ImageSubresourceLayers(), { Offset3D() }, ImageSubresourceLayers(), { Offset3D() } )
    {}

    ImageBlit( ImageSubresourceLayers srcSubresource, std::array<Offset3D,2> const& srcOffsets, ImageSubresourceLayers dstSubresource, std::array<Offset3D,2> const& dstOffsets)
    {
      m_imageBlit.srcSubresource = static_cast<VkImageSubresourceLayers>( srcSubresource );
      memcpy( &m_imageBlit.srcOffsets, srcOffsets.data(), 2 * sizeof( Offset3D ) );
      m_imageBlit.dstSubresource = static_cast<VkImageSubresourceLayers>( dstSubresource );
      memcpy( &m_imageBlit.dstOffsets, dstOffsets.data(), 2 * sizeof( Offset3D ) );
    }

    ImageBlit(VkImageBlit const & rhs)
      : m_imageBlit(rhs)
    {
    }

    ImageBlit& operator=(VkImageBlit const & rhs)
    {
      m_imageBlit = rhs;
      return *this;
    }

    const ImageSubresourceLayers& srcSubresource() const
    {
      return reinterpret_cast<const ImageSubresourceLayers&>( m_imageBlit.srcSubresource );
    }

    ImageSubresourceLayers& srcSubresource()
    {
      return reinterpret_cast<ImageSubresourceLayers&>( m_imageBlit.srcSubresource );
    }

    ImageBlit& srcSubresource( ImageSubresourceLayers srcSubresource )
    {
      m_imageBlit.srcSubresource = static_cast<VkImageSubresourceLayers>( srcSubresource );
      return *this;
    }

    const Offset3D* srcOffsets() const
    {
      return reinterpret_cast<const Offset3D*>( m_imageBlit.srcOffsets );
    }

    Offset3D* srcOffsets()
    {
      return reinterpret_cast<Offset3D*>( m_imageBlit.srcOffsets );
    }

    ImageBlit& srcOffsets( std::array<Offset3D,2> srcOffsets )
    {
      memcpy( &m_imageBlit.srcOffsets, srcOffsets.data(), 2 * sizeof( Offset3D ) );
      return *this;
    }

    const ImageSubresourceLayers& dstSubresource() const
    {
      return reinterpret_cast<const ImageSubresourceLayers&>( m_imageBlit.dstSubresource );
    }

    ImageSubresourceLayers& dstSubresource()
    {
      return reinterpret_cast<ImageSubresourceLayers&>( m_imageBlit.dstSubresource );
    }

    ImageBlit& dstSubresource( ImageSubresourceLayers dstSubresource )
    {
      m_imageBlit.dstSubresource = static_cast<VkImageSubresourceLayers>( dstSubresource );
      return *this;
    }

    const Offset3D* dstOffsets() const
    {
      return reinterpret_cast<const Offset3D*>( m_imageBlit.dstOffsets );
    }

    Offset3D* dstOffsets()
    {
      return reinterpret_cast<Offset3D*>( m_imageBlit.dstOffsets );
    }

    ImageBlit& dstOffsets( std::array<Offset3D,2> dstOffsets )
    {
      memcpy( &m_imageBlit.dstOffsets, dstOffsets.data(), 2 * sizeof( Offset3D ) );
      return *this;
    }

    static ImageBlit& null()
    {
      return *((ImageBlit*)(nullptr));
    }

    operator const VkImageBlit&() const
    {
      return m_imageBlit;
    }

  private:
    VkImageBlit m_imageBlit;
  };
  static_assert( sizeof( ImageBlit ) == sizeof( VkImageBlit ), "struct and wrapper have different size!" );

  class BufferImageCopy
  {
  public:
    BufferImageCopy()
      : BufferImageCopy( 0, 0, 0, ImageSubresourceLayers(), Offset3D(), Extent3D() )
    {}

    BufferImageCopy( DeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, ImageSubresourceLayers imageSubresource, Offset3D imageOffset, Extent3D imageExtent)
    {
      m_bufferImageCopy.bufferOffset = bufferOffset;
      m_bufferImageCopy.bufferRowLength = bufferRowLength;
      m_bufferImageCopy.bufferImageHeight = bufferImageHeight;
      m_bufferImageCopy.imageSubresource = static_cast<VkImageSubresourceLayers>( imageSubresource );
      m_bufferImageCopy.imageOffset = static_cast<VkOffset3D>( imageOffset );
      m_bufferImageCopy.imageExtent = static_cast<VkExtent3D>( imageExtent );
    }

    BufferImageCopy(VkBufferImageCopy const & rhs)
      : m_bufferImageCopy(rhs)
    {
    }

    BufferImageCopy& operator=(VkBufferImageCopy const & rhs)
    {
      m_bufferImageCopy = rhs;
      return *this;
    }

    const DeviceSize& bufferOffset() const
    {
      return m_bufferImageCopy.bufferOffset;
    }

    DeviceSize& bufferOffset()
    {
      return m_bufferImageCopy.bufferOffset;
    }

    BufferImageCopy& bufferOffset( DeviceSize bufferOffset )
    {
      m_bufferImageCopy.bufferOffset = bufferOffset;
      return *this;
    }

    const uint32_t& bufferRowLength() const
    {
      return m_bufferImageCopy.bufferRowLength;
    }

    uint32_t& bufferRowLength()
    {
      return m_bufferImageCopy.bufferRowLength;
    }

    BufferImageCopy& bufferRowLength( uint32_t bufferRowLength )
    {
      m_bufferImageCopy.bufferRowLength = bufferRowLength;
      return *this;
    }

    const uint32_t& bufferImageHeight() const
    {
      return m_bufferImageCopy.bufferImageHeight;
    }

    uint32_t& bufferImageHeight()
    {
      return m_bufferImageCopy.bufferImageHeight;
    }

    BufferImageCopy& bufferImageHeight( uint32_t bufferImageHeight )
    {
      m_bufferImageCopy.bufferImageHeight = bufferImageHeight;
      return *this;
    }

    const ImageSubresourceLayers& imageSubresource() const
    {
      return reinterpret_cast<const ImageSubresourceLayers&>( m_bufferImageCopy.imageSubresource );
    }

    ImageSubresourceLayers& imageSubresource()
    {
      return reinterpret_cast<ImageSubresourceLayers&>( m_bufferImageCopy.imageSubresource );
    }

    BufferImageCopy& imageSubresource( ImageSubresourceLayers imageSubresource )
    {
      m_bufferImageCopy.imageSubresource = static_cast<VkImageSubresourceLayers>( imageSubresource );
      return *this;
    }

    const Offset3D& imageOffset() const
    {
      return reinterpret_cast<const Offset3D&>( m_bufferImageCopy.imageOffset );
    }

    Offset3D& imageOffset()
    {
      return reinterpret_cast<Offset3D&>( m_bufferImageCopy.imageOffset );
    }

    BufferImageCopy& imageOffset( Offset3D imageOffset )
    {
      m_bufferImageCopy.imageOffset = static_cast<VkOffset3D>( imageOffset );
      return *this;
    }

    const Extent3D& imageExtent() const
    {
      return reinterpret_cast<const Extent3D&>( m_bufferImageCopy.imageExtent );
    }

    Extent3D& imageExtent()
    {
      return reinterpret_cast<Extent3D&>( m_bufferImageCopy.imageExtent );
    }

    BufferImageCopy& imageExtent( Extent3D imageExtent )
    {
      m_bufferImageCopy.imageExtent = static_cast<VkExtent3D>( imageExtent );
      return *this;
    }

    static BufferImageCopy& null()
    {
      return *((BufferImageCopy*)(nullptr));
    }

    operator const VkBufferImageCopy&() const
    {
      return m_bufferImageCopy;
    }

  private:
    VkBufferImageCopy m_bufferImageCopy;
  };
  static_assert( sizeof( BufferImageCopy ) == sizeof( VkBufferImageCopy ), "struct and wrapper have different size!" );

  class ImageResolve
  {
  public:
    ImageResolve()
      : ImageResolve( ImageSubresourceLayers(), Offset3D(), ImageSubresourceLayers(), Offset3D(), Extent3D() )
    {}

    ImageResolve( ImageSubresourceLayers srcSubresource, Offset3D srcOffset, ImageSubresourceLayers dstSubresource, Offset3D dstOffset, Extent3D extent)
    {
      m_imageResolve.srcSubresource = static_cast<VkImageSubresourceLayers>( srcSubresource );
      m_imageResolve.srcOffset = static_cast<VkOffset3D>( srcOffset );
      m_imageResolve.dstSubresource = static_cast<VkImageSubresourceLayers>( dstSubresource );
      m_imageResolve.dstOffset = static_cast<VkOffset3D>( dstOffset );
      m_imageResolve.extent = static_cast<VkExtent3D>( extent );
    }

    ImageResolve(VkImageResolve const & rhs)
      : m_imageResolve(rhs)
    {
    }

    ImageResolve& operator=(VkImageResolve const & rhs)
    {
      m_imageResolve = rhs;
      return *this;
    }

    const ImageSubresourceLayers& srcSubresource() const
    {
      return reinterpret_cast<const ImageSubresourceLayers&>( m_imageResolve.srcSubresource );
    }

    ImageSubresourceLayers& srcSubresource()
    {
      return reinterpret_cast<ImageSubresourceLayers&>( m_imageResolve.srcSubresource );
    }

    ImageResolve& srcSubresource( ImageSubresourceLayers srcSubresource )
    {
      m_imageResolve.srcSubresource = static_cast<VkImageSubresourceLayers>( srcSubresource );
      return *this;
    }

    const Offset3D& srcOffset() const
    {
      return reinterpret_cast<const Offset3D&>( m_imageResolve.srcOffset );
    }

    Offset3D& srcOffset()
    {
      return reinterpret_cast<Offset3D&>( m_imageResolve.srcOffset );
    }

    ImageResolve& srcOffset( Offset3D srcOffset )
    {
      m_imageResolve.srcOffset = static_cast<VkOffset3D>( srcOffset );
      return *this;
    }

    const ImageSubresourceLayers& dstSubresource() const
    {
      return reinterpret_cast<const ImageSubresourceLayers&>( m_imageResolve.dstSubresource );
    }

    ImageSubresourceLayers& dstSubresource()
    {
      return reinterpret_cast<ImageSubresourceLayers&>( m_imageResolve.dstSubresource );
    }

    ImageResolve& dstSubresource( ImageSubresourceLayers dstSubresource )
    {
      m_imageResolve.dstSubresource = static_cast<VkImageSubresourceLayers>( dstSubresource );
      return *this;
    }

    const Offset3D& dstOffset() const
    {
      return reinterpret_cast<const Offset3D&>( m_imageResolve.dstOffset );
    }

    Offset3D& dstOffset()
    {
      return reinterpret_cast<Offset3D&>( m_imageResolve.dstOffset );
    }

    ImageResolve& dstOffset( Offset3D dstOffset )
    {
      m_imageResolve.dstOffset = static_cast<VkOffset3D>( dstOffset );
      return *this;
    }

    const Extent3D& extent() const
    {
      return reinterpret_cast<const Extent3D&>( m_imageResolve.extent );
    }

    Extent3D& extent()
    {
      return reinterpret_cast<Extent3D&>( m_imageResolve.extent );
    }

    ImageResolve& extent( Extent3D extent )
    {
      m_imageResolve.extent = static_cast<VkExtent3D>( extent );
      return *this;
    }

    static ImageResolve& null()
    {
      return *((ImageResolve*)(nullptr));
    }

    operator const VkImageResolve&() const
    {
      return m_imageResolve;
    }

  private:
    VkImageResolve m_imageResolve;
  };
  static_assert( sizeof( ImageResolve ) == sizeof( VkImageResolve ), "struct and wrapper have different size!" );

  class ClearAttachment
  {
  public:
    ClearAttachment()
      : ClearAttachment( ImageAspectFlags(), 0, ClearValue() )
    {}

    ClearAttachment( ImageAspectFlags aspectMask, uint32_t colorAttachment, ClearValue clearValue)
    {
      m_clearAttachment.aspectMask = static_cast<VkImageAspectFlags>( aspectMask );
      m_clearAttachment.colorAttachment = colorAttachment;
      m_clearAttachment.clearValue = static_cast<VkClearValue>( clearValue );
    }

    ClearAttachment(VkClearAttachment const & rhs)
      : m_clearAttachment(rhs)
    {
    }

    ClearAttachment& operator=(VkClearAttachment const & rhs)
    {
      m_clearAttachment = rhs;
      return *this;
    }

    const ImageAspectFlags& aspectMask() const
    {
      return reinterpret_cast<const ImageAspectFlags&>( m_clearAttachment.aspectMask );
    }

    ImageAspectFlags& aspectMask()
    {
      return reinterpret_cast<ImageAspectFlags&>( m_clearAttachment.aspectMask );
    }

    ClearAttachment& aspectMask( ImageAspectFlags aspectMask )
    {
      m_clearAttachment.aspectMask = static_cast<VkImageAspectFlags>( aspectMask );
      return *this;
    }

    const uint32_t& colorAttachment() const
    {
      return m_clearAttachment.colorAttachment;
    }

    uint32_t& colorAttachment()
    {
      return m_clearAttachment.colorAttachment;
    }

    ClearAttachment& colorAttachment( uint32_t colorAttachment )
    {
      m_clearAttachment.colorAttachment = colorAttachment;
      return *this;
    }

    const ClearValue& clearValue() const
    {
      return reinterpret_cast<const ClearValue&>( m_clearAttachment.clearValue );
    }

    ClearValue& clearValue()
    {
      return reinterpret_cast<ClearValue&>( m_clearAttachment.clearValue );
    }

    ClearAttachment& clearValue( ClearValue clearValue )
    {
      m_clearAttachment.clearValue = static_cast<VkClearValue>( clearValue );
      return *this;
    }

    static ClearAttachment& null()
    {
      return *((ClearAttachment*)(nullptr));
    }

    operator const VkClearAttachment&() const
    {
      return m_clearAttachment;
    }

  private:
    VkClearAttachment m_clearAttachment;
  };
  static_assert( sizeof( ClearAttachment ) == sizeof( VkClearAttachment ), "struct and wrapper have different size!" );

  enum class SparseImageFormatFlagBits
  {
    eSingleMiptail = VK_SPARSE_IMAGE_FORMAT_SINGLE_MIPTAIL_BIT,
    eAlignedMipSize = VK_SPARSE_IMAGE_FORMAT_ALIGNED_MIP_SIZE_BIT,
    eNonstandardBlockSize = VK_SPARSE_IMAGE_FORMAT_NONSTANDARD_BLOCK_SIZE_BIT
  };

  typedef Flags<SparseImageFormatFlagBits, VkSparseImageFormatFlags> SparseImageFormatFlags;

  inline SparseImageFormatFlags operator|( SparseImageFormatFlagBits bit0, SparseImageFormatFlagBits bit1 )
  {
    return SparseImageFormatFlags( bit0 ) | bit1;
  }

  class SparseImageFormatProperties
  {
  public:
    const ImageAspectFlags& aspectMask() const
    {
      return reinterpret_cast<const ImageAspectFlags&>( m_sparseImageFormatProperties.aspectMask );
    }

    const Extent3D& imageGranularity() const
    {
      return reinterpret_cast<const Extent3D&>( m_sparseImageFormatProperties.imageGranularity );
    }

    const SparseImageFormatFlags& flags() const
    {
      return reinterpret_cast<const SparseImageFormatFlags&>( m_sparseImageFormatProperties.flags );
    }

    static SparseImageFormatProperties& null()
    {
      return *((SparseImageFormatProperties*)(nullptr));
    }

    operator const VkSparseImageFormatProperties&() const
    {
      return m_sparseImageFormatProperties;
    }

  private:
    VkSparseImageFormatProperties m_sparseImageFormatProperties;
  };
  static_assert( sizeof( SparseImageFormatProperties ) == sizeof( VkSparseImageFormatProperties ), "struct and wrapper have different size!" );

  class SparseImageMemoryRequirements
  {
  public:
    const SparseImageFormatProperties& formatProperties() const
    {
      return reinterpret_cast<const SparseImageFormatProperties&>( m_sparseImageMemoryRequirements.formatProperties );
    }

    const uint32_t& imageMipTailFirstLod() const
    {
      return m_sparseImageMemoryRequirements.imageMipTailFirstLod;
    }

    const DeviceSize& imageMipTailSize() const
    {
      return m_sparseImageMemoryRequirements.imageMipTailSize;
    }

    const DeviceSize& imageMipTailOffset() const
    {
      return m_sparseImageMemoryRequirements.imageMipTailOffset;
    }

    const DeviceSize& imageMipTailStride() const
    {
      return m_sparseImageMemoryRequirements.imageMipTailStride;
    }

    static SparseImageMemoryRequirements& null()
    {
      return *((SparseImageMemoryRequirements*)(nullptr));
    }

    operator const VkSparseImageMemoryRequirements&() const
    {
      return m_sparseImageMemoryRequirements;
    }

  private:
    VkSparseImageMemoryRequirements m_sparseImageMemoryRequirements;
  };
  static_assert( sizeof( SparseImageMemoryRequirements ) == sizeof( VkSparseImageMemoryRequirements ), "struct and wrapper have different size!" );

  enum class SparseMemoryBindFlagBits
  {
    eMetadata = VK_SPARSE_MEMORY_BIND_METADATA_BIT
  };

  typedef Flags<SparseMemoryBindFlagBits, VkSparseMemoryBindFlags> SparseMemoryBindFlags;

  inline SparseMemoryBindFlags operator|( SparseMemoryBindFlagBits bit0, SparseMemoryBindFlagBits bit1 )
  {
    return SparseMemoryBindFlags( bit0 ) | bit1;
  }

  class SparseMemoryBind
  {
  public:
    SparseMemoryBind()
      : SparseMemoryBind( 0, 0, DeviceMemory(), 0, SparseMemoryBindFlags() )
    {}

    SparseMemoryBind( DeviceSize resourceOffset, DeviceSize size, DeviceMemory memory, DeviceSize memoryOffset, SparseMemoryBindFlags flags)
    {
      m_sparseMemoryBind.resourceOffset = resourceOffset;
      m_sparseMemoryBind.size = size;
      m_sparseMemoryBind.memory = static_cast<VkDeviceMemory>( memory );
      m_sparseMemoryBind.memoryOffset = memoryOffset;
      m_sparseMemoryBind.flags = static_cast<VkSparseMemoryBindFlags>( flags );
    }

    SparseMemoryBind(VkSparseMemoryBind const & rhs)
      : m_sparseMemoryBind(rhs)
    {
    }

    SparseMemoryBind& operator=(VkSparseMemoryBind const & rhs)
    {
      m_sparseMemoryBind = rhs;
      return *this;
    }

    const DeviceSize& resourceOffset() const
    {
      return m_sparseMemoryBind.resourceOffset;
    }

    DeviceSize& resourceOffset()
    {
      return m_sparseMemoryBind.resourceOffset;
    }

    SparseMemoryBind& resourceOffset( DeviceSize resourceOffset )
    {
      m_sparseMemoryBind.resourceOffset = resourceOffset;
      return *this;
    }

    const DeviceSize& size() const
    {
      return m_sparseMemoryBind.size;
    }

    DeviceSize& size()
    {
      return m_sparseMemoryBind.size;
    }

    SparseMemoryBind& size( DeviceSize size )
    {
      m_sparseMemoryBind.size = size;
      return *this;
    }

    const DeviceMemory& memory() const
    {
      return reinterpret_cast<const DeviceMemory&>( m_sparseMemoryBind.memory );
    }

    DeviceMemory& memory()
    {
      return reinterpret_cast<DeviceMemory&>( m_sparseMemoryBind.memory );
    }

    SparseMemoryBind& memory( DeviceMemory memory )
    {
      m_sparseMemoryBind.memory = static_cast<VkDeviceMemory>( memory );
      return *this;
    }

    const DeviceSize& memoryOffset() const
    {
      return m_sparseMemoryBind.memoryOffset;
    }

    DeviceSize& memoryOffset()
    {
      return m_sparseMemoryBind.memoryOffset;
    }

    SparseMemoryBind& memoryOffset( DeviceSize memoryOffset )
    {
      m_sparseMemoryBind.memoryOffset = memoryOffset;
      return *this;
    }

    const SparseMemoryBindFlags& flags() const
    {
      return reinterpret_cast<const SparseMemoryBindFlags&>( m_sparseMemoryBind.flags );
    }

    SparseMemoryBindFlags& flags()
    {
      return reinterpret_cast<SparseMemoryBindFlags&>( m_sparseMemoryBind.flags );
    }

    SparseMemoryBind& flags( SparseMemoryBindFlags flags )
    {
      m_sparseMemoryBind.flags = static_cast<VkSparseMemoryBindFlags>( flags );
      return *this;
    }

    static SparseMemoryBind& null()
    {
      return *((SparseMemoryBind*)(nullptr));
    }

    operator const VkSparseMemoryBind&() const
    {
      return m_sparseMemoryBind;
    }

  private:
    VkSparseMemoryBind m_sparseMemoryBind;
  };
  static_assert( sizeof( SparseMemoryBind ) == sizeof( VkSparseMemoryBind ), "struct and wrapper have different size!" );

  class SparseImageMemoryBind
  {
  public:
    SparseImageMemoryBind()
      : SparseImageMemoryBind( ImageSubresource(), Offset3D(), Extent3D(), DeviceMemory(), 0, SparseMemoryBindFlags() )
    {}

    SparseImageMemoryBind( ImageSubresource subresource, Offset3D offset, Extent3D extent, DeviceMemory memory, DeviceSize memoryOffset, SparseMemoryBindFlags flags)
    {
      m_sparseImageMemoryBind.subresource = static_cast<VkImageSubresource>( subresource );
      m_sparseImageMemoryBind.offset = static_cast<VkOffset3D>( offset );
      m_sparseImageMemoryBind.extent = static_cast<VkExtent3D>( extent );
      m_sparseImageMemoryBind.memory = static_cast<VkDeviceMemory>( memory );
      m_sparseImageMemoryBind.memoryOffset = memoryOffset;
      m_sparseImageMemoryBind.flags = static_cast<VkSparseMemoryBindFlags>( flags );
    }

    SparseImageMemoryBind(VkSparseImageMemoryBind const & rhs)
      : m_sparseImageMemoryBind(rhs)
    {
    }

    SparseImageMemoryBind& operator=(VkSparseImageMemoryBind const & rhs)
    {
      m_sparseImageMemoryBind = rhs;
      return *this;
    }

    const ImageSubresource& subresource() const
    {
      return reinterpret_cast<const ImageSubresource&>( m_sparseImageMemoryBind.subresource );
    }

    ImageSubresource& subresource()
    {
      return reinterpret_cast<ImageSubresource&>( m_sparseImageMemoryBind.subresource );
    }

    SparseImageMemoryBind& subresource( ImageSubresource subresource )
    {
      m_sparseImageMemoryBind.subresource = static_cast<VkImageSubresource>( subresource );
      return *this;
    }

    const Offset3D& offset() const
    {
      return reinterpret_cast<const Offset3D&>( m_sparseImageMemoryBind.offset );
    }

    Offset3D& offset()
    {
      return reinterpret_cast<Offset3D&>( m_sparseImageMemoryBind.offset );
    }

    SparseImageMemoryBind& offset( Offset3D offset )
    {
      m_sparseImageMemoryBind.offset = static_cast<VkOffset3D>( offset );
      return *this;
    }

    const Extent3D& extent() const
    {
      return reinterpret_cast<const Extent3D&>( m_sparseImageMemoryBind.extent );
    }

    Extent3D& extent()
    {
      return reinterpret_cast<Extent3D&>( m_sparseImageMemoryBind.extent );
    }

    SparseImageMemoryBind& extent( Extent3D extent )
    {
      m_sparseImageMemoryBind.extent = static_cast<VkExtent3D>( extent );
      return *this;
    }

    const DeviceMemory& memory() const
    {
      return reinterpret_cast<const DeviceMemory&>( m_sparseImageMemoryBind.memory );
    }

    DeviceMemory& memory()
    {
      return reinterpret_cast<DeviceMemory&>( m_sparseImageMemoryBind.memory );
    }

    SparseImageMemoryBind& memory( DeviceMemory memory )
    {
      m_sparseImageMemoryBind.memory = static_cast<VkDeviceMemory>( memory );
      return *this;
    }

    const DeviceSize& memoryOffset() const
    {
      return m_sparseImageMemoryBind.memoryOffset;
    }

    DeviceSize& memoryOffset()
    {
      return m_sparseImageMemoryBind.memoryOffset;
    }

    SparseImageMemoryBind& memoryOffset( DeviceSize memoryOffset )
    {
      m_sparseImageMemoryBind.memoryOffset = memoryOffset;
      return *this;
    }

    const SparseMemoryBindFlags& flags() const
    {
      return reinterpret_cast<const SparseMemoryBindFlags&>( m_sparseImageMemoryBind.flags );
    }

    SparseMemoryBindFlags& flags()
    {
      return reinterpret_cast<SparseMemoryBindFlags&>( m_sparseImageMemoryBind.flags );
    }

    SparseImageMemoryBind& flags( SparseMemoryBindFlags flags )
    {
      m_sparseImageMemoryBind.flags = static_cast<VkSparseMemoryBindFlags>( flags );
      return *this;
    }

    static SparseImageMemoryBind& null()
    {
      return *((SparseImageMemoryBind*)(nullptr));
    }

    operator const VkSparseImageMemoryBind&() const
    {
      return m_sparseImageMemoryBind;
    }

  private:
    VkSparseImageMemoryBind m_sparseImageMemoryBind;
  };
  static_assert( sizeof( SparseImageMemoryBind ) == sizeof( VkSparseImageMemoryBind ), "struct and wrapper have different size!" );

  class SparseBufferMemoryBindInfo
  {
  public:
    SparseBufferMemoryBindInfo()
      : SparseBufferMemoryBindInfo( Buffer(), 0, nullptr )
    {}

    SparseBufferMemoryBindInfo( Buffer buffer, uint32_t bindCount, const SparseMemoryBind* pBinds)
    {
      m_sparseBufferMemoryBindInfo.buffer = static_cast<VkBuffer>( buffer );
      m_sparseBufferMemoryBindInfo.bindCount = bindCount;
      m_sparseBufferMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseMemoryBind*>( pBinds );
    }

    SparseBufferMemoryBindInfo(VkSparseBufferMemoryBindInfo const & rhs)
      : m_sparseBufferMemoryBindInfo(rhs)
    {
    }

    SparseBufferMemoryBindInfo& operator=(VkSparseBufferMemoryBindInfo const & rhs)
    {
      m_sparseBufferMemoryBindInfo = rhs;
      return *this;
    }

    const Buffer& buffer() const
    {
      return reinterpret_cast<const Buffer&>( m_sparseBufferMemoryBindInfo.buffer );
    }

    Buffer& buffer()
    {
      return reinterpret_cast<Buffer&>( m_sparseBufferMemoryBindInfo.buffer );
    }

    SparseBufferMemoryBindInfo& buffer( Buffer buffer )
    {
      m_sparseBufferMemoryBindInfo.buffer = static_cast<VkBuffer>( buffer );
      return *this;
    }

    const uint32_t& bindCount() const
    {
      return m_sparseBufferMemoryBindInfo.bindCount;
    }

    uint32_t& bindCount()
    {
      return m_sparseBufferMemoryBindInfo.bindCount;
    }

    SparseBufferMemoryBindInfo& bindCount( uint32_t bindCount )
    {
      m_sparseBufferMemoryBindInfo.bindCount = bindCount;
      return *this;
    }

    const SparseMemoryBind* pBinds() const
    {
      return reinterpret_cast<const SparseMemoryBind*>( m_sparseBufferMemoryBindInfo.pBinds );
    }

    const SparseMemoryBind* pBinds()
    {
      return reinterpret_cast<const SparseMemoryBind*>( m_sparseBufferMemoryBindInfo.pBinds );
    }

    SparseBufferMemoryBindInfo& pBinds( const SparseMemoryBind* pBinds )
    {
      m_sparseBufferMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseMemoryBind*>( pBinds );
      return *this;
    }

    static SparseBufferMemoryBindInfo& null()
    {
      return *((SparseBufferMemoryBindInfo*)(nullptr));
    }

    operator const VkSparseBufferMemoryBindInfo&() const
    {
      return m_sparseBufferMemoryBindInfo;
    }

  private:
    VkSparseBufferMemoryBindInfo m_sparseBufferMemoryBindInfo;
  };
  static_assert( sizeof( SparseBufferMemoryBindInfo ) == sizeof( VkSparseBufferMemoryBindInfo ), "struct and wrapper have different size!" );

  class SparseImageOpaqueMemoryBindInfo
  {
  public:
    SparseImageOpaqueMemoryBindInfo()
      : SparseImageOpaqueMemoryBindInfo( Image(), 0, nullptr )
    {}

    SparseImageOpaqueMemoryBindInfo( Image image, uint32_t bindCount, const SparseMemoryBind* pBinds)
    {
      m_sparseImageOpaqueMemoryBindInfo.image = static_cast<VkImage>( image );
      m_sparseImageOpaqueMemoryBindInfo.bindCount = bindCount;
      m_sparseImageOpaqueMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseMemoryBind*>( pBinds );
    }

    SparseImageOpaqueMemoryBindInfo(VkSparseImageOpaqueMemoryBindInfo const & rhs)
      : m_sparseImageOpaqueMemoryBindInfo(rhs)
    {
    }

    SparseImageOpaqueMemoryBindInfo& operator=(VkSparseImageOpaqueMemoryBindInfo const & rhs)
    {
      m_sparseImageOpaqueMemoryBindInfo = rhs;
      return *this;
    }

    const Image& image() const
    {
      return reinterpret_cast<const Image&>( m_sparseImageOpaqueMemoryBindInfo.image );
    }

    Image& image()
    {
      return reinterpret_cast<Image&>( m_sparseImageOpaqueMemoryBindInfo.image );
    }

    SparseImageOpaqueMemoryBindInfo& image( Image image )
    {
      m_sparseImageOpaqueMemoryBindInfo.image = static_cast<VkImage>( image );
      return *this;
    }

    const uint32_t& bindCount() const
    {
      return m_sparseImageOpaqueMemoryBindInfo.bindCount;
    }

    uint32_t& bindCount()
    {
      return m_sparseImageOpaqueMemoryBindInfo.bindCount;
    }

    SparseImageOpaqueMemoryBindInfo& bindCount( uint32_t bindCount )
    {
      m_sparseImageOpaqueMemoryBindInfo.bindCount = bindCount;
      return *this;
    }

    const SparseMemoryBind* pBinds() const
    {
      return reinterpret_cast<const SparseMemoryBind*>( m_sparseImageOpaqueMemoryBindInfo.pBinds );
    }

    const SparseMemoryBind* pBinds()
    {
      return reinterpret_cast<const SparseMemoryBind*>( m_sparseImageOpaqueMemoryBindInfo.pBinds );
    }

    SparseImageOpaqueMemoryBindInfo& pBinds( const SparseMemoryBind* pBinds )
    {
      m_sparseImageOpaqueMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseMemoryBind*>( pBinds );
      return *this;
    }

    static SparseImageOpaqueMemoryBindInfo& null()
    {
      return *((SparseImageOpaqueMemoryBindInfo*)(nullptr));
    }

    operator const VkSparseImageOpaqueMemoryBindInfo&() const
    {
      return m_sparseImageOpaqueMemoryBindInfo;
    }

  private:
    VkSparseImageOpaqueMemoryBindInfo m_sparseImageOpaqueMemoryBindInfo;
  };
  static_assert( sizeof( SparseImageOpaqueMemoryBindInfo ) == sizeof( VkSparseImageOpaqueMemoryBindInfo ), "struct and wrapper have different size!" );

  class SparseImageMemoryBindInfo
  {
  public:
    SparseImageMemoryBindInfo()
      : SparseImageMemoryBindInfo( Image(), 0, nullptr )
    {}

    SparseImageMemoryBindInfo( Image image, uint32_t bindCount, const SparseImageMemoryBind* pBinds)
    {
      m_sparseImageMemoryBindInfo.image = static_cast<VkImage>( image );
      m_sparseImageMemoryBindInfo.bindCount = bindCount;
      m_sparseImageMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseImageMemoryBind*>( pBinds );
    }

    SparseImageMemoryBindInfo(VkSparseImageMemoryBindInfo const & rhs)
      : m_sparseImageMemoryBindInfo(rhs)
    {
    }

    SparseImageMemoryBindInfo& operator=(VkSparseImageMemoryBindInfo const & rhs)
    {
      m_sparseImageMemoryBindInfo = rhs;
      return *this;
    }

    const Image& image() const
    {
      return reinterpret_cast<const Image&>( m_sparseImageMemoryBindInfo.image );
    }

    Image& image()
    {
      return reinterpret_cast<Image&>( m_sparseImageMemoryBindInfo.image );
    }

    SparseImageMemoryBindInfo& image( Image image )
    {
      m_sparseImageMemoryBindInfo.image = static_cast<VkImage>( image );
      return *this;
    }

    const uint32_t& bindCount() const
    {
      return m_sparseImageMemoryBindInfo.bindCount;
    }

    uint32_t& bindCount()
    {
      return m_sparseImageMemoryBindInfo.bindCount;
    }

    SparseImageMemoryBindInfo& bindCount( uint32_t bindCount )
    {
      m_sparseImageMemoryBindInfo.bindCount = bindCount;
      return *this;
    }

    const SparseImageMemoryBind* pBinds() const
    {
      return reinterpret_cast<const SparseImageMemoryBind*>( m_sparseImageMemoryBindInfo.pBinds );
    }

    const SparseImageMemoryBind* pBinds()
    {
      return reinterpret_cast<const SparseImageMemoryBind*>( m_sparseImageMemoryBindInfo.pBinds );
    }

    SparseImageMemoryBindInfo& pBinds( const SparseImageMemoryBind* pBinds )
    {
      m_sparseImageMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseImageMemoryBind*>( pBinds );
      return *this;
    }

    static SparseImageMemoryBindInfo& null()
    {
      return *((SparseImageMemoryBindInfo*)(nullptr));
    }

    operator const VkSparseImageMemoryBindInfo&() const
    {
      return m_sparseImageMemoryBindInfo;
    }

  private:
    VkSparseImageMemoryBindInfo m_sparseImageMemoryBindInfo;
  };
  static_assert( sizeof( SparseImageMemoryBindInfo ) == sizeof( VkSparseImageMemoryBindInfo ), "struct and wrapper have different size!" );

  class BindSparseInfo
  {
  public:
    BindSparseInfo()
      : BindSparseInfo( 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr )
    {}

    BindSparseInfo( uint32_t waitSemaphoreCount, const Semaphore* pWaitSemaphores, uint32_t bufferBindCount, const SparseBufferMemoryBindInfo* pBufferBinds, uint32_t imageOpaqueBindCount, const SparseImageOpaqueMemoryBindInfo* pImageOpaqueBinds, uint32_t imageBindCount, const SparseImageMemoryBindInfo* pImageBinds, uint32_t signalSemaphoreCount, const Semaphore* pSignalSemaphores)
    {
      m_bindSparseInfo.sType = VK_STRUCTURE_TYPE_BIND_SPARSE_INFO;
      m_bindSparseInfo.pNext = nullptr;
      m_bindSparseInfo.waitSemaphoreCount = waitSemaphoreCount;
      m_bindSparseInfo.pWaitSemaphores = reinterpret_cast<const VkSemaphore*>( pWaitSemaphores );
      m_bindSparseInfo.bufferBindCount = bufferBindCount;
      m_bindSparseInfo.pBufferBinds = reinterpret_cast<const VkSparseBufferMemoryBindInfo*>( pBufferBinds );
      m_bindSparseInfo.imageOpaqueBindCount = imageOpaqueBindCount;
      m_bindSparseInfo.pImageOpaqueBinds = reinterpret_cast<const VkSparseImageOpaqueMemoryBindInfo*>( pImageOpaqueBinds );
      m_bindSparseInfo.imageBindCount = imageBindCount;
      m_bindSparseInfo.pImageBinds = reinterpret_cast<const VkSparseImageMemoryBindInfo*>( pImageBinds );
      m_bindSparseInfo.signalSemaphoreCount = signalSemaphoreCount;
      m_bindSparseInfo.pSignalSemaphores = reinterpret_cast<const VkSemaphore*>( pSignalSemaphores );
    }

    BindSparseInfo(VkBindSparseInfo const & rhs)
      : m_bindSparseInfo(rhs)
    {
    }

    BindSparseInfo& operator=(VkBindSparseInfo const & rhs)
    {
      m_bindSparseInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_bindSparseInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_bindSparseInfo.sType );
    }

    BindSparseInfo& sType( StructureType sType )
    {
      m_bindSparseInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_bindSparseInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_bindSparseInfo.pNext );
    }

    BindSparseInfo& pNext( const void* pNext )
    {
      m_bindSparseInfo.pNext = pNext;
      return *this;
    }

    const uint32_t& waitSemaphoreCount() const
    {
      return m_bindSparseInfo.waitSemaphoreCount;
    }

    uint32_t& waitSemaphoreCount()
    {
      return m_bindSparseInfo.waitSemaphoreCount;
    }

    BindSparseInfo& waitSemaphoreCount( uint32_t waitSemaphoreCount )
    {
      m_bindSparseInfo.waitSemaphoreCount = waitSemaphoreCount;
      return *this;
    }

    const Semaphore* pWaitSemaphores() const
    {
      return reinterpret_cast<const Semaphore*>( m_bindSparseInfo.pWaitSemaphores );
    }

    const Semaphore* pWaitSemaphores()
    {
      return reinterpret_cast<const Semaphore*>( m_bindSparseInfo.pWaitSemaphores );
    }

    BindSparseInfo& pWaitSemaphores( const Semaphore* pWaitSemaphores )
    {
      m_bindSparseInfo.pWaitSemaphores = reinterpret_cast<const VkSemaphore*>( pWaitSemaphores );
      return *this;
    }

    const uint32_t& bufferBindCount() const
    {
      return m_bindSparseInfo.bufferBindCount;
    }

    uint32_t& bufferBindCount()
    {
      return m_bindSparseInfo.bufferBindCount;
    }

    BindSparseInfo& bufferBindCount( uint32_t bufferBindCount )
    {
      m_bindSparseInfo.bufferBindCount = bufferBindCount;
      return *this;
    }

    const SparseBufferMemoryBindInfo* pBufferBinds() const
    {
      return reinterpret_cast<const SparseBufferMemoryBindInfo*>( m_bindSparseInfo.pBufferBinds );
    }

    const SparseBufferMemoryBindInfo* pBufferBinds()
    {
      return reinterpret_cast<const SparseBufferMemoryBindInfo*>( m_bindSparseInfo.pBufferBinds );
    }

    BindSparseInfo& pBufferBinds( const SparseBufferMemoryBindInfo* pBufferBinds )
    {
      m_bindSparseInfo.pBufferBinds = reinterpret_cast<const VkSparseBufferMemoryBindInfo*>( pBufferBinds );
      return *this;
    }

    const uint32_t& imageOpaqueBindCount() const
    {
      return m_bindSparseInfo.imageOpaqueBindCount;
    }

    uint32_t& imageOpaqueBindCount()
    {
      return m_bindSparseInfo.imageOpaqueBindCount;
    }

    BindSparseInfo& imageOpaqueBindCount( uint32_t imageOpaqueBindCount )
    {
      m_bindSparseInfo.imageOpaqueBindCount = imageOpaqueBindCount;
      return *this;
    }

    const SparseImageOpaqueMemoryBindInfo* pImageOpaqueBinds() const
    {
      return reinterpret_cast<const SparseImageOpaqueMemoryBindInfo*>( m_bindSparseInfo.pImageOpaqueBinds );
    }

    const SparseImageOpaqueMemoryBindInfo* pImageOpaqueBinds()
    {
      return reinterpret_cast<const SparseImageOpaqueMemoryBindInfo*>( m_bindSparseInfo.pImageOpaqueBinds );
    }

    BindSparseInfo& pImageOpaqueBinds( const SparseImageOpaqueMemoryBindInfo* pImageOpaqueBinds )
    {
      m_bindSparseInfo.pImageOpaqueBinds = reinterpret_cast<const VkSparseImageOpaqueMemoryBindInfo*>( pImageOpaqueBinds );
      return *this;
    }

    const uint32_t& imageBindCount() const
    {
      return m_bindSparseInfo.imageBindCount;
    }

    uint32_t& imageBindCount()
    {
      return m_bindSparseInfo.imageBindCount;
    }

    BindSparseInfo& imageBindCount( uint32_t imageBindCount )
    {
      m_bindSparseInfo.imageBindCount = imageBindCount;
      return *this;
    }

    const SparseImageMemoryBindInfo* pImageBinds() const
    {
      return reinterpret_cast<const SparseImageMemoryBindInfo*>( m_bindSparseInfo.pImageBinds );
    }

    const SparseImageMemoryBindInfo* pImageBinds()
    {
      return reinterpret_cast<const SparseImageMemoryBindInfo*>( m_bindSparseInfo.pImageBinds );
    }

    BindSparseInfo& pImageBinds( const SparseImageMemoryBindInfo* pImageBinds )
    {
      m_bindSparseInfo.pImageBinds = reinterpret_cast<const VkSparseImageMemoryBindInfo*>( pImageBinds );
      return *this;
    }

    const uint32_t& signalSemaphoreCount() const
    {
      return m_bindSparseInfo.signalSemaphoreCount;
    }

    uint32_t& signalSemaphoreCount()
    {
      return m_bindSparseInfo.signalSemaphoreCount;
    }

    BindSparseInfo& signalSemaphoreCount( uint32_t signalSemaphoreCount )
    {
      m_bindSparseInfo.signalSemaphoreCount = signalSemaphoreCount;
      return *this;
    }

    const Semaphore* pSignalSemaphores() const
    {
      return reinterpret_cast<const Semaphore*>( m_bindSparseInfo.pSignalSemaphores );
    }

    const Semaphore* pSignalSemaphores()
    {
      return reinterpret_cast<const Semaphore*>( m_bindSparseInfo.pSignalSemaphores );
    }

    BindSparseInfo& pSignalSemaphores( const Semaphore* pSignalSemaphores )
    {
      m_bindSparseInfo.pSignalSemaphores = reinterpret_cast<const VkSemaphore*>( pSignalSemaphores );
      return *this;
    }

    static BindSparseInfo& null()
    {
      return *((BindSparseInfo*)(nullptr));
    }

    operator const VkBindSparseInfo&() const
    {
      return m_bindSparseInfo;
    }

  private:
    VkBindSparseInfo m_bindSparseInfo;
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
    eAllCommands = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
  };

  typedef Flags<PipelineStageFlagBits, VkPipelineStageFlags> PipelineStageFlags;

  inline PipelineStageFlags operator|( PipelineStageFlagBits bit0, PipelineStageFlagBits bit1 )
  {
    return PipelineStageFlags( bit0 ) | bit1;
  }

  enum class CommandPoolCreateFlagBits
  {
    eTransient = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
    eResetCommandBuffer = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
  };

  typedef Flags<CommandPoolCreateFlagBits, VkCommandPoolCreateFlags> CommandPoolCreateFlags;

  inline CommandPoolCreateFlags operator|( CommandPoolCreateFlagBits bit0, CommandPoolCreateFlagBits bit1 )
  {
    return CommandPoolCreateFlags( bit0 ) | bit1;
  }

  class CommandPoolCreateInfo
  {
  public:
    CommandPoolCreateInfo()
      : CommandPoolCreateInfo( CommandPoolCreateFlags(), 0 )
    {}

    CommandPoolCreateInfo( CommandPoolCreateFlags flags, uint32_t queueFamilyIndex)
    {
      m_commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      m_commandPoolCreateInfo.pNext = nullptr;
      m_commandPoolCreateInfo.flags = static_cast<VkCommandPoolCreateFlags>( flags );
      m_commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
    }

    CommandPoolCreateInfo(VkCommandPoolCreateInfo const & rhs)
      : m_commandPoolCreateInfo(rhs)
    {
    }

    CommandPoolCreateInfo& operator=(VkCommandPoolCreateInfo const & rhs)
    {
      m_commandPoolCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_commandPoolCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_commandPoolCreateInfo.sType );
    }

    CommandPoolCreateInfo& sType( StructureType sType )
    {
      m_commandPoolCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_commandPoolCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_commandPoolCreateInfo.pNext );
    }

    CommandPoolCreateInfo& pNext( const void* pNext )
    {
      m_commandPoolCreateInfo.pNext = pNext;
      return *this;
    }

    const CommandPoolCreateFlags& flags() const
    {
      return reinterpret_cast<const CommandPoolCreateFlags&>( m_commandPoolCreateInfo.flags );
    }

    CommandPoolCreateFlags& flags()
    {
      return reinterpret_cast<CommandPoolCreateFlags&>( m_commandPoolCreateInfo.flags );
    }

    CommandPoolCreateInfo& flags( CommandPoolCreateFlags flags )
    {
      m_commandPoolCreateInfo.flags = static_cast<VkCommandPoolCreateFlags>( flags );
      return *this;
    }

    const uint32_t& queueFamilyIndex() const
    {
      return m_commandPoolCreateInfo.queueFamilyIndex;
    }

    uint32_t& queueFamilyIndex()
    {
      return m_commandPoolCreateInfo.queueFamilyIndex;
    }

    CommandPoolCreateInfo& queueFamilyIndex( uint32_t queueFamilyIndex )
    {
      m_commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
      return *this;
    }

    static CommandPoolCreateInfo& null()
    {
      return *((CommandPoolCreateInfo*)(nullptr));
    }

    operator const VkCommandPoolCreateInfo&() const
    {
      return m_commandPoolCreateInfo;
    }

  private:
    VkCommandPoolCreateInfo m_commandPoolCreateInfo;
  };
  static_assert( sizeof( CommandPoolCreateInfo ) == sizeof( VkCommandPoolCreateInfo ), "struct and wrapper have different size!" );

  enum class CommandPoolResetFlagBits
  {
    eReleaseResources = VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT
  };

  typedef Flags<CommandPoolResetFlagBits, VkCommandPoolResetFlags> CommandPoolResetFlags;

  inline CommandPoolResetFlags operator|( CommandPoolResetFlagBits bit0, CommandPoolResetFlagBits bit1 )
  {
    return CommandPoolResetFlags( bit0 ) | bit1;
  }

  enum class CommandBufferResetFlagBits
  {
    eReleaseResources = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT
  };

  typedef Flags<CommandBufferResetFlagBits, VkCommandBufferResetFlags> CommandBufferResetFlags;

  inline CommandBufferResetFlags operator|( CommandBufferResetFlagBits bit0, CommandBufferResetFlagBits bit1 )
  {
    return CommandBufferResetFlags( bit0 ) | bit1;
  }

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

  typedef Flags<SampleCountFlagBits, VkSampleCountFlags> SampleCountFlags;

  inline SampleCountFlags operator|( SampleCountFlagBits bit0, SampleCountFlagBits bit1 )
  {
    return SampleCountFlags( bit0 ) | bit1;
  }

  class ImageFormatProperties
  {
  public:
    const Extent3D& maxExtent() const
    {
      return reinterpret_cast<const Extent3D&>( m_imageFormatProperties.maxExtent );
    }

    const uint32_t& maxMipLevels() const
    {
      return m_imageFormatProperties.maxMipLevels;
    }

    const uint32_t& maxArrayLayers() const
    {
      return m_imageFormatProperties.maxArrayLayers;
    }

    const SampleCountFlags& sampleCounts() const
    {
      return reinterpret_cast<const SampleCountFlags&>( m_imageFormatProperties.sampleCounts );
    }

    const DeviceSize& maxResourceSize() const
    {
      return m_imageFormatProperties.maxResourceSize;
    }

    static ImageFormatProperties& null()
    {
      return *((ImageFormatProperties*)(nullptr));
    }

    operator const VkImageFormatProperties&() const
    {
      return m_imageFormatProperties;
    }

  private:
    VkImageFormatProperties m_imageFormatProperties;
  };
  static_assert( sizeof( ImageFormatProperties ) == sizeof( VkImageFormatProperties ), "struct and wrapper have different size!" );

  class ImageCreateInfo
  {
  public:
    ImageCreateInfo()
      : ImageCreateInfo( ImageCreateFlags(), ImageType::e1D, Format::eUndefined, Extent3D(), 0, 0, SampleCountFlagBits::e1, ImageTiling::eOptimal, ImageUsageFlags(), SharingMode::eExclusive, 0, nullptr, ImageLayout::eUndefined )
    {}

    ImageCreateInfo( ImageCreateFlags flags, ImageType imageType, Format format, Extent3D extent, uint32_t mipLevels, uint32_t arrayLayers, SampleCountFlagBits samples, ImageTiling tiling, ImageUsageFlags usage, SharingMode sharingMode, uint32_t queueFamilyIndexCount, const uint32_t* pQueueFamilyIndices, ImageLayout initialLayout)
    {
      m_imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
      m_imageCreateInfo.pNext = nullptr;
      m_imageCreateInfo.flags = static_cast<VkImageCreateFlags>( flags );
      m_imageCreateInfo.imageType = static_cast<VkImageType>( imageType );
      m_imageCreateInfo.format = static_cast<VkFormat>( format );
      m_imageCreateInfo.extent = static_cast<VkExtent3D>( extent );
      m_imageCreateInfo.mipLevels = mipLevels;
      m_imageCreateInfo.arrayLayers = arrayLayers;
      m_imageCreateInfo.samples = static_cast<VkSampleCountFlagBits>( samples );
      m_imageCreateInfo.tiling = static_cast<VkImageTiling>( tiling );
      m_imageCreateInfo.usage = static_cast<VkImageUsageFlags>( usage );
      m_imageCreateInfo.sharingMode = static_cast<VkSharingMode>( sharingMode );
      m_imageCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount;
      m_imageCreateInfo.pQueueFamilyIndices = pQueueFamilyIndices;
      m_imageCreateInfo.initialLayout = static_cast<VkImageLayout>( initialLayout );
    }

    ImageCreateInfo(VkImageCreateInfo const & rhs)
      : m_imageCreateInfo(rhs)
    {
    }

    ImageCreateInfo& operator=(VkImageCreateInfo const & rhs)
    {
      m_imageCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_imageCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_imageCreateInfo.sType );
    }

    ImageCreateInfo& sType( StructureType sType )
    {
      m_imageCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_imageCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_imageCreateInfo.pNext );
    }

    ImageCreateInfo& pNext( const void* pNext )
    {
      m_imageCreateInfo.pNext = pNext;
      return *this;
    }

    const ImageCreateFlags& flags() const
    {
      return reinterpret_cast<const ImageCreateFlags&>( m_imageCreateInfo.flags );
    }

    ImageCreateFlags& flags()
    {
      return reinterpret_cast<ImageCreateFlags&>( m_imageCreateInfo.flags );
    }

    ImageCreateInfo& flags( ImageCreateFlags flags )
    {
      m_imageCreateInfo.flags = static_cast<VkImageCreateFlags>( flags );
      return *this;
    }

    const ImageType& imageType() const
    {
      return reinterpret_cast<const ImageType&>( m_imageCreateInfo.imageType );
    }

    ImageType& imageType()
    {
      return reinterpret_cast<ImageType&>( m_imageCreateInfo.imageType );
    }

    ImageCreateInfo& imageType( ImageType imageType )
    {
      m_imageCreateInfo.imageType = static_cast<VkImageType>( imageType );
      return *this;
    }

    const Format& format() const
    {
      return reinterpret_cast<const Format&>( m_imageCreateInfo.format );
    }

    Format& format()
    {
      return reinterpret_cast<Format&>( m_imageCreateInfo.format );
    }

    ImageCreateInfo& format( Format format )
    {
      m_imageCreateInfo.format = static_cast<VkFormat>( format );
      return *this;
    }

    const Extent3D& extent() const
    {
      return reinterpret_cast<const Extent3D&>( m_imageCreateInfo.extent );
    }

    Extent3D& extent()
    {
      return reinterpret_cast<Extent3D&>( m_imageCreateInfo.extent );
    }

    ImageCreateInfo& extent( Extent3D extent )
    {
      m_imageCreateInfo.extent = static_cast<VkExtent3D>( extent );
      return *this;
    }

    const uint32_t& mipLevels() const
    {
      return m_imageCreateInfo.mipLevels;
    }

    uint32_t& mipLevels()
    {
      return m_imageCreateInfo.mipLevels;
    }

    ImageCreateInfo& mipLevels( uint32_t mipLevels )
    {
      m_imageCreateInfo.mipLevels = mipLevels;
      return *this;
    }

    const uint32_t& arrayLayers() const
    {
      return m_imageCreateInfo.arrayLayers;
    }

    uint32_t& arrayLayers()
    {
      return m_imageCreateInfo.arrayLayers;
    }

    ImageCreateInfo& arrayLayers( uint32_t arrayLayers )
    {
      m_imageCreateInfo.arrayLayers = arrayLayers;
      return *this;
    }

    const SampleCountFlagBits& samples() const
    {
      return reinterpret_cast<const SampleCountFlagBits&>( m_imageCreateInfo.samples );
    }

    SampleCountFlagBits& samples()
    {
      return reinterpret_cast<SampleCountFlagBits&>( m_imageCreateInfo.samples );
    }

    ImageCreateInfo& samples( SampleCountFlagBits samples )
    {
      m_imageCreateInfo.samples = static_cast<VkSampleCountFlagBits>( samples );
      return *this;
    }

    const ImageTiling& tiling() const
    {
      return reinterpret_cast<const ImageTiling&>( m_imageCreateInfo.tiling );
    }

    ImageTiling& tiling()
    {
      return reinterpret_cast<ImageTiling&>( m_imageCreateInfo.tiling );
    }

    ImageCreateInfo& tiling( ImageTiling tiling )
    {
      m_imageCreateInfo.tiling = static_cast<VkImageTiling>( tiling );
      return *this;
    }

    const ImageUsageFlags& usage() const
    {
      return reinterpret_cast<const ImageUsageFlags&>( m_imageCreateInfo.usage );
    }

    ImageUsageFlags& usage()
    {
      return reinterpret_cast<ImageUsageFlags&>( m_imageCreateInfo.usage );
    }

    ImageCreateInfo& usage( ImageUsageFlags usage )
    {
      m_imageCreateInfo.usage = static_cast<VkImageUsageFlags>( usage );
      return *this;
    }

    const SharingMode& sharingMode() const
    {
      return reinterpret_cast<const SharingMode&>( m_imageCreateInfo.sharingMode );
    }

    SharingMode& sharingMode()
    {
      return reinterpret_cast<SharingMode&>( m_imageCreateInfo.sharingMode );
    }

    ImageCreateInfo& sharingMode( SharingMode sharingMode )
    {
      m_imageCreateInfo.sharingMode = static_cast<VkSharingMode>( sharingMode );
      return *this;
    }

    const uint32_t& queueFamilyIndexCount() const
    {
      return m_imageCreateInfo.queueFamilyIndexCount;
    }

    uint32_t& queueFamilyIndexCount()
    {
      return m_imageCreateInfo.queueFamilyIndexCount;
    }

    ImageCreateInfo& queueFamilyIndexCount( uint32_t queueFamilyIndexCount )
    {
      m_imageCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount;
      return *this;
    }

    const uint32_t* pQueueFamilyIndices() const
    {
      return reinterpret_cast<const uint32_t*>( m_imageCreateInfo.pQueueFamilyIndices );
    }

    const uint32_t* pQueueFamilyIndices()
    {
      return reinterpret_cast<const uint32_t*>( m_imageCreateInfo.pQueueFamilyIndices );
    }

    ImageCreateInfo& pQueueFamilyIndices( const uint32_t* pQueueFamilyIndices )
    {
      m_imageCreateInfo.pQueueFamilyIndices = pQueueFamilyIndices;
      return *this;
    }

    const ImageLayout& initialLayout() const
    {
      return reinterpret_cast<const ImageLayout&>( m_imageCreateInfo.initialLayout );
    }

    ImageLayout& initialLayout()
    {
      return reinterpret_cast<ImageLayout&>( m_imageCreateInfo.initialLayout );
    }

    ImageCreateInfo& initialLayout( ImageLayout initialLayout )
    {
      m_imageCreateInfo.initialLayout = static_cast<VkImageLayout>( initialLayout );
      return *this;
    }

    static ImageCreateInfo& null()
    {
      return *((ImageCreateInfo*)(nullptr));
    }

    operator const VkImageCreateInfo&() const
    {
      return m_imageCreateInfo;
    }

  private:
    VkImageCreateInfo m_imageCreateInfo;
  };
  static_assert( sizeof( ImageCreateInfo ) == sizeof( VkImageCreateInfo ), "struct and wrapper have different size!" );

  class PipelineMultisampleStateCreateInfo
  {
  public:
    PipelineMultisampleStateCreateInfo()
      : PipelineMultisampleStateCreateInfo( PipelineMultisampleStateCreateFlags(), SampleCountFlagBits::e1, 0, 0, nullptr, 0, 0 )
    {}

    PipelineMultisampleStateCreateInfo( PipelineMultisampleStateCreateFlags flags, SampleCountFlagBits rasterizationSamples, Bool32 sampleShadingEnable, float minSampleShading, const SampleMask* pSampleMask, Bool32 alphaToCoverageEnable, Bool32 alphaToOneEnable)
    {
      m_pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
      m_pipelineMultisampleStateCreateInfo.pNext = nullptr;
      m_pipelineMultisampleStateCreateInfo.flags = static_cast<VkPipelineMultisampleStateCreateFlags>( flags );
      m_pipelineMultisampleStateCreateInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>( rasterizationSamples );
      m_pipelineMultisampleStateCreateInfo.sampleShadingEnable = sampleShadingEnable;
      m_pipelineMultisampleStateCreateInfo.minSampleShading = minSampleShading;
      m_pipelineMultisampleStateCreateInfo.pSampleMask = pSampleMask;
      m_pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = alphaToCoverageEnable;
      m_pipelineMultisampleStateCreateInfo.alphaToOneEnable = alphaToOneEnable;
    }

    PipelineMultisampleStateCreateInfo(VkPipelineMultisampleStateCreateInfo const & rhs)
      : m_pipelineMultisampleStateCreateInfo(rhs)
    {
    }

    PipelineMultisampleStateCreateInfo& operator=(VkPipelineMultisampleStateCreateInfo const & rhs)
    {
      m_pipelineMultisampleStateCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineMultisampleStateCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_pipelineMultisampleStateCreateInfo.sType );
    }

    PipelineMultisampleStateCreateInfo& sType( StructureType sType )
    {
      m_pipelineMultisampleStateCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_pipelineMultisampleStateCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_pipelineMultisampleStateCreateInfo.pNext );
    }

    PipelineMultisampleStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineMultisampleStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineMultisampleStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineMultisampleStateCreateFlags&>( m_pipelineMultisampleStateCreateInfo.flags );
    }

    PipelineMultisampleStateCreateFlags& flags()
    {
      return reinterpret_cast<PipelineMultisampleStateCreateFlags&>( m_pipelineMultisampleStateCreateInfo.flags );
    }

    PipelineMultisampleStateCreateInfo& flags( PipelineMultisampleStateCreateFlags flags )
    {
      m_pipelineMultisampleStateCreateInfo.flags = static_cast<VkPipelineMultisampleStateCreateFlags>( flags );
      return *this;
    }

    const SampleCountFlagBits& rasterizationSamples() const
    {
      return reinterpret_cast<const SampleCountFlagBits&>( m_pipelineMultisampleStateCreateInfo.rasterizationSamples );
    }

    SampleCountFlagBits& rasterizationSamples()
    {
      return reinterpret_cast<SampleCountFlagBits&>( m_pipelineMultisampleStateCreateInfo.rasterizationSamples );
    }

    PipelineMultisampleStateCreateInfo& rasterizationSamples( SampleCountFlagBits rasterizationSamples )
    {
      m_pipelineMultisampleStateCreateInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>( rasterizationSamples );
      return *this;
    }

    const Bool32& sampleShadingEnable() const
    {
      return m_pipelineMultisampleStateCreateInfo.sampleShadingEnable;
    }

    Bool32& sampleShadingEnable()
    {
      return m_pipelineMultisampleStateCreateInfo.sampleShadingEnable;
    }

    PipelineMultisampleStateCreateInfo& sampleShadingEnable( Bool32 sampleShadingEnable )
    {
      m_pipelineMultisampleStateCreateInfo.sampleShadingEnable = sampleShadingEnable;
      return *this;
    }

    const float& minSampleShading() const
    {
      return m_pipelineMultisampleStateCreateInfo.minSampleShading;
    }

    float& minSampleShading()
    {
      return m_pipelineMultisampleStateCreateInfo.minSampleShading;
    }

    PipelineMultisampleStateCreateInfo& minSampleShading( float minSampleShading )
    {
      m_pipelineMultisampleStateCreateInfo.minSampleShading = minSampleShading;
      return *this;
    }

    const SampleMask* pSampleMask() const
    {
      return reinterpret_cast<const SampleMask*>( m_pipelineMultisampleStateCreateInfo.pSampleMask );
    }

    const SampleMask* pSampleMask()
    {
      return reinterpret_cast<const SampleMask*>( m_pipelineMultisampleStateCreateInfo.pSampleMask );
    }

    PipelineMultisampleStateCreateInfo& pSampleMask( const SampleMask* pSampleMask )
    {
      m_pipelineMultisampleStateCreateInfo.pSampleMask = pSampleMask;
      return *this;
    }

    const Bool32& alphaToCoverageEnable() const
    {
      return m_pipelineMultisampleStateCreateInfo.alphaToCoverageEnable;
    }

    Bool32& alphaToCoverageEnable()
    {
      return m_pipelineMultisampleStateCreateInfo.alphaToCoverageEnable;
    }

    PipelineMultisampleStateCreateInfo& alphaToCoverageEnable( Bool32 alphaToCoverageEnable )
    {
      m_pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = alphaToCoverageEnable;
      return *this;
    }

    const Bool32& alphaToOneEnable() const
    {
      return m_pipelineMultisampleStateCreateInfo.alphaToOneEnable;
    }

    Bool32& alphaToOneEnable()
    {
      return m_pipelineMultisampleStateCreateInfo.alphaToOneEnable;
    }

    PipelineMultisampleStateCreateInfo& alphaToOneEnable( Bool32 alphaToOneEnable )
    {
      m_pipelineMultisampleStateCreateInfo.alphaToOneEnable = alphaToOneEnable;
      return *this;
    }

    static PipelineMultisampleStateCreateInfo& null()
    {
      return *((PipelineMultisampleStateCreateInfo*)(nullptr));
    }

    operator const VkPipelineMultisampleStateCreateInfo&() const
    {
      return m_pipelineMultisampleStateCreateInfo;
    }

  private:
    VkPipelineMultisampleStateCreateInfo m_pipelineMultisampleStateCreateInfo;
  };
  static_assert( sizeof( PipelineMultisampleStateCreateInfo ) == sizeof( VkPipelineMultisampleStateCreateInfo ), "struct and wrapper have different size!" );

  class GraphicsPipelineCreateInfo
  {
  public:
    GraphicsPipelineCreateInfo()
      : GraphicsPipelineCreateInfo( PipelineCreateFlags(), 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, PipelineLayout(), RenderPass(), 0, Pipeline(), 0 )
    {}

    GraphicsPipelineCreateInfo( PipelineCreateFlags flags, uint32_t stageCount, const PipelineShaderStageCreateInfo* pStages, const PipelineVertexInputStateCreateInfo* pVertexInputState, const PipelineInputAssemblyStateCreateInfo* pInputAssemblyState, const PipelineTessellationStateCreateInfo* pTessellationState, const PipelineViewportStateCreateInfo* pViewportState, const PipelineRasterizationStateCreateInfo* pRasterizationState, const PipelineMultisampleStateCreateInfo* pMultisampleState, const PipelineDepthStencilStateCreateInfo* pDepthStencilState, const PipelineColorBlendStateCreateInfo* pColorBlendState, const PipelineDynamicStateCreateInfo* pDynamicState, PipelineLayout layout, RenderPass renderPass, uint32_t subpass, Pipeline basePipelineHandle, int32_t basePipelineIndex)
    {
      m_graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
      m_graphicsPipelineCreateInfo.pNext = nullptr;
      m_graphicsPipelineCreateInfo.flags = static_cast<VkPipelineCreateFlags>( flags );
      m_graphicsPipelineCreateInfo.stageCount = stageCount;
      m_graphicsPipelineCreateInfo.pStages = reinterpret_cast<const VkPipelineShaderStageCreateInfo*>( pStages );
      m_graphicsPipelineCreateInfo.pVertexInputState = reinterpret_cast<const VkPipelineVertexInputStateCreateInfo*>( pVertexInputState );
      m_graphicsPipelineCreateInfo.pInputAssemblyState = reinterpret_cast<const VkPipelineInputAssemblyStateCreateInfo*>( pInputAssemblyState );
      m_graphicsPipelineCreateInfo.pTessellationState = reinterpret_cast<const VkPipelineTessellationStateCreateInfo*>( pTessellationState );
      m_graphicsPipelineCreateInfo.pViewportState = reinterpret_cast<const VkPipelineViewportStateCreateInfo*>( pViewportState );
      m_graphicsPipelineCreateInfo.pRasterizationState = reinterpret_cast<const VkPipelineRasterizationStateCreateInfo*>( pRasterizationState );
      m_graphicsPipelineCreateInfo.pMultisampleState = reinterpret_cast<const VkPipelineMultisampleStateCreateInfo*>( pMultisampleState );
      m_graphicsPipelineCreateInfo.pDepthStencilState = reinterpret_cast<const VkPipelineDepthStencilStateCreateInfo*>( pDepthStencilState );
      m_graphicsPipelineCreateInfo.pColorBlendState = reinterpret_cast<const VkPipelineColorBlendStateCreateInfo*>( pColorBlendState );
      m_graphicsPipelineCreateInfo.pDynamicState = reinterpret_cast<const VkPipelineDynamicStateCreateInfo*>( pDynamicState );
      m_graphicsPipelineCreateInfo.layout = static_cast<VkPipelineLayout>( layout );
      m_graphicsPipelineCreateInfo.renderPass = static_cast<VkRenderPass>( renderPass );
      m_graphicsPipelineCreateInfo.subpass = subpass;
      m_graphicsPipelineCreateInfo.basePipelineHandle = static_cast<VkPipeline>( basePipelineHandle );
      m_graphicsPipelineCreateInfo.basePipelineIndex = basePipelineIndex;
    }

    GraphicsPipelineCreateInfo(VkGraphicsPipelineCreateInfo const & rhs)
      : m_graphicsPipelineCreateInfo(rhs)
    {
    }

    GraphicsPipelineCreateInfo& operator=(VkGraphicsPipelineCreateInfo const & rhs)
    {
      m_graphicsPipelineCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_graphicsPipelineCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_graphicsPipelineCreateInfo.sType );
    }

    GraphicsPipelineCreateInfo& sType( StructureType sType )
    {
      m_graphicsPipelineCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_graphicsPipelineCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_graphicsPipelineCreateInfo.pNext );
    }

    GraphicsPipelineCreateInfo& pNext( const void* pNext )
    {
      m_graphicsPipelineCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineCreateFlags&>( m_graphicsPipelineCreateInfo.flags );
    }

    PipelineCreateFlags& flags()
    {
      return reinterpret_cast<PipelineCreateFlags&>( m_graphicsPipelineCreateInfo.flags );
    }

    GraphicsPipelineCreateInfo& flags( PipelineCreateFlags flags )
    {
      m_graphicsPipelineCreateInfo.flags = static_cast<VkPipelineCreateFlags>( flags );
      return *this;
    }

    const uint32_t& stageCount() const
    {
      return m_graphicsPipelineCreateInfo.stageCount;
    }

    uint32_t& stageCount()
    {
      return m_graphicsPipelineCreateInfo.stageCount;
    }

    GraphicsPipelineCreateInfo& stageCount( uint32_t stageCount )
    {
      m_graphicsPipelineCreateInfo.stageCount = stageCount;
      return *this;
    }

    const PipelineShaderStageCreateInfo* pStages() const
    {
      return reinterpret_cast<const PipelineShaderStageCreateInfo*>( m_graphicsPipelineCreateInfo.pStages );
    }

    const PipelineShaderStageCreateInfo* pStages()
    {
      return reinterpret_cast<const PipelineShaderStageCreateInfo*>( m_graphicsPipelineCreateInfo.pStages );
    }

    GraphicsPipelineCreateInfo& pStages( const PipelineShaderStageCreateInfo* pStages )
    {
      m_graphicsPipelineCreateInfo.pStages = reinterpret_cast<const VkPipelineShaderStageCreateInfo*>( pStages );
      return *this;
    }

    const PipelineVertexInputStateCreateInfo* pVertexInputState() const
    {
      return reinterpret_cast<const PipelineVertexInputStateCreateInfo*>( m_graphicsPipelineCreateInfo.pVertexInputState );
    }

    const PipelineVertexInputStateCreateInfo* pVertexInputState()
    {
      return reinterpret_cast<const PipelineVertexInputStateCreateInfo*>( m_graphicsPipelineCreateInfo.pVertexInputState );
    }

    GraphicsPipelineCreateInfo& pVertexInputState( const PipelineVertexInputStateCreateInfo* pVertexInputState )
    {
      m_graphicsPipelineCreateInfo.pVertexInputState = reinterpret_cast<const VkPipelineVertexInputStateCreateInfo*>( pVertexInputState );
      return *this;
    }

    const PipelineInputAssemblyStateCreateInfo* pInputAssemblyState() const
    {
      return reinterpret_cast<const PipelineInputAssemblyStateCreateInfo*>( m_graphicsPipelineCreateInfo.pInputAssemblyState );
    }

    const PipelineInputAssemblyStateCreateInfo* pInputAssemblyState()
    {
      return reinterpret_cast<const PipelineInputAssemblyStateCreateInfo*>( m_graphicsPipelineCreateInfo.pInputAssemblyState );
    }

    GraphicsPipelineCreateInfo& pInputAssemblyState( const PipelineInputAssemblyStateCreateInfo* pInputAssemblyState )
    {
      m_graphicsPipelineCreateInfo.pInputAssemblyState = reinterpret_cast<const VkPipelineInputAssemblyStateCreateInfo*>( pInputAssemblyState );
      return *this;
    }

    const PipelineTessellationStateCreateInfo* pTessellationState() const
    {
      return reinterpret_cast<const PipelineTessellationStateCreateInfo*>( m_graphicsPipelineCreateInfo.pTessellationState );
    }

    const PipelineTessellationStateCreateInfo* pTessellationState()
    {
      return reinterpret_cast<const PipelineTessellationStateCreateInfo*>( m_graphicsPipelineCreateInfo.pTessellationState );
    }

    GraphicsPipelineCreateInfo& pTessellationState( const PipelineTessellationStateCreateInfo* pTessellationState )
    {
      m_graphicsPipelineCreateInfo.pTessellationState = reinterpret_cast<const VkPipelineTessellationStateCreateInfo*>( pTessellationState );
      return *this;
    }

    const PipelineViewportStateCreateInfo* pViewportState() const
    {
      return reinterpret_cast<const PipelineViewportStateCreateInfo*>( m_graphicsPipelineCreateInfo.pViewportState );
    }

    const PipelineViewportStateCreateInfo* pViewportState()
    {
      return reinterpret_cast<const PipelineViewportStateCreateInfo*>( m_graphicsPipelineCreateInfo.pViewportState );
    }

    GraphicsPipelineCreateInfo& pViewportState( const PipelineViewportStateCreateInfo* pViewportState )
    {
      m_graphicsPipelineCreateInfo.pViewportState = reinterpret_cast<const VkPipelineViewportStateCreateInfo*>( pViewportState );
      return *this;
    }

    const PipelineRasterizationStateCreateInfo* pRasterizationState() const
    {
      return reinterpret_cast<const PipelineRasterizationStateCreateInfo*>( m_graphicsPipelineCreateInfo.pRasterizationState );
    }

    const PipelineRasterizationStateCreateInfo* pRasterizationState()
    {
      return reinterpret_cast<const PipelineRasterizationStateCreateInfo*>( m_graphicsPipelineCreateInfo.pRasterizationState );
    }

    GraphicsPipelineCreateInfo& pRasterizationState( const PipelineRasterizationStateCreateInfo* pRasterizationState )
    {
      m_graphicsPipelineCreateInfo.pRasterizationState = reinterpret_cast<const VkPipelineRasterizationStateCreateInfo*>( pRasterizationState );
      return *this;
    }

    const PipelineMultisampleStateCreateInfo* pMultisampleState() const
    {
      return reinterpret_cast<const PipelineMultisampleStateCreateInfo*>( m_graphicsPipelineCreateInfo.pMultisampleState );
    }

    const PipelineMultisampleStateCreateInfo* pMultisampleState()
    {
      return reinterpret_cast<const PipelineMultisampleStateCreateInfo*>( m_graphicsPipelineCreateInfo.pMultisampleState );
    }

    GraphicsPipelineCreateInfo& pMultisampleState( const PipelineMultisampleStateCreateInfo* pMultisampleState )
    {
      m_graphicsPipelineCreateInfo.pMultisampleState = reinterpret_cast<const VkPipelineMultisampleStateCreateInfo*>( pMultisampleState );
      return *this;
    }

    const PipelineDepthStencilStateCreateInfo* pDepthStencilState() const
    {
      return reinterpret_cast<const PipelineDepthStencilStateCreateInfo*>( m_graphicsPipelineCreateInfo.pDepthStencilState );
    }

    const PipelineDepthStencilStateCreateInfo* pDepthStencilState()
    {
      return reinterpret_cast<const PipelineDepthStencilStateCreateInfo*>( m_graphicsPipelineCreateInfo.pDepthStencilState );
    }

    GraphicsPipelineCreateInfo& pDepthStencilState( const PipelineDepthStencilStateCreateInfo* pDepthStencilState )
    {
      m_graphicsPipelineCreateInfo.pDepthStencilState = reinterpret_cast<const VkPipelineDepthStencilStateCreateInfo*>( pDepthStencilState );
      return *this;
    }

    const PipelineColorBlendStateCreateInfo* pColorBlendState() const
    {
      return reinterpret_cast<const PipelineColorBlendStateCreateInfo*>( m_graphicsPipelineCreateInfo.pColorBlendState );
    }

    const PipelineColorBlendStateCreateInfo* pColorBlendState()
    {
      return reinterpret_cast<const PipelineColorBlendStateCreateInfo*>( m_graphicsPipelineCreateInfo.pColorBlendState );
    }

    GraphicsPipelineCreateInfo& pColorBlendState( const PipelineColorBlendStateCreateInfo* pColorBlendState )
    {
      m_graphicsPipelineCreateInfo.pColorBlendState = reinterpret_cast<const VkPipelineColorBlendStateCreateInfo*>( pColorBlendState );
      return *this;
    }

    const PipelineDynamicStateCreateInfo* pDynamicState() const
    {
      return reinterpret_cast<const PipelineDynamicStateCreateInfo*>( m_graphicsPipelineCreateInfo.pDynamicState );
    }

    const PipelineDynamicStateCreateInfo* pDynamicState()
    {
      return reinterpret_cast<const PipelineDynamicStateCreateInfo*>( m_graphicsPipelineCreateInfo.pDynamicState );
    }

    GraphicsPipelineCreateInfo& pDynamicState( const PipelineDynamicStateCreateInfo* pDynamicState )
    {
      m_graphicsPipelineCreateInfo.pDynamicState = reinterpret_cast<const VkPipelineDynamicStateCreateInfo*>( pDynamicState );
      return *this;
    }

    const PipelineLayout& layout() const
    {
      return reinterpret_cast<const PipelineLayout&>( m_graphicsPipelineCreateInfo.layout );
    }

    PipelineLayout& layout()
    {
      return reinterpret_cast<PipelineLayout&>( m_graphicsPipelineCreateInfo.layout );
    }

    GraphicsPipelineCreateInfo& layout( PipelineLayout layout )
    {
      m_graphicsPipelineCreateInfo.layout = static_cast<VkPipelineLayout>( layout );
      return *this;
    }

    const RenderPass& renderPass() const
    {
      return reinterpret_cast<const RenderPass&>( m_graphicsPipelineCreateInfo.renderPass );
    }

    RenderPass& renderPass()
    {
      return reinterpret_cast<RenderPass&>( m_graphicsPipelineCreateInfo.renderPass );
    }

    GraphicsPipelineCreateInfo& renderPass( RenderPass renderPass )
    {
      m_graphicsPipelineCreateInfo.renderPass = static_cast<VkRenderPass>( renderPass );
      return *this;
    }

    const uint32_t& subpass() const
    {
      return m_graphicsPipelineCreateInfo.subpass;
    }

    uint32_t& subpass()
    {
      return m_graphicsPipelineCreateInfo.subpass;
    }

    GraphicsPipelineCreateInfo& subpass( uint32_t subpass )
    {
      m_graphicsPipelineCreateInfo.subpass = subpass;
      return *this;
    }

    const Pipeline& basePipelineHandle() const
    {
      return reinterpret_cast<const Pipeline&>( m_graphicsPipelineCreateInfo.basePipelineHandle );
    }

    Pipeline& basePipelineHandle()
    {
      return reinterpret_cast<Pipeline&>( m_graphicsPipelineCreateInfo.basePipelineHandle );
    }

    GraphicsPipelineCreateInfo& basePipelineHandle( Pipeline basePipelineHandle )
    {
      m_graphicsPipelineCreateInfo.basePipelineHandle = static_cast<VkPipeline>( basePipelineHandle );
      return *this;
    }

    const int32_t& basePipelineIndex() const
    {
      return m_graphicsPipelineCreateInfo.basePipelineIndex;
    }

    int32_t& basePipelineIndex()
    {
      return m_graphicsPipelineCreateInfo.basePipelineIndex;
    }

    GraphicsPipelineCreateInfo& basePipelineIndex( int32_t basePipelineIndex )
    {
      m_graphicsPipelineCreateInfo.basePipelineIndex = basePipelineIndex;
      return *this;
    }

    static GraphicsPipelineCreateInfo& null()
    {
      return *((GraphicsPipelineCreateInfo*)(nullptr));
    }

    operator const VkGraphicsPipelineCreateInfo&() const
    {
      return m_graphicsPipelineCreateInfo;
    }

  private:
    VkGraphicsPipelineCreateInfo m_graphicsPipelineCreateInfo;
  };
  static_assert( sizeof( GraphicsPipelineCreateInfo ) == sizeof( VkGraphicsPipelineCreateInfo ), "struct and wrapper have different size!" );

  class PhysicalDeviceLimits
  {
  public:
    const uint32_t& maxImageDimension1D() const
    {
      return m_physicalDeviceLimits.maxImageDimension1D;
    }

    const uint32_t& maxImageDimension2D() const
    {
      return m_physicalDeviceLimits.maxImageDimension2D;
    }

    const uint32_t& maxImageDimension3D() const
    {
      return m_physicalDeviceLimits.maxImageDimension3D;
    }

    const uint32_t& maxImageDimensionCube() const
    {
      return m_physicalDeviceLimits.maxImageDimensionCube;
    }

    const uint32_t& maxImageArrayLayers() const
    {
      return m_physicalDeviceLimits.maxImageArrayLayers;
    }

    const uint32_t& maxTexelBufferElements() const
    {
      return m_physicalDeviceLimits.maxTexelBufferElements;
    }

    const uint32_t& maxUniformBufferRange() const
    {
      return m_physicalDeviceLimits.maxUniformBufferRange;
    }

    const uint32_t& maxStorageBufferRange() const
    {
      return m_physicalDeviceLimits.maxStorageBufferRange;
    }

    const uint32_t& maxPushConstantsSize() const
    {
      return m_physicalDeviceLimits.maxPushConstantsSize;
    }

    const uint32_t& maxMemoryAllocationCount() const
    {
      return m_physicalDeviceLimits.maxMemoryAllocationCount;
    }

    const uint32_t& maxSamplerAllocationCount() const
    {
      return m_physicalDeviceLimits.maxSamplerAllocationCount;
    }

    const DeviceSize& bufferImageGranularity() const
    {
      return m_physicalDeviceLimits.bufferImageGranularity;
    }

    const DeviceSize& sparseAddressSpaceSize() const
    {
      return m_physicalDeviceLimits.sparseAddressSpaceSize;
    }

    const uint32_t& maxBoundDescriptorSets() const
    {
      return m_physicalDeviceLimits.maxBoundDescriptorSets;
    }

    const uint32_t& maxPerStageDescriptorSamplers() const
    {
      return m_physicalDeviceLimits.maxPerStageDescriptorSamplers;
    }

    const uint32_t& maxPerStageDescriptorUniformBuffers() const
    {
      return m_physicalDeviceLimits.maxPerStageDescriptorUniformBuffers;
    }

    const uint32_t& maxPerStageDescriptorStorageBuffers() const
    {
      return m_physicalDeviceLimits.maxPerStageDescriptorStorageBuffers;
    }

    const uint32_t& maxPerStageDescriptorSampledImages() const
    {
      return m_physicalDeviceLimits.maxPerStageDescriptorSampledImages;
    }

    const uint32_t& maxPerStageDescriptorStorageImages() const
    {
      return m_physicalDeviceLimits.maxPerStageDescriptorStorageImages;
    }

    const uint32_t& maxPerStageDescriptorInputAttachments() const
    {
      return m_physicalDeviceLimits.maxPerStageDescriptorInputAttachments;
    }

    const uint32_t& maxPerStageResources() const
    {
      return m_physicalDeviceLimits.maxPerStageResources;
    }

    const uint32_t& maxDescriptorSetSamplers() const
    {
      return m_physicalDeviceLimits.maxDescriptorSetSamplers;
    }

    const uint32_t& maxDescriptorSetUniformBuffers() const
    {
      return m_physicalDeviceLimits.maxDescriptorSetUniformBuffers;
    }

    const uint32_t& maxDescriptorSetUniformBuffersDynamic() const
    {
      return m_physicalDeviceLimits.maxDescriptorSetUniformBuffersDynamic;
    }

    const uint32_t& maxDescriptorSetStorageBuffers() const
    {
      return m_physicalDeviceLimits.maxDescriptorSetStorageBuffers;
    }

    const uint32_t& maxDescriptorSetStorageBuffersDynamic() const
    {
      return m_physicalDeviceLimits.maxDescriptorSetStorageBuffersDynamic;
    }

    const uint32_t& maxDescriptorSetSampledImages() const
    {
      return m_physicalDeviceLimits.maxDescriptorSetSampledImages;
    }

    const uint32_t& maxDescriptorSetStorageImages() const
    {
      return m_physicalDeviceLimits.maxDescriptorSetStorageImages;
    }

    const uint32_t& maxDescriptorSetInputAttachments() const
    {
      return m_physicalDeviceLimits.maxDescriptorSetInputAttachments;
    }

    const uint32_t& maxVertexInputAttributes() const
    {
      return m_physicalDeviceLimits.maxVertexInputAttributes;
    }

    const uint32_t& maxVertexInputBindings() const
    {
      return m_physicalDeviceLimits.maxVertexInputBindings;
    }

    const uint32_t& maxVertexInputAttributeOffset() const
    {
      return m_physicalDeviceLimits.maxVertexInputAttributeOffset;
    }

    const uint32_t& maxVertexInputBindingStride() const
    {
      return m_physicalDeviceLimits.maxVertexInputBindingStride;
    }

    const uint32_t& maxVertexOutputComponents() const
    {
      return m_physicalDeviceLimits.maxVertexOutputComponents;
    }

    const uint32_t& maxTessellationGenerationLevel() const
    {
      return m_physicalDeviceLimits.maxTessellationGenerationLevel;
    }

    const uint32_t& maxTessellationPatchSize() const
    {
      return m_physicalDeviceLimits.maxTessellationPatchSize;
    }

    const uint32_t& maxTessellationControlPerVertexInputComponents() const
    {
      return m_physicalDeviceLimits.maxTessellationControlPerVertexInputComponents;
    }

    const uint32_t& maxTessellationControlPerVertexOutputComponents() const
    {
      return m_physicalDeviceLimits.maxTessellationControlPerVertexOutputComponents;
    }

    const uint32_t& maxTessellationControlPerPatchOutputComponents() const
    {
      return m_physicalDeviceLimits.maxTessellationControlPerPatchOutputComponents;
    }

    const uint32_t& maxTessellationControlTotalOutputComponents() const
    {
      return m_physicalDeviceLimits.maxTessellationControlTotalOutputComponents;
    }

    const uint32_t& maxTessellationEvaluationInputComponents() const
    {
      return m_physicalDeviceLimits.maxTessellationEvaluationInputComponents;
    }

    const uint32_t& maxTessellationEvaluationOutputComponents() const
    {
      return m_physicalDeviceLimits.maxTessellationEvaluationOutputComponents;
    }

    const uint32_t& maxGeometryShaderInvocations() const
    {
      return m_physicalDeviceLimits.maxGeometryShaderInvocations;
    }

    const uint32_t& maxGeometryInputComponents() const
    {
      return m_physicalDeviceLimits.maxGeometryInputComponents;
    }

    const uint32_t& maxGeometryOutputComponents() const
    {
      return m_physicalDeviceLimits.maxGeometryOutputComponents;
    }

    const uint32_t& maxGeometryOutputVertices() const
    {
      return m_physicalDeviceLimits.maxGeometryOutputVertices;
    }

    const uint32_t& maxGeometryTotalOutputComponents() const
    {
      return m_physicalDeviceLimits.maxGeometryTotalOutputComponents;
    }

    const uint32_t& maxFragmentInputComponents() const
    {
      return m_physicalDeviceLimits.maxFragmentInputComponents;
    }

    const uint32_t& maxFragmentOutputAttachments() const
    {
      return m_physicalDeviceLimits.maxFragmentOutputAttachments;
    }

    const uint32_t& maxFragmentDualSrcAttachments() const
    {
      return m_physicalDeviceLimits.maxFragmentDualSrcAttachments;
    }

    const uint32_t& maxFragmentCombinedOutputResources() const
    {
      return m_physicalDeviceLimits.maxFragmentCombinedOutputResources;
    }

    const uint32_t& maxComputeSharedMemorySize() const
    {
      return m_physicalDeviceLimits.maxComputeSharedMemorySize;
    }

    const uint32_t* maxComputeWorkGroupCount() const
    {
      return reinterpret_cast<const uint32_t*>( m_physicalDeviceLimits.maxComputeWorkGroupCount );
    }

    const uint32_t& maxComputeWorkGroupInvocations() const
    {
      return m_physicalDeviceLimits.maxComputeWorkGroupInvocations;
    }

    const uint32_t* maxComputeWorkGroupSize() const
    {
      return reinterpret_cast<const uint32_t*>( m_physicalDeviceLimits.maxComputeWorkGroupSize );
    }

    const uint32_t& subPixelPrecisionBits() const
    {
      return m_physicalDeviceLimits.subPixelPrecisionBits;
    }

    const uint32_t& subTexelPrecisionBits() const
    {
      return m_physicalDeviceLimits.subTexelPrecisionBits;
    }

    const uint32_t& mipmapPrecisionBits() const
    {
      return m_physicalDeviceLimits.mipmapPrecisionBits;
    }

    const uint32_t& maxDrawIndexedIndexValue() const
    {
      return m_physicalDeviceLimits.maxDrawIndexedIndexValue;
    }

    const uint32_t& maxDrawIndirectCount() const
    {
      return m_physicalDeviceLimits.maxDrawIndirectCount;
    }

    const float& maxSamplerLodBias() const
    {
      return m_physicalDeviceLimits.maxSamplerLodBias;
    }

    const float& maxSamplerAnisotropy() const
    {
      return m_physicalDeviceLimits.maxSamplerAnisotropy;
    }

    const uint32_t& maxViewports() const
    {
      return m_physicalDeviceLimits.maxViewports;
    }

    const uint32_t* maxViewportDimensions() const
    {
      return reinterpret_cast<const uint32_t*>( m_physicalDeviceLimits.maxViewportDimensions );
    }

    const float* viewportBoundsRange() const
    {
      return reinterpret_cast<const float*>( m_physicalDeviceLimits.viewportBoundsRange );
    }

    const uint32_t& viewportSubPixelBits() const
    {
      return m_physicalDeviceLimits.viewportSubPixelBits;
    }

    const size_t& minMemoryMapAlignment() const
    {
      return m_physicalDeviceLimits.minMemoryMapAlignment;
    }

    const DeviceSize& minTexelBufferOffsetAlignment() const
    {
      return m_physicalDeviceLimits.minTexelBufferOffsetAlignment;
    }

    const DeviceSize& minUniformBufferOffsetAlignment() const
    {
      return m_physicalDeviceLimits.minUniformBufferOffsetAlignment;
    }

    const DeviceSize& minStorageBufferOffsetAlignment() const
    {
      return m_physicalDeviceLimits.minStorageBufferOffsetAlignment;
    }

    const int32_t& minTexelOffset() const
    {
      return m_physicalDeviceLimits.minTexelOffset;
    }

    const uint32_t& maxTexelOffset() const
    {
      return m_physicalDeviceLimits.maxTexelOffset;
    }

    const int32_t& minTexelGatherOffset() const
    {
      return m_physicalDeviceLimits.minTexelGatherOffset;
    }

    const uint32_t& maxTexelGatherOffset() const
    {
      return m_physicalDeviceLimits.maxTexelGatherOffset;
    }

    const float& minInterpolationOffset() const
    {
      return m_physicalDeviceLimits.minInterpolationOffset;
    }

    const float& maxInterpolationOffset() const
    {
      return m_physicalDeviceLimits.maxInterpolationOffset;
    }

    const uint32_t& subPixelInterpolationOffsetBits() const
    {
      return m_physicalDeviceLimits.subPixelInterpolationOffsetBits;
    }

    const uint32_t& maxFramebufferWidth() const
    {
      return m_physicalDeviceLimits.maxFramebufferWidth;
    }

    const uint32_t& maxFramebufferHeight() const
    {
      return m_physicalDeviceLimits.maxFramebufferHeight;
    }

    const uint32_t& maxFramebufferLayers() const
    {
      return m_physicalDeviceLimits.maxFramebufferLayers;
    }

    const SampleCountFlags& framebufferColorSampleCounts() const
    {
      return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.framebufferColorSampleCounts );
    }

    const SampleCountFlags& framebufferDepthSampleCounts() const
    {
      return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.framebufferDepthSampleCounts );
    }

    const SampleCountFlags& framebufferStencilSampleCounts() const
    {
      return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.framebufferStencilSampleCounts );
    }

    const SampleCountFlags& framebufferNoAttachmentsSampleCounts() const
    {
      return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.framebufferNoAttachmentsSampleCounts );
    }

    const uint32_t& maxColorAttachments() const
    {
      return m_physicalDeviceLimits.maxColorAttachments;
    }

    const SampleCountFlags& sampledImageColorSampleCounts() const
    {
      return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.sampledImageColorSampleCounts );
    }

    const SampleCountFlags& sampledImageIntegerSampleCounts() const
    {
      return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.sampledImageIntegerSampleCounts );
    }

    const SampleCountFlags& sampledImageDepthSampleCounts() const
    {
      return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.sampledImageDepthSampleCounts );
    }

    const SampleCountFlags& sampledImageStencilSampleCounts() const
    {
      return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.sampledImageStencilSampleCounts );
    }

    const SampleCountFlags& storageImageSampleCounts() const
    {
      return reinterpret_cast<const SampleCountFlags&>( m_physicalDeviceLimits.storageImageSampleCounts );
    }

    const uint32_t& maxSampleMaskWords() const
    {
      return m_physicalDeviceLimits.maxSampleMaskWords;
    }

    const Bool32& timestampComputeAndGraphics() const
    {
      return m_physicalDeviceLimits.timestampComputeAndGraphics;
    }

    const float& timestampPeriod() const
    {
      return m_physicalDeviceLimits.timestampPeriod;
    }

    const uint32_t& maxClipDistances() const
    {
      return m_physicalDeviceLimits.maxClipDistances;
    }

    const uint32_t& maxCullDistances() const
    {
      return m_physicalDeviceLimits.maxCullDistances;
    }

    const uint32_t& maxCombinedClipAndCullDistances() const
    {
      return m_physicalDeviceLimits.maxCombinedClipAndCullDistances;
    }

    const uint32_t& discreteQueuePriorities() const
    {
      return m_physicalDeviceLimits.discreteQueuePriorities;
    }

    const float* pointSizeRange() const
    {
      return reinterpret_cast<const float*>( m_physicalDeviceLimits.pointSizeRange );
    }

    const float* lineWidthRange() const
    {
      return reinterpret_cast<const float*>( m_physicalDeviceLimits.lineWidthRange );
    }

    const float& pointSizeGranularity() const
    {
      return m_physicalDeviceLimits.pointSizeGranularity;
    }

    const float& lineWidthGranularity() const
    {
      return m_physicalDeviceLimits.lineWidthGranularity;
    }

    const Bool32& strictLines() const
    {
      return m_physicalDeviceLimits.strictLines;
    }

    const Bool32& standardSampleLocations() const
    {
      return m_physicalDeviceLimits.standardSampleLocations;
    }

    const DeviceSize& optimalBufferCopyOffsetAlignment() const
    {
      return m_physicalDeviceLimits.optimalBufferCopyOffsetAlignment;
    }

    const DeviceSize& optimalBufferCopyRowPitchAlignment() const
    {
      return m_physicalDeviceLimits.optimalBufferCopyRowPitchAlignment;
    }

    const DeviceSize& nonCoherentAtomSize() const
    {
      return m_physicalDeviceLimits.nonCoherentAtomSize;
    }

    static PhysicalDeviceLimits& null()
    {
      return *((PhysicalDeviceLimits*)(nullptr));
    }

    operator const VkPhysicalDeviceLimits&() const
    {
      return m_physicalDeviceLimits;
    }

  private:
    VkPhysicalDeviceLimits m_physicalDeviceLimits;
  };
  static_assert( sizeof( PhysicalDeviceLimits ) == sizeof( VkPhysicalDeviceLimits ), "struct and wrapper have different size!" );

  class PhysicalDeviceProperties
  {
  public:
    const uint32_t& apiVersion() const
    {
      return m_physicalDeviceProperties.apiVersion;
    }

    const uint32_t& driverVersion() const
    {
      return m_physicalDeviceProperties.driverVersion;
    }

    const uint32_t& vendorID() const
    {
      return m_physicalDeviceProperties.vendorID;
    }

    const uint32_t& deviceID() const
    {
      return m_physicalDeviceProperties.deviceID;
    }

    const PhysicalDeviceType& deviceType() const
    {
      return reinterpret_cast<const PhysicalDeviceType&>( m_physicalDeviceProperties.deviceType );
    }

    const char* deviceName() const
    {
      return reinterpret_cast<const char*>( m_physicalDeviceProperties.deviceName );
    }

    const uint8_t* pipelineCacheUUID() const
    {
      return reinterpret_cast<const uint8_t*>( m_physicalDeviceProperties.pipelineCacheUUID );
    }

    const PhysicalDeviceLimits& limits() const
    {
      return reinterpret_cast<const PhysicalDeviceLimits&>( m_physicalDeviceProperties.limits );
    }

    const PhysicalDeviceSparseProperties& sparseProperties() const
    {
      return reinterpret_cast<const PhysicalDeviceSparseProperties&>( m_physicalDeviceProperties.sparseProperties );
    }

    static PhysicalDeviceProperties& null()
    {
      return *((PhysicalDeviceProperties*)(nullptr));
    }

    operator const VkPhysicalDeviceProperties&() const
    {
      return m_physicalDeviceProperties;
    }

  private:
    VkPhysicalDeviceProperties m_physicalDeviceProperties;
  };
  static_assert( sizeof( PhysicalDeviceProperties ) == sizeof( VkPhysicalDeviceProperties ), "struct and wrapper have different size!" );

  enum class AttachmentDescriptionFlagBits
  {
    eMayAlias = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT
  };

  typedef Flags<AttachmentDescriptionFlagBits, VkAttachmentDescriptionFlags> AttachmentDescriptionFlags;

  inline AttachmentDescriptionFlags operator|( AttachmentDescriptionFlagBits bit0, AttachmentDescriptionFlagBits bit1 )
  {
    return AttachmentDescriptionFlags( bit0 ) | bit1;
  }

  class AttachmentDescription
  {
  public:
    AttachmentDescription()
      : AttachmentDescription( AttachmentDescriptionFlags(), Format::eUndefined, SampleCountFlagBits::e1, AttachmentLoadOp::eLoad, AttachmentStoreOp::eStore, AttachmentLoadOp::eLoad, AttachmentStoreOp::eStore, ImageLayout::eUndefined, ImageLayout::eUndefined )
    {}

    AttachmentDescription( AttachmentDescriptionFlags flags, Format format, SampleCountFlagBits samples, AttachmentLoadOp loadOp, AttachmentStoreOp storeOp, AttachmentLoadOp stencilLoadOp, AttachmentStoreOp stencilStoreOp, ImageLayout initialLayout, ImageLayout finalLayout)
    {
      m_attachmentDescription.flags = static_cast<VkAttachmentDescriptionFlags>( flags );
      m_attachmentDescription.format = static_cast<VkFormat>( format );
      m_attachmentDescription.samples = static_cast<VkSampleCountFlagBits>( samples );
      m_attachmentDescription.loadOp = static_cast<VkAttachmentLoadOp>( loadOp );
      m_attachmentDescription.storeOp = static_cast<VkAttachmentStoreOp>( storeOp );
      m_attachmentDescription.stencilLoadOp = static_cast<VkAttachmentLoadOp>( stencilLoadOp );
      m_attachmentDescription.stencilStoreOp = static_cast<VkAttachmentStoreOp>( stencilStoreOp );
      m_attachmentDescription.initialLayout = static_cast<VkImageLayout>( initialLayout );
      m_attachmentDescription.finalLayout = static_cast<VkImageLayout>( finalLayout );
    }

    AttachmentDescription(VkAttachmentDescription const & rhs)
      : m_attachmentDescription(rhs)
    {
    }

    AttachmentDescription& operator=(VkAttachmentDescription const & rhs)
    {
      m_attachmentDescription = rhs;
      return *this;
    }

    const AttachmentDescriptionFlags& flags() const
    {
      return reinterpret_cast<const AttachmentDescriptionFlags&>( m_attachmentDescription.flags );
    }

    AttachmentDescriptionFlags& flags()
    {
      return reinterpret_cast<AttachmentDescriptionFlags&>( m_attachmentDescription.flags );
    }

    AttachmentDescription& flags( AttachmentDescriptionFlags flags )
    {
      m_attachmentDescription.flags = static_cast<VkAttachmentDescriptionFlags>( flags );
      return *this;
    }

    const Format& format() const
    {
      return reinterpret_cast<const Format&>( m_attachmentDescription.format );
    }

    Format& format()
    {
      return reinterpret_cast<Format&>( m_attachmentDescription.format );
    }

    AttachmentDescription& format( Format format )
    {
      m_attachmentDescription.format = static_cast<VkFormat>( format );
      return *this;
    }

    const SampleCountFlagBits& samples() const
    {
      return reinterpret_cast<const SampleCountFlagBits&>( m_attachmentDescription.samples );
    }

    SampleCountFlagBits& samples()
    {
      return reinterpret_cast<SampleCountFlagBits&>( m_attachmentDescription.samples );
    }

    AttachmentDescription& samples( SampleCountFlagBits samples )
    {
      m_attachmentDescription.samples = static_cast<VkSampleCountFlagBits>( samples );
      return *this;
    }

    const AttachmentLoadOp& loadOp() const
    {
      return reinterpret_cast<const AttachmentLoadOp&>( m_attachmentDescription.loadOp );
    }

    AttachmentLoadOp& loadOp()
    {
      return reinterpret_cast<AttachmentLoadOp&>( m_attachmentDescription.loadOp );
    }

    AttachmentDescription& loadOp( AttachmentLoadOp loadOp )
    {
      m_attachmentDescription.loadOp = static_cast<VkAttachmentLoadOp>( loadOp );
      return *this;
    }

    const AttachmentStoreOp& storeOp() const
    {
      return reinterpret_cast<const AttachmentStoreOp&>( m_attachmentDescription.storeOp );
    }

    AttachmentStoreOp& storeOp()
    {
      return reinterpret_cast<AttachmentStoreOp&>( m_attachmentDescription.storeOp );
    }

    AttachmentDescription& storeOp( AttachmentStoreOp storeOp )
    {
      m_attachmentDescription.storeOp = static_cast<VkAttachmentStoreOp>( storeOp );
      return *this;
    }

    const AttachmentLoadOp& stencilLoadOp() const
    {
      return reinterpret_cast<const AttachmentLoadOp&>( m_attachmentDescription.stencilLoadOp );
    }

    AttachmentLoadOp& stencilLoadOp()
    {
      return reinterpret_cast<AttachmentLoadOp&>( m_attachmentDescription.stencilLoadOp );
    }

    AttachmentDescription& stencilLoadOp( AttachmentLoadOp stencilLoadOp )
    {
      m_attachmentDescription.stencilLoadOp = static_cast<VkAttachmentLoadOp>( stencilLoadOp );
      return *this;
    }

    const AttachmentStoreOp& stencilStoreOp() const
    {
      return reinterpret_cast<const AttachmentStoreOp&>( m_attachmentDescription.stencilStoreOp );
    }

    AttachmentStoreOp& stencilStoreOp()
    {
      return reinterpret_cast<AttachmentStoreOp&>( m_attachmentDescription.stencilStoreOp );
    }

    AttachmentDescription& stencilStoreOp( AttachmentStoreOp stencilStoreOp )
    {
      m_attachmentDescription.stencilStoreOp = static_cast<VkAttachmentStoreOp>( stencilStoreOp );
      return *this;
    }

    const ImageLayout& initialLayout() const
    {
      return reinterpret_cast<const ImageLayout&>( m_attachmentDescription.initialLayout );
    }

    ImageLayout& initialLayout()
    {
      return reinterpret_cast<ImageLayout&>( m_attachmentDescription.initialLayout );
    }

    AttachmentDescription& initialLayout( ImageLayout initialLayout )
    {
      m_attachmentDescription.initialLayout = static_cast<VkImageLayout>( initialLayout );
      return *this;
    }

    const ImageLayout& finalLayout() const
    {
      return reinterpret_cast<const ImageLayout&>( m_attachmentDescription.finalLayout );
    }

    ImageLayout& finalLayout()
    {
      return reinterpret_cast<ImageLayout&>( m_attachmentDescription.finalLayout );
    }

    AttachmentDescription& finalLayout( ImageLayout finalLayout )
    {
      m_attachmentDescription.finalLayout = static_cast<VkImageLayout>( finalLayout );
      return *this;
    }

    static AttachmentDescription& null()
    {
      return *((AttachmentDescription*)(nullptr));
    }

    operator const VkAttachmentDescription&() const
    {
      return m_attachmentDescription;
    }

  private:
    VkAttachmentDescription m_attachmentDescription;
  };
  static_assert( sizeof( AttachmentDescription ) == sizeof( VkAttachmentDescription ), "struct and wrapper have different size!" );

  enum class StencilFaceFlagBits
  {
    eFront = VK_STENCIL_FACE_FRONT_BIT,
    eBack = VK_STENCIL_FACE_BACK_BIT,
    eVkStencilFrontAndBack = VK_STENCIL_FRONT_AND_BACK
  };

  typedef Flags<StencilFaceFlagBits, VkStencilFaceFlags> StencilFaceFlags;

  inline StencilFaceFlags operator|( StencilFaceFlagBits bit0, StencilFaceFlagBits bit1 )
  {
    return StencilFaceFlags( bit0 ) | bit1;
  }

  enum class DescriptorPoolCreateFlagBits
  {
    eFreeDescriptorSet = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT
  };

  typedef Flags<DescriptorPoolCreateFlagBits, VkDescriptorPoolCreateFlags> DescriptorPoolCreateFlags;

  inline DescriptorPoolCreateFlags operator|( DescriptorPoolCreateFlagBits bit0, DescriptorPoolCreateFlagBits bit1 )
  {
    return DescriptorPoolCreateFlags( bit0 ) | bit1;
  }

  class DescriptorPoolCreateInfo
  {
  public:
    DescriptorPoolCreateInfo()
      : DescriptorPoolCreateInfo( DescriptorPoolCreateFlags(), 0, 0, nullptr )
    {}

    DescriptorPoolCreateInfo( DescriptorPoolCreateFlags flags, uint32_t maxSets, uint32_t poolSizeCount, const DescriptorPoolSize* pPoolSizes)
    {
      m_descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      m_descriptorPoolCreateInfo.pNext = nullptr;
      m_descriptorPoolCreateInfo.flags = static_cast<VkDescriptorPoolCreateFlags>( flags );
      m_descriptorPoolCreateInfo.maxSets = maxSets;
      m_descriptorPoolCreateInfo.poolSizeCount = poolSizeCount;
      m_descriptorPoolCreateInfo.pPoolSizes = reinterpret_cast<const VkDescriptorPoolSize*>( pPoolSizes );
    }

    DescriptorPoolCreateInfo(VkDescriptorPoolCreateInfo const & rhs)
      : m_descriptorPoolCreateInfo(rhs)
    {
    }

    DescriptorPoolCreateInfo& operator=(VkDescriptorPoolCreateInfo const & rhs)
    {
      m_descriptorPoolCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_descriptorPoolCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_descriptorPoolCreateInfo.sType );
    }

    DescriptorPoolCreateInfo& sType( StructureType sType )
    {
      m_descriptorPoolCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_descriptorPoolCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_descriptorPoolCreateInfo.pNext );
    }

    DescriptorPoolCreateInfo& pNext( const void* pNext )
    {
      m_descriptorPoolCreateInfo.pNext = pNext;
      return *this;
    }

    const DescriptorPoolCreateFlags& flags() const
    {
      return reinterpret_cast<const DescriptorPoolCreateFlags&>( m_descriptorPoolCreateInfo.flags );
    }

    DescriptorPoolCreateFlags& flags()
    {
      return reinterpret_cast<DescriptorPoolCreateFlags&>( m_descriptorPoolCreateInfo.flags );
    }

    DescriptorPoolCreateInfo& flags( DescriptorPoolCreateFlags flags )
    {
      m_descriptorPoolCreateInfo.flags = static_cast<VkDescriptorPoolCreateFlags>( flags );
      return *this;
    }

    const uint32_t& maxSets() const
    {
      return m_descriptorPoolCreateInfo.maxSets;
    }

    uint32_t& maxSets()
    {
      return m_descriptorPoolCreateInfo.maxSets;
    }

    DescriptorPoolCreateInfo& maxSets( uint32_t maxSets )
    {
      m_descriptorPoolCreateInfo.maxSets = maxSets;
      return *this;
    }

    const uint32_t& poolSizeCount() const
    {
      return m_descriptorPoolCreateInfo.poolSizeCount;
    }

    uint32_t& poolSizeCount()
    {
      return m_descriptorPoolCreateInfo.poolSizeCount;
    }

    DescriptorPoolCreateInfo& poolSizeCount( uint32_t poolSizeCount )
    {
      m_descriptorPoolCreateInfo.poolSizeCount = poolSizeCount;
      return *this;
    }

    const DescriptorPoolSize* pPoolSizes() const
    {
      return reinterpret_cast<const DescriptorPoolSize*>( m_descriptorPoolCreateInfo.pPoolSizes );
    }

    const DescriptorPoolSize* pPoolSizes()
    {
      return reinterpret_cast<const DescriptorPoolSize*>( m_descriptorPoolCreateInfo.pPoolSizes );
    }

    DescriptorPoolCreateInfo& pPoolSizes( const DescriptorPoolSize* pPoolSizes )
    {
      m_descriptorPoolCreateInfo.pPoolSizes = reinterpret_cast<const VkDescriptorPoolSize*>( pPoolSizes );
      return *this;
    }

    static DescriptorPoolCreateInfo& null()
    {
      return *((DescriptorPoolCreateInfo*)(nullptr));
    }

    operator const VkDescriptorPoolCreateInfo&() const
    {
      return m_descriptorPoolCreateInfo;
    }

  private:
    VkDescriptorPoolCreateInfo m_descriptorPoolCreateInfo;
  };
  static_assert( sizeof( DescriptorPoolCreateInfo ) == sizeof( VkDescriptorPoolCreateInfo ), "struct and wrapper have different size!" );

  enum class DependencyFlagBits
  {
    eByRegion = VK_DEPENDENCY_BY_REGION_BIT
  };

  typedef Flags<DependencyFlagBits, VkDependencyFlags> DependencyFlags;

  inline DependencyFlags operator|( DependencyFlagBits bit0, DependencyFlagBits bit1 )
  {
    return DependencyFlags( bit0 ) | bit1;
  }

  class CommandBuffer
  {
  public:
    CommandBuffer()
      : m_commandBuffer(VK_NULL_HANDLE)
    {}

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    CommandBuffer(VkCommandBuffer commandBuffer)
       : m_commandBuffer(commandBuffer)
    {}

    CommandBuffer& operator=(VkCommandBuffer commandBuffer)
    {
      m_commandBuffer = commandBuffer;
      return *this;
    }
#endif

    Result begin( const CommandBufferBeginInfo* pBeginInfo ) const
    {
      return static_cast<Result>( vkBeginCommandBuffer( m_commandBuffer, reinterpret_cast<const VkCommandBufferBeginInfo*>( pBeginInfo ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void begin( const CommandBufferBeginInfo& beginInfo ) const
    {
      Result result = static_cast<Result>( vkBeginCommandBuffer( m_commandBuffer, reinterpret_cast<const VkCommandBufferBeginInfo*>( &beginInfo ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::CommandBuffer::begin" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    Result end(  ) const
    {
      return static_cast<Result>( vkEndCommandBuffer( m_commandBuffer ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void end(  ) const
    {
      Result result = static_cast<Result>( vkEndCommandBuffer( m_commandBuffer ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::CommandBuffer::end" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    Result reset( CommandBufferResetFlags flags ) const
    {
      return static_cast<Result>( vkResetCommandBuffer( m_commandBuffer, static_cast<VkCommandBufferResetFlags>( flags ) ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void reset( CommandBufferResetFlags flags ) const
    {
      Result result = static_cast<Result>( vkResetCommandBuffer( m_commandBuffer, static_cast<VkCommandBufferResetFlags>( flags ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::CommandBuffer::reset" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void bindPipeline( PipelineBindPoint pipelineBindPoint, Pipeline pipeline ) const
    {
      vkCmdBindPipeline( m_commandBuffer, static_cast<VkPipelineBindPoint>( pipelineBindPoint ), static_cast<VkPipeline>( pipeline ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void bindPipeline( PipelineBindPoint pipelineBindPoint, Pipeline pipeline ) const
    {
      vkCmdBindPipeline( m_commandBuffer, static_cast<VkPipelineBindPoint>( pipelineBindPoint ), static_cast<VkPipeline>( pipeline ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void setViewport( uint32_t firstViewport, uint32_t viewportCount, const Viewport* pViewports ) const
    {
      vkCmdSetViewport( m_commandBuffer, firstViewport, viewportCount, reinterpret_cast<const VkViewport*>( pViewports ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void setViewport( uint32_t firstViewport, std::vector<Viewport> const& viewports ) const
    {
      vkCmdSetViewport( m_commandBuffer, firstViewport, static_cast<uint32_t>( viewports.size() ), reinterpret_cast<const VkViewport*>( viewports.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void setScissor( uint32_t firstScissor, uint32_t scissorCount, const Rect2D* pScissors ) const
    {
      vkCmdSetScissor( m_commandBuffer, firstScissor, scissorCount, reinterpret_cast<const VkRect2D*>( pScissors ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void setScissor( uint32_t firstScissor, std::vector<Rect2D> const& scissors ) const
    {
      vkCmdSetScissor( m_commandBuffer, firstScissor, static_cast<uint32_t>( scissors.size() ), reinterpret_cast<const VkRect2D*>( scissors.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void setLineWidth( float lineWidth ) const
    {
      vkCmdSetLineWidth( m_commandBuffer, lineWidth );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void setLineWidth( float lineWidth ) const
    {
      vkCmdSetLineWidth( m_commandBuffer, lineWidth );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void setDepthBias( float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor ) const
    {
      vkCmdSetDepthBias( m_commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void setDepthBias( float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor ) const
    {
      vkCmdSetDepthBias( m_commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void setBlendConstants( const float blendConstants[4] ) const
    {
      vkCmdSetBlendConstants( m_commandBuffer, blendConstants );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void setBlendConstants( const float blendConstants[4] ) const
    {
      vkCmdSetBlendConstants( m_commandBuffer, blendConstants );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void setDepthBounds( float minDepthBounds, float maxDepthBounds ) const
    {
      vkCmdSetDepthBounds( m_commandBuffer, minDepthBounds, maxDepthBounds );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void setDepthBounds( float minDepthBounds, float maxDepthBounds ) const
    {
      vkCmdSetDepthBounds( m_commandBuffer, minDepthBounds, maxDepthBounds );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void setStencilCompareMask( StencilFaceFlags faceMask, uint32_t compareMask ) const
    {
      vkCmdSetStencilCompareMask( m_commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), compareMask );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void setStencilCompareMask( StencilFaceFlags faceMask, uint32_t compareMask ) const
    {
      vkCmdSetStencilCompareMask( m_commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), compareMask );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void setStencilWriteMask( StencilFaceFlags faceMask, uint32_t writeMask ) const
    {
      vkCmdSetStencilWriteMask( m_commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), writeMask );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void setStencilWriteMask( StencilFaceFlags faceMask, uint32_t writeMask ) const
    {
      vkCmdSetStencilWriteMask( m_commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), writeMask );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void setStencilReference( StencilFaceFlags faceMask, uint32_t reference ) const
    {
      vkCmdSetStencilReference( m_commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), reference );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void setStencilReference( StencilFaceFlags faceMask, uint32_t reference ) const
    {
      vkCmdSetStencilReference( m_commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), reference );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void bindDescriptorSets( PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets ) const
    {
      vkCmdBindDescriptorSets( m_commandBuffer, static_cast<VkPipelineBindPoint>( pipelineBindPoint ), static_cast<VkPipelineLayout>( layout ), firstSet, descriptorSetCount, reinterpret_cast<const VkDescriptorSet*>( pDescriptorSets ), dynamicOffsetCount, pDynamicOffsets );
    }

#ifdef VKCPP_ENHANCED_MODE
    void bindDescriptorSets( PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, std::vector<DescriptorSet> const& descriptorSets, std::vector<uint32_t> const& dynamicOffsets ) const
    {
      vkCmdBindDescriptorSets( m_commandBuffer, static_cast<VkPipelineBindPoint>( pipelineBindPoint ), static_cast<VkPipelineLayout>( layout ), firstSet, static_cast<uint32_t>( descriptorSets.size() ), reinterpret_cast<const VkDescriptorSet*>( descriptorSets.data() ), static_cast<uint32_t>( dynamicOffsets.size() ), dynamicOffsets.data() );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void bindIndexBuffer( Buffer buffer, DeviceSize offset, IndexType indexType ) const
    {
      vkCmdBindIndexBuffer( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset, static_cast<VkIndexType>( indexType ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void bindIndexBuffer( Buffer buffer, DeviceSize offset, IndexType indexType ) const
    {
      vkCmdBindIndexBuffer( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset, static_cast<VkIndexType>( indexType ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void bindVertexBuffers( uint32_t firstBinding, uint32_t bindingCount, const Buffer* pBuffers, const DeviceSize* pOffsets ) const
    {
      vkCmdBindVertexBuffers( m_commandBuffer, firstBinding, bindingCount, reinterpret_cast<const VkBuffer*>( pBuffers ), pOffsets );
    }

#ifdef VKCPP_ENHANCED_MODE
    void bindVertexBuffers( uint32_t firstBinding, std::vector<Buffer> const& buffers, std::vector<DeviceSize> const& offsets ) const
    {
      if ( buffers.size() != offsets.size() )
      {
        throw std::logic_error( "vk::CommandBuffer::bindVertexBuffers: buffers.size() != offsets.size()" );
      }
      vkCmdBindVertexBuffers( m_commandBuffer, firstBinding, static_cast<uint32_t>( buffers.size() ), reinterpret_cast<const VkBuffer*>( buffers.data() ), offsets.data() );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void draw( uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance ) const
    {
      vkCmdDraw( m_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void draw( uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance ) const
    {
      vkCmdDraw( m_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void drawIndexed( uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance ) const
    {
      vkCmdDrawIndexed( m_commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void drawIndexed( uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance ) const
    {
      vkCmdDrawIndexed( m_commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void drawIndirect( Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride ) const
    {
      vkCmdDrawIndirect( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset, drawCount, stride );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void drawIndirect( Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride ) const
    {
      vkCmdDrawIndirect( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset, drawCount, stride );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void drawIndexedIndirect( Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride ) const
    {
      vkCmdDrawIndexedIndirect( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset, drawCount, stride );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void drawIndexedIndirect( Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride ) const
    {
      vkCmdDrawIndexedIndirect( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset, drawCount, stride );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void dispatch( uint32_t x, uint32_t y, uint32_t z ) const
    {
      vkCmdDispatch( m_commandBuffer, x, y, z );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void dispatch( uint32_t x, uint32_t y, uint32_t z ) const
    {
      vkCmdDispatch( m_commandBuffer, x, y, z );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void dispatchIndirect( Buffer buffer, DeviceSize offset ) const
    {
      vkCmdDispatchIndirect( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void dispatchIndirect( Buffer buffer, DeviceSize offset ) const
    {
      vkCmdDispatchIndirect( m_commandBuffer, static_cast<VkBuffer>( buffer ), offset );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void copyBuffer( Buffer srcBuffer, Buffer dstBuffer, uint32_t regionCount, const BufferCopy* pRegions ) const
    {
      vkCmdCopyBuffer( m_commandBuffer, static_cast<VkBuffer>( srcBuffer ), static_cast<VkBuffer>( dstBuffer ), regionCount, reinterpret_cast<const VkBufferCopy*>( pRegions ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void copyBuffer( Buffer srcBuffer, Buffer dstBuffer, std::vector<BufferCopy> const& regions ) const
    {
      vkCmdCopyBuffer( m_commandBuffer, static_cast<VkBuffer>( srcBuffer ), static_cast<VkBuffer>( dstBuffer ), static_cast<uint32_t>( regions.size() ), reinterpret_cast<const VkBufferCopy*>( regions.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void copyImage( Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageCopy* pRegions ) const
    {
      vkCmdCopyImage( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkImageCopy*>( pRegions ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void copyImage( Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, std::vector<ImageCopy> const& regions ) const
    {
      vkCmdCopyImage( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), static_cast<uint32_t>( regions.size() ), reinterpret_cast<const VkImageCopy*>( regions.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void blitImage( Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageBlit* pRegions, Filter filter ) const
    {
      vkCmdBlitImage( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkImageBlit*>( pRegions ), static_cast<VkFilter>( filter ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void blitImage( Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, std::vector<ImageBlit> const& regions, Filter filter ) const
    {
      vkCmdBlitImage( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), static_cast<uint32_t>( regions.size() ), reinterpret_cast<const VkImageBlit*>( regions.data() ), static_cast<VkFilter>( filter ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void copyBufferToImage( Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const BufferImageCopy* pRegions ) const
    {
      vkCmdCopyBufferToImage( m_commandBuffer, static_cast<VkBuffer>( srcBuffer ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkBufferImageCopy*>( pRegions ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void copyBufferToImage( Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, std::vector<BufferImageCopy> const& regions ) const
    {
      vkCmdCopyBufferToImage( m_commandBuffer, static_cast<VkBuffer>( srcBuffer ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), static_cast<uint32_t>( regions.size() ), reinterpret_cast<const VkBufferImageCopy*>( regions.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void copyImageToBuffer( Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, uint32_t regionCount, const BufferImageCopy* pRegions ) const
    {
      vkCmdCopyImageToBuffer( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkBuffer>( dstBuffer ), regionCount, reinterpret_cast<const VkBufferImageCopy*>( pRegions ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void copyImageToBuffer( Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, std::vector<BufferImageCopy> const& regions ) const
    {
      vkCmdCopyImageToBuffer( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkBuffer>( dstBuffer ), static_cast<uint32_t>( regions.size() ), reinterpret_cast<const VkBufferImageCopy*>( regions.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void updateBuffer( Buffer dstBuffer, DeviceSize dstOffset, DeviceSize dataSize, const uint32_t* pData ) const
    {
      vkCmdUpdateBuffer( m_commandBuffer, static_cast<VkBuffer>( dstBuffer ), dstOffset, dataSize, pData );
    }

#ifdef VKCPP_ENHANCED_MODE
    template <typename T>
    void updateBuffer( Buffer dstBuffer, DeviceSize dstOffset, std::vector<T> const& data ) const
    {
      static_assert( sizeof( T ) % sizeof( uint32_t ) == 0, "wrong size of template type T" );
      vkCmdUpdateBuffer( m_commandBuffer, static_cast<VkBuffer>( dstBuffer ), dstOffset, static_cast<DeviceSize>( data.size() * sizeof( T ) ), reinterpret_cast<const uint32_t*>( data.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void fillBuffer( Buffer dstBuffer, DeviceSize dstOffset, DeviceSize size, uint32_t data ) const
    {
      vkCmdFillBuffer( m_commandBuffer, static_cast<VkBuffer>( dstBuffer ), dstOffset, size, data );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void fillBuffer( Buffer dstBuffer, DeviceSize dstOffset, DeviceSize size, uint32_t data ) const
    {
      vkCmdFillBuffer( m_commandBuffer, static_cast<VkBuffer>( dstBuffer ), dstOffset, size, data );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void clearColorImage( Image image, ImageLayout imageLayout, const ClearColorValue* pColor, uint32_t rangeCount, const ImageSubresourceRange* pRanges ) const
    {
      vkCmdClearColorImage( m_commandBuffer, static_cast<VkImage>( image ), static_cast<VkImageLayout>( imageLayout ), reinterpret_cast<const VkClearColorValue*>( pColor ), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>( pRanges ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void clearColorImage( Image image, ImageLayout imageLayout, const ClearColorValue& color, std::vector<ImageSubresourceRange> const& ranges ) const
    {
      vkCmdClearColorImage( m_commandBuffer, static_cast<VkImage>( image ), static_cast<VkImageLayout>( imageLayout ), reinterpret_cast<const VkClearColorValue*>( &color ), static_cast<uint32_t>( ranges.size() ), reinterpret_cast<const VkImageSubresourceRange*>( ranges.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void clearDepthStencilImage( Image image, ImageLayout imageLayout, const ClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const ImageSubresourceRange* pRanges ) const
    {
      vkCmdClearDepthStencilImage( m_commandBuffer, static_cast<VkImage>( image ), static_cast<VkImageLayout>( imageLayout ), reinterpret_cast<const VkClearDepthStencilValue*>( pDepthStencil ), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>( pRanges ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void clearDepthStencilImage( Image image, ImageLayout imageLayout, const ClearDepthStencilValue& depthStencil, std::vector<ImageSubresourceRange> const& ranges ) const
    {
      vkCmdClearDepthStencilImage( m_commandBuffer, static_cast<VkImage>( image ), static_cast<VkImageLayout>( imageLayout ), reinterpret_cast<const VkClearDepthStencilValue*>( &depthStencil ), static_cast<uint32_t>( ranges.size() ), reinterpret_cast<const VkImageSubresourceRange*>( ranges.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void clearAttachments( uint32_t attachmentCount, const ClearAttachment* pAttachments, uint32_t rectCount, const ClearRect* pRects ) const
    {
      vkCmdClearAttachments( m_commandBuffer, attachmentCount, reinterpret_cast<const VkClearAttachment*>( pAttachments ), rectCount, reinterpret_cast<const VkClearRect*>( pRects ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void clearAttachments( std::vector<ClearAttachment> const& attachments, std::vector<ClearRect> const& rects ) const
    {
      vkCmdClearAttachments( m_commandBuffer, static_cast<uint32_t>( attachments.size() ), reinterpret_cast<const VkClearAttachment*>( attachments.data() ), static_cast<uint32_t>( rects.size() ), reinterpret_cast<const VkClearRect*>( rects.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void resolveImage( Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageResolve* pRegions ) const
    {
      vkCmdResolveImage( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkImageResolve*>( pRegions ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void resolveImage( Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, std::vector<ImageResolve> const& regions ) const
    {
      vkCmdResolveImage( m_commandBuffer, static_cast<VkImage>( srcImage ), static_cast<VkImageLayout>( srcImageLayout ), static_cast<VkImage>( dstImage ), static_cast<VkImageLayout>( dstImageLayout ), static_cast<uint32_t>( regions.size() ), reinterpret_cast<const VkImageResolve*>( regions.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void setEvent( Event event, PipelineStageFlags stageMask ) const
    {
      vkCmdSetEvent( m_commandBuffer, static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags>( stageMask ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void setEvent( Event event, PipelineStageFlags stageMask ) const
    {
      vkCmdSetEvent( m_commandBuffer, static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags>( stageMask ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void resetEvent( Event event, PipelineStageFlags stageMask ) const
    {
      vkCmdResetEvent( m_commandBuffer, static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags>( stageMask ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void resetEvent( Event event, PipelineStageFlags stageMask ) const
    {
      vkCmdResetEvent( m_commandBuffer, static_cast<VkEvent>( event ), static_cast<VkPipelineStageFlags>( stageMask ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void waitEvents( uint32_t eventCount, const Event* pEvents, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers ) const
    {
      vkCmdWaitEvents( m_commandBuffer, eventCount, reinterpret_cast<const VkEvent*>( pEvents ), static_cast<VkPipelineStageFlags>( srcStageMask ), static_cast<VkPipelineStageFlags>( dstStageMask ), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>( pMemoryBarriers ), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>( pBufferMemoryBarriers ), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>( pImageMemoryBarriers ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void waitEvents( std::vector<Event> const& events, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, std::vector<MemoryBarrier> const& memoryBarriers, std::vector<BufferMemoryBarrier> const& bufferMemoryBarriers, std::vector<ImageMemoryBarrier> const& imageMemoryBarriers ) const
    {
      vkCmdWaitEvents( m_commandBuffer, static_cast<uint32_t>( events.size() ), reinterpret_cast<const VkEvent*>( events.data() ), static_cast<VkPipelineStageFlags>( srcStageMask ), static_cast<VkPipelineStageFlags>( dstStageMask ), static_cast<uint32_t>( memoryBarriers.size() ), reinterpret_cast<const VkMemoryBarrier*>( memoryBarriers.data() ), static_cast<uint32_t>( bufferMemoryBarriers.size() ), reinterpret_cast<const VkBufferMemoryBarrier*>( bufferMemoryBarriers.data() ), static_cast<uint32_t>( imageMemoryBarriers.size() ), reinterpret_cast<const VkImageMemoryBarrier*>( imageMemoryBarriers.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void pipelineBarrier( PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers ) const
    {
      vkCmdPipelineBarrier( m_commandBuffer, static_cast<VkPipelineStageFlags>( srcStageMask ), static_cast<VkPipelineStageFlags>( dstStageMask ), static_cast<VkDependencyFlags>( dependencyFlags ), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>( pMemoryBarriers ), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>( pBufferMemoryBarriers ), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>( pImageMemoryBarriers ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void pipelineBarrier( PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, std::vector<MemoryBarrier> const& memoryBarriers, std::vector<BufferMemoryBarrier> const& bufferMemoryBarriers, std::vector<ImageMemoryBarrier> const& imageMemoryBarriers ) const
    {
      vkCmdPipelineBarrier( m_commandBuffer, static_cast<VkPipelineStageFlags>( srcStageMask ), static_cast<VkPipelineStageFlags>( dstStageMask ), static_cast<VkDependencyFlags>( dependencyFlags ), static_cast<uint32_t>( memoryBarriers.size() ), reinterpret_cast<const VkMemoryBarrier*>( memoryBarriers.data() ), static_cast<uint32_t>( bufferMemoryBarriers.size() ), reinterpret_cast<const VkBufferMemoryBarrier*>( bufferMemoryBarriers.data() ), static_cast<uint32_t>( imageMemoryBarriers.size() ), reinterpret_cast<const VkImageMemoryBarrier*>( imageMemoryBarriers.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void beginQuery( QueryPool queryPool, uint32_t query, QueryControlFlags flags ) const
    {
      vkCmdBeginQuery( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), query, static_cast<VkQueryControlFlags>( flags ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void beginQuery( QueryPool queryPool, uint32_t query, QueryControlFlags flags ) const
    {
      vkCmdBeginQuery( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), query, static_cast<VkQueryControlFlags>( flags ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void endQuery( QueryPool queryPool, uint32_t query ) const
    {
      vkCmdEndQuery( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), query );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void endQuery( QueryPool queryPool, uint32_t query ) const
    {
      vkCmdEndQuery( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), query );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void resetQueryPool( QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) const
    {
      vkCmdResetQueryPool( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), firstQuery, queryCount );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void resetQueryPool( QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) const
    {
      vkCmdResetQueryPool( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), firstQuery, queryCount );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void writeTimestamp( PipelineStageFlagBits pipelineStage, QueryPool queryPool, uint32_t query ) const
    {
      vkCmdWriteTimestamp( m_commandBuffer, static_cast<VkPipelineStageFlagBits>( pipelineStage ), static_cast<VkQueryPool>( queryPool ), query );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void writeTimestamp( PipelineStageFlagBits pipelineStage, QueryPool queryPool, uint32_t query ) const
    {
      vkCmdWriteTimestamp( m_commandBuffer, static_cast<VkPipelineStageFlagBits>( pipelineStage ), static_cast<VkQueryPool>( queryPool ), query );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void copyQueryPoolResults( QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize stride, QueryResultFlags flags ) const
    {
      vkCmdCopyQueryPoolResults( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), firstQuery, queryCount, static_cast<VkBuffer>( dstBuffer ), dstOffset, stride, static_cast<VkQueryResultFlags>( flags ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void copyQueryPoolResults( QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize stride, QueryResultFlags flags ) const
    {
      vkCmdCopyQueryPoolResults( m_commandBuffer, static_cast<VkQueryPool>( queryPool ), firstQuery, queryCount, static_cast<VkBuffer>( dstBuffer ), dstOffset, stride, static_cast<VkQueryResultFlags>( flags ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void pushConstants( PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues ) const
    {
      vkCmdPushConstants( m_commandBuffer, static_cast<VkPipelineLayout>( layout ), static_cast<VkShaderStageFlags>( stageFlags ), offset, size, pValues );
    }

#ifdef VKCPP_ENHANCED_MODE
    void pushConstants( PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, std::vector<uint8_t> const& values ) const
    {
      vkCmdPushConstants( m_commandBuffer, static_cast<VkPipelineLayout>( layout ), static_cast<VkShaderStageFlags>( stageFlags ), offset, static_cast<uint32_t>( values.size() ), values.data() );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void beginRenderPass( const RenderPassBeginInfo* pRenderPassBegin, SubpassContents contents ) const
    {
      vkCmdBeginRenderPass( m_commandBuffer, reinterpret_cast<const VkRenderPassBeginInfo*>( pRenderPassBegin ), static_cast<VkSubpassContents>( contents ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void beginRenderPass( const RenderPassBeginInfo& renderPassBegin, SubpassContents contents ) const
    {
      vkCmdBeginRenderPass( m_commandBuffer, reinterpret_cast<const VkRenderPassBeginInfo*>( &renderPassBegin ), static_cast<VkSubpassContents>( contents ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void nextSubpass( SubpassContents contents ) const
    {
      vkCmdNextSubpass( m_commandBuffer, static_cast<VkSubpassContents>( contents ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void nextSubpass( SubpassContents contents ) const
    {
      vkCmdNextSubpass( m_commandBuffer, static_cast<VkSubpassContents>( contents ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void endRenderPass(  ) const
    {
      vkCmdEndRenderPass( m_commandBuffer );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void endRenderPass(  ) const
    {
      vkCmdEndRenderPass( m_commandBuffer );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void executeCommands( uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers ) const
    {
      vkCmdExecuteCommands( m_commandBuffer, commandBufferCount, reinterpret_cast<const VkCommandBuffer*>( pCommandBuffers ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void executeCommands( std::vector<CommandBuffer> const& commandBuffers ) const
    {
      vkCmdExecuteCommands( m_commandBuffer, static_cast<uint32_t>( commandBuffers.size() ), reinterpret_cast<const VkCommandBuffer*>( commandBuffers.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

  class SubpassDependency
  {
  public:
    SubpassDependency()
      : SubpassDependency( 0, 0, PipelineStageFlags(), PipelineStageFlags(), AccessFlags(), AccessFlags(), DependencyFlags() )
    {}

    SubpassDependency( uint32_t srcSubpass, uint32_t dstSubpass, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, AccessFlags srcAccessMask, AccessFlags dstAccessMask, DependencyFlags dependencyFlags)
    {
      m_subpassDependency.srcSubpass = srcSubpass;
      m_subpassDependency.dstSubpass = dstSubpass;
      m_subpassDependency.srcStageMask = static_cast<VkPipelineStageFlags>( srcStageMask );
      m_subpassDependency.dstStageMask = static_cast<VkPipelineStageFlags>( dstStageMask );
      m_subpassDependency.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask );
      m_subpassDependency.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask );
      m_subpassDependency.dependencyFlags = static_cast<VkDependencyFlags>( dependencyFlags );
    }

    SubpassDependency(VkSubpassDependency const & rhs)
      : m_subpassDependency(rhs)
    {
    }

    SubpassDependency& operator=(VkSubpassDependency const & rhs)
    {
      m_subpassDependency = rhs;
      return *this;
    }

    const uint32_t& srcSubpass() const
    {
      return m_subpassDependency.srcSubpass;
    }

    uint32_t& srcSubpass()
    {
      return m_subpassDependency.srcSubpass;
    }

    SubpassDependency& srcSubpass( uint32_t srcSubpass )
    {
      m_subpassDependency.srcSubpass = srcSubpass;
      return *this;
    }

    const uint32_t& dstSubpass() const
    {
      return m_subpassDependency.dstSubpass;
    }

    uint32_t& dstSubpass()
    {
      return m_subpassDependency.dstSubpass;
    }

    SubpassDependency& dstSubpass( uint32_t dstSubpass )
    {
      m_subpassDependency.dstSubpass = dstSubpass;
      return *this;
    }

    const PipelineStageFlags& srcStageMask() const
    {
      return reinterpret_cast<const PipelineStageFlags&>( m_subpassDependency.srcStageMask );
    }

    PipelineStageFlags& srcStageMask()
    {
      return reinterpret_cast<PipelineStageFlags&>( m_subpassDependency.srcStageMask );
    }

    SubpassDependency& srcStageMask( PipelineStageFlags srcStageMask )
    {
      m_subpassDependency.srcStageMask = static_cast<VkPipelineStageFlags>( srcStageMask );
      return *this;
    }

    const PipelineStageFlags& dstStageMask() const
    {
      return reinterpret_cast<const PipelineStageFlags&>( m_subpassDependency.dstStageMask );
    }

    PipelineStageFlags& dstStageMask()
    {
      return reinterpret_cast<PipelineStageFlags&>( m_subpassDependency.dstStageMask );
    }

    SubpassDependency& dstStageMask( PipelineStageFlags dstStageMask )
    {
      m_subpassDependency.dstStageMask = static_cast<VkPipelineStageFlags>( dstStageMask );
      return *this;
    }

    const AccessFlags& srcAccessMask() const
    {
      return reinterpret_cast<const AccessFlags&>( m_subpassDependency.srcAccessMask );
    }

    AccessFlags& srcAccessMask()
    {
      return reinterpret_cast<AccessFlags&>( m_subpassDependency.srcAccessMask );
    }

    SubpassDependency& srcAccessMask( AccessFlags srcAccessMask )
    {
      m_subpassDependency.srcAccessMask = static_cast<VkAccessFlags>( srcAccessMask );
      return *this;
    }

    const AccessFlags& dstAccessMask() const
    {
      return reinterpret_cast<const AccessFlags&>( m_subpassDependency.dstAccessMask );
    }

    AccessFlags& dstAccessMask()
    {
      return reinterpret_cast<AccessFlags&>( m_subpassDependency.dstAccessMask );
    }

    SubpassDependency& dstAccessMask( AccessFlags dstAccessMask )
    {
      m_subpassDependency.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask );
      return *this;
    }

    const DependencyFlags& dependencyFlags() const
    {
      return reinterpret_cast<const DependencyFlags&>( m_subpassDependency.dependencyFlags );
    }

    DependencyFlags& dependencyFlags()
    {
      return reinterpret_cast<DependencyFlags&>( m_subpassDependency.dependencyFlags );
    }

    SubpassDependency& dependencyFlags( DependencyFlags dependencyFlags )
    {
      m_subpassDependency.dependencyFlags = static_cast<VkDependencyFlags>( dependencyFlags );
      return *this;
    }

    static SubpassDependency& null()
    {
      return *((SubpassDependency*)(nullptr));
    }

    operator const VkSubpassDependency&() const
    {
      return m_subpassDependency;
    }

  private:
    VkSubpassDependency m_subpassDependency;
  };
  static_assert( sizeof( SubpassDependency ) == sizeof( VkSubpassDependency ), "struct and wrapper have different size!" );

  class RenderPassCreateInfo
  {
  public:
    RenderPassCreateInfo()
      : RenderPassCreateInfo( RenderPassCreateFlags(), 0, nullptr, 0, nullptr, 0, nullptr )
    {}

    RenderPassCreateInfo( RenderPassCreateFlags flags, uint32_t attachmentCount, const AttachmentDescription* pAttachments, uint32_t subpassCount, const SubpassDescription* pSubpasses, uint32_t dependencyCount, const SubpassDependency* pDependencies)
    {
      m_renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
      m_renderPassCreateInfo.pNext = nullptr;
      m_renderPassCreateInfo.flags = static_cast<VkRenderPassCreateFlags>( flags );
      m_renderPassCreateInfo.attachmentCount = attachmentCount;
      m_renderPassCreateInfo.pAttachments = reinterpret_cast<const VkAttachmentDescription*>( pAttachments );
      m_renderPassCreateInfo.subpassCount = subpassCount;
      m_renderPassCreateInfo.pSubpasses = reinterpret_cast<const VkSubpassDescription*>( pSubpasses );
      m_renderPassCreateInfo.dependencyCount = dependencyCount;
      m_renderPassCreateInfo.pDependencies = reinterpret_cast<const VkSubpassDependency*>( pDependencies );
    }

    RenderPassCreateInfo(VkRenderPassCreateInfo const & rhs)
      : m_renderPassCreateInfo(rhs)
    {
    }

    RenderPassCreateInfo& operator=(VkRenderPassCreateInfo const & rhs)
    {
      m_renderPassCreateInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_renderPassCreateInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_renderPassCreateInfo.sType );
    }

    RenderPassCreateInfo& sType( StructureType sType )
    {
      m_renderPassCreateInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_renderPassCreateInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_renderPassCreateInfo.pNext );
    }

    RenderPassCreateInfo& pNext( const void* pNext )
    {
      m_renderPassCreateInfo.pNext = pNext;
      return *this;
    }

    const RenderPassCreateFlags& flags() const
    {
      return reinterpret_cast<const RenderPassCreateFlags&>( m_renderPassCreateInfo.flags );
    }

    RenderPassCreateFlags& flags()
    {
      return reinterpret_cast<RenderPassCreateFlags&>( m_renderPassCreateInfo.flags );
    }

    RenderPassCreateInfo& flags( RenderPassCreateFlags flags )
    {
      m_renderPassCreateInfo.flags = static_cast<VkRenderPassCreateFlags>( flags );
      return *this;
    }

    const uint32_t& attachmentCount() const
    {
      return m_renderPassCreateInfo.attachmentCount;
    }

    uint32_t& attachmentCount()
    {
      return m_renderPassCreateInfo.attachmentCount;
    }

    RenderPassCreateInfo& attachmentCount( uint32_t attachmentCount )
    {
      m_renderPassCreateInfo.attachmentCount = attachmentCount;
      return *this;
    }

    const AttachmentDescription* pAttachments() const
    {
      return reinterpret_cast<const AttachmentDescription*>( m_renderPassCreateInfo.pAttachments );
    }

    const AttachmentDescription* pAttachments()
    {
      return reinterpret_cast<const AttachmentDescription*>( m_renderPassCreateInfo.pAttachments );
    }

    RenderPassCreateInfo& pAttachments( const AttachmentDescription* pAttachments )
    {
      m_renderPassCreateInfo.pAttachments = reinterpret_cast<const VkAttachmentDescription*>( pAttachments );
      return *this;
    }

    const uint32_t& subpassCount() const
    {
      return m_renderPassCreateInfo.subpassCount;
    }

    uint32_t& subpassCount()
    {
      return m_renderPassCreateInfo.subpassCount;
    }

    RenderPassCreateInfo& subpassCount( uint32_t subpassCount )
    {
      m_renderPassCreateInfo.subpassCount = subpassCount;
      return *this;
    }

    const SubpassDescription* pSubpasses() const
    {
      return reinterpret_cast<const SubpassDescription*>( m_renderPassCreateInfo.pSubpasses );
    }

    const SubpassDescription* pSubpasses()
    {
      return reinterpret_cast<const SubpassDescription*>( m_renderPassCreateInfo.pSubpasses );
    }

    RenderPassCreateInfo& pSubpasses( const SubpassDescription* pSubpasses )
    {
      m_renderPassCreateInfo.pSubpasses = reinterpret_cast<const VkSubpassDescription*>( pSubpasses );
      return *this;
    }

    const uint32_t& dependencyCount() const
    {
      return m_renderPassCreateInfo.dependencyCount;
    }

    uint32_t& dependencyCount()
    {
      return m_renderPassCreateInfo.dependencyCount;
    }

    RenderPassCreateInfo& dependencyCount( uint32_t dependencyCount )
    {
      m_renderPassCreateInfo.dependencyCount = dependencyCount;
      return *this;
    }

    const SubpassDependency* pDependencies() const
    {
      return reinterpret_cast<const SubpassDependency*>( m_renderPassCreateInfo.pDependencies );
    }

    const SubpassDependency* pDependencies()
    {
      return reinterpret_cast<const SubpassDependency*>( m_renderPassCreateInfo.pDependencies );
    }

    RenderPassCreateInfo& pDependencies( const SubpassDependency* pDependencies )
    {
      m_renderPassCreateInfo.pDependencies = reinterpret_cast<const VkSubpassDependency*>( pDependencies );
      return *this;
    }

    static RenderPassCreateInfo& null()
    {
      return *((RenderPassCreateInfo*)(nullptr));
    }

    operator const VkRenderPassCreateInfo&() const
    {
      return m_renderPassCreateInfo;
    }

  private:
    VkRenderPassCreateInfo m_renderPassCreateInfo;
  };
  static_assert( sizeof( RenderPassCreateInfo ) == sizeof( VkRenderPassCreateInfo ), "struct and wrapper have different size!" );

  class SubmitInfo
  {
  public:
    SubmitInfo()
      : SubmitInfo( 0, nullptr, nullptr, 0, nullptr, 0, nullptr )
    {}

    SubmitInfo( uint32_t waitSemaphoreCount, const Semaphore* pWaitSemaphores, const PipelineStageFlags* pWaitDstStageMask, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers, uint32_t signalSemaphoreCount, const Semaphore* pSignalSemaphores)
    {
      m_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      m_submitInfo.pNext = nullptr;
      m_submitInfo.waitSemaphoreCount = waitSemaphoreCount;
      m_submitInfo.pWaitSemaphores = reinterpret_cast<const VkSemaphore*>( pWaitSemaphores );
      m_submitInfo.pWaitDstStageMask = reinterpret_cast<const VkPipelineStageFlags*>( pWaitDstStageMask );
      m_submitInfo.commandBufferCount = commandBufferCount;
      m_submitInfo.pCommandBuffers = reinterpret_cast<const VkCommandBuffer*>( pCommandBuffers );
      m_submitInfo.signalSemaphoreCount = signalSemaphoreCount;
      m_submitInfo.pSignalSemaphores = reinterpret_cast<const VkSemaphore*>( pSignalSemaphores );
    }

    SubmitInfo(VkSubmitInfo const & rhs)
      : m_submitInfo(rhs)
    {
    }

    SubmitInfo& operator=(VkSubmitInfo const & rhs)
    {
      m_submitInfo = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_submitInfo.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_submitInfo.sType );
    }

    SubmitInfo& sType( StructureType sType )
    {
      m_submitInfo.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_submitInfo.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_submitInfo.pNext );
    }

    SubmitInfo& pNext( const void* pNext )
    {
      m_submitInfo.pNext = pNext;
      return *this;
    }

    const uint32_t& waitSemaphoreCount() const
    {
      return m_submitInfo.waitSemaphoreCount;
    }

    uint32_t& waitSemaphoreCount()
    {
      return m_submitInfo.waitSemaphoreCount;
    }

    SubmitInfo& waitSemaphoreCount( uint32_t waitSemaphoreCount )
    {
      m_submitInfo.waitSemaphoreCount = waitSemaphoreCount;
      return *this;
    }

    const Semaphore* pWaitSemaphores() const
    {
      return reinterpret_cast<const Semaphore*>( m_submitInfo.pWaitSemaphores );
    }

    const Semaphore* pWaitSemaphores()
    {
      return reinterpret_cast<const Semaphore*>( m_submitInfo.pWaitSemaphores );
    }

    SubmitInfo& pWaitSemaphores( const Semaphore* pWaitSemaphores )
    {
      m_submitInfo.pWaitSemaphores = reinterpret_cast<const VkSemaphore*>( pWaitSemaphores );
      return *this;
    }

    const PipelineStageFlags* pWaitDstStageMask() const
    {
      return reinterpret_cast<const PipelineStageFlags*>( m_submitInfo.pWaitDstStageMask );
    }

    const PipelineStageFlags* pWaitDstStageMask()
    {
      return reinterpret_cast<const PipelineStageFlags*>( m_submitInfo.pWaitDstStageMask );
    }

    SubmitInfo& pWaitDstStageMask( const PipelineStageFlags* pWaitDstStageMask )
    {
      m_submitInfo.pWaitDstStageMask = reinterpret_cast<const VkPipelineStageFlags*>( pWaitDstStageMask );
      return *this;
    }

    const uint32_t& commandBufferCount() const
    {
      return m_submitInfo.commandBufferCount;
    }

    uint32_t& commandBufferCount()
    {
      return m_submitInfo.commandBufferCount;
    }

    SubmitInfo& commandBufferCount( uint32_t commandBufferCount )
    {
      m_submitInfo.commandBufferCount = commandBufferCount;
      return *this;
    }

    const CommandBuffer* pCommandBuffers() const
    {
      return reinterpret_cast<const CommandBuffer*>( m_submitInfo.pCommandBuffers );
    }

    const CommandBuffer* pCommandBuffers()
    {
      return reinterpret_cast<const CommandBuffer*>( m_submitInfo.pCommandBuffers );
    }

    SubmitInfo& pCommandBuffers( const CommandBuffer* pCommandBuffers )
    {
      m_submitInfo.pCommandBuffers = reinterpret_cast<const VkCommandBuffer*>( pCommandBuffers );
      return *this;
    }

    const uint32_t& signalSemaphoreCount() const
    {
      return m_submitInfo.signalSemaphoreCount;
    }

    uint32_t& signalSemaphoreCount()
    {
      return m_submitInfo.signalSemaphoreCount;
    }

    SubmitInfo& signalSemaphoreCount( uint32_t signalSemaphoreCount )
    {
      m_submitInfo.signalSemaphoreCount = signalSemaphoreCount;
      return *this;
    }

    const Semaphore* pSignalSemaphores() const
    {
      return reinterpret_cast<const Semaphore*>( m_submitInfo.pSignalSemaphores );
    }

    const Semaphore* pSignalSemaphores()
    {
      return reinterpret_cast<const Semaphore*>( m_submitInfo.pSignalSemaphores );
    }

    SubmitInfo& pSignalSemaphores( const Semaphore* pSignalSemaphores )
    {
      m_submitInfo.pSignalSemaphores = reinterpret_cast<const VkSemaphore*>( pSignalSemaphores );
      return *this;
    }

    static SubmitInfo& null()
    {
      return *((SubmitInfo*)(nullptr));
    }

    operator const VkSubmitInfo&() const
    {
      return m_submitInfo;
    }

  private:
    VkSubmitInfo m_submitInfo;
  };
  static_assert( sizeof( SubmitInfo ) == sizeof( VkSubmitInfo ), "struct and wrapper have different size!" );

  class Queue
  {
  public:
    Queue()
      : m_queue(VK_NULL_HANDLE)
    {}

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    Queue(VkQueue queue)
       : m_queue(queue)
    {}

    Queue& operator=(VkQueue queue)
    {
      m_queue = queue;
      return *this;
    }
#endif

    Result submit( uint32_t submitCount, const SubmitInfo* pSubmits, Fence fence ) const
    {
      return static_cast<Result>( vkQueueSubmit( m_queue, submitCount, reinterpret_cast<const VkSubmitInfo*>( pSubmits ), static_cast<VkFence>( fence ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void submit( std::vector<SubmitInfo> const& submits, Fence fence ) const
    {
      Result result = static_cast<Result>( vkQueueSubmit( m_queue, static_cast<uint32_t>( submits.size() ), reinterpret_cast<const VkSubmitInfo*>( submits.data() ), static_cast<VkFence>( fence ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Queue::submit" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    Result waitIdle(  ) const
    {
      return static_cast<Result>( vkQueueWaitIdle( m_queue ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void waitIdle(  ) const
    {
      Result result = static_cast<Result>( vkQueueWaitIdle( m_queue ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Queue::waitIdle" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result bindSparse( uint32_t bindInfoCount, const BindSparseInfo* pBindInfo, Fence fence ) const
    {
      return static_cast<Result>( vkQueueBindSparse( m_queue, bindInfoCount, reinterpret_cast<const VkBindSparseInfo*>( pBindInfo ), static_cast<VkFence>( fence ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void bindSparse( std::vector<BindSparseInfo> const& bindInfo, Fence fence ) const
    {
      Result result = static_cast<Result>( vkQueueBindSparse( m_queue, static_cast<uint32_t>( bindInfo.size() ), reinterpret_cast<const VkBindSparseInfo*>( bindInfo.data() ), static_cast<VkFence>( fence ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Queue::bindSparse" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result presentKHR( const PresentInfoKHR* pPresentInfo ) const
    {
      return static_cast<Result>( vkQueuePresentKHR( m_queue, reinterpret_cast<const VkPresentInfoKHR*>( pPresentInfo ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Result presentKHR( const PresentInfoKHR& presentInfo ) const
    {
      Result result = static_cast<Result>( vkQueuePresentKHR( m_queue, reinterpret_cast<const VkPresentInfoKHR*>( &presentInfo ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eSuboptimalKHR ) )
      {
        throw Exception( result, "vk::Queue::presentKHR" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

  enum class PresentModeKHR
  {
    eImmediateKHR = VK_PRESENT_MODE_IMMEDIATE_KHR,
    eMailboxKHR = VK_PRESENT_MODE_MAILBOX_KHR,
    eFifoKHR = VK_PRESENT_MODE_FIFO_KHR,
    eFifoRelaxedKHR = VK_PRESENT_MODE_FIFO_RELAXED_KHR
  };

  enum class ColorSpaceKHR
  {
    eVkColorspaceSrgbNonlinearKHR = VK_COLORSPACE_SRGB_NONLINEAR_KHR
  };

  class SurfaceFormatKHR
  {
  public:
    SurfaceFormatKHR()
      : SurfaceFormatKHR( Format::eUndefined, ColorSpaceKHR::eVkColorspaceSrgbNonlinearKHR )
    {}

    SurfaceFormatKHR( Format format, ColorSpaceKHR colorSpace)
    {
      m_surfaceFormatKHR.format = static_cast<VkFormat>( format );
      m_surfaceFormatKHR.colorSpace = static_cast<VkColorSpaceKHR>( colorSpace );
    }

    SurfaceFormatKHR(VkSurfaceFormatKHR const & rhs)
      : m_surfaceFormatKHR(rhs)
    {
    }

    SurfaceFormatKHR& operator=(VkSurfaceFormatKHR const & rhs)
    {
      m_surfaceFormatKHR = rhs;
      return *this;
    }

    const Format& format() const
    {
      return reinterpret_cast<const Format&>( m_surfaceFormatKHR.format );
    }

    Format& format()
    {
      return reinterpret_cast<Format&>( m_surfaceFormatKHR.format );
    }

    SurfaceFormatKHR& format( Format format )
    {
      m_surfaceFormatKHR.format = static_cast<VkFormat>( format );
      return *this;
    }

    const ColorSpaceKHR& colorSpace() const
    {
      return reinterpret_cast<const ColorSpaceKHR&>( m_surfaceFormatKHR.colorSpace );
    }

    ColorSpaceKHR& colorSpace()
    {
      return reinterpret_cast<ColorSpaceKHR&>( m_surfaceFormatKHR.colorSpace );
    }

    SurfaceFormatKHR& colorSpace( ColorSpaceKHR colorSpace )
    {
      m_surfaceFormatKHR.colorSpace = static_cast<VkColorSpaceKHR>( colorSpace );
      return *this;
    }

    static SurfaceFormatKHR& null()
    {
      return *((SurfaceFormatKHR*)(nullptr));
    }

    operator const VkSurfaceFormatKHR&() const
    {
      return m_surfaceFormatKHR;
    }

  private:
    VkSurfaceFormatKHR m_surfaceFormatKHR;
  };
  static_assert( sizeof( SurfaceFormatKHR ) == sizeof( VkSurfaceFormatKHR ), "struct and wrapper have different size!" );

  enum class DisplayPlaneAlphaFlagBitsKHR
  {
    eOpaque = VK_DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR,
    eGlobal = VK_DISPLAY_PLANE_ALPHA_GLOBAL_BIT_KHR,
    ePerPixel = VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_BIT_KHR,
    ePerPixelPremultiplied = VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_PREMULTIPLIED_BIT_KHR
  };

  typedef Flags<DisplayPlaneAlphaFlagBitsKHR, VkDisplayPlaneAlphaFlagsKHR> DisplayPlaneAlphaFlagsKHR;

  inline DisplayPlaneAlphaFlagsKHR operator|( DisplayPlaneAlphaFlagBitsKHR bit0, DisplayPlaneAlphaFlagBitsKHR bit1 )
  {
    return DisplayPlaneAlphaFlagsKHR( bit0 ) | bit1;
  }

  class DisplayPlaneCapabilitiesKHR
  {
  public:
    DisplayPlaneCapabilitiesKHR()
      : DisplayPlaneCapabilitiesKHR( DisplayPlaneAlphaFlagsKHR(), Offset2D(), Offset2D(), Extent2D(), Extent2D(), Offset2D(), Offset2D(), Extent2D(), Extent2D() )
    {}

    DisplayPlaneCapabilitiesKHR( DisplayPlaneAlphaFlagsKHR supportedAlpha, Offset2D minSrcPosition, Offset2D maxSrcPosition, Extent2D minSrcExtent, Extent2D maxSrcExtent, Offset2D minDstPosition, Offset2D maxDstPosition, Extent2D minDstExtent, Extent2D maxDstExtent)
    {
      m_displayPlaneCapabilitiesKHR.supportedAlpha = static_cast<VkDisplayPlaneAlphaFlagsKHR>( supportedAlpha );
      m_displayPlaneCapabilitiesKHR.minSrcPosition = static_cast<VkOffset2D>( minSrcPosition );
      m_displayPlaneCapabilitiesKHR.maxSrcPosition = static_cast<VkOffset2D>( maxSrcPosition );
      m_displayPlaneCapabilitiesKHR.minSrcExtent = static_cast<VkExtent2D>( minSrcExtent );
      m_displayPlaneCapabilitiesKHR.maxSrcExtent = static_cast<VkExtent2D>( maxSrcExtent );
      m_displayPlaneCapabilitiesKHR.minDstPosition = static_cast<VkOffset2D>( minDstPosition );
      m_displayPlaneCapabilitiesKHR.maxDstPosition = static_cast<VkOffset2D>( maxDstPosition );
      m_displayPlaneCapabilitiesKHR.minDstExtent = static_cast<VkExtent2D>( minDstExtent );
      m_displayPlaneCapabilitiesKHR.maxDstExtent = static_cast<VkExtent2D>( maxDstExtent );
    }

    DisplayPlaneCapabilitiesKHR(VkDisplayPlaneCapabilitiesKHR const & rhs)
      : m_displayPlaneCapabilitiesKHR(rhs)
    {
    }

    DisplayPlaneCapabilitiesKHR& operator=(VkDisplayPlaneCapabilitiesKHR const & rhs)
    {
      m_displayPlaneCapabilitiesKHR = rhs;
      return *this;
    }

    const DisplayPlaneAlphaFlagsKHR& supportedAlpha() const
    {
      return reinterpret_cast<const DisplayPlaneAlphaFlagsKHR&>( m_displayPlaneCapabilitiesKHR.supportedAlpha );
    }

    DisplayPlaneAlphaFlagsKHR& supportedAlpha()
    {
      return reinterpret_cast<DisplayPlaneAlphaFlagsKHR&>( m_displayPlaneCapabilitiesKHR.supportedAlpha );
    }

    DisplayPlaneCapabilitiesKHR& supportedAlpha( DisplayPlaneAlphaFlagsKHR supportedAlpha )
    {
      m_displayPlaneCapabilitiesKHR.supportedAlpha = static_cast<VkDisplayPlaneAlphaFlagsKHR>( supportedAlpha );
      return *this;
    }

    const Offset2D& minSrcPosition() const
    {
      return reinterpret_cast<const Offset2D&>( m_displayPlaneCapabilitiesKHR.minSrcPosition );
    }

    Offset2D& minSrcPosition()
    {
      return reinterpret_cast<Offset2D&>( m_displayPlaneCapabilitiesKHR.minSrcPosition );
    }

    DisplayPlaneCapabilitiesKHR& minSrcPosition( Offset2D minSrcPosition )
    {
      m_displayPlaneCapabilitiesKHR.minSrcPosition = static_cast<VkOffset2D>( minSrcPosition );
      return *this;
    }

    const Offset2D& maxSrcPosition() const
    {
      return reinterpret_cast<const Offset2D&>( m_displayPlaneCapabilitiesKHR.maxSrcPosition );
    }

    Offset2D& maxSrcPosition()
    {
      return reinterpret_cast<Offset2D&>( m_displayPlaneCapabilitiesKHR.maxSrcPosition );
    }

    DisplayPlaneCapabilitiesKHR& maxSrcPosition( Offset2D maxSrcPosition )
    {
      m_displayPlaneCapabilitiesKHR.maxSrcPosition = static_cast<VkOffset2D>( maxSrcPosition );
      return *this;
    }

    const Extent2D& minSrcExtent() const
    {
      return reinterpret_cast<const Extent2D&>( m_displayPlaneCapabilitiesKHR.minSrcExtent );
    }

    Extent2D& minSrcExtent()
    {
      return reinterpret_cast<Extent2D&>( m_displayPlaneCapabilitiesKHR.minSrcExtent );
    }

    DisplayPlaneCapabilitiesKHR& minSrcExtent( Extent2D minSrcExtent )
    {
      m_displayPlaneCapabilitiesKHR.minSrcExtent = static_cast<VkExtent2D>( minSrcExtent );
      return *this;
    }

    const Extent2D& maxSrcExtent() const
    {
      return reinterpret_cast<const Extent2D&>( m_displayPlaneCapabilitiesKHR.maxSrcExtent );
    }

    Extent2D& maxSrcExtent()
    {
      return reinterpret_cast<Extent2D&>( m_displayPlaneCapabilitiesKHR.maxSrcExtent );
    }

    DisplayPlaneCapabilitiesKHR& maxSrcExtent( Extent2D maxSrcExtent )
    {
      m_displayPlaneCapabilitiesKHR.maxSrcExtent = static_cast<VkExtent2D>( maxSrcExtent );
      return *this;
    }

    const Offset2D& minDstPosition() const
    {
      return reinterpret_cast<const Offset2D&>( m_displayPlaneCapabilitiesKHR.minDstPosition );
    }

    Offset2D& minDstPosition()
    {
      return reinterpret_cast<Offset2D&>( m_displayPlaneCapabilitiesKHR.minDstPosition );
    }

    DisplayPlaneCapabilitiesKHR& minDstPosition( Offset2D minDstPosition )
    {
      m_displayPlaneCapabilitiesKHR.minDstPosition = static_cast<VkOffset2D>( minDstPosition );
      return *this;
    }

    const Offset2D& maxDstPosition() const
    {
      return reinterpret_cast<const Offset2D&>( m_displayPlaneCapabilitiesKHR.maxDstPosition );
    }

    Offset2D& maxDstPosition()
    {
      return reinterpret_cast<Offset2D&>( m_displayPlaneCapabilitiesKHR.maxDstPosition );
    }

    DisplayPlaneCapabilitiesKHR& maxDstPosition( Offset2D maxDstPosition )
    {
      m_displayPlaneCapabilitiesKHR.maxDstPosition = static_cast<VkOffset2D>( maxDstPosition );
      return *this;
    }

    const Extent2D& minDstExtent() const
    {
      return reinterpret_cast<const Extent2D&>( m_displayPlaneCapabilitiesKHR.minDstExtent );
    }

    Extent2D& minDstExtent()
    {
      return reinterpret_cast<Extent2D&>( m_displayPlaneCapabilitiesKHR.minDstExtent );
    }

    DisplayPlaneCapabilitiesKHR& minDstExtent( Extent2D minDstExtent )
    {
      m_displayPlaneCapabilitiesKHR.minDstExtent = static_cast<VkExtent2D>( minDstExtent );
      return *this;
    }

    const Extent2D& maxDstExtent() const
    {
      return reinterpret_cast<const Extent2D&>( m_displayPlaneCapabilitiesKHR.maxDstExtent );
    }

    Extent2D& maxDstExtent()
    {
      return reinterpret_cast<Extent2D&>( m_displayPlaneCapabilitiesKHR.maxDstExtent );
    }

    DisplayPlaneCapabilitiesKHR& maxDstExtent( Extent2D maxDstExtent )
    {
      m_displayPlaneCapabilitiesKHR.maxDstExtent = static_cast<VkExtent2D>( maxDstExtent );
      return *this;
    }

    static DisplayPlaneCapabilitiesKHR& null()
    {
      return *((DisplayPlaneCapabilitiesKHR*)(nullptr));
    }

    operator const VkDisplayPlaneCapabilitiesKHR&() const
    {
      return m_displayPlaneCapabilitiesKHR;
    }

  private:
    VkDisplayPlaneCapabilitiesKHR m_displayPlaneCapabilitiesKHR;
  };
  static_assert( sizeof( DisplayPlaneCapabilitiesKHR ) == sizeof( VkDisplayPlaneCapabilitiesKHR ), "struct and wrapper have different size!" );

  enum class CompositeAlphaFlagBitsKHR
  {
    eOpaque = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    ePreMultiplied = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
    ePostMultiplied = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
    eInherit = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
  };

  typedef Flags<CompositeAlphaFlagBitsKHR, VkCompositeAlphaFlagsKHR> CompositeAlphaFlagsKHR;

  inline CompositeAlphaFlagsKHR operator|( CompositeAlphaFlagBitsKHR bit0, CompositeAlphaFlagBitsKHR bit1 )
  {
    return CompositeAlphaFlagsKHR( bit0 ) | bit1;
  }

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

  typedef Flags<SurfaceTransformFlagBitsKHR, VkSurfaceTransformFlagsKHR> SurfaceTransformFlagsKHR;

  inline SurfaceTransformFlagsKHR operator|( SurfaceTransformFlagBitsKHR bit0, SurfaceTransformFlagBitsKHR bit1 )
  {
    return SurfaceTransformFlagsKHR( bit0 ) | bit1;
  }

  class DisplayPropertiesKHR
  {
  public:
    DisplayPropertiesKHR()
      : DisplayPropertiesKHR( DisplayKHR(), nullptr, Extent2D(), Extent2D(), SurfaceTransformFlagsKHR(), 0, 0 )
    {}

    DisplayPropertiesKHR( DisplayKHR display, const char* displayName, Extent2D physicalDimensions, Extent2D physicalResolution, SurfaceTransformFlagsKHR supportedTransforms, Bool32 planeReorderPossible, Bool32 persistentContent)
    {
      m_displayPropertiesKHR.display = static_cast<VkDisplayKHR>( display );
      m_displayPropertiesKHR.displayName = displayName;
      m_displayPropertiesKHR.physicalDimensions = static_cast<VkExtent2D>( physicalDimensions );
      m_displayPropertiesKHR.physicalResolution = static_cast<VkExtent2D>( physicalResolution );
      m_displayPropertiesKHR.supportedTransforms = static_cast<VkSurfaceTransformFlagsKHR>( supportedTransforms );
      m_displayPropertiesKHR.planeReorderPossible = planeReorderPossible;
      m_displayPropertiesKHR.persistentContent = persistentContent;
    }

    DisplayPropertiesKHR(VkDisplayPropertiesKHR const & rhs)
      : m_displayPropertiesKHR(rhs)
    {
    }

    DisplayPropertiesKHR& operator=(VkDisplayPropertiesKHR const & rhs)
    {
      m_displayPropertiesKHR = rhs;
      return *this;
    }

    const DisplayKHR& display() const
    {
      return reinterpret_cast<const DisplayKHR&>( m_displayPropertiesKHR.display );
    }

    DisplayKHR& display()
    {
      return reinterpret_cast<DisplayKHR&>( m_displayPropertiesKHR.display );
    }

    DisplayPropertiesKHR& display( DisplayKHR display )
    {
      m_displayPropertiesKHR.display = static_cast<VkDisplayKHR>( display );
      return *this;
    }

    const char* displayName() const
    {
      return reinterpret_cast<const char*>( m_displayPropertiesKHR.displayName );
    }

    const char* displayName()
    {
      return reinterpret_cast<const char*>( m_displayPropertiesKHR.displayName );
    }

    DisplayPropertiesKHR& displayName( const char* displayName )
    {
      m_displayPropertiesKHR.displayName = displayName;
      return *this;
    }

    const Extent2D& physicalDimensions() const
    {
      return reinterpret_cast<const Extent2D&>( m_displayPropertiesKHR.physicalDimensions );
    }

    Extent2D& physicalDimensions()
    {
      return reinterpret_cast<Extent2D&>( m_displayPropertiesKHR.physicalDimensions );
    }

    DisplayPropertiesKHR& physicalDimensions( Extent2D physicalDimensions )
    {
      m_displayPropertiesKHR.physicalDimensions = static_cast<VkExtent2D>( physicalDimensions );
      return *this;
    }

    const Extent2D& physicalResolution() const
    {
      return reinterpret_cast<const Extent2D&>( m_displayPropertiesKHR.physicalResolution );
    }

    Extent2D& physicalResolution()
    {
      return reinterpret_cast<Extent2D&>( m_displayPropertiesKHR.physicalResolution );
    }

    DisplayPropertiesKHR& physicalResolution( Extent2D physicalResolution )
    {
      m_displayPropertiesKHR.physicalResolution = static_cast<VkExtent2D>( physicalResolution );
      return *this;
    }

    const SurfaceTransformFlagsKHR& supportedTransforms() const
    {
      return reinterpret_cast<const SurfaceTransformFlagsKHR&>( m_displayPropertiesKHR.supportedTransforms );
    }

    SurfaceTransformFlagsKHR& supportedTransforms()
    {
      return reinterpret_cast<SurfaceTransformFlagsKHR&>( m_displayPropertiesKHR.supportedTransforms );
    }

    DisplayPropertiesKHR& supportedTransforms( SurfaceTransformFlagsKHR supportedTransforms )
    {
      m_displayPropertiesKHR.supportedTransforms = static_cast<VkSurfaceTransformFlagsKHR>( supportedTransforms );
      return *this;
    }

    const Bool32& planeReorderPossible() const
    {
      return m_displayPropertiesKHR.planeReorderPossible;
    }

    Bool32& planeReorderPossible()
    {
      return m_displayPropertiesKHR.planeReorderPossible;
    }

    DisplayPropertiesKHR& planeReorderPossible( Bool32 planeReorderPossible )
    {
      m_displayPropertiesKHR.planeReorderPossible = planeReorderPossible;
      return *this;
    }

    const Bool32& persistentContent() const
    {
      return m_displayPropertiesKHR.persistentContent;
    }

    Bool32& persistentContent()
    {
      return m_displayPropertiesKHR.persistentContent;
    }

    DisplayPropertiesKHR& persistentContent( Bool32 persistentContent )
    {
      m_displayPropertiesKHR.persistentContent = persistentContent;
      return *this;
    }

    static DisplayPropertiesKHR& null()
    {
      return *((DisplayPropertiesKHR*)(nullptr));
    }

    operator const VkDisplayPropertiesKHR&() const
    {
      return m_displayPropertiesKHR;
    }

  private:
    VkDisplayPropertiesKHR m_displayPropertiesKHR;
  };
  static_assert( sizeof( DisplayPropertiesKHR ) == sizeof( VkDisplayPropertiesKHR ), "struct and wrapper have different size!" );

  class DisplaySurfaceCreateInfoKHR
  {
  public:
    DisplaySurfaceCreateInfoKHR()
      : DisplaySurfaceCreateInfoKHR( DisplaySurfaceCreateFlagsKHR(), DisplayModeKHR(), 0, 0, SurfaceTransformFlagBitsKHR::eIdentity, 0, DisplayPlaneAlphaFlagBitsKHR::eOpaque, Extent2D() )
    {}

    DisplaySurfaceCreateInfoKHR( DisplaySurfaceCreateFlagsKHR flags, DisplayModeKHR displayMode, uint32_t planeIndex, uint32_t planeStackIndex, SurfaceTransformFlagBitsKHR transform, float globalAlpha, DisplayPlaneAlphaFlagBitsKHR alphaMode, Extent2D imageExtent)
    {
      m_displaySurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR;
      m_displaySurfaceCreateInfoKHR.pNext = nullptr;
      m_displaySurfaceCreateInfoKHR.flags = static_cast<VkDisplaySurfaceCreateFlagsKHR>( flags );
      m_displaySurfaceCreateInfoKHR.displayMode = static_cast<VkDisplayModeKHR>( displayMode );
      m_displaySurfaceCreateInfoKHR.planeIndex = planeIndex;
      m_displaySurfaceCreateInfoKHR.planeStackIndex = planeStackIndex;
      m_displaySurfaceCreateInfoKHR.transform = static_cast<VkSurfaceTransformFlagBitsKHR>( transform );
      m_displaySurfaceCreateInfoKHR.globalAlpha = globalAlpha;
      m_displaySurfaceCreateInfoKHR.alphaMode = static_cast<VkDisplayPlaneAlphaFlagBitsKHR>( alphaMode );
      m_displaySurfaceCreateInfoKHR.imageExtent = static_cast<VkExtent2D>( imageExtent );
    }

    DisplaySurfaceCreateInfoKHR(VkDisplaySurfaceCreateInfoKHR const & rhs)
      : m_displaySurfaceCreateInfoKHR(rhs)
    {
    }

    DisplaySurfaceCreateInfoKHR& operator=(VkDisplaySurfaceCreateInfoKHR const & rhs)
    {
      m_displaySurfaceCreateInfoKHR = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_displaySurfaceCreateInfoKHR.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_displaySurfaceCreateInfoKHR.sType );
    }

    DisplaySurfaceCreateInfoKHR& sType( StructureType sType )
    {
      m_displaySurfaceCreateInfoKHR.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_displaySurfaceCreateInfoKHR.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_displaySurfaceCreateInfoKHR.pNext );
    }

    DisplaySurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_displaySurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const DisplaySurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const DisplaySurfaceCreateFlagsKHR&>( m_displaySurfaceCreateInfoKHR.flags );
    }

    DisplaySurfaceCreateFlagsKHR& flags()
    {
      return reinterpret_cast<DisplaySurfaceCreateFlagsKHR&>( m_displaySurfaceCreateInfoKHR.flags );
    }

    DisplaySurfaceCreateInfoKHR& flags( DisplaySurfaceCreateFlagsKHR flags )
    {
      m_displaySurfaceCreateInfoKHR.flags = static_cast<VkDisplaySurfaceCreateFlagsKHR>( flags );
      return *this;
    }

    const DisplayModeKHR& displayMode() const
    {
      return reinterpret_cast<const DisplayModeKHR&>( m_displaySurfaceCreateInfoKHR.displayMode );
    }

    DisplayModeKHR& displayMode()
    {
      return reinterpret_cast<DisplayModeKHR&>( m_displaySurfaceCreateInfoKHR.displayMode );
    }

    DisplaySurfaceCreateInfoKHR& displayMode( DisplayModeKHR displayMode )
    {
      m_displaySurfaceCreateInfoKHR.displayMode = static_cast<VkDisplayModeKHR>( displayMode );
      return *this;
    }

    const uint32_t& planeIndex() const
    {
      return m_displaySurfaceCreateInfoKHR.planeIndex;
    }

    uint32_t& planeIndex()
    {
      return m_displaySurfaceCreateInfoKHR.planeIndex;
    }

    DisplaySurfaceCreateInfoKHR& planeIndex( uint32_t planeIndex )
    {
      m_displaySurfaceCreateInfoKHR.planeIndex = planeIndex;
      return *this;
    }

    const uint32_t& planeStackIndex() const
    {
      return m_displaySurfaceCreateInfoKHR.planeStackIndex;
    }

    uint32_t& planeStackIndex()
    {
      return m_displaySurfaceCreateInfoKHR.planeStackIndex;
    }

    DisplaySurfaceCreateInfoKHR& planeStackIndex( uint32_t planeStackIndex )
    {
      m_displaySurfaceCreateInfoKHR.planeStackIndex = planeStackIndex;
      return *this;
    }

    const SurfaceTransformFlagBitsKHR& transform() const
    {
      return reinterpret_cast<const SurfaceTransformFlagBitsKHR&>( m_displaySurfaceCreateInfoKHR.transform );
    }

    SurfaceTransformFlagBitsKHR& transform()
    {
      return reinterpret_cast<SurfaceTransformFlagBitsKHR&>( m_displaySurfaceCreateInfoKHR.transform );
    }

    DisplaySurfaceCreateInfoKHR& transform( SurfaceTransformFlagBitsKHR transform )
    {
      m_displaySurfaceCreateInfoKHR.transform = static_cast<VkSurfaceTransformFlagBitsKHR>( transform );
      return *this;
    }

    const float& globalAlpha() const
    {
      return m_displaySurfaceCreateInfoKHR.globalAlpha;
    }

    float& globalAlpha()
    {
      return m_displaySurfaceCreateInfoKHR.globalAlpha;
    }

    DisplaySurfaceCreateInfoKHR& globalAlpha( float globalAlpha )
    {
      m_displaySurfaceCreateInfoKHR.globalAlpha = globalAlpha;
      return *this;
    }

    const DisplayPlaneAlphaFlagBitsKHR& alphaMode() const
    {
      return reinterpret_cast<const DisplayPlaneAlphaFlagBitsKHR&>( m_displaySurfaceCreateInfoKHR.alphaMode );
    }

    DisplayPlaneAlphaFlagBitsKHR& alphaMode()
    {
      return reinterpret_cast<DisplayPlaneAlphaFlagBitsKHR&>( m_displaySurfaceCreateInfoKHR.alphaMode );
    }

    DisplaySurfaceCreateInfoKHR& alphaMode( DisplayPlaneAlphaFlagBitsKHR alphaMode )
    {
      m_displaySurfaceCreateInfoKHR.alphaMode = static_cast<VkDisplayPlaneAlphaFlagBitsKHR>( alphaMode );
      return *this;
    }

    const Extent2D& imageExtent() const
    {
      return reinterpret_cast<const Extent2D&>( m_displaySurfaceCreateInfoKHR.imageExtent );
    }

    Extent2D& imageExtent()
    {
      return reinterpret_cast<Extent2D&>( m_displaySurfaceCreateInfoKHR.imageExtent );
    }

    DisplaySurfaceCreateInfoKHR& imageExtent( Extent2D imageExtent )
    {
      m_displaySurfaceCreateInfoKHR.imageExtent = static_cast<VkExtent2D>( imageExtent );
      return *this;
    }

    static DisplaySurfaceCreateInfoKHR& null()
    {
      return *((DisplaySurfaceCreateInfoKHR*)(nullptr));
    }

    operator const VkDisplaySurfaceCreateInfoKHR&() const
    {
      return m_displaySurfaceCreateInfoKHR;
    }

  private:
    VkDisplaySurfaceCreateInfoKHR m_displaySurfaceCreateInfoKHR;
  };
  static_assert( sizeof( DisplaySurfaceCreateInfoKHR ) == sizeof( VkDisplaySurfaceCreateInfoKHR ), "struct and wrapper have different size!" );

  class SurfaceCapabilitiesKHR
  {
  public:
    SurfaceCapabilitiesKHR()
      : SurfaceCapabilitiesKHR( 0, 0, Extent2D(), Extent2D(), Extent2D(), 0, SurfaceTransformFlagsKHR(), SurfaceTransformFlagBitsKHR::eIdentity, CompositeAlphaFlagsKHR(), ImageUsageFlags() )
    {}

    SurfaceCapabilitiesKHR( uint32_t minImageCount, uint32_t maxImageCount, Extent2D currentExtent, Extent2D minImageExtent, Extent2D maxImageExtent, uint32_t maxImageArrayLayers, SurfaceTransformFlagsKHR supportedTransforms, SurfaceTransformFlagBitsKHR currentTransform, CompositeAlphaFlagsKHR supportedCompositeAlpha, ImageUsageFlags supportedUsageFlags)
    {
      m_surfaceCapabilitiesKHR.minImageCount = minImageCount;
      m_surfaceCapabilitiesKHR.maxImageCount = maxImageCount;
      m_surfaceCapabilitiesKHR.currentExtent = static_cast<VkExtent2D>( currentExtent );
      m_surfaceCapabilitiesKHR.minImageExtent = static_cast<VkExtent2D>( minImageExtent );
      m_surfaceCapabilitiesKHR.maxImageExtent = static_cast<VkExtent2D>( maxImageExtent );
      m_surfaceCapabilitiesKHR.maxImageArrayLayers = maxImageArrayLayers;
      m_surfaceCapabilitiesKHR.supportedTransforms = static_cast<VkSurfaceTransformFlagsKHR>( supportedTransforms );
      m_surfaceCapabilitiesKHR.currentTransform = static_cast<VkSurfaceTransformFlagBitsKHR>( currentTransform );
      m_surfaceCapabilitiesKHR.supportedCompositeAlpha = static_cast<VkCompositeAlphaFlagsKHR>( supportedCompositeAlpha );
      m_surfaceCapabilitiesKHR.supportedUsageFlags = static_cast<VkImageUsageFlags>( supportedUsageFlags );
    }

    SurfaceCapabilitiesKHR(VkSurfaceCapabilitiesKHR const & rhs)
      : m_surfaceCapabilitiesKHR(rhs)
    {
    }

    SurfaceCapabilitiesKHR& operator=(VkSurfaceCapabilitiesKHR const & rhs)
    {
      m_surfaceCapabilitiesKHR = rhs;
      return *this;
    }

    const uint32_t& minImageCount() const
    {
      return m_surfaceCapabilitiesKHR.minImageCount;
    }

    uint32_t& minImageCount()
    {
      return m_surfaceCapabilitiesKHR.minImageCount;
    }

    SurfaceCapabilitiesKHR& minImageCount( uint32_t minImageCount )
    {
      m_surfaceCapabilitiesKHR.minImageCount = minImageCount;
      return *this;
    }

    const uint32_t& maxImageCount() const
    {
      return m_surfaceCapabilitiesKHR.maxImageCount;
    }

    uint32_t& maxImageCount()
    {
      return m_surfaceCapabilitiesKHR.maxImageCount;
    }

    SurfaceCapabilitiesKHR& maxImageCount( uint32_t maxImageCount )
    {
      m_surfaceCapabilitiesKHR.maxImageCount = maxImageCount;
      return *this;
    }

    const Extent2D& currentExtent() const
    {
      return reinterpret_cast<const Extent2D&>( m_surfaceCapabilitiesKHR.currentExtent );
    }

    Extent2D& currentExtent()
    {
      return reinterpret_cast<Extent2D&>( m_surfaceCapabilitiesKHR.currentExtent );
    }

    SurfaceCapabilitiesKHR& currentExtent( Extent2D currentExtent )
    {
      m_surfaceCapabilitiesKHR.currentExtent = static_cast<VkExtent2D>( currentExtent );
      return *this;
    }

    const Extent2D& minImageExtent() const
    {
      return reinterpret_cast<const Extent2D&>( m_surfaceCapabilitiesKHR.minImageExtent );
    }

    Extent2D& minImageExtent()
    {
      return reinterpret_cast<Extent2D&>( m_surfaceCapabilitiesKHR.minImageExtent );
    }

    SurfaceCapabilitiesKHR& minImageExtent( Extent2D minImageExtent )
    {
      m_surfaceCapabilitiesKHR.minImageExtent = static_cast<VkExtent2D>( minImageExtent );
      return *this;
    }

    const Extent2D& maxImageExtent() const
    {
      return reinterpret_cast<const Extent2D&>( m_surfaceCapabilitiesKHR.maxImageExtent );
    }

    Extent2D& maxImageExtent()
    {
      return reinterpret_cast<Extent2D&>( m_surfaceCapabilitiesKHR.maxImageExtent );
    }

    SurfaceCapabilitiesKHR& maxImageExtent( Extent2D maxImageExtent )
    {
      m_surfaceCapabilitiesKHR.maxImageExtent = static_cast<VkExtent2D>( maxImageExtent );
      return *this;
    }

    const uint32_t& maxImageArrayLayers() const
    {
      return m_surfaceCapabilitiesKHR.maxImageArrayLayers;
    }

    uint32_t& maxImageArrayLayers()
    {
      return m_surfaceCapabilitiesKHR.maxImageArrayLayers;
    }

    SurfaceCapabilitiesKHR& maxImageArrayLayers( uint32_t maxImageArrayLayers )
    {
      m_surfaceCapabilitiesKHR.maxImageArrayLayers = maxImageArrayLayers;
      return *this;
    }

    const SurfaceTransformFlagsKHR& supportedTransforms() const
    {
      return reinterpret_cast<const SurfaceTransformFlagsKHR&>( m_surfaceCapabilitiesKHR.supportedTransforms );
    }

    SurfaceTransformFlagsKHR& supportedTransforms()
    {
      return reinterpret_cast<SurfaceTransformFlagsKHR&>( m_surfaceCapabilitiesKHR.supportedTransforms );
    }

    SurfaceCapabilitiesKHR& supportedTransforms( SurfaceTransformFlagsKHR supportedTransforms )
    {
      m_surfaceCapabilitiesKHR.supportedTransforms = static_cast<VkSurfaceTransformFlagsKHR>( supportedTransforms );
      return *this;
    }

    const SurfaceTransformFlagBitsKHR& currentTransform() const
    {
      return reinterpret_cast<const SurfaceTransformFlagBitsKHR&>( m_surfaceCapabilitiesKHR.currentTransform );
    }

    SurfaceTransformFlagBitsKHR& currentTransform()
    {
      return reinterpret_cast<SurfaceTransformFlagBitsKHR&>( m_surfaceCapabilitiesKHR.currentTransform );
    }

    SurfaceCapabilitiesKHR& currentTransform( SurfaceTransformFlagBitsKHR currentTransform )
    {
      m_surfaceCapabilitiesKHR.currentTransform = static_cast<VkSurfaceTransformFlagBitsKHR>( currentTransform );
      return *this;
    }

    const CompositeAlphaFlagsKHR& supportedCompositeAlpha() const
    {
      return reinterpret_cast<const CompositeAlphaFlagsKHR&>( m_surfaceCapabilitiesKHR.supportedCompositeAlpha );
    }

    CompositeAlphaFlagsKHR& supportedCompositeAlpha()
    {
      return reinterpret_cast<CompositeAlphaFlagsKHR&>( m_surfaceCapabilitiesKHR.supportedCompositeAlpha );
    }

    SurfaceCapabilitiesKHR& supportedCompositeAlpha( CompositeAlphaFlagsKHR supportedCompositeAlpha )
    {
      m_surfaceCapabilitiesKHR.supportedCompositeAlpha = static_cast<VkCompositeAlphaFlagsKHR>( supportedCompositeAlpha );
      return *this;
    }

    const ImageUsageFlags& supportedUsageFlags() const
    {
      return reinterpret_cast<const ImageUsageFlags&>( m_surfaceCapabilitiesKHR.supportedUsageFlags );
    }

    ImageUsageFlags& supportedUsageFlags()
    {
      return reinterpret_cast<ImageUsageFlags&>( m_surfaceCapabilitiesKHR.supportedUsageFlags );
    }

    SurfaceCapabilitiesKHR& supportedUsageFlags( ImageUsageFlags supportedUsageFlags )
    {
      m_surfaceCapabilitiesKHR.supportedUsageFlags = static_cast<VkImageUsageFlags>( supportedUsageFlags );
      return *this;
    }

    static SurfaceCapabilitiesKHR& null()
    {
      return *((SurfaceCapabilitiesKHR*)(nullptr));
    }

    operator const VkSurfaceCapabilitiesKHR&() const
    {
      return m_surfaceCapabilitiesKHR;
    }

  private:
    VkSurfaceCapabilitiesKHR m_surfaceCapabilitiesKHR;
  };
  static_assert( sizeof( SurfaceCapabilitiesKHR ) == sizeof( VkSurfaceCapabilitiesKHR ), "struct and wrapper have different size!" );

  class SwapchainCreateInfoKHR
  {
  public:
    SwapchainCreateInfoKHR()
      : SwapchainCreateInfoKHR( SwapchainCreateFlagsKHR(), SurfaceKHR(), 0, Format::eUndefined, ColorSpaceKHR::eVkColorspaceSrgbNonlinearKHR, Extent2D(), 0, ImageUsageFlags(), SharingMode::eExclusive, 0, nullptr, SurfaceTransformFlagBitsKHR::eIdentity, CompositeAlphaFlagBitsKHR::eOpaque, PresentModeKHR::eImmediateKHR, 0, SwapchainKHR() )
    {}

    SwapchainCreateInfoKHR( SwapchainCreateFlagsKHR flags, SurfaceKHR surface, uint32_t minImageCount, Format imageFormat, ColorSpaceKHR imageColorSpace, Extent2D imageExtent, uint32_t imageArrayLayers, ImageUsageFlags imageUsage, SharingMode imageSharingMode, uint32_t queueFamilyIndexCount, const uint32_t* pQueueFamilyIndices, SurfaceTransformFlagBitsKHR preTransform, CompositeAlphaFlagBitsKHR compositeAlpha, PresentModeKHR presentMode, Bool32 clipped, SwapchainKHR oldSwapchain)
    {
      m_swapchainCreateInfoKHR.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      m_swapchainCreateInfoKHR.pNext = nullptr;
      m_swapchainCreateInfoKHR.flags = static_cast<VkSwapchainCreateFlagsKHR>( flags );
      m_swapchainCreateInfoKHR.surface = static_cast<VkSurfaceKHR>( surface );
      m_swapchainCreateInfoKHR.minImageCount = minImageCount;
      m_swapchainCreateInfoKHR.imageFormat = static_cast<VkFormat>( imageFormat );
      m_swapchainCreateInfoKHR.imageColorSpace = static_cast<VkColorSpaceKHR>( imageColorSpace );
      m_swapchainCreateInfoKHR.imageExtent = static_cast<VkExtent2D>( imageExtent );
      m_swapchainCreateInfoKHR.imageArrayLayers = imageArrayLayers;
      m_swapchainCreateInfoKHR.imageUsage = static_cast<VkImageUsageFlags>( imageUsage );
      m_swapchainCreateInfoKHR.imageSharingMode = static_cast<VkSharingMode>( imageSharingMode );
      m_swapchainCreateInfoKHR.queueFamilyIndexCount = queueFamilyIndexCount;
      m_swapchainCreateInfoKHR.pQueueFamilyIndices = pQueueFamilyIndices;
      m_swapchainCreateInfoKHR.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>( preTransform );
      m_swapchainCreateInfoKHR.compositeAlpha = static_cast<VkCompositeAlphaFlagBitsKHR>( compositeAlpha );
      m_swapchainCreateInfoKHR.presentMode = static_cast<VkPresentModeKHR>( presentMode );
      m_swapchainCreateInfoKHR.clipped = clipped;
      m_swapchainCreateInfoKHR.oldSwapchain = static_cast<VkSwapchainKHR>( oldSwapchain );
    }

    SwapchainCreateInfoKHR(VkSwapchainCreateInfoKHR const & rhs)
      : m_swapchainCreateInfoKHR(rhs)
    {
    }

    SwapchainCreateInfoKHR& operator=(VkSwapchainCreateInfoKHR const & rhs)
    {
      m_swapchainCreateInfoKHR = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_swapchainCreateInfoKHR.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_swapchainCreateInfoKHR.sType );
    }

    SwapchainCreateInfoKHR& sType( StructureType sType )
    {
      m_swapchainCreateInfoKHR.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_swapchainCreateInfoKHR.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_swapchainCreateInfoKHR.pNext );
    }

    SwapchainCreateInfoKHR& pNext( const void* pNext )
    {
      m_swapchainCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const SwapchainCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const SwapchainCreateFlagsKHR&>( m_swapchainCreateInfoKHR.flags );
    }

    SwapchainCreateFlagsKHR& flags()
    {
      return reinterpret_cast<SwapchainCreateFlagsKHR&>( m_swapchainCreateInfoKHR.flags );
    }

    SwapchainCreateInfoKHR& flags( SwapchainCreateFlagsKHR flags )
    {
      m_swapchainCreateInfoKHR.flags = static_cast<VkSwapchainCreateFlagsKHR>( flags );
      return *this;
    }

    const SurfaceKHR& surface() const
    {
      return reinterpret_cast<const SurfaceKHR&>( m_swapchainCreateInfoKHR.surface );
    }

    SurfaceKHR& surface()
    {
      return reinterpret_cast<SurfaceKHR&>( m_swapchainCreateInfoKHR.surface );
    }

    SwapchainCreateInfoKHR& surface( SurfaceKHR surface )
    {
      m_swapchainCreateInfoKHR.surface = static_cast<VkSurfaceKHR>( surface );
      return *this;
    }

    const uint32_t& minImageCount() const
    {
      return m_swapchainCreateInfoKHR.minImageCount;
    }

    uint32_t& minImageCount()
    {
      return m_swapchainCreateInfoKHR.minImageCount;
    }

    SwapchainCreateInfoKHR& minImageCount( uint32_t minImageCount )
    {
      m_swapchainCreateInfoKHR.minImageCount = minImageCount;
      return *this;
    }

    const Format& imageFormat() const
    {
      return reinterpret_cast<const Format&>( m_swapchainCreateInfoKHR.imageFormat );
    }

    Format& imageFormat()
    {
      return reinterpret_cast<Format&>( m_swapchainCreateInfoKHR.imageFormat );
    }

    SwapchainCreateInfoKHR& imageFormat( Format imageFormat )
    {
      m_swapchainCreateInfoKHR.imageFormat = static_cast<VkFormat>( imageFormat );
      return *this;
    }

    const ColorSpaceKHR& imageColorSpace() const
    {
      return reinterpret_cast<const ColorSpaceKHR&>( m_swapchainCreateInfoKHR.imageColorSpace );
    }

    ColorSpaceKHR& imageColorSpace()
    {
      return reinterpret_cast<ColorSpaceKHR&>( m_swapchainCreateInfoKHR.imageColorSpace );
    }

    SwapchainCreateInfoKHR& imageColorSpace( ColorSpaceKHR imageColorSpace )
    {
      m_swapchainCreateInfoKHR.imageColorSpace = static_cast<VkColorSpaceKHR>( imageColorSpace );
      return *this;
    }

    const Extent2D& imageExtent() const
    {
      return reinterpret_cast<const Extent2D&>( m_swapchainCreateInfoKHR.imageExtent );
    }

    Extent2D& imageExtent()
    {
      return reinterpret_cast<Extent2D&>( m_swapchainCreateInfoKHR.imageExtent );
    }

    SwapchainCreateInfoKHR& imageExtent( Extent2D imageExtent )
    {
      m_swapchainCreateInfoKHR.imageExtent = static_cast<VkExtent2D>( imageExtent );
      return *this;
    }

    const uint32_t& imageArrayLayers() const
    {
      return m_swapchainCreateInfoKHR.imageArrayLayers;
    }

    uint32_t& imageArrayLayers()
    {
      return m_swapchainCreateInfoKHR.imageArrayLayers;
    }

    SwapchainCreateInfoKHR& imageArrayLayers( uint32_t imageArrayLayers )
    {
      m_swapchainCreateInfoKHR.imageArrayLayers = imageArrayLayers;
      return *this;
    }

    const ImageUsageFlags& imageUsage() const
    {
      return reinterpret_cast<const ImageUsageFlags&>( m_swapchainCreateInfoKHR.imageUsage );
    }

    ImageUsageFlags& imageUsage()
    {
      return reinterpret_cast<ImageUsageFlags&>( m_swapchainCreateInfoKHR.imageUsage );
    }

    SwapchainCreateInfoKHR& imageUsage( ImageUsageFlags imageUsage )
    {
      m_swapchainCreateInfoKHR.imageUsage = static_cast<VkImageUsageFlags>( imageUsage );
      return *this;
    }

    const SharingMode& imageSharingMode() const
    {
      return reinterpret_cast<const SharingMode&>( m_swapchainCreateInfoKHR.imageSharingMode );
    }

    SharingMode& imageSharingMode()
    {
      return reinterpret_cast<SharingMode&>( m_swapchainCreateInfoKHR.imageSharingMode );
    }

    SwapchainCreateInfoKHR& imageSharingMode( SharingMode imageSharingMode )
    {
      m_swapchainCreateInfoKHR.imageSharingMode = static_cast<VkSharingMode>( imageSharingMode );
      return *this;
    }

    const uint32_t& queueFamilyIndexCount() const
    {
      return m_swapchainCreateInfoKHR.queueFamilyIndexCount;
    }

    uint32_t& queueFamilyIndexCount()
    {
      return m_swapchainCreateInfoKHR.queueFamilyIndexCount;
    }

    SwapchainCreateInfoKHR& queueFamilyIndexCount( uint32_t queueFamilyIndexCount )
    {
      m_swapchainCreateInfoKHR.queueFamilyIndexCount = queueFamilyIndexCount;
      return *this;
    }

    const uint32_t* pQueueFamilyIndices() const
    {
      return reinterpret_cast<const uint32_t*>( m_swapchainCreateInfoKHR.pQueueFamilyIndices );
    }

    const uint32_t* pQueueFamilyIndices()
    {
      return reinterpret_cast<const uint32_t*>( m_swapchainCreateInfoKHR.pQueueFamilyIndices );
    }

    SwapchainCreateInfoKHR& pQueueFamilyIndices( const uint32_t* pQueueFamilyIndices )
    {
      m_swapchainCreateInfoKHR.pQueueFamilyIndices = pQueueFamilyIndices;
      return *this;
    }

    const SurfaceTransformFlagBitsKHR& preTransform() const
    {
      return reinterpret_cast<const SurfaceTransformFlagBitsKHR&>( m_swapchainCreateInfoKHR.preTransform );
    }

    SurfaceTransformFlagBitsKHR& preTransform()
    {
      return reinterpret_cast<SurfaceTransformFlagBitsKHR&>( m_swapchainCreateInfoKHR.preTransform );
    }

    SwapchainCreateInfoKHR& preTransform( SurfaceTransformFlagBitsKHR preTransform )
    {
      m_swapchainCreateInfoKHR.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>( preTransform );
      return *this;
    }

    const CompositeAlphaFlagBitsKHR& compositeAlpha() const
    {
      return reinterpret_cast<const CompositeAlphaFlagBitsKHR&>( m_swapchainCreateInfoKHR.compositeAlpha );
    }

    CompositeAlphaFlagBitsKHR& compositeAlpha()
    {
      return reinterpret_cast<CompositeAlphaFlagBitsKHR&>( m_swapchainCreateInfoKHR.compositeAlpha );
    }

    SwapchainCreateInfoKHR& compositeAlpha( CompositeAlphaFlagBitsKHR compositeAlpha )
    {
      m_swapchainCreateInfoKHR.compositeAlpha = static_cast<VkCompositeAlphaFlagBitsKHR>( compositeAlpha );
      return *this;
    }

    const PresentModeKHR& presentMode() const
    {
      return reinterpret_cast<const PresentModeKHR&>( m_swapchainCreateInfoKHR.presentMode );
    }

    PresentModeKHR& presentMode()
    {
      return reinterpret_cast<PresentModeKHR&>( m_swapchainCreateInfoKHR.presentMode );
    }

    SwapchainCreateInfoKHR& presentMode( PresentModeKHR presentMode )
    {
      m_swapchainCreateInfoKHR.presentMode = static_cast<VkPresentModeKHR>( presentMode );
      return *this;
    }

    const Bool32& clipped() const
    {
      return m_swapchainCreateInfoKHR.clipped;
    }

    Bool32& clipped()
    {
      return m_swapchainCreateInfoKHR.clipped;
    }

    SwapchainCreateInfoKHR& clipped( Bool32 clipped )
    {
      m_swapchainCreateInfoKHR.clipped = clipped;
      return *this;
    }

    const SwapchainKHR& oldSwapchain() const
    {
      return reinterpret_cast<const SwapchainKHR&>( m_swapchainCreateInfoKHR.oldSwapchain );
    }

    SwapchainKHR& oldSwapchain()
    {
      return reinterpret_cast<SwapchainKHR&>( m_swapchainCreateInfoKHR.oldSwapchain );
    }

    SwapchainCreateInfoKHR& oldSwapchain( SwapchainKHR oldSwapchain )
    {
      m_swapchainCreateInfoKHR.oldSwapchain = static_cast<VkSwapchainKHR>( oldSwapchain );
      return *this;
    }

    static SwapchainCreateInfoKHR& null()
    {
      return *((SwapchainCreateInfoKHR*)(nullptr));
    }

    operator const VkSwapchainCreateInfoKHR&() const
    {
      return m_swapchainCreateInfoKHR;
    }

  private:
    VkSwapchainCreateInfoKHR m_swapchainCreateInfoKHR;
  };
  static_assert( sizeof( SwapchainCreateInfoKHR ) == sizeof( VkSwapchainCreateInfoKHR ), "struct and wrapper have different size!" );

  class Device
  {
  public:
    Device()
      : m_device(VK_NULL_HANDLE)
    {}

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    Device(VkDevice device)
       : m_device(device)
    {}

    Device& operator=(VkDevice device)
    {
      m_device = device;
      return *this;
    }
#endif

    PFN_vkVoidFunction getProcAddr( const char* pName ) const
    {
      return vkGetDeviceProcAddr( m_device, pName );
    }

#ifdef VKCPP_ENHANCED_MODE
    PFN_vkVoidFunction getProcAddr( std::string const& name ) const
    {
      return vkGetDeviceProcAddr( m_device, name.c_str() );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroy( const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyDevice( m_device, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroy( const AllocationCallbacks& allocator ) const
    {
      vkDestroyDevice( m_device, reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void getQueue( uint32_t queueFamilyIndex, uint32_t queueIndex, Queue* pQueue ) const
    {
      vkGetDeviceQueue( m_device, queueFamilyIndex, queueIndex, reinterpret_cast<VkQueue*>( pQueue ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Queue getQueue( uint32_t queueFamilyIndex, uint32_t queueIndex ) const
    {
      Queue queue;
      vkGetDeviceQueue( m_device, queueFamilyIndex, queueIndex, reinterpret_cast<VkQueue*>( &queue ) );
      return queue;
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    Result waitIdle(  ) const
    {
      return static_cast<Result>( vkDeviceWaitIdle( m_device ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void waitIdle(  ) const
    {
      Result result = static_cast<Result>( vkDeviceWaitIdle( m_device ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::waitIdle" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result allocateMemory( const MemoryAllocateInfo* pAllocateInfo, const AllocationCallbacks* pAllocator, DeviceMemory* pMemory ) const
    {
      return static_cast<Result>( vkAllocateMemory( m_device, reinterpret_cast<const VkMemoryAllocateInfo*>( pAllocateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkDeviceMemory*>( pMemory ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    DeviceMemory allocateMemory( const MemoryAllocateInfo& allocateInfo, const AllocationCallbacks& allocator ) const
    {
      DeviceMemory memory;
      Result result = static_cast<Result>( vkAllocateMemory( m_device, reinterpret_cast<const VkMemoryAllocateInfo*>( &allocateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkDeviceMemory*>( &memory ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::allocateMemory" );
      }
      return memory;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void freeMemory( DeviceMemory memory, const AllocationCallbacks* pAllocator ) const
    {
      vkFreeMemory( m_device, static_cast<VkDeviceMemory>( memory ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void freeMemory( DeviceMemory memory, const AllocationCallbacks& allocator ) const
    {
      vkFreeMemory( m_device, static_cast<VkDeviceMemory>( memory ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    Result mapMemory( DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags, void** ppData ) const
    {
      return static_cast<Result>( vkMapMemory( m_device, static_cast<VkDeviceMemory>( memory ), offset, size, static_cast<VkMemoryMapFlags>( flags ), ppData ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void* mapMemory( DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags ) const
    {
      void* pData;
      Result result = static_cast<Result>( vkMapMemory( m_device, static_cast<VkDeviceMemory>( memory ), offset, size, static_cast<VkMemoryMapFlags>( flags ), &pData ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::mapMemory" );
      }
      return pData;
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    void unmapMemory( DeviceMemory memory ) const
    {
      vkUnmapMemory( m_device, static_cast<VkDeviceMemory>( memory ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void unmapMemory( DeviceMemory memory ) const
    {
      vkUnmapMemory( m_device, static_cast<VkDeviceMemory>( memory ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result flushMappedMemoryRanges( uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges ) const
    {
      return static_cast<Result>( vkFlushMappedMemoryRanges( m_device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>( pMemoryRanges ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void flushMappedMemoryRanges( std::vector<MappedMemoryRange> const& memoryRanges ) const
    {
      Result result = static_cast<Result>( vkFlushMappedMemoryRanges( m_device, static_cast<uint32_t>( memoryRanges.size() ), reinterpret_cast<const VkMappedMemoryRange*>( memoryRanges.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::flushMappedMemoryRanges" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result invalidateMappedMemoryRanges( uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges ) const
    {
      return static_cast<Result>( vkInvalidateMappedMemoryRanges( m_device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>( pMemoryRanges ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void invalidateMappedMemoryRanges( std::vector<MappedMemoryRange> const& memoryRanges ) const
    {
      Result result = static_cast<Result>( vkInvalidateMappedMemoryRanges( m_device, static_cast<uint32_t>( memoryRanges.size() ), reinterpret_cast<const VkMappedMemoryRange*>( memoryRanges.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::invalidateMappedMemoryRanges" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void getMemoryCommitment( DeviceMemory memory, DeviceSize* pCommittedMemoryInBytes ) const
    {
      vkGetDeviceMemoryCommitment( m_device, static_cast<VkDeviceMemory>( memory ), pCommittedMemoryInBytes );
    }

#ifdef VKCPP_ENHANCED_MODE
    DeviceSize getMemoryCommitment( DeviceMemory memory ) const
    {
      DeviceSize committedMemoryInBytes;
      vkGetDeviceMemoryCommitment( m_device, static_cast<VkDeviceMemory>( memory ), &committedMemoryInBytes );
      return committedMemoryInBytes;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void getBufferMemoryRequirements( Buffer buffer, MemoryRequirements* pMemoryRequirements ) const
    {
      vkGetBufferMemoryRequirements( m_device, static_cast<VkBuffer>( buffer ), reinterpret_cast<VkMemoryRequirements*>( pMemoryRequirements ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    MemoryRequirements getBufferMemoryRequirements( Buffer buffer ) const
    {
      MemoryRequirements memoryRequirements;
      vkGetBufferMemoryRequirements( m_device, static_cast<VkBuffer>( buffer ), reinterpret_cast<VkMemoryRequirements*>( &memoryRequirements ) );
      return memoryRequirements;
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    Result bindBufferMemory( Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset ) const
    {
      return static_cast<Result>( vkBindBufferMemory( m_device, static_cast<VkBuffer>( buffer ), static_cast<VkDeviceMemory>( memory ), memoryOffset ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void bindBufferMemory( Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset ) const
    {
      Result result = static_cast<Result>( vkBindBufferMemory( m_device, static_cast<VkBuffer>( buffer ), static_cast<VkDeviceMemory>( memory ), memoryOffset ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::bindBufferMemory" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void getImageMemoryRequirements( Image image, MemoryRequirements* pMemoryRequirements ) const
    {
      vkGetImageMemoryRequirements( m_device, static_cast<VkImage>( image ), reinterpret_cast<VkMemoryRequirements*>( pMemoryRequirements ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    MemoryRequirements getImageMemoryRequirements( Image image ) const
    {
      MemoryRequirements memoryRequirements;
      vkGetImageMemoryRequirements( m_device, static_cast<VkImage>( image ), reinterpret_cast<VkMemoryRequirements*>( &memoryRequirements ) );
      return memoryRequirements;
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    Result bindImageMemory( Image image, DeviceMemory memory, DeviceSize memoryOffset ) const
    {
      return static_cast<Result>( vkBindImageMemory( m_device, static_cast<VkImage>( image ), static_cast<VkDeviceMemory>( memory ), memoryOffset ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void bindImageMemory( Image image, DeviceMemory memory, DeviceSize memoryOffset ) const
    {
      Result result = static_cast<Result>( vkBindImageMemory( m_device, static_cast<VkImage>( image ), static_cast<VkDeviceMemory>( memory ), memoryOffset ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::bindImageMemory" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void getImageSparseMemoryRequirements( Image image, uint32_t* pSparseMemoryRequirementCount, SparseImageMemoryRequirements* pSparseMemoryRequirements ) const
    {
      vkGetImageSparseMemoryRequirements( m_device, static_cast<VkImage>( image ), pSparseMemoryRequirementCount, reinterpret_cast<VkSparseImageMemoryRequirements*>( pSparseMemoryRequirements ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    std::vector<SparseImageMemoryRequirements> getImageSparseMemoryRequirements( Image image ) const
    {
      std::vector<SparseImageMemoryRequirements> sparseMemoryRequirements;
      uint32_t sparseMemoryRequirementCount;
      vkGetImageSparseMemoryRequirements( m_device, static_cast<VkImage>( image ), &sparseMemoryRequirementCount, reinterpret_cast<VkSparseImageMemoryRequirements*>( sparseMemoryRequirements.data() ) );
      sparseMemoryRequirements.resize( sparseMemoryRequirementCount );
      vkGetImageSparseMemoryRequirements( m_device, static_cast<VkImage>( image ), &sparseMemoryRequirementCount, reinterpret_cast<VkSparseImageMemoryRequirements*>( sparseMemoryRequirements.data() ) );
      return std::move( sparseMemoryRequirements );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createFence( const FenceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Fence* pFence ) const
    {
      return static_cast<Result>( vkCreateFence( m_device, reinterpret_cast<const VkFenceCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkFence*>( pFence ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Fence createFence( const FenceCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      Fence fence;
      Result result = static_cast<Result>( vkCreateFence( m_device, reinterpret_cast<const VkFenceCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkFence*>( &fence ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createFence" );
      }
      return fence;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyFence( Fence fence, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyFence( m_device, static_cast<VkFence>( fence ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyFence( Fence fence, const AllocationCallbacks& allocator ) const
    {
      vkDestroyFence( m_device, static_cast<VkFence>( fence ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result resetFences( uint32_t fenceCount, const Fence* pFences ) const
    {
      return static_cast<Result>( vkResetFences( m_device, fenceCount, reinterpret_cast<const VkFence*>( pFences ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void resetFences( std::vector<Fence> const& fences ) const
    {
      Result result = static_cast<Result>( vkResetFences( m_device, static_cast<uint32_t>( fences.size() ), reinterpret_cast<const VkFence*>( fences.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::resetFences" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    Result getFenceStatus( Fence fence ) const
    {
      return static_cast<Result>( vkGetFenceStatus( m_device, static_cast<VkFence>( fence ) ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    Result getFenceStatus( Fence fence ) const
    {
      Result result = static_cast<Result>( vkGetFenceStatus( m_device, static_cast<VkFence>( fence ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eNotReady ) )
      {
        throw Exception( result, "vk::Device::getFenceStatus" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result waitForFences( uint32_t fenceCount, const Fence* pFences, Bool32 waitAll, uint64_t timeout ) const
    {
      return static_cast<Result>( vkWaitForFences( m_device, fenceCount, reinterpret_cast<const VkFence*>( pFences ), waitAll, timeout ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Result waitForFences( std::vector<Fence> const& fences, Bool32 waitAll, uint64_t timeout ) const
    {
      Result result = static_cast<Result>( vkWaitForFences( m_device, static_cast<uint32_t>( fences.size() ), reinterpret_cast<const VkFence*>( fences.data() ), waitAll, timeout ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eTimeout ) )
      {
        throw Exception( result, "vk::Device::waitForFences" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createSemaphore( const SemaphoreCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Semaphore* pSemaphore ) const
    {
      return static_cast<Result>( vkCreateSemaphore( m_device, reinterpret_cast<const VkSemaphoreCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSemaphore*>( pSemaphore ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Semaphore createSemaphore( const SemaphoreCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      Semaphore semaphore;
      Result result = static_cast<Result>( vkCreateSemaphore( m_device, reinterpret_cast<const VkSemaphoreCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkSemaphore*>( &semaphore ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createSemaphore" );
      }
      return semaphore;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroySemaphore( Semaphore semaphore, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroySemaphore( m_device, static_cast<VkSemaphore>( semaphore ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroySemaphore( Semaphore semaphore, const AllocationCallbacks& allocator ) const
    {
      vkDestroySemaphore( m_device, static_cast<VkSemaphore>( semaphore ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createEvent( const EventCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Event* pEvent ) const
    {
      return static_cast<Result>( vkCreateEvent( m_device, reinterpret_cast<const VkEventCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkEvent*>( pEvent ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Event createEvent( const EventCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      Event event;
      Result result = static_cast<Result>( vkCreateEvent( m_device, reinterpret_cast<const VkEventCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkEvent*>( &event ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createEvent" );
      }
      return event;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyEvent( Event event, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyEvent( m_device, static_cast<VkEvent>( event ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyEvent( Event event, const AllocationCallbacks& allocator ) const
    {
      vkDestroyEvent( m_device, static_cast<VkEvent>( event ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    Result getEventStatus( Event event ) const
    {
      return static_cast<Result>( vkGetEventStatus( m_device, static_cast<VkEvent>( event ) ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    Result getEventStatus( Event event ) const
    {
      Result result = static_cast<Result>( vkGetEventStatus( m_device, static_cast<VkEvent>( event ) ) );
      if ( ( result != Result::eEventSet ) && ( result != Result::eEventReset ) )
      {
        throw Exception( result, "vk::Device::getEventStatus" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    Result setEvent( Event event ) const
    {
      return static_cast<Result>( vkSetEvent( m_device, static_cast<VkEvent>( event ) ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void setEvent( Event event ) const
    {
      Result result = static_cast<Result>( vkSetEvent( m_device, static_cast<VkEvent>( event ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::setEvent" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    Result resetEvent( Event event ) const
    {
      return static_cast<Result>( vkResetEvent( m_device, static_cast<VkEvent>( event ) ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void resetEvent( Event event ) const
    {
      Result result = static_cast<Result>( vkResetEvent( m_device, static_cast<VkEvent>( event ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::resetEvent" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createQueryPool( const QueryPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, QueryPool* pQueryPool ) const
    {
      return static_cast<Result>( vkCreateQueryPool( m_device, reinterpret_cast<const VkQueryPoolCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkQueryPool*>( pQueryPool ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    QueryPool createQueryPool( const QueryPoolCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      QueryPool queryPool;
      Result result = static_cast<Result>( vkCreateQueryPool( m_device, reinterpret_cast<const VkQueryPoolCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkQueryPool*>( &queryPool ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createQueryPool" );
      }
      return queryPool;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyQueryPool( QueryPool queryPool, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyQueryPool( m_device, static_cast<VkQueryPool>( queryPool ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyQueryPool( QueryPool queryPool, const AllocationCallbacks& allocator ) const
    {
      vkDestroyQueryPool( m_device, static_cast<VkQueryPool>( queryPool ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result getQueryPoolResults( QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, DeviceSize stride, QueryResultFlags flags ) const
    {
      return static_cast<Result>( vkGetQueryPoolResults( m_device, static_cast<VkQueryPool>( queryPool ), firstQuery, queryCount, dataSize, pData, stride, static_cast<VkQueryResultFlags>( flags ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    template <typename T>
    Result getQueryPoolResults( QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, std::vector<T> & data, DeviceSize stride, QueryResultFlags flags ) const
    {
      Result result = static_cast<Result>( vkGetQueryPoolResults( m_device, static_cast<VkQueryPool>( queryPool ), firstQuery, queryCount, static_cast<size_t>( data.size() * sizeof( T ) ), reinterpret_cast<void*>( data.data() ), stride, static_cast<VkQueryResultFlags>( flags ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eNotReady ) )
      {
        throw Exception( result, "vk::Device::getQueryPoolResults" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createBuffer( const BufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Buffer* pBuffer ) const
    {
      return static_cast<Result>( vkCreateBuffer( m_device, reinterpret_cast<const VkBufferCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkBuffer*>( pBuffer ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Buffer createBuffer( const BufferCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      Buffer buffer;
      Result result = static_cast<Result>( vkCreateBuffer( m_device, reinterpret_cast<const VkBufferCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkBuffer*>( &buffer ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createBuffer" );
      }
      return buffer;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyBuffer( Buffer buffer, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyBuffer( m_device, static_cast<VkBuffer>( buffer ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyBuffer( Buffer buffer, const AllocationCallbacks& allocator ) const
    {
      vkDestroyBuffer( m_device, static_cast<VkBuffer>( buffer ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createBufferView( const BufferViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, BufferView* pView ) const
    {
      return static_cast<Result>( vkCreateBufferView( m_device, reinterpret_cast<const VkBufferViewCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkBufferView*>( pView ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    BufferView createBufferView( const BufferViewCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      BufferView view;
      Result result = static_cast<Result>( vkCreateBufferView( m_device, reinterpret_cast<const VkBufferViewCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkBufferView*>( &view ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createBufferView" );
      }
      return view;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyBufferView( BufferView bufferView, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyBufferView( m_device, static_cast<VkBufferView>( bufferView ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyBufferView( BufferView bufferView, const AllocationCallbacks& allocator ) const
    {
      vkDestroyBufferView( m_device, static_cast<VkBufferView>( bufferView ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createImage( const ImageCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Image* pImage ) const
    {
      return static_cast<Result>( vkCreateImage( m_device, reinterpret_cast<const VkImageCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkImage*>( pImage ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Image createImage( const ImageCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      Image image;
      Result result = static_cast<Result>( vkCreateImage( m_device, reinterpret_cast<const VkImageCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkImage*>( &image ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createImage" );
      }
      return image;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyImage( Image image, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyImage( m_device, static_cast<VkImage>( image ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyImage( Image image, const AllocationCallbacks& allocator ) const
    {
      vkDestroyImage( m_device, static_cast<VkImage>( image ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void getImageSubresourceLayout( Image image, const ImageSubresource* pSubresource, SubresourceLayout* pLayout ) const
    {
      vkGetImageSubresourceLayout( m_device, static_cast<VkImage>( image ), reinterpret_cast<const VkImageSubresource*>( pSubresource ), reinterpret_cast<VkSubresourceLayout*>( pLayout ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    SubresourceLayout getImageSubresourceLayout( Image image, const ImageSubresource& subresource ) const
    {
      SubresourceLayout layout;
      vkGetImageSubresourceLayout( m_device, static_cast<VkImage>( image ), reinterpret_cast<const VkImageSubresource*>( &subresource ), reinterpret_cast<VkSubresourceLayout*>( &layout ) );
      return layout;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createImageView( const ImageViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ImageView* pView ) const
    {
      return static_cast<Result>( vkCreateImageView( m_device, reinterpret_cast<const VkImageViewCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkImageView*>( pView ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    ImageView createImageView( const ImageViewCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      ImageView view;
      Result result = static_cast<Result>( vkCreateImageView( m_device, reinterpret_cast<const VkImageViewCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkImageView*>( &view ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createImageView" );
      }
      return view;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyImageView( ImageView imageView, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyImageView( m_device, static_cast<VkImageView>( imageView ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyImageView( ImageView imageView, const AllocationCallbacks& allocator ) const
    {
      vkDestroyImageView( m_device, static_cast<VkImageView>( imageView ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createShaderModule( const ShaderModuleCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ShaderModule* pShaderModule ) const
    {
      return static_cast<Result>( vkCreateShaderModule( m_device, reinterpret_cast<const VkShaderModuleCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkShaderModule*>( pShaderModule ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    ShaderModule createShaderModule( const ShaderModuleCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      ShaderModule shaderModule;
      Result result = static_cast<Result>( vkCreateShaderModule( m_device, reinterpret_cast<const VkShaderModuleCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkShaderModule*>( &shaderModule ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createShaderModule" );
      }
      return shaderModule;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyShaderModule( ShaderModule shaderModule, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyShaderModule( m_device, static_cast<VkShaderModule>( shaderModule ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyShaderModule( ShaderModule shaderModule, const AllocationCallbacks& allocator ) const
    {
      vkDestroyShaderModule( m_device, static_cast<VkShaderModule>( shaderModule ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createPipelineCache( const PipelineCacheCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineCache* pPipelineCache ) const
    {
      return static_cast<Result>( vkCreatePipelineCache( m_device, reinterpret_cast<const VkPipelineCacheCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkPipelineCache*>( pPipelineCache ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    PipelineCache createPipelineCache( const PipelineCacheCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      PipelineCache pipelineCache;
      Result result = static_cast<Result>( vkCreatePipelineCache( m_device, reinterpret_cast<const VkPipelineCacheCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkPipelineCache*>( &pipelineCache ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createPipelineCache" );
      }
      return pipelineCache;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyPipelineCache( PipelineCache pipelineCache, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyPipelineCache( m_device, static_cast<VkPipelineCache>( pipelineCache ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyPipelineCache( PipelineCache pipelineCache, const AllocationCallbacks& allocator ) const
    {
      vkDestroyPipelineCache( m_device, static_cast<VkPipelineCache>( pipelineCache ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result getPipelineCacheData( PipelineCache pipelineCache, size_t* pDataSize, void* pData ) const
    {
      return static_cast<Result>( vkGetPipelineCacheData( m_device, static_cast<VkPipelineCache>( pipelineCache ), pDataSize, pData ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    std::vector<uint8_t> getPipelineCacheData( PipelineCache pipelineCache ) const
    {
      std::vector<uint8_t> data;
      size_t dataSize;
      Result result = static_cast<Result>( vkGetPipelineCacheData( m_device, static_cast<VkPipelineCache>( pipelineCache ), &dataSize, reinterpret_cast<void*>( data.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::getPipelineCacheData" );
      }
      data.resize( dataSize );
      result = static_cast<Result>( vkGetPipelineCacheData( m_device, static_cast<VkPipelineCache>( pipelineCache ), &dataSize, reinterpret_cast<void*>( data.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::getPipelineCacheData" );
      }
      return std::move( data );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result mergePipelineCaches( PipelineCache dstCache, uint32_t srcCacheCount, const PipelineCache* pSrcCaches ) const
    {
      return static_cast<Result>( vkMergePipelineCaches( m_device, static_cast<VkPipelineCache>( dstCache ), srcCacheCount, reinterpret_cast<const VkPipelineCache*>( pSrcCaches ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void mergePipelineCaches( PipelineCache dstCache, std::vector<PipelineCache> const& srcCaches ) const
    {
      Result result = static_cast<Result>( vkMergePipelineCaches( m_device, static_cast<VkPipelineCache>( dstCache ), static_cast<uint32_t>( srcCaches.size() ), reinterpret_cast<const VkPipelineCache*>( srcCaches.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::mergePipelineCaches" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createGraphicsPipelines( PipelineCache pipelineCache, uint32_t createInfoCount, const GraphicsPipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines ) const
    {
      return static_cast<Result>( vkCreateGraphicsPipelines( m_device, static_cast<VkPipelineCache>( pipelineCache ), createInfoCount, reinterpret_cast<const VkGraphicsPipelineCreateInfo*>( pCreateInfos ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkPipeline*>( pPipelines ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    std::vector<Pipeline> createGraphicsPipelines( PipelineCache pipelineCache, std::vector<GraphicsPipelineCreateInfo> const& createInfos, const AllocationCallbacks& allocator ) const
    {
      std::vector<Pipeline> pipelines( createInfos.size() );
      Result result = static_cast<Result>( vkCreateGraphicsPipelines( m_device, static_cast<VkPipelineCache>( pipelineCache ), static_cast<uint32_t>( createInfos.size() ), reinterpret_cast<const VkGraphicsPipelineCreateInfo*>( createInfos.data() ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkPipeline*>( pipelines.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createGraphicsPipelines" );
      }
      return std::move( pipelines );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createComputePipelines( PipelineCache pipelineCache, uint32_t createInfoCount, const ComputePipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines ) const
    {
      return static_cast<Result>( vkCreateComputePipelines( m_device, static_cast<VkPipelineCache>( pipelineCache ), createInfoCount, reinterpret_cast<const VkComputePipelineCreateInfo*>( pCreateInfos ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkPipeline*>( pPipelines ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    std::vector<Pipeline> createComputePipelines( PipelineCache pipelineCache, std::vector<ComputePipelineCreateInfo> const& createInfos, const AllocationCallbacks& allocator ) const
    {
      std::vector<Pipeline> pipelines( createInfos.size() );
      Result result = static_cast<Result>( vkCreateComputePipelines( m_device, static_cast<VkPipelineCache>( pipelineCache ), static_cast<uint32_t>( createInfos.size() ), reinterpret_cast<const VkComputePipelineCreateInfo*>( createInfos.data() ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkPipeline*>( pipelines.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createComputePipelines" );
      }
      return std::move( pipelines );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyPipeline( Pipeline pipeline, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyPipeline( m_device, static_cast<VkPipeline>( pipeline ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyPipeline( Pipeline pipeline, const AllocationCallbacks& allocator ) const
    {
      vkDestroyPipeline( m_device, static_cast<VkPipeline>( pipeline ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createPipelineLayout( const PipelineLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineLayout* pPipelineLayout ) const
    {
      return static_cast<Result>( vkCreatePipelineLayout( m_device, reinterpret_cast<const VkPipelineLayoutCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkPipelineLayout*>( pPipelineLayout ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    PipelineLayout createPipelineLayout( const PipelineLayoutCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      PipelineLayout pipelineLayout;
      Result result = static_cast<Result>( vkCreatePipelineLayout( m_device, reinterpret_cast<const VkPipelineLayoutCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkPipelineLayout*>( &pipelineLayout ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createPipelineLayout" );
      }
      return pipelineLayout;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyPipelineLayout( PipelineLayout pipelineLayout, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyPipelineLayout( m_device, static_cast<VkPipelineLayout>( pipelineLayout ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyPipelineLayout( PipelineLayout pipelineLayout, const AllocationCallbacks& allocator ) const
    {
      vkDestroyPipelineLayout( m_device, static_cast<VkPipelineLayout>( pipelineLayout ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createSampler( const SamplerCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Sampler* pSampler ) const
    {
      return static_cast<Result>( vkCreateSampler( m_device, reinterpret_cast<const VkSamplerCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSampler*>( pSampler ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Sampler createSampler( const SamplerCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      Sampler sampler;
      Result result = static_cast<Result>( vkCreateSampler( m_device, reinterpret_cast<const VkSamplerCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkSampler*>( &sampler ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createSampler" );
      }
      return sampler;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroySampler( Sampler sampler, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroySampler( m_device, static_cast<VkSampler>( sampler ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroySampler( Sampler sampler, const AllocationCallbacks& allocator ) const
    {
      vkDestroySampler( m_device, static_cast<VkSampler>( sampler ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createDescriptorSetLayout( const DescriptorSetLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorSetLayout* pSetLayout ) const
    {
      return static_cast<Result>( vkCreateDescriptorSetLayout( m_device, reinterpret_cast<const VkDescriptorSetLayoutCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkDescriptorSetLayout*>( pSetLayout ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    DescriptorSetLayout createDescriptorSetLayout( const DescriptorSetLayoutCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      DescriptorSetLayout setLayout;
      Result result = static_cast<Result>( vkCreateDescriptorSetLayout( m_device, reinterpret_cast<const VkDescriptorSetLayoutCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkDescriptorSetLayout*>( &setLayout ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createDescriptorSetLayout" );
      }
      return setLayout;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyDescriptorSetLayout( DescriptorSetLayout descriptorSetLayout, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyDescriptorSetLayout( m_device, static_cast<VkDescriptorSetLayout>( descriptorSetLayout ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyDescriptorSetLayout( DescriptorSetLayout descriptorSetLayout, const AllocationCallbacks& allocator ) const
    {
      vkDestroyDescriptorSetLayout( m_device, static_cast<VkDescriptorSetLayout>( descriptorSetLayout ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createDescriptorPool( const DescriptorPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorPool* pDescriptorPool ) const
    {
      return static_cast<Result>( vkCreateDescriptorPool( m_device, reinterpret_cast<const VkDescriptorPoolCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkDescriptorPool*>( pDescriptorPool ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    DescriptorPool createDescriptorPool( const DescriptorPoolCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      DescriptorPool descriptorPool;
      Result result = static_cast<Result>( vkCreateDescriptorPool( m_device, reinterpret_cast<const VkDescriptorPoolCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkDescriptorPool*>( &descriptorPool ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createDescriptorPool" );
      }
      return descriptorPool;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyDescriptorPool( DescriptorPool descriptorPool, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyDescriptorPool( m_device, static_cast<VkDescriptorPool>( descriptorPool ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyDescriptorPool( DescriptorPool descriptorPool, const AllocationCallbacks& allocator ) const
    {
      vkDestroyDescriptorPool( m_device, static_cast<VkDescriptorPool>( descriptorPool ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    Result resetDescriptorPool( DescriptorPool descriptorPool, DescriptorPoolResetFlags flags ) const
    {
      return static_cast<Result>( vkResetDescriptorPool( m_device, static_cast<VkDescriptorPool>( descriptorPool ), static_cast<VkDescriptorPoolResetFlags>( flags ) ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void resetDescriptorPool( DescriptorPool descriptorPool, DescriptorPoolResetFlags flags ) const
    {
      Result result = static_cast<Result>( vkResetDescriptorPool( m_device, static_cast<VkDescriptorPool>( descriptorPool ), static_cast<VkDescriptorPoolResetFlags>( flags ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::resetDescriptorPool" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result allocateDescriptorSets( const DescriptorSetAllocateInfo* pAllocateInfo, DescriptorSet* pDescriptorSets ) const
    {
      return static_cast<Result>( vkAllocateDescriptorSets( m_device, reinterpret_cast<const VkDescriptorSetAllocateInfo*>( pAllocateInfo ), reinterpret_cast<VkDescriptorSet*>( pDescriptorSets ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    std::vector<DescriptorSet> allocateDescriptorSets( const DescriptorSetAllocateInfo& allocateInfo ) const
    {
      std::vector<DescriptorSet> descriptorSets( allocateInfo.descriptorSetCount() );
      Result result = static_cast<Result>( vkAllocateDescriptorSets( m_device, reinterpret_cast<const VkDescriptorSetAllocateInfo*>( &allocateInfo ), reinterpret_cast<VkDescriptorSet*>( descriptorSets.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::allocateDescriptorSets" );
      }
      return std::move( descriptorSets );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result freeDescriptorSets( DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets ) const
    {
      return static_cast<Result>( vkFreeDescriptorSets( m_device, static_cast<VkDescriptorPool>( descriptorPool ), descriptorSetCount, reinterpret_cast<const VkDescriptorSet*>( pDescriptorSets ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void freeDescriptorSets( DescriptorPool descriptorPool, std::vector<DescriptorSet> const& descriptorSets ) const
    {
      Result result = static_cast<Result>( vkFreeDescriptorSets( m_device, static_cast<VkDescriptorPool>( descriptorPool ), static_cast<uint32_t>( descriptorSets.size() ), reinterpret_cast<const VkDescriptorSet*>( descriptorSets.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::freeDescriptorSets" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void updateDescriptorSets( uint32_t descriptorWriteCount, const WriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const CopyDescriptorSet* pDescriptorCopies ) const
    {
      vkUpdateDescriptorSets( m_device, descriptorWriteCount, reinterpret_cast<const VkWriteDescriptorSet*>( pDescriptorWrites ), descriptorCopyCount, reinterpret_cast<const VkCopyDescriptorSet*>( pDescriptorCopies ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void updateDescriptorSets( std::vector<WriteDescriptorSet> const& descriptorWrites, std::vector<CopyDescriptorSet> const& descriptorCopies ) const
    {
      vkUpdateDescriptorSets( m_device, static_cast<uint32_t>( descriptorWrites.size() ), reinterpret_cast<const VkWriteDescriptorSet*>( descriptorWrites.data() ), static_cast<uint32_t>( descriptorCopies.size() ), reinterpret_cast<const VkCopyDescriptorSet*>( descriptorCopies.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createFramebuffer( const FramebufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Framebuffer* pFramebuffer ) const
    {
      return static_cast<Result>( vkCreateFramebuffer( m_device, reinterpret_cast<const VkFramebufferCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkFramebuffer*>( pFramebuffer ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Framebuffer createFramebuffer( const FramebufferCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      Framebuffer framebuffer;
      Result result = static_cast<Result>( vkCreateFramebuffer( m_device, reinterpret_cast<const VkFramebufferCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkFramebuffer*>( &framebuffer ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createFramebuffer" );
      }
      return framebuffer;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyFramebuffer( Framebuffer framebuffer, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyFramebuffer( m_device, static_cast<VkFramebuffer>( framebuffer ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyFramebuffer( Framebuffer framebuffer, const AllocationCallbacks& allocator ) const
    {
      vkDestroyFramebuffer( m_device, static_cast<VkFramebuffer>( framebuffer ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createRenderPass( const RenderPassCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, RenderPass* pRenderPass ) const
    {
      return static_cast<Result>( vkCreateRenderPass( m_device, reinterpret_cast<const VkRenderPassCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkRenderPass*>( pRenderPass ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    RenderPass createRenderPass( const RenderPassCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      RenderPass renderPass;
      Result result = static_cast<Result>( vkCreateRenderPass( m_device, reinterpret_cast<const VkRenderPassCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkRenderPass*>( &renderPass ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createRenderPass" );
      }
      return renderPass;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyRenderPass( RenderPass renderPass, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyRenderPass( m_device, static_cast<VkRenderPass>( renderPass ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyRenderPass( RenderPass renderPass, const AllocationCallbacks& allocator ) const
    {
      vkDestroyRenderPass( m_device, static_cast<VkRenderPass>( renderPass ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void getRenderAreaGranularity( RenderPass renderPass, Extent2D* pGranularity ) const
    {
      vkGetRenderAreaGranularity( m_device, static_cast<VkRenderPass>( renderPass ), reinterpret_cast<VkExtent2D*>( pGranularity ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Extent2D getRenderAreaGranularity( RenderPass renderPass ) const
    {
      Extent2D granularity;
      vkGetRenderAreaGranularity( m_device, static_cast<VkRenderPass>( renderPass ), reinterpret_cast<VkExtent2D*>( &granularity ) );
      return granularity;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createCommandPool( const CommandPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, CommandPool* pCommandPool ) const
    {
      return static_cast<Result>( vkCreateCommandPool( m_device, reinterpret_cast<const VkCommandPoolCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkCommandPool*>( pCommandPool ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    CommandPool createCommandPool( const CommandPoolCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      CommandPool commandPool;
      Result result = static_cast<Result>( vkCreateCommandPool( m_device, reinterpret_cast<const VkCommandPoolCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkCommandPool*>( &commandPool ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createCommandPool" );
      }
      return commandPool;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyCommandPool( CommandPool commandPool, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyCommandPool( m_device, static_cast<VkCommandPool>( commandPool ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyCommandPool( CommandPool commandPool, const AllocationCallbacks& allocator ) const
    {
      vkDestroyCommandPool( m_device, static_cast<VkCommandPool>( commandPool ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
    Result resetCommandPool( CommandPool commandPool, CommandPoolResetFlags flags ) const
    {
      return static_cast<Result>( vkResetCommandPool( m_device, static_cast<VkCommandPool>( commandPool ), static_cast<VkCommandPoolResetFlags>( flags ) ) );
    }
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
    void resetCommandPool( CommandPool commandPool, CommandPoolResetFlags flags ) const
    {
      Result result = static_cast<Result>( vkResetCommandPool( m_device, static_cast<VkCommandPool>( commandPool ), static_cast<VkCommandPoolResetFlags>( flags ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::resetCommandPool" );
      }
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result allocateCommandBuffers( const CommandBufferAllocateInfo* pAllocateInfo, CommandBuffer* pCommandBuffers ) const
    {
      return static_cast<Result>( vkAllocateCommandBuffers( m_device, reinterpret_cast<const VkCommandBufferAllocateInfo*>( pAllocateInfo ), reinterpret_cast<VkCommandBuffer*>( pCommandBuffers ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    std::vector<CommandBuffer> allocateCommandBuffers( const CommandBufferAllocateInfo& allocateInfo ) const
    {
      std::vector<CommandBuffer> commandBuffers( allocateInfo.commandBufferCount() );
      Result result = static_cast<Result>( vkAllocateCommandBuffers( m_device, reinterpret_cast<const VkCommandBufferAllocateInfo*>( &allocateInfo ), reinterpret_cast<VkCommandBuffer*>( commandBuffers.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::allocateCommandBuffers" );
      }
      return std::move( commandBuffers );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void freeCommandBuffers( CommandPool commandPool, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers ) const
    {
      vkFreeCommandBuffers( m_device, static_cast<VkCommandPool>( commandPool ), commandBufferCount, reinterpret_cast<const VkCommandBuffer*>( pCommandBuffers ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void freeCommandBuffers( CommandPool commandPool, std::vector<CommandBuffer> const& commandBuffers ) const
    {
      vkFreeCommandBuffers( m_device, static_cast<VkCommandPool>( commandPool ), static_cast<uint32_t>( commandBuffers.size() ), reinterpret_cast<const VkCommandBuffer*>( commandBuffers.data() ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createSharedSwapchainsKHR( uint32_t swapchainCount, const SwapchainCreateInfoKHR* pCreateInfos, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchains ) const
    {
      return static_cast<Result>( vkCreateSharedSwapchainsKHR( m_device, swapchainCount, reinterpret_cast<const VkSwapchainCreateInfoKHR*>( pCreateInfos ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSwapchainKHR*>( pSwapchains ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    std::vector<SwapchainKHR> createSharedSwapchainsKHR( std::vector<SwapchainCreateInfoKHR> const& createInfos, const AllocationCallbacks& allocator ) const
    {
      std::vector<SwapchainKHR> swapchains( createInfos.size() );
      Result result = static_cast<Result>( vkCreateSharedSwapchainsKHR( m_device, static_cast<uint32_t>( createInfos.size() ), reinterpret_cast<const VkSwapchainCreateInfoKHR*>( createInfos.data() ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkSwapchainKHR*>( swapchains.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createSharedSwapchainsKHR" );
      }
      return std::move( swapchains );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createSwapchainKHR( const SwapchainCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchain ) const
    {
      return static_cast<Result>( vkCreateSwapchainKHR( m_device, reinterpret_cast<const VkSwapchainCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSwapchainKHR*>( pSwapchain ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    SwapchainKHR createSwapchainKHR( const SwapchainCreateInfoKHR& createInfo, const AllocationCallbacks& allocator ) const
    {
      SwapchainKHR swapchain;
      Result result = static_cast<Result>( vkCreateSwapchainKHR( m_device, reinterpret_cast<const VkSwapchainCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkSwapchainKHR*>( &swapchain ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Device::createSwapchainKHR" );
      }
      return swapchain;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroySwapchainKHR( SwapchainKHR swapchain, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroySwapchainKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroySwapchainKHR( SwapchainKHR swapchain, const AllocationCallbacks& allocator ) const
    {
      vkDestroySwapchainKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result getSwapchainImagesKHR( SwapchainKHR swapchain, uint32_t* pSwapchainImageCount, Image* pSwapchainImages ) const
    {
      return static_cast<Result>( vkGetSwapchainImagesKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), pSwapchainImageCount, reinterpret_cast<VkImage*>( pSwapchainImages ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Result getSwapchainImagesKHR( SwapchainKHR swapchain, std::vector<Image> & swapchainImages ) const
    {
      uint32_t swapchainImageCount;
      Result result = static_cast<Result>( vkGetSwapchainImagesKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), &swapchainImageCount, reinterpret_cast<VkImage*>( swapchainImages.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::Device::getSwapchainImagesKHR" );
      }
      swapchainImages.resize( swapchainImageCount );
      result = static_cast<Result>( vkGetSwapchainImagesKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), &swapchainImageCount, reinterpret_cast<VkImage*>( swapchainImages.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::Device::getSwapchainImagesKHR" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result acquireNextImageKHR( SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t* pImageIndex ) const
    {
      return static_cast<Result>( vkAcquireNextImageKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), timeout, static_cast<VkSemaphore>( semaphore ), static_cast<VkFence>( fence ), pImageIndex ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Result acquireNextImageKHR( SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t& imageIndex ) const
    {
      Result result = static_cast<Result>( vkAcquireNextImageKHR( m_device, static_cast<VkSwapchainKHR>( swapchain ), timeout, static_cast<VkSemaphore>( semaphore ), static_cast<VkFence>( fence ), &imageIndex ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eSuboptimalKHR ) )
      {
        throw Exception( result, "vk::Device::acquireNextImageKHR" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

  class PhysicalDevice
  {
  public:
    PhysicalDevice()
      : m_physicalDevice(VK_NULL_HANDLE)
    {}

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    PhysicalDevice(VkPhysicalDevice physicalDevice)
       : m_physicalDevice(physicalDevice)
    {}

    PhysicalDevice& operator=(VkPhysicalDevice physicalDevice)
    {
      m_physicalDevice = physicalDevice;
      return *this;
    }
#endif

    void getProperties( PhysicalDeviceProperties* pProperties ) const
    {
      vkGetPhysicalDeviceProperties( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties*>( pProperties ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    PhysicalDeviceProperties getProperties(  ) const
    {
      PhysicalDeviceProperties properties;
      vkGetPhysicalDeviceProperties( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties*>( &properties ) );
      return properties;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void getQueueFamilyProperties( uint32_t* pQueueFamilyPropertyCount, QueueFamilyProperties* pQueueFamilyProperties ) const
    {
      vkGetPhysicalDeviceQueueFamilyProperties( m_physicalDevice, pQueueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties*>( pQueueFamilyProperties ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    std::vector<QueueFamilyProperties> getQueueFamilyProperties(  ) const
    {
      std::vector<QueueFamilyProperties> queueFamilyProperties;
      uint32_t queueFamilyPropertyCount;
      vkGetPhysicalDeviceQueueFamilyProperties( m_physicalDevice, &queueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties*>( queueFamilyProperties.data() ) );
      queueFamilyProperties.resize( queueFamilyPropertyCount );
      vkGetPhysicalDeviceQueueFamilyProperties( m_physicalDevice, &queueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties*>( queueFamilyProperties.data() ) );
      return std::move( queueFamilyProperties );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void getMemoryProperties( PhysicalDeviceMemoryProperties* pMemoryProperties ) const
    {
      vkGetPhysicalDeviceMemoryProperties( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties*>( pMemoryProperties ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    PhysicalDeviceMemoryProperties getMemoryProperties(  ) const
    {
      PhysicalDeviceMemoryProperties memoryProperties;
      vkGetPhysicalDeviceMemoryProperties( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties*>( &memoryProperties ) );
      return memoryProperties;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void getFeatures( PhysicalDeviceFeatures* pFeatures ) const
    {
      vkGetPhysicalDeviceFeatures( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures*>( pFeatures ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    PhysicalDeviceFeatures getFeatures(  ) const
    {
      PhysicalDeviceFeatures features;
      vkGetPhysicalDeviceFeatures( m_physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures*>( &features ) );
      return features;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void getFormatProperties( Format format, FormatProperties* pFormatProperties ) const
    {
      vkGetPhysicalDeviceFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties*>( pFormatProperties ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    FormatProperties getFormatProperties( Format format ) const
    {
      FormatProperties formatProperties;
      vkGetPhysicalDeviceFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties*>( &formatProperties ) );
      return formatProperties;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result getImageFormatProperties( Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ImageFormatProperties* pImageFormatProperties ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceImageFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkImageTiling>( tiling ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageCreateFlags>( flags ), reinterpret_cast<VkImageFormatProperties*>( pImageFormatProperties ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    ImageFormatProperties getImageFormatProperties( Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags ) const
    {
      ImageFormatProperties imageFormatProperties;
      Result result = static_cast<Result>( vkGetPhysicalDeviceImageFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkImageTiling>( tiling ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageCreateFlags>( flags ), reinterpret_cast<VkImageFormatProperties*>( &imageFormatProperties ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::PhysicalDevice::getImageFormatProperties" );
      }
      return imageFormatProperties;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createDevice( const DeviceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Device* pDevice ) const
    {
      return static_cast<Result>( vkCreateDevice( m_physicalDevice, reinterpret_cast<const VkDeviceCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkDevice*>( pDevice ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Device createDevice( const DeviceCreateInfo& createInfo, const AllocationCallbacks& allocator ) const
    {
      Device device;
      Result result = static_cast<Result>( vkCreateDevice( m_physicalDevice, reinterpret_cast<const VkDeviceCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkDevice*>( &device ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::PhysicalDevice::createDevice" );
      }
      return device;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result enumerateDeviceLayerProperties( uint32_t* pPropertyCount, LayerProperties* pProperties ) const
    {
      return static_cast<Result>( vkEnumerateDeviceLayerProperties( m_physicalDevice, pPropertyCount, reinterpret_cast<VkLayerProperties*>( pProperties ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Result enumerateDeviceLayerProperties( std::vector<LayerProperties> & properties ) const
    {
      uint32_t propertyCount;
      Result result = static_cast<Result>( vkEnumerateDeviceLayerProperties( m_physicalDevice, &propertyCount, reinterpret_cast<VkLayerProperties*>( properties.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::enumerateDeviceLayerProperties" );
      }
      properties.resize( propertyCount );
      result = static_cast<Result>( vkEnumerateDeviceLayerProperties( m_physicalDevice, &propertyCount, reinterpret_cast<VkLayerProperties*>( properties.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::enumerateDeviceLayerProperties" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result enumerateDeviceExtensionProperties( const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties ) const
    {
      return static_cast<Result>( vkEnumerateDeviceExtensionProperties( m_physicalDevice, pLayerName, pPropertyCount, reinterpret_cast<VkExtensionProperties*>( pProperties ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Result enumerateDeviceExtensionProperties( std::string const& layerName, std::vector<ExtensionProperties> & properties ) const
    {
      uint32_t propertyCount;
      Result result = static_cast<Result>( vkEnumerateDeviceExtensionProperties( m_physicalDevice, layerName.data(), &propertyCount, reinterpret_cast<VkExtensionProperties*>( properties.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::enumerateDeviceExtensionProperties" );
      }
      properties.resize( propertyCount );
      result = static_cast<Result>( vkEnumerateDeviceExtensionProperties( m_physicalDevice, layerName.data(), &propertyCount, reinterpret_cast<VkExtensionProperties*>( properties.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::enumerateDeviceExtensionProperties" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void getSparseImageFormatProperties( Format format, ImageType type, SampleCountFlagBits samples, ImageUsageFlags usage, ImageTiling tiling, uint32_t* pPropertyCount, SparseImageFormatProperties* pProperties ) const
    {
      vkGetPhysicalDeviceSparseImageFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkSampleCountFlagBits>( samples ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageTiling>( tiling ), pPropertyCount, reinterpret_cast<VkSparseImageFormatProperties*>( pProperties ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    std::vector<SparseImageFormatProperties> getSparseImageFormatProperties( Format format, ImageType type, SampleCountFlagBits samples, ImageUsageFlags usage, ImageTiling tiling ) const
    {
      std::vector<SparseImageFormatProperties> properties;
      uint32_t propertyCount;
      vkGetPhysicalDeviceSparseImageFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkSampleCountFlagBits>( samples ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageTiling>( tiling ), &propertyCount, reinterpret_cast<VkSparseImageFormatProperties*>( properties.data() ) );
      properties.resize( propertyCount );
      vkGetPhysicalDeviceSparseImageFormatProperties( m_physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkSampleCountFlagBits>( samples ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageTiling>( tiling ), &propertyCount, reinterpret_cast<VkSparseImageFormatProperties*>( properties.data() ) );
      return std::move( properties );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result getDisplayPropertiesKHR( uint32_t* pPropertyCount, DisplayPropertiesKHR* pProperties ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceDisplayPropertiesKHR( m_physicalDevice, pPropertyCount, reinterpret_cast<VkDisplayPropertiesKHR*>( pProperties ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Result getDisplayPropertiesKHR( std::vector<DisplayPropertiesKHR> & properties ) const
    {
      uint32_t propertyCount;
      Result result = static_cast<Result>( vkGetPhysicalDeviceDisplayPropertiesKHR( m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayPropertiesKHR*>( properties.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::getDisplayPropertiesKHR" );
      }
      properties.resize( propertyCount );
      result = static_cast<Result>( vkGetPhysicalDeviceDisplayPropertiesKHR( m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayPropertiesKHR*>( properties.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::getDisplayPropertiesKHR" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result getDisplayPlanePropertiesKHR( uint32_t* pPropertyCount, DisplayPlanePropertiesKHR* pProperties ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceDisplayPlanePropertiesKHR( m_physicalDevice, pPropertyCount, reinterpret_cast<VkDisplayPlanePropertiesKHR*>( pProperties ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Result getDisplayPlanePropertiesKHR( std::vector<DisplayPlanePropertiesKHR> & properties ) const
    {
      uint32_t propertyCount;
      Result result = static_cast<Result>( vkGetPhysicalDeviceDisplayPlanePropertiesKHR( m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayPlanePropertiesKHR*>( properties.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::getDisplayPlanePropertiesKHR" );
      }
      properties.resize( propertyCount );
      result = static_cast<Result>( vkGetPhysicalDeviceDisplayPlanePropertiesKHR( m_physicalDevice, &propertyCount, reinterpret_cast<VkDisplayPlanePropertiesKHR*>( properties.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::getDisplayPlanePropertiesKHR" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result getDisplayPlaneSupportedDisplaysKHR( uint32_t planeIndex, uint32_t* pDisplayCount, DisplayKHR* pDisplays ) const
    {
      return static_cast<Result>( vkGetDisplayPlaneSupportedDisplaysKHR( m_physicalDevice, planeIndex, pDisplayCount, reinterpret_cast<VkDisplayKHR*>( pDisplays ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Result getDisplayPlaneSupportedDisplaysKHR( uint32_t planeIndex, std::vector<DisplayKHR> & displays ) const
    {
      uint32_t displayCount;
      Result result = static_cast<Result>( vkGetDisplayPlaneSupportedDisplaysKHR( m_physicalDevice, planeIndex, &displayCount, reinterpret_cast<VkDisplayKHR*>( displays.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::getDisplayPlaneSupportedDisplaysKHR" );
      }
      displays.resize( displayCount );
      result = static_cast<Result>( vkGetDisplayPlaneSupportedDisplaysKHR( m_physicalDevice, planeIndex, &displayCount, reinterpret_cast<VkDisplayKHR*>( displays.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::getDisplayPlaneSupportedDisplaysKHR" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result getDisplayModePropertiesKHR( DisplayKHR display, uint32_t* pPropertyCount, DisplayModePropertiesKHR* pProperties ) const
    {
      return static_cast<Result>( vkGetDisplayModePropertiesKHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), pPropertyCount, reinterpret_cast<VkDisplayModePropertiesKHR*>( pProperties ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Result getDisplayModePropertiesKHR( DisplayKHR display, std::vector<DisplayModePropertiesKHR> & properties ) const
    {
      uint32_t propertyCount;
      Result result = static_cast<Result>( vkGetDisplayModePropertiesKHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), &propertyCount, reinterpret_cast<VkDisplayModePropertiesKHR*>( properties.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::getDisplayModePropertiesKHR" );
      }
      properties.resize( propertyCount );
      result = static_cast<Result>( vkGetDisplayModePropertiesKHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), &propertyCount, reinterpret_cast<VkDisplayModePropertiesKHR*>( properties.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::getDisplayModePropertiesKHR" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result createDisplayModeKHR( DisplayKHR display, const DisplayModeCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, DisplayModeKHR* pMode ) const
    {
      return static_cast<Result>( vkCreateDisplayModeKHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), reinterpret_cast<const VkDisplayModeCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkDisplayModeKHR*>( pMode ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    DisplayModeKHR createDisplayModeKHR( DisplayKHR display, const DisplayModeCreateInfoKHR& createInfo, const AllocationCallbacks& allocator ) const
    {
      DisplayModeKHR mode;
      Result result = static_cast<Result>( vkCreateDisplayModeKHR( m_physicalDevice, static_cast<VkDisplayKHR>( display ), reinterpret_cast<const VkDisplayModeCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkDisplayModeKHR*>( &mode ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::PhysicalDevice::createDisplayModeKHR" );
      }
      return mode;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result getDisplayPlaneCapabilitiesKHR( DisplayModeKHR mode, uint32_t planeIndex, DisplayPlaneCapabilitiesKHR* pCapabilities ) const
    {
      return static_cast<Result>( vkGetDisplayPlaneCapabilitiesKHR( m_physicalDevice, static_cast<VkDisplayModeKHR>( mode ), planeIndex, reinterpret_cast<VkDisplayPlaneCapabilitiesKHR*>( pCapabilities ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    DisplayPlaneCapabilitiesKHR getDisplayPlaneCapabilitiesKHR( DisplayModeKHR mode, uint32_t planeIndex ) const
    {
      DisplayPlaneCapabilitiesKHR capabilities;
      Result result = static_cast<Result>( vkGetDisplayPlaneCapabilitiesKHR( m_physicalDevice, static_cast<VkDisplayModeKHR>( mode ), planeIndex, reinterpret_cast<VkDisplayPlaneCapabilitiesKHR*>( &capabilities ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::PhysicalDevice::getDisplayPlaneCapabilitiesKHR" );
      }
      return capabilities;
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_MIR_KHR
    Bool32 getMirPresentationSupportKHR( uint32_t queueFamilyIndex, MirConnection* connection ) const
    {
      return vkGetPhysicalDeviceMirPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, connection );
    }
#endif /*VK_USE_PLATFORM_MIR_KHR*/

#ifdef VKCPP_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_MIR_KHR
    Bool32 getMirPresentationSupportKHR( uint32_t queueFamilyIndex, MirConnection& connection ) const
    {
      return vkGetPhysicalDeviceMirPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, &connection );
    }
#endif /*VK_USE_PLATFORM_MIR_KHR*/
#endif /*VKCPP_ENHANCED_MODE*/

    Result getSurfaceSupportKHR( uint32_t queueFamilyIndex, SurfaceKHR surface, Bool32* pSupported ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceSurfaceSupportKHR( m_physicalDevice, queueFamilyIndex, static_cast<VkSurfaceKHR>( surface ), pSupported ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Bool32 getSurfaceSupportKHR( uint32_t queueFamilyIndex, SurfaceKHR surface ) const
    {
      Bool32 supported;
      Result result = static_cast<Result>( vkGetPhysicalDeviceSurfaceSupportKHR( m_physicalDevice, queueFamilyIndex, static_cast<VkSurfaceKHR>( surface ), &supported ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::PhysicalDevice::getSurfaceSupportKHR" );
      }
      return supported;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result getSurfaceCapabilitiesKHR( SurfaceKHR surface, SurfaceCapabilitiesKHR* pSurfaceCapabilities ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkSurfaceCapabilitiesKHR*>( pSurfaceCapabilities ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Result getSurfaceCapabilitiesKHR( SurfaceKHR surface, SurfaceCapabilitiesKHR& surfaceCapabilities ) const
    {
      Result result = static_cast<Result>( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<VkSurfaceCapabilitiesKHR*>( &surfaceCapabilities ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::getSurfaceCapabilitiesKHR" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result getSurfaceFormatsKHR( SurfaceKHR surface, uint32_t* pSurfaceFormatCount, SurfaceFormatKHR* pSurfaceFormats ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceSurfaceFormatsKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), pSurfaceFormatCount, reinterpret_cast<VkSurfaceFormatKHR*>( pSurfaceFormats ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Result getSurfaceFormatsKHR( SurfaceKHR surface, std::vector<SurfaceFormatKHR> & surfaceFormats ) const
    {
      uint32_t surfaceFormatCount;
      Result result = static_cast<Result>( vkGetPhysicalDeviceSurfaceFormatsKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &surfaceFormatCount, reinterpret_cast<VkSurfaceFormatKHR*>( surfaceFormats.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::getSurfaceFormatsKHR" );
      }
      surfaceFormats.resize( surfaceFormatCount );
      result = static_cast<Result>( vkGetPhysicalDeviceSurfaceFormatsKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &surfaceFormatCount, reinterpret_cast<VkSurfaceFormatKHR*>( surfaceFormats.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::getSurfaceFormatsKHR" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result getSurfacePresentModesKHR( SurfaceKHR surface, uint32_t* pPresentModeCount, PresentModeKHR* pPresentModes ) const
    {
      return static_cast<Result>( vkGetPhysicalDeviceSurfacePresentModesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), pPresentModeCount, reinterpret_cast<VkPresentModeKHR*>( pPresentModes ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    Result getSurfacePresentModesKHR( SurfaceKHR surface, std::vector<PresentModeKHR> & presentModes ) const
    {
      uint32_t presentModeCount;
      Result result = static_cast<Result>( vkGetPhysicalDeviceSurfacePresentModesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &presentModeCount, reinterpret_cast<VkPresentModeKHR*>( presentModes.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::getSurfacePresentModesKHR" );
      }
      presentModes.resize( presentModeCount );
      result = static_cast<Result>( vkGetPhysicalDeviceSurfacePresentModesKHR( m_physicalDevice, static_cast<VkSurfaceKHR>( surface ), &presentModeCount, reinterpret_cast<VkPresentModeKHR*>( presentModes.data() ) ) );
      if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
      {
        throw Exception( result, "vk::PhysicalDevice::getSurfacePresentModesKHR" );
      }
      return result;
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    Bool32 getWaylandPresentationSupportKHR( uint32_t queueFamilyIndex, struct wl_display* display ) const
    {
      return vkGetPhysicalDeviceWaylandPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, display );
    }
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VKCPP_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    Bool32 getWaylandPresentationSupportKHR( uint32_t queueFamilyIndex, struct wl_display& display ) const
    {
      return vkGetPhysicalDeviceWaylandPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, &display );
    }
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/
#endif /*VKCPP_ENHANCED_MODE*/

#ifndef VKCPP_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_WIN32_KHR
    Bool32 getWin32PresentationSupportKHR( uint32_t queueFamilyIndex ) const
    {
      return vkGetPhysicalDeviceWin32PresentationSupportKHR( m_physicalDevice, queueFamilyIndex );
    }
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#endif /*!VKCPP_ENHANCED_MODE*/

#ifdef VKCPP_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_WIN32_KHR
    Bool32 getWin32PresentationSupportKHR( uint32_t queueFamilyIndex ) const
    {
      return vkGetPhysicalDeviceWin32PresentationSupportKHR( m_physicalDevice, queueFamilyIndex );
    }
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#endif /*VKCPP_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
    Bool32 getXlibPresentationSupportKHR( uint32_t queueFamilyIndex, Display* dpy, VisualID visualID ) const
    {
      return vkGetPhysicalDeviceXlibPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, dpy, visualID );
    }
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#ifdef VKCPP_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_XLIB_KHR
    Bool32 getXlibPresentationSupportKHR( uint32_t queueFamilyIndex, Display& dpy, VisualID visualID ) const
    {
      return vkGetPhysicalDeviceXlibPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, &dpy, visualID );
    }
#endif /*VK_USE_PLATFORM_XLIB_KHR*/
#endif /*VKCPP_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_XCB_KHR
    Bool32 getXcbPresentationSupportKHR( uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id ) const
    {
      return vkGetPhysicalDeviceXcbPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, connection, visual_id );
    }
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifdef VKCPP_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_XCB_KHR
    Bool32 getXcbPresentationSupportKHR( uint32_t queueFamilyIndex, xcb_connection_t& connection, xcb_visualid_t visual_id ) const
    {
      return vkGetPhysicalDeviceXcbPresentationSupportKHR( m_physicalDevice, queueFamilyIndex, &connection, visual_id );
    }
#endif /*VK_USE_PLATFORM_XCB_KHR*/
#endif /*VKCPP_ENHANCED_MODE*/

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

  enum class DebugReportFlagBitsEXT
  {
    eInformation = VK_DEBUG_REPORT_INFORMATION_BIT_EXT,
    eWarning = VK_DEBUG_REPORT_WARNING_BIT_EXT,
    ePerformanceWarning = VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
    eError = VK_DEBUG_REPORT_ERROR_BIT_EXT,
    eDebug = VK_DEBUG_REPORT_DEBUG_BIT_EXT
  };

  typedef Flags<DebugReportFlagBitsEXT, VkDebugReportFlagsEXT> DebugReportFlagsEXT;

  inline DebugReportFlagsEXT operator|( DebugReportFlagBitsEXT bit0, DebugReportFlagBitsEXT bit1 )
  {
    return DebugReportFlagsEXT( bit0 ) | bit1;
  }

  class DebugReportCallbackCreateInfoEXT
  {
  public:
    DebugReportCallbackCreateInfoEXT( DebugReportFlagsEXT flags, PFN_vkDebugReportCallbackEXT pfnCallback, void* pUserData)
    {
      m_debugReportCallbackCreateInfoEXT.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
      m_debugReportCallbackCreateInfoEXT.pNext = nullptr;
      m_debugReportCallbackCreateInfoEXT.flags = static_cast<VkDebugReportFlagsEXT>( flags );
      m_debugReportCallbackCreateInfoEXT.pfnCallback = pfnCallback;
      m_debugReportCallbackCreateInfoEXT.pUserData = pUserData;
    }

    DebugReportCallbackCreateInfoEXT(VkDebugReportCallbackCreateInfoEXT const & rhs)
      : m_debugReportCallbackCreateInfoEXT(rhs)
    {
    }

    DebugReportCallbackCreateInfoEXT& operator=(VkDebugReportCallbackCreateInfoEXT const & rhs)
    {
      m_debugReportCallbackCreateInfoEXT = rhs;
      return *this;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_debugReportCallbackCreateInfoEXT.sType );
    }

    StructureType& sType()
    {
      return reinterpret_cast<StructureType&>( m_debugReportCallbackCreateInfoEXT.sType );
    }

    DebugReportCallbackCreateInfoEXT& sType( StructureType sType )
    {
      m_debugReportCallbackCreateInfoEXT.sType = static_cast<VkStructureType>( sType );
      return *this;
    }

    const void* pNext() const
    {
      return reinterpret_cast<const void*>( m_debugReportCallbackCreateInfoEXT.pNext );
    }

    const void* pNext()
    {
      return reinterpret_cast<const void*>( m_debugReportCallbackCreateInfoEXT.pNext );
    }

    DebugReportCallbackCreateInfoEXT& pNext( const void* pNext )
    {
      m_debugReportCallbackCreateInfoEXT.pNext = pNext;
      return *this;
    }

    const DebugReportFlagsEXT& flags() const
    {
      return reinterpret_cast<const DebugReportFlagsEXT&>( m_debugReportCallbackCreateInfoEXT.flags );
    }

    DebugReportFlagsEXT& flags()
    {
      return reinterpret_cast<DebugReportFlagsEXT&>( m_debugReportCallbackCreateInfoEXT.flags );
    }

    DebugReportCallbackCreateInfoEXT& flags( DebugReportFlagsEXT flags )
    {
      m_debugReportCallbackCreateInfoEXT.flags = static_cast<VkDebugReportFlagsEXT>( flags );
      return *this;
    }

    const PFN_vkDebugReportCallbackEXT& pfnCallback() const
    {
      return m_debugReportCallbackCreateInfoEXT.pfnCallback;
    }

    PFN_vkDebugReportCallbackEXT& pfnCallback()
    {
      return m_debugReportCallbackCreateInfoEXT.pfnCallback;
    }

    DebugReportCallbackCreateInfoEXT& pfnCallback( PFN_vkDebugReportCallbackEXT pfnCallback )
    {
      m_debugReportCallbackCreateInfoEXT.pfnCallback = pfnCallback;
      return *this;
    }

    const void* pUserData() const
    {
      return reinterpret_cast<const void*>( m_debugReportCallbackCreateInfoEXT.pUserData );
    }

    void* pUserData()
    {
      return reinterpret_cast<void*>( m_debugReportCallbackCreateInfoEXT.pUserData );
    }

    DebugReportCallbackCreateInfoEXT& pUserData( void* pUserData )
    {
      m_debugReportCallbackCreateInfoEXT.pUserData = pUserData;
      return *this;
    }

    static DebugReportCallbackCreateInfoEXT& null()
    {
      return *((DebugReportCallbackCreateInfoEXT*)(nullptr));
    }

    operator const VkDebugReportCallbackCreateInfoEXT&() const
    {
      return m_debugReportCallbackCreateInfoEXT;
    }

  private:
    VkDebugReportCallbackCreateInfoEXT m_debugReportCallbackCreateInfoEXT;
  };
  static_assert( sizeof( DebugReportCallbackCreateInfoEXT ) == sizeof( VkDebugReportCallbackCreateInfoEXT ), "struct and wrapper have different size!" );

  enum class DebugReportObjectTypeEXT
  {
    eUnknownEXT = VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT,
    eInstanceEXT = VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT,
    ePhysicalDeviceEXT = VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT,
    eDeviceEXT = VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT,
    eQueueEXT = VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT,
    eSemaphoreEXT = VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT,
    eCommandBufferEXT = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT,
    eFenceEXT = VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT,
    eDeviceMemoryEXT = VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT,
    eBufferEXT = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT,
    eImageEXT = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT,
    eEventEXT = VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT,
    eQueryPoolEXT = VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT,
    eBufferViewEXT = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT,
    eImageViewEXT = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT,
    eShaderModuleEXT = VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT,
    ePipelineCacheEXT = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT,
    ePipelineLayoutEXT = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT,
    eRenderPassEXT = VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT,
    ePipelineEXT = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
    eDescriptorSetLayoutEXT = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT,
    eSamplerEXT = VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT,
    eDescriptorPoolEXT = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT,
    eDescriptorSetEXT = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT,
    eFramebufferEXT = VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT,
    eCommandPoolEXT = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT,
    eSurfaceKhrEXT = VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT,
    eSwapchainKhrEXT = VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT,
    eDebugReportEXT = VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT
  };

  class Instance
  {
  public:
    Instance()
      : m_instance(VK_NULL_HANDLE)
    {}

#if defined(VK_CPP_TYPESAFE_CONVERSION)
    Instance(VkInstance instance)
       : m_instance(instance)
    {}

    Instance& operator=(VkInstance instance)
    {
      m_instance = instance;
      return *this;
    }
#endif

    void destroy( const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyInstance( m_instance, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroy( const AllocationCallbacks& allocator ) const
    {
      vkDestroyInstance( m_instance, reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    Result enumeratePhysicalDevices( uint32_t* pPhysicalDeviceCount, PhysicalDevice* pPhysicalDevices ) const
    {
      return static_cast<Result>( vkEnumeratePhysicalDevices( m_instance, pPhysicalDeviceCount, reinterpret_cast<VkPhysicalDevice*>( pPhysicalDevices ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    std::vector<PhysicalDevice> enumeratePhysicalDevices(  ) const
    {
      std::vector<PhysicalDevice> physicalDevices;
      uint32_t physicalDeviceCount;
      Result result = static_cast<Result>( vkEnumeratePhysicalDevices( m_instance, &physicalDeviceCount, reinterpret_cast<VkPhysicalDevice*>( physicalDevices.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Instance::enumeratePhysicalDevices" );
      }
      physicalDevices.resize( physicalDeviceCount );
      result = static_cast<Result>( vkEnumeratePhysicalDevices( m_instance, &physicalDeviceCount, reinterpret_cast<VkPhysicalDevice*>( physicalDevices.data() ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Instance::enumeratePhysicalDevices" );
      }
      return std::move( physicalDevices );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    PFN_vkVoidFunction getProcAddr( const char* pName ) const
    {
      return vkGetInstanceProcAddr( m_instance, pName );
    }

#ifdef VKCPP_ENHANCED_MODE
    PFN_vkVoidFunction getProcAddr( std::string const& name ) const
    {
      return vkGetInstanceProcAddr( m_instance, name.c_str() );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_ANDROID_KHR
    Result createAndroidSurfaceKHR( const AndroidSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateAndroidSurfaceKHR( m_instance, reinterpret_cast<const VkAndroidSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#ifdef VKCPP_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    SurfaceKHR createAndroidSurfaceKHR( const AndroidSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateAndroidSurfaceKHR( m_instance, reinterpret_cast<const VkAndroidSurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Instance::createAndroidSurfaceKHR" );
      }
      return surface;
    }
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/
#endif /*VKCPP_ENHANCED_MODE*/

    Result createDisplayPlaneSurfaceKHR( const DisplaySurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateDisplayPlaneSurfaceKHR( m_instance, reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    SurfaceKHR createDisplayPlaneSurfaceKHR( const DisplaySurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateDisplayPlaneSurfaceKHR( m_instance, reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Instance::createDisplayPlaneSurfaceKHR" );
      }
      return surface;
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_MIR_KHR
    Result createMirSurfaceKHR( const MirSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateMirSurfaceKHR( m_instance, reinterpret_cast<const VkMirSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }
#endif /*VK_USE_PLATFORM_MIR_KHR*/

#ifdef VKCPP_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_MIR_KHR
    SurfaceKHR createMirSurfaceKHR( const MirSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateMirSurfaceKHR( m_instance, reinterpret_cast<const VkMirSurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Instance::createMirSurfaceKHR" );
      }
      return surface;
    }
#endif /*VK_USE_PLATFORM_MIR_KHR*/
#endif /*VKCPP_ENHANCED_MODE*/

    void destroySurfaceKHR( SurfaceKHR surface, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroySurfaceKHR( m_instance, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroySurfaceKHR( SurfaceKHR surface, const AllocationCallbacks& allocator ) const
    {
      vkDestroySurfaceKHR( m_instance, static_cast<VkSurfaceKHR>( surface ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    Result createWaylandSurfaceKHR( const WaylandSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateWaylandSurfaceKHR( m_instance, reinterpret_cast<const VkWaylandSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VKCPP_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    SurfaceKHR createWaylandSurfaceKHR( const WaylandSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateWaylandSurfaceKHR( m_instance, reinterpret_cast<const VkWaylandSurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Instance::createWaylandSurfaceKHR" );
      }
      return surface;
    }
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/
#endif /*VKCPP_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
    Result createWin32SurfaceKHR( const Win32SurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateWin32SurfaceKHR( m_instance, reinterpret_cast<const VkWin32SurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VKCPP_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_WIN32_KHR
    SurfaceKHR createWin32SurfaceKHR( const Win32SurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateWin32SurfaceKHR( m_instance, reinterpret_cast<const VkWin32SurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Instance::createWin32SurfaceKHR" );
      }
      return surface;
    }
#endif /*VK_USE_PLATFORM_WIN32_KHR*/
#endif /*VKCPP_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
    Result createXlibSurfaceKHR( const XlibSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateXlibSurfaceKHR( m_instance, reinterpret_cast<const VkXlibSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#ifdef VKCPP_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_XLIB_KHR
    SurfaceKHR createXlibSurfaceKHR( const XlibSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateXlibSurfaceKHR( m_instance, reinterpret_cast<const VkXlibSurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Instance::createXlibSurfaceKHR" );
      }
      return surface;
    }
#endif /*VK_USE_PLATFORM_XLIB_KHR*/
#endif /*VKCPP_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_XCB_KHR
    Result createXcbSurfaceKHR( const XcbSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface ) const
    {
      return static_cast<Result>( vkCreateXcbSurfaceKHR( m_instance, reinterpret_cast<const VkXcbSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkSurfaceKHR*>( pSurface ) ) );
    }
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifdef VKCPP_ENHANCED_MODE
#ifdef VK_USE_PLATFORM_XCB_KHR
    SurfaceKHR createXcbSurfaceKHR( const XcbSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator ) const
    {
      SurfaceKHR surface;
      Result result = static_cast<Result>( vkCreateXcbSurfaceKHR( m_instance, reinterpret_cast<const VkXcbSurfaceCreateInfoKHR*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkSurfaceKHR*>( &surface ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Instance::createXcbSurfaceKHR" );
      }
      return surface;
    }
#endif /*VK_USE_PLATFORM_XCB_KHR*/
#endif /*VKCPP_ENHANCED_MODE*/

    Result createDebugReportCallbackEXT( const DebugReportCallbackCreateInfoEXT* pCreateInfo, const AllocationCallbacks* pAllocator, DebugReportCallbackEXT* pCallback ) const
    {
      return static_cast<Result>( vkCreateDebugReportCallbackEXT( m_instance, reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkDebugReportCallbackEXT*>( pCallback ) ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    DebugReportCallbackEXT createDebugReportCallbackEXT( const DebugReportCallbackCreateInfoEXT& createInfo, const AllocationCallbacks& allocator ) const
    {
      DebugReportCallbackEXT callback;
      Result result = static_cast<Result>( vkCreateDebugReportCallbackEXT( m_instance, reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkDebugReportCallbackEXT*>( &callback ) ) );
      if ( result != Result::eSuccess )
      {
        throw Exception( result, "vk::Instance::createDebugReportCallbackEXT" );
      }
      return callback;
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void destroyDebugReportCallbackEXT( DebugReportCallbackEXT callback, const AllocationCallbacks* pAllocator ) const
    {
      vkDestroyDebugReportCallbackEXT( m_instance, static_cast<VkDebugReportCallbackEXT>( callback ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
    }

#ifdef VKCPP_ENHANCED_MODE
    void destroyDebugReportCallbackEXT( DebugReportCallbackEXT callback, const AllocationCallbacks& allocator ) const
    {
      vkDestroyDebugReportCallbackEXT( m_instance, static_cast<VkDebugReportCallbackEXT>( callback ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ) );
    }
#endif /*VKCPP_ENHANCED_MODE*/

    void debugReportMessageEXT( DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage ) const
    {
      vkDebugReportMessageEXT( m_instance, static_cast<VkDebugReportFlagsEXT>( flags ), static_cast<VkDebugReportObjectTypeEXT>( objectType ), object, location, messageCode, pLayerPrefix, pMessage );
    }

#ifdef VKCPP_ENHANCED_MODE
    void debugReportMessageEXT( DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, std::string const& layerPrefix, std::string const& message ) const
    {
      vkDebugReportMessageEXT( m_instance, static_cast<VkDebugReportFlagsEXT>( flags ), static_cast<VkDebugReportObjectTypeEXT>( objectType ), object, location, messageCode, layerPrefix.c_str(), message.c_str() );
    }
#endif /*VKCPP_ENHANCED_MODE*/

#if !defined(VK_CPP_TYPESAFE_CONVERSION)
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

  enum class DebugReportErrorEXT
  {
    eNoneEXT = VK_DEBUG_REPORT_ERROR_NONE_EXT,
    eCallbackRefEXT = VK_DEBUG_REPORT_ERROR_CALLBACK_REF_EXT
  };

  inline Result createInstance( const InstanceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Instance* pInstance )
  {
    return static_cast<Result>( vkCreateInstance( reinterpret_cast<const VkInstanceCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), reinterpret_cast<VkInstance*>( pInstance ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Instance createInstance( const InstanceCreateInfo& createInfo, const AllocationCallbacks& allocator )
  {
    Instance instance;
    Result result = static_cast<Result>( vkCreateInstance( reinterpret_cast<const VkInstanceCreateInfo*>( &createInfo ), reinterpret_cast<const VkAllocationCallbacks*>( &allocator ), reinterpret_cast<VkInstance*>( &instance ) ) );
    if ( result != Result::eSuccess )
    {
      throw Exception( result, "vk::createInstance" );
    }
    return instance;
  }
#endif /*VKCPP_ENHANCED_MODE*/

  inline Result enumerateInstanceLayerProperties( uint32_t* pPropertyCount, LayerProperties* pProperties )
  {
    return static_cast<Result>( vkEnumerateInstanceLayerProperties( pPropertyCount, reinterpret_cast<VkLayerProperties*>( pProperties ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result enumerateInstanceLayerProperties( std::vector<LayerProperties> & properties )
  {
    uint32_t propertyCount;
    Result result = static_cast<Result>( vkEnumerateInstanceLayerProperties( &propertyCount, reinterpret_cast<VkLayerProperties*>( properties.data() ) ) );
    if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
    {
      throw Exception( result, "vk::enumerateInstanceLayerProperties" );
    }
    properties.resize( propertyCount );
    result = static_cast<Result>( vkEnumerateInstanceLayerProperties( &propertyCount, reinterpret_cast<VkLayerProperties*>( properties.data() ) ) );
    if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
    {
      throw Exception( result, "vk::enumerateInstanceLayerProperties" );
    }
    return result;
  }
#endif /*VKCPP_ENHANCED_MODE*/

  inline Result enumerateInstanceExtensionProperties( const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties )
  {
    return static_cast<Result>( vkEnumerateInstanceExtensionProperties( pLayerName, pPropertyCount, reinterpret_cast<VkExtensionProperties*>( pProperties ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result enumerateInstanceExtensionProperties( std::string const& layerName, std::vector<ExtensionProperties> & properties )
  {
    uint32_t propertyCount;
    Result result = static_cast<Result>( vkEnumerateInstanceExtensionProperties( layerName.data(), &propertyCount, reinterpret_cast<VkExtensionProperties*>( properties.data() ) ) );
    if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
    {
      throw Exception( result, "vk::enumerateInstanceExtensionProperties" );
    }
    properties.resize( propertyCount );
    result = static_cast<Result>( vkEnumerateInstanceExtensionProperties( layerName.data(), &propertyCount, reinterpret_cast<VkExtensionProperties*>( properties.data() ) ) );
    if ( ( result != Result::eSuccess ) && ( result != Result::eIncomplete ) )
    {
      throw Exception( result, "vk::enumerateInstanceExtensionProperties" );
    }
    return result;
  }
#endif /*VKCPP_ENHANCED_MODE*/

  inline std::string getString(FramebufferCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(FramebufferCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(QueryPoolCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(QueryPoolCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(RenderPassCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(RenderPassCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(SamplerCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(SamplerCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(PipelineLayoutCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(PipelineLayoutCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(PipelineCacheCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(PipelineCacheCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(PipelineDepthStencilStateCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(PipelineDepthStencilStateCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(PipelineDynamicStateCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(PipelineDynamicStateCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(PipelineColorBlendStateCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(PipelineColorBlendStateCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(PipelineMultisampleStateCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(PipelineMultisampleStateCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(PipelineRasterizationStateCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(PipelineRasterizationStateCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(PipelineViewportStateCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(PipelineViewportStateCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(PipelineTessellationStateCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(PipelineTessellationStateCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(PipelineInputAssemblyStateCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(PipelineInputAssemblyStateCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(PipelineVertexInputStateCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(PipelineVertexInputStateCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(PipelineShaderStageCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(PipelineShaderStageCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(DescriptorSetLayoutCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(DescriptorSetLayoutCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(BufferViewCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(BufferViewCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(InstanceCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(InstanceCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(DeviceCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(DeviceCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(DeviceQueueCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(DeviceQueueCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(ImageViewCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(ImageViewCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(SemaphoreCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(SemaphoreCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(ShaderModuleCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(ShaderModuleCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(EventCreateFlagBits)
  {
    return std::string();
  }

  inline std::string getString(EventCreateFlags)
  {
    return std::string();
  }

  inline std::string getString(MemoryMapFlagBits)
  {
    return std::string();
  }

  inline std::string getString(MemoryMapFlags)
  {
    return std::string();
  }

  inline std::string getString(SubpassDescriptionFlagBits)
  {
    return std::string();
  }

  inline std::string getString(SubpassDescriptionFlags)
  {
    return std::string();
  }

  inline std::string getString(DescriptorPoolResetFlagBits)
  {
    return std::string();
  }

  inline std::string getString(DescriptorPoolResetFlags)
  {
    return std::string();
  }

  inline std::string getString(SwapchainCreateFlagBitsKHR)
  {
    return std::string();
  }

  inline std::string getString(SwapchainCreateFlagsKHR)
  {
    return std::string();
  }

  inline std::string getString(DisplayModeCreateFlagBitsKHR)
  {
    return std::string();
  }

  inline std::string getString(DisplayModeCreateFlagsKHR)
  {
    return std::string();
  }

  inline std::string getString(DisplaySurfaceCreateFlagBitsKHR)
  {
    return std::string();
  }

  inline std::string getString(DisplaySurfaceCreateFlagsKHR)
  {
    return std::string();
  }

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  inline std::string getString(AndroidSurfaceCreateFlagBitsKHR)
  {
    return std::string();
  }
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  inline std::string getString(AndroidSurfaceCreateFlagsKHR)
  {
    return std::string();
  }
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#ifdef VK_USE_PLATFORM_MIR_KHR
  inline std::string getString(MirSurfaceCreateFlagBitsKHR)
  {
    return std::string();
  }
#endif /*VK_USE_PLATFORM_MIR_KHR*/

#ifdef VK_USE_PLATFORM_MIR_KHR
  inline std::string getString(MirSurfaceCreateFlagsKHR)
  {
    return std::string();
  }
#endif /*VK_USE_PLATFORM_MIR_KHR*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  inline std::string getString(WaylandSurfaceCreateFlagBitsKHR)
  {
    return std::string();
  }
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  inline std::string getString(WaylandSurfaceCreateFlagsKHR)
  {
    return std::string();
  }
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
  inline std::string getString(Win32SurfaceCreateFlagBitsKHR)
  {
    return std::string();
  }
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
  inline std::string getString(Win32SurfaceCreateFlagsKHR)
  {
    return std::string();
  }
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
  inline std::string getString(XlibSurfaceCreateFlagBitsKHR)
  {
    return std::string();
  }
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR
  inline std::string getString(XlibSurfaceCreateFlagsKHR)
  {
    return std::string();
  }
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
  inline std::string getString(XcbSurfaceCreateFlagBitsKHR)
  {
    return std::string();
  }
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR
  inline std::string getString(XcbSurfaceCreateFlagsKHR)
  {
    return std::string();
  }
#endif /*VK_USE_PLATFORM_XCB_KHR*/

  inline std::string getString(ImageLayout value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(AttachmentLoadOp value)
  {
    switch (value)
    {
    case AttachmentLoadOp::eLoad: return "Load";
    case AttachmentLoadOp::eClear: return "Clear";
    case AttachmentLoadOp::eDontCare: return "DontCare";
    default: return "unknown";
    }
  }

  inline std::string getString(AttachmentStoreOp value)
  {
    switch (value)
    {
    case AttachmentStoreOp::eStore: return "Store";
    case AttachmentStoreOp::eDontCare: return "DontCare";
    default: return "unknown";
    }
  }

  inline std::string getString(ImageType value)
  {
    switch (value)
    {
    case ImageType::e1D: return "1D";
    case ImageType::e2D: return "2D";
    case ImageType::e3D: return "3D";
    default: return "unknown";
    }
  }

  inline std::string getString(ImageTiling value)
  {
    switch (value)
    {
    case ImageTiling::eOptimal: return "Optimal";
    case ImageTiling::eLinear: return "Linear";
    default: return "unknown";
    }
  }

  inline std::string getString(ImageViewType value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(CommandBufferLevel value)
  {
    switch (value)
    {
    case CommandBufferLevel::ePrimary: return "Primary";
    case CommandBufferLevel::eSecondary: return "Secondary";
    default: return "unknown";
    }
  }

  inline std::string getString(ComponentSwizzle value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(DescriptorType value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(QueryType value)
  {
    switch (value)
    {
    case QueryType::eOcclusion: return "Occlusion";
    case QueryType::ePipelineStatistics: return "PipelineStatistics";
    case QueryType::eTimestamp: return "Timestamp";
    default: return "unknown";
    }
  }

  inline std::string getString(BorderColor value)
  {
    switch (value)
    {
    case BorderColor::eFloatTransparentBlack: return "FloatTransparentBlack";
    case BorderColor::eIntTransparentBlack: return "IntTransparentBlack";
    case BorderColor::eFloatOpaqueBlack: return "FloatOpaqueBlack";
    case BorderColor::eIntOpaqueBlack: return "IntOpaqueBlack";
    case BorderColor::eFloatOpaqueWhite: return "FloatOpaqueWhite";
    case BorderColor::eIntOpaqueWhite: return "IntOpaqueWhite";
    default: return "unknown";
    }
  }

  inline std::string getString(PipelineBindPoint value)
  {
    switch (value)
    {
    case PipelineBindPoint::eGraphics: return "Graphics";
    case PipelineBindPoint::eCompute: return "Compute";
    default: return "unknown";
    }
  }

  inline std::string getString(PipelineCacheHeaderVersion value)
  {
    switch (value)
    {
    case PipelineCacheHeaderVersion::eOne: return "One";
    default: return "unknown";
    }
  }

  inline std::string getString(PrimitiveTopology value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(SharingMode value)
  {
    switch (value)
    {
    case SharingMode::eExclusive: return "Exclusive";
    case SharingMode::eConcurrent: return "Concurrent";
    default: return "unknown";
    }
  }

  inline std::string getString(IndexType value)
  {
    switch (value)
    {
    case IndexType::eUint16: return "Uint16";
    case IndexType::eUint32: return "Uint32";
    default: return "unknown";
    }
  }

  inline std::string getString(Filter value)
  {
    switch (value)
    {
    case Filter::eNearest: return "Nearest";
    case Filter::eLinear: return "Linear";
    default: return "unknown";
    }
  }

  inline std::string getString(SamplerMipmapMode value)
  {
    switch (value)
    {
    case SamplerMipmapMode::eNearest: return "Nearest";
    case SamplerMipmapMode::eLinear: return "Linear";
    default: return "unknown";
    }
  }

  inline std::string getString(SamplerAddressMode value)
  {
    switch (value)
    {
    case SamplerAddressMode::eRepeat: return "Repeat";
    case SamplerAddressMode::eMirroredRepeat: return "MirroredRepeat";
    case SamplerAddressMode::eClampToEdge: return "ClampToEdge";
    case SamplerAddressMode::eClampToBorder: return "ClampToBorder";
    case SamplerAddressMode::eMirrorClampToEdge: return "MirrorClampToEdge";
    default: return "unknown";
    }
  }

  inline std::string getString(CompareOp value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(PolygonMode value)
  {
    switch (value)
    {
    case PolygonMode::eFill: return "Fill";
    case PolygonMode::eLine: return "Line";
    case PolygonMode::ePoint: return "Point";
    default: return "unknown";
    }
  }

  inline std::string getString(CullModeFlagBits value)
  {
    switch (value)
    {
    case CullModeFlagBits::eNone: return "None";
    case CullModeFlagBits::eFront: return "Front";
    case CullModeFlagBits::eBack: return "Back";
    case CullModeFlagBits::eFrontAndBack: return "FrontAndBack";
    default: return "unknown";
    }
  }

  inline std::string getString(CullModeFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CullModeFlagBits::eNone) result += "None | ";
    if (value & vk::CullModeFlagBits::eFront) result += "Front | ";
    if (value & vk::CullModeFlagBits::eBack) result += "Back | ";
    if (value & vk::CullModeFlagBits::eFrontAndBack) result += "FrontAndBack | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(FrontFace value)
  {
    switch (value)
    {
    case FrontFace::eCounterClockwise: return "CounterClockwise";
    case FrontFace::eClockwise: return "Clockwise";
    default: return "unknown";
    }
  }

  inline std::string getString(BlendFactor value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(BlendOp value)
  {
    switch (value)
    {
    case BlendOp::eAdd: return "Add";
    case BlendOp::eSubtract: return "Subtract";
    case BlendOp::eReverseSubtract: return "ReverseSubtract";
    case BlendOp::eMin: return "Min";
    case BlendOp::eMax: return "Max";
    default: return "unknown";
    }
  }

  inline std::string getString(StencilOp value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(LogicOp value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(InternalAllocationType value)
  {
    switch (value)
    {
    case InternalAllocationType::eExecutable: return "Executable";
    default: return "unknown";
    }
  }

  inline std::string getString(SystemAllocationScope value)
  {
    switch (value)
    {
    case SystemAllocationScope::eCommand: return "Command";
    case SystemAllocationScope::eObject: return "Object";
    case SystemAllocationScope::eCache: return "Cache";
    case SystemAllocationScope::eDevice: return "Device";
    case SystemAllocationScope::eInstance: return "Instance";
    default: return "unknown";
    }
  }

  inline std::string getString(PhysicalDeviceType value)
  {
    switch (value)
    {
    case PhysicalDeviceType::eOther: return "Other";
    case PhysicalDeviceType::eIntegratedGpu: return "IntegratedGpu";
    case PhysicalDeviceType::eDiscreteGpu: return "DiscreteGpu";
    case PhysicalDeviceType::eVirtualGpu: return "VirtualGpu";
    case PhysicalDeviceType::eCpu: return "Cpu";
    default: return "unknown";
    }
  }

  inline std::string getString(VertexInputRate value)
  {
    switch (value)
    {
    case VertexInputRate::eVertex: return "Vertex";
    case VertexInputRate::eInstance: return "Instance";
    default: return "unknown";
    }
  }

  inline std::string getString(Format value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(StructureType value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(SubpassContents value)
  {
    switch (value)
    {
    case SubpassContents::eInline: return "Inline";
    case SubpassContents::eSecondaryCommandBuffers: return "SecondaryCommandBuffers";
    default: return "unknown";
    }
  }

  inline std::string getString(Result value)
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
    case Result::eErrorSurfaceLostKHR: return "ErrorSurfaceLostKHR";
    case Result::eErrorNativeWindowInUseKHR: return "ErrorNativeWindowInUseKHR";
    case Result::eSuboptimalKHR: return "SuboptimalKHR";
    case Result::eErrorOutOfDateKHR: return "ErrorOutOfDateKHR";
    case Result::eErrorIncompatibleDisplayKHR: return "ErrorIncompatibleDisplayKHR";
    case Result::eErrorValidationFailedEXT: return "ErrorValidationFailedEXT";
    default: return "unknown";
    }
  }

  inline std::string getString(DynamicState value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(QueueFlagBits value)
  {
    switch (value)
    {
    case QueueFlagBits::eGraphics: return "Graphics";
    case QueueFlagBits::eCompute: return "Compute";
    case QueueFlagBits::eTransfer: return "Transfer";
    case QueueFlagBits::eSparseBinding: return "SparseBinding";
    default: return "unknown";
    }
  }

  inline std::string getString(QueueFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueueFlagBits::eGraphics) result += "Graphics | ";
    if (value & vk::QueueFlagBits::eCompute) result += "Compute | ";
    if (value & vk::QueueFlagBits::eTransfer) result += "Transfer | ";
    if (value & vk::QueueFlagBits::eSparseBinding) result += "SparseBinding | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(MemoryPropertyFlagBits value)
  {
    switch (value)
    {
    case MemoryPropertyFlagBits::eDeviceLocal: return "DeviceLocal";
    case MemoryPropertyFlagBits::eHostVisible: return "HostVisible";
    case MemoryPropertyFlagBits::eHostCoherent: return "HostCoherent";
    case MemoryPropertyFlagBits::eHostCached: return "HostCached";
    case MemoryPropertyFlagBits::eLazilyAllocated: return "LazilyAllocated";
    default: return "unknown";
    }
  }

  inline std::string getString(MemoryPropertyFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::MemoryPropertyFlagBits::eDeviceLocal) result += "DeviceLocal | ";
    if (value & vk::MemoryPropertyFlagBits::eHostVisible) result += "HostVisible | ";
    if (value & vk::MemoryPropertyFlagBits::eHostCoherent) result += "HostCoherent | ";
    if (value & vk::MemoryPropertyFlagBits::eHostCached) result += "HostCached | ";
    if (value & vk::MemoryPropertyFlagBits::eLazilyAllocated) result += "LazilyAllocated | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(MemoryHeapFlagBits value)
  {
    switch (value)
    {
    case MemoryHeapFlagBits::eDeviceLocal: return "DeviceLocal";
    default: return "unknown";
    }
  }

  inline std::string getString(MemoryHeapFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::MemoryHeapFlagBits::eDeviceLocal) result += "DeviceLocal | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(AccessFlagBits value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(AccessFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::AccessFlagBits::eIndirectCommandRead) result += "IndirectCommandRead | ";
    if (value & vk::AccessFlagBits::eIndexRead) result += "IndexRead | ";
    if (value & vk::AccessFlagBits::eVertexAttributeRead) result += "VertexAttributeRead | ";
    if (value & vk::AccessFlagBits::eUniformRead) result += "UniformRead | ";
    if (value & vk::AccessFlagBits::eInputAttachmentRead) result += "InputAttachmentRead | ";
    if (value & vk::AccessFlagBits::eShaderRead) result += "ShaderRead | ";
    if (value & vk::AccessFlagBits::eShaderWrite) result += "ShaderWrite | ";
    if (value & vk::AccessFlagBits::eColorAttachmentRead) result += "ColorAttachmentRead | ";
    if (value & vk::AccessFlagBits::eColorAttachmentWrite) result += "ColorAttachmentWrite | ";
    if (value & vk::AccessFlagBits::eDepthStencilAttachmentRead) result += "DepthStencilAttachmentRead | ";
    if (value & vk::AccessFlagBits::eDepthStencilAttachmentWrite) result += "DepthStencilAttachmentWrite | ";
    if (value & vk::AccessFlagBits::eTransferRead) result += "TransferRead | ";
    if (value & vk::AccessFlagBits::eTransferWrite) result += "TransferWrite | ";
    if (value & vk::AccessFlagBits::eHostRead) result += "HostRead | ";
    if (value & vk::AccessFlagBits::eHostWrite) result += "HostWrite | ";
    if (value & vk::AccessFlagBits::eMemoryRead) result += "MemoryRead | ";
    if (value & vk::AccessFlagBits::eMemoryWrite) result += "MemoryWrite | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(BufferUsageFlagBits value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(BufferUsageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::BufferUsageFlagBits::eTransferSrc) result += "TransferSrc | ";
    if (value & vk::BufferUsageFlagBits::eTransferDst) result += "TransferDst | ";
    if (value & vk::BufferUsageFlagBits::eUniformTexelBuffer) result += "UniformTexelBuffer | ";
    if (value & vk::BufferUsageFlagBits::eStorageTexelBuffer) result += "StorageTexelBuffer | ";
    if (value & vk::BufferUsageFlagBits::eUniformBuffer) result += "UniformBuffer | ";
    if (value & vk::BufferUsageFlagBits::eStorageBuffer) result += "StorageBuffer | ";
    if (value & vk::BufferUsageFlagBits::eIndexBuffer) result += "IndexBuffer | ";
    if (value & vk::BufferUsageFlagBits::eVertexBuffer) result += "VertexBuffer | ";
    if (value & vk::BufferUsageFlagBits::eIndirectBuffer) result += "IndirectBuffer | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(BufferCreateFlagBits value)
  {
    switch (value)
    {
    case BufferCreateFlagBits::eSparseBinding: return "SparseBinding";
    case BufferCreateFlagBits::eSparseResidency: return "SparseResidency";
    case BufferCreateFlagBits::eSparseAliased: return "SparseAliased";
    default: return "unknown";
    }
  }

  inline std::string getString(BufferCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::BufferCreateFlagBits::eSparseBinding) result += "SparseBinding | ";
    if (value & vk::BufferCreateFlagBits::eSparseResidency) result += "SparseResidency | ";
    if (value & vk::BufferCreateFlagBits::eSparseAliased) result += "SparseAliased | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(ShaderStageFlagBits value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(ShaderStageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ShaderStageFlagBits::eVertex) result += "Vertex | ";
    if (value & vk::ShaderStageFlagBits::eTessellationControl) result += "TessellationControl | ";
    if (value & vk::ShaderStageFlagBits::eTessellationEvaluation) result += "TessellationEvaluation | ";
    if (value & vk::ShaderStageFlagBits::eGeometry) result += "Geometry | ";
    if (value & vk::ShaderStageFlagBits::eFragment) result += "Fragment | ";
    if (value & vk::ShaderStageFlagBits::eCompute) result += "Compute | ";
    if (value & vk::ShaderStageFlagBits::eAllGraphics) result += "AllGraphics | ";
    if (value & vk::ShaderStageFlagBits::eAll) result += "All | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(ImageUsageFlagBits value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(ImageUsageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ImageUsageFlagBits::eTransferSrc) result += "TransferSrc | ";
    if (value & vk::ImageUsageFlagBits::eTransferDst) result += "TransferDst | ";
    if (value & vk::ImageUsageFlagBits::eSampled) result += "Sampled | ";
    if (value & vk::ImageUsageFlagBits::eStorage) result += "Storage | ";
    if (value & vk::ImageUsageFlagBits::eColorAttachment) result += "ColorAttachment | ";
    if (value & vk::ImageUsageFlagBits::eDepthStencilAttachment) result += "DepthStencilAttachment | ";
    if (value & vk::ImageUsageFlagBits::eTransientAttachment) result += "TransientAttachment | ";
    if (value & vk::ImageUsageFlagBits::eInputAttachment) result += "InputAttachment | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(ImageCreateFlagBits value)
  {
    switch (value)
    {
    case ImageCreateFlagBits::eSparseBinding: return "SparseBinding";
    case ImageCreateFlagBits::eSparseResidency: return "SparseResidency";
    case ImageCreateFlagBits::eSparseAliased: return "SparseAliased";
    case ImageCreateFlagBits::eMutableFormat: return "MutableFormat";
    case ImageCreateFlagBits::eCubeCompatible: return "CubeCompatible";
    default: return "unknown";
    }
  }

  inline std::string getString(ImageCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ImageCreateFlagBits::eSparseBinding) result += "SparseBinding | ";
    if (value & vk::ImageCreateFlagBits::eSparseResidency) result += "SparseResidency | ";
    if (value & vk::ImageCreateFlagBits::eSparseAliased) result += "SparseAliased | ";
    if (value & vk::ImageCreateFlagBits::eMutableFormat) result += "MutableFormat | ";
    if (value & vk::ImageCreateFlagBits::eCubeCompatible) result += "CubeCompatible | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(PipelineCreateFlagBits value)
  {
    switch (value)
    {
    case PipelineCreateFlagBits::eDisableOptimization: return "DisableOptimization";
    case PipelineCreateFlagBits::eAllowDerivatives: return "AllowDerivatives";
    case PipelineCreateFlagBits::eDerivative: return "Derivative";
    default: return "unknown";
    }
  }

  inline std::string getString(PipelineCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::PipelineCreateFlagBits::eDisableOptimization) result += "DisableOptimization | ";
    if (value & vk::PipelineCreateFlagBits::eAllowDerivatives) result += "AllowDerivatives | ";
    if (value & vk::PipelineCreateFlagBits::eDerivative) result += "Derivative | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(ColorComponentFlagBits value)
  {
    switch (value)
    {
    case ColorComponentFlagBits::eR: return "R";
    case ColorComponentFlagBits::eG: return "G";
    case ColorComponentFlagBits::eB: return "B";
    case ColorComponentFlagBits::eA: return "A";
    default: return "unknown";
    }
  }

  inline std::string getString(ColorComponentFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ColorComponentFlagBits::eR) result += "R | ";
    if (value & vk::ColorComponentFlagBits::eG) result += "G | ";
    if (value & vk::ColorComponentFlagBits::eB) result += "B | ";
    if (value & vk::ColorComponentFlagBits::eA) result += "A | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(FenceCreateFlagBits value)
  {
    switch (value)
    {
    case FenceCreateFlagBits::eSignaled: return "Signaled";
    default: return "unknown";
    }
  }

  inline std::string getString(FenceCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::FenceCreateFlagBits::eSignaled) result += "Signaled | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(FormatFeatureFlagBits value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(FormatFeatureFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::FormatFeatureFlagBits::eSampledImage) result += "SampledImage | ";
    if (value & vk::FormatFeatureFlagBits::eStorageImage) result += "StorageImage | ";
    if (value & vk::FormatFeatureFlagBits::eStorageImageAtomic) result += "StorageImageAtomic | ";
    if (value & vk::FormatFeatureFlagBits::eUniformTexelBuffer) result += "UniformTexelBuffer | ";
    if (value & vk::FormatFeatureFlagBits::eStorageTexelBuffer) result += "StorageTexelBuffer | ";
    if (value & vk::FormatFeatureFlagBits::eStorageTexelBufferAtomic) result += "StorageTexelBufferAtomic | ";
    if (value & vk::FormatFeatureFlagBits::eVertexBuffer) result += "VertexBuffer | ";
    if (value & vk::FormatFeatureFlagBits::eColorAttachment) result += "ColorAttachment | ";
    if (value & vk::FormatFeatureFlagBits::eColorAttachmentBlend) result += "ColorAttachmentBlend | ";
    if (value & vk::FormatFeatureFlagBits::eDepthStencilAttachment) result += "DepthStencilAttachment | ";
    if (value & vk::FormatFeatureFlagBits::eBlitSrc) result += "BlitSrc | ";
    if (value & vk::FormatFeatureFlagBits::eBlitDst) result += "BlitDst | ";
    if (value & vk::FormatFeatureFlagBits::eSampledImageFilterLinear) result += "SampledImageFilterLinear | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(QueryControlFlagBits value)
  {
    switch (value)
    {
    case QueryControlFlagBits::ePrecise: return "Precise";
    default: return "unknown";
    }
  }

  inline std::string getString(QueryControlFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueryControlFlagBits::ePrecise) result += "Precise | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(QueryResultFlagBits value)
  {
    switch (value)
    {
    case QueryResultFlagBits::e64: return "64";
    case QueryResultFlagBits::eWait: return "Wait";
    case QueryResultFlagBits::eWithAvailability: return "WithAvailability";
    case QueryResultFlagBits::ePartial: return "Partial";
    default: return "unknown";
    }
  }

  inline std::string getString(QueryResultFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueryResultFlagBits::e64) result += "64 | ";
    if (value & vk::QueryResultFlagBits::eWait) result += "Wait | ";
    if (value & vk::QueryResultFlagBits::eWithAvailability) result += "WithAvailability | ";
    if (value & vk::QueryResultFlagBits::ePartial) result += "Partial | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(CommandBufferUsageFlagBits value)
  {
    switch (value)
    {
    case CommandBufferUsageFlagBits::eOneTimeSubmit: return "OneTimeSubmit";
    case CommandBufferUsageFlagBits::eRenderPassContinue: return "RenderPassContinue";
    case CommandBufferUsageFlagBits::eSimultaneousUse: return "SimultaneousUse";
    default: return "unknown";
    }
  }

  inline std::string getString(CommandBufferUsageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandBufferUsageFlagBits::eOneTimeSubmit) result += "OneTimeSubmit | ";
    if (value & vk::CommandBufferUsageFlagBits::eRenderPassContinue) result += "RenderPassContinue | ";
    if (value & vk::CommandBufferUsageFlagBits::eSimultaneousUse) result += "SimultaneousUse | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(QueryPipelineStatisticFlagBits value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(QueryPipelineStatisticFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueryPipelineStatisticFlagBits::eInputAssemblyVertices) result += "InputAssemblyVertices | ";
    if (value & vk::QueryPipelineStatisticFlagBits::eInputAssemblyPrimitives) result += "InputAssemblyPrimitives | ";
    if (value & vk::QueryPipelineStatisticFlagBits::eVertexShaderInvocations) result += "VertexShaderInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::eGeometryShaderInvocations) result += "GeometryShaderInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::eGeometryShaderPrimitives) result += "GeometryShaderPrimitives | ";
    if (value & vk::QueryPipelineStatisticFlagBits::eClippingInvocations) result += "ClippingInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::eClippingPrimitives) result += "ClippingPrimitives | ";
    if (value & vk::QueryPipelineStatisticFlagBits::eFragmentShaderInvocations) result += "FragmentShaderInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::eTessellationControlShaderPatches) result += "TessellationControlShaderPatches | ";
    if (value & vk::QueryPipelineStatisticFlagBits::eTessellationEvaluationShaderInvocations) result += "TessellationEvaluationShaderInvocations | ";
    if (value & vk::QueryPipelineStatisticFlagBits::eComputeShaderInvocations) result += "ComputeShaderInvocations | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(ImageAspectFlagBits value)
  {
    switch (value)
    {
    case ImageAspectFlagBits::eColor: return "Color";
    case ImageAspectFlagBits::eDepth: return "Depth";
    case ImageAspectFlagBits::eStencil: return "Stencil";
    case ImageAspectFlagBits::eMetadata: return "Metadata";
    default: return "unknown";
    }
  }

  inline std::string getString(ImageAspectFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ImageAspectFlagBits::eColor) result += "Color | ";
    if (value & vk::ImageAspectFlagBits::eDepth) result += "Depth | ";
    if (value & vk::ImageAspectFlagBits::eStencil) result += "Stencil | ";
    if (value & vk::ImageAspectFlagBits::eMetadata) result += "Metadata | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(SparseImageFormatFlagBits value)
  {
    switch (value)
    {
    case SparseImageFormatFlagBits::eSingleMiptail: return "SingleMiptail";
    case SparseImageFormatFlagBits::eAlignedMipSize: return "AlignedMipSize";
    case SparseImageFormatFlagBits::eNonstandardBlockSize: return "NonstandardBlockSize";
    default: return "unknown";
    }
  }

  inline std::string getString(SparseImageFormatFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SparseImageFormatFlagBits::eSingleMiptail) result += "SingleMiptail | ";
    if (value & vk::SparseImageFormatFlagBits::eAlignedMipSize) result += "AlignedMipSize | ";
    if (value & vk::SparseImageFormatFlagBits::eNonstandardBlockSize) result += "NonstandardBlockSize | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(SparseMemoryBindFlagBits value)
  {
    switch (value)
    {
    case SparseMemoryBindFlagBits::eMetadata: return "Metadata";
    default: return "unknown";
    }
  }

  inline std::string getString(SparseMemoryBindFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SparseMemoryBindFlagBits::eMetadata) result += "Metadata | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(PipelineStageFlagBits value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(PipelineStageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::PipelineStageFlagBits::eTopOfPipe) result += "TopOfPipe | ";
    if (value & vk::PipelineStageFlagBits::eDrawIndirect) result += "DrawIndirect | ";
    if (value & vk::PipelineStageFlagBits::eVertexInput) result += "VertexInput | ";
    if (value & vk::PipelineStageFlagBits::eVertexShader) result += "VertexShader | ";
    if (value & vk::PipelineStageFlagBits::eTessellationControlShader) result += "TessellationControlShader | ";
    if (value & vk::PipelineStageFlagBits::eTessellationEvaluationShader) result += "TessellationEvaluationShader | ";
    if (value & vk::PipelineStageFlagBits::eGeometryShader) result += "GeometryShader | ";
    if (value & vk::PipelineStageFlagBits::eFragmentShader) result += "FragmentShader | ";
    if (value & vk::PipelineStageFlagBits::eEarlyFragmentTests) result += "EarlyFragmentTests | ";
    if (value & vk::PipelineStageFlagBits::eLateFragmentTests) result += "LateFragmentTests | ";
    if (value & vk::PipelineStageFlagBits::eColorAttachmentOutput) result += "ColorAttachmentOutput | ";
    if (value & vk::PipelineStageFlagBits::eComputeShader) result += "ComputeShader | ";
    if (value & vk::PipelineStageFlagBits::eTransfer) result += "Transfer | ";
    if (value & vk::PipelineStageFlagBits::eBottomOfPipe) result += "BottomOfPipe | ";
    if (value & vk::PipelineStageFlagBits::eHost) result += "Host | ";
    if (value & vk::PipelineStageFlagBits::eAllGraphics) result += "AllGraphics | ";
    if (value & vk::PipelineStageFlagBits::eAllCommands) result += "AllCommands | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(CommandPoolCreateFlagBits value)
  {
    switch (value)
    {
    case CommandPoolCreateFlagBits::eTransient: return "Transient";
    case CommandPoolCreateFlagBits::eResetCommandBuffer: return "ResetCommandBuffer";
    default: return "unknown";
    }
  }

  inline std::string getString(CommandPoolCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandPoolCreateFlagBits::eTransient) result += "Transient | ";
    if (value & vk::CommandPoolCreateFlagBits::eResetCommandBuffer) result += "ResetCommandBuffer | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(CommandPoolResetFlagBits value)
  {
    switch (value)
    {
    case CommandPoolResetFlagBits::eReleaseResources: return "ReleaseResources";
    default: return "unknown";
    }
  }

  inline std::string getString(CommandPoolResetFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandPoolResetFlagBits::eReleaseResources) result += "ReleaseResources | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(CommandBufferResetFlagBits value)
  {
    switch (value)
    {
    case CommandBufferResetFlagBits::eReleaseResources: return "ReleaseResources";
    default: return "unknown";
    }
  }

  inline std::string getString(CommandBufferResetFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandBufferResetFlagBits::eReleaseResources) result += "ReleaseResources | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(SampleCountFlagBits value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(SampleCountFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SampleCountFlagBits::e1) result += "1 | ";
    if (value & vk::SampleCountFlagBits::e2) result += "2 | ";
    if (value & vk::SampleCountFlagBits::e4) result += "4 | ";
    if (value & vk::SampleCountFlagBits::e8) result += "8 | ";
    if (value & vk::SampleCountFlagBits::e16) result += "16 | ";
    if (value & vk::SampleCountFlagBits::e32) result += "32 | ";
    if (value & vk::SampleCountFlagBits::e64) result += "64 | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(AttachmentDescriptionFlagBits value)
  {
    switch (value)
    {
    case AttachmentDescriptionFlagBits::eMayAlias: return "MayAlias";
    default: return "unknown";
    }
  }

  inline std::string getString(AttachmentDescriptionFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::AttachmentDescriptionFlagBits::eMayAlias) result += "MayAlias | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(StencilFaceFlagBits value)
  {
    switch (value)
    {
    case StencilFaceFlagBits::eFront: return "Front";
    case StencilFaceFlagBits::eBack: return "Back";
    case StencilFaceFlagBits::eVkStencilFrontAndBack: return "VkStencilFrontAndBack";
    default: return "unknown";
    }
  }

  inline std::string getString(StencilFaceFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::StencilFaceFlagBits::eFront) result += "Front | ";
    if (value & vk::StencilFaceFlagBits::eBack) result += "Back | ";
    if (value & vk::StencilFaceFlagBits::eVkStencilFrontAndBack) result += "VkStencilFrontAndBack | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(DescriptorPoolCreateFlagBits value)
  {
    switch (value)
    {
    case DescriptorPoolCreateFlagBits::eFreeDescriptorSet: return "FreeDescriptorSet";
    default: return "unknown";
    }
  }

  inline std::string getString(DescriptorPoolCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet) result += "FreeDescriptorSet | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(DependencyFlagBits value)
  {
    switch (value)
    {
    case DependencyFlagBits::eByRegion: return "ByRegion";
    default: return "unknown";
    }
  }

  inline std::string getString(DependencyFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DependencyFlagBits::eByRegion) result += "ByRegion | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(PresentModeKHR value)
  {
    switch (value)
    {
    case PresentModeKHR::eImmediateKHR: return "ImmediateKHR";
    case PresentModeKHR::eMailboxKHR: return "MailboxKHR";
    case PresentModeKHR::eFifoKHR: return "FifoKHR";
    case PresentModeKHR::eFifoRelaxedKHR: return "FifoRelaxedKHR";
    default: return "unknown";
    }
  }

  inline std::string getString(ColorSpaceKHR value)
  {
    switch (value)
    {
    case ColorSpaceKHR::eVkColorspaceSrgbNonlinearKHR: return "VkColorspaceSrgbNonlinearKHR";
    default: return "unknown";
    }
  }

  inline std::string getString(DisplayPlaneAlphaFlagBitsKHR value)
  {
    switch (value)
    {
    case DisplayPlaneAlphaFlagBitsKHR::eOpaque: return "Opaque";
    case DisplayPlaneAlphaFlagBitsKHR::eGlobal: return "Global";
    case DisplayPlaneAlphaFlagBitsKHR::ePerPixel: return "PerPixel";
    case DisplayPlaneAlphaFlagBitsKHR::ePerPixelPremultiplied: return "PerPixelPremultiplied";
    default: return "unknown";
    }
  }

  inline std::string getString(DisplayPlaneAlphaFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::eOpaque) result += "Opaque | ";
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::eGlobal) result += "Global | ";
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::ePerPixel) result += "PerPixel | ";
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::ePerPixelPremultiplied) result += "PerPixelPremultiplied | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(CompositeAlphaFlagBitsKHR value)
  {
    switch (value)
    {
    case CompositeAlphaFlagBitsKHR::eOpaque: return "Opaque";
    case CompositeAlphaFlagBitsKHR::ePreMultiplied: return "PreMultiplied";
    case CompositeAlphaFlagBitsKHR::ePostMultiplied: return "PostMultiplied";
    case CompositeAlphaFlagBitsKHR::eInherit: return "Inherit";
    default: return "unknown";
    }
  }

  inline std::string getString(CompositeAlphaFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CompositeAlphaFlagBitsKHR::eOpaque) result += "Opaque | ";
    if (value & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) result += "PreMultiplied | ";
    if (value & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) result += "PostMultiplied | ";
    if (value & vk::CompositeAlphaFlagBitsKHR::eInherit) result += "Inherit | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(SurfaceTransformFlagBitsKHR value)
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
    default: return "unknown";
    }
  }

  inline std::string getString(SurfaceTransformFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SurfaceTransformFlagBitsKHR::eIdentity) result += "Identity | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::eRotate90) result += "Rotate90 | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::eRotate180) result += "Rotate180 | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::eRotate270) result += "Rotate270 | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror) result += "HorizontalMirror | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90) result += "HorizontalMirrorRotate90 | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180) result += "HorizontalMirrorRotate180 | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270) result += "HorizontalMirrorRotate270 | ";
    if (value & vk::SurfaceTransformFlagBitsKHR::eInherit) result += "Inherit | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(DebugReportFlagBitsEXT value)
  {
    switch (value)
    {
    case DebugReportFlagBitsEXT::eInformation: return "Information";
    case DebugReportFlagBitsEXT::eWarning: return "Warning";
    case DebugReportFlagBitsEXT::ePerformanceWarning: return "PerformanceWarning";
    case DebugReportFlagBitsEXT::eError: return "Error";
    case DebugReportFlagBitsEXT::eDebug: return "Debug";
    default: return "unknown";
    }
  }

  inline std::string getString(DebugReportFlagsEXT value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DebugReportFlagBitsEXT::eInformation) result += "Information | ";
    if (value & vk::DebugReportFlagBitsEXT::eWarning) result += "Warning | ";
    if (value & vk::DebugReportFlagBitsEXT::ePerformanceWarning) result += "PerformanceWarning | ";
    if (value & vk::DebugReportFlagBitsEXT::eError) result += "Error | ";
    if (value & vk::DebugReportFlagBitsEXT::eDebug) result += "Debug | ";
    return result.substr(0, result.size() - 3);
  }

  inline std::string getString(DebugReportObjectTypeEXT value)
  {
    switch (value)
    {
    case DebugReportObjectTypeEXT::eUnknownEXT: return "UnknownEXT";
    case DebugReportObjectTypeEXT::eInstanceEXT: return "InstanceEXT";
    case DebugReportObjectTypeEXT::ePhysicalDeviceEXT: return "PhysicalDeviceEXT";
    case DebugReportObjectTypeEXT::eDeviceEXT: return "DeviceEXT";
    case DebugReportObjectTypeEXT::eQueueEXT: return "QueueEXT";
    case DebugReportObjectTypeEXT::eSemaphoreEXT: return "SemaphoreEXT";
    case DebugReportObjectTypeEXT::eCommandBufferEXT: return "CommandBufferEXT";
    case DebugReportObjectTypeEXT::eFenceEXT: return "FenceEXT";
    case DebugReportObjectTypeEXT::eDeviceMemoryEXT: return "DeviceMemoryEXT";
    case DebugReportObjectTypeEXT::eBufferEXT: return "BufferEXT";
    case DebugReportObjectTypeEXT::eImageEXT: return "ImageEXT";
    case DebugReportObjectTypeEXT::eEventEXT: return "EventEXT";
    case DebugReportObjectTypeEXT::eQueryPoolEXT: return "QueryPoolEXT";
    case DebugReportObjectTypeEXT::eBufferViewEXT: return "BufferViewEXT";
    case DebugReportObjectTypeEXT::eImageViewEXT: return "ImageViewEXT";
    case DebugReportObjectTypeEXT::eShaderModuleEXT: return "ShaderModuleEXT";
    case DebugReportObjectTypeEXT::ePipelineCacheEXT: return "PipelineCacheEXT";
    case DebugReportObjectTypeEXT::ePipelineLayoutEXT: return "PipelineLayoutEXT";
    case DebugReportObjectTypeEXT::eRenderPassEXT: return "RenderPassEXT";
    case DebugReportObjectTypeEXT::ePipelineEXT: return "PipelineEXT";
    case DebugReportObjectTypeEXT::eDescriptorSetLayoutEXT: return "DescriptorSetLayoutEXT";
    case DebugReportObjectTypeEXT::eSamplerEXT: return "SamplerEXT";
    case DebugReportObjectTypeEXT::eDescriptorPoolEXT: return "DescriptorPoolEXT";
    case DebugReportObjectTypeEXT::eDescriptorSetEXT: return "DescriptorSetEXT";
    case DebugReportObjectTypeEXT::eFramebufferEXT: return "FramebufferEXT";
    case DebugReportObjectTypeEXT::eCommandPoolEXT: return "CommandPoolEXT";
    case DebugReportObjectTypeEXT::eSurfaceKhrEXT: return "SurfaceKhrEXT";
    case DebugReportObjectTypeEXT::eSwapchainKhrEXT: return "SwapchainKhrEXT";
    case DebugReportObjectTypeEXT::eDebugReportEXT: return "DebugReportEXT";
    default: return "unknown";
    }
  }

  inline std::string getString(DebugReportErrorEXT value)
  {
    switch (value)
    {
    case DebugReportErrorEXT::eNoneEXT: return "NoneEXT";
    case DebugReportErrorEXT::eCallbackRefEXT: return "CallbackRefEXT";
    default: return "unknown";
    }
  }

} // namespace vk

#endif
