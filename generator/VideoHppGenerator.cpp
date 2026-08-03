// SPDX-FileCopyrightText: 2023-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

#include "VideoHppGenerator.hpp"

#include "XMLHelper.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

VideoHppGenerator::VideoHppGenerator( VideoXML && videoXML ) : m_videoXML( std::move( videoXML ) )
{
  for ( auto const & define : m_videoXML.types.defines )
  {
    checkForError( m_types.insert( { define.name, TypeData{ TypeCategory::Define, {}, define.xmlLine } } ).second,
                   define.xmlLine,
                   "define <" + define.name + "> already specified" );
  }
  for ( auto const & e : m_videoXML.types.enums )
  {
    checkForError( m_types.insert( { e.name, TypeData{ TypeCategory::Enum, {}, e.xmlLine } } ).second, e.xmlLine, "enum <" + e.name + "> already specified" );
    m_enums[e.name] = { .xmlLine = e.xmlLine };
  }
  for ( auto const & externalType : m_videoXML.types.externals )
  {
    checkForError( m_types.insert( { externalType.name, TypeData{ TypeCategory::External, {}, externalType.xmlLine } } ).second,
                   externalType.xmlLine,
                   "external type <" + externalType.name + "> already specified" );
  }
  for ( auto const & include : m_videoXML.types.includes )
  {
    checkForError( m_types.insert( { include.name, TypeData{ TypeCategory::Include, {}, include.xmlLine } } ).second,
                   include.xmlLine,
                   "type <" + include.name + "> already specified" );
  }
  for ( auto const & structure : m_videoXML.types.structs )
  {
    checkForError( m_types.insert( { structure.name, TypeData{ TypeCategory::Struct, {}, structure.xmlLine } } ).second,
                   structure.xmlLine,
                   "struct <" + structure.name + "> already specified" );
  }
  for ( auto const & e : m_videoXML.enums )
  {
    auto enumIt = m_enums.find( e.name );
    checkForError( enumIt != m_enums.end(), e.xmlLine, "enum <" + e.name + "> is not listed as enum in the types section" );
    checkForError( enumIt->second.values.empty(), e.xmlLine, "enum <" + e.name + "> already holds values" );
    for ( auto const & value : e.values )
    {
      if ( value.alias.empty() )
      {
        checkForError(
          !containsByName( enumIt->second.values, value.name ), value.xmlLine, "enum value <" + value.name + "> already part of enum <" + e.name + ">" );
        enumIt->second.values.push_back( { {}, value.name, value.value, value.xmlLine } );
      }
      else
      {
        auto valueIt = findByName( enumIt->second.values, value.alias );
        checkForError( valueIt != enumIt->second.values.end(), value.xmlLine, "enum value <" + value.name + "> uses unknown alias <" + value.alias + ">" );
        checkForError( std::ranges::find_if( valueIt->aliases, [&name = value.name]( auto const & alias ) { return alias.first == name; } ) ==
                         valueIt->aliases.end(),
                       value.xmlLine,
                       "enum alias <" + value.name + "> already listed for enum value <" + value.alias + ">" );
        valueIt->aliases.push_back( { value.name, value.xmlLine } );
      }
    }
  }
  for ( auto const & extension : m_videoXML.extensions )
  {
    for ( auto const & type : extension.require.types )
    {
      auto typeIt = m_types.find( type.name );
      checkForError( typeIt != m_types.end(), type.xmlLine, "unknown required type <" + type.name + ">" );
      typeIt->second.requiredBy.insert( extension.name );
    }

    ExtensionData extensionData{
      .depends = extension.require.include.name.empty() ? "" : stripPrefix( stripPostfix( extension.require.include.name, ".h" ), "vk_video/" ),
      .name    = extension.name,
      .number  = extension.number,
      .protect = extension.protect,
      .xmlLine = extension.xmlLine,
    };
    for ( auto const & constant : extension.require.enums )
    {
      if ( !constant.type.empty() )
      {
        extensionData.requireData.constants[constant.name] = { .type = constant.type, .value = constant.value, .xmlLine = constant.xmlLine };
      }
    }
    for ( auto const & type : extension.require.types )
    {
      extensionData.requireData.types.push_back( type.name );
    }
    extensionData.requireData.xmlLine = extension.require.xmlLine;

    m_extensions.push_back( std::move( extensionData ) );
  }

  addImplicitlyRequiredTypes();
  sortStructs();
  checkCorrectness();
}

