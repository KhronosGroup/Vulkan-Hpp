// Copyright(c) 2023, NVIDIA CORPORATION. All rights reserved.
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

#include "VideoHppGenerator.hpp"

#include "XMLHelper.hpp"

#include <iostream>
#include <vector>

VideoHppGenerator::VideoHppGenerator( tinyxml2::XMLDocument const & document )
{
  // read the document and check its correctness
  int                                       line     = document.GetLineNum();
  std::vector<tinyxml2::XMLElement const *> elements = getChildElements( &document );
  checkElements( line, elements, { { "registry", true } } );
  checkForError( elements.size() == 1, line, "encountered " + std::to_string( elements.size() ) + " elements named <registry> but only one is allowed" );
  readRegistry( elements[0] );
  addImplicitlyRequiredTypes();
  sortStructs();
  checkCorrectness();
}

void VideoHppGenerator::generateHppFile() const
{
  std::string const video_hpp = std::string( BASE_PATH ) + "/vulkan/" + "vulkan_video.hpp";
  std::cout << "VideoHppGenerator: Generating " << video_hpp << " ... " << std::endl;

  std::string const videoHppTemplate = R"(${copyrightMessage}

#ifndef VULKAN_VIDEO_HPP
#define VULKAN_VIDEO_HPP

#include <vk_video/vulkan_video_codecs_common.h>
#include <vk_video/vulkan_video_codec_h264std.h>
#include <vk_video/vulkan_video_codec_h264std_decode.h>
#include <vk_video/vulkan_video_codec_h264std_encode.h>
#include <vk_video/vulkan_video_codec_h265std.h>
#include <vk_video/vulkan_video_codec_h265std_decode.h>
#include <vk_video/vulkan_video_codec_h265std_encode.h>
#include <vulkan/vulkan.hpp>

#if !defined( VULKAN_HPP_VIDEO_NAMESPACE )
#  define VULKAN_HPP_VIDEO_NAMESPACE video
#endif

namespace VULKAN_HPP_NAMESPACE
{
namespace VULKAN_HPP_VIDEO_NAMESPACE
{
${enums}
${structs}
}   // namespace VULKAN_HPP_VIDEO_NAMESPACE
}   // namespace VULKAN_HPP_NAMESPACE
#endif
)";

  std::string str =
    replaceWithMap( videoHppTemplate, { { "copyrightMessage", m_copyrightMessage }, { "enums", generateEnums() }, { "structs", generateStructs() } } );

  writeToFile( str, video_hpp );
}

void VideoHppGenerator::addImplicitlyRequiredTypes()
{
  for ( auto & ext : m_extensions )
  {
    for ( auto reqIt = ext.requireData.types.begin(); reqIt != ext.requireData.types.end(); ++reqIt )
    {
      std::string name   = *reqIt;
      auto        typeIt = m_types.find( *reqIt );
      if ( ( typeIt != m_types.end() ) && ( typeIt->second.category == TypeCategory::Struct ) )
      {
        assert( typeIt->second.requiredBy.contains( ext.name ) );
        reqIt = addImplicitlyRequiredTypes( typeIt, ext, reqIt );
      }
    }
  }
}

std::vector<std::string>::iterator VideoHppGenerator::addImplicitlyRequiredTypes( std::map<std::string, TypeData>::iterator typeIt,
                                                                                  ExtensionData &                           extensionData,
                                                                                  std::vector<std::string>::iterator        reqIt )
{
  auto structIt = m_structs.find( typeIt->first );
  assert( structIt != m_structs.end() );
  for ( auto const & member : structIt->second.members )
  {
    auto memberTypeIt = m_types.find( member.type.type );
    if ( ( memberTypeIt != m_types.end() ) && ( memberTypeIt->second.category == TypeCategory::Struct ) )
    {
      reqIt = addImplicitlyRequiredTypes( memberTypeIt, extensionData, reqIt );
    }
  }
  assert( typeIt->second.requiredBy.empty() || ( *typeIt->second.requiredBy.begin() == extensionData.name ) ||
          ( *typeIt->second.requiredBy.begin() == extensionData.depends ) );
  if ( typeIt->second.requiredBy.empty() && ( std::find( extensionData.requireData.types.begin(), reqIt, typeIt->first ) == reqIt ) )
  {
    assert( std::none_of( reqIt, extensionData.requireData.types.end(), [&typeIt]( std::string const & type ) { return type == typeIt->first; } ) );
    typeIt->second.requiredBy.insert( extensionData.name );
    reqIt = std::next( extensionData.requireData.types.insert( reqIt, typeIt->first ) );
  }
  return reqIt;
}

