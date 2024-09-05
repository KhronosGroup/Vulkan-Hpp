cmake_minimum_required( VERSION 3.30 )

target_compile_features( VulkanHppModule PUBLIC cxx_std_23 )
target_compile_definitions( VulkanHppModule PUBLIC VULKAN_HPP_ENABLE_STD_MODULE )

vulkan_hpp__setup_test( NAME CppStdModule CXX_STANDARD 23 LIBRARIES VulkanHppModule NO_UTILS )

target_compile_features( CppStdModule PUBLIC cxx_std_23 )

if( NOT VULKAN_HPP_SAMPLES_BUILD_ONLY_DYNAMIC )
	if ( VULKAN_HPP_CPP20_MODULE_DYNAMIC_DISPATCHER )
		target_compile_definitions( CppStdModule PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 )
	else()
		target_compile_definitions( CppStdModule PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=0 )
	endif()
	target_link_libraries( CppStdModule PRIVATE Vulkan::Vulkan )
	set_target_properties( CppStdModule PROPERTIES CXX_EXTENSIONS OFF )
endif()