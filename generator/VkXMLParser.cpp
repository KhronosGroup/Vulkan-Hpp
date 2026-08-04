// SPDX-FileCopyrightText: 2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

#include "VkXMLParser.hpp"

#include "DependencyParser.hpp"
#include "XMLHelper.hpp"

#include <vector>

void  checkExtensionOrStructAndMember( std::string const & depends, int xmlLine, std::string const & prefix, std::vector<TypeStruct> const & structs );
bool  containsByNameAndExport( std::vector<Command> const & commands, std::string const & name, std::vector<std::string> const & exports );
bool  isLenByStructMember( std::string const & name, std::vector<Param> const & params, std::vector<TypeStruct> const & structs );
void  normalizeVersion( std::vector<std::vector<std::string>> & dependencies );
Alias parseAlias( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes, std::string const & category );
BitmaskValueVariant          parseBitmaskValue( tinyxml2::XMLElement const * element );
void                         parseCommand( tinyxml2::XMLElement const * element, std::vector<Command> & commands );
std::vector<Command>         parseCommands( tinyxml2::XMLElement const * element );
Component                    parseComponent( tinyxml2::XMLElement const * element );
ConstantValue                parseConstantValue( tinyxml2::XMLElement const * element );
Deprecate                    parseDeprecate( tinyxml2::XMLElement const * element );
EnumsVariant                 parseEnums( tinyxml2::XMLElement const * element );
EnumsBitmask                 parseEnumsBitmask( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
EnumsConstants               parseEnumsConstants( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
EnumsEnum                    parseEnumsEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
EnumUnused                   parseEnumUnused( tinyxml2::XMLElement const * element );
EnumValueVariant             parseEnumValue( tinyxml2::XMLElement const * element );
ExtensionRemove              parseExtensionRemove( tinyxml2::XMLElement const * element );
ExtensionRequire             parseExtensionRequire( tinyxml2::XMLElement const * element );
ExtensionRequireEnum         parseExtensionRequireEnum( tinyxml2::XMLElement const * element );
Extensions                   parseExtensions( tinyxml2::XMLElement const * element );
Feature                      parseFeature( tinyxml2::XMLElement const * element );
FeatureElement               parseFeatureElement( tinyxml2::XMLElement const * element );
Format                       parseFormat( tinyxml2::XMLElement const * element );
std::vector<Format>          parseFormats( tinyxml2::XMLElement const * element );
void                         parseImplicitExternSyncParams( tinyxml2::XMLElement const * element );
void                         parseImplicitExternSyncParamsParam( tinyxml2::XMLElement const * element );
MultiFeatureElement          parseMultiFeatureElement( tinyxml2::XMLElement const * element );
std::pair<std::string, Type> parseNameAndType( tinyxml2::XMLElement const * element );
std::tuple<std::string, Type, std::vector<std::string>, std::string> parseNameAndTypeModified( tinyxml2::XMLElement const * element );
std::pair<std::string, std::string>                                  parseNameWithAlias( tinyxml2::XMLElement const * element );
Param                                                                parseParam( tinyxml2::XMLElement const * element );
Plane                                                                parsePlane( tinyxml2::XMLElement const * element );
Platform                                                             parsePlatform( tinyxml2::XMLElement const * element );
Platforms                                                            parsePlatforms( tinyxml2::XMLElement const * element );
std::pair<std::string, Type>                                         parseProto( tinyxml2::XMLElement const * element );
Vkxml                                                                parseRegistry( tinyxml2::XMLElement const * element, std::string const & api );
Remove                                                               parseRemove( tinyxml2::XMLElement const * element );
Require                                                              parseRequire( tinyxml2::XMLElement const * element );
RequireEnum                                                          parseRequireEnum( tinyxml2::XMLElement const * element );
RequireType                                                          parseRequireType( tinyxml2::XMLElement const * element );
SPIRVCapabilities                                                    parseSPIRVCapabilities( tinyxml2::XMLElement const * element );
SPIRVCapability                                                      parseSPIRVCapability( tinyxml2::XMLElement const * element );
SPIRVCapabilityEnable                                                parseSPIRVCapabilityEnable( tinyxml2::XMLElement const * element );
SPIRVExtension                                                       parseSPIRVExtension( tinyxml2::XMLElement const * element );
SPIRVExtensionEnable                                                 parseSPIRVExtensionEnable( tinyxml2::XMLElement const * element );
SPIRVExtensions                                                      parseSPIRVExtensions( tinyxml2::XMLElement const * element );
StructMember                                                         parseStructMember( tinyxml2::XMLElement const * element );
SupersededName                                                       parseSupersededName( tinyxml2::XMLElement const * element );
Sync                                                                 parseSync( tinyxml2::XMLElement const * element );
SyncAccess                                                           parseSyncAccess( tinyxml2::XMLElement const * element );
SyncAccessSupport                                                    parseSyncAccessSupport( tinyxml2::XMLElement const * element );
SyncAccessEquivalent                                                 parseSyncAccessEquivalent( tinyxml2::XMLElement const * element );
SyncPipeline                                                         parseSyncPipeline( tinyxml2::XMLElement const * element );
SyncPipelineStage                                                    parseSyncPipelineStage( tinyxml2::XMLElement const * element );
SyncStage                                                            parseSyncStage( tinyxml2::XMLElement const * element );
SyncStageEquivalent                                                  parseSyncStageEquivalent( tinyxml2::XMLElement const * element );
SyncStageSupport                                                     parseSyncStageSupport( tinyxml2::XMLElement const * element );
Tag                                                                  parseTag( tinyxml2::XMLElement const * element );
Tags                                                                 parseTags( tinyxml2::XMLElement const * element );
std::string                                                          parseText( tinyxml2::XMLElement const * element );
Type                                                                 parseType( tinyxml2::XMLElement const * element );
TypeBaseType            parseTypeBaseType( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
BitmaskVariant          parseTypeBitmask( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
TypeDefine              parseTypeDefine( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
EnumVariant             parseTypeEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
TypeFuncPointer         parseTypeFuncPointer( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
HandleVariant           parseTypeHandle( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
StructVariant           parseTypeStruct( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes, std::string const & api );
TypeUnion               parseTypeUnion( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
Types                   parseTypes( tinyxml2::XMLElement const * element, std::string const & api );
TypeVariant             parseTypesType( tinyxml2::XMLElement const * element, std::string const & api );
UnionMember             parseUnionMember( tinyxml2::XMLElement const * element );
VideoCapabilities       parseVideoCapabilities( tinyxml2::XMLElement const * element );
VideoCodec              parseVideoCodec( tinyxml2::XMLElement const * element );
std::vector<VideoCodec> parseVideoCodecs( tinyxml2::XMLElement const * element );
VideoFormat             parseVideoFormat( tinyxml2::XMLElement const * element );
VideoFormatProperties   parseVideoFormatProperties( tinyxml2::XMLElement const * element );
VideoProfile            parseVideoProfile( tinyxml2::XMLElement const * element );
VideoProfileMember      parseVideoProfileMember( tinyxml2::XMLElement const * element );
VideoProfiles           parseVideoProfiles( tinyxml2::XMLElement const * element );
VideoRequireCapabilities parseVideoRequireCapabilities( tinyxml2::XMLElement const * element );

void checkExtensionOrStructAndMember( std::string const & depends, int xmlLine, std::string const & prefix, std::vector<TypeStruct> const & structs )
{
  std::vector<std::string> tokens = tokenize( depends, "::" );
  checkForError( "vk.xml", ( tokens.size() == 1 ) || ( tokens.size() == 2 ), xmlLine, prefix + " an unexpectedly formatted string: <" + depends + ">" );
  if ( tokens.size() == 2 )
  {
    auto structIt = std::ranges::find_if( structs, [&tokens]( TypeStruct const & s ) { return s.name == tokens[0]; } );
    checkForError( "vk.xml", structIt != structs.end(), xmlLine, prefix + " an unknown struct <" + tokens[0] + ">" );
    checkForError(
      "vk.xml", containsByName( structIt->members, tokens[1] ), xmlLine, prefix + " an unknown member <" + tokens[1] + "> of structure <" + tokens[0] + ">" );
  }
  // CHECK: after extensions: tokens.size() == 1 -> is extension
}

void checkNumber( std::string const & number, int line, std::string const & message )
{
  checkForError( "vk.xml", std::ranges::all_of( number, []( char c ) { return std::isdigit( c ); } ), line, message + " <" + number + ">" );
}

bool containsByNameAndExport( std::vector<Command> const & commands, std::string const & name, std::vector<std::string> const & exports )
{
  // check if there is a command with the specified name and at least one export in the specified exports list
  return std::ranges::any_of(
    commands,
    [&name, &exports]( Command const & command )
    {
      return ( command.name == name ) &&
             std::ranges::any_of(
               command.exports,
               [&exports]( std::string const & commandExport )
               { return std::ranges::any_of( exports, [&commandExport]( std::string const & exportItem ) { return exportItem == commandExport; } ); } );
    } );
}

bool isLenByStructMember( std::string const & name, std::vector<Param> const & params, std::vector<TypeStruct> const & structs )
{
  // check if name specifies a member of a struct
  std::vector<std::string> nameParts = tokenize( name, "->" );
  if ( nameParts.size() == 2 )
  {
    auto paramIt = std::ranges::find_if( params, [&n = nameParts[0]]( Param const & pd ) { return pd.name == n; } );
    if ( paramIt != params.end() )
    {
      auto structIt = std::ranges::find_if( structs, [&n = paramIt->type.name]( TypeStruct const & s ) { return s.name == n; } );
      return ( structIt != structs.end() ) && containsByName( structIt->members, nameParts[1] );
    }
  }
  return false;
}

void normalizeVersion( std::vector<std::vector<std::string>> & dependencies )
{
  for ( auto & dep : dependencies )
  {
    auto featureIt = std::ranges::find_if( dep, []( std::string const & d ) { return d.starts_with( "VK_VERSION" ); } );
    if ( featureIt == dep.end() )
    {
      // the dependency does not start with a VK_VERSION -> prepend a VK_VERSION_1_0 as a fallback
      dep.insert( dep.begin(), "VK_VERSION_1_0" );
    }
    else if ( featureIt != dep.begin() )
    {
      // the VK_VERSION dependency is not at the beginning of the dependencies, move it there, keeping the order of the other dependencies
      std::string version = *featureIt;
      dep.erase( featureIt );
      dep.insert( dep.begin(), version );
    }

    // remove any other VK_VERSION dependencies, keeping only the highest one
    featureIt = std::find_if( std::next( dep.begin() ), dep.end(), []( std::string const & d ) { return d.starts_with( "VK_VERSION" ); } );
    while ( featureIt != dep.end() )
    {
      if ( dep.front() < *featureIt )
      {
        dep.front() = *featureIt;
      }
      dep.erase( featureIt );
      featureIt = std::find_if( std::next( dep.begin() ), dep.end(), []( std::string const & d ) { return d.starts_with( "VK_VERSION" ); } );
    }
  }
  assert( std::ranges::all_of( dependencies, []( std::vector<std::string> const & dep ) { return dep[0].starts_with( "VK_VERSION" ); } ) );
}

Alias parseAlias( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes, std::string const & category )
{
  int const line = element->GetLineNum();

  checkAttributes( "vk.xml", line, attributes, { { "alias", {} }, { "category", { category } }, { "name", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  Alias alias{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      alias.alias = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      alias.name = attribute.second;
    }
  }

  return alias;
}

BitmaskValueVariant parseBitmaskValue( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  if ( attributes.contains( "alias" ) )
  {
    checkAttributes( "vk.xml", line, attributes, { { "alias", {} }, { "name", {} } }, { { "api", { "vulkan" } }, { "deprecated", { "aliased" } } } );

    EnumValueAlias alias{ .xmlLine = line };
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "alias" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        alias.alias = attribute.second;
      }
      else if ( attribute.first == "api" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        alias.api = attribute.second;
      }
      else if ( attribute.first == "deprecated" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        alias.deprecated = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        alias.name = attribute.second;
      }
    }
    return alias;
  }
  else
  {
    checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, { { "bitpos", {} }, { "comment", {} }, { "value", {} } } );

    BitmaskValueRegular value{ .xmlLine = line };
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "bitpos" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        value.bitPos = attribute.second;
      }
      else if ( attribute.first == "comment" )
      {
        value.comment = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        value.name = attribute.second;
      }
      else if ( attribute.first == "value" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        value.value = attribute.second;
      }
    }

    checkForError( "vk.xml",
                   value.bitPos.empty() ^ value.value.empty(),
                   value.xmlLine,
                   "both or none of \"bitPos\" and \"value\" are set for bitmask enum <" + value.name + "> which is invalid" );

    return value;
  }
}

void parseCommand( tinyxml2::XMLElement const * element, std::vector<Command> & commands )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );

  if ( attributes.contains( "alias" ) )
  {
    checkAttributes( "vk.xml", line, attributes, { { "alias", {} }, { "name", {} } }, {} );
    checkElements( "vk.xml", line, getChildElements( element ), {} );

    std::string alias, name;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "alias" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        alias = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        name = attribute.second;
      }
    }

    auto commandIt = findByName( commands, alias );
    checkForError( "vk.xml", commandIt != commands.end(), line, "command <" + name + "> is aliased to unknown command <" + alias + ">" );
    checkForError( "vk.xml",
                   std::find_if( std::next( commandIt ), commands.end(), [&alias]( Command const & c ) { return c.name == alias; } ) == commands.end(),
                   line,
                   "command <" + name + "> is aliased to multiply specfied command <" + alias + ">" );
    checkForError(
      "vk.xml", commandIt->aliases.insert( { name, line } ).second, line, "command <" + name + "> is already listed as alias for command <" + alias + ">" );
  }
  else
  {
    checkAttributes( "vk.xml",
                     line,
                     attributes,
                     {},
                     { { "allownoqueues", { "true" } },
                       { "api", { "vulkan", "vulkanbase", "vulkansc" } },
                       { "cmdbufferlevel", { "primary", "secondary" } },
                       { "comment", {} },
                       { "conditionalrendering", { "false", "true" } },
                       { "errorcodes", {} },
                       { "export", { "vulkan", "vulkansc" } },
                       { "queues", {} },
                       { "renderpass", { "both", "inside", "outside" } },
                       { "successcodes", {} },
                       { "tasks", { "action", "indirection", "state", "synchronization" } },
                       { "videocoding", { "both", "inside", "outside" } } } );

    std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
    checkElements( "vk.xml",
                   line,
                   children,
                   { { "param", MultipleAllowed::Yes }, { "proto", MultipleAllowed::No } },
                   { { "implicitexternsyncparams", MultipleAllowed::No } } );

    Command command{ .xmlLine = line };
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "allownoqueues" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        command.allowNoQueues = attribute.second;
      }
      else if ( attribute.first == "api" )
      {
        command.api = tokenize( attribute.second, "," );
      }
      else if ( attribute.first == "cmdbufferlevel" )
      {
        command.cmdBufferLevel = tokenize( attribute.second, "," );
      }
      else if ( attribute.first == "conditionalrendering" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        command.conditionalRendering = attribute.second;
      }
      else if ( attribute.first == "errorcodes" )
      {
        command.errorCodes = tokenize( attribute.second, "," );
      }
      else if ( attribute.first == "export" )
      {
        command.exports = tokenize( attribute.second, "," );
      }
      else if ( attribute.first == "queues" )
      {
        command.queues = tokenize( attribute.second, "," );
      }
      else if ( attribute.first == "tasks" )
      {
        command.tasks = tokenize( attribute.second, "," );
      }
      else if ( attribute.first == "renderpass" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        command.renderPass = attribute.second;
      }
      else if ( attribute.first == "successcodes" )
      {
        command.successCodes = tokenize( attribute.second, "," );
      }
      else if ( attribute.first == "videocoding" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        command.videoCoding = attribute.second;
      }
    }

    for ( auto child : children )
    {
      std::string value = child->Value();
      if ( value == "implicitexternsyncparams" )
      {
        parseImplicitExternSyncParams( child );
      }
      else if ( value == "param" )
      {
        Param param   = parseParam( child );
        auto  paramIt = findByName( command.params, param.name );
        if ( paramIt == command.params.end() )
        {
          checkForError( "vk.xml",
                         !containsByName( command.params, param.name ),
                         param.xmlLine,
                         "parameter <" + param.name + "> specified multiple times for command <" + command.name + ">" );
          command.params.push_back( std::move( param ) );
        }
        else
        {
          checkForError( "vk.xml",
                         param.api != paramIt->api,
                         param.xmlLine,
                         "command <" + command.name + "> has the parameter <" + param.name + "> multiply defined for the same set of apis" );
          std::vector<std::string> api = param.api;
          param.api                    = paramIt->api;
          checkForError( "vk.xml",
                         param == *paramIt,
                         param.xmlLine,
                         "command <" + command.name + "> has the parameter <" + param.name + "> multiply defined with different attributes" );
        }
      }
      else if ( value == "proto" )
      {
        std::tie( command.name, command.returnType ) = parseProto( child );
      }
    }

    auto paramIt = std::ranges::find_if( command.params, []( auto const & param ) { return !param.altLen.empty(); } );
    if ( paramIt != command.params.end() )
    {
      checkForError( "vk.xml",
                     paramIt->len.starts_with( "latexmath:" ),
                     paramIt->xmlLine,
                     "parameter <" + paramIt->name + "> has attribute <altLen> but attribute <len> does not start with \"latexmath:\"" );
    }
    paramIt = std::ranges::find_if( command.params, []( auto const & param ) { return param.len.starts_with( "latexmath:" ); } );
    if ( paramIt != command.params.end() )
    {
      checkForError( "vk.xml",
                     !paramIt->altLen.empty(),
                     paramIt->xmlLine,
                     "parameter <" + paramIt->name + "> has attribute <len> starting with \"latexmath:\" but no attribute <altLen>" );
    }
    for ( auto const & param : command.params )
    {
      checkForError( "vk.xml",
                     param.stride.empty() || containsByName( command.params, param.stride ),
                     param.xmlLine,
                     "parameter <" + param.name + "> has unknown stride <" + param.stride + ">" );
    }

    checkForError( "vk.xml",
                   ( command.returnType.name == "VkResult" ) || command.errorCodes.empty(),
                   line,
                   "command <" + command.name + "> does not return a VkResult but specifies errorcodes" );
    checkForError( "vk.xml",
                   ( command.returnType.name == "VkResult" ) || command.successCodes.empty(),
                   line,
                   "command <" + command.name + "> does not return a VkResult but specifies successcodes" );
    checkForError(
      "vk.xml",
      command.api.empty() || command.exports.empty() ||
        ( ( command.exports.size() == 1 ) && std::ranges::any_of( command.api, [&command]( auto const & a ) { return a == command.exports.front(); } ) ),
      line,
      "command <" + command.name + "> has disjunct attributes <api> and <export>" );

    checkForError( "vk.xml", !containsByNameAndExport( commands, command.name, command.exports ), line, "command <" + command.name + "> already specified" );
    commands.push_back( std::move( command ) );
    // CHECK: errorcodes, queues, successcodes after extensions
  }
}

