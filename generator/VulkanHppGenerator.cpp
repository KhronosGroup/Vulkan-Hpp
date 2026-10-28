// SPDX-FileCopyrightText: 2015-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

#include "VulkanHppGenerator.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <future>
#include <numeric>
#include <regex>

using namespace std::literals;

namespace
{
  size_t determineParamIndex( std::vector<Param> const & params, std::string const & name );
  template <typename T>
  typename std::map<std::string, T>::iterator findByNameOrAlias( std::map<std::string, T> & values, std::string const & name );
  template <typename T>
  typename std::vector<T>::const_iterator findByType( std::vector<T> const & values, std::string const & type );
  std::string                             generateCArraySizes( std::vector<std::string> const & sizes );
  std::string                             generateList( std::vector<std::string> const & elements, std::string const & prefix, std::string const & separator );
  std::string                             generateNoDiscard( bool returnsSomething, bool multiSuccessCodes, bool multiErrorCodes );
  std::string                             generateStandardArray( std::string const & type, std::vector<std::string> const & sizes );
  bool                                    isApiSupported( std::vector<std::string> const & apis, std::string const & api );
  bool                                    isOptional( std::vector<bool> const & optionals );
  bool                                    isUpperCase( std::string const & name );
  std::string                             startLowerCase( std::string const & input );
  std::string                             startUpperCase( std::string const & input );
}  // namespace

// Some types come in as non-const pointers, but are meant as const-pointers
static std::set<std::string> const specialPointerTypes = { "Display",          "IDirectFB",      "wl_display",
                                                           "xcb_connection_t", "_screen_window", "VkExportMetalObjectsInfoEXT" };

// Some structs hold multiple arrays with the same len attribute, but they are mutually exclusive.
// That information is not available in vk.xml, so we need to hardcode it here.
// We list the structs with mutually exclusive arrays and with parallel arrays separately, so that we can check for errors in the vk.xml.
static std::set<std::string> const structsWithMutualExclusiveArrays = { "VkAccelerationStructureBuildGeometryInfoKHR",
                                                                        "VkAccelerationStructureGeometryMicromapDataKHR",
                                                                        "VkAccelerationStructureTrianglesOpacityMicromapEXT",
                                                                        "VkMicromapBuildInfoEXT",
                                                                        "VkWriteDescriptorSet" };
static std::set<std::string> const structsWithParallelArrays        = { "VkAccelerationStructureTrianglesDisplacementMicromapNV",
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
                                                                        "VkSwapchainTimeDomainPropertiesEXT",
                                                                        "VkTensorCopyARM",
                                                                        "VkTensorDescriptionARM",
                                                                        "VkVideoDecodeAV1PictureInfoKHR",
                                                                        "VkWin32KeyedMutexAcquireReleaseInfoKHR",
                                                                        "VkWin32KeyedMutexAcquireReleaseInfoNV" };

//
// VulkanHppGenerator public interface
//

VulkanHppGenerator::VulkanHppGenerator( Vkxml && vkxml, std::string const & api ) : m_api( api ), m_vkxml( std::move( vkxml ) )
{
  m_copyrightMessage = generateCopyrightMessage( m_vkxml.copyright.text );

  for ( auto const & baseType : m_vkxml.types.baseTypes )
  {
    checkForError( m_types.insert( { baseType.name, TypeData{ TypeCategory::BaseType, {}, baseType.xmlLine } } ).second,
                   baseType.xmlLine,
                   "basetype <" + baseType.name + "> already specified" );
  }
  for ( auto const & bitmask : m_vkxml.types.bitmasks )
  {
    checkForError( m_types.insert( { bitmask.name, TypeData{ TypeCategory::Bitmask, {}, bitmask.xmlLine } } ).second,
                   bitmask.xmlLine,
                   "bitmask <" + bitmask.name + "> already specified" );
    std::ranges::for_each( bitmask.aliases,
                           [&]( auto const & alias )
                           {
                             checkForError( m_types.insert( { alias.first, TypeData{ TypeCategory::Bitmask, {}, alias.second } } ).second,
                                            alias.second,
                                            "alias <" + alias.first + "> of bitmask <" + bitmask.name + "> already specified" );
                           } );
  }
  for ( auto const & constant : m_vkxml.constants.values )
  {
    checkForError( m_types.insert( { constant.name, TypeData{ TypeCategory::Constant, {}, constant.xmlLine } } ).second,
                   constant.xmlLine,
                   "constant type <" + constant.name + "> already specified" );
  }
  for ( auto const & define : m_vkxml.types.defines )
  {
    checkForError( m_types.insert( { define.name, TypeData{ TypeCategory::Define, {}, define.xmlLine } } ).second,
                   define.xmlLine,
                   "type <" + define.name + "> already specified" );
  }
  for ( auto const & xmlEnum : m_vkxml.types.enums )
  {
    checkForError( m_types.insert( { xmlEnum.name, TypeData{ TypeCategory::Enum, {}, xmlEnum.xmlLine } } ).second,
                   xmlEnum.xmlLine,
                   "type <" + xmlEnum.name + "> already specified" );
    std::ranges::for_each( xmlEnum.aliases,
                           [&]( auto const & alias )
                           {
                             checkForError( m_types.insert( { alias.first, TypeData{ TypeCategory::Enum, {}, alias.second } } ).second,
                                            alias.second,
                                            "alias <" + alias.first + "> of handle <" + xmlEnum.name + "> already specified" );
                           } );

    // for enums, we compile some more data than is read from the vk.xml, so we insert the enums from the vk.xml into our own map
    auto [enumIt, inserted] = m_enums.insert( { xmlEnum.name, {} } );
    assert( inserted );
    enumIt->second.aliases   = xmlEnum.aliases;
    enumIt->second.bitwidth  = xmlEnum.bitwidth;
    enumIt->second.isBitmask = xmlEnum.category == "bitmask";
    enumIt->second.xmlLine   = xmlEnum.xmlLine;

    for ( auto const & enumValue : xmlEnum.values )
    {
      bool supported = true;
      if ( !enumValue.requiredBy.empty() )
      {
        auto featureIt = findByName( m_vkxml.features, enumValue.requiredBy );
        assert( featureIt != m_vkxml.features.end() );
        supported = isApiSupported( featureIt->api, m_api );
      }

      EnumValueData enumValueData;
      for ( auto const & alias : enumValue.aliases )
      {
        enumValueData.aliases.push_back( { enumValue.name, alias.name, "", true, alias.xmlLine } );
      }
      enumValueData.bitpos    = enumValue.bitPos;
      enumValueData.name      = enumValue.name;
      enumValueData.supported = supported;
      enumValueData.value     = enumValue.value;
      enumValueData.xmlLine   = enumValue.xmlLine;
      enumIt->second.values.push_back( std::move( enumValueData ) );
    }
  }
  for ( auto const & external : m_vkxml.types.externals )
  {
    checkForError( m_types.insert( { external.name, TypeData{ TypeCategory::External, {}, external.xmlLine } } ).second,
                   external.xmlLine,
                   "type <" + external.name + "> already specified" );
  }
  for ( auto const & funcPointer : m_vkxml.types.funcPointers )
  {
    checkForError( m_types.insert( { funcPointer.name, TypeData{ TypeCategory::FuncPointer, {}, funcPointer.xmlLine } } ).second,
                   funcPointer.xmlLine,
                   "funcpointer <" + funcPointer.name + "> already specified" );
  }
  for ( auto const & handle : m_vkxml.types.handles )
  {
    checkForError( m_types.insert( { handle.name, TypeData{ TypeCategory::Handle, {}, handle.xmlLine } } ).second,
                   handle.xmlLine,
                   "type <" + handle.name + "> already specified" );
    std::ranges::for_each( handle.aliases,
                           [&]( auto const & alias )
                           {
                             checkForError( m_types.insert( { alias.first, TypeData{ TypeCategory::Handle, {}, alias.second } } ).second,
                                            alias.second,
                                            "alias <" + alias.first + "> of handle <" + handle.name + "> already specified" );
                           } );

    // for handles, we compile some more data than is read from the vk.xml
    auto [handleIt, inserted] = m_handles.insert( { handle.name, {} } );
    assert( inserted );
    handleIt->second.isDispatchable = ( handle.type.name == "VK_DEFINE_HANDLE" );
    handleIt->second.xmlLine        = handle.xmlLine;
  }
  for ( auto const & include : m_vkxml.types.includes )
  {
    checkForError( m_types.insert( { include.name, TypeData{ TypeCategory::Include, {}, include.xmlLine } } ).second,
                   include.xmlLine,
                   "include <" + include.name + "> already specified" );
  }
  for ( auto const & structure : m_vkxml.types.structs )
  {
    checkForError( m_types.insert( { structure.name, TypeData{ TypeCategory::Struct, {}, structure.xmlLine } } ).second,
                   structure.xmlLine,
                   "struct <" + structure.name + "> already specified" );
    std::ranges::for_each( structure.aliases,
                           [&]( auto const & alias )
                           {
                             checkForError( m_types.insert( { alias.first, TypeData{ TypeCategory::Struct, {}, alias.second } } ).second,
                                            alias.second,
                                            "alias <" + alias.first + "> of struct <" + structure.name + "> already specified" );
                           } );

    // check if multiple structure members use the very same (not empty) len attribute
    // We maintain a list of known structs with mutually exclusive arrays and with parallel arrays; any struct with multiple arrays using the same len attribute
    // that is not listed there needs our attention!
    for ( auto m0It = structure.members.begin(); m0It != structure.members.end(); ++m0It )
    {
      if ( !m0It->len.empty() && ( m0It->len.front() != "null-terminated" ) )
      {
        for ( auto m1It = std::next( m0It ); m1It != structure.members.end(); ++m1It )
        {
          if ( !m1It->len.empty() && ( m0It->len.front() == m1It->len.front() ) )
          {
            checkForWarning(
              structsWithMutualExclusiveArrays.contains( structure.name ) || structsWithParallelArrays.contains( structure.name ),
              structure.xmlLine,
              "Encountered structure <" + structure.name +
                "> with multiple members referencing the same member for len. Need to be checked if they are supposed to be mutually exclusive." );
          }
        }
      }
    }
  }
  for ( auto const & u : m_vkxml.types.unions )
  {
    checkForError( m_types.insert( { u.name, TypeData{ TypeCategory::Union, {}, u.xmlLine } } ).second, u.xmlLine, "union <" + u.name + "> already specified" );
    std::ranges::for_each( u.aliases,
                           [&]( auto const & alias )
                           {
                             checkForError( m_types.insert( { alias.first, TypeData{ TypeCategory::Union, {}, alias.second } } ).second,
                                            alias.second,
                                            "alias <" + alias.first + "> of union <" + u.name + "> already specified" );
                           } );
  }
  for ( auto featureIt = m_vkxml.features.begin(); featureIt != m_vkxml.features.end(); )
  {
    if ( isApiSupported( featureIt->api, m_api ) )
    {
      ++featureIt;
    }
    else
    {
      m_unsupportedFeatures.push_back( std::move( *featureIt ) );
      featureIt = m_vkxml.features.erase( featureIt );
    }
  }
  for ( auto const & extension : m_vkxml.extensions.extensions )
  {
    bool const extensionSupported = isApiSupported( extension.supported, m_api );

    ExtensionData extensionData{ .deprecatedBy = extension.deprecatedBy,
                                 .isDeprecated = extension.isDeprecated,
                                 .name         = extension.name,
                                 .number       = extension.number,
                                 .obsoletedBy  = extension.obsoletedBy,
                                 .platform     = extension.platform,
                                 .promotedTo   = extension.promotedTo,
                                 .depends      = extension.depends,
                                 .ratified     = extension.ratified,
                                 .supported    = extension.supported,
                                 .type         = extension.type,
                                 .xmlLine      = extension.xmlLine };
    for ( auto const & deprecate : extension.deprecates )
    {
      DeprecateData deprecateData{ .explanationLink = deprecate.explanationLink, .xmlLine = deprecate.xmlLine };
      for ( auto const & command : deprecate.commands )
      {
        deprecateData.commands.push_back( { command.name, command.supersededBy } );
      }
      for ( auto const & feature : deprecate.features )
      {
        deprecateData.features.push_back( { feature.name, feature.structure } );
      }
      for ( auto const & type : deprecate.types )
      {
        deprecateData.types.push_back( { type.name, type.supersededBy } );
      }
      extensionData.deprecateData.push_back( std::move( deprecateData ) );
    }
    for ( auto const & remove : extension.removes )
    {
      RemoveData removeData{ .xmlLine = remove.xmlLine };
      removeData.features.push_back( { .name = { remove.feature.name }, .xmlLine = remove.feature.xmlLine } );
      extensionData.removeData.push_back( std::move( removeData ) );
    }
    for ( auto const & require : extension.require )
    {
      RequireData requireData{ .api = require.api, .depends = require.depends, .xmlLine = require.xmlLine };

      bool const requireSupported = require.api.empty() || ( require.api == m_api );

      for ( auto const & command : require.commands )
      {
        requireData.commands.push_back( { .name = command.name, .xmlLine = command.xmlLine } );
      }
      for ( auto const & e : require.enums )
      {
        if ( std::holds_alternative<ExtensionRequireEnumAliasVariant>( e ) )
        {
          auto const & aliasVariant = std::get<ExtensionRequireEnumAliasVariant>( e );
          if ( std::holds_alternative<ExtensionRequireEnumAlias>( aliasVariant ) )
          {
            auto const & requireEnumAlias = std::get<ExtensionRequireEnumAlias>( aliasVariant );
            auto const & typeIt           = m_types.find( requireEnumAlias.extends );
            checkForError( typeIt != m_types.end(),
                           requireEnumAlias.xmlLine,
                           "enum alias <" + requireEnumAlias.name + "> extends unknown type <" + requireEnumAlias.extends + ">" );
            checkForError( typeIt->second.category == TypeCategory::Enum,
                           requireEnumAlias.xmlLine,
                           "enum alias <" + requireEnumAlias.name + "> extends non-enum type <" + requireEnumAlias.extends + ">" );
            auto enumIt = findByNameOrAlias( m_enums, requireEnumAlias.extends );
            assert( enumIt != m_enums.end() );
            std::string protect = requireEnumAlias.protect.empty() ? getProtectFromPlatform( extensionData.platform ) : requireEnumAlias.protect;
            checkForError( enumIt->second.addEnumAlias(
                             requireEnumAlias.xmlLine, requireEnumAlias.name, requireEnumAlias.alias, protect, extensionSupported && requireSupported ),
                           requireEnumAlias.xmlLine,
                           "enum value alias <" + requireEnumAlias.name + "> already listed with different properties" );
          }
          else
          {
            // enum aliases that don't extend anything are listed as constants
            assert( std::holds_alternative<Alias>( aliasVariant ) );
            auto const & alias  = std::get<Alias>( aliasVariant );
            auto         typeIt = m_types.find( alias.alias );
            checkForError( typeIt != m_types.end(), alias.xmlLine, "enum alias <" + alias.name + "> is an alias of an unknown type <" + alias.alias + ">" );
            checkForError( typeIt->second.category == TypeCategory::Constant,
                           alias.xmlLine,
                           "constant alias <" + alias.name + "> is an alias of a non-constant type <" + alias.alias + ">" );
            checkForError( m_types.insert( { alias.name, TypeData{ TypeCategory::Constant, { extensionData.name }, alias.xmlLine } } ).second,
                           alias.xmlLine,
                           "constant alias <" + alias.name + "> already specified" );
            checkForError( !containsByName( m_vkxml.constants.values, alias.name ),
                           alias.xmlLine,
                           "constant alias <" + alias.name + "> already specified as a constant in the vk.xml" );
            auto constIt = findByName( m_vkxml.constants.values, alias.alias );
            if ( constIt != m_vkxml.constants.values.end() )
            {
              m_vkxml.constants.values.push_back( { .name = alias.name, .type = constIt->type, .value = constIt->value, .xmlLine = alias.xmlLine } );
              requireData.constants.push_back( { alias.name, alias.xmlLine } );
            }
          }
        }
        else if ( std::holds_alternative<ExtensionRequireEnumExtendVariant>( e ) )
        {
          auto const &        extendVariant = std::get<ExtensionRequireEnumExtendVariant>( e );
          std::string const & extends       = std::visit( []( auto const & val ) { return val.extends; }, extendVariant );
          std::string const & name          = std::visit( []( auto const & val ) { return val.name; }, extendVariant );
          int                 xmlLine       = std::visit( []( auto const & val ) { return val.xmlLine; }, extendVariant );
          auto                typeIt        = m_types.find( extends );
          checkForError( typeIt != m_types.end(), xmlLine, "enum value <" + name + "> extends unknown type <" + extends + ">" );
          checkForError( typeIt->second.category == TypeCategory::Enum, xmlLine, "enum value <" + name + "> extends non-enum type <" + extends + ">" );
          auto enumIt = findByNameOrAlias( m_enums, extends );
          assert( enumIt != m_enums.end() );

          if ( std::holds_alternative<ExtensionRequireEnumExtendByBitPos>( extendVariant ) )
          {
            auto const & extendByBitPos = std::get<ExtensionRequireEnumExtendByBitPos>( extendVariant );
            std::string  protect        = extendByBitPos.protect.empty() ? getProtectFromPlatform( extensionData.platform ) : extendByBitPos.protect;
            checkForError( enumIt->second.addEnumValue( xmlLine, name, protect, extendByBitPos.bitPos, "", extensionSupported && requireSupported, false ),
                           xmlLine,
                           "enum value <" + name + "> already listed with different properties" );
          }
          else if ( std::holds_alternative<ExtensionRequireEnumExtendByOffset>( extendVariant ) )
          {
            auto const & extendByOffset = std::get<ExtensionRequireEnumExtendByOffset>( extendVariant );

            std::string protect = extendByOffset.protect.empty() ? getProtectFromPlatform( extensionData.platform ) : extendByOffset.protect;
            checkForError( enumIt->second.addEnumValue(
                             xmlLine, name, protect, extendByOffset.offset, "", extensionSupported && requireSupported, extendByOffset.deprecated == "true" ),
                           xmlLine,
                           "enum value <" + name + "> already listed with different properties" );
          }
          else
          {
            assert( std::holds_alternative<ExtensionRequireEnumExtendByValue>( extendVariant ) );
            auto const & extendByValue = std::get<ExtensionRequireEnumExtendByValue>( extendVariant );

            checkForError(
              enumIt->second.addEnumValue(
                xmlLine, name, getProtectFromPlatform( extensionData.platform ), "", extendByValue.value, extensionSupported && requireSupported, false ),
              xmlLine,
              "enum value <" + name + "> already listed with different properties" );
          }
        }
        else
        {
          assert( std::holds_alternative<ExtensionRequireEnumConstantVariant>( e ) );
          auto const & constantVariant = std::get<ExtensionRequireEnumConstantVariant>( e );
          if ( std::holds_alternative<ExtensionRequireEnumConstant>( constantVariant ) )
          {
            auto const & enumConstant = std::get<ExtensionRequireEnumConstant>( constantVariant );
            checkForError( m_types.insert( { enumConstant.name, TypeData{ TypeCategory::Constant, { extensionData.name }, enumConstant.xmlLine } } ).second,
                           enumConstant.xmlLine,
                           "required constant <" + enumConstant.name + "> specified by value <" + enumConstant.value + "> is already specified" );
            requireData.enumConstants.push_back( { enumConstant.name, enumConstant.value, enumConstant.xmlLine } );
          }
          else
          {
            assert( std::holds_alternative<NameElement>( constantVariant ) );
            auto const & nameElement = std::get<NameElement>( constantVariant );
            auto         typeIt      = m_types.find( nameElement.name );
            checkForError( typeIt != m_types.end(), nameElement.xmlLine, "unknown required enum <" + nameElement.name + ">" );
            requireData.constants.push_back( { nameElement.name, nameElement.xmlLine } );
          }
        }
      }
      for ( auto const & feature : require.features )
      {
        requireData.features.push_back( { .name = feature.names, .structure = feature.structure, .xmlLine = feature.xmlLine } );
      }
      for ( auto const & type : require.types )
      {
        auto typeIt = m_types.find( type.name );
        checkForError( typeIt != m_types.end(), type.xmlLine, "required type <" + type.name + "> is not listed as a type" );

        requireData.types.push_back( { .name = type.name, .xmlLine = type.xmlLine } );
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

    if ( extensionSupported )
    {
      m_extensions.push_back( std::move( extensionData ) );
    }
    else
    {
      checkForError( !containsByName( m_unsupportedExtensions, extensionData.name ),
                     extensionData.xmlLine,
                     "unsupported extension <" + extensionData.name + "> already specified" );
      m_unsupportedExtensions.push_back( std::move( extensionData ) );
    }
  }

  auto dynamicStateIt = m_enums.find( "VkDynamicState" );
  assert( dynamicStateIt != m_enums.end() );
  for ( auto const & dynamicState : m_vkxml.dynamicStates )
  {
    for ( auto const & dynamicStateCmd : dynamicState.commands )
    {
      checkForError( containsByName( dynamicStateIt->second.values, dynamicStateCmd.pipeline ),
                     dynamicStateCmd.xmlLine,
                     "Dynamic state <" + dynamicState.name + "> uses command <" + dynamicStateCmd.name + "> with unknown pipeline <" +
                       dynamicStateCmd.pipeline + ">" );
    }
  }
  for ( auto const & dynamicStateValue : dynamicStateIt->second.values )
  {
    checkForError( std::ranges::any_of( m_vkxml.dynamicStates,
                                        [&dynamicStateValue]( auto const & dynamicState )
                                        {
                                          return std::ranges::any_of( dynamicState.commands,
                                                                      [&dynamicStateValue]( auto const & command )
                                                                      { return command.pipeline == dynamicStateValue.name; } );
                                        } ),
                   dynamicStateValue.xmlLine,
                   "Enum <VkDynamicState> has value <" + dynamicStateValue.name + "> that is not used in the dynamicstates list" );
  }

  for ( auto const & format : m_vkxml.formats )
  {
    auto formatIt = m_enums.find( "VkFormat" );
    assert( formatIt != m_enums.end() );

    checkForError( containsByName( formatIt->second.values, format.name ), format.xmlLine, "format <" + format.name + "> not listed in enum VkFormat" );

    FormatData formatData{ .blockExtent      = concatenate( format.blockExtent ),
                           .blockSize        = format.blockSize,
                           .chroma           = format.chroma,
                           .classAttribute   = format.classAttribute,
                           .compressed       = format.compressed,
                           .packed           = format.packed,
                           .spirvImageFormat = format.spirvImageFormat.name,
                           .texelsPerBlock   = format.texelsPerBlock,
                           .xmlLine          = format.xmlLine };
    for ( auto const & component : format.components )
    {
      formatData.components.push_back( { .bits          = component.bits,
                                         .name          = component.name,
                                         .numericFormat = component.numericFormat,
                                         .planeIndex    = component.planeIndex,
                                         .xmlLine       = component.xmlLine } );
    }
    for ( auto const & plane : format.planes )
    {
      checkForError( containsByNameOrAlias( formatIt->second.values, plane.compatible ),
                     plane.xmlLine,
                     "format <" + format.name + "> contains unknown plane compatible format <" + plane.compatible + ">" );
      formatData.planes.push_back(
        { .compatible = plane.compatible, .heightDivisor = plane.heightDivisor, .widthDivisor = plane.widthDivisor, .xmlLine = plane.xmlLine } );
    }
    m_formats.insert( { format.name, std::move( formatData ) } );
  }

  auto versionIt = findByName( m_vkxml.types.defines, "VK_HEADER_VERSION" );
  assert( versionIt != m_vkxml.types.defines.end() );
  m_version = versionIt->possibleDefinition;

  // insert the default "handle" without class (for createInstance, and such)
  m_handles.insert( { "", {} } );

  if ( m_api != "vulkanbase" )
  {
    // for all api but vulkanbase, we merge the internal features into the normal ones
    mergeInternalFeatures();
  }
  distributeEnumValueAliases();
  distributeRequirements();
  checkCorrectness();
  handleRemovals();

  // add the commands to the respective handles
  for ( auto & feature : m_vkxml.features )
  {
    forEachRequiredCommand( feature.require, [this]( std::string const & name, auto const & command ) { addCommandToHandle( name, command ); } );
  }
  for ( auto & extension : m_extensions )
  {
    forEachRequiredCommand( extension.requireData, [this]( std::string const & name, auto const & command ) { addCommandToHandle( name, command ); } );
  }

  m_definesPartition = partitionDefines( m_vkxml.types.defines );
}

void VulkanHppGenerator::distributeSecondLevelCommands()
{
  // distribute commands from instance/device to second-level handles, like Queue, Event,... for RAII handles
  assert( !m_handles.empty() && m_handles.begin()->first.empty() );  // the first "handle" is the empty one
  for ( auto handleIt = std::next( m_handles.begin() ); handleIt != m_handles.end(); ++handleIt )
  {
    for ( auto command = handleIt->second.commands.begin(); command != handleIt->second.commands.end(); )
    {
      bool foundCommand = false;
      if ( !m_RAIISpecialFunctions.contains( *command ) )
      {
        auto commandIt = findByNameOrAlias( m_vkxml.commands, *command );
        assert( commandIt != m_vkxml.commands.end() );
        assert( commandIt->params.front().type.name == handleIt->first );
        if ( ( 1 < commandIt->params.size() ) && ( isHandleType( commandIt->params[1].type.name ) ) && !isOptional( commandIt->params[1].optional ) )
        {
          auto secondLevelHandleIt = m_handles.find( commandIt->params[1].type.name );
          assert( secondLevelHandleIt != m_handles.end() );
          // filter out functions that seem to fit due to taking handles as first and second argument, but the first argument is not the
          // type to create the second one, and so it's unknown to the raii handle!
          assert( !secondLevelHandleIt->second.constructorIndices.empty() );
          if ( m_vkxml.commands[secondLevelHandleIt->second.constructorIndices.front()].params[0].type.name == handleIt->first )
          {
            assert( std::ranges::none_of( secondLevelHandleIt->second.constructorIndices,
                                          [this, handleIt]( auto constructorIndex )
                                          { return m_vkxml.commands[constructorIndex].params[0].type.name != handleIt->first; } ) );
            secondLevelHandleIt->second.secondLevelCommands.insert( *command );
            command      = handleIt->second.commands.erase( command );
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

void VulkanHppGenerator::generateCppmFile() const
{
  generateFileFromTemplate( m_api + ".cppm",
                            "CppmTemplate.hpp",
                            { { "api", m_api },
                              { "copyrightMessage", m_copyrightMessage },
                              { "vulkan_h", ( m_api == "vulkansc" ) ? "vulkan_sc_core.h" : ( m_api + ".h" ) },
                              { "headerVersion", m_version },
                              { "pfnCommands", generateCppModuleCommands() } } );
}

void VulkanHppGenerator::generateEnumsHppFile() const
{
  generateFileFromTemplate( m_api + "_enums.hpp",
                            "EnumsHppTemplate.hpp",
                            { { "copyrightMessage", m_copyrightMessage },
                              { "enums", generateEnums() },
                              { "Flags", readSnippet( "Flags.hpp" ) },
                              { "objectTypeToDebugReportObjectType", generateObjectTypeToDebugReportObjectType() } } );
}

void VulkanHppGenerator::generateExtensionInspectionFile() const
{
  generateFileFromTemplate(
    m_api + "_extension_inspection.hpp",
    "ExtensionInspectionHppTemplate.hpp",
    { { "api", m_api },
      { "copyrightMessage", m_copyrightMessage },
      { "deprecatedExtensions",
        generateReplacedExtensionsList( []( ExtensionData const & extension ) { return extension.isDeprecated; },
                                        []( ExtensionData const & extension ) { return extension.deprecatedBy; } ) },
      { "deviceExtensions", generateExtensionsList( "device" ) },
      { "deviceTest", generateExtensionTypeTest( "device" ) },
      { "deprecatedBy",
        generateExtensionReplacedBy( []( ExtensionData const & extension ) { return extension.isDeprecated; },
                                     []( ExtensionData const & extension ) { return extension.deprecatedBy; } ) },
      { "deprecatedTest", generateExtensionReplacedTest( []( ExtensionData const & extension ) { return extension.isDeprecated; } ) },
      { "extensionDependencies", generateExtensionDependencies() },
      { "instanceExtensions", generateExtensionsList( "instance" ) },
      { "instanceTest", generateExtensionTypeTest( "instance" ) },
      { "obsoletedBy",
        generateExtensionReplacedBy( []( ExtensionData const & extension ) { return !extension.obsoletedBy.empty(); },
                                     []( ExtensionData const & extension ) { return extension.obsoletedBy; } ) },
      { "obsoletedExtensions",
        generateReplacedExtensionsList( []( ExtensionData const & extension ) { return !extension.obsoletedBy.empty(); },
                                        []( ExtensionData const & extension ) { return extension.obsoletedBy; } ) },
      { "obsoletedTest", generateExtensionReplacedTest( []( ExtensionData const & extension ) { return !extension.obsoletedBy.empty(); } ) },
      { "promotedExtensions",
        generateReplacedExtensionsList( []( ExtensionData const & extension ) { return !extension.promotedTo.empty(); },
                                        []( ExtensionData const & extension ) { return extension.promotedTo; } ) },
      { "promotedTest", generateExtensionReplacedTest( []( ExtensionData const & extension ) { return !extension.promotedTo.empty(); } ) },
      { "promotedTo",
        generateExtensionReplacedBy( []( ExtensionData const & extension ) { return !extension.promotedTo.empty(); },
                                     []( ExtensionData const & extension ) { return extension.promotedTo; } ) },
      { "versions",
        std::accumulate( std::next( m_vkxml.features.begin() ),
                         m_vkxml.features.end(),
                         "\"" + m_vkxml.features[0].name + "\"",
                         []( std::string const & acc, auto const & feature ) { return acc + ", \"" + feature.name + "\""; } ) },
      { "voidExtension", ( m_api == "vulkan" ) ? "" : "(void)extension;" } } );
}

void VulkanHppGenerator::generateFormatTraitsHppFile() const
{
  generateFileFromTemplate( m_api + "_format_traits.hpp",
                            "FormatTraitsHppTemplate.hpp",
                            { { "api", m_api }, { "copyrightMessage", m_copyrightMessage }, { "formatTraits", generateFormatTraits() } } );
}

void VulkanHppGenerator::generateFuncsHppFile() const
{
  generateFileFromTemplate(
    m_api + "_funcs.hpp", "FuncsHppTemplate.hpp", { { "commandDefinitions", generateCommandDefinitions() }, { "copyrightMessage", m_copyrightMessage } } );
}

void VulkanHppGenerator::generateHandlesHppFile() const
{
  generateFileFromTemplate( m_api + "_handles.hpp",
                            "HandlesHppTemplate.hpp",
                            { { "copyrightMessage", m_copyrightMessage }, { "handles", generateHandles() }, { "uniqueHandles", generateUniqueHandles() } } );
}

void VulkanHppGenerator::generateHashHppFile() const
{
  generateFileFromTemplate( m_api + "_hash.hpp",
                            "HashHppTemplate.hpp",
                            { { "api", m_api },
                              { "copyrightMessage", m_copyrightMessage },
                              { "handleHashStructures", generateHandleHashStructures() },
                              { "structHashStructures", generateStructHashStructures() } } );
}

void VulkanHppGenerator::generateHppFile() const
{
  generateFileFromTemplate(
    m_api + ".hpp",
    "HppTemplate.hpp",
    { { "api", m_api },
      { "ArrayProxy", readSnippet( "ArrayProxy.hpp" ) },
      { "ArrayProxyNoTemporaries", readSnippet( "ArrayProxyNoTemporaries.hpp" ) },
      { "ArrayWrapper1D", readSnippet( "ArrayWrapper1D.hpp" ) },
      { "ArrayWrapper2D", readSnippet( "ArrayWrapper2D.hpp" ) },
      { "baseTypes", generateBaseTypes() },
      { "constexprDefines", generateConstexprDefines() },
      { "copyrightMessage", m_copyrightMessage },
      { "defines", readSnippet( "defines.hpp" ) },
      { "DispatchLoaderBase", readSnippet( "DispatchLoaderBase.hpp" ) },
      { "DispatchLoaderDynamic", generateDispatchLoaderDynamic() },
      { "DispatchLoaderStatic", generateDispatchLoaderStatic() },
      { "DynamicLoader", readSnippet( "DynamicLoader.hpp" ) },
      { "Exceptions", readSnippet( "Exceptions.hpp" ) },
      { "Exchange", readSnippet( "Exchange.hpp" ) },
      { "funcPointers", generateFuncPointers() },
      { "handleForwardDeclarations", generateHandleForwardDeclarations() },
      { "headerVersion", m_version },
      { "includes", replaceWithMap( readSnippet( "includes.hpp" ), { { "vulkan_h", ( m_api == "vulkansc" ) ? "vulkan_sc_core.h" : ( m_api + ".h" ) } } ) },
      { "IsDispatchedList", generateIsDispatchedList() },
      { "ObjectDestroy", readSnippet( "ObjectDestroy.hpp" ) },
      { "ObjectFree", readSnippet( "ObjectFree.hpp" ) },
      { "ObjectRelease", readSnippet( "ObjectRelease.hpp" ) },
      { "Optional", readSnippet( "Optional.hpp" ) },
      { "PoolFree", readSnippet( "PoolFree.hpp" ) },
      { "resultChecks", readSnippet( "resultChecks.hpp" ) },
      { "resultExceptions", generateResultExceptions() },
      { "structExtendsStructs", generateStructExtendsStructs() },
      { "structForwardDeclarations", generateStructForwardDeclarations() },
      { "ResultValue", readSnippet( "ResultValue.hpp" ) },
      { "StridedArrayProxy", readSnippet( "StridedArrayProxy.hpp" ) },
      { "StructureChain", readSnippet( "StructureChain.hpp" ) },
      { "throwResultException", generateThrowResultException() },
      { "UniqueHandle", readSnippet( "UniqueHandle.hpp" ) } } );
}

void VulkanHppGenerator::generateMacrosFile() const
{
  generateFileFromTemplate( m_api + "_hpp_macros.hpp",
                            "MacrosHppTemplate.hpp",
                            { { "copyrightMessage", m_copyrightMessage },
                              { "vulkan_hpp", m_api + ".hpp" },
                              { "vulkan_64_bit_ptr_defines", findByName( m_vkxml.types.defines, "VK_USE_64_BIT_PTR_DEFINES" )->possibleDefinition } } );
}

void VulkanHppGenerator::generateRAIIHppFile() const
{
  generateFileFromTemplate( m_api + "_raii.hpp",
                            "RAIIHppTemplate.hpp",
                            { { "api", m_api },
                              { "copyrightMessage", m_copyrightMessage },
                              { "RAIICommandDefinitions", generateRAIICommandDefinitions() },
                              { "RAIIDispatchers", generateRAIIDispatchers() },
                              { "RAIIHandles", generateRAIIHandles() } } );
}

void VulkanHppGenerator::generateSharedHppFile() const
{
  generateFileFromTemplate( m_api + "_shared.hpp",
                            "SharedHppTemplate.hpp",
                            { { "api", m_api },
                              { "copyrightMessage", m_copyrightMessage },
                              { "sharedHandles", generateSharedHandles() },
                              { "sharedHandlesNoDestroy", generateSharedHandlesNoDestroy() } } );
}

void VulkanHppGenerator::generateStaticAssertionsHppFile() const
{
  generateFileFromTemplate( m_api + "_static_assertions.hpp",
                            "StaticAssertionsHppTemplate.hpp",
                            { { "api", m_api }, { "copyrightMessage", m_copyrightMessage }, { "staticAssertions", generateStaticAssertions() } } );
}

void VulkanHppGenerator::generateStructsHppFile() const
{
  generateFileFromTemplate(
    m_api + "_structs.hpp", "StructsHppTemplate.hpp", { { "copyrightMessage", m_copyrightMessage }, { "structs", generateStructsAndUnions() } } );
}

void VulkanHppGenerator::generateToStringHppFile() const
{
  generateFileFromTemplate( m_api + "_to_string.hpp",
                            "ToStringHppTemplate.hpp",
                            { { "api", m_api },
                              { "bitmasksToString", generateBitmasksToString() },
                              { "copyrightMessage", m_copyrightMessage },
                              { "enumsToString", generateEnumsToString() } } );
}

void VulkanHppGenerator::prepareRAIIHandles()
{
  // filter out functions that are not usefull on this level of abstraction (like vkGetInstanceProcAddr)
  // and all the destruction functions, as they are used differently
  assert( m_handles.begin()->first.empty() );
  for ( auto handleIt = std::next( m_handles.begin() ); handleIt != m_handles.end(); ++handleIt )
  {
    handleIt->second.destructorIndex = determineRAIIHandleDestructor( handleIt->first );
    if ( handleIt->second.destructorIndex != INVALID_INDEX )
    {
      m_RAIISpecialFunctions.insert( m_vkxml.commands[handleIt->second.destructorIndex].name );
    }
    handleIt->second.constructorIndices = determineRAIIHandleConstructors( handleIt->first, handleIt->second.destructorIndex );
  }
}

//
// VulkanHppGenerator private interface
//

void VulkanHppGenerator::addCommandToHandle( std::string const & name, Command const & command )
{
  auto handleIt = m_handles.find( isHandleType( command.params[0].type.name ) ? command.params[0].type.name : "" );
  assert( handleIt != m_handles.end() );
  if ( !handleIt->second.commands.contains( name ) )
  {
    handleIt->second.commands.insert( name );
    registerDeleter( command );
  }
}

std::string VulkanHppGenerator::addTitleAndProtection( std::string const & title, std::string const & strIf, std::string const & strElse ) const
{
  std::string str;
  if ( !strIf.empty() )
  {
    auto const [enter, leave] = generateProtection( getProtectFromTitle( title ) );
    str                       = "\n" + enter + "  //=== " + title + " ===\n" + strIf;
    if ( !enter.empty() && !strElse.empty() )
    {
      str += "#else \n" + strElse;
    }
    str += leave;
  }
  return str;
}

template <typename RequireType>
void VulkanHppGenerator::appendCppModuleCommands( std::vector<RequireType> const & requireTypes,
                                                  std::set<std::string> &          listedCommands,
                                                  std::string const &              title,
                                                  std::string &                    commandMembers ) const
{
  std::string members;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & )
                          {
                            if ( listedCommands.insert( name ).second )
                            {
                              members += "using ::PFN_" + name + ";\n";
                            }
                          } );
  commandMembers += addTitleAndProtection( title, members );
}

void VulkanHppGenerator::checkBitmaskCorrectness() const
{
  for ( auto const & bitmask : m_vkxml.types.bitmasks )
  {
    // check that a bitmask is required somewhere
    // I think, it's not forbidden to not reference a bitmask, but it would probably be not intended?
    auto typeIt = m_types.find( bitmask.name );
    assert( typeIt != m_types.end() );
    checkForError( !typeIt->second.requiredBy.empty(), bitmask.xmlLine, "bitmask <" + bitmask.name + "> not required in any feature or extension" );

    // check that the requirement is an enum
    auto requireTypeIt = m_types.find( bitmask.require );
    checkForError( requireTypeIt != m_types.end(), bitmask.xmlLine, "bitmask <" + bitmask.name + "> requires unknown type <" + bitmask.require + ">" );
    checkForError( requireTypeIt->second.category == TypeCategory::Enum,
                   bitmask.xmlLine,
                   "bitmask <" + bitmask.name + "> requires non-enum type <" + bitmask.require + ">" );
    assert( m_enums.contains( bitmask.require ) );
    checkForError( !requireTypeIt->second.requiredBy.empty(),
                   bitmask.xmlLine,
                   "bitmask <" + bitmask.name + "> requires <" + bitmask.require + "> which is not required by any feature or extension!" );
  }
}

void VulkanHppGenerator::checkCommandCorrectness() const
{
  // prepare command checks by gathering all result codes (including aliases and not supported ones!) into one set of resultCodes
  std::set<std::string> resultCodes = gatherResultCodes();

  // command checks
  for ( auto const & command : m_vkxml.commands )
  {
    // check that a command is referenced somewhere
    // I think, it's not forbidden to not reference a function, but it would probably be not intended?
    checkForError( !isApiSupported( command.api, m_api ) || !command.requiredBy.empty(),
                   command.xmlLine,
                   "command <" + command.name + "> not required in any feature or extension" );

    // check for unknown error or succes codes
    for ( auto const & ec : command.errorCodes )
    {
      checkForError( resultCodes.contains( ec ), command.xmlLine, "command uses unknown error code <" + ec + ">" );
    }
    for ( auto const & sc : command.successCodes )
    {
      checkForError( resultCodes.contains( sc ), command.xmlLine, "command uses unknown success code <" + sc + ">" );
    }

    // check that functions returning a VkResult specify successcodes
    if ( ( command.returnType.name == "VkResult" ) && command.successCodes.empty() )
    {
      // emit an error if this function is required in at least one supported feature or extension
      // disabled or not supported features/extensions are still listed in requiredBy, but not in m_features/m_extensions
      bool functionUsed = false;
      for ( auto const & require : command.requiredBy )
      {
        functionUsed |= containsByName( m_vkxml.features, require ) || containsByName( m_extensions, require );
      }
      if ( functionUsed )
      {
        checkForError( false, command.xmlLine, "missing successcodes on command <" + command.name + "> returning VkResult!" );
      }
    }

    // check that all parameter types as well as the return type are known types
    for ( auto const & p : command.params )
    {
      checkForError( m_types.contains( p.type.name ), p.xmlLine, "comand uses parameter of unknown type <" + p.type.name + ">" );
    }
    checkForError( m_types.contains( command.returnType.name ), command.xmlLine, "command uses unknown return type <" + command.returnType.name + ">" );
  }

  auto queueFlagBitsIt = m_enums.find( "VkQueueFlagBits" );
  assert( queueFlagBitsIt != m_enums.end() );
  for ( auto const & command : m_vkxml.commands )
  {
    for ( auto const & q : command.queues )
    {
      checkForError( containsByName( queueFlagBitsIt->second.values, q ), command.xmlLine, "command <" + command.name + "> uses unknown queue <" + q + ">" );
    }
  }
}

void VulkanHppGenerator::checkCorrectness() const
{
  checkBitmaskCorrectness();
  checkCommandCorrectness();
  checkDefineCorrectness();
  checkEnumCorrectness();
  checkExtensionCorrectness();
  checkFuncPointerCorrectness();
  checkRequireCorrectness();
  checkStructCorrectness();
  checkUnionCorrectness();
}

void VulkanHppGenerator::checkDefineCorrectness() const
{
  // check that any requirements of a define is known
  for ( auto const & d : m_vkxml.types.defines )
  {
    checkForError( d.require.empty() || m_types.contains( d.require ), d.xmlLine, "define <" + d.name + "> uses unknown require <" + d.require + ">" );
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

    if ( e.second.isBitmask )
    {
      if ( std::ranges::any_of( e.second.values, []( auto const & v ) { return v.supported; } ) )
      {
        // check that any enum of a bitmask with supported values is listed as "require" for a bitmask
        auto bitmaskIt = std::ranges::find_if( m_vkxml.types.bitmasks, [&e]( auto const & bitmask ) { return bitmask.require == e.first; } );
        checkForError( bitmaskIt != m_vkxml.types.bitmasks.end(),
                       e.second.xmlLine,
                       "enum <" + e.first + "> is not listed as an requires or bitvalues for any bitmask in the types section" );

        // check that bitwidth of the enum and type of the corresponding bitmask are equal
        checkForError( ( e.second.bitwidth != "64" ) || ( bitmaskIt->type == "VkFlags64" ),
                       e.second.xmlLine,
                       "enum <" + e.first + "> is marked with bitwidth <64> but corresponding bitmask <" + bitmaskIt->name + "> is not of type <VkFlags64>" );
      }
    }
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

  // check that all specified objectType values are used with a handle type
  auto objectTypeIt = m_enums.find( "VkObjectType" );
  assert( objectTypeIt != m_enums.end() );
  for ( auto const & objectTypeValue : objectTypeIt->second.values )
  {
    checkForError(
      ( objectTypeValue.name == "VK_OBJECT_TYPE_UNKNOWN" ) ||
        std::ranges::any_of( m_vkxml.types.handles, [&objectTypeValue]( TypeHandle const & handle ) { return handle.objTypeEnum == objectTypeValue.name; } ),
      objectTypeValue.xmlLine,
      "VkObjectType value <" + objectTypeValue.name + "> not specified as \"objtypeenum\" for any handle" );
  }
}

bool VulkanHppGenerator::checkEquivalentSingularConstructor( std::vector<size_t> const &        constructorIndices,
                                                             Command const &                    singularConstructor,
                                                             std::vector<Param>::const_iterator lenIt ) const
{
  // check, if there is no singular constructor with the very same arguments as this array constructor
  // (besides the size, of course)
  auto isEquivalentSingularConstructor = [this, &singularConstructor, lenIt]( size_t constructorIndex )
  {
    auto const & constructor = m_vkxml.commands[constructorIndex];
    if ( constructor.params.size() + 1 != singularConstructor.params.size() )
    {
      return false;
    }
    size_t const lenIdx = std::distance( singularConstructor.params.begin(), lenIt );
    for ( size_t i = 0, j = 0; i < constructor.params.size(); ++i, ++j )
    {
      assert( j < singularConstructor.params.size() );
      if ( j == lenIdx )
      {
        ++j;
      }
      if ( constructor.params[i].type.name != singularConstructor.params[j].type.name )
      {
        return false;
      }
    }
    return true;
  };
  return ( std::ranges::any_of( constructorIndices, isEquivalentSingularConstructor ) );
}

void VulkanHppGenerator::checkExtensionCorrectness() const
{
  for ( auto const & extension : m_extensions )
  {
    // check for existence of any depends, deprecation, obsoletion, or promotion
    for ( auto const & dep : extension.depends )
    {
      checkForError( isFeature( dep.first ), extension.xmlLine, "extension <" + extension.name + "> lists an unknown feature dependency <" + dep.first + ">" );
      for ( auto depsIt = dep.second.begin(); depsIt != dep.second.end(); ++depsIt )
      {
        for ( auto const & d : *depsIt )
        {
          checkForError( isFeature( d ) || isExtension( d ),
                         extension.xmlLine,
                         "extension <" + extension.name + "> lists an unknown feature or extension dependency <" + d + ">" );
        }
        checkForError( std::none_of( std::next( depsIt ), dep.second.end(), [&depsIt]( auto const & d ) { return *depsIt == d; } ),
                       extension.xmlLine,
                       "extension <" + extension.name + "> lists multiple identical dependencies" );
      }
    }
    checkForError( extension.deprecatedBy.empty() || isFeature( extension.deprecatedBy ) || isExtension( extension.deprecatedBy ),
                   extension.xmlLine,
                   "extension <" + extension.name + "> is deprecated by unknown extension/version <" + extension.deprecatedBy + ">" );
    checkForError( extension.obsoletedBy.empty() || isFeature( extension.obsoletedBy ) || isExtension( extension.obsoletedBy ),
                   extension.xmlLine,
                   "extension <" + extension.name + "> is obsoleted by unknown extension/version <" + extension.obsoletedBy + ">" );
    checkForError( extension.promotedTo.empty() || isFeature( extension.promotedTo ) || isExtension( extension.promotedTo ),
                   extension.xmlLine,
                   "extension <" + extension.name + "> is promoted to unknown extension/version <" + extension.promotedTo + ">" );

    // check for existence of any requirement
    for ( auto const & require : extension.requireData )
    {
      std::vector<std::string> requireDepends = tokenizeAny( require.depends, ",+()" );
      for ( auto const & dep : requireDepends )
      {
        checkForError( isFeature( dep ) || isExtension( dep ), require.xmlLine, "extension <" + extension.name + "> lists an unknown depends <" + dep + ">" );
      }
    }
  }
}

inline
void VulkanHppGenerator::checkForError( bool condition, int line, std::string const & message ) const
{
  ::checkForError( "VulkanHppGenerator", condition, line, message );
}

void VulkanHppGenerator::checkForWarning( bool condition, int line, std::string const & message ) const
{
  ::checkForWarning( "VulkanHppGenerator", condition, line, message );
}

void VulkanHppGenerator::checkFuncPointerCorrectness() const
{
  for ( auto const & funcPointer : m_vkxml.types.funcPointers )
  {
    checkForError( funcPointer.require.empty() || m_types.contains( funcPointer.require ),
                   funcPointer.xmlLine,
                   "funcpointer requires unknown <" + funcPointer.require + ">" );
    for ( auto const & param : funcPointer.params )
    {
      checkForError( m_types.contains( param.type.name ), param.xmlLine, "funcpointer param of unknown type <" + param.type.name + ">" );
    }
  }
}

void VulkanHppGenerator::checkRequireCorrectness() const
{
  // checks by features and extensions
  for ( auto & feature : m_vkxml.features )
  {
    checkRequireCorrectness( feature.require, "feature", feature.name );
  }
  for ( auto & extension : m_extensions )
  {
    checkRequireCorrectness( extension.requireData, "extension", extension.name );
  }
}

template <typename RequireType>
void VulkanHppGenerator::checkRequireCorrectness( std::vector<RequireType> const & requireTypes, std::string const & section, std::string const & name ) const
{
  for ( auto const & require : requireTypes )
  {
    checkRequireDependenciesCorrectness( require, section, name );
    checkRequireTypesCorrectness( require );
  }
}

void
  VulkanHppGenerator::checkRequireDependenciesCorrectness( FeatureRequire const & featureRequire, std::string const & section, std::string const & name ) const
{
  for ( auto const & depends : featureRequire.depends )
  {
    size_t separatorPos = depends.find( "::" );
    if ( separatorPos == std::string::npos )
    {
      checkForError( isFeature( depends ) || isExtension( depends ),
                     featureRequire.xmlLine,
                     section + " <" + name + "> depends on unknown extension or feature <" + depends + ">" );
    }
    else
    {
      std::string structure = depends.substr( 0, separatorPos );
      std::string member    = depends.substr( separatorPos + 2 );
      auto        structIt  = findByName( m_vkxml.types.structs, structure );
      checkForError(
        structIt != m_vkxml.types.structs.end(), featureRequire.xmlLine, section + " <" + name + "> requires member of an unknown struct <" + structure + ">" );
      checkForError( std::ranges::any_of( structIt->members, [&member]( auto const & md ) { return md.name == member; } ),
                     featureRequire.xmlLine,
                     section + " <" + name + "> requires unknown member <" + member + "> as part of the struct <" + structure + ">" );
    }
  }
}

void VulkanHppGenerator::checkRequireDependenciesCorrectness( RequireData const & requireData, std::string const & section, std::string const & name ) const
{
  std::vector<std::string> dependencies = tokenizeAny( requireData.depends, ",+()" );
  for ( auto const & depends : dependencies )
  {
    size_t separatorPos = depends.find( "::" );
    if ( separatorPos == std::string::npos )
    {
      checkForError( isFeature( depends ) || isExtension( depends ),
                     requireData.xmlLine,
                     section + " <" + name + "> depends on unknown extension or feature <" + depends + ">" );
    }
    else
    {
      std::string structure = depends.substr( 0, separatorPos );
      std::string member    = depends.substr( separatorPos + 2 );
      auto        structIt  = findByName( m_vkxml.types.structs, structure );
      checkForError(
        structIt != m_vkxml.types.structs.end(), requireData.xmlLine, section + " <" + name + "> requires member of an unknown struct <" + structure + ">" );
      checkForError( std::ranges::any_of( structIt->members, [&member]( auto const & md ) { return md.name == member; } ),
                     requireData.xmlLine,
                     section + " <" + name + "> requires unknown member <" + member + "> as part of the struct <" + structure + ">" );
    }
  }
}

template <typename RequireType>
void VulkanHppGenerator::checkRequireTypesCorrectness( RequireType const & requireType ) const
{
  for ( auto const & type : requireType.types )
  {
    auto typeIt = m_types.find( type.name );
    assert( typeIt != m_types.end() );
    // every required type should be listed in the corresponding map
    switch ( typeIt->second.category )
    {
      case TypeCategory::Bitmask:
        checkForError( findByNameOrAlias( m_vkxml.types.bitmasks, type.name ) != m_vkxml.types.bitmasks.end(),
                       typeIt->second.xmlLine,
                       "required bitmask type <" + type.name + "> is not listed as bitmask" );
        break;
      case TypeCategory::BaseType:
        checkForError(
          containsByName( m_vkxml.types.baseTypes, type.name ), typeIt->second.xmlLine, "required base type <" + type.name + "> is not listed as a base type" );
        break;
      case TypeCategory::Constant:
        checkForError(
          containsByName( m_vkxml.constants.values, type.name ), typeIt->second.xmlLine, "required constant <" + type.name + "> is not listed as a constant" );
        break;
      case TypeCategory::Define:
        checkForError(
          containsByName( m_vkxml.types.defines, type.name ), typeIt->second.xmlLine, "required define <" + type.name + "> is not listed as a define" );
        break;
      case TypeCategory::Enum:
        checkForError(
          findByNameOrAlias( m_enums, type.name ) != m_enums.end(), typeIt->second.xmlLine, "required enum type <" + type.name + "> is not listed as an enum" );
        break;
      case TypeCategory::External:
        checkForError( containsByName( m_vkxml.types.externals, type.name ),
                       typeIt->second.xmlLine,
                       "required external type <" + type.name + "> is not listed as an external type" );
        break;
      case TypeCategory::FuncPointer:
        checkForError( containsByName( m_vkxml.types.funcPointers, type.name ),
                       typeIt->second.xmlLine,
                       "required funcpointer <" + type.name + "> is not listed as a funcpointer" );
        break;
      case TypeCategory::Handle:
        checkForError( containsByNameOrAlias( m_vkxml.types.handles, type.name ),
                       typeIt->second.xmlLine,
                       "required handle type <" + type.name + "> is not listed as a handle" );
        break;
      case TypeCategory::Include:
        checkForError(
          containsByName( m_vkxml.types.includes, type.name ), typeIt->second.xmlLine, "required include <" + type.name + "> is not listed as an include" );
        break;
      case TypeCategory::Struct:
        checkForError( containsByNameOrAlias( m_vkxml.types.structs, type.name ),
                       typeIt->second.xmlLine,
                       "required struct type <" + type.name + "> is not listed as a struct" );
        break;
      case TypeCategory::Union:
        checkForError( containsByNameOrAlias( m_vkxml.types.unions, type.name ),
                       typeIt->second.xmlLine,
                       "required union type <" + type.name + "> is not listed as a struct" );
        break;
      case TypeCategory::Unknown: break;
      default                   : assert( false ); break;
    }
  }
}

void VulkanHppGenerator::checkStructCorrectness() const
{
  std::set<std::string> sTypeValues;
  for ( auto const & structure : m_vkxml.types.structs )
  {
    // check that a struct is referenced somewhere
    // I think, it's not forbidden to not reference a struct, but it would probably be not intended?
    auto typeIt = m_types.find( structure.name );
    assert( typeIt != m_types.end() );
    checkForError( !typeIt->second.requiredBy.empty(), structure.xmlLine, "structure <" + structure.name + "> not required by any feature or extension" );

    // checks on the members of a struct
    checkStructMemberCorrectness( structure.name, structure.members, sTypeValues );
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
    else if ( enumValue.supported )
    {
      auto valueIt = sTypeValues.find( enumValue.name );
      for ( auto aliasIt = enumValue.aliases.begin(); valueIt == sTypeValues.end() && aliasIt != enumValue.aliases.end(); ++aliasIt )
      {
        valueIt = sTypeValues.find( aliasIt->name );
      }
      checkForError( valueIt != sTypeValues.end(), enumValue.xmlLine, "VkStructureType enum value <" + enumValue.name + "> never used" );
      sTypeValues.erase( valueIt );
    }
  }
  assert( std::ranges::none_of( sTypeValues,
                                [&structureTypeIt]( auto const & value )
                                {
                                  auto valueIt = findByName( structureTypeIt->second.values, value );
                                  assert( valueIt != structureTypeIt->second.values.end() );
                                  return valueIt->supported;
                                } ) );
}

void VulkanHppGenerator::checkStructMemberArraySizesAreValid( std::vector<std::string> const & arraySizes, int line ) const
{
  // check that any array size is either a number or a known constant
  for ( auto const & arraySize : arraySizes )
  {
    if ( !isNumber( arraySize ) && !containsByName( m_vkxml.constants.values, arraySize ) )
    {
      auto typeIt = m_types.find( arraySize );
      checkForError( ( typeIt != m_types.end() ) && isUpperCase( arraySize ) && ( typeIt->second.category == TypeCategory::External ),
                     line,
                     "struct member array size uses unknown constant <" + arraySize + ">" );
    }
  }
}

void VulkanHppGenerator::checkStructMemberCorrectness( std::string const &               structureName,
                                                       std::vector<StructMember> const & members,
                                                       std::set<std::string> &           sTypeValues ) const
{
  // determine if this struct is requird/used
  bool const structUsed = isTypeUsed( structureName );

  for ( auto const & member : members )
  {
    checkStructMemberArraySizesAreValid( member.arraySizes, member.xmlLine );
    checkStructMemberSelectorConnection( member.selector, members, member.type.name );
    checkStructMemberTypeIsKnown( member.type.name, member.xmlLine );
    checkStructMemberTypeIsRequired( member.type.name, member.xmlLine, structureName );
    checkStructMemberValueIsValid( member.values, member.type.name, member.name, member.xmlLine, structUsed, structureName, sTypeValues );
  }
}

void VulkanHppGenerator::checkStructMemberSelectorConnection( std::string const &               selector,
                                                              std::vector<StructMember> const & members,
                                                              std::string const &               memberType ) const
{
  // if a member specifies a selector, that member is a union and the selector is an enum
  // check that there's a 1-1 connection between the specified selections and the values of that enum
  if ( !selector.empty() )
  {
    auto selectorIt = findByName( members, selector );
    assert( selectorIt != members.end() );
    auto selectorEnumIt = findByNameOrAlias( m_enums, selectorIt->type.name );
    assert( selectorEnumIt != m_enums.end() );
    auto unionIt = findByName( m_vkxml.types.unions, memberType );
    assert( unionIt != m_vkxml.types.unions.end() );
    for ( auto const & unionMember : unionIt->members )
    {
      // check that each union member has a selection, that is a value of the seleting enum
      for ( auto const & selection : unionMember.selection )
      {
        checkForError( containsByNameOrAlias( selectorEnumIt->second.values, selection ),
                       unionMember.xmlLine,
                       "union member <" + unionMember.name + "> uses selection <" + selection + "> that is not part of the selector type <" +
                         selectorIt->type.name + ">" );
      }
    }
  }
}

void VulkanHppGenerator::checkStructMemberTypeIsKnown( std::string const & memberType, int line ) const
{
  // check that the member type is known
  checkForError( m_types.contains( memberType ), line, "struct member uses unknown type <" + memberType + ">" );
}

void VulkanHppGenerator::checkStructMemberTypeIsRequired( std::string const & memberType, int line, std::string const & structureName ) const
{
  // check that the member type is required in some feature or extension
  if ( memberType.starts_with( "Vk" ) )
  {
    auto typeIt = m_types.find( memberType );
    assert( typeIt != m_types.end() );
    checkForError( !typeIt->second.requiredBy.empty(),
                   line,
                   "struct member type <" + memberType + "> used in struct <" + structureName + "> is never required for any feature or extension" );
  }
}

void VulkanHppGenerator::checkStructMemberValueIsValid( std::string const &     memberValue,
                                                        std::string const &     memberType,
                                                        std::string const &     memberName,
                                                        int                     line,
                                                        bool                    structUsed,
                                                        std::string const &     structureName,
                                                        std::set<std::string> & sTypeValues ) const
{
  if ( !memberValue.empty() )
  {
    auto enumIt = m_enums.find( memberType );
    if ( enumIt != m_enums.end() )
    {
      // check that the value exists in the specified enum (if the struct is used at all)
      checkForError( !structUsed || containsByNameOrAlias( enumIt->second.values, memberValue ),
                     line,
                     "struct member <" + memberName + "> in structure <" + structureName + "> holds value <" + memberValue + "> of enum type <" + memberType +
                       "> which is not listed" );
      // special handling for sType: no value should appear more than once
      checkForError(
        !structUsed || ( memberName != "sType" ) || sTypeValues.insert( memberValue ).second, line, "sType value <" + memberValue + "> has been used before" );
    }
    else if ( memberType == "uint32_t" )
    {
      checkForError( isNumber( memberValue ),
                     line,
                     "value <" + memberValue + "> for member <" + memberName + "> in structure <" + structureName + "> of type <" + memberType +
                       "> is not a number" );
    }
    else
    {
      // don't know the type of the value -> error out
      checkForError( false,
                     line,
                     "member <" + memberName + "> in structure <" + structureName + "> holds value <" + memberValue + "> for an unhandled type <" + memberType +
                       ">" );
    }
  }
}

void VulkanHppGenerator::checkUnionCorrectness() const
{
  std::set<std::string> sTypeValues;
  for ( auto const & vkUnion : m_vkxml.types.unions )
  {
    // check that a struct is referenced somewhere
    // I think, it's not forbidden to not reference a struct, but it would probably be not intended?
    auto typeIt = m_types.find( vkUnion.name );
    assert( typeIt != m_types.end() );
    checkForError( !typeIt->second.requiredBy.empty(), vkUnion.xmlLine, "union <" + vkUnion.name + "> not required by any feature or extension" );

    // checks on the members of a union
    checkForError( !containsByName( vkUnion.members, "sType" ), vkUnion.xmlLine, "union <" + vkUnion.name + "> holds unexpected member named \"sType\"" );
    checkUnionMemberCorrectness( vkUnion.name, vkUnion.members );
  }
}

void VulkanHppGenerator::checkUnionMemberCorrectness( std::string const & structureName, std::vector<UnionMember> const & members ) const
{
  for ( auto const & member : members )
  {
    checkStructMemberArraySizesAreValid( member.arraySizes, member.xmlLine );
    checkStructMemberTypeIsKnown( member.type.name, member.xmlLine );
    checkStructMemberTypeIsRequired( member.type.name, member.xmlLine, structureName );
  }
}

bool VulkanHppGenerator::containsArray( std::string const & type ) const
{
  // a simple recursive check if a type is or contains an array
  if ( auto structureIt = findByName( m_vkxml.types.structs, type ); structureIt != m_vkxml.types.structs.end() )
  {
    return std::ranges::any_of( structureIt->members,
                                [this]( auto const & member ) { return !member.arraySizes.empty() || containsArray( member.type.name ); } );
  }
  else if ( auto unionIt = findByName( m_vkxml.types.unions, type ); unionIt != m_vkxml.types.unions.end() )
  {
    return std::ranges::any_of( unionIt->members, [this]( auto const & member ) { return !member.arraySizes.empty() || containsArray( member.type.name ); } );
  }
  else
  {
    return false;
  }
}

bool VulkanHppGenerator::containsDeprecated( std::vector<StructMember> const & members ) const
{
  return std::ranges::any_of( members, []( auto const & member ) { return !member.deprecated.empty(); } );
}

bool VulkanHppGenerator::containsFuncPointer( std::string const & type ) const
{
  // a simple recursive check if a type contains a funcpointer
  auto structureIt = findByName( m_vkxml.types.structs, type );
  // We just care for func pointers in structs, not in unions!
  return ( structureIt != m_vkxml.types.structs.end() ) &&
         std::ranges::any_of( structureIt->members,
                              [this, &type]( auto const & member )
                              {
                                return containsByName( m_vkxml.types.funcPointers, member.type.name ) ||
                                       ( ( member.type.name != type ) && containsFuncPointer( member.type.name ) );
                              } );
}

bool VulkanHppGenerator::containsFloatingPoints( std::vector<StructMember> const & members ) const
{
  return std::ranges::any_of( members,
                              [this]( auto const & member )
                              {
                                if ( member.type.isValue() )
                                {
                                  auto structureIt = findByName( m_vkxml.types.structs, member.type.name );
                                  return ( member.type.name == "float" ) || ( member.type.name == "double" ) ||
                                         ( ( structureIt != m_vkxml.types.structs.end() ) && containsFloatingPoints( structureIt->members ) );
                                }
                                else
                                {
                                  return false;
                                }
                              } );
}

bool VulkanHppGenerator::containsUnion( std::string const & type ) const
{
  // a simple recursive check if a type is or contains a union
  auto unionIt = findByName( m_vkxml.types.unions, type );
  if ( unionIt != m_vkxml.types.unions.end() )
  {
    return true;
  }
  else
  {
    auto structureIt = findByName( m_vkxml.types.structs, type );
    return ( structureIt != m_vkxml.types.structs.end() ) &&
           ( std::ranges::any_of( structureIt->members,
                                  [this]( auto const & member ) { return member.type.isValue() && containsUnion( member.type.name ); } ) );
  }
}

bool VulkanHppGenerator::describesVector( TypeStruct const & vkStruct, std::string const & type ) const
{
  auto const & members = vkStruct.members;
  return std::ranges::any_of( members,
                              [&members, &type]( auto const & member )
                              {
                                if ( ( type.empty() || ( member.type.name == type ) ) && member.type.isNonConstPointer() && ( member.len.size() == 1 ) )
                                {
                                  auto lenMemberIt = findByName( members, member.len[0] );
                                  assert( lenMemberIt != members.end() );
                                  return lenMemberIt->type.isValue() && ( ( lenMemberIt->type.name == "size_t" ) || ( lenMemberIt->type.name == "uint32_t" ) );
                                }
                                return false;
                              } );
}

bool VulkanHppGenerator::describesVector( TypeUnion const & vkUnion, std::string const & type ) const
{
  auto const & members = vkUnion.members;
  return std::ranges::any_of( members,
                              [&members, &type]( auto const & member )
                              {
                                if ( ( type.empty() || ( member.type.name == type ) ) && member.type.isNonConstPointer() )
                                {
                                  auto lenMemberIt = findByName( members, member.len );
                                  assert( lenMemberIt != members.end() );
                                  return lenMemberIt->type.isValue() && ( ( lenMemberIt->type.name == "size_t" ) || ( lenMemberIt->type.name == "uint32_t" ) );
                                }
                                return false;
                              } );
}

std::vector<size_t> VulkanHppGenerator::determineChainedReturnParams( std::vector<Param> const & params, std::vector<size_t> const & returnParams ) const
{
  std::vector<size_t> chainedParams;
  std::ranges::copy_if(
    returnParams, std::back_inserter( chainedParams ), [this, &params]( size_t rp ) { return isStructureChainAnchor( params[rp].type.name ); } );
  return chainedParams;
}

std::vector<std::string> VulkanHppGenerator::determineDataTypes( std::vector<Param> const &                params,
                                                                 std::map<size_t, VectorParamData> const & vectorParams,
                                                                 std::vector<size_t> const &               returnParams,
                                                                 std::set<size_t> const &                  templatedParams,
                                                                 bool                                      raii ) const
{
  std::vector<std::string> dataTypes;
  for ( auto rp : returnParams )
  {
    auto vectorParamIt = vectorParams.find( rp );
    if ( templatedParams.contains( rp ) )
    {
      assert( ( vectorParamIt == vectorParams.end() ) || !vectorParamIt->second.byStructure );
      if ( ( vectorParamIt != vectorParams.end() ) && ( params[rp].type.name == "void" ) &&
           std::ranges::any_of( returnParams, [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->first; } ) &&
           std::ranges::any_of( returnParams, [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->second.lenParam; } ) )
      {
        // sized data of type void is returned as a std::array<uint8_t>
        dataTypes.push_back( "uint8_t" );
      }
      else
      {
        // for all other (templated) return parameters, the dataType is the name of the parameter with prefix "p" removed and "Type" appended
        dataTypes.push_back( ( stripPrefix( params[rp].name, "p" ) + "Type" ) );
      }
    }
    else if ( ( vectorParamIt != vectorParams.end() ) && vectorParamIt->second.byStructure )
    {
      // for non-templated vector parameters, we have to distinguish between those specified by a structure and those specified by a simple length parameter
      dataTypes.push_back(
        trimEnd( stripPostfix( vectorMemberByStructure( params[rp].type.name ).type.compose( "Vk", raii ? "VULKAN_HPP_NAMESPACE" : "" ), "*" ) ) );
    }
    else
    {
      dataTypes.push_back(
        trimEnd( stripPostfix( params[rp].type.compose( "Vk", ( raii && m_handles.contains( params[rp].type.name ) ) ? "VULKAN_HPP_NAMESPACE" : "" ), "*" ) ) );
    }
  }
  return dataTypes;
}

size_t VulkanHppGenerator::determineDefaultStartIndex( std::vector<Param> const & params, std::set<size_t> const & skippedParams ) const
{
  // determine the index where the arguments start to have defaults
  size_t defaultStartIndex = INVALID_INDEX;
  for ( int i = static_cast<int>( params.size() ) - 1; ( 0 <= i ) && ( isOptional( params[i].optional ) || skippedParams.contains( i ) ); --i )
  {
    defaultStartIndex = i;
  }
  return defaultStartIndex;
}

bool VulkanHppGenerator::determineEnumeration( std::map<size_t, VectorParamData> const & vectorParams, std::vector<size_t> const & returnParams ) const
{
  // a command is considered to be enumerating some data, if for at least one vectorParam the data is a returnParam and either the vectorParams is
  // specified by a structure or the lenParam is a returnParam as well
  return std::ranges::any_of( vectorParams,
                              [&returnParams]( auto const & vp )
                              {
                                return std::ranges::any_of( returnParams, [&vp]( size_t rp ) { return rp == vp.first; } ) &&
                                       ( vp.second.byStructure ||
                                         std::ranges::any_of( returnParams, [&vp]( size_t rp ) { return rp == vp.second.lenParam; } ) );
                              } );
}

size_t VulkanHppGenerator::determineInitialSkipCount( std::string const & commandName ) const
{
  // determine the number of arguments to skip for a function
  // -> 0: the command is not bound to an instance or a device (the corresponding handle has no name)
  // -> 1: the command is bound to an instance or a device (the corresponding handle has a name)
  // -> 2: the command has been moved to a second handle
  auto commandIt = findByNameOrAlias( m_vkxml.commands, commandName );
  assert( commandIt != m_vkxml.commands.end() );
  auto handleIt = m_handles.find( isHandleType( commandIt->params[0].type.name ) ? commandIt->params[0].type.name : "" );
  assert( handleIt != m_handles.end() );
  if ( !handleIt->second.commands.contains( commandIt->name ) )
  {
    assert( 1 < commandIt->params.size() );
    assert( m_handles.contains( commandIt->params[1].type.name ) );
    return 2;
  }
  else
  {
    return handleIt->first.empty() ? 0 : 1;
  }
}

std::vector<size_t> VulkanHppGenerator::determineReturnParams( std::vector<Param> const & params ) const
{
  std::vector<size_t> returnParams;

  for ( size_t i = 0; i < params.size(); i++ )
  {
    // very special handling of parameters of some types, which always come as a non-const pointer but are not meant
    // to be a potential return value!
    if ( params[i].type.isNonConstPointer() && !specialPointerTypes.contains( params[i].type.name ) )
    {
      returnParams.push_back( i );
    }
  }
  return returnParams;
}

bool VulkanHppGenerator::isConstructorCandidate( Command const & command, std::string const & handleType ) const
{
  std::set<std::string> const candidatePrefixes{ "vkAcquire", "vkAllocate", "vkCreate", "vkEnumerate", "vkRegister" };
  std::set<std::string> const otherCandidates{ "vkGetDeviceQueue",   "vkGetDeviceQueue2",          "vkGetDisplayPlaneSupportedDisplaysKHR",
                                               "vkGetDrmDisplayEXT", "vkGetRandROutputDisplayEXT", "vkGetWinrtDisplayNV" };
  return isSupported( command.requiredBy ) &&
         ( std::ranges::any_of( candidatePrefixes, [&command]( std::string const & prefix ) { return command.name.starts_with( prefix ); } ) ||
           otherCandidates.contains( command.name ) ) &&
         isApiSupported( command.api, m_api ) &&
         std::ranges::any_of( command.params, [this, &handleType]( Param const & pd ) { return isConstructorCandidate( pd, handleType ); } );
}

bool VulkanHppGenerator::isConstructorCandidate( Param const & paramData, std::string const & handleType ) const
{
  if ( paramData.type.isNonConstPointer() )
  {
    if ( paramData.type.name == handleType )
    {
      return true;
    }
    else
    {
      auto structIt = findByName( m_vkxml.types.structs, paramData.type.name );
      if ( structIt != m_vkxml.types.structs.end() )
      {
        return describesVector( *structIt, handleType );
      }
    }
  }
  return false;
}

std::vector<size_t> VulkanHppGenerator::determineRAIIHandleConstructors( std::string const & handleType, size_t destructorIndex ) const
{
  std::vector<size_t> constructorIndices;
  for ( auto commandIt = m_vkxml.commands.begin(); commandIt != m_vkxml.commands.end(); ++commandIt )
  {
    if ( isConstructorCandidate( *commandIt, handleType ) )
    {
      // only commands that provide all information needed for the destructor can be considered a constructor!
      bool valid = true;
      if ( destructorIndex != INVALID_INDEX )
      {
        // get the destructors parameter to the handleType
        auto const & destructor      = m_vkxml.commands[destructorIndex];
        auto destructorHandleParamIt = std::ranges::find_if( destructor.params, [&handleType]( Param const & pd ) { return pd.type.name == handleType; } );
        assert( destructorHandleParamIt != destructor.params.end() );

        // lambda to check if a destructor parameter is a parameter of the constructor candidate
        // (or it's just the len parameter, which is not needed for the constructor)
        auto isConstructorCandidateParam = [&destructorHandleParamIt, &commandIt, this]( Param const & destructorParam )
        {
          // check if the destructor param type equals this param type, or, if this param type is a struct, is part of
          // that struct
          auto isDestructorParamType = [&destructorParam, this]( Param const & pd )
          {
            if ( pd.type.name != destructorParam.type.name )
            {
              // check if the destructor param type equals a structure member type
              auto structureIt = findByName( m_vkxml.types.structs, pd.type.name );
              if ( structureIt != m_vkxml.types.structs.end() )
              {
                return ( findByType( structureIt->members, destructorParam.type.name ) != structureIt->members.end() );
              }
              else
              {
                return false;
              }
            }
            return true;
          };

          return ( destructorParam.name == destructorHandleParamIt->len ) || std::ranges::any_of( commandIt->params, isDestructorParamType );
        };

        // the constructor candidate is valid, if none of the (relevant) destructor parameters is missing in the
        // constructor candidate params
        valid = std::ranges::all_of( destructor.params, isConstructorCandidateParam );
      }
      if ( valid )
      {
        constructorIndices.push_back( std::distance( m_vkxml.commands.begin(), commandIt ) );
      }
    }
  }
  return constructorIndices;
}

size_t VulkanHppGenerator::determineRAIIHandleDestructor( std::string const & handleType ) const
{
  std::string type         = stripPrefix( handleType, "Vk" );
  auto        destructorIt = findByName( m_vkxml.commands, "vkDestroy" + type );
  if ( destructorIt == m_vkxml.commands.end() )
  {
    destructorIt = findByName( m_vkxml.commands, "vkFree" + type + "s" );
    if ( destructorIt == m_vkxml.commands.end() )
    {
      destructorIt = findByName( m_vkxml.commands, "vkRelease" + type );
      if ( destructorIt == m_vkxml.commands.end() )
      {
        if ( handleType == "VkDeviceMemory" )
        {
          // special handling for vkDeviceMemory
          destructorIt = findByName( m_vkxml.commands, "vkFreeMemory" );
          assert( destructorIt != m_vkxml.commands.end() );
        }
        else if ( handleType == "VkDisplayKHR" )
        {
          // special handling for VkDisplayKHR
          destructorIt = findByName( m_vkxml.commands, "vkReleaseDisplayEXT" );
          assert( destructorIt != m_vkxml.commands.end() );
        }
        else
        {
          assert( isEnumerated( handleType ) || ( handleType == "VkDisplayModeKHR" ) || ( handleType == "VkQueue" ) );
        }
      }
    }
  }
  return ( destructorIt != m_vkxml.commands.end() ) ? std::distance( m_vkxml.commands.begin(), destructorIt ) : INVALID_INDEX;
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

std::set<size_t> VulkanHppGenerator::determineSkippedParams( std::vector<Param> const &                params,
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
      assert( !params[vpi.first].len.empty() );
      if ( ( std::ranges::none_of( returnParams, [&vpi]( size_t rpi ) { return vpi.first == rpi; } ) && isParam( params[vpi.first].len, params ) ) ||
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

TypeStruct const * VulkanHppGenerator::determineSubStruct( TypeStruct const & vkStruct ) const
{
  auto const & members = vkStruct.members;
  if ( members.front().name != "sType" )
  {
    // check if sd is a substruct of structure
    auto isSubStruct = [&vkStruct, &members]( TypeStruct const & subStructCandidate ) noexcept
    {
      // member-by-member comparison of type and name
      auto memberIt = members.begin();
      auto isMember = [&memberIt]( StructMember const & member ) noexcept
      {
        if ( ( member.type == memberIt->type ) && ( member.name == memberIt->name ) )
        {
          ++memberIt;
          return true;
        }
        return false;
      };

      auto const & subStructMembers = subStructCandidate.members;
      return ( subStructCandidate.xmlLine < vkStruct.xmlLine ) && ( subStructMembers.size() < members.size() ) &&
             std::ranges::all_of( subStructMembers, isMember );
    };

    // look for a struct in m_structs that starts identically to structure
    auto structIt = std::ranges::find_if( m_vkxml.types.structs, isSubStruct );
    return ( structIt == m_vkxml.types.structs.end() ) ? nullptr : &*structIt;
  }
  return nullptr;
}

std::map<size_t, VulkanHppGenerator::VectorParamData> VulkanHppGenerator::determineVectorParams( std::vector<Param> const & params ) const
{
  std::map<size_t, VectorParamData> vectorParams;

  // look for the parameters whose len equals the name of an other parameter
  for ( size_t i = 0; i < params.size(); i++ )
  {
    if ( !params[i].len.empty() && ( params[i].len != "null-terminated" ) && ( params[i].len != "1" ) )
    {
      VectorParamData & vpd = vectorParams[i];

      assert( params[i].altLen.empty() || params[i].altLen == "(samples + 31) / 32" );
      auto lenIt = findByName( params, params[i].altLen.empty() ? params[i].len : "samples" );
      if ( lenIt == params.end() )
      {
        lenIt = std::ranges::find_if( params, [this, &len = params[i].len]( auto const & pd ) { return isLenByStructMember( len, pd ); } );
      }
      assert( lenIt != params.end() );
      vpd.lenParam = std::distance( params.begin(), lenIt );
      if ( !params[i].stride.empty() )
      {
        vpd.strideParam = determineParamIndex( params, params[i].stride );
      }
    }
    else
    {
      auto structIt = findByName( m_vkxml.types.structs, params[i].type.name );
      if ( ( structIt != m_vkxml.types.structs.end() ) && !isStructureChainAnchor( params[i].type.name ) && describesVector( *structIt ) )
      {
        VectorParamData & vpd = vectorParams[i];
        vpd.byStructure       = true;
        vpd.lenParam          = i;
      }
    }
  }
  return vectorParams;
}

std::set<size_t> VulkanHppGenerator::determineVoidPointerParams( std::vector<Param> const & params ) const
{
  std::set<size_t> voidPointerParams;

  for ( size_t i = 0; i < params.size(); i++ )
  {
    if ( !params[i].type.isValue() && ( params[i].type.name == "void" ) && ( params[i].type.postfix != "**" ) )
    {
      voidPointerParams.insert( i );
    }
  }
  return voidPointerParams;
}

void VulkanHppGenerator::distributeEnumValueAliases()
{
  for ( auto & e : m_enums )
  {
    for ( auto & a : e.second.valueAliases )
    {
      auto valueIt = findByNameOrAlias( e.second.values, a.alias );
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
      checkForError( std::ranges::none_of( valueIt->aliases, [&a]( auto const & eav ) { return a.name == eav.name; } ),
                     a.xmlLine,
                     "enum value alias <" + a.name + "> already contained as alias for enum value <" + a.alias + ">" );
      valueIt->aliases.push_back( a );
    }
    e.second.valueAliases.clear();
  }
}

void VulkanHppGenerator::distributeRequirements()
{
  for ( auto const & feature : m_vkxml.features )
  {
    distributeRequirements( feature.require, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    distributeRequirements( extension.requireData, extension.name );
    distributeRequirements( extension.unsupportedRequireData, extension.name );
  }
  for ( auto const & feature : m_unsupportedFeatures )
  {
    distributeRequirements( feature.require, feature.name );
  }
  for ( auto const & extension : m_unsupportedExtensions )
  {
    distributeRequirements( extension.requireData, extension.name );
    distributeRequirements( extension.unsupportedRequireData, extension.name );
  }
}

template <typename RequireTypes>
void VulkanHppGenerator::distributeRequirements( std::vector<RequireTypes> const & requireTypes, std::string const & requiredBy )
{
  for ( auto const & require : requireTypes )
  {
    for ( auto const & type : require.types )
    {
      auto typeIt = m_types.find( type.name );
      checkForError( typeIt != m_types.end(), type.xmlLine, "unknown required type <" + type.name + ">" );
      typeIt->second.requiredBy.insert( requiredBy );
    }
  }
}

bool VulkanHppGenerator::encodesEnumeration( std::vector<Param> const &                params,
                                             std::map<size_t, VectorParamData> const & vectorParams,
                                             size_t                                    returnParam0,
                                             size_t                                    returnParam1 ) const
{
  auto vpIt = vectorParams.find( returnParam1 );
  return params[returnParam0].type.isNonConstPointer() &&
         ( ( params[returnParam0].type.name == "size_t" ) || ( params[returnParam0].type.name == "uint32_t" ) ) &&
         params[returnParam1].type.isNonConstPointer() && ( params[returnParam1].len == params[returnParam0].name ) && ( vpIt != vectorParams.end() ) &&
         ( vpIt->second.lenParam == returnParam0 ) && !vpIt->second.byStructure && ( vpIt->second.strideParam == INVALID_INDEX );
}

void VulkanHppGenerator::extendSpecialCommands( std::string const & name, bool definition, bool raii, std::string & cmd ) const
{
  if ( name == "vkSetDebugUtilsObjectNameEXT" )
  {
    if ( raii )
    {
      if ( definition )
      {
        cmd.append(
          R"(
    // wrapper function for command vkSetDebugUtilsObjectNameEXT, see
    // https://registry.khronos.org/vulkan/specs/latest/man/html/vkSetDebugUtilsObjectNameEXT.html
    template <typename HandleType>
    VULKAN_HPP_INLINE typename ResultValueType<void>::type Device::setDebugUtilsObjectNameEXT( HandleType const & handle, std::string const & name ) const
    {
      VULKAN_HPP_STATIC_ASSERT( VULKAN_HPP_NAMESPACE::isVulkanHandleType<HandleType>::value, "HandleType must be a Vulkan handle type" );
      // It might be, that neither constructors, nor setters, nor designated initializers are available... need to explicitly set member by member
      VULKAN_HPP_NAMESPACE::DebugUtilsObjectNameInfoEXT nameInfo;
      nameInfo.objectType   = handle.objectType;
#  if ( VK_USE_64_BIT_PTR_DEFINES == 1 )
      nameInfo.objectHandle = reinterpret_cast<uint64_t>( static_cast<typename HandleType::CType>( handle ) );
#  else
      nameInfo.objectHandle = static_cast<typename HandleType::CType>( handle );
#  endif
      nameInfo.pObjectName  = name.c_str();
      return setDebugUtilsObjectNameEXT( nameInfo );
    }
)" );
      }
      else
      {
        cmd.append( R"(
      // wrapper function for command vkSetDebugUtilsObjectNameEXT, see
      // https://registry.khronos.org/vulkan/specs/latest/man/html/vkSetDebugUtilsObjectNameEXT.html
      template <typename HandleType>
      typename ResultValueType<void>::type setDebugUtilsObjectNameEXT( HandleType const & handle, std::string const & name ) const;
)" );
      }
    }
    else
    {
      static std::string const endOfCmd = "#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */\n";
      assert( cmd.ends_with( endOfCmd ) );
      size_t pos = cmd.size() - endOfCmd.size();
      if ( definition )
      {
        cmd.insert(
          pos,
          R"(  // wrapper function for command vkSetDebugUtilsObjectNameEXT, see https://registry.khronos.org/vulkan/specs/latest/man/html/vkSetDebugUtilsObjectNameEXT.html
#  if VULKAN_HPP_CPP_VERSION < 20
  template <typename HandleType,
            typename Dispatch,
            typename std::enable_if<IS_DISPATCHED( vkSetDebugUtilsObjectNameEXT ) && isVulkanHandleType<HandleType>::value, bool>::type>
#  else
  template <VulkanHandleType HandleType, typename Dispatch>
  requires( IS_DISPATCHED( vkSetDebugUtilsObjectNameEXT ) )
#  endif
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
                                          Device::setDebugUtilsObjectNameEXT( HandleType const & handle, std::string const & name, Dispatch const & d ) const
  {
    // It might be, that neither constructors, nor setters, nor designated initializers are available... need to explicitly set member by member
    VULKAN_HPP_NAMESPACE::DebugUtilsObjectNameInfoEXT nameInfo;
    nameInfo.objectType   = handle.objectType;
#  if ( VK_USE_64_BIT_PTR_DEFINES == 1 )
    nameInfo.objectHandle = reinterpret_cast<uint64_t>( static_cast<typename HandleType::CType>( handle ) );
#  else
    nameInfo.objectHandle = static_cast<typename HandleType::CType>( handle );
#  endif
    nameInfo.pObjectName  = name.c_str();
    return setDebugUtilsObjectNameEXT( nameInfo, d );
  }
)" );
      }
      else
      {
        cmd.insert( pos, R"(    // wrapper function for command vkSetDebugUtilsObjectNameEXT, see
    // https://registry.khronos.org/vulkan/specs/latest/man/html/vkSetDebugUtilsObjectNameEXT.html
#  if VULKAN_HPP_CPP_VERSION < 20
    template <typename HandleType,
              typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE,
              typename std::enable_if<IS_DISPATCHED( vkSetDebugUtilsObjectNameEXT ) && isVulkanHandleType<HandleType>::value, bool>::type = true>
#  else
    template <VulkanHandleType HandleType, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    requires( IS_DISPATCHED( vkSetDebugUtilsObjectNameEXT ) )
#  endif
    VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS typename ResultValueType<void>::type
      setDebugUtilsObjectNameEXT( HandleType const & handle, std::string const & name, Dispatch const & d VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) const;
)" );
      }
    }
  }
  else if ( name == "vkSetDebugUtilsObjectTagEXT" )
  {
    if ( raii )
    {
      if ( definition )
      {
        cmd.append(
          R"(
    // wrapper function for command vkSetDebugUtilsObjectTagEXT, see
    // https://registry.khronos.org/vulkan/specs/latest/man/html/vkSetDebugUtilsObjectTagEXT.html
    template <typename HandleType, typename TagType>
    VULKAN_HPP_INLINE typename ResultValueType<void>::type
      Device::setDebugUtilsObjectTagEXT( HandleType const & handle, uint64_t name, TagType const & tag ) const
    {
      VULKAN_HPP_STATIC_ASSERT( VULKAN_HPP_NAMESPACE::isVulkanHandleType<HandleType>::value, "HandleType must be a Vulkan handle type" );
      // It might be, that neither constructors, nor setters, nor designated initializers are available... need to explicitly set member by member
      VULKAN_HPP_NAMESPACE::DebugUtilsObjectTagInfoEXT tagInfo;
      tagInfo.objectType   = handle.objectType;
#  if ( VK_USE_64_BIT_PTR_DEFINES == 1 )
      tagInfo.objectHandle = reinterpret_cast<uint64_t>( static_cast<typename HandleType::CType>( handle ) );
#  else
      tagInfo.objectHandle = static_cast<typename HandleType::CType>( handle );
#  endif
      tagInfo.tagName      = name;
      tagInfo.tagSize      = sizeof( TagType );
      tagInfo.pTag         = &tag;
      return setDebugUtilsObjectTagEXT( tagInfo );
    }
)" );
      }
      else
      {
        cmd.append( R"(
      // wrapper function for command vkSetDebugUtilsObjectTagEXT, see
      // https://registry.khronos.org/vulkan/specs/latest/man/html/vkSetDebugUtilsObjectTagEXT.html
      template <typename HandleType, typename TagType>
      typename ResultValueType<void>::type setDebugUtilsObjectTagEXT( HandleType const & handle, uint64_t name, TagType const & tag ) const;
)" );
      }
    }
    else
    {
      static std::string const endOfCmd = "#endif /* VULKAN_HPP_DISABLE_ENHANCED_MODE */\n";
      assert( cmd.ends_with( endOfCmd ) );
      size_t pos = cmd.size() - endOfCmd.size();
      if ( definition )
      {
        cmd.insert(
          pos,
          R"(  // wrapper function for command vkSetDebugUtilsObjectTagEXT, see https://registry.khronos.org/vulkan/specs/latest/man/html/vkSetDebugUtilsObjectTagEXT.html
#  if VULKAN_HPP_CPP_VERSION < 20
  template <typename HandleType,
            typename TagType,
            typename Dispatch,
            typename std::enable_if<IS_DISPATCHED( vkSetDebugUtilsObjectTagEXT ) && isVulkanHandleType<HandleType>::value, bool>::type>
#  else
  template <VulkanHandleType HandleType, typename TagType, typename Dispatch>
  requires( IS_DISPATCHED( vkSetDebugUtilsObjectTagEXT ) )
#  endif
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<void>::type
    Device::setDebugUtilsObjectTagEXT( HandleType const & handle, uint64_t name, TagType const & tag, Dispatch const & d ) const
  {
    // It might be, that neither constructors, nor setters, nor designated initializers are available... need to explicitly set member by member
    VULKAN_HPP_NAMESPACE::DebugUtilsObjectTagInfoEXT tagInfo;
    tagInfo.objectType   = handle.objectType;
#  if ( VK_USE_64_BIT_PTR_DEFINES == 1 )
    tagInfo.objectHandle = reinterpret_cast<uint64_t>( static_cast<typename HandleType::CType>( handle ) );
#  else
    tagInfo.objectHandle = static_cast<typename HandleType::CType>( handle );
#  endif
    tagInfo.tagName      = name;
    tagInfo.tagSize      = sizeof( TagType );
    tagInfo.pTag         = &tag;
    return setDebugUtilsObjectTagEXT( tagInfo, d );
  }
)" );
      }
      else
      {
        cmd.insert( pos, R"(    // wrapper function for command vkSetDebugUtilsObjectTagEXT, see
    // https://registry.khronos.org/vulkan/specs/latest/man/html/vkSetDebugUtilsObjectTagEXT.html
#  if VULKAN_HPP_CPP_VERSION < 20
    template <typename HandleType,
              typename TagType,
              typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE,
              typename std::enable_if<IS_DISPATCHED( vkSetDebugUtilsObjectTagEXT ) && isVulkanHandleType<HandleType>::value, bool>::type = true>
#  else
    template <VulkanHandleType HandleType, typename TagType, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    requires( IS_DISPATCHED( vkSetDebugUtilsObjectTagEXT ) )
#  endif
    VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS typename ResultValueType<void>::type setDebugUtilsObjectTagEXT(
      HandleType const & handle, uint64_t name, TagType const & tag, Dispatch const & d VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT ) const;
)" );
      }
    }
  }
}

std::vector<StructMember>::const_iterator VulkanHppGenerator::findHandleMember( std::vector<StructMember> const & members ) const
{
  return std::ranges::find_if( members, [this]( auto const & member ) { return isHandleType( member.type.name ); } );
}

std::vector<StructMember>::const_iterator VulkanHppGenerator::findVectorMember( std::vector<StructMember> const & members ) const
{
  return std::ranges::find_if(
    members, []( auto const & member ) { return member.arraySizes.empty() && !member.len.empty() && ( member.len[0] != "null-terminated" ); } );
}

template <typename RequireType>
void VulkanHppGenerator::forEachRequiredBitmask( std::vector<RequireType> const &                   requireTypes,
                                                 std::set<std::string> &                            encounteredBitmasks,
                                                 std::function<void( TypeBitmask const & )> const & bitmaskAction ) const
{
  for ( auto const & require : requireTypes )
  {
    for ( auto const & type : require.types )
    {
      if ( auto bitmaskIt = findByName( m_vkxml.types.bitmasks, type.name );
           ( bitmaskIt != m_vkxml.types.bitmasks.end() ) && encounteredBitmasks.insert( bitmaskIt->name ).second )
      {
        bitmaskAction( *bitmaskIt );
      }
    }
  }
}

template <typename RequireType>
void VulkanHppGenerator::forEachRequiredCommand( std::vector<RequireType> const &                                    requireTypes,
                                                 std::function<void( std::string const &, Command const & )> const & commandAction ) const
{
  for ( auto const & require : requireTypes )
  {
    for ( auto const & command : require.commands )
    {
      auto commandIt = findByNameOrAlias( m_vkxml.commands, command.name );
      checkForError( commandIt != m_vkxml.commands.end(), command.xmlLine, "unknown required command <" + command.name + ">" );
      commandAction( command.name, *commandIt );
    }
  }
}

void VulkanHppGenerator::forEachRequiredConstant( std::vector<FeatureRequire> const &                  featureRequires,
                                                  std::set<std::string> &                              encounteredConstants,
                                                  std::function<void( ConstantValue const & )> const & constantAction ) const
{
  for ( auto const & require : featureRequires )
  {
    for ( auto const & constant : require.enumConstants )
    {
      auto constantIt = findByName( m_vkxml.constants.values, constant.name );
      checkForError( constantIt != m_vkxml.constants.values.end(), constant.xmlLine, "unknown required constant <" + constant.name + ">" );
      if ( encounteredConstants.insert( constantIt->name ).second )
      {
        constantAction( *constantIt );
      }
    }
  }
}

void VulkanHppGenerator::forEachRequiredConstant( std::vector<RequireData> const &                     requireDatas,
                                                  std::set<std::string> &                              encounteredConstants,
                                                  std::function<void( ConstantValue const & )> const & constantAction ) const
{
  for ( auto const & require : requireDatas )
  {
    for ( auto const & constant : require.constants )
    {
      auto constantIt = findByName( m_vkxml.constants.values, constant.name );
      checkForError( constantIt != m_vkxml.constants.values.end(), constant.xmlLine, "unknown required constant <" + constant.name + ">" );
      if ( encounteredConstants.insert( constantIt->name ).second )
      {
        constantAction( *constantIt );
      }
    }
  }
}

void VulkanHppGenerator::forEachRequiredEnumConstant( std::vector<RequireData> const &                        requireData,
                                                      std::set<std::string> &                                 encounteredEnumConstants,
                                                      std::function<void( EnumConstantData const & )> const & enumConstantAction ) const
{
  for ( auto const & require : requireData )
  {
    for ( auto const & enumConstant : require.enumConstants )
    {
      if ( encounteredEnumConstants.insert( enumConstant.name ).second )
      {
        enumConstantAction( enumConstant );
      }
    }
  }
}

template <typename RequireType>
void VulkanHppGenerator::forEachRequiredHandle( std::vector<RequireType> const &                                          requireTypes,
                                                std::function<void( std::pair<std::string, HandleData> const & )> const & handleAction ) const
{
  for ( auto const & require : requireTypes )
  {
    for ( auto const & type : require.types )
    {
      if ( auto handleIt = m_handles.find( type.name ); handleIt != m_handles.end() )
      {
        handleAction( *handleIt );
      }
    }
  }
}

std::set<std::string> VulkanHppGenerator::gatherResultCodes() const
{
  auto resultIt = m_enums.find( "VkResult" );
  assert( resultIt != m_enums.end() );

  std::set<std::string> resultCodes;
  for ( auto rc : resultIt->second.values )
  {
    resultCodes.insert( rc.name );
    std::ranges::for_each( rc.aliases, [&resultCodes]( EnumValueAlias const & eva ) { resultCodes.insert( eva.name ); } );
  }

  return resultCodes;
}

std::tuple<std::string, std::string, std::string, std::string>
  VulkanHppGenerator::generateAllocatorTemplates( std::vector<size_t> const &               returnParams,
                                                  std::vector<std::string> const &          returnDataTypes,
                                                  std::map<size_t, VectorParamData> const & vectorParams,
                                                  std::vector<size_t> const &               chainedReturnParams,
                                                  CommandFlavourFlags                       flavourFlags,
                                                  bool                                      definition ) const
{
  assert( returnParams.size() == returnDataTypes.size() );
  assert( chainedReturnParams.size() <= 1 );

  bool const chained  = flavourFlags & CommandFlavourFlagBits::chained;
  bool const singular = flavourFlags & CommandFlavourFlagBits::singular;
  bool const unique   = flavourFlags & CommandFlavourFlagBits::unique;

  // gather the allocator templates first, to filter out multiple identical allocator templates
  std::vector<std::string> allocatorTemplatesVec, allocatorTemplatesVec20;
  if ( !singular )
  {
    for ( size_t i = 0; i < returnParams.size(); i++ )
    {
      if ( vectorParams.contains( returnParams[i] ) )
      {
        std::string allocatorTemplate, allocatorTemplate20;
        if ( chained && !chainedReturnParams.empty() && ( chainedReturnParams[0] == returnParams[i] ) )
        {
          allocatorTemplate = "typename StructureChainAllocator";
          if ( !definition )
          {
            allocatorTemplate += " = std::allocator<StructureChain>";
          }
          allocatorTemplate20 = "IsAllocator<StructureChain> " + allocatorTemplate.substr( 9 );
        }
        else
        {
          std::string dataType = unique ? ( "UniqueHandle<" + returnDataTypes[i] + ", Dispatch>" ) : returnDataTypes[i];
          allocatorTemplate    = "typename " + startUpperCase( returnDataTypes[i] ) + "Allocator";
          if ( !definition )
          {
            allocatorTemplate += " = std::allocator<" + dataType + ">";
          }
          allocatorTemplate20 = "IsAllocator<" + dataType + "> " + allocatorTemplate.substr( 9 );
        }
        if ( std::ranges::find( allocatorTemplatesVec, allocatorTemplate ) == allocatorTemplatesVec.end() )
        {
          allocatorTemplatesVec.push_back( allocatorTemplate );
          allocatorTemplatesVec20.push_back( allocatorTemplate20 );
        }
      }
    }
  }

  // roll the allocator templates out into a single string
  std::string allocatorTemplates;
  for ( auto const & allocatorTemplate : allocatorTemplatesVec )
  {
    allocatorTemplates += allocatorTemplate + ", ";
  }
  std::string allocatorTemplates20;
  for ( auto const & allocatorTemplate20 : allocatorTemplatesVec20 )
  {
    allocatorTemplates20 += allocatorTemplate20 + ", ";
  }

  std::string uniqueHandleAllocatorTemplates, uniqueHandleAllocatorTemplates20;
  if ( unique && !allocatorTemplates.empty() )
  {
    uniqueHandleAllocatorTemplates = ", " + stripPostfix( allocatorTemplates, ", " );
    allocatorTemplates.clear();
    uniqueHandleAllocatorTemplates20 = ", " + stripPostfix( allocatorTemplates20, ", " );
    allocatorTemplates20.clear();
  }
  return { allocatorTemplates, allocatorTemplates20, uniqueHandleAllocatorTemplates, uniqueHandleAllocatorTemplates20 };
}

std::string VulkanHppGenerator::generateArgumentListEnhanced( std::vector<Param> const &                params,
                                                              std::vector<size_t> const &               returnParams,
                                                              std::map<size_t, VectorParamData> const & vectorParams,
                                                              std::set<size_t> const &                  skippedParams,
                                                              std::set<size_t> const &                  singularParams,
                                                              std::set<size_t> const &                  templatedParams,
                                                              std::vector<size_t> const &               chainedReturnParams,
                                                              bool                                      raii,
                                                              bool                                      definition,
                                                              CommandFlavourFlags                       flavourFlags,
                                                              bool                                      withDispatcher ) const
{
  bool const   withAllocators    = flavourFlags & CommandFlavourFlagBits::withAllocator;
  size_t const defaultStartIndex = withAllocators ? ~0 : determineDefaultStartIndex( params, skippedParams );

  std::vector<std::string> arguments;
  for ( size_t i = 0; i < params.size(); ++i )
  {
    if ( !skippedParams.contains( i ) )
    {
      bool hasDefaultAssignment = false;

      std::string composedType = params[i].type.compose( "Vk", ( raii && m_handles.contains( params[i].type.name ) ) ? "VULKAN_HPP_NAMESPACE" : "" );

      if ( singularParams.contains( i ) )
      {
        assert( !isOptional( params[i].optional ) );
        assert( params[i].type.isConstPointer() && params[i].altLen.empty() && !params[i].len.empty() && !isLenByStructMember( params[i].len, params ) &&
                params[i].type.name.starts_with( "Vk" ) );
        assert( composedType.ends_with( " *" ) );
        arguments.push_back( stripPostfix( composedType, " *" ) + " & " + stripPluralS( startLowerCase( stripPrefix( params[i].name, "p" ) ) ) );
      }
      else if ( !params[i].arraySizes.empty() )
      {
        assert( params[i].arraySizes.size() == 1 );
        assert( params[i].type.postfix == "const" );
        arguments.push_back( "std::array<" + stripPrefix( params[i].type.name, "Vk" ) + ", " + params[i].arraySizes[0] + "> const & " + params[i].name );
      }
      else if ( params[i].type.isConstPointer() )
      {
        assert( composedType.ends_with( " *" ) );
        std::string name = startLowerCase( stripPrefix( params[i].name, "p" ) );
        if ( params[i].len.empty() )
        {
          assert( withDispatcher || !isHandleType( params[i].type.name ) );
          if ( params[i].type.name == "void" )
          {
            arguments.push_back( templatedParams.contains( i ) ? ( stripPrefix( params[i].name, "p" ) + "Type const & " + name )
                                                               : ( composedType + " " + params[i].name ) );
          }
          else if ( isOptional( params[i].optional ) )
          {
            arguments.push_back( "Optional<" + stripPostfix( composedType, " *" ) + "> " + name +
                                 ( ( definition || withAllocators ) ? "" : " VULKAN_HPP_DEFAULT_ASSIGNMENT( nullptr )" ) );
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
          if ( params[i].len == "null-terminated" )
          {
            assert( params[i].type.name == "char" );
            if ( isOptional( params[i].optional ) )
            {
              arguments.push_back( "Optional<std::string const> " + name +
                                   ( ( definition || withAllocators ) ? "" : " VULKAN_HPP_DEFAULT_ASSIGNMENT( nullptr )" ) );
              hasDefaultAssignment = true;
            }
            else
            {
              arguments.push_back( "std::string const & " + name );
            }
          }
          else
          {
            // an ArrayProxy also covers no data, so any optional flag can be ignored here
            std::string  type = stripPostfix( composedType, " *" );
            size_t const pos  = type.find( "void" );
            if ( pos != std::string::npos )
            {
              type.replace( pos, 4, stripPrefix( params[i].name, "p" ) + "Type" );
            }
            arguments.push_back( std::string( params[i].stride.empty() ? "" : "Strided" ) + "ArrayProxy<" + type + "> const & " + name );
            if ( isOptional( params[i].optional ) && !definition )
            {
              assert( params[i].stride.empty() );
              arguments.back() += " VULKAN_HPP_DEFAULT_ASSIGNMENT( nullptr )";
              hasDefaultAssignment = true;
            }
          }
        }
      }
      else if ( params[i].type.isNonConstPointer() )
      {
        if ( ( params[i].type.name == "void" ) && ( flavourFlags & CommandFlavourFlagBits::keepVoidPtr ) )
        {
          arguments.push_back( composedType + " " + params[i].name );
        }
        else
        {
          assert( withDispatcher || !isHandleType( params[i].type.name ) );
          assert( params[i].len.empty() && !isOptional( params[i].optional ) );
          assert( composedType.ends_with( " *" ) );
          arguments.push_back( stripPostfix( composedType, " *" ) + " & " + startLowerCase( stripPrefix( params[i].name, "p" ) ) );
        }
      }
      else
      {
        assert( params[i].type.isValue() );
        // parameters named "objectType" collide with the member variable -> append an _ to here
        std::string paramName = ( definition && ( params[i].name == "objectType" ) ) ? "objectType_" : params[i].name;
        arguments.push_back( composedType + " " + paramName );
      }
      arguments.back() += std::string( !definition && ( defaultStartIndex <= i ) && !hasDefaultAssignment ? " VULKAN_HPP_DEFAULT_ASSIGNMENT( {} )" : "" );
    }
  }
  if ( withAllocators )
  {
    if ( ( flavourFlags & CommandFlavourFlagBits::chained ) && needsStructureChainResize( vectorParams, chainedReturnParams ) )
    {
      arguments.push_back( "StructureChainAllocator const & structureChainAllocator" );
    }
    else
    {
      std::set<std::string> allocatorTypes;
      for ( auto sp : skippedParams )
      {
        if ( !params[sp].len.empty() )
        {
          std::string type;
          if ( templatedParams.contains( sp ) )
          {
            auto vectorParamIt = vectorParams.find( sp );
            if ( ( vectorParamIt != vectorParams.end() ) &&
                 std::ranges::any_of( returnParams, [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->first; } ) &&
                 std::ranges::any_of( returnParams, [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->second.lenParam; } ) )
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
            type = ( params[sp].type.name == "void" ) ? "Uint8_t" : startUpperCase( stripPrefix( params[sp].type.name, "Vk" ) );
          }
          if ( allocatorTypes.insert( type ).second )
          {
            arguments.push_back( type + "Allocator const & " + startLowerCase( type ) + "Allocator" );
          }
        }
        else if ( vectorParams.contains( sp ) && vectorParams.find( sp )->second.byStructure )
        {
          std::string type = stripPrefix( vectorMemberByStructure( params[sp].type.name ).type.name, "Vk" );
          assert( isupper( type[0] ) );
          if ( allocatorTypes.insert( type ).second )
          {
            arguments.push_back( type + "Allocator const & " + startLowerCase( type ) + "Allocator" );
          }
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

std::string VulkanHppGenerator::generateArgumentListStandard(
  std::vector<Param> const & params, std::set<size_t> const & skippedParams, bool definition, bool raii, bool withDispatcher ) const
{
  std::string argumentList;
  for ( size_t i = 0; i < params.size(); ++i )
  {
    if ( !skippedParams.contains( i ) )
    {
      // parameters named "objectType" collide with the member variable -> append an _ to here
      std::string paramName = ( definition && ( params[i].name == "objectType" ) ) ? "objectType_" : params[i].name;
      argumentList += params[i].type.compose( "Vk", ( raii && isHandleType( params[i].type.name ) ? "VULKAN_HPP_NAMESPACE" : "" ) ) + " " + paramName +
                      generateCArraySizes( params[i].arraySizes ) + ", ";
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

std::string VulkanHppGenerator::generateArgumentTemplates( std::vector<Param> const &                params,
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
           std::ranges::none_of( returnParams, [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->first; } ) ||
           std::ranges::none_of( returnParams, [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->second.lenParam; } ) )
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
  assert( !m_vkxml.types.baseTypes.empty() );
  std::string const basetypesTemplate = R"(
  //==================
  //=== BASE TYPEs ===
  //==================

${basetypes}
)";

  std::string basetypes;
  for ( auto const & baseType : m_vkxml.types.baseTypes )
  {
    // filter out VkFlags and VkFlags64, as they are mapped to our own Flags class, and basetypes without any type information
    if ( ( baseType.name != "VkFlags" ) && ( baseType.name != "VkFlags64" ) && !baseType.type.name.empty() )
    {
      basetypes += "  using " + stripPrefix( baseType.name, "Vk" ) + " = " + baseType.type.compose( "" ) + ";\n";
    }
  }

  return replaceWithMap( basetypesTemplate, { { "basetypes", basetypes } } );
}

std::string VulkanHppGenerator::generateBitmask( std::vector<TypeBitmask>::const_iterator bitmaskIt, std::string const & surroundingProtect ) const
{
  auto bitmaskBitsIt = m_enums.find( bitmaskIt->require );
  assert( bitmaskBitsIt != m_enums.end() );

  std::string bitmaskName = stripPrefix( bitmaskIt->name, "Vk" );
  std::string enumName    = stripPrefix( bitmaskBitsIt->first, "Vk" );

  std::string allFlags, wrappedType;
  if ( bitmaskBitsIt->second.values.empty() || std::ranges::none_of( bitmaskBitsIt->second.values, []( auto const & evd ) { return evd.supported; } ) )
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

    wrappedType = "using WrappedType = " + bitmaskBitsIt->first + ";";
  }

  std::string aliases;
  for ( auto const & a : bitmaskIt->aliases )
  {
    aliases += "  using " + stripPrefix( a.first, "Vk" ) + " = " + bitmaskName + ";\n";
  }

  static std::string const bitmaskTemplate = R"(
  // wrapper using for bitmask Vk${bitmaskName}, see https://registry.khronos.org/vulkan/specs/latest/man/html/Vk${bitmaskName}.html
  using ${bitmaskName} = Flags<${enumName}>;
${aliases}

  template <> struct FlagTraits<${enumName}>
  {
    ${WrappedType}
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool isBitmask = true;
    static VULKAN_HPP_CONST_OR_CONSTEXPR ${bitmaskName} allFlags = ${allFlags}
  };
)";

  return replaceWithMap(
    bitmaskTemplate,
    { { "aliases", aliases }, { "allFlags", allFlags }, { "bitmaskName", bitmaskName }, { "enumName", enumName }, { "WrappedType", wrappedType } } );
}

std::string VulkanHppGenerator::generateBitmasksToString() const
{
  std::string const bitmasksToStringTemplate = R"(
  //==========================
  //=== BITMASKs to_string ===
  //==========================

${bitmasksToString}
)";

  std::string           bitmasksToString;
  std::set<std::string> listedBitmasks;
  for ( auto const & feature : m_vkxml.features )
  {
    bitmasksToString += generateBitmasksToString( feature.require, listedBitmasks, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    bitmasksToString += generateBitmasksToString( extension.requireData, listedBitmasks, extension.name );
  }

  return replaceWithMap( bitmasksToStringTemplate, { { "bitmasksToString", bitmasksToString } } );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateBitmasksToString( std::vector<RequireType> const & requireTypes,
                                                          std::set<std::string> &          listedBitmasks,
                                                          std::string const &              title ) const
{
  std::string str;
  forEachRequiredBitmask( requireTypes, listedBitmasks, [&str, this]( TypeBitmask const & bitmask ) { str += generateBitmaskToString( bitmask ); } );
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateBitmaskToString( TypeBitmask const & bitmask ) const
{
  auto bitmaskBitsIt = m_enums.find( bitmask.require );
  assert( bitmaskBitsIt != m_enums.end() );

  std::string bitmaskName = stripPrefix( bitmask.name, "Vk" );
  std::string enumName    = stripPrefix( bitmaskBitsIt->first, "Vk" );

  std::string str;
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
        auto const [enter, leave] = generateProtection( value.protect );
        toStringChecks += ( ( previousEnter != enter ) ? ( previousLeave + enter ) : "" ) + "    if ( value & " + enumName + "::" + valueName +
                          " ) result += \" " + valueName.substr( 1 ) + " |\";\n";
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

  if ( toStringChecks.empty() )
  {
    static std::string const bitmaskToStringTemplate = R"(
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 std::string to_string( ${bitmaskName} )
  {
    return "${emptyValue}";
  }
)";
    str += replaceWithMap( bitmaskToStringTemplate, { { "bitmaskName", bitmaskName }, { "emptyValue", emptyValue } } );
  }
  else
  {
    static std::string const bitmaskToStringTemplate = R"(
  VULKAN_HPP_INLINE std::string to_string( ${bitmaskName} value )
  {
    std::string result = "{";
${toStringChecks}
    if ( result.size() > 1 )
      result.back() = '}';
    else
      result = "${emptyValue}";
    return result;
  }
)";
    str += replaceWithMap( bitmaskToStringTemplate, { { "bitmaskName", bitmaskName }, { "emptyValue", emptyValue }, { "toStringChecks", toStringChecks } } );
  }

  return str;
}

std::string VulkanHppGenerator::generateCallArgumentsEnhanced( Command const &          command,
                                                               size_t                   initialSkipCount,
                                                               bool                     nonConstPointerAsNullptr,
                                                               std::set<size_t> const & singularParams,
                                                               std::set<size_t> const & templatedParams,
                                                               bool                     raii,
                                                               bool                     raiiFactory,
                                                               CommandFlavourFlags      flavourFlags ) const
{
  assert( initialSkipCount <= command.params.size() );
  std::vector<std::string> arguments;
  if ( raii )
  {
    switch ( initialSkipCount )
    {
      case 0: break;
      case 1:
        assert( isHandleType( command.params[0].type.name ) && command.params[0].type.isValue() );
        assert( command.params[0].arraySizes.empty() && command.params[0].len.empty() );
        arguments.push_back( "static_cast<" + command.params[0].type.name + ">( m_" + startLowerCase( stripPrefix( command.params[0].type.name, "Vk" ) ) +
                             " )" );
        break;
      case 2:
        {
          assert( isHandleType( command.params[0].type.name ) && command.params[0].type.isValue() );
          assert( command.params[0].arraySizes.empty() && command.params[0].len.empty() );
          arguments.push_back( "static_cast<" + command.params[0].type.name + ">( m_" + startLowerCase( stripPrefix( command.params[0].type.name, "Vk" ) ) +
                               " )" );

          assert( isHandleType( command.params[1].type.name ) && command.params[1].type.isValue() );
          assert( command.params[1].arraySizes.empty() && command.params[1].len.empty() );
          arguments.push_back( "static_cast<" + command.params[1].type.name + ">( m_" + startLowerCase( stripPrefix( command.params[1].type.name, "Vk" ) ) +
                               " )" );
        }
        break;
      default: assert( false ); break;
    }
  }
  else
  {
    for ( size_t i = 0; i < initialSkipCount; ++i )
    {
      assert( isHandleType( command.params[i].type.name ) && command.params[i].type.isValue() );
      assert( command.params[i].arraySizes.empty() && command.params[i].len.empty() );
      arguments.push_back( "static_cast<" + command.params[i].type.name + ">( m_" + startLowerCase( stripPrefix( command.params[i].type.name, "Vk" ) ) + " )" );
    }
  }
  for ( size_t i = initialSkipCount; i < command.params.size(); ++i )
  {
    arguments.push_back(
      generateCallArgumentEnhanced( command.params, i, nonConstPointerAsNullptr, singularParams, templatedParams, flavourFlags, raiiFactory ) );
  }
  return generateList( arguments, "", ", " );
}

std::string VulkanHppGenerator::generateCallArgumentsStandard( std::vector<Param> const & params, size_t initialSkipCount ) const
{
  std::vector<std::string> arguments;
  for ( size_t i = 0; i < initialSkipCount; ++i )
  {
    auto const & param = params[i];
    assert( isHandleType( param.type.name ) && param.type.isValue() );
    assert( param.arraySizes.empty() && param.len.empty() );
    arguments.push_back( "static_cast<" + param.type.name + ">( m_" + startLowerCase( stripPrefix( param.type.name, "Vk" ) ) + " )" );
  }
  for ( size_t i = initialSkipCount; i < params.size(); ++i )
  {
    auto const & param    = params[i];
    std::string  argument = param.name;
    if ( param.type.name.starts_with( "Vk" ) )
    {
      if ( !param.arraySizes.empty() )
      {
        assert( param.arraySizes.size() == 1 );
        assert( param.type.isValue() );
        assert( param.type.postfix == "const" );
        argument = "reinterpret_cast<" + param.type.compose( "" ) + " *>( " + argument + " )";
      }
      else if ( param.type.isValue() )
      {
        // parameters named "objectType" collide with the member variable -> append an _ to here
        if ( argument == "objectType" )
        {
          argument += "_";
        }
        argument = "static_cast<" + param.type.name + ">( " + argument + " )";
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

std::string VulkanHppGenerator::generateCallArgumentEnhanced( std::vector<Param> const & params,
                                                              size_t                     paramIndex,
                                                              bool                       nonConstPointerAsNullptr,
                                                              std::set<size_t> const &   singularParams,
                                                              std::set<size_t> const &   templatedParams,
                                                              CommandFlavourFlags        flavourFlags,
                                                              bool                       raiiFactory ) const
{
  std::string   argument;
  Param const & param = params[paramIndex];
  if ( param.type.isConstPointer() || specialPointerTypes.contains( param.type.name ) )
  {
    // parameter is a const-pointer or one of the special pointer types that are considered to be const-pointers
    argument = generateCallArgumentEnhancedConstPointer( param, paramIndex, singularParams, templatedParams );
  }
  else if ( param.type.isNonConstPointer() && !specialPointerTypes.contains( param.type.name ) &&
            !( ( param.type.name == "void" ) && ( flavourFlags & CommandFlavourFlagBits::keepVoidPtr ) ) )
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

std::string VulkanHppGenerator::generateCallArgumentEnhancedConstPointer( Param const &            param,
                                                                          size_t                   paramIndex,
                                                                          std::set<size_t> const & singularParams,
                                                                          std::set<size_t> const & templatedParams ) const
{
  std::string argument;
  std::string name = startLowerCase( stripPrefix( param.name, "p" ) );
  if ( isHandleType( param.type.name ) && param.type.isValue() )
  {
    assert( !isOptional( param.optional ) );
    // if at all, this is the first argument, and it's the implicitly provided member handle
    assert( paramIndex == 0 );
    assert( param.arraySizes.empty() && param.len.empty() );
    argument = "m_" + startLowerCase( stripPrefix( param.type.name, "Vk" ) );
  }
  else if ( param.len.empty() )
  {
    // this const-pointer parameter has no length, that is it's a const-pointer to a single value
    if ( param.type.name == "void" )
    {
      argument =
        templatedParams.contains( paramIndex ) ? "reinterpret_cast<" + param.type.compose( "Vk", "VULKAN_HPP_NAMESPACE" ) + ">( &" + name + " )" : param.name;
    }
    else if ( isOptional( param.optional ) )
    {
      argument = name + ".get()";
    }
    else
    {
      argument = "&" + name;
    }
    if ( param.type.name.starts_with( "Vk" ) )
    {
      argument = "reinterpret_cast<" + param.type.compose( "" ) + ">( " + argument + " )";
    }
  }
  else if ( param.len == "null-terminated" )
  {
    // this const-pointer parameter is "null-terminated", that is it's a string
    assert( ( param.type.name == "char" ) && param.arraySizes.empty() );
    if ( isOptional( param.optional ) )
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
      assert( !isOptional( param.optional ) );
      argument = "&" + stripPluralS( name );
    }
    else
    {
      // this const-parameter is represented by some array, where data() also works with no data (optional)
      argument = name + ".data()";
    }
    if ( param.type.name.starts_with( "Vk" ) || ( param.type.name == "void" ) )
    {
      argument = "reinterpret_cast<" + param.type.compose( "" ) + ">( " + argument + " )";
    }
  }
  return argument;
}

std::string VulkanHppGenerator::generateCallArgumentEnhancedNonConstPointer( Param const &            param,
                                                                             size_t                   paramIndex,
                                                                             bool                     nonConstPointerAsNullptr,
                                                                             std::set<size_t> const & singularParams ) const
{
  std::string argument;
  std::string name = startLowerCase( stripPrefix( param.name, "p" ) );
  if ( param.len.empty() || ( param.len == "1" ) )
  {
    assert( param.arraySizes.empty() );
    if ( param.type.name.starts_with( "Vk" ) )
    {
      argument = "reinterpret_cast<" + param.type.compose( "" ) + ">( &" + name + " )";
    }
    else
    {
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
      if ( singularParams.contains( paramIndex ) )
      {
        argument = "&" + stripPluralS( name );
      }
      else
      {
        // get the data of the array, which also covers no data -> no need to look at param.optional
        argument = name + ".data()";
      }
      if ( param.type.name.starts_with( "Vk" ) || ( param.type.name == "void" ) )
      {
        argument = "reinterpret_cast<" + param.type.compose( "" ) + ">( " + argument + " )";
      }
    }
  }
  return argument;
}

std::string VulkanHppGenerator::generateCallArgumentEnhancedValue(
  std::vector<Param> const & params, size_t paramIndex, std::set<size_t> const & singularParams, CommandFlavourFlags flavourFlags, bool raiiFactory ) const
{
  std::string   argument;
  Param const & param = params[paramIndex];
  assert( ( flavourFlags & CommandFlavourFlagBits::keepVoidPtr ) || param.len.empty() );  // !keepVoidPtr -> no len
  if ( param.arraySizes.empty() )
  {
    if ( param.type.name.starts_with( "Vk" ) )
    {
      // check if this param is used as the len of an other param
      auto pointerIt = std::ranges::find_if( params, [&param]( Param const & pd ) { return pd.len == param.name; } );
      if ( pointerIt != params.end() )
      {
        assert( !isOptional( param.optional ) );
        argument = startLowerCase( stripPrefix( pointerIt->name, "p" ) ) + ".size()";
        if ( pointerIt->type.name == "void" )
        {
          argument += " * sizeof( " + stripPrefix( pointerIt->name, "p" ) + "Type )";
        }
      }
      else if ( raiiFactory && m_handles.contains( param.type.name ) )
      {
        assert( param.type.isValue() && param.len.empty() );
        if ( isOptional( param.optional ) )
        {
          argument = param.name + " ? static_cast<" + param.type.name + ">( **" + param.name + " ) : 0";
        }
        else
        {
          argument = "static_cast<" + param.type.name + ">( *" + param.name + " )";
        }
      }
      else
      {
        assert( param.type.isValue() );
        // parameters named "objectType" collide with the member variable -> append an _ to here
        std::string paramName = ( param.name == "objectType" ) ? "objectType_" : param.name;
        argument              = "static_cast<" + param.type.name + ">( " + paramName + " )";
      }
      // assert that none of the params uses this param as a stride
      assert( std::ranges::none_of( params, [&params, &paramIndex]( Param const & pd ) { return determineParamIndex( params, pd.stride ) == paramIndex; } ) );
    }
    else
    {
      auto pointerIt = std::ranges::find_if( params, [&param]( Param const & pd ) { return pd.len == param.name; } );
      if ( ( pointerIt != params.end() ) && !( ( pointerIt->type.name == "void" ) && ( flavourFlags & CommandFlavourFlagBits::keepVoidPtr ) ) )
      {
        // this parameter is the len of some other -> replace it with that parameter's size
        assert( ( param.type.name == "size_t" ) || ( param.type.name == "uint32_t" ) );
        if ( singularParams.contains( paramIndex ) )
        {
          if ( pointerIt->type.name == "void" )
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
          if ( pointerIt->type.name == "void" )
          {
            argument += " * sizeof( " + stripPrefix( pointerIt->name, "p" ) + "Type )";
          }
        }
      }
      else
      {
        pointerIt = std::ranges::find_if( params, [&paramName = param.name]( Param const & pd ) { return pd.stride == paramName; } );
        if ( pointerIt != params.end() )
        {
          // this parameter is the stride of some other -> replace it with that parameter's stride
          assert( param.type.name == "uint32_t" );
          argument = startLowerCase( stripPrefix( pointerIt->name, "p" ) ) + ".stride()";
        }
        else
        {
          argument = param.name;
        }
      }
    }
  }
  else
  {
    assert( !isOptional( param.optional ) );
    assert( param.arraySizes.size() == 1 );
    assert( param.type.postfix == "const" );
    argument = param.type.name.starts_with( "Vk" ) ? ( "reinterpret_cast<" + param.type.compose( "" ) + " *>( " + param.name + ".data() )" )
                                                   : ( param.name + ".data()" );
  }
  return argument;
}

std::string VulkanHppGenerator::generateCallSequence( std::string const &                       name,
                                                      Command const &                           command,
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
    auto paramIt = std::ranges::find_if( command.params, []( Param const & pd ) { return pd.name == "pCreateInfo"; } );
    assert( paramIt != command.params.end() && ( paramIt->type.name == "VkPipelineBinaryCreateInfoKHR" ) );
    {
      auto structIt = findByName( m_vkxml.types.structs, "VkPipelineBinaryCreateInfoKHR" );
      assert( structIt != m_vkxml.types.structs.end() );
      assert( std::ranges::any_of( structIt->members, []( StructMember const & member ) { return member.name == "pipeline"; } ) &&
              std::ranges::any_of( structIt->members, []( StructMember const & member ) { return member.name == "pPipelineCreateInfo"; } ) );
      auto memberIt = std::ranges::find_if( structIt->members, []( StructMember const & member ) { return member.name == "pKeysAndDataInfo"; } );
      assert( memberIt != structIt->members.end() && ( memberIt->type.name == "VkPipelineBinaryKeysAndDataKHR" ) );
    }
    {
      auto structIt = findByName( m_vkxml.types.structs, "VkPipelineBinaryKeysAndDataKHR" );
      assert( structIt != m_vkxml.types.structs.end() );
      assert( std::ranges::any_of( structIt->members, []( StructMember const & member ) { return member.name == "binaryCount"; } ) );
    }
    {
      auto structIt = findByName( m_vkxml.types.structs, "VkPipelineBinaryHandlesInfoKHR" );
      assert( structIt != m_vkxml.types.structs.end() );
      assert( std::ranges::any_of( structIt->members, []( StructMember const & member ) { return member.name == "pipelineBinaryCount"; } ) &&
              std::ranges::any_of( structIt->members, []( StructMember const & member ) { return member.name == "pPipelineBinaries"; } ) );
    }
#endif

    std::string const callSequenceTemplate = R"(    Result result_;
    if ( createInfo.pKeysAndDataInfo )
    {
      VULKAN_HPP_ASSERT( !createInfo.pipeline && !createInfo.pPipelineCreateInfo );
      pipelineBinaries.resize( createInfo.pKeysAndDataInfo->binaryCount );
      binaries.pipelineBinaryCount = createInfo.pKeysAndDataInfo->binaryCount;
      binaries.pPipelineBinaries = pipelineBinaries.data();
      result_ = static_cast<Result>( ${dispatcher}${vkCommand}( ${callArguments} ) );
    }
    else
    {
      VULKAN_HPP_ASSERT( !createInfo.pipeline ^ !createInfo.pPipelineCreateInfo );
      result_ = static_cast<Result>( ${dispatcher}${vkCommand}( ${callArguments} ) );
      if ( result_ == Result::eSuccess )
      {
        pipelineBinaries.resize( binaries.pipelineBinaryCount );
        binaries.pPipelineBinaries = pipelineBinaries.data();
        result_ = static_cast<Result>( ${dispatcher}${vkCommand}( ${callArguments} ) );
      }
    }
)";

    std::string callArguments =
      generateCallArgumentsEnhanced( command, initialSkipCount, false, singularParams, templatedParams, raii, raiiFactory, flavourFlags );

    return replaceWithMap( callSequenceTemplate, { { "callArguments", callArguments }, { "dispatcher", dispatcher }, { "vkCommand", name } } );
  }

  // if at least one returnParam is a size value of a vector param (and no singular params), we need two calls
  if ( singularParams.empty() &&
       std::ranges::any_of( returnParams,
                            [&vectorParams]( size_t rp )
                            { return std::ranges::any_of( vectorParams, [rp]( auto const & vp ) { return vp.second.lenParam == rp; } ); } ) )
  {
    auto vectorParamIt = vectorParams.begin();
    assert( std::ranges::any_of( returnParams, [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->first; } ) );
    assert( std::ranges::any_of( returnParams, [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->second.lenParam; } ) );

    std::string firstCallArguments  = generateCallArgumentsEnhanced( command, initialSkipCount, true, {}, templatedParams, raii, raiiFactory, flavourFlags );
    std::string secondCallArguments = generateCallArgumentsEnhanced( command, initialSkipCount, false, {}, templatedParams, raii, raiiFactory, flavourFlags );
    std::string vectorName, vectorSize;
    if ( vectorParamIt->second.byStructure )
    {
      auto structIt = findByName( m_vkxml.types.structs, command.params[vectorParamIt->first].type.name );
      assert( structIt != m_vkxml.types.structs.end() );
      vectorName = structIt->members.back().name;
      vectorSize =
        startLowerCase( stripPrefix( command.params[vectorParamIt->first].name, "p" ) ) + "." + structIt->members[vectorParamIt->second.lenParam].name;
    }
    else
    {
      vectorName = command.params[vectorParamIt->first].name;
      vectorSize = startLowerCase( stripPrefix( command.params[vectorParamIt->second.lenParam].name, "p" ) );
    }
    vectorName = startLowerCase( stripPrefix( vectorName, "p" ) );

    if ( ( flavourFlags & CommandFlavourFlagBits::chained ) && needsStructureChainResize( vectorParams, chainedReturnParams ) )
    {
      assert( vectorParams.size() == 1 );
      // chained data needs some more handling!!
      std::string vectorElementType = stripPostfix( command.params[vectorParamIt->first].type.compose( "Vk" ), " *" );

      if ( command.returnType.name == "VkResult" )
      {
        std::string const callSequenceTemplate = R"(Result result_;
    do
    {
      result_ = static_cast<Result>( ${dispatcher}${vkCommand}( ${firstCallArguments} ) );
      if ( ( result_ == Result::eSuccess ) && ${counterName} )
      {
        structureChains.resize( ${counterName} );
        ${vectorName}.resize( ${counterName} );
        for ( ${counterType} i = 0; i < ${counterName}; i++ )
        {
          ${vectorName}[i].pNext = structureChains[i].template get<${vectorElementType}>().pNext;
        }
        result_ = static_cast<Result>( ${dispatcher}${vkCommand}( ${secondCallArguments} ) );
      }
    } while ( result_ == Result::eIncomplete );)";

        return replaceWithMap( callSequenceTemplate,
                               { { "counterName", startLowerCase( stripPrefix( command.params[vectorParamIt->second.lenParam].name, "p" ) ) },
                                 { "counterType", command.params[vectorParamIt->second.lenParam].type.name },
                                 { "dispatcher", dispatcher },
                                 { "firstCallArguments", firstCallArguments },
                                 { "secondCallArguments", secondCallArguments },
                                 { "vectorElementType", vectorElementType },
                                 { "vectorName", vectorName },
                                 { "vkCommand", name } } );
      }
      else
      {
        std::string const callSequenceTemplate =
          R"(${dispatcher}${vkCommand}( ${firstCallArguments} );
    structureChains.resize( ${counterName} );
    ${vectorName}.resize( ${counterName} );
    for ( ${counterType} i = 0; i < ${counterName}; i++ )
    {
      ${vectorName}[i].pNext = structureChains[i].template get<${vectorElementType}>().pNext;
    }
    ${dispatcher}${vkCommand}( ${secondCallArguments} );)";

        return replaceWithMap( callSequenceTemplate,
                               { { "counterName", startLowerCase( stripPrefix( command.params[vectorParamIt->second.lenParam].name, "p" ) ) },
                                 { "counterType", command.params[vectorParamIt->second.lenParam].type.name },
                                 { "dispatcher", dispatcher },
                                 { "firstCallArguments", firstCallArguments },
                                 { "secondCallArguments", secondCallArguments },
                                 { "vectorElementType", vectorElementType },
                                 { "vectorName", vectorName },
                                 { "vkCommand", name } } );
      }
    }
    else if ( command.returnType.name == "VkResult" )
    {
      std::string resizes;
      for ( auto const & vp : vectorParams )
      {
        assert( std::ranges::any_of( returnParams, [&vp]( size_t rp ) { return rp == vp.first; } ) &&
                std::ranges::any_of( returnParams, [&vp]( size_t rp ) { return rp == vp.second.lenParam; } ) );
        resizes += startLowerCase( stripPrefix( command.params[vp.first].name, "p" ) ) + ".resize( " +
                   startLowerCase( stripPrefix( command.params[vp.second.lenParam].name, "p" ) ) + " );\n";
      }
      resizes.pop_back();

      switch ( command.successCodes.size() )
      {
        case 1:
          assert( command.successCodes[0] == "VK_SUCCESS" );
          if ( command.errorCodes.empty() )
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
              R"(Result result_ = static_cast<Result>( ${dispatcher}${vkCommand}( ${firstCallArguments} ) );
      if ( result_ == Result::eSuccess )
      {
        ${resizes}
        result_ = static_cast<Result>( ${dispatcher}${vkCommand}( ${secondCallArguments} ) );
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
            assert( ( command.successCodes[0] == "VK_SUCCESS" ) && ( command.successCodes[1] == "VK_INCOMPLETE" ) );
            std::string const callSequenceTemplate = R"(Result result_;
    do
    {
      result_ = static_cast<Result>( ${dispatcher}${vkCommand}( ${firstCallArguments} ) );
      if ( ( result_ == Result::eSuccess ) && ${counterName} )
      {
        ${resizes}
        result_ = static_cast<Result>( ${dispatcher}${vkCommand}( ${secondCallArguments} ) );
      }
    } while ( result_ == Result::eIncomplete );)";

            return replaceWithMap( callSequenceTemplate,
                                   { { "counterName", startLowerCase( stripPrefix( command.params[vectorParamIt->second.lenParam].name, "p" ) ) },
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
                                               { "structName", startLowerCase( stripPrefix( command.params[vectorParamIt->first].name, "p" ) ) },
                                               { "vectorName", vectorName },
                                               { "vectorSize", vectorSize } } );
      }
      else
      {
        std::string const resizeInstructionTemplate = R"(${vectorName}.resize( ${vectorSize} );)";
        resizeInstructions = replaceWithMap( resizeInstructionTemplate, { { "vectorName", vectorName }, { "vectorSize", vectorSize } } );
      }

      // no need to enumerate here, just two calls
      assert( command.returnType.name == "void" );
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
  else if ( ( command.returnType.name == "VkResult" ) && ( ( 1 < command.successCodes.size() ) || !command.errorCodes.empty() ) )
  {
    std::string const callSequenceTemplate = R"(Result result_ = static_cast<Result>( ${dispatcher}${vkCommand}( ${callArguments} ) );)";

    std::string callArguments =
      generateCallArgumentsEnhanced( command, initialSkipCount, false, singularParams, templatedParams, raii, raiiFactory, flavourFlags );

    return replaceWithMap( callSequenceTemplate, { { "callArguments", callArguments }, { "dispatcher", dispatcher }, { "vkCommand", name } } );
  }
  else
  {
    std::string const callSequenceTemplate = R"(${resultAssignment}${dispatcher}${vkCommand}( ${callArguments} );)";

    std::string callArguments =
      generateCallArgumentsEnhanced( command, initialSkipCount, false, singularParams, templatedParams, raii, raiiFactory, flavourFlags );
    std::string resultAssignment = generateResultAssignment( command );

    return replaceWithMap(
      callSequenceTemplate,
      { { "callArguments", callArguments }, { "dispatcher", dispatcher }, { "resultAssignment", resultAssignment }, { "vkCommand", name } } );
  }
}

std::string VulkanHppGenerator::generateCommand( std::string const & name, Command const & command, size_t initialSkipCount, bool definition, bool raii ) const
{
  std::string cmd;

  if ( name == "vkGetDescriptorEXT" )
  {
    // special handling for this function: do not return the void*, but keep its argument as it is
    return generateCommandSetInclusive( name,
                                        command,
                                        initialSkipCount,
                                        definition,
                                        { 3 },
                                        determineVectorParams( command.params ),
                                        false,
                                        { CommandFlavourFlagBits::noReturn | CommandFlavourFlagBits::keepVoidPtr, CommandFlavourFlagBits::singular },
                                        raii,
                                        false,
                                        { CommandFlavourFlagBits::noReturn | CommandFlavourFlagBits::keepVoidPtr, CommandFlavourFlagBits::singular } );
  }
  else if ( name == "vkGetDeviceFaultInfoEXT" )
  {
    // can't generate an enhanced version for such a complex command! Just use the standard version
    return generateCommandSetInclusive( name, command, initialSkipCount, definition, { 1, 2 }, {}, false, {}, raii, false, {} );
  }
  else if ( name == "vkGetMemoryHostPointerPropertiesEXT" )
  {
    // special handling for this function: need to keep the void* argument as a void*!
    cmd = generateCommandSetInclusive( name,
                                       command,
                                       initialSkipCount,
                                       definition,
                                       { 3 },
                                       {},
                                       false,
                                       { CommandFlavourFlagBits::enhanced | CommandFlavourFlagBits::keepVoidPtr },
                                       raii,
                                       false,
                                       { CommandFlavourFlagBits::enhanced | CommandFlavourFlagBits::keepVoidPtr } );
  }
  else
  {
    std::vector<size_t> returnParams = determineReturnParams( command.params );
    switch ( returnParams.size() )
    {
      case 0: cmd = generateCommand0Returns( name, command, initialSkipCount, definition, raii ); break;
      case 1: cmd = generateCommand1Returns( name, command, initialSkipCount, definition, raii, returnParams[0] ); break;
      case 2: cmd = generateCommand2Returns( name, command, initialSkipCount, definition, raii, returnParams ); break;
      case 3: cmd = generateCommand3Returns( name, command, initialSkipCount, definition, raii, returnParams ); break;
    }
  }

  if ( cmd.empty() )
  {
    throw std::runtime_error( "Never encountered a function like <" + name + "> !" );
  }

  extendSpecialCommands( name, definition, raii, cmd );

  return cmd;
}

std::string
  VulkanHppGenerator::generateCommand0Returns( std::string const & name, Command const & command, size_t initialSkipCount, bool definition, bool raii ) const
{
  if ( hasPointerParams( command.params ) )
  {
    // command has pointer parameters
    return generateCommandSetInclusive( name,
                                        command,
                                        initialSkipCount,
                                        definition,
                                        {},
                                        determineVectorParams( command.params ),
                                        false,
                                        { CommandFlavourFlagBits::enhanced },
                                        raii,
                                        false,
                                        { CommandFlavourFlagBits::enhanced } );
  }
  else if ( command.returnType.name != "VkResult" )
  {
    // command has no pointer parameters and does not return a VkResult
    return generateCommandSetInclusive( name, command, initialSkipCount, definition, {}, {}, false, {}, raii, false, { CommandFlavourFlagBits::enhanced } );
  }
  else
  {
    // command has no pointer parameters and returns a VkResult
    return generateCommandSetExclusive( name, command, initialSkipCount, definition, raii );
  }
}

std::string VulkanHppGenerator::generateCommand1Returns(
  std::string const & name, Command const & command, size_t initialSkipCount, bool definition, bool raii, size_t returnParam ) const
{
  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( command.params );
  if ( vectorParams.contains( returnParam ) )
  {
    return generateCommand1ReturnsVector( name, command, initialSkipCount, definition, raii, returnParam, vectorParams );
  }
  else
  {
    return generateCommand1ReturnsValue( name, command, initialSkipCount, definition, raii, returnParam, vectorParams );
  }
}

std::string VulkanHppGenerator::generateCommand1ReturnsStruct( std::string const &                       name,
                                                               Command const &                           command,
                                                               size_t                                    initialSkipCount,
                                                               bool                                      definition,
                                                               bool                                      raii,
                                                               size_t                                    returnParam,
                                                               std::map<size_t, VectorParamData> const & vectorParams,
                                                               std::vector<TypeStruct>::const_iterator   structIt ) const
{
  // If the name contains "Get", is is supposed to be not a creation function, but just gets something. Even if that something is a handle!
  if ( !structureHoldsHandle( *structIt ) || ( name.find( "Get" ) != name.npos ) )
  {
    // the returnType is a struct does not hold a handle
    if ( structureHoldsVector( *structIt ) )
    {
      // the returned struct specifies a vector of data
      // can't generate an enhanced version for such a complex command! Just use the standard version
      return generateCommandSetInclusive( name, command, initialSkipCount, definition, { returnParam }, vectorParams, false, {}, raii, true, {} );
    }
    else
    {
      // the returnType is a struct does not hold a handle or a vector
      if ( structIt->extendedBy.empty() )
      {
        // the returnType is a not extendable struct (without a handle or a vector)
        return generateCommandSetInclusive( name,
                                            command,
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
      else
      {
        // the returnType is an extendable struct (without a handle or a vector)
        std::string const & returnType = command.params[returnParam].type.name;
        if ( !structureChainHoldsHandle( returnType ) || ( name.find( "Get" ) != name.npos ) )
        {
          // there's no handle in the structure chain of the returnType struct or any handle is just returned, not created
          std::string commandStr;
          if ( raii && structureChainHoldsVector( returnType ) )
          {
            // raii functions returning a structure chain that might hold a vector need to also have the standard implementation!
            commandStr = generateCommandSetInclusive( name,
                                                      command,
                                                      initialSkipCount,
                                                      definition,
                                                      { returnParam },
                                                      vectorParams,
                                                      false,
                                                      { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::chained },
                                                      raii,
                                                      false,
                                                      {} );
          }
          // no matter if there's a vector in some extending struct, generate some enhanced functions
          return commandStr + generateCommandSetInclusive( name,
                                                           command,
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
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommand1ReturnsValue( std::string const &                       name,
                                                              Command const &                           command,
                                                              size_t                                    initialSkipCount,
                                                              bool                                      definition,
                                                              bool                                      raii,
                                                              size_t                                    returnParam,
                                                              std::map<size_t, VectorParamData> const & vectorParams ) const
{
  std::string const & returnType = command.params[returnParam].type.name;
  if ( bool isHandle = isHandleType( returnType ); isHandle || containsByName( m_vkxml.types.baseTypes, returnType ) ||
                                                   containsByName( m_vkxml.types.bitmasks, returnType ) || containsByName( m_enums, returnType ) ||
                                                   containsByName( m_vkxml.types.externals, returnType ) )
  {
    // the returnType is a handle or some basic type
    bool unique = isHandle && ( command.returnType.name == "VkResult" );  // only if the return type is VkResult, we need to have a unique version
    return generateCommandSetInclusive( name,
                                        command,
                                        initialSkipCount,
                                        definition,
                                        { returnParam },
                                        vectorParams,
                                        unique,
                                        { CommandFlavourFlagBits::enhanced },
                                        raii,
                                        isHandle,
                                        { CommandFlavourFlagBits::enhanced } );
  }
  else if ( auto structIt = findByNameOrAlias( m_vkxml.types.structs, returnType ); structIt != m_vkxml.types.structs.end() )
  {
    return generateCommand1ReturnsStruct( name, command, initialSkipCount, definition, raii, returnParam, vectorParams, structIt );
  }

  return "";
}

std::string VulkanHppGenerator::generateCommand1ReturnsVector( std::string const &                       name,
                                                               Command const &                           command,
                                                               size_t                                    initialSkipCount,
                                                               bool                                      definition,
                                                               bool                                      raii,
                                                               size_t                                    returnParam,
                                                               std::map<size_t, VectorParamData> const & vectorParams ) const
{
  std::string const & returnType = command.params[returnParam].type.name;
  if ( returnType == "void" )
  {
    // the retunType is a vector of void
    return generateCommandSetInclusive( name,
                                        command,
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
  else if ( isHandleType( returnType ) )
  {
    // the returnType is a vector of handles
    bool unique = ( command.returnType.name == "VkResult" );  // only if the return type is VkResult, we need to have a unique version
    std::vector<CommandFlavourFlags> flavourFlags     = { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator };
    std::vector<CommandFlavourFlags> raiiFlavourFlags = { CommandFlavourFlagBits::enhanced };
    if ( containsByName( command.params, command.params[returnParam].len ) )
    {
      // the vector has a len by param -> also generate the singular version
      flavourFlags.push_back( CommandFlavourFlagBits::singular );
      raiiFlavourFlags.push_back( CommandFlavourFlagBits::singular );
    }
    return generateCommandSetInclusive(
      name, command, initialSkipCount, definition, { returnParam }, vectorParams, unique, flavourFlags, raii, true, raiiFlavourFlags );
  }
  else if ( auto structIt = findByNameOrAlias( m_vkxml.types.structs, returnType ); structIt != m_vkxml.types.structs.end() )
  {
    return generateCommand1ReturnsVectorOfStructs( name, command, initialSkipCount, definition, raii, returnParam, vectorParams, structIt );
  }
  return "";
}

std::string VulkanHppGenerator::generateCommand1ReturnsVectorOfStructs( std::string const &                       name,
                                                                        Command const &                           command,
                                                                        size_t                                    initialSkipCount,
                                                                        bool                                      definition,
                                                                        bool                                      raii,
                                                                        size_t                                    returnParam,
                                                                        std::map<size_t, VectorParamData> const & vectorParams,
                                                                        std::vector<TypeStruct>::const_iterator   structIt ) const
{
  if ( auto handleMemberIt = findHandleMember( structIt->members ); handleMemberIt != structIt->members.end() )
  {
    // the returnType is a vector of structs with a handle
    if ( auto vectorMemberIt = findVectorMember( structIt->members ); vectorMemberIt != structIt->members.end() )
    {
      // the returnType is a vector of structs with a handle and a vector
      if ( structIt->extendedBy.empty() )
      {
        // the returnType is a vector of non-extendable structs with a handle and a vector
        if ( handleMemberIt == vectorMemberIt )
        {
          // the returnType is a vector of non-extendable structs with a vector of handles
          checkForError( std::find_if( std::next( vectorMemberIt ), structIt->members.end(), []( auto const & member ) { return !member.len.empty(); } ) ==
                           structIt->members.end(),
                         structIt->xmlLine,
                         "Structure " + structIt->name + " used for vector of handles return has multiple members with len!" );

          return generateCommandSetInclusive( name,
                                              command,
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
    }
  }
  else
  {
    // the returnType is a vector of structs without a handle
    if ( auto vectorMemberIt = findVectorMember( structIt->members ); vectorMemberIt != structIt->members.end() )
    {
      // the returnType is a vector of structs with a vector
      // can't generate an enhanced version for such a complex command! Just use the standard version
      return generateCommandSetInclusive( name, command, initialSkipCount, definition, { returnParam }, vectorParams, false, {}, raii, false, {} );
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommand2Returns(
  std::string const & name, Command const & command, size_t initialSkipCount, bool definition, bool raii, std::vector<size_t> const & returnParams ) const
{
  assert( returnParams.size() == 2 );

  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( command.params );
  if ( encodesEnumeration( command.params, vectorParams, returnParams[0], returnParams[1] ) )
  {
    return generateCommand2ReturnsEnum( name, command, initialSkipCount, definition, raii, returnParams, vectorParams );
  }
  else if ( bool param0IsVector = vectorParams.contains( returnParams[0] ), param1IsVector = vectorParams.contains( returnParams[1] );
            param0IsVector && param1IsVector )
  {
    // both parameters encode vectors
    return generateCommand2ReturnsVectorVector( name, command, initialSkipCount, definition, raii, returnParams, vectorParams );
  }
  else if ( param0IsVector )
  {
    // only the first parameter encodes a vector
    return generateCommand2ReturnsVectorValue( name, command, initialSkipCount, definition, raii, returnParams, vectorParams );
  }
  else if ( param1IsVector )
  {
    // only the second parameter encodes a vector
  }
  else
  {
    // no parameter encodes a vector
    return generateCommand2ReturnsValueValue( name, command, initialSkipCount, definition, raii, returnParams, vectorParams );
  }

  return "";
}

std::string VulkanHppGenerator::generateCommand2ReturnsEnum( std::string const &                       name,
                                                             Command const &                           command,
                                                             size_t                                    initialSkipCount,
                                                             bool                                      definition,
                                                             bool                                      raii,
                                                             std::vector<size_t> const &               returnParams,
                                                             std::map<size_t, VectorParamData> const & vectorParams ) const
{
  std::string const & returnType1 = command.params[returnParams[1]].type.name;
  if ( isHandleType( returnType1 ) )
  {
    // the return params specify an enumaration of handles
    return generateCommandSetInclusive( name,
                                        command,
                                        initialSkipCount,
                                        definition,
                                        returnParams,
                                        vectorParams,
                                        false,
                                        { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator },
                                        raii,
                                        name != "vkGetSwapchainImagesKHR",  // don't make a raii factory for swapchain images!
                                        { CommandFlavourFlagBits::enhanced } );
  }
  else if ( auto structIt = findByNameOrAlias( m_vkxml.types.structs, returnType1 ); structIt != m_vkxml.types.structs.end() )
  {
    // the return params specify an enumeration of structures
    if ( structIt->extendedBy.empty() )
    {
      // the return params specify an enumeration of not extendable structs
      // we don't care if that struct holds handles; it seems they are not created this way!
      if ( structureHoldsVector( *structIt ) )
      {
        // the return params specify an enumeration of not extendable structs with a vector!!
        // can't generate an enhanced version for such a complex command! Just use the standard version
        return generateCommandSetInclusive( name, command, initialSkipCount, definition, returnParams, vectorParams, false, {}, raii, true, {} );
      }
      else
      {
        // the return params specify an enumeration of not extendable struct without vectors
        return generateCommandSetInclusive( name,
                                            command,
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
    else
    {
      // the return params specify an enumeration of extendable structs
      if ( !structureChainHoldsHandle( returnType1 ) )
      {
        // the return params specify an enumeration of structure chains without handles
        if ( !structureChainHoldsVector( returnType1 ) )
        {
          // the return params specify an enumeration of structure chains without handles or vectors
          return generateCommandSetInclusive( name,
                                              command,
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
  else if ( ( returnType1 == "void" ) || m_enums.contains( returnType1 ) )
  {
    // the return params specify an enumeration of basic types or void
    return generateCommandSetInclusive( name,
                                        command,
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
  return "";
}

std::string VulkanHppGenerator::generateCommand2ReturnsValueValue( std::string const &                       name,
                                                                   Command const &                           command,
                                                                   size_t                                    initialSkipCount,
                                                                   bool                                      definition,
                                                                   bool                                      raii,
                                                                   std::vector<size_t> const &               returnParams,
                                                                   std::map<size_t, VectorParamData> const & vectorParams ) const
{
  std::string const & returnType0 = command.params[returnParams[0]].type.name;
  std::string const & returnType1 = command.params[returnParams[1]].type.name;
  if ( containsByName( m_vkxml.types.baseTypes, returnType0 ) )
  {
    if ( containsByName( m_vkxml.types.baseTypes, returnType1 ) )
    {
      // both types are just base types -> create a standard and an enhanced version
      return generateCommandSetInclusive( name,
                                          command,
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
  else if ( containsByName( m_vkxml.types.externals, returnType0 ) )
  {
    if ( returnType1 == "void" )
    {
      // a return type of void *, without any len/vector information, means, it points to some memory where the size of that memory is externally calculated ->
      // can't have an enhanced version
      return generateCommandSetInclusive( name, command, initialSkipCount, definition, returnParams, vectorParams, false, {}, raii, false, {} );
    }
    else if ( isStructureType( returnType1 ) && isStructureChainAnchor( returnType1 ) )
    {
      // return an external type (like uint32_t) and an extendable struct
      return generateCommandSetInclusive( name,
                                          command,
                                          initialSkipCount,
                                          definition,
                                          returnParams,
                                          vectorParams,
                                          false,
                                          { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::chained },
                                          raii,
                                          false,
                                          { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::chained } );
    }
  }
  else if ( isHandleType( returnType0 ) && ( name.find( "Get" ) != std::string::npos ) )
  {
    // the first return parameter is a handle, but it's just a getter -> the handle is not generated
    if ( auto structIt = findByNameOrAlias( m_vkxml.types.structs, returnType1 ); structIt != m_vkxml.types.structs.end() )
    {
      // the second return parameter is a structure
      std::vector<CommandFlavourFlags> flags = { CommandFlavourFlagBits::enhanced };
      if ( !structIt->extendedBy.empty() )
      {
        flags.push_back( CommandFlavourFlagBits::chained );
      }
      return generateCommandSetInclusive( name, command, initialSkipCount, definition, returnParams, vectorParams, false, flags, raii, false, flags );
    }
  }
  else if ( auto structIt = findByNameOrAlias( m_vkxml.types.structs, returnType0 ); structIt != m_vkxml.types.structs.end() )
  {
    // the first return param is a single struct
    if ( !structureHoldsHandle( *structIt ) )
    {
      // the first return param is a single struct without handles
      if ( !structureHoldsVector( *structIt ) )
      {
        // the first return param is a single struct without handles or vectors
        if ( structIt->extendedBy.empty() )
        {
          // the first return param is a single, not extendable struct without handles or vectors
          if ( containsByName( m_vkxml.types.externals, returnType1 ) )
          {
            // the second return param is just some basic type
            return generateCommandSetInclusive( name,
                                                command,
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
        else
        {
          // the first return param is an extendable struct
          if ( containsByName( m_vkxml.types.externals, returnType1 ) )
          {
            // the second return param is just some basic type
            return generateCommandSetInclusive( name,
                                                command,
                                                initialSkipCount,
                                                definition,
                                                returnParams,
                                                vectorParams,
                                                false,
                                                { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::chained },
                                                raii,
                                                false,
                                                { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::chained } );
          }
        }
      }
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommand2ReturnsVectorValue( std::string const &                       name,
                                                                    Command const &                           command,
                                                                    size_t                                    initialSkipCount,
                                                                    bool                                      definition,
                                                                    bool                                      raii,
                                                                    std::vector<size_t> const &               returnParams,
                                                                    std::map<size_t, VectorParamData> const & vectorParams ) const
{
  if ( command.params[returnParams[0]].type.name == "uint64_t" )
  {
    // the first return param is a vector of uint64_t
    if ( command.params[returnParams[1]].type.name == "uint64_t" )
    {
      // the second return param is a uint64_t
      return generateCommandSetInclusive( name,
                                          command,
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
  else if ( auto structIt = findByNameOrAlias( m_vkxml.types.structs, command.params[returnParams[0]].type.name ); structIt != m_vkxml.types.structs.end() )
  {
    // the first return param is a vector of structs
    if ( !structureHoldsHandle( *structIt ) )
    {
      // the first return param is a vector of structs without handles
      if ( structureHoldsVector( *structIt ) )
      {
        // the first return param is a vector of structs with vectors
        // can't generate an enhanced version for such a complex command, no matter if the struct is extendable or not, no matter what the second return
        // parameter is
        return generateCommandSetInclusive( name, command, initialSkipCount, definition, returnParams, vectorParams, false, {}, raii, true, {} );
      }
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommand2ReturnsVectorVector( std::string const &                       name,
                                                                     Command const &                           command,
                                                                     size_t                                    initialSkipCount,
                                                                     bool                                      definition,
                                                                     bool                                      raii,
                                                                     std::vector<size_t> const &               returnParams,
                                                                     std::map<size_t, VectorParamData> const & vectorParams ) const
{
  if ( m_handles.find( command.params[returnParams[0]].type.name ) != m_handles.end() )
  {
    // the first return param is a vector of handles
    if ( m_enums.find( command.params[returnParams[1]].type.name ) != m_enums.end() )
    {
      // the second return param is a vector of enums
      std::vector<CommandFlavourFlags> flavourFlags     = { CommandFlavourFlagBits::enhanced, CommandFlavourFlagBits::withAllocator };
      std::vector<CommandFlavourFlags> raiiFlavourFlags = { CommandFlavourFlagBits::enhanced };
      if ( containsByName( command.params, command.params[returnParams[0]].len ) && containsByName( command.params, command.params[returnParams[1]].len ) )
      {
        // the vector has a len by param -> also generate the singular version
        flavourFlags.push_back( CommandFlavourFlagBits::singular );
        raiiFlavourFlags.push_back( CommandFlavourFlagBits::singular );
      }
      return generateCommandSetInclusive(
        name, command, initialSkipCount, definition, returnParams, vectorParams, true, flavourFlags, raii, true, raiiFlavourFlags );
    }
  }
  else if ( auto structIt = findByNameOrAlias( m_vkxml.types.structs, command.params[returnParams[0]].type.name ); structIt != m_vkxml.types.structs.end() )
  {
    // the first return param is a vector of structs
    if ( !structureHoldsHandle( *structIt ) )
    {
      // the first return param is a vector of structs without handles
      if ( structureHoldsVector( *structIt ) )
      {
        // the first return param is a vector of structs with vectors
        // can't generate an enhanced version for such a complex command, no matter if the struct is extendable or not, no matter what the second return
        // parameter is
        return generateCommandSetInclusive( name, command, initialSkipCount, definition, returnParams, vectorParams, false, {}, raii, true, {} );
      }
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommand3Returns(
  std::string const & name, Command const & command, size_t initialSkipCount, bool definition, bool raii, std::vector<size_t> const & returnParams ) const
{
  assert( returnParams.size() == 3 );

  std::map<size_t, VectorParamData> vectorParams = determineVectorParams( command.params );
  if ( encodesEnumeration( command.params, vectorParams, returnParams[0], returnParams[1] ) )
  {
    if ( encodesEnumeration( command.params, vectorParams, returnParams[0], returnParams[2] ) )
    {
      return generateCommand3ReturnsEnumEnum( name, command, initialSkipCount, definition, raii, returnParams, vectorParams );
    }
    else
    {
      assert( !vectorParams.contains( returnParams[2] ) );
      return generateCommand3ReturnsEnumValue( name, command, initialSkipCount, definition, raii, returnParams, vectorParams );
    }
  }
  else if ( !vectorParams.contains( returnParams[0] ) )
  {
    if ( encodesEnumeration( command.params, vectorParams, returnParams[1], returnParams[2] ) )
    {
      return generateCommand3ReturnsValueEnum( name, command, initialSkipCount, definition, raii, returnParams, vectorParams );
    }
    else
    {
      return generateCommand3ReturnsValueValueValue( name, command, initialSkipCount, definition, raii, returnParams );
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommand3ReturnsEnumEnum( std::string const &                       name,
                                                                 Command const &                           command,
                                                                 size_t                                    initialSkipCount,
                                                                 bool                                      definition,
                                                                 bool                                      raii,
                                                                 std::vector<size_t> const &               returnParams,
                                                                 std::map<size_t, VectorParamData> const & vectorParams ) const
{
  assert( returnParams.size() == 3 );

  if ( containsByName( m_vkxml.types.externals, command.params[returnParams[1]].type.name ) )
  {
    // the first enumerated type is some external type
    if ( containsByName( m_vkxml.types.externals, command.params[returnParams[2]].type.name ) )
    {
      // both enumerated types are some external types
      return generateCommandSetInclusive( name,
                                          command,
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
  else if ( auto structIt1 = findByNameOrAlias( m_vkxml.types.structs, command.params[returnParams[1]].type.name ); structIt1 != m_vkxml.types.structs.end() )
  {
    // the first enumerated return param is a vector of structs
    if ( !structureHoldsHandle( *structIt1 ) )
    {
      // the first enumerated return param is a vector of structs without handles
      if ( !structureHoldsVector( *structIt1 ) )
      {
        // the first enumerated return param is a vector of structs without handles or vectors
        if ( structIt1->extendedBy.empty() )
        {
          // the first enumerated return param is a vector of non-extendable structs without handles or vectors
          if ( auto structIt2 = findByNameOrAlias( m_vkxml.types.structs, command.params[returnParams[2]].type.name );
               structIt2 != m_vkxml.types.structs.end() )
          {
            if ( !structureHoldsHandle( *structIt2 ) )
            {
              // the second enumerated return param is a vector of structs without handles
              if ( !structureHoldsVector( *structIt2 ) )
              {
                // the second enumerated return param is a vector of structs without handles or vectors
                if ( structIt2->extendedBy.empty() )
                {
                  // the second enumerated return param is a vector of non-extendable structs without handles or vectors
                  return generateCommandSetInclusive( name,
                                                      command,
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
  }
  return "";
}

std::string VulkanHppGenerator::generateCommand3ReturnsEnumValue( std::string const &                       name,
                                                                  Command const &                           command,
                                                                  size_t                                    initialSkipCount,
                                                                  bool                                      definition,
                                                                  bool                                      raii,
                                                                  std::vector<size_t> const &               returnParams,
                                                                  std::map<size_t, VectorParamData> const & vectorParams ) const
{
  assert( returnParams.size() == 3 );

  if ( m_enums.contains( command.params[returnParams[1]].type.name ) )
  {
    // the enumerated type is an enum
    if ( containsByName( m_vkxml.types.bitmasks, command.params[returnParams[2]].type.name ) )
    {
      // the third return type is a bitmask
      return generateCommandSetInclusive( name,
                                          command,
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
  return "";
}

std::string VulkanHppGenerator::generateCommand3ReturnsValueEnum( std::string const &                       name,
                                                                  Command const &                           command,
                                                                  size_t                                    initialSkipCount,
                                                                  bool                                      definition,
                                                                  bool                                      raii,
                                                                  std::vector<size_t> const &               returnParams,
                                                                  std::map<size_t, VectorParamData> const & vectorParams ) const
{
  assert( returnParams.size() == 3 );

  if ( auto structIt0 = findByNameOrAlias( m_vkxml.types.structs, command.params[returnParams[0]].type.name );
       ( structIt0 != m_vkxml.types.structs.end() ) && !structureHoldsHandle( *structIt0 ) && !structureHoldsVector( *structIt0 ) )
  {
    // the first return param is a struct without handles or vectors
    if ( structIt0->extendedBy.empty() )
    {
      // the first return param is a non-extendable struct without handles or vectors
      if ( auto structIt2 = findByNameOrAlias( m_vkxml.types.structs, command.params[returnParams[2]].type.name );
           ( command.params[returnParams[2]].type.name == "void" ) ||
           ( structIt2 != m_vkxml.types.structs.end() && !structureHoldsHandle( *structIt2 ) && !structureHoldsVector( *structIt2 ) ) )
      {
        // the enumerated return param is either void or a non-extendable struct without handles or vectors
        return generateCommandSetInclusive( name,
                                            command,
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
    else
    {
      // the first return param is an extendable struct without handles or vectors
      if ( command.params[returnParams[2]].type.name == "void" )
      {
        // the enumerated return type is void!
        return generateCommandSetInclusive( name,
                                            command,
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
  return "";
}

std::string VulkanHppGenerator::generateCommand3ReturnsValueValueValue(
  std::string const & name, Command const & command, size_t initialSkipCount, bool definition, bool raii, std::vector<size_t> const & returnParams ) const
{
  assert( returnParams.size() == 3 );

  if ( isHandleType( command.params[returnParams[0]].type.name ) && isHandleType( command.params[returnParams[1]].type.name ) &&
       isStructureType( command.params[returnParams[2]].type.name ) && ( name.find( "Get" ) != std::string::npos ) )
  {
    // this functions "gets" you some data, some of them might be handles, but they're not created, just got
    auto structIt = findByName( m_vkxml.types.structs, command.params[returnParams[2]].type.name );
    assert( structIt != m_vkxml.types.structs.end() );
    if ( structureHoldsVector( *structIt ) )
    {
      // the returned struct specifies a vector of data
      // can't generate an enhanced version for such a complex command! Just use the standard version
      return generateCommandSetInclusive( name, command, initialSkipCount, definition, returnParams, {}, false, {}, raii, true, {} );
    }
  }
  return "";
}

std::string VulkanHppGenerator::generateCommandDefinitions() const
{
  std::string const commandDefinitionsTemplate = R"(
  //===========================
  //=== COMMAND Definitions ===
  //===========================

${commandDefinitions}
)";

  std::string           commandDefinitions;
  std::set<std::string> listedCommands;  // some commands are listed with more than one extension!
  for ( auto const & feature : m_vkxml.features )
  {
    commandDefinitions += generateCommandDefinitions( feature.require, listedCommands, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    commandDefinitions += generateCommandDefinitions( extension.requireData, listedCommands, extension.name );
  }

  return replaceWithMap( commandDefinitionsTemplate, { { "commandDefinitions", commandDefinitions } } );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateCommandDefinitions( std::vector<RequireType> const & requireTypes,
                                                            std::set<std::string> &          listedCommands,
                                                            std::string const &              title ) const
{
  std::string str;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & command )
                          {
                            if ( listedCommands.insert( name ).second )
                            {
                              str += generateCommandDefinitions( name, isHandleType( command.params[0].type.name ) ? command.params[0].type.name : "" );
                            }
                          } );
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateCommandDefinitions( std::string const & name, std::string const & handle ) const
{
  auto commandIt = findByNameOrAlias( m_vkxml.commands, name );
  assert( commandIt != m_vkxml.commands.end() );

  std::string str = "\n" + generateCommand( name, *commandIt, handle.empty() ? 0 : 1, true, false );

  // special handling for destroy functions, filter out alias functions
  std::string commandName = generateCommandName( name, commandIt->params, 1 );
  if ( ( name == commandIt->name ) && ( ( ( name.substr( 2, 7 ) == "Destroy" ) && ( commandName != "destroy" ) ) || ( name.substr( 2, 4 ) == "Free" ) ||
                                        ( name == "vkReleasePerformanceConfigurationINTEL" ) || ( name == "vkReleaseDisplayEXT" ) ) )
  {
    Command specialCommand = *commandIt;
    assert( ( 1 < specialCommand.params.size() ) && ( specialCommand.params[0].type.name == handle ) );
    specialCommand.params[1].optional.clear();  // make sure, the object to destroy/free/release is not optional in the shortened version!

    std::string destroyCommandString = generateCommand( name, specialCommand, handle.empty() ? 0 : 1, true, false );
    std::string shortenedName;
    if ( name.substr( 2, 7 ) == "Destroy" )
    {
      shortenedName = "destroy";
    }
    else if ( name.substr( 2, 4 ) == "Free" )
    {
      shortenedName = "free";
    }
    else
    {
      assert( ( name == "vkReleasePerformanceConfigurationINTEL" ) || ( name == "vkReleaseDisplayEXT" ) );
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
    assert( 1 < commandIt->params.size() );
    pos = destroyCommandString.find( commandIt->params[1].name );  // skip the standard version of the function
    assert( pos != std::string::npos );
    pos = destroyCommandString.find( commandIt->params[1].name, pos + 1 );  // get the argument to destroy in the advanced version
    assert( pos != std::string::npos );
    pos = destroyCommandString.find( " VULKAN_HPP_DEFAULT_ASSIGNMENT( {} )", pos );
    if ( pos != std::string::npos )
    {
      destroyCommandString.erase( pos, strlen( " VULKAN_HPP_DEFAULT_ASSIGNMENT( {} )" ) );
    }
    str += "\n" + destroyCommandString;
  }
  return str;
}

std::string VulkanHppGenerator::generateCommandEnhanced( std::string const &                       name,
                                                         Command const &                           command,
                                                         size_t                                    initialSkipCount,
                                                         bool                                      definition,
                                                         std::map<size_t, VectorParamData> const & vectorParams,
                                                         std::vector<size_t> const &               returnParams,
                                                         CommandFlavourFlags                       flavourFlags ) const
{
  bool const chained  = flavourFlags & CommandFlavourFlagBits::chained;
  bool const singular = flavourFlags & CommandFlavourFlagBits::singular;

  assert( vectorParams.empty() || ( vectorParams.begin()->second.lenParam != INVALID_INDEX ) );
  assert( !singular || !returnParams.empty() );  // if singular is true, then there is at least one returnParam !

  std::set<size_t> skippedParams   = determineSkippedParams( command.params, initialSkipCount, vectorParams, returnParams, singular );
  std::set<size_t> templatedParams = ( flavourFlags & CommandFlavourFlagBits::keepVoidPtr ) ? std::set<size_t>() : determineVoidPointerParams( command.params );
  std::vector<size_t> chainedReturnParams;
  if ( chained )
  {
    chainedReturnParams = determineChainedReturnParams( command.params, returnParams );
    assert( chainedReturnParams.size() == 1 );
  }
  std::set<size_t> singularParams = singular ? determineSingularParams( returnParams[0], vectorParams ) : std::set<size_t>();
  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck =
    needsVectorSizeCheck( command.params, vectorParams, returnParams, singularParams, skippedParams );
  bool const enumerating = determineEnumeration( vectorParams, returnParams );

  std::vector<std::string> dataTypes      = determineDataTypes( command.params, vectorParams, returnParams, templatedParams, false );
  std::string              returnDataType = generateReturnDataType( vectorParams, returnParams, enumerating, dataTypes, flavourFlags, false );

  std::string argumentTemplates = generateArgumentTemplates( command.params, returnParams, vectorParams, templatedParams, chainedReturnParams, false );
  auto [allocatorTemplates, allocatorTemplates20, uniqueHandleAllocatorTemplates, uniqueHandleAllocatorTemplates20] =
    generateAllocatorTemplates( returnParams, dataTypes, vectorParams, chainedReturnParams, flavourFlags, definition );
  std::string typenameCheck       = generateTypenameCheck( returnParams, vectorParams, chainedReturnParams, definition, dataTypes, flavourFlags );
  std::string nodiscard           = generateNoDiscard( !returnParams.empty(), 1 < command.successCodes.size(), 1 < command.errorCodes.size() );
  std::string returnType          = generateReturnType( command.params, returnParams, vectorParams, flavourFlags, false, dataTypes );
  std::string decoratedReturnType = generateDecoratedReturnType( command, returnParams, enumerating, flavourFlags, returnType );
  std::string className           = initialSkipCount ? stripPrefix( command.params[initialSkipCount - 1].type.name, "Vk" ) : "";
  std::string classSeparator      = isHandleType( command.params[0].type.name ) ? "::" : "";
  std::string commandName         = generateCommandName( name, command.params, initialSkipCount, flavourFlags );
  std::string argumentList        = generateArgumentListEnhanced(
    command.params, returnParams, vectorParams, skippedParams, singularParams, templatedParams, chainedReturnParams, false, definition, flavourFlags, true );
  std::string constString    = isHandleType( command.params[0].type.name ) ? " const" : "";
  std::string noexceptString = generateNoExcept( command.errorCodes, returnParams, vectorParams, flavourFlags, vectorSizeCheck.first, false );

  if ( definition )
  {
    std::string vectorSizeCheckString =
      vectorSizeCheck.first ? generateVectorSizeCheck( name, command, initialSkipCount, vectorSizeCheck.second, skippedParams, false ) : "";
    std::string returnVariable   = generateReturnVariable( command, returnParams, vectorParams, flavourFlags );
    std::string dataDeclarations = generateDataDeclarations(
      command, returnParams, vectorParams, templatedParams, flavourFlags, false, dataTypes, returnDataType, returnType, returnVariable );
    std::string dataPreparation =
      generateDataPreparation( command, initialSkipCount, returnParams, vectorParams, templatedParams, flavourFlags, enumerating, dataTypes );
    std::string dataSizeChecks = generateDataSizeChecks( command, returnParams, dataTypes, vectorParams, templatedParams, singular );
    std::string callSequence   = generateCallSequence(
      name, command, returnParams, vectorParams, initialSkipCount, singularParams, templatedParams, chainedReturnParams, flavourFlags, false, false );
    std::string resultCheck = generateResultCheck( command,
                                                   className,
                                                   classSeparator,
                                                   commandName,
                                                   enumerating,
                                                   ( returnParams.size() == 1 ) && ( command.params[returnParams[0]].type.name == "VkPipeline" ) && !singular,
                                                   false );
    std::string returnStatement = generateReturnStatement( command,
                                                           returnVariable,
                                                           returnType,
                                                           decoratedReturnType,
                                                           returnDataType,
                                                           initialSkipCount,
                                                           returnParams.empty() ? INVALID_INDEX : returnParams[0],
                                                           flavourFlags,
                                                           enumerating,
                                                           false );

    std::string const functionTemplate =
      R"(  // wrapper function for command ${vkCommandName}, see https://registry.khronos.org/vulkan/specs/latest/man/html/${vkCommandName}.html
#  if VULKAN_HPP_CPP_VERSION < 20
  template <${argumentTemplates}${allocatorTemplates}typename Dispatch${uniqueHandleAllocatorTemplates}${typenameCheck}, typename std::enable_if<IS_DISPATCHED( ${vkCommandName} ), bool>::type>
#  else
  template <${argumentTemplates}${allocatorTemplates20}typename Dispatch${uniqueHandleAllocatorTemplates20}>
  requires( IS_DISPATCHED( ${vkCommandName} ) )
#  endif
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
                             { "allocatorTemplates20", allocatorTemplates20 },
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
                             { "functionPointerCheck", generateFunctionPointerCheck( name, command.requiredBy, false ) },
                             { "nodiscard", nodiscard },
                             { "noexcept", noexceptString },
                             { "resultCheck", resultCheck },
                             { "returnStatement", returnStatement },
                             { "typenameCheck", typenameCheck },
                             { "uniqueHandleAllocatorTemplates", uniqueHandleAllocatorTemplates },
                             { "uniqueHandleAllocatorTemplates20", uniqueHandleAllocatorTemplates20 },
                             { "vectorSizeCheck", vectorSizeCheckString },
                             { "vkCommandName", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    // wrapper function for command ${vkCommandName}, see https://registry.khronos.org/vulkan/specs/latest/man/html/${vkCommandName}.html
#  if VULKAN_HPP_CPP_VERSION < 20
    template <${argumentTemplates}${allocatorTemplates}typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE${uniqueHandleAllocatorTemplates}${typenameCheck}, typename std::enable_if<IS_DISPATCHED( ${vkCommandName} ), bool>::type = true>
#  else
    template <${argumentTemplates}${allocatorTemplates20}typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE${uniqueHandleAllocatorTemplates20}>
    requires( IS_DISPATCHED( ${vkCommandName} ) )
#  endif
    ${nodiscard}${decoratedReturnType} ${commandName}( ${argumentList} )${const}${noexcept};)";

    return replaceWithMap( functionTemplate,
                           { { "allocatorTemplates", allocatorTemplates },
                             { "allocatorTemplates20", allocatorTemplates20 },
                             { "argumentList", argumentList },
                             { "argumentTemplates", argumentTemplates },
                             { "commandName", commandName },
                             { "const", isHandleType( command.params[0].type.name ) ? " const" : "" },
                             { "decoratedReturnType", decoratedReturnType },
                             { "nodiscard", nodiscard },
                             { "noexcept", noexceptString },
                             { "typenameCheck", typenameCheck },
                             { "uniqueHandleAllocatorTemplates", uniqueHandleAllocatorTemplates },
                             { "uniqueHandleAllocatorTemplates20", uniqueHandleAllocatorTemplates20 },
                             { "vkCommandName", name } } );
  }
}

std::string VulkanHppGenerator::generateCommandName( std::string const &        vulkanCommandName,
                                                     std::vector<Param> const & params,
                                                     size_t                     initialSkipCount,
                                                     CommandFlavourFlags        flavourFlags ) const
{
  std::string commandName( startLowerCase( stripPrefix( vulkanCommandName, "vk" ) ) );
  for ( size_t i = initialSkipCount - 1; i < initialSkipCount; --i )  // count down to zero, then wrap around and stop
  {
    std::string const & argumentType = params[i].type.name;
    std::string         searchName   = stripPrefix( argumentType, "Vk" );
    std::string         argumentTag  = findTag( argumentType, m_vkxml.tags.tags );
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
    std::string commandTag = findTag( commandName, m_vkxml.tags.tags );
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
  std::string const commandTemplate = R"(
#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandStandard}
#else
${commandEnhanced}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

  return replaceWithMap( commandTemplate, std::map<std::string, std::string>( { { "commandEnhanced", enhanced }, { "commandStandard", standard } } ) );
}

std::string VulkanHppGenerator::generateCommandSetExclusive(
  std::string const & name, Command const & command, size_t initialSkipCount, bool definition, bool raii ) const
{
  if ( raii )
  {
    return generateRAIIHandleCommandEnhanced( name, command, initialSkipCount, {}, {}, definition );
  }
  else
  {
    return generateCommandSet( generateCommandStandard( name, command, initialSkipCount, definition, raii ),
                               generateCommandEnhanced( name, command, initialSkipCount, definition, {}, {}, {} ) );
  }
}

std::string VulkanHppGenerator::generateCommandSetInclusive( std::string const &                      name,
                                                             Command const &                          command,
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
  static std::vector<size_t> const               emptyReturnParams;
  static std::map<size_t, VectorParamData> const emptyVectorParams;
  if ( raii )
  {
    std::string raiiCommands;
    if ( raiiFlags.empty() )
    {
      // Functions without enhancements need to have a standard implementation
      raiiCommands = generateRAIIHandleCommandStandard( name, command, initialSkipCount, definition, raii );
    }
    else
    {
      if ( std::ranges::any_of( vectorParams,
                                [&command, &returnParams]( std::pair<size_t, VectorParamData> const & vpd )
                                {
                                  return ( command.params[vpd.first].type.name == "void" ) &&
                                         std::ranges::none_of( returnParams, [&vpd]( size_t returnParam ) { return vpd.second.lenParam == returnParam; } );
                                } ) )
      {
        // Functions taking a vector with data type void and the len for that data is not returned (no enumeration) need the standard implementation as well
        raiiCommands = generateRAIIHandleCommandStandard( name, command, initialSkipCount, definition, raii );
      }
      for ( auto flag : raiiFlags )
      {
        bool const noReturn = flag & CommandFlavourFlagBits::noReturn;
        assert( !noReturn || !raiiFactory );  // noReturn => !raiiFactory
        raiiCommands +=
          raiiFactory
            ? generateRAIIHandleCommandFactory( name, command, initialSkipCount, returnParams, vectorParams, definition, flag )
            : generateRAIIHandleCommandEnhanced(
                name, command, initialSkipCount, noReturn ? emptyReturnParams : returnParams, noReturn ? emptyVectorParams : vectorParams, definition, flag );
      }
    }
    return raiiCommands;
  }
  else
  {
    std::vector<std::string> enhancedCommands, uniqueCommands;
    for ( auto flag : flags )
    {
      bool const noReturn = flag & CommandFlavourFlagBits::noReturn;
      assert( !noReturn || !unique );  // noReturn => !unique
      enhancedCommands.push_back( generateCommandEnhanced(
        name, command, initialSkipCount, definition, noReturn ? emptyVectorParams : vectorParams, noReturn ? emptyReturnParams : returnParams, flag ) );
      if ( unique )
      {
        uniqueCommands.push_back(
          generateCommandEnhanced( name, command, initialSkipCount, definition, vectorParams, returnParams, flag | CommandFlavourFlagBits::unique ) );
      }
    }
    return generateCommandSet( definition, generateCommandStandard( name, command, initialSkipCount, definition, raii ), enhancedCommands, uniqueCommands );
  }
}

std::string
  VulkanHppGenerator::generateCommandStandard( std::string const & name, Command const & command, size_t initialSkipCount, bool definition, bool raii ) const
{
  std::set<size_t> skippedParams = determineSkippedParams( command.params, initialSkipCount, {}, {}, false );

  std::string argumentList = generateArgumentListStandard( command.params, skippedParams, definition, raii, true );
  std::string commandName  = generateCommandName( name, command.params, initialSkipCount );
  std::string nodiscard    = ( 1 < command.successCodes.size() + command.errorCodes.size() ) ? "VULKAN_HPP_NODISCARD " : "";
  std::string returnType   = stripPrefix( command.returnType.name, "Vk" );
  bool        isHandle     = isHandleType( command.params[0].type.name );

  if ( definition )
  {
    std::string functionBody = "d." + name + "( " + generateCallArgumentsStandard( command.params, initialSkipCount ) + " )";
    if ( command.returnType.name.starts_with( "Vk" ) )
    {
      functionBody = "return static_cast<" + returnType + ">( " + functionBody + " )";
    }
    else if ( command.returnType.name != "void" )
    {
      functionBody = "return " + functionBody;
    }

    std::string const functionTemplate =
      R"(  // wrapper function for command ${vkCommandName}, see https://registry.khronos.org/vulkan/specs/latest/man/html/${vkCommandName}.html
#  if VULKAN_HPP_CPP_VERSION < 20
  template <typename Dispatch, typename std::enable_if<IS_DISPATCHED( ${vkCommandName} ), bool>::type>
#  else
  template <typename Dispatch>
  requires( IS_DISPATCHED( ${vkCommandName} ) )
#  endif
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} )${const} VULKAN_HPP_NOEXCEPT
  {
    VULKAN_HPP_ASSERT( d.getVkHeaderVersion() == VK_HEADER_VERSION );
    ${functionBody};
  })";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "className", initialSkipCount ? stripPrefix( command.params[initialSkipCount - 1].type.name, "Vk" ) : "" },
                             { "classSeparator", isHandle ? "::" : "" },
                             { "commandName", commandName },
                             { "const", isHandle ? " const" : "" },
                             { "functionBody", functionBody },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType },
                             { "vkCommandName", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    // wrapper function for command ${vkCommandName}, see https://registry.khronos.org/vulkan/specs/latest/man/html/${vkCommandName}.html
#  if VULKAN_HPP_CPP_VERSION < 20
    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE, typename std::enable_if<IS_DISPATCHED( ${vkCommandName} ), bool>::type = true>
#  else
    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    requires( IS_DISPATCHED( ${vkCommandName} ) )
#  endif
    ${nodiscard}${returnType} ${commandName}( ${argumentList} VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )${const} VULKAN_HPP_NOEXCEPT;)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "const", isHandle ? " const" : "" },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType },
                             { "vkCommandName", name } } );
  }
}

std::string VulkanHppGenerator::generateConstexprString( TypeStruct const & vkStruct ) const
{
  // structs with a VkBaseInStructure and VkBaseOutStructure can't be a constexpr!
  bool const isConstExpression = ( vkStruct.name != "VkBaseInStructure" ) && ( vkStruct.name != "VkBaseOutStructure" );
  return isConstExpression
         ? ( std::string( "VULKAN_HPP_CONSTEXPR" ) +
             ( containsDeprecated( vkStruct.members )
                 ? "_17 "
                 : ( ( containsUnion( vkStruct.name ) || containsArray( vkStruct.name ) || containsDeprecated( vkStruct.members ) ) ? "_14 " : " " ) ) )
         : "";
}

std::string VulkanHppGenerator::generateConstexprDefines() const
{
  auto const constexprFunctionTemplate = std::string{ R"(#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
#else
  template <std::integral T>
#endif
  VULKAN_HPP_CONSTEXPR uint32_t ${constName}( ${arguments} )
  {
    return ${implementation};
  }
)" };
  auto const constexprCallTemplate     = std::string{ R"(  VULKAN_HPP_CONSTEXPR_INLINE auto ${constName} = ${callee}( ${arguments} );
)" };
  auto const constexprValueTemplate    = std::string{ R"(  VULKAN_HPP_CONSTEXPR_INLINE ${type} ${constName} = ${value};
)" };

  // handle the value and callee macros first so they are visible for use in functions below.

  auto constexprDefines = std::string{ R"(
  //===========================
  //=== CONSTEXPR CONSTANTs ===
  //===========================
)" };

  {
    auto const generateConstantsAndProtection = [&constexprValueTemplate, this]<typename RequireType>( std::vector<RequireType> const & requireTypes,
                                                                                                       std::string const &              title,
                                                                                                       std::set<std::string> &          listedConstants )
    {
      std::string constants;
      forEachRequiredConstant(
        requireTypes,
        listedConstants,
        [&constants, &constexprValueTemplate, this]( ConstantValue const & constantData )
        {
          constants +=
            replaceWithMap( constexprValueTemplate,
                            { { "type", constantData.type }, { "constName", generateTaggedCamelCase( constantData.name ) }, { "value", constantData.name } } );
        } );
      return addTitleAndProtection( title, constants );
    };

    std::set<std::string> listedConstants;
    for ( auto const & feature : m_vkxml.features )
    {
      constexprDefines += generateConstantsAndProtection( feature.require, feature.name, listedConstants );
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
    // make `macro` PascalCase and strip the `Vk` prefix
    auto const constName   = stripPrefix( toCamelCase( macro ), "Vk" );
    auto const valueString = replaceWithMap( constexprValueTemplate, { { "type", "uint32_t" }, { "constName", constName }, { "value", macro } } );

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
      constexprFunctionTemplate, { { "arguments", parametersString }, { "constName", constName }, { "implementation", data.possibleDefinition } } );

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
      constexprCallTemplate, { { "arguments", argumentsString }, { "callee", startLowerCase( data.possibleCallee ) }, { "constName", constName } } );
    constexprDefines += callerString;
  }

  // extension constexprs
  auto const extensionConstexprDefinesTemplate = std::string{ R"(
  //=================================
  //=== CONSTEXPR EXTENSION NAMEs ===
  //=================================

  ${enumConstants}
)" };

  std::string enumConstants;
  for ( auto const & extension : m_extensions )
  {
    std::string           enumConstantsByExtension;
    std::set<std::string> encounteredEnumConstants;
    forEachRequiredEnumConstant( extension.requireData,
                                 encounteredEnumConstants,
                                 [&enumConstantsByExtension, &extension, this]( EnumConstantData const & enumConstant )
                                 {
                                   std::string constant = generateTaggedCamelCase( enumConstant.name );
                                   std::string deprecationMessage;
                                   if ( extension.isDeprecated )
                                   {
                                     assert( extension.obsoletedBy.empty() && extension.promotedTo.empty() );
                                     deprecationMessage = extension.deprecatedBy.empty()
                                                          ? ( "The " + extension.name + " extension has been deprecated." )
                                                          : ( "The " + extension.name + " extension has been deprecated by " + extension.deprecatedBy + "." );
                                   }
                                   else if ( !extension.obsoletedBy.empty() )
                                   {
                                     assert( extension.promotedTo.empty() );
                                     deprecationMessage = "The " + extension.name + " extension has been obsoleted by " + extension.obsoletedBy + ".";
                                   }
                                   else if ( !extension.promotedTo.empty() )
                                   {
                                     // promoted extensions are _not_ deprecated!
                                   }
                                   auto const deprecatedPrefix = deprecationMessage.empty() ? "" : "VULKAN_HPP_DEPRECATED( \"" + deprecationMessage + "\" )";
                                   enumConstantsByExtension +=
                                     deprecatedPrefix + "VULKAN_HPP_CONSTEXPR_INLINE auto " + constant + " = " + enumConstant.name + ";\n";
                                 } );
    enumConstants += addTitleAndProtection( extension.name, enumConstantsByExtension );
  }

  constexprDefines += replaceWithMap( extensionConstexprDefinesTemplate, { { "enumConstants", enumConstants } } );

  return constexprDefines;
}

std::string VulkanHppGenerator::generateCppModuleCommands() const
{
  // generate PFN_* functions
  auto                  pfnCommands = std::string{ R"(
  //==================
  //=== PFN TYPEs ===
  //==================
)" };
  std::set<std::string> listedCommands;  // some commands are listed with more than one extension!
  for ( auto const & feature : m_vkxml.features )
  {
    appendCppModuleCommands( feature.require, listedCommands, feature.name, pfnCommands );
  }
  for ( auto const & extension : m_extensions )
  {
    appendCppModuleCommands( extension.requireData, listedCommands, extension.name, pfnCommands );
  }
  return pfnCommands;
}

std::string VulkanHppGenerator::generateDataDeclarations( Command const &                           command,
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
        command, returnParams, vectorParams, templatedParams, flavourFlags, dataTypes, dataType, returnType, returnVariable );
    case 2: return generateDataDeclarations2Returns( command, returnParams, vectorParams, flavourFlags, raii, dataTypes, dataType, returnVariable );
    case 3:
      assert( !( flavourFlags & ( CommandFlavourFlagBits::singular | CommandFlavourFlagBits::unique ) ) );
      return generateDataDeclarations3Returns( command, returnParams, vectorParams, flavourFlags, raii, dataTypes );
    default: assert( false ); return "";
  }
}

std::string VulkanHppGenerator::generateDataDeclarations1Return( Command const &                           command,
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
      std::string const dataDeclarationsTemplate = R"(${dataType} ${dataVariable};)";
      if ( dataType.starts_with( "VULKAN_HPP_NAMESPACE::" ) || m_types.contains( "Vk" + dataType ) )
      {
        return replaceWithMap( dataDeclarationsTemplate, { { "dataType", dataType }, { "dataVariable", returnVariable } } );
      }
      else
      {
        return replaceWithMap( dataDeclarationsTemplate, { { "dataType", returnType }, { "dataVariable", returnVariable } } );
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
                                 { "structType", stripPostfix( command.params[vectorParamIt->first].type.compose( "Vk" ), "*" ) },
                                 { "structVariable", startLowerCase( stripPrefix( command.params[vectorParamIt->first].name, "p" ) ) },
                                 { "vectorAllocator", stripPrefix( vectorAllocator, "," ) } } );
      }
      else
      {
        std::string vectorAllocator = ( ( flavourFlags & CommandFlavourFlagBits::withAllocator ) && !( flavourFlags & CommandFlavourFlagBits::unique ) )
                                      ? ( ", {}, " + startLowerCase( allocator ) )
                                      : "";
        std::string vectorSize      = getVectorSize( command.params, vectorParams, returnParams[0], dataTypes[0], templatedParams );

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

    std::string dataVariable = startLowerCase( stripPrefix( command.params[returnParams[0]].name, "p" ) );

    std::string const dataDeclarationsTemplate = R"(${returnType} ${returnVariable};
    ${dataType} & ${dataVariable} = ${returnVariable}.template get<${dataType}>();)";

    return replaceWithMap( dataDeclarationsTemplate,
                           { { "dataType", dataTypes[0] },
                             { "dataVariable", dataVariable },
                             { "returnType", ( command.returnType.name == "void" ) ? returnType : "StructureChain<X, Y, Z...>" },
                             { "returnVariable", returnVariable } } );
  }
}

std::string VulkanHppGenerator::generateDataDeclarations2Returns( Command const &                           command,
                                                                  std::vector<size_t> const &               returnParams,
                                                                  std::map<size_t, VectorParamData> const & vectorParams,
                                                                  CommandFlavourFlags                       flavourFlags,
                                                                  bool                                      raii,
                                                                  std::vector<std::string> const &          dataTypes,
                                                                  std::string const &                       returnType,
                                                                  std::string const &                       returnVariable ) const
{
  assert( returnParams.size() == 2 );

  bool const chained       = flavourFlags & CommandFlavourFlagBits::chained;
  bool const singular      = flavourFlags & CommandFlavourFlagBits::singular;
  bool const withAllocator = flavourFlags & CommandFlavourFlagBits::withAllocator;

  switch ( vectorParams.size() )
  {
    case 0:
      assert( !singular );
      assert( !chained ||
              ( isStructureChainAnchor( command.params[returnParams[0]].type.name ) &&
                containsByName( m_vkxml.types.externals, command.params[returnParams[1]].type.name ) ) ||
              ( ( containsByName( m_vkxml.types.externals, command.params[returnParams[0]].type.name ) ||
                  isHandleType( command.params[returnParams[0]].type.name ) ) &&
                isStructureChainAnchor( command.params[returnParams[1]].type.name ) ) );
      {
        std::string const dataDeclarationTemplate         = R"(    std::pair<${firstDataType},${secondDataType}> data_;
    ${firstDataType} & ${firstDataVariable} = data_.first;
    ${secondDataType} & ${secondDataVariable} = data_.second;)";
        std::string const dataDeclarationTemplateChained0 = R"(    std::pair<StructureChain<X, Y, Z...>,${secondDataType}> data_;
    ${firstDataType} & ${firstDataVariable} = data_.first.get();
    ${secondDataType} & ${secondDataVariable} = data_.second;)";
        std::string const dataDeclarationTemplateChained1 = R"(    std::pair<${firstDataType}, StructureChain<X, Y, Z...>> data_;
    ${firstDataType} & ${firstDataVariable} = data_.first;
    ${secondDataType} & ${secondDataVariable} = data_.second.get();)";

        std::string firstDataVariable  = startLowerCase( stripPrefix( command.params[returnParams[0]].name, "p" ) );
        std::string secondDataVariable = startLowerCase( stripPrefix( command.params[returnParams[1]].name, "p" ) );
        return replaceWithMap(
          chained ? ( isStructureChainAnchor( command.params[returnParams[0]].type.name ) ? dataDeclarationTemplateChained0 : dataDeclarationTemplateChained1 )
                  : dataDeclarationTemplate,
          { { "firstDataType", dataTypes[0] },
            { "firstDataVariable", firstDataVariable },
            { "secondDataType", dataTypes[1] },
            { "secondDataVariable", secondDataVariable } } );
      }
    case 1:
      assert( ( returnParams[0] == vectorParams.begin()->second.lenParam ) && ( returnParams[1] == vectorParams.begin()->first ) && !singular );
      {
        std::string counterVariable = startLowerCase( stripPrefix( command.params[returnParams[0]].name, "p" ) );
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
          std::string vectorVariable            = startLowerCase( stripPrefix( command.params[returnParams[1]].name, "p" ) );

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
      assert( !chained );
      {
        std::string firstDataVariable  = startLowerCase( stripPrefix( command.params[returnParams[0]].name, "p" ) );
        std::string secondDataVariable = startLowerCase( stripPrefix( command.params[returnParams[1]].name, "p" ) );
        if ( ( returnParams[0] == std::next( vectorParams.begin() )->first ) && !vectorParams.contains( returnParams[1] ) )
        {
          // the first return is the second vector param, the second return is just a value
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
            std::string vectorSize = startLowerCase( stripPrefix( command.params[vectorParams.begin()->first].name, "p" ) ) + ".size()";

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
        else
        {
          assert( !singular );
          assert( ( vectorParams.begin()->first == returnParams[0] ) && ( std::next( vectorParams.begin() )->first == returnParams[1] ) &&
                  ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam ) );

          // the two returns are vectors with the same len parameter
          std::string firstAllocatorType       = raii ? "" : ( startUpperCase( stripPrefix( dataTypes[0], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator" );
          std::string firstAllocateInitializer = withAllocator ? ( ", " + startLowerCase( firstAllocatorType ) ) : "";
          if ( !raii )
          {
            firstAllocatorType = ", " + firstAllocatorType;
          }
          std::string secondAllocatorType       = raii ? "" : ( startUpperCase( stripPrefix( dataTypes[1], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator" );
          std::string secondAllocateInitializer = withAllocator ? ( ", " + startLowerCase( secondAllocatorType ) ) : "";
          if ( !raii )
          {
            secondAllocatorType = ", " + secondAllocatorType;
          }
          std::string vectorSize = getVectorSize( command.params, vectorParams, returnParams[0], dataTypes[0], {} );

          std::string const dataDeclarationTemplate =
            R"(std::pair<std::vector<${firstDataType}${firstAllocatorType}>,std::vector<${secondDataType}${secondAllocatorType}>> data_( std::piecewise_construct, std::forward_as_tuple( ${vectorSize}${firstAllocateInitializer} ), std::forward_as_tuple( ${vectorSize}${secondAllocateInitializer} ) );
    std::vector<${firstDataType}${firstAllocatorType}> & ${firstDataVariable} = data_.first;
    std::vector<${secondDataType}${secondAllocatorType}> & ${secondDataVariable} = data_.second;)";

          return replaceWithMap( dataDeclarationTemplate,
                                 { { "firstAllocateInitializer", firstAllocateInitializer },
                                   { "firstAllocatorType", firstAllocatorType },
                                   { "firstDataType", dataTypes[0] },
                                   { "firstDataVariable", firstDataVariable },
                                   { "secondAllocateInitializer", secondAllocateInitializer },
                                   { "secondAllocatorType", secondAllocatorType },
                                   { "secondDataType", dataTypes[1] },
                                   { "secondDataVariable", secondDataVariable },
                                   { "vectorSize", vectorSize } } );
        }
      }
      break;
    case 3:
      {
        // the first vector param is not a return param
        assert( !chained );
        assert( std::ranges::none_of( returnParams, [&vectorParams]( auto returnParam ) { return vectorParams.begin()->first == returnParam; } ) );
        assert( std::next( vectorParams.begin() )->first == returnParams[0] );               // the second vector param is the first return param
        assert( std::next( std::next( vectorParams.begin() ) )->first == returnParams[1] );  // the third vector param is the second return param
        assert( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam );
        assert( vectorParams.begin()->second.lenParam == std::next( std::next( vectorParams.begin() ) )->second.lenParam );
        {
          std::string firstDataVariable  = startLowerCase( stripPrefix( command.params[returnParams[0]].name, "p" ) );
          std::string secondDataVariable = startLowerCase( stripPrefix( command.params[returnParams[1]].name, "p" ) );
          if ( singular )
          {
            firstDataVariable  = stripPluralS( firstDataVariable );
            secondDataVariable = stripPluralS( secondDataVariable );

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
            std::string firstAllocatorType       = raii ? "" : ( startUpperCase( stripPrefix( dataTypes[0], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator" );
            std::string firstAllocateInitializer = withAllocator ? ( ", " + startLowerCase( firstAllocatorType ) ) : "";
            if ( !raii )
            {
              firstAllocatorType = ", " + firstAllocatorType;
            }
            std::string secondAllocatorType       = raii ? "" : ( startUpperCase( stripPrefix( dataTypes[1], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator" );
            std::string secondAllocateInitializer = withAllocator ? ( ", " + startLowerCase( secondAllocatorType ) ) : "";
            if ( !raii )
            {
              secondAllocatorType = ", " + secondAllocatorType;
            }
            std::string vectorSize = startLowerCase( stripPrefix( command.params[vectorParams.begin()->first].name, "p" ) ) + ".size()";

            std::string const dataDeclarationTemplate =
              R"(std::pair<std::vector<${firstDataType}${firstAllocatorType}>,std::vector<${secondDataType}${secondAllocatorType}>> data_( std::piecewise_construct, std::forward_as_tuple( ${vectorSize}${firstAllocateInitializer} ), std::forward_as_tuple( ${vectorSize}${secondAllocateInitializer} ) );
    std::vector<${firstDataType}${firstAllocatorType}> & ${firstDataVariable} = data_.first;
    std::vector<${secondDataType}${secondAllocatorType}> & ${secondDataVariable} = data_.second;)";

            return replaceWithMap( dataDeclarationTemplate,
                                   { { "firstAllocateInitializer", firstAllocateInitializer },
                                     { "firstAllocatorType", firstAllocatorType },
                                     { "firstDataType", dataTypes[0] },
                                     { "firstDataVariable", firstDataVariable },
                                     { "secondAllocateInitializer", secondAllocateInitializer },
                                     { "secondAllocatorType", secondAllocatorType },
                                     { "secondDataType", dataTypes[1] },
                                     { "secondDataVariable", secondDataVariable },
                                     { "vectorSize", vectorSize } } );
          }
        }
      }
      break;
    default: assert( false ); return "";
  }
}

std::string VulkanHppGenerator::generateDataDeclarations3Returns( Command const &                           command,
                                                                  std::vector<size_t> const &               returnParams,
                                                                  std::map<size_t, VectorParamData> const & vectorParams,
                                                                  CommandFlavourFlags                       flavourFlags,
                                                                  bool                                      raii,
                                                                  std::vector<std::string> const &          dataTypes ) const
{
  switch ( vectorParams.size() )
  {
    case 0:
      {
        std::string const dataDeclarationsTemplate = R"(    std::tuple<${firstType}, ${secondType}, ${thirdType}> data_;
    ${firstType} &${firstVariable} = std::get<0>(data_);
    ${secondType} &${secondVariable} = std::get<1>(data_);
    ${thirdType} &${thirdVariable} = std::get<2>(data_);)";

        return replaceWithMap( dataDeclarationsTemplate,
                               { { "firstType", dataTypes[0] },
                                 { "firstVariable", startLowerCase( stripPrefix( command.params[returnParams[0]].name, "p" ) ) },
                                 { "secondType", dataTypes[1] },
                                 { "secondVariable", startLowerCase( stripPrefix( command.params[returnParams[1]].name, "p" ) ) },
                                 { "thirdType", dataTypes[2] },
                                 { "thirdVariable", startLowerCase( stripPrefix( command.params[returnParams[2]].name, "p" ) ) } } );
      }
      break;
    case 1:
      if ( ( returnParams[0] == vectorParams.begin()->second.lenParam ) && ( returnParams[1] == vectorParams.begin()->first ) )
      {
        assert( !( flavourFlags & CommandFlavourFlagBits::chained ) );

        // first a vector, then a value
        std::string counterVariable = startLowerCase( stripPrefix( command.params[returnParams[0]].name, "p" ) );
        std::string vectorVariable  = startLowerCase( stripPrefix( command.params[returnParams[1]].name, "p" ) );
        std::string valueVariable   = startLowerCase( stripPrefix( command.params[returnParams[2]].name, "p" ) );
        std::string vectorAllocatorType, pairConstructor;
        if ( !raii )
        {
          vectorAllocatorType = startUpperCase( stripPrefix( dataTypes[1], "VULKAN_HPP_NAMESPACE::" ) ) + "Allocator";
          pairConstructor =
            ( flavourFlags & CommandFlavourFlagBits::withAllocator )
              ? ( "( std::piecewise_construct, std::forward_as_tuple( " + startLowerCase( vectorAllocatorType ) + " ), std::forward_as_tuple() )" )
              : "";
          vectorAllocatorType = ", " + vectorAllocatorType;
        }

        std::string const dataDeclarationsTemplate =
          R"(std::pair<std::vector<${vectorElementType}${vectorAllocatorType}>,${valueType}> data_${pairConstructor};
    std::vector<${vectorElementType}${vectorAllocatorType}> & ${vectorVariable} = data_.first;
    ${valueType} & ${valueVariable} = data_.second;
    ${counterType} ${counterVariable};)";

        return replaceWithMap( dataDeclarationsTemplate,
                               { { "counterType", dataTypes[0] },
                                 { "counterVariable", counterVariable },
                                 { "vectorAllocatorType", vectorAllocatorType },
                                 { "vectorElementType", dataTypes[1] },
                                 { "vectorVariable", vectorVariable },
                                 { "pairConstructor", pairConstructor },
                                 { "valueType", dataTypes[2] },
                                 { "valueVariable", valueVariable } } );
      }
      else
      {
        assert( ( returnParams[1] == vectorParams.begin()->second.lenParam ) && ( returnParams[2] == vectorParams.begin()->first ) );

        std::string valueVariable   = startLowerCase( stripPrefix( command.params[returnParams[0]].name, "p" ) );
        std::string counterVariable = startLowerCase( stripPrefix( command.params[returnParams[1]].name, "p" ) );
        std::string vectorVariable  = startLowerCase( stripPrefix( command.params[returnParams[2]].name, "p" ) );
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
          R"(std::pair<StructureChain<X,Y,Z...>,std::vector<${vectorElementType}${vectorAllocatorType}>> data_${pairConstructor};
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

        std::string counterVariable      = startLowerCase( stripPrefix( command.params[returnParams[0]].name, "p" ) );
        std::string firstVectorVariable  = startLowerCase( stripPrefix( command.params[returnParams[1]].name, "p" ) );
        std::string secondVectorVariable = startLowerCase( stripPrefix( command.params[returnParams[2]].name, "p" ) );
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

std::string VulkanHppGenerator::generateDataPreparation( Command const &                           command,
                                                         size_t                                    initialSkipCount,
                                                         std::vector<size_t> const &               returnParams,
                                                         std::map<size_t, VectorParamData> const & vectorParams,
                                                         std::set<size_t> const &                  templatedParams,
                                                         CommandFlavourFlags                       flavourFlags,
                                                         bool                                      enumerating,
                                                         std::vector<std::string> const &          dataTypes ) const
{
  bool const chained  = flavourFlags & CommandFlavourFlagBits::chained;
  bool const singular = flavourFlags & CommandFlavourFlagBits::singular;
  bool const unique   = flavourFlags & CommandFlavourFlagBits::unique;

  if ( unique && ( returnParams.size() == 2 ) && vectorParams.contains( returnParams[0] ) && vectorParams.contains( returnParams[1] ) &&
       isHandleType( command.params[returnParams[0]].type.name ) && ( m_enums.find( command.params[returnParams[1]].type.name ) != m_enums.end() ) )
  {
    if ( singular )
    {
      std::string const dataPreparationTemplate =
        R"(std::pair<UniqueHandle<${handleType}, Dispatch>, ${enumType}> uniqueData_;
    detail::ObjectDestroy<${handleType}, Dispatch> deleter( *this, allocator, d );
    uniqueData_.first = UniqueHandle<${handleType}, Dispatch>( data_.first, deleter );
    uniqueData_.second = data_.second;)";

      return replaceWithMap( dataPreparationTemplate, { { "enumType", dataTypes[1] }, { "handleType", dataTypes[0] } } );
    }
    else
    {
      std::string const dataPreparationTemplate =
        R"(std::pair<std::vector<UniqueHandle<${handleType}, Dispatch>, ${handleType}Allocator>, std::vector<${enumType}>> uniqueData_;
    uniqueData_.first.reserve( ${vectorName}.size() );
    detail::ObjectDestroy<${handleType}, Dispatch> deleter( *this, allocator, d );
    for ( auto const & ${elementName} : data_.first )
    {
      uniqueData_.first.push_back( UniqueHandle<${handleType}, Dispatch>( ${elementName}, deleter ) );
    }
    uniqueData_.second = std::move( data_.second );)";

      return replaceWithMap( dataPreparationTemplate,
                             { { "elementName", stripPluralS( startLowerCase( stripPrefix( command.params[returnParams[0]].name, "p" ) ) ) },
                               { "enumType", dataTypes[1] },
                               { "handleType", dataTypes[0] },
                               { "vectorName", startLowerCase( stripPrefix( command.params[vectorParams.begin()->first].name, "p" ) ) } } );
    }
  }

  auto vectorParamIt = ( 1 < returnParams.size() ) ? vectorParams.find( returnParams[1] ) : vectorParams.end();
  if ( vectorParamIt != vectorParams.end() )
  {
    assert( !unique );

    std::string vectorName = startLowerCase( stripPrefix( command.params[vectorParamIt->first].name, "p" ) );

    if ( chained )
    {
      assert( !singular );
      assert( templatedParams.empty() );
      assert( returnParams.size() == 2 );
      assert( !vectorParams.contains( returnParams[0] ) );
      assert( ( vectorParamIt != vectorParams.end() ) && ( vectorParamIt->second.lenParam == returnParams[0] ) );

      std::string vectorElementType = stripPostfix( command.params[vectorParamIt->first].type.compose( "Vk" ), " *" );

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
                               { { "counterName", startLowerCase( stripPrefix( command.params[vectorParamIt->second.lenParam].name, "p" ) ) },
                                 { "counterType", command.params[vectorParamIt->second.lenParam].type.name },
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
                               { { "counterName", startLowerCase( stripPrefix( command.params[vectorParamIt->second.lenParam].name, "p" ) ) },
                                 { "counterType", command.params[vectorParamIt->second.lenParam].type.name },
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
        assert( std::ranges::any_of( returnParams, [&vp]( size_t rp ) { return rp == vp.first; } ) &&
                std::ranges::any_of( returnParams, [&vp]( size_t rp ) { return rp == vp.second.lenParam; } ) );
        resizes += startLowerCase( stripPrefix( command.params[vp.first].name, "p" ) ) + ".resize( " +
                   startLowerCase( stripPrefix( command.params[vp.second.lenParam].name, "p" ) ) + " );\n";
      }
      resizes.pop_back();

      std::string const dataPreparationTemplate =
        R"(VULKAN_HPP_ASSERT( ${counterName} <= ${vectorName}.size() );
    if ( ${counterName} < ${vectorName}.size() )
    {
      ${resizes}
    })";

      return replaceWithMap( dataPreparationTemplate,
                             { { "counterName", startLowerCase( stripPrefix( command.params[vectorParamIt->second.lenParam].name, "p" ) ) },
                               { "resizes", resizes },
                               { "vectorName", vectorName } } );
    }
  }
  else if ( unique && !singular && ( returnParams.size() == 1 ) && vectorParams.contains( returnParams[0] ) )
  {
    std::string className = initialSkipCount ? stripPrefix( command.params[initialSkipCount - 1].type.name, "Vk" ) : "";
    std::string deleterDefinition, vectorName, vectorSize;
    vectorParamIt = vectorParams.find( returnParams[0] );
    if ( vectorParamIt != vectorParams.end() && vectorParamIt->second.byStructure )
    {
      deleterDefinition = "detail::ObjectDestroy<" + className + ", Dispatch> deleter( *this, allocator, d )";
      auto structIt     = findByName( m_vkxml.types.structs, command.params[returnParams[0]].type.name );
      assert( structIt != m_vkxml.types.structs.end() );
      vectorName = startLowerCase( stripPrefix( structIt->members.back().name, "p" ) );
      vectorSize = vectorName + ".size()";
    }
    else
    {
      std::vector<std::string> lenParts = tokenize( command.params[returnParams[0]].len, "->" );
      switch ( lenParts.size() )
      {
        case 1: deleterDefinition = "detail::ObjectDestroy<" + className + ", Dispatch> deleter( *this, allocator, d )"; break;
        case 2:
          {
            auto vpiIt = vectorParams.find( returnParams[0] );
            assert( vpiIt != vectorParams.end() );
            std::string poolType, poolName;
            std::tie( poolType, poolName ) = getPoolTypeAndName( command.params[vpiIt->second.lenParam].type.name );
            assert( !poolType.empty() );
            poolType          = stripPrefix( poolType, "Vk" );
            poolName          = startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + poolName;
            deleterDefinition = "detail::PoolFree<" + className + ", " + poolType + ", Dispatch> deleter( *this, " + poolName + ", d )";
          }
          break;
        default: assert( false ); break;
      }
      vectorName = startLowerCase( stripPrefix( command.params[returnParams[0]].name, "p" ) );
      vectorSize = getVectorSize( command.params, vectorParams, returnParams[0], command.params[returnParams[0]].type.name, templatedParams );
    }

    std::string handleType       = stripPrefix( dataTypes[0], "VULKAN_HPP_NAMESPACE::" );
    std::string vectorAllocator  = ( flavourFlags & CommandFlavourFlagBits::withAllocator ) ? ( "( " + startLowerCase( handleType ) + "Allocator )" ) : "";
    std::string elementName      = stripPluralS( vectorName );
    std::string uniqueVectorName = "unique" + startUpperCase( vectorName );

    std::string const dataPreparationTemplate =
      R"(std::vector<UniqueHandle<${handleType}, Dispatch>, ${handleType}Allocator> ${uniqueVectorName}${vectorAllocator};
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

std::string VulkanHppGenerator::generateDataSizeChecks( Command const &                           command,
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
    std::string const dataSizeCheckTemplate = R"(    VULKAN_HPP_ASSERT( ${dataSize} % sizeof( ${dataType} ) == 0 );)";
    for ( size_t i = 0; i < returnParams.size(); i++ )
    {
      auto vectorParamIt = vectorParams.find( returnParams[i] );
      if ( ( vectorParamIt != vectorParams.end() ) && templatedParams.contains( returnParams[i] ) &&
           std::ranges::none_of( returnParams, [&vectorParamIt]( size_t rp ) noexcept { return rp == vectorParamIt->second.lenParam; } ) )
      {
        dataSizeChecks +=
          replaceWithMap( dataSizeCheckTemplate, { { "dataSize", command.params[vectorParamIt->second.lenParam].name }, { "dataType", returnParamTypes[i] } } );
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
  return containsByNameOrAlias( enumIt->second.values, debugReportObjectType ) ? generateEnumValueName( enumIt->first, debugReportObjectType, false )
                                                                               : "eUnknown";
}

std::string VulkanHppGenerator::generateDecoratedReturnType(
  Command const & command, std::vector<size_t> const & returnParams, bool enumerating, CommandFlavourFlags flavourFlags, std::string const & returnType ) const
{
  bool const chained = flavourFlags & CommandFlavourFlagBits::chained;
#if !defined( NDEBUG )
  const bool unique = flavourFlags & CommandFlavourFlagBits::unique;
#endif
  assert( !( chained && unique ) );

  std::string decoratedReturnType;
  if ( ( 1 < command.successCodes.size() ) && returnParams.empty() && !chained )
  {
    assert( ( command.returnType.name == "VkResult" ) && !unique );
    decoratedReturnType = "Result";
  }
  else if ( ( command.returnType.name != "VkResult" ) && ( command.returnType.name != "void" ) && returnParams.empty() )
  {
    assert( !chained && !unique );
    if ( command.returnType.name.starts_with( "Vk" ) )
    {
      decoratedReturnType = stripPrefix( command.returnType.name, "Vk" );
    }
    else if ( command.returnType.name.starts_with( "PFN_vk" ) )
    {
      decoratedReturnType = "PFN_" + stripPrefix( command.returnType.name, "PFN_vk" );
    }
    else
    {
      decoratedReturnType = command.returnType.name;
    }
  }
  else if ( ( command.returnType.name == "void" ) ||
            ( ( command.returnType.name == "VkResult" ) && ( command.successCodes.size() == 1 ) && command.errorCodes.empty() ) )
  {
    assert( !unique );
    assert( ( command.returnType.name != "void" ) || ( returnParams.size() <= 2 ) );
    decoratedReturnType = returnType;
  }
  else if ( command.returnType.name == "VkResult" )
  {
    assert( !command.successCodes.empty() && ( command.successCodes[0] == "VK_SUCCESS" ) );
    if ( ( command.successCodes.size() == 1 ) || ( ( command.successCodes.size() == 2 ) && ( command.successCodes[1] == "VK_INCOMPLETE" ) && enumerating ) )
    {
      decoratedReturnType = "typename ResultValueType<" + returnType + ">::type";

      if ( std::ranges::find( command.errorCodes, "VK_ERROR_OUT_OF_DATE_KHR" ) != command.errorCodes.end() )
      {
        static std::string decoratedReturnTypeTemplate = R"(
#if defined( VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS )
  ResultValue<${returnType}>
#else
  ${decoratedReturnType}
#endif
)";

        decoratedReturnType = replaceWithMap( decoratedReturnTypeTemplate, { { "decoratedReturnType", decoratedReturnType }, { "returnType", returnType } } );
      }
    }
    else if ( !command.errorCodes.empty() && ( 1 < command.successCodes.size() ) )
    {
      decoratedReturnType = "ResultValue<" + returnType + ">";
    }
    else
    {
      assert( false );
    }
  }
  else
  {
    decoratedReturnType = "std::pair<" + command.returnType.compose( "Vk" ) + ", " + returnType + ">";
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
                             reinterpret_cast<PFN_AllocationFunction>( pfnAllocation_ ),
                             reinterpret_cast<PFN_ReallocationFunction>( pfnReallocation_ ),
                             reinterpret_cast<PFN_FreeFunction>( pfnFree_ ),
                             reinterpret_cast<PFN_InternalAllocationNotification>( pfnInternalAllocation_ ),
                             reinterpret_cast<PFN_InternalFreeNotification>( pfnInternalFree_ ) )
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
    DebugReportCallbackCreateInfoEXT( DebugReportFlagsEXT           flags_,
                                      PFN_vkDebugReportCallbackEXT  pfnCallback_,
                                      void *                        pUserData_   = {},
                                      const void *                  pNext_       = nullptr ) VULKAN_HPP_NOEXCEPT
      : DebugReportCallbackCreateInfoEXT( flags_, reinterpret_cast<PFN_DebugReportCallbackEXT>( pfnCallback_ ), pUserData_, pNext_ )
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
    DebugUtilsMessengerCreateInfoEXT( DebugUtilsMessengerCreateFlagsEXT    flags_,
                                      DebugUtilsMessageSeverityFlagsEXT    messageSeverity_,
                                      DebugUtilsMessageTypeFlagsEXT        messageType_,
                                      PFN_vkDebugUtilsMessengerCallbackEXT pfnUserCallback_,
                                      void *                               pUserData_       = {},
                                      const void *                         pNext_ = nullptr ) VULKAN_HPP_NOEXCEPT
      : DebugUtilsMessengerCreateInfoEXT(
          flags_, messageSeverity_, messageType_, reinterpret_cast<PFN_DebugUtilsMessengerCallbackEXT>( pfnUserCallback_ ), pUserData_, pNext_ )
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
    DeviceDeviceMemoryReportCreateInfoEXT( DeviceMemoryReportFlagsEXT          flags_,
                                           PFN_vkDeviceMemoryReportCallbackEXT pfnUserCallback_,
                                           void *                              pUserData_       = {},
                                           const void *                        pNext_ = nullptr ) VULKAN_HPP_NOEXCEPT
      : DeviceDeviceMemoryReportCreateInfoEXT( flags_, reinterpret_cast<PFN_DeviceMemoryReportCallbackEXT>( pfnUserCallback_ ), pUserData_, pNext_ )
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
      return setPfnAllocation( reinterpret_cast<PFN_AllocationFunction>( pfnAllocation_ ) );
    }

    VULKAN_HPP_DEPRECATED( "This setter is deprecated. Use the one taking a function pointer type from the vk-namespace instead." )
    AllocationCallbacks & setPfnReallocation( PFN_vkReallocationFunction pfnReallocation_ ) VULKAN_HPP_NOEXCEPT
    {
      return setPfnReallocation( reinterpret_cast<PFN_ReallocationFunction>( pfnReallocation_ ) );
    }

    VULKAN_HPP_DEPRECATED( "This setter is deprecated. Use the one taking a function pointer type from the vk-namespace instead." )
    AllocationCallbacks & setPfnInternalAllocation( PFN_vkInternalAllocationNotification pfnInternalAllocation_ ) VULKAN_HPP_NOEXCEPT
    {
      return setPfnInternalAllocation( reinterpret_cast<PFN_InternalAllocationNotification>( pfnInternalAllocation_ ) );
    }

    VULKAN_HPP_DEPRECATED( "This setter is deprecated. Use the one taking a function pointer type from the vk-namespace instead." )
    AllocationCallbacks & setPfnInternalFree( PFN_vkInternalFreeNotification pfnInternalFree_ ) VULKAN_HPP_NOEXCEPT
    {
      return setPfnInternalFree( reinterpret_cast<PFN_InternalFreeNotification>( pfnInternalFree_ ) );
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
      return setPfnCallback( reinterpret_cast<PFN_DebugReportCallbackEXT>( pfnCallback_ ) );
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
      return setPfnUserCallback( reinterpret_cast<PFN_DebugUtilsMessengerCallbackEXT>( pfnUserCallback_ ) );
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
      return setPfnUserCallback( reinterpret_cast<PFN_DeviceMemoryReportCallbackEXT>( pfnUserCallback_ ) );
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
  std::string const dispatchLoaderDynamicTemplate = readSnippet( "DispatchLoaderDynamic.hpp" );

  std::string           commandMembers, deviceCommandAssignments, initialCommandAssignments, instanceCommandAssignments;
  std::set<std::string> listedCommands;  // some commands are listed with more than one extension!

  for ( auto const & feature : m_vkxml.features )
  {
    commandMembers += generateDispatchLoaderDynamicCommandMembers( feature.require, listedCommands, feature.name );
    initialCommandAssignments += generateDispatchLoaderDynamicInitialCommandAssignment( feature.require, listedCommands, feature.name );
    instanceCommandAssignments += generateDispatchLoaderDynamicInstanceCommandAssignment( feature.require, listedCommands, feature.name );
    deviceCommandAssignments += generateDispatchLoaderDynamicDeviceCommandAssignment( feature.require, listedCommands, feature.name );
    forEachRequiredCommand( feature.require, [&listedCommands]( std::string const & name, auto const & ) { listedCommands.insert( name ); } );
  }
  for ( auto const & extension : m_extensions )
  {
    commandMembers += generateDispatchLoaderDynamicCommandMembers( extension.requireData, listedCommands, extension.name );
    initialCommandAssignments += generateDispatchLoaderDynamicInitialCommandAssignment( extension.requireData, listedCommands, extension.name );
    instanceCommandAssignments += generateDispatchLoaderDynamicInstanceCommandAssignment( extension.requireData, listedCommands, extension.name );
    deviceCommandAssignments += generateDispatchLoaderDynamicDeviceCommandAssignment( extension.requireData, listedCommands, extension.name );
    forEachRequiredCommand( extension.requireData, [&listedCommands]( std::string const & name, auto const & ) { listedCommands.insert( name ); } );
  }

  return replaceWithMap( dispatchLoaderDynamicTemplate,
                         { { "commandMembers", commandMembers },
                           { "deviceCommandAssignments", deviceCommandAssignments },
                           { "initialCommandAssignments", initialCommandAssignments },
                           { "instanceCommandAssignments", instanceCommandAssignments } } );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateDispatchLoaderDynamicCommandMembers( std::vector<RequireType> const & requireTypes,
                                                                             std::set<std::string> const &    listedCommands,
                                                                             std::string const &              title ) const
{
  std::string members, placeholders;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & )
                          {
                            if ( !listedCommands.contains( name ) )
                            {
                              members += "    PFN_" + name + " " + name + " = 0;\n";
                              placeholders += "    PFN_dummy " + name + "_placeholder = 0;\n";
                            }
                          } );
  return addTitleAndProtection( title, members, placeholders );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateDispatchLoaderDynamicDeviceCommandAssignment( std::vector<RequireType> const & requireTypes,
                                                                                      std::set<std::string> const &    listedCommands,
                                                                                      std::string const &              title ) const
{
  std::string deviceCommandAssignments;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & command )
                          {
                            // ignore vkGetDeviceProcAddr as it is already part of instance command assignments
                            if ( !listedCommands.contains( name ) && ( name != "vkGetDeviceProcAddr" ) && isHandleType( command.params[0].type.name ) &&
                                 isDeviceCommand( command ) )
                            {
                              deviceCommandAssignments += generateDispatchLoaderDynamicCommandAssignment( name, command.name, "device" );
                            }
                          } );
  return addTitleAndProtection( title, deviceCommandAssignments );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateDispatchLoaderDynamicInitialCommandAssignment( std::vector<RequireType> const & requireTypes,
                                                                                       std::set<std::string> const &    listedCommands,
                                                                                       std::string const &              title ) const
{
  std::string initialCommandAssignments;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & command )
                          {
                            if ( !listedCommands.contains( name ) && !isHandleType( command.params[0].type.name ) )
                            {
                              initialCommandAssignments += generateDispatchLoaderDynamicCommandAssignment( name, command.name, "NULL" );
                            }
                          } );
  return addTitleAndProtection( title, initialCommandAssignments );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateDispatchLoaderDynamicInstanceCommandAssignment( std::vector<RequireType> const & requireTypes,
                                                                                        std::set<std::string> const &    listedCommands,
                                                                                        std::string const &              title ) const
{
  std::string instanceCommandAssignments;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & command )
                          {
                            if ( !listedCommands.contains( name ) && isHandleType( command.params[0].type.name ) )
                            {
                              instanceCommandAssignments += generateDispatchLoaderDynamicCommandAssignment( name, command.name, "instance" );
                            }
                          } );
  return addTitleAndProtection( title, instanceCommandAssignments );
}

std::string VulkanHppGenerator::generateDispatchLoaderStatic() const
{
  std::string const dispatchLoaderStaticTemplate = R"(
#if !defined( VK_NO_PROTOTYPES ) || ( defined( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC ) && ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 0 ) )
  class DispatchLoaderStatic : public DispatchLoaderBase
  {
  public:
  // These commands are listed as `VULKAN_HPP_INLINE` to account for P1779R3: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p1779r3.html
  // That is, member functions defined in a class definition in a module interface unit are no longer implicitly inline.
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
  for ( auto const & feature : m_vkxml.features )
  {
    commands += generateDispatchLoaderStaticCommands( feature.require, listedCommands, feature.name );
  }

  auto const [onlyexportedEnter, onlyexportedLeave] = generateProtection( "VK_ONLY_EXPORTED_PROTOTYPES", false );
  commands += onlyexportedEnter;

  for ( auto const & extension : m_extensions )
  {
    commands += generateDispatchLoaderStaticCommands( extension.requireData, listedCommands, extension.name );
  }
  commands += onlyexportedLeave;

  return replaceWithMap( dispatchLoaderStaticTemplate, { { "commands", commands } } );
}

std::string VulkanHppGenerator::generateDestroyCommand( std::string const & name, Command const & command ) const
{
  // special handling for destroy functions, filter out alias functions
  std::string commandName = generateCommandName( name, command.params, 1 );
  if ( containsByName( m_vkxml.commands, name ) &&
       ( ( ( name.substr( 2, 7 ) == "Destroy" ) && ( commandName != "destroy" ) ) || ( name.substr( 2, 4 ) == "Free" ) ||
         ( name == "vkReleasePerformanceConfigurationINTEL" ) || ( name == "vkReleaseDisplayEXT" ) ) )
  {
    assert( 1 < command.params.size() );
    // make sure, the object to destroy/free/release is not optional in the shortened version!
    Command localCommand = command;
    localCommand.params[1].optional.clear();

    std::string destroyCommandString = generateCommand( name, localCommand, 1, false, false );
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
      assert( ( name == "vkReleasePerformanceConfigurationINTEL" ) || ( name == "vkReleaseDisplayEXT" ) );
      shortenedName = "release";
    }
    size_t pos = destroyCommandString.find( commandName );
    while ( pos != std::string::npos )
    {
      destroyCommandString.replace( pos, commandName.length(), shortenedName );
      pos = destroyCommandString.find( commandName, pos );
    }
    // we need to remove the default argument for the first argument, to prevent ambiguities!
    assert( 1 < command.params.size() );
    pos = destroyCommandString.find( command.params[1].name );  // skip the standard version of the function
    assert( pos != std::string::npos );
    pos = destroyCommandString.find( command.params[1].name, pos + 1 );  // get the argument to destroy in the advanced version
    assert( pos != std::string::npos );
    pos = destroyCommandString.find( " VULKAN_HPP_DEFAULT_ASSIGNMENT( {} )", pos );
    if ( pos != std::string::npos )
    {
      destroyCommandString.erase( pos, strlen( " VULKAN_HPP_DEFAULT_ASSIGNMENT( {} )" ) );
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

template <typename RequireType>
std::string VulkanHppGenerator::generateDispatchLoaderStaticCommands( std::vector<RequireType> const & requireTypes,
                                                                      std::set<std::string> &          listedCommands,
                                                                      std::string const &              title ) const
{
  std::string str;
  forEachRequiredCommand(
    requireTypes,
    [&]( std::string const & name, auto const & command )
    {
      // some commands are listed for multiple extensions !
      if ( listedCommands.insert( name ).second )
      {
        str += "\n";
        std::string parameterList, parameters;
        assert( !command.params.empty() );
        for ( auto param : command.params )
        {
          parameterList += param.type.compose( "" ) + " " + param.name + generateCArraySizes( param.arraySizes ) + ", ";
          parameters += param.name + ", ";
        }
        assert( parameterList.ends_with( ", " ) && parameters.ends_with( ", " ) );
        parameterList.resize( parameterList.size() - 2 );
        parameters.resize( parameters.size() - 2 );

        std::string const commandTemplate = R"(
    VULKAN_HPP_INLINE ${returnType} ${commandName}( ${parameterList} ) const VULKAN_HPP_NOEXCEPT
    {
      return ::${commandName}( ${parameters} );
    }
)";

        str += replaceWithMap(
          commandTemplate,
          { { "commandName", name }, { "parameterList", parameterList }, { "parameters", parameters }, { "returnType", command.returnType.name } } );
      }
    } );
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateEnum( std::pair<std::string, EnumData> const & enumData, std::string const & surroundingProtect ) const
{
  std::string baseType, bitmask;
  if ( enumData.second.isBitmask )
  {
    auto bitmaskIt = std::ranges::find_if( m_vkxml.types.bitmasks, [&enumData]( auto const & bitmask ) { return bitmask.require == enumData.first; } );
    assert( bitmaskIt != m_vkxml.types.bitmasks.end() );
    baseType = " : " + bitmaskIt->name;
    bitmask  = generateBitmask( bitmaskIt, surroundingProtect );
  }

  std::string                        enumValues, previousEnter, previousLeave;
  std::map<std::string, std::string> valueToNameMap;
  for ( auto const & value : enumData.second.values )
  {
    if ( value.supported )
    {
      std::string valueName = generateEnumValueName( enumData.first, value.name, enumData.second.isBitmask );
      checkForError( !valueName.empty(), value.xmlLine, "generated enum value name for <" + value.name + "> is empty for enum <" + enumData.first + ">" );
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
            std::string enumTag     = findTag( enumData.first, m_vkxml.tags.tags );
            std::string aliasTag    = findTag( enumAliasIt->first, m_vkxml.tags.tags );
            std::string valueTag    = findTag( valueAlias.name, m_vkxml.tags.tags );
            if ( ( stripPostfix( enumData.first, enumTag ) == stripPostfix( enumAliasIt->first, aliasTag ) ) && ( aliasTag == valueTag ) )
            {
              enumName = enumAliasIt->first;
            }
          }

          std::string aliasName = generateEnumValueName( enumName, valueAlias.name, enumData.second.isBitmask );
          // aliases starting with the wrong prefix, determined out of the enumName, are skipped!
          if ( !aliasName.empty() )
          {
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
                               std::ranges::any_of( value.aliases, [mapIt]( auto const & eav ) { return eav.name == mapIt->second; } ),
                             valueAlias.xmlLine,
                             "generated enum alias value name <" + aliasName + ">, generated from <" + valueAlias.name +
                               "> is already generated from different enum value <" + mapIt->second + ">" );
            }

            if ( ( enumName != enumData.first ) && ( findTag( enumName, m_vkxml.tags.tags ) != findTag( enumData.first, m_vkxml.tags.tags ) ) )
            {
              // the enum value was introduced with a tagged enum, but is now an alias of an enum with a different, potentially empty, tag
              // even though, there has never been this generated with that tag, we add a tagged version here for consistency reasons
              assert( findTag( valueAlias.name, m_vkxml.tags.tags ) == findTag( enumName, m_vkxml.tags.tags ) );
              aliasName                   = generateEnumValueName( enumData.first, valueAlias.name, enumData.second.isBitmask );
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
                                 std::ranges::any_of( value.aliases, [mapIt]( auto const & eav ) { return eav.name == mapIt->second; } ),
                               valueAlias.xmlLine,
                               "generated enum alias value name <" + aliasName + ">, generated from <" + valueAlias.name +
                                 "> is already generated from different enum value <" + mapIt->second + ">" );
              }
            }
          }
        }
      }

      previousEnter = enter;
      previousLeave = leave;
    }
  }
  enumValues += previousLeave;

  std::string wrapperClassComment;
  if ( !enumValues.empty() )
  {
    size_t const pos = enumValues.rfind( ',' );
    assert( pos != std::string::npos );
    enumValues.erase( pos, 1 );
    enumValues = "\n" + enumValues + "  ";

    wrapperClassComment =
      "// wrapper class for enum " + enumData.first + ", see https://registry.khronos.org/vulkan/specs/latest/man/html/" + enumData.first + ".html";
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

  std::string const enumTemplate =
    R"(  ${wrapperClassComment}
  enum class ${enumName}${baseType}{${enumValues}};
  ${typeTraits}
  ${enumUsing}
  ${bitmask})";

  return replaceWithMap( enumTemplate,
                         { { "baseType", baseType },
                           { "bitmask", bitmask },
                           { "enumName", stripPrefix( enumData.first, "Vk" ) },
                           { "enumUsing", enumUsing },
                           { "enumValues", enumValues },
                           { "typeTraits", typeTraits },
                           { "wrapperClassComment", wrapperClassComment } } );
}

std::string VulkanHppGenerator::generateEnums() const
{
  std::string const enumsTemplate = R"(
  //=============
  //=== ENUMs ===
  //=============

${enums}
)";

  std::string           enums;
  std::set<std::string> listedEnums;
  for ( auto const & feature : m_vkxml.features )
  {
    enums += generateEnums( feature.require, listedEnums, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    enums += generateEnums( extension.requireData, listedEnums, extension.name );
  }

  return replaceWithMap( enumsTemplate, { { "enums", enums } } );
}

template <typename RequireType>
std::string
  VulkanHppGenerator::generateEnums( std::vector<RequireType> const & requireTypes, std::set<std::string> & listedEnums, std::string const & title ) const
{
  std::string surroundingProtect = getProtectFromTitle( title );
  std::string str;
  for ( auto const & require : requireTypes )
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
  std::string const enumsToStringTemplate = R"(
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
  for ( auto const & feature : m_vkxml.features )
  {
    enumsToString += generateEnumsToString( feature.require, listedEnums, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    enumsToString += generateEnumsToString( extension.requireData, listedEnums, extension.name );
  }

  return replaceWithMap( enumsToStringTemplate, { { "enumsToString", enumsToString } } );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateEnumsToString( std::vector<RequireType> const & requireTypes,
                                                       std::set<std::string> &          listedEnums,
                                                       std::string const &              title ) const
{
  std::string str;
  for ( auto const & require : requireTypes )
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

std::string VulkanHppGenerator::generateEnumInitializer( Type const &                       type,
                                                         std::vector<std::string> const &   arraySizes,
                                                         std::vector<EnumValueData> const & values,
                                                         bool                               bitmask ) const
{
  // enum arguments might need special initialization
  assert( type.prefix.empty() && !values.empty() );
  std::string valueName = generateEnumValueName( type.name, values.front().name, bitmask );
  std::string value     = stripPrefix( type.name, "Vk" ) + "::" + valueName;
  std::string str;
  if ( arraySizes.empty() )
  {
    str += value;
  }
  else
  {
    assert( arraySizes.size() == 1 );
    auto      constIt = findByName( m_vkxml.constants.values, arraySizes[0] );
    int const count   = std::stoi( ( constIt == m_vkxml.constants.values.end() ) ? arraySizes[0] : constIt->value );
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
  bool        isEmpty = enumData.second.values.empty() || std::ranges::none_of( enumData.second.values, []( auto const & evd ) { return evd.supported; } );
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
        auto const [enter, leave] = generateProtection( value.protect );
        if ( previousEnter != enter )
        {
          cases += previousLeave + enter;
        }

        std::string const caseTemplate = R"(      case ${enumName}::e${valueName} : return "${valueName}";
)";
        cases += replaceWithMap(
          caseTemplate,
          { { "enumName", enumName }, { "valueName", generateEnumValueName( enumData.first, value.name, enumData.second.isBitmask ).substr( 1 ) } } );

        previousEnter = enter;
        previousLeave = leave;
      }
    }
    cases += previousLeave;

    std::string const functionBodyTemplate =
      R"x(    switch ( value )
    {
${cases}      default: return "invalid ( " + toHexString( static_cast<uint32_t>( value ) ) + " )";
    }
)x";

    functionBody = replaceWithMap( functionBodyTemplate, { { "cases", cases } } );
  }

  std::string const enumToStringTemplate = R"(
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_20 std::string to_string( ${enumName}${argument} )
  {
${functionBody}
  }
)";

  return replaceWithMap( enumToStringTemplate, { { "argument", isEmpty ? "" : " value" }, { "enumName", enumName }, { "functionBody", functionBody } } );
}

std::string VulkanHppGenerator::generateEnumValueName( std::string const & enumName, std::string const & valueName, bool bitmask ) const
{
  auto [prefix, postfix] = determineEnumSuffixes( enumName, bitmask, m_vkxml.tags.tags );
  std::string tag        = findTag( valueName, m_vkxml.tags.tags, "" );
  if ( postfix == "_" + tag )
  {
    tag = findTag( valueName, m_vkxml.tags.tags, postfix );
  }

  std::string result;
  if ( valueName.starts_with( prefix ) )
  {
    result = "e" + toCamelCase( stripPostfix( stripPrefix( valueName, prefix ), postfix ) );
    if ( bitmask )
    {
      size_t const pos = result.rfind( "Bit" );
      if ( pos != std::string::npos )
      {
        result.erase( pos, 3 );
      }
    }
    if ( !tag.empty() && ( result.substr( result.length() - tag.length() ) == toCamelCase( tag ) ) )
    {
      result = result.substr( 0, result.length() - tag.length() ) + tag;
    }
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
      std::string dependsPerExtension;
      if ( !extensionDependencies.empty() )
      {
        dependsPerExtension += ", ";
      }
      dependsPerExtension += "{ \"" + extension.name + "\", { ";
      for ( auto const & dependsByVersion : extension.depends )
      {
        dependsPerExtension += "{ \"" + dependsByVersion.first + "\", { ";
        if ( !dependsByVersion.second.empty() )
        {
          for ( auto const & depends : dependsByVersion.second )
          {
            dependsPerExtension += "{ ";
            for ( auto const & dep : depends )
            {
              dependsPerExtension += "\""s + dep + "\", ";
            }
            dependsPerExtension += " }, ";
          }
          assert( dependsPerExtension.ends_with( ", " ) );
          dependsPerExtension = dependsPerExtension.substr( 0, dependsPerExtension.length() - 2 );
        }
        dependsPerExtension += " } }, ";
      }
      assert( dependsPerExtension.ends_with( ", " ) );
      dependsPerExtension = dependsPerExtension.substr( 0, dependsPerExtension.length() - 2 );
      dependsPerExtension += " } }";

      auto const [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
      extensionDependencies += ( ( previousEnter != enter ) ? ( "\n" + previousLeave + enter ) : "\n" ) + dependsPerExtension;
      previousEnter = enter;
      previousLeave = leave;
    }
  }

  if ( !previousLeave.empty() )
  {
    extensionDependencies += "\n" + previousLeave;
  }
  return extensionDependencies;
}

template <class Predicate, class Extraction>
std::string VulkanHppGenerator::generateExtensionReplacedBy( Predicate p, Extraction e ) const
{
  std::string replacedBy, previousEnter, previousLeave;
  for ( auto const & extension : m_extensions )
  {
    if ( p( extension ) )
    {
      auto const [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
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
  for ( auto const & extension : m_extensions )
  {
    if ( p( extension ) )
    {
      bool firstTest            = replacedTest.empty();
      auto const [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
      replacedTest +=
        ( ( previousEnter != enter ) ? ( "\n" + previousLeave + enter ) : "\n" ) + ( firstTest ? "" : " || " ) + "( extension == \"" + extension.name + "\" )";
      previousEnter = enter;
      previousLeave = leave;
    }
  }
  if ( !previousLeave.empty() )
  {
    // the test ends on a protected extension -> add the previousLeave and a "false" to the end
    // in order to make it work in case the protection is not defined.
    assert( !replacedTest.empty() );
    replacedTest += "\n" + previousLeave + " || false";
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
      if ( !extensionsList.empty() )
      {
        extensionsList += ", ";
      }
      auto const [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
      extensionsList += ( ( previousEnter != enter ) ? ( "\n" + previousLeave + enter ) : "\n" ) + "\"" + extension.name + "\"";
      previousEnter = enter;
      previousLeave = leave;
    }
  }
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
      auto const [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
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
  assert( !m_formats.empty() );

  auto formatIt = m_enums.find( "VkFormat" );
  assert( formatIt != m_enums.end() );
  assert( formatIt->second.values.front().name == "VK_FORMAT_UNDEFINED" );

  auto noPredicate = []( auto const & ) { return true; };

  auto generateAllFormatsList = [&]( auto const formatIt ) { return generateFormatTraitsList( formatIt->second, noPredicate ); };
  auto generateAlphaCases     = [this]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData )
      { return std::ranges::any_of( formatData.components, []( auto const & component ) { return component.name == "A"; } ); },
      []( FormatData const & ) { return ""; } );
  };
  auto generateComponentCases =
    [this]( auto const & formatData, std::function<std::string( ComponentData const & componentData )> generator, std::string const & defaultReturn )
  {
    return generateFormatTraitsSubCases<ComponentData>(
      formatData,
      []( FormatData const & formatData ) -> std::vector<ComponentData> const & { return formatData.components; },
      "component",
      generator,
      defaultReturn );
  };
  auto generatePlaneCases =
    [this]( auto const & formatData, std::function<std::string( PlaneData const & PlaneData )> generator, std::string const & defaultReturn )
  {
    return generateFormatTraitsSubCases<PlaneData>(
      formatData, []( FormatData const & formatData ) -> std::vector<PlaneData> const & { return formatData.planes; }, "plane", generator, defaultReturn );
  };

  auto generateBlockExtentCases = [this]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData ) { return !formatData.blockExtent.empty(); },
      []( FormatData const & formatData ) { return "return {{ " + formatData.blockExtent + " }};"; } );
  };
  auto generateBlockSizeCases = [&]( auto const formatIt )
  {
    return generateFormatTraitsCases( formatIt->second, noPredicate, []( FormatData const & formatData ) { return "return " + formatData.blockSize + ";"; } );
  };
  auto generateBlueCases = [this]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData )
      { return std::ranges::any_of( formatData.components, []( auto const & component ) { return component.name == "B"; } ); },
      []( FormatData const & ) { return ""; } );
  };
  auto generateClassCases = [&]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second, noPredicate, []( FormatData const & formatData ) { return "return \"" + formatData.classAttribute + "\";"; } );
  };
  auto generateColorFormatsList = [&]( auto const formatIt )
  {
    return generateFormatTraitsList( formatIt->second,
                                     []( FormatData const & formatData )
                                     {
                                       return std::ranges::any_of( formatData.components,
                                                                   []( auto const & component )
                                                                   {
                                                                     return ( component.name == "R" ) || ( component.name == "G" ) ||
                                                                            ( component.name == "B" ) || ( component.name == "A" );
                                                                   } );
                                     } );
  };
  auto generateComponentBitsCases = [&]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData ) { return formatData.components.front().bits != "compressed"; },
      [&]( FormatData const & formatData )
      { return generateComponentCases( formatData, []( ComponentData const & componentData ) { return componentData.bits; }, "0" ); } );
  };
  auto generateComponentCountCases = [&]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second, noPredicate, []( FormatData const & formatData ) { return "return " + std::to_string( formatData.components.size() ) + ";"; } );
  };
  auto generateComponentNameCases = [&]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      noPredicate,
      [&]( FormatData const & formatData )
      { return generateComponentCases( formatData, []( ComponentData const & componentData ) { return "\"" + componentData.name + "\""; }, "\"\"" ); } );
  };
  auto generateComponentNumericFormatCases = [&]( auto const formatIt )
  {
    return generateFormatTraitsCases( formatIt->second,
                                      noPredicate,
                                      [&]( FormatData const & formatData )
                                      {
                                        return generateComponentCases(
                                          formatData, []( ComponentData const & componentData ) { return "\"" + componentData.numericFormat + "\""; }, "\"\"" );
                                      } );
  };
  auto generateComponentPlaneIndexCases = [&]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData ) { return !formatData.components.front().planeIndex.empty(); },
      [&]( FormatData const & formatData )
      { return generateComponentCases( formatData, []( ComponentData const & componentData ) { return componentData.planeIndex; }, "0" ); } );
  };
  auto generateComponentsAreCompressedCases = [this]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData ) { return formatData.components.front().bits == "compressed"; },
      []( FormatData const & ) { return ""; } );
  };
  auto generateCompressionSchemeCases = [this]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData ) { return !formatData.compressed.empty(); },
      []( FormatData const & formatData ) { return " return \"" + formatData.compressed + "\";"; } );
  };
  auto generateDepthCases = [this]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData )
      { return std::ranges::any_of( formatData.components, []( auto const & component ) { return component.name == "D"; } ); },
      []( FormatData const & ) { return ""; } );
  };
  auto generateDepthFormatsList = [&]( auto const formatIt )
  {
    return generateFormatTraitsList( formatIt->second,
                                     []( FormatData const & formatData )
                                     { return std::ranges::any_of( formatData.components, []( auto const & component ) { return component.name == "D"; } ); } );
  };
  auto generateDepthStencilFormatsList = [&]( auto const formatIt )
  {
    return generateFormatTraitsList( formatIt->second,
                                     []( FormatData const & formatData )
                                     {
                                       return std::ranges::any_of( formatData.components, []( auto const & component ) { return component.name == "D"; } ) &&
                                              std::ranges::any_of( formatData.components, []( auto const & component ) { return component.name == "S"; } );
                                     } );
  };
  auto generateGreenCases = [this]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData )
      { return std::ranges::any_of( formatData.components, []( auto const & component ) { return component.name == "G"; } ); },
      []( FormatData const & ) { return ""; } );
  };
  auto generatePackedCases = [this]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData ) { return !formatData.packed.empty(); },
      []( FormatData const & formatData ) { return " return " + formatData.packed + ";"; } );
  };
  auto generatePlaneCompatibleCases = [&]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData ) { return !formatData.planes.empty(); },
      [&]( FormatData const & formatData )
      {
        return generatePlaneCases(
          formatData,
          [&]( PlaneData const & planeData ) { return "Format::" + generateEnumValueName( "VkFormat", planeData.compatible, false ); },
          "Format::eUndefined" );
      } );
  };
  auto generatePlaneCountCases = [this]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData ) { return !formatData.planes.empty(); },
      []( FormatData const & formatData ) { return " return " + std::to_string( formatData.planes.size() ) + ";"; } );
  };
  auto generatePlaneHeightDivisorCases = [&]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData ) { return !formatData.planes.empty(); },
      [&]( FormatData const & formatData )
      { return generatePlaneCases( formatData, [&]( PlaneData const & planeData ) { return planeData.heightDivisor; }, "1" ); } );
  };
  auto generatePlaneWidthDivisorCases = [&]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData ) { return !formatData.planes.empty(); },
      [&]( FormatData const & formatData )
      { return generatePlaneCases( formatData, [&]( PlaneData const & planeData ) { return planeData.widthDivisor; }, "1" ); } );
  };
  auto generateRedCases = [this]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData )
      { return std::ranges::any_of( formatData.components, []( auto const & component ) { return component.name == "R"; } ); },
      []( FormatData const & ) { return ""; } );
  };
  auto generateStencilCases = [this]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second,
      []( FormatData const & formatData )
      { return std::ranges::any_of( formatData.components, []( auto const & component ) { return component.name == "S"; } ); },
      []( FormatData const & ) { return ""; } );
  };
  auto generateStencilFormatsList = [&]( auto const formatIt )
  {
    return generateFormatTraitsList( formatIt->second,
                                     []( FormatData const & formatData )
                                     { return std::ranges::any_of( formatData.components, []( auto const & component ) { return component.name == "S"; } ); } );
  };
  auto generateTexelsPerBlockCases = [&]( auto const formatIt )
  {
    return generateFormatTraitsCases(
      formatIt->second, noPredicate, []( FormatData const & formatData ) { return "return " + formatData.texelsPerBlock + ";"; } );
  };

  return replaceWithMap( readSnippet( "FormatTraits.hpp" ),
                         { { "allFormats", generateAllFormatsList( formatIt ) },
                           { "alphaCases", generateAlphaCases( formatIt ) },
                           { "blockExtentCases", generateBlockExtentCases( formatIt ) },
                           { "blockSizeCases", generateBlockSizeCases( formatIt ) },
                           { "blueCases", generateBlueCases( formatIt ) },
                           { "classCases", generateClassCases( formatIt ) },
                           { "colorFormats", generateColorFormatsList( formatIt ) },
                           { "componentBitsCases", generateComponentBitsCases( formatIt ) },
                           { "componentCountCases", generateComponentCountCases( formatIt ) },
                           { "componentNameCases", generateComponentNameCases( formatIt ) },
                           { "componentNumericFormatCases", generateComponentNumericFormatCases( formatIt ) },
                           { "componentPlaneIndexCases", generateComponentPlaneIndexCases( formatIt ) },
                           { "componentsAreCompressedCases", generateComponentsAreCompressedCases( formatIt ) },
                           { "compressionSchemeCases", generateCompressionSchemeCases( formatIt ) },
                           { "depthCases", generateDepthCases( formatIt ) },
                           { "depthFormats", generateDepthFormatsList( formatIt ) },
                           { "depthStencilFormats", generateDepthStencilFormatsList( formatIt ) },
                           { "greenCases", generateGreenCases( formatIt ) },
                           { "packedCases", generatePackedCases( formatIt ) },
                           { "planeCompatibleCases", generatePlaneCompatibleCases( formatIt ) },
                           { "planeCountCases", generatePlaneCountCases( formatIt ) },
                           { "planeHeightDivisorCases", generatePlaneHeightDivisorCases( formatIt ) },
                           { "planeWidthDivisorCases", generatePlaneWidthDivisorCases( formatIt ) },
                           { "redCases", generateRedCases( formatIt ) },
                           { "stencilCases", generateStencilCases( formatIt ) },
                           { "stencilFormats", generateStencilFormatsList( formatIt ) },
                           { "texelsPerBlockCases", generateTexelsPerBlockCases( formatIt ) } } );
}

std::string VulkanHppGenerator::generateFormatTraitsCases( EnumData const &                                 enumData,
                                                           std::function<bool( FormatData const & )>        predicate,
                                                           std::function<std::string( FormatData const & )> generator ) const
{
  std::string cases;
  for ( auto enumValueIt = std::next( enumData.values.begin() ); enumValueIt != enumData.values.end(); ++enumValueIt )
  {
    if ( enumValueIt->supported )
    {
      auto formatIt = m_formats.find( enumValueIt->name );
      assert( formatIt != m_formats.end() );

      if ( predicate( formatIt->second ) )
      {
        cases += "      case Format::" + generateEnumValueName( "VkFormat", formatIt->first, false ) + ": " + generator( formatIt->second ) + "\n";
      }
    }
  }
  return cases;
}

std::string VulkanHppGenerator::generateFormatTraitsList( EnumData const & enumData, std::function<bool( FormatData const & )> predicate ) const
{
  std::string list;
  for ( auto enumValueIt = std::next( enumData.values.begin() ); enumValueIt != enumData.values.end(); ++enumValueIt )
  {
    if ( enumValueIt->supported )
    {
      auto formatIt = m_formats.find( enumValueIt->name );
      assert( formatIt != m_formats.end() );

      if ( predicate( formatIt->second ) )
      {
        if ( !list.empty() )
        {
          list += ", ";
        }
        list += "Format::" + generateEnumValueName( "VkFormat", formatIt->first, false );
      }
    }
  }
  return list;
}

std::string VulkanHppGenerator::generateFuncPointer( TypeFuncPointer const & funcPointer ) const
{
  auto const [enter, leave] = generateProtection( getProtectFromType( funcPointer.name ) );

  std::string funcPointerParams;
  if ( !funcPointer.params.empty() )
  {
    for ( auto const & param : funcPointer.params )
    {
      if ( !funcPointerParams.empty() )
      {
        funcPointerParams += ", ";
      }
      funcPointerParams += param.type.compose( "Vk" ) + " " + param.name;
    }
  }

  static std::string const funcPointerTemplate = R"(
  typedef ${returnType} (VKAPI_PTR *PFN_${funcPointerName})( ${funcPointerParams} );
)";

  return "\n" + enter +
         replaceWithMap( funcPointerTemplate,
                         { { "funcPointerParams", funcPointerParams },
                           { "funcPointerName", stripPrefix( funcPointer.name, "PFN_vk" ) },
                           { "returnType", funcPointer.returnType.compose( "Vk" ) } } ) +
         leave;
}

std::string VulkanHppGenerator::generateFuncPointers() const
{
  auto funcPointerGenerator = [this]<typename RequireType>( std::vector<RequireType> const & requireTypes, std::string const & title )
  {
    std::string str;
    for ( auto const & require : requireTypes )
    {
      for ( auto const & type : require.types )
      {
        if ( auto funcPointerIt = findByName( m_vkxml.types.funcPointers, type.name ); funcPointerIt != m_vkxml.types.funcPointers.end() )
        {
          str += generateFuncPointer( *funcPointerIt );
        }
      }
    }
    return addTitleAndProtection( title, str );
  };

  std::string funcPointers = R"(
  //=====================
  //=== FUNC_POINTERS ===
  //=====================

)";

  for ( auto const & feature : m_vkxml.features )
  {
    funcPointers += funcPointerGenerator( feature.require, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    funcPointers += funcPointerGenerator( extension.requireData, extension.name );
  }
  return funcPointers;
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

    auto vkHandleIt = findByName( m_vkxml.types.handles, handleData.first );
    assert( vkHandleIt != m_vkxml.types.handles.end() );
    std::string debugReportObjectType = generateDebugReportObjectType( vkHandleIt->objTypeEnum );

    // create CPPType template specialization
    std::string className = stripPrefix( handleData.first, "Vk" );
    std::string cppType;
    if ( debugReportObjectType != "eUnknown" )
    {
      static std::string const cppTypeFromDebugReportObjectTypeEXTTemplate = R"(
  template <>
  struct CppType<DebugReportObjectTypeEXT, DebugReportObjectTypeEXT::e${className}>
  {
    using Type = ${className};
  };
)";
      cppType = replaceWithMap( cppTypeFromDebugReportObjectTypeEXTTemplate, { { "className", className } } );
    }

    auto [enter, leave] = generateProtection( getProtectFromType( handleData.first ) );

    assert( !vkHandleIt->objTypeEnum.empty() );
    auto enumIt = m_enums.find( "VkObjectType" );
    assert( enumIt != m_enums.end() );
    assert( containsByNameOrAlias( enumIt->second.values, vkHandleIt->objTypeEnum ) );

    std::string usingAlias;
    for ( auto const & alias : vkHandleIt->aliases )
    {
      usingAlias += "  using " + stripPrefix( alias.first, "Vk" ) + " = " + stripPrefix( handleData.first, "Vk" ) + ";\n";
    }

    std::string const typesafeExplicitKeyword          = handleData.second.isDispatchable ? "" : "VULKAN_HPP_TYPESAFE_EXPLICIT ";
    std::string const typesafeConversionConditional    = handleData.second.isDispatchable ? "" : "#if ( VULKAN_HPP_TYPESAFE_CONVERSION == 1 )\n";
    std::string const typesafeConversionConditionalEnd = handleData.second.isDispatchable ? "" : "#endif\n";

    static std::string const templateString = R"(
  // wrapper class for handle Vk${className}, see https://registry.khronos.org/vulkan/specs/latest/man/html/Vk${className}.html
${enter}  class ${className}
  {
  public:
    using CType = Vk${className};
    using NativeType = Vk${className};

    static VULKAN_HPP_CONST_OR_CONSTEXPR ObjectType objectType = ObjectType::${objTypeEnum};
    static VULKAN_HPP_CONST_OR_CONSTEXPR DebugReportObjectTypeEXT debugReportObjectType = DebugReportObjectTypeEXT::${debugReportObjectType};

  public:
    ${className}() VULKAN_HPP_NOEXCEPT {}    // = default; - try to workaround a compiler issue
    ${className}( ${className} const & rhs ) = default;
    ${className} & operator=( ${className} const & rhs ) = default;

#if !defined(VULKAN_HPP_HANDLES_MOVE_EXCHANGE)
    ${className}( ${className} && rhs ) = default;
    ${className} & operator=( ${className} && rhs ) = default;
#else
    ${className}( ${className} && rhs ) VULKAN_HPP_NOEXCEPT
      : m_${memberName}( exchange( rhs.m_${memberName}, {} ) )
    {}
    ${className} & operator=( ${className} && rhs ) VULKAN_HPP_NOEXCEPT
    {
      m_${memberName} = exchange( rhs.m_${memberName}, {} );
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
  struct CppType<ObjectType, ObjectType::${objTypeEnum}>
  {
    using Type = ${className};
  };

${CppType}

#if ( VK_USE_64_BIT_PTR_DEFINES == 1 )
  template <>
  struct CppType<Vk${className}, VK_NULL_HANDLE>
  {
    using Type = ${className};
  };
#endif

  template <>
  struct isVulkanHandleType<${className}>
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
                             { "objTypeEnum", generateEnumValueName( enumIt->first, vkHandleIt->objTypeEnum, false ) },
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
  for ( auto const & feature : m_vkxml.features )
  {
    std::vector<std::string> commandNames = selectCommandsByHandle( feature.require, commands, listedCommands );
    if ( !commandNames.empty() )
    {
      str += "\n  //=== " + feature.name + " ===\n";
      for ( auto const & name : commandNames )
      {
        auto commandIt = findByName( m_vkxml.commands, name );
        assert( commandIt != m_vkxml.commands.end() );

        str += "\n";
        str += generateCommand( name, *commandIt, 1, false, false );
        str += generateDestroyCommand( name, *commandIt );
      }
    }
  }
  for ( auto const & extension : m_extensions )
  {
    std::vector<std::string> commandNames = selectCommandsByHandle( extension.requireData, commands, listedCommands );
    if ( !commandNames.empty() )
    {
      auto const [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
      str += "\n" + enter + "  //=== " + extension.name + " ===\n";
      for ( auto const & name : commandNames )
      {
        auto commandIt = findByNameOrAlias( m_vkxml.commands, name );
        assert( commandIt != m_vkxml.commands.end() );

        std::string commandString;
        std::string commandName = generateCommandName( name, commandIt->params, 1 );
        str += "\n";
        str += generateCommand( name, *commandIt, 1, false, false );
        str += generateDestroyCommand( name, *commandIt );
      }
      str += leave;
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateHandleDependencies( std::pair<std::string, HandleData> const & handleData, std::set<std::string> & listedHandles ) const
{
  std::string str;
  for ( auto const & name : handleData.second.commands )
  {
    auto commandIt = findByNameOrAlias( m_vkxml.commands, name );
    assert( commandIt != m_vkxml.commands.end() );
    for ( auto const & parameter : commandIt->params )
    {
      if ( parameter.type.isValue() && ( parameter.type.name != handleData.first ) )
      {
        auto handleIt = m_handles.find( parameter.type.name );
        if ( ( handleIt != m_handles.end() ) && !listedHandles.contains( parameter.type.name ) )
        {
          str += generateHandle( *handleIt, listedHandles );
        }
      }
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateHandleEmpty( HandleData const & handleData ) const
{
  std::string           str;
  std::set<std::string> listedCommands;  // some commands are listed with more than one extension !
  for ( auto const & feature : m_vkxml.features )
  {
    std::vector<std::string> commands = selectCommandsByHandle( feature.require, handleData.commands, listedCommands );
    if ( !commands.empty() )
    {
      str += "\n  //=== "s + feature.name + " ===\n";
      for ( auto const & name : commands )
      {
        auto commandIt = findByName( m_vkxml.commands, name );
        assert( commandIt != m_vkxml.commands.end() );
        str += "\n"s + generateCommand( name, *commandIt, 0, false, false );
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
  std::string const fowardDeclarationsTemplate = R"(
  //===================================
  //=== HANDLE forward declarations ===
  //===================================

${forwardDeclarations}
)";

  std::string forwardDeclarations;
  for ( auto const & feature : m_vkxml.features )
  {
    forwardDeclarations += generateHandleForwardDeclarations( feature.require, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    forwardDeclarations += generateHandleForwardDeclarations( extension.requireData, extension.name );
  }

  return replaceWithMap( fowardDeclarationsTemplate, { { "forwardDeclarations", forwardDeclarations } } );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateHandleForwardDeclarations( std::vector<RequireType> const & requireTypes, std::string const & title ) const
{
  std::string str;
  forEachRequiredHandle( requireTypes,
                         [&str]( std::pair<std::string, HandleData> const & handleData ) { str += "class " + stripPrefix( handleData.first, "Vk" ) + ";\n"; } );
  return addTitleAndProtection( title, str );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateHandleHashStructures( std::vector<RequireType> const & requireTypes,
                                                              std::string const &              title,
                                                              std::set<std::string> &          listedHandles ) const
{
  std::string const hashTemplate = R"(
  template <> struct hash<VULKAN_HPP_NAMESPACE::${type}>
  {
    std::size_t operator()(VULKAN_HPP_NAMESPACE::${type} const & ${name}) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<Vk${type}>{}(static_cast<Vk${type}>(${name}));
    }
  };
)";

  std::string str;
  forEachRequiredHandle( requireTypes,
                         [&str, &listedHandles, &hashTemplate]( std::pair<std::string, HandleData> const & handleData )
                         {
                           if ( listedHandles.insert( handleData.first ).second )
                           {
                             std::string handleType = stripPrefix( handleData.first, "Vk" );
                             std::string handleName = startLowerCase( handleType );
                             str += replaceWithMap( hashTemplate, { { "name", handleName }, { "type", handleType } } );
                           }
                         } );
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateHandleHashStructures() const
{
  std::string const hashesTemplate = R"(
  //===================================
  //=== HASH structures for handles ===
  //===================================

${hashes}
)";

  std::string           hashes;
  std::set<std::string> listedHandles;  // some handles are listed with more than one extension !
  for ( auto const & feature : m_vkxml.features )
  {
    hashes += generateHandleHashStructures( feature.require, feature.name, listedHandles );
  }
  for ( auto const & extension : m_extensions )
  {
    hashes += generateHandleHashStructures( extension.requireData, extension.name, listedHandles );
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

#if 20 <= VULKAN_HPP_CPP_VERSION
  template <typename T>
  concept VulkanHandleType = isVulkanHandleType<T>::value;
#endif
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

      std::string const typeTraitTemplate = R"(  template <>
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

  std::string const typeTraitsTemplate = R"(
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

std::string VulkanHppGenerator::generateIsDispatchedList() const
{
  std::string const isDispatchedListTemplate = R"(
  //==========================
  //=== Is Dispatched List ===
  //==========================

  // C++11-compatible void_t
  template <typename...>
  struct voider { typedef void type; };
  template <typename... Ts>
  using void_t = typename voider<Ts...>::type;

  // helper macro to declare a SFINAE-friendly has_<fn> trait
# define DECLARE_IS_DISPATCHED( name )                                              \
  template <typename D, typename = void>                                             \
  struct has_##name : std::false_type                                                \
  {                                                                                  \
  };                                                                                 \
  template <typename D>                                                               \
  struct has_##name<D, void_t<decltype( &D::name )>> : std::true_type                \
  {                                                                                  \
  };

  ${isDispatchedList}

# undef DECLARE_IS_DISPATCHED

# define IS_DISPATCHED( name ) ::VULKAN_HPP_NAMESPACE::detail::has_##name<Dispatch>::value
)";

  std::string isDispatchedList;
  for ( auto const & command : m_vkxml.commands )
  {
    if ( isApiSupported( command.api, m_api ) )
    {
      isDispatchedList += "DECLARE_IS_DISPATCHED( " + command.name + " )\n";
      for ( auto const & alias : command.aliases )
      {
        isDispatchedList += "DECLARE_IS_DISPATCHED( " + alias.first + " )\n";
      }
    }
  }

  return replaceWithMap( isDispatchedListTemplate, { { "isDispatchedList", isDispatchedList } } );
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

  std::string const typeTraits = R"(
  //=================================
  //=== Layer Setting Type Traits ===
  //=================================

  template <>
  struct CppType<LayerSettingTypeEXT, LayerSettingTypeEXT::eBool32>
  {
    using Type = Bool32;
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
      case LayerSettingTypeEXT::eBool32: return std::is_same<T, Bool32>::value;
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
  std::vector<StructMember>::const_iterator                                                                                   mit,
  std::map<std::vector<StructMember>::const_iterator, std::vector<std::vector<StructMember>::const_iterator>>::const_iterator litit,
  bool                                                                                                                        mutualExclusiveLens ) const
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
    assert( ( arrayIt->len.front() == litit->first->name ) || ( ( arrayIt->altLen == "codeSize / 4" ) && ( litit->first->name == "codeSize" ) ) );

    assert( arrayIt->type.isPointer() || !arrayIt->arraySizes.empty() );
    std::string argumentName = ( arrayIt->type.isPointer() ? startLowerCase( stripPrefix( arrayIt->name, "p" ) ) : arrayIt->name ) + "_";

    assert( mit->type.prefix.empty() && mit->type.postfix.empty() );
    initializer = argumentName + ".size()";
    if ( arrayIt->altLen == "codeSize / 4" )
    {
      initializer += " * 4";
    }
    if ( ( arrayIt->type.name == "void" ) &&
         ( std::count_if( arrayIt->type.postfix.begin(), arrayIt->type.postfix.end(), []( char c ) { return c == '*'; } ) < 2 ) )
    {
      initializer += " * sizeof(T)";
    }
  }
  if ( mit->type.name != "size_t" )
  {
    initializer = "static_cast<" + mit->type.name + ">( " + initializer + " )";
  }
  if ( !litit->second.front()->arraySizes.empty() )
  {
    assert( litit->second.front()->arraySizes.size() == 1 );
    initializer = "(std::min)( " + initializer + ", " + litit->second.front()->arraySizes[0] + " )";
  }
  return initializer;
}

std::string VulkanHppGenerator::generateName( Type const & type ) const
{
  std::string name = stripPrefix( type.name, "Vk" );
  assert( type.prefix.find( '*' ) == std::string::npos );
  if ( type.postfix.find( '*' ) != std::string::npos )
  {
    assert( type.postfix.find_first_of( '*' ) == type.postfix.find_last_of( '*' ) );
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
  return ( errorCodes.empty() && ( ( flavourFlags & CommandFlavourFlagBits::singular ) || returnParams.empty() ||
                                   std::ranges::none_of( returnParams, [&vectorParams]( size_t rp ) { return vectorParams.contains( rp ); } ) ) )
         ? ( vectorSizeCheck ? ( raii ? "" : " VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS" ) : " VULKAN_HPP_NOEXCEPT" )
         : "";
}

std::string VulkanHppGenerator::generateObjectDeleter( Command const & command, size_t initialSkipCount, size_t returnParam ) const
{
  std::string objectDeleter, allocator;
  if ( ( command.name.find( "Acquire" ) != std::string::npos ) || ( command.name.find( "Get" ) != std::string::npos ) )
  {
    if ( ( command.name == "vkAcquirePerformanceConfigurationINTEL" ) || ( command.name == "vkGetRandROutputDisplayEXT" ) ||
         ( command.name == "vkGetWinrtDisplayNV" ) || ( command.name == "vkGetDrmDisplayEXT" ) )
    {
      objectDeleter = "detail::ObjectRelease";
    }
    else
    {
      throw std::runtime_error( "Found " + command.name + " which requires special handling for the object deleter" );
    }
  }
  else
  {
    auto handleIt = m_handles.find( command.params[returnParam].type.name );
    assert( handleIt != m_handles.end() );

    if ( handleIt->second.deleteCommand.empty() )
    {
      objectDeleter = "detail::DummyDestroy";
    }
    else if ( command.name.find( "Allocate" ) != std::string::npos )
    {
      objectDeleter = "detail::ObjectFree";
    }
    else
    {
      assert( ( command.name.find( "Create" ) != std::string::npos ) || ( command.name.find( "Register" ) != std::string::npos ) );
      objectDeleter = "detail::ObjectDestroy";
    }
    allocator = "allocator, ";
  }

  std::string className  = initialSkipCount ? stripPrefix( command.params[initialSkipCount - 1].type.name, "Vk" ) : "";
  std::string parentName = ( className.empty() || ( command.params[returnParam].type.name == "VkDevice" ) ) ? "detail::NoParent" : className;
  return objectDeleter + "<" + parentName + ", Dispatch>( " + ( ( parentName == "detail::NoParent" ) ? "" : "*this, " ) + allocator + "d )";
}

std::string VulkanHppGenerator::generateObjectTypeToDebugReportObjectType() const
{
  auto objectTypeToDebugReportObjectTypeTemplate = std::string{ R"(
  //===========================================================
  //=== Mapping from ObjectType to DebugReportObjectTypeEXT ===
  //===========================================================

  VULKAN_HPP_INLINE DebugReportObjectTypeEXT debugReportObjectType( ObjectType objectType )
  {
    switch( objectType )
    {
${objectTypeCases}
      default:
        VULKAN_HPP_ASSERT( false && "unknown ObjectType" );
        return DebugReportObjectTypeEXT::eUnknown;
    }
  }
)" };

  auto const generateObjectTypeCases =
    [this]<typename RequireType>( std::vector<RequireType> const & requireTypes, std::string const & title, std::set<std::string> & listedTypes )
  {
    static std::string const objectTypeCaseTemplate = "      case ObjectType::${objectType} : return DebugReportObjectTypeEXT::${debugReportObjectType};\n";

    std::string objectTypeCases;
    forEachRequiredHandle( requireTypes,
                           [&objectTypeCases, &listedTypes, this]( std::pair<std::string, HandleData> const & handleData )
                           {
                             if ( listedTypes.insert( handleData.first ).second )
                             {
                               auto vkHandleIt = findByName( m_vkxml.types.handles, handleData.first );
                               assert( vkHandleIt != m_vkxml.types.handles.end() );
                               objectTypeCases +=
                                 replaceWithMap( objectTypeCaseTemplate,
                                                 { { "debugReportObjectType", generateDebugReportObjectType( vkHandleIt->objTypeEnum ) },
                                                   { "objectType", generateEnumValueName( "VkObjectType", vkHandleIt->objTypeEnum, false ) } } );
                             }
                           } );
    return addTitleAndProtection( title, objectTypeCases );
  };

  std::string           objectTypeCases;
  std::set<std::string> listedTypes;
  for ( auto const & feature : m_vkxml.features )
  {
    objectTypeCases += generateObjectTypeCases( feature.require, feature.name, listedTypes );
  }
  for ( auto const & extension : m_extensions )
  {
    objectTypeCases += generateObjectTypeCases( extension.requireData, extension.name, listedTypes );
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
  std::string const commandDefinitionsTemplate = R"(
  //===========================
  //=== COMMAND Definitions ===
  //===========================

${commandDefinitions}
)";

  std::string           commandDefinitions;
  std::set<std::string> listedCommands;  // some commands are listed with more than one extension!
  for ( auto const & feature : m_vkxml.features )
  {
    commandDefinitions += generateRAIICommandDefinitions( feature.require, listedCommands, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    commandDefinitions += generateRAIICommandDefinitions( extension.requireData, listedCommands, extension.name );
  }

  return replaceWithMap( commandDefinitionsTemplate, { { "commandDefinitions", commandDefinitions } } );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateRAIICommandDefinitions( std::vector<RequireType> const & requireTypes,
                                                                std::set<std::string> &          listedCommands,
                                                                std::string const &              title ) const
{
  std::string str;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & )
                          {
                            if ( listedCommands.insert( name ).second )
                            {
                              str += generateRAIIHandleCommand( name, determineInitialSkipCount( name ), true );
                            }
                          } );
  return addTitleAndProtection( title, str );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateRAIIDispatcherCommandsContextInitializers( std::vector<RequireType> const & requireTypes,
                                                                                   std::set<std::string> const &    listedCommands,
                                                                                   std::string const &              title ) const
{
  std::string contextInitializers;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & command )
                          {
                            if ( !listedCommands.contains( name ) && !isHandleType( command.params[0].type.name ) )
                            {
                              contextInitializers += ", " + name + "( PFN_" + name + "( getProcAddr( NULL, \"" + name + "\" ) ) )";
                            }
                          } );
  return addTitleAndProtection( title, contextInitializers );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateRAIIDispatcherCommandsContextMembers( std::vector<RequireType> const & requireTypes,
                                                                              std::set<std::string> const &    listedCommands,
                                                                              std::string const &              title ) const
{
  std::string contextMembers;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & command )
                          {
                            if ( !listedCommands.contains( name ) && !isHandleType( command.params[0].type.name ) )
                            {
                              contextMembers += "      PFN_" + name + " " + name + " = 0;\n";
                            }
                          } );
  return addTitleAndProtection( title, contextMembers );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateRAIIDispatcherCommandsDeviceAssignments( std::vector<RequireType> const & requireTypes,
                                                                                 std::set<std::string> const &    listedCommands,
                                                                                 std::string const &              title ) const
{
  std::string deviceAssignments;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & command )
                          {
                            if ( !listedCommands.contains( name ) && isHandleType( command.params[0].type.name ) &&
                                 ( ( command.params[0].type.name == "VkDevice" ) || hasParentHandle( command.params[0].type.name, "VkDevice" ) ) )
                            {
                              deviceAssignments += "        " + name + " = PFN_" + name + "( vkGetDeviceProcAddr( device, \"" + name + "\" ) );\n";
                              // if this is an alias'ed function, use it as a fallback for the original one
                              if ( name != command.name )
                              {
                                assert( command.aliases.contains( name ) );
                                deviceAssignments += "        if ( !" + command.name + " ) " + command.name + " = " + name + ";\n";
                              }
                            }
                          } );
  return addTitleAndProtection( title, deviceAssignments );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateRAIIDispatcherCommandsDeviceMembers( std::vector<RequireType> const & requireTypes,
                                                                             std::set<std::string> const &    listedCommands,
                                                                             std::string const &              title ) const
{
  std::string deviceMembers, deviceMemberDummies;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & command )
                          {
                            if ( !listedCommands.contains( name ) && isHandleType( command.params[0].type.name ) &&
                                 ( ( command.params[0].type.name == "VkDevice" ) || hasParentHandle( command.params[0].type.name, "VkDevice" ) ) )
                            {
                              deviceMembers += "      PFN_" + name + " " + name + " = 0;\n";
                              deviceMemberDummies += "      PFN_dummy " + name + "_placeholder = 0;\n";
                            }
                          } );
  return addTitleAndProtection( title, deviceMembers, deviceMemberDummies );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateRAIIDispatcherCommandsInstanceAssignments( std::vector<RequireType> const & requireTypes,
                                                                                   std::set<std::string> const &    listedCommands,
                                                                                   std::string const &              title ) const
{
  std::string instanceAssignments;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & command )
                          {
                            if ( !listedCommands.contains( name ) && isHandleType( command.params[0].type.name ) &&
                                 !( ( command.params[0].type.name == "VkDevice" ) || hasParentHandle( command.params[0].type.name, "VkDevice" ) ) )
                            {
                              assert( ( command.params[0].type.name == "VkInstance" ) || hasParentHandle( command.params[0].type.name, "VkInstance" ) );
                              // filter out vkGetInstanceProcAddr, as starting with Vulkan 1.2 it can resolve itself only (!) with an
                              // instance nullptr !
                              if ( name != "vkGetInstanceProcAddr" )
                              {
                                instanceAssignments += "        " + name + " = PFN_" + name + "( vkGetInstanceProcAddr( instance, \"" + name + "\" ) );\n";
                                // if this is an alias'ed function, use it as a fallback for the original one
                                if ( name != command.name )
                                {
                                  assert( command.aliases.contains( name ) );
                                  instanceAssignments += "        if ( !" + command.name + " ) " + command.name + " = " + name + ";\n";
                                }
                              }
                            }
                          } );
  return addTitleAndProtection( title, instanceAssignments );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateRAIIDispatcherCommandsInstanceMembers( std::vector<RequireType> const & requireTypes,
                                                                               std::set<std::string> const &    listedCommands,
                                                                               std::string const &              title ) const
{
  std::string instanceMembers, instanceMemberDummies;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & command )
                          {
                            if ( !listedCommands.contains( name ) && isHandleType( command.params[0].type.name ) &&
                                 !( ( command.params[0].type.name == "VkDevice" ) || hasParentHandle( command.params[0].type.name, "VkDevice" ) ) )
                            {
                              assert( ( command.params[0].type.name == "VkInstance" ) || hasParentHandle( command.params[0].type.name, "VkInstance" ) );

                              instanceMembers += +"      PFN_" + name + " " + name + " = 0;\n";
                              instanceMemberDummies += "      PFN_dummy " + name + "_placeholder = 0;\n";
                            }
                          } );
  return addTitleAndProtection( title, instanceMembers, instanceMemberDummies );
}

std::string VulkanHppGenerator::generateRAIIDispatchers() const
{
  std::string contextInitializers, contextMembers, deviceAssignments, deviceMembers, instanceAssignments, instanceMembers;

  std::set<std::string> listedCommands;
  for ( auto const & feature : m_vkxml.features )
  {
    contextInitializers += generateRAIIDispatcherCommandsContextInitializers( feature.require, listedCommands, feature.name );
    contextMembers += generateRAIIDispatcherCommandsContextMembers( feature.require, listedCommands, feature.name );
    deviceAssignments += generateRAIIDispatcherCommandsDeviceAssignments( feature.require, listedCommands, feature.name );
    deviceMembers += generateRAIIDispatcherCommandsDeviceMembers( feature.require, listedCommands, feature.name );
    instanceAssignments += generateRAIIDispatcherCommandsInstanceAssignments( feature.require, listedCommands, feature.name );
    instanceMembers += generateRAIIDispatcherCommandsInstanceMembers( feature.require, listedCommands, feature.name );
    forEachRequiredCommand( feature.require, [&listedCommands]( std::string const & name, auto const & ) { listedCommands.insert( name ); } );
  }
  for ( auto const & extension : m_extensions )
  {
    contextInitializers += generateRAIIDispatcherCommandsContextInitializers( extension.requireData, listedCommands, extension.name );
    contextMembers += generateRAIIDispatcherCommandsContextMembers( extension.requireData, listedCommands, extension.name );
    deviceAssignments += generateRAIIDispatcherCommandsDeviceAssignments( extension.requireData, listedCommands, extension.name );
    deviceMembers += generateRAIIDispatcherCommandsDeviceMembers( extension.requireData, listedCommands, extension.name );
    instanceAssignments += generateRAIIDispatcherCommandsInstanceAssignments( extension.requireData, listedCommands, extension.name );
    instanceMembers += generateRAIIDispatcherCommandsInstanceMembers( extension.requireData, listedCommands, extension.name );
    forEachRequiredCommand( extension.requireData, [&listedCommands]( std::string const & name, auto const & ) { listedCommands.insert( name ); } );
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

std::string VulkanHppGenerator::generateRAIIFactoryReturnStatements( Command const &     command,
                                                                     std::string const & vkType,
                                                                     bool                enumerating,
                                                                     std::string const & returnType,
                                                                     std::string const & returnVariable,
                                                                     bool                singular ) const
{
  auto vkHandleIt = findByName( m_vkxml.types.handles, vkType );
  assert( vkHandleIt != m_vkxml.types.handles.end() );
  auto handleIt = m_handles.find( vkHandleIt->name );
  assert( handleIt != m_handles.end() );

  std::string successCodePassToElement = ( enumerating ? ( command.successCodes.size() <= 2 ) : ( command.successCodes.size() <= 1 ) ) ? "" : ", result_";
  if ( returnType.starts_with( "std::pair" ) )
  {
    assert( returnVariable == "data_" );
    if ( singular )
    {
      assert( returnType.find( "std::vector" ) == std::string::npos );
      assert( returnType.substr( 10 ).starts_with( stripPrefix( vkType, "Vk" ) ) );

      std::string const & returnTemplate = R"(        ${returnType} data_RAII{ std::piecewise_construct, std::forward_as_tuple( nullptr ), {} };
        if ( result_ == Result::eSuccess )
        {
          data_RAII.first = ${handleType}( *this, ${handleConstructorArguments}${successCodePassToElement} );
          data_RAII.second = data_.second;
        }
        return VULKAN_HPP_NAMESPACE::detail::createResultValueType( result_, std::move( data_RAII ) );
)";

      std::string handleConstructorArguments = generateRAIIHandleSingularConstructorArguments( *handleIt, command.params, vkType, "data_.first" );

      return replaceWithMap( returnTemplate,
                             { { "handleConstructorArguments", handleConstructorArguments },
                               { "handleType", stripPrefix( vkType, "Vk" ) },
                               { "successCodePassToElement", successCodePassToElement },
                               { "returnType", returnType } } );
    }
    else
    {
      assert( ( returnType.starts_with( "std::pair<std::vector" ) ) && ( returnType.find( "std::vector", 22 ) != std::string::npos ) );
      assert( returnType.substr( 22 ).starts_with( stripPrefix( vkType, "Vk" ) ) );

      std::string const & returnTemplate = R"(        ${returnType} data_RAII;
        if ( result_ == Result::eSuccess )
        {
          data_RAII.first.reserve( data_.first.size() );
          for ( auto & element : data_.first )
          {
            data_RAII.first.emplace_back( *this, ${handleConstructorArguments}${successCodePassToElement} );
          }
          data_RAII.second = std::move( data_.second );
        }
        return VULKAN_HPP_NAMESPACE::detail::createResultValueType( result_, std::move( data_RAII ) );
)";

      std::string handleConstructorArguments = generateRAIIHandleSingularConstructorArguments( *handleIt, command.params, vkType, "element" );

      return replaceWithMap( returnTemplate,
                             { { "handleConstructorArguments", handleConstructorArguments },
                               { "successCodePassToElement", successCodePassToElement },
                               { "returnType", returnType } } );
    }
  }
  else if ( returnType.starts_with( "std::vector" ) )
  {
    assert( !command.successCodes.empty() );

    std::string const & returnTemplate = R"(        ${returnType} ${returnVariable}RAII;
        if ( result_ == Result::eSuccess )
        {
          ${returnVariable}RAII.reserve( ${returnVariable}.size() );
          for ( auto & ${element} : ${returnVariable} )
          {
            ${returnVariable}RAII.emplace_back( *this, ${handleConstructorArguments}${successCodePassToElement} );
          }
        }
        return VULKAN_HPP_NAMESPACE::detail::createResultValueType( result_, std::move( ${returnVariable}RAII ) );
)";

    std::string element                    = stripPluralS( returnVariable );
    std::string handleConstructorArguments = generateRAIIHandleSingularConstructorArguments( *handleIt, command.params, vkType, element );

    return replaceWithMap( returnTemplate,
                           { { "element", element },
                             { "handleConstructorArguments", handleConstructorArguments },
                             { "successCodePassToElement", successCodePassToElement },
                             { "returnType", returnType },
                             { "returnVariable", returnVariable } } );
  }
  else
  {
    std::string const & returnTypeTemplate = "${returnType}( *this, ${handleConstructorArguments}${successCodePassToElement} )";

    std::string handleConstructorArguments =
      generateRAIIHandleSingularConstructorArguments( *handleIt, command.params, vkType, singular ? stripPluralS( returnVariable ) : returnVariable );

    std::string constructorCall = replaceWithMap( returnTypeTemplate,
                                                  { { "returnType", returnType },
                                                    { "handleConstructorArguments", handleConstructorArguments },
                                                    { "successCodePassToElement", successCodePassToElement } } );

    if ( command.successCodes.empty() && command.errorCodes.empty() )
    {
      return "return " + constructorCall + ";";
    }
    else
    {
      return "return VULKAN_HPP_NAMESPACE::detail::createResultValueType( result_, " + constructorCall + " );";
    }
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
    std::string handleName = startLowerCase( stripPrefix( handle.first, "Vk" ) );

    auto [singularConstructors, arrayConstructors] = generateRAIIHandleConstructors( handle );

    auto [clearMembers,
          getConstructorSuccessCode,
          memberVariables,
          rawConstructorParams,
          rawConstructorInitializerList,
          moveConstructorInitializerList,
          moveAssignmentInstructions,
          swapMembers,
          releaseMembers] = generateRAIIHandleDetails( handle );

    std::string declarations = generateRAIIHandleCommandDeclarations( handle, specialFunctions );

    auto vkHandleIt = findByName( m_vkxml.types.handles, handle.first );
    assert( vkHandleIt != m_vkxml.types.handles.end() );
    auto enumIt = m_enums.find( "VkObjectType" );
    assert( enumIt != m_enums.end() );
    assert( containsByNameOrAlias( enumIt->second.values, vkHandleIt->objTypeEnum ) );
    std::string objTypeEnum = generateEnumValueName( enumIt->first, vkHandleIt->objTypeEnum, false );

    enumIt = m_enums.find( "VkDebugReportObjectTypeEXT" );
    assert( enumIt != m_enums.end() );
    std::string valueName = vkHandleIt->objTypeEnum;
    valueName             = valueName.replace( 3, 0, "DEBUG_REPORT_" ) + "_EXT";
    std::string debugReportObjectType =
      containsByNameOrAlias( enumIt->second.values, valueName ) ? generateEnumValueName( enumIt->first, valueName, false ) : "eUnknown";

    std::string dispatcherType =
      ( ( handle.first == "VkDevice" ) || ( m_vkxml.commands[handle.second.constructorIndices.front()].params.front().type.name == "VkDevice" ) )
        ? "detail::DeviceDispatcher"
        : "detail::InstanceDispatcher";

    std::string getParent;
    if ( ( handle.first != "VkInstance" ) && ( handle.first != "VkDevice" ) && ( handle.second.destructorIndex != INVALID_INDEX ) )
    {
      auto const & destructor = m_vkxml.commands[handle.second.destructorIndex];
      assert( !destructor.params.empty() );
      std::string parentType = stripPrefix( destructor.params.front().type.name, "Vk" );
      getParent              = "    VULKAN_HPP_NAMESPACE::" + parentType + " get" + parentType + "() const\n";
      getParent += "    {\n";
      getParent += "      return m_" + destructor.params.front().name + ";\n";
      getParent += "    }\n";
    }

    std::string assignmentOperator, copyConstructor;
    if ( handle.second.destructorIndex == INVALID_INDEX )
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

    std::string rawConstructor;
    if ( ( handle.first == "VkBuffer" ) || ( handle.first == "VkImage" ) )
    {
      std::string const rawConstructorTemplate = R"(protected:
      ${handleType}( ${rawConstructorParams} ) VULKAN_HPP_NOEXCEPT
        : ${rawConstructorInitializerList}
      {})";
      rawConstructor += replaceWithMap( rawConstructorTemplate,
                                        { { "handleType", handleType },
                                          { "rawConstructorParams", rawConstructorParams },
                                          { "rawConstructorInitializerList", rawConstructorInitializerList } } );
    }

    std::string const handleTemplate = R"(
  // wrapper class for handle Vk${handleType}, see https://registry.khronos.org/vulkan/specs/latest/man/html/Vk${handleType}.html
${enter}  class ${handleType}
  {
  public:
    using CType = Vk${handleType};
    using CppType = VULKAN_HPP_NAMESPACE::${handleType};

    static VULKAN_HPP_CONST_OR_CONSTEXPR ObjectType objectType = ObjectType::${objTypeEnum};
    static VULKAN_HPP_CONST_OR_CONSTEXPR DebugReportObjectTypeEXT debugReportObjectType = DebugReportObjectTypeEXT::${debugReportObjectType};

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

    VULKAN_HPP_NAMESPACE::${handleType} const & operator*() const & VULKAN_HPP_NOEXCEPT
    {
      return m_${handleName};
    }

    VULKAN_HPP_NAMESPACE::${handleType} const && operator*() const && VULKAN_HPP_NOEXCEPT
    {
      return std::move(m_${handleName});
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

    void swap( ${handleType} & rhs ) VULKAN_HPP_NOEXCEPT
    {
      ${swapMembers}
    }

    ${memberFunctionsDeclarations}

    ${rawConstructor}

  private:
    ${memberVariables}
  };

  template <>
  struct isVulkanRAIIHandleType<${handleType}>
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
                             { "rawConstructor", rawConstructor },
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
      std::string const handlesTemplate = R"(
${enter}  class ${handleType}s : public std::vector<${handleType}>
  {
  public:
    ${arrayConstructors}
    ${handleType}s( std::nullptr_t ) {}

    ${handleType}s() = delete;
    ${handleType}s( ${handleType}s const & ) = delete;
    ${handleType}s( ${handleType}s && rhs ) = default;
    ${handleType}s & operator=( ${handleType}s const & ) = delete;
    ${handleType}s & operator=( ${handleType}s && rhs ) = default;

    ${handleType}s( std::vector<${handleType}> && rhs )
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

std::string VulkanHppGenerator::generateRAIIHandleCommand( std::string const & name, size_t initialSkipCount, bool definition ) const
{
  std::string str;
  if ( !m_RAIISpecialFunctions.contains( name ) )
  {
    auto commandIt = findByNameOrAlias( m_vkxml.commands, name );
    assert( commandIt != m_vkxml.commands.end() );
    str = generateCommand( name, *commandIt, initialSkipCount, definition, true );
  }
  return str;
}

std::string VulkanHppGenerator::generateRAIIHandleCommandDeclarations( std::pair<std::string, HandleData> const & handle,
                                                                       std::set<std::string> const &              specialFunctions ) const
{
  std::string                     functionDeclarations;
  std::map<std::string, NameLine> listedCommands;  // some commands are listed with more than one extension !
  for ( auto const & feature : m_vkxml.features )
  {
    std::vector<std::string> firstLevelCommands, secondLevelCommands;

    forEachRequiredCommand( feature.require,
                            [&]( std::string const & name, auto const & command )
                            {
                              if ( !specialFunctions.contains( name ) )
                              {
                                if ( handle.second.commands.contains( name ) )
                                {
                                  assert( !listedCommands.contains( name ) );
                                  listedCommands.insert( { name, { feature.name, command.xmlLine } } );
                                  firstLevelCommands.push_back( name );
                                }
                                else if ( handle.second.secondLevelCommands.contains( name ) )
                                {
                                  auto listedIt = listedCommands.find( name );
                                  if ( listedIt == listedCommands.end() )
                                  {
                                    listedCommands.insert( { name, { feature.name, command.xmlLine } } );
                                    assert( !handle.first.empty() );
                                    secondLevelCommands.push_back( command.name );
                                  }
                                  else
                                  {
                                    checkForError( listedIt->second.name == feature.name,
                                                   command.xmlLine,
                                                   "command <" + command.name + "> already listed as required for feature <" + listedIt->second.name +
                                                     "> on line " + std::to_string( listedIt->second.xmlLine ) );
                                  }
                                }
                              }
                            } );
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
                                                                   Command const &                           command,
                                                                   size_t                                    initialSkipCount,
                                                                   std::vector<size_t> const &               returnParams,
                                                                   std::map<size_t, VectorParamData> const & vectorParams,
                                                                   bool                                      definition,
                                                                   CommandFlavourFlags                       flavourFlags ) const
{
  bool const singular = flavourFlags & CommandFlavourFlagBits::singular;

  std::set<size_t> skippedParams   = determineSkippedParams( command.params, initialSkipCount, vectorParams, returnParams, singular );
  std::set<size_t> singularParams  = singular ? determineSingularParams( returnParams[0], vectorParams ) : std::set<size_t>();
  std::set<size_t> templatedParams = ( flavourFlags & CommandFlavourFlagBits::keepVoidPtr ) ? std::set<size_t>() : determineVoidPointerParams( command.params );
  std::vector<size_t> chainedReturnParams =
    ( flavourFlags & CommandFlavourFlagBits::chained ) ? determineChainedReturnParams( command.params, returnParams ) : std::vector<size_t>();
  assert( chainedReturnParams.size() <= 1 );

  bool const               enumerating    = determineEnumeration( vectorParams, returnParams );
  std::vector<std::string> dataTypes      = determineDataTypes( command.params, vectorParams, returnParams, templatedParams, true );
  std::string              returnDataType = generateReturnDataType( vectorParams, returnParams, enumerating, dataTypes, flavourFlags, true );

  std::string argumentTemplates = generateArgumentTemplates( command.params, returnParams, vectorParams, templatedParams, chainedReturnParams, true );
  std::string argumentList      = generateArgumentListEnhanced(
    command.params, returnParams, vectorParams, skippedParams, singularParams, templatedParams, chainedReturnParams, true, definition, flavourFlags, false );
  std::string commandName = generateCommandName( name, command.params, initialSkipCount, flavourFlags );
  std::string nodiscard   = generateNoDiscard(
    !returnParams.empty() || ( ( command.returnType.name != "VkResult" ) && ( command.returnType.name != "void" ) ), 1 < command.successCodes.size(), false );
  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck =
    needsVectorSizeCheck( command.params, vectorParams, returnParams, singularParams, skippedParams );
  std::string noexceptString      = generateNoExcept( command.errorCodes, returnParams, vectorParams, flavourFlags, vectorSizeCheck.first, true );
  std::string returnType          = generateReturnType( command.params, returnParams, vectorParams, flavourFlags, true, dataTypes );
  std::string decoratedReturnType = generateDecoratedReturnType( command, returnParams, enumerating, flavourFlags, returnType );

  if ( definition )
  {
    std::string const definitionTemplate =
      R"(
  // wrapper function for command ${vkCommandName}, see https://registry.khronos.org/vulkan/specs/latest/man/html/${vkCommandName}.html
  ${argumentTemplates} ${nodiscard} VULKAN_HPP_INLINE ${returnType} ${className}::${commandName}( ${argumentList} ) const ${noexcept}
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
      name, command, returnParams, vectorParams, initialSkipCount, singularParams, templatedParams, chainedReturnParams, flavourFlags, true, false );
    std::string className        = initialSkipCount ? stripPrefix( command.params[initialSkipCount - 1].type.name, "Vk" ) : "Context";
    std::string returnVariable   = generateReturnVariable( command, returnParams, vectorParams, flavourFlags );
    std::string dataDeclarations = generateDataDeclarations(
      command, returnParams, vectorParams, templatedParams, flavourFlags, true, dataTypes, returnDataType, returnType, returnVariable );
    std::string dataPreparation =
      generateDataPreparation( command, initialSkipCount, returnParams, vectorParams, templatedParams, flavourFlags, enumerating, dataTypes );
    std::string dataSizeChecks = generateDataSizeChecks( command, returnParams, dataTypes, vectorParams, templatedParams, singular );
    std::string resultCheck = generateResultCheck( command,
                                                   className,
                                                   "::",
                                                   commandName,
                                                   enumerating,
                                                   ( returnParams.size() == 1 ) && ( command.params[returnParams[0]].type.name == "VkPipeline" ) && !singular,
                                                   true );
    std::string returnStatement = generateReturnStatement( command,
                                                           returnVariable,
                                                           returnType,
                                                           decoratedReturnType,
                                                           returnDataType,
                                                           initialSkipCount,
                                                           returnParams.empty() ? INVALID_INDEX : returnParams[0],
                                                           flavourFlags,
                                                           enumerating,
                                                           true );
    std::string vectorSizeCheckString =
      vectorSizeCheck.first ? generateVectorSizeCheck( name, command, initialSkipCount, vectorSizeCheck.second, skippedParams, false ) : "";

    return replaceWithMap( definitionTemplate,
                           { { "argumentList", argumentList },
                             { "argumentTemplates", argumentTemplates },
                             { "callSequence", callSequence },
                             { "className", className },
                             { "commandName", commandName },
                             { "dataDeclarations", dataDeclarations },
                             { "dataPreparation", dataPreparation },
                             { "dataSizeChecks", dataSizeChecks },
                             { "functionPointerCheck", generateFunctionPointerCheck( name, command.requiredBy, true ) },
                             { "nodiscard", nodiscard },
                             { "noexcept", noexceptString },
                             { "resultCheck", resultCheck },
                             { "returnStatement", returnStatement },
                             { "returnType", decoratedReturnType },
                             { "vectorSizeCheck", vectorSizeCheckString },
                             { "vkCommandName", name } } );
  }
  else
  {
    std::string const declarationTemplate =
      R"(
    // wrapper function for command ${vkCommandName}, see https://registry.khronos.org/vulkan/specs/latest/man/html/${vkCommandName}.html
    ${argumentTemplates} ${nodiscard} ${returnType} ${commandName}( ${argumentList} ) const ${noexcept};
)";

    return replaceWithMap( declarationTemplate,
                           { { "argumentList", argumentList },
                             { "argumentTemplates", argumentTemplates },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "noexcept", noexceptString },
                             { "returnType", decoratedReturnType },
                             { "vkCommandName", name } } );
  }
}

std::string VulkanHppGenerator::generateRAIIHandleCommandFactory( std::string const &                       name,
                                                                  Command const &                           command,
                                                                  size_t                                    initialSkipCount,
                                                                  std::vector<size_t> const &               returnParams,
                                                                  std::map<size_t, VectorParamData> const & vectorParams,
                                                                  bool                                      definition,
                                                                  CommandFlavourFlags                       flavourFlags ) const
{
  assert( std::ranges::any_of( returnParams, [this, &command]( auto returnParam ) { return isHandleType( command.params[returnParam].type.name ); } ) ||
          ( vectorParams.contains( returnParams.back() ) && vectorParams.find( returnParams.back() )->second.byStructure &&
            isHandleType( vectorMemberByStructure( command.params[returnParams.back()].type.name ).type.name ) ) );
  assert( determineVoidPointerParams( command.params ).empty() );

  bool const       enumerating    = determineEnumeration( vectorParams, returnParams );
  bool const       singular       = flavourFlags & CommandFlavourFlagBits::singular;
  std::set<size_t> skippedParams  = determineSkippedParams( command.params, initialSkipCount, vectorParams, returnParams, singular );
  std::set<size_t> singularParams = singular ? determineSingularParams( returnParams.back(), vectorParams ) : std::set<size_t>();
  std::string      argumentList   = generateRAIIHandleCommandFactoryArgumentList( command.params, skippedParams, definition, singular );
  std::string      commandName    = generateCommandName( name, command.params, initialSkipCount, flavourFlags );

  auto                     handleIt = m_handles.end();
  std::vector<std::string> returnTypes;
  std::string              noexceptString = enumerating ? "" : "VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS";
  for ( auto returnParam : returnParams )
  {
    // skip the return len parameter in enumeration functions
    if ( std::ranges::none_of( vectorParams,
                               [&returnParam]( auto const & vectorParam )
                               { return ( vectorParam.second.lenParam == returnParam ) && ( vectorParam.second.lenParam != vectorParam.first ); } ) )
    {
      auto vectorParamsIt = vectorParams.find( returnParam );
      if ( ( vectorParamsIt != vectorParams.end() ) && vectorParamsIt->second.byStructure )
      {
        returnTypes.push_back( vectorMemberByStructure( command.params[returnParam].type.name ).type.name );
      }
      else
      {
        returnTypes.push_back( command.params[returnParam].type.name );
      }
      if ( isHandleType( returnTypes.back() ) )
      {
        assert( handleIt == m_handles.end() );
        handleIt = m_handles.find( returnTypes.back() );
      }
      returnTypes.back() = stripPrefix( returnTypes.back(), "Vk" );
      if ( ( vectorParamsIt != vectorParams.end() ) && !singular )
      {
        noexceptString     = "";
        returnTypes.back() = "std::vector<" + returnTypes.back() + ">";
      }
    }
  }
  assert( handleIt != m_handles.end() );

  std::string returnType;
  switch ( returnTypes.size() )
  {
    case 0 : assert( false ); break;
    case 1 : returnType = returnTypes[0]; break;
    case 2 : returnType = "std::pair<" + returnTypes[0] + ", " + returnTypes[1] + ">"; break;
    default: assert( false ); break;
  }

  std::string decoratedReturnType = returnType;
  if ( !command.errorCodes.empty() )
  {
    decoratedReturnType = "typename ResultValueType<" + returnType + ">::type";
  }

  if ( definition )
  {
    std::string              className      = initialSkipCount ? stripPrefix( command.params[initialSkipCount - 1].type.name, "Vk" ) : "Context";
    std::vector<std::string> dataTypes      = determineDataTypes( command.params, vectorParams, returnParams, {}, true );
    std::string              returnDataType = generateReturnDataType( vectorParams, returnParams, enumerating, dataTypes, flavourFlags, true );
    std::string              returnVariable = generateReturnVariable( command, returnParams, vectorParams, flavourFlags );

    std::string dataDeclarations =
      generateDataDeclarations( command, returnParams, vectorParams, {}, flavourFlags, true, dataTypes, returnDataType, returnType, returnVariable );
    std::string callSequence =
      generateCallSequence( name, command, returnParams, vectorParams, initialSkipCount, singularParams, {}, {}, flavourFlags, true, true );
    std::string resultCheck      = generateResultCheck( command, className, "::", commandName, enumerating, false, true );
    std::string returnStatements = generateRAIIFactoryReturnStatements( command, handleIt->first, enumerating, returnType, returnVariable, singular );

    std::string const definitionTemplate =
      R"(
  // wrapper function for command ${vkCommandName}, see https://registry.khronos.org/vulkan/specs/latest/man/html/${vkCommandName}.html
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ${returnType} ${className}::${commandName}( ${argumentList} ) const ${noexcept}
  {
    ${functionPointerCheck}

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
                             { "functionPointerCheck", generateFunctionPointerCheck( name, command.requiredBy, true ) },
                             { "noexcept", noexceptString },
                             { "resultCheck", resultCheck },
                             { "returnStatements", returnStatements },
                             { "returnType", decoratedReturnType },
                             { "vkCommandName", name } } );
  }
  else
  {
    std::string const declarationTemplate =
      R"(
  // wrapper function for command ${vkCommandName}, see https://registry.khronos.org/vulkan/specs/latest/man/html/${vkCommandName}.html
  VULKAN_HPP_NODISCARD ${returnType} ${commandName}( ${argumentList} ) const ${noexcept};
)";

    return replaceWithMap( declarationTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "noexcept", noexceptString },
                             { "returnType", decoratedReturnType },
                             { "vkCommandName", name } } );
  }
}

std::string VulkanHppGenerator::generateRAIIHandleCommandFactoryArgumentList( std::vector<Param> const & params,
                                                                              std::set<size_t> const &   skippedParams,
                                                                              bool                       definition,
                                                                              bool                       singular ) const
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

std::string VulkanHppGenerator::generateRAIIHandleCommandStandard(
  std::string const & name, Command const & command, size_t initialSkipCount, bool definition, bool raii ) const
{
  std::set<size_t> skippedParams = determineSkippedParams( command.params, initialSkipCount, {}, {}, false );
  std::string      argumentList  = generateArgumentListStandard( command.params, skippedParams, definition, raii, false );
  std::string      commandName   = generateCommandName( name, command.params, initialSkipCount );
  std::string      nodiscard     = ( command.returnType.name != "void" ) ? "VULKAN_HPP_NODISCARD" : "";
  std::string      returnType    = stripPrefix( command.returnType.name, "Vk" );

  if ( definition )
  {
    std::string functionBody = "getDispatcher()->" + name + "( " + generateCallArgumentsStandard( command.params, initialSkipCount ) + " )";
    if ( command.returnType.name.starts_with( "Vk" ) )
    {
      functionBody = "return static_cast<" + returnType + ">( " + functionBody + " )";
    }
    else if ( command.returnType.name != "void" )
    {
      functionBody = "return " + functionBody;
    }

    std::string const functionTemplate =
      R"(  // wrapper function for command ${vkCommandName}, see https://registry.khronos.org/vulkan/specs/latest/man/html/${vkCommandName}.html
  ${nodiscard} VULKAN_HPP_INLINE ${returnType} ${className}::${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT
  {
    ${functionPointerCheck}
    ${functionBody};
  })";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "className", initialSkipCount ? stripPrefix( command.params[initialSkipCount - 1].type.name, "Vk" ) : "Context" },
                             { "commandName", commandName },
                             { "functionBody", functionBody },
                             { "functionPointerCheck", generateFunctionPointerCheck( name, command.requiredBy, true ) },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType },
                             { "vkCommandName", name } } );
  }
  else
  {
    std::string const declarationTemplate =
      R"(
    // wrapper function for command ${vkCommandName}, see https://registry.khronos.org/vulkan/specs/latest/man/html/${vkCommandName}.html
    ${nodiscard} ${returnType} ${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT;
)";

    return replaceWithMap( declarationTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType },
                             { "vkCommandName", name } } );
  }
}

std::pair<std::string, std::string> VulkanHppGenerator::generateRAIIHandleConstructor( std::pair<std::string, HandleData> const & handle,
                                                                                       Command const &                            constructor,
                                                                                       std::string const &                        enter,
                                                                                       std::string const &                        leave ) const
{
  std::string singularConstructor, arrayConstructor;
  if ( constructor.returnType.name == "VkResult" )
  {
    std::tie( singularConstructor, arrayConstructor ) = generateRAIIHandleConstructorResult( handle, constructor, enter, leave );
  }
  else if ( constructor.returnType.name == "void" )
  {
    std::tie( singularConstructor, arrayConstructor ) = generateRAIIHandleConstructorVoid( handle, constructor, enter, leave );
  }
  if ( singularConstructor.empty() && arrayConstructor.empty() )
  {
    throw std::runtime_error( "Never encountered a function like <" + constructor.name + "> !" );
  }
  return { singularConstructor, arrayConstructor };
}

std::pair<std::string, std::string>
  VulkanHppGenerator::generateRAIIHandleConstructor1Return2Vector( std::pair<std::string, HandleData> const & handle,
                                                                   Command const &                            constructor,
                                                                   std::string const &                        enter,
                                                                   std::string const &                        leave,
                                                                   size_t                                     returnParam,
                                                                   std::map<size_t, VectorParamData> const &  vectorParams ) const
{
  if ( returnParam == std::next( vectorParams.begin() )->first )
  {
    if ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam )
    {
      if ( constructor.params[vectorParams.begin()->second.lenParam].type.name == "uint32_t" )
      {
        if ( ( constructor.params[vectorParams.begin()->first].type.name != "void" ) &&
             !isHandleType( constructor.params[vectorParams.begin()->first].type.name ) )
        {
          std::string singularConstructor;
          auto        lenParamIt = constructor.params.begin() + vectorParams.begin()->second.lenParam;
          if ( !checkEquivalentSingularConstructor( handle.second.constructorIndices, constructor, lenParamIt ) )
          {
            singularConstructor = generateRAIIHandleConstructorByCall( handle, constructor, enter, leave, true, true );
          }
          return { singularConstructor, generateRAIIHandleConstructorByCall( handle, constructor, enter, leave, true, false ) };
        }
      }
    }
  }
  return {};
}

std::pair<std::string, std::string> VulkanHppGenerator::generateRAIIHandleConstructors( std::pair<std::string, HandleData> const & handle ) const
{
  auto const [enter, leave] = generateProtection( getProtectFromType( handle.first ) );

  std::string singularConstructors, arrayConstructors;
  for ( auto constructorIndex : handle.second.constructorIndices )
  {
    // there is a non-const parameter with handle type : the to-be-constructed handle

    // check for additional enter/leave guards for the constructors
    auto [constructorEnter, constructorLeave] = generateProtection( getProtectFromTitles( m_vkxml.commands[constructorIndex].requiredBy ) );
    if ( constructorEnter == enter )
    {
      constructorEnter.clear();
      constructorLeave.clear();
    }

    auto const [singularConstructor, arrayConstructor] =
      generateRAIIHandleConstructor( handle, m_vkxml.commands[constructorIndex], constructorEnter, constructorLeave );
    arrayConstructors += arrayConstructor;
    singularConstructors += singularConstructor;
  }
  singularConstructors += generateRAIIHandleConstructorTakeOwnership( handle );
  return { singularConstructors, arrayConstructors };
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorArgument( Param const & param, bool definition, bool singular, bool takesOwnership ) const
{
  std::string argument;
  if ( param.type.isConstPointer() )
  {
    assert( param.type.name.starts_with( "Vk" ) );
    assert( param.name.starts_with( "p" ) );
    std::string argumentName = startLowerCase( stripPrefix( param.name, "p" ) );
    std::string argumentType = stripPrefix( param.type.name, "Vk" );
    if ( isOptional( param.optional ) )
    {
      assert( param.len.empty() );
      argument = "Optional<" + argumentType + " const> " + argumentName + ( definition ? "" : " = nullptr" );
    }
    else if ( param.len.empty() )
    {
      argument = argumentType + " const & " + argumentName;
    }
    else if ( singular )
    {
      argument = argumentType + " const & " + stripPluralS( argumentName );
    }
    else
    {
      argument = "ArrayProxy<" + argumentType + "> const & " + argumentName;
    }
  }
  else if ( specialPointerTypes.contains( param.type.name ) )
  {
    assert( !isOptional( param.optional ) );
    assert( param.type.isNonConstPointer() );
    argument = param.type.name + " & " + param.name;
  }
  else if ( ( param.type.isValue() ) && isHandleType( param.type.name ) )
  {
    if ( takesOwnership )
    {
      assert( !isOptional( param.optional ) );
      argument = param.type.name + " " + param.name;
    }
    else
    {
      argument = stripPrefix( param.type.name, "Vk" );
      if ( isOptional( param.optional ) )
      {
        argument = "Optional<" + argument + " const>";
      }
      argument += " const & " + param.name;
    }
  }
  else
  {
    assert( !isOptional( param.optional ) );
    if ( param.arraySizes.empty() )
    {
      argument = param.type.compose( "Vk", "VULKAN_HPP_NAMESPACE" ) + " ";
    }
    else
    {
      argument = generateStandardArray( param.type.compose( "Vk", "VULKAN_HPP_NAMESPACE" ), param.arraySizes ) + " const & ";
    }
    argument += param.name;
  }
  return argument;
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorArguments( std::pair<std::string, HandleData> const & handle,
                                                                        Command const &                            constructor,
                                                                        bool                                       singular,
                                                                        bool                                       takesOwnership ) const
{
  auto [parentType, parentName] = getParentTypeAndName( handle );

  std::string arguments = parentType + " const & " + parentName;
  if ( takesOwnership )
  {
    arguments += ", " + handle.first + " " + generateRAIIHandleConstructorParamName( handle.first, m_vkxml.commands[handle.second.destructorIndex] );
  }

  parentType      = "Vk" + parentType;
  bool const skip = skipLeadingGrandParent( handle );
  for ( size_t i = skip ? 1 : 0; i < constructor.params.size(); i++ )
  {
    Param const & param = constructor.params[i];
    // filter parent and handle type
    if ( ( param.type.name != parentType ) && ( param.type.name != handle.first ) )
    {
      // the specialPointerTypes are considered const-pointers!
      if ( param.type.isNonConstPointer() && !specialPointerTypes.contains( param.type.name ) )
      {
#if !defined( NDEBUG )
        if ( ( param.type.name == "size_t" ) || ( param.type.name == "uint32_t" ) )
        {
          // this is supposed to be the returned size on an enumeration function!
          auto typeIt = std::ranges::find_if( constructor.params, [&handle]( Param const & pd ) { return pd.type.name == handle.first; } );
          assert( typeIt != constructor.params.end() );
          assert( typeIt->len == param.name );
        }
        else
        {
          // this is a returned vector, either directly or via a structure member
          assert( !param.len.empty() || vectorMemberByStructure( param.type.name ).type.name == handle.first );
        }
#endif
        continue;
      }
      else if ( std::ranges::any_of( constructor.params, [&param]( Param const & pd ) { return pd.len == param.name; } ) )
      {
        // this is the len of an other parameter, which will be mapped to an ArrayProxy
        assert( param.type.isValue() && ( param.type.name == "uint32_t" ) );
        assert( param.arraySizes.empty() && param.len.empty() && !isOptional( param.optional ) );
        continue;
      }
      arguments += ", " + generateRAIIHandleConstructorArgument( param, false, singular, takesOwnership );
    }
  }
  return arguments;
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorArgumentsNoDestructor( std::pair<std::string, HandleData> const & handle,
                                                                                    bool                                       takesOwnership ) const
{
  auto [parentType, parentName] = getParentTypeAndName( handle );

  std::string arguments = parentType + " const & " + parentName;
  if ( takesOwnership )
  {
    arguments += ", " + handle.first + " " + startLowerCase( stripPrefix( handle.first, "Vk" ) );
  }

  return arguments;
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorInitializationList( std::pair<std::string, HandleData> const & handle,
                                                                                 Command const &                            destructor,
                                                                                 bool                                       takesOwnership ) const
{
  auto const [parentType, parentName] = getParentTypeAndName( handle );
  std::string handleName              = generateRAIIHandleConstructorParamName( handle.first, destructor );

  std::string initializationList;
  for ( auto destructorParam : destructor.params )
  {
    if ( destructorParam.type.name == "Vk" + parentType )
    {
      initializationList += "m_" + parentName + "( " + parentName + " ), ";
    }
    else if ( destructorParam.type.name == handle.first )
    {
      if ( takesOwnership )
      {
        initializationList += "m_" + startLowerCase( stripPrefix( handle.first, "Vk" ) ) + "( " + handleName + " ), ";
      }
    }
    else if ( destructorParam.type.name == "VkAllocationCallbacks" )
    {
      assert( destructorParam.type.isConstPointer() && destructorParam.arraySizes.empty() && destructorParam.len.empty() &&
              isOptional( destructorParam.optional ) );
      initializationList += "m_allocator( static_cast<const AllocationCallbacks *>( allocator ) ), ";
    }
    else if ( isHandleType( destructorParam.type.name ) )
    {
      assert( destructorParam.type.isValue() && destructorParam.arraySizes.empty() && destructorParam.len.empty() && !isOptional( destructorParam.optional ) );
      initializationList += "m_" + destructorParam.name + "( ";
      if ( destructorParam.type.name == "Vk" + parentType )
      {
        initializationList += "*";
      }
      initializationList += destructorParam.name;
      initializationList += " ), ";
    }
    else
    {
      // we can ignore all other parameters here !
    }
  }
  return initializationList.empty() ? initializationList : initializationList.substr( 0, initializationList.size() - 2 );
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorInitializationListNoDestructor( std::pair<std::string, HandleData> const & handle,
                                                                                             bool                                       takesOwnership ) const
{
  auto const [parentType, parentName] = getParentTypeAndName( handle );
  std::string handleName              = startLowerCase( stripPrefix( handle.first, "Vk" ) );

  std::string initializationList;
  if ( !handle.second.secondLevelCommands.empty() )
  {
    assert( !handle.second.constructorIndices.empty() );
    auto const & constructorCommand = m_vkxml.commands[handle.second.constructorIndices.front()];
    assert( ( 1 < constructorCommand.params.size() ) );
    if ( std::next( constructorCommand.params.begin() )->type.name == "Vk" + parentType )
    {
      auto const & commandIt = findByName( m_vkxml.commands, *handle.second.secondLevelCommands.begin() );
      assert( ( 1 < commandIt->params.size() ) );
      assert( commandIt->params.front().type.name == constructorCommand.params.front().type.name );
      assert( std::next( commandIt->params.begin() )->type.name == handle.first );

      std::string grandParentType = stripPrefix( commandIt->params.front().type.name, "Vk" );
      initializationList += "m_" + startLowerCase( grandParentType ) + "( " + parentName + ".get" + grandParentType + "() ), ";
    }
#if !defined( NDEBUG )
    else
    {
      assert( constructorCommand.params.front().type.name == "Vk" + parentType );
    }
#endif
  }
  if ( takesOwnership )
  {
    initializationList += "m_" + handleName + "( " + handleName + " ), ";
  }
  return initializationList.empty() ? initializationList : initializationList.substr( 0, initializationList.size() - 2 );
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorParamName( std::string const & type, Command const & destructor ) const
{
  auto destructorParamIt = std::ranges::find_if( destructor.params, [&type]( Param const & destructorParam ) { return destructorParam.type.name == type; } );
  if ( destructorParamIt != destructor.params.end() )
  {
    assert( std::none_of(
      std::next( destructorParamIt ), destructor.params.end(), [&type]( Param const & destructorParam ) { return destructorParam.type.name == type; } ) );
    if ( !destructorParamIt->type.isValue() )
    {
      return startLowerCase( stripPrefix( stripPluralS( destructorParamIt->name ), "p" ) );
    }
    else
    {
      return destructorParamIt->name;
    }
  }
  return startLowerCase( stripPrefix( type, "Vk" ) );
}

std::pair<std::string, std::string> VulkanHppGenerator::generateRAIIHandleConstructorResult( std::pair<std::string, HandleData> const & handle,
                                                                                             Command const &                            constructor,
                                                                                             std::string const &                        enter,
                                                                                             std::string const &                        leave ) const
{
  assert( !constructor.successCodes.empty() );
  assert( constructor.successCodes[0] == "VK_SUCCESS" );
  switch ( constructor.successCodes.size() )
  {
    case 1:
      if ( !constructor.errorCodes.empty() )
      {
        std::vector<size_t> returnParams = determineReturnParams( constructor.params );
        switch ( returnParams.size() )
        {
          case 1:
            {
              assert( isHandleType( constructor.params[returnParams[0]].type.name ) );
              std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructor.params );
              switch ( vectorParams.size() )
              {
                case 0: return { generateRAIIHandleConstructorByCall( handle, constructor, enter, leave, false, false ), "" };
                case 1:
                  if ( returnParams[0] == vectorParams.begin()->first )
                  {
                    if ( isLenByStructMember( constructor.params[vectorParams.begin()->first].len, constructor.params[vectorParams.begin()->second.lenParam] ) )
                    {
                      return { "", generateRAIIHandleConstructorByCall( handle, constructor, enter, leave, true, false ) };
                    }
                  }
                  break;
                case 2 : return generateRAIIHandleConstructor1Return2Vector( handle, constructor, enter, leave, returnParams[0], vectorParams );
                default: break;
              }
            }
            break;
          case 2:
            if ( isHandleType( constructor.params[returnParams[0]].type.name ) && ( constructor.params[returnParams[1]].type.name == "VkResult" ) )
            {
              std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructor.params );
              if ( vectorParams.size() == 3 )
              {
                if ( std::ranges::all_of( returnParams, [&vectorParams]( auto returnParam ) { return vectorParams.contains( returnParam ); } ) )
                {
                  return { generateRAIIHandleConstructorByCall( handle, constructor, enter, leave, true, true ),
                           generateRAIIHandleConstructorByCall( handle, constructor, enter, leave, true, false ) };
                }
              }
            }
            break;
          default: assert( false ); break;
        }
      }
      break;
    case 2:
    case 3:
      if ( !constructor.errorCodes.empty() )
      {
        std::vector<size_t> returnParams = determineReturnParams( constructor.params );
        switch ( returnParams.size() )
        {
          case 1:
            {
              std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructor.params );
              if ( isHandleType( constructor.params[returnParams[0]].type.name ) )
              {
                if ( vectorParams.size() == 2 )
                {
                  return generateRAIIHandleConstructor1Return2Vector( handle, constructor, enter, leave, returnParams[0], vectorParams );
                }
              }
              else if ( ( vectorParams.size() == 1 ) && ( vectorParams.begin()->first == returnParams[0] ) && vectorParams.begin()->second.byStructure )
              {
                assert( isHandleTypeByStructure( constructor.params[returnParams[0]].type.name ) );
                return { "", generateRAIIHandleConstructorByCall( handle, constructor, enter, leave, true, false ) };
              }
            }
            break;
          case 2:
            if ( ( constructor.params[returnParams[0]].type.name == "size_t" ) || ( constructor.params[returnParams[0]].type.name == "uint32_t" ) )
            {
              assert( isHandleType( constructor.params[returnParams[1]].type.name ) );
              std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructor.params );
              if ( vectorParams.size() == 1 )
              {
                if ( returnParams[0] == vectorParams.begin()->second.lenParam )
                {
                  assert( returnParams[1] == vectorParams.begin()->first );
                  assert( constructor.successCodes[1] == "VK_INCOMPLETE" );
                  return { "", generateRAIIHandleConstructorByCall( handle, constructor, enter, leave, true, false ) };
                }
              }
            }
            else if ( isHandleType( constructor.params[returnParams[0]].type.name ) && ( constructor.params[returnParams[1]].type.name == "VkResult" ) )
            {
              std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructor.params );
              switch ( vectorParams.size() )
              {
                case 2:
                  if ( ( vectorParams.begin()->first == returnParams[0] ) && ( std::next( vectorParams.begin() )->first == returnParams[1] ) &&
                       ( vectorParams.begin()->second.lenParam == std::next( vectorParams.begin() )->second.lenParam ) )
                  {
                    return { "", generateRAIIHandleConstructorByCall( handle, constructor, enter, leave, true, false ) };
                  }
                  break;
                case 3:
                  if ( std::ranges::all_of( returnParams, [&vectorParams]( auto returnParam ) { return vectorParams.contains( returnParam ); } ) )
                  {
                    return { generateRAIIHandleConstructorByCall( handle, constructor, enter, leave, true, true ),
                             generateRAIIHandleConstructorByCall( handle, constructor, enter, leave, true, false ) };
                  }
                  break;
              }
            }
            break;
          default: break;
        }
      }
      break;
    case 4:
      if ( !constructor.errorCodes.empty() )
      {
        std::vector<size_t> returnParams = determineReturnParams( constructor.params );
        if ( returnParams.size() == 1 )
        {
          assert( isHandleType( constructor.params[returnParams[0]].type.name ) );
          std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructor.params );
          if ( vectorParams.size() == 2 )
          {
            return generateRAIIHandleConstructor1Return2Vector( handle, constructor, enter, leave, returnParams[0], vectorParams );
          }
        }
      }
      break;
    default: break;
  }
  return {};
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorByCall( std::pair<std::string, HandleData> const & handle,
                                                                     Command const &                            constructor,
                                                                     std::string const &                        enter,
                                                                     std::string const &                        leave,
                                                                     bool                                       isPlural,
                                                                     bool                                       forceSingular ) const
{
  auto const [parentType, parentName] = getParentTypeAndName( handle );

  size_t skipCount = 0;
  if ( isHandleType( constructor.params[skipCount].type.name ) )
  {
    ++skipCount;
  }
  auto vkHandleIt = findByName( m_vkxml.types.handles, handle.first );
  assert( vkHandleIt != m_vkxml.types.handles.end() );
  if ( vkHandleIt->parent == constructor.params[skipCount].type.name )
  {
    ++skipCount;
  }
  std::string constructorArguments = generateRAIIHandleConstructorArguments( handle, constructor, forceSingular, false );
  std::string createCall =
    generateCommandName( constructor.name, constructor.params, skipCount, forceSingular ? CommandFlavourFlagBits::singular : CommandFlavourFlagBits() );

  std::string createArguments;
  for ( size_t i = skipCount; i < constructor.params.size(); ++i )
  {
    if ( constructor.params[i].type.isValue() || specialPointerTypes.contains( constructor.params[i].type.name ) )
    {
      createArguments += constructor.params[i].name + ", ";
    }
    else if ( constructor.params[i].type.isConstPointer() )
    {
      assert( constructor.params[i].len.empty() || isPlural );
      if ( !constructor.params[i].len.empty() )
      {
        std::string::size_type const start = createArguments.find( constructor.params[i].len );
        assert( start != std::string::npos );
        // remove the size/count argument, and its trailing ", "
        createArguments.replace( start, constructor.params[i].len.length() + 2, "" );
      }
      std::string argument = startLowerCase( stripPrefix( constructor.params[i].name, "p" ) );
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

  std::vector<size_t> returnParams = determineReturnParams( constructor.params );
  if ( ( returnParams.size() == 2 ) && isHandleType( constructor.params[returnParams[0]].type.name ) &&
       ( constructor.params[returnParams[1]].type.name == "VkResult" ) )
  {
#if !defined( NDEBUG )
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructor.params );
#endif
    assert( vectorParams.contains( returnParams[0] ) && vectorParams.contains( returnParams[1] ) );

    assert( isPlural );
    if ( forceSingular )
    {
      std::string const constructorTemplate =
        R"(
#if !defined( VULKAN_HPP_NO_EXCEPTIONS )
${enter}    ${handleType}( ${constructorArguments} )
    {
      std::pair<${handleType}, Result> data_ = ${parentName}.${createCall}( ${createArguments} );
      if ( data_.second != Result::eSuccess )
      {
        VULKAN_HPP_NAMESPACE::detail::throwResultException( data_.second, VULKAN_HPP_RAII_NAMESPACE_STRING "::Device::${createCall}" );
      }
      *this = std::move( data_.first );
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
                               { "parentName", parentName } } );
    }
    else
    {
      std::string const constructorTemplate =
        R"(
#if !defined( VULKAN_HPP_NO_EXCEPTIONS )
${enter}    ${handleType}s( ${constructorArguments} )
    {
      std::pair<std::vector<${handleType}>, std::vector<Result>> data_ = ${parentName}.${createCall}( ${createArguments} );
      for ( auto const & result_ : data_.second )
      {
        if ( result_ != Result::eSuccess )
        {
          VULKAN_HPP_NAMESPACE::detail::throwResultException( result_, VULKAN_HPP_RAII_NAMESPACE_STRING "::Device::${createCall}" );
        }
      }
      *this = std::move( data_.first );
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
                               { "parentName", parentName } } );
    }
  }
  else
  {
    std::string const constructorTemplate =
      R"(
#if !defined( VULKAN_HPP_NO_EXCEPTIONS )
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
}

std::string VulkanHppGenerator::generateRAIIHandleConstructorTakeOwnership( std::pair<std::string, HandleData> const & handle ) const
{
  std::string handleType = stripPrefix( handle.first, "Vk" );
  std::string handleName = startLowerCase( handleType );

  auto const [parentType, parentName] = getParentTypeAndName( handle );

  std::string constructorArguments, initializationList;
  if ( handle.second.destructorIndex != INVALID_INDEX )
  {
    auto const & destructor = m_vkxml.commands[handle.second.destructorIndex];
    constructorArguments    = generateRAIIHandleConstructorArguments( handle, destructor, false, true );
    initializationList      = generateRAIIHandleConstructorInitializationList( handle, destructor, true );
  }
  else
  {
    constructorArguments = generateRAIIHandleConstructorArgumentsNoDestructor( handle, true );
    initializationList   = generateRAIIHandleConstructorInitializationListNoDestructor( handle, true );
  }

  assert( !handle.second.constructorIndices.empty() );
  auto const & constructor = m_vkxml.commands[handle.second.constructorIndices.front()];
  if ( ( 1 < constructor.successCodes.size() && ( constructor.successCodes[1] != "VK_INCOMPLETE" ) ) || ( 2 < constructor.successCodes.size() ) )
  {
#if !defined( NDEBUG )
    for ( auto constructorIndex : handle.second.constructorIndices )
    {
      assert( 1 < m_vkxml.commands[constructorIndex].successCodes.size() );
    }
#endif
    constructorArguments += ", Result successCode = Result::eSuccess";
    if ( isMultiSuccessCodeConstructor( handle.second.constructorIndices ) )
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
    dispatcherInit = "\n        m_dispatcher.reset( new detail::" + handleType + "Dispatcher( " + parentName + ".getDispatcher()->vkGet" + handleType +
                     "ProcAddr, static_cast<" + handle.first + ">( m_" + startLowerCase( handleType ) + " ) ) );";
  }

  std::string const constructorTemplate =
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

std::pair<std::string, std::string> VulkanHppGenerator::generateRAIIHandleConstructorVoid( std::pair<std::string, HandleData> const & handle,
                                                                                           Command const &                            constructor,
                                                                                           std::string const &                        enter,
                                                                                           std::string const &                        leave ) const
{
  assert( constructor.successCodes.empty() && constructor.errorCodes.empty() );
  std::vector<size_t> returnParams = determineReturnParams( constructor.params );
  if ( returnParams.size() == 1 )
  {
    assert( isHandleType( constructor.params[returnParams[0]].type.name ) );
    std::map<size_t, VectorParamData> vectorParams = determineVectorParams( constructor.params );
    if ( vectorParams.empty() )
    {
      return { generateRAIIHandleConstructorByCall( handle, constructor, enter, leave, false, false ), "" };
    }
  }
  return {};
}

std::string VulkanHppGenerator::generateRAIIHandleContext( std::pair<std::string, HandleData> const & handle,
                                                           std::set<std::string> const &              specialFunctions ) const
{
  std::string const contextTemplate = R"(
    class Context
    {
    public:
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
      Context() : m_dispatcher( new detail::ContextDispatcher( m_dynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" ) ) )
#else
      Context( PFN_vkGetInstanceProcAddr getInstanceProcAddr ) : m_dispatcher( new detail::ContextDispatcher( getInstanceProcAddr ) )
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

      detail::ContextDispatcher const * getDispatcher() const
      {
        VULKAN_HPP_ASSERT( m_dispatcher->getVkHeaderVersion() == VK_HEADER_VERSION );
        return &*m_dispatcher;
      }

      void swap( Context & rhs )
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
      std::unique_ptr<detail::ContextDispatcher> m_dispatcher;
    };

)";

  return replaceWithMap( contextTemplate, { { "memberFunctionDeclarations", generateRAIIHandleCommandDeclarations( handle, specialFunctions ) } } );
}

std::string VulkanHppGenerator::generateRAIIHandleDestructorCallArguments( std::string const & handleType, Command const & destructor ) const
{
  std::vector<std::string> arguments;
  for ( auto param : destructor.params )
  {
    if ( param.type.name == handleType )
    {
      std::string handleName = startLowerCase( stripPrefix( handleType, "Vk" ) );
      if ( param.type.isValue() )
      {
        arguments.push_back( "static_cast<" + handleType + ">( m_" + handleName + " )" );
      }
      else
      {
        arguments.push_back( "reinterpret_cast<" + handleType + " const *>( &m_" + handleName + " )" );
      }
    }
    else if ( param.type.name == "VkAllocationCallbacks" )
    {
      // VULKAN_HPP_NAMESPACE::AllocationCallbacks is stored as a member of the handle class
      arguments.push_back( "reinterpret_cast<VkAllocationCallbacks const *>( m_allocator )" );
    }
    else if ( isHandleType( param.type.name ) )
    {
      assert( param.arraySizes.empty() );
      std::string argument = "m_" + param.name;
      if ( param.type.isValue() )
      {
        arguments.push_back( "static_cast<" + param.type.name + ">( " + argument + " )" );
      }
      else
      {
        assert( param.type.isConstPointer() );
        assert( !param.len.empty() && std::ranges::any_of( destructor.params, [&param]( Param const & pd ) { return pd.name == param.len; } ) );
        arguments.push_back( "reinterpret_cast<" + param.type.name + " const *>( &" + argument + " )" );
      }
    }
    else
    {
      assert( ( param.type.name == "uint32_t" ) && param.type.isValue() && param.arraySizes.empty() && param.len.empty() && !isOptional( param.optional ) );
      assert( std::ranges::any_of( destructor.params, [&param]( Param const & pd ) { return pd.len == param.name; } ) );
      arguments.push_back( "1" );
    }
  }
  return generateList( arguments, "", ", " );
}

std::tuple<std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string>
  VulkanHppGenerator::generateRAIIHandleDetails( std::pair<std::string, HandleData> const & handle ) const
{
  std::string getConstructorSuccessCode;
  bool const  multiSuccessCodeContructor = isMultiSuccessCodeConstructor( handle.second.constructorIndices );
  if ( multiSuccessCodeContructor )
  {
    getConstructorSuccessCode = R"(
    Result getConstructorSuccessCode() const
    {
      return m_constructorSuccessCode;
    }
)";
  }

  auto const [parentType, parentName] = getParentTypeAndName( handle );

  std::string handleName = startLowerCase( stripPrefix( handle.first, "Vk" ) );

  std::string clearMembers, moveConstructorInitializerList, rawConstructorInitializerList, rawConstructorParams, moveAssignmentInstructions, memberVariables,
    swapMembers, releaseMembers;

  if ( handle.second.destructorIndex != INVALID_INDEX )
  {
    auto const & destructor = m_vkxml.commands[handle.second.destructorIndex];
    clearMembers            = "        if ( m_" + handleName + " )\n";
    clearMembers += "        {\n";
    clearMembers += "          getDispatcher()->" + destructor.name + "( " + generateRAIIHandleDestructorCallArguments( handle.first, destructor ) + " );\n";
    clearMembers += "        }";
    for ( auto const & destructorParam : destructor.params )
    {
      std::string memberName, memberType;
      if ( destructorParam.type.name == "Vk" + parentType )
      {
        memberName = parentName;
        memberType = "VULKAN_HPP_NAMESPACE::" + parentType;
      }
      else if ( destructorParam.type.name == handle.first )
      {
        assert( handle.first.starts_with( "Vk" ) );
        memberName = handleName;
        memberType = "VULKAN_HPP_NAMESPACE::" + stripPrefix( handle.first, "Vk" );
      }
      else if ( std::ranges::none_of( m_vkxml.commands[handle.second.destructorIndex].params,
                                      [&destructorParam]( Param const & pd ) { return pd.len == destructorParam.name; } ) )
      {
        std::string name = destructorParam.name;
        if ( !destructorParam.type.isValue() )
        {
          name = startLowerCase( stripPrefix( name, "p" ) );
        }
        memberName = name;
        memberType = destructorParam.type.compose( "Vk", m_handles.contains( destructorParam.type.name ) ? "VULKAN_HPP_NAMESPACE" : "" );
      }
      if ( !memberName.empty() )
      {
        clearMembers += "\n      m_" + memberName + " = nullptr;";
        moveConstructorInitializerList += "m_" + memberName + "( exchange( rhs.m_" + memberName + ", {} ) ), ";
        rawConstructorInitializerList += "m_" + memberName + "( exchange( " + memberName + ", {} ) ), ";
        rawConstructorParams += memberType + " " + memberName + ", ";
        moveAssignmentInstructions += "\n          std::swap( m_" + memberName + ", rhs.m_" + memberName + " );";
        memberVariables += "\n    " + memberType + " m_" + memberName + " = {};";
        swapMembers += "\n      std::swap( m_" + memberName + ", rhs.m_" + memberName + " );";
        if ( destructorParam.type.name != handle.first )
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
      assert( !handle.second.constructorIndices.empty() );
      auto const & constructor = m_vkxml.commands[handle.second.constructorIndices.front()];
      assert( !constructor.params.empty() );
      auto const & frontType = constructor.params.front().type.name;
      assert( isHandleType( frontType ) );
#if !defined( NDEBUG )
      auto handleIt = m_handles.find( "Vk" + parentType );
#endif
      assert( handleIt != m_handles.end() );
      std::string frontName = constructor.params.front().name;

      clearMembers += "\n        m_" + frontName + " = nullptr;";
      moveConstructorInitializerList = "m_" + frontName + "( exchange( rhs.m_" + frontName + ", {} ) ), ";
      rawConstructorInitializerList += "m_" + frontName + "( exchange( " + frontName + ", {} ) ), ";
      rawConstructorParams += "VULKAN_HPP_NAMESPACE::" + stripPrefix( frontType, "Vk" ) + " " + frontName + ", ";
      moveAssignmentInstructions = "\n          std::swap( m_" + frontName + ", rhs.m_" + frontName + " );";
      memberVariables            = "\n    VULKAN_HPP_NAMESPACE::" + stripPrefix( frontType, "Vk" ) + " m_" + frontName + " = {};";
      swapMembers                = "\n      std::swap( m_" + frontName + ", rhs.m_" + frontName + " );";
      releaseMembers += "\n        m_" + frontName + " = nullptr;";
    }
    clearMembers += "\n        m_" + handleName + " = nullptr;";
    moveConstructorInitializerList += "m_" + handleName + "( exchange( rhs.m_" + handleName + ", {} ) ), ";
    rawConstructorInitializerList += "m_" + handleName + "( exchange( " + handleName + ", {} ) ), ";
    rawConstructorParams += "VULKAN_HPP_NAMESPACE::" + stripPrefix( handle.first, "Vk" ) + " " + handleName + ", ";
    moveAssignmentInstructions += "\n          std::swap( m_" + handleName + ", rhs.m_" + handleName + " );";
    assert( handle.first.starts_with( "Vk" ) );
    memberVariables += "\n    VULKAN_HPP_NAMESPACE::" + stripPrefix( handle.first, "Vk" ) + " m_" + handleName + " = {};";
    swapMembers += "\n      std::swap( m_" + handleName + ", rhs.m_" + handleName + " );";
  }

  if ( multiSuccessCodeContructor )
  {
    clearMembers += "\n        m_constructorSuccessCode = Result::eErrorUnknown;";
    memberVariables += "\n    Result m_constructorSuccessCode = Result::eErrorUnknown;";
    swapMembers += "\n      std::swap( m_constructorSuccessCode, rhs.m_constructorSuccessCode );";
    moveConstructorInitializerList += "m_constructorSuccessCode( exchange( rhs.m_constructorSuccessCode, {} ) ), ";
    rawConstructorInitializerList += "m_constructorSuccessCode( exchange( constructorSuccessCode, {} ) ), ";
    rawConstructorParams += "Result constructorSuccessCode, ";
    moveAssignmentInstructions += "\n          std::swap( m_constructorSuccessCode, rhs.m_constructorSuccessCode );";
    releaseMembers += "\n        m_constructorSuccessCode = Result::eErrorUnknown;";
  }

  if ( handle.first == "VkInstance" )
  {
    memberVariables += "\n      std::unique_ptr<detail::InstanceDispatcher> m_dispatcher;";
    rawConstructorParams += "std::unique_ptr<detail::InstanceDispatcher> dispatcher";
  }
  else if ( handle.first == "VkDevice" )
  {
    memberVariables += "\n      std::unique_ptr<detail::DeviceDispatcher> m_dispatcher;";
    rawConstructorParams += "std::unique_ptr<detail::DeviceDispatcher> dispatcher";
  }
  else if ( m_vkxml.commands[handle.second.constructorIndices.front()].params.front().type.name == "VkDevice" )
  {
    memberVariables += "\n      detail::DeviceDispatcher const * m_dispatcher = nullptr;";
    rawConstructorParams += "detail::DeviceDispatcher const * dispatcher";
  }
  else
  {
    memberVariables += "\n      detail::InstanceDispatcher const * m_dispatcher = nullptr;";
    rawConstructorParams += "detail::InstanceDispatcher const * dispatcher";
  }
  clearMembers += "\n        m_dispatcher = nullptr;";
  swapMembers += "\n      std::swap( m_dispatcher, rhs.m_dispatcher );";
  releaseMembers += "\n        m_dispatcher = nullptr;";
  releaseMembers += "\n        return exchange( m_" + handleName + ", nullptr );";

  if ( ( handle.first == "VkInstance" ) || ( handle.first == "VkDevice" ) )
  {
    moveConstructorInitializerList += "m_dispatcher( rhs.m_dispatcher.release() )";
  }
  else
  {
    moveConstructorInitializerList += "m_dispatcher( exchange( rhs.m_dispatcher, nullptr ) )";
  }
  rawConstructorInitializerList += "m_dispatcher( exchange( dispatcher, nullptr ) )";
  moveAssignmentInstructions += "\n        std::swap( m_dispatcher, rhs.m_dispatcher );";

  return std::make_tuple( clearMembers,
                          getConstructorSuccessCode,
                          memberVariables,
                          rawConstructorParams,
                          rawConstructorInitializerList,
                          moveConstructorInitializerList,
                          moveAssignmentInstructions,
                          swapMembers,
                          releaseMembers );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateRAIIHandleForwardDeclarations( std::vector<RequireType> const & requireTypes, std::string const & title ) const
{
  std::string str;
  for ( auto const & require : requireTypes )
  {
    for ( auto const & type : require.types )
    {
      auto vkHandleIt = findByNameOrAlias( m_vkxml.types.handles, type.name );
      if ( vkHandleIt != m_vkxml.types.handles.end() )
      {
        str += ( vkHandleIt->name == type.name ) ? ( "  class " + stripPrefix( vkHandleIt->name, "Vk" ) + ";\n" )
                                                 : ( "  using " + stripPrefix( type.name, "Vk" ) + " = " + stripPrefix( vkHandleIt->name, "Vk" ) + ";\n" );
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateRAIIHandles() const
{
  std::string const raiiHandlesTemplate = R"(
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

#if 20 <= VULKAN_HPP_CPP_VERSION
  template <typename T>
  concept VulkanRAIIHandleType = isVulkanRAIIHandleType<T>::value;
#endif

${raiiHandles}

  // operators to compare VULKAN_HPP_NAMESPACE::raii-handles
#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanRAIIHandleType<T>::value, bool>::type = 0>
#else
  template <VulkanRAIIHandleType T>
#endif
#if defined(VULKAN_HPP_HAS_SPACESHIP_OPERATOR)
  auto operator<=>( T const & a, T const & b ) VULKAN_HPP_NOEXCEPT
  {
    return *a <=> *b;
  }
#else
  bool operator<(T const & a, T const & b ) VULKAN_HPP_NOEXCEPT
  {
    return *a < *b;
  }
#endif

#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanRAIIHandleType<T>::value, bool>::type = 0>
#else
  template <VulkanRAIIHandleType T>
#endif
  bool operator==( T const & a, T const & b ) VULKAN_HPP_NOEXCEPT
  {
    return *a == *b;
  }

#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanRAIIHandleType<T>::value, bool>::type = 0>
#else
  template <VulkanRAIIHandleType T>
#endif
  bool operator!=(T const & a, T const & b ) VULKAN_HPP_NOEXCEPT
  {
    return *a != *b;
  }

#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanRAIIHandleType<T>::value, bool>::type = 0>
#else
  template <VulkanRAIIHandleType T>
#endif
  bool operator==( T const & v, std::nullptr_t ) VULKAN_HPP_NOEXCEPT
  {
    return !*v;
  }

#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanRAIIHandleType<T>::value, bool>::type = 0>
#else
  template <VulkanRAIIHandleType T>
#endif
  bool operator==( std::nullptr_t, T const & v ) VULKAN_HPP_NOEXCEPT
  {
    return !*v;
  }

#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanRAIIHandleType<T>::value, bool>::type = 0>
#else
  template <VulkanRAIIHandleType T>
#endif
  bool operator!=( T const & v, std::nullptr_t ) VULKAN_HPP_NOEXCEPT
  {
    return *v;
  }

#if VULKAN_HPP_CPP_VERSION < 20
  template <typename T, typename std::enable_if<isVulkanRAIIHandleType<T>::value, bool>::type = 0>
#else
  template <VulkanRAIIHandleType T>
#endif
  bool operator!=( std::nullptr_t, T const & v ) VULKAN_HPP_NOEXCEPT
  {
    return *v;
  }
)";

  std::string forwardDeclarations;
  for ( auto const & feature : m_vkxml.features )
  {
    forwardDeclarations += generateRAIIHandleForwardDeclarations( feature.require, feature.name );
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
                                                                                std::vector<Param> const &                 params,
                                                                                std::string const &                        argumentType,
                                                                                std::string const &                        argumentName ) const
{
  std::string arguments = "*reinterpret_cast<" + argumentType + " *>( &" + argumentName + " )";
  if ( handle.second.destructorIndex != INVALID_INDEX )
  {
    auto [parentType, parentName] = getParentTypeAndName( handle );
    parentType                    = "Vk" + parentType;
    auto destructorIt             = m_vkxml.commands.begin() + handle.second.destructorIndex;
    for ( auto const & destructorParam : destructorIt->params )
    {
      if ( ( destructorParam.type.name != parentType ) && ( destructorParam.type.name != handle.first ) &&
           std::ranges::none_of( destructorIt->params, [&destructorParam]( Param const & pd ) { return pd.len == destructorParam.name; } ) )
      {
        if ( std::ranges::any_of( params, [&destructorParam]( Param const & pd ) { return pd.type.name == destructorParam.type.name; } ) )
        {
          if ( isHandleType( destructorParam.type.name ) )
          {
            assert( destructorParam.type.isValue() );
            arguments += ", static_cast<" + destructorParam.type.name + ">( *" + destructorParam.name + " )";
          }
          else
          {
            assert( destructorParam.type.name == "VkAllocationCallbacks" );
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
            auto structureIt = findByName( m_vkxml.types.structs, param.type.name );
            if ( structureIt != m_vkxml.types.structs.end() )
            {
              auto memberIt = findByType( structureIt->members, destructorParam.type.name );
              if ( memberIt != structureIt->members.end() )
              {
#if !defined( NDEBUG )
                found = true;
#endif
                assert( !param.type.isValue() );
                std::string argument = startLowerCase( stripPrefix( param.name, "p" ) ) + "." + memberIt->name;
                if ( isHandleType( memberIt->type.name ) )
                {
                  argument = "static_cast<" + memberIt->type.name + ">( " + argument + " )";
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
      auto const [enter, leave] = generateProtection( getProtectFromTitle( extension.name ) );
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

std::string VulkanHppGenerator::generateResultAssignment( Command const & command ) const
{
  std::string resultAssignment;
  if ( ( command.returnType.name != "void" ) &&
       !( ( command.returnType.name == "VkResult" ) && ( command.successCodes.size() == 1 ) && command.errorCodes.empty() ) )
  {
    resultAssignment = command.returnType.name + " result_ = ";
  }
  return resultAssignment;
}

std::string VulkanHppGenerator::generateResultCheck( Command const &     command,
                                                     std::string const & className,
                                                     std::string const & classSeparator,
                                                     std::string         commandName,
                                                     bool                enumerating,
                                                     bool                isMultiPipelineCreation,
                                                     bool                raii ) const
{
  std::string resultCheck;
  if ( !command.errorCodes.empty() )
  {
    std::string successCodeList = generateSuccessCodeList( command.successCodes, enumerating );

    if ( std::ranges::find( command.errorCodes, "VK_ERROR_OUT_OF_DATE_KHR" ) != command.errorCodes.end() )
    {
      std::vector<std::string> extendedSuccessCodes = command.successCodes;
      extendedSuccessCodes.push_back( "VK_ERROR_OUT_OF_DATE_KHR" );

      std::string const resultCheckTemplate = R"(
#if defined( VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS )
  ${namespace}detail::resultCheck( result_, ${namespaceString} "::${className}${classSeparator}${commandName}"${extendedSuccessCodeList} );
#else
  ${namespace}detail::resultCheck( result_, ${namespaceString} "::${className}${classSeparator}${commandName}"${successCodeList} );
#endif
 )";

      resultCheck = replaceWithMap( resultCheckTemplate,
                                    { { "className", className },
                                      { "classSeparator", classSeparator },
                                      { "commandName", commandName },
                                      { "extendedSuccessCodeList", generateSuccessCodeList( extendedSuccessCodes, enumerating ) },
                                      { "namespace", raii ? "VULKAN_HPP_NAMESPACE::" : "" },
                                      { "namespaceString", raii ? "VULKAN_HPP_RAII_NAMESPACE_STRING" : "VULKAN_HPP_NAMESPACE_STRING" },
                                      { "successCodeList", successCodeList } } );
    }
    else if ( isMultiPipelineCreation )
    {
      std::string const resultCheckTemplate =
        R"(${namespace}detail::resultCheck( result_, ${namespaceString} "::${className}${classSeparator}${commandName}"${successCodeList}, ${device}, pipelines, allocator.get(), d );)";

      resultCheck = replaceWithMap( resultCheckTemplate,
                                    { { "className", className },
                                      { "classSeparator", classSeparator },
                                      { "device", raii ? "static_cast<VkDevice>( m_device )" : "m_device" },
                                      { "commandName", commandName },
                                      { "namespace", raii ? "VULKAN_HPP_NAMESPACE::" : "" },
                                      { "namespaceString", raii ? "VULKAN_HPP_RAII_NAMESPACE_STRING" : "VULKAN_HPP_NAMESPACE_STRING" },
                                      { "successCodeList", successCodeList } } );
    }
    else
    {
      std::string const resultCheckTemplate =
        R"(${namespace}detail::resultCheck( result_, ${namespaceString} "::${className}${classSeparator}${commandName}"${successCodeList} );)";

      resultCheck = replaceWithMap( resultCheckTemplate,
                                    { { "className", className },
                                      { "classSeparator", classSeparator },
                                      { "commandName", commandName },
                                      { "namespace", raii ? "VULKAN_HPP_NAMESPACE::" : "" },
                                      { "namespaceString", raii ? "VULKAN_HPP_RAII_NAMESPACE_STRING" : "VULKAN_HPP_NAMESPACE_STRING" },
                                      { "successCodeList", successCodeList } } );
    }
  }

  return resultCheck;
}

// Intended only for `enum class Result`!
std::string VulkanHppGenerator::generateResultExceptions() const
{
  std::string const templateString = R"(
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

std::string VulkanHppGenerator::generateReturnDataType( std::map<size_t, VectorParamData> const & vectorParams,
                                                        std::vector<size_t> const &               returnParams,
                                                        bool                                      enumerating,
                                                        std::vector<std::string> const &          dataTypes,
                                                        CommandFlavourFlags                       flavourFlags,
                                                        bool                                      raii ) const
{
  assert( dataTypes.size() == returnParams.size() );

  std::vector<std::string> modifiedDataTypes( returnParams.size() );
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
      switch ( vectorParams.size() )
      {
        case 0:
          assert( !enumerating );
          combinedType = "std::tuple<" + modifiedDataTypes[0] + ", " + modifiedDataTypes[1] + ", " + modifiedDataTypes[2] + ">";
          break;
        case 1:
          assert( enumerating );
          if ( ( vectorParams.begin()->first == returnParams[1] ) && ( vectorParams.begin()->second.lenParam == returnParams[0] ) )
          {
            // start with the enumerated vector, then the simple data
            combinedType = "std::pair<" + modifiedDataTypes[1] + ", " + modifiedDataTypes[2] + ">";
          }
          else
          {
            assert( ( vectorParams.begin()->first == returnParams[2] ) && ( vectorParams.begin()->second.lenParam == returnParams[1] ) );
            // start with the simple data, the the enumerated vector
            combinedType = "std::pair<" + modifiedDataTypes[0] + ", " + modifiedDataTypes[2] + ">";
          }
          break;
        case 2:
          assert( enumerating );
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

std::string VulkanHppGenerator::generateReturnStatement( Command const &     command,
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
  bool const unique = flavourFlags & CommandFlavourFlagBits::unique;

  std::string returnStatement;
  if ( command.returnType.name.starts_with( "Vk" ) )
  {
    if ( ( command.successCodes.size() == 1 ) || ( enumerating && ( command.successCodes.size() == 2 ) ) )
    {
      assert( command.successCodes[0] == "VK_SUCCESS" );
      if ( command.errorCodes.empty() )
      {
        if ( !returnVariable.empty() )
        {
          returnStatement = "return " + returnVariable + ";";
        }
      }
      else
      {
        returnStatement = "return "s + ( raii ? "VULKAN_HPP_NAMESPACE::" : "" ) + "detail::createResultValueType( ";
        if ( returnVariable.empty() )
        {
          assert( !unique );
          returnStatement += "result_ );";
        }
        else if ( unique )
        {
          assert( returnParam != INVALID_INDEX );
          returnStatement += "result_, ";
          if ( dataType.starts_with( "std::" ) )
          {
            returnStatement += "std::move( unique" + startUpperCase( returnVariable ) + " )";
          }
          else
          {
#if !defined( NDEBUG )
            auto typeIt = m_types.find( "Vk" + dataType );
            assert( ( typeIt != m_types.end() ) && ( typeIt->second.category == TypeCategory::Handle ) );
#endif
            returnStatement += returnType + "( " + returnVariable + ", " + generateObjectDeleter( command, initialSkipCount, returnParam ) + " )";
          }
          returnStatement += " );";
        }
        else
        {
          returnStatement += "result_, std::move( " + returnVariable + " ) );";
        }

        if ( std::ranges::find( command.errorCodes, "VK_ERROR_OUT_OF_DATE_KHR" ) != command.errorCodes.end() )
        {
          assert( !unique && !returnVariable.empty() );
          static std::string returnStatementTemplate = R"(
#if defined( VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS )
  return ResultValue<${returnType}>( result_, std::move( ${returnVariable} ) );
#else
  ${returnStatement}
#endif)";

          returnStatement = replaceWithMap( returnStatementTemplate,
                                            { { "returnStatement", returnStatement }, { "returnType", returnType }, { "returnVariable", returnVariable } } );
        }
      }
    }
    else
    {
      if ( returnVariable.empty() )
      {
        assert( !unique );
        returnStatement = "return static_cast<" + stripPrefix( command.returnType.name, "Vk" ) + ">( result_ );";
      }
      else if ( unique )
      {
        assert( returnParam != INVALID_INDEX );
        assert( decoratedReturnType.starts_with( "ResultValue<" ) && decoratedReturnType.ends_with( ">" ) );
        returnStatement = "return " + decoratedReturnType + "( result_, ";
        if ( dataType.starts_with( "std::" ) )
        {
          returnStatement += "std::move( unique" + startUpperCase( returnVariable ) + " )";
        }
        else
        {
          returnStatement +=
            "UniqueHandle<" + dataType + ", Dispatch>( " + returnVariable + ", " + generateObjectDeleter( command, initialSkipCount, returnParam ) + " )";
        }
        returnStatement += " );";
      }
      else
      {
        returnStatement = "return { result_, " + returnVariable + " };";
      }
    }
  }
  else
  {
    assert( !unique );
    if ( returnVariable.empty() )
    {
      if ( command.returnType.name != "void" )
      {
        returnStatement = "return result_;";
      }
    }
    else
    {
      returnStatement = "return " + returnVariable + ";";
    }
  }
  return returnStatement;
}

std::string VulkanHppGenerator::generateReturnType( std::vector<Param> const &                params,
                                                    std::vector<size_t> const &               returnParams,
                                                    std::map<size_t, VectorParamData> const & vectorParams,
                                                    CommandFlavourFlags                       flavourFlags,
                                                    bool                                      raii,
                                                    std::vector<std::string> const &          dataTypes ) const
{
  assert( returnParams.size() == dataTypes.size() );
  bool const chained  = flavourFlags & CommandFlavourFlagBits::chained;
  bool const singular = flavourFlags & CommandFlavourFlagBits::singular;
  bool const unique   = flavourFlags & CommandFlavourFlagBits::unique;
  assert( !( chained && unique ) );
  assert( !( raii && unique ) );

  std::string returnType;
  switch ( returnParams.size() )
  {
    case 0: returnType = "void"; break;
    case 1:
      if ( chained )
      {
        assert( vectorParams.empty() );
        returnType = "StructureChain<X, Y, Z...>";
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
      if ( chained )
      {
        auto vectorIt = vectorParams.find( returnParams[1] );
        if ( vectorIt != vectorParams.end() )
        {
          assert( vectorIt->second.lenParam == returnParams[0] );
          returnType = std::string( "std::vector<StructureChain" ) + ( raii ? "" : ", StructureChainAllocator" ) + ">";
        }
        else if ( isStructureChainAnchor( "Vk" + dataTypes[0] ) && containsByName( m_vkxml.types.externals, dataTypes[1] ) )
        {
          returnType = "std::pair<StructureChain<X, Y, Z...>, " + dataTypes[1] + ">";
        }
        else
        {
          assert( ( containsByName( m_vkxml.types.externals, dataTypes[0] ) || isHandleType( "Vk" + stripPrefix( dataTypes[0], "VULKAN_HPP_NAMESPACE::" ) ) ) &&
                  isStructureChainAnchor( "Vk" + dataTypes[1] ) );
          returnType = "std::pair<" + dataTypes[0] + ", StructureChain<X, Y, Z...>>";
        }
      }
      else if ( vectorParams.contains( returnParams[0] ) )
      {
        std::string firstDataType =
          unique && isHandleType( params[returnParams[0]].type.name ) ? ( "UniqueHandle<" + dataTypes[0] + ", Dispatch>" ) : dataTypes[0];
        if ( vectorParams.contains( returnParams[1] ) )
        {
          std::string secondDataType =
            unique && isHandleType( params[returnParams[1]].type.name ) ? ( "UniqueHandle<" + dataTypes[1] + ", Dispatch>" ) : dataTypes[1];
          if ( singular )
          {
            returnType = "std::pair<" + firstDataType + ", " + secondDataType + ">";
          }
          else
          {
            returnType = "std::pair<std::vector<" + firstDataType + ( raii ? "" : ( ", " + startUpperCase( dataTypes[0] ) + "Allocator" ) ) +
                         ">, std::vector<" + secondDataType + ( raii ? "" : ( ", " + startUpperCase( dataTypes[1] ) + "Allocator" ) ) + ">>";
          }
        }
        else
        {
          if ( singular )
          {
            returnType = "std::pair<" + firstDataType + ", " + dataTypes[1] + ">";
          }
          else
          {
            returnType =
              "std::pair<std::vector<" + firstDataType + ( raii ? "" : ( ", " + startUpperCase( dataTypes[0] ) + "Allocator" ) ) + ">, " + dataTypes[1] + ">";
          }
        }
      }
      else if ( vectorParams.contains( returnParams[1] ) && !singular )
      {
        assert( vectorParams.find( returnParams[1] )->second.lenParam == returnParams[0] );
        std::string dataType = unique && isHandleType( params[returnParams[1]].type.name ) ? ( "UniqueHandle<" + dataTypes[1] + ", Dispatch>" ) : dataTypes[1];
        returnType           = "std::vector<" + dataType + ( raii ? "" : ( ", " + startUpperCase( dataTypes[1] ) + "Allocator" ) ) + ">";
      }
      else
      {
        returnType = "std::pair<" + dataTypes[0] + ", " + dataTypes[1] + ">";
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
        returnType =
          "std::pair<StructureChain<X, Y, Z...>, std::vector<" + dataTypes[2] + ( raii ? "" : ( ", " + startUpperCase( dataTypes[2] ) + "Allocator" ) ) + ">>";
      }
      else
      {
        if ( vectorParams.empty() )
        {
          returnType = "std::tuple<" + dataTypes[0] + ", " + dataTypes[1] + ", " + dataTypes[2] + ">";
        }
        else if ( vectorParams.contains( returnParams[1] ) )
        {
#if !defined( NDEBUG )
          auto vectorIt = vectorParams.find( returnParams[1] );
          assert( ( vectorIt != vectorParams.end() ) && ( vectorIt->second.lenParam == returnParams[0] ) );
#endif
          if ( vectorParams.contains( returnParams[2] ) )
          {
            // returns a pair of two vectors
#if !defined( NDEBUG )
            vectorIt = vectorParams.find( returnParams[2] );
            assert( ( vectorIt != vectorParams.end() ) && ( vectorIt->second.lenParam == returnParams[0] ) );
#endif
            returnType = "std::pair<std::vector<" + dataTypes[1] +
                         ( raii ? "" : ( ", " + startUpperCase( stripPrefix( dataTypes[1], "VULKAN_HPP_NAMESPACE::" ) + "Allocator" ) ) ) + ">, std::vector<" +
                         dataTypes[2] + ( raii ? "" : ( ", " + startUpperCase( stripPrefix( dataTypes[2], "VULKAN_HPP_NAMESPACE::" ) + "Allocator" ) ) ) + ">>";
          }
          else
          {
            // returns a pair of a vector and a value
            returnType = "std::pair<std::vector<" + dataTypes[1] +
                         ( raii ? "" : ( ", " + startUpperCase( stripPrefix( dataTypes[1], "VULKAN_HPP_NAMESPACE::" ) + "Allocator" ) ) ) + ">, " +
                         dataTypes[2] + ">";
          }
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

std::string VulkanHppGenerator::generateReturnVariable( Command const &                           command,
                                                        std::vector<size_t> const &               returnParams,
                                                        std::map<size_t, VectorParamData> const & vectorParams,
                                                        CommandFlavourFlags                       flavourFlags ) const
{
  bool const chained  = flavourFlags & CommandFlavourFlagBits::chained;
  bool const singular = flavourFlags & CommandFlavourFlagBits::singular;

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
          returnVariable = startLowerCase( stripPrefix( vectorMemberByStructure( command.params[returnParams[0]].type.name ).name, "p" ) );
        }
        else
        {
          returnVariable = startLowerCase( stripPrefix( command.params[returnParams[0]].name, "p" ) );
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
        if ( vectorParams.empty() )
        {
          // the return variable is simply named "data_", and holds the multi-return value stuff
          returnVariable = "data_";
        }
        else
        {
#if !defined( NDEBUG )
          auto vectorParamIt = vectorParams.find( returnParams[1] );
#endif
          assert( ( vectorParamIt != vectorParams.end() ) && ( vectorParamIt->second.lenParam == returnParams[0] ) );
          returnVariable = "structureChains";
        }
      }
      else
      {
        auto vectorParamIt = vectorParams.find( returnParams[1] );
        if ( ( vectorParamIt == vectorParams.end() ) || vectorParams.contains( returnParams[0] ) )
        {
          // the return variable is simply named "data_", and holds the multi-return value stuff
          returnVariable = "data_";
        }
        else
        {
          assert( vectorParamIt->second.lenParam == returnParams[0] );
          assert( !singular );
          returnVariable = startLowerCase( stripPrefix( command.params[returnParams[1]].name, "p" ) );
        }
      }
      break;
    case 3:
      assert( !singular );
      returnVariable = "data_";
      break;
    default: assert( false ); break;
  }
  return returnVariable;
}

std::string VulkanHppGenerator::generateSizeCheck( std::vector<std::vector<StructMember>::const_iterator> const & arrayIts,
                                                   std::string const &                                            structName,
                                                   bool                                                           mutualExclusiveLens ) const
{
  std::string sizeCheck;
  if ( 1 < arrayIts.size() )
  {
    static std::string const throwTextTemplate = R"(      if ( ${throwCheck} )
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
  for ( auto const & feature : m_vkxml.features )
  {
    staticAssertions += generateStaticAssertions( feature.require, feature.name, listedStructs );
  }
  for ( auto const & extension : m_extensions )
  {
    staticAssertions += generateStaticAssertions( extension.requireData, extension.name, listedStructs );
  }
  return staticAssertions;
}

template <typename RequireType>
std::string VulkanHppGenerator::generateStaticAssertions( std::vector<RequireType> const & requireTypes,
                                                          std::string const &              title,
                                                          std::set<std::string> &          listedTypes ) const
{
  std::string str;
  for ( auto const & require : requireTypes )
  {
    for ( auto const & type : require.types )
    {
      if ( auto handleIt = m_handles.find( type.name ); handleIt != m_handles.end() )
      {
        if ( listedTypes.insert( handleIt->first ).second )
        {
          std::string const assertionTemplate = R"(
VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::${className} ) == sizeof( Vk${className} ), "handle and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_copy_constructible<VULKAN_HPP_NAMESPACE::${className}>::value, "${className} is not copy_constructible!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::${className}>::value, "${className} is not nothrow_move_constructible!" );
)";
          str += replaceWithMap( assertionTemplate, { { "className", stripPrefix( handleIt->first, "Vk" ) } } );
        }
      }
      else if ( auto structIt = findByName( m_vkxml.types.structs, type.name ); structIt != m_vkxml.types.structs.end() )
      {
        if ( listedTypes.insert( structIt->name ).second )
        {
          std::string const assertionTemplate = R"(
VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::${structureType} ) == sizeof( Vk${structureType} ), "struct and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::${structureType}>::value, "struct wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::${structureType}>::value, "${structureType} is not nothrow_move_constructible!" );
)";
          str += replaceWithMap( assertionTemplate, { { "structureType", stripPrefix( structIt->name, "Vk" ) } } );
        }
      }
      else if ( auto unionIt = findByName( m_vkxml.types.unions, type.name ); unionIt != m_vkxml.types.unions.end() )
      {
        if ( listedTypes.insert( unionIt->name ).second )
        {
          std::string const assertionTemplate = R"(
VULKAN_HPP_STATIC_ASSERT( sizeof( VULKAN_HPP_NAMESPACE::${unionType} ) == sizeof( Vk${unionType} ), "union and wrapper have different size!" );
VULKAN_HPP_STATIC_ASSERT( std::is_standard_layout<VULKAN_HPP_NAMESPACE::${unionType}>::value, "union wrapper is not a standard layout!" );
VULKAN_HPP_STATIC_ASSERT( std::is_nothrow_move_constructible<VULKAN_HPP_NAMESPACE::${unionType}>::value, "${unionType} is not nothrow_move_constructible!" );
)";
          str += replaceWithMap( assertionTemplate, { { "unionType", stripPrefix( unionIt->name, "Vk" ) } } );
        }
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateStruct( TypeStruct const & vkStruct, std::set<std::string> & listedStructs ) const
{
  assert( !listedStructs.contains( vkStruct.name ) );

  std::string str;

  auto subStruct = determineSubStruct( vkStruct );
  if ( subStruct )
  {
    if ( !listedStructs.contains( subStruct->name ) )
    {
      str += generateStruct( *subStruct, listedStructs );
    }
  }

  str += generateStructure( vkStruct, subStruct );

  listedStructs.insert( vkStruct.name );
  return str;
}

std::string VulkanHppGenerator::generateStructCastAssignments( TypeStruct const & vkStruct ) const
{
  std::string castAssignments;
  if ( containsDeprecated( vkStruct.members ) )
  {
    for ( auto const & member : vkStruct.members )
    {
      if ( member.deprecated.empty() && ( member.type.name != "VkStructureType" ) )
      {
        if ( member.type.name.starts_with( "Vk" ) )
        {
          if ( member.type.isValue() )
          {
            castAssignments += member.name + " = static_cast<" + stripPrefix( member.type.name, "Vk" ) + ">( rhs." + member.name + " );\n";
          }
          else
          {
            castAssignments += member.name + " = reinterpret_cast<" + member.type.compose( "Vk" ) + ">( rhs." + member.name + " );\n";
          }
        }
        else
        {
          castAssignments += member.name + " = rhs." + member.name + ";\n";
        }
      }
    }
  }
  else
  {
    castAssignments = "*this = *reinterpret_cast<" + stripPrefix( vkStruct.name, "Vk" ) + " const *>( &rhs );\n";
  }
  assert( castAssignments.ends_with( "\n" ) );
  castAssignments.pop_back();
  return castAssignments;
}

std::string VulkanHppGenerator::generateStructCompareOperators( TypeStruct const & vkStruct ) const
{
  static std::set<std::string> const simpleTypes = { "char",   "double",  "DWORD",    "float",    "HANDLE",  "HINSTANCE", "HMONITOR",
                                                     "HWND",   "int",     "int8_t",   "int16_t",  "int32_t", "int64_t",   "LPCWSTR",
                                                     "size_t", "uint8_t", "uint16_t", "uint32_t", "uint64_t" };
  // two structs are compared by comparing each of the elements
  std::string compareMembers, spaceshipMembers;
  std::string intro             = "";
  bool        nonDefaultCompare = false;
  std::string spaceshipOrdering = containsFloatingPoints( vkStruct.members ) ? "std::partial_ordering" : "std::strong_ordering";
  for ( size_t i = 0; i < vkStruct.members.size(); i++ )
  {
    StructMember const & member = vkStruct.members[i];
    if ( member.deprecated.empty() )
    {
      auto typeIt = m_types.find( member.type.name );
      assert( typeIt != m_types.end() );
      if ( ( typeIt->second.category == TypeCategory::External ) && member.type.postfix.empty() && !simpleTypes.contains( member.type.name ) )
      {
        nonDefaultCompare = true;
        // this type might support operator==() or operator<=>()... that is, use memcmp
        compareMembers += intro + "( memcmp( &" + member.name + ", &rhs." + member.name + ", sizeof( " + member.type.name + " ) ) == 0 )";

        static std::string const spaceshipMemberTemplate =
          R"(      if ( auto cmp = memcmp( &${name}, &rhs.${name}, sizeof( ${type} ) ); cmp != 0 )
        return ( cmp < 0 ) ? ${ordering}::less : ${ordering}::greater;
)";
        spaceshipMembers +=
          replaceWithMap( spaceshipMemberTemplate, { { "name", member.name }, { "ordering", spaceshipOrdering }, { "type", member.type.name } } );
      }
      else if ( member.type.name == "char" && !member.len.empty() )
      {
        // compare null-terminated strings
        nonDefaultCompare = true;
        assert( member.len.size() < 3 );
        if ( member.len.size() == 1 )
        {
          assert( member.len[0] == "null-terminated" );
          if ( member.arraySizes.empty() )
          {
            compareMembers +=
              intro + "( ( " + member.name + " == rhs." + member.name + " ) || ( strcmp( " + member.name + ", rhs." + member.name + " ) == 0 ) )";

            static std::string const spaceshipMemberTemplate =
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

            static std::string const spaceshipMemberTemplate =
              R"(     if ( auto cmp = strcmp( ${name}, rhs.${name} ); cmp != 0 )
          return ( cmp < 0 ) ? ${ordering}::less : ${ordering}::greater;
)";
            spaceshipMembers += replaceWithMap( spaceshipMemberTemplate, { { "name", member.name }, { "ordering", spaceshipOrdering } } );
          }
        }
        else
        {
          assert( member.len[1] == "null-terminated" );
          assert( member.type.prefix.empty() && ( member.type.postfix == "const * const *" ) );
          static std::string const compareMemberTemplate =
            R"(std::equal( ${name}, ${name} + ${count}, rhs.${name}, []( char const * left, char const * right ) { return ( left == right ) || ( strcmp( left, right ) == 0 ); } ))";
          compareMembers += intro + replaceWithMap( compareMemberTemplate, { { "count", member.len[0] }, { "name", member.name } } );

          static std::string const spaceshipMemberTemplate = R"(      for ( size_t i = 0; i < ${count}; ++i )
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
      else if ( !member.arraySizes.empty() && !member.len.empty() )
      {
        nonDefaultCompare = true;

        assert( ( member.arraySizes.size() == 1 ) && ( member.len.size() == 1 ) );
        assert( std::ranges::any_of( vkStruct.members, [&member]( StructMember const & sm ) { return sm.name == member.len[0]; } ) );

        std::string type = member.type.compose( "Vk" );

        static std::string const compareMemberTemplate = R"(( memcmp( ${name}, rhs.${name}, ${count} * sizeof( ${type} ) ) == 0 ))";
        compareMembers += intro + replaceWithMap( compareMemberTemplate, { { "count", member.len[0] }, { "name", member.name }, { "type", type } } );

        static std::string const spaceshipMemberTemplate = R"(      for ( size_t i = 0; i < ${count}; ++i )
      {
        if ( auto cmp = ${name}[i] <=> rhs.${name}[i]; cmp != 0 ) return cmp;
      }
)";
        spaceshipMembers += replaceWithMap( spaceshipMemberTemplate, { { "count", member.len[0] }, { "name", member.name } } );
      }
      else
      {
        // for all others, we use the operator== of that type
        compareMembers += intro + "( " + member.name + " == rhs." + member.name + " )";
        spaceshipMembers += "      if ( auto cmp = " + member.name + " <=> rhs." + member.name + "; cmp != 0 ) return cmp;\n";
      }
      intro = "\n          && ";
    }
  }

  std::string structName = stripPrefix( vkStruct.name, "Vk" );

  std::string compareBody, spaceshipOperator, spaceshipOperatorIf, spaceshipOperatorElse, spaceshipOperatorEndif;
  if ( nonDefaultCompare )
  {
    compareBody = "      return " + compareMembers + ";";

    if ( !containsFuncPointer( vkStruct.name ) )
    {
      static std::string const spaceshipOperatorTemplate =
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
    static std::string const compareBodyTemplate = R"(#if defined( VULKAN_HPP_USE_REFLECT )
      return this->reflect() == rhs.reflect();
#else
      return ${compareMembers};
#endif)";
    compareBody                                  = replaceWithMap( compareBodyTemplate, { { "compareMembers", compareMembers } } );

    if ( !containsFuncPointer( vkStruct.name ) )
    {
      spaceshipOperatorIf    = "#if defined(VULKAN_HPP_HAS_SPACESHIP_OPERATOR)";
      spaceshipOperator      = "auto operator<=>( " + structName + " const & ) const = default;";
      spaceshipOperatorElse  = "#else";
      spaceshipOperatorEndif = "#endif\n";
    }
  }

  static std::string const compareTemplate = R"(
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

std::string VulkanHppGenerator::generateStructConstructors( TypeStruct const & vkStruct ) const
{
  // the constructor with all the elements as arguments, with defaults
  // and the simple copy constructor from the corresponding vulkan structure
  static std::string const constructors = R"(${constexpr}${structName}(${arguments}) VULKAN_HPP_NOEXCEPT
    ${initializers}
    {${ignores}}

    ${constexpr}${structName}( ${structName} const & rhs ) VULKAN_HPP_NOEXCEPT = default;

    ${structName}( Vk${structName} const & rhs ) VULKAN_HPP_NOEXCEPT
      : ${structName}( *reinterpret_cast<${structName} const *>( &rhs ) )
    {}

    ${enhancedConstructors}
)";

  std::vector<std::string> arguments, initializers;
  std::string              ignores;
  for ( auto const & member : vkStruct.members )
  {
    // gather the arguments
    std::string argument = generateStructConstructorArgument( vkStruct.name, member, true );
    if ( !argument.empty() )
    {
      arguments.push_back( argument );
    }

    // gather the initializers; skip members with exactly one legal value
    if ( member.values.empty() )
    {
      initializers.push_back( member.name + "{ " + member.name + "_ }" );
    }
  }

  auto pNextIt = std::ranges::find_if( vkStruct.members, []( StructMember const & member ) { return member.name == "pNext"; } );
  if ( pNextIt != vkStruct.members.end() )
  {
    // add pNext as a last optional argument to the constructor
    arguments.push_back( pNextIt->type.compose( "Vk" ) + " pNext_ = nullptr" );
  }

  std::string str = replaceWithMap( constructors,
                                    { { "arguments", generateList( arguments, "", ", " ) },
                                      { "constexpr", generateConstexprString( vkStruct ) },
                                      { "enhancedConstructors", vkStruct.returnedOnly ? "" : generateStructConstructorsEnhanced( vkStruct ) },
                                      { "structName", stripPrefix( vkStruct.name, "Vk" ) },
                                      { "ignores", ignores },
                                      { "initializers", generateList( initializers, ": ", ", " ) },
                                      { "structName", stripPrefix( vkStruct.name, "Vk" ) } } );
  return str;
}

std::string VulkanHppGenerator::generateStructConstructorsEnhanced( TypeStruct const & vkStruct ) const
{
  // some structs needs some special handling!
  auto const & members = vkStruct.members;
  if ( vkStruct.name == "VkLayerSettingEXT" )
  {
    assert( ( members.size() == 5 ) && ( members[0].name == "pLayerName" ) && ( members[1].name == "pSettingName" ) && ( members[2].name == "type" ) &&
            ( members[3].name == "valueCount" ) && ( members[4].name == "pValues" ) );

    static std::string const byTypeTemplate =
      R"(    LayerSettingEXT( char const * pLayerName_, char const * pSettingName_, LayerSettingTypeEXT type_, ArrayProxyNoTemporaries<const ${type}> const & values_ )
      : pLayerName( pLayerName_ )
      , pSettingName( pSettingName_ )
      , type( type_ )
      , valueCount( static_cast<uint32_t>( values_.size() ) )
      , pValues( values_.data() )
    {
      VULKAN_HPP_ASSERT( isSameType<${type}>(type) );
    })";

    static std::string const constructorTemplate = R"(
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
// NOTE: you need to provide the type because Bool32 and uint32_t are indistinguishable!
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
  else if ( std::ranges::any_of( members, [this, &members]( auto const & md ) { return hasLen( md, members ); } ) )
  {
    // map from len-members to all the array members using that len
    std::map<std::vector<StructMember>::const_iterator, std::vector<std::vector<StructMember>::const_iterator>> lenIts;
    for ( auto mit = members.begin(); mit != members.end(); ++mit )
    {
      if ( hasLen( *mit, members ) )
      {
        std::string lenName = ( mit->altLen == "codeSize / 4" ) ? "codeSize" : mit->len.front();
        auto        lenIt   = findByName( members, lenName );
        lenIts[lenIt].push_back( mit );
      }
    }

    std::vector<std::string> arguments, initializers;
    bool                     arrayListed = false;
    std::string              templateHeader, sizeChecks, copyOps;
    for ( auto mit = members.begin(); mit != members.end(); ++mit )
    {
      // gather the initializers
      if ( mit->name == "pNext" )  // for pNext, we just get the initializer... the argument is added at the end
      {
        initializers.push_back( "pNext( pNext_ )" );
      }
      else if ( mit->values.empty() )  // skip constant members
      {
        auto litit = lenIts.find( mit );
        if ( litit != lenIts.end() )
        {
          // len arguments just have an initalizer, from the array size
          bool mutualExclusiveLens = structsWithMutualExclusiveArrays.contains( vkStruct.name );
          initializers.push_back( mit->name + "( " + generateLenInitializer( mit, litit, mutualExclusiveLens ) + " )" );
          sizeChecks += generateSizeCheck( litit->second, stripPrefix( vkStruct.name, "Vk" ), mutualExclusiveLens );
        }
        else if ( hasLen( *mit, members ) )
        {
          assert( mit->type.isPointer() || !mit->arraySizes.empty() );
          std::string argumentName = ( mit->type.isPointer() ? startLowerCase( stripPrefix( mit->name, "p" ) ) : mit->name ) + "_";

          std::string argumentType = mit->type.compose( "Vk" );
          if ( mit->type.isPointer() )
          {
            argumentType = trimEnd( stripPostfix( argumentType, "*" ) );
          }
          if ( ( mit->type.name == "void" ) && ( argumentType.find( '*' ) == std::string::npos ) )
          {
            // the argument after stripping one pointer is just void
            assert( templateHeader.empty() );
            templateHeader = "    template <typename T>\n";

            size_t const pos = argumentType.find( "void" );
            assert( pos != std::string::npos );
            argumentType.replace( pos, strlen( "void" ), "T" );
          }

          if ( mit->len[0] == "null-terminated" )
          {
            assert( ( mit->type.name == "char" ) && ( mit->arraySizes.size() == 1 ) );
            arguments.push_back( "std::string const & " + argumentName );
          }
          else if ( mit->arraySizes.empty() )
          {
            arguments.push_back( "ArrayProxyNoTemporaries<" + argumentType + "> const & " + argumentName );
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
            assert( mit->deprecated.empty() && ( mit->arraySizes.size() == 1 ) );
            if ( mit->len[0] == "null-terminated" )
            {
              static std::string const strcpyTemplate = R"(
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
              static std::string const memcpyTemplate = R"(
    VULKAN_HPP_ASSERT( ${memberName}_.size() < ${arraySize} );
    memcpy( ${memberName}, ${memberName}_.data(), ${len} * sizeof( ${argumentType} ) );)";

              std::string arraySizeExpression = ( mit->len[0] == "null-terminated" )
                                                ? ( "std::min<size_t>( " + mit->name + "_.size(), " + mit->arraySizes[0] + " )" )
                                                : ( mit->len[0] + " * sizeof( " + argumentType + " )" );
              copyOps += replaceWithMap(
                memcpyTemplate,
                { { "arraySize", mit->arraySizes[0] }, { "argumentType", argumentType }, { "len", mit->len[0] }, { "memberName", mit->name } } );
            }
          }
        }
        else
        {
          std::string argument = generateStructConstructorArgument( vkStruct.name, *mit, arrayListed );
          if ( !argument.empty() )
          {
            arguments.push_back( argument );
          }
          initializers.push_back( mit->name + "( " + mit->name + "_ )" );
        }
      }
    }

    auto pNextIt = std::ranges::find_if( members, []( StructMember const & member ) { return member.name == "pNext"; } );
    if ( pNextIt != members.end() )
    {
      // add pNext as a last optional argument to the constructor
      arguments.push_back( pNextIt->type.compose( "Vk", "VULKAN_HPP_NAMESPACE" ) + " pNext_ = nullptr" );
    }

    static std::string const constructorTemplate = R"(
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
${templateHeader}    ${structName}( ${arguments} )
    ${initializers}
    {
      ${sizeChecks}
      ${copyOps}
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

    return replaceWithMap( constructorTemplate,
                           { { "arguments", generateList( arguments, "", ", " ) },
                             { "copyOps", copyOps },
                             { "initializers", generateList( initializers, ": ", ", " ) },
                             { "sizeChecks", sizeChecks },
                             { "structName", stripPrefix( vkStruct.name, "Vk" ) },
                             { "templateHeader", templateHeader } } );
  }
  return "";
}

std::string VulkanHppGenerator::generateStructConstructorArgument( std::string const & structName, StructMember const & member, bool withDefault ) const
{
  // skip members 'pNext' and members with a specified value, as they are never explicitly set
  std::string str;
  if ( ( member.name != "pNext" ) && member.values.empty() )
  {
    if ( member.type.name.starts_with( "PFN_vk" ) )
    {
      str += "PFN_" + stripPrefix( member.type.name, "PFN_vk" ) + " ";
    }
    else if ( member.arraySizes.empty() )
    {
      str += member.type.compose( "Vk" ) + " ";
    }
    else
    {
      str += generateStandardArray( member.type.compose( "Vk" ), member.arraySizes ) + " const & ";
    }
    str += member.name + "_";

    if ( withDefault )
    {
      str += " = ";
      auto enumIt = m_enums.find( member.type.name );
      if ( enumIt != m_enums.end() && member.type.postfix.empty() )
      {
        str += generateEnumInitializer( member.type, member.arraySizes, enumIt->second.values, enumIt->second.isBitmask );
      }
      else if ( structName.starts_with( "VkRayTracingShaderGroupCreateInfo" ) && member.name.ends_with( "Shader" ) )
      {
        // the shader member of VkRayTracingShaderGroupCreateInfoNV and VkRayTracingShaderGroupCreateInfoKHR can be initialized with VK_SHADER_UNUSED_NV or
        // VK_SHADER_UNUSD_KHR
        assert( structName.ends_with( "KHR" ) || structName.ends_with( "NV" ) );
        assert( member.type.name == "uint32_t" );
        str += generateTaggedCamelCase( structName.ends_with( "KHR" ) ? "VK_SHADER_UNUSED_KHR" : "VK_SHADER_UNUSED_NV" );
      }
      else
      {
        // if there's no default value, it can be initialized with just {}
        str += "{}";
      }
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateStructHashStructure( TypeStruct const & vkStruct, std::set<std::string> & listedStructs ) const
{
  assert( !listedStructs.contains( vkStruct.name ) );

  std::string str;
  for ( auto const & member : vkStruct.members )
  {
    if ( auto structIt = findByNameOrAlias( m_vkxml.types.structs, member.type.name );
         ( structIt != m_vkxml.types.structs.end() ) && ( vkStruct.name != member.type.name ) && !listedStructs.contains( structIt->name ) )
    {
      str += generateStructHashStructure( *structIt, listedStructs );
    }
  }

  if ( !containsUnion( vkStruct.name ) )
  {
    static std::string const hashTemplate = R"(
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

    auto [enter, leave] = generateProtection( getProtectFromType( vkStruct.name ) );

    std::string structureType = stripPrefix( vkStruct.name, "Vk" );
    std::string structureName = startLowerCase( structureType );
    str += replaceWithMap( hashTemplate,
                           { { "enter", enter },
                             { "hashSum", generateStructHashSum( vkStruct ) },
                             { "leave", leave },
                             { "structureName", structureName },
                             { "structureType", structureType } } );
  }
  listedStructs.insert( vkStruct.name );
  return str;
}

std::string VulkanHppGenerator::generateStructHashStructures() const
{
  std::string const hashesTemplate = R"(
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

  std::set<std::string> listedStructs;  // some structs are used in multiple features/extensions, so we need to make sure we only list them once
  std::string           hashes;
  for ( auto const & feature : m_vkxml.features )
  {
    std::string byFeatureHashes;
    for ( auto const & require : feature.require )
    {
      for ( auto const & type : require.types )
      {
        auto vkStructIt = findByNameOrAlias( m_vkxml.types.structs, type.name );
        if ( vkStructIt != m_vkxml.types.structs.end() && !listedStructs.contains( vkStructIt->name ) && isTypeUsed( vkStructIt->name ) )
        {
          byFeatureHashes += generateStructHashStructure( *vkStructIt, listedStructs );
        }
      }
    }
    hashes += addTitleAndProtection( feature.name, byFeatureHashes );
  }
  for ( auto const & extension : m_extensions )
  {
    std::string byExtensionHashes;
    for ( auto const & require : extension.requireData )
    {
      for ( auto const & type : require.types )
      {
        auto vkStructIt = findByNameOrAlias( m_vkxml.types.structs, type.name );
        if ( vkStructIt != m_vkxml.types.structs.end() && !listedStructs.contains( vkStructIt->name ) && isTypeUsed( vkStructIt->name ) )
        {
          byExtensionHashes += generateStructHashStructure( *vkStructIt, listedStructs );
        }
      }
    }
    hashes += addTitleAndProtection( extension.name, byExtensionHashes );
  }
  return replaceWithMap( hashesTemplate, { { "hashes", hashes } } );
}

std::string VulkanHppGenerator::generateStructHashSum( TypeStruct const & vkStruct ) const
{
  std::string hashSum;
  std::string structureName = startLowerCase( stripPrefix( vkStruct.name, "Vk" ) );
  for ( auto const & member : vkStruct.members )
  {
    if ( member.deprecated.empty() )
    {
      if ( !member.arraySizes.empty() )
      {
        assert( member.arraySizes.size() < 3 );
        hashSum += "    for ( size_t i = 0; i < " + member.arraySizes[0] + "; ++i )\n";
        hashSum += "    {\n";
        if ( member.arraySizes.size() == 1 )
        {
          hashSum += "      VULKAN_HPP_HASH_COMBINE( seed, " + structureName + "." + member.name + "[i] );\n";
        }
        else
        {
          hashSum += "      for ( size_t j=0; j < " + member.arraySizes[1] + "; ++j )\n";
          hashSum += "      {\n";
          hashSum += "        VULKAN_HPP_HASH_COMBINE( seed, " + structureName + "." + member.name + "[i][j] );\n";
          hashSum += "      }\n";
        }
        hashSum += "    }\n";
      }
      else if ( member.type.name == "char" && !member.len.empty() )
      {
        assert( member.len.size() < 3 );
        if ( member.len.size() == 1 )
        {
          assert( member.len[0] == "null-terminated" );
          hashSum += "    for ( char const * p = " + structureName + "." + member.name + "; *p != '\\0'; ++p )\n";
          hashSum += "    {\n";
          hashSum += "      VULKAN_HPP_HASH_COMBINE( seed, *p );\n";
          hashSum += "    }\n";
        }
        else
        {
          assert( member.len[1] == "null-terminated" );
          hashSum += "    for ( size_t i = 0; i < " + structureName + "." + member.len[0] + "; ++i )\n";
          hashSum += "    {\n";
          hashSum += "        for ( char const * p = " + structureName + "." + member.name + "[i]; *p != '\\0'; ++p )\n";
          hashSum += "        {\n";
          hashSum += "          VULKAN_HPP_HASH_COMBINE( seed, *p );\n";
          hashSum += "        }\n";
          hashSum += "    }\n";
        }
      }
      else
      {
        hashSum += "    VULKAN_HPP_HASH_COMBINE( seed, " + structureName + "." + member.name + " );\n";
      }
    }
  }
  assert( !hashSum.empty() );
  return hashSum.substr( 0, hashSum.size() - 1 );
}

std::string VulkanHppGenerator::generateStructsAndUnions() const
{
  std::string const structsTemplate = R"(
  //===============
  //=== STRUCTS ===
  //===============

${structs}
)";

  std::set<std::string> listedTypes;  // some structs/unions are used in multiple features/extensions, so we need to make sure we only list them once
  std::string           structs;
  for ( auto const & feature : m_vkxml.features )
  {
    std::string byFeatureTypes;
    for ( auto const & require : feature.require )
    {
      for ( auto const & type : require.types )
      {
        if ( auto structIt = findByNameOrAlias( m_vkxml.types.structs, type.name );
             ( structIt != m_vkxml.types.structs.end() ) && !listedTypes.contains( structIt->name ) && isTypeUsed( structIt->name ) )
        {
          byFeatureTypes += generateStruct( *structIt, listedTypes );
        }
        else if ( auto unionIt = findByNameOrAlias( m_vkxml.types.unions, type.name );
                  ( unionIt != m_vkxml.types.unions.end() ) && !listedTypes.contains( unionIt->name ) && isTypeUsed( unionIt->name ) )
        {
          byFeatureTypes += generateUnion( *unionIt );
          listedTypes.insert( unionIt->name );
        }
      }
    }
    structs += addTitleAndProtection( feature.name, byFeatureTypes );
  }
  for ( auto const & extension : m_extensions )
  {
    std::string byExtensionTypes;
    for ( auto const & require : extension.requireData )
    {
      for ( auto const & type : require.types )
      {
        if ( auto structIt = findByNameOrAlias( m_vkxml.types.structs, type.name );
             ( structIt != m_vkxml.types.structs.end() ) && !listedTypes.contains( structIt->name ) && isTypeUsed( structIt->name ) )
        {
          byExtensionTypes += generateStruct( *structIt, listedTypes );
        }
        else if ( auto unionIt = findByNameOrAlias( m_vkxml.types.unions, type.name );
                  ( unionIt != m_vkxml.types.unions.end() ) && !listedTypes.contains( unionIt->name ) && isTypeUsed( unionIt->name ) )
        {
          byExtensionTypes += generateUnion( *unionIt );
          listedTypes.insert( unionIt->name );
        }
      }
    }
    structs += addTitleAndProtection( extension.name, byExtensionTypes );
  }

  return replaceWithMap( structsTemplate, { { "structs", structs } } );
}

std::string VulkanHppGenerator::generateStructure( TypeStruct const & vkStruct, TypeStruct const * subStruct ) const
{
  auto const [enter, leave] = generateProtection( getProtectFromType( vkStruct.name ) );

  std::string str = "\n" + enter;

  std::string              constructorsAndSetters;
  static std::string const constructorsTemplate = R"(
#if !defined( VULKAN_HPP_NO_CONSTRUCTORS ) && !defined( VULKAN_HPP_NO_STRUCT_CONSTRUCTORS )
${pushIgnored}${constructors}
${subConstructors}
${deprecatedConstructors}
    ${structName} & operator=( ${structName} const & rhs ) VULKAN_HPP_NOEXCEPT = default;${popIgnored}
#endif /*VULKAN_HPP_NO_CONSTRUCTORS*/

    ${structName} & operator=( Vk${structName} const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      ${castAssignments}
      return *this;
    }
)";

  std::string pushIgnored, popIgnored;
  if ( containsDeprecated( vkStruct.members ) )
  {
    pushIgnored = R"(
#if defined( _MSC_VER )
#  pragma warning( push )
#  pragma warning( disable : 4996 ) // 'function': was declared deprecated
#elif defined( __clang__ )
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined( __GNUC__ )
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#else
// unknown compiler... just ignore the warnings for yourselves ;)
#endif

)";
    popIgnored  = R"(

#if defined( _MSC_VER )
#  pragma warning( pop )
#elif defined( __clang__ )
#  pragma clang diagnostic pop
#elif defined( __GNUC__ )
#  pragma GCC diagnostic pop
#else
// unknown compiler... just ignore the warnings for yourselves ;)
#endif
)";
  }

  constructorsAndSetters = replaceWithMap( constructorsTemplate,
                                           { { "castAssignments", generateStructCastAssignments( vkStruct ) },
                                             { "constructors", generateStructConstructors( vkStruct ) },
                                             { "deprecatedConstructors", generateDeprecatedConstructors( vkStruct.name ) },
                                             { "popIgnored", popIgnored },
                                             { "pushIgnored", pushIgnored },
                                             { "structName", stripPrefix( vkStruct.name, "Vk" ) },
                                             { "subConstructors", generateStructSubConstructor( vkStruct, subStruct ) } } );

  if ( !vkStruct.returnedOnly )
  {
    // only structs that are not returnedOnly get setters!
    constructorsAndSetters += "\n#if !defined( VULKAN_HPP_NO_SETTERS ) && !defined( VULKAN_HPP_NO_STRUCT_SETTERS )";
    for ( size_t i = 0; i < vkStruct.members.size(); i++ )
    {
      constructorsAndSetters += generateStructSetter( vkStruct, i );
    }
    constructorsAndSetters += generateDeprecatedStructSetters( vkStruct.name );
    constructorsAndSetters += "#endif /*VULKAN_HPP_NO_SETTERS*/\n";
  }

  std::string structureType = stripPrefix( vkStruct.name, "Vk" );
  // the member variables
  std::string membersString, memberNames, memberTypes, sTypeValue;
  std::tie( membersString, memberNames, memberTypes, sTypeValue ) = generateStructMembers( vkStruct );

  // use reflection only if VULKAN_HPP_USE_REFLECT is defined
  static std::string const reflectTemplate = R"(
#if defined( VULKAN_HPP_USE_REFLECT )
    std::tuple<${memberTypes}> reflect() const VULKAN_HPP_NOEXCEPT
    {
      return std::tie( ${memberNames} );
    }
#endif
)";
  std::string              reflect         = replaceWithMap( reflectTemplate, { { "memberNames", memberNames }, { "memberTypes", memberTypes } } );

  // operator==() and operator!=()
  // only structs without a union as a member can have a meaningfull == and != operation; we filter them out
  std::string compareOperators;
  if ( !containsUnion( vkStruct.name ) )
  {
    compareOperators += generateStructCompareOperators( vkStruct );
  }

  static std::string const structureTemplate =
    R"(  // wrapper struct for struct Vk${structureType}, see https://registry.khronos.org/vulkan/specs/latest/man/html/Vk${structureType}.html
  struct ${structureType}
  {
    using NativeType = Vk${structureType};

${allowDuplicate}
${typeValue}
${constructorsAndSetters}

    operator Vk${structureType} const &() const VULKAN_HPP_NOEXCEPT
    {
      return *reinterpret_cast<Vk${structureType} const *>( this );
    }

    operator Vk${structureType} &() VULKAN_HPP_NOEXCEPT
    {
      return *reinterpret_cast<Vk${structureType} *>( this );
    }

    operator Vk${structureType} const *() const VULKAN_HPP_NOEXCEPT
    {
      return reinterpret_cast<Vk${structureType} const *>( this );
    }

    operator Vk${structureType} *() VULKAN_HPP_NOEXCEPT
    {
      return reinterpret_cast<Vk${structureType}*>( this );
    }
${reflect}
${compareOperators}
    public:
${members}
  };

#if 20 <= VULKAN_HPP_CPP_VERSION
  template <>
  struct CppType<Vk${structureType}>
  {
    using Type = ${structureType};
  };
#endif
)";

  std::string allowDuplicate, typeValue;
  if ( !sTypeValue.empty() )
  {
    allowDuplicate = std::string( "    static bool const allowDuplicate = " ) + ( vkStruct.allowDuplicate ? "true;" : "false;" );
    typeValue      = "    static VULKAN_HPP_CONST_OR_CONSTEXPR StructureType structureType = StructureType::" + sTypeValue + ";\n";
  }
  str += replaceWithMap( structureTemplate,
                         { { "allowDuplicate", allowDuplicate },
                           { "constructorsAndSetters", constructorsAndSetters },
                           { "compareOperators", compareOperators },
                           { "members", membersString },
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

  for ( auto const & alias : vkStruct.aliases )
  {
    str += "  using " + stripPrefix( alias.first, "Vk" ) + " = " + structureType + ";\n";
  }

  str += leave;
  return str;
}

std::string VulkanHppGenerator::generateStructExtendsStructs() const
{
  std::string const structExtendsTemplate = R"(
  //=======================
  //=== STRUCTS EXTENDS ===
  //=======================

${structExtends})";

  std::string           structExtends;
  std::set<std::string> listedStructs;
  for ( auto const & feature : m_vkxml.features )
  {
    structExtends += generateStructExtendsStructs( feature.require, listedStructs, feature.name );
  }
  for ( auto const & extension : m_extensions )
  {
    structExtends += generateStructExtendsStructs( extension.requireData, listedStructs, extension.name );
  }

  return replaceWithMap( structExtendsTemplate, { { "structExtends", structExtends } } );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateStructExtendsStructs( std::vector<RequireType> const & requireTypes,
                                                              std::set<std::string> &          listedStructs,
                                                              std::string const &              title ) const
{
  std::string str;
  for ( auto const & require : requireTypes )
  {
    for ( auto const & type : require.types )
    {
      if ( auto structIt = findByName( m_vkxml.types.structs, type.name ); structIt != m_vkxml.types.structs.end() )
      {
        if ( listedStructs.insert( structIt->name ).second )
        {
          auto const [enter, leave] = generateProtection( getProtectFromTitle( title ) );

          // append all allowed structure chains
          for ( auto extend : structIt->structExtends )
          {
            auto typeIt = m_types.find( extend );
            assert( typeIt != m_types.end() );
            if ( isSupported( typeIt->second.requiredBy ) )
            {
              auto extendsIt                  = findByNameOrAlias( m_vkxml.types.structs, extend );
              auto const [subEnter, subLeave] = generateProtection( getProtectFromType( extendsIt->name ) );

              if ( enter != subEnter )
              {
                str += subEnter;
              }

              str += "  template <> struct StructExtends<" + stripPrefix( structIt->name, "Vk" ) + ", " + stripPrefix( extend, "Vk" ) +
                     ">{ enum { value = true }; };\n";

              if ( leave != subLeave )
              {
                str += subLeave;
              }
            }
          }
        }
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateStructForwardDeclarations() const
{
  std::string const fowardDeclarationsTemplate = R"(
  //===================================
  //=== STRUCT forward declarations ===
  //===================================

${forwardDeclarations}
)";

  std::string           forwardDeclarations;
  std::set<std::string> listedStructs;
  for ( auto const & feature : m_vkxml.features )
  {
    forwardDeclarations += generateStructForwardDeclarations( feature.require, feature.name, listedStructs );
  }
  for ( auto const & extension : m_extensions )
  {
    forwardDeclarations += generateStructForwardDeclarations( extension.requireData, extension.name, listedStructs );
  }

  return replaceWithMap( fowardDeclarationsTemplate, { { "forwardDeclarations", forwardDeclarations } } );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateStructForwardDeclarations( std::vector<RequireType> const & requireTypes,
                                                                   std::string const &              title,
                                                                   std::set<std::string> &          listedStructs ) const
{
  std::string str;
  for ( auto const & require : requireTypes )
  {
    for ( auto const & type : require.types )
    {
      if ( auto structIt = findByName( m_vkxml.types.structs, type.name ); structIt != m_vkxml.types.structs.end() )
      {
        if ( listedStructs.insert( structIt->name ).second )
        {
          std::string structureType = stripPrefix( structIt->name, "Vk" );
          str += "  struct " + structureType + ";\n";
          for ( auto const & alias : structIt->aliases )
          {
            str += "  using " + stripPrefix( alias.first, "Vk" ) + " = " + structureType + ";\n";
          }
        }
      }
      else if ( auto unionIt = findByName( m_vkxml.types.unions, type.name ); unionIt != m_vkxml.types.unions.end() )
      {
        if ( listedStructs.insert( unionIt->name ).second )
        {
          std::string unionType = stripPrefix( unionIt->name, "Vk" );
          str += "  union " + unionType + ";\n";

          for ( auto const & alias : unionIt->aliases )
          {
            str += "  using " + stripPrefix( alias.first, "Vk" ) + " = " + unionType + ";\n";
          }
        }
      }
    }
  }
  return addTitleAndProtection( title, str );
}

std::tuple<std::string, std::string, std::string, std::string> VulkanHppGenerator::generateStructMembers( TypeStruct const & vkStruct ) const
{
  std::string members, memberNames, memberTypes, sTypeValue;
  for ( auto const & member : vkStruct.members )
  {
    members += "    ";

    if ( !member.deprecated.empty() )
    {
      members += "VULKAN_HPP_DEPRECATED( \"" + member.deprecated + "\" ) ";
    }

    std::string type;
    if ( member.bitCount && member.type.name.starts_with( "Vk" ) )
    {
      assert( member.type.prefix.empty() && member.type.postfix.empty() );  // never encounterd a different case
      type = member.type.name;
    }
    else if ( member.type.name.starts_with( "PFN_vk" ) )
    {
      assert( member.type.prefix.empty() && member.type.postfix.empty() );  // never encounterd a different case
      type = "PFN_" + stripPrefix( member.type.name, "PFN_vk" );
    }
    else if ( member.arraySizes.empty() )
    {
      type = member.type.compose( "Vk" );
    }
    else
    {
      assert( member.type.prefix.empty() && member.type.postfix.empty() );
      type = generateStandardArrayWrapper( member.type.compose( "Vk" ), member.arraySizes );
    }
    members += type + " " + member.name;
    if ( member.deprecated.empty() && !member.values.empty() )
    {
      // special handling for members with legal value: use it as the default
      members += " = ";
      if ( member.type.name == "uint32_t" )
      {
        members += member.values;
      }
      else
      {
        auto enumIt = m_enums.find( member.type.name );
        assert( enumIt != m_enums.end() );
        assert( containsByNameOrAlias( enumIt->second.values, member.values ) );
        std::string valueName = generateEnumValueName( enumIt->first, member.values, enumIt->second.isBitmask );
        members += stripPrefix( member.type.name, "Vk" ) + "::" + valueName;
        if ( member.name == "sType" )
        {
          sTypeValue = valueName;
        }
      }
    }
    else
    {
      // when we don't have any default initialization value, everything can be initialized by just '{}'
      assert( member.arraySizes.empty() || !member.bitCount );
      if ( member.bitCount )
      {
        assert( member.deprecated.empty() );
        members += " : " + std::to_string( member.bitCount );  // except for bitfield members, where no default member initialization
                                                               // is supported (up to C++20)
      }
      else
      {
        members += " = ";
        auto enumIt = m_enums.find( member.type.name );
        if ( member.arraySizes.empty() && ( enumIt != m_enums.end() ) && member.type.postfix.empty() )
        {
          members += generateEnumInitializer( member.type, member.arraySizes, enumIt->second.values, enumIt->second.isBitmask );
        }
        else
        {
          if ( vkStruct.name.starts_with( "VkRayTracingShaderGroupCreateInfo" ) && member.name.ends_with( "Shader" ) )
          {
            // the shader member of VkRayTracingShaderGroupCreateInfoNV and VkRayTracingShaderGroupCreateInfoKHR can be initialized with VK_SHADER_UNUSED_NV
            // or VK_SHADER_UNUSD_KHR
            assert( vkStruct.name.ends_with( "KHR" ) || vkStruct.name.ends_with( "NV" ) );
            assert( member.type.name == "uint32_t" );
            members += generateTaggedCamelCase( vkStruct.name.ends_with( "KHR" ) ? "VK_SHADER_UNUSED_KHR" : "VK_SHADER_UNUSED_NV" );
          }
          else
          {
            members += "{}";
          }
        }
      }
    }
    members += ";\n";

    if ( member.deprecated.empty() )
    {
      memberNames += member.name + ", ";
      memberTypes += type + " const &, ";
    }
  }
  return std::make_tuple( members, stripPostfix( memberNames, ", " ), stripPostfix( memberTypes, ", " ), sTypeValue );
}

std::string VulkanHppGenerator::generateStructSetter( TypeStruct const & vkStruct, size_t index ) const
{
  std::string          str;
  StructMember const & member = vkStruct.members[index];
  if ( member.type.name != "VkStructureType" )  // filter out StructureType, which is supposed to be immutable !
  {
    static std::string const templateString = R"(
    ${deprecated}${constexpr}${structureName} & set${MemberName}( ${memberType} ${reference}${memberName}_ ) & VULKAN_HPP_NOEXCEPT
    {
      ${assignment};
      return *this;
    }

    ${deprecated}${constexpr}${structureName} && set${MemberName}( ${memberType} ${reference}${memberName}_ ) && VULKAN_HPP_NOEXCEPT
    {
      ${assignment};
      return std::move( *this );
    }
)";

    std::string memberType =
      member.type.name.starts_with( "PFN_vk" )
        ? "PFN_" + stripPrefix( member.type.name, "PFN_vk" )
        : ( member.arraySizes.empty() ? member.type.compose( "Vk" ) : generateStandardArray( member.type.compose( "Vk" ), member.arraySizes ) );
    bool const  isReinterpretation = member.bitCount && member.type.name.starts_with( "Vk" );
    std::string assignment;
    if ( member.deprecated.empty() )
    {
      if ( isReinterpretation )
      {
        assignment = member.name + " = " + "*reinterpret_cast<" + member.type.name + "*>(&" + member.name + "_)";
      }
      else
      {
        assignment = member.name + " = " + member.name + "_";
      }
    }
    else
    {
      assignment = "detail::ignore( " + member.name + "_ )";
    }

    str += replaceWithMap( templateString,
                           { { "assignment", assignment },
                             { "constexpr", isReinterpretation ? "" : "VULKAN_HPP_CONSTEXPR_14 " },
                             { "deprecated", member.deprecated.empty() ? "" : "VULKAN_HPP_DEPRECATED( \"" + member.deprecated + "\" ) " },
                             { "memberName", member.name },
                             { "MemberName", startUpperCase( member.name ) },
                             { "memberType", memberType },
                             { "reference",
                               ( member.type.postfix.empty() &&
                                 ( containsByName( m_vkxml.types.structs, member.type.name ) || containsByName( m_vkxml.types.unions, member.type.name ) ) )
                                 ? "const & "
                                 : "" },
                             { "structureName", stripPrefix( vkStruct.name, "Vk" ) } } );

    if ( hasLen( member, vkStruct.members ) )
    {
      assert( member.type.isPointer() || !member.arraySizes.empty() );
      std::string arrayName = member.type.isPointer() ? startLowerCase( stripPrefix( member.name, "p" ) ) : member.name;

      if ( member.len[0] == "null-terminated" )
      {
        assert( member.deprecated.empty() && member.altLen.empty() && ( member.len.size() == 1 ) && ( member.arraySizes.size() == 1 ) &&
                ( member.type.name == "char" ) );

        static std::string const setStringTemplate = R"(
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
                                 { "structureName", stripPrefix( vkStruct.name, "Vk" ) } } );
      }
      else if ( ( vkStruct.name == "VkLayerSettingEXT" ) && ( index == 4 ) )
      {
        // VkLayerSettingEXT::pValues needs some special handling!
        assert( member.deprecated.empty() && ( member.name == "pValues" ) );
        static std::string const byTypeTemplate =
          R"(    LayerSettingEXT & setValues( ArrayProxyNoTemporaries<const ${type}> const & values_ ) VULKAN_HPP_NOEXCEPT
    {
      valueCount = static_cast<uint32_t>( values_.size() );
      pValues    = values_.data();
      return *this;
    })";

        static std::string const setArrayTemplate = R"(
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
        assert( ( member.altLen == "codeSize / 4" ) || containsByName( vkStruct.members, member.len[0] ) );
        std::string lenName, lenValue;
        if ( member.altLen == "codeSize / 4" )
        {
          lenName  = "codeSize";
          lenValue = arrayName + "_.size() * 4";
        }
        else
        {
          lenName  = member.len[0];
          lenValue = arrayName + "_.size()";
        }

        if ( member.type.isPointer() )
        {
          assert( memberType.back() == '*' );
          memberType = trimEnd( stripPostfix( memberType, "*" ) );
        }

        std::string templateHeader;
        if ( ( member.type.name == "void" ) && ( memberType.find( '*' ) == std::string::npos ) )
        {
          assert( templateHeader.empty() );
          templateHeader = "template <typename T>\n    ";

          size_t const pos = memberType.find( "void" );
          assert( pos != std::string::npos );
          memberType.replace( pos, strlen( "void" ), "T" );

          lenValue += " * sizeof(T)";
        }

        auto lenMember = findByName( vkStruct.members, lenName );
        assert( lenMember != vkStruct.members.end() && lenMember->type.prefix.empty() && lenMember->type.postfix.empty() );
        if ( lenMember->type.name != "size_t" )
        {
          lenValue = "static_cast<" + lenMember->type.name + ">( " + lenValue + " )";
        }

        if ( !member.arraySizes.empty() )
        {
          assert( member.arraySizes.size() == 1 );
          lenValue = "(std::min)( " + lenValue + ", " + member.arraySizes[0] + " )";
        }

        if ( member.type.isPointer() )
        {
          std::string functionBody;
          if ( member.deprecated.empty() )
          {
            static std::string const functionBodyTemplate = R"(      ${lenName} = ${lenValue};
      ${memberName} = ${arrayName}_.data();)";

            functionBody = replaceWithMap( functionBodyTemplate,
                                           { { "arrayName", arrayName }, { "lenName", lenName }, { "lenValue", lenValue }, { "memberName", member.name } } );
          }
          else
          {
            functionBody = "detail::ignore( " + arrayName + "_ );";
          }

          static std::string const setArrayTemplate = R"(
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
    ${deprecated}${templateHeader}${structureName} & set${ArrayName}( ArrayProxyNoTemporaries<${memberType}> const & ${arrayName}_ ) VULKAN_HPP_NOEXCEPT
    {
      ${functionBody}
      return *this;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

          str += replaceWithMap( setArrayTemplate,
                                 { { "arrayName", arrayName },
                                   { "ArrayName", startUpperCase( arrayName ) },
                                   { "deprecated", member.deprecated.empty() ? "" : "VULKAN_HPP_DEPRECATED( \"" + member.deprecated + "\" ) " },
                                   { "functionBody", functionBody },
                                   { "memberType", memberType },
                                   { "structureName", stripPrefix( vkStruct.name, "Vk" ) },
                                   { "templateHeader", templateHeader } } );
        }
        else
        {
          assert( member.arraySizes.size() == 1 );

          static std::string const setArrayTemplate = R"(
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
                                   { "memberType", member.type.compose( "Vk", "VULKAN_HPP_NAMESPACE" ) },
                                   { "structureName", stripPrefix( vkStruct.name, "Vk" ) },
                                   { "templateHeader", templateHeader } } );
        }
      }
    }
  }
  return str;
}

std::string VulkanHppGenerator::generateStructSubConstructor( TypeStruct const & vkStruct, TypeStruct const * subStruct ) const
{
  if ( subStruct )
  {
    std::string subStructArgumentName = startLowerCase( stripPrefix( subStruct->name, "Vk" ) );

    std::string subCopies;
    bool        firstArgument = true;
    for ( size_t i = 0; i < subStruct->members.size(); i++ )
    {
      assert( vkStruct.members[i].arraySizes.empty() );
      static std::string const subCopiesTemplate =
        R"(      ${separator} ${structMemberName}( ${subStructArgumentName}.${subStructMemberName} )
)";
      subCopies += replaceWithMap( subCopiesTemplate,
                                   { { "separator", firstArgument ? ":" : "," },
                                     { "structMemberName", vkStruct.members[i].name },
                                     { "subStructMemberName", subStruct->members[i].name },
                                     { "subStructArgumentName", subStructArgumentName } } );
      firstArgument = false;
    }

    std::vector<std::string> subArguments;
    for ( size_t i = subStruct->members.size(); i < vkStruct.members.size(); i++ )
    {
      std::string argument = generateStructConstructorArgument( vkStruct.name, vkStruct.members[i], true );
      if ( !argument.empty() )
      {
        subArguments.push_back( argument );
      }

      assert( vkStruct.members[i].arraySizes.empty() );
      static std::string const subCopiesTemplate = R"(    , ${memberName}( ${memberName}_ )
)";
      subCopies += replaceWithMap( subCopiesTemplate, { { "memberName", vkStruct.members[i].name } } );
    }

    static std::string const subStructConstructorTemplate = R"(
    explicit ${structName}( ${subStructName} const & ${subStructArgumentName}${subArguments} )
${subCopies}    {}
)";
    return replaceWithMap( subStructConstructorTemplate,
                           { { "structName", stripPrefix( vkStruct.name, "Vk" ) },
                             { "subArguments", generateList( subArguments, ", ", ", " ) },
                             { "subCopies", subCopies },
                             { "subStructArgumentName", subStructArgumentName },
                             { "subStructName", stripPrefix( subStruct->name, "Vk" ) } } );
  }
  return "";
}

std::string VulkanHppGenerator::generateSuccessCode( std::string const & code ) const
{
  // on each success code: prepend 'Result::e' to the tagged camel case version of the code
  return "Result::e" + generateTaggedCamelCase( code );
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
      auto const [enter, leave] = generateProtection( value.protect );
      std::string valueName     = generateEnumValueName( enumIt->first, value.name, false );
      cases += enter + "      case Result::" + valueName + ": throw " + stripPrefix( valueName, "eError" ) + "Error( message );\n" + leave;
    }
  }
  cases.pop_back();  // remove last newline

  std::string const throwTemplate = R"(
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
    std::set<std::string> elementTypes;
    for ( size_t i = 0; i < returnParams.size(); ++i )
    {
      if ( vectorParams.contains( returnParams[i] ) )
      {
        std::string elementType         = ( ( flavourFlags & CommandFlavourFlagBits::chained ) &&
                                            std::ranges::any_of( chainedReturnParams, [&returnParams, i]( size_t crp ) { return crp == returnParams[i]; } ) )
                                          ? "StructureChain"
                                          : dataTypes[i];
        std::string extendedElementType = elementType;
        if ( flavourFlags & CommandFlavourFlagBits::unique )
        {
          extendedElementType = "UniqueHandle<" + elementType + ", Dispatch>";
        }
        elementType = startUpperCase( stripPrefix( elementType, "VULKAN_HPP_NAMESPACE::" ) );
        // prevent multiple checks on the same allocator type
        if ( elementTypes.insert( elementType ).second )
        {
          if ( !enableIf.empty() )
          {
            enableIf += " && ";
          }
          enableIf += "std::is_same<typename " + elementType + "Allocator::value_type, " + extendedElementType + ">::value ";
        }
      }
    }
    if ( !enableIf.empty() )
    {
      typenameCheck = ", typename std::enable_if<" + enableIf + ", int>::type" + ( definition ? "" : " = 0" );
    }
  }
  return typenameCheck;
}

std::string VulkanHppGenerator::generateUnion( TypeUnion const & vkUnion ) const
{
  auto [enter, leave]   = generateProtection( getProtectFromType( vkUnion.name ) );
  std::string unionName = stripPrefix( vkUnion.name, "Vk" );

  bool                  firstMember = true;
  std::set<std::string> listedTypes;  // create just one constructor per different type !
  std::string           constructors;
  for ( auto memberIt = vkUnion.members.begin(); memberIt != vkUnion.members.end(); ++memberIt )
  {
    std::string typeName = memberIt->type.name;

    if ( memberIt->type.name == "VkBool32" )
    {
      // VkBool32 is aliased to uint32_t, so use uint32_t as the type name for the constructor to avoid creating a VkBool32 constructor if the union also
      // contains a uint32_t constructor.
      typeName = "uint32_t";
    }
    else
    {
      // if the type is a struct, check if it is an alias and use the struct name instead of the alias for the constructor
      auto memberStructIt = findByNameOrAlias( m_vkxml.types.structs, memberIt->type.name );
      if ( ( memberStructIt != m_vkxml.types.structs.end() ) && ( memberStructIt->name != memberIt->type.name ) )
      {
        assert( memberStructIt->aliases.contains( memberIt->type.name ) );
        typeName = memberStructIt->name;
      }
    }

    if ( listedTypes.insert( typeName ).second )
    {
      bool const multipleType = std::any_of(
        std::next( memberIt ), vkUnion.members.end(), [memberIt]( UnionMember const & member ) noexcept { return member.type == memberIt->type; } );
      std::string memberType = ( memberIt->arraySizes.empty() )
                               ? memberIt->type.compose( "Vk" )
                               : ( "const " + generateStandardArray( memberIt->type.compose( "Vk", "VULKAN_HPP_NAMESPACE" ), memberIt->arraySizes ) + "&" );

      // In a majority of cases this can be constexpr in C++11 as well, however, determining when exactly
      // that is the case is a lot more involved and probably not worth it.
      static std::string const constructorTemplate = R"(
    VULKAN_HPP_CONSTEXPR_14 ${unionName}( ${memberType} ${argumentName}_${defaultAssignment} )
      : ${memberName}( ${argumentName}_ )
    {})";

      constructors += ( firstMember ? "" : "\n" ) + replaceWithMap( constructorTemplate,
                                                                    { { "argumentName", multipleType ? generateName( memberIt->type ) : memberIt->name },
                                                                      { "defaultAssignment", firstMember ? " = {}" : "" },
                                                                      { "memberName", memberIt->name },
                                                                      { "memberType", memberType },
                                                                      { "unionName", stripPrefix( vkUnion.name, "Vk" ) } } );
      firstMember = false;

      if ( !memberIt->arraySizes.empty() )
      {
        assert( !multipleType );
        assert( memberIt->arraySizes.size() == 1 );
        int const size = std::stoi( memberIt->arraySizes[0] );
        assert( std::to_string( size ) == memberIt->arraySizes[0] );
        std::vector<std::string> arguments, callArguments;
        for ( int i = 0; i < size; i++ )
        {
          std::string argumentIndex = std::to_string( i );
          arguments.push_back( memberIt->type.name + " " + memberIt->name + "_" + argumentIndex );
          callArguments.push_back( memberIt->name + "_" + argumentIndex );
        }

        static std::string const constructorBySequenceTemplate = R"(
    VULKAN_HPP_CONSTEXPR ${unionName}( ${arguments} )
      : ${memberName}{ { { ${callArguments} } } }
    {})";

        constructors += "\n" + replaceWithMap( constructorBySequenceTemplate,
                                               { { "arguments", generateList( arguments, "", ", " ) },
                                                 { "callArguments", generateList( callArguments, "", ", " ) },
                                                 { "memberName", memberIt->name },
                                                 { "unionName", stripPrefix( vkUnion.name, "Vk" ) } } );
      }
    }
  }

  // one setter per union element
  std::string setters;
  for ( size_t i = 0; i < vkUnion.members.size(); i++ )
  {
    setters += generateUnionSetter( vkUnion, i );
  }
  // filter out leading and trailing newline
  setters = setters.substr( 1, setters.length() - 2 );

  // the union member variables
  std::string membersString;
  // if there's at least one Vk... type in this union, check for unrestricted unions support
  bool const needsUnrestrictedUnions =
    std::ranges::any_of( vkUnion.members, []( UnionMember const & member ) { return member.type.name.starts_with( "Vk" ); } );
  if ( needsUnrestrictedUnions )
  {
    membersString += "#ifdef VULKAN_HPP_HAS_UNRESTRICTED_UNIONS\n";
  }
  for ( auto const & member : vkUnion.members )
  {
    static std::string const memberTemplate = R"(    ${memberType} ${memberName};
)";
    membersString += replaceWithMap(
      memberTemplate,
      { { "memberName", member.name },
        { "memberType",
          member.arraySizes.empty() ? member.type.compose( "Vk" )
                                    : generateStandardArrayWrapper( member.type.compose( "Vk", "VULKAN_HPP_NAMESPACE" ), member.arraySizes ) } } );
  }
  if ( needsUnrestrictedUnions )
  {
    membersString += "#else\n";
    for ( auto const & member : vkUnion.members )
    {
      static std::string const memberTemplate = R"(    ${memberType} ${memberName}${arraySizes};
)";
      membersString += replaceWithMap(
        memberTemplate,
        { { "arraySizes", generateCArraySizes( member.arraySizes ) }, { "memberName", member.name }, { "memberType", member.type.compose( "" ) } } );
    }
    membersString += "#endif  /*VULKAN_HPP_HAS_UNRESTRICTED_UNIONS*/\n";
  }

  static std::string const unionTemplate = R"(
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
      return *reinterpret_cast<Vk${unionName} const *>( this );
    }

    operator Vk${unionName} &()
    {
      return *reinterpret_cast<Vk${unionName} *>( this );
    }

${members}
  };

#if 20 <= VULKAN_HPP_CPP_VERSION
  template <>
  struct CppType<Vk${unionName}>
  {
    using Type = ${unionName};
  };
#endif

${aliases}
${leave})";

  std::string aliases;
  for ( auto const & alias : vkUnion.aliases )
  {
    aliases += "  using " + stripPrefix( alias.first, "Vk" ) + " = " + unionName + ";\n";
  }

  return replaceWithMap( unionTemplate,
                         { { "aliases", aliases },
                           { "constructors", constructors },
                           { "enter", enter },
                           { "leave", leave },
                           { "members", membersString },
                           { "setters", setters },
                           { "unionName", unionName } } );
}

std::string VulkanHppGenerator::generateUnionSetter( TypeUnion const & vkUnion, size_t index ) const
{
  std::string         str;
  UnionMember const & member = vkUnion.members[index];
  assert( member.type.name != "VkStructureType" );  // VkStructureType is supposed to not be part of any union
  {
    static std::string const templateString = R"(
    VULKAN_HPP_CONSTEXPR_14 ${unionName} & set${MemberName}( ${memberType} ${reference}${memberName}_ ) & VULKAN_HPP_NOEXCEPT
    {
      ${assignment};
      return *this;
    }

    VULKAN_HPP_CONSTEXPR_14 ${unionName} && set${MemberName}( ${memberType} ${reference}${memberName}_ ) && VULKAN_HPP_NOEXCEPT
    {
      ${assignment};
      return std::move( *this );
    }
)";

    std::string memberType =
      member.type.name.starts_with( "PFN_vk" )
        ? "PFN_" + stripPrefix( member.type.name, "PFN_vk" )
        : ( member.arraySizes.empty() ? member.type.compose( "Vk" ) : generateStandardArray( member.type.compose( "Vk" ), member.arraySizes ) );

    str += replaceWithMap( templateString,
                           { { "assignment", member.name + " = " + member.name + "_" },
                             { "memberName", member.name },
                             { "MemberName", startUpperCase( member.name ) },
                             { "memberType", memberType },
                             { "reference",
                               ( member.type.postfix.empty() &&
                                 ( containsByName( m_vkxml.types.structs, member.type.name ) || containsByName( m_vkxml.types.unions, member.type.name ) ) )
                                 ? "const & "
                                 : "" },
                             { "unionName", stripPrefix( vkUnion.name, "Vk" ) } } );

    assert( member.len.empty() || ( member.len == "null-terminated" ) );
  }
  return str;
}

std::string VulkanHppGenerator::generateUniqueHandle( std::pair<std::string, HandleData> const & handleData ) const
{
  auto vkHandleIt = findByName( m_vkxml.types.handles, handleData.first );
  assert( vkHandleIt != m_vkxml.types.handles.end() );

  std::string type = stripPrefix( handleData.first, "Vk" );
  std::string aliasHandle;
  for ( auto const & alias : vkHandleIt->aliases )
  {
    static std::string const aliasHandleTemplate = R"(  using Unique${aliasType} = UniqueHandle<${type}, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;)";

    aliasHandle += replaceWithMap( aliasHandleTemplate, { { "aliasType", stripPrefix( alias.first, "Vk" ) }, { "type", type } } );
  }

  static std::string const uniqueHandleTemplate = R"(  template <typename Dispatch>
  class UniqueHandleTraits<${type}, Dispatch>
  {
  public:
    using deleter = detail::${deleterType}${deleterAction}<${deleterParent}${deleterPool}, Dispatch>;
  };
  using Unique${type} = UniqueHandle<${type}, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;
${aliasHandle})";

  assert( !handleData.second.constructorIndices.empty() );
  auto const & command       = m_vkxml.commands[handleData.second.constructorIndices.front()];
  std::string  deleterParent = ( ( handleData.first != "VkDevice" ) && isHandleType( command.params[0].type.name ) ) ? command.params[0].type.name : "";

  std::string deleterAction = "Destroy";
  if ( !handleData.second.deleteCommand.empty() )
  {
    if ( handleData.second.deleteCommand.substr( 2, 4 ) == "Free" )
    {
      deleterAction = "Free";
    }
    else if ( handleData.second.deleteCommand.substr( 2, 7 ) == "Release" )
    {
      deleterAction = "Release";
    }
  }

  return replaceWithMap( uniqueHandleTemplate,
                         { { "aliasHandle", aliasHandle },
                           { "deleterAction", deleterAction },
                           { "deleterParent", deleterParent.empty() ? "detail::NoParent" : stripPrefix( deleterParent, "Vk" ) },
                           { "deleterPool", handleData.second.deletePool.empty() ? "" : ", " + stripPrefix( handleData.second.deletePool, "Vk" ) },
                           { "deleterType", handleData.second.deleteCommand.empty() ? "Dummy" : ( handleData.second.deletePool.empty() ? "Object" : "Pool" ) },
                           { "type", type } } );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateUniqueHandle( std::vector<RequireType> const & requireTypes,
                                                      std::string const &              title,
                                                      std::set<std::string> &          listedHandles ) const
{
  std::string str;
  forEachRequiredHandle( requireTypes,
                         [&str, &listedHandles, this]( std::pair<std::string, HandleData> const & handleData )
                         {
                           if ( listedHandles.insert( handleData.first ).second )
                           {
                             str += generateUniqueHandle( handleData );
                           }
                         } );
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

  std::string           uniqueHandles;
  std::set<std::string> listedHandles;
  for ( auto const & feature : m_vkxml.features )
  {
    uniqueHandles += generateUniqueHandle( feature.require, feature.name, listedHandles );
  }
  for ( auto const & extension : m_extensions )
  {
    uniqueHandles += generateUniqueHandle( extension.requireData, extension.name, listedHandles );
  }
  if ( uniqueHandles.back() == '\n' )
  {
    uniqueHandles.pop_back();
  }
  return replaceWithMap( uniqueHandlesTemplate, { { "uniqueHandles", uniqueHandles } } );
}

std::string VulkanHppGenerator::generateSharedHandle( std::pair<std::string, HandleData> const & handleData ) const
{
  if ( !handleData.second.deleteCommand.empty() )
  {
    auto vkHandleIt = findByName( m_vkxml.types.handles, handleData.first );
    assert( vkHandleIt != m_vkxml.types.handles.end() );

    std::string type = stripPrefix( handleData.first, "Vk" );
    std::string aliasHandle;
    for ( auto const & alias : vkHandleIt->aliases )
    {
      static std::string const aliasHandleTemplate = R"(  using Shared${aliasType} = SharedHandle<${type}>;)";

      aliasHandle += replaceWithMap( aliasHandleTemplate, { { "aliasType", stripPrefix( alias.first, "Vk" ) }, { "type", type } } );
    }

    static std::string const sharedHandleTemplate = R"(  template <>
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
    auto vkHandleIt = findByName( m_vkxml.types.handles, handleData.first );
    assert( vkHandleIt != m_vkxml.types.handles.end() );

    std::string type = stripPrefix( handleData.first, "Vk" );
    std::string aliasHandle;
    for ( auto const & alias : vkHandleIt->aliases )
    {
      static std::string const aliasHandleTemplate = R"(  using Shared${aliasType} = SharedHandle<${type}>;)";

      aliasHandle += replaceWithMap( aliasHandleTemplate, { { "aliasType", stripPrefix( alias.first, "Vk" ) }, { "type", type } } );
    }

    static std::string const sharedHandleTemplate = R"(
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
                           { { "aliasHandle", aliasHandle }, { "parent", "Shared" + stripPrefix( vkHandleIt->parent, "Vk" ) }, { "type", type } } );
  }
  return "";
}

template <typename RequireType>
std::string VulkanHppGenerator::generateSharedHandle( std::vector<RequireType> const & requireTypes,
                                                      std::string const &              title,
                                                      std::set<std::string> &          listedHandles ) const
{
  std::string str;
  forEachRequiredHandle( requireTypes,
                         [&str, &listedHandles, this]( std::pair<std::string, HandleData> const & handleData )
                         {
                           if ( listedHandles.insert( handleData.first ).second )
                           {
                             str += generateSharedHandle( handleData );
                           }
                         } );
  return addTitleAndProtection( title, str );
}

template <typename RequireType>
std::string VulkanHppGenerator::generateSharedHandleNoDestroy( std::vector<RequireType> const & requireTypes, std::string const & title ) const
{
  std::string str;
  forEachRequiredHandle( requireTypes,
                         [&str, this]( std::pair<std::string, HandleData> const & handleData ) { str += generateSharedHandleNoDestroy( handleData ); } );
  return addTitleAndProtection( title, str );
}

std::string VulkanHppGenerator::generateSharedHandlesNoDestroy() const
{
  std::string sharedHandles;
  for ( auto const & feature : m_vkxml.features )
  {
    sharedHandles += generateSharedHandleNoDestroy( feature.require, feature.name );
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

  std::string           sharedHandles;
  std::set<std::string> listedHandles;
  for ( auto const & feature : m_vkxml.features )
  {
    sharedHandles += generateSharedHandle( feature.require, feature.name, listedHandles );
  }
  for ( auto const & extension : m_extensions )
  {
    sharedHandles += generateSharedHandle( extension.requireData, extension.name, listedHandles );
  }
  if ( sharedHandles.back() == '\n' )
  {
    sharedHandles.pop_back();
  }
  return replaceWithMap( sharedHandlesTemplate, { { "sharedHandles", sharedHandles } } );
}

std::string VulkanHppGenerator::generateTaggedCamelCase( std::string const & name ) const
{
  assert( name.starts_with( "VK_" ) );
  std::string taggedName = stripPrefix( name, "VK_" );
  std::string leadingTag = taggedName.substr( 0, taggedName.find_first_of( '_' ) );
  if ( containsByName( m_vkxml.tags.tags, leadingTag ) )
  {
    taggedName = stripPrefix( taggedName, leadingTag + "_" );
  }
  else
  {
    leadingTag = "";
  }
  std::string trailingTag = taggedName.substr( taggedName.find_last_of( '_' ) + 1 );
  if ( containsByName( m_vkxml.tags.tags, trailingTag ) )
  {
    taggedName = stripPostfix( taggedName, "_" + trailingTag );
  }
  else
  {
    trailingTag = "";
  }
  return leadingTag + toCamelCase( taggedName ) + trailingTag;
}

std::string VulkanHppGenerator::generateVectorSizeCheck( std::string const &                           name,
                                                         Command const &                               command,
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

  std::string className   = stripPrefix( command.params[initialSkipCount - 1].type.name, "Vk" );
  std::string commandName = generateCommandName( name, command.params, initialSkipCount );

  std::string assertions, throws;
  for ( auto const & cvm : countToVectorMap )
  {
    size_t const defaultStartIndex = determineDefaultStartIndex( command.params, skippedParams );
    std::string  firstVectorName   = startLowerCase( stripPrefix( command.params[cvm.second[0]].name, "p" ) );

    if ( cvm.second.size() == 1 )
    {
      std::string sizeValue;
      if ( !command.params[cvm.second[0]].altLen.empty() )
      {
        assert( command.params[cvm.second[0]].altLen == "(samples + 31) / 32" );
        sizeValue = "( static_cast<uint32_t>( samples ) + 31 ) / 32";
      }
      else if ( isLenByStructMember( command.params[cvm.second[0]].len, command.params[cvm.first] ) )
      {
        std::vector<std::string> lenParts = tokenize( command.params[cvm.second[0]].len, "->" );
        assert( lenParts.size() == 2 );
        sizeValue = startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + lenParts[1];
      }
      else
      {
        assert( !command.params[cvm.second[0]].len.empty() );
        sizeValue = command.params[cvm.second[0]].len;
      }
      assertions += replaceWithMap( assertTemplateSingle, { { "sizeValue", sizeValue }, { "vectorName", firstVectorName } } );
      throws += replaceWithMap( throwTemplateSingle,
                                { { "className", className }, { "commandName", commandName }, { "sizeValue", sizeValue }, { "vectorName", firstVectorName } } );
    }
    else
    {
      for ( size_t i = 1; i < cvm.second.size(); i++ )
      {
        std::string secondVectorName  = startLowerCase( stripPrefix( command.params[cvm.second[i]].name, "p" ) );
        bool const  withZeroSizeCheck = isOptional( command.params[cvm.second[i]].optional ) && ( defaultStartIndex <= cvm.second[i] );
        if ( command.params[cvm.second[0]].type.name == "void" )
        {
          assert( command.params[cvm.second[i]].type.name == "void" );
          std::string firstDataType  = stripPrefix( command.params[cvm.second[0]].name, "p" ) + "Type";
          std::string secondDataType = stripPrefix( command.params[cvm.second[i]].name, "p" ) + "Type";
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
    bool const skip = skipLeadingGrandParent( handle );
    assert( !handle.second.constructorIndices.empty() && ( ( skip ? 1u : 0u ) < m_vkxml.commands[handle.second.constructorIndices.front()].params.size() ) );
    auto const & param = m_vkxml.commands[handle.second.constructorIndices.front()].params[skip ? 1 : 0];
    assert( isHandleType( param.type.name ) && param.type.isValue() );
    parentType = stripPrefix( param.type.name, "Vk" );
    parentName = param.name;
  }
  return { parentType, parentName };
}

std::pair<std::string, std::string> VulkanHppGenerator::getPoolTypeAndName( std::string const & type ) const
{
  auto structIt = findByName( m_vkxml.types.structs, type );
  assert( structIt != m_vkxml.types.structs.end() );
  auto memberIt = std::ranges::find_if( structIt->members, []( StructMember const & member ) { return member.name.find( "Pool" ) != std::string::npos; } );
  assert( memberIt != structIt->members.end() );
  assert( std::none_of(
    std::next( memberIt ), structIt->members.end(), []( StructMember const & member ) { return member.name.find( "Pool" ) != std::string::npos; } ) );
  return { memberIt->type.name, memberIt->name };
}

std::string VulkanHppGenerator::getProtectFromPlatform( std::string const & platformName ) const
{
  auto platformIt = findByName( m_vkxml.platforms.platforms, platformName );
  return ( platformIt != m_vkxml.platforms.platforms.end() ) ? platformIt->protect : "";
}

std::string VulkanHppGenerator::getProtectFromTitle( std::string const & title ) const
{
  if ( !containsByName( m_vkxml.features, title ) )
  {
    auto extensionIt = findByName( m_extensions, title );
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

std::string VulkanHppGenerator::getVectorSize( std::vector<Param> const &                params,
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
    auto structIt = findByName( m_vkxml.types.structs, params[returnParam].type.name );
    assert( structIt != m_vkxml.types.structs.end() );
    vectorSize = startLowerCase( stripPrefix( params[returnParam].name, "p" ) ) + "." + structIt->members[returnVectorIt->second.lenParam].name;
  }
  else
  {
    std::vector<std::string> lenParts = tokenize( params[returnParam].len, "->" );
    switch ( lenParts.size() )
    {
      case 1:
        {
          std::string const & len    = lenParts[0];
          size_t              lenIdx = std::distance( params.begin(), std::ranges::find_if( params, [&len]( Param const & pd ) { return pd.name == len; } ) );
          assert( lenIdx < params.size() );
          // look for the len, not being the len of the return param, but of an other vector param
          auto lenVectorParamIt = std::ranges::find_if(
            vectorParams, [&lenIdx, &returnParam]( auto const & vpi ) { return ( vpi.first != returnParam ) && ( vpi.second.lenParam == lenIdx ); } );
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

void VulkanHppGenerator::handleRemoval( Remove const & remove )
{
  for ( auto const & command : remove.commands )
  {
    bool removed   = false;
    auto commandIt = findByNameOrAlias( m_vkxml.commands, command.name );
    assert( commandIt != m_vkxml.commands.end() );
    for ( auto const & requiredBy : commandIt->requiredBy )
    {
      auto featureIt = findByName( m_vkxml.features, requiredBy );
      if ( featureIt != m_vkxml.features.end() )
      {
        removed |= handleRemovalCommand( command.name, featureIt->require );
      }
      else
      {
        auto extensionIt = std::ranges::find_if( m_extensions, [&requiredBy]( ExtensionData const & ed ) { return ed.name == requiredBy; } );
        if ( extensionIt != m_extensions.end() )
        {
          removed |= handleRemovalCommand( command.name, extensionIt->requireData );
        }
        else
        {
          assert( containsByName( m_unsupportedExtensions, requiredBy ) );
          removed = true;
        }
      }
    }
    checkForError( removed, commandIt->xmlLine, "to be removed command <" + commandIt->name + "> could not be removed" );
  }

  for ( auto const & e : remove.enums )
  {
    bool removed = false;
    for ( auto enumIt = m_enums.begin(); !removed && enumIt != m_enums.end(); ++enumIt )
    {
      auto valueIt = findByName( enumIt->second.values, e.name );
      if ( valueIt != enumIt->second.values.end() )
      {
        enumIt->second.values.erase( valueIt );
        removed = true;
      }
    }
    checkForError( removed, 0, "to be removed enum value <" + e.name + "> could not be removed" );
  }

  for ( auto const & type : remove.types )
  {
    auto typeIt = m_types.find( type.name );
    assert( typeIt != m_types.end() );
    bool removed = typeIt->second.requiredBy.empty();
    for ( auto const & requiredBy : typeIt->second.requiredBy )
    {
      auto featureIt = findByName( m_vkxml.features, requiredBy );
      if ( featureIt != m_vkxml.features.end() )
      {
        removed |= handleRemovalType( type.name, featureIt->require );
      }
      else
      {
        auto extensionIt = std::ranges::find_if( m_extensions, [&requiredBy]( ExtensionData const & ed ) { return ed.name == requiredBy; } );
        if ( extensionIt != m_extensions.end() )
        {
          removed |= handleRemovalType( type.name, extensionIt->requireData );
        }
        else
        {
          assert( containsByName( m_unsupportedExtensions, requiredBy ) );
          removed = true;
        }
      }
    }
    checkForError( removed, typeIt->second.xmlLine, "to be removed type <" + typeIt->first + "> could not be removed" );
  }
}

template <typename RequireType>
bool VulkanHppGenerator::handleRemovalCommand( std::string const & command, std::vector<RequireType> & requireTypes )
{
  bool removed = false;
  for ( auto requireTypeIt = requireTypes.begin(); !removed && ( requireTypeIt != requireTypes.end() ); ++requireTypeIt )
  {
    auto requireCommandIt = std::ranges::find_if( requireTypeIt->commands, [&command]( auto const & c ) { return c.name == command; } );
    if ( requireCommandIt != requireTypeIt->commands.end() )
    {
      assert( std::none_of(
        std::next( requireCommandIt ), requireTypeIt->commands.end(), [&command]( auto const & requireCommand ) { return requireCommand.name == command; } ) );
      requireTypeIt->commands.erase( requireCommandIt );
      assert( !requireTypeIt->commands.empty() || !requireTypeIt->types.empty() );
      removed = true;

#if !defined( NDEBUG )
      for ( auto it = std::next( requireTypeIt ); it != requireTypes.end(); ++it )
      {
        assert( std::ranges::none_of( it->commands, [&command]( auto const & requireCommand ) { return requireCommand.name == command; } ) );
      }
#endif
    }
  }
  return removed;
}

void VulkanHppGenerator::handleRemovals()
{
  for ( auto const & feature : m_vkxml.features )
  {
    for ( auto const & remove : feature.removes )
    {
      handleRemoval( remove );
    }
  }
}

template <typename RequireType>
bool VulkanHppGenerator::handleRemovalType( std::string const & type, std::vector<RequireType> & requireTypes )
{
  bool removed = false;
  for ( auto requireTypeIt = requireTypes.begin(); !removed && ( requireTypeIt != requireTypes.end() ); )
  {
    auto typeIt = std::ranges::find_if( requireTypeIt->types, [&type]( auto const & requireType ) { return requireType.name == type; } );
    if ( typeIt != requireTypeIt->types.end() )
    {
      assert( std::none_of( std::next( typeIt ), requireTypeIt->types.end(), [&type]( auto const & requireType ) { return requireType.name == type; } ) );
      requireTypeIt->types.erase( typeIt );
      requireTypeIt = ( requireTypeIt->commands.empty() && requireTypeIt->types.empty() ) ? requireTypes.erase( requireTypeIt ) : std::next( requireTypeIt );
      removed       = true;

#if !defined( NDEBUG )
      for ( auto it = std::next( requireTypeIt ); it != requireTypes.end(); ++it )
      {
        assert( std::ranges::none_of( it->types, [&type]( auto const & requireType ) { return requireType.name == type; } ) );
      }
#endif
    }
    else
    {
      requireTypeIt = std::next( requireTypeIt );
    }
  }
  return removed;
}

bool VulkanHppGenerator::hasArrayConstructor( HandleData const & handleData ) const
{
  for ( auto constructorIndex : handleData.constructorIndices )
  {
    if ( !determineVectorParams( m_vkxml.commands[constructorIndex].params ).empty() )
    {
      return true;
    }
  }
  return false;
}

bool VulkanHppGenerator::hasLen( StructMember const & member, std::vector<StructMember> const & members ) const
{
  return !member.len.empty() && ( member.altLen.ends_with( " / 4" ) || ( ( member.len[0] == "null-terminated" ) && !member.arraySizes.empty() ) ||
                                  ( containsByName( members, member.len[0] ) && findByName( members, member.len[0] )->type.isValue() ) );
}

bool VulkanHppGenerator::hasParentHandle( std::string const & handle, std::string const & parent ) const
{
  std::string candidate = handle;
  while ( !candidate.empty() )
  {
    auto vkHandleIt = findByName( m_vkxml.types.handles, candidate );
    assert( vkHandleIt != m_vkxml.types.handles.end() );
    if ( vkHandleIt->parent == parent )
    {
      return true;
    }
    else
    {
      candidate = vkHandleIt->parent;
    }
  }
  return false;
}

bool VulkanHppGenerator::hasPointerParams( std::vector<Param> const & params ) const
{
  return std::ranges::any_of( params, []( auto const & pd ) { return pd.type.isPointer() || !pd.arraySizes.empty(); } );
}

bool VulkanHppGenerator::isDeviceCommand( Command const & command ) const
{
  return !command.params.empty() && isHandleType( command.params[0].type.name ) && ( command.params[0].type.name != "VkInstance" ) &&
         ( command.params[0].type.name != "VkPhysicalDevice" );
}

bool VulkanHppGenerator::isEnumerated( std::string const & type ) const
{
  std::string tag      = findTag( type, m_vkxml.tags.tags );
  std::string untagged = stripPostfix( type, tag );
  assert( !untagged.ends_with( "s" ) );
  if ( untagged.ends_with( "y" ) )
  {
    untagged.pop_back();
    untagged += "ie";
  }
  std::string pluralType = stripPrefix( untagged, "Vk" ) + "s" + tag;

  return std::ranges::any_of(
    m_vkxml.commands, [&pluralType]( auto const & command ) { return command.name.starts_with( "vkEnumerate" ) && command.name.ends_with( pluralType ); } );
}

bool VulkanHppGenerator::isExtension( std::string const & name ) const
{
  return containsByName( m_extensions, name ) || containsByName( m_unsupportedExtensions, name );
}

bool VulkanHppGenerator::isFeature( std::string const & name ) const
{
  return containsByName( m_vkxml.features, name ) || containsByName( m_unsupportedFeatures, name );
}

bool VulkanHppGenerator::isHandleType( std::string const & type ) const
{
  return type.starts_with( "Vk" ) && ( findByNameOrAlias( m_vkxml.types.handles, type ) != m_vkxml.types.handles.end() );
}

bool VulkanHppGenerator::isHandleTypeByStructure( std::string const & type ) const
{
  auto structIt = findByName( m_vkxml.types.structs, type );
  return ( structIt != m_vkxml.types.structs.end() ) && describesVector( *structIt ) && isHandleType( structIt->members[3].type.name );
}

bool VulkanHppGenerator::isLenByStructMember( std::string const & name, std::vector<Param> const & params ) const
{
  // check if name specifies a member of a struct
  std::vector<std::string> nameParts = tokenize( name, "->" );
  if ( nameParts.size() == 2 )
  {
    auto paramIt = std::ranges::find_if( params, [&n = nameParts[0]]( Param const & pd ) { return pd.name == n; } );
    if ( paramIt != params.end() )
    {
#if !defined( NDEBUG )
      auto structureIt = findByName( m_vkxml.types.structs, paramIt->type.name );
      assert( structureIt != m_vkxml.types.structs.end() );
      assert( isStructMember( nameParts[1], structureIt->members ) );
#endif
      return true;
    }
  }
  return false;
}

bool VulkanHppGenerator::isLenByStructMember( std::string const & name, Param const & param ) const
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
    auto structureIt = findByName( m_vkxml.types.structs, param.type.name );
    assert( structureIt != m_vkxml.types.structs.end() );
    assert( isStructMember( nameParts[1], structureIt->members ) );
#endif
    return true;
  }
  return false;
}

bool VulkanHppGenerator::isMultiSuccessCodeConstructor( std::vector<size_t> const & constructorIndices ) const
{
  bool ok = !constructorIndices.empty();
  if ( ok )
  {
    auto const & constructor = m_vkxml.commands[constructorIndices.front()];
    ok = ( 2 < constructor.successCodes.size() ) || ( ( constructor.successCodes.size() == 2 ) && ( constructor.successCodes[1] != "VK_INCOMPLETE" ) );
#if !defined( NDEBUG )
    for ( auto constructorIndexIt = std::next( constructorIndices.begin() ); constructorIndexIt != constructorIndices.end(); ++constructorIndexIt )
    {
      auto const & nextConstructor = m_vkxml.commands[*constructorIndexIt];
      assert( ok == ( 2 < nextConstructor.successCodes.size() ) ||
              ( ( nextConstructor.successCodes.size() == 2 ) && ( nextConstructor.successCodes[1] != "VK_INCOMPLETE" ) ) );
    }
#endif
  }
  return ok;
}

bool VulkanHppGenerator::isParam( std::string const & name, std::vector<Param> const & params ) const
{
  return findByName( params, name ) != params.end();
}

bool VulkanHppGenerator::isSimpleStructure( std::string const & name ) const
{
  auto structIt = findByName( m_vkxml.types.structs, name );
  return ( structIt != m_vkxml.types.structs.end() ) && !structureHoldsHandle( *structIt ) && !structureHoldsVector( *structIt );
}

bool VulkanHppGenerator::isStructMember( std::string const & name, std::vector<StructMember> const & members ) const
{
  return findByName( members, name ) != members.end();
}

bool VulkanHppGenerator::isStructureChainAnchor( std::string const & type ) const
{
  if ( type.starts_with( "Vk" ) )
  {
    auto structIt = findByNameOrAlias( m_vkxml.types.structs, type );
    return ( structIt != m_vkxml.types.structs.end() ) && !structIt->extendedBy.empty();
  }
  return false;
}

bool VulkanHppGenerator::isStructureType( std::string const & type ) const
{
  return type.starts_with( "Vk" ) && ( findByNameOrAlias( m_vkxml.types.structs, type ) != m_vkxml.types.structs.end() );
}

bool VulkanHppGenerator::isSupported( std::set<std::string> const & requiredBy ) const
{
  for ( auto const & r : requiredBy )
  {
    if ( containsByName( m_vkxml.features, r ) || containsByName( m_extensions, r ) )
    {
      return true;
    }
    else
    {
      assert( containsByName( m_unsupportedFeatures, r ) || containsByName( m_unsupportedExtensions, r ) );
    }
  }
  return false;
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
    auto featureIt = findByName( m_vkxml.features, require );
    if ( featureIt != m_vkxml.features.end() )
    {
      for ( auto const & r : featureIt->require )
      {
        if ( std::ranges::any_of( r.types, [&type]( auto const & requireType ) { return requireType.name == type; } ) )
        {
          return true;
        }
      }
    }
    else
    {
      auto extensionIt = findByName( m_extensions, require );
      if ( extensionIt != m_extensions.end() )
      {
        for ( auto const & r : extensionIt->requireData )
        {
          if ( std::ranges::any_of( r.types, [&type]( auto const & requireType ) { return requireType.name == type; } ) )
          {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool VulkanHppGenerator::isVectorByStructure( std::string const & type ) const
{
  auto structIt = findByName( m_vkxml.types.structs, type );
  return ( structIt != m_vkxml.types.structs.end() ) && describesVector( *structIt );
}

void VulkanHppGenerator::mergeInternalFeatures()
{
  // separate the internal and public features
  std::map<std::string, Feature> internalFeatures;
  for ( auto it = m_vkxml.features.begin(); it != m_vkxml.features.end(); )
  {
    if ( it->apiType == "internal" )
    {
      internalFeatures.insert( { it->name, std::move( *it ) } );
      it = m_vkxml.features.erase( it );
    }
    else
    {
      ++it;
    }
  }

  for ( auto & feature : m_vkxml.features )
  {
    mergeInternalFeatures( feature.depends, feature, internalFeatures );
  }

  // Check that all internal features have been processed
  checkForError( internalFeatures.empty(), 0, "not all internal features were merged into public features" );
}

void VulkanHppGenerator::mergeInternalFeatures( std::vector<std::string> const & depends, Feature & feature, std::map<std::string, Feature> & internalFeatures )
{
  for ( auto const & dep : depends )
  {
    auto depIt = internalFeatures.find( dep );
    if ( depIt != internalFeatures.end() )
    {
      // Recursively merge dependencies of the internal feature
      mergeInternalFeatures( depIt->second.depends, feature, internalFeatures );

      // adjust the requiredBy sets of the commands and enum values that are required by the internal feature to include the public feature
      for ( auto const & require : depIt->second.require )
      {
        for ( auto const & command : require.commands )
        {
          for ( auto & commandSpec : m_vkxml.commands )
          {
            if ( ( commandSpec.name == command.name ) || containsByName( commandSpec.aliases, command.name ) )
            {
              assert( commandSpec.requiredBy.contains( depIt->first ) );
              commandSpec.requiredBy.erase( depIt->first );
              commandSpec.requiredBy.insert( feature.name );
            }
          }
        }
        for ( auto const & e : require.enumRegulars )
        {
          auto enumIt = findByName( m_vkxml.types.enums, e.extends );
          assert( enumIt != m_vkxml.types.enums.end() );
          auto valueIt = findByName( enumIt->values, e.name );
          assert( valueIt != enumIt->values.end() );
          valueIt->requiredBy = depIt->first;
        }
      }

      // Merge the internal feature's require data into the public feature
      feature.require.insert(
        feature.require.end(), std::make_move_iterator( depIt->second.require.begin() ), std::make_move_iterator( depIt->second.require.end() ) );

      // Remove the internal feature to avoid re-processing
      internalFeatures.erase( depIt );
    }
  }
}

bool VulkanHppGenerator::needsStructureChainResize( std::map<size_t, VectorParamData> const & vectorParams,
                                                    std::vector<size_t> const &               chainedReturnParams ) const
{
  auto it = std::ranges::find_if( chainedReturnParams, [&vectorParams]( size_t crp ) { return vectorParams.contains( crp ); } );
  // assert that there's no other chained vector param !
  assert( ( it == chainedReturnParams.end() ) ||
          std::none_of( std::next( it ), chainedReturnParams.end(), [&vectorParams]( size_t crp ) { return vectorParams.contains( crp ); } ) );
  return ( it != chainedReturnParams.end() );
}

std::pair<bool, std::map<size_t, std::vector<size_t>>> VulkanHppGenerator::needsVectorSizeCheck( std::vector<Param> const &                params,
                                                                                                 std::map<size_t, VectorParamData> const & vectorParams,
                                                                                                 std::vector<size_t> const &               returnParams,
                                                                                                 std::set<size_t> const &                  singularParams,
                                                                                                 std::set<size_t> const &                  skippedParams ) const
{
  std::map<size_t, std::vector<size_t>> countToVectorMap;
  for ( auto const & vpi : vectorParams )
  {
    if ( ( vpi.second.lenParam != INVALID_INDEX ) && std::ranges::none_of( returnParams, [&vpi]( size_t rp ) { return rp == vpi.first; } ) &&
         ( !singularParams.contains( vpi.second.lenParam ) || isLenByStructMember( params[vpi.first].len, params[vpi.second.lenParam] ) ) )
    {
      countToVectorMap[vpi.second.lenParam].push_back( vpi.first );
    }
  }
  return { std::ranges::any_of( countToVectorMap,
                                [this, &params, &skippedParams]( auto const & cvm )
                                {
                                  return ( 1 < cvm.second.size() ) || isLenByStructMember( params[cvm.second[0]].len, params[cvm.first] ) ||
                                         std::ranges::none_of( skippedParams,
                                                               [&params, &len = params[cvm.second[0]].len]( size_t sp ) { return params[sp].name == len; } );
                                } ),
           countToVectorMap };
}

VulkanHppGenerator::DefinesPartition VulkanHppGenerator::partitionDefines( std::vector<TypeDefine> const & defines ) const
{
  DefinesPartition partition{};
  for ( auto const & define : defines )
  {
    // VK_DEFINE_HANDLE is macro magic that cannot be constexpr-ed
    // Also filter out the VKSC_ macros, as although they are in the spec, they are not defined in any header.
    if ( define.name.starts_with( "VK_" ) && ( define.name != "VK_DEFINE_HANDLE" ) )
    {
      if ( define.possibleCallee.empty() )
      {
        if ( define.possibleDefinition.empty() )
        {
#if !defined( NDEBUG )
          const std::set<std::string> ignoredDefines{ "VK_DEFINE_NON_DISPATCHABLE_HANDLE", "VK_NULL_HANDLE" };
#endif
          assert( ignoredDefines.contains( define.name ) );
        }
        else
        {
          if ( define.params.empty() )
          {
            partition.values.insert( { define.name, define } );
          }
          else
          {
            partition.callees.insert( { define.name, define } );
          }
        }
      }
      else
      {
        assert( !define.params.empty() && define.possibleDefinition.empty() );
        partition.callers.insert( { define.name, define } );
      }
    }
  }
  return partition;
}

void VulkanHppGenerator::registerDeleter( Command const & command )
{
  // some special handling for release functions that don't release an object
  std::set<std::string> const noDeleterFunctions = {
    "vkReleaseCapturedPipelineData", "vkReleaseFullScreenExclusiveMode", "vkReleaseProfilingLock", "vkReleaseSwapchainImages"
  };

  std::string tag = findTag( command.name, m_vkxml.tags.tags );
  if ( ( command.name.substr( 2, 7 ) == "Destroy" ) || ( command.name.substr( 2, 4 ) == "Free" ) ||
       ( ( command.name.substr( 2, 7 ) == "Release" ) && !noDeleterFunctions.contains( stripPostfix( command.name, tag ) ) ) )
  {
    std::string key;
    size_t      valueIndex = 0;
    switch ( command.params.size() )
    {
      case 2:
        if ( command.params.back().type.name == "VkAllocationCallbacks" )
        {
          key        = "";
          valueIndex = 0;
        }
        else
        {
          key        = command.params[0].type.name;
          valueIndex = 1;
        }
        break;
      case 3:
        assert( command.params.back().type.name == "VkAllocationCallbacks" );
        key        = command.params[0].type.name;
        valueIndex = 1;
        break;
      case 4:
        key        = command.params[0].type.name;
        valueIndex = 3;
        assert( m_handles.contains( command.params[valueIndex].type.name ) );
        m_handles.find( command.params[valueIndex].type.name )->second.deletePool = command.params[1].type.name;
        break;
      default: assert( false );
    }
    auto keyHandleIt = m_handles.find( key );
    assert( keyHandleIt != m_handles.end() );
    keyHandleIt->second.childrenHandles.insert( command.params[valueIndex].type.name );

    auto handleIt = m_handles.find( command.params[valueIndex].type.name );
    assert( handleIt != m_handles.end() );
    handleIt->second.deleteCommand  = command.name;
    handleIt->second.destructorType = key;
  }
}

void VulkanHppGenerator::rescheduleRAIIHandle( std::string &                              str,
                                               std::pair<std::string, HandleData> const & handle,
                                               std::set<std::string> &                    listedHandles,
                                               std::set<std::string> const &              specialFunctions ) const
{
  auto vkHandleIt = findByName( m_vkxml.types.handles, handle.first );
  assert( vkHandleIt != m_vkxml.types.handles.end() );

  listedHandles.insert( handle.first );
  if ( !vkHandleIt->parent.empty() && !listedHandles.contains( vkHandleIt->parent ) )
  {
    auto parentIt = m_handles.find( vkHandleIt->parent );
    assert( parentIt != m_handles.end() );
    str += generateRAIIHandle( *parentIt, listedHandles, specialFunctions );
  }

  for ( auto constructorIndex : handle.second.constructorIndices )
  {
    for ( auto const & param : m_vkxml.commands[constructorIndex].params )
    {
      auto handleIt = m_handles.find( param.type.name );
      if ( handleIt != m_handles.end() && !listedHandles.contains( param.type.name ) )
      {
        str += generateRAIIHandle( *handleIt, listedHandles, specialFunctions );
      }
    }
  }
}

template <typename RequireType>
std::vector<std::string> VulkanHppGenerator::selectCommandsByHandle( std::vector<RequireType> const & requireTypes,
                                                                     std::set<std::string> const &    handleCommands,
                                                                     std::set<std::string> &          listedCommands ) const
{
  std::vector<std::string> selectedCommands;
  forEachRequiredCommand( requireTypes,
                          [&]( std::string const & name, auto const & )
                          {
                            if ( handleCommands.contains( name ) && listedCommands.insert( name ).second )
                            {
                              selectedCommands.push_back( name );
                            }
                          } );
  return selectedCommands;
}

bool VulkanHppGenerator::skipLeadingGrandParent( std::pair<std::string, HandleData> const & handle ) const
{
  auto vkHandleIt = findByName( m_vkxml.types.handles, handle.first );
  assert( vkHandleIt != m_vkxml.types.handles.end() );

  bool skip = false;
  assert( !handle.second.constructorIndices.empty() );
  auto const constructor = m_vkxml.commands[handle.second.constructorIndices.front()];
  if ( ( 1 < constructor.params.size() ) && isHandleType( constructor.params[0].type.name ) && ( constructor.params[1].type.name == vkHandleIt->parent ) )
  {
    auto vkParentIt = findByName( m_vkxml.types.handles, vkHandleIt->parent );
    assert( vkParentIt != m_vkxml.types.handles.end() );
    skip = ( constructor.params[0].type.name == vkParentIt->parent );
#if !defined( NDEBUG )
    for ( auto constructorIndexIt = std::next( handle.second.constructorIndices.begin() ); constructorIndexIt != handle.second.constructorIndices.end();
          ++constructorIndexIt )
    {
      auto const & nextConstructor = m_vkxml.commands[*constructorIndexIt];
      assert( nextConstructor.params[0].type.name == constructor.params[0].type.name );
      assert( !skip || ( nextConstructor.params[1].type.name == constructor.params[1].type.name ) );
    }
#endif
  }
  return skip;
}

std::string VulkanHppGenerator::stripPluralS( std::string const & name ) const
{
  std::string strippedName = name;
  std::string tag          = findTag( name, m_vkxml.tags.tags );
  if ( strippedName.ends_with( "s" + tag ) )
  {
    size_t const pos = strippedName.rfind( 's' );
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

bool VulkanHppGenerator::structureChainHoldsHandle( std::string const & name ) const
{
  auto structIt = findByNameOrAlias( m_vkxml.types.structs, name );
  assert( structIt != m_vkxml.types.structs.end() );
  auto extendedByIt = std::ranges::find_if( structIt->extendedBy,
                                            [this]( auto const & s )
                                            {
                                              assert( containsByName( m_vkxml.types.structs, s ) );
                                              return structureHoldsHandle( *findByName( m_vkxml.types.structs, s ) );
                                            } );
  return extendedByIt != structIt->extendedBy.end();
}

bool VulkanHppGenerator::structureChainHoldsVector( std::string const & name ) const
{
  auto structIt = findByNameOrAlias( m_vkxml.types.structs, name );
  assert( structIt != m_vkxml.types.structs.end() );
  auto extendedByIt = std::ranges::find_if( structIt->extendedBy,
                                            [this]( auto const & s )
                                            {
                                              assert( containsByName( m_vkxml.types.structs, s ) );
                                              return structureHoldsVector( *findByName( m_vkxml.types.structs, s ) );
                                            } );
  return extendedByIt != structIt->extendedBy.end();
}

bool VulkanHppGenerator::structureHoldsHandle( TypeStruct const & vkStruct ) const
{
  return findHandleMember( vkStruct.members ) != vkStruct.members.end();
}

bool VulkanHppGenerator::structureHoldsVector( TypeStruct const & vkStruct ) const
{
  return findVectorMember( vkStruct.members ) != vkStruct.members.end();
}

std::string VulkanHppGenerator::toString( TypeCategory category ) const
{
  switch ( category )
  {
    case TypeCategory::Bitmask    : return "bitmask";
    case TypeCategory::BaseType   : return "basetype";
    case TypeCategory::Define     : return "define";
    case TypeCategory::Enum       : return "enum";
    case TypeCategory::External   : return "external";
    case TypeCategory::FuncPointer: return "funcpointer";
    case TypeCategory::Handle     : return "handle";
    case TypeCategory::Struct     : return "struct";
    case TypeCategory::Union      : return "union";
    case TypeCategory::Unknown    : return "unkown";
    default                       : assert( false ); return "";
  }
}

StructMember const & VulkanHppGenerator::vectorMemberByStructure( std::string const & structureType ) const
{
  auto structIt = findByName( m_vkxml.types.structs, structureType );
  assert( structIt != m_vkxml.types.structs.end() );
  assert( describesVector( *structIt ) );
  return structIt->members.back();
}

bool VulkanHppGenerator::EnumData::addEnumAlias( int line, std::string const & name, std::string const & alias, std::string const & protect, bool supported )
{
  auto aliasIt = findByName( valueAliases, name );
  bool ok      = ( aliasIt == valueAliases.end() );
  if ( ok )
  {
    valueAliases.push_back( { alias, name, protect, supported, line } );
  }
  else
  {
    ok = ( ( aliasIt->alias == alias ) && ( aliasIt->protect == protect ) && ( aliasIt->supported == supported ) );
  }
  return ok;
}

bool VulkanHppGenerator::EnumData::addEnumValue(
  int line, std::string const & name, std::string const & protect, std::string const & bitpos, std::string const & value, bool supported, bool deprecated )
{
  auto valueIt = findByName( values, name );
  bool ok      = ( valueIt == values.end() );
  if ( ok )
  {
    values.push_back( { name, {}, bitpos, deprecated, protect, supported, value, line } );
  }
  else if ( supported )  // only for supported enum values, we need to check for consistency!
  {
    ok = ( bitpos == valueIt->bitpos ) && ( protect == valueIt->protect ) && ( value == valueIt->value );
    // if a previous version was not supported, make it supported now
    if ( !valueIt->supported )
    {
      valueIt->supported = true;
      valueIt->xmlLine   = line;
    }
  }
  return ok || !supported;
}

//
// VulkanHppGenerator local functions
//

namespace
{
  size_t determineParamIndex( std::vector<Param> const & params, std::string const & name )
  {
    auto it = findByName( params, name );
    return ( it != params.end() ) ? static_cast<size_t>( std::distance( params.begin(), it ) ) : INVALID_INDEX;
  }

  template <typename T>
  typename std::map<std::string, T>::iterator findByNameOrAlias( std::map<std::string, T> & values, std::string const & name )
  {
    auto it = values.find( name );
    if ( it == values.end() )
    {
      it = std::ranges::find_if( values, [&name]( auto & value ) { return value.second.aliases.contains( name ); } );
    }
    assert( it != values.end() );
    return it;
  }

  template <typename T>
  typename std::vector<T>::const_iterator findByType( std::vector<T> const & values, std::string const & type )
  {
    return std::ranges::find_if( values, [&type]( auto const & value ) { return value.type.name == type; } );
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
      for ( auto it = std::next( elements.begin() ); it != elements.end(); ++it )
      {
        list += separator + *it;
      }
    }
    return list;
  }

  std::string generateNoDiscard( bool returnsSomething, bool multiSuccessCodes, bool multiErrorCodes )
  {
    return ( returnsSomething || multiSuccessCodes ) ? "VULKAN_HPP_NODISCARD " : ( multiErrorCodes ? "VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS " : "" );
  }

  std::string generateStandardArray( std::string const & type, std::vector<std::string> const & sizes )
  {
    assert( !sizes.empty() );

    std::string arrayString = "std::array<" + type + "," + sizes.back() + ">";
    for ( auto it = std::prev( sizes.rend() ); it != sizes.rbegin(); --it )
    {
      arrayString = "std::array<" + arrayString + "," + *it + ">";
    }
    return arrayString;
  }

  bool isApiSupported( std::vector<std::string> const & apis, std::string const & api )
  {
    return apis.empty() || ( std::ranges::find( apis, api ) != apis.end() );
  }

  bool isOptional( std::vector<bool> const & optionals )
  {
    return ( optionals.size() == 1 ) && optionals[0];
  }

  bool isUpperCase( std::string const & name )
  {
    return std::ranges::all_of( name, []( auto const & c ) noexcept { return c == toupper( c ); } );
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
}  // namespace

int main( int argc, char const ** argv )
{
  if ( ( argc % 2 ) == 0 )
  {
    std::cout << "VulkanHppGenerator usage: VulkanHppGenerator [-f filename][-api [vulkan|vulkanbase|vulkansc]]" << std::endl;
    std::cout << "\tdefault for filename is <" << VK_SPEC << ">" << std::endl;
    std::cout << "\tdefault for api <vulkan>" << std::endl;
    std::cout << "\tsupported values for api are <vulkan>, <vulkanbase>, and <vulkansc>" << std::endl;
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

  if ( ( api != "vulkan" ) && ( api != "vulkanbase" ) && ( api != "vulkansc" ) )
  {
    std::cout << "unsupported api <" << api << ">" << std::endl;
    return -1;
  }

#if defined( CLANG_FORMAT_EXECUTABLE )
  std::cout << "VulkanHppGenerator: Found ";
  std::string commandString = "\"" CLANG_FORMAT_EXECUTABLE "\" --version ";
  int const   ret           = std::system( commandString.c_str() );
  if ( ret != 0 )
  {
    std::cout << "VulkanHppGenerator: failed to determine clang_format version with error <" << ret << ">\n";
  }
#else
  std::cout << "VulkanHppGenerator: could not find clang-format. The generated files will not be formatted accordingly.\n";
#endif

  tinyxml2::XMLDocument doc;
  std::cout << "VulkanHppGenerator: Loading " << filename << std::endl;
  tinyxml2::XMLError const error = doc.LoadFile( filename.c_str() );
  if ( error != tinyxml2::XML_SUCCESS )
  {
    std::cout << "VulkanHppGenerator: failed to load file " << filename << " with error <" << toString( error ) << ">" << std::endl;
    return -1;
  }

  try
  {
    std::cout << "VulkanHppGenerator: Parsing " << filename << std::endl;

    VulkanHppGenerator generator( parseVkXml( doc, api ), api );

    generator.prepareRAIIHandles();
    generator.generateCppmFile();
    generator.generateHppFile();
    generator.generateEnumsHppFile();
    generator.generateExtensionInspectionFile();
    generator.generateFormatTraitsHppFile();
    generator.generateFuncsHppFile();
    generator.generateHandlesHppFile();
    generator.generateHashHppFile();
    generator.generateMacrosFile();
    generator.generateSharedHppFile();
    generator.generateStaticAssertionsHppFile();
    generator.generateStructsHppFile();
    generator.generateToStringHppFile();

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
