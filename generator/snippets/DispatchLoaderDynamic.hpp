// SPDX-FileCopyrightText: 2026 The Khronos Group, Inc.
// SPDX-License-Identifier: Apache-2.0

using PFN_dummy = void ( * )();

class DispatchLoaderDynamic : public DispatchLoaderBase
{
public:
        ${commandMembers}

public:
    DispatchLoaderDynamic() VULKAN_HPP_NOEXCEPT = default;
    DispatchLoaderDynamic( DispatchLoaderDynamic const & rhs ) VULKAN_HPP_NOEXCEPT = default;

    DispatchLoaderDynamic(PFN_vkGetInstanceProcAddr getInstanceProcAddr) VULKAN_HPP_NOEXCEPT
    {
        init(getInstanceProcAddr);
    }

    // This interface does not require a linked vulkan library.
    DispatchLoaderDynamic( VkInstance                instance,
                            PFN_vkGetInstanceProcAddr getInstanceProcAddr) VULKAN_HPP_NOEXCEPT
    {
        init( instance, getInstanceProcAddr );
    }

    // This interface does not require a linked vulkan library.
    DispatchLoaderDynamic( VkInstance                instance,
                            PFN_vkGetInstanceProcAddr getInstanceProcAddr,
                            VkDevice                  device            = {},
                            PFN_vkGetDeviceProcAddr   getDeviceProcAddr = nullptr ) VULKAN_HPP_NOEXCEPT
    {
        init( instance, getInstanceProcAddr, device, getDeviceProcAddr );
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
        VULKAN_HPP_ASSERT(getInstanceProcAddr);

        vkGetInstanceProcAddr = getInstanceProcAddr;

    ${initialCommandAssignments}
    }

    // This interface does not require a linked vulkan library.
    void init( VkInstance                instance,
               PFN_vkGetInstanceProcAddr getInstanceProcAddr) VULKAN_HPP_NOEXCEPT
    {
        VULKAN_HPP_ASSERT(instance && getInstanceProcAddr);
        vkGetInstanceProcAddr = getInstanceProcAddr;
        init( Instance(instance) );
    }

    // This interface does not require a linked vulkan library.
    void init( VkInstance                instance,
               PFN_vkGetInstanceProcAddr getInstanceProcAddr,
               VkDevice                  device,
               PFN_vkGetDeviceProcAddr   getDeviceProcAddr ) VULKAN_HPP_NOEXCEPT
    {
        init( instance, getInstanceProcAddr );
        VULKAN_HPP_ASSERT( device && getDeviceProcAddr );
        vkGetDeviceProcAddr = getDeviceProcAddr;
        init( Device(device) );
    }

    void init( Instance instanceCpp ) VULKAN_HPP_NOEXCEPT
    {
        VkInstance instance = static_cast<VkInstance>( instanceCpp );

    ${instanceCommandAssignments}
    }

    void init( Device deviceCpp ) VULKAN_HPP_NOEXCEPT
    {
        VkDevice device = static_cast<VkDevice>( deviceCpp );

    ${deviceCommandAssignments}
    }

    template <typename DynamicLoader>
    void init( Instance const & instance, Device const & device, DynamicLoader const & dl ) VULKAN_HPP_NOEXCEPT
    {
        PFN_vkGetInstanceProcAddr getInstanceProcAddr = dl.template getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
        PFN_vkGetDeviceProcAddr getDeviceProcAddr = dl.template getProcAddress<PFN_vkGetDeviceProcAddr>( "vkGetDeviceProcAddr" );
        init( static_cast<VkInstance>( instance ), getInstanceProcAddr, static_cast<VkDevice>( device ), device ? getDeviceProcAddr : nullptr );
    }

    template <typename DynamicLoader
    #if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
        = VULKAN_HPP_NAMESPACE::detail::DynamicLoader
    #endif
    >
    void init( Instance const & instance, Device const & device ) VULKAN_HPP_NOEXCEPT
    {
        static DynamicLoader dl;
        init(instance, device, dl);
    }
};