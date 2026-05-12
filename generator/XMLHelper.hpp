// SPDX-FileCopyrightText: 2023 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <tinyxml2.h>
#include <vector>

struct Type;

enum class MultipleAllowed
{
  No,
  Yes
};

void checkAttributes( std::string const &                                  intro,
                      int                                                  line,
                      std::map<std::string, std::string> const &           attributes,
                      std::map<std::string, std::set<std::string>> const & required,
                      std::map<std::string, std::set<std::string>> const & optional );
void checkElements( std::string const &                               intro,
                    int                                               line,
                    std::vector<tinyxml2::XMLElement const *> const & elements,
                    std::map<std::string, MultipleAllowed> const &    required,
                    std::map<std::string, MultipleAllowed> const &    optional = {} );
void checkForError( std::string const & intro, bool condition, int line, std::string const & message );
void checkForWarning( std::string const & intro, bool condition, int line, std::string const & message );
template <typename T>
bool containsByName( std::vector<T> const & values, std::string const & name );
template <typename T>
bool containsByNameOrAlias( std::map<std::string, T> const & values, std::string const & name );
template <typename T>
typename std::vector<T>::const_iterator findByName( std::vector<T> const & values, std::string const & name );
template <typename T>
typename std::vector<T>::iterator findByName( std::vector<T> & values, std::string const & name );
std::string                       generateCopyrightMessage( std::string const & comment );
void        generateFileFromTemplate( std::string const & fileName, std::string const & snippetFile, std::map<std::string, std::string> const & replacements );
std::string generateStandardArrayWrapper( std::string const & type, std::vector<std::string> const & sizes );
std::map<std::string, std::string> getAttributes( tinyxml2::XMLElement const * element );
template <typename ElementContainer>
std::vector<tinyxml2::XMLElement const *>        getChildElements( ElementContainer const * element );
std::string                                      readComment( std::string const & intro, tinyxml2::XMLElement const * element );
std::pair<std::vector<std::string>, std::string> readModifiers( std::string const & intro, tinyxml2::XMLNode const * node );
std::string                                      readSnippet( std::string const & snippetFile );
Type                                             readType( tinyxml2::XMLElement const * element );
std::string                                      replaceWithMap( std::string const & input, std::map<std::string, std::string> const & replacements );
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

class SyncedMessageHandler
{
public:
  void message( std::string const & m )
  {
    std::lock_guard<std::mutex> guard( m_messageMutex );
    std::cout << m;
  }

private:
  std::mutex m_messageMutex;
};

inline SyncedMessageHandler messager;

struct Type
{
  std::string compose( std::string const & prefixToStrip, std::string const & nameSpace = "" ) const
  {
    return prefix + ( prefix.empty() ? "" : " " ) +
           ( prefixToStrip.empty()
               ? name
               : ( ( ( name.starts_with( prefixToStrip ) && !nameSpace.empty() ) ? ( nameSpace + "::" ) : "" ) + stripPrefix( name, prefixToStrip ) ) ) +
           ( postfix.empty() ? "" : " " ) + postfix;
  }

  bool operator==( Type const & rhs ) const noexcept
  {
    return ( prefix == rhs.prefix ) && ( name == rhs.name ) && ( postfix == rhs.postfix );
  }

  bool operator!=( Type const & rhs ) const noexcept
  {
    return !operator==( rhs );
  }

  bool operator<( Type const & rhs ) const noexcept
  {
    return ( prefix < rhs.prefix ) || ( ( prefix == rhs.prefix ) && ( ( name < rhs.name ) || ( ( name == rhs.name ) && ( postfix < rhs.postfix ) ) ) );
  }

  bool isConstPointer() const noexcept
  {
    return postfix.find( "const *" ) != std::string::npos;
  }

  bool isNonConstPointer() const noexcept
  {
    return isPointer() && postfix.find( "const" ) == std::string::npos;
  }

  bool isPointer() const noexcept
  {
    return postfix.find( '*' ) != std::string::npos;
  }

  bool isValue() const noexcept
  {
    return ( ( prefix.find( '*' ) == std::string::npos ) && ( postfix.find( '*' ) == std::string::npos ) );
  }