void VideoHppGenerator::generateCppmFile() const
{
  generateFileFromTemplate(
    "vulkan_video.cppm", "VideoCppmTemplate.hpp", { { "copyrightMessage", m_videoXML.copyrightMessage }, { "includes", generateIncludes() } } );
}

void VideoHppGenerator::generateHppFile() const
{
  generateFileFromTemplate( "vulkan_video.hpp",
                            "VideoHppTemplate.hpp",
                            { { "constants", generateConstants() },
                              { "copyrightMessage", m_videoXML.copyrightMessage },
                              { "enums", generateEnums() },
                              { "includes", generateIncludes() },
                              { "structs", generateStructs() } } );
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
  auto structIt = findByName( m_videoXML.types.structs, typeIt->first );
  assert( structIt != m_videoXML.types.structs.end() );
  for ( auto const & member : structIt->members )
  {
    auto memberTypeIt = m_types.find( member.type.name );
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
  for ( auto const & structure : m_videoXML.types.structs )
  {
    // check that a struct is referenced somewhere
    // I think, it's not forbidden to not reference a struct, but it would probably be not intended?
    auto typeIt = m_types.find( structure.name );
    assert( typeIt != m_types.end() );
    checkForError( !typeIt->second.requiredBy.empty(), structure.xmlLine, "structure <" + structure.name + "> not required by any extension" );

    assert( typeIt->second.requiredBy.size() == 1 );
    auto extIt = std::ranges::find_if( m_extensions, [&typeIt]( ExtensionData const & ed ) { return ed.name == *typeIt->second.requiredBy.begin(); } );
    assert( extIt != m_extensions.end() );

    // checks on the members of a struct
    for ( auto const & member : structure.members )
    {
      // check that each member type is known
      checkForError( m_types.contains( member.type.name ), member.xmlLine, "struct member uses unknown type <" + member.type.name + ">" );

      // check that all member types are required in some extension (it's just a warning!!)
      if ( member.type.name.starts_with( "StdVideo" ) )
      {
        auto memberTypeIt = m_types.find( member.type.name );
        assert( memberTypeIt != m_types.end() );
        checkForWarning( !memberTypeIt->second.requiredBy.empty(),
                         member.xmlLine,
                         "struct member type <" + member.type.name + "> used in struct <" + structure.name + "> is never required for any extension" );
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
            auto depIt = std::ranges::find_if( m_extensions, [&extIt]( ExtensionData const & ed ) { return ed.name == extIt->depends; } );
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

void VideoHppGenerator::checkForError( bool condition, int line, std::string const & message ) const
{
  ::checkForError( "VideoHppGenerator", condition, line, message );
}

void VideoHppGenerator::checkForWarning( bool condition, int line, std::string const & message ) const
{
  ::checkForWarning( "VideoHppGenerator", condition, line, message );
}

std::string VideoHppGenerator::generateConstants() const
{
  {
    std::string const enumsTemplate = R"(
  //=================
  //=== CONSTANTs ===
  //=================

${constants}
)";

    std::string constants;
    for ( auto const & extension : m_extensions )
    {
      constants += generateConstants( extension );
    }

    return replaceWithMap( enumsTemplate, { { "constants", constants } } );
  }
}

std::string VideoHppGenerator::generateConstants( ExtensionData const & extensionData ) const
{
  std::string str;
  for ( auto const & constant : extensionData.requireData.constants )
  {
    str += "VULKAN_HPP_CONSTEXPR_INLINE " + constant.second.type + " " + toCamelCase( stripPrefix( constant.first, "STD_VIDEO_" ), true ) + " = " +
           constant.second.value + ";\n";
  }
  if ( !str.empty() )
  {
    str = "\n#if defined( " + extensionData.protect + " )\n  //=== " + extensionData.name + " ===\n" + str + "#endif\n";
  }
  return str;
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

    for ( auto const & alias : value.aliases )
    {
      std::string aliasName = "e" + toCamelCase( stripPrefix( alias.first, prefix ), true );
      assert( valueToNameMap.insert( { aliasName, alias.first } ).second );
      enumValues += "    " + aliasName + " VULKAN_HPP_DEPRECATED_17( \"" + aliasName + " is deprecated, " + valueName +
                    " should be used instead.\" ) = " + alias.first + ",\n";
    }
  }

  if ( !enumValues.empty() )
  {
    size_t pos = enumValues.rfind( ',' );
    assert( pos != std::string::npos );
    enumValues.erase( pos, 1 );
    enumValues = "\n" + enumValues + "  ";
  }

  std::string const enumTemplate = R"(  enum class ${enumName}
  {${enumValues}};
)";

  return replaceWithMap( enumTemplate, { { "enumName", stripPrefix( enumData.first, "StdVideo" ) }, { "enumValues", enumValues } } );
}

std::string VideoHppGenerator::generateEnums() const
{
  {
    std::string const enumsTemplate = R"(
  //=============
  //=== ENUMs ===
  //=============

${enums}
)";

    std::string enums;
    for ( auto const & extension : m_extensions )
    {
      enums += generateEnums( extension );
    }

    return replaceWithMap( enumsTemplate, { { "enums", enums } } );
  }
}

std::string VideoHppGenerator::generateEnums( ExtensionData const & extensionData ) const
{
  std::string str;
  for ( auto const & type : extensionData.requireData.types )
  {
    auto enumIt = m_enums.find( type );
    if ( enumIt != m_enums.end() )
    {
      str += "\n" + generateEnum( *enumIt );
    }
  }
  if ( !str.empty() )
  {
    str = "\n#if defined( " + extensionData.protect + " )\n  //=== " + extensionData.name + " ===\n" + str + "#endif\n";
  }
  return str;
}

std::string VideoHppGenerator::generateIncludes() const
{
  std::string includes;
  for ( auto const & extension : m_extensions )
  {
    std::string include = "<vk_video/" + extension.name + ".h>";
    includes += "#if __has_include( " + include + " )\n";
    includes += "#  include <vk_video/" + extension.name + ".h>\n";
    includes += "#endif\n";
  }

  return includes;
}

std::string VideoHppGenerator::generateStruct( CategoryStruct const & categoryStruct ) const
{
  static std::string const structureTemplate = R"(  struct ${structureType}
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

    operator StdVideo${structureType} const *() const VULKAN_HPP_NOEXCEPT
    {
      return reinterpret_cast<const StdVideo${structureType}*>( this );
    }

    operator StdVideo${structureType} *() VULKAN_HPP_NOEXCEPT
    {
      return reinterpret_cast<StdVideo${structureType}*>( this );
    }
${compareOperators}
    public:
${members}
  };
)";

  return replaceWithMap( structureTemplate,
                         { { "compareOperators", generateStructCompareOperators( categoryStruct ) },
                           { "members", generateStructMembers( categoryStruct ) },
                           { "structureType", stripPrefix( categoryStruct.name, "StdVideo" ) } } );
}

