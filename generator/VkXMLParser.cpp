// Copyright(c) 2026, NVIDIA CORPORATION. All rights reserved.
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

#include "VkXMLParser.hpp"

#include "XMLHelper.hpp"

#include <vector>

void checkNoList( std::string const & item, int line );
bool containsByNameAndExport( std::vector<Command> const & commands, std::string const & name, std::vector<std::string> const & exports );
bool isLenByStructMember( std::string const & name, std::vector<Param> const & params, std::map<std::string, Struct> const & structs );
std::pair<std::string, BaseType>
  parseBaseType( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes, std::string const & api );
std::pair<std::vector<std::string>, std::pair<std::string, Bitmask>> parseBitmask( tinyxml2::XMLElement const *               element,
                                                                                   std::map<std::string, std::string> const & attributes );
void                                                                 parseCommand( tinyxml2::XMLElement const * element, std::vector<Command> & commands );
std::vector<Command>                                                 parseCommands( tinyxml2::XMLElement const * element );
std::pair<std::string, Constant>                                     parseConstant( tinyxml2::XMLElement const * element );
CategoryAlias parseCategoryAlias( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes, std::string const & category );
std::pair<std::vector<std::string>, std::pair<std::string, Define>> parseDefine( tinyxml2::XMLElement const *               element,
                                                                                 std::map<std::string, std::string> const & attributes );
Deprecate                                                           parseDeprecate( tinyxml2::XMLElement const * element );
std::pair<std::string, Enum>         parseEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
void                                 parseEnum( tinyxml2::XMLElement const * element, std::pair<std::string const, EnumValues> & enumValues );
Enums                                parseEnums( tinyxml2::XMLElement const * element );
std::pair<std::string, ExternalType> parseExternalType( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
Feature                              parseFeature( tinyxml2::XMLElement const * element );
FeatureElement                       parseFeatureElement( tinyxml2::XMLElement const * element );
std::pair<std::string, FuncPointer>  parseFuncPointer( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
std::pair<std::string, Handle>       parseHandle( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
void                                 parseImplicitExternSyncParams( tinyxml2::XMLElement const * element );
void                                 parseImplicitExternSyncParamsParam( tinyxml2::XMLElement const * element );
std::pair<std::string, int>          parseInclude( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
std::string                          parseMemberEnum( tinyxml2::XMLElement const * element );
std::string                          parseName( tinyxml2::XMLElement const * element );
std::pair<std::string, Type>         parseNameAndType( tinyxml2::XMLElement const * element );
std::tuple<std::string, Type, std::vector<std::string>, std::string> parseNameAndTypeModified( tinyxml2::XMLElement const * element );
NameElement                                                          parseNameElement( tinyxml2::XMLElement const * element );
Param                                                                parseParam( tinyxml2::XMLElement const * element );
std::pair<std::string, Platform>                                     parsePlatform( tinyxml2::XMLElement const * element );
std::map<std::string, Platform>                                      parsePlatforms( tinyxml2::XMLElement const * element );
std::pair<std::string, Type>                                         parseProto( tinyxml2::XMLElement const * element );
Vkxml                                                                parseRegistry( tinyxml2::XMLElement const * element, std::string const & api );
Remove                                                               parseRemove( tinyxml2::XMLElement const * element );
Require                                                              parseRequire( tinyxml2::XMLElement const * element );
RequireEnum                                                          parseRequireEnum( tinyxml2::XMLElement const * element );
RequireType                                                          parseRequireType( tinyxml2::XMLElement const * element );
std::pair<std::string, Struct>
                            parseStruct( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes, std::string const & api );
StructMember                parseStructMember( tinyxml2::XMLElement const * element );
SupersededName              parseSupersededName( tinyxml2::XMLElement const * element );
std::pair<std::string, Tag> parseTag( tinyxml2::XMLElement const * element );
std::map<std::string, Tag>  parseTags( tinyxml2::XMLElement const * element );
Type                        parseType( tinyxml2::XMLElement const * element );
Types                       parseTypes( tinyxml2::XMLElement const * element, std::string const & api );
void                        parseTypesType( tinyxml2::XMLElement const * element, Types & types, std::string const & api );
std::pair<std::string, Union> parseUnion( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
UnionMember                   parseUnionMember( tinyxml2::XMLElement const * element );

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

void checkNoList( std::string const & item, int line )
{
  checkForError( "vk.xml", item.find( ',' ) == std::string::npos, line, "item <" + item + "> contains unexpected coma, looks like list" );
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

bool isLenByStructMember( std::string const & name, std::vector<Param> const & params, std::map<std::string, Struct> const & structs )
{
  // check if name specifies a member of a struct
  std::vector<std::string> nameParts = tokenize( name, "->" );
  if ( nameParts.size() == 2 )
  {
    auto paramIt = std::ranges::find_if( params, [&n = nameParts[0]]( Param const & pd ) { return pd.name == n; } );
    if ( paramIt != params.end() )
    {
      auto structIt = structs.find( paramIt->type.name );
      return ( structIt != structs.end() ) && containsByName( structIt->second.members, nameParts[1] );
    }
  }
  return false;
}

std::pair<std::string, BaseType> parseBaseType( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, attributes, { { "category", { "basetype" } } }, {} );

  // we're ignoring all the text nodes in between, as they are just some C-text with ifdefs and such
  auto [name, type] = parseNameAndType( element );
  return { name, { .type = type, .xmlLine = line } };
}

CategoryAlias parseCategoryAlias( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes, std::string const & category )
{
  int const line = element->GetLineNum();

  checkAttributes( "vk.xml", line, attributes, { { "alias", {} }, { "category", { category } }, { "name", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  CategoryAlias categoryAlias{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      checkNoList( attribute.second, line );
      categoryAlias.alias = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( attribute.second, line );
      categoryAlias.name = attribute.second;
    }
  }

  return categoryAlias;
}

std::pair<std::vector<std::string>, std::pair<std::string, Bitmask>> parseBitmask( tinyxml2::XMLElement const *               element,
                                                                                   std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();

  checkAttributes( "vk.xml",
                   line,
                   attributes,
                   { { "category", { "bitmask" } } },
                   { { "api", { "vulkan", "vulkanbase", "vulkansc" } }, { "bitvalues", {} }, { "requires", {} } } );

  std::vector<std::string> api = { "vulkan" };  // default is vulkan, if api attribute is not specified
  Bitmask                  bitmask;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "api" )
    {
      api = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "bitvalues" )
    {
      checkNoList( attribute.second, line );
      bitmask.bitValues = attribute.second;
    }
    else if ( attribute.first == "requires" )
    {
      checkNoList( attribute.second, line );
      bitmask.require = attribute.second;
    }
  }

  checkForError( "vk.xml", bitmask.bitValues.empty() || bitmask.require.empty(), line, "attributes <bitvalues> and <requires> are both specified" );

  std::string name;
  std::tie( name, bitmask.type ) = parseNameAndType( element );

  checkForWarning(
    "vk.xml", ( bitmask.type.name == "VkFlags" ) || ( bitmask.type.name == "VkFlags64" ), line, "unexpected bitmask type <" + bitmask.type.name + ">" );
  checkForError( "vk.xml", bitmask.type.postfix != "*", line, "unexpected pointer type <" + bitmask.type.name + ">" );
  checkForError(
    "vk.xml", ( bitmask.type.name != "VkFlags64" ) || !bitmask.bitValues.empty(), line, "bitmask of type <VkFlags64> needs attribute bitvalues to be set" );

  return { api, { name, bitmask } };
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
        checkNoList( attribute.second, line );
        alias = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        checkNoList( attribute.second, line );
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
        checkNoList( attribute.second, line );
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
        checkNoList( attribute.second, line );
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
        checkNoList( attribute.second, line );
        command.renderPass = attribute.second;
      }
      else if ( attribute.first == "successcodes" )
      {
        command.successCodes = tokenize( attribute.second, "," );
      }
      else if ( attribute.first == "videocoding" )
      {
        checkNoList( attribute.second, line );
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

std::pair<std::string, Constant> parseConstant( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} }, { "type", {} }, { "value", {} } }, { { "comment", {} } } );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  Constant    constant{ .xmlLine = line };
  std::string name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( attribute.second, line );
      name = attribute.second;
    }
    else if ( attribute.first == "type" )
    {
      checkNoList( attribute.second, line );
      constant.type = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      checkNoList( attribute.second, line );
      constant.value = attribute.second;
    }
  }
  return { name, constant };
}