void VideoHppGenerator::checkCorrectness() const
{
  // only structs to check here!
  for ( auto const & structure : m_structs )
  {
    // check that a struct is referenced somewhere
    // I think, it's not forbidden to not reference a struct, but it would probably be not intended?
    auto typeIt = m_types.find( structure.first );
    assert( typeIt != m_types.end() );
    checkForError( !typeIt->second.requiredBy.empty(), structure.second.xmlLine, "structure <" + structure.first + "> not required by any extension" );

    assert( typeIt->second.requiredBy.size() == 1 );
    auto extIt =
      std::find_if( m_extensions.begin(), m_extensions.end(), [&typeIt]( ExtensionData const & ed ) { return ed.name == *typeIt->second.requiredBy.begin(); } );
    assert( extIt != m_extensions.end() );

    // checks on the members of a struct
    for ( auto const & member : structure.second.members )
    {
      // check that each member type is known
      checkForError( m_types.contains( member.type.type ), member.xmlLine, "struct member uses unknown type <" + member.type.type + ">" );

      // check that all member types are required in some extension (it's just a warning!!)
      if ( member.type.type.starts_with( "StdVideo" ) )
      {
        auto memberTypeIt = m_types.find( member.type.type );
        assert( memberTypeIt != m_types.end() );
        checkForWarning( !memberTypeIt->second.requiredBy.empty(),
                         member.xmlLine,
                         "struct member type <" + member.type.type + "> used in struct <" + structure.first + "> is never required for any extension" );
      }

      // check that all array sizes are a known constant
      for ( auto const & arraySize : member.arraySizes )
      {
        if ( !isNumber( arraySize ) )
        {
          bool found = extIt->requireData.constants.contains( arraySize );
          if ( !found )
          {
            checkForError(
              !extIt->depends.empty(), extIt->xmlLine, "struct member <" + member.name + "> uses unknown constant <" + arraySize + "> as array size" );
            auto depIt = std::find_if( m_extensions.begin(), m_extensions.end(), [&extIt]( ExtensionData const & ed ) { return ed.name == extIt->depends; } );
            assert( depIt != m_extensions.end() );
            checkForError( depIt->requireData.constants.contains( arraySize ),
                           member.xmlLine,
                           "struct member <" + member.name + "> uses unknown constant <" + arraySize + "> as array size" );
          }
        }
      }
    }
  }
}

std::string VideoHppGenerator::generateEnum( std::pair<std::string, EnumData> const & enumData ) const
{
  std::string enumValues;
#if !defined( NDEBUG )
  std::map<std::string, std::string> valueToNameMap;
#endif

  // convert the enum name to upper case
  std::string prefix = toUpperCase( enumData.first ) + "_";
  for ( auto const & value : enumData.second.values )
  {
    std::string valueName = "e" + toCamelCase( stripPrefix( value.name, prefix ), true );
    assert( valueToNameMap.insert( { valueName, value.name } ).second );
    enumValues += "    " + valueName + " = " + value.name + ",\n";
  }

  if ( !enumValues.empty() )
  {
    size_t pos = enumValues.rfind( ',' );
    assert( pos != std::string::npos );
    enumValues.erase( pos, 1 );
    enumValues = "\n" + enumValues + "  ";
  }

  const std::string enumTemplate = R"(  enum class ${enumName}
  {${enumValues}};
)";

  return replaceWithMap( enumTemplate, { { "enumName", stripPrefix( enumData.first, "StdVideo" ) }, { "enumValues", enumValues } } );
}

