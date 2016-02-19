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


#pragma once
#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <vulkan/vulkan.h>
#ifdef VKCPP_ENHANCED_MODE
# include <vector>
#endif    // VKCPP_ENHANCED_MODE

static_assert( VK_MAKE_VERSION(1, 0, 3) == VK_API_VERSION, "Wrong VK_API_VERSION!" );

namespace vk
{
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

  enum class SwapchainCreateFlagsKHBits
  {
  };

  typedef Flags<SwapchainCreateFlagsKHBits, VkSwapchainCreateFlagsKHR> SwapchainCreateFlagsKHR;

  inline SwapchainCreateFlagsKHR operator|( SwapchainCreateFlagsKHBits bit0, SwapchainCreateFlagsKHBits bit1 )
  {
    return SwapchainCreateFlagsKHR( bit0 ) | bit1;
  }

  enum class DisplayModeCreateFlagsKHBits
  {
  };

  typedef Flags<DisplayModeCreateFlagsKHBits, VkDisplayModeCreateFlagsKHR> DisplayModeCreateFlagsKHR;

  inline DisplayModeCreateFlagsKHR operator|( DisplayModeCreateFlagsKHBits bit0, DisplayModeCreateFlagsKHBits bit1 )
  {
    return DisplayModeCreateFlagsKHR( bit0 ) | bit1;
  }

  enum class DisplaySurfaceCreateFlagsKHBits
  {
  };

  typedef Flags<DisplaySurfaceCreateFlagsKHBits, VkDisplaySurfaceCreateFlagsKHR> DisplaySurfaceCreateFlagsKHR;

  inline DisplaySurfaceCreateFlagsKHR operator|( DisplaySurfaceCreateFlagsKHBits bit0, DisplaySurfaceCreateFlagsKHBits bit1 )
  {
    return DisplaySurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

  enum class AndroidSurfaceCreateFlagsKHBits
  {
  };

  enum class MirSurfaceCreateFlagsKHBits
  {
  };

  enum class WaylandSurfaceCreateFlagsKHBits
  {
  };

  enum class Win32SurfaceCreateFlagsKHBits
  {
  };

  enum class XlibSurfaceCreateFlagsKHBits
  {
  };

  enum class XcbSurfaceCreateFlagsKHBits
  {
  };

  typedef VkInstance Instance;
  typedef VkPhysicalDevice PhysicalDevice;
  typedef VkDevice Device;
  typedef VkQueue Queue;
  typedef VkCommandBuffer CommandBuffer;
  typedef VkDeviceMemory DeviceMemory;
  typedef VkCommandPool CommandPool;
  typedef VkBuffer Buffer;
  typedef VkBufferView BufferView;
  typedef VkImage Image;
  typedef VkImageView ImageView;
  typedef VkShaderModule ShaderModule;
  typedef VkPipeline Pipeline;
  typedef VkPipelineLayout PipelineLayout;
  typedef VkSampler Sampler;
  typedef VkDescriptorSet DescriptorSet;
  typedef VkDescriptorSetLayout DescriptorSetLayout;
  typedef VkDescriptorPool DescriptorPool;
  typedef VkFence Fence;
  typedef VkSemaphore Semaphore;
  typedef VkEvent Event;
  typedef VkQueryPool QueryPool;
  typedef VkFramebuffer Framebuffer;
  typedef VkRenderPass RenderPass;
  typedef VkPipelineCache PipelineCache;
  typedef VkDisplayKHR DisplayKHR;
  typedef VkDisplayModeKHR DisplayModeKHR;
  typedef VkSurfaceKHR SurfaceKHR;
  typedef VkSwapchainKHR SwapchainKHR;
  typedef VkDebugReportCallbackEXT DebugReportCallbackEXT;
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

    const int32_t& x() const
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

    Offset2D& y( int32_t y )
    {
      m_offset2D.y = y;
      return *this;
    }

    operator const VkOffset2D&() const
    {
      return m_offset2D;
    }

  private:
    VkOffset2D m_offset2D;
  };

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

    const int32_t& x() const
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

    Offset3D& y( int32_t y )
    {
      m_offset3D.y = y;
      return *this;
    }

    const int32_t& z() const
    {
      return m_offset3D.z;
    }

    Offset3D& z( int32_t z )
    {
      m_offset3D.z = z;
      return *this;
    }

    operator const VkOffset3D&() const
    {
      return m_offset3D;
    }