std::pair<std::vector<std::string>, std::pair<std::string, Define>> parseDefine( tinyxml2::XMLElement const *               element,
                                                                                 std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml",
                   line,
                   attributes,
                   { { "category", { "define" } } },
                   { { "api", { "vulkan", "vulkanbase", "vulkansc" } }, { "comment", {} }, { "name", {} }, { "requires", {} } } );

  std::vector<std::string> apis = { "vulkan" };
  std::string              name, require;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "api" )
    {
      apis = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( attribute.second, line );
      name = attribute.second;
    }
    else if ( attribute.first == "requires" )
    {
      checkNoList( attribute.second, line );
      require = attribute.second;
    }
  }

  if ( !name.empty() )
  {
    checkForError( "vk.xml", !element->FirstChildElement(), line, "unknown formatting of type category=define name <" + name + ">" );
    checkForError( "vk.xml",
                   element->LastChild() && element->LastChild()->ToText() && element->LastChild()->ToText()->Value(),
                   line,
                   "unknown formatting of type category=define named <" + name + ">" );
  }
  else if ( element->GetText() )
  {
    std::string                  text  = element->GetText();
    tinyxml2::XMLElement const * child = element->FirstChildElement();
    checkForError( "vk.xml",
                   child && !child->FirstAttribute() && ( strcmp( child->Value(), "name" ) == 0 ) && child->GetText(),
                   line,
                   "unknown formatting of type category define" );
    name = trim( child->GetText() );
    //  ignore all the other defines
    checkForWarning( "vk.xml",
                     !child->NextSiblingElement() ||
                       ( child->NextSiblingElement() && !child->NextSiblingElement()->FirstAttribute() &&
                         ( strcmp( child->NextSiblingElement()->Value(), "type" ) == 0 ) && !child->NextSiblingElement()->NextSiblingElement() ),
                     line,
                     "unknown formatting of type category define" );
  }
  assert( !name.empty() );

  MacroVisitor definesVisitor{};
  element->Accept( &definesVisitor );

  return { apis, { name, { .macro = definesVisitor.macro, .require = require, .xmlLine = line } } };
}

Deprecate parseDeprecate( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "explanationlink", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, {}, { { "command", MultipleAllowed::Yes }, { "type", MultipleAllowed::Yes } } );

  Deprecate deprecate{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "explanationlink" )
    {
      checkNoList( attribute.second, line );
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

std::pair<std::string, Enum> parseEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, attributes, { { "category", { "enum" } }, { "name", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  return { attributes.find( "name" )->second, { .xmlLine = line } };
}

void parseEnum( tinyxml2::XMLElement const * element, std::pair<std::string const, EnumValues> & enumValues )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  if ( attributes.contains( "alias" ) )
  {
    checkAttributes( "vk.xml", line, attributes, { { "alias", {} }, { "name", {} } }, { { "api", { "vulkan" } }, { "deprecated", { "aliased" } } } );
    std::string alias, api, deprecated, name;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "alias" )
      {
        checkNoList( attribute.second, line );
        alias = attribute.second;
      }
      else if ( attribute.first == "api" )
      {
        checkNoList( attribute.second, line );
        api = attribute.second;
      }
      else if ( attribute.first == "deprecated" )
      {
        checkNoList( attribute.second, line );
        deprecated = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        checkNoList( attribute.second, line );
        name = attribute.second;
      }
    }

    auto enumValueIt = findByName( enumValues.second.values, alias );
    checkForError(
      "vk.xml", enumValueIt != enumValues.second.values.end(), line, "enum value alias <" + alias + "> not found for enum <" + enumValues.first + ">" );
    checkForError( "vk.xml",
                   enumValueIt->aliases.insert( { name, { .api = api, .deprecated = deprecated, .xmlLine = line } } ).second,
                   line,
                   "enum value alias <" + name + "> already specified for enum <" + enumValues.first + ">" );
  }
  else
  {
    if ( enumValues.second.type == "bitmask" )
    {
      checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, { { "bitpos", {} }, { "comment", {} }, { "value", {} } } );
    }
    else
    {
      checkAttributes( "vk.xml", line, attributes, { { "name", {} }, { "value", {} } }, { { "comment", {} } } );
    }

    std::string bitPos, name, value;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "bitpos" )
      {
        checkNoList( attribute.second, line );
        bitPos = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        checkNoList( attribute.second, line );
        name = attribute.second;
      }
      else if ( attribute.first == "value" )
      {
        checkNoList( attribute.second, line );
        value = attribute.second;
      }
    }
    assert( !name.empty() );

    checkForError(
      "vk.xml", !containsByName( enumValues.second.values, name ), line, "enum value <" + name + "> already specified for enum <" + enumValues.first + ">" );
    checkForError( "vk.xml",
                   ( enumValues.second.type != "bitmask" ) || ( bitPos.empty() ^ value.empty() ),
                   line,
                   "both or none of \"bitPos\" and \"value\" are set for bitmask enum <" + name + "> which is invalid" );
    enumValues.second.values.push_back( { .bitPos = bitPos, .name = name, .value = value, .xmlLine = line } );
  }
}

