cmake_minimum_required( VERSION 3.25 )

vulkan_hpp__setup_test( NAME Cpp20Modules CXX_STANDARD 20 LIBRARIES VulkanHppModule NO_UTILS )

if( NOT VULKAN_HPP_SAMPLES_BUILD_ONLY_DYNAMIC )
	if ( VULKAN_HPP_CPP20_MODULE_DYNAMIC_DISPATCHER )
		target_compile_definitions( Cpp20Modules PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 )
	else()
		target_compile_definitions( Cpp20Modules PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=0 )
	endif()
	target_link_libraries( Cpp20Modules PRIVATE Vulkan::Vulkan )
	set_target_properties( Cpp20Modules PROPERTIES CXX_EXTENSIONS OFF
	)
endif()