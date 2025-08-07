cmake_minimum_required( VERSION 3.28 )

vulkan_hpp__setup_test( NAME Cpp20Modules LIBRARIES VulkanHppModule NO_UTILS )
set_target_properties( Cpp20Modules PROPERTIES CXX_SCAN_FOR_MODULES ON )