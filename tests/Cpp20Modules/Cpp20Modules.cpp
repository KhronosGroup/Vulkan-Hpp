import vulkan;

#include <iostream>

static std::string AppName    = "01_InitInstance";
static std::string EngineName = "Vulkan.hpp";

int main( int /*argc*/, char ** /*argv*/ )
{
  /* VULKAN_HPP_KEY_START */
  try
  {
    // initialize the vk::ApplicationInfo structure
    vk::ApplicationInfo applicationInfo( AppName.c_str(), 1, EngineName.c_str(), 1, vk::cMakeApiVersion( 1, 0, 0, 0 ) );

    // initialize the vk::InstanceCreateInfo
    vk::InstanceCreateInfo instanceCreateInfo( {}, &applicationInfo );

    // create an Instance
    vk::Instance instance = vk::createInstance( instanceCreateInfo );

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