std::string VideoHppGenerator::generateEnums() const
{
  {
    const std::string enumsTemplate = R"(
  //=============
  //=== ENUMs ===
  //=============

${enums}
)";

    std::string enums;
    for ( auto const & extension : m_extensions )
    {
      enums += generateEnums( extension.requireData, extension.name );
    }

    return replaceWithMap( enumsTemplate, { { "enums", enums } } );
  }
}

std::string VideoHppGenerator::generateEnums( RequireData const & requireData, std::string const & title ) const
{
  std::string str;
  for ( auto const & type : requireData.types )
  {
    auto enumIt = m_enums.find( type );
    if ( enumIt != m_enums.end() )
    {
      str += "\n" + generateEnum( *enumIt );
    }
  }
  if ( !str.empty() )
  {
    str = "\n    //=== " + title + " ===\n" + str;
  }
  return str;
}

std::string VideoHppGenerator::generateStruct( std::pair<std::string, StructureData> const & structData ) const
{
  static const std::string structureTemplate = R"(  struct ${structureType}
  {
    using NativeType = StdVideo${structureType};

    operator StdVideo${structureType} const &() const VULKAN_HPP_NOEXCEPT
    {
      return *reinterpret_cast<const StdVideo${structureType}*>( this );
    }

    operator StdVideo${structureType} &() VULKAN_HPP_NOEXCEPT
    {
      return *reinterpret_cast<StdVideo${structureType}*>( this );
    }
${compareOperators}
    public:
${members}
  };
)";

  return replaceWithMap( structureTemplate,
                         { { "compareOperators", generateStructCompareOperators( structData ) },
                           { "members", generateStructMembers( structData ) },
                           { "structureType", stripPrefix( structData.first, "StdVideo" ) } } );
}

std::string VideoHppGenerator::generateStructCompareOperators( std::pair<std::string, StructureData> const & structData ) const
{
  static const std::set<std::string> simpleTypes = { "char",   "double",  "DWORD",    "float",    "HANDLE",  "HINSTANCE", "HMONITOR",
                                                     "HWND",   "int",     "int8_t",   "int16_t",  "int32_t", "int64_t",   "LPCWSTR",
                                                     "size_t", "uint8_t", "uint16_t", "uint32_t", "uint64_t" };

  // two structs are compared by comparing each of the elements
  std::string compareMembers;
  std::string intro = "";
  for ( size_t i = 0; i < structData.second.members.size(); i++ )
  {
    MemberData const & member = structData.second.members[i];
    auto               typeIt = m_types.find( member.type.type );
    assert( typeIt != m_types.end() );
    if ( ( typeIt->second.category == TypeCategory::ExternalType ) && member.type.postfix.empty() && !simpleTypes.contains( member.type.type ) )
    {
      // this type might support operator==() or operator<=>()... that is, use memcmp
      compareMembers += intro + "( memcmp( &" + member.name + ", &rhs." + member.name + ", sizeof( " + member.type.type + " ) ) == 0 )";
    }
    else
    {
      assert( member.type.type != "char" );
      // for all others, we use the operator== of that type
      compareMembers += intro + "( " + member.name + " == rhs." + member.name + " )";
    }
    intro = "\n          && ";
  }

  static const std::string compareTemplate = R"(
    bool operator==( ${name} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return ${compareMembers};
    }

    bool operator!=( ${name} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return !operator==( rhs );
    }
)";

  return replaceWithMap( compareTemplate, { { "name", stripPrefix( structData.first, "StdVideo" ) }, { "compareMembers", compareMembers } } );
}

