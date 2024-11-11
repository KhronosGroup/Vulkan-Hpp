${licenseHeader}

#ifndef VULKAN_EXTENSION_INSPECTION_HPP
#  define VULKAN_EXTENSION_INSPECTION_HPP

#if defined( VULKAN_HPP_ENABLE_STD_MODULE ) && defined( VULKAN_HPP_STD_MODULE )
import VULKAN_HPP_STD_MODULE;
#else
#  include <map>
#  include <set>
#  include <string>
#  include <vector>
#  include <vulkan/${api}.hpp>
#endif

namespace VULKAN_HPP_NAMESPACE
{
  //======================================
  //=== Extension inspection functions ===
  //======================================

  std::set<std::string> const &                                        getDeviceExtensions();
  std::set<std::string> const &                                        getInstanceExtensions();
  std::map<std::string, std::string> const &                           getDeprecatedExtensions();
  std::map<std::string, std::vector<std::vector<std::string>>> const & getExtensionDepends( std::string const & extension );
  ${getExtensionDependsByVersionDeclaration}
  std::map<std::string, std::string> const &              getObsoletedExtensions();
  std::map<std::string, std::string> const &              getPromotedExtensions();
  VULKAN_HPP_CONSTEXPR_20 std::string getExtensionDeprecatedBy( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 std::string getExtensionObsoletedBy( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 std::string getExtensionPromotedTo( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 bool        isDeprecatedExtension( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 bool        isDeviceExtension( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 bool        isInstanceExtension( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 bool        isObsoletedExtension( std::string const & extension );
  VULKAN_HPP_CONSTEXPR_20 bool        isPromotedExtension( std::string const & extension );

  //=====================================================
  //=== Extension inspection function implementations ===
  //=====================================================

  VULKAN_HPP_INLINE std::map<std::string, std::string> const & getDeprecatedExtensions()
  {
    static const std::map<std::string, std::string> deprecatedExtensions = { ${deprecatedExtensions} };
    return deprecatedExtensions;
  }

  VULKAN_HPP_INLINE std::set<std::string> const & getDeviceExtensions()
  {
    static const std::set<std::string> deviceExtensions = { ${deviceExtensions} };
    return deviceExtensions;
  }

  VULKAN_HPP_INLINE std::set<std::string> const & getInstanceExtensions()
  {
    static const std::set<std::string> instanceExtensions = { ${instanceExtensions} };
    return instanceExtensions;
  }

  VULKAN_HPP_INLINE std::map<std::string, std::vector<std::vector<std::string>>> const & getExtensionDepends( std::string const & extension )
  {
    static const std::map<std::string, std::vector<std::vector<std::string>>> noDependencies;
    static const std::map<std::string, std::map<std::string, std::vector<std::vector<std::string>>>> dependencies = { ${extensionDependencies} };
    auto depIt = dependencies.find( extension );
    return ( depIt != dependencies.end() ) ? depIt->second : noDependencies;
  }

  ${getExtensionDependsByVersionDefinition}

  VULKAN_HPP_INLINE std::map<std::string, std::string> const & getObsoletedExtensions()
  {
    static const std::map<std::string, std::string> obsoletedExtensions = { ${obsoletedExtensions} };
    return obsoletedExtensions;
  }

  VULKAN_HPP_INLINE std::map<std::string, std::string> const & getPromotedExtensions()
  {
    static const std::map<std::string, std::string> promotedExtensions = { ${promotedExtensions} };
    return promotedExtensions;
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 std::string getExtensionDeprecatedBy( std::string const & extension )
  {
    ${voidExtension}
    ${deprecatedBy}
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 std::string getExtensionObsoletedBy( std::string const & extension )
  {
    ${voidExtension}
    ${obsoletedBy}
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 std::string getExtensionPromotedTo( std::string const & extension )
  {
    ${promotedTo}
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isDeprecatedExtension( std::string const & extension )
  {
    ${voidExtension}
    return ${deprecatedTest};
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isDeviceExtension( std::string const & extension )
  {
    return ${deviceTest};
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isInstanceExtension( std::string const & extension )
  {
    return ${instanceTest};
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isObsoletedExtension( std::string const & extension )
  {
    ${voidExtension}
    return ${obsoletedTest};
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 bool isPromotedExtension( std::string const & extension )
  {
    return ${promotedTest};
  }
}   // namespace VULKAN_HPP_NAMESPACE

#endif