std::string VideoHppGenerator::generateStructCompareOperators( CategoryStruct const & categoryStruct ) const
{
  static std::set<std::string> const simpleTypes = { "char",   "double",  "DWORD",    "float",    "HANDLE",  "HINSTANCE", "HMONITOR",
                                                     "HWND",   "int",     "int8_t",   "int16_t",  "int32_t", "int64_t",   "LPCWSTR",
                                                     "size_t", "uint8_t", "uint16_t", "uint32_t", "uint64_t" };

  // two structs are compared by comparing each of the elements
  std::string compareMembers;
  std::string intro = "";
  for ( size_t i = 0; i < categoryStruct.members.size(); i++ )
  {
    StructMember const & member = categoryStruct.members[i];
    auto                 typeIt = m_types.find( member.type.name );
    assert( typeIt != m_types.end() );
    if ( ( typeIt->second.category == TypeCategory::External ) && member.type.postfix.empty() && !simpleTypes.contains( member.type.name ) )
    {
      // this type might support operator==() or operator<=>()... that is, use memcmp
      compareMembers += intro + "( memcmp( &" + member.name + ", &rhs." + member.name + ", sizeof( " + member.type.name + " ) ) == 0 )";
    }
    else
    {
      assert( member.type.name != "char" );
      // for all others, we use the operator== of that type
      compareMembers += intro + "( " + member.name + " == rhs." + member.name + " )";
    }
    intro = "\n          && ";
  }

  static std::string const compareTemplate = R"(
    bool operator==( ${name} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return ${compareMembers};
    }

    bool operator!=( ${name} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return !operator==( rhs );
    }
)";

  return replaceWithMap( compareTemplate, { { "name", stripPrefix( categoryStruct.name, "StdVideo" ) }, { "compareMembers", compareMembers } } );
}

