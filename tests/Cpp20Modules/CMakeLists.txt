cmake_minimum_required( VERSION 3.25 )

vulkan_hpp__setup_test( NAME Cpp20Modules CXX_STANDARD 20 LIBRARIES VulkanHppModule )

if( NOT VULKAN_HPP_SAMPLES_BUILD_ONLY_DYNAMIC )
	target_link_libraries( Cpp20Modules PRIVATE Vulkan::Vulkan )
	set_target_properties( Cpp20Modules PROPERTIES 
		CXX_EXTENSIONS OFF
	)
endif()