Enums parseEnums( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes(
    "vk.xml", line, attributes, { { "name", {} } }, { { "bitwidth", { "64" } }, { "comment", {} }, { "type", { "bitmask", "constants", "enum" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );

  std::string bitwidth, name, type;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "bitwidth" )
    {
      checkNoList( attribute.second, line );
      bitwidth = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( attribute.second, line );
      name = attribute.second;
    }
    else if ( attribute.first == "type" )
    {
      checkNoList( attribute.second, line );
      type = attribute.second;
    }
  }
  assert( !name.empty() );

  Enums enums{ .xmlLine = line };
  if ( type == "constants" )
  {
    assert( bitwidth.empty() && ( name == "API Constants" ) );
    checkElements( "vk.xml", line, children, { { "enum", MultipleAllowed::Yes } } );
    for ( auto const & child : children )
    {
      auto constant = parseConstant( child );
      checkForError( "vk.xml", enums.constants.insert( constant ).second, constant.second.xmlLine, "enum constant <" + constant.first + "> already specified" );
    }
  }
  else
  {
    checkForError( "vk.xml", ( type == "bitmask" ) || ( type == "enum" ), line, "unexpected enum type <" + type + ">" );
    checkElements(
      "vk.xml", line, children, {}, { { "comment", MultipleAllowed::Yes }, { "enum", MultipleAllowed::Yes }, { "unused", MultipleAllowed::Yes } } );

    auto [enumIt, inserted] = enums.enumValues.insert( { name, { .bitwidth = bitwidth, .type = type, .xmlLine = line } } );
    checkForError( "vk.xml", inserted, line, "enum <" + name + "> already specified on line " + std::to_string( enumIt->second.xmlLine ) );
    for ( auto const & child : children )
    {
      std::string value = child->Value();
      if ( value == "enum" )
      {
        parseEnum( child, *enumIt );
      }
    }
  }
  return enums;
}

std::pair<std::string, ExternalType> parseExternalType( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, attributes, { { "name", {} }, { "requires", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  std::string name, require;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( attribute.second, line );
      name = attribute.second;
    }
    else if ( attribute.first == "requires" )
    {
      checkNoList( attribute.second, line );
      require = attribute.second;
    }
  }

  return { name, { .includedBy = require, .xmlLine = line } };
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
      checkNoList( attribute.second, line );
      feature.apiType = attribute.second;
    }
    else if ( attribute.first == "comment" )
    {
      feature.comment = attribute.second;
    }
    else if ( attribute.first == "depends" )
    {
      feature.depends = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( attribute.second, line );
      feature.name = attribute.second;
    }
    else if ( attribute.first == "number " )
    {
      checkNoList( attribute.second, line );
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
      checkNoList( attribute.second, line );
      feature.name = attribute.second;
    }
    else if ( attribute.first == "struct" )
    {
      checkNoList( attribute.second, line );
      feature.structure = attribute.second;
    }
  }

  return feature;
}

std::pair<std::string, FuncPointer> parseFuncPointer( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, attributes, { { "category", { "funcpointer" } } }, { { "requires", {} } } );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "proto", MultipleAllowed::No } }, { { "param", MultipleAllowed::Yes } } );

  std::string require;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "requires" )
    {
      checkNoList( attribute.second, line );
      require = attribute.second;
    }
  }

  std::string                   name;
  Type                          type;
  std::vector<FuncPointerParam> params;

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "param" )
    {
      int childLine               = child->GetLineNum();
      auto [paramName, paramType] = parseNameAndType( child );
      assert( !name.empty() );
      checkForError( "vk.xml", !containsByName( params, paramName ), childLine, "param <" + paramName + "> already listed for funcPointer <" + name + ">" );
      params.push_back( { .name = paramName, .type = paramType, .xmlLine = childLine } );
    }
    else if ( value == "proto" )
    {
      std::tie( name, type ) = parseNameAndType( child );
      checkForError( "vk.xml", name.starts_with( "PFN_vk" ), line, "name <" + name + "> does not begin with <PFN_vk>" );
    }
  }

  return { name, { .params = params, .require = require, .returnType = type, .xmlLine = line } };
}

std::pair<std::string, Handle> parseHandle( tinyxml2::XMLElement const * handleElement, std::map<std::string, std::string> const & attributes )
{
  int const line = handleElement->GetLineNum();

  checkAttributes( "vk.xml", line, attributes, { { "category", { "handle" } }, { "objtypeenum", {} } }, { { "parent", {} } } );

  std::string objTypeEnum, parent;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "objtypeenum" )
    {
      checkNoList( attribute.second, line );
      objTypeEnum = attribute.second;
    }
    else if ( attribute.first == "parent" )
    {
      checkNoList( attribute.second, line );
      parent = attribute.second;
    }
  }

  auto [name, type] = parseNameAndType( handleElement );

  checkForError( "vk.xml", name.starts_with( "Vk" ), line, "name <" + name + "> does not begin with <Vk>" );
  checkForError( "vk.xml",
                 ( type.name == "VK_DEFINE_HANDLE" ) || ( type.name == "VK_DEFINE_NON_DISPATCHABLE_HANDLE" ),
                 line,
                 "handle with invalid type <" + type.name + ">" );
  // CHECK: objtypeenum after extensions

  return { name, { .objTypeEnum = objTypeEnum, .parent = parent, .type = type, .xmlLine = line } };
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

