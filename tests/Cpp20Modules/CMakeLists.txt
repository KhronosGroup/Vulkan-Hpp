cmake_minimum_required( VERSION 3.25 )

vulkan_hpp__setup_test( NAME Cpp20Modules )
target_compile_features( Cpp20Modules PRIVATE cxx_std_20 )
target_link_libraries( Cpp20Modules PRIVATE VulkanHppModule )

if( NOT ( VULKAN_HPP_SAMPLES_BUILD_ONLY_DYNAMIC AND VULKAN_HPP_SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP ) )
	find_package(Vulkan REQUIRED)
	target_link_libraries( Cpp20Modules PRIVATE Vulkan::Vulkan )
endif()