  std::string prefix  = {};
  std::string name    = {};
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
// intro      : an intro to any warning or error message
// line       : the line in the xml file where the attributes are listed
// attributes : the map of name/value pairs of the encountered attributes
// required   : the required attributes, with a set of allowed values per attribute
// optional   : the optional attributes, with a set of allowed values per attribute
inline void checkAttributes( std::string const &                                  intro,
                             int                                                  line,
                             std::map<std::string, std::string> const &           attributes,
                             std::map<std::string, std::set<std::string>> const & required,
                             std::map<std::string, std::set<std::string>> const & optional )
{
  // check if all required attributes are included and if there is a set of allowed values, check if the actual
  // value is part of that set
  for ( auto const & r : required )
  {
    auto attributesIt = attributes.find( r.first );
    checkForError( intro, attributesIt != attributes.end(), line, "missing attribute <" + r.first + ">" );
    if ( !r.second.empty() )
    {
      std::vector<std::string> values = tokenize( attributesIt->second, "," );
      for ( auto const & v : values )
      {
        checkForError( intro, r.second.find( v ) != r.second.end(), line, "unexpected attribute value <" + v + "> in attribute <" + attributesIt->first + ">" );
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
        checkForWarning( intro, false, line, "unknown attribute <" + a.first + ">" );
        continue;
      }
      else if ( !optionalIt->second.empty() )
      {
        std::vector<std::string> values = tokenize( a.second, "," );
        for ( auto const & v : values )
        {
          checkForError(
            intro, optionalIt->second.find( v ) != optionalIt->second.end(), line, "unexpected attribute value <" + v + "> in attribute <" + a.first + ">" );
        }
      }
    }
  }
}

inline void checkElements( std::string const &                               intro,
                           int                                               line,
                           std::vector<tinyxml2::XMLElement const *> const & elements,
                           std::map<std::string, MultipleAllowed> const &    required,
                           std::map<std::string, MultipleAllowed> const &    optional )
{
  std::map<std::string, size_t> encountered;
  for ( auto const & e : elements )
  {
    std::string value = e->Value();
    encountered[value]++;
    checkForWarning(
      intro, ( required.find( value ) != required.end() ) || ( optional.find( value ) != optional.end() ), e->GetLineNum(), "unknown element <" + value + ">" );
  }
  for ( auto const & r : required )
  {
    auto encounteredIt = encountered.find( r.first );
    checkForError( intro, encounteredIt != encountered.end(), line, "missing required element <" + r.first + ">" );
    checkForError( intro,
                   ( r.second == MultipleAllowed::Yes ) || ( encounteredIt->second == 1 ),
                   line,
                   "required element <" + r.first + "> is supposed to be listed exactly once, but is listed " + std::to_string( encounteredIt->second ) +
                     " times" );
  }
  for ( auto const & o : optional )
  {
    auto encounteredIt = encountered.find( o.first );
    if ( encounteredIt != encountered.end() )
    {
      checkForError( intro,
                     ( o.second == MultipleAllowed::Yes ) || ( encounteredIt->second == 1 ),
                     line,
                     "optional element <" + o.first + "> is supposed to be listed at most once, but is listed " + std::to_string( encounteredIt->second ) +
                       " times" );
    }
  }
}

inline void checkForError( std::string const & intro, bool condition, int line, std::string const & message )
{
  if ( !condition )
  {
    throw std::runtime_error( intro + ": Spec error on line " + std::to_string( line ) + ": " + message );
  }
}

inline void checkForWarning( std::string const & intro, bool condition, int line, std::string const & message )
{
  if ( !condition )
  {
    std::cerr << intro << ": Spec warning on line " << std::to_string( line ) << ": " << message << "!" << std::endl;
  }
}

template <typename T>
bool containsByName( std::vector<T> const & values, std::string const & name )
{
  return std::ranges::any_of( values, [&name]( T const & value ) { return value.name == name; } );
}

template <typename T>
bool containsByNameOrAlias( std::map<std::string, T> const & values, std::string const & name )
{
  return values.contains( name ) ||
         std::ranges::any_of( values, [&name]( std::pair<std::string, T> const & value ) { return value.second.aliases.contains( name ); } );
}

template <typename T>
typename std::vector<T>::const_iterator findByName( std::vector<T> const & values, std::string const & name )
{
  return std::ranges::find_if( values, [&name]( T const & value ) { return value.name == name; } );
}

template <typename T>
typename std::vector<T>::iterator findByName( std::vector<T> & values, std::string const & name )
{
  return std::ranges::find_if( values, [&name]( T const & value ) { return value.name == name; } );
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

inline void generateFileFromTemplate( std::string const & fileName, std::string const & snippetFile, std::map<std::string, std::string> const & replacements )
{
  std::string const completeFileName = std::string( BASE_PATH ) + "/vulkan/" + fileName;
  messager.message( "VulkanHppGenerator: Generating " + completeFileName + " ...\n" );
  writeToFile( replaceWithMap( readSnippet( snippetFile ), replacements ), completeFileName );
}

inline std::string generateStandardArrayWrapper( std::string const & type, std::vector<std::string> const & sizes )
{
  std::string arrayString = "ArrayWrapper" + std::to_string( sizes.size() ) + "D<" + type;
  for ( auto const & size : sizes )
  {
    arrayString += ", " + size;
  }
  arrayString += ">";
  return arrayString;
}

inline std::map<std::string, std::string> getAttributes( tinyxml2::XMLElement const * element )
{
  assert( element );
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

inline bool isHexNumber( std::string const & name ) noexcept
{
  return name.starts_with( "0x" ) && ( name.find_first_not_of( "0123456789ABCDEF", 2 ) == std::string::npos );
}

inline bool isNumber( std::string const & name ) noexcept
{
  return name.find_first_not_of( "0123456789" ) == std::string::npos;
}

inline std::string readComment( std::string const & intro, tinyxml2::XMLElement const * element )
{
  int const line = element->GetLineNum();
  checkAttributes( intro, line, getAttributes( element ), {}, {} );
  checkElements( intro, line, getChildElements( element ), {} );
  return element->GetText();
}

inline std::pair<std::vector<std::string>, std::string> readModifiers( std::string const & intro, tinyxml2::XMLNode const * node )
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
        std::string::size_type const startPos = value.find( '[', endPos );
        checkForError( intro, startPos != std::string::npos, node->GetLineNum(), "could not find '[' in <" + value + ">" );
        endPos = value.find( ']', startPos );
        checkForError( intro, endPos != std::string::npos, node->GetLineNum(), "could not find ']' in <" + value + ">" );
        checkForError( intro, startPos + 2 <= endPos, node->GetLineNum(), "missing content between '[' and ']' in <" + value + ">" );
        arraySizes.push_back( value.substr( startPos + 1, endPos - startPos - 1 ) );
      }
    }
    else if ( value[0] == ':' )
    {
      bitCount = trim( value.substr( 1 ) );
    }
    else
    {
      checkForError( intro, ( value[0] == ';' ) || ( value[0] == ')' ), node->GetLineNum(), "unknown modifier <" + value + ">" );
    }
  }
  return { arraySizes, bitCount };
}

