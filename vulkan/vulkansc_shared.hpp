// Copyright 2015-2023 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT
//

// This header is generated from the Khronos Vulkan XML API Registry.

#ifndef VULKAN_SHARED_HPP
#define VULKAN_SHARED_HPP

#include <vulkan/vulkan.hpp>

namespace VULKAN_HPP_NAMESPACE
{
#if !defined( VULKAN_HPP_NO_SMART_HANDLE )

  template <typename HandleType>
  class SharedHandleTraits;

  class NoParent;

  template <typename HandleType, typename = void>
  struct HasParentType : std::false_type
  {
  };

  template <typename HandleType>
  struct HasParentType<HandleType, decltype( (void)typename HandleType::ParentType() )> : std::true_type
  {
  };

  template <typename HandleType, typename Enable = void>
  struct GetParentType
  {
    using type = NoParent;
  };

  template <typename HandleType>
  struct GetParentType<HandleType, typename std::enable_if<HasParentType<HandleType>::value>::type>
  {
    using type = typename HandleType::ParentType;
  };

  template <class HandleType>
  using parent_of_t = typename GetParentType<HandleType>::type;

  template <class HandleType>
  VULKAN_HPP_CONSTEXPR_INLINE bool has_parent = !std::is_same<parent_of_t<HandleType>, NoParent>::value;

  //=====================================================================================================================

  template <typename HandleType>
  class SharedHandle;

  template <typename ParentType, typename Deleter>
  struct SharedHeader
  {
    SharedHeader( SharedHandle<ParentType> parent, Deleter deleter = Deleter() ) VULKAN_HPP_NOEXCEPT
      : parent( std::move( parent ) )
      , deleter( std::move( deleter ) )
    {
    }

    SharedHandle<ParentType> parent{};
    Deleter                  deleter;
  };

  template <typename Deleter>
  struct SharedHeader<NoParent, Deleter>
  {
    SharedHeader( Deleter deleter = Deleter() ) VULKAN_HPP_NOEXCEPT : deleter( std::move( deleter ) ) {}

    Deleter deleter;
  };

  //=====================================================================================================================

  template <typename HeaderType>
  class ReferenceCounter
  {
  public:
    template <typename... Args>
    ReferenceCounter( Args &&... control_args ) : m_header( std::forward<Args>( control_args )... ){};
    ReferenceCounter( const ReferenceCounter & )             = delete;
    ReferenceCounter & operator=( const ReferenceCounter & ) = delete;

  public:
    size_t addRef() VULKAN_HPP_NOEXCEPT
    {
      return m_ref_cnt.fetch_add( 1, std::memory_order_relaxed );
    }

    size_t release() VULKAN_HPP_NOEXCEPT
    {
      return m_ref_cnt.fetch_sub( 1, std::memory_order_acq_rel );
    }

  public:
    std::atomic_size_t m_ref_cnt{ 1 };
    HeaderType         m_header{};
  };

  //=====================================================================================================================

  template <typename HandleType, typename HeaderType, typename ForwardType = SharedHandle<HandleType>>
  class SharedHandleBase
  {
  public:
    SharedHandleBase() = default;

    template <typename... Args>
    SharedHandleBase( HandleType handle, Args &&... control_args )
      : m_control( new ReferenceCounter<HeaderType>( std::forward<Args>( control_args )... ) ), m_handle( handle )
    {
    }

    SharedHandleBase( const SharedHandleBase & o ) VULKAN_HPP_NOEXCEPT
    {
      o.addRef();
      m_handle  = o.m_handle;
      m_control = o.m_control;
    }

    SharedHandleBase( SharedHandleBase && o ) VULKAN_HPP_NOEXCEPT
      : m_handle( o.m_handle )
      , m_control( o.m_control )
    {
      o.m_handle  = nullptr;
      o.m_control = nullptr;
    }

    SharedHandleBase & operator=( const SharedHandleBase & o ) VULKAN_HPP_NOEXCEPT
    {
      SharedHandleBase( o ).swap( *this );
      return *this;
    }

    SharedHandleBase & operator=( SharedHandleBase && o ) VULKAN_HPP_NOEXCEPT
    {
      SharedHandleBase( std::move( o ) ).swap( *this );
      return *this;
    }