std::vector<Command> parseCommands( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, { { "comment", {} } } );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "command", MultipleAllowed::Yes } } );

  std::vector<Command> commands;
  for ( auto child : children )
  {
    parseCommand( child, commands );
  }

  return commands;
}

Component parseComponent( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml",
                   line,
                   attributes,
                   { { "bits", {} },
                     { "name", {} },
                     { "numericFormat", { "BOOL", "SINT", "SFIXED5", "SFLOAT", "SNORM", "SRGB", "SSCALED", "UFLOAT", "UINT", "UNORM", "USCALED" } } },
                   { { "planeIndex", { "0", "1", "2" } } } );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  Component component{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "bits" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      checkForError( "vk.xml",
                     isNumber( attribute.second ) || ( attribute.second == "compressed" ),
                     line,
                     "unexpected value for attribute \"bits\": <" + attribute.second + ">" );
      component.bits = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      component.name = attribute.second;
    }
    else if ( attribute.first == "numericFormat" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      component.numericFormat = attribute.second;
    }
    else if ( attribute.first == "planeIndex" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      component.planeIndex = attribute.second;
    }
  }
  return component;
}

ConstantValue parseConstantValue( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} }, { "type", {} }, { "value", {} } }, { { "comment", {} } } );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  ConstantValue constant{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      constant.name = attribute.second;
    }
    else if ( attribute.first == "type" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      constant.type = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      constant.value = attribute.second;
    }
  }
  return constant;
}

Deprecate parseDeprecate( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "explanationlink", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, {}, { { "command", MultipleAllowed::Yes }, { "feature", MultipleAllowed::Yes }, { "type", MultipleAllowed::Yes } } );

  Deprecate deprecate{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "explanationlink" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      deprecate.explanationLink = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "command" )
    {
      SupersededName deprecateCommand = parseSupersededName( child );
      checkForError( "vk.xml",
                     !containsByName( deprecate.commands, deprecateCommand.name ),
                     line,
                     "deprecate command <" + deprecateCommand.name + "> already listed for this deprecate block" );
      deprecate.commands.push_back( std::move( deprecateCommand ) );
    }
    else if ( value == "feature" )
    {
      deprecate.features.push_back( parseFeatureElement( child ) );
    }
    else if ( value == "type" )
    {
      SupersededName deprecateType = parseSupersededName( child );
      checkForError( "vk.xml",
                     !containsByName( deprecate.types, deprecateType.name ),
                     line,
                     "deprecate type <" + deprecateType.name + "> already listed for this deprecate block" );
      deprecate.types.push_back( std::move( deprecateType ) );
    }
  }

  return deprecate;
}

EnumsVariant parseEnums( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );

  auto typeIt = attributes.find( "type" );
  checkForError( "vk.xml", typeIt != attributes.end(), line, "enum is missing attribute \"type\"" );
  if ( typeIt->second == "bitmask" )
  {
    return parseEnumsBitmask( element, attributes );
  }
  else if ( typeIt->second == "constants" )
  {
    return parseEnumsConstants( element, attributes );
  }
  else
  {
    assert( typeIt->second == "enum" );
    return parseEnumsEnum( element, attributes );
  }
}

EnumsBitmask parseEnumsBitmask( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, { { "bitwidth", { "64" } }, { "comment", {} }, { "type", { "bitmask" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, {}, { { "comment", MultipleAllowed::No }, { "enum", MultipleAllowed::Yes } } );

  EnumsBitmask bitmask{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "bitwidth" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      bitmask.bitWidth = attribute.second;
    }
    else if ( attribute.first == "comment" )
    {
      bitmask.comment = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      bitmask.name = attribute.second;
    }
  }

  for ( auto const & child : children )
  {
    std::string value = child->Value();
    if ( value == "comment" )
    {
      Comment comment = parseComment( "vk.xml", child );
    }
    else if ( value == "enum" )
    {
      auto bitmaskValue = parseBitmaskValue( child );
      if ( std::holds_alternative<EnumValueAlias>( bitmaskValue ) )
      {
        auto const & alias   = std::get<EnumValueAlias>( bitmaskValue );
        auto         valueIt = findByName( bitmask.values, alias.alias );
        checkForError(
          "vk.xml", valueIt != bitmask.values.end(), alias.xmlLine, "bitmask value alias <" + alias.alias + "> not found for bitmask <" + bitmask.name + ">" );
        checkForError( "vk.xml",
                       !containsByName( valueIt->aliases, alias.name ),
                       alias.xmlLine,
                       "bitmask value alias <" + alias.alias + "> already specified for bitmask value <" + alias.name + "> in bitmask <" + bitmask.name + ">" );
        valueIt->aliases.push_back( std::move( alias ) );
      }
      else
      {
        assert( std::holds_alternative<BitmaskValueRegular>( bitmaskValue ) );

        auto const & regularValue = std::get<BitmaskValueRegular>( bitmaskValue );
        checkForError( "vk.xml",
                       !containsByName( bitmask.values, regularValue.name ),
                       regularValue.xmlLine,
                       "bitmask value <" + regularValue.name + "> already specified for bitmask <" + bitmask.name + ">" );
        bitmask.values.push_back( { .name    = regularValue.name,
                                    .bitPos  = regularValue.bitPos,
                                    .comment = regularValue.comment,
                                    .value   = regularValue.value,
                                    .xmlLine = regularValue.xmlLine } );
      }
    }
  }

  return bitmask;
}

EnumsConstants parseEnumsConstants( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, attributes, { { "comment", {} }, { "name", {} } }, { { "type", { "constants" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, {}, { { "enum", MultipleAllowed::Yes } } );

  EnumsConstants constants{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      constants.comment = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      constants.name = attribute.second;
    }
  }

  for ( auto const & child : children )
  {
    std::string value = child->Value();
    if ( value == "enum" )
    {
      auto constant = parseConstantValue( child );
      checkForError( "vk.xml",
                     !containsByName( constants.values, constant.name ),
                     constant.xmlLine,
                     "constant value <" + constant.name + "> already specified for constants <" + constants.name + ">" );
      constants.values.push_back( std::move( constant ) );
    }
  }

  return constants;
}

EnumsEnum parseEnumsEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, { { "comment", {} }, { "type", { "enum" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, {}, { { "comment", MultipleAllowed::Yes }, { "enum", MultipleAllowed::Yes }, { "unused", MultipleAllowed::No } } );

  EnumsEnum enumsEnum{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      enumsEnum.comment = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      enumsEnum.name = attribute.second;
    }
  }

  for ( auto const & child : children )
  {
    std::string value = child->Value();
    if ( value == "comment" )
    {
      Comment comment = parseComment( "vk.xml", child );
    }
    else if ( value == "enum" )
    {
      auto enumValue = parseEnumValue( child );
      if ( std::holds_alternative<EnumValueAlias>( enumValue ) )
      {
        auto const & alias   = std::get<EnumValueAlias>( enumValue );
        auto         valueIt = findByName( enumsEnum.values, alias.alias );
        checkForError(
          "vk.xml", valueIt != enumsEnum.values.end(), alias.xmlLine, "enum value alias <" + alias.alias + "> not found for enum <" + enumsEnum.name + ">" );
        checkForError( "vk.xml",
                       !containsByName( valueIt->aliases, alias.name ),
                       alias.xmlLine,
                       "enum value alias <" + alias.name + "> already specified for enum value <" + alias.name + "> in enum <" + enumsEnum.name + ">" );
        valueIt->aliases.push_back( std::move( alias ) );
      }
      else
      {
        assert( std::holds_alternative<EnumValueRegular>( enumValue ) );

        auto const & simpleValue = std::get<EnumValueRegular>( enumValue );
        checkForError( "vk.xml",
                       !containsByName( enumsEnum.values, simpleValue.name ),
                       simpleValue.xmlLine,
                       "enum value <" + simpleValue.name + "> already specified for enum <" + enumsEnum.name + ">" );
        enumsEnum.values.push_back( { .name = simpleValue.name, .comment = simpleValue.comment, .value = simpleValue.value, .xmlLine = simpleValue.xmlLine } );
      }
    }
    else if ( value == "unused" )
    {
      EnumUnused unused = parseEnumUnused( child );
    }
  }

  return enumsEnum;
}

Extension parseExtension( tinyxml2::XMLElement const * element )
{
  int const                                 line       = element->GetLineNum();
  std::map<std::string, std::string>        attributes = getAttributes( element );
  std::vector<tinyxml2::XMLElement const *> children   = getChildElements( element );
  checkElements(
    "vk.xml", line, children, { { "require", MultipleAllowed::Yes } }, { { "deprecate", MultipleAllowed::Yes }, { "remove", MultipleAllowed::No } } );

  auto supportedIt = attributes.find( "supported" );
  assert( supportedIt != attributes.end() );
  if ( supportedIt->second == "disabled" )
  {
    checkAttributes( "vk.xml",
                     line,
                     attributes,
                     { { "name", {} }, { "number", {} }, { "supported", { "disabled" } } },
                     { { "author", {} },
                       { "comment", {} },
                       { "contact", {} },
                       { "depends", {} },
                       { "nofeatures", { "true" } },
                       { "platform", {} },
                       { "ratified", { "vulkan" } },
                       { "specialuse", { "glemulation" } },
                       { "type", { "device", "instance" } } } );
  }
  else
  {
    checkAttributes( "vk.xml",
                     line,
                     attributes,
                     { { "author", {} },
                       { "contact", {} },
                       { "name", {} },
                       { "number", {} },
                       { "supported", { "vulkan", "vulkansc" } },
                       { "type", { "device", "instance" } } },
                     { { "comment", {} },
                       { "depends", {} },
                       { "deprecatedby", {} },
                       { "nofeatures", { "true" } },
                       { "obsoletedby", {} },
                       { "platform", {} },
                       { "promotedto", {} },
                       { "provisional", { "true" } },
                       { "ratified", { "vulkan", "vulkansc" } },
                       { "sortorder", { "1" } },
                       { "specialuse", { "cadsupport", "d3demulation", "debugging", "devtools", "glemulation" } } } );
  }

  Extension extension{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "author" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      extension.author = attribute.second;
    }
    else if ( attribute.first == "comment" )
    {
      extension.comment = attribute.second;
    }
    else if ( attribute.first == "contact" )
    {
      extension.contacts = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "depends" )
    {
      DependencyParser                      dependencyParser( attribute.second );
      std::vector<std::vector<std::string>> dependencies = vectorize( normalize( dependencyParser.parse() ) );
      normalizeVersion( dependencies );
      for ( auto & dep : dependencies )
      {
        auto it = extension.depends.insert( { dep[0], {} } ).first;
        it->second.push_back( {} );
        for ( auto depIt = std::next( dep.begin() ); depIt != dep.end(); ++depIt )
        {
          it->second.back().insert( *depIt );
        }
      }
    }
    else if ( attribute.first == "deprecatedby" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      extension.deprecatedBy = attribute.second;
      extension.isDeprecated = true;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      extension.name = attribute.second;
    }
    else if ( attribute.first == "nofeatures" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      extension.noFeatures = ( attribute.second == "true" );
    }
    else if ( attribute.first == "number" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      checkForError( "vk.xml", isNumber( attribute.second ), line, "extension attribute \"number\" is not a number: <" + attribute.second + ">" );
      extension.number = attribute.second;
    }
    else if ( attribute.first == "obsoletedby" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      extension.obsoletedBy = attribute.second;
    }
    else if ( attribute.first == "platform" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      extension.platform = attribute.second;
    }
    else if ( attribute.first == "promotedto" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      extension.promotedTo = attribute.second;
    }
    else if ( attribute.first == "provisional" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      extension.provisional = ( attribute.second == "true" );
      checkForError( "vk.xml",
                     !extension.provisional || ( extension.platform == "provisional" ),
                     line,
                     "extension <" + extension.name + "> is marked as provisional but the platform <" + extension.platform + "> is not \"provisional\"" );
    }
    else if ( attribute.first == "ratified" )
    {
      extension.ratified = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "specialuse" )
    {
      extension.specialUse = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "sortorder" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      checkForError( "vk.xml", isNumber( attribute.second ), line, "extension attribute \"sortorder\" is not a number: <" + attribute.second + ">" );
      extension.sortOrder = std::stoi( attribute.second );
    }
    else if ( attribute.first == "supported" )
    {
      extension.supported = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "type" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      extension.type = attribute.second;
    }
  }

  checkForError( "vk.xml",
                 ( ( extension.supported.size() == 1 ) && ( extension.supported[0] == "disabled" ) ) ||
                   std::ranges::all_of( extension.ratified,
                                        [&supported = extension.supported]( auto const & ratified )
                                        { return std::ranges::any_of( supported, [&ratified]( auto const & supported ) { return ratified == supported; } ); } ),
                 line,
                 "extension <" + extension.name + "> is ratified for <" + concatenate( extension.ratified ) + "> but not all of them are supported <" +
                   concatenate( extension.supported ) + ")" );

  for ( auto const & child : children )
  {
    std::string value = child->Value();
    if ( value == "deprecate" )
    {
      Deprecate deprecate = parseDeprecate( child );
      for ( auto const & deprecateCommand : deprecate.commands )
      {
        checkForError( "vk.xml",
                       std::ranges::none_of( extension.deprecates,
                                             [&deprecateCommand]( auto const & deprecate )
                                             { return containsByName( deprecate.commands, deprecateCommand.name ); } ),
                       deprecateCommand.xmlLine,
                       "deprecate command <" + deprecateCommand.name + "> already listed as deprecated for extension <" + extension.name + ">" );
      }
      for ( auto const & deprecateType : deprecate.types )
      {
        checkForError( "vk.xml",
                       std::ranges::none_of( extension.deprecates,
                                             [&deprecateType]( auto const & deprecate ) { return containsByName( deprecate.types, deprecateType.name ); } ),
                       deprecateType.xmlLine,
                       "deprecate type <" + deprecateType.name + "> already listed as deprecated for extension <" + extension.name + ">" );
      }
      extension.deprecates.push_back( std::move( deprecate ) );
    }
    else if ( value == "remove" )
    {
      extension.removes.push_back( parseExtensionRemove( child ) );
    }
    else if ( value == "require" )
    {
      ExtensionRequire require         = parseExtensionRequire( child );
      auto             extensionNameIt = std::ranges::find_if( require.enums, []( auto const & e ) { return e.name.ends_with( "_EXTENSION_NAME" ); } );
      if ( extensionNameIt != require.enums.end() )
      {
        checkForError( "vk.xml",
                       extension.require.empty(),
                       extensionNameIt->xmlLine,
                       "extension name enum <" + extensionNameIt->name + "> encountered in second or later require section" );
      }
      for ( auto const & alreadyRequired : extension.require )
      {
        for ( auto const & command : require.commands )
        {
          checkForError( "vk.xml",
                         !containsByName( alreadyRequired.commands, command.name ),
                         command.xmlLine,
                         "command <" + command.name + "> already listed as required for extension <" + extension.name + ">" );
        }
        if ( alreadyRequired.depends == require.depends )
        {
          for ( auto const & e : require.enums )
          {
            checkForError( "vk.xml",
                           !containsByName( alreadyRequired.enums, e.name ),
                           e.xmlLine,
                           "require enum <" + e.name + "> already listed as required for extension <" + extension.name + ">" );
          }
          for ( auto const & feature : require.features )
          {
            for ( auto const & featureName : feature.names )
            {
              checkForError( "vk.xml",
                             std::ranges::none_of( alreadyRequired.features,
                                                   [&feature, &featureName]( MultiFeatureElement const & alreadyRequiredFeature )
                                                   {
                                                     return std::ranges::any_of( alreadyRequiredFeature.names,
                                                                                 [&featureName]( auto const & alreadyRequiredFeatureName )
                                                                                 { return alreadyRequiredFeatureName == featureName; } ) &&
                                                            ( alreadyRequiredFeature.structure == feature.structure );
                                                   } ),
                             feature.xmlLine,
                             "require feature <" + featureName + "> with struct <" + feature.structure + "> already listed as required for extension <" +
                               extension.name + ">" );
            }
          }
        }
        for ( auto const & type : require.types )
        {
          checkForError( "vk.xml",
                         !containsByName( alreadyRequired.types, type.name ),
                         require.xmlLine,
                         "type <" + type.name + "> already listed as required for extension <" + extension.name + ">" );
        }
      }
      extension.require.push_back( std::move( require ) );
    }
  }

  return extension;
}

EnumUnused parseEnumUnused( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "comment", {} }, { "start", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  EnumUnused unused{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      unused.comment = attribute.second;
    }
    else if ( attribute.first == "start" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      checkForError( "vk.xml",
                     isSignedNumber( attribute.second ) || isHexNumber( attribute.second ),
                     unused.xmlLine,
                     "unexpected format of unused attribute start <" + attribute.second + ">" );
      unused.start = attribute.second;
    }
  }

  return unused;
}

EnumValueVariant parseEnumValue( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  if ( attributes.contains( "alias" ) )
  {
    checkAttributes( "vk.xml", line, attributes, { { "alias", {} }, { "name", {} } }, { { "api", { "vulkan" } }, { "deprecated", { "aliased" } } } );

    EnumValueAlias alias{ .xmlLine = line };
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "alias" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        alias.alias = attribute.second;
      }
      else if ( attribute.first == "api" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        alias.api = attribute.second;
      }
      else if ( attribute.first == "deprecated" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        alias.deprecated = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        alias.name = attribute.second;
      }
    }
    return alias;
  }
  else
  {
    checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, { { "comment", {} }, { "value", {} } } );

    EnumValueRegular value{ .xmlLine = line };
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "comment" )
      {
        value.comment = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        value.name = attribute.second;
      }
      else if ( attribute.first == "value" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        value.value = attribute.second;
      }
    }

    return value;
  }
}

ExtensionRemove parseExtensionRemove( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, {}, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "feature", MultipleAllowed::No } }, {} );

  ExtensionRemove remove{ .xmlLine = line };

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "feature" )
    {
      remove.feature = parseFeatureElement( child );
    }
  }

  return remove;
}