std::pair<std::string, int> parseInclude( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, attributes, { { "category", { "include" } }, { "name", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  return { attributes.find( "name" )->second, line };
}

std::string parseMemberEnum( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  std::string enumString = element->GetText();

  checkForError( "vk.xml",
                 element->PreviousSibling() && ( strcmp( element->PreviousSibling()->Value(), "[" ) == 0 ) && element->NextSibling() &&
                   ( strcmp( element->NextSibling()->Value(), "]" ) == 0 ),
                 line,
                 std::string( "struct member array specifiation is ill-formatted: <" ) + enumString + ">" );

  return enumString;
}

std::string parseName( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  std::string name = element->GetText();

  return name;
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

  std::string              name;
  Type                     type;
  std::vector<std::string> arraySizes;
  std::string              bitCount;
  for ( auto child : children )
  {
    std::string const value = child->Value();
    if ( value == "enum" )
    {
      arraySizes.push_back( child->GetText() );
      checkForError( "vk.xml",
                     child->PreviousSibling() && ( strcmp( child->PreviousSibling()->Value(), "[" ) == 0 ) && child->NextSibling() &&
                       ( strcmp( child->NextSibling()->Value(), "]" ) == 0 ),
                     line,
                     std::string( "array specifiation is ill-formatted: <" ) + arraySizes.back() + ">" );
    }
    else if ( value == "name" )
    {
      name                             = parseName( child );
      std::tie( arraySizes, bitCount ) = readModifiers( "vk.xml", child->NextSibling() );
    }
    else if ( value == "type" )
    {
      type = parseType( child );
    }
  }
  return { name, type, arraySizes, bitCount };
}

NameElement parseNameElement( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  NameElement nameElement{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( attribute.second, line );
      nameElement.name = attribute.second;
    }
  }

  return nameElement;
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
      checkNoList( attribute.second, line );
      param.altLen = attribute.second;
    }
    else if ( attribute.first == "api" )
    {
      param.api = tokenize( attribute.second, "," );
    }
    if ( attribute.first == "externsync" )
    {
      checkNoList( attribute.second, line );
      param.externSync = attribute.second;
    }
    else if ( attribute.first == "len" )
    {
      checkNoList( attribute.second, line );
      param.len = attribute.second;
    }
    else if ( attribute.first == "noautovalidity" )
    {
      checkNoList( attribute.second, line );
      param.noAutoValidity = attribute.second;
    }
    else if ( attribute.first == "objecttype" )
    {
      checkNoList( attribute.second, line );
      param.objectType = attribute.second;
    }
    else if ( attribute.first == "optional" )
    {
      param.optional = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "stride" )
    {
      checkNoList( attribute.second, line );
      param.stride = attribute.second;
    }
    else if ( attribute.first == "validstructs" )
    {
      checkNoList( attribute.second, line );
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

std::pair<std::string, Platform> parsePlatform( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "comment", {} }, { "name", {} }, { "protect", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  Platform platform;
  platform.xmlLine = line;

  std::string name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      platform.comment = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( attribute.second, line );
      name = attribute.second;
    }
    else if ( attribute.first == "protect" )
    {
      checkNoList( attribute.second, line );
      platform.protect = attribute.second;
    }
  }

  return { name, std::move( platform ) };
}