std::string VideoHppGenerator::generateStructMembers( std::pair<std::string, StructureData> const & structData ) const
{
  std::string members;
  for ( auto const & member : structData.second.members )
  {
    members += "    ";
    std::string type;
    if ( !member.bitCount.empty() && member.type.type.starts_with( "StdVideo" ) )
    {
      assert( member.type.prefix.empty() && member.type.postfix.empty() );  // never encounterd a different case
      type = member.type.type;
    }
    else if ( member.arraySizes.empty() )
    {
      type = member.type.compose( "VULKAN_HPP_NAMESPACE::VULKAN_HPP_VIDEO_NAMESPACE", "StdVideo" );
    }
    else
    {
      assert( member.type.prefix.empty() && member.type.postfix.empty() );
      type = generateStandardArrayWrapper( member.type.compose( "VULKAN_HPP_NAMESPACE::VULKAN_HPP_VIDEO_NAMESPACE" ), member.arraySizes );
    }
    members += type + " " + member.name;

    // as we don't have any meaningful default initialization values, everything can be initialized by just '{}' !
    assert( member.arraySizes.empty() || member.bitCount.empty() );
    if ( !member.bitCount.empty() )
    {
      members += " : " + member.bitCount;  // except for bitfield members, where no default member initialization
                                           // is supported (up to C++20)
    }
    else
    {
      members += " = ";
      auto enumIt = m_enums.find( member.type.type );
      if ( member.arraySizes.empty() && ( enumIt != m_enums.end() ) && member.type.postfix.empty() )
      {
        assert( member.type.prefix.empty() && member.arraySizes.empty() && !enumIt->second.values.empty() );

        std::string prefix    = toUpperCase( member.type.type ) + "_";
        std::string valueName = "e" + toCamelCase( stripPrefix( enumIt->second.values.front().name, prefix ), true );

        members += type + "::" + valueName;
      }
      else
      {
        members += "{}";
      }
    }
    members += ";\n";
  }
  return members;
}

std::string VideoHppGenerator::generateStructs() const
{
  const std::string structsTemplate = R"(
  //===============
  //=== STRUCTS ===
  //===============

${structs}
)";

  std::string structs;
  for ( auto const & extension : m_extensions )
  {
    structs += generateStructs( extension.requireData, extension.name );
  }
  return replaceWithMap( structsTemplate, { { "structs", structs } } );
}

std::string VideoHppGenerator::generateStructs( RequireData const & requireData, std::string const & title ) const
{
  std::string str;
  for ( auto const & type : requireData.types )
  {
    auto structIt = m_structs.find( type );
    if ( structIt != m_structs.end() )
    {
      str += "\n" + generateStruct( *structIt );
    }
  }
  if ( !str.empty() )
  {
    str = "\n    //=== " + title + " ===\n" + str;
  }
  return str;
}

bool VideoHppGenerator::isExtension( std::string const & name ) const
{
  return std::any_of( m_extensions.begin(), m_extensions.end(), [&name]( ExtensionData const & ed ) { return ed.name == name; } );
}

void VideoHppGenerator::readEnums( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} }, { "type", { "enum" } } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "enum", {} } }, { "comment" } );

  std::string name, type;
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
  }

  // get the EnumData entry in enum map
  auto enumIt = m_enums.find( name );
  checkForError( enumIt != m_enums.end(), line, "enum <" + name + "> is not listed as enum in the types section" );
  checkForError( enumIt->second.values.empty(), line, "enum <" + name + "> already holds values" );

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

void VideoHppGenerator::readEnumsEnum( tinyxml2::XMLElement const * element, std::map<std::string, EnumData>::iterator enumIt )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} }, { "value", {} } }, { { "comment", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string name, value;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      value = attribute.second;
    }
  }

  std::string prefix = toUpperCase( enumIt->first ) + "_";
  checkForError( name.starts_with( prefix ), line, "encountered enum value <" + name + "> that does not begin with expected prefix <" + prefix + ">" );
  checkForError( isNumber( value ) || isHexNumber( value ), line, "enum value uses unknown constant <" + value + ">" );

  checkForError( std::none_of( enumIt->second.values.begin(), enumIt->second.values.end(), [&name]( EnumValueData const & evd ) { return evd.name == name; } ),
                 line,
                 "enum value <" + name + "> already part of enum <" + enumIt->first + ">" );
  enumIt->second.values.push_back( { name, value, line } );
}