std::string VideoHppGenerator::generateStructMembers( CategoryStruct const & categoryStruct ) const
{
  std::string members;
  for ( auto const & member : categoryStruct.members )
  {
    members += "    ";
    std::string type;
    if ( !member.bitCount.empty() && member.type.name.starts_with( "StdVideo" ) )
    {
      assert( member.type.prefix.empty() && member.type.postfix.empty() );  // never encounterd a different case
      type = member.type.name;
    }
    else if ( member.arraySizes.empty() )
    {
      type = member.type.compose( "StdVideo", "VULKAN_HPP_NAMESPACE::VULKAN_HPP_VIDEO_NAMESPACE" );
    }
    else
    {
      assert( member.type.prefix.empty() && member.type.postfix.empty() );
      type = generateStandardArrayWrapper( member.type.compose( "" ), member.arraySizes );
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
      auto enumIt = m_enums.find( member.type.name );
      if ( member.arraySizes.empty() && ( enumIt != m_enums.end() ) && member.type.postfix.empty() )
      {
        assert( member.type.prefix.empty() && member.arraySizes.empty() && !enumIt->second.values.empty() );

        std::string prefix    = toUpperCase( member.type.name ) + "_";
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
  std::string const structsTemplate = R"(
  //===============
  //=== STRUCTS ===
  //===============

${structs}
)";

  std::string structs;
  for ( auto const & extension : m_extensions )
  {
    structs += generateStructs( extension );
  }
  return replaceWithMap( structsTemplate, { { "structs", structs } } );
}

std::string VideoHppGenerator::generateStructs( ExtensionData const & extensionData ) const
{
  std::string str;
  for ( auto const & type : extensionData.requireData.types )
  {
    auto structIt = findByName( m_videoXML.types.structs, type );
    if ( structIt != m_videoXML.types.structs.end() )
    {
      str += "\n" + generateStruct( *structIt );
    }
  }
  if ( !str.empty() )
  {
    str = "\n#if defined( " + extensionData.protect + " )\n  //=== " + extensionData.name + " ===\n" + str + "#endif\n";
  }
  return str;
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
        auto structIt = findByName( m_videoXML.types.structs, typeIt->first );
        assert( structIt != m_videoXML.types.structs.end() );
        for ( auto const & member : structIt->members )
        {
          auto memberTypeIt = m_types.find( member.type.name );
          assert( memberTypeIt != m_types.end() );
          if ( ( memberTypeIt->second.category == TypeCategory::Struct ) && ( std::find( ext.requireData.types.begin(), reqIt, member.type.name ) == reqIt ) )
          {
            auto it = std::find( std::next( reqIt ), ext.requireData.types.end(), member.type.name );
            if ( it != ext.requireData.types.end() )
            {
              ext.requireData.types.erase( it );
              reqIt = std::next( ext.requireData.types.insert( reqIt, member.type.name ) );
            }
#if !defined( NDEBUG )
            else
            {
              auto depIt = std::ranges::find_if( m_extensions, [&ext]( ExtensionData const & ed ) { return ed.name == ext.depends; } );
              assert( ( depIt != m_extensions.end() ) &&
                      std::ranges::any_of( depIt->requireData.types, [&member]( std::string const & type ) { return type == member.type.name; } ) );
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
    VideoHppGenerator generator( parseVideoXML( doc ) );

    generator.generateCppmFile();
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