std::map<std::string, Platform> parsePlatforms( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), { { "comment", {} } }, {} );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "platform", MultipleAllowed::Yes } } );

  std::map<std::string, Platform> platforms;
  for ( auto child : children )
  {
    std::pair<std::string, Platform> platform = parsePlatform( child );

    checkForError(
      "vk.xml",
      std::ranges::find_if( platforms, [protect = platform.second.protect]( auto const & platform ) { return protect == platform.second.protect; } ) ==
        platforms.end(),
      line,
      "platform <" + platform.first + "> uses protect <" + platform.second.protect + "> that is already used by some other platform" );
    checkForError( "vk.xml", platforms.insert( std::move( platform ) ).second, line, "platform <" + platform.first + "> already specified" );
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
            auto        structIt   = vkxml.structs.find( param.type.name );
            checkForError( "vk.xml",
                           structIt != vkxml.structs.end(),
                           param.xmlLine,
                           "type <" + param.type.name + "> of parameter <" + param.name + "> of command <" + command.name +
                             "> with externsync attribute is not a struct" );
            checkForError( "vk.xml",
                           containsByName( structIt->second.members, memberName ),
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
                         param.validStructs.empty() || vkxml.structs.contains( param.validStructs ),
                         param.xmlLine,
                         "unknown validstructs <" + param.validStructs + "> specified for parameter <" + param.name + "> of command <" + command.name + ">" );
        }
      }
    }
    else if ( value == "comment" )
    {
      std::string comment = readComment( "VkXMLParser", child );

      if ( comment.starts_with( "\nCopyright" ) )
      {
        checkForError( "VkXMLParser",
                       vkxml.copyright.text.empty(),
                       line,
                       "Copyright message has already been encountered on line " + std::to_string( vkxml.copyright.xmlLine ) );
        vkxml.copyright = { .text = comment, .xmlLine = line };
      }
    }
    else if ( value == "enums" )
    {
      Enums enums = parseEnums( child );
      if ( !enums.constants.empty() )
      {
        assert( vkxml.constants.empty() );
        for ( auto const & constant : enums.constants )
        {
          checkForError( "vk.xml",
                         vkxml.externalTypes.contains( constant.second.type ),
                         constant.second.xmlLine,
                         "enum constant <" + constant.first + "> has unknown type <" + constant.second.type + ">" );
        }
        vkxml.constants = std::move( enums.constants );
      }
      for ( auto & enumValue : enums.enumValues )
      {
        auto enumIt = vkxml.enums.find( enumValue.first );
        checkForError( "vk.xml", enumIt != vkxml.enums.end(), line, "enum <" + enumValue.first + "> not specified" );
        enumIt->second.bitwidth = enumValue.second.bitwidth;
        enumIt->second.type     = enumValue.second.type;
        enumIt->second.values   = std::move( enumValue.second.values );
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
          auto enumIt = std::ranges::find_if( vkxml.enums, [&e]( auto const & enum_ ) { return containsByName( enum_.second.values, e.name ); } );
          checkForError( "vk.xml", enumIt != vkxml.enums.end(), e.xmlLine, "feature <" + feature.name + "> removes unknown enum <" + e.name + ">" );
        }
      }
      for ( auto const & require : feature.require )
      {
        for ( auto const & depend : require.depends )
        {
          std::vector<std::string> tokens = tokenize( depend, "::" );
          if ( tokens.size() == 2 )
          {
            auto structIt = vkxml.structs.find( tokens[0] );
            checkForError( "vk.xml",
                           structIt != vkxml.structs.end(),
                           require.xmlLine,
                           "feature <" + feature.name + "> has a require-member depending on unknown struct <" + tokens[0] + ">" );
            checkForError( "vk.xml",
                           containsByName( structIt->second.members, tokens[1] ),
                           require.xmlLine,
                           "feature <" + feature.name + "> has a require-member depending on unknown member <" + tokens[1] + "> of structure <" + tokens[0] +
                             ">" );
          }
          // CHECK: after extensions: tokens.size() == 1 -> is extension
        }
        for ( auto const & e : require.enums )
        {
          if ( !e.extends.empty() )
          {
            auto enumIt = vkxml.enums.find( e.extends );
            checkForError(
              "vk.xml", enumIt != vkxml.enums.end(), e.xmlLine, "feature <" + feature.name + "> requires extending an unknown enum <" + e.extends + ">" );
            if ( e.alias.empty() )
            {
              checkForError( "vk.xml",
                             !containsByName( enumIt->second.values, e.name ),
                             e.xmlLine,
                             "feature <" + feature.name + "> requires to extend enum <" + e.extends + "> with an already specified value <" + e.name + ">" );
              enumIt->second.values.push_back( { .bitPos = e.bitPos, .name = e.name, .value = e.value, .xmlLine = e.xmlLine } );
            }
            else
            {
              auto valueIt = findByName( enumIt->second.values, e.alias );
              checkForError( "vk.xml",
                             valueIt != enumIt->second.values.end(),
                             e.xmlLine,
                             "feature <" + feature.name + "> requires to extend enum <" + e.extends + "> with an alias <" + e.name +
                               "> for an unknown enum value <" + e.alias + ">" );
              checkForError( "vk.xml",
                             valueIt->aliases.insert( { e.name, { .api = e.api, .deprecated = e.deprectated, .xmlLine = e.xmlLine } } ).second,
                             e.xmlLine,
                             "feature <" + feature.name + "> requires to extend enum <" + e.extends + "> with an already know alias <" + e.name +
                               "> for enum value <" + e.alias + ">" );
            }
          }
        }
        for ( auto const & requireFeature : require.features )
        {
          auto structIt = vkxml.structs.find( requireFeature.structure );
          checkForError( "vk.xml",
                         structIt != vkxml.structs.end(),
                         requireFeature.xmlLine,
                         "feature <" + feature.name + "> requires feature <" + requireFeature.name + "> of unknown struct <" + requireFeature.structure + ">" );
          checkForError( "vk.xml",
                         containsByName( structIt->second.members, requireFeature.name ),
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
    else if ( value == "platforms" )
    {
      vkxml.platforms = parsePlatforms( child );
    }
    else if ( value == "tags" )
    {
      vkxml.tags = parseTags( child );
    }
    else if ( value == "types" )
    {
      Types types         = parseTypes( child, api );
      vkxml.baseTypes     = std::move( types.baseTypes );
      vkxml.bitmasks      = std::move( types.bitmasks );
      vkxml.defines       = std::move( types.defines );
      vkxml.enums         = std::move( types.enums );
      vkxml.externalTypes = std::move( types.externalTypes );
      vkxml.funcPointers  = std::move( types.funcPointers );
      vkxml.handles       = std::move( types.handles );
      vkxml.includes      = std::move( types.includes );
      vkxml.structs       = std::move( types.structs );
      vkxml.unions        = std::move( types.unions );
      vkxml.types         = std::move( types.types );
    }
    // else
    //{
    //   checkForError( "vk.xml", false, line, "unknown element <" + value + ">" );
    // }
  }
  checkForError( "VkXMLParser", !vkxml.copyright.text.empty(), line, "Copyright message is missing" );

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
    else if ( value == "comment" )
    {
      readComment( "vk.xml", child );
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
    else if ( value == "comment" )
    {
      readComment( "vk.xml", child );
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
      checkNoList( attribute.second, line );
      requireEnum.alias = attribute.second;
    }
    else if ( attribute.first == "api" )
    {
      checkNoList( attribute.second, line );
      requireEnum.api = attribute.second;
    }
    else if ( attribute.first == "bitpos" )
    {
      checkNoList( attribute.second, line );
      checkNumber( attribute.second, line, "require enum with non-numeric bitPos" );
      requireEnum.bitPos = attribute.second;
    }
    else if ( attribute.first == "comment" )
    {
      requireEnum.comment = attribute.second;
    }
    else if ( attribute.first == "deprecated" )
    {
      checkNoList( attribute.second, line );
      requireEnum.deprectated = attribute.second;
    }
    else if ( attribute.first == "dir" )
    {
      checkNoList( attribute.second, line );
      requireEnum.dir = attribute.second;
    }
    else if ( attribute.first == "extends" )
    {
      checkNoList( attribute.second, line );
      requireEnum.extends = attribute.second;
    }
    else if ( attribute.first == "extnumber" )
    {
      checkNoList( attribute.second, line );
      checkNumber( attribute.second, line, "require enum with non-numeric extnumber" );
      requireEnum.extNumber = attribute.second;
      // CHECK: extnumber after extensions
    }
    else if ( attribute.first == "offset" )
    {
      checkNoList( attribute.second, line );
      checkNumber( attribute.second, line, "require enum with non-numeric offset" );
      requireEnum.offset = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( attribute.second, line );
      requireEnum.name = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      checkNoList( attribute.second, line );
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
      checkNoList( attribute.second, line );
      type.name = attribute.second;
    }
  }

  return type;
}

std::pair<std::string, Struct>
  parseStruct( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes, std::string const & api )
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

  std::string name;
  Struct      structure{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "allowduplicate" )
    {
      checkNoList( attribute.second, line );
      structure.allowDuplicate = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( attribute.second, line );
      name = attribute.second;
    }
    else if ( attribute.first == "requiredlimittype" )
    {
      checkNoList( attribute.second, line );
      structure.requiredLimitType = attribute.second;
    }
    else if ( attribute.first == "returnedonly" )
    {
      checkNoList( attribute.second, line );
      structure.returnedOnly = attribute.second;
    }
    else if ( attribute.first == "structextends" )
    {
      structure.structExtends = tokenize( attribute.second, "," );
    }
  }
  assert( !name.empty() );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "member" )
    {
      StructMember member = parseStructMember( child );

      // `VkDeviceCreateInfo::ppEnabledLayerNames` needs to be special-cased with old attributes to maintain API compatibility
      // See https://github.com/KhronosGroup/Vulkan-Hpp/issues/2531
      if ( ( name == "VkDeviceCreateInfo" ) && ( member.name == "ppEnabledLayerNames" ) )
      {
        member.len = { "enabledLayerCount", "null-terminated" };
      }

      if ( member.api.empty() || std::ranges::any_of( member.api, [&api]( auto const & a ) { return a == api; } ) )
      {
        if ( !member.selector.empty() )
        {
          checkForError( "vk.xml",
                         containsByName( structure.members, member.selector ),
                         line,
                         "struct member <" + member.name + "> references unknown struct member <" + member.selector + "> in its selector attribute" );
        }

        checkForError(
          "vk.xml", !containsByName( structure.members, member.name ), line, "member <" + member.name + "> already listed for struct <" + name + ">" );

        structure.members.push_back( std::move( member ) );
      }
    }
  }

  auto memberIt = std::ranges::find_if( structure.members, []( auto const & member ) { return !member.altLen.empty(); } );
  if ( memberIt != structure.members.end() )
  {
    checkForError( "vk.xml",
                   ( memberIt->len.size() == 1 ) && memberIt->len[0].starts_with( "latexmath:" ),
                   memberIt->xmlLine,
                   "struct member <" + memberIt->name + "> has attribute <altLen> but attribute <len> does not start with \"latexmath:\"" );
  }
  memberIt =
    std::ranges::find_if( structure.members, []( auto const & member ) { return ( member.len.size() == 1 ) && member.len[0].starts_with( "latexmath:" ); } );
  if ( memberIt != structure.members.end() )
  {
    checkForError( "vk.xml",
                   !memberIt->altLen.empty(),
                   memberIt->xmlLine,
                   "struct member <" + memberIt->name + "> has attribute <len> starting with \"latexmath:\" but no attribute <altLen>" );
  }

  checkForError( "vk.xml",
                 ( structure.requiredLimitType != "true" ) ||
                   std::ranges::all_of( structure.members,
                                        []( StructMember const & member )
                                        { return ( member.name == "sType" ) || ( member.name == "pNext" ) || !member.limitType.empty(); } ),
                 line,
                 "struct <" + name + "> requires all members to have a limittype" );

  return { name, std::move( structure ) };
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
      checkNoList( attribute.second, line );
      member.altLen = attribute.second;
    }
    else if ( attribute.first == "api" )
    {
      member.api = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "deprecated" )
    {
      checkNoList( attribute.second, line );
      member.deprecated = attribute.second;
    }
    else if ( attribute.first == "externsync" )
    {
      checkNoList( attribute.second, line );
      member.externSync = attribute.second;
    }
    else if ( attribute.first == "featurelink" )
    {
      checkNoList( attribute.second, line );
      member.featureLink = attribute.second;
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
      checkNoList( attribute.second, line );
      member.noAutoValidity = attribute.second;
    }
    else if ( attribute.first == "objecttype" )
    {
      checkNoList( attribute.second, line );
      member.objectType = attribute.second;
    }
    else if ( attribute.first == "optional" )
    {
      member.optional = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "selector" )
    {
      checkNoList( attribute.second, line );
      member.selector = attribute.second;
    }
    else if ( attribute.first == "values" )
    {
      checkNoList( attribute.second, line );
      member.values = attribute.second;
    }
  }

  std::tie( member.name, member.type, member.arraySizes, member.bitCount ) = parseNameAndTypeModified( element );

  checkForError( "vk.xml", member.altLen.empty() || !member.len.empty(), line, "member <" + member.name + "> has attribute <altLen>, but no attribute <len>" );
  checkForError( "vk.xml",
                 member.len.empty() || !member.arraySizes.empty() || member.type.isPointer(),
                 line,
                 "member <" + member.name + "> has attribute <len> but is not a pointer" );
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
      checkNoList( attribute.second, line );
      deprecateCommand.name = attribute.second;
    }
    else if ( attribute.first == "supersededby" )
    {
      checkNoList( attribute.second, line );
      deprecateCommand.supersededBy = attribute.second;
    }
  }

  return deprecateCommand;
}