void VideoHppGenerator::readExtension( tinyxml2::XMLElement const * element )
{
  int                                       line       = element->GetLineNum();
  std::map<std::string, std::string>        attributes = getAttributes( element );
  std::vector<tinyxml2::XMLElement const *> children   = getChildElements( element );

  checkAttributes( line, attributes, { { "name", {} }, { "comment", {} }, { "supported", { "vulkan" } } }, {} );
  checkElements( line, children, { { "require", false } } );

  ExtensionData extensionData{ .xmlLine = line };
  std::string   supported;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      extensionData.name = attribute.second;
      checkForError( !isExtension( extensionData.name ), line, "already encountered extension <" + extensionData.name + ">" );
    }
    else if ( attribute.first == "supported" )
    {
      supported = attribute.second;
    }
  }
  checkForError( supported == "vulkan", line, "extension <" + extensionData.name + "> has unknown supported type <" + supported + ">" );

  for ( auto child : children )
  {
    const std::string value = child->Value();
    assert( value == "require" );
    readExtensionRequire( child, extensionData );
  }

  m_extensions.push_back( extensionData );
}

void VideoHppGenerator::readExtensionRequire( tinyxml2::XMLElement const * element, ExtensionData & extensionData )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "enum", "type" } );

  extensionData.requireData.xmlLine = line;

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "enum" )
    {
      readRequireEnum( child, extensionData.requireData.constants );
    }
    else if ( value == "type" )
    {
      readRequireType( child, extensionData );
    }
  }
  assert( !extensionData.requireData.types.empty() );
}

void VideoHppGenerator::readExtensions( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "extension", false } } );

  for ( auto child : children )
  {
    const std::string value = child->Value();
    assert( value == "extension" );
    readExtension( child );
  }
}

void VideoHppGenerator::readRegistry( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "comment", false }, { "enums", false }, { "extensions", true }, { "types", true } } );
  for ( auto child : children )
  {
    const std::string value = child->Value();
    if ( value == "comment" )
    {
      std::string comment = readComment( child );
      if ( comment.find( "\nCopyright" ) == 0 )
      {
        m_copyrightMessage = generateCopyrightMessage( comment );
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
    else
    {
      assert( value == "types" );
      readTypes( child );
    }
  }
  checkForError( !m_copyrightMessage.empty(), -1, "missing copyright message" );
}

void VideoHppGenerator::readRequireEnum( tinyxml2::XMLElement const * element, std::map<std::string, ConstantData> & constants )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkElements( line, getChildElements( element ), {} );
  checkAttributes( line, attributes, { { "name", {} }, { "value", {} } }, {} );

  std::string name, value;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      value = attribute.second;
    }
  }

  if ( !name.ends_with( "_SPEC_VERSION" ) && !name.ends_with( "_EXTENSION_NAME" ) )
  {
    checkForError( isNumber( value ) || isHexNumber( value ), line, "enum value uses unknown constant <" + value + ">" );
    checkForError( constants.insert( { name, { value, line } } ).second, line, "required enum <" + name + "> already specified" );
  }
}

