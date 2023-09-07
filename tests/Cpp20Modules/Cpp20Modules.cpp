import vulkan_hpp;

#include <iostream>
#include <vulkan/vulkan_hpp_macros.hpp>

static std::string AppName    = "Cpp20Modules";
static std::string EngineName = "Vulkan.cppm";

#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

int main( int /*argc*/, char ** /*argv*/ )
{
  /* VULKAN_HPP_KEY_START */
  try
  {
#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
    // initialize minimal set of function pointers
    VULKAN_HPP_DEFAULT_DISPATCHER.init();
#endif

    // initialize the vk::ApplicationInfo structure
    vk::ApplicationInfo applicationInfo( AppName.c_str(), 1, EngineName.c_str(), 1, vk::makeApiVersion( 1, 0, 0, 0 ) );

    // initialize the vk::InstanceCreateInfo
    vk::InstanceCreateInfo instanceCreateInfo( {}, &applicationInfo );

    // create an Instance
    vk::Instance instance = vk::createInstance( instanceCreateInfo );

#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
    // initialize function pointers for instance
    VULKAN_HPP_DEFAULT_DISPATCHER.init( instance );
#endif

    // destroy it again
    instance.destroy();
  }
  catch ( vk::SystemError & err )
  {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( std::exception & err )
  {
    std::cout << "std::exception: " << err.what() << std::endl;
    exit( -1 );
  }
  catch ( ... )
  {
    std::cout << "unknown error\n";
    exit( -1 );
  }

  /* VULKAN_HPP_KEY_END */

  return 0;
}