std::pair<std::string, Tag> parseTag( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "author", {} }, { "contact", {} }, { "name", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  Tag tag;
  tag.xmlLine = line;

  std::string name;
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
      checkNoList( attribute.second, line );
      name = attribute.second;
    }
  }
  return { name, std::move( tag ) };
}

std::map<std::string, Tag> parseTags( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), { { "comment", {} } }, {} );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "tag", MultipleAllowed::Yes } } );

  std::map<std::string, Tag> tags;
  for ( auto child : children )
  {
    std::pair<std::string, Tag> tag = parseTag( child );

    checkForError( "vk.xml", tags.insert( std::move( tag ) ).second, line, "tag <" + tag.first + "> already specified" );
  }

  return tags;
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
      parseTypesType( child, types, api );
    }
  }

  // structs might alias a struct that's specified later than the alias !
  for ( auto structAlias : types.structAliases )
  {
    auto structIt = types.structs.find( structAlias.alias );
    checkForError( "vk.xml",
                   structIt != types.structs.end(),
                   structAlias.xmlLine,
                   "struct <" + structAlias.name + "> is an alias of an unknown struct <" + structAlias.alias + ">." );
    checkForError( "vk.xml",
                   structIt->second.aliases.insert( { structAlias.name, structAlias.xmlLine } ).second,
                   structAlias.xmlLine,
                   "struct alias <" + structAlias.name + "> is already listed as an alias for struct <" + structAlias.alias + ">" );
  }
  types.structAliases.clear();

  for ( auto const & [defineName, define] : types.defines )
  {
    if ( !define.require.empty() )
    {
      checkForError(
        "vk.xml", types.defines.contains( define.require ), define.xmlLine, "define <" + defineName + "> requires unknown define <" + define.require + ">" );
    }
  }
  for ( auto const & [funcPointerName, funcPointer] : types.funcPointers )
  {
    if ( !funcPointer.require.empty() )
    {
      checkForError( "vk.xml",
                     types.handles.contains( funcPointer.require ) || types.structs.contains( funcPointer.require ),
                     funcPointer.xmlLine,
                     "funcpointer <" + funcPointerName + "> requires unknown type <" + funcPointer.require + ">" );
    }
  }
  for ( auto const & [handleName, handle] : types.handles )
  {
    checkForError( "vk.xml",
                   handle.parent.empty() || types.handles.contains( handle.parent ),
                   handle.xmlLine,
                   "handle <" + handleName + "> specifies unknown parent handle <" + handle.parent + ">" );
  }
  for ( auto const & [structName, structure] : types.structs )
  {
    for ( auto const & member : structure.members )
    {
      if ( !member.selector.empty() )
      {
        checkForError( "vk.xml",
                       !types.enums.contains( member.selector ),
                       member.xmlLine,
                       "struct member <" + member.name + "> in struct <" + structName + "> references unknown selector enum <" + member.selector + ">" );
        checkForError( "vk.xml",
                       types.unions.contains( member.type.name ),
                       member.xmlLine,
                       "struct member <" + member.name + "> in struct <" + structName + "> has selector <" + member.selector + "> but its type <" +
                         member.type.name + "> is not a union" );
      }
      checkForError( "vk.xml",
                     types.types.contains( member.type.name ),
                     member.xmlLine,
                     "struct member <" + member.name + "> in struct <" + structName + "> has unknown type <" + member.type.name + ">" );
    }

    for ( auto const & structExtend : structure.structExtends )
    {
      checkForError(
        "vk.xml", types.structs.contains( structExtend ), structure.xmlLine, "struct <" + structName + "> extends unknown struct <" + structExtend + ">" );
    }

    auto memberIt = std::ranges::find_if( structure.members, []( auto const & member ) { return !member.values.empty(); } );
    if ( memberIt != structure.members.end() )
    {
      checkForError( "vk.xml",
                     !types.enums.contains( memberIt->values ),
                     memberIt->xmlLine,
                     "struct member <" + memberIt->name + "> in struct <" + structName + "> references unknown enum <" + memberIt->values + ">" );
    }
  }

  return types;
}

