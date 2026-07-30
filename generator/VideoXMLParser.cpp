// SPDX-FileCopyrightText: 2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

#include "VideoXMLParser.hpp"

#include "XMLHelper.hpp"

#include <vector>

CategoryDefine         parseCategoryDefine( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
CategoryStruct         parseCategoryStruct( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
Constant               parseConstant( tinyxml2::XMLElement const * element );
Enum                   parseEnum( tinyxml2::XMLElement const * element );
EnumValue              parseEnumValue( tinyxml2::XMLElement const * element );
Extension              parseExtension( tinyxml2::XMLElement const * element );
std::vector<Extension> parseExtensions( tinyxml2::XMLElement const * element );
VideoXML               parseRegistry( tinyxml2::XMLElement const * element );
Require                parseRequire( tinyxml2::XMLElement const * element );
StructMember           parseStructMember( tinyxml2::XMLElement const * element );
Text                   parseText( tinyxml2::XMLElement const * element );
Types                  parseTypes( tinyxml2::XMLElement const * element );
void                   parseTypesType( tinyxml2::XMLElement const * element, Types & types );

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

Constant parseConstant( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "video.xml", line, attributes, { { "name", {} }, { "value", {} } }, { { "type", { "uint32_t", "uint8_t" } } } );
  checkElements( "videao.xml", line, getChildElements( element ), {} );

  Constant constant{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      checkNoList( "video.xml", attribute.second, line );
      constant.name = attribute.second;
    }
    else if ( attribute.first == "type" )
    {
      checkNoList( "video.xml", attribute.second, line );
      constant.type = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      checkNoList( "video.xml", attribute.second, line );
      constant.value = attribute.second;
    }
  }

  if ( !( constant.name.ends_with( "_SPEC_VERSION" ) || constant.name.ends_with( "_EXTENSION_NAME" ) ) )
  {
    checkForError( "video.xml", !constant.type.empty(), line, "constant <" + constant.name + "> has no type specified" );
    checkForError(
      "video.xml", isNumber( constant.value ) || isHexNumber( constant.value ), line, "enum value uses unknown constant <" + constant.value + ">" );
  }

  return constant;
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

Extension parseExtension( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "video.xml", line, attributes, { { "comment", {} }, { "name", {} }, { "number", {} }, { "supported", { "vulkan" } } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "video.xml", line, children, { { "require", MultipleAllowed::No } } );

  Extension extension{ .xmlLine = line };
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "comment" )
    {
      checkNoList( "video.xml", attribute.second, line );
      checkForError( "video.xml",
                     attribute.second.starts_with( "protect with VULKAN_VIDEO_CODEC" ),
                     line,
                     "unexpected content of attribute <comment>: \"" + attribute.second + "\"" );
      extension.protect = attribute.second.substr( strlen( "protect with " ) );
    }
    else if ( attribute.first == "name" )
    {
      checkNoList( "video.xml", attribute.second, line );
      extension.name = attribute.second;
    }
    else if ( attribute.first == "number" )
    {
      checkNoList( "video.xml", attribute.second, line );
      checkForError( "video.xml", isNumber( attribute.second ), line, "extension number <" + attribute.second + "> is not a number" );
      extension.number = attribute.second;
    }
    else if ( attribute.first == "supported" )
    {
      checkNoList( "video.xml", attribute.second, line );
      extension.supported = attribute.second;
    }
  }

  for ( auto child : children )
  {
    std::string const value = child->Value();
    if ( value == "require" )
    {
      extension.require = parseRequire( child );
    }
  }

  return extension;
}

std::vector<Extension> parseExtensions( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( "video.xml", line, getAttributes( element ), {}, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "video.xml", line, children, { { "extension", MultipleAllowed::Yes } } );

  std::vector<Extension> extensions;
  for ( auto child : children )
  {
    std::string const value = child->Value();
    if ( value == "extension" )
    {
      Extension extension = parseExtension( child );

      checkForError( "video.xml", !containsByName( extensions, extension.name ), extension.xmlLine, "already encountered extension <" + extension.name + ">" );
      checkForError( "video.xml",
                     std::ranges::none_of( extensions, [&number = extension.number]( auto const & extension ) { return extension.number == number; } ),
                     extension.xmlLine,
                     "extension number <" + extension.number + "> already encountered" );

      extensions.push_back( std::move( extension ) );
    }
  }

  return extensions;
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
    else if ( value == "enums" )
    {
      videoXML.enums.push_back( parseEnum( child ) );
    }
    else if ( value == "extensions" )
    {
      std::vector<Extension> extensions = parseExtensions( child );

      for ( auto const & extension : extensions )
      {
        checkForError( "video.xml",
                       extension.require.include.name.empty() || containsByName( videoXML.types.includes, extension.require.include.name ),
                       extension.require.xmlLine,
                       "extension <" + extension.name + "> uses unknown header <" + extension.require.include.name + ">" );
      }

      videoXML.extensions = std::move( extensions );
    }
    else if ( value == "types" )
    {
      videoXML.types = parseTypes( child );
    }
  }
  checkForError( "video.xml", !videoXML.copyrightMessage.empty(), -1, "missing copyright message" );
  return videoXML;
}

Require parseRequire( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( "video.xml", line, attributes, {}, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( "video.xml", line, children, { { "type", MultipleAllowed::Yes } }, { { "enum", MultipleAllowed::Yes } } );

  Require require{ .xmlLine = line };
  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "enum" )
    {
      Constant constant = parseConstant( child );
      checkForError(
        "video.xml", !containsByName( require.enums, constant.name ), constant.xmlLine, "required enum <" + constant.name + "> already specified" );
      require.enums.push_back( std::move( constant ) );
    }
    else if ( value == "type" )
    {
      NameElement type = parseNameElement( child );
      if ( type.name.starts_with( "vk_video/vulkan_video_codec" ) && type.name.ends_with( ".h" ) )
      {
        checkForError( "video.xml", require.include.name.empty(), type.xmlLine, "require section holds more than one include type: <" + type.name + ">" );
        require.include = std::move( type );
      }
      else
      {
        checkForError( "video.xml", !containsByName( require.types, type.name ), type.xmlLine, "required type <" + type.name + "> already specified" );
        require.types.push_back( std::move( type ) );
      }
    }
  }
  return require;
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
      TypeInclude include = parseTypeInclude( "video.xml", element, attributes );

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

    TypeExternal external = parseTypeExternal( "video.xml", element, attributes );

    checkForError( "video.xml",
                   containsByName( types.includes, external.require ),
                   external.xmlLine,
                   "external type <" + external.name + "> requires unknown <" + external.require + ">" );
    checkForError( "video.xml", !containsByName( types.externals, external.name ), external.xmlLine, "struct <" + external.name + "> already specified" );

    types.externals.push_back( std::move( external ) );
  }
}

VideoXML parseVideoXML( tinyxml2::XMLDocument const & document )
{
  std::vector<tinyxml2::XMLElement const *> elements = getChildElements( &document );
  checkElements( "vk.xml", document.GetLineNum(), elements, { { "registry", MultipleAllowed::No } } );
  return parseRegistry( elements[0] );
}
