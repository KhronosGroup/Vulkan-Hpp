// SPDX-FileCopyrightText: 2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

#include "VideoXMLParser.hpp"

#include "XMLHelper.hpp"

#include <vector>

CategoryDefine parseCategoryDefine( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
CategoryStruct parseCategoryStruct( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
EnumValue      parseEnumValue( tinyxml2::XMLElement const * element );
VideoXML       parseRegistry( tinyxml2::XMLElement const * element );
StructMember   parseStructMember( tinyxml2::XMLElement const * element );
Text           parseText( tinyxml2::XMLElement const * element );
void           parseTypesType( tinyxml2::XMLElement const * element, Types & types );

CategoryDefine parseCategoryDefine( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( "video.xml", line, attributes, { { "category", { "define" } } }, { { "requires", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "video.xml", line, children, { { "name", MultipleAllowed::No } }, { { "type", MultipleAllowed::No } } );

  CategoryDefine define{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "requires" )
    {
      checkNoList( "video.xml", attribute.second, line );
      define.require = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string const value = child->Value();
    if ( value == "name" )
    {
      define.name = parseText( child ).text;
    }
    else if ( value == "type" )
    {
      define.type = parseText( child ).text;
    }
  }

  return define;
}

CategoryStruct parseCategoryStruct( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( "video.xml", line, attributes, { { "category", { "struct" } }, { "name", {} } }, { { "comment", {} }, { "requires", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "video.xml", line, children, { { "member", MultipleAllowed::Yes } }, { { "comment", MultipleAllowed::Yes } } );

  CategoryStruct structure{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( "video.xml", attribute.second, line );
      structure.name = attribute.second;
    }
    else if ( attribute.first == "requires" )
    {
      checkNoList( "video.xml", attribute.second, line );
      structure.require = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "member" )
    {
      StructMember member = parseStructMember( child );

      checkForError( "video.xml", !containsByName( structure.members, member.name ), member.xmlLine, "struct member name <" + member.name + "> already used" );

      structure.members.push_back( member );
    }
  }

  return structure;
}

Enum parseEnum( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "video.xml", line, attributes, { { "name", {} }, { "type", { "enum" } } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "video.xml", line, children, { { "enum", MultipleAllowed::Yes } }, { { "comment", MultipleAllowed::No } } );

  Enum enumStruct;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( "video.xml", attribute.second, line );
      enumStruct.name = attribute.second;
    }
  }

  std::string prefix = toUpperCase( enumStruct.name ) + "_";

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "enum" )
    {
      EnumValue enumValue = parseEnumValue( child );

      if ( enumValue.alias.empty() )
      {
        checkForError( "video.xml",
                       enumValue.name.starts_with( prefix ),
                       enumValue.xmlLine,
                       "encountered enum value <" + enumValue.name + "> that does not begin with expected prefix <" + prefix + ">" );
      }

      enumStruct.values.push_back( std::move( enumValue ) );
    }
  }

  return enumStruct;
}

EnumValue parseEnumValue( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkElements( "video.xml", line, getChildElements( element ), {} );

  EnumValue value;
  if ( attributes.contains( "alias" ) )
  {
    checkAttributes( "video.xml", line, attributes, { { "alias", {} }, { "deprecated", { "aliased" } }, { "name", {} } }, {} );

    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "alias" )
      {
        checkNoList( "video.xml", attribute.second, line );
        value.alias = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        checkNoList( "video.xml", attribute.second, line );
        value.name = attribute.second;
      }
    }
  }
  else
  {
    checkAttributes( "video.xml", line, attributes, { { "name", {} }, { "value", {} } }, { { "comment", {} } } );

    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "comment" )
      {
        value.comment = attribute.second;
      }
      if ( attribute.first == "name" )
      {
        checkNoList( "video.xml", attribute.second, line );
        value.name = attribute.second;
      }
      else if ( attribute.first == "value" )
      {
        checkNoList( "video.xml", attribute.second, line );
        checkForError(
          "video.xml", isNumber( attribute.second ) || isHexNumber( attribute.second ), line, "enum value uses non-number value <" + attribute.second + ">" );
        value.value = attribute.second;
      }
    }
  }

  return value;
}

VideoXML parseRegistry( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements(
    "video.xml",
    line,
    children,
    { { "comment", MultipleAllowed::Yes }, { "enums", MultipleAllowed::Yes }, { "extensions", MultipleAllowed::No }, { "types", MultipleAllowed::No } } );

  VideoXML videoXML{ .xmlLine = line };
  for ( auto child : children )
  {
    std::string const value = child->Value();
    if ( value == "comment" )
    {
      Comment comment = parseComment( "video.xml", child );
      if ( comment.text.find( "\nCopyright" ) == 0 )
      {
        videoXML.copyrightMessage = generateCopyrightMessage( comment.text );
      }
    }
    else if (value == "enums")
    {
      videoXML.enums.push_back( parseEnum( child ) );
    }
    else if ( value == "types" )
    {
      videoXML.types = parseTypes( child );
    }
  }
  checkForError( "video.xml", !videoXML.copyrightMessage.empty(), -1, "missing copyright message" );
  return videoXML;
}

