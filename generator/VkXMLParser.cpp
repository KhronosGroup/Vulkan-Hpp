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

std::pair<std::string, BaseType>
  parseBaseType( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes, std::string const & api );
std::pair<std::vector<std::string>, std::pair<std::string, Bitmask>> parseBitmask( tinyxml2::XMLElement const *               element,
                                                                                   std::map<std::string, std::string> const & attributes );
std::pair<std::string, Constant>                                     parseConstant( tinyxml2::XMLElement const * element );
CategoryAlias parseCategoryAlias( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes, std::string const & category );
std::pair<std::vector<std::string>, std::pair<std::string, Define>> parseDefine( tinyxml2::XMLElement const *               element,
                                                                                 std::map<std::string, std::string> const & attributes );
std::pair<std::string, Enum>         parseEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
void                                 parseEnum( tinyxml2::XMLElement const * element, std::pair<std::string const, EnumValues> & enumValues );
Enums                                parseEnums( tinyxml2::XMLElement const * element );
std::pair<std::string, ExternalType> parseExternalType( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
std::pair<std::string, FuncPointer>  parseFuncPointer( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
std::pair<std::string, Handle>       parseHandle( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
std::pair<std::string, int>          parseInclude( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
std::string                          parseMemberEnum( tinyxml2::XMLElement const * element );
std::string                          parseName( tinyxml2::XMLElement const * element );
std::pair<std::string, Type>         parseNameAndType( tinyxml2::XMLElement const * element );
std::pair<std::string, Platform>     parsePlatform( tinyxml2::XMLElement const * element );
std::map<std::string, Platform>      parsePlatforms( tinyxml2::XMLElement const * element );
Vkxml                                parseRegistry( tinyxml2::XMLElement const * element, std::string const & api );
std::pair<std::string, Struct>
                            parseStruct( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes, std::string const & api );
StructMember                parseStructMember( tinyxml2::XMLElement const * element );
std::pair<std::string, Tag> parseTag( tinyxml2::XMLElement const * element );
std::map<std::string, Tag>  parseTags( tinyxml2::XMLElement const * element );
Type                        parseType( tinyxml2::XMLElement const * element );
Types                       parseTypes( tinyxml2::XMLElement const * element, std::string const & api );
void                        parseTypesType( tinyxml2::XMLElement const * element, Types & types, std::string const & api );
std::pair<std::string, Union> parseUnion( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
UnionMember                   parseUnionMember( tinyxml2::XMLElement const * element );

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

  return { .alias = alias, .name = name, .xmlLine = line };
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
  std::string              bitvalues, require;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "api" )
    {
      api = tokenize( attribute.second, "," );
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

  checkForError( "vk.xml", bitvalues.empty() || require.empty(), line, "attributes <bitvalues> and <requires> are both specified" );

  auto [name, type] = parseNameAndType( element );

  checkForError( "vk.xml", name.starts_with( "Vk" ), line, "name <" + name + "> does not begin with <Vk>" );
  checkForWarning( "vk.xml", ( type.name == "VkFlags" ) || ( type.name == "VkFlags64" ), line, "unexpected bitmask type <" + type.name + ">" );
  checkForError( "vk.xml", type.postfix != "*", line, "unexpected pointer type <" + type.name + ">" );
  checkForError( "vk.xml", ( type.name != "VkFlags64" ) || !bitvalues.empty(), line, "bitmask of type <VkFlags64> needs attribute bitvalues to be set" );

  if ( !bitvalues.empty() )
  {
    require = bitvalues;
  }

  return { api, { name, { .require = require, .type = type, .xmlLine = line } } };
}

std::pair<std::string, Constant> parseConstant( tinyxml2::XMLElement const * element )
{
  int const                          line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "vk.xml", line, attributes, { { "name", {} }, { "type", {} }, { "value", {} } }, { { "comment", {} } } );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  std::string name, type, value;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
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
  assert( !name.empty() && !type.empty() && !value.empty() );
  return { name, { .type = type, .value = value, .xmlLine = line } };
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
      name = attribute.second;
    }
    else if ( attribute.first == "requires" )
    {
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
                     !child->NextSiblingElement() || ( child->NextSiblingElement() &&
                                                       !child->NextSiblingElement()->FirstAttribute() &&
                                                       ( strcmp( child->NextSiblingElement()->Value(), "type" ) == 0 ) &&
                                                       !child->NextSiblingElement()->NextSiblingElement() ),
                     line,
                     "unknown formatting of type category define" );
  }
  assert( !name.empty() );

  MacroVisitor definesVisitor{};
  element->Accept( &definesVisitor );

  return { apis, { name, { .macro = definesVisitor.macro, .require = require, .xmlLine = line } } };
}

std::pair<std::string, Enum> parseEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, attributes, { { "category", { "enum" } }, { "name", {} } }, {} );
  checkElements( "vk.xml", line, getChildElements( element ), {} );

  std::string name = attributes.find( "name" )->second;

  return { name, { .xmlLine = line } };
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
        alias = attribute.second;
      }
      else if ( attribute.first == "api" )
      {
        api = attribute.second;
      }
      else if ( attribute.first == "deprecated" )
      {
        deprecated = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
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

    std::string bitpos, name, value;
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
    assert( !name.empty() );

    checkForError(
      "vk.xml", !containsByName( enumValues.second.values, name ), line, "enum value <" + name + "> already specified for enum <" + enumValues.first + ">" );
    checkForError( "vk.xml",
                   ( enumValues.second.type != "bitmask" ) || ( bitpos.empty() ^ value.empty() ),
                   line,
                   "both or none of \"bitpos\" and \"value\" are set for bitmask enum <" + name + "> which is invalid" );
    enumValues.second.values.push_back( { .bitpos = bitpos, .name = name, .value = value, .xmlLine = line } );
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

  Enums enums;
  if ( type == "constants" )
  {
    assert( bitwidth.empty() && ( name == "API Constants" ) );
    checkElements( "vk.xml", line, children, { { "enum", MultipleAllowed::Yes } } );
    for ( auto const & child : children )
    {
      auto constant = parseConstant( child );
      checkForError( "vk.xml", enums.constants.insert( constant ).second, line, "enum constant <" + constant.first + "> already specified" );
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
  for ( auto attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else
    {
      require = attribute.second;
    }
  }

  return { name, { .includedBy = require, .xmlLine = line } };
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
      objTypeEnum = attribute.second;
    }
    else if ( attribute.first == "parent" )
    {
      parent = attribute.second;
    }
  }

  auto [name, type] = parseNameAndType( handleElement );

  checkForError( "vk.xml", name.starts_with( "Vk" ), line, "name <" + name + "> does not begin with <Vk>" );
  checkForError( "vk.xml",
                 ( type.name == "VK_DEFINE_HANDLE" ) || ( type.name == "VK_DEFINE_NON_DISPATCHABLE_HANDLE" ),
                 line,
                 "handle with invalid type <" + type.name + ">" );

  return { name, { .objTypeEnum = objTypeEnum, .parent = parent, .type = type, .xmlLine = line } };
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
                 element->PreviousSibling() &&
                   ( strcmp( element->PreviousSibling()->Value(), "[" ) == 0 ) &&
                   element->NextSibling() &&
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
  checkForError( "vk.xml", !name.empty(), line, "empty name element" );
  return name;
}

