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

#pragma once

#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <tinyxml2.h>
#include <vector>

struct TypeInfo;

void                               checkAttributes( int                                                  line,
                                                    std::map<std::string, std::string> const &           attributes,
                                                    std::map<std::string, std::set<std::string>> const & required,
                                                    std::map<std::string, std::set<std::string>> const & optional );
void                               checkElements( int                                               line,
                                                  std::vector<tinyxml2::XMLElement const *> const & elements,
                                                  std::map<std::string, bool> const &               required,
                                                  std::set<std::string> const &                     optional = {} );
void                               checkForError( bool condition, int line, std::string const & message );
void                               checkForWarning( bool condition, int line, std::string const & message );
std::string                        generateCopyrightMessage( std::string const & comment );
std::string                        generateStandardArrayWrapper( std::string const & type, std::vector<std::string> const & sizes );
std::map<std::string, std::string> getAttributes( tinyxml2::XMLElement const * element );
template <typename ElementContainer>
std::vector<tinyxml2::XMLElement const *>        getChildElements( ElementContainer const * element );
std::string                                      readComment( tinyxml2::XMLElement const * element );
std::pair<std::vector<std::string>, std::string> readModifiers( tinyxml2::XMLNode const * node );
TypeInfo                                         readTypeInfo( tinyxml2::XMLElement const * element );
std::string                                      replaceWithMap( std::string const & input, std::map<std::string, std::string> replacements );
std::string                                      stripPostfix( std::string const & value, std::string const & postfix );
std::string                                      stripPrefix( std::string const & value, std::string const & prefix );
std::string                                      toCamelCase( std::string const & value, bool keepSeparatedNumbersSeparated = false );
std::vector<std::string>                         tokenize( std::string const & tokenString, std::string const & separator );
std::string                                      toString( tinyxml2::XMLError error );
std::string                                      toUpperCase( std::string const & name );
std::string                                      trim( std::string const & input );
std::string                                      trimEnd( std::string const & input );
std::string                                      trimStars( std::string const & input );
void                                             writeToFile( std::string const & str, std::string const & fileName );

struct TypeInfo
{
  std::string compose( std::string const & nameSpace, std::string const & prefix_ = "Vk" ) const
  {
    return prefix + ( prefix.empty() ? "" : " " ) +
           ( nameSpace.empty() ? type : ( ( ( type.starts_with( prefix_ ) ) ? ( nameSpace + "::" ) : "" ) + stripPrefix( type, prefix_ ) ) ) +
           ( postfix.empty() ? "" : " " ) + postfix;
  }

  bool operator==( TypeInfo const & rhs ) const
  {
    return ( prefix == rhs.prefix ) && ( type == rhs.type ) && ( postfix == rhs.postfix );
  }

  bool operator!=( TypeInfo const & rhs ) const
  {
    return !operator==( rhs );
  }

  bool operator<( TypeInfo const & rhs ) const
  {
    return ( prefix < rhs.prefix ) || ( ( prefix == rhs.prefix ) && ( ( type < rhs.type ) || ( ( type == rhs.type ) && ( postfix < rhs.postfix ) ) ) );
  }

  bool isConstPointer() const
  {
    return ( prefix.find( "const" ) != std::string::npos ) && ( postfix.find( '*' ) != std::string::npos );
  }

  bool isNonConstPointer() const
  {
    return ( prefix.find( "const" ) == std::string::npos ) && ( postfix.find( '*' ) != std::string::npos );
  }

  bool isValue() const
  {
    return ( ( prefix.find( '*' ) == std::string::npos ) && ( postfix.find( '*' ) == std::string::npos ) );
  }

  std::string prefix  = {};
  std::string type    = {};
  std::string postfix = {};
};

struct ExternalTypeData
{
  std::string require = {};
  int         xmlLine = 0;
};

struct IncludeData
{
  int xmlLine = {};
};

enum class TypeCategory
{
  Bitmask,
  BaseType,
  Constant,
  Define,
  Enum,
  ExternalType,
  FuncPointer,
  Handle,
  Include,
  Struct,
  Union,
  Unknown
};

struct TypeData
{
  TypeCategory          category   = TypeCategory::Unknown;
  std::set<std::string> requiredBy = {};
  int                   xmlLine    = {};
};