StructMember parseStructMember( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "video.xml", line, attributes, {}, { { "len", {} }, { "optional", { "false", "true" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "video.xml",
                 line,
                 children,
                 { { "name", MultipleAllowed::No }, { "type", MultipleAllowed::No } },
                 { { "comment", MultipleAllowed::No }, { "enum", MultipleAllowed::Yes } } );

  StructMember structMember{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "len" )
    {
      checkNoList( "video.xml", attribute.second, line );
      structMember.len = attribute.second;
      // the "len" attribute can be something completely unrelated to this struct!! Can't do a checkForError whatsoever.
    }
    else if ( attribute.first == "optional" )
    {
      checkNoList( "video.xml", attribute.second, line );
      structMember.optional = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "enum" )
    {
      Text text = parseText( child );

      checkForError( "video.xml",
                     child->PreviousSibling() && child->NextSibling(),
                     text.xmlLine,
                     "struct member array specification is ill-formatted: <" + text.text + ">" );
      std::string previous = child->PreviousSibling()->Value();
      std::string next     = child->NextSibling()->Value();
      checkForError( "video.xml",
                     previous.ends_with( "[" ) && next.starts_with( "]" ),
                     text.xmlLine,
                     "struct member array specification is ill-formatted: <" + text.text + ">" );

      structMember.arraySizes.push_back( text.text );
    }
    else if ( value == "name" )
    {
      structMember.name       = parseText( child ).text;
      NameModifiers modifiers = parseNameModifiers( "video.xml", child->NextSibling() );
      structMember.arraySizes = modifiers.arraySizes;
      structMember.bitCount   = modifiers.bitCount;
    }
    else if ( value == "type" )
    {
      structMember.type = parseType( "video.xml", child );
    }
  }

  return structMember;
}

Text parseText( tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( "vk.xml", line, getAttributes( element ), {}, {} );
  checkElements( "video.xml", line, getChildElements( element ), {} );

  assert( element->GetText() );
  return { .text = element->GetText(), .xmlLine = line };
}

Types parseTypes( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( "video.xml", line, getAttributes( element ), { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "video.xml", line, children, { { "type", MultipleAllowed::Yes } } );

  Types types{ .xmlLine = line };
  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "type" )
    {
      parseTypesType( child, types );
    }
  }

  return types;
}

void parseTypesType( tinyxml2::XMLElement const * element, Types & types )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );

  auto categoryIt = attributes.find( "category" );
  if ( categoryIt != attributes.end() )
  {
    if ( categoryIt->second == "define" )
    {
      CategoryDefine define = parseCategoryDefine( element, attributes );

      checkForError( "video.xml",
                     define.require.empty() || containsByName( types.defines, define.require ),
                     define.xmlLine,
                     "define <" + define.name + "> requires unknown type <" + define.require + ">" );
      checkForError( "video.xml",
                     define.type.empty() || containsByName( types.defines, define.type ),
                     line,
                     "define <" + define.name + "> of unknown type <" + define.type + ">" );
      checkForError( "video.xml", !containsByName( types.defines, define.name ), define.xmlLine, "define <" + define.name + "> already specified" );

      types.defines.push_back( std::move( define ) );
    }
    else if ( categoryIt->second == "enum" )
    {
      CategoryEnum categoryEnum = parseCategoryEnum( "video.xml", element, attributes );

      checkForError(
        "video.xml", !containsByName( types.enums, categoryEnum.name ), categoryEnum.xmlLine, "enum <" + categoryEnum.name + "> already specified" );

      types.enums.push_back( std::move( categoryEnum ) );
    }
    else if ( categoryIt->second == "include" )
    {
      CategoryInclude include = parseCategoryInclude( "video.xml", element, attributes );

      checkForError( "video.xml", !containsByName( types.enums, include.name ), include.xmlLine, "include <" + include.name + "> already specified" );

      types.includes.push_back( std::move( include ) );
    }
    else if ( categoryIt->second == "struct" )
    {
      CategoryStruct structure = parseCategoryStruct( element, attributes );

      checkForError( "video.xml",
                     structure.require.empty() || containsByName( types.enums, structure.require ),
                     structure.xmlLine,
                     "struct <" + structure.name + "> requires unknown enum <" + structure.require + ">" );
      checkForError( "video.xml", !containsByName( types.structs, structure.name ), structure.xmlLine, "struct <" + structure.name + "> already specified" );

      types.structs.push_back( std::move( structure ) );
    }
    else
    {
      checkForError( "video.xml", false, line, "unknown category <" + categoryIt->second + "> encountered" );
    }
  }
  else
  {
    auto requiresIt = attributes.find( "requires" );
    checkForError(
      "video.xml", requiresIt != attributes.end(), line, "type with neither attribute <category> not attribute <requires> - don't know how to handle" );

    ExternalType externalType = parseExternalType( "video.xml", element, attributes );

    checkForError( "video.xml",
                   containsByName( types.includes, externalType.require ),
                   externalType.xmlLine,
                   "external type <" + externalType.name + "> requires unknown <" + externalType.require + ">" );
    checkForError(
      "video.xml", !containsByName( types.externalTypes, externalType.name ), externalType.xmlLine, "struct <" + externalType.name + "> already specified" );

    types.externalTypes.push_back( std::move( externalType ) );
  }
}

VideoXML parseVideoXML( tinyxml2::XMLDocument const & document )
{
  std::vector<tinyxml2::XMLElement const *> elements = getChildElements( &document );
  checkElements( "vk.xml", document.GetLineNum(), elements, { { "registry", MultipleAllowed::No } } );
  return parseRegistry( elements[0] );
}
