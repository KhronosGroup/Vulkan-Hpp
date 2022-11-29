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

#include <algorithm>
#include <cassert>
#include <fstream>
#include <regex>
#include <sstream>

void                                checkAttributes( int                                                  line,
                                                     std::map<std::string, std::string> const &           attributes,
                                                     std::map<std::string, std::set<std::string>> const & required,
                                                     std::map<std::string, std::set<std::string>> const & optional );
void                                checkElements( int                                               line,
                                                   std::vector<tinyxml2::XMLElement const *> const & elements,
                                                   std::map<std::string, bool> const &               required,
                                                   std::set<std::string> const &                     optional = {} );
void                                checkForError( bool condition, int line, std::string const & message );
void                                checkForWarning( bool condition, int line, std::string const & message );
std::string                         findTag( std::set<std::string> const & tags, std::string const & name, std::string const & postfix = "" );
std::string                         generateCArraySizes( std::vector<std::string> const & sizes );
std::pair<std::string, std::string> generateEnumSuffixes( std::string const & name, bool bitmask, std::set<std::string> const & tags );
std::string generateEnumValueName( std::string const & enumName, std::string const & valueName, bool bitmask, std::set<std::string> const & tags );
std::string generateNamespacedType( std::string const & type );
std::string generateNoDiscard( bool returnsSomething, bool multiSuccessCodes, bool multiErrorCodes );
std::string generateStandardArray( std::string const & type, std::vector<std::string> const & sizes );
std::string generateStandardArrayWrapper( std::string const & type, std::vector<std::string> const & sizes );
std::string generateSuccessCode( std::string const & code, std::set<std::string> const & tags );
std::map<std::string, std::string> getAttributes( tinyxml2::XMLElement const * element );
template <typename ElementContainer>
std::vector<tinyxml2::XMLElement const *>        getChildElements( ElementContainer const * element );
std::pair<std::vector<std::string>, std::string> readModifiers( tinyxml2::XMLNode const * node );
std::string                                      readSnippet( std::string const & snippetFile );
std::string                                      replaceWithMap( std::string const & input, std::map<std::string, std::string> replacements );
std::string                                      startLowerCase( std::string const & input );
std::string                                      startUpperCase( std::string const & input );
std::string                                      stripPostfix( std::string const & value, std::string const & postfix );
std::string                                      stripPluralS( std::string const & name, std::set<std::string> const & tags );
std::string                                      stripPrefix( std::string const & value, std::string const & prefix );
std::string                                      toCamelCase( std::string const & value );
std::string                                      toUpperCase( std::string const & name );
std::vector<std::string>                         tokenize( std::string const & tokenString, std::string const & separator );
std::vector<std::string>                         tokenizeAny( std::string const & tokenString, std::string const & separators );
std::string                                      toString( tinyxml2::XMLError error );
std::string                                      trim( std::string const & input );
std::string                                      trimEnd( std::string const & input );
std::string                                      trimStars( std::string const & input );
void                                             writeToFile( std::string const & str, std::string const & fileName );

const std::set<std::string> altLens             = { "2*VK_UUID_SIZE", "codeSize / 4", "(rasterizationSamples + 31) / 32", "(samples + 31) / 32" };
const std::set<std::string> specialPointerTypes = { "Display", "IDirectFB", "wl_display", "xcb_connection_t", "_screen_window" };

//
// VulkanHppGenerator public interface
//

VulkanHppGenerator::VulkanHppGenerator( tinyxml2::XMLDocument const & document )
{
  // insert the default "handle" without class (for createInstance, and such)
  m_handles.insert( std::make_pair( "", HandleData( {}, "", false, 0 ) ) );

  // read the document and check its correctness
  int                                       line     = document.GetLineNum();
  std::vector<tinyxml2::XMLElement const *> elements = getChildElements( &document );
  checkElements( line, elements, { { "registry", true } } );
  checkForError( elements.size() == 1, line, "encountered " + std::to_string( elements.size() ) + " elements named <registry> but only one is allowed" );
  readRegistry( elements[0] );
  checkCorrectness();

  // some "FlagBits" enums are not specified, but needed for our "Flags" handling -> add them here
  for ( auto & feature : m_features )
  {
    addMissingFlagBits( feature.second.requireData, feature.first );
  }
  for ( auto & extension : m_extensions )
  {
    addMissingFlagBits( extension.second.requireData, extension.first );
  }

  // determine the extensionsByNumber map
  for ( auto extensionIt = m_extensions.begin(); extensionIt != m_extensions.end(); ++extensionIt )
  {
    int number = stoi( extensionIt->second.number );
    assert( m_extensionsByNumber.find( number ) == m_extensionsByNumber.end() );
    m_extensionsByNumber[number] = extensionIt;
  }
}

void VulkanHppGenerator::generateVulkanEnumsHppFile() const
{
  std::string const vulkan_enums_hpp = std::string( BASE_PATH ) + "/vulkan/vulkan_enums.hpp";
  std::cout << "VulkanHppGenerator: Generating " << vulkan_enums_hpp << " ..." << std::endl;

  std::string const vulkanEnumsHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_ENUMS_HPP
#  define VULKAN_ENUMS_HPP

namespace VULKAN_HPP_NAMESPACE
{
  template <typename EnumType, EnumType value>
  struct CppType
  {};
${enums}
${indexTypeTraits}
}   // namespace VULKAN_HPP_NAMESPACE
#endif
)";

  std::string str = replaceWithMap(
    vulkanEnumsHppTemplate, { { "enums", generateEnums() }, { "indexTypeTraits", generateIndexTypeTraits() }, { "licenseHeader", m_vulkanLicenseHeader } } );

  writeToFile( str, vulkan_enums_hpp );
}

void VulkanHppGenerator::generateVulkanFormatTraitsHppFile() const
{
  std::string const vulkan_format_traits_hpp = std::string( BASE_PATH ) + "/vulkan/vulkan_format_traits.hpp";
  std::cout << "VulkanHppGenerator: Generating " << vulkan_format_traits_hpp << " ..." << std::endl;

  std::string const vulkanFormatTraitsHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_FORMAT_TRAITS_HPP
#  define VULKAN_FORMAT_TRAITS_HPP

#include <vulkan/vulkan.hpp>

namespace VULKAN_HPP_NAMESPACE
{
${formatTraits}
}   // namespace VULKAN_HPP_NAMESPACE
#endif
)";

  std::string str = replaceWithMap( vulkanFormatTraitsHppTemplate, { { "formatTraits", generateFormatTraits() }, { "licenseHeader", m_vulkanLicenseHeader } } );

  writeToFile( str, vulkan_format_traits_hpp );
}

void VulkanHppGenerator::generateVulkanFuncsHppFile() const
{
  std::string const vulkan_funcs_hpp = std::string( BASE_PATH ) + "/vulkan/vulkan_funcs.hpp";
  std::cout << "VulkanHppGenerator: Generating " << vulkan_funcs_hpp << " ..." << std::endl;

  std::string const vulkanFuncsHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_FUNCS_HPP
#  define VULKAN_FUNCS_HPP

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

void VulkanHppGenerator::generateVulkanHandlesHppFile() const
{
  std::string const vulkan_handles_hpp = std::string( BASE_PATH ) + "/vulkan/vulkan_handles.hpp";
  std::cout << "VulkanHppGenerator: Generating " << vulkan_handles_hpp << " ..." << std::endl;

  std::string const vulkanHandlesHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_HANDLES_HPP
#  define VULKAN_HANDLES_HPP

namespace VULKAN_HPP_NAMESPACE
{
${structForwardDeclarations}
${handles}
}   // namespace VULKAN_HPP_NAMESPACE
#endif
)";

  std::string str = replaceWithMap(
    vulkanHandlesHppTemplate,
    { { "handles", generateHandles() }, { "licenseHeader", m_vulkanLicenseHeader }, { "structForwardDeclarations", generateStructForwardDeclarations() } } );

  writeToFile( str, vulkan_handles_hpp );
}

void VulkanHppGenerator::generateVulkanHashHppFile() const
{
  std::string const vulkan_hash_hpp = std::string( BASE_PATH ) + "/vulkan/vulkan_hash.hpp";
  std::cout << "VulkanHppGenerator: Generating " << vulkan_hash_hpp << " ..." << std::endl;

  std::string const vulkanHandlesHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_HASH_HPP
#  define VULKAN_HASH_HPP

#include <vulkan/vulkan.hpp>

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
                                    { { "handleHashStructures", generateHandleHashStructures() },
                                      { "licenseHeader", m_vulkanLicenseHeader },
                                      { "structHashStructures", generateStructHashStructures() } } );

  writeToFile( str, vulkan_hash_hpp );
}

void VulkanHppGenerator::generateVulkanHppFile() const
{
  std::string const vulkan_hpp = std::string( BASE_PATH ) + "/vulkan/vulkan.hpp";
  std::cout << "VulkanHppGenerator: Generating " << vulkan_hpp << " ... " << std::endl;

  std::string const vulkanHppTemplate = R"(${licenseHeader}
${includes}

static_assert( VK_HEADER_VERSION == ${headerVersion}, "Wrong VK_HEADER_VERSION!" );

// 32-bit vulkan is not typesafe for non-dispatchable handles, so don't allow copy constructors on this platform by default.
// To enable this feature on 32-bit platforms please define VULKAN_HPP_TYPESAFE_CONVERSION
${typesafeCheck}
#  if !defined( VULKAN_HPP_TYPESAFE_CONVERSION )
#    define VULKAN_HPP_TYPESAFE_CONVERSION
#  endif
#endif

${defines}

namespace VULKAN_HPP_NAMESPACE
{
${ArrayWrapper1D}
${ArrayWrapper2D}
${Flags}
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
${ArrayProxy}
${ArrayProxyNoTemporaries}
${StridedArrayProxy}
${Optional}
${StructureChain}
${UniqueHandle}
#endif  // VULKAN_HPP_DISABLE_ENHANCED_MODE

${DispatchLoaderBase}
${DispatchLoaderStatic}
${DispatchLoaderDefault}
#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
${ObjectDestroy}
${ObjectFree}
${ObjectRelease}
${PoolFree}
#endif // !VULKAN_HPP_NO_SMART_HANDLE
${baseTypes}
} // namespace VULKAN_HPP_NAMESPACE

#include <vulkan/vulkan_enums.hpp>
#if !defined( VULKAN_HPP_NO_TO_STRING )
#include <vulkan/vulkan_to_string.hpp>
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
} // namespace VULKAN_HPP_NAMESPACE

// clang-format off
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>
#include <vulkan/vulkan_funcs.hpp>
// clang-format on

namespace VULKAN_HPP_NAMESPACE
{
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
${structExtendsStructs}
#endif // VULKAN_HPP_DISABLE_ENHANCED_MODE

${DynamicLoader}
${DispatchLoaderDynamic}
}   // namespace VULKAN_HPP_NAMESPACE
#endif
)";

  std::string str = replaceWithMap( vulkanHppTemplate,
                                    { { "ArrayProxy", readSnippet( "ArrayProxy.hpp" ) },
                                      { "ArrayProxyNoTemporaries", readSnippet( "ArrayProxyNoTemporaries.hpp" ) },
                                      { "ArrayWrapper1D", readSnippet( "ArrayWrapper1D.hpp" ) },
                                      { "ArrayWrapper2D", readSnippet( "ArrayWrapper2D.hpp" ) },
                                      { "baseTypes", generateBaseTypes() },
                                      { "defines", readSnippet( "defines.hpp" ) },
                                      { "DispatchLoaderBase", readSnippet( "DispatchLoaderBase.hpp" ) },
                                      { "DispatchLoaderDefault", readSnippet( "DispatchLoaderDefault.hpp" ) },
                                      { "DispatchLoaderDynamic", generateDispatchLoaderDynamic() },
                                      { "DispatchLoaderStatic", generateDispatchLoaderStatic() },
                                      { "DynamicLoader", readSnippet( "DynamicLoader.hpp" ) },
                                      { "Exceptions", readSnippet( "Exceptions.hpp" ) },
                                      { "Flags", readSnippet( "Flags.hpp" ) },
                                      { "headerVersion", m_version },
                                      { "includes", readSnippet( "includes.hpp" ) },
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
                                      { "typesafeCheck", m_typesafeCheck },
                                      { "UniqueHandle", readSnippet( "UniqueHandle.hpp" ) } } );

  writeToFile( str, vulkan_hpp );
}

void VulkanHppGenerator::generateVulkanRAIIHppFile() const
{
  std::string const vulkan_raii_hpp = std::string( BASE_PATH ) + "/vulkan/vulkan_raii.hpp";
  std::cout << "VulkanHppGenerator: Generating " << vulkan_raii_hpp << " ..." << std::endl;

  std::string const vulkanHandlesHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_RAII_HPP
#define VULKAN_RAII_HPP

#include <memory>
#include <utility>  // std::exchange, std::forward
#include <vulkan/vulkan.hpp>

#if !defined( VULKAN_HPP_RAII_NAMESPACE )
#  define VULKAN_HPP_RAII_NAMESPACE raii
#endif

#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE ) && !defined(VULKAN_HPP_NO_EXCEPTIONS)
namespace VULKAN_HPP_NAMESPACE
{
  namespace VULKAN_HPP_RAII_NAMESPACE
  {
    template <class T, class U = T>
    VULKAN_HPP_CONSTEXPR_14 VULKAN_HPP_INLINE T exchange( T & obj, U && newValue )
    {
#  if ( 14 <= VULKAN_HPP_CPP_VERSION )
      return std::exchange<T>( obj, std::forward<U>( newValue ) );
#  else
      T oldValue = std::move( obj );
      obj        = std::forward<U>( newValue );
      return oldValue;
#  endif
    }

${RAIIDispatchers}
${RAIIHandles}
${RAIICommandDefinitions}
  } // namespace VULKAN_HPP_RAII_NAMESPACE
}   // namespace VULKAN_HPP_NAMESPACE
#endif
#endif
)";

  std::string str = replaceWithMap( vulkanHandlesHppTemplate,
                                    { { "licenseHeader", m_vulkanLicenseHeader },
                                      { "RAIICommandDefinitions", generateRAIICommandDefinitions() },
                                      { "RAIIDispatchers", generateRAIIDispatchers() },
                                      { "RAIIHandles", generateRAIIHandles() } } );

  writeToFile( str, vulkan_raii_hpp );
}

void VulkanHppGenerator::generateVulkanStaticAssertionsHppFile() const
{
  std::string const static_assertions_hpp = std::string( BASE_PATH ) + "/vulkan/vulkan_static_assertions.hpp";
  std::cout << "VulkanHppGenerator: Generating " << static_assertions_hpp << " ..." << std::endl;

  std::string const vulkanHandlesHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_STATIC_ASSERTIONS_HPP
#  define VULKAN_STATIC_ASSERTIONS_HPP

#include <vulkan/vulkan.hpp>

//=========================
//=== static_assertions ===
//=========================

${staticAssertions}
#endif
)";

  std::string str =
    replaceWithMap( vulkanHandlesHppTemplate, { { "licenseHeader", m_vulkanLicenseHeader }, { "staticAssertions", generateStaticAssertions() } } );

  writeToFile( str, static_assertions_hpp );
}

void VulkanHppGenerator::generateVulkanStructsHppFile() const
{
  std::string const vulkan_structs_hpp = std::string( BASE_PATH ) + "/vulkan/vulkan_structs.hpp";
  std::cout << "VulkanHppGenerator: Generating " << vulkan_structs_hpp << " ..." << std::endl;

  std::string const vulkanHandlesHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_STRUCTS_HPP
#  define VULKAN_STRUCTS_HPP

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

void VulkanHppGenerator::generateVulkanToStringHppFile() const
{
  std::string const vulkan_to_string_hpp = std::string( BASE_PATH ) + "/vulkan/vulkan_to_string.hpp";
  std::cout << "VulkanHppGenerator: Generating " << vulkan_to_string_hpp << "..." << std::endl;

  std::string const vulkanHandlesHppTemplate = R"(${licenseHeader}
#ifndef VULKAN_TO_STRING_HPP
#  define VULKAN_TO_STRING_HPP

#include <vulkan/vulkan_enums.hpp>

#if __cpp_lib_format
#  include <format>   // std::format
#else
#  include <sstream>  // std::stringstream
#endif

namespace VULKAN_HPP_NAMESPACE
{
${bitmasksToString}
${enumsToString}
} // namespace VULKAN_HPP_NAMESPACE
#endif
)";

  std::string str = replaceWithMap(
    vulkanHandlesHppTemplate,
    { { "bitmasksToString", generateBitmasksToString() }, { "enumsToString", generateEnumsToString() }, { "licenseHeader", m_vulkanLicenseHeader } } );

  writeToFile( str, vulkan_to_string_hpp );
}

void VulkanHppGenerator::prepareRAIIHandles()
{
  // filter out functions that are not usefull on this level of abstraction (like vkGetInstanceProcAddr)
  // and all the construction and destruction functions, as they are used differently
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

  distributeSecondLevelCommands( m_RAIISpecialFunctions );

  // we rename a couple of function parameters to prevent this warning, treated as an error:
  // warning C4458: declaration of 'objectType' hides class member
  for ( auto & command : m_commands )
  {
    for ( auto & param : command.second.params )
    {
      if ( param.name == "objectType" )
      {
        param.name += "_";
      }
    }
  }
}

//
// VulkanHppGenerator private interface
//

void VulkanHppGenerator::addCommand( std::string const & name, CommandData & commandData )
{
  // find the handle this command is going to be associated to
  checkForError( !commandData.params.empty(), commandData.xmlLine, "command <" + name + "> with no params" );
  std::map<std::string, HandleData>::iterator handleIt = m_handles.find( commandData.params[0].type.type );
  if ( handleIt == m_handles.end() )
  {
    handleIt = m_handles.begin();
    assert( handleIt->first == "" );
  }
  commandData.handle = handleIt->first;

  // add this command to the list of commands
  checkForError( m_commands.insert( std::make_pair( name, commandData ) ).second, commandData.xmlLine, "already encountered command <" + name + ">" );

  // put the command into the handle's list of commands
  assert( handleIt->second.commands.find( name ) == handleIt->second.commands.end() );
  handleIt->second.commands.insert( name );
}

void VulkanHppGenerator::addMissingFlagBits( std::vector<RequireData> & requireData, std::string const & referencedIn )
{
  for ( auto & require : requireData )
  {
    std::vector<std::string> newTypes;
    for ( auto const & type : require.types )
    {
      auto bitmaskIt = m_bitmasks.find( type );
      if ( ( bitmaskIt != m_bitmasks.end() ) && bitmaskIt->second.requirements.empty() )
      {
        // generate the flagBits enum name out of the bitmask name: VkFooFlagsXXX -> VkFooFlagBitsXXX
        size_t pos = bitmaskIt->first.find( "Flags" );
        assert( pos != std::string::npos );
        std::string flagBits = bitmaskIt->first.substr( 0, pos + 4 ) + "Bit" + bitmaskIt->first.substr( pos + 4 );

        // as the bitmask's requirement is still empty, this flagBits should not be listed in the require list!
        assert( std::find_if( require.types.begin(), require.types.end(), [&flagBits]( std::string const & type ) { return ( type == flagBits ); } ) ==
                require.types.end() );

        bitmaskIt->second.requirements = flagBits;

        // some flagsBits are specified but never listed as required for any flags!
        // so, even if this bitmask has no enum listed as required, it might still already exist in the enums list
        auto enumIt = m_enums.find( flagBits );
        if ( enumIt == m_enums.end() )
        {
          m_enums.insert( std::make_pair( flagBits, EnumData( 0, true ) ) );

          assert( m_types.find( flagBits ) == m_types.end() );
          m_types.insert( std::make_pair( flagBits, TypeData( TypeCategory::Bitmask, referencedIn ) ) );
        }
        else
        {
          assert( m_types.find( flagBits ) != m_types.end() );
          enumIt->second.isBitmask = true;
        }

        newTypes.push_back( flagBits );
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
    auto [enter, leave] = generateProtection( getProtectFromTitle( title ) );
    str                 = "\n" + enter + "  //=== " + title + " ===\n" + strIf;
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
  return std::find_if_not( vectorParams.begin(),
                           vectorParams.end(),
                           [&params]( auto const & vpi )
                           {
                             return params[vpi.second.lenParam].type.isValue() &&
                                    ( ( params[vpi.second.lenParam].type.type == "uint32_t" ) || ( params[vpi.second.lenParam].type.type == "VkDeviceSize" ) ||
                                      ( params[vpi.second.lenParam].type.type == "VkSampleCountFlagBits" ) );
                           } ) == vectorParams.end();
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
      if ( listedCommands.insert( command ).second )
      {
        auto commandIt = m_commands.find( command );
        assert( commandIt != m_commands.end() );

        members += "    PFN_" + commandIt->first + " " + commandIt->first + " = 0;\n";
        placeholders += "    PFN_dummy " + commandIt->first + "_placeholder = 0;\n";
        if ( commandIt->second.handle.empty() )
        {
          initial += generateDispatchLoaderDynamicCommandAssignment( commandIt->first, commandIt->second, "NULL" );
        }
        else
        {
          instance += generateDispatchLoaderDynamicCommandAssignment( commandIt->first, commandIt->second, "instance" );
          if ( isDeviceCommand( commandIt->second ) )
          {
            device += generateDispatchLoaderDynamicCommandAssignment( commandIt->first, commandIt->second, "device" );
          }
        }
      }
    }
  }
  auto [enter, leave] = generateProtection( getProtectFromTitle( title ) );
  std::string header  = "\n" + enter + "  //=== " + title + " ===\n";
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
      if ( listedCommands.insert( command ).second )
      {
        auto commandIt = m_commands.find( command );
        assert( commandIt != m_commands.end() );
        if ( commandIt->second.handle.empty() )
        {
          assert( commandIt->second.alias.empty() );

          ci += ", " + commandIt->first + "( PFN_" + commandIt->first + "( getProcAddr( NULL, \"" + commandIt->first + "\" ) ) )";

          cm += "      PFN_" + commandIt->first + " " + commandIt->first + " = 0;\n";
        }
        else if ( ( commandIt->second.handle == "VkDevice" ) || hasParentHandle( commandIt->second.handle, "VkDevice" ) )
        {
          da += "        " + commandIt->first + " = PFN_" + commandIt->first + "( vkGetDeviceProcAddr( device, \"" + commandIt->first + "\" ) );\n";
          // if this is an alias'ed function, use it as a fallback for the original one
          if ( !commandIt->second.alias.empty() )
          {
            da += "        if ( !" + commandIt->second.alias + " ) " + commandIt->second.alias + " = " + commandIt->first + ";\n";
          }

          dm += "      PFN_" + commandIt->first + " " + commandIt->first + " = 0;\n";
          dmp += "      PFN_dummy " + commandIt->first + "_placeholder = 0;\n";
        }
        else
        {
          assert( ( commandIt->second.handle == "VkInstance" ) || hasParentHandle( commandIt->second.handle, "VkInstance" ) );

          // filter out vkGetInstanceProcAddr, as starting with Vulkan 1.2 it can resolve itself only (!) with an
          // instance nullptr !
          if ( command != "vkGetInstanceProcAddr" )
          {
            ia += "        " + commandIt->first + " = PFN_" + commandIt->first + "( vkGetInstanceProcAddr( instance, \"" + commandIt->first + "\" ) );\n";
            // if this is an alias'ed function, use it as a fallback for the original one
            if ( !commandIt->second.alias.empty() )
            {
              ia += "        if ( !" + commandIt->second.alias + " ) " + commandIt->second.alias + " = " + commandIt->first + ";\n";
            }
          }

          im += +"      PFN_" + commandIt->first + " " + commandIt->first + " = 0;\n";
          imp += "      PFN_dummy " + commandIt->first + "_placeholder = 0;\n";
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
    // check that a bitmask is referenced somewhere
    // I think, it's not forbidden to not reference a bitmask, but it would probably be not intended?
    auto typeIt = m_types.find( bitmask.first );
    assert( typeIt != m_types.end() );
    checkForError( !typeIt->second.referencedIn.empty(), bitmask.second.xmlLine, "bitmask <" + bitmask.first + "> not listed in any feature or extension" );

    // check that the requirement is an enum
    if ( !bitmask.second.requirements.empty() )
    {
      checkForError( m_enums.find( bitmask.second.requirements ) != m_enums.end(),
                     bitmask.second.xmlLine,
                     "bitmask requires unknown <" + bitmask.second.requirements + ">" );
    }
  }
}

void VulkanHppGenerator::checkCommandCorrectness() const
{
  // prepare command checks by gathering all result codes and aliases into one set of resultCodes
  auto resultIt = m_enums.find( "VkResult" );
  assert( resultIt != m_enums.end() );
  std::set<std::string> resultCodes;
  for ( auto rc : resultIt->second.values )
  {
    resultCodes.insert( rc.name );
  }
  for ( auto rc : resultIt->second.aliases )
  {
    resultCodes.insert( rc.first );
  }

  // command checks
  for ( auto const & command : m_commands )
  {
    // check that a command is referenced somewhere
    // I think, it's not forbidden to not reference a function, but it would probably be not intended?
    checkForError( !command.second.referencedIn.empty(), command.second.xmlLine, "command <" + command.first + "> not listed in any feature or extension" );

    // check for unknow error or succes codes
    for ( auto const & ec : command.second.errorCodes )
    {
      checkForError( resultCodes.find( ec ) != resultCodes.end(), command.second.xmlLine, "command uses unknown error code <" + ec + ">" );
    }
    for ( auto const & sc : command.second.successCodes )
    {
      checkForError( resultCodes.find( sc ) != resultCodes.end(), command.second.xmlLine, "command uses unknown success code <" + sc + ">" );
    }

    // check that functions returning a VkResult specify successcodes
    checkForError( ( command.second.returnType != "VkResult" ) || !command.second.successCodes.empty(),
                   command.second.xmlLine,
                   "missing successcodes on command <" + command.first + "> returning VkResult!" );

    // check that all parameter types as well as the return type are known types
    for ( auto const & p : command.second.params )
    {
      checkForError( m_types.find( p.type.type ) != m_types.end(), p.xmlLine, "comand uses parameter of unknown type <" + p.type.type + ">" );
    }
    checkForError( m_types.find( command.second.returnType ) != m_types.end(),
                   command.second.xmlLine,
                   "command uses unknown return type <" + command.second.returnType + ">" );
  }
}

void VulkanHppGenerator::checkCorrectness() const
{
  checkForError( !m_vulkanLicenseHeader.empty(), -1, "missing license header" );
  checkBitmaskCorrectness();
  checkCommandCorrectness();
  checkEnumCorrectness();
  checkExtensionCorrectness();
  checkFuncPointerCorrectness();
  checkHandleCorrectness();
  checkStructCorrectness();
}

void VulkanHppGenerator::checkEnumCorrectness() const
{
  for ( auto const & e : m_enums )
  {
    // check that a bitmask is referenced somewhere
    // it's not forbidden to not reference a bitmask, and in fact that happens! So just warn here
    auto typeIt = m_types.find( e.first );
    assert( typeIt != m_types.end() );
    checkForWarning( !typeIt->second.referencedIn.empty(), e.second.xmlLine, "enum <" + e.first + "> not listed in any feature or extension" );

    // check that the aliasNames are known enum values or known aliases
    for ( auto const & alias : e.second.aliases )
    {
      checkForError(
        ( std::find_if( e.second.values.begin(), e.second.values.end(), [&alias]( EnumValueData const & evd ) { return evd.name == alias.second.name; } ) !=
          e.second.values.end() ) ||
          ( e.second.aliases.find( alias.second.name ) != e.second.aliases.end() ),
        alias.second.xmlLine,
        "enum <" + alias.first + "> uses unknown alias <" + alias.second.name + ">" );
    }

    // check that any protection fits to the corresponding extension
    for ( auto const & v : e.second.values )
    {
      if ( !v.protect.empty() )
      {
        auto extIt = m_extensions.find( v.extension );
        assert( extIt != m_extensions.end() );
        auto platformIt = m_platforms.find( extIt->second.platform );
        assert( platformIt != m_platforms.end() );
        checkForError( v.protect == platformIt->second.protect,
                       v.xmlLine,
                       "attribute <protect> of enum value <" + v.name + "> is \"" + v.protect + "\" but corresponding extension <" + v.extension +
                         "> belongs to platform <" + platformIt->first + "> with protection \"" + platformIt->second.protect + "\"" );
      }
    }
  }

  // enum checks by features and extensions
  for ( auto & feature : m_features )
  {
    checkEnumCorrectness( feature.second.requireData );
  }
  for ( auto & ext : m_extensions )
  {
    checkEnumCorrectness( ext.second.requireData );
  }

  // special check for VkFormat
  if ( !m_formats.empty() )
  {
    auto enumIt = m_enums.find( "VkFormat" );
    assert( enumIt != m_enums.end() );
    assert( enumIt->second.values.front().name == "VK_FORMAT_UNDEFINED" );
    for ( auto enumValueIt = std::next( enumIt->second.values.begin() ); enumValueIt != enumIt->second.values.end(); ++enumValueIt )
    {
      auto formatIt = m_formats.find( enumValueIt->name );
      if ( formatIt == m_formats.end() )
      {
        auto aliasIt = std::find_if(
          enumIt->second.aliases.begin(), enumIt->second.aliases.end(), [&enumValueIt]( auto const & ead ) { return ead.second.name == enumValueIt->name; } );
        checkForError( aliasIt != enumIt->second.aliases.end(), enumValueIt->xmlLine, "missing format specification for <" + enumValueIt->name + ">" );
      }
    }
  }
}

void VulkanHppGenerator::checkEnumCorrectness( std::vector<RequireData> const & requireData ) const
{
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto typeIt = m_types.find( type );
      assert( typeIt != m_types.end() );
      switch ( typeIt->second.category )
      {
        case TypeCategory::Bitmask:
          {
            // check that each "require" listed for a bitmask is listed for a feature or an extension
            auto bitmaskIt = m_bitmasks.find( type );
            if ( bitmaskIt != m_bitmasks.end() )
            {
              // not for every bitmask is a "require" listed
              if ( !bitmaskIt->second.requirements.empty() )
              {
                auto requireTypeIt = m_types.find( bitmaskIt->second.requirements );
                assert( requireTypeIt != m_types.end() );
                checkForError( !requireTypeIt->second.referencedIn.empty(),
                               bitmaskIt->second.xmlLine,
                               "bitmask <" + bitmaskIt->first + "> requires <" + bitmaskIt->second.requirements +
                                 "> which is not listed for any feature or extension!" );
              }
            }
            else
            {
              // every bitmask not listed in the m_bitmasks, should be an alias of such a thing
              assert( std::find_if( m_bitmasks.begin(),
                                    m_bitmasks.end(),
                                    [&type]( std::pair<const std::string, BitmaskData> const & bd ) { return bd.second.alias == type; } ) != m_bitmasks.end() );
            }
          }
          break;
        case TypeCategory::Enum:
          {
            auto enumIt = m_enums.find( type );
            if ( enumIt != m_enums.end() )
            {
              if ( enumIt->second.isBitmask )
              {
                // check that any enum of a bitmask is listed as "require" or "bitvalues" for a bitmask
                auto bitmaskIt = std::find_if(
                  m_bitmasks.begin(), m_bitmasks.end(), [&enumIt]( auto const & bitmask ) { return bitmask.second.requirements == enumIt->first; } );
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
              assert( std::find_if( m_enums.begin(),
                                    m_enums.end(),
                                    [&type]( std::pair<const std::string, EnumData> const & ed ) { return ed.second.alias == type; } ) != m_enums.end() );
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
    size_t lenIdx = std::distance( constructorIt->second.params.begin(), lenIt );
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
  return ( std::find_if( constructorIts.begin(), constructorIts.end(), isEquivalentSingularConstructor ) != constructorIts.end() );
}

void VulkanHppGenerator::checkExtensionCorrectness() const
{
  for ( auto const & extension : m_extensions )
  {
    // check for existence of any deprecation, obsoletion, or promotion
    if ( !extension.second.deprecatedBy.empty() )
    {
      checkForError( ( m_extensions.find( extension.second.deprecatedBy ) != m_extensions.end() ) ||
                       ( m_features.find( extension.second.deprecatedBy ) != m_features.end() ),
                     extension.second.xmlLine,
                     "extension deprecated by unknown extension/version <" + extension.second.promotedTo + ">" );
    }
    if ( !extension.second.obsoletedBy.empty() )
    {
      checkForError( ( m_extensions.find( extension.second.obsoletedBy ) != m_extensions.end() ) ||
                       ( m_features.find( extension.second.obsoletedBy ) != m_features.end() ),
                     extension.second.xmlLine,
                     "extension obsoleted by unknown extension/version <" + extension.second.promotedTo + ">" );
    }
    if ( !extension.second.promotedTo.empty() )
    {
      checkForError( ( m_extensions.find( extension.second.promotedTo ) != m_extensions.end() ) ||
                       ( m_features.find( extension.second.promotedTo ) != m_features.end() ),
                     extension.second.xmlLine,
                     "extension promoted to unknown extension/version <" + extension.second.promotedTo + ">" );
    }

    // check for existence of any requirement
    for ( auto const & require : extension.second.requireData )
    {
      if ( !require.depends.empty() )
      {
        for ( auto const & depends : require.depends )
        {
          checkForError( ( m_features.find( depends ) != m_features.end() ) || ( m_extensions.find( depends ) != m_extensions.end() ),
                         require.xmlLine,
                         "extension <" + extension.first + "> lists an unknown depends <" + depends + ">" );
        }
      }
    }
  }
}

void VulkanHppGenerator::checkFuncPointerCorrectness() const
{
  for ( auto const & funcPointer : m_funcPointers )
  {
    if ( !funcPointer.second.requirements.empty() )
    {
      checkForError( m_types.find( funcPointer.second.requirements ) != m_types.end(),
                     funcPointer.second.xmlLine,
                     "funcpointer requires unknown <" + funcPointer.second.requirements + ">" );
    }
    for ( auto const & argument : funcPointer.second.arguments )
    {
      checkForError( m_types.find( argument.type ) != m_types.end(), argument.xmlLine, "funcpointer argument of unknown type <" + argument.type + ">" );
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
    checkForError( m_handles.find( handle.second.parent ) != m_handles.end(),
                   handle.second.xmlLine,
                   "handle <" + handle.first + "> with unknown parent <" + handle.second.parent + ">" );

    // check existence of objTypeEnum used with this handle type
    if ( !handle.first.empty() )
    {
      assert( !handle.second.objTypeEnum.empty() );
      checkForError( std::find_if( objectTypeIt->second.values.begin(),
                                   objectTypeIt->second.values.end(),
                                   [&handle]( EnumValueData const & evd )
                                   { return evd.name == handle.second.objTypeEnum; } ) != objectTypeIt->second.values.end(),
                     handle.second.xmlLine,
                     "handle <" + handle.first + "> specifies unknown \"objtypeenum\" <" + handle.second.objTypeEnum + ">" );
    }
  }

  // check that all specified objectType values are used with a handle type
  for ( auto const & objectTypeValue : objectTypeIt->second.values )
  {
    if ( objectTypeValue.name != "VK_OBJECT_TYPE_UNKNOWN" )
    {
      checkForError( std::find_if( m_handles.begin(),
                                   m_handles.end(),
                                   [&objectTypeValue]( std::pair<std::string, HandleData> const & hd )
                                   { return hd.second.objTypeEnum == objectTypeValue.name; } ) != m_handles.end(),
                     objectTypeValue.xmlLine,
                     "VkObjectType value <" + objectTypeValue.name + "> not specified as \"objtypeenum\" for any handle" );
    }
  }
}

void VulkanHppGenerator::checkStructCorrectness() const
{
  for ( auto const & structAlias : m_structureAliases )
  {
    auto structIt = m_structures.find( structAlias.second.alias );
    checkForError( structIt != m_structures.end(), structAlias.second.xmlLine, "unknown struct alias <" + structAlias.second.alias + ">" );
  }

  for ( auto const & structAliasInverse : m_structureAliasesInverse )
  {
    auto structIt = m_structures.find( structAliasInverse.first );
    if ( structIt == m_structures.end() )
    {
      assert( !structAliasInverse.second.empty() );
      auto aliasIt = m_structureAliases.find( *structAliasInverse.second.begin() );
      assert( aliasIt != m_structureAliases.end() );
      checkForError( false, aliasIt->second.xmlLine, "struct <" + aliasIt->first + "> uses unknown alias <" + aliasIt->second.alias + ">" );
    }
  }

  std::set<std::string> sTypeValues;
  for ( auto const & structure : m_structures )
  {
    // check that a struct is referenced somewhere
    // I think, it's not forbidden to not reference a struct, but it would probably be not intended?
    auto typeIt = m_types.find( structure.first );
    assert( typeIt != m_types.end() );
    checkForError(
      !typeIt->second.referencedIn.empty(), structure.second.xmlLine, "structure <" + structure.first + "> not listed in any feature or extension" );

    // check for existence of all structs that are extended by this struct
    for ( auto const & extend : structure.second.structExtends )
    {
      checkForError( ( m_structures.find( extend ) != m_structures.end() ) || ( m_structureAliases.find( extend ) != m_structureAliases.end() ),
                     structure.second.xmlLine,
                     "struct <" + structure.first + "> extends unknown <" + extend + ">" );
    }

    // checks on the members of a struct
    checkStructMemberCorrectness( structure.first, structure.second.members, sTypeValues );
  }

  // enum VkStructureType checks (need to be after structure checks because of sTypeValues gathered there)
  auto structureTypeIt = m_enums.find( "VkStructureType" );
  assert( structureTypeIt != m_enums.end() );
  for ( auto const & enumValue : structureTypeIt->second.values )
  {
    if ( ( enumValue.name == "VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO" ) || ( enumValue.name == "VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO" ) )
    {
      checkForError(
        sTypeValues.find( enumValue.name ) == sTypeValues.end(), enumValue.xmlLine, "Reserved VkStructureType enum value <" + enumValue.name + "> is used" );
    }
    else
    {
      checkForError( sTypeValues.erase( enumValue.name ) == 1, enumValue.xmlLine, "VkStructureType enum value <" + enumValue.name + "> never used" );
    }
  }
  assert( sTypeValues.empty() );
}

void VulkanHppGenerator::checkStructMemberCorrectness( std::string const &             structureName,
                                                       std::vector<MemberData> const & members,
                                                       std::set<std::string> &         sTypeValues ) const
{
  for ( auto const & member : members )
  {
    // check that all member types are required in some feature or extension
    checkForError(
      !member.type.type.starts_with( "Vk" ) ||
        ( std::find_if( m_features.begin(),
                        m_features.end(),
                        [&member]( std::pair<std::string, FeatureData> const & fd )
                        {
                          return std::find_if( fd.second.requireData.begin(),
                                               fd.second.requireData.end(),
                                               [&member]( RequireData const & rd ) {
                                                 return std::find_if( rd.types.begin(),
                                                                      rd.types.end(),
                                                                      [&member]( std::string const & t ) { return t == member.type.type; } ) != rd.types.end();
                                               } ) != fd.second.requireData.end();
                        } ) != m_features.end() ) ||
        ( std::find_if( m_extensions.begin(),
                        m_extensions.end(),
                        [&member]( std::pair<std::string, ExtensionData> const & ed )
                        {
                          return std::find_if( ed.second.requireData.begin(),
                                               ed.second.requireData.end(),
                                               [&member]( RequireData const & rd ) {
                                                 return std::find_if( rd.types.begin(),
                                                                      rd.types.end(),
                                                                      [&member]( std::string const & t ) { return t == member.type.type; } ) != rd.types.end();
                                               } ) != ed.second.requireData.end();
                        } ) != m_extensions.end() ),
      member.xmlLine,
      "struct member type <" + member.type.type + "> used in struct <" + structureName + "> is never listed for any feature or extension" );

    // if a member specifies a selector, that member is a union and the selector is an enum
    // check that there's a 1-1 connection between the specified selections and the values of that enum
    if ( !member.selector.empty() )
    {
      auto selectorIt = findStructMemberIt( member.selector, members );
      assert( selectorIt != members.end() );
      auto selectorEnumIt = m_enums.find( selectorIt->type.type );
      assert( selectorEnumIt != m_enums.end() );
      auto unionIt = m_structures.find( member.type.type );
      assert( ( unionIt != m_structures.end() ) && unionIt->second.isUnion );
      for ( auto const & unionMember : unionIt->second.members )
      {
        // check that each union member has a selection, that is a value of the seleting enum
        assert( !unionMember.selection.empty() );
        for ( auto const & selection : unionMember.selection )
        {
          checkForError( std::find_if( selectorEnumIt->second.values.begin(),
                                       selectorEnumIt->second.values.end(),
                                       [&selection]( EnumValueData const & evd ) { return evd.name == selection; } ) != selectorEnumIt->second.values.end(),
                         unionMember.xmlLine,
                         "union member <" + unionMember.name + "> uses selection <" + selection + "> that is not part of the selector type <" +
                           selectorIt->type.type + ">" );
        }
      }
    }

    // check that each member type is known
    checkForError( m_types.find( member.type.type ) != m_types.end(), member.xmlLine, "struct member uses unknown type <" + member.type.type + ">" );

    // check that any used constant is a known constant
    if ( !member.usedConstant.empty() )
    {
      checkForError( m_constants.find( member.usedConstant ) != m_constants.end(),
                     member.xmlLine,
                     "struct member array size uses unknown constant <" + member.usedConstant + ">" );
    }

    // checks if a value is specified
    if ( !member.value.empty() )
    {
      auto enumIt = m_enums.find( member.type.type );
      if ( enumIt != m_enums.end() )
      {
        // check that the value exists in the specified enum
        checkForError( std::find_if( enumIt->second.values.begin(),
                                     enumIt->second.values.end(),
                                     [&member]( auto const & evd ) { return member.value == evd.name; } ) != enumIt->second.values.end(),
                       member.xmlLine,
                       "value <" + member.value + "> for member <" + member.name + "> in structure <" + structureName + "> of enum type <" + member.type.type +
                         "> not listed" );
        // special handling for sType: no value should appear more than once
        if ( member.name == "sType" )
        {
          checkForError( sTypeValues.insert( member.value ).second, member.xmlLine, "sType value <" + member.value + "> has been used before" );
        }
      }
      else if ( member.type.type == "uint32_t" )
      {
        // check that a value for a uint32_t is all digits
        checkForError( member.value.find_first_not_of( "0123456789" ) == std::string::npos,
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

void VulkanHppGenerator::checkValidStructs( int line, std::map<std::string, std::string> const & attributes ) const
{
  auto it = attributes.find( "validstructs" );
  if ( it != attributes.end() )
  {
    std::vector<std::string> validStructs = tokenize( it->second, "," );
    for ( auto const & vs : validStructs )
    {
      checkForError( m_structures.find( vs ) != m_structures.end(), line, "unknown struct <" + vs + "> listed in attribute <validstructs>" );
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
      assert( !enumerating || ( ( vectorParams.find( returnParams[1] ) != vectorParams.end() ) &&
                                ( vectorParams.find( returnParams[1] )->second.lenParam == returnParams[0] ) ) );
      combinedType = enumerating ? modifiedDataTypes[1] : ( "std::pair<" + modifiedDataTypes[0] + ", " + modifiedDataTypes[1] + ">" );
      break;
    case 3:
      assert( enumerating && ( vectorParams.size() == 2 ) && ( vectorParams.begin()->first == returnParams[1] ) &&
              ( vectorParams.begin()->second.lenParam == returnParams[0] ) && ( std::next( vectorParams.begin() )->first == returnParams[2] ) &&
              ( std::next( vectorParams.begin() )->second.lenParam == returnParams[0] ) );
      combinedType = "std::pair<" + modifiedDataTypes[1] + ", " + modifiedDataTypes[2] + ">";
      break;
    default: assert( false ); break;
  }
  return combinedType;
}

bool VulkanHppGenerator::containsArray( std::string const & type ) const
{
  // a simple recursive check if a type is or contains an array
  auto structureIt = m_structures.find( type );
  bool found       = false;
  if ( structureIt != m_structures.end() )
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
  auto structureIt = m_structures.find( type );
  bool found       = false;
  if ( structureIt != m_structures.end() )
  {
    for ( auto memberIt = structureIt->second.members.begin(); memberIt != structureIt->second.members.end() && !found; ++memberIt )
    {
      found = ( m_funcPointers.find( memberIt->type.type ) != m_funcPointers.end() ) ||
              ( ( memberIt->type.type != type ) && containsFuncPointer( memberIt->type.type ) );
    }
  }
  return found;
}

bool VulkanHppGenerator::containsFloatingPoints( std::vector<MemberData> const & members ) const
{
  for ( auto const & m : members )
  {
    if ( ( ( m.type.type == "float" ) || ( m.type.type == "double" ) ) && m.type.isValue() )
    {
      return true;
    }
  }
  return false;
}

bool VulkanHppGenerator::containsUnion( std::string const & type ) const
{
  // a simple recursive check if a type is or contains a union
  auto structureIt = m_structures.find( type );
  bool found       = false;
  if ( structureIt != m_structures.end() )
  {
    found = structureIt->second.isUnion;
    for ( auto memberIt = structureIt->second.members.begin(); memberIt != structureIt->second.members.end() && !found; ++memberIt )
    {
      found = memberIt->type.isValue() && containsUnion( memberIt->type.type );
    }
  }
  return found;
}

std::vector<size_t> VulkanHppGenerator::determineConstPointerParams( std::vector<ParamData> const & params ) const
{
  std::vector<size_t> constPointerParams;

  for ( size_t i = 0; i < params.size(); i++ )
  {
    // very special handling for some types, which come in as non-const pointers, but are meant as const-pointers
    if ( params[i].type.isConstPointer() ||
         ( params[i].type.isNonConstPointer() && ( specialPointerTypes.find( params[i].type.type ) != specialPointerTypes.end() ) ) )
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
    if ( templatedParams.find( rp ) != templatedParams.end() )
    {
      auto vectorParamIt = vectorParams.find( rp );
      if ( ( vectorParamIt != vectorParams.end() ) && ( std::find( returnParams.begin(), returnParams.end(), vectorParamIt->first ) != returnParams.end() ) &&
           ( std::find( returnParams.begin(), returnParams.end(), vectorParamIt->second.lenParam ) != returnParams.end() ) )
      {
        dataTypes.push_back( "uint8_t" );
      }
      else
      {
        dataTypes.push_back( ( stripPrefix( params[rp].name, "p" ) + "Type" ) );
      }
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
  for ( int i = static_cast<int>( params.size() ) - 1; ( 0 <= i ) && ( params[i].optional || ( skippedParams.find( i ) != skippedParams.end() ) ); --i )
  {
    defaultStartIndex = i;
  }
  return defaultStartIndex;
}

bool VulkanHppGenerator::determineEnumeration( std::map<size_t, VectorParamData> const & vectorParams, std::vector<size_t> const & returnParams ) const
{
  // a command is considered to be enumerating some data, if for at least one vectorParam both, the data and the counter, are returnParams
  return std::find_if( vectorParams.begin(),
                       vectorParams.end(),
                       [&returnParams]( auto const & vp )
                       {
                         return std::find( returnParams.begin(), returnParams.end(), vp.first ) != returnParams.end() &&
                                std::find( returnParams.begin(), returnParams.end(), vp.second.lenParam ) != returnParams.end();
                       } ) != vectorParams.end();
}

size_t VulkanHppGenerator::determineInitialSkipCount( std::string const & command ) const
{
  // determine the number of arguments to skip for a function
  // -> 0: the command is not bound to an instance or a device (the corresponding handle has no name)
  // -> 1: the command bound to an instance or a device (the corresponding handle has a name)
  // -> 2: the command has been moved to a second handle
  auto commandIt = m_commands.find( command );
  assert( commandIt != m_commands.end() );
  auto handleIt = m_handles.find( commandIt->second.handle );
  assert( handleIt != m_handles.end() );
  if ( handleIt->second.commands.find( command ) == handleIt->second.commands.end() )
  {
    assert( 1 < commandIt->second.params.size() );
    assert( m_handles.find( commandIt->second.params[1].type.type ) != m_handles.end() );
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
    if ( params[i].type.isNonConstPointer() && ( specialPointerTypes.find( params[i].type.type ) == specialPointerTypes.end() ) )
    {
      nonConstPointerParamIndices.push_back( i );
    }
  }
  return nonConstPointerParamIndices;
}

std::vector<std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator>
  VulkanHppGenerator::determineRAIIHandleConstructors( std::string const & handleType, std::map<std::string, CommandData>::const_iterator destructorIt ) const
{
  std::vector<std::map<std::string, CommandData>::const_iterator> constructorIts;
  auto                                                            isConstructorCandidate = [&handleType]( std::pair<std::string, CommandData> const & cd )
  {
    return std::find_if( cd.second.params.begin(),
                         cd.second.params.end(),
                         [&handleType]( ParamData const & pd )
                         { return ( pd.type.type == handleType ) && pd.type.isNonConstPointer(); } ) != cd.second.params.end();
  };
  for ( auto commandIt = m_commands.begin(); commandIt != m_commands.end(); )
  {
    // find the commands that get a non-const pointer to the handleType, that is, return a handle type
    commandIt = std::find_if( commandIt, m_commands.end(), isConstructorCandidate );
    if ( commandIt != m_commands.end() )
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
              auto structureIt = m_structures.find( pd.type.type );
              return ( structureIt != m_structures.end() ) &&
                     ( findStructMemberItByType( destructorParam.type.type, structureIt->second.members ) != structureIt->second.members.end() );
            }
            return true;
          };

          return ( destructorParam.name == desctructorHandleParamIt->len ) ||
                 ( std::find_if( commandIt->second.params.begin(), commandIt->second.params.end(), isDestructorParamType ) != commandIt->second.params.end() );
        };

        // the constructor candidate is valid, if none of the (relevant) destructor parameters is missing in the
        // constructor candidate params
        valid = ( std::find_if_not( destructorIt->second.params.begin(), destructorIt->second.params.end(), isConstructorCandidateParam ) ==
                  destructorIt->second.params.end() );
      }
      if ( valid )
      {
        // filter out alias functions
        if ( commandIt->second.alias.empty() )
        {
          constructorIts.push_back( commandIt );
        }
      }
      ++commandIt;
    }
  }
  assert( !constructorIts.empty() );
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
    assert( !params[vpi.first].len.empty() );
    if ( ( ( std::find_if( returnParams.begin(), returnParams.end(), [&vpi]( size_t rpi ) { return vpi.first == rpi; } ) == returnParams.end() ) &&
           isParam( params[vpi.first].len, params ) ) ||
         ( singular && params[vpi.second.lenParam].type.isValue() ) )
    {
      skippedParams.insert( vpi.second.lenParam );
    }
    if ( vpi.second.strideParam != INVALID_INDEX )
    {
      skippedParams.insert( vpi.second.strideParam );
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
    auto isSubStruct = [&structure]( std::pair<std::string, StructureData> const & sd )
    {
      // member-by-member comparison of type and name
      auto memberIt = structure.second.members.begin();
      auto isMember = [&memberIt]( MemberData const & md )
      {
        if ( ( md.type == memberIt->type ) && ( md.name == memberIt->name ) )
        {
          ++memberIt;
          return true;
        }
        return false;
      };

      return ( sd.second.members.size() < structure.second.members.size() ) &&
             ( std::find_if_not( sd.second.members.begin(), sd.second.members.end(), isMember ) == sd.second.members.end() );
    };

    // look for a struct in m_structures that starts identically to structure
    auto structIt = std::find_if( m_structures.begin(), m_structures.end(), isSubStruct );
    return ( structIt == m_structures.end() ) ? "" : structIt->first;
  }
  return "";
}

std::map<size_t, VulkanHppGenerator::VectorParamData> VulkanHppGenerator::determineVectorParams( std::vector<ParamData> const & params ) const
{
  std::map<size_t, VectorParamData> vectorParams;

  // look for the parameters whose len equals the name of an other parameter
  for ( size_t i = 0; i < params.size(); i++ )
  {
    if ( !params[i].len.empty() && ( params[i].len != "null-terminated" ) )
    {
      VectorParamData & vpd = vectorParams[i];

      std::string len;
      if ( altLens.find( params[i].len ) != altLens.end() )
      {
        checkForError( params[i].len == "(samples + 31) / 32", params[i].xmlLine, "unknown command parameter len <" + params[i].len + ">" );
        len = "samples";
      }
      else
      {
        len = params[i].len;
      }
      auto lenIt =
        std::find_if( params.begin(), params.end(), [&len, this]( auto const & pd ) { return ( len == pd.name ) || isLenByStructMember( len, pd ); } );
      assert( lenIt != params.end() );
      vpd.lenParam = std::distance( params.begin(), lenIt );
      if ( !params[i].stride.empty() )
      {
        std::string const & stride   = params[i].stride;
        auto                strideIt = std::find_if( params.begin(), params.end(), [&stride]( auto const & pd ) { return stride == pd.name; } );
        assert( strideIt != params.end() );
        vpd.strideParam = std::distance( params.begin(), strideIt );
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

void VulkanHppGenerator::distributeSecondLevelCommands( std::set<std::string> const & specialFunctions )
{
  // distribute commands from instance/device to second-level handles, like Queue, Event,... for RAII handles
  for ( auto & handle : m_handles )
  {
    if ( !handle.first.empty() )
    {
      for ( auto command = handle.second.commands.begin(); command != handle.second.commands.end(); )
      {
        bool foundCommand = false;
        if ( specialFunctions.find( *command ) == specialFunctions.end() )
        {
          auto commandIt = m_commands.find( *command );
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
              assert( std::find_if( handleIt->second.constructorIts.begin(),
                                    handleIt->second.constructorIts.end(),
                                    [&handle]( auto const & constructorIt )
                                    { return constructorIt->second.handle != handle.first; } ) == handleIt->second.constructorIts.end() );
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

std::string VulkanHppGenerator::findBaseName( std::string aliasName, std::map<std::string, EnumAliasData> const & aliases ) const
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

std::pair<std::string, std::string> VulkanHppGenerator::generateAllocatorTemplates( std::vector<size_t> const &               returnParams,
                                                                                    std::vector<std::string> const &          returnDataTypes,
                                                                                    std::map<size_t, VectorParamData> const & vectorParams,
                                                                                    CommandFlavourFlags                       flavourFlags,
                                                                                    bool                                      definition ) const
{
  bool chained  = flavourFlags & CommandFlavourFlagBits::chained;
  bool singular = flavourFlags & CommandFlavourFlagBits::singular;
  bool unique   = flavourFlags & CommandFlavourFlagBits::unique;

  assert( returnParams.size() == returnDataTypes.size() );
  std::string allocatorTemplates;
  if ( !singular )
  {
    for ( size_t i = 0; i < returnParams.size(); i++ )
    {
      if ( vectorParams.find( returnParams[i] ) != vectorParams.end() )
      {
        if ( chained )
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
                                                              bool                                      definition,
                                                              CommandFlavourFlags                       flavourFlags,
                                                              bool                                      withDispatcher ) const
{
  bool withAllocators = flavourFlags & CommandFlavourFlagBits::withAllocator;

  size_t defaultStartIndex = withAllocators ? ~0 : determineDefaultStartIndex( params, skippedParams );

  std::string argumentList;
  bool        encounteredArgument = false;
  for ( size_t i = 0; i < params.size(); ++i )
  {
    if ( skippedParams.find( i ) == skippedParams.end() )
    {
      if ( encounteredArgument )
      {
        argumentList += ", ";
      }
      bool hasDefaultAssignment = false;

      std::string composedType = params[i].type.compose( "VULKAN_HPP_NAMESPACE" );

      if ( singularParams.find( i ) != singularParams.end() )
      {
        assert( !params[i].optional );
        assert( params[i].type.isConstPointer() && !params[i].len.empty() && !isLenByStructMember( params[i].len, params ) &&
                params[i].type.type.starts_with( "Vk" ) );
        assert( !isHandleType( params[i].type.type ) );
        assert( composedType.ends_with( " *" ) );
        argumentList += stripPostfix( composedType, " *" ) + " & " + stripPluralS( startLowerCase( stripPrefix( params[i].name, "p" ) ), m_tags );
      }
      else if ( params[i].type.isConstPointer() )
      {
        assert( composedType.ends_with( " *" ) );
        std::string name = startLowerCase( stripPrefix( params[i].name, "p" ) );
        if ( params[i].len.empty() )
        {
          assert( withDispatcher || !isHandleType( params[i].type.type ) );
          assert( !params[i].type.prefix.empty() && ( params[i].type.postfix == "*" ) );
          assert( params[i].arraySizes.empty() );
          if ( params[i].type.type == "void" )
          {
            argumentList += ( templatedParams.find( i ) == templatedParams.end() ) ? ( composedType + " " + params[i].name )
                                                                                   : ( stripPrefix( params[i].name, "p" ) + "Type const & " + name );
          }
          else if ( params[i].optional )
          {
            argumentList += "Optional<" + stripPostfix( composedType, " *" ) + "> " + name +
                            ( ( definition || withAllocators ) ? "" : " VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT" );
            hasDefaultAssignment = true;
          }
          else
          {
            argumentList += stripPostfix( composedType, " *" ) + " & " + name;
          }
        }
        else
        {
          // a const-pointer with a non-empty len is either null-terminated (aka a string) or represented by an
          // ArrayProxy
          assert( params[i].arraySizes.empty() );
          if ( params[i].len == "null-terminated" )
          {
            assert( params[i].type.type == "char" );
            if ( params[i].optional )
            {
              argumentList +=
                "Optional<const std::string> " + name + ( ( definition || withAllocators ) ? "" : " VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT" );
              hasDefaultAssignment = true;
            }
            else
            {
              argumentList += "const std::string & " + name;
            }
          }
          else
          {
            // an ArrayProxy also covers no data, so any optional flag can be ignored here
            std::string type = stripPostfix( composedType, " *" );
            size_t      pos  = type.find( "void" );
            if ( pos != std::string::npos )
            {
              type.replace( pos, 4, stripPrefix( params[i].name, "p" ) + "Type" );
            }
            argumentList +=
              std::string( "VULKAN_HPP_NAMESPACE::" ) + ( params[i].stride.empty() ? "" : "Strided" ) + "ArrayProxy<" + type + "> const & " + name;
            if ( params[i].optional && !definition )
            {
              assert( params[i].stride.empty() );
              argumentList += " VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT";
              hasDefaultAssignment = true;
            }
          }
        }
      }
      else if ( params[i].type.isNonConstPointer() )
      {
        assert( withDispatcher || !isHandleType( params[i].type.type ) );
        assert( params[i].len.empty() && !params[i].optional );
        assert( composedType.ends_with( " *" ) );
        argumentList += stripPostfix( composedType, " *" ) + " & " + params[i].name;
      }
      else
      {
        assert( params[i].type.isValue() );
        argumentList += composedType + " " + params[i].name + generateCArraySizes( params[i].arraySizes );
      }
      argumentList += std::string( !definition && ( defaultStartIndex <= i ) && !hasDefaultAssignment ? " VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT" : "" );
      encounteredArgument = true;
    }
  }
  if ( withAllocators )
  {
    if ( flavourFlags & CommandFlavourFlagBits::chained )
    {
      if ( encounteredArgument )
      {
        argumentList += ", ";
      }
      argumentList += "StructureChainAllocator & structureChainAllocator";
      encounteredArgument = true;
    }
    else
    {
      for ( auto sp : skippedParams )
      {
        if ( !params[sp].len.empty() )
        {
          if ( encounteredArgument )
          {
            argumentList += ", ";
          }
          std::string type;
          if ( templatedParams.find( sp ) != templatedParams.end() )
          {
            auto vectorParamIt = vectorParams.find( sp );
            if ( ( vectorParamIt != vectorParams.end() ) &&
                 ( std::find( returnParams.begin(), returnParams.end(), vectorParamIt->first ) != returnParams.end() ) &&
                 ( std::find( returnParams.begin(), returnParams.end(), vectorParamIt->second.lenParam ) != returnParams.end() ) )
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
          argumentList += type + "Allocator & " + startLowerCase( type ) + "Allocator";
          encounteredArgument = true;
        }
      }
    }
  }
  if ( withDispatcher )
  {
    if ( encounteredArgument )
    {
      argumentList += ", ";
    }
    argumentList += std::string( "Dispatch const & d" ) + ( definition ? "" : " VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT" );
  }
  return argumentList;
}

std::string VulkanHppGenerator::generateArgumentListStandard( std::vector<ParamData> const & params, std::set<size_t> const & skippedParams ) const
{
  std::string argumentList;
  for ( size_t i = 0; i < params.size(); ++i )
  {
    if ( skippedParams.find( i ) == skippedParams.end() )
    {
      argumentList += params[i].type.compose( "VULKAN_HPP_NAMESPACE" ) + " " + params[i].name + generateCArraySizes( params[i].arraySizes ) + ", ";
    }
  }
  argumentList += "Dispatch const & d ";
  return argumentList;
}

std::string VulkanHppGenerator::generateArgumentTemplates( std::vector<ParamData> const &            params,
                                                           std::vector<size_t> const &               returnParams,
                                                           std::map<size_t, VectorParamData> const & vectorParams,
                                                           std::set<size_t> const &                  templatedParams,
                                                           CommandFlavourFlags                       flavourFlags,
                                                           bool                                      raii ) const
{
  std::string argumentTemplates;
  if ( !templatedParams.empty() )
  {
    assert( !( flavourFlags & CommandFlavourFlagBits::chained ) );
    for ( auto t : templatedParams )
    {
      assert( params[t].name.starts_with( "p" ) );
      auto vectorParamIt = vectorParams.find( t );
      if ( ( vectorParamIt == vectorParams.end() ) || ( std::find( returnParams.begin(), returnParams.end(), vectorParamIt->first ) == returnParams.end() ) ||
           ( std::find( returnParams.begin(), returnParams.end(), vectorParamIt->second.lenParam ) == returnParams.end() ) )
      {
        // only templated parameters that are not part of an enumeration are really templated
        argumentTemplates += "typename " + stripPrefix( params[t].name, "p" ) + "Type, ";
      }
    }
  }
  else if ( flavourFlags & CommandFlavourFlagBits::chained )
  {
    argumentTemplates = ( returnParams.size() == 1 ) ? "typename X, typename Y, typename... Z, " : "typename StructureChain, ";
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
    // filter out VkFlags and VkFlags64, as they are mapped to our own Flags class
    if ( ( baseType.first != "VkFlags" ) && ( baseType.first != "VkFlags64" ) )
    {
      basetypes += "  using " + stripPrefix( baseType.first, "Vk" ) + " = " + baseType.second.typeInfo.compose( "VULKAN_HPP_NAMESPACE" ) + ";\n";
    }
  }

  return replaceWithMap( basetypesTemplate, { { "basetypes", basetypes } } );
}

std::string VulkanHppGenerator::generateBitmask( std::map<std::string, BitmaskData>::const_iterator bitmaskIt, std::string const & surroundingProtect ) const
{
  auto bitmaskBitsIt = m_enums.find( bitmaskIt->second.requirements );
  assert( bitmaskBitsIt != m_enums.end() );

  std::string bitmaskName = stripPrefix( bitmaskIt->first, "Vk" );
  std::string enumName    = stripPrefix( bitmaskBitsIt->first, "Vk" );
  std::string alias       = bitmaskIt->second.alias.empty() ? "" : ( "  using " + stripPrefix( bitmaskIt->second.alias, "Vk" ) + " = " + bitmaskName + ";\n" );

  std::string allFlags;
  if ( bitmaskBitsIt->second.values.empty() )
  {
    allFlags = " {};";
  }
  else
  {
    bool        encounteredFlag = false;
    std::string previousEnter, previousLeave;
    for ( auto const & value : bitmaskBitsIt->second.values )
    {
      // determine the values protect, if any
      std::string valueProtect = getProtect( value );

      // if the value's protect differs from the surrounding protect, generate protection code
      std::string enter, leave;
      if ( !valueProtect.empty() && ( valueProtect != surroundingProtect ) )
      {
        tie( enter, leave ) = generateProtection( valueProtect );
      }
      std::string valueName = generateEnumValueName( bitmaskBitsIt->first, value.name, true, m_tags );
      allFlags +=
        ( ( previousEnter != enter ) ? ( "\n" + previousLeave + enter ) : "\n" ) + "        " + ( encounteredFlag ? "| " : "  " ) + enumName + "::" + valueName;
      encounteredFlag = true;
      previousEnter   = enter;
      previousLeave   = leave;
    }
    if ( !previousLeave.empty() )
    {
      allFlags += "\n" + previousLeave;
    }
    allFlags += ";";
  }

  static const std::string bitmaskTemplate = R"(
  using ${bitmaskName} = Flags<${enumName}>;
${alias}

  template <> struct FlagTraits<${enumName}>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ${bitmaskName} allFlags = ${allFlags}
  };
)";

  return replaceWithMap( bitmaskTemplate, { { "alias", alias }, { "allFlags", allFlags }, { "bitmaskName", bitmaskName }, { "enumName", enumName } } );
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
    bitmasksToString += generateBitmasksToString( feature.second.requireData, listedBitmasks, feature.first );
  }
  for ( auto const & extIt : m_extensionsByNumber )
  {
    bitmasksToString += generateBitmasksToString( extIt.second->second.requireData, listedBitmasks, extIt.second->first );
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
      auto bitmaskIt = m_bitmasks.find( type );
      if ( ( bitmaskIt != m_bitmasks.end() ) && ( listedBitmasks.find( type ) == listedBitmasks.end() ) )
      {
        listedBitmasks.insert( type );
        str += generateBitmaskToString( bitmaskIt );
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateBitmaskToString( std::map<std::string, BitmaskData>::const_iterator bitmaskIt ) const
{
  auto bitmaskBitsIt = m_enums.find( bitmaskIt->second.requirements );
  assert( bitmaskBitsIt != m_enums.end() );

  std::string bitmaskName = stripPrefix( bitmaskIt->first, "Vk" );
  std::string enumName    = stripPrefix( bitmaskBitsIt->first, "Vk" );

  std::string str;
  if ( bitmaskBitsIt->second.values.empty() )
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
      return "{}";

    std::string result;
${toStringChecks}
    return "{ " + result.substr( 0, result.size() - 3 ) + " }";
  }
)";

    std::string toStringChecks;
    std::string previousEnter, previousLeave;
    for ( auto const & value : bitmaskBitsIt->second.values )
    {
      auto [enter, leave]   = generateProtection( getProtect( value ) );
      std::string valueName = generateEnumValueName( bitmaskBitsIt->first, value.name, true, m_tags );
      if ( value.singleBit )
      {
        toStringChecks += ( ( previousEnter != enter ) ? ( previousLeave + enter ) : "" ) + "    if ( value & " + enumName + "::" + valueName +
                          " ) result += \"" + valueName.substr( 1 ) + " | \";\n";
      }
      previousEnter = enter;
      previousLeave = leave;
    }
    if ( !previousLeave.empty() )
    {
      assert( previousLeave.ends_with( "\n" ) );
      toStringChecks += previousLeave;
      previousLeave.resize( previousLeave.size() - strlen( "\n" ) );
    }

    str += replaceWithMap( bitmaskToStringTemplate, { { "bitmaskName", bitmaskName }, { "toStringChecks", toStringChecks } } );
  }

  return str;
}

std::string VulkanHppGenerator::generateCallArgumentsEnhanced( CommandData const &      commandData,
                                                               size_t                   initialSkipCount,
                                                               bool                     nonConstPointerAsNullptr,
                                                               std::set<size_t> const & singularParams,
                                                               std::set<size_t> const & templatedParams,
                                                               bool                     raiiHandleMemberFunction ) const
{
  assert( initialSkipCount <= commandData.params.size() );
  std::string arguments;
  bool        encounteredArgument = false;
  if ( raiiHandleMemberFunction )
  {
    switch ( initialSkipCount )
    {
      case 1:
        assert( isHandleType( commandData.params[0].type.type ) && commandData.params[0].type.isValue() );
        assert( commandData.params[0].arraySizes.empty() && commandData.params[0].len.empty() );
        assert( commandData.params[0].type.type == commandData.handle );
        arguments           = "static_cast<" + commandData.handle + ">( m_" + startLowerCase( stripPrefix( commandData.handle, "Vk" ) ) + " )";
        encounteredArgument = true;
        break;
      case 2:
        {
          assert( isHandleType( commandData.params[0].type.type ) && commandData.params[0].type.isValue() );
          assert( commandData.params[0].arraySizes.empty() && commandData.params[0].len.empty() );
          assert( commandData.params[0].type.type == commandData.handle );
          auto handleIt = m_handles.find( commandData.params[1].type.type );
          assert( handleIt != m_handles.end() );
          arguments = "static_cast<" + commandData.handle + ">( m_" + startLowerCase( stripPrefix( commandData.handle, "Vk" ) ) + " )";

          assert( commandData.params[1].type.isValue() && commandData.params[1].arraySizes.empty() && commandData.params[1].len.empty() );
          arguments += ", static_cast<" + commandData.params[1].type.type + ">( m_" +
                       generateRAIIHandleConstructorParamName( handleIt->first, handleIt->second.destructorIt ) + " )";
          encounteredArgument = true;
        }
        break;
    }
  }
  else
  {
    for ( size_t i = 0; i < initialSkipCount; ++i )
    {
      if ( encounteredArgument )
      {
        arguments += ", ";
      }
      assert( isHandleType( commandData.params[i].type.type ) && commandData.params[i].type.isValue() );
      assert( commandData.params[i].arraySizes.empty() && commandData.params[i].len.empty() );
      arguments += "m_" + startLowerCase( stripPrefix( commandData.params[i].type.type, "Vk" ) );
      encounteredArgument = true;
    }
  }
  for ( size_t i = initialSkipCount; i < commandData.params.size(); ++i )
  {
    if ( encounteredArgument )
    {
      arguments += ", ";
    }
    arguments += generateCallArgumentEnhanced( commandData.params, i, nonConstPointerAsNullptr, singularParams, templatedParams );
    encounteredArgument = true;
  }
  return arguments;
}

std::string VulkanHppGenerator::generateCallArgumentsRAIIFactory( std::vector<ParamData> const & params,
                                                                  size_t                         initialSkipCount,
                                                                  std::set<size_t> const &       skippedParams,
                                                                  std::set<size_t> const &       singularParams ) const
{
  assert( initialSkipCount <= params.size() );
  std::string arguments = "*this";
  // skip the last parameter!
  for ( size_t i = initialSkipCount; i < params.size() - 1; ++i )
  {
    if ( skippedParams.find( i ) == skippedParams.end() )
    {
      std::string argument = params[i].name;
      if ( !params[i].type.isValue() )
      {
        argument = startLowerCase( stripPrefix( argument, "p" ) );
        if ( singularParams.find( i ) != singularParams.end() )
        {
          argument = stripPluralS( argument, m_tags );
        }
      }
      else
      {
        assert( singularParams.find( i ) == singularParams.end() );
      }
      arguments += ", " + argument;
    }
  }
  return arguments;
}

std::string VulkanHppGenerator::generateCallArgumentsStandard( std::string const & handle, std::vector<ParamData> const & params ) const
{
  std::string arguments;
  bool        encounteredArgument = false;
  for ( auto const & param : params )
  {
    if ( encounteredArgument )
    {
      arguments += ", ";
    }
    if ( ( param.type.type == handle ) && param.type.isValue() )
    {
      assert( param.arraySizes.empty() && param.len.empty() );
      arguments += "m_" + startLowerCase( stripPrefix( param.type.type, "Vk" ) );
    }
    else
    {
      std::string argument = param.name;
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
          argument = "static_cast<" + param.type.type + ">( " + argument + " )";
        }
        else
        {
          assert( !param.type.postfix.empty() );
          argument = "reinterpret_cast<" + param.type.compose( "" ) + ">( " + argument + " )";
        }
      }
      arguments += argument;
    }
    encounteredArgument = true;
  }
  return arguments;
}

std::string VulkanHppGenerator::generateCallArgumentEnhanced( std::vector<ParamData> const & params,
                                                              size_t                         paramIndex,
                                                              bool                           nonConstPointerAsNullptr,
                                                              std::set<size_t> const &       singularParams,
                                                              std::set<size_t> const &       templatedParams ) const
{
  std::string       argument;
  ParamData const & param = params[paramIndex];
  if ( param.type.isConstPointer() || ( specialPointerTypes.find( param.type.type ) != specialPointerTypes.end() ) )
  {
    // parameter is a const-pointer or one of the special pointer types that are considered to be const-pointers
    argument = generateCallArgumentEnhancedConstPointer( param, paramIndex, singularParams, templatedParams );
  }
  else if ( param.type.isNonConstPointer() && ( specialPointerTypes.find( param.type.type ) == specialPointerTypes.end() ) )
  {
    // parameter is a non-const pointer and none of the special pointer types, that are considered const-pointers
    argument = generateCallArgumentEnhancedNonConstPointer( param, paramIndex, nonConstPointerAsNullptr, singularParams );
  }
  else
  {
    argument = generateCallArgumentEnhancedValue( params, paramIndex, singularParams );
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
    assert( param.arraySizes.empty() && param.len.empty() );
    argument = "m_" + startLowerCase( stripPrefix( param.type.type, "Vk" ) );
  }
  else if ( param.len.empty() )
  {
    // this const-pointer parameter has no length, that is it's a const-pointer to a single value
    if ( param.type.type == "void" )
    {
      argument = ( templatedParams.find( paramIndex ) == templatedParams.end() )
                 ? param.name
                 : "reinterpret_cast<" + param.type.compose( "VULKAN_HPP_NAMESPACE" ) + ">( &" + name + " )";
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
  else if ( param.len == "null-terminated" )
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
    if ( singularParams.find( paramIndex ) != singularParams.end() )
    {
      assert( !param.optional );
      argument = "&" + stripPluralS( name, m_tags );
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
  if ( param.len.empty() )
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
    // the non-const pointer has a len -> it will be represented by some array
    assert( param.arraySizes.empty() );
    if ( nonConstPointerAsNullptr )
    {
      argument = "nullptr";
    }
    else
    {
      if ( singularParams.find( paramIndex ) != singularParams.end() )
      {
        argument = "&" + stripPluralS( name, m_tags );
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

std::string VulkanHppGenerator::generateCallArgumentEnhancedValue( std::vector<ParamData> const & params,
                                                                   size_t                         paramIndex,
                                                                   std::set<size_t> const &       singularParams ) const
{
  std::string       argument;
  ParamData const & param = params[paramIndex];
  assert( param.len.empty() );
  if ( param.type.type.starts_with( "Vk" ) )
  {
    if ( param.arraySizes.empty() )
    {
      auto pointerIt = std::find_if( params.begin(), params.end(), [&param]( ParamData const & pd ) { return pd.len == param.name; } );
      if ( pointerIt != params.end() )
      {
        assert( !param.optional );
        argument = startLowerCase( stripPrefix( pointerIt->name, "p" ) ) + ".size()";
        if ( pointerIt->type.type == "void" )
        {
          argument += " * sizeof( " + stripPrefix( pointerIt->name, "p" ) + "Type )";
        }
      }
      else
      {
        argument = "static_cast<" + param.type.compose( "" ) + ">( " + param.name + " )";
      }
      assert( std::find_if( params.begin(), params.end(), [&param]( ParamData const & pd ) { return pd.stride == param.name; } ) == params.end() );
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
    auto pointerIt = std::find_if( params.begin(), params.end(), [&param]( ParamData const & pd ) { return pd.len == param.name; } );
    if ( pointerIt != params.end() )
    {
      // this parameter is the len of some other -> replace it with that parameter's size
      assert( param.arraySizes.empty() );
      assert( ( param.type.type == "size_t" ) || ( param.type.type == "uint32_t" ) );
      if ( singularParams.find( paramIndex ) == singularParams.end() )
      {
        argument = startLowerCase( stripPrefix( pointerIt->name, "p" ) ) + ".size()";
        if ( pointerIt->type.type == "void" )
        {
          argument += " * sizeof( " + stripPrefix( pointerIt->name, "p" ) + "Type )";
        }
      }
      else
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
    }
    else
    {
      assert( !param.optional );
      assert( param.arraySizes.size() <= 1 );
      pointerIt = std::find_if( params.begin(), params.end(), [&param]( ParamData const & pd ) { return pd.stride == param.name; } );
      if ( pointerIt != params.end() )
      {
        // this parameter is the stride of some other -> replace it with that parameter's stride
        assert( param.arraySizes.empty() );
        assert( param.type.type == "uint32_t" );
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
                                                      CommandFlavourFlags                       flavourFlags,
                                                      bool                                      raii ) const
{
  std::string dispatcher = raii ? "getDispatcher()->" : "d.";
  // if at least one returnParam is a size value of a vector param (and no singular params), we need two calls
  if ( singularParams.empty() &&
       ( std::find_if( returnParams.begin(),
                       returnParams.end(),
                       [&vectorParams]( size_t rp )
                       {
                         return ( std::find_if( vectorParams.begin(), vectorParams.end(), [rp]( auto const & vp ) { return vp.second.lenParam == rp; } ) !=
                                  vectorParams.end() );
                       } ) != returnParams.end() ) )
  {
    auto vectorParamIt = vectorParams.find( returnParams[1] );
    assert( ( vectorParamIt != vectorParams.end() ) && ( vectorParamIt->second.lenParam == returnParams[0] ) );

    std::string firstCallArguments  = generateCallArgumentsEnhanced( commandData, initialSkipCount, true, {}, templatedParams, raii );
    std::string secondCallArguments = generateCallArgumentsEnhanced( commandData, initialSkipCount, false, {}, templatedParams, raii );
    std::string vectorName          = startLowerCase( stripPrefix( commandData.params[vectorParamIt->first].name, "p" ) );
    std::string vectorSize          = startLowerCase( stripPrefix( commandData.params[vectorParamIt->second.lenParam].name, "p" ) );

    if ( flavourFlags & CommandFlavourFlagBits::chained )
    {
      assert( vectorParams.size() == 1 );
      // chained data needs some more handling!!
      std::string vectorElementType = stripPostfix( commandData.params[vectorParamIt->first].type.compose( "VULKAN_HPP_NAMESPACE" ), " *" );

      if ( commandData.returnType == "VkResult" )
      {
        const std::string callSequenceTemplate = R"(VkResult result;
    do
    {
      result = ${dispatcher}${vkCommand}( ${firstCallArguments} );
      if ( ( result == VK_SUCCESS ) && ${counterName} )
      {
        structureChains.resize( ${counterName} );
        ${vectorName}.resize( ${counterName} );
        for ( ${counterType} i = 0; i < ${counterName}; i++ )
        {
          ${vectorName}[i].pNext = structureChains[i].template get<${vectorElementType}>().pNext;
        }
        result = ${dispatcher}${vkCommand}( ${secondCallArguments} );
      }
    } while ( result == VK_INCOMPLETE );)";

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
      assert( ( commandData.successCodes.size() == 2 ) && ( commandData.successCodes[0] == "VK_SUCCESS" ) &&
              ( commandData.successCodes[1] == "VK_INCOMPLETE" ) );

      std::string resizes;
      for ( auto const & vp : vectorParams )
      {
        assert( ( std::find( returnParams.begin(), returnParams.end(), vp.first ) != returnParams.end() ) &&
                ( std::find( returnParams.begin(), returnParams.end(), vp.second.lenParam ) != returnParams.end() ) );
        resizes += startLowerCase( stripPrefix( commandData.params[vp.first].name, "p" ) ) + ".resize( " +
                   startLowerCase( stripPrefix( commandData.params[vp.second.lenParam].name, "p" ) ) + " );\n";
      }
      resizes.pop_back();

      std::string const callSequenceTemplate = R"(VkResult result;
    do
    {
      result = ${dispatcher}${vkCommand}( ${firstCallArguments} );
      if ( ( result == VK_SUCCESS ) && ${counterName} )
      {
        ${resizes}
        result = ${dispatcher}${vkCommand}( ${secondCallArguments} );
      }
    } while ( result == VK_INCOMPLETE );)";

      return replaceWithMap( callSequenceTemplate,
                             { { "counterName", startLowerCase( stripPrefix( commandData.params[vectorParamIt->second.lenParam].name, "p" ) ) },
                               { "dispatcher", dispatcher },
                               { "firstCallArguments", firstCallArguments },
                               { "secondCallArguments", secondCallArguments },
                               { "resizes", resizes },
                               { "vkCommand", name } } );
    }
    else
    {
      // no need to enumerate here, just two calls
      assert( commandData.returnType == "void" );
      std::string const callSequenceTemplate = R"(${dispatcher}${vkCommand}( ${firstCallArguments} );
    ${vectorName}.resize( ${vectorSize} );
    ${dispatcher}${vkCommand}( ${secondCallArguments} );)";

      return replaceWithMap( callSequenceTemplate,
                             { { "dispatcher", dispatcher },
                               { "firstCallArguments", firstCallArguments },
                               { "secondCallArguments", secondCallArguments },
                               { "vectorName", vectorName },
                               { "vectorSize", vectorSize },
                               { "vkCommand", name } } );
    }
  }
  else
  {
    std::string const callSequenceTemplate = R"(${resultAssignment}${dispatcher}${vkCommand}( ${callArguments} );)";

    std::string callArguments    = generateCallArgumentsEnhanced( commandData, initialSkipCount, false, singularParams, templatedParams, raii );
    std::string resultAssignment = generateResultAssignment( commandData );

    return replaceWithMap(
      callSequenceTemplate,
      { { "callArguments", callArguments }, { "dispatcher", dispatcher }, { "resultAssignment", resultAssignment }, { "vkCommand", name } } );
  }
}

std::string VulkanHppGenerator::generateCommand( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const
{
  std::string str;
  if ( commandData.returnType == "VkResult" )
  {
    assert( !commandData.successCodes.empty() );
    if ( commandData.successCodes.size() == 1 )
    {
      if ( commandData.errorCodes.empty() )
      {
        str = generateCommandResultSingleSuccessNoErrors( name, commandData, initialSkipCount, definition );
      }
      else
      {
        str = generateCommandResultSingleSuccessWithErrors( name, commandData, initialSkipCount, definition );
      }
    }
    else
    {
      if ( commandData.errorCodes.empty() )
      {
        str = generateCommandResultMultiSuccessNoErrors( name, commandData, initialSkipCount, definition );
      }
      else
      {
        str = generateCommandResultMultiSuccessWithErrors( name, commandData, initialSkipCount, definition );
      }
    }
  }
  else if ( commandData.returnType == "void" )
  {
    std::vector<size_t> returnParams = determineReturnParams( commandData.params );
    switch ( returnParams.size() )
    {
      case 0: str = generateCommandVoid0Return( name, commandData, initialSkipCount, definition ); break;
      case 1: str = generateCommandVoid1Return( name, commandData, initialSkipCount, definition, returnParams[0] ); break;
      case 2: str = generateCommandVoid2Return( name, commandData, initialSkipCount, definition, returnParams ); break;
    }
  }
  else
  {
    str = generateCommandValue( name, commandData, initialSkipCount, definition );
  }

  if ( str.empty() )
  {
    throw std::runtime_error( "Never encountered a function like <" + name + "> !" );
  }
  return str;
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
    commandDefinitions += generateCommandDefinitions( feature.second.requireData, listedCommands, feature.first );
  }
  for ( auto const & extIt : m_extensionsByNumber )
  {
    commandDefinitions += generateCommandDefinitions( extIt.second->second.requireData, listedCommands, extIt.second->first );
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
      if ( listedCommands.insert( command ).second )
      {
        auto commandIt = m_commands.find( command );
        assert( commandIt != m_commands.end() );
        str += generateCommandDefinitions( command, commandIt->second.handle );
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateCommandDefinitions( std::string const & command, std::string const & handle ) const
{
  auto commandIt = m_commands.find( command );
  assert( commandIt != m_commands.end() );

  std::string str = "\n" + generateCommand( commandIt->first, commandIt->second, handle.empty() ? 0 : 1, true );

  // special handling for destroy functions, filter out alias functions
  std::string commandName = generateCommandName( commandIt->first, commandIt->second.params, 1, m_tags );
  if ( commandIt->second.alias.empty() &&
       ( ( ( commandIt->first.substr( 2, 7 ) == "Destroy" ) && ( commandName != "destroy" ) ) || ( commandIt->first.substr( 2, 4 ) == "Free" ) ||
         ( commandIt->first == "vkReleasePerformanceConfigurationINTEL" ) ) )
  {
    CommandData commandData = commandIt->second;
    assert( ( 1 < commandData.params.size() ) && ( commandData.params[0].type.type == handle ) );
    commandData.params[1].optional = false;  // make sure, the object to destroy/free/release is not optional in the shortened version!

    std::string destroyCommandString = generateCommand( commandIt->first, commandData, handle.empty() ? 0 : 1, true );
    std::string shortenedName;
    if ( commandIt->first.substr( 2, 7 ) == "Destroy" )
    {
      shortenedName = "destroy";
    }
    else if ( commandIt->first.substr( 2, 4 ) == "Free" )
    {
      shortenedName = "free";
    }
    else
    {
      assert( commandIt->first == "vkReleasePerformanceConfigurationINTEL" );
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
  bool singular = flavourFlags & CommandFlavourFlagBits::singular;

  assert( vectorParams.empty() || ( vectorParams.begin()->second.lenParam != INVALID_INDEX ) );
  assert( !singular || !returnParams.empty() );  // if singular is true, then there is at least one returnParam !

  std::set<size_t> skippedParams = determineSkippedParams( commandData.params, initialSkipCount, vectorParams, returnParams, singular );
  // special handling for vkGetMemoryHostPointerPropertiesEXT: here, we really need to stick with the const void * parameter !
  std::set<size_t> templatedParams = ( name == "vkGetMemoryHostPointerPropertiesEXT" ) ? std::set<size_t>() : determineVoidPointerParams( commandData.params );
  std::set<size_t> singularParams  = singular ? determineSingularParams( returnParams[0], vectorParams ) : std::set<size_t>();
  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck =
    needsVectorSizeCheck( commandData.params, vectorParams, returnParams, singularParams );
  bool enumerating = determineEnumeration( vectorParams, returnParams );

  std::vector<std::string> dataTypes = determineDataTypes( commandData.params, vectorParams, returnParams, templatedParams );
  std::string              dataType  = combineDataTypes( vectorParams, returnParams, enumerating, dataTypes, flavourFlags, false );

  std::string argumentTemplates = generateArgumentTemplates( commandData.params, returnParams, vectorParams, templatedParams, flavourFlags, false );
  auto [allocatorTemplates, uniqueHandleAllocatorTemplates] = generateAllocatorTemplates( returnParams, dataTypes, vectorParams, flavourFlags, definition );
  std::string typenameCheck                                 = generateTypenameCheck( returnParams, vectorParams, definition, dataTypes, flavourFlags );
  std::string nodiscard      = generateNoDiscard( !returnParams.empty(), 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string returnType     = generateReturnType( commandData, returnParams, vectorParams, flavourFlags, false, dataType );
  std::string className      = initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "";
  std::string classSeparator = commandData.handle.empty() ? "" : "::";
  std::string commandName    = generateCommandName( name, commandData.params, initialSkipCount, m_tags, flavourFlags );
  std::string argumentList   = generateArgumentListEnhanced(
    commandData.params, returnParams, vectorParams, skippedParams, singularParams, templatedParams, definition, flavourFlags, true );
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
      generateDataPreparation( commandData, initialSkipCount, returnParams, vectorParams, templatedParams, flavourFlags, enumerating );
    std::string dataSizeChecks = generateDataSizeChecks( commandData, returnParams, dataTypes, vectorParams, templatedParams, singular );
    std::string callSequence =
      generateCallSequence( name, commandData, returnParams, vectorParams, initialSkipCount, singularParams, templatedParams, flavourFlags, false );
    std::string resultCheck     = generateResultCheck( commandData, className, classSeparator, commandName, enumerating );
    std::string returnStatement = generateReturnStatement( name,
                                                           commandData,
                                                           returnVariable,
                                                           returnType,
                                                           dataType,
                                                           initialSkipCount,
                                                           returnParams.empty() ? INVALID_INDEX : returnParams[0],
                                                           flavourFlags,
                                                           enumerating,
                                                           false );

    std::string const functionTemplate =
      R"(  template <${argumentTemplates}${allocatorTemplates}typename Dispatch${uniqueHandleAllocatorTemplates}${typenameCheck}>
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} )${const}${noexcept}
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
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
                             { "nodiscard", nodiscard },
                             { "noexcept", noexceptString },
                             { "resultCheck", resultCheck },
                             { "returnStatement", returnStatement },
                             { "returnType", returnType },
                             { "typenameCheck", typenameCheck },
                             { "uniqueHandleAllocatorTemplates", uniqueHandleAllocatorTemplates },
                             { "vectorSizeCheck", vectorSizeCheckString } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <${argumentTemplates}${allocatorTemplates}typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE${uniqueHandleAllocatorTemplates}${typenameCheck}>
    ${nodiscard}${returnType} ${commandName}( ${argumentList} )${const}${noexcept};)";

    return replaceWithMap( functionTemplate,
                           { { "allocatorTemplates", allocatorTemplates },
                             { "argumentList", argumentList },
                             { "argumentTemplates", argumentTemplates },
                             { "commandName", commandName },
                             { "const", commandData.handle.empty() ? "" : " const" },
                             { "nodiscard", nodiscard },
                             { "noexcept", noexceptString },
                             { "returnType", returnType },
                             { "typenameCheck", typenameCheck },
                             { "uniqueHandleAllocatorTemplates", uniqueHandleAllocatorTemplates } } );
  }
}

std::string VulkanHppGenerator::generateCommandName( std::string const &            vulkanCommandName,
                                                     std::vector<ParamData> const & params,
                                                     size_t                         initialSkipCount,
                                                     std::set<std::string> const &  tags,
                                                     CommandFlavourFlags            flavourFlags ) const
{
  std::string commandName( startLowerCase( stripPrefix( vulkanCommandName, "vk" ) ) );
  for ( size_t i = initialSkipCount - 1; i < initialSkipCount; --i )  // count down to zero, then wrap around and stop
  {
    std::string const & argumentType = params[i].type.type;
    std::string         searchName   = stripPrefix( argumentType, "Vk" );
    std::string         argumentTag  = findTag( tags, argumentType );
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
    std::string commandTag = findTag( tags, commandName );
    if ( !argumentTag.empty() && ( argumentTag == commandTag ) )
    {
      commandName = stripPostfix( commandName, argumentTag );
    }
  }
  if ( flavourFlags & CommandFlavourFlagBits::singular )
  {
    commandName = stripPluralS( commandName, m_tags );
  }
  if ( flavourFlags & CommandFlavourFlagBits::unique )
  {
    commandName += "Unique";
  }
  return commandName;
}

std::string VulkanHppGenerator::generateCommandResultMultiSuccessNoErrors( std::string const & name,
                                                                           CommandData const & commandData,
                                                                           size_t              initialSkipCount,
                                                                           bool                definition ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandData.params );
  switch ( returnParams.size() )
  {
    case 0: return generateCommandResultMultiSuccessNoErrors0Return( name, commandData, initialSkipCount, definition ); break;
    case 2: return generateCommandResultMultiSuccessNoErrors2Return( name, commandData, initialSkipCount, definition, returnParams ); break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultMultiSuccessNoErrors0Return( std::string const & name,
                                                                                  CommandData const & commandData,
                                                                                  size_t              initialSkipCount,
                                                                                  bool                definition ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  if ( vectorParams.empty() )
  {
    std::vector<size_t> constPointerParams = determineConstPointerParams( commandData.params );
    if ( constPointerParams.empty() )
    {
      return generateCommandSetStandardOrEnhanced( generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                   generateCommandEnhanced( name, commandData, initialSkipCount, definition, {}, {} ) );
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultMultiSuccessNoErrors2Return(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, std::vector<size_t> const & returnParams ) const
{
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
              return generateCommandSetStandardEnhancedWithAllocator(
                definition,
                generateCommandStandard( name, commandData, initialSkipCount, definition ),
                generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams ),
                generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams, CommandFlavourFlagBits::withAllocator ) );
            }
          }
        }
      }
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultMultiSuccessWithErrors( std::string const & name,
                                                                             CommandData const & commandData,
                                                                             size_t              initialSkipCount,
                                                                             bool                definition ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandData.params );
  switch ( returnParams.size() )
  {
    case 0: return generateCommandResultWithErrors0Return( name, commandData, initialSkipCount, definition ); break;
    case 1: return generateCommandResultMultiSuccessWithErrors1Return( name, commandData, initialSkipCount, definition, returnParams[0] ); break;
    case 2: return generateCommandResultMultiSuccessWithErrors2Return( name, commandData, initialSkipCount, definition, returnParams ); break;
    case 3: return generateCommandResultMultiSuccessWithErrors3Return( name, commandData, initialSkipCount, definition, returnParams ); break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultMultiSuccessWithErrors1Return(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const
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
          return generateCommandSetStandardEnhancedSingular(
            definition,
            generateCommandStandard( name, commandData, initialSkipCount, definition ),
            generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ),
            generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::singular ) );
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
            if ( isStructureChainAnchor( commandData.params[vectorParams.begin()->first].type.type ) )
            {
              return generateCommandSetStandardEnhancedWithAllocatorSingularUnique(
                definition,
                generateCommandStandard( name, commandData, initialSkipCount, definition ),
                generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ),
                generateCommandEnhanced(
                  name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::withAllocator ),
                generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::singular ),
                generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::unique ),
                generateCommandEnhanced( name,
                                         commandData,
                                         initialSkipCount,
                                         definition,
                                         vectorParams,
                                         { returnParam },
                                         CommandFlavourFlagBits::unique | CommandFlavourFlagBits::withAllocator ),
                generateCommandEnhanced( name,
                                         commandData,
                                         initialSkipCount,
                                         definition,
                                         vectorParams,
                                         { returnParam },
                                         CommandFlavourFlagBits::unique | CommandFlavourFlagBits::singular ) );
            }
          }
        }
      }
    }
  }
  else if ( isStructureChainAnchor( commandData.params[returnParam].type.type ) )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
    if ( vectorParams.empty() )
    {
      return generateCommandSetStandardEnhancedChained(
        definition,
        generateCommandStandard( name, commandData, initialSkipCount, definition ),
        generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ),
        generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::chained ) );
    }
  }
  else
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
    if ( vectorParams.empty() )
    {
      return generateCommandSetStandardEnhanced( definition,
                                                 generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                 generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ) );
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultMultiSuccessWithErrors2Return(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, std::vector<size_t> const & returnParams ) const
{
  if ( ( commandData.successCodes.size() == 2 ) && ( commandData.successCodes[0] == "VK_SUCCESS" ) && ( commandData.successCodes[1] == "VK_INCOMPLETE" ) )
  {
    if ( ( commandData.params[returnParams[0]].type.type == "size_t" ) || ( commandData.params[returnParams[0]].type.type == "uint32_t" ) )
    {
      if ( isStructureChainAnchor( commandData.params[returnParams[1]].type.type ) )
      {
        std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
        if ( vectorParams.size() == 1 )
        {
          if ( returnParams[0] == vectorParams.begin()->second.lenParam )
          {
            if ( returnParams[1] == vectorParams.begin()->first )
            {
              return generateCommandSetStandardEnhancedWithAllocatorChained(
                definition,
                generateCommandStandard( name, commandData, initialSkipCount, definition ),
                generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams ),
                generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams, CommandFlavourFlagBits::withAllocator ),
                generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams, CommandFlavourFlagBits::chained ),
                generateCommandEnhanced( name,
                                         commandData,
                                         initialSkipCount,
                                         definition,
                                         vectorParams,
                                         returnParams,
                                         CommandFlavourFlagBits::chained | CommandFlavourFlagBits::withAllocator ) );
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
              return generateCommandSetStandardEnhancedWithAllocator(
                definition,
                generateCommandStandard( name, commandData, initialSkipCount, definition ),
                generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams ),
                generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams, CommandFlavourFlagBits::withAllocator ) );
            }
          }
        }
      }
    }
    else if ( ( commandData.params[returnParams[0]].type.type != "void" ) && !isHandleType( commandData.params[returnParams[0]].type.type ) &&
              !isStructureChainAnchor( commandData.params[returnParams[0]].type.type ) )
    {
      if ( ( commandData.params[returnParams[1]].type.type != "void" ) && !isHandleType( commandData.params[returnParams[1]].type.type ) &&
           !isStructureChainAnchor( commandData.params[returnParams[1]].type.type ) )
      {
        std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
        if ( vectorParams.empty() )
        {
          return generateCommandSetStandardEnhanced( definition,
                                                     generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                     generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams ) );
        }
      }
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultMultiSuccessWithErrors3Return(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, std::vector<size_t> const & returnParams ) const
{
  if ( commandData.params[returnParams[0]].type.type == "uint32_t" )
  {
    if ( ( commandData.params[returnParams[1]].type.type != "void" ) && !isHandleType( commandData.params[returnParams[1]].type.type ) &&
         !isStructureChainAnchor( commandData.params[returnParams[1]].type.type ) )
    {
      if ( ( commandData.params[returnParams[2]].type.type != "void" ) && !isHandleType( commandData.params[returnParams[2]].type.type ) &&
           !isStructureChainAnchor( commandData.params[returnParams[2]].type.type ) )
      {
        std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
        if ( vectorParams.size() == 2 )
        {
          if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
          {
            if ( returnParams[0] == vectorParams.begin()->second.lenParam )
            {
              if ( returnParams[1] == vectorParams.begin()->first )
              {
                if ( returnParams[2] == std::next( vectorParams.begin() )->first )
                {
                  return generateCommandSetStandardEnhancedWithAllocator(
                    definition,
                    generateCommandStandard( name, commandData, initialSkipCount, definition ),
                    generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams ),
                    generateCommandEnhanced(
                      name, commandData, initialSkipCount, definition, vectorParams, returnParams, CommandFlavourFlagBits::withAllocator ) );
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

std::string VulkanHppGenerator::generateCommandResultSingleSuccessNoErrors( std::string const & name,
                                                                            CommandData const & commandData,
                                                                            size_t              initialSkipCount,
                                                                            bool                definition ) const
{
  std::vector<size_t>               returnParams = determineReturnParams( commandData.params );
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  switch ( returnParams.size() )
  {
    case 0:
      switch ( vectorParams.size() )
      {
        case 0:
          {
            std::vector<size_t> constPointerParams = determineConstPointerParams( commandData.params );
            switch ( constPointerParams.size() )
            {
              case 0:
                return generateCommandSetStandardOrEnhanced( generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                             generateCommandEnhanced( name, commandData, initialSkipCount, definition, {}, {} ) );
              case 1:
                return generateCommandSetStandardEnhanced( definition,
                                                           generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                           generateCommandEnhanced( name, commandData, initialSkipCount, definition, {}, {} ) );
            }
          }
          break;
        case 1:
          if ( commandData.params[vectorParams.begin()->second.lenParam].type.isValue() )
          {
            if ( isHandleType( commandData.params[vectorParams.begin()->first].type.type ) )
            {
              return generateCommandSetStandardEnhanced( definition,
                                                         generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                         generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, {} ) );
            }
          }
          break;
      }
      break;
    case 1:
      if ( vectorParams.empty() )
      {
        if ( ( commandData.params[returnParams[0]].type.type != "void" ) && !isHandleType( commandData.params[returnParams[0]].type.type ) &&
             !isStructureChainAnchor( commandData.params[returnParams[0]].type.type ) )
        {
          return generateCommandSetStandardEnhanced( definition,
                                                     generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                     generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams ) );
        }
      }
      break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors( std::string const & name,
                                                                              CommandData const & commandData,
                                                                              size_t              initialSkipCount,
                                                                              bool                definition ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandData.params );
  switch ( returnParams.size() )
  {
    case 0: return generateCommandResultWithErrors0Return( name, commandData, initialSkipCount, definition ); break;
    case 1: return generateCommandResultSingleSuccessWithErrors1Return( name, commandData, initialSkipCount, definition, returnParams[0] ); break;
    case 2: return generateCommandResultSingleSuccessWithErrors2Return( name, commandData, initialSkipCount, definition, returnParams ); break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors1Return(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const
{
  if ( commandData.params[returnParam].type.type == "void" )
  {
    return generateCommandResultSingleSuccessWithErrors1ReturnVoid( name, commandData, initialSkipCount, definition, returnParam );
  }
  else if ( isHandleType( commandData.params[returnParam].type.type ) )
  {
    return generateCommandResultSingleSuccessWithErrors1ReturnHandle( name, commandData, initialSkipCount, definition, returnParam );
  }
  else if ( isStructureChainAnchor( commandData.params[returnParam].type.type ) )
  {
    return generateCommandResultSingleSuccessWithErrors1ReturnChain( name, commandData, initialSkipCount, definition, returnParam );
  }
  else
  {
    return generateCommandResultSingleSuccessWithErrors1ReturnValue( name, commandData, initialSkipCount, definition, returnParam );
  }
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors1ReturnChain(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  if ( vectorParams.empty() )
  {
    return generateCommandSetStandardEnhancedChained(
      definition,
      generateCommandStandard( name, commandData, initialSkipCount, definition ),
      generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ),
      generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::chained ) );
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors1ReturnHandle(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  switch ( vectorParams.size() )
  {
    case 0:
      return generateCommandSetStandardEnhancedUnique(
        definition,
        generateCommandStandard( name, commandData, initialSkipCount, definition ),
        generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ),
        generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::unique ) );
      break;
    case 1:
      return generateCommandResultSingleSuccessWithErrors1ReturnHandle1Vector(
        name, commandData, initialSkipCount, definition, returnParam, *vectorParams.begin() );
      break;
    case 2:
      return generateCommandResultSingleSuccessWithErrors1ReturnHandle2Vector( name, commandData, initialSkipCount, definition, returnParam, vectorParams );
      break;
  }
  return "";
}

std::string
  VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors1ReturnHandle1Vector( std::string const &                        name,
                                                                                        CommandData const &                        commandData,
                                                                                        size_t                                     initialSkipCount,
                                                                                        bool                                       definition,
                                                                                        size_t                                     returnParam,
                                                                                        std::pair<size_t, VectorParamData> const & vectorParamIndex ) const
{
  if ( returnParam == vectorParamIndex.first )
  {
    if ( isLenByStructMember( commandData.params[vectorParamIndex.first].len, commandData.params[vectorParamIndex.second.lenParam] ) )
    {
      return generateCommandSetStandardEnhancedWithAllocatorUnique(
        definition,
        generateCommandStandard( name, commandData, initialSkipCount, definition ),
        generateCommandEnhanced( name, commandData, initialSkipCount, definition, { vectorParamIndex }, { returnParam } ),
        generateCommandEnhanced(
          name, commandData, initialSkipCount, definition, { vectorParamIndex }, { returnParam }, CommandFlavourFlagBits::withAllocator ),
        generateCommandEnhanced( name, commandData, initialSkipCount, definition, { vectorParamIndex }, { returnParam }, CommandFlavourFlagBits::unique ),
        generateCommandEnhanced( name,
                                 commandData,
                                 initialSkipCount,
                                 definition,
                                 { vectorParamIndex },
                                 { returnParam },
                                 CommandFlavourFlagBits::unique | CommandFlavourFlagBits::withAllocator ) );
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors1ReturnHandle2Vector( std::string const &                       name,
                                                                                                  CommandData const &                       commandData,
                                                                                                  size_t                                    initialSkipCount,
                                                                                                  bool                                      definition,
                                                                                                  size_t                                    returnParam,
                                                                                                  std::map<size_t, VectorParamData> const & vectorParams ) const
{
  if ( returnParam == std::next( vectorParams.begin() )->first )
  {
    if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
    {
      if ( commandData.params[vectorParams.begin()->second.lenParam].type.isValue() )
      {
        if ( ( commandData.params[vectorParams.begin()->first].type.type != "void" ) &&
             !isHandleType( commandData.params[vectorParams.begin()->first].type.type ) )
        {
          return generateCommandSetStandardEnhancedWithAllocatorSingularUnique(
            definition,
            generateCommandStandard( name, commandData, initialSkipCount, definition ),
            generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ),
            generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::withAllocator ),
            generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::singular ),
            generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::unique ),
            generateCommandEnhanced( name,
                                     commandData,
                                     initialSkipCount,
                                     definition,
                                     vectorParams,
                                     { returnParam },
                                     CommandFlavourFlagBits::unique | CommandFlavourFlagBits::withAllocator ),
            generateCommandEnhanced( name,
                                     commandData,
                                     initialSkipCount,
                                     definition,
                                     vectorParams,
                                     { returnParam },
                                     CommandFlavourFlagBits::singular | CommandFlavourFlagBits::unique ) );
        }
      }
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors1ReturnValue(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  switch ( vectorParams.size() )
  {
    case 0:
      return generateCommandSetStandardEnhanced( definition,
                                                 generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                 generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ) );
    case 2:
      return generateCommandResultSingleSuccessWithErrors1ReturnValue2Vectors( name, commandData, initialSkipCount, definition, returnParam, vectorParams );
      break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors1ReturnValue2Vectors( std::string const &                       name,
                                                                                                  CommandData const &                       commandData,
                                                                                                  size_t                                    initialSkipCount,
                                                                                                  bool                                      definition,
                                                                                                  size_t                                    returnParam,
                                                                                                  std::map<size_t, VectorParamData> const & vectorParams ) const
{
  if ( returnParam == std::next( vectorParams.begin() )->first )
  {
    if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
    {
      if ( commandData.params[vectorParams.begin()->second.lenParam].type.type == "uint32_t" )
      {
        if ( ( commandData.params[vectorParams.begin()->first].type.type != "void" ) &&
             !isHandleType( commandData.params[vectorParams.begin()->first].type.type ) &&
             !isStructureChainAnchor( commandData.params[vectorParams.begin()->first].type.type ) )
        {
          return generateCommandSetStandardEnhancedWithAllocatorSingular(
            definition,
            generateCommandStandard( name, commandData, initialSkipCount, definition ),
            generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ),
            generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::withAllocator ),
            generateCommandEnhanced( name,
                                     commandData,
                                     initialSkipCount,
                                     definition,
                                     vectorParams,
                                     { returnParam },
                                     CommandFlavourFlagBits::singular | CommandFlavourFlagBits::unique ) );
        }
      }
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors1ReturnVoid(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  switch ( vectorParams.size() )
  {
    case 0:
      return generateCommandSetStandardEnhanced( definition,
                                                 generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                 generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ) );
      break;
    case 1:
      if ( returnParam == vectorParams.begin()->first )
      {
        if ( commandData.params[vectorParams.begin()->second.lenParam].type.isValue() )
        {
          return generateCommandSetStandardEnhancedSingular(
            definition,
            generateCommandStandard( name, commandData, initialSkipCount, definition ),
            generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ),
            generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::singular ) );
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
                return generateCommandSetStandardEnhancedSingular(
                  definition,
                  generateCommandStandard( name, commandData, initialSkipCount, definition ),
                  generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ),
                  generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::singular ) );
              }
            }
          }
        }
      }
      break;
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandResultSingleSuccessWithErrors2Return(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, std::vector<size_t> const & returnParams ) const
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
          if ( vectorParams.find( returnParams[1] ) == vectorParams.end() )
          {
            assert( ( returnParams[1] != vectorParams.begin()->second.lenParam ) && ( returnParams[1] != std::next( vectorParams.begin() )->second.lenParam ) );
            if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
            {
              if ( commandData.params[vectorParams.begin()->second.lenParam].type.isValue() )
              {
                if ( ( commandData.params[vectorParams.begin()->first].type.type != "void" ) &&
                     !isHandleType( commandData.params[vectorParams.begin()->first].type.type ) &&
                     !isStructureChainAnchor( commandData.params[vectorParams.begin()->first].type.type ) )
                {
                  return generateCommandSetStandardEnhancedWithAllocatorSingular(
                    definition,
                    generateCommandStandard( name, commandData, initialSkipCount, definition ),
                    generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams ),
                    generateCommandEnhanced(
                      name, commandData, initialSkipCount, definition, vectorParams, returnParams, CommandFlavourFlagBits::withAllocator ),
                    generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams, CommandFlavourFlagBits::singular ) );
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

std::string VulkanHppGenerator::generateCommandResultWithErrors0Return( std::string const & name,
                                                                        CommandData const & commandData,
                                                                        size_t              initialSkipCount,
                                                                        bool                definition ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  if ( vectorParams.empty() && determineConstPointerParams( commandData.params ).empty() )
  {
    return generateCommandSetStandardOrEnhanced( generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                 generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, {} ) );
  }
  else if ( allVectorSizesSupported( commandData.params, vectorParams ) )
  {
    return generateCommandSetStandardEnhanced( definition,
                                               generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                               generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, {} ) );
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandSetStandard( std::string const & standard ) const
{
  const std::string commandTemplate = R"(
${commandStandard}
)";

  return replaceWithMap( commandTemplate, std::map<std::string, std::string>( { { "commandStandard", standard } } ) );
}

std::string VulkanHppGenerator::generateCommandSetStandardEnhanced( bool definition, std::string const & standard, std::string const & enhanced ) const
{
  const std::string commandTemplate = R"(
${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

  return replaceWithMap( commandTemplate,
                         std::map<std::string, std::string>(
                           { { "commandEnhanced", enhanced }, { "commandStandard", standard }, { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

std::string VulkanHppGenerator::generateCommandSetStandardEnhancedChained( bool                definition,
                                                                           std::string const & standard,
                                                                           std::string const & enhanced,
                                                                           std::string const & enhancedChained ) const
{
  std::string const commandTemplate = R"(
${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedChained}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

  return replaceWithMap( commandTemplate,
                         std::map<std::string, std::string>( { { "commandEnhanced", enhanced },
                                                               { "commandEnhancedChained", enhancedChained },
                                                               { "commandStandard", standard },
                                                               { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

std::string VulkanHppGenerator::generateCommandSetStandardEnhancedSingular( bool                definition,
                                                                            std::string const & standard,
                                                                            std::string const & enhanced,
                                                                            std::string const & enhancedSingular ) const
{
  std::string const commandTemplate = R"(
${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedSingular}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

  return replaceWithMap( commandTemplate,
                         std::map<std::string, std::string>( { { "commandEnhanced", enhanced },
                                                               { "commandEnhancedSingular", enhancedSingular },
                                                               { "commandStandard", standard },
                                                               { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

std::string VulkanHppGenerator::generateCommandSetStandardEnhancedUnique( bool                definition,
                                                                          std::string const & standard,
                                                                          std::string const & enhanced,
                                                                          std::string const & enhancedUnique ) const
{
  std::string const commandTemplate = R"(
${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
#  ifndef VULKAN_HPP_NO_SMART_HANDLE
${commandEnhancedUnique}
#  endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

  return replaceWithMap( commandTemplate,
                         std::map<std::string, std::string>( { { "commandEnhanced", enhanced },
                                                               { "commandEnhancedUnique", enhancedUnique },
                                                               { "commandStandard", standard },
                                                               { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

std::string VulkanHppGenerator::generateCommandSetStandardEnhancedWithAllocator( bool                definition,
                                                                                 std::string const & standard,
                                                                                 std::string const & enhanced,
                                                                                 std::string const & enhancedWithAllocator ) const
{
  const std::string commandTemplate = R"(
${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedWithAllocator}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

  return replaceWithMap( commandTemplate,
                         std::map<std::string, std::string>( { { "commandEnhanced", enhanced },
                                                               { "commandEnhancedWithAllocator", enhancedWithAllocator },
                                                               { "commandStandard", standard },
                                                               { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

std::string VulkanHppGenerator::generateCommandSetStandardEnhancedWithAllocatorChained( bool                definition,
                                                                                        std::string const & standard,
                                                                                        std::string const & enhanced,
                                                                                        std::string const & enhancedWithAllocator,
                                                                                        std::string const & enhancedChained,
                                                                                        std::string const & enhancedChainedWithAllocator ) const
{
  std::string const commandTemplate = R"(
${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedWithAllocator}${newlineOnDefinition}
${commandEnhancedChained}${newlineOnDefinition}
${commandEnhancedChainedWithAllocator}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

  return replaceWithMap( commandTemplate,
                         std::map<std::string, std::string>( { { "commandEnhanced", enhanced },
                                                               { "commandEnhancedChained", enhancedChained },
                                                               { "commandEnhancedChainedWithAllocator", enhancedChainedWithAllocator },
                                                               { "commandEnhancedWithAllocator", enhancedWithAllocator },
                                                               { "commandStandard", standard },
                                                               { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

std::string VulkanHppGenerator::generateCommandSetStandardEnhancedWithAllocatorSingular( bool                definition,
                                                                                         std::string const & standard,
                                                                                         std::string const & enhanced,
                                                                                         std::string const & enhancedWithAllocator,
                                                                                         std::string const & enhancedSingular ) const
{
  std::string const commandTemplate = R"(
${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedWithAllocator}${newlineOnDefinition}
${commandEnhancedSingular}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

  return replaceWithMap( commandTemplate,
                         std::map<std::string, std::string>( { { "commandEnhanced", enhanced },
                                                               { "commandEnhancedSingular", enhancedSingular },
                                                               { "commandEnhancedWithAllocator", enhancedWithAllocator },
                                                               { "commandStandard", standard },
                                                               { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

std::string VulkanHppGenerator::generateCommandSetStandardEnhancedWithAllocatorSingularUnique( bool                definition,
                                                                                               std::string const & standard,
                                                                                               std::string const & enhanced,
                                                                                               std::string const & enhancedWithAllocator,
                                                                                               std::string const & enhancedSingular,
                                                                                               std::string const & enhancedUnique,
                                                                                               std::string const & enhancedUniqueWithAllocator,
                                                                                               std::string const & enhancedUniqueSingular ) const
{
  std::string const commandTemplate = R"(
${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedWithAllocator}${newlineOnDefinition}
${commandEnhancedSingular}${newlineOnDefinition}
#  ifndef VULKAN_HPP_NO_SMART_HANDLE
${commandEnhancedUnique}${newlineOnDefinition}
${commandEnhancedUniqueWithAllocator}${newlineOnDefinition}
${commandEnhancedUniqueSingular}
#  endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

  return replaceWithMap( commandTemplate,
                         std::map<std::string, std::string>( { { "commandEnhanced", enhanced },
                                                               { "commandEnhancedSingular", enhancedSingular },
                                                               { "commandEnhancedUnique", enhancedUnique },
                                                               { "commandEnhancedUniqueSingular", enhancedUniqueSingular },
                                                               { "commandEnhancedUniqueWithAllocator", enhancedUniqueWithAllocator },
                                                               { "commandEnhancedWithAllocator", enhancedWithAllocator },
                                                               { "commandStandard", standard },
                                                               { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

std::string VulkanHppGenerator::generateCommandSetStandardEnhancedWithAllocatorUnique( bool                definition,
                                                                                       std::string const & standard,
                                                                                       std::string const & enhanced,
                                                                                       std::string const & enhancedWithAllocator,
                                                                                       std::string const & enhancedUnique,
                                                                                       std::string const & enhancedUniqueWithAllocator ) const
{
  std::string const commandTemplate = R"(
${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedWithAllocator}${newlineOnDefinition}
#  ifndef VULKAN_HPP_NO_SMART_HANDLE
${commandEnhancedUnique}${newlineOnDefinition}
${commandEnhancedUniqueWithAllocator}
#  endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

  return replaceWithMap( commandTemplate,
                         std::map<std::string, std::string>( { { "commandEnhanced", enhanced },
                                                               { "commandEnhancedUnique", enhancedUnique },
                                                               { "commandEnhancedUniqueWithAllocator", enhancedUniqueWithAllocator },
                                                               { "commandEnhancedWithAllocator", enhancedWithAllocator },
                                                               { "commandStandard", standard },
                                                               { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

std::string VulkanHppGenerator::generateCommandSetStandardOrEnhanced( std::string const & standard, std::string const & enhanced ) const
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

std::string
  VulkanHppGenerator::generateCommandStandard( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const
{
  std::set<size_t> skippedParams = determineSkippedParams( commandData.params, initialSkipCount, {}, {}, false );

  std::string argumentList = generateArgumentListStandard( commandData.params, skippedParams );
  std::string commandName  = generateCommandName( name, commandData.params, initialSkipCount, m_tags );
  std::string nodiscard    = ( 1 < commandData.successCodes.size() + commandData.errorCodes.size() ) ? "VULKAN_HPP_NODISCARD " : "";
  std::string returnType   = stripPrefix( commandData.returnType, "Vk" );

  if ( definition )
  {
    std::string functionBody = "d." + name + "( " + generateCallArgumentsStandard( commandData.handle, commandData.params ) + " )";
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

std::string
  VulkanHppGenerator::generateCommandValue( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandData.params );
  if ( returnParams.empty() )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
    if ( vectorParams.empty() && determineConstPointerParams( commandData.params ).empty() )
    {
      return generateCommandSetStandard( generateCommandStandard( name, commandData, initialSkipCount, definition ) );
    }
    else if ( vectorParams.size() <= 1 )
    {
      return generateCommandSetStandardEnhanced( definition,
                                                 generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                 generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams ) );
    }
  }
  return "";
}

std::string
  VulkanHppGenerator::generateCommandVoid0Return( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  if ( vectorParams.empty() && determineConstPointerParams( commandData.params ).empty() )
  {
    return generateCommandSetStandard( generateCommandStandard( name, commandData, initialSkipCount, definition ) );
  }
  else if ( allVectorSizesSupported( commandData.params, vectorParams ) )
  {
    // All the vectorParams have a counter by value, of type "uint32_t", "VkDeviceSize", or "VkSampleCountFlagBits" (!)
    return generateCommandSetStandardEnhanced( definition,
                                               generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                               generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, {} ) );
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandVoid1Return(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
  if ( commandData.params[returnParam].type.postfix == "**" )
  {
    // get a pointer to something
    if ( commandData.params[returnParam].type.type == "void" )
    {
      if ( vectorParams.empty() )
      {
        return generateCommandSetStandardEnhanced( definition,
                                                   generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                   generateCommandEnhanced( name, commandData, initialSkipCount, definition, {}, { returnParam } ) );
      }
    }
  }
  else if ( isHandleType( commandData.params[returnParam].type.type ) )
  {
    if ( vectorParams.empty() )
    {
      return generateCommandSetStandardEnhanced( definition,
                                                 generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                 generateCommandEnhanced( name, commandData, initialSkipCount, definition, {}, { returnParam } ) );
    }
  }
  else if ( isStructureChainAnchor( commandData.params[returnParam].type.type ) )
  {
    if ( vectorParams.empty() )
    {
      return generateCommandSetStandardEnhancedChained(
        definition,
        generateCommandStandard( name, commandData, initialSkipCount, definition ),
        generateCommandEnhanced( name, commandData, initialSkipCount, definition, {}, { returnParam } ),
        generateCommandEnhanced( name, commandData, initialSkipCount, definition, {}, { returnParam }, CommandFlavourFlagBits::chained ) );
    }
  }
  else if ( commandData.params[returnParam].type.type == "void" )
  {
    switch ( vectorParams.size() )
    {
      case 0:
        return generateCommandSetStandardEnhanced( definition,
                                                   generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                   generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ) );
      case 1:
        if ( returnParam == vectorParams.begin()->first )
        {
          if ( name == stripPluralS( name, m_tags ) )
          {
            return generateCommandSetStandardEnhanced(
              definition,
              generateCommandStandard( name, commandData, initialSkipCount, definition ),
              generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::singular ) );
          }
        }
        break;
    }
  }
  else
  {
    switch ( vectorParams.size() )
    {
      case 0:
        return generateCommandSetStandardEnhanced( definition,
                                                   generateCommandStandard( name, commandData, initialSkipCount, definition ),
                                                   generateCommandEnhanced( name, commandData, initialSkipCount, definition, {}, { returnParam } ) );
        break;
      case 1:
        if ( returnParam == vectorParams.begin()->first )
        {
          // you get a vector of stuff, with the size being one of the parameters
          return generateCommandSetStandardEnhancedWithAllocator(
            definition,
            generateCommandStandard( name, commandData, initialSkipCount, definition ),
            generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ),
            generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam }, CommandFlavourFlagBits::withAllocator ) );
        }
        else
        {
          if ( !isHandleType( commandData.params[vectorParams.begin()->first].type.type ) &&
               !isStructureChainAnchor( commandData.params[vectorParams.begin()->first].type.type ) &&
               ( commandData.params[vectorParams.begin()->first].type.type != "void" ) )
          {
            if ( isLenByStructMember( commandData.params[vectorParams.begin()->first].len, commandData.params[vectorParams.begin()->second.lenParam] ) )
            {
              return generateCommandSetStandardEnhanced(
                definition,
                generateCommandStandard( name, commandData, initialSkipCount, definition ),
                generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, { returnParam } ) );
            }
          }
        }
        break;
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandVoid2Return(
  std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, std::vector<size_t> const & returnParams ) const
{
  if ( commandData.params[returnParams[0]].type.type == "uint32_t" )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandData.params );
    if ( vectorParams.size() == 1 )
    {
      if ( returnParams[0] == vectorParams.begin()->second.lenParam )
      {
        if ( returnParams[1] == vectorParams.begin()->first )
        {
          if ( isStructureChainAnchor( commandData.params[returnParams[1]].type.type ) )
          {
            return generateCommandSetStandardEnhancedWithAllocatorChained(
              definition,
              generateCommandStandard( name, commandData, initialSkipCount, definition ),
              generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams ),
              generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams, CommandFlavourFlagBits::withAllocator ),
              generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams, CommandFlavourFlagBits::chained ),
              generateCommandEnhanced( name,
                                       commandData,
                                       initialSkipCount,
                                       definition,
                                       vectorParams,
                                       returnParams,
                                       CommandFlavourFlagBits::chained | CommandFlavourFlagBits::withAllocator ) );
          }
          else if ( !isHandleType( commandData.params[returnParams[1]].type.type ) )
          {
            return generateCommandSetStandardEnhancedWithAllocator(
              definition,
              generateCommandStandard( name, commandData, initialSkipCount, definition ),
              generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams ),
              generateCommandEnhanced( name, commandData, initialSkipCount, definition, vectorParams, returnParams, CommandFlavourFlagBits::withAllocator ) );
          }
        }
      }
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateConstexprString( std::string const & structName ) const
{
  // structs with a VkBaseInStructure and VkBaseOutStructure can't be a constexpr!
  bool isConstExpression = ( structName != "VkBaseInStructure" ) && ( structName != "VkBaseOutStructure" );
  return isConstExpression ? ( std::string( "VULKAN_HPP_CONSTEXPR" ) + ( ( containsUnion( structName ) || containsArray( structName ) ) ? "_14 " : " " ) ) : "";
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
      assert( ( vectorParams.size() == 2 ) && ( returnParams[0] == vectorParams.begin()->second.lenParam ) &&
              ( returnParams[1] == vectorParams.begin()->first ) && ( returnParams[2] == std::next( vectorParams.begin() )->first ) &&
              ( returnParams[0] == std::next( vectorParams.begin() )->second.lenParam ) && templatedParams.empty() &&
              !( flavourFlags & ( CommandFlavourFlagBits::chained | CommandFlavourFlagBits::singular | CommandFlavourFlagBits::unique ) ) );
      return generateDataDeclarations3Returns( commandData, returnParams, flavourFlags, raii, dataTypes );
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

      return replaceWithMap( dataDeclarationsTemplate, { { "returnType", dataType }, { "returnVariable", returnVariable } } );
    }
    else
    {
      std::string allocator       = stripPrefix( dataTypes[0], "VULKAN_HPP_NAMESPACE::" ) + "Allocator";
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
                                                                  std::string const &                       dataType,
                                                                  std::string const &                       returnVariable ) const
{
  bool chained       = flavourFlags & CommandFlavourFlagBits::chained;
  bool singular      = flavourFlags & CommandFlavourFlagBits::singular;
  bool withAllocator = flavourFlags & CommandFlavourFlagBits::withAllocator;

  switch ( vectorParams.size() )
  {
    case 0:
      assert( !singular && !chained );
      {
        std::string firstDataVariable  = startLowerCase( stripPrefix( commandData.params[returnParams[0]].name, "p" ) );
        std::string secondDataVariable = startLowerCase( stripPrefix( commandData.params[returnParams[1]].name, "p" ) );

        std::string const dataDeclarationTemplate = R"(std::pair<${firstDataType},${secondDataType}> data;
    ${firstDataType} & ${firstDataVariable} = data.first;
    ${secondDataType} & ${secondDataVariable} = data.second;)";

        return replaceWithMap( dataDeclarationTemplate,
                               { { "firstDataType", dataTypes[0] },
                                 { "firstDataVariable", firstDataVariable },
                                 { "secondDataType", dataTypes[1] },
                                 { "secondDataVariable", secondDataVariable } } );
      }
      break;
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
                                   { "returnType", dataType },
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
      assert( ( returnParams[0] == std::next( vectorParams.begin() )->first ) && ( vectorParams.find( returnParams[1] ) == vectorParams.end() ) && !chained );
      {
        std::string firstDataVariable  = startLowerCase( stripPrefix( commandData.params[returnParams[0]].name, "p" ) );
        std::string secondDataVariable = startLowerCase( stripPrefix( commandData.params[returnParams[1]].name, "p" ) );
        if ( singular )
        {
          firstDataVariable = stripPluralS( firstDataVariable, m_tags );

          std::string const dataDeclarationTemplate = R"(std::pair<${firstDataType},${secondDataType}> data;
    ${firstDataType} & ${firstDataVariable} = data.first;
    ${secondDataType} & ${secondDataVariable} = data.second;)";

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
            R"(std::pair<std::vector<${firstDataType}${allocatorType}>,${secondDataType}> data( std::piecewise_construct, std::forward_as_tuple( ${vectorSize}${allocateInitializer} ), std::forward_as_tuple( 0 ) );
    std::vector<${firstDataType}${allocatorType}> & ${firstDataVariable} = data.first;
    ${secondDataType} & ${secondDataVariable} = data.second;)";

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

std::string VulkanHppGenerator::generateDataDeclarations3Returns( CommandData const &              commandData,
                                                                  std::vector<size_t> const &      returnParams,
                                                                  CommandFlavourFlags              flavourFlags,
                                                                  bool                             raii,
                                                                  std::vector<std::string> const & dataTypes ) const
{
  std::string counterVariable      = startLowerCase( stripPrefix( commandData.params[returnParams[0]].name, "p" ) );
  std::string firstVectorVariable  = startLowerCase( stripPrefix( commandData.params[returnParams[1]].name, "p" ) );
  std::string secondVectorVariable = startLowerCase( stripPrefix( commandData.params[returnParams[2]].name, "p" ) );
  std::string firstVectorAllocatorType, secondVectorAllocatorType, pairConstructor;
  if ( !raii )
  {
    firstVectorAllocatorType  = startUpperCase( stripPrefix( dataTypes[1], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator";
    secondVectorAllocatorType = startUpperCase( stripPrefix( dataTypes[2], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator";
    pairConstructor           = ( flavourFlags & CommandFlavourFlagBits::withAllocator )
                                ? ( "( std::piecewise_construct, std::forward_as_tuple( " + startLowerCase( firstVectorAllocatorType ) + " ), std::forward_as_tuple( " +
                          startLowerCase( secondVectorAllocatorType ) + " ) )" )
                                : "";
    firstVectorAllocatorType  = ", " + firstVectorAllocatorType;
    secondVectorAllocatorType = ", " + secondVectorAllocatorType;
  }

  std::string const dataDeclarationsTemplate =
    R"(std::pair<std::vector<${firstVectorElementType}${firstVectorAllocatorType}>, std::vector<${secondVectorElementType}${secondVectorAllocatorType}>> data${pairConstructor};
    std::vector<${firstVectorElementType}${firstVectorAllocatorType}> & ${firstVectorVariable} = data.first;
    std::vector<${secondVectorElementType}${secondVectorAllocatorType}> & ${secondVectorVariable} = data.second;
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

std::string VulkanHppGenerator::generateDataPreparation( CommandData const &                       commandData,
                                                         size_t                                    initialSkipCount,
                                                         std::vector<size_t> const &               returnParams,
                                                         std::map<size_t, VectorParamData> const & vectorParams,
                                                         std::set<size_t> const &                  templatedParams,
                                                         CommandFlavourFlags                       flavourFlags,
                                                         bool                                      enumerating ) const
{
  bool chained  = flavourFlags & CommandFlavourFlagBits::chained;
  bool singular = flavourFlags & CommandFlavourFlagBits::singular;
  bool unique   = flavourFlags & CommandFlavourFlagBits::unique;

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
      assert( vectorParams.find( returnParams[0] ) == vectorParams.end() );
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
        assert( ( std::find( returnParams.begin(), returnParams.end(), vp.first ) != returnParams.end() ) &&
                ( std::find( returnParams.begin(), returnParams.end(), vp.second.lenParam ) != returnParams.end() ) );
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
  else if ( unique && !singular && ( returnParams.size() == 1 ) && ( vectorParams.find( returnParams[0] ) != vectorParams.end() ) )
  {
    assert( !enumerating );
    std::string              className = initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "";
    std::string              deleterDefinition;
    std::vector<std::string> lenParts = tokenize( commandData.params[returnParams[0]].len, "->" );
    switch ( lenParts.size() )
    {
      case 1: deleterDefinition = "ObjectDestroy<" + className + ", Dispatch> deleter( *this, allocator, d )"; break;
      case 2:
        {
          auto vpiIt = vectorParams.find( returnParams[0] );
          assert( vpiIt != vectorParams.end() );
          std::string poolType, poolName;
          std::tie( poolType, poolName ) = getPoolTypeAndName( commandData.params[vpiIt->second.lenParam].type.type );
          assert( !poolType.empty() );
          poolType          = stripPrefix( poolType, "Vk" );
          poolName          = startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + poolName;
          deleterDefinition = "PoolFree<" + className + ", " + poolType + ", Dispatch> deleter( *this, " + poolName + ", d )";
        }
        break;
    }

    std::string handleType       = stripPrefix( commandData.params[returnParams[0]].type.type, "Vk" );
    std::string uniqueVectorName = "unique" + stripPrefix( commandData.params[returnParams[0]].name, "p" );
    std::string vectorAllocator  = ( flavourFlags & CommandFlavourFlagBits::withAllocator ) ? ( "( " + startLowerCase( handleType ) + "Allocator )" ) : "";
    std::string vectorName       = startLowerCase( stripPrefix( commandData.params[returnParams[0]].name, "p" ) );
    std::string elementName      = stripPluralS( vectorName, m_tags );
    std::string vectorSize = getVectorSize( commandData.params, vectorParams, returnParams[0], commandData.params[returnParams[0]].type.type, templatedParams );

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
      if ( ( vectorParamIt != vectorParams.end() ) && ( templatedParams.find( returnParams[i] ) != templatedParams.end() ) &&
           ( std::find( returnParams.begin(), returnParams.end(), vectorParamIt->second.lenParam ) == returnParams.end() ) )
      {
        dataSizeChecks += replaceWithMap( dataSizeCheckTemplate,
                                          { { "dataSize", commandData.params[vectorParamIt->second.lenParam].name }, { "dataType", returnParamTypes[i] } } );
      }
    }
  }

  return dataSizeChecks;
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

    void init( PFN_vkGetInstanceProcAddr getInstanceProcAddr ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT(getInstanceProcAddr);

      vkGetInstanceProcAddr = getInstanceProcAddr;

${initialCommandAssignments}
    }

    // This interface does not require a linked vulkan library.
    DispatchLoaderDynamic( VkInstance                instance,
                           PFN_vkGetInstanceProcAddr getInstanceProcAddr,
                           VkDevice                  device            = {},
                           PFN_vkGetDeviceProcAddr   getDeviceProcAddr = nullptr ) VULKAN_HPP_NOEXCEPT
    {
      init( instance, getInstanceProcAddr, device, getDeviceProcAddr );
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
      if (device) {
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
      = VULKAN_HPP_NAMESPACE::DynamicLoader
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
    appendDispatchLoaderDynamicCommands( feature.second.requireData,
                                         listedCommands,
                                         feature.first,
                                         commandMembers,
                                         initialCommandAssignments,
                                         instanceCommandAssignments,
                                         deviceCommandAssignments );
  }
  for ( auto const & extIt : m_extensionsByNumber )
  {
    appendDispatchLoaderDynamicCommands( extIt.second->second.requireData,
                                         listedCommands,
                                         extIt.second->first,
                                         commandMembers,
                                         initialCommandAssignments,
                                         instanceCommandAssignments,
                                         deviceCommandAssignments );
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
#if !defined( VK_NO_PROTOTYPES )
  class DispatchLoaderStatic : public DispatchLoaderBase
  {
  public:
${commands}
  };
#endif
)";

  std::string           commands;
  std::set<std::string> listedCommands;
  for ( auto const & feature : m_features )
  {
    commands += generateDispatchLoaderStaticCommands( feature.second.requireData, listedCommands, feature.first );
  }
  for ( auto const & extIt : m_extensionsByNumber )
  {
    commands += generateDispatchLoaderStaticCommands( extIt.second->second.requireData, listedCommands, extIt.second->first );
  }

  return replaceWithMap( dispatchLoaderStaticTemplate, { { "commands", commands } } );
}

std::string VulkanHppGenerator::generateDestroyCommand( std::string const & name, CommandData const & commandData ) const
{
  // special handling for destroy functions, filter out alias functions
  std::string commandName = generateCommandName( name, commandData.params, 1, m_tags );
  if ( commandData.alias.empty() && ( ( ( name.substr( 2, 7 ) == "Destroy" ) && ( commandName != "destroy" ) ) || ( name.substr( 2, 4 ) == "Free" ) ||
                                      ( name == "vkReleasePerformanceConfigurationINTEL" ) ) )
  {
    assert( 1 < commandData.params.size() );
    // make sure, the object to destroy/free/release is not optional in the shortened version!
    CommandData localCommandData        = commandData;
    localCommandData.params[1].optional = false;

    std::string destroyCommandString = generateCommand( name, localCommandData, 1, false );
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
                                                                                CommandData const & commandData,
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
  if ( !commandData.alias.empty() )
  {
    str += "      if ( !" + commandData.alias + " ) " + commandData.alias + " = " + commandName + ";\n";
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
      if ( listedCommands.insert( command ).second )
      {
        auto commandIt = m_commands.find( command );
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
                               { { "commandName", commandIt->first },
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
      std::find_if( m_bitmasks.begin(), m_bitmasks.end(), [&enumData]( auto const & bitmask ) { return bitmask.second.requirements == enumData.first; } );
    assert( bitmaskIt != m_bitmasks.end() );
    baseType = " : " + bitmaskIt->first;
    bitmask  = generateBitmask( bitmaskIt, surroundingProtect );
  }

  std::string                        enumValues, previousEnter, previousLeave;
  std::map<std::string, std::string> valueToNameMap;
  for ( auto const & value : enumData.second.values )
  {
    // determine the values protect, if any
    std::string valueProtect = getProtect( value );

    // if the value's protect differs from the surrounding protect, generate protection code
    std::string enter, leave;
    if ( !valueProtect.empty() && ( valueProtect != surroundingProtect ) )
    {
      tie( enter, leave ) = generateProtection( valueProtect );
    }
    if ( previousEnter != enter )
    {
      enumValues += previousLeave + enter;
    }
    std::string valueName = generateEnumValueName( enumData.first, value.name, enumData.second.isBitmask, m_tags );
    enumValues += "    " + valueName + " = " + value.name + ",\n";
    assert( valueToNameMap.find( valueName ) == valueToNameMap.end() );
    valueToNameMap[valueName] = value.name;

    previousEnter = enter;
    previousLeave = leave;
  }
  enumValues += previousLeave;

  for ( auto const & alias : enumData.second.aliases )
  {
    std::string aliasName =
      generateEnumValueName( enumData.second.alias.empty() ? enumData.first : enumData.second.alias, alias.first, enumData.second.isBitmask, m_tags );
    // make sure to only list alias values that differ from all previous values
    auto valueToNameIt = valueToNameMap.find( aliasName );
    if ( valueToNameIt == valueToNameMap.end() )
    {
#if !defined( NDEBUG )
      auto enumIt = std::find_if(
        enumData.second.values.begin(), enumData.second.values.end(), [&alias]( EnumValueData const & evd ) { return alias.second.name == evd.name; } );
      if ( enumIt == enumData.second.values.end() )
      {
        auto aliasIt = enumData.second.aliases.find( alias.second.name );
        assert( aliasIt != enumData.second.aliases.end() );
        auto nextAliasIt = enumData.second.aliases.find( aliasIt->second.name );
        while ( nextAliasIt != enumData.second.aliases.end() )
        {
          aliasIt     = nextAliasIt;
          nextAliasIt = enumData.second.aliases.find( aliasIt->second.name );
        }
        enumIt = std::find_if(
          enumData.second.values.begin(), enumData.second.values.end(), [&aliasIt]( EnumValueData const & evd ) { return aliasIt->second.name == evd.name; } );
      }
      assert( enumIt != enumData.second.values.end() );
      assert( enumIt->extension.empty() || generateProtection( getProtectFromTitle( enumIt->extension ) ).first.empty() );
#endif
      enumValues += "    " + aliasName + " = " + alias.first + ",\n";

      // map the aliasName to the name of the base
      std::string baseName = findBaseName( alias.second.name, enumData.second.aliases );
      assert( std::find_if( enumData.second.values.begin(),
                            enumData.second.values.end(),
                            [&baseName]( EnumValueData const & evd ) { return evd.name == baseName; } ) != enumData.second.values.end() );
      valueToNameMap[aliasName] = baseName;
    }
#if !defined( NDEBUG )
    else
    {
      // verify, that the identical value represents the identical name
      std::string baseName = findBaseName( alias.second.name, enumData.second.aliases );
      assert( std::find_if( enumData.second.values.begin(),
                            enumData.second.values.end(),
                            [&baseName]( EnumValueData const & evd ) { return evd.name == baseName; } ) != enumData.second.values.end() );
      assert( baseName == valueToNameIt->second );
    }
#endif
  }
  if ( !enumValues.empty() )
  {
    size_t pos = enumValues.rfind( ',' );
    assert( pos != std::string::npos );
    enumValues.erase( pos, 1 );
    enumValues = "\n" + enumValues + "  ";
  }

  std::string enumUsing;
  if ( !enumData.second.alias.empty() )
  {
    enumUsing += "  using " + stripPrefix( enumData.second.alias, "Vk" ) + " = " + stripPrefix( enumData.first, "Vk" ) + ";\n";
  }

  const std::string enumTemplate = R"(  enum class ${enumName}${baseType}
  {${enumValues}};
${enumUsing}${bitmask})";

  return replaceWithMap( enumTemplate,
                         { { "baseType", baseType },
                           { "bitmask", bitmask },
                           { "enumName", stripPrefix( enumData.first, "Vk" ) },
                           { "enumUsing", enumUsing },
                           { "enumValues", enumValues } } );
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
    enums += generateEnums( feature.second.requireData, listedEnums, feature.first );
  }
  for ( auto const & extIt : m_extensionsByNumber )
  {
    enums += generateEnums( extIt.second->second.requireData, listedEnums, extIt.second->first );
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
      auto enumIt = m_enums.find( type );
      if ( ( enumIt != m_enums.end() ) && ( listedEnums.find( type ) == listedEnums.end() ) )
      {
        listedEnums.insert( type );
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
    enumsToString += generateEnumsToString( feature.second.requireData, listedEnums, feature.first );
  }
  for ( auto const & extIt : m_extensionsByNumber )
  {
    enumsToString += generateEnumsToString( extIt.second->second.requireData, listedEnums, extIt.second->first );
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
      auto enumIt = m_enums.find( type );
      if ( ( enumIt != m_enums.end() ) && ( listedEnums.find( type ) == listedEnums.end() ) )
      {
        listedEnums.insert( type );

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
  std::string valueName = generateEnumValueName( type.type, values.front().name, bitmask, m_tags );
  std::string value     = generateNamespacedType( type.type ) + "::" + valueName;
  std::string str;
  if ( arraySizes.empty() )
  {
    str += value;
  }
  else
  {
    assert( arraySizes.size() == 1 );
    auto constIt = m_constants.find( arraySizes[0] );
    int  count   = std::stoi( ( constIt == m_constants.end() ) ? arraySizes[0] : constIt->second );
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
  if ( enumData.second.values.empty() )
  {
    functionBody = R"x(    return "(void)";)x";
  }
  else
  {
    std::string cases, previousEnter, previousLeave;
    for ( auto const & value : enumData.second.values )
    {
      auto [enter, leave] = generateProtection( getProtect( value ) );
      if ( previousEnter != enter )
      {
        cases += previousLeave + enter;
      }

      const std::string caseTemplate = R"(      case ${enumName}::e${valueName} : return "${valueName}";
)";
      cases += replaceWithMap(
        caseTemplate,
        { { "enumName", enumName }, { "valueName", generateEnumValueName( enumData.first, value.name, enumData.second.isBitmask, m_tags ).substr( 1 ) } } );

      previousEnter = enter;
      previousLeave = leave;
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

  return replaceWithMap( enumToStringTemplate,
                         { { "argument", enumData.second.values.empty() ? "" : " value" }, { "enumName", enumName }, { "functionBody", functionBody } } );
}

std::string VulkanHppGenerator::generateFailureCheck( std::vector<std::string> const & successCodes ) const
{
  assert( !successCodes.empty() );
  std::string failureCheck = "result != " + generateSuccessCode( successCodes[0], m_tags );
  if ( 1 < successCodes.size() )
  {
    failureCheck = "( " + failureCheck + " )";
    for ( size_t i = 1; i < successCodes.size(); ++i )
    {
      failureCheck += "&& ( result != " + generateSuccessCode( successCodes[i], m_tags ) + " )";
    }
  }
  return failureCheck;
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
    auto traitIt = m_formats.find( formatValuesIt->name );
    assert( traitIt != m_formats.end() );
    std::string caseString = "      case VULKAN_HPP_NAMESPACE::Format::" + generateEnumValueName( "VkFormat", traitIt->first, false, m_tags ) + ":";

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
        compatibleCases += "          case " + std::to_string( i ) + ": return VULKAN_HPP_NAMESPACE::Format::" +
                           generateEnumValueName( "VkFormat", traitIt->second.planes[i].compatible, false, m_tags ) + ";\n";
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

std::string VulkanHppGenerator::generateFunctionPointerCheck( std::string const & function, std::string const & referencedIn ) const
{
  std::string functionPointerCheck;
  if ( m_extensions.find( referencedIn ) != m_extensions.end() )
  {
    std::string message  = "Function <" + function + "> needs extension <" + referencedIn + "> enabled!";
    functionPointerCheck = "VULKAN_HPP_ASSERT( getDispatcher()->" + function + " && \"" + message + "\" );";
  }
  return functionPointerCheck;
}

std::string VulkanHppGenerator::generateHandle( std::pair<std::string, HandleData> const & handleData, std::set<std::string> & listedHandles ) const
{
  assert( listedHandles.find( handleData.first ) == listedHandles.end() );

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
    // append any forward declaration of Deleters used by this handle
    if ( !handleData.second.childrenHandles.empty() )
    {
      str += generateUniqueTypes( handleData.first, handleData.second.childrenHandles );
    }
    else if ( handleData.first == "VkPhysicalDevice" )
    {
      // special handling for class Device, as it's created from PhysicalDevice, but destroys itself
      str += generateUniqueTypes( "", { "VkDevice" } );
    }

    // list all the commands that are mapped to members of this class
    std::string commands = generateHandleCommandDeclarations( handleData.second.commands );

    // create CPPType template specialization and the debugReportObjectType
    std::string valueName = handleData.second.objTypeEnum;
    valueName             = valueName.replace( 3, 0, "DEBUG_REPORT_" ) + "_EXT";
    auto enumIt           = m_enums.find( "VkDebugReportObjectTypeEXT" );
    assert( enumIt != m_enums.end() );
    auto valueIt =
      std::find_if( enumIt->second.values.begin(), enumIt->second.values.end(), [&valueName]( EnumValueData const & evd ) { return valueName == evd.name; } );

    std::string className = stripPrefix( handleData.first, "Vk" );
    std::string cppType, debugReportObjectType;
    if ( valueIt == enumIt->second.values.end() )
    {
      debugReportObjectType = "eUnknown";
    }
    else
    {
      static const std::string cppTypeFromDebugReportObjectTypeEXTTemplate = R"(
  template <>
  struct CppType<VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT, VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::e${className}>
  {
    using Type = VULKAN_HPP_NAMESPACE::${className};
  };
)";
      cppType               = replaceWithMap( cppTypeFromDebugReportObjectTypeEXTTemplate, { { "className", className } } );
      debugReportObjectType = generateEnumValueName( enumIt->first, valueIt->name, false, m_tags );
    }

    auto [enter, leave] = generateProtection( handleData.second.alias.empty() ? getProtectFromType( handleData.first ) : "" );

    assert( !handleData.second.objTypeEnum.empty() );
    enumIt = m_enums.find( "VkObjectType" );
    assert( enumIt != m_enums.end() );
    valueIt = std::find_if( enumIt->second.values.begin(),
                            enumIt->second.values.end(),
                            [&handleData]( EnumValueData const & evd ) { return evd.name == handleData.second.objTypeEnum; } );
    assert( valueIt != enumIt->second.values.end() );
    std::string usingAlias;
    if ( !handleData.second.alias.empty() )
    {
      usingAlias += "  using " + stripPrefix( handleData.second.alias, "Vk" ) + " = " + stripPrefix( handleData.first, "Vk" ) + ";\n";
    }

    const std::string typesafeExplicitKeyword          = handleData.second.isDispatchable ? "" : "VULKAN_HPP_TYPESAFE_EXPLICIT ";
    const std::string typesafeConversionConditional    = handleData.second.isDispatchable ? "" : "#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)\n";
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
    VULKAN_HPP_CONSTEXPR ${className}() = default;
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

#if defined(VULKAN_HPP_HAS_SPACESHIP_OPERATOR)
    auto operator<=>( ${className} const & ) const = default;
#else
    bool operator==( ${className} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_${memberName} == rhs.m_${memberName};
    }

    bool operator!=(${className} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_${memberName} != rhs.m_${memberName};
    }

    bool operator<(${className} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_${memberName} < rhs.m_${memberName};
    }
#endif
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
                             { "objTypeEnum", generateEnumValueName( enumIt->first, valueIt->name, false, m_tags ) },
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
    std::vector<std::string> commandNames = selectCommandsByHandle( feature.second.requireData, commands, listedCommands );
    if ( !commandNames.empty() )
    {
      str += "\n  //=== " + feature.first + " ===\n";
      for ( auto const & command : commandNames )
      {
        auto commandIt = m_commands.find( command );
        assert( commandIt != m_commands.end() );

        str += "\n";
        str += generateCommand( commandIt->first, commandIt->second, 1, false );
        str += generateDestroyCommand( commandIt->first, commandIt->second );
      }
    }
  }
  for ( auto const & extIt : m_extensionsByNumber )
  {
    std::vector<std::string> commandNames = selectCommandsByHandle( extIt.second->second.requireData, commands, listedCommands );
    if ( !commandNames.empty() )
    {
      auto [enter, leave] = generateProtection( getProtectFromTitle( extIt.second->first ) );
      str += "\n" + enter + "  //=== " + extIt.second->first + " ===\n";
      for ( auto const & command : commandNames )
      {
        auto commandIt = m_commands.find( command );
        assert( commandIt != m_commands.end() );

        std::string commandString;
        std::string commandName = generateCommandName( commandIt->first, commandIt->second.params, 1, m_tags );
        str += "\n";
        str += generateCommand( commandIt->first, commandIt->second, 1, false );
        str += generateDestroyCommand( commandIt->first, commandIt->second );
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
    auto commandIt = m_commands.find( command );
    assert( commandIt != m_commands.end() );
    for ( auto const & parameter : commandIt->second.params )
    {
      auto handleIt = m_handles.find( parameter.type.type );
      if ( ( handleIt != m_handles.end() ) && ( parameter.type.type != handleData.first ) &&
           ( listedHandles.find( parameter.type.type ) == listedHandles.end() ) )
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
    std::vector<std::string> commands = selectCommandsByHandle( feature.second.requireData, handleData.commands, listedCommands );
    if ( !commands.empty() )
    {
      str += "\n  //=== " + feature.first + " ===\n";
      for ( auto const & command : commands )
      {
        auto commandIt = m_commands.find( command );
        assert( commandIt != m_commands.end() );
        if ( commandIt->first == "vkCreateInstance" )
        {
          // special handling for createInstance, as we need to explicitly place the forward declarations and the
          // deleter classes here
#if !defined( NDEBUG )
          auto handleIt = m_handles.find( "" );
          assert( ( handleIt != m_handles.end() ) && ( handleIt->second.childrenHandles.size() == 2 ) );
          assert( handleIt->second.childrenHandles.find( "VkInstance" ) != handleIt->second.childrenHandles.end() );
#endif

          str += generateUniqueTypes( "", { "VkInstance" } );
        }
        str += "\n";

        str += generateCommand( commandIt->first, commandIt->second, 0, false );
      }
    }
  }
#if !defined( NDEBUG )
  for ( auto const & extIt : m_extensionsByNumber )
  {
    assert( selectCommandsByHandle( extIt.second->second.requireData, handleData.commands, listedCommands ).empty() );
  }
#endif
  return str;
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
      auto handleIt = m_handles.find( type );
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
    hashes += generateHandleHashStructures( feature.second.requireData, feature.first );
  }
  for ( auto const & extIt : m_extensionsByNumber )
  {
    hashes += generateHandleHashStructures( extIt.second->second.requireData, extIt.second->first );
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
  for ( auto const & handle : m_handles )
  {
    if ( listedHandles.find( handle.first ) == listedHandles.end() )
    {
      str += generateHandle( handle, listedHandles );
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateIndexTypeTraits() const
{
  const std::string indexTypeTraitsTemplate = R"(
  //=========================
  //=== Index Type Traits ===
  //=========================

  template<typename T>
  struct IndexTypeValue
  {};

${indexTypeTraits}
)";

  auto indexType = m_enums.find( "VkIndexType" );
  assert( indexType != m_enums.end() );

  std::string           indexTypeTraits;
  std::set<std::string> listedCppTypes;
  for ( auto const & value : indexType->second.values )
  {
    std::string valueName = generateEnumValueName( indexType->first, value.name, false, m_tags );
    std::string cppType;
    if ( !valueName.starts_with( "eNone" ) )
    {
      // get the bit count out of the value Name (8, 16, 32, ... ) and generate the cppType (uint8_t,...)
      assert( valueName.starts_with( "eUint" ) );
      auto beginDigit = valueName.begin() + strlen( "eUint" );
      assert( isdigit( *beginDigit ) );
      auto endDigit = std::find_if_not( beginDigit, valueName.end(), []( std::string::value_type c ) { return isdigit( c ); } );
      cppType       = "uint" + valueName.substr( strlen( "eUint" ), endDigit - beginDigit ) + "_t";
    }

    if ( !cppType.empty() )
    {
      if ( listedCppTypes.insert( cppType ).second )
      {
        // IndexType traits aren't necessarily invertible.
        // The Type -> Enum translation will only occur for the first prefixed enum value.
        // A hypothetical extension to this enum with a conflicting prefix will use the core spec value.
        const std::string typeToEnumTemplate = R"(
  template <>
  struct IndexTypeValue<${cppType}>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR IndexType value = IndexType::${valueName};
  };
)";
        indexTypeTraits += replaceWithMap( typeToEnumTemplate, { { "cppType", cppType }, { "valueName", valueName } } );
      }

      // Enum -> Type translations are always able to occur.
      const std::string enumToTypeTemplate = R"(
  template <>
  struct CppType<IndexType, IndexType::${valueName}>
  {
    using Type = ${cppType};
  };
)";
      indexTypeTraits += replaceWithMap( enumToTypeTemplate, { { "cppType", cppType }, { "valueName", valueName } } );
    }
  }

  return replaceWithMap( indexTypeTraitsTemplate, { { "indexTypeTraits", indexTypeTraits } } );
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
    assert( ( arrayIt->len.front() == litit->first->name ) || ( ( arrayIt->len.front() == "codeSize / 4" ) && ( litit->first->name == "codeSize" ) ) );

    assert( arrayIt->name.starts_with( "p" ) );
    std::string argumentName = startLowerCase( stripPrefix( arrayIt->name, "p" ) ) + "_";

    assert( mit->type.prefix.empty() && mit->type.postfix.empty() );
    initializer = argumentName + ".size()";
    if ( arrayIt->len.front() == "codeSize / 4" )
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
             ( std::find_if( returnParams.begin(),
                             returnParams.end(),
                             [&vectorParams]( size_t rp ) { return vectorParams.find( rp ) != vectorParams.end(); } ) == returnParams.end() ) ) )
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
      objectDeleter = "ObjectRelease";
    }
    else
    {
      throw std::runtime_error( "Found " + commandName + " which requires special handling for the object deleter" );
    }
  }
  else if ( commandName.find( "Allocate" ) != std::string::npos )
  {
    objectDeleter = "ObjectFree";
    allocator     = "allocator, ";
  }
  else
  {
    assert( ( commandName.find( "Create" ) != std::string::npos ) || ( commandName.find( "Register" ) != std::string::npos ) );
    objectDeleter = "ObjectDestroy";
    allocator     = "allocator, ";
  }
  std::string className  = initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "";
  std::string parentName = ( className.empty() || ( commandData.params[returnParam].type.type == "VkDevice" ) ) ? "NoParent" : className;
  return objectDeleter + "<" + parentName + ", Dispatch>( " + ( ( parentName == "NoParent" ) ? "" : "*this, " ) + allocator + "d )";
}

std::pair<std::string, std::string> VulkanHppGenerator::generateProtection( std::string const & protect ) const
{
  return protect.empty() ? std::make_pair( "", "" ) : std::make_pair( "#if defined( " + protect + " )\n", "#endif /*" + protect + "*/\n" );
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
    commandDefinitions += generateRAIICommandDefinitions( feature.second.requireData, listedCommands, feature.first );
  }
  for ( auto const & extIt : m_extensionsByNumber )
  {
    commandDefinitions += generateRAIICommandDefinitions( extIt.second->second.requireData, listedCommands, extIt.second->first );
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
      if ( listedCommands.insert( command ).second )
      {
        str += generateRAIIHandleCommand( command, determineInitialSkipCount( command ), true );
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
    appendRAIIDispatcherCommands( feature.second.requireData,
                                  listedCommands,
                                  feature.first,
                                  contextInitializers,
                                  contextMembers,
                                  deviceAssignments,
                                  deviceMembers,
                                  instanceAssignments,
                                  instanceMembers );
  }
  for ( auto const & extension : m_extensions )
  {
    appendRAIIDispatcherCommands( extension.second.requireData,
                                  listedCommands,
                                  extension.first,
                                  contextInitializers,
                                  contextMembers,
                                  deviceAssignments,
                                  deviceMembers,
                                  instanceAssignments,
                                  instanceMembers );
  }

  std::string contextDispatcherTemplate = R"(
    class ContextDispatcher : public DispatchLoaderBase
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
    class InstanceDispatcher : public DispatchLoaderBase
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
    class DeviceDispatcher : public DispatchLoaderBase
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

std::string VulkanHppGenerator::generateRAIIHandle( std::pair<std::string, HandleData> const & handle,
                                                    std::set<std::string> &                    listedHandles,
                                                    std::set<std::string> const &              specialFunctions ) const
{
  std::string str;
  if ( listedHandles.find( handle.first ) == listedHandles.end() )
  {
    rescheduleRAIIHandle( str, handle, listedHandles, specialFunctions );

    auto [enter, leave]    = generateProtection( handle.second.alias.empty() ? getProtectFromType( handle.first ) : "" );
    std::string handleType = stripPrefix( handle.first, "Vk" );
    std::string handleName = generateRAIIHandleConstructorParamName( handle.first, handle.second.destructorIt );

    auto [singularConstructors, arrayConstructors] = generateRAIIHandleConstructors( handle );

    auto [clearMembers, getConstructorSuccessCode, memberVariables, moveConstructorInitializerList, moveAssignmentInstructions, swapMembers, releaseMembers] =
      generateRAIIHandleDetails( handle );

    std::string declarations = generateRAIIHandleCommandDeclarations( handle, specialFunctions );

    assert( !handle.second.objTypeEnum.empty() );
    auto enumIt = m_enums.find( "VkObjectType" );
    assert( enumIt != m_enums.end() );
    auto valueIt = std::find_if(
      enumIt->second.values.begin(), enumIt->second.values.end(), [&handle]( EnumValueData const & evd ) { return evd.name == handle.second.objTypeEnum; } );
    assert( valueIt != enumIt->second.values.end() );
    std::string objTypeEnum = generateEnumValueName( enumIt->first, valueIt->name, false, m_tags );

    enumIt = m_enums.find( "VkDebugReportObjectTypeEXT" );
    assert( enumIt != m_enums.end() );
    std::string valueName = handle.second.objTypeEnum;
    valueName             = valueName.replace( 3, 0, "DEBUG_REPORT_" ) + "_EXT";
    valueIt =
      std::find_if( enumIt->second.values.begin(), enumIt->second.values.end(), [&valueName]( EnumValueData const & evd ) { return valueName == evd.name; } );
    std::string debugReportObjectType =
      ( valueIt != enumIt->second.values.end() ) ? generateEnumValueName( enumIt->first, valueIt->name, false, m_tags ) : "eUnknown";

    std::string dispatcherType = ( ( handle.first == "VkDevice" ) || ( handle.second.constructorIts.front()->second.params.front().type.type == "VkDevice" ) )
                                 ? "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher"
                                 : "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher";

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
  if ( m_RAIISpecialFunctions.find( command ) == m_RAIISpecialFunctions.end() )
  {
    auto commandIt = m_commands.find( command );
    assert( commandIt != m_commands.end() );

    if ( commandIt->second.returnType == "VkResult" )
    {
      str = generateRAIIHandleCommandResult( commandIt, initialSkipCount, definition );
    }
    else if ( commandIt->second.returnType == "void" )
    {
      str = generateRAIIHandleCommandVoid( commandIt, initialSkipCount, definition );
    }
    else
    {
      str = generateRAIIHandleCommandValue( commandIt, initialSkipCount, definition );
    }

    if ( str.empty() )
    {
      throw std::runtime_error( "Never encountered a command like <" + command + "> !" );
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateRAIIHandleCommandDeclarations( std::pair<std::string, HandleData> const & handle,
                                                                       std::set<std::string> const &              specialFunctions ) const
{
  std::string           functionDeclarations;
  std::set<std::string> listedCommands;  // some commands are listed with more than one extension !
  for ( auto const & feature : m_features )
  {
    std::vector<std::string> firstLevelCommands, secondLevelCommands;

    for ( auto const & require : feature.second.requireData )
    {
      for ( auto const & command : require.commands )
      {
        if ( specialFunctions.find( command ) == specialFunctions.end() )
        {
          if ( handle.second.commands.find( command ) != handle.second.commands.end() )
          {
            assert( listedCommands.find( command ) == listedCommands.end() );
            listedCommands.insert( command );
            firstLevelCommands.push_back( command );
          }
          else if ( handle.second.secondLevelCommands.find( command ) != handle.second.secondLevelCommands.end() )
          {
            assert( listedCommands.find( command ) == listedCommands.end() );
            listedCommands.insert( command );
            assert( !handle.first.empty() );
            secondLevelCommands.push_back( command );
          }
        }
      }
    }
    if ( !firstLevelCommands.empty() || !secondLevelCommands.empty() )
    {
      functionDeclarations += "\n  //=== " + feature.first + " ===\n";
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

  for ( auto const & extIt : m_extensionsByNumber )
  {
    std::vector<std::string> firstLevelCommands, secondLevelCommands;
    for ( auto & req : extIt.second->second.requireData )
    {
      for ( auto const & command : req.commands )
      {
        if ( ( specialFunctions.find( command ) == specialFunctions.end() ) && ( listedCommands.find( command ) == listedCommands.end() ) )
        {
          if ( handle.second.commands.find( command ) != handle.second.commands.end() )
          {
            listedCommands.insert( command );
            firstLevelCommands.push_back( command );
          }
          else if ( handle.second.secondLevelCommands.find( command ) != handle.second.secondLevelCommands.end() )
          {
            listedCommands.insert( command );
            secondLevelCommands.push_back( command );
          }
        }
      }
    }
    if ( !firstLevelCommands.empty() || !secondLevelCommands.empty() )
    {
      std::string enter, leave;
      if ( extIt.second->first != m_types.find( handle.first )->second.referencedIn )
      {
        std::tie( enter, leave ) = generateProtection( getProtectFromTitle( extIt.second->first ) );
      }
      functionDeclarations += "\n" + enter + "  //=== " + extIt.second->first + " ===\n";
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

std::string VulkanHppGenerator::generateRAIIHandleCommandEnhanced( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                   size_t                                             initialSkipCount,
                                                                   std::vector<size_t> const &                        returnParams,
                                                                   std::map<size_t, VectorParamData> const &          vectorParams,
                                                                   bool                                               definition,
                                                                   CommandFlavourFlags                                flavourFlags ) const
{
  bool singular = flavourFlags & CommandFlavourFlagBits::singular;

  std::set<size_t> skippedParams  = determineSkippedParams( commandIt->second.params, initialSkipCount, vectorParams, returnParams, singular );
  std::set<size_t> singularParams = singular ? determineSingularParams( returnParams[0], vectorParams ) : std::set<size_t>();
  // special handling for vkGetMemoryHostPointerPropertiesEXT: here, we really need to stick with the const void * parameter !
  std::set<size_t> templatedParams =
    ( commandIt->first == "vkGetMemoryHostPointerPropertiesEXT" ) ? std::set<size_t>() : determineVoidPointerParams( commandIt->second.params );

  bool                     enumerating = determineEnumeration( vectorParams, returnParams );
  std::vector<std::string> dataTypes   = determineDataTypes( commandIt->second.params, vectorParams, returnParams, templatedParams );
  std::string              dataType    = combineDataTypes( vectorParams, returnParams, enumerating, dataTypes, flavourFlags, true );

  std::string argumentTemplates = generateArgumentTemplates( commandIt->second.params, returnParams, vectorParams, templatedParams, flavourFlags, true );
  std::string argumentList      = generateArgumentListEnhanced(
    commandIt->second.params, returnParams, vectorParams, skippedParams, singularParams, templatedParams, definition, flavourFlags, false );
  std::string commandName = generateCommandName( commandIt->first, commandIt->second.params, initialSkipCount, m_tags, flavourFlags );
  std::string nodiscard =
    generateNoDiscard( !returnParams.empty() || ( ( commandIt->second.returnType != "VkResult" ) && ( commandIt->second.returnType != "void" ) ),
                       1 < commandIt->second.successCodes.size(),
                       false );
  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck =
    needsVectorSizeCheck( commandIt->second.params, vectorParams, returnParams, singularParams );
  std::string noexceptString = generateNoExcept( commandIt->second.errorCodes, returnParams, vectorParams, flavourFlags, vectorSizeCheck.first, true );
  std::string returnType     = generateReturnType( commandIt->second, returnParams, vectorParams, flavourFlags, true, dataType );

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
      commandIt->first, commandIt->second, returnParams, vectorParams, initialSkipCount, singularParams, templatedParams, flavourFlags, true );
    std::string className        = initialSkipCount ? stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) : "Context";
    std::string returnVariable   = generateReturnVariable( commandIt->second, returnParams, vectorParams, flavourFlags );
    std::string dataDeclarations = generateDataDeclarations(
      commandIt->second, returnParams, vectorParams, templatedParams, flavourFlags, true, dataTypes, dataType, returnType, returnVariable );
    std::string dataPreparation =
      generateDataPreparation( commandIt->second, initialSkipCount, returnParams, vectorParams, templatedParams, flavourFlags, enumerating );
    std::string dataSizeChecks  = generateDataSizeChecks( commandIt->second, returnParams, dataTypes, vectorParams, templatedParams, singular );
    std::string resultCheck     = generateResultCheck( commandIt->second, className, "::", commandName, enumerating );
    std::string returnStatement = generateReturnStatement( commandIt->first,
                                                           commandIt->second,
                                                           returnVariable,
                                                           returnType,
                                                           dataType,
                                                           initialSkipCount,
                                                           returnParams.empty() ? INVALID_INDEX : returnParams[0],
                                                           flavourFlags,
                                                           enumerating,
                                                           true );
    std::string vectorSizeCheckString =
      vectorSizeCheck.first ? generateRAIIHandleVectorSizeCheck( commandIt->first, commandIt->second, initialSkipCount, vectorSizeCheck.second, skippedParams )
                            : "";

    return replaceWithMap( definitionTemplate,
                           { { "argumentList", argumentList },
                             { "argumentTemplates", argumentTemplates },
                             { "callSequence", callSequence },
                             { "className", className },
                             { "commandName", commandName },
                             { "dataDeclarations", dataDeclarations },
                             { "dataPreparation", dataPreparation },
                             { "dataSizeChecks", dataSizeChecks },
                             { "functionPointerCheck", generateFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
                             { "nodiscard", nodiscard },
                             { "noexcept", noexceptString },
                             { "resultCheck", resultCheck },
                             { "returnStatement", returnStatement },
                             { "returnType", returnType },
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
                             { "returnType", returnType } } );
  }
}

std::string VulkanHppGenerator::generateRAIIHandleCommandFactory( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                  size_t                                             initialSkipCount,
                                                                  std::vector<size_t> const &                        returnParams,
                                                                  std::map<size_t, VectorParamData> const &          vectorParams,
                                                                  bool                                               definition,
                                                                  CommandFlavourFlags                                flavourFlags ) const
{
  bool singular = flavourFlags & CommandFlavourFlagBits::singular;

  assert( isHandleType( commandIt->second.params[returnParams.back()].type.type ) );
  assert( ( returnParams.size() == 1 ) ||
          ( ( returnParams.size() == 2 ) && ( vectorParams.size() == 1 ) && ( returnParams[0] == vectorParams.begin()->second.lenParam ) &&
            ( returnParams[1] == vectorParams.begin()->first ) ) );

  std::set<size_t> skippedParams  = determineSkippedParams( commandIt->second.params, initialSkipCount, vectorParams, returnParams, singular );
  std::set<size_t> singularParams = singular ? determineSingularParams( returnParams.back(), vectorParams ) : std::set<size_t>();
  std::string      argumentList   = generateRAIIHandleCommandFactoryArgumentList( commandIt->second.params, skippedParams, definition, singular );
  std::string      commandName    = generateCommandName( commandIt->first, commandIt->second.params, initialSkipCount, m_tags, flavourFlags );
  std::string      handleType     = stripPostfix( commandIt->second.params[returnParams.back()].type.compose( "VULKAN_HPP_RAII_NAMESPACE" ), " *" );
  std::string      returnType     = handleType;
  if ( ( vectorParams.find( returnParams.back() ) != vectorParams.end() ) && !singular )
  {
    returnType = "std::vector<" + handleType + ">";
    handleType += "s";
  }

  if ( definition )
  {
    std::string callArguments = generateCallArgumentsRAIIFactory( commandIt->second.params, initialSkipCount, skippedParams, singularParams );
    std::string className     = initialSkipCount ? stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) : "Context";

    std::string const definitionTemplate =
      R"(
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ${returnType} ${className}::${commandName}( ${argumentList} ) const
  {
    return ${handleType}( ${callArguments} );
  }
)";

    return replaceWithMap( definitionTemplate,
                           { { "argumentList", argumentList },
                             { "callArguments", callArguments },
                             { "className", className },
                             { "commandName", commandName },
                             { "handleType", handleType },
                             { "returnType", returnType } } );
  }
  else
  {
    std::string const declarationTemplate =
      R"(
  VULKAN_HPP_NODISCARD ${returnType} ${commandName}( ${argumentList} ) const;
)";

    return replaceWithMap( declarationTemplate, { { "argumentList", argumentList }, { "commandName", commandName }, { "returnType", returnType } } );
  }
}

std::string VulkanHppGenerator::generateRAIIHandleCommandFactoryArgumentList( std::vector<ParamData> const & params,
                                                                              std::set<size_t> const &       skippedParams,
                                                                              bool                           definition,
                                                                              bool                           singular ) const
{
  std::string arguments;
  bool        encounteredArgument = false;
  for ( size_t i = 0; i < params.size(); ++i )
  {
    if ( skippedParams.find( i ) == skippedParams.end() )
    {
      if ( encounteredArgument )
      {
        arguments += ", ";
      }
      arguments += generateRAIIHandleConstructorArgument( params[i], definition, singular, false );
      encounteredArgument = true;
    }
  }
  return arguments;
}

std::string VulkanHppGenerator::generateRAIIHandleCommandResult( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                 size_t                                             initialSkipCount,
                                                                 bool                                               definition ) const
{
  assert( !commandIt->second.successCodes.empty() );
  if ( commandIt->second.successCodes.size() == 1 )
  {
    if ( commandIt->second.errorCodes.empty() )
    {
      return generateRAIIHandleCommandResultSingleSuccessNoErrors( commandIt, initialSkipCount, definition );
    }
    else
    {
      return generateRAIIHandleCommandResultSingleSuccessWithErrors( commandIt, initialSkipCount, definition );
    }
  }
  else
  {
    if ( commandIt->second.errorCodes.empty() )
    {
      return generateRAIIHandleCommandResultMultiSuccessNoErrors( commandIt, initialSkipCount, definition );
    }
    else
    {
      return generateRAIIHandleCommandResultMultiSuccessWithErrors( commandIt, initialSkipCount, definition );
    }
  }
}

std::string VulkanHppGenerator::generateRAIIHandleCommandResultMultiSuccessNoErrors( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                     size_t                                             initialSkipCount,
                                                                                     bool                                               definition ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandIt->second.params );
  switch ( returnParams.size() )
  {
    case 0:
      return generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, determineVectorParams( commandIt->second.params ), definition );
    case 2: return generateRAIIHandleCommandResultMultiSuccessNoErrors2Return( commandIt, initialSkipCount, definition, returnParams );
  }
  return "";
}

std::string VulkanHppGenerator::generateRAIIHandleCommandResultMultiSuccessNoErrors2Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                            size_t                                             initialSkipCount,
                                                                                            bool                                               definition,
                                                                                            std::vector<size_t> const & returnParams ) const
{
  if ( ( commandIt->second.successCodes.size() == 2 ) && ( commandIt->second.successCodes[0] == "VK_SUCCESS" ) &&
       ( commandIt->second.successCodes[1] == "VK_INCOMPLETE" ) )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandIt->second.params );
    if ( vectorParams.size() == 1 )
    {
      if ( returnParams[0] == vectorParams.begin()->second.lenParam )
      {
        if ( returnParams[1] == vectorParams.begin()->first )
        {
          if ( ( commandIt->second.params[returnParams[0]].type.type == "uint32_t" ) || ( commandIt->second.params[returnParams[0]].type.type == "size_t" ) )
          {
            if ( ( commandIt->second.params[returnParams[1]].type.type != "void" ) && !isHandleType( commandIt->second.params[returnParams[1]].type.type ) &&
                 !isStructureChainAnchor( commandIt->second.params[returnParams[1]].type.type ) )
            {
              return generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition );
            }
          }
        }
      }
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateRAIIHandleCommandResultMultiSuccessWithErrors( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                       size_t                                             initialSkipCount,
                                                                                       bool                                               definition ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandIt->second.params );
  switch ( returnParams.size() )
  {
    case 0:
      return generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, determineVectorParams( commandIt->second.params ), definition );
      break;
    case 1: return generateRAIIHandleCommandResultMultiSuccessWithErrors1Return( commandIt, initialSkipCount, definition, returnParams[0] ); break;
    case 2: return generateRAIIHandleCommandResultMultiSuccessWithErrors2Return( commandIt, initialSkipCount, definition, returnParams ); break;
    case 3: return generateRAIIHandleCommandResultMultiSuccessWithErrors3Return( commandIt, initialSkipCount, definition, returnParams ); break;
  }
  return "";
}

std::string VulkanHppGenerator::generateRAIIHandleCommandResultMultiSuccessWithErrors1Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                              size_t initialSkipCount,
                                                                                              bool   definition,
                                                                                              size_t returnParam ) const
{
  std::string str;
  if ( commandIt->second.params[returnParam].type.type == "void" )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandIt->second.params );
    if ( vectorParams.size() == 1 )
    {
      if ( returnParam == vectorParams.begin()->first )
      {
        if ( commandIt->second.params[vectorParams.begin()->second.lenParam].type.type == "size_t" )
        {
          str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, { returnParam }, vectorParams, definition );
          str += generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, { returnParam }, vectorParams, definition, CommandFlavourFlagBits::singular );
        }
      }
    }
  }
  else if ( isHandleType( commandIt->second.params[returnParam].type.type ) )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandIt->second.params );
    if ( vectorParams.size() == 2 )
    {
      if ( returnParam == std::next( vectorParams.begin() )->first )
      {
        if ( commandIt->second.params[vectorParams.begin()->second.lenParam].type.type == "uint32_t" )
        {
          if ( isStructureChainAnchor( commandIt->second.params[vectorParams.begin()->first].type.type ) )
          {
            str = generateRAIIHandleCommandFactory( commandIt, initialSkipCount, { returnParam }, vectorParams, definition );
            str += generateRAIIHandleCommandFactory( commandIt, initialSkipCount, { returnParam }, vectorParams, definition, CommandFlavourFlagBits::singular );
          }
        }
      }
    }
  }
  else
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandIt->second.params );
    if ( vectorParams.empty() )
    {
      str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, { returnParam }, vectorParams, definition );
      if ( isStructureChainAnchor( commandIt->second.params[returnParam].type.type ) )
      {
        str += generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, { returnParam }, vectorParams, definition, CommandFlavourFlagBits::chained );
      }
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateRAIIHandleCommandResultMultiSuccessWithErrors2Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                              size_t                      initialSkipCount,
                                                                                              bool                        definition,
                                                                                              std::vector<size_t> const & returnParams ) const
{
  if ( ( commandIt->second.successCodes.size() == 2 ) && ( commandIt->second.successCodes[0] == "VK_SUCCESS" ) &&
       ( commandIt->second.successCodes[1] == "VK_INCOMPLETE" ) )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandIt->second.params );
    switch ( vectorParams.size() )
    {
      case 0:
        if ( ( commandIt->second.params[returnParams[0]].type.type != "void" ) && !isHandleType( commandIt->second.params[returnParams[0]].type.type ) &&
             !isStructureChainAnchor( commandIt->second.params[returnParams[0]].type.type ) )
        {
          if ( ( commandIt->second.params[returnParams[1]].type.type != "void" ) && !isHandleType( commandIt->second.params[returnParams[1]].type.type ) &&
               !isStructureChainAnchor( commandIt->second.params[returnParams[1]].type.type ) )
          {
            return generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition );
          }
        }
        break;
      case 1:
        if ( returnParams[0] == vectorParams.begin()->second.lenParam )
        {
          if ( returnParams[1] == vectorParams.begin()->first )
          {
            if ( ( commandIt->second.params[returnParams[0]].type.type == "uint32_t" ) || ( commandIt->second.params[returnParams[0]].type.type == "size_t" ) )
            {
              // needs some very special handling of "vkGetSwapchainImagesKHR" !!
              if ( isHandleType( commandIt->second.params[returnParams[1]].type.type ) && ( commandIt->first != "vkGetSwapchainImagesKHR" ) )
              {
                return generateRAIIHandleCommandFactory( commandIt, initialSkipCount, returnParams, vectorParams, definition );
              }
              else
              {
                std::string str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition );
                if ( isStructureChainAnchor( commandIt->second.params[returnParams[1]].type.type ) )
                {
                  str +=
                    generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition, CommandFlavourFlagBits::chained );
                }
                return str;
              }
            }
          }
        }
        break;
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateRAIIHandleCommandResultMultiSuccessWithErrors3Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                              size_t                      initialSkipCount,
                                                                                              bool                        definition,
                                                                                              std::vector<size_t> const & returnParams ) const
{
  if ( commandIt->second.params[returnParams[0]].type.type == "uint32_t" )
  {
    if ( ( commandIt->second.params[returnParams[1]].type.type != "void" ) && !isHandleType( commandIt->second.params[returnParams[1]].type.type ) &&
         !isStructureChainAnchor( commandIt->second.params[returnParams[1]].type.type ) )
    {
      if ( ( commandIt->second.params[returnParams[2]].type.type != "void" ) && !isHandleType( commandIt->second.params[returnParams[2]].type.type ) &&
           !isStructureChainAnchor( commandIt->second.params[returnParams[2]].type.type ) )
      {
        std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandIt->second.params );
        if ( vectorParams.size() == 2 )
        {
          if ( returnParams[0] == vectorParams.begin()->second.lenParam )
          {
            if ( returnParams[1] == vectorParams.begin()->first )
            {
              if ( returnParams[2] == std::next( vectorParams.begin() )->first )
              {
                if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
                {
                  if ( ( commandIt->second.successCodes.size() == 2 ) && ( commandIt->second.successCodes[0] == "VK_SUCCESS" ) &&
                       ( commandIt->second.successCodes[1] == "VK_INCOMPLETE" ) )
                  {
                    return generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition );
                  }
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

std::string VulkanHppGenerator::generateRAIIHandleCommandResultSingleSuccessNoErrors( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                      size_t                                             initialSkipCount,
                                                                                      bool                                               definition ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandIt->second.params );
  if ( returnParams.size() < 2 )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandIt->second.params );
    return generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition );
  }
  return "";
}

std::string VulkanHppGenerator::generateRAIIHandleCommandResultSingleSuccessWithErrors( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                        size_t                                             initialSkipCount,
                                                                                        bool                                               definition ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandIt->second.params );
  switch ( returnParams.size() )
  {
    case 0:
      return generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, determineVectorParams( commandIt->second.params ), definition );
      break;
    case 1: return generateRAIIHandleCommandResultSingleSuccessWithErrors1Return( commandIt, initialSkipCount, definition, returnParams[0] ); break;
    case 2: return generateRAIIHandleCommandResultSingleSuccessWithErrors2Return( commandIt, initialSkipCount, definition, returnParams ); break;
  }
  return "";
}

std::string VulkanHppGenerator::generateRAIIHandleCommandResultSingleSuccessWithErrors1Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                               size_t initialSkipCount,
                                                                                               bool   definition,
                                                                                               size_t returnParam ) const
{
  std::string                       str;
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandIt->second.params );
  if ( commandIt->second.params[returnParam].type.type == "void" )
  {
    switch ( vectorParams.size() )
    {
      case 0: str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, { returnParam }, vectorParams, definition ); break;
      case 1:
        if ( returnParam == vectorParams.begin()->first )
        {
          if ( commandIt->second.params[vectorParams.begin()->second.lenParam].type.type == "size_t" )
          {
            str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, { returnParam }, vectorParams, definition );
            str +=
              generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, { returnParam }, vectorParams, definition, CommandFlavourFlagBits::singular );
          }
        }
        break;
      case 2:
        if ( returnParam == std::next( vectorParams.begin() )->first )
        {
          if ( commandIt->second.params[vectorParams.begin()->second.lenParam].type.type == "uint32_t" )
          {
            if ( isHandleType( commandIt->second.params[vectorParams.begin()->first].type.type ) )
            {
              if ( commandIt->second.params[std::next( vectorParams.begin() )->second.lenParam].type.type == "size_t" )
              {
                str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, { returnParam }, vectorParams, definition );
                str +=
                  generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, { returnParam }, vectorParams, definition, CommandFlavourFlagBits::singular );
              }
            }
          }
        }
        break;
    }
  }
  else if ( isHandleType( commandIt->second.params[returnParam].type.type ) )
  {
    switch ( vectorParams.size() )
    {
      case 0: str = generateRAIIHandleCommandFactory( commandIt, initialSkipCount, { returnParam }, vectorParams, definition ); break;
      case 1:
        if ( returnParam == vectorParams.begin()->first )
        {
          if ( m_structures.find( commandIt->second.params[vectorParams.begin()->second.lenParam].type.type ) != m_structures.end() )
          {
            str = generateRAIIHandleCommandFactory( commandIt, initialSkipCount, { returnParam }, vectorParams, definition );
          }
        }
        break;
      case 2:
        if ( returnParam == std::next( vectorParams.begin() )->first )
        {
          if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
          {
            if ( commandIt->second.params[vectorParams.begin()->second.lenParam].type.type == "uint32_t" )
            {
              if ( ( commandIt->second.params[vectorParams.begin()->first].type.type != "void" ) &&
                   !isHandleType( commandIt->second.params[vectorParams.begin()->first].type.type ) )
              {
                str = generateRAIIHandleCommandFactory( commandIt, initialSkipCount, { returnParam }, vectorParams, definition );
                str +=
                  generateRAIIHandleCommandFactory( commandIt, initialSkipCount, { returnParam }, vectorParams, definition, CommandFlavourFlagBits::singular );
              }
            }
          }
        }
        break;
    }
  }
  else if ( isStructureChainAnchor( commandIt->second.params[returnParam].type.type ) )
  {
    if ( vectorParams.empty() )
    {
      str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, { returnParam }, vectorParams, definition );
      str += generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, { returnParam }, vectorParams, definition, CommandFlavourFlagBits::chained );
    }
  }
  else if ( vectorParams.empty() )
  {
    str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, { returnParam }, vectorParams, definition );
  }
  return str;
}

std::string VulkanHppGenerator::generateRAIIHandleCommandResultSingleSuccessWithErrors2Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                               size_t                      initialSkipCount,
                                                                                               bool                        definition,
                                                                                               std::vector<size_t> const & returnParams ) const
{
  std::string str;
  if ( commandIt->second.params[returnParams[0]].type.type == "uint64_t" )
  {
    if ( commandIt->second.params[returnParams[1]].type.type == "uint64_t" )
    {
      std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandIt->second.params );
      if ( vectorParams.size() == 2 )
      {
        if ( returnParams[0] == std::next( vectorParams.begin() )->first )
        {
          assert( returnParams[1] != std::next( vectorParams.begin() )->second.lenParam );
          if ( returnParams[1] != vectorParams.begin()->first )
          {
            assert( returnParams[1] != vectorParams.begin()->second.lenParam );
            if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
            {
              if ( commandIt->second.params[vectorParams.begin()->second.lenParam].type.type == "uint32_t" )
              {
                if ( ( commandIt->second.params[vectorParams.begin()->first].type.type != "void" ) &&
                     !isHandleType( commandIt->second.params[vectorParams.begin()->first].type.type ) &&
                     !isStructureChainAnchor( commandIt->second.params[vectorParams.begin()->first].type.type ) )
                {
                  // two returns and two vectors! But one input vector, one output vector of the same size,
                  // and one output value
                  str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition );
                  str +=
                    generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition, CommandFlavourFlagBits::singular );
                }
              }
            }
          }
        }
      }
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateRAIIHandleCommandValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                size_t                                             initialSkipCount,
                                                                bool                                               definition ) const
{
  std::vector<size_t> returnParams = determineReturnParams( commandIt->second.params );
  if ( returnParams.empty() )
  {
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandIt->second.params );
    if ( vectorParams.size() <= 1 )
    {
      return generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition );
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateRAIIHandleCommandVoid( std::map<std::string, CommandData>::const_iterator commandIt,
                                                               size_t                                             initialSkipCount,
                                                               bool                                               definition ) const
{
  std::string                       str;
  std::vector<size_t>               returnParams = determineReturnParams( commandIt->second.params );
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( commandIt->second.params );
  switch ( returnParams.size() )
  {
    case 0: str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition ); break;
    case 1:
      if ( commandIt->second.params[returnParams[0]].type.postfix == "**" )
      {
        // get a pointer to something
        if ( commandIt->second.params[returnParams[0]].type.type == "void" )
        {
          if ( vectorParams.empty() )
          {
            str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition );
          }
        }
      }
      else if ( isHandleType( commandIt->second.params[returnParams[0]].type.type ) )
      {
        if ( vectorParams.empty() )
        {
          str = generateRAIIHandleCommandFactory( commandIt, initialSkipCount, returnParams, vectorParams, definition );
        }
      }
      else if ( commandIt->second.params[returnParams[0]].type.type == "void" )
      {
        str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition, CommandFlavourFlagBits::singular );
      }
      else
      {
        auto returnVectorParamIt = vectorParams.find( returnParams[0] );
        if ( returnVectorParamIt == vectorParams.end() )
        {
          str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition );
          if ( isStructureChainAnchor( commandIt->second.params[returnParams[0]].type.type ) )
          {
            str += generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition, CommandFlavourFlagBits::chained );
          }
        }
      }
      break;
    case 2:
      if ( commandIt->second.params[returnParams[0]].type.type == "uint32_t" )
      {
        if ( vectorParams.size() == 1 )
        {
          if ( returnParams[0] == vectorParams.begin()->second.lenParam )
          {
            if ( returnParams[1] == vectorParams.begin()->first )
            {
              str = generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition );
              if ( isStructureChainAnchor( commandIt->second.params[returnParams[1]].type.type ) )
              {
                str +=
                  generateRAIIHandleCommandEnhanced( commandIt, initialSkipCount, returnParams, vectorParams, definition, CommandFlavourFlagBits::chained );
              }
            }
          }
        }
      }
      break;
  }
  return str;
}

std::pair<std::string, std::string>
  VulkanHppGenerator::generateRAIIHandleConstructor( std::pair<std::string, HandleData> const &                             handle,
                                                     std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
                                                     std::string const &                                                    enter,
                                                     std::string const &                                                    leave ) const
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
          auto        lenParamIt    = constructorIt->second.params.begin() + vectorParams.begin()->second.lenParam;
          auto        handleParamIt = constructorIt->second.params.begin() + std::next( vectorParams.begin() )->first;
          if ( !checkEquivalentSingularConstructor( handle.second.constructorIts, constructorIt, lenParamIt ) )
          {
            singularConstructor = generateRAIIHandleConstructorVectorSingular( handle, constructorIt, handleParamIt, enter, leave );
          }
          return std::make_pair( singularConstructor, generateRAIIHandleConstructorVector( handle, constructorIt, handleParamIt, enter, leave ) );
        }
      }
    }
  }
  return std::make_pair( "", "" );
}

std::pair<std::string, std::string> VulkanHppGenerator::generateRAIIHandleConstructors( std::pair<std::string, HandleData> const & handle ) const
{
  auto [enter, leave] = generateProtection( handle.second.alias.empty() ? getProtectFromType( handle.first ) : "" );

  std::string singularConstructors, arrayConstructors;
  for ( auto constructorIt : handle.second.constructorIts )
  {
    // there is a non-const parameter with handle type : the to-be-constructed handle

    // check for additional enter/leave guards for the constructors
    auto [constructorEnter, constructorLeave] = generateProtection( getProtectFromTitle( constructorIt->second.referencedIn ) );
    if ( constructorEnter == enter )
    {
      constructorEnter.clear();
      constructorLeave.clear();
    }

    auto [singularConstructor, arrayConstructor] = generateRAIIHandleConstructor( handle, constructorIt, constructorEnter, constructorLeave );
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
      assert( param.len.empty() );
      argument = "VULKAN_HPP_NAMESPACE::Optional<const " + argumentType + "> " + argumentName + ( definition ? "" : " = nullptr" );
    }
    else if ( param.len.empty() )
    {
      argument = argumentType + " const & " + argumentName;
    }
    else if ( singular )
    {
      argument = argumentType + " const & " + stripPluralS( argumentName, m_tags );
    }
    else
    {
      argument = "VULKAN_HPP_NAMESPACE::ArrayProxy<" + argumentType + "> const & " + argumentName;
    }
  }
  else if ( specialPointerTypes.find( param.type.type ) != specialPointerTypes.end() )
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
    argument = param.type.compose( "VULKAN_HPP_NAMESPACE" ) + " " + param.name;
  }
  return argument;
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorArguments( std::pair<std::string, HandleData> const &                             handle,
                                                                        std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
                                                                        bool                                                                   singular,
                                                                        bool takesOwnership ) const
{
  auto [parentType, parentName] = getParentTypeAndName( handle );

  std::string arguments = "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::" + parentType + " const & " + parentName;
  if ( takesOwnership )
  {
    arguments += ", " + handle.first + " " + generateRAIIHandleConstructorParamName( handle.first, handle.second.destructorIt );
  }

  if ( constructorIt != m_commands.end() )
  {
    parentType = "Vk" + parentType;
    bool skip  = skipLeadingGrandParent( handle );
    for ( size_t i = skip ? 1 : 0; i < constructorIt->second.params.size(); i++ )
    {
      ParamData const & param = constructorIt->second.params[i];
      // filter parent and handle type
      if ( ( param.type.type != parentType ) && ( param.type.type != handle.first ) )
      {
        // the specialPointerTypes are considered const-pointers!
        if ( param.type.isNonConstPointer() && ( specialPointerTypes.find( param.type.type ) == specialPointerTypes.end() ) )
        {
          // this is supposed to be the returned size on an enumeration function!
#if !defined( NDEBUG )
          assert( param.type.type == "uint32_t" );
          auto typeIt = std::find_if( constructorIt->second.params.begin(),
                                      constructorIt->second.params.end(),
                                      [&handle]( ParamData const & pd ) { return pd.type.type == handle.first; } );
          assert( typeIt != constructorIt->second.params.end() );
          assert( typeIt->len == param.name );
#endif
          continue;
        }
        else if ( std::find_if( constructorIt->second.params.begin(),
                                constructorIt->second.params.end(),
                                [&param]( ParamData const & pd ) { return pd.len == param.name; } ) != constructorIt->second.params.end() )
        {
          // this is the len of an other parameter, which will be mapped to an ArrayProxy
          assert( param.type.isValue() && ( param.type.type == "uint32_t" ) );
          assert( param.arraySizes.empty() && param.len.empty() && !param.optional );
          continue;
        }
        arguments += ", " + generateRAIIHandleConstructorArgument( param, false, singular, takesOwnership );
      }
    }
  }
  return arguments;
}

std::string
  VulkanHppGenerator::generateRAIIHandleConstructorCallArguments( std::pair<std::string, HandleData> const &                             handle,
                                                                  std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
                                                                  bool                     nonConstPointerAsNullptr,
                                                                  std::set<size_t> const & singularParams,
                                                                  bool                     allocatorIsMemberVariable ) const
{
  std::string arguments;
  bool        encounteredArgument = false;
  size_t      i                   = 0;
  if ( skipLeadingGrandParent( handle ) )
  {
    assert( ( 1 < constructorIt->second.params.size() ) && ( m_handles.find( constructorIt->second.params[0].type.type ) != m_handles.end() ) &&
            ( m_handles.find( constructorIt->second.params[1].type.type ) != m_handles.end() ) );
    arguments += "static_cast<" + constructorIt->second.params[0].type.type + ">( " + constructorIt->second.params[1].name + ".get" +
                 stripPrefix( constructorIt->second.params[0].type.type, "Vk" ) + "() )";
    encounteredArgument = true;
    i                   = 1;
  }
  for ( ; i < constructorIt->second.params.size(); ++i )
  {
    ParamData const & param = constructorIt->second.params[i];
    if ( encounteredArgument )
    {
      arguments += ", ";
    }
    if ( param.type.type == handle.first )
    {
      assert( param.type.isNonConstPointer() && param.arraySizes.empty() );
      if ( param.len.empty() || !singularParams.empty() )
      {
        assert( !param.optional );
        assert( singularParams.empty() || ( param.len == constructorIt->second.params[*singularParams.begin()].name ) );
        std::string paramName = generateRAIIHandleConstructorParamName( handle.first, handle.second.destructorIt );
        arguments += "reinterpret_cast<" + handle.first + "*>( &m_" + paramName + " )";
      }
      else if ( nonConstPointerAsNullptr )
      {
        arguments += "nullptr";
      }
      else
      {
        arguments += startLowerCase( stripPrefix( param.name, "p" ) ) + ".data()";
      }
    }
    else if ( param.type.type == "VkAllocationCallbacks" )
    {
      assert( param.optional );
      if ( allocatorIsMemberVariable )
      {
        arguments += "reinterpret_cast<const VkAllocationCallbacks *>( m_allocator )";
      }
      else
      {
        arguments += "reinterpret_cast<const VkAllocationCallbacks *>(static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )";
      }
    }
    else if ( m_handles.find( param.type.type ) != m_handles.end() )
    {
      assert( param.type.isValue() && param.arraySizes.empty() && param.len.empty() );
      if ( param.optional )
      {
        arguments += param.name + " ? static_cast<" + param.type.type + ">( **" + param.name + " ) : 0";
      }
      else
      {
        arguments += "static_cast<" + param.type.type + ">( *" + param.name + " )";
      }
    }
    else
    {
      assert( !param.optional );
      arguments += generateCallArgumentEnhanced( constructorIt->second.params, i, nonConstPointerAsNullptr, singularParams, {} );
    }
    encounteredArgument = true;
  }
  return arguments;
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorEnumerate( std::pair<std::string, HandleData> const &                             handle,
                                                                        std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
                                                                        std::vector<ParamData>::const_iterator                                 handleParamIt,
                                                                        std::vector<ParamData>::const_iterator                                 lenParamIt,
                                                                        std::string const &                                                    enter,
                                                                        std::string const &                                                    leave ) const
{
  std::string handleConstructorArguments = generateRAIIHandleSingularConstructorArguments( handle, constructorIt );
  std::string handleType                 = stripPrefix( handle.first, "Vk" );
  std::string dispatcherType             = hasParentHandle( handle.first, "VkDevice" ) ? "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher"
                                                                                       : "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher";

  const std::string constructorTemplate =
    R"(
${enter}    ${handleType}s( ${constructorArguments} )
    {
      ${dispatcherType} const * dispatcher = ${parentName}.getDispatcher();
      std::vector<${vectorElementType}> ${vectorName};
      ${counterType} ${counterName};
      VULKAN_HPP_NAMESPACE::Result result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( dispatcher->${constructorCall}( ${firstCallArguments} ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && ${counterName} )
        {
          ${vectorName}.resize( ${counterName} );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( dispatcher->${constructorCall}( ${secondCallArguments} ) );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      if ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
        VULKAN_HPP_ASSERT( ${counterName} <= ${vectorName}.size() );
        this->reserve( ${counterName} );
        for ( auto const & ${handleName} : ${vectorName} )
        {
          this->emplace_back( ${parentName}, ${handleConstructorArguments} );
        }
      }
      else
      {
        throwResultException( result, "${constructorCall}" );
      }
    }
${leave})";

  return replaceWithMap( constructorTemplate,
                         { { "constructorArguments", generateRAIIHandleConstructorArguments( handle, constructorIt, false, false ) },
                           { "constructorCall", constructorIt->first },
                           { "counterName", startLowerCase( stripPrefix( lenParamIt->name, "p" ) ) },
                           { "counterType", lenParamIt->type.type },
                           { "dispatcherType", dispatcherType },
                           { "enter", enter },
                           { "firstCallArguments", generateRAIIHandleConstructorCallArguments( handle, constructorIt, true, {}, true ) },
                           { "handleConstructorArguments", handleConstructorArguments },
                           { "handleName", startLowerCase( handleType ) },
                           { "handleType", handleType },
                           { "leave", leave },
                           { "parentName", constructorIt->second.params.front().name },
                           { "secondCallArguments", generateRAIIHandleConstructorCallArguments( handle, constructorIt, false, {}, true ) },
                           { "vectorElementType", handleParamIt->type.type },
                           { "vectorName", startLowerCase( stripPrefix( handleParamIt->name, "p" ) ) } } );
}

std::string
  VulkanHppGenerator::generateRAIIHandleConstructorInitializationList( std::pair<std::string, HandleData> const &                             handle,
                                                                       std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
                                                                       std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator destructorIt,
                                                                       bool takesOwnership ) const
{
  auto [parentType, parentName] = getParentTypeAndName( handle );
  std::string handleName        = generateRAIIHandleConstructorParamName( handle.first, destructorIt );

  std::string initializationList;
  if ( destructorIt != m_commands.end() )
  {
    for ( auto destructorParam : destructorIt->second.params )
    {
      if ( destructorParam.type.type == "Vk" + parentType )
      {
        initializationList += "m_" + parentName + "( *" + parentName + " ), ";
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
        assert( destructorParam.type.isConstPointer() && destructorParam.arraySizes.empty() && destructorParam.len.empty() && destructorParam.optional );
        initializationList += "m_allocator( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ), ";
      }
      else if ( isHandleType( destructorParam.type.type ) )
      {
        assert( destructorParam.type.isValue() && destructorParam.arraySizes.empty() && destructorParam.len.empty() && !destructorParam.optional );
        initializationList += "m_" + destructorParam.name + "( ";
        auto constructorParamIt = std::find_if( constructorIt->second.params.begin(),
                                                constructorIt->second.params.end(),
                                                [&destructorParam]( ParamData const & pd ) { return pd.type.type == destructorParam.type.type; } );
        if ( constructorParamIt != constructorIt->second.params.end() )
        {
          assert( constructorParamIt->type.isValue() && constructorParamIt->arraySizes.empty() && constructorParamIt->len.empty() &&
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
            auto structureIt = m_structures.find( constructorParam.type.type );
            if ( structureIt != m_structures.end() )
            {
              auto structureMemberIt = findStructMemberItByType( destructorParam.type.type, structureIt->second.members );
              if ( structureMemberIt != structureIt->second.members.end() )
              {
                assert( constructorParam.type.isConstPointer() && constructorParam.arraySizes.empty() && constructorParam.len.empty() &&
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
      assert( std::find_if( std::next( destructorParamIt ),
                            destructorIt->second.params.end(),
                            [&type]( ParamData const & destructorParam ) { return destructorParam.type.type == type; } ) == destructorIt->second.params.end() );
      if ( !destructorParamIt->type.isValue() )
      {
        return startLowerCase( stripPrefix( stripPluralS( destructorParamIt->name, m_tags ), "p" ) );
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
            case 0:
              return std::make_pair( generateRAIIHandleConstructorResultSingleSuccessWithErrors1Return0Vector( handle, constructorIt, enter, leave ), "" );
            case 1:
              if ( returnParams[0] == vectorParams.begin()->first )
              {
                if ( isLenByStructMember( constructorIt->second.params[vectorParams.begin()->first].len,
                                          constructorIt->second.params[vectorParams.begin()->second.lenParam] ) )
                {
                  auto handleParamIt = constructorIt->second.params.begin() + returnParams[0];
                  return std::make_pair( "", generateRAIIHandleConstructorVector( handle, constructorIt, handleParamIt, enter, leave ) );
                }
              }
              break;
            case 2: return generateRAIIHandleConstructor1Return2Vector( handle, constructorIt, enter, leave, returnParams[0], vectorParams );
          }
        }
      }
      break;
    case 2:
      if ( !constructorIt->second.errorCodes.empty() )
      {
        std::vector<size_t> returnParams = determineReturnParams( constructorIt->second.params );
        switch ( returnParams.size() )
        {
          case 1:
            assert( isHandleType( constructorIt->second.params[returnParams[0]].type.type ) );
            {
              std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructorIt->second.params );
              if ( vectorParams.size() == 2 )
              {
                return generateRAIIHandleConstructor1Return2Vector( handle, constructorIt, enter, leave, returnParams[0], vectorParams );
              }
            }
            break;
          case 2:
            if ( constructorIt->second.params[returnParams[0]].type.type == "uint32_t" )
            {
              assert( isHandleType( constructorIt->second.params[returnParams[1]].type.type ) );
              std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructorIt->second.params );
              if ( vectorParams.size() == 1 )
              {
                if ( returnParams[0] == vectorParams.begin()->second.lenParam )
                {
                  assert( returnParams[1] == vectorParams.begin()->first );
                  assert( constructorIt->second.successCodes[1] == "VK_INCOMPLETE" );
                  auto lenParamIt    = constructorIt->second.params.begin() + returnParams[0];
                  auto handleParamIt = constructorIt->second.params.begin() + returnParams[1];
                  return std::make_pair( "", generateRAIIHandleConstructorEnumerate( handle, constructorIt, handleParamIt, lenParamIt, enter, leave ) );
                }
              }
            }
            break;
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
  }
  return std::make_pair( "", "" );
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorResultSingleSuccessWithErrors1Return0Vector(
  std::pair<std::string, HandleData> const &                             handle,
  std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
  std::string const &                                                    enter,
  std::string const &                                                    leave ) const
{
  auto [parentType, parentName] = getParentTypeAndName( handle );

  std::string getDispatcher = parentName + ".getDispatcher()";
  std::string dispatcherInitializer, dispatcherInit;
  if ( ( handle.first != "VkInstance" ) && ( handle.first != "VkDevice" ) )
  {
    dispatcherInitializer = "m_dispatcher( " + getDispatcher + " )";
  }
  else
  {
    std::string handleType = stripPrefix( handle.first, "Vk" );
    dispatcherInit         = "\n        m_dispatcher.reset( new VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::" + handleType + "Dispatcher( " + parentName +
                     ".getDispatcher()->vkGet" + handleType + "ProcAddr, static_cast<" + handle.first + ">( m_" + startLowerCase( handleType ) + " ) ) );";
  }

  std::string constructorArguments = generateRAIIHandleConstructorArguments( handle, constructorIt, false, false );

  std::string callArguments = generateRAIIHandleConstructorCallArguments( handle, constructorIt, false, {}, handle.second.destructorIt != m_commands.end() );

  std::string initializationList = generateRAIIHandleConstructorInitializationList( handle, constructorIt, handle.second.destructorIt, false );
  if ( !initializationList.empty() && !dispatcherInitializer.empty() )
  {
    initializationList += ", ";
  }

  const std::string constructorTemplate =
    R"(
${enter}    ${handleType}( ${constructorArguments} )
      : ${initializationList}${dispatcherInitializer}
    {
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( ${getDispatcher}->${constructorCall}( ${callArguments} ) );
      if ( ${failureCheck} )
      {
        throwResultException( result, "${constructorCall}" );
      }${dispatcherInit}
    }
${leave})";

  return replaceWithMap( constructorTemplate,
                         { { "callArguments", callArguments },
                           { "constructorArguments", constructorArguments },
                           { "constructorCall", constructorIt->first },
                           { "dispatcherInitializer", dispatcherInitializer },
                           { "dispatcherInit", dispatcherInit },
                           { "enter", enter },
                           { "failureCheck", generateFailureCheck( constructorIt->second.successCodes ) },
                           { "getDispatcher", getDispatcher },
                           { "leave", leave },
                           { "handleType", stripPrefix( handle.first, "Vk" ) },
                           { "initializationList", initializationList } } );
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorTakeOwnership( std::pair<std::string, HandleData> const & handle ) const
{
  std::string handleType = stripPrefix( handle.first, "Vk" );
  std::string handleName = startLowerCase( handleType );

  auto [parentType, parentName] = getParentTypeAndName( handle );

  std::string constructorArguments = generateRAIIHandleConstructorArguments( handle, handle.second.destructorIt, false, true );
  std::string initializationList   = generateRAIIHandleConstructorInitializationList( handle, handle.second.destructorIt, handle.second.destructorIt, true );
  assert( !handle.second.constructorIts.empty() );
  if ( 1 < handle.second.constructorIts[0]->second.successCodes.size() && ( handle.second.constructorIts[0]->second.successCodes[1] != "VK_INCOMPLETE" ) )
  {
#if !defined( NDEBUG )
    for ( size_t i = 1; i < handle.second.constructorIts.size(); ++i )
    {
      assert( 1 < handle.second.constructorIts[i]->second.successCodes.size() );
    }
#endif
    constructorArguments += ", VULKAN_HPP_NAMESPACE::Result successCode = VULKAN_HPP_NAMESPACE::Result::eSuccess";
    initializationList += ", m_constructorSuccessCode( successCode )";
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
    dispatcherInit = "\n        m_dispatcher.reset( new VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::" + handleType + "Dispatcher( " + parentName +
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

std::string VulkanHppGenerator::generateRAIIHandleConstructorVector( std::pair<std::string, HandleData> const &                             handle,
                                                                     std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
                                                                     std::vector<ParamData>::const_iterator                                 handleParamIt,
                                                                     std::string const &                                                    enter,
                                                                     std::string const &                                                    leave ) const
{
  std::string vectorSize;
  auto        lenIt = std::find_if( constructorIt->second.params.begin(),
                             constructorIt->second.params.end(),
                             [&handleParamIt]( ParamData const & pd ) { return pd.name == handleParamIt->len; } );
  if ( lenIt == constructorIt->second.params.end() )
  {
    std::vector<std::string> lenParts = tokenize( handleParamIt->len, "->" );
    assert( lenParts.size() == 2 );
    lenIt = std::find_if(
      constructorIt->second.params.begin(), constructorIt->second.params.end(), [&lenParts]( ParamData const & pd ) { return pd.name == lenParts[0]; } );
#if !defined( NDEBUG )
    assert( lenIt != constructorIt->second.params.end() );
    auto structureIt = m_structures.find( lenIt->type.type );
    assert( structureIt != m_structures.end() );
    assert( isStructMember( lenParts[1], structureIt->second.members ) );
    assert( constructorIt->second.successCodes.size() == 1 );
#endif
    vectorSize = startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + lenParts[1];
  }
  else
  {
    auto arrayIt = std::find_if( constructorIt->second.params.begin(),
                                 constructorIt->second.params.end(),
                                 [&lenIt, &handleParamIt]( ParamData const & pd ) { return ( pd.len == lenIt->name ) && ( pd.name != handleParamIt->name ); } );
    assert( arrayIt != constructorIt->second.params.end() );
    vectorSize = startLowerCase( stripPrefix( arrayIt->name, "p" ) ) + ".size()";
  }

  std::string handleConstructorArguments = generateRAIIHandleSingularConstructorArguments( handle, constructorIt );
  std::string handleType                 = stripPrefix( handle.first, "Vk" );
  std::string successCodePassToElement   = ( 1 < constructorIt->second.successCodes.size() ) ? ", result" : "";

  const std::string constructorTemplate =
    R"(
${enter}    ${handleType}s( ${constructorArguments} )
    {
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * dispatcher = ${parentName}.getDispatcher();
      std::vector<${vectorElementType}> ${vectorName}( ${vectorSize} );
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( dispatcher->${constructorCall}( ${callArguments} ) );
      if ( ${successCheck} )
      {
        this->reserve( ${vectorSize} );
        for ( auto const & ${handleName} : ${vectorName} )
        {
          this->emplace_back( ${parentName}, ${handleConstructorArguments}${successCodePassToElement} );
        }
      }
      else
      {
        throwResultException( result, "${constructorCall}" );
      }
    }
${leave})";

  return replaceWithMap( constructorTemplate,
                         { { "callArguments", generateRAIIHandleConstructorCallArguments( handle, constructorIt, false, {}, false ) },
                           { "constructorArguments", generateRAIIHandleConstructorArguments( handle, constructorIt, false, false ) },
                           { "constructorCall", constructorIt->first },
                           { "enter", enter },
                           { "handleConstructorArguments", handleConstructorArguments },
                           { "handleName", startLowerCase( handleType ) },
                           { "handleType", handleType },
                           { "leave", leave },
                           { "parentName", constructorIt->second.params.front().name },
                           { "successCheck", generateSuccessCheck( constructorIt->second.successCodes ) },
                           { "successCodePassToElement", successCodePassToElement },
                           { "vectorElementType", handleParamIt->type.type },
                           { "vectorName", startLowerCase( stripPrefix( handleParamIt->name, "p" ) ) },
                           { "vectorSize", vectorSize } } );
}

std::string
  VulkanHppGenerator::generateRAIIHandleConstructorVectorSingular( std::pair<std::string, HandleData> const &                             handle,
                                                                   std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
                                                                   std::vector<ParamData>::const_iterator                                 handleParamIt,
                                                                   std::string const &                                                    enter,
                                                                   std::string const &                                                    leave ) const
{
  size_t                            returnParam    = static_cast<size_t>( std::distance( constructorIt->second.params.begin(), handleParamIt ) );
  std::map<size_t, VectorParamData> vectorParams   = determineVectorParams( constructorIt->second.params );
  std::set<size_t>                  singularParams = determineSingularParams( returnParam, vectorParams );

  std::string callArguments      = generateRAIIHandleConstructorCallArguments( handle, constructorIt, false, singularParams, true );
  std::string initializationList = generateRAIIHandleConstructorInitializationList( handle, constructorIt, handle.second.destructorIt, false );
  assert( !initializationList.empty() );
  std::string failureCheck = generateFailureCheck( constructorIt->second.successCodes );
  failureCheck             = std::regex_replace( failureCheck, std::regex( "result" ), "m_constructorSuccessCode" );

  const std::string singularConstructorTemplate =
    R"(
${enter}    ${handleType}( ${constructorArguments} )
      : ${initializationList}, m_dispatcher( ${firstArgument}.getDispatcher() )
    {
      m_constructorSuccessCode = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${constructorCall}( ${callArguments} ) );
      if ( ${failureCheck} )
      {
        throwResultException( m_constructorSuccessCode, "${constructorCall}" );
      }
    }
${leave})";

  return replaceWithMap( singularConstructorTemplate,
                         { { "initializationList", initializationList },
                           { "callArguments", callArguments },
                           { "constructorArguments", generateRAIIHandleConstructorArguments( handle, constructorIt, true, false ) },
                           { "constructorCall", constructorIt->first },
                           { "enter", enter },
                           { "firstArgument", constructorIt->second.params[0].name },
                           { "failureCheck", failureCheck },
                           { "leave", leave },
                           { "handleType", stripPrefix( handle.first, "Vk" ) } } );
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
      return std::make_pair( generateRAIIHandleConstructorVoid1Return0Vector( handle, constructorIt, enter, leave ), "" );
    }
  }
  return std::make_pair( "", "" );
}

std::string
  VulkanHppGenerator::generateRAIIHandleConstructorVoid1Return0Vector( std::pair<std::string, HandleData> const &                             handle,
                                                                       std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
                                                                       std::string const &                                                    enter,
                                                                       std::string const &                                                    leave ) const
{
  std::string callArguments        = generateRAIIHandleConstructorCallArguments( handle, constructorIt, false, {}, true );
  std::string constructorArguments = generateRAIIHandleConstructorArguments( handle, constructorIt, false, false );
  std::string initializationList   = generateRAIIHandleConstructorInitializationList( handle, constructorIt, handle.second.destructorIt, false );
  if ( !initializationList.empty() )
  {
    initializationList += ", ";
  }

  const std::string constructorTemplate =
    R"(
${enter}    ${handleType}( ${constructorArguments} )
      : ${initializationList}m_dispatcher( ${firstArgument}.getDispatcher() )
    {
      getDispatcher()->${constructorCall}( ${callArguments} );
    }
${leave})";

  return replaceWithMap( constructorTemplate,
                         { { "callArguments", callArguments },
                           { "constructorArguments", constructorArguments },
                           { "constructorCall", constructorIt->first },
                           { "enter", enter },
                           { "firstArgument", constructorIt->second.params[0].name },
                           { "leave", leave },
                           { "handleType", stripPrefix( handle.first, "Vk" ) },
                           { "initializationList", initializationList } } );
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
        : m_dispatcher( new VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ContextDispatcher(
            m_dynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" ) ) )
#else
      Context( PFN_vkGetInstanceProcAddr getInstanceProcAddr )
        : m_dispatcher( new VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ContextDispatcher( getInstanceProcAddr ) )
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

      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ContextDispatcher const * getDispatcher() const
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
      VULKAN_HPP_NAMESPACE::DynamicLoader                                                 m_dynamicLoader;
#endif
      std::unique_ptr<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ContextDispatcher> m_dispatcher;
    };

)";

  return replaceWithMap( contextTemplate, { { "memberFunctionDeclarations", generateRAIIHandleCommandDeclarations( handle, specialFunctions ) } } );
}

std::string VulkanHppGenerator::generateRAIIHandleDestructorCallArguments( std::string const &                                handleType,
                                                                           std::map<std::string, CommandData>::const_iterator destructorIt ) const
{
  std::string arguments;
  bool        encounteredArgument = false;
  for ( auto param : destructorIt->second.params )
  {
    if ( encounteredArgument )
    {
      arguments += ", ";
    }
    if ( param.type.type == handleType )
    {
      std::string handleName = param.name;
      if ( param.type.isValue() )
      {
        arguments += "static_cast<" + handleType + ">( m_" + handleName + " )";
      }
      else
      {
        arguments += "reinterpret_cast<" + handleType + " const *>( &m_" + stripPluralS( startLowerCase( stripPrefix( handleName, "p" ) ), m_tags ) + " )";
      }
    }
    else if ( param.type.type == "VkAllocationCallbacks" )
    {
      // vk::AllocationCallbacks is stored as a member of the handle class
      arguments += "reinterpret_cast<const VkAllocationCallbacks *>( m_allocator )";
    }
    else if ( isHandleType( param.type.type ) )
    {
      assert( param.arraySizes.empty() );
      std::string argument = "m_" + param.name;
      if ( param.type.isValue() )
      {
        arguments += "static_cast<" + param.type.type + ">( " + argument + " )";
      }
      else
      {
        assert( param.type.isConstPointer() );
        assert( !param.len.empty() &&
                ( std::find_if( destructorIt->second.params.begin(),
                                destructorIt->second.params.end(),
                                [&param]( ParamData const & pd ) { return pd.name == param.len; } ) != destructorIt->second.params.end() ) );
        arguments += "reinterpret_cast<" + param.type.type + " const *>( &" + argument + " )";
      }
    }
    else
    {
      assert( ( param.type.type == "uint32_t" ) && param.type.isValue() && param.arraySizes.empty() && param.len.empty() && !param.optional );
      assert( std::find_if( destructorIt->second.params.begin(),
                            destructorIt->second.params.end(),
                            [&param]( ParamData const & pd ) { return pd.len == param.name; } ) != destructorIt->second.params.end() );
      arguments += "1";
    }
    encounteredArgument = true;
  }
  return arguments;
}

std::tuple<std::string, std::string, std::string, std::string, std::string, std::string, std::string>
  VulkanHppGenerator::generateRAIIHandleDetails( std::pair<std::string, HandleData> const & handle ) const
{
  std::string getConstructorSuccessCode;
  bool        multiSuccessCodeContructor = isMultiSuccessCodeConstructor( handle.second.constructorIts );
  if ( multiSuccessCodeContructor )
  {
    getConstructorSuccessCode = R"(
    VULKAN_HPP_NAMESPACE::Result getConstructorSuccessCode() const
    {
      return m_constructorSuccessCode;
    }
)";
  }

  auto [parentType, parentName] = getParentTypeAndName( handle );

  std::string handleName = generateRAIIHandleConstructorParamName( handle.first, handle.second.destructorIt );

  std::string clearMembers, moveConstructorInitializerList, moveAssignmentInstructions, memberVariables, swapMembers, releaseMembers;

  if ( handle.second.destructorIt != m_commands.end() )
  {
    moveAssignmentInstructions = "          clear();";

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
      else if ( std::find_if( handle.second.destructorIt->second.params.begin(),
                              handle.second.destructorIt->second.params.end(),
                              [&destructorParam]( ParamData const & pd )
                              { return pd.len == destructorParam.name; } ) == handle.second.destructorIt->second.params.end() )
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
        moveConstructorInitializerList += "m_" + memberName + "( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_" + memberName + ", {} ) ), ";
        moveAssignmentInstructions +=
          "\n          m_" + memberName + " = VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_" + memberName + ", {} );";
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
      moveConstructorInitializerList = "m_" + frontName + "( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_" + frontName + ", {} ) ), ";
      moveAssignmentInstructions =
        "\n          m_" + frontName + " = VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_" + frontName + ", {} );";
      memberVariables = "\n    VULKAN_HPP_NAMESPACE::" + stripPrefix( frontType, "Vk" ) + " m_" + frontName + " = {};";
      swapMembers     = "\n      std::swap( m_" + frontName + ", rhs.m_" + frontName + " );";
      releaseMembers += "\n        m_" + frontName + " = nullptr;";
    }
    clearMembers += "\n        m_" + handleName + " = nullptr;";
    moveConstructorInitializerList += "m_" + handleName + "( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_" + handleName + ", {} ) ), ";
    moveAssignmentInstructions +=
      "\n          m_" + handleName + " = VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_" + handleName + ", {} );";
    memberVariables += "\n    " + generateNamespacedType( handle.first ) + " m_" + handleName + " = {};";
    swapMembers += "\n      std::swap( m_" + handleName + ", rhs.m_" + handleName + " );";
  }

  if ( multiSuccessCodeContructor )
  {
    clearMembers += "\n        m_constructorSuccessCode = VULKAN_HPP_NAMESPACE::Result::eErrorUnknown;";
    memberVariables += "\n    VULKAN_HPP_NAMESPACE::Result m_constructorSuccessCode = VULKAN_HPP_NAMESPACE::Result::eErrorUnknown;";
    swapMembers += "\n      std::swap( m_constructorSuccessCode, rhs.m_constructorSuccessCode );";
    moveConstructorInitializerList +=
      "m_constructorSuccessCode( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_constructorSuccessCode, {} ) ), ";
    moveAssignmentInstructions +=
      "\n          m_constructorSuccessCode = VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_constructorSuccessCode, {} );";
    releaseMembers += "\n        m_constructorSuccessCode = VULKAN_HPP_NAMESPACE::Result::eErrorUnknown;";
  }

  if ( handle.first == "VkInstance" )
  {
    memberVariables += "\n      std::unique_ptr<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher> m_dispatcher;";
  }
  else if ( handle.first == "VkDevice" )
  {
    memberVariables += "\n      std::unique_ptr<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher> m_dispatcher;";
  }
  else if ( handle.second.constructorIts.front()->second.params.front().type.type == "VkDevice" )
  {
    memberVariables += "\n      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher = nullptr;";
  }
  else
  {
    memberVariables += "\n      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher const * m_dispatcher = nullptr;";
  }
  clearMembers += "\n        m_dispatcher = nullptr;";
  swapMembers += "\n      std::swap( m_dispatcher, rhs.m_dispatcher );";
  releaseMembers += "\n        m_dispatcher = nullptr;";
  releaseMembers += "\n        return VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( m_" + handleName + ", nullptr );";

  if ( ( handle.first == "VkInstance" ) || ( handle.first == "VkDevice" ) )
  {
    moveConstructorInitializerList += "m_dispatcher( rhs.m_dispatcher.release() )";
    moveAssignmentInstructions += "\n        m_dispatcher.reset( rhs.m_dispatcher.release() );";
  }
  else
  {
    moveConstructorInitializerList += "m_dispatcher( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr ) )";
    moveAssignmentInstructions += "\n        m_dispatcher = VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_dispatcher, nullptr );";
  }

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
      auto handleIt = m_handles.find( type );
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

${raiiHandles}
)";

  std::string forwardDeclarations;
  for ( auto const & feature : m_features )
  {
    forwardDeclarations += generateRAIIHandleForwardDeclarations( feature.second.requireData, feature.first );
  }
  for ( auto const & extIt : m_extensionsByNumber )
  {
    forwardDeclarations += generateRAIIHandleForwardDeclarations( extIt.second->second.requireData, extIt.second->first );
  }

  std::set<std::string> listedHandles;
  auto                  handleIt = m_handles.begin();
  assert( handleIt->first.empty() );
  std::string raiiHandles = generateRAIIHandleContext( *handleIt, m_RAIISpecialFunctions );
  for ( ++handleIt; handleIt != m_handles.end(); ++handleIt )
  {
    raiiHandles += generateRAIIHandle( *handleIt, listedHandles, m_RAIISpecialFunctions );
  }
  return replaceWithMap( raiiHandlesTemplate, { { "forwardDeclarations", forwardDeclarations }, { "raiiHandles", raiiHandles } } );
}

std::string VulkanHppGenerator::generateRAIIHandleSingularConstructorArguments( std::pair<std::string, HandleData> const &         handle,
                                                                                std::map<std::string, CommandData>::const_iterator constructorIt ) const
{
  std::string arguments = startLowerCase( stripPrefix( handle.first, "Vk" ) );
  if ( handle.second.destructorIt != m_commands.end() )
  {
    auto [parentType, parentName] = getParentTypeAndName( handle );
    parentType                    = "Vk" + parentType;
    for ( auto const & destructorParam : handle.second.destructorIt->second.params )
    {
      if ( ( destructorParam.type.type != parentType ) && ( destructorParam.type.type != handle.first ) &&
           ( std::find_if( handle.second.destructorIt->second.params.begin(),
                           handle.second.destructorIt->second.params.end(),
                           [&destructorParam]( ParamData const & pd )
                           { return pd.len == destructorParam.name; } ) == handle.second.destructorIt->second.params.end() ) )
      {
        if ( std::find_if( constructorIt->second.params.begin(),
                           constructorIt->second.params.end(),
                           [&destructorParam]( ParamData const & pd )
                           { return pd.type.type == destructorParam.type.type; } ) != constructorIt->second.params.end() )
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
          for ( auto const & constructorParam : constructorIt->second.params )
          {
            auto structureIt = m_structures.find( constructorParam.type.type );
            if ( structureIt != m_structures.end() )
            {
              auto memberIt = findStructMemberItByType( destructorParam.type.type, structureIt->second.members );
              if ( memberIt != structureIt->second.members.end() )
              {
#if !defined( NDEBUG )
                found = true;
#endif
                assert( !constructorParam.type.isValue() );
                std::string argument = startLowerCase( stripPrefix( constructorParam.name, "p" ) ) + "." + memberIt->name;
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

std::string VulkanHppGenerator::generateRAIIHandleVectorSizeCheck( std::string const &                           name,
                                                                   CommandData const &                           commandData,
                                                                   size_t                                        initialSkipCount,
                                                                   std::map<size_t, std::vector<size_t>> const & countToVectorMap,
                                                                   std::set<size_t> const &                      skippedParams ) const
{
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

  std::string const throwTemplateByLen = R"#(    if ( ${vectorName}.size() != ${sizeValue} )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::${className}::${commandName}: ${vectorName}.size() != ${sizeValue}" );
    })#";

  std::string className   = stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" );
  std::string commandName = generateCommandName( name, commandData.params, initialSkipCount, m_tags );

  std::string sizeChecks;
  for ( auto const & cvm : countToVectorMap )
  {
    size_t      defaultStartIndex = determineDefaultStartIndex( commandData.params, skippedParams );
    std::string firstVectorName   = startLowerCase( stripPrefix( commandData.params[cvm.second[0]].name, "p" ) );

    if ( cvm.second.size() == 1 )
    {
      assert( isLenByStructMember( commandData.params[cvm.second[0]].len, commandData.params[cvm.first] ) );

      std::vector<std::string> lenParts = tokenize( commandData.params[cvm.second[0]].len, "->" );
      assert( lenParts.size() == 2 );
      std::string sizeValue = startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + lenParts[1];
      sizeChecks += replaceWithMap(
        throwTemplateByLen, { { "className", className }, { "commandName", commandName }, { "sizeValue", sizeValue }, { "vectorName", firstVectorName } } );
    }
    else
    {
      for ( size_t i = 1; i < cvm.second.size(); i++ )
      {
        std::string secondVectorName  = startLowerCase( stripPrefix( commandData.params[cvm.second[i]].name, "p" ) );
        bool        withZeroSizeCheck = commandData.params[cvm.second[i]].optional && ( defaultStartIndex <= cvm.second[i] );
        if ( commandData.params[cvm.second[0]].type.type == "void" )
        {
          assert( commandData.params[cvm.second[i]].type.type == "void" );
          std::string firstDataType  = stripPrefix( commandData.params[cvm.second[0]].name, "p" ) + "Type";
          std::string secondDataType = stripPrefix( commandData.params[cvm.second[i]].name, "p" ) + "Type";
          sizeChecks += replaceWithMap( throwTemplateVoid,
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
          sizeChecks += replaceWithMap( throwTemplate,
                                        { { "firstVectorName", firstVectorName },
                                          { "className", className },
                                          { "commandName", commandName },
                                          { "secondVectorName", secondVectorName },
                                          { "zeroSizeCheck", withZeroSizeCheck ? ( "!" + secondVectorName + ".empty() && " ) : "" } } );
        }
        if ( i + 1 < cvm.second.size() )
        {
          sizeChecks += "\n";
        }
      }
    }
  }
  if ( !sizeChecks.empty() )
  {
    sizeChecks += "\n";
  }

  return sizeChecks;
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
      R"(resultCheck( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}"${successCodeList} );)";

    resultCheck = replaceWithMap(
      resultCheckTemplate,
      { { "className", className }, { "classSeparator", classSeparator }, { "commandName", commandName }, { "successCodeList", successCodeList } } );
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
    if ( value.name.starts_with( "VK_ERROR" ) )
    {
      auto [enter, leave]   = generateProtection( getProtect( value ) );
      std::string valueName = generateEnumValueName( enumIt->first, value.name, false, m_tags );
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
                                                         std::string const & dataType,
                                                         size_t              initialSkipCount,
                                                         size_t              returnParam,
                                                         CommandFlavourFlags flavourFlags,
                                                         bool                enumerating,
                                                         bool                raii ) const
{
  bool unique = flavourFlags & CommandFlavourFlagBits::unique;

  std::string returnStatement;
  if ( commandData.returnType.starts_with( "Vk" ) )
  {
    if ( ( commandData.successCodes.size() == 1 ) || enumerating )
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
          returnStatement = "return createResultValueType( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ) );";
        }
        else if ( unique )
        {
          assert( returnParam != INVALID_INDEX );
          returnStatement = "return createResultValueType( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), ";
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
          returnStatement = "return createResultValueType( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), " + returnVariable + " );";
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
        assert( returnType.starts_with( "ResultValue<" ) && returnType.ends_with( ">" ) );
        returnStatement = "return " + returnType + "( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), ";
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
        assert( returnType.starts_with( raii ? "std::pair<VULKAN_HPP_NAMESPACE::Result, " : "ResultValue<" ) && returnType.ends_with( ">" ) );
        returnStatement =
          "return " + ( raii ? "std::make_pair" : returnType ) + "( static_cast<VULKAN_HPP_NAMESPACE::Result>( result ), " + returnVariable + " );";
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

std::string VulkanHppGenerator::generateReturnType( CommandData const &                       commandData,
                                                    std::vector<size_t> const &               returnParams,
                                                    std::map<size_t, VectorParamData> const & vectorParams,
                                                    CommandFlavourFlags                       flavourFlags,
                                                    bool                                      raii,
                                                    std::string const &                       dataType ) const
{
  bool chained = flavourFlags & CommandFlavourFlagBits::chained;
  bool unique  = flavourFlags & CommandFlavourFlagBits::unique;

  std::string modifiedDataType = dataType;
  if ( chained )
  {
    assert( !unique );
    modifiedDataType = dataType.starts_with( "std::vector" )
                       ? ( std::string( "std::vector<StructureChain" ) + ( raii ? "" : ", StructureChainAllocator" ) + ">" )
                       : "StructureChain<X, Y, Z...>";
  }
  else if ( unique )
  {
    assert( !chained );
    assert( ( returnParams.size() == 1 ) && isHandleType( commandData.params[returnParams[0]].type.type ) );
    if ( dataType.starts_with( "std::vector" ) )
    {
      auto from = dataType.find( '<' ) + 1;
      assert( from != std::string::npos );
      auto to = dataType.find( '>', from );
      assert( to == dataType.length() - 1 );
      std::string type = dataType.substr( from, to - from );
      assert( type.starts_with( "VULKAN_HPP_NAMESPACE::" ) );
      modifiedDataType.replace( from, to - from, "UniqueHandle<" + type + ", Dispatch>, " + stripPrefix( type, "VULKAN_HPP_NAMESPACE::" ) + "Allocator" );
    }
    else
    {
      assert( !dataType.starts_with( "std::" ) );
      modifiedDataType = "UniqueHandle<" + dataType + ", Dispatch>";
    }
  }

  std::string returnType;
  if ( ( 1 < commandData.successCodes.size() ) && returnParams.empty() && !chained )
  {
    assert( ( commandData.returnType == "VkResult" ) && !unique );
    returnType = "VULKAN_HPP_NAMESPACE::Result";
  }
  else if ( ( commandData.returnType != "VkResult" ) && ( commandData.returnType != "void" ) )
  {
    assert( returnParams.empty() && !chained && !unique );
    if ( commandData.returnType.starts_with( "Vk" ) )
    {
      returnType = generateNamespacedType( commandData.returnType );
    }
    else
    {
      returnType = commandData.returnType;
    }
  }
  else if ( ( commandData.returnType == "void" ) ||
            ( ( commandData.returnType == "VkResult" ) && ( commandData.successCodes.size() == 1 ) && ( commandData.errorCodes.empty() || raii ) ) )
  {
    assert( !unique );
    assert( ( commandData.returnType != "void" ) || ( returnParams.size() <= 2 ) );
    returnType = modifiedDataType;
  }
  else
  {
    assert( commandData.returnType == "VkResult" );
    assert( !commandData.successCodes.empty() && ( commandData.successCodes[0] == "VK_SUCCESS" ) );
    if ( ( 1 < commandData.successCodes.size() ) && ( ( returnParams.size() == 1 ) || ( ( returnParams.size() == 2 ) && vectorParams.empty() ) ) )
    {
      assert( !commandData.errorCodes.empty() );
      returnType = ( raii ? "std::pair<VULKAN_HPP_NAMESPACE::Result, " : "ResultValue<" ) + modifiedDataType + ">";
    }
    else
    {
      assert(
        ( ( commandData.successCodes.size() == 1 ) || ( ( commandData.successCodes.size() == 2 ) && ( commandData.successCodes[1] == "VK_INCOMPLETE" ) ) ) &&
        ( returnParams.size() <= 3 ) );
      returnType = raii ? modifiedDataType : ( "typename ResultValueType<" + modifiedDataType + ">::type" );
    }
  }
  return returnType;
}

std::string VulkanHppGenerator::generateReturnVariable( CommandData const &                       commandData,
                                                        std::vector<size_t> const &               returnParams,
                                                        std::map<size_t, VectorParamData> const & vectorParams,
                                                        CommandFlavourFlags                       flavourFlags ) const
{
  bool chained  = flavourFlags & CommandFlavourFlagBits::chained;
  bool singular = flavourFlags & CommandFlavourFlagBits::singular;

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
        returnVariable = startLowerCase( stripPrefix( commandData.params[returnParams[0]].name, "p" ) );
        if ( singular )
        {
          returnVariable = stripPluralS( returnVariable, m_tags );
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
        {  // the return variable is simply named "data", and holds the multi-return value stuff
          returnVariable = "data";
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
      assert( !chained && !singular );
      assert( ( vectorParams.size() == 2 ) && ( vectorParams.begin()->first == returnParams[1] ) &&
              ( vectorParams.begin()->second.lenParam == returnParams[0] ) && ( std::next( vectorParams.begin() )->first == returnParams[2] ) &&
              ( std::next( vectorParams.begin() )->second.lenParam == returnParams[0] ) );
      returnVariable = "data";
      break;
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
  std::string staticAssertions;
  for ( auto const & feature : m_features )
  {
    staticAssertions += generateStaticAssertions( feature.second.requireData, feature.first );
  }
  for ( auto const & extIt : m_extensionsByNumber )
  {
    staticAssertions += generateStaticAssertions( extIt.second->second.requireData, extIt.second->first );
  }
  return staticAssertions;
}

std::string VulkanHppGenerator::generateStaticAssertions( std::vector<RequireData> const & requireData, std::string const & title ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto handleIt = m_handles.find( type );
      if ( handleIt != m_handles.end() )
      {
        std::string const assertionTemplate = R"(
VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::${className} ) == sizeof( Vk${className} ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::${className}>::value, "${className} is not nothrow_move_constructible!" );
)";
        str += replaceWithMap( assertionTemplate, { { "className", stripPrefix( handleIt->first, "Vk" ) } } );
      }
      else
      {
        auto structIt = m_structures.find( type );
        if ( structIt != m_structures.end() )
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
  assert( listedStructs.find( structure.first ) == listedStructs.end() );

  std::string str;
  for ( auto const & member : structure.second.members )
  {
    auto structIt = m_structures.find( member.type.type );
    if ( ( structIt != m_structures.end() ) && ( structure.first != member.type.type ) && ( listedStructs.find( member.type.type ) == listedStructs.end() ) )
    {
      str += generateStruct( *structIt, listedStructs );
    }
  }

  if ( !structure.second.subStruct.empty() )
  {
    auto structureIt = m_structures.find( structure.second.subStruct );
    if ( ( structureIt != m_structures.end() ) && ( listedStructs.find( structureIt->first ) == listedStructs.end() ) )
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
    if ( ( typeIt->second.category == TypeCategory::Requires ) && member.type.postfix.empty() && ( simpleTypes.find( member.type.type ) == simpleTypes.end() ) )
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
    else if ( member.type.type == "char" && !member.len.empty() )
    {
      // compare null-terminated strings
      nonDefaultCompare = true;
      assert( member.len.size() < 3 );
      if ( member.len.size() == 1 )
      {
        assert( member.len[0] == "null-terminated" );
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
        assert( member.len[1] == "null-terminated" );
        assert( ( member.type.prefix == "const" ) && ( member.type.postfix == "* const *" ) );
        static const std::string compareMemberTemplate =
          R"(std::equal( ${name}, ${name} + ${count}, rhs.${name}, []( char const * left, char const * right ) { return ( left == right ) || ( strcmp( left, right ) == 0 ); } ))";
        compareMembers += intro + replaceWithMap( compareMemberTemplate, { { "count", member.len[0] }, { "name", member.name } } );

        static const std::string spaceshipMemberTemplate = R"(      for ( size_t i = 0; i < ${count}; ++i )
      {
        if ( ${name}[i] != rhs.${name}[i] )
          if ( auto cmp = strcmp( ${name}[i], rhs.${name}[i] ); cmp != 0 )
            return cmp < 0 ? ${ordering}::less : ${ordering}::greater;
      }
)";
        spaceshipMembers +=
          replaceWithMap( spaceshipMemberTemplate, { { "count", member.len[0] }, { "name", member.name }, { "ordering", spaceshipOrdering } } );
      }
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

  std::string arguments, initializers;
  bool        listedArgument = false;
  bool        firstArgument  = true;
  for ( auto const & member : structData.second.members )
  {
    // gather the arguments
    std::string argument = generateStructConstructorArgument( listedArgument, member, true );
    if ( !argument.empty() )
    {
      listedArgument = true;
      arguments += argument;
    }

    // gather the initializers; skip members with exactly one legal value
    if ( member.value.empty() )
    {
      initializers += std::string( firstArgument ? ": " : ", " ) + member.name + "( " + member.name + "_ )";
      firstArgument = false;
    }
  }
  auto pNextIt = std::find_if( structData.second.members.begin(), structData.second.members.end(), []( MemberData const & md ) { return md.name == "pNext"; } );
  if ( pNextIt != structData.second.members.end() )
  {
    // add pNext as a last optional argument to the constructor
    arguments += std::string( listedArgument ? ", " : "" ) + pNextIt->type.compose( "VULKAN_HPP_NAMESPACE" ) + " pNext_ = nullptr";
  }

  std::string str = replaceWithMap( constructors,
                                    { { "arguments", arguments },
                                      { "constexpr", generateConstexprString( structData.first ) },
                                      { "initializers", initializers },
                                      { "structName", stripPrefix( structData.first, "Vk" ) } } );

  str += generateStructConstructorsEnhanced( structData );
  return str;
}

std::string VulkanHppGenerator::generateStructConstructorsEnhanced( std::pair<std::string, StructureData> const & structData ) const
{
  if ( std::find_if( structData.second.members.begin(),
                     structData.second.members.end(),
                     [this, &structData]( MemberData const & md ) { return hasLen( structData.second.members, md ); } ) != structData.second.members.end() )
  {
    // map from len-members to all the array members using that len
    std::map<std::vector<MemberData>::const_iterator, std::vector<std::vector<MemberData>::const_iterator>> lenIts;
    for ( auto mit = structData.second.members.begin(); mit != structData.second.members.end(); ++mit )
    {
      if ( hasLen( structData.second.members, *mit ) )
      {
        std::string lenName = ( mit->len.front() == "codeSize / 4" ) ? "codeSize" : mit->len.front();
        auto        lenIt   = findStructMemberIt( lenName, structData.second.members );
        assert( lenIt != mit );
        lenIts[lenIt].push_back( mit );
      }
    }

    std::string arguments, initializers;
    bool        listedArgument = false;
    bool        firstArgument  = true;
    bool        arrayListed    = false;
    std::string templateHeader, sizeChecks;
    for ( auto mit = structData.second.members.begin(); mit != structData.second.members.end(); ++mit )
    {
      // gather the initializers
      if ( mit->name == "pNext" )  // for pNext, we just get the initializer... the argument is added at the end
      {
        initializers += std::string( firstArgument ? ":" : "," ) + " pNext( pNext_ )";
        firstArgument = false;
      }
      else if ( mit->value.empty() )  // skip constant members
      {
        auto litit = lenIts.find( mit );
        if ( litit != lenIts.end() )
        {
          // len arguments just have an initalizer, from the ArrayProxyNoTemporaries size
          initializers +=
            ( firstArgument ? ": " : ", " ) + mit->name + "( " + generateLenInitializer( mit, litit, structData.second.mutualExclusiveLens ) + " )";
          sizeChecks += generateSizeCheck( litit->second, stripPrefix( structData.first, "Vk" ), structData.second.mutualExclusiveLens );
        }
        else if ( hasLen( structData.second.members, *mit ) )
        {
          assert( mit->name.starts_with( "p" ) );
          std::string argumentName = startLowerCase( stripPrefix( mit->name, "p" ) ) + "_";

          assert( mit->type.postfix.ends_with( "*" ) );
          std::string argumentType = trimEnd( stripPostfix( mit->type.compose( "VULKAN_HPP_NAMESPACE" ), "*" ) );
          if ( ( mit->type.type == "void" ) && ( argumentType.find( '*' ) == std::string::npos ) )
          {
            // the argument after stripping one pointer is just void
            assert( templateHeader.empty() );
            templateHeader = "    template <typename T>\n";

            size_t pos = argumentType.find( "void" );
            assert( pos != std::string::npos );
            argumentType.replace( pos, strlen( "void" ), "T" );
          }

          arguments += listedArgument ? ", " : "";
          arguments += "VULKAN_HPP_NAMESPACE::ArrayProxyNoTemporaries<" + argumentType + "> const & " + argumentName;
          if ( arrayListed )
          {
            arguments += " = {}";
          }
          listedArgument = true;
          arrayListed    = true;

          initializers += ( firstArgument ? ": " : ", " ) + mit->name + "( " + argumentName + ".data() )";
        }
        else
        {
          std::string argument = generateStructConstructorArgument( listedArgument, *mit, arrayListed );
          if ( !argument.empty() )
          {
            listedArgument = true;
            arguments += argument;
          }
          initializers += ( firstArgument ? ": " : ", " ) + mit->name + "( " + mit->name + "_ )";
        }
        firstArgument = false;
      }
    }

    auto pNextIt =
      std::find_if( structData.second.members.begin(), structData.second.members.end(), []( MemberData const & md ) { return md.name == "pNext"; } );
    if ( pNextIt != structData.second.members.end() )
    {
      // add pNext as a last optional argument to the constructor
      arguments += std::string( listedArgument ? ", " : "" ) + pNextIt->type.compose( "VULKAN_HPP_NAMESPACE" ) + " pNext_ = nullptr";
    }

    static const std::string constructorTemplate = R"(
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
${templateHeader}    ${structName}( ${arguments} )
    ${initializers}
    {${sizeChecks}}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

    return replaceWithMap( constructorTemplate,
                           { { "arguments", arguments },
                             { "initializers", initializers },
                             { "sizeChecks", sizeChecks },
                             { "structName", stripPrefix( structData.first, "Vk" ) },
                             { "templateHeader", templateHeader } } );
  }
  return "";
}

std::string VulkanHppGenerator::generateStructConstructorArgument( bool listedArgument, MemberData const & memberData, bool withDefault ) const
{
  // skip members 'pNext' and members with a specified value, as they are never explicitly set
  std::string str;
  if ( ( memberData.name != "pNext" ) && memberData.value.empty() )
  {
    str += ( listedArgument ? ( ", " ) : "" );
    if ( memberData.arraySizes.empty() )
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
      else
      {
        assert( memberData.value.empty() );
        // all the rest can be initialized with just {}
        str += "{}";
      }
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateStructHashStructure( std::pair<std::string, StructureData> const & structure,
                                                             std::set<std::string> &                       listedStructs ) const
{
  assert( listedStructs.find( structure.first ) == listedStructs.end() );

  std::string str;
  for ( auto const & member : structure.second.members )
  {
    auto structIt = m_structures.find( member.type.type );
    if ( ( structIt != m_structures.end() ) && ( structure.first != member.type.type ) && ( listedStructs.find( member.type.type ) == listedStructs.end() ) )
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

    auto [enter, leave] =
      generateProtection( m_structureAliasesInverse.find( structure.first ) == m_structureAliasesInverse.end() ? getProtectFromType( structure.first ) : "" );

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
  for ( auto const & structure : m_structures )
  {
    if ( listedStructs.find( structure.first ) == listedStructs.end() )
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
    else if ( member.type.type == "char" && !member.len.empty() )
    {
      assert( member.len.size() < 3 );
      if ( member.len.size() == 1 )
      {
        assert( member.len[0] == "null-terminated" );
        hashSum += "    for ( const char* p = " + structName + "." + member.name + "; *p != '\\0'; ++p )\n";
        hashSum += "    {\n";
        hashSum += "      VULKAN_HPP_HASH_COMBINE( seed, *p );\n";
        hashSum += "    }\n";
      }
      else
      {
        assert( member.len[1] == "null-terminated" );
        hashSum += "    for ( size_t i = 0; i < " + structName + "." + member.len[0] + "; ++i )\n";
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
  for ( auto const & structure : m_structures )
  {
    if ( listedStructs.find( structure.first ) == listedStructs.end() )
    {
      structs += generateStruct( structure, listedStructs );
    }
  }
  return replaceWithMap( structsTemplate, { { "structs", structs } } );
}

std::string VulkanHppGenerator::generateStructure( std::pair<std::string, StructureData> const & structure ) const
{
  auto [enter, leave] =
    generateProtection( m_structureAliasesInverse.find( structure.first ) == m_structureAliasesInverse.end() ? getProtectFromType( structure.first ) : "" );

  std::string str = "\n" + enter;

  static const std::string constructorsTemplate = R"(
#if !defined( VULKAN_HPP_NO_STRUCT_CONSTRUCTORS )
${constructors}
${subConstructors}
    ${structName} & operator=( ${structName} const & rhs ) VULKAN_HPP_NOEXCEPT = default;
#endif /*VULKAN_HPP_NO_STRUCT_CONSTRUCTORS*/

    ${structName} & operator=( Vk${structName} const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      *this = *reinterpret_cast<VULKAN_HPP_NAMESPACE::${structName} const *>( &rhs );
      return *this;
    }
)";

  std::string constructorsAndSetters = replaceWithMap( constructorsTemplate,
                                                       { { "constructors", generateStructConstructors( structure ) },
                                                         { "structName", stripPrefix( structure.first, "Vk" ) },
                                                         { "subConstructors", generateStructSubConstructor( structure ) } } );

  if ( !structure.second.returnedOnly )
  {
    // only structs that are not returnedOnly get setters!
    constructorsAndSetters += "\n#if !defined( VULKAN_HPP_NO_STRUCT_SETTERS )";
    for ( size_t i = 0; i < structure.second.members.size(); i++ )
    {
      constructorsAndSetters += generateStructSetter( stripPrefix( structure.first, "Vk" ), structure.second.members, i );
    }
    constructorsAndSetters += "#endif /*VULKAN_HPP_NO_STRUCT_SETTERS*/\n";
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

  auto aliasIt = m_structureAliasesInverse.find( structure.first );
  if ( aliasIt != m_structureAliasesInverse.end() )
  {
    for ( std::string const & alias : aliasIt->second )
    {
      str += "  using " + stripPrefix( alias, "Vk" ) + " = " + structureType + ";\n";
    }
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
    structExtends += generateStructExtendsStructs( feature.second.requireData, listedStructs, feature.first );
  }
  for ( auto const & extIt : m_extensionsByNumber )
  {
    structExtends += generateStructExtendsStructs( extIt.second->second.requireData, listedStructs, extIt.second->first );
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
      auto structIt = m_structures.find( type );
      if ( structIt != m_structures.end() )
      {
        assert( listedStructs.find( type ) == listedStructs.end() );
        listedStructs.insert( type );

        auto [enter, leave] = generateProtection( getProtectFromTitle( title ) );

        // append all allowed structure chains
        for ( auto extendName : structIt->second.structExtends )
        {
          std::map<std::string, StructureData>::const_iterator itExtend = m_structures.find( extendName );
          if ( itExtend == m_structures.end() )
          {
            // look if the extendName acutally is an alias of some other structure
            auto aliasIt = m_structureAliases.find( extendName );
            if ( aliasIt != m_structureAliases.end() )
            {
              itExtend = m_structures.find( aliasIt->second.alias );
              assert( itExtend != m_structures.end() );
            }
          }

          auto [subEnter, subLeave] = generateProtection(
            m_structureAliasesInverse.find( itExtend->first ) == m_structureAliasesInverse.end() ? getProtectFromType( itExtend->first ) : "" );

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

  std::string forwardDeclarations;
  for ( auto const & feature : m_features )
  {
    forwardDeclarations += generateStructForwardDeclarations( feature.second.requireData, feature.first );
  }
  for ( auto const & extIt : m_extensionsByNumber )
  {
    forwardDeclarations += generateStructForwardDeclarations( extIt.second->second.requireData, extIt.second->first );
  }

  return replaceWithMap( fowardDeclarationsTemplate, { { "forwardDeclarations", forwardDeclarations } } );
}

std::string VulkanHppGenerator::generateStructForwardDeclarations( std::vector<RequireData> const & requireData, std::string const & title ) const
{
  std::string str;
  for ( auto const & require : requireData )
  {
    for ( auto const & type : require.types )
    {
      auto structIt = m_structures.find( type );
      if ( structIt != m_structures.end() )
      {
        std::string structureType = stripPrefix( structIt->first, "Vk" );
        str += ( structIt->second.isUnion ? "  union " : "  struct " ) + structureType + ";\n";
        auto inverseIt = m_structureAliasesInverse.find( type );
        if ( inverseIt != m_structureAliasesInverse.end() )
        {
          for ( auto alias : inverseIt->second )
          {
            str += "  using " + stripPrefix( alias, "Vk" ) + " = " + structureType + ";\n";
          }
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
        std::string enumValue = member.value;
        auto        valueIt   = std::find_if(
          enumIt->second.values.begin(), enumIt->second.values.end(), [&enumValue]( EnumValueData const & evd ) { return enumValue == evd.name; } );
        assert( valueIt != enumIt->second.values.end() );
        std::string valueName = generateEnumValueName( enumIt->first, valueIt->name, enumIt->second.isBitmask, m_tags );
        members += stripPrefix( member.type.type, "Vk" ) + "::" + valueName;
        if ( member.name == "sType" )
        {
          sTypeValue = valueName;
        }
      }
    }
    else
    {
      // as we don't have any meaningful default initialization values, everything can be initialized by just '{}'
      // !
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
        else
        {
          members += "{}";
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

    std::string memberType         = member.arraySizes.empty() ? member.type.compose( "VULKAN_HPP_NAMESPACE" )
                                                               : generateStandardArray( member.type.compose( "VULKAN_HPP_NAMESPACE" ), member.arraySizes );
    bool        isReinterpretation = !member.bitCount.empty() && member.type.type.starts_with( "Vk" );
    std::string assignment;
    if ( isReinterpretation )
    {
      assignment = member.name + " = " + "*reinterpret_cast<" + member.type.type + "*>(&" + member.name + "_)";
    }
    else
    {
      assignment = member.name + " = " + member.name + "_";
    }

    str +=
      replaceWithMap( templateString,
                      { { "assignment", assignment },
                        { "constexpr", isReinterpretation ? "" : "VULKAN_HPP_CONSTEXPR_14 " },
                        { "memberName", member.name },
                        { "MemberName", startUpperCase( member.name ) },
                        { "memberType", memberType },
                        { "reference", ( member.type.postfix.empty() && ( m_structures.find( member.type.type ) != m_structures.end() ) ) ? "const & " : "" },
                        { "structureName", structureName } } );

    if ( hasLen( memberData, member ) )
    {
      assert( member.name.front() == 'p' );
      std::string arrayName = startLowerCase( stripPrefix( member.name, "p" ) );

      std::string lenName, lenValue;
      if ( member.len[0] == "codeSize / 4" )
      {
        lenName  = "codeSize";
        lenValue = arrayName + "_.size() * 4";
      }
      else
      {
        lenName  = member.len[0];
        lenValue = arrayName + "_.size()";
      }

      assert( memberType.back() == '*' );
      memberType = trimEnd( stripPostfix( memberType, "*" ) );

      std::string templateHeader;
      if ( ( member.type.type == "void" ) && ( memberType.find( '*' ) == std::string::npos ) )
      {
        assert( templateHeader.empty() );
        templateHeader = "template <typename T>\n    ";

        size_t pos = memberType.find( "void" );
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
  }
  return str;
}

std::string VulkanHppGenerator::generateStructSubConstructor( std::pair<std::string, StructureData> const & structData ) const
{
  if ( !structData.second.subStruct.empty() )
  {
    auto const & subStruct = m_structures.find( structData.second.subStruct );
    assert( subStruct != m_structures.end() );

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

    std::string subArguments;
    bool        listedArgument = true;
    for ( size_t i = subStruct->second.members.size(); i < structData.second.members.size(); i++ )
    {
      std::string argument = generateStructConstructorArgument( listedArgument, structData.second.members[i], true );
      if ( !argument.empty() )
      {
        listedArgument = true;
        subArguments += argument;
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
                             { "subArguments", subArguments },
                             { "subCopies", subCopies },
                             { "subStructArgumentName", subStructArgumentName },
                             { "subStructName", stripPrefix( subStruct->first, "Vk" ) } } );
  }
  return "";
}

std::string VulkanHppGenerator::generateSuccessCheck( std::vector<std::string> const & successCodes ) const
{
  assert( !successCodes.empty() );
  std::string successCheck = "result == " + generateSuccessCode( successCodes[0], m_tags );
  if ( 1 < successCodes.size() )
  {
    successCheck = "( " + successCheck + " )";
    for ( size_t i = 1; i < successCodes.size(); ++i )
    {
      successCheck += "|| ( result == " + generateSuccessCode( successCodes[i], m_tags ) + " )";
    }
  }
  return successCheck;
}

std::string VulkanHppGenerator::generateSuccessCodeList( std::vector<std::string> const & successCodes, bool enumerating ) const
{
  std::string successCodeList;
  if ( ( 1 < successCodes.size() ) && !enumerating )
  {
    successCodeList = ", { " + generateSuccessCode( successCodes[0], m_tags );
    for ( size_t i = 1; i < successCodes.size(); ++i )
    {
      successCodeList += ", " + generateSuccessCode( successCodes[i], m_tags );
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
    if ( value.name.starts_with( "VK_ERROR" ) )
    {
      auto [enter, leave]   = generateProtection( getProtect( value ) );
      std::string valueName = generateEnumValueName( enumIt->first, value.name, false, m_tags );
      cases += enter + "      case Result::" + valueName + ": throw " + stripPrefix( valueName, "eError" ) + "Error( message );\n" + leave;
    }
  }
  cases.pop_back();  // remove last newline

  const std::string throwTemplate = R"(
  namespace
  {
    [[noreturn]] void throwResultException( Result result, char const * message )
    {
      switch ( result )
      {
${cases}
        default: throw SystemError( make_error_code( result ) );
      }
    }
  })";
  return replaceWithMap( throwTemplate, { { "cases", cases } } );
}

std::string VulkanHppGenerator::generateTypenameCheck( std::vector<size_t> const &               returnParams,
                                                       std::map<size_t, VectorParamData> const & vectorParams,
                                                       bool                                      definition,
                                                       std::vector<std::string> const &          dataTypes,
                                                       CommandFlavourFlags                       flavourFlags ) const
{
  std::string typenameCheck, enableIf;
  if ( !( flavourFlags & CommandFlavourFlagBits::singular ) && ( flavourFlags & CommandFlavourFlagBits::withAllocator ) )
  {
    for ( size_t i = 0; i < returnParams.size(); ++i )
    {
      if ( vectorParams.find( returnParams[i] ) != vectorParams.end() )
      {
        std::string elementType = ( flavourFlags & CommandFlavourFlagBits::chained ) ? "StructureChain" : stripPrefix( dataTypes[i], "VULKAN_HPP_NAMESPACE::" );
        std::string extendedElementType = elementType;
        if ( flavourFlags & CommandFlavourFlagBits::unique )
        {
          extendedElementType = "UniqueHandle<" + elementType + ", Dispatch>";
        }
        std::string index = std::to_string( i );
        if ( definition )
        {
          typenameCheck += ", typename B" + index;
        }
        else
        {
          typenameCheck += ", typename B" + index + " = " + startUpperCase( elementType ) + "Allocator";
        }
        enableIf += enableIf.empty() ? ", typename std::enable_if<" : " && ";
        enableIf += "std::is_same<typename B" + index + "::value_type, " + extendedElementType + ">::value";
      }
    }
    assert( !typenameCheck.empty() );
    if ( !typenameCheck.empty() )
    {
      typenameCheck += enableIf + ", int>::type";
      if ( !definition )
      {
        typenameCheck += " = 0";
      }
    }
  }
  return typenameCheck;
}

std::string VulkanHppGenerator::generateUnion( std::pair<std::string, StructureData> const & structure ) const
{
  auto [enter, leave] =
    generateProtection( m_structureAliasesInverse.find( structure.first ) == m_structureAliasesInverse.end() ? getProtectFromType( structure.first ) : "" );
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

      bool multipleType =
        ( std::find_if( std::next( memberIt ),
                        structure.second.members.end(),
                        [memberIt]( MemberData const & member ) { return member.type == memberIt->type; } ) != structure.second.members.end() );
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
        int size = std::stoi( memberIt->arraySizes[0] );
        assert( std::to_string( size ) == memberIt->arraySizes[0] );
        std::string arguments, callArguments;
        bool        firstArgument = true;
        for ( int i = 0; i < size; i++ )
        {
          if ( !firstArgument )
          {
            arguments += ", ";
            callArguments += ", ";
          }
          std::string argumentIndex = std::to_string( i );
          arguments += memberIt->type.type + " " + memberIt->name + "_" + argumentIndex;
          callArguments += memberIt->name + "_" + argumentIndex;
          firstArgument = false;
        }

        static const std::string constructorBySequenceTemplate = R"(
    VULKAN_HPP_CONSTEXPR ${unionName}( ${arguments} )
      : ${memberName}( { ${callArguments} } )
    {})";

        constructors += "\n" + replaceWithMap( constructorBySequenceTemplate,
                                               { { "arguments", arguments },
                                                 { "callArguments", callArguments },
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
  bool needsUnrestrictedUnions =
    ( std::find_if( structure.second.members.begin(),
                    structure.second.members.end(),
                    []( MemberData const & member ) { return member.type.type.starts_with( "Vk" ); } ) != structure.second.members.end() );
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
#if !defined( VULKAN_HPP_NO_UNION_CONSTRUCTORS )
${constructors}
#endif /*VULKAN_HPP_NO_UNION_CONSTRUCTORS*/

#if !defined( VULKAN_HPP_NO_UNION_SETTERS )
${setters}
#endif /*VULKAN_HPP_NO_UNION_SETTERS*/

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

std::string VulkanHppGenerator::generateUniqueTypes( std::string const & parentType, std::set<std::string> const & childrenTypes ) const
{
  std::string childrenTraits;
  for ( auto const & childType : childrenTypes )
  {
    auto handleIt = m_handles.find( childType );
    assert( handleIt != m_handles.end() );

    std::string type = stripPrefix( childType, "Vk" );

    auto [enter, leave] = generateProtection( handleIt->second.alias.empty() ? getProtectFromType( handleIt->first ) : "" );

    std::string aliasHandle;
    if ( !handleIt->second.alias.empty() )
    {
      static const std::string aliasHandleTemplate =
        R"(  using Unique${aliasType} = UniqueHandle<${type}, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;
)";

      aliasHandle += replaceWithMap( aliasHandleTemplate, { { "aliasType", stripPrefix( handleIt->second.alias, "Vk" ) }, { "type", type } } );
    }

    static const std::string traitsTemplate = R"(${enter}  template <typename Dispatch>
  class UniqueHandleTraits<${type}, Dispatch>
  {
  public:
    using deleter = ${deleterType}${deleterAction}<${deleterParent}${deleterPool}, Dispatch>;
  };
  using Unique${type} = UniqueHandle<${type}, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;
${aliasHandle}${leave})";

    childrenTraits += replaceWithMap( traitsTemplate,
                                      { { "aliasHandle", aliasHandle },
                                        { "deleterAction", ( handleIt->second.deleteCommand.substr( 2, 4 ) == "Free" ) ? "Free" : "Destroy" },
                                        { "deleterParent", parentType.empty() ? "NoParent" : stripPrefix( parentType, "Vk" ) },
                                        { "deleterPool", handleIt->second.deletePool.empty() ? "" : ", " + stripPrefix( handleIt->second.deletePool, "Vk" ) },
                                        { "deleterType", handleIt->second.deletePool.empty() ? "Object" : "Pool" },
                                        { "enter", enter },
                                        { "leave", leave },
                                        { "type", type } } );
  }

  static const std::string uniqueTypeTemplate = R"(
#ifndef VULKAN_HPP_NO_SMART_HANDLE
${parentClass}${childrenTraits}#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
)";

  return replaceWithMap(
    uniqueTypeTemplate,
    { { "childrenTraits", childrenTraits }, { "parentClass", parentType.empty() ? "" : ( "  class " + stripPrefix( parentType, "Vk" ) + ";\n" ) } } );
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
  std::string const assertTemplateByLen = "    VULKAN_HPP_ASSERT( ${vectorName}.size() == ${sizeValue} );";
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
  std::string const throwTemplateByLen = R"#(    if ( ${vectorName}.size() != ${sizeValue} )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::${className}::${commandName}: ${vectorName}.size() != ${sizeValue}" );
    })#";

  std::string className   = stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" );
  std::string commandName = generateCommandName( name, commandData.params, initialSkipCount, m_tags );

  std::string assertions, throws;
  for ( auto const & cvm : countToVectorMap )
  {
    size_t      defaultStartIndex = determineDefaultStartIndex( commandData.params, skippedParams );
    std::string firstVectorName   = startLowerCase( stripPrefix( commandData.params[cvm.second[0]].name, "p" ) );

    if ( cvm.second.size() == 1 )
    {
      assert( isLenByStructMember( commandData.params[cvm.second[0]].len, commandData.params[cvm.first] ) );

      std::vector<std::string> lenParts = tokenize( commandData.params[cvm.second[0]].len, "->" );
      assert( lenParts.size() == 2 );
      std::string sizeValue = startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + lenParts[1];
      assertions += replaceWithMap( assertTemplateByLen, { { "sizeValue", sizeValue }, { "vectorName", firstVectorName } } );
      throws += replaceWithMap( throwTemplateByLen,
                                { { "className", className }, { "commandName", commandName }, { "sizeValue", sizeValue }, { "vectorName", firstVectorName } } );
    }
    else
    {
      for ( size_t i = 1; i < cvm.second.size(); i++ )
      {
        std::string secondVectorName  = startLowerCase( stripPrefix( commandData.params[cvm.second[i]].name, "p" ) );
        bool        withZeroSizeCheck = commandData.params[cvm.second[i]].optional && ( defaultStartIndex <= cvm.second[i] );
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
    bool skip = skipLeadingGrandParent( handle );
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
  if ( m_features.find( title ) == m_features.end() )
  {
    auto extensionIt = m_extensions.find( title );
    assert( extensionIt != m_extensions.end() );
    return extensionIt->second.platform;
  }
  return "";
}

std::pair<std::string, std::string> VulkanHppGenerator::getPoolTypeAndName( std::string const & type ) const
{
  auto structIt = m_structures.find( type );
  assert( structIt != m_structures.end() );
  auto memberIt = std::find_if(
    structIt->second.members.begin(), structIt->second.members.end(), []( MemberData const & md ) { return md.name.find( "Pool" ) != std::string::npos; } );
  assert( memberIt != structIt->second.members.end() );
  assert( std::find_if( std::next( memberIt ),
                        structIt->second.members.end(),
                        []( MemberData const & md ) { return md.name.find( "Pool" ) != std::string::npos; } ) == structIt->second.members.end() );
  return std::make_pair( memberIt->type.type, memberIt->name );
}

std::string VulkanHppGenerator::getProtect( EnumValueData const & evd ) const
{
  assert( evd.protect.empty() || ( evd.protect == getProtectFromTitle( evd.extension ) ) );
  return evd.protect.empty() ? getProtectFromTitle( evd.extension ) : evd.protect;
}

std::string VulkanHppGenerator::getProtectFromPlatform( std::string const & platform ) const
{
  auto platformIt = m_platforms.find( platform );
  return ( platformIt != m_platforms.end() ) ? platformIt->second.protect : "";
}

std::string VulkanHppGenerator::getProtectFromTitle( std::string const & title ) const
{
  if ( m_features.find( title ) == m_features.end() )
  {
    auto extensionIt = m_extensions.find( title );
    return ( extensionIt != m_extensions.end() ) ? getProtectFromPlatform( extensionIt->second.platform ) : "";
  }
  return "";
}

std::string VulkanHppGenerator::getProtectFromType( std::string const & type ) const
{
  auto typeIt = m_types.find( type );
  assert( typeIt != m_types.end() );
  return getProtectFromTitle( typeIt->second.referencedIn );
}

std::string VulkanHppGenerator::getVectorSize( std::vector<ParamData> const &            params,
                                               std::map<size_t, VectorParamData> const & vectorParams,
                                               size_t                                    returnParam,
                                               std::string const &                       returnParamType,
                                               std::set<size_t> const &                  templatedParams ) const
{
  std::string              vectorSize;
  std::vector<std::string> lenParts = tokenize( params[returnParam].len, "->" );
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
          if ( templatedParams.find( returnParam ) != templatedParams.end() )
          {
            vectorSize += " / sizeof( " + returnParamType + " )";
          }
        }
        else
        {
          assert( templatedParams.find( returnParam ) == templatedParams.end() );
          vectorSize = startLowerCase( stripPrefix( params[lenVectorParamIt->first].name, "p" ) ) + ".size()";
        }
      }
      break;
    case 2:
      assert( vectorParams.find( returnParam ) != vectorParams.end() );
      vectorSize = startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + lenParts[1];
      break;
    default: assert( false ); break;
  }
  assert( !vectorSize.empty() );
  return vectorSize;
}

bool VulkanHppGenerator::hasLen( std::vector<MemberData> const & members, MemberData const & md ) const
{
  if ( !md.len.empty() && !( md.len[0] == "null-terminated" ) && ( ( altLens.find( md.len[0] ) == altLens.end() ) || ( md.len[0] == "codeSize / 4" ) ) )
  {
    auto lenIt = findStructMemberIt( md.len.front(), members );
    return ( lenIt == members.end() ) || lenIt->type.isValue();
  }
  return false;
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
  return !commandData.handle.empty() && !commandData.params.empty() && ( m_handles.find( commandData.params[0].type.type ) != m_handles.end() ) &&
         ( commandData.params[0].type.type != "VkInstance" ) && ( commandData.params[0].type.type != "VkPhysicalDevice" );
}

bool VulkanHppGenerator::isHandleType( std::string const & type ) const
{
  if ( type.starts_with( "Vk" ) )
  {
    auto it = m_handles.find( type );
    if ( it == m_handles.end() )
    {
      it = std::find_if( m_handles.begin(), m_handles.end(), [&type]( std::pair<std::string, HandleData> const & hd ) { return hd.second.alias == type; } );
    }
    return ( it != m_handles.end() );
  }
  return false;
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
      auto structureIt = m_structures.find( paramIt->type.type );
      assert( structureIt != m_structures.end() );
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
    auto structureIt = m_structures.find( param.type.type );
    assert( structureIt != m_structures.end() );
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
  return std::find_if( params.begin(), params.end(), [&name]( ParamData const & pd ) { return pd.name == name; } ) != params.end();
}

bool VulkanHppGenerator::isStructMember( std::string const & name, std::vector<MemberData> const & memberData ) const
{
  return findStructMemberIt( name, memberData ) != memberData.end();
}

bool VulkanHppGenerator::isStructureChainAnchor( std::string const & type ) const
{
  if ( type.starts_with( "Vk" ) )
  {
    auto it = m_structures.find( type );
    if ( it == m_structures.end() )
    {
      auto aliasIt = m_structureAliases.find( type );
      if ( aliasIt != m_structureAliases.end() )
      {
        it = m_structures.find( aliasIt->second.alias );
      }
    }
    if ( it != m_structures.end() )
    {
      return m_extendedStructs.find( it->first ) != m_extendedStructs.end();
    }
  }
  return false;
}

std::pair<bool, std::map<size_t, std::vector<size_t>>> VulkanHppGenerator::needsVectorSizeCheck( std::vector<ParamData> const &            params,
                                                                                                 std::map<size_t, VectorParamData> const & vectorParams,
                                                                                                 std::vector<size_t> const &               returnParams,
                                                                                                 std::set<size_t> const & singularParams ) const
{
  std::map<size_t, std::vector<size_t>> countToVectorMap;
  for ( auto const & vpi : vectorParams )
  {
    if ( ( vpi.second.lenParam != INVALID_INDEX ) && ( std::find( returnParams.begin(), returnParams.end(), vpi.first ) == returnParams.end() ) &&
         ( ( singularParams.find( vpi.second.lenParam ) == singularParams.end() ) ||
           isLenByStructMember( params[vpi.first].len, params[vpi.second.lenParam] ) ) )
    {
      countToVectorMap[vpi.second.lenParam].push_back( vpi.first );
    }
  }
  return std::make_pair( std::find_if( countToVectorMap.begin(),
                                       countToVectorMap.end(),
                                       [this, &params]( auto const & cvm ) {
                                         return ( 1 < cvm.second.size() ) || isLenByStructMember( params[cvm.second[0]].len, params[cvm.first] );
                                       } ) != countToVectorMap.end(),
                         countToVectorMap );
}

void VulkanHppGenerator::readCommands( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, { { "comment", {} } } );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "command", false } } );
  for ( auto child : children )
  {
    assert( child->Value() == std::string( "command" ) );
    readCommandsCommand( child );
  }
}

void VulkanHppGenerator::readCommandsCommand( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  if ( attributes.find( "alias" ) != attributes.end() )
  {
    // for command aliases, create a copy of the aliased command
    checkAttributes( line,
                     attributes,
                     {},
                     {
                       { "alias", {} },
                       { "name", {} },
                     } );
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
        checkForError( name.starts_with( "vk" ), line, "name <" + name + "> should begin with <vk>" );
      }
    }

    auto commandIt = m_commands.find( alias );
    checkForError( commandIt != m_commands.end(), line, "missing command <" + alias + ">" );
    CommandData commandData = commandIt->second;
    commandData.alias       = alias;
    commandData.xmlLine     = line;
    addCommand( name, commandData );
  }
  else
  {
    checkAttributes( line,
                     attributes,
                     {},
                     { { "cmdbufferlevel", { "primary", "secondary" } },
                       { "comment", {} },
                       { "errorcodes", {} },
                       { "queues", { "compute", "decode", "encode", "graphics", "opticalflow", "sparse_binding", "transfer" } },
                       { "renderpass", { "both", "inside", "outside" } },
                       { "successcodes", {} },
                       { "tasks", { "action", "indirection", "state", "synchronization" } },
                       { "videocoding", { "both", "inside", "outside" } } } );

    std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
    checkElements( line, children, { { "param", false }, { "proto", true } }, { "implicitexternsyncparams" } );

    CommandData commandData( line );
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "errorcodes" )
      {
        commandData.errorCodes = tokenize( attribute.second, "," );
        // errorCodes are checked in checkCorrectness after complete reading
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
        commandData.params.push_back( readCommandsCommandParam( child, commandData.params ) );
      }
      else if ( value == "proto" )
      {
        std::tie( name, commandData.returnType ) = readCommandsCommandProto( child );
      }
    }
    assert( !name.empty() );
    checkForError( ( commandData.returnType == "VkResult" ) || commandData.errorCodes.empty(),
                   line,
                   "command <" + name + "> does not return a VkResult but specifies errorcodes" );
    checkForError( ( commandData.returnType == "VkResult" ) || commandData.successCodes.empty(),
                   line,
                   "command <" + name + "> does not return a VkResult but specifies successcodes" );
    for ( auto const & param : commandData.params )
    {
      checkForError( param.stride.empty() || isParam( param.stride, commandData.params ),
                     param.xmlLine,
                     "attribute <stride> holds an unknown value <" + param.stride + ">" );
    }

    registerDeleter( name, std::make_pair( name, commandData ) );
    addCommand( name, commandData );
  }
}

VulkanHppGenerator::ParamData VulkanHppGenerator::readCommandsCommandParam( tinyxml2::XMLElement const * element, std::vector<ParamData> const & params )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line,
                   attributes,
                   {},
                   { { "altlen", {} },
                     { "externsync", {} },
                     { "len", {} },
                     { "noautovalidity", { "true" } },
                     { "objecttype", { "objectType" } },
                     { "optional", { "false", "true" } },
                     { "stride", {} },
                     { "validstructs", {} } } );
  checkValidStructs( line, attributes );

  ParamData paramData( line );
  for ( auto attribute : attributes )
  {
    if ( attribute.first == "altlen" )
    {
      assert( paramData.len.empty() );
      paramData.len = attribute.second;
      checkForError( altLens.find( paramData.len ) != altLens.end(), line, "attribute <altlen> holds unknown value <" + paramData.len + ">" );
    }
    else if ( attribute.first == "len" )
    {
      if ( paramData.len.empty() )
      {
        checkForError( ( attribute.second == "null-terminated" ) || isParam( attribute.second, params ) || isLenByStructMember( attribute.second, params ),
                       line,
                       "attribute <len> holds an unknown value <" + attribute.second + ">" );
        paramData.len = attribute.second;
      }
    }
    else if ( attribute.first == "stride" )
    {
      paramData.stride = attribute.second;
    }
    else if ( attribute.first == "optional" )
    {
      paramData.optional = ( attribute.second == "true" );
    }
  }

  NameData nameData;
  std::tie( nameData, paramData.type ) = readNameAndType( element );

  checkForError( m_types.find( paramData.type.type ) != m_types.end(), line, "unknown type <" + paramData.type.type + ">" );
  checkForError( paramData.type.prefix.empty() || ( paramData.type.prefix == "const" ) || ( paramData.type.prefix == "const struct" ) ||
                   ( paramData.type.prefix == "struct" ),
                 line,
                 "unexpected type prefix <" + paramData.type.prefix + ">" );
  checkForError( paramData.type.postfix.empty() || ( paramData.type.postfix == "*" ) || ( paramData.type.postfix == "**" ) ||
                   ( paramData.type.postfix == "* const *" ),
                 line,
                 "unexpected type postfix <" + paramData.type.postfix + ">" );
  checkForError( std::find_if( params.begin(), params.end(), [&name = nameData.name]( ParamData const & pd ) { return pd.name == name; } ) == params.end(),
                 line,
                 "command param <" + nameData.name + "> already used" );
  paramData.name       = nameData.name;
  paramData.arraySizes = nameData.arraySizes;

  return paramData;
}

std::pair<std::string, std::string> VulkanHppGenerator::readCommandsCommandProto( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );

  auto [nameData, typeInfo] = readNameAndType( element );

  checkForError( nameData.name.starts_with( "vk" ), line, "name <" + nameData.name + "> does not begin with <vk>" );
  checkForError( nameData.arraySizes.empty(), line, "name <" + nameData.name + "> with unsupported arraySizes" );
  checkForError( m_types.find( typeInfo.type ) != m_types.end(), line, "unknown type <" + typeInfo.type + ">" );
  checkForError( typeInfo.prefix.empty(), line, "unexpected type prefix <" + typeInfo.prefix + ">" );
  checkForError( typeInfo.postfix.empty(), line, "unexpected type postfix <" + typeInfo.postfix + ">" );
  checkForError( m_commands.find( nameData.name ) == m_commands.end(), line, "command <" + nameData.name + "> already specified" );

  return std::make_pair( nameData.name, typeInfo.type );
}

void VulkanHppGenerator::readEnums( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, { { "bitwidth", { "64" } }, { "comment", {} }, { "type", { "bitmask", "enum" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );

  std::string bitwidth, name, type;
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

  if ( name == "API Constants" )
  {
    checkElements( line, children, { { "enum", false } }, {} );
    for ( auto const & child : children )
    {
      readEnumsConstant( child );
    }
  }
  else
  {
    checkElements( line, children, {}, { "comment", "enum", "unused" } );
    checkForError( !type.empty(), line, "enum without type" );

    // get the EnumData entry in enum map
    std::map<std::string, EnumData>::iterator enumIt = m_enums.find( name );
    if ( enumIt == m_enums.end() )
    {
      enumIt =
        std::find_if( m_enums.begin(), m_enums.end(), [&name]( std::pair<std::string, EnumData> const & enumData ) { return enumData.second.alias == name; } );
    }
    checkForError( enumIt != m_enums.end(), line, "enum <" + name + "> is not listed as enum in the types section" );
    checkForError( enumIt->second.values.empty(), line, "enum <" + name + "> already holds values" );

    // mark it as a bitmask, if it is one
    bool bitmask = ( type == "bitmask" );
    if ( bitmask )
    {
      checkForError( name.find( "FlagBits" ) != std::string::npos, line, "bitmask <" + name + "> does not contain <FlagBits>" );
    }
    enumIt->second.isBitmask = bitmask;
    enumIt->second.bitwidth  = bitwidth;

    // read the names of the enum values
    for ( auto child : children )
    {
      std::string value = child->Value();
      if ( value == "comment" )
      {
        readComment( child );
      }
      else if ( value == "enum" )
      {
        readEnumsEnum( child, enumIt );
      }
    }
  }
}

void VulkanHppGenerator::readEnumsConstant( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, { { "alias", {} }, { "comment", {} }, { "type", {} }, { "value", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string alias, name, value;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      checkForError( m_constants.find( attribute.second ) != m_constants.end(), line, "unknown enum constant alias <" + attribute.second + ">" );
      alias = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkForError( m_constants.find( attribute.second ) == m_constants.end(), line, "already specified enum constant <" + attribute.second + ">" );
      name = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      checkForError( !attribute.second.empty(), line, "value of enum constant is empty" );
      value = attribute.second;
    }
  }
  checkForError( alias.empty() != value.empty(), line, "for enum <" + name + "> either alias or value need to be specified" );
  m_constants[name] = alias.empty() ? value : m_constants[alias];
}

void VulkanHppGenerator::readEnumsEnum( tinyxml2::XMLElement const * element, std::map<std::string, EnumData>::iterator enumIt )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  if ( attributes.find( "alias" ) != attributes.end() )
  {
    checkAttributes( line, attributes, { { "alias", {} }, { "name", {} } }, { { "comment", {} } } );
    checkElements( line, getChildElements( element ), {} );

    std::string alias, bitpos, name, value;
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
    assert( !name.empty() );

    enumIt->second.addEnumAlias( line, name, alias );
  }
  else
  {
    checkAttributes( line, attributes, { { "name", {} } }, { { "bitpos", {} }, { "comment", {} }, { "value", {} } } );
    checkElements( line, getChildElements( element ), {} );

    std::string alias, bitpos, name, protect, value;
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

    std::string prefix = generateEnumSuffixes( enumIt->first, enumIt->second.isBitmask, m_tags ).first;
    checkForError( name.starts_with( prefix ), line, "encountered enum value <" + name + "> that does not begin with expected prefix <" + prefix + ">" );

    checkForError( bitpos.empty() ^ value.empty(), line, "invalid set of attributes for enum <" + name + ">" );
    enumIt->second.addEnumValue( line, name, protect, !bitpos.empty(), "" );
  }
}

std::string VulkanHppGenerator::readComment( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  checkElements( line, getChildElements( element ), {} );

  return element->GetText();
}

void VulkanHppGenerator::readExtensions( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "extension", false } } );

  for ( auto child : children )
  {
    assert( child->Value() == std::string( "extension" ) );
    readExtensionsExtension( child );
  }
}

void VulkanHppGenerator::readExtensionsExtension( tinyxml2::XMLElement const * element )
{
  int                                       line       = element->GetLineNum();
  std::map<std::string, std::string>        attributes = getAttributes( element );
  std::vector<tinyxml2::XMLElement const *> children   = getChildElements( element );

  auto it = attributes.find( "supported" );
  checkForError( it != attributes.end(), line, "Missing attribute <supported> for extension!" );

  if ( it->second == "disabled" )
  {
    checkElements( line, children, {}, { "require" } );
    // kick out all the disabled stuff we've read before !!
    for ( auto const & child : children )
    {
      assert( child->Value() == std::string( "require" ) );
      readExtensionsExtensionDisabledRequire( child );
    }
  }
  else
  {
    checkAttributes( line,
                     attributes,
                     { { "name", {} }, { "number", {} }, { "supported", { "disabled", "enabled", "vulkan" } } },
                     { { "author", {} },
                       { "comment", {} },
                       { "contact", {} },
                       { "depends", {} },
                       { "deprecatedby", {} },
                       { "obsoletedby", {} },
                       { "platform", {} },
                       { "promotedto", {} },
                       { "provisional", { "true" } },
                       { "requires", {} },
                       { "requiresCore", {} },
                       { "sortorder", { "1" } },
                       { "specialuse", { "cadsupport", "d3demulation", "debugging", "devtools", "glemulation" } },
                       { "type", { "device", "instance" } } } );
    checkElements( line, children, { { "require", false } } );

    std::string              deprecatedBy, name, number, obsoletedBy, platform, promotedTo, supported;
    std::vector<std::string> depends;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "deprecatedby" )
      {
        deprecatedBy = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        name = attribute.second;
      }
      else if ( attribute.first == "number" )
      {
        number = attribute.second;
      }
      else if ( attribute.first == "obsoletedby" )
      {
        obsoletedBy = attribute.second;
      }
      else if ( attribute.first == "platform" )
      {
        platform = attribute.second;
        checkForError( m_platforms.find( platform ) != m_platforms.end(), line, "unknown platform <" + platform + ">" );
      }
      else if ( attribute.first == "promotedto" )
      {
        promotedTo = attribute.second;
      }
      else if ( attribute.first == "provisional" )
      {
        if ( platform.empty() )
        {
          // for now, having the attribute provisional="true" implies attribute platform="provisional" to get
          // stuff protected by VK_ENABLE_BETA_EXTENSIONS
          platform = "provisional";
        }
        checkForError( platform == "provisional",
                       line,
                       "while attribute <provisional> is set to \"true\", attribute <platform> is not set to \"provisional\" but to \"" + platform + "\"" );
      }
      else if ( ( attribute.first == "depends" ) || ( attribute.first == "requires" ) )
      {
        // we don't care about the logical implications of ',' and '+' here, we're just interested to get the depends strings
        depends = tokenizeAny( attribute.second, ",+" );
      }
      else if ( attribute.first == "requiresCore" )
      {
        std::string const & requiresCore = attribute.second;
        checkForError( std::find_if( m_features.begin(),
                                     m_features.end(),
                                     [&requiresCore]( std::pair<std::string, FeatureData> const & feature )
                                     { return feature.second.number == requiresCore; } ) != m_features.end(),
                       line,
                       "unknown feature number <" + attribute.second + ">" );
      }
      else if ( attribute.first == "supported" )
      {
        supported = attribute.second;
        assert( supported != "disabled" );
      }
    }

    auto pitb = m_extensions.insert( std::make_pair( name, ExtensionData( line, deprecatedBy, number, obsoletedBy, platform, promotedTo ) ) );
    checkForError( pitb.second, line, "already encountered extension <" + name + ">" );
    for ( auto const & d : depends )
    {
      checkForError( pitb.first->second.depends.insert( d ).second, line, "required depends <" + d + "> already listed" );
    }

    // extract the tag from the name, which is supposed to look like VK_<tag>_<other>
    size_t tagStart = name.find( '_' );
    checkForError( tagStart != std::string::npos, line, "name <" + name + "> is missing an underscore '_'" );
    size_t tagEnd = name.find( '_', tagStart + 1 );
    checkForError( tagEnd != std::string::npos, line, "name <" + name + "> is missing an underscore '_'" );
    std::string tag = name.substr( tagStart + 1, tagEnd - tagStart - 1 );
    checkForError( m_tags.find( tag ) != m_tags.end(), line, "name <" + name + "> is using an unknown tag <" + tag + ">" );

    for ( auto child : children )
    {
      assert( child->Value() == std::string( "require" ) );
      readExtensionsExtensionRequire( child, pitb.first );
    }
  }
}

void VulkanHppGenerator::readExtensionsExtensionDisabledRequire( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "enum", false } }, { "command", "comment", "type" } );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "command" )
    {
      readExtensionsExtensionDisabledRequireCommand( child );
    }
    else if ( value == "comment" )
    {
      readComment( child );
    }
    else if ( value == "enum" )
    {
      // just skip enums of disabled extensions
    }
    else
    {
      assert( value == "type" );
      readExtensionsExtensionDisabledRequireType( child );
    }
  }
}

void VulkanHppGenerator::readExtensionsExtensionDisabledRequireCommand( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name = attributes.find( "name" )->second;

  // first unlink the command from its class
  auto commandIt = m_commands.find( name );
  if ( commandIt != m_commands.end() )
  {
    auto handleIt = m_handles.find( commandIt->second.handle );
    checkForError( handleIt != m_handles.end(), line, "cannot find handle corresponding to command <" + name + ">" );
    handleIt->second.commands.erase( commandIt->first );

    // then erase the command from the command list
    m_commands.erase( commandIt );
  }
}

void VulkanHppGenerator::readExtensionsExtensionDisabledRequireType( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name = attributes.find( "name" )->second;

  auto typeIt = m_types.find( name );
  if ( typeIt != m_types.end() )
  {
    switch ( typeIt->second.category )
    {
      case TypeCategory::Bitmask:
        {
          auto bitmasksIt = m_bitmasks.find( name );
          checkForError( bitmasksIt != m_bitmasks.end(), line, "trying to remove unknown bitmask <" + name + ">" );
          checkForError(
            bitmasksIt->second.alias.empty(), line, "trying to remove disabled bitmask <" + name + "> which has alias <" + bitmasksIt->second.alias + ">" );
          m_bitmasks.erase( bitmasksIt );
        }
        break;
      case TypeCategory::Enum:
        {
          auto enumIt = m_enums.find( name );
          checkForError( enumIt != m_enums.end(), line, "trying to remove unknown enum <" + name + ">" );
          checkForError( enumIt->second.alias.empty(), line, "trying to remove disabled enum <" + name + "> which has alias <" + enumIt->second.alias + ">" );
          m_enums.erase( enumIt );
        }
        break;
      case TypeCategory::Struct:
        {
          auto structIt = m_structures.find( name );
          checkForError( structIt != m_structures.end(), line, "trying to remove unknown struct <" + name + ">" );
          auto inverseAliasIt = m_structureAliasesInverse.find( name );
          if ( inverseAliasIt != m_structureAliasesInverse.end() )
          {
            checkForError(
              false, line, "trying to remove disabled structure <" + name + "> which has " + std::to_string( inverseAliasIt->second.size() ) + "aliases" );
          }
          m_structures.erase( structIt );
        }
        break;
      default: checkForError( false, line, "trying to remove <" + name + "> of unhandled type <" + toString( typeIt->second.category ) + ">" ); break;
    }
  }
}

void VulkanHppGenerator::readExtensionsExtensionRequire( tinyxml2::XMLElement const * element, std::map<std::string, ExtensionData>::iterator extensionIt )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, { { "depends", {} }, { "extension", {} }, { "feature", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "command", "comment", "enum", "type" } );

  std::vector<std::string> depends;
  for ( auto const & attribute : attributes )
  {
    if ( ( attribute.first == "depends" ) || ( attribute.first == "extension" ) )
    {
      assert( depends.empty() );
      depends = tokenizeAny( attribute.second, ",+" );
      for ( auto const & d : depends )
      {
        checkForError( std::find_if( extensionIt->second.requireData.begin(),
                                     extensionIt->second.requireData.end(),
                                     [&d]( RequireData const & rd ) { return std::find( rd.depends.begin(), rd.depends.end(), d ) != rd.depends.end(); } ) ==
                         extensionIt->second.requireData.end(),
                       line,
                       "required extension <" + d + "> already listed" );
      }
    }
    else
    {
      assert( attribute.first == "feature" );
      checkForError( m_features.find( attribute.second ) != m_features.end(), line, "unknown feature <" + attribute.second + ">" );
      assert( depends.empty() );
      depends.push_back( attribute.second );
    }
  }

  RequireData requireData( line, depends );
  bool        requireDataEmpty = true;
  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "command" )
    {
      readExtensionsExtensionRequireCommand( child, extensionIt->first, requireData );
      requireDataEmpty = false;
    }
    else if ( value == "comment" )
    {
      readComment( child );
    }
    else if ( value == "enum" )
    {
      readRequireEnum( child, extensionIt->first );
    }
    else if ( value == "type" )
    {
      readExtensionsExtensionRequireType( child, extensionIt->first, requireData );
      requireDataEmpty = false;
    }
  }
  if ( !requireDataEmpty )
  {
    extensionIt->second.requireData.push_back( requireData );
  }
}

void VulkanHppGenerator::readExtensionsExtensionRequireCommand( tinyxml2::XMLElement const * element,
                                                                std::string const &          extensionName,
                                                                RequireData &                requireData )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, { { "comment", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
  }
  assert( !name.empty() );

  // mark this command be part of this extension
  auto commandIt = m_commands.find( name );
  checkForError(
    commandIt != m_commands.end(), line, "command <" + name + "> marked as required in extension <" + extensionName + "> was not listed before as a command!" );
  if ( commandIt->second.referencedIn.empty() )
  {
    commandIt->second.referencedIn = extensionName;
  }
  else
  {
    checkForError( getPlatform( commandIt->second.referencedIn ) == getPlatform( extensionName ),
                   line,
                   "command <" + name + "> is referenced in extensions <" + commandIt->second.referencedIn + "> and <" + extensionName +
                     "> and thus protected by different platforms <" + getPlatform( commandIt->second.referencedIn ) + "> and <" +
                     getPlatform( extensionName ) + ">!" );
  }
  assert( std::find( requireData.commands.begin(), requireData.commands.end(), name ) == requireData.commands.end() );
  requireData.commands.push_back( name );
}

void VulkanHppGenerator::readExtensionsExtensionRequireType( tinyxml2::XMLElement const * element,
                                                             std::string const &          extensionName,
                                                             RequireData &                requireData )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, { { "comment", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
  }
  assert( !name.empty() );

  auto typeIt = m_types.find( name );
  checkForError( typeIt != m_types.end(), line, "failed to find required type <" + name + ">" );
  if ( typeIt->second.referencedIn.empty() )
  {
    typeIt->second.referencedIn = extensionName;
    assert( std::find( requireData.types.begin(), requireData.types.end(), name ) == requireData.types.end() );
    requireData.types.push_back( name );
  }
  else
  {
    checkForError( getPlatform( typeIt->second.referencedIn ) == getPlatform( extensionName ),
                   line,
                   "type <" + name + "> is referenced in extensions <" + typeIt->second.referencedIn + "> and <" + extensionName +
                     "> and thus protected by different platforms <" + getPlatform( typeIt->second.referencedIn ) + "> and <" + getPlatform( extensionName ) +
                     ">!" );
  }
}

void VulkanHppGenerator::readFeature( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "api", { "vulkan" } }, { "comment", {} }, { "name", {} }, { "number", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "require", false } } );

  std::string name, number, modifiedNumber;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "number" )
    {
      number         = attribute.second;
      modifiedNumber = number;
      std::replace( modifiedNumber.begin(), modifiedNumber.end(), '.', '_' );
    }
  }
  assert( !name.empty() && !number.empty() );
  checkForError( name == "VK_VERSION_" + modifiedNumber, line, "unexpected formatting of name <" + name + ">" );
  checkForError( m_features.find( name ) == m_features.end(), line, "already specified feature <" + name + ">" );

  auto featureIt = m_features.insert( std::make_pair( name, number ) ).first;
  for ( auto child : children )
  {
    assert( child->Value() == std::string( "require" ) );
    readFeatureRequire( child, featureIt );
  }
}

void VulkanHppGenerator::readFeatureRequire( tinyxml2::XMLElement const * element, std::map<std::string, FeatureData>::iterator featureIt )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, { { "comment", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "command", "comment", "enum", "type" } );

  RequireData requireData( line, { "" } );
  bool        requireDataEmpty = true;
  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "command" )
    {
      readFeatureRequireCommand( child, featureIt, requireData );
      requireDataEmpty = false;
    }
    else if ( value == "comment" )
    {
      readComment( child );
    }
    else if ( value == "enum" )
    {
      readRequireEnum( child, "" );
    }
    else if ( value == "type" )
    {
      readFeatureRequireType( child, featureIt, requireData );
      requireDataEmpty = false;
    }
  }
  if ( !requireDataEmpty )
  {
    featureIt->second.requireData.push_back( requireData );
  }
}

void VulkanHppGenerator::readFeatureRequireCommand( tinyxml2::XMLElement const *                 element,
                                                    std::map<std::string, FeatureData>::iterator featureIt,
                                                    RequireData &                                requireData )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, { { "name", {} } } );
  std::string name      = attributes.find( "name" )->second;
  auto        commandIt = m_commands.find( name );
  checkForError( commandIt != m_commands.end(), line, "feature requires unknown command <" + name + ">" );
  checkForError( commandIt->second.referencedIn.empty(), line, "command <" + name + "> already listed with feature <" + commandIt->second.referencedIn + ">" );
  commandIt->second.referencedIn = featureIt->first;
  assert( std::find( requireData.commands.begin(), requireData.commands.end(), name ) == requireData.commands.end() );
  requireData.commands.push_back( name );
}

void VulkanHppGenerator::readFeatureRequireType( tinyxml2::XMLElement const *                 element,
                                                 std::map<std::string, FeatureData>::iterator featureIt,
                                                 RequireData &                                requireData )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, { { "comment", {} }, { "name", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string name          = attributes.find( "name" )->second;
  auto        requireTypeIt = std::find_if( requireData.types.begin(), requireData.types.end(), [&name]( std::string const & type ) { return type == name; } );
  checkForError( requireTypeIt == requireData.types.end(), line, "type <" + name + "> already listed for this feature!" );

  // some types are in fact includes (like vk_platform) or defines (like VK_API_VERSION)
  if ( ( m_defines.find( name ) == m_defines.end() ) && ( m_includes.find( name ) == m_includes.end() ) )
  {
    auto typeIt = m_types.find( name );
    checkForError( typeIt != m_types.end(), line, "feature requires unknown type <" + name + ">" );
    checkForError( typeIt->second.referencedIn.empty() || ( typeIt->second.referencedIn == featureIt->first ),
                   line,
                   "type <" + name + "> already listed on feature <" + typeIt->second.referencedIn + ">" );
    typeIt->second.referencedIn = featureIt->first;

    requireData.types.push_back( name );
  }
}

void VulkanHppGenerator::readFormats( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "format", false } } );

  for ( auto child : children )
  {
    readFormatsFormat( child );
  }
}

void VulkanHppGenerator::readFormatsFormat( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line,
                   attributes,
                   { { "blockSize", { "1", "2", "3", "4", "5", "6", "8", "12", "16", "24", "32" } },
                     { "class", {} },
                     { "name", {} },
                     { "texelsPerBlock", { "1", "16", "20", "25", "30", "36", "40", "48", "50", "60", "64", "80", "100", "120", "144" } } },
                   { { "blockExtent", { "1", "2", "4", "5", "6", "8", "10", "12" } },
                     { "chroma", { "420", "422", "444" } },
                     { "compressed", { "ASTC HDR", "ASTC LDR", "BC", "EAC", "ETC", "ETC2", "PVRTC" } },
                     { "packed", { "8", "16", "32" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "component", false } }, { "plane", "spirvimageformat" } );

  FormatData  format( line );
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

  auto formatIt = m_enums.find( "VkFormat" );
  assert( formatIt != m_enums.end() );

  checkForError( std::find_if( formatIt->second.values.begin(),
                               formatIt->second.values.end(),
                               [&name]( EnumValueData const & evd ) { return evd.name == name; } ) != formatIt->second.values.end() ||
                   ( formatIt->second.aliases.find( name ) != formatIt->second.aliases.end() ),
                 line,
                 "encountered unknown format <" + name + ">" );
  auto [it, inserted] = m_formats.insert( std::make_pair( name, format ) );
  checkForError( inserted, line, "format <" + name + "> already specified on line " + std::to_string( it->second.xmlLine ) );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "component" )
    {
      readFormatsFormatComponent( child, it->second );
    }
    else if ( value == "plane" )
    {
      readFormatsFormatPlane( child, it->second );
    }
    else if ( value == "spirvimageformat" )
    {
      readFormatsFormatSPIRVImageFormat( child, it->second );
    }
  }

  if ( it->second.components.front().bits == "compressed" )
  {
    for ( auto componentIt = std::next( it->second.components.begin() ); componentIt != it->second.components.end(); ++componentIt )
    {
      checkForError( componentIt->bits == "compressed", line, "component is expected to be marked as compressed in attribute <bits>" );
    }
  }
  if ( !it->second.components.front().planeIndex.empty() )
  {
    for ( auto componentIt = std::next( it->second.components.begin() ); componentIt != it->second.components.end(); ++componentIt )
    {
      checkForError( !componentIt->planeIndex.empty(), line, "component is expected to have a planeIndex" );
    }
    size_t planeCount = 1 + std::stoi( it->second.components.back().planeIndex );
    checkForError( it->second.planes.size() == planeCount, line, "number of planes does not fit to largest planeIndex of the components" );
  }
}

void VulkanHppGenerator::readFormatsFormatComponent( tinyxml2::XMLElement const * element, FormatData & formatData )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line,
                   attributes,
                   { { "bits", { "1", "2", "4", "5", "6", "8", "9", "10", "11", "12", "16", "24", "32", "64", "compressed" } },
                     { "name", {} },
                     { "numericFormat", { "SFLOAT", "SINT", "SNORM", "SRGB", "SSCALED", "UFLOAT", "UINT", "UNORM", "USCALED" } } },
                   { { "planeIndex", { "0", "1", "2" } } } );
  checkElements( line, getChildElements( element ), {} );

  formatData.components.emplace_back( line );
  ComponentData & component = formatData.components.back();
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

void VulkanHppGenerator::readFormatsFormatPlane( tinyxml2::XMLElement const * element, FormatData & formatData )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes(
    line, attributes, { { "compatible", {} }, { "index", { "0", "1", "2" } }, { "heightDivisor", { "1", "2" } }, { "widthDivisor", { "1", "2" } } }, {} );
  checkElements( line, getChildElements( element ), {} );

  formatData.planes.emplace_back( line );
  PlaneData & plane = formatData.planes.back();
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "compatible" )
    {
      plane.compatible = attribute.second;
      auto formatIt    = m_enums.find( "VkFormat" );
      assert( formatIt != m_enums.end() );
      checkForError( std::find_if( formatIt->second.values.begin(),
                                   formatIt->second.values.end(),
                                   [&plane]( EnumValueData const & evd ) { return evd.name == plane.compatible; } ) != formatIt->second.values.end(),
                     line,
                     "encountered unknown format <" + plane.compatible + ">" );
    }
    else if ( attribute.first == "index" )
    {
      size_t index = std::stoi( attribute.second );
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

void VulkanHppGenerator::readFormatsFormatSPIRVImageFormat( tinyxml2::XMLElement const * element, FormatData & formatData )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  for ( auto const & attribute : attributes )
  {
    assert( attribute.first == "name" );
    checkForError( formatData.spirvImageFormat.empty(), line, "spirvimageformat <" + attribute.second + "> already specified" );
    formatData.spirvImageFormat = attribute.second;
  }
}

std::pair<VulkanHppGenerator::NameData, VulkanHppGenerator::TypeInfo> VulkanHppGenerator::readNameAndType( tinyxml2::XMLElement const * element )
{
  int                                       line     = element->GetLineNum();
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "name", true } }, { { "type" } } );

  NameData nameData;
  TypeInfo typeInfo;
  for ( auto child : children )
  {
    line = child->GetLineNum();
    checkAttributes( line, getAttributes( child ), {}, {} );
    checkElements( line, getChildElements( child ), {} );

    std::string value = child->Value();
    if ( value == "name" )
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

void VulkanHppGenerator::readPlatforms( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "platform", false } } );

  for ( auto child : children )
  {
    readPlatformsPlatform( child );
  }
}

void VulkanHppGenerator::readPlatformsPlatform( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "comment", {} }, { "name", {} }, { "protect", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name, protect;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
      checkForError( !name.empty(), line, "attribute <name> is empty" );
    }
    else if ( attribute.first == "protect" )
    {
      protect = attribute.second;
      checkForError( !protect.empty(), line, "attribute <protect> is empty" );
    }
  }
  assert( !name.empty() && !protect.empty() );

  checkForError( std::find_if( m_platforms.begin(),
                               m_platforms.end(),
                               [&protect]( std::pair<std::string, PlatformData> const & p ) { return p.second.protect == protect; } ) == m_platforms.end(),
                 line,
                 "platform protect <" + protect + "> already specified" );
  checkForError( m_platforms.insert( std::make_pair( name, PlatformData( protect ) ) ).second, line, "platform name <" + name + "> already specified" );
}

void VulkanHppGenerator::readRegistry( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line,
                 children,
                 { { "commands", true },
                   { "comment", false },
                   { "enums", false },
                   { "extensions", true },
                   { "feature", false },
                   { "platforms", true },
                   { "spirvcapabilities", true },
                   { "spirvextensions", true },
                   { "tags", true },
                   { "types", true } },
                 { "formats" } );
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
        setVulkanLicenseHeader( child->GetLineNum(), comment );
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
    else if ( value == "tags" )
    {
      readTags( child );
    }
    else if ( value == "types" )
    {
      readTypes( child );
    }
  }
}

void VulkanHppGenerator::readRequireEnum( tinyxml2::XMLElement const * element, std::string const & extensionName )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  if ( attributes.find( "alias" ) != attributes.end() )
  {
    checkAttributes( line, attributes, { { "alias", {} }, { "name", {} } }, { { "comment", {} }, { "extends", {} } } );
    checkElements( line, getChildElements( element ), {} );

    std::string alias, bitpos, name, extends, extnumber, offset, value;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "alias" )
      {
        alias = attribute.second;
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

    if ( !extends.empty() )
    {
      auto enumIt = m_enums.find( extends );
      checkForError( enumIt != m_enums.end(), line, "feature extends unknown enum <" + extends + ">" );

      // add this enum name to the list of aliases
      enumIt->second.addEnumAlias( line, name, alias );
    }
  }
  else
  {
    checkAttributes( line,
                     attributes,
                     { { "name", {} } },
                     { { "bitpos", {} },
                       { "comment", {} },
                       { "extends", {} },
                       { "dir", { "-" } },
                       { "extnumber", {} },
                       { "offset", {} },
                       { "protect", { "VK_ENABLE_BETA_EXTENSIONS" } },
                       { "value", {} } } );
    checkElements( line, getChildElements( element ), {} );

    std::string bitpos, name, extends, offset, protect, value;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "bitpos" )
      {
        bitpos = attribute.second;
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
      else if ( attribute.first == "value" )
      {
        value = attribute.second;
      }
    }

    if ( !extends.empty() )
    {
      auto enumIt = m_enums.find( extends );
      checkForError( enumIt != m_enums.end(), line, "feature extends unknown enum <" + extends + ">" );

      // add this enum name to the list of values
      checkForError( bitpos.empty() + offset.empty() + value.empty() == 2,
                     line,
                     "exactly one out of bitpos = <" + bitpos + ">, offset = <" + offset + ">, and value = <" + value + "> are supposed to be empty" );
      enumIt->second.addEnumValue( element->GetLineNum(), name, protect, !bitpos.empty(), extensionName );
    }
    else if ( value.empty() )
    {
      checkForError( m_constants.find( name ) != m_constants.end(), line, "unknown required enum <" + name + ">" );
    }
  }
}

void VulkanHppGenerator::readSPIRVCapabilities( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "spirvcapability" } );

  for ( auto child : children )
  {
    assert( child->Value() == std::string( "spirvcapability" ) );
    readSPIRVCapabilitiesSPIRVCapability( child );
  }
}

void VulkanHppGenerator::readSPIRVCapabilitiesSPIRVCapability( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "enable" } );

  for ( auto child : children )
  {
    assert( child->Value() == std::string( "enable" ) );
    readSPIRVCapabilitiesSPIRVCapabilityEnable( child );
  }
}

void VulkanHppGenerator::readSPIRVCapabilitiesSPIRVCapabilityEnable( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkElements( line, getChildElements( element ), {}, {} );

  if ( attributes.find( "extension" ) != attributes.end() )
  {
    readSPIRVCapabilitiesSPIRVCapabilityEnableExtension( line, attributes );
  }
  else if ( attributes.find( "property" ) != attributes.end() )
  {
    readSPIRVCapabilitiesSPIRVCapabilityEnableProperty( line, attributes );
  }
  else if ( attributes.find( "struct" ) != attributes.end() )
  {
    readSPIRVCapabilitiesSPIRVCapabilityEnableStruct( line, attributes );
  }
  else if ( attributes.find( "version" ) != attributes.end() )
  {
    readSPIRVCapabilitiesSPIRVCapabilityEnableVersion( line, attributes );
  }
  else
  {
    checkForError( false, line, "unknown set of attributes specified for SPIR-V capability" );
  }
}

void VulkanHppGenerator::readSPIRVCapabilitiesSPIRVCapabilityEnableExtension( int xmlLine, std::map<std::string, std::string> const & attributes )
{
  checkAttributes( xmlLine, attributes, { { "extension", {} } }, {} );

  checkForError( attributes.size() == 1, xmlLine, "unexpected attributes in addition to <extension> specified for SPIR-V capability" );
  for ( auto const & attribute : attributes )
  {
    assert( attribute.first == "extension" );
    checkForError(
      m_extensions.find( attribute.second ) != m_extensions.end(), xmlLine, "unknown extension <" + attribute.second + "> specified for SPIR-V capability" );
  }
}

void VulkanHppGenerator::readSPIRVCapabilitiesSPIRVCapabilityEnableProperty( int xmlLine, std::map<std::string, std::string> const & attributes )
{
  checkAttributes( xmlLine, attributes, { { "member", {} }, { "property", {} }, { "requires", {} }, { "value", {} } }, {} );

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
      std::vector<std::string> requiresAttribute = tokenize( attribute.second, "," );
      for ( auto const & r : requiresAttribute )
      {
        checkForError( ( m_features.find( r ) != m_features.end() ) || ( m_extensions.find( r ) != m_extensions.end() ),
                       xmlLine,
                       "unknown requires <" + r + "> specified for SPIR-V capability" );
      }
    }
    else if ( attribute.first == "value" )
    {
      value = attribute.second;
    }
  }
  assert( !member.empty() && !property.empty() && !value.empty() );

  auto propertyIt = m_structures.find( property );
  checkForError( propertyIt != m_structures.end(), xmlLine, "unknown property <" + property + "> specified for SPIR-V capability" );
  auto memberIt = findStructMemberIt( member, propertyIt->second.members );
  checkForError( memberIt != propertyIt->second.members.end(), xmlLine, "unknown member <" + member + "> specified for SPIR-V capability" );
  if ( memberIt->type.type == "VkBool32" )
  {
    checkForError( ( value == "VK_FALSE" ) || ( value == "VK_TRUE" ),
                   xmlLine,
                   "unknown value <" + value + "> for boolean member <" + member + "> specified for SPIR-V capability" );
  }
  else
  {
    auto bitmaskIt = m_bitmasks.find( memberIt->type.type );
    checkForError( bitmaskIt != m_bitmasks.end(), xmlLine, "attribute member = <" + member + "> specified for SPIR-V capability is not a bitmask" );
    assert( !bitmaskIt->second.requirements.empty() );
    auto enumIt = m_enums.find( bitmaskIt->second.requirements );
    checkForError( enumIt != m_enums.end(),
                   xmlLine,
                   "attribute member = <" + member + "> specified for SPIR-V capability requires an unknown enum <" + bitmaskIt->second.requirements + ">" );
    auto valueIt =
      std::find_if( enumIt->second.values.begin(), enumIt->second.values.end(), [&value]( EnumValueData const & evd ) { return evd.name == value; } );
    checkForError( valueIt != enumIt->second.values.end(), xmlLine, "unknown attribute value = <" + value + "> specified for SPIR-V capability" );
  }
}

void VulkanHppGenerator::readSPIRVCapabilitiesSPIRVCapabilityEnableStruct( int xmlLine, std::map<std::string, std::string> const & attributes )
{
  checkAttributes( xmlLine, attributes, { { "feature", {} }, { "struct", {} } }, { { "alias", {} }, { "requires", {} } } );

  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "requires" )
    {
      std::vector<std::string> requiresAttribute = tokenize( attribute.second, "," );
      for ( auto const & r : requiresAttribute )
      {
        checkForError( ( m_features.find( r ) != m_features.end() ) || ( m_extensions.find( r ) != m_extensions.end() ),
                       xmlLine,
                       "unknown requires <" + r + "> specified for SPIR-V capability" );
      }
    }
    else if ( attribute.first == "struct" )
    {
      checkForError( ( m_structures.find( attribute.second ) != m_structures.end() ) ||
                       ( m_structureAliases.find( attribute.second ) != m_structureAliases.end() ),
                     xmlLine,
                     "unknown structure <" + attribute.second + "> specified for SPIR-V capability" );
      checkForError( attributes.find( "feature" ) != attributes.end(),
                     xmlLine,
                     "missing feature attribute for SPIR-V capability specified with struct <" + attribute.second + ">" );
    }
    else
    {
      assert( ( attribute.first == "alias" ) || ( attribute.first == "feature" ) );
    }
  }
}

void VulkanHppGenerator::readSPIRVCapabilitiesSPIRVCapabilityEnableVersion( int xmlLine, std::map<std::string, std::string> const & attributes )
{
  checkAttributes( xmlLine, attributes, { { "version", {} } }, {} );

  checkForError( attributes.size() == 1, xmlLine, "unexpected attributes in addition to <version> specified for SPIR-V capability" );
  for ( auto const & attribute : attributes )
  {
    assert( attribute.first == "version" );
    std::string feature = attribute.second;
    if ( feature.starts_with( "VK_API_" ) )
    {
      feature.erase( 3, 4 );  // remove "API_" from the version -> VK_VERSION_x_y
    }
    checkForError( feature.starts_with( "VK_VERSION_" ), xmlLine, "unknown version <" + attribute.second + "> specified for SPIR-V capability" );
    checkForError( m_features.find( feature ) != m_features.end(), xmlLine, "unknown version <" + attribute.second + "> specified for SPIR-V capability" );
  }
}

void VulkanHppGenerator::readSPIRVExtensions( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "spirvextension" } );

  for ( auto child : children )
  {
    assert( child->Value() == std::string( "spirvextension" ) );
    readSPIRVExtensionsExtension( child );
  }
}

void VulkanHppGenerator::readSPIRVExtensionsExtension( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "enable" } );

  for ( auto child : children )
  {
    assert( child->Value() == std::string( "enable" ) );
    readSPIRVExtensionsExtensionEnable( child );
  }
}

void VulkanHppGenerator::readSPIRVExtensionsExtensionEnable( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, { { "extension", {} }, { "version", {} } } );
  checkElements( line, getChildElements( element ), {}, {} );

  checkForError( !attributes.empty(), line, "no version or extension specified for SPIR-V extension" );
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "extension" )
    {
      checkForError(
        m_extensions.find( attribute.second ) != m_extensions.end(), line, "unknown extension <" + attribute.second + "> specified for SPIR-V extension" );
    }
    else
    {
      assert( attribute.first == "version" );
      std::string feature = attribute.second;
      if ( feature.starts_with( "VK_API_" ) )
      {
        feature.erase( 3, 4 );  // remove "API_" from the version -> VK_VERSION_x_y
      }
      checkForError( feature.starts_with( "VK_VERSION_" ), line, "unknown version <" + attribute.second + "> specified for SPIR-V extension" );
      checkForError( m_features.find( feature ) != m_features.end(), line, "unknown version <" + attribute.second + "> specified for SPIR-V extension" );
    }
  }
}

void VulkanHppGenerator::readTags( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "tag", false } } );

  for ( auto child : children )
  {
    readTagsTag( child );
  }
}

void VulkanHppGenerator::readTagsTag( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "author", {} }, { "contact", {} }, { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkForError( m_tags.find( attribute.second ) == m_tags.end(), line, "tag named <" + attribute.second + "> has already been specified" );
      m_tags.insert( attribute.second );
    }
    else
    {
      checkForError( ( attribute.first == "author" ) || ( attribute.first == "contact" ), line, "unknown attribute <" + attribute.first + ">" );
    }
  }
}

void VulkanHppGenerator::readTypes( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "comment", false }, { "type", false } } );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "comment" )
    {
      readComment( child );
    }
    else
    {
      assert( value == "type" );
      readTypesType( child );
    }
  }
}

void VulkanHppGenerator::readTypesType( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );

  auto categoryIt = attributes.find( "category" );
  if ( categoryIt != attributes.end() )
  {
    if ( categoryIt->second == "basetype" )
    {
      readTypesTypeBasetype( element, attributes );
    }
    else if ( categoryIt->second == "bitmask" )
    {
      readTypesTypeBitmask( element, attributes );
    }
    else if ( categoryIt->second == "define" )
    {
      readTypesTypeDefine( element, attributes );
    }
    else if ( categoryIt->second == "enum" )
    {
      readTypesTypeEnum( element, attributes );
    }
    else if ( categoryIt->second == "funcpointer" )
    {
      readTypesTypeFuncpointer( element, attributes );
    }
    else if ( categoryIt->second == "handle" )
    {
      readTypesTypeHandle( element, attributes );
    }
    else if ( categoryIt->second == "include" )
    {
      readTypesTypeInclude( element, attributes );
    }
    else if ( categoryIt->second == "struct" )
    {
      readTypesTypeStruct( element, false, attributes );
    }
    else
    {
      checkForError( categoryIt->second == "union", element->GetLineNum(), "unknown type category <" + categoryIt->second + ">" );
      readTypesTypeStruct( element, true, attributes );
    }
  }
  else
  {
    auto requiresIt = attributes.find( "requires" );
    if ( requiresIt != attributes.end() )
    {
      readTypesTypeRequires( element, attributes );
    }
    else
    {
      checkForError( ( attributes.size() == 1 ) && ( attributes.begin()->first == "name" ) && ( attributes.begin()->second == "int" ), line, "unknown type" );
      checkForError( m_types.insert( std::make_pair( attributes.begin()->second, TypeCategory::Unknown ) ).second,
                     line,
                     "type <" + attributes.begin()->second + "> already specified" );
    }
  }
}

void VulkanHppGenerator::readTypesTypeBasetype( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
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

  if ( !typeInfo.type.empty() )
  {
    checkForError( m_baseTypes.insert( std::make_pair( nameData.name, BaseTypeData( typeInfo, line ) ) ).second,
                   line,
                   "basetype <" + nameData.name + "> already specified" );
  }
  checkForError(
    m_types.insert( std::make_pair( nameData.name, TypeCategory::BaseType ) ).second, line, "basetype <" + nameData.name + "> already specified as a type" );
}

void VulkanHppGenerator::readTypesTypeBitmask( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();

  auto aliasIt = attributes.find( "alias" );
  if ( aliasIt != attributes.end() )
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

    auto bitmasksIt = m_bitmasks.find( alias );
    checkForError( bitmasksIt != m_bitmasks.end(), line, "missing alias <" + alias + ">." );
    checkForError(
      bitmasksIt->second.alias.empty(), line, "alias for bitmask <" + bitmasksIt->first + "> already specified as <" + bitmasksIt->second.alias + ">" );
    bitmasksIt->second.alias = name;
    checkForError( m_types.insert( std::make_pair( name, TypeCategory::Bitmask ) ).second, line, "aliased bitmask <" + name + "> already specified as a type" );
  }
  else
  {
    checkAttributes( line, attributes, { { "category", { "bitmask" } } }, { { "bitvalues", {} }, { "requires", {} } } );

    std::string bitvalues, requirements;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "bitvalues" )
      {
        bitvalues = attribute.second;
      }
      else if ( attribute.first == "requires" )
      {
        requirements = attribute.second;
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
    checkForError( bitvalues.empty() || requirements.empty(), line, "attributes <bitvalues> and <requires> are both specified" );
    checkForError( ( typeInfo.type != "VkFlags64" ) || !bitvalues.empty(), line, "bitmask of type <VkFlags64> needs attribute bitvalues to be set" );

    if ( !bitvalues.empty() )
    {
      requirements = bitvalues;
    }
    m_bitmasks.insert( std::make_pair( nameData.name, BitmaskData( requirements, typeInfo.type, line ) ) );
    checkForError(
      m_types.insert( std::make_pair( nameData.name, TypeCategory::Bitmask ) ).second, line, "bitmask <" + nameData.name + "> already specified as a type" );
  }
}

void VulkanHppGenerator::readTypesTypeDefine( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "define" } } }, { { "name", {} }, { "requires", {} } } );

  std::string name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "requires" )
    {
      checkForError( m_defines.find( attribute.second ) != m_defines.end(), line, "using undefined requires <" + attribute.second + ">" );
    }
  }

  if ( !name.empty() )
  {
    checkForError( !element->FirstChildElement(), line, "unknown formatting of type category=define name <" + name + ">" );
    checkForError( element->LastChild() && element->LastChild()->ToText() && element->LastChild()->ToText()->Value(),
                   line,
                   "unknown formatting of type category=define named <" + name + ">" );

    // filter out the check for the different types of VK_DEFINE_NON_DISPATCHABLE_HANDLE
    if ( name == "VK_USE_64_BIT_PTR_DEFINES" )
    {
      m_typesafeCheck = "#if ( VK_USE_64_BIT_PTR_DEFINES == 1 )";
    }
    else if ( ( name == "VK_DEFINE_NON_DISPATCHABLE_HANDLE" ) && ( m_typesafeCheck.empty() ) )
    {
      std::string text  = element->LastChild()->ToText()->Value();
      size_t      start = text.find( "#if defined(__LP64__)" );
      checkForError( start != std::string::npos, line, "unexpected text in type category=define named <" + name + ">" );
      size_t end = text.find_first_of( "\r\n", start + 1 );
      checkForError( end != std::string::npos, line, "unexpected text in type category=define named <" + name + ">" );
      m_typesafeCheck = text.substr( start, end - start );
    }
  }
  else if ( element->GetText() )
  {
    std::string text = element->GetText();
    if ( ( text.find( "class" ) != std::string::npos ) || ( text.find( "struct" ) != std::string::npos ) )
    {
      // here are a couple of structs as defines, which really are types!
      tinyxml2::XMLElement const * child = element->FirstChildElement();
      checkForError( child && ( strcmp( child->Value(), "name" ) == 0 ) && child->GetText(), line, "unexpected formatting of type category=define" );
      name = child->GetText();
      checkForError( m_types.insert( std::make_pair( name, TypeCategory::Define ) ).second, line, "type <" + name + "> has already been speficied" );
    }
    else
    {
      tinyxml2::XMLElement const * child = element->FirstChildElement();
      checkForError( child && !child->FirstAttribute() && ( strcmp( child->Value(), "name" ) == 0 ) && child->GetText(),
                     line,
                     "unknown formatting of type category define" );
      name = trim( child->GetText() );
      if ( name == "VK_HEADER_VERSION" )
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
  checkForError( m_defines.insert( name ).second, line, "define <" + name + "> has already been specified" );
}

void VulkanHppGenerator::readTypesTypeEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "enum" } }, { "name", {} } }, { { "alias", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string alias, name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      alias = attribute.second;
      checkForError( !alias.empty(), line, "enum with empty alias" );
    }
    else if ( attribute.first == "name" )
    {
      name = attribute.second;
      checkForError( !name.empty(), line, "enum with empty name" );
      checkForError( m_enums.find( name ) == m_enums.end(), line, "enum <" + name + "> already specified" );
    }
  }
  assert( !name.empty() );

  if ( alias.empty() )
  {
    checkForError( m_enums.insert( std::make_pair( name, EnumData( line ) ) ).second, line, "enum <" + name + "> already specified" );
  }
  else
  {
    auto enumIt = m_enums.find( alias );
    checkForError( enumIt != m_enums.end(), line, "enum with unknown alias <" + alias + ">" );
    checkForError( enumIt->second.alias.empty(), line, "enum <" + enumIt->first + "> already has an alias <" + enumIt->second.alias + ">" );
    enumIt->second.alias = name;
  }
  checkForError( m_types.insert( std::make_pair( name, TypeCategory::Enum ) ).second, line, "enum <" + name + "> already specified as a type" );
}

void VulkanHppGenerator::readTypesTypeFuncpointer( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "funcpointer" } } }, { { "requires", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "name", true } }, { "type" } );

  std::string requirements;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "requires" )
    {
      requirements = attribute.second;
    }
  }

  auto                  funcPointerIt = m_funcPointers.end();
  std::set<std::string> argumentNames;
  for ( auto const & child : children )
  {
    std::string value     = child->Value();
    int         childLine = child->GetLineNum();
    if ( value == "name" )
    {
      std::string name = child->GetText();
      checkForError( !name.empty(), childLine, "funcpointer with empty name" );
      checkForError( m_funcPointers.find( name ) == m_funcPointers.end(), childLine, "funcpointer <" + name + "> already specified" );
      funcPointerIt = m_funcPointers.insert( std::make_pair( name, FuncPointerData( requirements, line ) ) ).first;
      checkForError(
        m_types.insert( std::make_pair( name, TypeCategory::FuncPointer ) ).second, childLine, "funcpointer <" + name + "> already specified as a type" );
    }
    else if ( value == "type" )
    {
      assert( funcPointerIt != m_funcPointers.end() );
      std::string type = child->GetText();
      funcPointerIt->second.arguments.push_back( { type, childLine } );

      auto         sibling      = child->NextSibling();
      char const * siblingValue = sibling->Value();
      assert( siblingValue != nullptr );
      std::string argumentName = siblingValue;
      argumentName             = argumentName.substr( argumentName.find_first_not_of( "* " ) );
      argumentName             = argumentName.substr( 0, argumentName.find_first_of( ",)" ) );
      checkForError( argumentNames.insert( argumentName ).second,
                     childLine,
                     "funcpointer <" + funcPointerIt->first + "> already has an argument named <" + argumentName + ">" );
    }
  }
}

void VulkanHppGenerator::readTypesTypeHandle( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();

  auto aliasIt = attributes.find( "alias" );
  if ( aliasIt != attributes.end() )
  {
    checkAttributes( line, attributes, { { "alias", {} }, { "category", { "handle" } }, { "name", {} } }, {} );
    checkElements( line, getChildElements( element ), {} );

    auto handlesIt = m_handles.find( aliasIt->second );
    checkForError( handlesIt != m_handles.end(), line, "using unspecified alias <" + aliasIt->second + ">." );
    checkForError( handlesIt->second.alias.empty(), line, "handle <" + handlesIt->first + "> already has an alias <" + handlesIt->second.alias + ">" );
    handlesIt->second.alias = attributes.find( "name" )->second;
    checkForError( m_types.insert( std::make_pair( handlesIt->second.alias, TypeCategory::Handle ) ).second,
                   line,
                   "handle alias <" + handlesIt->second.alias + "> already specified as a type" );
  }
  else
  {
    checkAttributes( line, attributes, { { "category", { "handle" } } }, { { "objtypeenum", {} }, { "parent", {} } } );

    std::string objTypeEnum, parent;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "objtypeenum" )
      {
        objTypeEnum = attribute.second;
      }
      else if ( attribute.first == "parent" )
      {
        parent = attribute.second;
      }
    }

    NameData nameData;
    TypeInfo typeInfo;
    std::tie( nameData, typeInfo ) = readNameAndType( element );
    const bool isDispatchable      = typeInfo.type == "VK_DEFINE_HANDLE";

    checkForError( nameData.name.starts_with( "Vk" ), line, "name <" + nameData.name + "> does not begin with <Vk>" );
    checkForError( nameData.arraySizes.empty(), line, "name <" + nameData.name + "> with unsupported arraySizes" );
    checkForError( ( typeInfo.type == "VK_DEFINE_HANDLE" ) || ( typeInfo.type == "VK_DEFINE_NON_DISPATCHABLE_HANDLE" ),
                   line,
                   "handle with invalid type <" + typeInfo.type + ">" );
    checkForError( typeInfo.prefix.empty(), line, "unexpected type prefix <" + typeInfo.prefix + ">" );
    checkForError( typeInfo.postfix == "(", line, "unexpected type postfix <" + typeInfo.postfix + ">" );
    checkForError( !objTypeEnum.empty(), line, "handle <" + nameData.name + "> does not specify attribute \"objtypeenum\"" );

    checkForError( parent.find( ',' ) == std::string::npos, line, "mulitple parents specified for handle <" + nameData.name + ">" );
    checkForError( m_handles.insert( std::make_pair( nameData.name, HandleData( parent, objTypeEnum, isDispatchable, line ) ) ).second,
                   line,
                   "handle <" + nameData.name + "> already specified" );
    checkForError(
      m_types.insert( std::make_pair( nameData.name, TypeCategory::Handle ) ).second, line, "handle <" + nameData.name + "> already specified as a type" );
  }
}

void VulkanHppGenerator::readTypesTypeInclude( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "include" } }, { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name = attributes.find( "name" )->second;
  checkForError( m_includes.insert( name ).second, element->GetLineNum(), "include named <" + name + "> already specified" );
}

void VulkanHppGenerator::readTypesTypeRequires( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "name", {} }, { "requires", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  for ( auto attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkForError(
        m_types.insert( std::make_pair( attribute.second, TypeCategory::Requires ) ).second, line, "type named <" + attribute.second + "> already specified" );
    }
    else
    {
      assert( attribute.first == "requires" );
      checkForError( m_includes.find( attribute.second ) != m_includes.end(), line, "type requires unknown include <" + attribute.second + ">" );
    }
  }
}

void VulkanHppGenerator::readTypesTypeStruct( tinyxml2::XMLElement const * element, bool isUnion, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  if ( attributes.find( "alias" ) != attributes.end() )
  {
    checkAttributes( line, attributes, { { "alias", {} }, { "category", { "struct" } }, { "name", {} } }, {} );
    checkElements( line, getChildElements( element ), {}, {} );

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

    checkForError(
      m_structureAliases.insert( std::make_pair( name, StructureAliasData( alias, line ) ) ).second, line, "structure alias <" + name + "> already used" );
    checkForError( m_structureAliasesInverse[alias].insert( name ).second, line, "structure alias <" + name + "> already used with structure <" + alias + ">" );
    checkForError( m_types.insert( std::make_pair( name, TypeCategory::Struct ) ).second, line, "struct <" + name + "> already specified as a type" );
  }
  else
  {
    checkAttributes( line,
                     attributes,
                     { { "category", { isUnion ? "union" : "struct" } }, { "name", {} } },
                     { { "allowduplicate", { "false", "true" } }, { "comment", {} }, { "returnedonly", { "true" } }, { "structextends", {} } } );
    std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
    checkElements( line, children, {}, { "member", "comment" } );

    std::string              category, name;
    std::vector<std::string> structExtends;
    bool                     allowDuplicate = false;
    bool                     returnedOnly   = false;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "allowduplicate" )
      {
        allowDuplicate = ( attribute.second == "true" );
      }
      else if ( attribute.first == "category" )
      {
        category = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        name = attribute.second;
      }
      else if ( attribute.first == "returnedonly" )
      {
        checkForError( attribute.second == "true", line, "unknown value for attribute returnedonly: <" + attribute.second + ">" );
        returnedOnly = true;
      }
      else if ( attribute.first == "structextends" )
      {
        structExtends = tokenize( attribute.second, "," );
      }
    }
    assert( !name.empty() );
    // make this warn a check, as soon as vk.xml has been fixed on attribute "allowduplicate" !
    checkForWarning( !allowDuplicate || !structExtends.empty(), line, "attribute <allowduplicate> is true, but no structures are listed in <structextends>" );

    checkForError( m_structures.find( name ) == m_structures.end(), line, "struct <" + name + "> already specfied" );
    std::map<std::string, StructureData>::iterator it = m_structures.insert( std::make_pair( name, StructureData( structExtends, line ) ) ).first;
    it->second.allowDuplicate                         = allowDuplicate;
    it->second.isUnion                                = isUnion;
    it->second.returnedOnly                           = returnedOnly;

    for ( auto child : children )
    {
      std::string value = child->Value();
      if ( value == "comment" )
      {
        readComment( child );
      }
      else if ( value == "member" )
      {
        readTypesTypeStructMember( child, it->second.members, isUnion );
      }
    }
    it->second.subStruct = determineSubStruct( *it );

    // check if multiple structure members use the very same (not empty) len attribute
    // Note: even though the arrays are not marked as optional, they still might be mutually exclusive (like in
    // VkWriteDescriptorSet)! That is, there's not enough information available in vk.xml to decide on that, so we
    // need this external knowledge!
    static std::set<std::string> mutualExclusiveStructs = {
      "VkAccelerationStructureBuildGeometryInfoKHR", "VkAccelerationStructureTrianglesOpacityMicromapEXT", "VkMicromapBuildInfoEXT", "VkWriteDescriptorSet"
    };
    static std::set<std::string> multipleLenStructs = { "VkImageConstraintsInfoFUCHSIA",
                                                        "VkIndirectCommandsLayoutTokenNV",
                                                        "VkPresentInfoKHR",
                                                        "VkSemaphoreWaitInfo",
                                                        "VkSubmitInfo",
                                                        "VkSubpassDescription",
                                                        "VkSubpassDescription2",
                                                        "VkWin32KeyedMutexAcquireReleaseInfoKHR",
                                                        "VkWin32KeyedMutexAcquireReleaseInfoNV" };
    bool                         warned             = false;
    for ( auto m0It = it->second.members.begin(); !warned && ( m0It != it->second.members.end() ); ++m0It )
    {
      if ( !m0It->len.empty() && ( m0It->len.front() != "null-terminated" ) )
      {
        for ( auto m1It = std::next( m0It ); !warned && ( m1It != it->second.members.end() ); ++m1It )
        {
          if ( !m1It->len.empty() && ( m0It->len.front() == m1It->len.front() ) )
          {
            if ( mutualExclusiveStructs.find( it->first ) != mutualExclusiveStructs.end() )
            {
              it->second.mutualExclusiveLens = true;
            }
            else
            {
              checkForWarning(
                multipleLenStructs.find( it->first ) != multipleLenStructs.end(),
                line,
                "Encountered structure <" + it->first +
                  "> with multiple members referencing the same member for len. Need to be checked if they are supposed to be mutually exclusive." );
              warned = true;
            }
          }
        }
      }
    }

    m_extendedStructs.insert( structExtends.begin(), structExtends.end() );
    checkForError( m_types.insert( std::make_pair( name, ( category == "struct" ) ? TypeCategory::Struct : TypeCategory::Union ) ).second,
                   line,
                   "struct <" + name + "> already specified as a type" );  // log type and alias in m_types
  }
}

void VulkanHppGenerator::readTypesTypeStructMember( tinyxml2::XMLElement const * element, std::vector<MemberData> & members, bool isUnion )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line,
                   attributes,
                   {},
                   { { "altlen", {} },
                     { "externsync", { "true" } },
                     { "len", {} },
                     { "limittype", { "bitmask", "bits", "exact", "max", "min", "mul", "noauto", "pot", "range", "struct" } },
                     { "noautovalidity", { "true" } },
                     { "objecttype", { "objectType" } },
                     { "optional", { "false", "true" } },
                     { "selection", {} },
                     { "selector", {} },
                     { "values", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "name", true }, { "type", true } }, { "comment", "enum" } );

  MemberData memberData( line );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "enum" )
    {
      readTypesTypeStructMemberEnum( child, memberData );
    }
    else if ( value == "name" )
    {
      readTypesTypeStructMemberName( child, memberData, members );
    }
    else if ( value == "type" )
    {
      readTypesTypeStructMemberType( child, memberData );
    }
  }

  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "altlen" )
    {
      assert( memberData.len.empty() );
      memberData.len = tokenize( attribute.second, "," );
      checkForError( memberData.len.size() == 1, line, "member attribute <altlen> holds unknown number of data: " + std::to_string( memberData.len.size() ) );
      checkForError( altLens.find( memberData.len[0] ) != altLens.end(), line, "member attribute <altlen> holds unknown value <" + memberData.len[0] + ">" );
    }
    else if ( attribute.first == "len" )
    {
      if ( memberData.len.empty() )
      {
        memberData.len = tokenize( attribute.second, "," );
        checkForError( !memberData.len.empty() && ( memberData.len.size() <= 2 ),
                       line,
                       "member attribute <len> holds unknown number of data: " + std::to_string( memberData.len.size() ) );
        auto lenMember = findStructMemberIt( memberData.len[0], members );
        checkForError( lenMember != members.end() || ( memberData.len[0] == "null-terminated" ),
                       line,
                       "member attribute <len> holds unknown value <" + memberData.len[0] + ">" );
        if ( lenMember != members.end() )
        {
          checkForError( lenMember->type.prefix.empty(),
                         line,
                         "member attribute <len> references a member of unexpected type <" + lenMember->type.compose( "VULKAN_HPP_NAMESPACE" ) + ">" );
        }
        if ( 1 < memberData.len.size() )
        {
          checkForError( ( memberData.len[1] == "1" ) || ( memberData.len[1] == "null-terminated" ),
                         line,
                         "member attribute <len> holds unknown second value <" + memberData.len[1] + ">" );
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
      checkForError( m_enums.find( selectorIt->type.type ) != m_enums.end(),
                     line,
                     "member attribute <selector> references unknown enum type <" + selectorIt->type.type + ">" );
    }
    else if ( attribute.first == "values" )
    {
      std::vector<std::string> values = tokenize( attribute.second, "," );
      checkForError( values.size() == 1, line, "attribute \"values\" holds multiple values <" + attribute.first + ">, but it's expected to hold just one" );
      memberData.value = values[0];
    }
  }

  members.push_back( memberData );
}

void VulkanHppGenerator::readTypesTypeStructMemberEnum( tinyxml2::XMLElement const * element, MemberData & memberData )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  checkElements( line, getChildElements( element ), {}, {} );

  std::string enumString = element->GetText();

  checkForError( element->PreviousSibling() && ( strcmp( element->PreviousSibling()->Value(), "[" ) == 0 ) && element->NextSibling() &&
                   ( strcmp( element->NextSibling()->Value(), "]" ) == 0 ),
                 line,
                 std::string( "structure member array specifiation is ill-formatted: <" ) + enumString + ">" );

  memberData.arraySizes.push_back( enumString );
  checkForError( memberData.usedConstant.empty(), line, "struct already holds a constant <" + memberData.usedConstant + ">" );
  memberData.usedConstant = enumString;
}

void VulkanHppGenerator::readTypesTypeStructMemberName( tinyxml2::XMLElement const * element, MemberData & memberData, std::vector<MemberData> const & members )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  checkElements( line, getChildElements( element ), {}, {} );

  std::string name = element->GetText();
  checkForError( !isStructMember( name, members ), line, "structure member name <" + name + "> already used" );

  memberData.name                                        = name;
  std::tie( memberData.arraySizes, memberData.bitCount ) = readModifiers( element->NextSibling() );
}

void VulkanHppGenerator::readTypesTypeStructMemberType( tinyxml2::XMLElement const * element, MemberData & memberData )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  checkElements( line, getChildElements( element ), {}, {} );

  memberData.type = readTypeInfo( element );
}

VulkanHppGenerator::TypeInfo VulkanHppGenerator::readTypeInfo( tinyxml2::XMLElement const * element ) const
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

void VulkanHppGenerator::registerDeleter( std::string const & name, std::pair<std::string, CommandData> const & commandData )
{
  if ( ( commandData.first.substr( 2, 7 ) == "Destroy" ) || ( commandData.first.substr( 2, 4 ) == "Free" ) )
  {
    std::string key;
    size_t      valueIndex;
    switch ( commandData.second.params.size() )
    {
      case 2:
      case 3:
        assert( commandData.second.params.back().type.type == "VkAllocationCallbacks" );
        key        = ( commandData.second.params.size() == 2 ) ? "" : commandData.second.params[0].type.type;
        valueIndex = commandData.second.params.size() - 2;
        break;
      case 4:
        key        = commandData.second.params[0].type.type;
        valueIndex = 3;
        assert( m_handles.find( commandData.second.params[valueIndex].type.type ) != m_handles.end() );
        m_handles.find( commandData.second.params[valueIndex].type.type )->second.deletePool = commandData.second.params[1].type.type;
        break;
      default: assert( false ); valueIndex = 0;
    }
    auto keyHandleIt = m_handles.find( key );
    assert( ( keyHandleIt != m_handles.end() ) &&
            ( keyHandleIt->second.childrenHandles.find( commandData.second.params[valueIndex].type.type ) == keyHandleIt->second.childrenHandles.end() ) );
    keyHandleIt->second.childrenHandles.insert( commandData.second.params[valueIndex].type.type );

    auto handleIt = m_handles.find( commandData.second.params[valueIndex].type.type );
    assert( handleIt != m_handles.end() );
    handleIt->second.deleteCommand = name;
  }
}

void VulkanHppGenerator::rescheduleRAIIHandle( std::string &                              str,
                                               std::pair<std::string, HandleData> const & handle,
                                               std::set<std::string> &                    listedHandles,
                                               std::set<std::string> const &              specialFunctions ) const
{
  listedHandles.insert( handle.first );
  if ( !handle.second.parent.empty() && ( listedHandles.find( handle.second.parent ) == listedHandles.end() ) )
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
      if ( handleIt != m_handles.end() && ( listedHandles.find( param.type.type ) == listedHandles.end() ) )
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
      if ( ( handleCommands.find( command ) != handleCommands.end() ) && listedCommands.insert( command ).second )
      {
        selectedCommands.push_back( command );
      }
    }
  }
  return selectedCommands;
}

void VulkanHppGenerator::setVulkanLicenseHeader( int line, std::string const & comment )
{
  checkForError( m_vulkanLicenseHeader.empty(), line, "second encounter of a Copyright comment" );
  m_vulkanLicenseHeader = comment;

  // replace any '\n' with "\n// "
  for ( size_t pos = m_vulkanLicenseHeader.find( '\n' ); pos != std::string::npos; pos = m_vulkanLicenseHeader.find( '\n', pos + 1 ) )
  {
    m_vulkanLicenseHeader.replace( pos, 1, "\n// " );
  }
  // replace any " \n" with "\n"
  for ( size_t pos = m_vulkanLicenseHeader.find( " \n" ); pos != std::string::npos; pos = m_vulkanLicenseHeader.find( " \n", pos ) )
  {
    m_vulkanLicenseHeader.replace( pos, 2, "\n" );
  }
  // remove any trailing spaces
  m_vulkanLicenseHeader = trimEnd( m_vulkanLicenseHeader );

  // and add a little message on our own
  m_vulkanLicenseHeader += "\n\n// This header is generated from the Khronos Vulkan XML API Registry.";
  m_vulkanLicenseHeader = trim( m_vulkanLicenseHeader ) + "\n";
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

std::string VulkanHppGenerator::toString( TypeCategory category )
{
  switch ( category )
  {
    case TypeCategory::Bitmask: return "bitmask";
    case TypeCategory::BaseType: return "basetype";
    case TypeCategory::Define: return "define";
    case TypeCategory::Enum: return "enum";
    case TypeCategory::FuncPointer: return "funcpointer";
    case TypeCategory::Handle: return "handle";
    case TypeCategory::Requires: return "requires";
    case TypeCategory::Struct: return "struct";
    case TypeCategory::Union: return "union";
    case TypeCategory::Unknown: return "unkown";
    default: assert( false ); return "";
  }
}

void VulkanHppGenerator::EnumData::addEnumAlias( int line, std::string const & name, std::string const & aliasName )
{
  auto aliasIt = aliases.find( name );
  checkForError(
    ( aliasIt == aliases.end() ) || ( aliasIt->second.name == aliasName ), line, "enum alias <" + name + "> already listed for a different enum value" );
  aliases.insert( std::make_pair( name, EnumAliasData( aliasName, line ) ) );
}

void VulkanHppGenerator::EnumData::addEnumValue(
  int line, std::string const & valueName, std::string const & protect, bool bitpos, std::string const & extension )
{
  auto valueIt = std::find_if( values.begin(), values.end(), [&valueName]( EnumValueData const & evd ) { return evd.name == valueName; } );
  if ( valueIt == values.end() )
  {
    values.emplace_back( line, valueName, protect, extension, bitpos );
  }
}

std::string VulkanHppGenerator::TypeInfo::compose( std::string const & nameSpace ) const
{
  return prefix + ( prefix.empty() ? "" : " " ) +
         ( nameSpace.empty() ? type : ( ( ( type.substr( 0, 2 ) == "Vk" ) ? ( nameSpace + "::" ) : "" ) + stripPrefix( type, "Vk" ) ) ) +
         ( postfix.empty() ? "" : " " ) + postfix;
}

VulkanHppGenerator::RequireData::RequireData( int line, std::vector<std::string> const & depends_ ) : depends( depends_ ), xmlLine( line ) {}

//
// VulkanHppGenerator local functions
//

// check the validity of an attributes map
// line       : the line in the xml file where the attributes are listed
// attributes : the map of name/value pairs of the encountered attributes
// required   : the required attributes, with a set of allowed values per attribute
// optional   : the optional attributes, with a set of allowed values per attribute
void checkAttributes( int                                                  line,
                      std::map<std::string, std::string> const &           attributes,
                      std::map<std::string, std::set<std::string>> const & required,
                      std::map<std::string, std::set<std::string>> const & optional )
{
  // check if all required attributes are included and if there is a set of allowed values, check if the actual
  // value is part of that set
  for ( auto const & r : required )
  {
    auto attributesIt = attributes.find( r.first );
    checkForError( attributesIt != attributes.end(), line, "missing attribute <" + r.first + ">" );
    checkForError( r.second.empty() || ( r.second.find( attributesIt->second ) != r.second.end() ),
                   line,
                   "unexpected attribute value <" + attributesIt->second + "> in attribute <" + r.first + ">" );
  }
  // check if all not required attributes or optional, and if there is a set of allowed values, check if the
  // actual value is part of that set
  for ( auto const & a : attributes )
  {
    if ( required.find( a.first ) == required.end() )
    {
      auto optionalIt = optional.find( a.first );
      if ( optionalIt == optional.end() )
      {
        checkForWarning( false, line, "unknown attribute <" + a.first + ">" );
        continue;
      }
      if ( !optionalIt->second.empty() )
      {
        std::vector<std::string> values = tokenize( a.second, "," );
        for ( auto const & v : values )
        {
          checkForWarning(
            optionalIt->second.find( v ) != optionalIt->second.end(), line, "unexpected attribute value <" + v + "> in attribute <" + a.first + ">" );
        }
      }
    }
  }
}

void checkElements( int                                               line,
                    std::vector<tinyxml2::XMLElement const *> const & elements,
                    std::map<std::string, bool> const &               required,
                    std::set<std::string> const &                     optional )
{
  std::map<std::string, size_t> encountered;
  for ( auto const & e : elements )
  {
    std::string value = e->Value();
    encountered[value]++;
    checkForWarning(
      ( required.find( value ) != required.end() ) || ( optional.find( value ) != optional.end() ), e->GetLineNum(), "unknown element <" + value + ">" );
  }
  for ( auto const & r : required )
  {
    auto encounteredIt = encountered.find( r.first );
    checkForError( encounteredIt != encountered.end(), line, "missing required element <" + r.first + ">" );
    // check: r.second (means: required excactly once) => (encouteredIt->second == 1)
    checkForError( !r.second || ( encounteredIt->second == 1 ),
                   line,
                   "required element <" + r.first + "> is supposed to be listed exactly once, but is listed " + std::to_string( encounteredIt->second ) );
  }
}

void checkForError( bool condition, int line, std::string const & message )
{
  if ( !condition )
  {
    throw std::runtime_error( "VulkanHppGenerator: Spec error on line " + std::to_string( line ) + ": " + message );
  }
}

void checkForWarning( bool condition, int line, std::string const & message )
{
  if ( !condition )
  {
    std::cerr << "VulkanHppGenerator: Spec warning on line " << std::to_string( line ) << ": " << message << "!" << std::endl;
  }
}

std::string findTag( std::set<std::string> const & tags, std::string const & name, std::string const & postfix )
{
  auto tagIt = std::find_if( tags.begin(), tags.end(), [&name, &postfix]( std::string const & t ) { return name.ends_with( t + postfix ); } );
  return ( tagIt != tags.end() ) ? *tagIt : "";
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

std::pair<std::string, std::string> generateEnumSuffixes( std::string const & name, bool bitmask, std::set<std::string> const & tags )
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
      size_t pos = name.find( "FlagBits" );
      assert( pos != std::string::npos );
      std::string shortenedName = name;
      shortenedName.erase( pos, strlen( "FlagBits" ) );
      std::string tag = findTag( tags, shortenedName );
      prefix          = toUpperCase( stripPostfix( shortenedName, tag ) ) + "_";
    }
    else
    {
      // for a non-bitmask enum, convert the name to upper case
      prefix = toUpperCase( name ) + "_";
    }

    // if the enum name contains a tag move it from the prefix to the postfix to generate correct enum value
    // names.
    for ( auto const & tag : tags )
    {
      if ( prefix.ends_with( tag + "_" ) )
      {
        prefix.erase( prefix.length() - tag.length() - 1 );
        postfix = "_" + tag;
        break;
      }
      else if ( name.ends_with( tag ) )
      {
        postfix = "_" + tag;
        break;
      }
    }
  }

  return std::make_pair( prefix, postfix );
}

std::string generateEnumValueName( std::string const & enumName, std::string const & valueName, bool bitmask, std::set<std::string> const & tags )
{
  std::string prefix, postfix;
  std::tie( prefix, postfix ) = generateEnumSuffixes( enumName, bitmask, tags );
  std::string tag             = findTag( tags, valueName, "" );
  if ( postfix == "_" + tag )
  {
    tag = findTag( tags, valueName, postfix );
  }

  std::string result = "e" + toCamelCase( stripPostfix( stripPrefix( valueName, prefix ), postfix ) );
  if ( bitmask )
  {
    size_t pos = result.find( "Bit" );
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

std::string generateStandardArrayWrapper( std::string const & type, std::vector<std::string> const & sizes )
{
  std::string arrayString = "VULKAN_HPP_NAMESPACE::ArrayWrapper" + std::to_string( sizes.size() ) + "D<" + type;
  for ( auto const & size : sizes )
  {
    arrayString += ", " + size;
  }
  arrayString += ">";
  return arrayString;
}

std::string generateSuccessCode( std::string const & code, std::set<std::string> const & tags )
{
  std::string tag = findTag( tags, code );
  // on each success code: prepend 'VULKAN_HPP_NAMESPACE::Result::e', strip "VK_" and a tag, convert it to camel
  // case, and add the tag again
  return "VULKAN_HPP_NAMESPACE::Result::e" + toCamelCase( stripPostfix( stripPrefix( code, "VK_" ), tag ) ) + tag;
}

std::map<std::string, std::string> getAttributes( tinyxml2::XMLElement const * element )
{
  std::map<std::string, std::string> attributes;
  for ( auto attribute = element->FirstAttribute(); attribute; attribute = attribute->Next() )
  {
    assert( attributes.find( attribute->Name() ) == attributes.end() );
    attributes[attribute->Name()] = attribute->Value();
  }
  return attributes;
}

template <typename ElementContainer>
std::vector<tinyxml2::XMLElement const *> getChildElements( ElementContainer const * element )
{
  std::vector<tinyxml2::XMLElement const *> childElements;
  for ( tinyxml2::XMLElement const * childElement = element->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement() )
  {
    childElements.push_back( childElement );
  }
  return childElements;
}

std::pair<std::vector<std::string>, std::string> readModifiers( tinyxml2::XMLNode const * node )
{
  std::vector<std::string> arraySizes;
  std::string              bitCount;
  if ( node && node->ToText() )
  {
    // following the name there might be some array size
    std::string value = node->Value();
    assert( !value.empty() );
    if ( value[0] == '[' )
    {
      std::string::size_type endPos = 0;
      while ( endPos + 1 != value.length() )
      {
        std::string::size_type startPos = value.find( '[', endPos );
        checkForError( startPos != std::string::npos, node->GetLineNum(), "could not find '[' in <" + value + ">" );
        endPos = value.find( ']', startPos );
        checkForError( endPos != std::string::npos, node->GetLineNum(), "could not find ']' in <" + value + ">" );
        checkForError( startPos + 2 <= endPos, node->GetLineNum(), "missing content between '[' and ']' in <" + value + ">" );
        arraySizes.push_back( value.substr( startPos + 1, endPos - startPos - 1 ) );
      }
    }
    else if ( value[0] == ':' )
    {
      bitCount = value.substr( 1 );
    }
    else
    {
      checkForError( ( value[0] == ';' ) || ( value[0] == ')' ), node->GetLineNum(), "unknown modifier <" + value + ">" );
    }
  }
  return std::make_pair( arraySizes, bitCount );
}

std::string readSnippet( std::string const & snippetFile )
{
  std::ifstream ifs( std::string( BASE_PATH ) + "/snippets/" + snippetFile );
  assert( !ifs.fail() );
  std::ostringstream oss;
  oss << ifs.rdbuf();
  return oss.str();
}

std::string replaceWithMap( std::string const & input, std::map<std::string, std::string> replacements )
{
  // This will match ${someVariable} and contain someVariable in match group 1
  std::regex re( R"(\$\{([^\}]+)\})" );
  auto       it  = std::sregex_iterator( input.begin(), input.end(), re );
  auto       end = std::sregex_iterator();

  // No match, just return the original string
  if ( it == end )
  {
    assert( replacements.empty() );
    return input;
  }

#if !defined( NDEBUG )
  std::set<std::string> matchedReplacements;
#endif

  std::string result = "";
  while ( it != end )
  {
    std::smatch match         = *it;
    auto        itReplacement = replacements.find( match[1].str() );
    assert( itReplacement != replacements.end() );
#if !defined( NDEBUG )
    matchedReplacements.insert( match[1].str() );
#endif

    result += match.prefix().str() + ( ( itReplacement != replacements.end() ) ? itReplacement->second : match[0].str() );
    ++it;

    // we've passed the last match. Append the rest of the orignal string
    if ( it == end )
    {
      result += match.suffix().str();
    }
  }
#if !defined( NDEBUG )
  std::set<std::string> missedReplacements;
  for ( auto r : replacements )
  {
    if ( matchedReplacements.find( r.first ) == matchedReplacements.end() )
    {
      missedReplacements.insert( r.first );
    }
  }
  assert( missedReplacements.empty() );
#endif
  return result;
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

std::string stripPostfix( std::string const & value, std::string const & postfix )
{
  std::string strippedValue = value;
  if ( strippedValue.ends_with( postfix ) )
  {
    strippedValue.erase( strippedValue.length() - postfix.length() );
  }
  return strippedValue;
}

std::string stripPluralS( std::string const & name, std::set<std::string> const & tags )
{
  std::string strippedName = name;
  std::string tag          = findTag( tags, name );
  if ( strippedName.ends_with( "s" + tag ) )
  {
    size_t pos = strippedName.rfind( 's' );
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

std::string stripPrefix( std::string const & value, std::string const & prefix )
{
  std::string strippedValue = value;
  if ( strippedValue.starts_with( prefix ) )
  {
    strippedValue.erase( 0, prefix.length() );
  }
  return strippedValue;
}

std::string toCamelCase( std::string const & value )
{
  assert( !value.empty() && ( isupper( value[0] ) || isdigit( value[0] ) ) );
  std::string result;
  result.reserve( value.size() );
  bool keepUpper = true;
  for ( auto c : value )
  {
    if ( c == '_' )
    {
      keepUpper = true;
    }
    else if ( isdigit( c ) )
    {
      keepUpper = true;
      result.push_back( c );
    }
    else if ( keepUpper )
    {
      result.push_back( c );
      keepUpper = false;
    }
    else
    {
      result.push_back( static_cast<char>( tolower( c ) ) );
    }
  }
  return result;
}

std::string toUpperCase( std::string const & name )
{
  std::string convertedName;
  bool        previousIsLowerCase = false;
  bool        previousIsDigit     = false;
  for ( auto c : name )
  {
    if ( ( isupper( c ) && ( previousIsLowerCase || previousIsDigit ) ) || ( isdigit( c ) && previousIsLowerCase ) )
    {
      convertedName.push_back( '_' );
    }
    convertedName.push_back( static_cast<char>( toupper( c ) ) );
    previousIsLowerCase = !!islower( c );
    previousIsDigit     = !!isdigit( c );
  }
  return convertedName;
}

std::vector<std::string> tokenize( std::string const & tokenString, std::string const & separator )
{
  std::vector<std::string> tokens;
  if ( !tokenString.empty() )
  {
    size_t start = 0, end;
    do
    {
      end = tokenString.find( separator, start );
      if ( start != end )
      {
        tokens.push_back( trim( tokenString.substr( start, end - start ) ) );
      }
      start = end + separator.length();
    } while ( end != std::string::npos );
  }
  return tokens;
}

std::vector<std::string> tokenizeAny( std::string const & tokenString, std::string const & separators )
{
  std::vector<std::string> tokens;
  if ( !tokenString.empty() )
  {
    size_t start = 0, end;
    do
    {
      end = tokenString.find_first_of( separators, start );
      if ( start != end )
      {
        tokens.push_back( trim( tokenString.substr( start, end - start ) ) );
      }
      start = end + 1;
    } while ( end != std::string::npos );
  }
  return tokens;
}

std::string trim( std::string const & input )
{
  std::string result = input;
  result.erase( result.begin(), std::find_if( result.begin(), result.end(), []( char c ) { return !std::isspace( c ); } ) );
  result.erase( std::find_if( result.rbegin(), result.rend(), []( char c ) { return !std::isspace( c ); } ).base(), result.end() );
  return result;
}

std::string trimEnd( std::string const & input )
{
  std::string result = input;
  result.erase( std::find_if( result.rbegin(), result.rend(), []( char c ) { return !std::isspace( c ); } ).base(), result.end() );
  return result;
}

std::string trimStars( std::string const & input )
{
  std::string result = input;
  size_t      pos    = result.find( '*' );
  while ( pos != std::string::npos )
  {
    if ( ( 0 < pos ) && ( result[pos - 1] != ' ' ) && ( result[pos - 1] != '*' ) )
    {
      result.insert( pos, 1, ' ' );
      ++pos;
    }
    else if ( ( pos < result.length() - 1 ) && ( result[pos + 1] != ' ' ) && ( result[pos + 1] != '*' ) )
    {
      result.insert( pos + 1, 1, ' ' );
    }
    pos = result.find( '*', pos + 1 );
  }
  return result;
}

void writeToFile( std::string const & str, std::string const & fileName )
{
  std::ofstream ofs( fileName );
  assert( !ofs.fail() );
  ofs << str;
  ofs.close();

#if defined( CLANG_FORMAT_EXECUTABLE )
  std::cout << "VulkanHppGenerator: Formatting " << fileName << " ..." << std::endl;
  std::string commandString = "\"" CLANG_FORMAT_EXECUTABLE "\" -i --style=file " + fileName;
  int         ret           = std::system( commandString.c_str() );
  if ( ret != 0 )
  {
    std::cout << "VulkanHppGenerator: failed to format file " << fileName << " with error <" << ret << ">\n";
  }
#endif
}

std::string toString( tinyxml2::XMLError error )
{
  switch ( error )
  {
    case tinyxml2::XML_SUCCESS: return "XML_SUCCESS";
    case tinyxml2::XML_NO_ATTRIBUTE: return "XML_NO_ATTRIBUTE";
    case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE: return "XML_WRONG_ATTRIBUTE_TYPE";
    case tinyxml2::XML_ERROR_FILE_NOT_FOUND: return "XML_ERROR_FILE_NOT_FOUND";
    case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED: return "XML_ERROR_FILE_COULD_NOT_BE_OPENED";
    case tinyxml2::XML_ERROR_FILE_READ_ERROR: return "XML_ERROR_FILE_READ_ERROR";
    case tinyxml2::XML_ERROR_PARSING_ELEMENT: return "XML_ERROR_PARSING_ELEMENT";
    case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE: return "XML_ERROR_PARSING_ATTRIBUTE";
    case tinyxml2::XML_ERROR_PARSING_TEXT: return "XML_ERROR_PARSING_TEXT";
    case tinyxml2::XML_ERROR_PARSING_CDATA: return "XML_ERROR_PARSING_CDATA";
    case tinyxml2::XML_ERROR_PARSING_COMMENT: return "XML_ERROR_PARSING_COMMENT";
    case tinyxml2::XML_ERROR_PARSING_DECLARATION: return "XML_ERROR_PARSING_DECLARATION";
    case tinyxml2::XML_ERROR_PARSING_UNKNOWN: return "XML_ERROR_PARSING_UNKNOWN";
    case tinyxml2::XML_ERROR_EMPTY_DOCUMENT: return "XML_ERROR_EMPTY_DOCUMENT";
    case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT: return "XML_ERROR_MISMATCHED_ELEMENT";
    case tinyxml2::XML_ERROR_PARSING: return "XML_ERROR_PARSING";
    case tinyxml2::XML_CAN_NOT_CONVERT_TEXT: return "XML_CAN_NOT_CONVERT_TEXT";
    case tinyxml2::XML_NO_TEXT_NODE: return "XML_NO_TEXT_NODE";
    default: return "unknown error code <" + std::to_string( error ) + ">";
  }
}

int main( int argc, char ** argv )
{
  try
  {
    tinyxml2::XMLDocument doc;

    std::string filename = ( argc == 1 ) ? VK_SPEC : argv[1];

#if defined( CLANG_FORMAT_EXECUTABLE )
    std::cout << "VulkanHppGenerator: Found ";
    std::string commandString = "\"" CLANG_FORMAT_EXECUTABLE "\" --version ";
    int         ret           = std::system( commandString.c_str() );
    if ( ret != 0 )
    {
      std::cout << "VulkanHppGenerator: failed to determine clang_format version with error <" << ret << ">\n";
    }
#endif

    std::cout << "VulkanHppGenerator: Loading " << filename << std::endl;
    tinyxml2::XMLError error = doc.LoadFile( filename.c_str() );
    if ( error != tinyxml2::XML_SUCCESS )
    {
      std::cout << "VulkanHppGenerator: failed to load file " << filename << " with error <" << toString( error ) << ">" << std::endl;
      return -1;
    }

    std::cout << "VulkanHppGenerator: Parsing " << filename << std::endl;
    VulkanHppGenerator generator( doc );

    generator.generateVulkanHppFile();
    generator.generateVulkanEnumsHppFile();
    generator.generateVulkanFormatTraitsHppFile();
    generator.generateVulkanFuncsHppFile();
    generator.generateVulkanHandlesHppFile();
    generator.generateVulkanHashHppFile();
    generator.prepareRAIIHandles();
    generator.generateVulkanRAIIHppFile();
    generator.generateVulkanStaticAssertionsHppFile();
    generator.generateVulkanStructsHppFile();
    generator.generateVulkanToStringHppFile();

#if !defined( CLANG_FORMAT_EXECUTABLE )
    std::cout << "VulkanHppGenerator: could not find clang-format. The generated files will not be formatted accordingly.\n";
#endif
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