inline std::string readSnippet( std::string const & snippetFile )
{
  std::ifstream ifs( std::string( BASE_PATH ) + "/generator/snippets/" + snippetFile );
  assert( !ifs.fail() );
  // skip the snippet license header (3 loc)
  std::string line;
  for ( uint32_t i = 0; i < 3; i++ )
  {
    std::getline(ifs, line);
  }
  // return the remainder
  std::ostringstream oss;
  oss << ifs.rdbuf();
  return oss.str();
}

inline Type readType( tinyxml2::XMLElement const * element )
{
  Type                      type;
  tinyxml2::XMLNode const * previousSibling = element->PreviousSibling();
  if ( previousSibling && previousSibling->ToText() )
  {
    type.prefix = trim( previousSibling->Value() );
  }
  type.name                             = element->GetText();
  tinyxml2::XMLNode const * nextSibling = element->NextSibling();
  if ( nextSibling && nextSibling->ToText() )
  {
    type.postfix = trimStars( trimEnd( nextSibling->Value() ) );
  }
  if ( type.prefix.starts_with( "const" ) )
  {
    type.prefix  = trim( type.prefix.substr( 5 ) );
    type.postfix = ( type.postfix.empty() ? "const" : ( "const " + type.postfix ) );
  }
  return type;
}