// check the validity of an attributes map
// line       : the line in the xml file where the attributes are listed
// attributes : the map of name/value pairs of the encountered attributes
// required   : the required attributes, with a set of allowed values per attribute
// optional   : the optional attributes, with a set of allowed values per attribute
inline void checkAttributes( int                                                  line,
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
    if ( !r.second.empty() )
    {
      std::vector<std::string> values = tokenize( attributesIt->second, "," );
      for ( auto const & v : values )
      {
        checkForError( r.second.find( v ) != r.second.end(), line, "unexpected attribute value <" + v + "> in attribute <" + attributesIt->first + ">" );
      }
    }
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

inline void checkElements( int                                               line,
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

inline void checkForError( bool condition, int line, std::string const & message )
{
  if ( !condition )
  {
    throw std::runtime_error( "VulkanHppGenerator: Spec error on line " + std::to_string( line ) + ": " + message );
  }
}

inline void checkForWarning( bool condition, int line, std::string const & message )
{
  if ( !condition )
  {
    std::cerr << "VulkanHppGenerator: Spec warning on line " << std::to_string( line ) << ": " << message << "!" << std::endl;
  }
}

inline std::string generateCopyrightMessage( std::string const & comment )
{
  std::string copyrightMessage = comment;

  // replace any '\n' with "\n// "
  for ( size_t pos = copyrightMessage.find( '\n' ); pos != std::string::npos; pos = copyrightMessage.find( '\n', pos + 1 ) )
  {
    copyrightMessage.replace( pos, 1, "\n// " );
  }
  // remove any trailing spaces
  copyrightMessage = trimEnd( copyrightMessage );

  // and add a little message on our own
  copyrightMessage += "\n\n// This header is generated from the Khronos Vulkan XML API Registry.";
  return trim( copyrightMessage ) + "\n";
}

inline std::string generateStandardArrayWrapper( std::string const & type, std::vector<std::string> const & sizes )
{
  std::string arrayString = "VULKAN_HPP_NAMESPACE::ArrayWrapper" + std::to_string( sizes.size() ) + "D<" + type;
  for ( auto const & size : sizes )
  {
    arrayString += ", " + size;
  }
  arrayString += ">";
  return arrayString;
}

inline std::map<std::string, std::string> getAttributes( tinyxml2::XMLElement const * element )
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
inline std::vector<tinyxml2::XMLElement const *> getChildElements( ElementContainer const * element )
{
  std::vector<tinyxml2::XMLElement const *> childElements;
  for ( tinyxml2::XMLElement const * childElement = element->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement() )
  {
    childElements.push_back( childElement );
  }
  return childElements;
}

inline bool isHexNumber( std::string const & name )
{
  return name.starts_with( "0x" ) && ( name.find_first_not_of( "0123456789ABCDEF", 2 ) == std::string::npos );
}

inline bool isNumber( std::string const & name )
{
  return name.find_first_not_of( "0123456789" ) == std::string::npos;
}

inline std::string readComment( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  checkElements( line, getChildElements( element ), {} );
  return element->GetText();
}

inline std::pair<std::vector<std::string>, std::string> readModifiers( tinyxml2::XMLNode const * node )
{
  std::vector<std::string> arraySizes;
  std::string              bitCount;
  if ( node && node->ToText() )
  {
    // following the name there might be some array size
    std::string value = trim( node->Value() );
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
      bitCount = trim( value.substr( 1 ) );
    }
    else
    {
      checkForError( ( value[0] == ';' ) || ( value[0] == ')' ), node->GetLineNum(), "unknown modifier <" + value + ">" );
    }
  }
  return std::make_pair( arraySizes, bitCount );
}

inline TypeInfo readTypeInfo( tinyxml2::XMLElement const * element )
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

inline std::string replaceWithMap( std::string const & input, std::map<std::string, std::string> replacements )
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

inline std::string stripPostfix( std::string const & value, std::string const & postfix )
{
  std::string strippedValue = value;
  if ( strippedValue.ends_with( postfix ) )
  {
    strippedValue.erase( strippedValue.length() - postfix.length() );
  }
  return strippedValue;
}

inline std::string stripPrefix( std::string const & value, std::string const & prefix )
{
  std::string strippedValue = value;
  if ( strippedValue.starts_with( prefix ) )
  {
    strippedValue.erase( 0, prefix.length() );
  }
  return strippedValue;
}

inline std::string toCamelCase( std::string const & value, bool keepSeparatedNumbersSeparated )
{
  assert( !value.empty() && ( isupper( value[0] ) || isdigit( value[0] ) ) );
  std::string result;
  result.reserve( value.size() );
  for ( size_t i = 0; i < value.size(); ++i )
  {
    if ( value[i] == '_' )
    {
      if ( keepSeparatedNumbersSeparated && ( 0 < i ) && isdigit( value[i - 1] ) && ( i < value.size() - 1 ) && isdigit( value[i + 1] ) )
      {
        result.push_back( '_' );
      }
    }
    else
    {
      result.push_back( ( ( 0 == i ) || ( value[i - 1] == '_' ) || isdigit( value[i - 1] ) ) ? value[i] : static_cast<char>( tolower( value[i] ) ) );
    }
  }
#if 0
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
#endif
  return result;
}

inline std::vector<std::string> tokenize( std::string const & tokenString, std::string const & separator )
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

inline std::string toString( tinyxml2::XMLError error )
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

inline std::string trim( std::string const & input )
{
  std::string result = input;
  result.erase( result.begin(), std::find_if( result.begin(), result.end(), []( char c ) { return !std::isspace( c ); } ) );
  result.erase( std::find_if( result.rbegin(), result.rend(), []( char c ) { return !std::isspace( c ); } ).base(), result.end() );
  return result;
}

inline std::string trimEnd( std::string const & input )
{
  std::string result = input;
  result.erase( std::find_if( result.rbegin(), result.rend(), []( char c ) { return !std::isspace( c ); } ).base(), result.end() );
  return result;
}

inline std::string trimStars( std::string const & input )
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