void VideoHppGenerator::readRequireType( tinyxml2::XMLElement const * element, ExtensionData & extensionData )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, { { "comment", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string name = attributes.find( "name" )->second;
  if ( name.starts_with( "vk_video/vulkan_video_codec" ) && name.ends_with( ".h" ) )
  {
    checkForError( extensionData.depends.empty(), line, "extension <" + extensionData.name + "> already depends on <" + extensionData.name + ">" );
    extensionData.depends = stripPrefix( stripPostfix( name, ".h" ), "vk_video/" );
    checkForError( isExtension( extensionData.depends ), line, "extension <" + extensionData.name + "> uses unknown header <" + name + ">" );
  }
  else
  {
    auto typeIt = m_types.find( name );
    checkForError( typeIt != m_types.end(), line, "unknown required type <" + name + ">" );
    typeIt->second.requiredBy.insert( extensionData.name );
    extensionData.requireData.types.push_back( name );
  }
}

void VideoHppGenerator::readStructMember( tinyxml2::XMLElement const * element, std::vector<MemberData> & members )
{
  (void)members;
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "name", true }, { "type", true } }, { "comment", "enum" } );

  MemberData memberData;
  memberData.xmlLine = line;

  std::string name;
  for ( auto child : children )
  {
    int childLine = child->GetLineNum();
    checkAttributes( childLine, getAttributes( child ), {}, {} );
    checkElements( childLine, getChildElements( child ), {}, {} );

    std::string value = child->Value();
    if ( value == "enum" )
    {
      std::string enumString = child->GetText();

      checkForError( child->PreviousSibling() && child->NextSibling(), line, "struct member array specification is ill-formatted: <" + enumString + ">" );
      std::string previous = child->PreviousSibling()->Value();
      std::string next     = child->NextSibling()->Value();
      checkForError( previous.ends_with( "[" ) && next.starts_with( "]" ), line, "struct member array specification is ill-formatted: <" + enumString + ">" );

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

  checkForError( std::none_of( members.begin(), members.end(), [&name]( MemberData const & md ) { return md.name == name; } ),
                 line,
                 "struct member name <" + name + "> already used" );
  memberData.name = name;
  members.push_back( memberData );
}

void VideoHppGenerator::readTypeDefine( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "define" } } }, { { "requires", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "name", false } }, { "type" } );

  std::string require;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "requires" )
    {
      require = attribute.second;
    }
  }

  std::string name, type;
  for ( auto child : children )
  {
    const std::string value = child->Value();
    if ( value == "name" )
    {
      name = child->GetText();
    }
    else if ( value == "type" )
    {
      type = child->GetText();
    }
  }
  checkForError( require.empty() || m_defines.contains( require ), line, "define <" + name + "> requires unknown type <" + require + ">" );
  checkForError( type.empty() || m_defines.contains( type ), line, "define <" + name + "> of unknown type <" + type + ">" );

  checkForError( m_types.insert( { name, TypeData{ TypeCategory::Define, {}, line } } ).second, line, "define <" + name + "> already specified" );
  assert( !m_defines.contains( name ) );
  m_defines[name] = { require, line };
}

void VideoHppGenerator::readTypeEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "enum" } }, { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
  }

  checkForError( m_types.insert( { name, TypeData{ TypeCategory::Enum, {}, line } } ).second, line, "enum <" + name + "> already specified" );
  assert( !m_enums.contains( name ) );
  m_enums[name] = EnumData{ .xmlLine = line };
}

void VideoHppGenerator::readTypeInclude( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "include" } }, { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name = attributes.find( "name" )->second;
  checkForError( m_types.insert( { name, TypeData{ TypeCategory::Include, {}, line } } ).second, line, "type <" + name + "> already specified" );
  assert( !m_includes.contains( name ) );
  m_includes[name] = { line };
}

void VideoHppGenerator::readTypeRequires( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
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

void VideoHppGenerator::readTypes( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "type", false } } );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "type" )
    {
      readTypesType( child );
    }
  }
}

void VideoHppGenerator::readTypeStruct( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "struct" } }, { "name", {} } }, { { "comment", {} }, { "requires", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "member", false } }, { "comment" } );

  StructureData structureData{ .xmlLine = line };

  std::string name, require;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "requires" )
    {
      require = attribute.second;
    }
  }
  assert( !name.empty() );
  checkForError( require.empty() || m_types.contains( require ), line, "struct <" + name + "> requires unknown type <" + require + ">" );
  checkForError( m_types.insert( { name, TypeData{ TypeCategory::Struct, {}, line } } ).second, line, "struct <" + name + "> already specified" );
  assert( !m_structs.contains( name ) );
  std::map<std::string, StructureData>::iterator it = m_structs.insert( std::make_pair( name, structureData ) ).first;

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "member" )
    {
      readStructMember( child, it->second.members );
    }
  }
}