  private:
    VkOffset3D m_offset3D;
  };

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

    const uint32_t& width() const
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

    Extent2D& height( uint32_t height )
    {
      m_extent2D.height = height;
      return *this;
    }

    operator const VkExtent2D&() const
    {
      return m_extent2D;
    }

  private:
    VkExtent2D m_extent2D;
  };

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

    const uint32_t& width() const
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

    Extent3D& height( uint32_t height )
    {
      m_extent3D.height = height;
      return *this;
    }

    const uint32_t& depth() const
    {
      return m_extent3D.depth;
    }

    Extent3D& depth( uint32_t depth )
    {
      m_extent3D.depth = depth;
      return *this;
    }

    operator const VkExtent3D&() const
    {
      return m_extent3D;
    }

  private:
    VkExtent3D m_extent3D;
  };

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

    const float& x() const
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

    Viewport& y( float y )
    {
      m_viewport.y = y;
      return *this;
    }

    const float& width() const
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

    Viewport& height( float height )
    {
      m_viewport.height = height;
      return *this;
    }

    const float& minDepth() const
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

    Viewport& maxDepth( float maxDepth )
    {
      m_viewport.maxDepth = maxDepth;
      return *this;
    }

    operator const VkViewport&() const
    {
      return m_viewport;
    }

  private:
    VkViewport m_viewport;
  };

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

    const Offset2D& offset() const
    {
      return reinterpret_cast<const Offset2D&>( m_rect2D.offset );
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

    Rect2D& extent( Extent2D extent )
    {
      m_rect2D.extent = static_cast<VkExtent2D>( extent );
      return *this;
    }

    operator const VkRect2D&() const
    {
      return m_rect2D;
    }

  private:
    VkRect2D m_rect2D;
  };

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

    const Rect2D& rect() const
    {
      return reinterpret_cast<const Rect2D&>( m_clearRect.rect );
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

    ClearRect& baseArrayLayer( uint32_t baseArrayLayer )
    {
      m_clearRect.baseArrayLayer = baseArrayLayer;
      return *this;
    }

    const uint32_t& layerCount() const
    {
      return m_clearRect.layerCount;
    }

    ClearRect& layerCount( uint32_t layerCount )
    {
      m_clearRect.layerCount = layerCount;
      return *this;
    }

    operator const VkClearRect&() const
    {
      return m_clearRect;
    }

  private:
    VkClearRect m_clearRect;
  };

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

    operator const VkExtensionProperties&() const
    {
      return m_extensionProperties;
    }

  private:
    VkExtensionProperties m_extensionProperties;
  };

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

    operator const VkLayerProperties&() const
    {
      return m_layerProperties;
    }

  private:
    VkLayerProperties m_layerProperties;
  };

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

    const void* pUserData() const
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

    AllocationCallbacks& pfnAllocation( PFN_vkAllocationFunction pfnAllocation )
    {
      m_allocationCallbacks.pfnAllocation = pfnAllocation;
      return *this;
    }

    const PFN_vkReallocationFunction& pfnReallocation() const
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

    AllocationCallbacks& pfnFree( PFN_vkFreeFunction pfnFree )
    {
      m_allocationCallbacks.pfnFree = pfnFree;
      return *this;
    }

    const PFN_vkInternalAllocationNotification& pfnInternalAllocation() const
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

    AllocationCallbacks& pfnInternalFree( PFN_vkInternalFreeNotification pfnInternalFree )
    {
      m_allocationCallbacks.pfnInternalFree = pfnInternalFree;
      return *this;
    }

    operator const VkAllocationCallbacks&() const
    {
      return m_allocationCallbacks;
    }

  private:
    VkAllocationCallbacks m_allocationCallbacks;
  };

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

    operator const VkMemoryRequirements&() const
    {
      return m_memoryRequirements;
    }

  private:
    VkMemoryRequirements m_memoryRequirements;
  };

  class DescriptorBufferInfo
  {
  public:
    DescriptorBufferInfo()
      : DescriptorBufferInfo( Buffer(), 0, 0 )
    {}

    DescriptorBufferInfo( Buffer buffer, DeviceSize offset, DeviceSize range)
    {
      m_descriptorBufferInfo.buffer = buffer;
      m_descriptorBufferInfo.offset = offset;
      m_descriptorBufferInfo.range = range;
    }

    const Buffer& buffer() const
    {
      return m_descriptorBufferInfo.buffer;
    }

    DescriptorBufferInfo& buffer( Buffer buffer )
    {
      m_descriptorBufferInfo.buffer = buffer;
      return *this;
    }

    const DeviceSize& offset() const
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

    DescriptorBufferInfo& range( DeviceSize range )
    {
      m_descriptorBufferInfo.range = range;
      return *this;
    }

    operator const VkDescriptorBufferInfo&() const
    {
      return m_descriptorBufferInfo;
    }

  private:
    VkDescriptorBufferInfo m_descriptorBufferInfo;
  };

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

    const DeviceSize& offset() const
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

    SubresourceLayout& size( DeviceSize size )
    {
      m_subresourceLayout.size = size;
      return *this;
    }

    const DeviceSize& rowPitch() const
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

    SubresourceLayout& arrayPitch( DeviceSize arrayPitch )
    {
      m_subresourceLayout.arrayPitch = arrayPitch;
      return *this;
    }

    const DeviceSize& depthPitch() const
    {
      return m_subresourceLayout.depthPitch;
    }

    SubresourceLayout& depthPitch( DeviceSize depthPitch )
    {
      m_subresourceLayout.depthPitch = depthPitch;
      return *this;
    }

    operator const VkSubresourceLayout&() const
    {
      return m_subresourceLayout;
    }

  private:
    VkSubresourceLayout m_subresourceLayout;
  };

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

    const DeviceSize& srcOffset() const
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

    BufferCopy& dstOffset( DeviceSize dstOffset )
    {
      m_bufferCopy.dstOffset = dstOffset;
      return *this;
    }

    const DeviceSize& size() const
    {
      return m_bufferCopy.size;
    }

    BufferCopy& size( DeviceSize size )
    {
      m_bufferCopy.size = size;
      return *this;
    }

    operator const VkBufferCopy&() const
    {
      return m_bufferCopy;
    }

  private:
    VkBufferCopy m_bufferCopy;
  };

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

    const uint32_t& constantID() const
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

    SpecializationMapEntry& offset( uint32_t offset )
    {
      m_specializationMapEntry.offset = offset;
      return *this;
    }

    const size_t& size() const
    {
      return m_specializationMapEntry.size;
    }

    SpecializationMapEntry& size( size_t size )
    {
      m_specializationMapEntry.size = size;
      return *this;
    }

    operator const VkSpecializationMapEntry&() const
    {
      return m_specializationMapEntry;
    }

  private:
    VkSpecializationMapEntry m_specializationMapEntry;
  };

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

    const uint32_t& mapEntryCount() const
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

    SpecializationInfo& pMapEntries( const SpecializationMapEntry* pMapEntries )
    {
      m_specializationInfo.pMapEntries = reinterpret_cast<const VkSpecializationMapEntry*>( pMapEntries );
      return *this;
    }

    const size_t& dataSize() const
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

    SpecializationInfo& pData( const void* pData )
    {
      m_specializationInfo.pData = pData;
      return *this;
    }

    operator const VkSpecializationInfo&() const
    {
      return m_specializationInfo;
    }

  private:
    VkSpecializationInfo m_specializationInfo;
  };

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

    const float& depth() const
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

    ClearDepthStencilValue& stencil( uint32_t stencil )
    {
      m_clearDepthStencilValue.stencil = stencil;
      return *this;
    }

    operator const VkClearDepthStencilValue&() const
    {
      return m_clearDepthStencilValue;
    }

  private:
    VkClearDepthStencilValue m_clearDepthStencilValue;
  };

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

    const Bool32& robustBufferAccess() const
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

    PhysicalDeviceFeatures& fullDrawIndexUint32( Bool32 fullDrawIndexUint32 )
    {
      m_physicalDeviceFeatures.fullDrawIndexUint32 = fullDrawIndexUint32;
      return *this;
    }

    const Bool32& imageCubeArray() const
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

    PhysicalDeviceFeatures& independentBlend( Bool32 independentBlend )
    {
      m_physicalDeviceFeatures.independentBlend = independentBlend;
      return *this;
    }

    const Bool32& geometryShader() const
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

    PhysicalDeviceFeatures& tessellationShader( Bool32 tessellationShader )
    {
      m_physicalDeviceFeatures.tessellationShader = tessellationShader;
      return *this;
    }

    const Bool32& sampleRateShading() const
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

    PhysicalDeviceFeatures& dualSrcBlend( Bool32 dualSrcBlend )
    {
      m_physicalDeviceFeatures.dualSrcBlend = dualSrcBlend;
      return *this;
    }

    const Bool32& logicOp() const
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

    PhysicalDeviceFeatures& multiDrawIndirect( Bool32 multiDrawIndirect )
    {
      m_physicalDeviceFeatures.multiDrawIndirect = multiDrawIndirect;
      return *this;
    }

    const Bool32& drawIndirectFirstInstance() const
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

    PhysicalDeviceFeatures& depthClamp( Bool32 depthClamp )
    {
      m_physicalDeviceFeatures.depthClamp = depthClamp;
      return *this;
    }

    const Bool32& depthBiasClamp() const
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

    PhysicalDeviceFeatures& fillModeNonSolid( Bool32 fillModeNonSolid )
    {
      m_physicalDeviceFeatures.fillModeNonSolid = fillModeNonSolid;
      return *this;
    }

    const Bool32& depthBounds() const
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

    PhysicalDeviceFeatures& wideLines( Bool32 wideLines )
    {
      m_physicalDeviceFeatures.wideLines = wideLines;
      return *this;
    }

    const Bool32& largePoints() const
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

    PhysicalDeviceFeatures& alphaToOne( Bool32 alphaToOne )
    {
      m_physicalDeviceFeatures.alphaToOne = alphaToOne;
      return *this;
    }

    const Bool32& multiViewport() const
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

    PhysicalDeviceFeatures& samplerAnisotropy( Bool32 samplerAnisotropy )
    {
      m_physicalDeviceFeatures.samplerAnisotropy = samplerAnisotropy;
      return *this;
    }

    const Bool32& textureCompressionETC2() const
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

    PhysicalDeviceFeatures& textureCompressionASTC_LDR( Bool32 textureCompressionASTC_LDR )
    {
      m_physicalDeviceFeatures.textureCompressionASTC_LDR = textureCompressionASTC_LDR;
      return *this;
    }

    const Bool32& textureCompressionBC() const
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

    PhysicalDeviceFeatures& occlusionQueryPrecise( Bool32 occlusionQueryPrecise )
    {
      m_physicalDeviceFeatures.occlusionQueryPrecise = occlusionQueryPrecise;
      return *this;
    }

    const Bool32& pipelineStatisticsQuery() const
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

    PhysicalDeviceFeatures& vertexPipelineStoresAndAtomics( Bool32 vertexPipelineStoresAndAtomics )
    {
      m_physicalDeviceFeatures.vertexPipelineStoresAndAtomics = vertexPipelineStoresAndAtomics;
      return *this;
    }

    const Bool32& fragmentStoresAndAtomics() const
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

    PhysicalDeviceFeatures& shaderTessellationAndGeometryPointSize( Bool32 shaderTessellationAndGeometryPointSize )
    {
      m_physicalDeviceFeatures.shaderTessellationAndGeometryPointSize = shaderTessellationAndGeometryPointSize;
      return *this;
    }

    const Bool32& shaderImageGatherExtended() const
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

    PhysicalDeviceFeatures& shaderStorageImageExtendedFormats( Bool32 shaderStorageImageExtendedFormats )
    {
      m_physicalDeviceFeatures.shaderStorageImageExtendedFormats = shaderStorageImageExtendedFormats;
      return *this;
    }

    const Bool32& shaderStorageImageMultisample() const
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

    PhysicalDeviceFeatures& shaderStorageImageReadWithoutFormat( Bool32 shaderStorageImageReadWithoutFormat )
    {
      m_physicalDeviceFeatures.shaderStorageImageReadWithoutFormat = shaderStorageImageReadWithoutFormat;
      return *this;
    }

    const Bool32& shaderStorageImageWriteWithoutFormat() const
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

    PhysicalDeviceFeatures& shaderUniformBufferArrayDynamicIndexing( Bool32 shaderUniformBufferArrayDynamicIndexing )
    {
      m_physicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing = shaderUniformBufferArrayDynamicIndexing;
      return *this;
    }

    const Bool32& shaderSampledImageArrayDynamicIndexing() const
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

    PhysicalDeviceFeatures& shaderStorageBufferArrayDynamicIndexing( Bool32 shaderStorageBufferArrayDynamicIndexing )
    {
      m_physicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing = shaderStorageBufferArrayDynamicIndexing;
      return *this;
    }

    const Bool32& shaderStorageImageArrayDynamicIndexing() const
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

    PhysicalDeviceFeatures& shaderClipDistance( Bool32 shaderClipDistance )
    {
      m_physicalDeviceFeatures.shaderClipDistance = shaderClipDistance;
      return *this;
    }

    const Bool32& shaderCullDistance() const
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

    PhysicalDeviceFeatures& shaderFloat64( Bool32 shaderFloat64 )
    {
      m_physicalDeviceFeatures.shaderFloat64 = shaderFloat64;
      return *this;
    }

    const Bool32& shaderInt64() const
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

    PhysicalDeviceFeatures& shaderInt16( Bool32 shaderInt16 )
    {
      m_physicalDeviceFeatures.shaderInt16 = shaderInt16;
      return *this;
    }

    const Bool32& shaderResourceResidency() const
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

    PhysicalDeviceFeatures& shaderResourceMinLod( Bool32 shaderResourceMinLod )
    {
      m_physicalDeviceFeatures.shaderResourceMinLod = shaderResourceMinLod;
      return *this;
    }

    const Bool32& sparseBinding() const
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

    PhysicalDeviceFeatures& sparseResidencyBuffer( Bool32 sparseResidencyBuffer )
    {
      m_physicalDeviceFeatures.sparseResidencyBuffer = sparseResidencyBuffer;
      return *this;
    }

    const Bool32& sparseResidencyImage2D() const
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

    PhysicalDeviceFeatures& sparseResidencyImage3D( Bool32 sparseResidencyImage3D )
    {
      m_physicalDeviceFeatures.sparseResidencyImage3D = sparseResidencyImage3D;
      return *this;
    }

    const Bool32& sparseResidency2Samples() const
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

    PhysicalDeviceFeatures& sparseResidency4Samples( Bool32 sparseResidency4Samples )
    {
      m_physicalDeviceFeatures.sparseResidency4Samples = sparseResidency4Samples;
      return *this;
    }

    const Bool32& sparseResidency8Samples() const
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

    PhysicalDeviceFeatures& sparseResidency16Samples( Bool32 sparseResidency16Samples )
    {
      m_physicalDeviceFeatures.sparseResidency16Samples = sparseResidency16Samples;
      return *this;
    }

    const Bool32& sparseResidencyAliased() const
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

    PhysicalDeviceFeatures& variableMultisampleRate( Bool32 variableMultisampleRate )
    {
      m_physicalDeviceFeatures.variableMultisampleRate = variableMultisampleRate;
      return *this;
    }

    const Bool32& inheritedQueries() const
    {
      return m_physicalDeviceFeatures.inheritedQueries;
    }

    PhysicalDeviceFeatures& inheritedQueries( Bool32 inheritedQueries )
    {
      m_physicalDeviceFeatures.inheritedQueries = inheritedQueries;
      return *this;
    }

    operator const VkPhysicalDeviceFeatures&() const
    {
      return m_physicalDeviceFeatures;
    }

  private:
    VkPhysicalDeviceFeatures m_physicalDeviceFeatures;
  };

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

    operator const VkPhysicalDeviceSparseProperties&() const
    {
      return m_physicalDeviceSparseProperties;
    }

  private:
    VkPhysicalDeviceSparseProperties m_physicalDeviceSparseProperties;
  };

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

    const uint32_t& vertexCount() const
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

    DrawIndirectCommand& instanceCount( uint32_t instanceCount )
    {
      m_drawIndirectCommand.instanceCount = instanceCount;
      return *this;
    }

    const uint32_t& firstVertex() const
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

    DrawIndirectCommand& firstInstance( uint32_t firstInstance )
    {
      m_drawIndirectCommand.firstInstance = firstInstance;
      return *this;
    }

    operator const VkDrawIndirectCommand&() const
    {
      return m_drawIndirectCommand;
    }

  private:
    VkDrawIndirectCommand m_drawIndirectCommand;
  };

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

    const uint32_t& indexCount() const
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

    DrawIndexedIndirectCommand& instanceCount( uint32_t instanceCount )
    {
      m_drawIndexedIndirectCommand.instanceCount = instanceCount;
      return *this;
    }

    const uint32_t& firstIndex() const
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

    DrawIndexedIndirectCommand& vertexOffset( int32_t vertexOffset )
    {
      m_drawIndexedIndirectCommand.vertexOffset = vertexOffset;
      return *this;
    }

    const uint32_t& firstInstance() const
    {
      return m_drawIndexedIndirectCommand.firstInstance;
    }

    DrawIndexedIndirectCommand& firstInstance( uint32_t firstInstance )
    {
      m_drawIndexedIndirectCommand.firstInstance = firstInstance;
      return *this;
    }

    operator const VkDrawIndexedIndirectCommand&() const
    {
      return m_drawIndexedIndirectCommand;
    }

  private:
    VkDrawIndexedIndirectCommand m_drawIndexedIndirectCommand;
  };

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

    const uint32_t& x() const
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

    DispatchIndirectCommand& y( uint32_t y )
    {
      m_dispatchIndirectCommand.y = y;
      return *this;
    }

    const uint32_t& z() const
    {
      return m_dispatchIndirectCommand.z;
    }

    DispatchIndirectCommand& z( uint32_t z )
    {
      m_dispatchIndirectCommand.z = z;
      return *this;
    }

    operator const VkDispatchIndirectCommand&() const
    {
      return m_dispatchIndirectCommand;
    }

  private:
    VkDispatchIndirectCommand m_dispatchIndirectCommand;
  };

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
    ePresentSrcKhr = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
  };

  class DescriptorImageInfo
  {
  public:
    DescriptorImageInfo()
      : DescriptorImageInfo( Sampler(), ImageView(), ImageLayout::eUndefined )
    {}

    DescriptorImageInfo( Sampler sampler, ImageView imageView, ImageLayout imageLayout)
    {
      m_descriptorImageInfo.sampler = sampler;
      m_descriptorImageInfo.imageView = imageView;
      m_descriptorImageInfo.imageLayout = static_cast<VkImageLayout>( imageLayout );
    }

    const Sampler& sampler() const
    {
      return m_descriptorImageInfo.sampler;
    }

    DescriptorImageInfo& sampler( Sampler sampler )
    {
      m_descriptorImageInfo.sampler = sampler;
      return *this;
    }

    const ImageView& imageView() const
    {
      return m_descriptorImageInfo.imageView;
    }

    DescriptorImageInfo& imageView( ImageView imageView )
    {
      m_descriptorImageInfo.imageView = imageView;
      return *this;
    }

    const ImageLayout& imageLayout() const
    {
      return reinterpret_cast<const ImageLayout&>( m_descriptorImageInfo.imageLayout );
    }

    DescriptorImageInfo& imageLayout( ImageLayout imageLayout )
    {
      m_descriptorImageInfo.imageLayout = static_cast<VkImageLayout>( imageLayout );
      return *this;
    }

    operator const VkDescriptorImageInfo&() const
    {
      return m_descriptorImageInfo;
    }

  private:
    VkDescriptorImageInfo m_descriptorImageInfo;
  };

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

    const uint32_t& attachment() const
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

    AttachmentReference& layout( ImageLayout layout )
    {
      m_attachmentReference.layout = static_cast<VkImageLayout>( layout );
      return *this;
    }

    operator const VkAttachmentReference&() const
    {
      return m_attachmentReference;
    }

  private:
    VkAttachmentReference m_attachmentReference;
  };

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

    const ComponentSwizzle& r() const
    {
      return reinterpret_cast<const ComponentSwizzle&>( m_componentMapping.r );
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

    ComponentMapping& g( ComponentSwizzle g )
    {
      m_componentMapping.g = static_cast<VkComponentSwizzle>( g );
      return *this;
    }

    const ComponentSwizzle& b() const
    {
      return reinterpret_cast<const ComponentSwizzle&>( m_componentMapping.b );
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

    ComponentMapping& a( ComponentSwizzle a )
    {
      m_componentMapping.a = static_cast<VkComponentSwizzle>( a );
      return *this;
    }

    operator const VkComponentMapping&() const
    {
      return m_componentMapping;
    }

  private:
    VkComponentMapping m_componentMapping;
  };

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

    const DescriptorType& type() const
    {
      return reinterpret_cast<const DescriptorType&>( m_descriptorPoolSize.type );
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

    DescriptorPoolSize& descriptorCount( uint32_t descriptorCount )
    {
      m_descriptorPoolSize.descriptorCount = descriptorCount;
      return *this;
    }

    operator const VkDescriptorPoolSize&() const
    {
      return m_descriptorPoolSize;
    }

  private:
    VkDescriptorPoolSize m_descriptorPoolSize;
  };

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

    const SubpassDescriptionFlags& flags() const
    {
      return reinterpret_cast<const SubpassDescriptionFlags&>( m_subpassDescription.flags );
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

    SubpassDescription& pipelineBindPoint( PipelineBindPoint pipelineBindPoint )
    {
      m_subpassDescription.pipelineBindPoint = static_cast<VkPipelineBindPoint>( pipelineBindPoint );
      return *this;
    }

    const uint32_t& inputAttachmentCount() const
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

    SubpassDescription& pInputAttachments( const AttachmentReference* pInputAttachments )
    {
      m_subpassDescription.pInputAttachments = reinterpret_cast<const VkAttachmentReference*>( pInputAttachments );
      return *this;
    }

    const uint32_t& colorAttachmentCount() const
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

    SubpassDescription& pColorAttachments( const AttachmentReference* pColorAttachments )
    {
      m_subpassDescription.pColorAttachments = reinterpret_cast<const VkAttachmentReference*>( pColorAttachments );
      return *this;
    }

    const AttachmentReference* pResolveAttachments() const
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

    SubpassDescription& pDepthStencilAttachment( const AttachmentReference* pDepthStencilAttachment )
    {
      m_subpassDescription.pDepthStencilAttachment = reinterpret_cast<const VkAttachmentReference*>( pDepthStencilAttachment );
      return *this;
    }

    const uint32_t& preserveAttachmentCount() const
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

    SubpassDescription& pPreserveAttachments( const uint32_t* pPreserveAttachments )
    {
      m_subpassDescription.pPreserveAttachments = pPreserveAttachments;
      return *this;
    }

    operator const VkSubpassDescription&() const
    {
      return m_subpassDescription;
    }

  private:
    VkSubpassDescription m_subpassDescription;
  };

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

    const StencilOp& failOp() const
    {
      return reinterpret_cast<const StencilOp&>( m_stencilOpState.failOp );
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

    StencilOpState& passOp( StencilOp passOp )
    {
      m_stencilOpState.passOp = static_cast<VkStencilOp>( passOp );
      return *this;
    }

    const StencilOp& depthFailOp() const
    {
      return reinterpret_cast<const StencilOp&>( m_stencilOpState.depthFailOp );
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

    StencilOpState& compareOp( CompareOp compareOp )
    {
      m_stencilOpState.compareOp = static_cast<VkCompareOp>( compareOp );
      return *this;
    }

    const uint32_t& compareMask() const
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

    StencilOpState& writeMask( uint32_t writeMask )
    {
      m_stencilOpState.writeMask = writeMask;
      return *this;
    }

    const uint32_t& reference() const
    {
      return m_stencilOpState.reference;
    }

    StencilOpState& reference( uint32_t reference )
    {
      m_stencilOpState.reference = reference;
      return *this;
    }

    operator const VkStencilOpState&() const
    {
      return m_stencilOpState;
    }

  private:
    VkStencilOpState m_stencilOpState;
  };

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

    const uint32_t& binding() const
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

    VertexInputBindingDescription& stride( uint32_t stride )
    {
      m_vertexInputBindingDescription.stride = stride;
      return *this;
    }

    const VertexInputRate& inputRate() const
    {
      return reinterpret_cast<const VertexInputRate&>( m_vertexInputBindingDescription.inputRate );
    }

    VertexInputBindingDescription& inputRate( VertexInputRate inputRate )
    {
      m_vertexInputBindingDescription.inputRate = static_cast<VkVertexInputRate>( inputRate );
      return *this;
    }

    operator const VkVertexInputBindingDescription&() const
    {
      return m_vertexInputBindingDescription;
    }

  private:
    VkVertexInputBindingDescription m_vertexInputBindingDescription;
  };

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

    const uint32_t& location() const
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

    VertexInputAttributeDescription& binding( uint32_t binding )
    {
      m_vertexInputAttributeDescription.binding = binding;
      return *this;
    }

    const Format& format() const
    {
      return reinterpret_cast<const Format&>( m_vertexInputAttributeDescription.format );
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

    VertexInputAttributeDescription& offset( uint32_t offset )
    {
      m_vertexInputAttributeDescription.offset = offset;
      return *this;
    }

    operator const VkVertexInputAttributeDescription&() const
    {
      return m_vertexInputAttributeDescription;
    }

  private:
    VkVertexInputAttributeDescription m_vertexInputAttributeDescription;
  };

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
    eSwapchainCreateInfoKhr = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    ePresentInfoKhr = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    eDisplayModeCreateInfoKhr = VK_STRUCTURE_TYPE_DISPLAY_MODE_CREATE_INFO_KHR,
    eDisplaySurfaceCreateInfoKhr = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR,
    eDisplayPresentInfoKhr = VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR,
    eXlibSurfaceCreateInfoKhr = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
    eXcbSurfaceCreateInfoKhr = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
    eWaylandSurfaceCreateInfoKhr = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
    eMirSurfaceCreateInfoKhr = VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR,
    eAndroidSurfaceCreateInfoKhr = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
    eWin32SurfaceCreateInfoKhr = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
    eDebugReportCreateInfoExt = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT
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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_applicationInfo.sType );
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

    ApplicationInfo& pNext( const void* pNext )
    {
      m_applicationInfo.pNext = pNext;
      return *this;
    }

    const char* pApplicationName() const
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

    ApplicationInfo& applicationVersion( uint32_t applicationVersion )
    {
      m_applicationInfo.applicationVersion = applicationVersion;
      return *this;
    }

    const char* pEngineName() const
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

    ApplicationInfo& engineVersion( uint32_t engineVersion )
    {
      m_applicationInfo.engineVersion = engineVersion;
      return *this;
    }

    const uint32_t& apiVersion() const
    {
      return m_applicationInfo.apiVersion;
    }

    ApplicationInfo& apiVersion( uint32_t apiVersion )
    {
      m_applicationInfo.apiVersion = apiVersion;
      return *this;
    }

    operator const VkApplicationInfo&() const
    {
      return m_applicationInfo;
    }

  private:
    VkApplicationInfo m_applicationInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_deviceQueueCreateInfo.sType );
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

    DeviceQueueCreateInfo& pNext( const void* pNext )
    {
      m_deviceQueueCreateInfo.pNext = pNext;
      return *this;
    }

    const DeviceQueueCreateFlags& flags() const
    {
      return reinterpret_cast<const DeviceQueueCreateFlags&>( m_deviceQueueCreateInfo.flags );
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

    DeviceQueueCreateInfo& queueFamilyIndex( uint32_t queueFamilyIndex )
    {
      m_deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
      return *this;
    }

    const uint32_t& queueCount() const
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

    DeviceQueueCreateInfo& pQueuePriorities( const float* pQueuePriorities )
    {
      m_deviceQueueCreateInfo.pQueuePriorities = pQueuePriorities;
      return *this;
    }

    operator const VkDeviceQueueCreateInfo&() const
    {
      return m_deviceQueueCreateInfo;
    }

  private:
    VkDeviceQueueCreateInfo m_deviceQueueCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_deviceCreateInfo.sType );
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

    DeviceCreateInfo& pNext( const void* pNext )
    {
      m_deviceCreateInfo.pNext = pNext;
      return *this;
    }

    const DeviceCreateFlags& flags() const
    {
      return reinterpret_cast<const DeviceCreateFlags&>( m_deviceCreateInfo.flags );
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

    DeviceCreateInfo& queueCreateInfoCount( uint32_t queueCreateInfoCount )
    {
      m_deviceCreateInfo.queueCreateInfoCount = queueCreateInfoCount;
      return *this;
    }

    const DeviceQueueCreateInfo* pQueueCreateInfos() const
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

    DeviceCreateInfo& enabledLayerCount( uint32_t enabledLayerCount )
    {
      m_deviceCreateInfo.enabledLayerCount = enabledLayerCount;
      return *this;
    }

    const char* const* ppEnabledLayerNames() const
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

    DeviceCreateInfo& enabledExtensionCount( uint32_t enabledExtensionCount )
    {
      m_deviceCreateInfo.enabledExtensionCount = enabledExtensionCount;
      return *this;
    }

    const char* const* ppEnabledExtensionNames() const
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

    DeviceCreateInfo& pEnabledFeatures( const PhysicalDeviceFeatures* pEnabledFeatures )
    {
      m_deviceCreateInfo.pEnabledFeatures = reinterpret_cast<const VkPhysicalDeviceFeatures*>( pEnabledFeatures );
      return *this;
    }

    operator const VkDeviceCreateInfo&() const
    {
      return m_deviceCreateInfo;
    }

  private:
    VkDeviceCreateInfo m_deviceCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_instanceCreateInfo.sType );
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

    InstanceCreateInfo& pNext( const void* pNext )
    {
      m_instanceCreateInfo.pNext = pNext;
      return *this;
    }

    const InstanceCreateFlags& flags() const
    {
      return reinterpret_cast<const InstanceCreateFlags&>( m_instanceCreateInfo.flags );
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

    InstanceCreateInfo& pApplicationInfo( const ApplicationInfo* pApplicationInfo )
    {
      m_instanceCreateInfo.pApplicationInfo = reinterpret_cast<const VkApplicationInfo*>( pApplicationInfo );
      return *this;
    }

    const uint32_t& enabledLayerCount() const
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

    InstanceCreateInfo& ppEnabledLayerNames( const char* const* ppEnabledLayerNames )
    {
      m_instanceCreateInfo.ppEnabledLayerNames = ppEnabledLayerNames;
      return *this;
    }

    const uint32_t& enabledExtensionCount() const
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

    InstanceCreateInfo& ppEnabledExtensionNames( const char* const* ppEnabledExtensionNames )
    {
      m_instanceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames;
      return *this;
    }

    operator const VkInstanceCreateInfo&() const
    {
      return m_instanceCreateInfo;
    }

  private:
    VkInstanceCreateInfo m_instanceCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_memoryAllocateInfo.sType );
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

    MemoryAllocateInfo& pNext( const void* pNext )
    {
      m_memoryAllocateInfo.pNext = pNext;
      return *this;
    }

    const DeviceSize& allocationSize() const
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

    MemoryAllocateInfo& memoryTypeIndex( uint32_t memoryTypeIndex )
    {
      m_memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;
      return *this;
    }

    operator const VkMemoryAllocateInfo&() const
    {
      return m_memoryAllocateInfo;
    }

  private:
    VkMemoryAllocateInfo m_memoryAllocateInfo;
  };

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
      m_mappedMemoryRange.memory = memory;
      m_mappedMemoryRange.offset = offset;
      m_mappedMemoryRange.size = size;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_mappedMemoryRange.sType );
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

    MappedMemoryRange& pNext( const void* pNext )
    {
      m_mappedMemoryRange.pNext = pNext;
      return *this;
    }

    const DeviceMemory& memory() const
    {
      return m_mappedMemoryRange.memory;
    }

    MappedMemoryRange& memory( DeviceMemory memory )
    {
      m_mappedMemoryRange.memory = memory;
      return *this;
    }

    const DeviceSize& offset() const
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

    MappedMemoryRange& size( DeviceSize size )
    {
      m_mappedMemoryRange.size = size;
      return *this;
    }

    operator const VkMappedMemoryRange&() const
    {
      return m_mappedMemoryRange;
    }

  private:
    VkMappedMemoryRange m_mappedMemoryRange;
  };

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
      m_writeDescriptorSet.dstSet = dstSet;
      m_writeDescriptorSet.dstBinding = dstBinding;
      m_writeDescriptorSet.dstArrayElement = dstArrayElement;
      m_writeDescriptorSet.descriptorCount = descriptorCount;
      m_writeDescriptorSet.descriptorType = static_cast<VkDescriptorType>( descriptorType );
      m_writeDescriptorSet.pImageInfo = reinterpret_cast<const VkDescriptorImageInfo*>( pImageInfo );
      m_writeDescriptorSet.pBufferInfo = reinterpret_cast<const VkDescriptorBufferInfo*>( pBufferInfo );
      m_writeDescriptorSet.pTexelBufferView = pTexelBufferView;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_writeDescriptorSet.sType );
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

    WriteDescriptorSet& pNext( const void* pNext )
    {
      m_writeDescriptorSet.pNext = pNext;
      return *this;
    }

    const DescriptorSet& dstSet() const
    {
      return m_writeDescriptorSet.dstSet;
    }

    WriteDescriptorSet& dstSet( DescriptorSet dstSet )
    {
      m_writeDescriptorSet.dstSet = dstSet;
      return *this;
    }

    const uint32_t& dstBinding() const
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

    WriteDescriptorSet& dstArrayElement( uint32_t dstArrayElement )
    {
      m_writeDescriptorSet.dstArrayElement = dstArrayElement;
      return *this;
    }

    const uint32_t& descriptorCount() const
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

    WriteDescriptorSet& descriptorType( DescriptorType descriptorType )
    {
      m_writeDescriptorSet.descriptorType = static_cast<VkDescriptorType>( descriptorType );
      return *this;
    }

    const DescriptorImageInfo* pImageInfo() const
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

    WriteDescriptorSet& pBufferInfo( const DescriptorBufferInfo* pBufferInfo )
    {
      m_writeDescriptorSet.pBufferInfo = reinterpret_cast<const VkDescriptorBufferInfo*>( pBufferInfo );
      return *this;
    }

    const BufferView* pTexelBufferView() const
    {
      return reinterpret_cast<const BufferView*>( m_writeDescriptorSet.pTexelBufferView );
    }

    WriteDescriptorSet& pTexelBufferView( const BufferView* pTexelBufferView )
    {
      m_writeDescriptorSet.pTexelBufferView = pTexelBufferView;
      return *this;
    }

    operator const VkWriteDescriptorSet&() const
    {
      return m_writeDescriptorSet;
    }

  private:
    VkWriteDescriptorSet m_writeDescriptorSet;
  };

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
      m_copyDescriptorSet.srcSet = srcSet;
      m_copyDescriptorSet.srcBinding = srcBinding;
      m_copyDescriptorSet.srcArrayElement = srcArrayElement;
      m_copyDescriptorSet.dstSet = dstSet;
      m_copyDescriptorSet.dstBinding = dstBinding;
      m_copyDescriptorSet.dstArrayElement = dstArrayElement;
      m_copyDescriptorSet.descriptorCount = descriptorCount;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_copyDescriptorSet.sType );
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

    CopyDescriptorSet& pNext( const void* pNext )
    {
      m_copyDescriptorSet.pNext = pNext;
      return *this;
    }

    const DescriptorSet& srcSet() const
    {
      return m_copyDescriptorSet.srcSet;
    }

    CopyDescriptorSet& srcSet( DescriptorSet srcSet )
    {
      m_copyDescriptorSet.srcSet = srcSet;
      return *this;
    }

    const uint32_t& srcBinding() const
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

    CopyDescriptorSet& srcArrayElement( uint32_t srcArrayElement )
    {
      m_copyDescriptorSet.srcArrayElement = srcArrayElement;
      return *this;
    }

    const DescriptorSet& dstSet() const
    {
      return m_copyDescriptorSet.dstSet;
    }

    CopyDescriptorSet& dstSet( DescriptorSet dstSet )
    {
      m_copyDescriptorSet.dstSet = dstSet;
      return *this;
    }

    const uint32_t& dstBinding() const
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

    CopyDescriptorSet& dstArrayElement( uint32_t dstArrayElement )
    {
      m_copyDescriptorSet.dstArrayElement = dstArrayElement;
      return *this;
    }

    const uint32_t& descriptorCount() const
    {
      return m_copyDescriptorSet.descriptorCount;
    }

    CopyDescriptorSet& descriptorCount( uint32_t descriptorCount )
    {
      m_copyDescriptorSet.descriptorCount = descriptorCount;
      return *this;
    }

    operator const VkCopyDescriptorSet&() const
    {
      return m_copyDescriptorSet;
    }

  private:
    VkCopyDescriptorSet m_copyDescriptorSet;
  };

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
      m_bufferViewCreateInfo.buffer = buffer;
      m_bufferViewCreateInfo.format = static_cast<VkFormat>( format );
      m_bufferViewCreateInfo.offset = offset;
      m_bufferViewCreateInfo.range = range;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_bufferViewCreateInfo.sType );
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

    BufferViewCreateInfo& pNext( const void* pNext )
    {
      m_bufferViewCreateInfo.pNext = pNext;
      return *this;
    }

    const BufferViewCreateFlags& flags() const
    {
      return reinterpret_cast<const BufferViewCreateFlags&>( m_bufferViewCreateInfo.flags );
    }

    BufferViewCreateInfo& flags( BufferViewCreateFlags flags )
    {
      m_bufferViewCreateInfo.flags = static_cast<VkBufferViewCreateFlags>( flags );
      return *this;
    }

    const Buffer& buffer() const
    {
      return m_bufferViewCreateInfo.buffer;
    }

    BufferViewCreateInfo& buffer( Buffer buffer )
    {
      m_bufferViewCreateInfo.buffer = buffer;
      return *this;
    }

    const Format& format() const
    {
      return reinterpret_cast<const Format&>( m_bufferViewCreateInfo.format );
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

    BufferViewCreateInfo& offset( DeviceSize offset )
    {
      m_bufferViewCreateInfo.offset = offset;
      return *this;
    }

    const DeviceSize& range() const
    {
      return m_bufferViewCreateInfo.range;
    }

    BufferViewCreateInfo& range( DeviceSize range )
    {
      m_bufferViewCreateInfo.range = range;
      return *this;
    }

    operator const VkBufferViewCreateInfo&() const
    {
      return m_bufferViewCreateInfo;
    }

  private:
    VkBufferViewCreateInfo m_bufferViewCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_shaderModuleCreateInfo.sType );
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

    ShaderModuleCreateInfo& pNext( const void* pNext )
    {
      m_shaderModuleCreateInfo.pNext = pNext;
      return *this;
    }

    const ShaderModuleCreateFlags& flags() const
    {
      return reinterpret_cast<const ShaderModuleCreateFlags&>( m_shaderModuleCreateInfo.flags );
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

    ShaderModuleCreateInfo& codeSize( size_t codeSize )
    {
      m_shaderModuleCreateInfo.codeSize = codeSize;
      return *this;
    }

    const uint32_t* pCode() const
    {
      return reinterpret_cast<const uint32_t*>( m_shaderModuleCreateInfo.pCode );
    }

    ShaderModuleCreateInfo& pCode( const uint32_t* pCode )
    {
      m_shaderModuleCreateInfo.pCode = pCode;
      return *this;
    }

    operator const VkShaderModuleCreateInfo&() const
    {
      return m_shaderModuleCreateInfo;
    }

  private:
    VkShaderModuleCreateInfo m_shaderModuleCreateInfo;
  };

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
      m_descriptorSetAllocateInfo.descriptorPool = descriptorPool;
      m_descriptorSetAllocateInfo.descriptorSetCount = descriptorSetCount;
      m_descriptorSetAllocateInfo.pSetLayouts = pSetLayouts;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_descriptorSetAllocateInfo.sType );
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

    DescriptorSetAllocateInfo& pNext( const void* pNext )
    {
      m_descriptorSetAllocateInfo.pNext = pNext;
      return *this;
    }

    const DescriptorPool& descriptorPool() const
    {
      return m_descriptorSetAllocateInfo.descriptorPool;
    }

    DescriptorSetAllocateInfo& descriptorPool( DescriptorPool descriptorPool )
    {
      m_descriptorSetAllocateInfo.descriptorPool = descriptorPool;
      return *this;
    }

    const uint32_t& descriptorSetCount() const
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

    DescriptorSetAllocateInfo& pSetLayouts( const DescriptorSetLayout* pSetLayouts )
    {
      m_descriptorSetAllocateInfo.pSetLayouts = pSetLayouts;
      return *this;
    }

    operator const VkDescriptorSetAllocateInfo&() const
    {
      return m_descriptorSetAllocateInfo;
    }

  private:
    VkDescriptorSetAllocateInfo m_descriptorSetAllocateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineVertexInputStateCreateInfo.sType );
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

    PipelineVertexInputStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineVertexInputStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineVertexInputStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineVertexInputStateCreateFlags&>( m_pipelineVertexInputStateCreateInfo.flags );
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

    PipelineVertexInputStateCreateInfo& vertexBindingDescriptionCount( uint32_t vertexBindingDescriptionCount )
    {
      m_pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexBindingDescriptionCount;
      return *this;
    }

    const VertexInputBindingDescription* pVertexBindingDescriptions() const
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

    PipelineVertexInputStateCreateInfo& vertexAttributeDescriptionCount( uint32_t vertexAttributeDescriptionCount )
    {
      m_pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexAttributeDescriptionCount;
      return *this;
    }

    const VertexInputAttributeDescription* pVertexAttributeDescriptions() const
    {
      return reinterpret_cast<const VertexInputAttributeDescription*>( m_pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions );
    }

    PipelineVertexInputStateCreateInfo& pVertexAttributeDescriptions( const VertexInputAttributeDescription* pVertexAttributeDescriptions )
    {
      m_pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = reinterpret_cast<const VkVertexInputAttributeDescription*>( pVertexAttributeDescriptions );
      return *this;
    }

    operator const VkPipelineVertexInputStateCreateInfo&() const
    {
      return m_pipelineVertexInputStateCreateInfo;
    }

  private:
    VkPipelineVertexInputStateCreateInfo m_pipelineVertexInputStateCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineInputAssemblyStateCreateInfo.sType );
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

    PipelineInputAssemblyStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineInputAssemblyStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineInputAssemblyStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineInputAssemblyStateCreateFlags&>( m_pipelineInputAssemblyStateCreateInfo.flags );
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

    PipelineInputAssemblyStateCreateInfo& topology( PrimitiveTopology topology )
    {
      m_pipelineInputAssemblyStateCreateInfo.topology = static_cast<VkPrimitiveTopology>( topology );
      return *this;
    }

    const Bool32& primitiveRestartEnable() const
    {
      return m_pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable;
    }

    PipelineInputAssemblyStateCreateInfo& primitiveRestartEnable( Bool32 primitiveRestartEnable )
    {
      m_pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
      return *this;
    }

    operator const VkPipelineInputAssemblyStateCreateInfo&() const
    {
      return m_pipelineInputAssemblyStateCreateInfo;
    }

  private:
    VkPipelineInputAssemblyStateCreateInfo m_pipelineInputAssemblyStateCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineTessellationStateCreateInfo.sType );
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

    PipelineTessellationStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineTessellationStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineTessellationStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineTessellationStateCreateFlags&>( m_pipelineTessellationStateCreateInfo.flags );
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

    PipelineTessellationStateCreateInfo& patchControlPoints( uint32_t patchControlPoints )
    {
      m_pipelineTessellationStateCreateInfo.patchControlPoints = patchControlPoints;
      return *this;
    }

    operator const VkPipelineTessellationStateCreateInfo&() const
    {
      return m_pipelineTessellationStateCreateInfo;
    }

  private:
    VkPipelineTessellationStateCreateInfo m_pipelineTessellationStateCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineViewportStateCreateInfo.sType );
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

    PipelineViewportStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineViewportStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineViewportStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineViewportStateCreateFlags&>( m_pipelineViewportStateCreateInfo.flags );
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

    PipelineViewportStateCreateInfo& viewportCount( uint32_t viewportCount )
    {
      m_pipelineViewportStateCreateInfo.viewportCount = viewportCount;
      return *this;
    }

    const Viewport* pViewports() const
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

    PipelineViewportStateCreateInfo& scissorCount( uint32_t scissorCount )
    {
      m_pipelineViewportStateCreateInfo.scissorCount = scissorCount;
      return *this;
    }

    const Rect2D* pScissors() const
    {
      return reinterpret_cast<const Rect2D*>( m_pipelineViewportStateCreateInfo.pScissors );
    }

    PipelineViewportStateCreateInfo& pScissors( const Rect2D* pScissors )
    {
      m_pipelineViewportStateCreateInfo.pScissors = reinterpret_cast<const VkRect2D*>( pScissors );
      return *this;
    }

    operator const VkPipelineViewportStateCreateInfo&() const
    {
      return m_pipelineViewportStateCreateInfo;
    }

  private:
    VkPipelineViewportStateCreateInfo m_pipelineViewportStateCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineRasterizationStateCreateInfo.sType );
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

    PipelineRasterizationStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineRasterizationStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineRasterizationStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineRasterizationStateCreateFlags&>( m_pipelineRasterizationStateCreateInfo.flags );
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

    PipelineRasterizationStateCreateInfo& depthClampEnable( Bool32 depthClampEnable )
    {
      m_pipelineRasterizationStateCreateInfo.depthClampEnable = depthClampEnable;
      return *this;
    }

    const Bool32& rasterizerDiscardEnable() const
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

    PipelineRasterizationStateCreateInfo& polygonMode( PolygonMode polygonMode )
    {
      m_pipelineRasterizationStateCreateInfo.polygonMode = static_cast<VkPolygonMode>( polygonMode );
      return *this;
    }

    const CullModeFlags& cullMode() const
    {
      return reinterpret_cast<const CullModeFlags&>( m_pipelineRasterizationStateCreateInfo.cullMode );
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

    PipelineRasterizationStateCreateInfo& frontFace( FrontFace frontFace )
    {
      m_pipelineRasterizationStateCreateInfo.frontFace = static_cast<VkFrontFace>( frontFace );
      return *this;
    }

    const Bool32& depthBiasEnable() const
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

    PipelineRasterizationStateCreateInfo& depthBiasConstantFactor( float depthBiasConstantFactor )
    {
      m_pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = depthBiasConstantFactor;
      return *this;
    }

    const float& depthBiasClamp() const
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

    PipelineRasterizationStateCreateInfo& depthBiasSlopeFactor( float depthBiasSlopeFactor )
    {
      m_pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = depthBiasSlopeFactor;
      return *this;
    }

    const float& lineWidth() const
    {
      return m_pipelineRasterizationStateCreateInfo.lineWidth;
    }

    PipelineRasterizationStateCreateInfo& lineWidth( float lineWidth )
    {
      m_pipelineRasterizationStateCreateInfo.lineWidth = lineWidth;
      return *this;
    }

    operator const VkPipelineRasterizationStateCreateInfo&() const
    {
      return m_pipelineRasterizationStateCreateInfo;
    }

  private:
    VkPipelineRasterizationStateCreateInfo m_pipelineRasterizationStateCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineDepthStencilStateCreateInfo.sType );
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

    PipelineDepthStencilStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineDepthStencilStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineDepthStencilStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineDepthStencilStateCreateFlags&>( m_pipelineDepthStencilStateCreateInfo.flags );
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

    PipelineDepthStencilStateCreateInfo& depthTestEnable( Bool32 depthTestEnable )
    {
      m_pipelineDepthStencilStateCreateInfo.depthTestEnable = depthTestEnable;
      return *this;
    }

    const Bool32& depthWriteEnable() const
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

    PipelineDepthStencilStateCreateInfo& depthCompareOp( CompareOp depthCompareOp )
    {
      m_pipelineDepthStencilStateCreateInfo.depthCompareOp = static_cast<VkCompareOp>( depthCompareOp );
      return *this;
    }

    const Bool32& depthBoundsTestEnable() const
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

    PipelineDepthStencilStateCreateInfo& stencilTestEnable( Bool32 stencilTestEnable )
    {
      m_pipelineDepthStencilStateCreateInfo.stencilTestEnable = stencilTestEnable;
      return *this;
    }

    const StencilOpState& front() const
    {
      return reinterpret_cast<const StencilOpState&>( m_pipelineDepthStencilStateCreateInfo.front );
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

    PipelineDepthStencilStateCreateInfo& back( StencilOpState back )
    {
      m_pipelineDepthStencilStateCreateInfo.back = static_cast<VkStencilOpState>( back );
      return *this;
    }

    const float& minDepthBounds() const
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

    PipelineDepthStencilStateCreateInfo& maxDepthBounds( float maxDepthBounds )
    {
      m_pipelineDepthStencilStateCreateInfo.maxDepthBounds = maxDepthBounds;
      return *this;
    }

    operator const VkPipelineDepthStencilStateCreateInfo&() const
    {
      return m_pipelineDepthStencilStateCreateInfo;
    }

  private:
    VkPipelineDepthStencilStateCreateInfo m_pipelineDepthStencilStateCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineCacheCreateInfo.sType );
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

    PipelineCacheCreateInfo& pNext( const void* pNext )
    {
      m_pipelineCacheCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineCacheCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineCacheCreateFlags&>( m_pipelineCacheCreateInfo.flags );
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

    PipelineCacheCreateInfo& initialDataSize( size_t initialDataSize )
    {
      m_pipelineCacheCreateInfo.initialDataSize = initialDataSize;
      return *this;
    }

    const void* pInitialData() const
    {
      return reinterpret_cast<const void*>( m_pipelineCacheCreateInfo.pInitialData );
    }

    PipelineCacheCreateInfo& pInitialData( const void* pInitialData )
    {
      m_pipelineCacheCreateInfo.pInitialData = pInitialData;
      return *this;
    }

    operator const VkPipelineCacheCreateInfo&() const
    {
      return m_pipelineCacheCreateInfo;
    }

  private:
    VkPipelineCacheCreateInfo m_pipelineCacheCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_samplerCreateInfo.sType );
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

    SamplerCreateInfo& pNext( const void* pNext )
    {
      m_samplerCreateInfo.pNext = pNext;
      return *this;
    }

    const SamplerCreateFlags& flags() const
    {
      return reinterpret_cast<const SamplerCreateFlags&>( m_samplerCreateInfo.flags );
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

    SamplerCreateInfo& magFilter( Filter magFilter )
    {
      m_samplerCreateInfo.magFilter = static_cast<VkFilter>( magFilter );
      return *this;
    }

    const Filter& minFilter() const
    {
      return reinterpret_cast<const Filter&>( m_samplerCreateInfo.minFilter );
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

    SamplerCreateInfo& mipmapMode( SamplerMipmapMode mipmapMode )
    {
      m_samplerCreateInfo.mipmapMode = static_cast<VkSamplerMipmapMode>( mipmapMode );
      return *this;
    }

    const SamplerAddressMode& addressModeU() const
    {
      return reinterpret_cast<const SamplerAddressMode&>( m_samplerCreateInfo.addressModeU );
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

    SamplerCreateInfo& addressModeV( SamplerAddressMode addressModeV )
    {
      m_samplerCreateInfo.addressModeV = static_cast<VkSamplerAddressMode>( addressModeV );
      return *this;
    }

    const SamplerAddressMode& addressModeW() const
    {
      return reinterpret_cast<const SamplerAddressMode&>( m_samplerCreateInfo.addressModeW );
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

    SamplerCreateInfo& mipLodBias( float mipLodBias )
    {
      m_samplerCreateInfo.mipLodBias = mipLodBias;
      return *this;
    }

    const Bool32& anisotropyEnable() const
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

    SamplerCreateInfo& maxAnisotropy( float maxAnisotropy )
    {
      m_samplerCreateInfo.maxAnisotropy = maxAnisotropy;
      return *this;
    }

    const Bool32& compareEnable() const
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

    SamplerCreateInfo& compareOp( CompareOp compareOp )
    {
      m_samplerCreateInfo.compareOp = static_cast<VkCompareOp>( compareOp );
      return *this;
    }

    const float& minLod() const
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

    SamplerCreateInfo& maxLod( float maxLod )
    {
      m_samplerCreateInfo.maxLod = maxLod;
      return *this;
    }

    const BorderColor& borderColor() const
    {
      return reinterpret_cast<const BorderColor&>( m_samplerCreateInfo.borderColor );
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

    SamplerCreateInfo& unnormalizedCoordinates( Bool32 unnormalizedCoordinates )
    {
      m_samplerCreateInfo.unnormalizedCoordinates = unnormalizedCoordinates;
      return *this;
    }

    operator const VkSamplerCreateInfo&() const
    {
      return m_samplerCreateInfo;
    }

  private:
    VkSamplerCreateInfo m_samplerCreateInfo;
  };

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
      m_commandBufferAllocateInfo.commandPool = commandPool;
      m_commandBufferAllocateInfo.level = static_cast<VkCommandBufferLevel>( level );
      m_commandBufferAllocateInfo.commandBufferCount = commandBufferCount;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_commandBufferAllocateInfo.sType );
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

    CommandBufferAllocateInfo& pNext( const void* pNext )
    {
      m_commandBufferAllocateInfo.pNext = pNext;
      return *this;
    }

    const CommandPool& commandPool() const
    {
      return m_commandBufferAllocateInfo.commandPool;
    }

    CommandBufferAllocateInfo& commandPool( CommandPool commandPool )
    {
      m_commandBufferAllocateInfo.commandPool = commandPool;
      return *this;
    }

    const CommandBufferLevel& level() const
    {
      return reinterpret_cast<const CommandBufferLevel&>( m_commandBufferAllocateInfo.level );
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

    CommandBufferAllocateInfo& commandBufferCount( uint32_t commandBufferCount )
    {
      m_commandBufferAllocateInfo.commandBufferCount = commandBufferCount;
      return *this;
    }

    operator const VkCommandBufferAllocateInfo&() const
    {
      return m_commandBufferAllocateInfo;
    }

  private:
    VkCommandBufferAllocateInfo m_commandBufferAllocateInfo;
  };

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
      m_renderPassBeginInfo.renderPass = renderPass;
      m_renderPassBeginInfo.framebuffer = framebuffer;
      m_renderPassBeginInfo.renderArea = static_cast<VkRect2D>( renderArea );
      m_renderPassBeginInfo.clearValueCount = clearValueCount;
      m_renderPassBeginInfo.pClearValues = reinterpret_cast<const VkClearValue*>( pClearValues );
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_renderPassBeginInfo.sType );
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

    RenderPassBeginInfo& pNext( const void* pNext )
    {
      m_renderPassBeginInfo.pNext = pNext;
      return *this;
    }

    const RenderPass& renderPass() const
    {
      return m_renderPassBeginInfo.renderPass;
    }

    RenderPassBeginInfo& renderPass( RenderPass renderPass )
    {
      m_renderPassBeginInfo.renderPass = renderPass;
      return *this;
    }

    const Framebuffer& framebuffer() const
    {
      return m_renderPassBeginInfo.framebuffer;
    }

    RenderPassBeginInfo& framebuffer( Framebuffer framebuffer )
    {
      m_renderPassBeginInfo.framebuffer = framebuffer;
      return *this;
    }

    const Rect2D& renderArea() const
    {
      return reinterpret_cast<const Rect2D&>( m_renderPassBeginInfo.renderArea );
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

    RenderPassBeginInfo& clearValueCount( uint32_t clearValueCount )
    {
      m_renderPassBeginInfo.clearValueCount = clearValueCount;
      return *this;
    }

    const ClearValue* pClearValues() const
    {
      return reinterpret_cast<const ClearValue*>( m_renderPassBeginInfo.pClearValues );
    }

    RenderPassBeginInfo& pClearValues( const ClearValue* pClearValues )
    {
      m_renderPassBeginInfo.pClearValues = reinterpret_cast<const VkClearValue*>( pClearValues );
      return *this;
    }

    operator const VkRenderPassBeginInfo&() const
    {
      return m_renderPassBeginInfo;
    }

  private:
    VkRenderPassBeginInfo m_renderPassBeginInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_eventCreateInfo.sType );
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

    EventCreateInfo& pNext( const void* pNext )
    {
      m_eventCreateInfo.pNext = pNext;
      return *this;
    }

    const EventCreateFlags& flags() const
    {
      return reinterpret_cast<const EventCreateFlags&>( m_eventCreateInfo.flags );
    }

    EventCreateInfo& flags( EventCreateFlags flags )
    {
      m_eventCreateInfo.flags = static_cast<VkEventCreateFlags>( flags );
      return *this;
    }

    operator const VkEventCreateInfo&() const
    {
      return m_eventCreateInfo;
    }

  private:
    VkEventCreateInfo m_eventCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_semaphoreCreateInfo.sType );
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

    SemaphoreCreateInfo& pNext( const void* pNext )
    {
      m_semaphoreCreateInfo.pNext = pNext;
      return *this;
    }

    const SemaphoreCreateFlags& flags() const
    {
      return reinterpret_cast<const SemaphoreCreateFlags&>( m_semaphoreCreateInfo.flags );
    }

    SemaphoreCreateInfo& flags( SemaphoreCreateFlags flags )
    {
      m_semaphoreCreateInfo.flags = static_cast<VkSemaphoreCreateFlags>( flags );
      return *this;
    }

    operator const VkSemaphoreCreateInfo&() const
    {
      return m_semaphoreCreateInfo;
    }

  private:
    VkSemaphoreCreateInfo m_semaphoreCreateInfo;
  };

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
      m_framebufferCreateInfo.renderPass = renderPass;
      m_framebufferCreateInfo.attachmentCount = attachmentCount;
      m_framebufferCreateInfo.pAttachments = pAttachments;
      m_framebufferCreateInfo.width = width;
      m_framebufferCreateInfo.height = height;
      m_framebufferCreateInfo.layers = layers;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_framebufferCreateInfo.sType );
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

    FramebufferCreateInfo& pNext( const void* pNext )
    {
      m_framebufferCreateInfo.pNext = pNext;
      return *this;
    }

    const FramebufferCreateFlags& flags() const
    {
      return reinterpret_cast<const FramebufferCreateFlags&>( m_framebufferCreateInfo.flags );
    }

    FramebufferCreateInfo& flags( FramebufferCreateFlags flags )
    {
      m_framebufferCreateInfo.flags = static_cast<VkFramebufferCreateFlags>( flags );
      return *this;
    }

    const RenderPass& renderPass() const
    {
      return m_framebufferCreateInfo.renderPass;
    }

    FramebufferCreateInfo& renderPass( RenderPass renderPass )
    {
      m_framebufferCreateInfo.renderPass = renderPass;
      return *this;
    }

    const uint32_t& attachmentCount() const
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

    FramebufferCreateInfo& pAttachments( const ImageView* pAttachments )
    {
      m_framebufferCreateInfo.pAttachments = pAttachments;
      return *this;
    }

    const uint32_t& width() const
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

    FramebufferCreateInfo& height( uint32_t height )
    {
      m_framebufferCreateInfo.height = height;
      return *this;
    }

    const uint32_t& layers() const
    {
      return m_framebufferCreateInfo.layers;
    }

    FramebufferCreateInfo& layers( uint32_t layers )
    {
      m_framebufferCreateInfo.layers = layers;
      return *this;
    }

    operator const VkFramebufferCreateInfo&() const
    {
      return m_framebufferCreateInfo;
    }

  private:
    VkFramebufferCreateInfo m_framebufferCreateInfo;
  };

  enum class SubpassContents
  {
    eInline = VK_SUBPASS_CONTENTS_INLINE,
    eSecondaryCommandBuffers = VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS
  };

  enum class Result
  {
    eVkSuccess = VK_SUCCESS,
    eVkNotReady = VK_NOT_READY,
    eVkTimeout = VK_TIMEOUT,
    eVkEventSet = VK_EVENT_SET,
    eVkEventReset = VK_EVENT_RESET,
    eVkIncomplete = VK_INCOMPLETE,
    eVkErrorOutOfHostMemory = VK_ERROR_OUT_OF_HOST_MEMORY,
    eVkErrorOutOfDeviceMemory = VK_ERROR_OUT_OF_DEVICE_MEMORY,
    eVkErrorInitializationFailed = VK_ERROR_INITIALIZATION_FAILED,
    eVkErrorDeviceLost = VK_ERROR_DEVICE_LOST,
    eVkErrorMemoryMapFailed = VK_ERROR_MEMORY_MAP_FAILED,
    eVkErrorLayerNotPresent = VK_ERROR_LAYER_NOT_PRESENT,
    eVkErrorExtensionNotPresent = VK_ERROR_EXTENSION_NOT_PRESENT,
    eVkErrorFeatureNotPresent = VK_ERROR_FEATURE_NOT_PRESENT,
    eVkErrorIncompatibleDriver = VK_ERROR_INCOMPATIBLE_DRIVER,
    eVkErrorTooManyObjects = VK_ERROR_TOO_MANY_OBJECTS,
    eVkErrorFormatNotSupported = VK_ERROR_FORMAT_NOT_SUPPORTED,
    eVkErrorSurfaceLostKhr = VK_ERROR_SURFACE_LOST_KHR,
    eVkErrorNativeWindowInUseKhr = VK_ERROR_NATIVE_WINDOW_IN_USE_KHR,
    eVkSuboptimalKhr = VK_SUBOPTIMAL_KHR,
    eVkErrorOutOfDateKhr = VK_ERROR_OUT_OF_DATE_KHR,
    eVkErrorIncompatibleDisplayKhr = VK_ERROR_INCOMPATIBLE_DISPLAY_KHR,
    eVkErrorValidationFailedExt = VK_ERROR_VALIDATION_FAILED_EXT
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
      m_presentInfoKHR.pWaitSemaphores = pWaitSemaphores;
      m_presentInfoKHR.swapchainCount = swapchainCount;
      m_presentInfoKHR.pSwapchains = pSwapchains;
      m_presentInfoKHR.pImageIndices = pImageIndices;
      m_presentInfoKHR.pResults = reinterpret_cast<VkResult*>( pResults );
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_presentInfoKHR.sType );
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

    PresentInfoKHR& pNext( const void* pNext )
    {
      m_presentInfoKHR.pNext = pNext;
      return *this;
    }

    const uint32_t& waitSemaphoreCount() const
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

    PresentInfoKHR& pWaitSemaphores( const Semaphore* pWaitSemaphores )
    {
      m_presentInfoKHR.pWaitSemaphores = pWaitSemaphores;
      return *this;
    }

    const uint32_t& swapchainCount() const
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

    PresentInfoKHR& pSwapchains( const SwapchainKHR* pSwapchains )
    {
      m_presentInfoKHR.pSwapchains = pSwapchains;
      return *this;
    }

    const uint32_t* pImageIndices() const
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
      return reinterpret_cast<Result*>( m_presentInfoKHR.pResults );
    }

    PresentInfoKHR& pResults( Result* pResults )
    {
      m_presentInfoKHR.pResults = reinterpret_cast<VkResult*>( pResults );
      return *this;
    }

    operator const VkPresentInfoKHR&() const
    {
      return m_presentInfoKHR;
    }

  private:
    VkPresentInfoKHR m_presentInfoKHR;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineDynamicStateCreateInfo.sType );
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

    PipelineDynamicStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineDynamicStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineDynamicStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineDynamicStateCreateFlags&>( m_pipelineDynamicStateCreateInfo.flags );
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

    PipelineDynamicStateCreateInfo& dynamicStateCount( uint32_t dynamicStateCount )
    {
      m_pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStateCount;
      return *this;
    }

    const DynamicState* pDynamicStates() const
    {
      return reinterpret_cast<const DynamicState*>( m_pipelineDynamicStateCreateInfo.pDynamicStates );
    }

    PipelineDynamicStateCreateInfo& pDynamicStates( const DynamicState* pDynamicStates )
    {
      m_pipelineDynamicStateCreateInfo.pDynamicStates = reinterpret_cast<const VkDynamicState*>( pDynamicStates );
      return *this;
    }

    operator const VkPipelineDynamicStateCreateInfo&() const
    {
      return m_pipelineDynamicStateCreateInfo;
    }

  private:
    VkPipelineDynamicStateCreateInfo m_pipelineDynamicStateCreateInfo;
  };

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

    operator const VkQueueFamilyProperties&() const
    {
      return m_queueFamilyProperties;
    }

  private:
    VkQueueFamilyProperties m_queueFamilyProperties;
  };

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

    operator const VkMemoryType&() const
    {
      return m_memoryType;
    }

  private:
    VkMemoryType m_memoryType;
  };

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

    operator const VkMemoryHeap&() const
    {
      return m_memoryHeap;
    }

  private:
    VkMemoryHeap m_memoryHeap;
  };

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

    operator const VkPhysicalDeviceMemoryProperties&() const
    {
      return m_physicalDeviceMemoryProperties;
    }

  private:
    VkPhysicalDeviceMemoryProperties m_physicalDeviceMemoryProperties;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_memoryBarrier.sType );
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

    MemoryBarrier& pNext( const void* pNext )
    {
      m_memoryBarrier.pNext = pNext;
      return *this;
    }

    const AccessFlags& srcAccessMask() const
    {
      return reinterpret_cast<const AccessFlags&>( m_memoryBarrier.srcAccessMask );
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

    MemoryBarrier& dstAccessMask( AccessFlags dstAccessMask )
    {
      m_memoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask );
      return *this;
    }

    operator const VkMemoryBarrier&() const
    {
      return m_memoryBarrier;
    }

  private:
    VkMemoryBarrier m_memoryBarrier;
  };

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
      m_bufferMemoryBarrier.buffer = buffer;
      m_bufferMemoryBarrier.offset = offset;
      m_bufferMemoryBarrier.size = size;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_bufferMemoryBarrier.sType );
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

    BufferMemoryBarrier& pNext( const void* pNext )
    {
      m_bufferMemoryBarrier.pNext = pNext;
      return *this;
    }

    const AccessFlags& srcAccessMask() const
    {
      return reinterpret_cast<const AccessFlags&>( m_bufferMemoryBarrier.srcAccessMask );
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

    BufferMemoryBarrier& dstAccessMask( AccessFlags dstAccessMask )
    {
      m_bufferMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask );
      return *this;
    }

    const uint32_t& srcQueueFamilyIndex() const
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

    BufferMemoryBarrier& dstQueueFamilyIndex( uint32_t dstQueueFamilyIndex )
    {
      m_bufferMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
      return *this;
    }

    const Buffer& buffer() const
    {
      return m_bufferMemoryBarrier.buffer;
    }

    BufferMemoryBarrier& buffer( Buffer buffer )
    {
      m_bufferMemoryBarrier.buffer = buffer;
      return *this;
    }

    const DeviceSize& offset() const
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

    BufferMemoryBarrier& size( DeviceSize size )
    {
      m_bufferMemoryBarrier.size = size;
      return *this;
    }

    operator const VkBufferMemoryBarrier&() const
    {
      return m_bufferMemoryBarrier;
    }

  private:
    VkBufferMemoryBarrier m_bufferMemoryBarrier;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_bufferCreateInfo.sType );
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

    BufferCreateInfo& pNext( const void* pNext )
    {
      m_bufferCreateInfo.pNext = pNext;
      return *this;
    }

    const BufferCreateFlags& flags() const
    {
      return reinterpret_cast<const BufferCreateFlags&>( m_bufferCreateInfo.flags );
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

    BufferCreateInfo& size( DeviceSize size )
    {
      m_bufferCreateInfo.size = size;
      return *this;
    }

    const BufferUsageFlags& usage() const
    {
      return reinterpret_cast<const BufferUsageFlags&>( m_bufferCreateInfo.usage );
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

    BufferCreateInfo& sharingMode( SharingMode sharingMode )
    {
      m_bufferCreateInfo.sharingMode = static_cast<VkSharingMode>( sharingMode );
      return *this;
    }

    const uint32_t& queueFamilyIndexCount() const
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

    BufferCreateInfo& pQueueFamilyIndices( const uint32_t* pQueueFamilyIndices )
    {
      m_bufferCreateInfo.pQueueFamilyIndices = pQueueFamilyIndices;
      return *this;
    }

    operator const VkBufferCreateInfo&() const
    {
      return m_bufferCreateInfo;
    }

  private:
    VkBufferCreateInfo m_bufferCreateInfo;
  };

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
      m_descriptorSetLayoutBinding.pImmutableSamplers = pImmutableSamplers;
    }

    const uint32_t& binding() const
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

    DescriptorSetLayoutBinding& descriptorType( DescriptorType descriptorType )
    {
      m_descriptorSetLayoutBinding.descriptorType = static_cast<VkDescriptorType>( descriptorType );
      return *this;
    }

    const uint32_t& descriptorCount() const
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

    DescriptorSetLayoutBinding& stageFlags( ShaderStageFlags stageFlags )
    {
      m_descriptorSetLayoutBinding.stageFlags = static_cast<VkShaderStageFlags>( stageFlags );
      return *this;
    }

    const Sampler* pImmutableSamplers() const
    {
      return reinterpret_cast<const Sampler*>( m_descriptorSetLayoutBinding.pImmutableSamplers );
    }

    DescriptorSetLayoutBinding& pImmutableSamplers( const Sampler* pImmutableSamplers )
    {
      m_descriptorSetLayoutBinding.pImmutableSamplers = pImmutableSamplers;
      return *this;
    }

    operator const VkDescriptorSetLayoutBinding&() const
    {
      return m_descriptorSetLayoutBinding;
    }

  private:
    VkDescriptorSetLayoutBinding m_descriptorSetLayoutBinding;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_descriptorSetLayoutCreateInfo.sType );
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

    DescriptorSetLayoutCreateInfo& pNext( const void* pNext )
    {
      m_descriptorSetLayoutCreateInfo.pNext = pNext;
      return *this;
    }

    const DescriptorSetLayoutCreateFlags& flags() const
    {
      return reinterpret_cast<const DescriptorSetLayoutCreateFlags&>( m_descriptorSetLayoutCreateInfo.flags );
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

    DescriptorSetLayoutCreateInfo& bindingCount( uint32_t bindingCount )
    {
      m_descriptorSetLayoutCreateInfo.bindingCount = bindingCount;
      return *this;
    }

    const DescriptorSetLayoutBinding* pBindings() const
    {
      return reinterpret_cast<const DescriptorSetLayoutBinding*>( m_descriptorSetLayoutCreateInfo.pBindings );
    }

    DescriptorSetLayoutCreateInfo& pBindings( const DescriptorSetLayoutBinding* pBindings )
    {
      m_descriptorSetLayoutCreateInfo.pBindings = reinterpret_cast<const VkDescriptorSetLayoutBinding*>( pBindings );
      return *this;
    }

    operator const VkDescriptorSetLayoutCreateInfo&() const
    {
      return m_descriptorSetLayoutCreateInfo;
    }

  private:
    VkDescriptorSetLayoutCreateInfo m_descriptorSetLayoutCreateInfo;
  };

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
      m_pipelineShaderStageCreateInfo.module = module;
      m_pipelineShaderStageCreateInfo.pName = pName;
      m_pipelineShaderStageCreateInfo.pSpecializationInfo = reinterpret_cast<const VkSpecializationInfo*>( pSpecializationInfo );
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineShaderStageCreateInfo.sType );
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

    PipelineShaderStageCreateInfo& pNext( const void* pNext )
    {
      m_pipelineShaderStageCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineShaderStageCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineShaderStageCreateFlags&>( m_pipelineShaderStageCreateInfo.flags );
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

    PipelineShaderStageCreateInfo& stage( ShaderStageFlagBits stage )
    {
      m_pipelineShaderStageCreateInfo.stage = static_cast<VkShaderStageFlagBits>( stage );
      return *this;
    }

    const ShaderModule& module() const
    {
      return m_pipelineShaderStageCreateInfo.module;
    }

    PipelineShaderStageCreateInfo& module( ShaderModule module )
    {
      m_pipelineShaderStageCreateInfo.module = module;
      return *this;
    }

    const char* pName() const
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

    PipelineShaderStageCreateInfo& pSpecializationInfo( const SpecializationInfo* pSpecializationInfo )
    {
      m_pipelineShaderStageCreateInfo.pSpecializationInfo = reinterpret_cast<const VkSpecializationInfo*>( pSpecializationInfo );
      return *this;
    }

    operator const VkPipelineShaderStageCreateInfo&() const
    {
      return m_pipelineShaderStageCreateInfo;
    }

  private:
    VkPipelineShaderStageCreateInfo m_pipelineShaderStageCreateInfo;
  };

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

    const ShaderStageFlags& stageFlags() const
    {
      return reinterpret_cast<const ShaderStageFlags&>( m_pushConstantRange.stageFlags );
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

    PushConstantRange& offset( uint32_t offset )
    {
      m_pushConstantRange.offset = offset;
      return *this;
    }

    const uint32_t& size() const
    {
      return m_pushConstantRange.size;
    }

    PushConstantRange& size( uint32_t size )
    {
      m_pushConstantRange.size = size;
      return *this;
    }

    operator const VkPushConstantRange&() const
    {
      return m_pushConstantRange;
    }

  private:
    VkPushConstantRange m_pushConstantRange;
  };

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
      m_pipelineLayoutCreateInfo.pSetLayouts = pSetLayouts;
      m_pipelineLayoutCreateInfo.pushConstantRangeCount = pushConstantRangeCount;
      m_pipelineLayoutCreateInfo.pPushConstantRanges = reinterpret_cast<const VkPushConstantRange*>( pPushConstantRanges );
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineLayoutCreateInfo.sType );
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

    PipelineLayoutCreateInfo& pNext( const void* pNext )
    {
      m_pipelineLayoutCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineLayoutCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineLayoutCreateFlags&>( m_pipelineLayoutCreateInfo.flags );
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

    PipelineLayoutCreateInfo& setLayoutCount( uint32_t setLayoutCount )
    {
      m_pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount;
      return *this;
    }

    const DescriptorSetLayout* pSetLayouts() const
    {
      return reinterpret_cast<const DescriptorSetLayout*>( m_pipelineLayoutCreateInfo.pSetLayouts );
    }

    PipelineLayoutCreateInfo& pSetLayouts( const DescriptorSetLayout* pSetLayouts )
    {
      m_pipelineLayoutCreateInfo.pSetLayouts = pSetLayouts;
      return *this;
    }

    const uint32_t& pushConstantRangeCount() const
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

    PipelineLayoutCreateInfo& pPushConstantRanges( const PushConstantRange* pPushConstantRanges )
    {
      m_pipelineLayoutCreateInfo.pPushConstantRanges = reinterpret_cast<const VkPushConstantRange*>( pPushConstantRanges );
      return *this;
    }

    operator const VkPipelineLayoutCreateInfo&() const
    {
      return m_pipelineLayoutCreateInfo;
    }

  private:
    VkPipelineLayoutCreateInfo m_pipelineLayoutCreateInfo;
  };

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
      m_computePipelineCreateInfo.layout = layout;
      m_computePipelineCreateInfo.basePipelineHandle = basePipelineHandle;
      m_computePipelineCreateInfo.basePipelineIndex = basePipelineIndex;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_computePipelineCreateInfo.sType );
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

    ComputePipelineCreateInfo& pNext( const void* pNext )
    {
      m_computePipelineCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineCreateFlags&>( m_computePipelineCreateInfo.flags );
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

    ComputePipelineCreateInfo& stage( PipelineShaderStageCreateInfo stage )
    {
      m_computePipelineCreateInfo.stage = static_cast<VkPipelineShaderStageCreateInfo>( stage );
      return *this;
    }

    const PipelineLayout& layout() const
    {
      return m_computePipelineCreateInfo.layout;
    }

    ComputePipelineCreateInfo& layout( PipelineLayout layout )
    {
      m_computePipelineCreateInfo.layout = layout;
      return *this;
    }

    const Pipeline& basePipelineHandle() const
    {
      return m_computePipelineCreateInfo.basePipelineHandle;
    }

    ComputePipelineCreateInfo& basePipelineHandle( Pipeline basePipelineHandle )
    {
      m_computePipelineCreateInfo.basePipelineHandle = basePipelineHandle;
      return *this;
    }

    const int32_t& basePipelineIndex() const
    {
      return m_computePipelineCreateInfo.basePipelineIndex;
    }

    ComputePipelineCreateInfo& basePipelineIndex( int32_t basePipelineIndex )
    {
      m_computePipelineCreateInfo.basePipelineIndex = basePipelineIndex;
      return *this;
    }

    operator const VkComputePipelineCreateInfo&() const
    {
      return m_computePipelineCreateInfo;
    }

  private:
    VkComputePipelineCreateInfo m_computePipelineCreateInfo;
  };

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

    const Bool32& blendEnable() const
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

    PipelineColorBlendAttachmentState& srcColorBlendFactor( BlendFactor srcColorBlendFactor )
    {
      m_pipelineColorBlendAttachmentState.srcColorBlendFactor = static_cast<VkBlendFactor>( srcColorBlendFactor );
      return *this;
    }

    const BlendFactor& dstColorBlendFactor() const
    {
      return reinterpret_cast<const BlendFactor&>( m_pipelineColorBlendAttachmentState.dstColorBlendFactor );
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

    PipelineColorBlendAttachmentState& colorBlendOp( BlendOp colorBlendOp )
    {
      m_pipelineColorBlendAttachmentState.colorBlendOp = static_cast<VkBlendOp>( colorBlendOp );
      return *this;
    }

    const BlendFactor& srcAlphaBlendFactor() const
    {
      return reinterpret_cast<const BlendFactor&>( m_pipelineColorBlendAttachmentState.srcAlphaBlendFactor );
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

    PipelineColorBlendAttachmentState& dstAlphaBlendFactor( BlendFactor dstAlphaBlendFactor )
    {
      m_pipelineColorBlendAttachmentState.dstAlphaBlendFactor = static_cast<VkBlendFactor>( dstAlphaBlendFactor );
      return *this;
    }

    const BlendOp& alphaBlendOp() const
    {
      return reinterpret_cast<const BlendOp&>( m_pipelineColorBlendAttachmentState.alphaBlendOp );
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

    PipelineColorBlendAttachmentState& colorWriteMask( ColorComponentFlags colorWriteMask )
    {
      m_pipelineColorBlendAttachmentState.colorWriteMask = static_cast<VkColorComponentFlags>( colorWriteMask );
      return *this;
    }

    operator const VkPipelineColorBlendAttachmentState&() const
    {
      return m_pipelineColorBlendAttachmentState;
    }

  private:
    VkPipelineColorBlendAttachmentState m_pipelineColorBlendAttachmentState;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineColorBlendStateCreateInfo.sType );
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

    PipelineColorBlendStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineColorBlendStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineColorBlendStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineColorBlendStateCreateFlags&>( m_pipelineColorBlendStateCreateInfo.flags );
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

    PipelineColorBlendStateCreateInfo& logicOpEnable( Bool32 logicOpEnable )
    {
      m_pipelineColorBlendStateCreateInfo.logicOpEnable = logicOpEnable;
      return *this;
    }

    const LogicOp& logicOp() const
    {
      return reinterpret_cast<const LogicOp&>( m_pipelineColorBlendStateCreateInfo.logicOp );
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

    PipelineColorBlendStateCreateInfo& attachmentCount( uint32_t attachmentCount )
    {
      m_pipelineColorBlendStateCreateInfo.attachmentCount = attachmentCount;
      return *this;
    }

    const PipelineColorBlendAttachmentState* pAttachments() const
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

    PipelineColorBlendStateCreateInfo& blendConstants( std::array<float,4> blendConstants )
    {
      memcpy( &m_pipelineColorBlendStateCreateInfo.blendConstants, blendConstants.data(), 4 * sizeof( float ) );
      return *this;
    }

    operator const VkPipelineColorBlendStateCreateInfo&() const
    {
      return m_pipelineColorBlendStateCreateInfo;
    }

  private:
    VkPipelineColorBlendStateCreateInfo m_pipelineColorBlendStateCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_fenceCreateInfo.sType );
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

    FenceCreateInfo& pNext( const void* pNext )
    {
      m_fenceCreateInfo.pNext = pNext;
      return *this;
    }

    const FenceCreateFlags& flags() const
    {
      return reinterpret_cast<const FenceCreateFlags&>( m_fenceCreateInfo.flags );
    }

    FenceCreateInfo& flags( FenceCreateFlags flags )
    {
      m_fenceCreateInfo.flags = static_cast<VkFenceCreateFlags>( flags );
      return *this;
    }

    operator const VkFenceCreateInfo&() const
    {
      return m_fenceCreateInfo;
    }

  private:
    VkFenceCreateInfo m_fenceCreateInfo;
  };

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

    operator const VkFormatProperties&() const
    {
      return m_formatProperties;
    }

  private:
    VkFormatProperties m_formatProperties;
  };

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
      m_commandBufferInheritanceInfo.renderPass = renderPass;
      m_commandBufferInheritanceInfo.subpass = subpass;
      m_commandBufferInheritanceInfo.framebuffer = framebuffer;
      m_commandBufferInheritanceInfo.occlusionQueryEnable = occlusionQueryEnable;
      m_commandBufferInheritanceInfo.queryFlags = static_cast<VkQueryControlFlags>( queryFlags );
      m_commandBufferInheritanceInfo.pipelineStatistics = static_cast<VkQueryPipelineStatisticFlags>( pipelineStatistics );
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_commandBufferInheritanceInfo.sType );
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

    CommandBufferInheritanceInfo& pNext( const void* pNext )
    {
      m_commandBufferInheritanceInfo.pNext = pNext;
      return *this;
    }

    const RenderPass& renderPass() const
    {
      return m_commandBufferInheritanceInfo.renderPass;
    }

    CommandBufferInheritanceInfo& renderPass( RenderPass renderPass )
    {
      m_commandBufferInheritanceInfo.renderPass = renderPass;
      return *this;
    }

    const uint32_t& subpass() const
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
      return m_commandBufferInheritanceInfo.framebuffer;
    }

    CommandBufferInheritanceInfo& framebuffer( Framebuffer framebuffer )
    {
      m_commandBufferInheritanceInfo.framebuffer = framebuffer;
      return *this;
    }

    const Bool32& occlusionQueryEnable() const
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

    CommandBufferInheritanceInfo& queryFlags( QueryControlFlags queryFlags )
    {
      m_commandBufferInheritanceInfo.queryFlags = static_cast<VkQueryControlFlags>( queryFlags );
      return *this;
    }

    const QueryPipelineStatisticFlags& pipelineStatistics() const
    {
      return reinterpret_cast<const QueryPipelineStatisticFlags&>( m_commandBufferInheritanceInfo.pipelineStatistics );
    }

    CommandBufferInheritanceInfo& pipelineStatistics( QueryPipelineStatisticFlags pipelineStatistics )
    {
      m_commandBufferInheritanceInfo.pipelineStatistics = static_cast<VkQueryPipelineStatisticFlags>( pipelineStatistics );
      return *this;
    }

    operator const VkCommandBufferInheritanceInfo&() const
    {
      return m_commandBufferInheritanceInfo;
    }

  private:
    VkCommandBufferInheritanceInfo m_commandBufferInheritanceInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_commandBufferBeginInfo.sType );
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

    CommandBufferBeginInfo& pNext( const void* pNext )
    {
      m_commandBufferBeginInfo.pNext = pNext;
      return *this;
    }

    const CommandBufferUsageFlags& flags() const
    {
      return reinterpret_cast<const CommandBufferUsageFlags&>( m_commandBufferBeginInfo.flags );
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

    CommandBufferBeginInfo& pInheritanceInfo( const CommandBufferInheritanceInfo* pInheritanceInfo )
    {
      m_commandBufferBeginInfo.pInheritanceInfo = reinterpret_cast<const VkCommandBufferInheritanceInfo*>( pInheritanceInfo );
      return *this;
    }

    operator const VkCommandBufferBeginInfo&() const
    {
      return m_commandBufferBeginInfo;
    }

  private:
    VkCommandBufferBeginInfo m_commandBufferBeginInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_queryPoolCreateInfo.sType );
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

    QueryPoolCreateInfo& pNext( const void* pNext )
    {
      m_queryPoolCreateInfo.pNext = pNext;
      return *this;
    }

    const QueryPoolCreateFlags& flags() const
    {
      return reinterpret_cast<const QueryPoolCreateFlags&>( m_queryPoolCreateInfo.flags );
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

    QueryPoolCreateInfo& queryType( QueryType queryType )
    {
      m_queryPoolCreateInfo.queryType = static_cast<VkQueryType>( queryType );
      return *this;
    }

    const uint32_t& queryCount() const
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

    QueryPoolCreateInfo& pipelineStatistics( QueryPipelineStatisticFlags pipelineStatistics )
    {
      m_queryPoolCreateInfo.pipelineStatistics = static_cast<VkQueryPipelineStatisticFlags>( pipelineStatistics );
      return *this;
    }

    operator const VkQueryPoolCreateInfo&() const
    {
      return m_queryPoolCreateInfo;
    }

  private:
    VkQueryPoolCreateInfo m_queryPoolCreateInfo;
  };

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

    const ImageAspectFlags& aspectMask() const
    {
      return reinterpret_cast<const ImageAspectFlags&>( m_imageSubresource.aspectMask );
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

    ImageSubresource& mipLevel( uint32_t mipLevel )
    {
      m_imageSubresource.mipLevel = mipLevel;
      return *this;
    }

    const uint32_t& arrayLayer() const
    {
      return m_imageSubresource.arrayLayer;
    }

    ImageSubresource& arrayLayer( uint32_t arrayLayer )
    {
      m_imageSubresource.arrayLayer = arrayLayer;
      return *this;
    }

    operator const VkImageSubresource&() const
    {
      return m_imageSubresource;
    }

  private:
    VkImageSubresource m_imageSubresource;
  };

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

    const ImageAspectFlags& aspectMask() const
    {
      return reinterpret_cast<const ImageAspectFlags&>( m_imageSubresourceLayers.aspectMask );
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

    ImageSubresourceLayers& mipLevel( uint32_t mipLevel )
    {
      m_imageSubresourceLayers.mipLevel = mipLevel;
      return *this;
    }

    const uint32_t& baseArrayLayer() const
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

    ImageSubresourceLayers& layerCount( uint32_t layerCount )
    {
      m_imageSubresourceLayers.layerCount = layerCount;
      return *this;
    }

    operator const VkImageSubresourceLayers&() const
    {
      return m_imageSubresourceLayers;
    }

  private:
    VkImageSubresourceLayers m_imageSubresourceLayers;
  };

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

    const ImageAspectFlags& aspectMask() const
    {
      return reinterpret_cast<const ImageAspectFlags&>( m_imageSubresourceRange.aspectMask );
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

    ImageSubresourceRange& baseMipLevel( uint32_t baseMipLevel )
    {
      m_imageSubresourceRange.baseMipLevel = baseMipLevel;
      return *this;
    }

    const uint32_t& levelCount() const
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

    ImageSubresourceRange& baseArrayLayer( uint32_t baseArrayLayer )
    {
      m_imageSubresourceRange.baseArrayLayer = baseArrayLayer;
      return *this;
    }

    const uint32_t& layerCount() const
    {
      return m_imageSubresourceRange.layerCount;
    }

    ImageSubresourceRange& layerCount( uint32_t layerCount )
    {
      m_imageSubresourceRange.layerCount = layerCount;
      return *this;
    }

    operator const VkImageSubresourceRange&() const
    {
      return m_imageSubresourceRange;
    }

  private:
    VkImageSubresourceRange m_imageSubresourceRange;
  };

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
      m_imageMemoryBarrier.image = image;
      m_imageMemoryBarrier.subresourceRange = static_cast<VkImageSubresourceRange>( subresourceRange );
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_imageMemoryBarrier.sType );
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

    ImageMemoryBarrier& pNext( const void* pNext )
    {
      m_imageMemoryBarrier.pNext = pNext;
      return *this;
    }

    const AccessFlags& srcAccessMask() const
    {
      return reinterpret_cast<const AccessFlags&>( m_imageMemoryBarrier.srcAccessMask );
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

    ImageMemoryBarrier& dstAccessMask( AccessFlags dstAccessMask )
    {
      m_imageMemoryBarrier.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask );
      return *this;
    }

    const ImageLayout& oldLayout() const
    {
      return reinterpret_cast<const ImageLayout&>( m_imageMemoryBarrier.oldLayout );
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

    ImageMemoryBarrier& newLayout( ImageLayout newLayout )
    {
      m_imageMemoryBarrier.newLayout = static_cast<VkImageLayout>( newLayout );
      return *this;
    }

    const uint32_t& srcQueueFamilyIndex() const
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

    ImageMemoryBarrier& dstQueueFamilyIndex( uint32_t dstQueueFamilyIndex )
    {
      m_imageMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
      return *this;
    }

    const Image& image() const
    {
      return m_imageMemoryBarrier.image;
    }

    ImageMemoryBarrier& image( Image image )
    {
      m_imageMemoryBarrier.image = image;
      return *this;
    }

    const ImageSubresourceRange& subresourceRange() const
    {
      return reinterpret_cast<const ImageSubresourceRange&>( m_imageMemoryBarrier.subresourceRange );
    }

    ImageMemoryBarrier& subresourceRange( ImageSubresourceRange subresourceRange )
    {
      m_imageMemoryBarrier.subresourceRange = static_cast<VkImageSubresourceRange>( subresourceRange );
      return *this;
    }

    operator const VkImageMemoryBarrier&() const
    {
      return m_imageMemoryBarrier;
    }

  private:
    VkImageMemoryBarrier m_imageMemoryBarrier;
  };

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
      m_imageViewCreateInfo.image = image;
      m_imageViewCreateInfo.viewType = static_cast<VkImageViewType>( viewType );
      m_imageViewCreateInfo.format = static_cast<VkFormat>( format );
      m_imageViewCreateInfo.components = static_cast<VkComponentMapping>( components );
      m_imageViewCreateInfo.subresourceRange = static_cast<VkImageSubresourceRange>( subresourceRange );
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_imageViewCreateInfo.sType );
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

    ImageViewCreateInfo& pNext( const void* pNext )
    {
      m_imageViewCreateInfo.pNext = pNext;
      return *this;
    }

    const ImageViewCreateFlags& flags() const
    {
      return reinterpret_cast<const ImageViewCreateFlags&>( m_imageViewCreateInfo.flags );
    }

    ImageViewCreateInfo& flags( ImageViewCreateFlags flags )
    {
      m_imageViewCreateInfo.flags = static_cast<VkImageViewCreateFlags>( flags );
      return *this;
    }

    const Image& image() const
    {
      return m_imageViewCreateInfo.image;
    }

    ImageViewCreateInfo& image( Image image )
    {
      m_imageViewCreateInfo.image = image;
      return *this;
    }

    const ImageViewType& viewType() const
    {
      return reinterpret_cast<const ImageViewType&>( m_imageViewCreateInfo.viewType );
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

    ImageViewCreateInfo& format( Format format )
    {
      m_imageViewCreateInfo.format = static_cast<VkFormat>( format );
      return *this;
    }

    const ComponentMapping& components() const
    {
      return reinterpret_cast<const ComponentMapping&>( m_imageViewCreateInfo.components );
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

    ImageViewCreateInfo& subresourceRange( ImageSubresourceRange subresourceRange )
    {
      m_imageViewCreateInfo.subresourceRange = static_cast<VkImageSubresourceRange>( subresourceRange );
      return *this;
    }

    operator const VkImageViewCreateInfo&() const
    {
      return m_imageViewCreateInfo;
    }

  private:
    VkImageViewCreateInfo m_imageViewCreateInfo;
  };

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

    const ImageSubresourceLayers& srcSubresource() const
    {
      return reinterpret_cast<const ImageSubresourceLayers&>( m_imageCopy.srcSubresource );
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

    ImageCopy& srcOffset( Offset3D srcOffset )
    {
      m_imageCopy.srcOffset = static_cast<VkOffset3D>( srcOffset );
      return *this;
    }

    const ImageSubresourceLayers& dstSubresource() const
    {
      return reinterpret_cast<const ImageSubresourceLayers&>( m_imageCopy.dstSubresource );
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

    ImageCopy& dstOffset( Offset3D dstOffset )
    {
      m_imageCopy.dstOffset = static_cast<VkOffset3D>( dstOffset );
      return *this;
    }

    const Extent3D& extent() const
    {
      return reinterpret_cast<const Extent3D&>( m_imageCopy.extent );
    }

    ImageCopy& extent( Extent3D extent )
    {
      m_imageCopy.extent = static_cast<VkExtent3D>( extent );
      return *this;
    }

    operator const VkImageCopy&() const
    {
      return m_imageCopy;
    }

  private:
    VkImageCopy m_imageCopy;
  };

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

    const ImageSubresourceLayers& srcSubresource() const
    {
      return reinterpret_cast<const ImageSubresourceLayers&>( m_imageBlit.srcSubresource );
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

    ImageBlit& srcOffsets( std::array<Offset3D,2> srcOffsets )
    {
      memcpy( &m_imageBlit.srcOffsets, srcOffsets.data(), 2 * sizeof( Offset3D ) );
      return *this;
    }

    const ImageSubresourceLayers& dstSubresource() const
    {
      return reinterpret_cast<const ImageSubresourceLayers&>( m_imageBlit.dstSubresource );
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

    ImageBlit& dstOffsets( std::array<Offset3D,2> dstOffsets )
    {
      memcpy( &m_imageBlit.dstOffsets, dstOffsets.data(), 2 * sizeof( Offset3D ) );
      return *this;
    }

    operator const VkImageBlit&() const
    {
      return m_imageBlit;
    }

  private:
    VkImageBlit m_imageBlit;
  };

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

    const DeviceSize& bufferOffset() const
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

    BufferImageCopy& bufferRowLength( uint32_t bufferRowLength )
    {
      m_bufferImageCopy.bufferRowLength = bufferRowLength;
      return *this;
    }

    const uint32_t& bufferImageHeight() const
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

    BufferImageCopy& imageSubresource( ImageSubresourceLayers imageSubresource )
    {
      m_bufferImageCopy.imageSubresource = static_cast<VkImageSubresourceLayers>( imageSubresource );
      return *this;
    }

    const Offset3D& imageOffset() const
    {
      return reinterpret_cast<const Offset3D&>( m_bufferImageCopy.imageOffset );
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

    BufferImageCopy& imageExtent( Extent3D imageExtent )
    {
      m_bufferImageCopy.imageExtent = static_cast<VkExtent3D>( imageExtent );
      return *this;
    }

    operator const VkBufferImageCopy&() const
    {
      return m_bufferImageCopy;
    }

  private:
    VkBufferImageCopy m_bufferImageCopy;
  };

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

    const ImageSubresourceLayers& srcSubresource() const
    {
      return reinterpret_cast<const ImageSubresourceLayers&>( m_imageResolve.srcSubresource );
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

    ImageResolve& srcOffset( Offset3D srcOffset )
    {
      m_imageResolve.srcOffset = static_cast<VkOffset3D>( srcOffset );
      return *this;
    }

    const ImageSubresourceLayers& dstSubresource() const
    {
      return reinterpret_cast<const ImageSubresourceLayers&>( m_imageResolve.dstSubresource );
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

    ImageResolve& dstOffset( Offset3D dstOffset )
    {
      m_imageResolve.dstOffset = static_cast<VkOffset3D>( dstOffset );
      return *this;
    }

    const Extent3D& extent() const
    {
      return reinterpret_cast<const Extent3D&>( m_imageResolve.extent );
    }

    ImageResolve& extent( Extent3D extent )
    {
      m_imageResolve.extent = static_cast<VkExtent3D>( extent );
      return *this;
    }

    operator const VkImageResolve&() const
    {
      return m_imageResolve;
    }

  private:
    VkImageResolve m_imageResolve;
  };

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

    const ImageAspectFlags& aspectMask() const
    {
      return reinterpret_cast<const ImageAspectFlags&>( m_clearAttachment.aspectMask );
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

    ClearAttachment& colorAttachment( uint32_t colorAttachment )
    {
      m_clearAttachment.colorAttachment = colorAttachment;
      return *this;
    }

    const ClearValue& clearValue() const
    {
      return reinterpret_cast<const ClearValue&>( m_clearAttachment.clearValue );
    }

    ClearAttachment& clearValue( ClearValue clearValue )
    {
      m_clearAttachment.clearValue = static_cast<VkClearValue>( clearValue );
      return *this;
    }

    operator const VkClearAttachment&() const
    {
      return m_clearAttachment;
    }

  private:
    VkClearAttachment m_clearAttachment;
  };

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

    operator const VkSparseImageFormatProperties&() const
    {
      return m_sparseImageFormatProperties;
    }

  private:
    VkSparseImageFormatProperties m_sparseImageFormatProperties;
  };

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

    operator const VkSparseImageMemoryRequirements&() const
    {
      return m_sparseImageMemoryRequirements;
    }

  private:
    VkSparseImageMemoryRequirements m_sparseImageMemoryRequirements;
  };

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
      m_sparseMemoryBind.memory = memory;
      m_sparseMemoryBind.memoryOffset = memoryOffset;
      m_sparseMemoryBind.flags = static_cast<VkSparseMemoryBindFlags>( flags );
    }

    const DeviceSize& resourceOffset() const
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

    SparseMemoryBind& size( DeviceSize size )
    {
      m_sparseMemoryBind.size = size;
      return *this;
    }

    const DeviceMemory& memory() const
    {
      return m_sparseMemoryBind.memory;
    }

    SparseMemoryBind& memory( DeviceMemory memory )
    {
      m_sparseMemoryBind.memory = memory;
      return *this;
    }

    const DeviceSize& memoryOffset() const
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

    SparseMemoryBind& flags( SparseMemoryBindFlags flags )
    {
      m_sparseMemoryBind.flags = static_cast<VkSparseMemoryBindFlags>( flags );
      return *this;
    }

    operator const VkSparseMemoryBind&() const
    {
      return m_sparseMemoryBind;
    }

  private:
    VkSparseMemoryBind m_sparseMemoryBind;
  };

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
      m_sparseImageMemoryBind.memory = memory;
      m_sparseImageMemoryBind.memoryOffset = memoryOffset;
      m_sparseImageMemoryBind.flags = static_cast<VkSparseMemoryBindFlags>( flags );
    }

    const ImageSubresource& subresource() const
    {
      return reinterpret_cast<const ImageSubresource&>( m_sparseImageMemoryBind.subresource );
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

    SparseImageMemoryBind& offset( Offset3D offset )
    {
      m_sparseImageMemoryBind.offset = static_cast<VkOffset3D>( offset );
      return *this;
    }

    const Extent3D& extent() const
    {
      return reinterpret_cast<const Extent3D&>( m_sparseImageMemoryBind.extent );
    }

    SparseImageMemoryBind& extent( Extent3D extent )
    {
      m_sparseImageMemoryBind.extent = static_cast<VkExtent3D>( extent );
      return *this;
    }

    const DeviceMemory& memory() const
    {
      return m_sparseImageMemoryBind.memory;
    }

    SparseImageMemoryBind& memory( DeviceMemory memory )
    {
      m_sparseImageMemoryBind.memory = memory;
      return *this;
    }

    const DeviceSize& memoryOffset() const
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

    SparseImageMemoryBind& flags( SparseMemoryBindFlags flags )
    {
      m_sparseImageMemoryBind.flags = static_cast<VkSparseMemoryBindFlags>( flags );
      return *this;
    }

    operator const VkSparseImageMemoryBind&() const
    {
      return m_sparseImageMemoryBind;
    }

  private:
    VkSparseImageMemoryBind m_sparseImageMemoryBind;
  };

  class SparseBufferMemoryBindInfo
  {
  public:
    SparseBufferMemoryBindInfo()
      : SparseBufferMemoryBindInfo( Buffer(), 0, nullptr )
    {}

    SparseBufferMemoryBindInfo( Buffer buffer, uint32_t bindCount, const SparseMemoryBind* pBinds)
    {
      m_sparseBufferMemoryBindInfo.buffer = buffer;
      m_sparseBufferMemoryBindInfo.bindCount = bindCount;
      m_sparseBufferMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseMemoryBind*>( pBinds );
    }

    const Buffer& buffer() const
    {
      return m_sparseBufferMemoryBindInfo.buffer;
    }

    SparseBufferMemoryBindInfo& buffer( Buffer buffer )
    {
      m_sparseBufferMemoryBindInfo.buffer = buffer;
      return *this;
    }

    const uint32_t& bindCount() const
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

    SparseBufferMemoryBindInfo& pBinds( const SparseMemoryBind* pBinds )
    {
      m_sparseBufferMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseMemoryBind*>( pBinds );
      return *this;
    }

    operator const VkSparseBufferMemoryBindInfo&() const
    {
      return m_sparseBufferMemoryBindInfo;
    }

  private:
    VkSparseBufferMemoryBindInfo m_sparseBufferMemoryBindInfo;
  };

  class SparseImageOpaqueMemoryBindInfo
  {
  public:
    SparseImageOpaqueMemoryBindInfo()
      : SparseImageOpaqueMemoryBindInfo( Image(), 0, nullptr )
    {}

    SparseImageOpaqueMemoryBindInfo( Image image, uint32_t bindCount, const SparseMemoryBind* pBinds)
    {
      m_sparseImageOpaqueMemoryBindInfo.image = image;
      m_sparseImageOpaqueMemoryBindInfo.bindCount = bindCount;
      m_sparseImageOpaqueMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseMemoryBind*>( pBinds );
    }

    const Image& image() const
    {
      return m_sparseImageOpaqueMemoryBindInfo.image;
    }

    SparseImageOpaqueMemoryBindInfo& image( Image image )
    {
      m_sparseImageOpaqueMemoryBindInfo.image = image;
      return *this;
    }

    const uint32_t& bindCount() const
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

    SparseImageOpaqueMemoryBindInfo& pBinds( const SparseMemoryBind* pBinds )
    {
      m_sparseImageOpaqueMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseMemoryBind*>( pBinds );
      return *this;
    }

    operator const VkSparseImageOpaqueMemoryBindInfo&() const
    {
      return m_sparseImageOpaqueMemoryBindInfo;
    }

  private:
    VkSparseImageOpaqueMemoryBindInfo m_sparseImageOpaqueMemoryBindInfo;
  };

  class SparseImageMemoryBindInfo
  {
  public:
    SparseImageMemoryBindInfo()
      : SparseImageMemoryBindInfo( Image(), 0, nullptr )
    {}

    SparseImageMemoryBindInfo( Image image, uint32_t bindCount, const SparseImageMemoryBind* pBinds)
    {
      m_sparseImageMemoryBindInfo.image = image;
      m_sparseImageMemoryBindInfo.bindCount = bindCount;
      m_sparseImageMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseImageMemoryBind*>( pBinds );
    }

    const Image& image() const
    {
      return m_sparseImageMemoryBindInfo.image;
    }

    SparseImageMemoryBindInfo& image( Image image )
    {
      m_sparseImageMemoryBindInfo.image = image;
      return *this;
    }

    const uint32_t& bindCount() const
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

    SparseImageMemoryBindInfo& pBinds( const SparseImageMemoryBind* pBinds )
    {
      m_sparseImageMemoryBindInfo.pBinds = reinterpret_cast<const VkSparseImageMemoryBind*>( pBinds );
      return *this;
    }

    operator const VkSparseImageMemoryBindInfo&() const
    {
      return m_sparseImageMemoryBindInfo;
    }

  private:
    VkSparseImageMemoryBindInfo m_sparseImageMemoryBindInfo;
  };

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
      m_bindSparseInfo.pWaitSemaphores = pWaitSemaphores;
      m_bindSparseInfo.bufferBindCount = bufferBindCount;
      m_bindSparseInfo.pBufferBinds = reinterpret_cast<const VkSparseBufferMemoryBindInfo*>( pBufferBinds );
      m_bindSparseInfo.imageOpaqueBindCount = imageOpaqueBindCount;
      m_bindSparseInfo.pImageOpaqueBinds = reinterpret_cast<const VkSparseImageOpaqueMemoryBindInfo*>( pImageOpaqueBinds );
      m_bindSparseInfo.imageBindCount = imageBindCount;
      m_bindSparseInfo.pImageBinds = reinterpret_cast<const VkSparseImageMemoryBindInfo*>( pImageBinds );
      m_bindSparseInfo.signalSemaphoreCount = signalSemaphoreCount;
      m_bindSparseInfo.pSignalSemaphores = pSignalSemaphores;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_bindSparseInfo.sType );
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

    BindSparseInfo& pNext( const void* pNext )
    {
      m_bindSparseInfo.pNext = pNext;
      return *this;
    }

    const uint32_t& waitSemaphoreCount() const
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

    BindSparseInfo& pWaitSemaphores( const Semaphore* pWaitSemaphores )
    {
      m_bindSparseInfo.pWaitSemaphores = pWaitSemaphores;
      return *this;
    }

    const uint32_t& bufferBindCount() const
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

    BindSparseInfo& pBufferBinds( const SparseBufferMemoryBindInfo* pBufferBinds )
    {
      m_bindSparseInfo.pBufferBinds = reinterpret_cast<const VkSparseBufferMemoryBindInfo*>( pBufferBinds );
      return *this;
    }

    const uint32_t& imageOpaqueBindCount() const
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

    BindSparseInfo& pImageOpaqueBinds( const SparseImageOpaqueMemoryBindInfo* pImageOpaqueBinds )
    {
      m_bindSparseInfo.pImageOpaqueBinds = reinterpret_cast<const VkSparseImageOpaqueMemoryBindInfo*>( pImageOpaqueBinds );
      return *this;
    }

    const uint32_t& imageBindCount() const
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

    BindSparseInfo& pImageBinds( const SparseImageMemoryBindInfo* pImageBinds )
    {
      m_bindSparseInfo.pImageBinds = reinterpret_cast<const VkSparseImageMemoryBindInfo*>( pImageBinds );
      return *this;
    }

    const uint32_t& signalSemaphoreCount() const
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

    BindSparseInfo& pSignalSemaphores( const Semaphore* pSignalSemaphores )
    {
      m_bindSparseInfo.pSignalSemaphores = pSignalSemaphores;
      return *this;
    }

    operator const VkBindSparseInfo&() const
    {
      return m_bindSparseInfo;
    }

  private:
    VkBindSparseInfo m_bindSparseInfo;
  };

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
      m_submitInfo.pWaitSemaphores = pWaitSemaphores;
      m_submitInfo.pWaitDstStageMask = reinterpret_cast<const VkPipelineStageFlags*>( pWaitDstStageMask );
      m_submitInfo.commandBufferCount = commandBufferCount;
      m_submitInfo.pCommandBuffers = pCommandBuffers;
      m_submitInfo.signalSemaphoreCount = signalSemaphoreCount;
      m_submitInfo.pSignalSemaphores = pSignalSemaphores;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_submitInfo.sType );
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

    SubmitInfo& pNext( const void* pNext )
    {
      m_submitInfo.pNext = pNext;
      return *this;
    }

    const uint32_t& waitSemaphoreCount() const
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

    SubmitInfo& pWaitSemaphores( const Semaphore* pWaitSemaphores )
    {
      m_submitInfo.pWaitSemaphores = pWaitSemaphores;
      return *this;
    }

    const PipelineStageFlags* pWaitDstStageMask() const
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

    SubmitInfo& commandBufferCount( uint32_t commandBufferCount )
    {
      m_submitInfo.commandBufferCount = commandBufferCount;
      return *this;
    }

    const CommandBuffer* pCommandBuffers() const
    {
      return reinterpret_cast<const CommandBuffer*>( m_submitInfo.pCommandBuffers );
    }

    SubmitInfo& pCommandBuffers( const CommandBuffer* pCommandBuffers )
    {
      m_submitInfo.pCommandBuffers = pCommandBuffers;
      return *this;
    }

    const uint32_t& signalSemaphoreCount() const
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

    SubmitInfo& pSignalSemaphores( const Semaphore* pSignalSemaphores )
    {
      m_submitInfo.pSignalSemaphores = pSignalSemaphores;
      return *this;
    }

    operator const VkSubmitInfo&() const
    {
      return m_submitInfo;
    }

  private:
    VkSubmitInfo m_submitInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_commandPoolCreateInfo.sType );
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

    CommandPoolCreateInfo& pNext( const void* pNext )
    {
      m_commandPoolCreateInfo.pNext = pNext;
      return *this;
    }

    const CommandPoolCreateFlags& flags() const
    {
      return reinterpret_cast<const CommandPoolCreateFlags&>( m_commandPoolCreateInfo.flags );
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

    CommandPoolCreateInfo& queueFamilyIndex( uint32_t queueFamilyIndex )
    {
      m_commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
      return *this;
    }

    operator const VkCommandPoolCreateInfo&() const
    {
      return m_commandPoolCreateInfo;
    }

  private:
    VkCommandPoolCreateInfo m_commandPoolCreateInfo;
  };

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

    operator const VkImageFormatProperties&() const
    {
      return m_imageFormatProperties;
    }

  private:
    VkImageFormatProperties m_imageFormatProperties;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_imageCreateInfo.sType );
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

    ImageCreateInfo& pNext( const void* pNext )
    {
      m_imageCreateInfo.pNext = pNext;
      return *this;
    }

    const ImageCreateFlags& flags() const
    {
      return reinterpret_cast<const ImageCreateFlags&>( m_imageCreateInfo.flags );
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

    ImageCreateInfo& imageType( ImageType imageType )
    {
      m_imageCreateInfo.imageType = static_cast<VkImageType>( imageType );
      return *this;
    }

    const Format& format() const
    {
      return reinterpret_cast<const Format&>( m_imageCreateInfo.format );
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

    ImageCreateInfo& extent( Extent3D extent )
    {
      m_imageCreateInfo.extent = static_cast<VkExtent3D>( extent );
      return *this;
    }

    const uint32_t& mipLevels() const
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

    ImageCreateInfo& arrayLayers( uint32_t arrayLayers )
    {
      m_imageCreateInfo.arrayLayers = arrayLayers;
      return *this;
    }

    const SampleCountFlagBits& samples() const
    {
      return reinterpret_cast<const SampleCountFlagBits&>( m_imageCreateInfo.samples );
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

    ImageCreateInfo& tiling( ImageTiling tiling )
    {
      m_imageCreateInfo.tiling = static_cast<VkImageTiling>( tiling );
      return *this;
    }

    const ImageUsageFlags& usage() const
    {
      return reinterpret_cast<const ImageUsageFlags&>( m_imageCreateInfo.usage );
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

    ImageCreateInfo& sharingMode( SharingMode sharingMode )
    {
      m_imageCreateInfo.sharingMode = static_cast<VkSharingMode>( sharingMode );
      return *this;
    }

    const uint32_t& queueFamilyIndexCount() const
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

    ImageCreateInfo& pQueueFamilyIndices( const uint32_t* pQueueFamilyIndices )
    {
      m_imageCreateInfo.pQueueFamilyIndices = pQueueFamilyIndices;
      return *this;
    }

    const ImageLayout& initialLayout() const
    {
      return reinterpret_cast<const ImageLayout&>( m_imageCreateInfo.initialLayout );
    }

    ImageCreateInfo& initialLayout( ImageLayout initialLayout )
    {
      m_imageCreateInfo.initialLayout = static_cast<VkImageLayout>( initialLayout );
      return *this;
    }

    operator const VkImageCreateInfo&() const
    {
      return m_imageCreateInfo;
    }

  private:
    VkImageCreateInfo m_imageCreateInfo;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_pipelineMultisampleStateCreateInfo.sType );
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

    PipelineMultisampleStateCreateInfo& pNext( const void* pNext )
    {
      m_pipelineMultisampleStateCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineMultisampleStateCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineMultisampleStateCreateFlags&>( m_pipelineMultisampleStateCreateInfo.flags );
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

    PipelineMultisampleStateCreateInfo& rasterizationSamples( SampleCountFlagBits rasterizationSamples )
    {
      m_pipelineMultisampleStateCreateInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>( rasterizationSamples );
      return *this;
    }

    const Bool32& sampleShadingEnable() const
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

    PipelineMultisampleStateCreateInfo& minSampleShading( float minSampleShading )
    {
      m_pipelineMultisampleStateCreateInfo.minSampleShading = minSampleShading;
      return *this;
    }

    const SampleMask* pSampleMask() const
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

    PipelineMultisampleStateCreateInfo& alphaToCoverageEnable( Bool32 alphaToCoverageEnable )
    {
      m_pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = alphaToCoverageEnable;
      return *this;
    }

    const Bool32& alphaToOneEnable() const
    {
      return m_pipelineMultisampleStateCreateInfo.alphaToOneEnable;
    }

    PipelineMultisampleStateCreateInfo& alphaToOneEnable( Bool32 alphaToOneEnable )
    {
      m_pipelineMultisampleStateCreateInfo.alphaToOneEnable = alphaToOneEnable;
      return *this;
    }

    operator const VkPipelineMultisampleStateCreateInfo&() const
    {
      return m_pipelineMultisampleStateCreateInfo;
    }

  private:
    VkPipelineMultisampleStateCreateInfo m_pipelineMultisampleStateCreateInfo;
  };

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
      m_graphicsPipelineCreateInfo.layout = layout;
      m_graphicsPipelineCreateInfo.renderPass = renderPass;
      m_graphicsPipelineCreateInfo.subpass = subpass;
      m_graphicsPipelineCreateInfo.basePipelineHandle = basePipelineHandle;
      m_graphicsPipelineCreateInfo.basePipelineIndex = basePipelineIndex;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_graphicsPipelineCreateInfo.sType );
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

    GraphicsPipelineCreateInfo& pNext( const void* pNext )
    {
      m_graphicsPipelineCreateInfo.pNext = pNext;
      return *this;
    }

    const PipelineCreateFlags& flags() const
    {
      return reinterpret_cast<const PipelineCreateFlags&>( m_graphicsPipelineCreateInfo.flags );
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

    GraphicsPipelineCreateInfo& stageCount( uint32_t stageCount )
    {
      m_graphicsPipelineCreateInfo.stageCount = stageCount;
      return *this;
    }

    const PipelineShaderStageCreateInfo* pStages() const
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

    GraphicsPipelineCreateInfo& pVertexInputState( const PipelineVertexInputStateCreateInfo* pVertexInputState )
    {
      m_graphicsPipelineCreateInfo.pVertexInputState = reinterpret_cast<const VkPipelineVertexInputStateCreateInfo*>( pVertexInputState );
      return *this;
    }

    const PipelineInputAssemblyStateCreateInfo* pInputAssemblyState() const
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

    GraphicsPipelineCreateInfo& pTessellationState( const PipelineTessellationStateCreateInfo* pTessellationState )
    {
      m_graphicsPipelineCreateInfo.pTessellationState = reinterpret_cast<const VkPipelineTessellationStateCreateInfo*>( pTessellationState );
      return *this;
    }

    const PipelineViewportStateCreateInfo* pViewportState() const
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

    GraphicsPipelineCreateInfo& pRasterizationState( const PipelineRasterizationStateCreateInfo* pRasterizationState )
    {
      m_graphicsPipelineCreateInfo.pRasterizationState = reinterpret_cast<const VkPipelineRasterizationStateCreateInfo*>( pRasterizationState );
      return *this;
    }

    const PipelineMultisampleStateCreateInfo* pMultisampleState() const
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

    GraphicsPipelineCreateInfo& pDepthStencilState( const PipelineDepthStencilStateCreateInfo* pDepthStencilState )
    {
      m_graphicsPipelineCreateInfo.pDepthStencilState = reinterpret_cast<const VkPipelineDepthStencilStateCreateInfo*>( pDepthStencilState );
      return *this;
    }

    const PipelineColorBlendStateCreateInfo* pColorBlendState() const
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

    GraphicsPipelineCreateInfo& pDynamicState( const PipelineDynamicStateCreateInfo* pDynamicState )
    {
      m_graphicsPipelineCreateInfo.pDynamicState = reinterpret_cast<const VkPipelineDynamicStateCreateInfo*>( pDynamicState );
      return *this;
    }

    const PipelineLayout& layout() const
    {
      return m_graphicsPipelineCreateInfo.layout;
    }

    GraphicsPipelineCreateInfo& layout( PipelineLayout layout )
    {
      m_graphicsPipelineCreateInfo.layout = layout;
      return *this;
    }

    const RenderPass& renderPass() const
    {
      return m_graphicsPipelineCreateInfo.renderPass;
    }

    GraphicsPipelineCreateInfo& renderPass( RenderPass renderPass )
    {
      m_graphicsPipelineCreateInfo.renderPass = renderPass;
      return *this;
    }

    const uint32_t& subpass() const
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
      return m_graphicsPipelineCreateInfo.basePipelineHandle;
    }

    GraphicsPipelineCreateInfo& basePipelineHandle( Pipeline basePipelineHandle )
    {
      m_graphicsPipelineCreateInfo.basePipelineHandle = basePipelineHandle;
      return *this;
    }

    const int32_t& basePipelineIndex() const
    {
      return m_graphicsPipelineCreateInfo.basePipelineIndex;
    }

    GraphicsPipelineCreateInfo& basePipelineIndex( int32_t basePipelineIndex )
    {
      m_graphicsPipelineCreateInfo.basePipelineIndex = basePipelineIndex;
      return *this;
    }

    operator const VkGraphicsPipelineCreateInfo&() const
    {
      return m_graphicsPipelineCreateInfo;
    }

  private:
    VkGraphicsPipelineCreateInfo m_graphicsPipelineCreateInfo;
  };

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

    operator const VkPhysicalDeviceLimits&() const
    {
      return m_physicalDeviceLimits;
    }

  private:
    VkPhysicalDeviceLimits m_physicalDeviceLimits;
  };

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

    operator const VkPhysicalDeviceProperties&() const
    {
      return m_physicalDeviceProperties;
    }

  private:
    VkPhysicalDeviceProperties m_physicalDeviceProperties;
  };

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

    const AttachmentDescriptionFlags& flags() const
    {
      return reinterpret_cast<const AttachmentDescriptionFlags&>( m_attachmentDescription.flags );
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

    AttachmentDescription& format( Format format )
    {
      m_attachmentDescription.format = static_cast<VkFormat>( format );
      return *this;
    }

    const SampleCountFlagBits& samples() const
    {
      return reinterpret_cast<const SampleCountFlagBits&>( m_attachmentDescription.samples );
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

    AttachmentDescription& loadOp( AttachmentLoadOp loadOp )
    {
      m_attachmentDescription.loadOp = static_cast<VkAttachmentLoadOp>( loadOp );
      return *this;
    }

    const AttachmentStoreOp& storeOp() const
    {
      return reinterpret_cast<const AttachmentStoreOp&>( m_attachmentDescription.storeOp );
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

    AttachmentDescription& stencilLoadOp( AttachmentLoadOp stencilLoadOp )
    {
      m_attachmentDescription.stencilLoadOp = static_cast<VkAttachmentLoadOp>( stencilLoadOp );
      return *this;
    }

    const AttachmentStoreOp& stencilStoreOp() const
    {
      return reinterpret_cast<const AttachmentStoreOp&>( m_attachmentDescription.stencilStoreOp );
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

    AttachmentDescription& initialLayout( ImageLayout initialLayout )
    {
      m_attachmentDescription.initialLayout = static_cast<VkImageLayout>( initialLayout );
      return *this;
    }

    const ImageLayout& finalLayout() const
    {
      return reinterpret_cast<const ImageLayout&>( m_attachmentDescription.finalLayout );
    }

    AttachmentDescription& finalLayout( ImageLayout finalLayout )
    {
      m_attachmentDescription.finalLayout = static_cast<VkImageLayout>( finalLayout );
      return *this;
    }

    operator const VkAttachmentDescription&() const
    {
      return m_attachmentDescription;
    }

  private:
    VkAttachmentDescription m_attachmentDescription;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_descriptorPoolCreateInfo.sType );
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

    DescriptorPoolCreateInfo& pNext( const void* pNext )
    {
      m_descriptorPoolCreateInfo.pNext = pNext;
      return *this;
    }

    const DescriptorPoolCreateFlags& flags() const
    {
      return reinterpret_cast<const DescriptorPoolCreateFlags&>( m_descriptorPoolCreateInfo.flags );
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

    DescriptorPoolCreateInfo& maxSets( uint32_t maxSets )
    {
      m_descriptorPoolCreateInfo.maxSets = maxSets;
      return *this;
    }

    const uint32_t& poolSizeCount() const
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

    DescriptorPoolCreateInfo& pPoolSizes( const DescriptorPoolSize* pPoolSizes )
    {
      m_descriptorPoolCreateInfo.pPoolSizes = reinterpret_cast<const VkDescriptorPoolSize*>( pPoolSizes );
      return *this;
    }

    operator const VkDescriptorPoolCreateInfo&() const
    {
      return m_descriptorPoolCreateInfo;
    }

  private:
    VkDescriptorPoolCreateInfo m_descriptorPoolCreateInfo;
  };

  enum class DependencyFlagBits
  {
    eByRegion = VK_DEPENDENCY_BY_REGION_BIT
  };

  typedef Flags<DependencyFlagBits, VkDependencyFlags> DependencyFlags;

  inline DependencyFlags operator|( DependencyFlagBits bit0, DependencyFlagBits bit1 )
  {
    return DependencyFlags( bit0 ) | bit1;
  }

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

    const uint32_t& srcSubpass() const
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

    SubpassDependency& dstSubpass( uint32_t dstSubpass )
    {
      m_subpassDependency.dstSubpass = dstSubpass;
      return *this;
    }

    const PipelineStageFlags& srcStageMask() const
    {
      return reinterpret_cast<const PipelineStageFlags&>( m_subpassDependency.srcStageMask );
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

    SubpassDependency& dstStageMask( PipelineStageFlags dstStageMask )
    {
      m_subpassDependency.dstStageMask = static_cast<VkPipelineStageFlags>( dstStageMask );
      return *this;
    }

    const AccessFlags& srcAccessMask() const
    {
      return reinterpret_cast<const AccessFlags&>( m_subpassDependency.srcAccessMask );
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

    SubpassDependency& dstAccessMask( AccessFlags dstAccessMask )
    {
      m_subpassDependency.dstAccessMask = static_cast<VkAccessFlags>( dstAccessMask );
      return *this;
    }

    const DependencyFlags& dependencyFlags() const
    {
      return reinterpret_cast<const DependencyFlags&>( m_subpassDependency.dependencyFlags );
    }

    SubpassDependency& dependencyFlags( DependencyFlags dependencyFlags )
    {
      m_subpassDependency.dependencyFlags = static_cast<VkDependencyFlags>( dependencyFlags );
      return *this;
    }

    operator const VkSubpassDependency&() const
    {
      return m_subpassDependency;
    }

  private:
    VkSubpassDependency m_subpassDependency;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_renderPassCreateInfo.sType );
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

    RenderPassCreateInfo& pNext( const void* pNext )
    {
      m_renderPassCreateInfo.pNext = pNext;
      return *this;
    }

    const RenderPassCreateFlags& flags() const
    {
      return reinterpret_cast<const RenderPassCreateFlags&>( m_renderPassCreateInfo.flags );
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

    RenderPassCreateInfo& attachmentCount( uint32_t attachmentCount )
    {
      m_renderPassCreateInfo.attachmentCount = attachmentCount;
      return *this;
    }

    const AttachmentDescription* pAttachments() const
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

    RenderPassCreateInfo& subpassCount( uint32_t subpassCount )
    {
      m_renderPassCreateInfo.subpassCount = subpassCount;
      return *this;
    }

    const SubpassDescription* pSubpasses() const
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

    RenderPassCreateInfo& dependencyCount( uint32_t dependencyCount )
    {
      m_renderPassCreateInfo.dependencyCount = dependencyCount;
      return *this;
    }

    const SubpassDependency* pDependencies() const
    {
      return reinterpret_cast<const SubpassDependency*>( m_renderPassCreateInfo.pDependencies );
    }

    RenderPassCreateInfo& pDependencies( const SubpassDependency* pDependencies )
    {
      m_renderPassCreateInfo.pDependencies = reinterpret_cast<const VkSubpassDependency*>( pDependencies );
      return *this;
    }

    operator const VkRenderPassCreateInfo&() const
    {
      return m_renderPassCreateInfo;
    }

  private:
    VkRenderPassCreateInfo m_renderPassCreateInfo;
  };

  enum class PresentModeKHR
  {
    eVkPresentModeImmediateKhr = VK_PRESENT_MODE_IMMEDIATE_KHR,
    eVkPresentModeMailboxKhr = VK_PRESENT_MODE_MAILBOX_KHR,
    eVkPresentModeFifoKhr = VK_PRESENT_MODE_FIFO_KHR,
    eVkPresentModeFifoRelaxedKhr = VK_PRESENT_MODE_FIFO_RELAXED_KHR
  };

  enum class ColorSpaceKHR
  {
    eVkColorspaceSrgbNonlinearKhr = VK_COLORSPACE_SRGB_NONLINEAR_KHR
  };

  class SurfaceFormatKHR
  {
  public:
    SurfaceFormatKHR()
      : SurfaceFormatKHR( Format::eUndefined, ColorSpaceKHR::eVkColorspaceSrgbNonlinearKhr )
    {}

    SurfaceFormatKHR( Format format, ColorSpaceKHR colorSpace)
    {
      m_surfaceFormatKHR.format = static_cast<VkFormat>( format );
      m_surfaceFormatKHR.colorSpace = static_cast<VkColorSpaceKHR>( colorSpace );
    }

    const Format& format() const
    {
      return reinterpret_cast<const Format&>( m_surfaceFormatKHR.format );
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

    SurfaceFormatKHR& colorSpace( ColorSpaceKHR colorSpace )
    {
      m_surfaceFormatKHR.colorSpace = static_cast<VkColorSpaceKHR>( colorSpace );
      return *this;
    }

    operator const VkSurfaceFormatKHR&() const
    {
      return m_surfaceFormatKHR;
    }

  private:
    VkSurfaceFormatKHR m_surfaceFormatKHR;
  };

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

    const uint32_t& minImageCount() const
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

    SurfaceCapabilitiesKHR& maxImageCount( uint32_t maxImageCount )
    {
      m_surfaceCapabilitiesKHR.maxImageCount = maxImageCount;
      return *this;
    }

    const Extent2D& currentExtent() const
    {
      return reinterpret_cast<const Extent2D&>( m_surfaceCapabilitiesKHR.currentExtent );
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

    SurfaceCapabilitiesKHR& minImageExtent( Extent2D minImageExtent )
    {
      m_surfaceCapabilitiesKHR.minImageExtent = static_cast<VkExtent2D>( minImageExtent );
      return *this;
    }

    const Extent2D& maxImageExtent() const
    {
      return reinterpret_cast<const Extent2D&>( m_surfaceCapabilitiesKHR.maxImageExtent );
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

    SurfaceCapabilitiesKHR& maxImageArrayLayers( uint32_t maxImageArrayLayers )
    {
      m_surfaceCapabilitiesKHR.maxImageArrayLayers = maxImageArrayLayers;
      return *this;
    }

    const SurfaceTransformFlagsKHR& supportedTransforms() const
    {
      return reinterpret_cast<const SurfaceTransformFlagsKHR&>( m_surfaceCapabilitiesKHR.supportedTransforms );
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

    SurfaceCapabilitiesKHR& currentTransform( SurfaceTransformFlagBitsKHR currentTransform )
    {
      m_surfaceCapabilitiesKHR.currentTransform = static_cast<VkSurfaceTransformFlagBitsKHR>( currentTransform );
      return *this;
    }

    const CompositeAlphaFlagsKHR& supportedCompositeAlpha() const
    {
      return reinterpret_cast<const CompositeAlphaFlagsKHR&>( m_surfaceCapabilitiesKHR.supportedCompositeAlpha );
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

    SurfaceCapabilitiesKHR& supportedUsageFlags( ImageUsageFlags supportedUsageFlags )
    {
      m_surfaceCapabilitiesKHR.supportedUsageFlags = static_cast<VkImageUsageFlags>( supportedUsageFlags );
      return *this;
    }

    operator const VkSurfaceCapabilitiesKHR&() const
    {
      return m_surfaceCapabilitiesKHR;
    }

  private:
    VkSurfaceCapabilitiesKHR m_surfaceCapabilitiesKHR;
  };

  class SwapchainCreateInfoKHR
  {
  public:
    SwapchainCreateInfoKHR()
      : SwapchainCreateInfoKHR( SwapchainCreateFlagsKHR(), SurfaceKHR(), 0, Format::eUndefined, ColorSpaceKHR::eVkColorspaceSrgbNonlinearKhr, Extent2D(), 0, ImageUsageFlags(), SharingMode::eExclusive, 0, nullptr, SurfaceTransformFlagBitsKHR::eIdentity, CompositeAlphaFlagBitsKHR::eOpaque, PresentModeKHR::eVkPresentModeImmediateKhr, 0, SwapchainKHR() )
    {}

    SwapchainCreateInfoKHR( SwapchainCreateFlagsKHR flags, SurfaceKHR surface, uint32_t minImageCount, Format imageFormat, ColorSpaceKHR imageColorSpace, Extent2D imageExtent, uint32_t imageArrayLayers, ImageUsageFlags imageUsage, SharingMode imageSharingMode, uint32_t queueFamilyIndexCount, const uint32_t* pQueueFamilyIndices, SurfaceTransformFlagBitsKHR preTransform, CompositeAlphaFlagBitsKHR compositeAlpha, PresentModeKHR presentMode, Bool32 clipped, SwapchainKHR oldSwapchain)
    {
      m_swapchainCreateInfoKHR.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      m_swapchainCreateInfoKHR.pNext = nullptr;
      m_swapchainCreateInfoKHR.flags = static_cast<VkSwapchainCreateFlagsKHR>( flags );
      m_swapchainCreateInfoKHR.surface = surface;
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
      m_swapchainCreateInfoKHR.oldSwapchain = oldSwapchain;
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_swapchainCreateInfoKHR.sType );
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

    SwapchainCreateInfoKHR& pNext( const void* pNext )
    {
      m_swapchainCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const SwapchainCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const SwapchainCreateFlagsKHR&>( m_swapchainCreateInfoKHR.flags );
    }

    SwapchainCreateInfoKHR& flags( SwapchainCreateFlagsKHR flags )
    {
      m_swapchainCreateInfoKHR.flags = static_cast<VkSwapchainCreateFlagsKHR>( flags );
      return *this;
    }

    const SurfaceKHR& surface() const
    {
      return m_swapchainCreateInfoKHR.surface;
    }

    SwapchainCreateInfoKHR& surface( SurfaceKHR surface )
    {
      m_swapchainCreateInfoKHR.surface = surface;
      return *this;
    }

    const uint32_t& minImageCount() const
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

    SwapchainCreateInfoKHR& imageFormat( Format imageFormat )
    {
      m_swapchainCreateInfoKHR.imageFormat = static_cast<VkFormat>( imageFormat );
      return *this;
    }

    const ColorSpaceKHR& imageColorSpace() const
    {
      return reinterpret_cast<const ColorSpaceKHR&>( m_swapchainCreateInfoKHR.imageColorSpace );
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

    SwapchainCreateInfoKHR& imageExtent( Extent2D imageExtent )
    {
      m_swapchainCreateInfoKHR.imageExtent = static_cast<VkExtent2D>( imageExtent );
      return *this;
    }

    const uint32_t& imageArrayLayers() const
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

    SwapchainCreateInfoKHR& imageUsage( ImageUsageFlags imageUsage )
    {
      m_swapchainCreateInfoKHR.imageUsage = static_cast<VkImageUsageFlags>( imageUsage );
      return *this;
    }

    const SharingMode& imageSharingMode() const
    {
      return reinterpret_cast<const SharingMode&>( m_swapchainCreateInfoKHR.imageSharingMode );
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

    SwapchainCreateInfoKHR& queueFamilyIndexCount( uint32_t queueFamilyIndexCount )
    {
      m_swapchainCreateInfoKHR.queueFamilyIndexCount = queueFamilyIndexCount;
      return *this;
    }

    const uint32_t* pQueueFamilyIndices() const
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

    SwapchainCreateInfoKHR& preTransform( SurfaceTransformFlagBitsKHR preTransform )
    {
      m_swapchainCreateInfoKHR.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>( preTransform );
      return *this;
    }

    const CompositeAlphaFlagBitsKHR& compositeAlpha() const
    {
      return reinterpret_cast<const CompositeAlphaFlagBitsKHR&>( m_swapchainCreateInfoKHR.compositeAlpha );
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

    SwapchainCreateInfoKHR& presentMode( PresentModeKHR presentMode )
    {
      m_swapchainCreateInfoKHR.presentMode = static_cast<VkPresentModeKHR>( presentMode );
      return *this;
    }

    const Bool32& clipped() const
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
      return m_swapchainCreateInfoKHR.oldSwapchain;
    }

    SwapchainCreateInfoKHR& oldSwapchain( SwapchainKHR oldSwapchain )
    {
      m_swapchainCreateInfoKHR.oldSwapchain = oldSwapchain;
      return *this;
    }

    operator const VkSwapchainCreateInfoKHR&() const
    {
      return m_swapchainCreateInfoKHR;
    }

  private:
    VkSwapchainCreateInfoKHR m_swapchainCreateInfoKHR;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_debugReportCallbackCreateInfoEXT.sType );
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

    DebugReportCallbackCreateInfoEXT& pNext( const void* pNext )
    {
      m_debugReportCallbackCreateInfoEXT.pNext = pNext;
      return *this;
    }

    const DebugReportFlagsEXT& flags() const
    {
      return reinterpret_cast<const DebugReportFlagsEXT&>( m_debugReportCallbackCreateInfoEXT.flags );
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

    DebugReportCallbackCreateInfoEXT& pfnCallback( PFN_vkDebugReportCallbackEXT pfnCallback )
    {
      m_debugReportCallbackCreateInfoEXT.pfnCallback = pfnCallback;
      return *this;
    }

    const void* pUserData() const
    {
      return reinterpret_cast<void*>( m_debugReportCallbackCreateInfoEXT.pUserData );
    }

    DebugReportCallbackCreateInfoEXT& pUserData( void* pUserData )
    {
      m_debugReportCallbackCreateInfoEXT.pUserData = pUserData;
      return *this;
    }

    operator const VkDebugReportCallbackCreateInfoEXT&() const
    {
      return m_debugReportCallbackCreateInfoEXT;
    }

  private:
    VkDebugReportCallbackCreateInfoEXT m_debugReportCallbackCreateInfoEXT;
  };

  inline Result createInstance( const InstanceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Instance* pInstance )
  {
    return static_cast<Result>( vkCreateInstance( reinterpret_cast<const VkInstanceCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pInstance ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createInstance( const InstanceCreateInfo& createInfo, const AllocationCallbacks& allocator, Instance& instance )
  {
    return createInstance( &createInfo, &allocator, &instance );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyInstance( Instance instance, const AllocationCallbacks* pAllocator )
  {
    vkDestroyInstance( instance, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyInstance( Instance instance, const AllocationCallbacks& allocator )
  {
    destroyInstance( instance, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result enumeratePhysicalDevices( Instance instance, uint32_t* pPhysicalDeviceCount, PhysicalDevice* pPhysicalDevices )
  {
    return static_cast<Result>( vkEnumeratePhysicalDevices( instance, pPhysicalDeviceCount, pPhysicalDevices ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result enumeratePhysicalDevices( Instance instance, std::vector<PhysicalDevice> & physicalDevices )
  {
    uint32_t pPhysicalDeviceCount = 0;
    Result result = enumeratePhysicalDevices( instance, &pPhysicalDeviceCount, nullptr );
    if ( result == Result::eVkSuccess )
    {
      physicalDevices.resize( pPhysicalDeviceCount );
      result = enumeratePhysicalDevices( instance, &pPhysicalDeviceCount, physicalDevices.data() );
    }
    return result;
  }
#endif    // VKCPP_ENHANCED_MODE

  inline PFN_vkVoidFunction getDeviceProcAddr( Device device, const char* pName )
  {
    return vkGetDeviceProcAddr( device, pName );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline PFN_vkVoidFunction getDeviceProcAddr( Device device, std::string const&  name )
  {
    return getDeviceProcAddr( device, name.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline PFN_vkVoidFunction getInstanceProcAddr( Instance instance, const char* pName )
  {
    return vkGetInstanceProcAddr( instance, pName );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline PFN_vkVoidFunction getInstanceProcAddr( Instance instance, std::string const&  name )
  {
    return getInstanceProcAddr( instance, name.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void getPhysicalDeviceProperties( PhysicalDevice physicalDevice, PhysicalDeviceProperties* pProperties )
  {
    vkGetPhysicalDeviceProperties( physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties*>( pProperties ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void getPhysicalDeviceProperties( PhysicalDevice physicalDevice, PhysicalDeviceProperties& properties )
  {
    getPhysicalDeviceProperties( physicalDevice, &properties );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void getPhysicalDeviceQueueFamilyProperties( PhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, QueueFamilyProperties* pQueueFamilyProperties )
  {
    vkGetPhysicalDeviceQueueFamilyProperties( physicalDevice, pQueueFamilyPropertyCount, reinterpret_cast<VkQueueFamilyProperties*>( pQueueFamilyProperties ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline std::vector<QueueFamilyProperties> getPhysicalDeviceQueueFamilyProperties( PhysicalDevice physicalDevice )
  {
    uint32_t pQueueFamilyPropertyCount = 0;
    getPhysicalDeviceQueueFamilyProperties( physicalDevice, &pQueueFamilyPropertyCount, nullptr );
    std::vector<QueueFamilyProperties> pQueueFamilyProperties( pQueueFamilyPropertyCount );
    getPhysicalDeviceQueueFamilyProperties( physicalDevice, &pQueueFamilyPropertyCount, pQueueFamilyProperties.data() );
    return std::move( pQueueFamilyProperties );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void getPhysicalDeviceMemoryProperties( PhysicalDevice physicalDevice, PhysicalDeviceMemoryProperties* pMemoryProperties )
  {
    vkGetPhysicalDeviceMemoryProperties( physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties*>( pMemoryProperties ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void getPhysicalDeviceMemoryProperties( PhysicalDevice physicalDevice, PhysicalDeviceMemoryProperties& memoryProperties )
  {
    getPhysicalDeviceMemoryProperties( physicalDevice, &memoryProperties );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void getPhysicalDeviceFeatures( PhysicalDevice physicalDevice, PhysicalDeviceFeatures* pFeatures )
  {
    vkGetPhysicalDeviceFeatures( physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures*>( pFeatures ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void getPhysicalDeviceFeatures( PhysicalDevice physicalDevice, PhysicalDeviceFeatures& features )
  {
    getPhysicalDeviceFeatures( physicalDevice, &features );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void getPhysicalDeviceFormatProperties( PhysicalDevice physicalDevice, Format format, FormatProperties* pFormatProperties )
  {
    vkGetPhysicalDeviceFormatProperties( physicalDevice, static_cast<VkFormat>( format ), reinterpret_cast<VkFormatProperties*>( pFormatProperties ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void getPhysicalDeviceFormatProperties( PhysicalDevice physicalDevice, Format format, FormatProperties& formatProperties )
  {
    getPhysicalDeviceFormatProperties( physicalDevice, format, &formatProperties );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getPhysicalDeviceImageFormatProperties( PhysicalDevice physicalDevice, Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ImageFormatProperties* pImageFormatProperties )
  {
    return static_cast<Result>( vkGetPhysicalDeviceImageFormatProperties( physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkImageTiling>( tiling ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageCreateFlags>( flags ), reinterpret_cast<VkImageFormatProperties*>( pImageFormatProperties ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result getPhysicalDeviceImageFormatProperties( PhysicalDevice physicalDevice, Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ImageFormatProperties& imageFormatProperties )
  {
    return getPhysicalDeviceImageFormatProperties( physicalDevice, format, type, tiling, usage, flags, &imageFormatProperties );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createDevice( PhysicalDevice physicalDevice, const DeviceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Device* pDevice )
  {
    return static_cast<Result>( vkCreateDevice( physicalDevice, reinterpret_cast<const VkDeviceCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pDevice ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createDevice( PhysicalDevice physicalDevice, const DeviceCreateInfo& createInfo, const AllocationCallbacks& allocator, Device& device )
  {
    return createDevice( physicalDevice, &createInfo, &allocator, &device );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyDevice( Device device, const AllocationCallbacks* pAllocator )
  {
    vkDestroyDevice( device, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyDevice( Device device, const AllocationCallbacks& allocator )
  {
    destroyDevice( device, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result enumerateInstanceLayerProperties( uint32_t* pPropertyCount, LayerProperties* pProperties )
  {
    return static_cast<Result>( vkEnumerateInstanceLayerProperties( pPropertyCount, reinterpret_cast<VkLayerProperties*>( pProperties ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result enumerateInstanceLayerProperties( std::vector<LayerProperties> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = enumerateInstanceLayerProperties( &pPropertyCount, nullptr );
    if ( result == Result::eVkSuccess )
    {
      properties.resize( pPropertyCount );
      result = enumerateInstanceLayerProperties( &pPropertyCount, properties.data() );
    }
    return result;
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result enumerateInstanceExtensionProperties( const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties )
  {
    return static_cast<Result>( vkEnumerateInstanceExtensionProperties( pLayerName, pPropertyCount, reinterpret_cast<VkExtensionProperties*>( pProperties ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result enumerateInstanceExtensionProperties( std::string const&  layerName, std::vector<ExtensionProperties> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = enumerateInstanceExtensionProperties( layerName.data(), &pPropertyCount, nullptr );
    if ( result == Result::eVkSuccess )
    {
      properties.resize( pPropertyCount );
      result = enumerateInstanceExtensionProperties( layerName.data(), &pPropertyCount, properties.data() );
    }
    return result;
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result enumerateDeviceLayerProperties( PhysicalDevice physicalDevice, uint32_t* pPropertyCount, LayerProperties* pProperties )
  {
    return static_cast<Result>( vkEnumerateDeviceLayerProperties( physicalDevice, pPropertyCount, reinterpret_cast<VkLayerProperties*>( pProperties ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result enumerateDeviceLayerProperties( PhysicalDevice physicalDevice, std::vector<LayerProperties> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = enumerateDeviceLayerProperties( physicalDevice, &pPropertyCount, nullptr );
    if ( result == Result::eVkSuccess )
    {
      properties.resize( pPropertyCount );
      result = enumerateDeviceLayerProperties( physicalDevice, &pPropertyCount, properties.data() );
    }
    return result;
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result enumerateDeviceExtensionProperties( PhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties )
  {
    return static_cast<Result>( vkEnumerateDeviceExtensionProperties( physicalDevice, pLayerName, pPropertyCount, reinterpret_cast<VkExtensionProperties*>( pProperties ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result enumerateDeviceExtensionProperties( PhysicalDevice physicalDevice, std::string const&  layerName, std::vector<ExtensionProperties> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = enumerateDeviceExtensionProperties( physicalDevice, layerName.data(), &pPropertyCount, nullptr );
    if ( result == Result::eVkSuccess )
    {
      properties.resize( pPropertyCount );
      result = enumerateDeviceExtensionProperties( physicalDevice, layerName.data(), &pPropertyCount, properties.data() );
    }
    return result;
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void getDeviceQueue( Device device, uint32_t queueFamilyIndex, uint32_t queueIndex, Queue* pQueue )
  {
    vkGetDeviceQueue( device, queueFamilyIndex, queueIndex, pQueue );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void getDeviceQueue( Device device, uint32_t queueFamilyIndex, uint32_t queueIndex, Queue& queue )
  {
    getDeviceQueue( device, queueFamilyIndex, queueIndex, &queue );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result queueSubmit( Queue queue, uint32_t submitCount, const SubmitInfo* pSubmits, Fence fence )
  {
    return static_cast<Result>( vkQueueSubmit( queue, submitCount, reinterpret_cast<const VkSubmitInfo*>( pSubmits ), fence ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result queueSubmit( Queue queue, std::vector<SubmitInfo> const& submits, Fence fence )
  {
    return queueSubmit( queue, static_cast<uint32_t>( submits.size() ), submits.data(), fence );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result queueWaitIdle( Queue queue )
  {
    return static_cast<Result>( vkQueueWaitIdle( queue ) );
  }

  inline Result deviceWaitIdle( Device device )
  {
    return static_cast<Result>( vkDeviceWaitIdle( device ) );
  }

  inline Result allocateMemory( Device device, const MemoryAllocateInfo* pAllocateInfo, const AllocationCallbacks* pAllocator, DeviceMemory* pMemory )
  {
    return static_cast<Result>( vkAllocateMemory( device, reinterpret_cast<const VkMemoryAllocateInfo*>( pAllocateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pMemory ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result allocateMemory( Device device, const MemoryAllocateInfo& allocateInfo, const AllocationCallbacks& allocator, DeviceMemory& memory )
  {
    return allocateMemory( device, &allocateInfo, &allocator, &memory );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void freeMemory( Device device, DeviceMemory memory, const AllocationCallbacks* pAllocator )
  {
    vkFreeMemory( device, memory, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void freeMemory( Device device, DeviceMemory memory, const AllocationCallbacks& allocator )
  {
    freeMemory( device, memory, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result mapMemory( Device device, DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags, void** ppData )
  {
    return static_cast<Result>( vkMapMemory( device, memory, offset, size, static_cast<VkMemoryMapFlags>( flags ), ppData ) );
  }

  inline void unmapMemory( Device device, DeviceMemory memory )
  {
    vkUnmapMemory( device, memory );
  }

  inline Result flushMappedMemoryRanges( Device device, uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges )
  {
    return static_cast<Result>( vkFlushMappedMemoryRanges( device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>( pMemoryRanges ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result flushMappedMemoryRanges( Device device, std::vector<MappedMemoryRange> const& memoryRanges )
  {
    return flushMappedMemoryRanges( device, static_cast<uint32_t>( memoryRanges.size() ), memoryRanges.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result invalidateMappedMemoryRanges( Device device, uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges )
  {
    return static_cast<Result>( vkInvalidateMappedMemoryRanges( device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>( pMemoryRanges ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result invalidateMappedMemoryRanges( Device device, std::vector<MappedMemoryRange> const& memoryRanges )
  {
    return invalidateMappedMemoryRanges( device, static_cast<uint32_t>( memoryRanges.size() ), memoryRanges.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void getDeviceMemoryCommitment( Device device, DeviceMemory memory, DeviceSize* pCommittedMemoryInBytes )
  {
    vkGetDeviceMemoryCommitment( device, memory, pCommittedMemoryInBytes );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void getDeviceMemoryCommitment( Device device, DeviceMemory memory, DeviceSize& committedMemoryInBytes )
  {
    getDeviceMemoryCommitment( device, memory, &committedMemoryInBytes );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void getBufferMemoryRequirements( Device device, Buffer buffer, MemoryRequirements* pMemoryRequirements )
  {
    vkGetBufferMemoryRequirements( device, buffer, reinterpret_cast<VkMemoryRequirements*>( pMemoryRequirements ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void getBufferMemoryRequirements( Device device, Buffer buffer, MemoryRequirements& memoryRequirements )
  {
    getBufferMemoryRequirements( device, buffer, &memoryRequirements );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result bindBufferMemory( Device device, Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset )
  {
    return static_cast<Result>( vkBindBufferMemory( device, buffer, memory, memoryOffset ) );
  }

  inline void getImageMemoryRequirements( Device device, Image image, MemoryRequirements* pMemoryRequirements )
  {
    vkGetImageMemoryRequirements( device, image, reinterpret_cast<VkMemoryRequirements*>( pMemoryRequirements ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void getImageMemoryRequirements( Device device, Image image, MemoryRequirements& memoryRequirements )
  {
    getImageMemoryRequirements( device, image, &memoryRequirements );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result bindImageMemory( Device device, Image image, DeviceMemory memory, DeviceSize memoryOffset )
  {
    return static_cast<Result>( vkBindImageMemory( device, image, memory, memoryOffset ) );
  }

  inline void getImageSparseMemoryRequirements( Device device, Image image, uint32_t* pSparseMemoryRequirementCount, SparseImageMemoryRequirements* pSparseMemoryRequirements )
  {
    vkGetImageSparseMemoryRequirements( device, image, pSparseMemoryRequirementCount, reinterpret_cast<VkSparseImageMemoryRequirements*>( pSparseMemoryRequirements ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline std::vector<SparseImageMemoryRequirements> getImageSparseMemoryRequirements( Device device, Image image )
  {
    uint32_t pSparseMemoryRequirementCount = 0;
    getImageSparseMemoryRequirements( device, image, &pSparseMemoryRequirementCount, nullptr );
    std::vector<SparseImageMemoryRequirements> pSparseMemoryRequirements( pSparseMemoryRequirementCount );
    getImageSparseMemoryRequirements( device, image, &pSparseMemoryRequirementCount, pSparseMemoryRequirements.data() );
    return std::move( pSparseMemoryRequirements );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void getPhysicalDeviceSparseImageFormatProperties( PhysicalDevice physicalDevice, Format format, ImageType type, SampleCountFlagBits samples, ImageUsageFlags usage, ImageTiling tiling, uint32_t* pPropertyCount, SparseImageFormatProperties* pProperties )
  {
    vkGetPhysicalDeviceSparseImageFormatProperties( physicalDevice, static_cast<VkFormat>( format ), static_cast<VkImageType>( type ), static_cast<VkSampleCountFlagBits>( samples ), static_cast<VkImageUsageFlags>( usage ), static_cast<VkImageTiling>( tiling ), pPropertyCount, reinterpret_cast<VkSparseImageFormatProperties*>( pProperties ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline std::vector<SparseImageFormatProperties> getPhysicalDeviceSparseImageFormatProperties( PhysicalDevice physicalDevice, Format format, ImageType type, SampleCountFlagBits samples, ImageUsageFlags usage, ImageTiling tiling )
  {
    uint32_t pPropertyCount = 0;
    getPhysicalDeviceSparseImageFormatProperties( physicalDevice, format, type, samples, usage, tiling, &pPropertyCount, nullptr );
    std::vector<SparseImageFormatProperties> pProperties( pPropertyCount );
    getPhysicalDeviceSparseImageFormatProperties( physicalDevice, format, type, samples, usage, tiling, &pPropertyCount, pProperties.data() );
    return std::move( pProperties );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result queueBindSparse( Queue queue, uint32_t bindInfoCount, const BindSparseInfo* pBindInfo, Fence fence )
  {
    return static_cast<Result>( vkQueueBindSparse( queue, bindInfoCount, reinterpret_cast<const VkBindSparseInfo*>( pBindInfo ), fence ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result queueBindSparse( Queue queue, std::vector<BindSparseInfo> const& bindInfo, Fence fence )
  {
    return queueBindSparse( queue, static_cast<uint32_t>( bindInfo.size() ), bindInfo.data(), fence );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createFence( Device device, const FenceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Fence* pFence )
  {
    return static_cast<Result>( vkCreateFence( device, reinterpret_cast<const VkFenceCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pFence ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createFence( Device device, const FenceCreateInfo& createInfo, const AllocationCallbacks& allocator, Fence& fence )
  {
    return createFence( device, &createInfo, &allocator, &fence );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyFence( Device device, Fence fence, const AllocationCallbacks* pAllocator )
  {
    vkDestroyFence( device, fence, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyFence( Device device, Fence fence, const AllocationCallbacks& allocator )
  {
    destroyFence( device, fence, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result resetFences( Device device, uint32_t fenceCount, const Fence* pFences )
  {
    return static_cast<Result>( vkResetFences( device, fenceCount, pFences ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result resetFences( Device device, std::vector<Fence> const& fences )
  {
    return resetFences( device, static_cast<uint32_t>( fences.size() ), fences.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getFenceStatus( Device device, Fence fence )
  {
    return static_cast<Result>( vkGetFenceStatus( device, fence ) );
  }

  inline Result waitForFences( Device device, uint32_t fenceCount, const Fence* pFences, Bool32 waitAll, uint64_t timeout )
  {
    return static_cast<Result>( vkWaitForFences( device, fenceCount, pFences, waitAll, timeout ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result waitForFences( Device device, std::vector<Fence> const& fences, Bool32 waitAll, uint64_t timeout )
  {
    return waitForFences( device, static_cast<uint32_t>( fences.size() ), fences.data(), waitAll, timeout );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createSemaphore( Device device, const SemaphoreCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Semaphore* pSemaphore )
  {
    return static_cast<Result>( vkCreateSemaphore( device, reinterpret_cast<const VkSemaphoreCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSemaphore ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createSemaphore( Device device, const SemaphoreCreateInfo& createInfo, const AllocationCallbacks& allocator, Semaphore& semaphore )
  {
    return createSemaphore( device, &createInfo, &allocator, &semaphore );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroySemaphore( Device device, Semaphore semaphore, const AllocationCallbacks* pAllocator )
  {
    vkDestroySemaphore( device, semaphore, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroySemaphore( Device device, Semaphore semaphore, const AllocationCallbacks& allocator )
  {
    destroySemaphore( device, semaphore, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createEvent( Device device, const EventCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Event* pEvent )
  {
    return static_cast<Result>( vkCreateEvent( device, reinterpret_cast<const VkEventCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pEvent ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createEvent( Device device, const EventCreateInfo& createInfo, const AllocationCallbacks& allocator, Event& event )
  {
    return createEvent( device, &createInfo, &allocator, &event );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyEvent( Device device, Event event, const AllocationCallbacks* pAllocator )
  {
    vkDestroyEvent( device, event, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyEvent( Device device, Event event, const AllocationCallbacks& allocator )
  {
    destroyEvent( device, event, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getEventStatus( Device device, Event event )
  {
    return static_cast<Result>( vkGetEventStatus( device, event ) );
  }

  inline Result setEvent( Device device, Event event )
  {
    return static_cast<Result>( vkSetEvent( device, event ) );
  }

  inline Result resetEvent( Device device, Event event )
  {
    return static_cast<Result>( vkResetEvent( device, event ) );
  }

  inline Result createQueryPool( Device device, const QueryPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, QueryPool* pQueryPool )
  {
    return static_cast<Result>( vkCreateQueryPool( device, reinterpret_cast<const VkQueryPoolCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pQueryPool ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createQueryPool( Device device, const QueryPoolCreateInfo& createInfo, const AllocationCallbacks& allocator, QueryPool& queryPool )
  {
    return createQueryPool( device, &createInfo, &allocator, &queryPool );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyQueryPool( Device device, QueryPool queryPool, const AllocationCallbacks* pAllocator )
  {
    vkDestroyQueryPool( device, queryPool, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyQueryPool( Device device, QueryPool queryPool, const AllocationCallbacks& allocator )
  {
    destroyQueryPool( device, queryPool, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getQueryPoolResults( Device device, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, DeviceSize stride, QueryResultFlags flags )
  {
    return static_cast<Result>( vkGetQueryPoolResults( device, queryPool, firstQuery, queryCount, dataSize, pData, stride, static_cast<VkQueryResultFlags>( flags ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result getQueryPoolResults( Device device, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, std::vector<uint8_t> & data, DeviceSize stride, QueryResultFlags flags )
  {
    return getQueryPoolResults( device, queryPool, firstQuery, queryCount, static_cast<size_t>( data.size() ), data.data(), stride, flags );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createBuffer( Device device, const BufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Buffer* pBuffer )
  {
    return static_cast<Result>( vkCreateBuffer( device, reinterpret_cast<const VkBufferCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pBuffer ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createBuffer( Device device, const BufferCreateInfo& createInfo, const AllocationCallbacks& allocator, Buffer& buffer )
  {
    return createBuffer( device, &createInfo, &allocator, &buffer );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyBuffer( Device device, Buffer buffer, const AllocationCallbacks* pAllocator )
  {
    vkDestroyBuffer( device, buffer, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyBuffer( Device device, Buffer buffer, const AllocationCallbacks& allocator )
  {
    destroyBuffer( device, buffer, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createBufferView( Device device, const BufferViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, BufferView* pView )
  {
    return static_cast<Result>( vkCreateBufferView( device, reinterpret_cast<const VkBufferViewCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pView ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createBufferView( Device device, const BufferViewCreateInfo& createInfo, const AllocationCallbacks& allocator, BufferView& view )
  {
    return createBufferView( device, &createInfo, &allocator, &view );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyBufferView( Device device, BufferView bufferView, const AllocationCallbacks* pAllocator )
  {
    vkDestroyBufferView( device, bufferView, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyBufferView( Device device, BufferView bufferView, const AllocationCallbacks& allocator )
  {
    destroyBufferView( device, bufferView, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createImage( Device device, const ImageCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Image* pImage )
  {
    return static_cast<Result>( vkCreateImage( device, reinterpret_cast<const VkImageCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pImage ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createImage( Device device, const ImageCreateInfo& createInfo, const AllocationCallbacks& allocator, Image& image )
  {
    return createImage( device, &createInfo, &allocator, &image );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyImage( Device device, Image image, const AllocationCallbacks* pAllocator )
  {
    vkDestroyImage( device, image, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyImage( Device device, Image image, const AllocationCallbacks& allocator )
  {
    destroyImage( device, image, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void getImageSubresourceLayout( Device device, Image image, const ImageSubresource* pSubresource, SubresourceLayout* pLayout )
  {
    vkGetImageSubresourceLayout( device, image, reinterpret_cast<const VkImageSubresource*>( pSubresource ), reinterpret_cast<VkSubresourceLayout*>( pLayout ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void getImageSubresourceLayout( Device device, Image image, const ImageSubresource& subresource, SubresourceLayout& layout )
  {
    getImageSubresourceLayout( device, image, &subresource, &layout );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createImageView( Device device, const ImageViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ImageView* pView )
  {
    return static_cast<Result>( vkCreateImageView( device, reinterpret_cast<const VkImageViewCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pView ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createImageView( Device device, const ImageViewCreateInfo& createInfo, const AllocationCallbacks& allocator, ImageView& view )
  {
    return createImageView( device, &createInfo, &allocator, &view );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyImageView( Device device, ImageView imageView, const AllocationCallbacks* pAllocator )
  {
    vkDestroyImageView( device, imageView, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyImageView( Device device, ImageView imageView, const AllocationCallbacks& allocator )
  {
    destroyImageView( device, imageView, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createShaderModule( Device device, const ShaderModuleCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ShaderModule* pShaderModule )
  {
    return static_cast<Result>( vkCreateShaderModule( device, reinterpret_cast<const VkShaderModuleCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pShaderModule ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createShaderModule( Device device, const ShaderModuleCreateInfo& createInfo, const AllocationCallbacks& allocator, ShaderModule& shaderModule )
  {
    return createShaderModule( device, &createInfo, &allocator, &shaderModule );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyShaderModule( Device device, ShaderModule shaderModule, const AllocationCallbacks* pAllocator )
  {
    vkDestroyShaderModule( device, shaderModule, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyShaderModule( Device device, ShaderModule shaderModule, const AllocationCallbacks& allocator )
  {
    destroyShaderModule( device, shaderModule, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createPipelineCache( Device device, const PipelineCacheCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineCache* pPipelineCache )
  {
    return static_cast<Result>( vkCreatePipelineCache( device, reinterpret_cast<const VkPipelineCacheCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pPipelineCache ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createPipelineCache( Device device, const PipelineCacheCreateInfo& createInfo, const AllocationCallbacks& allocator, PipelineCache& pipelineCache )
  {
    return createPipelineCache( device, &createInfo, &allocator, &pipelineCache );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyPipelineCache( Device device, PipelineCache pipelineCache, const AllocationCallbacks* pAllocator )
  {
    vkDestroyPipelineCache( device, pipelineCache, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyPipelineCache( Device device, PipelineCache pipelineCache, const AllocationCallbacks& allocator )
  {
    destroyPipelineCache( device, pipelineCache, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getPipelineCacheData( Device device, PipelineCache pipelineCache, size_t* pDataSize, void* pData )
  {
    return static_cast<Result>( vkGetPipelineCacheData( device, pipelineCache, pDataSize, pData ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result getPipelineCacheData( Device device, PipelineCache pipelineCache, std::vector<uint8_t> & data )
  {
    size_t pDataSize = 0;
    Result result = getPipelineCacheData( device, pipelineCache, &pDataSize, nullptr );
    if ( result == Result::eVkSuccess )
    {
      data.resize( pDataSize );
      result = getPipelineCacheData( device, pipelineCache, &pDataSize, data.data() );
    }
    return result;
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result mergePipelineCaches( Device device, PipelineCache dstCache, uint32_t srcCacheCount, const PipelineCache* pSrcCaches )
  {
    return static_cast<Result>( vkMergePipelineCaches( device, dstCache, srcCacheCount, pSrcCaches ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result mergePipelineCaches( Device device, PipelineCache dstCache, std::vector<PipelineCache> const& srcCaches )
  {
    return mergePipelineCaches( device, dstCache, static_cast<uint32_t>( srcCaches.size() ), srcCaches.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createGraphicsPipelines( Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const GraphicsPipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines )
  {
    return static_cast<Result>( vkCreateGraphicsPipelines( device, pipelineCache, createInfoCount, reinterpret_cast<const VkGraphicsPipelineCreateInfo*>( pCreateInfos ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pPipelines ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createGraphicsPipelines( Device device, PipelineCache pipelineCache, std::vector<GraphicsPipelineCreateInfo> const& createInfos, const AllocationCallbacks& allocator, std::vector<Pipeline> & pipelines )
  {
    assert( createInfos.size() <= pipelines.size() );
    return createGraphicsPipelines( device, pipelineCache, static_cast<uint32_t>( createInfos.size() ), createInfos.data(), &allocator, pipelines.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createComputePipelines( Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const ComputePipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines )
  {
    return static_cast<Result>( vkCreateComputePipelines( device, pipelineCache, createInfoCount, reinterpret_cast<const VkComputePipelineCreateInfo*>( pCreateInfos ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pPipelines ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createComputePipelines( Device device, PipelineCache pipelineCache, std::vector<ComputePipelineCreateInfo> const& createInfos, const AllocationCallbacks& allocator, std::vector<Pipeline> & pipelines )
  {
    assert( createInfos.size() <= pipelines.size() );
    return createComputePipelines( device, pipelineCache, static_cast<uint32_t>( createInfos.size() ), createInfos.data(), &allocator, pipelines.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyPipeline( Device device, Pipeline pipeline, const AllocationCallbacks* pAllocator )
  {
    vkDestroyPipeline( device, pipeline, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyPipeline( Device device, Pipeline pipeline, const AllocationCallbacks& allocator )
  {
    destroyPipeline( device, pipeline, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createPipelineLayout( Device device, const PipelineLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineLayout* pPipelineLayout )
  {
    return static_cast<Result>( vkCreatePipelineLayout( device, reinterpret_cast<const VkPipelineLayoutCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pPipelineLayout ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createPipelineLayout( Device device, const PipelineLayoutCreateInfo& createInfo, const AllocationCallbacks& allocator, PipelineLayout& pipelineLayout )
  {
    return createPipelineLayout( device, &createInfo, &allocator, &pipelineLayout );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyPipelineLayout( Device device, PipelineLayout pipelineLayout, const AllocationCallbacks* pAllocator )
  {
    vkDestroyPipelineLayout( device, pipelineLayout, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyPipelineLayout( Device device, PipelineLayout pipelineLayout, const AllocationCallbacks& allocator )
  {
    destroyPipelineLayout( device, pipelineLayout, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createSampler( Device device, const SamplerCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Sampler* pSampler )
  {
    return static_cast<Result>( vkCreateSampler( device, reinterpret_cast<const VkSamplerCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSampler ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createSampler( Device device, const SamplerCreateInfo& createInfo, const AllocationCallbacks& allocator, Sampler& sampler )
  {
    return createSampler( device, &createInfo, &allocator, &sampler );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroySampler( Device device, Sampler sampler, const AllocationCallbacks* pAllocator )
  {
    vkDestroySampler( device, sampler, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroySampler( Device device, Sampler sampler, const AllocationCallbacks& allocator )
  {
    destroySampler( device, sampler, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createDescriptorSetLayout( Device device, const DescriptorSetLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorSetLayout* pSetLayout )
  {
    return static_cast<Result>( vkCreateDescriptorSetLayout( device, reinterpret_cast<const VkDescriptorSetLayoutCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSetLayout ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createDescriptorSetLayout( Device device, const DescriptorSetLayoutCreateInfo& createInfo, const AllocationCallbacks& allocator, DescriptorSetLayout& setLayout )
  {
    return createDescriptorSetLayout( device, &createInfo, &allocator, &setLayout );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyDescriptorSetLayout( Device device, DescriptorSetLayout descriptorSetLayout, const AllocationCallbacks* pAllocator )
  {
    vkDestroyDescriptorSetLayout( device, descriptorSetLayout, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyDescriptorSetLayout( Device device, DescriptorSetLayout descriptorSetLayout, const AllocationCallbacks& allocator )
  {
    destroyDescriptorSetLayout( device, descriptorSetLayout, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createDescriptorPool( Device device, const DescriptorPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorPool* pDescriptorPool )
  {
    return static_cast<Result>( vkCreateDescriptorPool( device, reinterpret_cast<const VkDescriptorPoolCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pDescriptorPool ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createDescriptorPool( Device device, const DescriptorPoolCreateInfo& createInfo, const AllocationCallbacks& allocator, DescriptorPool& descriptorPool )
  {
    return createDescriptorPool( device, &createInfo, &allocator, &descriptorPool );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyDescriptorPool( Device device, DescriptorPool descriptorPool, const AllocationCallbacks* pAllocator )
  {
    vkDestroyDescriptorPool( device, descriptorPool, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyDescriptorPool( Device device, DescriptorPool descriptorPool, const AllocationCallbacks& allocator )
  {
    destroyDescriptorPool( device, descriptorPool, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result resetDescriptorPool( Device device, DescriptorPool descriptorPool, DescriptorPoolResetFlags flags )
  {
    return static_cast<Result>( vkResetDescriptorPool( device, descriptorPool, static_cast<VkDescriptorPoolResetFlags>( flags ) ) );
  }

  inline Result allocateDescriptorSets( Device device, const DescriptorSetAllocateInfo* pAllocateInfo, DescriptorSet* pDescriptorSets )
  {
    return static_cast<Result>( vkAllocateDescriptorSets( device, reinterpret_cast<const VkDescriptorSetAllocateInfo*>( pAllocateInfo ), pDescriptorSets ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result allocateDescriptorSets( Device device, const DescriptorSetAllocateInfo& allocateInfo, std::vector<DescriptorSet> & descriptorSets )
  {
    return allocateDescriptorSets( device, &allocateInfo, descriptorSets.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result freeDescriptorSets( Device device, DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets )
  {
    return static_cast<Result>( vkFreeDescriptorSets( device, descriptorPool, descriptorSetCount, pDescriptorSets ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result freeDescriptorSets( Device device, DescriptorPool descriptorPool, std::vector<DescriptorSet> const& descriptorSets )
  {
    return freeDescriptorSets( device, descriptorPool, static_cast<uint32_t>( descriptorSets.size() ), descriptorSets.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void updateDescriptorSets( Device device, uint32_t descriptorWriteCount, const WriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const CopyDescriptorSet* pDescriptorCopies )
  {
    vkUpdateDescriptorSets( device, descriptorWriteCount, reinterpret_cast<const VkWriteDescriptorSet*>( pDescriptorWrites ), descriptorCopyCount, reinterpret_cast<const VkCopyDescriptorSet*>( pDescriptorCopies ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void updateDescriptorSets( Device device, std::vector<WriteDescriptorSet> const& descriptorWrites, std::vector<CopyDescriptorSet> const& descriptorCopies )
  {
    updateDescriptorSets( device, static_cast<uint32_t>( descriptorWrites.size() ), descriptorWrites.data(), static_cast<uint32_t>( descriptorCopies.size() ), descriptorCopies.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createFramebuffer( Device device, const FramebufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Framebuffer* pFramebuffer )
  {
    return static_cast<Result>( vkCreateFramebuffer( device, reinterpret_cast<const VkFramebufferCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pFramebuffer ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createFramebuffer( Device device, const FramebufferCreateInfo& createInfo, const AllocationCallbacks& allocator, Framebuffer& framebuffer )
  {
    return createFramebuffer( device, &createInfo, &allocator, &framebuffer );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyFramebuffer( Device device, Framebuffer framebuffer, const AllocationCallbacks* pAllocator )
  {
    vkDestroyFramebuffer( device, framebuffer, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyFramebuffer( Device device, Framebuffer framebuffer, const AllocationCallbacks& allocator )
  {
    destroyFramebuffer( device, framebuffer, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createRenderPass( Device device, const RenderPassCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, RenderPass* pRenderPass )
  {
    return static_cast<Result>( vkCreateRenderPass( device, reinterpret_cast<const VkRenderPassCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pRenderPass ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createRenderPass( Device device, const RenderPassCreateInfo& createInfo, const AllocationCallbacks& allocator, RenderPass& renderPass )
  {
    return createRenderPass( device, &createInfo, &allocator, &renderPass );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyRenderPass( Device device, RenderPass renderPass, const AllocationCallbacks* pAllocator )
  {
    vkDestroyRenderPass( device, renderPass, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyRenderPass( Device device, RenderPass renderPass, const AllocationCallbacks& allocator )
  {
    destroyRenderPass( device, renderPass, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void getRenderAreaGranularity( Device device, RenderPass renderPass, Extent2D* pGranularity )
  {
    vkGetRenderAreaGranularity( device, renderPass, reinterpret_cast<VkExtent2D*>( pGranularity ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void getRenderAreaGranularity( Device device, RenderPass renderPass, Extent2D& granularity )
  {
    getRenderAreaGranularity( device, renderPass, &granularity );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createCommandPool( Device device, const CommandPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, CommandPool* pCommandPool )
  {
    return static_cast<Result>( vkCreateCommandPool( device, reinterpret_cast<const VkCommandPoolCreateInfo*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pCommandPool ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createCommandPool( Device device, const CommandPoolCreateInfo& createInfo, const AllocationCallbacks& allocator, CommandPool& commandPool )
  {
    return createCommandPool( device, &createInfo, &allocator, &commandPool );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyCommandPool( Device device, CommandPool commandPool, const AllocationCallbacks* pAllocator )
  {
    vkDestroyCommandPool( device, commandPool, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyCommandPool( Device device, CommandPool commandPool, const AllocationCallbacks& allocator )
  {
    destroyCommandPool( device, commandPool, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result resetCommandPool( Device device, CommandPool commandPool, CommandPoolResetFlags flags )
  {
    return static_cast<Result>( vkResetCommandPool( device, commandPool, static_cast<VkCommandPoolResetFlags>( flags ) ) );
  }

  inline Result allocateCommandBuffers( Device device, const CommandBufferAllocateInfo* pAllocateInfo, CommandBuffer* pCommandBuffers )
  {
    return static_cast<Result>( vkAllocateCommandBuffers( device, reinterpret_cast<const VkCommandBufferAllocateInfo*>( pAllocateInfo ), pCommandBuffers ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result allocateCommandBuffers( Device device, const CommandBufferAllocateInfo& allocateInfo, std::vector<CommandBuffer> & commandBuffers )
  {
    return allocateCommandBuffers( device, &allocateInfo, commandBuffers.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void freeCommandBuffers( Device device, CommandPool commandPool, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers )
  {
    vkFreeCommandBuffers( device, commandPool, commandBufferCount, pCommandBuffers );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void freeCommandBuffers( Device device, CommandPool commandPool, std::vector<CommandBuffer> const& commandBuffers )
  {
    freeCommandBuffers( device, commandPool, static_cast<uint32_t>( commandBuffers.size() ), commandBuffers.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result beginCommandBuffer( CommandBuffer commandBuffer, const CommandBufferBeginInfo* pBeginInfo )
  {
    return static_cast<Result>( vkBeginCommandBuffer( commandBuffer, reinterpret_cast<const VkCommandBufferBeginInfo*>( pBeginInfo ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result beginCommandBuffer( CommandBuffer commandBuffer, const CommandBufferBeginInfo& beginInfo )
  {
    return beginCommandBuffer( commandBuffer, &beginInfo );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result endCommandBuffer( CommandBuffer commandBuffer )
  {
    return static_cast<Result>( vkEndCommandBuffer( commandBuffer ) );
  }

  inline Result resetCommandBuffer( CommandBuffer commandBuffer, CommandBufferResetFlags flags )
  {
    return static_cast<Result>( vkResetCommandBuffer( commandBuffer, static_cast<VkCommandBufferResetFlags>( flags ) ) );
  }

  inline void cmdBindPipeline( CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, Pipeline pipeline )
  {
    vkCmdBindPipeline( commandBuffer, static_cast<VkPipelineBindPoint>( pipelineBindPoint ), pipeline );
  }

  inline void cmdSetViewport( CommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const Viewport* pViewports )
  {
    vkCmdSetViewport( commandBuffer, firstViewport, viewportCount, reinterpret_cast<const VkViewport*>( pViewports ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdSetViewport( CommandBuffer commandBuffer, uint32_t firstViewport, std::vector<Viewport> const& viewports )
  {
    cmdSetViewport( commandBuffer, firstViewport, static_cast<uint32_t>( viewports.size() ), viewports.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdSetScissor( CommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const Rect2D* pScissors )
  {
    vkCmdSetScissor( commandBuffer, firstScissor, scissorCount, reinterpret_cast<const VkRect2D*>( pScissors ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdSetScissor( CommandBuffer commandBuffer, uint32_t firstScissor, std::vector<Rect2D> const& scissors )
  {
    cmdSetScissor( commandBuffer, firstScissor, static_cast<uint32_t>( scissors.size() ), scissors.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdSetLineWidth( CommandBuffer commandBuffer, float lineWidth )
  {
    vkCmdSetLineWidth( commandBuffer, lineWidth );
  }

  inline void cmdSetDepthBias( CommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor )
  {
    vkCmdSetDepthBias( commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );
  }

  inline void cmdSetBlendConstants( CommandBuffer commandBuffer, const float blendConstants[4] )
  {
    vkCmdSetBlendConstants( commandBuffer, blendConstants );
  }

  inline void cmdSetDepthBounds( CommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds )
  {
    vkCmdSetDepthBounds( commandBuffer, minDepthBounds, maxDepthBounds );
  }

  inline void cmdSetStencilCompareMask( CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t compareMask )
  {
    vkCmdSetStencilCompareMask( commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), compareMask );
  }

  inline void cmdSetStencilWriteMask( CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t writeMask )
  {
    vkCmdSetStencilWriteMask( commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), writeMask );
  }

  inline void cmdSetStencilReference( CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t reference )
  {
    vkCmdSetStencilReference( commandBuffer, static_cast<VkStencilFaceFlags>( faceMask ), reference );
  }

  inline void cmdBindDescriptorSets( CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets )
  {
    vkCmdBindDescriptorSets( commandBuffer, static_cast<VkPipelineBindPoint>( pipelineBindPoint ), layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdBindDescriptorSets( CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, std::vector<DescriptorSet> const& descriptorSets, std::vector<uint32_t> const& dynamicOffsets )
  {
    cmdBindDescriptorSets( commandBuffer, pipelineBindPoint, layout, firstSet, static_cast<uint32_t>( descriptorSets.size() ), descriptorSets.data(), static_cast<uint32_t>( dynamicOffsets.size() ), dynamicOffsets.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdBindIndexBuffer( CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, IndexType indexType )
  {
    vkCmdBindIndexBuffer( commandBuffer, buffer, offset, static_cast<VkIndexType>( indexType ) );
  }

  inline void cmdBindVertexBuffers( CommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const Buffer* pBuffers, const DeviceSize* pOffsets )
  {
    vkCmdBindVertexBuffers( commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdBindVertexBuffers( CommandBuffer commandBuffer, uint32_t firstBinding, std::vector<Buffer> const& buffers, std::vector<DeviceSize> const& offsets )
  {
    assert( buffers.size() <= offsets.size() );
    cmdBindVertexBuffers( commandBuffer, firstBinding, static_cast<uint32_t>( buffers.size() ), buffers.data(), offsets.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdDraw( CommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance )
  {
    vkCmdDraw( commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance );
  }

  inline void cmdDrawIndexed( CommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance )
  {
    vkCmdDrawIndexed( commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance );
  }

  inline void cmdDrawIndirect( CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride )
  {
    vkCmdDrawIndirect( commandBuffer, buffer, offset, drawCount, stride );
  }

  inline void cmdDrawIndexedIndirect( CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride )
  {
    vkCmdDrawIndexedIndirect( commandBuffer, buffer, offset, drawCount, stride );
  }

  inline void cmdDispatch( CommandBuffer commandBuffer, uint32_t x, uint32_t y, uint32_t z )
  {
    vkCmdDispatch( commandBuffer, x, y, z );
  }

  inline void cmdDispatchIndirect( CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset )
  {
    vkCmdDispatchIndirect( commandBuffer, buffer, offset );
  }

  inline void cmdCopyBuffer( CommandBuffer commandBuffer, Buffer srcBuffer, Buffer dstBuffer, uint32_t regionCount, const BufferCopy* pRegions )
  {
    vkCmdCopyBuffer( commandBuffer, srcBuffer, dstBuffer, regionCount, reinterpret_cast<const VkBufferCopy*>( pRegions ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdCopyBuffer( CommandBuffer commandBuffer, Buffer srcBuffer, Buffer dstBuffer, std::vector<BufferCopy> const& regions )
  {
    cmdCopyBuffer( commandBuffer, srcBuffer, dstBuffer, static_cast<uint32_t>( regions.size() ), regions.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdCopyImage( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageCopy* pRegions )
  {
    vkCmdCopyImage( commandBuffer, srcImage, static_cast<VkImageLayout>( srcImageLayout ), dstImage, static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkImageCopy*>( pRegions ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdCopyImage( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, std::vector<ImageCopy> const& regions )
  {
    cmdCopyImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, static_cast<uint32_t>( regions.size() ), regions.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdBlitImage( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageBlit* pRegions, Filter filter )
  {
    vkCmdBlitImage( commandBuffer, srcImage, static_cast<VkImageLayout>( srcImageLayout ), dstImage, static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkImageBlit*>( pRegions ), static_cast<VkFilter>( filter ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdBlitImage( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, std::vector<ImageBlit> const& regions, Filter filter )
  {
    cmdBlitImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, static_cast<uint32_t>( regions.size() ), regions.data(), filter );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdCopyBufferToImage( CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const BufferImageCopy* pRegions )
  {
    vkCmdCopyBufferToImage( commandBuffer, srcBuffer, dstImage, static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkBufferImageCopy*>( pRegions ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdCopyBufferToImage( CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, std::vector<BufferImageCopy> const& regions )
  {
    cmdCopyBufferToImage( commandBuffer, srcBuffer, dstImage, dstImageLayout, static_cast<uint32_t>( regions.size() ), regions.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdCopyImageToBuffer( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, uint32_t regionCount, const BufferImageCopy* pRegions )
  {
    vkCmdCopyImageToBuffer( commandBuffer, srcImage, static_cast<VkImageLayout>( srcImageLayout ), dstBuffer, regionCount, reinterpret_cast<const VkBufferImageCopy*>( pRegions ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdCopyImageToBuffer( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, std::vector<BufferImageCopy> const& regions )
  {
    cmdCopyImageToBuffer( commandBuffer, srcImage, srcImageLayout, dstBuffer, static_cast<uint32_t>( regions.size() ), regions.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdUpdateBuffer( CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize dataSize, const uint32_t* pData )
  {
    vkCmdUpdateBuffer( commandBuffer, dstBuffer, dstOffset, dataSize, pData );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdUpdateBuffer( CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize dataSize, std::vector<uint32_t> const& data )
  {
    cmdUpdateBuffer( commandBuffer, dstBuffer, dstOffset, dataSize, data.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdFillBuffer( CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize size, uint32_t data )
  {
    vkCmdFillBuffer( commandBuffer, dstBuffer, dstOffset, size, data );
  }

  inline void cmdClearColorImage( CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue* pColor, uint32_t rangeCount, const ImageSubresourceRange* pRanges )
  {
    vkCmdClearColorImage( commandBuffer, image, static_cast<VkImageLayout>( imageLayout ), reinterpret_cast<const VkClearColorValue*>( pColor ), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>( pRanges ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdClearColorImage( CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue& color, std::vector<ImageSubresourceRange> const& ranges )
  {
    cmdClearColorImage( commandBuffer, image, imageLayout, &color, static_cast<uint32_t>( ranges.size() ), ranges.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdClearDepthStencilImage( CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const ImageSubresourceRange* pRanges )
  {
    vkCmdClearDepthStencilImage( commandBuffer, image, static_cast<VkImageLayout>( imageLayout ), reinterpret_cast<const VkClearDepthStencilValue*>( pDepthStencil ), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>( pRanges ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdClearDepthStencilImage( CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue& depthStencil, std::vector<ImageSubresourceRange> const& ranges )
  {
    cmdClearDepthStencilImage( commandBuffer, image, imageLayout, &depthStencil, static_cast<uint32_t>( ranges.size() ), ranges.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdClearAttachments( CommandBuffer commandBuffer, uint32_t attachmentCount, const ClearAttachment* pAttachments, uint32_t rectCount, const ClearRect* pRects )
  {
    vkCmdClearAttachments( commandBuffer, attachmentCount, reinterpret_cast<const VkClearAttachment*>( pAttachments ), rectCount, reinterpret_cast<const VkClearRect*>( pRects ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdClearAttachments( CommandBuffer commandBuffer, std::vector<ClearAttachment> const& attachments, std::vector<ClearRect> const& rects )
  {
    cmdClearAttachments( commandBuffer, static_cast<uint32_t>( attachments.size() ), attachments.data(), static_cast<uint32_t>( rects.size() ), rects.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdResolveImage( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageResolve* pRegions )
  {
    vkCmdResolveImage( commandBuffer, srcImage, static_cast<VkImageLayout>( srcImageLayout ), dstImage, static_cast<VkImageLayout>( dstImageLayout ), regionCount, reinterpret_cast<const VkImageResolve*>( pRegions ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdResolveImage( CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, std::vector<ImageResolve> const& regions )
  {
    cmdResolveImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, static_cast<uint32_t>( regions.size() ), regions.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdSetEvent( CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask )
  {
    vkCmdSetEvent( commandBuffer, event, static_cast<VkPipelineStageFlags>( stageMask ) );
  }

  inline void cmdResetEvent( CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask )
  {
    vkCmdResetEvent( commandBuffer, event, static_cast<VkPipelineStageFlags>( stageMask ) );
  }

  inline void cmdWaitEvents( CommandBuffer commandBuffer, uint32_t eventCount, const Event* pEvents, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers )
  {
    vkCmdWaitEvents( commandBuffer, eventCount, pEvents, static_cast<VkPipelineStageFlags>( srcStageMask ), static_cast<VkPipelineStageFlags>( dstStageMask ), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>( pMemoryBarriers ), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>( pBufferMemoryBarriers ), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>( pImageMemoryBarriers ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdWaitEvents( CommandBuffer commandBuffer, std::vector<Event> const& events, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, std::vector<MemoryBarrier> const& memoryBarriers, std::vector<BufferMemoryBarrier> const& bufferMemoryBarriers, std::vector<ImageMemoryBarrier> const& imageMemoryBarriers )
  {
    cmdWaitEvents( commandBuffer, static_cast<uint32_t>( events.size() ), events.data(), srcStageMask, dstStageMask, static_cast<uint32_t>( memoryBarriers.size() ), memoryBarriers.data(), static_cast<uint32_t>( bufferMemoryBarriers.size() ), bufferMemoryBarriers.data(), static_cast<uint32_t>( imageMemoryBarriers.size() ), imageMemoryBarriers.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdPipelineBarrier( CommandBuffer commandBuffer, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers )
  {
    vkCmdPipelineBarrier( commandBuffer, static_cast<VkPipelineStageFlags>( srcStageMask ), static_cast<VkPipelineStageFlags>( dstStageMask ), static_cast<VkDependencyFlags>( dependencyFlags ), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>( pMemoryBarriers ), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>( pBufferMemoryBarriers ), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>( pImageMemoryBarriers ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdPipelineBarrier( CommandBuffer commandBuffer, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, std::vector<MemoryBarrier> const& memoryBarriers, std::vector<BufferMemoryBarrier> const& bufferMemoryBarriers, std::vector<ImageMemoryBarrier> const& imageMemoryBarriers )
  {
    cmdPipelineBarrier( commandBuffer, srcStageMask, dstStageMask, dependencyFlags, static_cast<uint32_t>( memoryBarriers.size() ), memoryBarriers.data(), static_cast<uint32_t>( bufferMemoryBarriers.size() ), bufferMemoryBarriers.data(), static_cast<uint32_t>( imageMemoryBarriers.size() ), imageMemoryBarriers.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdBeginQuery( CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query, QueryControlFlags flags )
  {
    vkCmdBeginQuery( commandBuffer, queryPool, query, static_cast<VkQueryControlFlags>( flags ) );
  }

  inline void cmdEndQuery( CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query )
  {
    vkCmdEndQuery( commandBuffer, queryPool, query );
  }

  inline void cmdResetQueryPool( CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount )
  {
    vkCmdResetQueryPool( commandBuffer, queryPool, firstQuery, queryCount );
  }

  inline void cmdWriteTimestamp( CommandBuffer commandBuffer, PipelineStageFlagBits pipelineStage, QueryPool queryPool, uint32_t query )
  {
    vkCmdWriteTimestamp( commandBuffer, static_cast<VkPipelineStageFlagBits>( pipelineStage ), queryPool, query );
  }

  inline void cmdCopyQueryPoolResults( CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize stride, QueryResultFlags flags )
  {
    vkCmdCopyQueryPoolResults( commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, static_cast<VkQueryResultFlags>( flags ) );
  }

  inline void cmdPushConstants( CommandBuffer commandBuffer, PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues )
  {
    vkCmdPushConstants( commandBuffer, layout, static_cast<VkShaderStageFlags>( stageFlags ), offset, size, pValues );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdPushConstants( CommandBuffer commandBuffer, PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, std::vector<uint8_t> const& values )
  {
    cmdPushConstants( commandBuffer, layout, stageFlags, offset, static_cast<uint32_t>( values.size() ), values.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdBeginRenderPass( CommandBuffer commandBuffer, const RenderPassBeginInfo* pRenderPassBegin, SubpassContents contents )
  {
    vkCmdBeginRenderPass( commandBuffer, reinterpret_cast<const VkRenderPassBeginInfo*>( pRenderPassBegin ), static_cast<VkSubpassContents>( contents ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdBeginRenderPass( CommandBuffer commandBuffer, const RenderPassBeginInfo& renderPassBegin, SubpassContents contents )
  {
    cmdBeginRenderPass( commandBuffer, &renderPassBegin, contents );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void cmdNextSubpass( CommandBuffer commandBuffer, SubpassContents contents )
  {
    vkCmdNextSubpass( commandBuffer, static_cast<VkSubpassContents>( contents ) );
  }

  inline void cmdEndRenderPass( CommandBuffer commandBuffer )
  {
    vkCmdEndRenderPass( commandBuffer );
  }

  inline void cmdExecuteCommands( CommandBuffer commandBuffer, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers )
  {
    vkCmdExecuteCommands( commandBuffer, commandBufferCount, pCommandBuffers );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void cmdExecuteCommands( CommandBuffer commandBuffer, std::vector<CommandBuffer> const& commandBuffers )
  {
    cmdExecuteCommands( commandBuffer, static_cast<uint32_t>( commandBuffers.size() ), commandBuffers.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

  static const char * getString(FramebufferCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(FramebufferCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(QueryPoolCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(QueryPoolCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(RenderPassCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(RenderPassCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(SamplerCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(SamplerCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PipelineLayoutCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineLayoutCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PipelineCacheCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineCacheCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PipelineDepthStencilStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineDepthStencilStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PipelineDynamicStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineDynamicStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PipelineColorBlendStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineColorBlendStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PipelineMultisampleStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineMultisampleStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PipelineRasterizationStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineRasterizationStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PipelineViewportStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineViewportStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PipelineTessellationStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineTessellationStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PipelineInputAssemblyStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineInputAssemblyStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PipelineVertexInputStateCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineVertexInputStateCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PipelineShaderStageCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(PipelineShaderStageCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(DescriptorSetLayoutCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(DescriptorSetLayoutCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(BufferViewCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(BufferViewCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(InstanceCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(InstanceCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(DeviceCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(DeviceCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(DeviceQueueCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(DeviceQueueCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(ImageViewCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(ImageViewCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(SemaphoreCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(SemaphoreCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(ShaderModuleCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(ShaderModuleCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(EventCreateFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(EventCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(MemoryMapFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(MemoryMapFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(SubpassDescriptionFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(SubpassDescriptionFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(DescriptorPoolResetFlagBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(DescriptorPoolResetFlags value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(SwapchainCreateFlagsKHBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(SwapchainCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(DisplayModeCreateFlagsKHBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(DisplayModeCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(DisplaySurfaceCreateFlagsKHBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static std::string getString(DisplaySurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(AndroidSurfaceCreateFlagsKHBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * getString(MirSurfaceCreateFlagsKHBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * getString(WaylandSurfaceCreateFlagsKHBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * getString(Win32SurfaceCreateFlagsKHBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * getString(XlibSurfaceCreateFlagsKHBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * getString(XcbSurfaceCreateFlagsKHBits value)
  {
    switch (value)
    {
    default: return "unknown";
    }
  }

  static const char * getString(ImageLayout value)
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
    case ImageLayout::ePresentSrcKhr: return "PresentSrcKhr";
    default: return "unknown";
    }
  }

  static const char * getString(AttachmentLoadOp value)
  {
    switch (value)
    {
    case AttachmentLoadOp::eLoad: return "Load";
    case AttachmentLoadOp::eClear: return "Clear";
    case AttachmentLoadOp::eDontCare: return "DontCare";
    default: return "unknown";
    }
  }

  static const char * getString(AttachmentStoreOp value)
  {
    switch (value)
    {
    case AttachmentStoreOp::eStore: return "Store";
    case AttachmentStoreOp::eDontCare: return "DontCare";
    default: return "unknown";
    }
  }

  static const char * getString(ImageType value)
  {
    switch (value)
    {
    case ImageType::e1D: return "1D";
    case ImageType::e2D: return "2D";
    case ImageType::e3D: return "3D";
    default: return "unknown";
    }
  }

  static const char * getString(ImageTiling value)
  {
    switch (value)
    {
    case ImageTiling::eOptimal: return "Optimal";
    case ImageTiling::eLinear: return "Linear";
    default: return "unknown";
    }
  }

  static const char * getString(ImageViewType value)
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

  static const char * getString(CommandBufferLevel value)
  {
    switch (value)
    {
    case CommandBufferLevel::ePrimary: return "Primary";
    case CommandBufferLevel::eSecondary: return "Secondary";
    default: return "unknown";
    }
  }

  static const char * getString(ComponentSwizzle value)
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

  static const char * getString(DescriptorType value)
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

  static const char * getString(QueryType value)
  {
    switch (value)
    {
    case QueryType::eOcclusion: return "Occlusion";
    case QueryType::ePipelineStatistics: return "PipelineStatistics";
    case QueryType::eTimestamp: return "Timestamp";
    default: return "unknown";
    }
  }

  static const char * getString(BorderColor value)
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

  static const char * getString(PipelineBindPoint value)
  {
    switch (value)
    {
    case PipelineBindPoint::eGraphics: return "Graphics";
    case PipelineBindPoint::eCompute: return "Compute";
    default: return "unknown";
    }
  }

  static const char * getString(PipelineCacheHeaderVersion value)
  {
    switch (value)
    {
    case PipelineCacheHeaderVersion::eOne: return "One";
    default: return "unknown";
    }
  }

  static const char * getString(PrimitiveTopology value)
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

  static const char * getString(SharingMode value)
  {
    switch (value)
    {
    case SharingMode::eExclusive: return "Exclusive";
    case SharingMode::eConcurrent: return "Concurrent";
    default: return "unknown";
    }
  }

  static const char * getString(IndexType value)
  {
    switch (value)
    {
    case IndexType::eUint16: return "Uint16";
    case IndexType::eUint32: return "Uint32";
    default: return "unknown";
    }
  }

  static const char * getString(Filter value)
  {
    switch (value)
    {
    case Filter::eNearest: return "Nearest";
    case Filter::eLinear: return "Linear";
    default: return "unknown";
    }
  }

  static const char * getString(SamplerMipmapMode value)
  {
    switch (value)
    {
    case SamplerMipmapMode::eNearest: return "Nearest";
    case SamplerMipmapMode::eLinear: return "Linear";
    default: return "unknown";
    }
  }

  static const char * getString(SamplerAddressMode value)
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

  static const char * getString(CompareOp value)
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

  static const char * getString(PolygonMode value)
  {
    switch (value)
    {
    case PolygonMode::eFill: return "Fill";
    case PolygonMode::eLine: return "Line";
    case PolygonMode::ePoint: return "Point";
    default: return "unknown";
    }
  }

  static const char * getString(CullModeFlagBits value)
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

  static std::string getString(CullModeFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CullModeFlagBits::eNone) result += "None | ";
    if (value & vk::CullModeFlagBits::eFront) result += "Front | ";
    if (value & vk::CullModeFlagBits::eBack) result += "Back | ";
    if (value & vk::CullModeFlagBits::eFrontAndBack) result += "FrontAndBack | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(FrontFace value)
  {
    switch (value)
    {
    case FrontFace::eCounterClockwise: return "CounterClockwise";
    case FrontFace::eClockwise: return "Clockwise";
    default: return "unknown";
    }
  }

  static const char * getString(BlendFactor value)
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

  static const char * getString(BlendOp value)
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

  static const char * getString(StencilOp value)
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

  static const char * getString(LogicOp value)
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

  static const char * getString(InternalAllocationType value)
  {
    switch (value)
    {
    case InternalAllocationType::eExecutable: return "Executable";
    default: return "unknown";
    }
  }

  static const char * getString(SystemAllocationScope value)
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

  static const char * getString(PhysicalDeviceType value)
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

  static const char * getString(VertexInputRate value)
  {
    switch (value)
    {
    case VertexInputRate::eVertex: return "Vertex";
    case VertexInputRate::eInstance: return "Instance";
    default: return "unknown";
    }
  }

  static const char * getString(Format value)
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

  static const char * getString(StructureType value)
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
    case StructureType::eSwapchainCreateInfoKhr: return "SwapchainCreateInfoKhr";
    case StructureType::ePresentInfoKhr: return "PresentInfoKhr";
    case StructureType::eDisplayModeCreateInfoKhr: return "DisplayModeCreateInfoKhr";
    case StructureType::eDisplaySurfaceCreateInfoKhr: return "DisplaySurfaceCreateInfoKhr";
    case StructureType::eDisplayPresentInfoKhr: return "DisplayPresentInfoKhr";
    case StructureType::eXlibSurfaceCreateInfoKhr: return "XlibSurfaceCreateInfoKhr";
    case StructureType::eXcbSurfaceCreateInfoKhr: return "XcbSurfaceCreateInfoKhr";
    case StructureType::eWaylandSurfaceCreateInfoKhr: return "WaylandSurfaceCreateInfoKhr";
    case StructureType::eMirSurfaceCreateInfoKhr: return "MirSurfaceCreateInfoKhr";
    case StructureType::eAndroidSurfaceCreateInfoKhr: return "AndroidSurfaceCreateInfoKhr";
    case StructureType::eWin32SurfaceCreateInfoKhr: return "Win32SurfaceCreateInfoKhr";
    case StructureType::eDebugReportCreateInfoExt: return "DebugReportCreateInfoExt";
    default: return "unknown";
    }
  }

  static const char * getString(SubpassContents value)
  {
    switch (value)
    {
    case SubpassContents::eInline: return "Inline";
    case SubpassContents::eSecondaryCommandBuffers: return "SecondaryCommandBuffers";
    default: return "unknown";
    }
  }

  static const char * getString(Result value)
  {
    switch (value)
    {
    case Result::eVkSuccess: return "VkSuccess";
    case Result::eVkNotReady: return "VkNotReady";
    case Result::eVkTimeout: return "VkTimeout";
    case Result::eVkEventSet: return "VkEventSet";
    case Result::eVkEventReset: return "VkEventReset";
    case Result::eVkIncomplete: return "VkIncomplete";
    case Result::eVkErrorOutOfHostMemory: return "VkErrorOutOfHostMemory";
    case Result::eVkErrorOutOfDeviceMemory: return "VkErrorOutOfDeviceMemory";
    case Result::eVkErrorInitializationFailed: return "VkErrorInitializationFailed";
    case Result::eVkErrorDeviceLost: return "VkErrorDeviceLost";
    case Result::eVkErrorMemoryMapFailed: return "VkErrorMemoryMapFailed";
    case Result::eVkErrorLayerNotPresent: return "VkErrorLayerNotPresent";
    case Result::eVkErrorExtensionNotPresent: return "VkErrorExtensionNotPresent";
    case Result::eVkErrorFeatureNotPresent: return "VkErrorFeatureNotPresent";
    case Result::eVkErrorIncompatibleDriver: return "VkErrorIncompatibleDriver";
    case Result::eVkErrorTooManyObjects: return "VkErrorTooManyObjects";
    case Result::eVkErrorFormatNotSupported: return "VkErrorFormatNotSupported";
    case Result::eVkErrorSurfaceLostKhr: return "VkErrorSurfaceLostKhr";
    case Result::eVkErrorNativeWindowInUseKhr: return "VkErrorNativeWindowInUseKhr";
    case Result::eVkSuboptimalKhr: return "VkSuboptimalKhr";
    case Result::eVkErrorOutOfDateKhr: return "VkErrorOutOfDateKhr";
    case Result::eVkErrorIncompatibleDisplayKhr: return "VkErrorIncompatibleDisplayKhr";
    case Result::eVkErrorValidationFailedExt: return "VkErrorValidationFailedExt";
    default: return "unknown";
    }
  }

  static const char * getString(DynamicState value)
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

  static const char * getString(QueueFlagBits value)
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

  static std::string getString(QueueFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueueFlagBits::eGraphics) result += "Graphics | ";
    if (value & vk::QueueFlagBits::eCompute) result += "Compute | ";
    if (value & vk::QueueFlagBits::eTransfer) result += "Transfer | ";
    if (value & vk::QueueFlagBits::eSparseBinding) result += "SparseBinding | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(MemoryPropertyFlagBits value)
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

  static std::string getString(MemoryPropertyFlags value)
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
  static const char * getString(MemoryHeapFlagBits value)
  {
    switch (value)
    {
    case MemoryHeapFlagBits::eDeviceLocal: return "DeviceLocal";
    default: return "unknown";
    }
  }

  static std::string getString(MemoryHeapFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::MemoryHeapFlagBits::eDeviceLocal) result += "DeviceLocal | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(AccessFlagBits value)
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

  static std::string getString(AccessFlags value)
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
  static const char * getString(BufferUsageFlagBits value)
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

  static std::string getString(BufferUsageFlags value)
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
  static const char * getString(BufferCreateFlagBits value)
  {
    switch (value)
    {
    case BufferCreateFlagBits::eSparseBinding: return "SparseBinding";
    case BufferCreateFlagBits::eSparseResidency: return "SparseResidency";
    case BufferCreateFlagBits::eSparseAliased: return "SparseAliased";
    default: return "unknown";
    }
  }

  static std::string getString(BufferCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::BufferCreateFlagBits::eSparseBinding) result += "SparseBinding | ";
    if (value & vk::BufferCreateFlagBits::eSparseResidency) result += "SparseResidency | ";
    if (value & vk::BufferCreateFlagBits::eSparseAliased) result += "SparseAliased | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(ShaderStageFlagBits value)
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

  static std::string getString(ShaderStageFlags value)
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
  static const char * getString(ImageUsageFlagBits value)
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

  static std::string getString(ImageUsageFlags value)
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
  static const char * getString(ImageCreateFlagBits value)
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

  static std::string getString(ImageCreateFlags value)
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
  static const char * getString(PipelineCreateFlagBits value)
  {
    switch (value)
    {
    case PipelineCreateFlagBits::eDisableOptimization: return "DisableOptimization";
    case PipelineCreateFlagBits::eAllowDerivatives: return "AllowDerivatives";
    case PipelineCreateFlagBits::eDerivative: return "Derivative";
    default: return "unknown";
    }
  }

  static std::string getString(PipelineCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::PipelineCreateFlagBits::eDisableOptimization) result += "DisableOptimization | ";
    if (value & vk::PipelineCreateFlagBits::eAllowDerivatives) result += "AllowDerivatives | ";
    if (value & vk::PipelineCreateFlagBits::eDerivative) result += "Derivative | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(ColorComponentFlagBits value)
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

  static std::string getString(ColorComponentFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ColorComponentFlagBits::eR) result += "R | ";
    if (value & vk::ColorComponentFlagBits::eG) result += "G | ";
    if (value & vk::ColorComponentFlagBits::eB) result += "B | ";
    if (value & vk::ColorComponentFlagBits::eA) result += "A | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(FenceCreateFlagBits value)
  {
    switch (value)
    {
    case FenceCreateFlagBits::eSignaled: return "Signaled";
    default: return "unknown";
    }
  }

  static std::string getString(FenceCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::FenceCreateFlagBits::eSignaled) result += "Signaled | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(FormatFeatureFlagBits value)
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

  static std::string getString(FormatFeatureFlags value)
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
  static const char * getString(QueryControlFlagBits value)
  {
    switch (value)
    {
    case QueryControlFlagBits::ePrecise: return "Precise";
    default: return "unknown";
    }
  }

  static std::string getString(QueryControlFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueryControlFlagBits::ePrecise) result += "Precise | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(QueryResultFlagBits value)
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

  static std::string getString(QueryResultFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::QueryResultFlagBits::e64) result += "64 | ";
    if (value & vk::QueryResultFlagBits::eWait) result += "Wait | ";
    if (value & vk::QueryResultFlagBits::eWithAvailability) result += "WithAvailability | ";
    if (value & vk::QueryResultFlagBits::ePartial) result += "Partial | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(CommandBufferUsageFlagBits value)
  {
    switch (value)
    {
    case CommandBufferUsageFlagBits::eOneTimeSubmit: return "OneTimeSubmit";
    case CommandBufferUsageFlagBits::eRenderPassContinue: return "RenderPassContinue";
    case CommandBufferUsageFlagBits::eSimultaneousUse: return "SimultaneousUse";
    default: return "unknown";
    }
  }

  static std::string getString(CommandBufferUsageFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandBufferUsageFlagBits::eOneTimeSubmit) result += "OneTimeSubmit | ";
    if (value & vk::CommandBufferUsageFlagBits::eRenderPassContinue) result += "RenderPassContinue | ";
    if (value & vk::CommandBufferUsageFlagBits::eSimultaneousUse) result += "SimultaneousUse | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(QueryPipelineStatisticFlagBits value)
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

  static std::string getString(QueryPipelineStatisticFlags value)
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
  static const char * getString(ImageAspectFlagBits value)
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

  static std::string getString(ImageAspectFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::ImageAspectFlagBits::eColor) result += "Color | ";
    if (value & vk::ImageAspectFlagBits::eDepth) result += "Depth | ";
    if (value & vk::ImageAspectFlagBits::eStencil) result += "Stencil | ";
    if (value & vk::ImageAspectFlagBits::eMetadata) result += "Metadata | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(SparseImageFormatFlagBits value)
  {
    switch (value)
    {
    case SparseImageFormatFlagBits::eSingleMiptail: return "SingleMiptail";
    case SparseImageFormatFlagBits::eAlignedMipSize: return "AlignedMipSize";
    case SparseImageFormatFlagBits::eNonstandardBlockSize: return "NonstandardBlockSize";
    default: return "unknown";
    }
  }

  static std::string getString(SparseImageFormatFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SparseImageFormatFlagBits::eSingleMiptail) result += "SingleMiptail | ";
    if (value & vk::SparseImageFormatFlagBits::eAlignedMipSize) result += "AlignedMipSize | ";
    if (value & vk::SparseImageFormatFlagBits::eNonstandardBlockSize) result += "NonstandardBlockSize | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(SparseMemoryBindFlagBits value)
  {
    switch (value)
    {
    case SparseMemoryBindFlagBits::eMetadata: return "Metadata";
    default: return "unknown";
    }
  }

  static std::string getString(SparseMemoryBindFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::SparseMemoryBindFlagBits::eMetadata) result += "Metadata | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PipelineStageFlagBits value)
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

  static std::string getString(PipelineStageFlags value)
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
  static const char * getString(CommandPoolCreateFlagBits value)
  {
    switch (value)
    {
    case CommandPoolCreateFlagBits::eTransient: return "Transient";
    case CommandPoolCreateFlagBits::eResetCommandBuffer: return "ResetCommandBuffer";
    default: return "unknown";
    }
  }

  static std::string getString(CommandPoolCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandPoolCreateFlagBits::eTransient) result += "Transient | ";
    if (value & vk::CommandPoolCreateFlagBits::eResetCommandBuffer) result += "ResetCommandBuffer | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(CommandPoolResetFlagBits value)
  {
    switch (value)
    {
    case CommandPoolResetFlagBits::eReleaseResources: return "ReleaseResources";
    default: return "unknown";
    }
  }

  static std::string getString(CommandPoolResetFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandPoolResetFlagBits::eReleaseResources) result += "ReleaseResources | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(CommandBufferResetFlagBits value)
  {
    switch (value)
    {
    case CommandBufferResetFlagBits::eReleaseResources: return "ReleaseResources";
    default: return "unknown";
    }
  }

  static std::string getString(CommandBufferResetFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CommandBufferResetFlagBits::eReleaseResources) result += "ReleaseResources | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(SampleCountFlagBits value)
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

  static std::string getString(SampleCountFlags value)
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
  static const char * getString(AttachmentDescriptionFlagBits value)
  {
    switch (value)
    {
    case AttachmentDescriptionFlagBits::eMayAlias: return "MayAlias";
    default: return "unknown";
    }
  }

  static std::string getString(AttachmentDescriptionFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::AttachmentDescriptionFlagBits::eMayAlias) result += "MayAlias | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(StencilFaceFlagBits value)
  {
    switch (value)
    {
    case StencilFaceFlagBits::eFront: return "Front";
    case StencilFaceFlagBits::eBack: return "Back";
    case StencilFaceFlagBits::eVkStencilFrontAndBack: return "VkStencilFrontAndBack";
    default: return "unknown";
    }
  }

  static std::string getString(StencilFaceFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::StencilFaceFlagBits::eFront) result += "Front | ";
    if (value & vk::StencilFaceFlagBits::eBack) result += "Back | ";
    if (value & vk::StencilFaceFlagBits::eVkStencilFrontAndBack) result += "VkStencilFrontAndBack | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(DescriptorPoolCreateFlagBits value)
  {
    switch (value)
    {
    case DescriptorPoolCreateFlagBits::eFreeDescriptorSet: return "FreeDescriptorSet";
    default: return "unknown";
    }
  }

  static std::string getString(DescriptorPoolCreateFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet) result += "FreeDescriptorSet | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(DependencyFlagBits value)
  {
    switch (value)
    {
    case DependencyFlagBits::eByRegion: return "ByRegion";
    default: return "unknown";
    }
  }

  static std::string getString(DependencyFlags value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DependencyFlagBits::eByRegion) result += "ByRegion | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(PresentModeKHR value)
  {
    switch (value)
    {
    case PresentModeKHR::eVkPresentModeImmediateKhr: return "VkPresentModeImmediateKhr";
    case PresentModeKHR::eVkPresentModeMailboxKhr: return "VkPresentModeMailboxKhr";
    case PresentModeKHR::eVkPresentModeFifoKhr: return "VkPresentModeFifoKhr";
    case PresentModeKHR::eVkPresentModeFifoRelaxedKhr: return "VkPresentModeFifoRelaxedKhr";
    default: return "unknown";
    }
  }

  static const char * getString(ColorSpaceKHR value)
  {
    switch (value)
    {
    case ColorSpaceKHR::eVkColorspaceSrgbNonlinearKhr: return "VkColorspaceSrgbNonlinearKhr";
    default: return "unknown";
    }
  }

  static const char * getString(CompositeAlphaFlagBitsKHR value)
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

  static std::string getString(CompositeAlphaFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::CompositeAlphaFlagBitsKHR::eOpaque) result += "Opaque | ";
    if (value & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) result += "PreMultiplied | ";
    if (value & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) result += "PostMultiplied | ";
    if (value & vk::CompositeAlphaFlagBitsKHR::eInherit) result += "Inherit | ";
    return result.substr(0, result.size() - 3);
  }
  static const char * getString(SurfaceTransformFlagBitsKHR value)
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

  static std::string getString(SurfaceTransformFlagsKHR value)
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
  static const char * getString(DebugReportFlagBitsEXT value)
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

  static std::string getString(DebugReportFlagsEXT value)
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
  inline void destroySurfaceKHR( Instance instance, SurfaceKHR surface, const AllocationCallbacks* pAllocator )
  {
    vkDestroySurfaceKHR( instance, surface, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroySurfaceKHR( Instance instance, SurfaceKHR surface, const AllocationCallbacks& allocator )
  {
    destroySurfaceKHR( instance, surface, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getPhysicalDeviceSurfaceSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, SurfaceKHR surface, Bool32* pSupported )
  {
    return static_cast<Result>( vkGetPhysicalDeviceSurfaceSupportKHR( physicalDevice, queueFamilyIndex, surface, pSupported ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result getPhysicalDeviceSurfaceSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, SurfaceKHR surface, Bool32& supported )
  {
    return getPhysicalDeviceSurfaceSupportKHR( physicalDevice, queueFamilyIndex, surface, &supported );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getPhysicalDeviceSurfaceCapabilitiesKHR( PhysicalDevice physicalDevice, SurfaceKHR surface, SurfaceCapabilitiesKHR* pSurfaceCapabilities )
  {
    return static_cast<Result>( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( physicalDevice, surface, reinterpret_cast<VkSurfaceCapabilitiesKHR*>( pSurfaceCapabilities ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result getPhysicalDeviceSurfaceCapabilitiesKHR( PhysicalDevice physicalDevice, SurfaceKHR surface, SurfaceCapabilitiesKHR& surfaceCapabilities )
  {
    return getPhysicalDeviceSurfaceCapabilitiesKHR( physicalDevice, surface, &surfaceCapabilities );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getPhysicalDeviceSurfaceFormatsKHR( PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pSurfaceFormatCount, SurfaceFormatKHR* pSurfaceFormats )
  {
    return static_cast<Result>( vkGetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, pSurfaceFormatCount, reinterpret_cast<VkSurfaceFormatKHR*>( pSurfaceFormats ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result getPhysicalDeviceSurfaceFormatsKHR( PhysicalDevice physicalDevice, SurfaceKHR surface, std::vector<SurfaceFormatKHR> & surfaceFormats )
  {
    uint32_t pSurfaceFormatCount = 0;
    Result result = getPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, &pSurfaceFormatCount, nullptr );
    if ( result == Result::eVkSuccess )
    {
      surfaceFormats.resize( pSurfaceFormatCount );
      result = getPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, &pSurfaceFormatCount, surfaceFormats.data() );
    }
    return result;
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getPhysicalDeviceSurfacePresentModesKHR( PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pPresentModeCount, PresentModeKHR* pPresentModes )
  {
    return static_cast<Result>( vkGetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, pPresentModeCount, reinterpret_cast<VkPresentModeKHR*>( pPresentModes ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result getPhysicalDeviceSurfacePresentModesKHR( PhysicalDevice physicalDevice, SurfaceKHR surface, std::vector<PresentModeKHR> & presentModes )
  {
    uint32_t pPresentModeCount = 0;
    Result result = getPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, &pPresentModeCount, nullptr );
    if ( result == Result::eVkSuccess )
    {
      presentModes.resize( pPresentModeCount );
      result = getPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, &pPresentModeCount, presentModes.data() );
    }
    return result;
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createSwapchainKHR( Device device, const SwapchainCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchain )
  {
    return static_cast<Result>( vkCreateSwapchainKHR( device, reinterpret_cast<const VkSwapchainCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSwapchain ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createSwapchainKHR( Device device, const SwapchainCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SwapchainKHR& swapchain )
  {
    return createSwapchainKHR( device, &createInfo, &allocator, &swapchain );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroySwapchainKHR( Device device, SwapchainKHR swapchain, const AllocationCallbacks* pAllocator )
  {
    vkDestroySwapchainKHR( device, swapchain, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroySwapchainKHR( Device device, SwapchainKHR swapchain, const AllocationCallbacks& allocator )
  {
    destroySwapchainKHR( device, swapchain, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getSwapchainImagesKHR( Device device, SwapchainKHR swapchain, uint32_t* pSwapchainImageCount, Image* pSwapchainImages )
  {
    return static_cast<Result>( vkGetSwapchainImagesKHR( device, swapchain, pSwapchainImageCount, pSwapchainImages ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result getSwapchainImagesKHR( Device device, SwapchainKHR swapchain, std::vector<Image> & swapchainImages )
  {
    uint32_t pSwapchainImageCount = 0;
    Result result = getSwapchainImagesKHR( device, swapchain, &pSwapchainImageCount, nullptr );
    if ( result == Result::eVkSuccess )
    {
      swapchainImages.resize( pSwapchainImageCount );
      result = getSwapchainImagesKHR( device, swapchain, &pSwapchainImageCount, swapchainImages.data() );
    }
    return result;
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result acquireNextImageKHR( Device device, SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t* pImageIndex )
  {
    return static_cast<Result>( vkAcquireNextImageKHR( device, swapchain, timeout, semaphore, fence, pImageIndex ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result acquireNextImageKHR( Device device, SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t& imageIndex )
  {
    return acquireNextImageKHR( device, swapchain, timeout, semaphore, fence, &imageIndex );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result queuePresentKHR( Queue queue, const PresentInfoKHR* pPresentInfo )
  {
    return static_cast<Result>( vkQueuePresentKHR( queue, reinterpret_cast<const VkPresentInfoKHR*>( pPresentInfo ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result queuePresentKHR( Queue queue, const PresentInfoKHR& presentInfo )
  {
    return queuePresentKHR( queue, &presentInfo );
  }
#endif    // VKCPP_ENHANCED_MODE

  class DisplayPlanePropertiesKHR
  {
  public:
    DisplayPlanePropertiesKHR()
      : DisplayPlanePropertiesKHR( DisplayKHR(), 0 )
    {}

    DisplayPlanePropertiesKHR( DisplayKHR currentDisplay, uint32_t currentStackIndex)
    {
      m_displayPlanePropertiesKHR.currentDisplay = currentDisplay;
      m_displayPlanePropertiesKHR.currentStackIndex = currentStackIndex;
    }

    const DisplayKHR& currentDisplay() const
    {
      return m_displayPlanePropertiesKHR.currentDisplay;
    }

    DisplayPlanePropertiesKHR& currentDisplay( DisplayKHR currentDisplay )
    {
      m_displayPlanePropertiesKHR.currentDisplay = currentDisplay;
      return *this;
    }

    const uint32_t& currentStackIndex() const
    {
      return m_displayPlanePropertiesKHR.currentStackIndex;
    }

    DisplayPlanePropertiesKHR& currentStackIndex( uint32_t currentStackIndex )
    {
      m_displayPlanePropertiesKHR.currentStackIndex = currentStackIndex;
      return *this;
    }

    operator const VkDisplayPlanePropertiesKHR&() const
    {
      return m_displayPlanePropertiesKHR;
    }

  private:
    VkDisplayPlanePropertiesKHR m_displayPlanePropertiesKHR;
  };

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

    const Extent2D& visibleRegion() const
    {
      return reinterpret_cast<const Extent2D&>( m_displayModeParametersKHR.visibleRegion );
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

    DisplayModeParametersKHR& refreshRate( uint32_t refreshRate )
    {
      m_displayModeParametersKHR.refreshRate = refreshRate;
      return *this;
    }

    operator const VkDisplayModeParametersKHR&() const
    {
      return m_displayModeParametersKHR;
    }

  private:
    VkDisplayModeParametersKHR m_displayModeParametersKHR;
  };

  class DisplayModePropertiesKHR
  {
  public:
    DisplayModePropertiesKHR()
      : DisplayModePropertiesKHR( DisplayModeKHR(), DisplayModeParametersKHR() )
    {}

    DisplayModePropertiesKHR( DisplayModeKHR displayMode, DisplayModeParametersKHR parameters)
    {
      m_displayModePropertiesKHR.displayMode = displayMode;
      m_displayModePropertiesKHR.parameters = static_cast<VkDisplayModeParametersKHR>( parameters );
    }

    const DisplayModeKHR& displayMode() const
    {
      return m_displayModePropertiesKHR.displayMode;
    }

    DisplayModePropertiesKHR& displayMode( DisplayModeKHR displayMode )
    {
      m_displayModePropertiesKHR.displayMode = displayMode;
      return *this;
    }

    const DisplayModeParametersKHR& parameters() const
    {
      return reinterpret_cast<const DisplayModeParametersKHR&>( m_displayModePropertiesKHR.parameters );
    }

    DisplayModePropertiesKHR& parameters( DisplayModeParametersKHR parameters )
    {
      m_displayModePropertiesKHR.parameters = static_cast<VkDisplayModeParametersKHR>( parameters );
      return *this;
    }

    operator const VkDisplayModePropertiesKHR&() const
    {
      return m_displayModePropertiesKHR;
    }

  private:
    VkDisplayModePropertiesKHR m_displayModePropertiesKHR;
  };

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_displayModeCreateInfoKHR.sType );
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

    DisplayModeCreateInfoKHR& pNext( const void* pNext )
    {
      m_displayModeCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const DisplayModeCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const DisplayModeCreateFlagsKHR&>( m_displayModeCreateInfoKHR.flags );
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

    DisplayModeCreateInfoKHR& parameters( DisplayModeParametersKHR parameters )
    {
      m_displayModeCreateInfoKHR.parameters = static_cast<VkDisplayModeParametersKHR>( parameters );
      return *this;
    }

    operator const VkDisplayModeCreateInfoKHR&() const
    {
      return m_displayModeCreateInfoKHR;
    }

  private:
    VkDisplayModeCreateInfoKHR m_displayModeCreateInfoKHR;
  };

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

    const DisplayPlaneAlphaFlagsKHR& supportedAlpha() const
    {
      return reinterpret_cast<const DisplayPlaneAlphaFlagsKHR&>( m_displayPlaneCapabilitiesKHR.supportedAlpha );
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

    DisplayPlaneCapabilitiesKHR& minSrcPosition( Offset2D minSrcPosition )
    {
      m_displayPlaneCapabilitiesKHR.minSrcPosition = static_cast<VkOffset2D>( minSrcPosition );
      return *this;
    }

    const Offset2D& maxSrcPosition() const
    {
      return reinterpret_cast<const Offset2D&>( m_displayPlaneCapabilitiesKHR.maxSrcPosition );
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

    DisplayPlaneCapabilitiesKHR& minSrcExtent( Extent2D minSrcExtent )
    {
      m_displayPlaneCapabilitiesKHR.minSrcExtent = static_cast<VkExtent2D>( minSrcExtent );
      return *this;
    }

    const Extent2D& maxSrcExtent() const
    {
      return reinterpret_cast<const Extent2D&>( m_displayPlaneCapabilitiesKHR.maxSrcExtent );
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

    DisplayPlaneCapabilitiesKHR& minDstPosition( Offset2D minDstPosition )
    {
      m_displayPlaneCapabilitiesKHR.minDstPosition = static_cast<VkOffset2D>( minDstPosition );
      return *this;
    }

    const Offset2D& maxDstPosition() const
    {
      return reinterpret_cast<const Offset2D&>( m_displayPlaneCapabilitiesKHR.maxDstPosition );
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

    DisplayPlaneCapabilitiesKHR& minDstExtent( Extent2D minDstExtent )
    {
      m_displayPlaneCapabilitiesKHR.minDstExtent = static_cast<VkExtent2D>( minDstExtent );
      return *this;
    }

    const Extent2D& maxDstExtent() const
    {
      return reinterpret_cast<const Extent2D&>( m_displayPlaneCapabilitiesKHR.maxDstExtent );
    }

    DisplayPlaneCapabilitiesKHR& maxDstExtent( Extent2D maxDstExtent )
    {
      m_displayPlaneCapabilitiesKHR.maxDstExtent = static_cast<VkExtent2D>( maxDstExtent );
      return *this;
    }

    operator const VkDisplayPlaneCapabilitiesKHR&() const
    {
      return m_displayPlaneCapabilitiesKHR;
    }

  private:
    VkDisplayPlaneCapabilitiesKHR m_displayPlaneCapabilitiesKHR;
  };

  class DisplayPropertiesKHR
  {
  public:
    DisplayPropertiesKHR()
      : DisplayPropertiesKHR( DisplayKHR(), nullptr, Extent2D(), Extent2D(), SurfaceTransformFlagsKHR(), 0, 0 )
    {}

    DisplayPropertiesKHR( DisplayKHR display, const char* displayName, Extent2D physicalDimensions, Extent2D physicalResolution, SurfaceTransformFlagsKHR supportedTransforms, Bool32 planeReorderPossible, Bool32 persistentContent)
    {
      m_displayPropertiesKHR.display = display;
      m_displayPropertiesKHR.displayName = displayName;
      m_displayPropertiesKHR.physicalDimensions = static_cast<VkExtent2D>( physicalDimensions );
      m_displayPropertiesKHR.physicalResolution = static_cast<VkExtent2D>( physicalResolution );
      m_displayPropertiesKHR.supportedTransforms = static_cast<VkSurfaceTransformFlagsKHR>( supportedTransforms );
      m_displayPropertiesKHR.planeReorderPossible = planeReorderPossible;
      m_displayPropertiesKHR.persistentContent = persistentContent;
    }

    const DisplayKHR& display() const
    {
      return m_displayPropertiesKHR.display;
    }

    DisplayPropertiesKHR& display( DisplayKHR display )
    {
      m_displayPropertiesKHR.display = display;
      return *this;
    }

    const char* displayName() const
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

    DisplayPropertiesKHR& physicalDimensions( Extent2D physicalDimensions )
    {
      m_displayPropertiesKHR.physicalDimensions = static_cast<VkExtent2D>( physicalDimensions );
      return *this;
    }

    const Extent2D& physicalResolution() const
    {
      return reinterpret_cast<const Extent2D&>( m_displayPropertiesKHR.physicalResolution );
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

    DisplayPropertiesKHR& supportedTransforms( SurfaceTransformFlagsKHR supportedTransforms )
    {
      m_displayPropertiesKHR.supportedTransforms = static_cast<VkSurfaceTransformFlagsKHR>( supportedTransforms );
      return *this;
    }

    const Bool32& planeReorderPossible() const
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

    DisplayPropertiesKHR& persistentContent( Bool32 persistentContent )
    {
      m_displayPropertiesKHR.persistentContent = persistentContent;
      return *this;
    }

    operator const VkDisplayPropertiesKHR&() const
    {
      return m_displayPropertiesKHR;
    }

  private:
    VkDisplayPropertiesKHR m_displayPropertiesKHR;
  };

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
      m_displaySurfaceCreateInfoKHR.displayMode = displayMode;
      m_displaySurfaceCreateInfoKHR.planeIndex = planeIndex;
      m_displaySurfaceCreateInfoKHR.planeStackIndex = planeStackIndex;
      m_displaySurfaceCreateInfoKHR.transform = static_cast<VkSurfaceTransformFlagBitsKHR>( transform );
      m_displaySurfaceCreateInfoKHR.globalAlpha = globalAlpha;
      m_displaySurfaceCreateInfoKHR.alphaMode = static_cast<VkDisplayPlaneAlphaFlagBitsKHR>( alphaMode );
      m_displaySurfaceCreateInfoKHR.imageExtent = static_cast<VkExtent2D>( imageExtent );
    }

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_displaySurfaceCreateInfoKHR.sType );
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

    DisplaySurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_displaySurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const DisplaySurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const DisplaySurfaceCreateFlagsKHR&>( m_displaySurfaceCreateInfoKHR.flags );
    }

    DisplaySurfaceCreateInfoKHR& flags( DisplaySurfaceCreateFlagsKHR flags )
    {
      m_displaySurfaceCreateInfoKHR.flags = static_cast<VkDisplaySurfaceCreateFlagsKHR>( flags );
      return *this;
    }

    const DisplayModeKHR& displayMode() const
    {
      return m_displaySurfaceCreateInfoKHR.displayMode;
    }

    DisplaySurfaceCreateInfoKHR& displayMode( DisplayModeKHR displayMode )
    {
      m_displaySurfaceCreateInfoKHR.displayMode = displayMode;
      return *this;
    }

    const uint32_t& planeIndex() const
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

    DisplaySurfaceCreateInfoKHR& planeStackIndex( uint32_t planeStackIndex )
    {
      m_displaySurfaceCreateInfoKHR.planeStackIndex = planeStackIndex;
      return *this;
    }

    const SurfaceTransformFlagBitsKHR& transform() const
    {
      return reinterpret_cast<const SurfaceTransformFlagBitsKHR&>( m_displaySurfaceCreateInfoKHR.transform );
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

    DisplaySurfaceCreateInfoKHR& globalAlpha( float globalAlpha )
    {
      m_displaySurfaceCreateInfoKHR.globalAlpha = globalAlpha;
      return *this;
    }

    const DisplayPlaneAlphaFlagBitsKHR& alphaMode() const
    {
      return reinterpret_cast<const DisplayPlaneAlphaFlagBitsKHR&>( m_displaySurfaceCreateInfoKHR.alphaMode );
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

    DisplaySurfaceCreateInfoKHR& imageExtent( Extent2D imageExtent )
    {
      m_displaySurfaceCreateInfoKHR.imageExtent = static_cast<VkExtent2D>( imageExtent );
      return *this;
    }

    operator const VkDisplaySurfaceCreateInfoKHR&() const
    {
      return m_displaySurfaceCreateInfoKHR;
    }

  private:
    VkDisplaySurfaceCreateInfoKHR m_displaySurfaceCreateInfoKHR;
  };

  inline Result getPhysicalDeviceDisplayPropertiesKHR( PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPropertiesKHR* pProperties )
  {
    return static_cast<Result>( vkGetPhysicalDeviceDisplayPropertiesKHR( physicalDevice, pPropertyCount, reinterpret_cast<VkDisplayPropertiesKHR*>( pProperties ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result getPhysicalDeviceDisplayPropertiesKHR( PhysicalDevice physicalDevice, std::vector<DisplayPropertiesKHR> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = getPhysicalDeviceDisplayPropertiesKHR( physicalDevice, &pPropertyCount, nullptr );
    if ( result == Result::eVkSuccess )
    {
      properties.resize( pPropertyCount );
      result = getPhysicalDeviceDisplayPropertiesKHR( physicalDevice, &pPropertyCount, properties.data() );
    }
    return result;
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getPhysicalDeviceDisplayPlanePropertiesKHR( PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPlanePropertiesKHR* pProperties )
  {
    return static_cast<Result>( vkGetPhysicalDeviceDisplayPlanePropertiesKHR( physicalDevice, pPropertyCount, reinterpret_cast<VkDisplayPlanePropertiesKHR*>( pProperties ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result getPhysicalDeviceDisplayPlanePropertiesKHR( PhysicalDevice physicalDevice, std::vector<DisplayPlanePropertiesKHR> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = getPhysicalDeviceDisplayPlanePropertiesKHR( physicalDevice, &pPropertyCount, nullptr );
    if ( result == Result::eVkSuccess )
    {
      properties.resize( pPropertyCount );
      result = getPhysicalDeviceDisplayPlanePropertiesKHR( physicalDevice, &pPropertyCount, properties.data() );
    }
    return result;
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getDisplayPlaneSupportedDisplaysKHR( PhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, DisplayKHR* pDisplays )
  {
    return static_cast<Result>( vkGetDisplayPlaneSupportedDisplaysKHR( physicalDevice, planeIndex, pDisplayCount, pDisplays ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result getDisplayPlaneSupportedDisplaysKHR( PhysicalDevice physicalDevice, uint32_t planeIndex, std::vector<DisplayKHR> & displays )
  {
    uint32_t pDisplayCount = 0;
    Result result = getDisplayPlaneSupportedDisplaysKHR( physicalDevice, planeIndex, &pDisplayCount, nullptr );
    if ( result == Result::eVkSuccess )
    {
      displays.resize( pDisplayCount );
      result = getDisplayPlaneSupportedDisplaysKHR( physicalDevice, planeIndex, &pDisplayCount, displays.data() );
    }
    return result;
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getDisplayModePropertiesKHR( PhysicalDevice physicalDevice, DisplayKHR display, uint32_t* pPropertyCount, DisplayModePropertiesKHR* pProperties )
  {
    return static_cast<Result>( vkGetDisplayModePropertiesKHR( physicalDevice, display, pPropertyCount, reinterpret_cast<VkDisplayModePropertiesKHR*>( pProperties ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result getDisplayModePropertiesKHR( PhysicalDevice physicalDevice, DisplayKHR display, std::vector<DisplayModePropertiesKHR> & properties )
  {
    uint32_t pPropertyCount = 0;
    Result result = getDisplayModePropertiesKHR( physicalDevice, display, &pPropertyCount, nullptr );
    if ( result == Result::eVkSuccess )
    {
      properties.resize( pPropertyCount );
      result = getDisplayModePropertiesKHR( physicalDevice, display, &pPropertyCount, properties.data() );
    }
    return result;
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createDisplayModeKHR( PhysicalDevice physicalDevice, DisplayKHR display, const DisplayModeCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, DisplayModeKHR* pMode )
  {
    return static_cast<Result>( vkCreateDisplayModeKHR( physicalDevice, display, reinterpret_cast<const VkDisplayModeCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pMode ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createDisplayModeKHR( PhysicalDevice physicalDevice, DisplayKHR display, const DisplayModeCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, DisplayModeKHR& mode )
  {
    return createDisplayModeKHR( physicalDevice, display, &createInfo, &allocator, &mode );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result getDisplayPlaneCapabilitiesKHR( PhysicalDevice physicalDevice, DisplayModeKHR mode, uint32_t planeIndex, DisplayPlaneCapabilitiesKHR* pCapabilities )
  {
    return static_cast<Result>( vkGetDisplayPlaneCapabilitiesKHR( physicalDevice, mode, planeIndex, reinterpret_cast<VkDisplayPlaneCapabilitiesKHR*>( pCapabilities ) ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result getDisplayPlaneCapabilitiesKHR( PhysicalDevice physicalDevice, DisplayModeKHR mode, uint32_t planeIndex, DisplayPlaneCapabilitiesKHR& capabilities )
  {
    return getDisplayPlaneCapabilitiesKHR( physicalDevice, mode, planeIndex, &capabilities );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Result createDisplayPlaneSurfaceKHR( Instance instance, const DisplaySurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( vkCreateDisplayPlaneSurfaceKHR( instance, reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createDisplayPlaneSurfaceKHR( Instance instance, const DisplaySurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createDisplayPlaneSurfaceKHR( instance, &createInfo, &allocator, &surface );
  }
#endif    // VKCPP_ENHANCED_MODE

  static const char * getString(DisplayPlaneAlphaFlagBitsKHR value)
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

  static std::string getString(DisplayPlaneAlphaFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::eOpaque) result += "Opaque | ";
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::eGlobal) result += "Global | ";
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::ePerPixel) result += "PerPixel | ";
    if (value & vk::DisplayPlaneAlphaFlagBitsKHR::ePerPixelPremultiplied) result += "PerPixelPremultiplied | ";
    return result.substr(0, result.size() - 3);
  }
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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_displayPresentInfoKHR.sType );
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

    DisplayPresentInfoKHR& pNext( const void* pNext )
    {
      m_displayPresentInfoKHR.pNext = pNext;
      return *this;
    }

    const Rect2D& srcRect() const
    {
      return reinterpret_cast<const Rect2D&>( m_displayPresentInfoKHR.srcRect );
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

    DisplayPresentInfoKHR& dstRect( Rect2D dstRect )
    {
      m_displayPresentInfoKHR.dstRect = static_cast<VkRect2D>( dstRect );
      return *this;
    }

    const Bool32& persistent() const
    {
      return m_displayPresentInfoKHR.persistent;
    }

    DisplayPresentInfoKHR& persistent( Bool32 persistent )
    {
      m_displayPresentInfoKHR.persistent = persistent;
      return *this;
    }

    operator const VkDisplayPresentInfoKHR&() const
    {
      return m_displayPresentInfoKHR;
    }

  private:
    VkDisplayPresentInfoKHR m_displayPresentInfoKHR;
  };

  inline Result createSharedSwapchainsKHR( Device device, uint32_t swapchainCount, const SwapchainCreateInfoKHR* pCreateInfos, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchains )
  {
    return static_cast<Result>( vkCreateSharedSwapchainsKHR( device, swapchainCount, reinterpret_cast<const VkSwapchainCreateInfoKHR*>( pCreateInfos ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSwapchains ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createSharedSwapchainsKHR( Device device, std::vector<SwapchainCreateInfoKHR> const& createInfos, const AllocationCallbacks& allocator, std::vector<SwapchainKHR> & swapchains )
  {
    assert( createInfos.size() <= swapchains.size() );
    return createSharedSwapchainsKHR( device, static_cast<uint32_t>( createInfos.size() ), createInfos.data(), &allocator, swapchains.data() );
  }
#endif    // VKCPP_ENHANCED_MODE

#ifdef VK_USE_PLATFORM_XLIB_KHR

  typedef Flags<XlibSurfaceCreateFlagsKHBits, VkXlibSurfaceCreateFlagsKHR> XlibSurfaceCreateFlagsKHR;

  inline XlibSurfaceCreateFlagsKHR operator|( XlibSurfaceCreateFlagsKHBits bit0, XlibSurfaceCreateFlagsKHBits bit1 )
  {
    return XlibSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_xlibSurfaceCreateInfoKHR.sType );
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

    XlibSurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_xlibSurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const XlibSurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const XlibSurfaceCreateFlagsKHR&>( m_xlibSurfaceCreateInfoKHR.flags );
    }

    XlibSurfaceCreateInfoKHR& flags( XlibSurfaceCreateFlagsKHR flags )
    {
      m_xlibSurfaceCreateInfoKHR.flags = static_cast<VkXlibSurfaceCreateFlagsKHR>( flags );
      return *this;
    }

    const Display* dpy() const
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

    XlibSurfaceCreateInfoKHR& window( Window window )
    {
      m_xlibSurfaceCreateInfoKHR.window = window;
      return *this;
    }

    operator const VkXlibSurfaceCreateInfoKHR&() const
    {
      return m_xlibSurfaceCreateInfoKHR;
    }

  private:
    VkXlibSurfaceCreateInfoKHR m_xlibSurfaceCreateInfoKHR;
  };

  inline Result createXlibSurfaceKHR( Instance instance, const XlibSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( vkCreateXlibSurfaceKHR( instance, reinterpret_cast<const VkXlibSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createXlibSurfaceKHR( Instance instance, const XlibSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createXlibSurfaceKHR( instance, &createInfo, &allocator, &surface );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Bool32 getPhysicalDeviceXlibPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID )
  {
    return vkGetPhysicalDeviceXlibPresentationSupportKHR( physicalDevice, queueFamilyIndex, dpy, visualID );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Bool32 getPhysicalDeviceXlibPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID )
  {
    return getPhysicalDeviceXlibPresentationSupportKHR( physicalDevice, queueFamilyIndex, dpy, visualID );
  }
#endif    // VKCPP_ENHANCED_MODE

  static std::string getString(XlibSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
#endif /* VK_USE_PLATFORM_XLIB_KHR */

#ifdef VK_USE_PLATFORM_XCB_KHR

  typedef Flags<XcbSurfaceCreateFlagsKHBits, VkXcbSurfaceCreateFlagsKHR> XcbSurfaceCreateFlagsKHR;

  inline XcbSurfaceCreateFlagsKHR operator|( XcbSurfaceCreateFlagsKHBits bit0, XcbSurfaceCreateFlagsKHBits bit1 )
  {
    return XcbSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_xcbSurfaceCreateInfoKHR.sType );
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

    XcbSurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_xcbSurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const XcbSurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const XcbSurfaceCreateFlagsKHR&>( m_xcbSurfaceCreateInfoKHR.flags );
    }

    XcbSurfaceCreateInfoKHR& flags( XcbSurfaceCreateFlagsKHR flags )
    {
      m_xcbSurfaceCreateInfoKHR.flags = static_cast<VkXcbSurfaceCreateFlagsKHR>( flags );
      return *this;
    }

    const xcb_connection_t* connection() const
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

    XcbSurfaceCreateInfoKHR& window( xcb_window_t window )
    {
      m_xcbSurfaceCreateInfoKHR.window = window;
      return *this;
    }

    operator const VkXcbSurfaceCreateInfoKHR&() const
    {
      return m_xcbSurfaceCreateInfoKHR;
    }

  private:
    VkXcbSurfaceCreateInfoKHR m_xcbSurfaceCreateInfoKHR;
  };

  inline Result createXcbSurfaceKHR( Instance instance, const XcbSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( vkCreateXcbSurfaceKHR( instance, reinterpret_cast<const VkXcbSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createXcbSurfaceKHR( Instance instance, const XcbSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createXcbSurfaceKHR( instance, &createInfo, &allocator, &surface );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Bool32 getPhysicalDeviceXcbPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id )
  {
    return vkGetPhysicalDeviceXcbPresentationSupportKHR( physicalDevice, queueFamilyIndex, connection, visual_id );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Bool32 getPhysicalDeviceXcbPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id )
  {
    return getPhysicalDeviceXcbPresentationSupportKHR( physicalDevice, queueFamilyIndex, connection, visual_id );
  }
#endif    // VKCPP_ENHANCED_MODE

  static std::string getString(XcbSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
#endif /* VK_USE_PLATFORM_XCB_KHR */

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

  typedef Flags<WaylandSurfaceCreateFlagsKHBits, VkWaylandSurfaceCreateFlagsKHR> WaylandSurfaceCreateFlagsKHR;

  inline WaylandSurfaceCreateFlagsKHR operator|( WaylandSurfaceCreateFlagsKHBits bit0, WaylandSurfaceCreateFlagsKHBits bit1 )
  {
    return WaylandSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_waylandSurfaceCreateInfoKHR.sType );
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

    WaylandSurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_waylandSurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const WaylandSurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const WaylandSurfaceCreateFlagsKHR&>( m_waylandSurfaceCreateInfoKHR.flags );
    }

    WaylandSurfaceCreateInfoKHR& flags( WaylandSurfaceCreateFlagsKHR flags )
    {
      m_waylandSurfaceCreateInfoKHR.flags = static_cast<VkWaylandSurfaceCreateFlagsKHR>( flags );
      return *this;
    }

    const struct wl_display* display() const
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
      return reinterpret_cast<struct wl_surface*>( m_waylandSurfaceCreateInfoKHR.surface );
    }

    WaylandSurfaceCreateInfoKHR& surface( struct wl_surface* surface )
    {
      m_waylandSurfaceCreateInfoKHR.surface = surface;
      return *this;
    }

    operator const VkWaylandSurfaceCreateInfoKHR&() const
    {
      return m_waylandSurfaceCreateInfoKHR;
    }

  private:
    VkWaylandSurfaceCreateInfoKHR m_waylandSurfaceCreateInfoKHR;
  };

  inline Result createWaylandSurfaceKHR( Instance instance, const WaylandSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( vkCreateWaylandSurfaceKHR( instance, reinterpret_cast<const VkWaylandSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createWaylandSurfaceKHR( Instance instance, const WaylandSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createWaylandSurfaceKHR( instance, &createInfo, &allocator, &surface );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Bool32 getPhysicalDeviceWaylandPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display )
  {
    return vkGetPhysicalDeviceWaylandPresentationSupportKHR( physicalDevice, queueFamilyIndex, display );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Bool32 getPhysicalDeviceWaylandPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display )
  {
    return getPhysicalDeviceWaylandPresentationSupportKHR( physicalDevice, queueFamilyIndex, display );
  }
#endif    // VKCPP_ENHANCED_MODE

  static std::string getString(WaylandSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
#endif /* VK_USE_PLATFORM_WAYLAND_KHR */

#ifdef VK_USE_PLATFORM_MIR_KHR

  typedef Flags<MirSurfaceCreateFlagsKHBits, VkMirSurfaceCreateFlagsKHR> MirSurfaceCreateFlagsKHR;

  inline MirSurfaceCreateFlagsKHR operator|( MirSurfaceCreateFlagsKHBits bit0, MirSurfaceCreateFlagsKHBits bit1 )
  {
    return MirSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_mirSurfaceCreateInfoKHR.sType );
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

    MirSurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_mirSurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const MirSurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const MirSurfaceCreateFlagsKHR&>( m_mirSurfaceCreateInfoKHR.flags );
    }

    MirSurfaceCreateInfoKHR& flags( MirSurfaceCreateFlagsKHR flags )
    {
      m_mirSurfaceCreateInfoKHR.flags = static_cast<VkMirSurfaceCreateFlagsKHR>( flags );
      return *this;
    }

    const MirConnection* connection() const
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
      return reinterpret_cast<MirSurface*>( m_mirSurfaceCreateInfoKHR.mirSurface );
    }

    MirSurfaceCreateInfoKHR& mirSurface( MirSurface* mirSurface )
    {
      m_mirSurfaceCreateInfoKHR.mirSurface = mirSurface;
      return *this;
    }

    operator const VkMirSurfaceCreateInfoKHR&() const
    {
      return m_mirSurfaceCreateInfoKHR;
    }

  private:
    VkMirSurfaceCreateInfoKHR m_mirSurfaceCreateInfoKHR;
  };

  inline Result createMirSurfaceKHR( Instance instance, const MirSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( vkCreateMirSurfaceKHR( instance, reinterpret_cast<const VkMirSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createMirSurfaceKHR( Instance instance, const MirSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createMirSurfaceKHR( instance, &createInfo, &allocator, &surface );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Bool32 getPhysicalDeviceMirPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, MirConnection* connection )
  {
    return vkGetPhysicalDeviceMirPresentationSupportKHR( physicalDevice, queueFamilyIndex, connection );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Bool32 getPhysicalDeviceMirPresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, MirConnection* connection )
  {
    return getPhysicalDeviceMirPresentationSupportKHR( physicalDevice, queueFamilyIndex, connection );
  }
#endif    // VKCPP_ENHANCED_MODE

  static std::string getString(MirSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
#endif /* VK_USE_PLATFORM_MIR_KHR */

#ifdef VK_USE_PLATFORM_ANDROID_KHR

  typedef Flags<AndroidSurfaceCreateFlagsKHBits, VkAndroidSurfaceCreateFlagsKHR> AndroidSurfaceCreateFlagsKHR;

  inline AndroidSurfaceCreateFlagsKHR operator|( AndroidSurfaceCreateFlagsKHBits bit0, AndroidSurfaceCreateFlagsKHBits bit1 )
  {
    return AndroidSurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_androidSurfaceCreateInfoKHR.sType );
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

    AndroidSurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_androidSurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const AndroidSurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const AndroidSurfaceCreateFlagsKHR&>( m_androidSurfaceCreateInfoKHR.flags );
    }

    AndroidSurfaceCreateInfoKHR& flags( AndroidSurfaceCreateFlagsKHR flags )
    {
      m_androidSurfaceCreateInfoKHR.flags = static_cast<VkAndroidSurfaceCreateFlagsKHR>( flags );
      return *this;
    }

    const ANativeWindow* window() const
    {
      return reinterpret_cast<ANativeWindow*>( m_androidSurfaceCreateInfoKHR.window );
    }

    AndroidSurfaceCreateInfoKHR& window( ANativeWindow* window )
    {
      m_androidSurfaceCreateInfoKHR.window = window;
      return *this;
    }

    operator const VkAndroidSurfaceCreateInfoKHR&() const
    {
      return m_androidSurfaceCreateInfoKHR;
    }

  private:
    VkAndroidSurfaceCreateInfoKHR m_androidSurfaceCreateInfoKHR;
  };

  inline Result createAndroidSurfaceKHR( Instance instance, const AndroidSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( vkCreateAndroidSurfaceKHR( instance, reinterpret_cast<const VkAndroidSurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createAndroidSurfaceKHR( Instance instance, const AndroidSurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createAndroidSurfaceKHR( instance, &createInfo, &allocator, &surface );
  }
#endif    // VKCPP_ENHANCED_MODE

  static std::string getString(AndroidSurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
#endif /* VK_USE_PLATFORM_ANDROID_KHR */

#ifdef VK_USE_PLATFORM_WIN32_KHR

  typedef Flags<Win32SurfaceCreateFlagsKHBits, VkWin32SurfaceCreateFlagsKHR> Win32SurfaceCreateFlagsKHR;

  inline Win32SurfaceCreateFlagsKHR operator|( Win32SurfaceCreateFlagsKHBits bit0, Win32SurfaceCreateFlagsKHBits bit1 )
  {
    return Win32SurfaceCreateFlagsKHR( bit0 ) | bit1;
  }

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

    const StructureType& sType() const
    {
      return reinterpret_cast<const StructureType&>( m_win32SurfaceCreateInfoKHR.sType );
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

    Win32SurfaceCreateInfoKHR& pNext( const void* pNext )
    {
      m_win32SurfaceCreateInfoKHR.pNext = pNext;
      return *this;
    }

    const Win32SurfaceCreateFlagsKHR& flags() const
    {
      return reinterpret_cast<const Win32SurfaceCreateFlagsKHR&>( m_win32SurfaceCreateInfoKHR.flags );
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

    Win32SurfaceCreateInfoKHR& hinstance( HINSTANCE hinstance )
    {
      m_win32SurfaceCreateInfoKHR.hinstance = hinstance;
      return *this;
    }

    const HWND& hwnd() const
    {
      return m_win32SurfaceCreateInfoKHR.hwnd;
    }

    Win32SurfaceCreateInfoKHR& hwnd( HWND hwnd )
    {
      m_win32SurfaceCreateInfoKHR.hwnd = hwnd;
      return *this;
    }

    operator const VkWin32SurfaceCreateInfoKHR&() const
    {
      return m_win32SurfaceCreateInfoKHR;
    }

  private:
    VkWin32SurfaceCreateInfoKHR m_win32SurfaceCreateInfoKHR;
  };

  inline Result createWin32SurfaceKHR( Instance instance, const Win32SurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface )
  {
    return static_cast<Result>( vkCreateWin32SurfaceKHR( instance, reinterpret_cast<const VkWin32SurfaceCreateInfoKHR*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pSurface ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createWin32SurfaceKHR( Instance instance, const Win32SurfaceCreateInfoKHR& createInfo, const AllocationCallbacks& allocator, SurfaceKHR& surface )
  {
    return createWin32SurfaceKHR( instance, &createInfo, &allocator, &surface );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline Bool32 getPhysicalDeviceWin32PresentationSupportKHR( PhysicalDevice physicalDevice, uint32_t queueFamilyIndex )
  {
    return vkGetPhysicalDeviceWin32PresentationSupportKHR( physicalDevice, queueFamilyIndex );
  }

  static std::string getString(Win32SurfaceCreateFlagsKHR value)
  {
    if (!value) return std::string();
    std::string result;
    return result.substr(0, result.size() - 3);
  }
#endif /* VK_USE_PLATFORM_WIN32_KHR */

  enum class DebugReportObjectTypeEXT
  {
    eVkDebugReportObjectTypeUnknownExt = VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT,
    eVkDebugReportObjectTypeInstanceExt = VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT,
    eVkDebugReportObjectTypePhysicalDeviceExt = VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT,
    eVkDebugReportObjectTypeDeviceExt = VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT,
    eVkDebugReportObjectTypeQueueExt = VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT,
    eVkDebugReportObjectTypeSemaphoreExt = VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT,
    eVkDebugReportObjectTypeCommandBufferExt = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT,
    eVkDebugReportObjectTypeFenceExt = VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT,
    eVkDebugReportObjectTypeDeviceMemoryExt = VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT,
    eVkDebugReportObjectTypeBufferExt = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT,
    eVkDebugReportObjectTypeImageExt = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT,
    eVkDebugReportObjectTypeEventExt = VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT,
    eVkDebugReportObjectTypeQueryPoolExt = VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT,
    eVkDebugReportObjectTypeBufferViewExt = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT,
    eVkDebugReportObjectTypeImageViewExt = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT,
    eVkDebugReportObjectTypeShaderModuleExt = VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT,
    eVkDebugReportObjectTypePipelineCacheExt = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT,
    eVkDebugReportObjectTypePipelineLayoutExt = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT,
    eVkDebugReportObjectTypeRenderPassExt = VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT,
    eVkDebugReportObjectTypePipelineExt = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
    eVkDebugReportObjectTypeDescriptorSetLayoutExt = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT,
    eVkDebugReportObjectTypeSamplerExt = VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT,
    eVkDebugReportObjectTypeDescriptorPoolExt = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT,
    eVkDebugReportObjectTypeDescriptorSetExt = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT,
    eVkDebugReportObjectTypeFramebufferExt = VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT,
    eVkDebugReportObjectTypeCommandPoolExt = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT,
    eVkDebugReportObjectTypeSurfaceKhrExt = VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT,
    eVkDebugReportObjectTypeSwapchainKhrExt = VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT,
    eVkDebugReportObjectTypeDebugReportExt = VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT
  };

  enum class DebugReportErrorEXT
  {
    eVkDebugReportErrorNoneExt = VK_DEBUG_REPORT_ERROR_NONE_EXT,
    eVkDebugReportErrorCallbackRefExt = VK_DEBUG_REPORT_ERROR_CALLBACK_REF_EXT
  };

  inline Result createDebugReportCallbackEXT( Instance instance, const DebugReportCallbackCreateInfoEXT* pCreateInfo, const AllocationCallbacks* pAllocator, DebugReportCallbackEXT* pCallback )
  {
    return static_cast<Result>( vkCreateDebugReportCallbackEXT( instance, reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT*>( pCreateInfo ), reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ), pCallback ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline Result createDebugReportCallbackEXT( Instance instance, const DebugReportCallbackCreateInfoEXT& createInfo, const AllocationCallbacks& allocator, DebugReportCallbackEXT& callback )
  {
    return createDebugReportCallbackEXT( instance, &createInfo, &allocator, &callback );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void destroyDebugReportCallbackEXT( Instance instance, DebugReportCallbackEXT callback, const AllocationCallbacks* pAllocator )
  {
    vkDestroyDebugReportCallbackEXT( instance, callback, reinterpret_cast<const VkAllocationCallbacks*>( pAllocator ) );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void destroyDebugReportCallbackEXT( Instance instance, DebugReportCallbackEXT callback, const AllocationCallbacks& allocator )
  {
    destroyDebugReportCallbackEXT( instance, callback, &allocator );
  }
#endif    // VKCPP_ENHANCED_MODE

  inline void debugReportMessageEXT( Instance instance, DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage )
  {
    vkDebugReportMessageEXT( instance, static_cast<VkDebugReportFlagsEXT>( flags ), static_cast<VkDebugReportObjectTypeEXT>( objectType ), object, location, messageCode, pLayerPrefix, pMessage );
  }

#ifdef VKCPP_ENHANCED_MODE
  inline void debugReportMessageEXT( Instance instance, DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char& layerPrefix, const char& message )
  {
    debugReportMessageEXT( instance, flags, objectType, object, location, messageCode, &layerPrefix, &message );
  }
#endif    // VKCPP_ENHANCED_MODE

  static const char * getString(DebugReportObjectTypeEXT value)
  {
    switch (value)
    {
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeUnknownExt: return "VkDebugReportObjectTypeUnknownExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeInstanceExt: return "VkDebugReportObjectTypeInstanceExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypePhysicalDeviceExt: return "VkDebugReportObjectTypePhysicalDeviceExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeDeviceExt: return "VkDebugReportObjectTypeDeviceExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeQueueExt: return "VkDebugReportObjectTypeQueueExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeSemaphoreExt: return "VkDebugReportObjectTypeSemaphoreExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeCommandBufferExt: return "VkDebugReportObjectTypeCommandBufferExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeFenceExt: return "VkDebugReportObjectTypeFenceExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeDeviceMemoryExt: return "VkDebugReportObjectTypeDeviceMemoryExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeBufferExt: return "VkDebugReportObjectTypeBufferExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeImageExt: return "VkDebugReportObjectTypeImageExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeEventExt: return "VkDebugReportObjectTypeEventExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeQueryPoolExt: return "VkDebugReportObjectTypeQueryPoolExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeBufferViewExt: return "VkDebugReportObjectTypeBufferViewExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeImageViewExt: return "VkDebugReportObjectTypeImageViewExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeShaderModuleExt: return "VkDebugReportObjectTypeShaderModuleExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypePipelineCacheExt: return "VkDebugReportObjectTypePipelineCacheExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypePipelineLayoutExt: return "VkDebugReportObjectTypePipelineLayoutExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeRenderPassExt: return "VkDebugReportObjectTypeRenderPassExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypePipelineExt: return "VkDebugReportObjectTypePipelineExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeDescriptorSetLayoutExt: return "VkDebugReportObjectTypeDescriptorSetLayoutExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeSamplerExt: return "VkDebugReportObjectTypeSamplerExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeDescriptorPoolExt: return "VkDebugReportObjectTypeDescriptorPoolExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeDescriptorSetExt: return "VkDebugReportObjectTypeDescriptorSetExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeFramebufferExt: return "VkDebugReportObjectTypeFramebufferExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeCommandPoolExt: return "VkDebugReportObjectTypeCommandPoolExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeSurfaceKhrExt: return "VkDebugReportObjectTypeSurfaceKhrExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeSwapchainKhrExt: return "VkDebugReportObjectTypeSwapchainKhrExt";
    case DebugReportObjectTypeEXT::eVkDebugReportObjectTypeDebugReportExt: return "VkDebugReportObjectTypeDebugReportExt";
    default: return "unknown";
    }
  }

  static const char * getString(DebugReportErrorEXT value)
  {
    switch (value)
    {
    case DebugReportErrorEXT::eVkDebugReportErrorNoneExt: return "VkDebugReportErrorNoneExt";
    case DebugReportErrorEXT::eVkDebugReportErrorCallbackRefExt: return "VkDebugReportErrorCallbackRefExt";
    default: return "unknown";
    }
  }

}
