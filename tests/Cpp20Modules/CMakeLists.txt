cmake_minimum_required( VERSION 3.25 )

vulkan_hpp__setup_test( NAME Cpp20Modules CXX_STANDARD 20 LIBRARIES VulkanHppModule )

if( NOT ( VULKAN_HPP_SAMPLES_BUILD_ONLY_DYNAMIC AND VULKAN_HPP_SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP ) )
	target_link_libraries( Cpp20Modules PRIVATE Vulkan::Vulkan )
endif()