ExtensionRequire parseExtensionRequire( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, {}, { { "api", { "vulkansc" } }, { "comment", {} }, { "depends", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml",
                 line,
                 children,
                 {},
                 { { "command", MultipleAllowed::Yes },
                   { "comment", MultipleAllowed::Yes },
                   { "enum", MultipleAllowed::Yes },
                   { "feature", MultipleAllowed::Yes },
                   { "type", MultipleAllowed::Yes } } );

  ExtensionRequire require{ .xmlLine = line };
  std::string      att;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "api" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      require.api = attribute.second;
    }
    else if ( attribute.first == "comment" )
    {
      require.comment = attribute.second;
    }
    else if ( attribute.first == "depends" )
    {
      require.depends = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "command" )
    {
      NameElement requireCommand = parseNameElement( child );
      checkForError( "vk.xml",
                     !containsByName( require.commands, requireCommand.name ),
                     requireCommand.xmlLine,
                     "require command <" + requireCommand.name + "> already listed for this require block" );
      require.commands.push_back( std::move( requireCommand ) );
    }
    else if ( value == "enum" )
    {
      ExtensionRequireEnum requireEnum = parseExtensionRequireEnum( child );
      checkForError( "vk.xml",
                     !containsByName( require.enums, requireEnum.name ),
                     requireEnum.xmlLine,
                     "require enum <" + requireEnum.name + "> already listed for this require block" );
      checkForError( "vk.xml",
                     !requireEnum.alias.empty() || !requireEnum.name.ends_with( "_EXTENSION_NAME" ) ||
                       std::ranges::none_of( require.enums, []( auto const & e ) { return e.name.ends_with( "_EXTENSION_NAME" ); } ),
                     requireEnum.xmlLine,
                     "extension name enum <" + requireEnum.name + "> is not the first enum ending with \"_EXTENSION_NAME\"" );
      require.enums.push_back( std::move( requireEnum ) );
    }
    else if ( value == "feature" )
    {
      MultiFeatureElement requireFeature = parseMultiFeatureElement( child );
      for ( auto const & name : requireFeature.names )
      {
        checkForError( "vk.xml",
                       std::ranges::none_of( require.features,
                                             [&requireFeature, &name]( MultiFeatureElement const & feature )
                                             {
                                               return std::ranges::any_of( feature.names,
                                                                           [&name]( auto const & featureName ) { return featureName == name; } ) &&
                                                      ( feature.structure == requireFeature.structure );
                                             } ),
                       requireFeature.xmlLine,
                       "require feature <" + name + "> with struct <" + requireFeature.structure + "> already listed for this require block" );
      }
      require.features.push_back( std::move( requireFeature ) );
    }
    else if ( value == "type" )
    {
      NameElement requireType = parseNameElement( child );
      checkForError( "vk.xml",
                     !containsByName( require.types, requireType.name ),
                     requireType.xmlLine,
                     "require type <" + requireType.name + "> already listed for this require block" );
      require.types.push_back( std::move( requireType ) );
    }
  }

  return require;
}

ExtensionRequireEnum parseExtensionRequireEnum( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  if ( attributes.contains( "alias" ) )
  {
    checkAttributes(
      "vk.xml",
      line,
      attributes,
      { { "alias", {} }, { "name", {} } },
      { { "api", { "vulkan" } }, { "comment", {} }, { "deprecated", { "aliased" } }, { "extends", {} }, { "protect", { "VK_ENABLE_BETA_EXTENSIONS" } } } );
  }
  else if ( attributes.contains( "bitpos" ) )
  {
    checkAttributes( "vk.xml",
                     line,
                     attributes,
                     { { "bitpos", {} }, { "extends", {} }, { "name", {} } },
                     { { "comment", {} }, { "protect", { "VK_ENABLE_BETA_EXTENSIONS" } } } );
  }
  else if ( attributes.contains( "offset" ) )
  {
    checkAttributes(
      "vk.xml",
      line,
      attributes,
      { { "extends", {} }, { "name", {} }, { "offset", {} } },
      { { "comment", {} }, { "dir", { "-" } }, { "deprecated", { "true" } }, { "extnumber", {} }, { "protect", { "VK_ENABLE_BETA_EXTENSIONS" } } } );
  }
  else if ( attributes.contains( "value" ) )
  {
    checkAttributes( "vk.xml", line, attributes, { { "name", {} }, { "value", {} } }, { { "comment", {} }, { "extends", {} } } );
  }
  else
  {
    checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, {} );
  }
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  ExtensionRequireEnum requireEnum{ .xmlLine = line };
  std::string          attr;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      requireEnum.alias = attribute.second;
    }
    else if ( attribute.first == "api" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      requireEnum.api = attribute.second;
    }
    else if ( attribute.first == "bitpos" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      checkForError( "vk.xml", isNumber( attribute.second ), line, "require enum with non-numeric bitpos <" + attribute.second + ">" );
      requireEnum.bitPos = attribute.second;
    }
    else if ( attribute.first == "comment" )
    {
      requireEnum.comment = attribute.second;
    }
    else if ( attribute.first == "deprecated" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      requireEnum.deprecated = attribute.second;
    }
    else if ( attribute.first == "dir" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      requireEnum.dir = attribute.second;
    }
    else if ( attribute.first == "extends" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      requireEnum.extends = attribute.second;
    }
    else if ( attribute.first == "extnumber" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      checkForError( "vk.xml", isNumber( attribute.second ), line, "require enum with non-numeric extnumber <" + attribute.second + ">" );
      requireEnum.extNumber = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      requireEnum.name = attribute.second;
    }
    else if ( attribute.first == "offset" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      checkForError( "vk.xml", isNumber( attribute.second ), line, "require enum with non-numeric offset <" + attribute.second + ">" );
      requireEnum.offset = attribute.second;
    }
    else if ( attribute.first == "protect" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      requireEnum.protect = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      checkForError( "vk.xml",
                     requireEnum.name.ends_with( "_EXTENSION_NAME" ) || isSignedNumber( attribute.second ),
                     line,
                     "require enum with non-numeric value <" + attribute.second + ">" );
      requireEnum.value = attribute.second;
    }
  }

  return requireEnum;
}

Extensions parseExtensions( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "extension", MultipleAllowed::Yes } } );

  Extensions extensions{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      extensions.comment = attribute.second;
    }
  }

  for ( auto const & child : children )
  {
    std::string value = child->Value();
    if ( value == "extension" )
    {
      Extension extension = parseExtension( child );
      checkForError(
        "vk.xml", !containsByName( extensions.extensions, extension.name ), extension.xmlLine, "extension <" + extension.name + "> already specified" );
      checkForError( "vk.xml",
                     std::ranges::none_of( extensions.extensions, [&extension]( auto const & e ) { return e.number == extension.number; } ),
                     extension.xmlLine,
                     "extension <" + extension.name + "> has the same number <" + extension.number + "> as another extension" );
      for ( auto const & deprecate : extension.deprecates )
      {
        for ( auto const & deprecateCommand : deprecate.commands )
        {
          checkForError( "vk.xml",
                         std::ranges::none_of( extensions.extensions,
                                               [&deprecateCommand]( auto const & e )
                                               {
                                                 return std::ranges::any_of( e.deprecates,
                                                                             [&deprecateCommand]( auto const & deprecate )
                                                                             { return containsByName( deprecate.commands, deprecateCommand.name ); } );
                                               } ),
                         deprecateCommand.xmlLine,
                         "extension <" + extension.name + "> deprecates command <" + deprecateCommand.name +
                           "> which is already deprecated by another extension" );
        }
        for ( auto const & deprecateType : deprecate.types )
        {
          checkForError( "vk.xml",
                         std::ranges::none_of( extensions.extensions,
                                               [&deprecateType]( auto const & e )
                                               {
                                                 return std::ranges::any_of( e.deprecates,
                                                                             [&deprecateType]( auto const & deprecate )
                                                                             { return containsByName( deprecate.types, deprecateType.name ); } );
                                               } ),
                         deprecateType.xmlLine,
                         "extension <" + extension.name + "> deprecates type <" + deprecateType.name + "> which is already deprecated by another extension" );
        }
      }
      extensions.extensions.push_back( std::move( extension ) );
    }
  }

  // filter out dependencies that are already promoted to a version less or equal to the base version of the dependency set
  // also filter out dependency sets that are equal to a previous dependency set
  for ( auto & extension : extensions.extensions )
  {
    for ( auto & depend : extension.depends )
    {
      for ( auto depSetIt = depend.second.begin(); depSetIt != depend.second.end(); )
      {
        for ( auto depIt = depSetIt->begin(); depIt != depSetIt->end(); )
        {
          // dependency extensions that are promoted to a version less or equal to the base version of this dependency set can be erased
          auto extIt = std::ranges::find_if( extensions.extensions, [depIt]( auto const & e ) { return e.name == *depIt; } );
          checkForError( "vk.xml",
                         extIt != extensions.extensions.end(),
                         extension.xmlLine,
                         "extension <" + extension.name + "> depends on unknown extension <" + *depIt + ">" );
          if ( ( extIt->promotedTo.starts_with( "VK_VERSION_" ) ) && ( extIt->promotedTo <= depend.first ) )
          {
            depIt = depSetIt->erase( depIt );
          }
          else
          {
            ++depIt;
          }
        }
        // if any of the previous dependency sets equals the current one, it can be erased
        if ( std::any_of( depend.second.begin(), depSetIt, [depSetIt]( std::set<std::string> const & depSet ) { return depSet == *depSetIt; } ) )
        {
          depSetIt = depend.second.erase( depSetIt );
        }
        else
        {
          ++depSetIt;
        }
      }
    }
  }

  return extensions;
}

Feature parseFeature( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml",
                   line,
                   attributes,
                   { { "api", { "vulkan", "vulkanbase", "vulkansc" } }, { "comment", {} }, { "name", {} }, { "number", {} } },
                   { { "apitype", { "internal" } }, { "depends", {} } } );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements(
    "vk.xml", line, children, { { "require", MultipleAllowed::Yes } }, { { "deprecate", MultipleAllowed::Yes }, { "remove", MultipleAllowed::Yes } } );

  Feature feature{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "api" )
    {
      feature.api = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "apitype" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      feature.apiType = attribute.second;
    }
    else if ( attribute.first == "comment" )
    {
      feature.comment = attribute.second;
    }
    else if ( attribute.first == "depends" )
    {
      // Feature depends uses '+' (AND) per the dependency syntax, but ',' was used historically with the same meaning.
      // Accept both so this works with both old (comma-separated) and new (plus-separated) XML.
      feature.depends = tokenizeAny( attribute.second, ",+" );
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      feature.name = attribute.second;
    }
    else if ( attribute.first == "number " )
    {
      checkNoList( "vk.xml", attribute.second, line );
      checkForError( "vk.xml",
                     ( attribute.second.length() == 3 ) && std::isdigit( attribute.second[0] ) && ( attribute.second[1] == '.' ) &&
                       std::isdigit( attribute.second[2] ),
                     line,
                     "feature <" + feature.name + "> has ill-formatted attribute number = \"" + attribute.second + "\"" );
      std::string nameEnd = attribute.second;
      nameEnd[1]          = '_';
      checkForError(
        "vk.xml", feature.name.ends_with( nameEnd ), line, "feature <" + feature.name + "> does not end with the expected number <" + nameEnd + ">" );
      feature.number = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "deprecate" )
    {
      Deprecate deprecate = parseDeprecate( child );
      for ( auto const & deprecateCommand : deprecate.commands )
      {
        checkForError( "vk.xml",
                       std::ranges::none_of( feature.deprecates,
                                             [&deprecateCommand]( auto const & deprecate )
                                             { return containsByName( deprecate.commands, deprecateCommand.name ); } ),
                       deprecateCommand.xmlLine,
                       "deprecate command <" + deprecateCommand.name + "> already listed as deprecated for feature <" + feature.name + ">" );
      }
      checkForWarning( "vk.xml", deprecate.features.empty(), deprecate.xmlLine, "feature <" + feature.name + "> has deprecated features" );
      for ( auto const & deprecateType : deprecate.types )
      {
        checkForError( "vk.xml",
                       std::ranges::none_of( feature.deprecates,
                                             [&deprecateType]( auto const & deprecate ) { return containsByName( deprecate.types, deprecateType.name ); } ),
                       deprecateType.xmlLine,
                       "deprecate type <" + deprecateType.name + "> already listed as deprecated for feature <" + feature.name + ">" );
      }
      feature.deprecates.push_back( std::move( deprecate ) );
    }
    else if ( value == "remove" )
    {
      Remove remove = parseRemove( child );
      for ( auto const & removeCommand : remove.commands )
      {
        checkForError(
          "vk.xml",
          std::ranges::none_of( feature.removes, [&removeCommand]( auto const & remove ) { return containsByName( remove.commands, removeCommand.name ); } ),
          removeCommand.xmlLine,
          "command <" + removeCommand.name + "> already listed as removed for feature <" + feature.name + ">" );
      }
      for ( auto const & removeEnum : remove.enums )
      {
        checkForError(
          "vk.xml",
          std::ranges::none_of( feature.removes, [&removeEnum]( auto const & remove ) { return containsByName( remove.enums, removeEnum.name ); } ),
          removeEnum.xmlLine,
          "remove enum <" + removeEnum.name + "> already listed as removed for feature <" + feature.name + ">" );
      }
      for ( auto const & removeFeature : remove.features )
      {
        checkForError( "vk.xml",
                       std::ranges::none_of( feature.removes,
                                             [&removeFeature]( auto const & remove )
                                             {
                                               return std::ranges::any_of(
                                                 remove.features,
                                                 [&removeFeature]( FeatureElement const & feature )
                                                 { return ( feature.name == removeFeature.name ) && ( feature.structure == removeFeature.structure ); } );
                                             } ),
                       removeFeature.xmlLine,
                       "feature <" + removeFeature.name + "> with struct <" + removeFeature.structure + "> already listed as removed for feature <" +
                         feature.name + ">" );
      }
      for ( auto const & removeType : remove.types )
      {
        checkForError(
          "vk.xml",
          std::ranges::none_of( feature.removes, [&removeType]( auto const & remove ) { return containsByName( remove.types, removeType.name ); } ),
          removeType.xmlLine,
          "type <" + removeType.name + "> already listed as removed for feature <" + feature.name + ">" );
      }
      feature.removes.push_back( std::move( remove ) );
    }
    else if ( value == "require" )
    {
      Require require = parseRequire( child );
      for ( auto const & requireCommand : require.commands )
      {
        checkForError( "vk.xml",
                       std::ranges::none_of( feature.require,
                                             [&requireCommand]( auto const & require ) { return containsByName( require.commands, requireCommand.name ); } ),
                       require.xmlLine,
                       "command <" + requireCommand.name + "> already listed as required for feature <" + feature.name + ">" );
      }
      for ( auto const & requireEnum : require.enums )
      {
        checkForError(
          "vk.xml",
          std::ranges::none_of( feature.require, [&requireEnum]( auto const & require ) { return containsByName( require.enums, requireEnum.name ); } ),
          require.xmlLine,
          "enum <" + requireEnum.name + "> already listed as required for feature <" + feature.name + ">" );
      }
      for ( auto const & requireFeature : require.features )
      {
        checkForError( "vk.xml",
                       std::ranges::none_of( feature.require,
                                             [&requireFeature]( auto const & require )
                                             {
                                               return std::ranges::any_of(
                                                 require.features,
                                                 [&requireFeature]( FeatureElement const & feature )
                                                 { return ( feature.name == requireFeature.name ) && ( feature.structure == requireFeature.structure ); } );
                                             } ),
                       require.xmlLine,
                       "feature <" + requireFeature.name + "> with struct <" + requireFeature.structure + "> already listed as required for feature <" +
                         feature.name + ">" );
      }
      for ( auto const & requireType : require.types )
      {
        checkForError(
          "vk.xml",
          std::ranges::none_of( feature.require, [&requireType]( auto const & require ) { return containsByName( require.types, requireType.name ); } ),
          require.xmlLine,
          "type <" + requireType.name + "> already listed as required for feature <" + feature.name + ">" );
      }
      feature.require.push_back( std::move( require ) );
    }
  }

  return feature;
}

FeatureElement parseFeatureElement( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} }, { "struct", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  FeatureElement feature{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      feature.name = attribute.second;
    }
    else if ( attribute.first == "struct" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      feature.structure = attribute.second;
    }
  }

  return feature;
}

Format parseFormat( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml",
                   line,
                   attributes,
                   { { "blockSize", {} }, { "class", {} }, { "name", {} }, { "texelsPerBlock", {} } },
                   { { "blockExtent", {} }, { "chroma", { "420", "422", "444" } }, { "compressed", {} }, { "packed", { "8", "16", "32" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements(
    "vk.xml", line, children, { { "component", MultipleAllowed::Yes } }, { { "plane", MultipleAllowed::Yes }, { "spirvimageformat", MultipleAllowed::No } } );

  Format format{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "blockExtent" )
    {
      format.blockExtent = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "blockSize" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      format.blockSize = attribute.second;
    }
    else if ( attribute.first == "chroma" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      format.chroma = attribute.second;
    }
    else if ( attribute.first == "class" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      format.classAttribute = attribute.second;
    }
    else if ( attribute.first == "compressed" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      format.compressed = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      format.name = attribute.second;
    }
    else if ( attribute.first == "packed" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      format.packed = attribute.second;
    }
    else if ( attribute.first == "texelsPerBlock" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      format.texelsPerBlock = attribute.second;
    }
  }

  checkForError( "vk.xml",
                 format.blockExtent.empty() ||
                   ( ( format.blockExtent.size() == 3 ) && std::ranges::all_of( format.blockExtent, []( auto const & e ) { return isNumber( e ); } ) ),
                 format.xmlLine,
                 "format <" + format.name + "> has ill-formatted attribute blockExtent = <" + concatenate( format.blockExtent ) + ">" );
  checkForError(
    "vk.xml", isNumber( format.blockSize ), format.xmlLine, "format <" + format.name + "> has non-number <" + format.blockSize + "> as blockSize" );
  checkForError( "vk.xml",
                 isNumber( format.texelsPerBlock ),
                 format.xmlLine,
                 "format <" + format.name + "> has non-number <" + format.texelsPerBlock + "> as texelsPerBlock" );
  checkForError( "vk.xml",
                 ( format.texelsPerBlock == "1" ) || ( !format.blockExtent.empty() && !format.compressed.empty() ),
                 format.xmlLine,
                 "format <" + format.name + "> has texelsPerBlock > 1 but no \"blockExtent\" or no \"compressed\" attribute" );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "component" )
    {
      format.components.push_back( parseComponent( child ) );
    }
    else if ( value == "plane" )
    {
      format.planes.push_back( parsePlane( child ) );
    }
    else if ( value == "spirvimageformat" )
    {
      format.spirvImageFormat = parseNameElement( child );
    }
  }

  bool isCompressed = ( format.components.front().bits == "compressed" );
  auto componentIt  = std::find_if( std::next( format.components.begin() ),
                                    format.components.end(),
                                    [&isCompressed]( auto const & component ) { return isCompressed != ( component.bits == "compressed" ); } );
  if ( componentIt != format.components.end() )
  {
    checkForError( "vk.xml",
                   false,
                   componentIt->xmlLine,
                   isCompressed ? "in format <" + format.name + "> component <" + componentIt->name + "> should be marked as compressed"
                                : "in format <" + format.name + "> component <" + componentIt->name + "> should not be marked as compressed" );
  }

  bool hasPlaneIndex = !format.components.front().planeIndex.empty();
  componentIt        = std::find_if( std::next( format.components.begin() ),
                                     format.components.end(),
                                     [&hasPlaneIndex]( auto const & component ) { return hasPlaneIndex != !component.planeIndex.empty(); } );
  if ( componentIt != format.components.end() )
  {
    checkForError( "vk.xml",
                   false,
                   componentIt->xmlLine,
                   hasPlaneIndex ? "in format <" + format.name + "> component <" + componentIt->name + "> is missing a planeIndex"
                                 : "in format <" + format.name + "> component <" + componentIt->name + "> has an unexpected planeIndex" );
  }
  if ( hasPlaneIndex )
  {
    auto planeCount = format.planes.size();
    componentIt =
      std::ranges::find_if( format.components, [&planeCount]( auto const & component ) { return planeCount <= std::stoul( component.planeIndex ); } );
    if ( componentIt != format.components.end() )
    {
      checkForError( "vk.xml",
                     false,
                     componentIt->xmlLine,
                     "in format <" + format.name + "> component <" + componentIt->name + "> has an unexpected planeIndex <" + componentIt->planeIndex + ">" );
    }
  }

  return format;
}