    ~SharedHandleBase()
    {
      // only this function owns the last reference to the control block
      // the same principle is used in the default deleter of std::shared_ptr
      if ( m_control && ( m_control->release() == 1 ) )
      {
        ForwardType::internalDestroy( getHeader(), m_handle );
        delete m_control;
      }
    }

  public:
    HandleType get() const VULKAN_HPP_NOEXCEPT
    {
      return m_handle;
    }

    explicit operator bool() const VULKAN_HPP_NOEXCEPT
    {
      return bool( m_handle );
    }

    const HandleType * operator->() const VULKAN_HPP_NOEXCEPT
    {
      return &m_handle;
    }

    HandleType * operator->() VULKAN_HPP_NOEXCEPT
    {
      return &m_handle;
    }

    void reset() VULKAN_HPP_NOEXCEPT
    {
      SharedHandleBase().swap( *this );
    }

    void swap( SharedHandleBase & o ) VULKAN_HPP_NOEXCEPT
    {
      std::swap( m_handle, o.m_handle );
      std::swap( m_control, o.m_control );
    }

    template <typename T = HandleType>
    typename std::enable_if<has_parent<T>, const SharedHandle<parent_of_t<HandleType>> &>::type getParent() const VULKAN_HPP_NOEXCEPT
    {
      return getHeader().parent;
    }

  protected:
    template <typename T = HandleType>
    static typename std::enable_if<!has_parent<T>, void>::type internalDestroy( const HeaderType & control, HandleType handle ) VULKAN_HPP_NOEXCEPT
    {
      control.deleter.destroy( handle );
    }

    template <typename T = HandleType>
    static typename std::enable_if<has_parent<T>, void>::type internalDestroy( const HeaderType & control, HandleType handle ) VULKAN_HPP_NOEXCEPT
    {
      control.deleter.destroy( control.parent.get(), handle );
    }

    const HeaderType & getHeader() const VULKAN_HPP_NOEXCEPT
    {
      return m_control->m_header;
    }

  private:
    void addRef() const VULKAN_HPP_NOEXCEPT
    {
      if ( m_control )
        m_control->addRef();
    }

  protected:
    ReferenceCounter<HeaderType> * m_control = nullptr;
    HandleType                     m_handle{};
  };

  template <typename HandleType>
  class SharedHandle : public SharedHandleBase<HandleType, SharedHeader<parent_of_t<HandleType>, typename SharedHandleTraits<HandleType>::deleter>>
  {
  private:
    using BaseType    = SharedHandleBase<HandleType, SharedHeader<parent_of_t<HandleType>, typename SharedHandleTraits<HandleType>::deleter>>;
    using DeleterType = typename SharedHandleTraits<HandleType>::deleter;
    friend BaseType;

  public:
    using element_type = HandleType;

    SharedHandle() = default;

    template <typename T = HandleType, typename = typename std::enable_if<has_parent<T>>::type>
    explicit SharedHandle( HandleType handle, SharedHandle<parent_of_t<HandleType>> parent, DeleterType deleter = DeleterType() ) VULKAN_HPP_NOEXCEPT
      : BaseType( handle, std::move( parent ), std::move( deleter ) )
    {
    }

    template <typename T = HandleType, typename = typename std::enable_if<!has_parent<T>>::type>
    explicit SharedHandle( HandleType handle, DeleterType deleter = DeleterType() ) VULKAN_HPP_NOEXCEPT : BaseType( handle, std::move( deleter ) )
    {
    }

  protected:
    using BaseType::internalDestroy;
  };

  template <typename SharedType>
  VULKAN_HPP_INLINE std::vector<typename SharedType::element_type> sharedToRaw( std::vector<SharedType> const & handles )
  {
    std::vector<typename SharedType::element_type> newBuffer( handles.size() );
    std::transform( handles.begin(), handles.end(), newBuffer.begin(), []( SharedType const & handle ) { return handle.get(); } );
    return newBuffer;
  }

  template <typename HandleType>
  class ObjectDestroyShared
  {
  public:
    using ParentType = parent_of_t<HandleType>;