inline std::string replaceWithMap( std::string const & input, std::map<std::string, std::string> const & replacements )
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
  assert( value.empty() || isupper( value[0] ) || isdigit( value[0] ) );
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
    case tinyxml2::XML_SUCCESS                       : return "XML_SUCCESS";
    case tinyxml2::XML_NO_ATTRIBUTE                  : return "XML_NO_ATTRIBUTE";
    case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE          : return "XML_WRONG_ATTRIBUTE_TYPE";
    case tinyxml2::XML_ERROR_FILE_NOT_FOUND          : return "XML_ERROR_FILE_NOT_FOUND";
    case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED: return "XML_ERROR_FILE_COULD_NOT_BE_OPENED";
    case tinyxml2::XML_ERROR_FILE_READ_ERROR         : return "XML_ERROR_FILE_READ_ERROR";
    case tinyxml2::XML_ERROR_PARSING_ELEMENT         : return "XML_ERROR_PARSING_ELEMENT";
    case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE       : return "XML_ERROR_PARSING_ATTRIBUTE";
    case tinyxml2::XML_ERROR_PARSING_TEXT            : return "XML_ERROR_PARSING_TEXT";
    case tinyxml2::XML_ERROR_PARSING_CDATA           : return "XML_ERROR_PARSING_CDATA";
    case tinyxml2::XML_ERROR_PARSING_COMMENT         : return "XML_ERROR_PARSING_COMMENT";
    case tinyxml2::XML_ERROR_PARSING_DECLARATION     : return "XML_ERROR_PARSING_DECLARATION";
    case tinyxml2::XML_ERROR_PARSING_UNKNOWN         : return "XML_ERROR_PARSING_UNKNOWN";
    case tinyxml2::XML_ERROR_EMPTY_DOCUMENT          : return "XML_ERROR_EMPTY_DOCUMENT";
    case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT      : return "XML_ERROR_MISMATCHED_ELEMENT";
    case tinyxml2::XML_ERROR_PARSING                 : return "XML_ERROR_PARSING";
    case tinyxml2::XML_CAN_NOT_CONVERT_TEXT          : return "XML_CAN_NOT_CONVERT_TEXT";
    case tinyxml2::XML_NO_TEXT_NODE                  : return "XML_NO_TEXT_NODE";
    default                                          : return "unknown error code <" + std::to_string( error ) + ">";
  }
}

inline std::string toUpperCase( std::string const & name )
{
  assert( !name.empty() );
  std::string convertedName;
  convertedName.push_back( static_cast<char>( toupper( name.front() ) ) );
  size_t n = name.length();
  for ( size_t i = 1; i < n; ++i )
  {
    if ( ( isupper( name[i] ) && ( islower( name[i - 1] ) || isdigit( name[i - 1] ) || ( ( i < n - 1 ) && islower( name[i + 1] ) ) ) ) ||
         ( isdigit( name[i] ) && islower( name[i - 1] ) ) )
    {
      convertedName.push_back( '_' );
    }
    convertedName.push_back( static_cast<char>( toupper( name[i] ) ) );
  }
  return convertedName;
}

inline std::string trim( std::string const & input )
{
  std::string result = input;
  result.erase( result.begin(), std::ranges::find_if( result, []( char c ) noexcept { return !std::isspace( c ); } ) );
  result.erase( std::find_if( result.rbegin(), result.rend(), []( char c ) noexcept { return !std::isspace( c ); } ).base(), result.end() );
  return result;
}

inline std::string trimEnd( std::string const & input )
{
  std::string result = input;
  result.erase( std::find_if( result.rbegin(), result.rend(), []( char c ) noexcept { return !std::isspace( c ); } ).base(), result.end() );
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

inline void writeToFile( std::string const & str, std::string const & fileName )
{
  std::ofstream ofs( fileName );
  assert( !ofs.fail() );
  ofs << str;
  ofs.close();

#if defined( CLANG_FORMAT_EXECUTABLE )
  // explicitly flush std::cout, as std::system spawns a sub-process
  std::cout.flush();

  messager.message( "VulkanHppGenerator: Formatting " + fileName + " ...\n" );
  std::string const commandString = "\"" CLANG_FORMAT_EXECUTABLE "\" -i --style=file " + fileName;
  int const         ret           = std::system( commandString.c_str() );
  if ( ret != 0 )
  {
    throw std::runtime_error( "VulkanHppGenerator: failed to format file " + fileName + " with error <" + std::to_string( ret ) + ">" );
  }
#endif
}