void parseTypesType( tinyxml2::XMLElement const * element, Types & types, std::string const & api )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );

  auto categoryIt = attributes.find( "category" );
  if ( categoryIt != attributes.end() )
  {
    if ( categoryIt->second == "basetype" )
    {
      std::pair<std::string, BaseType> baseType = parseBaseType( element, attributes );

      checkForError(
        "vk.xml", types.types.insert( baseType.first ).second, baseType.second.xmlLine, "base type <" + baseType.first + "> already specified as a type" );
      checkForError( "vk.xml", types.baseTypes.insert( baseType ).second, baseType.second.xmlLine, "basetype <" + baseType.first + "> already specified" );
    }
    else if ( categoryIt->second == "bitmask" )
    {
      if ( attributes.contains( "alias" ) )
      {
        CategoryAlias alias = parseCategoryAlias( element, attributes, "bitmask" );

        checkForError( "vk.xml", types.types.insert( alias.name ).second, alias.xmlLine, "bitmask alias <" + alias.name + "> already specified as a type" );
        auto bitmaskIt = types.bitmasks.find( alias.alias );
        checkForError(
          "vk.xml", bitmaskIt != types.bitmasks.end(), alias.xmlLine, "bitmask <" + alias.name + "> is an alias of an unknown bitmask <" + alias.alias + ">." );
        checkForError( "vk.xml",
                       bitmaskIt->second.aliases.insert( { alias.name, alias.xmlLine } ).second,
                       alias.xmlLine,
                       "bitmask alias <" + alias.name + "> is already listed as an alias for bitmask <" + alias.alias + ">" );
      }
      else
      {
        auto [bitmaskApi, bitmask] = parseBitmask( element, attributes );

        if ( std::ranges::any_of( bitmaskApi, [&api]( auto const & a ) { return a == api; } ) )
        {
          checkForError(
            "vk.xml", types.types.insert( bitmask.first ).second, bitmask.second.xmlLine, "bitmask <" + bitmask.first + "> already specified as a type" );
          checkForError( "vk.xml", types.bitmasks.insert( bitmask ).second, bitmask.second.xmlLine, "bitmask <" + bitmask.first + "> already specified" );
        }
      }
    }
    else if ( categoryIt->second == "define" )
    {
      auto [defineApi, define] = parseDefine( element, attributes );
      if ( std::ranges::any_of( defineApi, [&api]( auto const & a ) { return a == api; } ) )
      {
        checkForError(
          "vk.xml", types.types.insert( define.first ).second, define.second.xmlLine, "define <" + define.first + "> already specified as a type" );
        checkForError( "vk.xml", types.defines.insert( define ).second, define.second.xmlLine, "define <" + define.first + "> already specified" );
      }
    }
    else if ( categoryIt->second == "enum" )
    {
      if ( attributes.contains( "alias" ) )
      {
        CategoryAlias alias = parseCategoryAlias( element, attributes, "enum" );

        checkForError( "vk.xml", types.types.insert( alias.name ).second, alias.xmlLine, "enum alias <" + alias.name + "> already specified as a type" );

        auto enumIt = types.enums.find( alias.alias );
        checkForError(
          "vk.xml", enumIt != types.enums.end(), alias.xmlLine, "enum <" + alias.name + "> is an alias of an unknown enum <" + alias.alias + ">." );
        checkForError( "vk.xml",
                       enumIt->second.aliases.insert( { alias.name, alias.xmlLine } ).second,
                       alias.xmlLine,
                       "enum alias <" + alias.name + "> is already listed as an alias for enum <" + alias.alias + ">" );
      }
      else
      {
        std::pair<std::string, Enum> enumData = parseEnum( element, attributes );

        checkForError(
          "vk.xml", types.types.insert( enumData.first ).second, enumData.second.xmlLine, "enum <" + enumData.first + "> already specified as a type" );
        checkForError( "vk.xml", types.enums.insert( enumData ).second, enumData.second.xmlLine, "enum <" + enumData.first + "> already specified" );
      }
    }
    else if ( categoryIt->second == "handle" )
    {
      if ( attributes.contains( "alias" ) )
      {
        CategoryAlias alias = parseCategoryAlias( element, attributes, "handle" );

        checkForError( "vk.xml", types.types.insert( alias.name ).second, alias.xmlLine, "handle alias <" + alias.name + "> already specified as a type" );
        auto handleIt = types.handles.find( alias.alias );
        checkForError(
          "vk.xml", handleIt != types.handles.end(), alias.xmlLine, "handle <" + alias.name + "> is an alias of an unknown handle <" + alias.alias + ">." );
        checkForError( "vk.xml",
                       handleIt->second.aliases.insert( { alias.name, alias.xmlLine } ).second,
                       alias.xmlLine,
                       "handle alias <" + alias.name + "> is already listed as an alias for handle <" + alias.alias + ">" );
      }
      else
      {
        std::pair<std::string, Handle> handle = parseHandle( element, attributes );

        checkForError(
          "vk.xml", types.types.insert( handle.first ).second, handle.second.xmlLine, "handle <" + handle.first + "> already specified as a type" );
        checkForError( "vk.xml", types.handles.insert( handle ).second, handle.second.xmlLine, "handle <" + handle.first + "> already specified" );
      }
    }
    else if ( categoryIt->second == "include" )
    {
      std::pair<std::string, int> include = parseInclude( element, attributes );

      checkForError( "vk.xml", types.types.insert( include.first ).second, include.second, "include <" + include.first + "> already specified as a type" );
      checkForError( "vk.xml", types.includes.insert( include ).second, include.second, "include <" + include.first + "> already specified" );
    }
    else if ( categoryIt->second == "funcpointer" )
    {
      std::pair<std::string, FuncPointer> funcPointer = parseFuncPointer( element, attributes );

      checkForError( "vk.xml",
                     types.types.insert( funcPointer.first ).second,
                     funcPointer.second.xmlLine,
                     "funcpointer <" + funcPointer.first + "> already specified as a type" );
      checkForError(
        "vk.xml", types.funcPointers.insert( funcPointer ).second, funcPointer.second.xmlLine, "funcpointer <" + funcPointer.first + "> already specified" );
    }
    else if ( categoryIt->second == "struct" )
    {
      if ( attributes.contains( "alias" ) )
      {
        CategoryAlias alias = parseCategoryAlias( element, attributes, "struct" );

        checkForError( "vk.xml", types.types.insert( alias.name ).second, alias.xmlLine, "struct alias <" + alias.name + "> already specified as a type" );
        checkForError( "vk.xml", !containsByName( types.structAliases, alias.name ), alias.xmlLine, "struct alias <" + alias.name + "> already encountered" );
        types.structAliases.push_back( std::move( alias ) );
      }
      else
      {
        std::pair<std::string, Struct> structure = parseStruct( element, attributes, api );

        checkForError(
          "vk.xml", types.types.insert( structure.first ).second, structure.second.xmlLine, "struct <" + structure.first + "> already specified as a type" );
        checkForError( "vk.xml", types.structs.insert( structure ).second, structure.second.xmlLine, "struct <" + structure.first + "> already specified" );
      }
    }
    else if ( categoryIt->second == "union" )
    {
      std::pair<std::string, Union> u = parseUnion( element, attributes );

      checkForError( "vk.xml", types.types.insert( u.first ).second, u.second.xmlLine, "union <" + u.first + "> already specified as a type" );
      checkForError( "vk.xml", types.unions.insert( u ).second, u.second.xmlLine, "union <" + u.first + "> already specified" );
    }
    else
    {
      checkForError( "vk.xml", false, 0, "unknown category <" + categoryIt->second + "> encountered" );
    }
  }
  else
  {
    auto requiresIt = attributes.find( "requires" );
    checkForError( "vk.xml", requiresIt != attributes.end(), line, "encountered type without category and requires" );

    std::pair<std::string, ExternalType> externalType = parseExternalType( element, attributes );

    checkForError( "vk.xml",
                   types.types.insert( externalType.first ).second,
                   externalType.second.xmlLine,
                   "external type <" + externalType.first + "> already specified as a type" );
    checkForError( "vk.xml",
                   types.includes.contains( externalType.second.includedBy ),
                   externalType.second.xmlLine,
                   "type <" + externalType.first + "> requires unknown <" + externalType.second.includedBy + ">" );
    checkForError( "vk.xml",
                   types.externalTypes.insert( externalType ).second,
                   externalType.second.xmlLine,
                   "external type <" + externalType.first + "> already specified" );
  }
}