    template <class Dispatcher>
    using destroy_pfn_t = typename std::conditional<has_parent<HandleType>,

                                                    void ( ParentType::* )( HandleType, const AllocationCallbacks *, const Dispatcher & ) const,

                                                    void ( HandleType::* )( const AllocationCallbacks *, const Dispatcher & ) const>::type;
    using selector_t    = typename std::conditional<has_parent<HandleType>, ParentType, HandleType>::type;

    template <typename Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ObjectDestroyShared( Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                         const Dispatcher & dispatch                             VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
      : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<destroy_pfn_t<Dispatcher>>( &selector_t::destroy ) ) )
      , m_dispatch( &dispatch )
      , m_allocationCallbacks( allocationCallbacks )
    {
    }

  public:
    template <typename T = HandleType>
    typename std::enable_if<has_parent<T>, void>::type destroy( ParentType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
      ( parent.*m_destroy )( handle, m_allocationCallbacks, *m_dispatch );
    }

    template <typename T = HandleType>
    typename std::enable_if<!has_parent<T>, void>::type destroy( HandleType handle ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
      ( handle.*m_destroy )( m_allocationCallbacks, *m_dispatch );
    }

  private:
    destroy_pfn_t<DispatchLoaderBase>   m_destroy             = nullptr;
    const DispatchLoaderBase *          m_dispatch            = nullptr;
    Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
  };

  template <typename HandleType>
  class ObjectFreeShared
  {
  public:
    using ParentType = parent_of_t<HandleType>;

    template <class Dispatcher>
    using destroy_pfn_t = void ( ParentType::* )( HandleType, const AllocationCallbacks *, const Dispatcher & ) const;

    template <class Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ObjectFreeShared( Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                      const Dispatcher & dispatch                             VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
      : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<destroy_pfn_t<Dispatcher>>( &ParentType::free ) ) )
      , m_dispatch( &dispatch )
      , m_allocationCallbacks( allocationCallbacks )
    {
    }

  public:
    void destroy( ParentType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
      ( parent.*m_destroy )( handle, m_allocationCallbacks, *m_dispatch );
    }

  private:
    destroy_pfn_t<DispatchLoaderBase>   m_destroy             = nullptr;
    const DispatchLoaderBase *          m_dispatch            = nullptr;
    Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
  };

  template <typename HandleType>
  class ObjectReleaseShared
  {
  public:
    using ParentType = parent_of_t<HandleType>;

    template <class Dispatcher>
    using destroy_pfn_t = void ( ParentType::* )( HandleType, const Dispatcher & ) const;

    template <class Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ObjectReleaseShared( Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                         const Dispatcher & dispatch                             VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
      : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<destroy_pfn_t<Dispatcher>>( &ParentType::release ) ) ), m_dispatch( &dispatch )
    {
    }

  public:
    void destroy( ParentType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
      ( parent.*m_destroy )( handle, *m_dispatch );
    }

  private:
    destroy_pfn_t<DispatchLoaderBase> m_destroy  = nullptr;
    const DispatchLoaderBase *        m_dispatch = nullptr;
  };

  template <typename HandleType, typename PoolType>
  class PoolFreeShared
  {
  public:
    using ParentType = parent_of_t<HandleType>;

    template <class Dispatcher>
    using ret_t = decltype( std::declval<ParentType>().free( PoolType(), 0u, nullptr, Dispatcher() ) );

    template <class Dispatcher>
    using destroy_pfn_t = ret_t<Dispatcher> ( ParentType::* )( PoolType, uint32_t, const HandleType *, const Dispatcher & ) const;

    PoolFreeShared() = default;

    template <class Dispatcher = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    PoolFreeShared( SharedHandle<PoolType> pool, const Dispatcher & dispatch VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )
      : m_destroy( reinterpret_cast<decltype( m_destroy )>( static_cast<destroy_pfn_t<Dispatcher>>( &ParentType::free ) ) )
      , m_pool( std::move( pool ) )
      , m_dispatch( &dispatch )
    {
    }

  public:
    void destroy( ParentType parent, HandleType handle ) const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_destroy && m_dispatch );
      ( parent.*m_destroy )( m_pool.get(), 1u, &handle, *m_dispatch );
    }

  private:
    destroy_pfn_t<DispatchLoaderBase> m_destroy  = nullptr;
    const DispatchLoaderBase *        m_dispatch = nullptr;
    SharedHandle<PoolType>            m_pool{};
  };

  //======================
  //=== SHARED HANDLEs ===
  //======================

  //=== VK_VERSION_1_0 ===
  template <>
  class SharedHandleTraits<Instance>
  {
  public:
    using deleter = ObjectDestroyShared<Instance>;
  };

  using SharedInstance = SharedHandle<Instance>;

  template <>
  class SharedHandleTraits<Device>
  {
  public:
    using deleter = ObjectDestroyShared<Device>;
  };

  using SharedDevice = SharedHandle<Device>;

  template <>
  class SharedHandleTraits<Fence>
  {
  public:
    using deleter = ObjectDestroyShared<Fence>;
  };

  using SharedFence = SharedHandle<Fence>;

  template <>
  class SharedHandleTraits<Semaphore>
  {
  public:
    using deleter = ObjectDestroyShared<Semaphore>;
  };

  using SharedSemaphore = SharedHandle<Semaphore>;

  template <>
  class SharedHandleTraits<Event>
  {
  public:
    using deleter = ObjectDestroyShared<Event>;
  };

  using SharedEvent = SharedHandle<Event>;

  template <>
  class SharedHandleTraits<Buffer>
  {
  public:
    using deleter = ObjectDestroyShared<Buffer>;
  };

  using SharedBuffer = SharedHandle<Buffer>;

  template <>
  class SharedHandleTraits<BufferView>
  {
  public:
    using deleter = ObjectDestroyShared<BufferView>;
  };

  using SharedBufferView = SharedHandle<BufferView>;

  template <>
  class SharedHandleTraits<Image>
  {
  public:
    using deleter = ObjectDestroyShared<Image>;
  };

  using SharedImage = SharedHandle<Image>;

  template <>
  class SharedHandleTraits<ImageView>
  {
  public:
    using deleter = ObjectDestroyShared<ImageView>;
  };

  using SharedImageView = SharedHandle<ImageView>;

  template <>
  class SharedHandleTraits<PipelineCache>
  {
  public:
    using deleter = ObjectDestroyShared<PipelineCache>;
  };

  using SharedPipelineCache = SharedHandle<PipelineCache>;

  template <>
  class SharedHandleTraits<Pipeline>
  {
  public:
    using deleter = ObjectDestroyShared<Pipeline>;
  };

  using SharedPipeline = SharedHandle<Pipeline>;

  template <>
  class SharedHandleTraits<PipelineLayout>
  {
  public:
    using deleter = ObjectDestroyShared<PipelineLayout>;
  };

  using SharedPipelineLayout = SharedHandle<PipelineLayout>;

  template <>
  class SharedHandleTraits<Sampler>
  {
  public:
    using deleter = ObjectDestroyShared<Sampler>;
  };

  using SharedSampler = SharedHandle<Sampler>;

  template <>
  class SharedHandleTraits<DescriptorSet>
  {
  public:
    using deleter = PoolFreeShared<DescriptorSet, DescriptorPool>;
  };

  using SharedDescriptorSet = SharedHandle<DescriptorSet>;

  template <>
  class SharedHandleTraits<DescriptorSetLayout>
  {
  public:
    using deleter = ObjectDestroyShared<DescriptorSetLayout>;
  };

  using SharedDescriptorSetLayout = SharedHandle<DescriptorSetLayout>;

  template <>
  class SharedHandleTraits<Framebuffer>
  {
  public:
    using deleter = ObjectDestroyShared<Framebuffer>;
  };

  using SharedFramebuffer = SharedHandle<Framebuffer>;

  template <>
  class SharedHandleTraits<RenderPass>
  {
  public:
    using deleter = ObjectDestroyShared<RenderPass>;
  };

  using SharedRenderPass = SharedHandle<RenderPass>;

  template <>
  class SharedHandleTraits<CommandBuffer>
  {
  public:
    using deleter = PoolFreeShared<CommandBuffer, CommandPool>;
  };

  using SharedCommandBuffer = SharedHandle<CommandBuffer>;

  //=== VK_VERSION_1_1 ===
  template <>
  class SharedHandleTraits<SamplerYcbcrConversion>
  {
  public:
    using deleter = ObjectDestroyShared<SamplerYcbcrConversion>;
  };

  using SharedSamplerYcbcrConversion    = SharedHandle<SamplerYcbcrConversion>;
  using SharedSamplerYcbcrConversionKHR = SharedHandle<SamplerYcbcrConversion>;

  //=== VK_VERSION_1_3 ===
  template <>
  class SharedHandleTraits<PrivateDataSlot>
  {
  public:
    using deleter = ObjectDestroyShared<PrivateDataSlot>;
  };

  using SharedPrivateDataSlot    = SharedHandle<PrivateDataSlot>;
  using SharedPrivateDataSlotEXT = SharedHandle<PrivateDataSlot>;

  //=== VK_KHR_surface ===
  template <>
  class SharedHandleTraits<SurfaceKHR>
  {
  public:
    using deleter = ObjectDestroyShared<SurfaceKHR>;
  };

  using SharedSurfaceKHR = SharedHandle<SurfaceKHR>;

  //=== VK_EXT_debug_utils ===
  template <>
  class SharedHandleTraits<DebugUtilsMessengerEXT>
  {
  public:
    using deleter = ObjectDestroyShared<DebugUtilsMessengerEXT>;
  };

  using SharedDebugUtilsMessengerEXT = SharedHandle<DebugUtilsMessengerEXT>;

  enum class SwapchainOwns
  {
    no,
    yes,
  };

  struct ImageHeader : SharedHeader<parent_of_t<VULKAN_HPP_NAMESPACE::Image>, typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter>
  {
    ImageHeader(
      SharedHandle<parent_of_t<VULKAN_HPP_NAMESPACE::Image>>            parent,
      typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter deleter        = typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter(),
      SwapchainOwns                                                     swapchainOwned = SwapchainOwns::no ) VULKAN_HPP_NOEXCEPT
      : SharedHeader<parent_of_t<VULKAN_HPP_NAMESPACE::Image>, typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter>( std::move( parent ),
                                                                                                                                   std::move( deleter ) )
      , swapchainOwned( swapchainOwned )
    {
    }

    SwapchainOwns swapchainOwned = SwapchainOwns::no;
  };

  template <>
  class SharedHandle<VULKAN_HPP_NAMESPACE::Image> : public SharedHandleBase<VULKAN_HPP_NAMESPACE::Image, ImageHeader>
  {
    using BaseType    = SharedHandleBase<VULKAN_HPP_NAMESPACE::Image, ImageHeader>;
    using DeleterType = typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::Image>::deleter;
    friend BaseType;

  public:
    using element_type = VULKAN_HPP_NAMESPACE::Image;

  public:
    SharedHandle() = default;

    explicit SharedHandle( VULKAN_HPP_NAMESPACE::Image                            handle,
                           SharedHandle<parent_of_t<VULKAN_HPP_NAMESPACE::Image>> parent,
                           SwapchainOwns                                          swapchain_owned = SwapchainOwns::no,
                           DeleterType                                            deleter         = DeleterType() ) VULKAN_HPP_NOEXCEPT
      : BaseType( handle, std::move( parent ), std::move( deleter ), swapchain_owned )
    {
    }

  protected:
    static void internalDestroy( const ImageHeader & control, VULKAN_HPP_NAMESPACE::Image handle ) VULKAN_HPP_NOEXCEPT
    {
      if ( control.swapchainOwned == SwapchainOwns::no )
      {
        control.deleter.destroy( control.parent.get(), handle );
      }
    }
  };

  struct SwapchainHeader
  {
    SwapchainHeader( SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR>                           surface,
                     SharedHandle<parent_of_t<VULKAN_HPP_NAMESPACE::SwapchainKHR>>            parent,
                     typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::SwapchainKHR>::deleter deleter =
                       typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::SwapchainKHR>::deleter() ) VULKAN_HPP_NOEXCEPT
      : surface( std::move( surface ) )
      , parent( std::move( parent ) )
      , deleter( std::move( deleter ) )
    {
    }

    SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR>                           surface{};
    SharedHandle<parent_of_t<VULKAN_HPP_NAMESPACE::SwapchainKHR>>            parent{};
    typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::SwapchainKHR>::deleter deleter{};
  };

  template <>
  class SharedHandle<VULKAN_HPP_NAMESPACE::SwapchainKHR> : public SharedHandleBase<VULKAN_HPP_NAMESPACE::SwapchainKHR, SwapchainHeader>
  {
    using BaseType    = SharedHandleBase<VULKAN_HPP_NAMESPACE::SwapchainKHR, SwapchainHeader>;
    using DeleterType = typename SharedHandleTraits<VULKAN_HPP_NAMESPACE::SwapchainKHR>::deleter;
    friend BaseType;

  public:
    using element_type = VULKAN_HPP_NAMESPACE::SwapchainKHR;

  public:
    SharedHandle() = default;

    explicit SharedHandle( VULKAN_HPP_NAMESPACE::SwapchainKHR                            handle,
                           SharedHandle<parent_of_t<VULKAN_HPP_NAMESPACE::SwapchainKHR>> parent,
                           SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR>                surface,
                           DeleterType                                                   deleter = DeleterType() ) VULKAN_HPP_NOEXCEPT
      : BaseType( handle, std::move( surface ), std::move( parent ), std::move( deleter ) )
    {
    }

  public:
    const SharedHandle<VULKAN_HPP_NAMESPACE::SurfaceKHR> & getSurface() const VULKAN_HPP_NOEXCEPT
    {
      return getHeader().surface;
    }

  protected:
    using BaseType::internalDestroy;
  };

  template <typename HandleType, typename ParentType>
  class SharedHandleBaseNoDestroy : public SharedHandleBase<HandleType, ParentType>
  {
    using BaseType = SharedHandleBase<HandleType, ParentType>;

  public:
    using BaseType::SharedHandleBase;

    const ParentType & getParent() const VULKAN_HPP_NOEXCEPT
    {
      return BaseType::getHeader();
    }

  protected:
    static void internalDestroy( const ParentType &, HandleType ) VULKAN_HPP_NOEXCEPT {}
  };

  //=== VK_VERSION_1_0 ===

  template <>
  class SharedHandle<PhysicalDevice> : public SharedHandleBaseNoDestroy<PhysicalDevice, SharedInstance>
  {
    friend SharedHandleBase<PhysicalDevice, SharedInstance>;

  public:
    using element_type = PhysicalDevice;
    using SharedHandleBaseNoDestroy<PhysicalDevice, SharedInstance>::SharedHandleBaseNoDestroy;
  };

  using SharedPhysicalDevice = SharedHandle<PhysicalDevice>;

  template <>
  class SharedHandle<Queue> : public SharedHandleBaseNoDestroy<Queue, SharedDevice>
  {
    friend SharedHandleBase<Queue, SharedDevice>;

  public:
    using element_type = Queue;
    using SharedHandleBaseNoDestroy<Queue, SharedDevice>::SharedHandleBaseNoDestroy;
  };

  using SharedQueue = SharedHandle<Queue>;

  template <>
  class SharedHandle<DeviceMemory> : public SharedHandleBaseNoDestroy<DeviceMemory, SharedDevice>
  {
    friend SharedHandleBase<DeviceMemory, SharedDevice>;

  public:
    using element_type = DeviceMemory;
    using SharedHandleBaseNoDestroy<DeviceMemory, SharedDevice>::SharedHandleBaseNoDestroy;
  };

  using SharedDeviceMemory = SharedHandle<DeviceMemory>;

  template <>
  class SharedHandle<QueryPool> : public SharedHandleBaseNoDestroy<QueryPool, SharedDevice>
  {
    friend SharedHandleBase<QueryPool, SharedDevice>;

  public:
    using element_type = QueryPool;
    using SharedHandleBaseNoDestroy<QueryPool, SharedDevice>::SharedHandleBaseNoDestroy;
  };

  using SharedQueryPool = SharedHandle<QueryPool>;

  template <>
  class SharedHandle<ShaderModule> : public SharedHandleBaseNoDestroy<ShaderModule, SharedDevice>
  {
    friend SharedHandleBase<ShaderModule, SharedDevice>;

  public:
    using element_type = ShaderModule;
    using SharedHandleBaseNoDestroy<ShaderModule, SharedDevice>::SharedHandleBaseNoDestroy;
  };

  using SharedShaderModule = SharedHandle<ShaderModule>;

  template <>
  class SharedHandle<DescriptorPool> : public SharedHandleBaseNoDestroy<DescriptorPool, SharedDevice>
  {
    friend SharedHandleBase<DescriptorPool, SharedDevice>;

  public:
    using element_type = DescriptorPool;
    using SharedHandleBaseNoDestroy<DescriptorPool, SharedDevice>::SharedHandleBaseNoDestroy;
  };

  using SharedDescriptorPool = SharedHandle<DescriptorPool>;

  template <>
  class SharedHandle<CommandPool> : public SharedHandleBaseNoDestroy<CommandPool, SharedDevice>
  {
    friend SharedHandleBase<CommandPool, SharedDevice>;

  public:
    using element_type = CommandPool;
    using SharedHandleBaseNoDestroy<CommandPool, SharedDevice>::SharedHandleBaseNoDestroy;
  };

  using SharedCommandPool = SharedHandle<CommandPool>;

  //=== VK_KHR_swapchain ===

  template <>
  class SharedHandle<SwapchainKHR> : public SharedHandleBaseNoDestroy<SwapchainKHR, SharedDevice>
  {
    friend SharedHandleBase<SwapchainKHR, SharedDevice>;

  public:
    using element_type = SwapchainKHR;
    using SharedHandleBaseNoDestroy<SwapchainKHR, SharedDevice>::SharedHandleBaseNoDestroy;
  };

  using SharedSwapchainKHR = SharedHandle<SwapchainKHR>;

  //=== VK_KHR_display ===

  template <>
  class SharedHandle<DisplayKHR> : public SharedHandleBaseNoDestroy<DisplayKHR, SharedPhysicalDevice>
  {
    friend SharedHandleBase<DisplayKHR, SharedPhysicalDevice>;

  public:
    using element_type = DisplayKHR;
    using SharedHandleBaseNoDestroy<DisplayKHR, SharedPhysicalDevice>::SharedHandleBaseNoDestroy;
  };

  using SharedDisplayKHR = SharedHandle<DisplayKHR>;

  template <>
  class SharedHandle<DisplayModeKHR> : public SharedHandleBaseNoDestroy<DisplayModeKHR, SharedDisplayKHR>
  {
    friend SharedHandleBase<DisplayModeKHR, SharedDisplayKHR>;

  public:
    using element_type = DisplayModeKHR;
    using SharedHandleBaseNoDestroy<DisplayModeKHR, SharedDisplayKHR>::SharedHandleBaseNoDestroy;
  };

  using SharedDisplayModeKHR = SharedHandle<DisplayModeKHR>;

#  if defined( VK_USE_PLATFORM_SCI )
  //=== VK_NV_external_sci_sync2 ===

  template <>
  class SharedHandle<SemaphoreSciSyncPoolNV> : public SharedHandleBaseNoDestroy<SemaphoreSciSyncPoolNV, SharedDevice>
  {
    friend SharedHandleBase<SemaphoreSciSyncPoolNV, SharedDevice>;

  public:
    using element_type = SemaphoreSciSyncPoolNV;
    using SharedHandleBaseNoDestroy<SemaphoreSciSyncPoolNV, SharedDevice>::SharedHandleBaseNoDestroy;
  };

  using SharedSemaphoreSciSyncPoolNV = SharedHandle<SemaphoreSciSyncPoolNV>;
#  endif /*VK_USE_PLATFORM_SCI*/
#endif   // !VULKAN_HPP_NO_SMART_HANDLE
}  // namespace VULKAN_HPP_NAMESPACE
#endif  // VULKAN_SHARED_HPP