void VideoHppGenerator::readTypesType( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );

  auto categoryIt = attributes.find( "category" );
  if ( categoryIt != attributes.end() )
  {
    if ( categoryIt->second == "define" )
    {
      readTypeDefine( element, attributes );
    }
    else if ( categoryIt->second == "enum" )
    {
      readTypeEnum( element, attributes );
    }
    else if ( categoryIt->second == "include" )
    {
      readTypeInclude( element, attributes );
    }
    else if ( categoryIt->second == "struct" )
    {
      readTypeStruct( element, attributes );
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

void VideoHppGenerator::sortStructs()
{
  for ( auto & ext : m_extensions )
  {
    for ( auto reqIt = ext.requireData.types.begin(); reqIt != ext.requireData.types.end(); ++reqIt )
    {
      std::string name   = *reqIt;
      auto        typeIt = m_types.find( *reqIt );
      if ( ( typeIt != m_types.end() ) && ( typeIt->second.category == TypeCategory::Struct ) )
      {
        auto structIt = m_structs.find( typeIt->first );
        assert( structIt != m_structs.end() );
        for ( auto const & member : structIt->second.members )
        {
          auto memberTypeIt = m_types.find( member.type.type );
          assert( memberTypeIt != m_types.end() );
          if ( ( memberTypeIt->second.category == TypeCategory::Struct ) && ( std::find( ext.requireData.types.begin(), reqIt, member.type.type ) == reqIt ) )
          {
            auto it = std::find( std::next( reqIt ), ext.requireData.types.end(), member.type.type );
            if ( it != ext.requireData.types.end() )
            {
              ext.requireData.types.erase( it );
              reqIt = std::next( ext.requireData.types.insert( reqIt, member.type.type ) );
            }
#if !defined( NDEBUG )
            else
            {
              auto depIt = std::find_if( m_extensions.begin(), m_extensions.end(), [&ext]( ExtensionData const & ed ) { return ed.name == ext.depends; } );
              assert( ( depIt != m_extensions.end() ) && std::any_of( depIt->requireData.types.begin(),
                                                                      depIt->requireData.types.end(),
                                                                      [&member]( std::string const & type ) { return type == member.type.type; } ) );
            }
#endif
          }
        }
      }
    }
  }
}

int main( int argc, char ** argv )
{
  if ( ( argc % 2 ) == 0 )
  {
    std::cout << "VideoHppGenerator usage: VideoHppGenerator [-f filename]" << std::endl;
    std::cout << "\tdefault for filename is <" << VIDEO_SPEC << ">" << std::endl;
    return -1;
  }

  std::string filename = VIDEO_SPEC;
  for ( int i = 1; i < argc; i += 2 )
  {
    if ( strcmp( argv[i], "-f" ) == 0 )
    {
      filename = argv[i + 1];
    }
    else
    {
      std::cout << "unsupported argument <" << argv[i] << ">" << std::endl;
      return -1;
    }
  }

#if defined( CLANG_FORMAT_EXECUTABLE )
  std::cout << "VideoHppGenerator: Found ";
  std::string commandString = "\"" CLANG_FORMAT_EXECUTABLE "\" --version ";
  int         ret           = std::system( commandString.c_str() );
  if ( ret != 0 )
  {
    std::cout << "VideoHppGenerator: failed to determine clang_format version with error <" << ret << ">\n";
  }
#endif

  tinyxml2::XMLDocument doc;
  std::cout << "VideoHppGenerator: Loading " << filename << std::endl;
  tinyxml2::XMLError error = doc.LoadFile( filename.c_str() );
  if ( error != tinyxml2::XML_SUCCESS )
  {
    std::cout << "VideoHppGenerator: failed to load file " << filename << " with error <" << toString( error ) << ">" << std::endl;
    return -1;
  }

  try
  {
    std::cout << "VideoHppGenerator: Parsing " << filename << std::endl;
    VideoHppGenerator generator( doc );

    generator.generateHppFile();

#if !defined( CLANG_FORMAT_EXECUTABLE )
    std::cout << "VideoHppGenerator: could not find clang-format. The generated files will not be formatted accordingly.\n";
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
  return 0;
}
