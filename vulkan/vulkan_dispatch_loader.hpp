// Copyright 2015-2026 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

VULKAN_HPP_EXPORT namespace VULKAN_HPP_NAMESPACE
{
  namespace detail
  {
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
#  elif defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined( __Fuchsia__ )
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
          // modern versions of macOS don't search /usr/local/lib automatically contrary to what man dlopen says
          // Vulkan SDK uses this as the system-wide installation location, so we're going to fallback to this if all else fails
          if ( !m_library && ( std::getenv( "DYLD_FALLBACK_LIBRARY_PATH" ) == NULL ) )
          {
            m_library = dlopen( "/usr/local/lib/libvulkan.dylib", RTLD_NOW | RTLD_LOCAL );
          }
          // for Homebrew installations on Apple Silicon
          if ( !m_library )
          {
            m_library = dlopen( "/opt/homebrew/lib/libvulkan.dylib", RTLD_NOW | RTLD_LOCAL );
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
#  elif defined( __unix__ ) || defined( __QNX__ ) || defined( __Fuchsia__ )
          m_library = dlopen( "libvulkan.so", RTLD_NOW | RTLD_LOCAL );
          if ( !m_library )
          {
            m_library = dlopen( "libvulkan.so.1", RTLD_NOW | RTLD_LOCAL );
          }
#  else
#    error unsupported platform
#  endif
        }

#  ifndef VULKAN_HPP_NO_EXCEPTIONS
        if ( !m_library )
        {
          // NOTE there should be an InitializationFailedError, but msvc insists on the symbol does not exist within the scope of this function.
          throw std::runtime_error( "Failed to load vulkan library!" );
        }
#  endif
      }

      DynamicLoader( DynamicLoader const & ) = delete;

      DynamicLoader( DynamicLoader && other ) VULKAN_HPP_NOEXCEPT : m_library( other.m_library )
      {
        other.m_library = nullptr;
      }

      DynamicLoader & operator=( DynamicLoader const & ) = delete;

      DynamicLoader & operator=( DynamicLoader && other ) VULKAN_HPP_NOEXCEPT
      {
        std::swap( m_library, other.m_library );
        return *this;
      }

      ~DynamicLoader() VULKAN_HPP_NOEXCEPT
      {
        if ( m_library )
        {
#  if defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined( __Fuchsia__ )
          dlclose( m_library );
#  elif defined( _WIN32 )
          ::FreeLibrary( m_library );
#  else
#    error unsupported platform
#  endif
        }
      }

      template <typename T>
      T getProcAddress( char const * function ) const VULKAN_HPP_NOEXCEPT
      {
#  if defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined( __Fuchsia__ )
        return (T)(void *)dlsym( m_library, function );
#  elif defined( _WIN32 )
        return (T)(void *)::GetProcAddress( m_library, function );
#  else
#    error unsupported platform
#  endif
      }

      bool success() const VULKAN_HPP_NOEXCEPT
      {
        return m_library != nullptr;
      }

    private:
#  if defined( __unix__ ) || defined( __APPLE__ ) || defined( __QNX__ ) || defined( __Fuchsia__ )
      void * m_library;
#  elif defined( _WIN32 )
      ::HINSTANCE m_library;
#  else
#    error unsupported platform
#  endif
    };
#endif
  }  // namespace detail

  class Instance;
  class Device;
  using PFN_dummy = void ( * )();

#if VK_NO_PROTOTYPES
#  define VULKAN_HPP_DEFAULT_ADDR( x ) 0
#else
#  define VULKAN_HPP_DEFAULT_ADDR( x ) ::x
#endif

  class DispatchLoader
  {
  public:
    DispatchLoader() VULKAN_HPP_NOEXCEPT                             = default;
    DispatchLoader( DispatchLoader const & rhs ) VULKAN_HPP_NOEXCEPT = default;

    DispatchLoader( PFN_vkGetInstanceProcAddr getInstanceProcAddr ) VULKAN_HPP_NOEXCEPT
    {
      init( getInstanceProcAddr );
    }

    template <typename DynamicLoader
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
              = VULKAN_HPP_NAMESPACE::detail::DynamicLoader
#endif
              >
    void init()
    {
      static DynamicLoader dl;
      init( dl );
    }

    template <typename DynamicLoader>
    void init( DynamicLoader const & dl ) VULKAN_HPP_NOEXCEPT
    {
      PFN_vkGetInstanceProcAddr getInstanceProcAddr = dl.template getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
      init( getInstanceProcAddr );
    }

    void init( PFN_vkGetInstanceProcAddr getInstanceProcAddr ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( getInstanceProcAddr );

      vkGetInstanceProcAddr = getInstanceProcAddr;

      //=== VK_VERSION_1_0 ===
      vkCreateInstance = PFN_vkCreateInstance( vkGetInstanceProcAddr( NULL, "vkCreateInstance" ) );
      vkEnumerateInstanceExtensionProperties =
        PFN_vkEnumerateInstanceExtensionProperties( vkGetInstanceProcAddr( NULL, "vkEnumerateInstanceExtensionProperties" ) );
      vkEnumerateInstanceLayerProperties = PFN_vkEnumerateInstanceLayerProperties( vkGetInstanceProcAddr( NULL, "vkEnumerateInstanceLayerProperties" ) );

      //=== VK_VERSION_1_1 ===
      vkEnumerateInstanceVersion = PFN_vkEnumerateInstanceVersion( vkGetInstanceProcAddr( NULL, "vkEnumerateInstanceVersion" ) );
    }

    void init( VkInstance instance ) VULKAN_HPP_NOEXCEPT
    {
      //=== VK_VERSION_1_0 ===
      vkDestroyInstance                   = PFN_vkDestroyInstance( vkGetInstanceProcAddr( instance, "vkDestroyInstance" ) );
      vkEnumeratePhysicalDevices          = PFN_vkEnumeratePhysicalDevices( vkGetInstanceProcAddr( instance, "vkEnumeratePhysicalDevices" ) );
      vkGetPhysicalDeviceFeatures         = PFN_vkGetPhysicalDeviceFeatures( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFeatures" ) );
      vkGetPhysicalDeviceFormatProperties = PFN_vkGetPhysicalDeviceFormatProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFormatProperties" ) );
      vkGetPhysicalDeviceImageFormatProperties =
        PFN_vkGetPhysicalDeviceImageFormatProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceImageFormatProperties" ) );
      vkGetPhysicalDeviceProperties = PFN_vkGetPhysicalDeviceProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceProperties" ) );
      vkGetPhysicalDeviceQueueFamilyProperties =
        PFN_vkGetPhysicalDeviceQueueFamilyProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceQueueFamilyProperties" ) );
      vkGetPhysicalDeviceMemoryProperties = PFN_vkGetPhysicalDeviceMemoryProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceMemoryProperties" ) );
      vkGetDeviceProcAddr                 = PFN_vkGetDeviceProcAddr( vkGetInstanceProcAddr( instance, "vkGetDeviceProcAddr" ) );
      vkCreateDevice                      = PFN_vkCreateDevice( vkGetInstanceProcAddr( instance, "vkCreateDevice" ) );
      vkDestroyDevice                     = PFN_vkDestroyDevice( vkGetInstanceProcAddr( instance, "vkDestroyDevice" ) );
      vkEnumerateDeviceExtensionProperties =
        PFN_vkEnumerateDeviceExtensionProperties( vkGetInstanceProcAddr( instance, "vkEnumerateDeviceExtensionProperties" ) );
      vkEnumerateDeviceLayerProperties   = PFN_vkEnumerateDeviceLayerProperties( vkGetInstanceProcAddr( instance, "vkEnumerateDeviceLayerProperties" ) );
      vkGetDeviceQueue                   = PFN_vkGetDeviceQueue( vkGetInstanceProcAddr( instance, "vkGetDeviceQueue" ) );
      vkQueueSubmit                      = PFN_vkQueueSubmit( vkGetInstanceProcAddr( instance, "vkQueueSubmit" ) );
      vkQueueWaitIdle                    = PFN_vkQueueWaitIdle( vkGetInstanceProcAddr( instance, "vkQueueWaitIdle" ) );
      vkDeviceWaitIdle                   = PFN_vkDeviceWaitIdle( vkGetInstanceProcAddr( instance, "vkDeviceWaitIdle" ) );
      vkAllocateMemory                   = PFN_vkAllocateMemory( vkGetInstanceProcAddr( instance, "vkAllocateMemory" ) );
      vkFreeMemory                       = PFN_vkFreeMemory( vkGetInstanceProcAddr( instance, "vkFreeMemory" ) );
      vkMapMemory                        = PFN_vkMapMemory( vkGetInstanceProcAddr( instance, "vkMapMemory" ) );
      vkUnmapMemory                      = PFN_vkUnmapMemory( vkGetInstanceProcAddr( instance, "vkUnmapMemory" ) );
      vkFlushMappedMemoryRanges          = PFN_vkFlushMappedMemoryRanges( vkGetInstanceProcAddr( instance, "vkFlushMappedMemoryRanges" ) );
      vkInvalidateMappedMemoryRanges     = PFN_vkInvalidateMappedMemoryRanges( vkGetInstanceProcAddr( instance, "vkInvalidateMappedMemoryRanges" ) );
      vkGetDeviceMemoryCommitment        = PFN_vkGetDeviceMemoryCommitment( vkGetInstanceProcAddr( instance, "vkGetDeviceMemoryCommitment" ) );
      vkBindBufferMemory                 = PFN_vkBindBufferMemory( vkGetInstanceProcAddr( instance, "vkBindBufferMemory" ) );
      vkBindImageMemory                  = PFN_vkBindImageMemory( vkGetInstanceProcAddr( instance, "vkBindImageMemory" ) );
      vkGetBufferMemoryRequirements      = PFN_vkGetBufferMemoryRequirements( vkGetInstanceProcAddr( instance, "vkGetBufferMemoryRequirements" ) );
      vkGetImageMemoryRequirements       = PFN_vkGetImageMemoryRequirements( vkGetInstanceProcAddr( instance, "vkGetImageMemoryRequirements" ) );
      vkGetImageSparseMemoryRequirements = PFN_vkGetImageSparseMemoryRequirements( vkGetInstanceProcAddr( instance, "vkGetImageSparseMemoryRequirements" ) );
      vkGetPhysicalDeviceSparseImageFormatProperties =
        PFN_vkGetPhysicalDeviceSparseImageFormatProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSparseImageFormatProperties" ) );
      vkQueueBindSparse            = PFN_vkQueueBindSparse( vkGetInstanceProcAddr( instance, "vkQueueBindSparse" ) );
      vkCreateFence                = PFN_vkCreateFence( vkGetInstanceProcAddr( instance, "vkCreateFence" ) );
      vkDestroyFence               = PFN_vkDestroyFence( vkGetInstanceProcAddr( instance, "vkDestroyFence" ) );
      vkResetFences                = PFN_vkResetFences( vkGetInstanceProcAddr( instance, "vkResetFences" ) );
      vkGetFenceStatus             = PFN_vkGetFenceStatus( vkGetInstanceProcAddr( instance, "vkGetFenceStatus" ) );
      vkWaitForFences              = PFN_vkWaitForFences( vkGetInstanceProcAddr( instance, "vkWaitForFences" ) );
      vkCreateSemaphore            = PFN_vkCreateSemaphore( vkGetInstanceProcAddr( instance, "vkCreateSemaphore" ) );
      vkDestroySemaphore           = PFN_vkDestroySemaphore( vkGetInstanceProcAddr( instance, "vkDestroySemaphore" ) );
      vkCreateQueryPool            = PFN_vkCreateQueryPool( vkGetInstanceProcAddr( instance, "vkCreateQueryPool" ) );
      vkDestroyQueryPool           = PFN_vkDestroyQueryPool( vkGetInstanceProcAddr( instance, "vkDestroyQueryPool" ) );
      vkGetQueryPoolResults        = PFN_vkGetQueryPoolResults( vkGetInstanceProcAddr( instance, "vkGetQueryPoolResults" ) );
      vkCreateBuffer               = PFN_vkCreateBuffer( vkGetInstanceProcAddr( instance, "vkCreateBuffer" ) );
      vkDestroyBuffer              = PFN_vkDestroyBuffer( vkGetInstanceProcAddr( instance, "vkDestroyBuffer" ) );
      vkCreateImage                = PFN_vkCreateImage( vkGetInstanceProcAddr( instance, "vkCreateImage" ) );
      vkDestroyImage               = PFN_vkDestroyImage( vkGetInstanceProcAddr( instance, "vkDestroyImage" ) );
      vkGetImageSubresourceLayout  = PFN_vkGetImageSubresourceLayout( vkGetInstanceProcAddr( instance, "vkGetImageSubresourceLayout" ) );
      vkCreateImageView            = PFN_vkCreateImageView( vkGetInstanceProcAddr( instance, "vkCreateImageView" ) );
      vkDestroyImageView           = PFN_vkDestroyImageView( vkGetInstanceProcAddr( instance, "vkDestroyImageView" ) );
      vkCreateCommandPool          = PFN_vkCreateCommandPool( vkGetInstanceProcAddr( instance, "vkCreateCommandPool" ) );
      vkDestroyCommandPool         = PFN_vkDestroyCommandPool( vkGetInstanceProcAddr( instance, "vkDestroyCommandPool" ) );
      vkResetCommandPool           = PFN_vkResetCommandPool( vkGetInstanceProcAddr( instance, "vkResetCommandPool" ) );
      vkAllocateCommandBuffers     = PFN_vkAllocateCommandBuffers( vkGetInstanceProcAddr( instance, "vkAllocateCommandBuffers" ) );
      vkFreeCommandBuffers         = PFN_vkFreeCommandBuffers( vkGetInstanceProcAddr( instance, "vkFreeCommandBuffers" ) );
      vkBeginCommandBuffer         = PFN_vkBeginCommandBuffer( vkGetInstanceProcAddr( instance, "vkBeginCommandBuffer" ) );
      vkEndCommandBuffer           = PFN_vkEndCommandBuffer( vkGetInstanceProcAddr( instance, "vkEndCommandBuffer" ) );
      vkResetCommandBuffer         = PFN_vkResetCommandBuffer( vkGetInstanceProcAddr( instance, "vkResetCommandBuffer" ) );
      vkCmdCopyBuffer              = PFN_vkCmdCopyBuffer( vkGetInstanceProcAddr( instance, "vkCmdCopyBuffer" ) );
      vkCmdCopyImage               = PFN_vkCmdCopyImage( vkGetInstanceProcAddr( instance, "vkCmdCopyImage" ) );
      vkCmdCopyBufferToImage       = PFN_vkCmdCopyBufferToImage( vkGetInstanceProcAddr( instance, "vkCmdCopyBufferToImage" ) );
      vkCmdCopyImageToBuffer       = PFN_vkCmdCopyImageToBuffer( vkGetInstanceProcAddr( instance, "vkCmdCopyImageToBuffer" ) );
      vkCmdUpdateBuffer            = PFN_vkCmdUpdateBuffer( vkGetInstanceProcAddr( instance, "vkCmdUpdateBuffer" ) );
      vkCmdFillBuffer              = PFN_vkCmdFillBuffer( vkGetInstanceProcAddr( instance, "vkCmdFillBuffer" ) );
      vkCmdPipelineBarrier         = PFN_vkCmdPipelineBarrier( vkGetInstanceProcAddr( instance, "vkCmdPipelineBarrier" ) );
      vkCmdBeginQuery              = PFN_vkCmdBeginQuery( vkGetInstanceProcAddr( instance, "vkCmdBeginQuery" ) );
      vkCmdEndQuery                = PFN_vkCmdEndQuery( vkGetInstanceProcAddr( instance, "vkCmdEndQuery" ) );
      vkCmdResetQueryPool          = PFN_vkCmdResetQueryPool( vkGetInstanceProcAddr( instance, "vkCmdResetQueryPool" ) );
      vkCmdWriteTimestamp          = PFN_vkCmdWriteTimestamp( vkGetInstanceProcAddr( instance, "vkCmdWriteTimestamp" ) );
      vkCmdCopyQueryPoolResults    = PFN_vkCmdCopyQueryPoolResults( vkGetInstanceProcAddr( instance, "vkCmdCopyQueryPoolResults" ) );
      vkCmdExecuteCommands         = PFN_vkCmdExecuteCommands( vkGetInstanceProcAddr( instance, "vkCmdExecuteCommands" ) );
      vkCreateEvent                = PFN_vkCreateEvent( vkGetInstanceProcAddr( instance, "vkCreateEvent" ) );
      vkDestroyEvent               = PFN_vkDestroyEvent( vkGetInstanceProcAddr( instance, "vkDestroyEvent" ) );
      vkGetEventStatus             = PFN_vkGetEventStatus( vkGetInstanceProcAddr( instance, "vkGetEventStatus" ) );
      vkSetEvent                   = PFN_vkSetEvent( vkGetInstanceProcAddr( instance, "vkSetEvent" ) );
      vkResetEvent                 = PFN_vkResetEvent( vkGetInstanceProcAddr( instance, "vkResetEvent" ) );
      vkCreateBufferView           = PFN_vkCreateBufferView( vkGetInstanceProcAddr( instance, "vkCreateBufferView" ) );
      vkDestroyBufferView          = PFN_vkDestroyBufferView( vkGetInstanceProcAddr( instance, "vkDestroyBufferView" ) );
      vkCreateShaderModule         = PFN_vkCreateShaderModule( vkGetInstanceProcAddr( instance, "vkCreateShaderModule" ) );
      vkDestroyShaderModule        = PFN_vkDestroyShaderModule( vkGetInstanceProcAddr( instance, "vkDestroyShaderModule" ) );
      vkCreatePipelineCache        = PFN_vkCreatePipelineCache( vkGetInstanceProcAddr( instance, "vkCreatePipelineCache" ) );
      vkDestroyPipelineCache       = PFN_vkDestroyPipelineCache( vkGetInstanceProcAddr( instance, "vkDestroyPipelineCache" ) );
      vkGetPipelineCacheData       = PFN_vkGetPipelineCacheData( vkGetInstanceProcAddr( instance, "vkGetPipelineCacheData" ) );
      vkMergePipelineCaches        = PFN_vkMergePipelineCaches( vkGetInstanceProcAddr( instance, "vkMergePipelineCaches" ) );
      vkCreateComputePipelines     = PFN_vkCreateComputePipelines( vkGetInstanceProcAddr( instance, "vkCreateComputePipelines" ) );
      vkDestroyPipeline            = PFN_vkDestroyPipeline( vkGetInstanceProcAddr( instance, "vkDestroyPipeline" ) );
      vkCreatePipelineLayout       = PFN_vkCreatePipelineLayout( vkGetInstanceProcAddr( instance, "vkCreatePipelineLayout" ) );
      vkDestroyPipelineLayout      = PFN_vkDestroyPipelineLayout( vkGetInstanceProcAddr( instance, "vkDestroyPipelineLayout" ) );
      vkCreateSampler              = PFN_vkCreateSampler( vkGetInstanceProcAddr( instance, "vkCreateSampler" ) );
      vkDestroySampler             = PFN_vkDestroySampler( vkGetInstanceProcAddr( instance, "vkDestroySampler" ) );
      vkCreateDescriptorSetLayout  = PFN_vkCreateDescriptorSetLayout( vkGetInstanceProcAddr( instance, "vkCreateDescriptorSetLayout" ) );
      vkDestroyDescriptorSetLayout = PFN_vkDestroyDescriptorSetLayout( vkGetInstanceProcAddr( instance, "vkDestroyDescriptorSetLayout" ) );
      vkCreateDescriptorPool       = PFN_vkCreateDescriptorPool( vkGetInstanceProcAddr( instance, "vkCreateDescriptorPool" ) );
      vkDestroyDescriptorPool      = PFN_vkDestroyDescriptorPool( vkGetInstanceProcAddr( instance, "vkDestroyDescriptorPool" ) );
      vkResetDescriptorPool        = PFN_vkResetDescriptorPool( vkGetInstanceProcAddr( instance, "vkResetDescriptorPool" ) );
      vkAllocateDescriptorSets     = PFN_vkAllocateDescriptorSets( vkGetInstanceProcAddr( instance, "vkAllocateDescriptorSets" ) );
      vkFreeDescriptorSets         = PFN_vkFreeDescriptorSets( vkGetInstanceProcAddr( instance, "vkFreeDescriptorSets" ) );
      vkUpdateDescriptorSets       = PFN_vkUpdateDescriptorSets( vkGetInstanceProcAddr( instance, "vkUpdateDescriptorSets" ) );
      vkCmdBindPipeline            = PFN_vkCmdBindPipeline( vkGetInstanceProcAddr( instance, "vkCmdBindPipeline" ) );
      vkCmdBindDescriptorSets      = PFN_vkCmdBindDescriptorSets( vkGetInstanceProcAddr( instance, "vkCmdBindDescriptorSets" ) );
      vkCmdClearColorImage         = PFN_vkCmdClearColorImage( vkGetInstanceProcAddr( instance, "vkCmdClearColorImage" ) );
      vkCmdDispatch                = PFN_vkCmdDispatch( vkGetInstanceProcAddr( instance, "vkCmdDispatch" ) );
      vkCmdDispatchIndirect        = PFN_vkCmdDispatchIndirect( vkGetInstanceProcAddr( instance, "vkCmdDispatchIndirect" ) );
      vkCmdSetEvent                = PFN_vkCmdSetEvent( vkGetInstanceProcAddr( instance, "vkCmdSetEvent" ) );
      vkCmdResetEvent              = PFN_vkCmdResetEvent( vkGetInstanceProcAddr( instance, "vkCmdResetEvent" ) );
      vkCmdWaitEvents              = PFN_vkCmdWaitEvents( vkGetInstanceProcAddr( instance, "vkCmdWaitEvents" ) );
      vkCmdPushConstants           = PFN_vkCmdPushConstants( vkGetInstanceProcAddr( instance, "vkCmdPushConstants" ) );
      vkCreateGraphicsPipelines    = PFN_vkCreateGraphicsPipelines( vkGetInstanceProcAddr( instance, "vkCreateGraphicsPipelines" ) );
      vkCreateFramebuffer          = PFN_vkCreateFramebuffer( vkGetInstanceProcAddr( instance, "vkCreateFramebuffer" ) );
      vkDestroyFramebuffer         = PFN_vkDestroyFramebuffer( vkGetInstanceProcAddr( instance, "vkDestroyFramebuffer" ) );
      vkCreateRenderPass           = PFN_vkCreateRenderPass( vkGetInstanceProcAddr( instance, "vkCreateRenderPass" ) );
      vkDestroyRenderPass          = PFN_vkDestroyRenderPass( vkGetInstanceProcAddr( instance, "vkDestroyRenderPass" ) );
      vkGetRenderAreaGranularity   = PFN_vkGetRenderAreaGranularity( vkGetInstanceProcAddr( instance, "vkGetRenderAreaGranularity" ) );
      vkCmdSetViewport             = PFN_vkCmdSetViewport( vkGetInstanceProcAddr( instance, "vkCmdSetViewport" ) );
      vkCmdSetScissor              = PFN_vkCmdSetScissor( vkGetInstanceProcAddr( instance, "vkCmdSetScissor" ) );
      vkCmdSetLineWidth            = PFN_vkCmdSetLineWidth( vkGetInstanceProcAddr( instance, "vkCmdSetLineWidth" ) );
      vkCmdSetDepthBias            = PFN_vkCmdSetDepthBias( vkGetInstanceProcAddr( instance, "vkCmdSetDepthBias" ) );
      vkCmdSetBlendConstants       = PFN_vkCmdSetBlendConstants( vkGetInstanceProcAddr( instance, "vkCmdSetBlendConstants" ) );
      vkCmdSetDepthBounds          = PFN_vkCmdSetDepthBounds( vkGetInstanceProcAddr( instance, "vkCmdSetDepthBounds" ) );
      vkCmdSetStencilCompareMask   = PFN_vkCmdSetStencilCompareMask( vkGetInstanceProcAddr( instance, "vkCmdSetStencilCompareMask" ) );
      vkCmdSetStencilWriteMask     = PFN_vkCmdSetStencilWriteMask( vkGetInstanceProcAddr( instance, "vkCmdSetStencilWriteMask" ) );
      vkCmdSetStencilReference     = PFN_vkCmdSetStencilReference( vkGetInstanceProcAddr( instance, "vkCmdSetStencilReference" ) );
      vkCmdBindIndexBuffer         = PFN_vkCmdBindIndexBuffer( vkGetInstanceProcAddr( instance, "vkCmdBindIndexBuffer" ) );
      vkCmdBindVertexBuffers       = PFN_vkCmdBindVertexBuffers( vkGetInstanceProcAddr( instance, "vkCmdBindVertexBuffers" ) );
      vkCmdDraw                    = PFN_vkCmdDraw( vkGetInstanceProcAddr( instance, "vkCmdDraw" ) );
      vkCmdDrawIndexed             = PFN_vkCmdDrawIndexed( vkGetInstanceProcAddr( instance, "vkCmdDrawIndexed" ) );
      vkCmdDrawIndirect            = PFN_vkCmdDrawIndirect( vkGetInstanceProcAddr( instance, "vkCmdDrawIndirect" ) );
      vkCmdDrawIndexedIndirect     = PFN_vkCmdDrawIndexedIndirect( vkGetInstanceProcAddr( instance, "vkCmdDrawIndexedIndirect" ) );
      vkCmdBlitImage               = PFN_vkCmdBlitImage( vkGetInstanceProcAddr( instance, "vkCmdBlitImage" ) );
      vkCmdClearDepthStencilImage  = PFN_vkCmdClearDepthStencilImage( vkGetInstanceProcAddr( instance, "vkCmdClearDepthStencilImage" ) );
      vkCmdClearAttachments        = PFN_vkCmdClearAttachments( vkGetInstanceProcAddr( instance, "vkCmdClearAttachments" ) );
      vkCmdResolveImage            = PFN_vkCmdResolveImage( vkGetInstanceProcAddr( instance, "vkCmdResolveImage" ) );
      vkCmdBeginRenderPass         = PFN_vkCmdBeginRenderPass( vkGetInstanceProcAddr( instance, "vkCmdBeginRenderPass" ) );
      vkCmdNextSubpass             = PFN_vkCmdNextSubpass( vkGetInstanceProcAddr( instance, "vkCmdNextSubpass" ) );
      vkCmdEndRenderPass           = PFN_vkCmdEndRenderPass( vkGetInstanceProcAddr( instance, "vkCmdEndRenderPass" ) );

      //=== VK_VERSION_1_1 ===
      vkBindBufferMemory2                 = PFN_vkBindBufferMemory2( vkGetInstanceProcAddr( instance, "vkBindBufferMemory2" ) );
      vkBindImageMemory2                  = PFN_vkBindImageMemory2( vkGetInstanceProcAddr( instance, "vkBindImageMemory2" ) );
      vkGetDeviceGroupPeerMemoryFeatures  = PFN_vkGetDeviceGroupPeerMemoryFeatures( vkGetInstanceProcAddr( instance, "vkGetDeviceGroupPeerMemoryFeatures" ) );
      vkCmdSetDeviceMask                  = PFN_vkCmdSetDeviceMask( vkGetInstanceProcAddr( instance, "vkCmdSetDeviceMask" ) );
      vkEnumeratePhysicalDeviceGroups     = PFN_vkEnumeratePhysicalDeviceGroups( vkGetInstanceProcAddr( instance, "vkEnumeratePhysicalDeviceGroups" ) );
      vkGetImageMemoryRequirements2       = PFN_vkGetImageMemoryRequirements2( vkGetInstanceProcAddr( instance, "vkGetImageMemoryRequirements2" ) );
      vkGetBufferMemoryRequirements2      = PFN_vkGetBufferMemoryRequirements2( vkGetInstanceProcAddr( instance, "vkGetBufferMemoryRequirements2" ) );
      vkGetImageSparseMemoryRequirements2 = PFN_vkGetImageSparseMemoryRequirements2( vkGetInstanceProcAddr( instance, "vkGetImageSparseMemoryRequirements2" ) );
      vkGetPhysicalDeviceFeatures2        = PFN_vkGetPhysicalDeviceFeatures2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFeatures2" ) );
      vkGetPhysicalDeviceProperties2      = PFN_vkGetPhysicalDeviceProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceProperties2" ) );
      vkGetPhysicalDeviceFormatProperties2 =
        PFN_vkGetPhysicalDeviceFormatProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFormatProperties2" ) );
      vkGetPhysicalDeviceImageFormatProperties2 =
        PFN_vkGetPhysicalDeviceImageFormatProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceImageFormatProperties2" ) );
      vkGetPhysicalDeviceQueueFamilyProperties2 =
        PFN_vkGetPhysicalDeviceQueueFamilyProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceQueueFamilyProperties2" ) );
      vkGetPhysicalDeviceMemoryProperties2 =
        PFN_vkGetPhysicalDeviceMemoryProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceMemoryProperties2" ) );
      vkGetPhysicalDeviceSparseImageFormatProperties2 =
        PFN_vkGetPhysicalDeviceSparseImageFormatProperties2( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSparseImageFormatProperties2" ) );
      vkTrimCommandPool = PFN_vkTrimCommandPool( vkGetInstanceProcAddr( instance, "vkTrimCommandPool" ) );
      vkGetDeviceQueue2 = PFN_vkGetDeviceQueue2( vkGetInstanceProcAddr( instance, "vkGetDeviceQueue2" ) );
      vkGetPhysicalDeviceExternalBufferProperties =
        PFN_vkGetPhysicalDeviceExternalBufferProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalBufferProperties" ) );
      vkGetPhysicalDeviceExternalFenceProperties =
        PFN_vkGetPhysicalDeviceExternalFenceProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalFenceProperties" ) );
      vkGetPhysicalDeviceExternalSemaphoreProperties =
        PFN_vkGetPhysicalDeviceExternalSemaphoreProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalSemaphoreProperties" ) );
      vkCmdDispatchBase                 = PFN_vkCmdDispatchBase( vkGetInstanceProcAddr( instance, "vkCmdDispatchBase" ) );
      vkCreateDescriptorUpdateTemplate  = PFN_vkCreateDescriptorUpdateTemplate( vkGetInstanceProcAddr( instance, "vkCreateDescriptorUpdateTemplate" ) );
      vkDestroyDescriptorUpdateTemplate = PFN_vkDestroyDescriptorUpdateTemplate( vkGetInstanceProcAddr( instance, "vkDestroyDescriptorUpdateTemplate" ) );
      vkUpdateDescriptorSetWithTemplate = PFN_vkUpdateDescriptorSetWithTemplate( vkGetInstanceProcAddr( instance, "vkUpdateDescriptorSetWithTemplate" ) );
      vkGetDescriptorSetLayoutSupport   = PFN_vkGetDescriptorSetLayoutSupport( vkGetInstanceProcAddr( instance, "vkGetDescriptorSetLayoutSupport" ) );
      vkCreateSamplerYcbcrConversion    = PFN_vkCreateSamplerYcbcrConversion( vkGetInstanceProcAddr( instance, "vkCreateSamplerYcbcrConversion" ) );
      vkDestroySamplerYcbcrConversion   = PFN_vkDestroySamplerYcbcrConversion( vkGetInstanceProcAddr( instance, "vkDestroySamplerYcbcrConversion" ) );

      //=== VK_VERSION_1_2 ===
      vkResetQueryPool                = PFN_vkResetQueryPool( vkGetInstanceProcAddr( instance, "vkResetQueryPool" ) );
      vkGetSemaphoreCounterValue      = PFN_vkGetSemaphoreCounterValue( vkGetInstanceProcAddr( instance, "vkGetSemaphoreCounterValue" ) );
      vkWaitSemaphores                = PFN_vkWaitSemaphores( vkGetInstanceProcAddr( instance, "vkWaitSemaphores" ) );
      vkSignalSemaphore               = PFN_vkSignalSemaphore( vkGetInstanceProcAddr( instance, "vkSignalSemaphore" ) );
      vkGetBufferDeviceAddress        = PFN_vkGetBufferDeviceAddress( vkGetInstanceProcAddr( instance, "vkGetBufferDeviceAddress" ) );
      vkGetBufferOpaqueCaptureAddress = PFN_vkGetBufferOpaqueCaptureAddress( vkGetInstanceProcAddr( instance, "vkGetBufferOpaqueCaptureAddress" ) );
      vkGetDeviceMemoryOpaqueCaptureAddress =
        PFN_vkGetDeviceMemoryOpaqueCaptureAddress( vkGetInstanceProcAddr( instance, "vkGetDeviceMemoryOpaqueCaptureAddress" ) );
      vkCmdDrawIndirectCount        = PFN_vkCmdDrawIndirectCount( vkGetInstanceProcAddr( instance, "vkCmdDrawIndirectCount" ) );
      vkCmdDrawIndexedIndirectCount = PFN_vkCmdDrawIndexedIndirectCount( vkGetInstanceProcAddr( instance, "vkCmdDrawIndexedIndirectCount" ) );
      vkCreateRenderPass2           = PFN_vkCreateRenderPass2( vkGetInstanceProcAddr( instance, "vkCreateRenderPass2" ) );
      vkCmdBeginRenderPass2         = PFN_vkCmdBeginRenderPass2( vkGetInstanceProcAddr( instance, "vkCmdBeginRenderPass2" ) );
      vkCmdNextSubpass2             = PFN_vkCmdNextSubpass2( vkGetInstanceProcAddr( instance, "vkCmdNextSubpass2" ) );
      vkCmdEndRenderPass2           = PFN_vkCmdEndRenderPass2( vkGetInstanceProcAddr( instance, "vkCmdEndRenderPass2" ) );

      //=== VK_VERSION_1_3 ===
      vkGetPhysicalDeviceToolProperties   = PFN_vkGetPhysicalDeviceToolProperties( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceToolProperties" ) );
      vkCreatePrivateDataSlot             = PFN_vkCreatePrivateDataSlot( vkGetInstanceProcAddr( instance, "vkCreatePrivateDataSlot" ) );
      vkDestroyPrivateDataSlot            = PFN_vkDestroyPrivateDataSlot( vkGetInstanceProcAddr( instance, "vkDestroyPrivateDataSlot" ) );
      vkSetPrivateData                    = PFN_vkSetPrivateData( vkGetInstanceProcAddr( instance, "vkSetPrivateData" ) );
      vkGetPrivateData                    = PFN_vkGetPrivateData( vkGetInstanceProcAddr( instance, "vkGetPrivateData" ) );
      vkCmdPipelineBarrier2               = PFN_vkCmdPipelineBarrier2( vkGetInstanceProcAddr( instance, "vkCmdPipelineBarrier2" ) );
      vkCmdWriteTimestamp2                = PFN_vkCmdWriteTimestamp2( vkGetInstanceProcAddr( instance, "vkCmdWriteTimestamp2" ) );
      vkQueueSubmit2                      = PFN_vkQueueSubmit2( vkGetInstanceProcAddr( instance, "vkQueueSubmit2" ) );
      vkCmdCopyBuffer2                    = PFN_vkCmdCopyBuffer2( vkGetInstanceProcAddr( instance, "vkCmdCopyBuffer2" ) );
      vkCmdCopyImage2                     = PFN_vkCmdCopyImage2( vkGetInstanceProcAddr( instance, "vkCmdCopyImage2" ) );
      vkCmdCopyBufferToImage2             = PFN_vkCmdCopyBufferToImage2( vkGetInstanceProcAddr( instance, "vkCmdCopyBufferToImage2" ) );
      vkCmdCopyImageToBuffer2             = PFN_vkCmdCopyImageToBuffer2( vkGetInstanceProcAddr( instance, "vkCmdCopyImageToBuffer2" ) );
      vkGetDeviceBufferMemoryRequirements = PFN_vkGetDeviceBufferMemoryRequirements( vkGetInstanceProcAddr( instance, "vkGetDeviceBufferMemoryRequirements" ) );
      vkGetDeviceImageMemoryRequirements  = PFN_vkGetDeviceImageMemoryRequirements( vkGetInstanceProcAddr( instance, "vkGetDeviceImageMemoryRequirements" ) );
      vkGetDeviceImageSparseMemoryRequirements =
        PFN_vkGetDeviceImageSparseMemoryRequirements( vkGetInstanceProcAddr( instance, "vkGetDeviceImageSparseMemoryRequirements" ) );
      vkCmdSetEvent2                  = PFN_vkCmdSetEvent2( vkGetInstanceProcAddr( instance, "vkCmdSetEvent2" ) );
      vkCmdResetEvent2                = PFN_vkCmdResetEvent2( vkGetInstanceProcAddr( instance, "vkCmdResetEvent2" ) );
      vkCmdWaitEvents2                = PFN_vkCmdWaitEvents2( vkGetInstanceProcAddr( instance, "vkCmdWaitEvents2" ) );
      vkCmdBlitImage2                 = PFN_vkCmdBlitImage2( vkGetInstanceProcAddr( instance, "vkCmdBlitImage2" ) );
      vkCmdResolveImage2              = PFN_vkCmdResolveImage2( vkGetInstanceProcAddr( instance, "vkCmdResolveImage2" ) );
      vkCmdBeginRendering             = PFN_vkCmdBeginRendering( vkGetInstanceProcAddr( instance, "vkCmdBeginRendering" ) );
      vkCmdEndRendering               = PFN_vkCmdEndRendering( vkGetInstanceProcAddr( instance, "vkCmdEndRendering" ) );
      vkCmdSetCullMode                = PFN_vkCmdSetCullMode( vkGetInstanceProcAddr( instance, "vkCmdSetCullMode" ) );
      vkCmdSetFrontFace               = PFN_vkCmdSetFrontFace( vkGetInstanceProcAddr( instance, "vkCmdSetFrontFace" ) );
      vkCmdSetPrimitiveTopology       = PFN_vkCmdSetPrimitiveTopology( vkGetInstanceProcAddr( instance, "vkCmdSetPrimitiveTopology" ) );
      vkCmdSetViewportWithCount       = PFN_vkCmdSetViewportWithCount( vkGetInstanceProcAddr( instance, "vkCmdSetViewportWithCount" ) );
      vkCmdSetScissorWithCount        = PFN_vkCmdSetScissorWithCount( vkGetInstanceProcAddr( instance, "vkCmdSetScissorWithCount" ) );
      vkCmdBindVertexBuffers2         = PFN_vkCmdBindVertexBuffers2( vkGetInstanceProcAddr( instance, "vkCmdBindVertexBuffers2" ) );
      vkCmdSetDepthTestEnable         = PFN_vkCmdSetDepthTestEnable( vkGetInstanceProcAddr( instance, "vkCmdSetDepthTestEnable" ) );
      vkCmdSetDepthWriteEnable        = PFN_vkCmdSetDepthWriteEnable( vkGetInstanceProcAddr( instance, "vkCmdSetDepthWriteEnable" ) );
      vkCmdSetDepthCompareOp          = PFN_vkCmdSetDepthCompareOp( vkGetInstanceProcAddr( instance, "vkCmdSetDepthCompareOp" ) );
      vkCmdSetDepthBoundsTestEnable   = PFN_vkCmdSetDepthBoundsTestEnable( vkGetInstanceProcAddr( instance, "vkCmdSetDepthBoundsTestEnable" ) );
      vkCmdSetStencilTestEnable       = PFN_vkCmdSetStencilTestEnable( vkGetInstanceProcAddr( instance, "vkCmdSetStencilTestEnable" ) );
      vkCmdSetStencilOp               = PFN_vkCmdSetStencilOp( vkGetInstanceProcAddr( instance, "vkCmdSetStencilOp" ) );
      vkCmdSetRasterizerDiscardEnable = PFN_vkCmdSetRasterizerDiscardEnable( vkGetInstanceProcAddr( instance, "vkCmdSetRasterizerDiscardEnable" ) );
      vkCmdSetDepthBiasEnable         = PFN_vkCmdSetDepthBiasEnable( vkGetInstanceProcAddr( instance, "vkCmdSetDepthBiasEnable" ) );
      vkCmdSetPrimitiveRestartEnable  = PFN_vkCmdSetPrimitiveRestartEnable( vkGetInstanceProcAddr( instance, "vkCmdSetPrimitiveRestartEnable" ) );

      //=== VK_VERSION_1_4 ===
      vkMapMemory2                        = PFN_vkMapMemory2( vkGetInstanceProcAddr( instance, "vkMapMemory2" ) );
      vkUnmapMemory2                      = PFN_vkUnmapMemory2( vkGetInstanceProcAddr( instance, "vkUnmapMemory2" ) );
      vkGetDeviceImageSubresourceLayout   = PFN_vkGetDeviceImageSubresourceLayout( vkGetInstanceProcAddr( instance, "vkGetDeviceImageSubresourceLayout" ) );
      vkGetImageSubresourceLayout2        = PFN_vkGetImageSubresourceLayout2( vkGetInstanceProcAddr( instance, "vkGetImageSubresourceLayout2" ) );
      vkCopyMemoryToImage                 = PFN_vkCopyMemoryToImage( vkGetInstanceProcAddr( instance, "vkCopyMemoryToImage" ) );
      vkCopyImageToMemory                 = PFN_vkCopyImageToMemory( vkGetInstanceProcAddr( instance, "vkCopyImageToMemory" ) );
      vkCopyImageToImage                  = PFN_vkCopyImageToImage( vkGetInstanceProcAddr( instance, "vkCopyImageToImage" ) );
      vkTransitionImageLayout             = PFN_vkTransitionImageLayout( vkGetInstanceProcAddr( instance, "vkTransitionImageLayout" ) );
      vkCmdPushDescriptorSet              = PFN_vkCmdPushDescriptorSet( vkGetInstanceProcAddr( instance, "vkCmdPushDescriptorSet" ) );
      vkCmdPushDescriptorSetWithTemplate  = PFN_vkCmdPushDescriptorSetWithTemplate( vkGetInstanceProcAddr( instance, "vkCmdPushDescriptorSetWithTemplate" ) );
      vkCmdBindDescriptorSets2            = PFN_vkCmdBindDescriptorSets2( vkGetInstanceProcAddr( instance, "vkCmdBindDescriptorSets2" ) );
      vkCmdPushConstants2                 = PFN_vkCmdPushConstants2( vkGetInstanceProcAddr( instance, "vkCmdPushConstants2" ) );
      vkCmdPushDescriptorSet2             = PFN_vkCmdPushDescriptorSet2( vkGetInstanceProcAddr( instance, "vkCmdPushDescriptorSet2" ) );
      vkCmdPushDescriptorSetWithTemplate2 = PFN_vkCmdPushDescriptorSetWithTemplate2( vkGetInstanceProcAddr( instance, "vkCmdPushDescriptorSetWithTemplate2" ) );
      vkCmdSetLineStipple                 = PFN_vkCmdSetLineStipple( vkGetInstanceProcAddr( instance, "vkCmdSetLineStipple" ) );
      vkCmdBindIndexBuffer2               = PFN_vkCmdBindIndexBuffer2( vkGetInstanceProcAddr( instance, "vkCmdBindIndexBuffer2" ) );
      vkGetRenderingAreaGranularity       = PFN_vkGetRenderingAreaGranularity( vkGetInstanceProcAddr( instance, "vkGetRenderingAreaGranularity" ) );
      vkCmdSetRenderingAttachmentLocations =
        PFN_vkCmdSetRenderingAttachmentLocations( vkGetInstanceProcAddr( instance, "vkCmdSetRenderingAttachmentLocations" ) );
      vkCmdSetRenderingInputAttachmentIndices =
        PFN_vkCmdSetRenderingInputAttachmentIndices( vkGetInstanceProcAddr( instance, "vkCmdSetRenderingInputAttachmentIndices" ) );

      //=== VK_KHR_surface ===
      vkDestroySurfaceKHR = PFN_vkDestroySurfaceKHR( vkGetInstanceProcAddr( instance, "vkDestroySurfaceKHR" ) );
      vkGetPhysicalDeviceSurfaceSupportKHR =
        PFN_vkGetPhysicalDeviceSurfaceSupportKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceSupportKHR" ) );
      vkGetPhysicalDeviceSurfaceCapabilitiesKHR =
        PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR" ) );
      vkGetPhysicalDeviceSurfaceFormatsKHR =
        PFN_vkGetPhysicalDeviceSurfaceFormatsKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceFormatsKHR" ) );
      vkGetPhysicalDeviceSurfacePresentModesKHR =
        PFN_vkGetPhysicalDeviceSurfacePresentModesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfacePresentModesKHR" ) );

      //=== VK_KHR_swapchain ===
      vkCreateSwapchainKHR    = PFN_vkCreateSwapchainKHR( vkGetInstanceProcAddr( instance, "vkCreateSwapchainKHR" ) );
      vkDestroySwapchainKHR   = PFN_vkDestroySwapchainKHR( vkGetInstanceProcAddr( instance, "vkDestroySwapchainKHR" ) );
      vkGetSwapchainImagesKHR = PFN_vkGetSwapchainImagesKHR( vkGetInstanceProcAddr( instance, "vkGetSwapchainImagesKHR" ) );
      vkAcquireNextImageKHR   = PFN_vkAcquireNextImageKHR( vkGetInstanceProcAddr( instance, "vkAcquireNextImageKHR" ) );
      vkQueuePresentKHR       = PFN_vkQueuePresentKHR( vkGetInstanceProcAddr( instance, "vkQueuePresentKHR" ) );
      vkGetDeviceGroupPresentCapabilitiesKHR =
        PFN_vkGetDeviceGroupPresentCapabilitiesKHR( vkGetInstanceProcAddr( instance, "vkGetDeviceGroupPresentCapabilitiesKHR" ) );
      vkGetDeviceGroupSurfacePresentModesKHR =
        PFN_vkGetDeviceGroupSurfacePresentModesKHR( vkGetInstanceProcAddr( instance, "vkGetDeviceGroupSurfacePresentModesKHR" ) );
      vkGetPhysicalDevicePresentRectanglesKHR =
        PFN_vkGetPhysicalDevicePresentRectanglesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDevicePresentRectanglesKHR" ) );
      vkAcquireNextImage2KHR = PFN_vkAcquireNextImage2KHR( vkGetInstanceProcAddr( instance, "vkAcquireNextImage2KHR" ) );

      //=== VK_KHR_display ===
      vkGetPhysicalDeviceDisplayPropertiesKHR =
        PFN_vkGetPhysicalDeviceDisplayPropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDisplayPropertiesKHR" ) );
      vkGetPhysicalDeviceDisplayPlanePropertiesKHR =
        PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR" ) );
      vkGetDisplayPlaneSupportedDisplaysKHR =
        PFN_vkGetDisplayPlaneSupportedDisplaysKHR( vkGetInstanceProcAddr( instance, "vkGetDisplayPlaneSupportedDisplaysKHR" ) );
      vkGetDisplayModePropertiesKHR    = PFN_vkGetDisplayModePropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetDisplayModePropertiesKHR" ) );
      vkCreateDisplayModeKHR           = PFN_vkCreateDisplayModeKHR( vkGetInstanceProcAddr( instance, "vkCreateDisplayModeKHR" ) );
      vkGetDisplayPlaneCapabilitiesKHR = PFN_vkGetDisplayPlaneCapabilitiesKHR( vkGetInstanceProcAddr( instance, "vkGetDisplayPlaneCapabilitiesKHR" ) );
      vkCreateDisplayPlaneSurfaceKHR   = PFN_vkCreateDisplayPlaneSurfaceKHR( vkGetInstanceProcAddr( instance, "vkCreateDisplayPlaneSurfaceKHR" ) );

      //=== VK_KHR_display_swapchain ===
      vkCreateSharedSwapchainsKHR = PFN_vkCreateSharedSwapchainsKHR( vkGetInstanceProcAddr( instance, "vkCreateSharedSwapchainsKHR" ) );

#if defined( VK_USE_PLATFORM_XLIB_KHR )
      //=== VK_KHR_xlib_surface ===
      vkCreateXlibSurfaceKHR = PFN_vkCreateXlibSurfaceKHR( vkGetInstanceProcAddr( instance, "vkCreateXlibSurfaceKHR" ) );
      vkGetPhysicalDeviceXlibPresentationSupportKHR =
        PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceXlibPresentationSupportKHR" ) );
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#if defined( VK_USE_PLATFORM_XCB_KHR )
      //=== VK_KHR_xcb_surface ===
      vkCreateXcbSurfaceKHR = PFN_vkCreateXcbSurfaceKHR( vkGetInstanceProcAddr( instance, "vkCreateXcbSurfaceKHR" ) );
      vkGetPhysicalDeviceXcbPresentationSupportKHR =
        PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceXcbPresentationSupportKHR" ) );
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#if defined( VK_USE_PLATFORM_WAYLAND_KHR )
      //=== VK_KHR_wayland_surface ===
      vkCreateWaylandSurfaceKHR = PFN_vkCreateWaylandSurfaceKHR( vkGetInstanceProcAddr( instance, "vkCreateWaylandSurfaceKHR" ) );
      vkGetPhysicalDeviceWaylandPresentationSupportKHR =
        PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceWaylandPresentationSupportKHR" ) );
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
      //=== VK_KHR_android_surface ===
      vkCreateAndroidSurfaceKHR = PFN_vkCreateAndroidSurfaceKHR( vkGetInstanceProcAddr( instance, "vkCreateAndroidSurfaceKHR" ) );
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_KHR_win32_surface ===
      vkCreateWin32SurfaceKHR = PFN_vkCreateWin32SurfaceKHR( vkGetInstanceProcAddr( instance, "vkCreateWin32SurfaceKHR" ) );
      vkGetPhysicalDeviceWin32PresentationSupportKHR =
        PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceWin32PresentationSupportKHR" ) );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_EXT_debug_report ===
      vkCreateDebugReportCallbackEXT  = PFN_vkCreateDebugReportCallbackEXT( vkGetInstanceProcAddr( instance, "vkCreateDebugReportCallbackEXT" ) );
      vkDestroyDebugReportCallbackEXT = PFN_vkDestroyDebugReportCallbackEXT( vkGetInstanceProcAddr( instance, "vkDestroyDebugReportCallbackEXT" ) );
      vkDebugReportMessageEXT         = PFN_vkDebugReportMessageEXT( vkGetInstanceProcAddr( instance, "vkDebugReportMessageEXT" ) );

      //=== VK_EXT_debug_marker ===
      vkDebugMarkerSetObjectTagEXT  = PFN_vkDebugMarkerSetObjectTagEXT( vkGetInstanceProcAddr( instance, "vkDebugMarkerSetObjectTagEXT" ) );
      vkDebugMarkerSetObjectNameEXT = PFN_vkDebugMarkerSetObjectNameEXT( vkGetInstanceProcAddr( instance, "vkDebugMarkerSetObjectNameEXT" ) );
      vkCmdDebugMarkerBeginEXT      = PFN_vkCmdDebugMarkerBeginEXT( vkGetInstanceProcAddr( instance, "vkCmdDebugMarkerBeginEXT" ) );
      vkCmdDebugMarkerEndEXT        = PFN_vkCmdDebugMarkerEndEXT( vkGetInstanceProcAddr( instance, "vkCmdDebugMarkerEndEXT" ) );
      vkCmdDebugMarkerInsertEXT     = PFN_vkCmdDebugMarkerInsertEXT( vkGetInstanceProcAddr( instance, "vkCmdDebugMarkerInsertEXT" ) );

      //=== VK_KHR_video_queue ===
      vkGetPhysicalDeviceVideoCapabilitiesKHR =
        PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceVideoCapabilitiesKHR" ) );
      vkGetPhysicalDeviceVideoFormatPropertiesKHR =
        PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceVideoFormatPropertiesKHR" ) );
      vkCreateVideoSessionKHR  = PFN_vkCreateVideoSessionKHR( vkGetInstanceProcAddr( instance, "vkCreateVideoSessionKHR" ) );
      vkDestroyVideoSessionKHR = PFN_vkDestroyVideoSessionKHR( vkGetInstanceProcAddr( instance, "vkDestroyVideoSessionKHR" ) );
      vkGetVideoSessionMemoryRequirementsKHR =
        PFN_vkGetVideoSessionMemoryRequirementsKHR( vkGetInstanceProcAddr( instance, "vkGetVideoSessionMemoryRequirementsKHR" ) );
      vkBindVideoSessionMemoryKHR        = PFN_vkBindVideoSessionMemoryKHR( vkGetInstanceProcAddr( instance, "vkBindVideoSessionMemoryKHR" ) );
      vkCreateVideoSessionParametersKHR  = PFN_vkCreateVideoSessionParametersKHR( vkGetInstanceProcAddr( instance, "vkCreateVideoSessionParametersKHR" ) );
      vkUpdateVideoSessionParametersKHR  = PFN_vkUpdateVideoSessionParametersKHR( vkGetInstanceProcAddr( instance, "vkUpdateVideoSessionParametersKHR" ) );
      vkDestroyVideoSessionParametersKHR = PFN_vkDestroyVideoSessionParametersKHR( vkGetInstanceProcAddr( instance, "vkDestroyVideoSessionParametersKHR" ) );
      vkCmdBeginVideoCodingKHR           = PFN_vkCmdBeginVideoCodingKHR( vkGetInstanceProcAddr( instance, "vkCmdBeginVideoCodingKHR" ) );
      vkCmdEndVideoCodingKHR             = PFN_vkCmdEndVideoCodingKHR( vkGetInstanceProcAddr( instance, "vkCmdEndVideoCodingKHR" ) );
      vkCmdControlVideoCodingKHR         = PFN_vkCmdControlVideoCodingKHR( vkGetInstanceProcAddr( instance, "vkCmdControlVideoCodingKHR" ) );

      //=== VK_KHR_video_decode_queue ===
      vkCmdDecodeVideoKHR = PFN_vkCmdDecodeVideoKHR( vkGetInstanceProcAddr( instance, "vkCmdDecodeVideoKHR" ) );

      //=== VK_EXT_transform_feedback ===
      vkCmdBindTransformFeedbackBuffersEXT =
        PFN_vkCmdBindTransformFeedbackBuffersEXT( vkGetInstanceProcAddr( instance, "vkCmdBindTransformFeedbackBuffersEXT" ) );
      vkCmdBeginTransformFeedbackEXT = PFN_vkCmdBeginTransformFeedbackEXT( vkGetInstanceProcAddr( instance, "vkCmdBeginTransformFeedbackEXT" ) );
      vkCmdEndTransformFeedbackEXT   = PFN_vkCmdEndTransformFeedbackEXT( vkGetInstanceProcAddr( instance, "vkCmdEndTransformFeedbackEXT" ) );
      vkCmdBeginQueryIndexedEXT      = PFN_vkCmdBeginQueryIndexedEXT( vkGetInstanceProcAddr( instance, "vkCmdBeginQueryIndexedEXT" ) );
      vkCmdEndQueryIndexedEXT        = PFN_vkCmdEndQueryIndexedEXT( vkGetInstanceProcAddr( instance, "vkCmdEndQueryIndexedEXT" ) );
      vkCmdDrawIndirectByteCountEXT  = PFN_vkCmdDrawIndirectByteCountEXT( vkGetInstanceProcAddr( instance, "vkCmdDrawIndirectByteCountEXT" ) );

      //=== VK_NVX_binary_import ===
      vkCreateCuModuleNVX    = PFN_vkCreateCuModuleNVX( vkGetInstanceProcAddr( instance, "vkCreateCuModuleNVX" ) );
      vkCreateCuFunctionNVX  = PFN_vkCreateCuFunctionNVX( vkGetInstanceProcAddr( instance, "vkCreateCuFunctionNVX" ) );
      vkDestroyCuModuleNVX   = PFN_vkDestroyCuModuleNVX( vkGetInstanceProcAddr( instance, "vkDestroyCuModuleNVX" ) );
      vkDestroyCuFunctionNVX = PFN_vkDestroyCuFunctionNVX( vkGetInstanceProcAddr( instance, "vkDestroyCuFunctionNVX" ) );
      vkCmdCuLaunchKernelNVX = PFN_vkCmdCuLaunchKernelNVX( vkGetInstanceProcAddr( instance, "vkCmdCuLaunchKernelNVX" ) );

      //=== VK_NVX_image_view_handle ===
      vkGetImageViewHandleNVX   = PFN_vkGetImageViewHandleNVX( vkGetInstanceProcAddr( instance, "vkGetImageViewHandleNVX" ) );
      vkGetImageViewHandle64NVX = PFN_vkGetImageViewHandle64NVX( vkGetInstanceProcAddr( instance, "vkGetImageViewHandle64NVX" ) );
      vkGetImageViewAddressNVX  = PFN_vkGetImageViewAddressNVX( vkGetInstanceProcAddr( instance, "vkGetImageViewAddressNVX" ) );
      vkGetDeviceCombinedImageSamplerIndexNVX =
        PFN_vkGetDeviceCombinedImageSamplerIndexNVX( vkGetInstanceProcAddr( instance, "vkGetDeviceCombinedImageSamplerIndexNVX" ) );

      //=== VK_AMD_draw_indirect_count ===
      vkCmdDrawIndirectCountAMD = PFN_vkCmdDrawIndirectCountAMD( vkGetInstanceProcAddr( instance, "vkCmdDrawIndirectCountAMD" ) );
      if ( !vkCmdDrawIndirectCount )
        vkCmdDrawIndirectCount = vkCmdDrawIndirectCountAMD;
      vkCmdDrawIndexedIndirectCountAMD = PFN_vkCmdDrawIndexedIndirectCountAMD( vkGetInstanceProcAddr( instance, "vkCmdDrawIndexedIndirectCountAMD" ) );
      if ( !vkCmdDrawIndexedIndirectCount )
        vkCmdDrawIndexedIndirectCount = vkCmdDrawIndexedIndirectCountAMD;

      //=== VK_AMD_shader_info ===
      vkGetShaderInfoAMD = PFN_vkGetShaderInfoAMD( vkGetInstanceProcAddr( instance, "vkGetShaderInfoAMD" ) );

      //=== VK_KHR_dynamic_rendering ===
      vkCmdBeginRenderingKHR = PFN_vkCmdBeginRenderingKHR( vkGetInstanceProcAddr( instance, "vkCmdBeginRenderingKHR" ) );
      if ( !vkCmdBeginRendering )
        vkCmdBeginRendering = vkCmdBeginRenderingKHR;
      vkCmdEndRenderingKHR = PFN_vkCmdEndRenderingKHR( vkGetInstanceProcAddr( instance, "vkCmdEndRenderingKHR" ) );
      if ( !vkCmdEndRendering )
        vkCmdEndRendering = vkCmdEndRenderingKHR;

#if defined( VK_USE_PLATFORM_GGP )
      //=== VK_GGP_stream_descriptor_surface ===
      vkCreateStreamDescriptorSurfaceGGP = PFN_vkCreateStreamDescriptorSurfaceGGP( vkGetInstanceProcAddr( instance, "vkCreateStreamDescriptorSurfaceGGP" ) );
#endif /*VK_USE_PLATFORM_GGP*/

      //=== VK_NV_external_memory_capabilities ===
      vkGetPhysicalDeviceExternalImageFormatPropertiesNV =
        PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalImageFormatPropertiesNV" ) );

#if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_NV_external_memory_win32 ===
      vkGetMemoryWin32HandleNV = PFN_vkGetMemoryWin32HandleNV( vkGetInstanceProcAddr( instance, "vkGetMemoryWin32HandleNV" ) );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_KHR_get_physical_device_properties2 ===
      vkGetPhysicalDeviceFeatures2KHR = PFN_vkGetPhysicalDeviceFeatures2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFeatures2KHR" ) );
      if ( !vkGetPhysicalDeviceFeatures2 )
        vkGetPhysicalDeviceFeatures2 = vkGetPhysicalDeviceFeatures2KHR;
      vkGetPhysicalDeviceProperties2KHR = PFN_vkGetPhysicalDeviceProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceProperties2KHR" ) );
      if ( !vkGetPhysicalDeviceProperties2 )
        vkGetPhysicalDeviceProperties2 = vkGetPhysicalDeviceProperties2KHR;
      vkGetPhysicalDeviceFormatProperties2KHR =
        PFN_vkGetPhysicalDeviceFormatProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFormatProperties2KHR" ) );
      if ( !vkGetPhysicalDeviceFormatProperties2 )
        vkGetPhysicalDeviceFormatProperties2 = vkGetPhysicalDeviceFormatProperties2KHR;
      vkGetPhysicalDeviceImageFormatProperties2KHR =
        PFN_vkGetPhysicalDeviceImageFormatProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceImageFormatProperties2KHR" ) );
      if ( !vkGetPhysicalDeviceImageFormatProperties2 )
        vkGetPhysicalDeviceImageFormatProperties2 = vkGetPhysicalDeviceImageFormatProperties2KHR;
      vkGetPhysicalDeviceQueueFamilyProperties2KHR =
        PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceQueueFamilyProperties2KHR" ) );
      if ( !vkGetPhysicalDeviceQueueFamilyProperties2 )
        vkGetPhysicalDeviceQueueFamilyProperties2 = vkGetPhysicalDeviceQueueFamilyProperties2KHR;
      vkGetPhysicalDeviceMemoryProperties2KHR =
        PFN_vkGetPhysicalDeviceMemoryProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceMemoryProperties2KHR" ) );
      if ( !vkGetPhysicalDeviceMemoryProperties2 )
        vkGetPhysicalDeviceMemoryProperties2 = vkGetPhysicalDeviceMemoryProperties2KHR;
      vkGetPhysicalDeviceSparseImageFormatProperties2KHR =
        PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSparseImageFormatProperties2KHR" ) );
      if ( !vkGetPhysicalDeviceSparseImageFormatProperties2 )
        vkGetPhysicalDeviceSparseImageFormatProperties2 = vkGetPhysicalDeviceSparseImageFormatProperties2KHR;

      //=== VK_KHR_device_group ===
      vkGetDeviceGroupPeerMemoryFeaturesKHR =
        PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR( vkGetInstanceProcAddr( instance, "vkGetDeviceGroupPeerMemoryFeaturesKHR" ) );
      if ( !vkGetDeviceGroupPeerMemoryFeatures )
        vkGetDeviceGroupPeerMemoryFeatures = vkGetDeviceGroupPeerMemoryFeaturesKHR;
      vkCmdSetDeviceMaskKHR = PFN_vkCmdSetDeviceMaskKHR( vkGetInstanceProcAddr( instance, "vkCmdSetDeviceMaskKHR" ) );
      if ( !vkCmdSetDeviceMask )
        vkCmdSetDeviceMask = vkCmdSetDeviceMaskKHR;
      vkCmdDispatchBaseKHR = PFN_vkCmdDispatchBaseKHR( vkGetInstanceProcAddr( instance, "vkCmdDispatchBaseKHR" ) );
      if ( !vkCmdDispatchBase )
        vkCmdDispatchBase = vkCmdDispatchBaseKHR;

#if defined( VK_USE_PLATFORM_VI_NN )
      //=== VK_NN_vi_surface ===
      vkCreateViSurfaceNN = PFN_vkCreateViSurfaceNN( vkGetInstanceProcAddr( instance, "vkCreateViSurfaceNN" ) );
#endif /*VK_USE_PLATFORM_VI_NN*/

      //=== VK_KHR_maintenance1 ===
      vkTrimCommandPoolKHR = PFN_vkTrimCommandPoolKHR( vkGetInstanceProcAddr( instance, "vkTrimCommandPoolKHR" ) );
      if ( !vkTrimCommandPool )
        vkTrimCommandPool = vkTrimCommandPoolKHR;

      //=== VK_KHR_device_group_creation ===
      vkEnumeratePhysicalDeviceGroupsKHR = PFN_vkEnumeratePhysicalDeviceGroupsKHR( vkGetInstanceProcAddr( instance, "vkEnumeratePhysicalDeviceGroupsKHR" ) );
      if ( !vkEnumeratePhysicalDeviceGroups )
        vkEnumeratePhysicalDeviceGroups = vkEnumeratePhysicalDeviceGroupsKHR;

      //=== VK_KHR_external_memory_capabilities ===
      vkGetPhysicalDeviceExternalBufferPropertiesKHR =
        PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalBufferPropertiesKHR" ) );
      if ( !vkGetPhysicalDeviceExternalBufferProperties )
        vkGetPhysicalDeviceExternalBufferProperties = vkGetPhysicalDeviceExternalBufferPropertiesKHR;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_KHR_external_memory_win32 ===
      vkGetMemoryWin32HandleKHR           = PFN_vkGetMemoryWin32HandleKHR( vkGetInstanceProcAddr( instance, "vkGetMemoryWin32HandleKHR" ) );
      vkGetMemoryWin32HandlePropertiesKHR = PFN_vkGetMemoryWin32HandlePropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetMemoryWin32HandlePropertiesKHR" ) );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_KHR_external_memory_fd ===
      vkGetMemoryFdKHR           = PFN_vkGetMemoryFdKHR( vkGetInstanceProcAddr( instance, "vkGetMemoryFdKHR" ) );
      vkGetMemoryFdPropertiesKHR = PFN_vkGetMemoryFdPropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetMemoryFdPropertiesKHR" ) );

      //=== VK_KHR_external_semaphore_capabilities ===
      vkGetPhysicalDeviceExternalSemaphorePropertiesKHR =
        PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR" ) );
      if ( !vkGetPhysicalDeviceExternalSemaphoreProperties )
        vkGetPhysicalDeviceExternalSemaphoreProperties = vkGetPhysicalDeviceExternalSemaphorePropertiesKHR;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_KHR_external_semaphore_win32 ===
      vkImportSemaphoreWin32HandleKHR = PFN_vkImportSemaphoreWin32HandleKHR( vkGetInstanceProcAddr( instance, "vkImportSemaphoreWin32HandleKHR" ) );
      vkGetSemaphoreWin32HandleKHR    = PFN_vkGetSemaphoreWin32HandleKHR( vkGetInstanceProcAddr( instance, "vkGetSemaphoreWin32HandleKHR" ) );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_KHR_external_semaphore_fd ===
      vkImportSemaphoreFdKHR = PFN_vkImportSemaphoreFdKHR( vkGetInstanceProcAddr( instance, "vkImportSemaphoreFdKHR" ) );
      vkGetSemaphoreFdKHR    = PFN_vkGetSemaphoreFdKHR( vkGetInstanceProcAddr( instance, "vkGetSemaphoreFdKHR" ) );

      //=== VK_KHR_push_descriptor ===
      vkCmdPushDescriptorSetKHR = PFN_vkCmdPushDescriptorSetKHR( vkGetInstanceProcAddr( instance, "vkCmdPushDescriptorSetKHR" ) );
      if ( !vkCmdPushDescriptorSet )
        vkCmdPushDescriptorSet = vkCmdPushDescriptorSetKHR;
      vkCmdPushDescriptorSetWithTemplateKHR =
        PFN_vkCmdPushDescriptorSetWithTemplateKHR( vkGetInstanceProcAddr( instance, "vkCmdPushDescriptorSetWithTemplateKHR" ) );
      if ( !vkCmdPushDescriptorSetWithTemplate )
        vkCmdPushDescriptorSetWithTemplate = vkCmdPushDescriptorSetWithTemplateKHR;

      //=== VK_EXT_conditional_rendering ===
      vkCmdBeginConditionalRenderingEXT = PFN_vkCmdBeginConditionalRenderingEXT( vkGetInstanceProcAddr( instance, "vkCmdBeginConditionalRenderingEXT" ) );
      vkCmdEndConditionalRenderingEXT   = PFN_vkCmdEndConditionalRenderingEXT( vkGetInstanceProcAddr( instance, "vkCmdEndConditionalRenderingEXT" ) );

      //=== VK_KHR_descriptor_update_template ===
      vkCreateDescriptorUpdateTemplateKHR = PFN_vkCreateDescriptorUpdateTemplateKHR( vkGetInstanceProcAddr( instance, "vkCreateDescriptorUpdateTemplateKHR" ) );
      if ( !vkCreateDescriptorUpdateTemplate )
        vkCreateDescriptorUpdateTemplate = vkCreateDescriptorUpdateTemplateKHR;
      vkDestroyDescriptorUpdateTemplateKHR =
        PFN_vkDestroyDescriptorUpdateTemplateKHR( vkGetInstanceProcAddr( instance, "vkDestroyDescriptorUpdateTemplateKHR" ) );
      if ( !vkDestroyDescriptorUpdateTemplate )
        vkDestroyDescriptorUpdateTemplate = vkDestroyDescriptorUpdateTemplateKHR;
      vkUpdateDescriptorSetWithTemplateKHR =
        PFN_vkUpdateDescriptorSetWithTemplateKHR( vkGetInstanceProcAddr( instance, "vkUpdateDescriptorSetWithTemplateKHR" ) );
      if ( !vkUpdateDescriptorSetWithTemplate )
        vkUpdateDescriptorSetWithTemplate = vkUpdateDescriptorSetWithTemplateKHR;

      //=== VK_NV_clip_space_w_scaling ===
      vkCmdSetViewportWScalingNV = PFN_vkCmdSetViewportWScalingNV( vkGetInstanceProcAddr( instance, "vkCmdSetViewportWScalingNV" ) );

      //=== VK_EXT_direct_mode_display ===
      vkReleaseDisplayEXT = PFN_vkReleaseDisplayEXT( vkGetInstanceProcAddr( instance, "vkReleaseDisplayEXT" ) );

#if defined( VK_USE_PLATFORM_XLIB_XRANDR_EXT )
      //=== VK_EXT_acquire_xlib_display ===
      vkAcquireXlibDisplayEXT    = PFN_vkAcquireXlibDisplayEXT( vkGetInstanceProcAddr( instance, "vkAcquireXlibDisplayEXT" ) );
      vkGetRandROutputDisplayEXT = PFN_vkGetRandROutputDisplayEXT( vkGetInstanceProcAddr( instance, "vkGetRandROutputDisplayEXT" ) );
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_EXT*/

      //=== VK_EXT_display_surface_counter ===
      vkGetPhysicalDeviceSurfaceCapabilities2EXT =
        PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceCapabilities2EXT" ) );

      //=== VK_EXT_display_control ===
      vkDisplayPowerControlEXT  = PFN_vkDisplayPowerControlEXT( vkGetInstanceProcAddr( instance, "vkDisplayPowerControlEXT" ) );
      vkRegisterDeviceEventEXT  = PFN_vkRegisterDeviceEventEXT( vkGetInstanceProcAddr( instance, "vkRegisterDeviceEventEXT" ) );
      vkRegisterDisplayEventEXT = PFN_vkRegisterDisplayEventEXT( vkGetInstanceProcAddr( instance, "vkRegisterDisplayEventEXT" ) );
      vkGetSwapchainCounterEXT  = PFN_vkGetSwapchainCounterEXT( vkGetInstanceProcAddr( instance, "vkGetSwapchainCounterEXT" ) );

      //=== VK_GOOGLE_display_timing ===
      vkGetRefreshCycleDurationGOOGLE   = PFN_vkGetRefreshCycleDurationGOOGLE( vkGetInstanceProcAddr( instance, "vkGetRefreshCycleDurationGOOGLE" ) );
      vkGetPastPresentationTimingGOOGLE = PFN_vkGetPastPresentationTimingGOOGLE( vkGetInstanceProcAddr( instance, "vkGetPastPresentationTimingGOOGLE" ) );

      //=== VK_EXT_discard_rectangles ===
      vkCmdSetDiscardRectangleEXT       = PFN_vkCmdSetDiscardRectangleEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDiscardRectangleEXT" ) );
      vkCmdSetDiscardRectangleEnableEXT = PFN_vkCmdSetDiscardRectangleEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDiscardRectangleEnableEXT" ) );
      vkCmdSetDiscardRectangleModeEXT   = PFN_vkCmdSetDiscardRectangleModeEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDiscardRectangleModeEXT" ) );

      //=== VK_EXT_hdr_metadata ===
      vkSetHdrMetadataEXT = PFN_vkSetHdrMetadataEXT( vkGetInstanceProcAddr( instance, "vkSetHdrMetadataEXT" ) );

      //=== VK_KHR_create_renderpass2 ===
      vkCreateRenderPass2KHR = PFN_vkCreateRenderPass2KHR( vkGetInstanceProcAddr( instance, "vkCreateRenderPass2KHR" ) );
      if ( !vkCreateRenderPass2 )
        vkCreateRenderPass2 = vkCreateRenderPass2KHR;
      vkCmdBeginRenderPass2KHR = PFN_vkCmdBeginRenderPass2KHR( vkGetInstanceProcAddr( instance, "vkCmdBeginRenderPass2KHR" ) );
      if ( !vkCmdBeginRenderPass2 )
        vkCmdBeginRenderPass2 = vkCmdBeginRenderPass2KHR;
      vkCmdNextSubpass2KHR = PFN_vkCmdNextSubpass2KHR( vkGetInstanceProcAddr( instance, "vkCmdNextSubpass2KHR" ) );
      if ( !vkCmdNextSubpass2 )
        vkCmdNextSubpass2 = vkCmdNextSubpass2KHR;
      vkCmdEndRenderPass2KHR = PFN_vkCmdEndRenderPass2KHR( vkGetInstanceProcAddr( instance, "vkCmdEndRenderPass2KHR" ) );
      if ( !vkCmdEndRenderPass2 )
        vkCmdEndRenderPass2 = vkCmdEndRenderPass2KHR;

      //=== VK_KHR_shared_presentable_image ===
      vkGetSwapchainStatusKHR = PFN_vkGetSwapchainStatusKHR( vkGetInstanceProcAddr( instance, "vkGetSwapchainStatusKHR" ) );

      //=== VK_KHR_external_fence_capabilities ===
      vkGetPhysicalDeviceExternalFencePropertiesKHR =
        PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalFencePropertiesKHR" ) );
      if ( !vkGetPhysicalDeviceExternalFenceProperties )
        vkGetPhysicalDeviceExternalFenceProperties = vkGetPhysicalDeviceExternalFencePropertiesKHR;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_KHR_external_fence_win32 ===
      vkImportFenceWin32HandleKHR = PFN_vkImportFenceWin32HandleKHR( vkGetInstanceProcAddr( instance, "vkImportFenceWin32HandleKHR" ) );
      vkGetFenceWin32HandleKHR    = PFN_vkGetFenceWin32HandleKHR( vkGetInstanceProcAddr( instance, "vkGetFenceWin32HandleKHR" ) );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_KHR_external_fence_fd ===
      vkImportFenceFdKHR = PFN_vkImportFenceFdKHR( vkGetInstanceProcAddr( instance, "vkImportFenceFdKHR" ) );
      vkGetFenceFdKHR    = PFN_vkGetFenceFdKHR( vkGetInstanceProcAddr( instance, "vkGetFenceFdKHR" ) );

      //=== VK_KHR_performance_query ===
      vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(
        vkGetInstanceProcAddr( instance, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR" ) );
      vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR = PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(
        vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR" ) );
      vkAcquireProfilingLockKHR = PFN_vkAcquireProfilingLockKHR( vkGetInstanceProcAddr( instance, "vkAcquireProfilingLockKHR" ) );
      vkReleaseProfilingLockKHR = PFN_vkReleaseProfilingLockKHR( vkGetInstanceProcAddr( instance, "vkReleaseProfilingLockKHR" ) );

      //=== VK_KHR_get_surface_capabilities2 ===
      vkGetPhysicalDeviceSurfaceCapabilities2KHR =
        PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceCapabilities2KHR" ) );
      vkGetPhysicalDeviceSurfaceFormats2KHR =
        PFN_vkGetPhysicalDeviceSurfaceFormats2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfaceFormats2KHR" ) );

      //=== VK_KHR_get_display_properties2 ===
      vkGetPhysicalDeviceDisplayProperties2KHR =
        PFN_vkGetPhysicalDeviceDisplayProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDisplayProperties2KHR" ) );
      vkGetPhysicalDeviceDisplayPlaneProperties2KHR =
        PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR" ) );
      vkGetDisplayModeProperties2KHR    = PFN_vkGetDisplayModeProperties2KHR( vkGetInstanceProcAddr( instance, "vkGetDisplayModeProperties2KHR" ) );
      vkGetDisplayPlaneCapabilities2KHR = PFN_vkGetDisplayPlaneCapabilities2KHR( vkGetInstanceProcAddr( instance, "vkGetDisplayPlaneCapabilities2KHR" ) );

#if defined( VK_USE_PLATFORM_IOS_MVK )
      //=== VK_MVK_ios_surface ===
      vkCreateIOSSurfaceMVK = PFN_vkCreateIOSSurfaceMVK( vkGetInstanceProcAddr( instance, "vkCreateIOSSurfaceMVK" ) );
#endif /*VK_USE_PLATFORM_IOS_MVK*/

#if defined( VK_USE_PLATFORM_MACOS_MVK )
      //=== VK_MVK_macos_surface ===
      vkCreateMacOSSurfaceMVK = PFN_vkCreateMacOSSurfaceMVK( vkGetInstanceProcAddr( instance, "vkCreateMacOSSurfaceMVK" ) );
#endif /*VK_USE_PLATFORM_MACOS_MVK*/

      //=== VK_EXT_debug_utils ===
      vkSetDebugUtilsObjectNameEXT    = PFN_vkSetDebugUtilsObjectNameEXT( vkGetInstanceProcAddr( instance, "vkSetDebugUtilsObjectNameEXT" ) );
      vkSetDebugUtilsObjectTagEXT     = PFN_vkSetDebugUtilsObjectTagEXT( vkGetInstanceProcAddr( instance, "vkSetDebugUtilsObjectTagEXT" ) );
      vkQueueBeginDebugUtilsLabelEXT  = PFN_vkQueueBeginDebugUtilsLabelEXT( vkGetInstanceProcAddr( instance, "vkQueueBeginDebugUtilsLabelEXT" ) );
      vkQueueEndDebugUtilsLabelEXT    = PFN_vkQueueEndDebugUtilsLabelEXT( vkGetInstanceProcAddr( instance, "vkQueueEndDebugUtilsLabelEXT" ) );
      vkQueueInsertDebugUtilsLabelEXT = PFN_vkQueueInsertDebugUtilsLabelEXT( vkGetInstanceProcAddr( instance, "vkQueueInsertDebugUtilsLabelEXT" ) );
      vkCmdBeginDebugUtilsLabelEXT    = PFN_vkCmdBeginDebugUtilsLabelEXT( vkGetInstanceProcAddr( instance, "vkCmdBeginDebugUtilsLabelEXT" ) );
      vkCmdEndDebugUtilsLabelEXT      = PFN_vkCmdEndDebugUtilsLabelEXT( vkGetInstanceProcAddr( instance, "vkCmdEndDebugUtilsLabelEXT" ) );
      vkCmdInsertDebugUtilsLabelEXT   = PFN_vkCmdInsertDebugUtilsLabelEXT( vkGetInstanceProcAddr( instance, "vkCmdInsertDebugUtilsLabelEXT" ) );
      vkCreateDebugUtilsMessengerEXT  = PFN_vkCreateDebugUtilsMessengerEXT( vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" ) );
      vkDestroyDebugUtilsMessengerEXT = PFN_vkDestroyDebugUtilsMessengerEXT( vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT" ) );
      vkSubmitDebugUtilsMessageEXT    = PFN_vkSubmitDebugUtilsMessageEXT( vkGetInstanceProcAddr( instance, "vkSubmitDebugUtilsMessageEXT" ) );

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
      //=== VK_ANDROID_external_memory_android_hardware_buffer ===
      vkGetAndroidHardwareBufferPropertiesANDROID =
        PFN_vkGetAndroidHardwareBufferPropertiesANDROID( vkGetInstanceProcAddr( instance, "vkGetAndroidHardwareBufferPropertiesANDROID" ) );
      vkGetMemoryAndroidHardwareBufferANDROID =
        PFN_vkGetMemoryAndroidHardwareBufferANDROID( vkGetInstanceProcAddr( instance, "vkGetMemoryAndroidHardwareBufferANDROID" ) );
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#if defined( VK_ENABLE_BETA_EXTENSIONS )
      //=== VK_AMDX_shader_enqueue ===
      vkCreateExecutionGraphPipelinesAMDX = PFN_vkCreateExecutionGraphPipelinesAMDX( vkGetInstanceProcAddr( instance, "vkCreateExecutionGraphPipelinesAMDX" ) );
      vkGetExecutionGraphPipelineScratchSizeAMDX =
        PFN_vkGetExecutionGraphPipelineScratchSizeAMDX( vkGetInstanceProcAddr( instance, "vkGetExecutionGraphPipelineScratchSizeAMDX" ) );
      vkGetExecutionGraphPipelineNodeIndexAMDX =
        PFN_vkGetExecutionGraphPipelineNodeIndexAMDX( vkGetInstanceProcAddr( instance, "vkGetExecutionGraphPipelineNodeIndexAMDX" ) );
      vkCmdInitializeGraphScratchMemoryAMDX =
        PFN_vkCmdInitializeGraphScratchMemoryAMDX( vkGetInstanceProcAddr( instance, "vkCmdInitializeGraphScratchMemoryAMDX" ) );
      vkCmdDispatchGraphAMDX              = PFN_vkCmdDispatchGraphAMDX( vkGetInstanceProcAddr( instance, "vkCmdDispatchGraphAMDX" ) );
      vkCmdDispatchGraphIndirectAMDX      = PFN_vkCmdDispatchGraphIndirectAMDX( vkGetInstanceProcAddr( instance, "vkCmdDispatchGraphIndirectAMDX" ) );
      vkCmdDispatchGraphIndirectCountAMDX = PFN_vkCmdDispatchGraphIndirectCountAMDX( vkGetInstanceProcAddr( instance, "vkCmdDispatchGraphIndirectCountAMDX" ) );
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

      //=== VK_EXT_descriptor_heap ===
      vkWriteSamplerDescriptorsEXT   = PFN_vkWriteSamplerDescriptorsEXT( vkGetInstanceProcAddr( instance, "vkWriteSamplerDescriptorsEXT" ) );
      vkWriteResourceDescriptorsEXT  = PFN_vkWriteResourceDescriptorsEXT( vkGetInstanceProcAddr( instance, "vkWriteResourceDescriptorsEXT" ) );
      vkCmdBindSamplerHeapEXT        = PFN_vkCmdBindSamplerHeapEXT( vkGetInstanceProcAddr( instance, "vkCmdBindSamplerHeapEXT" ) );
      vkCmdBindResourceHeapEXT       = PFN_vkCmdBindResourceHeapEXT( vkGetInstanceProcAddr( instance, "vkCmdBindResourceHeapEXT" ) );
      vkCmdPushDataEXT               = PFN_vkCmdPushDataEXT( vkGetInstanceProcAddr( instance, "vkCmdPushDataEXT" ) );
      vkGetImageOpaqueCaptureDataEXT = PFN_vkGetImageOpaqueCaptureDataEXT( vkGetInstanceProcAddr( instance, "vkGetImageOpaqueCaptureDataEXT" ) );
      vkGetPhysicalDeviceDescriptorSizeEXT =
        PFN_vkGetPhysicalDeviceDescriptorSizeEXT( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDescriptorSizeEXT" ) );
      vkRegisterCustomBorderColorEXT   = PFN_vkRegisterCustomBorderColorEXT( vkGetInstanceProcAddr( instance, "vkRegisterCustomBorderColorEXT" ) );
      vkUnregisterCustomBorderColorEXT = PFN_vkUnregisterCustomBorderColorEXT( vkGetInstanceProcAddr( instance, "vkUnregisterCustomBorderColorEXT" ) );
      vkGetTensorOpaqueCaptureDataARM  = PFN_vkGetTensorOpaqueCaptureDataARM( vkGetInstanceProcAddr( instance, "vkGetTensorOpaqueCaptureDataARM" ) );

      //=== VK_EXT_sample_locations ===
      vkCmdSetSampleLocationsEXT = PFN_vkCmdSetSampleLocationsEXT( vkGetInstanceProcAddr( instance, "vkCmdSetSampleLocationsEXT" ) );
      vkGetPhysicalDeviceMultisamplePropertiesEXT =
        PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceMultisamplePropertiesEXT" ) );

      //=== VK_KHR_get_memory_requirements2 ===
      vkGetImageMemoryRequirements2KHR = PFN_vkGetImageMemoryRequirements2KHR( vkGetInstanceProcAddr( instance, "vkGetImageMemoryRequirements2KHR" ) );
      if ( !vkGetImageMemoryRequirements2 )
        vkGetImageMemoryRequirements2 = vkGetImageMemoryRequirements2KHR;
      vkGetBufferMemoryRequirements2KHR = PFN_vkGetBufferMemoryRequirements2KHR( vkGetInstanceProcAddr( instance, "vkGetBufferMemoryRequirements2KHR" ) );
      if ( !vkGetBufferMemoryRequirements2 )
        vkGetBufferMemoryRequirements2 = vkGetBufferMemoryRequirements2KHR;
      vkGetImageSparseMemoryRequirements2KHR =
        PFN_vkGetImageSparseMemoryRequirements2KHR( vkGetInstanceProcAddr( instance, "vkGetImageSparseMemoryRequirements2KHR" ) );
      if ( !vkGetImageSparseMemoryRequirements2 )
        vkGetImageSparseMemoryRequirements2 = vkGetImageSparseMemoryRequirements2KHR;

      //=== VK_KHR_acceleration_structure ===
      vkCreateAccelerationStructureKHR    = PFN_vkCreateAccelerationStructureKHR( vkGetInstanceProcAddr( instance, "vkCreateAccelerationStructureKHR" ) );
      vkDestroyAccelerationStructureKHR   = PFN_vkDestroyAccelerationStructureKHR( vkGetInstanceProcAddr( instance, "vkDestroyAccelerationStructureKHR" ) );
      vkCmdBuildAccelerationStructuresKHR = PFN_vkCmdBuildAccelerationStructuresKHR( vkGetInstanceProcAddr( instance, "vkCmdBuildAccelerationStructuresKHR" ) );
      vkCmdBuildAccelerationStructuresIndirectKHR =
        PFN_vkCmdBuildAccelerationStructuresIndirectKHR( vkGetInstanceProcAddr( instance, "vkCmdBuildAccelerationStructuresIndirectKHR" ) );
      vkBuildAccelerationStructuresKHR = PFN_vkBuildAccelerationStructuresKHR( vkGetInstanceProcAddr( instance, "vkBuildAccelerationStructuresKHR" ) );
      vkCopyAccelerationStructureKHR   = PFN_vkCopyAccelerationStructureKHR( vkGetInstanceProcAddr( instance, "vkCopyAccelerationStructureKHR" ) );
      vkCopyAccelerationStructureToMemoryKHR =
        PFN_vkCopyAccelerationStructureToMemoryKHR( vkGetInstanceProcAddr( instance, "vkCopyAccelerationStructureToMemoryKHR" ) );
      vkCopyMemoryToAccelerationStructureKHR =
        PFN_vkCopyMemoryToAccelerationStructureKHR( vkGetInstanceProcAddr( instance, "vkCopyMemoryToAccelerationStructureKHR" ) );
      vkWriteAccelerationStructuresPropertiesKHR =
        PFN_vkWriteAccelerationStructuresPropertiesKHR( vkGetInstanceProcAddr( instance, "vkWriteAccelerationStructuresPropertiesKHR" ) );
      vkCmdCopyAccelerationStructureKHR = PFN_vkCmdCopyAccelerationStructureKHR( vkGetInstanceProcAddr( instance, "vkCmdCopyAccelerationStructureKHR" ) );
      vkCmdCopyAccelerationStructureToMemoryKHR =
        PFN_vkCmdCopyAccelerationStructureToMemoryKHR( vkGetInstanceProcAddr( instance, "vkCmdCopyAccelerationStructureToMemoryKHR" ) );
      vkCmdCopyMemoryToAccelerationStructureKHR =
        PFN_vkCmdCopyMemoryToAccelerationStructureKHR( vkGetInstanceProcAddr( instance, "vkCmdCopyMemoryToAccelerationStructureKHR" ) );
      vkGetAccelerationStructureDeviceAddressKHR =
        PFN_vkGetAccelerationStructureDeviceAddressKHR( vkGetInstanceProcAddr( instance, "vkGetAccelerationStructureDeviceAddressKHR" ) );
      vkCmdWriteAccelerationStructuresPropertiesKHR =
        PFN_vkCmdWriteAccelerationStructuresPropertiesKHR( vkGetInstanceProcAddr( instance, "vkCmdWriteAccelerationStructuresPropertiesKHR" ) );
      vkGetDeviceAccelerationStructureCompatibilityKHR =
        PFN_vkGetDeviceAccelerationStructureCompatibilityKHR( vkGetInstanceProcAddr( instance, "vkGetDeviceAccelerationStructureCompatibilityKHR" ) );
      vkGetAccelerationStructureBuildSizesKHR =
        PFN_vkGetAccelerationStructureBuildSizesKHR( vkGetInstanceProcAddr( instance, "vkGetAccelerationStructureBuildSizesKHR" ) );

      //=== VK_KHR_ray_tracing_pipeline ===
      vkCmdTraceRaysKHR              = PFN_vkCmdTraceRaysKHR( vkGetInstanceProcAddr( instance, "vkCmdTraceRaysKHR" ) );
      vkCreateRayTracingPipelinesKHR = PFN_vkCreateRayTracingPipelinesKHR( vkGetInstanceProcAddr( instance, "vkCreateRayTracingPipelinesKHR" ) );
      vkGetRayTracingShaderGroupHandlesKHR =
        PFN_vkGetRayTracingShaderGroupHandlesKHR( vkGetInstanceProcAddr( instance, "vkGetRayTracingShaderGroupHandlesKHR" ) );
      vkGetRayTracingCaptureReplayShaderGroupHandlesKHR =
        PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR( vkGetInstanceProcAddr( instance, "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR" ) );
      vkCmdTraceRaysIndirectKHR = PFN_vkCmdTraceRaysIndirectKHR( vkGetInstanceProcAddr( instance, "vkCmdTraceRaysIndirectKHR" ) );
      vkGetRayTracingShaderGroupStackSizeKHR =
        PFN_vkGetRayTracingShaderGroupStackSizeKHR( vkGetInstanceProcAddr( instance, "vkGetRayTracingShaderGroupStackSizeKHR" ) );
      vkCmdSetRayTracingPipelineStackSizeKHR =
        PFN_vkCmdSetRayTracingPipelineStackSizeKHR( vkGetInstanceProcAddr( instance, "vkCmdSetRayTracingPipelineStackSizeKHR" ) );

      //=== VK_KHR_sampler_ycbcr_conversion ===
      vkCreateSamplerYcbcrConversionKHR = PFN_vkCreateSamplerYcbcrConversionKHR( vkGetInstanceProcAddr( instance, "vkCreateSamplerYcbcrConversionKHR" ) );
      if ( !vkCreateSamplerYcbcrConversion )
        vkCreateSamplerYcbcrConversion = vkCreateSamplerYcbcrConversionKHR;
      vkDestroySamplerYcbcrConversionKHR = PFN_vkDestroySamplerYcbcrConversionKHR( vkGetInstanceProcAddr( instance, "vkDestroySamplerYcbcrConversionKHR" ) );
      if ( !vkDestroySamplerYcbcrConversion )
        vkDestroySamplerYcbcrConversion = vkDestroySamplerYcbcrConversionKHR;

      //=== VK_KHR_bind_memory2 ===
      vkBindBufferMemory2KHR = PFN_vkBindBufferMemory2KHR( vkGetInstanceProcAddr( instance, "vkBindBufferMemory2KHR" ) );
      if ( !vkBindBufferMemory2 )
        vkBindBufferMemory2 = vkBindBufferMemory2KHR;
      vkBindImageMemory2KHR = PFN_vkBindImageMemory2KHR( vkGetInstanceProcAddr( instance, "vkBindImageMemory2KHR" ) );
      if ( !vkBindImageMemory2 )
        vkBindImageMemory2 = vkBindImageMemory2KHR;

      //=== VK_EXT_image_drm_format_modifier ===
      vkGetImageDrmFormatModifierPropertiesEXT =
        PFN_vkGetImageDrmFormatModifierPropertiesEXT( vkGetInstanceProcAddr( instance, "vkGetImageDrmFormatModifierPropertiesEXT" ) );

      //=== VK_EXT_validation_cache ===
      vkCreateValidationCacheEXT  = PFN_vkCreateValidationCacheEXT( vkGetInstanceProcAddr( instance, "vkCreateValidationCacheEXT" ) );
      vkDestroyValidationCacheEXT = PFN_vkDestroyValidationCacheEXT( vkGetInstanceProcAddr( instance, "vkDestroyValidationCacheEXT" ) );
      vkMergeValidationCachesEXT  = PFN_vkMergeValidationCachesEXT( vkGetInstanceProcAddr( instance, "vkMergeValidationCachesEXT" ) );
      vkGetValidationCacheDataEXT = PFN_vkGetValidationCacheDataEXT( vkGetInstanceProcAddr( instance, "vkGetValidationCacheDataEXT" ) );

      //=== VK_NV_shading_rate_image ===
      vkCmdBindShadingRateImageNV = PFN_vkCmdBindShadingRateImageNV( vkGetInstanceProcAddr( instance, "vkCmdBindShadingRateImageNV" ) );
      vkCmdSetViewportShadingRatePaletteNV =
        PFN_vkCmdSetViewportShadingRatePaletteNV( vkGetInstanceProcAddr( instance, "vkCmdSetViewportShadingRatePaletteNV" ) );
      vkCmdSetCoarseSampleOrderNV = PFN_vkCmdSetCoarseSampleOrderNV( vkGetInstanceProcAddr( instance, "vkCmdSetCoarseSampleOrderNV" ) );

      //=== VK_NV_ray_tracing ===
      vkCreateAccelerationStructureNV  = PFN_vkCreateAccelerationStructureNV( vkGetInstanceProcAddr( instance, "vkCreateAccelerationStructureNV" ) );
      vkDestroyAccelerationStructureNV = PFN_vkDestroyAccelerationStructureNV( vkGetInstanceProcAddr( instance, "vkDestroyAccelerationStructureNV" ) );
      vkGetAccelerationStructureMemoryRequirementsNV =
        PFN_vkGetAccelerationStructureMemoryRequirementsNV( vkGetInstanceProcAddr( instance, "vkGetAccelerationStructureMemoryRequirementsNV" ) );
      vkBindAccelerationStructureMemoryNV = PFN_vkBindAccelerationStructureMemoryNV( vkGetInstanceProcAddr( instance, "vkBindAccelerationStructureMemoryNV" ) );
      vkCmdBuildAccelerationStructureNV   = PFN_vkCmdBuildAccelerationStructureNV( vkGetInstanceProcAddr( instance, "vkCmdBuildAccelerationStructureNV" ) );
      vkCmdCopyAccelerationStructureNV    = PFN_vkCmdCopyAccelerationStructureNV( vkGetInstanceProcAddr( instance, "vkCmdCopyAccelerationStructureNV" ) );
      vkCmdTraceRaysNV                    = PFN_vkCmdTraceRaysNV( vkGetInstanceProcAddr( instance, "vkCmdTraceRaysNV" ) );
      vkCreateRayTracingPipelinesNV       = PFN_vkCreateRayTracingPipelinesNV( vkGetInstanceProcAddr( instance, "vkCreateRayTracingPipelinesNV" ) );
      vkGetRayTracingShaderGroupHandlesNV = PFN_vkGetRayTracingShaderGroupHandlesNV( vkGetInstanceProcAddr( instance, "vkGetRayTracingShaderGroupHandlesNV" ) );
      if ( !vkGetRayTracingShaderGroupHandlesKHR )
        vkGetRayTracingShaderGroupHandlesKHR = vkGetRayTracingShaderGroupHandlesNV;
      vkGetAccelerationStructureHandleNV = PFN_vkGetAccelerationStructureHandleNV( vkGetInstanceProcAddr( instance, "vkGetAccelerationStructureHandleNV" ) );
      vkCmdWriteAccelerationStructuresPropertiesNV =
        PFN_vkCmdWriteAccelerationStructuresPropertiesNV( vkGetInstanceProcAddr( instance, "vkCmdWriteAccelerationStructuresPropertiesNV" ) );
      vkCompileDeferredNV = PFN_vkCompileDeferredNV( vkGetInstanceProcAddr( instance, "vkCompileDeferredNV" ) );

      //=== VK_KHR_maintenance3 ===
      vkGetDescriptorSetLayoutSupportKHR = PFN_vkGetDescriptorSetLayoutSupportKHR( vkGetInstanceProcAddr( instance, "vkGetDescriptorSetLayoutSupportKHR" ) );
      if ( !vkGetDescriptorSetLayoutSupport )
        vkGetDescriptorSetLayoutSupport = vkGetDescriptorSetLayoutSupportKHR;

      //=== VK_KHR_draw_indirect_count ===
      vkCmdDrawIndirectCountKHR = PFN_vkCmdDrawIndirectCountKHR( vkGetInstanceProcAddr( instance, "vkCmdDrawIndirectCountKHR" ) );
      if ( !vkCmdDrawIndirectCount )
        vkCmdDrawIndirectCount = vkCmdDrawIndirectCountKHR;
      vkCmdDrawIndexedIndirectCountKHR = PFN_vkCmdDrawIndexedIndirectCountKHR( vkGetInstanceProcAddr( instance, "vkCmdDrawIndexedIndirectCountKHR" ) );
      if ( !vkCmdDrawIndexedIndirectCount )
        vkCmdDrawIndexedIndirectCount = vkCmdDrawIndexedIndirectCountKHR;

      //=== VK_EXT_external_memory_host ===
      vkGetMemoryHostPointerPropertiesEXT = PFN_vkGetMemoryHostPointerPropertiesEXT( vkGetInstanceProcAddr( instance, "vkGetMemoryHostPointerPropertiesEXT" ) );

      //=== VK_AMD_buffer_marker ===
      vkCmdWriteBufferMarkerAMD  = PFN_vkCmdWriteBufferMarkerAMD( vkGetInstanceProcAddr( instance, "vkCmdWriteBufferMarkerAMD" ) );
      vkCmdWriteBufferMarker2AMD = PFN_vkCmdWriteBufferMarker2AMD( vkGetInstanceProcAddr( instance, "vkCmdWriteBufferMarker2AMD" ) );

      //=== VK_EXT_calibrated_timestamps ===
      vkGetPhysicalDeviceCalibrateableTimeDomainsEXT =
        PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT" ) );
      if ( !vkGetPhysicalDeviceCalibrateableTimeDomainsKHR )
        vkGetPhysicalDeviceCalibrateableTimeDomainsKHR = vkGetPhysicalDeviceCalibrateableTimeDomainsEXT;
      vkGetCalibratedTimestampsEXT = PFN_vkGetCalibratedTimestampsEXT( vkGetInstanceProcAddr( instance, "vkGetCalibratedTimestampsEXT" ) );
      if ( !vkGetCalibratedTimestampsKHR )
        vkGetCalibratedTimestampsKHR = vkGetCalibratedTimestampsEXT;

      //=== VK_NV_mesh_shader ===
      vkCmdDrawMeshTasksNV              = PFN_vkCmdDrawMeshTasksNV( vkGetInstanceProcAddr( instance, "vkCmdDrawMeshTasksNV" ) );
      vkCmdDrawMeshTasksIndirectNV      = PFN_vkCmdDrawMeshTasksIndirectNV( vkGetInstanceProcAddr( instance, "vkCmdDrawMeshTasksIndirectNV" ) );
      vkCmdDrawMeshTasksIndirectCountNV = PFN_vkCmdDrawMeshTasksIndirectCountNV( vkGetInstanceProcAddr( instance, "vkCmdDrawMeshTasksIndirectCountNV" ) );

      //=== VK_NV_scissor_exclusive ===
      vkCmdSetExclusiveScissorEnableNV = PFN_vkCmdSetExclusiveScissorEnableNV( vkGetInstanceProcAddr( instance, "vkCmdSetExclusiveScissorEnableNV" ) );
      vkCmdSetExclusiveScissorNV       = PFN_vkCmdSetExclusiveScissorNV( vkGetInstanceProcAddr( instance, "vkCmdSetExclusiveScissorNV" ) );

      //=== VK_NV_device_diagnostic_checkpoints ===
      vkCmdSetCheckpointNV        = PFN_vkCmdSetCheckpointNV( vkGetInstanceProcAddr( instance, "vkCmdSetCheckpointNV" ) );
      vkGetQueueCheckpointDataNV  = PFN_vkGetQueueCheckpointDataNV( vkGetInstanceProcAddr( instance, "vkGetQueueCheckpointDataNV" ) );
      vkGetQueueCheckpointData2NV = PFN_vkGetQueueCheckpointData2NV( vkGetInstanceProcAddr( instance, "vkGetQueueCheckpointData2NV" ) );

      //=== VK_KHR_timeline_semaphore ===
      vkGetSemaphoreCounterValueKHR = PFN_vkGetSemaphoreCounterValueKHR( vkGetInstanceProcAddr( instance, "vkGetSemaphoreCounterValueKHR" ) );
      if ( !vkGetSemaphoreCounterValue )
        vkGetSemaphoreCounterValue = vkGetSemaphoreCounterValueKHR;
      vkWaitSemaphoresKHR = PFN_vkWaitSemaphoresKHR( vkGetInstanceProcAddr( instance, "vkWaitSemaphoresKHR" ) );
      if ( !vkWaitSemaphores )
        vkWaitSemaphores = vkWaitSemaphoresKHR;
      vkSignalSemaphoreKHR = PFN_vkSignalSemaphoreKHR( vkGetInstanceProcAddr( instance, "vkSignalSemaphoreKHR" ) );
      if ( !vkSignalSemaphore )
        vkSignalSemaphore = vkSignalSemaphoreKHR;

      //=== VK_EXT_present_timing ===
      vkSetSwapchainPresentTimingQueueSizeEXT =
        PFN_vkSetSwapchainPresentTimingQueueSizeEXT( vkGetInstanceProcAddr( instance, "vkSetSwapchainPresentTimingQueueSizeEXT" ) );
      vkGetSwapchainTimingPropertiesEXT = PFN_vkGetSwapchainTimingPropertiesEXT( vkGetInstanceProcAddr( instance, "vkGetSwapchainTimingPropertiesEXT" ) );
      vkGetSwapchainTimeDomainPropertiesEXT =
        PFN_vkGetSwapchainTimeDomainPropertiesEXT( vkGetInstanceProcAddr( instance, "vkGetSwapchainTimeDomainPropertiesEXT" ) );
      vkGetPastPresentationTimingEXT = PFN_vkGetPastPresentationTimingEXT( vkGetInstanceProcAddr( instance, "vkGetPastPresentationTimingEXT" ) );

      //=== VK_INTEL_performance_query ===
      vkInitializePerformanceApiINTEL   = PFN_vkInitializePerformanceApiINTEL( vkGetInstanceProcAddr( instance, "vkInitializePerformanceApiINTEL" ) );
      vkUninitializePerformanceApiINTEL = PFN_vkUninitializePerformanceApiINTEL( vkGetInstanceProcAddr( instance, "vkUninitializePerformanceApiINTEL" ) );
      vkCmdSetPerformanceMarkerINTEL    = PFN_vkCmdSetPerformanceMarkerINTEL( vkGetInstanceProcAddr( instance, "vkCmdSetPerformanceMarkerINTEL" ) );
      vkCmdSetPerformanceStreamMarkerINTEL =
        PFN_vkCmdSetPerformanceStreamMarkerINTEL( vkGetInstanceProcAddr( instance, "vkCmdSetPerformanceStreamMarkerINTEL" ) );
      vkCmdSetPerformanceOverrideINTEL = PFN_vkCmdSetPerformanceOverrideINTEL( vkGetInstanceProcAddr( instance, "vkCmdSetPerformanceOverrideINTEL" ) );
      vkAcquirePerformanceConfigurationINTEL =
        PFN_vkAcquirePerformanceConfigurationINTEL( vkGetInstanceProcAddr( instance, "vkAcquirePerformanceConfigurationINTEL" ) );
      vkReleasePerformanceConfigurationINTEL =
        PFN_vkReleasePerformanceConfigurationINTEL( vkGetInstanceProcAddr( instance, "vkReleasePerformanceConfigurationINTEL" ) );
      vkQueueSetPerformanceConfigurationINTEL =
        PFN_vkQueueSetPerformanceConfigurationINTEL( vkGetInstanceProcAddr( instance, "vkQueueSetPerformanceConfigurationINTEL" ) );
      vkGetPerformanceParameterINTEL = PFN_vkGetPerformanceParameterINTEL( vkGetInstanceProcAddr( instance, "vkGetPerformanceParameterINTEL" ) );

      //=== VK_AMD_display_native_hdr ===
      vkSetLocalDimmingAMD = PFN_vkSetLocalDimmingAMD( vkGetInstanceProcAddr( instance, "vkSetLocalDimmingAMD" ) );

#if defined( VK_USE_PLATFORM_FUCHSIA )
      //=== VK_FUCHSIA_imagepipe_surface ===
      vkCreateImagePipeSurfaceFUCHSIA = PFN_vkCreateImagePipeSurfaceFUCHSIA( vkGetInstanceProcAddr( instance, "vkCreateImagePipeSurfaceFUCHSIA" ) );
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#if defined( VK_USE_PLATFORM_METAL_EXT )
      //=== VK_EXT_metal_surface ===
      vkCreateMetalSurfaceEXT = PFN_vkCreateMetalSurfaceEXT( vkGetInstanceProcAddr( instance, "vkCreateMetalSurfaceEXT" ) );
#endif /*VK_USE_PLATFORM_METAL_EXT*/

      //=== VK_KHR_fragment_shading_rate ===
      vkGetPhysicalDeviceFragmentShadingRatesKHR =
        PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceFragmentShadingRatesKHR" ) );
      vkCmdSetFragmentShadingRateKHR = PFN_vkCmdSetFragmentShadingRateKHR( vkGetInstanceProcAddr( instance, "vkCmdSetFragmentShadingRateKHR" ) );

      //=== VK_KHR_dynamic_rendering_local_read ===
      vkCmdSetRenderingAttachmentLocationsKHR =
        PFN_vkCmdSetRenderingAttachmentLocationsKHR( vkGetInstanceProcAddr( instance, "vkCmdSetRenderingAttachmentLocationsKHR" ) );
      if ( !vkCmdSetRenderingAttachmentLocations )
        vkCmdSetRenderingAttachmentLocations = vkCmdSetRenderingAttachmentLocationsKHR;
      vkCmdSetRenderingInputAttachmentIndicesKHR =
        PFN_vkCmdSetRenderingInputAttachmentIndicesKHR( vkGetInstanceProcAddr( instance, "vkCmdSetRenderingInputAttachmentIndicesKHR" ) );
      if ( !vkCmdSetRenderingInputAttachmentIndices )
        vkCmdSetRenderingInputAttachmentIndices = vkCmdSetRenderingInputAttachmentIndicesKHR;

      //=== VK_EXT_buffer_device_address ===
      vkGetBufferDeviceAddressEXT = PFN_vkGetBufferDeviceAddressEXT( vkGetInstanceProcAddr( instance, "vkGetBufferDeviceAddressEXT" ) );
      if ( !vkGetBufferDeviceAddress )
        vkGetBufferDeviceAddress = vkGetBufferDeviceAddressEXT;

      //=== VK_EXT_tooling_info ===
      vkGetPhysicalDeviceToolPropertiesEXT =
        PFN_vkGetPhysicalDeviceToolPropertiesEXT( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceToolPropertiesEXT" ) );
      if ( !vkGetPhysicalDeviceToolProperties )
        vkGetPhysicalDeviceToolProperties = vkGetPhysicalDeviceToolPropertiesEXT;

      //=== VK_KHR_present_wait ===
      vkWaitForPresentKHR = PFN_vkWaitForPresentKHR( vkGetInstanceProcAddr( instance, "vkWaitForPresentKHR" ) );

      //=== VK_NV_cooperative_matrix ===
      vkGetPhysicalDeviceCooperativeMatrixPropertiesNV =
        PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV" ) );

      //=== VK_NV_coverage_reduction_mode ===
      vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV = PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(
        vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV" ) );

#if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_EXT_full_screen_exclusive ===
      vkGetPhysicalDeviceSurfacePresentModes2EXT =
        PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceSurfacePresentModes2EXT" ) );
      vkAcquireFullScreenExclusiveModeEXT = PFN_vkAcquireFullScreenExclusiveModeEXT( vkGetInstanceProcAddr( instance, "vkAcquireFullScreenExclusiveModeEXT" ) );
      vkReleaseFullScreenExclusiveModeEXT = PFN_vkReleaseFullScreenExclusiveModeEXT( vkGetInstanceProcAddr( instance, "vkReleaseFullScreenExclusiveModeEXT" ) );
      vkGetDeviceGroupSurfacePresentModes2EXT =
        PFN_vkGetDeviceGroupSurfacePresentModes2EXT( vkGetInstanceProcAddr( instance, "vkGetDeviceGroupSurfacePresentModes2EXT" ) );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_EXT_headless_surface ===
      vkCreateHeadlessSurfaceEXT = PFN_vkCreateHeadlessSurfaceEXT( vkGetInstanceProcAddr( instance, "vkCreateHeadlessSurfaceEXT" ) );

      //=== VK_KHR_buffer_device_address ===
      vkGetBufferDeviceAddressKHR = PFN_vkGetBufferDeviceAddressKHR( vkGetInstanceProcAddr( instance, "vkGetBufferDeviceAddressKHR" ) );
      if ( !vkGetBufferDeviceAddress )
        vkGetBufferDeviceAddress = vkGetBufferDeviceAddressKHR;
      vkGetBufferOpaqueCaptureAddressKHR = PFN_vkGetBufferOpaqueCaptureAddressKHR( vkGetInstanceProcAddr( instance, "vkGetBufferOpaqueCaptureAddressKHR" ) );
      if ( !vkGetBufferOpaqueCaptureAddress )
        vkGetBufferOpaqueCaptureAddress = vkGetBufferOpaqueCaptureAddressKHR;
      vkGetDeviceMemoryOpaqueCaptureAddressKHR =
        PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR( vkGetInstanceProcAddr( instance, "vkGetDeviceMemoryOpaqueCaptureAddressKHR" ) );
      if ( !vkGetDeviceMemoryOpaqueCaptureAddress )
        vkGetDeviceMemoryOpaqueCaptureAddress = vkGetDeviceMemoryOpaqueCaptureAddressKHR;

      //=== VK_EXT_line_rasterization ===
      vkCmdSetLineStippleEXT = PFN_vkCmdSetLineStippleEXT( vkGetInstanceProcAddr( instance, "vkCmdSetLineStippleEXT" ) );
      if ( !vkCmdSetLineStipple )
        vkCmdSetLineStipple = vkCmdSetLineStippleEXT;

      //=== VK_EXT_host_query_reset ===
      vkResetQueryPoolEXT = PFN_vkResetQueryPoolEXT( vkGetInstanceProcAddr( instance, "vkResetQueryPoolEXT" ) );
      if ( !vkResetQueryPool )
        vkResetQueryPool = vkResetQueryPoolEXT;

      //=== VK_EXT_extended_dynamic_state ===
      vkCmdSetCullModeEXT = PFN_vkCmdSetCullModeEXT( vkGetInstanceProcAddr( instance, "vkCmdSetCullModeEXT" ) );
      if ( !vkCmdSetCullMode )
        vkCmdSetCullMode = vkCmdSetCullModeEXT;
      vkCmdSetFrontFaceEXT = PFN_vkCmdSetFrontFaceEXT( vkGetInstanceProcAddr( instance, "vkCmdSetFrontFaceEXT" ) );
      if ( !vkCmdSetFrontFace )
        vkCmdSetFrontFace = vkCmdSetFrontFaceEXT;
      vkCmdSetPrimitiveTopologyEXT = PFN_vkCmdSetPrimitiveTopologyEXT( vkGetInstanceProcAddr( instance, "vkCmdSetPrimitiveTopologyEXT" ) );
      if ( !vkCmdSetPrimitiveTopology )
        vkCmdSetPrimitiveTopology = vkCmdSetPrimitiveTopologyEXT;
      vkCmdSetViewportWithCountEXT = PFN_vkCmdSetViewportWithCountEXT( vkGetInstanceProcAddr( instance, "vkCmdSetViewportWithCountEXT" ) );
      if ( !vkCmdSetViewportWithCount )
        vkCmdSetViewportWithCount = vkCmdSetViewportWithCountEXT;
      vkCmdSetScissorWithCountEXT = PFN_vkCmdSetScissorWithCountEXT( vkGetInstanceProcAddr( instance, "vkCmdSetScissorWithCountEXT" ) );
      if ( !vkCmdSetScissorWithCount )
        vkCmdSetScissorWithCount = vkCmdSetScissorWithCountEXT;
      vkCmdBindVertexBuffers2EXT = PFN_vkCmdBindVertexBuffers2EXT( vkGetInstanceProcAddr( instance, "vkCmdBindVertexBuffers2EXT" ) );
      if ( !vkCmdBindVertexBuffers2 )
        vkCmdBindVertexBuffers2 = vkCmdBindVertexBuffers2EXT;
      vkCmdSetDepthTestEnableEXT = PFN_vkCmdSetDepthTestEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthTestEnableEXT" ) );
      if ( !vkCmdSetDepthTestEnable )
        vkCmdSetDepthTestEnable = vkCmdSetDepthTestEnableEXT;
      vkCmdSetDepthWriteEnableEXT = PFN_vkCmdSetDepthWriteEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthWriteEnableEXT" ) );
      if ( !vkCmdSetDepthWriteEnable )
        vkCmdSetDepthWriteEnable = vkCmdSetDepthWriteEnableEXT;
      vkCmdSetDepthCompareOpEXT = PFN_vkCmdSetDepthCompareOpEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthCompareOpEXT" ) );
      if ( !vkCmdSetDepthCompareOp )
        vkCmdSetDepthCompareOp = vkCmdSetDepthCompareOpEXT;
      vkCmdSetDepthBoundsTestEnableEXT = PFN_vkCmdSetDepthBoundsTestEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthBoundsTestEnableEXT" ) );
      if ( !vkCmdSetDepthBoundsTestEnable )
        vkCmdSetDepthBoundsTestEnable = vkCmdSetDepthBoundsTestEnableEXT;
      vkCmdSetStencilTestEnableEXT = PFN_vkCmdSetStencilTestEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetStencilTestEnableEXT" ) );
      if ( !vkCmdSetStencilTestEnable )
        vkCmdSetStencilTestEnable = vkCmdSetStencilTestEnableEXT;
      vkCmdSetStencilOpEXT = PFN_vkCmdSetStencilOpEXT( vkGetInstanceProcAddr( instance, "vkCmdSetStencilOpEXT" ) );
      if ( !vkCmdSetStencilOp )
        vkCmdSetStencilOp = vkCmdSetStencilOpEXT;

      //=== VK_KHR_deferred_host_operations ===
      vkCreateDeferredOperationKHR  = PFN_vkCreateDeferredOperationKHR( vkGetInstanceProcAddr( instance, "vkCreateDeferredOperationKHR" ) );
      vkDestroyDeferredOperationKHR = PFN_vkDestroyDeferredOperationKHR( vkGetInstanceProcAddr( instance, "vkDestroyDeferredOperationKHR" ) );
      vkGetDeferredOperationMaxConcurrencyKHR =
        PFN_vkGetDeferredOperationMaxConcurrencyKHR( vkGetInstanceProcAddr( instance, "vkGetDeferredOperationMaxConcurrencyKHR" ) );
      vkGetDeferredOperationResultKHR = PFN_vkGetDeferredOperationResultKHR( vkGetInstanceProcAddr( instance, "vkGetDeferredOperationResultKHR" ) );
      vkDeferredOperationJoinKHR      = PFN_vkDeferredOperationJoinKHR( vkGetInstanceProcAddr( instance, "vkDeferredOperationJoinKHR" ) );

      //=== VK_KHR_pipeline_executable_properties ===
      vkGetPipelineExecutablePropertiesKHR =
        PFN_vkGetPipelineExecutablePropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetPipelineExecutablePropertiesKHR" ) );
      vkGetPipelineExecutableStatisticsKHR =
        PFN_vkGetPipelineExecutableStatisticsKHR( vkGetInstanceProcAddr( instance, "vkGetPipelineExecutableStatisticsKHR" ) );
      vkGetPipelineExecutableInternalRepresentationsKHR =
        PFN_vkGetPipelineExecutableInternalRepresentationsKHR( vkGetInstanceProcAddr( instance, "vkGetPipelineExecutableInternalRepresentationsKHR" ) );

      //=== VK_EXT_host_image_copy ===
      vkCopyMemoryToImageEXT = PFN_vkCopyMemoryToImageEXT( vkGetInstanceProcAddr( instance, "vkCopyMemoryToImageEXT" ) );
      if ( !vkCopyMemoryToImage )
        vkCopyMemoryToImage = vkCopyMemoryToImageEXT;
      vkCopyImageToMemoryEXT = PFN_vkCopyImageToMemoryEXT( vkGetInstanceProcAddr( instance, "vkCopyImageToMemoryEXT" ) );
      if ( !vkCopyImageToMemory )
        vkCopyImageToMemory = vkCopyImageToMemoryEXT;
      vkCopyImageToImageEXT = PFN_vkCopyImageToImageEXT( vkGetInstanceProcAddr( instance, "vkCopyImageToImageEXT" ) );
      if ( !vkCopyImageToImage )
        vkCopyImageToImage = vkCopyImageToImageEXT;
      vkTransitionImageLayoutEXT = PFN_vkTransitionImageLayoutEXT( vkGetInstanceProcAddr( instance, "vkTransitionImageLayoutEXT" ) );
      if ( !vkTransitionImageLayout )
        vkTransitionImageLayout = vkTransitionImageLayoutEXT;
      vkGetImageSubresourceLayout2EXT = PFN_vkGetImageSubresourceLayout2EXT( vkGetInstanceProcAddr( instance, "vkGetImageSubresourceLayout2EXT" ) );
      if ( !vkGetImageSubresourceLayout2 )
        vkGetImageSubresourceLayout2 = vkGetImageSubresourceLayout2EXT;

      //=== VK_KHR_map_memory2 ===
      vkMapMemory2KHR = PFN_vkMapMemory2KHR( vkGetInstanceProcAddr( instance, "vkMapMemory2KHR" ) );
      if ( !vkMapMemory2 )
        vkMapMemory2 = vkMapMemory2KHR;
      vkUnmapMemory2KHR = PFN_vkUnmapMemory2KHR( vkGetInstanceProcAddr( instance, "vkUnmapMemory2KHR" ) );
      if ( !vkUnmapMemory2 )
        vkUnmapMemory2 = vkUnmapMemory2KHR;

      //=== VK_EXT_swapchain_maintenance1 ===
      vkReleaseSwapchainImagesEXT = PFN_vkReleaseSwapchainImagesEXT( vkGetInstanceProcAddr( instance, "vkReleaseSwapchainImagesEXT" ) );
      if ( !vkReleaseSwapchainImagesKHR )
        vkReleaseSwapchainImagesKHR = vkReleaseSwapchainImagesEXT;

      //=== VK_NV_device_generated_commands ===
      vkGetGeneratedCommandsMemoryRequirementsNV =
        PFN_vkGetGeneratedCommandsMemoryRequirementsNV( vkGetInstanceProcAddr( instance, "vkGetGeneratedCommandsMemoryRequirementsNV" ) );
      vkCmdPreprocessGeneratedCommandsNV = PFN_vkCmdPreprocessGeneratedCommandsNV( vkGetInstanceProcAddr( instance, "vkCmdPreprocessGeneratedCommandsNV" ) );
      vkCmdExecuteGeneratedCommandsNV    = PFN_vkCmdExecuteGeneratedCommandsNV( vkGetInstanceProcAddr( instance, "vkCmdExecuteGeneratedCommandsNV" ) );
      vkCmdBindPipelineShaderGroupNV     = PFN_vkCmdBindPipelineShaderGroupNV( vkGetInstanceProcAddr( instance, "vkCmdBindPipelineShaderGroupNV" ) );
      vkCreateIndirectCommandsLayoutNV   = PFN_vkCreateIndirectCommandsLayoutNV( vkGetInstanceProcAddr( instance, "vkCreateIndirectCommandsLayoutNV" ) );
      vkDestroyIndirectCommandsLayoutNV  = PFN_vkDestroyIndirectCommandsLayoutNV( vkGetInstanceProcAddr( instance, "vkDestroyIndirectCommandsLayoutNV" ) );

      //=== VK_EXT_depth_bias_control ===
      vkCmdSetDepthBias2EXT = PFN_vkCmdSetDepthBias2EXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthBias2EXT" ) );

      //=== VK_EXT_acquire_drm_display ===
      vkAcquireDrmDisplayEXT = PFN_vkAcquireDrmDisplayEXT( vkGetInstanceProcAddr( instance, "vkAcquireDrmDisplayEXT" ) );
      vkGetDrmDisplayEXT     = PFN_vkGetDrmDisplayEXT( vkGetInstanceProcAddr( instance, "vkGetDrmDisplayEXT" ) );

      //=== VK_EXT_private_data ===
      vkCreatePrivateDataSlotEXT = PFN_vkCreatePrivateDataSlotEXT( vkGetInstanceProcAddr( instance, "vkCreatePrivateDataSlotEXT" ) );
      if ( !vkCreatePrivateDataSlot )
        vkCreatePrivateDataSlot = vkCreatePrivateDataSlotEXT;
      vkDestroyPrivateDataSlotEXT = PFN_vkDestroyPrivateDataSlotEXT( vkGetInstanceProcAddr( instance, "vkDestroyPrivateDataSlotEXT" ) );
      if ( !vkDestroyPrivateDataSlot )
        vkDestroyPrivateDataSlot = vkDestroyPrivateDataSlotEXT;
      vkSetPrivateDataEXT = PFN_vkSetPrivateDataEXT( vkGetInstanceProcAddr( instance, "vkSetPrivateDataEXT" ) );
      if ( !vkSetPrivateData )
        vkSetPrivateData = vkSetPrivateDataEXT;
      vkGetPrivateDataEXT = PFN_vkGetPrivateDataEXT( vkGetInstanceProcAddr( instance, "vkGetPrivateDataEXT" ) );
      if ( !vkGetPrivateData )
        vkGetPrivateData = vkGetPrivateDataEXT;

      //=== VK_KHR_video_encode_queue ===
      vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR = PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(
        vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR" ) );
      vkGetEncodedVideoSessionParametersKHR =
        PFN_vkGetEncodedVideoSessionParametersKHR( vkGetInstanceProcAddr( instance, "vkGetEncodedVideoSessionParametersKHR" ) );
      vkCmdEncodeVideoKHR = PFN_vkCmdEncodeVideoKHR( vkGetInstanceProcAddr( instance, "vkCmdEncodeVideoKHR" ) );

#if defined( VK_ENABLE_BETA_EXTENSIONS )
      //=== VK_NV_cuda_kernel_launch ===
      vkCreateCudaModuleNV    = PFN_vkCreateCudaModuleNV( vkGetInstanceProcAddr( instance, "vkCreateCudaModuleNV" ) );
      vkGetCudaModuleCacheNV  = PFN_vkGetCudaModuleCacheNV( vkGetInstanceProcAddr( instance, "vkGetCudaModuleCacheNV" ) );
      vkCreateCudaFunctionNV  = PFN_vkCreateCudaFunctionNV( vkGetInstanceProcAddr( instance, "vkCreateCudaFunctionNV" ) );
      vkDestroyCudaModuleNV   = PFN_vkDestroyCudaModuleNV( vkGetInstanceProcAddr( instance, "vkDestroyCudaModuleNV" ) );
      vkDestroyCudaFunctionNV = PFN_vkDestroyCudaFunctionNV( vkGetInstanceProcAddr( instance, "vkDestroyCudaFunctionNV" ) );
      vkCmdCudaLaunchKernelNV = PFN_vkCmdCudaLaunchKernelNV( vkGetInstanceProcAddr( instance, "vkCmdCudaLaunchKernelNV" ) );
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

      //=== VK_QCOM_tile_shading ===
      vkCmdDispatchTileQCOM          = PFN_vkCmdDispatchTileQCOM( vkGetInstanceProcAddr( instance, "vkCmdDispatchTileQCOM" ) );
      vkCmdBeginPerTileExecutionQCOM = PFN_vkCmdBeginPerTileExecutionQCOM( vkGetInstanceProcAddr( instance, "vkCmdBeginPerTileExecutionQCOM" ) );
      vkCmdEndPerTileExecutionQCOM   = PFN_vkCmdEndPerTileExecutionQCOM( vkGetInstanceProcAddr( instance, "vkCmdEndPerTileExecutionQCOM" ) );

#if defined( VK_USE_PLATFORM_METAL_EXT )
      //=== VK_EXT_metal_objects ===
      vkExportMetalObjectsEXT = PFN_vkExportMetalObjectsEXT( vkGetInstanceProcAddr( instance, "vkExportMetalObjectsEXT" ) );
#endif /*VK_USE_PLATFORM_METAL_EXT*/

      //=== VK_KHR_synchronization2 ===
      vkCmdSetEvent2KHR = PFN_vkCmdSetEvent2KHR( vkGetInstanceProcAddr( instance, "vkCmdSetEvent2KHR" ) );
      if ( !vkCmdSetEvent2 )
        vkCmdSetEvent2 = vkCmdSetEvent2KHR;
      vkCmdResetEvent2KHR = PFN_vkCmdResetEvent2KHR( vkGetInstanceProcAddr( instance, "vkCmdResetEvent2KHR" ) );
      if ( !vkCmdResetEvent2 )
        vkCmdResetEvent2 = vkCmdResetEvent2KHR;
      vkCmdWaitEvents2KHR = PFN_vkCmdWaitEvents2KHR( vkGetInstanceProcAddr( instance, "vkCmdWaitEvents2KHR" ) );
      if ( !vkCmdWaitEvents2 )
        vkCmdWaitEvents2 = vkCmdWaitEvents2KHR;
      vkCmdPipelineBarrier2KHR = PFN_vkCmdPipelineBarrier2KHR( vkGetInstanceProcAddr( instance, "vkCmdPipelineBarrier2KHR" ) );
      if ( !vkCmdPipelineBarrier2 )
        vkCmdPipelineBarrier2 = vkCmdPipelineBarrier2KHR;
      vkCmdWriteTimestamp2KHR = PFN_vkCmdWriteTimestamp2KHR( vkGetInstanceProcAddr( instance, "vkCmdWriteTimestamp2KHR" ) );
      if ( !vkCmdWriteTimestamp2 )
        vkCmdWriteTimestamp2 = vkCmdWriteTimestamp2KHR;
      vkQueueSubmit2KHR = PFN_vkQueueSubmit2KHR( vkGetInstanceProcAddr( instance, "vkQueueSubmit2KHR" ) );
      if ( !vkQueueSubmit2 )
        vkQueueSubmit2 = vkQueueSubmit2KHR;

      //=== VK_EXT_descriptor_buffer ===
      vkGetDescriptorSetLayoutSizeEXT = PFN_vkGetDescriptorSetLayoutSizeEXT( vkGetInstanceProcAddr( instance, "vkGetDescriptorSetLayoutSizeEXT" ) );
      vkGetDescriptorSetLayoutBindingOffsetEXT =
        PFN_vkGetDescriptorSetLayoutBindingOffsetEXT( vkGetInstanceProcAddr( instance, "vkGetDescriptorSetLayoutBindingOffsetEXT" ) );
      vkGetDescriptorEXT                 = PFN_vkGetDescriptorEXT( vkGetInstanceProcAddr( instance, "vkGetDescriptorEXT" ) );
      vkCmdBindDescriptorBuffersEXT      = PFN_vkCmdBindDescriptorBuffersEXT( vkGetInstanceProcAddr( instance, "vkCmdBindDescriptorBuffersEXT" ) );
      vkCmdSetDescriptorBufferOffsetsEXT = PFN_vkCmdSetDescriptorBufferOffsetsEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDescriptorBufferOffsetsEXT" ) );
      vkCmdBindDescriptorBufferEmbeddedSamplersEXT =
        PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT( vkGetInstanceProcAddr( instance, "vkCmdBindDescriptorBufferEmbeddedSamplersEXT" ) );
      vkGetBufferOpaqueCaptureDescriptorDataEXT =
        PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT( vkGetInstanceProcAddr( instance, "vkGetBufferOpaqueCaptureDescriptorDataEXT" ) );
      vkGetImageOpaqueCaptureDescriptorDataEXT =
        PFN_vkGetImageOpaqueCaptureDescriptorDataEXT( vkGetInstanceProcAddr( instance, "vkGetImageOpaqueCaptureDescriptorDataEXT" ) );
      vkGetImageViewOpaqueCaptureDescriptorDataEXT =
        PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT( vkGetInstanceProcAddr( instance, "vkGetImageViewOpaqueCaptureDescriptorDataEXT" ) );
      vkGetSamplerOpaqueCaptureDescriptorDataEXT =
        PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT( vkGetInstanceProcAddr( instance, "vkGetSamplerOpaqueCaptureDescriptorDataEXT" ) );
      vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT = PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT(
        vkGetInstanceProcAddr( instance, "vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT" ) );

      //=== VK_NV_fragment_shading_rate_enums ===
      vkCmdSetFragmentShadingRateEnumNV = PFN_vkCmdSetFragmentShadingRateEnumNV( vkGetInstanceProcAddr( instance, "vkCmdSetFragmentShadingRateEnumNV" ) );

      //=== VK_EXT_mesh_shader ===
      vkCmdDrawMeshTasksEXT              = PFN_vkCmdDrawMeshTasksEXT( vkGetInstanceProcAddr( instance, "vkCmdDrawMeshTasksEXT" ) );
      vkCmdDrawMeshTasksIndirectEXT      = PFN_vkCmdDrawMeshTasksIndirectEXT( vkGetInstanceProcAddr( instance, "vkCmdDrawMeshTasksIndirectEXT" ) );
      vkCmdDrawMeshTasksIndirectCountEXT = PFN_vkCmdDrawMeshTasksIndirectCountEXT( vkGetInstanceProcAddr( instance, "vkCmdDrawMeshTasksIndirectCountEXT" ) );

      //=== VK_KHR_copy_commands2 ===
      vkCmdCopyBuffer2KHR = PFN_vkCmdCopyBuffer2KHR( vkGetInstanceProcAddr( instance, "vkCmdCopyBuffer2KHR" ) );
      if ( !vkCmdCopyBuffer2 )
        vkCmdCopyBuffer2 = vkCmdCopyBuffer2KHR;
      vkCmdCopyImage2KHR = PFN_vkCmdCopyImage2KHR( vkGetInstanceProcAddr( instance, "vkCmdCopyImage2KHR" ) );
      if ( !vkCmdCopyImage2 )
        vkCmdCopyImage2 = vkCmdCopyImage2KHR;
      vkCmdCopyBufferToImage2KHR = PFN_vkCmdCopyBufferToImage2KHR( vkGetInstanceProcAddr( instance, "vkCmdCopyBufferToImage2KHR" ) );
      if ( !vkCmdCopyBufferToImage2 )
        vkCmdCopyBufferToImage2 = vkCmdCopyBufferToImage2KHR;
      vkCmdCopyImageToBuffer2KHR = PFN_vkCmdCopyImageToBuffer2KHR( vkGetInstanceProcAddr( instance, "vkCmdCopyImageToBuffer2KHR" ) );
      if ( !vkCmdCopyImageToBuffer2 )
        vkCmdCopyImageToBuffer2 = vkCmdCopyImageToBuffer2KHR;
      vkCmdBlitImage2KHR = PFN_vkCmdBlitImage2KHR( vkGetInstanceProcAddr( instance, "vkCmdBlitImage2KHR" ) );
      if ( !vkCmdBlitImage2 )
        vkCmdBlitImage2 = vkCmdBlitImage2KHR;
      vkCmdResolveImage2KHR = PFN_vkCmdResolveImage2KHR( vkGetInstanceProcAddr( instance, "vkCmdResolveImage2KHR" ) );
      if ( !vkCmdResolveImage2 )
        vkCmdResolveImage2 = vkCmdResolveImage2KHR;

      //=== VK_EXT_device_fault ===
      vkGetDeviceFaultInfoEXT = PFN_vkGetDeviceFaultInfoEXT( vkGetInstanceProcAddr( instance, "vkGetDeviceFaultInfoEXT" ) );

#if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_NV_acquire_winrt_display ===
      vkAcquireWinrtDisplayNV = PFN_vkAcquireWinrtDisplayNV( vkGetInstanceProcAddr( instance, "vkAcquireWinrtDisplayNV" ) );
      vkGetWinrtDisplayNV     = PFN_vkGetWinrtDisplayNV( vkGetInstanceProcAddr( instance, "vkGetWinrtDisplayNV" ) );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#if defined( VK_USE_PLATFORM_DIRECTFB_EXT )
      //=== VK_EXT_directfb_surface ===
      vkCreateDirectFBSurfaceEXT = PFN_vkCreateDirectFBSurfaceEXT( vkGetInstanceProcAddr( instance, "vkCreateDirectFBSurfaceEXT" ) );
      vkGetPhysicalDeviceDirectFBPresentationSupportEXT =
        PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceDirectFBPresentationSupportEXT" ) );
#endif /*VK_USE_PLATFORM_DIRECTFB_EXT*/

      //=== VK_EXT_vertex_input_dynamic_state ===
      vkCmdSetVertexInputEXT = PFN_vkCmdSetVertexInputEXT( vkGetInstanceProcAddr( instance, "vkCmdSetVertexInputEXT" ) );

#if defined( VK_USE_PLATFORM_FUCHSIA )
      //=== VK_FUCHSIA_external_memory ===
      vkGetMemoryZirconHandleFUCHSIA = PFN_vkGetMemoryZirconHandleFUCHSIA( vkGetInstanceProcAddr( instance, "vkGetMemoryZirconHandleFUCHSIA" ) );
      vkGetMemoryZirconHandlePropertiesFUCHSIA =
        PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA( vkGetInstanceProcAddr( instance, "vkGetMemoryZirconHandlePropertiesFUCHSIA" ) );
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#if defined( VK_USE_PLATFORM_FUCHSIA )
      //=== VK_FUCHSIA_external_semaphore ===
      vkImportSemaphoreZirconHandleFUCHSIA =
        PFN_vkImportSemaphoreZirconHandleFUCHSIA( vkGetInstanceProcAddr( instance, "vkImportSemaphoreZirconHandleFUCHSIA" ) );
      vkGetSemaphoreZirconHandleFUCHSIA = PFN_vkGetSemaphoreZirconHandleFUCHSIA( vkGetInstanceProcAddr( instance, "vkGetSemaphoreZirconHandleFUCHSIA" ) );
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#if defined( VK_USE_PLATFORM_FUCHSIA )
      //=== VK_FUCHSIA_buffer_collection ===
      vkCreateBufferCollectionFUCHSIA = PFN_vkCreateBufferCollectionFUCHSIA( vkGetInstanceProcAddr( instance, "vkCreateBufferCollectionFUCHSIA" ) );
      vkSetBufferCollectionImageConstraintsFUCHSIA =
        PFN_vkSetBufferCollectionImageConstraintsFUCHSIA( vkGetInstanceProcAddr( instance, "vkSetBufferCollectionImageConstraintsFUCHSIA" ) );
      vkSetBufferCollectionBufferConstraintsFUCHSIA =
        PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA( vkGetInstanceProcAddr( instance, "vkSetBufferCollectionBufferConstraintsFUCHSIA" ) );
      vkDestroyBufferCollectionFUCHSIA = PFN_vkDestroyBufferCollectionFUCHSIA( vkGetInstanceProcAddr( instance, "vkDestroyBufferCollectionFUCHSIA" ) );
      vkGetBufferCollectionPropertiesFUCHSIA =
        PFN_vkGetBufferCollectionPropertiesFUCHSIA( vkGetInstanceProcAddr( instance, "vkGetBufferCollectionPropertiesFUCHSIA" ) );
#endif /*VK_USE_PLATFORM_FUCHSIA*/

      //=== VK_HUAWEI_subpass_shading ===
      vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI =
        PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI( vkGetInstanceProcAddr( instance, "vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI" ) );
      vkCmdSubpassShadingHUAWEI = PFN_vkCmdSubpassShadingHUAWEI( vkGetInstanceProcAddr( instance, "vkCmdSubpassShadingHUAWEI" ) );

      //=== VK_HUAWEI_invocation_mask ===
      vkCmdBindInvocationMaskHUAWEI = PFN_vkCmdBindInvocationMaskHUAWEI( vkGetInstanceProcAddr( instance, "vkCmdBindInvocationMaskHUAWEI" ) );

      //=== VK_NV_external_memory_rdma ===
      vkGetMemoryRemoteAddressNV = PFN_vkGetMemoryRemoteAddressNV( vkGetInstanceProcAddr( instance, "vkGetMemoryRemoteAddressNV" ) );

      //=== VK_EXT_pipeline_properties ===
      vkGetPipelinePropertiesEXT = PFN_vkGetPipelinePropertiesEXT( vkGetInstanceProcAddr( instance, "vkGetPipelinePropertiesEXT" ) );

      //=== VK_EXT_extended_dynamic_state2 ===
      vkCmdSetPatchControlPointsEXT      = PFN_vkCmdSetPatchControlPointsEXT( vkGetInstanceProcAddr( instance, "vkCmdSetPatchControlPointsEXT" ) );
      vkCmdSetRasterizerDiscardEnableEXT = PFN_vkCmdSetRasterizerDiscardEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetRasterizerDiscardEnableEXT" ) );
      if ( !vkCmdSetRasterizerDiscardEnable )
        vkCmdSetRasterizerDiscardEnable = vkCmdSetRasterizerDiscardEnableEXT;
      vkCmdSetDepthBiasEnableEXT = PFN_vkCmdSetDepthBiasEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthBiasEnableEXT" ) );
      if ( !vkCmdSetDepthBiasEnable )
        vkCmdSetDepthBiasEnable = vkCmdSetDepthBiasEnableEXT;
      vkCmdSetLogicOpEXT                = PFN_vkCmdSetLogicOpEXT( vkGetInstanceProcAddr( instance, "vkCmdSetLogicOpEXT" ) );
      vkCmdSetPrimitiveRestartEnableEXT = PFN_vkCmdSetPrimitiveRestartEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetPrimitiveRestartEnableEXT" ) );
      if ( !vkCmdSetPrimitiveRestartEnable )
        vkCmdSetPrimitiveRestartEnable = vkCmdSetPrimitiveRestartEnableEXT;

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
      //=== VK_QNX_screen_surface ===
      vkCreateScreenSurfaceQNX = PFN_vkCreateScreenSurfaceQNX( vkGetInstanceProcAddr( instance, "vkCreateScreenSurfaceQNX" ) );
      vkGetPhysicalDeviceScreenPresentationSupportQNX =
        PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceScreenPresentationSupportQNX" ) );
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/

      //=== VK_EXT_color_write_enable ===
      vkCmdSetColorWriteEnableEXT = PFN_vkCmdSetColorWriteEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetColorWriteEnableEXT" ) );

      //=== VK_KHR_ray_tracing_maintenance1 ===
      vkCmdTraceRaysIndirect2KHR = PFN_vkCmdTraceRaysIndirect2KHR( vkGetInstanceProcAddr( instance, "vkCmdTraceRaysIndirect2KHR" ) );

      //=== VK_EXT_multi_draw ===
      vkCmdDrawMultiEXT        = PFN_vkCmdDrawMultiEXT( vkGetInstanceProcAddr( instance, "vkCmdDrawMultiEXT" ) );
      vkCmdDrawMultiIndexedEXT = PFN_vkCmdDrawMultiIndexedEXT( vkGetInstanceProcAddr( instance, "vkCmdDrawMultiIndexedEXT" ) );

      //=== VK_EXT_opacity_micromap ===
      vkCreateMicromapEXT                 = PFN_vkCreateMicromapEXT( vkGetInstanceProcAddr( instance, "vkCreateMicromapEXT" ) );
      vkDestroyMicromapEXT                = PFN_vkDestroyMicromapEXT( vkGetInstanceProcAddr( instance, "vkDestroyMicromapEXT" ) );
      vkCmdBuildMicromapsEXT              = PFN_vkCmdBuildMicromapsEXT( vkGetInstanceProcAddr( instance, "vkCmdBuildMicromapsEXT" ) );
      vkBuildMicromapsEXT                 = PFN_vkBuildMicromapsEXT( vkGetInstanceProcAddr( instance, "vkBuildMicromapsEXT" ) );
      vkCopyMicromapEXT                   = PFN_vkCopyMicromapEXT( vkGetInstanceProcAddr( instance, "vkCopyMicromapEXT" ) );
      vkCopyMicromapToMemoryEXT           = PFN_vkCopyMicromapToMemoryEXT( vkGetInstanceProcAddr( instance, "vkCopyMicromapToMemoryEXT" ) );
      vkCopyMemoryToMicromapEXT           = PFN_vkCopyMemoryToMicromapEXT( vkGetInstanceProcAddr( instance, "vkCopyMemoryToMicromapEXT" ) );
      vkWriteMicromapsPropertiesEXT       = PFN_vkWriteMicromapsPropertiesEXT( vkGetInstanceProcAddr( instance, "vkWriteMicromapsPropertiesEXT" ) );
      vkCmdCopyMicromapEXT                = PFN_vkCmdCopyMicromapEXT( vkGetInstanceProcAddr( instance, "vkCmdCopyMicromapEXT" ) );
      vkCmdCopyMicromapToMemoryEXT        = PFN_vkCmdCopyMicromapToMemoryEXT( vkGetInstanceProcAddr( instance, "vkCmdCopyMicromapToMemoryEXT" ) );
      vkCmdCopyMemoryToMicromapEXT        = PFN_vkCmdCopyMemoryToMicromapEXT( vkGetInstanceProcAddr( instance, "vkCmdCopyMemoryToMicromapEXT" ) );
      vkCmdWriteMicromapsPropertiesEXT    = PFN_vkCmdWriteMicromapsPropertiesEXT( vkGetInstanceProcAddr( instance, "vkCmdWriteMicromapsPropertiesEXT" ) );
      vkGetDeviceMicromapCompatibilityEXT = PFN_vkGetDeviceMicromapCompatibilityEXT( vkGetInstanceProcAddr( instance, "vkGetDeviceMicromapCompatibilityEXT" ) );
      vkGetMicromapBuildSizesEXT          = PFN_vkGetMicromapBuildSizesEXT( vkGetInstanceProcAddr( instance, "vkGetMicromapBuildSizesEXT" ) );

      //=== VK_HUAWEI_cluster_culling_shader ===
      vkCmdDrawClusterHUAWEI         = PFN_vkCmdDrawClusterHUAWEI( vkGetInstanceProcAddr( instance, "vkCmdDrawClusterHUAWEI" ) );
      vkCmdDrawClusterIndirectHUAWEI = PFN_vkCmdDrawClusterIndirectHUAWEI( vkGetInstanceProcAddr( instance, "vkCmdDrawClusterIndirectHUAWEI" ) );

      //=== VK_EXT_pageable_device_local_memory ===
      vkSetDeviceMemoryPriorityEXT = PFN_vkSetDeviceMemoryPriorityEXT( vkGetInstanceProcAddr( instance, "vkSetDeviceMemoryPriorityEXT" ) );

      //=== VK_KHR_maintenance4 ===
      vkGetDeviceBufferMemoryRequirementsKHR =
        PFN_vkGetDeviceBufferMemoryRequirementsKHR( vkGetInstanceProcAddr( instance, "vkGetDeviceBufferMemoryRequirementsKHR" ) );
      if ( !vkGetDeviceBufferMemoryRequirements )
        vkGetDeviceBufferMemoryRequirements = vkGetDeviceBufferMemoryRequirementsKHR;
      vkGetDeviceImageMemoryRequirementsKHR =
        PFN_vkGetDeviceImageMemoryRequirementsKHR( vkGetInstanceProcAddr( instance, "vkGetDeviceImageMemoryRequirementsKHR" ) );
      if ( !vkGetDeviceImageMemoryRequirements )
        vkGetDeviceImageMemoryRequirements = vkGetDeviceImageMemoryRequirementsKHR;
      vkGetDeviceImageSparseMemoryRequirementsKHR =
        PFN_vkGetDeviceImageSparseMemoryRequirementsKHR( vkGetInstanceProcAddr( instance, "vkGetDeviceImageSparseMemoryRequirementsKHR" ) );
      if ( !vkGetDeviceImageSparseMemoryRequirements )
        vkGetDeviceImageSparseMemoryRequirements = vkGetDeviceImageSparseMemoryRequirementsKHR;

      //=== VK_VALVE_descriptor_set_host_mapping ===
      vkGetDescriptorSetLayoutHostMappingInfoVALVE =
        PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE( vkGetInstanceProcAddr( instance, "vkGetDescriptorSetLayoutHostMappingInfoVALVE" ) );
      vkGetDescriptorSetHostMappingVALVE = PFN_vkGetDescriptorSetHostMappingVALVE( vkGetInstanceProcAddr( instance, "vkGetDescriptorSetHostMappingVALVE" ) );

      //=== VK_NV_copy_memory_indirect ===
      vkCmdCopyMemoryIndirectNV        = PFN_vkCmdCopyMemoryIndirectNV( vkGetInstanceProcAddr( instance, "vkCmdCopyMemoryIndirectNV" ) );
      vkCmdCopyMemoryToImageIndirectNV = PFN_vkCmdCopyMemoryToImageIndirectNV( vkGetInstanceProcAddr( instance, "vkCmdCopyMemoryToImageIndirectNV" ) );

      //=== VK_NV_memory_decompression ===
      vkCmdDecompressMemoryNV = PFN_vkCmdDecompressMemoryNV( vkGetInstanceProcAddr( instance, "vkCmdDecompressMemoryNV" ) );
      vkCmdDecompressMemoryIndirectCountNV =
        PFN_vkCmdDecompressMemoryIndirectCountNV( vkGetInstanceProcAddr( instance, "vkCmdDecompressMemoryIndirectCountNV" ) );

      //=== VK_NV_device_generated_commands_compute ===
      vkGetPipelineIndirectMemoryRequirementsNV =
        PFN_vkGetPipelineIndirectMemoryRequirementsNV( vkGetInstanceProcAddr( instance, "vkGetPipelineIndirectMemoryRequirementsNV" ) );
      vkCmdUpdatePipelineIndirectBufferNV = PFN_vkCmdUpdatePipelineIndirectBufferNV( vkGetInstanceProcAddr( instance, "vkCmdUpdatePipelineIndirectBufferNV" ) );
      vkGetPipelineIndirectDeviceAddressNV =
        PFN_vkGetPipelineIndirectDeviceAddressNV( vkGetInstanceProcAddr( instance, "vkGetPipelineIndirectDeviceAddressNV" ) );

#if defined( VK_USE_PLATFORM_OHOS )
      //=== VK_OHOS_external_memory ===
      vkGetNativeBufferPropertiesOHOS = PFN_vkGetNativeBufferPropertiesOHOS( vkGetInstanceProcAddr( instance, "vkGetNativeBufferPropertiesOHOS" ) );
      vkGetMemoryNativeBufferOHOS     = PFN_vkGetMemoryNativeBufferOHOS( vkGetInstanceProcAddr( instance, "vkGetMemoryNativeBufferOHOS" ) );
#endif /*VK_USE_PLATFORM_OHOS*/

      //=== VK_EXT_extended_dynamic_state3 ===
      vkCmdSetDepthClampEnableEXT         = PFN_vkCmdSetDepthClampEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthClampEnableEXT" ) );
      vkCmdSetPolygonModeEXT              = PFN_vkCmdSetPolygonModeEXT( vkGetInstanceProcAddr( instance, "vkCmdSetPolygonModeEXT" ) );
      vkCmdSetRasterizationSamplesEXT     = PFN_vkCmdSetRasterizationSamplesEXT( vkGetInstanceProcAddr( instance, "vkCmdSetRasterizationSamplesEXT" ) );
      vkCmdSetSampleMaskEXT               = PFN_vkCmdSetSampleMaskEXT( vkGetInstanceProcAddr( instance, "vkCmdSetSampleMaskEXT" ) );
      vkCmdSetAlphaToCoverageEnableEXT    = PFN_vkCmdSetAlphaToCoverageEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetAlphaToCoverageEnableEXT" ) );
      vkCmdSetAlphaToOneEnableEXT         = PFN_vkCmdSetAlphaToOneEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetAlphaToOneEnableEXT" ) );
      vkCmdSetLogicOpEnableEXT            = PFN_vkCmdSetLogicOpEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetLogicOpEnableEXT" ) );
      vkCmdSetColorBlendEnableEXT         = PFN_vkCmdSetColorBlendEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetColorBlendEnableEXT" ) );
      vkCmdSetColorBlendEquationEXT       = PFN_vkCmdSetColorBlendEquationEXT( vkGetInstanceProcAddr( instance, "vkCmdSetColorBlendEquationEXT" ) );
      vkCmdSetColorWriteMaskEXT           = PFN_vkCmdSetColorWriteMaskEXT( vkGetInstanceProcAddr( instance, "vkCmdSetColorWriteMaskEXT" ) );
      vkCmdSetTessellationDomainOriginEXT = PFN_vkCmdSetTessellationDomainOriginEXT( vkGetInstanceProcAddr( instance, "vkCmdSetTessellationDomainOriginEXT" ) );
      vkCmdSetRasterizationStreamEXT      = PFN_vkCmdSetRasterizationStreamEXT( vkGetInstanceProcAddr( instance, "vkCmdSetRasterizationStreamEXT" ) );
      vkCmdSetConservativeRasterizationModeEXT =
        PFN_vkCmdSetConservativeRasterizationModeEXT( vkGetInstanceProcAddr( instance, "vkCmdSetConservativeRasterizationModeEXT" ) );
      vkCmdSetExtraPrimitiveOverestimationSizeEXT =
        PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT( vkGetInstanceProcAddr( instance, "vkCmdSetExtraPrimitiveOverestimationSizeEXT" ) );
      vkCmdSetDepthClipEnableEXT       = PFN_vkCmdSetDepthClipEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthClipEnableEXT" ) );
      vkCmdSetSampleLocationsEnableEXT = PFN_vkCmdSetSampleLocationsEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetSampleLocationsEnableEXT" ) );
      vkCmdSetColorBlendAdvancedEXT    = PFN_vkCmdSetColorBlendAdvancedEXT( vkGetInstanceProcAddr( instance, "vkCmdSetColorBlendAdvancedEXT" ) );
      vkCmdSetProvokingVertexModeEXT   = PFN_vkCmdSetProvokingVertexModeEXT( vkGetInstanceProcAddr( instance, "vkCmdSetProvokingVertexModeEXT" ) );
      vkCmdSetLineRasterizationModeEXT = PFN_vkCmdSetLineRasterizationModeEXT( vkGetInstanceProcAddr( instance, "vkCmdSetLineRasterizationModeEXT" ) );
      vkCmdSetLineStippleEnableEXT     = PFN_vkCmdSetLineStippleEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetLineStippleEnableEXT" ) );
      vkCmdSetDepthClipNegativeOneToOneEXT =
        PFN_vkCmdSetDepthClipNegativeOneToOneEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthClipNegativeOneToOneEXT" ) );
      vkCmdSetViewportWScalingEnableNV  = PFN_vkCmdSetViewportWScalingEnableNV( vkGetInstanceProcAddr( instance, "vkCmdSetViewportWScalingEnableNV" ) );
      vkCmdSetViewportSwizzleNV         = PFN_vkCmdSetViewportSwizzleNV( vkGetInstanceProcAddr( instance, "vkCmdSetViewportSwizzleNV" ) );
      vkCmdSetCoverageToColorEnableNV   = PFN_vkCmdSetCoverageToColorEnableNV( vkGetInstanceProcAddr( instance, "vkCmdSetCoverageToColorEnableNV" ) );
      vkCmdSetCoverageToColorLocationNV = PFN_vkCmdSetCoverageToColorLocationNV( vkGetInstanceProcAddr( instance, "vkCmdSetCoverageToColorLocationNV" ) );
      vkCmdSetCoverageModulationModeNV  = PFN_vkCmdSetCoverageModulationModeNV( vkGetInstanceProcAddr( instance, "vkCmdSetCoverageModulationModeNV" ) );
      vkCmdSetCoverageModulationTableEnableNV =
        PFN_vkCmdSetCoverageModulationTableEnableNV( vkGetInstanceProcAddr( instance, "vkCmdSetCoverageModulationTableEnableNV" ) );
      vkCmdSetCoverageModulationTableNV = PFN_vkCmdSetCoverageModulationTableNV( vkGetInstanceProcAddr( instance, "vkCmdSetCoverageModulationTableNV" ) );
      vkCmdSetShadingRateImageEnableNV  = PFN_vkCmdSetShadingRateImageEnableNV( vkGetInstanceProcAddr( instance, "vkCmdSetShadingRateImageEnableNV" ) );
      vkCmdSetRepresentativeFragmentTestEnableNV =
        PFN_vkCmdSetRepresentativeFragmentTestEnableNV( vkGetInstanceProcAddr( instance, "vkCmdSetRepresentativeFragmentTestEnableNV" ) );
      vkCmdSetCoverageReductionModeNV = PFN_vkCmdSetCoverageReductionModeNV( vkGetInstanceProcAddr( instance, "vkCmdSetCoverageReductionModeNV" ) );

      //=== VK_ARM_tensors ===
      vkCreateTensorARM                = PFN_vkCreateTensorARM( vkGetInstanceProcAddr( instance, "vkCreateTensorARM" ) );
      vkDestroyTensorARM               = PFN_vkDestroyTensorARM( vkGetInstanceProcAddr( instance, "vkDestroyTensorARM" ) );
      vkCreateTensorViewARM            = PFN_vkCreateTensorViewARM( vkGetInstanceProcAddr( instance, "vkCreateTensorViewARM" ) );
      vkDestroyTensorViewARM           = PFN_vkDestroyTensorViewARM( vkGetInstanceProcAddr( instance, "vkDestroyTensorViewARM" ) );
      vkGetTensorMemoryRequirementsARM = PFN_vkGetTensorMemoryRequirementsARM( vkGetInstanceProcAddr( instance, "vkGetTensorMemoryRequirementsARM" ) );
      vkBindTensorMemoryARM            = PFN_vkBindTensorMemoryARM( vkGetInstanceProcAddr( instance, "vkBindTensorMemoryARM" ) );
      vkGetDeviceTensorMemoryRequirementsARM =
        PFN_vkGetDeviceTensorMemoryRequirementsARM( vkGetInstanceProcAddr( instance, "vkGetDeviceTensorMemoryRequirementsARM" ) );
      vkCmdCopyTensorARM = PFN_vkCmdCopyTensorARM( vkGetInstanceProcAddr( instance, "vkCmdCopyTensorARM" ) );
      vkGetPhysicalDeviceExternalTensorPropertiesARM =
        PFN_vkGetPhysicalDeviceExternalTensorPropertiesARM( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceExternalTensorPropertiesARM" ) );
      vkGetTensorOpaqueCaptureDescriptorDataARM =
        PFN_vkGetTensorOpaqueCaptureDescriptorDataARM( vkGetInstanceProcAddr( instance, "vkGetTensorOpaqueCaptureDescriptorDataARM" ) );
      vkGetTensorViewOpaqueCaptureDescriptorDataARM =
        PFN_vkGetTensorViewOpaqueCaptureDescriptorDataARM( vkGetInstanceProcAddr( instance, "vkGetTensorViewOpaqueCaptureDescriptorDataARM" ) );

      //=== VK_EXT_shader_module_identifier ===
      vkGetShaderModuleIdentifierEXT = PFN_vkGetShaderModuleIdentifierEXT( vkGetInstanceProcAddr( instance, "vkGetShaderModuleIdentifierEXT" ) );
      vkGetShaderModuleCreateInfoIdentifierEXT =
        PFN_vkGetShaderModuleCreateInfoIdentifierEXT( vkGetInstanceProcAddr( instance, "vkGetShaderModuleCreateInfoIdentifierEXT" ) );

      //=== VK_NV_optical_flow ===
      vkGetPhysicalDeviceOpticalFlowImageFormatsNV =
        PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceOpticalFlowImageFormatsNV" ) );
      vkCreateOpticalFlowSessionNV    = PFN_vkCreateOpticalFlowSessionNV( vkGetInstanceProcAddr( instance, "vkCreateOpticalFlowSessionNV" ) );
      vkDestroyOpticalFlowSessionNV   = PFN_vkDestroyOpticalFlowSessionNV( vkGetInstanceProcAddr( instance, "vkDestroyOpticalFlowSessionNV" ) );
      vkBindOpticalFlowSessionImageNV = PFN_vkBindOpticalFlowSessionImageNV( vkGetInstanceProcAddr( instance, "vkBindOpticalFlowSessionImageNV" ) );
      vkCmdOpticalFlowExecuteNV       = PFN_vkCmdOpticalFlowExecuteNV( vkGetInstanceProcAddr( instance, "vkCmdOpticalFlowExecuteNV" ) );

      //=== VK_KHR_maintenance5 ===
      vkCmdBindIndexBuffer2KHR = PFN_vkCmdBindIndexBuffer2KHR( vkGetInstanceProcAddr( instance, "vkCmdBindIndexBuffer2KHR" ) );
      if ( !vkCmdBindIndexBuffer2 )
        vkCmdBindIndexBuffer2 = vkCmdBindIndexBuffer2KHR;
      vkGetRenderingAreaGranularityKHR = PFN_vkGetRenderingAreaGranularityKHR( vkGetInstanceProcAddr( instance, "vkGetRenderingAreaGranularityKHR" ) );
      if ( !vkGetRenderingAreaGranularity )
        vkGetRenderingAreaGranularity = vkGetRenderingAreaGranularityKHR;
      vkGetDeviceImageSubresourceLayoutKHR =
        PFN_vkGetDeviceImageSubresourceLayoutKHR( vkGetInstanceProcAddr( instance, "vkGetDeviceImageSubresourceLayoutKHR" ) );
      if ( !vkGetDeviceImageSubresourceLayout )
        vkGetDeviceImageSubresourceLayout = vkGetDeviceImageSubresourceLayoutKHR;
      vkGetImageSubresourceLayout2KHR = PFN_vkGetImageSubresourceLayout2KHR( vkGetInstanceProcAddr( instance, "vkGetImageSubresourceLayout2KHR" ) );
      if ( !vkGetImageSubresourceLayout2 )
        vkGetImageSubresourceLayout2 = vkGetImageSubresourceLayout2KHR;

      //=== VK_AMD_anti_lag ===
      vkAntiLagUpdateAMD = PFN_vkAntiLagUpdateAMD( vkGetInstanceProcAddr( instance, "vkAntiLagUpdateAMD" ) );

      //=== VK_KHR_present_wait2 ===
      vkWaitForPresent2KHR = PFN_vkWaitForPresent2KHR( vkGetInstanceProcAddr( instance, "vkWaitForPresent2KHR" ) );

      //=== VK_EXT_shader_object ===
      vkCreateShadersEXT         = PFN_vkCreateShadersEXT( vkGetInstanceProcAddr( instance, "vkCreateShadersEXT" ) );
      vkDestroyShaderEXT         = PFN_vkDestroyShaderEXT( vkGetInstanceProcAddr( instance, "vkDestroyShaderEXT" ) );
      vkGetShaderBinaryDataEXT   = PFN_vkGetShaderBinaryDataEXT( vkGetInstanceProcAddr( instance, "vkGetShaderBinaryDataEXT" ) );
      vkCmdBindShadersEXT        = PFN_vkCmdBindShadersEXT( vkGetInstanceProcAddr( instance, "vkCmdBindShadersEXT" ) );
      vkCmdSetDepthClampRangeEXT = PFN_vkCmdSetDepthClampRangeEXT( vkGetInstanceProcAddr( instance, "vkCmdSetDepthClampRangeEXT" ) );

      //=== VK_KHR_pipeline_binary ===
      vkCreatePipelineBinariesKHR      = PFN_vkCreatePipelineBinariesKHR( vkGetInstanceProcAddr( instance, "vkCreatePipelineBinariesKHR" ) );
      vkDestroyPipelineBinaryKHR       = PFN_vkDestroyPipelineBinaryKHR( vkGetInstanceProcAddr( instance, "vkDestroyPipelineBinaryKHR" ) );
      vkGetPipelineKeyKHR              = PFN_vkGetPipelineKeyKHR( vkGetInstanceProcAddr( instance, "vkGetPipelineKeyKHR" ) );
      vkGetPipelineBinaryDataKHR       = PFN_vkGetPipelineBinaryDataKHR( vkGetInstanceProcAddr( instance, "vkGetPipelineBinaryDataKHR" ) );
      vkReleaseCapturedPipelineDataKHR = PFN_vkReleaseCapturedPipelineDataKHR( vkGetInstanceProcAddr( instance, "vkReleaseCapturedPipelineDataKHR" ) );

      //=== VK_QCOM_tile_properties ===
      vkGetFramebufferTilePropertiesQCOM = PFN_vkGetFramebufferTilePropertiesQCOM( vkGetInstanceProcAddr( instance, "vkGetFramebufferTilePropertiesQCOM" ) );
      vkGetDynamicRenderingTilePropertiesQCOM =
        PFN_vkGetDynamicRenderingTilePropertiesQCOM( vkGetInstanceProcAddr( instance, "vkGetDynamicRenderingTilePropertiesQCOM" ) );

      //=== VK_KHR_swapchain_maintenance1 ===
      vkReleaseSwapchainImagesKHR = PFN_vkReleaseSwapchainImagesKHR( vkGetInstanceProcAddr( instance, "vkReleaseSwapchainImagesKHR" ) );

      //=== VK_NV_cooperative_vector ===
      vkGetPhysicalDeviceCooperativeVectorPropertiesNV =
        PFN_vkGetPhysicalDeviceCooperativeVectorPropertiesNV( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceCooperativeVectorPropertiesNV" ) );
      vkConvertCooperativeVectorMatrixNV = PFN_vkConvertCooperativeVectorMatrixNV( vkGetInstanceProcAddr( instance, "vkConvertCooperativeVectorMatrixNV" ) );
      vkCmdConvertCooperativeVectorMatrixNV =
        PFN_vkCmdConvertCooperativeVectorMatrixNV( vkGetInstanceProcAddr( instance, "vkCmdConvertCooperativeVectorMatrixNV" ) );

      //=== VK_NV_low_latency2 ===
      vkSetLatencySleepModeNV  = PFN_vkSetLatencySleepModeNV( vkGetInstanceProcAddr( instance, "vkSetLatencySleepModeNV" ) );
      vkLatencySleepNV         = PFN_vkLatencySleepNV( vkGetInstanceProcAddr( instance, "vkLatencySleepNV" ) );
      vkSetLatencyMarkerNV     = PFN_vkSetLatencyMarkerNV( vkGetInstanceProcAddr( instance, "vkSetLatencyMarkerNV" ) );
      vkGetLatencyTimingsNV    = PFN_vkGetLatencyTimingsNV( vkGetInstanceProcAddr( instance, "vkGetLatencyTimingsNV" ) );
      vkQueueNotifyOutOfBandNV = PFN_vkQueueNotifyOutOfBandNV( vkGetInstanceProcAddr( instance, "vkQueueNotifyOutOfBandNV" ) );

      //=== VK_KHR_cooperative_matrix ===
      vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR =
        PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR" ) );

      //=== VK_ARM_data_graph ===
      vkCreateDataGraphPipelinesARM       = PFN_vkCreateDataGraphPipelinesARM( vkGetInstanceProcAddr( instance, "vkCreateDataGraphPipelinesARM" ) );
      vkCreateDataGraphPipelineSessionARM = PFN_vkCreateDataGraphPipelineSessionARM( vkGetInstanceProcAddr( instance, "vkCreateDataGraphPipelineSessionARM" ) );
      vkGetDataGraphPipelineSessionBindPointRequirementsARM =
        PFN_vkGetDataGraphPipelineSessionBindPointRequirementsARM( vkGetInstanceProcAddr( instance, "vkGetDataGraphPipelineSessionBindPointRequirementsARM" ) );
      vkGetDataGraphPipelineSessionMemoryRequirementsARM =
        PFN_vkGetDataGraphPipelineSessionMemoryRequirementsARM( vkGetInstanceProcAddr( instance, "vkGetDataGraphPipelineSessionMemoryRequirementsARM" ) );
      vkBindDataGraphPipelineSessionMemoryARM =
        PFN_vkBindDataGraphPipelineSessionMemoryARM( vkGetInstanceProcAddr( instance, "vkBindDataGraphPipelineSessionMemoryARM" ) );
      vkDestroyDataGraphPipelineSessionARM =
        PFN_vkDestroyDataGraphPipelineSessionARM( vkGetInstanceProcAddr( instance, "vkDestroyDataGraphPipelineSessionARM" ) );
      vkCmdDispatchDataGraphARM = PFN_vkCmdDispatchDataGraphARM( vkGetInstanceProcAddr( instance, "vkCmdDispatchDataGraphARM" ) );
      vkGetDataGraphPipelineAvailablePropertiesARM =
        PFN_vkGetDataGraphPipelineAvailablePropertiesARM( vkGetInstanceProcAddr( instance, "vkGetDataGraphPipelineAvailablePropertiesARM" ) );
      vkGetDataGraphPipelinePropertiesARM = PFN_vkGetDataGraphPipelinePropertiesARM( vkGetInstanceProcAddr( instance, "vkGetDataGraphPipelinePropertiesARM" ) );
      vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM =
        PFN_vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM" ) );
      vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM = PFN_vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM(
        vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM" ) );

      //=== VK_EXT_attachment_feedback_loop_dynamic_state ===
      vkCmdSetAttachmentFeedbackLoopEnableEXT =
        PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT( vkGetInstanceProcAddr( instance, "vkCmdSetAttachmentFeedbackLoopEnableEXT" ) );

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
      //=== VK_QNX_external_memory_screen_buffer ===
      vkGetScreenBufferPropertiesQNX = PFN_vkGetScreenBufferPropertiesQNX( vkGetInstanceProcAddr( instance, "vkGetScreenBufferPropertiesQNX" ) );
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/

      //=== VK_KHR_line_rasterization ===
      vkCmdSetLineStippleKHR = PFN_vkCmdSetLineStippleKHR( vkGetInstanceProcAddr( instance, "vkCmdSetLineStippleKHR" ) );
      if ( !vkCmdSetLineStipple )
        vkCmdSetLineStipple = vkCmdSetLineStippleKHR;

      //=== VK_KHR_calibrated_timestamps ===
      vkGetPhysicalDeviceCalibrateableTimeDomainsKHR =
        PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceCalibrateableTimeDomainsKHR" ) );
      vkGetCalibratedTimestampsKHR = PFN_vkGetCalibratedTimestampsKHR( vkGetInstanceProcAddr( instance, "vkGetCalibratedTimestampsKHR" ) );

      //=== VK_KHR_maintenance6 ===
      vkCmdBindDescriptorSets2KHR = PFN_vkCmdBindDescriptorSets2KHR( vkGetInstanceProcAddr( instance, "vkCmdBindDescriptorSets2KHR" ) );
      if ( !vkCmdBindDescriptorSets2 )
        vkCmdBindDescriptorSets2 = vkCmdBindDescriptorSets2KHR;
      vkCmdPushConstants2KHR = PFN_vkCmdPushConstants2KHR( vkGetInstanceProcAddr( instance, "vkCmdPushConstants2KHR" ) );
      if ( !vkCmdPushConstants2 )
        vkCmdPushConstants2 = vkCmdPushConstants2KHR;
      vkCmdPushDescriptorSet2KHR = PFN_vkCmdPushDescriptorSet2KHR( vkGetInstanceProcAddr( instance, "vkCmdPushDescriptorSet2KHR" ) );
      if ( !vkCmdPushDescriptorSet2 )
        vkCmdPushDescriptorSet2 = vkCmdPushDescriptorSet2KHR;
      vkCmdPushDescriptorSetWithTemplate2KHR =
        PFN_vkCmdPushDescriptorSetWithTemplate2KHR( vkGetInstanceProcAddr( instance, "vkCmdPushDescriptorSetWithTemplate2KHR" ) );
      if ( !vkCmdPushDescriptorSetWithTemplate2 )
        vkCmdPushDescriptorSetWithTemplate2 = vkCmdPushDescriptorSetWithTemplate2KHR;
      vkCmdSetDescriptorBufferOffsets2EXT = PFN_vkCmdSetDescriptorBufferOffsets2EXT( vkGetInstanceProcAddr( instance, "vkCmdSetDescriptorBufferOffsets2EXT" ) );
      vkCmdBindDescriptorBufferEmbeddedSamplers2EXT =
        PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT( vkGetInstanceProcAddr( instance, "vkCmdBindDescriptorBufferEmbeddedSamplers2EXT" ) );

      //=== VK_QCOM_tile_memory_heap ===
      vkCmdBindTileMemoryQCOM = PFN_vkCmdBindTileMemoryQCOM( vkGetInstanceProcAddr( instance, "vkCmdBindTileMemoryQCOM" ) );

      //=== VK_KHR_copy_memory_indirect ===
      vkCmdCopyMemoryIndirectKHR        = PFN_vkCmdCopyMemoryIndirectKHR( vkGetInstanceProcAddr( instance, "vkCmdCopyMemoryIndirectKHR" ) );
      vkCmdCopyMemoryToImageIndirectKHR = PFN_vkCmdCopyMemoryToImageIndirectKHR( vkGetInstanceProcAddr( instance, "vkCmdCopyMemoryToImageIndirectKHR" ) );

      //=== VK_EXT_memory_decompression ===
      vkCmdDecompressMemoryEXT = PFN_vkCmdDecompressMemoryEXT( vkGetInstanceProcAddr( instance, "vkCmdDecompressMemoryEXT" ) );
      vkCmdDecompressMemoryIndirectCountEXT =
        PFN_vkCmdDecompressMemoryIndirectCountEXT( vkGetInstanceProcAddr( instance, "vkCmdDecompressMemoryIndirectCountEXT" ) );

      //=== VK_NV_external_compute_queue ===
      vkCreateExternalComputeQueueNV  = PFN_vkCreateExternalComputeQueueNV( vkGetInstanceProcAddr( instance, "vkCreateExternalComputeQueueNV" ) );
      vkDestroyExternalComputeQueueNV = PFN_vkDestroyExternalComputeQueueNV( vkGetInstanceProcAddr( instance, "vkDestroyExternalComputeQueueNV" ) );
      vkGetExternalComputeQueueDataNV = PFN_vkGetExternalComputeQueueDataNV( vkGetInstanceProcAddr( instance, "vkGetExternalComputeQueueDataNV" ) );

      //=== VK_NV_cluster_acceleration_structure ===
      vkGetClusterAccelerationStructureBuildSizesNV =
        PFN_vkGetClusterAccelerationStructureBuildSizesNV( vkGetInstanceProcAddr( instance, "vkGetClusterAccelerationStructureBuildSizesNV" ) );
      vkCmdBuildClusterAccelerationStructureIndirectNV =
        PFN_vkCmdBuildClusterAccelerationStructureIndirectNV( vkGetInstanceProcAddr( instance, "vkCmdBuildClusterAccelerationStructureIndirectNV" ) );

      //=== VK_NV_partitioned_acceleration_structure ===
      vkGetPartitionedAccelerationStructuresBuildSizesNV =
        PFN_vkGetPartitionedAccelerationStructuresBuildSizesNV( vkGetInstanceProcAddr( instance, "vkGetPartitionedAccelerationStructuresBuildSizesNV" ) );
      vkCmdBuildPartitionedAccelerationStructuresNV =
        PFN_vkCmdBuildPartitionedAccelerationStructuresNV( vkGetInstanceProcAddr( instance, "vkCmdBuildPartitionedAccelerationStructuresNV" ) );

      //=== VK_EXT_device_generated_commands ===
      vkGetGeneratedCommandsMemoryRequirementsEXT =
        PFN_vkGetGeneratedCommandsMemoryRequirementsEXT( vkGetInstanceProcAddr( instance, "vkGetGeneratedCommandsMemoryRequirementsEXT" ) );
      vkCmdPreprocessGeneratedCommandsEXT = PFN_vkCmdPreprocessGeneratedCommandsEXT( vkGetInstanceProcAddr( instance, "vkCmdPreprocessGeneratedCommandsEXT" ) );
      vkCmdExecuteGeneratedCommandsEXT    = PFN_vkCmdExecuteGeneratedCommandsEXT( vkGetInstanceProcAddr( instance, "vkCmdExecuteGeneratedCommandsEXT" ) );
      vkCreateIndirectCommandsLayoutEXT   = PFN_vkCreateIndirectCommandsLayoutEXT( vkGetInstanceProcAddr( instance, "vkCreateIndirectCommandsLayoutEXT" ) );
      vkDestroyIndirectCommandsLayoutEXT  = PFN_vkDestroyIndirectCommandsLayoutEXT( vkGetInstanceProcAddr( instance, "vkDestroyIndirectCommandsLayoutEXT" ) );
      vkCreateIndirectExecutionSetEXT     = PFN_vkCreateIndirectExecutionSetEXT( vkGetInstanceProcAddr( instance, "vkCreateIndirectExecutionSetEXT" ) );
      vkDestroyIndirectExecutionSetEXT    = PFN_vkDestroyIndirectExecutionSetEXT( vkGetInstanceProcAddr( instance, "vkDestroyIndirectExecutionSetEXT" ) );
      vkUpdateIndirectExecutionSetPipelineEXT =
        PFN_vkUpdateIndirectExecutionSetPipelineEXT( vkGetInstanceProcAddr( instance, "vkUpdateIndirectExecutionSetPipelineEXT" ) );
      vkUpdateIndirectExecutionSetShaderEXT =
        PFN_vkUpdateIndirectExecutionSetShaderEXT( vkGetInstanceProcAddr( instance, "vkUpdateIndirectExecutionSetShaderEXT" ) );

#if defined( VK_USE_PLATFORM_OHOS )
      //=== VK_OHOS_surface ===
      vkCreateSurfaceOHOS = PFN_vkCreateSurfaceOHOS( vkGetInstanceProcAddr( instance, "vkCreateSurfaceOHOS" ) );
#endif /*VK_USE_PLATFORM_OHOS*/

      //=== VK_NV_cooperative_matrix2 ===
      vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV = PFN_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV(
        vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV" ) );

#if defined( VK_USE_PLATFORM_METAL_EXT )
      //=== VK_EXT_external_memory_metal ===
      vkGetMemoryMetalHandleEXT           = PFN_vkGetMemoryMetalHandleEXT( vkGetInstanceProcAddr( instance, "vkGetMemoryMetalHandleEXT" ) );
      vkGetMemoryMetalHandlePropertiesEXT = PFN_vkGetMemoryMetalHandlePropertiesEXT( vkGetInstanceProcAddr( instance, "vkGetMemoryMetalHandlePropertiesEXT" ) );
#endif /*VK_USE_PLATFORM_METAL_EXT*/

      //=== VK_ARM_performance_counters_by_region ===
      vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM = PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM(
        vkGetInstanceProcAddr( instance, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM" ) );

      //=== VK_EXT_fragment_density_map_offset ===
      vkCmdEndRendering2EXT = PFN_vkCmdEndRendering2EXT( vkGetInstanceProcAddr( instance, "vkCmdEndRendering2EXT" ) );
      if ( !vkCmdEndRendering2KHR )
        vkCmdEndRendering2KHR = vkCmdEndRendering2EXT;

      //=== VK_EXT_custom_resolve ===
      vkCmdBeginCustomResolveEXT = PFN_vkCmdBeginCustomResolveEXT( vkGetInstanceProcAddr( instance, "vkCmdBeginCustomResolveEXT" ) );

      //=== VK_KHR_maintenance10 ===
      vkCmdEndRendering2KHR = PFN_vkCmdEndRendering2KHR( vkGetInstanceProcAddr( instance, "vkCmdEndRendering2KHR" ) );

      //=== VK_NV_compute_occupancy_priority ===
      vkCmdSetComputeOccupancyPriorityNV = PFN_vkCmdSetComputeOccupancyPriorityNV( vkGetInstanceProcAddr( instance, "vkCmdSetComputeOccupancyPriorityNV" ) );

#if defined( VK_USE_PLATFORM_UBM_SEC )
      //=== VK_SEC_ubm_surface ===
      vkCreateUbmSurfaceSEC = PFN_vkCreateUbmSurfaceSEC( vkGetInstanceProcAddr( instance, "vkCreateUbmSurfaceSEC" ) );
      vkGetPhysicalDeviceUbmPresentationSupportSEC =
        PFN_vkGetPhysicalDeviceUbmPresentationSupportSEC( vkGetInstanceProcAddr( instance, "vkGetPhysicalDeviceUbmPresentationSupportSEC" ) );
#endif /*VK_USE_PLATFORM_UBM_SEC*/
    }

    void init( VkDevice device ) VULKAN_HPP_NOEXCEPT
    {
      //=== VK_VERSION_1_0 ===
      vkGetDeviceProcAddr                = PFN_vkGetDeviceProcAddr( vkGetDeviceProcAddr( device, "vkGetDeviceProcAddr" ) );
      vkDestroyDevice                    = PFN_vkDestroyDevice( vkGetDeviceProcAddr( device, "vkDestroyDevice" ) );
      vkGetDeviceQueue                   = PFN_vkGetDeviceQueue( vkGetDeviceProcAddr( device, "vkGetDeviceQueue" ) );
      vkQueueSubmit                      = PFN_vkQueueSubmit( vkGetDeviceProcAddr( device, "vkQueueSubmit" ) );
      vkQueueWaitIdle                    = PFN_vkQueueWaitIdle( vkGetDeviceProcAddr( device, "vkQueueWaitIdle" ) );
      vkDeviceWaitIdle                   = PFN_vkDeviceWaitIdle( vkGetDeviceProcAddr( device, "vkDeviceWaitIdle" ) );
      vkAllocateMemory                   = PFN_vkAllocateMemory( vkGetDeviceProcAddr( device, "vkAllocateMemory" ) );
      vkFreeMemory                       = PFN_vkFreeMemory( vkGetDeviceProcAddr( device, "vkFreeMemory" ) );
      vkMapMemory                        = PFN_vkMapMemory( vkGetDeviceProcAddr( device, "vkMapMemory" ) );
      vkUnmapMemory                      = PFN_vkUnmapMemory( vkGetDeviceProcAddr( device, "vkUnmapMemory" ) );
      vkFlushMappedMemoryRanges          = PFN_vkFlushMappedMemoryRanges( vkGetDeviceProcAddr( device, "vkFlushMappedMemoryRanges" ) );
      vkInvalidateMappedMemoryRanges     = PFN_vkInvalidateMappedMemoryRanges( vkGetDeviceProcAddr( device, "vkInvalidateMappedMemoryRanges" ) );
      vkGetDeviceMemoryCommitment        = PFN_vkGetDeviceMemoryCommitment( vkGetDeviceProcAddr( device, "vkGetDeviceMemoryCommitment" ) );
      vkBindBufferMemory                 = PFN_vkBindBufferMemory( vkGetDeviceProcAddr( device, "vkBindBufferMemory" ) );
      vkBindImageMemory                  = PFN_vkBindImageMemory( vkGetDeviceProcAddr( device, "vkBindImageMemory" ) );
      vkGetBufferMemoryRequirements      = PFN_vkGetBufferMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetBufferMemoryRequirements" ) );
      vkGetImageMemoryRequirements       = PFN_vkGetImageMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetImageMemoryRequirements" ) );
      vkGetImageSparseMemoryRequirements = PFN_vkGetImageSparseMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetImageSparseMemoryRequirements" ) );
      vkQueueBindSparse                  = PFN_vkQueueBindSparse( vkGetDeviceProcAddr( device, "vkQueueBindSparse" ) );
      vkCreateFence                      = PFN_vkCreateFence( vkGetDeviceProcAddr( device, "vkCreateFence" ) );
      vkDestroyFence                     = PFN_vkDestroyFence( vkGetDeviceProcAddr( device, "vkDestroyFence" ) );
      vkResetFences                      = PFN_vkResetFences( vkGetDeviceProcAddr( device, "vkResetFences" ) );
      vkGetFenceStatus                   = PFN_vkGetFenceStatus( vkGetDeviceProcAddr( device, "vkGetFenceStatus" ) );
      vkWaitForFences                    = PFN_vkWaitForFences( vkGetDeviceProcAddr( device, "vkWaitForFences" ) );
      vkCreateSemaphore                  = PFN_vkCreateSemaphore( vkGetDeviceProcAddr( device, "vkCreateSemaphore" ) );
      vkDestroySemaphore                 = PFN_vkDestroySemaphore( vkGetDeviceProcAddr( device, "vkDestroySemaphore" ) );
      vkCreateQueryPool                  = PFN_vkCreateQueryPool( vkGetDeviceProcAddr( device, "vkCreateQueryPool" ) );
      vkDestroyQueryPool                 = PFN_vkDestroyQueryPool( vkGetDeviceProcAddr( device, "vkDestroyQueryPool" ) );
      vkGetQueryPoolResults              = PFN_vkGetQueryPoolResults( vkGetDeviceProcAddr( device, "vkGetQueryPoolResults" ) );
      vkCreateBuffer                     = PFN_vkCreateBuffer( vkGetDeviceProcAddr( device, "vkCreateBuffer" ) );
      vkDestroyBuffer                    = PFN_vkDestroyBuffer( vkGetDeviceProcAddr( device, "vkDestroyBuffer" ) );
      vkCreateImage                      = PFN_vkCreateImage( vkGetDeviceProcAddr( device, "vkCreateImage" ) );
      vkDestroyImage                     = PFN_vkDestroyImage( vkGetDeviceProcAddr( device, "vkDestroyImage" ) );
      vkGetImageSubresourceLayout        = PFN_vkGetImageSubresourceLayout( vkGetDeviceProcAddr( device, "vkGetImageSubresourceLayout" ) );
      vkCreateImageView                  = PFN_vkCreateImageView( vkGetDeviceProcAddr( device, "vkCreateImageView" ) );
      vkDestroyImageView                 = PFN_vkDestroyImageView( vkGetDeviceProcAddr( device, "vkDestroyImageView" ) );
      vkCreateCommandPool                = PFN_vkCreateCommandPool( vkGetDeviceProcAddr( device, "vkCreateCommandPool" ) );
      vkDestroyCommandPool               = PFN_vkDestroyCommandPool( vkGetDeviceProcAddr( device, "vkDestroyCommandPool" ) );
      vkResetCommandPool                 = PFN_vkResetCommandPool( vkGetDeviceProcAddr( device, "vkResetCommandPool" ) );
      vkAllocateCommandBuffers           = PFN_vkAllocateCommandBuffers( vkGetDeviceProcAddr( device, "vkAllocateCommandBuffers" ) );
      vkFreeCommandBuffers               = PFN_vkFreeCommandBuffers( vkGetDeviceProcAddr( device, "vkFreeCommandBuffers" ) );
      vkBeginCommandBuffer               = PFN_vkBeginCommandBuffer( vkGetDeviceProcAddr( device, "vkBeginCommandBuffer" ) );
      vkEndCommandBuffer                 = PFN_vkEndCommandBuffer( vkGetDeviceProcAddr( device, "vkEndCommandBuffer" ) );
      vkResetCommandBuffer               = PFN_vkResetCommandBuffer( vkGetDeviceProcAddr( device, "vkResetCommandBuffer" ) );
      vkCmdCopyBuffer                    = PFN_vkCmdCopyBuffer( vkGetDeviceProcAddr( device, "vkCmdCopyBuffer" ) );
      vkCmdCopyImage                     = PFN_vkCmdCopyImage( vkGetDeviceProcAddr( device, "vkCmdCopyImage" ) );
      vkCmdCopyBufferToImage             = PFN_vkCmdCopyBufferToImage( vkGetDeviceProcAddr( device, "vkCmdCopyBufferToImage" ) );
      vkCmdCopyImageToBuffer             = PFN_vkCmdCopyImageToBuffer( vkGetDeviceProcAddr( device, "vkCmdCopyImageToBuffer" ) );
      vkCmdUpdateBuffer                  = PFN_vkCmdUpdateBuffer( vkGetDeviceProcAddr( device, "vkCmdUpdateBuffer" ) );
      vkCmdFillBuffer                    = PFN_vkCmdFillBuffer( vkGetDeviceProcAddr( device, "vkCmdFillBuffer" ) );
      vkCmdPipelineBarrier               = PFN_vkCmdPipelineBarrier( vkGetDeviceProcAddr( device, "vkCmdPipelineBarrier" ) );
      vkCmdBeginQuery                    = PFN_vkCmdBeginQuery( vkGetDeviceProcAddr( device, "vkCmdBeginQuery" ) );
      vkCmdEndQuery                      = PFN_vkCmdEndQuery( vkGetDeviceProcAddr( device, "vkCmdEndQuery" ) );
      vkCmdResetQueryPool                = PFN_vkCmdResetQueryPool( vkGetDeviceProcAddr( device, "vkCmdResetQueryPool" ) );
      vkCmdWriteTimestamp                = PFN_vkCmdWriteTimestamp( vkGetDeviceProcAddr( device, "vkCmdWriteTimestamp" ) );
      vkCmdCopyQueryPoolResults          = PFN_vkCmdCopyQueryPoolResults( vkGetDeviceProcAddr( device, "vkCmdCopyQueryPoolResults" ) );
      vkCmdExecuteCommands               = PFN_vkCmdExecuteCommands( vkGetDeviceProcAddr( device, "vkCmdExecuteCommands" ) );
      vkCreateEvent                      = PFN_vkCreateEvent( vkGetDeviceProcAddr( device, "vkCreateEvent" ) );
      vkDestroyEvent                     = PFN_vkDestroyEvent( vkGetDeviceProcAddr( device, "vkDestroyEvent" ) );
      vkGetEventStatus                   = PFN_vkGetEventStatus( vkGetDeviceProcAddr( device, "vkGetEventStatus" ) );
      vkSetEvent                         = PFN_vkSetEvent( vkGetDeviceProcAddr( device, "vkSetEvent" ) );
      vkResetEvent                       = PFN_vkResetEvent( vkGetDeviceProcAddr( device, "vkResetEvent" ) );
      vkCreateBufferView                 = PFN_vkCreateBufferView( vkGetDeviceProcAddr( device, "vkCreateBufferView" ) );
      vkDestroyBufferView                = PFN_vkDestroyBufferView( vkGetDeviceProcAddr( device, "vkDestroyBufferView" ) );
      vkCreateShaderModule               = PFN_vkCreateShaderModule( vkGetDeviceProcAddr( device, "vkCreateShaderModule" ) );
      vkDestroyShaderModule              = PFN_vkDestroyShaderModule( vkGetDeviceProcAddr( device, "vkDestroyShaderModule" ) );
      vkCreatePipelineCache              = PFN_vkCreatePipelineCache( vkGetDeviceProcAddr( device, "vkCreatePipelineCache" ) );
      vkDestroyPipelineCache             = PFN_vkDestroyPipelineCache( vkGetDeviceProcAddr( device, "vkDestroyPipelineCache" ) );
      vkGetPipelineCacheData             = PFN_vkGetPipelineCacheData( vkGetDeviceProcAddr( device, "vkGetPipelineCacheData" ) );
      vkMergePipelineCaches              = PFN_vkMergePipelineCaches( vkGetDeviceProcAddr( device, "vkMergePipelineCaches" ) );
      vkCreateComputePipelines           = PFN_vkCreateComputePipelines( vkGetDeviceProcAddr( device, "vkCreateComputePipelines" ) );
      vkDestroyPipeline                  = PFN_vkDestroyPipeline( vkGetDeviceProcAddr( device, "vkDestroyPipeline" ) );
      vkCreatePipelineLayout             = PFN_vkCreatePipelineLayout( vkGetDeviceProcAddr( device, "vkCreatePipelineLayout" ) );
      vkDestroyPipelineLayout            = PFN_vkDestroyPipelineLayout( vkGetDeviceProcAddr( device, "vkDestroyPipelineLayout" ) );
      vkCreateSampler                    = PFN_vkCreateSampler( vkGetDeviceProcAddr( device, "vkCreateSampler" ) );
      vkDestroySampler                   = PFN_vkDestroySampler( vkGetDeviceProcAddr( device, "vkDestroySampler" ) );
      vkCreateDescriptorSetLayout        = PFN_vkCreateDescriptorSetLayout( vkGetDeviceProcAddr( device, "vkCreateDescriptorSetLayout" ) );
      vkDestroyDescriptorSetLayout       = PFN_vkDestroyDescriptorSetLayout( vkGetDeviceProcAddr( device, "vkDestroyDescriptorSetLayout" ) );
      vkCreateDescriptorPool             = PFN_vkCreateDescriptorPool( vkGetDeviceProcAddr( device, "vkCreateDescriptorPool" ) );
      vkDestroyDescriptorPool            = PFN_vkDestroyDescriptorPool( vkGetDeviceProcAddr( device, "vkDestroyDescriptorPool" ) );
      vkResetDescriptorPool              = PFN_vkResetDescriptorPool( vkGetDeviceProcAddr( device, "vkResetDescriptorPool" ) );
      vkAllocateDescriptorSets           = PFN_vkAllocateDescriptorSets( vkGetDeviceProcAddr( device, "vkAllocateDescriptorSets" ) );
      vkFreeDescriptorSets               = PFN_vkFreeDescriptorSets( vkGetDeviceProcAddr( device, "vkFreeDescriptorSets" ) );
      vkUpdateDescriptorSets             = PFN_vkUpdateDescriptorSets( vkGetDeviceProcAddr( device, "vkUpdateDescriptorSets" ) );
      vkCmdBindPipeline                  = PFN_vkCmdBindPipeline( vkGetDeviceProcAddr( device, "vkCmdBindPipeline" ) );
      vkCmdBindDescriptorSets            = PFN_vkCmdBindDescriptorSets( vkGetDeviceProcAddr( device, "vkCmdBindDescriptorSets" ) );
      vkCmdClearColorImage               = PFN_vkCmdClearColorImage( vkGetDeviceProcAddr( device, "vkCmdClearColorImage" ) );
      vkCmdDispatch                      = PFN_vkCmdDispatch( vkGetDeviceProcAddr( device, "vkCmdDispatch" ) );
      vkCmdDispatchIndirect              = PFN_vkCmdDispatchIndirect( vkGetDeviceProcAddr( device, "vkCmdDispatchIndirect" ) );
      vkCmdSetEvent                      = PFN_vkCmdSetEvent( vkGetDeviceProcAddr( device, "vkCmdSetEvent" ) );
      vkCmdResetEvent                    = PFN_vkCmdResetEvent( vkGetDeviceProcAddr( device, "vkCmdResetEvent" ) );
      vkCmdWaitEvents                    = PFN_vkCmdWaitEvents( vkGetDeviceProcAddr( device, "vkCmdWaitEvents" ) );
      vkCmdPushConstants                 = PFN_vkCmdPushConstants( vkGetDeviceProcAddr( device, "vkCmdPushConstants" ) );
      vkCreateGraphicsPipelines          = PFN_vkCreateGraphicsPipelines( vkGetDeviceProcAddr( device, "vkCreateGraphicsPipelines" ) );
      vkCreateFramebuffer                = PFN_vkCreateFramebuffer( vkGetDeviceProcAddr( device, "vkCreateFramebuffer" ) );
      vkDestroyFramebuffer               = PFN_vkDestroyFramebuffer( vkGetDeviceProcAddr( device, "vkDestroyFramebuffer" ) );
      vkCreateRenderPass                 = PFN_vkCreateRenderPass( vkGetDeviceProcAddr( device, "vkCreateRenderPass" ) );
      vkDestroyRenderPass                = PFN_vkDestroyRenderPass( vkGetDeviceProcAddr( device, "vkDestroyRenderPass" ) );
      vkGetRenderAreaGranularity         = PFN_vkGetRenderAreaGranularity( vkGetDeviceProcAddr( device, "vkGetRenderAreaGranularity" ) );
      vkCmdSetViewport                   = PFN_vkCmdSetViewport( vkGetDeviceProcAddr( device, "vkCmdSetViewport" ) );
      vkCmdSetScissor                    = PFN_vkCmdSetScissor( vkGetDeviceProcAddr( device, "vkCmdSetScissor" ) );
      vkCmdSetLineWidth                  = PFN_vkCmdSetLineWidth( vkGetDeviceProcAddr( device, "vkCmdSetLineWidth" ) );
      vkCmdSetDepthBias                  = PFN_vkCmdSetDepthBias( vkGetDeviceProcAddr( device, "vkCmdSetDepthBias" ) );
      vkCmdSetBlendConstants             = PFN_vkCmdSetBlendConstants( vkGetDeviceProcAddr( device, "vkCmdSetBlendConstants" ) );
      vkCmdSetDepthBounds                = PFN_vkCmdSetDepthBounds( vkGetDeviceProcAddr( device, "vkCmdSetDepthBounds" ) );
      vkCmdSetStencilCompareMask         = PFN_vkCmdSetStencilCompareMask( vkGetDeviceProcAddr( device, "vkCmdSetStencilCompareMask" ) );
      vkCmdSetStencilWriteMask           = PFN_vkCmdSetStencilWriteMask( vkGetDeviceProcAddr( device, "vkCmdSetStencilWriteMask" ) );
      vkCmdSetStencilReference           = PFN_vkCmdSetStencilReference( vkGetDeviceProcAddr( device, "vkCmdSetStencilReference" ) );
      vkCmdBindIndexBuffer               = PFN_vkCmdBindIndexBuffer( vkGetDeviceProcAddr( device, "vkCmdBindIndexBuffer" ) );
      vkCmdBindVertexBuffers             = PFN_vkCmdBindVertexBuffers( vkGetDeviceProcAddr( device, "vkCmdBindVertexBuffers" ) );
      vkCmdDraw                          = PFN_vkCmdDraw( vkGetDeviceProcAddr( device, "vkCmdDraw" ) );
      vkCmdDrawIndexed                   = PFN_vkCmdDrawIndexed( vkGetDeviceProcAddr( device, "vkCmdDrawIndexed" ) );
      vkCmdDrawIndirect                  = PFN_vkCmdDrawIndirect( vkGetDeviceProcAddr( device, "vkCmdDrawIndirect" ) );
      vkCmdDrawIndexedIndirect           = PFN_vkCmdDrawIndexedIndirect( vkGetDeviceProcAddr( device, "vkCmdDrawIndexedIndirect" ) );
      vkCmdBlitImage                     = PFN_vkCmdBlitImage( vkGetDeviceProcAddr( device, "vkCmdBlitImage" ) );
      vkCmdClearDepthStencilImage        = PFN_vkCmdClearDepthStencilImage( vkGetDeviceProcAddr( device, "vkCmdClearDepthStencilImage" ) );
      vkCmdClearAttachments              = PFN_vkCmdClearAttachments( vkGetDeviceProcAddr( device, "vkCmdClearAttachments" ) );
      vkCmdResolveImage                  = PFN_vkCmdResolveImage( vkGetDeviceProcAddr( device, "vkCmdResolveImage" ) );
      vkCmdBeginRenderPass               = PFN_vkCmdBeginRenderPass( vkGetDeviceProcAddr( device, "vkCmdBeginRenderPass" ) );
      vkCmdNextSubpass                   = PFN_vkCmdNextSubpass( vkGetDeviceProcAddr( device, "vkCmdNextSubpass" ) );
      vkCmdEndRenderPass                 = PFN_vkCmdEndRenderPass( vkGetDeviceProcAddr( device, "vkCmdEndRenderPass" ) );

      //=== VK_VERSION_1_1 ===
      vkBindBufferMemory2                 = PFN_vkBindBufferMemory2( vkGetDeviceProcAddr( device, "vkBindBufferMemory2" ) );
      vkBindImageMemory2                  = PFN_vkBindImageMemory2( vkGetDeviceProcAddr( device, "vkBindImageMemory2" ) );
      vkGetDeviceGroupPeerMemoryFeatures  = PFN_vkGetDeviceGroupPeerMemoryFeatures( vkGetDeviceProcAddr( device, "vkGetDeviceGroupPeerMemoryFeatures" ) );
      vkCmdSetDeviceMask                  = PFN_vkCmdSetDeviceMask( vkGetDeviceProcAddr( device, "vkCmdSetDeviceMask" ) );
      vkGetImageMemoryRequirements2       = PFN_vkGetImageMemoryRequirements2( vkGetDeviceProcAddr( device, "vkGetImageMemoryRequirements2" ) );
      vkGetBufferMemoryRequirements2      = PFN_vkGetBufferMemoryRequirements2( vkGetDeviceProcAddr( device, "vkGetBufferMemoryRequirements2" ) );
      vkGetImageSparseMemoryRequirements2 = PFN_vkGetImageSparseMemoryRequirements2( vkGetDeviceProcAddr( device, "vkGetImageSparseMemoryRequirements2" ) );
      vkTrimCommandPool                   = PFN_vkTrimCommandPool( vkGetDeviceProcAddr( device, "vkTrimCommandPool" ) );
      vkGetDeviceQueue2                   = PFN_vkGetDeviceQueue2( vkGetDeviceProcAddr( device, "vkGetDeviceQueue2" ) );
      vkCmdDispatchBase                   = PFN_vkCmdDispatchBase( vkGetDeviceProcAddr( device, "vkCmdDispatchBase" ) );
      vkCreateDescriptorUpdateTemplate    = PFN_vkCreateDescriptorUpdateTemplate( vkGetDeviceProcAddr( device, "vkCreateDescriptorUpdateTemplate" ) );
      vkDestroyDescriptorUpdateTemplate   = PFN_vkDestroyDescriptorUpdateTemplate( vkGetDeviceProcAddr( device, "vkDestroyDescriptorUpdateTemplate" ) );
      vkUpdateDescriptorSetWithTemplate   = PFN_vkUpdateDescriptorSetWithTemplate( vkGetDeviceProcAddr( device, "vkUpdateDescriptorSetWithTemplate" ) );
      vkGetDescriptorSetLayoutSupport     = PFN_vkGetDescriptorSetLayoutSupport( vkGetDeviceProcAddr( device, "vkGetDescriptorSetLayoutSupport" ) );
      vkCreateSamplerYcbcrConversion      = PFN_vkCreateSamplerYcbcrConversion( vkGetDeviceProcAddr( device, "vkCreateSamplerYcbcrConversion" ) );
      vkDestroySamplerYcbcrConversion     = PFN_vkDestroySamplerYcbcrConversion( vkGetDeviceProcAddr( device, "vkDestroySamplerYcbcrConversion" ) );

      //=== VK_VERSION_1_2 ===
      vkResetQueryPool                = PFN_vkResetQueryPool( vkGetDeviceProcAddr( device, "vkResetQueryPool" ) );
      vkGetSemaphoreCounterValue      = PFN_vkGetSemaphoreCounterValue( vkGetDeviceProcAddr( device, "vkGetSemaphoreCounterValue" ) );
      vkWaitSemaphores                = PFN_vkWaitSemaphores( vkGetDeviceProcAddr( device, "vkWaitSemaphores" ) );
      vkSignalSemaphore               = PFN_vkSignalSemaphore( vkGetDeviceProcAddr( device, "vkSignalSemaphore" ) );
      vkGetBufferDeviceAddress        = PFN_vkGetBufferDeviceAddress( vkGetDeviceProcAddr( device, "vkGetBufferDeviceAddress" ) );
      vkGetBufferOpaqueCaptureAddress = PFN_vkGetBufferOpaqueCaptureAddress( vkGetDeviceProcAddr( device, "vkGetBufferOpaqueCaptureAddress" ) );
      vkGetDeviceMemoryOpaqueCaptureAddress =
        PFN_vkGetDeviceMemoryOpaqueCaptureAddress( vkGetDeviceProcAddr( device, "vkGetDeviceMemoryOpaqueCaptureAddress" ) );
      vkCmdDrawIndirectCount        = PFN_vkCmdDrawIndirectCount( vkGetDeviceProcAddr( device, "vkCmdDrawIndirectCount" ) );
      vkCmdDrawIndexedIndirectCount = PFN_vkCmdDrawIndexedIndirectCount( vkGetDeviceProcAddr( device, "vkCmdDrawIndexedIndirectCount" ) );
      vkCreateRenderPass2           = PFN_vkCreateRenderPass2( vkGetDeviceProcAddr( device, "vkCreateRenderPass2" ) );
      vkCmdBeginRenderPass2         = PFN_vkCmdBeginRenderPass2( vkGetDeviceProcAddr( device, "vkCmdBeginRenderPass2" ) );
      vkCmdNextSubpass2             = PFN_vkCmdNextSubpass2( vkGetDeviceProcAddr( device, "vkCmdNextSubpass2" ) );
      vkCmdEndRenderPass2           = PFN_vkCmdEndRenderPass2( vkGetDeviceProcAddr( device, "vkCmdEndRenderPass2" ) );

      //=== VK_VERSION_1_3 ===
      vkCreatePrivateDataSlot             = PFN_vkCreatePrivateDataSlot( vkGetDeviceProcAddr( device, "vkCreatePrivateDataSlot" ) );
      vkDestroyPrivateDataSlot            = PFN_vkDestroyPrivateDataSlot( vkGetDeviceProcAddr( device, "vkDestroyPrivateDataSlot" ) );
      vkSetPrivateData                    = PFN_vkSetPrivateData( vkGetDeviceProcAddr( device, "vkSetPrivateData" ) );
      vkGetPrivateData                    = PFN_vkGetPrivateData( vkGetDeviceProcAddr( device, "vkGetPrivateData" ) );
      vkCmdPipelineBarrier2               = PFN_vkCmdPipelineBarrier2( vkGetDeviceProcAddr( device, "vkCmdPipelineBarrier2" ) );
      vkCmdWriteTimestamp2                = PFN_vkCmdWriteTimestamp2( vkGetDeviceProcAddr( device, "vkCmdWriteTimestamp2" ) );
      vkQueueSubmit2                      = PFN_vkQueueSubmit2( vkGetDeviceProcAddr( device, "vkQueueSubmit2" ) );
      vkCmdCopyBuffer2                    = PFN_vkCmdCopyBuffer2( vkGetDeviceProcAddr( device, "vkCmdCopyBuffer2" ) );
      vkCmdCopyImage2                     = PFN_vkCmdCopyImage2( vkGetDeviceProcAddr( device, "vkCmdCopyImage2" ) );
      vkCmdCopyBufferToImage2             = PFN_vkCmdCopyBufferToImage2( vkGetDeviceProcAddr( device, "vkCmdCopyBufferToImage2" ) );
      vkCmdCopyImageToBuffer2             = PFN_vkCmdCopyImageToBuffer2( vkGetDeviceProcAddr( device, "vkCmdCopyImageToBuffer2" ) );
      vkGetDeviceBufferMemoryRequirements = PFN_vkGetDeviceBufferMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetDeviceBufferMemoryRequirements" ) );
      vkGetDeviceImageMemoryRequirements  = PFN_vkGetDeviceImageMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetDeviceImageMemoryRequirements" ) );
      vkGetDeviceImageSparseMemoryRequirements =
        PFN_vkGetDeviceImageSparseMemoryRequirements( vkGetDeviceProcAddr( device, "vkGetDeviceImageSparseMemoryRequirements" ) );
      vkCmdSetEvent2                  = PFN_vkCmdSetEvent2( vkGetDeviceProcAddr( device, "vkCmdSetEvent2" ) );
      vkCmdResetEvent2                = PFN_vkCmdResetEvent2( vkGetDeviceProcAddr( device, "vkCmdResetEvent2" ) );
      vkCmdWaitEvents2                = PFN_vkCmdWaitEvents2( vkGetDeviceProcAddr( device, "vkCmdWaitEvents2" ) );
      vkCmdBlitImage2                 = PFN_vkCmdBlitImage2( vkGetDeviceProcAddr( device, "vkCmdBlitImage2" ) );
      vkCmdResolveImage2              = PFN_vkCmdResolveImage2( vkGetDeviceProcAddr( device, "vkCmdResolveImage2" ) );
      vkCmdBeginRendering             = PFN_vkCmdBeginRendering( vkGetDeviceProcAddr( device, "vkCmdBeginRendering" ) );
      vkCmdEndRendering               = PFN_vkCmdEndRendering( vkGetDeviceProcAddr( device, "vkCmdEndRendering" ) );
      vkCmdSetCullMode                = PFN_vkCmdSetCullMode( vkGetDeviceProcAddr( device, "vkCmdSetCullMode" ) );
      vkCmdSetFrontFace               = PFN_vkCmdSetFrontFace( vkGetDeviceProcAddr( device, "vkCmdSetFrontFace" ) );
      vkCmdSetPrimitiveTopology       = PFN_vkCmdSetPrimitiveTopology( vkGetDeviceProcAddr( device, "vkCmdSetPrimitiveTopology" ) );
      vkCmdSetViewportWithCount       = PFN_vkCmdSetViewportWithCount( vkGetDeviceProcAddr( device, "vkCmdSetViewportWithCount" ) );
      vkCmdSetScissorWithCount        = PFN_vkCmdSetScissorWithCount( vkGetDeviceProcAddr( device, "vkCmdSetScissorWithCount" ) );
      vkCmdBindVertexBuffers2         = PFN_vkCmdBindVertexBuffers2( vkGetDeviceProcAddr( device, "vkCmdBindVertexBuffers2" ) );
      vkCmdSetDepthTestEnable         = PFN_vkCmdSetDepthTestEnable( vkGetDeviceProcAddr( device, "vkCmdSetDepthTestEnable" ) );
      vkCmdSetDepthWriteEnable        = PFN_vkCmdSetDepthWriteEnable( vkGetDeviceProcAddr( device, "vkCmdSetDepthWriteEnable" ) );
      vkCmdSetDepthCompareOp          = PFN_vkCmdSetDepthCompareOp( vkGetDeviceProcAddr( device, "vkCmdSetDepthCompareOp" ) );
      vkCmdSetDepthBoundsTestEnable   = PFN_vkCmdSetDepthBoundsTestEnable( vkGetDeviceProcAddr( device, "vkCmdSetDepthBoundsTestEnable" ) );
      vkCmdSetStencilTestEnable       = PFN_vkCmdSetStencilTestEnable( vkGetDeviceProcAddr( device, "vkCmdSetStencilTestEnable" ) );
      vkCmdSetStencilOp               = PFN_vkCmdSetStencilOp( vkGetDeviceProcAddr( device, "vkCmdSetStencilOp" ) );
      vkCmdSetRasterizerDiscardEnable = PFN_vkCmdSetRasterizerDiscardEnable( vkGetDeviceProcAddr( device, "vkCmdSetRasterizerDiscardEnable" ) );
      vkCmdSetDepthBiasEnable         = PFN_vkCmdSetDepthBiasEnable( vkGetDeviceProcAddr( device, "vkCmdSetDepthBiasEnable" ) );
      vkCmdSetPrimitiveRestartEnable  = PFN_vkCmdSetPrimitiveRestartEnable( vkGetDeviceProcAddr( device, "vkCmdSetPrimitiveRestartEnable" ) );

      //=== VK_VERSION_1_4 ===
      vkMapMemory2                         = PFN_vkMapMemory2( vkGetDeviceProcAddr( device, "vkMapMemory2" ) );
      vkUnmapMemory2                       = PFN_vkUnmapMemory2( vkGetDeviceProcAddr( device, "vkUnmapMemory2" ) );
      vkGetDeviceImageSubresourceLayout    = PFN_vkGetDeviceImageSubresourceLayout( vkGetDeviceProcAddr( device, "vkGetDeviceImageSubresourceLayout" ) );
      vkGetImageSubresourceLayout2         = PFN_vkGetImageSubresourceLayout2( vkGetDeviceProcAddr( device, "vkGetImageSubresourceLayout2" ) );
      vkCopyMemoryToImage                  = PFN_vkCopyMemoryToImage( vkGetDeviceProcAddr( device, "vkCopyMemoryToImage" ) );
      vkCopyImageToMemory                  = PFN_vkCopyImageToMemory( vkGetDeviceProcAddr( device, "vkCopyImageToMemory" ) );
      vkCopyImageToImage                   = PFN_vkCopyImageToImage( vkGetDeviceProcAddr( device, "vkCopyImageToImage" ) );
      vkTransitionImageLayout              = PFN_vkTransitionImageLayout( vkGetDeviceProcAddr( device, "vkTransitionImageLayout" ) );
      vkCmdPushDescriptorSet               = PFN_vkCmdPushDescriptorSet( vkGetDeviceProcAddr( device, "vkCmdPushDescriptorSet" ) );
      vkCmdPushDescriptorSetWithTemplate   = PFN_vkCmdPushDescriptorSetWithTemplate( vkGetDeviceProcAddr( device, "vkCmdPushDescriptorSetWithTemplate" ) );
      vkCmdBindDescriptorSets2             = PFN_vkCmdBindDescriptorSets2( vkGetDeviceProcAddr( device, "vkCmdBindDescriptorSets2" ) );
      vkCmdPushConstants2                  = PFN_vkCmdPushConstants2( vkGetDeviceProcAddr( device, "vkCmdPushConstants2" ) );
      vkCmdPushDescriptorSet2              = PFN_vkCmdPushDescriptorSet2( vkGetDeviceProcAddr( device, "vkCmdPushDescriptorSet2" ) );
      vkCmdPushDescriptorSetWithTemplate2  = PFN_vkCmdPushDescriptorSetWithTemplate2( vkGetDeviceProcAddr( device, "vkCmdPushDescriptorSetWithTemplate2" ) );
      vkCmdSetLineStipple                  = PFN_vkCmdSetLineStipple( vkGetDeviceProcAddr( device, "vkCmdSetLineStipple" ) );
      vkCmdBindIndexBuffer2                = PFN_vkCmdBindIndexBuffer2( vkGetDeviceProcAddr( device, "vkCmdBindIndexBuffer2" ) );
      vkGetRenderingAreaGranularity        = PFN_vkGetRenderingAreaGranularity( vkGetDeviceProcAddr( device, "vkGetRenderingAreaGranularity" ) );
      vkCmdSetRenderingAttachmentLocations = PFN_vkCmdSetRenderingAttachmentLocations( vkGetDeviceProcAddr( device, "vkCmdSetRenderingAttachmentLocations" ) );
      vkCmdSetRenderingInputAttachmentIndices =
        PFN_vkCmdSetRenderingInputAttachmentIndices( vkGetDeviceProcAddr( device, "vkCmdSetRenderingInputAttachmentIndices" ) );

      //=== VK_KHR_swapchain ===
      vkCreateSwapchainKHR    = PFN_vkCreateSwapchainKHR( vkGetDeviceProcAddr( device, "vkCreateSwapchainKHR" ) );
      vkDestroySwapchainKHR   = PFN_vkDestroySwapchainKHR( vkGetDeviceProcAddr( device, "vkDestroySwapchainKHR" ) );
      vkGetSwapchainImagesKHR = PFN_vkGetSwapchainImagesKHR( vkGetDeviceProcAddr( device, "vkGetSwapchainImagesKHR" ) );
      vkAcquireNextImageKHR   = PFN_vkAcquireNextImageKHR( vkGetDeviceProcAddr( device, "vkAcquireNextImageKHR" ) );
      vkQueuePresentKHR       = PFN_vkQueuePresentKHR( vkGetDeviceProcAddr( device, "vkQueuePresentKHR" ) );
      vkGetDeviceGroupPresentCapabilitiesKHR =
        PFN_vkGetDeviceGroupPresentCapabilitiesKHR( vkGetDeviceProcAddr( device, "vkGetDeviceGroupPresentCapabilitiesKHR" ) );
      vkGetDeviceGroupSurfacePresentModesKHR =
        PFN_vkGetDeviceGroupSurfacePresentModesKHR( vkGetDeviceProcAddr( device, "vkGetDeviceGroupSurfacePresentModesKHR" ) );
      vkAcquireNextImage2KHR = PFN_vkAcquireNextImage2KHR( vkGetDeviceProcAddr( device, "vkAcquireNextImage2KHR" ) );

      //=== VK_KHR_display_swapchain ===
      vkCreateSharedSwapchainsKHR = PFN_vkCreateSharedSwapchainsKHR( vkGetDeviceProcAddr( device, "vkCreateSharedSwapchainsKHR" ) );

      //=== VK_EXT_debug_marker ===
      vkDebugMarkerSetObjectTagEXT  = PFN_vkDebugMarkerSetObjectTagEXT( vkGetDeviceProcAddr( device, "vkDebugMarkerSetObjectTagEXT" ) );
      vkDebugMarkerSetObjectNameEXT = PFN_vkDebugMarkerSetObjectNameEXT( vkGetDeviceProcAddr( device, "vkDebugMarkerSetObjectNameEXT" ) );
      vkCmdDebugMarkerBeginEXT      = PFN_vkCmdDebugMarkerBeginEXT( vkGetDeviceProcAddr( device, "vkCmdDebugMarkerBeginEXT" ) );
      vkCmdDebugMarkerEndEXT        = PFN_vkCmdDebugMarkerEndEXT( vkGetDeviceProcAddr( device, "vkCmdDebugMarkerEndEXT" ) );
      vkCmdDebugMarkerInsertEXT     = PFN_vkCmdDebugMarkerInsertEXT( vkGetDeviceProcAddr( device, "vkCmdDebugMarkerInsertEXT" ) );

      //=== VK_KHR_video_queue ===
      vkCreateVideoSessionKHR  = PFN_vkCreateVideoSessionKHR( vkGetDeviceProcAddr( device, "vkCreateVideoSessionKHR" ) );
      vkDestroyVideoSessionKHR = PFN_vkDestroyVideoSessionKHR( vkGetDeviceProcAddr( device, "vkDestroyVideoSessionKHR" ) );
      vkGetVideoSessionMemoryRequirementsKHR =
        PFN_vkGetVideoSessionMemoryRequirementsKHR( vkGetDeviceProcAddr( device, "vkGetVideoSessionMemoryRequirementsKHR" ) );
      vkBindVideoSessionMemoryKHR        = PFN_vkBindVideoSessionMemoryKHR( vkGetDeviceProcAddr( device, "vkBindVideoSessionMemoryKHR" ) );
      vkCreateVideoSessionParametersKHR  = PFN_vkCreateVideoSessionParametersKHR( vkGetDeviceProcAddr( device, "vkCreateVideoSessionParametersKHR" ) );
      vkUpdateVideoSessionParametersKHR  = PFN_vkUpdateVideoSessionParametersKHR( vkGetDeviceProcAddr( device, "vkUpdateVideoSessionParametersKHR" ) );
      vkDestroyVideoSessionParametersKHR = PFN_vkDestroyVideoSessionParametersKHR( vkGetDeviceProcAddr( device, "vkDestroyVideoSessionParametersKHR" ) );
      vkCmdBeginVideoCodingKHR           = PFN_vkCmdBeginVideoCodingKHR( vkGetDeviceProcAddr( device, "vkCmdBeginVideoCodingKHR" ) );
      vkCmdEndVideoCodingKHR             = PFN_vkCmdEndVideoCodingKHR( vkGetDeviceProcAddr( device, "vkCmdEndVideoCodingKHR" ) );
      vkCmdControlVideoCodingKHR         = PFN_vkCmdControlVideoCodingKHR( vkGetDeviceProcAddr( device, "vkCmdControlVideoCodingKHR" ) );

      //=== VK_KHR_video_decode_queue ===
      vkCmdDecodeVideoKHR = PFN_vkCmdDecodeVideoKHR( vkGetDeviceProcAddr( device, "vkCmdDecodeVideoKHR" ) );

      //=== VK_EXT_transform_feedback ===
      vkCmdBindTransformFeedbackBuffersEXT = PFN_vkCmdBindTransformFeedbackBuffersEXT( vkGetDeviceProcAddr( device, "vkCmdBindTransformFeedbackBuffersEXT" ) );
      vkCmdBeginTransformFeedbackEXT       = PFN_vkCmdBeginTransformFeedbackEXT( vkGetDeviceProcAddr( device, "vkCmdBeginTransformFeedbackEXT" ) );
      vkCmdEndTransformFeedbackEXT         = PFN_vkCmdEndTransformFeedbackEXT( vkGetDeviceProcAddr( device, "vkCmdEndTransformFeedbackEXT" ) );
      vkCmdBeginQueryIndexedEXT            = PFN_vkCmdBeginQueryIndexedEXT( vkGetDeviceProcAddr( device, "vkCmdBeginQueryIndexedEXT" ) );
      vkCmdEndQueryIndexedEXT              = PFN_vkCmdEndQueryIndexedEXT( vkGetDeviceProcAddr( device, "vkCmdEndQueryIndexedEXT" ) );
      vkCmdDrawIndirectByteCountEXT        = PFN_vkCmdDrawIndirectByteCountEXT( vkGetDeviceProcAddr( device, "vkCmdDrawIndirectByteCountEXT" ) );

      //=== VK_NVX_binary_import ===
      vkCreateCuModuleNVX    = PFN_vkCreateCuModuleNVX( vkGetDeviceProcAddr( device, "vkCreateCuModuleNVX" ) );
      vkCreateCuFunctionNVX  = PFN_vkCreateCuFunctionNVX( vkGetDeviceProcAddr( device, "vkCreateCuFunctionNVX" ) );
      vkDestroyCuModuleNVX   = PFN_vkDestroyCuModuleNVX( vkGetDeviceProcAddr( device, "vkDestroyCuModuleNVX" ) );
      vkDestroyCuFunctionNVX = PFN_vkDestroyCuFunctionNVX( vkGetDeviceProcAddr( device, "vkDestroyCuFunctionNVX" ) );
      vkCmdCuLaunchKernelNVX = PFN_vkCmdCuLaunchKernelNVX( vkGetDeviceProcAddr( device, "vkCmdCuLaunchKernelNVX" ) );

      //=== VK_NVX_image_view_handle ===
      vkGetImageViewHandleNVX   = PFN_vkGetImageViewHandleNVX( vkGetDeviceProcAddr( device, "vkGetImageViewHandleNVX" ) );
      vkGetImageViewHandle64NVX = PFN_vkGetImageViewHandle64NVX( vkGetDeviceProcAddr( device, "vkGetImageViewHandle64NVX" ) );
      vkGetImageViewAddressNVX  = PFN_vkGetImageViewAddressNVX( vkGetDeviceProcAddr( device, "vkGetImageViewAddressNVX" ) );
      vkGetDeviceCombinedImageSamplerIndexNVX =
        PFN_vkGetDeviceCombinedImageSamplerIndexNVX( vkGetDeviceProcAddr( device, "vkGetDeviceCombinedImageSamplerIndexNVX" ) );

      //=== VK_AMD_draw_indirect_count ===
      vkCmdDrawIndirectCountAMD = PFN_vkCmdDrawIndirectCountAMD( vkGetDeviceProcAddr( device, "vkCmdDrawIndirectCountAMD" ) );
      if ( !vkCmdDrawIndirectCount )
        vkCmdDrawIndirectCount = vkCmdDrawIndirectCountAMD;
      vkCmdDrawIndexedIndirectCountAMD = PFN_vkCmdDrawIndexedIndirectCountAMD( vkGetDeviceProcAddr( device, "vkCmdDrawIndexedIndirectCountAMD" ) );
      if ( !vkCmdDrawIndexedIndirectCount )
        vkCmdDrawIndexedIndirectCount = vkCmdDrawIndexedIndirectCountAMD;

      //=== VK_AMD_shader_info ===
      vkGetShaderInfoAMD = PFN_vkGetShaderInfoAMD( vkGetDeviceProcAddr( device, "vkGetShaderInfoAMD" ) );

      //=== VK_KHR_dynamic_rendering ===
      vkCmdBeginRenderingKHR = PFN_vkCmdBeginRenderingKHR( vkGetDeviceProcAddr( device, "vkCmdBeginRenderingKHR" ) );
      if ( !vkCmdBeginRendering )
        vkCmdBeginRendering = vkCmdBeginRenderingKHR;
      vkCmdEndRenderingKHR = PFN_vkCmdEndRenderingKHR( vkGetDeviceProcAddr( device, "vkCmdEndRenderingKHR" ) );
      if ( !vkCmdEndRendering )
        vkCmdEndRendering = vkCmdEndRenderingKHR;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_NV_external_memory_win32 ===
      vkGetMemoryWin32HandleNV = PFN_vkGetMemoryWin32HandleNV( vkGetDeviceProcAddr( device, "vkGetMemoryWin32HandleNV" ) );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_KHR_device_group ===
      vkGetDeviceGroupPeerMemoryFeaturesKHR =
        PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR( vkGetDeviceProcAddr( device, "vkGetDeviceGroupPeerMemoryFeaturesKHR" ) );
      if ( !vkGetDeviceGroupPeerMemoryFeatures )
        vkGetDeviceGroupPeerMemoryFeatures = vkGetDeviceGroupPeerMemoryFeaturesKHR;
      vkCmdSetDeviceMaskKHR = PFN_vkCmdSetDeviceMaskKHR( vkGetDeviceProcAddr( device, "vkCmdSetDeviceMaskKHR" ) );
      if ( !vkCmdSetDeviceMask )
        vkCmdSetDeviceMask = vkCmdSetDeviceMaskKHR;
      vkCmdDispatchBaseKHR = PFN_vkCmdDispatchBaseKHR( vkGetDeviceProcAddr( device, "vkCmdDispatchBaseKHR" ) );
      if ( !vkCmdDispatchBase )
        vkCmdDispatchBase = vkCmdDispatchBaseKHR;

      //=== VK_KHR_maintenance1 ===
      vkTrimCommandPoolKHR = PFN_vkTrimCommandPoolKHR( vkGetDeviceProcAddr( device, "vkTrimCommandPoolKHR" ) );
      if ( !vkTrimCommandPool )
        vkTrimCommandPool = vkTrimCommandPoolKHR;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_KHR_external_memory_win32 ===
      vkGetMemoryWin32HandleKHR           = PFN_vkGetMemoryWin32HandleKHR( vkGetDeviceProcAddr( device, "vkGetMemoryWin32HandleKHR" ) );
      vkGetMemoryWin32HandlePropertiesKHR = PFN_vkGetMemoryWin32HandlePropertiesKHR( vkGetDeviceProcAddr( device, "vkGetMemoryWin32HandlePropertiesKHR" ) );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_KHR_external_memory_fd ===
      vkGetMemoryFdKHR           = PFN_vkGetMemoryFdKHR( vkGetDeviceProcAddr( device, "vkGetMemoryFdKHR" ) );
      vkGetMemoryFdPropertiesKHR = PFN_vkGetMemoryFdPropertiesKHR( vkGetDeviceProcAddr( device, "vkGetMemoryFdPropertiesKHR" ) );

#if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_KHR_external_semaphore_win32 ===
      vkImportSemaphoreWin32HandleKHR = PFN_vkImportSemaphoreWin32HandleKHR( vkGetDeviceProcAddr( device, "vkImportSemaphoreWin32HandleKHR" ) );
      vkGetSemaphoreWin32HandleKHR    = PFN_vkGetSemaphoreWin32HandleKHR( vkGetDeviceProcAddr( device, "vkGetSemaphoreWin32HandleKHR" ) );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_KHR_external_semaphore_fd ===
      vkImportSemaphoreFdKHR = PFN_vkImportSemaphoreFdKHR( vkGetDeviceProcAddr( device, "vkImportSemaphoreFdKHR" ) );
      vkGetSemaphoreFdKHR    = PFN_vkGetSemaphoreFdKHR( vkGetDeviceProcAddr( device, "vkGetSemaphoreFdKHR" ) );

      //=== VK_KHR_push_descriptor ===
      vkCmdPushDescriptorSetKHR = PFN_vkCmdPushDescriptorSetKHR( vkGetDeviceProcAddr( device, "vkCmdPushDescriptorSetKHR" ) );
      if ( !vkCmdPushDescriptorSet )
        vkCmdPushDescriptorSet = vkCmdPushDescriptorSetKHR;
      vkCmdPushDescriptorSetWithTemplateKHR =
        PFN_vkCmdPushDescriptorSetWithTemplateKHR( vkGetDeviceProcAddr( device, "vkCmdPushDescriptorSetWithTemplateKHR" ) );
      if ( !vkCmdPushDescriptorSetWithTemplate )
        vkCmdPushDescriptorSetWithTemplate = vkCmdPushDescriptorSetWithTemplateKHR;

      //=== VK_EXT_conditional_rendering ===
      vkCmdBeginConditionalRenderingEXT = PFN_vkCmdBeginConditionalRenderingEXT( vkGetDeviceProcAddr( device, "vkCmdBeginConditionalRenderingEXT" ) );
      vkCmdEndConditionalRenderingEXT   = PFN_vkCmdEndConditionalRenderingEXT( vkGetDeviceProcAddr( device, "vkCmdEndConditionalRenderingEXT" ) );

      //=== VK_KHR_descriptor_update_template ===
      vkCreateDescriptorUpdateTemplateKHR = PFN_vkCreateDescriptorUpdateTemplateKHR( vkGetDeviceProcAddr( device, "vkCreateDescriptorUpdateTemplateKHR" ) );
      if ( !vkCreateDescriptorUpdateTemplate )
        vkCreateDescriptorUpdateTemplate = vkCreateDescriptorUpdateTemplateKHR;
      vkDestroyDescriptorUpdateTemplateKHR = PFN_vkDestroyDescriptorUpdateTemplateKHR( vkGetDeviceProcAddr( device, "vkDestroyDescriptorUpdateTemplateKHR" ) );
      if ( !vkDestroyDescriptorUpdateTemplate )
        vkDestroyDescriptorUpdateTemplate = vkDestroyDescriptorUpdateTemplateKHR;
      vkUpdateDescriptorSetWithTemplateKHR = PFN_vkUpdateDescriptorSetWithTemplateKHR( vkGetDeviceProcAddr( device, "vkUpdateDescriptorSetWithTemplateKHR" ) );
      if ( !vkUpdateDescriptorSetWithTemplate )
        vkUpdateDescriptorSetWithTemplate = vkUpdateDescriptorSetWithTemplateKHR;

      //=== VK_NV_clip_space_w_scaling ===
      vkCmdSetViewportWScalingNV = PFN_vkCmdSetViewportWScalingNV( vkGetDeviceProcAddr( device, "vkCmdSetViewportWScalingNV" ) );

      //=== VK_EXT_display_control ===
      vkDisplayPowerControlEXT  = PFN_vkDisplayPowerControlEXT( vkGetDeviceProcAddr( device, "vkDisplayPowerControlEXT" ) );
      vkRegisterDeviceEventEXT  = PFN_vkRegisterDeviceEventEXT( vkGetDeviceProcAddr( device, "vkRegisterDeviceEventEXT" ) );
      vkRegisterDisplayEventEXT = PFN_vkRegisterDisplayEventEXT( vkGetDeviceProcAddr( device, "vkRegisterDisplayEventEXT" ) );
      vkGetSwapchainCounterEXT  = PFN_vkGetSwapchainCounterEXT( vkGetDeviceProcAddr( device, "vkGetSwapchainCounterEXT" ) );

      //=== VK_GOOGLE_display_timing ===
      vkGetRefreshCycleDurationGOOGLE   = PFN_vkGetRefreshCycleDurationGOOGLE( vkGetDeviceProcAddr( device, "vkGetRefreshCycleDurationGOOGLE" ) );
      vkGetPastPresentationTimingGOOGLE = PFN_vkGetPastPresentationTimingGOOGLE( vkGetDeviceProcAddr( device, "vkGetPastPresentationTimingGOOGLE" ) );

      //=== VK_EXT_discard_rectangles ===
      vkCmdSetDiscardRectangleEXT       = PFN_vkCmdSetDiscardRectangleEXT( vkGetDeviceProcAddr( device, "vkCmdSetDiscardRectangleEXT" ) );
      vkCmdSetDiscardRectangleEnableEXT = PFN_vkCmdSetDiscardRectangleEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDiscardRectangleEnableEXT" ) );
      vkCmdSetDiscardRectangleModeEXT   = PFN_vkCmdSetDiscardRectangleModeEXT( vkGetDeviceProcAddr( device, "vkCmdSetDiscardRectangleModeEXT" ) );

      //=== VK_EXT_hdr_metadata ===
      vkSetHdrMetadataEXT = PFN_vkSetHdrMetadataEXT( vkGetDeviceProcAddr( device, "vkSetHdrMetadataEXT" ) );

      //=== VK_KHR_create_renderpass2 ===
      vkCreateRenderPass2KHR = PFN_vkCreateRenderPass2KHR( vkGetDeviceProcAddr( device, "vkCreateRenderPass2KHR" ) );
      if ( !vkCreateRenderPass2 )
        vkCreateRenderPass2 = vkCreateRenderPass2KHR;
      vkCmdBeginRenderPass2KHR = PFN_vkCmdBeginRenderPass2KHR( vkGetDeviceProcAddr( device, "vkCmdBeginRenderPass2KHR" ) );
      if ( !vkCmdBeginRenderPass2 )
        vkCmdBeginRenderPass2 = vkCmdBeginRenderPass2KHR;
      vkCmdNextSubpass2KHR = PFN_vkCmdNextSubpass2KHR( vkGetDeviceProcAddr( device, "vkCmdNextSubpass2KHR" ) );
      if ( !vkCmdNextSubpass2 )
        vkCmdNextSubpass2 = vkCmdNextSubpass2KHR;
      vkCmdEndRenderPass2KHR = PFN_vkCmdEndRenderPass2KHR( vkGetDeviceProcAddr( device, "vkCmdEndRenderPass2KHR" ) );
      if ( !vkCmdEndRenderPass2 )
        vkCmdEndRenderPass2 = vkCmdEndRenderPass2KHR;

      //=== VK_KHR_shared_presentable_image ===
      vkGetSwapchainStatusKHR = PFN_vkGetSwapchainStatusKHR( vkGetDeviceProcAddr( device, "vkGetSwapchainStatusKHR" ) );

#if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_KHR_external_fence_win32 ===
      vkImportFenceWin32HandleKHR = PFN_vkImportFenceWin32HandleKHR( vkGetDeviceProcAddr( device, "vkImportFenceWin32HandleKHR" ) );
      vkGetFenceWin32HandleKHR    = PFN_vkGetFenceWin32HandleKHR( vkGetDeviceProcAddr( device, "vkGetFenceWin32HandleKHR" ) );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_KHR_external_fence_fd ===
      vkImportFenceFdKHR = PFN_vkImportFenceFdKHR( vkGetDeviceProcAddr( device, "vkImportFenceFdKHR" ) );
      vkGetFenceFdKHR    = PFN_vkGetFenceFdKHR( vkGetDeviceProcAddr( device, "vkGetFenceFdKHR" ) );

      //=== VK_KHR_performance_query ===
      vkAcquireProfilingLockKHR = PFN_vkAcquireProfilingLockKHR( vkGetDeviceProcAddr( device, "vkAcquireProfilingLockKHR" ) );
      vkReleaseProfilingLockKHR = PFN_vkReleaseProfilingLockKHR( vkGetDeviceProcAddr( device, "vkReleaseProfilingLockKHR" ) );

      //=== VK_EXT_debug_utils ===
      vkSetDebugUtilsObjectNameEXT    = PFN_vkSetDebugUtilsObjectNameEXT( vkGetDeviceProcAddr( device, "vkSetDebugUtilsObjectNameEXT" ) );
      vkSetDebugUtilsObjectTagEXT     = PFN_vkSetDebugUtilsObjectTagEXT( vkGetDeviceProcAddr( device, "vkSetDebugUtilsObjectTagEXT" ) );
      vkQueueBeginDebugUtilsLabelEXT  = PFN_vkQueueBeginDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkQueueBeginDebugUtilsLabelEXT" ) );
      vkQueueEndDebugUtilsLabelEXT    = PFN_vkQueueEndDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkQueueEndDebugUtilsLabelEXT" ) );
      vkQueueInsertDebugUtilsLabelEXT = PFN_vkQueueInsertDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkQueueInsertDebugUtilsLabelEXT" ) );
      vkCmdBeginDebugUtilsLabelEXT    = PFN_vkCmdBeginDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkCmdBeginDebugUtilsLabelEXT" ) );
      vkCmdEndDebugUtilsLabelEXT      = PFN_vkCmdEndDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkCmdEndDebugUtilsLabelEXT" ) );
      vkCmdInsertDebugUtilsLabelEXT   = PFN_vkCmdInsertDebugUtilsLabelEXT( vkGetDeviceProcAddr( device, "vkCmdInsertDebugUtilsLabelEXT" ) );

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
      //=== VK_ANDROID_external_memory_android_hardware_buffer ===
      vkGetAndroidHardwareBufferPropertiesANDROID =
        PFN_vkGetAndroidHardwareBufferPropertiesANDROID( vkGetDeviceProcAddr( device, "vkGetAndroidHardwareBufferPropertiesANDROID" ) );
      vkGetMemoryAndroidHardwareBufferANDROID =
        PFN_vkGetMemoryAndroidHardwareBufferANDROID( vkGetDeviceProcAddr( device, "vkGetMemoryAndroidHardwareBufferANDROID" ) );
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#if defined( VK_ENABLE_BETA_EXTENSIONS )
      //=== VK_AMDX_shader_enqueue ===
      vkCreateExecutionGraphPipelinesAMDX = PFN_vkCreateExecutionGraphPipelinesAMDX( vkGetDeviceProcAddr( device, "vkCreateExecutionGraphPipelinesAMDX" ) );
      vkGetExecutionGraphPipelineScratchSizeAMDX =
        PFN_vkGetExecutionGraphPipelineScratchSizeAMDX( vkGetDeviceProcAddr( device, "vkGetExecutionGraphPipelineScratchSizeAMDX" ) );
      vkGetExecutionGraphPipelineNodeIndexAMDX =
        PFN_vkGetExecutionGraphPipelineNodeIndexAMDX( vkGetDeviceProcAddr( device, "vkGetExecutionGraphPipelineNodeIndexAMDX" ) );
      vkCmdInitializeGraphScratchMemoryAMDX =
        PFN_vkCmdInitializeGraphScratchMemoryAMDX( vkGetDeviceProcAddr( device, "vkCmdInitializeGraphScratchMemoryAMDX" ) );
      vkCmdDispatchGraphAMDX              = PFN_vkCmdDispatchGraphAMDX( vkGetDeviceProcAddr( device, "vkCmdDispatchGraphAMDX" ) );
      vkCmdDispatchGraphIndirectAMDX      = PFN_vkCmdDispatchGraphIndirectAMDX( vkGetDeviceProcAddr( device, "vkCmdDispatchGraphIndirectAMDX" ) );
      vkCmdDispatchGraphIndirectCountAMDX = PFN_vkCmdDispatchGraphIndirectCountAMDX( vkGetDeviceProcAddr( device, "vkCmdDispatchGraphIndirectCountAMDX" ) );
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

      //=== VK_EXT_descriptor_heap ===
      vkWriteSamplerDescriptorsEXT     = PFN_vkWriteSamplerDescriptorsEXT( vkGetDeviceProcAddr( device, "vkWriteSamplerDescriptorsEXT" ) );
      vkWriteResourceDescriptorsEXT    = PFN_vkWriteResourceDescriptorsEXT( vkGetDeviceProcAddr( device, "vkWriteResourceDescriptorsEXT" ) );
      vkCmdBindSamplerHeapEXT          = PFN_vkCmdBindSamplerHeapEXT( vkGetDeviceProcAddr( device, "vkCmdBindSamplerHeapEXT" ) );
      vkCmdBindResourceHeapEXT         = PFN_vkCmdBindResourceHeapEXT( vkGetDeviceProcAddr( device, "vkCmdBindResourceHeapEXT" ) );
      vkCmdPushDataEXT                 = PFN_vkCmdPushDataEXT( vkGetDeviceProcAddr( device, "vkCmdPushDataEXT" ) );
      vkGetImageOpaqueCaptureDataEXT   = PFN_vkGetImageOpaqueCaptureDataEXT( vkGetDeviceProcAddr( device, "vkGetImageOpaqueCaptureDataEXT" ) );
      vkRegisterCustomBorderColorEXT   = PFN_vkRegisterCustomBorderColorEXT( vkGetDeviceProcAddr( device, "vkRegisterCustomBorderColorEXT" ) );
      vkUnregisterCustomBorderColorEXT = PFN_vkUnregisterCustomBorderColorEXT( vkGetDeviceProcAddr( device, "vkUnregisterCustomBorderColorEXT" ) );
      vkGetTensorOpaqueCaptureDataARM  = PFN_vkGetTensorOpaqueCaptureDataARM( vkGetDeviceProcAddr( device, "vkGetTensorOpaqueCaptureDataARM" ) );

      //=== VK_EXT_sample_locations ===
      vkCmdSetSampleLocationsEXT = PFN_vkCmdSetSampleLocationsEXT( vkGetDeviceProcAddr( device, "vkCmdSetSampleLocationsEXT" ) );

      //=== VK_KHR_get_memory_requirements2 ===
      vkGetImageMemoryRequirements2KHR = PFN_vkGetImageMemoryRequirements2KHR( vkGetDeviceProcAddr( device, "vkGetImageMemoryRequirements2KHR" ) );
      if ( !vkGetImageMemoryRequirements2 )
        vkGetImageMemoryRequirements2 = vkGetImageMemoryRequirements2KHR;
      vkGetBufferMemoryRequirements2KHR = PFN_vkGetBufferMemoryRequirements2KHR( vkGetDeviceProcAddr( device, "vkGetBufferMemoryRequirements2KHR" ) );
      if ( !vkGetBufferMemoryRequirements2 )
        vkGetBufferMemoryRequirements2 = vkGetBufferMemoryRequirements2KHR;
      vkGetImageSparseMemoryRequirements2KHR =
        PFN_vkGetImageSparseMemoryRequirements2KHR( vkGetDeviceProcAddr( device, "vkGetImageSparseMemoryRequirements2KHR" ) );
      if ( !vkGetImageSparseMemoryRequirements2 )
        vkGetImageSparseMemoryRequirements2 = vkGetImageSparseMemoryRequirements2KHR;

      //=== VK_KHR_acceleration_structure ===
      vkCreateAccelerationStructureKHR    = PFN_vkCreateAccelerationStructureKHR( vkGetDeviceProcAddr( device, "vkCreateAccelerationStructureKHR" ) );
      vkDestroyAccelerationStructureKHR   = PFN_vkDestroyAccelerationStructureKHR( vkGetDeviceProcAddr( device, "vkDestroyAccelerationStructureKHR" ) );
      vkCmdBuildAccelerationStructuresKHR = PFN_vkCmdBuildAccelerationStructuresKHR( vkGetDeviceProcAddr( device, "vkCmdBuildAccelerationStructuresKHR" ) );
      vkCmdBuildAccelerationStructuresIndirectKHR =
        PFN_vkCmdBuildAccelerationStructuresIndirectKHR( vkGetDeviceProcAddr( device, "vkCmdBuildAccelerationStructuresIndirectKHR" ) );
      vkBuildAccelerationStructuresKHR = PFN_vkBuildAccelerationStructuresKHR( vkGetDeviceProcAddr( device, "vkBuildAccelerationStructuresKHR" ) );
      vkCopyAccelerationStructureKHR   = PFN_vkCopyAccelerationStructureKHR( vkGetDeviceProcAddr( device, "vkCopyAccelerationStructureKHR" ) );
      vkCopyAccelerationStructureToMemoryKHR =
        PFN_vkCopyAccelerationStructureToMemoryKHR( vkGetDeviceProcAddr( device, "vkCopyAccelerationStructureToMemoryKHR" ) );
      vkCopyMemoryToAccelerationStructureKHR =
        PFN_vkCopyMemoryToAccelerationStructureKHR( vkGetDeviceProcAddr( device, "vkCopyMemoryToAccelerationStructureKHR" ) );
      vkWriteAccelerationStructuresPropertiesKHR =
        PFN_vkWriteAccelerationStructuresPropertiesKHR( vkGetDeviceProcAddr( device, "vkWriteAccelerationStructuresPropertiesKHR" ) );
      vkCmdCopyAccelerationStructureKHR = PFN_vkCmdCopyAccelerationStructureKHR( vkGetDeviceProcAddr( device, "vkCmdCopyAccelerationStructureKHR" ) );
      vkCmdCopyAccelerationStructureToMemoryKHR =
        PFN_vkCmdCopyAccelerationStructureToMemoryKHR( vkGetDeviceProcAddr( device, "vkCmdCopyAccelerationStructureToMemoryKHR" ) );
      vkCmdCopyMemoryToAccelerationStructureKHR =
        PFN_vkCmdCopyMemoryToAccelerationStructureKHR( vkGetDeviceProcAddr( device, "vkCmdCopyMemoryToAccelerationStructureKHR" ) );
      vkGetAccelerationStructureDeviceAddressKHR =
        PFN_vkGetAccelerationStructureDeviceAddressKHR( vkGetDeviceProcAddr( device, "vkGetAccelerationStructureDeviceAddressKHR" ) );
      vkCmdWriteAccelerationStructuresPropertiesKHR =
        PFN_vkCmdWriteAccelerationStructuresPropertiesKHR( vkGetDeviceProcAddr( device, "vkCmdWriteAccelerationStructuresPropertiesKHR" ) );
      vkGetDeviceAccelerationStructureCompatibilityKHR =
        PFN_vkGetDeviceAccelerationStructureCompatibilityKHR( vkGetDeviceProcAddr( device, "vkGetDeviceAccelerationStructureCompatibilityKHR" ) );
      vkGetAccelerationStructureBuildSizesKHR =
        PFN_vkGetAccelerationStructureBuildSizesKHR( vkGetDeviceProcAddr( device, "vkGetAccelerationStructureBuildSizesKHR" ) );

      //=== VK_KHR_ray_tracing_pipeline ===
      vkCmdTraceRaysKHR                    = PFN_vkCmdTraceRaysKHR( vkGetDeviceProcAddr( device, "vkCmdTraceRaysKHR" ) );
      vkCreateRayTracingPipelinesKHR       = PFN_vkCreateRayTracingPipelinesKHR( vkGetDeviceProcAddr( device, "vkCreateRayTracingPipelinesKHR" ) );
      vkGetRayTracingShaderGroupHandlesKHR = PFN_vkGetRayTracingShaderGroupHandlesKHR( vkGetDeviceProcAddr( device, "vkGetRayTracingShaderGroupHandlesKHR" ) );
      vkGetRayTracingCaptureReplayShaderGroupHandlesKHR =
        PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR( vkGetDeviceProcAddr( device, "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR" ) );
      vkCmdTraceRaysIndirectKHR = PFN_vkCmdTraceRaysIndirectKHR( vkGetDeviceProcAddr( device, "vkCmdTraceRaysIndirectKHR" ) );
      vkGetRayTracingShaderGroupStackSizeKHR =
        PFN_vkGetRayTracingShaderGroupStackSizeKHR( vkGetDeviceProcAddr( device, "vkGetRayTracingShaderGroupStackSizeKHR" ) );
      vkCmdSetRayTracingPipelineStackSizeKHR =
        PFN_vkCmdSetRayTracingPipelineStackSizeKHR( vkGetDeviceProcAddr( device, "vkCmdSetRayTracingPipelineStackSizeKHR" ) );

      //=== VK_KHR_sampler_ycbcr_conversion ===
      vkCreateSamplerYcbcrConversionKHR = PFN_vkCreateSamplerYcbcrConversionKHR( vkGetDeviceProcAddr( device, "vkCreateSamplerYcbcrConversionKHR" ) );
      if ( !vkCreateSamplerYcbcrConversion )
        vkCreateSamplerYcbcrConversion = vkCreateSamplerYcbcrConversionKHR;
      vkDestroySamplerYcbcrConversionKHR = PFN_vkDestroySamplerYcbcrConversionKHR( vkGetDeviceProcAddr( device, "vkDestroySamplerYcbcrConversionKHR" ) );
      if ( !vkDestroySamplerYcbcrConversion )
        vkDestroySamplerYcbcrConversion = vkDestroySamplerYcbcrConversionKHR;

      //=== VK_KHR_bind_memory2 ===
      vkBindBufferMemory2KHR = PFN_vkBindBufferMemory2KHR( vkGetDeviceProcAddr( device, "vkBindBufferMemory2KHR" ) );
      if ( !vkBindBufferMemory2 )
        vkBindBufferMemory2 = vkBindBufferMemory2KHR;
      vkBindImageMemory2KHR = PFN_vkBindImageMemory2KHR( vkGetDeviceProcAddr( device, "vkBindImageMemory2KHR" ) );
      if ( !vkBindImageMemory2 )
        vkBindImageMemory2 = vkBindImageMemory2KHR;

      //=== VK_EXT_image_drm_format_modifier ===
      vkGetImageDrmFormatModifierPropertiesEXT =
        PFN_vkGetImageDrmFormatModifierPropertiesEXT( vkGetDeviceProcAddr( device, "vkGetImageDrmFormatModifierPropertiesEXT" ) );

      //=== VK_EXT_validation_cache ===
      vkCreateValidationCacheEXT  = PFN_vkCreateValidationCacheEXT( vkGetDeviceProcAddr( device, "vkCreateValidationCacheEXT" ) );
      vkDestroyValidationCacheEXT = PFN_vkDestroyValidationCacheEXT( vkGetDeviceProcAddr( device, "vkDestroyValidationCacheEXT" ) );
      vkMergeValidationCachesEXT  = PFN_vkMergeValidationCachesEXT( vkGetDeviceProcAddr( device, "vkMergeValidationCachesEXT" ) );
      vkGetValidationCacheDataEXT = PFN_vkGetValidationCacheDataEXT( vkGetDeviceProcAddr( device, "vkGetValidationCacheDataEXT" ) );

      //=== VK_NV_shading_rate_image ===
      vkCmdBindShadingRateImageNV          = PFN_vkCmdBindShadingRateImageNV( vkGetDeviceProcAddr( device, "vkCmdBindShadingRateImageNV" ) );
      vkCmdSetViewportShadingRatePaletteNV = PFN_vkCmdSetViewportShadingRatePaletteNV( vkGetDeviceProcAddr( device, "vkCmdSetViewportShadingRatePaletteNV" ) );
      vkCmdSetCoarseSampleOrderNV          = PFN_vkCmdSetCoarseSampleOrderNV( vkGetDeviceProcAddr( device, "vkCmdSetCoarseSampleOrderNV" ) );

      //=== VK_NV_ray_tracing ===
      vkCreateAccelerationStructureNV  = PFN_vkCreateAccelerationStructureNV( vkGetDeviceProcAddr( device, "vkCreateAccelerationStructureNV" ) );
      vkDestroyAccelerationStructureNV = PFN_vkDestroyAccelerationStructureNV( vkGetDeviceProcAddr( device, "vkDestroyAccelerationStructureNV" ) );
      vkGetAccelerationStructureMemoryRequirementsNV =
        PFN_vkGetAccelerationStructureMemoryRequirementsNV( vkGetDeviceProcAddr( device, "vkGetAccelerationStructureMemoryRequirementsNV" ) );
      vkBindAccelerationStructureMemoryNV = PFN_vkBindAccelerationStructureMemoryNV( vkGetDeviceProcAddr( device, "vkBindAccelerationStructureMemoryNV" ) );
      vkCmdBuildAccelerationStructureNV   = PFN_vkCmdBuildAccelerationStructureNV( vkGetDeviceProcAddr( device, "vkCmdBuildAccelerationStructureNV" ) );
      vkCmdCopyAccelerationStructureNV    = PFN_vkCmdCopyAccelerationStructureNV( vkGetDeviceProcAddr( device, "vkCmdCopyAccelerationStructureNV" ) );
      vkCmdTraceRaysNV                    = PFN_vkCmdTraceRaysNV( vkGetDeviceProcAddr( device, "vkCmdTraceRaysNV" ) );
      vkCreateRayTracingPipelinesNV       = PFN_vkCreateRayTracingPipelinesNV( vkGetDeviceProcAddr( device, "vkCreateRayTracingPipelinesNV" ) );
      vkGetRayTracingShaderGroupHandlesNV = PFN_vkGetRayTracingShaderGroupHandlesNV( vkGetDeviceProcAddr( device, "vkGetRayTracingShaderGroupHandlesNV" ) );
      if ( !vkGetRayTracingShaderGroupHandlesKHR )
        vkGetRayTracingShaderGroupHandlesKHR = vkGetRayTracingShaderGroupHandlesNV;
      vkGetAccelerationStructureHandleNV = PFN_vkGetAccelerationStructureHandleNV( vkGetDeviceProcAddr( device, "vkGetAccelerationStructureHandleNV" ) );
      vkCmdWriteAccelerationStructuresPropertiesNV =
        PFN_vkCmdWriteAccelerationStructuresPropertiesNV( vkGetDeviceProcAddr( device, "vkCmdWriteAccelerationStructuresPropertiesNV" ) );
      vkCompileDeferredNV = PFN_vkCompileDeferredNV( vkGetDeviceProcAddr( device, "vkCompileDeferredNV" ) );

      //=== VK_KHR_maintenance3 ===
      vkGetDescriptorSetLayoutSupportKHR = PFN_vkGetDescriptorSetLayoutSupportKHR( vkGetDeviceProcAddr( device, "vkGetDescriptorSetLayoutSupportKHR" ) );
      if ( !vkGetDescriptorSetLayoutSupport )
        vkGetDescriptorSetLayoutSupport = vkGetDescriptorSetLayoutSupportKHR;

      //=== VK_KHR_draw_indirect_count ===
      vkCmdDrawIndirectCountKHR = PFN_vkCmdDrawIndirectCountKHR( vkGetDeviceProcAddr( device, "vkCmdDrawIndirectCountKHR" ) );
      if ( !vkCmdDrawIndirectCount )
        vkCmdDrawIndirectCount = vkCmdDrawIndirectCountKHR;
      vkCmdDrawIndexedIndirectCountKHR = PFN_vkCmdDrawIndexedIndirectCountKHR( vkGetDeviceProcAddr( device, "vkCmdDrawIndexedIndirectCountKHR" ) );
      if ( !vkCmdDrawIndexedIndirectCount )
        vkCmdDrawIndexedIndirectCount = vkCmdDrawIndexedIndirectCountKHR;

      //=== VK_EXT_external_memory_host ===
      vkGetMemoryHostPointerPropertiesEXT = PFN_vkGetMemoryHostPointerPropertiesEXT( vkGetDeviceProcAddr( device, "vkGetMemoryHostPointerPropertiesEXT" ) );

      //=== VK_AMD_buffer_marker ===
      vkCmdWriteBufferMarkerAMD  = PFN_vkCmdWriteBufferMarkerAMD( vkGetDeviceProcAddr( device, "vkCmdWriteBufferMarkerAMD" ) );
      vkCmdWriteBufferMarker2AMD = PFN_vkCmdWriteBufferMarker2AMD( vkGetDeviceProcAddr( device, "vkCmdWriteBufferMarker2AMD" ) );

      //=== VK_EXT_calibrated_timestamps ===
      vkGetCalibratedTimestampsEXT = PFN_vkGetCalibratedTimestampsEXT( vkGetDeviceProcAddr( device, "vkGetCalibratedTimestampsEXT" ) );
      if ( !vkGetCalibratedTimestampsKHR )
        vkGetCalibratedTimestampsKHR = vkGetCalibratedTimestampsEXT;

      //=== VK_NV_mesh_shader ===
      vkCmdDrawMeshTasksNV              = PFN_vkCmdDrawMeshTasksNV( vkGetDeviceProcAddr( device, "vkCmdDrawMeshTasksNV" ) );
      vkCmdDrawMeshTasksIndirectNV      = PFN_vkCmdDrawMeshTasksIndirectNV( vkGetDeviceProcAddr( device, "vkCmdDrawMeshTasksIndirectNV" ) );
      vkCmdDrawMeshTasksIndirectCountNV = PFN_vkCmdDrawMeshTasksIndirectCountNV( vkGetDeviceProcAddr( device, "vkCmdDrawMeshTasksIndirectCountNV" ) );

      //=== VK_NV_scissor_exclusive ===
      vkCmdSetExclusiveScissorEnableNV = PFN_vkCmdSetExclusiveScissorEnableNV( vkGetDeviceProcAddr( device, "vkCmdSetExclusiveScissorEnableNV" ) );
      vkCmdSetExclusiveScissorNV       = PFN_vkCmdSetExclusiveScissorNV( vkGetDeviceProcAddr( device, "vkCmdSetExclusiveScissorNV" ) );

      //=== VK_NV_device_diagnostic_checkpoints ===
      vkCmdSetCheckpointNV        = PFN_vkCmdSetCheckpointNV( vkGetDeviceProcAddr( device, "vkCmdSetCheckpointNV" ) );
      vkGetQueueCheckpointDataNV  = PFN_vkGetQueueCheckpointDataNV( vkGetDeviceProcAddr( device, "vkGetQueueCheckpointDataNV" ) );
      vkGetQueueCheckpointData2NV = PFN_vkGetQueueCheckpointData2NV( vkGetDeviceProcAddr( device, "vkGetQueueCheckpointData2NV" ) );

      //=== VK_KHR_timeline_semaphore ===
      vkGetSemaphoreCounterValueKHR = PFN_vkGetSemaphoreCounterValueKHR( vkGetDeviceProcAddr( device, "vkGetSemaphoreCounterValueKHR" ) );
      if ( !vkGetSemaphoreCounterValue )
        vkGetSemaphoreCounterValue = vkGetSemaphoreCounterValueKHR;
      vkWaitSemaphoresKHR = PFN_vkWaitSemaphoresKHR( vkGetDeviceProcAddr( device, "vkWaitSemaphoresKHR" ) );
      if ( !vkWaitSemaphores )
        vkWaitSemaphores = vkWaitSemaphoresKHR;
      vkSignalSemaphoreKHR = PFN_vkSignalSemaphoreKHR( vkGetDeviceProcAddr( device, "vkSignalSemaphoreKHR" ) );
      if ( !vkSignalSemaphore )
        vkSignalSemaphore = vkSignalSemaphoreKHR;

      //=== VK_EXT_present_timing ===
      vkSetSwapchainPresentTimingQueueSizeEXT =
        PFN_vkSetSwapchainPresentTimingQueueSizeEXT( vkGetDeviceProcAddr( device, "vkSetSwapchainPresentTimingQueueSizeEXT" ) );
      vkGetSwapchainTimingPropertiesEXT = PFN_vkGetSwapchainTimingPropertiesEXT( vkGetDeviceProcAddr( device, "vkGetSwapchainTimingPropertiesEXT" ) );
      vkGetSwapchainTimeDomainPropertiesEXT =
        PFN_vkGetSwapchainTimeDomainPropertiesEXT( vkGetDeviceProcAddr( device, "vkGetSwapchainTimeDomainPropertiesEXT" ) );
      vkGetPastPresentationTimingEXT = PFN_vkGetPastPresentationTimingEXT( vkGetDeviceProcAddr( device, "vkGetPastPresentationTimingEXT" ) );

      //=== VK_INTEL_performance_query ===
      vkInitializePerformanceApiINTEL      = PFN_vkInitializePerformanceApiINTEL( vkGetDeviceProcAddr( device, "vkInitializePerformanceApiINTEL" ) );
      vkUninitializePerformanceApiINTEL    = PFN_vkUninitializePerformanceApiINTEL( vkGetDeviceProcAddr( device, "vkUninitializePerformanceApiINTEL" ) );
      vkCmdSetPerformanceMarkerINTEL       = PFN_vkCmdSetPerformanceMarkerINTEL( vkGetDeviceProcAddr( device, "vkCmdSetPerformanceMarkerINTEL" ) );
      vkCmdSetPerformanceStreamMarkerINTEL = PFN_vkCmdSetPerformanceStreamMarkerINTEL( vkGetDeviceProcAddr( device, "vkCmdSetPerformanceStreamMarkerINTEL" ) );
      vkCmdSetPerformanceOverrideINTEL     = PFN_vkCmdSetPerformanceOverrideINTEL( vkGetDeviceProcAddr( device, "vkCmdSetPerformanceOverrideINTEL" ) );
      vkAcquirePerformanceConfigurationINTEL =
        PFN_vkAcquirePerformanceConfigurationINTEL( vkGetDeviceProcAddr( device, "vkAcquirePerformanceConfigurationINTEL" ) );
      vkReleasePerformanceConfigurationINTEL =
        PFN_vkReleasePerformanceConfigurationINTEL( vkGetDeviceProcAddr( device, "vkReleasePerformanceConfigurationINTEL" ) );
      vkQueueSetPerformanceConfigurationINTEL =
        PFN_vkQueueSetPerformanceConfigurationINTEL( vkGetDeviceProcAddr( device, "vkQueueSetPerformanceConfigurationINTEL" ) );
      vkGetPerformanceParameterINTEL = PFN_vkGetPerformanceParameterINTEL( vkGetDeviceProcAddr( device, "vkGetPerformanceParameterINTEL" ) );

      //=== VK_AMD_display_native_hdr ===
      vkSetLocalDimmingAMD = PFN_vkSetLocalDimmingAMD( vkGetDeviceProcAddr( device, "vkSetLocalDimmingAMD" ) );

      //=== VK_KHR_fragment_shading_rate ===
      vkCmdSetFragmentShadingRateKHR = PFN_vkCmdSetFragmentShadingRateKHR( vkGetDeviceProcAddr( device, "vkCmdSetFragmentShadingRateKHR" ) );

      //=== VK_KHR_dynamic_rendering_local_read ===
      vkCmdSetRenderingAttachmentLocationsKHR =
        PFN_vkCmdSetRenderingAttachmentLocationsKHR( vkGetDeviceProcAddr( device, "vkCmdSetRenderingAttachmentLocationsKHR" ) );
      if ( !vkCmdSetRenderingAttachmentLocations )
        vkCmdSetRenderingAttachmentLocations = vkCmdSetRenderingAttachmentLocationsKHR;
      vkCmdSetRenderingInputAttachmentIndicesKHR =
        PFN_vkCmdSetRenderingInputAttachmentIndicesKHR( vkGetDeviceProcAddr( device, "vkCmdSetRenderingInputAttachmentIndicesKHR" ) );
      if ( !vkCmdSetRenderingInputAttachmentIndices )
        vkCmdSetRenderingInputAttachmentIndices = vkCmdSetRenderingInputAttachmentIndicesKHR;

      //=== VK_EXT_buffer_device_address ===
      vkGetBufferDeviceAddressEXT = PFN_vkGetBufferDeviceAddressEXT( vkGetDeviceProcAddr( device, "vkGetBufferDeviceAddressEXT" ) );
      if ( !vkGetBufferDeviceAddress )
        vkGetBufferDeviceAddress = vkGetBufferDeviceAddressEXT;

      //=== VK_KHR_present_wait ===
      vkWaitForPresentKHR = PFN_vkWaitForPresentKHR( vkGetDeviceProcAddr( device, "vkWaitForPresentKHR" ) );

#if defined( VK_USE_PLATFORM_WIN32_KHR )
      //=== VK_EXT_full_screen_exclusive ===
      vkAcquireFullScreenExclusiveModeEXT = PFN_vkAcquireFullScreenExclusiveModeEXT( vkGetDeviceProcAddr( device, "vkAcquireFullScreenExclusiveModeEXT" ) );
      vkReleaseFullScreenExclusiveModeEXT = PFN_vkReleaseFullScreenExclusiveModeEXT( vkGetDeviceProcAddr( device, "vkReleaseFullScreenExclusiveModeEXT" ) );
      vkGetDeviceGroupSurfacePresentModes2EXT =
        PFN_vkGetDeviceGroupSurfacePresentModes2EXT( vkGetDeviceProcAddr( device, "vkGetDeviceGroupSurfacePresentModes2EXT" ) );
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

      //=== VK_KHR_buffer_device_address ===
      vkGetBufferDeviceAddressKHR = PFN_vkGetBufferDeviceAddressKHR( vkGetDeviceProcAddr( device, "vkGetBufferDeviceAddressKHR" ) );
      if ( !vkGetBufferDeviceAddress )
        vkGetBufferDeviceAddress = vkGetBufferDeviceAddressKHR;
      vkGetBufferOpaqueCaptureAddressKHR = PFN_vkGetBufferOpaqueCaptureAddressKHR( vkGetDeviceProcAddr( device, "vkGetBufferOpaqueCaptureAddressKHR" ) );
      if ( !vkGetBufferOpaqueCaptureAddress )
        vkGetBufferOpaqueCaptureAddress = vkGetBufferOpaqueCaptureAddressKHR;
      vkGetDeviceMemoryOpaqueCaptureAddressKHR =
        PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR( vkGetDeviceProcAddr( device, "vkGetDeviceMemoryOpaqueCaptureAddressKHR" ) );
      if ( !vkGetDeviceMemoryOpaqueCaptureAddress )
        vkGetDeviceMemoryOpaqueCaptureAddress = vkGetDeviceMemoryOpaqueCaptureAddressKHR;

      //=== VK_EXT_line_rasterization ===
      vkCmdSetLineStippleEXT = PFN_vkCmdSetLineStippleEXT( vkGetDeviceProcAddr( device, "vkCmdSetLineStippleEXT" ) );
      if ( !vkCmdSetLineStipple )
        vkCmdSetLineStipple = vkCmdSetLineStippleEXT;

      //=== VK_EXT_host_query_reset ===
      vkResetQueryPoolEXT = PFN_vkResetQueryPoolEXT( vkGetDeviceProcAddr( device, "vkResetQueryPoolEXT" ) );
      if ( !vkResetQueryPool )
        vkResetQueryPool = vkResetQueryPoolEXT;

      //=== VK_EXT_extended_dynamic_state ===
      vkCmdSetCullModeEXT = PFN_vkCmdSetCullModeEXT( vkGetDeviceProcAddr( device, "vkCmdSetCullModeEXT" ) );
      if ( !vkCmdSetCullMode )
        vkCmdSetCullMode = vkCmdSetCullModeEXT;
      vkCmdSetFrontFaceEXT = PFN_vkCmdSetFrontFaceEXT( vkGetDeviceProcAddr( device, "vkCmdSetFrontFaceEXT" ) );
      if ( !vkCmdSetFrontFace )
        vkCmdSetFrontFace = vkCmdSetFrontFaceEXT;
      vkCmdSetPrimitiveTopologyEXT = PFN_vkCmdSetPrimitiveTopologyEXT( vkGetDeviceProcAddr( device, "vkCmdSetPrimitiveTopologyEXT" ) );
      if ( !vkCmdSetPrimitiveTopology )
        vkCmdSetPrimitiveTopology = vkCmdSetPrimitiveTopologyEXT;
      vkCmdSetViewportWithCountEXT = PFN_vkCmdSetViewportWithCountEXT( vkGetDeviceProcAddr( device, "vkCmdSetViewportWithCountEXT" ) );
      if ( !vkCmdSetViewportWithCount )
        vkCmdSetViewportWithCount = vkCmdSetViewportWithCountEXT;
      vkCmdSetScissorWithCountEXT = PFN_vkCmdSetScissorWithCountEXT( vkGetDeviceProcAddr( device, "vkCmdSetScissorWithCountEXT" ) );
      if ( !vkCmdSetScissorWithCount )
        vkCmdSetScissorWithCount = vkCmdSetScissorWithCountEXT;
      vkCmdBindVertexBuffers2EXT = PFN_vkCmdBindVertexBuffers2EXT( vkGetDeviceProcAddr( device, "vkCmdBindVertexBuffers2EXT" ) );
      if ( !vkCmdBindVertexBuffers2 )
        vkCmdBindVertexBuffers2 = vkCmdBindVertexBuffers2EXT;
      vkCmdSetDepthTestEnableEXT = PFN_vkCmdSetDepthTestEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthTestEnableEXT" ) );
      if ( !vkCmdSetDepthTestEnable )
        vkCmdSetDepthTestEnable = vkCmdSetDepthTestEnableEXT;
      vkCmdSetDepthWriteEnableEXT = PFN_vkCmdSetDepthWriteEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthWriteEnableEXT" ) );
      if ( !vkCmdSetDepthWriteEnable )
        vkCmdSetDepthWriteEnable = vkCmdSetDepthWriteEnableEXT;
      vkCmdSetDepthCompareOpEXT = PFN_vkCmdSetDepthCompareOpEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthCompareOpEXT" ) );
      if ( !vkCmdSetDepthCompareOp )
        vkCmdSetDepthCompareOp = vkCmdSetDepthCompareOpEXT;
      vkCmdSetDepthBoundsTestEnableEXT = PFN_vkCmdSetDepthBoundsTestEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthBoundsTestEnableEXT" ) );
      if ( !vkCmdSetDepthBoundsTestEnable )
        vkCmdSetDepthBoundsTestEnable = vkCmdSetDepthBoundsTestEnableEXT;
      vkCmdSetStencilTestEnableEXT = PFN_vkCmdSetStencilTestEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetStencilTestEnableEXT" ) );
      if ( !vkCmdSetStencilTestEnable )
        vkCmdSetStencilTestEnable = vkCmdSetStencilTestEnableEXT;
      vkCmdSetStencilOpEXT = PFN_vkCmdSetStencilOpEXT( vkGetDeviceProcAddr( device, "vkCmdSetStencilOpEXT" ) );
      if ( !vkCmdSetStencilOp )
        vkCmdSetStencilOp = vkCmdSetStencilOpEXT;

      //=== VK_KHR_deferred_host_operations ===
      vkCreateDeferredOperationKHR  = PFN_vkCreateDeferredOperationKHR( vkGetDeviceProcAddr( device, "vkCreateDeferredOperationKHR" ) );
      vkDestroyDeferredOperationKHR = PFN_vkDestroyDeferredOperationKHR( vkGetDeviceProcAddr( device, "vkDestroyDeferredOperationKHR" ) );
      vkGetDeferredOperationMaxConcurrencyKHR =
        PFN_vkGetDeferredOperationMaxConcurrencyKHR( vkGetDeviceProcAddr( device, "vkGetDeferredOperationMaxConcurrencyKHR" ) );
      vkGetDeferredOperationResultKHR = PFN_vkGetDeferredOperationResultKHR( vkGetDeviceProcAddr( device, "vkGetDeferredOperationResultKHR" ) );
      vkDeferredOperationJoinKHR      = PFN_vkDeferredOperationJoinKHR( vkGetDeviceProcAddr( device, "vkDeferredOperationJoinKHR" ) );

      //=== VK_KHR_pipeline_executable_properties ===
      vkGetPipelineExecutablePropertiesKHR = PFN_vkGetPipelineExecutablePropertiesKHR( vkGetDeviceProcAddr( device, "vkGetPipelineExecutablePropertiesKHR" ) );
      vkGetPipelineExecutableStatisticsKHR = PFN_vkGetPipelineExecutableStatisticsKHR( vkGetDeviceProcAddr( device, "vkGetPipelineExecutableStatisticsKHR" ) );
      vkGetPipelineExecutableInternalRepresentationsKHR =
        PFN_vkGetPipelineExecutableInternalRepresentationsKHR( vkGetDeviceProcAddr( device, "vkGetPipelineExecutableInternalRepresentationsKHR" ) );

      //=== VK_EXT_host_image_copy ===
      vkCopyMemoryToImageEXT = PFN_vkCopyMemoryToImageEXT( vkGetDeviceProcAddr( device, "vkCopyMemoryToImageEXT" ) );
      if ( !vkCopyMemoryToImage )
        vkCopyMemoryToImage = vkCopyMemoryToImageEXT;
      vkCopyImageToMemoryEXT = PFN_vkCopyImageToMemoryEXT( vkGetDeviceProcAddr( device, "vkCopyImageToMemoryEXT" ) );
      if ( !vkCopyImageToMemory )
        vkCopyImageToMemory = vkCopyImageToMemoryEXT;
      vkCopyImageToImageEXT = PFN_vkCopyImageToImageEXT( vkGetDeviceProcAddr( device, "vkCopyImageToImageEXT" ) );
      if ( !vkCopyImageToImage )
        vkCopyImageToImage = vkCopyImageToImageEXT;
      vkTransitionImageLayoutEXT = PFN_vkTransitionImageLayoutEXT( vkGetDeviceProcAddr( device, "vkTransitionImageLayoutEXT" ) );
      if ( !vkTransitionImageLayout )
        vkTransitionImageLayout = vkTransitionImageLayoutEXT;
      vkGetImageSubresourceLayout2EXT = PFN_vkGetImageSubresourceLayout2EXT( vkGetDeviceProcAddr( device, "vkGetImageSubresourceLayout2EXT" ) );
      if ( !vkGetImageSubresourceLayout2 )
        vkGetImageSubresourceLayout2 = vkGetImageSubresourceLayout2EXT;

      //=== VK_KHR_map_memory2 ===
      vkMapMemory2KHR = PFN_vkMapMemory2KHR( vkGetDeviceProcAddr( device, "vkMapMemory2KHR" ) );
      if ( !vkMapMemory2 )
        vkMapMemory2 = vkMapMemory2KHR;
      vkUnmapMemory2KHR = PFN_vkUnmapMemory2KHR( vkGetDeviceProcAddr( device, "vkUnmapMemory2KHR" ) );
      if ( !vkUnmapMemory2 )
        vkUnmapMemory2 = vkUnmapMemory2KHR;

      //=== VK_EXT_swapchain_maintenance1 ===
      vkReleaseSwapchainImagesEXT = PFN_vkReleaseSwapchainImagesEXT( vkGetDeviceProcAddr( device, "vkReleaseSwapchainImagesEXT" ) );
      if ( !vkReleaseSwapchainImagesKHR )
        vkReleaseSwapchainImagesKHR = vkReleaseSwapchainImagesEXT;

      //=== VK_NV_device_generated_commands ===
      vkGetGeneratedCommandsMemoryRequirementsNV =
        PFN_vkGetGeneratedCommandsMemoryRequirementsNV( vkGetDeviceProcAddr( device, "vkGetGeneratedCommandsMemoryRequirementsNV" ) );
      vkCmdPreprocessGeneratedCommandsNV = PFN_vkCmdPreprocessGeneratedCommandsNV( vkGetDeviceProcAddr( device, "vkCmdPreprocessGeneratedCommandsNV" ) );
      vkCmdExecuteGeneratedCommandsNV    = PFN_vkCmdExecuteGeneratedCommandsNV( vkGetDeviceProcAddr( device, "vkCmdExecuteGeneratedCommandsNV" ) );
      vkCmdBindPipelineShaderGroupNV     = PFN_vkCmdBindPipelineShaderGroupNV( vkGetDeviceProcAddr( device, "vkCmdBindPipelineShaderGroupNV" ) );
      vkCreateIndirectCommandsLayoutNV   = PFN_vkCreateIndirectCommandsLayoutNV( vkGetDeviceProcAddr( device, "vkCreateIndirectCommandsLayoutNV" ) );
      vkDestroyIndirectCommandsLayoutNV  = PFN_vkDestroyIndirectCommandsLayoutNV( vkGetDeviceProcAddr( device, "vkDestroyIndirectCommandsLayoutNV" ) );

      //=== VK_EXT_depth_bias_control ===
      vkCmdSetDepthBias2EXT = PFN_vkCmdSetDepthBias2EXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthBias2EXT" ) );

      //=== VK_EXT_private_data ===
      vkCreatePrivateDataSlotEXT = PFN_vkCreatePrivateDataSlotEXT( vkGetDeviceProcAddr( device, "vkCreatePrivateDataSlotEXT" ) );
      if ( !vkCreatePrivateDataSlot )
        vkCreatePrivateDataSlot = vkCreatePrivateDataSlotEXT;
      vkDestroyPrivateDataSlotEXT = PFN_vkDestroyPrivateDataSlotEXT( vkGetDeviceProcAddr( device, "vkDestroyPrivateDataSlotEXT" ) );
      if ( !vkDestroyPrivateDataSlot )
        vkDestroyPrivateDataSlot = vkDestroyPrivateDataSlotEXT;
      vkSetPrivateDataEXT = PFN_vkSetPrivateDataEXT( vkGetDeviceProcAddr( device, "vkSetPrivateDataEXT" ) );
      if ( !vkSetPrivateData )
        vkSetPrivateData = vkSetPrivateDataEXT;
      vkGetPrivateDataEXT = PFN_vkGetPrivateDataEXT( vkGetDeviceProcAddr( device, "vkGetPrivateDataEXT" ) );
      if ( !vkGetPrivateData )
        vkGetPrivateData = vkGetPrivateDataEXT;

      //=== VK_KHR_video_encode_queue ===
      vkGetEncodedVideoSessionParametersKHR =
        PFN_vkGetEncodedVideoSessionParametersKHR( vkGetDeviceProcAddr( device, "vkGetEncodedVideoSessionParametersKHR" ) );
      vkCmdEncodeVideoKHR = PFN_vkCmdEncodeVideoKHR( vkGetDeviceProcAddr( device, "vkCmdEncodeVideoKHR" ) );

#if defined( VK_ENABLE_BETA_EXTENSIONS )
      //=== VK_NV_cuda_kernel_launch ===
      vkCreateCudaModuleNV    = PFN_vkCreateCudaModuleNV( vkGetDeviceProcAddr( device, "vkCreateCudaModuleNV" ) );
      vkGetCudaModuleCacheNV  = PFN_vkGetCudaModuleCacheNV( vkGetDeviceProcAddr( device, "vkGetCudaModuleCacheNV" ) );
      vkCreateCudaFunctionNV  = PFN_vkCreateCudaFunctionNV( vkGetDeviceProcAddr( device, "vkCreateCudaFunctionNV" ) );
      vkDestroyCudaModuleNV   = PFN_vkDestroyCudaModuleNV( vkGetDeviceProcAddr( device, "vkDestroyCudaModuleNV" ) );
      vkDestroyCudaFunctionNV = PFN_vkDestroyCudaFunctionNV( vkGetDeviceProcAddr( device, "vkDestroyCudaFunctionNV" ) );
      vkCmdCudaLaunchKernelNV = PFN_vkCmdCudaLaunchKernelNV( vkGetDeviceProcAddr( device, "vkCmdCudaLaunchKernelNV" ) );
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

      //=== VK_QCOM_tile_shading ===
      vkCmdDispatchTileQCOM          = PFN_vkCmdDispatchTileQCOM( vkGetDeviceProcAddr( device, "vkCmdDispatchTileQCOM" ) );
      vkCmdBeginPerTileExecutionQCOM = PFN_vkCmdBeginPerTileExecutionQCOM( vkGetDeviceProcAddr( device, "vkCmdBeginPerTileExecutionQCOM" ) );
      vkCmdEndPerTileExecutionQCOM   = PFN_vkCmdEndPerTileExecutionQCOM( vkGetDeviceProcAddr( device, "vkCmdEndPerTileExecutionQCOM" ) );

#if defined( VK_USE_PLATFORM_METAL_EXT )
      //=== VK_EXT_metal_objects ===
      vkExportMetalObjectsEXT = PFN_vkExportMetalObjectsEXT( vkGetDeviceProcAddr( device, "vkExportMetalObjectsEXT" ) );
#endif /*VK_USE_PLATFORM_METAL_EXT*/

      //=== VK_KHR_synchronization2 ===
      vkCmdSetEvent2KHR = PFN_vkCmdSetEvent2KHR( vkGetDeviceProcAddr( device, "vkCmdSetEvent2KHR" ) );
      if ( !vkCmdSetEvent2 )
        vkCmdSetEvent2 = vkCmdSetEvent2KHR;
      vkCmdResetEvent2KHR = PFN_vkCmdResetEvent2KHR( vkGetDeviceProcAddr( device, "vkCmdResetEvent2KHR" ) );
      if ( !vkCmdResetEvent2 )
        vkCmdResetEvent2 = vkCmdResetEvent2KHR;
      vkCmdWaitEvents2KHR = PFN_vkCmdWaitEvents2KHR( vkGetDeviceProcAddr( device, "vkCmdWaitEvents2KHR" ) );
      if ( !vkCmdWaitEvents2 )
        vkCmdWaitEvents2 = vkCmdWaitEvents2KHR;
      vkCmdPipelineBarrier2KHR = PFN_vkCmdPipelineBarrier2KHR( vkGetDeviceProcAddr( device, "vkCmdPipelineBarrier2KHR" ) );
      if ( !vkCmdPipelineBarrier2 )
        vkCmdPipelineBarrier2 = vkCmdPipelineBarrier2KHR;
      vkCmdWriteTimestamp2KHR = PFN_vkCmdWriteTimestamp2KHR( vkGetDeviceProcAddr( device, "vkCmdWriteTimestamp2KHR" ) );
      if ( !vkCmdWriteTimestamp2 )
        vkCmdWriteTimestamp2 = vkCmdWriteTimestamp2KHR;
      vkQueueSubmit2KHR = PFN_vkQueueSubmit2KHR( vkGetDeviceProcAddr( device, "vkQueueSubmit2KHR" ) );
      if ( !vkQueueSubmit2 )
        vkQueueSubmit2 = vkQueueSubmit2KHR;

      //=== VK_EXT_descriptor_buffer ===
      vkGetDescriptorSetLayoutSizeEXT = PFN_vkGetDescriptorSetLayoutSizeEXT( vkGetDeviceProcAddr( device, "vkGetDescriptorSetLayoutSizeEXT" ) );
      vkGetDescriptorSetLayoutBindingOffsetEXT =
        PFN_vkGetDescriptorSetLayoutBindingOffsetEXT( vkGetDeviceProcAddr( device, "vkGetDescriptorSetLayoutBindingOffsetEXT" ) );
      vkGetDescriptorEXT                 = PFN_vkGetDescriptorEXT( vkGetDeviceProcAddr( device, "vkGetDescriptorEXT" ) );
      vkCmdBindDescriptorBuffersEXT      = PFN_vkCmdBindDescriptorBuffersEXT( vkGetDeviceProcAddr( device, "vkCmdBindDescriptorBuffersEXT" ) );
      vkCmdSetDescriptorBufferOffsetsEXT = PFN_vkCmdSetDescriptorBufferOffsetsEXT( vkGetDeviceProcAddr( device, "vkCmdSetDescriptorBufferOffsetsEXT" ) );
      vkCmdBindDescriptorBufferEmbeddedSamplersEXT =
        PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT( vkGetDeviceProcAddr( device, "vkCmdBindDescriptorBufferEmbeddedSamplersEXT" ) );
      vkGetBufferOpaqueCaptureDescriptorDataEXT =
        PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT( vkGetDeviceProcAddr( device, "vkGetBufferOpaqueCaptureDescriptorDataEXT" ) );
      vkGetImageOpaqueCaptureDescriptorDataEXT =
        PFN_vkGetImageOpaqueCaptureDescriptorDataEXT( vkGetDeviceProcAddr( device, "vkGetImageOpaqueCaptureDescriptorDataEXT" ) );
      vkGetImageViewOpaqueCaptureDescriptorDataEXT =
        PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT( vkGetDeviceProcAddr( device, "vkGetImageViewOpaqueCaptureDescriptorDataEXT" ) );
      vkGetSamplerOpaqueCaptureDescriptorDataEXT =
        PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT( vkGetDeviceProcAddr( device, "vkGetSamplerOpaqueCaptureDescriptorDataEXT" ) );
      vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT = PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT(
        vkGetDeviceProcAddr( device, "vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT" ) );

      //=== VK_NV_fragment_shading_rate_enums ===
      vkCmdSetFragmentShadingRateEnumNV = PFN_vkCmdSetFragmentShadingRateEnumNV( vkGetDeviceProcAddr( device, "vkCmdSetFragmentShadingRateEnumNV" ) );

      //=== VK_EXT_mesh_shader ===
      vkCmdDrawMeshTasksEXT              = PFN_vkCmdDrawMeshTasksEXT( vkGetDeviceProcAddr( device, "vkCmdDrawMeshTasksEXT" ) );
      vkCmdDrawMeshTasksIndirectEXT      = PFN_vkCmdDrawMeshTasksIndirectEXT( vkGetDeviceProcAddr( device, "vkCmdDrawMeshTasksIndirectEXT" ) );
      vkCmdDrawMeshTasksIndirectCountEXT = PFN_vkCmdDrawMeshTasksIndirectCountEXT( vkGetDeviceProcAddr( device, "vkCmdDrawMeshTasksIndirectCountEXT" ) );

      //=== VK_KHR_copy_commands2 ===
      vkCmdCopyBuffer2KHR = PFN_vkCmdCopyBuffer2KHR( vkGetDeviceProcAddr( device, "vkCmdCopyBuffer2KHR" ) );
      if ( !vkCmdCopyBuffer2 )
        vkCmdCopyBuffer2 = vkCmdCopyBuffer2KHR;
      vkCmdCopyImage2KHR = PFN_vkCmdCopyImage2KHR( vkGetDeviceProcAddr( device, "vkCmdCopyImage2KHR" ) );
      if ( !vkCmdCopyImage2 )
        vkCmdCopyImage2 = vkCmdCopyImage2KHR;
      vkCmdCopyBufferToImage2KHR = PFN_vkCmdCopyBufferToImage2KHR( vkGetDeviceProcAddr( device, "vkCmdCopyBufferToImage2KHR" ) );
      if ( !vkCmdCopyBufferToImage2 )
        vkCmdCopyBufferToImage2 = vkCmdCopyBufferToImage2KHR;
      vkCmdCopyImageToBuffer2KHR = PFN_vkCmdCopyImageToBuffer2KHR( vkGetDeviceProcAddr( device, "vkCmdCopyImageToBuffer2KHR" ) );
      if ( !vkCmdCopyImageToBuffer2 )
        vkCmdCopyImageToBuffer2 = vkCmdCopyImageToBuffer2KHR;
      vkCmdBlitImage2KHR = PFN_vkCmdBlitImage2KHR( vkGetDeviceProcAddr( device, "vkCmdBlitImage2KHR" ) );
      if ( !vkCmdBlitImage2 )
        vkCmdBlitImage2 = vkCmdBlitImage2KHR;
      vkCmdResolveImage2KHR = PFN_vkCmdResolveImage2KHR( vkGetDeviceProcAddr( device, "vkCmdResolveImage2KHR" ) );
      if ( !vkCmdResolveImage2 )
        vkCmdResolveImage2 = vkCmdResolveImage2KHR;

      //=== VK_EXT_device_fault ===
      vkGetDeviceFaultInfoEXT = PFN_vkGetDeviceFaultInfoEXT( vkGetDeviceProcAddr( device, "vkGetDeviceFaultInfoEXT" ) );

      //=== VK_EXT_vertex_input_dynamic_state ===
      vkCmdSetVertexInputEXT = PFN_vkCmdSetVertexInputEXT( vkGetDeviceProcAddr( device, "vkCmdSetVertexInputEXT" ) );

#if defined( VK_USE_PLATFORM_FUCHSIA )
      //=== VK_FUCHSIA_external_memory ===
      vkGetMemoryZirconHandleFUCHSIA = PFN_vkGetMemoryZirconHandleFUCHSIA( vkGetDeviceProcAddr( device, "vkGetMemoryZirconHandleFUCHSIA" ) );
      vkGetMemoryZirconHandlePropertiesFUCHSIA =
        PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA( vkGetDeviceProcAddr( device, "vkGetMemoryZirconHandlePropertiesFUCHSIA" ) );
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#if defined( VK_USE_PLATFORM_FUCHSIA )
      //=== VK_FUCHSIA_external_semaphore ===
      vkImportSemaphoreZirconHandleFUCHSIA = PFN_vkImportSemaphoreZirconHandleFUCHSIA( vkGetDeviceProcAddr( device, "vkImportSemaphoreZirconHandleFUCHSIA" ) );
      vkGetSemaphoreZirconHandleFUCHSIA    = PFN_vkGetSemaphoreZirconHandleFUCHSIA( vkGetDeviceProcAddr( device, "vkGetSemaphoreZirconHandleFUCHSIA" ) );
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#if defined( VK_USE_PLATFORM_FUCHSIA )
      //=== VK_FUCHSIA_buffer_collection ===
      vkCreateBufferCollectionFUCHSIA = PFN_vkCreateBufferCollectionFUCHSIA( vkGetDeviceProcAddr( device, "vkCreateBufferCollectionFUCHSIA" ) );
      vkSetBufferCollectionImageConstraintsFUCHSIA =
        PFN_vkSetBufferCollectionImageConstraintsFUCHSIA( vkGetDeviceProcAddr( device, "vkSetBufferCollectionImageConstraintsFUCHSIA" ) );
      vkSetBufferCollectionBufferConstraintsFUCHSIA =
        PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA( vkGetDeviceProcAddr( device, "vkSetBufferCollectionBufferConstraintsFUCHSIA" ) );
      vkDestroyBufferCollectionFUCHSIA = PFN_vkDestroyBufferCollectionFUCHSIA( vkGetDeviceProcAddr( device, "vkDestroyBufferCollectionFUCHSIA" ) );
      vkGetBufferCollectionPropertiesFUCHSIA =
        PFN_vkGetBufferCollectionPropertiesFUCHSIA( vkGetDeviceProcAddr( device, "vkGetBufferCollectionPropertiesFUCHSIA" ) );
#endif /*VK_USE_PLATFORM_FUCHSIA*/

      //=== VK_HUAWEI_subpass_shading ===
      vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI =
        PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI( vkGetDeviceProcAddr( device, "vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI" ) );
      vkCmdSubpassShadingHUAWEI = PFN_vkCmdSubpassShadingHUAWEI( vkGetDeviceProcAddr( device, "vkCmdSubpassShadingHUAWEI" ) );

      //=== VK_HUAWEI_invocation_mask ===
      vkCmdBindInvocationMaskHUAWEI = PFN_vkCmdBindInvocationMaskHUAWEI( vkGetDeviceProcAddr( device, "vkCmdBindInvocationMaskHUAWEI" ) );

      //=== VK_NV_external_memory_rdma ===
      vkGetMemoryRemoteAddressNV = PFN_vkGetMemoryRemoteAddressNV( vkGetDeviceProcAddr( device, "vkGetMemoryRemoteAddressNV" ) );

      //=== VK_EXT_pipeline_properties ===
      vkGetPipelinePropertiesEXT = PFN_vkGetPipelinePropertiesEXT( vkGetDeviceProcAddr( device, "vkGetPipelinePropertiesEXT" ) );

      //=== VK_EXT_extended_dynamic_state2 ===
      vkCmdSetPatchControlPointsEXT      = PFN_vkCmdSetPatchControlPointsEXT( vkGetDeviceProcAddr( device, "vkCmdSetPatchControlPointsEXT" ) );
      vkCmdSetRasterizerDiscardEnableEXT = PFN_vkCmdSetRasterizerDiscardEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetRasterizerDiscardEnableEXT" ) );
      if ( !vkCmdSetRasterizerDiscardEnable )
        vkCmdSetRasterizerDiscardEnable = vkCmdSetRasterizerDiscardEnableEXT;
      vkCmdSetDepthBiasEnableEXT = PFN_vkCmdSetDepthBiasEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthBiasEnableEXT" ) );
      if ( !vkCmdSetDepthBiasEnable )
        vkCmdSetDepthBiasEnable = vkCmdSetDepthBiasEnableEXT;
      vkCmdSetLogicOpEXT                = PFN_vkCmdSetLogicOpEXT( vkGetDeviceProcAddr( device, "vkCmdSetLogicOpEXT" ) );
      vkCmdSetPrimitiveRestartEnableEXT = PFN_vkCmdSetPrimitiveRestartEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetPrimitiveRestartEnableEXT" ) );
      if ( !vkCmdSetPrimitiveRestartEnable )
        vkCmdSetPrimitiveRestartEnable = vkCmdSetPrimitiveRestartEnableEXT;

      //=== VK_EXT_color_write_enable ===
      vkCmdSetColorWriteEnableEXT = PFN_vkCmdSetColorWriteEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetColorWriteEnableEXT" ) );

      //=== VK_KHR_ray_tracing_maintenance1 ===
      vkCmdTraceRaysIndirect2KHR = PFN_vkCmdTraceRaysIndirect2KHR( vkGetDeviceProcAddr( device, "vkCmdTraceRaysIndirect2KHR" ) );

      //=== VK_EXT_multi_draw ===
      vkCmdDrawMultiEXT        = PFN_vkCmdDrawMultiEXT( vkGetDeviceProcAddr( device, "vkCmdDrawMultiEXT" ) );
      vkCmdDrawMultiIndexedEXT = PFN_vkCmdDrawMultiIndexedEXT( vkGetDeviceProcAddr( device, "vkCmdDrawMultiIndexedEXT" ) );

      //=== VK_EXT_opacity_micromap ===
      vkCreateMicromapEXT                 = PFN_vkCreateMicromapEXT( vkGetDeviceProcAddr( device, "vkCreateMicromapEXT" ) );
      vkDestroyMicromapEXT                = PFN_vkDestroyMicromapEXT( vkGetDeviceProcAddr( device, "vkDestroyMicromapEXT" ) );
      vkCmdBuildMicromapsEXT              = PFN_vkCmdBuildMicromapsEXT( vkGetDeviceProcAddr( device, "vkCmdBuildMicromapsEXT" ) );
      vkBuildMicromapsEXT                 = PFN_vkBuildMicromapsEXT( vkGetDeviceProcAddr( device, "vkBuildMicromapsEXT" ) );
      vkCopyMicromapEXT                   = PFN_vkCopyMicromapEXT( vkGetDeviceProcAddr( device, "vkCopyMicromapEXT" ) );
      vkCopyMicromapToMemoryEXT           = PFN_vkCopyMicromapToMemoryEXT( vkGetDeviceProcAddr( device, "vkCopyMicromapToMemoryEXT" ) );
      vkCopyMemoryToMicromapEXT           = PFN_vkCopyMemoryToMicromapEXT( vkGetDeviceProcAddr( device, "vkCopyMemoryToMicromapEXT" ) );
      vkWriteMicromapsPropertiesEXT       = PFN_vkWriteMicromapsPropertiesEXT( vkGetDeviceProcAddr( device, "vkWriteMicromapsPropertiesEXT" ) );
      vkCmdCopyMicromapEXT                = PFN_vkCmdCopyMicromapEXT( vkGetDeviceProcAddr( device, "vkCmdCopyMicromapEXT" ) );
      vkCmdCopyMicromapToMemoryEXT        = PFN_vkCmdCopyMicromapToMemoryEXT( vkGetDeviceProcAddr( device, "vkCmdCopyMicromapToMemoryEXT" ) );
      vkCmdCopyMemoryToMicromapEXT        = PFN_vkCmdCopyMemoryToMicromapEXT( vkGetDeviceProcAddr( device, "vkCmdCopyMemoryToMicromapEXT" ) );
      vkCmdWriteMicromapsPropertiesEXT    = PFN_vkCmdWriteMicromapsPropertiesEXT( vkGetDeviceProcAddr( device, "vkCmdWriteMicromapsPropertiesEXT" ) );
      vkGetDeviceMicromapCompatibilityEXT = PFN_vkGetDeviceMicromapCompatibilityEXT( vkGetDeviceProcAddr( device, "vkGetDeviceMicromapCompatibilityEXT" ) );
      vkGetMicromapBuildSizesEXT          = PFN_vkGetMicromapBuildSizesEXT( vkGetDeviceProcAddr( device, "vkGetMicromapBuildSizesEXT" ) );

      //=== VK_HUAWEI_cluster_culling_shader ===
      vkCmdDrawClusterHUAWEI         = PFN_vkCmdDrawClusterHUAWEI( vkGetDeviceProcAddr( device, "vkCmdDrawClusterHUAWEI" ) );
      vkCmdDrawClusterIndirectHUAWEI = PFN_vkCmdDrawClusterIndirectHUAWEI( vkGetDeviceProcAddr( device, "vkCmdDrawClusterIndirectHUAWEI" ) );

      //=== VK_EXT_pageable_device_local_memory ===
      vkSetDeviceMemoryPriorityEXT = PFN_vkSetDeviceMemoryPriorityEXT( vkGetDeviceProcAddr( device, "vkSetDeviceMemoryPriorityEXT" ) );

      //=== VK_KHR_maintenance4 ===
      vkGetDeviceBufferMemoryRequirementsKHR =
        PFN_vkGetDeviceBufferMemoryRequirementsKHR( vkGetDeviceProcAddr( device, "vkGetDeviceBufferMemoryRequirementsKHR" ) );
      if ( !vkGetDeviceBufferMemoryRequirements )
        vkGetDeviceBufferMemoryRequirements = vkGetDeviceBufferMemoryRequirementsKHR;
      vkGetDeviceImageMemoryRequirementsKHR =
        PFN_vkGetDeviceImageMemoryRequirementsKHR( vkGetDeviceProcAddr( device, "vkGetDeviceImageMemoryRequirementsKHR" ) );
      if ( !vkGetDeviceImageMemoryRequirements )
        vkGetDeviceImageMemoryRequirements = vkGetDeviceImageMemoryRequirementsKHR;
      vkGetDeviceImageSparseMemoryRequirementsKHR =
        PFN_vkGetDeviceImageSparseMemoryRequirementsKHR( vkGetDeviceProcAddr( device, "vkGetDeviceImageSparseMemoryRequirementsKHR" ) );
      if ( !vkGetDeviceImageSparseMemoryRequirements )
        vkGetDeviceImageSparseMemoryRequirements = vkGetDeviceImageSparseMemoryRequirementsKHR;

      //=== VK_VALVE_descriptor_set_host_mapping ===
      vkGetDescriptorSetLayoutHostMappingInfoVALVE =
        PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE( vkGetDeviceProcAddr( device, "vkGetDescriptorSetLayoutHostMappingInfoVALVE" ) );
      vkGetDescriptorSetHostMappingVALVE = PFN_vkGetDescriptorSetHostMappingVALVE( vkGetDeviceProcAddr( device, "vkGetDescriptorSetHostMappingVALVE" ) );

      //=== VK_NV_copy_memory_indirect ===
      vkCmdCopyMemoryIndirectNV        = PFN_vkCmdCopyMemoryIndirectNV( vkGetDeviceProcAddr( device, "vkCmdCopyMemoryIndirectNV" ) );
      vkCmdCopyMemoryToImageIndirectNV = PFN_vkCmdCopyMemoryToImageIndirectNV( vkGetDeviceProcAddr( device, "vkCmdCopyMemoryToImageIndirectNV" ) );

      //=== VK_NV_memory_decompression ===
      vkCmdDecompressMemoryNV              = PFN_vkCmdDecompressMemoryNV( vkGetDeviceProcAddr( device, "vkCmdDecompressMemoryNV" ) );
      vkCmdDecompressMemoryIndirectCountNV = PFN_vkCmdDecompressMemoryIndirectCountNV( vkGetDeviceProcAddr( device, "vkCmdDecompressMemoryIndirectCountNV" ) );

      //=== VK_NV_device_generated_commands_compute ===
      vkGetPipelineIndirectMemoryRequirementsNV =
        PFN_vkGetPipelineIndirectMemoryRequirementsNV( vkGetDeviceProcAddr( device, "vkGetPipelineIndirectMemoryRequirementsNV" ) );
      vkCmdUpdatePipelineIndirectBufferNV  = PFN_vkCmdUpdatePipelineIndirectBufferNV( vkGetDeviceProcAddr( device, "vkCmdUpdatePipelineIndirectBufferNV" ) );
      vkGetPipelineIndirectDeviceAddressNV = PFN_vkGetPipelineIndirectDeviceAddressNV( vkGetDeviceProcAddr( device, "vkGetPipelineIndirectDeviceAddressNV" ) );

#if defined( VK_USE_PLATFORM_OHOS )
      //=== VK_OHOS_external_memory ===
      vkGetNativeBufferPropertiesOHOS = PFN_vkGetNativeBufferPropertiesOHOS( vkGetDeviceProcAddr( device, "vkGetNativeBufferPropertiesOHOS" ) );
      vkGetMemoryNativeBufferOHOS     = PFN_vkGetMemoryNativeBufferOHOS( vkGetDeviceProcAddr( device, "vkGetMemoryNativeBufferOHOS" ) );
#endif /*VK_USE_PLATFORM_OHOS*/

      //=== VK_EXT_extended_dynamic_state3 ===
      vkCmdSetDepthClampEnableEXT         = PFN_vkCmdSetDepthClampEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthClampEnableEXT" ) );
      vkCmdSetPolygonModeEXT              = PFN_vkCmdSetPolygonModeEXT( vkGetDeviceProcAddr( device, "vkCmdSetPolygonModeEXT" ) );
      vkCmdSetRasterizationSamplesEXT     = PFN_vkCmdSetRasterizationSamplesEXT( vkGetDeviceProcAddr( device, "vkCmdSetRasterizationSamplesEXT" ) );
      vkCmdSetSampleMaskEXT               = PFN_vkCmdSetSampleMaskEXT( vkGetDeviceProcAddr( device, "vkCmdSetSampleMaskEXT" ) );
      vkCmdSetAlphaToCoverageEnableEXT    = PFN_vkCmdSetAlphaToCoverageEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetAlphaToCoverageEnableEXT" ) );
      vkCmdSetAlphaToOneEnableEXT         = PFN_vkCmdSetAlphaToOneEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetAlphaToOneEnableEXT" ) );
      vkCmdSetLogicOpEnableEXT            = PFN_vkCmdSetLogicOpEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetLogicOpEnableEXT" ) );
      vkCmdSetColorBlendEnableEXT         = PFN_vkCmdSetColorBlendEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetColorBlendEnableEXT" ) );
      vkCmdSetColorBlendEquationEXT       = PFN_vkCmdSetColorBlendEquationEXT( vkGetDeviceProcAddr( device, "vkCmdSetColorBlendEquationEXT" ) );
      vkCmdSetColorWriteMaskEXT           = PFN_vkCmdSetColorWriteMaskEXT( vkGetDeviceProcAddr( device, "vkCmdSetColorWriteMaskEXT" ) );
      vkCmdSetTessellationDomainOriginEXT = PFN_vkCmdSetTessellationDomainOriginEXT( vkGetDeviceProcAddr( device, "vkCmdSetTessellationDomainOriginEXT" ) );
      vkCmdSetRasterizationStreamEXT      = PFN_vkCmdSetRasterizationStreamEXT( vkGetDeviceProcAddr( device, "vkCmdSetRasterizationStreamEXT" ) );
      vkCmdSetConservativeRasterizationModeEXT =
        PFN_vkCmdSetConservativeRasterizationModeEXT( vkGetDeviceProcAddr( device, "vkCmdSetConservativeRasterizationModeEXT" ) );
      vkCmdSetExtraPrimitiveOverestimationSizeEXT =
        PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT( vkGetDeviceProcAddr( device, "vkCmdSetExtraPrimitiveOverestimationSizeEXT" ) );
      vkCmdSetDepthClipEnableEXT           = PFN_vkCmdSetDepthClipEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthClipEnableEXT" ) );
      vkCmdSetSampleLocationsEnableEXT     = PFN_vkCmdSetSampleLocationsEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetSampleLocationsEnableEXT" ) );
      vkCmdSetColorBlendAdvancedEXT        = PFN_vkCmdSetColorBlendAdvancedEXT( vkGetDeviceProcAddr( device, "vkCmdSetColorBlendAdvancedEXT" ) );
      vkCmdSetProvokingVertexModeEXT       = PFN_vkCmdSetProvokingVertexModeEXT( vkGetDeviceProcAddr( device, "vkCmdSetProvokingVertexModeEXT" ) );
      vkCmdSetLineRasterizationModeEXT     = PFN_vkCmdSetLineRasterizationModeEXT( vkGetDeviceProcAddr( device, "vkCmdSetLineRasterizationModeEXT" ) );
      vkCmdSetLineStippleEnableEXT         = PFN_vkCmdSetLineStippleEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetLineStippleEnableEXT" ) );
      vkCmdSetDepthClipNegativeOneToOneEXT = PFN_vkCmdSetDepthClipNegativeOneToOneEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthClipNegativeOneToOneEXT" ) );
      vkCmdSetViewportWScalingEnableNV     = PFN_vkCmdSetViewportWScalingEnableNV( vkGetDeviceProcAddr( device, "vkCmdSetViewportWScalingEnableNV" ) );
      vkCmdSetViewportSwizzleNV            = PFN_vkCmdSetViewportSwizzleNV( vkGetDeviceProcAddr( device, "vkCmdSetViewportSwizzleNV" ) );
      vkCmdSetCoverageToColorEnableNV      = PFN_vkCmdSetCoverageToColorEnableNV( vkGetDeviceProcAddr( device, "vkCmdSetCoverageToColorEnableNV" ) );
      vkCmdSetCoverageToColorLocationNV    = PFN_vkCmdSetCoverageToColorLocationNV( vkGetDeviceProcAddr( device, "vkCmdSetCoverageToColorLocationNV" ) );
      vkCmdSetCoverageModulationModeNV     = PFN_vkCmdSetCoverageModulationModeNV( vkGetDeviceProcAddr( device, "vkCmdSetCoverageModulationModeNV" ) );
      vkCmdSetCoverageModulationTableEnableNV =
        PFN_vkCmdSetCoverageModulationTableEnableNV( vkGetDeviceProcAddr( device, "vkCmdSetCoverageModulationTableEnableNV" ) );
      vkCmdSetCoverageModulationTableNV = PFN_vkCmdSetCoverageModulationTableNV( vkGetDeviceProcAddr( device, "vkCmdSetCoverageModulationTableNV" ) );
      vkCmdSetShadingRateImageEnableNV  = PFN_vkCmdSetShadingRateImageEnableNV( vkGetDeviceProcAddr( device, "vkCmdSetShadingRateImageEnableNV" ) );
      vkCmdSetRepresentativeFragmentTestEnableNV =
        PFN_vkCmdSetRepresentativeFragmentTestEnableNV( vkGetDeviceProcAddr( device, "vkCmdSetRepresentativeFragmentTestEnableNV" ) );
      vkCmdSetCoverageReductionModeNV = PFN_vkCmdSetCoverageReductionModeNV( vkGetDeviceProcAddr( device, "vkCmdSetCoverageReductionModeNV" ) );

      //=== VK_ARM_tensors ===
      vkCreateTensorARM                = PFN_vkCreateTensorARM( vkGetDeviceProcAddr( device, "vkCreateTensorARM" ) );
      vkDestroyTensorARM               = PFN_vkDestroyTensorARM( vkGetDeviceProcAddr( device, "vkDestroyTensorARM" ) );
      vkCreateTensorViewARM            = PFN_vkCreateTensorViewARM( vkGetDeviceProcAddr( device, "vkCreateTensorViewARM" ) );
      vkDestroyTensorViewARM           = PFN_vkDestroyTensorViewARM( vkGetDeviceProcAddr( device, "vkDestroyTensorViewARM" ) );
      vkGetTensorMemoryRequirementsARM = PFN_vkGetTensorMemoryRequirementsARM( vkGetDeviceProcAddr( device, "vkGetTensorMemoryRequirementsARM" ) );
      vkBindTensorMemoryARM            = PFN_vkBindTensorMemoryARM( vkGetDeviceProcAddr( device, "vkBindTensorMemoryARM" ) );
      vkGetDeviceTensorMemoryRequirementsARM =
        PFN_vkGetDeviceTensorMemoryRequirementsARM( vkGetDeviceProcAddr( device, "vkGetDeviceTensorMemoryRequirementsARM" ) );
      vkCmdCopyTensorARM = PFN_vkCmdCopyTensorARM( vkGetDeviceProcAddr( device, "vkCmdCopyTensorARM" ) );
      vkGetTensorOpaqueCaptureDescriptorDataARM =
        PFN_vkGetTensorOpaqueCaptureDescriptorDataARM( vkGetDeviceProcAddr( device, "vkGetTensorOpaqueCaptureDescriptorDataARM" ) );
      vkGetTensorViewOpaqueCaptureDescriptorDataARM =
        PFN_vkGetTensorViewOpaqueCaptureDescriptorDataARM( vkGetDeviceProcAddr( device, "vkGetTensorViewOpaqueCaptureDescriptorDataARM" ) );

      //=== VK_EXT_shader_module_identifier ===
      vkGetShaderModuleIdentifierEXT = PFN_vkGetShaderModuleIdentifierEXT( vkGetDeviceProcAddr( device, "vkGetShaderModuleIdentifierEXT" ) );
      vkGetShaderModuleCreateInfoIdentifierEXT =
        PFN_vkGetShaderModuleCreateInfoIdentifierEXT( vkGetDeviceProcAddr( device, "vkGetShaderModuleCreateInfoIdentifierEXT" ) );

      //=== VK_NV_optical_flow ===
      vkCreateOpticalFlowSessionNV    = PFN_vkCreateOpticalFlowSessionNV( vkGetDeviceProcAddr( device, "vkCreateOpticalFlowSessionNV" ) );
      vkDestroyOpticalFlowSessionNV   = PFN_vkDestroyOpticalFlowSessionNV( vkGetDeviceProcAddr( device, "vkDestroyOpticalFlowSessionNV" ) );
      vkBindOpticalFlowSessionImageNV = PFN_vkBindOpticalFlowSessionImageNV( vkGetDeviceProcAddr( device, "vkBindOpticalFlowSessionImageNV" ) );
      vkCmdOpticalFlowExecuteNV       = PFN_vkCmdOpticalFlowExecuteNV( vkGetDeviceProcAddr( device, "vkCmdOpticalFlowExecuteNV" ) );

      //=== VK_KHR_maintenance5 ===
      vkCmdBindIndexBuffer2KHR = PFN_vkCmdBindIndexBuffer2KHR( vkGetDeviceProcAddr( device, "vkCmdBindIndexBuffer2KHR" ) );
      if ( !vkCmdBindIndexBuffer2 )
        vkCmdBindIndexBuffer2 = vkCmdBindIndexBuffer2KHR;
      vkGetRenderingAreaGranularityKHR = PFN_vkGetRenderingAreaGranularityKHR( vkGetDeviceProcAddr( device, "vkGetRenderingAreaGranularityKHR" ) );
      if ( !vkGetRenderingAreaGranularity )
        vkGetRenderingAreaGranularity = vkGetRenderingAreaGranularityKHR;
      vkGetDeviceImageSubresourceLayoutKHR = PFN_vkGetDeviceImageSubresourceLayoutKHR( vkGetDeviceProcAddr( device, "vkGetDeviceImageSubresourceLayoutKHR" ) );
      if ( !vkGetDeviceImageSubresourceLayout )
        vkGetDeviceImageSubresourceLayout = vkGetDeviceImageSubresourceLayoutKHR;
      vkGetImageSubresourceLayout2KHR = PFN_vkGetImageSubresourceLayout2KHR( vkGetDeviceProcAddr( device, "vkGetImageSubresourceLayout2KHR" ) );
      if ( !vkGetImageSubresourceLayout2 )
        vkGetImageSubresourceLayout2 = vkGetImageSubresourceLayout2KHR;

      //=== VK_AMD_anti_lag ===
      vkAntiLagUpdateAMD = PFN_vkAntiLagUpdateAMD( vkGetDeviceProcAddr( device, "vkAntiLagUpdateAMD" ) );

      //=== VK_KHR_present_wait2 ===
      vkWaitForPresent2KHR = PFN_vkWaitForPresent2KHR( vkGetDeviceProcAddr( device, "vkWaitForPresent2KHR" ) );

      //=== VK_EXT_shader_object ===
      vkCreateShadersEXT         = PFN_vkCreateShadersEXT( vkGetDeviceProcAddr( device, "vkCreateShadersEXT" ) );
      vkDestroyShaderEXT         = PFN_vkDestroyShaderEXT( vkGetDeviceProcAddr( device, "vkDestroyShaderEXT" ) );
      vkGetShaderBinaryDataEXT   = PFN_vkGetShaderBinaryDataEXT( vkGetDeviceProcAddr( device, "vkGetShaderBinaryDataEXT" ) );
      vkCmdBindShadersEXT        = PFN_vkCmdBindShadersEXT( vkGetDeviceProcAddr( device, "vkCmdBindShadersEXT" ) );
      vkCmdSetDepthClampRangeEXT = PFN_vkCmdSetDepthClampRangeEXT( vkGetDeviceProcAddr( device, "vkCmdSetDepthClampRangeEXT" ) );

      //=== VK_KHR_pipeline_binary ===
      vkCreatePipelineBinariesKHR      = PFN_vkCreatePipelineBinariesKHR( vkGetDeviceProcAddr( device, "vkCreatePipelineBinariesKHR" ) );
      vkDestroyPipelineBinaryKHR       = PFN_vkDestroyPipelineBinaryKHR( vkGetDeviceProcAddr( device, "vkDestroyPipelineBinaryKHR" ) );
      vkGetPipelineKeyKHR              = PFN_vkGetPipelineKeyKHR( vkGetDeviceProcAddr( device, "vkGetPipelineKeyKHR" ) );
      vkGetPipelineBinaryDataKHR       = PFN_vkGetPipelineBinaryDataKHR( vkGetDeviceProcAddr( device, "vkGetPipelineBinaryDataKHR" ) );
      vkReleaseCapturedPipelineDataKHR = PFN_vkReleaseCapturedPipelineDataKHR( vkGetDeviceProcAddr( device, "vkReleaseCapturedPipelineDataKHR" ) );

      //=== VK_QCOM_tile_properties ===
      vkGetFramebufferTilePropertiesQCOM = PFN_vkGetFramebufferTilePropertiesQCOM( vkGetDeviceProcAddr( device, "vkGetFramebufferTilePropertiesQCOM" ) );
      vkGetDynamicRenderingTilePropertiesQCOM =
        PFN_vkGetDynamicRenderingTilePropertiesQCOM( vkGetDeviceProcAddr( device, "vkGetDynamicRenderingTilePropertiesQCOM" ) );

      //=== VK_KHR_swapchain_maintenance1 ===
      vkReleaseSwapchainImagesKHR = PFN_vkReleaseSwapchainImagesKHR( vkGetDeviceProcAddr( device, "vkReleaseSwapchainImagesKHR" ) );

      //=== VK_NV_cooperative_vector ===
      vkConvertCooperativeVectorMatrixNV = PFN_vkConvertCooperativeVectorMatrixNV( vkGetDeviceProcAddr( device, "vkConvertCooperativeVectorMatrixNV" ) );
      vkCmdConvertCooperativeVectorMatrixNV =
        PFN_vkCmdConvertCooperativeVectorMatrixNV( vkGetDeviceProcAddr( device, "vkCmdConvertCooperativeVectorMatrixNV" ) );

      //=== VK_NV_low_latency2 ===
      vkSetLatencySleepModeNV  = PFN_vkSetLatencySleepModeNV( vkGetDeviceProcAddr( device, "vkSetLatencySleepModeNV" ) );
      vkLatencySleepNV         = PFN_vkLatencySleepNV( vkGetDeviceProcAddr( device, "vkLatencySleepNV" ) );
      vkSetLatencyMarkerNV     = PFN_vkSetLatencyMarkerNV( vkGetDeviceProcAddr( device, "vkSetLatencyMarkerNV" ) );
      vkGetLatencyTimingsNV    = PFN_vkGetLatencyTimingsNV( vkGetDeviceProcAddr( device, "vkGetLatencyTimingsNV" ) );
      vkQueueNotifyOutOfBandNV = PFN_vkQueueNotifyOutOfBandNV( vkGetDeviceProcAddr( device, "vkQueueNotifyOutOfBandNV" ) );

      //=== VK_ARM_data_graph ===
      vkCreateDataGraphPipelinesARM       = PFN_vkCreateDataGraphPipelinesARM( vkGetDeviceProcAddr( device, "vkCreateDataGraphPipelinesARM" ) );
      vkCreateDataGraphPipelineSessionARM = PFN_vkCreateDataGraphPipelineSessionARM( vkGetDeviceProcAddr( device, "vkCreateDataGraphPipelineSessionARM" ) );
      vkGetDataGraphPipelineSessionBindPointRequirementsARM =
        PFN_vkGetDataGraphPipelineSessionBindPointRequirementsARM( vkGetDeviceProcAddr( device, "vkGetDataGraphPipelineSessionBindPointRequirementsARM" ) );
      vkGetDataGraphPipelineSessionMemoryRequirementsARM =
        PFN_vkGetDataGraphPipelineSessionMemoryRequirementsARM( vkGetDeviceProcAddr( device, "vkGetDataGraphPipelineSessionMemoryRequirementsARM" ) );
      vkBindDataGraphPipelineSessionMemoryARM =
        PFN_vkBindDataGraphPipelineSessionMemoryARM( vkGetDeviceProcAddr( device, "vkBindDataGraphPipelineSessionMemoryARM" ) );
      vkDestroyDataGraphPipelineSessionARM = PFN_vkDestroyDataGraphPipelineSessionARM( vkGetDeviceProcAddr( device, "vkDestroyDataGraphPipelineSessionARM" ) );
      vkCmdDispatchDataGraphARM            = PFN_vkCmdDispatchDataGraphARM( vkGetDeviceProcAddr( device, "vkCmdDispatchDataGraphARM" ) );
      vkGetDataGraphPipelineAvailablePropertiesARM =
        PFN_vkGetDataGraphPipelineAvailablePropertiesARM( vkGetDeviceProcAddr( device, "vkGetDataGraphPipelineAvailablePropertiesARM" ) );
      vkGetDataGraphPipelinePropertiesARM = PFN_vkGetDataGraphPipelinePropertiesARM( vkGetDeviceProcAddr( device, "vkGetDataGraphPipelinePropertiesARM" ) );

      //=== VK_EXT_attachment_feedback_loop_dynamic_state ===
      vkCmdSetAttachmentFeedbackLoopEnableEXT =
        PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT( vkGetDeviceProcAddr( device, "vkCmdSetAttachmentFeedbackLoopEnableEXT" ) );

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
      //=== VK_QNX_external_memory_screen_buffer ===
      vkGetScreenBufferPropertiesQNX = PFN_vkGetScreenBufferPropertiesQNX( vkGetDeviceProcAddr( device, "vkGetScreenBufferPropertiesQNX" ) );
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/

      //=== VK_KHR_line_rasterization ===
      vkCmdSetLineStippleKHR = PFN_vkCmdSetLineStippleKHR( vkGetDeviceProcAddr( device, "vkCmdSetLineStippleKHR" ) );
      if ( !vkCmdSetLineStipple )
        vkCmdSetLineStipple = vkCmdSetLineStippleKHR;

      //=== VK_KHR_calibrated_timestamps ===
      vkGetCalibratedTimestampsKHR = PFN_vkGetCalibratedTimestampsKHR( vkGetDeviceProcAddr( device, "vkGetCalibratedTimestampsKHR" ) );

      //=== VK_KHR_maintenance6 ===
      vkCmdBindDescriptorSets2KHR = PFN_vkCmdBindDescriptorSets2KHR( vkGetDeviceProcAddr( device, "vkCmdBindDescriptorSets2KHR" ) );
      if ( !vkCmdBindDescriptorSets2 )
        vkCmdBindDescriptorSets2 = vkCmdBindDescriptorSets2KHR;
      vkCmdPushConstants2KHR = PFN_vkCmdPushConstants2KHR( vkGetDeviceProcAddr( device, "vkCmdPushConstants2KHR" ) );
      if ( !vkCmdPushConstants2 )
        vkCmdPushConstants2 = vkCmdPushConstants2KHR;
      vkCmdPushDescriptorSet2KHR = PFN_vkCmdPushDescriptorSet2KHR( vkGetDeviceProcAddr( device, "vkCmdPushDescriptorSet2KHR" ) );
      if ( !vkCmdPushDescriptorSet2 )
        vkCmdPushDescriptorSet2 = vkCmdPushDescriptorSet2KHR;
      vkCmdPushDescriptorSetWithTemplate2KHR =
        PFN_vkCmdPushDescriptorSetWithTemplate2KHR( vkGetDeviceProcAddr( device, "vkCmdPushDescriptorSetWithTemplate2KHR" ) );
      if ( !vkCmdPushDescriptorSetWithTemplate2 )
        vkCmdPushDescriptorSetWithTemplate2 = vkCmdPushDescriptorSetWithTemplate2KHR;
      vkCmdSetDescriptorBufferOffsets2EXT = PFN_vkCmdSetDescriptorBufferOffsets2EXT( vkGetDeviceProcAddr( device, "vkCmdSetDescriptorBufferOffsets2EXT" ) );
      vkCmdBindDescriptorBufferEmbeddedSamplers2EXT =
        PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT( vkGetDeviceProcAddr( device, "vkCmdBindDescriptorBufferEmbeddedSamplers2EXT" ) );

      //=== VK_QCOM_tile_memory_heap ===
      vkCmdBindTileMemoryQCOM = PFN_vkCmdBindTileMemoryQCOM( vkGetDeviceProcAddr( device, "vkCmdBindTileMemoryQCOM" ) );

      //=== VK_KHR_copy_memory_indirect ===
      vkCmdCopyMemoryIndirectKHR        = PFN_vkCmdCopyMemoryIndirectKHR( vkGetDeviceProcAddr( device, "vkCmdCopyMemoryIndirectKHR" ) );
      vkCmdCopyMemoryToImageIndirectKHR = PFN_vkCmdCopyMemoryToImageIndirectKHR( vkGetDeviceProcAddr( device, "vkCmdCopyMemoryToImageIndirectKHR" ) );

      //=== VK_EXT_memory_decompression ===
      vkCmdDecompressMemoryEXT = PFN_vkCmdDecompressMemoryEXT( vkGetDeviceProcAddr( device, "vkCmdDecompressMemoryEXT" ) );
      vkCmdDecompressMemoryIndirectCountEXT =
        PFN_vkCmdDecompressMemoryIndirectCountEXT( vkGetDeviceProcAddr( device, "vkCmdDecompressMemoryIndirectCountEXT" ) );

      //=== VK_NV_external_compute_queue ===
      vkCreateExternalComputeQueueNV  = PFN_vkCreateExternalComputeQueueNV( vkGetDeviceProcAddr( device, "vkCreateExternalComputeQueueNV" ) );
      vkDestroyExternalComputeQueueNV = PFN_vkDestroyExternalComputeQueueNV( vkGetDeviceProcAddr( device, "vkDestroyExternalComputeQueueNV" ) );
      vkGetExternalComputeQueueDataNV = PFN_vkGetExternalComputeQueueDataNV( vkGetDeviceProcAddr( device, "vkGetExternalComputeQueueDataNV" ) );

      //=== VK_NV_cluster_acceleration_structure ===
      vkGetClusterAccelerationStructureBuildSizesNV =
        PFN_vkGetClusterAccelerationStructureBuildSizesNV( vkGetDeviceProcAddr( device, "vkGetClusterAccelerationStructureBuildSizesNV" ) );
      vkCmdBuildClusterAccelerationStructureIndirectNV =
        PFN_vkCmdBuildClusterAccelerationStructureIndirectNV( vkGetDeviceProcAddr( device, "vkCmdBuildClusterAccelerationStructureIndirectNV" ) );

      //=== VK_NV_partitioned_acceleration_structure ===
      vkGetPartitionedAccelerationStructuresBuildSizesNV =
        PFN_vkGetPartitionedAccelerationStructuresBuildSizesNV( vkGetDeviceProcAddr( device, "vkGetPartitionedAccelerationStructuresBuildSizesNV" ) );
      vkCmdBuildPartitionedAccelerationStructuresNV =
        PFN_vkCmdBuildPartitionedAccelerationStructuresNV( vkGetDeviceProcAddr( device, "vkCmdBuildPartitionedAccelerationStructuresNV" ) );

      //=== VK_EXT_device_generated_commands ===
      vkGetGeneratedCommandsMemoryRequirementsEXT =
        PFN_vkGetGeneratedCommandsMemoryRequirementsEXT( vkGetDeviceProcAddr( device, "vkGetGeneratedCommandsMemoryRequirementsEXT" ) );
      vkCmdPreprocessGeneratedCommandsEXT = PFN_vkCmdPreprocessGeneratedCommandsEXT( vkGetDeviceProcAddr( device, "vkCmdPreprocessGeneratedCommandsEXT" ) );
      vkCmdExecuteGeneratedCommandsEXT    = PFN_vkCmdExecuteGeneratedCommandsEXT( vkGetDeviceProcAddr( device, "vkCmdExecuteGeneratedCommandsEXT" ) );
      vkCreateIndirectCommandsLayoutEXT   = PFN_vkCreateIndirectCommandsLayoutEXT( vkGetDeviceProcAddr( device, "vkCreateIndirectCommandsLayoutEXT" ) );
      vkDestroyIndirectCommandsLayoutEXT  = PFN_vkDestroyIndirectCommandsLayoutEXT( vkGetDeviceProcAddr( device, "vkDestroyIndirectCommandsLayoutEXT" ) );
      vkCreateIndirectExecutionSetEXT     = PFN_vkCreateIndirectExecutionSetEXT( vkGetDeviceProcAddr( device, "vkCreateIndirectExecutionSetEXT" ) );
      vkDestroyIndirectExecutionSetEXT    = PFN_vkDestroyIndirectExecutionSetEXT( vkGetDeviceProcAddr( device, "vkDestroyIndirectExecutionSetEXT" ) );
      vkUpdateIndirectExecutionSetPipelineEXT =
        PFN_vkUpdateIndirectExecutionSetPipelineEXT( vkGetDeviceProcAddr( device, "vkUpdateIndirectExecutionSetPipelineEXT" ) );
      vkUpdateIndirectExecutionSetShaderEXT =
        PFN_vkUpdateIndirectExecutionSetShaderEXT( vkGetDeviceProcAddr( device, "vkUpdateIndirectExecutionSetShaderEXT" ) );

#if defined( VK_USE_PLATFORM_METAL_EXT )
      //=== VK_EXT_external_memory_metal ===
      vkGetMemoryMetalHandleEXT           = PFN_vkGetMemoryMetalHandleEXT( vkGetDeviceProcAddr( device, "vkGetMemoryMetalHandleEXT" ) );
      vkGetMemoryMetalHandlePropertiesEXT = PFN_vkGetMemoryMetalHandlePropertiesEXT( vkGetDeviceProcAddr( device, "vkGetMemoryMetalHandlePropertiesEXT" ) );
#endif /*VK_USE_PLATFORM_METAL_EXT*/

      //=== VK_EXT_fragment_density_map_offset ===
      vkCmdEndRendering2EXT = PFN_vkCmdEndRendering2EXT( vkGetDeviceProcAddr( device, "vkCmdEndRendering2EXT" ) );
      if ( !vkCmdEndRendering2KHR )
        vkCmdEndRendering2KHR = vkCmdEndRendering2EXT;

      //=== VK_EXT_custom_resolve ===
      vkCmdBeginCustomResolveEXT = PFN_vkCmdBeginCustomResolveEXT( vkGetDeviceProcAddr( device, "vkCmdBeginCustomResolveEXT" ) );

      //=== VK_KHR_maintenance10 ===
      vkCmdEndRendering2KHR = PFN_vkCmdEndRendering2KHR( vkGetDeviceProcAddr( device, "vkCmdEndRendering2KHR" ) );

      //=== VK_NV_compute_occupancy_priority ===
      vkCmdSetComputeOccupancyPriorityNV = PFN_vkCmdSetComputeOccupancyPriorityNV( vkGetDeviceProcAddr( device, "vkCmdSetComputeOccupancyPriorityNV" ) );
    }

    void init( Instance instanceCpp ) VULKAN_HPP_NOEXCEPT;
    void init( Device deviceCpp ) VULKAN_HPP_NOEXCEPT;

  public:
    //=== VK_VERSION_1_0 ===
    PFN_vkCreateInstance                         vkCreateInstance                         = VULKAN_HPP_DEFAULT_ADDR( vkCreateInstance );
    PFN_vkDestroyInstance                        vkDestroyInstance                        = VULKAN_HPP_DEFAULT_ADDR( vkDestroyInstance );
    PFN_vkEnumeratePhysicalDevices               vkEnumeratePhysicalDevices               = VULKAN_HPP_DEFAULT_ADDR( vkEnumeratePhysicalDevices );
    PFN_vkGetPhysicalDeviceFeatures              vkGetPhysicalDeviceFeatures              = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceFeatures );
    PFN_vkGetPhysicalDeviceFormatProperties      vkGetPhysicalDeviceFormatProperties      = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceFormatProperties );
    PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceImageFormatProperties );
    PFN_vkGetPhysicalDeviceProperties            vkGetPhysicalDeviceProperties            = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceProperties );
    PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceQueueFamilyProperties );
    PFN_vkGetPhysicalDeviceMemoryProperties      vkGetPhysicalDeviceMemoryProperties      = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceMemoryProperties );
    PFN_vkGetInstanceProcAddr                    vkGetInstanceProcAddr                    = VULKAN_HPP_DEFAULT_ADDR( vkGetInstanceProcAddr );
    PFN_vkGetDeviceProcAddr                      vkGetDeviceProcAddr                      = VULKAN_HPP_DEFAULT_ADDR( vkGetDeviceProcAddr );
    PFN_vkCreateDevice                           vkCreateDevice                           = VULKAN_HPP_DEFAULT_ADDR( vkCreateDevice );
    PFN_vkDestroyDevice                          vkDestroyDevice                          = VULKAN_HPP_DEFAULT_ADDR( vkDestroyDevice );
    PFN_vkEnumerateInstanceExtensionProperties   vkEnumerateInstanceExtensionProperties   = VULKAN_HPP_DEFAULT_ADDR( vkEnumerateInstanceExtensionProperties );
    PFN_vkEnumerateDeviceExtensionProperties     vkEnumerateDeviceExtensionProperties     = VULKAN_HPP_DEFAULT_ADDR( vkEnumerateDeviceExtensionProperties );
    PFN_vkEnumerateInstanceLayerProperties       vkEnumerateInstanceLayerProperties       = VULKAN_HPP_DEFAULT_ADDR( vkEnumerateInstanceLayerProperties );
    PFN_vkEnumerateDeviceLayerProperties         vkEnumerateDeviceLayerProperties         = VULKAN_HPP_DEFAULT_ADDR( vkEnumerateDeviceLayerProperties );
    PFN_vkGetDeviceQueue                         vkGetDeviceQueue                         = VULKAN_HPP_DEFAULT_ADDR( vkGetDeviceQueue );
    PFN_vkQueueSubmit                            vkQueueSubmit                            = VULKAN_HPP_DEFAULT_ADDR( vkQueueSubmit );
    PFN_vkQueueWaitIdle                          vkQueueWaitIdle                          = VULKAN_HPP_DEFAULT_ADDR( vkQueueWaitIdle );
    PFN_vkDeviceWaitIdle                         vkDeviceWaitIdle                         = VULKAN_HPP_DEFAULT_ADDR( vkDeviceWaitIdle );
    PFN_vkAllocateMemory                         vkAllocateMemory                         = VULKAN_HPP_DEFAULT_ADDR( vkAllocateMemory );
    PFN_vkFreeMemory                             vkFreeMemory                             = VULKAN_HPP_DEFAULT_ADDR( vkFreeMemory );
    PFN_vkMapMemory                              vkMapMemory                              = VULKAN_HPP_DEFAULT_ADDR( vkMapMemory );
    PFN_vkUnmapMemory                            vkUnmapMemory                            = VULKAN_HPP_DEFAULT_ADDR( vkUnmapMemory );
    PFN_vkFlushMappedMemoryRanges                vkFlushMappedMemoryRanges                = VULKAN_HPP_DEFAULT_ADDR( vkFlushMappedMemoryRanges );
    PFN_vkInvalidateMappedMemoryRanges           vkInvalidateMappedMemoryRanges           = VULKAN_HPP_DEFAULT_ADDR( vkInvalidateMappedMemoryRanges );
    PFN_vkGetDeviceMemoryCommitment              vkGetDeviceMemoryCommitment              = VULKAN_HPP_DEFAULT_ADDR( vkGetDeviceMemoryCommitment );
    PFN_vkBindBufferMemory                       vkBindBufferMemory                       = VULKAN_HPP_DEFAULT_ADDR( vkBindBufferMemory );
    PFN_vkBindImageMemory                        vkBindImageMemory                        = VULKAN_HPP_DEFAULT_ADDR( vkBindImageMemory );
    PFN_vkGetBufferMemoryRequirements            vkGetBufferMemoryRequirements            = VULKAN_HPP_DEFAULT_ADDR( vkGetBufferMemoryRequirements );
    PFN_vkGetImageMemoryRequirements             vkGetImageMemoryRequirements             = VULKAN_HPP_DEFAULT_ADDR( vkGetImageMemoryRequirements );
    PFN_vkGetImageSparseMemoryRequirements       vkGetImageSparseMemoryRequirements       = VULKAN_HPP_DEFAULT_ADDR( vkGetImageSparseMemoryRequirements );
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceSparseImageFormatProperties );
    PFN_vkQueueBindSparse            vkQueueBindSparse            = VULKAN_HPP_DEFAULT_ADDR( vkQueueBindSparse );
    PFN_vkCreateFence                vkCreateFence                = VULKAN_HPP_DEFAULT_ADDR( vkCreateFence );
    PFN_vkDestroyFence               vkDestroyFence               = VULKAN_HPP_DEFAULT_ADDR( vkDestroyFence );
    PFN_vkResetFences                vkResetFences                = VULKAN_HPP_DEFAULT_ADDR( vkResetFences );
    PFN_vkGetFenceStatus             vkGetFenceStatus             = VULKAN_HPP_DEFAULT_ADDR( vkGetFenceStatus );
    PFN_vkWaitForFences              vkWaitForFences              = VULKAN_HPP_DEFAULT_ADDR( vkWaitForFences );
    PFN_vkCreateSemaphore            vkCreateSemaphore            = VULKAN_HPP_DEFAULT_ADDR( vkCreateSemaphore );
    PFN_vkDestroySemaphore           vkDestroySemaphore           = VULKAN_HPP_DEFAULT_ADDR( vkDestroySemaphore );
    PFN_vkCreateQueryPool            vkCreateQueryPool            = VULKAN_HPP_DEFAULT_ADDR( vkCreateQueryPool );
    PFN_vkDestroyQueryPool           vkDestroyQueryPool           = VULKAN_HPP_DEFAULT_ADDR( vkDestroyQueryPool );
    PFN_vkGetQueryPoolResults        vkGetQueryPoolResults        = VULKAN_HPP_DEFAULT_ADDR( vkGetQueryPoolResults );
    PFN_vkCreateBuffer               vkCreateBuffer               = VULKAN_HPP_DEFAULT_ADDR( vkCreateBuffer );
    PFN_vkDestroyBuffer              vkDestroyBuffer              = VULKAN_HPP_DEFAULT_ADDR( vkDestroyBuffer );
    PFN_vkCreateImage                vkCreateImage                = VULKAN_HPP_DEFAULT_ADDR( vkCreateImage );
    PFN_vkDestroyImage               vkDestroyImage               = VULKAN_HPP_DEFAULT_ADDR( vkDestroyImage );
    PFN_vkGetImageSubresourceLayout  vkGetImageSubresourceLayout  = VULKAN_HPP_DEFAULT_ADDR( vkGetImageSubresourceLayout );
    PFN_vkCreateImageView            vkCreateImageView            = VULKAN_HPP_DEFAULT_ADDR( vkCreateImageView );
    PFN_vkDestroyImageView           vkDestroyImageView           = VULKAN_HPP_DEFAULT_ADDR( vkDestroyImageView );
    PFN_vkCreateCommandPool          vkCreateCommandPool          = VULKAN_HPP_DEFAULT_ADDR( vkCreateCommandPool );
    PFN_vkDestroyCommandPool         vkDestroyCommandPool         = VULKAN_HPP_DEFAULT_ADDR( vkDestroyCommandPool );
    PFN_vkResetCommandPool           vkResetCommandPool           = VULKAN_HPP_DEFAULT_ADDR( vkResetCommandPool );
    PFN_vkAllocateCommandBuffers     vkAllocateCommandBuffers     = VULKAN_HPP_DEFAULT_ADDR( vkAllocateCommandBuffers );
    PFN_vkFreeCommandBuffers         vkFreeCommandBuffers         = VULKAN_HPP_DEFAULT_ADDR( vkFreeCommandBuffers );
    PFN_vkBeginCommandBuffer         vkBeginCommandBuffer         = VULKAN_HPP_DEFAULT_ADDR( vkBeginCommandBuffer );
    PFN_vkEndCommandBuffer           vkEndCommandBuffer           = VULKAN_HPP_DEFAULT_ADDR( vkEndCommandBuffer );
    PFN_vkResetCommandBuffer         vkResetCommandBuffer         = VULKAN_HPP_DEFAULT_ADDR( vkResetCommandBuffer );
    PFN_vkCmdCopyBuffer              vkCmdCopyBuffer              = VULKAN_HPP_DEFAULT_ADDR( vkCmdCopyBuffer );
    PFN_vkCmdCopyImage               vkCmdCopyImage               = VULKAN_HPP_DEFAULT_ADDR( vkCmdCopyImage );
    PFN_vkCmdCopyBufferToImage       vkCmdCopyBufferToImage       = VULKAN_HPP_DEFAULT_ADDR( vkCmdCopyBufferToImage );
    PFN_vkCmdCopyImageToBuffer       vkCmdCopyImageToBuffer       = VULKAN_HPP_DEFAULT_ADDR( vkCmdCopyImageToBuffer );
    PFN_vkCmdUpdateBuffer            vkCmdUpdateBuffer            = VULKAN_HPP_DEFAULT_ADDR( vkCmdUpdateBuffer );
    PFN_vkCmdFillBuffer              vkCmdFillBuffer              = VULKAN_HPP_DEFAULT_ADDR( vkCmdFillBuffer );
    PFN_vkCmdPipelineBarrier         vkCmdPipelineBarrier         = VULKAN_HPP_DEFAULT_ADDR( vkCmdPipelineBarrier );
    PFN_vkCmdBeginQuery              vkCmdBeginQuery              = VULKAN_HPP_DEFAULT_ADDR( vkCmdBeginQuery );
    PFN_vkCmdEndQuery                vkCmdEndQuery                = VULKAN_HPP_DEFAULT_ADDR( vkCmdEndQuery );
    PFN_vkCmdResetQueryPool          vkCmdResetQueryPool          = VULKAN_HPP_DEFAULT_ADDR( vkCmdResetQueryPool );
    PFN_vkCmdWriteTimestamp          vkCmdWriteTimestamp          = VULKAN_HPP_DEFAULT_ADDR( vkCmdWriteTimestamp );
    PFN_vkCmdCopyQueryPoolResults    vkCmdCopyQueryPoolResults    = VULKAN_HPP_DEFAULT_ADDR( vkCmdCopyQueryPoolResults );
    PFN_vkCmdExecuteCommands         vkCmdExecuteCommands         = VULKAN_HPP_DEFAULT_ADDR( vkCmdExecuteCommands );
    PFN_vkCreateEvent                vkCreateEvent                = VULKAN_HPP_DEFAULT_ADDR( vkCreateEvent );
    PFN_vkDestroyEvent               vkDestroyEvent               = VULKAN_HPP_DEFAULT_ADDR( vkDestroyEvent );
    PFN_vkGetEventStatus             vkGetEventStatus             = VULKAN_HPP_DEFAULT_ADDR( vkGetEventStatus );
    PFN_vkSetEvent                   vkSetEvent                   = VULKAN_HPP_DEFAULT_ADDR( vkSetEvent );
    PFN_vkResetEvent                 vkResetEvent                 = VULKAN_HPP_DEFAULT_ADDR( vkResetEvent );
    PFN_vkCreateBufferView           vkCreateBufferView           = VULKAN_HPP_DEFAULT_ADDR( vkCreateBufferView );
    PFN_vkDestroyBufferView          vkDestroyBufferView          = VULKAN_HPP_DEFAULT_ADDR( vkDestroyBufferView );
    PFN_vkCreateShaderModule         vkCreateShaderModule         = VULKAN_HPP_DEFAULT_ADDR( vkCreateShaderModule );
    PFN_vkDestroyShaderModule        vkDestroyShaderModule        = VULKAN_HPP_DEFAULT_ADDR( vkDestroyShaderModule );
    PFN_vkCreatePipelineCache        vkCreatePipelineCache        = VULKAN_HPP_DEFAULT_ADDR( vkCreatePipelineCache );
    PFN_vkDestroyPipelineCache       vkDestroyPipelineCache       = VULKAN_HPP_DEFAULT_ADDR( vkDestroyPipelineCache );
    PFN_vkGetPipelineCacheData       vkGetPipelineCacheData       = VULKAN_HPP_DEFAULT_ADDR( vkGetPipelineCacheData );
    PFN_vkMergePipelineCaches        vkMergePipelineCaches        = VULKAN_HPP_DEFAULT_ADDR( vkMergePipelineCaches );
    PFN_vkCreateComputePipelines     vkCreateComputePipelines     = VULKAN_HPP_DEFAULT_ADDR( vkCreateComputePipelines );
    PFN_vkDestroyPipeline            vkDestroyPipeline            = VULKAN_HPP_DEFAULT_ADDR( vkDestroyPipeline );
    PFN_vkCreatePipelineLayout       vkCreatePipelineLayout       = VULKAN_HPP_DEFAULT_ADDR( vkCreatePipelineLayout );
    PFN_vkDestroyPipelineLayout      vkDestroyPipelineLayout      = VULKAN_HPP_DEFAULT_ADDR( vkDestroyPipelineLayout );
    PFN_vkCreateSampler              vkCreateSampler              = VULKAN_HPP_DEFAULT_ADDR( vkCreateSampler );
    PFN_vkDestroySampler             vkDestroySampler             = VULKAN_HPP_DEFAULT_ADDR( vkDestroySampler );
    PFN_vkCreateDescriptorSetLayout  vkCreateDescriptorSetLayout  = VULKAN_HPP_DEFAULT_ADDR( vkCreateDescriptorSetLayout );
    PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout = VULKAN_HPP_DEFAULT_ADDR( vkDestroyDescriptorSetLayout );
    PFN_vkCreateDescriptorPool       vkCreateDescriptorPool       = VULKAN_HPP_DEFAULT_ADDR( vkCreateDescriptorPool );
    PFN_vkDestroyDescriptorPool      vkDestroyDescriptorPool      = VULKAN_HPP_DEFAULT_ADDR( vkDestroyDescriptorPool );
    PFN_vkResetDescriptorPool        vkResetDescriptorPool        = VULKAN_HPP_DEFAULT_ADDR( vkResetDescriptorPool );
    PFN_vkAllocateDescriptorSets     vkAllocateDescriptorSets     = VULKAN_HPP_DEFAULT_ADDR( vkAllocateDescriptorSets );
    PFN_vkFreeDescriptorSets         vkFreeDescriptorSets         = VULKAN_HPP_DEFAULT_ADDR( vkFreeDescriptorSets );
    PFN_vkUpdateDescriptorSets       vkUpdateDescriptorSets       = VULKAN_HPP_DEFAULT_ADDR( vkUpdateDescriptorSets );
    PFN_vkCmdBindPipeline            vkCmdBindPipeline            = VULKAN_HPP_DEFAULT_ADDR( vkCmdBindPipeline );
    PFN_vkCmdBindDescriptorSets      vkCmdBindDescriptorSets      = VULKAN_HPP_DEFAULT_ADDR( vkCmdBindDescriptorSets );
    PFN_vkCmdClearColorImage         vkCmdClearColorImage         = VULKAN_HPP_DEFAULT_ADDR( vkCmdClearColorImage );
    PFN_vkCmdDispatch                vkCmdDispatch                = VULKAN_HPP_DEFAULT_ADDR( vkCmdDispatch );
    PFN_vkCmdDispatchIndirect        vkCmdDispatchIndirect        = VULKAN_HPP_DEFAULT_ADDR( vkCmdDispatchIndirect );
    PFN_vkCmdSetEvent                vkCmdSetEvent                = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetEvent );
    PFN_vkCmdResetEvent              vkCmdResetEvent              = VULKAN_HPP_DEFAULT_ADDR( vkCmdResetEvent );
    PFN_vkCmdWaitEvents              vkCmdWaitEvents              = VULKAN_HPP_DEFAULT_ADDR( vkCmdWaitEvents );
    PFN_vkCmdPushConstants           vkCmdPushConstants           = VULKAN_HPP_DEFAULT_ADDR( vkCmdPushConstants );
    PFN_vkCreateGraphicsPipelines    vkCreateGraphicsPipelines    = VULKAN_HPP_DEFAULT_ADDR( vkCreateGraphicsPipelines );
    PFN_vkCreateFramebuffer          vkCreateFramebuffer          = VULKAN_HPP_DEFAULT_ADDR( vkCreateFramebuffer );
    PFN_vkDestroyFramebuffer         vkDestroyFramebuffer         = VULKAN_HPP_DEFAULT_ADDR( vkDestroyFramebuffer );
    PFN_vkCreateRenderPass           vkCreateRenderPass           = VULKAN_HPP_DEFAULT_ADDR( vkCreateRenderPass );
    PFN_vkDestroyRenderPass          vkDestroyRenderPass          = VULKAN_HPP_DEFAULT_ADDR( vkDestroyRenderPass );
    PFN_vkGetRenderAreaGranularity   vkGetRenderAreaGranularity   = VULKAN_HPP_DEFAULT_ADDR( vkGetRenderAreaGranularity );
    PFN_vkCmdSetViewport             vkCmdSetViewport             = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetViewport );
    PFN_vkCmdSetScissor              vkCmdSetScissor              = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetScissor );
    PFN_vkCmdSetLineWidth            vkCmdSetLineWidth            = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetLineWidth );
    PFN_vkCmdSetDepthBias            vkCmdSetDepthBias            = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetDepthBias );
    PFN_vkCmdSetBlendConstants       vkCmdSetBlendConstants       = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetBlendConstants );
    PFN_vkCmdSetDepthBounds          vkCmdSetDepthBounds          = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetDepthBounds );
    PFN_vkCmdSetStencilCompareMask   vkCmdSetStencilCompareMask   = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetStencilCompareMask );
    PFN_vkCmdSetStencilWriteMask     vkCmdSetStencilWriteMask     = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetStencilWriteMask );
    PFN_vkCmdSetStencilReference     vkCmdSetStencilReference     = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetStencilReference );
    PFN_vkCmdBindIndexBuffer         vkCmdBindIndexBuffer         = VULKAN_HPP_DEFAULT_ADDR( vkCmdBindIndexBuffer );
    PFN_vkCmdBindVertexBuffers       vkCmdBindVertexBuffers       = VULKAN_HPP_DEFAULT_ADDR( vkCmdBindVertexBuffers );
    PFN_vkCmdDraw                    vkCmdDraw                    = VULKAN_HPP_DEFAULT_ADDR( vkCmdDraw );
    PFN_vkCmdDrawIndexed             vkCmdDrawIndexed             = VULKAN_HPP_DEFAULT_ADDR( vkCmdDrawIndexed );
    PFN_vkCmdDrawIndirect            vkCmdDrawIndirect            = VULKAN_HPP_DEFAULT_ADDR( vkCmdDrawIndirect );
    PFN_vkCmdDrawIndexedIndirect     vkCmdDrawIndexedIndirect     = VULKAN_HPP_DEFAULT_ADDR( vkCmdDrawIndexedIndirect );
    PFN_vkCmdBlitImage               vkCmdBlitImage               = VULKAN_HPP_DEFAULT_ADDR( vkCmdBlitImage );
    PFN_vkCmdClearDepthStencilImage  vkCmdClearDepthStencilImage  = VULKAN_HPP_DEFAULT_ADDR( vkCmdClearDepthStencilImage );
    PFN_vkCmdClearAttachments        vkCmdClearAttachments        = VULKAN_HPP_DEFAULT_ADDR( vkCmdClearAttachments );
    PFN_vkCmdResolveImage            vkCmdResolveImage            = VULKAN_HPP_DEFAULT_ADDR( vkCmdResolveImage );
    PFN_vkCmdBeginRenderPass         vkCmdBeginRenderPass         = VULKAN_HPP_DEFAULT_ADDR( vkCmdBeginRenderPass );
    PFN_vkCmdNextSubpass             vkCmdNextSubpass             = VULKAN_HPP_DEFAULT_ADDR( vkCmdNextSubpass );
    PFN_vkCmdEndRenderPass           vkCmdEndRenderPass           = VULKAN_HPP_DEFAULT_ADDR( vkCmdEndRenderPass );

    //=== VK_VERSION_1_1 ===
    PFN_vkEnumerateInstanceVersion                vkEnumerateInstanceVersion           = VULKAN_HPP_DEFAULT_ADDR( vkEnumerateInstanceVersion );
    PFN_vkBindBufferMemory2                       vkBindBufferMemory2                  = VULKAN_HPP_DEFAULT_ADDR( vkBindBufferMemory2 );
    PFN_vkBindImageMemory2                        vkBindImageMemory2                   = VULKAN_HPP_DEFAULT_ADDR( vkBindImageMemory2 );
    PFN_vkGetDeviceGroupPeerMemoryFeatures        vkGetDeviceGroupPeerMemoryFeatures   = VULKAN_HPP_DEFAULT_ADDR( vkGetDeviceGroupPeerMemoryFeatures );
    PFN_vkCmdSetDeviceMask                        vkCmdSetDeviceMask                   = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetDeviceMask );
    PFN_vkEnumeratePhysicalDeviceGroups           vkEnumeratePhysicalDeviceGroups      = VULKAN_HPP_DEFAULT_ADDR( vkEnumeratePhysicalDeviceGroups );
    PFN_vkGetImageMemoryRequirements2             vkGetImageMemoryRequirements2        = VULKAN_HPP_DEFAULT_ADDR( vkGetImageMemoryRequirements2 );
    PFN_vkGetBufferMemoryRequirements2            vkGetBufferMemoryRequirements2       = VULKAN_HPP_DEFAULT_ADDR( vkGetBufferMemoryRequirements2 );
    PFN_vkGetImageSparseMemoryRequirements2       vkGetImageSparseMemoryRequirements2  = VULKAN_HPP_DEFAULT_ADDR( vkGetImageSparseMemoryRequirements2 );
    PFN_vkGetPhysicalDeviceFeatures2              vkGetPhysicalDeviceFeatures2         = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceFeatures2 );
    PFN_vkGetPhysicalDeviceProperties2            vkGetPhysicalDeviceProperties2       = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceProperties2 );
    PFN_vkGetPhysicalDeviceFormatProperties2      vkGetPhysicalDeviceFormatProperties2 = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceFormatProperties2 );
    PFN_vkGetPhysicalDeviceImageFormatProperties2 vkGetPhysicalDeviceImageFormatProperties2 =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceImageFormatProperties2 );
    PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2 =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceQueueFamilyProperties2 );
    PFN_vkGetPhysicalDeviceMemoryProperties2            vkGetPhysicalDeviceMemoryProperties2 = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceMemoryProperties2 );
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 vkGetPhysicalDeviceSparseImageFormatProperties2 =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceSparseImageFormatProperties2 );
    PFN_vkTrimCommandPool                           vkTrimCommandPool = VULKAN_HPP_DEFAULT_ADDR( vkTrimCommandPool );
    PFN_vkGetDeviceQueue2                           vkGetDeviceQueue2 = VULKAN_HPP_DEFAULT_ADDR( vkGetDeviceQueue2 );
    PFN_vkGetPhysicalDeviceExternalBufferProperties vkGetPhysicalDeviceExternalBufferProperties =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceExternalBufferProperties );
    PFN_vkGetPhysicalDeviceExternalFenceProperties vkGetPhysicalDeviceExternalFenceProperties =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceExternalFenceProperties );
    PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceExternalSemaphoreProperties );
    PFN_vkCmdDispatchBase                 vkCmdDispatchBase                 = VULKAN_HPP_DEFAULT_ADDR( vkCmdDispatchBase );
    PFN_vkCreateDescriptorUpdateTemplate  vkCreateDescriptorUpdateTemplate  = VULKAN_HPP_DEFAULT_ADDR( vkCreateDescriptorUpdateTemplate );
    PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate = VULKAN_HPP_DEFAULT_ADDR( vkDestroyDescriptorUpdateTemplate );
    PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate = VULKAN_HPP_DEFAULT_ADDR( vkUpdateDescriptorSetWithTemplate );
    PFN_vkGetDescriptorSetLayoutSupport   vkGetDescriptorSetLayoutSupport   = VULKAN_HPP_DEFAULT_ADDR( vkGetDescriptorSetLayoutSupport );
    PFN_vkCreateSamplerYcbcrConversion    vkCreateSamplerYcbcrConversion    = VULKAN_HPP_DEFAULT_ADDR( vkCreateSamplerYcbcrConversion );
    PFN_vkDestroySamplerYcbcrConversion   vkDestroySamplerYcbcrConversion   = VULKAN_HPP_DEFAULT_ADDR( vkDestroySamplerYcbcrConversion );

    //=== VK_VERSION_1_2 ===
    PFN_vkResetQueryPool                      vkResetQueryPool                      = VULKAN_HPP_DEFAULT_ADDR( vkResetQueryPool );
    PFN_vkGetSemaphoreCounterValue            vkGetSemaphoreCounterValue            = VULKAN_HPP_DEFAULT_ADDR( vkGetSemaphoreCounterValue );
    PFN_vkWaitSemaphores                      vkWaitSemaphores                      = VULKAN_HPP_DEFAULT_ADDR( vkWaitSemaphores );
    PFN_vkSignalSemaphore                     vkSignalSemaphore                     = VULKAN_HPP_DEFAULT_ADDR( vkSignalSemaphore );
    PFN_vkGetBufferDeviceAddress              vkGetBufferDeviceAddress              = VULKAN_HPP_DEFAULT_ADDR( vkGetBufferDeviceAddress );
    PFN_vkGetBufferOpaqueCaptureAddress       vkGetBufferOpaqueCaptureAddress       = VULKAN_HPP_DEFAULT_ADDR( vkGetBufferOpaqueCaptureAddress );
    PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress = VULKAN_HPP_DEFAULT_ADDR( vkGetDeviceMemoryOpaqueCaptureAddress );
    PFN_vkCmdDrawIndirectCount                vkCmdDrawIndirectCount                = VULKAN_HPP_DEFAULT_ADDR( vkCmdDrawIndirectCount );
    PFN_vkCmdDrawIndexedIndirectCount         vkCmdDrawIndexedIndirectCount         = VULKAN_HPP_DEFAULT_ADDR( vkCmdDrawIndexedIndirectCount );
    PFN_vkCreateRenderPass2                   vkCreateRenderPass2                   = VULKAN_HPP_DEFAULT_ADDR( vkCreateRenderPass2 );
    PFN_vkCmdBeginRenderPass2                 vkCmdBeginRenderPass2                 = VULKAN_HPP_DEFAULT_ADDR( vkCmdBeginRenderPass2 );
    PFN_vkCmdNextSubpass2                     vkCmdNextSubpass2                     = VULKAN_HPP_DEFAULT_ADDR( vkCmdNextSubpass2 );
    PFN_vkCmdEndRenderPass2                   vkCmdEndRenderPass2                   = VULKAN_HPP_DEFAULT_ADDR( vkCmdEndRenderPass2 );

    //=== VK_VERSION_1_3 ===
    PFN_vkGetPhysicalDeviceToolProperties        vkGetPhysicalDeviceToolProperties        = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceToolProperties );
    PFN_vkCreatePrivateDataSlot                  vkCreatePrivateDataSlot                  = VULKAN_HPP_DEFAULT_ADDR( vkCreatePrivateDataSlot );
    PFN_vkDestroyPrivateDataSlot                 vkDestroyPrivateDataSlot                 = VULKAN_HPP_DEFAULT_ADDR( vkDestroyPrivateDataSlot );
    PFN_vkSetPrivateData                         vkSetPrivateData                         = VULKAN_HPP_DEFAULT_ADDR( vkSetPrivateData );
    PFN_vkGetPrivateData                         vkGetPrivateData                         = VULKAN_HPP_DEFAULT_ADDR( vkGetPrivateData );
    PFN_vkCmdPipelineBarrier2                    vkCmdPipelineBarrier2                    = VULKAN_HPP_DEFAULT_ADDR( vkCmdPipelineBarrier2 );
    PFN_vkCmdWriteTimestamp2                     vkCmdWriteTimestamp2                     = VULKAN_HPP_DEFAULT_ADDR( vkCmdWriteTimestamp2 );
    PFN_vkQueueSubmit2                           vkQueueSubmit2                           = VULKAN_HPP_DEFAULT_ADDR( vkQueueSubmit2 );
    PFN_vkCmdCopyBuffer2                         vkCmdCopyBuffer2                         = VULKAN_HPP_DEFAULT_ADDR( vkCmdCopyBuffer2 );
    PFN_vkCmdCopyImage2                          vkCmdCopyImage2                          = VULKAN_HPP_DEFAULT_ADDR( vkCmdCopyImage2 );
    PFN_vkCmdCopyBufferToImage2                  vkCmdCopyBufferToImage2                  = VULKAN_HPP_DEFAULT_ADDR( vkCmdCopyBufferToImage2 );
    PFN_vkCmdCopyImageToBuffer2                  vkCmdCopyImageToBuffer2                  = VULKAN_HPP_DEFAULT_ADDR( vkCmdCopyImageToBuffer2 );
    PFN_vkGetDeviceBufferMemoryRequirements      vkGetDeviceBufferMemoryRequirements      = VULKAN_HPP_DEFAULT_ADDR( vkGetDeviceBufferMemoryRequirements );
    PFN_vkGetDeviceImageMemoryRequirements       vkGetDeviceImageMemoryRequirements       = VULKAN_HPP_DEFAULT_ADDR( vkGetDeviceImageMemoryRequirements );
    PFN_vkGetDeviceImageSparseMemoryRequirements vkGetDeviceImageSparseMemoryRequirements = VULKAN_HPP_DEFAULT_ADDR( vkGetDeviceImageSparseMemoryRequirements );
    PFN_vkCmdSetEvent2                           vkCmdSetEvent2                           = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetEvent2 );
    PFN_vkCmdResetEvent2                         vkCmdResetEvent2                         = VULKAN_HPP_DEFAULT_ADDR( vkCmdResetEvent2 );
    PFN_vkCmdWaitEvents2                         vkCmdWaitEvents2                         = VULKAN_HPP_DEFAULT_ADDR( vkCmdWaitEvents2 );
    PFN_vkCmdBlitImage2                          vkCmdBlitImage2                          = VULKAN_HPP_DEFAULT_ADDR( vkCmdBlitImage2 );
    PFN_vkCmdResolveImage2                       vkCmdResolveImage2                       = VULKAN_HPP_DEFAULT_ADDR( vkCmdResolveImage2 );
    PFN_vkCmdBeginRendering                      vkCmdBeginRendering                      = VULKAN_HPP_DEFAULT_ADDR( vkCmdBeginRendering );
    PFN_vkCmdEndRendering                        vkCmdEndRendering                        = VULKAN_HPP_DEFAULT_ADDR( vkCmdEndRendering );
    PFN_vkCmdSetCullMode                         vkCmdSetCullMode                         = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetCullMode );
    PFN_vkCmdSetFrontFace                        vkCmdSetFrontFace                        = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetFrontFace );
    PFN_vkCmdSetPrimitiveTopology                vkCmdSetPrimitiveTopology                = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetPrimitiveTopology );
    PFN_vkCmdSetViewportWithCount                vkCmdSetViewportWithCount                = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetViewportWithCount );
    PFN_vkCmdSetScissorWithCount                 vkCmdSetScissorWithCount                 = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetScissorWithCount );
    PFN_vkCmdBindVertexBuffers2                  vkCmdBindVertexBuffers2                  = VULKAN_HPP_DEFAULT_ADDR( vkCmdBindVertexBuffers2 );
    PFN_vkCmdSetDepthTestEnable                  vkCmdSetDepthTestEnable                  = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetDepthTestEnable );
    PFN_vkCmdSetDepthWriteEnable                 vkCmdSetDepthWriteEnable                 = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetDepthWriteEnable );
    PFN_vkCmdSetDepthCompareOp                   vkCmdSetDepthCompareOp                   = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetDepthCompareOp );
    PFN_vkCmdSetDepthBoundsTestEnable            vkCmdSetDepthBoundsTestEnable            = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetDepthBoundsTestEnable );
    PFN_vkCmdSetStencilTestEnable                vkCmdSetStencilTestEnable                = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetStencilTestEnable );
    PFN_vkCmdSetStencilOp                        vkCmdSetStencilOp                        = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetStencilOp );
    PFN_vkCmdSetRasterizerDiscardEnable          vkCmdSetRasterizerDiscardEnable          = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetRasterizerDiscardEnable );
    PFN_vkCmdSetDepthBiasEnable                  vkCmdSetDepthBiasEnable                  = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetDepthBiasEnable );
    PFN_vkCmdSetPrimitiveRestartEnable           vkCmdSetPrimitiveRestartEnable           = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetPrimitiveRestartEnable );

    //=== VK_VERSION_1_4 ===
    PFN_vkMapMemory2                            vkMapMemory2                            = VULKAN_HPP_DEFAULT_ADDR( vkMapMemory2 );
    PFN_vkUnmapMemory2                          vkUnmapMemory2                          = VULKAN_HPP_DEFAULT_ADDR( vkUnmapMemory2 );
    PFN_vkGetDeviceImageSubresourceLayout       vkGetDeviceImageSubresourceLayout       = VULKAN_HPP_DEFAULT_ADDR( vkGetDeviceImageSubresourceLayout );
    PFN_vkGetImageSubresourceLayout2            vkGetImageSubresourceLayout2            = VULKAN_HPP_DEFAULT_ADDR( vkGetImageSubresourceLayout2 );
    PFN_vkCopyMemoryToImage                     vkCopyMemoryToImage                     = VULKAN_HPP_DEFAULT_ADDR( vkCopyMemoryToImage );
    PFN_vkCopyImageToMemory                     vkCopyImageToMemory                     = VULKAN_HPP_DEFAULT_ADDR( vkCopyImageToMemory );
    PFN_vkCopyImageToImage                      vkCopyImageToImage                      = VULKAN_HPP_DEFAULT_ADDR( vkCopyImageToImage );
    PFN_vkTransitionImageLayout                 vkTransitionImageLayout                 = VULKAN_HPP_DEFAULT_ADDR( vkTransitionImageLayout );
    PFN_vkCmdPushDescriptorSet                  vkCmdPushDescriptorSet                  = VULKAN_HPP_DEFAULT_ADDR( vkCmdPushDescriptorSet );
    PFN_vkCmdPushDescriptorSetWithTemplate      vkCmdPushDescriptorSetWithTemplate      = VULKAN_HPP_DEFAULT_ADDR( vkCmdPushDescriptorSetWithTemplate );
    PFN_vkCmdBindDescriptorSets2                vkCmdBindDescriptorSets2                = VULKAN_HPP_DEFAULT_ADDR( vkCmdBindDescriptorSets2 );
    PFN_vkCmdPushConstants2                     vkCmdPushConstants2                     = VULKAN_HPP_DEFAULT_ADDR( vkCmdPushConstants2 );
    PFN_vkCmdPushDescriptorSet2                 vkCmdPushDescriptorSet2                 = VULKAN_HPP_DEFAULT_ADDR( vkCmdPushDescriptorSet2 );
    PFN_vkCmdPushDescriptorSetWithTemplate2     vkCmdPushDescriptorSetWithTemplate2     = VULKAN_HPP_DEFAULT_ADDR( vkCmdPushDescriptorSetWithTemplate2 );
    PFN_vkCmdSetLineStipple                     vkCmdSetLineStipple                     = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetLineStipple );
    PFN_vkCmdBindIndexBuffer2                   vkCmdBindIndexBuffer2                   = VULKAN_HPP_DEFAULT_ADDR( vkCmdBindIndexBuffer2 );
    PFN_vkGetRenderingAreaGranularity           vkGetRenderingAreaGranularity           = VULKAN_HPP_DEFAULT_ADDR( vkGetRenderingAreaGranularity );
    PFN_vkCmdSetRenderingAttachmentLocations    vkCmdSetRenderingAttachmentLocations    = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetRenderingAttachmentLocations );
    PFN_vkCmdSetRenderingInputAttachmentIndices vkCmdSetRenderingInputAttachmentIndices = VULKAN_HPP_DEFAULT_ADDR( vkCmdSetRenderingInputAttachmentIndices );

    //=== VK_KHR_surface ===
    PFN_vkDestroySurfaceKHR                       vkDestroySurfaceKHR                  = VULKAN_HPP_DEFAULT_ADDR( vkDestroySurfaceKHR );
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR      vkGetPhysicalDeviceSurfaceSupportKHR = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceSurfaceSupportKHR );
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceSurfaceCapabilitiesKHR );
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR      vkGetPhysicalDeviceSurfaceFormatsKHR = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceSurfaceFormatsKHR );
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceSurfacePresentModesKHR );

    //=== VK_KHR_swapchain ===
    PFN_vkCreateSwapchainKHR                    vkCreateSwapchainKHR                    = VULKAN_HPP_DEFAULT_ADDR( vkCreateSwapchainKHR );
    PFN_vkDestroySwapchainKHR                   vkDestroySwapchainKHR                   = VULKAN_HPP_DEFAULT_ADDR( vkDestroySwapchainKHR );
    PFN_vkGetSwapchainImagesKHR                 vkGetSwapchainImagesKHR                 = VULKAN_HPP_DEFAULT_ADDR( vkGetSwapchainImagesKHR );
    PFN_vkAcquireNextImageKHR                   vkAcquireNextImageKHR                   = VULKAN_HPP_DEFAULT_ADDR( vkAcquireNextImageKHR );
    PFN_vkQueuePresentKHR                       vkQueuePresentKHR                       = VULKAN_HPP_DEFAULT_ADDR( vkQueuePresentKHR );
    PFN_vkGetDeviceGroupPresentCapabilitiesKHR  vkGetDeviceGroupPresentCapabilitiesKHR  = VULKAN_HPP_DEFAULT_ADDR( vkGetDeviceGroupPresentCapabilitiesKHR );
    PFN_vkGetDeviceGroupSurfacePresentModesKHR  vkGetDeviceGroupSurfacePresentModesKHR  = VULKAN_HPP_DEFAULT_ADDR( vkGetDeviceGroupSurfacePresentModesKHR );
    PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDevicePresentRectanglesKHR );
    PFN_vkAcquireNextImage2KHR                  vkAcquireNextImage2KHR                  = VULKAN_HPP_DEFAULT_ADDR( vkAcquireNextImage2KHR );

    //=== VK_KHR_display ===
    PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceDisplayPropertiesKHR );
    PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceDisplayPlanePropertiesKHR );
    PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR = VULKAN_HPP_DEFAULT_ADDR( vkGetDisplayPlaneSupportedDisplaysKHR );
    PFN_vkGetDisplayModePropertiesKHR         vkGetDisplayModePropertiesKHR         = VULKAN_HPP_DEFAULT_ADDR( vkGetDisplayModePropertiesKHR );
    PFN_vkCreateDisplayModeKHR                vkCreateDisplayModeKHR                = VULKAN_HPP_DEFAULT_ADDR( vkCreateDisplayModeKHR );
    PFN_vkGetDisplayPlaneCapabilitiesKHR      vkGetDisplayPlaneCapabilitiesKHR      = VULKAN_HPP_DEFAULT_ADDR( vkGetDisplayPlaneCapabilitiesKHR );
    PFN_vkCreateDisplayPlaneSurfaceKHR        vkCreateDisplayPlaneSurfaceKHR        = VULKAN_HPP_DEFAULT_ADDR( vkCreateDisplayPlaneSurfaceKHR );

    //=== VK_KHR_display_swapchain ===
    PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR = VULKAN_HPP_DEFAULT_ADDR( vkCreateSharedSwapchainsKHR );

#if defined( VK_USE_PLATFORM_XLIB_KHR )
    //=== VK_KHR_xlib_surface ===
    PFN_vkCreateXlibSurfaceKHR                        vkCreateXlibSurfaceKHR = VULKAN_HPP_DEFAULT_ADDR( vkCreateXlibSurfaceKHR );
    PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceXlibPresentationSupportKHR );
#else
    PFN_dummy vkCreateXlibSurfaceKHR_placeholder                        = 0;
    PFN_dummy vkGetPhysicalDeviceXlibPresentationSupportKHR_placeholder = 0;
#endif /*VK_USE_PLATFORM_XLIB_KHR*/

#if defined( VK_USE_PLATFORM_XCB_KHR )
    //=== VK_KHR_xcb_surface ===
    PFN_vkCreateXcbSurfaceKHR                        vkCreateXcbSurfaceKHR = VULKAN_HPP_DEFAULT_ADDR( vkCreateXcbSurfaceKHR );
    PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceXcbPresentationSupportKHR );
#else
    PFN_dummy vkCreateXcbSurfaceKHR_placeholder                        = 0;
    PFN_dummy vkGetPhysicalDeviceXcbPresentationSupportKHR_placeholder = 0;
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#if defined( VK_USE_PLATFORM_WAYLAND_KHR )
    //=== VK_KHR_wayland_surface ===
    PFN_vkCreateWaylandSurfaceKHR                        vkCreateWaylandSurfaceKHR = VULKAN_HPP_DEFAULT_ADDR( vkCreateWaylandSurfaceKHR );
    PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceWaylandPresentationSupportKHR );
#else
    PFN_dummy vkCreateWaylandSurfaceKHR_placeholder                        = 0;
    PFN_dummy vkGetPhysicalDeviceWaylandPresentationSupportKHR_placeholder = 0;
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
    //=== VK_KHR_android_surface ===
    PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR = VULKAN_HPP_DEFAULT_ADDR( vkCreateAndroidSurfaceKHR );
#else
    PFN_dummy vkCreateAndroidSurfaceKHR_placeholder = 0;
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#if defined( VK_USE_PLATFORM_WIN32_KHR )
    //=== VK_KHR_win32_surface ===
    PFN_vkCreateWin32SurfaceKHR                        vkCreateWin32SurfaceKHR = VULKAN_HPP_DEFAULT_ADDR( vkCreateWin32SurfaceKHR );
    PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceWin32PresentationSupportKHR );
#else
    PFN_dummy vkCreateWin32SurfaceKHR_placeholder                        = 0;
    PFN_dummy vkGetPhysicalDeviceWin32PresentationSupportKHR_placeholder = 0;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

    //=== VK_EXT_debug_report ===
    PFN_vkCreateDebugReportCallbackEXT  vkCreateDebugReportCallbackEXT  = 0;
    PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = 0;
    PFN_vkDebugReportMessageEXT         vkDebugReportMessageEXT         = 0;

    //=== VK_EXT_debug_marker ===
    PFN_vkDebugMarkerSetObjectTagEXT  vkDebugMarkerSetObjectTagEXT  = 0;
    PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT = 0;
    PFN_vkCmdDebugMarkerBeginEXT      vkCmdDebugMarkerBeginEXT      = 0;
    PFN_vkCmdDebugMarkerEndEXT        vkCmdDebugMarkerEndEXT        = 0;
    PFN_vkCmdDebugMarkerInsertEXT     vkCmdDebugMarkerInsertEXT     = 0;

    //=== VK_KHR_video_queue ===
    PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR     vkGetPhysicalDeviceVideoCapabilitiesKHR     = 0;
    PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR vkGetPhysicalDeviceVideoFormatPropertiesKHR = 0;
    PFN_vkCreateVideoSessionKHR                     vkCreateVideoSessionKHR                     = 0;
    PFN_vkDestroyVideoSessionKHR                    vkDestroyVideoSessionKHR                    = 0;
    PFN_vkGetVideoSessionMemoryRequirementsKHR      vkGetVideoSessionMemoryRequirementsKHR      = 0;
    PFN_vkBindVideoSessionMemoryKHR                 vkBindVideoSessionMemoryKHR                 = 0;
    PFN_vkCreateVideoSessionParametersKHR           vkCreateVideoSessionParametersKHR           = 0;
    PFN_vkUpdateVideoSessionParametersKHR           vkUpdateVideoSessionParametersKHR           = 0;
    PFN_vkDestroyVideoSessionParametersKHR          vkDestroyVideoSessionParametersKHR          = 0;
    PFN_vkCmdBeginVideoCodingKHR                    vkCmdBeginVideoCodingKHR                    = 0;
    PFN_vkCmdEndVideoCodingKHR                      vkCmdEndVideoCodingKHR                      = 0;
    PFN_vkCmdControlVideoCodingKHR                  vkCmdControlVideoCodingKHR                  = 0;

    //=== VK_KHR_video_decode_queue ===
    PFN_vkCmdDecodeVideoKHR vkCmdDecodeVideoKHR = 0;

    //=== VK_EXT_transform_feedback ===
    PFN_vkCmdBindTransformFeedbackBuffersEXT vkCmdBindTransformFeedbackBuffersEXT = 0;
    PFN_vkCmdBeginTransformFeedbackEXT       vkCmdBeginTransformFeedbackEXT       = 0;
    PFN_vkCmdEndTransformFeedbackEXT         vkCmdEndTransformFeedbackEXT         = 0;
    PFN_vkCmdBeginQueryIndexedEXT            vkCmdBeginQueryIndexedEXT            = 0;
    PFN_vkCmdEndQueryIndexedEXT              vkCmdEndQueryIndexedEXT              = 0;
    PFN_vkCmdDrawIndirectByteCountEXT        vkCmdDrawIndirectByteCountEXT        = 0;

    //=== VK_NVX_binary_import ===
    PFN_vkCreateCuModuleNVX    vkCreateCuModuleNVX    = 0;
    PFN_vkCreateCuFunctionNVX  vkCreateCuFunctionNVX  = 0;
    PFN_vkDestroyCuModuleNVX   vkDestroyCuModuleNVX   = 0;
    PFN_vkDestroyCuFunctionNVX vkDestroyCuFunctionNVX = 0;
    PFN_vkCmdCuLaunchKernelNVX vkCmdCuLaunchKernelNVX = 0;

    //=== VK_NVX_image_view_handle ===
    PFN_vkGetImageViewHandleNVX                 vkGetImageViewHandleNVX                 = 0;
    PFN_vkGetImageViewHandle64NVX               vkGetImageViewHandle64NVX               = 0;
    PFN_vkGetImageViewAddressNVX                vkGetImageViewAddressNVX                = 0;
    PFN_vkGetDeviceCombinedImageSamplerIndexNVX vkGetDeviceCombinedImageSamplerIndexNVX = 0;

    //=== VK_AMD_draw_indirect_count ===
    PFN_vkCmdDrawIndirectCountAMD        vkCmdDrawIndirectCountAMD        = 0;
    PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD = 0;

    //=== VK_AMD_shader_info ===
    PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD = 0;

    //=== VK_KHR_dynamic_rendering ===
    PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR = 0;
    PFN_vkCmdEndRenderingKHR   vkCmdEndRenderingKHR   = 0;

#if defined( VK_USE_PLATFORM_GGP )
    //=== VK_GGP_stream_descriptor_surface ===
    PFN_vkCreateStreamDescriptorSurfaceGGP vkCreateStreamDescriptorSurfaceGGP = 0;
#else
    PFN_dummy vkCreateStreamDescriptorSurfaceGGP_placeholder = 0;
#endif /*VK_USE_PLATFORM_GGP*/

    //=== VK_NV_external_memory_capabilities ===
    PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV vkGetPhysicalDeviceExternalImageFormatPropertiesNV = 0;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
    //=== VK_NV_external_memory_win32 ===
    PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV = 0;
#else
    PFN_dummy vkGetMemoryWin32HandleNV_placeholder = 0;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

    //=== VK_KHR_get_physical_device_properties2 ===
    PFN_vkGetPhysicalDeviceFeatures2KHR                    vkGetPhysicalDeviceFeatures2KHR                    = 0;
    PFN_vkGetPhysicalDeviceProperties2KHR                  vkGetPhysicalDeviceProperties2KHR                  = 0;
    PFN_vkGetPhysicalDeviceFormatProperties2KHR            vkGetPhysicalDeviceFormatProperties2KHR            = 0;
    PFN_vkGetPhysicalDeviceImageFormatProperties2KHR       vkGetPhysicalDeviceImageFormatProperties2KHR       = 0;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR       vkGetPhysicalDeviceQueueFamilyProperties2KHR       = 0;
    PFN_vkGetPhysicalDeviceMemoryProperties2KHR            vkGetPhysicalDeviceMemoryProperties2KHR            = 0;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR vkGetPhysicalDeviceSparseImageFormatProperties2KHR = 0;

    //=== VK_KHR_device_group ===
    PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR = 0;
    PFN_vkCmdSetDeviceMaskKHR                 vkCmdSetDeviceMaskKHR                 = 0;
    PFN_vkCmdDispatchBaseKHR                  vkCmdDispatchBaseKHR                  = 0;

#if defined( VK_USE_PLATFORM_VI_NN )
    //=== VK_NN_vi_surface ===
    PFN_vkCreateViSurfaceNN vkCreateViSurfaceNN = 0;
#else
    PFN_dummy vkCreateViSurfaceNN_placeholder = 0;
#endif /*VK_USE_PLATFORM_VI_NN*/

    //=== VK_KHR_maintenance1 ===
    PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR = 0;

    //=== VK_KHR_device_group_creation ===
    PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR = 0;

    //=== VK_KHR_external_memory_capabilities ===
    PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR vkGetPhysicalDeviceExternalBufferPropertiesKHR = 0;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
    //=== VK_KHR_external_memory_win32 ===
    PFN_vkGetMemoryWin32HandleKHR           vkGetMemoryWin32HandleKHR           = 0;
    PFN_vkGetMemoryWin32HandlePropertiesKHR vkGetMemoryWin32HandlePropertiesKHR = 0;
#else
    PFN_dummy vkGetMemoryWin32HandleKHR_placeholder           = 0;
    PFN_dummy vkGetMemoryWin32HandlePropertiesKHR_placeholder = 0;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

    //=== VK_KHR_external_memory_fd ===
    PFN_vkGetMemoryFdKHR           vkGetMemoryFdKHR           = 0;
    PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR = 0;

    //=== VK_KHR_external_semaphore_capabilities ===
    PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR vkGetPhysicalDeviceExternalSemaphorePropertiesKHR = 0;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
    //=== VK_KHR_external_semaphore_win32 ===
    PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR = 0;
    PFN_vkGetSemaphoreWin32HandleKHR    vkGetSemaphoreWin32HandleKHR    = 0;
#else
    PFN_dummy vkImportSemaphoreWin32HandleKHR_placeholder = 0;
    PFN_dummy vkGetSemaphoreWin32HandleKHR_placeholder    = 0;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

    //=== VK_KHR_external_semaphore_fd ===
    PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR = 0;
    PFN_vkGetSemaphoreFdKHR    vkGetSemaphoreFdKHR    = 0;

    //=== VK_KHR_push_descriptor ===
    PFN_vkCmdPushDescriptorSetKHR             vkCmdPushDescriptorSetKHR             = 0;
    PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR = 0;

    //=== VK_EXT_conditional_rendering ===
    PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT = 0;
    PFN_vkCmdEndConditionalRenderingEXT   vkCmdEndConditionalRenderingEXT   = 0;

    //=== VK_KHR_descriptor_update_template ===
    PFN_vkCreateDescriptorUpdateTemplateKHR  vkCreateDescriptorUpdateTemplateKHR  = 0;
    PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR = 0;
    PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR = 0;

    //=== VK_NV_clip_space_w_scaling ===
    PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV = 0;

    //=== VK_EXT_direct_mode_display ===
    PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT = 0;

#if defined( VK_USE_PLATFORM_XLIB_XRANDR_EXT )
    //=== VK_EXT_acquire_xlib_display ===
    PFN_vkAcquireXlibDisplayEXT    vkAcquireXlibDisplayEXT    = 0;
    PFN_vkGetRandROutputDisplayEXT vkGetRandROutputDisplayEXT = 0;
#else
    PFN_dummy vkAcquireXlibDisplayEXT_placeholder    = 0;
    PFN_dummy vkGetRandROutputDisplayEXT_placeholder = 0;
#endif /*VK_USE_PLATFORM_XLIB_XRANDR_EXT*/

    //=== VK_EXT_display_surface_counter ===
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT = 0;

    //=== VK_EXT_display_control ===
    PFN_vkDisplayPowerControlEXT  vkDisplayPowerControlEXT  = 0;
    PFN_vkRegisterDeviceEventEXT  vkRegisterDeviceEventEXT  = 0;
    PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT = 0;
    PFN_vkGetSwapchainCounterEXT  vkGetSwapchainCounterEXT  = 0;

    //=== VK_GOOGLE_display_timing ===
    PFN_vkGetRefreshCycleDurationGOOGLE   vkGetRefreshCycleDurationGOOGLE   = 0;
    PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE = 0;

    //=== VK_EXT_discard_rectangles ===
    PFN_vkCmdSetDiscardRectangleEXT       vkCmdSetDiscardRectangleEXT       = 0;
    PFN_vkCmdSetDiscardRectangleEnableEXT vkCmdSetDiscardRectangleEnableEXT = 0;
    PFN_vkCmdSetDiscardRectangleModeEXT   vkCmdSetDiscardRectangleModeEXT   = 0;

    //=== VK_EXT_hdr_metadata ===
    PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT = 0;

    //=== VK_KHR_create_renderpass2 ===
    PFN_vkCreateRenderPass2KHR   vkCreateRenderPass2KHR   = 0;
    PFN_vkCmdBeginRenderPass2KHR vkCmdBeginRenderPass2KHR = 0;
    PFN_vkCmdNextSubpass2KHR     vkCmdNextSubpass2KHR     = 0;
    PFN_vkCmdEndRenderPass2KHR   vkCmdEndRenderPass2KHR   = 0;

    //=== VK_KHR_shared_presentable_image ===
    PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR = 0;

    //=== VK_KHR_external_fence_capabilities ===
    PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR vkGetPhysicalDeviceExternalFencePropertiesKHR = 0;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
    //=== VK_KHR_external_fence_win32 ===
    PFN_vkImportFenceWin32HandleKHR vkImportFenceWin32HandleKHR = 0;
    PFN_vkGetFenceWin32HandleKHR    vkGetFenceWin32HandleKHR    = 0;
#else
    PFN_dummy vkImportFenceWin32HandleKHR_placeholder = 0;
    PFN_dummy vkGetFenceWin32HandleKHR_placeholder    = 0;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

    //=== VK_KHR_external_fence_fd ===
    PFN_vkImportFenceFdKHR vkImportFenceFdKHR = 0;
    PFN_vkGetFenceFdKHR    vkGetFenceFdKHR    = 0;

    //=== VK_KHR_performance_query ===
    PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = 0;
    PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR         vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR         = 0;
    PFN_vkAcquireProfilingLockKHR                                       vkAcquireProfilingLockKHR                                       = 0;
    PFN_vkReleaseProfilingLockKHR                                       vkReleaseProfilingLockKHR                                       = 0;

    //=== VK_KHR_get_surface_capabilities2 ===
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR = 0;
    PFN_vkGetPhysicalDeviceSurfaceFormats2KHR      vkGetPhysicalDeviceSurfaceFormats2KHR      = 0;

    //=== VK_KHR_get_display_properties2 ===
    PFN_vkGetPhysicalDeviceDisplayProperties2KHR vkGetPhysicalDeviceDisplayProperties2KHR = VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceDisplayProperties2KHR );
    PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR vkGetPhysicalDeviceDisplayPlaneProperties2KHR =
      VULKAN_HPP_DEFAULT_ADDR( vkGetPhysicalDeviceDisplayPlaneProperties2KHR );
    PFN_vkGetDisplayModeProperties2KHR    vkGetDisplayModeProperties2KHR    = VULKAN_HPP_DEFAULT_ADDR( vkGetDisplayModeProperties2KHR );
    PFN_vkGetDisplayPlaneCapabilities2KHR vkGetDisplayPlaneCapabilities2KHR = VULKAN_HPP_DEFAULT_ADDR( vkGetDisplayPlaneCapabilities2KHR );

#if defined( VK_USE_PLATFORM_IOS_MVK )
    //=== VK_MVK_ios_surface ===
    PFN_vkCreateIOSSurfaceMVK vkCreateIOSSurfaceMVK = 0;
#else
    PFN_dummy vkCreateIOSSurfaceMVK_placeholder = 0;
#endif /*VK_USE_PLATFORM_IOS_MVK*/

#if defined( VK_USE_PLATFORM_MACOS_MVK )
    //=== VK_MVK_macos_surface ===
    PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK = 0;
#else
    PFN_dummy vkCreateMacOSSurfaceMVK_placeholder = 0;
#endif /*VK_USE_PLATFORM_MACOS_MVK*/

    //=== VK_EXT_debug_utils ===
    PFN_vkSetDebugUtilsObjectNameEXT    vkSetDebugUtilsObjectNameEXT    = 0;
    PFN_vkSetDebugUtilsObjectTagEXT     vkSetDebugUtilsObjectTagEXT     = 0;
    PFN_vkQueueBeginDebugUtilsLabelEXT  vkQueueBeginDebugUtilsLabelEXT  = 0;
    PFN_vkQueueEndDebugUtilsLabelEXT    vkQueueEndDebugUtilsLabelEXT    = 0;
    PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT = 0;
    PFN_vkCmdBeginDebugUtilsLabelEXT    vkCmdBeginDebugUtilsLabelEXT    = 0;
    PFN_vkCmdEndDebugUtilsLabelEXT      vkCmdEndDebugUtilsLabelEXT      = 0;
    PFN_vkCmdInsertDebugUtilsLabelEXT   vkCmdInsertDebugUtilsLabelEXT   = 0;
    PFN_vkCreateDebugUtilsMessengerEXT  vkCreateDebugUtilsMessengerEXT  = 0;
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = 0;
    PFN_vkSubmitDebugUtilsMessageEXT    vkSubmitDebugUtilsMessageEXT    = 0;

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
    //=== VK_ANDROID_external_memory_android_hardware_buffer ===
    PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID = 0;
    PFN_vkGetMemoryAndroidHardwareBufferANDROID     vkGetMemoryAndroidHardwareBufferANDROID     = 0;
#else
    PFN_dummy vkGetAndroidHardwareBufferPropertiesANDROID_placeholder = 0;
    PFN_dummy vkGetMemoryAndroidHardwareBufferANDROID_placeholder     = 0;
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

#if defined( VK_ENABLE_BETA_EXTENSIONS )
    //=== VK_AMDX_shader_enqueue ===
    PFN_vkCreateExecutionGraphPipelinesAMDX        vkCreateExecutionGraphPipelinesAMDX        = 0;
    PFN_vkGetExecutionGraphPipelineScratchSizeAMDX vkGetExecutionGraphPipelineScratchSizeAMDX = 0;
    PFN_vkGetExecutionGraphPipelineNodeIndexAMDX   vkGetExecutionGraphPipelineNodeIndexAMDX   = 0;
    PFN_vkCmdInitializeGraphScratchMemoryAMDX      vkCmdInitializeGraphScratchMemoryAMDX      = 0;
    PFN_vkCmdDispatchGraphAMDX                     vkCmdDispatchGraphAMDX                     = 0;
    PFN_vkCmdDispatchGraphIndirectAMDX             vkCmdDispatchGraphIndirectAMDX             = 0;
    PFN_vkCmdDispatchGraphIndirectCountAMDX        vkCmdDispatchGraphIndirectCountAMDX        = 0;
#else
    PFN_dummy vkCreateExecutionGraphPipelinesAMDX_placeholder        = 0;
    PFN_dummy vkGetExecutionGraphPipelineScratchSizeAMDX_placeholder = 0;
    PFN_dummy vkGetExecutionGraphPipelineNodeIndexAMDX_placeholder   = 0;
    PFN_dummy vkCmdInitializeGraphScratchMemoryAMDX_placeholder      = 0;
    PFN_dummy vkCmdDispatchGraphAMDX_placeholder                     = 0;
    PFN_dummy vkCmdDispatchGraphIndirectAMDX_placeholder             = 0;
    PFN_dummy vkCmdDispatchGraphIndirectCountAMDX_placeholder        = 0;
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

    //=== VK_EXT_descriptor_heap ===
    PFN_vkWriteSamplerDescriptorsEXT         vkWriteSamplerDescriptorsEXT         = 0;
    PFN_vkWriteResourceDescriptorsEXT        vkWriteResourceDescriptorsEXT        = 0;
    PFN_vkCmdBindSamplerHeapEXT              vkCmdBindSamplerHeapEXT              = 0;
    PFN_vkCmdBindResourceHeapEXT             vkCmdBindResourceHeapEXT             = 0;
    PFN_vkCmdPushDataEXT                     vkCmdPushDataEXT                     = 0;
    PFN_vkGetImageOpaqueCaptureDataEXT       vkGetImageOpaqueCaptureDataEXT       = 0;
    PFN_vkGetPhysicalDeviceDescriptorSizeEXT vkGetPhysicalDeviceDescriptorSizeEXT = 0;
    PFN_vkRegisterCustomBorderColorEXT       vkRegisterCustomBorderColorEXT       = 0;
    PFN_vkUnregisterCustomBorderColorEXT     vkUnregisterCustomBorderColorEXT     = 0;
    PFN_vkGetTensorOpaqueCaptureDataARM      vkGetTensorOpaqueCaptureDataARM      = 0;

    //=== VK_EXT_sample_locations ===
    PFN_vkCmdSetSampleLocationsEXT                  vkCmdSetSampleLocationsEXT                  = 0;
    PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT = 0;

    //=== VK_KHR_get_memory_requirements2 ===
    PFN_vkGetImageMemoryRequirements2KHR       vkGetImageMemoryRequirements2KHR       = 0;
    PFN_vkGetBufferMemoryRequirements2KHR      vkGetBufferMemoryRequirements2KHR      = 0;
    PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR = 0;

    //=== VK_KHR_acceleration_structure ===
    PFN_vkCreateAccelerationStructureKHR                 vkCreateAccelerationStructureKHR                 = 0;
    PFN_vkDestroyAccelerationStructureKHR                vkDestroyAccelerationStructureKHR                = 0;
    PFN_vkCmdBuildAccelerationStructuresKHR              vkCmdBuildAccelerationStructuresKHR              = 0;
    PFN_vkCmdBuildAccelerationStructuresIndirectKHR      vkCmdBuildAccelerationStructuresIndirectKHR      = 0;
    PFN_vkBuildAccelerationStructuresKHR                 vkBuildAccelerationStructuresKHR                 = 0;
    PFN_vkCopyAccelerationStructureKHR                   vkCopyAccelerationStructureKHR                   = 0;
    PFN_vkCopyAccelerationStructureToMemoryKHR           vkCopyAccelerationStructureToMemoryKHR           = 0;
    PFN_vkCopyMemoryToAccelerationStructureKHR           vkCopyMemoryToAccelerationStructureKHR           = 0;
    PFN_vkWriteAccelerationStructuresPropertiesKHR       vkWriteAccelerationStructuresPropertiesKHR       = 0;
    PFN_vkCmdCopyAccelerationStructureKHR                vkCmdCopyAccelerationStructureKHR                = 0;
    PFN_vkCmdCopyAccelerationStructureToMemoryKHR        vkCmdCopyAccelerationStructureToMemoryKHR        = 0;
    PFN_vkCmdCopyMemoryToAccelerationStructureKHR        vkCmdCopyMemoryToAccelerationStructureKHR        = 0;
    PFN_vkGetAccelerationStructureDeviceAddressKHR       vkGetAccelerationStructureDeviceAddressKHR       = 0;
    PFN_vkCmdWriteAccelerationStructuresPropertiesKHR    vkCmdWriteAccelerationStructuresPropertiesKHR    = 0;
    PFN_vkGetDeviceAccelerationStructureCompatibilityKHR vkGetDeviceAccelerationStructureCompatibilityKHR = 0;
    PFN_vkGetAccelerationStructureBuildSizesKHR          vkGetAccelerationStructureBuildSizesKHR          = 0;

    //=== VK_KHR_ray_tracing_pipeline ===
    PFN_vkCmdTraceRaysKHR                                 vkCmdTraceRaysKHR                                 = 0;
    PFN_vkCreateRayTracingPipelinesKHR                    vkCreateRayTracingPipelinesKHR                    = 0;
    PFN_vkGetRayTracingShaderGroupHandlesKHR              vkGetRayTracingShaderGroupHandlesKHR              = 0;
    PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR vkGetRayTracingCaptureReplayShaderGroupHandlesKHR = 0;
    PFN_vkCmdTraceRaysIndirectKHR                         vkCmdTraceRaysIndirectKHR                         = 0;
    PFN_vkGetRayTracingShaderGroupStackSizeKHR            vkGetRayTracingShaderGroupStackSizeKHR            = 0;
    PFN_vkCmdSetRayTracingPipelineStackSizeKHR            vkCmdSetRayTracingPipelineStackSizeKHR            = 0;

    //=== VK_KHR_sampler_ycbcr_conversion ===
    PFN_vkCreateSamplerYcbcrConversionKHR  vkCreateSamplerYcbcrConversionKHR  = 0;
    PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR = 0;

    //=== VK_KHR_bind_memory2 ===
    PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR = 0;
    PFN_vkBindImageMemory2KHR  vkBindImageMemory2KHR  = 0;

    //=== VK_EXT_image_drm_format_modifier ===
    PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT = 0;

    //=== VK_EXT_validation_cache ===
    PFN_vkCreateValidationCacheEXT  vkCreateValidationCacheEXT  = 0;
    PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT = 0;
    PFN_vkMergeValidationCachesEXT  vkMergeValidationCachesEXT  = 0;
    PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT = 0;

    //=== VK_NV_shading_rate_image ===
    PFN_vkCmdBindShadingRateImageNV          vkCmdBindShadingRateImageNV          = 0;
    PFN_vkCmdSetViewportShadingRatePaletteNV vkCmdSetViewportShadingRatePaletteNV = 0;
    PFN_vkCmdSetCoarseSampleOrderNV          vkCmdSetCoarseSampleOrderNV          = 0;

    //=== VK_NV_ray_tracing ===
    PFN_vkCreateAccelerationStructureNV                vkCreateAccelerationStructureNV                = 0;
    PFN_vkDestroyAccelerationStructureNV               vkDestroyAccelerationStructureNV               = 0;
    PFN_vkGetAccelerationStructureMemoryRequirementsNV vkGetAccelerationStructureMemoryRequirementsNV = 0;
    PFN_vkBindAccelerationStructureMemoryNV            vkBindAccelerationStructureMemoryNV            = 0;
    PFN_vkCmdBuildAccelerationStructureNV              vkCmdBuildAccelerationStructureNV              = 0;
    PFN_vkCmdCopyAccelerationStructureNV               vkCmdCopyAccelerationStructureNV               = 0;
    PFN_vkCmdTraceRaysNV                               vkCmdTraceRaysNV                               = 0;
    PFN_vkCreateRayTracingPipelinesNV                  vkCreateRayTracingPipelinesNV                  = 0;
    PFN_vkGetRayTracingShaderGroupHandlesNV            vkGetRayTracingShaderGroupHandlesNV            = 0;
    PFN_vkGetAccelerationStructureHandleNV             vkGetAccelerationStructureHandleNV             = 0;
    PFN_vkCmdWriteAccelerationStructuresPropertiesNV   vkCmdWriteAccelerationStructuresPropertiesNV   = 0;
    PFN_vkCompileDeferredNV                            vkCompileDeferredNV                            = 0;

    //=== VK_KHR_maintenance3 ===
    PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR = 0;

    //=== VK_KHR_draw_indirect_count ===
    PFN_vkCmdDrawIndirectCountKHR        vkCmdDrawIndirectCountKHR        = 0;
    PFN_vkCmdDrawIndexedIndirectCountKHR vkCmdDrawIndexedIndirectCountKHR = 0;

    //=== VK_EXT_external_memory_host ===
    PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT = 0;

    //=== VK_AMD_buffer_marker ===
    PFN_vkCmdWriteBufferMarkerAMD  vkCmdWriteBufferMarkerAMD  = 0;
    PFN_vkCmdWriteBufferMarker2AMD vkCmdWriteBufferMarker2AMD = 0;

    //=== VK_EXT_calibrated_timestamps ===
    PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT vkGetPhysicalDeviceCalibrateableTimeDomainsEXT = 0;
    PFN_vkGetCalibratedTimestampsEXT                   vkGetCalibratedTimestampsEXT                   = 0;

    //=== VK_NV_mesh_shader ===
    PFN_vkCmdDrawMeshTasksNV              vkCmdDrawMeshTasksNV              = 0;
    PFN_vkCmdDrawMeshTasksIndirectNV      vkCmdDrawMeshTasksIndirectNV      = 0;
    PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV = 0;

    //=== VK_NV_scissor_exclusive ===
    PFN_vkCmdSetExclusiveScissorEnableNV vkCmdSetExclusiveScissorEnableNV = 0;
    PFN_vkCmdSetExclusiveScissorNV       vkCmdSetExclusiveScissorNV       = 0;

    //=== VK_NV_device_diagnostic_checkpoints ===
    PFN_vkCmdSetCheckpointNV        vkCmdSetCheckpointNV        = 0;
    PFN_vkGetQueueCheckpointDataNV  vkGetQueueCheckpointDataNV  = 0;
    PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV = 0;

    //=== VK_KHR_timeline_semaphore ===
    PFN_vkGetSemaphoreCounterValueKHR vkGetSemaphoreCounterValueKHR = 0;
    PFN_vkWaitSemaphoresKHR           vkWaitSemaphoresKHR           = 0;
    PFN_vkSignalSemaphoreKHR          vkSignalSemaphoreKHR          = 0;

    //=== VK_EXT_present_timing ===
    PFN_vkSetSwapchainPresentTimingQueueSizeEXT vkSetSwapchainPresentTimingQueueSizeEXT = 0;
    PFN_vkGetSwapchainTimingPropertiesEXT       vkGetSwapchainTimingPropertiesEXT       = 0;
    PFN_vkGetSwapchainTimeDomainPropertiesEXT   vkGetSwapchainTimeDomainPropertiesEXT   = 0;
    PFN_vkGetPastPresentationTimingEXT          vkGetPastPresentationTimingEXT          = 0;

    //=== VK_INTEL_performance_query ===
    PFN_vkInitializePerformanceApiINTEL         vkInitializePerformanceApiINTEL         = 0;
    PFN_vkUninitializePerformanceApiINTEL       vkUninitializePerformanceApiINTEL       = 0;
    PFN_vkCmdSetPerformanceMarkerINTEL          vkCmdSetPerformanceMarkerINTEL          = 0;
    PFN_vkCmdSetPerformanceStreamMarkerINTEL    vkCmdSetPerformanceStreamMarkerINTEL    = 0;
    PFN_vkCmdSetPerformanceOverrideINTEL        vkCmdSetPerformanceOverrideINTEL        = 0;
    PFN_vkAcquirePerformanceConfigurationINTEL  vkAcquirePerformanceConfigurationINTEL  = 0;
    PFN_vkReleasePerformanceConfigurationINTEL  vkReleasePerformanceConfigurationINTEL  = 0;
    PFN_vkQueueSetPerformanceConfigurationINTEL vkQueueSetPerformanceConfigurationINTEL = 0;
    PFN_vkGetPerformanceParameterINTEL          vkGetPerformanceParameterINTEL          = 0;

    //=== VK_AMD_display_native_hdr ===
    PFN_vkSetLocalDimmingAMD vkSetLocalDimmingAMD = 0;

#if defined( VK_USE_PLATFORM_FUCHSIA )
    //=== VK_FUCHSIA_imagepipe_surface ===
    PFN_vkCreateImagePipeSurfaceFUCHSIA vkCreateImagePipeSurfaceFUCHSIA = 0;
#else
    PFN_dummy vkCreateImagePipeSurfaceFUCHSIA_placeholder = 0;
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#if defined( VK_USE_PLATFORM_METAL_EXT )
    //=== VK_EXT_metal_surface ===
    PFN_vkCreateMetalSurfaceEXT vkCreateMetalSurfaceEXT = 0;
#else
    PFN_dummy vkCreateMetalSurfaceEXT_placeholder = 0;
#endif /*VK_USE_PLATFORM_METAL_EXT*/

    //=== VK_KHR_fragment_shading_rate ===
    PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR vkGetPhysicalDeviceFragmentShadingRatesKHR = 0;
    PFN_vkCmdSetFragmentShadingRateKHR             vkCmdSetFragmentShadingRateKHR             = 0;

    //=== VK_KHR_dynamic_rendering_local_read ===
    PFN_vkCmdSetRenderingAttachmentLocationsKHR    vkCmdSetRenderingAttachmentLocationsKHR    = 0;
    PFN_vkCmdSetRenderingInputAttachmentIndicesKHR vkCmdSetRenderingInputAttachmentIndicesKHR = 0;

    //=== VK_EXT_buffer_device_address ===
    PFN_vkGetBufferDeviceAddressEXT vkGetBufferDeviceAddressEXT = 0;

    //=== VK_EXT_tooling_info ===
    PFN_vkGetPhysicalDeviceToolPropertiesEXT vkGetPhysicalDeviceToolPropertiesEXT = 0;

    //=== VK_KHR_present_wait ===
    PFN_vkWaitForPresentKHR vkWaitForPresentKHR = 0;

    //=== VK_NV_cooperative_matrix ===
    PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV vkGetPhysicalDeviceCooperativeMatrixPropertiesNV = 0;

    //=== VK_NV_coverage_reduction_mode ===
    PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV = 0;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
    //=== VK_EXT_full_screen_exclusive ===
    PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT vkGetPhysicalDeviceSurfacePresentModes2EXT = 0;
    PFN_vkAcquireFullScreenExclusiveModeEXT        vkAcquireFullScreenExclusiveModeEXT        = 0;
    PFN_vkReleaseFullScreenExclusiveModeEXT        vkReleaseFullScreenExclusiveModeEXT        = 0;
    PFN_vkGetDeviceGroupSurfacePresentModes2EXT    vkGetDeviceGroupSurfacePresentModes2EXT    = 0;
#else
    PFN_dummy vkGetPhysicalDeviceSurfacePresentModes2EXT_placeholder = 0;
    PFN_dummy vkAcquireFullScreenExclusiveModeEXT_placeholder        = 0;
    PFN_dummy vkReleaseFullScreenExclusiveModeEXT_placeholder        = 0;
    PFN_dummy vkGetDeviceGroupSurfacePresentModes2EXT_placeholder    = 0;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

    //=== VK_EXT_headless_surface ===
    PFN_vkCreateHeadlessSurfaceEXT vkCreateHeadlessSurfaceEXT = VULKAN_HPP_DEFAULT_ADDR( vkCreateHeadlessSurfaceEXT );

    //=== VK_KHR_buffer_device_address ===
    PFN_vkGetBufferDeviceAddressKHR              vkGetBufferDeviceAddressKHR              = 0;
    PFN_vkGetBufferOpaqueCaptureAddressKHR       vkGetBufferOpaqueCaptureAddressKHR       = 0;
    PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR vkGetDeviceMemoryOpaqueCaptureAddressKHR = 0;

    //=== VK_EXT_line_rasterization ===
    PFN_vkCmdSetLineStippleEXT vkCmdSetLineStippleEXT = 0;

    //=== VK_EXT_host_query_reset ===
    PFN_vkResetQueryPoolEXT vkResetQueryPoolEXT = 0;

    //=== VK_EXT_extended_dynamic_state ===
    PFN_vkCmdSetCullModeEXT              vkCmdSetCullModeEXT              = 0;
    PFN_vkCmdSetFrontFaceEXT             vkCmdSetFrontFaceEXT             = 0;
    PFN_vkCmdSetPrimitiveTopologyEXT     vkCmdSetPrimitiveTopologyEXT     = 0;
    PFN_vkCmdSetViewportWithCountEXT     vkCmdSetViewportWithCountEXT     = 0;
    PFN_vkCmdSetScissorWithCountEXT      vkCmdSetScissorWithCountEXT      = 0;
    PFN_vkCmdBindVertexBuffers2EXT       vkCmdBindVertexBuffers2EXT       = 0;
    PFN_vkCmdSetDepthTestEnableEXT       vkCmdSetDepthTestEnableEXT       = 0;
    PFN_vkCmdSetDepthWriteEnableEXT      vkCmdSetDepthWriteEnableEXT      = 0;
    PFN_vkCmdSetDepthCompareOpEXT        vkCmdSetDepthCompareOpEXT        = 0;
    PFN_vkCmdSetDepthBoundsTestEnableEXT vkCmdSetDepthBoundsTestEnableEXT = 0;
    PFN_vkCmdSetStencilTestEnableEXT     vkCmdSetStencilTestEnableEXT     = 0;
    PFN_vkCmdSetStencilOpEXT             vkCmdSetStencilOpEXT             = 0;

    //=== VK_KHR_deferred_host_operations ===
    PFN_vkCreateDeferredOperationKHR            vkCreateDeferredOperationKHR            = 0;
    PFN_vkDestroyDeferredOperationKHR           vkDestroyDeferredOperationKHR           = 0;
    PFN_vkGetDeferredOperationMaxConcurrencyKHR vkGetDeferredOperationMaxConcurrencyKHR = 0;
    PFN_vkGetDeferredOperationResultKHR         vkGetDeferredOperationResultKHR         = 0;
    PFN_vkDeferredOperationJoinKHR              vkDeferredOperationJoinKHR              = 0;

    //=== VK_KHR_pipeline_executable_properties ===
    PFN_vkGetPipelineExecutablePropertiesKHR              vkGetPipelineExecutablePropertiesKHR              = 0;
    PFN_vkGetPipelineExecutableStatisticsKHR              vkGetPipelineExecutableStatisticsKHR              = 0;
    PFN_vkGetPipelineExecutableInternalRepresentationsKHR vkGetPipelineExecutableInternalRepresentationsKHR = 0;

    //=== VK_EXT_host_image_copy ===
    PFN_vkCopyMemoryToImageEXT          vkCopyMemoryToImageEXT          = 0;
    PFN_vkCopyImageToMemoryEXT          vkCopyImageToMemoryEXT          = 0;
    PFN_vkCopyImageToImageEXT           vkCopyImageToImageEXT           = 0;
    PFN_vkTransitionImageLayoutEXT      vkTransitionImageLayoutEXT      = 0;
    PFN_vkGetImageSubresourceLayout2EXT vkGetImageSubresourceLayout2EXT = 0;

    //=== VK_KHR_map_memory2 ===
    PFN_vkMapMemory2KHR   vkMapMemory2KHR   = 0;
    PFN_vkUnmapMemory2KHR vkUnmapMemory2KHR = 0;

    //=== VK_EXT_swapchain_maintenance1 ===
    PFN_vkReleaseSwapchainImagesEXT vkReleaseSwapchainImagesEXT = 0;

    //=== VK_NV_device_generated_commands ===
    PFN_vkGetGeneratedCommandsMemoryRequirementsNV vkGetGeneratedCommandsMemoryRequirementsNV = 0;
    PFN_vkCmdPreprocessGeneratedCommandsNV         vkCmdPreprocessGeneratedCommandsNV         = 0;
    PFN_vkCmdExecuteGeneratedCommandsNV            vkCmdExecuteGeneratedCommandsNV            = 0;
    PFN_vkCmdBindPipelineShaderGroupNV             vkCmdBindPipelineShaderGroupNV             = 0;
    PFN_vkCreateIndirectCommandsLayoutNV           vkCreateIndirectCommandsLayoutNV           = 0;
    PFN_vkDestroyIndirectCommandsLayoutNV          vkDestroyIndirectCommandsLayoutNV          = 0;

    //=== VK_EXT_depth_bias_control ===
    PFN_vkCmdSetDepthBias2EXT vkCmdSetDepthBias2EXT = 0;

    //=== VK_EXT_acquire_drm_display ===
    PFN_vkAcquireDrmDisplayEXT vkAcquireDrmDisplayEXT = 0;
    PFN_vkGetDrmDisplayEXT     vkGetDrmDisplayEXT     = 0;

    //=== VK_EXT_private_data ===
    PFN_vkCreatePrivateDataSlotEXT  vkCreatePrivateDataSlotEXT  = 0;
    PFN_vkDestroyPrivateDataSlotEXT vkDestroyPrivateDataSlotEXT = 0;
    PFN_vkSetPrivateDataEXT         vkSetPrivateDataEXT         = 0;
    PFN_vkGetPrivateDataEXT         vkGetPrivateDataEXT         = 0;

    //=== VK_KHR_video_encode_queue ===
    PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR = 0;
    PFN_vkGetEncodedVideoSessionParametersKHR                   vkGetEncodedVideoSessionParametersKHR                   = 0;
    PFN_vkCmdEncodeVideoKHR                                     vkCmdEncodeVideoKHR                                     = 0;

#if defined( VK_ENABLE_BETA_EXTENSIONS )
    //=== VK_NV_cuda_kernel_launch ===
    PFN_vkCreateCudaModuleNV    vkCreateCudaModuleNV    = 0;
    PFN_vkGetCudaModuleCacheNV  vkGetCudaModuleCacheNV  = 0;
    PFN_vkCreateCudaFunctionNV  vkCreateCudaFunctionNV  = 0;
    PFN_vkDestroyCudaModuleNV   vkDestroyCudaModuleNV   = 0;
    PFN_vkDestroyCudaFunctionNV vkDestroyCudaFunctionNV = 0;
    PFN_vkCmdCudaLaunchKernelNV vkCmdCudaLaunchKernelNV = 0;
#else
    PFN_dummy vkCreateCudaModuleNV_placeholder    = 0;
    PFN_dummy vkGetCudaModuleCacheNV_placeholder  = 0;
    PFN_dummy vkCreateCudaFunctionNV_placeholder  = 0;
    PFN_dummy vkDestroyCudaModuleNV_placeholder   = 0;
    PFN_dummy vkDestroyCudaFunctionNV_placeholder = 0;
    PFN_dummy vkCmdCudaLaunchKernelNV_placeholder = 0;
#endif /*VK_ENABLE_BETA_EXTENSIONS*/

    //=== VK_QCOM_tile_shading ===
    PFN_vkCmdDispatchTileQCOM          vkCmdDispatchTileQCOM          = 0;
    PFN_vkCmdBeginPerTileExecutionQCOM vkCmdBeginPerTileExecutionQCOM = 0;
    PFN_vkCmdEndPerTileExecutionQCOM   vkCmdEndPerTileExecutionQCOM   = 0;

#if defined( VK_USE_PLATFORM_METAL_EXT )
    //=== VK_EXT_metal_objects ===
    PFN_vkExportMetalObjectsEXT vkExportMetalObjectsEXT = 0;
#else
    PFN_dummy vkExportMetalObjectsEXT_placeholder = 0;
#endif /*VK_USE_PLATFORM_METAL_EXT*/

    //=== VK_KHR_synchronization2 ===
    PFN_vkCmdSetEvent2KHR        vkCmdSetEvent2KHR        = 0;
    PFN_vkCmdResetEvent2KHR      vkCmdResetEvent2KHR      = 0;
    PFN_vkCmdWaitEvents2KHR      vkCmdWaitEvents2KHR      = 0;
    PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR = 0;
    PFN_vkCmdWriteTimestamp2KHR  vkCmdWriteTimestamp2KHR  = 0;
    PFN_vkQueueSubmit2KHR        vkQueueSubmit2KHR        = 0;

    //=== VK_EXT_descriptor_buffer ===
    PFN_vkGetDescriptorSetLayoutSizeEXT                          vkGetDescriptorSetLayoutSizeEXT                          = 0;
    PFN_vkGetDescriptorSetLayoutBindingOffsetEXT                 vkGetDescriptorSetLayoutBindingOffsetEXT                 = 0;
    PFN_vkGetDescriptorEXT                                       vkGetDescriptorEXT                                       = 0;
    PFN_vkCmdBindDescriptorBuffersEXT                            vkCmdBindDescriptorBuffersEXT                            = 0;
    PFN_vkCmdSetDescriptorBufferOffsetsEXT                       vkCmdSetDescriptorBufferOffsetsEXT                       = 0;
    PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT             vkCmdBindDescriptorBufferEmbeddedSamplersEXT             = 0;
    PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT                vkGetBufferOpaqueCaptureDescriptorDataEXT                = 0;
    PFN_vkGetImageOpaqueCaptureDescriptorDataEXT                 vkGetImageOpaqueCaptureDescriptorDataEXT                 = 0;
    PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT             vkGetImageViewOpaqueCaptureDescriptorDataEXT             = 0;
    PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT               vkGetSamplerOpaqueCaptureDescriptorDataEXT               = 0;
    PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT = 0;

    //=== VK_NV_fragment_shading_rate_enums ===
    PFN_vkCmdSetFragmentShadingRateEnumNV vkCmdSetFragmentShadingRateEnumNV = 0;

    //=== VK_EXT_mesh_shader ===
    PFN_vkCmdDrawMeshTasksEXT              vkCmdDrawMeshTasksEXT              = 0;
    PFN_vkCmdDrawMeshTasksIndirectEXT      vkCmdDrawMeshTasksIndirectEXT      = 0;
    PFN_vkCmdDrawMeshTasksIndirectCountEXT vkCmdDrawMeshTasksIndirectCountEXT = 0;

    //=== VK_KHR_copy_commands2 ===
    PFN_vkCmdCopyBuffer2KHR        vkCmdCopyBuffer2KHR        = 0;
    PFN_vkCmdCopyImage2KHR         vkCmdCopyImage2KHR         = 0;
    PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR = 0;
    PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR = 0;
    PFN_vkCmdBlitImage2KHR         vkCmdBlitImage2KHR         = 0;
    PFN_vkCmdResolveImage2KHR      vkCmdResolveImage2KHR      = 0;

    //=== VK_EXT_device_fault ===
    PFN_vkGetDeviceFaultInfoEXT vkGetDeviceFaultInfoEXT = 0;

#if defined( VK_USE_PLATFORM_WIN32_KHR )
    //=== VK_NV_acquire_winrt_display ===
    PFN_vkAcquireWinrtDisplayNV vkAcquireWinrtDisplayNV = 0;
    PFN_vkGetWinrtDisplayNV     vkGetWinrtDisplayNV     = 0;
#else
    PFN_dummy vkAcquireWinrtDisplayNV_placeholder = 0;
    PFN_dummy vkGetWinrtDisplayNV_placeholder     = 0;
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#if defined( VK_USE_PLATFORM_DIRECTFB_EXT )
    //=== VK_EXT_directfb_surface ===
    PFN_vkCreateDirectFBSurfaceEXT                        vkCreateDirectFBSurfaceEXT                        = 0;
    PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT vkGetPhysicalDeviceDirectFBPresentationSupportEXT = 0;
#else
    PFN_dummy vkCreateDirectFBSurfaceEXT_placeholder                        = 0;
    PFN_dummy vkGetPhysicalDeviceDirectFBPresentationSupportEXT_placeholder = 0;
#endif /*VK_USE_PLATFORM_DIRECTFB_EXT*/

    //=== VK_EXT_vertex_input_dynamic_state ===
    PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT = 0;

#if defined( VK_USE_PLATFORM_FUCHSIA )
    //=== VK_FUCHSIA_external_memory ===
    PFN_vkGetMemoryZirconHandleFUCHSIA           vkGetMemoryZirconHandleFUCHSIA           = 0;
    PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA vkGetMemoryZirconHandlePropertiesFUCHSIA = 0;
#else
    PFN_dummy vkGetMemoryZirconHandleFUCHSIA_placeholder           = 0;
    PFN_dummy vkGetMemoryZirconHandlePropertiesFUCHSIA_placeholder = 0;
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#if defined( VK_USE_PLATFORM_FUCHSIA )
    //=== VK_FUCHSIA_external_semaphore ===
    PFN_vkImportSemaphoreZirconHandleFUCHSIA vkImportSemaphoreZirconHandleFUCHSIA = 0;
    PFN_vkGetSemaphoreZirconHandleFUCHSIA    vkGetSemaphoreZirconHandleFUCHSIA    = 0;
#else
    PFN_dummy vkImportSemaphoreZirconHandleFUCHSIA_placeholder = 0;
    PFN_dummy vkGetSemaphoreZirconHandleFUCHSIA_placeholder    = 0;
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#if defined( VK_USE_PLATFORM_FUCHSIA )
    //=== VK_FUCHSIA_buffer_collection ===
    PFN_vkCreateBufferCollectionFUCHSIA               vkCreateBufferCollectionFUCHSIA               = 0;
    PFN_vkSetBufferCollectionImageConstraintsFUCHSIA  vkSetBufferCollectionImageConstraintsFUCHSIA  = 0;
    PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA vkSetBufferCollectionBufferConstraintsFUCHSIA = 0;
    PFN_vkDestroyBufferCollectionFUCHSIA              vkDestroyBufferCollectionFUCHSIA              = 0;
    PFN_vkGetBufferCollectionPropertiesFUCHSIA        vkGetBufferCollectionPropertiesFUCHSIA        = 0;
#else
    PFN_dummy vkCreateBufferCollectionFUCHSIA_placeholder               = 0;
    PFN_dummy vkSetBufferCollectionImageConstraintsFUCHSIA_placeholder  = 0;
    PFN_dummy vkSetBufferCollectionBufferConstraintsFUCHSIA_placeholder = 0;
    PFN_dummy vkDestroyBufferCollectionFUCHSIA_placeholder              = 0;
    PFN_dummy vkGetBufferCollectionPropertiesFUCHSIA_placeholder        = 0;
#endif /*VK_USE_PLATFORM_FUCHSIA*/

    //=== VK_HUAWEI_subpass_shading ===
    PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI = 0;
    PFN_vkCmdSubpassShadingHUAWEI                       vkCmdSubpassShadingHUAWEI                       = 0;

    //=== VK_HUAWEI_invocation_mask ===
    PFN_vkCmdBindInvocationMaskHUAWEI vkCmdBindInvocationMaskHUAWEI = 0;

    //=== VK_NV_external_memory_rdma ===
    PFN_vkGetMemoryRemoteAddressNV vkGetMemoryRemoteAddressNV = 0;

    //=== VK_EXT_pipeline_properties ===
    PFN_vkGetPipelinePropertiesEXT vkGetPipelinePropertiesEXT = 0;

    //=== VK_EXT_extended_dynamic_state2 ===
    PFN_vkCmdSetPatchControlPointsEXT      vkCmdSetPatchControlPointsEXT      = 0;
    PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT = 0;
    PFN_vkCmdSetDepthBiasEnableEXT         vkCmdSetDepthBiasEnableEXT         = 0;
    PFN_vkCmdSetLogicOpEXT                 vkCmdSetLogicOpEXT                 = 0;
    PFN_vkCmdSetPrimitiveRestartEnableEXT  vkCmdSetPrimitiveRestartEnableEXT  = 0;

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
    //=== VK_QNX_screen_surface ===
    PFN_vkCreateScreenSurfaceQNX                        vkCreateScreenSurfaceQNX                        = 0;
    PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX vkGetPhysicalDeviceScreenPresentationSupportQNX = 0;
#else
    PFN_dummy vkCreateScreenSurfaceQNX_placeholder                        = 0;
    PFN_dummy vkGetPhysicalDeviceScreenPresentationSupportQNX_placeholder = 0;
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/

    //=== VK_EXT_color_write_enable ===
    PFN_vkCmdSetColorWriteEnableEXT vkCmdSetColorWriteEnableEXT = 0;

    //=== VK_KHR_ray_tracing_maintenance1 ===
    PFN_vkCmdTraceRaysIndirect2KHR vkCmdTraceRaysIndirect2KHR = 0;

    //=== VK_EXT_multi_draw ===
    PFN_vkCmdDrawMultiEXT        vkCmdDrawMultiEXT        = 0;
    PFN_vkCmdDrawMultiIndexedEXT vkCmdDrawMultiIndexedEXT = 0;

    //=== VK_EXT_opacity_micromap ===
    PFN_vkCreateMicromapEXT                 vkCreateMicromapEXT                 = 0;
    PFN_vkDestroyMicromapEXT                vkDestroyMicromapEXT                = 0;
    PFN_vkCmdBuildMicromapsEXT              vkCmdBuildMicromapsEXT              = 0;
    PFN_vkBuildMicromapsEXT                 vkBuildMicromapsEXT                 = 0;
    PFN_vkCopyMicromapEXT                   vkCopyMicromapEXT                   = 0;
    PFN_vkCopyMicromapToMemoryEXT           vkCopyMicromapToMemoryEXT           = 0;
    PFN_vkCopyMemoryToMicromapEXT           vkCopyMemoryToMicromapEXT           = 0;
    PFN_vkWriteMicromapsPropertiesEXT       vkWriteMicromapsPropertiesEXT       = 0;
    PFN_vkCmdCopyMicromapEXT                vkCmdCopyMicromapEXT                = 0;
    PFN_vkCmdCopyMicromapToMemoryEXT        vkCmdCopyMicromapToMemoryEXT        = 0;
    PFN_vkCmdCopyMemoryToMicromapEXT        vkCmdCopyMemoryToMicromapEXT        = 0;
    PFN_vkCmdWriteMicromapsPropertiesEXT    vkCmdWriteMicromapsPropertiesEXT    = 0;
    PFN_vkGetDeviceMicromapCompatibilityEXT vkGetDeviceMicromapCompatibilityEXT = 0;
    PFN_vkGetMicromapBuildSizesEXT          vkGetMicromapBuildSizesEXT          = 0;

    //=== VK_HUAWEI_cluster_culling_shader ===
    PFN_vkCmdDrawClusterHUAWEI         vkCmdDrawClusterHUAWEI         = 0;
    PFN_vkCmdDrawClusterIndirectHUAWEI vkCmdDrawClusterIndirectHUAWEI = 0;

    //=== VK_EXT_pageable_device_local_memory ===
    PFN_vkSetDeviceMemoryPriorityEXT vkSetDeviceMemoryPriorityEXT = 0;

    //=== VK_KHR_maintenance4 ===
    PFN_vkGetDeviceBufferMemoryRequirementsKHR      vkGetDeviceBufferMemoryRequirementsKHR      = 0;
    PFN_vkGetDeviceImageMemoryRequirementsKHR       vkGetDeviceImageMemoryRequirementsKHR       = 0;
    PFN_vkGetDeviceImageSparseMemoryRequirementsKHR vkGetDeviceImageSparseMemoryRequirementsKHR = 0;

    //=== VK_VALVE_descriptor_set_host_mapping ===
    PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE vkGetDescriptorSetLayoutHostMappingInfoVALVE = 0;
    PFN_vkGetDescriptorSetHostMappingVALVE           vkGetDescriptorSetHostMappingVALVE           = 0;

    //=== VK_NV_copy_memory_indirect ===
    PFN_vkCmdCopyMemoryIndirectNV        vkCmdCopyMemoryIndirectNV        = 0;
    PFN_vkCmdCopyMemoryToImageIndirectNV vkCmdCopyMemoryToImageIndirectNV = 0;

    //=== VK_NV_memory_decompression ===
    PFN_vkCmdDecompressMemoryNV              vkCmdDecompressMemoryNV              = 0;
    PFN_vkCmdDecompressMemoryIndirectCountNV vkCmdDecompressMemoryIndirectCountNV = 0;

    //=== VK_NV_device_generated_commands_compute ===
    PFN_vkGetPipelineIndirectMemoryRequirementsNV vkGetPipelineIndirectMemoryRequirementsNV = 0;
    PFN_vkCmdUpdatePipelineIndirectBufferNV       vkCmdUpdatePipelineIndirectBufferNV       = 0;
    PFN_vkGetPipelineIndirectDeviceAddressNV      vkGetPipelineIndirectDeviceAddressNV      = 0;

#if defined( VK_USE_PLATFORM_OHOS )
    //=== VK_OHOS_external_memory ===
    PFN_vkGetNativeBufferPropertiesOHOS vkGetNativeBufferPropertiesOHOS = 0;
    PFN_vkGetMemoryNativeBufferOHOS     vkGetMemoryNativeBufferOHOS     = 0;
#else
    PFN_dummy vkGetNativeBufferPropertiesOHOS_placeholder = 0;
    PFN_dummy vkGetMemoryNativeBufferOHOS_placeholder     = 0;
#endif /*VK_USE_PLATFORM_OHOS*/

    //=== VK_EXT_extended_dynamic_state3 ===
    PFN_vkCmdSetDepthClampEnableEXT                 vkCmdSetDepthClampEnableEXT                 = 0;
    PFN_vkCmdSetPolygonModeEXT                      vkCmdSetPolygonModeEXT                      = 0;
    PFN_vkCmdSetRasterizationSamplesEXT             vkCmdSetRasterizationSamplesEXT             = 0;
    PFN_vkCmdSetSampleMaskEXT                       vkCmdSetSampleMaskEXT                       = 0;
    PFN_vkCmdSetAlphaToCoverageEnableEXT            vkCmdSetAlphaToCoverageEnableEXT            = 0;
    PFN_vkCmdSetAlphaToOneEnableEXT                 vkCmdSetAlphaToOneEnableEXT                 = 0;
    PFN_vkCmdSetLogicOpEnableEXT                    vkCmdSetLogicOpEnableEXT                    = 0;
    PFN_vkCmdSetColorBlendEnableEXT                 vkCmdSetColorBlendEnableEXT                 = 0;
    PFN_vkCmdSetColorBlendEquationEXT               vkCmdSetColorBlendEquationEXT               = 0;
    PFN_vkCmdSetColorWriteMaskEXT                   vkCmdSetColorWriteMaskEXT                   = 0;
    PFN_vkCmdSetTessellationDomainOriginEXT         vkCmdSetTessellationDomainOriginEXT         = 0;
    PFN_vkCmdSetRasterizationStreamEXT              vkCmdSetRasterizationStreamEXT              = 0;
    PFN_vkCmdSetConservativeRasterizationModeEXT    vkCmdSetConservativeRasterizationModeEXT    = 0;
    PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT vkCmdSetExtraPrimitiveOverestimationSizeEXT = 0;
    PFN_vkCmdSetDepthClipEnableEXT                  vkCmdSetDepthClipEnableEXT                  = 0;
    PFN_vkCmdSetSampleLocationsEnableEXT            vkCmdSetSampleLocationsEnableEXT            = 0;
    PFN_vkCmdSetColorBlendAdvancedEXT               vkCmdSetColorBlendAdvancedEXT               = 0;
    PFN_vkCmdSetProvokingVertexModeEXT              vkCmdSetProvokingVertexModeEXT              = 0;
    PFN_vkCmdSetLineRasterizationModeEXT            vkCmdSetLineRasterizationModeEXT            = 0;
    PFN_vkCmdSetLineStippleEnableEXT                vkCmdSetLineStippleEnableEXT                = 0;
    PFN_vkCmdSetDepthClipNegativeOneToOneEXT        vkCmdSetDepthClipNegativeOneToOneEXT        = 0;
    PFN_vkCmdSetViewportWScalingEnableNV            vkCmdSetViewportWScalingEnableNV            = 0;
    PFN_vkCmdSetViewportSwizzleNV                   vkCmdSetViewportSwizzleNV                   = 0;
    PFN_vkCmdSetCoverageToColorEnableNV             vkCmdSetCoverageToColorEnableNV             = 0;
    PFN_vkCmdSetCoverageToColorLocationNV           vkCmdSetCoverageToColorLocationNV           = 0;
    PFN_vkCmdSetCoverageModulationModeNV            vkCmdSetCoverageModulationModeNV            = 0;
    PFN_vkCmdSetCoverageModulationTableEnableNV     vkCmdSetCoverageModulationTableEnableNV     = 0;
    PFN_vkCmdSetCoverageModulationTableNV           vkCmdSetCoverageModulationTableNV           = 0;
    PFN_vkCmdSetShadingRateImageEnableNV            vkCmdSetShadingRateImageEnableNV            = 0;
    PFN_vkCmdSetRepresentativeFragmentTestEnableNV  vkCmdSetRepresentativeFragmentTestEnableNV  = 0;
    PFN_vkCmdSetCoverageReductionModeNV             vkCmdSetCoverageReductionModeNV             = 0;

    //=== VK_ARM_tensors ===
    PFN_vkCreateTensorARM                              vkCreateTensorARM                              = 0;
    PFN_vkDestroyTensorARM                             vkDestroyTensorARM                             = 0;
    PFN_vkCreateTensorViewARM                          vkCreateTensorViewARM                          = 0;
    PFN_vkDestroyTensorViewARM                         vkDestroyTensorViewARM                         = 0;
    PFN_vkGetTensorMemoryRequirementsARM               vkGetTensorMemoryRequirementsARM               = 0;
    PFN_vkBindTensorMemoryARM                          vkBindTensorMemoryARM                          = 0;
    PFN_vkGetDeviceTensorMemoryRequirementsARM         vkGetDeviceTensorMemoryRequirementsARM         = 0;
    PFN_vkCmdCopyTensorARM                             vkCmdCopyTensorARM                             = 0;
    PFN_vkGetPhysicalDeviceExternalTensorPropertiesARM vkGetPhysicalDeviceExternalTensorPropertiesARM = 0;
    PFN_vkGetTensorOpaqueCaptureDescriptorDataARM      vkGetTensorOpaqueCaptureDescriptorDataARM      = 0;
    PFN_vkGetTensorViewOpaqueCaptureDescriptorDataARM  vkGetTensorViewOpaqueCaptureDescriptorDataARM  = 0;

    //=== VK_EXT_shader_module_identifier ===
    PFN_vkGetShaderModuleIdentifierEXT           vkGetShaderModuleIdentifierEXT           = 0;
    PFN_vkGetShaderModuleCreateInfoIdentifierEXT vkGetShaderModuleCreateInfoIdentifierEXT = 0;

    //=== VK_NV_optical_flow ===
    PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV vkGetPhysicalDeviceOpticalFlowImageFormatsNV = 0;
    PFN_vkCreateOpticalFlowSessionNV                 vkCreateOpticalFlowSessionNV                 = 0;
    PFN_vkDestroyOpticalFlowSessionNV                vkDestroyOpticalFlowSessionNV                = 0;
    PFN_vkBindOpticalFlowSessionImageNV              vkBindOpticalFlowSessionImageNV              = 0;
    PFN_vkCmdOpticalFlowExecuteNV                    vkCmdOpticalFlowExecuteNV                    = 0;

    //=== VK_KHR_maintenance5 ===
    PFN_vkCmdBindIndexBuffer2KHR             vkCmdBindIndexBuffer2KHR             = 0;
    PFN_vkGetRenderingAreaGranularityKHR     vkGetRenderingAreaGranularityKHR     = 0;
    PFN_vkGetDeviceImageSubresourceLayoutKHR vkGetDeviceImageSubresourceLayoutKHR = 0;
    PFN_vkGetImageSubresourceLayout2KHR      vkGetImageSubresourceLayout2KHR      = 0;

    //=== VK_AMD_anti_lag ===
    PFN_vkAntiLagUpdateAMD vkAntiLagUpdateAMD = 0;

    //=== VK_KHR_present_wait2 ===
    PFN_vkWaitForPresent2KHR vkWaitForPresent2KHR = 0;

    //=== VK_EXT_shader_object ===
    PFN_vkCreateShadersEXT         vkCreateShadersEXT         = 0;
    PFN_vkDestroyShaderEXT         vkDestroyShaderEXT         = 0;
    PFN_vkGetShaderBinaryDataEXT   vkGetShaderBinaryDataEXT   = 0;
    PFN_vkCmdBindShadersEXT        vkCmdBindShadersEXT        = 0;
    PFN_vkCmdSetDepthClampRangeEXT vkCmdSetDepthClampRangeEXT = 0;

    //=== VK_KHR_pipeline_binary ===
    PFN_vkCreatePipelineBinariesKHR      vkCreatePipelineBinariesKHR      = 0;
    PFN_vkDestroyPipelineBinaryKHR       vkDestroyPipelineBinaryKHR       = 0;
    PFN_vkGetPipelineKeyKHR              vkGetPipelineKeyKHR              = 0;
    PFN_vkGetPipelineBinaryDataKHR       vkGetPipelineBinaryDataKHR       = 0;
    PFN_vkReleaseCapturedPipelineDataKHR vkReleaseCapturedPipelineDataKHR = 0;

    //=== VK_QCOM_tile_properties ===
    PFN_vkGetFramebufferTilePropertiesQCOM      vkGetFramebufferTilePropertiesQCOM      = 0;
    PFN_vkGetDynamicRenderingTilePropertiesQCOM vkGetDynamicRenderingTilePropertiesQCOM = 0;

    //=== VK_KHR_swapchain_maintenance1 ===
    PFN_vkReleaseSwapchainImagesKHR vkReleaseSwapchainImagesKHR = 0;

    //=== VK_NV_cooperative_vector ===
    PFN_vkGetPhysicalDeviceCooperativeVectorPropertiesNV vkGetPhysicalDeviceCooperativeVectorPropertiesNV = 0;
    PFN_vkConvertCooperativeVectorMatrixNV               vkConvertCooperativeVectorMatrixNV               = 0;
    PFN_vkCmdConvertCooperativeVectorMatrixNV            vkCmdConvertCooperativeVectorMatrixNV            = 0;

    //=== VK_NV_low_latency2 ===
    PFN_vkSetLatencySleepModeNV  vkSetLatencySleepModeNV  = 0;
    PFN_vkLatencySleepNV         vkLatencySleepNV         = 0;
    PFN_vkSetLatencyMarkerNV     vkSetLatencyMarkerNV     = 0;
    PFN_vkGetLatencyTimingsNV    vkGetLatencyTimingsNV    = 0;
    PFN_vkQueueNotifyOutOfBandNV vkQueueNotifyOutOfBandNV = 0;

    //=== VK_KHR_cooperative_matrix ===
    PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR = 0;

    //=== VK_ARM_data_graph ===
    PFN_vkCreateDataGraphPipelinesARM                                        vkCreateDataGraphPipelinesARM                                        = 0;
    PFN_vkCreateDataGraphPipelineSessionARM                                  vkCreateDataGraphPipelineSessionARM                                  = 0;
    PFN_vkGetDataGraphPipelineSessionBindPointRequirementsARM                vkGetDataGraphPipelineSessionBindPointRequirementsARM                = 0;
    PFN_vkGetDataGraphPipelineSessionMemoryRequirementsARM                   vkGetDataGraphPipelineSessionMemoryRequirementsARM                   = 0;
    PFN_vkBindDataGraphPipelineSessionMemoryARM                              vkBindDataGraphPipelineSessionMemoryARM                              = 0;
    PFN_vkDestroyDataGraphPipelineSessionARM                                 vkDestroyDataGraphPipelineSessionARM                                 = 0;
    PFN_vkCmdDispatchDataGraphARM                                            vkCmdDispatchDataGraphARM                                            = 0;
    PFN_vkGetDataGraphPipelineAvailablePropertiesARM                         vkGetDataGraphPipelineAvailablePropertiesARM                         = 0;
    PFN_vkGetDataGraphPipelinePropertiesARM                                  vkGetDataGraphPipelinePropertiesARM                                  = 0;
    PFN_vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM                 vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM                 = 0;
    PFN_vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM = 0;

    //=== VK_EXT_attachment_feedback_loop_dynamic_state ===
    PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT vkCmdSetAttachmentFeedbackLoopEnableEXT = 0;

#if defined( VK_USE_PLATFORM_SCREEN_QNX )
    //=== VK_QNX_external_memory_screen_buffer ===
    PFN_vkGetScreenBufferPropertiesQNX vkGetScreenBufferPropertiesQNX = 0;
#else
    PFN_dummy vkGetScreenBufferPropertiesQNX_placeholder = 0;
#endif /*VK_USE_PLATFORM_SCREEN_QNX*/

    //=== VK_KHR_line_rasterization ===
    PFN_vkCmdSetLineStippleKHR vkCmdSetLineStippleKHR = 0;

    //=== VK_KHR_calibrated_timestamps ===
    PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR vkGetPhysicalDeviceCalibrateableTimeDomainsKHR = 0;
    PFN_vkGetCalibratedTimestampsKHR                   vkGetCalibratedTimestampsKHR                   = 0;

    //=== VK_KHR_maintenance6 ===
    PFN_vkCmdBindDescriptorSets2KHR                   vkCmdBindDescriptorSets2KHR                   = 0;
    PFN_vkCmdPushConstants2KHR                        vkCmdPushConstants2KHR                        = 0;
    PFN_vkCmdPushDescriptorSet2KHR                    vkCmdPushDescriptorSet2KHR                    = 0;
    PFN_vkCmdPushDescriptorSetWithTemplate2KHR        vkCmdPushDescriptorSetWithTemplate2KHR        = 0;
    PFN_vkCmdSetDescriptorBufferOffsets2EXT           vkCmdSetDescriptorBufferOffsets2EXT           = 0;
    PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT vkCmdBindDescriptorBufferEmbeddedSamplers2EXT = 0;

    //=== VK_QCOM_tile_memory_heap ===
    PFN_vkCmdBindTileMemoryQCOM vkCmdBindTileMemoryQCOM = 0;

    //=== VK_KHR_copy_memory_indirect ===
    PFN_vkCmdCopyMemoryIndirectKHR        vkCmdCopyMemoryIndirectKHR        = 0;
    PFN_vkCmdCopyMemoryToImageIndirectKHR vkCmdCopyMemoryToImageIndirectKHR = 0;

    //=== VK_EXT_memory_decompression ===
    PFN_vkCmdDecompressMemoryEXT              vkCmdDecompressMemoryEXT              = 0;
    PFN_vkCmdDecompressMemoryIndirectCountEXT vkCmdDecompressMemoryIndirectCountEXT = 0;

    //=== VK_NV_external_compute_queue ===
    PFN_vkCreateExternalComputeQueueNV  vkCreateExternalComputeQueueNV  = 0;
    PFN_vkDestroyExternalComputeQueueNV vkDestroyExternalComputeQueueNV = 0;
    PFN_vkGetExternalComputeQueueDataNV vkGetExternalComputeQueueDataNV = 0;

    //=== VK_NV_cluster_acceleration_structure ===
    PFN_vkGetClusterAccelerationStructureBuildSizesNV    vkGetClusterAccelerationStructureBuildSizesNV    = 0;
    PFN_vkCmdBuildClusterAccelerationStructureIndirectNV vkCmdBuildClusterAccelerationStructureIndirectNV = 0;

    //=== VK_NV_partitioned_acceleration_structure ===
    PFN_vkGetPartitionedAccelerationStructuresBuildSizesNV vkGetPartitionedAccelerationStructuresBuildSizesNV = 0;
    PFN_vkCmdBuildPartitionedAccelerationStructuresNV      vkCmdBuildPartitionedAccelerationStructuresNV      = 0;

    //=== VK_EXT_device_generated_commands ===
    PFN_vkGetGeneratedCommandsMemoryRequirementsEXT vkGetGeneratedCommandsMemoryRequirementsEXT = 0;
    PFN_vkCmdPreprocessGeneratedCommandsEXT         vkCmdPreprocessGeneratedCommandsEXT         = 0;
    PFN_vkCmdExecuteGeneratedCommandsEXT            vkCmdExecuteGeneratedCommandsEXT            = 0;
    PFN_vkCreateIndirectCommandsLayoutEXT           vkCreateIndirectCommandsLayoutEXT           = 0;
    PFN_vkDestroyIndirectCommandsLayoutEXT          vkDestroyIndirectCommandsLayoutEXT          = 0;
    PFN_vkCreateIndirectExecutionSetEXT             vkCreateIndirectExecutionSetEXT             = 0;
    PFN_vkDestroyIndirectExecutionSetEXT            vkDestroyIndirectExecutionSetEXT            = 0;
    PFN_vkUpdateIndirectExecutionSetPipelineEXT     vkUpdateIndirectExecutionSetPipelineEXT     = 0;
    PFN_vkUpdateIndirectExecutionSetShaderEXT       vkUpdateIndirectExecutionSetShaderEXT       = 0;

#if defined( VK_USE_PLATFORM_OHOS )
    //=== VK_OHOS_surface ===
    PFN_vkCreateSurfaceOHOS vkCreateSurfaceOHOS = 0;
#else
    PFN_dummy vkCreateSurfaceOHOS_placeholder = 0;
#endif /*VK_USE_PLATFORM_OHOS*/

    //=== VK_NV_cooperative_matrix2 ===
    PFN_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV = 0;

#if defined( VK_USE_PLATFORM_METAL_EXT )
    //=== VK_EXT_external_memory_metal ===
    PFN_vkGetMemoryMetalHandleEXT           vkGetMemoryMetalHandleEXT           = 0;
    PFN_vkGetMemoryMetalHandlePropertiesEXT vkGetMemoryMetalHandlePropertiesEXT = 0;
#else
    PFN_dummy vkGetMemoryMetalHandleEXT_placeholder           = 0;
    PFN_dummy vkGetMemoryMetalHandlePropertiesEXT_placeholder = 0;
#endif /*VK_USE_PLATFORM_METAL_EXT*/

    //=== VK_ARM_performance_counters_by_region ===
    PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM = 0;

    //=== VK_EXT_fragment_density_map_offset ===
    PFN_vkCmdEndRendering2EXT vkCmdEndRendering2EXT = 0;

    //=== VK_EXT_custom_resolve ===
    PFN_vkCmdBeginCustomResolveEXT vkCmdBeginCustomResolveEXT = 0;

    //=== VK_KHR_maintenance10 ===
    PFN_vkCmdEndRendering2KHR vkCmdEndRendering2KHR = 0;

    //=== VK_NV_compute_occupancy_priority ===
    PFN_vkCmdSetComputeOccupancyPriorityNV vkCmdSetComputeOccupancyPriorityNV = 0;

#if defined( VK_USE_PLATFORM_UBM_SEC )
    //=== VK_SEC_ubm_surface ===
    PFN_vkCreateUbmSurfaceSEC                        vkCreateUbmSurfaceSEC                        = 0;
    PFN_vkGetPhysicalDeviceUbmPresentationSupportSEC vkGetPhysicalDeviceUbmPresentationSupportSEC = 0;
#else
    PFN_dummy vkCreateUbmSurfaceSEC_placeholder                        = 0;
    PFN_dummy vkGetPhysicalDeviceUbmPresentationSupportSEC_placeholder = 0;
#endif /*VK_USE_PLATFORM_UBM_SEC*/
  };
}