std::vector<Format> parseFormats( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "format", MultipleAllowed::Yes } } );

  std::vector<Format> formats;
  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "format" )
    {
      Format format = parseFormat( child );
      checkForError( "vk.xml", !containsByName( formats, format.name ), format.xmlLine, "format <" + format.name + "> already specified in formats block" );
      formats.push_back( std::move( format ) );
    }
  }
  return formats;
}

void parseImplicitExternSyncParams( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, {} );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "param", MultipleAllowed::No } }, {} );

  for ( auto child : children )
  {
    std::string const value = child->Value();
    if ( value == "param" )
    {
      parseImplicitExternSyncParamsParam( child );
    }
  }
}

void parseImplicitExternSyncParamsParam( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );
}

MultiFeatureElement parseMultiFeatureElement( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} }, { "struct", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  MultiFeatureElement feature{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      feature.names = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "struct" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      feature.structure = attribute.second;
    }
  }

  return feature;
}

std::pair<std::string, Type> parseNameAndType( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();

  auto [name, type, arraySizes, bitCount] = parseNameAndTypeModified( element );
  checkForError( "vk.xml", arraySizes.empty() && bitCount.empty(), line, "unexpected array size or bit count specification for name <" + name + ">" );

  return { name, type };
}

std::tuple<std::string, Type, std::vector<std::string>, std::string> parseNameAndTypeModified( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml",
                 line,
                 children,
                 { { "name", MultipleAllowed::No } },
                 { { "comment", MultipleAllowed::No }, { "enum", MultipleAllowed::No }, { "type", MultipleAllowed::No } } );

  std::string   name;
  Type          type;
  NameModifiers modifiers;
  for ( auto child : children )
  {
    std::string const value = child->Value();
    if ( value == "enum" )
    {
      modifiers.arraySizes.push_back( child->GetText() );
      checkForError( "vk.xml",
                     child->PreviousSibling() && ( strcmp( child->PreviousSibling()->Value(), "[" ) == 0 ) && child->NextSibling() &&
                       ( strcmp( child->NextSibling()->Value(), "]" ) == 0 ),
                     line,
                     std::string( "array specifiation is ill-formatted: <" ) + modifiers.arraySizes.back() + ">" );
    }
    else if ( value == "name" )
    {
      name      = parseText( child );
      modifiers = parseNameModifiers( "vk.xml", child->NextSibling() );
    }
    else if ( value == "type" )
    {
      type = parseType( child );
    }
  }
  return { name, type, modifiers.arraySizes, modifiers.bitCount };
}

std::pair<std::string, std::string> parseNameWithAlias( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, {}, { { "alias", {} } } );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  std::string alias, name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      alias = attribute.second;
    }
  }

  name = element->GetText();

  return { name, alias };
}

Param parseParam( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml",
                   line,
                   attributes,
                   {},
                   { { "altlen", { "(samples + 31) / 32" } },
                     { "api", { "vulkan", "vulkanbase", "vulkansc" } },
                     { "externsync", {} },
                     { "len", {} },
                     { "noautovalidity", { "true" } },
                     { "objecttype", { "objectType" } },
                     { "optional", { "false", "true" } },
                     { "stride", {} },
                     { "validstructs", {} } } );

  Param param{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "altlen" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      param.altLen = attribute.second;
    }
    else if ( attribute.first == "api" )
    {
      param.api = tokenize( attribute.second, "," );
    }
    if ( attribute.first == "externsync" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      param.externSync = attribute.second;
    }
    else if ( attribute.first == "len" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      param.len = attribute.second;
    }
    else if ( attribute.first == "noautovalidity" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      param.noAutoValidity = attribute.second;
    }
    else if ( attribute.first == "objecttype" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      param.objectType = attribute.second;
    }
    else if ( attribute.first == "optional" )
    {
      param.optional = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "stride" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      param.stride = attribute.second;
    }
    else if ( attribute.first == "validstructs" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      param.validStructs = attribute.second;
    }
  }

  std::string bitCount;
  std::tie( param.name, param.type, param.arraySizes, bitCount ) = parseNameAndTypeModified( element );
  checkForError( "vk.xml", bitCount.empty(), line, "unexpected bit count specification for param <" + param.name + ">" );
  checkForError( "vk.xml", param.type.prefix.empty() || ( param.type.prefix == "struct" ), line, "unexpected type prefix <" + param.type.prefix + ">" );
  checkForError( "vk.xml",
                 param.type.postfix.empty() || ( param.type.postfix == "const" ) || ( param.type.postfix == "*" ) || ( param.type.postfix == "const *" ) ||
                   ( param.type.postfix == "**" ) || ( param.type.postfix == "* const *" ) || ( param.type.postfix == "const * const *" ),
                 line,
                 "unexpected type postfix <" + param.type.postfix + ">" );
  assert( param.type.postfix != "* const *" );
  checkForError( "vk.xml",
                 !param.externSync.starts_with( "maybe:" ) || param.externSync.starts_with( "maybe:" + param.name + "[]." ),
                 line,
                 "param <" + param.name + "> has unknown value <" + param.externSync + "> for attribute externsync" );
  checkForError( "vk.xml",
                 param.objectType.empty() || ( param.type.name == "uint64_t" ),
                 line,
                 "param <" + param.name + "> has attribute <objectype> but its type is <" + param.type.name + "> instead of <uint64_t>" );
  checkForError( "vk.xml", param.altLen.empty() || !param.len.empty(), line, "param <" + param.name + "> has attribute <altLen>, but no attribute <len>" );
  checkForError( "vk.xml", param.len.empty() || param.type.isPointer(), line, "param <" + param.name + "> has attribute <len> but is not a pointer" );

  return param;
}

Plane parsePlane( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml",
                   line,
                   attributes,
                   { { "compatible", {} }, { "heightDivisor", { "1", "2" } }, { "index", { "0", "1", "2" } }, { "widthDivisor", { "1", "2" } } },
                   { { "planeIndex", { "0", "1", "2" } } } );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  Plane plane{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "compatible" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      plane.compatible = attribute.second;
    }
    else if ( attribute.first == "heightDivisor" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      plane.heightDivisor = attribute.second;
    }
    else if ( attribute.first == "index" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      plane.index = attribute.second;
    }
    else if ( attribute.first == "planeIndex" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      plane.planeIndex = attribute.second;
    }
    else if ( attribute.first == "widthDivisor" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      plane.widthDivisor = attribute.second;
    }
  }
  return plane;
}

Platform parsePlatform( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "comment", {} }, { "name", {} }, { "protect", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  Platform platform{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      platform.comment = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      platform.name = attribute.second;
    }
    else if ( attribute.first == "protect" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      platform.protect = attribute.second;
    }
  }

  return platform;
}

Platforms parsePlatforms( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "platform", MultipleAllowed::Yes } } );

  Platforms platforms{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      platforms.comment = attribute.second;
    }
  }

  for ( auto child : children )
  {
    Platform platform = parsePlatform( child );

    checkForError( "vk.xml", !containsByName( platforms.platforms, platform.name ), platform.xmlLine, "platform <" + platform.name + "> already specified" );
    checkForError( "vk.xml",
                   std::ranges::find_if( platforms.platforms, [protect = platform.protect]( auto const & platform ) { return protect == platform.protect; } ) ==
                     platforms.platforms.end(),
                   platform.xmlLine,
                   "platform <" + platform.name + "> uses protect <" + platform.protect + "> that is already used by some other platform" );

    platforms.platforms.push_back( std::move( platform ) );
  }

  return platforms;
}

std::pair<std::string, Type> parseProto( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, {} );

  auto [name, type] = parseNameAndType( element );

  checkForError( "vk.xml", type.prefix.empty(), line, "unexpected type prefix <" + type.prefix + ">" );
  checkForError( "vk.xml", type.postfix.empty(), line, "unexpected type postfix <" + type.postfix + ">" );

  return { name, type };
}