std::pair<std::string, Union> parseUnion( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, attributes, { { "category", { "union" } }, { "name", {} } }, { { "comment", {} }, { "returnedonly", { "true" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "member", MultipleAllowed::Yes } }, { { "comment", MultipleAllowed::Yes } } );

  std::string name;
  Union       u{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( attribute.second, line );
      name = attribute.second;
    }
    else if ( attribute.first == "returnedonly" )
    {
      checkNoList( attribute.second, line );
      u.returnedOnly = attribute.second;
    }
  }
  assert( !name.empty() );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "member" )
    {
      UnionMember member = parseUnionMember( child );
      checkForError( "vk.xml", !containsByName( u.members, member.name ), line, "member <" + member.name + "> already listed for union <" + name + ">" );
      u.members.push_back( std::move( member ) );
    }
  }

  return { name, std::move( u ) };
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
      checkNoList( attribute.second, line );
      member.len = attribute.second;
    }
    else if ( attribute.first == "noautovalidity" )
    {
      checkNoList( attribute.second, line );
      member.noAutoValidity = attribute.second;
    }
    else if ( attribute.first == "optional" )
    {
      checkNoList( attribute.second, line );
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

  // CHECK: selection after extensions
#if 0
  for ( auto const & [unionName, u] : vkxml.unions )
  {
    if ( !u.members.front().selection.empty() )
    {
      auto selectionEnumIt = std::ranges::find_if(
        vkxml.enums, [selection = u.members.front().selection.front()]( auto const & e ) { return containsByName( e.second.values, selection ); } );
      checkForError( "vk.xml",
                     selectionEnumIt != types.enums.end(),
                     u.xmlLine,
                     "union <" + unionName + "> uses a selection attribute <" + u.members.front().selection.front() + "> of unknown type" );
      for ( auto const & member : u.members )
      {
        for ( auto const & selection : member.selection )
        {
          checkForError( "vk.xml",
                         containsByName( selectionEnumIt->second.values, selection ),
                         member.xmlLine,
                         "union <" + unionName + "> uses unknow selection <" + selection + ">" );
        }
      }
    }
  }
#endif

  return member;
}

Vkxml parseVkXml( tinyxml2::XMLDocument const & document, std::string const & api )
{
  std::vector<tinyxml2::XMLElement const *> elements = getChildElements( &document );
  checkElements( "vk.xml", document.GetLineNum(), elements, { { "registry", MultipleAllowed::No } } );
  return parseRegistry( elements[0], api );
}
