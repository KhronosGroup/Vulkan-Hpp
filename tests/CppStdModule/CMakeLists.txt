cmake_minimum_required( VERSION 3.30 )

# TODO: use VULKAN_HPP_DISPATCH_LOADER_[DYNAMIC|STATIC]_TYPE (PR #2244)
vulkan_hpp__setup_test( NAME CppStdModule CXX_MODULE )