Vkxml parseRegistry( tinyxml2::XMLElement const * element, std::string const & api )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, {} );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml",
                 line,
                 children,
                 { { "commands", MultipleAllowed::No },
                   { "comment", MultipleAllowed::Yes },
                   { "enums", MultipleAllowed::Yes },
                   { "extensions", MultipleAllowed::No },
                   { "feature", MultipleAllowed::Yes },
                   { "formats", MultipleAllowed::No },
                   { "platforms", MultipleAllowed::No },
                   { "spirvcapabilities", MultipleAllowed::No },
                   { "spirvextensions", MultipleAllowed::No },
                   { "sync", MultipleAllowed::No },
                   { "tags", MultipleAllowed::No },
                   { "types", MultipleAllowed::No },
                   { "videocodecs", MultipleAllowed::No } } );

  Vkxml vkxml;
  for ( auto child : children )
  {
    std::string const value = child->Value();
    if ( value == "commands" )
    {
      vkxml.commands = parseCommands( child );

      for ( auto const & command : vkxml.commands )
      {
        for ( auto const & param : command.params )
        {
          if ( param.externSync.starts_with( "maybe:" ) )
          {
            auto pos = param.externSync.find_last_of( '.' );
            checkForError( "vk.xml",
                           pos != std::string::npos,
                           param.xmlLine,
                           "unexpected value <" + param.externSync + "> for attribute externsync of parameter <" + param.name + "> of command <" +
                             command.name + ">, expected format is \"maybe:<paramname>[].<membername>\"" );
            std::string memberName = param.externSync.substr( pos + 1 );
            auto        structIt   = findByName( vkxml.structs, param.type.name );
            checkForError( "vk.xml",
                           structIt != vkxml.structs.end(),
                           param.xmlLine,
                           "type <" + param.type.name + "> of parameter <" + param.name + "> of command <" + command.name +
                             "> with externsync attribute is not a struct" );
            checkForError( "vk.xml",
                           containsByName( structIt->members, memberName ),
                           param.xmlLine,
                           "struct <" + param.type.name + "> used in externsync attribute of parameter <" + param.name + "> of command <" + command.name +
                             "> does not have member <" + memberName + ">" );
          }
          checkForError( "vk.xml",
                         param.len.empty() || ( param.len == "null-terminated" ) || ( param.len == "1" ) || param.len.starts_with( "latexmath:" ) ||
                           containsByName( command.params, param.len ) || isLenByStructMember( param.len, command.params, vkxml.structs ),
                         param.xmlLine,
                         "unknown len <" + param.len + "> specified for parameter <" + param.name + "> of command <" + command.name + ">" );
          checkForError( "vk.xml",
                         vkxml.types.contains( param.type.name ),
                         param.xmlLine,
                         "unknown type <" + param.type.name + "> of parameter <" + param.name + "> of command <" + command.name + ">" );
          checkForError( "vk.xml",
                         param.validStructs.empty() || containsByName( vkxml.structs, param.validStructs ),
                         param.xmlLine,
                         "unknown validstructs <" + param.validStructs + "> specified for parameter <" + param.name + "> of command <" + command.name + ">" );
        }
      }
    }
    else if ( value == "comment" )
    {
      Comment comment = parseComment( "vk.xml", child );

      if ( comment.text.starts_with( "\nCopyright" ) )
      {
        checkForError(
          "vk.xml", vkxml.copyright.text.empty(), line, "Copyright message has already been encountered on line " + std::to_string( vkxml.copyright.xmlLine ) );
        vkxml.copyright = std::move( comment );
      }
    }
    else if ( value == "enums" )
    {
      EnumsVariant enums = parseEnums( child );
      if ( std::holds_alternative<EnumsBitmask>( enums ) )
      {
        auto const & bitmask = std::get<EnumsBitmask>( enums );
        auto         enumIt  = findByName( vkxml.enums, bitmask.name );
        checkForError( "vk.xml", enumIt != vkxml.enums.end(), line, "bitmask <" + bitmask.name + "> not specified" );

        enumIt->bitwidth       = bitmask.bitWidth;
        enumIt->category       = "bitmask";
        auto [prefix, postfix] = determineEnumSuffixes( bitmask.name, true, vkxml.tags.tags );
        for ( auto & bitmaskValue : bitmask.values )
        {
          checkForError( "vk.xml",
                         bitmaskValue.name.starts_with( prefix ),
                         bitmaskValue.xmlLine,
                         "bitmask value <" + bitmaskValue.name + "> does not start with expected prefix <" + prefix + ">" );
          checkForError( "vk.xml",
                         postfix.empty() || bitmaskValue.name.ends_with( postfix ),
                         bitmaskValue.xmlLine,
                         "bitmask value <" + bitmaskValue.name + "> does not end with expected postfix <" + postfix + ">" );

          enumIt->values.push_back( { .name    = std::move( bitmaskValue.name ),
                                      .bitPos  = std::move( bitmaskValue.bitPos ),
                                      .comment = std::move( bitmaskValue.comment ),
                                      .value   = std::move( bitmaskValue.value ),
                                      .aliases = std::move( bitmaskValue.aliases ),
                                      .xmlLine = bitmaskValue.xmlLine } );
        }
      }
      else if ( std::holds_alternative<EnumsConstants>( enums ) )
      {
        auto const & enumsConstants = std::get<EnumsConstants>( enums );

        checkForError( "vk.xml", vkxml.constants.values.empty(), enumsConstants.xmlLine, "constants have already been encountered" );
        for ( auto const & constantValue : enumsConstants.values )
        {
          checkForError( "vk.xml",
                         containsByName( vkxml.externals, constantValue.type ),
                         constantValue.xmlLine,
                         "constant value <" + constantValue.name + "> has unknown type <" + constantValue.type + ">" );
        }

        vkxml.constants = std::move( enumsConstants );
      }
      else
      {
        assert( std::holds_alternative<EnumsEnum>( enums ) );
        auto const & enumsEnum = std::get<EnumsEnum>( enums );
        auto         enumIt    = findByName( vkxml.enums, enumsEnum.name );
        checkForError( "vk.xml", enumIt != vkxml.enums.end(), line, "enum <" + enumsEnum.name + "> not specified" );

        enumIt->category       = "enum";
        auto [prefix, postfix] = determineEnumSuffixes( enumsEnum.name, false, vkxml.tags.tags );
        for ( auto & enumsValue : enumsEnum.values )
        {
          checkForError( "vk.xml",
                         enumsValue.name.starts_with( prefix ),
                         enumsValue.xmlLine,
                         "enum value <" + enumsValue.name + "> does not start with expected prefix <" + prefix + ">" );
          checkForError( "vk.xml",
                         postfix.empty() || enumsValue.name.ends_with( postfix ),
                         enumsValue.xmlLine,
                         "enum value <" + enumsValue.name + "> does not end with expected postfix <" + postfix + ">" );
        }
        enumIt->values = std::move( enumsEnum.values );
      }
    }
    else if ( value == "extensions" )
    {
      vkxml.extensions = parseExtensions( child );
      for ( auto const & extension : vkxml.extensions.extensions )
      {
        checkForError( "vk.xml",
                       extension.platform.empty() || containsByName( vkxml.platforms.platforms, extension.platform ),
                       extension.xmlLine,
                       "extension <" + extension.name + "> references an unknown platform <" + extension.platform + ">" );
        checkForError( "vk.xml",
                       extension.deprecatedBy.empty() || containsByName( vkxml.extensions.extensions, extension.deprecatedBy ) ||
                         containsByName( vkxml.features, extension.deprecatedBy ),
                       extension.xmlLine,
                       "extension <" + extension.name + "> is deprecated by an unknown feature or extension <" + extension.deprecatedBy + ">" );
        checkForError( "vk.xml",
                       extension.promotedTo.empty() || containsByName( vkxml.extensions.extensions, extension.promotedTo ) ||
                         containsByName( vkxml.features, extension.promotedTo ),
                       extension.xmlLine,
                       "extension <" + extension.name + "> is promoted to an unknown feature or extension <" + extension.promotedTo + ">" );
        checkForError( "vk.xml",
                       extension.obsoletedBy.empty() || containsByName( vkxml.extensions.extensions, extension.obsoletedBy ),
                       extension.xmlLine,
                       "extension <" + extension.name + "> is obsoleted by an unknown extension <" + extension.obsoletedBy + ">" );
        for ( auto const & deprecate : extension.deprecates )
        {
          for ( auto const & command : deprecate.commands )
          {
            checkForError( "vk.xml",
                           containsByNameOrAlias( vkxml.commands, command.name ),
                           command.xmlLine,
                           "extension <" + extension.name + "> deprecates unknown command <" + command.name + ">" );
            checkForError( "vk.xml",
                           command.supersededBy.empty() || containsByName( vkxml.commands, command.supersededBy ),
                           command.xmlLine,
                           "deprecated command <" + command.name + "> superseded by unknown command <" + command.supersededBy + ">" );
          }
          for ( auto const & feature : deprecate.features )
          {
            auto structIt = findByName( vkxml.structs, feature.structure );
            checkForError( "vk.xml",
                           structIt != vkxml.structs.end(),
                           feature.xmlLine,
                           "deprecated feature <" + feature.name + "> specifies unknown struct <" + feature.structure + ">" );
            checkForError( "vk.xml",
                           std::ranges::any_of( structIt->members, [&feature]( auto const & member ) { return member.name == feature.name; } ),
                           feature.xmlLine,
                           "deprecated feature <" + feature.name + "> is not a member of struct <" + feature.structure + ">" );
          }
          for ( auto const & type : deprecate.types )
          {
            checkForError(
              "vk.xml", vkxml.types.contains( type.name ), type.xmlLine, "extension <" + extension.name + "> deprecates unknown type <" + type.name + ">" );
            checkForError( "vk.xml",
                           type.supersededBy.empty() || vkxml.types.contains( type.supersededBy ),
                           type.xmlLine,
                           "extension <" + extension.name + "> deprecates type <" + type.name + "> superseded by unknown type <" + type.supersededBy + ">" );
          }
        }

        for ( auto const & remove : extension.removes )
        {
          auto structIt = findByName( vkxml.structs, remove.feature.structure );
          checkForError( "vk.xml",
                         structIt != vkxml.structs.end(),
                         remove.feature.xmlLine,
                         "extension <" + extension.name + "> removes feature <" + remove.feature.name + "> in unknown struct <" + remove.feature.structure +
                           " >" );
          checkForError( "vk.xml",
                         std::ranges::any_of( structIt->members, [&remove]( auto const & member ) { return member.name == remove.feature.name; } ),
                         remove.feature.xmlLine,
                         "extension <" + extension.name + "> removes feature <" + remove.feature.name + "> that is not a member of struct <" +
                           remove.feature.structure + " >" );
        }

        for ( auto const & require : extension.require )
        {
          for ( auto const & command : require.commands )
          {
            checkForError( "vk.xml",
                           containsByNameOrAlias( vkxml.commands, command.name ),
                           command.xmlLine,
                           "extension <" + extension.name + "> requires unknown command <" + command.name + ">" );
          }
          for ( auto const & e : require.enums )
          {
            checkForError( "vk.xml",
                           e.extNumber.empty() ||
                             std::ranges::any_of( vkxml.extensions.extensions,
                                                  [&extNumber = e.extNumber]( auto const & extension ) { return extNumber == extension.number; } ),
                           e.xmlLine,
                           "enum value <" + e.name + "> references an unknown extension by number <" + e.extNumber + ">" );
            if ( !e.extends.empty() )
            {
              auto enumIt = findByName( vkxml.enums, e.extends );
              checkForError( "vk.xml", enumIt != vkxml.enums.end(), e.xmlLine, "enum value <" + e.name + "> extends unknown enum <" + e.extends + ">" );

              auto valueIt = findByName( enumIt->values, e.name );
              if ( valueIt != enumIt->values.end() )
              {
                checkForError( "vk.xml",
                               ( valueIt->value == e.value ) && ( valueIt->bitPos == e.bitPos ),
                               e.xmlLine,
                               "enum value <" + e.name + "> already listed for enum <" + e.extends + "> with the same value or bitpos" );
              }
            }
          }
          for ( auto const & feature : require.features )
          {
            auto structIt = findByNameOrAlias( vkxml.structs, feature.structure );
            checkForError( "vk.xml",
                           structIt != vkxml.structs.end(),
                           feature.xmlLine,
                           "extension <" + extension.name + "> requires some feature in unknown struct <" + feature.structure + " >" );
            for ( auto const & featureName : feature.names )
            {
              auto memberIt = std::ranges::find_if( structIt->members, [&featureName]( auto const & member ) { return member.name == featureName; } );
              checkForError( "vk.xml",
                             memberIt != structIt->members.end(),
                             feature.xmlLine,
                             "extension <" + extension.name + "> requires feature <" + featureName + "> that is not a member of the required feature struct <" +
                               feature.structure + ">" );
              checkForError( "vk.xml",
                             ( memberIt->type.isValue() && ( memberIt->type.name == "VkBool32" ) ),
                             feature.xmlLine,
                             "extension <" + extension.name + "> requires feature <" + featureName +
                               "> that is not a VkBool32 member of the required feature struct <" + feature.structure + ">" );
            }
          }
        }

        if ( std::ranges::none_of( extension.supported, []( std::string const & s ) { return s == "disabled"; } ) )
        {
          // extract the tag from the name, which is supposed to look like VK_<tag>_<other>
          size_t const tagStart = extension.name.find( '_' );
          checkForError( "vk.xml", tagStart != std::string::npos, extension.xmlLine, "extension name <" + extension.name + "> is missing an underscore '_'" );
          size_t const tagEnd = extension.name.find( '_', tagStart + 1 );
          checkForError( "vk.xml", tagEnd != std::string::npos, extension.xmlLine, "extension name <" + extension.name + "> is missing an underscore '_'" );
          std::string tag = extension.name.substr( tagStart + 1, tagEnd - tagStart - 1 );
          checkForError( "vk.xml",
                         containsByName( vkxml.tags.tags, tag ),
                         extension.xmlLine,
                         "extension name <" + extension.name + "> is using an unknown tag <" + tag + ">" );
        }
      }
    }
    else if ( value == "feature" )
    {
      Feature feature = parseFeature( child );
      checkForError( "vk.xml", !containsByName( vkxml.features, feature.name ), feature.xmlLine, "A feature named <" + feature.name + "> already specified" );
      checkForError( "vk.xml",
                     std::ranges::all_of( feature.depends, [&vkxml]( auto const & depends ) { return containsByName( vkxml.features, depends ); } ),
                     feature.xmlLine,
                     "feature <" + feature.name + "> depends on unknown feature(s) <" + concatenate( feature.depends ) + ">" );
      for ( auto const & deprecate : feature.deprecates )
      {
        for ( auto const & type : deprecate.types )
        {
          checkForError(
            "vk.xml", vkxml.types.contains( type.name ), deprecate.xmlLine, "feature <" + feature.name + "> deprecates unknown type <" + type.name + ">" );
          checkForError( "vk.xml",
                         type.supersededBy.empty() || vkxml.types.contains( type.supersededBy ),
                         deprecate.xmlLine,
                         "feature <" + feature.name + "> deprecates type <" + type.name + "> superseded by unknown type <" + type.supersededBy + ">" );
        }
      }
      for ( auto const & remove : feature.removes )
      {
        for ( auto const & e : remove.enums )
        {
          auto enumIt = std::ranges::find_if( vkxml.enums, [&e]( auto const & enum_ ) { return containsByName( enum_.values, e.name ); } );
          checkForError( "vk.xml", enumIt != vkxml.enums.end(), e.xmlLine, "feature <" + feature.name + "> removes unknown enum <" + e.name + ">" );
        }
      }
      for ( auto const & require : feature.require )
      {
        for ( auto const & e : require.enums )
        {
          if ( !e.extends.empty() )
          {
            auto enumIt = findByName( vkxml.enums, e.extends );
            checkForError(
              "vk.xml", enumIt != vkxml.enums.end(), e.xmlLine, "feature <" + feature.name + "> requires extending an unknown enum <" + e.extends + ">" );
            if ( e.alias.empty() )
            {
              checkForError( "vk.xml",
                             !containsByName( enumIt->values, e.name ),
                             e.xmlLine,
                             "feature <" + feature.name + "> requires to extend enum <" + e.extends + "> with an already specified value <" + e.name + ">" );
              enumIt->values.push_back( { .name = e.name, .bitPos = e.bitPos, .comment = e.comment, .value = e.value, .xmlLine = e.xmlLine } );
            }
            else
            {
              auto valueIt = findByName( enumIt->values, e.alias );
              checkForError( "vk.xml",
                             valueIt != enumIt->values.end(),
                             e.xmlLine,
                             "feature <" + feature.name + "> requires to extend enum <" + e.extends + "> with an alias <" + e.name +
                               "> for an unknown enum value <" + e.alias + ">" );
              checkForError( "vk.xml",
                             !containsByName( valueIt->aliases, e.name ),
                             e.xmlLine,
                             "feature <" + feature.name + "> requires to extend enum <" + e.extends + "> with an already specified alias <" + e.name + ">" );
              valueIt->aliases.push_back( { .name = e.name, .api = e.api, .deprecated = e.deprectated, .xmlLine = e.xmlLine } );
            }
          }
        }
        for ( auto const & requireFeature : require.features )
        {
          auto structIt = findByName( vkxml.structs, requireFeature.structure );
          checkForError( "vk.xml",
                         structIt != vkxml.structs.end(),
                         requireFeature.xmlLine,
                         "feature <" + feature.name + "> requires feature <" + requireFeature.name + "> of unknown struct <" + requireFeature.structure + ">" );
          checkForError( "vk.xml",
                         containsByName( structIt->members, requireFeature.name ),
                         requireFeature.xmlLine,
                         "feature <" + feature.name + "> requires unknown member <" + requireFeature.name + "> in struct <" + requireFeature.structure + ">" );
        }
        for ( auto const & type : require.types )
        {
          checkForError( "vk.xml",
                         vkxml.types.contains( type.name ),
                         type.xmlLine,
                         "feature <" + feature.name + "> has a require depending on unknown type <" + type.name + ">" );
        }
      }
      vkxml.features.push_back( std::move( feature ) );
    }
    else if ( value == "formats" )
    {
      std::vector<Format> formats = parseFormats( child );

      for ( auto const & format : formats )
      {
        std::string tag = findTag( format.name, vkxml.tags.tags );
        checkForError( "vk.xml",
                       format.packed.empty() || format.name.ends_with( "PACK" + format.packed + ( tag.empty() ? "" : "_" + tag ) ),
                       format.xmlLine,
                       "format <" + format.name + "> has ill-formatted attribute packed = <" + format.packed + ">" );
      }
      vkxml.formats = std::move( formats );
    }
    else if ( value == "platforms" )
    {
      vkxml.platforms = parsePlatforms( child );
    }
    else if ( value == "spirvcapabilities" )
    {
      SPIRVCapabilities spirvCapabilities = parseSPIRVCapabilities( child );
      for ( auto const & spirvCapability : spirvCapabilities.capabilities )
      {
        for ( auto const & enable : spirvCapability.enables )
        {
          checkForError( "vk.xml",
                         enable.extension.empty() || containsByName( vkxml.extensions.extensions, enable.extension ),
                         enable.xmlLine,
                         "spirvcapability <" + spirvCapability.name + "> enables unknown extension <" + enable.extension + ">" );
          if ( !enable.property.empty() )
          {
            checkForError( "vk.xml",
                           std::ranges::all_of( enable.require,
                                                [&vkxml]( auto const & r )
                                                { return containsByName( vkxml.extensions.extensions, r ) || containsByName( vkxml.features, r ); } ),
                           enable.xmlLine,
                           "spirvcapability <" + spirvCapability.name + "> requires unknown feature or extension <" + concatenate( enable.require ) + ">" );
            auto structIt = findByName( vkxml.structs, enable.property );
            checkForError( "vk.xml",
                           structIt != vkxml.structs.end(),
                           enable.xmlLine,
                           "spirvcapability <" + spirvCapability.name + "> enables unknown property struct <" + enable.property + ">" );
            auto memberIt = findByName( structIt->members, enable.member );
            checkForError( "vk.xml",
                           memberIt != structIt->members.end(),
                           enable.xmlLine,
                           "spirvcapability <" + spirvCapability.name + "> enables unknown member <" + enable.member + "> in property struct <" +
                             enable.property + ">" );
            checkForError( "vk.xml",
                           ( memberIt->type.name == "VkBool32" ) || containsByName( vkxml.bitmasks, memberIt->type.name ),
                           enable.xmlLine,
                           "spirvcapability <" + spirvCapability.name + "> enables member <" + enable.member + "> in property struct <" + enable.property +
                             "> of unhandled type <" + memberIt->type.name + ">" );
            if ( memberIt->type.name == "VkBool32" )
            {
              checkForError( "vk.xml",
                             enable.value == "VK_TRUE",
                             enable.xmlLine,
                             "spirvcapability <" + spirvCapability.name + "> enables member <" + enable.member + "> in property struct <" + enable.property +
                               "> with unexpected value <" + enable.value + ">" );
            }
          }
          if ( !enable.structure.empty() )
          {
            checkForError( "vk.xml",
                           std::ranges::all_of( enable.require,
                                                [&vkxml]( auto const & r )
                                                { return containsByName( vkxml.extensions.extensions, r ) || containsByName( vkxml.features, r ); } ),
                           enable.xmlLine,
                           "spirvcapability <" + spirvCapability.name + "> requires unknown feature or extension <" + concatenate( enable.require ) + ">" );
            auto structIt = findByNameOrAlias( vkxml.structs, enable.structure );
            checkForError( "vk.xml",
                           structIt != vkxml.structs.end(),
                           enable.xmlLine,
                           "spirvcapability <" + spirvCapability.name + "> enables unknown struct <" + enable.structure + ">" );
            auto memberIt = findByName( structIt->members, enable.feature );
            checkForError( "vk.xml",
                           memberIt != structIt->members.end(),
                           enable.xmlLine,
                           "spirvcapability <" + spirvCapability.name + "> enables struct <" + enable.structure + "> with unknown feature member <" +
                             enable.feature + ">" );
            checkForError( "vk.xml",
                           memberIt->type.name == "VkBool32",
                           enable.xmlLine,
                           "spirvcapability <" + spirvCapability.name + "> enables feature member <" + enable.feature + "> in struct <" + enable.structure +
                             "> of unexpected type <" + memberIt->type.name + ">" );
            checkForError(
              "vk.xml", enable.alias.empty() || ( memberIt->featureLink == enable.alias ), enable.xmlLine, "unknown alias <" + enable.alias + "> " );
          }
          checkForError( "vk.xml",
                         enable.version.empty() || containsByName( vkxml.features, enable.version ),
                         enable.xmlLine,
                         "spirvcapability <" + spirvCapability.name + "> enables unknown feature version <" + enable.version + ">" );
        }
      }
      vkxml.spirvCapabilities = std::move( spirvCapabilities );
    }
    else if ( value == "spirvextensions" )
    {
      SPIRVExtensions spirvExtensions = parseSPIRVExtensions( child );
      for ( auto const & spirvExtension : spirvExtensions.extensions )
      {
        for ( auto const & enable : spirvExtension.enables )
        {
          if ( !enable.extension.empty() )
          {
            checkForError( "vk.xml",
                           containsByName( vkxml.extensions.extensions, enable.extension ),
                           enable.xmlLine,
                           "spirvextension <" + spirvExtension.name + "> enables unknown extension <" + enable.extension + ">" );
          }
          else
          {
            checkForError( "vk.xml",
                           containsByName( vkxml.features, enable.version ),
                           enable.xmlLine,
                           "spirvextension <" + spirvExtension.name + "> enables unknown version <" + enable.version + ">" );
          }
        }
      }
      vkxml.spirvExtensions = std::move( spirvExtensions );
    }
    else if ( value == "sync" )
    {
      vkxml.sync = parseSync( child );
    }
    else if ( value == "tags" )
    {
      vkxml.tags = parseTags( child );
    }
    else if ( value == "types" )
    {
      Types types        = parseTypes( child, api );
      vkxml.baseTypes    = std::move( types.baseTypes );
      vkxml.bitmasks     = std::move( types.bitmasks );
      vkxml.defines      = std::move( types.defines );
      vkxml.enums        = std::move( types.enums );
      vkxml.externals    = std::move( types.externals );
      vkxml.funcPointers = std::move( types.funcPointers );
      vkxml.handles      = std::move( types.handles );
      vkxml.includes     = std::move( types.includes );
      vkxml.structs      = std::move( types.structs );
      vkxml.unions       = std::move( types.unions );
      vkxml.types        = std::move( types.types );
    }
    else if ( value == "videocodecs" )
    {
      vkxml.videoCodecs = parseVideoCodecs( child );
      for ( auto const & videoCodec : vkxml.videoCodecs )
      {
        for ( auto const & videoCapabilities : videoCodec.videoCapabilities )
        {
          checkForError( "vk.xml",
                         containsByName( vkxml.structs, videoCapabilities.structure ),
                         videoCapabilities.xmlLine,
                         "videocodec <" + videoCodec.name + "> has video capabilities with unknown struct <" + videoCapabilities.structure + ">" );
        }
        for ( auto const & videoFormat : videoCodec.videoFormats )
        {
          if ( videoFormat.videoFormatProperties.has_value() )
          {
            auto const & videoFormatProperties = videoFormat.videoFormatProperties.value();
            auto         structIt              = findByName( vkxml.structs, videoFormatProperties.structure );
            checkForError( "vk.xml",
                           structIt != vkxml.structs.end(),
                           videoFormatProperties.xmlLine,
                           "videoformat <" + videoFormat.name + "> in videocodec <" + videoCodec.name + "> has videoformatproperties with unknown struct <" +
                             videoFormatProperties.structure + ">" );
          }
          if ( videoFormat.videoRequireCapabilities.has_value() )
          {
            auto const & videoRequireCapabilities = videoFormat.videoRequireCapabilities.value();
            auto         structIt                 = findByName( vkxml.structs, videoRequireCapabilities.structure );
            checkForError( "vk.xml",
                           structIt != vkxml.structs.end(),
                           videoRequireCapabilities.xmlLine,
                           "videoformat <" + videoFormat.name + "> in videocodec <" + videoCodec.name + "> has videorequirecapabilities with unknown struct <" +
                             videoRequireCapabilities.structure + ">" );
            checkForError( "vk.xml",
                           containsByName( structIt->members, videoRequireCapabilities.member ),
                           videoRequireCapabilities.xmlLine,
                           "videoformat <" + videoFormat.name + "> in videocodec <" + videoCodec.name + "> has videorequirecapabilities with unknown member <" +
                             videoRequireCapabilities.member + "> for struct <" + videoRequireCapabilities.structure + ">" );
          }
        }
        if ( videoCodec.videoProfiles.has_value() )
        {
          auto const & videoProfiles = videoCodec.videoProfiles.value();
          checkForError( "vk.xml",
                         containsByName( vkxml.structs, videoProfiles.structure ),
                         videoProfiles.xmlLine,
                         "videocodec <" + videoCodec.name + "> has videoprofiles with unknown struct <" + videoProfiles.structure + ">" );
        }
      }
    }
    // else
    //{
    //   checkForError( "vk.xml", false, line, "unknown element <" + value + ">" );
    // }
  }

  checkForError( "VkXMLParser", !vkxml.copyright.text.empty(), line, "Copyright message is missing" );

  for ( auto const & feature : vkxml.features )
  {
    for ( auto const & require : feature.require )
    {
      for ( auto const & depend : require.depends )
      {
        checkExtensionOrStructAndMember( depend, require.xmlLine, "feature <" + feature.name + "> has a require-member depending on", vkxml.structs );
      }
    }
  }
  for ( auto const & structure : vkxml.structs )
  {
    for ( auto const & member : structure.members )
    {
      if ( !member.alias.empty() )
      {
        checkExtensionOrStructAndMember(
          member.alias, member.xmlLine, "member <" + member.name + "> of struct <" + structure.name + "> has an alias with", vkxml.structs );
      }
    }
  }

  return vkxml;
}

Remove parseRemove( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, {}, { { "comment", {} }, { "reasonlink", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml",
                 line,
                 children,
                 {},
                 { { "command", MultipleAllowed::Yes },
                   { "comment", MultipleAllowed::Yes },
                   { "enum", MultipleAllowed::Yes },
                   { "feature", MultipleAllowed::Yes },
                   { "type", MultipleAllowed::Yes } } );

  Remove remove{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      remove.comment = attribute.second;
    }
    else if ( attribute.first == "reasonlink" )
    {
      remove.reasonLink = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "command" )
    {
      NameElement removeCommand = parseNameElement( child );
      checkForError( "vk.xml",
                     !containsByName( remove.commands, removeCommand.name ),
                     removeCommand.xmlLine,
                     "remove command <" + removeCommand.name + "> already listed for this remove block" );
      remove.commands.push_back( std::move( removeCommand ) );
    }
    else if ( value == "enum" )
    {
      NameElement removeEnum = parseNameElement( child );
      checkForError( "vk.xml",
                     !containsByName( remove.enums, removeEnum.name ),
                     removeEnum.xmlLine,
                     "remove enum <" + removeEnum.name + "> already listed for this remove block" );
      remove.enums.push_back( std::move( removeEnum ) );
    }
    else if ( value == "feature" )
    {
      FeatureElement removeFeature = parseFeatureElement( child );
      checkForError( "vk.xml",
                     std::ranges::none_of( remove.features,
                                           [&removeFeature]( FeatureElement const & feature )
                                           { return ( feature.name == removeFeature.name ) && ( feature.structure == removeFeature.structure ); } ),
                     removeFeature.xmlLine,
                     "remove feature <" + removeFeature.name + "> with struct <" + removeFeature.structure + "> already listed for this remove block" );
      remove.features.push_back( std::move( removeFeature ) );
    }
    else if ( value == "type" )
    {
      NameElement removeType = parseNameElement( child );
      checkForError( "vk.xml",
                     !containsByName( remove.types, removeType.name ),
                     removeType.xmlLine,
                     "remove type <" + removeType.name + "> already listed for this remove block" );
      remove.types.push_back( std::move( removeType ) );
    }
  }

  return remove;
}

