// Copyright(c) 2015-2020, NVIDIA CORPORATION. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "VulkanHppGenerator.hpp"

#include "XMLHelper.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <future>
#include <regex>
#include <sstream>

using namespace std::literals;

namespace
{
  std::vector<std::pair<std::string, size_t>> filterNumbers( std::vector<std::string> const & names );
  template <typename T>
  typename std::vector<T>::iterator findByName( std::vector<T> & values, std::string const & name );
  template <typename T>
  typename std::map<std::string, T>::const_iterator findByNameOrAlias( std::map<std::string, T> const & values, std::string const & name );
  template <typename T>
  typename std::map<std::string, T>::iterator findByNameOrAlias( std::map<std::string, T> & values, std::string const & name );
  template <typename T>
  typename std::vector<T>::const_iterator findByNameOrAlias( std::vector<T> const & values, std::string const & name );
  template <typename T>
  typename std::vector<T>::iterator findByNameOrAlias( std::vector<T> & values, std::string const & name );
  std::string                       generateCArraySizes( std::vector<std::string> const & sizes );
  std::string                       generateList( std::vector<std::string> const & elements, std::string const & prefix, std::string const & separator );
  std::string                       generateNamespacedType( std::string const & type );
  std::string                       generateNoDiscard( bool returnsSomething, bool multiSuccessCodes, bool multiErrorCodes );
  std::string                       generateStandardArray( std::string const & type, std::vector<std::string> const & sizes );
  bool                              isAllUpper( std::string const & name );
  VulkanHppGenerator::MacroData     parseMacro( std::vector<std::string> const & completeMacro );
  std::string                       readSnippet( std::string const & snippetFile );
  std::string                       startLowerCase( std::string const & input );
  std::string                       startUpperCase( std::string const & input );
  std::vector<std::string>          tokenizeAny( std::string const & tokenString, std::string const & separators );
}  // namespace

void writeToFile( std::string const & str, std::string const & fileName );

const std::set<std::string> specialPointerTypes = { "Display", "IDirectFB", "wl_display", "xcb_connection_t", "_screen_window" };

//
// VulkanHppGenerator public interface
//

VulkanHppGenerator::VulkanHppGenerator( tinyxml2::XMLDocument const & document, std::string const & api ) : m_api( api )
{
  // insert the default "handle" without class (for createInstance, and such)
  m_handles.insert( std::make_pair( "", HandleData() ) );

  // read the document and check its correctness
  const int                                 line     = document.GetLineNum();
  std::vector<tinyxml2::XMLElement const *> elements = getChildElements( &document );
  checkElements( line, elements, { { "registry", true } } );
  checkForError( elements.size() == 1, line, "encountered " + std::to_string( elements.size() ) + " elements named <registry> but only one is allowed" );
  readRegistry( elements[0] );
  filterLenMembers();
  checkCorrectness();
  handleRemovals();

  // add the commands to the respective handles
  // some "FlagBits" enums are not specified, but needed for our "Flags" handling -> add them here
  for ( auto & feature : m_features )
  {
    addCommandsToHandle( feature.requireData );
    addMissingFlagBits( feature.requireData, feature.name );
  }
  for ( auto & extension : m_extensions )
  {
    addCommandsToHandle( extension.requireData );
    addMissingFlagBits( extension.requireData, extension.name );
  }

  m_definesPartition = partitionDefines( m_defines );
}

void VulkanHppGenerator::distributeSecondLevelCommands()
{
  // distribute commands from instance/device to second-level handles, like Queue, Event,... for RAII handles
  for ( auto & handle : m_handles )
  {
    if ( !handle.first.empty() )
    {
      for ( auto command = handle.second.commands.begin(); command != handle.second.commands.end(); )
      {
        bool foundCommand = false;
        if ( !m_RAIISpecialFunctions.contains( *command ) )
        {
          auto commandIt = findByNameOrAlias( m_commands, *command );
          assert( commandIt != m_commands.end() );
          assert( commandIt->second.params.front().type.type == handle.first );
          if ( ( 1 < commandIt->second.params.size() ) && ( isHandleType( commandIt->second.params[1].type.type ) ) && !commandIt->second.params[1].optional )
          {
            auto handleIt = m_handles.find( commandIt->second.params[1].type.type );
            assert( handleIt != m_handles.end() );
            // filter out functions seem to fit due to taking handles as first and second argument, but the first argument is not the
            // type to create the second one, and so it's unknown to the raii handle!
            assert( !handleIt->second.constructorIts.empty() );
            if ( ( *handleIt->second.constructorIts.begin() )->second.handle == handle.first )
            {
              assert( std::none_of( handleIt->second.constructorIts.begin(),
                                    handleIt->second.constructorIts.end(),
                                    [&handle]( auto const & constructorIt ) { return constructorIt->second.handle != handle.first; } ) );
              handleIt->second.secondLevelCommands.insert( *command );
              command      = handle.second.commands.erase( command );
              foundCommand = true;
            }
          }
        }
        if ( !foundCommand )
        {
          ++command;
        }
      }
    }
  }
}

void VulkanHppGenerator::generateEnumsHppFile() const
{
  std::string const vulkan_enums_hpp = std::string( BASE_PATH ) + "/vulkan/" + m_api + "_enums.hpp";
  messager.message( "VulkanHppGenerator: Generating " + vulkan_enums_hpp + " ...\n" );

  std::string const vulkanEnumsHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_ENUMS_HPP
#  define VULKAN_ENUMS_HPP

// include-what-you-use: make sure, vulkan.hpp is used by code-completers
// IWYU pragma: private; include "vulkan.hpp"

namespace VULKAN_HPP_NAMESPACE
{
${Flags}

${enums}
${objectTypeToDebugReportObjectType}
}   // namespace VULKAN_HPP_NAMESPACE
#endif
)";

  std::string str = replaceWithMap( vulkanEnumsHppTemplate,
                                    { { "enums", generateEnums() },
                                      { "Flags", readSnippet( "Flags.hpp" ) },
                                      { "licenseHeader", m_vulkanLicenseHeader },
                                      { "objectTypeToDebugReportObjectType", generateObjectTypeToDebugReportObjectType() } } );

  writeToFile( str, vulkan_enums_hpp );
}

void VulkanHppGenerator::generateExtensionInspectionFile() const
{
  std::string const vulkan_extension_inspection_hpp = std::string( BASE_PATH ) + "/vulkan/" + m_api + "_extension_inspection.hpp";
  messager.message( "VulkanHppGenerator: Generating " + vulkan_extension_inspection_hpp + " ...\n" );

  std::string const vulkanExtensionInspectionHppTemplate = readSnippet( "ExtensionInspection.hpp" );
  std::string const deprecatedExtensions = generateReplacedExtensionsList( []( ExtensionData const & extension ) { return extension.isDeprecated; },
                                                                           []( ExtensionData const & extension ) { return extension.deprecatedBy; } );
  std::string const deprecatedBy         = generateExtensionReplacedBy( []( ExtensionData const & extension ) { return extension.isDeprecated; },
                                                                []( ExtensionData const & extension ) { return extension.deprecatedBy; } );
  std::string const obsoletedBy          = generateExtensionReplacedBy( []( ExtensionData const & extension ) { return !extension.obsoletedBy.empty(); },
                                                               []( ExtensionData const & extension ) { return extension.obsoletedBy; } );
  std::string const obsoletedExtensions  = generateReplacedExtensionsList( []( ExtensionData const & extension ) { return !extension.obsoletedBy.empty(); },
                                                                          []( ExtensionData const & extension ) { return extension.obsoletedBy; } );
  std::string const promotedExtensions   = generateReplacedExtensionsList( []( ExtensionData const & extension ) { return !extension.promotedTo.empty(); },
                                                                         []( ExtensionData const & extension ) { return extension.promotedTo; } );
  std::string const promotedTo           = generateExtensionReplacedBy( []( ExtensionData const & extension ) { return !extension.promotedTo.empty(); },
                                                              []( ExtensionData const & extension ) { return extension.promotedTo; } );

  std::string str =
    replaceWithMap( vulkanExtensionInspectionHppTemplate,
                    { { "api", m_api },
                      { "deprecatedExtensions", deprecatedExtensions },
                      { "deviceExtensions", generateExtensionsList( "device" ) },
                      { "deviceTest", generateExtensionTypeTest( "device" ) },
                      { "deprecatedBy", deprecatedBy },
                      { "deprecatedTest", generateExtensionReplacedTest( []( ExtensionData const & extension ) { return extension.isDeprecated; } ) },
                      { "extensionDependencies", generateExtensionDependencies() },
                      { "getExtensionDependsByVersionDeclaration", generateExtensionDependsByVersion( false ) },
                      { "getExtensionDependsByVersionDefinition", generateExtensionDependsByVersion( true ) },
                      { "instanceExtensions", generateExtensionsList( "instance" ) },
                      { "instanceTest", generateExtensionTypeTest( "instance" ) },
                      { "licenseHeader", m_vulkanLicenseHeader },
                      { "obsoletedBy", obsoletedBy },
                      { "obsoletedExtensions", obsoletedExtensions },
                      { "obsoletedTest", generateExtensionReplacedTest( []( ExtensionData const & extension ) { return !extension.obsoletedBy.empty(); } ) },
                      { "promotedExtensions", promotedExtensions },
                      { "promotedTest", generateExtensionReplacedTest( []( ExtensionData const & extension ) { return !extension.promotedTo.empty(); } ) },
                      { "promotedTo", promotedTo },
                      { "voidExtension", ( m_api == "vulkan" ) ? "" : "(void)extension;" } } );

  writeToFile( str, vulkan_extension_inspection_hpp );
}

void VulkanHppGenerator::generateFormatTraitsHppFile() const
{
  std::string const vulkan_format_traits_hpp = std::string( BASE_PATH ) + "/vulkan/" + m_api + "_format_traits.hpp";
  messager.message( "VulkanHppGenerator: Generating " + vulkan_format_traits_hpp + " ...\n" );

  std::string const vulkanFormatTraitsHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_FORMAT_TRAITS_HPP
#  define VULKAN_FORMAT_TRAITS_HPP

#include <vulkan/${api}.hpp>

namespace VULKAN_HPP_NAMESPACE
{
${formatTraits}
}   // namespace VULKAN_HPP_NAMESPACE
#endif
)";

  std::string str = replaceWithMap( vulkanFormatTraitsHppTemplate,
                                    { { "api", m_api }, { "formatTraits", generateFormatTraits() }, { "licenseHeader", m_vulkanLicenseHeader } } );

  writeToFile( str, vulkan_format_traits_hpp );
}

void VulkanHppGenerator::generateFuncsHppFile() const
{
  std::string const vulkan_funcs_hpp = std::string( BASE_PATH ) + "/vulkan/" + m_api + "_funcs.hpp";
  messager.message( "VulkanHppGenerator: Generating " + vulkan_funcs_hpp + " ...\n" );

  std::string const vulkanFuncsHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_FUNCS_HPP
#  define VULKAN_FUNCS_HPP

// include-what-you-use: make sure, vulkan.hpp is used by code-completers
// IWYU pragma: private; include "vulkan.hpp"

namespace VULKAN_HPP_NAMESPACE
{
${commandDefinitions}
}   // namespace VULKAN_HPP_NAMESPACE
#endif
)";

  std::string str =
    replaceWithMap( vulkanFuncsHppTemplate, { { "commandDefinitions", generateCommandDefinitions() }, { "licenseHeader", m_vulkanLicenseHeader } } );

  writeToFile( str, vulkan_funcs_hpp );
}

void VulkanHppGenerator::generateHandlesHppFile() const
{
  std::string const vulkan_handles_hpp = std::string( BASE_PATH ) + "/vulkan/" + m_api + "_handles.hpp";
  messager.message( "VulkanHppGenerator: Generating " + vulkan_handles_hpp + " ...\n" );

  std::string const vulkanHandlesHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_HANDLES_HPP
#  define VULKAN_HANDLES_HPP

// include-what-you-use: make sure, vulkan.hpp is used by code-completers
// IWYU pragma: private; include "vulkan.hpp"

namespace VULKAN_HPP_NAMESPACE
{
  ${structForwardDeclarations}
  ${handleForwardDeclarations}
  ${funcPointerReturns}
  ${uniqueHandles}
  ${handles}

  // operators to compare vk::-handles
#  if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::isVulkanHandleType<T>::value, int>::type = 0>
  auto operator<=>( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) <=> static_cast<typename T::NativeType>( rhs );
  }
#else
  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::isVulkanHandleType<T>::value, int>::type = 0>
  bool operator==( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) == static_cast<typename T::NativeType>( rhs );
  }

  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::isVulkanHandleType<T>::value, int>::type = 0>
  bool operator!=( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) != static_cast<typename T::NativeType>( rhs );
  }

  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::isVulkanHandleType<T>::value, int>::type = 0>
  bool operator<( T const & lhs, T const & rhs )
  {
    return static_cast<typename T::NativeType>( lhs ) < static_cast<typename T::NativeType>( rhs );
  }
#endif

  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::isVulkanHandleType<T>::value, int>::type = 0>
  bool operator==( T const & v, std::nullptr_t )
  {
    return !v;
  }

  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::isVulkanHandleType<T>::value, int>::type = 0>
  bool operator==( std::nullptr_t, T const & v )
  {
    return !v;
  }

  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::isVulkanHandleType<T>::value, int>::type = 0>
  bool operator!=( T const & v, std::nullptr_t )
  {
    return !!v;
  }

  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::isVulkanHandleType<T>::value, int>::type = 0>
  bool operator!=( std::nullptr_t, T const & v )
  {
    return !!v;
  }
}   // namespace VULKAN_HPP_NAMESPACE
#endif
)";

  std::string str = replaceWithMap( vulkanHandlesHppTemplate,
                                    {
                                      { "funcPointerReturns", generateFuncPointerReturns() },
                                      { "handles", generateHandles() },
                                      { "handleForwardDeclarations", generateHandleForwardDeclarations() },
                                      { "licenseHeader", m_vulkanLicenseHeader },
                                      { "structForwardDeclarations", generateStructForwardDeclarations() },
                                      { "uniqueHandles", generateUniqueHandles() },
                                    } );

  writeToFile( str, vulkan_handles_hpp );
}

void VulkanHppGenerator::generateHashHppFile() const
{
  std::string const vulkan_hash_hpp = std::string( BASE_PATH ) + "/vulkan/" + m_api + "_hash.hpp";
  messager.message( "VulkanHppGenerator: Generating " + vulkan_hash_hpp + " ...\n" );

  std::string const vulkanHandlesHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_HASH_HPP
#  define VULKAN_HASH_HPP

#include <vulkan/${api}.hpp>

namespace std
{
  //=======================================
  //=== HASH structures for Flags types ===
  //=======================================

  template <typename BitType>
  struct hash<VULKAN_HPP_NAMESPACE::Flags<BitType>>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Flags<BitType> const & flags ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<typename std::underlying_type<BitType>::type>{}(
        static_cast<typename std::underlying_type<BitType>::type>( flags ) );
    }
  };

${handleHashStructures}
${structHashStructures}
} // namespace std
#endif
)";

  std::string str = replaceWithMap( vulkanHandlesHppTemplate,
                                    { { "api", m_api },
                                      { "handleHashStructures", generateHandleHashStructures() },
                                      { "licenseHeader", m_vulkanLicenseHeader },
                                      { "structHashStructures", generateStructHashStructures() } } );

  writeToFile( str, vulkan_hash_hpp );
}

void VulkanHppGenerator::generateHppFile() const
{
  std::string const vulkan_hpp = std::string( BASE_PATH ) + "/vulkan/" + m_api + ".hpp";
  messager.message( "VulkanHppGenerator: Generating " + vulkan_hpp + " ...\n" );

  std::string const vulkanHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_HPP
#  define VULKAN_HPP

${includes}

static_assert( VK_HEADER_VERSION == ${headerVersion}, "Wrong VK_HEADER_VERSION!" );

${defines}

namespace VULKAN_HPP_NAMESPACE
{
  ${ArrayWrapper1D}
  ${ArrayWrapper2D}
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
  ${ArrayProxy}
  ${ArrayProxyNoTemporaries}
  ${StridedArrayProxy}
  ${Optional}
  ${StructureChain}
  ${UniqueHandle}
#endif  // VULKAN_HPP_DISABLE_ENHANCED_MODE

  namespace detail
  {
    ${DispatchLoaderBase}
    ${DispatchLoaderStatic}
  }
  ${Exchange}

#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
  struct AllocationCallbacks;

  namespace detail
  {
    ${ObjectDestroy}
    ${ObjectFree}
    ${ObjectRelease}
    ${PoolFree}
  }
#endif // !VULKAN_HPP_NO_SMART_HANDLE

  ${baseTypes}

  template <typename Type, Type value = Type{}>
  struct CppType
  {};
} // namespace VULKAN_HPP_NAMESPACE

#include <vulkan/${api}_enums.hpp>
#if !defined( VULKAN_HPP_NO_TO_STRING )
#include <vulkan/${api}_to_string.hpp>
#endif

#ifndef VULKAN_HPP_NO_EXCEPTIONS
namespace std
{
  template <>
  struct is_error_code_enum<VULKAN_HPP_NAMESPACE::Result> : public true_type
  {};
}  // namespace std
#endif

namespace VULKAN_HPP_NAMESPACE
{
#ifndef VULKAN_HPP_NO_EXCEPTIONS
${Exceptions}
${resultExceptions}
${throwResultException}
#endif

${ResultValue}
${resultChecks}
${constexprDefines}
} // namespace VULKAN_HPP_NAMESPACE

// clang-format off
#include <vulkan/${api}_handles.hpp>
#include <vulkan/${api}_structs.hpp>
#include <vulkan/${api}_funcs.hpp>
// clang-format on


namespace VULKAN_HPP_NAMESPACE
{
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
  ${structExtendsStructs}
#endif // VULKAN_HPP_DISABLE_ENHANCED_MODE

  namespace detail
  {
    ${DynamicLoader}
    ${DispatchLoaderDynamic}
  }
}   // namespace VULKAN_HPP_NAMESPACE
#endif
)";

  std::string str = replaceWithMap(
    vulkanHppTemplate,
    { { "api", m_api },
      { "ArrayProxy", readSnippet( "ArrayProxy.hpp" ) },
      { "ArrayProxyNoTemporaries", readSnippet( "ArrayProxyNoTemporaries.hpp" ) },
      { "ArrayWrapper1D", readSnippet( "ArrayWrapper1D.hpp" ) },
      { "ArrayWrapper2D", readSnippet( "ArrayWrapper2D.hpp" ) },
      { "baseTypes", generateBaseTypes() },
      { "constexprDefines", generateConstexprDefines() },
      { "defines", readSnippet( "defines.hpp" ) },
      { "DispatchLoaderBase", readSnippet( "DispatchLoaderBase.hpp" ) },
      { "DispatchLoaderDynamic", generateDispatchLoaderDynamic() },
      { "DispatchLoaderStatic", generateDispatchLoaderStatic() },
      { "DynamicLoader", readSnippet( "DynamicLoader.hpp" ) },
      { "Exceptions", readSnippet( "Exceptions.hpp" ) },
      { "Exchange", readSnippet( "Exchange.hpp" ) },
      { "headerVersion", m_version },
      { "includes", replaceWithMap( readSnippet( "includes.hpp" ), { { "vulkan_h", ( m_api == "vulkan" ) ? "vulkan.h" : "vulkan_sc_core.h" } } ) },
      { "licenseHeader", m_vulkanLicenseHeader },
      { "ObjectDestroy", readSnippet( "ObjectDestroy.hpp" ) },
      { "ObjectFree", readSnippet( "ObjectFree.hpp" ) },
      { "ObjectRelease", readSnippet( "ObjectRelease.hpp" ) },
      { "Optional", readSnippet( "Optional.hpp" ) },
      { "PoolFree", readSnippet( "PoolFree.hpp" ) },
      { "resultChecks", readSnippet( "resultChecks.hpp" ) },
      { "resultExceptions", generateResultExceptions() },
      { "structExtendsStructs", generateStructExtendsStructs() },
      { "ResultValue", readSnippet( "ResultValue.hpp" ) },
      { "StridedArrayProxy", readSnippet( "StridedArrayProxy.hpp" ) },
      { "StructureChain", readSnippet( "StructureChain.hpp" ) },
      { "throwResultException", generateThrowResultException() },
      { "UniqueHandle", readSnippet( "UniqueHandle.hpp" ) } } );

  writeToFile( str, vulkan_hpp );
}

void VulkanHppGenerator::generateMacrosFile() const
{
  std::string const macros_hpp = std::string( BASE_PATH ) + "/vulkan/" + m_api + "_hpp_macros.hpp";
  messager.message( "VulkanHppGenerator: Generating " + macros_hpp + " ...\n" );

  std::string const macrosTemplate = R"(${licenseHeader}

#ifndef VULKAN_HPP_MACROS_HPP
#define VULKAN_HPP_MACROS_HPP

${macros}

#endif)";

  std::string str =
    replaceWithMap( macrosTemplate,
                    { { "licenseHeader", m_vulkanLicenseHeader },
                      { "macros",
                        replaceWithMap( readSnippet( "macros.hpp" ),
                                        { { "vulkan_hpp", m_api + ".hpp" },
                                          { "vulkan_64_bit_ptr_defines", m_defines.at( "VK_USE_64_BIT_PTR_DEFINES" ).possibleDefinition } } ) } } );

  writeToFile( str, macros_hpp );
}

void VulkanHppGenerator::generateRAIIHppFile() const
{
  std::string const vulkan_raii_hpp = std::string( BASE_PATH ) + "/vulkan/" + m_api + "_raii.hpp";
  messager.message( "VulkanHppGenerator: Generating " + vulkan_raii_hpp + " ...\n" );

  std::string const vulkanHandlesHppTemplate = R"(${licenseHeader}
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
          using Type = VULKAN_HPP_EXPECTED<T, VULKAN_HPP_NAMESPACE::Result>;
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
)";

  std::string str = replaceWithMap( vulkanHandlesHppTemplate,
                                    { { "api", m_api },
                                      { "licenseHeader", m_vulkanLicenseHeader },
                                      { "RAIICommandDefinitions", generateRAIICommandDefinitions() },
                                      { "RAIIDispatchers", generateRAIIDispatchers() },
                                      { "RAIIHandles", generateRAIIHandles() } } );

  writeToFile( str, vulkan_raii_hpp );
}

void VulkanHppGenerator::generateSharedHppFile() const
{
  std::string const vulkan_shared_hpp = std::string( BASE_PATH ) + "/vulkan/" + m_api + "_shared.hpp";
  messager.message( "VulkanHppGenerator: Generating " + vulkan_shared_hpp + " ...\n" );

  std::string const vulkanHandlesHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_SHARED_HPP
#define VULKAN_SHARED_HPP

#include <vulkan/${api}.hpp>

#if !( defined( VULKAN_HPP_ENABLE_STD_MODULE ) && defined( VULKAN_HPP_STD_MODULE ) )
#include <atomic>  // std::atomic_size_t
#endif

namespace VULKAN_HPP_NAMESPACE
{
#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
  ${sharedHandle}
  namespace detail
  {
    ${sharedDestroy}
  }
  ${sharedHandles}
  ${sharedHandleSpecializations}
  ${sharedHandlesNoDestroy}
#endif // !VULKAN_HPP_NO_SMART_HANDLE
} // namespace VULKAN_HPP_NAMESPACE
#endif // VULKAN_SHARED_HPP
)";

  std::string str = replaceWithMap( vulkanHandlesHppTemplate,
                                    {
                                      { "api", m_api },
                                      { "licenseHeader", m_vulkanLicenseHeader },
                                      { "sharedDestroy", readSnippet( "SharedDestroy.hpp" ) },
                                      { "sharedHandle", readSnippet( "SharedHandle.hpp" ) },
                                      { "sharedHandles", generateSharedHandles() },
                                      { "sharedHandlesNoDestroy", generateSharedHandlesNoDestroy() },
                                      { "sharedHandleSpecializations", readSnippet( "SharedHandleSpecializations.hpp" ) },
                                    } );

  writeToFile( str, vulkan_shared_hpp );
}

void VulkanHppGenerator::generateStaticAssertionsHppFile() const
{
  std::string const static_assertions_hpp = std::string( BASE_PATH ) + "/vulkan/" + m_api + "_static_assertions.hpp";
  messager.message( "VulkanHppGenerator: Generating " + static_assertions_hpp + " ...\n" );

  std::string const vulkanHandlesHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_STATIC_ASSERTIONS_HPP
#  define VULKAN_STATIC_ASSERTIONS_HPP

#include <vulkan/${api}.hpp>

//=========================
//=== static_assertions ===
//=========================

${staticAssertions}
#endif
)";

  std::string str = replaceWithMap( vulkanHandlesHppTemplate,
                                    { { "api", m_api }, { "licenseHeader", m_vulkanLicenseHeader }, { "staticAssertions", generateStaticAssertions() } } );

  writeToFile( str, static_assertions_hpp );
}

void VulkanHppGenerator::generateStructsHppFile() const
{
  std::string const vulkan_structs_hpp = std::string( BASE_PATH ) + "/vulkan/" + m_api + "_structs.hpp";
  messager.message( "VulkanHppGenerator: Generating " + vulkan_structs_hpp + " ...\n" );

  std::string const vulkanHandlesHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_STRUCTS_HPP
#  define VULKAN_STRUCTS_HPP

// include-what-you-use: make sure, vulkan.hpp is used by code-completers
// IWYU pragma: private; include "vulkan.hpp"

#include <cstring>  // strcmp

namespace VULKAN_HPP_NAMESPACE
{
${structs}
}   // namespace VULKAN_HPP_NAMESPACE
#endif
)";

  std::string str = replaceWithMap( vulkanHandlesHppTemplate, { { "licenseHeader", m_vulkanLicenseHeader }, { "structs", generateStructs() } } );

  writeToFile( str, vulkan_structs_hpp );
}

void VulkanHppGenerator::generateToStringHppFile() const
{
  std::string const vulkan_to_string_hpp = std::string( BASE_PATH ) + "/vulkan/" + m_api + "_to_string.hpp";
  messager.message( "VulkanHppGenerator: Generating " + vulkan_to_string_hpp + " ...\n" );

  std::string const vulkanHandlesHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_TO_STRING_HPP
#  define VULKAN_TO_STRING_HPP

#include <vulkan/${api}.hpp>

// ignore warnings on using deprecated enum values in this header
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined( _MSC_VER )
#  pragma warning( push )
#  pragma warning( disable : 4996 )
#endif

#if defined( VULKAN_HPP_ENABLE_STD_MODULE ) && defined( VULKAN_HPP_STD_MODULE )
import VULKAN_HPP_STD_MODULE;
#else
#  if __cpp_lib_format
#    include <format>   // std::format
#  else
#    include <sstream>  // std::stringstream
#  endif
#endif

namespace VULKAN_HPP_NAMESPACE
{
${bitmasksToString}
${enumsToString}
} // namespace VULKAN_HPP_NAMESPACE

#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic pop
#elif defined( _MSC_VER )
#  pragma warning( pop )
#endif

#endif
)";

  std::string str = replaceWithMap( vulkanHandlesHppTemplate,
                                    { { "api", m_api },
                                      { "bitmasksToString", generateBitmasksToString() },
                                      { "enumsToString", generateEnumsToString() },
                                      { "licenseHeader", m_vulkanLicenseHeader } } );

  writeToFile( str, vulkan_to_string_hpp );
}

void VulkanHppGenerator::generateCppModuleFile() const
{
  std::string const vulkan_cppm = std::string( BASE_PATH ) + "/vulkan/" + m_api + ".cppm";
  messager.message( "VulkanHppGenerator: Generating " + vulkan_cppm + " ...\n" );

  std::string const vulkanCppmTemplate = R"(${licenseHeader}

// Note: This module is still in an experimental state.
// Any feedback is welcome on https://github.com/KhronosGroup/Vulkan-Hpp/issues.

module;

#include <vulkan/vulkan_hpp_macros.hpp>

#if defined( __cpp_lib_modules ) && !defined( VULKAN_HPP_ENABLE_STD_MODULE )
#define VULKAN_HPP_ENABLE_STD_MODULE
#endif

#include <vulkan/${api}.hpp>
#include <vulkan/${api}_extension_inspection.hpp>
#include <vulkan/${api}_format_traits.hpp>
#include <vulkan/${api}_hash.hpp>
#include <vulkan/${api}_raii.hpp>
#include <vulkan/${api}_shared.hpp>
#include <vulkan/${api}_to_string.hpp>

export module ${api}_hpp;

export namespace VULKAN_HPP_NAMESPACE
{
  ${usings}

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
  namespace VULKAN_HPP_RAII_NAMESPACE
  {
    ${raiiUsings}
  } // namespace VULKAN_HPP_RAII_NAMESPACE
#endif
} // namespace VULKAN_HPP_NAMESPACE

export namespace std
{
  ${hashSpecializations}
}

// This VkFlags type is used as part of a bitfield in some structure.
// As it that can't be mimiced by vk-data types, we need to export just that!!
export VkGeometryInstanceFlagsKHR;
)";

  auto const str = replaceWithMap( vulkanCppmTemplate,
                                   { { "api", m_api },
                                     { "hashSpecializations", generateCppModuleHashSpecializations() },
                                     { "licenseHeader", m_vulkanLicenseHeader },
                                     { "raiiUsings", generateCppModuleRaiiUsings() },
                                     { "usings", generateCppModuleUsings() } } );

  writeToFile( str, vulkan_cppm );
}

void VulkanHppGenerator::prepareRAIIHandles()
{
  // filter out functions that are not usefull on this level of abstraction (like vkGetInstanceProcAddr)
  // and all the destruction functions, as they are used differently
  assert( m_handles.begin()->first.empty() );
  for ( auto handleIt = std::next( m_handles.begin() ); handleIt != m_handles.end(); ++handleIt )
  {
    handleIt->second.destructorIt = determineRAIIHandleDestructor( handleIt->first );
    if ( handleIt->second.destructorIt != m_commands.end() )
    {
      m_RAIISpecialFunctions.insert( handleIt->second.destructorIt->first );
    }
    handleIt->second.constructorIts = determineRAIIHandleConstructors( handleIt->first, handleIt->second.destructorIt );
  }
}

//
// VulkanHppGenerator private interface
//

void VulkanHppGenerator::addCommand( std::string const & name, CommandData & commandData )
{
  // find the handle this command is going to be associated to
  checkForError( !commandData.params.empty(), commandData.xmlLine, "command <" + name + "> with no params" );
  auto handleIt = m_handles.find( commandData.params[0].type.type );
  if ( handleIt == m_handles.end() )
  {
    handleIt = m_handles.begin();
    assert( handleIt->first == "" );
  }
  commandData.handle = handleIt->first;

  // add this command to the list of commands
  checkForError( m_commands.insert( std::make_pair( name, commandData ) ).second, commandData.xmlLine, "already encountered command <" + name + ">" );
}

void VulkanHppGenerator::addCommandsToHandle( std::vector<RequireData> const & requireData )
{
  for ( auto const & require : requireData )
  {
    for ( auto const & command : require.commands )
    {
      auto commandIt = findByNameOrAlias( m_commands, command.name );
      assert( commandIt != m_commands.end() );
      auto handleIt = m_handles.find( commandIt->second.handle );
      assert( handleIt != m_handles.end() );
      if ( !handleIt->second.commands.contains( command.name ) )
      {
        handleIt->second.commands.insert( command.name );
        registerDeleter( command.name, commandIt->second );
      }
    }
  }
}

void VulkanHppGenerator::addMissingFlagBits( std::vector<RequireData> & requireData, std::string const & requiredBy )
{
  for ( auto & require : requireData )
  {
    std::vector<NameLine> newTypes;
    for ( auto const & type : require.types )
    {
      auto bitmaskIt = m_bitmasks.find( type.name );
      if ( bitmaskIt != m_bitmasks.end() )
      {
        if ( bitmaskIt->second.require.empty() )
        {
          // generate the flagBits enum name out of the bitmask name: VkFooFlagsXXX -> VkFooFlagBitsXXX
          const size_t pos = bitmaskIt->first.find( "Flags" );
          assert( pos != std::string::npos );
          std::string flagBits = bitmaskIt->first.substr( 0, pos + 4 ) + "Bit" + bitmaskIt->first.substr( pos + 4 );

          bitmaskIt->second.require = flagBits;

          // some flagsBits are specified but never listed as required for any flags!
          // so, even if this bitmask has no enum listed as required, it might still already exist in the enums list
          auto enumIt = m_enums.find( flagBits );
          if ( enumIt == m_enums.end() )
          {
            m_enums.insert( std::make_pair( flagBits, EnumData{ .isBitmask = true, .xmlLine = 0 } ) );

            assert( !m_types.contains( flagBits ) );
            m_types.insert( std::make_pair( flagBits, TypeData{ TypeCategory::Bitmask, { requiredBy }, 0 } ) );
          }
          else
          {
            assert( m_types.contains( flagBits ) );
            enumIt->second.isBitmask = true;
          }
        }
        if ( std::none_of(
               require.types.begin(), require.types.end(), [bitmaskIt]( auto const & requireType ) { return requireType.name == bitmaskIt->second.require; } ) )
        {
          // this bitmask requires a flags type that is not listed in here, so add it
          newTypes.push_back( { bitmaskIt->second.require, bitmaskIt->second.xmlLine } );
        }
      }
    }
    // add all the newly created flagBits types to the require list as if they had been part of the vk.xml!
    require.types.insert( require.types.end(), newTypes.begin(), newTypes.end() );
  }
}

std::string VulkanHppGenerator::addTitleAndProtection( std::string const & title, std::string const & strIf, std::string const & strElse ) const
{
  std::string str;
  if ( !strIf.empty() )
  {
    const auto [enter, leave] = generateProtection( getProtectFromTitle( title ) );
    str                       = "\n" + enter + "  //=== " + title + " ===\n" + strIf;
    if ( !enter.empty() && !strElse.empty() )
    {
      str += "#else \n" + strElse;
    }
    str += leave;
  }
  return str;
}

bool VulkanHppGenerator::allVectorSizesSupported( std::vector<ParamData> const & params, std::map<size_t, VectorParamData> const & vectorParams ) const
{
  // check if all vector sizes are by value and their type is one of "uint32_t", "VkDeviceSize", or "VkSampleCountFlagBits"
  return std::all_of( vectorParams.begin(),
                      vectorParams.end(),
                      [&params]( auto const & vpi )
                      {
                        return params[vpi.second.lenParam].type.isValue() &&
                               ( ( params[vpi.second.lenParam].type.type == "uint32_t" ) || ( params[vpi.second.lenParam].type.type == "VkDeviceSize" ) ||
                                 ( params[vpi.second.lenParam].type.type == "VkSampleCountFlagBits" ) );
                      } );
}

void VulkanHppGenerator::appendDispatchLoaderDynamicCommands( std::vector<RequireData> const & requireData,
                                                              std::set<std::string> &          listedCommands,
                                                              std::string const &              title,
                                                              std::string &                    commandMembers,
                                                              std::string &                    initialCommandAssignments,
                                                              std::string &                    instanceCommandAssignments,
                                                              std::string &                    deviceCommandAssignments ) const
{
  std::string members, initial, instance, device, placeholders;
  for ( auto const & require : requireData )
  {
    for ( auto const & command : require.commands )
    {
      if ( listedCommands.insert( command.name ).second )
      {
        auto commandIt = findByNameOrAlias( m_commands, command.name );
        assert( commandIt != m_commands.end() );

        members += "    PFN_" + command.name + " " + command.name + " = 0;\n";
        placeholders += "    PFN_dummy " + command.name + "_placeholder = 0;\n";
        if ( commandIt->second.handle.empty() )
        {
          initial += generateDispatchLoaderDynamicCommandAssignment( command.name, commandIt->first, "NULL" );
        }
        else
        {
          instance += generateDispatchLoaderDynamicCommandAssignment( command.name, commandIt->first, "instance" );
          if ( isDeviceCommand( commandIt->second ) )
          {
            device += generateDispatchLoaderDynamicCommandAssignment( command.name, commandIt->first, "device" );
          }
        }
      }
    }
  }
  const auto [enter, leave] = generateProtection( getProtectFromTitle( title ) );
  std::string header        = "\n" + enter + "  //=== " + title + " ===\n";
  if ( !members.empty() )
  {
    commandMembers += header + members;
    if ( !enter.empty() )
    {
      commandMembers += "#else\n" + placeholders;
    }
    commandMembers += leave;
  }
  if ( !initial.empty() )
  {
    initialCommandAssignments += header + initial + leave;
  }
  if ( !instance.empty() )
  {
    instanceCommandAssignments += header + instance + leave;
  }
  if ( !device.empty() )
  {
    deviceCommandAssignments += header + device + leave;
  }
}

void VulkanHppGenerator::appendRAIIDispatcherCommands( std::vector<RequireData> const & requireData,
                                                       std::set<std::string> &          listedCommands,
                                                       std::string const &              title,
                                                       std::string &                    contextInitializers,
                                                       std::string &                    contextMembers,
                                                       std::string &                    deviceAssignments,
                                                       std::string &                    deviceMembers,
                                                       std::string &                    instanceAssignments,
                                                       std::string &                    instanceMembers ) const
{
  std::string ci, cm, da, dm, dmp, ia, im, imp;
  for ( auto const & require : requireData )
  {
    for ( auto const & command : require.commands )
    {
      if ( listedCommands.insert( command.name ).second )
      {
        auto commandIt = findByNameOrAlias( m_commands, command.name );
        if ( commandIt->second.handle.empty() )
        {
          ci += ", " + command.name + "( PFN_" + command.name + "( getProcAddr( NULL, \"" + command.name + "\" ) ) )";

          cm += "      PFN_" + command.name + " " + command.name + " = 0;\n";
        }
        else if ( ( commandIt->second.handle == "VkDevice" ) || hasParentHandle( commandIt->second.handle, "VkDevice" ) )
        {
          da += "        " + command.name + " = PFN_" + command.name + "( vkGetDeviceProcAddr( device, \"" + command.name + "\" ) );\n";
          // if this is an alias'ed function, use it as a fallback for the original one
          if ( command.name != commandIt->first )
          {
            da += "        if ( !" + commandIt->first + " ) " + commandIt->first + " = " + command.name + ";\n";
          }

          dm += "      PFN_" + command.name + " " + command.name + " = 0;\n";
          dmp += "      PFN_dummy " + command.name + "_placeholder = 0;\n";
        }
        else
        {
          assert( ( commandIt->second.handle == "VkInstance" ) || hasParentHandle( commandIt->second.handle, "VkInstance" ) );

          // filter out vkGetInstanceProcAddr, as starting with Vulkan 1.2 it can resolve itself only (!) with an
          // instance nullptr !
          if ( command.name != "vkGetInstanceProcAddr" )
          {
            ia += "        " + command.name + " = PFN_" + command.name + "( vkGetInstanceProcAddr( instance, \"" + command.name + "\" ) );\n";
            // if this is an alias'ed function, use it as a fallback for the original one
            if ( command.name != commandIt->first )
            {
              ia += "        if ( !" + commandIt->first + " ) " + commandIt->first + " = " + command.name + ";\n";
            }
          }

          im += +"      PFN_" + command.name + " " + command.name + " = 0;\n";
          imp += "      PFN_dummy " + command.name + "_placeholder = 0;\n";
        }
      }
    }
  }
  contextInitializers += addTitleAndProtection( title, ci );
  contextMembers += addTitleAndProtection( title, cm );
  deviceAssignments += addTitleAndProtection( title, da );
  deviceMembers += addTitleAndProtection( title, dm, dmp );
  instanceAssignments += addTitleAndProtection( title, ia );
  instanceMembers += addTitleAndProtection( title, im, imp );
}

void VulkanHppGenerator::checkBitmaskCorrectness() const
{
  for ( auto const & bitmask : m_bitmasks )
  {
    // check that a bitmask is required somewhere
    // I think, it's not forbidden to not reference a bitmask, but it would probably be not intended?
    auto typeIt = m_types.find( bitmask.first );
    assert( typeIt != m_types.end() );
    checkForError( !typeIt->second.requiredBy.empty(), bitmask.second.xmlLine, "bitmask <" + bitmask.first + "> not required in any feature or extension" );

    // check that the requirement is an enum
    if ( !bitmask.second.require.empty() )
    {
      checkForError( m_enums.contains( bitmask.second.require ),
                     bitmask.second.xmlLine,
                     "bitmask <" + bitmask.first + "> requires unknown enum <" + bitmask.second.require + ">" );
    }
  }
}

void VulkanHppGenerator::checkCommandCorrectness() const
{
  // prepare command checks by gathering all result codes (including aliases and not supported ones!) into one set of resultCodes
  auto resultIt = m_enums.find( "VkResult" );
  assert( resultIt != m_enums.end() );
  std::set<std::string> resultCodes;
  for ( auto rc : resultIt->second.values )
  {
    resultCodes.insert( rc.name );
    for ( auto ac : rc.aliases )
    {
      resultCodes.insert( ac.name );
    }
  }
  // some special handling needed for vulkansc!!
  if ( m_api == "vulkansc" )
  {
    resultCodes.insert( { "VK_ERROR_FRAGMENTATION_EXT",
                          "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR",
                          "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR",
                          "VK_ERROR_NOT_PERMITTED_EXT",
                          "VK_PIPELINE_COMPILE_REQUIRED_EXT" } );
  }

  // command checks
  for ( auto const & command : m_commands )
  {
    // check that a command is referenced somewhere
    // I think, it's not forbidden to not reference a function, but it would probably be not intended?
    checkForError( !command.second.requiredBy.empty(), command.second.xmlLine, "command <" + command.first + "> not required in any feature or extension" );

    // check for unknown error or succes codes
    for ( auto const & ec : command.second.errorCodes )
    {
      checkForError( resultCodes.contains( ec ), command.second.xmlLine, "command uses unknown error code <" + ec + ">" );
    }
    for ( auto const & sc : command.second.successCodes )
    {
      checkForError( resultCodes.contains( sc ), command.second.xmlLine, "command uses unknown success code <" + sc + ">" );
    }

    // check that functions returning a VkResult specify successcodes
    if ( ( command.second.returnType == "VkResult" ) && command.second.successCodes.empty() )
    {
      // emit an error if this function is required in at least one supported feature or extension
      // disabled or not supported features/extensions are still listed in requiredBy, but not in m_features/m_extensions
      bool functionUsed = false;
      for ( auto const & require : command.second.requiredBy )
      {
        functionUsed |= isSupportedFeature( require ) || isSupportedExtension( require );
      }
      if ( functionUsed )
      {
        checkForError( false, command.second.xmlLine, "missing successcodes on command <" + command.first + "> returning VkResult!" );
      }
    }

    // check that all parameter types as well as the return type are known types
    for ( auto const & p : command.second.params )
    {
      checkForError( m_types.contains( p.type.type ), p.xmlLine, "comand uses parameter of unknown type <" + p.type.type + ">" );
    }
    checkForError(
      m_types.contains( command.second.returnType ), command.second.xmlLine, "command uses unknown return type <" + command.second.returnType + ">" );
  }
}

void VulkanHppGenerator::checkCorrectness() const
{
  checkForError( !m_vulkanLicenseHeader.empty(), -1, "missing license header" );
  checkBitmaskCorrectness();
  checkCommandCorrectness();
  checkDefineCorrectness();
  checkEnumCorrectness();
  checkExtensionCorrectness();
  checkFeatureCorrectness();
  checkFuncPointerCorrectness();
  checkHandleCorrectness();
  checkSpirVCapabilityCorrectness();
  checkStructCorrectness();
  checkSyncAccessCorrectness();
  checkSyncStageCorrectness();
}

void VulkanHppGenerator::checkDefineCorrectness() const
{
  // check that any requirements of a define is known
  for ( auto const & d : m_defines )
  {
    checkForError( d.second.require.empty() || m_types.contains( d.second.require ),
                   d.second.xmlLine,
                   "define <" + d.first + "> uses unknown require <" + d.second.require + ">" );
  }
}

void VulkanHppGenerator::checkEnumCorrectness() const
{
  for ( auto const & e : m_enums )
  {
    // check that a bitmask is required somewhere
    // some bitmasks are never required, so make this a warning only
    auto typeIt = m_types.find( e.first );
    assert( typeIt != m_types.end() );
    checkForWarning( !typeIt->second.requiredBy.empty(), e.second.xmlLine, "enum <" + e.first + "> not required in any feature or extension" );
  }

  // enum checks by features and extensions
  for ( auto & feature : m_features )
  {
    checkEnumCorrectness( feature.requireData );
  }
  for ( auto & ext : m_extensions )
  {
    checkEnumCorrectness( ext.requireData );
  }

  // special check for VkFormat
  if ( !m_formats.empty() )
  {
    auto enumIt = m_enums.find( "VkFormat" );
    assert( enumIt != m_enums.end() );
    assert( enumIt->second.values.front().name == "VK_FORMAT_UNDEFINED" );
    for ( auto enumValueIt = std::next( enumIt->second.values.begin() ); enumValueIt != enumIt->second.values.end(); ++enumValueIt )
    {
      checkForError( !enumValueIt->supported || m_formats.contains( enumValueIt->name ),
                     enumValueIt->xmlLine,
                     "missing format specification for <" + enumValueIt->name + ">" );
    }
  }
}

void VulkanHppGenerator::checkEnumCorrectness( std::vector<RequireData> const & requireData ) const
{
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto typeIt = m_types.find( type.name );
      assert( typeIt != m_types.end() );
      switch ( typeIt->second.category )
      {
        case TypeCategory::Bitmask:
          {
            // check that each "require" listed for a bitmask is listed for a feature or an extension
            auto bitmaskIt = m_bitmasks.find( type.name );
            if ( bitmaskIt != m_bitmasks.end() )
            {
              // not for every bitmask is a "require" listed
              if ( !bitmaskIt->second.require.empty() )
              {
                auto requireTypeIt = m_types.find( bitmaskIt->second.require );
                assert( requireTypeIt != m_types.end() );
                checkForError( !requireTypeIt->second.requiredBy.empty(),
                               bitmaskIt->second.xmlLine,
                               "bitmask <" + bitmaskIt->first + "> requires <" + bitmaskIt->second.require +
                                 "> which is not required by any feature or extension!" );
              }
            }
          }
          break;
        case TypeCategory::Enum:
          {
            auto enumIt = m_enums.find( type.name );
            if ( enumIt != m_enums.end() )
            {
              if ( enumIt->second.isBitmask && !enumIt->second.values.empty() )
              {
                // check that any non-empty enum of a bitmask is listed as "require" or "bitvalues" for a bitmask
                auto bitmaskIt =
                  std::find_if( m_bitmasks.begin(), m_bitmasks.end(), [&enumIt]( auto const & bitmask ) { return bitmask.second.require == enumIt->first; } );
                checkForError( bitmaskIt != m_bitmasks.end(),
                               enumIt->second.xmlLine,
                               "enum <" + enumIt->first + "> is not listed as an requires or bitvalues for any bitmask in the types section" );

                // check that bitwidth of the enum and type of the corresponding bitmask are equal
                checkForError( ( enumIt->second.bitwidth != "64" ) || ( bitmaskIt->second.type == "VkFlags64" ),
                               enumIt->second.xmlLine,
                               "enum <" + enumIt->first + "> is marked with bitwidth <64> but corresponding bitmask <" + bitmaskIt->first +
                                 "> is not of type <VkFlags64>" );
              }
            }
            else
            {
              // every enum not listed in the m_enums, should be an alias of such a thing
              checkForError(
                findByNameOrAlias( m_enums, type.name ) != m_enums.end(), typeIt->second.xmlLine, "enum type <" + type.name + "> is not listed as an enum" );
            }
          }
          break;
        default: break;
      }
    }
  }
}

bool VulkanHppGenerator::checkEquivalentSingularConstructor( std::vector<std::map<std::string, CommandData>::const_iterator> const & constructorIts,
                                                             std::map<std::string, CommandData>::const_iterator                      constructorIt,
                                                             std::vector<ParamData>::const_iterator                                  lenIt ) const
{
  // check, if there is no singular constructor with the very same arguments as this array constructor
  // (besides the size, of course)
  auto isEquivalentSingularConstructor = [constructorIt, lenIt]( std::map<std::string, CommandData>::const_iterator it )
  {
    if ( it->second.params.size() + 1 != constructorIt->second.params.size() )
    {
      return false;
    }
    const size_t lenIdx = std::distance( constructorIt->second.params.begin(), lenIt );
    for ( size_t i = 0, j = 0; i < it->second.params.size(); ++i, ++j )
    {
      assert( j < constructorIt->second.params.size() );
      if ( j == lenIdx )
      {
        ++j;
      }
      if ( it->second.params[i].type.type != constructorIt->second.params[j].type.type )
      {
        return false;
      }
    }
    return true;
  };
  return ( std::any_of( constructorIts.begin(), constructorIts.end(), isEquivalentSingularConstructor ) );
}

void VulkanHppGenerator::checkExtensionCorrectness() const
{
  for ( auto const & extension : m_extensions )
  {
    // check for existence of any depends, deprecation, obsoletion, or promotion
    for ( auto const & dependsByVersion : extension.depends )
    {
      checkForError( isFeature( dependsByVersion.first ),
                     extension.xmlLine,
                     "extension <" + extension.name + "> lists an unknown feature <" + dependsByVersion.first + ">" );
      for ( auto const & dependsSet : dependsByVersion.second )
      {
        for ( auto const & depends : dependsSet )
        {
          checkForError( isExtension( depends ), extension.xmlLine, "extension <" + extension.name + "> lists an unknown depends <" + depends + ">" );
        }
      }
    }
    if ( !extension.deprecatedBy.empty() )
    {
      checkForError( isFeature( extension.deprecatedBy ) || isExtension( extension.deprecatedBy ),
                     extension.xmlLine,
                     "extension <" + extension.name + "> is deprecated by unknown extension/version <" + extension.deprecatedBy + ">" );
    }
    if ( !extension.obsoletedBy.empty() )
    {
      checkForError( isFeature( extension.obsoletedBy ) || isExtension( extension.obsoletedBy ),
                     extension.xmlLine,
                     "extension <" + extension.name + "> is obsoleted by unknown extension/version <" + extension.obsoletedBy + ">" );
    }
    if ( !extension.promotedTo.empty() )
    {
      checkForError( isFeature( extension.promotedTo ) || isExtension( extension.promotedTo ),
                     extension.xmlLine,
                     "extension <" + extension.name + "> is promoted to unknown extension/version <" + extension.promotedTo + ">" );
    }

    // check for existence of any requirement
    for ( auto const & require : extension.requireData )
    {
      if ( !require.depends.empty() )
      {
        std::vector<std::string> depends = tokenizeAny( require.depends, ",+()" );
        for ( auto const & depend : depends )
        {
          checkForError(
            isFeature( depend ) || isExtension( depend ), require.xmlLine, "extension <" + extension.name + "> lists an unknown depends <" + depend + ">" );
        }
      }
    }
  }
}

void VulkanHppGenerator::checkFeatureCorrectness() const
{
  // check that each require depends actually is an extension
  // remove this check temporarily !
  // for ( auto const & feature : m_features )
  //{
  //  for ( auto const & require : feature.requireData )
  //  {
  //    checkForError(
  //      require.depends.empty() ||
  //        std::any_of( m_extensions.begin(), m_extensions.end(), [&depends = require.depends]( ExtensionData const & ed ) { return ed.name == depends; } ),
  //      require.xmlLine,
  //      "feature <" + feature.name + "> depends on an unknown extension <" + require.depends + ">" );
  //  }
  //}
}

void VulkanHppGenerator::checkFuncPointerCorrectness() const
{
  for ( auto const & funcPointer : m_funcPointers )
  {
    if ( !funcPointer.second.require.empty() )
    {
      checkForError(
        m_types.contains( funcPointer.second.require ), funcPointer.second.xmlLine, "funcpointer requires unknown <" + funcPointer.second.require + ">" );
    }
    for ( auto const & argument : funcPointer.second.arguments )
    {
      checkForError( m_types.contains( argument.type.type ), argument.xmlLine, "funcpointer argument of unknown type <" + argument.type.type + ">" );
    }
  }
}

void VulkanHppGenerator::checkHandleCorrectness() const
{
  // prepare handle checks by getting the VkObjectType enum
  auto objectTypeIt = m_enums.find( "VkObjectType" );
  assert( objectTypeIt != m_enums.end() );

  // handle checks
  for ( auto const & handle : m_handles )
  {
    // check the existence of the parent
    checkForError(
      m_handles.contains( handle.second.parent ), handle.second.xmlLine, "handle <" + handle.first + "> with unknown parent <" + handle.second.parent + ">" );

    // check existence of objTypeEnum used with this handle type
    if ( !handle.first.empty() )
    {
      assert( !handle.second.objTypeEnum.empty() );

      // only check with used handles!
      checkForError( !isTypeUsed( handle.first ) || contains( objectTypeIt->second.values, handle.second.objTypeEnum ),
                     handle.second.xmlLine,
                     "handle <" + handle.first + "> specifies unknown \"objtypeenum\" <" + handle.second.objTypeEnum + ">" );
    }
  }

  // check that all specified objectType values are used with a handle type
  for ( auto const & objectTypeValue : objectTypeIt->second.values )
  {
    if ( objectTypeValue.name != "VK_OBJECT_TYPE_UNKNOWN" )
    {
      checkForError( std::any_of( m_handles.begin(),
                                  m_handles.end(),
                                  [&objectTypeValue]( std::pair<std::string, HandleData> const & hd )
                                  { return hd.second.objTypeEnum == objectTypeValue.name; } ),
                     objectTypeValue.xmlLine,
                     "VkObjectType value <" + objectTypeValue.name + "> not specified as \"objtypeenum\" for any handle" );
    }
  }
}

void VulkanHppGenerator::checkSpirVCapabilityCorrectness() const
{
  for ( auto const & capability : m_spirVCapabilities )
  {
    for ( auto const & enable : capability.second.structs )
    {
      assert( !enable.second.empty() );
      auto structIt = findByNameOrAlias( m_structs, enable.first );
      checkForError( structIt != m_structs.end(),
                     enable.second.begin()->second,
                     "unknown structure <" + enable.first + "> specified for SPIR-V capability <" + capability.first + ">" );

      for ( auto const & member : enable.second )
      {
        auto memberIt =
          std::find_if( structIt->second.members.begin(), structIt->second.members.end(), [&member]( auto const & md ) { return md.name == member.first; } );
        checkForError( memberIt != structIt->second.members.end(),
                       member.second,
                       "unknown member <" + member.first + "> in struct <" + enable.first + "> specified for SPIR-V capability <" + capability.first + ">" );
      }
    }
  }
}

void VulkanHppGenerator::checkStructCorrectness() const
{
  std::set<std::string> sTypeValues;
  for ( auto const & structure : m_structs )
  {
    // check that a struct is referenced somewhere
    // I think, it's not forbidden to not reference a struct, but it would probably be not intended?
    auto typeIt = m_types.find( structure.first );
    assert( typeIt != m_types.end() );
    checkForError(
      !typeIt->second.requiredBy.empty(), structure.second.xmlLine, "structure <" + structure.first + "> not required by any feature or extension" );

    // check for existence of all structs that are extended by this struct
    for ( auto const & extend : structure.second.structExtends )
    {
      checkForError( findByNameOrAlias( m_structs, extend ) != m_structs.end(),
                     structure.second.xmlLine,
                     "struct <" + structure.first + "> extends unknown <" + extend + ">" );
    }

    // checks on the members of a struct
    checkStructMemberCorrectness( structure.first, structure.second.members, sTypeValues );
  }

  // enum VkStructureType checks (need to be after structure checks because of sTypeValues gathered there)
  auto structureTypeIt = m_enums.find( "VkStructureType" );
  assert( structureTypeIt != m_enums.end() );
  static std::set<std::string> reservedValues = { "VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO",
                                                  "VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO",
                                                  "VK_STRUCTURE_TYPE_PRIVATE_VENDOR_INFO_PLACEHOLDER_OFFSET_0_NV" };
  for ( auto const & enumValue : structureTypeIt->second.values )
  {
    if ( reservedValues.contains( enumValue.name ) )
    {
      checkForError( !sTypeValues.contains( enumValue.name ), enumValue.xmlLine, "Reserved VkStructureType enum value <" + enumValue.name + "> is used" );
    }
    else
    {
      checkForWarning( !enumValue.supported || ( sTypeValues.erase( enumValue.name ) == 1 ),
                       enumValue.xmlLine,
                       "VkStructureType enum value <" + enumValue.name + "> never used" );
    }
  }
  assert( sTypeValues.empty() );
}

void VulkanHppGenerator::checkStructMemberCorrectness( std::string const &             structureName,
                                                       std::vector<MemberData> const & members,
                                                       std::set<std::string> &         sTypeValues ) const
{
  // determine if this struct is requird/used
  const bool structUsed = isTypeUsed( structureName );

  for ( auto const & member : members )
  {
    // check that all member types are required in some feature or extension
    if ( member.type.type.starts_with( "Vk" ) )
    {
      auto memberTypeIt = m_types.find( member.type.type );
      assert( memberTypeIt != m_types.end() );
      checkForError( !memberTypeIt->second.requiredBy.empty(),
                     member.xmlLine,
                     "struct member type <" + member.type.type + "> used in struct <" + structureName + "> is never required for any feature or extension" );
    }

    // if a member specifies a selector, that member is a union and the selector is an enum
    // check that there's a 1-1 connection between the specified selections and the values of that enum
    if ( !member.selector.empty() )
    {
      auto selectorIt = findStructMemberIt( member.selector, members );
      assert( selectorIt != members.end() );
      auto selectorEnumIt = m_enums.find( selectorIt->type.type );
      assert( selectorEnumIt != m_enums.end() );
      auto unionIt = m_structs.find( member.type.type );
      assert( ( unionIt != m_structs.end() ) && unionIt->second.isUnion );
      for ( auto const & unionMember : unionIt->second.members )
      {
        // check that each union member has a selection, that is a value of the seleting enum
        assert( !unionMember.selection.empty() );
        for ( auto const & selection : unionMember.selection )
        {
          checkForError( contains( selectorEnumIt->second.values, selection ),
                         unionMember.xmlLine,
                         "union member <" + unionMember.name + "> uses selection <" + selection + "> that is not part of the selector type <" +
                           selectorIt->type.type + ">" );
        }
      }
    }

    // check that each member type is known
    checkForError( m_types.contains( member.type.type ), member.xmlLine, "struct member uses unknown type <" + member.type.type + ">" );

    // check that any used constant is a known constant or some potentially externally defined constant
    for ( auto const & arraySize : member.arraySizes )
    {
      if ( !isNumber( arraySize ) && !m_constants.contains( arraySize ) )
      {
        auto typeIt = m_types.find( arraySize );
        checkForError( ( typeIt != m_types.end() ) && isAllUpper( arraySize ) && ( typeIt->second.category == TypeCategory::ExternalType ),
                       member.xmlLine,
                       "struct member array size uses unknown constant <" + arraySize + ">" );
      }
    }

    // checks if a value is specified
    if ( !member.value.empty() )
    {
      auto enumIt = m_enums.find( member.type.type );
      if ( enumIt != m_enums.end() )
      {
        // check that the value exists in the specified enum (if the struct is used at all)
        if ( structUsed )
        {
          checkForError( contains( enumIt->second.values, member.value ),
                         member.xmlLine,
                         "value <" + member.value + "> for member <" + member.name + "> in structure <" + structureName + "> of enum type <" +
                           member.type.type + "> not listed" );
          // special handling for sType: no value should appear more than once
          if ( member.name == "sType" )
          {
            checkForError( sTypeValues.insert( member.value ).second, member.xmlLine, "sType value <" + member.value + "> has been used before" );
          }
        }
      }
      else if ( member.type.type == "uint32_t" )
      {
        checkForError( isNumber( member.value ),
                       member.xmlLine,
                       "value <" + member.value + "> for member <" + member.name + "> in structure <" + structureName + "> of type <" + member.type.type +
                         "> is not a number" );
      }
      else
      {
        // don't know the type of the value -> error out
        checkForError( false,
                       member.xmlLine,
                       "member <" + member.name + "> in structure <" + structureName + "> holds value <" + member.value + "> for an unhandled type <" +
                         member.type.type + ">" );
      }
    }
  }
}

void VulkanHppGenerator::checkSyncAccessCorrectness() const
{
  auto accessFlagBitsIt = m_enums.find( "VkAccessFlagBits" );
  assert( accessFlagBitsIt != m_enums.end() );
  auto accessFlagBits2It = m_enums.find( "VkAccessFlagBits2" );
  assert( accessFlagBits2It != m_enums.end() );

  for ( auto const & syncAccess : m_syncAccesses )
  {
    auto nameIt = findByNameOrAlias( accessFlagBits2It->second.values, syncAccess.first );
    checkForError( nameIt != accessFlagBits2It->second.values.end(),
                   syncAccess.second.xmlLine,
                   "syncaccess name <" + syncAccess.first + "> not specified as a VkAccessFlagBits value!" );

    if ( !syncAccess.second.name.empty() )
    {
      // with alias
      auto aliasIt = findByNameOrAlias( accessFlagBitsIt->second.values, syncAccess.second.name );
      checkForError( aliasIt != accessFlagBitsIt->second.values.end(),
                     syncAccess.second.xmlLine,
                     "syncaccess alias <" + syncAccess.second.name + "> not specified as a VkAccessFlagBits value!" );
      checkForError( ( aliasIt->value == nameIt->value ) && ( aliasIt->bitpos == nameIt->bitpos ),
                     syncAccess.second.xmlLine,
                     "syncaccess name <" + syncAccess.first + "> has an alias <" + syncAccess.second.name + "> with a different value or bitpos!" );
    }
  }
}

void VulkanHppGenerator::checkSyncStageCorrectness() const
{
  auto stageFlagBitsIt = m_enums.find( "VkPipelineStageFlagBits" );
  assert( stageFlagBitsIt != m_enums.end() );
  auto stageFlagBits2It = m_enums.find( "VkPipelineStageFlagBits2" );
  assert( stageFlagBits2It != m_enums.end() );

  for ( auto const & syncStage : m_syncStages )
  {
    auto nameIt = findByNameOrAlias( stageFlagBits2It->second.values, syncStage.first );
    checkForError( nameIt != stageFlagBits2It->second.values.end(),
                   syncStage.second.xmlLine,
                   "syncstage name <" + syncStage.first + "> not specified as a VkPipelineStageFlagBits2 value!" );

    if ( !syncStage.second.name.empty() )
    {
      // with alias
      auto aliasIt = findByNameOrAlias( stageFlagBitsIt->second.values, syncStage.second.name );
      checkForError( aliasIt != stageFlagBitsIt->second.values.end(),
                     syncStage.second.xmlLine,
                     "syncstage alias <" + syncStage.second.name + "> not specified as a VkPipelineStageFlagBits value!" );
      checkForError( ( aliasIt->value == nameIt->value ) && ( aliasIt->bitpos == nameIt->bitpos ),
                     syncStage.second.xmlLine,
                     "syncstate name <" + syncStage.first + "> has an alias <" + syncStage.second.name + "> with a different value or bitpos!" );
    }
  }
}

std::string VulkanHppGenerator::combineDataTypes( std::map<size_t, VectorParamData> const & vectorParams,
                                                  std::vector<size_t> const &               returnParams,
                                                  bool                                      enumerating,
                                                  std::vector<std::string> const &          dataTypes,
                                                  CommandFlavourFlags                       flavourFlags,
                                                  bool                                      raii ) const
{
  assert( dataTypes.size() == returnParams.size() );

  std::vector<std::string> modifiedDataTypes( dataTypes.size() );
  for ( size_t i = 0; i < returnParams.size(); ++i )
  {
    auto vectorParamIt   = vectorParams.find( returnParams[i] );
    modifiedDataTypes[i] = ( vectorParamIt == vectorParams.end() || ( flavourFlags & CommandFlavourFlagBits::singular ) )
                           ? dataTypes[i]
                           : ( "std::vector<" + dataTypes[i] +
                               ( raii || ( flavourFlags & CommandFlavourFlagBits::unique )
                                   ? ">"
                                   : ( ", " + startUpperCase( stripPrefix( dataTypes[i], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator>" ) ) );
  }

  std::string combinedType;
  switch ( modifiedDataTypes.size() )
  {
    case 0: combinedType = "void"; break;
    case 1: combinedType = modifiedDataTypes[0]; break;
    case 2:
      assert( !enumerating || ( vectorParams.contains( returnParams[1] ) && ( vectorParams.find( returnParams[1] )->second.lenParam == returnParams[0] ) ) );
      combinedType = enumerating ? modifiedDataTypes[1] : ( "std::pair<" + modifiedDataTypes[0] + ", " + modifiedDataTypes[1] + ">" );
      break;
    case 3:
      assert( enumerating );
      switch ( vectorParams.size() )
      {
        case 1:
          assert( ( vectorParams.begin()->first == returnParams[2] ) && ( vectorParams.begin()->second.lenParam == returnParams[1] ) );
          combinedType = "std::pair<" + modifiedDataTypes[0] + ", " + modifiedDataTypes[2] + ">";
          break;
        case 2:
          assert( ( vectorParams.begin()->first == returnParams[1] ) && ( vectorParams.begin()->second.lenParam == returnParams[0] ) &&
                  ( std::next( vectorParams.begin() )->first == returnParams[2] ) &&
                  ( std::next( vectorParams.begin() )->second.lenParam == returnParams[0] ) );
          combinedType = "std::pair<" + modifiedDataTypes[1] + ", " + modifiedDataTypes[2] + ">";
          break;
        default: assert( false ); break;
      }
      break;
    default: assert( false ); break;
  }
  return combinedType;
}

bool VulkanHppGenerator::contains( std::vector<EnumValueData> const & enumValues, std::string const & name ) const
{
  return std::any_of( enumValues.begin(),
                      enumValues.end(),
                      [&name]( EnumValueData const & ev ) {
                        return ( ev.name == name ) ||
                               std::any_of( ev.aliases.begin(), ev.aliases.end(), [&name]( auto const & eav ) { return eav.name == name; } );
                      } );
}

bool VulkanHppGenerator::containsArray( std::string const & type ) const
{
  // a simple recursive check if a type is or contains an array
  auto structureIt = m_structs.find( type );
  bool found       = false;
  if ( structureIt != m_structs.end() )
  {
    for ( auto memberIt = structureIt->second.members.begin(); memberIt != structureIt->second.members.end() && !found; ++memberIt )
    {
      found = !memberIt->arraySizes.empty() || containsArray( memberIt->type.type );
    }
  }
  return found;
}

bool VulkanHppGenerator::containsFuncPointer( std::string const & type ) const
{
  // a simple recursive check if a type contains a funcpointer
  auto structureIt = m_structs.find( type );
  bool found       = false;
  if ( structureIt != m_structs.end() )
  {
    for ( auto memberIt = structureIt->second.members.begin(); memberIt != structureIt->second.members.end() && !found; ++memberIt )
    {
      found = m_funcPointers.contains( memberIt->type.type ) || ( ( memberIt->type.type != type ) && containsFuncPointer( memberIt->type.type ) );
    }
  }
  return found;
}

bool VulkanHppGenerator::containsFloatingPoints( std::vector<MemberData> const & members ) const
{
  for ( auto const & m : members )
  {
    if ( m.type.isValue() )
    {
      if ( ( m.type.type == "float" ) || ( m.type.type == "double" ) )
      {
        return true;
      }
      else
      {
        auto structureIt = m_structs.find( m.type.type );
        if ( structureIt != m_structs.end() && containsFloatingPoints( structureIt->second.members ) )
        {
          return true;
        }
      }
    }
  }
  return false;
}

bool VulkanHppGenerator::containsUnion( std::string const & type ) const
{
  // a simple recursive check if a type is or contains a union
  auto structureIt = m_structs.find( type );
  bool found       = false;
  if ( structureIt != m_structs.end() )
  {
    found = structureIt->second.isUnion;
    for ( auto memberIt = structureIt->second.members.begin(); memberIt != structureIt->second.members.end() && !found; ++memberIt )
    {
      found = memberIt->type.isValue() && containsUnion( memberIt->type.type );
    }
  }
  return found;
}

bool VulkanHppGenerator::describesVector( StructureData const & structure, std::string const & type ) const
{
  for ( auto const & member : structure.members )
  {
    if ( ( type.empty() ? true : ( member.type.type == type ) ) && member.type.isNonConstPointer() && ( member.lenMembers.size() == 1 ) )
    {
      assert( member.lenMembers[0].second < structure.members.size() );
      auto const & lenMember = structure.members[member.lenMembers[0].second];
      if ( lenMember.type.isValue() && ( ( lenMember.type.type == "size_t" ) || ( lenMember.type.type == "uint32_t" ) ) )
      {
        return true;
      }
    }
  }
  return false;
}

std::vector<size_t> VulkanHppGenerator::determineChainedReturnParams( std::vector<ParamData> const & params, std::vector<size_t> const & returnParams ) const
{
  std::vector<size_t> chainedParams;

  for ( auto rp : returnParams )
  {
    if ( isStructureChainAnchor( params[rp].type.type ) )
    {
      chainedParams.push_back( rp );
    }
  }
  return chainedParams;
}

std::vector<size_t> VulkanHppGenerator::determineConstPointerParams( std::vector<ParamData> const & params ) const
{
  std::vector<size_t> constPointerParams;

  for ( size_t i = 0; i < params.size(); i++ )
  {
    // very special handling for some types, which come in as non-const pointers, but are meant as const-pointers
    if ( params[i].type.isConstPointer() || ( params[i].type.isNonConstPointer() && specialPointerTypes.contains( params[i].type.type ) ) )
    {
      constPointerParams.push_back( i );
    }
  }
  return constPointerParams;
}

std::vector<std::string> VulkanHppGenerator::determineDataTypes( std::vector<VulkanHppGenerator::ParamData> const & params,
                                                                 std::map<size_t, VectorParamData> const &          vectorParams,
                                                                 std::vector<size_t> const &                        returnParams,
                                                                 std::set<size_t> const &                           templatedParams ) const
{
  std::vector<std::string> dataTypes;
  for ( auto rp : returnParams )
  {
    auto vectorParamIt = vectorParams.find( rp );
    if ( templatedParams.contains( rp ) )
    {
      assert( ( vectorParamIt == vectorParams.end() ) || !vectorParamIt->second.byStructure );
      if ( ( vectorParamIt != vectorParams.end() ) &&
           std::any_of( returnParams.begin(), returnParams.end(), [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->first; } ) &&
           std::any_of( returnParams.begin(), returnParams.end(), [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->second.lenParam; } ) )
      {
        dataTypes.push_back( "uint8_t" );
      }
      else
      {
        dataTypes.push_back( ( stripPrefix( params[rp].name, "p" ) + "Type" ) );
      }
    }
    else if ( ( vectorParamIt != vectorParams.end() ) && vectorParamIt->second.byStructure )
    {
      dataTypes.push_back( trimEnd( stripPostfix( vectorMemberByStructure( params[rp].type.type ).type.compose( "VULKAN_HPP_NAMESPACE" ), "*" ) ) );
    }
    else
    {
      dataTypes.push_back( trimEnd( stripPostfix( params[rp].type.compose( "VULKAN_HPP_NAMESPACE" ), "*" ) ) );
    }
  }
  return dataTypes;
}

size_t VulkanHppGenerator::determineDefaultStartIndex( std::vector<ParamData> const & params, std::set<size_t> const & skippedParams ) const
{
  // determine the index where the arguments start to have defaults
  size_t defaultStartIndex = INVALID_INDEX;
  for ( int i = static_cast<int>( params.size() ) - 1; ( 0 <= i ) && ( params[i].optional || skippedParams.contains( i ) ); --i )
  {
    defaultStartIndex = i;
  }
  return defaultStartIndex;
}

bool VulkanHppGenerator::determineEnumeration( std::map<size_t, VectorParamData> const & vectorParams, std::vector<size_t> const & returnParams ) const
{
  // a command is considered to be enumerating some data, if for at least one vectorParam the data is a returnParam and either the vectorParams is specified
  // by a structure or the lenParam is a returnParam as well
  return std::any_of( vectorParams.begin(),
                      vectorParams.end(),
                      [&returnParams]( auto const & vp )
                      {
                        return std::any_of( returnParams.begin(), returnParams.end(), [&vp]( size_t rp ) { return rp == vp.first; } ) &&
                               ( vp.second.byStructure ||
                                 std::any_of( returnParams.begin(), returnParams.end(), [&vp]( size_t rp ) { return rp == vp.second.lenParam; } ) );
                      } );
}

size_t VulkanHppGenerator::determineInitialSkipCount( std::string const & command ) const
{
  // determine the number of arguments to skip for a function
  // -> 0: the command is not bound to an instance or a device (the corresponding handle has no name)
  // -> 1: the command bound to an instance or a device (the corresponding handle has a name)
  // -> 2: the command has been moved to a second handle
  auto commandIt = findByNameOrAlias( m_commands, command );
  assert( commandIt != m_commands.end() );
  auto handleIt = m_handles.find( commandIt->second.handle );
  assert( handleIt != m_handles.end() );
  if ( !handleIt->second.commands.contains( command ) )
  {
    assert( 1 < commandIt->second.params.size() );
    assert( m_handles.contains( commandIt->second.params[1].type.type ) );
    return 2;
  }
  else
  {
    return handleIt->first.empty() ? 0 : 1;
  }
}

std::vector<size_t> VulkanHppGenerator::determineReturnParams( std::vector<ParamData> const & params ) const
{
  std::vector<size_t> nonConstPointerParamIndices;

  for ( size_t i = 0; i < params.size(); i++ )
  {
    // very special handling of parameters of some types, which always come as a non-const pointer but are not meant
    // to be a potential return value!
    if ( params[i].type.isNonConstPointer() && !specialPointerTypes.contains( params[i].type.type ) )
    {
      nonConstPointerParamIndices.push_back( i );
    }
  }
  return nonConstPointerParamIndices;
}

bool VulkanHppGenerator::isConstructorCandidate( std::pair<std::string, VulkanHppGenerator::CommandData> const & command, std::string const & handleType ) const
{
  return isSupported( command.second.requiredBy ) &&
         std::any_of( command.second.params.begin(),
                      command.second.params.end(),
                      [this, &handleType]( ParamData const & pd ) { return isConstructorCandidate( pd, handleType ); } );
}

bool VulkanHppGenerator::isConstructorCandidate( ParamData const & paramData, std::string const & handleType ) const
{
  if ( paramData.type.isNonConstPointer() )
  {
    if ( paramData.type.type == handleType )
    {
      return true;
    }
    else
    {
      auto structIt = m_structs.find( paramData.type.type );
      if ( structIt != m_structs.end() )
      {
        return describesVector( structIt->second, handleType );
      }
    }
  }
  return false;
}

std::vector<std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator>
  VulkanHppGenerator::determineRAIIHandleConstructors( std::string const & handleType, std::map<std::string, CommandData>::const_iterator destructorIt ) const
{
  std::vector<std::map<std::string, CommandData>::const_iterator> constructorIts;
  for ( auto commandIt = m_commands.begin(); commandIt != m_commands.end(); ++commandIt )
  {
    if ( isConstructorCandidate( *commandIt, handleType ) )
    {
      // only commands that provide all information needed for the destructor can be considered a constructor!
      bool valid = true;
      if ( destructorIt != m_commands.end() )
      {
        // get the destructors parameter to the handleType
        auto desctructorHandleParamIt = std::find_if( destructorIt->second.params.begin(),
                                                      destructorIt->second.params.end(),
                                                      [&handleType]( ParamData const & pd ) { return pd.type.type == handleType; } );
        assert( desctructorHandleParamIt != destructorIt->second.params.end() );

        // lambda to check if a destructor parameter is a parameter of the constructor candidate
        // (or it's just the len parameter, which is not needed for the constructor)
        auto isConstructorCandidateParam = [&desctructorHandleParamIt, &commandIt, this]( ParamData const & destructorParam )
        {
          // check if the destructor param type equals this param type, or, if this param type is a struct, is part of
          // that struct
          auto isDestructorParamType = [&destructorParam, this]( ParamData const & pd )
          {
            if ( pd.type.type != destructorParam.type.type )
            {
              // check if the destructor param type equals a structure member type
              auto structureIt = m_structs.find( pd.type.type );
              return ( structureIt != m_structs.end() ) &&
                     ( findStructMemberItByType( destructorParam.type.type, structureIt->second.members ) != structureIt->second.members.end() );
            }
            return true;
          };

          return ( destructorParam.name == desctructorHandleParamIt->lenExpression ) ||
                 std::any_of( commandIt->second.params.begin(), commandIt->second.params.end(), isDestructorParamType );
        };

        // the constructor candidate is valid, if none of the (relevant) destructor parameters is missing in the
        // constructor candidate params
        valid = std::all_of( destructorIt->second.params.begin(), destructorIt->second.params.end(), isConstructorCandidateParam );
      }
      if ( valid )
      {
        constructorIts.push_back( commandIt );
      }
    }
  }
  return constructorIts;
}

std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator VulkanHppGenerator::determineRAIIHandleDestructor( std::string const & handleType ) const
{
  std::string type         = stripPrefix( handleType, "Vk" );
  auto        destructorIt = m_commands.find( "vkDestroy" + type );
  if ( destructorIt == m_commands.end() )
  {
    destructorIt = m_commands.find( "vkFree" + type + "s" );
    if ( destructorIt == m_commands.end() )
    {
      destructorIt = m_commands.find( "vkRelease" + type );
      if ( destructorIt == m_commands.end() )
      {
        if ( handleType == "VkDeviceMemory" )
        {
          // special handling for vkDeviceMemory
          destructorIt = m_commands.find( "vkFreeMemory" );
          assert( destructorIt != m_commands.end() );
        }
        else if ( handleType == "VkDisplayKHR" )
        {
          // special handling for VkDisplayKHR
          destructorIt = m_commands.find( "vkReleaseDisplayEXT" );
          assert( destructorIt != m_commands.end() );
        }
        else
        {
          assert( ( handleType == "VkDisplayModeKHR" ) || ( handleType == "VkPhysicalDevice" ) || ( handleType == "VkQueue" ) );
        }
      }
    }
  }
  return destructorIt;
}

std::set<size_t> VulkanHppGenerator::determineSingularParams( size_t returnParam, std::map<size_t, VectorParamData> const & vectorParams ) const
{
  auto returnVectorIt = vectorParams.find( returnParam );
  assert( returnVectorIt != vectorParams.end() );
  std::set<size_t> singularParams;
  singularParams.insert( returnVectorIt->second.lenParam );
  for ( auto const & vpi : vectorParams )
  {
    if ( vpi.second.lenParam == returnVectorIt->second.lenParam )
    {
      singularParams.insert( vpi.first );
    }
  }
  return singularParams;
}

std::set<size_t> VulkanHppGenerator::determineSkippedParams( std::vector<ParamData> const &            params,
                                                             size_t                                    initialSkipCount,
                                                             std::map<size_t, VectorParamData> const & vectorParams,
                                                             std::vector<size_t> const &               returnParams,
                                                             bool                                      singular ) const
{
  // skip the initial skips (get fed by the object)
  assert( initialSkipCount <= params.size() );
  std::set<size_t> skippedParams;
  for ( size_t i = 0; i < initialSkipCount; ++i )
  {
    skippedParams.insert( i );
  }

  // skip the size parameters (get derived from an array), and a stride parameter
  for ( auto const & vpi : vectorParams )
  {
    if ( !vpi.second.byStructure )
    {
      assert( !params[vpi.first].lenExpression.empty() );
      if ( ( std::none_of( returnParams.begin(), returnParams.end(), [&vpi]( size_t rpi ) { return vpi.first == rpi; } ) &&
             isParam( params[vpi.first].lenExpression, params ) ) ||
           ( singular && params[vpi.second.lenParam].type.isValue() ) )
      {
        skippedParams.insert( vpi.second.lenParam );
      }
      if ( vpi.second.strideParam != INVALID_INDEX )
      {
        skippedParams.insert( vpi.second.strideParam );
      }
    }
  }

  // skip the return parameters (get resolved by local variables to be returned)
  skippedParams.insert( returnParams.begin(), returnParams.end() );

  return skippedParams;
}

std::string VulkanHppGenerator::determineSubStruct( std::pair<std::string, StructureData> const & structure ) const
{
  if ( structure.second.members.front().name != "sType" )
  {
    // check if sd is a substruct of structure
    auto isSubStruct = [&structure]( std::pair<std::string, StructureData> const & sd ) noexcept
    {
      // member-by-member comparison of type and name
      auto memberIt = structure.second.members.begin();
      auto isMember = [&memberIt]( MemberData const & md ) noexcept
      {
        if ( ( md.type == memberIt->type ) && ( md.name == memberIt->name ) )
        {
          ++memberIt;
          return true;
        }
        return false;
      };

      return ( sd.second.members.size() < structure.second.members.size() ) && std::all_of( sd.second.members.begin(), sd.second.members.end(), isMember );
    };

    // look for a struct in m_structs that starts identically to structure
    auto structIt = std::find_if( m_structs.begin(), m_structs.end(), isSubStruct );
    return ( structIt == m_structs.end() ) ? "" : structIt->first;
  }
  return "";
}

std::map<size_t, VulkanHppGenerator::VectorParamData> VulkanHppGenerator::determineVectorParams( std::vector<ParamData> const & params ) const
{
  std::map<size_t, VectorParamData> vectorParams;

  // look for the parameters whose len equals the name of an other parameter
  for ( size_t i = 0; i < params.size(); i++ )
  {
    if ( !params[i].lenExpression.empty() && ( params[i].lenExpression != "null-terminated" ) && ( params[i].lenExpression != "1" ) )
    {
      VectorParamData & vpd = vectorParams[i];

      if ( params[i].lenParams.empty() )
      {
        std::string const & lenExpression = params[i].lenExpression;
        assert( std::none_of( params.begin(), params.end(), [&lenExpression]( auto const & pd ) { return ( lenExpression == pd.name ); } ) );
        auto lenIt =
          std::find_if( params.begin(), params.end(), [this, &lenExpression]( auto const & pd ) { return isLenByStructMember( lenExpression, pd ); } );
        assert( lenIt != params.end() );
        vpd.lenParam = std::distance( params.begin(), lenIt );
      }
      else
      {
        assert( params[i].lenParams.size() == 1 );
        vpd.lenParam = params[i].lenParams[0].second;
      }
      if ( !params[i].strideParam.first.empty() )
      {
        vpd.strideParam = params[i].strideParam.second;
      }
    }
    else
    {
      auto structIt = m_structs.find( params[i].type.type );
      if ( ( structIt != m_structs.end() ) && !isStructureChainAnchor( params[i].type.type ) && describesVector( structIt->second ) )
      {
        VectorParamData & vpd = vectorParams[i];
        vpd.byStructure       = true;
        vpd.lenParam          = 2;
      }
    }
  }
  return vectorParams;
}

std::set<size_t> VulkanHppGenerator::determineVoidPointerParams( std::vector<ParamData> const & params ) const
{
  std::set<size_t> voidPointerParams;

  for ( size_t i = 0; i < params.size(); i++ )
  {
    if ( !params[i].type.isValue() && ( params[i].type.type == "void" ) && ( params[i].type.postfix != "**" ) )
    {
      voidPointerParams.insert( i );
    }
  }
  return voidPointerParams;
}

void VulkanHppGenerator::distributeEnumExtends()
{
  for ( auto const & extendedEnum : m_enumExtends )
  {
    assert( !extendedEnum.second.empty() );
    auto typeIt = m_types.find( extendedEnum.first );
    checkForError( typeIt != m_types.end(),
                   extendedEnum.second.front().xmlLine,
                   "enum value <" + extendedEnum.second.front().name + "> extends unknown type <" + extendedEnum.first + ">" );
    checkForError( typeIt->second.category == TypeCategory::Enum,
                   extendedEnum.second.front().xmlLine,
                   "enum value <" + extendedEnum.second.front().name + "> extends non-enum type <" + extendedEnum.first + ">" );

    const auto enumIt = m_enums.find( extendedEnum.first );
    assert( enumIt != m_enums.end() );

    for ( auto const & eed : extendedEnum.second )
    {
      for ( auto const & requiredBy : eed.requiredBy )
      {
        typeIt->second.requiredBy.insert( requiredBy );
      }
      enumIt->second.addEnumAlias(
        eed.xmlLine, eed.name, eed.alias, getProtectFromPlatform( eed.platform ), ( eed.api.empty() || ( eed.api == m_api ) ) && eed.supported );
    }
  }
  m_enumExtends.clear();
}

void VulkanHppGenerator::distributeEnumValueAliases()
{
  assert( m_enumExtends.empty() );

  for ( auto & e : m_enums )
  {
    for ( auto & a : e.second.valueAliases )
    {
      auto valueIt = findByName( e.second.values, a.alias );
      if ( valueIt == e.second.values.end() )
      {
        auto aliasIt = findByName( e.second.valueAliases, a.alias );
        checkForError(
          aliasIt != e.second.valueAliases.end(), a.xmlLine, "enum value alias <" + a.name + "> aliases non-existent enum value <" + a.alias + ">" );
        valueIt = findByName( e.second.values, aliasIt->alias );
      }
      checkForError( valueIt != e.second.values.end(), a.xmlLine, "enum value alias <" + a.name + "> aliases non-existent enum value <" + a.alias + ">" );
      checkForError(
        a.protect == valueIt->protect, a.xmlLine, "enum value alias <" + a.name + "> aliases enum value <" + a.alias + "> with different properties" );
      checkForError( std::none_of( valueIt->aliases.begin(), valueIt->aliases.end(), [&a]( auto const & eav ) { return a.name == eav.name; } ),
                     a.xmlLine,
                     "enum value alias <" + a.name + "> already contained as alias for enum value <" + a.alias + ">" );
      valueIt->aliases.push_back( a );
    }
    e.second.valueAliases.clear();
  }
}

void VulkanHppGenerator::distributeRequirements()
{
  for ( auto const & feature : m_features )
  {
    distributeRequirements( feature.requireData, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    distributeRequirements( extension.requireData, extension.name );
    distributeRequirements( extension.unsupportedRequireData, extension.name );
  }
  for ( auto const & feature : m_unsupportedFeatures )
  {
    distributeRequirements( feature.requireData, feature.name );
  }
  for ( auto const & extension : m_unsupportedExtensions )
  {
    distributeRequirements( extension.requireData, extension.name );
    distributeRequirements( extension.unsupportedRequireData, extension.name );
  }
}

void VulkanHppGenerator::distributeRequirements( std::vector<RequireData> const & requireData, std::string const & requiredBy )
{
  for ( auto const & require : requireData )
  {
    for ( auto const & command : require.commands )
    {
      auto commandIt = findByNameOrAlias( m_commands, command.name );
      checkForError( commandIt != m_commands.end(), command.xmlLine, "unknown required command <" + command.name + ">" );
      commandIt->second.requiredBy.insert( requiredBy );
    }
    for ( auto const & type : require.types )
    {
      auto typeIt = m_types.find( type.name );
      checkForError( typeIt != m_types.end(), type.xmlLine, "unknown required type <" + type.name + ">" );
      typeIt->second.requiredBy.insert( requiredBy );
    }
  }
}

void VulkanHppGenerator::distributeStructAliases()
{
  for ( auto const & alias : m_structsAliases )
  {
    auto structIt = findByNameOrAlias( m_structs, alias.second.name );
    checkForError(
      structIt != m_structs.end(), alias.second.xmlLine, "struct alias <" + alias.first + "> references an unknown struct <" + alias.second.name + ">" );
    checkForError( structIt->second.aliases.insert( { alias.first, alias.second.xmlLine } ).second,
                   alias.second.xmlLine,
                   "struct alias <" + alias.first + "> already listed as an alias for struct <" + alias.second.name + ">" );
  }
  m_structsAliases.clear();
}

void VulkanHppGenerator::filterLenMembers()
{
  for ( auto & sd : m_structs )
  {
    for ( auto & member : sd.second.members )
    {
      std::erase_if( member.lenMembers, [this]( std::pair<std::string, size_t> const & nameIndex ) { return m_constants.contains( nameIndex.first ); } );

      for ( auto & lenMember : member.lenMembers )
      {
        if ( lenMember.second == static_cast<size_t>( ~0 ) )
        {
          auto lenMemberIt = findStructMemberIt( lenMember.first, sd.second.members );
          checkForError( lenMemberIt != sd.second.members.end(),
                         member.xmlLine,
                         "struct member <" + member.name + "> uses unknown len member <" + lenMember.first + "> in its \"altlen\" attribute <" +
                           member.lenExpressions[0] + ">" );
          lenMember.second = std::distance( sd.second.members.cbegin(), lenMemberIt );
        }
      }
    }
  }
}

std::map<std::string, VulkanHppGenerator::NameLine>::const_iterator VulkanHppGenerator::findAlias( std::string const &                     name,
                                                                                                   std::map<std::string, NameLine> const & aliases ) const
{
  auto lambda = [&name]( std::pair<std::string, NameLine> const & ad ) { return ad.second.name == name; };
  auto it     = std::find_if( aliases.begin(), aliases.end(), lambda );
  assert( ( it == aliases.end() ) || std::none_of( std::next( it ), aliases.end(), lambda ) );
  return it;
}

std::string VulkanHppGenerator::findBaseName( std::string aliasName, std::map<std::string, NameLine> const & aliases ) const
{
  std::string baseName = aliasName;
  auto        aliasIt  = aliases.find( baseName );
  while ( aliasIt != aliases.end() )
  {
    baseName = aliasIt->second.name;
    aliasIt  = aliases.find( baseName );
  }
  return baseName;
}

std::vector<VulkanHppGenerator::FeatureData>::const_iterator VulkanHppGenerator::findFeature( std::string const & name ) const
{
  return std::find_if( m_features.begin(), m_features.end(), [&name]( FeatureData const & fd ) { return fd.name == name; } );
}

std::vector<VulkanHppGenerator::ParamData>::const_iterator VulkanHppGenerator::findParamIt( std::string const &            name,
                                                                                            std::vector<ParamData> const & paramData ) const
{
  return std::find_if( paramData.begin(), paramData.end(), [&name]( ParamData const & pd ) { return pd.name == name; } );
}

std::vector<VulkanHppGenerator::MemberData>::const_iterator VulkanHppGenerator::findStructMemberIt( std::string const &             name,
                                                                                                    std::vector<MemberData> const & memberData ) const
{
  return std::find_if( memberData.begin(), memberData.end(), [&name]( MemberData const & md ) { return md.name == name; } );
}

std::vector<VulkanHppGenerator::MemberData>::const_iterator VulkanHppGenerator::findStructMemberItByType( std::string const &             type,
                                                                                                          std::vector<MemberData> const & memberData ) const
{
  return std::find_if( memberData.begin(), memberData.end(), [&type]( MemberData const & md ) { return md.type.type == type; } );
}

std::vector<VulkanHppGenerator::ExtensionData>::const_iterator VulkanHppGenerator::findSupportedExtension( std::string const & name ) const
{
  return std::find_if( m_extensions.begin(), m_extensions.end(), [&name]( ExtensionData const & ed ) { return ed.name == name; } );
}

std::string VulkanHppGenerator::findTag( std::string const & name, std::string const & postfix ) const
{
  auto tagIt = std::find_if(
    m_tags.begin(), m_tags.end(), [&name, &postfix]( std::pair<std::string, TagData> const & t ) { return name.ends_with( t.first + postfix ); } );
  return ( tagIt != m_tags.end() ) ? tagIt->first : "";
}

std::pair<std::string, std::string> VulkanHppGenerator::generateAllocatorTemplates( std::vector<size_t> const &               returnParams,
                                                                                    std::vector<std::string> const &          returnDataTypes,
                                                                                    std::map<size_t, VectorParamData> const & vectorParams,
                                                                                    std::vector<size_t> const &               chainedReturnParams,
                                                                                    CommandFlavourFlags                       flavourFlags,
                                                                                    bool                                      definition ) const
{
  assert( chainedReturnParams.size() <= 1 );
  const bool chained  = flavourFlags & CommandFlavourFlagBits::chained;
  const bool singular = flavourFlags & CommandFlavourFlagBits::singular;
  const bool unique   = flavourFlags & CommandFlavourFlagBits::unique;

  assert( returnParams.size() == returnDataTypes.size() );
  std::string allocatorTemplates;
  if ( !singular )
  {
    for ( size_t i = 0; i < returnParams.size(); i++ )
    {
      if ( vectorParams.contains( returnParams[i] ) )
      {
        if ( chained && !chainedReturnParams.empty() && ( chainedReturnParams[0] == returnParams[i] ) )
        {
          allocatorTemplates += "typename StructureChainAllocator";
          if ( !definition )
          {
            allocatorTemplates += " = std::allocator<StructureChain>";
          }
        }
        else
        {
          allocatorTemplates += "typename " + startUpperCase( stripPrefix( returnDataTypes[i], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator";
          if ( !definition )
          {
            allocatorTemplates += " = std::allocator<" + ( unique ? ( "UniqueHandle<" + returnDataTypes[i] + ", Dispatch>" ) : returnDataTypes[i] ) + ">";
          }
        }
        allocatorTemplates += ", ";
      }
    }
  }
  std::string uniqueHandleAllocatorTemplates;
  if ( unique && !allocatorTemplates.empty() )
  {
    uniqueHandleAllocatorTemplates = ", " + stripPostfix( allocatorTemplates, ", " );
    allocatorTemplates.clear();
  }
  return std::make_pair( allocatorTemplates, uniqueHandleAllocatorTemplates );
}

std::string VulkanHppGenerator::generateArgumentListEnhanced( std::vector<ParamData> const &            params,
                                                              std::vector<size_t> const &               returnParams,
                                                              std::map<size_t, VectorParamData> const & vectorParams,
                                                              std::set<size_t> const &                  skippedParams,
                                                              std::set<size_t> const &                  singularParams,
                                                              std::set<size_t> const &                  templatedParams,
                                                              std::vector<size_t> const &               chainedReturnParams,
                                                              bool                                      definition,
                                                              CommandFlavourFlags                       flavourFlags,
                                                              bool                                      withDispatcher ) const
{
  const bool   withAllocators    = flavourFlags & CommandFlavourFlagBits::withAllocator;
  const size_t defaultStartIndex = withAllocators ? ~0 : determineDefaultStartIndex( params, skippedParams );

  std::vector<std::string> arguments;
  for ( size_t i = 0; i < params.size(); ++i )
  {
    if ( !skippedParams.contains( i ) )
    {
      bool hasDefaultAssignment = false;

      std::string composedType = params[i].type.compose( "VULKAN_HPP_NAMESPACE" );

      if ( singularParams.contains( i ) )
      {
        assert( !params[i].optional );
        assert( params[i].type.isConstPointer() && !params[i].lenExpression.empty() && !isLenByStructMember( params[i].lenExpression, params ) &&
                params[i].type.type.starts_with( "Vk" ) );
        assert( !isHandleType( params[i].type.type ) );
        assert( composedType.ends_with( " *" ) );
        arguments.push_back( stripPostfix( composedType, " *" ) + " & " + stripPluralS( startLowerCase( stripPrefix( params[i].name, "p" ) ) ) );
      }
      else if ( params[i].type.isConstPointer() )
      {
        assert( composedType.ends_with( " *" ) );
        std::string name = startLowerCase( stripPrefix( params[i].name, "p" ) );
        if ( params[i].lenExpression.empty() )
        {
          assert( withDispatcher || !isHandleType( params[i].type.type ) );
          assert( !params[i].type.prefix.empty() && ( params[i].type.postfix == "*" ) );
          assert( params[i].arraySizes.empty() );
          if ( params[i].type.type == "void" )
          {
            arguments.push_back( templatedParams.contains( i ) ? ( stripPrefix( params[i].name, "p" ) + "Type const & " + name )
                                                               : ( composedType + " " + params[i].name ) );
          }
          else if ( params[i].optional )
          {
            arguments.push_back( "Optional<" + stripPostfix( composedType, " *" ) + "> " + name +
                                 ( ( definition || withAllocators ) ? "" : " VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT" ) );
            hasDefaultAssignment = true;
          }
          else
          {
            arguments.push_back( stripPostfix( composedType, " *" ) + " & " + name );
          }
        }
        else
        {
          // a const-pointer with a non-empty len is either null-terminated (aka a string) or represented by an
          // ArrayProxy
          assert( params[i].arraySizes.empty() );
          if ( params[i].lenExpression == "null-terminated" )
          {
            assert( params[i].type.type == "char" );
            if ( params[i].optional )
            {
              arguments.push_back( "Optional<const std::string> " + name +
                                   ( ( definition || withAllocators ) ? "" : " VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT" ) );
              hasDefaultAssignment = true;
            }
            else
            {
              arguments.push_back( "const std::string & " + name );
            }
          }
          else
          {
            // an ArrayProxy also covers no data, so any optional flag can be ignored here
            std::string  type = stripPostfix( composedType, " *" );
            const size_t pos  = type.find( "void" );
            if ( pos != std::string::npos )
            {
              type.replace( pos, 4, stripPrefix( params[i].name, "p" ) + "Type" );
            }
            arguments.push_back( std::string( "VULKAN_HPP_NAMESPACE::" ) + ( params[i].strideParam.first.empty() ? "" : "Strided" ) + "ArrayProxy<" + type +
                                 "> const & " + name );
            if ( params[i].optional && !definition )
            {
              assert( params[i].strideParam.first.empty() );
              arguments.back() += " VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT";
              hasDefaultAssignment = true;
            }
          }
        }
      }
      else if ( params[i].type.isNonConstPointer() )
      {
        if ( ( params[i].type.type == "void" ) && ( flavourFlags & CommandFlavourFlagBits::keepVoidPtr ) )
        {
          arguments.push_back( composedType + " " );
        }
        else
        {
          assert( withDispatcher || !isHandleType( params[i].type.type ) );
          assert( params[i].lenExpression.empty() && !params[i].optional );
          assert( composedType.ends_with( " *" ) );
          arguments.push_back( stripPostfix( composedType, " *" ) + " & " );
        }
        arguments.back() += params[i].name;
      }
      else
      {
        assert( params[i].type.isValue() );
        // parameters named "objectType" collide with the member variable -> append an _ to here
        std::string paramName = ( definition && ( params[i].name == "objectType" ) ) ? "objectType_" : params[i].name;
        arguments.push_back( composedType + " " + paramName + generateCArraySizes( params[i].arraySizes ) );
      }
      arguments.back() += std::string( !definition && ( defaultStartIndex <= i ) && !hasDefaultAssignment ? " VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT" : "" );
    }
  }
  if ( withAllocators )
  {
    if ( ( flavourFlags & CommandFlavourFlagBits::chained ) && needsStructureChainResize( vectorParams, chainedReturnParams ) )
    {
      arguments.push_back( "StructureChainAllocator & structureChainAllocator" );
    }
    else
    {
      for ( auto sp : skippedParams )
      {
        if ( !params[sp].lenExpression.empty() )
        {
          std::string type;
          if ( templatedParams.contains( sp ) )
          {
            auto vectorParamIt = vectorParams.find( sp );
            if ( ( vectorParamIt != vectorParams.end() ) &&
                 std::any_of( returnParams.begin(), returnParams.end(), [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->first; } ) &&
                 std::any_of(
                   returnParams.begin(), returnParams.end(), [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->second.lenParam; } ) )
            {
              type = "Uint8_t";
            }
            else
            {
              type = stripPrefix( params[sp].name, "p" ) + "Type";
            }
          }
          else
          {
            type = ( params[sp].type.type == "void" ) ? "Uint8_t" : startUpperCase( stripPrefix( params[sp].type.type, "Vk" ) );
          }
          arguments.push_back( type + "Allocator & " + startLowerCase( type ) + "Allocator" );
        }
        else if ( vectorParams.contains( sp ) && vectorParams.find( sp )->second.byStructure )
        {
          std::string type = stripPrefix( vectorMemberByStructure( params[sp].type.type ).type.type, "Vk" );
          assert( isupper( type[0] ) );
          arguments.push_back( type + "Allocator & " + startLowerCase( type ) + "Allocator" );
        }
      }
    }
  }
  if ( withDispatcher )
  {
    arguments.push_back( std::string( "Dispatch const & d" ) + ( definition ? "" : " VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT" ) );
  }
  return generateList( arguments, "", ", " );
}

std::string VulkanHppGenerator::generateArgumentListStandard( std::vector<ParamData> const & params,
                                                              std::set<size_t> const &       skippedParams,
                                                              bool                           definition,
                                                              bool                           withDispatcher ) const
{
  std::string argumentList;
  for ( size_t i = 0; i < params.size(); ++i )
  {
    if ( !skippedParams.contains( i ) )
    {
      // parameters named "objectType" collide with the member variable -> append an _ to here
      std::string paramName = ( definition && ( params[i].name == "objectType" ) ) ? "objectType_" : params[i].name;
      argumentList += params[i].type.compose( "VULKAN_HPP_NAMESPACE" ) + " " + paramName + generateCArraySizes( params[i].arraySizes ) + ", ";
    }
  }
  if ( withDispatcher )
  {
    argumentList += "Dispatch const & d ";
  }
  else if ( !argumentList.empty() )
  {
    assert( argumentList.ends_with( ", " ) );
    argumentList = stripPostfix( argumentList, ", " ) + " ";
  }
  return argumentList;
}

std::string VulkanHppGenerator::generateArgumentTemplates( std::vector<ParamData> const &            params,
                                                           std::vector<size_t> const &               returnParams,
                                                           std::map<size_t, VectorParamData> const & vectorParams,
                                                           std::set<size_t> const &                  templatedParams,
                                                           std::vector<size_t> const &               chainedReturnParams,
                                                           bool                                      raii ) const
{
  std::string argumentTemplates;
  if ( !chainedReturnParams.empty() )
  {
    assert( chainedReturnParams.size() == 1 );
    argumentTemplates = vectorParams.contains( chainedReturnParams[0] ) ? "typename StructureChain, " : "typename X, typename Y, typename... Z, ";
  }
  if ( !templatedParams.empty() )
  {
    for ( auto t : templatedParams )
    {
      assert( params[t].name.starts_with( "p" ) );
      auto vectorParamIt = vectorParams.find( t );
      if ( ( vectorParamIt == vectorParams.end() ) ||
           std::none_of( returnParams.begin(), returnParams.end(), [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->first; } ) ||
           std::none_of( returnParams.begin(), returnParams.end(), [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->second.lenParam; } ) )
      {
        assert( chainedReturnParams.empty() );
        // only templated parameters that are not part of an enumeration are really templated
        argumentTemplates += "typename " + stripPrefix( params[t].name, "p" ) + "Type, ";
      }
    }
  }
  if ( !argumentTemplates.empty() && raii )
  {
    argumentTemplates = "template <" + stripPostfix( argumentTemplates, ", " ) + ">";
  }
  return argumentTemplates;
}

std::string VulkanHppGenerator::generateBaseTypes() const
{
  assert( !m_baseTypes.empty() );
  const std::string basetypesTemplate = R"(
  //==================
  //=== BASE TYPEs ===
  //==================

${basetypes}
)";

  std::string basetypes;
  for ( auto const & baseType : m_baseTypes )
  {
    // filter out VkFlags and VkFlags64, as they are mapped to our own Flags class, and basetypes without any type information
    if ( ( baseType.first != "VkFlags" ) && ( baseType.first != "VkFlags64" ) && !baseType.second.typeInfo.type.empty() )
    {
      basetypes += "  using " + stripPrefix( baseType.first, "Vk" ) + " = " + baseType.second.typeInfo.compose( "VULKAN_HPP_NAMESPACE" ) + ";\n";
    }
  }

  return replaceWithMap( basetypesTemplate, { { "basetypes", basetypes } } );
}

std::string VulkanHppGenerator::generateBitmask( std::map<std::string, BitmaskData>::const_iterator bitmaskIt, std::string const & surroundingProtect ) const
{
  auto bitmaskBitsIt = m_enums.find( bitmaskIt->second.require );
  assert( bitmaskBitsIt != m_enums.end() );

  std::string bitmaskName = stripPrefix( bitmaskIt->first, "Vk" );
  std::string enumName    = stripPrefix( bitmaskBitsIt->first, "Vk" );

  std::string allFlags;
  if ( bitmaskBitsIt->second.values.empty() ||
       std::none_of( bitmaskBitsIt->second.values.begin(), bitmaskBitsIt->second.values.end(), []( auto const & evd ) { return evd.supported; } ) )
  {
    allFlags = " {};";
  }
  else
  {
    bool        encounteredFlag = false;
    std::string previousEnter, previousLeave;
    for ( auto const & value : bitmaskBitsIt->second.values )
    {
      if ( value.supported )
      {
        // if the value's protect differs from the surrounding protect, generate protection code
        std::string enter, leave;
        if ( !value.protect.empty() && ( value.protect != surroundingProtect ) )
        {
          tie( enter, leave ) = generateProtection( value.protect );
        }
        std::string valueName = generateEnumValueName( bitmaskBitsIt->first, value.name, true );
        allFlags += ( ( previousEnter != enter ) ? ( "\n" + previousLeave + enter ) : "\n" ) + "        " + ( encounteredFlag ? "| " : "  " ) + enumName +
                    "::" + valueName;
        encounteredFlag = true;
        previousEnter   = enter;
        previousLeave   = leave;
      }
    }
    if ( !previousLeave.empty() )
    {
      allFlags += "\n" + previousLeave;
    }
    allFlags += ";";
  }

  std::string aliases;
  for ( auto const & a : bitmaskIt->second.aliases )
  {
    aliases += "  using " + stripPrefix( a.first, "Vk" ) + " = " + bitmaskName + ";\n";
  }

  static const std::string bitmaskTemplate = R"(
  using ${bitmaskName} = Flags<${enumName}>;
${aliases}

  template <> struct FlagTraits<${enumName}>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ${bitmaskName} allFlags = ${allFlags}
  };
)";

  return replaceWithMap( bitmaskTemplate, { { "aliases", aliases }, { "allFlags", allFlags }, { "bitmaskName", bitmaskName }, { "enumName", enumName } } );
}

std::string VulkanHppGenerator::generateBitmasksToString() const
{
  const std::string bitmasksToStringTemplate = R"(
  //==========================
  //=== BITMASKs to_string ===
  //==========================

${bitmasksToString}
)";

  std::string           bitmasksToString;
  std::set<std::string> listedBitmasks;
  for ( auto const & feature : m_features )
  {
    bitmasksToString += generateBitmasksToString( feature.requireData, listedBitmasks, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    bitmasksToString += generateBitmasksToString( extension.requireData, listedBitmasks, extension.name );
  }

  return replaceWithMap( bitmasksToStringTemplate, { { "bitmasksToString", bitmasksToString } } );
}

std::string VulkanHppGenerator::generateBitmasksToString( std::vector<RequireData> const & requireData,
                                                          std::set<std::string> &          listedBitmasks,
                                                          std::string const &              title ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto bitmaskIt = m_bitmasks.find( type.name );
      if ( ( bitmaskIt != m_bitmasks.end() ) && !listedBitmasks.contains( type.name ) )
      {
        listedBitmasks.insert( type.name );
        str += generateBitmaskToString( bitmaskIt );
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateBitmaskToString( std::map<std::string, BitmaskData>::const_iterator bitmaskIt ) const
{
  auto bitmaskBitsIt = m_enums.find( bitmaskIt->second.require );
  assert( bitmaskBitsIt != m_enums.end() );

  std::string bitmaskName = stripPrefix( bitmaskIt->first, "Vk" );
  std::string enumName    = stripPrefix( bitmaskBitsIt->first, "Vk" );

  std::string str;
  if ( bitmaskBitsIt->second.values.empty() ||
       std::none_of( bitmaskBitsIt->second.values.begin(), bitmaskBitsIt->second.values.end(), []( auto const & evd ) { return evd.supported; } ) )
  {
    static std::string bitmaskToStringTemplate = R"(
  VULKAN_HPP_INLINE std::string to_string( ${bitmaskName} )
  {
    return "{}";
  }
)";
    str += replaceWithMap( bitmaskToStringTemplate, { { "bitmaskName", bitmaskName } } );
  }
  else
  {
    static const std::string bitmaskToStringTemplate = R"(
  VULKAN_HPP_INLINE std::string to_string( ${bitmaskName} value )
  {
    if ( !value )
      return "${emptyValue}";

    std::string result;
${toStringChecks}
    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }
)";

    std::string emptyValue = "{}";
    std::string toStringChecks;
    std::string previousEnter, previousLeave;
    for ( auto const & value : bitmaskBitsIt->second.values )
    {
      if ( value.supported )
      {
        std::string valueName = generateEnumValueName( bitmaskBitsIt->first, value.name, true );
        if ( value.value == "0" )
        {
          assert( emptyValue == "{}" );
          emptyValue = valueName.substr( 1 );
        }
        else if ( !value.bitpos.empty() )
        {
          const auto [enter, leave] = generateProtection( value.protect );
          toStringChecks += ( ( previousEnter != enter ) ? ( previousLeave + enter ) : "" ) + "    if ( value & " + enumName + "::" + valueName +
                            " ) result += \"" + valueName.substr( 1 ) + " | \";\n";
          previousEnter = enter;
          previousLeave = leave;
        }
      }
    }
    if ( !previousLeave.empty() )
    {
      assert( previousLeave.ends_with( "\n" ) );
      toStringChecks += previousLeave;
      previousLeave.resize( previousLeave.size() - strlen( "\n" ) );
    }

    str += replaceWithMap( bitmaskToStringTemplate, { { "bitmaskName", bitmaskName }, { "emptyValue", emptyValue }, { "toStringChecks", toStringChecks } } );
  }

  return str;
}

std::string VulkanHppGenerator::generateCallArgumentsEnhanced( CommandData const &      commandData,
                                                               size_t                   initialSkipCount,
                                                               bool                     nonConstPointerAsNullptr,
                                                               std::set<size_t> const & singularParams,
                                                               std::set<size_t> const & templatedParams,
                                                               bool                     raii,
                                                               bool                     raiiFactory,
                                                               CommandFlavourFlags      flavourFlags ) const
{
  assert( initialSkipCount <= commandData.params.size() );
  std::vector<std::string> arguments;
  if ( raii )
  {
    switch ( initialSkipCount )
    {
      case 0: break;
      case 1:
        assert( isHandleType( commandData.params[0].type.type ) && commandData.params[0].type.isValue() );
        assert( commandData.params[0].arraySizes.empty() && commandData.params[0].lenExpression.empty() );
        assert( commandData.params[0].type.type == commandData.handle );
        arguments.push_back( "static_cast<" + commandData.handle + ">( m_" + startLowerCase( stripPrefix( commandData.handle, "Vk" ) ) + " )" );
        break;
      case 2:
        {
          assert( isHandleType( commandData.params[0].type.type ) && commandData.params[0].type.isValue() );
          assert( commandData.params[0].arraySizes.empty() && commandData.params[0].lenExpression.empty() );
          assert( commandData.params[0].type.type == commandData.handle );
          auto handleIt = m_handles.find( commandData.params[1].type.type );
          assert( handleIt != m_handles.end() );
          arguments.push_back( "static_cast<" + commandData.handle + ">( m_" + startLowerCase( stripPrefix( commandData.handle, "Vk" ) ) + " )" );

          assert( commandData.params[1].type.isValue() && commandData.params[1].arraySizes.empty() && commandData.params[1].lenExpression.empty() );
          arguments.push_back( "static_cast<" + commandData.params[1].type.type + ">( m_" +
                               generateRAIIHandleConstructorParamName( handleIt->first, handleIt->second.destructorIt ) + " )" );
        }
        break;
      default: assert( false ); break;
    }
  }
  else
  {
    for ( size_t i = 0; i < initialSkipCount; ++i )
    {
      assert( isHandleType( commandData.params[i].type.type ) && commandData.params[i].type.isValue() );
      assert( commandData.params[i].arraySizes.empty() && commandData.params[i].lenExpression.empty() );
      arguments.push_back( "m_" + startLowerCase( stripPrefix( commandData.params[i].type.type, "Vk" ) ) );
    }
  }
  for ( size_t i = initialSkipCount; i < commandData.params.size(); ++i )
  {
    arguments.push_back(
      generateCallArgumentEnhanced( commandData.params, i, nonConstPointerAsNullptr, singularParams, templatedParams, flavourFlags, raiiFactory ) );
  }
  return generateList( arguments, "", ", " );
}

std::string VulkanHppGenerator::generateCallArgumentsStandard( std::vector<ParamData> const & params, size_t initialSkipCount ) const
{
  std::vector<std::string> arguments;
  for ( size_t i = 0; i < initialSkipCount; ++i )
  {
    auto const & param = params[i];
    assert( isHandleType( param.type.type ) && param.type.isValue() );
    assert( param.arraySizes.empty() && param.lenExpression.empty() );
    arguments.push_back( "static_cast<" + param.type.type + ">( m_" + startLowerCase( stripPrefix( param.type.type, "Vk" ) ) + " )" );
  }
  for ( size_t i = initialSkipCount; i < params.size(); ++i )
  {
    auto const & param    = params[i];
    std::string  argument = param.name;
    if ( param.type.type.starts_with( "Vk" ) )
    {
      if ( !param.arraySizes.empty() )
      {
        assert( param.arraySizes.size() == 1 );
        assert( param.type.isValue() );
        assert( param.type.postfix.empty() );
        argument = "reinterpret_cast<" + param.type.compose( "" ) + " *>( " + argument + " )";
      }
      else if ( param.type.isValue() )
      {
        // parameters named "objectType" collide with the member variable -> append an _ to here
        if ( argument == "objectType" )
        {
          argument += "_";
        }
        argument = "static_cast<" + param.type.type + ">( " + argument + " )";
      }
      else
      {
        assert( !param.type.postfix.empty() );
        argument = "reinterpret_cast<" + param.type.compose( "" ) + ">( " + argument + " )";
      }
    }
    arguments.push_back( argument );
  }
  return generateList( arguments, "", ", " );
}

std::string VulkanHppGenerator::generateCallArgumentEnhanced( std::vector<ParamData> const & params,
                                                              size_t                         paramIndex,
                                                              bool                           nonConstPointerAsNullptr,
                                                              std::set<size_t> const &       singularParams,
                                                              std::set<size_t> const &       templatedParams,
                                                              CommandFlavourFlags            flavourFlags,
                                                              bool                           raiiFactory ) const
{
  std::string       argument;
  ParamData const & param = params[paramIndex];
  if ( param.type.isConstPointer() || specialPointerTypes.contains( param.type.type ) )
  {
    // parameter is a const-pointer or one of the special pointer types that are considered to be const-pointers
    argument = generateCallArgumentEnhancedConstPointer( param, paramIndex, singularParams, templatedParams );
  }
  else if ( param.type.isNonConstPointer() && !specialPointerTypes.contains( param.type.type ) &&
            !( ( param.type.type == "void" ) && ( flavourFlags & CommandFlavourFlagBits::keepVoidPtr ) ) )
  {
    // parameter is a non-const pointer, none of the special pointer types, that are considered const-pointers, and no void-ptr to keep
    argument = generateCallArgumentEnhancedNonConstPointer( param, paramIndex, nonConstPointerAsNullptr, singularParams );
  }
  else
  {
    argument = generateCallArgumentEnhancedValue( params, paramIndex, singularParams, flavourFlags, raiiFactory );
  }
  assert( !argument.empty() );
  return argument;
}

std::string VulkanHppGenerator::generateCallArgumentEnhancedConstPointer( ParamData const &        param,
                                                                          size_t                   paramIndex,
                                                                          std::set<size_t> const & singularParams,
                                                                          std::set<size_t> const & templatedParams ) const
{
  std::string argument;
  std::string name = startLowerCase( stripPrefix( param.name, "p" ) );
  if ( isHandleType( param.type.type ) && param.type.isValue() )
  {
    assert( !param.optional );
    // if at all, this is the first argument, and it's the implicitly provided member handle
    assert( paramIndex == 0 );
    assert( param.arraySizes.empty() && param.lenExpression.empty() );
    argument = "m_" + startLowerCase( stripPrefix( param.type.type, "Vk" ) );
  }
  else if ( param.lenExpression.empty() )
  {
    // this const-pointer parameter has no length, that is it's a const-pointer to a single value
    if ( param.type.type == "void" )
    {
      argument =
        templatedParams.contains( paramIndex ) ? "reinterpret_cast<" + param.type.compose( "VULKAN_HPP_NAMESPACE" ) + ">( &" + name + " )" : param.name;
    }
    else if ( param.optional )
    {
      argument = "static_cast<" + param.type.compose( "VULKAN_HPP_NAMESPACE" ) + ">( " + name + " )";
    }
    else
    {
      argument = "&" + name;
    }
    if ( param.type.type.starts_with( "Vk" ) )
    {
      argument = "reinterpret_cast<" + param.type.compose( "" ) + ">( " + argument + " )";
    }
  }
  else if ( param.lenExpression == "null-terminated" )
  {
    // this const-pointer parameter is "null-terminated", that is it's a string
    assert( ( param.type.type == "char" ) && param.arraySizes.empty() );
    if ( param.optional )
    {
      argument = name + " ? " + name + "->c_str() : nullptr";
    }
    else
    {
      argument = name + ".c_str()";
    }
  }
  else
  {
    // this const-pointer parameter has some explicit length
    if ( singularParams.contains( paramIndex ) )
    {
      assert( !param.optional );
      argument = "&" + stripPluralS( name );
    }
    else
    {
      // this const-parameter is represented by some array, where data() also works with no data (optional)
      argument = name + ".data()";
    }
    if ( param.type.type.starts_with( "Vk" ) || ( param.type.type == "void" ) )
    {
      argument = "reinterpret_cast<" + param.type.compose( "" ) + ">( " + argument + " )";
    }
  }
  return argument;
}

std::string VulkanHppGenerator::generateCallArgumentEnhancedNonConstPointer( ParamData const &        param,
                                                                             size_t                   paramIndex,
                                                                             bool                     nonConstPointerAsNullptr,
                                                                             std::set<size_t> const & singularParams ) const
{
  std::string argument;
  std::string name = startLowerCase( stripPrefix( param.name, "p" ) );
  if ( param.lenExpression.empty() || ( param.lenExpression == "1" ) )
  {
    assert( param.arraySizes.empty() );
    if ( param.type.type.starts_with( "Vk" ) )
    {
      argument = "reinterpret_cast<" + param.type.compose( "" ) + ">( &" + name + " )";
    }
    else
    {
      assert( !param.optional );
      argument = "&" + name;
    }
  }
  else
  {
    // the non-const pointer has a lenExpression -> it will be represented by some array
    assert( param.arraySizes.empty() );
    if ( nonConstPointerAsNullptr )
    {
      argument = "nullptr";
    }
    else
    {
      if ( singularParams.contains( paramIndex ) )
      {
        argument = "&" + stripPluralS( name );
      }
      else
      {
        // get the data of the array, which also covers no data -> no need to look at param.optional
        argument = name + ".data()";
      }
      if ( param.type.type.starts_with( "Vk" ) || ( param.type.type == "void" ) )
      {
        argument = "reinterpret_cast<" + param.type.compose( "" ) + ">( " + argument + " )";
      }
    }
  }
  return argument;
}

std::string VulkanHppGenerator::generateCallArgumentEnhancedValue(
  std::vector<ParamData> const & params, size_t paramIndex, std::set<size_t> const & singularParams, CommandFlavourFlags flavourFlags, bool raiiFactory ) const
{
  std::string       argument;
  ParamData const & param = params[paramIndex];
  assert( ( flavourFlags & CommandFlavourFlagBits::keepVoidPtr ) || param.lenExpression.empty() );  // !keepVoidPtr -> no lenExpression
  if ( param.type.type.starts_with( "Vk" ) )
  {
    if ( param.arraySizes.empty() )
    {
      // check if this param is used as the len of an other param
      auto pointerIt = std::find_if( params.begin(), params.end(), [&param]( ParamData const & pd ) { return pd.lenExpression == param.name; } );
      if ( pointerIt != params.end() )
      {
        assert( !param.optional );
        argument = startLowerCase( stripPrefix( pointerIt->name, "p" ) ) + ".size()";
        if ( pointerIt->type.type == "void" )
        {
          argument += " * sizeof( " + stripPrefix( pointerIt->name, "p" ) + "Type )";
        }
      }
      else if ( raiiFactory && m_handles.contains( param.type.type ) )
      {
        assert( param.type.isValue() && param.lenExpression.empty() );
        if ( param.optional )
        {
          argument = param.name + " ? static_cast<" + param.type.type + ">( **" + param.name + " ) : 0";
        }
        else
        {
          argument = "static_cast<" + param.type.type + ">( *" + param.name + " )";
        }
      }
      else
      {
        assert( param.type.isValue() );
        // parameters named "objectType" collide with the member variable -> append an _ to here
        std::string paramName = ( param.name == "objectType" ) ? "objectType_" : param.name;
        argument              = "static_cast<" + param.type.type + ">( " + paramName + " )";
      }
      // check if this param is used as the stride of an other param
      assert( std::none_of( params.begin(), params.end(), [paramIndex]( ParamData const & pd ) { return pd.strideParam.second == paramIndex; } ) );
    }
    else
    {
      assert( !param.optional );
      assert( param.arraySizes.size() == 1 );
      assert( param.type.prefix == "const" );
      argument = "reinterpret_cast<" + param.type.compose( "" ) + " *>( " + param.name + " )";
    }
  }
  else
  {
    auto pointerIt = std::find_if( params.begin(), params.end(), [&param]( ParamData const & pd ) { return pd.lenExpression == param.name; } );
    if ( ( pointerIt != params.end() ) && !( ( pointerIt->type.type == "void" ) && ( flavourFlags & CommandFlavourFlagBits::keepVoidPtr ) ) )
    {
      // this parameter is the len of some other -> replace it with that parameter's size
      assert( param.arraySizes.empty() );
      assert( ( param.type.type == "size_t" ) || ( param.type.type == "uint32_t" ) );
      if ( singularParams.contains( paramIndex ) )
      {
        if ( pointerIt->type.type == "void" )
        {
          argument = "sizeof( " + stripPrefix( pointerIt->name, "p" ) + "Type )";
        }
        else
        {
          argument = "1";
        }
      }
      else
      {
        argument = startLowerCase( stripPrefix( pointerIt->name, "p" ) ) + ".size()";
        if ( pointerIt->type.type == "void" )
        {
          argument += " * sizeof( " + stripPrefix( pointerIt->name, "p" ) + "Type )";
        }
      }
    }
    else
    {
      assert( !param.optional );
      assert( param.arraySizes.size() <= 1 );
      pointerIt = std::find_if( params.begin(), params.end(), [paramIndex]( ParamData const & pd ) { return pd.strideParam.second == paramIndex; } );
      if ( pointerIt != params.end() )
      {
        // this parameter is the stride of some other -> replace it with that parameter's stride
        assert( param.arraySizes.empty() );
        assert( param.type.type == "uint32_t" );
        assert( pointerIt->strideParam.first == param.name );
        argument = startLowerCase( stripPrefix( pointerIt->name, "p" ) ) + ".stride()";
      }
      else
      {
        argument = param.name;
      }
    }
  }
  return argument;
}

std::string VulkanHppGenerator::generateCallSequence( std::string const &                       name,
                                                      CommandData const &                       commandData,
                                                      std::vector<size_t> const &               returnParams,
                                                      std::map<size_t, VectorParamData> const & vectorParams,
                                                      size_t                                    initialSkipCount,
                                                      std::set<size_t> const &                  singularParams,
                                                      std::set<size_t> const &                  templatedParams,
                                                      std::vector<size_t> const &               chainedReturnParams,
                                                      CommandFlavourFlags                       flavourFlags,
                                                      bool                                      raii,
                                                      bool                                      raiiFactory ) const
{
  std::string dispatcher = raii ? "getDispatcher()->" : "d.";

  // first some special handling on vkCreatePipelineBinariesKHR !!
  if ( name == "vkCreatePipelineBinariesKHR" )
  {
#if !defined( NDEBUG )
    auto paramIt = std::find_if( commandData.params.begin(), commandData.params.end(), []( ParamData const & pd ) { return pd.name == "pCreateInfo"; } );
    assert( paramIt != commandData.params.end() && ( paramIt->type.type == "VkPipelineBinaryCreateInfoKHR" ) );
    auto structIt = m_structs.find( "VkPipelineBinaryCreateInfoKHR" );
    assert( ( structIt != m_structs.end() ) &&
            std::any_of( structIt->second.members.begin(), structIt->second.members.end(), []( MemberData const & md ) { return md.name == "pipeline"; } ) &&
            std::any_of(
              structIt->second.members.begin(), structIt->second.members.end(), []( MemberData const & md ) { return md.name == "pPipelineCreateInfo"; } ) );
    auto memberIt =
      std::find_if( structIt->second.members.begin(), structIt->second.members.end(), []( MemberData const & md ) { return md.name == "pKeysAndDataInfo"; } );
    assert( memberIt != structIt->second.members.end() && ( memberIt->type.type == "VkPipelineBinaryKeysAndDataKHR" ) );
    structIt = m_structs.find( "VkPipelineBinaryKeysAndDataKHR" );
    assert( ( structIt != m_structs.end() ) &&
            std::any_of( structIt->second.members.begin(), structIt->second.members.end(), []( MemberData const & md ) { return md.name == "binaryCount"; } ) );
    structIt = m_structs.find( "VkPipelineBinaryHandlesInfoKHR" );
    assert(
      ( structIt != m_structs.end() ) &&
      std::any_of(
        structIt->second.members.begin(), structIt->second.members.end(), []( MemberData const & md ) { return md.name == "pipelineBinaryCount"; } ) &&
      std::any_of( structIt->second.members.begin(), structIt->second.members.end(), []( MemberData const & md ) { return md.name == "pPipelineBinaries"; } ) );
#endif

    const std::string callSequenceTemplate = R"(    VULKAN_HPP_NAMESPACE::Result result;
    if ( createInfo.pKeysAndDataInfo )
    {
      VULKAN_HPP_ASSERT( !createInfo.pipeline && !createInfo.pPipelineCreateInfo );
      pipelineBinaries.resize( createInfo.pKeysAndDataInfo->binaryCount );
      binaries.pipelineBinaryCount = createInfo.pKeysAndDataInfo->binaryCount;
      binaries.pPipelineBinaries = pipelineBinaries.data();
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( ${dispatcher}${vkCommand}( ${callArguments} ) );
    }
    else
    {
      VULKAN_HPP_ASSERT( !createInfo.pipeline ^ !createInfo.pPipelineCreateInfo );
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( ${dispatcher}${vkCommand}( ${callArguments} ) );
      if ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
        pipelineBinaries.resize( binaries.pipelineBinaryCount );
        binaries.pPipelineBinaries = pipelineBinaries.data();
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( ${dispatcher}${vkCommand}( ${callArguments} ) );
      }
    }
)";

    std::string callArguments =
      generateCallArgumentsEnhanced( commandData, initialSkipCount, false, singularParams, templatedParams, raii, raiiFactory, flavourFlags );

    return replaceWithMap( callSequenceTemplate, { { "callArguments", callArguments }, { "dispatcher", dispatcher }, { "vkCommand", name } } );
  }

  // if at least one returnParam is a size value of a vector param (and no singular params), we need two calls
  if ( singularParams.empty() &&
       std::any_of( returnParams.begin(),
                    returnParams.end(),
                    [&vectorParams]( size_t rp )
                    { return std::any_of( vectorParams.begin(), vectorParams.end(), [rp]( auto const & vp ) { return vp.second.lenParam == rp; } ); } ) )
  {
    auto vectorParamIt = vectorParams.begin();
    assert( std::any_of( returnParams.begin(), returnParams.end(), [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->first; } ) );
    assert( std::any_of( returnParams.begin(), returnParams.end(), [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->second.lenParam; } ) );

    std::string firstCallArguments = generateCallArgumentsEnhanced( commandData, initialSkipCount, true, {}, templatedParams, raii, raiiFactory, flavourFlags );
    std::string secondCallArguments =
      generateCallArgumentsEnhanced( commandData, initialSkipCount, false, {}, templatedParams, raii, raiiFactory, flavourFlags );
    std::string vectorName, vectorSize;
    if ( vectorParamIt->second.byStructure )
    {
      auto structIt = m_structs.find( commandData.params[vectorParamIt->first].type.type );
      assert( structIt != m_structs.end() );
      vectorName = structIt->second.members.back().name;
      vectorSize = startLowerCase( stripPrefix( commandData.params[vectorParamIt->first].name, "p" ) ) + "." +
                   structIt->second.members[vectorParamIt->second.lenParam].name;
    }
    else
    {
      vectorName = commandData.params[vectorParamIt->first].name;
      vectorSize = startLowerCase( stripPrefix( commandData.params[vectorParamIt->second.lenParam].name, "p" ) );
    }
    vectorName = startLowerCase( stripPrefix( vectorName, "p" ) );

    if ( ( flavourFlags & CommandFlavourFlagBits::chained ) && needsStructureChainResize( vectorParams, chainedReturnParams ) )
    {
      assert( vectorParams.size() == 1 );
      // chained data needs some more handling!!
      std::string vectorElementType = stripPostfix( commandData.params[vectorParamIt->first].type.compose( "VULKAN_HPP_NAMESPACE" ), " *" );

      if ( commandData.returnType == "VkResult" )
      {
        const std::string callSequenceTemplate = R"(VULKAN_HPP_NAMESPACE::Result result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( ${dispatcher}${vkCommand}( ${firstCallArguments} ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && ${counterName} )
      {
        structureChains.resize( ${counterName} );
        ${vectorName}.resize( ${counterName} );
        for ( ${counterType} i = 0; i < ${counterName}; i++ )
        {
          ${vectorName}[i].pNext = structureChains[i].template get<${vectorElementType}>().pNext;
        }
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( ${dispatcher}${vkCommand}( ${secondCallArguments} ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );)";

        return replaceWithMap( callSequenceTemplate,
                               { { "counterName", startLowerCase( stripPrefix( commandData.params[vectorParamIt->second.lenParam].name, "p" ) ) },
                                 { "counterType", commandData.params[vectorParamIt->second.lenParam].type.type },
                                 { "dispatcher", dispatcher },
                                 { "firstCallArguments", firstCallArguments },
                                 { "secondCallArguments", secondCallArguments },
                                 { "vectorElementType", vectorElementType },
                                 { "vectorName", vectorName },
                                 { "vkCommand", name } } );
      }
      else
      {
        const std::string callSequenceTemplate =
          R"(${dispatcher}${vkCommand}( ${firstCallArguments} );
    structureChains.resize( ${counterName} );
    ${vectorName}.resize( ${counterName} );
    for ( ${counterType} i = 0; i < ${counterName}; i++ )
    {
      ${vectorName}[i].pNext = structureChains[i].template get<${vectorElementType}>().pNext;
    }
    ${dispatcher}${vkCommand}( ${secondCallArguments} );)";

        return replaceWithMap( callSequenceTemplate,
                               { { "counterName", startLowerCase( stripPrefix( commandData.params[vectorParamIt->second.lenParam].name, "p" ) ) },
                                 { "counterType", commandData.params[vectorParamIt->second.lenParam].type.type },
                                 { "dispatcher", dispatcher },
                                 { "firstCallArguments", firstCallArguments },
                                 { "secondCallArguments", secondCallArguments },
                                 { "vectorElementType", vectorElementType },
                                 { "vectorName", vectorName },
                                 { "vkCommand", name } } );
      }
    }
    else if ( commandData.returnType == "VkResult" )
    {
      std::string resizes;
      for ( auto const & vp : vectorParams )
      {
        assert( std::any_of( returnParams.begin(), returnParams.end(), [&vp]( size_t rp ) { return rp == vp.first; } ) &&
                std::any_of( returnParams.begin(), returnParams.end(), [&vp]( size_t rp ) { return rp == vp.second.lenParam; } ) );
        resizes += startLowerCase( stripPrefix( commandData.params[vp.first].name, "p" ) ) + ".resize( " +
                   startLowerCase( stripPrefix( commandData.params[vp.second.lenParam].name, "p" ) ) + " );\n";
      }
      resizes.pop_back();

      switch ( commandData.successCodes.size() )
      {
        case 1:
          assert( commandData.successCodes[0] == "VK_SUCCESS" );
          if ( commandData.errorCodes.empty() )
          {
            std::string const callSequenceTemplate = R"(${dispatcher}${vkCommand}( ${firstCallArguments} );
      ${resizes}
      ${dispatcher}${vkCommand}( ${secondCallArguments} );
)";

            return replaceWithMap( callSequenceTemplate,
                                   { { "dispatcher", dispatcher },
                                     { "firstCallArguments", firstCallArguments },
                                     { "secondCallArguments", secondCallArguments },
                                     { "resizes", resizes },
                                     { "vkCommand", name } } );
          }
          else
          {
            std::string const callSequenceTemplate =
              R"(VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( ${dispatcher}${vkCommand}( ${firstCallArguments} ) );
      if ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
        ${resizes}
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( ${dispatcher}${vkCommand}( ${secondCallArguments} ) );
      }
)";

            return replaceWithMap( callSequenceTemplate,
                                   { { "dispatcher", dispatcher },
                                     { "firstCallArguments", firstCallArguments },
                                     { "secondCallArguments", secondCallArguments },
                                     { "resizes", resizes },
                                     { "vkCommand", name } } );
          }
        case 2:
        case 3:
          {
            assert( ( commandData.successCodes[0] == "VK_SUCCESS" ) && ( commandData.successCodes[1] == "VK_INCOMPLETE" ) );
            std::string const callSequenceTemplate = R"(VULKAN_HPP_NAMESPACE::Result result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( ${dispatcher}${vkCommand}( ${firstCallArguments} ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && ${counterName} )
      {
        ${resizes}
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( ${dispatcher}${vkCommand}( ${secondCallArguments} ) );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );)";

            return replaceWithMap( callSequenceTemplate,
                                   { { "counterName", startLowerCase( stripPrefix( commandData.params[vectorParamIt->second.lenParam].name, "p" ) ) },
                                     { "dispatcher", dispatcher },
                                     { "firstCallArguments", firstCallArguments },
                                     { "secondCallArguments", secondCallArguments },
                                     { "resizes", resizes },
                                     { "vkCommand", name } } );
          }
        default: assert( false ); return "";
      }
    }
    else
    {
      std::string resizeInstructions;
      if ( vectorParamIt->second.byStructure )
      {
        std::string const resizeInstructionTemplate = R"(${vectorName}.resize( ${vectorSize} );
    ${structName}.${pointerName} = ${vectorName}.data();)";

        resizeInstructions = replaceWithMap( resizeInstructionTemplate,
                                             { { "pointerName", "p" + startUpperCase( vectorName ) },
                                               { "structName", startLowerCase( stripPrefix( commandData.params[vectorParamIt->first].name, "p" ) ) },
                                               { "vectorName", vectorName },
                                               { "vectorSize", vectorSize } } );
      }
      else
      {
        std::string const resizeInstructionTemplate = R"(${vectorName}.resize( ${vectorSize} );)";
        resizeInstructions = replaceWithMap( resizeInstructionTemplate, { { "vectorName", vectorName }, { "vectorSize", vectorSize } } );
      }

      // no need to enumerate here, just two calls
      assert( commandData.returnType == "void" );
      std::string const callSequenceTemplate = R"(${dispatcher}${vkCommand}( ${firstCallArguments} );
    ${resizeInstructions}
    ${dispatcher}${vkCommand}( ${secondCallArguments} );)";

      return replaceWithMap( callSequenceTemplate,
                             { { "dispatcher", dispatcher },
                               { "firstCallArguments", firstCallArguments },
                               { "resizeInstructions", resizeInstructions },
                               { "secondCallArguments", secondCallArguments },
                               { "vkCommand", name } } );
    }
  }
  else if ( ( commandData.returnType == "VkResult" ) && ( ( 1 < commandData.successCodes.size() ) || !commandData.errorCodes.empty() ) )
  {
    std::string const callSequenceTemplate =
      R"(VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( ${dispatcher}${vkCommand}( ${callArguments} ) );)";

    std::string callArguments =
      generateCallArgumentsEnhanced( commandData, initialSkipCount, false, singularParams, templatedParams, raii, raiiFactory, flavourFlags );

    return replaceWithMap( callSequenceTemplate, { { "callArguments", callArguments }, { "dispatcher", dispatcher }, { "vkCommand", name } } );
  }
  else
  {
    std::string const callSequenceTemplate = R"(${resultAssignment}${dispatcher}${vkCommand}( ${callArguments} );)";

    std::string callArguments =
      generateCallArgumentsEnhanced( commandData, initialSkipCount, false, singularParams, templatedParams, raii, raiiFactory, flavourFlags );
    std::string resultAssignment = generateResultAssignment( commandData );

    return replaceWithMap(
      callSequenceTemplate,
      { { "callArguments", callArguments }, { "dispatcher", dispatcher }, { "resultAssignment", resultAssignment }, { "vkCommand", name } } );
  }
}

std::string
  VulkanHppGenerator::generateCommand( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const
{
  std::string cmd;
  if ( commandData.returnType == "VkResult" )
  {
    cmd = generateCommandResult( name, commandData, initialSkipCount, definition, raii );
  }
  else if ( commandData.returnType == "void" )
  {
    cmd = generateCommandVoid( name, commandData, initialSkipCount, definition, raii );
  }
  else
  {
    cmd = generateCommandValue( name, commandData, initialSkipCount, definition, raii );
  }

  if ( cmd.empty() )
  {
    throw std::runtime_error( "Never encountered a function like <" + name + "> !" );
  }

  return cmd;
}

std::string VulkanHppGenerator::generateCommandDefinitions() const
{
  const std::string commandDefinitionsTemplate = R"(
  //===========================
  //=== COMMAND Definitions ===
  //===========================

${commandDefinitions}
)";

  std::string           commandDefinitions;
  std::set<std::string> listedCommands;  // some commands are listed with more than one extension!
  for ( auto const & feature : m_features )
  {
    commandDefinitions += generateCommandDefinitions( feature.requireData, listedCommands, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    commandDefinitions += generateCommandDefinitions( extension.requireData, listedCommands, extension.name );
  }

  return replaceWithMap( commandDefinitionsTemplate, { { "commandDefinitions", commandDefinitions } } );
}

std::string VulkanHppGenerator::generateCommandDefinitions( std::vector<RequireData> const & requireData,
                                                            std::set<std::string> &          listedCommands,
                                                            std::string const &              title ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & command : require.commands )
    {
      if ( listedCommands.insert( command.name ).second )
      {
        auto commandIt = findByNameOrAlias( m_commands, command.name );
        assert( commandIt != m_commands.end() );
        str += generateCommandDefinitions( command.name, commandIt->second.handle );
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateCommandDefinitions( std::string const & command, std::string const & handle ) const
{
  auto commandIt = findByNameOrAlias( m_commands, command );
  assert( commandIt != m_commands.end() );

  std::string str = "\n" + generateCommand( command, commandIt->second, handle.empty() ? 0 : 1, true, false );

  // special handling for destroy functions, filter out alias functions
  std::string commandName = generateCommandName( command, commandIt->second.params, 1 );
  if ( ( command == commandIt->first ) && ( ( ( command.substr( 2, 7 ) == "Destroy" ) && ( commandName != "destroy" ) ) ||
                                            ( command.substr( 2, 4 ) == "Free" ) || ( command == "vkReleasePerformanceConfigurationINTEL" ) ) )
  {
    CommandData specialCommandData = commandIt->second;
    assert( ( 1 < specialCommandData.params.size() ) && ( specialCommandData.params[0].type.type == handle ) );
    specialCommandData.params[1].optional = false;  // make sure, the object to destroy/free/release is not optional in the shortened version!

    std::string destroyCommandString = generateCommand( command, specialCommandData, handle.empty() ? 0 : 1, true, false );
    std::string shortenedName;
    if ( command.substr( 2, 7 ) == "Destroy" )
    {
      shortenedName = "destroy";
    }
    else if ( command.substr( 2, 4 ) == "Free" )
    {
      shortenedName = "free";
    }
    else
    {
      assert( command == "vkReleasePerformanceConfigurationINTEL" );
      shortenedName = "release";
    }
    size_t pos = destroyCommandString.find( commandName );
    while ( pos != std::string::npos )
    {
      destroyCommandString.replace( pos, commandName.length(), shortenedName );
      pos = destroyCommandString.find( commandName, pos );
    }

    // special handling for "free", to prevent interfering with MSVC debug free!
    if ( shortenedName == "free" )
    {
      std::string toEncloseString = stripPrefix( handle, "Vk" ) + "::free";
      std::string enclosedString  = "( " + toEncloseString + " )";
      pos                         = destroyCommandString.find( toEncloseString );
      while ( pos != std::string::npos )
      {
        destroyCommandString.replace( pos, toEncloseString.length(), enclosedString );
        pos = destroyCommandString.find( toEncloseString, pos + enclosedString.length() );
      }
    }

    // we need to remove the default argument for the first argument, to prevent ambiguities!
    assert( 1 < commandIt->second.params.size() );
    pos = destroyCommandString.find( commandIt->second.params[1].name );  // skip the standard version of the function
    assert( pos != std::string::npos );
    pos = destroyCommandString.find( commandIt->second.params[1].name,
                                     pos + 1 );  // get the argument to destroy in the advanced version
    assert( pos != std::string::npos );
    pos = destroyCommandString.find( " VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT", pos );
    if ( pos != std::string::npos )
    {
      destroyCommandString.erase( pos, strlen( " VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT" ) );
    }
    str += "\n" + destroyCommandString;
  }
  return str;
}

std::string VulkanHppGenerator::generateCommandEnhanced( std::string const &                       name,
                                                         CommandData const &                       commandData,
                                                         size_t                                    initialSkipCount,
                                                         bool                                      definition,
                                                         std::map<size_t, VectorParamData> const & vectorParams,
                                                         std::vector<size_t> const &               returnParams,
                                                         CommandFlavourFlags                       flavourFlags ) const
{
  const bool chained  = flavourFlags & CommandFlavourFlagBits::chained;
  const bool singular = flavourFlags & CommandFlavourFlagBits::singular;

  assert( vectorParams.empty() || ( vectorParams.begin()->second.lenParam != INVALID_INDEX ) );
  assert( !singular || !returnParams.empty() );  // if singular is true, then there is at least one returnParam !

  std::set<size_t> skippedParams = determineSkippedParams( commandData.params, initialSkipCount, vectorParams, returnParams, singular );
  std::set<size_t> templatedParams =
    ( flavourFlags & CommandFlavourFlagBits::keepVoidPtr ) ? std::set<size_t>() : determineVoidPointerParams( commandData.params );
  std::vector<size_t> chainedReturnParams;
  if ( chained )
  {
    chainedReturnParams = determineChainedReturnParams( commandData.params, returnParams );
    assert( chainedReturnParams.size() == 1 );
  }
  std::set<size_t> singularParams = singular ? determineSingularParams( returnParams[0], vectorParams ) : std::set<size_t>();
  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck =
    needsVectorSizeCheck( commandData.params, vectorParams, returnParams, singularParams, skippedParams );
  const bool enumerating = determineEnumeration( vectorParams, returnParams );

  std::vector<std::string> dataTypes = determineDataTypes( commandData.params, vectorParams, returnParams, templatedParams );
  std::string              dataType  = combineDataTypes( vectorParams, returnParams, enumerating, dataTypes, flavourFlags, false );

  std::string argumentTemplates = generateArgumentTemplates( commandData.params, returnParams, vectorParams, templatedParams, chainedReturnParams, false );
  auto [allocatorTemplates, uniqueHandleAllocatorTemplates] =
    generateAllocatorTemplates( returnParams, dataTypes, vectorParams, chainedReturnParams, flavourFlags, definition );
  std::string typenameCheck       = generateTypenameCheck( returnParams, vectorParams, chainedReturnParams, definition, dataTypes, flavourFlags );
  std::string nodiscard           = generateNoDiscard( !returnParams.empty(), 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string returnType          = generateReturnType( returnParams, vectorParams, flavourFlags, false, dataTypes );
  std::string decoratedReturnType = generateDecoratedReturnType( commandData, returnParams, vectorParams, flavourFlags, false, returnType );
  std::string className           = initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "";
  std::string classSeparator      = commandData.handle.empty() ? "" : "::";
  std::string commandName         = generateCommandName( name, commandData.params, initialSkipCount, flavourFlags );
  std::string argumentList        = generateArgumentListEnhanced(
    commandData.params, returnParams, vectorParams, skippedParams, singularParams, templatedParams, chainedReturnParams, definition, flavourFlags, true );
  std::string constString    = commandData.handle.empty() ? "" : " const";
  std::string noexceptString = generateNoExcept( commandData.errorCodes, returnParams, vectorParams, flavourFlags, vectorSizeCheck.first, false );

  if ( definition )
  {
    std::string vectorSizeCheckString =
      vectorSizeCheck.first ? generateVectorSizeCheck( name, commandData, initialSkipCount, vectorSizeCheck.second, skippedParams, false ) : "";
    std::string returnVariable   = generateReturnVariable( commandData, returnParams, vectorParams, flavourFlags );
    std::string dataDeclarations = generateDataDeclarations(
      commandData, returnParams, vectorParams, templatedParams, flavourFlags, false, dataTypes, dataType, returnType, returnVariable );
    std::string dataPreparation =
      generateDataPreparation( commandData, initialSkipCount, returnParams, vectorParams, templatedParams, flavourFlags, enumerating, dataTypes );
    std::string dataSizeChecks = generateDataSizeChecks( commandData, returnParams, dataTypes, vectorParams, templatedParams, singular );
    std::string callSequence   = generateCallSequence(
      name, commandData, returnParams, vectorParams, initialSkipCount, singularParams, templatedParams, chainedReturnParams, flavourFlags, false, false );
    std::string resultCheck     = generateResultCheck( commandData, className, classSeparator, commandName, enumerating );
    std::string returnStatement = generateReturnStatement( name,
                                                           commandData,
                                                           returnVariable,
                                                           returnType,
                                                           decoratedReturnType,
                                                           dataType,
                                                           initialSkipCount,
                                                           returnParams.empty() ? INVALID_INDEX : returnParams[0],
                                                           flavourFlags,
                                                           enumerating,
                                                           false );

    std::string const functionTemplate =
      R"(  template <${argumentTemplates}${allocatorTemplates}typename Dispatch${uniqueHandleAllocatorTemplates}${typenameCheck}>
  ${nodiscard}VULKAN_HPP_INLINE ${decoratedReturnType} ${className}${classSeparator}${commandName}( ${argumentList} )${const}${noexcept}
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
${functionPointerCheck}
#endif
${vectorSizeCheck}
    ${dataSizeChecks}
    ${dataDeclarations}
    ${callSequence}
    ${resultCheck}
    ${dataPreparation}
    ${returnStatement}
  })";

    return replaceWithMap( functionTemplate,
                           { { "allocatorTemplates", allocatorTemplates },
                             { "argumentList", argumentList },
                             { "argumentTemplates", argumentTemplates },
                             { "callSequence", callSequence },
                             { "className", className },
                             { "classSeparator", classSeparator },
                             { "commandName", commandName },
                             { "const", constString },
                             { "dataDeclarations", dataDeclarations },
                             { "dataPreparation", dataPreparation },
                             { "dataSizeChecks", dataSizeChecks },
                             { "decoratedReturnType", decoratedReturnType },
                             { "functionPointerCheck", generateFunctionPointerCheck( name, commandData.requiredBy, false ) },
                             { "nodiscard", nodiscard },
                             { "noexcept", noexceptString },
                             { "resultCheck", resultCheck },
                             { "returnStatement", returnStatement },
                             { "typenameCheck", typenameCheck },
                             { "uniqueHandleAllocatorTemplates", uniqueHandleAllocatorTemplates },
                             { "vectorSizeCheck", vectorSizeCheckString } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <${argumentTemplates}${allocatorTemplates}typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE${uniqueHandleAllocatorTemplates}${typenameCheck}>
    ${nodiscard}${decoratedReturnType} ${commandName}( ${argumentList} )${const}${noexcept};)";

    return replaceWithMap( functionTemplate,
                           { { "allocatorTemplates", allocatorTemplates },
                             { "argumentList", argumentList },
                             { "argumentTemplates", argumentTemplates },
                             { "commandName", commandName },
                             { "const", commandData.handle.empty() ? "" : " const" },
                             { "decoratedReturnType", decoratedReturnType },
                             { "nodiscard", nodiscard },
                             { "noexcept", noexceptString },
                             { "typenameCheck", typenameCheck },
                             { "uniqueHandleAllocatorTemplates", uniqueHandleAllocatorTemplates } } );
  }
}

std::string VulkanHppGenerator::generateCommandName( std::string const &            vulkanCommandName,
                                                     std::vector<ParamData> const & params,
                                                     size_t                         initialSkipCount,
                                                     CommandFlavourFlags            flavourFlags ) const
{
  std::string commandName( startLowerCase( stripPrefix( vulkanCommandName, "vk" ) ) );
  for ( size_t i = initialSkipCount - 1; i < initialSkipCount; --i )  // count down to zero, then wrap around and stop
  {
    std::string const & argumentType = params[i].type.type;
    std::string         searchName   = stripPrefix( argumentType, "Vk" );
    std::string         argumentTag  = findTag( argumentType );
    if ( !argumentTag.empty() )
    {
      searchName = stripPostfix( searchName, argumentTag );
    }
    size_t pos = commandName.find( searchName );
    if ( pos == std::string::npos )
    {
      searchName = startLowerCase( searchName );
      pos        = commandName.find( searchName );
    }
    if ( pos != std::string::npos )
    {
      size_t len = searchName.length();
      if ( commandName.find( searchName + "s" ) == pos )
      {
        // filter out any plural of the searchName as well!
        ++len;
      }
      commandName.erase( pos, len );
    }
    else if ( ( searchName == "commandBuffer" ) && commandName.starts_with( "cmd" ) )
    {
      commandName.erase( 0, 3 );
      pos = 0;
    }
    if ( pos == 0 )
    {
      commandName = startLowerCase( commandName );
    }
    std::string commandTag = findTag( commandName );
    if ( !argumentTag.empty() && ( argumentTag == commandTag ) )
    {
      commandName = stripPostfix( commandName, argumentTag );
    }
  }
  if ( flavourFlags & CommandFlavourFlagBits::singular )
  {
    commandName = stripPluralS( commandName );
  }
  if ( flavourFlags & CommandFlavourFlagBits::unique )
  {
    commandName += "Unique";
  }
  return commandName;
}

std::string VulkanHppGenerator::generateCommandResult(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const
{
  assert( !commandData.successCodes.empty() );
  if ( commandData.successCodes.size() == 1 )
  {
    return generateCommandResultSingleSuccess( name, commandData, initialSkipCount, definition, raii );
  }
  else if ( commandData.errorCodes.empty() )
  {
    return generateCommandResultMultiSuccessNoErrors( name, commandData, initialSkipCount, definition, raii );
  }
  else
  {
    return generateCommandResultMultiSuccessWithErrors( name, commandData, initialSkipCount, definition, raii );
  }
}

std::string VulkanHppGenerator::generateCommandResultMultiSuccessNoErrors(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandData.params );
  switch ( returnParams.size() )
  {
    case 0:
      {
        std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
        if ( vectorParams.empty() )
        {
          std::vector<size_t> constPointerParams = determineConstPointerParams( commandData.params );
          if ( constPointerParams.empty() )
          {
            return generateCommandSetExclusive( name, commandData, initialSkipCount, definition, raii );
          }
        }
      }
      break;
    case 2:
      if ( ( commandData.successCodes.size() == 2 ) && ( commandData.successCodes[0] == "VK_SUCCESS" ) && ( commandData.successCodes[1] == "VK_INCOMPLETE" ) )
      {
        if ( ( commandData.params[returnParams[0]].type.type == "size_t" ) || ( commandData.params[returnParams[0]].type.type == "uint32_t" ) )
        {
          if ( ( commandData.params[returnParams[1]].type.type != "void" ) && !isHandleType( commandData.params[returnParams[1]].type.type ) &&
               !isStructureChainAnchor( commandData.params[returnParams[1]].type.type ) )
          {
            std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
            if ( vectorParams.size() == 1 )
            {
              if ( returnParams[0] == vectorParams.begin()->second.lenParam )
              {
                if ( returnParams[1] == vectorParams.begin()->first )
                {
                  return generateCommandSetInclusive( name,
                                                      commandData,
                                                      initialSkipCount,
                                                      definition,
                                                      returnParams,
                                                      vectorParams,
                                                      false,
                                                      { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator },
                                                      raii,
                                                      false,
                                                      { CommandFlavourFlagBits::enhanced } );
                }
              }
            }
          }
        }
      }
      break;
    default: break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultMultiSuccessWithErrors(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandData.params );
  switch ( returnParams.size() )
  {
    case 0: return generateCommandResultWithErrors0Return( name, commandData, initialSkipCount, definition, raii );
    case 1: return generateCommandResultMultiSuccessWithErrors1Return( name, commandData, initialSkipCount, definition, returnParams[0], raii );
    case 2: return generateCommandResultMultiSuccessWithErrors2Return( name, commandData, initialSkipCount, definition, returnParams, raii );
    case 3: return generateCommandResultMultiSuccessWithErrors3Return( name, commandData, initialSkipCount, definition, returnParams, raii );
    default: break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultMultiSuccessWithErrors1Return(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const
{
  if ( commandData.params[returnParam].type.type == "void" )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
    if ( vectorParams.size() == 1 )
    {
      if ( returnParam == vectorParams.begin()->first )
      {
        if ( commandData.params[vectorParams.begin()->second.lenParam].type.isValue() )
        {
          return generateCommandSetInclusive( name,
                                              commandData,
                                              initialSkipCount,
                                              definition,
                                              { returnParam },
                                              vectorParams,
                                              false,
                                              { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::singular },
                                              raii,
                                              false,
                                              { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::singular } );
        }
      }
    }
  }
  else if ( isHandleType( commandData.params[returnParam].type.type ) )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
    if ( vectorParams.size() == 2 )
    {
      if ( returnParam == std::next( vectorParams.begin() )->first )
      {
        if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
        {
          if ( commandData.params[vectorParams.begin()->second.lenParam].type.type == "uint32_t" )
          {
            if ( ( ( commandData.params[vectorParams.begin()->first].type.type != "void" ) &&
                   !isHandleType( commandData.params[vectorParams.begin()->first].type.type ) ) )
            {
              return generateCommandSetInclusive( name,
                                                  commandData,
                                                  initialSkipCount,
                                                  definition,
                                                  { returnParam },
                                                  vectorParams,
                                                  true,
                                                  { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator, CommandFlavourFlagBits::singular },
                                                  raii,
                                                  true,
                                                  { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::singular } );
            }
          }
        }
      }
    }
  }
  else if ( isHandleTypeByStructure( commandData.params[returnParam].type.type ) )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
    if ( vectorParams.size() == 1 )
    {
      return generateCommandSetInclusive( name,
                                          commandData,
                                          initialSkipCount,
                                          definition,
                                          { returnParam },
                                          vectorParams,
                                          true,
                                          { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator },
                                          raii,
                                          true,
                                          { CommandFlavourFlagBits::enhanced } );
    }
  }
  else if ( isVectorByStructure( commandData.params[returnParam].type.type ) )
  {
    // vector by structure is too complex to be supported! Just generate the standard version
    return generateCommandSetInclusive( name, commandData, initialSkipCount, definition, { returnParam }, {}, false, {}, raii, false, {} );
  }
  else
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
    if ( vectorParams.empty() )
    {
      return generateCommandSetInclusive( name,
                                          commandData,
                                          initialSkipCount,
                                          definition,
                                          { returnParam },
                                          vectorParams,
                                          false,
                                          { CommandFlavourFlagBits::enhanced },
                                          raii,
                                          false,
                                          { CommandFlavourFlagBits::enhanced } );
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultMultiSuccessWithErrors2Return( std::string const &         name,
                                                                                    CommandData const &         commandData,
                                                                                    size_t                      initialSkipCount,
                                                                                    bool                        definition,
                                                                                    std::vector<size_t> const & returnParams,
                                                                                    bool                        raii ) const
{
  if ( ( 2 <= commandData.successCodes.size() ) && ( commandData.successCodes[0] == "VK_SUCCESS" ) && ( commandData.successCodes[1] == "VK_INCOMPLETE" ) )
  {
    if ( ( commandData.params[returnParams[0]].type.type == "size_t" ) || ( commandData.params[returnParams[0]].type.type == "uint32_t" ) )
    {
      // needs some very special handling of "vkGetSwapchainImagesKHR" !!
      if ( isHandleType( commandData.params[returnParams[1]].type.type ) && ( name != "vkGetSwapchainImagesKHR" ) )
      {
        std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
        if ( vectorParams.size() == 1 )
        {
          if ( returnParams[0] == vectorParams.begin()->second.lenParam )
          {
            if ( returnParams[1] == vectorParams.begin()->first )
            {
              return generateCommandSetInclusive( name,
                                                  commandData,
                                                  initialSkipCount,
                                                  definition,
                                                  returnParams,
                                                  vectorParams,
                                                  false,
                                                  { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator },
                                                  raii,
                                                  true,
                                                  { CommandFlavourFlagBits::enhanced } );
            }
          }
        }
      }
      else if ( isStructureChainAnchor( commandData.params[returnParams[1]].type.type ) )
      {
        std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
        if ( vectorParams.size() == 1 )
        {
          if ( returnParams[0] == vectorParams.begin()->second.lenParam )
          {
            if ( returnParams[1] == vectorParams.begin()->first )
            {
              return generateCommandSetInclusive( name,
                                                  commandData,
                                                  initialSkipCount,
                                                  definition,
                                                  returnParams,
                                                  vectorParams,
                                                  false,
                                                  { CommandFlavourFlagBits::enhanced,
                                                    CommandFlavourFlagBits::withAllocator,
                                                    CommandFlavourFlagBits::chained,
                                                    CommandFlavourFlagBits::chained | CommandFlavourFlagBits::withAllocator },
                                                  raii,
                                                  false,
                                                  { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::chained } );
            }
          }
        }
      }
      else
      {
        std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
        if ( vectorParams.size() == 1 )
        {
          if ( returnParams[0] == vectorParams.begin()->second.lenParam )
          {
            if ( returnParams[1] == vectorParams.begin()->first )
            {
              return generateCommandSetInclusive( name,
                                                  commandData,
                                                  initialSkipCount,
                                                  definition,
                                                  returnParams,
                                                  vectorParams,
                                                  false,
                                                  { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator },
                                                  raii,
                                                  false,
                                                  { CommandFlavourFlagBits::enhanced } );
            }
          }
        }
      }
    }
    else if ( isStructureType( commandData.params[returnParams[0]].type.type ) )
    {
      // can't generate an enhanced version for such a complex command! Just use the standard version
      return generateCommandStandard( name, commandData, initialSkipCount, definition );
    }
  }

  return "";
}

std::string VulkanHppGenerator::generateCommandResultMultiSuccessWithErrors3Return( std::string const &         name,
                                                                                    CommandData const &         commandData,
                                                                                    size_t                      initialSkipCount,
                                                                                    bool                        definition,
                                                                                    std::vector<size_t> const & returnParams,
                                                                                    bool                        raii ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  switch ( vectorParams.size() )
  {
    case 1:
      if ( ( commandData.params[returnParams[0]].type.type != "void" ) && !isHandleType( commandData.params[returnParams[0]].type.type ) )
      {
        if ( ( commandData.params[returnParams[1]].type.type == "size_t" ) || ( commandData.params[returnParams[1]].type.type == "uint32_t" ) )
        {
          if ( ( commandData.params[returnParams[2]].type.type == "void" ) ||
               ( !isHandleType( commandData.params[returnParams[2]].type.type ) && !isStructureChainAnchor( commandData.params[returnParams[2]].type.type ) ) )
          {
            if ( returnParams[1] == vectorParams.begin()->second.lenParam )
            {
              if ( returnParams[2] == vectorParams.begin()->first )
              {
                std::vector<CommandFlavourFlags> flags     = { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator };
                std::vector<CommandFlavourFlags> raiiFlags = { CommandFlavourFlagBits::enhanced };
                if ( isStructureChainAnchor( commandData.params[returnParams[0]].type.type ) )
                {
                  flags.push_back( CommandFlavourFlagBits::chained );
                  flags.push_back( CommandFlavourFlagBits::chained | CommandFlavourFlagBits::withAllocator );
                  raiiFlags.push_back( CommandFlavourFlagBits::chained );
                }
                return generateCommandSetInclusive(
                  name, commandData, initialSkipCount, definition, returnParams, vectorParams, false, flags, raii, false, raiiFlags );
              }
            }
          }
        }
      }
      break;
    case 2:
      if ( commandData.params[returnParams[0]].type.type == "uint32_t" )
      {
        if ( isStructureType( commandData.params[returnParams[1]].type.type ) && !isStructureChainAnchor( commandData.params[returnParams[1]].type.type ) )
        {
          if ( isStructureType( commandData.params[returnParams[2]].type.type ) && !isStructureChainAnchor( commandData.params[returnParams[2]].type.type ) )
          {
            if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
            {
              if ( returnParams[0] == vectorParams.begin()->second.lenParam )
              {
                if ( returnParams[1] == vectorParams.begin()->first )
                {
                  if ( returnParams[2] == std::next( vectorParams.begin() )->first )
                  {
                    return generateCommandSetInclusive( name,
                                                        commandData,
                                                        initialSkipCount,
                                                        definition,
                                                        returnParams,
                                                        vectorParams,
                                                        false,
                                                        { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator },
                                                        raii,
                                                        false,
                                                        { CommandFlavourFlagBits::enhanced } );
                  }
                }
              }
            }
          }
        }
      }
      break;
    default: assert( false ); break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccess(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const
{
  if ( commandData.errorCodes.empty() )
  {
    return generateCommandResultSingleSuccessNoErrors( name, commandData, initialSkipCount, definition, raii );
  }
  else
  {
    return generateCommandResultSingleSuccessWithErrors( name, commandData, initialSkipCount, definition, raii );
  }
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessNoErrors(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandData.params );
  switch ( returnParams.size() )
  {
    case 0:
      {
        std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
        switch ( vectorParams.size() )
        {
          case 0:
            if ( determineConstPointerParams( commandData.params ).empty() )
            {
              return generateCommandSetExclusive( name, commandData, initialSkipCount, definition, raii );
            }
            else
            {
              return generateCommandSetInclusive( name,
                                                  commandData,
                                                  initialSkipCount,
                                                  definition,
                                                  returnParams,
                                                  vectorParams,
                                                  false,
                                                  { CommandFlavourFlagBits::enhanced },
                                                  raii,
                                                  false,
                                                  { CommandFlavourFlagBits::enhanced } );
            }
            break;
          case 1:
            if ( !raii )
            {
              if ( commandData.params[vectorParams.begin()->second.lenParam].type.isValue() )
              {
                if ( isHandleType( commandData.params[vectorParams.begin()->first].type.type ) )
                {
                  return generateCommandSet( definition,
                                             generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                             { generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, {}, {} ) } );
                }
              }
            }
            break;
          default: break;
        }
      }
      break;
    case 1:
      {
        std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
        if ( vectorParams.empty() )
        {
          if ( isStructureType( commandData.params[returnParams[0]].type.type ) && !isStructureChainAnchor( commandData.params[returnParams[0]].type.type ) )
          {
            return generateCommandSetInclusive( name,
                                                commandData,
                                                initialSkipCount,
                                                definition,
                                                returnParams,
                                                vectorParams,
                                                false,
                                                { CommandFlavourFlagBits::enhanced },
                                                raii,
                                                false,
                                                { CommandFlavourFlagBits::enhanced } );
          }
        }
      }
      break;
    case 2:
      if ( ( commandData.params[returnParams[0]].type.type == "size_t" ) || ( commandData.params[returnParams[0]].type.type == "uint32_t" ) )
      {
        if ( isStructureType( commandData.params[returnParams[1]].type.type ) && !isStructureChainAnchor( commandData.params[returnParams[1]].type.type ) )
        {
          std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
          if ( vectorParams.size() == 1 )
          {
            if ( returnParams[0] == vectorParams.begin()->second.lenParam )
            {
              if ( returnParams[1] == vectorParams.begin()->first )
              {
                return generateCommandSetInclusive( name,
                                                    commandData,
                                                    initialSkipCount,
                                                    definition,
                                                    returnParams,
                                                    vectorParams,
                                                    false,
                                                    { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator },
                                                    raii,
                                                    false,
                                                    { CommandFlavourFlagBits::enhanced } );
              }
            }
          }
        }
      }
      break;
    default: break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandData.params );
  switch ( returnParams.size() )
  {
    case 0: return generateCommandResultWithErrors0Return( name, commandData, initialSkipCount, definition, raii );
    case 1: return generateCommandResultSingleSuccessWithErrors1Return( name, commandData, initialSkipCount, definition, returnParams[0], raii );
    case 2: return generateCommandResultSingleSuccessWithErrors2Return( name, commandData, initialSkipCount, definition, returnParams, raii );
    case 3: return generateCommandResultSingleSuccessWithErrors3Return( name, commandData, initialSkipCount, definition, returnParams, raii );
    default: break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors1Return(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const
{
  if ( commandData.params[returnParam].type.type == "void" )
  {
    return generateCommandResultSingleSuccessWithErrors1ReturnVoid( name, commandData, initialSkipCount, definition, returnParam, raii );
  }
  else if ( isHandleType( commandData.params[returnParam].type.type ) )
  {
    return generateCommandResultSingleSuccessWithErrors1ReturnHandle( name, commandData, initialSkipCount, definition, returnParam, raii );
  }
  else if ( isStructureChainAnchor( commandData.params[returnParam].type.type ) )
  {
    return generateCommandResultSingleSuccessWithErrors1ReturnChain( name, commandData, initialSkipCount, definition, returnParam, raii );
  }
  else
  {
    return generateCommandResultSingleSuccessWithErrors1ReturnValue( name, commandData, initialSkipCount, definition, returnParam, raii );
  }
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors1ReturnChain(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  if ( vectorParams.empty() )
  {
    return generateCommandSetInclusive( name,
                                        commandData,
                                        initialSkipCount,
                                        definition,
                                        { returnParam },
                                        vectorParams,
                                        false,
                                        { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::chained },
                                        raii,
                                        false,
                                        { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::chained } );
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors1ReturnHandle(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  switch ( vectorParams.size() )
  {
    case 0:
      return generateCommandSetInclusive( name,
                                          commandData,
                                          initialSkipCount,
                                          definition,
                                          { returnParam },
                                          vectorParams,
                                          true,
                                          { CommandFlavourFlagBits::enhanced },
                                          raii,
                                          true,
                                          { CommandFlavourFlagBits::enhanced } );
    case 1:
      if ( returnParam == vectorParams.begin()->first )
      {
        if ( isLenByStructMember( commandData.params[vectorParams.begin()->first].lenExpression, commandData.params[vectorParams.begin()->second.lenParam] ) )
        {
          return generateCommandSetInclusive( name,
                                              commandData,
                                              initialSkipCount,
                                              definition,
                                              { returnParam },
                                              vectorParams,
                                              true,
                                              { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator },
                                              raii,
                                              true,
                                              { CommandFlavourFlagBits::enhanced } );
        }
      }
      break;
    case 2:
      if ( returnParam == std::next( vectorParams.begin() )->first )
      {
        if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
        {
          if ( commandData.params[vectorParams.begin()->second.lenParam].type.isValue() )
          {
            if ( ( commandData.params[vectorParams.begin()->first].type.type != "void" ) &&
                 !isHandleType( commandData.params[vectorParams.begin()->first].type.type ) )
            {
              return generateCommandSetInclusive( name,
                                                  commandData,
                                                  initialSkipCount,
                                                  definition,
                                                  { returnParam },
                                                  vectorParams,
                                                  true,
                                                  { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator, CommandFlavourFlagBits::singular },
                                                  raii,
                                                  true,
                                                  { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::singular } );
            }
          }
        }
      }
      break;
    default: break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors1ReturnValue(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  switch ( vectorParams.size() )
  {
    case 0:
      {
        CommandFlavourFlags flavourFlags = CommandFlavourFlagBits::enhanced;
        if ( name == "vkGetMemoryHostPointerPropertiesEXT" )
        {
          // special handling for this function: need to keep the void* argument as a void*!
          flavourFlags |= CommandFlavourFlagBits::keepVoidPtr;
        }
        return generateCommandSetInclusive(
          name, commandData, initialSkipCount, definition, { returnParam }, vectorParams, false, { flavourFlags }, raii, false, { flavourFlags } );
      }
    case 2:
      if ( returnParam == std::next( vectorParams.begin() )->first )
      {
        if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
        {
          if ( commandData.params[vectorParams.begin()->second.lenParam].type.type == "uint32_t" )
          {
            if ( isStructureType( commandData.params[vectorParams.begin()->first].type.type ) &&
                 !isStructureChainAnchor( commandData.params[vectorParams.begin()->first].type.type ) )
            {
              return generateCommandSetInclusive( name,
                                                  commandData,
                                                  initialSkipCount,
                                                  definition,
                                                  { returnParam },
                                                  vectorParams,
                                                  false,
                                                  { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator, CommandFlavourFlagBits::singular },
                                                  raii,
                                                  false,
                                                  { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::singular } );
            }
          }
        }
      }
      break;
    default: break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors1ReturnVoid(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  switch ( vectorParams.size() )
  {
    case 0:
      return generateCommandSetInclusive( name,
                                          commandData,
                                          initialSkipCount,
                                          definition,
                                          { returnParam },
                                          vectorParams,
                                          false,
                                          { CommandFlavourFlagBits::enhanced },
                                          raii,
                                          false,
                                          { CommandFlavourFlagBits::enhanced } );
    case 1:
      if ( returnParam == vectorParams.begin()->first )
      {
        if ( commandData.params[vectorParams.begin()->second.lenParam].type.isValue() )
        {
          return generateCommandSetInclusive( name,
                                              commandData,
                                              initialSkipCount,
                                              definition,
                                              { returnParam },
                                              vectorParams,
                                              false,
                                              { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::singular },
                                              raii,
                                              false,
                                              { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::singular } );
        }
      }
      break;
    case 2:
      if ( returnParam == std::next( vectorParams.begin() )->first )
      {
        if ( vectorParams.begin()->second.lenParam != std::next( vectorParams.begin() )->second.lenParam )
        {
          if ( commandData.params[vectorParams.begin()->second.lenParam].type.isValue() )
          {
            if ( isHandleType( commandData.params[vectorParams.begin()->first].type.type ) )
            {
              if ( commandData.params[std::next( vectorParams.begin() )->second.lenParam].type.isValue() )
              {
                return generateCommandSetInclusive( name,
                                                    commandData,
                                                    initialSkipCount,
                                                    definition,
                                                    { returnParam },
                                                    vectorParams,
                                                    false,
                                                    { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::singular },
                                                    raii,
                                                    false,
                                                    { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::singular } );
              }
            }
          }
        }
      }
      break;
    default: break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors2Return( std::string const &         name,
                                                                                     CommandData const &         commandData,
                                                                                     size_t                      initialSkipCount,
                                                                                     bool                        definition,
                                                                                     std::vector<size_t> const & returnParams,
                                                                                     bool                        raii ) const
{
  if ( ( commandData.params[returnParams[0]].type.type != "void" ) && !isHandleType( commandData.params[returnParams[0]].type.type ) &&
       !isStructureChainAnchor( commandData.params[returnParams[0]].type.type ) )
  {
    if ( ( commandData.params[returnParams[1]].type.type != "void" ) && !isHandleType( commandData.params[returnParams[1]].type.type ) &&
         !isStructureChainAnchor( commandData.params[returnParams[1]].type.type ) )
    {
      std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
      if ( vectorParams.size() == 2 )
      {
        if ( returnParams[0] == std::next( vectorParams.begin() )->first )
        {
          if ( !vectorParams.contains( returnParams[1] ) )
          {
            assert( ( returnParams[1] != vectorParams.begin()->second.lenParam ) && ( returnParams[1] != std::next( vectorParams.begin() )->second.lenParam ) );
            if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
            {
              if ( commandData.params[vectorParams.begin()->second.lenParam].type.isValue() )
              {
                if ( isStructureType( commandData.params[vectorParams.begin()->first].type.type ) &&
                     !isStructureChainAnchor( commandData.params[vectorParams.begin()->first].type.type ) )
                {
                  return generateCommandSetInclusive(
                    name,
                    commandData,
                    initialSkipCount,
                    definition,
                    returnParams,
                    vectorParams,
                    false,
                    { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator, CommandFlavourFlagBits::singular },
                    raii,
                    false,
                    { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::singular } );
                }
              }
            }
          }
        }
      }
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors3Return( std::string const &         name,
                                                                                     CommandData const &         commandData,
                                                                                     size_t                      initialSkipCount,
                                                                                     bool                        definition,
                                                                                     std::vector<size_t> const & returnParams,
                                                                                     bool                        raii ) const
{
  if ( isStructureType( commandData.params[returnParams[0]].type.type ) && !isStructureChainAnchor( commandData.params[returnParams[0]].type.type ) &&
       commandData.params[returnParams[0]].lenParams.empty() && ( commandData.params[returnParams[1]].type.type == "size_t" ) &&
       commandData.params[returnParams[1]].lenParams.empty() && ( commandData.params[returnParams[2]].type.type == "void" ) &&
       ( commandData.params[returnParams[2]].lenExpression == commandData.params[returnParams[1]].name ) )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
    if ( vectorParams.size() == 1 )
    {
      if ( ( returnParams[2] == vectorParams.begin()->first ) && ( returnParams[1] == vectorParams.begin()->second.lenParam ) )
      {
        return generateCommandSetInclusive( name,
                                            commandData,
                                            initialSkipCount,
                                            definition,
                                            returnParams,
                                            vectorParams,
                                            false,
                                            { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator },
                                            raii,
                                            false,
                                            { CommandFlavourFlagBits::enhanced } );
      }
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultWithErrors0Return(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  if ( vectorParams.empty() && determineConstPointerParams( commandData.params ).empty() )
  {
    return generateCommandSetExclusive( name, commandData, initialSkipCount, definition, raii );
  }
  else if ( allVectorSizesSupported( commandData.params, vectorParams ) )
  {
    return generateCommandSetInclusive( name,
                                        commandData,
                                        initialSkipCount,
                                        definition,
                                        {},
                                        vectorParams,
                                        false,
                                        { CommandFlavourFlagBits::enhanced },
                                        raii,
                                        false,
                                        { CommandFlavourFlagBits::enhanced } );
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandSet( bool                             definition,
                                                    std::string const &              standard,
                                                    std::vector<std::string> const & enhanced,
                                                    std::vector<std::string> const & unique ) const
{
  assert( unique.empty() || ( enhanced.size() == unique.size() ) );

  std::string commandSet = "\n" + standard;
  if ( !enhanced.empty() )
  {
    std::string separator = definition ? "\n" : "";
    commandSet += separator + "\n#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE\n";
    bool firstEnhanced = true;
    for ( auto const & e : enhanced )
    {
      if ( !firstEnhanced )
      {
        commandSet += separator + "\n";
      }
      firstEnhanced = false;
      commandSet += e;
    }
    if ( !unique.empty() )
    {
      commandSet += separator + "\n#  ifndef VULKAN_HPP_NO_SMART_HANDLE\n";
      bool firstUnique = true;
      for ( auto const & u : unique )
      {
        if ( !firstUnique )
        {
          commandSet += separator + "\n";
        }
        firstUnique = false;
        commandSet += u;
      }
      commandSet += "\n#  endif /* VULKAN_HPP_NO_SMART_HANDLE */";
    }
    commandSet += "\n#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */";
  }
  commandSet += "\n";
  return commandSet;
}

std::string VulkanHppGenerator::generateCommandSet( std::string const & standard, std::string const & enhanced ) const
{
  const std::string commandTemplate = R"(
#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandStandard}
#else
${commandEnhanced}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

  return replaceWithMap( commandTemplate, std::map<std::string, std::string>( { { "commandEnhanced", enhanced }, { "commandStandard", standard } } ) );
}

std::string VulkanHppGenerator::generateCommandSetExclusive(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const
{
  if ( raii )
  {
    return generateRAIIHandleCommandEnhanced( name, commandData, initialSkipCount, {}, {}, definition );
  }
  else
  {
    return generateCommandSet( generateCommandStandard( name, commandData, initialSkipCount, definition ),
                               generateCommandEnhanced( name, commandData, initialSkipCount, definition, {}, {}, {} ) );
  }
}

std::string VulkanHppGenerator::generateCommandSetInclusive( std::string const &                      name,
                                                             CommandData const &                      commandData,
                                                             size_t                                   initialSkipCount,
                                                             bool                                     definition,
                                                             std::vector<size_t>                      returnParams,
                                                             std::map<size_t, VectorParamData>        vectorParams,
                                                             bool                                     unique,
                                                             std::vector<CommandFlavourFlags> const & flags,
                                                             bool                                     raii,
                                                             bool                                     raiiFactory,
                                                             std::vector<CommandFlavourFlags> const & raiiFlags ) const
{
  static const std::vector<size_t>               emptyReturnParams;
  static const std::map<size_t, VectorParamData> emptyVectorParams;
  if ( raii )
  {
    std::string raiiCommands;
    if ( raiiFlags.empty() )
    {
      // Functions without enhancements need to have a standard implementation
      raiiCommands = generateRAIIHandleCommandStandard( name, commandData, initialSkipCount, definition );
    }
    else
    {
      for ( auto flag : raiiFlags )
      {
        const bool noReturn = flag & CommandFlavourFlagBits::noReturn;
        assert( !noReturn || !raiiFactory );  // noReturn => !raiiFactory
        raiiCommands += raiiFactory ? generateRAIIHandleCommandFactory( name, commandData, initialSkipCount, returnParams, vectorParams, definition, flag )
                                    : generateRAIIHandleCommandEnhanced( name,
                                                                         commandData,
                                                                         initialSkipCount,
                                                                         noReturn ? emptyReturnParams : returnParams,
                                                                         noReturn ? emptyVectorParams : vectorParams,
                                                                         definition,
                                                                         flag );
      }
    }
    return raiiCommands;
  }
  else
  {
    std::vector<std::string> enhancedCommands, uniqueCommands;
    for ( auto flag : flags )
    {
      const bool noReturn = flag & CommandFlavourFlagBits::noReturn;
      assert( !noReturn || !unique );  // noReturn => !unique
      enhancedCommands.push_back( generateCommandEnhanced(
        name, commandData, initialSkipCount, definition, noReturn ? emptyVectorParams : vectorParams, noReturn ? emptyReturnParams : returnParams, flag ) );
      if ( unique )
      {
        uniqueCommands.push_back(
          generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams, flag | CommandFlavourFlagBits::unique ) );
      }
    }
    return generateCommandSet( definition, generateCommandStandard( name, commandData, initialSkipCount, definition ), enhancedCommands, uniqueCommands );
  }
}

std::string
  VulkanHppGenerator::generateCommandStandard( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const
{
  std::set<size_t> skippedParams = determineSkippedParams( commandData.params, initialSkipCount, {}, {}, false );

  std::string argumentList = generateArgumentListStandard( commandData.params, skippedParams, definition, true );
  std::string commandName  = generateCommandName( name, commandData.params, initialSkipCount );
  std::string nodiscard    = ( 1 < commandData.successCodes.size() + commandData.errorCodes.size() ) ? "VULKAN_HPP_NODISCARD " : "";
  std::string returnType   = stripPrefix( commandData.returnType, "Vk" );

  if ( definition )
  {
    std::string functionBody = "d." + name + "( " + generateCallArgumentsStandard( commandData.params, initialSkipCount ) + " )";
    if ( commandData.returnType.starts_with( "Vk" ) )
    {
      functionBody = "return static_cast<" + returnType + ">( " + functionBody + " )";
    }
    else if ( commandData.returnType != "void" )
    {
      functionBody = "return " + functionBody;
    }

    std::string const functionTemplate =
      R"(  template <typename Dispatch>
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} )${const} VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    ${functionBody};
  })";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "className", initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
                             { "classSeparator", commandData.handle.empty() ? "" : "::" },
                             { "commandName", commandName },
                             { "const", commandData.handle.empty() ? "" : " const" },
                             { "functionBody", functionBody },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ${nodiscard}${returnType} ${commandName}( ${argumentList} VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )${const} VULKAN_HPP_NOEXCEPT;)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "const", commandData.handle.empty() ? "" : " const" },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType } } );
  }
}

std::string VulkanHppGenerator::generateCommandVoid(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandData.params );
  switch ( returnParams.size() )
  {
    case 0: return generateCommandVoid0Return( name, commandData, initialSkipCount, definition, raii );
    case 1: return generateCommandVoid1Return( name, commandData, initialSkipCount, definition, returnParams[0], raii );
    case 2: return generateCommandVoid2Return( name, commandData, initialSkipCount, definition, returnParams, raii );
    default: break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandValue(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandData.params );
  if ( returnParams.empty() )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
    if ( vectorParams.empty() )
    {
      if ( determineConstPointerParams( commandData.params ).empty() )
      {
        return generateCommandSetInclusive(
          name, commandData, initialSkipCount, definition, {}, vectorParams, false, {}, raii, false, { CommandFlavourFlagBits::enhanced } );
      }
      else
      {
        return generateCommandSetInclusive( name,
                                            commandData,
                                            initialSkipCount,
                                            definition,
                                            {},
                                            vectorParams,
                                            false,
                                            { CommandFlavourFlagBits::enhanced },
                                            raii,
                                            false,
                                            { CommandFlavourFlagBits::enhanced } );
      }
    }
    else if ( vectorParams.size() <= 1 )
    {
      if ( !raii )
      {
        return generateCommandSet( definition,
                                   generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                   { generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams, {} ) } );
      }
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandVoid0Return(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  if ( vectorParams.empty() && determineConstPointerParams( commandData.params ).empty() )
  {
    return generateCommandSetInclusive(
      name, commandData, initialSkipCount, definition, {}, vectorParams, false, {}, raii, false, { CommandFlavourFlagBits::enhanced } );
  }
  else if ( allVectorSizesSupported( commandData.params, vectorParams ) )
  {
    // All the vectorParams have a counter by value, of type "uint32_t", "VkDeviceSize", or "VkSampleCountFlagBits" (!)
    return generateCommandSetInclusive( name,
                                        commandData,
                                        initialSkipCount,
                                        definition,
                                        {},
                                        vectorParams,
                                        false,
                                        { CommandFlavourFlagBits::enhanced },
                                        raii,
                                        false,
                                        { CommandFlavourFlagBits::enhanced } );
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandVoid1Return(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  if ( ( commandData.params[returnParam].type.type == "void" ) )
  {
    switch ( vectorParams.size() )
    {
      case 0:
        return generateCommandSetInclusive( name,
                                            commandData,
                                            initialSkipCount,
                                            definition,
                                            { returnParam },
                                            vectorParams,
                                            false,
                                            { CommandFlavourFlagBits::enhanced },
                                            raii,
                                            false,
                                            { CommandFlavourFlagBits::enhanced } );
      case 1:
        if ( returnParam == vectorParams.begin()->first )
        {
          if ( name == stripPluralS( name ) )
          {
            std::vector<CommandFlavourFlags> flavourFlags;
            if ( name == "vkGetDescriptorEXT" )
            {
              // special handling for this function: do not return the void*, but keep its argument as it is
              flavourFlags.push_back( CommandFlavourFlagBits::noReturn | CommandFlavourFlagBits::keepVoidPtr );
            }
            flavourFlags.push_back( CommandFlavourFlagBits::singular );
            return generateCommandSetInclusive(
              name, commandData, initialSkipCount, definition, { returnParam }, vectorParams, false, flavourFlags, raii, false, flavourFlags );
          }
        }
        break;
      default: break;
    }
  }
  else if ( isHandleType( commandData.params[returnParam].type.type ) )
  {
    if ( vectorParams.empty() )
    {
      return generateCommandSetInclusive( name,
                                          commandData,
                                          initialSkipCount,
                                          definition,
                                          { returnParam },
                                          vectorParams,
                                          false,
                                          { CommandFlavourFlagBits::enhanced },
                                          raii,
                                          true,
                                          { CommandFlavourFlagBits::enhanced } );
    }
  }
  else if ( isStructureChainAnchor( commandData.params[returnParam].type.type ) )
  {
    if ( vectorParams.empty() )
    {
      return generateCommandSetInclusive( name,
                                          commandData,
                                          initialSkipCount,
                                          definition,
                                          { returnParam },
                                          vectorParams,
                                          false,
                                          { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::chained },
                                          raii,
                                          false,
                                          { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::chained } );
    }
  }
  else
  {
    switch ( vectorParams.size() )
    {
      case 0:
        return generateCommandSetInclusive( name,
                                            commandData,
                                            initialSkipCount,
                                            definition,
                                            { returnParam },
                                            vectorParams,
                                            false,
                                            { CommandFlavourFlagBits::enhanced },
                                            raii,
                                            false,
                                            { CommandFlavourFlagBits::enhanced } );
      case 1:
        if ( returnParam == vectorParams.begin()->first )
        {
          // you get a vector of stuff, with the size being one of the parameters
          return generateCommandSetInclusive( name,
                                              commandData,
                                              initialSkipCount,
                                              definition,
                                              { returnParam },
                                              vectorParams,
                                              false,
                                              { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator },
                                              raii,
                                              false,
                                              { CommandFlavourFlagBits::enhanced } );
        }
        else
        {
          if ( !isHandleType( commandData.params[vectorParams.begin()->first].type.type ) &&
               !isStructureChainAnchor( commandData.params[vectorParams.begin()->first].type.type ) &&
               ( commandData.params[vectorParams.begin()->first].type.type != "void" ) )
          {
            if ( isLenByStructMember( commandData.params[vectorParams.begin()->first].lenExpression,
                                      commandData.params[vectorParams.begin()->second.lenParam] ) )
            {
              return generateCommandSetInclusive( name,
                                                  commandData,
                                                  initialSkipCount,
                                                  definition,
                                                  { returnParam },
                                                  vectorParams,
                                                  false,
                                                  { CommandFlavourFlagBits::enhanced },
                                                  raii,
                                                  false,
                                                  { CommandFlavourFlagBits::enhanced } );
            }
          }
        }
        break;
      default: break;
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandVoid2Return( std::string const &         name,
                                                            CommandData const &         commandData,
                                                            size_t                      initialSkipCount,
                                                            bool                        definition,
                                                            std::vector<size_t> const & returnParams,
                                                            bool                        raii ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  switch ( vectorParams.size() )
  {
    case 0:
      if ( isStructureType( commandData.params[returnParams[0]].type.type ) && !isStructureChainAnchor( commandData.params[returnParams[0]].type.type ) )
      {
        if ( isStructureType( commandData.params[returnParams[1]].type.type ) && !isStructureChainAnchor( commandData.params[returnParams[1]].type.type ) )
        {
          return generateCommandSetInclusive( name,
                                              commandData,
                                              initialSkipCount,
                                              definition,
                                              returnParams,
                                              vectorParams,
                                              false,
                                              { CommandFlavourFlagBits::enhanced },
                                              raii,
                                              false,
                                              { CommandFlavourFlagBits::enhanced } );
        }
      }
      break;
    case 1:
      if ( ( returnParams[0] == vectorParams.begin()->second.lenParam ) && ( commandData.params[returnParams[0]].type.type == "uint32_t" ) )
      {
        if ( returnParams[1] == vectorParams.begin()->first )
        {
          if ( isStructureChainAnchor( commandData.params[returnParams[1]].type.type ) )
          {
            return generateCommandSetInclusive( name,
                                                commandData,
                                                initialSkipCount,
                                                definition,
                                                returnParams,
                                                vectorParams,
                                                false,
                                                { CommandFlavourFlagBits::enhanced,
                                                  CommandFlavourFlagBits::withAllocator,
                                                  CommandFlavourFlagBits::chained,
                                                  CommandFlavourFlagBits::chained | CommandFlavourFlagBits::withAllocator },
                                                raii,
                                                false,
                                                { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::chained } );
          }
          else
          {
            return generateCommandSetInclusive( name,
                                                commandData,
                                                initialSkipCount,
                                                definition,
                                                returnParams,
                                                vectorParams,
                                                false,
                                                { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator },
                                                raii,
                                                false,
                                                { CommandFlavourFlagBits::enhanced } );
          }
        }
      }
      break;
    default: break;
  }
  return "";
}

std::string VulkanHppGenerator::generateConstexprString( std::string const & structName ) const
{
  // structs with a VkBaseInStructure and VkBaseOutStructure can't be a constexpr!
  const bool isConstExpression = ( structName != "VkBaseInStructure" ) && ( structName != "VkBaseOutStructure" );
  return isConstExpression ? ( std::string( "VULKAN_HPP_CONSTEXPR" ) + ( ( containsUnion( structName ) || containsArray( structName ) ) ? "_14 " : " " ) ) : "";
}

std::string VulkanHppGenerator::generateConstexprDefines() const
{
  auto const constexprFunctionTemplate = std::string{ R"(  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  ${deprecated}VULKAN_HPP_CONSTEXPR uint32_t ${constName}( ${arguments} )
  {
    return ${implementation};
  }
)" };
  auto const constexprCallTemplate     = std::string{ R"(  ${deprecated}VULKAN_HPP_CONSTEXPR_INLINE auto ${constName} = ${callee}( ${arguments} );
)" };
  auto const constexprValueTemplate    = std::string{ R"(  ${deprecated}VULKAN_HPP_CONSTEXPR_INLINE ${type} ${constName} = ${value};
)" };
  auto const deprecatedAttribute       = std::string{ R"(VULKAN_HPP_DEPRECATED("${reason}") )" };

  // handle the value and callee macros first so they are visible for use in functions below.

  auto constexprDefines = std::string{ R"(
  //===========================
  //=== CONSTEXPR CONSTANTs ===
  //===========================
)" };

  {
    auto const generateConstantsAndProtection =
      [&constexprValueTemplate, this]( std::vector<RequireData> const & requireData, std::string const & title, std::set<std::string> & listedConstants )
    {
      auto constants = std::string{};
      for ( auto const & require : requireData )
      {
        for ( auto const & constant : require.constants )
        {
          if ( !listedConstants.contains( constant ) )
          {
            auto        constIt = m_constants.find( constant );
            std::string tag     = findTag( constant );
            constants += replaceWithMap( constexprValueTemplate,
                                         { { "type", constIt->second.type },
                                           { "constName", stripPrefix( toCamelCase( stripPostfix( constant, tag ) ), "Vk" ) + tag },
                                           { "deprecated", "" },
                                           { "value", constant } } );
            listedConstants.insert( constant );
          }
        }
      }
      return addTitleAndProtection( title, constants );
    };

    std::set<std::string> listedConstants;
    for ( auto const & feature : m_features )
    {
      constexprDefines += generateConstantsAndProtection( feature.requireData, feature.name, listedConstants );
    }
    for ( auto const & extension : m_extensions )
    {
      constexprDefines += generateConstantsAndProtection( extension.requireData, extension.name, listedConstants );
    }
  }

  // values
  constexprDefines += R"(
  //========================
  //=== CONSTEXPR VALUEs ===
  //========================
)";
  for ( auto const & [macro, data] : m_definesPartition.values )
  {
    auto const deprecated = data.deprecated ? replaceWithMap( deprecatedAttribute, { { "reason", data.deprecationReason } } ) : "";

    // make `macro` PascalCase and strip the `Vk` prefix
    auto const constName = stripPrefix( toCamelCase( macro ), "Vk" );
    auto const valueString =
      replaceWithMap( constexprValueTemplate, { { "type", "uint32_t" }, { "constName", constName }, { "deprecated", deprecated }, { "value", macro } } );

    constexprDefines += valueString;
  }

  // functions
  constexprDefines += R"(
  //=========================
  //=== CONSTEXPR CALLEEs ===
  //=========================
)";
  for ( auto const & [macro, data] : m_definesPartition.callees )
  {
    auto const deprecated = data.deprecated ? replaceWithMap( deprecatedAttribute, { { "reason", data.deprecationReason } } ) : "";
    // make `macro` camelCase and strip the `Vk` prefix
    auto const constName = startLowerCase( stripPrefix( toCamelCase( macro ), "Vk" ) );
    // for every parameter, need to use auto const and append a comma if needed (i.e. has more than one parameter, and not for the last one)
    auto parametersString = std::string{};
    for ( auto const & paramString : data.params )
    {
      parametersString += "T const " + paramString + ", ";
    }
    // trim the last two characters (i.e. the last comma and the space)
    parametersString.resize( parametersString.size() - 2 );

    auto const functionString = replaceWithMap(
      constexprFunctionTemplate,
      { { "arguments", parametersString }, { "constName", constName }, { "deprecated", deprecated }, { "implementation", data.possibleDefinition } } );

    constexprDefines += functionString;
  }

  // callers
  constexprDefines += R"(
  //=========================
  //=== CONSTEXPR CALLERs ===
  //=========================
)";
  for ( auto const & [macro, data] : m_definesPartition.callers )
  {
    auto const deprecated = data.deprecated ? replaceWithMap( deprecatedAttribute, { { "reason", data.deprecationReason } } ) : "";
    // make `macro` PascalCase and strip the `Vk` prefix
    auto const constName       = stripPrefix( toCamelCase( macro ), "Vk" );
    auto       argumentsString = std::string{};

    // for every argument, append a comma if needed (i.e. has more than one parameter, and not for the last one)
    for ( auto const & argString : data.params )
    {
      argumentsString += argString + ", ";
    }
    // trim the last two characters (i.e. the last comma and the space)
    argumentsString.resize( argumentsString.size() - 2 );

    auto const callerString = replaceWithMap(
      constexprCallTemplate,
      { { "arguments", argumentsString }, { "callee", startLowerCase( data.possibleCallee ) }, { "constName", constName }, { "deprecated", deprecated } } );
    constexprDefines += callerString;
  }

  // extension constexprs
  auto const extensionConstexprDefinesTemplate = std::string{ R"(
  //=================================
  //=== CONSTEXPR EXTENSION NAMEs ===
  //=================================

  ${extensionConstexprs}
)" };

  auto extensionConstexprs = std::string{};

  static auto const extensionTemplate           = std::string{ R"(${deprecated}VULKAN_HPP_CONSTEXPR_INLINE auto ${var} = ${macro};
)" };
  static auto const deprecatedPrefixTemplate    = std::string{ R"(VULKAN_HPP_DEPRECATED( ${message} ) )" };
  static auto const deprecatedByMessageTemplate = std::string{ R"("The ${extensionName} extension has been deprecated by ${deprecatedBy}.")" };
  static auto const deprecatedMessageTemplate   = std::string{ R"("The ${extensionName} extension has been deprecated.")" };
  static auto const obsoletedMessageTemplate    = std::string{ R"("The ${extensionName} extension has been obsoleted by ${obsoletedBy}.")" };

  // I really, really wish C++ had discards for structured bindings...
  for ( auto const & extension : m_extensions )
  {
    auto const & requireDatas = extension.requireData;

    // assert that requireDatas has at least one require...
    // and the first require has at least two enumConstants, which we are going to use
    assert( requireDatas.size() >= 1 );
    assert( requireDatas.front().enumConstants.size() >= 2 );
    auto const & enumConstants = requireDatas.front().enumConstants;

    auto const VENDORPascalCaseStripPrefix = []( std::string const & macro )
    {
      auto       prefixStripped = stripPrefix( macro, "VK_" );
      auto const vendor         = prefixStripped.substr( 0, prefixStripped.find( '_' ) );
      return vendor + toCamelCase( stripPrefix( prefixStripped, vendor + "_" ) );
    };

    // add asserts so we don't get a nullptr exception below
    auto const & extensionMacroPtr =
      std::find_if( enumConstants.begin(), enumConstants.end(), []( auto const & keyval ) { return keyval.first.ends_with( "_EXTENSION_NAME" ); } );
    auto const & specVersionMacroPtr =
      std::find_if( enumConstants.begin(), enumConstants.end(), []( auto const & keyval ) { return keyval.first.ends_with( "_SPEC_VERSION" ); } );
    assert( extensionMacroPtr != enumConstants.end() );
    assert( specVersionMacroPtr != enumConstants.end() );

    auto const & extensionMacro   = extensionMacroPtr->first;
    auto const & specVersionMacro = specVersionMacroPtr->first;

    auto const extensionVar   = VENDORPascalCaseStripPrefix( extensionMacro );
    auto const specVersionVar = VENDORPascalCaseStripPrefix( specVersionMacro );

    std::string deprecationMessage;
    if ( extension.isDeprecated )
    {
      assert( extension.obsoletedBy.empty() && extension.promotedTo.empty() );
      deprecationMessage = extension.deprecatedBy.empty()
                           ? replaceWithMap( deprecatedMessageTemplate, { { "extensionName", extension.name } } )
                           : replaceWithMap( deprecatedByMessageTemplate, { { "extensionName", extension.name }, { "deprecatedBy", extension.deprecatedBy } } );
    }
    else if ( !extension.obsoletedBy.empty() )
    {
      assert( extension.promotedTo.empty() );
      deprecationMessage = replaceWithMap( obsoletedMessageTemplate, { { "extensionName", extension.name }, { "obsoletedBy", extension.obsoletedBy } } );
    }
    else if ( !extension.promotedTo.empty() )
    {
      // promoted extensions are _not_ deprecated!
    }
    auto const deprecatedPrefix = deprecationMessage.empty() ? "" : replaceWithMap( deprecatedPrefixTemplate, { { "message", deprecationMessage } } );

    auto const thisExtensionConstexprs =
      replaceWithMap( extensionTemplate, { { "deprecated", deprecatedPrefix }, { "macro", extensionMacro }, { "var", extensionVar } } ) +
      replaceWithMap( extensionTemplate, { { "deprecated", deprecatedPrefix }, { "macro", specVersionMacro }, { "var", specVersionVar } } );

    extensionConstexprs += addTitleAndProtection( extension.name, thisExtensionConstexprs );
  }

  constexprDefines += replaceWithMap( extensionConstexprDefinesTemplate, { { "extensionConstexprs", extensionConstexprs } } );

  return constexprDefines;
}

std::string VulkanHppGenerator::generateConstexprUsings() const
{
  auto constexprUsings = std::string{ R"(
  //===========================
  //=== CONSTEXPR CONSTANTs ===
  //===========================
)" };

  auto const constexprUsingTemplate = std::string{ R"(  using VULKAN_HPP_NAMESPACE::${constName};
)" };

  auto const pascalCasePrefixStrip = []( std::string const & macro ) { return stripPrefix( toCamelCase( macro ), "Vk" ); };
  auto const camelCasePrefixStrip  = []( std::string const & macro ) { return startLowerCase( stripPrefix( toCamelCase( macro ), "Vk" ) ); };
  auto const VENDORCasePrefixStrip = []( std::string const & macro )
  {
    auto       prefixStripped = stripPrefix( macro, "VK_" );
    auto const vendor         = prefixStripped.substr( 0, prefixStripped.find( '_' ) );
    return vendor + toCamelCase( stripPrefix( prefixStripped, vendor + "_" ) );
  };

  // constants
  {
    auto const generateConstantsAndProtection =
      [&]( std::vector<RequireData> const & requireData, std::string const & title, std::set<std::string> & listedConstants )
    {
      auto constants = std::string{};
      for ( auto const & require : requireData )
      {
        for ( auto const & constant : require.constants )
        {
          if ( !listedConstants.contains( constant ) )
          {
            assert( m_constants.find( constant ) != m_constants.end() );
            std::string tag = findTag( constant );
            constants += replaceWithMap( constexprUsingTemplate, { { "constName", stripPrefix( toCamelCase( stripPostfix( constant, tag ) ), "Vk" ) + tag } } );
            listedConstants.insert( constant );
          }
        }

        for ( auto const & [key, _] : require.enumConstants )
        {
          // keys are the constants themselves. Values are their definitions, and don't need them...
          // Again, recall these constants are all in the form VK_<vendor>_<name>_EXTENSION_NAME and VK_<vendor>_<name>_SPEC_VERSION
          // strip the Vk, get the vendor, and PascalCase the rest
          constants += replaceWithMap( constexprUsingTemplate, { { "constName", VENDORCasePrefixStrip( key ) } } );
          listedConstants.insert( key );
        }
      }
      return addTitleAndProtection( title, constants );
    };

    std::set<std::string> listedConstants;
    for ( auto const & feature : m_features )
    {
      constexprUsings += generateConstantsAndProtection( feature.requireData, feature.name, listedConstants );
    }
    for ( auto const & extension : m_extensions )
    {
      constexprUsings += generateConstantsAndProtection( extension.requireData, extension.name, listedConstants );
    }
  }

  // values
  constexprUsings += R"(
  //========================
  //=== CONSTEXPR VALUEs ===
  //========================
)";
  for ( auto const & macro : m_definesPartition.values )
  {
    // make `macro` PascalCase and strip the `Vk` prefix
    auto const constName = pascalCasePrefixStrip( macro.first );
    constexprUsings += replaceWithMap( constexprUsingTemplate, { { "constName", constName } } );
  }

  // callees
  constexprUsings += R"(
  //=========================
  //=== CONSTEXPR CALLEEs ===
  //=========================
)";
  for ( auto const & macro : m_definesPartition.callees )
  {
    // make `macro` camelCase and strip the `Vk` prefix
    auto const constName = camelCasePrefixStrip( macro.first );
    constexprUsings += replaceWithMap( constexprUsingTemplate, { { "constName", constName } } );
  }

  // callers
  constexprUsings += R"(
  //==========================
  //=== CONSTEXPR CALLERs ===
  //==========================
)";
  for ( auto const & macro : m_definesPartition.callers )
  {
    // make `macro` PascalCase and strip the `Vk` prefix
    auto const constName = pascalCasePrefixStrip( macro.first );
    constexprUsings += replaceWithMap( constexprUsingTemplate, { { "constName", constName } } );
  }

  return constexprUsings;
}

std::string VulkanHppGenerator::generateCppModuleFuncpointerUsings() const
{
  std::string funcpointerUsings = R"(
  //====================
  //=== FUNCPOINTERs ===
  //====================
)";

  auto const generateUsingsAndProtection = [this]( std::vector<RequireData> const & requireData, std::string const & title )
  {
    auto usings = std::string{};
    for ( auto const & require : requireData )
    {
      for ( auto const & type : require.types )
      {
        if ( auto const & funcpointerIt = m_funcPointers.find( type.name ); funcpointerIt != m_funcPointers.end() )
        {
          usings += "  using VULKAN_HPP_NAMESPACE::PFN_" + stripPrefix( funcpointerIt->first, "PFN_vk" ) + ";\n";
        }
      }
    }
    return addTitleAndProtection( title, usings );
  };

  for ( auto const & feature : m_features )
  {
    funcpointerUsings += generateUsingsAndProtection( feature.requireData, feature.name );
  }

  for ( auto const & extension : m_extensions )
  {
    funcpointerUsings += generateUsingsAndProtection( extension.requireData, extension.name );
  }

  return funcpointerUsings;
}

std::string VulkanHppGenerator::generateCppModuleHandleUsings() const
{
  auto const usingTemplate = std::string{ R"(  using VULKAN_HPP_NAMESPACE::${className};
)" };

  auto handleUsings = std::string{ R"(
  //===============
  //=== HANDLEs ===
  //===============

  using VULKAN_HPP_NAMESPACE::isVulkanHandleType;
)" };

  auto const generateUsingsAndProtection = [&usingTemplate, this]( std::vector<RequireData> const & requireData, std::string const & title )
  {
    auto usings = std::string{};
    for ( auto const & require : requireData )
    {
      for ( auto const & type : require.types )
      {
        if ( auto const & handleIt = m_handles.find( type.name ); handleIt != m_handles.end() )
        {
          usings += replaceWithMap( usingTemplate, { { "className", stripPrefix( handleIt->first, "Vk" ) } } );
        }
      }
    }
    return addTitleAndProtection( title, usings );
  };

  for ( auto const & feature : m_features )
  {
    handleUsings += generateUsingsAndProtection( feature.requireData, feature.name );
  }

  for ( auto const & extension : m_extensions )
  {
    handleUsings += generateUsingsAndProtection( extension.requireData, extension.name );
  }

  return handleUsings;
}

std::string VulkanHppGenerator::generateCppModuleStructUsings() const
{
  auto const usingTemplate = std::string{ R"(  using VULKAN_HPP_NAMESPACE::${structName};
)" };

  auto structUsings  = std::string{ R"(
  //===============
  //=== STRUCTs ===
  //===============
)" };
  auto listedStructs = std::set<std::string>{};

  auto const generateUsingsAndProtection = [&listedStructs, &usingTemplate, this]( std::vector<RequireData> const & requireData, std::string const & title )
  {
    auto localUsings = std::string{};
    for ( auto const & require : requireData )
    {
      for ( auto const & type : require.types )
      {
        if ( auto const & structIt = m_structs.find( type.name ); structIt != m_structs.end() && listedStructs.insert( type.name ).second )
        {
          auto const structureType = stripPrefix( structIt->first, "Vk" );
          localUsings += replaceWithMap( usingTemplate, { { "structName", structureType } } );

          for ( auto const & alias : structIt->second.aliases )
          {
            localUsings += replaceWithMap( usingTemplate, { { "structName", stripPrefix( alias.first, "Vk" ) } } );
          }
        }
      }
    }
    return addTitleAndProtection( title, localUsings );
  };

  for ( auto const & feature : m_features )
  {
    structUsings += generateUsingsAndProtection( feature.requireData, feature.name );
  }

  for ( auto const & extension : m_extensions )
  {
    structUsings += generateUsingsAndProtection( extension.requireData, extension.name );
  }

  return structUsings;
}

std::string VulkanHppGenerator::generateCppModuleSharedHandleUsings() const
{
  auto const usingTemplate                        = std::string{ R"(  using VULKAN_HPP_NAMESPACE::Shared${handleName};
)" };
  auto       sharedHandleUsings                   = std::string{ R"(
  //======================
  //=== SHARED HANDLEs ===
  //======================
)" };
  auto const [smartHandleEnter, smartHandleLeave] = generateProtection( "VULKAN_HPP_NO_SMART_HANDLE", false );

  sharedHandleUsings += "\n" + smartHandleEnter;

  auto const generateUsingsAndProtection = [&usingTemplate, this]( std::vector<RequireData> const & requireData, std::string const & title )
  {
    auto usings = std::string{};
    for ( auto const & require : requireData )
    {
      for ( auto const & type : require.types )
      {
        if ( auto const & handleIt = m_handles.find( type.name ); handleIt != m_handles.end() )
        {
          usings += replaceWithMap( usingTemplate, { { "handleName", stripPrefix( handleIt->first, "Vk" ) } } );
        }
      }
    }
    return addTitleAndProtection( title, usings );
  };

  for ( auto const & feature : m_features )
  {
    sharedHandleUsings += generateUsingsAndProtection( feature.requireData, feature.name );
  }

  for ( auto const & extension : m_extensions )
  {
    sharedHandleUsings += generateUsingsAndProtection( extension.requireData, extension.name );
  }

  sharedHandleUsings += R"(  using VULKAN_HPP_NAMESPACE::SharedHandleTraits;
)";

  sharedHandleUsings += smartHandleLeave + "\n";

  return sharedHandleUsings;
}

std::string VulkanHppGenerator::generateCppModuleUniqueHandleUsings() const
{
  auto const usingTemplate                        = std::string{ R"(  using VULKAN_HPP_NAMESPACE::Unique${handleName};
)" };
  auto       uniqueHandleUsings                   = std::string{ R"(
  //======================
  //=== UNIQUE HANDLEs ===
  //======================
)" };
  auto const [smartHandleEnter, smartHandleLeave] = generateProtection( "VULKAN_HPP_NO_SMART_HANDLE", false );

  uniqueHandleUsings += "\n" + smartHandleEnter;

  auto const generateUsingsAndProtection = [&usingTemplate, this]( std::vector<RequireData> const & requireData, std::string const & title )
  {
    auto usings = std::string{};
    for ( auto const & require : requireData )
    {
      for ( auto const & type : require.types )
      {
        if ( auto const & handleIt = m_handles.find( type.name ); handleIt != m_handles.end() && !handleIt->second.deleteCommand.empty() )
        {
          usings += replaceWithMap( usingTemplate, { { "handleName", stripPrefix( handleIt->first, "Vk" ) } } );
        }
      }
    }
    return addTitleAndProtection( title, usings );
  };

  for ( auto const & feature : m_features )
  {
    uniqueHandleUsings += generateUsingsAndProtection( feature.requireData, feature.name );
  }

  for ( auto const & extension : m_extensions )
  {
    uniqueHandleUsings += generateUsingsAndProtection( extension.requireData, extension.name );
  }

  uniqueHandleUsings += R"(  using VULKAN_HPP_NAMESPACE::UniqueHandleTraits;
)";

  uniqueHandleUsings += smartHandleLeave + "\n";

  return uniqueHandleUsings;
}

std::string VulkanHppGenerator::generateCppModuleFuncsUsings() const
{
  auto const usingTemplate = std::string{ R"(  using VULKAN_HPP_NAMESPACE::${funcName};
)" };

  auto funcUsings = std::string{ R"(
  //===========================
  //=== COMMAND Definitions ===
  //===========================
)" };

  for ( auto const & func : m_handles.at( "" ).commands )
  {
    funcUsings += replaceWithMap( usingTemplate, { { "funcName", startLowerCase( stripPrefix( func, "vk" ) ) } } );
  }

  auto const [enter, leave] = generateProtection( "VULKAN_HPP_NO_SMART_HANDLE", false );
  funcUsings += "\n" + enter + replaceWithMap( usingTemplate, { { "funcName", "createInstanceUnique" } } ) + leave + "\n";

  return funcUsings;
}

std::string VulkanHppGenerator::generateCppModuleEnumUsings() const
{
  auto const usingTemplate = std::string{ R"(  using VULKAN_HPP_NAMESPACE::${enumName};
)" };

  auto enumUsings  = std::string{ R"(
  //=============
  //=== ENUMs ===
  //=============
)" };
  auto listedEnums = std::set<std::string>{};

  // insert CppType first
  enumUsings += replaceWithMap( usingTemplate, { { "enumName", "CppType" } } );

  auto const generateUsingsAndProtection = [&listedEnums, &usingTemplate, this]( std::vector<RequireData> const & requireData, std::string const & title )
  {
    auto localUsings = std::string{};
    for ( auto const & require : requireData )
    {
      for ( auto const & type : require.types )
      {
        if ( auto const & enumIt = m_enums.find( type.name ); enumIt != m_enums.end() && listedEnums.insert( type.name ).second )
        {
          auto const enumName = stripPrefix( enumIt->first, "Vk" );
          localUsings += replaceWithMap( usingTemplate, { { "enumName", enumName } } );

          for ( auto const & alias : enumIt->second.aliases )
          {
            localUsings += replaceWithMap( usingTemplate, { { "enumName", stripPrefix( alias.first, "Vk" ) } } );
          }

          if ( auto const bitmaskIt =
                 std::find_if( m_bitmasks.begin(),
                               m_bitmasks.end(),
                               [&enumIt]( std::pair<std::string, BitmaskData> const & bitmask ) { return bitmask.second.require == enumIt->first; } );
               bitmaskIt != m_bitmasks.end() )
          {
            localUsings += replaceWithMap( usingTemplate, { { "enumName", stripPrefix( bitmaskIt->first, "Vk" ) } } );
            for ( auto const & alias : bitmaskIt->second.aliases )
            {
              localUsings += replaceWithMap( usingTemplate, { { "enumName", stripPrefix( alias.first, "Vk" ) } } );
            }
          }
        }
      }
    }
    return addTitleAndProtection( title, localUsings );
  };

  for ( auto const & feature : m_features )
  {
    enumUsings += generateUsingsAndProtection( feature.requireData, feature.name );
  }

  for ( auto const & extension : m_extensions )
  {
    enumUsings += generateUsingsAndProtection( extension.requireData, extension.name );
  }

  // finally insert IndexTypeValue
  auto const indexTypeComment = R"(
  //=========================
  //=== Index Type Traits ===
  //=========================
)";
  enumUsings += indexTypeComment + replaceWithMap( usingTemplate, { { "enumName", "IndexTypeValue" } } );

  return enumUsings;
}

std::string VulkanHppGenerator::generateCppModuleFormatTraitsUsings() const
{
  // everything is hardcoded, so things are very easy...
  auto formatTraitsUsings = std::string{ R"(
  //=====================
  //=== Format Traits ===
  //=====================
)" };

  auto const usingTemplate        = std::string{ R"(  using VULKAN_HPP_NAMESPACE::${function};
)" };
  auto const formatTraitFunctions = std::array{ "blockExtent",
                                                "blockSize",
                                                "compatibilityClass",
                                                "componentBits",
                                                "componentCount",
                                                "componentName",
                                                "componentNumericFormat",
                                                "componentPlaneIndex",
                                                "componentsAreCompressed",
                                                "compressionScheme",
                                                "isCompressed",
                                                "packed",
                                                "planeCompatibleFormat",
                                                "planeCount",
                                                "planeHeightDivisor",
                                                "planeWidthDivisor",
                                                "texelsPerBlock" };

  for ( auto const & func : formatTraitFunctions )
  {
    formatTraitsUsings += replaceWithMap( usingTemplate, { { "function", func } } );
  }

  return formatTraitsUsings;
}

std::string VulkanHppGenerator::generateCppModuleExtensionInspectionUsings() const
{
  auto const usingTemplate = std::string{ R"(  using VULKAN_HPP_NAMESPACE::${function};
)" };

  auto extensionInspectionsUsings = std::string{ R"(
  //======================================
  //=== Extension inspection functions ===
  //======================================
)" };

  auto const extensionInspectionFunctions =
    std::array{ "getDeviceExtensions",    "getInstanceExtensions", "getDeprecatedExtensions",  "getExtensionDepends",     "getExtensionDepends",
                "getObsoletedExtensions", "getPromotedExtensions", "getExtensionDeprecatedBy", "getExtensionObsoletedBy", "getExtensionPromotedTo",
                "isDeprecatedExtension",  "isDeviceExtension",     "isInstanceExtension",      "isObsoletedExtension",    "isPromotedExtension" };

  for ( auto const & func : extensionInspectionFunctions )
  {
    extensionInspectionsUsings += replaceWithMap( usingTemplate, { { "function", func } } );
  }

  return extensionInspectionsUsings;
}

std::string VulkanHppGenerator::generateCppModuleUsings() const
{
  auto const usingTemplate = std::string{ R"(  using VULKAN_HPP_NAMESPACE::${className};
)" };

  auto const hardCodedEnhancedModeTypes      = std::array{ "ArrayProxy", "ArrayProxyNoTemporaries", "StridedArrayProxy", "Optional", "StructureChain" };
  auto const hardCodedSmartHandleHelperTypes = std::array{ "ObjectDestroy",       "ObjectFree",       "ObjectRelease",       "PoolFree",
                                                           "ObjectDestroyShared", "ObjectFreeShared", "ObjectReleaseShared", "PoolFreeShared" };
  auto const hardCodedFunctions              = std::array{ "exchange" };

  auto usings = std::string{ R"(  //=====================================
  //=== HARDCODED TYPEs AND FUNCTIONs ===
  //=====================================
  using VULKAN_HPP_NAMESPACE::ArrayWrapper1D;
  using VULKAN_HPP_NAMESPACE::ArrayWrapper2D;
  using VULKAN_HPP_NAMESPACE::Flags;
  using VULKAN_HPP_NAMESPACE::FlagTraits;
  namespace detail
  {
    using VULKAN_HPP_NAMESPACE::detail::DispatchLoaderBase;
    using VULKAN_HPP_NAMESPACE::detail::DispatchLoaderDynamic;
#if !defined( VK_NO_PROTOTYPES )
    using VULKAN_HPP_NAMESPACE::detail::DispatchLoaderStatic;
    using VULKAN_HPP_NAMESPACE::detail::getDispatchLoaderStatic;
#endif /*VK_NO_PROTOTYPES*/
  }
)" };

  // insert the Flags bitwise operators
  auto const flagsBitWiseOperatorsUsings = std::array{ "operator&", "operator|", "operator^", "operator~" };
  for ( auto const & operatorName : flagsBitWiseOperatorsUsings )
  {
    usings += replaceWithMap( usingTemplate, { { "className", operatorName } } );
  }

  // delete the namespace declaration for the default dispatcher macro using statement
  usings += R"(using VULKAN_HPP_DEFAULT_DISPATCHER_TYPE;
)";

  auto enhancedModeUsings = std::string{};
  for ( auto const & className : hardCodedEnhancedModeTypes )
  {
    enhancedModeUsings += replaceWithMap( usingTemplate, { { "className", std::string{ className } } } );
  }
  // protect the enhanced-mode usings with a macro
  const auto [enterEnhancedMode, leaveEnhancedMode] = generateProtection( "VULKAN_HPP_DISABLE_ENHANCED_MODE", false );
  usings += "\n" + enterEnhancedMode + enhancedModeUsings + leaveEnhancedMode + "\n";

  std::string smartHandleUsings = "  namespace detail\n  {\n";
  for ( auto const & helperType : hardCodedSmartHandleHelperTypes )
  {
    smartHandleUsings += "    using VULKAN_HPP_NAMESPACE::detail::"s + helperType + ";\n";
  }
  smartHandleUsings += R"(    }

  using VULKAN_HPP_NAMESPACE::SharedHandle;
  using VULKAN_HPP_NAMESPACE::UniqueHandle;
)";

  // likewise for the smart-handle usings
  const auto [enterNoSmartHandle, leaveNoSmartHandle] = generateProtection( "VULKAN_HPP_NO_SMART_HANDLE", false );
  usings += "\n" + enterNoSmartHandle + smartHandleUsings + leaveNoSmartHandle + "\n";

  for ( auto const & functionName : hardCodedFunctions )
  {
    usings += "\n" + replaceWithMap( usingTemplate, { { "className", std::string{ functionName } } } );
  }

  // now generate baseTypes
  auto baseTypes = std::string{ R"(
  //==================
  //=== BASE TYPEs ===
  //==================
)" };
  for ( auto const & baseType : m_baseTypes )
  {
    if ( baseType.first != "VkFlags" && baseType.first != "VkFlags64" && !baseType.second.typeInfo.type.empty() )
    {
      baseTypes += replaceWithMap( usingTemplate, { { "className", stripPrefix( baseType.first, "Vk" ) } } );
    }
  }
  usings += baseTypes;

  // generate Enums
  usings += generateCppModuleEnumUsings();

  // to_string, toHexString
  auto const toString                       = std::array{ "to_string", "toHexString" };
  auto const [toStringEnter, toStringLeave] = generateProtection( "VULKAN_HPP_NO_TO_STRING", false );

  usings += R"(
  //======================
  //=== ENUM to_string ===
  //======================
)" + toStringEnter;
  for ( auto const & name : toString )
  {
    usings += replaceWithMap( usingTemplate, { { "className", name } } );
  }
  usings += toStringLeave + "\n";

  // hardcoded exceptions and functions
  auto const hardCodedExceptionTypesAndFunctions =
    std::array{ "ErrorCategoryImpl", "Error", "LogicError", "SystemError", "errorCategory", "make_error_code", "make_error_condition" };
  const auto [exceptionsEnter, exceptionsLeave] = generateProtection( "VULKAN_HPP_NO_EXCEPTIONS", false );

  auto exceptionsUsings = std::string{ R"(
  //=============================
  //=== EXCEPTIONs AND ERRORs ===
  //=============================
)" } + exceptionsEnter;

  for ( auto const & name : hardCodedExceptionTypesAndFunctions )
  {
    exceptionsUsings += replaceWithMap( usingTemplate, { { "className", name } } );
  }

  usings += exceptionsUsings;

  // result Exceptions
  auto resultExceptionsUsings = std::string{};
  auto const & [name, data]   = *m_enums.find( "VkResult" );
  for ( auto const & value : data.values )
  {
    if ( value.supported && value.name.starts_with( "VK_ERROR" ) )
    {
      auto [enter, leave] = generateProtection( value.protect );
      enter               = enter.empty() ? enter : "\n" + enter;
      leave               = leave.empty() ? leave : leave + "\n";

      auto const valueName = generateEnumValueName( name, value.name, false );
      auto const className = stripPrefix( valueName, "eError" ) + "Error";

      resultExceptionsUsings += enter + replaceWithMap( usingTemplate, { { "className", className } } ) + leave;
    }
  }
  usings += resultExceptionsUsings;

  usings += exceptionsLeave + "\n";

  // some hardcoded types
  auto const hardCodedResultValueTypes = std::array{ "ResultValue", "ResultValueType" };
  for ( auto const & valueType : hardCodedResultValueTypes )
  {
    usings += replaceWithMap( usingTemplate, { { "className", valueType } } );
  }

  usings += "\n" + generateConstexprUsings() + "\n";

  // structs, handles, UniqueHandles, etc
  usings += generateCppModuleFuncpointerUsings();
  usings += generateCppModuleStructUsings();
  usings += generateCppModuleHandleUsings();
  usings += generateCppModuleUniqueHandleUsings();
  usings += generateCppModuleSharedHandleUsings();
  usings += generateCppModuleFuncsUsings();

  auto const [enterDisableEnhanced, leaveDisableEnhanced] = generateProtection( "VULKAN_HPP_DISABLE_ENHANCED_MODE", false );
  usings += "\n" + enterDisableEnhanced + replaceWithMap( usingTemplate, { { "className", "StructExtends" } } ) + leaveDisableEnhanced + "\n";

  auto const dynamicLoaderUsing = std::string{ R"(#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
  namespace detail
  {
    using VULKAN_HPP_NAMESPACE::detail::DynamicLoader;
  }
#endif /*VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL*/
)" };
  usings += dynamicLoaderUsing;

  usings += generateCppModuleFormatTraitsUsings();
  usings += generateCppModuleExtensionInspectionUsings();

  return usings;
}

std::string VulkanHppGenerator::generateCppModuleRaiiUsings() const
{
  auto const raiiUsingTemplate = std::string{ R"(    using VULKAN_HPP_RAII_NAMESPACE::${className};
)" };
  auto       usings            = std::string{ R"(    //======================
    //=== RAII HARDCODED ===
    //======================

    using VULKAN_HPP_RAII_NAMESPACE::Context;
    namespace detail
    {
      using VULKAN_HPP_RAII_NAMESPACE::detail::ContextDispatcher;
      using VULKAN_HPP_RAII_NAMESPACE::detail::InstanceDispatcher;
      using VULKAN_HPP_RAII_NAMESPACE::detail::DeviceDispatcher;
    }

    //====================
    //=== RAII HANDLEs ===
    //====================
)" };

  auto const generateUsingsAndProtection = [&raiiUsingTemplate, this]( std::vector<RequireData> const & requireData, std::string const & title )
  {
    auto usings = std::string{};
    for ( auto const & require : requireData )
    {
      for ( auto const & type : require.types )
      {
        if ( auto const & handleIt = m_handles.find( type.name ); handleIt != m_handles.end() )
        {
          usings += replaceWithMap( raiiUsingTemplate, { { "className", stripPrefix( handleIt->first, "Vk" ) } } );

          // if there is an array constructor, generate the plural type also
          if ( hasArrayConstructor( handleIt->second ) )
          {
            usings += replaceWithMap( raiiUsingTemplate, { { "className", stripPrefix( type.name, "Vk" ) + "s" } } );
          }
        }
      }
    }
    return addTitleAndProtection( title, usings );
  };

  // now, insert features and extensions with protection, and strip Vk prefix
  for ( auto const & feature : m_features )
  {
    usings += generateUsingsAndProtection( feature.requireData, feature.name );
  }

  for ( auto const & extension : m_extensions )
  {
    usings += generateUsingsAndProtection( extension.requireData, extension.name );
  }

  return usings;
}

std::string VulkanHppGenerator::generateCppModuleHandleHashSpecializations() const
{
  const std::string hashesTemplate = R"(
  //========================================
  //=== HASH specializations for handles ===
  //========================================

${specializations}
)";

  auto const generateSpecializations = [this]( std::vector<RequireData> const & requireData, std::string const & title )
  {
    auto specializations = std::string{};
    for ( auto const & require : requireData )
    {
      for ( auto const & type : require.types )
      {
        if ( auto const & handleIt = m_handles.find( type.name ); handleIt != m_handles.end() )
        {
          specializations += "  template <> struct hash<VULKAN_HPP_NAMESPACE::" + stripPrefix( handleIt->first, "Vk" ) + ">;\n";
        }
      }
    }
    return addTitleAndProtection( title, specializations );
  };

  std::string specializations;
  for ( auto const & feature : m_features )
  {
    specializations += generateSpecializations( feature.requireData, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    specializations += generateSpecializations( extension.requireData, extension.name );
  }
  return replaceWithMap( hashesTemplate, { { "specializations", specializations } } );
}

std::string VulkanHppGenerator::generateCppModuleHashSpecializations() const
{
  std::string const hasSpecializationsTemplate = R"(
  //=======================================
  //=== HASH specialization for Flags types ===
  //=======================================

  template <typename BitType>
  struct hash<VULKAN_HPP_NAMESPACE::Flags<BitType>>;

  ${handleHashSpecializations}
  ${structHashSpecializations}
)";

  return replaceWithMap( hasSpecializationsTemplate,
                         { { "handleHashSpecializations", generateCppModuleHandleHashSpecializations() },
                           { "structHashSpecializations", generateCppModuleStructHashSpecializations() } } );
}

std::string VulkanHppGenerator::generateCppModuleStructHashSpecializations() const
{
  const std::string hashesTemplate = R"(
  //========================================
  //=== HASH specializations for structs ===
  //========================================

${specializations}
)";

  auto const generateSpecializations = [this]( std::vector<RequireData> const & requireData, std::string const & title )
  {
    auto specializations = std::string{};
    for ( auto const & require : requireData )
    {
      for ( auto const & type : require.types )
      {
        if ( auto const & structIt = m_structs.find( type.name ); structIt != m_structs.end() )
        {
          specializations += "  template <> struct hash<VULKAN_HPP_NAMESPACE::" + stripPrefix( structIt->first, "Vk" ) + ">;\n";
        }
      }
    }
    return addTitleAndProtection( title, specializations );
  };

  std::string specializations;
  for ( auto const & feature : m_features )
  {
    specializations += generateSpecializations( feature.requireData, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    specializations += generateSpecializations( extension.requireData, extension.name );
  }
  return replaceWithMap( hashesTemplate, { { "specializations", specializations } } );
}

std::string VulkanHppGenerator::generateDataDeclarations( CommandData const &                       commandData,
                                                          std::vector<size_t> const &               returnParams,
                                                          std::map<size_t, VectorParamData> const & vectorParams,
                                                          std::set<size_t> const &                  templatedParams,
                                                          CommandFlavourFlags                       flavourFlags,
                                                          bool                                      raii,
                                                          std::vector<std::string> const &          dataTypes,
                                                          std::string const &                       dataType,
                                                          std::string const &                       returnType,
                                                          std::string const &                       returnVariable ) const
{
  assert( dataTypes.size() == returnParams.size() );

  switch ( returnParams.size() )
  {
    case 0: return "";  // no returnParams -> no data declarations
    case 1:
      return generateDataDeclarations1Return(
        commandData, returnParams, vectorParams, templatedParams, flavourFlags, dataTypes, dataType, returnType, returnVariable );
    case 2:
      assert( !( flavourFlags & CommandFlavourFlagBits::unique ) );
      return generateDataDeclarations2Returns( commandData, returnParams, vectorParams, flavourFlags, raii, dataTypes, dataType, returnVariable );
    case 3:
      assert( !( flavourFlags & ( CommandFlavourFlagBits::singular | CommandFlavourFlagBits::unique ) ) );
      return generateDataDeclarations3Returns( commandData, returnParams, vectorParams, flavourFlags, raii, dataTypes );
    default: assert( false ); return "";
  }
}

std::string VulkanHppGenerator::generateDataDeclarations1Return( CommandData const &                       commandData,
                                                                 std::vector<size_t> const &               returnParams,
                                                                 std::map<size_t, VectorParamData> const & vectorParams,
                                                                 std::set<size_t> const &                  templatedParams,
                                                                 CommandFlavourFlags                       flavourFlags,
                                                                 std::vector<std::string> const &          dataTypes,
                                                                 std::string const &                       dataType,
                                                                 std::string const &                       returnType,
                                                                 std::string const &                       returnVariable ) const
{
  auto vectorParamIt = vectorParams.find( returnParams[0] );
  if ( !( flavourFlags & CommandFlavourFlagBits::chained ) )
  {
    if ( ( vectorParamIt == vectorParams.end() ) || ( flavourFlags & CommandFlavourFlagBits::singular ) )
    {
      std::string const dataDeclarationsTemplate = R"(${returnType} ${returnVariable};)";
      if ( dataType.starts_with( "VULKAN_HPP_NAMESPACE" ) )
      {
        return replaceWithMap( dataDeclarationsTemplate, { { "returnType", dataType }, { "returnVariable", returnVariable } } );
      }
      else
      {
        return replaceWithMap( dataDeclarationsTemplate, { { "returnType", returnType }, { "returnVariable", returnVariable } } );
      }
    }
    else
    {
      std::string allocator = stripPrefix( dataTypes[0], "VULKAN_HPP_NAMESPACE::" ) + "Allocator";
      if ( vectorParamIt->second.byStructure )
      {
        std::string vectorAllocator = ( ( flavourFlags & CommandFlavourFlagBits::withAllocator ) && !( flavourFlags & CommandFlavourFlagBits::unique ) )
                                      ? ( "( " + startLowerCase( allocator ) + " )" )
                                      : "";

        std::string const dataDeclarationTemplate = R"(${dataType} ${returnVariable}${vectorAllocator};
    ${structType} ${structVariable};)";

        return replaceWithMap( dataDeclarationTemplate,
                               { { "dataType", dataType },
                                 { "returnVariable", returnVariable },
                                 { "structType", stripPostfix( commandData.params[vectorParamIt->first].type.compose( "VULKAN_HPP_NAMESPACE" ), "*" ) },
                                 { "structVariable", startLowerCase( stripPrefix( commandData.params[vectorParamIt->first].name, "p" ) ) },
                                 { "vectorAllocator", stripPrefix( vectorAllocator, "," ) } } );
      }
      else
      {
        std::string vectorAllocator = ( ( flavourFlags & CommandFlavourFlagBits::withAllocator ) && !( flavourFlags & CommandFlavourFlagBits::unique ) )
                                      ? ( ", " + startLowerCase( allocator ) )
                                      : "";
        std::string vectorSize      = getVectorSize( commandData.params, vectorParams, returnParams[0], dataTypes[0], templatedParams );

        std::string const dataDeclarationsTemplate = R"(${dataType} ${returnVariable}( ${vectorSize}${vectorAllocator} );)";

        return replaceWithMap(
          dataDeclarationsTemplate,
          { { "dataType", dataType }, { "returnVariable", returnVariable }, { "vectorAllocator", vectorAllocator }, { "vectorSize", vectorSize } } );
      }
    }
  }
  else
  {
    assert( ( vectorParamIt == vectorParams.end() ) || ( flavourFlags & CommandFlavourFlagBits::singular ) );

    std::string dataVariable = startLowerCase( stripPrefix( commandData.params[returnParams[0]].name, "p" ) );

    std::string const dataDeclarationsTemplate = R"(${returnType} ${returnVariable};
    ${dataType} & ${dataVariable} = ${returnVariable}.template get<${dataType}>();)";

    return replaceWithMap( dataDeclarationsTemplate,
                           { { "dataType", dataTypes[0] },
                             { "dataVariable", dataVariable },
                             { "returnType", ( commandData.returnType == "void" ) ? returnType : "StructureChain<X, Y, Z...>" },
                             { "returnVariable", returnVariable } } );
  }
}

std::string VulkanHppGenerator::generateDataDeclarations2Returns( CommandData const &                       commandData,
                                                                  std::vector<size_t> const &               returnParams,
                                                                  std::map<size_t, VectorParamData> const & vectorParams,
                                                                  CommandFlavourFlags                       flavourFlags,
                                                                  bool                                      raii,
                                                                  std::vector<std::string> const &          dataTypes,
                                                                  std::string const &                       returnType,
                                                                  std::string const &                       returnVariable ) const
{
  const bool chained       = flavourFlags & CommandFlavourFlagBits::chained;
  const bool singular      = flavourFlags & CommandFlavourFlagBits::singular;
  const bool withAllocator = flavourFlags & CommandFlavourFlagBits::withAllocator;

  switch ( vectorParams.size() )
  {
    case 0:
      assert( !singular && !chained );
      {
        std::string firstDataVariable  = startLowerCase( stripPrefix( commandData.params[returnParams[0]].name, "p" ) );
        std::string secondDataVariable = startLowerCase( stripPrefix( commandData.params[returnParams[1]].name, "p" ) );

        std::string const dataDeclarationTemplate = R"(std::pair<${firstDataType},${secondDataType}> data_;
    ${firstDataType} & ${firstDataVariable} = data_.first;
    ${secondDataType} & ${secondDataVariable} = data_.second;)";

        return replaceWithMap( dataDeclarationTemplate,
                               { { "firstDataType", dataTypes[0] },
                                 { "firstDataVariable", firstDataVariable },
                                 { "secondDataType", dataTypes[1] },
                                 { "secondDataVariable", secondDataVariable } } );
      }
    case 1:
      assert( ( returnParams[0] == vectorParams.begin()->second.lenParam ) && ( returnParams[1] == vectorParams.begin()->first ) && !singular );
      {
        std::string counterVariable = startLowerCase( stripPrefix( commandData.params[returnParams[0]].name, "p" ) );
        if ( !chained )
        {
          std::string vectorAllocator = withAllocator ? ( "( " + startLowerCase( stripPrefix( dataTypes[1], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator )" ) : "";

          std::string const dataDeclarationTemplate = R"(${returnType} ${returnVariable}${vectorAllocator};
    ${counterType} ${counterVariable};)";

          return replaceWithMap( dataDeclarationTemplate,
                                 { { "counterType", dataTypes[0] },
                                   { "counterVariable", counterVariable },
                                   { "returnType", returnType },
                                   { "returnVariable", returnVariable },
                                   { "vectorAllocator", vectorAllocator } } );
        }
        else
        {
          std::string structureChainAllocator   = raii ? "" : ", StructureChainAllocator";
          std::string structureChainInitializer = withAllocator ? ( "( structureChainAllocator )" ) : "";
          std::string vectorVariable            = startLowerCase( stripPrefix( commandData.params[returnParams[1]].name, "p" ) );

          std::string const dataDeclarationTemplate =
            R"(std::vector<StructureChain${structureChainAllocator}> structureChains${structureChainInitializer};
    std::vector<${vectorElementType}> ${vectorVariable};
    ${counterType} ${counterVariable};)";

          return replaceWithMap( dataDeclarationTemplate,
                                 {
                                   { "counterType", dataTypes[0] },
                                   { "counterVariable", counterVariable },
                                   { "structureChainAllocator", structureChainAllocator },
                                   { "structureChainInitializer", structureChainInitializer },
                                   { "vectorElementType", dataTypes[1] },
                                   { "vectorVariable", vectorVariable },
                                 } );
        }
      }
      break;
    case 2:
      assert( ( returnParams[0] == std::next( vectorParams.begin() )->first ) && !vectorParams.contains( returnParams[1] ) && !chained );
      {
        std::string firstDataVariable  = startLowerCase( stripPrefix( commandData.params[returnParams[0]].name, "p" ) );
        std::string secondDataVariable = startLowerCase( stripPrefix( commandData.params[returnParams[1]].name, "p" ) );
        if ( singular )
        {
          firstDataVariable = stripPluralS( firstDataVariable );

          std::string const dataDeclarationTemplate = R"(std::pair<${firstDataType},${secondDataType}> data_;
    ${firstDataType} & ${firstDataVariable} = data_.first;
    ${secondDataType} & ${secondDataVariable} = data_.second;)";

          return replaceWithMap( dataDeclarationTemplate,
                                 { { "firstDataType", dataTypes[0] },
                                   { "firstDataVariable", firstDataVariable },
                                   { "secondDataType", dataTypes[1] },
                                   { "secondDataVariable", secondDataVariable } } );
        }
        else
        {
          std::string allocatorType       = raii ? "" : ( startUpperCase( stripPrefix( dataTypes[0], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator" );
          std::string allocateInitializer = withAllocator ? ( ", " + startLowerCase( allocatorType ) ) : "";
          if ( !raii )
          {
            allocatorType = ", " + allocatorType;
          }
          std::string vectorSize = startLowerCase( stripPrefix( commandData.params[vectorParams.begin()->first].name, "p" ) ) + ".size()";

          std::string const dataDeclarationTemplate =
            R"(std::pair<std::vector<${firstDataType}${allocatorType}>,${secondDataType}> data_( std::piecewise_construct, std::forward_as_tuple( ${vectorSize}${allocateInitializer} ), std::forward_as_tuple( 0 ) );
    std::vector<${firstDataType}${allocatorType}> & ${firstDataVariable} = data_.first;
    ${secondDataType} & ${secondDataVariable} = data_.second;)";

          return replaceWithMap( dataDeclarationTemplate,
                                 { { "allocateInitializer", allocateInitializer },
                                   { "allocatorType", allocatorType },
                                   { "firstDataType", dataTypes[0] },
                                   { "firstDataVariable", firstDataVariable },
                                   { "secondDataType", dataTypes[1] },
                                   { "secondDataVariable", secondDataVariable },
                                   { "vectorSize", vectorSize } } );
        }
      }
      break;
    default: assert( false ); return "";
  }
}

std::string VulkanHppGenerator::generateDataDeclarations3Returns( CommandData const &                       commandData,
                                                                  std::vector<size_t> const &               returnParams,
                                                                  std::map<size_t, VectorParamData> const & vectorParams,
                                                                  CommandFlavourFlags                       flavourFlags,
                                                                  bool                                      raii,
                                                                  std::vector<std::string> const &          dataTypes ) const
{
  switch ( vectorParams.size() )
  {
    case 1:
      {
        assert( ( returnParams[1] == vectorParams.begin()->second.lenParam ) && ( returnParams[2] == vectorParams.begin()->first ) );

        std::string valueVariable   = startLowerCase( stripPrefix( commandData.params[returnParams[0]].name, "p" ) );
        std::string counterVariable = startLowerCase( stripPrefix( commandData.params[returnParams[1]].name, "p" ) );
        std::string vectorVariable  = startLowerCase( stripPrefix( commandData.params[returnParams[2]].name, "p" ) );
        std::string vectorAllocatorType, pairConstructor;
        if ( !raii )
        {
          vectorAllocatorType = startUpperCase( stripPrefix( dataTypes[2], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator";
          pairConstructor =
            ( flavourFlags & CommandFlavourFlagBits::withAllocator )
              ? ( "( std::piecewise_construct, std::forward_as_tuple(), std::forward_as_tuple( " + startLowerCase( vectorAllocatorType ) + " ) )" )
              : "";
          vectorAllocatorType = ", " + vectorAllocatorType;
        }

        std::string const chainedDataDeclarationsTemplate =
          R"(std::pair<VULKAN_HPP_NAMESPACE::StructureChain<X,Y,Z...>,std::vector<${vectorElementType}${vectorAllocatorType}>> data_${pairConstructor};
    ${valueType} & ${valueVariable} = data_.first.template get<${valueType}>();
    std::vector<${vectorElementType}${vectorAllocatorType}> & ${vectorVariable} = data_.second;
    ${counterType} ${counterVariable};)";
        std::string const dataDeclarationsTemplate =
          R"(std::pair<${valueType},std::vector<${vectorElementType}${vectorAllocatorType}>> data_${pairConstructor};
    ${valueType} & ${valueVariable} = data_.first;
    std::vector<${vectorElementType}${vectorAllocatorType}> & ${vectorVariable} = data_.second;
    ${counterType} ${counterVariable};)";

        return replaceWithMap( ( flavourFlags & CommandFlavourFlagBits::chained ) ? chainedDataDeclarationsTemplate : dataDeclarationsTemplate,
                               { { "counterType", dataTypes[1] },
                                 { "counterVariable", counterVariable },
                                 { "vectorAllocatorType", vectorAllocatorType },
                                 { "vectorElementType", dataTypes[2] },
                                 { "vectorVariable", vectorVariable },
                                 { "pairConstructor", pairConstructor },
                                 { "valueType", dataTypes[0] },
                                 { "valueVariable", valueVariable } } );
      }
      break;
    case 2:
      {
        assert( !( flavourFlags & CommandFlavourFlagBits::chained ) );
        assert( ( returnParams[0] == vectorParams.begin()->second.lenParam ) && ( returnParams[1] == vectorParams.begin()->first ) &&
                ( returnParams[2] == std::next( vectorParams.begin() )->first ) && ( returnParams[0] == std::next( vectorParams.begin() )->second.lenParam ) );

        std::string counterVariable      = startLowerCase( stripPrefix( commandData.params[returnParams[0]].name, "p" ) );
        std::string firstVectorVariable  = startLowerCase( stripPrefix( commandData.params[returnParams[1]].name, "p" ) );
        std::string secondVectorVariable = startLowerCase( stripPrefix( commandData.params[returnParams[2]].name, "p" ) );
        std::string firstVectorAllocatorType, secondVectorAllocatorType, pairConstructor;
        if ( !raii )
        {
          firstVectorAllocatorType  = startUpperCase( stripPrefix( dataTypes[1], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator";
          secondVectorAllocatorType = startUpperCase( stripPrefix( dataTypes[2], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator";
          pairConstructor           = ( flavourFlags & CommandFlavourFlagBits::withAllocator )
                                      ? ( "( std::piecewise_construct, std::forward_as_tuple( " + startLowerCase( firstVectorAllocatorType ) +
                                " ), std::forward_as_tuple( " + startLowerCase( secondVectorAllocatorType ) + " ) )" )
                                      : "";
          firstVectorAllocatorType  = ", " + firstVectorAllocatorType;
          secondVectorAllocatorType = ", " + secondVectorAllocatorType;
        }

        std::string const dataDeclarationsTemplate =
          R"(std::pair<std::vector<${firstVectorElementType}${firstVectorAllocatorType}>, std::vector<${secondVectorElementType}${secondVectorAllocatorType}>> data_${pairConstructor};
    std::vector<${firstVectorElementType}${firstVectorAllocatorType}> & ${firstVectorVariable} = data_.first;
    std::vector<${secondVectorElementType}${secondVectorAllocatorType}> & ${secondVectorVariable} = data_.second;
    ${counterType} ${counterVariable};)";

        return replaceWithMap( dataDeclarationsTemplate,
                               { { "counterType", dataTypes[0] },
                                 { "counterVariable", counterVariable },
                                 { "firstVectorAllocatorType", firstVectorAllocatorType },
                                 { "firstVectorElementType", dataTypes[1] },
                                 { "firstVectorVariable", firstVectorVariable },
                                 { "pairConstructor", pairConstructor },
                                 { "secondVectorAllocatorType", secondVectorAllocatorType },
                                 { "secondVectorElementType", dataTypes[2] },
                                 { "secondVectorVariable", secondVectorVariable } } );
      }
    default: assert( false ); return "";
  }
}

std::string VulkanHppGenerator::generateDataPreparation( CommandData const &                       commandData,
                                                         size_t                                    initialSkipCount,
                                                         std::vector<size_t> const &               returnParams,
                                                         std::map<size_t, VectorParamData> const & vectorParams,
                                                         std::set<size_t> const &                  templatedParams,
                                                         CommandFlavourFlags                       flavourFlags,
                                                         bool                                      enumerating,
                                                         std::vector<std::string> const &          dataTypes ) const
{
  const bool chained  = flavourFlags & CommandFlavourFlagBits::chained;
  const bool singular = flavourFlags & CommandFlavourFlagBits::singular;
  const bool unique   = flavourFlags & CommandFlavourFlagBits::unique;

  auto vectorParamIt = ( 1 < returnParams.size() ) ? vectorParams.find( returnParams[1] ) : vectorParams.end();
  if ( vectorParamIt != vectorParams.end() )
  {
    assert( !unique );

    std::string vectorName = startLowerCase( stripPrefix( commandData.params[vectorParamIt->first].name, "p" ) );

    if ( chained )
    {
      assert( !singular );
      assert( templatedParams.empty() );
      assert( returnParams.size() == 2 );
      assert( !vectorParams.contains( returnParams[0] ) );
      assert( ( vectorParamIt != vectorParams.end() ) && ( vectorParamIt->second.lenParam == returnParams[0] ) );

      std::string vectorElementType = stripPostfix( commandData.params[vectorParamIt->first].type.compose( "VULKAN_HPP_NAMESPACE" ), " *" );

      if ( enumerating )
      {
        std::string const dataPreparationTemplate =
          R"(VULKAN_HPP_ASSERT( ${counterName} <= ${vectorName}.size() );
      if ( ${counterName} < ${vectorName}.size() )
      {
        structureChains.resize( ${counterName} );
      }
      for ( ${counterType} i = 0; i < ${counterName}; i++ )
      {
        structureChains[i].template get<${vectorElementType}>() = ${vectorName}[i];
      })";

        return replaceWithMap( dataPreparationTemplate,
                               { { "counterName", startLowerCase( stripPrefix( commandData.params[vectorParamIt->second.lenParam].name, "p" ) ) },
                                 { "counterType", commandData.params[vectorParamIt->second.lenParam].type.type },
                                 { "vectorElementType", vectorElementType },
                                 { "vectorName", vectorName } } );
      }
      else
      {
        std::string const dataPreparationTemplate =
          R"(for ( ${counterType} i = 0; i < ${counterName}; i++ )
    {
      structureChains[i].template get<${vectorElementType}>() = ${vectorName}[i];
    })";

        return replaceWithMap( dataPreparationTemplate,
                               { { "counterName", startLowerCase( stripPrefix( commandData.params[vectorParamIt->second.lenParam].name, "p" ) ) },
                                 { "counterType", commandData.params[vectorParamIt->second.lenParam].type.type },
                                 { "vectorElementType", vectorElementType },
                                 { "vectorName", vectorName } } );
      }
    }
    else if ( enumerating )
    {
      assert( !singular );
      assert( ( vectorParams.size() != 2 ) ||
              ( ( vectorParams.begin()->first == returnParams[1] ) && ( vectorParams.begin()->second.lenParam == returnParams[0] ) &&
                ( std::next( vectorParams.begin() )->first == returnParams[2] ) &&
                ( std::next( vectorParams.begin() )->second.lenParam == returnParams[0] ) ) );

      std::string resizes;
      for ( auto const & vp : vectorParams )
      {
        assert( std::any_of( returnParams.begin(), returnParams.end(), [&vp]( size_t rp ) { return rp == vp.first; } ) &&
                std::any_of( returnParams.begin(), returnParams.end(), [&vp]( size_t rp ) { return rp == vp.second.lenParam; } ) );
        resizes += startLowerCase( stripPrefix( commandData.params[vp.first].name, "p" ) ) + ".resize( " +
                   startLowerCase( stripPrefix( commandData.params[vp.second.lenParam].name, "p" ) ) + " );\n";
      }
      resizes.pop_back();

      std::string const dataPreparationTemplate =
        R"(VULKAN_HPP_ASSERT( ${counterName} <= ${vectorName}.size() );
    if ( ${counterName} < ${vectorName}.size() )
    {
      ${resizes}
    })";

      return replaceWithMap( dataPreparationTemplate,
                             { { "counterName", startLowerCase( stripPrefix( commandData.params[vectorParamIt->second.lenParam].name, "p" ) ) },
                               { "resizes", resizes },
                               { "vectorName", vectorName } } );
    }
  }
  else if ( unique && !singular && ( returnParams.size() == 1 ) && vectorParams.contains( returnParams[0] ) )
  {
    std::string className = initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "";
    std::string deleterDefinition, vectorName, vectorSize;
    vectorParamIt = vectorParams.find( returnParams[0] );
    if ( vectorParamIt != vectorParams.end() && vectorParamIt->second.byStructure )
    {
      deleterDefinition = "detail::ObjectDestroy<" + className + ", Dispatch> deleter( *this, allocator, d )";
      auto structIt     = m_structs.find( commandData.params[returnParams[0]].type.type );
      assert( structIt != m_structs.end() );
      vectorName = startLowerCase( stripPrefix( structIt->second.members.back().name, "p" ) );
      vectorSize = vectorName + ".size()";
    }
    else
    {
      std::vector<std::string> lenParts = tokenize( commandData.params[returnParams[0]].lenExpression, "->" );
      switch ( lenParts.size() )
      {
        case 1: deleterDefinition = "detail::ObjectDestroy<" + className + ", Dispatch> deleter( *this, allocator, d )"; break;
        case 2:
          {
            auto vpiIt = vectorParams.find( returnParams[0] );
            assert( vpiIt != vectorParams.end() );
            std::string poolType, poolName;
            std::tie( poolType, poolName ) = getPoolTypeAndName( commandData.params[vpiIt->second.lenParam].type.type );
            assert( !poolType.empty() );
            poolType          = stripPrefix( poolType, "Vk" );
            poolName          = startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + poolName;
            deleterDefinition = "detail::PoolFree<" + className + ", " + poolType + ", Dispatch> deleter( *this, " + poolName + ", d )";
          }
          break;
        default: assert( false ); break;
      }
      vectorName = startLowerCase( stripPrefix( commandData.params[returnParams[0]].name, "p" ) );
      vectorSize = getVectorSize( commandData.params, vectorParams, returnParams[0], commandData.params[returnParams[0]].type.type, templatedParams );
    }

    std::string handleType       = stripPrefix( dataTypes[0], "VULKAN_HPP_NAMESPACE::" );
    std::string vectorAllocator  = ( flavourFlags & CommandFlavourFlagBits::withAllocator ) ? ( "( " + startLowerCase( handleType ) + "Allocator )" ) : "";
    std::string elementName      = stripPluralS( vectorName );
    std::string uniqueVectorName = "unique" + startUpperCase( vectorName );

    std::string const dataPreparationTemplate =
      R"(std::vector<UniqueHandle<VULKAN_HPP_NAMESPACE::${handleType}, Dispatch>, ${handleType}Allocator> ${uniqueVectorName}${vectorAllocator};
    ${uniqueVectorName}.reserve( ${vectorSize} );
    ${deleterDefinition};
    for ( auto const & ${elementName} : ${vectorName} )
    {
      ${uniqueVectorName}.push_back( UniqueHandle<${handleType}, Dispatch>( ${elementName}, deleter ) );
    })";

    return replaceWithMap( dataPreparationTemplate,
                           { { "elementName", elementName },
                             { "deleterDefinition", deleterDefinition },
                             { "handleType", handleType },
                             { "uniqueVectorName", uniqueVectorName },
                             { "vectorAllocator", vectorAllocator },
                             { "vectorName", vectorName },
                             { "vectorSize", vectorSize } } );
  }
  return "";
}

std::string VulkanHppGenerator::generateDataSizeChecks( CommandData const &                       commandData,
                                                        std::vector<size_t> const &               returnParams,
                                                        std::vector<std::string> const &          returnParamTypes,
                                                        std::map<size_t, VectorParamData> const & vectorParams,
                                                        std::set<size_t> const &                  templatedParams,
                                                        bool                                      singular ) const
{
  assert( returnParams.size() == returnParamTypes.size() );
  std::string dataSizeChecks;
  if ( !singular )
  {
    const std::string dataSizeCheckTemplate = R"(    VULKAN_HPP_ASSERT( ${dataSize} % sizeof( ${dataType} ) == 0 );)";
    for ( size_t i = 0; i < returnParams.size(); i++ )
    {
      auto vectorParamIt = vectorParams.find( returnParams[i] );
      if ( ( vectorParamIt != vectorParams.end() ) && templatedParams.contains( returnParams[i] ) &&
           std::none_of( returnParams.begin(), returnParams.end(), [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->second.lenParam; } ) )
      {
        dataSizeChecks += replaceWithMap( dataSizeCheckTemplate,
                                          { { "dataSize", commandData.params[vectorParamIt->second.lenParam].name }, { "dataType", returnParamTypes[i] } } );
      }
    }
  }

  return dataSizeChecks;
}

std::string VulkanHppGenerator::generateDebugReportObjectType( std::string const & objectType ) const
{
  std::string debugReportObjectType = objectType;
  debugReportObjectType             = debugReportObjectType.replace( 3, 0, "DEBUG_REPORT_" ) + "_EXT";
  auto enumIt                       = m_enums.find( "VkDebugReportObjectTypeEXT" );
  assert( enumIt != m_enums.end() );
  return contains( enumIt->second.values, debugReportObjectType ) ? generateEnumValueName( enumIt->first, debugReportObjectType, false ) : "eUnknown";
}

std::string VulkanHppGenerator::generateDecoratedReturnType( CommandData const &                       commandData,
                                                             std::vector<size_t> const &               returnParams,
                                                             std::map<size_t, VectorParamData> const & vectorParams,
                                                             CommandFlavourFlags                       flavourFlags,
                                                             bool                                      raii,
                                                             std::string const &                       returnType ) const
{
  const bool chained = flavourFlags & CommandFlavourFlagBits::chained;
#if !defined( NDEBUG )
  const bool unique = flavourFlags & CommandFlavourFlagBits::unique;
#endif
  assert( !( chained && unique ) );

  std::string decoratedReturnType;
  if ( ( 1 < commandData.successCodes.size() ) && returnParams.empty() && !chained )
  {
    assert( ( commandData.returnType == "VkResult" ) && !unique );
    decoratedReturnType = "VULKAN_HPP_NAMESPACE::Result";
  }
  else if ( ( commandData.returnType != "VkResult" ) && ( commandData.returnType != "void" ) )
  {
    assert( returnParams.empty() && !chained && !unique );
    if ( commandData.returnType.starts_with( "Vk" ) )
    {
      decoratedReturnType = generateNamespacedType( commandData.returnType );
    }
    else if ( commandData.returnType.starts_with( "PFN_vk" ) )
    {
      decoratedReturnType = "VULKAN_HPP_NAMESPACE::PFN_" + stripPrefix( commandData.returnType, "PFN_vk" );
    }
    else
    {
      decoratedReturnType = commandData.returnType;
    }
  }
  else if ( ( commandData.returnType == "void" ) ||
            ( ( commandData.returnType == "VkResult" ) && ( commandData.successCodes.size() == 1 ) && ( commandData.errorCodes.empty() || raii ) ) )
  {
    assert( !unique );
    assert( ( commandData.returnType != "void" ) || ( returnParams.size() <= 2 ) );
    decoratedReturnType = returnType;
  }
  else
  {
    assert( commandData.returnType == "VkResult" );
    assert( !commandData.successCodes.empty() && ( commandData.successCodes[0] == "VK_SUCCESS" ) );
    if ( ( commandData.successCodes.size() == 1 ) || ( ( commandData.successCodes.size() == 2 ) && ( commandData.successCodes[1] == "VK_INCOMPLETE" ) ) )
    {
      decoratedReturnType = raii ? returnType : ( "typename ResultValueType<" + returnType + ">::type" );
    }
    else if ( !commandData.errorCodes.empty() && ( 1 < commandData.successCodes.size() ) &&
              ( ( returnParams.size() == 1 ) ||
                ( ( returnParams.size() == 2 ) && ( vectorParams.empty() || ( ( vectorParams.size() == 1 ) && ( commandData.successCodes.size() == 3 ) &&
                                                                              ( commandData.successCodes[1] == "VK_INCOMPLETE" ) ) ) ) ) )
    {
      decoratedReturnType = ( raii ? "std::pair<VULKAN_HPP_NAMESPACE::Result, " : "ResultValue<" ) + returnType + ">";
    }
    else
    {
      assert( false );
    }
  }
  return decoratedReturnType;
}

std::string VulkanHppGenerator::generateDeprecatedConstructors( std::string const & name ) const
{
  std::string str;
  if ( name == "VkAllocationCallbacks" )
  {
    // To be removed around December 2025
    str = R"(
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic push
#  if defined( __clang__ )
#    pragma clang diagnostic ignored "-Wunknown-warning-option"
#  endif
#  pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
    VULKAN_HPP_DEPRECATED( "This constructor is deprecated. Use the one taking function pointer types from the vk-namespace instead." )
    AllocationCallbacks( void *                               pUserData_,
                         PFN_vkAllocationFunction             pfnAllocation_,
                         PFN_vkReallocationFunction           pfnReallocation_       = {},
                         PFN_vkFreeFunction                   pfnFree_               = {},
                         PFN_vkInternalAllocationNotification pfnInternalAllocation_ = {},
                         PFN_vkInternalFreeNotification       pfnInternalFree_       = {} ) VULKAN_HPP_NOEXCEPT
      : AllocationCallbacks( pUserData_,
                             reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_AllocationFunction>( pfnAllocation_ ),
                             reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_ReallocationFunction>( pfnReallocation_ ),
                             reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_FreeFunction>( pfnFree_ ),
                             reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_InternalAllocationNotification>( pfnInternalAllocation_ ),
                             reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_InternalFreeNotification>( pfnInternalFree_ ) )
    {
    }
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic pop
#endif
)";
  }
  else if ( name == "VkDebugReportCallbackCreateInfoEXT" )
  {
    // To be removed around December 2025
    str = R"(
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic push
#  if defined( __clang__ )
#    pragma clang diagnostic ignored "-Wunknown-warning-option"
#  endif
#  pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
    VULKAN_HPP_DEPRECATED( "This constructor is deprecated. Use the one taking function pointer types from the vk-namespace instead." )
    DebugReportCallbackCreateInfoEXT( VULKAN_HPP_NAMESPACE::DebugReportFlagsEXT flags_,
                                      PFN_vkDebugReportCallbackEXT              pfnCallback_,
                                      void *                                    pUserData_   = {},
                                      const void *                              pNext_       = nullptr ) VULKAN_HPP_NOEXCEPT
      : DebugReportCallbackCreateInfoEXT( flags_, reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_DebugReportCallbackEXT>( pfnCallback_ ), pUserData_, pNext_ )
    {
    }
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic pop
#endif
)";
  }
  else if ( name == "VkDebugUtilsMessengerCreateInfoEXT" )
  {
    // To be removed around December 2025
    str = R"(
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic push
#  if defined( __clang__ )
#    pragma clang diagnostic ignored "-Wunknown-warning-option"
#  endif
#  pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
    VULKAN_HPP_DEPRECATED( "This constructor is deprecated. Use the one taking function pointer types from the vk-namespace instead." )
    DebugUtilsMessengerCreateInfoEXT( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateFlagsEXT flags_,
                                      VULKAN_HPP_NAMESPACE::DebugUtilsMessageSeverityFlagsEXT messageSeverity_,
                                      VULKAN_HPP_NAMESPACE::DebugUtilsMessageTypeFlagsEXT     messageType_,
                                      PFN_vkDebugUtilsMessengerCallbackEXT                    pfnUserCallback_,
                                      void *                                                  pUserData_       = {},
                                      const void *                                            pNext_ = nullptr ) VULKAN_HPP_NOEXCEPT
      : DebugUtilsMessengerCreateInfoEXT( flags_, messageSeverity_, messageType_, reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_DebugUtilsMessengerCallbackEXT>( pfnUserCallback_ ), pUserData_, pNext_ )
    {
    }
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic pop
#endif
)";
  }
  else if ( name == "VkDeviceDeviceMemoryReportCreateInfoEXT" )
  {
    // To be removed around December 2025
    str = R"(
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic push
#  if defined( __clang__ )
#    pragma clang diagnostic ignored "-Wunknown-warning-option"
#  endif
#  pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
    VULKAN_HPP_DEPRECATED( "This constructor is deprecated. Use the one taking function pointer types from the vk-namespace instead." )
    DeviceDeviceMemoryReportCreateInfoEXT( VULKAN_HPP_NAMESPACE::DeviceMemoryReportFlagsEXT flags_,
                                           PFN_vkDeviceMemoryReportCallbackEXT              pfnUserCallback_,
                                           void *                                           pUserData_       = {},
                                           const void *                                     pNext_ = nullptr ) VULKAN_HPP_NOEXCEPT
      : DeviceDeviceMemoryReportCreateInfoEXT( flags_, reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_DeviceMemoryReportCallbackEXT>( pfnUserCallback_ ), pUserData_, pNext_ )
    {
    }
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic pop
#endif
)";
  }
  else if ( name == "DirectDriverLoadingInfoLUNARG" )
  {
    // To be removed around December 2025
    str = R"(
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic push
#  if defined( __clang__ )
#    pragma clang diagnostic ignored "-Wunknown-warning-option"
#  endif
#  pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
    VULKAN_HPP_DEPRECATED( "This constructor is deprecated. Use the one taking function pointer types from the vk-namespace instead." )
    DirectDriverLoadingInfoLUNARG( VULKAN_HPP_NAMESPACE::DirectDriverLoadingFlagsLUNARG flags_,
                                   PFN_vkGetInstanceProcAddrLUNARG                      pfnGetInstanceProcAddr_,
                                   void *                                               pNext_ = nullptr ) VULKAN_HPP_NOEXCEPT
      : DirectDriverLoadingInfoLUNARG( flags_, reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_GetInstanceProcAddrLUNARG>( pfnGetInstanceProcAddr_ ), pNext_ )
    {
    }
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic pop
#endif
)";
  }
  return str;
}

std::string VulkanHppGenerator::generateDeprecatedStructSetters( std::string const & name ) const
{
  std::string str;
  if ( name == "VkAllocationCallbacks" )
  {
    // To be removed around December 2025
    str = R"(
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic push
#  if defined( __clang__ )
#    pragma clang diagnostic ignored "-Wunknown-warning-option"
#  endif
#  pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
    VULKAN_HPP_DEPRECATED( "This setter is deprecated. Use the one taking a function pointer type from the vk-namespace instead." )
    AllocationCallbacks & setPfnAllocation( PFN_vkAllocationFunction pfnAllocation_ ) VULKAN_HPP_NOEXCEPT
    {
      return setPfnAllocation( reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_AllocationFunction>( pfnAllocation_ ) );
    }

    VULKAN_HPP_DEPRECATED( "This setter is deprecated. Use the one taking a function pointer type from the vk-namespace instead." )
    AllocationCallbacks & setPfnReallocation( PFN_vkReallocationFunction pfnReallocation_ ) VULKAN_HPP_NOEXCEPT
    {
      return setPfnReallocation( reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_ReallocationFunction>( pfnReallocation_ ) );
    }

    VULKAN_HPP_DEPRECATED( "This setter is deprecated. Use the one taking a function pointer type from the vk-namespace instead." )
    AllocationCallbacks & setPfnInternalAllocation( PFN_vkInternalAllocationNotification pfnInternalAllocation_ ) VULKAN_HPP_NOEXCEPT
    {
      return setPfnInternalAllocation( reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_InternalAllocationNotification>( pfnInternalAllocation_ ) );
    }

    VULKAN_HPP_DEPRECATED( "This setter is deprecated. Use the one taking a function pointer type from the vk-namespace instead." )
    AllocationCallbacks & setPfnInternalFree( PFN_vkInternalFreeNotification pfnInternalFree_ ) VULKAN_HPP_NOEXCEPT
    {
      return setPfnInternalFree( reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_InternalFreeNotification>( pfnInternalFree_ ) );
    }
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic pop
#endif
)";
  }
  else if ( name == "VkDebugReportCallbackCreateInfoEXT" )
  {
    // To be removed around December 2025
    str = R"(
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic push
#  if defined( __clang__ )
#    pragma clang diagnostic ignored "-Wunknown-warning-option"
#  endif
#  pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
    VULKAN_HPP_DEPRECATED( "This setter is deprecated. Use the one taking a function pointer type from the vk-namespace instead." )
    DebugReportCallbackCreateInfoEXT & setPfnCallback( PFN_vkDebugReportCallbackEXT pfnCallback_ ) VULKAN_HPP_NOEXCEPT
    {
      return setPfnCallback( reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_DebugReportCallbackEXT>( pfnCallback_ ) );
    }
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic pop
#endif
)";
  }
  else if ( name == "VkDebugUtilsMessengerCreateInfoEXT" )
  {
    // To be removed around December 2025
    str = R"(
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic push
#  if defined( __clang__ )
#    pragma clang diagnostic ignored "-Wunknown-warning-option"
#  endif
#  pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
    VULKAN_HPP_DEPRECATED( "This setter is deprecated. Use the one taking a function pointer type from the vk-namespace instead." )
    DebugUtilsMessengerCreateInfoEXT & setPfnUserCallback( PFN_vkDebugUtilsMessengerCallbackEXT pfnUserCallback_ ) VULKAN_HPP_NOEXCEPT
    {
      return setPfnUserCallback( reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_DebugUtilsMessengerCallbackEXT>( pfnUserCallback_ ) );
    }
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic pop
#endif
)";
  }
  else if ( name == "VkDeviceDeviceMemoryReportCreateInfoEXT" )
  {
    // To be removed around December 2025
    str = R"(
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic push
#  if defined( __clang__ )
#    pragma clang diagnostic ignored "-Wunknown-warning-option"
#  endif
#  pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
    VULKAN_HPP_DEPRECATED( "This setter is deprecated. Use the one taking a function pointer type from the vk-namespace instead." )
    DeviceDeviceMemoryReportCreateInfoEXT & setPfnUserCallback( PFN_vkDeviceMemoryReportCallbackEXT pfnUserCallback_ ) VULKAN_HPP_NOEXCEPT
    {
      return setPfnUserCallback( reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_DeviceMemoryReportCallbackEXT>( pfnUserCallback_ ) );
    }
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic pop
#endif
)";
  }
  else if ( name == "DirectDriverLoadingInfoLUNARG" )
  {
    // To be removed around December 2025
    str = R"(
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic push
#  if defined( __clang__ )
#    pragma clang diagnostic ignored "-Wunknown-warning-option"
#  endif
#  pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
    VULKAN_HPP_DEPRECATED( "This setter is deprecated. Use the one taking a function pointer type from the vk-namespace instead." )
    DirectDriverLoadingInfoLUNARG & setPfnGetInstanceProcAddr( PFN_vkGetInstanceProcAddrLUNARG pfnGetInstanceProcAddr_ ) VULKAN_HPP_NOEXCEPT
    {
      return setPfnGetInstanceProcAddr( reinterpret_cast<VULKAN_HPP_NAMESPACE::PFN_GetInstanceProcAddLUNARG>( pfnGetInstanceProcAddr_ ) );
    }
#if defined( __clang__ ) || defined( __GNUC__ )
#  pragma GCC diagnostic pop
#endif
)";
  }
  return str;
}

std::string VulkanHppGenerator::generateDispatchLoaderDynamic() const
{
  const std::string dispatchLoaderDynamicTemplate = R"(
  using PFN_dummy = void ( * )();

  class DispatchLoaderDynamic : public DispatchLoaderBase
  {
  public:
${commandMembers}

  public:
    DispatchLoaderDynamic() VULKAN_HPP_NOEXCEPT = default;
    DispatchLoaderDynamic( DispatchLoaderDynamic const & rhs ) VULKAN_HPP_NOEXCEPT = default;

    DispatchLoaderDynamic(PFN_vkGetInstanceProcAddr getInstanceProcAddr) VULKAN_HPP_NOEXCEPT
    {
      init(getInstanceProcAddr);
    }

    // This interface does not require a linked vulkan library.
    DispatchLoaderDynamic( VkInstance                instance,
                           PFN_vkGetInstanceProcAddr getInstanceProcAddr,
                           VkDevice                  device            = {},
                           PFN_vkGetDeviceProcAddr   getDeviceProcAddr = nullptr ) VULKAN_HPP_NOEXCEPT
    {
      init( instance, getInstanceProcAddr, device, getDeviceProcAddr );
    }

    template <typename DynamicLoader
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
      = VULKAN_HPP_NAMESPACE::detail::DynamicLoader
#endif
    >
    void init()
    {
      static DynamicLoader dl;
      init( dl );
    }

    template <typename DynamicLoader>
    void init( DynamicLoader const & dl ) VULKAN_HPP_NOEXCEPT
    {
      PFN_vkGetInstanceProcAddr getInstanceProcAddr = dl.template getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
      init( getInstanceProcAddr );
    }

    void init( PFN_vkGetInstanceProcAddr getInstanceProcAddr ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT(getInstanceProcAddr);

      vkGetInstanceProcAddr = getInstanceProcAddr;

${initialCommandAssignments}
    }

    // This interface does not require a linked vulkan library.
    void init( VkInstance                instance,
               PFN_vkGetInstanceProcAddr getInstanceProcAddr,
               VkDevice                  device              = {},
               PFN_vkGetDeviceProcAddr /*getDeviceProcAddr*/ = nullptr ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT(instance && getInstanceProcAddr);
      vkGetInstanceProcAddr = getInstanceProcAddr;
      init( VULKAN_HPP_NAMESPACE::Instance(instance) );
      if (device)
      {
        init( VULKAN_HPP_NAMESPACE::Device(device) );
      }
    }

    void init( VULKAN_HPP_NAMESPACE::Instance instanceCpp ) VULKAN_HPP_NOEXCEPT
    {
      VkInstance instance = static_cast<VkInstance>(instanceCpp);

${instanceCommandAssignments}
    }

    void init( VULKAN_HPP_NAMESPACE::Device deviceCpp ) VULKAN_HPP_NOEXCEPT
    {
      VkDevice device = static_cast<VkDevice>(deviceCpp);

${deviceCommandAssignments}
    }

    template <typename DynamicLoader>
    void init(VULKAN_HPP_NAMESPACE::Instance const & instance, VULKAN_HPP_NAMESPACE::Device const & device, DynamicLoader const & dl) VULKAN_HPP_NOEXCEPT
    {
      PFN_vkGetInstanceProcAddr getInstanceProcAddr = dl.template getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
      PFN_vkGetDeviceProcAddr getDeviceProcAddr = dl.template getProcAddress<PFN_vkGetDeviceProcAddr>("vkGetDeviceProcAddr");
      init(static_cast<VkInstance>(instance), getInstanceProcAddr, static_cast<VkDevice>(device), device ? getDeviceProcAddr : nullptr);
    }

    template <typename DynamicLoader
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
      = VULKAN_HPP_NAMESPACE::detail::DynamicLoader
#endif
    >
    void init(VULKAN_HPP_NAMESPACE::Instance const & instance, VULKAN_HPP_NAMESPACE::Device const & device) VULKAN_HPP_NOEXCEPT
    {
      static DynamicLoader dl;
      init(instance, device, dl);
    }
  };)";

  std::string           commandMembers, deviceCommandAssignments, initialCommandAssignments, instanceCommandAssignments;
  std::set<std::string> listedCommands;  // some commands are listed with more than one extension!
  for ( auto const & feature : m_features )
  {
    appendDispatchLoaderDynamicCommands(
      feature.requireData, listedCommands, feature.name, commandMembers, initialCommandAssignments, instanceCommandAssignments, deviceCommandAssignments );
  }
  for ( auto const & extension : m_extensions )
  {
    appendDispatchLoaderDynamicCommands(
      extension.requireData, listedCommands, extension.name, commandMembers, initialCommandAssignments, instanceCommandAssignments, deviceCommandAssignments );
  }

  return replaceWithMap( dispatchLoaderDynamicTemplate,
                         { { "commandMembers", commandMembers },
                           { "deviceCommandAssignments", deviceCommandAssignments },
                           { "initialCommandAssignments", initialCommandAssignments },
                           { "instanceCommandAssignments", instanceCommandAssignments } } );
}

std::string VulkanHppGenerator::generateDispatchLoaderStatic() const
{
  const std::string dispatchLoaderStaticTemplate = R"(
#if !defined( VK_NO_PROTOTYPES ) || ( defined( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC ) && ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 0 ) )
  class DispatchLoaderStatic : public DispatchLoaderBase
  {
  public:
${commands}
  };

  inline DispatchLoaderStatic & getDispatchLoaderStatic()
  {
    static DispatchLoaderStatic dls;
    return dls;
  }
#endif
)";

  std::string           commands;
  std::set<std::string> listedCommands;
  for ( auto const & feature : m_features )
  {
    commands += generateDispatchLoaderStaticCommands( feature.requireData, listedCommands, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    commands += generateDispatchLoaderStaticCommands( extension.requireData, listedCommands, extension.name );
  }

  return replaceWithMap( dispatchLoaderStaticTemplate, { { "commands", commands } } );
}

std::string VulkanHppGenerator::generateDestroyCommand( std::string const & name, CommandData const & commandData ) const
{
  // special handling for destroy functions, filter out alias functions
  std::string commandName = generateCommandName( name, commandData.params, 1 );
  if ( m_commands.contains( name ) && ( ( ( name.substr( 2, 7 ) == "Destroy" ) && ( commandName != "destroy" ) ) || ( name.substr( 2, 4 ) == "Free" ) ||
                                        ( name == "vkReleasePerformanceConfigurationINTEL" ) ) )
  {
    assert( 1 < commandData.params.size() );
    // make sure, the object to destroy/free/release is not optional in the shortened version!
    CommandData localCommandData        = commandData;
    localCommandData.params[1].optional = false;

    std::string destroyCommandString = generateCommand( name, localCommandData, 1, false, false );
    std::string shortenedName;
    if ( name.substr( 2, 7 ) == "Destroy" )
    {
      shortenedName = "destroy";
    }
    else if ( name.substr( 2, 4 ) == "Free" )
    {
      // enclose "free" in parenthesis to prevent interference with MSVC debug free
      shortenedName = "( free )";
    }
    else
    {
      assert( name == "vkReleasePerformanceConfigurationINTEL" );
      shortenedName = "release";
    }
    size_t pos = destroyCommandString.find( commandName );
    while ( pos != std::string::npos )
    {
      destroyCommandString.replace( pos, commandName.length(), shortenedName );
      pos = destroyCommandString.find( commandName, pos );
    }
    // we need to remove the default argument for the first argument, to prevent ambiguities!
    assert( 1 < localCommandData.params.size() );
    pos = destroyCommandString.find( localCommandData.params[1].name );  // skip the standard version of the function
    assert( pos != std::string::npos );
    pos = destroyCommandString.find( localCommandData.params[1].name,
                                     pos + 1 );  // get the argument to destroy in the advanced version
    assert( pos != std::string::npos );
    pos = destroyCommandString.find( " VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT", pos );
    if ( pos != std::string::npos )
    {
      destroyCommandString.erase( pos, strlen( " VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT" ) );
    }
    return "\n" + destroyCommandString;
  }
  return "";
}

std::string VulkanHppGenerator::generateDispatchLoaderDynamicCommandAssignment( std::string const & commandName,
                                                                                std::string const & aliasName,
                                                                                std::string const & firstArg ) const
{
  if ( commandName == "vkGetInstanceProcAddr" )
  {
    // Don't overwite vkGetInstanceProcAddr with NULL.
    return "";
  }
  std::string str = "      " + commandName + " = PFN_" + commandName + "( vkGet" + ( ( firstArg == "device" ) ? "Device" : "Instance" ) + "ProcAddr( " +
                    firstArg + ", \"" + commandName + "\" ) );\n";
  // if this is an alias'ed function, use it as a fallback for the original one
  if ( commandName != aliasName )
  {
    str += "      if ( !" + aliasName + " ) " + aliasName + " = " + commandName + ";\n";
  }
  return str;
}

std::string VulkanHppGenerator::generateDispatchLoaderStaticCommands( std::vector<RequireData> const & requireData,
                                                                      std::set<std::string> &          listedCommands,
                                                                      std::string const &              title ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & command : require.commands )
    {
      // some commands are listed for multiple extensions !
      if ( listedCommands.insert( command.name ).second )
      {
        auto commandIt = findByNameOrAlias( m_commands, command.name );
        assert( commandIt != m_commands.end() );

        str += "\n";
        std::string parameterList, parameters;
        assert( !commandIt->second.params.empty() );
        for ( auto param : commandIt->second.params )
        {
          parameterList += param.type.compose( "" ) + " " + param.name + generateCArraySizes( param.arraySizes ) + ", ";
          parameters += param.name + ", ";
        }
        assert( parameterList.ends_with( ", " ) && parameters.ends_with( ", " ) );
        parameterList.resize( parameterList.size() - 2 );
        parameters.resize( parameters.size() - 2 );

        const std::string commandTemplate = R"(
    ${returnType} ${commandName}( ${parameterList} ) const VULKAN_HPP_NOEXCEPT
    {
      return ::${commandName}( ${parameters} );
    }
)";

        str += replaceWithMap( commandTemplate,
                               { { "commandName", command.name },
                                 { "parameterList", parameterList },
                                 { "parameters", parameters },
                                 { "returnType", commandIt->second.returnType } } );
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateEnum( std::pair<std::string, EnumData> const & enumData, std::string const & surroundingProtect ) const
{
  std::string baseType, bitmask;
  if ( enumData.second.isBitmask )
  {
    auto bitmaskIt =
      std::find_if( m_bitmasks.begin(), m_bitmasks.end(), [&enumData]( auto const & bitmask ) { return bitmask.second.require == enumData.first; } );
    assert( bitmaskIt != m_bitmasks.end() );
    baseType = " : " + bitmaskIt->first;
    bitmask  = generateBitmask( bitmaskIt, surroundingProtect );
  }

  std::string                        enumValues, previousEnter, previousLeave;
  std::map<std::string, std::string> valueToNameMap;
  for ( auto const & value : enumData.second.values )
  {
    if ( value.supported )
    {
      std::string valueName = generateEnumValueName( enumData.first, value.name, enumData.second.isBitmask );
      checkForError( valueToNameMap.insert( { valueName, value.name } ).second,
                     value.xmlLine,
                     "generated enum value name <" + valueName + "> already listed for enum <" + enumData.first + ">" );

      // if the value's protect differs from the surrounding protect, generate protection code
      std::string enter, leave;
      if ( !value.protect.empty() && ( value.protect != surroundingProtect ) )
      {
        tie( enter, leave ) = generateProtection( value.protect );
      }
      if ( previousEnter != enter )
      {
        enumValues += previousLeave + enter;
      }
      enumValues += "    " + valueName;
      if ( value.deprecated )
      {
        enumValues += " VULKAN_HPP_DEPRECATED_17( \"" + valueName + " is deprecated, but no reason was given in the API XML\" )";
      }
      enumValues += " = " + value.name + ",\n";

      for ( auto const & valueAlias : value.aliases )
      {
        // only generate supported aliases
        if ( valueAlias.supported )
        {
          std::string enumName = enumData.first;
          for ( auto aliasIt = enumData.second.aliases.begin(); ( aliasIt != enumData.second.aliases.end() ) && ( enumName == enumData.first ); ++aliasIt )
          {
            auto        enumAliasIt = enumData.second.aliases.begin();
            std::string enumTag     = findTag( enumData.first );
            std::string aliasTag    = findTag( enumAliasIt->first );
            std::string valueTag    = findTag( valueAlias.name );
            if ( ( stripPostfix( enumData.first, enumTag ) == stripPostfix( enumAliasIt->first, aliasTag ) ) && ( aliasTag == valueTag ) )
            {
              enumName = enumAliasIt->first;
            }
          }

          std::string                                        aliasName = generateEnumValueName( enumName, valueAlias.name, enumData.second.isBitmask );
          std::map<std::string, std::string>::const_iterator mapIt;
          bool                                               inserted;
          std::tie( mapIt, inserted ) = valueToNameMap.insert( { aliasName, valueAlias.name } );
          if ( inserted )
          {
            enumValues += "    " + aliasName + " = " + valueAlias.name + ",\n";
          }
          else
          {
            // some aliases are so close to the original, that no new entry can be generated!
            assert( mapIt->second != valueAlias.name );
            checkForError( ( mapIt->second == value.name ) ||
                             std::any_of( value.aliases.begin(), value.aliases.end(), [mapIt]( auto const & eav ) { return eav.name == mapIt->second; } ),
                           valueAlias.xmlLine,
                           "generated enum alias value name <" + aliasName + ">, generated from <" + valueAlias.name +
                             "> is already generated from different enum value <" + mapIt->second + ">" );
          }
        }
      }

      previousEnter = enter;
      previousLeave = leave;
    }
  }
  enumValues += previousLeave;

  if ( !enumValues.empty() )
  {
    const size_t pos = enumValues.rfind( ',' );
    assert( pos != std::string::npos );
    enumValues.erase( pos, 1 );
    enumValues = "\n" + enumValues + "  ";
  }

  std::string enumUsing;
  for ( auto const & alias : enumData.second.aliases )
  {
    enumUsing += "  using " + stripPrefix( alias.first, "Vk" ) + " = " + stripPrefix( enumData.first, "Vk" ) + ";\n";
  }

  std::string typeTraits;
  if ( enumData.first == "VkIndexType" )
  {
    typeTraits = generateIndexTypeTraits( enumData );
  }
  else if ( enumData.first == "VkLayerSettingTypeEXT" )
  {
    typeTraits = generateLayerSettingTypeTraits();
  }

  const std::string enumTemplate = R"(  enum class ${enumName}${baseType}
  {${enumValues}};
${typeTraits}${enumUsing}${bitmask})";

  return replaceWithMap( enumTemplate,
                         { { "baseType", baseType },
                           { "bitmask", bitmask },
                           { "enumName", stripPrefix( enumData.first, "Vk" ) },
                           { "enumUsing", enumUsing },
                           { "enumValues", enumValues },
                           { "typeTraits", typeTraits } } );
}

std::string VulkanHppGenerator::generateEnums() const
{
  const std::string enumsTemplate = R"(
  //=============
  //=== ENUMs ===
  //=============

${enums}
)";

  std::string           enums;
  std::set<std::string> listedEnums;
  for ( auto const & feature : m_features )
  {
    enums += generateEnums( feature.requireData, listedEnums, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    enums += generateEnums( extension.requireData, listedEnums, extension.name );
  }

  return replaceWithMap( enumsTemplate, { { "enums", enums } } );
}

std::string
  VulkanHppGenerator::generateEnums( std::vector<RequireData> const & requireData, std::set<std::string> & listedEnums, std::string const & title ) const
{
  std::string surroundingProtect = getProtectFromTitle( title );
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto enumIt = m_enums.find( type.name );
      if ( ( enumIt != m_enums.end() ) && !listedEnums.contains( type.name ) )
      {
        listedEnums.insert( type.name );
        str += "\n";
        str += generateEnum( *enumIt, surroundingProtect );
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateEnumsToString() const
{
  // start with toHexString, which is used in all the to_string functions here!
  const std::string enumsToStringTemplate = R"(
  //=======================
  //=== ENUMs to_string ===
  //=======================

  VULKAN_HPP_INLINE std::string toHexString( uint32_t value )
  {
#if __cpp_lib_format
    return std::format( "{:x}", value );
#else
    std::stringstream stream;
    stream << std::hex << value;
    return stream.str();
#endif
  }

${enumsToString}
)";

  std::string           enumsToString;
  std::set<std::string> listedEnums;
  for ( auto const & feature : m_features )
  {
    enumsToString += generateEnumsToString( feature.requireData, listedEnums, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    enumsToString += generateEnumsToString( extension.requireData, listedEnums, extension.name );
  }

  return replaceWithMap( enumsToStringTemplate, { { "enumsToString", enumsToString } } );
}

std::string VulkanHppGenerator::generateEnumsToString( std::vector<RequireData> const & requireData,
                                                       std::set<std::string> &          listedEnums,
                                                       std::string const &              title ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto enumIt = m_enums.find( type.name );
      if ( ( enumIt != m_enums.end() ) && !listedEnums.contains( type.name ) )
      {
        listedEnums.insert( type.name );

        str += "\n";
        str += generateEnumToString( *enumIt );
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateEnumInitializer( TypeInfo const &                   type,
                                                         std::vector<std::string> const &   arraySizes,
                                                         std::vector<EnumValueData> const & values,
                                                         bool                               bitmask ) const
{
  // enum arguments might need special initialization
  assert( type.prefix.empty() && !values.empty() );
  std::string valueName = generateEnumValueName( type.type, values.front().name, bitmask );
  std::string value     = generateNamespacedType( type.type ) + "::" + valueName;
  std::string str;
  if ( arraySizes.empty() )
  {
    str += value;
  }
  else
  {
    assert( arraySizes.size() == 1 );
    auto      constIt = m_constants.find( arraySizes[0] );
    const int count   = std::stoi( ( constIt == m_constants.end() ) ? arraySizes[0] : constIt->second.value );
    assert( 1 < count );
    str += "{ { " + value;
    for ( int i = 1; i < count; i++ )
    {
      str += ", " + value;
    }
    str += " } }";
  }
  return str;
}

std::string VulkanHppGenerator::generateEnumToString( std::pair<std::string, EnumData> const & enumData ) const
{
  std::string enumName = stripPrefix( enumData.first, "Vk" );
  std::string functionBody;
  bool        isEmpty = enumData.second.values.empty() ||
                 std::none_of( enumData.second.values.begin(), enumData.second.values.end(), []( auto const & evd ) { return evd.supported; } );
  if ( isEmpty )
  {
    functionBody = R"x(    return "(void)";)x";
  }
  else
  {
    std::string cases, previousEnter, previousLeave;
    for ( auto const & value : enumData.second.values )
    {
      if ( value.supported )
      {
        const auto [enter, leave] = generateProtection( value.protect );
        if ( previousEnter != enter )
        {
          cases += previousLeave + enter;
        }

        const std::string caseTemplate = R"(      case ${enumName}::e${valueName} : return "${valueName}";
)";
        cases += replaceWithMap(
          caseTemplate,
          { { "enumName", enumName }, { "valueName", generateEnumValueName( enumData.first, value.name, enumData.second.isBitmask ).substr( 1 ) } } );

        previousEnter = enter;
        previousLeave = leave;
      }
    }
    cases += previousLeave;

    const std::string functionBodyTemplate =
      R"x(    switch ( value )
    {
${cases}      default: return "invalid ( " + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
)x";

    functionBody = replaceWithMap( functionBodyTemplate, { { "cases", cases } } );
  }

  const std::string enumToStringTemplate = R"(
  VULKAN_HPP_INLINE std::string to_string( ${enumName}${argument} )
  {
${functionBody}
  }
)";

  return replaceWithMap( enumToStringTemplate, { { "argument", isEmpty ? "" : " value" }, { "enumName", enumName }, { "functionBody", functionBody } } );
}

std::pair<std::string, std::string> VulkanHppGenerator::generateEnumSuffixes( std::string const & name, bool bitmask ) const
{
  std::string prefix, postfix;
  if ( name == "VkResult" )
  {
    prefix = "VK_";
  }
  else
  {
    if ( bitmask )
    {
      // for a bitmask enum, start with "VK", cut off the trailing "FlagBits", and convert that name to upper case
      // end that with "Bit"
      const size_t pos = name.find( "FlagBits" );
      assert( pos != std::string::npos );
      std::string shortenedName = name;
      shortenedName.erase( pos, strlen( "FlagBits" ) );
      std::string tag = findTag( shortenedName );
      prefix          = toUpperCase( stripPostfix( shortenedName, tag ) ) + "_";
    }
    else
    {
      // for a non-bitmask enum, convert the name to upper case
      prefix = toUpperCase( name ) + "_";
    }

    // if the enum name contains a tag move it from the prefix to the postfix to generate correct enum value
    // names.
    for ( auto const & tag : m_tags )
    {
      if ( prefix.ends_with( tag.first + "_" ) )
      {
        prefix.erase( prefix.length() - tag.first.length() - 1 );
        postfix = "_" + tag.first;
        break;
      }
      else if ( name.ends_with( tag.first ) )
      {
        postfix = "_" + tag.first;
        break;
      }
    }
  }

  return std::make_pair( prefix, postfix );
}

std::string VulkanHppGenerator::generateEnumValueName( std::string const & enumName, std::string const & valueName, bool bitmask ) const
{
  std::string prefix, postfix;
  std::tie( prefix, postfix ) = generateEnumSuffixes( enumName, bitmask );
  std::string tag             = findTag( valueName, "" );
  if ( postfix == "_" + tag )
  {
    tag = findTag( valueName, postfix );
  }

  std::string result = "e" + toCamelCase( stripPostfix( stripPrefix( valueName, prefix ), postfix ) );
  if ( bitmask )
  {
    const size_t pos = result.rfind( "Bit" );
    if ( pos != std::string::npos )
    {
      result.erase( pos, 3 );
    }
  }
  if ( !tag.empty() && ( result.substr( result.length() - tag.length() ) == toCamelCase( tag ) ) )
  {
    result = result.substr( 0, result.length() - tag.length() ) + tag;
  }
  return result;
}

std::string VulkanHppGenerator::generateExtensionDependencies() const
{
  std::string extensionDependencies, previousEnter, previousLeave;
  for ( auto const & extension : m_extensions )
  {
    if ( !extension.depends.empty() )
    {
      std::string dependsPerExtension = "{ \"" + extension.name + "\", { ";
      for ( auto const & dependsByVersion : extension.depends )
      {
        dependsPerExtension += "{ \"" + dependsByVersion.first + "\", { ";
        if ( !dependsByVersion.second.empty() )
        {
          dependsPerExtension += " { ";
          for ( auto const & dependsSet : dependsByVersion.second )
          {
            for ( auto const & depends : dependsSet )
            {
              dependsPerExtension += "\"" + depends + "\", ";
            }
          }
          dependsPerExtension += " }, ";
          assert( dependsPerExtension.ends_with( ", " ) );
          dependsPerExtension = dependsPerExtension.substr( 0, dependsPerExtension.length() - 2 );
        }
        dependsPerExtension += " } }, ";
      }
      assert( dependsPerExtension.ends_with( ", " ) );
      dependsPerExtension = dependsPerExtension.substr( 0, dependsPerExtension.length() - 2 );
      dependsPerExtension += " } }, ";

      const auto [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
      extensionDependencies += ( ( previousEnter != enter ) ? ( "\n" + previousLeave + enter ) : "\n" ) + dependsPerExtension;
      previousEnter = enter;
      previousLeave = leave;
    }
  }
  assert( extensionDependencies.ends_with( ", " ) );
  extensionDependencies = extensionDependencies.substr( 0, extensionDependencies.length() - 2 );

  if ( !previousLeave.empty() )
  {
    extensionDependencies += "\n" + previousLeave;
  }
  return extensionDependencies;
}

std::string VulkanHppGenerator::generateExtensionDependsByVersion( bool definition ) const
{
  if ( m_api != "vulkan" )
  {
    return "";
  }

  if ( definition )
  {
    const std::string generateExtensionDependsTemplate =
      R"(  VULKAN_HPP_INLINE std::pair<bool, std::vector<std::vector<std::string>> const &> getExtensionDepends( std::string const & version, std::string const & extension )
    {
#if !defined( NDEBUG )
      static std::set<std::string> versions = { ${versions} };
      assert( versions.find( version ) != versions.end() );
#endif
      static std::vector<std::vector<std::string>> noDependencies;

      std::map<std::string, std::vector<std::vector<std::string>>> const & dependencies = getExtensionDepends( extension );
      if ( dependencies.empty() )
      {
        return { true, noDependencies };
      }
      auto depIt = dependencies.lower_bound( version );
      if ( ( depIt == dependencies.end() ) || ( depIt->first != version ) )
      {
        depIt = std::prev( depIt );
      }
      if ( depIt == dependencies.end() )
      {
        return { false, noDependencies };
      }
      else
      {
        return { true, depIt->second };
      }
    }
)";

    std::string versions;
    for ( auto const & feature : m_features )
    {
      versions += "\"" + feature.name + "\", ";
    }
    assert( versions.ends_with( ", " ) );
    versions = versions.substr( 0, versions.length() - 2 );

    return replaceWithMap( generateExtensionDependsTemplate, { { "versions", versions } } );
  }
  else
  {
    return "std::pair<bool, std::vector<std::vector<std::string>> const &> getExtensionDepends( std::string const & version, std::string const & extension );";
  }
}

template <class Predicate, class Extraction>
std::string VulkanHppGenerator::generateExtensionReplacedBy( Predicate p, Extraction e ) const
{
  std::string replacedBy, previousEnter, previousLeave;
  for ( auto const & extension : m_extensions )
  {
    if ( p( extension ) )
    {
      const auto [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
      replacedBy += ( ( previousEnter != enter ) ? ( "\n" + previousLeave + enter ) : "\n" ) + "  if ( extension == \"" + extension.name + "\" ) { return \"" +
                    e( extension ) + "\"; }";
      previousEnter = enter;
      previousLeave = leave;
    }
  }
  if ( !previousLeave.empty() )
  {
    replacedBy += "\n" + previousLeave;
  }
  replacedBy += "\n  return \"\";";
  return replacedBy;
}

template <class Predicate>
std::string VulkanHppGenerator::generateExtensionReplacedTest( Predicate p ) const
{
  std::string replacedTest, previousEnter, previousLeave;
  bool        unprotectedEntry = false;
  for ( auto const & extension : m_extensions )
  {
    if ( p( extension ) )
    {
      const auto [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
      unprotectedEntry |= enter.empty();
      replacedTest += ( ( previousEnter != enter ) ? ( "\n" + previousLeave + enter ) : "\n" ) + "( extension == \"" + extension.name + "\" ) || ";
      previousEnter = enter;
      previousLeave = leave;
    }
  }
  if ( unprotectedEntry )
  {
    assert( replacedTest.ends_with( " || " ) );
    replacedTest = replacedTest.substr( 0, replacedTest.length() - 4 );
  }
  if ( !previousLeave.empty() )
  {
    replacedTest += "\n" + previousLeave;
  }
  if ( !unprotectedEntry )
  {
    replacedTest += "false";  // there might be no replacements at all, so add a "false" at the end...
  }
  return replacedTest;
}

std::string VulkanHppGenerator::generateExtensionsList( std::string const & type ) const
{
  std::string extensionsList, previousEnter, previousLeave;
  for ( auto const & extension : m_extensions )
  {
    if ( extension.type == type )
    {
      const auto [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
      extensionsList += ( ( previousEnter != enter ) ? ( "\n" + previousLeave + enter ) : "\n" ) + "\"" + extension.name + "\", ";
      previousEnter = enter;
      previousLeave = leave;
    }
  }
  assert( extensionsList.ends_with( ", " ) );
  extensionsList = extensionsList.substr( 0, extensionsList.length() - 2 );
  if ( !previousLeave.empty() )
  {
    extensionsList += "\n" + previousLeave;
  }
  return extensionsList;
}

std::string VulkanHppGenerator::generateExtensionTypeTest( std::string const & type ) const
{
  std::string typeTest, previousEnter, previousLeave;
  bool        first = true;
  for ( auto const & extension : m_extensions )
  {
    if ( extension.type == type )
    {
      const auto [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
      typeTest +=
        ( ( previousEnter != enter ) ? ( "\n" + previousLeave + enter ) : "\n" ) + ( first ? "" : " || " ) + "( extension == \"" + extension.name + "\" )";
      previousEnter = enter;
      previousLeave = leave;
      first         = false;
    }
  }
  if ( !previousLeave.empty() )
  {
    typeTest += "\n" + previousLeave;
  }
  return typeTest;
}

std::string VulkanHppGenerator::generateFormatTraits() const
{
  if ( m_formats.empty() )
  {
    return "";
  }

  const std::string formatTraitsTemplate = R"(
  //=====================
  //=== Format Traits ===
  //=====================

  // The three-dimensional extent of a texel block.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 std::array<uint8_t, 3> blockExtent( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${blockExtentCases}
      default: return {{1, 1, 1 }};
    }
  }

  // The texel block size in bytes.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t blockSize( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${blockSizeCases}
      default : VULKAN_HPP_ASSERT( false ); return 0;
    }
  }

  // The class of the format (can't be just named "class"!)
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 char const * compatibilityClass( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${classCases}
      default : VULKAN_HPP_ASSERT( false ); return "";
    }
  }

  // The number of bits in this component, if not compressed, otherwise 0.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t componentBits( VULKAN_HPP_NAMESPACE::Format format, uint8_t component )
  {
    switch( format )
    {
${componentBitsCases}
      default: return 0;
    }
  }

  // The number of components of this format.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t componentCount( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${componentCountCases}
      default: return 0;
    }
  }

  // The name of the component
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 char const * componentName( VULKAN_HPP_NAMESPACE::Format format, uint8_t component )
  {
    switch( format )
    {
${componentNameCases}
      default: return "";
    }
  }

  // The numeric format of the component
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 char const * componentNumericFormat( VULKAN_HPP_NAMESPACE::Format format, uint8_t component )
  {
    switch( format )
    {
${componentNumericFormatCases}
      default: return "";
    }
  }

  // The plane this component lies in.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t componentPlaneIndex( VULKAN_HPP_NAMESPACE::Format format, uint8_t component )
  {
    switch( format )
    {
${componentPlaneIndexCases}
      default: return 0;
    }
  }

  // True, if the components of this format are compressed, otherwise false.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool componentsAreCompressed( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${componentsAreCompressedCases}
        return true;
      default: return false;
    }
  }

  // A textual description of the compression scheme, or an empty string if it is not compressed
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 char const * compressionScheme( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${compressionSchemeCases}
      default: return "";
    }
  }

  // True, if this format is a compressed one.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool isCompressed( VULKAN_HPP_NAMESPACE::Format format )
  {
    return ( *VULKAN_HPP_NAMESPACE::compressionScheme( format ) != 0 );
  }

  // The number of bits into which the format is packed. A single image element in this format
  // can be stored in the same space as a scalar type of this bit width.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t packed( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${packedCases}
      default: return 0;
    }
  }

  // The single-plane format that this plane is compatible with.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 VULKAN_HPP_NAMESPACE::Format planeCompatibleFormat( VULKAN_HPP_NAMESPACE::Format format, uint8_t plane )
  {
    switch( format )
    {
${planeCompatibleCases}
      default: VULKAN_HPP_ASSERT( plane == 0 ); return format;
    }
  }

  // The number of image planes of this format.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t planeCount( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${planeCountCases}
      default: return 1;
    }
  }

  // The relative height of this plane. A value of k means that this plane is 1/k the height of the overall format.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t planeHeightDivisor( VULKAN_HPP_NAMESPACE::Format format, uint8_t plane )
  {
    switch( format )
    {
${planeHeightDivisorCases}
      default: VULKAN_HPP_ASSERT( plane == 0 ); return 1;
    }
  }

  // The relative width of this plane. A value of k means that this plane is 1/k the width of the overall format.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t planeWidthDivisor( VULKAN_HPP_NAMESPACE::Format format, uint8_t plane )
  {
    switch( format )
    {
${planeWidthDivisorCases}
      default: VULKAN_HPP_ASSERT( plane == 0 ); return 1;
    }
  }

  // The number of texels in a texel block.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t texelsPerBlock( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${texelsPerBlockCases}
      default: VULKAN_HPP_ASSERT( false ); return 0;
    }
  }
)";

  auto formatIt = m_enums.find( "VkFormat" );
  assert( formatIt != m_enums.end() );
  assert( formatIt->second.values.front().name == "VK_FORMAT_UNDEFINED" );

  std::string blockSizeCases, blockExtentCases, classCases, componentBitsCases, componentCountCases, componentNameCases, componentNumericFormatCases,
    componentPlaneIndexCases, componentsAreCompressedCases, compressionSchemeCases, packedCases, planeCompatibleCases, planeCountCases, planeHeightDivisorCases,
    planeWidthDivisorCases, texelsPerBlockCases;
  for ( auto formatValuesIt = std::next( formatIt->second.values.begin() ); formatValuesIt != formatIt->second.values.end(); ++formatValuesIt )
  {
    if ( formatValuesIt->supported )
    {
      auto traitIt = m_formats.find( formatValuesIt->name );
      assert( traitIt != m_formats.end() );
      std::string caseString = "      case VULKAN_HPP_NAMESPACE::Format::" + generateEnumValueName( "VkFormat", traitIt->first, false ) + ":";

      blockSizeCases += caseString + " return " + traitIt->second.blockSize + ";\n";

      if ( !traitIt->second.blockExtent.empty() )
      {
        std::vector<std::string> blockExtent = tokenize( traitIt->second.blockExtent, "," );
        assert( blockExtent.size() == 3 );
        blockExtentCases += caseString + " return {{ " + blockExtent[0] + ", " + blockExtent[1] + ", " + blockExtent[2] + " }};\n";
      }

      classCases += caseString + " return \"" + traitIt->second.classAttribute + "\";\n";

      if ( traitIt->second.components.front().bits != "compressed" )
      {
        const std::string componentBitsCaseTemplate = R"(${caseString}
        switch( component )
        {
${componentCases}
          default: VULKAN_HPP_ASSERT( false ); return 0;
        }
)";

        std::string componentCases;
        for ( size_t i = 0; i < traitIt->second.components.size(); ++i )
        {
          componentCases += "          case " + std::to_string( i ) + ": return " + traitIt->second.components[i].bits + ";\n";
        }
        componentCases.pop_back();
        componentBitsCases += replaceWithMap( componentBitsCaseTemplate, { { "caseString", caseString }, { "componentCases", componentCases } } );
      }

      componentCountCases += caseString + " return " + std::to_string( traitIt->second.components.size() ) + ";\n";

      {
        const std::string componentNameCaseTemplate = R"(${caseString}
        switch( component )
        {
${componentCases}
          default: VULKAN_HPP_ASSERT( false ); return "";
        }
)";

        std::string componentCases;
        for ( size_t i = 0; i < traitIt->second.components.size(); ++i )
        {
          componentCases += "          case " + std::to_string( i ) + ": return \"" + traitIt->second.components[i].name + "\";\n";
        }
        componentCases.pop_back();
        componentNameCases += replaceWithMap( componentNameCaseTemplate, { { "caseString", caseString }, { "componentCases", componentCases } } );
      }

      {
        const std::string componentNumericFormatCaseTemplate = R"(${caseString}
        switch( component )
        {
${componentCases}
          default: VULKAN_HPP_ASSERT( false ); return "";
        }
)";

        std::string componentCases;
        for ( size_t i = 0; i < traitIt->second.components.size(); ++i )
        {
          componentCases += "          case " + std::to_string( i ) + ": return \"" + traitIt->second.components[i].numericFormat + "\";\n";
        }
        componentCases.pop_back();
        componentNumericFormatCases +=
          replaceWithMap( componentNumericFormatCaseTemplate, { { "caseString", caseString }, { "componentCases", componentCases } } );
      }

      if ( !traitIt->second.components.front().planeIndex.empty() )
      {
        const std::string componentPlaneIndexCaseTemplate = R"(${caseString}
        switch( component )
        {
${componentCases}
          default: VULKAN_HPP_ASSERT( false ); return 0;
        }
)";

        std::string componentCases;
        for ( size_t i = 0; i < traitIt->second.components.size(); ++i )
        {
          componentCases += "          case " + std::to_string( i ) + ": return " + traitIt->second.components[i].planeIndex + ";\n";
        }
        componentCases.pop_back();
        componentPlaneIndexCases += replaceWithMap( componentPlaneIndexCaseTemplate, { { "caseString", caseString }, { "componentCases", componentCases } } );
      }

      if ( traitIt->second.components.front().bits == "compressed" )
      {
        componentsAreCompressedCases += caseString + "\n";
      }

      if ( !traitIt->second.compressed.empty() )
      {
        compressionSchemeCases += caseString + " return \"" + traitIt->second.compressed + "\";\n";
      }

      if ( !traitIt->second.packed.empty() )
      {
        packedCases += caseString + " return " + traitIt->second.packed + ";\n";
      }

      if ( !traitIt->second.planes.empty() )
      {
        const std::string planeCompatibleCaseTemplate = R"(${caseString}
        switch( plane )
        {
${compatibleCases}
          default: VULKAN_HPP_ASSERT( false ); return VULKAN_HPP_NAMESPACE::Format::eUndefined;
        }
)";

        const std::string planeHeightDivisorCaseTemplate = R"(${caseString}
        switch( plane )
        {
${heightDivisorCases}
          default: VULKAN_HPP_ASSERT( false ); return 1;
        }
)";

        const std::string planeWidthDivisorCaseTemplate = R"(${caseString}
        switch( plane )
        {
${widthDivisorCases}
          default: VULKAN_HPP_ASSERT( false ); return 1;
        }
)";

        std::string compatibleCases, heightDivisorCases, widthDivisorCases;
        for ( size_t i = 0; i < traitIt->second.planes.size(); ++i )
        {
          compatibleCases += "          case " + std::to_string( i ) +
                             ": return VULKAN_HPP_NAMESPACE::Format::" + generateEnumValueName( "VkFormat", traitIt->second.planes[i].compatible, false ) +
                             ";\n";
          heightDivisorCases += "          case " + std::to_string( i ) + ": return " + traitIt->second.planes[i].heightDivisor + ";\n";
          widthDivisorCases += "          case " + std::to_string( i ) + ": return " + traitIt->second.planes[i].widthDivisor + ";\n";
        }
        compatibleCases.pop_back();
        heightDivisorCases.pop_back();
        widthDivisorCases.pop_back();

        planeCompatibleCases += replaceWithMap( planeCompatibleCaseTemplate, { { "caseString", caseString }, { "compatibleCases", compatibleCases } } );

        planeCountCases += caseString + " return " + std::to_string( traitIt->second.planes.size() ) + ";\n";

        planeHeightDivisorCases +=
          replaceWithMap( planeHeightDivisorCaseTemplate, { { "caseString", caseString }, { "heightDivisorCases", heightDivisorCases } } );

        planeWidthDivisorCases += replaceWithMap( planeWidthDivisorCaseTemplate, { { "caseString", caseString }, { "widthDivisorCases", widthDivisorCases } } );
      }

      texelsPerBlockCases += caseString + " return " + traitIt->second.texelsPerBlock + ";\n";
    }
  }

  return replaceWithMap( formatTraitsTemplate,
                         { { "blockExtentCases", blockExtentCases },
                           { "blockSizeCases", blockSizeCases },
                           { "classCases", classCases },
                           { "componentBitsCases", componentBitsCases },
                           { "componentCountCases", componentCountCases },
                           { "componentNameCases", componentNameCases },
                           { "componentNumericFormatCases", componentNumericFormatCases },
                           { "componentPlaneIndexCases", componentPlaneIndexCases },
                           { "componentsAreCompressedCases", componentsAreCompressedCases },
                           { "compressionSchemeCases", compressionSchemeCases },
                           { "packedCases", packedCases },
                           { "planeCompatibleCases", planeCompatibleCases },
                           { "planeCountCases", planeCountCases },
                           { "planeHeightDivisorCases", planeHeightDivisorCases },
                           { "planeWidthDivisorCases", planeWidthDivisorCases },
                           { "texelsPerBlockCases", texelsPerBlockCases } } );
}

std::string VulkanHppGenerator::generateFuncPointer( std::pair<std::string, FuncPointerData> const & funcPointer, std::set<std::string> & listedStructs ) const
{
  std::string str;
  for ( auto const & argument : funcPointer.second.arguments )
  {
    auto typeIt = m_types.find( argument.type.type );
    assert( typeIt != m_types.end() );
    if ( ( typeIt->second.category == TypeCategory::Struct ) || ( typeIt->second.category == TypeCategory::Union ) )
    {
      auto structIt = findByNameOrAlias( m_structs, argument.type.type );
      assert( structIt != m_structs.end() );
      if ( !listedStructs.contains( argument.type.type ) )
      {
        str += generateStruct( *structIt, listedStructs );
      }
    }
    else
    {
      assert( typeIt->second.category != TypeCategory::FuncPointer );
    }
  }

  const auto [enter, leave] = generateProtection( getProtectFromType( funcPointer.first ) );

  std::string funcPointerArguments;
  if ( !funcPointer.second.arguments.empty() )
  {
    for ( auto const & argument : funcPointer.second.arguments )
    {
      funcPointerArguments += argument.type.compose( "VULKAN_HPP_NAMESPACE" ) + " " + argument.name + ", ";
    }
    assert( !funcPointerArguments.empty() );
    funcPointerArguments.pop_back();
    funcPointerArguments.pop_back();
  }

  static const std::string funcPointerTemplate = R"(
  typedef ${returnType} (VKAPI_PTR *PFN_${funcPointerName})( ${funcPointerArguments} );
)";

  str += "\n" + enter +
         replaceWithMap( funcPointerTemplate,
                         { { "funcPointerArguments", funcPointerArguments },
                           { "funcPointerName", stripPrefix( funcPointer.first, "PFN_vk" ) },
                           { "returnType", funcPointer.second.returnType.compose( "VULKAN_HPP_NAMESPACE" ) } } ) +
         leave;

  listedStructs.insert( funcPointer.first );
  return str;
}

std::string VulkanHppGenerator::generateFuncPointerReturns() const
{
  std::string           str;
  std::set<std::string> listedFuncPointers;
  for ( auto const & handle : m_handles )
  {
    for ( auto const & command : handle.second.commands )
    {
      auto commandIt = findByNameOrAlias( m_commands, command );
      assert( commandIt != m_commands.end() );
      auto funcPointerIt = m_funcPointers.find( commandIt->second.returnType );
      if ( ( funcPointerIt != m_funcPointers.end() ) && !listedFuncPointers.contains( commandIt->second.returnType ) )
      {
        assert( funcPointerIt->second.arguments.empty() );
        str += generateFuncPointer( *funcPointerIt, listedFuncPointers );
      }
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateFunctionPointerCheck( std::string const & function, std::set<std::string> const & requiredBy, bool raii ) const
{
  std::string functionPointerCheck;
  if ( !requiredBy.empty() )
  {
    std::string message = "Function <" + function + "> requires <" + *requiredBy.begin() + ">";
    for ( auto it = std::next( requiredBy.begin() ); it != requiredBy.end(); ++it )
    {
      message += " or <" + *it + ">";
    }

    functionPointerCheck = std::string( "VULKAN_HPP_ASSERT( " ) + ( raii ? "getDispatcher()->" : "d." ) + function + " && \"" + message + "\" );";
  }
  return functionPointerCheck;
}

std::string VulkanHppGenerator::generateHandle( std::pair<std::string, HandleData> const & handleData, std::set<std::string> & listedHandles ) const
{
  assert( !listedHandles.contains( handleData.first ) );

  // first check for any handle that needs to be listed before this one
  std::string str = generateHandleDependencies( handleData, listedHandles );

  // list the commands of this handle
  if ( handleData.first.empty() )
  {
    // the free functions, not bound to any handle
    str += generateHandleEmpty( handleData.second );
  }
  else
  {
    // list all the commands that are mapped to members of this class
    std::string commands = generateHandleCommandDeclarations( handleData.second.commands );

    std::string debugReportObjectType = generateDebugReportObjectType( handleData.second.objTypeEnum );

    // create CPPType template specialization
    std::string className = stripPrefix( handleData.first, "Vk" );
    std::string cppType;
    if ( debugReportObjectType != "eUnknown" )
    {
      static const std::string cppTypeFromDebugReportObjectTypeEXTTemplate = R"(
  template <>
  struct CppType<VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT, VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::e${className}>
  {
    using Type = VULKAN_HPP_NAMESPACE::${className};
  };
)";
      cppType = replaceWithMap( cppTypeFromDebugReportObjectTypeEXTTemplate, { { "className", className } } );
    }

    auto [enter, leave] = generateProtection( getProtectFromType( handleData.first ) );

    assert( !handleData.second.objTypeEnum.empty() );
    auto enumIt = m_enums.find( "VkObjectType" );
    assert( enumIt != m_enums.end() );
    assert( contains( enumIt->second.values, handleData.second.objTypeEnum ) );

    std::string usingAlias;
    for ( auto const & alias : handleData.second.aliases )
    {
      usingAlias += "  using " + stripPrefix( alias.first, "Vk" ) + " = " + stripPrefix( handleData.first, "Vk" ) + ";\n";
    }

    const std::string typesafeExplicitKeyword          = handleData.second.isDispatchable ? "" : "VULKAN_HPP_TYPESAFE_EXPLICIT ";
    const std::string typesafeConversionConditional    = handleData.second.isDispatchable ? "" : "#if ( VULKAN_HPP_TYPESAFE_CONVERSION == 1 )\n";
    const std::string typesafeConversionConditionalEnd = handleData.second.isDispatchable ? "" : "#endif\n";

    static const std::string templateString = R"(
${enter}  class ${className}
  {
  public:
    using CType = Vk${className};
    using NativeType = Vk${className};

    static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::${objTypeEnum};
    static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType = VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::${debugReportObjectType};

  public:
    ${className}() VULKAN_HPP_NOEXCEPT {};    // = default - try to workaround a compiler issue
    ${className}( ${className} const & rhs ) = default;
    ${className} & operator=( ${className} const & rhs ) = default;

#if !defined(VULKAN_HPP_HANDLES_MOVE_EXCHANGE)
    ${className}( ${className} && rhs ) = default;
    ${className} & operator=( ${className} && rhs ) = default;
#else
    ${className}( ${className} && rhs ) VULKAN_HPP_NOEXCEPT
      : m_${memberName}( VULKAN_HPP_NAMESPACE::exchange( rhs.m_${memberName}, {} ) )
    {}
    ${className} & operator=( ${className} && rhs ) VULKAN_HPP_NOEXCEPT
    {
      m_${memberName} = VULKAN_HPP_NAMESPACE::exchange( rhs.m_${memberName}, {} );
      return *this;
    }
#endif

    VULKAN_HPP_CONSTEXPR ${className}( std::nullptr_t ) VULKAN_HPP_NOEXCEPT
    {}
    ${typesafeExplicitKeyword}${className}( Vk${className} ${memberName} ) VULKAN_HPP_NOEXCEPT
      : m_${memberName}( ${memberName} )
    {}

${typesafeConversionConditional}    ${className} & operator=(Vk${className} ${memberName}) VULKAN_HPP_NOEXCEPT
    {
      m_${memberName} = ${memberName};
      return *this;
    }
${typesafeConversionConditionalEnd}
    ${className} & operator=( std::nullptr_t ) VULKAN_HPP_NOEXCEPT
    {
      m_${memberName} = {};
      return *this;
    }

    ${commands}
    ${typesafeExplicitKeyword}operator Vk${className}() const VULKAN_HPP_NOEXCEPT
    {
      return m_${memberName};
    }

    explicit operator bool() const VULKAN_HPP_NOEXCEPT
    {
      return m_${memberName} != VK_NULL_HANDLE;
    }

    bool operator!() const VULKAN_HPP_NOEXCEPT
    {
      return m_${memberName} == VK_NULL_HANDLE;
    }

  private:
    Vk${className} m_${memberName} = {};
  };

  template <>
  struct CppType<VULKAN_HPP_NAMESPACE::ObjectType, VULKAN_HPP_NAMESPACE::ObjectType::${objTypeEnum}>
  {
    using Type = VULKAN_HPP_NAMESPACE::${className};
  };

${CppType}

#if ( VK_USE_64_BIT_PTR_DEFINES == 1 )
  template <>
  struct CppType<Vk${className}, VK_NULL_HANDLE>
  {
    using Type = VULKAN_HPP_NAMESPACE::${className};
  };
#endif

  template <>
  struct isVulkanHandleType<VULKAN_HPP_NAMESPACE::${className}>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool value = true;
  };
${usingAlias}${leave})";

    str += replaceWithMap( templateString,
                           { { "className", className },
                             { "commands", commands },
                             { "CppType", cppType },
                             { "debugReportObjectType", debugReportObjectType },
                             { "enter", enter },
                             { "leave", leave },
                             { "memberName", startLowerCase( stripPrefix( handleData.first, "Vk" ) ) },
                             { "objTypeEnum", generateEnumValueName( enumIt->first, handleData.second.objTypeEnum, false ) },
                             { "usingAlias", usingAlias },
                             { "typesafeExplicitKeyword", typesafeExplicitKeyword },
                             { "typesafeConversionConditional", typesafeConversionConditional },
                             { "typesafeConversionConditionalEnd", typesafeConversionConditionalEnd } } );
  }

  listedHandles.insert( handleData.first );
  return str;
}

std::string VulkanHppGenerator::generateHandleCommandDeclarations( std::set<std::string> const & commands ) const
{
  std::string           str;
  std::set<std::string> listedCommands;  // some commands are listed with more than one extension!
  for ( auto const & feature : m_features )
  {
    std::vector<std::string> commandNames = selectCommandsByHandle( feature.requireData, commands, listedCommands );
    if ( !commandNames.empty() )
    {
      str += "\n  //=== " + feature.name + " ===\n";
      for ( auto const & command : commandNames )
      {
        auto commandIt = m_commands.find( command );
        assert( commandIt != m_commands.end() );

        str += "\n";
        str += generateCommand( commandIt->first, commandIt->second, 1, false, false );
        str += generateDestroyCommand( commandIt->first, commandIt->second );
      }
    }
  }
  for ( auto const & extension : m_extensions )
  {
    std::vector<std::string> commandNames = selectCommandsByHandle( extension.requireData, commands, listedCommands );
    if ( !commandNames.empty() )
    {
      const auto [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
      str += "\n" + enter + "  //=== " + extension.name + " ===\n";
      for ( auto const & command : commandNames )
      {
        auto commandIt = findByNameOrAlias( m_commands, command );
        assert( commandIt != m_commands.end() );

        std::string commandString;
        std::string commandName = generateCommandName( command, commandIt->second.params, 1 );
        str += "\n";
        str += generateCommand( command, commandIt->second, 1, false, false );
        str += generateDestroyCommand( command, commandIt->second );
      }
      str += leave;
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateHandleDependencies( std::pair<std::string, HandleData> const & handleData, std::set<std::string> & listedHandles ) const
{
  std::string str;
  for ( auto const & command : handleData.second.commands )
  {
    auto commandIt = findByNameOrAlias( m_commands, command );
    assert( commandIt != m_commands.end() );
    for ( auto const & parameter : commandIt->second.params )
    {
      auto handleIt = m_handles.find( parameter.type.type );
      if ( ( handleIt != m_handles.end() ) && ( parameter.type.type != handleData.first ) && !listedHandles.contains( parameter.type.type ) )
      {
        str += generateHandle( *handleIt, listedHandles );
      }
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateHandleEmpty( HandleData const & handleData ) const
{
  std::string           str;
  std::set<std::string> listedCommands;  // some commands are listed with more than one extension !
  for ( auto const & feature : m_features )
  {
    std::vector<std::string> commands = selectCommandsByHandle( feature.requireData, handleData.commands, listedCommands );
    if ( !commands.empty() )
    {
      str += "\n  //=== "s + feature.name + " ===\n";
      for ( auto const & command : commands )
      {
        auto commandIt = m_commands.find( command );
        assert( commandIt != m_commands.end() );
        str += "\n"s + generateCommand( commandIt->first, commandIt->second, 0, false, false );
      }
    }
  }
#if !defined( NDEBUG )
  for ( auto const & extension : m_extensions )
  {
    assert( selectCommandsByHandle( extension.requireData, handleData.commands, listedCommands ).empty() );
  }
#endif
  return str;
}

std::string VulkanHppGenerator::generateHandleForwardDeclarations() const
{
  const std::string fowardDeclarationsTemplate = R"(
  //===================================
  //=== HANDLE forward declarations ===
  //===================================

${forwardDeclarations}
)";

  std::string forwardDeclarations;
  for ( auto const & feature : m_features )
  {
    forwardDeclarations += generateHandleForwardDeclarations( feature.requireData, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    forwardDeclarations += generateHandleForwardDeclarations( extension.requireData, extension.name );
  }

  return replaceWithMap( fowardDeclarationsTemplate, { { "forwardDeclarations", forwardDeclarations } } );
}

std::string VulkanHppGenerator::generateHandleForwardDeclarations( std::vector<RequireData> const & requireData, std::string const & title ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto handleIt = m_handles.find( type.name );
      if ( handleIt != m_handles.end() )
      {
        str += "class " + stripPrefix( handleIt->first, "Vk" ) + ";\n";
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateHandleHashStructures( std::vector<RequireData> const & requireData, std::string const & title ) const
{
  const std::string hashTemplate = R"(
  template <> struct hash<VULKAN_HPP_NAMESPACE::${type}>
  {
    std::size_t operator()(VULKAN_HPP_NAMESPACE::${type} const & ${name}) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<Vk${type}>{}(static_cast<Vk${type}>(${name}));
    }
  };
)";

  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto handleIt = m_handles.find( type.name );
      if ( handleIt != m_handles.end() )
      {
        std::string handleType = stripPrefix( handleIt->first, "Vk" );
        std::string handleName = startLowerCase( handleType );
        str += replaceWithMap( hashTemplate, { { "name", handleName }, { "type", handleType } } );
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateHandleHashStructures() const
{
  const std::string hashesTemplate = R"(
  //===================================
  //=== HASH structures for handles ===
  //===================================

${hashes}
)";

  std::string hashes;
  for ( auto const & feature : m_features )
  {
    hashes += generateHandleHashStructures( feature.requireData, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    hashes += generateHandleHashStructures( extension.requireData, extension.name );
  }
  return replaceWithMap( hashesTemplate, { { "hashes", hashes } } );
}

std::string VulkanHppGenerator::generateHandles() const
{
  // Note: reordering structs or handles by features and extensions is not possible!
  std::string str = R"(
  //===============
  //=== HANDLEs ===
  //===============

  template <typename Type>
  struct isVulkanHandleType
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool value = false;
  };
)";

  std::set<std::string> listedHandles;
  str += generateHandle( *m_handles.begin(), listedHandles );  // artificial handle without a name!
  for ( auto handleIt = std::next( m_handles.begin() ); handleIt != m_handles.end(); ++handleIt )
  {
    if ( !listedHandles.contains( handleIt->first ) && isTypeUsed( handleIt->first ) )
    {
      str += generateHandle( *handleIt, listedHandles );
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateIndexTypeTraits( std::pair<std::string, EnumData> const & enumData ) const
{
  assert( enumData.first == "VkIndexType" );

  std::string typeTraits;
  for ( auto const & value : enumData.second.values )
  {
    assert( value.name.starts_with( "VK_INDEX_TYPE_" ) );
    std::string type = stripPrefix( value.name, "VK_INDEX_TYPE_" );

    if ( !type.starts_with( "NONE" ) )
    {
      checkForError( type.starts_with( "UINT" ), value.xmlLine, "unknown VkIndexType <" + value.name + "> encountered" );
      std::string::size_type pos = type.find_first_of( "0123456789" );
      assert( pos != std::string::npos );
      std::string::size_type end   = type.find_first_not_of( "0123456789", pos );
      std::string::size_type count = ( end != std::string::npos ) ? ( end - pos ) : end;

      std::string valueName = generateEnumValueName( "VkIndexType", value.name, false );
      std::string cppType   = "uint" + type.substr( pos, count ) + "_t";

      const std::string typeTraitTemplate = R"(  template <>
  struct IndexTypeValue<${cppType}>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR IndexType value = IndexType::${valueName};
  };

  template <>
  struct CppType<IndexType, IndexType::${valueName}>
  {
    using Type = ${cppType};
  };
)";

      typeTraits += replaceWithMap( typeTraitTemplate, { { "cppType", cppType }, { "valueName", valueName } } );
    }
  }

  const std::string typeTraitsTemplate = R"(
  //=========================
  //=== Index Type Traits ===
  //=========================

  template<typename T>
  struct IndexTypeValue
  {};

${typeTraits}
)";

  return replaceWithMap( typeTraitsTemplate, { { "typeTraits", typeTraits } } );
}

std::string VulkanHppGenerator::generateLayerSettingTypeTraits() const
{
#if !defined( NDEBUG )
  auto enumIt = m_enums.find( "VkLayerSettingTypeEXT" );
  assert( ( enumIt != m_enums.end() ) && ( enumIt->second.values.size() == 8 ) && ( enumIt->second.values[0].name == "VK_LAYER_SETTING_TYPE_BOOL32_EXT" ) &&
          ( enumIt->second.values[1].name == "VK_LAYER_SETTING_TYPE_INT32_EXT" ) && ( enumIt->second.values[2].name == "VK_LAYER_SETTING_TYPE_INT64_EXT" ) &&
          ( enumIt->second.values[3].name == "VK_LAYER_SETTING_TYPE_UINT32_EXT" ) && ( enumIt->second.values[4].name == "VK_LAYER_SETTING_TYPE_UINT64_EXT" ) &&
          ( enumIt->second.values[5].name == "VK_LAYER_SETTING_TYPE_FLOAT32_EXT" ) &&
          ( enumIt->second.values[6].name == "VK_LAYER_SETTING_TYPE_FLOAT64_EXT" ) && ( enumIt->second.values[7].name == "VK_LAYER_SETTING_TYPE_STRING_EXT" ) );
#endif

  const std::string typeTraits = R"(
  //=================================
  //=== Layer Setting Type Traits ===
  //=================================

  template <>
  struct CppType<LayerSettingTypeEXT, LayerSettingTypeEXT::eBool32>
  {
    using Type = vk::Bool32;
  };

  template <>
  struct CppType<LayerSettingTypeEXT, LayerSettingTypeEXT::eInt32>
  {
    using Type = int32_t;
  };

  template <>
  struct CppType<LayerSettingTypeEXT, LayerSettingTypeEXT::eInt64>
  {
    using Type = int64_t;
  };

  template <>
  struct CppType<LayerSettingTypeEXT, LayerSettingTypeEXT::eUint32>
  {
    using Type = uint32_t;
  };

  template <>
  struct CppType<LayerSettingTypeEXT, LayerSettingTypeEXT::eUint64>
  {
    using Type = uint64_t;
  };

  template <>
  struct CppType<LayerSettingTypeEXT, LayerSettingTypeEXT::eFloat32>
  {
    using Type = float;
  };

  template <>
  struct CppType<LayerSettingTypeEXT, LayerSettingTypeEXT::eFloat64>
  {
    using Type = double;
  };

  template <>
  struct CppType<LayerSettingTypeEXT, LayerSettingTypeEXT::eString>
  {
    using Type = char *;
  };

  template <typename T>
  bool isSameType( LayerSettingTypeEXT layerSettingType )
  {
    switch ( layerSettingType )
    {
      case LayerSettingTypeEXT::eBool32: return std::is_same<T, VULKAN_HPP_NAMESPACE::Bool32>::value;
      case LayerSettingTypeEXT::eInt32: return std::is_same<T, int32_t>::value;
      case LayerSettingTypeEXT::eInt64: return std::is_same<T, int64_t>::value;
      case LayerSettingTypeEXT::eUint32: return std::is_same<T, uint32_t>::value;
      case LayerSettingTypeEXT::eUint64: return std::is_same<T, uint64_t>::value;
      case LayerSettingTypeEXT::eFloat32: return std::is_same<T, float>::value;
      case LayerSettingTypeEXT::eFloat64: return std::is_same<T, double>::value;
      case LayerSettingTypeEXT::eString: return std::is_same<T, char *>::value;
      default: return false;
    }
  }
)";

  return typeTraits;
}

std::string VulkanHppGenerator::generateLenInitializer(
  std::vector<MemberData>::const_iterator                                                                                 mit,
  std::map<std::vector<MemberData>::const_iterator, std::vector<std::vector<MemberData>::const_iterator>>::const_iterator litit,
  bool                                                                                                                    mutualExclusiveLens ) const
{
  std::string initializer;
  if ( mutualExclusiveLens )
  {
    // there are multiple mutually exclusive arrays related to this len
    for ( size_t i = 0; i + 1 < litit->second.size(); i++ )
    {
      auto        arrayIt      = litit->second[i];
      std::string argumentName = startLowerCase( stripPrefix( arrayIt->name, "p" ) ) + "_";
      initializer += "!" + argumentName + ".empty() ? " + argumentName + ".size() : ";
    }
    auto        arrayIt      = litit->second.back();
    std::string argumentName = startLowerCase( stripPrefix( arrayIt->name, "p" ) ) + "_";
    initializer += argumentName + ".size()";
  }
  else
  {
    auto arrayIt = litit->second.front();
    assert( ( arrayIt->lenExpressions.front() == litit->first->name ) ||
            ( ( arrayIt->lenExpressions.front() == "codeSize / 4" ) && ( litit->first->name == "codeSize" ) ) );

    assert( arrayIt->type.isPointer() || !arrayIt->arraySizes.empty() );
    assert( !arrayIt->type.isPointer() || arrayIt->name.starts_with( "p" ) );
    std::string argumentName = ( arrayIt->type.isPointer() ? startLowerCase( stripPrefix( arrayIt->name, "p" ) ) : arrayIt->name ) + "_";

    assert( mit->type.prefix.empty() && mit->type.postfix.empty() );
    initializer = argumentName + ".size()";
    if ( arrayIt->lenExpressions.front() == "codeSize / 4" )
    {
      initializer += " * 4";
    }
    if ( ( arrayIt->type.type == "void" ) &&
         ( std::count_if( arrayIt->type.postfix.begin(), arrayIt->type.postfix.end(), []( char c ) { return c == '*'; } ) < 2 ) )
    {
      initializer += " * sizeof(T)";
    }
  }
  if ( mit->type.type != "size_t" )
  {
    initializer = "static_cast<" + mit->type.type + ">( " + initializer + " )";
  }
  if ( !litit->second.front()->arraySizes.empty() )
  {
    assert( litit->second.front()->arraySizes.size() == 1 );
    initializer = "(std::min)( " + initializer + ", " + litit->second.front()->arraySizes[0] + " )";
  }
  return initializer;
}

std::string VulkanHppGenerator::generateName( TypeInfo const & typeInfo ) const
{
  std::string name = stripPrefix( typeInfo.type, "Vk" );
  assert( typeInfo.prefix.find( '*' ) == std::string::npos );
  if ( typeInfo.postfix.find( '*' ) != std::string::npos )
  {
    assert( typeInfo.postfix.find_first_of( '*' ) == typeInfo.postfix.find_last_of( '*' ) );
    name = "p" + name;
  }
  else
  {
    name = startLowerCase( name );
  }
  return name;
}

std::string VulkanHppGenerator::generateNoExcept( std::vector<std::string> const &          errorCodes,
                                                  std::vector<size_t> const &               returnParams,
                                                  std::map<size_t, VectorParamData> const & vectorParams,
                                                  CommandFlavourFlags                       flavourFlags,
                                                  bool                                      vectorSizeCheck,
                                                  bool                                      raii ) const
{
  // noexcept is only possible with no error codes, and the return param (if any) is not a vector param (unless it's the singular version)
  return ( errorCodes.empty() &&
           ( ( flavourFlags & CommandFlavourFlagBits::singular ) || returnParams.empty() ||
             std::none_of( returnParams.begin(), returnParams.end(), [&vectorParams]( size_t rp ) { return vectorParams.contains( rp ); } ) ) )
         ? ( vectorSizeCheck ? ( raii ? "" : " VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS" ) : " VULKAN_HPP_NOEXCEPT" )
         : "";
}

std::string VulkanHppGenerator::generateObjectDeleter( std::string const & commandName,
                                                       CommandData const & commandData,
                                                       size_t              initialSkipCount,
                                                       size_t              returnParam ) const
{
  std::string objectDeleter, allocator;
  if ( ( commandName.find( "Acquire" ) != std::string::npos ) || ( commandName.find( "Get" ) != std::string::npos ) )
  {
    if ( ( commandName == "vkAcquirePerformanceConfigurationINTEL" ) || ( commandName == "vkGetRandROutputDisplayEXT" ) ||
         ( commandName == "vkGetWinrtDisplayNV" ) || ( commandName == "vkGetDrmDisplayEXT" ) )
    {
      objectDeleter = "detail::ObjectRelease";
    }
    else
    {
      throw std::runtime_error( "Found " + commandName + " which requires special handling for the object deleter" );
    }
  }
  else if ( commandName.find( "Allocate" ) != std::string::npos )
  {
    objectDeleter = "detail::ObjectFree";
    allocator     = "allocator, ";
  }
  else
  {
    assert( ( commandName.find( "Create" ) != std::string::npos ) || ( commandName.find( "Register" ) != std::string::npos ) );
    objectDeleter = "detail::ObjectDestroy";
    allocator     = "allocator, ";
  }
  std::string className  = initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "";
  std::string parentName = ( className.empty() || ( commandData.params[returnParam].type.type == "VkDevice" ) ) ? "detail::NoParent" : className;
  return objectDeleter + "<" + parentName + ", Dispatch>( " + ( ( parentName == "detail::NoParent" ) ? "" : "*this, " ) + allocator + "d )";
}

std::string VulkanHppGenerator::generateObjectTypeToDebugReportObjectType() const
{
  auto objectTypeToDebugReportObjectTypeTemplate = std::string{ R"(
  //===========================================================
  //=== Mapping from ObjectType to DebugReportObjectTypeEXT ===
  //===========================================================

  VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType( VULKAN_HPP_NAMESPACE::ObjectType objectType )
  {
    switch( objectType )
    {
${objectTypeCases}
      default:
        VULKAN_HPP_ASSERT( false && "unknown ObjectType" );
        return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::eUnknown;
    }
  }
)" };

  auto const generateObjectTypeCases = [this]( std::vector<RequireData> const & requireData, std::string const & title )
  {
    static const std::string objectTypeCaseTemplate =
      "      case VULKAN_HPP_NAMESPACE::ObjectType::${objectType} : return VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::${debugReportObjectType};\n";

    std::string objectTypeCases;
    for ( auto const & require : requireData )
    {
      for ( auto const & type : require.types )
      {
        auto handleIt = m_handles.find( type.name );
        if ( handleIt != m_handles.end() )
        {
          objectTypeCases += replaceWithMap( objectTypeCaseTemplate,
                                             { { "debugReportObjectType", generateDebugReportObjectType( handleIt->second.objTypeEnum ) },
                                               { "objectType", generateEnumValueName( "VkObjectType", handleIt->second.objTypeEnum, false ) } } );
        }
      }
    }
    return addTitleAndProtection( title, objectTypeCases );
  };

  std::string objectTypeCases;
  for ( auto const & feature : m_features )
  {
    objectTypeCases += generateObjectTypeCases( feature.requireData, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    objectTypeCases += generateObjectTypeCases( extension.requireData, extension.name );
  }

  return replaceWithMap( objectTypeToDebugReportObjectTypeTemplate, { { "objectTypeCases", objectTypeCases } } );
}

std::pair<std::string, std::string> VulkanHppGenerator::generateProtection( std::string const & protect, bool defined ) const
{
  auto const openProtect = defined ? "#if defined( " : "#if !defined( ";
  return protect.empty() ? std::make_pair( "", "" ) : std::make_pair( openProtect + protect + " )\n", "#endif /*" + protect + "*/\n" );
}

std::string VulkanHppGenerator::generateRAIICommandDefinitions() const
{
  const std::string commandDefinitionsTemplate = R"(
  //===========================
  //=== COMMAND Definitions ===
  //===========================

${commandDefinitions}
)";

  std::string           commandDefinitions;
  std::set<std::string> listedCommands;  // some commands are listed with more than one extension!
  for ( auto const & feature : m_features )
  {
    commandDefinitions += generateRAIICommandDefinitions( feature.requireData, listedCommands, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    commandDefinitions += generateRAIICommandDefinitions( extension.requireData, listedCommands, extension.name );
  }

  return replaceWithMap( commandDefinitionsTemplate, { { "commandDefinitions", commandDefinitions } } );
}

std::string VulkanHppGenerator::generateRAIICommandDefinitions( std::vector<RequireData> const & requireData,
                                                                std::set<std::string> &          listedCommands,
                                                                std::string const &              title ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & command : require.commands )
    {
      if ( listedCommands.insert( command.name ).second )
      {
        str += generateRAIIHandleCommand( command.name, determineInitialSkipCount( command.name ), true );
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateRAIIDispatchers() const
{
  std::string contextInitializers, contextMembers, deviceAssignments, deviceMembers, instanceAssignments, instanceMembers;

  std::set<std::string> listedCommands;
  for ( auto const & feature : m_features )
  {
    appendRAIIDispatcherCommands( feature.requireData,
                                  listedCommands,
                                  feature.name,
                                  contextInitializers,
                                  contextMembers,
                                  deviceAssignments,
                                  deviceMembers,
                                  instanceAssignments,
                                  instanceMembers );
  }
  for ( auto const & extension : m_extensions )
  {
    appendRAIIDispatcherCommands( extension.requireData,
                                  listedCommands,
                                  extension.name,
                                  contextInitializers,
                                  contextMembers,
                                  deviceAssignments,
                                  deviceMembers,
                                  instanceAssignments,
                                  instanceMembers );
  }

  std::string contextDispatcherTemplate = R"(
    class ContextDispatcher : public ::VULKAN_HPP_NAMESPACE::detail::DispatchLoaderBase
    {
    public:
      ContextDispatcher( PFN_vkGetInstanceProcAddr getProcAddr )
        : vkGetInstanceProcAddr( getProcAddr )${contextInitializers}
      {}

    public:
      PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = 0;
${contextMembers}
    };
)";

  std::string str = replaceWithMap( contextDispatcherTemplate, { { "contextInitializers", contextInitializers }, { "contextMembers", contextMembers } } );

  std::string instanceDispatcherTemplate = R"(
    class InstanceDispatcher : public ::VULKAN_HPP_NAMESPACE::detail::DispatchLoaderBase
    {
    public:
      InstanceDispatcher( PFN_vkGetInstanceProcAddr getProcAddr, VkInstance instance )
        : vkGetInstanceProcAddr( getProcAddr )
      {
${instanceAssignments}
        vkGetDeviceProcAddr =
          PFN_vkGetDeviceProcAddr( vkGetInstanceProcAddr( instance, "vkGetDeviceProcAddr" ) );
      }

    public:
${instanceMembers}
      PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = 0;
    };
)";

  str += replaceWithMap( instanceDispatcherTemplate, { { "instanceAssignments", instanceAssignments }, { "instanceMembers", instanceMembers } } );

  std::string deviceDispatcherTemplate = R"(
    class DeviceDispatcher : public ::VULKAN_HPP_NAMESPACE::detail::DispatchLoaderBase
    {
    public:
      DeviceDispatcher( PFN_vkGetDeviceProcAddr getProcAddr, VkDevice device ) : vkGetDeviceProcAddr( getProcAddr )
      {
${deviceAssignments}
      }

    public:
${deviceMembers}
    };
)";

  str += replaceWithMap( deviceDispatcherTemplate, { { "deviceAssignments", deviceAssignments }, { "deviceMembers", deviceMembers } } );
  return str;
}

std::string VulkanHppGenerator::generateRAIIFactoryReturnStatements( std::vector<ParamData> const &   params,
                                                                     std::vector<std::string> const & successCodes,
                                                                     std::string const &              vkType,
                                                                     bool                             enumerating,
                                                                     std::string const &              returnType,
                                                                     std::string const &              returnVariable,
                                                                     bool                             singular ) const
{
  auto handleIt = m_handles.find( vkType );
  assert( handleIt != m_handles.end() );

  std::string successCodePassToElement = ( enumerating ? ( successCodes.size() <= 2 ) : ( successCodes.size() <= 1 ) ) ? "" : ", result";
  if ( returnType.starts_with( "std::vector" ) )
  {
    assert( !successCodes.empty() );

    std::string const & returnTemplate = R"(${returnType} ${returnVariable}RAII;
        ${returnVariable}RAII.reserve( ${returnVariable}.size() );
        for ( auto & ${element} : ${returnVariable} )
        {
          ${returnVariable}RAII.emplace_back( *this, ${handleConstructorArguments}${successCodePassToElement} );
        }
        return ${returnVariable}RAII;
)";

    std::string element                    = stripPluralS( returnVariable );
    std::string handleConstructorArguments = generateRAIIHandleSingularConstructorArguments( *handleIt, params, vkType, element );

    return replaceWithMap( returnTemplate,
                           { { "element", element },
                             { "handleConstructorArguments", handleConstructorArguments },
                             { "successCodePassToElement", successCodePassToElement },
                             { "returnType", returnType },
                             { "returnVariable", returnVariable } } );
  }
  else
  {
    std::string const & returnTemplate = "return ${returnType}( *this, ${handleConstructorArguments}${successCodePassToElement} );";

    std::string handleConstructorArguments =
      generateRAIIHandleSingularConstructorArguments( *handleIt, params, vkType, singular ? stripPluralS( returnVariable ) : returnVariable );

    return replaceWithMap( returnTemplate,
                           { { "returnType", returnType },
                             { "handleConstructorArguments", handleConstructorArguments },
                             { "successCodePassToElement", successCodePassToElement } } );
  }
}

std::string VulkanHppGenerator::generateRAIIHandle( std::pair<std::string, HandleData> const & handle,
                                                    std::set<std::string> &                    listedHandles,
                                                    std::set<std::string> const &              specialFunctions ) const
{
  std::string str;
  if ( !listedHandles.contains( handle.first ) )
  {
    rescheduleRAIIHandle( str, handle, listedHandles, specialFunctions );

    auto [enter, leave]    = generateProtection( getProtectFromType( handle.first ) );
    std::string handleType = stripPrefix( handle.first, "Vk" );
    std::string handleName = generateRAIIHandleConstructorParamName( handle.first, handle.second.destructorIt );

    auto [singularConstructors, arrayConstructors] = generateRAIIHandleConstructors( handle );

    auto [clearMembers, getConstructorSuccessCode, memberVariables, moveConstructorInitializerList, moveAssignmentInstructions, swapMembers, releaseMembers] =
      generateRAIIHandleDetails( handle );

    std::string declarations = generateRAIIHandleCommandDeclarations( handle, specialFunctions );

    assert( !handle.second.objTypeEnum.empty() );
    auto enumIt = m_enums.find( "VkObjectType" );
    assert( enumIt != m_enums.end() );
    assert( contains( enumIt->second.values, handle.second.objTypeEnum ) );
    std::string objTypeEnum = generateEnumValueName( enumIt->first, handle.second.objTypeEnum, false );

    enumIt = m_enums.find( "VkDebugReportObjectTypeEXT" );
    assert( enumIt != m_enums.end() );
    std::string valueName             = handle.second.objTypeEnum;
    valueName                         = valueName.replace( 3, 0, "DEBUG_REPORT_" ) + "_EXT";
    std::string debugReportObjectType = contains( enumIt->second.values, valueName ) ? generateEnumValueName( enumIt->first, valueName, false ) : "eUnknown";

    std::string dispatcherType = ( ( handle.first == "VkDevice" ) || ( handle.second.constructorIts.front()->second.params.front().type.type == "VkDevice" ) )
                                 ? "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::detail::DeviceDispatcher"
                                 : "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::detail::InstanceDispatcher";

    std::string getParent;
    if ( ( handle.first != "VkInstance" ) && ( handle.first != "VkDevice" ) && ( handle.second.destructorIt != m_commands.end() ) )
    {
      assert( !handle.second.destructorIt->second.params.empty() );
      std::string parentType = stripPrefix( handle.second.destructorIt->second.params.front().type.type, "Vk" );
      getParent              = "    VULKAN_HPP_NAMESPACE::" + parentType + " get" + parentType + "() const\n";
      getParent += "    {\n";
      getParent += "      return m_" + handle.second.destructorIt->second.params.front().name + ";\n";
      getParent += "    }\n";
    }

    std::string assignmentOperator, copyConstructor;
    if ( handle.second.destructorIt == m_commands.end() )
    {
      // allow copy constructor and assignment operator for classes without destructor
      std::string const copyConstructorTemplate =
        R"(      ${handleType}( ${handleType} const & rhs ) : m_${handleName}( rhs.m_${handleName} ), m_dispatcher( rhs.m_dispatcher ) {})";
      copyConstructor += replaceWithMap( copyConstructorTemplate, { { "handleName", handleName }, { "handleType", handleType } } );

      std::string assignmentOperatorTemplate = R"(      ${handleType} & operator=( ${handleType} const & rhs )
      {
        m_${handleName} = rhs.m_${handleName};
        m_dispatcher    = rhs.m_dispatcher;
        return *this;
      })";
      assignmentOperator += replaceWithMap( assignmentOperatorTemplate, { { "handleName", handleName }, { "handleType", handleType } } );
    }
    else
    {
      std::string const copyConstructorTemplate = R"(      ${handleType}( ${handleType} const & ) = delete;)";
      copyConstructor += replaceWithMap( copyConstructorTemplate, { { "handleType", handleType } } );

      std::string const assignmentOperatorTemplate = R"(      ${handleType} & operator=( ${handleType} const & ) = delete;)";
      assignmentOperator += replaceWithMap( assignmentOperatorTemplate, { { "handleType", handleType } } );
    }

    const std::string handleTemplate = R"(
${enter}  class ${handleType}
  {
  public:
    using CType = Vk${handleType};
    using CppType = VULKAN_HPP_NAMESPACE::${handleType};

    static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::${objTypeEnum};
    static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType = VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::${debugReportObjectType};

  public:
${singularConstructors}
    ${handleType}( std::nullptr_t ) {}

    ~${handleType}()
    {
      clear();
    }

    ${handleType}() = delete;
${copyConstructor}
    ${handleType}( ${handleType} && rhs ) VULKAN_HPP_NOEXCEPT
      : ${moveConstructorInitializerList}
    {}
${assignmentOperator}
    ${handleType} & operator=( ${handleType} && rhs ) VULKAN_HPP_NOEXCEPT
    {
      if ( this != &rhs )
      {
${moveAssignmentInstructions}
      }
      return *this;
    }

    VULKAN_HPP_NAMESPACE::${handleType} const & operator*() const VULKAN_HPP_NOEXCEPT
    {
      return m_${handleName};
    }

    operator VULKAN_HPP_NAMESPACE::${handleType}() const VULKAN_HPP_NOEXCEPT
    {
      return m_${handleName};
    }

    void clear() VULKAN_HPP_NOEXCEPT
    {
${clearMembers}
    }

    VULKAN_HPP_NAMESPACE::${handleType} release()
    {
${releaseMembers}
    }

${getConstructorSuccessCode}
${getParent}
    ${dispatcherType} const * getDispatcher() const
    {
      VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
      return ${getDispatcherReturn}m_dispatcher;
    }

    void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::${handleType} & rhs ) VULKAN_HPP_NOEXCEPT
    {
      ${swapMembers}
    }

    ${memberFunctionsDeclarations}

  private:
    ${memberVariables}
  };

  template <>
  struct isVulkanRAIIHandleType<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::${handleType}>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool value = true;
  };

${leave})";

    str += replaceWithMap( handleTemplate,
                           { { "assignmentOperator", assignmentOperator },
                             { "clearMembers", clearMembers },
                             { "copyConstructor", copyConstructor },
                             { "debugReportObjectType", debugReportObjectType },
                             { "dispatcherType", dispatcherType },
                             { "enter", enter },
                             { "getConstructorSuccessCode", getConstructorSuccessCode },
                             { "getDispatcherReturn", ( handleType == "Device" ) || ( handleType == "Instance" ) ? "&*" : "" },
                             { "getParent", getParent },
                             { "handleName", handleName },
                             { "handleType", handleType },
                             { "leave", leave },
                             { "memberFunctionsDeclarations", declarations },
                             { "memberVariables", memberVariables },
                             { "moveAssignmentInstructions", moveAssignmentInstructions },
                             { "moveConstructorInitializerList", moveConstructorInitializerList },
                             { "objTypeEnum", objTypeEnum },
                             { "releaseMembers", releaseMembers },
                             { "singularConstructors", singularConstructors },
                             { "swapMembers", swapMembers } } );

    if ( !arrayConstructors.empty() )
    {
      // it's a handle class with a friendly handles class
      const std::string handlesTemplate = R"(
${enter}  class ${handleType}s : public std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::${handleType}>
  {
  public:
    ${arrayConstructors}
    ${handleType}s( std::nullptr_t ) {}

    ${handleType}s() = delete;
    ${handleType}s( ${handleType}s const & ) = delete;
    ${handleType}s( ${handleType}s && rhs ) = default;
    ${handleType}s & operator=( ${handleType}s const & ) = delete;
    ${handleType}s & operator=( ${handleType}s && rhs ) = default;

  private:
    ${handleType}s( std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::${handleType}> && rhs )
      {
        std::swap( *this, rhs );
      }
  };
${leave}
)";

      str +=
        replaceWithMap( handlesTemplate, { { "arrayConstructors", arrayConstructors }, { "enter", enter }, { "handleType", handleType }, { "leave", leave } } );
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateRAIIHandleCommand( std::string const & command, size_t initialSkipCount, bool definition ) const
{
  std::string str;
  if ( !m_RAIISpecialFunctions.contains( command ) )
  {
    auto commandIt = findByNameOrAlias( m_commands, command );
    assert( commandIt != m_commands.end() );
    str = generateCommand( command, commandIt->second, initialSkipCount, definition, true );
  }
  return str;
}

std::string VulkanHppGenerator::generateRAIIHandleCommandDeclarations( std::pair<std::string, HandleData> const & handle,
                                                                       std::set<std::string> const &              specialFunctions ) const
{
  std::string                     functionDeclarations;
  std::map<std::string, NameLine> listedCommands;  // some commands are listed with more than one extension !
  for ( auto const & feature : m_features )
  {
    std::vector<std::string> firstLevelCommands, secondLevelCommands;

    for ( auto const & require : feature.requireData )
    {
      for ( auto const & command : require.commands )
      {
        if ( !specialFunctions.contains( command.name ) )
        {
          if ( handle.second.commands.contains( command.name ) )
          {
            assert( !listedCommands.contains( command.name ) );
            listedCommands.insert( { command.name, { feature.name, command.xmlLine } } );
            firstLevelCommands.push_back( command.name );
          }
          else if ( handle.second.secondLevelCommands.contains( command.name ) )
          {
            auto listedIt = listedCommands.find( command.name );
            if ( listedIt == listedCommands.end() )
            {
              listedCommands.insert( { command.name, { feature.name, command.xmlLine } } );
              assert( !handle.first.empty() );
              secondLevelCommands.push_back( command.name );
            }
            else
            {
              checkForError( listedIt->second.name == feature.name,
                             command.xmlLine,
                             "command <" + command.name + "> already listed as required for feature <" + listedIt->second.name + "> on line " +
                               std::to_string( listedIt->second.xmlLine ) );
            }
          }
        }
      }
    }
    if ( !firstLevelCommands.empty() || !secondLevelCommands.empty() )
    {
      functionDeclarations += "\n  //=== " + feature.name + " ===\n";
      for ( auto const & command : firstLevelCommands )
      {
        functionDeclarations += generateRAIIHandleCommand( command, handle.first.empty() ? 0 : 1, false );
      }
      for ( auto const & command : secondLevelCommands )
      {
        assert( !handle.first.empty() );
        functionDeclarations += generateRAIIHandleCommand( command, 2, false );
      }
    }
  }

  for ( auto const & extension : m_extensions )
  {
    std::vector<std::string> firstLevelCommands, secondLevelCommands;
    for ( auto & req : extension.requireData )
    {
      for ( auto const & command : req.commands )
      {
        if ( !specialFunctions.contains( command.name ) && !listedCommands.contains( command.name ) )
        {
          if ( handle.second.commands.contains( command.name ) )
          {
            listedCommands.insert( { command.name, { extension.name, command.xmlLine } } );
            firstLevelCommands.push_back( command.name );
          }
          else if ( handle.second.secondLevelCommands.contains( command.name ) )
          {
            listedCommands.insert( { command.name, { extension.name, command.xmlLine } } );
            secondLevelCommands.push_back( command.name );
          }
        }
      }
    }
    if ( !firstLevelCommands.empty() || !secondLevelCommands.empty() )
    {
      std::string handleProtect = getProtectFromType( handle.first );
      std::string titleProtect  = getProtectFromTitle( extension.name );
      std::string enter, leave;
      if ( !titleProtect.empty() && ( titleProtect != handleProtect ) )
      {
        std::tie( enter, leave ) = generateProtection( titleProtect );
      }
      functionDeclarations += "\n" + enter + "  //=== " + extension.name + " ===\n";
      for ( auto const & command : firstLevelCommands )
      {
        functionDeclarations += generateRAIIHandleCommand( command, handle.first.empty() ? 0 : 1, false );
      }
      for ( auto const & command : secondLevelCommands )
      {
        assert( !handle.first.empty() );
        functionDeclarations += generateRAIIHandleCommand( command, 2, false );
      }
      functionDeclarations += leave;
    }
  }
  return functionDeclarations;
}

std::string VulkanHppGenerator::generateRAIIHandleCommandEnhanced( std::string const &                       name,
                                                                   CommandData const &                       commandData,
                                                                   size_t                                    initialSkipCount,
                                                                   std::vector<size_t> const &               returnParams,
                                                                   std::map<size_t, VectorParamData> const & vectorParams,
                                                                   bool                                      definition,
                                                                   CommandFlavourFlags                       flavourFlags ) const
{
  const bool singular = flavourFlags & CommandFlavourFlagBits::singular;

  std::set<size_t> skippedParams  = determineSkippedParams( commandData.params, initialSkipCount, vectorParams, returnParams, singular );
  std::set<size_t> singularParams = singular ? determineSingularParams( returnParams[0], vectorParams ) : std::set<size_t>();
  std::set<size_t> templatedParams =
    ( flavourFlags & CommandFlavourFlagBits::keepVoidPtr ) ? std::set<size_t>() : determineVoidPointerParams( commandData.params );
  std::vector<size_t> chainedReturnParams =
    ( flavourFlags & CommandFlavourFlagBits::chained ) ? determineChainedReturnParams( commandData.params, returnParams ) : std::vector<size_t>();
  assert( chainedReturnParams.size() <= 1 );

  const bool               enumerating = determineEnumeration( vectorParams, returnParams );
  std::vector<std::string> dataTypes   = determineDataTypes( commandData.params, vectorParams, returnParams, templatedParams );
  std::string              dataType    = combineDataTypes( vectorParams, returnParams, enumerating, dataTypes, flavourFlags, true );

  std::string argumentTemplates = generateArgumentTemplates( commandData.params, returnParams, vectorParams, templatedParams, chainedReturnParams, true );
  std::string argumentList      = generateArgumentListEnhanced(
    commandData.params, returnParams, vectorParams, skippedParams, singularParams, templatedParams, chainedReturnParams, definition, flavourFlags, false );
  std::string commandName = generateCommandName( name, commandData.params, initialSkipCount, flavourFlags );
  std::string nodiscard   = generateNoDiscard(
    !returnParams.empty() || ( ( commandData.returnType != "VkResult" ) && ( commandData.returnType != "void" ) ), 1 < commandData.successCodes.size(), false );
  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck =
    needsVectorSizeCheck( commandData.params, vectorParams, returnParams, singularParams, skippedParams );
  std::string noexceptString      = generateNoExcept( commandData.errorCodes, returnParams, vectorParams, flavourFlags, vectorSizeCheck.first, true );
  std::string returnType          = generateReturnType( returnParams, vectorParams, flavourFlags, true, dataTypes );
  std::string decoratedReturnType = generateDecoratedReturnType( commandData, returnParams, vectorParams, flavourFlags, true, returnType );

  if ( definition )
  {
    std::string const definitionTemplate =
      R"(
  ${argumentTemplates}
  ${nodiscard} VULKAN_HPP_INLINE ${returnType} ${className}::${commandName}( ${argumentList} ) const ${noexcept}
  {
${functionPointerCheck}
${vectorSizeCheck}
    ${dataSizeChecks}
    ${dataDeclarations}
    ${callSequence}
    ${resultCheck}
    ${dataPreparation}
    ${returnStatement}
  }
)";

    std::string callSequence = generateCallSequence(
      name, commandData, returnParams, vectorParams, initialSkipCount, singularParams, templatedParams, chainedReturnParams, flavourFlags, true, false );
    std::string className      = initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "Context";
    std::string returnVariable = generateReturnVariable( commandData, returnParams, vectorParams, flavourFlags );
    std::string dataDeclarations =
      generateDataDeclarations( commandData, returnParams, vectorParams, templatedParams, flavourFlags, true, dataTypes, dataType, returnType, returnVariable );
    std::string dataPreparation =
      generateDataPreparation( commandData, initialSkipCount, returnParams, vectorParams, templatedParams, flavourFlags, enumerating, dataTypes );
    std::string dataSizeChecks  = generateDataSizeChecks( commandData, returnParams, dataTypes, vectorParams, templatedParams, singular );
    std::string resultCheck     = generateResultCheck( commandData, className, "::", commandName, enumerating );
    std::string returnStatement = generateReturnStatement( name,
                                                           commandData,
                                                           returnVariable,
                                                           returnType,
                                                           decoratedReturnType,
                                                           dataType,
                                                           initialSkipCount,
                                                           returnParams.empty() ? INVALID_INDEX : returnParams[0],
                                                           flavourFlags,
                                                           enumerating,
                                                           true );
    std::string vectorSizeCheckString =
      vectorSizeCheck.first ? generateVectorSizeCheck( name, commandData, initialSkipCount, vectorSizeCheck.second, skippedParams, false ) : "";

    return replaceWithMap( definitionTemplate,
                           { { "argumentList", argumentList },
                             { "argumentTemplates", argumentTemplates },
                             { "callSequence", callSequence },
                             { "className", className },
                             { "commandName", commandName },
                             { "dataDeclarations", dataDeclarations },
                             { "dataPreparation", dataPreparation },
                             { "dataSizeChecks", dataSizeChecks },
                             { "functionPointerCheck", generateFunctionPointerCheck( name, commandData.requiredBy, true ) },
                             { "nodiscard", nodiscard },
                             { "noexcept", noexceptString },
                             { "resultCheck", resultCheck },
                             { "returnStatement", returnStatement },
                             { "returnType", decoratedReturnType },
                             { "vectorSizeCheck", vectorSizeCheckString } } );
  }
  else
  {
    std::string const declarationTemplate =
      R"(
    ${argumentTemplates}
    ${nodiscard} ${returnType} ${commandName}( ${argumentList} ) const ${noexcept};
)";

    return replaceWithMap( declarationTemplate,
                           { { "argumentList", argumentList },
                             { "argumentTemplates", argumentTemplates },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "noexcept", noexceptString },
                             { "returnType", decoratedReturnType } } );
  }
}

std::string VulkanHppGenerator::generateRAIIHandleCommandFactory( std::string const &                       name,
                                                                  CommandData const &                       commandData,
                                                                  size_t                                    initialSkipCount,
                                                                  std::vector<size_t> const &               returnParams,
                                                                  std::map<size_t, VectorParamData> const & vectorParams,
                                                                  bool                                      definition,
                                                                  CommandFlavourFlags                       flavourFlags ) const
{
  assert( isHandleType( commandData.params[returnParams.back()].type.type ) ||
          ( vectorParams.contains( returnParams.back() ) && vectorParams.find( returnParams.back() )->second.byStructure &&
            isHandleType( vectorMemberByStructure( commandData.params[returnParams.back()].type.type ).type.type ) ) );
  assert( ( returnParams.size() == 1 ) ||
          ( ( returnParams.size() == 2 ) && ( vectorParams.size() == 1 ) && ( returnParams[0] == vectorParams.begin()->second.lenParam ) &&
            ( returnParams[1] == vectorParams.begin()->first ) ) );
  assert( determineVoidPointerParams( commandData.params ).empty() );

  const bool       enumerating    = determineEnumeration( vectorParams, returnParams );
  const bool       singular       = flavourFlags & CommandFlavourFlagBits::singular;
  std::set<size_t> skippedParams  = determineSkippedParams( commandData.params, initialSkipCount, vectorParams, returnParams, singular );
  std::set<size_t> singularParams = singular ? determineSingularParams( returnParams.back(), vectorParams ) : std::set<size_t>();
  std::string      argumentList   = generateRAIIHandleCommandFactoryArgumentList( commandData.params, skippedParams, definition, singular );
  std::string      commandName    = generateCommandName( name, commandData.params, initialSkipCount, flavourFlags );

  std::string handleType;
  if ( ( vectorParams.size() == 1 ) && vectorParams.begin()->second.byStructure )
  {
    assert( vectorParams.begin()->first == returnParams.back() );
    handleType = vectorMemberByStructure( commandData.params.back().type.type ).type.compose( "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE" );
  }
  else
  {
    handleType = commandData.params[returnParams.back()].type.compose( "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE" );
  }
  handleType                 = stripPostfix( handleType, " *" );
  std::string noexceptString = enumerating ? "" : "VULKAN_HPP_RAII_CREATE_NOEXCEPT";
  std::string returnType     = handleType;
  if ( vectorParams.contains( returnParams.back() ) && !singular )
  {
    noexceptString = "";
    returnType     = "std::vector<" + handleType + ">";
    handleType += "s";
  }

  if ( definition )
  {
    std::string              className      = initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "Context";
    std::vector<std::string> dataTypes      = determineDataTypes( commandData.params, vectorParams, returnParams, {} );
    std::string              dataType       = combineDataTypes( vectorParams, returnParams, enumerating, dataTypes, flavourFlags, true );
    std::string              returnVariable = generateReturnVariable( commandData, returnParams, vectorParams, flavourFlags );
    std::string              vulkanType;
    auto                     vectorParamIt = vectorParams.find( returnParams.back() );
    if ( ( vectorParamIt != vectorParams.end() ) && vectorParamIt->second.byStructure )
    {
      vulkanType = vectorMemberByStructure( commandData.params[returnParams.back()].type.type ).type.type;
    }
    else
    {
      vulkanType = commandData.params[returnParams.back()].type.type;
    }

    std::string dataDeclarations =
      generateDataDeclarations( commandData, returnParams, vectorParams, {}, flavourFlags, true, dataTypes, dataType, returnType, returnVariable );
    std::string callSequence =
      generateCallSequence( name, commandData, returnParams, vectorParams, initialSkipCount, singularParams, {}, {}, flavourFlags, true, true );
    std::string resultCheck = generateResultCheckExpected( commandData.successCodes, className, commandName );
    std::string returnStatements =
      generateRAIIFactoryReturnStatements( commandData.params, commandData.successCodes, vulkanType, enumerating, returnType, returnVariable, singular );

    std::string const definitionTemplate =
      R"(
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::detail::CreateReturnType<${returnType}>::Type ${className}::${commandName}( ${argumentList} ) const ${noexcept}
  {
    ${dataDeclarations}
    ${callSequence}
    ${resultCheck}
    ${returnStatements}
  }
)";

    return replaceWithMap( definitionTemplate,
                           { { "argumentList", argumentList },
                             { "callSequence", callSequence },
                             { "className", className },
                             { "commandName", commandName },
                             { "dataDeclarations", dataDeclarations },
                             { "noexcept", noexceptString },
                             { "resultCheck", resultCheck },
                             { "returnStatements", returnStatements },
                             { "returnType", returnType } } );
  }
  else
  {
    std::string const declarationTemplate =
      R"(
  VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::detail::CreateReturnType<${returnType}>::Type ${commandName}( ${argumentList} ) const ${noexcept};
)";

    return replaceWithMap( declarationTemplate,
                           { { "argumentList", argumentList }, { "commandName", commandName }, { "noexcept", noexceptString }, { "returnType", returnType } } );
  }
}

std::string VulkanHppGenerator::generateRAIIHandleCommandFactoryArgumentList( std::vector<ParamData> const & params,
                                                                              std::set<size_t> const &       skippedParams,
                                                                              bool                           definition,
                                                                              bool                           singular ) const
{
  std::vector<std::string> arguments;
  for ( size_t i = 0; i < params.size(); ++i )
  {
    if ( !skippedParams.contains( i ) )
    {
      arguments.push_back( generateRAIIHandleConstructorArgument( params[i], definition, singular, false ) );
    }
  }
  return generateList( arguments, "", ", " );
}

std::string VulkanHppGenerator::generateRAIIHandleCommandStandard( std::string const & name,
                                                                   CommandData const & commandData,
                                                                   size_t              initialSkipCount,
                                                                   bool                definition ) const
{
  std::set<size_t> skippedParams = determineSkippedParams( commandData.params, initialSkipCount, {}, {}, false );
  std::string      argumentList  = generateArgumentListStandard( commandData.params, skippedParams, definition, false );
  std::string      commandName   = generateCommandName( name, commandData.params, initialSkipCount );
  std::string      nodiscard     = ( commandData.returnType != "void" ) ? "VULKAN_HPP_NODISCARD" : "";
  std::string      returnType =
    commandData.returnType.starts_with( "Vk" ) ? "VULKAN_HPP_NAMESPACE::" + stripPrefix( commandData.returnType, "Vk" ) : commandData.returnType;

  if ( definition )
  {
    std::string functionBody = "getDispatcher()->" + name + "( " + generateCallArgumentsStandard( commandData.params, initialSkipCount ) + " )";
    if ( commandData.returnType.starts_with( "Vk" ) )
    {
      functionBody = "return static_cast<" + returnType + ">( " + functionBody + " )";
    }
    else if ( commandData.returnType != "void" )
    {
      functionBody = "return " + functionBody;
    }

    std::string const functionTemplate =
      R"(  ${nodiscard} VULKAN_HPP_INLINE ${returnType} ${className}::${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT
  {
    ${functionPointerCheck}
    ${functionBody};
  })";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "className", initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "Context" },
                             { "commandName", commandName },
                             { "functionBody", functionBody },
                             { "functionPointerCheck", generateFunctionPointerCheck( name, commandData.requiredBy, true ) },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType } } );
  }
  else
  {
    std::string const declarationTemplate =
      R"(
    ${nodiscard} ${returnType} ${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT;
)";

    return replaceWithMap( declarationTemplate,
                           { { "argumentList", argumentList }, { "commandName", commandName }, { "nodiscard", nodiscard }, { "returnType", returnType } } );
  }
}

std::pair<std::string, std::string> VulkanHppGenerator::generateRAIIHandleConstructor( std::pair<std::string, HandleData> const &         handle,
                                                                                       std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                                       std::string const &                                enter,
                                                                                       std::string const &                                leave ) const
{
  std::string singularConstructor, arrayConstructor;
  if ( constructorIt->second.returnType == "VkResult" )
  {
    std::tie( singularConstructor, arrayConstructor ) = generateRAIIHandleConstructorResult( handle, constructorIt, enter, leave );
  }
  else if ( constructorIt->second.returnType == "void" )
  {
    std::tie( singularConstructor, arrayConstructor ) = generateRAIIHandleConstructorVoid( handle, constructorIt, enter, leave );
  }
  if ( singularConstructor.empty() && arrayConstructor.empty() )
  {
    throw std::runtime_error( "Never encountered a function like <" + constructorIt->first + "> !" );
  }
  return std::make_pair( singularConstructor, arrayConstructor );
}

std::pair<std::string, std::string>
  VulkanHppGenerator::generateRAIIHandleConstructor1Return2Vector( std::pair<std::string, HandleData> const &         handle,
                                                                   std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                   std::string const &                                enter,
                                                                   std::string const &                                leave,
                                                                   size_t                                             returnParam,
                                                                   std::map<size_t, VectorParamData> const &          vectorParams ) const
{
  if ( returnParam == std::next( vectorParams.begin() )->first )
  {
    if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
    {
      if ( constructorIt->second.params[vectorParams.begin()->second.lenParam].type.type == "uint32_t" )
      {
        if ( ( constructorIt->second.params[vectorParams.begin()->first].type.type != "void" ) &&
             !isHandleType( constructorIt->second.params[vectorParams.begin()->first].type.type ) )
        {
          std::string singularConstructor;
          auto        lenParamIt = constructorIt->second.params.begin() + vectorParams.begin()->second.lenParam;
          if ( !checkEquivalentSingularConstructor( handle.second.constructorIts, constructorIt, lenParamIt ) )
          {
            singularConstructor = generateRAIIHandleConstructorByCall( handle, constructorIt, enter, leave, true, true );
          }
          return std::make_pair( singularConstructor, generateRAIIHandleConstructorByCall( handle, constructorIt, enter, leave, true, false ) );
        }
      }
    }
  }
  return std::make_pair( "", "" );
}

std::pair<std::string, std::string> VulkanHppGenerator::generateRAIIHandleConstructors( std::pair<std::string, HandleData> const & handle ) const
{
  const auto [enter, leave] = generateProtection( getProtectFromType( handle.first ) );

  std::string singularConstructors, arrayConstructors;
  for ( auto constructorIt : handle.second.constructorIts )
  {
    // there is a non-const parameter with handle type : the to-be-constructed handle

    // check for additional enter/leave guards for the constructors
    auto [constructorEnter, constructorLeave] = generateProtection( getProtectFromTitles( constructorIt->second.requiredBy ) );
    if ( constructorEnter == enter )
    {
      constructorEnter.clear();
      constructorLeave.clear();
    }

    const auto [singularConstructor, arrayConstructor] = generateRAIIHandleConstructor( handle, constructorIt, constructorEnter, constructorLeave );
    arrayConstructors += arrayConstructor;
    singularConstructors += singularConstructor;
  }
  singularConstructors += generateRAIIHandleConstructorTakeOwnership( handle );
  return std::make_pair( singularConstructors, arrayConstructors );
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorArgument( ParamData const & param, bool definition, bool singular, bool takesOwnership ) const
{
  std::string argument;
  if ( param.type.isConstPointer() )
  {
    assert( param.type.type.starts_with( "Vk" ) );
    assert( param.name.starts_with( "p" ) );
    std::string argumentName = startLowerCase( stripPrefix( param.name, "p" ) );
    std::string argumentType = generateNamespacedType( param.type.type );
    if ( param.optional )
    {
      assert( param.lenExpression.empty() );
      argument = "VULKAN_HPP_NAMESPACE::Optional<const " + argumentType + "> " + argumentName + ( definition ? "" : " = nullptr" );
    }
    else if ( param.lenExpression.empty() )
    {
      argument = argumentType + " const & " + argumentName;
    }
    else if ( singular )
    {
      argument = argumentType + " const & " + stripPluralS( argumentName );
    }
    else
    {
      argument = "VULKAN_HPP_NAMESPACE::ArrayProxy<" + argumentType + "> const & " + argumentName;
    }
  }
  else if ( specialPointerTypes.contains( param.type.type ) )
  {
    assert( !param.optional );
    assert( param.type.isNonConstPointer() );
    argument = param.type.type + " & " + param.name;
  }
  else if ( ( param.type.isValue() ) && isHandleType( param.type.type ) )
  {
    if ( takesOwnership )
    {
      assert( !param.optional );
      argument = param.type.type + " " + param.name;
    }
    else
    {
      argument = "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::" + stripPrefix( param.type.type, "Vk" );
      if ( param.optional )
      {
        argument = "VULKAN_HPP_NAMESPACE::Optional<const " + argument + ">";
      }
      argument += " const & " + param.name;
    }
  }
  else
  {
    assert( !param.optional );
    if ( param.arraySizes.empty() )
    {
      argument = param.type.compose( "VULKAN_HPP_NAMESPACE" ) + " ";
    }
    else
    {
      argument = generateStandardArray( param.type.compose( "VULKAN_HPP_NAMESPACE" ), param.arraySizes ) + " const & ";
    }
    argument += param.name;
  }
  return argument;
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorArguments( std::pair<std::string, HandleData> const &         handle,
                                                                        std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                        bool                                               singular,
                                                                        bool                                               takesOwnership ) const
{
  auto [parentType, parentName] = getParentTypeAndName( handle );

  std::string arguments = "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::" + parentType + " const & " + parentName;
  if ( takesOwnership )
  {
    arguments += ", " + handle.first + " " + generateRAIIHandleConstructorParamName( handle.first, handle.second.destructorIt );
  }

  if ( constructorIt != m_commands.end() )
  {
    parentType      = "Vk" + parentType;
    const bool skip = skipLeadingGrandParent( handle );
    for ( size_t i = skip ? 1 : 0; i < constructorIt->second.params.size(); i++ )
    {
      ParamData const & param = constructorIt->second.params[i];
      // filter parent and handle type
      if ( ( param.type.type != parentType ) && ( param.type.type != handle.first ) )
      {
        // the specialPointerTypes are considered const-pointers!
        if ( param.type.isNonConstPointer() && !specialPointerTypes.contains( param.type.type ) )
        {
          // this is supposed to be the returned size on an enumeration function!
#if !defined( NDEBUG )
          if ( ( param.type.type == "size_t" ) || ( param.type.type == "uint32_t" ) )
          {
            auto typeIt = std::find_if( constructorIt->second.params.begin(),
                                        constructorIt->second.params.end(),
                                        [&handle]( ParamData const & pd ) { return pd.type.type == handle.first; } );
            assert( typeIt != constructorIt->second.params.end() );
            assert( typeIt->lenExpression == param.name );
          }
          else
          {
            assert( vectorMemberByStructure( param.type.type ).type.type == handle.first );
          }
#endif
          continue;
        }
        else if ( std::any_of( constructorIt->second.params.begin(),
                               constructorIt->second.params.end(),
                               [&param]( ParamData const & pd ) { return pd.lenExpression == param.name; } ) )
        {
          // this is the len of an other parameter, which will be mapped to an ArrayProxy
          assert( param.type.isValue() && ( param.type.type == "uint32_t" ) );
          assert( param.arraySizes.empty() && param.lenExpression.empty() && !param.optional );
          continue;
        }
        arguments += ", " + generateRAIIHandleConstructorArgument( param, false, singular, takesOwnership );
      }
    }
  }
  return arguments;
}

std::string
  VulkanHppGenerator::generateRAIIHandleConstructorInitializationList( std::pair<std::string, HandleData> const &                             handle,
                                                                       std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
                                                                       std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator destructorIt,
                                                                       bool takesOwnership ) const
{
  const auto [parentType, parentName] = getParentTypeAndName( handle );
  std::string handleName              = generateRAIIHandleConstructorParamName( handle.first, destructorIt );

  std::string initializationList;
  if ( destructorIt != m_commands.end() )
  {
    for ( auto destructorParam : destructorIt->second.params )
    {
      if ( destructorParam.type.type == "Vk" + parentType )
      {
        initializationList += "m_" + parentName + "( " + parentName + " ), ";
      }
      else if ( destructorParam.type.type == handle.first )
      {
        if ( takesOwnership )
        {
          initializationList += "m_" + handleName + "( " + handleName + " ), ";
        }
      }
      else if ( destructorParam.type.type == "VkAllocationCallbacks" )
      {
        assert( destructorParam.type.isConstPointer() && destructorParam.arraySizes.empty() && destructorParam.lenExpression.empty() &&
                destructorParam.optional );
        initializationList += "m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ), ";
      }
      else if ( isHandleType( destructorParam.type.type ) )
      {
        assert( destructorParam.type.isValue() && destructorParam.arraySizes.empty() && destructorParam.lenExpression.empty() && !destructorParam.optional );
        initializationList += "m_" + destructorParam.name + "( ";
        auto constructorParamIt = std::find_if( constructorIt->second.params.begin(),
                                                constructorIt->second.params.end(),
                                                [&destructorParam]( ParamData const & pd ) { return pd.type.type == destructorParam.type.type; } );
        if ( constructorParamIt != constructorIt->second.params.end() )
        {
          assert( constructorParamIt->type.isValue() && constructorParamIt->arraySizes.empty() && constructorParamIt->lenExpression.empty() &&
                  !constructorParamIt->optional );
          if ( constructorParamIt->type.type == "Vk" + parentType )
          {
            initializationList += "*";
          }
          initializationList += constructorParamIt->name;
        }
        else
        {
#if !defined( NDEBUG )
          bool found = false;
#endif
          for ( auto constructorParam : constructorIt->second.params )
          {
            auto structureIt = m_structs.find( constructorParam.type.type );
            if ( structureIt != m_structs.end() )
            {
              auto structureMemberIt = findStructMemberItByType( destructorParam.type.type, structureIt->second.members );
              if ( structureMemberIt != structureIt->second.members.end() )
              {
                assert( constructorParam.type.isConstPointer() && constructorParam.arraySizes.empty() && constructorParam.lenExpression.empty() &&
                        !constructorParam.optional );
                initializationList += startLowerCase( stripPrefix( constructorParam.name, "p" ) ) + "." + structureMemberIt->name;
#if !defined( NDEBUG )
                found = true;
#endif
                break;
              }
            }
          }
          assert( found );
        }
        initializationList += " ), ";
      }
      else
      {
        // we can ignore all other parameters here !
      }
    }
  }
  else
  {
    if ( !handle.second.secondLevelCommands.empty() )
    {
      assert( !handle.second.constructorIts.empty() );
#if !defined( NDEBUG )
      auto constructorCommandIt = m_commands.find( handle.second.constructorIts.front()->first );
#endif
      assert( ( constructorCommandIt != m_commands.end() ) && ( 1 < constructorCommandIt->second.params.size() ) );
      assert( std::next( constructorCommandIt->second.params.begin() )->type.type == "Vk" + parentType );

      auto commandIt = m_commands.find( *handle.second.secondLevelCommands.begin() );
      assert( ( commandIt != m_commands.end() ) && ( 1 < commandIt->second.params.size() ) );
      assert( commandIt->second.params.front().type.type == constructorCommandIt->second.params.front().type.type );
      assert( std::next( commandIt->second.params.begin() )->type.type == handle.first );

      std::string grandParentType = stripPrefix( commandIt->second.params.front().type.type, "Vk" );
      initializationList += "m_" + startLowerCase( grandParentType ) + "( " + parentName + ".get" + grandParentType + "() ), ";
    }
    if ( takesOwnership )
    {
      initializationList += "m_" + handleName + "( " + handleName + " ), ";
    }
  }
  return initializationList.empty() ? initializationList : initializationList.substr( 0, initializationList.size() - 2 );
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorParamName( std::string const &                                type,
                                                                        std::map<std::string, CommandData>::const_iterator destructorIt ) const
{
  if ( destructorIt != m_commands.end() )
  {
    auto destructorParamIt = std::find_if( destructorIt->second.params.begin(),
                                           destructorIt->second.params.end(),
                                           [&type]( ParamData const & destructorParam ) { return destructorParam.type.type == type; } );
    if ( destructorParamIt != destructorIt->second.params.end() )
    {
      assert( std::none_of( std::next( destructorParamIt ),
                            destructorIt->second.params.end(),
                            [&type]( ParamData const & destructorParam ) { return destructorParam.type.type == type; } ) );
      if ( !destructorParamIt->type.isValue() )
      {
        return startLowerCase( stripPrefix( stripPluralS( destructorParamIt->name ), "p" ) );
      }
      else
      {
        return destructorParamIt->name;
      }
    }
  }
  return startLowerCase( stripPrefix( type, "Vk" ) );
}

std::pair<std::string, std::string> VulkanHppGenerator::generateRAIIHandleConstructorResult( std::pair<std::string, HandleData> const &         handle,
                                                                                             std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                                             std::string const &                                enter,
                                                                                             std::string const &                                leave ) const
{
  assert( !constructorIt->second.successCodes.empty() );
  assert( constructorIt->second.successCodes[0] == "VK_SUCCESS" );
  switch ( constructorIt->second.successCodes.size() )
  {
    case 1:
      if ( !constructorIt->second.errorCodes.empty() )
      {
        std::vector<size_t> returnParams = determineReturnParams( constructorIt->second.params );
        if ( returnParams.size() == 1 )
        {
          assert( isHandleType( constructorIt->second.params[returnParams[0]].type.type ) );
          std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructorIt->second.params );
          switch ( vectorParams.size() )
          {
            case 0: return std::make_pair( generateRAIIHandleConstructorByCall( handle, constructorIt, enter, leave, false, false ), "" );
            case 1:
              if ( returnParams[0] == vectorParams.begin()->first )
              {
                if ( isLenByStructMember( constructorIt->second.params[vectorParams.begin()->first].lenExpression,
                                          constructorIt->second.params[vectorParams.begin()->second.lenParam] ) )
                {
                  return std::make_pair( "", generateRAIIHandleConstructorByCall( handle, constructorIt, enter, leave, true, false ) );
                }
              }
              break;
            case 2: return generateRAIIHandleConstructor1Return2Vector( handle, constructorIt, enter, leave, returnParams[0], vectorParams );
            default: break;
          }
        }
      }
      break;
    case 2:
    case 3:
      if ( !constructorIt->second.errorCodes.empty() )
      {
        std::vector<size_t> returnParams = determineReturnParams( constructorIt->second.params );
        switch ( returnParams.size() )
        {
          case 1:
            {
              std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructorIt->second.params );
              if ( isHandleType( constructorIt->second.params[returnParams[0]].type.type ) )
              {
                if ( vectorParams.size() == 2 )
                {
                  return generateRAIIHandleConstructor1Return2Vector( handle, constructorIt, enter, leave, returnParams[0], vectorParams );
                }
              }
              else if ( ( vectorParams.size() == 1 ) && ( vectorParams.begin()->first == returnParams[0] ) && vectorParams.begin()->second.byStructure )
              {
                assert( isHandleTypeByStructure( constructorIt->second.params[returnParams[0]].type.type ) );
                return std::make_pair( "", generateRAIIHandleConstructorByCall( handle, constructorIt, enter, leave, true, false ) );
              }
            }
            break;
          case 2:
            if ( ( constructorIt->second.params[returnParams[0]].type.type == "size_t" ) ||
                 ( constructorIt->second.params[returnParams[0]].type.type == "uint32_t" ) )
            {
              assert( isHandleType( constructorIt->second.params[returnParams[1]].type.type ) );
              std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructorIt->second.params );
              if ( vectorParams.size() == 1 )
              {
                if ( returnParams[0] == vectorParams.begin()->second.lenParam )
                {
                  assert( returnParams[1] == vectorParams.begin()->first );
                  assert( constructorIt->second.successCodes[1] == "VK_INCOMPLETE" );
                  return std::make_pair( "", generateRAIIHandleConstructorByCall( handle, constructorIt, enter, leave, true, false ) );
                }
              }
            }
            break;
          default: break;
        }
      }
      break;
    case 4:
      if ( !constructorIt->second.errorCodes.empty() )
      {
        std::vector<size_t> returnParams = determineReturnParams( constructorIt->second.params );
        if ( returnParams.size() == 1 )
        {
          assert( isHandleType( constructorIt->second.params[returnParams[0]].type.type ) );
          std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructorIt->second.params );
          if ( vectorParams.size() == 2 )
          {
            return generateRAIIHandleConstructor1Return2Vector( handle, constructorIt, enter, leave, returnParams[0], vectorParams );
          }
        }
      }
      break;
    default: break;
  }
  return std::make_pair( "", "" );
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorByCall( std::pair<std::string, HandleData> const &         handle,
                                                                     std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                     std::string const &                                enter,
                                                                     std::string const &                                leave,
                                                                     bool                                               isPlural,
                                                                     bool                                               forceSingular ) const
{
  const auto [parentType, parentName] = getParentTypeAndName( handle );

  size_t skipCount = 0;
  if ( constructorIt->second.handle == constructorIt->second.params[skipCount].type.type )
  {
    ++skipCount;
  }
  if ( handle.second.parent == constructorIt->second.params[skipCount].type.type )
  {
    ++skipCount;
  }
  std::string constructorArguments = generateRAIIHandleConstructorArguments( handle, constructorIt, forceSingular, false );
  std::string createCall           = generateCommandName(
    constructorIt->first, constructorIt->second.params, skipCount, forceSingular ? CommandFlavourFlagBits::singular : CommandFlavourFlagBits() );

  std::string createArguments;
  for ( size_t i = skipCount; i < constructorIt->second.params.size() - 1; ++i )
  {
    if ( constructorIt->second.params[i].type.isValue() || specialPointerTypes.contains( constructorIt->second.params[i].type.type ) )
    {
      createArguments += constructorIt->second.params[i].name + ", ";
    }
    else if ( constructorIt->second.params[i].type.isConstPointer() )
    {
      assert( constructorIt->second.params[i].lenParams.empty() || isPlural );
      if ( !constructorIt->second.params[i].lenParams.empty() )
      {
        assert( constructorIt->second.params[i].lenParams.size() == 1 );
        const std::string::size_type start = createArguments.find( constructorIt->second.params[i].lenParams[0].first );
        assert( start != std::string::npos );
        // remove the size/count argument, and its trailing ", "
        createArguments.replace( start, constructorIt->second.params[i].lenParams[0].first.length() + 2, "" );
      }
      std::string argument = startLowerCase( stripPrefix( constructorIt->second.params[i].name, "p" ) );
      if ( isPlural && forceSingular )
      {
        argument = stripPluralS( argument );
      }
      createArguments += argument + ", ";
    }
    else
    {
      // this is the count argument (by pointer) for an enumeration function -> skip it
    }
  }
  if ( !createArguments.empty() )
  {
    createArguments = stripPostfix( createArguments, ", " );
  }
  assert( ( constructorIt->second.params.back().type.type == handle.first ) ||
          ( vectorMemberByStructure( constructorIt->second.params.back().type.type ).type.type == handle.first ) );

  const std::string constructorTemplate =
    R"(
#if !defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
${enter}    ${handleType}${plural}( ${constructorArguments} )
    {
      *this = ${parentName}.${createCall}( ${createArguments} );
    }
${leave}#endif
)";

  return replaceWithMap( constructorTemplate,
                         { { "constructorArguments", constructorArguments },
                           { "createArguments", createArguments },
                           { "createCall", createCall },
                           { "enter", enter },
                           { "leave", leave },
                           { "handleType", stripPrefix( handle.first, "Vk" ) },
                           { "plural", isPlural && !forceSingular ? "s" : "" },
                           { "parentName", parentName } } );
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorTakeOwnership( std::pair<std::string, HandleData> const & handle ) const
{
  std::string handleType = stripPrefix( handle.first, "Vk" );
  std::string handleName = startLowerCase( handleType );

  const auto [parentType, parentName] = getParentTypeAndName( handle );

  std::string constructorArguments = generateRAIIHandleConstructorArguments( handle, handle.second.destructorIt, false, true );
  std::string initializationList   = generateRAIIHandleConstructorInitializationList( handle, handle.second.destructorIt, handle.second.destructorIt, true );
  assert( !handle.second.constructorIts.empty() );
  if ( ( 1 < handle.second.constructorIts[0]->second.successCodes.size() && ( handle.second.constructorIts[0]->second.successCodes[1] != "VK_INCOMPLETE" ) ) ||
       ( 2 < handle.second.constructorIts[0]->second.successCodes.size() ) )
  {
#if !defined( NDEBUG )
    for ( size_t i = 1; i < handle.second.constructorIts.size(); ++i )
    {
      assert( 1 < handle.second.constructorIts[i]->second.successCodes.size() );
    }
#endif
    constructorArguments += ", VULKAN_HPP_NAMESPACE::Result successCode = VULKAN_HPP_NAMESPACE::Result::eSuccess";
    if ( isMultiSuccessCodeConstructor( handle.second.constructorIts ) )
    {
      initializationList += ", m_constructorSuccessCode( successCode )";
    }
  }

  std::string dispatcherInitializer;
  if ( ( handle.first != "VkInstance" ) && ( handle.first != "VkDevice" ) )
  {
    dispatcherInitializer = "m_dispatcher( " + parentName + ".getDispatcher() )";
  }
  if ( !initializationList.empty() && !dispatcherInitializer.empty() )
  {
    initializationList += ", ";
  }

  std::string dispatcherInit;
  if ( ( handle.first == "VkDevice" ) || ( handle.first == "VkInstance" ) )
  {
    dispatcherInit = "\n        m_dispatcher.reset( new VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::detail::" + handleType + "Dispatcher( " + parentName +
                     ".getDispatcher()->vkGet" + handleType + "ProcAddr, static_cast<" + handle.first + ">( m_" + startLowerCase( handleType ) + " ) ) );";
  }

  const std::string constructorTemplate =
    R"(
    ${handleType}( ${constructorArguments} )
      : ${initializationList}${dispatcherInitializer}
    {${dispatcherInit}}
)";

  return replaceWithMap( constructorTemplate,
                         { { "constructorArguments", constructorArguments },
                           { "dispatcherInitializer", dispatcherInitializer },
                           { "dispatcherInit", dispatcherInit },
                           { "handleType", handleType },
                           { "initializationList", initializationList } } );
}

std::pair<std::string, std::string> VulkanHppGenerator::generateRAIIHandleConstructorVoid( std::pair<std::string, HandleData> const &         handle,
                                                                                           std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                                           std::string const &                                enter,
                                                                                           std::string const &                                leave ) const
{
  assert( constructorIt->second.successCodes.empty() && constructorIt->second.errorCodes.empty() );
  std::vector<size_t> returnParams = determineReturnParams( constructorIt->second.params );
  if ( returnParams.size() == 1 )
  {
    assert( isHandleType( constructorIt->second.params[returnParams[0]].type.type ) );
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructorIt->second.params );
    if ( vectorParams.empty() )
    {
      return std::make_pair( generateRAIIHandleConstructorByCall( handle, constructorIt, enter, leave, false, false ), "" );
    }
  }
  return std::make_pair( "", "" );
}

std::string VulkanHppGenerator::generateRAIIHandleContext( std::pair<std::string, HandleData> const & handle,
                                                           std::set<std::string> const &              specialFunctions ) const
{
  const std::string contextTemplate = R"(
    class Context
    {
    public:
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
      Context()
        : m_dispatcher( new VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::detail::ContextDispatcher(
            m_dynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" ) ) )
#else
      Context( PFN_vkGetInstanceProcAddr getInstanceProcAddr )
        : m_dispatcher( new VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::detail::ContextDispatcher( getInstanceProcAddr ) )
#endif
      {}

      ~Context() = default;

      Context( Context const & ) = delete;
      Context( Context && rhs ) VULKAN_HPP_NOEXCEPT
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
        : m_dynamicLoader( std::move( rhs.m_dynamicLoader ) )
        , m_dispatcher( rhs.m_dispatcher.release() )
#else
        : m_dispatcher( rhs.m_dispatcher.release() )
#endif
      {}
      Context & operator=( Context const & ) = delete;
      Context & operator=( Context && rhs ) VULKAN_HPP_NOEXCEPT
      {
        if ( this != &rhs )
        {
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
          m_dynamicLoader = std::move( rhs.m_dynamicLoader );
#endif
          m_dispatcher.reset( rhs.m_dispatcher.release() );
        }
        return *this;
      }

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::detail::ContextDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return &*m_dispatcher;
      }

      void swap( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Context & rhs )
      {
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
        std::swap( m_dynamicLoader, rhs.m_dynamicLoader );
#endif
        m_dispatcher.swap( rhs.m_dispatcher );
      }

${memberFunctionDeclarations}

    private:
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
      VULKAN_HPP_NAMESPACE::detail::DynamicLoader m_dynamicLoader;
#endif
      std::unique_ptr<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::detail::ContextDispatcher> m_dispatcher;
    };

)";

  return replaceWithMap( contextTemplate, { { "memberFunctionDeclarations", generateRAIIHandleCommandDeclarations( handle, specialFunctions ) } } );
}

std::string VulkanHppGenerator::generateRAIIHandleDestructorCallArguments( std::string const &                                handleType,
                                                                           std::map<std::string, CommandData>::const_iterator destructorIt ) const
{
  std::vector<std::string> arguments;
  for ( auto param : destructorIt->second.params )
  {
    if ( param.type.type == handleType )
    {
      std::string handleName = param.name;
      if ( param.type.isValue() )
      {
        arguments.push_back( "static_cast<" + handleType + ">( m_" + handleName + " )" );
      }
      else
      {
        arguments.push_back( "reinterpret_cast<" + handleType + " const *>( &m_" + stripPluralS( startLowerCase( stripPrefix( handleName, "p" ) ) ) + " )" );
      }
    }
    else if ( param.type.type == "VkAllocationCallbacks" )
    {
      // vk::AllocationCallbacks is stored as a member of the handle class
      arguments.push_back( "reinterpret_cast<const VkAllocationCallbacks *>( m_allocator )" );
    }
    else if ( isHandleType( param.type.type ) )
    {
      assert( param.arraySizes.empty() );
      std::string argument = "m_" + param.name;
      if ( param.type.isValue() )
      {
        arguments.push_back( "static_cast<" + param.type.type + ">( " + argument + " )" );
      }
      else
      {
        assert( param.type.isConstPointer() );
        assert( !param.lenExpression.empty() && std::any_of( destructorIt->second.params.begin(),
                                                             destructorIt->second.params.end(),
                                                             [&param]( ParamData const & pd ) { return pd.name == param.lenExpression; } ) );
        arguments.push_back( "reinterpret_cast<" + param.type.type + " const *>( &" + argument + " )" );
      }
    }
    else
    {
      assert( ( param.type.type == "uint32_t" ) && param.type.isValue() && param.arraySizes.empty() && param.lenExpression.empty() && !param.optional );
      assert( std::any_of(
        destructorIt->second.params.begin(), destructorIt->second.params.end(), [&param]( ParamData const & pd ) { return pd.lenExpression == param.name; } ) );
      arguments.push_back( "1" );
    }
  }
  return generateList( arguments, "", ", " );
}

std::tuple<std::string, std::string, std::string, std::string, std::string, std::string, std::string>
  VulkanHppGenerator::generateRAIIHandleDetails( std::pair<std::string, HandleData> const & handle ) const
{
  std::string getConstructorSuccessCode;
  const bool  multiSuccessCodeContructor = isMultiSuccessCodeConstructor( handle.second.constructorIts );
  if ( multiSuccessCodeContructor )
  {
    getConstructorSuccessCode = R"(
    VULKAN_HPP_NAMESPACE::Result getConstructorSuccessCode() const
    {
      return m_constructorSuccessCode;
    }
)";
  }

  const auto [parentType, parentName] = getParentTypeAndName( handle );

  std::string handleName = generateRAIIHandleConstructorParamName( handle.first, handle.second.destructorIt );

  std::string clearMembers, moveConstructorInitializerList, moveAssignmentInstructions, memberVariables, swapMembers, releaseMembers;

  if ( handle.second.destructorIt != m_commands.end() )
  {
    clearMembers = "        if ( m_" + handleName + " )\n";
    clearMembers += "        {\n";
    clearMembers += "          getDispatcher()->" + handle.second.destructorIt->first + "( " +
                    generateRAIIHandleDestructorCallArguments( handle.first, handle.second.destructorIt ) + " );\n";
    clearMembers += "        }";
    for ( auto const & destructorParam : handle.second.destructorIt->second.params )
    {
      std::string memberName, memberType;
      if ( destructorParam.type.type == "Vk" + parentType )
      {
        memberName = parentName;
        memberType = "VULKAN_HPP_NAMESPACE::" + parentType;
      }
      else if ( destructorParam.type.type == handle.first )
      {
        memberName = handleName;
        memberType = generateNamespacedType( handle.first );
      }
      else if ( std::none_of( handle.second.destructorIt->second.params.begin(),
                              handle.second.destructorIt->second.params.end(),
                              [&destructorParam]( ParamData const & pd ) { return pd.lenExpression == destructorParam.name; } ) )
      {
        std::string name = destructorParam.name;
        if ( !destructorParam.type.isValue() )
        {
          name = startLowerCase( stripPrefix( name, "p" ) );
        }
        memberName = name;
        memberType = destructorParam.type.compose( "VULKAN_HPP_NAMESPACE" );
      }
      if ( !memberName.empty() )
      {
        clearMembers += "\n      m_" + memberName + " = nullptr;";
        moveConstructorInitializerList += "m_" + memberName + "( VULKAN_HPP_NAMESPACE::exchange( rhs.m_" + memberName + ", {} ) ), ";
        moveAssignmentInstructions += "\n          std::swap( m_" + memberName + ", rhs.m_" + memberName + " );";
        memberVariables += "\n    " + memberType + " m_" + memberName + " = {};";
        swapMembers += "\n      std::swap( m_" + memberName + ", rhs.m_" + memberName + " );";
        if ( destructorParam.type.type != handle.first )
        {
          releaseMembers += "\n      m_" + memberName + " = nullptr;";
        }
      }
    }
  }
  else
  {
    if ( !handle.second.secondLevelCommands.empty() )
    {
      assert( !handle.second.constructorIts.empty() );
      assert( !handle.second.constructorIts.front()->second.params.empty() );
      auto const & frontType = handle.second.constructorIts.front()->second.params.front().type.type;
      assert( isHandleType( frontType ) );
#if !defined( NDEBUG )
      auto handleIt = m_handles.find( "Vk" + parentType );
#endif
      assert( handleIt != m_handles.end() );
      assert( handleIt->second.parent == frontType );
      std::string frontName = handle.second.constructorIts.front()->second.params.front().name;

      clearMembers += "\n        m_" + frontName + " = nullptr;";
      moveConstructorInitializerList = "m_" + frontName + "( VULKAN_HPP_NAMESPACE::exchange( rhs.m_" + frontName + ", {} ) ), ";
      moveAssignmentInstructions     = "\n          std::swap( m_" + frontName + ", rhs.m_" + frontName + " );";
      memberVariables                = "\n    VULKAN_HPP_NAMESPACE::" + stripPrefix( frontType, "Vk" ) + " m_" + frontName + " = {};";
      swapMembers                    = "\n      std::swap( m_" + frontName + ", rhs.m_" + frontName + " );";
      releaseMembers += "\n        m_" + frontName + " = nullptr;";
    }
    clearMembers += "\n        m_" + handleName + " = nullptr;";
    moveConstructorInitializerList += "m_" + handleName + "( VULKAN_HPP_NAMESPACE::exchange( rhs.m_" + handleName + ", {} ) ), ";
    moveAssignmentInstructions += "\n          std::swap( m_" + handleName + ", rhs.m_" + handleName + " );";
    memberVariables += "\n    " + generateNamespacedType( handle.first ) + " m_" + handleName + " = {};";
    swapMembers += "\n      std::swap( m_" + handleName + ", rhs.m_" + handleName + " );";
  }

  if ( multiSuccessCodeContructor )
  {
    clearMembers += "\n        m_constructorSuccessCode = VULKAN_HPP_NAMESPACE::Result::eErrorUnknown;";
    memberVariables += "\n    VULKAN_HPP_NAMESPACE::Result m_constructorSuccessCode = VULKAN_HPP_NAMESPACE::Result::eErrorUnknown;";
    swapMembers += "\n      std::swap( m_constructorSuccessCode, rhs.m_constructorSuccessCode );";
    moveConstructorInitializerList += "m_constructorSuccessCode( VULKAN_HPP_NAMESPACE::exchange( rhs.m_constructorSuccessCode, {} ) ), ";
    moveAssignmentInstructions += "\n          std::swap( m_constructorSuccessCode, rhs.m_constructorSuccessCode );";
    releaseMembers += "\n        m_constructorSuccessCode = VULKAN_HPP_NAMESPACE::Result::eErrorUnknown;";
  }

  if ( handle.first == "VkInstance" )
  {
    memberVariables += "\n      std::unique_ptr<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::detail::InstanceDispatcher> m_dispatcher;";
  }
  else if ( handle.first == "VkDevice" )
  {
    memberVariables += "\n      std::unique_ptr<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::detail::DeviceDispatcher> m_dispatcher;";
  }
  else if ( handle.second.constructorIts.front()->second.params.front().type.type == "VkDevice" )
  {
    memberVariables += "\n      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::detail::DeviceDispatcher const * m_dispatcher = nullptr;";
  }
  else
  {
    memberVariables += "\n      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::detail::InstanceDispatcher const * m_dispatcher = nullptr;";
  }
  clearMembers += "\n        m_dispatcher = nullptr;";
  swapMembers += "\n      std::swap( m_dispatcher, rhs.m_dispatcher );";
  releaseMembers += "\n        m_dispatcher = nullptr;";
  releaseMembers += "\n        return VULKAN_HPP_NAMESPACE::exchange( m_" + handleName + ", nullptr );";

  if ( ( handle.first == "VkInstance" ) || ( handle.first == "VkDevice" ) )
  {
    moveConstructorInitializerList += "m_dispatcher( rhs.m_dispatcher.release() )";
  }
  else
  {
    moveConstructorInitializerList += "m_dispatcher( VULKAN_HPP_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )";
  }
  moveAssignmentInstructions += "\n        std::swap( m_dispatcher, rhs.m_dispatcher );";

  return std::make_tuple(
    clearMembers, getConstructorSuccessCode, memberVariables, moveConstructorInitializerList, moveAssignmentInstructions, swapMembers, releaseMembers );
}

std::string VulkanHppGenerator::generateRAIIHandleForwardDeclarations( std::vector<RequireData> const & requireData, std::string const & title ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto handleIt = m_handles.find( type.name );
      if ( handleIt != m_handles.end() )
      {
        str += "  class " + stripPrefix( handleIt->first, "Vk" ) + ";\n";
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateRAIIHandles() const
{
  const std::string raiiHandlesTemplate = R"(
  //========================================
  //=== RAII HANDLE forward declarations ===
  //========================================

${forwardDeclarations}

  //====================
  //=== RAII HANDLES ===
  //====================

  template <typename Type>
  struct isVulkanRAIIHandleType
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool value = false;
  };

${raiiHandles}

  // operators to compare vk::raii-handles
#if defined(VULKAN_HPP_HAS_SPACESHIP_OPERATOR)
  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::isVulkanRAIIHandleType<T>::value,bool>::type = 0>
  auto operator<=>( T const & a, T const & b ) VULKAN_HPP_NOEXCEPT
  {
    return *a <=> *b;
  }
#else
  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::isVulkanRAIIHandleType<T>::value,bool>::type = 0>
  bool operator==( T const & a, T const & b ) VULKAN_HPP_NOEXCEPT
  {
    return *a == *b;
  }

  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::isVulkanRAIIHandleType<T>::value,bool>::type = 0>
  bool operator!=(T const & a, T const & b ) VULKAN_HPP_NOEXCEPT
  {
    return *a != *b;
  }

  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::isVulkanRAIIHandleType<T>::value,bool>::type = 0>
  bool operator<(T const & a, T const & b ) VULKAN_HPP_NOEXCEPT
  {
    return *a < *b;
  }
#endif

  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::isVulkanRAIIHandleType<T>::value,bool>::type = 0>
  bool operator==( const T & v, std::nullptr_t ) VULKAN_HPP_NOEXCEPT
  {
    return !*v;
  }

  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::isVulkanRAIIHandleType<T>::value,bool>::type = 0>
  bool operator==( std::nullptr_t, const T & v ) VULKAN_HPP_NOEXCEPT
  {
    return !*v;
  }

  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::isVulkanRAIIHandleType<T>::value,bool>::type = 0>
  bool operator!=( const T & v, std::nullptr_t ) VULKAN_HPP_NOEXCEPT
  {
    return *v;
  }

  template <typename T, typename std::enable_if<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::isVulkanRAIIHandleType<T>::value,bool>::type = 0>
  bool operator!=( std::nullptr_t, const T & v ) VULKAN_HPP_NOEXCEPT
  {
    return *v;
  }
)";

  std::string forwardDeclarations;
  for ( auto const & feature : m_features )
  {
    forwardDeclarations += generateRAIIHandleForwardDeclarations( feature.requireData, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    forwardDeclarations += generateRAIIHandleForwardDeclarations( extension.requireData, extension.name );
  }

  std::set<std::string> listedHandles;
  auto                  handleIt = m_handles.begin();
  assert( handleIt->first.empty() );
  std::string raiiHandles = generateRAIIHandleContext( *handleIt, m_RAIISpecialFunctions );
  for ( ++handleIt; handleIt != m_handles.end(); ++handleIt )
  {
    if ( isTypeUsed( handleIt->first ) )
    {
      raiiHandles += generateRAIIHandle( *handleIt, listedHandles, m_RAIISpecialFunctions );
    }
  }
  return replaceWithMap( raiiHandlesTemplate, { { "forwardDeclarations", forwardDeclarations }, { "raiiHandles", raiiHandles } } );
}

std::string VulkanHppGenerator::generateRAIIHandleSingularConstructorArguments( std::pair<std::string, HandleData> const & handle,
                                                                                std::vector<ParamData> const &             params,
                                                                                std::string const &                        argumentType,
                                                                                std::string const &                        argumentName ) const
{
  std::string arguments = "*reinterpret_cast<" + argumentType + " *>( &" + argumentName + " )";
  if ( handle.second.destructorIt != m_commands.end() )
  {
    auto [parentType, parentName] = getParentTypeAndName( handle );
    parentType                    = "Vk" + parentType;
    for ( auto const & destructorParam : handle.second.destructorIt->second.params )
    {
      if ( ( destructorParam.type.type != parentType ) && ( destructorParam.type.type != handle.first ) &&
           std::none_of( handle.second.destructorIt->second.params.begin(),
                         handle.second.destructorIt->second.params.end(),
                         [&destructorParam]( ParamData const & pd ) { return pd.lenExpression == destructorParam.name; } ) )
      {
        if ( std::any_of( params.begin(), params.end(), [&destructorParam]( ParamData const & pd ) { return pd.type.type == destructorParam.type.type; } ) )
        {
          if ( isHandleType( destructorParam.type.type ) )
          {
            assert( destructorParam.type.isValue() );
            arguments += ", static_cast<" + destructorParam.type.type + ">( *" + destructorParam.name + " )";
          }
          else
          {
            assert( destructorParam.type.type == "VkAllocationCallbacks" );
            arguments += ", allocator";
          }
        }
        else
        {
#if !defined( NDEBUG )
          bool found = false;
#endif
          for ( auto const & param : params )
          {
            auto structureIt = m_structs.find( param.type.type );
            if ( structureIt != m_structs.end() )
            {
              auto memberIt = findStructMemberItByType( destructorParam.type.type, structureIt->second.members );
              if ( memberIt != structureIt->second.members.end() )
              {
#if !defined( NDEBUG )
                found = true;
#endif
                assert( !param.type.isValue() );
                std::string argument = startLowerCase( stripPrefix( param.name, "p" ) ) + "." + memberIt->name;
                if ( isHandleType( memberIt->type.type ) )
                {
                  argument = "static_cast<" + memberIt->type.type + ">( " + argument + " )";
                }
                arguments += ", " + argument;
                break;
              }
            }
          }
          assert( found );
        }
      }
    }
  }
  return arguments;
}

template <class Predicate, class Extraction>
std::string VulkanHppGenerator::generateReplacedExtensionsList( Predicate p, Extraction e ) const
{
  std::string extensionsList, previousEnter, previousLeave;
  for ( auto const & extension : m_extensions )
  {
    if ( p( extension ) )
    {
      const auto [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
      extensionsList +=
        ( ( previousEnter != enter ) ? ( "\n" + previousLeave + enter ) : "\n" ) + "{ \"" + extension.name + "\", \"" + e( extension ) + "\"}, ";
      previousEnter = enter;
      previousLeave = leave;
    }
  }
  if ( !extensionsList.empty() )
  {
    assert( extensionsList.ends_with( ", " ) );
    extensionsList = extensionsList.substr( 0, extensionsList.length() - 2 );
    if ( !previousLeave.empty() )
    {
      extensionsList += "\n" + previousLeave;
    }
  }
  return extensionsList;
}

std::string VulkanHppGenerator::generateResultAssignment( CommandData const & commandData ) const
{
  std::string resultAssignment;
  if ( ( commandData.returnType != "void" ) &&
       !( ( commandData.returnType == "VkResult" ) && ( commandData.successCodes.size() == 1 ) && commandData.errorCodes.empty() ) )
  {
    resultAssignment = commandData.returnType + " result = ";
  }
  return resultAssignment;
}

std::string VulkanHppGenerator::generateResultCheck(
  CommandData const & commandData, std::string const & className, std::string const & classSeparator, std::string commandName, bool enumerating ) const
{
  std::string resultCheck;
  if ( !commandData.errorCodes.empty() )
  {
    std::string successCodeList = generateSuccessCodeList( commandData.successCodes, enumerating );

    std::string const resultCheckTemplate =
      R"(VULKAN_HPP_NAMESPACE::detail::resultCheck( result, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}"${successCodeList} );)";

    resultCheck = replaceWithMap(
      resultCheckTemplate,
      { { "className", className }, { "classSeparator", classSeparator }, { "commandName", commandName }, { "successCodeList", successCodeList } } );
  }
  return resultCheck;
}

std::string VulkanHppGenerator::generateResultCheckExpected( std::vector<std::string> const & successCodes,
                                                             std::string const &              className,
                                                             std::string const &              commandName ) const
{
  std::string resultCheck;
  if ( !successCodes.empty() )
  {
    std::string condition = "result != " + generateSuccessCode( successCodes[0] );
    if ( 1 < successCodes.size() )
    {
      condition = "( " + condition + " )";
      for ( size_t i = 1; i < successCodes.size(); ++i )
      {
        condition += " && ( result != " + generateSuccessCode( successCodes[i] ) + " )";
      }
    }

    std::string const resultCheckTemplate = R"(if ( ${condition} )
{
#if defined( VULKAN_HPP_RAII_NO_EXCEPTIONS )
  return VULKAN_HPP_UNEXPECTED( result );
#else
  VULKAN_HPP_NAMESPACE::detail::throwResultException( result, "${className}::${commandName}" );
#endif
}
)";

    resultCheck = replaceWithMap( resultCheckTemplate, { { "className", className }, { "commandName", commandName }, { "condition", condition } } );
  }
  return resultCheck;
}

// Intended only for `enum class Result`!
std::string VulkanHppGenerator::generateResultExceptions() const
{
  const std::string templateString = R"(
${enter}  class ${className} : public SystemError
  {
  public:
    ${className}( std::string const & message )
      : SystemError( make_error_code( ${enumName}::${enumMemberName} ), message ) {}
    ${className}( char const * message )
      : SystemError( make_error_code( ${enumName}::${enumMemberName} ), message ) {}
  };
${leave})";

  std::string str;
  auto        enumIt = m_enums.find( "VkResult" );
  for ( auto const & value : enumIt->second.values )
  {
    if ( value.supported && value.name.starts_with( "VK_ERROR" ) )
    {
      auto [enter, leave]   = generateProtection( value.protect );
      std::string valueName = generateEnumValueName( enumIt->first, value.name, false );
      str += replaceWithMap( templateString,
                             { { "className", stripPrefix( valueName, "eError" ) + "Error" },
                               { "enter", enter },
                               { "enumName", stripPrefix( enumIt->first, "Vk" ) },
                               { "enumMemberName", valueName },
                               { "leave", leave } } );
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateReturnStatement( std::string const & commandName,
                                                         CommandData const & commandData,
                                                         std::string const & returnVariable,
                                                         std::string const & returnType,
                                                         std::string const & decoratedReturnType,
                                                         std::string const & dataType,
                                                         size_t              initialSkipCount,
                                                         size_t              returnParam,
                                                         CommandFlavourFlags flavourFlags,
                                                         bool                enumerating,
                                                         bool                raii ) const
{
  const bool unique = flavourFlags & CommandFlavourFlagBits::unique;

  std::string returnStatement;
  if ( commandData.returnType.starts_with( "Vk" ) )
  {
    if ( ( commandData.successCodes.size() == 1 ) || ( enumerating && ( commandData.successCodes.size() == 2 ) ) )
    {
      assert( commandData.successCodes[0] == "VK_SUCCESS" );
      if ( raii || commandData.errorCodes.empty() )
      {
        if ( !returnVariable.empty() )
        {
          returnStatement = "return " + returnVariable + ";";
        }
      }
      else
      {
        if ( returnVariable.empty() )
        {
          assert( !unique );
          returnStatement = "return VULKAN_HPP_NAMESPACE::detail::createResultValueType( result );";
        }
        else if ( unique )
        {
          assert( returnParam != INVALID_INDEX );
          returnStatement = "return VULKAN_HPP_NAMESPACE::detail::createResultValueType( result, ";
          if ( dataType.starts_with( "std::" ) )
          {
            returnStatement += "std::move( unique" + startUpperCase( returnVariable ) + " )";
          }
          else
          {
            assert( dataType.starts_with( "VULKAN_HPP_NAMESPACE::" ) );
            returnStatement +=
              returnType + "( " + returnVariable + ", " + generateObjectDeleter( commandName, commandData, initialSkipCount, returnParam ) + " )";
          }
          returnStatement += " );";
        }
        else
        {
          returnStatement = "return VULKAN_HPP_NAMESPACE::detail::createResultValueType( result, std::move( " + returnVariable + " ) );";
        }
      }
    }
    else
    {
      if ( returnVariable.empty() )
      {
        assert( !unique );
        returnStatement = "return static_cast<VULKAN_HPP_NAMESPACE::" + stripPrefix( commandData.returnType, "Vk" ) + ">( result );";
      }
      else if ( unique )
      {
        assert( returnParam != INVALID_INDEX );
        assert( decoratedReturnType.starts_with( "ResultValue<" ) && decoratedReturnType.ends_with( ">" ) );
        returnStatement = "return " + decoratedReturnType + "( result, ";
        if ( dataType.starts_with( "std::" ) )
        {
          returnStatement += "std::move( unique" + startUpperCase( returnVariable ) + " )";
        }
        else
        {
          returnStatement += "UniqueHandle<" + dataType + ", Dispatch>( " + returnVariable + ", " +
                             generateObjectDeleter( commandName, commandData, initialSkipCount, returnParam ) + " )";
        }
        returnStatement += " );";
      }
      else
      {
        assert( decoratedReturnType.starts_with( raii ? "std::pair<VULKAN_HPP_NAMESPACE::Result, " : "ResultValue<" ) && decoratedReturnType.ends_with( ">" ) );
        returnStatement = "return " + ( raii ? "std::make_pair" : decoratedReturnType ) + "( result, std::move( " + returnVariable + " ) );";
      }
    }
  }
  else
  {
    assert( !unique );
    if ( returnVariable.empty() )
    {
      if ( commandData.returnType != "void" )
      {
        returnStatement = "return result;";
      }
    }
    else
    {
      returnStatement = "return " + returnVariable + ";";
    }
  }
  return returnStatement;
}

std::string VulkanHppGenerator::generateReturnType( std::vector<size_t> const &               returnParams,
                                                    std::map<size_t, VectorParamData> const & vectorParams,
                                                    CommandFlavourFlags                       flavourFlags,
                                                    bool                                      raii,
                                                    std::vector<std::string> const &          dataTypes ) const
{
  assert( returnParams.size() == dataTypes.size() );
  const bool chained  = flavourFlags & CommandFlavourFlagBits::chained;
  const bool singular = flavourFlags & CommandFlavourFlagBits::singular;
  const bool unique   = flavourFlags & CommandFlavourFlagBits::unique;
  assert( !( chained && unique ) );

  std::string returnType;
  switch ( returnParams.size() )
  {
    case 0: returnType = "void"; break;
    case 1:
      if ( chained )
      {
        assert( vectorParams.empty() );
        returnType = "VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>";
      }
      else if ( unique )
      {
        if ( vectorParams.contains( returnParams[0] ) && !singular )
        {
          returnType = "std::vector<UniqueHandle<" + dataTypes[0] + ", Dispatch>, " + stripPrefix( dataTypes[0], "VULKAN_HPP_NAMESPACE::" ) + "Allocator>";
        }
        else
        {
          returnType = "UniqueHandle<" + dataTypes[0] + ", Dispatch>";
        }
      }
      else
      {
        if ( vectorParams.contains( returnParams[0] ) && !singular )
        {
          returnType = "std::vector<" + dataTypes[0] + ( raii ? "" : ( ", " + stripPrefix( dataTypes[0], "VULKAN_HPP_NAMESPACE::" ) + "Allocator" ) ) + ">";
        }
        else if ( dataTypes[0].starts_with( "Vk" ) )
        {
          returnType = "VULKAN_HPP_NAMESPACE::" + stripPrefix( dataTypes[0], "Vk" );
        }
        else
        {
          returnType = dataTypes[0];
        }
      }
      break;
    case 2:
      assert( !unique );
      if ( chained )
      {
#if !defined( NDEBUG )
        auto vectorIt = vectorParams.find( returnParams[1] );
#endif
        assert( ( vectorIt != vectorParams.end() ) && ( vectorIt->second.lenParam == returnParams[0] ) );
        returnType = std::string( "std::vector<StructureChain" ) + ( raii ? "" : ", StructureChainAllocator" ) + ">";
      }
      else
      {
        if ( vectorParams.contains( returnParams[0] ) )
        {
          if ( singular )
          {
            returnType = "std::pair<" + dataTypes[0] + ", " + dataTypes[1] + ">";
          }
          else
          {
            returnType =
              "std::pair<std::vector<" + dataTypes[0] + ( raii ? "" : ( ", " + startUpperCase( dataTypes[0] ) + "Allocator" ) ) + ">, " + dataTypes[1] + ">";
          }
        }
        else if ( vectorParams.contains( returnParams[1] ) )
        {
          assert( vectorParams.find( returnParams[1] )->second.lenParam == returnParams[0] );
          if ( dataTypes[1].starts_with( "Vk" ) )
          {
            std::string type = stripPrefix( dataTypes[1], "Vk" );
            returnType       = "std::vector<VULKAN_HPP_NAMESPACE::" + type + ( raii ? "" : ( ", " + type + "Allocator" ) ) + ">";
          }
          else
          {
            returnType = "std::vector<" + dataTypes[1] +
                         ( raii ? "" : ( ", " + startUpperCase( stripPrefix( dataTypes[1], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator" ) ) + ">";
          }
        }
        else
        {
          returnType = "std::pair<" + dataTypes[0] + ", " + dataTypes[1] + ">";
        }
      }
      break;
    case 3:
      assert( !unique );
      if ( chained )
      {
        assert( dataTypes[2] == "uint8_t" );
#if !defined( NDEBUG )
        auto vectorIt = vectorParams.find( returnParams[2] );
        assert( ( vectorIt != vectorParams.end() ) && ( vectorIt->second.lenParam == returnParams[1] ) );
        assert( !vectorParams.contains( returnParams[0] ) );
#endif
        returnType = "std::pair<VULKAN_HPP_NAMESPACE::StructureChain<X, Y, Z...>, std::vector<" + dataTypes[2] +
                     ( raii ? "" : ( ", " + startUpperCase( dataTypes[2] ) + "Allocator" ) ) + ">>";
      }
      else
      {
        if ( vectorParams.contains( returnParams[1] ) )
        {
#if !defined( NDEBUG )
          auto vectorIt = vectorParams.find( returnParams[1] );
          assert( ( vectorIt != vectorParams.end() ) && ( vectorIt->second.lenParam == returnParams[0] ) );
          vectorIt = vectorParams.find( returnParams[2] );
          assert( ( vectorIt != vectorParams.end() ) && ( vectorIt->second.lenParam == returnParams[0] ) );
#endif
          returnType = "std::pair<std::vector<" + dataTypes[1] +
                       ( raii ? "" : ( ", " + stripPrefix( dataTypes[1], "VULKAN_HPP_NAMESPACE::" ) + "Allocator" ) ) + ">, std::vector<" + dataTypes[2] +
                       ( raii ? "" : ( ", " + stripPrefix( dataTypes[2], "VULKAN_HPP_NAMESPACE::" ) + "Allocator" ) ) + ">>";
        }
        else
        {
#if !defined( NDEBUG )
          auto vectorIt = vectorParams.find( returnParams[2] );
          assert( ( vectorIt != vectorParams.end() ) && ( vectorIt->second.lenParam == returnParams[1] ) );
          assert( !vectorParams.contains( returnParams[0] ) );
#endif
          returnType = "std::pair<" + dataTypes[0] + ", std::vector<" + dataTypes[2] +
                       ( raii ? "" : ( ", " + startUpperCase( stripPrefix( dataTypes[2], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator" ) ) + ">>";
        }
      }
      break;
    default: assert( false );
  }

  return returnType;
}

std::string VulkanHppGenerator::generateReturnVariable( CommandData const &                       commandData,
                                                        std::vector<size_t> const &               returnParams,
                                                        std::map<size_t, VectorParamData> const & vectorParams,
                                                        CommandFlavourFlags                       flavourFlags ) const
{
  const bool chained  = flavourFlags & CommandFlavourFlagBits::chained;
  const bool singular = flavourFlags & CommandFlavourFlagBits::singular;

  std::string returnVariable;
  switch ( returnParams.size() )
  {
    case 0: break;  // no return variable
    case 1:
      if ( chained )
      {
        returnVariable = "structureChain";
      }
      else
      {
        auto vectorParamIt = vectorParams.find( returnParams[0] );
        if ( ( vectorParamIt != vectorParams.end() ) && vectorParamIt->second.byStructure )
        {
          returnVariable = startLowerCase( stripPrefix( vectorMemberByStructure( commandData.params[returnParams[0]].type.type ).name, "p" ) );
        }
        else
        {
          returnVariable = startLowerCase( stripPrefix( commandData.params[returnParams[0]].name, "p" ) );
        }
        if ( singular )
        {
          returnVariable = stripPluralS( returnVariable );
        }
      }
      break;
    case 2:
      if ( chained )
      {
        returnVariable = "structureChains";
      }
      else
      {
        auto vectorParamIt = vectorParams.find( returnParams[1] );
        if ( vectorParamIt == vectorParams.end() )
        {  // the return variable is simply named "data_", and holds the multi-return value stuff
          returnVariable = "data_";
        }
        else
        {
          assert( vectorParamIt->second.lenParam == returnParams[0] );
          assert( !singular );
          returnVariable = startLowerCase( stripPrefix( commandData.params[returnParams[1]].name, "p" ) );
        }
      }
      break;
    case 3:
      assert( !singular );
      assert( ( ( vectorParams.size() == 1 ) && ( vectorParams.begin()->first == returnParams[2] ) &&
                ( vectorParams.begin()->second.lenParam == returnParams[1] ) ) ||
              ( ( vectorParams.size() == 2 ) && ( vectorParams.begin()->first == returnParams[1] ) &&
                ( vectorParams.begin()->second.lenParam == returnParams[0] ) && ( std::next( vectorParams.begin() )->first == returnParams[2] ) &&
                ( std::next( vectorParams.begin() )->second.lenParam == returnParams[0] ) ) );
      returnVariable = "data_";
      break;
    default: assert( false ); break;
  }
  return returnVariable;
}

std::string VulkanHppGenerator::generateSizeCheck( std::vector<std::vector<MemberData>::const_iterator> const & arrayIts,
                                                   std::string const &                                          structName,
                                                   bool                                                         mutualExclusiveLens ) const
{
  std::string sizeCheck;
  if ( 1 < arrayIts.size() )
  {
    static const std::string throwTextTemplate = R"(      if ( ${throwCheck} )
      {
        throw LogicError( VULKAN_HPP_NAMESPACE_STRING"::${structName}::${structName}: ${throwCheck}" );
      }
)";

    std::string assertionText, throwText;
    if ( mutualExclusiveLens )
    {
      // exactly one of the arrays has to be non-empty
      std::string sum;
      for ( auto it : arrayIts )
      {
        sum += "!" + startLowerCase( stripPrefix( it->name, "p" ) ) + "_.empty() + ";
      }
      sum.resize( sum.size() - 3 );
      assertionText += "      VULKAN_HPP_ASSERT( ( " + sum + " ) <= 1);\n";
      throwText += replaceWithMap( throwTextTemplate, { { "structName", structName }, { "throwCheck", "1 < ( " + sum + " )" } } );
    }
    else
    {
      for ( size_t first = 0; first + 1 < arrayIts.size(); ++first )
      {
        assert( arrayIts[first]->name.starts_with( "p" ) );
        std::string firstName = startLowerCase( stripPrefix( arrayIts[first]->name, "p" ) ) + "_";
        for ( auto second = first + 1; second < arrayIts.size(); ++second )
        {
          assert( arrayIts[second]->name.starts_with( "p" ) );
          std::string secondName     = startLowerCase( stripPrefix( arrayIts[second]->name, "p" ) ) + "_";
          std::string assertionCheck = firstName + ".size() == " + secondName + ".size()";
          std::string throwCheck     = firstName + ".size() != " + secondName + ".size()";
          if ( ( !arrayIts[first]->optional.empty() && arrayIts[first]->optional.front() ) ||
               ( !arrayIts[second]->optional.empty() && arrayIts[second]->optional.front() ) )
          {
            assertionCheck = "( " + assertionCheck + " )";
            throwCheck     = "( " + throwCheck + " )";
            if ( !arrayIts[second]->optional.empty() && arrayIts[second]->optional.front() )
            {
              assertionCheck = secondName + ".empty() || " + assertionCheck;
              throwCheck     = "!" + secondName + ".empty() && " + throwCheck;
            }
            if ( !arrayIts[first]->optional.empty() && arrayIts[first]->optional.front() )
            {
              assertionCheck = firstName + ".empty() || " + assertionCheck;
              throwCheck     = "!" + firstName + ".empty() && " + throwCheck;
            }
          }
          assertionText += "      VULKAN_HPP_ASSERT( " + assertionCheck + " );\n";
          throwText += replaceWithMap( throwTextTemplate, { { "structName", structName }, { "throwCheck", throwCheck } } );
        }
      }
    }
    sizeCheck += "\n#ifdef VULKAN_HPP_NO_EXCEPTIONS\n" + assertionText + "#else\n" + throwText + "#endif /*VULKAN_HPP_NO_EXCEPTIONS*/\n" + "    ";
  }
  return sizeCheck;
}

std::string VulkanHppGenerator::generateStaticAssertions() const
{
  std::string           staticAssertions;
  std::set<std::string> listedStructs;
  for ( auto const & feature : m_features )
  {
    staticAssertions += generateStaticAssertions( feature.requireData, feature.name, listedStructs );
  }
  for ( auto const & extension : m_extensions )
  {
    staticAssertions += generateStaticAssertions( extension.requireData, extension.name, listedStructs );
  }
  return staticAssertions;
}

std::string VulkanHppGenerator::generateStaticAssertions( std::vector<RequireData> const & requireData,
                                                          std::string const &              title,
                                                          std::set<std::string> &          listedStructs ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto handleIt = m_handles.find( type.name );
      if ( handleIt != m_handles.end() )
      {
        std::string const assertionTemplate = R"(
VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::${className} ) == sizeof( Vk${className} ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_copy_constructible<VULKAN_HPP_NAMESPACE::${className}>::value, "${className} is not copy_constructible!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::${className}>::value, "${className} is not nothrow_move_constructible!" );
)";
        str += replaceWithMap( assertionTemplate, { { "className", stripPrefix( handleIt->first, "Vk" ) } } );
      }
      else
      {
        auto structIt = m_structs.find( type.name );
        if ( ( structIt != m_structs.end() ) && listedStructs.insert( type.name ).second )
        {
          std::string const assertionTemplate = R"(
VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::${structureType} ) == sizeof( Vk${structureType} ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::${structureType}>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::${structureType}>::value, "${structureType} is not nothrow_move_constructible!" );
)";
          str += replaceWithMap( assertionTemplate, { { "structureType", stripPrefix( structIt->first, "Vk" ) } } );
        }
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateStruct( std::pair<std::string, StructureData> const & structure, std::set<std::string> & listedStructs ) const
{
  assert( !listedStructs.contains( structure.first ) );

  std::string str;
  for ( auto const & member : structure.second.members )
  {
    auto typeIt = m_types.find( member.type.type );
    assert( typeIt != m_types.end() );
    if ( ( typeIt->second.category == TypeCategory::Struct ) || ( typeIt->second.category == TypeCategory::Union ) )
    {
      auto structIt = findByNameOrAlias( m_structs, member.type.type );
      assert( structIt != m_structs.end() );
      if ( ( structure.first != member.type.type ) && !listedStructs.contains( member.type.type ) )
      {
        str += generateStruct( *structIt, listedStructs );
      }
    }
    else if ( typeIt->second.category == TypeCategory::FuncPointer )
    {
      auto funcPtrIt = m_funcPointers.find( member.type.type );
      assert( funcPtrIt != m_funcPointers.end() );
      if ( !listedStructs.contains( member.type.type ) )
      {
        str += generateFuncPointer( *funcPtrIt, listedStructs );
      }
    }
  }

  if ( !structure.second.subStruct.empty() )
  {
    auto structureIt = m_structs.find( structure.second.subStruct );
    if ( ( structureIt != m_structs.end() ) && !listedStructs.contains( structureIt->first ) )
    {
      str += generateStruct( *structureIt, listedStructs );
    }
  }

  if ( structure.second.isUnion )
  {
    str += generateUnion( structure );
  }
  else
  {
    str += generateStructure( structure );
  }

  listedStructs.insert( structure.first );
  return str;
}

std::string VulkanHppGenerator::generateStructCompareOperators( std::pair<std::string, StructureData> const & structData ) const
{
  static const std::set<std::string> simpleTypes = { "char",   "double",  "DWORD",    "float",    "HANDLE",  "HINSTANCE", "HMONITOR",
                                                     "HWND",   "int",     "int8_t",   "int16_t",  "int32_t", "int64_t",   "LPCWSTR",
                                                     "size_t", "uint8_t", "uint16_t", "uint32_t", "uint64_t" };
  // two structs are compared by comparing each of the elements
  std::string compareMembers, spaceshipMembers;
  std::string intro             = "";
  bool        nonDefaultCompare = false;
  std::string spaceshipOrdering = containsFloatingPoints( structData.second.members ) ? "std::partial_ordering" : "std::strong_ordering";
  for ( size_t i = 0; i < structData.second.members.size(); i++ )
  {
    MemberData const & member = structData.second.members[i];
    auto               typeIt = m_types.find( member.type.type );
    assert( typeIt != m_types.end() );
    if ( ( typeIt->second.category == TypeCategory::ExternalType ) && member.type.postfix.empty() && !simpleTypes.contains( member.type.type ) )
    {
      nonDefaultCompare = true;
      // this type might support operator==() or operator<=>()... that is, use memcmp
      compareMembers += intro + "( memcmp( &" + member.name + ", &rhs." + member.name + ", sizeof( " + member.type.type + " ) ) == 0 )";

      static const std::string spaceshipMemberTemplate =
        R"(      if ( auto cmp = memcmp( &${name}, &rhs.${name}, sizeof( ${type} ) ); cmp != 0 )
        return ( cmp < 0 ) ? ${ordering}::less : ${ordering}::greater;
)";
      spaceshipMembers +=
        replaceWithMap( spaceshipMemberTemplate, { { "name", member.name }, { "ordering", spaceshipOrdering }, { "type", member.type.type } } );
    }
    else if ( member.type.type == "char" && !member.lenExpressions.empty() )
    {
      // compare null-terminated strings
      nonDefaultCompare = true;
      assert( member.lenExpressions.size() < 3 );
      if ( member.lenExpressions.size() == 1 )
      {
        assert( member.lenExpressions[0] == "null-terminated" );
        if ( member.arraySizes.empty() )
        {
          compareMembers += intro + "( ( " + member.name + " == rhs." + member.name + " ) || ( strcmp( " + member.name + ", rhs." + member.name + " ) == 0 ) )";

          static const std::string spaceshipMemberTemplate =
            R"(     if ( ${name} != rhs.${name} )
        if ( auto cmp = strcmp( ${name}, rhs.${name} ); cmp != 0 )
          return ( cmp < 0 ) ? ${ordering}::less : ${ordering}::greater;
)";
          spaceshipMembers += replaceWithMap( spaceshipMemberTemplate, { { "name", member.name }, { "ordering", spaceshipOrdering } } );
        }
        else
        {
          assert( member.arraySizes.size() == 1 );
          compareMembers += intro + "( strcmp( " + member.name + ", rhs." + member.name + " ) == 0 )";

          static const std::string spaceshipMemberTemplate =
            R"(     if ( auto cmp = strcmp( ${name}, rhs.${name} ); cmp != 0 )
          return ( cmp < 0 ) ? ${ordering}::less : ${ordering}::greater;
)";
          spaceshipMembers += replaceWithMap( spaceshipMemberTemplate, { { "name", member.name }, { "ordering", spaceshipOrdering } } );
        }
      }
      else
      {
        assert( member.lenExpressions[1] == "null-terminated" );
        assert( ( member.type.prefix == "const" ) && ( member.type.postfix == "* const *" ) );
        static const std::string compareMemberTemplate =
          R"(std::equal( ${name}, ${name} + ${count}, rhs.${name}, []( char const * left, char const * right ) { return ( left == right ) || ( strcmp( left, right ) == 0 ); } ))";
        compareMembers += intro + replaceWithMap( compareMemberTemplate, { { "count", member.lenExpressions[0] }, { "name", member.name } } );

        static const std::string spaceshipMemberTemplate = R"(      for ( size_t i = 0; i < ${count}; ++i )
      {
        if ( ${name}[i] != rhs.${name}[i] )
          if ( auto cmp = strcmp( ${name}[i], rhs.${name}[i] ); cmp != 0 )
            return cmp < 0 ? ${ordering}::less : ${ordering}::greater;
      }
)";
        spaceshipMembers +=
          replaceWithMap( spaceshipMemberTemplate, { { "count", member.lenExpressions[0] }, { "name", member.name }, { "ordering", spaceshipOrdering } } );
      }
    }
    else if ( !member.arraySizes.empty() && !member.lenExpressions.empty() )
    {
      nonDefaultCompare = true;

      assert( ( member.arraySizes.size() == 1 ) && ( member.lenExpressions.size() == 1 ) );
      assert( std::any_of( structData.second.members.begin(),
                           structData.second.members.end(),
                           [&member]( MemberData const & m ) { return m.name == member.lenExpressions[0]; } ) );

      std::string type = member.type.type.starts_with( "Vk" ) ? member.type.compose( "VULKAN_HPP_NAMESPACE" ) : member.type.type;

      static const std::string compareMemberTemplate = R"(( memcmp( ${name}, rhs.${name}, ${count} * sizeof( ${type} ) ) == 0 ))";
      compareMembers += intro + replaceWithMap( compareMemberTemplate, { { "count", member.lenExpressions[0] }, { "name", member.name }, { "type", type } } );

      static const std::string spaceshipMemberTemplate = R"(      for ( size_t i = 0; i < ${count}; ++i )
      {
        if ( auto cmp = ${name}[i] <=> rhs.${name}[i]; cmp != 0 ) return cmp;
      }
)";
      spaceshipMembers += replaceWithMap( spaceshipMemberTemplate, { { "count", member.lenExpressions[0] }, { "name", member.name } } );
    }
    else
    {
      // for all others, we use the operator== of that type
      compareMembers += intro + "( " + member.name + " == rhs." + member.name + " )";
      spaceshipMembers += "      if ( auto cmp = " + member.name + " <=> rhs." + member.name + "; cmp != 0 ) return cmp;\n";
    }
    intro = "\n          && ";
  }

  std::string structName = stripPrefix( structData.first, "Vk" );

  std::string compareBody, spaceshipOperator, spaceshipOperatorIf, spaceshipOperatorElse, spaceshipOperatorEndif;
  if ( nonDefaultCompare )
  {
    compareBody = "      return " + compareMembers + ";";

    if ( !containsFuncPointer( structData.first ) )
    {
      static const std::string spaceshipOperatorTemplate =
        R"(    ${ordering} operator<=>( ${name} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
${spaceshipMembers}
      return ${ordering}::equivalent;
    })";
      spaceshipOperatorIf = "#if defined(VULKAN_HPP_HAS_SPACESHIP_OPERATOR)";
      spaceshipOperator =
        replaceWithMap( spaceshipOperatorTemplate, { { "name", structName }, { "ordering", spaceshipOrdering }, { "spaceshipMembers", spaceshipMembers } } );
      spaceshipOperatorElse  = "#endif\n";
      spaceshipOperatorEndif = "";
    }
  }
  else
  {
    // use reflection only if VULKAN_HPP_USE_REFLECT is defined
    static const std::string compareBodyTemplate = R"(#if defined( VULKAN_HPP_USE_REFLECT )
      return this->reflect() == rhs.reflect();
#else
      return ${compareMembers};
#endif)";
    compareBody                                  = replaceWithMap( compareBodyTemplate, { { "compareMembers", compareMembers } } );

    if ( !containsFuncPointer( structData.first ) )
    {
      spaceshipOperatorIf    = "#if defined(VULKAN_HPP_HAS_SPACESHIP_OPERATOR)";
      spaceshipOperator      = "auto operator<=>( " + structName + " const & ) const = default;";
      spaceshipOperatorElse  = "#else";
      spaceshipOperatorEndif = "#endif\n";
    }
  }

  static const std::string compareTemplate = R"(
${spaceshipOperatorIf}
${spaceshipOperator}
${spaceshipOperatorElse}
    bool operator==( ${name} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
${compareBody}
    }

    bool operator!=( ${name} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return !operator==( rhs );
    }
${spaceshipOperatorEndif})";

  return replaceWithMap( compareTemplate,
                         { { "name", structName },
                           { "compareBody", compareBody },
                           { "spaceshipOperator", spaceshipOperator },
                           { "spaceshipOperatorElse", spaceshipOperatorElse },
                           { "spaceshipOperatorEndif", spaceshipOperatorEndif },
                           { "spaceshipOperatorIf", spaceshipOperatorIf } } );
}

std::string VulkanHppGenerator::generateStructConstructors( std::pair<std::string, StructureData> const & structData ) const
{
  // the constructor with all the elements as arguments, with defaults
  // and the simple copy constructor from the corresponding vulkan structure
  static const std::string constructors = R"(${constexpr}${structName}(${arguments}) VULKAN_HPP_NOEXCEPT
    ${initializers}
    {}

    ${constexpr}${structName}( ${structName} const & rhs ) VULKAN_HPP_NOEXCEPT = default;

    ${structName}( Vk${structName} const & rhs ) VULKAN_HPP_NOEXCEPT
      : ${structName}( *reinterpret_cast<${structName} const *>( &rhs ) )
    {}
)";

  std::vector<std::string> arguments, initializers;
  for ( auto const & member : structData.second.members )
  {
    // gather the arguments
    std::string argument = generateStructConstructorArgument( member, true );
    if ( !argument.empty() )
    {
      arguments.push_back( argument );
    }

    // gather the initializers; skip members with exactly one legal value
    if ( member.value.empty() )
    {
      initializers.push_back( member.name + "{ " + member.name + "_ }" );
    }
  }
  auto pNextIt = std::find_if( structData.second.members.begin(), structData.second.members.end(), []( MemberData const & md ) { return md.name == "pNext"; } );
  if ( pNextIt != structData.second.members.end() )
  {
    // add pNext as a last optional argument to the constructor
    arguments.push_back( pNextIt->type.compose( "VULKAN_HPP_NAMESPACE" ) + " pNext_ = nullptr" );
  }

  std::string str = replaceWithMap( constructors,
                                    { { "arguments", generateList( arguments, "", ", " ) },
                                      { "constexpr", generateConstexprString( structData.first ) },
                                      { "initializers", generateList( initializers, ": ", ", " ) },
                                      { "structName", stripPrefix( structData.first, "Vk" ) } } );

  if ( !structData.second.returnedOnly )
  {
    str += generateStructConstructorsEnhanced( structData );
  }
  return str;
}

std::string VulkanHppGenerator::generateStructConstructorsEnhanced( std::pair<std::string, StructureData> const & structData ) const
{
  // some structs needs some special handling!
  if ( structData.first == "VkLayerSettingEXT" )
  {
    assert( ( structData.second.members.size() == 5 ) && ( structData.second.members[0].name == "pLayerName" ) &&
            ( structData.second.members[1].name == "pSettingName" ) && ( structData.second.members[2].name == "type" ) &&
            ( structData.second.members[3].name == "valueCount" ) && ( structData.second.members[4].name == "pValues" ) );

    static const std::string byTypeTemplate =
      R"(    LayerSettingEXT( char const * pLayerName_, char const * pSettingName_, VULKAN_HPP_NAMESPACE::LayerSettingTypeEXT type_, vk::ArrayProxyNoTemporaries<const ${type}> const & values_ )
      : pLayerName( pLayerName_ )
      , pSettingName( pSettingName_ )
      , type( type_ )
      , valueCount( static_cast<uint32_t>( values_.size() ) )
      , pValues( values_.data() )
    {
      VULKAN_HPP_ASSERT( VULKAN_HPP_NAMESPACE::isSameType<${type}>(type) );
    })";

    static const std::string constructorTemplate = R"(
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
// NOTE: you need to provide the type because vk::Bool32 and uint32_t are indistinguishable!
${byInt32}
${byInt64}
${byUint32}
${byUint64}
${byFloat32}
${byFloat64}
${byString}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

    return replaceWithMap( constructorTemplate,
                           {
                             { "byInt32", replaceWithMap( byTypeTemplate, { { "type", "int32_t" } } ) },
                             { "byInt64", replaceWithMap( byTypeTemplate, { { "type", "int64_t" } } ) },
                             { "byUint32", replaceWithMap( byTypeTemplate, { { "type", "uint32_t" } } ) },
                             { "byUint64", replaceWithMap( byTypeTemplate, { { "type", "uint64_t" } } ) },
                             { "byFloat32", replaceWithMap( byTypeTemplate, { { "type", "float" } } ) },
                             { "byFloat64", replaceWithMap( byTypeTemplate, { { "type", "double" } } ) },
                             { "byString", replaceWithMap( byTypeTemplate, { { "type", "char *" } } ) },
                           } );
  }
  else if ( std::any_of( structData.second.members.begin(),
                         structData.second.members.end(),
                         [this, &members = structData.second.members]( MemberData const & md ) { return hasLen( md, members ); } ) )
  {
    // map from len-members to all the array members using that len
    std::map<std::vector<MemberData>::const_iterator, std::vector<std::vector<MemberData>::const_iterator>> lenIts;
    for ( auto mit = structData.second.members.begin(); mit != structData.second.members.end(); ++mit )
    {
      if ( hasLen( *mit, structData.second.members ) )
      {
        std::string lenName = ( mit->lenExpressions.front() == "codeSize / 4" ) ? "codeSize" : mit->lenExpressions.front();
        auto        lenIt   = findStructMemberIt( lenName, structData.second.members );
        assert( lenIt != mit );
        lenIts[lenIt].push_back( mit );
      }
    }

    std::vector<std::string> arguments, initializers;
    bool                     arrayListed = false;
    std::string              templateHeader, sizeChecks, copyOps;
    for ( auto mit = structData.second.members.begin(); mit != structData.second.members.end(); ++mit )
    {
      // gather the initializers
      if ( mit->name == "pNext" )  // for pNext, we just get the initializer... the argument is added at the end
      {
        initializers.push_back( "pNext( pNext_ )" );
      }
      else if ( mit->value.empty() )  // skip constant members
      {
        auto litit = lenIts.find( mit );
        if ( litit != lenIts.end() )
        {
          // len arguments just have an initalizer, from the array size
          initializers.push_back( mit->name + "( " + generateLenInitializer( mit, litit, structData.second.mutualExclusiveLens ) + " )" );
          sizeChecks += generateSizeCheck( litit->second, stripPrefix( structData.first, "Vk" ), structData.second.mutualExclusiveLens );
        }
        else if ( hasLen( *mit, structData.second.members ) )
        {
          assert( mit->type.isPointer() || !mit->arraySizes.empty() );
          std::string argumentName = ( mit->type.isPointer() ? startLowerCase( stripPrefix( mit->name, "p" ) ) : mit->name ) + "_";

          std::string argumentType = mit->type.compose( "VULKAN_HPP_NAMESPACE" );
          if ( mit->type.isPointer() )
          {
            argumentType = trimEnd( stripPostfix( argumentType, "*" ) );
          }
          if ( ( mit->type.type == "void" ) && ( argumentType.find( '*' ) == std::string::npos ) )
          {
            // the argument after stripping one pointer is just void
            assert( templateHeader.empty() );
            templateHeader = "    template <typename T>\n";

            const size_t pos = argumentType.find( "void" );
            assert( pos != std::string::npos );
            argumentType.replace( pos, strlen( "void" ), "T" );
          }

          if ( mit->lenExpressions[0] == "null-terminated" )
          {
            assert( ( mit->type.type == "char" ) && ( mit->arraySizes.size() == 1 ) );
            arguments.push_back( "std::string const & " + argumentName );
          }
          else if ( mit->arraySizes.empty() )
          {
            arguments.push_back( "VULKAN_HPP_NAMESPACE::ArrayProxyNoTemporaries<" + argumentType + "> const & " + argumentName );
          }
          else
          {
            assert( mit->arraySizes.size() == 1 );
            arguments.push_back( "VULKAN_HPP_NAMESPACE::ArrayProxy<" + argumentType + "> const & " + argumentName );
          }
          if ( arrayListed )
          {
            arguments.back() += " = {}";
          }
          arrayListed = true;

          if ( mit->type.isPointer() )
          {
            initializers.push_back( mit->name + "( " + argumentName + ".data() )" );
          }
          else
          {
            assert( mit->arraySizes.size() == 1 );
            if ( mit->lenExpressions[0] == "null-terminated" )
            {
              static const std::string strcpyTemplate = R"(
    VULKAN_HPP_ASSERT( ${memberName}_.size() < ${arraySize} );
#if defined( _WIN32 )
    strncpy_s( ${memberName}, ${memberSize}, ${memberName}_.data(), ${memberName}_.size() );
#else
    strncpy( ${memberName}, ${memberName}_.data(), std::min<size_t>( ${memberSize}, ${memberName}_.size() ) );
#endif
)";

              copyOps +=
                replaceWithMap( strcpyTemplate, { { "arraySize", mit->arraySizes[0] }, { "memberName", mit->name }, { "memberSize", mit->arraySizes[0] } } );
            }
            else
            {
              static const std::string memcpyTemplate = R"(
    VULKAN_HPP_ASSERT( ${memberName}_.size() < ${arraySize} );
    memcpy( ${memberName}, ${memberName}_.data(), ${lenExpression} * sizeof( ${argumentType} ) );)";

              std::string arraySizeExpression = ( mit->lenExpressions[0] == "null-terminated" )
                                                ? ( "std::min<size_t>( " + mit->name + "_.size(), " + mit->arraySizes[0] + " )" )
                                                : ( mit->lenExpressions[0] + " * sizeof( " + argumentType + " )" );
              copyOps += replaceWithMap( memcpyTemplate,
                                         { { "arraySize", mit->arraySizes[0] },
                                           { "argumentType", argumentType },
                                           { "lenExpression", mit->lenExpressions[0] },
                                           { "memberName", mit->name } } );
            }
          }
        }
        else
        {
          std::string argument = generateStructConstructorArgument( *mit, arrayListed );
          if ( !argument.empty() )
          {
            arguments.push_back( argument );
          }
          initializers.push_back( mit->name + "( " + mit->name + "_ )" );
        }
      }
    }

    auto pNextIt =
      std::find_if( structData.second.members.begin(), structData.second.members.end(), []( MemberData const & md ) { return md.name == "pNext"; } );
    if ( pNextIt != structData.second.members.end() )
    {
      // add pNext as a last optional argument to the constructor
      arguments.push_back( pNextIt->type.compose( "VULKAN_HPP_NAMESPACE" ) + " pNext_ = nullptr" );
    }

    static const std::string constructorTemplate = R"(
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
${templateHeader}    ${structName}( ${arguments} )
    ${initializers}
    {${sizeChecks}${copyOps}}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

    return replaceWithMap( constructorTemplate,
                           { { "arguments", generateList( arguments, "", ", " ) },
                             { "copyOps", copyOps },
                             { "initializers", generateList( initializers, ": ", ", " ) },
                             { "sizeChecks", sizeChecks },
                             { "structName", stripPrefix( structData.first, "Vk" ) },
                             { "templateHeader", templateHeader } } );
  }
  return "";
}

std::string VulkanHppGenerator::generateStructConstructorArgument( MemberData const & memberData, bool withDefault ) const
{
  // skip members 'pNext' and members with a specified value, as they are never explicitly set
  std::string str;
  if ( ( memberData.name != "pNext" ) && memberData.value.empty() )
  {
    if ( memberData.type.type.starts_with( "PFN_vk" ) )
    {
      str += "VULKAN_HPP_NAMESPACE::PFN_" + stripPrefix( memberData.type.type, "PFN_vk" ) + " ";
    }
    else if ( memberData.arraySizes.empty() )
    {
      str += memberData.type.compose( "VULKAN_HPP_NAMESPACE" ) + " ";
    }
    else
    {
      str += generateStandardArray( memberData.type.compose( "VULKAN_HPP_NAMESPACE" ), memberData.arraySizes ) + " const & ";
    }
    str += memberData.name + "_";

    if ( withDefault )
    {
      str += " = ";
      auto enumIt = m_enums.find( memberData.type.type );
      if ( enumIt != m_enums.end() && memberData.type.postfix.empty() )
      {
        str += generateEnumInitializer( memberData.type, memberData.arraySizes, enumIt->second.values, enumIt->second.isBitmask );
      }
      else if ( memberData.defaultValue.empty() )
      {
        // if there's no default value, it can be initialized with just {}
        str += "{}";
      }
      else
      {
        assert( memberData.defaultValue.starts_with( "VK_" ) );
        std::string tag = findTag( memberData.defaultValue );
        str += "VULKAN_HPP_NAMESPACE::" + toCamelCase( stripPostfix( stripPrefix( memberData.defaultValue, "VK_" ), tag ) ) + tag;
      }
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateStructHashStructure( std::pair<std::string, StructureData> const & structure,
                                                             std::set<std::string> &                       listedStructs ) const
{
  assert( !listedStructs.contains( structure.first ) );

  std::string str;
  for ( auto const & member : structure.second.members )
  {
    auto structIt = m_structs.find( member.type.type );
    if ( ( structIt != m_structs.end() ) && ( structure.first != member.type.type ) && !listedStructs.contains( member.type.type ) )
    {
      str += generateStructHashStructure( *structIt, listedStructs );
    }
  }

  if ( !containsUnion( structure.first ) )
  {
    static const std::string hashTemplate = R"(
  ${enter}template <> struct hash<VULKAN_HPP_NAMESPACE::${structureType}>
  {
    std::size_t operator()(VULKAN_HPP_NAMESPACE::${structureType} const & ${structureName}) const VULKAN_HPP_NOEXCEPT
    {
      std::size_t seed = 0;
${hashSum}
      return seed;
    }
  };
${leave})";

    auto [enter, leave] = generateProtection( getProtectFromType( structure.first ) );

    std::string structureType = stripPrefix( structure.first, "Vk" );
    std::string structureName = startLowerCase( structureType );
    str += replaceWithMap( hashTemplate,
                           { { "enter", enter },
                             { "hashSum", generateStructHashSum( structureName, structure.second.members ) },
                             { "leave", leave },
                             { "structureName", structureName },
                             { "structureType", structureType } } );
  }

  listedStructs.insert( structure.first );
  return str;
}

std::string VulkanHppGenerator::generateStructHashStructures() const
{
  const std::string hashesTemplate = R"(
#if 14 <= VULKAN_HPP_CPP_VERSION
  //======================================
  //=== HASH structures for structures ===
  //======================================

#  if !defined( VULKAN_HPP_HASH_COMBINE )
#    define VULKAN_HPP_HASH_COMBINE( seed, value ) \
      seed ^= std::hash<std::decay<decltype( value )>::type>{}( value ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 )
#  endif

${hashes}
#endif    // 14 <= VULKAN_HPP_CPP_VERSION
)";

  // Note reordering structs or handles by features and extensions is not possible!
  std::set<std::string> listedStructs;
  std::string           hashes;
  for ( auto const & structure : m_structs )
  {
    if ( !listedStructs.contains( structure.first ) && isTypeUsed( structure.first ) )
    {
      hashes += generateStructHashStructure( structure, listedStructs );
    }
  }
  return replaceWithMap( hashesTemplate, { { "hashes", hashes } } );
}

std::string VulkanHppGenerator::generateStructHashSum( std::string const & structName, std::vector<MemberData> const & members ) const
{
  std::string hashSum;
  for ( auto const & member : members )
  {
    if ( !member.arraySizes.empty() )
    {
      assert( member.arraySizes.size() < 3 );
      hashSum += "    for ( size_t i = 0; i < " + member.arraySizes[0] + "; ++i )\n";
      hashSum += "    {\n";
      if ( member.arraySizes.size() == 1 )
      {
        hashSum += "      VULKAN_HPP_HASH_COMBINE( seed, " + structName + "." + member.name + "[i] );\n";
      }
      else
      {
        hashSum += "      for ( size_t j=0; j < " + member.arraySizes[1] + "; ++j )\n";
        hashSum += "      {\n";
        hashSum += "        VULKAN_HPP_HASH_COMBINE( seed, " + structName + "." + member.name + "[i][j] );\n";
        hashSum += "      }\n";
      }
      hashSum += "    }\n";
    }
    else if ( member.type.type == "char" && !member.lenExpressions.empty() )
    {
      assert( member.lenExpressions.size() < 3 );
      if ( member.lenExpressions.size() == 1 )
      {
        assert( member.lenExpressions[0] == "null-terminated" );
        hashSum += "    for ( const char* p = " + structName + "." + member.name + "; *p != '\\0'; ++p )\n";
        hashSum += "    {\n";
        hashSum += "      VULKAN_HPP_HASH_COMBINE( seed, *p );\n";
        hashSum += "    }\n";
      }
      else
      {
        assert( member.lenExpressions[1] == "null-terminated" );
        hashSum += "    for ( size_t i = 0; i < " + structName + "." + member.lenExpressions[0] + "; ++i )\n";
        hashSum += "    {\n";
        hashSum += "        for ( const char* p = " + structName + "." + member.name + "[i]; *p != '\\0'; ++p )\n";
        hashSum += "        {\n";
        hashSum += "          VULKAN_HPP_HASH_COMBINE( seed, *p );\n";
        hashSum += "        }\n";
        hashSum += "    }\n";
      }
    }
    else
    {
      hashSum += "    VULKAN_HPP_HASH_COMBINE( seed, " + structName + "." + member.name + " );\n";
    }
  }
  assert( !hashSum.empty() );
  return hashSum.substr( 0, hashSum.size() - 1 );
}

std::string VulkanHppGenerator::generateStructs() const
{
  const std::string structsTemplate = R"(
  //===============
  //=== STRUCTS ===
  //===============

${structs}
)";

  // Note reordering structs or handles by features and extensions is not possible!
  std::set<std::string> listedStructs;
  std::string           structs;
  for ( auto const & structure : m_structs )
  {
    if ( !listedStructs.contains( structure.first ) && isTypeUsed( structure.first ) )
    {
      structs += generateStruct( structure, listedStructs );
    }
  }
  return replaceWithMap( structsTemplate, { { "structs", structs } } );
}

std::string VulkanHppGenerator::generateStructure( std::pair<std::string, StructureData> const & structure ) const
{
  const auto [enter, leave] = generateProtection( getProtectFromType( structure.first ) );

  std::string str = "\n" + enter;

  std::string constructorsAndSetters;
  if ( strcmp( &structure.first[0], "VkDeviceFaultInfoEXT" ) == 0 )
  {
    // special handling for this structure, as it is filled with dynamic memory on vk::Device::getFaultInfoEXT!
    constructorsAndSetters += R"(
#if !defined( VULKAN_HPP_NO_CONSTRUCTORS ) && !defined( VULKAN_HPP_NO_STRUCT_CONSTRUCTORS )
    VULKAN_HPP_CONSTEXPR_14 DeviceFaultInfoEXT( std::array<char, VK_MAX_DESCRIPTION_SIZE> const & description_       = {},
                                                VULKAN_HPP_NAMESPACE::DeviceFaultAddressInfoEXT * pAddressInfos_     = {},
                                                VULKAN_HPP_NAMESPACE::DeviceFaultVendorInfoEXT *  pVendorInfos_      = {},
                                                void *                                            pVendorBinaryData_ = {},
                                                void *                                            pNext_             = nullptr ) VULKAN_HPP_NOEXCEPT
      : pNext{ pNext_ }
      , description{ description_ }
      , pAddressInfos{ pAddressInfos_ }
      , pVendorInfos{ pVendorInfos_ }
      , pVendorBinaryData{ pVendorBinaryData_ }
    {
    }

#  ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
    VULKAN_HPP_CONSTEXPR_14 DeviceFaultInfoEXT( DeviceFaultInfoEXT const & rhs ) VULKAN_HPP_NOEXCEPT = default;

    DeviceFaultInfoEXT( VkDeviceFaultInfoEXT const & rhs ) VULKAN_HPP_NOEXCEPT : DeviceFaultInfoEXT( *reinterpret_cast<DeviceFaultInfoEXT const *>( &rhs ) ) {}

    DeviceFaultInfoEXT & operator=( DeviceFaultInfoEXT const & rhs ) VULKAN_HPP_NOEXCEPT = default;
#  else
    DeviceFaultInfoEXT( DeviceFaultInfoEXT const & )             = delete;
    DeviceFaultInfoEXT & operator=( DeviceFaultInfoEXT const & ) = delete;

    DeviceFaultInfoEXT( DeviceFaultInfoEXT && rhs ) VULKAN_HPP_NOEXCEPT
      : pNext{ rhs.pNext }
      , pAddressInfos{ rhs.pAddressInfos }
      , pVendorInfos{ rhs.pVendorInfos }
      , pVendorBinaryData{ rhs.pVendorBinaryData }
    {
      memcpy( description, rhs.description, VK_MAX_DESCRIPTION_SIZE );

      rhs.pNext = nullptr;
      memset( rhs.description, 0, VK_MAX_DESCRIPTION_SIZE );
      rhs.pAddressInfos     = nullptr;
      rhs.pVendorInfos      = nullptr;
      rhs.pVendorBinaryData = nullptr;
    }

    DeviceFaultInfoEXT & operator=( DeviceFaultInfoEXT && rhs ) VULKAN_HPP_NOEXCEPT
    {
      free( pAddressInfos );
      free( pVendorInfos );
      free( pVendorBinaryData );

      pNext = rhs.pNext;
      memcpy( description, rhs.description, VK_MAX_DESCRIPTION_SIZE );
      pAddressInfos     = rhs.pAddressInfos;
      pVendorInfos      = rhs.pVendorInfos;
      pVendorBinaryData = rhs.pVendorBinaryData;

      rhs.pNext = nullptr;
      memset( rhs.description, 0, VK_MAX_DESCRIPTION_SIZE );
      rhs.pAddressInfos     = nullptr;
      rhs.pVendorInfos      = nullptr;
      rhs.pVendorBinaryData = nullptr;

      return *this;
    }

    ~DeviceFaultInfoEXT() VULKAN_HPP_NOEXCEPT
    {
      free( pAddressInfos );
      free( pVendorInfos );
      free( pVendorBinaryData );
    }
#  endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif   /*VULKAN_HPP_NO_CONSTRUCTORS*/
)";
  }
  else
  {
    static const std::string constructorsTemplate = R"(
#if !defined( VULKAN_HPP_NO_CONSTRUCTORS ) && !defined( VULKAN_HPP_NO_STRUCT_CONSTRUCTORS )
${constructors}
${subConstructors}
${deprecatedConstructors}
    ${structName} & operator=( ${structName} const & rhs ) VULKAN_HPP_NOEXCEPT = default;
#endif /*VULKAN_HPP_NO_CONSTRUCTORS*/

    ${structName} & operator=( Vk${structName} const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      *this = *reinterpret_cast<VULKAN_HPP_NAMESPACE::${structName} const *>( &rhs );
      return *this;
    }
)";

    constructorsAndSetters = replaceWithMap( constructorsTemplate,
                                             { { "constructors", generateStructConstructors( structure ) },
                                               { "deprecatedConstructors", generateDeprecatedConstructors( structure.first ) },
                                               { "structName", stripPrefix( structure.first, "Vk" ) },
                                               { "subConstructors", generateStructSubConstructor( structure ) } } );
  }

  if ( !structure.second.returnedOnly )
  {
    // only structs that are not returnedOnly get setters!
    constructorsAndSetters += "\n#if !defined( VULKAN_HPP_NO_SETTERS ) && !defined( VULKAN_HPP_NO_STRUCT_SETTERS )";
    for ( size_t i = 0; i < structure.second.members.size(); i++ )
    {
      constructorsAndSetters += generateStructSetter( stripPrefix( structure.first, "Vk" ), structure.second.members, i );
    }
    constructorsAndSetters += generateDeprecatedStructSetters( structure.first );
    constructorsAndSetters += "#endif /*VULKAN_HPP_NO_SETTERS*/\n";
  }

  std::string structureType = stripPrefix( structure.first, "Vk" );
  // the member variables
  std::string members, memberNames, memberTypes, sTypeValue;
  std::tie( members, memberNames, memberTypes, sTypeValue ) = generateStructMembers( structure );

  // reflect is meaningfull for structs only, filter out unions
  std::string reflect;
  if ( !structure.second.isUnion )
  {
    // use reflection only if VULKAN_HPP_USE_REFLECT is defined
    static const std::string reflectTemplate = R"(
#if defined( VULKAN_HPP_USE_REFLECT )
#if 14 <= VULKAN_HPP_CPP_VERSION
    auto
#else
    std::tuple<${memberTypes}>
#endif
      reflect() const VULKAN_HPP_NOEXCEPT
    {
      return std::tie( ${memberNames} );
    }
#endif
)";

    reflect = replaceWithMap( reflectTemplate, { { "memberNames", memberNames }, { "memberTypes", memberTypes } } );
  }

  // operator==() and operator!=()
  // only structs without a union as a member can have a meaningfull == and != operation; we filter them out
  std::string compareOperators;
  if ( !containsUnion( structure.first ) )
  {
    compareOperators += generateStructCompareOperators( structure );
  }

  static const std::string structureTemplate = R"(  struct ${structureType}
  {
    using NativeType = Vk${structureType};

${allowDuplicate}
${typeValue}
${constructorsAndSetters}

    operator Vk${structureType} const &() const VULKAN_HPP_NOEXCEPT
    {
      return *reinterpret_cast<const Vk${structureType}*>( this );
    }

    operator Vk${structureType} &() VULKAN_HPP_NOEXCEPT
    {
      return *reinterpret_cast<Vk${structureType}*>( this );
    }
${reflect}
${compareOperators}
    public:
${members}
  };
)";

  std::string allowDuplicate, typeValue;
  if ( !sTypeValue.empty() )
  {
    allowDuplicate = std::string( "    static const bool allowDuplicate = " ) + ( structure.second.allowDuplicate ? "true;" : "false;" );
    typeValue      = "    static VULKAN_HPP_CONST_OR_CONSTEXPR StructureType structureType = StructureType::" + sTypeValue + ";\n";
  }
  str += replaceWithMap( structureTemplate,
                         { { "allowDuplicate", allowDuplicate },
                           { "constructorsAndSetters", constructorsAndSetters },
                           { "compareOperators", compareOperators },
                           { "members", members },
                           { "reflect", reflect },
                           { "structureType", structureType },
                           { "typeValue", typeValue } } );

  if ( !sTypeValue.empty() )
  {
    std::string cppTypeTemplate = R"(
  template <>
  struct CppType<StructureType, StructureType::${sTypeValue}>
  {
    using Type = ${structureType};
  };
)";
    str += replaceWithMap( cppTypeTemplate, { { "sTypeValue", sTypeValue }, { "structureType", structureType } } );
  }

  for ( auto const & alias : structure.second.aliases )
  {
    str += "  using " + stripPrefix( alias.first, "Vk" ) + " = " + structureType + ";\n";
  }

  str += leave;
  return str;
}

std::string VulkanHppGenerator::generateStructExtendsStructs() const
{
  const std::string structExtendsTemplate = R"(
  //=======================
  //=== STRUCTS EXTENDS ===
  //=======================

${structExtends})";

  std::string           structExtends;
  std::set<std::string> listedStructs;
  for ( auto const & feature : m_features )
  {
    structExtends += generateStructExtendsStructs( feature.requireData, listedStructs, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    structExtends += generateStructExtendsStructs( extension.requireData, listedStructs, extension.name );
  }

  return replaceWithMap( structExtendsTemplate, { { "structExtends", structExtends } } );
}

std::string VulkanHppGenerator::generateStructExtendsStructs( std::vector<RequireData> const & requireData,
                                                              std::set<std::string> &          listedStructs,
                                                              std::string const &              title ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto structIt = m_structs.find( type.name );
      if ( ( structIt != m_structs.end() ) && listedStructs.insert( type.name ).second )
      {
        const auto [enter, leave] = generateProtection( getProtectFromTitle( title ) );

        // append all allowed structure chains
        for ( auto extendName : structIt->second.structExtends )
        {
          auto extendsIt                  = findByNameOrAlias( m_structs, extendName );
          const auto [subEnter, subLeave] = generateProtection( getProtectFromType( extendsIt->first ) );

          if ( enter != subEnter )
          {
            str += subEnter;
          }

          str += "  template <> struct StructExtends<" + stripPrefix( structIt->first, "Vk" ) + ", " + stripPrefix( extendName, "Vk" ) +
                 ">{ enum { value = true }; };\n";

          if ( leave != subLeave )
          {
            str += subLeave;
          }
        }
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateStructForwardDeclarations() const
{
  const std::string fowardDeclarationsTemplate = R"(
  //===================================
  //=== STRUCT forward declarations ===
  //===================================

${forwardDeclarations}
)";

  std::string           forwardDeclarations;
  std::set<std::string> listedStructs;
  for ( auto const & feature : m_features )
  {
    forwardDeclarations += generateStructForwardDeclarations( feature.requireData, feature.name, listedStructs );
  }
  for ( auto const & extension : m_extensions )
  {
    forwardDeclarations += generateStructForwardDeclarations( extension.requireData, extension.name, listedStructs );
  }

  return replaceWithMap( fowardDeclarationsTemplate, { { "forwardDeclarations", forwardDeclarations } } );
}

std::string VulkanHppGenerator::generateStructForwardDeclarations( std::vector<RequireData> const & requireData,
                                                                   std::string const &              title,
                                                                   std::set<std::string> &          listedStructs ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto structIt = m_structs.find( type.name );
      if ( ( structIt != m_structs.end() ) && listedStructs.insert( type.name ).second )
      {
        std::string structureType = stripPrefix( structIt->first, "Vk" );
        str += ( structIt->second.isUnion ? "  union " : "  struct " ) + structureType + ";\n";

        for ( auto const & alias : structIt->second.aliases )
        {
          str += "  using " + stripPrefix( alias.first, "Vk" ) + " = " + structureType + ";\n";
        }
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::tuple<std::string, std::string, std::string, std::string>
  VulkanHppGenerator::generateStructMembers( std::pair<std::string, StructureData> const & structData ) const
{
  std::string members, memberNames, memberTypes, sTypeValue;
  for ( auto const & member : structData.second.members )
  {
    members += "    ";
    std::string type;
    if ( !member.bitCount.empty() && member.type.type.starts_with( "Vk" ) )
    {
      assert( member.type.prefix.empty() && member.type.postfix.empty() );  // never encounterd a different case
      type = member.type.type;
    }
    else if ( member.type.type.starts_with( "PFN_vk" ) )
    {
      assert( member.type.prefix.empty() && member.type.postfix.empty() );  // never encounterd a different case
      type = "VULKAN_HPP_NAMESPACE::PFN_" + stripPrefix( member.type.type, "PFN_vk" );
    }
    else if ( member.arraySizes.empty() )
    {
      type = member.type.compose( "VULKAN_HPP_NAMESPACE" );
    }
    else
    {
      assert( member.type.prefix.empty() && member.type.postfix.empty() );
      type = generateStandardArrayWrapper( member.type.compose( "VULKAN_HPP_NAMESPACE" ), member.arraySizes );
    }
    members += type + " " + member.name;
    if ( !member.value.empty() )
    {
      // special handling for members with legal value: use it as the default
      members += " = ";
      if ( member.type.type == "uint32_t" )
      {
        members += member.value;
      }
      else
      {
        auto enumIt = m_enums.find( member.type.type );
        assert( enumIt != m_enums.end() );
        assert( contains( enumIt->second.values, member.value ) );
        std::string valueName = generateEnumValueName( enumIt->first, member.value, enumIt->second.isBitmask );
        members += stripPrefix( member.type.type, "Vk" ) + "::" + valueName;
        if ( member.name == "sType" )
        {
          sTypeValue = valueName;
        }
      }
    }
    else
    {
      // when we don't have any default initialization value, everything can be initialized by just '{}'
      assert( member.arraySizes.empty() || member.bitCount.empty() );
      if ( !member.bitCount.empty() )
      {
        members += " : " + member.bitCount;  // except for bitfield members, where no default member initializatin
                                             // is supported (up to C++20)
      }
      else
      {
        members += " = ";
        auto enumIt = m_enums.find( member.type.type );
        if ( member.arraySizes.empty() && ( enumIt != m_enums.end() ) && member.type.postfix.empty() )
        {
          members += generateEnumInitializer( member.type, member.arraySizes, enumIt->second.values, enumIt->second.isBitmask );
        }
        else if ( member.defaultValue.empty() )
        {
          members += "{}";
        }
        else
        {
          assert( member.defaultValue.starts_with( "VK_" ) );
          std::string tag = findTag( member.defaultValue );
          members += "VULKAN_HPP_NAMESPACE::" + toCamelCase( stripPostfix( stripPrefix( member.defaultValue, "VK_" ), tag ) ) + tag;
        }
      }
    }
    members += ";\n";

    memberNames += member.name + ", ";
    memberTypes += type + " const &, ";
  }
  return std::make_tuple( members, stripPostfix( memberNames, ", " ), stripPostfix( memberTypes, ", " ), sTypeValue );
}

std::string VulkanHppGenerator::generateStructSetter( std::string const & structureName, std::vector<MemberData> const & memberData, size_t index ) const
{
  std::string        str;
  MemberData const & member = memberData[index];
  if ( member.type.type != "VkStructureType" )  // filter out StructureType, which is supposed to be immutable !
  {
    static const std::string templateString = R"(
    ${constexpr}${structureName} & set${MemberName}( ${memberType} ${reference}${memberName}_ ) VULKAN_HPP_NOEXCEPT
    {
      ${assignment};
      return *this;
    }
)";

    std::string memberType         = member.type.type.starts_with( "PFN_vk" )
                                     ? "VULKAN_HPP_NAMESPACE::PFN_" + stripPrefix( member.type.type, "PFN_vk" )
                                     : ( member.arraySizes.empty() ? member.type.compose( "VULKAN_HPP_NAMESPACE" )
                                                                   : generateStandardArray( member.type.compose( "VULKAN_HPP_NAMESPACE" ), member.arraySizes ) );
    const bool  isReinterpretation = !member.bitCount.empty() && member.type.type.starts_with( "Vk" );
    std::string assignment;
    if ( isReinterpretation )
    {
      assignment = member.name + " = " + "*reinterpret_cast<" + member.type.type + "*>(&" + member.name + "_)";
    }
    else
    {
      assignment = member.name + " = " + member.name + "_";
    }

    str += replaceWithMap( templateString,
                           { { "assignment", assignment },
                             { "constexpr", isReinterpretation ? "" : "VULKAN_HPP_CONSTEXPR_14 " },
                             { "memberName", member.name },
                             { "MemberName", startUpperCase( member.name ) },
                             { "memberType", memberType },
                             { "reference", ( member.type.postfix.empty() && m_structs.contains( member.type.type ) ) ? "const & " : "" },
                             { "structureName", structureName } } );

    if ( hasLen( member, memberData ) )
    {
      assert( member.type.isPointer() || !member.arraySizes.empty() );
      assert( !member.type.isPointer() || member.name.starts_with( "p" ) );
      std::string arrayName = member.type.isPointer() ? startLowerCase( stripPrefix( member.name, "p" ) ) : member.name;

      if ( member.lenExpressions[0] == "null-terminated" )
      {
        assert( member.lenMembers.empty() && ( member.lenExpressions.size() == 1 ) && ( member.arraySizes.size() == 1 ) && ( member.type.type == "char" ) );

        static const std::string setStringTemplate = R"(
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
    ${structureName} & set${ArrayName}( std::string const & ${arrayName}_ ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( ${arrayName}_.size() < ${arraySize} );
#if defined( _WIN32 )
      strncpy_s( ${arrayName}, ${arraySize}, ${arrayName}_.data(), ${arrayName}_.size() );
#else
      strncpy( ${arrayName}, ${arrayName}_.data(), std::min<size_t>( ${arraySize}, ${arrayName}_.size() ) );
#endif
      return *this;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

        str += replaceWithMap( setStringTemplate,
                               { { "arrayName", arrayName },
                                 { "ArrayName", startUpperCase( arrayName ) },
                                 { "arraySize", member.arraySizes[0] },
                                 { "structureName", structureName } } );
      }
      else if ( ( structureName == "LayerSettingEXT" ) && ( index == 4 ) )
      {
        // VkLayerSettingEXT::pValues needs some special handling!
        assert( member.name == "pValues" );
        static const std::string byTypeTemplate =
          R"(    LayerSettingEXT & setValues( VULKAN_HPP_NAMESPACE::ArrayProxyNoTemporaries<const ${type}> const & values_ ) VULKAN_HPP_NOEXCEPT
    {
      valueCount = static_cast<uint32_t>( values_.size() );
      pValues    = values_.data();
      return *this;
    })";

        static const std::string setArrayTemplate = R"(
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
${byInt32}
${byInt64}
${byUint32}
${byUint64}
${byFloat32}
${byFloat64}
${byString}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

        return replaceWithMap( setArrayTemplate,
                               {
                                 { "byInt32", replaceWithMap( byTypeTemplate, { { "type", "int32_t" } } ) },
                                 { "byInt64", replaceWithMap( byTypeTemplate, { { "type", "int64_t" } } ) },
                                 { "byUint32", replaceWithMap( byTypeTemplate, { { "type", "uint32_t" } } ) },
                                 { "byUint64", replaceWithMap( byTypeTemplate, { { "type", "uint64_t" } } ) },
                                 { "byFloat32", replaceWithMap( byTypeTemplate, { { "type", "float" } } ) },
                                 { "byFloat64", replaceWithMap( byTypeTemplate, { { "type", "double" } } ) },
                                 { "byString", replaceWithMap( byTypeTemplate, { { "type", "char *" } } ) },
                               } );
      }
      else
      {
        assert( ( member.lenExpressions[0] == member.lenMembers[0].first ) || ( member.lenExpressions[0] == "codeSize / 4" ) );
        std::string lenName, lenValue;
        if ( member.lenExpressions[0] == "codeSize / 4" )
        {
          lenName  = "codeSize";
          lenValue = arrayName + "_.size() * 4";
        }
        else
        {
          lenName  = member.lenExpressions[0];
          lenValue = arrayName + "_.size()";
        }

        if ( member.type.isPointer() )
        {
          assert( memberType.back() == '*' );
          memberType = trimEnd( stripPostfix( memberType, "*" ) );
        }

        std::string templateHeader;
        if ( ( member.type.type == "void" ) && ( memberType.find( '*' ) == std::string::npos ) )
        {
          assert( templateHeader.empty() );
          templateHeader = "template <typename T>\n    ";

          const size_t pos = memberType.find( "void" );
          assert( pos != std::string::npos );
          memberType.replace( pos, strlen( "void" ), "T" );

          lenValue += " * sizeof(T)";
        }

        auto lenMember = findStructMemberIt( lenName, memberData );
        assert( lenMember != memberData.end() && lenMember->type.prefix.empty() && lenMember->type.postfix.empty() );
        if ( lenMember->type.type != "size_t" )
        {
          lenValue = "static_cast<" + lenMember->type.type + ">( " + lenValue + " )";
        }

        if ( !member.arraySizes.empty() )
        {
          assert( member.arraySizes.size() == 1 );
          lenValue = "(std::min)( " + lenValue + ", " + member.arraySizes[0] + " )";
        }

        if ( member.type.isPointer() )
        {
          static const std::string setArrayTemplate = R"(
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
    ${templateHeader}${structureName} & set${ArrayName}( VULKAN_HPP_NAMESPACE::ArrayProxyNoTemporaries<${memberType}> const & ${arrayName}_ ) VULKAN_HPP_NOEXCEPT
    {
      ${lenName} = ${lenValue};
      ${memberName} = ${arrayName}_.data();
      return *this;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

          str += replaceWithMap( setArrayTemplate,
                                 { { "arrayName", arrayName },
                                   { "ArrayName", startUpperCase( arrayName ) },
                                   { "lenName", lenName },
                                   { "lenValue", lenValue },
                                   { "memberName", member.name },
                                   { "memberType", memberType },
                                   { "structureName", structureName },
                                   { "templateHeader", templateHeader } } );
        }
        else
        {
          assert( member.arraySizes.size() == 1 );

          static const std::string setArrayTemplate = R"(
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
    ${templateHeader}${structureName} & set${ArrayName}( VULKAN_HPP_NAMESPACE::ArrayProxy<${memberType}> const & ${arrayName}_ ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( ${arrayName}_.size() <= ${arraySize} );
      ${lenName} = ${lenValue};
      memcpy( ${arrayName}, ${arrayName}_.data(), ${lenName} );
      return *this;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

          str += replaceWithMap( setArrayTemplate,
                                 { { "arrayName", arrayName },
                                   { "ArrayName", startUpperCase( arrayName ) },
                                   { "arraySize", member.arraySizes[0] },
                                   { "lenName", lenName },
                                   { "lenValue", lenValue },
                                   { "memberType", member.type.compose( "VULKAN_HPP_NAMESPACE" ) },
                                   { "structureName", structureName },
                                   { "templateHeader", templateHeader } } );
        }
      }
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateStructSubConstructor( std::pair<std::string, StructureData> const & structData ) const
{
  if ( !structData.second.subStruct.empty() )
  {
    auto const & subStruct = m_structs.find( structData.second.subStruct );
    assert( subStruct != m_structs.end() );

    std::string subStructArgumentName = startLowerCase( stripPrefix( subStruct->first, "Vk" ) );

    std::string subCopies;
    bool        firstArgument = true;
    for ( size_t i = 0; i < subStruct->second.members.size(); i++ )
    {
      assert( structData.second.members[i].arraySizes.empty() );
      static const std::string subCopiesTemplate =
        R"(      ${separator} ${structMemberName}( ${subStructArgumentName}.${subStructMemberName} )
)";
      subCopies += replaceWithMap( subCopiesTemplate,
                                   { { "separator", firstArgument ? ":" : "," },
                                     { "structMemberName", structData.second.members[i].name },
                                     { "subStructMemberName", subStruct->second.members[i].name },
                                     { "subStructArgumentName", subStructArgumentName } } );
      firstArgument = false;
    }

    std::vector<std::string> subArguments;
    for ( size_t i = subStruct->second.members.size(); i < structData.second.members.size(); i++ )
    {
      std::string argument = generateStructConstructorArgument( structData.second.members[i], true );
      if ( !argument.empty() )
      {
        subArguments.push_back( argument );
      }

      assert( structData.second.members[i].arraySizes.empty() );
      static const std::string subCopiesTemplate = R"(    , ${memberName}( ${memberName}_ )
)";
      subCopies += replaceWithMap( subCopiesTemplate, { { "memberName", structData.second.members[i].name } } );
    }

    static const std::string subStructConstructorTemplate = R"(
    explicit ${structName}( ${subStructName} const & ${subStructArgumentName}${subArguments} )
${subCopies}    {}
)";
    return replaceWithMap( subStructConstructorTemplate,
                           { { "structName", stripPrefix( structData.first, "Vk" ) },
                             { "subArguments", generateList( subArguments, ", ", ", " ) },
                             { "subCopies", subCopies },
                             { "subStructArgumentName", subStructArgumentName },
                             { "subStructName", stripPrefix( subStruct->first, "Vk" ) } } );
  }
  return "";
}

std::string VulkanHppGenerator::generateSuccessCheck( std::vector<std::string> const & successCodes ) const
{
  assert( !successCodes.empty() );
  std::string successCheck = "result == " + generateSuccessCode( successCodes[0] );
  if ( 1 < successCodes.size() )
  {
    successCheck = "( " + successCheck + " )";
    for ( size_t i = 1; i < successCodes.size(); ++i )
    {
      successCheck += "|| ( result == " + generateSuccessCode( successCodes[i] ) + " )";
    }
  }
  return successCheck;
}

std::string VulkanHppGenerator::generateSuccessCode( std::string const & code ) const
{
  std::string tag = findTag( code );
  // on each success code: prepend 'VULKAN_HPP_NAMESPACE::Result::e', strip "VK_" and a tag, convert it to camel
  // case, and add the tag again
  return "VULKAN_HPP_NAMESPACE::Result::e" + toCamelCase( stripPostfix( stripPrefix( code, "VK_" ), tag ) ) + tag;
}

std::string VulkanHppGenerator::generateSuccessCodeList( std::vector<std::string> const & successCodes, bool enumerating ) const
{
  std::string successCodeList;
  size_t      skipSize = ( enumerating && ( 1 < successCodes.size() ) && ( successCodes[1] == "VK_INCOMPLETE" ) ) ? 2 : 1;
  if ( skipSize < successCodes.size() )
  {
    successCodeList = ", { " + generateSuccessCode( successCodes[0] );
    for ( size_t i = 1; i < successCodes.size(); ++i )
    {
      successCodeList += ", " + generateSuccessCode( successCodes[i] );
    }
    successCodeList += " }";
  }
  return successCodeList;
}

std::string VulkanHppGenerator::generateThrowResultException() const
{
  auto enumIt = m_enums.find( "VkResult" );

  std::string cases;
  for ( auto const & value : enumIt->second.values )
  {
    if ( value.supported && value.name.starts_with( "VK_ERROR" ) )
    {
      const auto [enter, leave] = generateProtection( value.protect );
      std::string valueName     = generateEnumValueName( enumIt->first, value.name, false );
      cases += enter + "      case Result::" + valueName + ": throw " + stripPrefix( valueName, "eError" ) + "Error( message );\n" + leave;
    }
  }
  cases.pop_back();  // remove last newline

  const std::string throwTemplate = R"(
  namespace detail
  {
    [[noreturn]] VULKAN_HPP_INLINE void throwResultException( Result result, char const * message )
    {
      switch ( result )
      {
${cases}
        default: throw SystemError( make_error_code( result ), message );
      }
    }
  })";
  return replaceWithMap( throwTemplate, { { "cases", cases } } );
}

std::string VulkanHppGenerator::generateTypenameCheck( std::vector<size_t> const &               returnParams,
                                                       std::map<size_t, VectorParamData> const & vectorParams,
                                                       std::vector<size_t> const &               chainedReturnParams,
                                                       bool                                      definition,
                                                       std::vector<std::string> const &          dataTypes,
                                                       CommandFlavourFlags                       flavourFlags ) const
{
  std::string typenameCheck, enableIf;
  if ( !( flavourFlags & CommandFlavourFlagBits::singular ) )
  {
    for ( size_t i = 0; i < returnParams.size(); ++i )
    {
      if ( vectorParams.contains( returnParams[i] ) )
      {
        std::string elementType =
          ( ( flavourFlags & CommandFlavourFlagBits::chained ) &&
            std::any_of( chainedReturnParams.begin(), chainedReturnParams.end(), [&returnParams, i]( size_t crp ) { return crp == returnParams[i]; } ) )
            ? "StructureChain"
            : dataTypes[i];
        std::string extendedElementType = elementType;
        if ( flavourFlags & CommandFlavourFlagBits::unique )
        {
          extendedElementType = "UniqueHandle<" + elementType + ", Dispatch>";
        }
        elementType = startUpperCase( stripPrefix( elementType, "VULKAN_HPP_NAMESPACE::" ) );
        if ( !enableIf.empty() )
        {
          enableIf += " && ";
        }
        enableIf += "std::is_same<typename " + elementType + "Allocator::value_type, " + extendedElementType + " > ::value ";
      }
    }
    if ( !enableIf.empty() )
    {
      typenameCheck = ", typename std::enable_if<" + enableIf + ", int>::type" + ( definition ? "" : " = 0" );
    }
  }
  return typenameCheck;
}

std::string VulkanHppGenerator::generateUnion( std::pair<std::string, StructureData> const & structure ) const
{
  auto [enter, leave]   = generateProtection( getProtectFromType( structure.first ) );
  std::string unionName = stripPrefix( structure.first, "Vk" );

  bool               firstMember = true;
  std::set<TypeInfo> listedTypes;  // create just one constructor per different type !
  std::string        constructors;
  for ( auto memberIt = structure.second.members.begin(); memberIt != structure.second.members.end(); ++memberIt )
  {
    if ( listedTypes.insert( memberIt->type ).second )
    {
      // VkBool32 is aliased to uint32_t. Don't create a VkBool32 constructor if the union also contains a
      // uint32_t constructor.
      if ( memberIt->type.type == "VkBool32" )
      {
        if ( findStructMemberItByType( "uint32_t", structure.second.members ) != structure.second.members.end() )
        {
          continue;
        }
      }

      const bool multipleType = std::any_of(
        std::next( memberIt ), structure.second.members.end(), [memberIt]( MemberData const & member ) noexcept { return member.type == memberIt->type; } );
      std::string memberType = ( memberIt->arraySizes.empty() )
                               ? memberIt->type.compose( "VULKAN_HPP_NAMESPACE" )
                               : ( "const " + generateStandardArray( memberIt->type.compose( "VULKAN_HPP_NAMESPACE" ), memberIt->arraySizes ) + "&" );

      // In a majority of cases this can be constexpr in C++11 as well, however, determining when exactly
      // that is the case is a lot more involved and probably not worth it.
      static const std::string constructorTemplate = R"(
    VULKAN_HPP_CONSTEXPR_14 ${unionName}( ${memberType} ${argumentName}_${defaultAssignment} )
      : ${memberName}( ${argumentName}_ )
    {})";

      constructors += ( firstMember ? "" : "\n" ) + replaceWithMap( constructorTemplate,
                                                                    { { "argumentName", multipleType ? generateName( memberIt->type ) : memberIt->name },
                                                                      { "defaultAssignment", firstMember ? " = {}" : "" },
                                                                      { "memberName", memberIt->name },
                                                                      { "memberType", memberType },
                                                                      { "unionName", stripPrefix( structure.first, "Vk" ) } } );
      firstMember = false;

      if ( !memberIt->arraySizes.empty() )
      {
        assert( !multipleType );
        assert( memberIt->arraySizes.size() == 1 );
        const int size = std::stoi( memberIt->arraySizes[0] );
        assert( std::to_string( size ) == memberIt->arraySizes[0] );
        std::vector<std::string> arguments, callArguments;
        for ( int i = 0; i < size; i++ )
        {
          std::string argumentIndex = std::to_string( i );
          arguments.push_back( memberIt->type.type + " " + memberIt->name + "_" + argumentIndex );
          callArguments.push_back( memberIt->name + "_" + argumentIndex );
        }

        static const std::string constructorBySequenceTemplate = R"(
    VULKAN_HPP_CONSTEXPR ${unionName}( ${arguments} )
      : ${memberName}{ { { ${callArguments} } } }
    {})";

        constructors += "\n" + replaceWithMap( constructorBySequenceTemplate,
                                               { { "arguments", generateList( arguments, "", ", " ) },
                                                 { "callArguments", generateList( callArguments, "", ", " ) },
                                                 { "memberName", memberIt->name },
                                                 { "unionName", stripPrefix( structure.first, "Vk" ) } } );
      }
    }
  }

  // one setter per union element
  std::string setters;
  for ( size_t i = 0; i < structure.second.members.size(); i++ )
  {
    setters += generateStructSetter( stripPrefix( structure.first, "Vk" ), structure.second.members, i );
  }
  // filter out leading and trailing newline
  setters = setters.substr( 1, setters.length() - 2 );

  // the union member variables
  std::string members;
  // if there's at least one Vk... type in this union, check for unrestricted unions support
  const bool needsUnrestrictedUnions = std::any_of(
    structure.second.members.begin(), structure.second.members.end(), []( MemberData const & member ) { return member.type.type.starts_with( "Vk" ); } );
  if ( needsUnrestrictedUnions )
  {
    members += "#ifdef VULKAN_HPP_HAS_UNRESTRICTED_UNIONS\n";
  }
  for ( auto const & member : structure.second.members )
  {
    static const std::string memberTemplate = R"(    ${memberType} ${memberName};
)";
    members +=
      replaceWithMap( memberTemplate,
                      { { "memberName", member.name },
                        { "memberType",
                          member.arraySizes.empty() ? member.type.compose( "VULKAN_HPP_NAMESPACE" )
                                                    : generateStandardArrayWrapper( member.type.compose( "VULKAN_HPP_NAMESPACE" ), member.arraySizes ) } } );
  }
  if ( needsUnrestrictedUnions )
  {
    members += "#else\n";
    for ( auto const & member : structure.second.members )
    {
      static const std::string memberTemplate = R"(    ${memberType} ${memberName}${arraySizes};
)";
      members += replaceWithMap(
        memberTemplate,
        { { "arraySizes", generateCArraySizes( member.arraySizes ) }, { "memberName", member.name }, { "memberType", member.type.compose( "" ) } } );
    }
    members += "#endif  /*VULKAN_HPP_HAS_UNRESTRICTED_UNIONS*/\n";
  }

  static const std::string unionTemplate = R"(
${enter}  union ${unionName}
  {
    using NativeType = Vk${unionName};
#if !defined( VULKAN_HPP_NO_CONSTRUCTORS ) && !defined( VULKAN_HPP_NO_UNION_CONSTRUCTORS )
${constructors}
#endif /*VULKAN_HPP_NO_CONSTRUCTORS*/

#if !defined( VULKAN_HPP_NO_SETTERS ) && !defined( VULKAN_HPP_NO_UNION_SETTERS )
${setters}
#endif /*VULKAN_HPP_NO_SETTERS*/

    operator Vk${unionName} const &() const
    {
      return *reinterpret_cast<const Vk${unionName}*>( this );
    }

    operator Vk${unionName} &()
    {
      return *reinterpret_cast<Vk${unionName}*>( this );
    }

${members}
  };
${leave})";

  return replaceWithMap(
    unionTemplate,
    { { "constructors", constructors }, { "enter", enter }, { "leave", leave }, { "members", members }, { "setters", setters }, { "unionName", unionName } } );
}

std::string VulkanHppGenerator::generateUniqueHandle( std::pair<std::string, HandleData> const & handleData ) const
{
  if ( !handleData.second.deleteCommand.empty() )
  {
    std::string type = stripPrefix( handleData.first, "Vk" );
    std::string aliasHandle;
    for ( auto const & alias : handleData.second.aliases )
    {
      static const std::string aliasHandleTemplate = R"(  using Unique${aliasType} = UniqueHandle<${type}, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;)";

      aliasHandle += replaceWithMap( aliasHandleTemplate, { { "aliasType", stripPrefix( alias.first, "Vk" ) }, { "type", type } } );
    }

    static const std::string uniqueHandleTemplate = R"(  template <typename Dispatch>
  class UniqueHandleTraits<${type}, Dispatch>
  {
  public:
    using deleter = detail::${deleterType}${deleterAction}<${deleterParent}${deleterPool}, Dispatch>;
  };
  using Unique${type} = UniqueHandle<${type}, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;
${aliasHandle})";

    return replaceWithMap(
      uniqueHandleTemplate,
      { { "aliasHandle", aliasHandle },
        { "deleterAction", ( handleData.second.deleteCommand.substr( 2, 4 ) == "Free" ) ? "Free" : "Destroy" },
        { "deleterParent", handleData.second.destructorType.empty() ? "detail::NoParent" : stripPrefix( handleData.second.destructorType, "Vk" ) },
        { "deleterPool", handleData.second.deletePool.empty() ? "" : ", " + stripPrefix( handleData.second.deletePool, "Vk" ) },
        { "deleterType", handleData.second.deletePool.empty() ? "Object" : "Pool" },
        { "type", type } } );
  }
  return "";
}

std::string VulkanHppGenerator::generateUniqueHandle( std::vector<RequireData> const & requireData, std::string const & title ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto handleIt = m_handles.find( type.name );
      if ( handleIt != m_handles.end() )
      {
        str += generateUniqueHandle( *handleIt );
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateUniqueHandles() const
{
  std::string uniqueHandlesTemplate = R"(
#ifndef VULKAN_HPP_NO_SMART_HANDLE
  //======================
  //=== UNIQUE HANDLEs ===
  //======================

${uniqueHandles}
#endif  /*VULKAN_HPP_NO_SMART_HANDLE*/
)";

  std::string uniqueHandles;
  for ( auto const & feature : m_features )
  {
    uniqueHandles += generateUniqueHandle( feature.requireData, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    uniqueHandles += generateUniqueHandle( extension.requireData, extension.name );
  }
  assert( uniqueHandles.back() == '\n' );
  uniqueHandles.pop_back();
  return replaceWithMap( uniqueHandlesTemplate, { { "uniqueHandles", uniqueHandles } } );
}

std::string VulkanHppGenerator::generateSharedHandle( std::pair<std::string, HandleData> const & handleData ) const
{
  if ( !handleData.second.deleteCommand.empty() )
  {
    std::string type = stripPrefix( handleData.first, "Vk" );
    std::string aliasHandle;
    for ( auto const & alias : handleData.second.aliases )
    {
      static const std::string aliasHandleTemplate = R"(  using Shared${aliasType} = SharedHandle<${type}>;)";

      aliasHandle += replaceWithMap( aliasHandleTemplate, { { "aliasType", stripPrefix( alias.first, "Vk" ) }, { "type", type } } );
    }

    static const std::string sharedHandleTemplate = R"(  template <>
  class SharedHandleTraits<${type}>
  {
  public:
    using DestructorType = ${destructor};
    using deleter = detail::${deleterType}${deleterAction}Shared<${type}${deleterPool}>;
  };
  using Shared${type} = SharedHandle<${type}>;
${aliasHandle})";

    return replaceWithMap(
      sharedHandleTemplate,
      { { "aliasHandle", aliasHandle },
        { "deleterAction", ( handleData.second.deleteCommand.substr( 2, 4 ) == "Free" ) ? "Free" : "Destroy" },
        { "deleterPool", handleData.second.deletePool.empty() ? "" : ", " + stripPrefix( handleData.second.deletePool, "Vk" ) },
        { "deleterType", handleData.second.deletePool.empty() ? "Object" : "Pool" },
        { "destructor", handleData.second.destructorType.empty() ? "NoDestructor" : stripPrefix( handleData.second.destructorType, "Vk" ) },
        { "type", type } } );
  }
  return "";
}

std::string VulkanHppGenerator::generateSharedHandleNoDestroy( std::pair<std::string, HandleData> const & handleData ) const
{
  if ( handleData.second.deleteCommand.empty() )
  {
    std::string type = stripPrefix( handleData.first, "Vk" );
    std::string aliasHandle;
    for ( auto const & alias : handleData.second.aliases )
    {
      static const std::string aliasHandleTemplate = R"(  using Shared${aliasType} = SharedHandle<${type}>;)";

      aliasHandle += replaceWithMap( aliasHandleTemplate, { { "aliasType", stripPrefix( alias.first, "Vk" ) }, { "type", type } } );
    }

    static const std::string sharedHandleTemplate = R"(
template <>
class SharedHandle<${type}> : public SharedHandleBaseNoDestroy<${type}, ${parent}>
{
  friend SharedHandleBase<${type}, ${parent}>;

public:
  SharedHandle() = default;
  explicit SharedHandle(${type} handle, ${parent} parent) noexcept
    : SharedHandleBaseNoDestroy<${type}, ${parent}>(handle, std::move(parent))
  {}
};
using Shared${type} = SharedHandle<${type}>;
${aliasHandle})";

    return replaceWithMap( sharedHandleTemplate,
                           { { "aliasHandle", aliasHandle }, { "parent", "Shared" + stripPrefix( handleData.second.parent, "Vk" ) }, { "type", type } } );
  }
  return "";
}

std::string VulkanHppGenerator::generateSharedHandle( std::vector<RequireData> const & requireData, std::string const & title ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto handleIt = m_handles.find( type.name );
      if ( handleIt != m_handles.end() )
      {
        str += generateSharedHandle( *handleIt );
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateSharedHandleNoDestroy( std::vector<RequireData> const & requireData, std::string const & title ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto handleIt = m_handles.find( type.name );
      if ( handleIt != m_handles.end() )
      {
        str += generateSharedHandleNoDestroy( *handleIt );
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateSharedHandlesNoDestroy() const
{
  std::string sharedHandles;
  for ( auto const & feature : m_features )
  {
    sharedHandles += generateSharedHandleNoDestroy( feature.requireData, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    sharedHandles += generateSharedHandleNoDestroy( extension.requireData, extension.name );
  }
  assert( sharedHandles.back() == '\n' );
  sharedHandles.pop_back();
  return sharedHandles;
}

std::string VulkanHppGenerator::generateSharedHandles() const
{
  std::string sharedHandlesTemplate = R"(
  //======================
  //=== SHARED HANDLEs ===
  //======================

${sharedHandles}
)";

  std::string sharedHandles;
  for ( auto const & feature : m_features )
  {
    sharedHandles += generateSharedHandle( feature.requireData, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    sharedHandles += generateSharedHandle( extension.requireData, extension.name );
  }
  assert( sharedHandles.back() == '\n' );
  sharedHandles.pop_back();
  return replaceWithMap( sharedHandlesTemplate, { { "sharedHandles", sharedHandles } } );
}

std::string VulkanHppGenerator::generateVectorSizeCheck( std::string const &                           name,
                                                         CommandData const &                           commandData,
                                                         size_t                                        initialSkipCount,
                                                         std::map<size_t, std::vector<size_t>> const & countToVectorMap,
                                                         std::set<size_t> const &                      skippedParams,
                                                         bool                                          onlyThrows ) const
{
  std::string const assertTemplate = "    VULKAN_HPP_ASSERT( ${zeroSizeCheck}${firstVectorName}.size() == ${secondVectorName}.size() );";
  std::string const assertTemplateVoid =
    "    VULKAN_HPP_ASSERT( ${zeroSizeCheck}${firstVectorName}.size() * sizeof( ${firstDataType} ) == ${secondVectorName}.size() * sizeof( ${secondDataType} ) );";
  std::string const assertTemplateSingle = "    VULKAN_HPP_ASSERT( ${vectorName}.size() == ${sizeValue} );";
  std::string const throwTemplate =
    R"#(    if ( ${zeroSizeCheck}${firstVectorName}.size() != ${secondVectorName}.size() )
  {
    throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::${className}::${commandName}: ${firstVectorName}.size() != ${secondVectorName}.size()" );
  })#";
  std::string const throwTemplateVoid =
    R"#(    if ( ${zeroSizeCheck}${firstVectorName}.size() * sizeof( ${firstDataType} ) != ${secondVectorName}.size() * sizeof( ${secondDataType} ) )
  {
    throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::${className}::${commandName}: ${firstVectorName}.size() * sizeof( ${firstDataType} ) != ${secondVectorName}.size() * sizeof( ${secondDataType} )" );
  })#";
  std::string const throwTemplateSingle = R"#(    if ( ${vectorName}.size() != ${sizeValue} )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::${className}::${commandName}: ${vectorName}.size() != ${sizeValue}" );
    })#";

  std::string className   = stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" );
  std::string commandName = generateCommandName( name, commandData.params, initialSkipCount );

  std::string assertions, throws;
  for ( auto const & cvm : countToVectorMap )
  {
    const size_t defaultStartIndex = determineDefaultStartIndex( commandData.params, skippedParams );
    std::string  firstVectorName   = startLowerCase( stripPrefix( commandData.params[cvm.second[0]].name, "p" ) );

    if ( cvm.second.size() == 1 )
    {
      std::string sizeValue;
      if ( isLenByStructMember( commandData.params[cvm.second[0]].lenExpression, commandData.params[cvm.first] ) )
      {
        std::vector<std::string> lenParts = tokenize( commandData.params[cvm.second[0]].lenExpression, "->" );
        assert( lenParts.size() == 2 );
        sizeValue = startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + lenParts[1];
      }
      else
      {
        assert( !commandData.params[cvm.second[0]].lenParams.empty() );
        if ( commandData.params[cvm.first].type.type == "VkSampleCountFlagBits" )
        {
          assert( commandData.params[cvm.second[0]].lenExpression == "(" + commandData.params[cvm.second[0]].lenParams[0].first + " + 31) / 32" );
          sizeValue = "( static_cast<uint32_t>( " + commandData.params[cvm.second[0]].lenParams[0].first + " ) + 31 ) / 32";
        }
        else
        {
          sizeValue = commandData.params[cvm.second[0]].lenExpression;
        }
      }
      assertions += replaceWithMap( assertTemplateSingle, { { "sizeValue", sizeValue }, { "vectorName", firstVectorName } } );
      throws += replaceWithMap( throwTemplateSingle,
                                { { "className", className }, { "commandName", commandName }, { "sizeValue", sizeValue }, { "vectorName", firstVectorName } } );
    }
    else
    {
      for ( size_t i = 1; i < cvm.second.size(); i++ )
      {
        std::string secondVectorName  = startLowerCase( stripPrefix( commandData.params[cvm.second[i]].name, "p" ) );
        const bool  withZeroSizeCheck = commandData.params[cvm.second[i]].optional && ( defaultStartIndex <= cvm.second[i] );
        if ( commandData.params[cvm.second[0]].type.type == "void" )
        {
          assert( commandData.params[cvm.second[i]].type.type == "void" );
          std::string firstDataType  = stripPrefix( commandData.params[cvm.second[0]].name, "p" ) + "Type";
          std::string secondDataType = stripPrefix( commandData.params[cvm.second[i]].name, "p" ) + "Type";
          assertions += replaceWithMap( assertTemplateVoid,
                                        { { "firstDataType", firstDataType },
                                          { "firstVectorName", firstVectorName },
                                          { "secondDataType", secondDataType },
                                          { "secondVectorName", secondVectorName },
                                          { "zeroSizeCheck", withZeroSizeCheck ? ( secondVectorName + ".empty() || " ) : "" } } );
          throws += replaceWithMap( throwTemplateVoid,
                                    { { "firstDataType", firstDataType },
                                      { "firstVectorName", firstVectorName },
                                      { "className", className },
                                      { "commandName", commandName },
                                      { "secondDataType", secondDataType },
                                      { "secondVectorName", secondVectorName },
                                      { "zeroSizeCheck", withZeroSizeCheck ? ( "!" + secondVectorName + ".empty() && " ) : "" } } );
        }
        else
        {
          assertions += replaceWithMap( assertTemplate,
                                        { { "firstVectorName", firstVectorName },
                                          { "secondVectorName", secondVectorName },
                                          { "zeroSizeCheck", withZeroSizeCheck ? ( secondVectorName + ".empty() || " ) : "" } } );
          throws += replaceWithMap( throwTemplate,
                                    { { "firstVectorName", firstVectorName },
                                      { "className", className },
                                      { "commandName", commandName },
                                      { "secondVectorName", secondVectorName },
                                      { "zeroSizeCheck", withZeroSizeCheck ? ( "!" + secondVectorName + ".empty() && " ) : "" } } );
        }
        if ( i + 1 < cvm.second.size() )
        {
          assertions += "\n";
          throws += "\n";
        }
      }
    }
  }

  std::string const sizeCheckTemplate =
    R"(#ifdef VULKAN_HPP_NO_EXCEPTIONS
${assertions}
#else
${throws}
#endif  /*VULKAN_HPP_NO_EXCEPTIONS*/)";

  return onlyThrows ? throws : replaceWithMap( sizeCheckTemplate, { { "assertions", assertions }, { "throws", throws } } );
}

std::pair<std::string, std::string> VulkanHppGenerator::getParentTypeAndName( std::pair<std::string, HandleData> const & handle ) const
{
  std::string parentType, parentName;
  if ( handle.first == "VkInstance" )
  {
    parentType = "Context";
    parentName = "context";
  }
  else
  {
    const bool skip = skipLeadingGrandParent( handle );
    assert( !handle.second.constructorIts.empty() && ( ( skip ? 1u : 0u ) < handle.second.constructorIts.front()->second.params.size() ) );
    auto const & param = handle.second.constructorIts.front()->second.params[skip ? 1 : 0];
    assert( isHandleType( param.type.type ) && param.type.isValue() );
    parentType = stripPrefix( param.type.type, "Vk" );
    parentName = param.name;
  }
  return std::make_pair( parentType, parentName );
}

std::string VulkanHppGenerator::getPlatform( std::string const & title ) const
{
  if ( !isSupportedFeature( title ) )
  {
    auto extensionIt = findSupportedExtension( title );
    assert( extensionIt != m_extensions.end() );
    return extensionIt->platform;
  }
  return "";
}

std::pair<std::string, std::string> VulkanHppGenerator::getPoolTypeAndName( std::string const & type ) const
{
  auto structIt = m_structs.find( type );
  assert( structIt != m_structs.end() );
  auto memberIt = std::find_if(
    structIt->second.members.begin(), structIt->second.members.end(), []( MemberData const & md ) { return md.name.find( "Pool" ) != std::string::npos; } );
  assert( memberIt != structIt->second.members.end() );
  assert( std::none_of(
    std::next( memberIt ), structIt->second.members.end(), []( MemberData const & md ) { return md.name.find( "Pool" ) != std::string::npos; } ) );
  return std::make_pair( memberIt->type.type, memberIt->name );
}

std::string VulkanHppGenerator::getProtectFromPlatform( std::string const & platform ) const
{
  auto platformIt = m_platforms.find( platform );
  return ( platformIt != m_platforms.end() ) ? platformIt->second.protect : "";
}

std::string VulkanHppGenerator::getProtectFromTitle( std::string const & title ) const
{
  if ( !isSupportedFeature( title ) )
  {
    auto extensionIt = findSupportedExtension( title );
    return ( extensionIt != m_extensions.end() ) ? getProtectFromPlatform( extensionIt->platform ) : "";
  }
  return "";
}

std::string VulkanHppGenerator::getProtectFromTitles( std::set<std::string> const & titles ) const
{
  for ( auto titleIt = titles.begin(); titleIt != titles.end(); ++titleIt )
  {
    std::string protect = getProtectFromTitle( *titleIt );
    if ( !protect.empty() )
    {
#if !defined( NDEBUG )
      for ( titleIt = std::next( titleIt ); titleIt != titles.end(); ++titleIt )
      {
        std::string p = getProtectFromTitle( *titleIt );
        assert( p.empty() || ( p == protect ) );
      }
#endif
      return protect;
    }
  }
  return "";
}

std::string VulkanHppGenerator::getProtectFromType( std::string const & type ) const
{
  auto typeIt = m_types.find( type );
  assert( typeIt != m_types.end() );
  return getProtectFromTitles( typeIt->second.requiredBy );
}

std::string VulkanHppGenerator::getVectorSize( std::vector<ParamData> const &            params,
                                               std::map<size_t, VectorParamData> const & vectorParams,
                                               size_t                                    returnParam,
                                               std::string const &                       returnParamType,
                                               std::set<size_t> const &                  templatedParams ) const
{
  std::string vectorSize;
  auto        returnVectorIt = vectorParams.find( returnParam );
  assert( returnVectorIt != vectorParams.end() );
  if ( returnVectorIt->second.byStructure )
  {
    auto structIt = m_structs.find( params[returnParam].type.type );
    assert( structIt != m_structs.end() );
    vectorSize = startLowerCase( stripPrefix( params[returnParam].name, "p" ) ) + "." + structIt->second.members[returnVectorIt->second.lenParam].name;
  }
  else
  {
    std::vector<std::string> lenParts = tokenize( params[returnParam].lenExpression, "->" );
    switch ( lenParts.size() )
    {
      case 1:
        {
          std::string const & len = lenParts[0];
          size_t              lenIdx =
            std::distance( params.begin(), std::find_if( params.begin(), params.end(), [&len]( ParamData const & pd ) { return pd.name == len; } ) );
          assert( lenIdx < params.size() );
          // look for the len, not being the len of the return param, but of an other vector param
          auto lenVectorParamIt =
            std::find_if( vectorParams.begin(),
                          vectorParams.end(),
                          [&lenIdx, &returnParam]( auto const & vpi ) { return ( vpi.first != returnParam ) && ( vpi.second.lenParam == lenIdx ); } );
          if ( lenVectorParamIt == vectorParams.end() )
          {
            vectorSize = lenParts[0];
            if ( templatedParams.contains( returnParam ) )
            {
              vectorSize += " / sizeof( " + returnParamType + " )";
            }
          }
          else
          {
            assert( !templatedParams.contains( returnParam ) );
            vectorSize = startLowerCase( stripPrefix( params[lenVectorParamIt->first].name, "p" ) ) + ".size()";
          }
        }
        break;
      case 2:
        assert( vectorParams.contains( returnParam ) );
        vectorSize = startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + lenParts[1];
        break;
      default: assert( false ); break;
    }
  }
  assert( !vectorSize.empty() );
  return vectorSize;
}

void VulkanHppGenerator::handleRemoval( RemoveData const & removeData )
{
  for ( auto const & command : removeData.commands )
  {
    bool removed   = false;
    auto commandIt = findByNameOrAlias( m_commands, command );
    assert( commandIt != m_commands.end() );
    for ( auto const & requiredBy : commandIt->second.requiredBy )
    {
      auto featureIt = std::find_if( m_features.begin(), m_features.end(), [&requiredBy]( FeatureData const & fd ) { return fd.name == requiredBy; } );
      if ( featureIt != m_features.end() )
      {
        removed |= handleRemovalCommand( command, featureIt->requireData );
      }
      else
      {
        auto extensionIt =
          std::find_if( m_extensions.begin(), m_extensions.end(), [&requiredBy]( ExtensionData const & ed ) { return ed.name == requiredBy; } );
        if ( extensionIt != m_extensions.end() )
        {
          removed |= handleRemovalCommand( command, extensionIt->requireData );
        }
        else
        {
          assert( isUnsupportedExtension( requiredBy ) );
          removed = true;
        }
      }
    }
    checkForError( removed, commandIt->second.xmlLine, "to be removed command <" + commandIt->first + "> could not be removed" );
  }

  for ( auto const & e : removeData.enums )
  {
    bool removed = false;
    for ( auto enumIt = m_enums.begin(); !removed && enumIt != m_enums.end(); ++enumIt )
    {
      auto valueIt = findByName( enumIt->second.values, e );
      if ( valueIt != enumIt->second.values.end() )
      {
        enumIt->second.values.erase( valueIt );
        removed = true;
      }
    }
    checkForError( removed, 0, "to be removed enum value <" + e + "> could not be removed" );
  }

  for ( auto const & type : removeData.types )
  {
    auto typeIt = m_types.find( type );
    assert( typeIt != m_types.end() );
    bool removed = typeIt->second.requiredBy.empty();
    for ( auto const & requiredBy : typeIt->second.requiredBy )
    {
      auto featureIt = std::find_if( m_features.begin(), m_features.end(), [&requiredBy]( FeatureData const & fd ) { return fd.name == requiredBy; } );
      if ( featureIt != m_features.end() )
      {
        removed |= handleRemovalType( type, featureIt->requireData );
      }
      else
      {
        auto extensionIt =
          std::find_if( m_extensions.begin(), m_extensions.end(), [&requiredBy]( ExtensionData const & ed ) { return ed.name == requiredBy; } );
        if ( extensionIt != m_extensions.end() )
        {
          removed |= handleRemovalType( type, extensionIt->requireData );
        }
        else
        {
          assert( isUnsupportedExtension( requiredBy ) );
          removed = true;
        }
      }
    }
    checkForError( removed, typeIt->second.xmlLine, "to be removed type <" + typeIt->first + "> could not be removed" );
  }
}

bool VulkanHppGenerator::handleRemovalCommand( std::string const & command, std::vector<RequireData> & requireData )
{
  bool removed = false;
  for ( auto requireDataIt = requireData.begin(); !removed && ( requireDataIt != requireData.end() ); ++requireDataIt )
  {
    auto requireCommandIt =
      std::find_if( requireDataIt->commands.begin(), requireDataIt->commands.end(), [&command]( auto const & c ) { return c.name == command; } );
    if ( requireCommandIt != requireDataIt->commands.end() )
    {
      assert( std::none_of(
        std::next( requireCommandIt ), requireDataIt->commands.end(), [&command]( auto const & requireCommand ) { return requireCommand.name == command; } ) );
      requireDataIt->commands.erase( requireCommandIt );
      assert( !requireDataIt->commands.empty() || !requireDataIt->types.empty() );
      removed = true;

#if !defined( NDEBUG )
      for ( auto it = std::next( requireDataIt ); it != requireData.end(); ++it )
      {
        assert(
          std::none_of( it->commands.begin(), it->commands.end(), [&command]( auto const & requireCommand ) { return requireCommand.name == command; } ) );
      }
#endif
    }
  }
  return removed;
}

void VulkanHppGenerator::handleRemovals()
{
  for ( auto const & feature : m_features )
  {
    for ( auto const & removeData : feature.removeData )
    {
      handleRemoval( removeData );
    }
  }
}

bool VulkanHppGenerator::handleRemovalType( std::string const & type, std::vector<RequireData> & requireData )
{
  bool removed = false;
  for ( auto requireDataIt = requireData.begin(); !removed && ( requireDataIt != requireData.end() ); )
  {
    auto requireTypeIt =
      std::find_if( requireDataIt->types.begin(), requireDataIt->types.end(), [&type]( auto const & requireType ) { return requireType.name == type; } );
    if ( requireTypeIt != requireDataIt->types.end() )
    {
      assert(
        std::none_of( std::next( requireTypeIt ), requireDataIt->types.end(), [&type]( auto const & requireType ) { return requireType.name == type; } ) );
      requireDataIt->types.erase( requireTypeIt );
      requireDataIt = ( requireDataIt->commands.empty() && requireDataIt->types.empty() ) ? requireData.erase( requireDataIt ) : std::next( requireDataIt );
      removed       = true;

#if !defined( NDEBUG )
      for ( auto it = std::next( requireDataIt ); it != requireData.end(); ++it )
      {
        assert( std::none_of( it->types.begin(), it->types.end(), [&type]( auto const & requireType ) { return requireType.name == type; } ) );
      }
#endif
    }
    else
    {
      requireDataIt = std::next( requireDataIt );
    }
  }
  return removed;
}

bool VulkanHppGenerator::hasArrayConstructor( HandleData const & handleData ) const
{
  for ( auto constructorIt : handleData.constructorIts )
  {
    if ( !determineVectorParams( constructorIt->second.params ).empty() )
    {
      return true;
    }
  }
  return false;
}

bool VulkanHppGenerator::hasLen( MemberData const & memberData, std::vector<MemberData> const & members ) const
{
  return ( !memberData.lenMembers.empty() &&
           ( ( ( memberData.lenExpressions[0] == memberData.lenMembers[0].first ) && members[memberData.lenMembers[0].second].type.isValue() ) ||
             ( memberData.lenExpressions[0] == ( memberData.lenMembers[0].first + " / 4" ) ) ) ) ||
         ( !memberData.lenExpressions.empty() && ( memberData.lenExpressions[0] == "null-terminated" ) && !memberData.arraySizes.empty() );
}

bool VulkanHppGenerator::hasParentHandle( std::string const & handle, std::string const & parent ) const
{
  std::string candidate = handle;
  while ( !candidate.empty() )
  {
    auto const & handleIt = m_handles.find( candidate );
    assert( handleIt != m_handles.end() );
    if ( handleIt->second.parent == parent )
    {
      return true;
    }
    else
    {
      candidate = handleIt->second.parent;
    }
  }
  return false;
}

bool VulkanHppGenerator::isDeviceCommand( CommandData const & commandData ) const
{
  return !commandData.handle.empty() && !commandData.params.empty() && m_handles.contains( commandData.params[0].type.type ) &&
         ( commandData.params[0].type.type != "VkInstance" ) && ( commandData.params[0].type.type != "VkPhysicalDevice" );
}

bool VulkanHppGenerator::isExtension( std::string const & name ) const
{
  return isSupportedExtension( name ) || isUnsupportedExtension( name );
}

bool VulkanHppGenerator::isFeature( std::string const & name ) const
{
  return isSupportedFeature( name ) || isUnsupportedFeature( name );
}

bool VulkanHppGenerator::isHandleType( std::string const & type ) const
{
  return type.starts_with( "Vk" ) && ( findByNameOrAlias( m_handles, type ) != m_handles.end() );
}

bool VulkanHppGenerator::isHandleTypeByStructure( std::string const & type ) const
{
  auto structIt = m_structs.find( type );
  return ( structIt != m_structs.end() ) && describesVector( structIt->second ) && isHandleType( structIt->second.members[3].type.type );
}

bool VulkanHppGenerator::isLenByStructMember( std::string const & name, std::vector<ParamData> const & params ) const
{
  // check if name specifies a member of a struct
  std::vector<std::string> nameParts = tokenize( name, "->" );
  if ( nameParts.size() == 1 )
  {
    // older versions of vk.xml used the notation parameter::member
    nameParts = tokenize( name, "::" );
  }
  if ( nameParts.size() == 2 )
  {
    auto paramIt = std::find_if( params.begin(), params.end(), [&n = nameParts[0]]( ParamData const & pd ) { return pd.name == n; } );
    if ( paramIt != params.end() )
    {
#if !defined( NDEBUG )
      auto structureIt = m_structs.find( paramIt->type.type );
      assert( structureIt != m_structs.end() );
      assert( isStructMember( nameParts[1], structureIt->second.members ) );
#endif
      return true;
    }
  }
  return false;
}

bool VulkanHppGenerator::isLenByStructMember( std::string const & name, ParamData const & param ) const
{
  // check if name specifies a member of a struct
  std::vector<std::string> nameParts = tokenize( name, "->" );
  if ( nameParts.size() == 1 )
  {
    // older versions of vk.xml used the notation parameter::member
    nameParts = tokenize( name, "::" );
  }
  if ( ( nameParts.size() == 2 ) && ( nameParts[0] == param.name ) )
  {
#if !defined( NDEBUG )
    auto structureIt = m_structs.find( param.type.type );
    assert( structureIt != m_structs.end() );
    assert( isStructMember( nameParts[1], structureIt->second.members ) );
#endif
    return true;
  }
  return false;
}

bool VulkanHppGenerator::isMultiSuccessCodeConstructor( std::vector<std::map<std::string, CommandData>::const_iterator> const & constructorIts ) const
{
  bool ok = !constructorIts.empty();
  if ( ok )
  {
    auto constructorIt = constructorIts.begin();
    ok                 = ( 2 < ( *constructorIt )->second.successCodes.size() ) ||
         ( ( ( *constructorIt )->second.successCodes.size() == 2 ) && ( ( *constructorIt )->second.successCodes[1] != "VK_INCOMPLETE" ) );
#if !defined( NDEBUG )
    for ( constructorIt = std::next( constructorIt ); constructorIt != constructorIts.end(); ++constructorIt )
    {
      assert( ok == ( 2 < ( *constructorIt )->second.successCodes.size() ) ||
              ( ( ( *constructorIt )->second.successCodes.size() == 2 ) && ( ( *constructorIt )->second.successCodes[1] != "VK_INCOMPLETE" ) ) );
    }
#endif
  }
  return ok;
}

bool VulkanHppGenerator::isParam( std::string const & name, std::vector<ParamData> const & params ) const
{
  return findParamIt( name, params ) != params.end();
}

bool VulkanHppGenerator::isStructMember( std::string const & name, std::vector<MemberData> const & memberData ) const
{
  return findStructMemberIt( name, memberData ) != memberData.end();
}

bool VulkanHppGenerator::isStructureChainAnchor( std::string const & type ) const
{
  if ( type.starts_with( "Vk" ) )
  {
    auto it = findByNameOrAlias( m_structs, type );
    if ( it != m_structs.end() )
    {
      return it->second.isExtended;
    }
  }
  return false;
}

bool VulkanHppGenerator::isStructureType( std::string const & type ) const
{
  return type.starts_with( "Vk" ) && ( findByNameOrAlias( m_structs, type ) != m_structs.end() );
}

bool VulkanHppGenerator::isSupported( std::set<std::string> const & requiredBy ) const
{
  for ( auto const & r : requiredBy )
  {
    if ( isSupportedFeature( r ) || isSupportedExtension( r ) )
    {
      return true;
    }
    else
    {
      assert( isUnsupportedFeature( r ) || isUnsupportedExtension( r ) );
    }
  }
  return false;
}

bool VulkanHppGenerator::isSupportedExtension( std::string const & name ) const
{
  return std::any_of( m_extensions.begin(), m_extensions.end(), [&name]( ExtensionData const & ed ) { return ed.name == name; } );
}

bool VulkanHppGenerator::isSupportedFeature( std::string const & name ) const
{
  return std::any_of( m_features.begin(), m_features.end(), [&name]( FeatureData const & fd ) { return fd.name == name; } );
}

bool VulkanHppGenerator::isTypeRequired( std::string const & type ) const
{
  auto typeIt = m_types.find( type );
  assert( typeIt != m_types.end() );
  return !typeIt->second.requiredBy.empty();
}

bool VulkanHppGenerator::isTypeUsed( std::string const & type ) const
{
  auto typeIt = m_types.find( type );
  assert( typeIt != m_types.end() );
  for ( auto const & require : typeIt->second.requiredBy )
  {
    auto featureIt = findFeature( require );
    if ( featureIt != m_features.end() )
    {
      for ( auto const & r : featureIt->requireData )
      {
        if ( std::any_of( r.types.begin(), r.types.end(), [&type]( auto const & requireType ) { return requireType.name == type; } ) )
        {
          return true;
        }
      }
    }
    else
    {
      auto extensionIt = findSupportedExtension( require );
      if ( extensionIt != m_extensions.end() )
      {
        for ( auto const & r : extensionIt->requireData )
        {
          if ( std::any_of( r.types.begin(), r.types.end(), [&type]( auto const & requireType ) { return requireType.name == type; } ) )
          {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool VulkanHppGenerator::isUnsupportedExtension( std::string const & name ) const
{
  return std::any_of( m_unsupportedExtensions.begin(), m_unsupportedExtensions.end(), [&name]( auto const & ed ) { return ed.name == name; } );
}

bool VulkanHppGenerator::isUnsupportedFeature( std::string const & name ) const
{
  return std::any_of( m_unsupportedFeatures.begin(), m_unsupportedFeatures.end(), [&name]( auto const & ed ) { return ed.name == name; } );
}

bool VulkanHppGenerator::isVectorByStructure( std::string const & type ) const
{
  auto structIt = m_structs.find( type );
  return ( structIt != m_structs.end() ) && describesVector( structIt->second );
}

void VulkanHppGenerator::markExtendedStructs()
{
  for ( auto const & s : m_structs )
  {
    for ( auto const & extends : s.second.structExtends )
    {
      auto structIt = m_structs.find( extends );
      checkForError( structIt != m_structs.end(), s.second.xmlLine, "struct <" + s.first + "> extends unknown struct <" + extends + ">" );
      structIt->second.isExtended = true;
    }
  }
}

bool VulkanHppGenerator::needsStructureChainResize( std::map<size_t, VectorParamData> const & vectorParams,
                                                    std::vector<size_t> const &               chainedReturnParams ) const
{
  auto it = std::find_if( chainedReturnParams.begin(), chainedReturnParams.end(), [&vectorParams]( size_t crp ) { return vectorParams.contains( crp ); } );
  // assert that there's no other chained vector param !
  assert( ( it == chainedReturnParams.end() ) ||
          std::none_of( std::next( it ), chainedReturnParams.end(), [&vectorParams]( size_t crp ) { return vectorParams.contains( crp ); } ) );
  return ( it != chainedReturnParams.end() );
}

std::pair<bool, std::map<size_t, std::vector<size_t>>> VulkanHppGenerator::needsVectorSizeCheck( std::vector<ParamData> const &            params,
                                                                                                 std::map<size_t, VectorParamData> const & vectorParams,
                                                                                                 std::vector<size_t> const &               returnParams,
                                                                                                 std::set<size_t> const &                  singularParams,
                                                                                                 std::set<size_t> const &                  skippedParams ) const
{
  std::map<size_t, std::vector<size_t>> countToVectorMap;
  for ( auto const & vpi : vectorParams )
  {
    if ( ( vpi.second.lenParam != INVALID_INDEX ) &&
         std::none_of( returnParams.begin(), returnParams.end(), [&vpi]( size_t rp ) { return rp == vpi.first; } ) &&
         ( !singularParams.contains( vpi.second.lenParam ) || isLenByStructMember( params[vpi.first].lenExpression, params[vpi.second.lenParam] ) ) )
    {
      countToVectorMap[vpi.second.lenParam].push_back( vpi.first );
    }
  }
  return std::make_pair( std::any_of( countToVectorMap.begin(),
                                      countToVectorMap.end(),
                                      [this, &params, &skippedParams]( auto const & cvm )
                                      {
                                        return ( 1 < cvm.second.size() ) || isLenByStructMember( params[cvm.second[0]].lenExpression, params[cvm.first] ) ||
                                               std::none_of( params[cvm.second[0]].lenParams.begin(),
                                                             params[cvm.second[0]].lenParams.end(),
                                                             [&skippedParams]( std::pair<std::string, size_t> const & lenParam )
                                                             { return skippedParams.contains( lenParam.second ); } );
                                      } ),
                         countToVectorMap );
}

void VulkanHppGenerator::readCommand( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  auto                               aliasIt    = attributes.find( "alias" );
  if ( aliasIt != attributes.end() )
  {
    checkAttributes( line, attributes, { { "alias", {} }, { "name", {} } }, {} );

    std::string alias = aliasIt->second;
    std::string name  = attributes.find( "name" )->second;

    checkForError( name.starts_with( "vk" ), line, "name <" + name + "> should begin with \"vk\"" );
    auto commandIt = m_commands.find( alias );
    checkForError( commandIt != m_commands.end(), line, "command <" + name + "> is aliased to unknown command <" + alias + ">" );
    checkForError(
      commandIt->second.aliases.insert( { name, line } ).second, line, "command <" + name + "> is already listed as alias for command <" + alias + ">" );
  }
  else
  {
    checkAttributes( line,
                     attributes,
                     {},
                     { { "api", { "vulkan", "vulkansc" } },
                       { "cmdbufferlevel", { "primary", "secondary" } },
                       { "comment", {} },
                       { "errorcodes", {} },
                       { "queues", {} },
                       { "renderpass", { "both", "inside", "outside" } },
                       { "successcodes", {} },
                       { "tasks", { "action", "indirection", "state", "synchronization" } },
                       { "videocoding", { "both", "inside", "outside" } } } );

    std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
    checkElements( line, children, { { "param", false }, { "proto", true } }, { "implicitexternsyncparams" } );

    CommandData commandData;
    commandData.xmlLine = line;
    std::string api;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "api" )
      {
        api = attribute.second;
      }
      else if ( attribute.first == "errorcodes" )
      {
        commandData.errorCodes = tokenize( attribute.second, "," );
        // errorCodes are checked in checkCorrectness after complete reading
      }
      else if ( attribute.first == "queues" )
      {
        m_commandQueues.insert( attribute.second );
      }
      else if ( attribute.first == "successcodes" )
      {
        commandData.successCodes = tokenize( attribute.second, "," );
        // successCodes are checked in checkCorrectness after complete reading
      }
    }

    std::string name;
    for ( auto child : children )
    {
      std::string value = child->Value();
      if ( value == "param" )
      {
        std::pair<bool, ParamData> result = readCommandParam( child, commandData.params );
        if ( result.first )
        {
          commandData.params.push_back( result.second );
        }
      }
      else if ( value == "proto" )
      {
        std::tie( name, commandData.returnType ) = readCommandProto( child );
      }
    }

    for ( auto & param : commandData.params )
    {
      for ( auto & lenParam : param.lenParams )
      {
        auto paramIt = findParamIt( lenParam.first, commandData.params );
        checkForError( paramIt != commandData.params.end(),
                       param.xmlLine,
                       "param <" + param.name + "> uses unknown len parameter <" + lenParam.first + "> in its \"altlen\" attribute <" + param.lenExpression +
                         ">" );
        lenParam.second = std::distance( commandData.params.cbegin(), paramIt );
      }
      if ( !param.strideParam.first.empty() )
      {
        auto paramIt = findParamIt( param.strideParam.first, commandData.params );
        checkForError(
          paramIt != commandData.params.end(), param.xmlLine, "param <" + param.name + "> uses unknown stride parameter <" + param.strideParam.first + ">" );
        param.strideParam.second = std::distance( commandData.params.cbegin(), paramIt );
      }
    }

    assert( !name.empty() );
    checkForError( ( commandData.returnType == "VkResult" ) || commandData.errorCodes.empty(),
                   line,
                   "command <" + name + "> does not return a VkResult but specifies errorcodes" );
    checkForError( ( commandData.returnType == "VkResult" ) || commandData.successCodes.empty(),
                   line,
                   "command <" + name + "> does not return a VkResult but specifies successcodes" );

    if ( api.empty() || ( api == m_api ) )
    {
      checkForError( !m_commands.contains( name ), line, "command <" + name + "> already specified" );
      addCommand( name, commandData );
    }
  }
}

std::pair<bool, VulkanHppGenerator::ParamData> VulkanHppGenerator::readCommandParam( tinyxml2::XMLElement const *   element,
                                                                                     std::vector<ParamData> const & params )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line,
                   attributes,
                   {},
                   { { "altlen", {} },
                     { "api", { "vulkan", "vulkansc" } },
                     { "externsync", {} },
                     { "len", {} },
                     { "noautovalidity", { "true" } },
                     { "objecttype", { "objectType" } },
                     { "optional", { "false", "true" } },
                     { "stride", {} },
                     { "validstructs", {} } } );

  ParamData paramData;
  paramData.xmlLine = line;
  std::string api;
  for ( auto attribute : attributes )
  {
    if ( attribute.first == "altlen" )
    {
      assert( paramData.lenExpression.empty() );
      paramData.lenExpression = attribute.second;
      paramData.lenParams     = filterNumbers( tokenizeAny( attribute.second, " /()+" ) );
    }
    else if ( attribute.first == "api" )
    {
      api = attribute.second;
    }
    else if ( attribute.first == "len" )
    {
      if ( paramData.lenExpression.empty() )
      {
        paramData.lenExpression = attribute.second;
        auto paramIt            = findParamIt( attribute.second, params );
        if ( paramIt != params.end() )
        {
          paramData.lenParams.push_back( { attribute.second, std::distance( params.cbegin(), paramIt ) } );
        }
        else
        {
          checkForError( ( attribute.second == "null-terminated" ) || ( attribute.second == "1" ) || isLenByStructMember( attribute.second, params ),
                         line,
                         "attribute <len> holds an unknown value <" + attribute.second + ">" );
        }
      }
    }
    else if ( attribute.first == "stride" )
    {
      paramData.strideParam.first = attribute.second;
    }
    else if ( attribute.first == "optional" )
    {
      paramData.optional = ( attribute.second == "true" );
    }
    else if ( attribute.first == "validstructs" )
    {
      std::vector<std::string> validStructs = tokenize( attribute.second, "," );
      for ( auto const & vs : validStructs )
      {
        checkForError( m_structs.contains( vs ), line, "unknown struct <" + vs + "> listed in attribute <validstructs>" );
      }
    }
  }

  NameData nameData;
  std::tie( nameData, paramData.type ) = readNameAndType( element );

  checkForError( paramData.type.prefix.empty() || ( paramData.type.prefix == "const" ) || ( paramData.type.prefix == "const struct" ) ||
                   ( paramData.type.prefix == "struct" ),
                 line,
                 "unexpected type prefix <" + paramData.type.prefix + ">" );
  checkForError( paramData.type.postfix.empty() || ( paramData.type.postfix == "*" ) || ( paramData.type.postfix == "**" ) ||
                   ( paramData.type.postfix == "* const *" ),
                 line,
                 "unexpected type postfix <" + paramData.type.postfix + ">" );
  checkForError(
    paramData.lenParams.empty() || paramData.type.isPointer(), line, "parameter <" + nameData.name + "> has an attribute <len> but is not a pointer" );
  paramData.name       = nameData.name;
  paramData.arraySizes = nameData.arraySizes;

  if ( api.empty() || ( api == m_api ) )
  {
    checkForError( std::none_of( params.begin(), params.end(), [&name = nameData.name]( ParamData const & pd ) { return pd.name == name; } ),
                   line,
                   "command param <" + nameData.name + "> already used" );
  }
  return std::make_pair( api.empty() || ( api == m_api ), paramData );
}

std::pair<std::string, std::string> VulkanHppGenerator::readCommandProto( tinyxml2::XMLElement const * element )
{
  const int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );

  auto [nameData, typeInfo] = readNameAndType( element );

  checkForError( nameData.name.starts_with( "vk" ), line, "name <" + nameData.name + "> does not begin with <vk>" );
  checkForError( nameData.arraySizes.empty(), line, "name <" + nameData.name + "> with unsupported arraySizes" );
  checkForError( m_types.contains( typeInfo.type ), line, "unknown type <" + typeInfo.type + ">" );
  checkForError( typeInfo.prefix.empty(), line, "unexpected type prefix <" + typeInfo.prefix + ">" );
  checkForError( typeInfo.postfix.empty(), line, "unexpected type postfix <" + typeInfo.postfix + ">" );

  return std::make_pair( nameData.name, typeInfo.type );
}

void VulkanHppGenerator::readCommands( tinyxml2::XMLElement const * element )
{
  const int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, { { "comment", {} } } );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "command", false } } );
  for ( auto child : children )
  {
    readCommand( child );
  }
}

void VulkanHppGenerator::readEnums( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, { { "bitwidth", { "64" } }, { "comment", {} }, { "type", { "bitmask", "constants", "enum" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );

  std::string bitwidth, name;
  std::string type = "constants";  // hard-coded fallback for older versions, where "API Constants" did not have a type attribute, yet
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "bitwidth" )
    {
      bitwidth = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "type" )
    {
      type = attribute.second;
    }
  }
  assert( !name.empty() );

  if ( type == "constants" )
  {
    assert( name == "API Constants" );
    checkElements( line, children, { { "enum", false } }, {} );
    for ( auto const & child : children )
    {
      readEnumsConstants( child );
    }
  }
  else
  {
    checkElements( line, children, {}, { "comment", "enum", "unused" } );
    checkForError( !type.empty(), line, "enum without type" );

    // get the EnumData entry in enum map
    auto enumIt = findByNameOrAlias( m_enums, name );
    checkForError( enumIt != m_enums.end(), line, "enum <" + name + "> is not listed as enum in the types section" );
    checkForError( enumIt->second.values.empty(), line, "enum <" + name + "> already holds values" );

    // mark it as a bitmask, if it is one
    const bool bitmask = ( type == "bitmask" );
    if ( bitmask )
    {
      checkForError( name.find( "FlagBits" ) != std::string::npos, line, "bitmask <" + name + "> does not contain \"FlagBits\"" );
    }
    enumIt->second.isBitmask = bitmask;
    enumIt->second.bitwidth  = bitwidth;

    // read the names of the enum values
    for ( auto child : children )
    {
      std::string value = child->Value();
      if ( value == "enum" )
      {
        readEnumsEnum( child, enumIt );
      }
    }
  }
}

void VulkanHppGenerator::readEnumsConstants( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkElements( line, getChildElements( element ), {} );

  std::string name, type, value;
  auto        aliasIt = attributes.find( "alias" );
  if ( aliasIt != attributes.end() )
  {
    checkAttributes( line, attributes, { { "alias", {} }, { "name", {} } }, {} );

    std::string alias;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "alias" )
      {
        alias = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        name = attribute.second;
        checkForError( !m_constants.contains( name ), line, "already specified enum constant <" + name + ">" );
      }
    }

    auto constIt = m_constants.find( alias );
    checkForError( constIt != m_constants.end(), line, "constant alias <" + name + "> is an alias of an unknown constant <" + alias + ">." );
    type  = constIt->second.type;
    value = constIt->second.value;
  }
  else
  {
    checkAttributes( line, attributes, { { "name", {} }, { "type", {} }, { "value", {} } }, { { "comment", {} } } );

    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "name" )
      {
        name = attribute.second;
        checkForError( !m_constants.contains( name ), line, "already specified enum constant <" + name + ">" );
      }
      else if ( attribute.first == "type" )
      {
        type = attribute.second;
      }
      else if ( attribute.first == "value" )
      {
        value = attribute.second;
      }
    }
  }
  checkForError( m_types.insert( { name, TypeData{ TypeCategory::Constant, {}, line } } ).second, line, "enum <" + name + "> already specified" );
  m_constants[name] = { type, value, line };
}

void VulkanHppGenerator::readEnumsEnum( tinyxml2::XMLElement const * element, std::map<std::string, EnumData>::iterator enumIt )
{
  const int                          line       = element->GetLineNum();
  bool                               deprecated = false;
  std::map<std::string, std::string> attributes = getAttributes( element );
  if ( attributes.contains( "alias" ) )
  {
    checkAttributes(
      line, attributes, { { "alias", {} }, { "name", {} } }, { { "api", { "vulkan", "vulkansc" } }, { "comment", {} }, { "deprecated", { "aliased" } } } );
    checkElements( line, getChildElements( element ), {} );

    std::string alias, api, bitpos, name, value;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "alias" )
      {
        alias = attribute.second;
      }
      else if ( attribute.first == "api" )
      {
        api = attribute.second;
      }
      else if ( attribute.first == "deprecated" )
      {
        deprecated = true;
      }
      else if ( attribute.first == "name" )
      {
        name = attribute.second;
      }
    }
    assert( !name.empty() );

    if ( api.empty() || ( api == m_api ) )
    {
      enumIt->second.addEnumAlias( line, name, alias, "", true );
    }
  }
  else
  {
    checkAttributes( line, attributes, { { "name", {} } }, { { "bitpos", {} }, { "comment", {} }, { "value", {} } } );
    checkElements( line, getChildElements( element ), {} );

    std::string alias, bitpos, name, value;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "bitpos" )
      {
        bitpos = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        name = attribute.second;
      }
      else if ( attribute.first == "value" )
      {
        value = attribute.second;
      }
    }

    std::string prefix = generateEnumSuffixes( enumIt->first, enumIt->second.isBitmask ).first;
    checkForError( name.starts_with( prefix ), line, "encountered enum value <" + name + "> that does not begin with expected prefix <" + prefix + ">" );

    checkForError( bitpos.empty() ^ value.empty(), line, "both or none of \"bitpos\" and \"value\" are set for enum <" + name + "> which is invalid" );
    enumIt->second.addEnumValue( line, name, "", bitpos, value, true, deprecated );
  }
}

void VulkanHppGenerator::readExtensionRequire( tinyxml2::XMLElement const * element, ExtensionData & extensionData, bool extensionSupported )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, { { "api", { "vulkan", "vulkansc" } }, { "comment", {} }, { "depends", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "command", "comment", "enum", "feature", "type" } );

  RequireData requireData{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "api" )
    {
      requireData.api = attribute.second;
    }
    else if ( attribute.first == "depends" )
    {
      assert( requireData.depends.empty() );
      requireData.depends = attribute.second;
      checkForWarning( std::none_of( extensionData.requireData.begin(),
                                     extensionData.requireData.end(),
                                     [&requireData]( RequireData const & rd )
                                     { return ( rd.api == requireData.api ) && ( rd.depends == requireData.depends ); } ),
                       line,
                       "required dependency <" + requireData.depends + "> already listed for extension <" + extensionData.name + "> with the same api" );
    }
  }

  const bool requireSupported = requireData.api.empty() || ( requireData.api == m_api );
  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "command" )
    {
      requireData.commands.push_back( readRequireCommand( child ) );
    }
    else if ( value == "enum" )
    {
      readRequireEnum( child, extensionData.name, extensionData.platform, extensionSupported && requireSupported, requireData );
    }
    else if ( value == "feature" )
    {
      requireData.features.push_back( readRequireFeature( child ) );
    }
    else if ( value == "type" )
    {
      requireData.types.push_back( readRequireType( child ) );
    }
  }
  if ( !requireData.commands.empty() || !requireData.types.empty() || !requireData.enumConstants.empty() )
  {
    if ( requireSupported )
    {
      extensionData.requireData.push_back( requireData );
    }
    else
    {
      extensionData.unsupportedRequireData.push_back( requireData );
    }
  }
}

void VulkanHppGenerator::readExtensions( tinyxml2::XMLElement const * element )
{
  const int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, { { "comment", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "extension", false } } );

  for ( auto child : children )
  {
    readExtension( child );
  }
}

void VulkanHppGenerator::readExtension( tinyxml2::XMLElement const * element )
{
  const int                                 line       = element->GetLineNum();
  std::map<std::string, std::string>        attributes = getAttributes( element );
  std::vector<tinyxml2::XMLElement const *> children   = getChildElements( element );

  checkAttributes( line,
                   attributes,
                   { { "name", {} }, { "number", {} }, { "supported", { "disabled", "vulkan", "vulkansc" } } },
                   { { "author", {} },
                     { "comment", {} },
                     { "contact", {} },
                     { "depends", {} },
                     { "deprecatedby", {} },
                     { "nofeatures", { "true" } },
                     { "obsoletedby", {} },
                     { "platform", {} },
                     { "promotedto", {} },
                     { "provisional", { "true" } },
                     { "ratified", { "vulkan", "vulkansc" } },
                     { "sortorder", { "1" } },
                     { "specialuse", { "cadsupport", "d3demulation", "debugging", "devtools", "glemulation" } },
                     { "type", { "device", "instance" } } } );
  checkElements( line, children, { { "require", false } }, { "remove" } );

  ExtensionData extensionData{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "depends" )
    {
      // currently, we map the few complex depends attributes to a canonical format!
      const std::map<std::string, std::string> complexToCanonicalDepends = {
        { "(VK_EXT_filter_cubic)+(VK_VERSION_1_2,VK_EXT_sampler_filter_minmax)",
          "VK_EXT_filter_cubic+VK_EXT_sampler_filter_minmax,VK_VERSION_1_2+VK_EXT_filter_cubic" },
        { "VK_KHR_swapchain+VK_KHR_get_surface_capabilities2+(VK_KHR_get_physical_device_properties2,VK_VERSION_1_1)",
          "VK_KHR_swapchain+VK_KHR_get_surface_capabilities2+VK_KHR_get_physical_device_properties2,VK_VERSION_1_1+VK_KHR_swapchain+VK_KHR_get_surface_capabilities2" },
        { "((VK_KHR_bind_memory2+VK_KHR_get_physical_device_properties2+VK_KHR_sampler_ycbcr_conversion),VK_VERSION_1_1)+(VK_KHR_image_format_list,VK_VERSION_1_2)",
          "VK_KHR_bind_memory2+VK_KHR_get_physical_device_properties2+VK_KHR_sampler_ycbcr_conversion+VK_KHR_image_format_list,VK_VERSION_1_1+VK_KHR_image_format_list,VK_VERSION_1_2" },
        { "VK_KHR_swapchain+(VK_KHR_maintenance2,VK_VERSION_1_1)+(VK_KHR_image_format_list,VK_VERSION_1_2)",
          "VK_KHR_swapchain+VK_KHR_maintenance2+VK_KHR_image_format_list,VK_VERSION_1_1+VK_KHR_swapchain+VK_KHR_image_format_list,VK_VERSION_1_2+VK_KHR_swapchain" },
        { "(VK_KHR_create_renderpass2,VK_VERSION_1_2)+(VK_KHR_get_physical_device_properties2,VK_VERSION_1_1)",
          "VK_KHR_create_renderpass2+VK_KHR_get_physical_device_properties2,VK_VERSION_1_1+VK_KHR_create_renderpass2,VK_VERSION_1_2" },
        { "(VK_KHR_get_physical_device_properties2+VK_KHR_device_group),VK_VERSION_1_1",
          "VK_KHR_get_physical_device_properties2+VK_KHR_device_group,VK_VERSION_1_1" },
        { "(VK_KHR_get_physical_device_properties2,VK_VERSION_1_1)+(VK_KHR_dynamic_rendering,VK_VERSION_1_3)",
          "VK_KHR_get_physical_device_properties2+VK_KHR_dynamic_rendering,VK_VERSION_1_1+VK_KHR_dynamic_rendering,VK_VERSION_1_3" }
      };
      auto        canonicalIt = complexToCanonicalDepends.find( attribute.second );
      std::string depends;
      if ( canonicalIt == complexToCanonicalDepends.end() )
      {
        depends = attribute.second;
        std::erase_if( depends, []( char c ) { return ( c == '(' ) || ( c == ')' ); } );
      }
      else
      {
        depends = canonicalIt->second;
      }

      // first tokenize by ',', giving a vector of dependencies for different vulkan versions
      std::vector<std::string> allDependencies = tokenize( depends, "," );
      for ( auto dep : allDependencies )
      {
        // for ease of handling, prepend the (optional) VK_VERSION_1_0
        if ( !dep.starts_with( "VK_VERSION" ) )
        {
          assert( dep.find( "VK_VERSION" ) == std::string::npos );
          dep = "VK_VERSION_1_0+" + dep;
        }

        // then tokenize by '+', giving a vector of dependendies for the vulkan version listed as the first element here
        std::vector<std::string> dependsByVersion = tokenize( dep, "+" );
        extensionData.depends[dependsByVersion[0]].push_back( { std::next( dependsByVersion.begin() ), dependsByVersion.end() } );
      }
    }
    else if ( attribute.first == "deprecatedby" )
    {
      extensionData.deprecatedBy = attribute.second;
      extensionData.isDeprecated = true;
    }
    else if ( attribute.first == "name" )
    {
      extensionData.name = attribute.second;
      checkForError( !isExtension( extensionData.name ), line, "already encountered extension <" + extensionData.name + ">" );
    }
    else if ( attribute.first == "number" )
    {
      extensionData.number = attribute.second;
    }
    else if ( attribute.first == "obsoletedby" )
    {
      extensionData.obsoletedBy = attribute.second;
    }
    else if ( attribute.first == "platform" )
    {
      extensionData.platform = attribute.second;
      checkForError( m_platforms.contains( extensionData.platform ), line, "unknown platform <" + extensionData.platform + ">" );
    }
    else if ( attribute.first == "promotedto" )
    {
      extensionData.promotedTo = attribute.second;
    }
    else if ( attribute.first == "provisional" )
    {
      if ( extensionData.platform.empty() )
      {
        // for now, having the attribute provisional="true" implies attribute platform="provisional" to get
        // stuff protected by VK_ENABLE_BETA_EXTENSIONS
        extensionData.platform = "provisional";
      }
      checkForError( extensionData.platform == "provisional",
                     line,
                     "while attribute <provisional> is set to \"true\", attribute <platform> is not set to \"provisional\" but to \"" + extensionData.platform +
                       "\"" );
    }
    else if ( attribute.first == "ratified" )
    {
      extensionData.ratified = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "supported" )
    {
      extensionData.supported = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "type" )
    {
      extensionData.type = attribute.second;
    }
  }

  const bool extensionSupported =
    extensionData.supported.empty() ||
    std::any_of( extensionData.supported.begin(), extensionData.supported.end(), [this]( std::string const & s ) { return s == m_api; } );
  checkForError( !extensionSupported || !extensionData.type.empty(), line, "missing attribute \"type\" for supported extension <" + extensionData.name + ">" );
  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "remove" )
    {
      extensionData.removeData.push_back( readRemoveData( child ) );
    }
    else if ( value == "require" )
    {
      readExtensionRequire( child, extensionData, extensionSupported );
    }
  }

  if ( std::none_of( extensionData.supported.begin(), extensionData.supported.end(), []( std::string const & s ) { return s == "disabled"; } ) )
  {
    // extract the tag from the name, which is supposed to look like VK_<tag>_<other>
    const size_t tagStart = extensionData.name.find( '_' );
    checkForError( tagStart != std::string::npos, line, "name <" + extensionData.name + "> is missing an underscore '_'" );
    const size_t tagEnd = extensionData.name.find( '_', tagStart + 1 );
    checkForError( tagEnd != std::string::npos, line, "name <" + extensionData.name + "> is missing an underscore '_'" );
    std::string tag = extensionData.name.substr( tagStart + 1, tagEnd - tagStart - 1 );
    checkForError( ( m_tags.find( tag ) != m_tags.end() ), line, "name <" + extensionData.name + "> is using an unknown tag <" + tag + ">" );
  }

  if ( extensionSupported )
  {
    m_extensions.push_back( extensionData );
  }
  else
  {
    checkForError( !isUnsupportedExtension( extensionData.name ), line, "unsupported extension <" + extensionData.name + "> already specified" );
    m_unsupportedExtensions.push_back( extensionData );
  }
}

void VulkanHppGenerator::readFeature( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "api", { "vulkan", "vulkansc" } }, { "comment", {} }, { "name", {} }, { "number", {} } }, { { "depends", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "require", false } }, { "remove" } );

  FeatureData featureData;
  featureData.xmlLine = line;
  std::vector<std::string> api;
  std::string              modifiedNumber;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "api" )
    {
      api = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "name" )
    {
      featureData.name = attribute.second;
    }
    else if ( attribute.first == "number" )
    {
      featureData.number = attribute.second;
      modifiedNumber     = featureData.number;
      std::replace( modifiedNumber.begin(), modifiedNumber.end(), '.', '_' );
    }
  }

  const bool featureSupported = std::any_of( api.begin(), api.end(), [this]( std::string const & a ) { return a == m_api; } );
  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "remove" )
    {
      featureData.removeData.push_back( readRemoveData( child ) );
    }
    else if ( value == "require" )
    {
      readFeatureRequire( child, featureData, featureSupported );
    }
  }

  checkForError( featureData.name ==
                   ( std::any_of( api.begin(), api.end(), []( std::string const & a ) { return a == "vulkan"; } ) ? "VK_VERSION_" : "VKSC_VERSION_" ) +
                     modifiedNumber,
                 line,
                 "unexpected formatting of name <" + featureData.name + ">" );
  checkForError( !isFeature( featureData.name ), line, "feature <" + featureData.name + "> already specified" );
  if ( featureSupported )
  {
    m_features.push_back( featureData );
  }
  else
  {
    m_unsupportedFeatures.push_back( featureData );
  }
}

void VulkanHppGenerator::readFeatureRequire( tinyxml2::XMLElement const * element, FeatureData & featureData, bool featureSupported )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, { { "comment", {} }, { "depends", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "command", "comment", "enum", "feature", "type" } );

  std::string depends;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "depends" )
    {
      assert( depends.empty() );
      depends = attribute.second;
      checkForError(
        std::none_of( featureData.requireData.begin(), featureData.requireData.end(), [&depends]( RequireData const & rd ) { return rd.depends == depends; } ),
        line,
        "require depends <" + depends + "> already listed for feature <" + featureData.name + ">" );
    }
  }

  featureData.requireData.push_back( {} );
  RequireData & requireData = featureData.requireData.back();
  requireData.depends       = depends;
  requireData.xmlLine       = line;

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "command" )
    {
      requireData.commands.push_back( readRequireCommand( child ) );
    }
    else if ( value == "enum" )
    {
      readRequireEnum( child, featureData.name, "", featureSupported, requireData );
    }
    else if ( value == "feature" )
    {
      requireData.features.push_back( readRequireFeature( child ) );
    }
    else if ( value == "type" )
    {
      requireData.types.push_back( readRequireType( child ) );
    }
  }
}

void VulkanHppGenerator::readFormat( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes(
    line,
    attributes,
    { { "blockSize", { "1", "2", "3", "4", "5", "6", "8", "12", "16", "24", "27", "32", "36", "48", "64", "80", "100", "125", "150", "180", "216" } },
      { "class", {} },
      { "name", {} },
      { "texelsPerBlock", { "1", "16", "20", "25", "27", "30", "36", "40", "48", "50", "60", "64", "80", "100", "120", "125", "144", "150", "180", "216" } } },
    { { "blockExtent", { "1", "2", "3", "4", "5", "6", "8", "10", "12" } },
      { "chroma", { "420", "422", "444" } },
      { "compressed", { "ASTC HDR", "ASTC LDR", "BC", "EAC", "ETC", "ETC2", "PVRTC" } },
      { "packed", { "8", "16", "32" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "component", false } }, { "plane", "spirvimageformat" } );

  FormatData format;
  format.xmlLine = line;
  std::string name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "blockExtent" )
    {
      checkForError( tokenize( attribute.second, "," ).size() == 3, line, "unexpected number of elements in attribute <blockExtent>" );
      format.blockExtent = attribute.second;
    }
    if ( attribute.first == "blockSize" )
    {
      format.blockSize = attribute.second;
    }
    else if ( attribute.first == "chroma" )
    {
      format.chroma = attribute.second;
    }
    else if ( attribute.first == "class" )
    {
      format.classAttribute = attribute.second;
    }
    else if ( attribute.first == "compressed" )
    {
      format.compressed = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "packed" )
    {
      format.packed = attribute.second;
    }
    else if ( attribute.first == "texelsPerBlock" )
    {
      format.texelsPerBlock = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "component" )
    {
      readFormatComponent( child, format );
    }
    else if ( value == "plane" )
    {
      readFormatPlane( child, format );
    }
    else if ( value == "spirvimageformat" )
    {
      readFormatSPIRVImageFormat( child, format );
    }
  }

  if ( format.components.front().bits == "compressed" )
  {
    for ( auto componentIt = std::next( format.components.begin() ); componentIt != format.components.end(); ++componentIt )
    {
      checkForError( componentIt->bits == "compressed", line, "component is expected to be marked as compressed in attribute <bits>" );
    }
  }
  if ( !format.components.front().planeIndex.empty() )
  {
    for ( auto componentIt = std::next( format.components.begin() ); componentIt != format.components.end(); ++componentIt )
    {
      checkForError( !componentIt->planeIndex.empty(), line, "component is expected to have a planeIndex" );
    }
    const size_t planeCount = 1 + static_cast<size_t>( std::stoi( format.components.back().planeIndex ) );
    checkForError( format.planes.size() == planeCount, line, "number of planes does not fit to largest planeIndex of the components" );
  }

  auto formatIt = m_enums.find( "VkFormat" );
  assert( formatIt != m_enums.end() );

  checkForError( contains( formatIt->second.values, name ) && m_formats.insert( { name, format } ).second, line, "format <" + name + "> already specified" );
}

void VulkanHppGenerator::readFormatComponent( tinyxml2::XMLElement const * element, FormatData & formatData )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line,
                   attributes,
                   { { "bits", { "1", "2", "4", "5", "6", "8", "9", "10", "11", "12", "14", "16", "24", "32", "64", "compressed" } },
                     { "name", {} },
                     { "numericFormat", { "BOOL", "SFLOAT", "SINT", "SNORM", "SRGB", "SFIXED5", "SSCALED", "UFLOAT", "UINT", "UNORM", "USCALED" } } },
                   { { "planeIndex", { "0", "1", "2" } } } );
  checkElements( line, getChildElements( element ), {} );

  formatData.components.emplace_back();
  ComponentData & component = formatData.components.back();
  component.xmlLine         = line;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "bits" )
    {
      checkForError(
        ( attribute.second != "compressed" ) || !formatData.compressed.empty(), line, "component of a not compressed format is marked as compressed" );
      component.bits = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      component.name = attribute.second;
    }
    else if ( attribute.first == "numericFormat" )
    {
      component.numericFormat = attribute.second;
    }
    else if ( attribute.first == "planeIndex" )
    {
      component.planeIndex = attribute.second;
    }
  }
}

void VulkanHppGenerator::readFormatPlane( tinyxml2::XMLElement const * element, FormatData & formatData )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes(
    line, attributes, { { "compatible", {} }, { "index", { "0", "1", "2" } }, { "heightDivisor", { "1", "2" } }, { "widthDivisor", { "1", "2" } } }, {} );
  checkElements( line, getChildElements( element ), {} );

  formatData.planes.emplace_back();
  PlaneData & plane = formatData.planes.back();
  plane.xmlLine     = line;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "compatible" )
    {
      plane.compatible = attribute.second;
      auto formatIt    = m_enums.find( "VkFormat" );
      assert( formatIt != m_enums.end() );
      checkForError( contains( formatIt->second.values, plane.compatible ), line, "encountered unknown format <" + plane.compatible + ">" );
    }
    else if ( attribute.first == "index" )
    {
      const size_t index = std::stoi( attribute.second );
      checkForError( index + 1 == formatData.planes.size(), line, "unexpected index <" + attribute.second + ">" );
    }
    else if ( attribute.first == "heightDivisor" )
    {
      plane.heightDivisor = attribute.second;
    }
    else if ( attribute.first == "widthDivisor" )
    {
      plane.widthDivisor = attribute.second;
    }
  }
}

void VulkanHppGenerator::readFormats( tinyxml2::XMLElement const * element )
{
  const int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, { { "comment", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "format", false } } );

  for ( auto child : children )
  {
    readFormat( child );
  }
}

void VulkanHppGenerator::readFormatSPIRVImageFormat( tinyxml2::XMLElement const * element, FormatData & formatData )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name = attributes.find( "name" )->second;

  checkForError( formatData.spirvImageFormat.empty(), line, "spirvimageformat <" + name + "> already specified" );
  formatData.spirvImageFormat = name;
}

std::string VulkanHppGenerator::readName( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  return attributes.find( "name" )->second;
}

std::pair<VulkanHppGenerator::NameData, TypeInfo> VulkanHppGenerator::readNameAndType( tinyxml2::XMLElement const * element )
{
  int                                       line     = element->GetLineNum();
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "name", true } }, { { "enum" }, { "type" } } );

  NameData nameData;
  TypeInfo typeInfo;
  for ( auto child : children )
  {
    line = child->GetLineNum();
    checkAttributes( line, getAttributes( child ), {}, {} );
    checkElements( line, getChildElements( child ), {} );

    std::string value = child->Value();
    if ( value == "enum" )
    {
      nameData.arraySizes.push_back( child->GetText() );
      checkForError( child->PreviousSibling() && ( strcmp( child->PreviousSibling()->Value(), "[" ) == 0 ) && child->NextSibling() &&
                       ( strcmp( child->NextSibling()->Value(), "]" ) == 0 ),
                     line,
                     std::string( "array specifiation is ill-formatted: <" ) + nameData.arraySizes.back() + ">" );
      checkForError( m_constants.contains( nameData.arraySizes.back() ), line, "using unknown enum value <" + nameData.arraySizes.back() + ">" );
    }
    else if ( value == "name" )
    {
      nameData.name = child->GetText();
      std::string bitCount;
      std::tie( nameData.arraySizes, bitCount ) = readModifiers( child->NextSibling() );
      checkForError( bitCount.empty(), line, "name <" + nameData.name + "> with unsupported bitCount <" + bitCount + ">" );
    }
    else if ( value == "type" )
    {
      typeInfo = readTypeInfo( child );
    }
  }
  return std::make_pair( nameData, typeInfo );
}

void VulkanHppGenerator::readPlatform( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "comment", {} }, { "name", {} }, { "protect", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name, protect;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "protect" )
    {
      protect = attribute.second;
    }
  }

  checkForError( std::none_of( m_platforms.begin(),
                               m_platforms.end(),
                               [&protect]( std::pair<std::string, PlatformData> const & p ) { return p.second.protect == protect; } ),
                 line,
                 "protect <" + protect + "> of platform <" + name + "> is already used with some other platform" );
  checkForError( m_platforms.insert( { name, { protect, line } } ).second, line, "platform <" + name + "> is already specified" );
}

void VulkanHppGenerator::readPlatforms( tinyxml2::XMLElement const * element )
{
  const int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "platform", false } } );

  for ( auto child : children )
  {
    readPlatform( child );
  }
}

void VulkanHppGenerator::readRegistry( tinyxml2::XMLElement const * element )
{
  const int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line,
                 children,
                 { { "commands", false },
                   { "comment", false },
                   { "enums", false },
                   { "extensions", false },
                   { "feature", false },
                   { "formats", false },
                   { "platforms", true },
                   { "spirvcapabilities", true },
                   { "spirvextensions", true },
                   { "sync", true },
                   { "tags", true },
                   { "types", false } },
                 { "videocodecs" } );  // make this optional for now, make it required around October 2024
  for ( auto child : children )
  {
    const std::string value = child->Value();
    if ( value == "commands" )
    {
      readCommands( child );
    }
    else if ( value == "comment" )
    {
      std::string comment = readComment( child );
      if ( comment.find( "\nCopyright" ) == 0 )
      {
        m_vulkanLicenseHeader = generateCopyrightMessage( comment );
      }
    }
    else if ( value == "enums" )
    {
      readEnums( child );
    }
    else if ( value == "extensions" )
    {
      readExtensions( child );
    }
    else if ( value == "feature" )
    {
      readFeature( child );
    }
    else if ( value == "formats" )
    {
      readFormats( child );
    }
    else if ( value == "platforms" )
    {
      readPlatforms( child );
    }
    else if ( value == "spirvcapabilities" )
    {
      readSPIRVCapabilities( child );
    }
    else if ( value == "spirvextensions" )
    {
      readSPIRVExtensions( child );
    }
    else if ( value == "sync" )
    {
      readSync( child );
    }
    else if ( value == "tags" )
    {
      readTags( child );
    }
    else if ( value == "types" )
    {
      readTypes( child );
    }
    else if ( value == "videocodecs" )
    {
      readVideoCodecs( child );
    }
  }

  // after everything is read, distribute some information
  distributeEnumExtends();
  distributeEnumValueAliases();
  distributeStructAliases();
  distributeRequirements();
  markExtendedStructs();
}

VulkanHppGenerator::RemoveData VulkanHppGenerator::readRemoveData( tinyxml2::XMLElement const * element )
{
  const int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, { { "comment", {} }, { "reasonlink", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "command", "enum", "feature", "type" } );

  RemoveData removeData;
  removeData.xmlLine = line;
  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "command" )
    {
      removeData.commands.push_back( readName( child ) );
    }
    else if ( value == "enum" )
    {
      removeData.enums.push_back( readName( child ) );
    }
    else if ( value == "feature" )
    {
      removeData.features.push_back( readRequireFeature( child ) );
    }
    else if ( value == "type" )
    {
      removeData.types.push_back( readName( child ) );
    }
  }
  return removeData;
}

VulkanHppGenerator::NameLine VulkanHppGenerator::readRequireCommand( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, { { "comment", {} } } );
  checkElements( line, getChildElements( element ), {} );

  return { attributes.find( "name" )->second, line };
}

void VulkanHppGenerator::readRequireEnum(
  tinyxml2::XMLElement const * element, std::string const & requiredBy, std::string const & platform, bool supported, RequireData & requireData )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkElements( line, getChildElements( element ), {} );

  if ( attributes.contains( "alias" ) )
  {
    checkAttributes( line,
                     attributes,
                     { { "alias", {} }, { "name", {} } },
                     { { "api", { "vulkan", "vulkansc" } }, { "comment", {} }, { "deprecated", { "aliased" } }, { "extends", {} } } );

    std::string alias, api, extends, name;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "alias" )
      {
        alias = attribute.second;
      }
      else if ( attribute.first == "api" )
      {
        api = attribute.second;
      }
      else if ( attribute.first == "extends" )
      {
        extends = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        name = attribute.second;
      }
    }

    if ( extends.empty() )
    {
      // enum aliases that don't extend something are listed as constants
      auto typeIt = m_types.find( alias );
      checkForError( typeIt != m_types.end(), line, "enum alias <" + name + "> is an alias of an unknown enum <" + alias + ">" );
      checkForError( typeIt->second.category == TypeCategory::Constant, line, "enum alias <" + name + "> is an alias of a non-constant type <" + alias + ">" );
      checkForError(
        m_types.insert( { name, TypeData{ TypeCategory::Constant, { requiredBy }, line } } ).second, line, "required enum <" + name + "> already specified" );

      auto constIt = m_constants.find( alias );
      if ( constIt != m_constants.end() )
      {
        typeIt = m_types.find( name );
        typeIt->second.requiredBy.insert( requiredBy );
        m_constants[name] = { constIt->second.type, constIt->second.value, line };
        requireData.constants.push_back( name );
      }
    }
    else
    {
      auto extendIt = m_enumExtends.insert( { extends, {} } ).first;
      auto eedIt    = std::find_if( extendIt->second.begin(), extendIt->second.end(), [&name]( auto const & eed ) { return eed.name == name; } );
      if ( eedIt == extendIt->second.end() )
      {
        extendIt->second.push_back( { alias, api, name, platform, { requiredBy }, supported, line } );
      }
      else
      {
        checkForError( ( eedIt->alias == alias ) && ( eedIt->api == api ) && ( eedIt->platform == platform ) && ( eedIt->supported == supported ),
                       line,
                       "extending enum <" + extends + "> with already listed value <" + name + "> but different properties" );
        eedIt->requiredBy.insert( requiredBy );
      }
    }
  }
  else
  {
    checkAttributes( line,
                     attributes,
                     { { "name", {} } },
                     { { "api", { "vulkan", "vulkansc" } },
                       { "bitpos", {} },
                       { "comment", {} },
                       { "deprecated", { "true" } },
                       { "dir", { "-" } },
                       { "extends", {} },
                       { "extnumber", {} },
                       { "offset", {} },
                       { "protect", { "VK_ENABLE_BETA_EXTENSIONS" } },
                       { "type", { "uint32_t" } },
                       { "value", {} } } );

    std::string api, bitpos, extends, name, offset, protect, type, value;
    bool        deprecated = false;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "api" )
      {
        api = attribute.second;
      }
      else if ( attribute.first == "bitpos" )
      {
        bitpos = attribute.second;
      }
      else if ( attribute.first == "deprecated" )
      {
        deprecated = true;
      }
      else if ( attribute.first == "extends" )
      {
        extends = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        name = attribute.second;
      }
      else if ( attribute.first == "offset" )
      {
        offset = attribute.second;
      }
      else if ( attribute.first == "protect" )
      {
        protect = attribute.second;
      }
      else if ( attribute.first == "type" )
      {
        type = attribute.second;
      }
      else if ( attribute.first == "value" )
      {
        value = attribute.second;
      }
    }

    if ( extends.empty() )
    {
      if ( value.empty() )
      {
        auto typeIt = m_types.find( name );
        checkForError( typeIt != m_types.end(), line, "unknown required enum <" + name + ">" );
        typeIt->second.requiredBy.insert( requiredBy );
        requireData.constants.push_back( name );
      }
      else
      {
        if ( api.empty() || ( api == m_api ) )
        {
          checkForError( m_types.insert( { name, TypeData{ TypeCategory::Constant, { requiredBy }, line } } ).second,
                         line,
                         "required enum <" + name + "> specified by value <" + value + "> is already specified" );
          if ( type == "uint32_t" )
          {
            assert( !m_constants.contains( name ) );
            m_constants[name] = { type, value, line };
          }
        }

        checkForError(
          !supported || name.ends_with( "_EXTENSION_NAME" ) || name.ends_with( "_SPEC_VERSION" ), line, "encountered unexpected enum <" + name + ">" );

        requireData.enumConstants.emplace( name, value );
      }
    }
    else
    {
      checkForError( bitpos.empty() + offset.empty() + value.empty() == 2,
                     line,
                     "exactly one out of bitpos = <" + bitpos + ">, offset = <" + offset + ">, and value = <" + value + "> are supposed to be empty" );
      auto typeIt = m_types.find( extends );
      checkForError( typeIt != m_types.end(), line, "enum value <" + name + "> extends unknown type <" + extends + ">" );
      checkForError( typeIt->second.category == TypeCategory::Enum, line, "enum value <" + name + "> extends non-enum type <" + extends + ">" );
      typeIt->second.requiredBy.insert( requiredBy );
      auto enumIt = findByNameOrAlias( m_enums, extends );
      assert( enumIt != m_enums.end() );

      enumIt->second.addEnumValue( line,
                                   name,
                                   protect.empty() ? getProtectFromPlatform( platform ) : protect,
                                   bitpos + offset,
                                   value,
                                   ( api.empty() || ( api == m_api ) ) && supported,
                                   deprecated );
    }
  }
}

VulkanHppGenerator::RequireFeature VulkanHppGenerator::readRequireFeature( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} }, { "struct", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::vector<std::string> name;
  std::string              structure;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "struct" )
    {
      structure = attribute.second;
    }
  }
  assert( !name.empty() && !structure.empty() );

  auto structIt = m_structs.find( structure );
  if ( structIt == m_structs.end() )
  {
    auto aliasIt = std::find_if(
      m_structsAliases.begin(), m_structsAliases.end(), [&structure]( std::pair<std::string, NameLine> const & ad ) { return ad.first == structure; } );
    checkForError( aliasIt != m_structsAliases.end(), line, "encountered unknown required feature struct <" + structure + ">" );
    auto nextAliasIt = aliasIt;
    do
    {
      nextAliasIt = std::find_if( m_structsAliases.begin(),
                                  m_structsAliases.end(),
                                  [&structure = aliasIt->second.name]( std::pair<std::string, NameLine> const & ad ) { return ad.first == structure; } );
      if ( nextAliasIt != m_structsAliases.end() )
      {
        aliasIt = nextAliasIt;
      }
    } while ( nextAliasIt != m_structsAliases.end() );
    structIt = m_structs.find( aliasIt->second.name );
    assert( structIt != m_structs.end() );
  }
  for ( auto const & n : name )
  {
    auto memberIt = std::find_if( structIt->second.members.begin(), structIt->second.members.end(), [&n]( MemberData const & md ) { return md.name == n; } );
    checkForError(
      memberIt != structIt->second.members.end(), line, "required feature name <" + n + "> not part of the required feature struct <" + structure + ">" );
    checkForError( ( memberIt->type.isValue() && ( memberIt->type.type == "VkBool32" ) ),
                   line,
                   "required feature name <" + n + "> is not a VkBool32 member of the required feature struct <" + structure + ">" );
  }

  return { name, structure, line };
}

VulkanHppGenerator::NameLine VulkanHppGenerator::readRequireType( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, { { "comment", {} } } );
  checkElements( line, getChildElements( element ), {} );

  return { attributes.find( "name" )->second, line };
}

void VulkanHppGenerator::readSPIRVCapability( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "enable" } );

  std::string name              = attributes.find( "name" )->second;
  auto [capabilityIt, inserted] = m_spirVCapabilities.insert( { name, { {}, line } } );
  checkForError( inserted, line, "spirvcapability <" + name + "> already listed" );

  for ( auto child : children )
  {
    readSPIRVCapabilityEnable( child, capabilityIt->second );
  }
}

void VulkanHppGenerator::readSPIRVCapabilityEnable( tinyxml2::XMLElement const * element, SpirVCapabilityData & capability )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkElements( line, getChildElements( element ), {}, {} );

  if ( attributes.contains( "extension" ) )
  {
    checkAttributes( line, attributes, { { "extension", {} } }, {} );

    std::string const & extension = attributes.find( "extension" )->second;
    checkForError( isExtension( extension ), line, "unknown extension <" + extension + "> specified for SPIR-V capability" );
  }
  else if ( attributes.contains( "property" ) )
  {
    checkAttributes( line, attributes, { { "member", {} }, { "property", {} }, { "requires", {} }, { "value", {} } }, {} );

    std::string member, property, value;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "member" )
      {
        member = attribute.second;
      }
      else if ( attribute.first == "property" )
      {
        property = attribute.second;
      }
      if ( attribute.first == "requires" )
      {
        std::vector<std::string> require = tokenize( attribute.second, "," );
        for ( auto const & r : require )
        {
          checkForError( isFeature( r ) || isExtension( r ), line, "unknown requires <" + r + "> specified for SPIR-V capability" );
        }
      }
      else if ( attribute.first == "value" )
      {
        value = attribute.second;
      }
    }

    auto propertyIt = m_structs.find( property );
    checkForError( propertyIt != m_structs.end(), line, "unknown property <" + property + "> specified for SPIR-V capability" );
    auto memberIt = findStructMemberIt( member, propertyIt->second.members );
    checkForError( memberIt != propertyIt->second.members.end(), line, "unknown member <" + member + "> specified for SPIR-V capability" );
    if ( memberIt->type.type == "VkBool32" )
    {
      checkForError( ( value == "VK_FALSE" ) || ( value == "VK_TRUE" ),
                     line,
                     "unknown value <" + value + "> for boolean member <" + member + "> specified for SPIR-V capability" );
    }
    else
    {
      auto bitmaskIt = m_bitmasks.find( memberIt->type.type );
      checkForError( bitmaskIt != m_bitmasks.end(), line, "member <" + member + "> specified for SPIR-V capability is not a bitmask" );
      checkForError( !bitmaskIt->second.require.empty(), line, "member <" + member + "> specified for SPIR-V capability has no required enum" );
      const auto enumIt = m_enums.find( bitmaskIt->second.require );
      checkForError(
        enumIt != m_enums.end(), line, "member <" + member + "> specified for SPIR-V capability requires an unknown enum <" + bitmaskIt->second.require + ">" );
      checkForError( contains( enumIt->second.values, value ), line, "unknown attribute value <" + value + "> specified for SPIR-V capability" );
    }
  }
  else if ( attributes.contains( "struct" ) )
  {
    checkAttributes( line, attributes, { { "feature", {} }, { "struct", {} } }, { { "alias", {} }, { "requires", {} } } );

    std::string feature, structure;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "feature" )
      {
        feature = attribute.second;
      }
      else if ( attribute.first == "requires" )
      {
        std::vector<std::string> require = tokenize( attribute.second, "," );
        for ( auto const & r : require )
        {
          checkForError( isFeature( r ) || isExtension( r ), line, "unknown requires <" + r + "> specified for SPIR-V capability" );
        }
      }
      else if ( attribute.first == "struct" )
      {
        structure = attribute.second;
      }
    }

    auto structIt = capability.structs.insert( { structure, {} } ).first;
    checkForError( structIt->second.insert( { feature, line } ).second,
                   line,
                   "feature <" + feature + "> already specified for struct <" + structure + "> for SPIR-V capability" );
  }
  else if ( attributes.contains( "version" ) )
  {
    checkAttributes( line, attributes, { { "version", {} } }, {} );

    std::string version = attributes.find( "version" )->second;
    if ( version.starts_with( "VK_API_" ) )
    {
      version.erase( 3, 4 );  // remove "API_" from the version -> VK_VERSION_x_y
    }
    checkForError( isFeature( version ), line, "unknown version <" + version + "> specified for SPIR-V capability" );
  }
}

void VulkanHppGenerator::readSPIRVCapabilities( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "spirvcapability" } );

  for ( auto child : children )
  {
    readSPIRVCapability( child );
  }
}

void VulkanHppGenerator::readSPIRVExtension( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "enable" } );

  for ( auto child : children )
  {
    readSPIRVExtensionEnable( child );
  }
}

void VulkanHppGenerator::readSPIRVExtensionEnable( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, { { "extension", {} }, { "version", {} } } );
  checkElements( line, getChildElements( element ), {}, {} );

  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "extension" )
    {
      checkForError( isExtension( attribute.second ), line, "unknown extension <" + attribute.second + "> specified for SPIR-V extension" );
    }
    else
    {
      assert( attribute.first == "version" );
      std::string feature = attribute.second;
      if ( feature.starts_with( "VK_API_" ) )
      {
        feature.erase( 3, 4 );  // remove "API_" from the version -> VK_VERSION_x_y
      }
      checkForError( isFeature( feature ), line, "unknown version <" + attribute.second + "> specified for SPIR-V extension" );
    }
  }
}

void VulkanHppGenerator::readSPIRVExtensions( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "spirvextension" } );

  for ( auto child : children )
  {
    readSPIRVExtension( child );
  }
}

void VulkanHppGenerator::readStructMember( tinyxml2::XMLElement const * element, std::vector<MemberData> & members, bool isUnion )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line,
                   attributes,
                   {},
                   { { "altlen", {} },
                     { "api", { "vulkan", "vulkansc" } },
                     { "deprecated", { "ignored" } },
                     { "externsync", { "true" } },
                     { "featurelink", {} },
                     { "len", {} },
                     { "limittype", { "bitmask", "bits", "exact", "max", "min", "mul", "noauto", "not", "pot", "range", "struct" } },
                     { "noautovalidity", { "true" } },
                     { "objecttype", { "objectType" } },
                     { "optional", { "false", "true" } },
                     { "selection", {} },
                     { "selector", {} },
                     { "values", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "name", true }, { "type", true } }, { "comment", "enum" } );

  MemberData memberData;
  memberData.xmlLine = line;

  std::string api;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "api" )
    {
      api = attribute.second;
    }
    else if ( attribute.first == "altlen" )
    {
      assert( memberData.lenExpressions.empty() );
      memberData.lenExpressions = tokenize( attribute.second, "," );
      checkForError( memberData.lenExpressions.size() == 1,
                     line,
                     "member attribute <altlen> holds unknown number of data: " + std::to_string( memberData.lenExpressions.size() ) );
      memberData.lenMembers = filterNumbers( tokenizeAny( attribute.second, " /()+*" ) );
    }
    else if ( attribute.second == "deprecated" )
    {
      assert( false );
      // the struct member is marked as deprecated/ignored, but still exisits -> no modifications needed here
    }
    else if ( attribute.first == "len" )
    {
      if ( memberData.lenExpressions.empty() )
      {
        memberData.lenExpressions = tokenize( attribute.second, "," );
        checkForError( !memberData.lenExpressions.empty() && ( memberData.lenExpressions.size() <= 2 ),
                       line,
                       "member attribute <len> holds unknown number of data: " + std::to_string( memberData.lenExpressions.size() ) );
        if ( memberData.lenExpressions[0] != "null-terminated" )
        {
          auto lenMemberIt = findStructMemberIt( memberData.lenExpressions[0], members );
          checkForError( lenMemberIt != members.end(), line, "member attribute <len> holds unknown value <" + memberData.lenExpressions[0] + ">" );
          memberData.lenMembers.push_back( { memberData.lenExpressions[0], std::distance( members.cbegin(), lenMemberIt ) } );
        }
        if ( 1 < memberData.lenExpressions.size() )
        {
          checkForError( ( memberData.lenExpressions[1] == "1" ) || ( memberData.lenExpressions[1] == "null-terminated" ),
                         line,
                         "member attribute <len> holds unknown second value <" + memberData.lenExpressions[1] + ">" );
        }
      }
    }
    else if ( attribute.first == "noautovalidity" )
    {
      memberData.noAutoValidity = ( attribute.second == "true" );
    }
    else if ( attribute.first == "optional" )
    {
      std::vector<std::string> optional = tokenize( attribute.second, "," );
      memberData.optional.reserve( optional.size() );
      for ( auto const & o : optional )
      {
        memberData.optional.push_back( o == "true" );
      }
    }
    else if ( attribute.first == "selection" )
    {
      checkForError( isUnion, line, "attribute <selection> is used with a non-union structure." );
      memberData.selection = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "selector" )
    {
      memberData.selector = attribute.second;
      auto selectorIt     = findStructMemberIt( memberData.selector, members );
      checkForError( selectorIt != members.end(), line, "member attribute <selector> holds unknown value <" + memberData.selector + ">" );
      checkForError(
        m_enums.contains( selectorIt->type.type ), line, "member attribute <selector> references unknown enum type <" + selectorIt->type.type + ">" );
    }
    else if ( attribute.first == "values" )
    {
      std::vector<std::string> values = tokenize( attribute.second, "," );
      checkForError( values.size() == 1, line, "attribute \"values\" holds multiple values <" + attribute.first + ">, but it's expected to hold just one" );
      memberData.value = values[0];
    }
  }

  std::string name;
  for ( auto child : children )
  {
    const int childLine = child->GetLineNum();
    checkAttributes( childLine, getAttributes( child ), {}, {} );
    checkElements( childLine, getChildElements( child ), {}, {} );

    std::string value = child->Value();
    if ( value == "enum" )
    {
      std::string enumString = child->GetText();

      checkForError( child->PreviousSibling() && ( strcmp( child->PreviousSibling()->Value(), "[" ) == 0 ) && child->NextSibling() &&
                       ( strcmp( child->NextSibling()->Value(), "]" ) == 0 ),
                     line,
                     std::string( "struct member array specifiation is ill-formatted: <" ) + enumString + ">" );

      memberData.arraySizes.push_back( enumString );
    }
    else if ( value == "name" )
    {
      name                                                   = child->GetText();
      std::tie( memberData.arraySizes, memberData.bitCount ) = readModifiers( child->NextSibling() );
    }
    else if ( value == "type" )
    {
      memberData.type = readTypeInfo( child );
    }
  }
  assert( !name.empty() );

  if ( api.empty() || ( api == m_api ) )
  {
    checkForError( std::none_of( members.begin(), members.end(), [&name]( MemberData const & md ) { return md.name == name; } ),
                   line,
                   "struct member name <" + name + "> already used" );
    memberData.name = name;
    members.push_back( memberData );
  }
}

void VulkanHppGenerator::readSync( tinyxml2::XMLElement const * element )
{
  const int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "syncaccess", false }, { "syncpipeline", false }, { "syncstage", false } }, {} );

  auto accessFlagBits2It = m_enums.find( "VkAccessFlagBits2" );
  assert( accessFlagBits2It != m_enums.end() );
  auto stageFlagBits2It = m_enums.find( "VkPipelineStageFlagBits2" );
  assert( stageFlagBits2It != m_enums.end() );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "syncaccess" )
    {
      readSyncAccess( child, accessFlagBits2It );
    }
    else if ( value == "syncpipeline" )
    {
      readSyncPipeline( child );
    }
    else
    {
      assert( value == "syncstage" );
      readSyncStage( child, stageFlagBits2It );
    }
  }
}

void VulkanHppGenerator::readSyncAccess( tinyxml2::XMLElement const * element, std::map<std::string, EnumData>::const_iterator accessFlagBits2It )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, { { "alias", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "comment", "syncequivalent", "syncsupport" } );

  std::string alias, name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      alias = attribute.second;
    }
    else
    {
      assert( attribute.first == "name" );
      name = attribute.second;
    }
  }

  checkForError( m_syncAccesses.insert( { name, { alias, line } } ).second, line, "syncaccess <" + name + "> already listed" );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "syncequivalent" )
    {
      readSyncAccessEquivalent( child, accessFlagBits2It );
    }
    else if ( value == "syncsupport" )
    {
      readSyncAccessSupport( child );
    }
  }
}

void VulkanHppGenerator::readSyncAccessEquivalent( tinyxml2::XMLElement const * element, std::map<std::string, EnumData>::const_iterator accessFlagBits2It )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "access", {} } }, {} );
  checkElements( line, getChildElements( element ), {}, {} );

  for ( auto const & attribute : attributes )
  {
    assert( attribute.first == "access" );
    std::vector<std::string> access = tokenize( attribute.second, "," );
    for ( auto const & a : access )
    {
      checkForError( contains( accessFlagBits2It->second.values, a ), line, "syncequivalent access uses unknown value <" + a + ">!" );
    }
  }
}

void VulkanHppGenerator::readSyncAccessSupport( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "stage", {} } }, {} );
  checkElements( line, getChildElements( element ), {}, {} );

  std::vector<std::string> stages = tokenize( attributes.find( "stage" )->second, "," );
  for ( auto const & stage : stages )
  {
    checkForError( m_syncStages.contains( stage ), line, "syncsupport uses unknown stage <" + stage + ">!" );
  }
}

void VulkanHppGenerator::readSyncPipeline( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, { { "depends", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "syncpipelinestage", false } }, {} );

  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "depends" )
    {
      std::vector<std::string> depends = tokenize( attribute.second, "," );
      for ( auto const & d : depends )
      {
        checkForError( isExtension( d ), line, "syncpipeline depends on unknown extension <" + d + ">" );
      }
    }
  }
}

void VulkanHppGenerator::readSyncStage( tinyxml2::XMLElement const * element, std::map<std::string, EnumData>::const_iterator stageFlagBits2It )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, { { "alias", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "syncequivalent", "syncsupport" } );

  std::string alias, name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      alias = attribute.second;
    }
    else
    {
      assert( attribute.first == "name" );
      name = attribute.second;
    }
  }

  checkForError( m_syncStages.insert( { name, { alias, line } } ).second, line, "syncstage <" + name + "> already listed" );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "syncequivalent" )
    {
      readSyncStageEquivalent( child, stageFlagBits2It );
    }
    else
    {
      assert( value == "syncsupport" );
      readSyncStageSupport( child );
    }
  }
}

void VulkanHppGenerator::readSyncStageEquivalent( tinyxml2::XMLElement const * element, std::map<std::string, EnumData>::const_iterator stageFlagBits2It )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "stage", {} } }, {} );
  checkElements( line, getChildElements( element ), {}, {} );

  for ( auto const & attribute : attributes )
  {
    assert( attribute.first == "stage" );
    std::vector<std::string> stage = tokenize( attribute.second, "," );
    for ( auto const & s : stage )
    {
      checkForError( contains( stageFlagBits2It->second.values, s ), line, "syncequivalent stage uses unknown value <" + s + ">!" );
    }
  }
}

void VulkanHppGenerator::readSyncStageSupport( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "queues", {} } }, {} );
  checkElements( line, getChildElements( element ), {}, {} );

  for ( auto const & attribute : attributes )
  {
    assert( attribute.first == "queues" );
    checkForError( m_commandQueues.contains( attribute.second ), line, "syncsupport queues uses unknown value <" + attribute.second + ">" );
  }
}

void VulkanHppGenerator::readTag( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "author", {} }, { "contact", {} }, { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name = attributes.find( "name" )->second;
  checkForError( m_tags.insert( { name, { line } } ).second, line, "tag <" + name + "> already specified" );
}

void VulkanHppGenerator::readTags( tinyxml2::XMLElement const * element )
{
  const int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "tag", false } } );

  for ( auto child : children )
  {
    readTag( child );
  }
}

void VulkanHppGenerator::readTypeBasetype( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  const int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "basetype" } } }, {} );

  NameData nameData;
  TypeInfo typeInfo;
  std::tie( nameData, typeInfo ) = readNameAndType( element );

  if ( typeInfo.prefix == "typedef" )
  {
    // remove redundant typeInfo.prefix "typedef"
    typeInfo.prefix.clear();
  }

  checkForError( nameData.arraySizes.empty(), line, "name <" + nameData.name + "> with unsupported arraySizes" );
  checkForError( typeInfo.prefix.empty(), line, "unexpected type prefix <" + typeInfo.prefix + ">" );
  checkForError( typeInfo.postfix.empty() || ( typeInfo.postfix == "*" ), line, "unexpected type postfix <" + typeInfo.postfix + ">" );

  checkForError(
    m_types.insert( { nameData.name, TypeData{ TypeCategory::BaseType, {}, line } } ).second, line, "basetype <" + nameData.name + "> already specified" );
  assert( !m_baseTypes.contains( nameData.name ) );
  m_baseTypes[nameData.name] = { typeInfo, line };
}

void VulkanHppGenerator::readTypeBitmask( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  const int line = element->GetLineNum();

  if ( attributes.contains( "alias" ) )
  {
    checkAttributes( line, attributes, { { "alias", {} }, { "category", { "bitmask" } }, { "name", {} } }, {} );
    checkElements( line, getChildElements( element ), {} );

    std::string alias, name;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "alias" )
      {
        alias = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        name = attribute.second;
      }
    }

    checkForError( m_types.insert( { name, TypeData{ TypeCategory::Bitmask, {}, line } } ).second, line, "bitmask <" + name + "> already specified" );

    auto bitmaskIt = m_bitmasks.find( alias );
    checkForError( bitmaskIt != m_bitmasks.end(), line, "bitmask <" + name + "> is an alias of an unknown bitmask <" + alias + ">." );
    checkForError( bitmaskIt->second.aliases.insert( { name, line } ).second,
                   line,
                   "bitmask alias <" + name + "> is already listed as an alias for bitmask <" + alias + ">" );
  }
  else
  {
    checkAttributes( line, attributes, { { "category", { "bitmask" } } }, { { "api", { "vulkan", "vulkansc" } }, { "bitvalues", {} }, { "requires", {} } } );

    std::string api, bitvalues, require;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "api" )
      {
        api = attribute.second;
      }
      else if ( attribute.first == "bitvalues" )
      {
        bitvalues = attribute.second;
      }
      else if ( attribute.first == "requires" )
      {
        require = attribute.second;
      }
    }

    NameData nameData;
    TypeInfo typeInfo;
    std::tie( nameData, typeInfo ) = readNameAndType( element );

    checkForError( nameData.name.starts_with( "Vk" ), line, "name <" + nameData.name + "> does not begin with <Vk>" );
    checkForError( nameData.arraySizes.empty(), line, "name <" + nameData.name + "> with unsupported arraySizes" );
    checkForWarning( ( typeInfo.type == "VkFlags" ) || ( typeInfo.type == "VkFlags64" ), line, "unexpected bitmask type <" + typeInfo.type + ">" );
    checkForError( typeInfo.prefix == "typedef", line, "unexpected type prefix <" + typeInfo.prefix + ">" );
    checkForError( typeInfo.postfix.empty(), line, "unexpected type postfix <" + typeInfo.postfix + ">" );
    checkForError( bitvalues.empty() || require.empty(), line, "attributes <bitvalues> and <requires> are both specified" );
    checkForError( ( typeInfo.type != "VkFlags64" ) || !bitvalues.empty(), line, "bitmask of type <VkFlags64> needs attribute bitvalues to be set" );

    if ( !bitvalues.empty() )
    {
      require = bitvalues;
    }

    if ( api.empty() || ( api == m_api ) )
    {
      checkForError(
        m_types.insert( { nameData.name, TypeData{ TypeCategory::Bitmask, {}, line } } ).second, line, "bitmask <" + nameData.name + "> already specified" );
      assert( !m_bitmasks.contains( nameData.name ) );
      m_bitmasks[nameData.name] = { {}, require, typeInfo.type, line };
    }
  }
}

VulkanHppGenerator::DefinesPartition VulkanHppGenerator::partitionDefines( std::map<std::string, DefineData> const & defines )
{
  DefinesPartition partition{};
  for ( auto const & define : defines )
  {
    // VK_DEFINE_HANDLE is macro magic that cannot be constexpr-ed
    // Also filter out the VKSC_ macros, as although they are in the spec, they are not defined in any header.
    if ( define.first.starts_with( "VK_" ) && ( define.first != "VK_DEFINE_HANDLE" ) )
    {
      if ( define.second.possibleCallee.empty() )
      {
        if ( define.second.possibleDefinition.empty() )
        {
#if !defined( NDEBUG )
          const std::set<std::string> ignoredDefines{ "VK_DEFINE_NON_DISPATCHABLE_HANDLE", "VK_NULL_HANDLE", "VK_USE_64_BIT_PTR_DEFINES" };
#endif
          assert( ignoredDefines.contains( define.first ) );
        }
        else
        {
          if ( define.second.params.empty() )
          {
            partition.values.insert( define );
          }
          else
          {
            partition.callees.insert( define );
          }
        }
      }
      else
      {
        assert( !define.second.params.empty() && define.second.possibleDefinition.empty() );
        partition.callers.insert( define );
      }
    }
  }
  return partition;
}

void VulkanHppGenerator::readTypeDefine( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  const int line = element->GetLineNum();
  checkAttributes( line,
                   attributes,
                   { { "category", { "define" } } },
                   { { "api", { "vulkan", "vulkansc" } }, { "comment", {} }, { "deprecated", { "true" } }, { "name", {} }, { "requires", {} } } );

  std::string api, name, require;
  bool        deprecated = false;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "api" )
    {
      api = attribute.second;
    }
    else if ( attribute.first == "deprecated" )
    {
      assert( attribute.second == "true" );
      deprecated = true;
    }
    else if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "requires" )
    {
      require = attribute.second;
    }
  }

  if ( !name.empty() )
  {
    checkForError( !element->FirstChildElement(), line, "unknown formatting of type category=define name <" + name + ">" );
    checkForError( element->LastChild() && element->LastChild()->ToText() && element->LastChild()->ToText()->Value(),
                   line,
                   "unknown formatting of type category=define named <" + name + ">" );
  }
  else if ( element->GetText() )
  {
    std::string text = element->GetText();
    if ( ( text.find( "class" ) != std::string::npos ) || ( text.find( "struct" ) != std::string::npos ) )
    {
      // here are a couple of structs as defines, which really are types!
      assert( false );
      tinyxml2::XMLElement const * child = element->FirstChildElement();
      checkForError( child && ( strcmp( child->Value(), "name" ) == 0 ) && child->GetText(), line, "unexpected formatting of type category=define" );
      name = child->GetText();
      checkForError(
        m_types.insert( std::make_pair( name, TypeData{ .category = TypeCategory::Define } ) ).second, line, "type <" + name + "> has already been speficied" );
    }
    else
    {
      tinyxml2::XMLElement const * child = element->FirstChildElement();
      checkForError( child && !child->FirstAttribute() && ( strcmp( child->Value(), "name" ) == 0 ) && child->GetText(),
                     line,
                     "unknown formatting of type category define" );
      name = trim( child->GetText() );
      if ( ( name == "VK_HEADER_VERSION" ) && ( api.empty() || ( api == m_api ) ) )
      {
        m_version = trimEnd( element->LastChild()->ToText()->Value() );
      }
      // ignore all the other defines
      checkForWarning( !child->NextSiblingElement() ||
                         ( child->NextSiblingElement() && !child->NextSiblingElement()->FirstAttribute() &&
                           ( strcmp( child->NextSiblingElement()->Value(), "type" ) == 0 ) && !child->NextSiblingElement()->NextSiblingElement() ),
                       line,
                       "unknown formatting of type category define" );
    }
  }
  assert( !name.empty() );

  if ( api.empty() || ( api == m_api ) )
  {
    MacroVisitor definesVisitor{};
    element->Accept( &definesVisitor );
    auto const & [deprecatedReason, possibleCallee, params, possibleDefinition] = parseMacro( definesVisitor.macro );

    checkForError( m_types.insert( { name, TypeData{ TypeCategory::Define, {}, line } } ).second, line, "define <" + name + "> already specified" );
    assert( !m_defines.contains( name ) );
    m_defines[name] = { deprecated, require, line, deprecatedReason, possibleCallee, params, possibleDefinition };
  }
}

void VulkanHppGenerator::readTypeEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  const int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "enum" } }, { "name", {} } }, { { "alias", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string alias, name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      alias = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
  }

  checkForError( m_types.insert( { name, TypeData{ TypeCategory::Enum, {}, line } } ).second, line, "enum <" + name + "> already specified" );
  if ( alias.empty() )
  {
    assert( !m_enums.contains( name ) );
    m_enums[name] = EnumData{ .xmlLine = line };
  }
  else
  {
    auto enumIt = m_enums.find( alias );
    checkForError( enumIt != m_enums.end(), line, "enum <" + name + "> is an alias of an unknown enum <" + alias + ">" );
    checkForError( enumIt->second.aliases.insert( { name, line } ).second, line, "enum <" + name + "> already specified as some alias" );
  }
}

void VulkanHppGenerator::readTypeFuncpointer( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  const int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "funcpointer" } } }, { { "requires", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "name", true } }, { "type" } );

  std::string text = element->GetText();
  assert( text.starts_with( "typedef " ) && text.ends_with( " (VKAPI_PTR *" ) );
  text = stripPostfix( stripPrefix( text, "typedef " ), " (VKAPI_PTR *" );
  TypeInfo returnType;
  if ( text.ends_with( "*" ) )
  {
    returnType.postfix = "*";
    text               = stripPostfix( text, "*" );
  }
  returnType.type = text;

  std::string require;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "requires" )
    {
      require = attribute.second;
    }
  }

  std::string                          name;
  std::vector<FuncPointerArgumentData> arguments;
  for ( auto const & child : children )
  {
    std::string value = child->Value();
    if ( value == "name" )
    {
      name = child->GetText();
    }
    else if ( value == "type" )
    {
      const int argumentLine = child->GetLineNum();
      TypeInfo  typeInfo;
      typeInfo.type = child->GetText();

      auto previousSibling = child->PreviousSibling();
      assert( previousSibling );
      char const * previousSiblingValue = previousSibling->Value();
      assert( previousSiblingValue );
      std::string previousSiblingText = previousSiblingValue;
      if ( previousSiblingText.ends_with( "const " ) )
      {
        typeInfo.prefix = "const";
      }

      auto nextSibling = child->NextSibling();
      assert( nextSibling );
      char const * nextSiblingValue = nextSibling->Value();
      assert( nextSiblingValue != nullptr );
      std::string argumentName = nextSiblingValue;
      if ( argumentName.front() == '*' )
      {
        typeInfo.postfix = "*";
      }
      argumentName = argumentName.substr( argumentName.find_first_not_of( "* " ) );
      argumentName = argumentName.substr( 0, argumentName.find_first_of( ",)" ) );

      checkForError( std::none_of( arguments.begin(),
                                   arguments.end(),
                                   [&argumentName]( FuncPointerArgumentData const & argument ) { return argument.name == argumentName; } ),
                     line,
                     "argument <" + argumentName + "> already listed in funcpointer <" + name + ">" );
      arguments.push_back( { argumentName, typeInfo, argumentLine } );
    }
  }
  assert( !name.empty() );

  std::set<std::string> argumentNames;
  checkForError( m_types.insert( { name, TypeData{ TypeCategory::FuncPointer, {}, line } } ).second, line, "funcpointer <" + name + "> already specified" );
  assert( !m_funcPointers.contains( name ) );
  m_funcPointers[name] = { arguments, require, returnType, line };
}

void VulkanHppGenerator::readTypeHandle( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  const int line = element->GetLineNum();

  auto aliasIt = attributes.find( "alias" );
  if ( aliasIt != attributes.end() )
  {
    checkAttributes( line, attributes, { { "alias", {} }, { "category", { "handle" } }, { "name", {} } }, {} );
    checkElements( line, getChildElements( element ), {} );

    std::string alias = aliasIt->second;
    std::string name  = attributes.find( "name" )->second;

    checkForError( m_types.insert( { name, TypeData{ TypeCategory::Handle, {}, line } } ).second, line, "handle <" + name + "> already specified" );

    auto handleIt = m_handles.find( alias );
    checkForError( handleIt != m_handles.end(), line, "handle <" + name + "> uses unknown alias <" + alias + ">." );
    checkForError( handleIt->second.aliases.insert( { name, line } ).second, line, "handle <" + name + "> already listed as alias for handle <" + alias + ">" );
  }
  else
  {
    checkAttributes( line, attributes, { { "category", { "handle" } }, { "objtypeenum", {} } }, { { "parent", {} } } );

    HandleData handleData;
    handleData.xmlLine = line;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "objtypeenum" )
      {
        handleData.objTypeEnum = attribute.second;
      }
      else if ( attribute.first == "parent" )
      {
        handleData.parent = attribute.second;
      }
    }

    NameData nameData;
    TypeInfo typeInfo;
    std::tie( nameData, typeInfo ) = readNameAndType( element );
    handleData.isDispatchable      = typeInfo.type == "VK_DEFINE_HANDLE";

    checkForError( nameData.name.starts_with( "Vk" ), line, "name <" + nameData.name + "> does not begin with <Vk>" );
    checkForError( nameData.arraySizes.empty(), line, "name <" + nameData.name + "> with unsupported arraySizes" );
    checkForError( ( typeInfo.type == "VK_DEFINE_HANDLE" ) || ( typeInfo.type == "VK_DEFINE_NON_DISPATCHABLE_HANDLE" ),
                   line,
                   "handle with invalid type <" + typeInfo.type + ">" );
    checkForError( typeInfo.prefix.empty(), line, "unexpected type prefix <" + typeInfo.prefix + ">" );
    checkForError( typeInfo.postfix == "(", line, "unexpected type postfix <" + typeInfo.postfix + ">" );
    checkForError( !handleData.objTypeEnum.empty(), line, "handle <" + nameData.name + "> does not specify attribute \"objtypeenum\"" );

    checkForError(
      m_types.insert( { nameData.name, TypeData{ TypeCategory::Handle, {}, line } } ).second, line, "handle <" + nameData.name + "> already specified" );
    assert( !m_handles.contains( nameData.name ) );
    m_handles.insert( { nameData.name, handleData } );
  }
}

void VulkanHppGenerator::readTypeInclude( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  const int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "include" } }, { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name = attributes.find( "name" )->second;
  checkForError( m_types.insert( { name, TypeData{ TypeCategory::Include, {}, line } } ).second, line, "type <" + name + "> already specified" );
  assert( !m_includes.contains( name ) );
  m_includes[name] = { line };
}

void VulkanHppGenerator::readTypeRequires( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  const int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "name", {} }, { "requires", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name, require;
  for ( auto attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else
    {
      assert( attribute.first == "requires" );
      require = attribute.second;
    }
  }

  checkForError( m_includes.contains( require ), line, "type <" + name + "> requires unknown <" + require + ">" );
  checkForError( m_types.insert( { name, TypeData{ TypeCategory::ExternalType, {}, line } } ).second, line, "type <" + name + "> already specified" );
  assert( !m_externalTypes.contains( name ) );
  m_externalTypes[name] = { require, line };
}

void VulkanHppGenerator::readTypeStruct( tinyxml2::XMLElement const * element, bool isUnion, std::map<std::string, std::string> const & attributes )
{
  const int line    = element->GetLineNum();
  auto      aliasIt = attributes.find( "alias" );
  if ( aliasIt != attributes.end() )
  {
    checkAttributes( line, attributes, { { "alias", {} }, { "category", { "struct" } }, { "name", {} } }, {} );
    checkElements( line, getChildElements( element ), {}, {} );

    std::string alias = aliasIt->second;
    std::string name  = attributes.find( "name" )->second;

    checkForError( m_types.insert( { name, TypeData{ TypeCategory::Struct, {}, line } } ).second, line, "struct <" + name + "> already specified" );
    checkForError(
      std::none_of( m_structsAliases.begin(), m_structsAliases.end(), [&name]( std::pair<std::string, NameLine> const & ad ) { return ad.first == name; } ),
      line,
      "struct alias <" + name + "> already listed" );
    m_structsAliases.push_back( { name, { alias, line } } );
  }
  else
  {
    checkAttributes( line,
                     attributes,
                     { { "category", { isUnion ? "union" : "struct" } }, { "name", {} } },
                     { { "allowduplicate", { "false", "true" } }, { "comment", {} }, { "returnedonly", { "true" } }, { "structextends", {} } } );
    std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
    checkElements( line, children, {}, { "member", "comment" } );

    StructureData structureData;
    structureData.xmlLine = line;
    structureData.isUnion = isUnion;

    std::string name;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "allowduplicate" )
      {
        structureData.allowDuplicate = ( attribute.second == "true" );
      }
      else if ( attribute.first == "category" )
      {
        assert( isUnion ? ( attribute.second == "union" ) : ( attribute.second == "struct" ) );
      }
      else if ( attribute.first == "name" )
      {
        name = attribute.second;
      }
      else if ( attribute.first == "returnedonly" )
      {
        assert( attribute.second == "true" );
        structureData.returnedOnly = true;
      }
      else if ( attribute.first == "structextends" )
      {
        structureData.structExtends = tokenize( attribute.second, "," );
      }
    }
    assert( !name.empty() );

    checkForError( !structureData.allowDuplicate || !structureData.structExtends.empty(),
                   line,
                   "attribute <allowduplicate> is true, but no structures are listed in <structextends>" );

    checkForError( m_types.insert( { name, TypeData{ isUnion ? TypeCategory::Union : TypeCategory::Struct, {}, line } } ).second,
                   line,
                   "struct <" + name + "> already specified" );
    assert( !m_structs.contains( name ) );

    std::map<std::string, StructureData>::iterator it = m_structs.insert( std::make_pair( name, structureData ) ).first;

    for ( auto child : children )
    {
      std::string value = child->Value();
      if ( value == "member" )
      {
        readStructMember( child, it->second.members, isUnion );
      }
    }
    it->second.subStruct = determineSubStruct( *it );

    // add some default values for some structures here!
    if ( ( it->first == "VkRayTracingShaderGroupCreateInfoNV" ) || ( it->first == "VkRayTracingShaderGroupCreateInfoKHR" ) )
    {
      assert( ( ( it->first != "VkRayTracingShaderGroupCreateInfoNV" ) || ( it->second.members.size() == 7 ) ) &&
              ( ( it->first != "VkRayTracingShaderGroupCreateInfoKHR" ) || ( it->second.members.size() == 8 ) ) );
      assert( ( it->second.members[3].name == "generalShader" ) && ( it->second.members[4].name == "closestHitShader" ) &&
              ( it->second.members[5].name == "anyHitShader" ) && ( it->second.members[6].name == "intersectionShader" ) );
      const bool isKHR = ( it->first == "VkRayTracingShaderGroupCreateInfoKHR" );
      for ( const size_t i : { 3, 4, 5, 6 } )
      {
        it->second.members[i].defaultValue = isKHR ? "VK_SHADER_UNUSED_KHR" : "VK_SHADER_UNUSED_NV";
      }
    }

    // check if multiple structure members use the very same (not empty) len attribute
    // Note: even though the arrays are not marked as optional, they still might be mutually exclusive (like in
    // VkWriteDescriptorSet)! That is, there's not enough information available in vk.xml to decide on that, so we
    // need this external knowledge!
    static std::set<std::string> mutualExclusiveStructs = {
      "VkAccelerationStructureBuildGeometryInfoKHR", "VkAccelerationStructureTrianglesOpacityMicromapEXT", "VkMicromapBuildInfoEXT", "VkWriteDescriptorSet"
    };
    static std::set<std::string> multipleLenStructs = { "VkAccelerationStructureTrianglesDisplacementMicromapNV",
                                                        "VkImageConstraintsInfoFUCHSIA",
                                                        "VkIndirectCommandsLayoutTokenNV",
                                                        "VkIndirectExecutionSetShaderInfoEXT",
                                                        "VkPipelineBinaryKeysAndDataKHR",
                                                        "VkPresentInfoKHR",
                                                        "VkSemaphoreWaitInfo",
                                                        "VkSetDescriptorBufferOffsetsInfoEXT",
                                                        "VkSubmitInfo",
                                                        "VkSubpassDescription",
                                                        "VkSubpassDescription2",
                                                        "VkVideoDecodeAV1PictureInfoKHR",
                                                        "VkWin32KeyedMutexAcquireReleaseInfoKHR",
                                                        "VkWin32KeyedMutexAcquireReleaseInfoNV" };
    bool                         warned             = false;
    for ( auto m0It = it->second.members.begin(); !warned && ( m0It != it->second.members.end() ); ++m0It )
    {
      if ( !m0It->lenExpressions.empty() && ( m0It->lenExpressions.front() != "null-terminated" ) )
      {
        for ( auto m1It = std::next( m0It ); !warned && ( m1It != it->second.members.end() ); ++m1It )
        {
          if ( !m1It->lenExpressions.empty() && ( m0It->lenExpressions.front() == m1It->lenExpressions.front() ) )
          {
            if ( mutualExclusiveStructs.contains( it->first ) )
            {
              it->second.mutualExclusiveLens = true;
            }
            else
            {
              checkForWarning(
                multipleLenStructs.contains( it->first ),
                line,
                "Encountered structure <" + it->first +
                  "> with multiple members referencing the same member for len. Need to be checked if they are supposed to be mutually exclusive." );
              warned = true;
            }
          }
        }
      }
    }
  }
}

void VulkanHppGenerator::readTypes( tinyxml2::XMLElement const * element )
{
  const int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, { { "comment", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "type", false } }, { "comment" } );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "type" )
    {
      readTypesType( child );
    }
  }
}

void VulkanHppGenerator::readTypesType( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );

  auto categoryIt = attributes.find( "category" );
  if ( categoryIt != attributes.end() )
  {
    if ( categoryIt->second == "basetype" )
    {
      readTypeBasetype( element, attributes );
    }
    else if ( categoryIt->second == "bitmask" )
    {
      readTypeBitmask( element, attributes );
    }
    else if ( categoryIt->second == "define" )
    {
      readTypeDefine( element, attributes );
    }
    else if ( categoryIt->second == "enum" )
    {
      readTypeEnum( element, attributes );
    }
    else if ( categoryIt->second == "funcpointer" )
    {
      readTypeFuncpointer( element, attributes );
    }
    else if ( categoryIt->second == "handle" )
    {
      readTypeHandle( element, attributes );
    }
    else if ( categoryIt->second == "include" )
    {
      readTypeInclude( element, attributes );
    }
    else if ( ( categoryIt->second == "struct" ) || ( categoryIt->second == "union" ) )
    {
      readTypeStruct( element, ( categoryIt->second == "union" ), attributes );
    }
    else
    {
      checkForError( false, line, "unknown category <" + categoryIt->second + "> encountered" );
    }
  }
  else
  {
    auto requiresIt = attributes.find( "requires" );
    if ( requiresIt != attributes.end() )
    {
      readTypeRequires( element, attributes );
    }
    else
    {
      checkForError( ( attributes.size() == 1 ) && ( attributes.begin()->first == "name" ) && ( attributes.begin()->second == "int" ), line, "unknown type" );
      checkForError( m_types.insert( { "int", TypeData{ TypeCategory::Unknown, {}, line } } ).second, line, "type <int> already specified" );
    }
  }
}

TypeInfo VulkanHppGenerator::readTypeInfo( tinyxml2::XMLElement const * element ) const
{
  TypeInfo                  typeInfo;
  tinyxml2::XMLNode const * previousSibling = element->PreviousSibling();
  if ( previousSibling && previousSibling->ToText() )
  {
    typeInfo.prefix = trim( previousSibling->Value() );
  }
  typeInfo.type                         = element->GetText();
  tinyxml2::XMLNode const * nextSibling = element->NextSibling();
  if ( nextSibling && nextSibling->ToText() )
  {
    typeInfo.postfix = trimStars( trimEnd( nextSibling->Value() ) );
  }
  return typeInfo;
}

void VulkanHppGenerator::readVideoCapabilities( tinyxml2::XMLElement const * element, VideoCodec & videoCodec )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "struct", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string capabilities = attributes.find( "struct" )->second;
  checkForError( std::find( videoCodec.capabilities.begin(), videoCodec.capabilities.end(), capabilities ) == videoCodec.capabilities.end(),
                 line,
                 "videocapabilities struct <" + capabilities + "> already listed for videoCodec <" + videoCodec.name + ">" );
  checkForError( m_structs.contains( capabilities ), line, "videocodec <" + videoCodec.name + "> lists unknown capabilities struct <" + capabilities + ">" );
  videoCodec.capabilities.push_back( capabilities );
}

void VulkanHppGenerator::readVideoCodec( tinyxml2::XMLElement const * element )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, { { "extend", {} }, { "value", {} } } );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "videocapabilities", false } }, { "videoformat", "videoprofiles" } );

  VideoCodec videoCodec;
  videoCodec.xmlLine = line;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "extend" )
    {
      videoCodec.extend = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      videoCodec.name = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      videoCodec.value = attribute.second;
    }
  }

  checkForError( findByName( m_videoCodecs, videoCodec.name ) == m_videoCodecs.end(), line, "Video Codec <" + videoCodec.name + "> already listed" );
  checkForError( videoCodec.extend.empty() || findByName( m_videoCodecs, videoCodec.extend ) != m_videoCodecs.end(),
                 line,
                 "Video Codec <" + videoCodec.name + "> extends unknown <" + videoCodec.extend + ">" );
  if ( !videoCodec.value.empty() )
  {
    auto enumIt = m_enums.find( "VkVideoCodecOperationFlagBitsKHR" );
    assert( enumIt != m_enums.end() );
    checkForError( findByName( enumIt->second.values, videoCodec.value ) != enumIt->second.values.end(),
                   line,
                   "Video Codec <" + videoCodec.name + "> lists unknown value <" + videoCodec.value + ">" );
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "videocapabilities" )
    {
      readVideoCapabilities( child, videoCodec );
    }
    else if ( value == "videoformat" )
    {
      readVideoFormat( child, videoCodec );
    }
    else if ( value == "videoprofiles" )
    {
      readVideoProfiles( child, videoCodec );
    }
  }

  m_videoCodecs.push_back( videoCodec );
}

void VulkanHppGenerator::readVideoCodecs( tinyxml2::XMLElement const * element )
{
  const int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "videocodec", false } } );

  for ( auto child : children )
  {
    std::string value = child->Value();
    assert( value == "videocodec" );
    readVideoCodec( child );
  }
}

void VulkanHppGenerator::readVideoFormat( tinyxml2::XMLElement const * element, VideoCodec & videoCodec )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );

  if ( attributes.contains( "extend" ) )
  {
    checkAttributes( line, attributes, { { "extend", {} } }, {} );

    std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
    checkElements( line, children, { { "videoformatproperties", true } } );

    std::string extend = attributes.find( "extend" )->second;

    std::vector<VideoFormat>::iterator videoFormatIt;

    auto videoCodecIt = std::find_if( m_videoCodecs.begin(),
                                      m_videoCodecs.end(),
                                      [&extend, &videoFormatIt]( auto & vc )
                                      {
                                        videoFormatIt =
                                          std::find_if( vc.formats.begin(), vc.formats.end(), [&extend]( auto & vf ) { return vf.name == extend; } );
                                        return videoFormatIt != vc.formats.end();
                                      } );
    checkForError( videoCodecIt != m_videoCodecs.end(), line, "videocodec <" + videoCodec.name + "> extends unknown videoformat <" + extend + ">" );

    for ( auto child : children )
    {
      std::string value = child->Value();
      if ( value == "videoformatproperties" )
      {
        readVideoFormatProperties( child, videoCodecIt->name, *videoFormatIt );
      }
    }
  }
  else
  {
    checkAttributes( line, attributes, { { "name", {} }, { "usage", {} } }, {} );

    std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
    checkElements( line, children, {}, { "videoformatproperties", "videorequirecapabilities" } );

    auto flagBitsIt = m_enums.find( "VkImageUsageFlagBits" );
    assert( flagBitsIt != m_enums.end() );

    VideoFormat format;
    format.xmlLine = line;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "name" )
      {
        format.name = attribute.second;
        checkForError( findByName( videoCodec.formats, format.name ) == videoCodec.formats.end(),
                       line,
                       "Video Format <" + format.name + "> has already been listed for Video Codec <" + videoCodec.name + ">" );
      }
      else if ( attribute.first == "usage" )
      {
        format.usage = tokenize( attribute.second, "+" );
        for ( auto const & usage : format.usage )
        {
          checkForError( findByName( flagBitsIt->second.values, usage ) != flagBitsIt->second.values.end(),
                         line,
                         "Unknown Video Format <" + format.name + "> listed for Video Codec <" + videoCodec.name + ">" );
        }
      }
    }
    videoCodec.formats.push_back( format );

    for ( auto child : children )
    {
      std::string value = child->Value();
      if ( value == "videoformatproperties" )
      {
        readVideoFormatProperties( child, videoCodec.name, videoCodec.formats.back() );
      }
      else if ( value == "videorequirecapabilities" )
      {
        readVideoRequireCapabilities( child, videoCodec );
      }
    }
  }
}

void VulkanHppGenerator::readVideoProfileMember( tinyxml2::XMLElement const * element, VideoCodec & videoCodec )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "videoprofile" } );

  VideoProfileMember profileMember;
  profileMember.xmlLine = line;
  profileMember.name    = attributes.find( "name" )->second;

  checkForError( findByName( videoCodec.profiles.back().members, profileMember.name ) == videoCodec.profiles.back().members.end(),
                 line,
                 "Video Profile Member <" + profileMember.name + "> already listed for Video Profiles <" + videoCodec.profiles.back().name );

  videoCodec.profiles.back().members.push_back( profileMember );

  for ( auto child : children )
  {
    std::string value = child->Value();
    assert( value == "videoprofile" );
    readVideoProfile( child, videoCodec );
  }
}

void VulkanHppGenerator::readVideoProfile( tinyxml2::XMLElement const * element, VideoCodec & videoCodec )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} }, { "value", {} } }, {} );
  checkElements( line, getChildElements( element ), {}, {} );

  VideoProfile profile;
  profile.xmlLine = line;

  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      profile.name = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      profile.value = attribute.second;
    }
  }

  checkForError( findByName( videoCodec.profiles.back().members.back().profiles, profile.name ) == videoCodec.profiles.back().members.back().profiles.end(),
                 line,
                 "Video Profile <" + profile.name + "> already listed for VideoProfileMember <" + videoCodec.profiles.back().members.back().name );

  // value checking would need cross-loading of video.xml !!

  videoCodec.profiles.back().members.back().profiles.push_back( profile );
}

void VulkanHppGenerator::readVideoProfiles( tinyxml2::XMLElement const * element, VideoCodec & videoCodec )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "struct", {} } }, {} );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "videoprofilemember" } );

  VideoProfiles profiles;
  profiles.xmlLine = line;
  profiles.name    = attributes.find( "struct" )->second;

  checkForError( findByName( videoCodec.profiles, profiles.name ) == videoCodec.profiles.end(),
                 line,
                 "Video Profiles struct <" + profiles.name + "> already listed for Video Codec <" + videoCodec.name + ">" );
  checkForError( m_structs.find( profiles.name ) != m_structs.end(),
                 line,
                 "Unknown Video Profiles struct <" + profiles.name + "> used with VideoCodec <" + videoCodec.name + ">" );

  videoCodec.profiles.push_back( profiles );

  for ( auto child : children )
  {
    std::string value = child->Value();
    assert( value == "videoprofilemember" );
    readVideoProfileMember( child, videoCodec );
  }
}

void VulkanHppGenerator::readVideoFormatProperties( tinyxml2::XMLElement const * element, std::string const & videoCodec, VideoFormat & videoFormat )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "struct", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string formatProperties = attributes.find( "struct" )->second;

  checkForError( m_structs.find( formatProperties ) != m_structs.end(),
                 line,
                 "videoCodec <" + videoCodec + "> lists unknown struct <" + formatProperties + "> as videoformatproperties" );
  checkForError( std::find( videoFormat.formatProperties.begin(), videoFormat.formatProperties.end(), formatProperties ) == videoFormat.formatProperties.end(),
                 line,
                 "videoformatproperties <" + formatProperties + "> listed in videocodec <" + videoCodec + "> already listed for videoformat <" +
                   videoFormat.name + ">" );

  videoFormat.formatProperties.push_back( formatProperties );
}

void VulkanHppGenerator::readVideoRequireCapabilities( tinyxml2::XMLElement const * element, VideoCodec & videoCodec )
{
  const int                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "member", {} }, { "struct", {} }, { "value", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  VideoRequireCapabilities requireCapabilities;
  requireCapabilities.xmlLine = line;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "member" )
    {
      requireCapabilities.member = attribute.second;
    }
    else if ( attribute.first == "struct" )
    {
      requireCapabilities.name = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      requireCapabilities.value = attribute.second;
    }
  }

  VideoFormat & format = videoCodec.formats.back();

  auto structIt = m_structs.find( requireCapabilities.name );
  checkForError( structIt != m_structs.end(),
                 line,
                 "Unknown Video Require Capablities struct <" + requireCapabilities.name + "> listed for Video Format <" + format.name + "> in VideoCodec <" +
                   videoCodec.name + ">" );

  auto memberIt = findByName( structIt->second.members, requireCapabilities.member );
  checkForError( memberIt != structIt->second.members.end(),
                 line,
                 "Unknown member <" + requireCapabilities.member + "> used for Video Require Capabilities struct <" + requireCapabilities.name +
                   "> in VideoFormat <" + format.name + "> in VideoCodec <" + videoCodec.name + ">" );

  auto bitmaskIt = m_bitmasks.find( memberIt->type.type );
  checkForError( bitmaskIt != m_bitmasks.end(), line, "Video Require Capabilities member <" + requireCapabilities.member + "> is not a bitmask" );

  auto enumIt = m_enums.find( bitmaskIt->second.require );
  assert( enumIt != m_enums.end() );

  checkForError( findByName( enumIt->second.values, requireCapabilities.value ) != enumIt->second.values.end(),
                 line,
                 "Unknown value <" + requireCapabilities.value + "> listed for Video Require Capabilities <" + requireCapabilities.name + "> in Video Codec <" +
                   videoCodec.name + ">" );

  format.requireCapabilities.push_back( requireCapabilities );
}

void VulkanHppGenerator::registerDeleter( std::string const & commandName, CommandData const & commandData )
{
  // some special handling for release functions that don't release an object
  const std::set<std::string> noDeleterFunctions = {
    "vkReleaseCapturedPipelineDataKHR", "vkReleaseFullScreenExclusiveModeEXT", "vkReleaseProfilingLockKHR", "vkReleaseSwapchainImagesEXT"
  };

  if ( ( commandName.substr( 2, 7 ) == "Destroy" ) || ( commandName.substr( 2, 4 ) == "Free" ) ||
       ( ( commandName.substr( 2, 7 ) == "Release" ) && !noDeleterFunctions.contains( commandName ) ) )
  {
    std::string key;
    size_t      valueIndex = 0;
    switch ( commandData.params.size() )
    {
      case 2:
        if ( commandData.params.back().type.type == "VkAllocationCallbacks" )
        {
          key        = "";
          valueIndex = 0;
        }
        else
        {
          key        = commandData.params[0].type.type;
          valueIndex = 1;
        }
        break;
      case 3:
        assert( commandData.params.back().type.type == "VkAllocationCallbacks" );
        key        = commandData.params[0].type.type;
        valueIndex = 1;
        break;
      case 4:
        key        = commandData.params[0].type.type;
        valueIndex = 3;
        assert( m_handles.contains( commandData.params[valueIndex].type.type ) );
        m_handles.find( commandData.params[valueIndex].type.type )->second.deletePool = commandData.params[1].type.type;
        break;
      default: assert( false );
    }
    auto keyHandleIt = m_handles.find( key );
    assert( keyHandleIt != m_handles.end() );
    keyHandleIt->second.childrenHandles.insert( commandData.params[valueIndex].type.type );

    auto handleIt = m_handles.find( commandData.params[valueIndex].type.type );
    assert( handleIt != m_handles.end() );
    handleIt->second.deleteCommand  = commandName;
    handleIt->second.destructorType = key;
  }
}

void VulkanHppGenerator::rescheduleRAIIHandle( std::string &                              str,
                                               std::pair<std::string, HandleData> const & handle,
                                               std::set<std::string> &                    listedHandles,
                                               std::set<std::string> const &              specialFunctions ) const
{
  listedHandles.insert( handle.first );
  if ( !handle.second.parent.empty() && !listedHandles.contains( handle.second.parent ) )
  {
    auto parentIt = m_handles.find( handle.second.parent );
    assert( parentIt != m_handles.end() );
    str += generateRAIIHandle( *parentIt, listedHandles, specialFunctions );
  }

  for ( auto constructorIt : handle.second.constructorIts )
  {
    for ( auto const & param : constructorIt->second.params )
    {
      auto handleIt = m_handles.find( param.type.type );
      if ( handleIt != m_handles.end() && !listedHandles.contains( param.type.type ) )
      {
        str += generateRAIIHandle( *handleIt, listedHandles, specialFunctions );
      }
    }
  }
}

std::vector<std::string> VulkanHppGenerator::selectCommandsByHandle( std::vector<RequireData> const & requireData,
                                                                     std::set<std::string> const &    handleCommands,
                                                                     std::set<std::string> &          listedCommands ) const
{
  std::vector<std::string> selectedCommands;
  for ( auto const & require : requireData )
  {
    for ( auto const & command : require.commands )
    {
      if ( handleCommands.contains( command.name ) && listedCommands.insert( command.name ).second )
      {
        selectedCommands.push_back( command.name );
      }
    }
  }
  return selectedCommands;
}

bool VulkanHppGenerator::skipLeadingGrandParent( std::pair<std::string, HandleData> const & handle ) const
{
  bool skip = false;
  assert( !handle.second.constructorIts.empty() );
  auto constructorIt = handle.second.constructorIts.begin();
  if ( ( 1 < ( *constructorIt )->second.params.size() ) && isHandleType( ( *constructorIt )->second.params[0].type.type ) &&
       ( ( *constructorIt )->second.params[1].type.type == handle.second.parent ) )
  {
    auto parentIt = m_handles.find( handle.second.parent );
    assert( parentIt != m_handles.end() );
    skip = ( ( *constructorIt )->second.params[0].type.type == parentIt->second.parent );
#if !defined( NDEBUG )
    for ( auto it = std::next( constructorIt ); it != handle.second.constructorIts.end(); ++it )
    {
      assert( ( *it )->second.params[0].type.type == ( *constructorIt )->second.params[0].type.type );
      assert( !skip || ( ( *it )->second.params[1].type.type == ( *constructorIt )->second.params[1].type.type ) );
    }
#endif
  }
  return skip;
}

std::string VulkanHppGenerator::stripPluralS( std::string const & name ) const
{
  std::string strippedName = name;
  std::string tag          = findTag( name );
  if ( strippedName.ends_with( "s" + tag ) )
  {
    const size_t pos = strippedName.rfind( 's' );
    if ( ( 2 <= pos ) && ( strippedName.substr( pos - 2, 3 ) == "ies" ) )
    {
      strippedName.replace( pos - 2, 3, "y" );
    }
    else
    {
      strippedName.erase( pos, 1 );
    }
  }
  return strippedName;
}

std::string VulkanHppGenerator::toString( TypeCategory category )
{
  switch ( category )
  {
    case TypeCategory::Bitmask: return "bitmask";
    case TypeCategory::BaseType: return "basetype";
    case TypeCategory::Define: return "define";
    case TypeCategory::Enum: return "enum";
    case TypeCategory::ExternalType: return "ExternalType";
    case TypeCategory::FuncPointer: return "funcpointer";
    case TypeCategory::Handle: return "handle";
    case TypeCategory::Struct: return "struct";
    case TypeCategory::Union: return "union";
    case TypeCategory::Unknown: return "unkown";
    default: assert( false ); return "";
  }
}

VulkanHppGenerator::MemberData const & VulkanHppGenerator::vectorMemberByStructure( std::string const & structureType ) const
{
  auto structIt = m_structs.find( structureType );
  assert( structIt != m_structs.end() );
  assert( describesVector( structIt->second ) );
  return structIt->second.members.back();
}

void VulkanHppGenerator::EnumData::addEnumAlias( int line, std::string const & name, std::string const & alias, std::string const & protect, bool supported )
{
  auto aliasIt = findByName( valueAliases, name );
  if ( aliasIt == valueAliases.end() )
  {
    valueAliases.push_back( { alias, name, protect, supported, line } );
  }
  else
  {
    checkForError( ( alias == aliasIt->alias ) && ( protect == aliasIt->protect ) && ( supported == aliasIt->supported ),
                   line,
                   "enum value alias <" + name + "> already listed with different properties" );
  }
}

void VulkanHppGenerator::EnumData::addEnumValue(
  int line, std::string const & name, std::string const & protect, std::string const & bitpos, std::string const & value, bool supported, bool deprecated )
{
  auto valueIt = findByName( values, name );
  if ( valueIt == values.end() )
  {
    values.push_back( { {}, bitpos, deprecated, name, protect, supported, value, line } );
  }
  else if ( supported )  // only for supported enum values, we need to check for consistency!
  {
    checkForError( ( bitpos == valueIt->bitpos ) && ( protect == valueIt->protect ) && ( value == valueIt->value ),
                   line,
                   "enum value <" + name + "> already listed with different properties" );
    // if a previous version was not supported, make it supported now
    if ( !valueIt->supported )
    {
      valueIt->supported = true;
      valueIt->xmlLine   = line;
    }
  }
}

//
// VulkanHppGenerator local functions
//

namespace
{
  std::vector<std::pair<std::string, size_t>> filterNumbers( std::vector<std::string> const & names )
  {
    std::vector<std::pair<std::string, size_t>> filteredNames;
    for ( auto const & name : names )
    {
      if ( !isNumber( name ) )
      {
        filteredNames.push_back( { name, ~0 } );
      }
    }
    return filteredNames;
  }

  template <typename T>
  typename std::vector<T>::iterator findByName( std::vector<T> & values, std::string const & name )
  {
    return std::find_if( values.begin(), values.end(), [&name]( T const & value ) { return value.name == name; } );
  }

  template <typename T>
  typename std::map<std::string, T>::const_iterator findByNameOrAlias( std::map<std::string, T> const & values, std::string const & name )
  {
    auto it = values.find( name );
    if ( it == values.end() )
    {
      it = std::find_if(
        values.begin(), values.end(), [&name]( auto const & value ) { return ( value.first == name ) || value.second.aliases.contains( name ); } );
    }
    return it;
  }

  template <typename T>
  typename std::map<std::string, T>::iterator findByNameOrAlias( std::map<std::string, T> & values, std::string const & name )
  {
    auto it = values.find( name );
    if ( it == values.end() )
    {
      it = std::find_if(
        values.begin(), values.end(), [&name]( auto const & value ) { return ( value.first == name ) || value.second.aliases.contains( name ); } );
    }
    assert( it != values.end() );
    return it;
  }

  template <typename T>
  typename std::vector<T>::const_iterator findByNameOrAlias( std::vector<T> const & values, std::string const & name )
  {
    return std::find_if( values.begin(),
                         values.end(),
                         [&name]( T const & value ) {
                           return ( value.name == name ) ||
                                  std::any_of( value.aliases.begin(), value.aliases.end(), [&name]( auto const & eav ) { return eav.name == name; } );
                         } );
  }

  template <typename T>
  typename std::vector<T>::iterator findByNameOrAlias( std::vector<T> & values, std::string const & name )
  {
    return std::find_if( values.begin(), values.end(), [&name]( T const & value ) { return ( value.name == name ) || value.aliases.contains( name ); } );
  }

  std::string generateCArraySizes( std::vector<std::string> const & sizes )
  {
    std::string arraySizes;
    for ( auto const & s : sizes )
    {
      arraySizes += "[" + s + "]";
    }
    return arraySizes;
  }

  std::string generateList( std::vector<std::string> const & elements, std::string const & prefix, std::string const & separator )
  {
    std::string list;
    if ( !elements.empty() )
    {
      list = prefix + elements[0];
      for ( size_t i = 1; i < elements.size(); ++i )
      {
        list += separator + elements[i];
      }
    }
    return list;
  }

  std::string generateNamespacedType( std::string const & type )
  {
    return type.starts_with( "Vk" ) ? ( "VULKAN_HPP_NAMESPACE::" + stripPrefix( type, "Vk" ) ) : type;
  }

  std::string generateNoDiscard( bool returnsSomething, bool multiSuccessCodes, bool multiErrorCodes )
  {
    return ( returnsSomething || multiSuccessCodes ) ? "VULKAN_HPP_NODISCARD " : ( multiErrorCodes ? "VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS " : "" );
  }

  std::string generateStandardArray( std::string const & type, std::vector<std::string> const & sizes )
  {
    std::string arrayString = "std::array<" + type + "," + sizes.back() + ">";
    for ( size_t i = sizes.size() - 2; i < sizes.size(); i-- )
    {
      arrayString = "std::array<" + arrayString + "," + sizes[i] + ">";
    }
    return arrayString;
  }

  bool isAllUpper( std::string const & name )
  {
    return std::none_of( name.begin(), name.end(), []( auto const & c ) noexcept { return c != toupper( c ); } );
  }

  // function to take three or four-vector of strings containing a macro definition, and return
  // a tuple with possibly the deprecation reason, possibly the called macro, the macro parameters, and possibly the definition
  VulkanHppGenerator::MacroData parseMacro( std::vector<std::string> const & completeMacro )
  {
    // #define macro definition
    // #define macro( params ) definition
    // #define macro1 macro2( params )
    auto const paramsRegex  = std::regex{ R"((\(.*?\)))" };
    auto const commentRegex = std::regex{ R"(\s*//.*)" };

    auto rawComment = completeMacro[0];
    std::erase( rawComment, '/' );
    auto const strippedComment = trim( stripPostfix( stripPrefix( rawComment, " DEPRECATED:" ), "#define " ) );

    // special case for VK_USE_64_BIT_PTR_DEFINES
    if ( completeMacro.size() == 1 && completeMacro.front().find( "#ifndef VK_USE_64_BIT_PTR_DEFINES" ) != std::string::npos )
    {
      return { {}, {}, {}, strippedComment };
    }

    // macro with parameters and implementation
    if ( completeMacro.size() == 3 )
    {
      auto const & paramsAndDefinitionAndTrailingComment = completeMacro[2];

      if ( paramsAndDefinitionAndTrailingComment.find( '(' ) == std::string::npos )
      {
        // no opening parenthesis found => no parameters
        return { strippedComment, {}, {}, std::regex_replace( paramsAndDefinitionAndTrailingComment, commentRegex, "" ) };
      }

      // match the first set of parentheses only
      auto paramsMatch = std::smatch{};
      std::regex_search( paramsAndDefinitionAndTrailingComment, paramsMatch, paramsRegex );

      // remove the leading and trailing parentheses and tokenise the remaining string
      auto params = tokenize( stripPrefix( stripPostfix( paramsMatch[1].str(), ")" ), "(" ), "," );

      // replace the parameters with empty string, leaving behind the implementation and (possibly) a trailing comment
      auto implementation = std::regex_replace( paramsAndDefinitionAndTrailingComment, paramsRegex, "", std::regex_constants::format_first_only );
      implementation      = implementation.substr( 0, implementation.find( "//" ) );
      std::erase( implementation, '\\' );
      implementation = trim( implementation );

      return { strippedComment, {}, params, implementation };
    }
    if ( completeMacro.size() == 4 )
    {
      auto const & calledMacro            = toCamelCase( stripPrefix( completeMacro[2], "VK_" ) );
      auto const & argsAndTrailingComment = completeMacro[3];

      auto argsMatch = std::smatch{};
      std::regex_search( argsAndTrailingComment, argsMatch, paramsRegex );
      auto args = tokenize( stripPrefix( stripPostfix( argsMatch[1].str(), ")" ), "(" ), "," );

      return { strippedComment, calledMacro, args, {} };
    }
    return {};
  }

  std::string readSnippet( std::string const & snippetFile )
  {
    std::ifstream ifs( std::string( BASE_PATH ) + "/snippets/" + snippetFile );
    assert( !ifs.fail() );
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
  }

  std::string startLowerCase( std::string const & input )
  {
    assert( !input.empty() );
    return static_cast<char>( tolower( input[0] ) ) + input.substr( 1 );
  }

  std::string startUpperCase( std::string const & input )
  {
    assert( !input.empty() );
    return static_cast<char>( toupper( input[0] ) ) + input.substr( 1 );
  }

  std::vector<std::string> tokenizeAny( std::string const & tokenString, std::string const & separators )
  {
    const size_t             len = tokenString.length();
    std::vector<std::string> tokens;
    if ( !tokenString.empty() )
    {
      size_t start = 0, end;
      do
      {
        end = tokenString.find_first_of( separators, start );
        if ( ( start != end ) && ( start < len ) )
        {
          tokens.push_back( trim( tokenString.substr( start, end - start ) ) );
        }
        start = end + 1;
      } while ( end != std::string::npos );
    }
    return tokens;
  }
}  // namespace

int main( int argc, char const ** argv )
{
  if ( ( argc % 2 ) == 0 )
  {
    std::cout << "VulkanHppGenerator usage: VulkanHppGenerator [-f filename][-api [vulkan|vulkansc]]" << std::endl;
    std::cout << "\tdefault for filename is <" << VK_SPEC << ">" << std::endl;
    std::cout << "\tdefault for api <vulkan>" << std::endl;
    std::cout << "\tsupported values for api are <vulkan> and <vulkansc>" << std::endl;
    return -1;
  }

  std::string api      = "vulkan";
  std::string filename = VK_SPEC;
  for ( int i = 1; i < argc; i += 2 )
  {
    if ( strcmp( argv[i], "-api" ) == 0 )
    {
      api = argv[i + 1];
    }
    else if ( strcmp( argv[i], "-f" ) == 0 )
    {
      filename = argv[i + 1];
    }
    else
    {
      std::cout << "unsupported argument <" << argv[i] << ">" << std::endl;
      return -1;
    }
  }

  if ( ( api != "vulkan" ) && ( api != "vulkansc" ) )
  {
    std::cout << "unsupported api <" << api << ">" << std::endl;
    return -1;
  }

#if defined( CLANG_FORMAT_EXECUTABLE )
  std::cout << "VulkanHppGenerator: Found ";
  std::string commandString = "\"" CLANG_FORMAT_EXECUTABLE "\" --version ";
  const int   ret           = std::system( commandString.c_str() );
  if ( ret != 0 )
  {
    std::cout << "VulkanHppGenerator: failed to determine clang_format version with error <" << ret << ">\n";
  }
#else
  std::cout << "VulkanHppGenerator: could not find clang-format. The generated files will not be formatted accordingly.\n";
#endif

  tinyxml2::XMLDocument doc;
  std::cout << "VulkanHppGenerator: Loading " << filename << std::endl;
  const tinyxml2::XMLError error = doc.LoadFile( filename.c_str() );
  if ( error != tinyxml2::XML_SUCCESS )
  {
    std::cout << "VulkanHppGenerator: failed to load file " << filename << " with error <" << toString( error ) << ">" << std::endl;
    return -1;
  }

  try
  {
    std::cout << "VulkanHppGenerator: Parsing " << filename << std::endl;
    VulkanHppGenerator generator( doc, api );

    generator.prepareRAIIHandles();

    std::vector<std::future<void>> generateFutures;
    generateFutures.emplace_back( std::async( &VulkanHppGenerator::generateCppModuleFile, &generator ) );
    generateFutures.emplace_back( std::async( &VulkanHppGenerator::generateHppFile, &generator ) );
    generateFutures.emplace_back( std::async( &VulkanHppGenerator::generateEnumsHppFile, &generator ) );
    generateFutures.emplace_back( std::async( &VulkanHppGenerator::generateExtensionInspectionFile, &generator ) );
    generateFutures.emplace_back( std::async( &VulkanHppGenerator::generateFormatTraitsHppFile, &generator ) );
    generateFutures.emplace_back( std::async( &VulkanHppGenerator::generateFuncsHppFile, &generator ) );
    generateFutures.emplace_back( std::async( &VulkanHppGenerator::generateHandlesHppFile, &generator ) );
    generateFutures.emplace_back( std::async( &VulkanHppGenerator::generateHashHppFile, &generator ) );
    generateFutures.emplace_back( std::async( &VulkanHppGenerator::generateMacrosFile, &generator ) );
    generateFutures.emplace_back( std::async( &VulkanHppGenerator::generateSharedHppFile, &generator ) );
    generateFutures.emplace_back( std::async( &VulkanHppGenerator::generateStaticAssertionsHppFile, &generator ) );
    generateFutures.emplace_back( std::async( &VulkanHppGenerator::generateStructsHppFile, &generator ) );
    generateFutures.emplace_back( std::async( &VulkanHppGenerator::generateToStringHppFile, &generator ) );
    std::ranges::for_each( generateFutures, []( auto & generatorFuture ) { generatorFuture.wait(); } );

    // this modifies the generator data and needs to be done after all the other generations are done
    generator.distributeSecondLevelCommands();
    generator.generateRAIIHppFile();
  }
  catch ( std::exception const & e )
  {
    std::cout << "caught exception: " << e.what() << std::endl;
    return -1;
  }
  catch ( ... )
  {
    std::cout << "caught unknown exception" << std::endl;
    return -1;
  }
}
