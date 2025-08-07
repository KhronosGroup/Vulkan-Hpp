cmake_minimum_required( VERSION 3.30 )

vulkan_hpp__setup_test( NAME CppStdModule LIBRARIES VulkanHppModule NO_UTILS )
set_target_properties( CppStdModule PROPERTIES CXX_SCAN_FOR_MODULES ON )