Require parseRequire( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, {}, { { "comment", {} }, { "depends", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml",
                 line,
                 children,
                 {},
                 { { "command", MultipleAllowed::Yes },
                   { "comment", MultipleAllowed::Yes },
                   { "enum", MultipleAllowed::Yes },
                   { "feature", MultipleAllowed::Yes },
                   { "type", MultipleAllowed::Yes } } );

  Require require{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      require.comment = attribute.second;
    }
    else if ( attribute.first == "depends" )
    {
      require.depends = tokenize( attribute.second, "," );
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "command" )
    {
      NameElement requireCommand = parseNameElement( child );
      checkForError( "vk.xml",
                     !containsByName( require.commands, requireCommand.name ),
                     requireCommand.xmlLine,
                     "require command <" + requireCommand.name + "> already listed for this require block" );
      require.commands.push_back( std::move( requireCommand ) );
    }
    else if ( value == "enum" )
    {
      RequireEnum requireEnum = parseRequireEnum( child );
      checkForError( "vk.xml",
                     !containsByName( require.enums, requireEnum.name ),
                     requireEnum.xmlLine,
                     "require enum <" + requireEnum.name + "> already listed for this require block" );
      require.enums.push_back( std::move( requireEnum ) );
    }
    else if ( value == "feature" )
    {
      FeatureElement requireFeature = parseFeatureElement( child );
      checkForError( "vk.xml",
                     std::ranges::none_of( require.features,
                                           [&requireFeature]( FeatureElement const & feature )
                                           { return ( feature.name == requireFeature.name ) && ( feature.structure == requireFeature.structure ); } ),
                     requireFeature.xmlLine,
                     "require feature <" + requireFeature.name + "> with struct <" + requireFeature.structure + "> already listed for this require block" );
      require.features.push_back( std::move( requireFeature ) );
    }
    else if ( value == "type" )
    {
      RequireType requireType = parseRequireType( child );
      checkForError( "vk.xml",
                     !containsByName( require.types, requireType.name ),
                     requireType.xmlLine,
                     "require type <" + requireType.name + "> already listed for this require block" );
      require.types.push_back( std::move( requireType ) );
    }
  }

  return require;
}

RequireEnum parseRequireEnum( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  if ( attributes.contains( "extends" ) )
  {
    if ( attributes.contains( "alias" ) )
    {
      checkAttributes(
        "vk.xml", line, attributes, { { "alias", {} }, { "extends", {} }, { "name", {} } }, { { "api", { "vulkan" } }, { "deprecated", { "aliased" } } } );
    }
    else
    {
      checkAttributes( "vk.xml",
                       line,
                       attributes,
                       { { "extends", {} }, { "name", {} } },
                       { { "bitpos", {} }, { "comment", {} }, { "offset", {} }, { "dir", { "-" } }, { "extnumber", {} }, { "value", {} } } );
    }
  }
  else
  {
    checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, {} );
  }
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  RequireEnum requireEnum{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      requireEnum.alias = attribute.second;
    }
    else if ( attribute.first == "api" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      requireEnum.api = attribute.second;
    }
    else if ( attribute.first == "bitpos" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      checkNumber( attribute.second, line, "require enum with non-numeric bitPos" );
      requireEnum.bitPos = attribute.second;
    }
    else if ( attribute.first == "comment" )
    {
      requireEnum.comment = attribute.second;
    }
    else if ( attribute.first == "deprecated" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      requireEnum.deprectated = attribute.second;
    }
    else if ( attribute.first == "dir" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      requireEnum.dir = attribute.second;
    }
    else if ( attribute.first == "extends" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      requireEnum.extends = attribute.second;
    }
    else if ( attribute.first == "extnumber" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      checkNumber( attribute.second, line, "require enum with non-numeric extnumber" );
      requireEnum.extNumber = attribute.second;
      // CHECK: extnumber after extensions
    }
    else if ( attribute.first == "offset" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      checkNumber( attribute.second, line, "require enum with non-numeric offset" );
      requireEnum.offset = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      requireEnum.name = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      checkNumber( attribute.second, line, "require enum with non-numeric value" );
      requireEnum.value = attribute.second;
    }
  }

  checkForError( "vk.xml",
                 requireEnum.extends.empty() || !requireEnum.alias.empty() ||
                   ( !requireEnum.bitPos.empty() + !requireEnum.offset.empty() + !requireEnum.value.empty() ) == 1,
                 line,
                 "require enum <" + requireEnum.name +
                   "> has attribute <extends> but none or more than one of attributes <bitPos>, <offset>, and <value> are set" );

  return requireEnum;
}

RequireType parseRequireType( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, { { "comment", {} } } );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  RequireType type{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      type.comment = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      type.name = attribute.second;
    }
  }

  return type;
}

SPIRVCapabilities parseSPIRVCapabilities( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "spirvcapability", MultipleAllowed::Yes } } );

  SPIRVCapabilities spirvCapabilities{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      spirvCapabilities.comment = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string const value = child->Value();
    if ( value == "spirvcapability" )
    {
      SPIRVCapability spirvCapability = parseSPIRVCapability( child );
      checkForError( "vk.xml",
                     !containsByName( spirvCapabilities.capabilities, spirvCapability.name ),
                     spirvCapability.xmlLine,
                     "spirvcapability <" + spirvCapability.name + "> already listed" );
      spirvCapabilities.capabilities.push_back( std::move( spirvCapability ) );
    }
  }

  return spirvCapabilities;
}

SPIRVCapability parseSPIRVCapability( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "enable", MultipleAllowed::Yes } } );

  SPIRVCapability spirvCapability{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      spirvCapability.name = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string const value = child->Value();
    if ( value == "enable" )
    {
      spirvCapability.enables.push_back( parseSPIRVCapabilityEnable( child ) );
    }
  }

  return spirvCapability;
}

SPIRVCapabilityEnable parseSPIRVCapabilityEnable( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );

  SPIRVCapabilityEnable enable{ .xmlLine = line };
  if ( attributes.contains( "extension" ) )
  {
    checkAttributes( "vk.xml", line, attributes, { { "extension", {} } }, {} );

    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "extension" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        enable.extension = attribute.second;
      }
    }
  }
  else if ( attributes.contains( "property" ) )
  {
    checkAttributes( "vk.xml", line, attributes, { { "member", {} }, { "property", {} }, { "requires", {} }, { "value", {} } }, {} );

    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "member" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        enable.member = attribute.second;
      }
      else if ( attribute.first == "property" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        enable.property = attribute.second;
      }
      else if ( attribute.first == "requires" )
      {
        enable.require = tokenize( attribute.second, "," );
      }
      else if ( attribute.first == "value" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        enable.value = attribute.second;
      }
    }
  }
  else if ( attributes.contains( "struct" ) )
  {
    checkAttributes( "vk.xml", line, attributes, { { "feature", {} }, { "requires", {} }, { "struct", {} } }, { { "alias", {} } } );

    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "alias" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        enable.alias = attribute.second;
      }
      else if ( attribute.first == "feature" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        enable.feature = attribute.second;
      }
      else if ( attribute.first == "requires" )
      {
        enable.require = tokenize( attribute.second, "," );
      }
      else if ( attribute.first == "struct" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        enable.structure = attribute.second;
      }
    }
  }
  else if ( attributes.contains( "version" ) )
  {
    checkAttributes( "vk.xml", line, attributes, { { "version", {} } }, {} );

    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "version" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        enable.version = attribute.second;
      }
    }
  }

  checkElements( "vk.xml", line, getChildElements( element ), {} );

  return enable;
}

SPIRVExtension parseSPIRVExtension( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "enable", MultipleAllowed::Yes } } );

  SPIRVExtension spirvExtension{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      spirvExtension.name = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "enable" )
    {
      SPIRVExtensionEnable enable = parseSPIRVExtensionEnable( child );
      if ( !enable.version.empty() )
      {
        assert( !enable.version.empty() );
        checkForError( "vk.xml",
                       std::ranges::none_of( spirvExtension.enables, []( auto const & enable ) { return !enable.version.empty(); } ),
                       enable.xmlLine,
                       "spirvextension <" + spirvExtension.name + "> enables multiple versions" );
      }
      spirvExtension.enables.push_back( std::move( enable ) );
    }
  }

  return spirvExtension;
}

SPIRVExtensionEnable parseSPIRVExtensionEnable( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  if ( attributes.contains( "extension" ) )
  {
    checkAttributes( "vk.xml", line, attributes, { { "extension", {} } }, {} );
  }
  else
  {
    checkAttributes( "vk.xml", line, attributes, { { "version", {} } }, {} );
  }
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  SPIRVExtensionEnable enable{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "extension" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      enable.extension = attribute.second;
    }
    else if ( attribute.first == "version" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      enable.version = attribute.second;
    }
  }

  return enable;
}

SPIRVExtensions parseSPIRVExtensions( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "spirvextension", MultipleAllowed::Yes } } );

  SPIRVExtensions spirvExtensions{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      spirvExtensions.comment = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "spirvextension" )
    {
      spirvExtensions.extensions.push_back( parseSPIRVExtension( child ) );
    }
  }

  return spirvExtensions;
}

StructMember parseStructMember( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml",
                   line,
                   attributes,
                   {},
                   { { "altlen", {} },
                     { "api", { "vulkan", "vulkanbase", "vulkansc" } },
                     { "deprecated", { "ignored", "unused" } },
                     { "externsync", { "maybe", "true" } },
                     { "featurelink", {} },
                     { "flagsextend", {} },
                     { "flagsextendmember", {} },
                     { "len", {} },
                     { "limittype", { "bitmask", "bits", "exact", "max", "min", "mul", "noauto", "not", "pot", "range", "struct" } },
                     { "noautovalidity", { "true" } },
                     { "objecttype", { "objectType" } },
                     { "optional", { "false", "true" } },
                     { "selector", {} },
                     { "values", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml",
                 line,
                 children,
                 { { "name", MultipleAllowed::No }, { "type", MultipleAllowed::No } },
                 { { "comment", MultipleAllowed::No }, { "enum", MultipleAllowed::No } } );

  StructMember member{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "altlen" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      member.altLen = attribute.second;
    }
    else if ( attribute.first == "api" )
    {
      member.api = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "deprecated" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      member.deprecated = attribute.second;
    }
    else if ( attribute.first == "externsync" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      member.externSync = attribute.second;
    }
    else if ( attribute.first == "featurelink" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      member.featureLink = attribute.second;
    }
    else if ( attribute.first == "flagsextend" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      member.flagsExtend = attribute.second;
    }
    else if ( attribute.first == "flagsextendmember" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      member.flagsExtendMember = attribute.second;
    }
    else if ( attribute.first == "len" )
    {
      member.len = tokenize( attribute.second, "," );
      checkForError( "vk.xml", member.len.size() <= 2, line, "member attribute <len> holds unknown number of data: " + std::to_string( member.len.size() ) );
      if ( 1 < member.len.size() )
      {
        checkForError( "vk.xml",
                       ( member.len[1] == "1" ) || ( member.len[1] == "null-terminated" ),
                       line,
                       "member attribute <len> holds unknown second value <" + member.len[1] + ">" );
      }
    }
    else if ( attribute.first == "limittype" )
    {
      member.limitType = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "noautovalidity" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      member.noAutoValidity = attribute.second;
    }
    else if ( attribute.first == "objecttype" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      member.objectType = attribute.second;
    }
    else if ( attribute.first == "optional" )
    {
      member.optional = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "selector" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      member.selector = attribute.second;
    }
    else if ( attribute.first == "values" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      member.values = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "comment" )
    {
      member.comment = parseText( child );
    }
    else if ( value == "enum" )
    {
      assert( member.arraySizes.empty() );
      checkForError( "vk.xml",
                     child->PreviousSibling() && ( strcmp( child->PreviousSibling()->Value(), "[" ) == 0 ) && child->NextSibling() &&
                       ( strcmp( child->NextSibling()->Value(), "]" ) == 0 ),
                     line,
                     "array specifiation is ill-formatted" );
      member.arraySizes.push_back( parseText( child ) );
    }
    else if ( value == "name" )
    {
      std::tie( member.name, member.alias ) = parseNameWithAlias( child );
      NameModifiers modifiers               = parseNameModifiers( "vk.xml", child->NextSibling() );
      member.arraySizes                     = modifiers.arraySizes;
      member.bitCount                       = modifiers.bitCount;
    }
    else if ( value == "type" )
    {
      member.type = parseType( child );
    }
  }

  checkForError( "vk.xml", member.altLen.empty() || !member.len.empty(), line, "member <" + member.name + "> has attribute <altLen>, but no attribute <len>" );
  checkForError( "vk.xml",
                 member.len.empty() || !member.arraySizes.empty() || member.type.isPointer(),
                 line,
                 "member <" + member.name + "> has attribute <len> but is not a pointer" );
  checkForError( "vk.xml",
                 member.flagsExtend.empty() == member.flagsExtendMember.empty(),
                 line,
                 "member <" + member.name + "> has just one of the two attribute \"flagsextend\" and \"flagsextendmember\"" );
  // CHECK: values after extensions

  return member;
}

SupersededName parseSupersededName( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, { { "supersededby", {} } } );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  SupersededName deprecateCommand{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      deprecateCommand.name = attribute.second;
    }
    else if ( attribute.first == "supersededby" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      deprecateCommand.supersededBy = attribute.second;
    }
  }

  return deprecateCommand;
}

Sync parseSync( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, {}, { { "comment", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements(
    "vk.xml", line, children, { { "syncaccess", MultipleAllowed::Yes }, { "syncpipeline", MultipleAllowed::Yes }, { "syncstage", MultipleAllowed::Yes } }, {} );

  Sync sync{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      sync.comment = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "syncaccess" )
    {
      SyncAccess syncAccess = parseSyncAccess( child );
      checkForError( "vk.xml", !containsByName( sync.accesses, syncAccess.name ), syncAccess.xmlLine, "syncaccess <" + syncAccess.name + "> already listed" );
      sync.accesses.push_back( std::move( syncAccess ) );
    }
    else if ( value == "syncpipeline" )
    {
      SyncPipeline syncPipeline = parseSyncPipeline( child );
      checkForError(
        "vk.xml", !containsByName( sync.pipelines, syncPipeline.name ), syncPipeline.xmlLine, "syncpipeline <" + syncPipeline.name + "> already listed" );
      sync.pipelines.push_back( std::move( syncPipeline ) );
    }
    else if ( value == "syncstage" )
    {
      SyncStage syncStage = parseSyncStage( child );
      checkForError( "vk.xml", !containsByName( sync.stages, syncStage.name ), syncStage.xmlLine, "syncstage <" + syncStage.name + "> already listed" );
      sync.stages.push_back( std::move( syncStage ) );
    }
  }

  return sync;
}

SyncAccess parseSyncAccess( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, { { "alias", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements(
    "vk.xml", line, children, {}, { { "comment", MultipleAllowed::No }, { "syncequivalent", MultipleAllowed::No }, { "syncsupport", MultipleAllowed::No } } );

  SyncAccess syncAccess{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      syncAccess.alias = attribute.second;
      // CHECK: alias after extensions
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      syncAccess.name = attribute.second;
      // CHECK: name after extensions
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "comment" )
    {
      syncAccess.comment = parseComment( "vk.xml", child );
    }
    else if ( value == "syncequivalent" )
    {
      syncAccess.equivalent = std::make_optional( parseSyncAccessEquivalent( child ) );
    }
    else if ( value == "syncsupport" )
    {
      syncAccess.support = std::make_optional( parseSyncAccessSupport( child ) );
    }
  }

  return syncAccess;
}

SyncAccessEquivalent parseSyncAccessEquivalent( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "access", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {}, {} );

  SyncAccessEquivalent syncEquivalent{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "access" )
    {
      syncEquivalent.accesses = tokenize( attribute.second, "," );
      // CHECK: stages after extensions
    }
  }

  return syncEquivalent;
}

SyncAccessSupport parseSyncAccessSupport( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "stage", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {}, {} );

  SyncAccessSupport syncSupport{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "stage" )
    {
      syncSupport.stages = tokenize( attribute.second, "," );
      // CHECK: stages after extensions
    }
  }

  return syncSupport;
}

SyncPipeline parseSyncPipeline( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, { { "depends", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, {}, { { "syncpipelinestage", MultipleAllowed::Yes } } );

  SyncPipeline syncPipeline{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "depends" )
    {
      syncPipeline.depends = tokenize( attribute.second, "," );
      // CHECK: depends after extensions
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      syncPipeline.name = attribute.second;
    }
  }

  for ( auto const & child : children )
  {
    std::string value = child->Value();
    if ( value == "syncpipelinestage" )
    {
      SyncPipelineStage syncPipelineStage = parseSyncPipelineStage( child );
      checkForError( "vk.xml",
                     !containsByName( syncPipeline.stages, syncPipelineStage.name ),
                     syncPipelineStage.xmlLine,
                     "syncpipelinestage <" + syncPipelineStage.name + "> already listed for syncpipeline <" + syncPipeline.name + ">" );
      syncPipeline.stages.push_back( std::move( syncPipelineStage ) );
    }
  }

  return syncPipeline;
}

SyncPipelineStage parseSyncPipelineStage( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, {}, { { "before", { "VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT" } }, { "order", { "None" } } } );
  checkElements( "vk.xml", line, getChildElements( element ), {}, {} );

  SyncPipelineStage syncPipelineStage{ .name = element->GetText(), .xmlLine = line };
  // CHECK: name after extensions

  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "before" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      syncPipelineStage.before = attribute.second;
    }
    else if ( attribute.first == "order" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      syncPipelineStage.order = attribute.second;
    }
  }

  return syncPipelineStage;
}

SyncStage parseSyncStage( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, { { "alias", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, {}, { { "syncequivalent", MultipleAllowed::No }, { "syncsupport", MultipleAllowed::No } } );

  SyncStage syncStage{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      syncStage.alias = attribute.second;
      // CHECK: alias after extensions
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      syncStage.name = attribute.second;
      // CHECK: name after extensions
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "syncequivalent" )
    {
      syncStage.equivalent = std::make_optional( parseSyncStageEquivalent( child ) );
    }
    else if ( value == "syncsupport" )
    {
      syncStage.support = std::make_optional( parseSyncStageSupport( child ) );
    }
  }

  return syncStage;
}

SyncStageEquivalent parseSyncStageEquivalent( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "stage", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {}, {} );

  SyncStageEquivalent syncEquivalent{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "stage" )
    {
      syncEquivalent.stages = tokenize( attribute.second, "," );
      // CHECK: stages after extensions
    }
  }

  return syncEquivalent;
}

