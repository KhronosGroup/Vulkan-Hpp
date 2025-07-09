${licenseHeader}
#ifndef VULKAN_RAII_HPP
#define VULKAN_RAII_HPP

#include <vulkan/${api}.hpp>
#if !( defined( VULKAN_HPP_ENABLE_STD_MODULE ) && defined( VULKAN_HPP_STD_MODULE ) )
#  include <memory>   // std::unique_ptr
#  include <utility>  // std::forward
#endif

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
namespace VULKAN_HPP_NAMESPACE
{
  namespace VULKAN_HPP_RAII_NAMESPACE
  {
    namespace detail
    {
      template <class T>
      class CreateReturnType
      {
        public:
#if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
          using Type = VULKAN_HPP_EXPECTED<T, Result>;
#else
          using Type = T;
#endif
      };

      using PFN_dummy = void ( * )();

      ${RAIIDispatchers}
    }

    ${RAIIHandles}
    ${RAIICommandDefinitions}

    //====================
    //=== RAII Helpers ===
    //====================

    template <typename RAIIType>
    std::vector<typename RAIIType::CppType> filterCppTypes( std::vector<RAIIType> const & raiiTypes )
    {
      std::vector<typename RAIIType::CppType> cppTypes( raiiTypes.size() );
      std::transform( raiiTypes.begin(), raiiTypes.end(), cppTypes.begin(), []( RAIIType const & d ) { return *d; } );
      return cppTypes;
    }

    template <typename RAIIType, class UnaryPredicate>
    std::vector<typename RAIIType::CppType> filterCppTypes( std::vector<RAIIType> const & raiiTypes, UnaryPredicate p)
    {
      std::vector<typename RAIIType::CppType> cppTypes;
      for (auto const& t : raiiTypes)
      {
        if (p(t))
        {
          cppTypes.push_back( *t );
        }
      }
      return cppTypes;
    }

  } // namespace VULKAN_HPP_RAII_NAMESPACE
}   // namespace VULKAN_HPP_NAMESPACE
#endif
#endif