std::pair<std::string, Type> parseNameAndType( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "vk.xml", line, children, { { "name", MultipleAllowed::No } }, { { "type", MultipleAllowed::No } } );

  std::string name;
  Type        type;
  for ( auto child : children )
  {
    std::string const value = child->Value();
    if ( value == "name" )
    {
      name = parseName( child );
    }
    else
    {
      type = parseType( child );
    }
  }
  return { name, type };
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
      name = attribute.second;
    }
    else if ( attribute.first == "protect" )
    {
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
    if ( value == "comment" )
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
        vkxml.constants = std::move( enums.constants );
      }
      for ( auto const & enumValue : enums.enumValues )
      {
        auto enumIt = vkxml.enums.find( enumValue.first );
        checkForError( "vk.xml", enumIt != vkxml.enums.end(), line, "enum <" + enumValue.first + "> not specified" );
        enumIt->second.values   = std::move( enumValue.second.values );
        enumIt->second.bitwidth = enumValue.second.bitwidth;
        enumIt->second.type     = enumValue.second.type;
      }
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
    }
    // else
    //{
    //   checkForError( "vk.xml", false, line, "unknown element <" + value + ">" );
    // }
  }
  checkForError( "VkXMLParser", !vkxml.copyright.text.empty(), line, "Copyright message is missing" );

  return vkxml;
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
      structure.allowDuplicate = ( attribute.second == "true" );
    }
    else if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "requiredlimittype" )
    {
      assert( attribute.second == "true" );
      structure.requiredLimitType = true;
    }
    else if ( attribute.first == "returnedonly" )
    {
      assert( attribute.second == "true" );
      structure.returnedOnly = true;
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

  checkForError( "vk.xml",
                 !structure.requiredLimitType ||
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
      member.altLen = attribute.second;
    }
    else if ( attribute.first == "api" )
    {
      member.api = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "deprecated" )
    {
      member.deprecated = attribute.second;
    }
    else if ( attribute.first == "externsync" )
    {
      member.externSync = attribute.second;
    }
    else if ( attribute.first == "featurelink" )
    {
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
      member.limitType = attribute.second;
    }
    else if ( attribute.first == "noautovalidity" )
    {
      assert( attribute.second == "true" );
      member.noAutoValidity = true;
    }
    else if ( attribute.first == "objecttype" )
    {
      assert( attribute.second == "objectType" );
      member.objectType = true;
    }
    else if ( attribute.first == "optional" )
    {
      std::vector<std::string> optional = tokenize( attribute.second, "," );
      for ( auto const & o : optional )
      {
        member.optional.push_back( o == "true" );
      }
    }
    else if ( attribute.first == "selector" )
    {
      member.selector = attribute.second;
    }
    else if ( attribute.first == "values" )
    {
      member.values = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "enum" )
    {
      assert( member.arraySizes.empty() );
      member.arraySizes.push_back( parseMemberEnum( child ) );
    }
    else if ( value == "name" )
    {
      member.name                                    = parseName( child );
      std::tie( member.arraySizes, member.bitCount ) = readModifiers( "vk.xml", child->NextSibling() );
    }
    else if ( value == "type" )
    {
      member.type = parseType( child );
    }
  }
  assert( !member.name.empty() );

  return member;
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
    std::string prefix = preType->Value();
    checkForError( "vk.xml",
                   ( prefix == "const " ) || ( prefix == "const struct " ) || ( prefix == "struct " ) || ( prefix == "typedef " ),
                   line,
                   "unexpected text <" + prefix + "> before type" );
    // change from west-const to east-const
    if ( prefix.starts_with( "const " ) )
    {
      type.postfix = "const ";
    }
    if ( prefix.find( "struct " ) != std::string::npos )
    {
      type.prefix = "struct ";
    }
  }

  tinyxml2::XMLNode const * postType = element->NextSibling();
  if ( postType && postType->ToText() )
  {
    std::string postfix = trim( postType->Value() );
    checkForError( "vk.xml",
                   ( postfix == "*" ) || ( postfix == "* const*" ) || ( postfix == "* const *" ) || ( postfix == "(" ),
                   line,
                   "unexpected text <" + postfix + "> after type" );
    if ( postfix != "(" )
    {
      type.postfix += postfix;
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

  // check that each selector in a struct actually is an enum
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

        // check that the selector type is a union
        checkForError( "vk.xml",
                       types.unions.contains( member.type.name ),
                       member.xmlLine,
                       "struct member <" +
                         member.name +
                         "> in struct <" +
                         structName +
                         "> has selector <" +
                         member.selector +
                         "> but its type <" +
                         member.type.name +
                         "> is not a union" );
      }
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
        "vk.xml", types.baseTypes.insert( std::move( baseType ) ).second, baseType.second.xmlLine, "basetype <" + baseType.first + "> already specified" );
    }
    else if ( categoryIt->second == "bitmask" )
    {
      if ( attributes.contains( "alias" ) )
      {
        CategoryAlias alias     = parseCategoryAlias( element, attributes, "bitmask" );
        auto          bitmaskIt = types.bitmasks.find( alias.alias );
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
            "vk.xml", types.bitmasks.insert( std::move( bitmask ) ).second, bitmask.second.xmlLine, "bitmask <" + bitmask.first + "> already specified" );
        }
      }
    }
    else if ( categoryIt->second == "define" )
    {
      auto [defineApi, define] = parseDefine( element, attributes );
      if ( std::ranges::any_of( defineApi, [&api]( auto const & a ) { return a == api; } ) )
      {
        checkForError( "vk.xml", types.defines.insert( std::move( define ) ).second, define.second.xmlLine, "define <" + define.first + "> already specified" );
      }
    }
    else if ( categoryIt->second == "enum" )
    {
      if ( attributes.contains( "alias" ) )
      {
        CategoryAlias alias  = parseCategoryAlias( element, attributes, "enum" );
        auto          enumIt = types.enums.find( alias.alias );
        checkForError(
          "vk.xml", enumIt != types.enums.end(), alias.xmlLine, "handle <" + alias.name + "> is an alias of an unknown handle <" + alias.alias + ">." );
        checkForError( "vk.xml",
                       enumIt->second.aliases.insert( { alias.name, alias.xmlLine } ).second,
                       alias.xmlLine,
                       "handle alias <" + alias.name + "> is already listed as an alias for handle <" + alias.alias + ">" );
      }
      else
      {
        std::pair<std::string, Enum> enumData = parseEnum( element, attributes );
        checkForError(
          "vk.xml", types.enums.insert( std::move( enumData ) ).second, enumData.second.xmlLine, "handle <" + enumData.first + "> already specified" );
      }
    }
    else if ( categoryIt->second == "handle" )
    {
      if ( attributes.contains( "alias" ) )
      {
        CategoryAlias alias    = parseCategoryAlias( element, attributes, "handle" );
        auto          handleIt = types.handles.find( alias.alias );
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
        checkForError( "vk.xml", types.handles.insert( std::move( handle ) ).second, handle.second.xmlLine, "handle <" + handle.first + "> already specified" );
      }
    }
    else if ( categoryIt->second == "include" )
    {
      std::pair<std::string, int> include = parseInclude( element, attributes );
      checkForError( "vk.xml", types.includes.insert( std::move( include ) ).second, include.second, "include <" + include.first + "> already specified" );
    }
    else if ( categoryIt->second == "funcpointer" )
    {
      std::pair<std::string, FuncPointer> funcPointer = parseFuncPointer( element, attributes );
      checkForError( "vk.xml",
                     types.funcPointers.insert( std::move( funcPointer ) ).second,
                     funcPointer.second.xmlLine,
                     "funcpointer <" + funcPointer.first + "> already specified" );
    }
    else if ( categoryIt->second == "struct" )
    {
      if ( attributes.contains( "alias" ) )
      {
        CategoryAlias alias = parseCategoryAlias( element, attributes, "struct" );
        checkForError( "vk.xml", !containsByName( types.structAliases, alias.name ), alias.xmlLine, "struct alias <" + alias.name + "> already encountered" );
        types.structAliases.push_back( std::move( alias ) );
      }
      else
      {
        std::pair<std::string, Struct> structure = parseStruct( element, attributes, api );
        checkForError(
          "vk.xml", types.structs.insert( std::move( structure ) ).second, structure.second.xmlLine, "struct <" + structure.first + "> already specified" );
      }
    }
    else if ( categoryIt->second == "union" )
    {
      std::pair<std::string, Union> u = parseUnion( element, attributes );
      checkForError( "vk.xml", types.unions.insert( std::move( u ) ).second, u.second.xmlLine, "union <" + u.first + "> already specified" );
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
                   types.includes.contains( externalType.second.includedBy ),
                   externalType.second.xmlLine,
                   "type <" + externalType.first + "> requires unknown <" + externalType.second.includedBy + ">" );
    checkForError( "vk.xml",
                   types.externalTypes.insert( std::move( externalType ) ).second,
                   externalType.second.xmlLine,
                   "external type <" + externalType.first + "> already specified" );
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
      member.len = attribute.second;
    }
    else if ( attribute.first == "noautovalidity" )
    {
      assert( attribute.second == "true" );
      member.noAutoValidity = true;
    }
    else if ( attribute.first == "optional" )
    {
      assert( attribute.second == "true" );
      member.optional = true;
    }
    else if ( attribute.first == "selection" )
    {
      member.selection = tokenize( attribute.second, "," );
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "name" )
    {
      member.name = parseName( child );
      std::string bitCount;
      std::tie( member.arraySizes, bitCount ) = readModifiers( "vk.xml", child->NextSibling() );
      assert( bitCount.empty() );
    }
    else if ( value == "type" )
    {
      member.type = parseType( child );
    }
  }
  assert( !member.name.empty() );

  return member;
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
      name = attribute.second;
    }
    else if ( attribute.first == "returnedonly" )
    {
      assert( attribute.second == "true" );
      u.returnedOnly = true;
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

Vkxml parseVkXml( tinyxml2::XMLDocument const & document, std::string const & api )
{
  std::vector<tinyxml2::XMLElement const *> elements = getChildElements( &document );
  checkElements( "vk.xml", document.GetLineNum(), elements, { { "registry", MultipleAllowed::No } } );
  return parseRegistry( elements[0], api );
}