SyncStageSupport parseSyncStageSupport( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "queues", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {}, {} );

  SyncStageSupport syncSupport{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "queues" )
    {
      syncSupport.queues = tokenize( attribute.second, "," );
      // CHECK: queues after extensions
    }
  }

  return syncSupport;
}

Tag parseTag( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "author", {} }, { "contact", {} }, { "name", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  Tag tag{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "author" )
    {
      tag.author = attribute.second;
    }
    else if ( attribute.first == "contact" )
    {
      tag.contact = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      tag.name = attribute.second;
    }
  }

  return tag;
}

Tags parseTags( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "tag", MultipleAllowed::Yes } } );

  Tags tags{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      tags.comment = attribute.second;
    }
  }

  for ( auto child : children )
  {
    Tag tag = parseTag( child );

    checkForError( "vk.xml", !containsByName( tags.tags, tag.name ), tag.xmlLine, "tag <" + tag.name + "> already specfied" );

    tags.tags.push_back( std::move( tag ) );
  }

  return tags;
}

std::string parseText( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  std::string name = element->GetText();

  return name;
}

Type parseType( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {}, {} );

  Type type;
  type.name = element->GetText();

  tinyxml2::XMLNode const * preType = element->PreviousSibling();
  if ( preType && preType->ToText() )
  {
    std::string prefix = trim( preType->Value() );
    checkForError( "vk.xml",
                   ( prefix == "const" ) || ( prefix == "const struct" ) || ( prefix == "struct" ) || ( prefix == "typedef" ),
                   line,
                   "unexpected text <" + prefix + "> before type" );
    // change from west-const to east-const
    if ( prefix.starts_with( "const" ) )
    {
      type.postfix = "const";
    }
    if ( prefix.find( "struct" ) != std::string::npos )
    {
      type.prefix = "struct";
    }
  }

  tinyxml2::XMLNode const * postType = element->NextSibling();
  if ( postType && postType->ToText() )
  {
    std::string postfix = trim( postType->Value() );
    if ( auto pos = postfix.find( "const*" ); pos != std::string::npos )
    {
      postfix.replace( pos, strlen( "const*" ), "const *" );
    }
    checkForError( "vk.xml",
                   ( postfix == "*" ) || ( postfix == "**" ) || ( postfix == "* const*" ) || ( postfix == "* const *" ) || ( postfix == "(" ),
                   line,
                   "unexpected text <" + postfix + "> after type" );
    if ( postfix != "(" )
    {
      type.postfix += ( type.postfix.empty() ? "" : " " ) + postfix;
    }
  }
  return type;
}

TypeBaseType parseTypeBaseType( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, attributes, { { "category", { "basetype" } } }, {} );

  TypeBaseType baseType{ .xmlLine = line };
  // we're ignoring all the text nodes in between, as they are just some C-text with ifdefs and such
  std::tie( baseType.name, baseType.type ) = parseNameAndType( element );

  return baseType;
}

BitmaskVariant parseTypeBitmask( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  if ( attributes.contains( "alias" ) )
  {
    return parseAlias( element, attributes, "bitmask" );
  }
  else
  {
    int const line = element->GetLineNum();
    checkAttributes( "vk.xml",
                     line,
                     attributes,
                     { { "category", { "bitmask" } } },
                     { { "api", { "vulkan", "vulkanbase", "vulkansc" } }, { "bitvalues", {} }, { "requires", {} } } );

    TypeBitmask bitmask{ .api = { "vulkan" }, .xmlLine = line };  // default is vulkan, if api attribute is not specified
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "api" )
      {
        bitmask.api = tokenize( attribute.second, "," );
      }
      else if ( attribute.first == "bitvalues" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        bitmask.bitValues = attribute.second;
      }
      else if ( attribute.first == "requires" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        bitmask.require = attribute.second;
      }
    }

    checkForError( "vk.xml", bitmask.bitValues.empty() || bitmask.require.empty(), line, "attributes <bitvalues> and <requires> are both specified" );

    std::tie( bitmask.name, bitmask.type ) = parseNameAndType( element );

    checkForWarning(
      "vk.xml", ( bitmask.type.name == "VkFlags" ) || ( bitmask.type.name == "VkFlags64" ), line, "unexpected bitmask type <" + bitmask.type.name + ">" );
    checkForError( "vk.xml", bitmask.type.postfix != "*", line, "unexpected pointer type <" + bitmask.type.name + ">" );
    checkForError(
      "vk.xml", ( bitmask.type.name != "VkFlags64" ) || !bitmask.bitValues.empty(), line, "bitmask of type <VkFlags64> needs attribute bitvalues to be set" );

    return bitmask;
  }
}

TypeDefine parseTypeDefine( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml",
                   line,
                   attributes,
                   { { "category", { "define" } } },
                   { { "api", { "vulkan", "vulkanbase", "vulkansc" } }, { "comment", {} }, { "name", {} }, { "requires", {} } } );

  TypeDefine define{ .api = { "vulkan" }, .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "api" )
    {
      define.api = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      define.name = attribute.second;
    }
    else if ( attribute.first == "requires" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      define.require = attribute.second;
    }
  }

  if ( !define.name.empty() )
  {
    checkForError( "vk.xml", !element->FirstChildElement(), line, "unknown formatting of type category=define name <" + define.name + ">" );
    checkForError( "vk.xml",
                   element->LastChild() && element->LastChild()->ToText() && element->LastChild()->ToText()->Value(),
                   line,
                   "unknown formatting of type category=define named <" + define.name + ">" );
  }
  else if ( element->GetText() )
  {
    std::string                  text  = element->GetText();
    tinyxml2::XMLElement const * child = element->FirstChildElement();
    checkForError( "vk.xml",
                   child && !child->FirstAttribute() && ( strcmp( child->Value(), "name" ) == 0 ) && child->GetText(),
                   line,
                   "unknown formatting of type category define" );
    define.name = trim( child->GetText() );
    //  ignore all the other defines
    checkForWarning( "vk.xml",
                     !child->NextSiblingElement() ||
                       ( child->NextSiblingElement() && !child->NextSiblingElement()->FirstAttribute() &&
                         ( strcmp( child->NextSiblingElement()->Value(), "type" ) == 0 ) && !child->NextSiblingElement()->NextSiblingElement() ),
                     line,
                     "unknown formatting of type category define" );
  }
  assert( !define.name.empty() );

  MacroVisitor definesVisitor{};
  element->Accept( &definesVisitor );
  define.macro = definesVisitor.macro;

  return define;
}

EnumVariant parseTypeEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  if ( attributes.contains( "alias" ) )
  {
    return parseAlias( element, attributes, "enum" );
  }
  else
  {
    int const line = element->GetLineNum();
    checkAttributes( "vk.xml", line, attributes, { { "category", { "enum" } }, { "name", {} } }, {} );
    checkElements( "vk.xml", line, getChildElements( element ), {} );

    TypeEnum typeEnum{ .xmlLine = line };
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "name" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        typeEnum.name = attribute.second;
      }
    }

    return typeEnum;
  }
}

TypeFuncPointer parseTypeFuncPointer( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, attributes, { { "category", { "funcpointer" } } }, { { "requires", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "proto", MultipleAllowed::No } }, { { "param", MultipleAllowed::Yes } } );

  TypeFuncPointer funcPointer{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "requires" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      funcPointer.require = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "param" )
    {
      int childLine               = child->GetLineNum();
      auto [paramName, paramType] = parseNameAndType( child );
      assert( !paramName.empty() );
      checkForError( "vk.xml",
                     !containsByName( funcPointer.params, paramName ),
                     childLine,
                     "param <" + paramName + "> already listed for funcPointer <" + funcPointer.name + ">" );
      funcPointer.params.push_back( { .name = paramName, .type = paramType, .xmlLine = childLine } );
    }
    else if ( value == "proto" )
    {
      std::tie( funcPointer.name, funcPointer.returnType ) = parseNameAndType( child );
      checkForError( "vk.xml", funcPointer.name.starts_with( "PFN_vk" ), line, "name <" + funcPointer.name + "> does not begin with <PFN_vk>" );
    }
  }

  return funcPointer;
}

HandleVariant parseTypeHandle( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  if ( attributes.contains( "alias" ) )
  {
    return parseAlias( element, attributes, "handle" );
  }
  else
  {
    int const line = element->GetLineNum();
    checkAttributes( "vk.xml", line, attributes, { { "category", { "handle" } }, { "objtypeenum", {} } }, { { "parent", {} } } );

    TypeHandle handle{ .xmlLine = line };
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "objtypeenum" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        handle.objTypeEnum = attribute.second;
      }
      else if ( attribute.first == "parent" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        handle.parent = attribute.second;
      }
    }

    std::tie( handle.name, handle.type ) = parseNameAndType( element );

    checkForError( "vk.xml",
                   ( handle.type.name == "VK_DEFINE_HANDLE" ) || ( handle.type.name == "VK_DEFINE_NON_DISPATCHABLE_HANDLE" ),
                   line,
                   "handle with invalid type <" + handle.type.name + ">" );

    return handle;
  }
}

StructVariant parseTypeStruct( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes, std::string const & api )
{
  if ( attributes.contains( "alias" ) )
  {
    return parseAlias( element, attributes, "struct" );
  }
  else
  {
    int const line = element->GetLineNum();
    checkAttributes( "vk.xml",
                     line,
                     attributes,
                     { { "category", { "struct" } }, { "name", {} } },
                     { { "allowduplicate", { "false", "true" } },
                       { "comment", {} },
                       { "requiredlimittype", { "true" } },
                       { "returnedonly", { "true" } },
                       { "structextends", {} } } );
    std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
    checkElements( "vk.xml", line, children, { { "member", MultipleAllowed::Yes } }, { { "comment", MultipleAllowed::Yes } } );

    TypeStruct typeStruct{ .xmlLine = line };
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "allowduplicate" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        typeStruct.allowDuplicate = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        typeStruct.name = attribute.second;
      }
      else if ( attribute.first == "requiredlimittype" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        typeStruct.requiredLimitType = attribute.second;
      }
      else if ( attribute.first == "returnedonly" )
      {
        checkNoList( "vk.xml", attribute.second, line );
        typeStruct.returnedOnly = attribute.second;
      }
      else if ( attribute.first == "structextends" )
      {
        typeStruct.structExtends = tokenize( attribute.second, "," );
      }
    }

    for ( auto child : children )
    {
      std::string value = child->Value();
      if ( value == "member" )
      {
        StructMember member = parseStructMember( child );

        // `VkDeviceCreateInfo::ppEnabledLayerNames` needs to be special-cased with old attributes to maintain API compatibility
        // See https://github.com/KhronosGroup/Vulkan-Hpp/issues/2531
        if ( ( typeStruct.name == "VkDeviceCreateInfo" ) && ( member.name == "ppEnabledLayerNames" ) )
        {
          member.len = { "enabledLayerCount", "null-terminated" };
        }

        if ( member.api.empty() || std::ranges::any_of( member.api, [&api]( auto const & a ) { return a == api; } ) )
        {
          if ( !member.selector.empty() )
          {
            checkForError( "vk.xml",
                           containsByName( typeStruct.members, member.selector ),
                           line,
                           "struct member <" + member.name + "> references unknown struct member <" + member.selector + "> in its selector attribute" );
          }

          checkForError( "vk.xml",
                         !containsByName( typeStruct.members, member.name ),
                         line,
                         "member <" + member.name + "> already listed for struct <" + typeStruct.name + ">" );

          typeStruct.members.push_back( std::move( member ) );
        }
      }
    }

    auto memberIt = std::ranges::find_if( typeStruct.members, []( auto const & member ) { return !member.altLen.empty(); } );
    if ( memberIt != typeStruct.members.end() )
    {
      checkForError( "vk.xml",
                     ( memberIt->len.size() == 1 ) && memberIt->len[0].starts_with( "latexmath:" ),
                     memberIt->xmlLine,
                     "struct member <" + memberIt->name + "> has attribute <altLen> but attribute <len> does not start with \"latexmath:\"" );
    }
    memberIt =
      std::ranges::find_if( typeStruct.members, []( auto const & member ) { return ( member.len.size() == 1 ) && member.len[0].starts_with( "latexmath:" ); } );
    if ( memberIt != typeStruct.members.end() )
    {
      checkForError( "vk.xml",
                     !memberIt->altLen.empty(),
                     memberIt->xmlLine,
                     "struct member <" + memberIt->name + "> has attribute <len> starting with \"latexmath:\" but no attribute <altLen>" );
    }

    checkForError( "vk.xml",
                   ( typeStruct.requiredLimitType != "true" ) ||
                     std::ranges::all_of( typeStruct.members,
                                          []( StructMember const & member )
                                          { return ( member.name == "sType" ) || ( member.name == "pNext" ) || !member.limitType.empty(); } ),
                   line,
                   "struct <" + typeStruct.name + "> requires all members to have a limittype" );

    return typeStruct;
  }
}

TypeUnion parseTypeUnion( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, attributes, { { "category", { "union" } }, { "name", {} } }, { { "comment", {} }, { "returnedonly", { "true" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "member", MultipleAllowed::Yes } }, { { "comment", MultipleAllowed::Yes } } );

  TypeUnion typeUnion{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      typeUnion.name = attribute.second;
    }
    else if ( attribute.first == "returnedonly" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      typeUnion.returnedOnly = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "member" )
    {
      UnionMember member = parseUnionMember( child );
      checkForError(
        "vk.xml", !containsByName( typeUnion.members, member.name ), line, "member <" + member.name + "> already listed for union <" + typeUnion.name + ">" );
      typeUnion.members.push_back( std::move( member ) );
    }
  }

  return typeUnion;
}

Types parseTypes( tinyxml2::XMLElement const * element, std::string const & api )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, { { "comment", {} } } );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "comment", MultipleAllowed::Yes }, { "type", MultipleAllowed::Yes } } );

  Types types;
  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "type" )
    {
      TypeVariant type = parseTypesType( child, api );

      if ( std::holds_alternative<TypeBaseType>( type ) )
      {
        auto const & baseType = std::get<TypeBaseType>( type );

        checkForError(
          "vk.xml", types.types.insert( baseType.name ).second, baseType.xmlLine, "base type <" + baseType.name + "> already specified as a type" );
        checkForError( "vk.xml", !containsByName( types.baseTypes, baseType.name ), baseType.xmlLine, "basetype <" + baseType.name + "> already specified" );
        types.baseTypes.push_back( std::move( baseType ) );
      }
      else if ( std::holds_alternative<BitmaskVariant>( type ) )
      {
        auto const & bitmaskVariant = std::get<BitmaskVariant>( type );
        if ( std::holds_alternative<Alias>( bitmaskVariant ) )
        {
          auto const & alias = std::get<Alias>( bitmaskVariant );

          checkForError( "vk.xml", types.types.insert( alias.name ).second, alias.xmlLine, "bitmask alias <" + alias.name + "> already specified as a type" );
          auto bitmaskIt = findByName( types.bitmasks, alias.alias );
          checkForError( "vk.xml",
                         bitmaskIt != types.bitmasks.end(),
                         alias.xmlLine,
                         "bitmask <" + alias.name + "> is an alias of an unknown bitmask <" + alias.alias + ">." );
          checkForError( "vk.xml",
                         bitmaskIt->aliases.insert( { alias.name, alias.xmlLine } ).second,
                         alias.xmlLine,
                         "bitmask alias <" + alias.name + "> is already listed as an alias for bitmask <" + alias.alias + ">" );
        }
        else
        {
          assert( std::holds_alternative<TypeBitmask>( bitmaskVariant ) );
          auto const & bitmask = std::get<TypeBitmask>( bitmaskVariant );

          if ( std::ranges::any_of( bitmask.api, [&api]( auto const & a ) { return a == api; } ) )
          {
            checkForError( "vk.xml", types.types.insert( bitmask.name ).second, bitmask.xmlLine, "bitmask <" + bitmask.name + "> already specified as a type" );
            checkForError( "vk.xml", !containsByName( types.bitmasks, bitmask.name ), bitmask.xmlLine, "bitmask <" + bitmask.name + "> already specified" );
            types.bitmasks.push_back( std::move( bitmask ) );
          }
        }
      }
      else if ( std::holds_alternative<TypeDefine>( type ) )
      {
        auto const & define = std::get<TypeDefine>( type );

        if ( std::ranges::any_of( define.api, [&api]( auto const & a ) { return a == api; } ) )
        {
          checkForError( "vk.xml", types.types.insert( define.name ).second, define.xmlLine, "define <" + define.name + "> already specified as a type" );
          checkForError( "vk.xml", !containsByName( types.defines, define.name ), define.xmlLine, "define <" + define.name + "> already specified" );
          types.defines.push_back( std::move( define ) );
        }
      }
      else if ( std::holds_alternative<EnumVariant>( type ) )
      {
        auto const & enumVariant = std::get<EnumVariant>( type );

        if ( std::holds_alternative<Alias>( enumVariant ) )
        {
          auto const & alias = std::get<Alias>( enumVariant );

          checkForError( "vk.xml", types.types.insert( alias.name ).second, alias.xmlLine, "enum alias <" + alias.name + "> already specified as a type" );

          auto enumIt = findByName( types.enums, alias.alias );
          checkForError(
            "vk.xml", enumIt != types.enums.end(), alias.xmlLine, "enum <" + alias.name + "> is an alias of an unknown enum <" + alias.alias + ">." );
          checkForError( "vk.xml",
                         enumIt->aliases.insert( { alias.name, alias.xmlLine } ).second,
                         alias.xmlLine,
                         "enum alias <" + alias.name + "> is already listed as an alias for enum <" + alias.alias + ">" );
        }
        else
        {
          assert( std::holds_alternative<TypeEnum>( enumVariant ) );
          auto const & enumData = std::get<TypeEnum>( enumVariant );

          checkForError( "vk.xml", types.types.insert( enumData.name ).second, enumData.xmlLine, "enum <" + enumData.name + "> already specified as a type" );
          checkForError( "vk.xml", !containsByName( types.enums, enumData.name ), enumData.xmlLine, "enum <" + enumData.name + "> already specified" );
          types.enums.push_back( std::move( enumData ) );
        }
      }
      else if ( std::holds_alternative<TypeFuncPointer>( type ) )
      {
        auto const & funcPointer = std::get<TypeFuncPointer>( type );

        checkForError(
          "vk.xml", types.types.insert( funcPointer.name ).second, funcPointer.xmlLine, "funcpointer <" + funcPointer.name + "> already specified as a type" );
        checkForError(
          "vk.xml", !containsByName( types.funcPointers, funcPointer.name ), funcPointer.xmlLine, "funcpointer <" + funcPointer.name + "> already specified" );
        types.funcPointers.push_back( std::move( funcPointer ) );
      }
      else if ( std::holds_alternative<HandleVariant>( type ) )
      {
        auto const & handleVariant = std::get<HandleVariant>( type );

        if ( std::holds_alternative<Alias>( handleVariant ) )
        {
          auto const & alias = std::get<Alias>( handleVariant );

          checkForError( "vk.xml", types.types.insert( alias.name ).second, alias.xmlLine, "handle alias <" + alias.name + "> already specified as a type" );
          auto handleIt = findByName( types.handles, alias.alias );
          checkForError(
            "vk.xml", handleIt != types.handles.end(), alias.xmlLine, "handle <" + alias.name + "> is an alias of an unknown handle <" + alias.alias + ">." );
          checkForError( "vk.xml",
                         handleIt->aliases.insert( { alias.name, alias.xmlLine } ).second,
                         alias.xmlLine,
                         "handle alias <" + alias.name + "> is already listed as an alias for handle <" + alias.alias + ">" );
        }
        else
        {
          assert( std::holds_alternative<TypeHandle>( handleVariant ) );
          auto const & handle = std::get<TypeHandle>( handleVariant );

          checkForError( "vk.xml", types.types.insert( handle.name ).second, handle.xmlLine, "handle <" + handle.name + "> already specified as a type" );
          checkForError( "vk.xml", !containsByName( types.handles, handle.name ), handle.xmlLine, "handle <" + handle.name + "> already specified" );
          types.handles.push_back( std::move( handle ) );
        }
      }
      else if ( std::holds_alternative<TypeInclude>( type ) )
      {
        auto const & include = std::get<TypeInclude>( type );

        checkForError( "vk.xml", types.types.insert( include.name ).second, include.xmlLine, "include <" + include.name + "> already specified as a type" );
        checkForError( "vk.xml", !containsByName( types.includes, include.name ), include.xmlLine, "include <" + include.name + "> already specified" );
        types.includes.push_back( std::move( include ) );
      }
      else if ( std::holds_alternative<StructVariant>( type ) )
      {
        auto const & structVariant = std::get<StructVariant>( type );

        if ( std::holds_alternative<Alias>( structVariant ) )
        {
          auto const & alias = std::get<Alias>( structVariant );

          checkForError( "vk.xml", types.types.insert( alias.name ).second, alias.xmlLine, "struct alias <" + alias.name + "> already specified as a type" );
          checkForError( "vk.xml", !containsByName( types.structAliases, alias.name ), alias.xmlLine, "struct alias <" + alias.name + "> already encountered" );
          types.structAliases.push_back( std::move( alias ) );
        }
        else
        {
          assert( std::holds_alternative<TypeStruct>( structVariant ) );
          auto const & structure = std::get<TypeStruct>( structVariant );

          checkForError(
            "vk.xml", types.types.insert( structure.name ).second, structure.xmlLine, "struct <" + structure.name + "> already specified as a type" );
          checkForError( "vk.xml", !containsByName( types.structs, structure.name ), structure.xmlLine, "struct <" + structure.name + "> already specified" );
          types.structs.push_back( std::move( structure ) );
        }
      }
      else if ( std::holds_alternative<TypeUnion>( type ) )
      {
        auto const & typeUnion = std::get<TypeUnion>( type );

        checkForError( "vk.xml", types.types.insert( typeUnion.name ).second, typeUnion.xmlLine, "union <" + typeUnion.name + "> already specified as a type" );
        checkForError( "vk.xml", !containsByName( types.unions, typeUnion.name ), typeUnion.xmlLine, "union <" + typeUnion.name + "> already specified" );
        types.unions.push_back( std::move( typeUnion ) );
      }
      else
      {
        assert( std::holds_alternative<TypeExternal>( type ) );
        auto const & external = std::get<TypeExternal>( type );

        checkForError(
          "vk.xml", types.types.insert( external.name ).second, external.xmlLine, "external type <" + external.name + "> already specified as a type" );
        checkForError( "vk.xml",
                       containsByName( types.includes, external.require ),
                       external.xmlLine,
                       "external type <" + external.name + "> requires unknown <" + external.require + ">" );
        checkForError(
          "vk.xml", !containsByName( types.externals, external.name ), external.xmlLine, "external type <" + external.name + "> already specified" );
        types.externals.push_back( std::move( external ) );
      }
    }
  }

  // structs might alias a struct that's specified later than the alias !
  for ( auto structAlias : types.structAliases )
  {
    auto structIt = findByName( types.structs, structAlias.alias );
    checkForError( "vk.xml",
                   structIt != types.structs.end(),
                   structAlias.xmlLine,
                   "struct <" + structAlias.name + "> is an alias of an unknown struct <" + structAlias.alias + ">." );
    checkForError( "vk.xml",
                   structIt->aliases.insert( { structAlias.name, structAlias.xmlLine } ).second,
                   structAlias.xmlLine,
                   "struct alias <" + structAlias.name + "> is already listed as an alias for struct <" + structAlias.alias + ">" );
  }
  types.structAliases.clear();

  for ( auto const & define : types.defines )
  {
    if ( !define.require.empty() )
    {
      checkForError( "vk.xml",
                     containsByName( types.defines, define.require ),
                     define.xmlLine,
                     "define <" + define.name + "> requires unknown define <" + define.require + ">" );
    }
  }
  for ( auto const & funcPointer : types.funcPointers )
  {
    if ( !funcPointer.require.empty() )
    {
      checkForError( "vk.xml",
                     containsByName( types.handles, funcPointer.require ) || containsByName( types.structs, funcPointer.require ),
                     funcPointer.xmlLine,
                     "funcpointer <" + funcPointer.name + "> requires unknown type <" + funcPointer.require + ">" );
    }
  }
  for ( auto const & handle : types.handles )
  {
    checkForError( "vk.xml",
                   handle.parent.empty() || containsByName( types.handles, handle.parent ),
                   handle.xmlLine,
                   "handle <" + handle.name + "> specifies unknown parent handle <" + handle.parent + ">" );
  }
  for ( auto const & structure : types.structs )
  {
    for ( auto const & member : structure.members )
    {
      if ( !member.flagsExtend.empty() )
      {
        auto extendIt = findByName( types.structs, member.flagsExtend );
        checkForError( "vk.xml",
                       extendIt != types.structs.end(),
                       member.xmlLine,
                       "struct member <" + member.name + "> in struct <" + structure.name + "> specifies unknown struct <" + member.flagsExtend +
                         "> as \"flagsextend\"" );
        checkForError(
          "vk.xml",
          std::ranges::any_of( extendIt->structExtends, [&structName = structure.name]( auto const & extendsName ) { return extendsName == structName; } ),
          member.xmlLine,
          "struct member <" + member.name + "> in struct <" + structure.name + "> specifies struct <" + extendIt->name + "> as \"flagsextend\" but <" +
            extendIt->name + "> does not extend <" + structure.name + ">" );

        assert( !member.flagsExtendMember.empty() );
        checkForError( "vk.xml",
                       containsByName( extendIt->members, member.flagsExtendMember ),
                       member.xmlLine,
                       "struct member <" + member.name + "> in struct <" + structure.name + "> specifies unknown member <" + member.flagsExtendMember +
                         "> in struct <" + extendIt->name + "> as \"flagsextendmember\"" );
      }
      if ( !member.selector.empty() )
      {
        checkForError( "vk.xml",
                       !containsByName( types.enums, member.selector ),
                       member.xmlLine,
                       "struct member <" + member.name + "> in struct <" + structure.name + "> references unknown selector enum <" + member.selector + ">" );
        checkForError( "vk.xml",
                       containsByName( types.unions, member.type.name ),
                       member.xmlLine,
                       "struct member <" + member.name + "> in struct <" + structure.name + "> has selector <" + member.selector + "> but its type <" +
                         member.type.name + "> is not a union" );
      }
      checkForError( "vk.xml",
                     types.types.contains( member.type.name ),
                     member.xmlLine,
                     "struct member <" + member.name + "> in struct <" + structure.name + "> has unknown type <" + member.type.name + ">" );
    }

    for ( auto const & structExtend : structure.structExtends )
    {
      checkForError( "vk.xml",
                     containsByName( types.structs, structExtend ),
                     structure.xmlLine,
                     "struct <" + structure.name + "> extends unknown struct <" + structExtend + ">" );
    }

    auto memberIt = std::ranges::find_if( structure.members, []( auto const & member ) { return !member.values.empty(); } );
    if ( memberIt != structure.members.end() )
    {
      checkForError( "vk.xml",
                     !containsByName( types.enums, memberIt->values ),
                     memberIt->xmlLine,
                     "struct member <" + memberIt->name + "> in struct <" + structure.name + "> references unknown enum <" + memberIt->values + ">" );
    }
  }

  return types;
}

TypeVariant parseTypesType( tinyxml2::XMLElement const * element, std::string const & api )
{
  std::map<std::string, std::string> attributes = getAttributes( element );

  auto categoryIt = attributes.find( "category" );
  if ( categoryIt != attributes.end() )
  {
    if ( categoryIt->second == "basetype" )
    {
      return parseTypeBaseType( element, attributes );
    }
    else if ( categoryIt->second == "bitmask" )
    {
      return parseTypeBitmask( element, attributes );
    }
    else if ( categoryIt->second == "define" )
    {
      return parseTypeDefine( element, attributes );
    }
    else if ( categoryIt->second == "enum" )
    {
      return parseTypeEnum( element, attributes );
    }
    else if ( categoryIt->second == "funcpointer" )
    {
      return parseTypeFuncPointer( element, attributes );
    }
    else if ( categoryIt->second == "handle" )
    {
      return parseTypeHandle( element, attributes );
    }
    else if ( categoryIt->second == "include" )
    {
      return parseTypeInclude( "vk.xml", element, attributes );
    }
    else if ( categoryIt->second == "struct" )
    {
      return parseTypeStruct( element, attributes, api );
    }
    else
    {
      checkForError( "vk.xml", categoryIt->second == "union", 0, "unknown category <" + categoryIt->second + "> encountered" );

      return parseTypeUnion( element, attributes );
    }
  }
  else
  {
    auto requiresIt = attributes.find( "requires" );
    checkForError( "vk.xml", requiresIt != attributes.end(), element->GetLineNum(), "encountered type without category and requires" );

    return parseTypeExternal( "vk.xml", element, attributes );
  }
}

UnionMember parseUnionMember( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes(
    "vk.xml", line, attributes, {}, { { "noautovalidity", { "true" } }, { "len", { "null-terminated" } }, { "optional", { "true" } }, { "selection", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "name", MultipleAllowed::No }, { "type", MultipleAllowed::No } }, { { "comment", MultipleAllowed::No } } );

  UnionMember member{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "len" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      member.len = attribute.second;
    }
    else if ( attribute.first == "noautovalidity" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      member.noAutoValidity = attribute.second;
    }
    else if ( attribute.first == "optional" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      member.optional = attribute.second;
    }
    else if ( attribute.first == "selection" )
    {
      member.selection = tokenize( attribute.second, "," );
    }
  }

  std::string bitCount;
  std::tie( member.name, member.type, member.arraySizes, bitCount ) = parseNameAndTypeModified( element );
  checkForError( "vk.xml", bitCount.empty(), line, "unexpected array size or bit count specification for name <" + member.name + ">" );
  assert( !member.name.empty() );

  return member;
}

VideoCapabilities parseVideoCapabilities( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "struct", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {}, {} );

  VideoCapabilities videoCapabilities{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "struct" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoCapabilities.structure = attribute.second;
    }
  }

  return videoCapabilities;
}

VideoCodec parseVideoCodec( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, { { "extend", {} }, { "value", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml",
                 line,
                 children,
                 { { "videocapabilities", MultipleAllowed::Yes } },
                 { { "videoformat", MultipleAllowed::Yes }, { "videoprofiles", MultipleAllowed::No } } );

  VideoCodec videoCodec{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "extend" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoCodec.extend = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoCodec.name = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoCodec.value = attribute.second;
      // CHECK: value after extensions
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "videocapabilities" )
    {
      VideoCapabilities videoCapabilities = parseVideoCapabilities( child );
      checkForError( "vk.xml",
                     std::ranges::none_of( videoCodec.videoCapabilities,
                                           [&videoCapabilities]( VideoCapabilities const & vc ) { return vc.structure == videoCapabilities.structure; } ),
                     videoCapabilities.xmlLine,
                     "videocodec <" + videoCodec.name + "> already lists videocapabilities on <" + videoCapabilities.structure + ">" );
      videoCodec.videoCapabilities.push_back( std::move( videoCapabilities ) );
    }
    else if ( value == "videoformat" )
    {
      VideoFormat videoFormat = parseVideoFormat( child );
      checkForError( "vk.xml",
                     videoFormat.name.empty() || !containsByName( videoCodec.videoFormats, videoFormat.name ),
                     videoFormat.xmlLine,
                     "videocodec <" + videoCodec.name + "> already lists a videoformat <" + videoFormat.name + ">" );
      videoCodec.videoFormats.push_back( std::move( videoFormat ) );
    }
    else if ( value == "videoprofiles" )
    {
      videoCodec.videoProfiles = std::make_optional<VideoProfiles>( parseVideoProfiles( child ) );
    }
  }

  return videoCodec;
}

std::vector<VideoCodec> parseVideoCodecs( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "videocodec", MultipleAllowed::Yes } } );

  std::vector<VideoCodec> videoCodecs;
  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "videocodec" )
    {
      VideoCodec videoCodec = parseVideoCodec( child );
      checkForError( "vk.xml", !containsByName( videoCodecs, videoCodec.name ), line, "video codec <" + videoCodec.name + "> already specified" );
      if ( !videoCodec.extend.empty() )
      {
        checkForError( "vk.xml",
                       containsByName( videoCodecs, videoCodec.extend ),
                       line,
                       "video codec <" + videoCodec.name + "> extends unknown video codec <" + videoCodec.extend + ">" );
      }
      for ( auto const & videoFormat : videoCodec.videoFormats )
      {
        if ( !videoFormat.extend.empty() )
        {
          checkForError(
            "vk.xml",
            std::ranges::any_of( videoCodecs, [&videoFormat]( VideoCodec const & vc ) { return containsByName( vc.videoFormats, videoFormat.extend ); } ),
            videoFormat.xmlLine,
            "videocodec <" + videoCodec.name + "> extends unknown video format <" + videoFormat.extend + ">" );
        }
      }
      videoCodecs.push_back( std::move( videoCodec ) );
    }
  }

  return videoCodecs;
}

VideoFormat parseVideoFormat( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  if ( attributes.contains( "extend" ) )
  {
    checkAttributes( "vk.xml", line, attributes, { { "extend", {} } }, {} );
  }
  else
  {
    checkAttributes( "vk.xml", line, attributes, { { "name", {} }, { "usage", {} } }, {} );
  }
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, {}, { { "videoformatproperties", MultipleAllowed::No }, { "videorequirecapabilities", MultipleAllowed::No } } );

  VideoFormat videoFormat{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "extend" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoFormat.extend = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoFormat.name = attribute.second;
    }
    else if ( attribute.first == "usage" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoFormat.usage = attribute.second;
      // CHECK: usage after extensions
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "videoformatproperties" )
    {
      videoFormat.videoFormatProperties = std::make_optional<VideoFormatProperties>( parseVideoFormatProperties( child ) );
    }
    else if ( value == "videorequirecapabilities" )
    {
      videoFormat.videoRequireCapabilities = std::make_optional<VideoRequireCapabilities>( parseVideoRequireCapabilities( child ) );
    }
  }

  return videoFormat;
}

VideoFormatProperties parseVideoFormatProperties( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "struct", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {}, {} );

  VideoFormatProperties videoFormatProperties{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "struct" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoFormatProperties.structure = attribute.second;
    }
  }
  return videoFormatProperties;
}

VideoProfile parseVideoProfile( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} }, { "value", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {}, {} );

  VideoProfile videoProfile{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoProfile.name = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoProfile.value = attribute.second;
    }
  }

  return videoProfile;
}

VideoProfileMember parseVideoProfileMember( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "videoprofile", MultipleAllowed::Yes } }, {} );

  VideoProfileMember videoProfileMember{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoProfileMember.name = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "videoprofile" )
    {
      VideoProfile videoProfile = parseVideoProfile( child );
      checkForError( "vk.xml",
                     !containsByName( videoProfileMember.videoProfiles, videoProfile.name ),
                     videoProfile.xmlLine,
                     "videoprofilemember <" + videoProfileMember.name + "> already lists a videoprofile <" + videoProfile.name + ">" );
      videoProfileMember.videoProfiles.push_back( std::move( videoProfile ) );
    }
  }

  return videoProfileMember;
}

VideoProfiles parseVideoProfiles( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "struct", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "videoprofilemember", MultipleAllowed::Yes } } );

  VideoProfiles videoProfiles{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "struct" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoProfiles.structure = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "videoprofilemember" )
    {
      VideoProfileMember videoProfileMember = parseVideoProfileMember( child );
      checkForError( "vk.xml",
                     !containsByName( videoProfiles.videoProfileMembers, videoProfileMember.name ),
                     videoProfileMember.xmlLine,
                     "videoprofilemember <" + videoProfileMember.name + "> already listed for videoprofiles <" + videoProfiles.structure + ">" );
      videoProfiles.videoProfileMembers.push_back( std::move( videoProfileMember ) );
    }
  }

  return videoProfiles;
}

VideoRequireCapabilities parseVideoRequireCapabilities( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "member", {} }, { "struct", {} }, { "value", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  VideoRequireCapabilities videoRequireCapabilities{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "member" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoRequireCapabilities.member = attribute.second;
    }
    else if ( attribute.first == "struct" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoRequireCapabilities.structure = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      checkNoList( "vk.xml", attribute.second, line );
      videoRequireCapabilities.value = attribute.second;
      // CHECK: value after extensions
    }
  }

  return videoRequireCapabilities;
}

// public interface

std::string concatenate( std::vector<std::string> const & list )
{
  std::string str;
  for ( auto const & entry : list )
  {
    if ( !str.empty() )
    {
      str += ", ";
    }
    str += entry;
  }
  return str;
}

std::pair<std::string, std::string> determineEnumSuffixes( std::string const & name, bool bitmask, std::vector<Tag> const & tags )
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
      size_t const pos = name.find( "FlagBits" );
      assert( pos != std::string::npos );
      std::string shortenedName = name;
      shortenedName.erase( pos, strlen( "FlagBits" ) );
      std::string tag = findTag( shortenedName, tags );
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
      if ( prefix.ends_with( tag.name + "_" ) )
      {
        prefix.erase( prefix.length() - tag.name.length() - 1 );
        postfix = "_" + tag.name;
        break;
      }
      else if ( name.ends_with( tag.name ) )
      {
        postfix = "_" + tag.name;
        break;
      }
    }
  }

  return { prefix, postfix };
}

std::string findTag( std::string const & name, std::vector<Tag> const & tags, std::string const & postfix )
{
  auto tagIt = std::ranges::find_if( tags, [&name, &postfix]( auto const & t ) { return name.ends_with( t.name + postfix ); } );
  return ( tagIt != tags.end() ) ? tagIt->name : "";
}

Vkxml parseVkXml( tinyxml2::XMLDocument const & document, std::string const & api )
{
  std::vector<tinyxml2::XMLElement const *> elements = getChildElements( &document );
  checkElements( "vk.xml", document.GetLineNum(), elements, { { "registry", MultipleAllowed::No } } );
  return parseRegistry( elements[0], api );
}
