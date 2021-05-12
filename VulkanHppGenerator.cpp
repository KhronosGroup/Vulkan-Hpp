// Copyright(c) 2015-2020, NVIDIA CORPORATION. All rights reserved.
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

#include "VulkanHppGenerator.hpp"

#include <cassert>
#include <fstream>
#include <regex>

bool             beginsWith( std::string const & text, std::string const & prefix );
bool             endsWith( std::string const & text, std::string const & postfix );
void             check( bool condition, int line, std::string const & message );
void             checkAttributes( int                                                  line,
                                  std::map<std::string, std::string> const &           attributes,
                                  std::map<std::string, std::set<std::string>> const & required,
                                  std::map<std::string, std::set<std::string>> const & optional );
void             checkElements( int                                               line,
                                std::vector<tinyxml2::XMLElement const *> const & elements,
                                std::map<std::string, bool> const &               required,
                                std::set<std::string> const &                     optional = {} );
std::string      constructStandardArray( std::string const & type, std::vector<std::string> const & sizes );
std::string      createEnumValueName( std::string const & name,
                                      std::string const & prefix,
                                      std::string const & postfix,
                                      bool                bitmask,
                                      std::string const & tag );
std::string      createSuccessCode( std::string const & code, std::set<std::string> const & tags );
std::string      determineCommandName( std::string const &           vulkanCommandName,
                                       std::string const &           argumentType,
                                       std::set<std::string> const & tags );
std::string      determineNoDiscard( bool multiSuccessCodes, bool multiErrorCodes );
std::set<size_t> determineSingularParams( size_t                           returnParamIndex,
                                          std::map<size_t, size_t> const & vectorParamIndices );
std::set<size_t> determineSkippedParams( size_t returnParamIndex, std::map<size_t, size_t> const & vectorParamIndices );
std::string findTag( std::set<std::string> const & tags, std::string const & name, std::string const & postfix = "" );
std::map<std::string, std::string> getAttributes( tinyxml2::XMLElement const * element );
template <typename ElementContainer>
std::vector<tinyxml2::XMLElement const *> getChildElements( ElementContainer const * element );
std::string getEnumPostfix( std::string const & name, std::set<std::string> const & tags, std::string & prefix );
std::string namespacedType( std::string const & type );
std::string readTypePostfix( tinyxml2::XMLNode const * node );
std::string readTypePrefix( tinyxml2::XMLNode const * node );
void        replaceAll( std::string & str, std::string const & from, std::string const & to );
std::string replaceWithMap( std::string const & input, std::map<std::string, std::string> replacements );
std::string startLowerCase( std::string const & input );
std::string startUpperCase( std::string const & input );
std::string stripPostfix( std::string const & value, std::string const & postfix );
std::string stripPluralS( std::string const & name );
std::string stripPrefix( std::string const & value, std::string const & prefix );
std::string toCamelCase( std::string const & value );
std::string toUpperCase( std::string const & name );
std::vector<std::string> tokenize( std::string const & tokenString, std::string const & separator );
std::string              trim( std::string const & input );
std::string              trimEnd( std::string const & input );
std::string              trimStars( std::string const & input );
void                     warn( bool condition, int line, std::string const & message );

#if defined( NDEBUG )
template <typename T>
void unreferenced( T const & t )
{
  t;
};
#endif

const std::set<std::string> ignoreLens          = { "null-terminated",
                                           R"(latexmath:[\lceil{\mathit{rasterizationSamples} \over 32}\rceil])",
                                           "2*VK_UUID_SIZE",
                                           "2*ename:VK_UUID_SIZE" };
const std::set<std::string> specialPointerTypes = {
  "Display", "IDirectFB", "wl_display", "xcb_connection_t", "_screen_window"
};

bool beginsWith( std::string const & text, std::string const & prefix )
{
  return prefix.empty() || text.substr( 0, prefix.length() ) == prefix;
}

bool endsWith( std::string const & text, std::string const & postfix )
{
  return postfix.empty() ||
         ( ( postfix.length() <= text.length() ) && ( text.substr( text.length() - postfix.length() ) == postfix ) );
}

void check( bool condition, int line, std::string const & message )
{
  if ( !condition )
  {
    throw std::runtime_error( "VulkanHppGenerator: Spec error on line " + std::to_string( line ) + ": " + message );
  }
}

// check the validity of an attributes map
// line       : the line in the xml file where the attributes are listed
// attributes : the map of name/value pairs of the encountered attributes
// required   : the required attributes, with a set of allowed values per attribute
// optional   : the optional attributes, with a set of allowed values per attribute
void checkAttributes( int                                                  line,
                      std::map<std::string, std::string> const &           attributes,
                      std::map<std::string, std::set<std::string>> const & required,
                      std::map<std::string, std::set<std::string>> const & optional )
{
  // check if all required attributes are included and if there is a set of allowed values, check if the actual value is
  // part of that set
  for ( auto const & r : required )
  {
    auto attributesIt = attributes.find( r.first );
    check( attributesIt != attributes.end(), line, "missing attribute <" + r.first + ">" );
    check( r.second.empty() || ( r.second.find( attributesIt->second ) != r.second.end() ),
           line,
           "unexpected attribute value <" + attributesIt->second + "> in attribute <" + r.first + ">" );
  }
  // check if all not required attributes or optional, and if there is a set of allowed values, check if the actual
  // value is part of that set
  for ( auto const & a : attributes )
  {
    if ( required.find( a.first ) == required.end() )
    {
      auto optionalIt = optional.find( a.first );
      if ( optionalIt == optional.end() )
      {
        warn( false, line, "unknown attribute <" + a.first + ">" );
        continue;
      }
      if ( !optionalIt->second.empty() )
      {
        std::vector<std::string> values = tokenize( a.second, "," );
        for ( auto const & v : values )
        {
          warn( optionalIt->second.find( v ) != optionalIt->second.end(),
                line,
                "unexpected attribute value <" + v + "> in attribute <" + a.first + ">" );
        }
      }
    }
  }
}

void checkedAssignment( std::string &       str,
                        std::string const & leave,
                        std::string const & commandName,
                        std::string         aliasName )
{
  str.erase( str.length() - leave.length() );
  str += "      if ( !" + commandName + " ) " + commandName + " = " + aliasName + ";\n" + leave;
}

void checkElements( int                                               line,
                    std::vector<tinyxml2::XMLElement const *> const & elements,
                    std::map<std::string, bool> const &               required,
                    std::set<std::string> const &                     optional )
{
  std::map<std::string, size_t> encountered;
  for ( auto const & e : elements )
  {
    std::string value = e->Value();
    encountered[value]++;
    warn( ( required.find( value ) != required.end() ) || ( optional.find( value ) != optional.end() ),
          e->GetLineNum(),
          "unknown element <" + value + ">" );
  }
  for ( auto const & r : required )
  {
    auto encounteredIt = encountered.find( r.first );
    check( encounteredIt != encountered.end(), line, "missing required element <" + r.first + ">" );
    // check: r.second (means: required excactly once) => (encouteredIt->second == 1)
    check( !r.second || ( encounteredIt->second == 1 ),
           line,
           "required element <" + r.first + "> is supposed to be listed exactly once, but is listed " +
             std::to_string( encounteredIt->second ) );
  }
}

std::string constructCArraySizes( std::vector<std::string> const & sizes )
{
  std::string arraySizes;
  for ( auto const & s : sizes )
  {
    arraySizes += "[" + s + "]";
  }
  return arraySizes;
}

std::string constructStandardArray( std::string const & type, std::vector<std::string> const & sizes )
{
  std::string arrayString = "std::array<" + type + "," + sizes.back() + ">";
  for ( size_t i = sizes.size() - 2; i < sizes.size(); i-- )
  {
    arrayString = "std::array<" + arrayString + "," + sizes[i] + ">";
  }
  return arrayString;
}

std::string constructStandardArrayWrapper( std::string const & type, std::vector<std::string> const & sizes )
{
  std::string arrayString = "VULKAN_HPP_NAMESPACE::ArrayWrapper" + std::to_string( sizes.size() ) + "D<" + type;
  for ( auto const & size : sizes )
  {
    arrayString += ", " + size;
  }
  arrayString += ">";
  return arrayString;
}

std::string createEnumValueName( std::string const & name,
                                 std::string const & prefix,
                                 std::string const & postfix,
                                 bool                bitmask,
                                 std::string const & tag )
{
  std::string result = "e" + toCamelCase( stripPostfix( stripPrefix( name, prefix ), postfix ) );
  if ( bitmask )
  {
    size_t pos = result.find( "Bit" );
    if ( pos != std::string::npos )
    {
      result.erase( pos, 3 );
    }
  }
  if ( !tag.empty() && ( result.substr( result.length() - tag.length() ) == toCamelCase( tag ) ) )
  {
    result = result.substr( 0, result.length() - tag.length() ) + tag;
  }
  return result;
}

std::string createSuccessCode( std::string const & code, std::set<std::string> const & tags )
{
  std::string tag = findTag( tags, code );
  // on each success code: prepend 'e', strip "VK_" and a tag, convert it to camel case, and add the tag again
  return "e" + toCamelCase( stripPostfix( stripPrefix( code, "VK_" ), tag ) ) + tag;
}

std::string determineCommandName( std::string const &           vulkanCommandName,
                                  std::string const &           argumentType,
                                  std::set<std::string> const & tags )
{
  std::string commandName( startLowerCase( stripPrefix( vulkanCommandName, "vk" ) ) );

  if ( !argumentType.empty() )
  {
    std::string searchName  = stripPrefix( argumentType, "Vk" );
    std::string argumentTag = findTag( tags, argumentType );
    if ( !argumentTag.empty() )
    {
      searchName = stripPostfix( searchName, argumentTag );
    }
    size_t pos = commandName.find( searchName );
    if ( ( pos == std::string::npos ) && isupper( searchName[0] ) )
    {
      searchName[0] = static_cast<char>( tolower( searchName[0] ) );
      pos           = commandName.find( searchName );
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
    else if ( ( searchName == "commandBuffer" ) && beginsWith( commandName, "cmd" ) )
    {
      commandName.erase( 0, 3 );
      pos = 0;
    }
    if ( ( pos == 0 ) && isupper( commandName[0] ) )
    {
      commandName[0] = static_cast<char>( tolower( commandName[0] ) );
    }
    std::string commandTag = findTag( tags, commandName );
    if ( !argumentTag.empty() && ( argumentTag == commandTag ) )
    {
      commandName = stripPostfix( commandName, argumentTag );
    }
  }
  return commandName;
}

std::string determineNoDiscard( bool multiSuccessCodes, bool multiErrorCodes )
{
  return ( multiSuccessCodes ) ? "VULKAN_HPP_NODISCARD "
                               : ( multiErrorCodes ? "VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS " : "" );
}

std::set<size_t> determineSingularParams( size_t returnParamIndex, std::map<size_t, size_t> const & vectorParamIndices )
{
  auto returnVectorIt = vectorParamIndices.find( returnParamIndex );
  assert( returnVectorIt != vectorParamIndices.end() );
  std::set<size_t> singularParams;
  singularParams.insert( returnVectorIt->second );
  for ( auto const & vpi : vectorParamIndices )
  {
    if ( vpi.second == returnVectorIt->second )
    {
      singularParams.insert( vpi.first );
    }
  }
  return singularParams;
}

std::set<size_t> determineSkippedParams( size_t returnParamIndex, std::map<size_t, size_t> const & vectorParamIndices )
{
  std::set<size_t> skippedParams;

  // the size-parameters of vector parameters are not explicitly used in the enhanced API
  std::for_each( vectorParamIndices.begin(),
                 vectorParamIndices.end(),
                 [&skippedParams]( std::pair<size_t, size_t> const & vp )
                 {
                   if ( vp.second != INVALID_INDEX )
                   {
                     skippedParams.insert( vp.second );
                   }
                 } );

  // and the return parameter is also skipped
  if ( returnParamIndex != INVALID_INDEX )
  {
    skippedParams.insert( returnParamIndex );
  }
  return skippedParams;
}

std::string findTag( std::set<std::string> const & tags, std::string const & name, std::string const & postfix )
{
  auto tagIt = std::find_if(
    tags.begin(), tags.end(), [&name, &postfix]( std::string const & t ) { return endsWith( name, t + postfix ); } );
  return ( tagIt != tags.end() ) ? *tagIt : "";
}

std::map<std::string, std::string> getAttributes( tinyxml2::XMLElement const * element )
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
std::vector<tinyxml2::XMLElement const *> getChildElements( ElementContainer const * element )
{
  std::vector<tinyxml2::XMLElement const *> childElements;
  for ( tinyxml2::XMLElement const * childElement = element->FirstChildElement(); childElement;
        childElement                              = childElement->NextSiblingElement() )
  {
    childElements.push_back( childElement );
  }
  return childElements;
}

std::string getEnumPostfix( std::string const & name, std::set<std::string> const & tags, std::string & prefix )
{
  std::string postfix;
  if ( name != "VkResult" )
  {
    // if the enum name contains a tag move it from the prefix to the postfix to generate correct enum value names.
    for ( auto const & tag : tags )
    {
      if ( endsWith( prefix, tag + "_" ) )
      {
        prefix.erase( prefix.length() - tag.length() - 1 );
        postfix = "_" + tag;
        break;
      }
      else if ( endsWith( name, tag ) )
      {
        postfix = "_" + tag;
        break;
      }
    }
  }
  return postfix;
}

std::pair<std::vector<std::string>, std::string> readModifiers( tinyxml2::XMLNode const * node )
{
  std::vector<std::string> arraySizes;
  std::string              bitCount;
  if ( node && node->ToText() )
  {
    // following the name there might be some array size
    std::string value = node->Value();
    assert( !value.empty() );
    if ( value[0] == '[' )
    {
      std::string::size_type endPos = 0;
      while ( endPos + 1 != value.length() )
      {
        std::string::size_type startPos = value.find( '[', endPos );
        check( startPos != std::string::npos, node->GetLineNum(), "could not find '[' in <" + value + ">" );
        endPos = value.find( ']', startPos );
        check( endPos != std::string::npos, node->GetLineNum(), "could not find ']' in <" + value + ">" );
        check( startPos + 2 <= endPos, node->GetLineNum(), "missing content between '[' and ']' in <" + value + ">" );
        arraySizes.push_back( value.substr( startPos + 1, endPos - startPos - 1 ) );
      }
    }
    else if ( value[0] == ':' )
    {
      bitCount = value.substr( 1 );
    }
    else
    {
      check( ( value[0] == ';' ) || ( value[0] == ')' ), node->GetLineNum(), "unknown modifier <" + value + ">" );
    }
  }
  return std::make_pair( arraySizes, bitCount );
}

std::string namespacedType( std::string const & type )
{
  return beginsWith( type, "Vk" ) ? ( "VULKAN_HPP_NAMESPACE::" + stripPrefix( type, "Vk" ) ) : type;
}

std::string readTypePostfix( tinyxml2::XMLNode const * node )
{
  std::string postfix;
  if ( node && node->ToText() )
  {
    postfix = trimStars( trimEnd( node->Value() ) );
  }
  return postfix;
}

std::string readTypePrefix( tinyxml2::XMLNode const * node )
{
  std::string prefix;
  if ( node && node->ToText() )
  {
    prefix = trim( node->Value() );
  }
  return prefix;
}

void replaceAll( std::string & str, std::string const & from, std::string const & to )
{
  size_t pos = 0;
  while ( ( pos = str.find( from, pos ) ) != std::string::npos )
  {
    str.replace( pos, from.length(), to );
    pos += to.length();  // Handles case where 'to' is a substring of 'from'
  }
}

std::string replaceWithMap( std::string const & input, std::map<std::string, std::string> replacements )
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

    result +=
      match.prefix().str() + ( ( itReplacement != replacements.end() ) ? itReplacement->second : match[0].str() );
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

std::string startLowerCase( std::string const & input )
{
  return input.empty() ? "" : static_cast<char>( tolower( input[0] ) ) + input.substr( 1 );
}

std::string startUpperCase( std::string const & input )
{
  return input.empty() ? "" : static_cast<char>( toupper( input[0] ) ) + input.substr( 1 );
}

std::string stripPostfix( std::string const & value, std::string const & postfix )
{
  std::string strippedValue = value;
  if ( endsWith( strippedValue, postfix ) )
  {
    strippedValue.erase( strippedValue.length() - postfix.length() );
  }
  return strippedValue;
}

std::string stripPluralS( std::string const & name )
{
  std::string strippedName( name );
  size_t      pos = strippedName.rfind( 's' );
  if ( pos != std::string::npos )
  {
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

std::string stripPrefix( std::string const & value, std::string const & prefix )
{
  std::string strippedValue = value;
  if ( beginsWith( strippedValue, prefix ) )
  {
    strippedValue.erase( 0, prefix.length() );
  }
  return strippedValue;
}

std::string toCamelCase( std::string const & value )
{
  assert( !value.empty() && ( isupper( value[0] ) || isdigit( value[0] ) ) );
  std::string result;
  result.reserve( value.size() );
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
  return result;
}

template <typename StringContainer>
std::string toString( StringContainer const & strings )
{
  std::string str;
  bool        encounteredMember = false;
  for ( auto s : strings )
  {
    if ( encounteredMember )
    {
      str += ", ";
    }
    str += s;
    encounteredMember = true;
  }
  return str;
}

std::string toUpperCase( std::string const & name )
{
  std::string convertedName;
  bool        previousIsLowerCase = false;
  bool        previousIsDigit     = false;
  for ( auto c : name )
  {
    if ( ( isupper( c ) && ( previousIsLowerCase || previousIsDigit ) ) || ( isdigit( c ) && !previousIsDigit ) )
    {
      convertedName.push_back( '_' );
    }
    convertedName.push_back( static_cast<char>( toupper( c ) ) );
    previousIsLowerCase = !!islower( c );
    previousIsDigit     = !!isdigit( c );
  }
  return convertedName;
}

std::vector<std::string> tokenize( std::string const & tokenString, std::string const & separator )
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

std::string trim( std::string const & input )
{
  std::string result = input;
  result.erase( result.begin(),
                std::find_if( result.begin(), result.end(), []( char c ) { return !std::isspace( c ); } ) );
  result.erase( std::find_if( result.rbegin(), result.rend(), []( char c ) { return !std::isspace( c ); } ).base(),
                result.end() );
  return result;
}

std::string trimEnd( std::string const & input )
{
  std::string result = input;
  result.erase( std::find_if( result.rbegin(), result.rend(), []( char c ) { return !std::isspace( c ); } ).base(),
                result.end() );
  return result;
}

std::string trimStars( std::string const & input )
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

void warn( bool condition, int line, std::string const & message )
{
  if ( !condition )
  {
    std::cerr << "VulkanHppGenerator: Spec warning on line " << std::to_string( line ) << ": " << message << "!"
              << std::endl;
  }
}

VulkanHppGenerator::VulkanHppGenerator( tinyxml2::XMLDocument const & document )
{
  m_handles.insert( std::make_pair(
    "", HandleData( {}, "", 0 ) ) );  // insert the default "handle" without class (for createInstance, and such)

  int                                       line     = document.GetLineNum();
  std::vector<tinyxml2::XMLElement const *> elements = getChildElements( &document );
  checkElements( line, elements, { { "registry", true } } );
  check( elements.size() == 1,
         line,
         "encountered " + std::to_string( elements.size() ) + " elements named <registry> but only one is allowed" );
  readRegistry( elements[0] );
  checkCorrectness();

  for ( auto extensionIt = m_extensions.begin(); extensionIt != m_extensions.end(); ++extensionIt )
  {
    int number = atoi( extensionIt->second.number.c_str() );
    assert( m_extensionsByNumber.find( number ) == m_extensionsByNumber.end() );
    m_extensionsByNumber[number] = extensionIt;
  }
}

void VulkanHppGenerator::addCommand( std::string const & name, CommandData & commandData )
{
  // find the handle this command is going to be associated to
  check( !commandData.params.empty(), commandData.xmlLine, "command <" + name + "> with no params" );
  std::map<std::string, HandleData>::iterator handleIt = m_handles.find( commandData.params[0].type.type );
  if ( handleIt == m_handles.end() )
  {
    handleIt = m_handles.find( "" );
  }
  check( handleIt != m_handles.end(), commandData.xmlLine, "could not find a handle to hold command <" + name + ">" );
  commandData.handle = handleIt->first;

  // add this command to the list of commands
  check( m_commands.insert( std::make_pair( name, commandData ) ).second,
         commandData.xmlLine,
         "already encountered command <" + name + ">" );

  // put the command into the handle's list of commands
  check( handleIt->second.commands.insert( name ).second,
         commandData.xmlLine,
         "command list of handle <" + handleIt->first + "> already holds a commnand <" + name + ">" );
}

void VulkanHppGenerator::appendArgumentPlainType( std::string & str, ParamData const & paramData ) const
{
  // this parameter is just a plain type
  if ( !paramData.type.postfix.empty() )
  {
    assert( paramData.type.postfix.back() == '*' );
    // it's a pointer
    std::string parameterName = startLowerCase( stripPrefix( paramData.name, "p" ) );
    // it's a non-const pointer, and char is the only type that occurs -> use the address of the parameter
    assert( paramData.type.type.find( "char" ) == std::string::npos );
    str += "&" + parameterName;
  }
  else
  {
    // it's a plain parameter -> just use its name
    str += paramData.name;
  }
}

void VulkanHppGenerator::appendArguments( std::string &                    str,
                                          CommandData const &              commandData,
                                          size_t                           returnParamIndex,
                                          size_t                           templateParamIndex,
                                          std::map<size_t, size_t> const & vectorParamIndices,
                                          bool                             twoStep,
                                          bool                             firstCall,
                                          size_t                           from,
                                          size_t                           to ) const
{
  assert( from <= to );

  bool encounteredArgument = false;
  for ( size_t i = from; i < to; i++ )
  {
    if ( encounteredArgument )
    {
      str += ", ";
    }

    auto it = vectorParamIndices.find( i );
    if ( it != vectorParamIndices.end() )
    {
      appendArgumentVector(
        str, it->first, commandData.params[it->first], returnParamIndex, templateParamIndex, twoStep, firstCall );
    }
    else
    {
      it = find_if( vectorParamIndices.begin(),
                    vectorParamIndices.end(),
                    [i]( std::pair<size_t, size_t> const & vpi ) { return vpi.second == i; } );
      if ( it != vectorParamIndices.end() )
      {
        // this parameter is a count parameter for a vector parameter
        // the corresponding vector parameter is not the return parameter, or it's not a two-step algorithm
        // for the non-singular version, the count is the size of the vector parameter
        // -> use the vector parameter name without leading 'p' to get the size (in number of elements, not in bytes)
        assert( commandData.params[it->first].name[0] == 'p' );
        str += startLowerCase( stripPrefix( commandData.params[it->first].name, "p" ) ) + ".size() ";
        if ( it->first == templateParamIndex )
        {
          // if the vector parameter is templatized -> multiply by the size of that type to get the size in bytes
          str += "* sizeof( T ) ";
        }
      }
      else if ( beginsWith( commandData.params[i].type.type, "Vk" ) )
      {
        str += "static_cast<" + commandData.params[i].type.type + ">( " + commandData.params[i].name + " )";
      }
      else
      {
        appendArgumentPlainType( str, commandData.params[i] );
      }
    }
    encounteredArgument = true;
  }
}

void VulkanHppGenerator::appendArgumentVector( std::string &     str,
                                               size_t            paramIndex,
                                               ParamData const & paramData,
                                               size_t            returnParamIndex,
                                               size_t            templateParamIndex,
                                               bool              twoStep,
                                               bool              firstCall ) const
{
  // this parameter is a vector parameter
  assert( paramData.type.postfix.back() == '*' );
  if ( ( returnParamIndex == paramIndex ) && twoStep && firstCall )
  {
    // this parameter is the return parameter, and it's the first call of a two-step algorithm -> just just nullptr
    str += "nullptr";
  }
  else
  {
    std::string parameterName = startLowerCase( stripPrefix( paramData.name, "p" ) );
    if ( beginsWith( paramData.type.type, "Vk" ) || ( paramIndex == templateParamIndex ) )
    {
      // CHECK for !commandData.params[it->first].optional

      // this parameter is a vulkan type or a templated type -> need to reinterpret cast
      str += "reinterpret_cast<";
      if ( paramData.type.prefix.find( "const" ) == 0 )
      {
        str += "const ";
      }
      str += paramData.type.type + "*>( " + parameterName + ".data() )";
    }
    else
    {
      // this parameter is just a vetor -> get the pointer to its data
      str += parameterName + ".data()";
    }
  }
}

void VulkanHppGenerator::appendBaseTypes( std::string & str ) const
{
  assert( !m_baseTypes.empty() );
  for ( auto const & baseType : m_baseTypes )
  {
    if ( ( baseType.first != "VkFlags" ) &&
         ( baseType.first !=
           "VkFlags64" ) )  // filter out VkFlags and VkFlags64, as they are mapped to our own Flags class
    {
      str += "  using " + stripPrefix( baseType.first, "Vk" ) + " = " + baseType.second.type + ";\n";
    }
  }
}

void VulkanHppGenerator::appendBitmasks( std::string & str ) const
{
  str += R"(
  //================
  //=== BITMASKs ===
  //================
)";

  std::set<std::string> listedBitmasks;
  for ( auto const & feature : m_features )
  {
    str += "\n  //=== " + feature.first + " ===\n";
    for ( auto const & type : feature.second.types )
    {
      auto bitmaskIt = m_bitmasks.find( type.first );
      if ( bitmaskIt != m_bitmasks.end() )
      {
        assert( listedBitmasks.find( type.first ) == listedBitmasks.end() );
        listedBitmasks.insert( type.first );
        appendBitmask( str, bitmaskIt );
      }
    }
  }

  for ( auto const & extIt : m_extensionsByNumber )
  {
    std::vector<std::map<std::string, BitmaskData>::const_iterator> bitmaskIts;
    for ( auto const & type : extIt.second->second.types )
    {
      auto bitmaskIt = m_bitmasks.find( type );
      if ( bitmaskIt != m_bitmasks.end() )
      {
        bitmaskIts.push_back( bitmaskIt );
      }
    }

    if ( !bitmaskIts.empty() )
    {
      std::string enter, leave;
      std::tie( enter, leave ) =
        generateProtection( bitmaskIts.front()->first, !bitmaskIts.front()->second.alias.empty() );
      if ( !enter.empty() )
      {
        enter = "\n" + enter;
      }
      str += enter + "\n  //=== " + extIt.second->first + " ===\n";
      for ( auto bitmaskIt : bitmaskIts )
      {
        assert( listedBitmasks.find( bitmaskIt->first ) == listedBitmasks.end() );
        listedBitmasks.insert( bitmaskIt->first );
        appendBitmask( str, bitmaskIt );
      }
      str += leave;
    }
  }
}

void VulkanHppGenerator::appendBitmask( std::string &                                      str,
                                        std::map<std::string, BitmaskData>::const_iterator bitmaskIt ) const
{
  auto bitmaskBits = m_enums.find( bitmaskIt->second.requirements );
  bool hasBits     = ( bitmaskBits != m_enums.end() );
  check( bitmaskIt->second.requirements.empty() || hasBits,
         bitmaskIt->second.xmlLine,
         "bitmask <" + bitmaskIt->first + "> references the undefined requires <" + bitmaskIt->second.requirements +
           ">" );

  std::string strippedBitmaskName = stripPrefix( bitmaskIt->first, "Vk" );
  std::string strippedEnumName    = hasBits ? stripPrefix( bitmaskBits->first, "Vk" ) : "";

  appendBitmask( str,
                 strippedBitmaskName,
                 bitmaskIt->second.type,
                 bitmaskIt->second.alias,
                 strippedEnumName,
                 hasBits ? bitmaskBits->second.values : std::vector<EnumValueData>() );
  appendBitmaskToStringFunction(
    str, strippedBitmaskName, strippedEnumName, hasBits ? bitmaskBits->second.values : std::vector<EnumValueData>() );
}

void VulkanHppGenerator::appendBitmask( std::string &                      str,
                                        std::string const &                bitmaskName,
                                        std::string const &                bitmaskType,
                                        std::string const &                bitmaskAlias,
                                        std::string const &                enumName,
                                        std::vector<EnumValueData> const & enumValues ) const
{
  // each Flags class is using the class 'Flags' with the corresponding FlagBits enum as the template parameter
  // if there's no enum for the FlagBits, introduce an artificial empty one
  std::string emptyEnumName;
  if ( enumName.empty() )
  {
    emptyEnumName = bitmaskName;
    size_t pos    = emptyEnumName.rfind( "Flags" );
    assert( pos != std::string::npos );
    emptyEnumName.replace( pos, 5, "FlagBits" );

    // if this emptyEnumName is not in the list of enums, list it here
    if ( m_enums.find( "Vk" + emptyEnumName ) == m_enums.end() )
    {
      const std::string templateString = R"x(
  enum class ${enumName} : ${bitmaskType}
  {};

  VULKAN_HPP_INLINE std::string to_string( ${enumName} )
  {
    return "(void)";
  }
)x";

      str += replaceWithMap( templateString, { { "enumName", emptyEnumName }, { "bitmaskType", bitmaskType } } );
    }
  }
  std::string name = ( enumName.empty() ? emptyEnumName : enumName );
  str += "\n  using " + bitmaskName + " = Flags<" + name + ">;\n";

  if ( !enumValues.empty() )
  {
    std::string allFlags;
    bool        encounteredFlag = false;
    std::string previousEnter, previousLeave;
    for ( auto const & value : enumValues )
    {
      std::string enter, leave;
      if ( !value.extension.empty() )
      {
        std::tie( enter, leave ) = generateProtection( value.extension );
        if ( !leave.empty() )
        {
          assert( leave.back() );
          leave.pop_back();
          leave = "\n" + leave;
        }
      }
      allFlags += ( ( previousEnter != enter ) ? ( previousLeave + "\n" + enter ) : "\n" ) + "        " +
                  ( encounteredFlag ? "| " : "  " ) + bitmaskType + "( " + enumName + "::" + value.vkValue + " )";
      encounteredFlag = true;
      previousEnter   = enter;
      previousLeave   = leave;
    }
    allFlags += previousLeave;

    static const std::string bitmaskOperatorsTemplate = R"(
  template <> struct FlagTraits<${enumName}>
  {
    enum : ${bitmaskType}
    {
      allFlags = ${allFlags}
    };
  };

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR ${bitmaskName} operator|( ${enumName} bit0, ${enumName} bit1 ) VULKAN_HPP_NOEXCEPT
  {
    return ${bitmaskName}( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR ${bitmaskName} operator&( ${enumName} bit0, ${enumName} bit1 ) VULKAN_HPP_NOEXCEPT
  {
    return ${bitmaskName}( bit0 ) & bit1;
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR ${bitmaskName} operator^( ${enumName} bit0, ${enumName} bit1 ) VULKAN_HPP_NOEXCEPT
  {
    return ${bitmaskName}( bit0 ) ^ bit1;
  }

  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR ${bitmaskName} operator~( ${enumName} bits ) VULKAN_HPP_NOEXCEPT
  {
    return ~( ${bitmaskName}( bits ) );
  }
)";

    str += replaceWithMap( bitmaskOperatorsTemplate,
                           { { "bitmaskName", bitmaskName },
                             { "bitmaskType", bitmaskType },
                             { "enumName", enumName },
                             { "allFlags", allFlags } } );
  }

  if ( !bitmaskAlias.empty() )
  {
    str +=
      "\n"
      "  using " +
      stripPrefix( bitmaskAlias, "Vk" ) + " = " + bitmaskName + ";\n";
  }
}

void VulkanHppGenerator::appendBitmaskToStringFunction( std::string &                      str,
                                                        std::string const &                bitmaskName,
                                                        std::string const &                enumName,
                                                        std::vector<EnumValueData> const & enumValues ) const
{
  str +=
    "\n"
    "  VULKAN_HPP_INLINE std::string to_string( " +
    bitmaskName + ( enumValues.empty() ? " " : " value " ) +
    " )\n"
    "  {\n";

  if ( enumValues.empty() )
  {
    str += "    return \"{}\";\n";
  }
  else
  {
    // 'or' together all the bits in the value
    str +=
      "    if ( !value ) return \"{}\";\n"
      "    std::string result;\n";
    for ( auto const & evd : enumValues )
    {
      if ( evd.singleBit )
      {
        std::string enter, leave;
        if ( !evd.extension.empty() )
        {
          std::tie( enter, leave ) = generateProtection( evd.extension );
          if ( !leave.empty() )
          {
            leave = "\n" + leave;
            leave.pop_back();
          }
        }
        str += "\n" + enter + "    if ( value & " + enumName + "::" + evd.vkValue + " ) result += \"" +
               evd.vkValue.substr( 1 ) + " | \";" + leave;
      }
    }
    str +=
      "\n"
      "    return \"{ \" + result.substr(0, result.size() - 3) + \" }\";\n";
  }

  str += "  }\n";
}

void VulkanHppGenerator::appendCall( std::string &                    str,
                                     std::string const &              name,
                                     CommandData const &              commandData,
                                     size_t                           returnParamIndex,
                                     size_t                           templateParamIndex,
                                     std::map<size_t, size_t> const & vectorParamIndices,
                                     bool                             twoStep,
                                     bool                             firstCall ) const
{
  // the original function call
  str += "d." + name + "( ";

  if ( !commandData.handle.empty() )
  {
    auto handleIt = m_handles.find( commandData.handle );
    assert( handleIt != m_handles.end() );

    // if it's member of a class -> the first argument is the member variable, starting with "m_"
    assert( handleIt->first == commandData.params[0].type.type );
    str += "m_" + startLowerCase( stripPrefix( handleIt->first, "Vk" ) );
    if ( 1 < commandData.params.size() )
    {
      str += ", ";
    }
  }

  appendArguments( str,
                   commandData,
                   returnParamIndex,
                   templateParamIndex,
                   vectorParamIndices,
                   twoStep,
                   firstCall,
                   commandData.handle.empty() ? 0 : 1,
                   commandData.params.size() );
  str += " )";
}

void VulkanHppGenerator::appendCommand( std::string &       str,
                                        std::string const & name,
                                        CommandData const & commandData,
                                        size_t              initialSkipCount,
                                        bool                definition ) const
{
  bool appendedFunction = false;
  if ( commandData.returnType == "VkResult" )
  {
    appendedFunction = appendCommandResult( str, name, commandData, initialSkipCount, definition );
  }
  else
  {
    assert( commandData.successCodes.empty() && commandData.errorCodes.empty() );
    if ( commandData.returnType == "void" )
    {
      appendedFunction = appendCommandVoid( str, name, commandData, initialSkipCount, definition );
    }
    else
    {
      appendedFunction = appendCommandValue( str, name, commandData, initialSkipCount, definition );
    }
  }

  if ( !appendedFunction )
  {
    throw std::runtime_error( "Never encountered a function like <" + name + "> !" );
  }
}

void VulkanHppGenerator::appendCommandChained( std::string &                    str,
                                               std::string const &              name,
                                               CommandData const &              commandData,
                                               size_t                           initialSkipCount,
                                               bool                             definition,
                                               std::map<size_t, size_t> const & vectorParamIndices,
                                               size_t                           nonConstPointerIndex ) const
{
  assert( ( commandData.returnType == "VkResult" ) || ( commandData.returnType == "void" ) );

  std::string const functionTemplate = R"(
${enter}${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedChained}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
${leave})";

  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );

  str += replaceWithMap(
    functionTemplate,
    std::map<std::string, std::string>(
      { { "commandEnhanced",
          ( commandData.returnType == "void" )
            ? constructCommandVoidGetValue(
                name, commandData, initialSkipCount, definition, vectorParamIndices, nonConstPointerIndex )
            : constructCommandResultGetValue( name, commandData, initialSkipCount, definition, nonConstPointerIndex ) },
        { "commandEnhancedChained",
          ( commandData.returnType == "void" )
            ? constructCommandVoidGetChain( name, commandData, initialSkipCount, definition, nonConstPointerIndex )
            : constructCommandResultGetChain( name, commandData, initialSkipCount, definition, nonConstPointerIndex ) },
        { "commandStandard", constructCommandStandard( name, commandData, initialSkipCount, definition ) },
        { "enter", enter },
        { "leave", leave },
        { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

bool VulkanHppGenerator::appendCommandResult( std::string &       str,
                                              std::string const & name,
                                              CommandData const & commandData,
                                              size_t              initialSkipCount,
                                              bool                definition ) const
{
  std::vector<size_t> returnParamIndices = determineNonConstPointerParamIndices( commandData.params );
  switch ( returnParamIndices.size() )
  {
    case 0: return appendCommandResult0Return( str, name, commandData, initialSkipCount, definition );
    case 1:
      return appendCommandResult1Return( str, name, commandData, initialSkipCount, definition, returnParamIndices[0] );
    case 2:
      return appendCommandResult2Return( str, name, commandData, initialSkipCount, definition, returnParamIndices );
    case 3:
      return appendCommandResult3Return( str, name, commandData, initialSkipCount, definition, returnParamIndices );
    default: return false;
  }
}

bool VulkanHppGenerator::appendCommandResult0Return( std::string &       str,
                                                     std::string const & name,
                                                     CommandData const & commandData,
                                                     size_t              initialSkipCount,
                                                     bool                definition ) const
{
  std::map<size_t, size_t> vectorParamIndices       = determineVectorParamIndicesNew( commandData.params );
  std::vector<size_t>      constPointerParamIndices = determineConstPointerParamIndices( commandData.params );

  if ( vectorParamIndices.empty() && std::find_if( constPointerParamIndices.begin(),
                                                   constPointerParamIndices.end(),
                                                   [&commandData]( size_t idx ) {
                                                     return commandData.params[idx].type.type != "void";
                                                   } ) == constPointerParamIndices.end() )
  {
    // no vector paramter and no non-void const-pointer
    // function returning a result but no fancy input have either standard or enhanced call
    appendCommandStandardOrEnhanced( str, name, commandData, initialSkipCount, definition );
  }
  else
  {
    // functions with some fancy input have both, standard and enhanced call
    appendCommandStandardAndEnhanced( str, name, commandData, initialSkipCount, definition, vectorParamIndices, {} );
  }
  return true;
}

bool VulkanHppGenerator::appendCommandResult1Return( std::string &       str,
                                                     std::string const & name,
                                                     CommandData const & commandData,
                                                     size_t              initialSkipCount,
                                                     bool                definition,
                                                     size_t              returnParamIndex ) const
{
  bool                     appendedFunction   = false;
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndicesNew( commandData.params );
  if ( isHandleType( commandData.params[returnParamIndex].type.type ) )
  {
    // get handle(s)
    auto returnVectorParamIt = vectorParamIndices.find( returnParamIndex );
    if ( returnVectorParamIt == vectorParamIndices.end() )
    {
      // the return parameter is not a vector -> get just one handle
      // provide standard, enhanced, and unique call
      appendCommandUnique( str, name, commandData, initialSkipCount, returnParamIndex, definition );
      appendedFunction = true;
    }
    else
    {
      // get a vector of handles
      if ( ( commandData.params[returnVectorParamIt->second].type.isValue() ) )
      {
        if ( ( vectorParamIndices.size() == 2 ) &&
             ( vectorParamIndices.begin()->second == std::next( vectorParamIndices.begin() )->second ) )
        {
          // provide standard, enhanced, vector, singular, and unique (and the combinations!) calls
          appendCommandVectorSingularUnique(
            str, name, commandData, initialSkipCount, vectorParamIndices, returnParamIndex, definition );
          appendedFunction = true;
        }
      }
      else if ( ( ( isLenByStructMember( commandData.params[returnVectorParamIt->first].len,
                                         commandData.params[returnVectorParamIt->second] ) ) ) &&
                ( vectorParamIndices.size() == 1 ) )
      {
        // provide standard, enhanced, vector, and unique (and the combinations!) calls
        appendCommandVectorUnique(
          str, name, commandData, initialSkipCount, vectorParamIndices, returnParamIndex, definition );
        appendedFunction = true;
      }
    }
  }
  else if ( isStructureChainAnchor( commandData.params[returnParamIndex].type.type ) )
  {
    auto returnVectorParamIt = vectorParamIndices.find( returnParamIndex );
    if ( returnVectorParamIt == vectorParamIndices.end() )
    {
      // provide standard, enhanced, and chained call
      appendCommandChained(
        str, name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex );
      appendedFunction = true;
    }
  }
  else
  {
    auto returnVectorParamIt = vectorParamIndices.find( returnParamIndex );
    if ( returnVectorParamIt == vectorParamIndices.end() )
    {
      appendCommandStandardAndEnhanced(
        str, name, commandData, initialSkipCount, definition, vectorParamIndices, { returnParamIndex } );
      appendedFunction = true;
    }
    else if ( commandData.params[returnVectorParamIt->second].type.isValue() )
    {
      // the vector size is given by value
      // -> provide standard, enhanced, and singular calls
      if ( commandData.params[returnVectorParamIt->first].type.type == "void" )
      {
        appendCommandSingular(
          str, name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex );
      }
      else
      {
        appendCommandVectorSingular(
          str, name, commandData, initialSkipCount, vectorParamIndices, returnParamIndex, definition );
      }
      appendedFunction = true;
    }
  }
  return appendedFunction;
}

bool VulkanHppGenerator::appendCommandResult2Return( std::string &               str,
                                                     std::string const &         name,
                                                     CommandData const &         commandData,
                                                     size_t                      initialSkipCount,
                                                     bool                        definition,
                                                     std::vector<size_t> const & returnParamIndices ) const
{
  assert( returnParamIndices.size() == 2 );

  bool                     appendedFunction   = false;
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndicesNew( commandData.params );
  if ( !isHandleType( commandData.params[returnParamIndices[0]].type.type ) &&
       !isStructureChainAnchor( commandData.params[returnParamIndices[0]].type.type ) )
  {
    assert( !isStructureChainAnchor( commandData.params[returnParamIndices[1]].type.type ) );
    // non of the return parameters is a StructureChain
    // Note: if the vector returned holds handles, the function does not create them, but just gets them
    assert( !vectorParamIndices.empty() );
    switch ( vectorParamIndices.size() )
    {
      case 1:
        {
          // two returns but just one vector
          auto vectorParamIndexIt = vectorParamIndices.begin();
          if ( ( vectorParamIndexIt->second == returnParamIndices[0] ) &&
               ( vectorParamIndexIt->first == returnParamIndices[1] ) )
          {
            // the size is a return value as well -> enumerate the values
            // and the vector data is not of type void
            // provide standard, enhanced, and vector calls
            appendCommandVector(
              str, name, commandData, initialSkipCount, definition, *vectorParamIndexIt, returnParamIndices );
            appendedFunction = true;
          }
        }
        break;
      case 2:
        {
          // two returns and two vectors! But one input vector, one output vector of the same size, and one
          // output value
          if ( ( vectorParamIndices.find( returnParamIndices[0] ) != vectorParamIndices.end() ) &&
               ( vectorParamIndices.find( returnParamIndices[1] ) == vectorParamIndices.end() ) )
          {
            // provide standard, enhanced deprecated, enhanced, and enhanced with allocator calls
            appendCommandStandardEnhancedDeprecatedAllocator(
              str, name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndices );
            appendedFunction = true;
          }
        }
        break;
    }
  }
  return appendedFunction;
}

bool VulkanHppGenerator::appendCommandResult3Return( std::string &               str,
                                                     std::string const &         name,
                                                     CommandData const &         commandData,
                                                     size_t                      initialSkipCount,
                                                     bool                        definition,
                                                     std::vector<size_t> const & returnParamIndices ) const
{
  assert( returnParamIndices.size() == 3 );

  bool                     appendedFunction   = false;
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndicesNew( commandData.params );
  if ( ( vectorParamIndices.size() == 2 ) && ( vectorParamIndices.begin()->second == returnParamIndices[0] ) &&
       ( vectorParamIndices.begin()->first == returnParamIndices[1] ) &&
       ( std::next( vectorParamIndices.begin() )->first == returnParamIndices[2] ) )
  {
    // two vector parameters
    auto                           firstVectorParam  = vectorParamIndices.begin();
    auto                           secondVectorParam = std::next( firstVectorParam );
    std::vector<ParamData> const & params            = commandData.params;
    if ( ( firstVectorParam->second != INVALID_INDEX ) && ( firstVectorParam->second == secondVectorParam->second ) )
    {
      // the two vectors use the very same size parameter
      if ( params[firstVectorParam->first].type.isNonConstPointer() &&
           params[secondVectorParam->first].type.isNonConstPointer() &&
           params[firstVectorParam->second].type.isNonConstPointer() )
      {
        // both vectors, as well as the size parameter are non-const pointer that is output parameters
        // provide standard, enhanced, vector and deprecated calls!
        appendCommandVectorDeprecated(
          str, name, commandData, initialSkipCount, vectorParamIndices, returnParamIndices, definition );
        appendedFunction = true;
      }
    }
  }
  return appendedFunction;
}

void VulkanHppGenerator::appendCommandSingular( std::string &                    str,
                                                std::string const &              name,
                                                CommandData const &              commandData,
                                                size_t                           initialSkipCount,
                                                bool                             definition,
                                                std::map<size_t, size_t> const & vectorParamIndices,
                                                size_t                           returnParamIndex ) const
{
  assert( commandData.returnType == "VkResult" );

  std::string const functionTemplate = R"(
${enter}${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhancedDeprecated}${newlineOnDefinition}
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedSingular}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
${leave})";

  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );

  str += replaceWithMap(
    functionTemplate,
    std::map<std::string, std::string>(
      { { "commandEnhanced",
          constructCommandResultGetVector(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex ) },
        { "commandEnhancedDeprecated",
          constructCommandResultGetVectorDeprecated(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex ) },
        { "commandEnhancedSingular",
          constructCommandResultGetVectorOfVoidSingular(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex ) },
        { "commandStandard", constructCommandStandard( name, commandData, initialSkipCount, definition ) },
        { "enter", enter },
        { "leave", leave },
        { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

void VulkanHppGenerator::appendCommandStandard( std::string &       str,
                                                std::string const & name,
                                                CommandData const & commandData,
                                                size_t              initialSkipCount,
                                                bool                definition ) const
{
  const std::string functionTemplate = R"(
${enter}${commandStandard}
${leave})";

  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );

  str += replaceWithMap(
    functionTemplate,
    std::map<std::string, std::string>(
      { { "commandStandard", constructCommandStandard( name, commandData, initialSkipCount, definition ) },
        { "enter", enter },
        { "leave", leave } } ) );
}

void VulkanHppGenerator::appendCommandStandardAndEnhanced(
  std::string &                    str,
  std::string const &              name,
  CommandData const &              commandData,
  size_t                           initialSkipCount,
  bool                             definition,
  std::map<size_t, size_t> const & vectorParamIndices,
  std::vector<size_t> const &      nonConstPointerParamIndices ) const
{
  const std::string functionTemplate = R"(
${enter}${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
${leave})";

  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );

  std::string commandEnhanced;
  switch ( nonConstPointerParamIndices.size() )
  {
    case 0:
      if ( commandData.returnType == "void" )
      {
        commandEnhanced = constructCommandVoid( name, commandData, initialSkipCount, definition, vectorParamIndices );
      }
      else if ( commandData.returnType == "VkResult" )
      {
        switch ( vectorParamIndices.size() )
        {
          case 0:
          case 1:
            commandEnhanced =
              constructCommandResult( name, commandData, initialSkipCount, definition, vectorParamIndices );
            break;
          case 2:
            if ( ( vectorParamIndices.begin()->second != INVALID_INDEX ) &&
                 ( vectorParamIndices.begin()->second == std::next( vectorParamIndices.begin() )->second ) &&
                 ( commandData.params[vectorParamIndices.begin()->second].type.isValue() ) )
            {
              commandEnhanced = constructCommandResultGetTwoVectors(
                name, commandData, initialSkipCount, definition, vectorParamIndices );
            }
            break;
        }
      }
      else if ( vectorParamIndices.empty() )
      {
        commandEnhanced = constructCommandType( name, commandData, initialSkipCount, definition );
      }
      break;
    case 1:
      if ( commandData.returnType == "VkBool32" )
      {
        commandEnhanced = constructCommandBoolGetValue(
          name, commandData, initialSkipCount, definition, nonConstPointerParamIndices[0] );
      }
      else if ( commandData.returnType == "VkResult" )
      {
        commandEnhanced = constructCommandResultGetValue(
          name, commandData, initialSkipCount, definition, nonConstPointerParamIndices[0] );
      }
      else if ( commandData.returnType == "void" )
      {
        commandEnhanced = constructCommandVoidGetValue(
          name, commandData, initialSkipCount, definition, vectorParamIndices, nonConstPointerParamIndices[0] );
      }
      break;
    case 2:
      if ( ( commandData.returnType == "VkResult" ) && ( 1 < commandData.successCodes.size() ) )
      {
        commandEnhanced = constructCommandResultGetTwoValues(
          name, commandData, initialSkipCount, definition, nonConstPointerParamIndices );
      }
      break;
  }

  if ( commandEnhanced.empty() )
  {
    throw std::runtime_error( "Never encountered a function like <" + name + "> !" );
  }

  str += replaceWithMap(
    functionTemplate,
    std::map<std::string, std::string>(
      { { "commandEnhanced", commandEnhanced },
        { "commandStandard", constructCommandStandard( name, commandData, initialSkipCount, definition ) },
        { "enter", enter },
        { "leave", leave },
        { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

void VulkanHppGenerator::appendCommandStandardEnhancedDeprecatedAllocator(
  std::string &                    str,
  std::string const &              name,
  CommandData const &              commandData,
  size_t                           initialSkipCount,
  bool                             definition,
  std::map<size_t, size_t> const & vectorParamIndices,
  std::vector<size_t> const &      nonConstPointerParamIndices ) const
{
  assert( ( vectorParamIndices.size() == 2 ) && ( nonConstPointerParamIndices.size() == 2 ) );
  assert( vectorParamIndices.find( nonConstPointerParamIndices[0] ) != vectorParamIndices.end() );
  assert( vectorParamIndices.find( nonConstPointerParamIndices[1] ) == vectorParamIndices.end() );
  assert( commandData.returnType == "VkResult" );

  const std::string functionTemplate = R"(
${enter}${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhancedDeprecated}${newlineOnDefinition}
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedWithAllocator}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
${leave})";

  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );

  str += replaceWithMap(
    functionTemplate,
    std::map<std::string, std::string>(
      { { "commandEnhanced",
          constructCommandResultGetVectorAndValue(
            name, commandData, initialSkipCount, definition, vectorParamIndices, nonConstPointerParamIndices, false ) },
        { "commandEnhancedDeprecated",
          constructCommandResultGetValueDeprecated(
            name, commandData, initialSkipCount, definition, vectorParamIndices, nonConstPointerParamIndices[1] ) },
        { "commandEnhancedWithAllocator",
          constructCommandResultGetVectorAndValue(
            name, commandData, initialSkipCount, definition, vectorParamIndices, nonConstPointerParamIndices, true ) },
        { "commandStandard", constructCommandStandard( name, commandData, initialSkipCount, definition ) },
        { "enter", enter },
        { "leave", leave },
        { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

void VulkanHppGenerator::appendCommandStandardOrEnhanced( std::string &       str,
                                                          std::string const & name,
                                                          CommandData const & commandData,
                                                          size_t              initialSkipCount,
                                                          bool                definition ) const
{
  assert( commandData.returnType == "VkResult" );

  const std::string functionTemplate = R"(
${enter}#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandStandard}
#else
${commandEnhanced}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
${leave}
)";

  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );

  str += replaceWithMap(
    functionTemplate,
    std::map<std::string, std::string>(
      { { "commandEnhanced", constructCommandResult( name, commandData, initialSkipCount, definition, {} ) },
        { "commandStandard", constructCommandStandard( name, commandData, initialSkipCount, definition ) },
        { "enter", enter },
        { "leave", leave } } ) );
}

void VulkanHppGenerator::appendCommandUnique( std::string &       str,
                                              std::string const & name,
                                              CommandData const & commandData,
                                              size_t              initialSkipCount,
                                              size_t              nonConstPointerIndex,
                                              bool                definition ) const
{
  std::string const functionTemplate = R"(
${enter}${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
#  ifndef VULKAN_HPP_NO_SMART_HANDLE
${commandEnhancedUnique}
#  endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
${leave})";

  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );

  str += replaceWithMap(
    functionTemplate,
    std::map<std::string, std::string>(
      { { "commandEnhanced",
          constructCommandResultGetValue( name, commandData, initialSkipCount, definition, nonConstPointerIndex ) },
        { "commandEnhancedUnique",
          constructCommandResultGetHandleUnique(
            name, commandData, initialSkipCount, definition, nonConstPointerIndex ) },
        { "commandStandard", constructCommandStandard( name, commandData, initialSkipCount, definition ) },
        { "enter", enter },
        { "leave", leave },
        { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

bool VulkanHppGenerator::appendCommandValue( std::string &       str,
                                             std::string const & name,
                                             CommandData const & commandData,
                                             size_t              initialSkipCount,
                                             bool                definition ) const
{
  std::vector<size_t> returnParamIndices = determineNonConstPointerParamIndices( commandData.params );
  if ( returnParamIndices.empty() )
  {
    return appendCommandValue0Return( str, name, commandData, initialSkipCount, definition );
  }
  else
  {
    return false;
  }
}

bool VulkanHppGenerator::appendCommandValue0Return( std::string &       str,
                                                    std::string const & name,
                                                    CommandData const & commandData,
                                                    size_t              initialSkipCount,
                                                    bool                definition ) const
{
  std::map<size_t, size_t> vectorParamIndices       = determineVectorParamIndicesNew( commandData.params );
  std::vector<size_t>      constPointerParamIndices = determineConstPointerParamIndices( commandData.params );
  if ( vectorParamIndices.empty() && std::find_if( constPointerParamIndices.begin(),
                                                   constPointerParamIndices.end(),
                                                   [&commandData]( size_t idx ) {
                                                     return commandData.params[idx].type.type != "void";
                                                   } ) == constPointerParamIndices.end() )
  {
    // no vector paramter and no non-void const-pointer
    // void functions and functions returning some value with no fancy input have just standard call
    appendCommandStandard( str, name, commandData, initialSkipCount, definition );
  }
  else
  {
    // functions with some fancy input have both, standard and enhanced call
    std::vector<size_t> nonConstPointerParamIndices = determineNonConstPointerParamIndices( commandData.params );
    appendCommandStandardAndEnhanced(
      str, name, commandData, initialSkipCount, definition, vectorParamIndices, nonConstPointerParamIndices );
  }
  return true;
}

void VulkanHppGenerator::appendCommandVector( std::string &                     str,
                                              std::string const &               name,
                                              CommandData const &               commandData,
                                              size_t                            initialSkipCount,
                                              bool                              definition,
                                              std::pair<size_t, size_t> const & vectorParamIndex,
                                              std::vector<size_t> const &       returnParamIndices ) const
{
  assert( ( commandData.returnType == "VkResult" ) || ( commandData.returnType == "void" ) );

  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );

  const std::string functionTemplate = R"(
${enter}${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedWithAllocators}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
${leave})";

  str += replaceWithMap(
    functionTemplate,
    std::map<std::string, std::string>(
      { { "commandEnhanced",
          ( commandData.returnType == "VkResult" )
            ? constructCommandResultEnumerate(
                name, commandData, initialSkipCount, definition, vectorParamIndex, false )
            : constructCommandVoidEnumerate(
                name, commandData, initialSkipCount, definition, vectorParamIndex, returnParamIndices, false ) },
        { "commandEnhancedWithAllocators",
          ( commandData.returnType == "VkResult" )
            ? constructCommandResultEnumerate( name, commandData, initialSkipCount, definition, vectorParamIndex, true )
            : constructCommandVoidEnumerate(
                name, commandData, initialSkipCount, definition, vectorParamIndex, returnParamIndices, true ) },
        { "commandStandard", constructCommandStandard( name, commandData, initialSkipCount, definition ) },
        { "enter", enter },
        { "leave", leave },
        { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

void VulkanHppGenerator::appendCommandVectorChained( std::string &                    str,
                                                     std::string const &              name,
                                                     CommandData const &              commandData,
                                                     size_t                           initialSkipCount,
                                                     bool                             definition,
                                                     std::map<size_t, size_t> const & vectorParamIndices,
                                                     std::vector<size_t> const &      returnParamIndices ) const
{
  assert( ( commandData.returnType == "VkResult" ) || ( commandData.returnType == "void" ) );
  assert( vectorParamIndices.size() == 1 );

  std::string const functionTemplate = R"(
${enter}${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedWithAllocator}${newlineOnDefinition}
${commandEnhancedChained}${newlineOnDefinition}
${commandEnhancedChainedWithAllocator}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
${leave})";

  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );

  str += replaceWithMap(
    functionTemplate,
    std::map<std::string, std::string>(
      { { "commandEnhanced",
          ( commandData.returnType == "VkResult" )
            ? constructCommandResultEnumerate(
                name, commandData, initialSkipCount, definition, *vectorParamIndices.begin(), false )
            : constructCommandVoidEnumerate( name,
                                             commandData,
                                             initialSkipCount,
                                             definition,
                                             *vectorParamIndices.begin(),
                                             returnParamIndices,
                                             false ) },
        { "commandEnhancedChained",
          ( commandData.returnType == "VkResult" )
            ? constructCommandResultEnumerateChained( name,
                                                      commandData,
                                                      initialSkipCount,
                                                      definition,
                                                      *vectorParamIndices.begin(),
                                                      returnParamIndices,
                                                      false )
            : constructCommandVoidEnumerateChained( name,
                                                    commandData,
                                                    initialSkipCount,
                                                    definition,
                                                    *vectorParamIndices.begin(),
                                                    returnParamIndices,
                                                    false ) },
        { "commandEnhancedChainedWithAllocator",
          ( commandData.returnType == "VkResult" )
            ? constructCommandResultEnumerateChained(
                name, commandData, initialSkipCount, definition, *vectorParamIndices.begin(), returnParamIndices, true )
            : constructCommandVoidEnumerateChained( name,
                                                    commandData,
                                                    initialSkipCount,
                                                    definition,
                                                    *vectorParamIndices.begin(),
                                                    returnParamIndices,
                                                    true ) },
        { "commandEnhancedWithAllocator",
          ( commandData.returnType == "VkResult" )
            ? constructCommandResultEnumerate(
                name, commandData, initialSkipCount, definition, *vectorParamIndices.begin(), true )
            : constructCommandVoidEnumerate( name,
                                             commandData,
                                             initialSkipCount,
                                             definition,
                                             *vectorParamIndices.begin(),
                                             returnParamIndices,
                                             true ) },
        { "commandStandard", constructCommandStandard( name, commandData, initialSkipCount, definition ) },
        { "enter", enter },
        { "leave", leave },
        { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

void VulkanHppGenerator::appendCommandVectorDeprecated( std::string &                    str,
                                                        std::string const &              name,
                                                        CommandData const &              commandData,
                                                        size_t                           initialSkipCount,
                                                        std::map<size_t, size_t> const & vectorParamIndices,
                                                        std::vector<size_t> const &      returnParamIndices,
                                                        bool                             definition ) const
{
  assert( commandData.returnType == "VkResult" );
  assert( vectorParamIndices.size() == 2 );

  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );
  assert( enter.empty() );

  const std::string functionTemplate = R"(
${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhancedDeprecated}${newlineOnDefinition}
${commandEnhancedWithAllocatorsDeprecated}${newlineOnDefinition}
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedWithAllocators}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

  str += replaceWithMap(
    functionTemplate,
    std::map<std::string, std::string>(
      { { "commandEnhanced",
          constructCommandResultEnumerateTwoVectors(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndices, false ) },
        { "commandEnhancedDeprecated",
          constructCommandResultEnumerateTwoVectorsDeprecated(
            name, commandData, initialSkipCount, definition, vectorParamIndices, false ) },
        { "commandEnhancedWithAllocators",
          constructCommandResultEnumerateTwoVectors(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndices, true ) },
        { "commandEnhancedWithAllocatorsDeprecated",
          constructCommandResultEnumerateTwoVectorsDeprecated(
            name, commandData, initialSkipCount, definition, vectorParamIndices, true ) },
        { "commandStandard", constructCommandStandard( name, commandData, initialSkipCount, definition ) },
        { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

void VulkanHppGenerator::appendCommandVectorSingular( std::string &                    str,
                                                      std::string const &              name,
                                                      CommandData const &              commandData,
                                                      size_t                           initialSkipCount,
                                                      std::map<size_t, size_t> const & vectorParamIndices,
                                                      size_t                           returnParamIndex,
                                                      bool                             definition ) const
{
  assert( commandData.returnType == "VkResult" );

  std::string const functionTemplate = R"(
${enter}${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedWithAllocators}${newlineOnDefinition}
${commandEnhancedSingular}${newlineOnDefinition}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
${leave})";

  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );

  str += replaceWithMap(
    functionTemplate,
    std::map<std::string, std::string>(
      { { "commandEnhanced",
          constructCommandResultGetVector(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex, false ) },
        { "commandEnhancedSingular",
          constructCommandResultGetVectorSingular(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex ) },
        { "commandEnhancedWithAllocators",
          constructCommandResultGetVector(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex, true ) },
        { "commandStandard", constructCommandStandard( name, commandData, initialSkipCount, definition ) },
        { "enter", enter },
        { "leave", leave },
        { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

void VulkanHppGenerator::appendCommandVectorSingularUnique( std::string &                    str,
                                                            std::string const &              name,
                                                            CommandData const &              commandData,
                                                            size_t                           initialSkipCount,
                                                            std::map<size_t, size_t> const & vectorParamIndices,
                                                            size_t                           returnParamIndex,
                                                            bool                             definition ) const
{
  assert( commandData.returnType == "VkResult" );

  std::string const functionTemplate = R"(
${enter}${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedWithAllocators}${newlineOnDefinition}
${commandEnhancedSingular}${newlineOnDefinition}
#  ifndef VULKAN_HPP_NO_SMART_HANDLE
${commandEnhancedUnique}${newlineOnDefinition}
${commandEnhancedUniqueWithAllocators}${newlineOnDefinition}
${commandEnhancedUniqueSingular}
#  endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
${leave})";

  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );

  str += replaceWithMap(
    functionTemplate,
    std::map<std::string, std::string>(
      { { "commandEnhanced",
          constructCommandResultGetVectorOfHandles(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex, false ) },
        { "commandEnhancedSingular",
          constructCommandResultGetVectorOfHandlesSingular(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex ) },
        { "commandEnhancedUnique",
          constructCommandResultGetVectorOfHandlesUnique(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex, false ) },
        { "commandEnhancedUniqueSingular",
          constructCommandResultGetVectorOfHandlesUniqueSingular(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex ) },
        { "commandEnhancedUniqueWithAllocators",
          constructCommandResultGetVectorOfHandlesUnique(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex, true ) },
        { "commandEnhancedWithAllocators",
          constructCommandResultGetVectorOfHandles(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex, true ) },
        { "commandStandard", constructCommandStandard( name, commandData, initialSkipCount, definition ) },
        { "enter", enter },
        { "leave", leave },
        { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

void VulkanHppGenerator::appendCommandVectorUnique( std::string &                    str,
                                                    std::string const &              name,
                                                    CommandData const &              commandData,
                                                    size_t                           initialSkipCount,
                                                    std::map<size_t, size_t> const & vectorParamIndices,
                                                    size_t                           returnParamIndex,
                                                    bool                             definition ) const
{
  assert( commandData.returnType == "VkResult" );

  std::string const functionTemplate = R"(
${enter}${commandStandard}${newlineOnDefinition}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
${commandEnhanced}${newlineOnDefinition}
${commandEnhancedWithAllocators}${newlineOnDefinition}
#  ifndef VULKAN_HPP_NO_SMART_HANDLE
${commandEnhancedUnique}${newlineOnDefinition}
${commandEnhancedUniqueWithAllocators}
#  endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
${leave})";

  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );

  str += replaceWithMap(
    functionTemplate,
    std::map<std::string, std::string>(
      { { "commandEnhanced",
          constructCommandResultGetVectorOfHandles(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex, false ) },
        { "commandEnhancedUnique",
          constructCommandResultGetVectorOfHandlesUnique(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex, false ) },
        { "commandEnhancedUniqueWithAllocators",
          constructCommandResultGetVectorOfHandlesUnique(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex, true ) },
        { "commandEnhancedWithAllocators",
          constructCommandResultGetVectorOfHandles(
            name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex, true ) },
        { "commandStandard", constructCommandStandard( name, commandData, initialSkipCount, definition ) },
        { "enter", enter },
        { "leave", leave },
        { "newlineOnDefinition", definition ? "\n" : "" } } ) );
}

void VulkanHppGenerator::appendDispatchLoaderDynamic( std::string & str )
{
  str += R"(
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
  class DynamicLoader
  {
  public:
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
    DynamicLoader( std::string const & vulkanLibraryName = {} ) VULKAN_HPP_NOEXCEPT
#  else
    DynamicLoader( std::string const & vulkanLibraryName = {} )
#  endif
    {
      if ( !vulkanLibraryName.empty() )
      {
#  if defined( __linux__ ) || defined( __APPLE__ ) || defined( __QNXNTO__ ) || defined(__Fuchsia__)
        m_library = dlopen( vulkanLibraryName.c_str(), RTLD_NOW | RTLD_LOCAL );
#  elif defined( _WIN32 )
        m_library = ::LoadLibraryA( vulkanLibraryName.c_str() );
#  else
#    error unsupported platform
#  endif
      }
      else
      {
#  if defined( __linux__ ) || defined( __QNXNTO__ ) || defined(__Fuchsia__)
        m_library = dlopen( "libvulkan.so", RTLD_NOW | RTLD_LOCAL );
        if ( m_library == nullptr )
        {
          m_library = dlopen( "libvulkan.so.1", RTLD_NOW | RTLD_LOCAL );
        }
#  elif defined( __APPLE__ )
        m_library = dlopen( "libvulkan.dylib", RTLD_NOW | RTLD_LOCAL );
#  elif defined( _WIN32 )
        m_library = ::LoadLibraryA( "vulkan-1.dll" );
#  else
#    error unsupported platform
#  endif
      }

#ifndef VULKAN_HPP_NO_EXCEPTIONS
      if ( m_library == nullptr )
      {
        // NOTE there should be an InitializationFailedError, but msvc insists on the symbol does not exist within the scope of this function.
        throw std::runtime_error( "Failed to load vulkan library!" );
      }
#endif
    }

    DynamicLoader( DynamicLoader const & ) = delete;

    DynamicLoader( DynamicLoader && other ) VULKAN_HPP_NOEXCEPT : m_library(other.m_library)
    {
      other.m_library = nullptr;
    }

    DynamicLoader &operator=( DynamicLoader const & ) = delete;

    DynamicLoader &operator=( DynamicLoader && other ) VULKAN_HPP_NOEXCEPT
    {
      std::swap(m_library, other.m_library);
      return *this;
    }

    ~DynamicLoader() VULKAN_HPP_NOEXCEPT
    {
      if ( m_library )
      {
#  if defined( __linux__ ) || defined( __APPLE__ ) || defined( __QNXNTO__ ) || defined(__Fuchsia__)
        dlclose( m_library );
#  elif defined( _WIN32 )
        ::FreeLibrary( m_library );
#  else
#    error unsupported platform
#  endif
      }
    }

    template <typename T>
    T getProcAddress( const char* function ) const VULKAN_HPP_NOEXCEPT
    {
#  if defined( __linux__ ) || defined( __APPLE__ ) || defined( __QNXNTO__ ) || defined(__Fuchsia__)
      return (T)dlsym( m_library, function );
#  elif defined( _WIN32 )
      return (T)::GetProcAddress( m_library, function );
#  else
#    error unsupported platform
#  endif
    }

    bool success() const VULKAN_HPP_NOEXCEPT { return m_library != nullptr; }

  private:
#  if defined( __linux__ ) || defined( __APPLE__ ) || defined( __QNXNTO__ ) || defined(__Fuchsia__)
    void * m_library;
#  elif defined( _WIN32 )
    ::HINSTANCE m_library;
#  else
#    error unsupported platform
#  endif
  };
#endif

)";
  str += R"(
  class DispatchLoaderDynamic
  {
  public:
    using PFN_dummy = void ( * )();

)";

  std::string emptyFunctions;
  std::string deviceFunctions;
  std::string deviceFunctionsInstance;
  std::string instanceFunctions;
  for ( auto const & command : m_commands )
  {
    appendDispatchLoaderDynamicCommand(
      str, emptyFunctions, deviceFunctions, deviceFunctionsInstance, instanceFunctions, command.first, command.second );
  }

  // append initialization function to fetch function pointers
  str += R"(
  public:
    DispatchLoaderDynamic() VULKAN_HPP_NOEXCEPT = default;
    DispatchLoaderDynamic( DispatchLoaderDynamic const & rhs ) VULKAN_HPP_NOEXCEPT = default;

#if !defined( VK_NO_PROTOTYPES )
    // This interface is designed to be used for per-device function pointers in combination with a linked vulkan library.
    template <typename DynamicLoader>
    void init(VULKAN_HPP_NAMESPACE::Instance const & instance, VULKAN_HPP_NAMESPACE::Device const & device, DynamicLoader const & dl) VULKAN_HPP_NOEXCEPT
    {
      PFN_vkGetInstanceProcAddr getInstanceProcAddr = dl.template getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
      PFN_vkGetDeviceProcAddr getDeviceProcAddr = dl.template getProcAddress<PFN_vkGetDeviceProcAddr>("vkGetDeviceProcAddr");
      init(static_cast<VkInstance>(instance), getInstanceProcAddr, static_cast<VkDevice>(device), device ? getDeviceProcAddr : nullptr);
    }

    // This interface is designed to be used for per-device function pointers in combination with a linked vulkan library.
    template <typename DynamicLoader
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
      = VULKAN_HPP_NAMESPACE::DynamicLoader
#endif
    >
    void init(VULKAN_HPP_NAMESPACE::Instance const & instance, VULKAN_HPP_NAMESPACE::Device const & device) VULKAN_HPP_NOEXCEPT
    {
      static DynamicLoader dl;
      init(instance, device, dl);
    }
#endif // !defined( VK_NO_PROTOTYPES )

    DispatchLoaderDynamic(PFN_vkGetInstanceProcAddr getInstanceProcAddr) VULKAN_HPP_NOEXCEPT
    {
      init(getInstanceProcAddr);
    }

    void init( PFN_vkGetInstanceProcAddr getInstanceProcAddr ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT(getInstanceProcAddr);

      vkGetInstanceProcAddr = getInstanceProcAddr;
)";

  str += emptyFunctions;

  str += R"(    }

    // This interface does not require a linked vulkan library.
    DispatchLoaderDynamic( VkInstance                instance,
                           PFN_vkGetInstanceProcAddr getInstanceProcAddr,
                           VkDevice                  device            = {},
                           PFN_vkGetDeviceProcAddr   getDeviceProcAddr = nullptr ) VULKAN_HPP_NOEXCEPT
    {
      init( instance, getInstanceProcAddr, device, getDeviceProcAddr );
    }

    // This interface does not require a linked vulkan library.
    void init( VkInstance                instance,
               PFN_vkGetInstanceProcAddr getInstanceProcAddr,
               VkDevice                  device              = {},
               PFN_vkGetDeviceProcAddr /*getDeviceProcAddr*/ = nullptr ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT(instance && getInstanceProcAddr);
      vkGetInstanceProcAddr = getInstanceProcAddr;
      init( VULKAN_HPP_NAMESPACE::Instance(instance) );
      if (device) {
        init( VULKAN_HPP_NAMESPACE::Device(device) );
      }
    }

    void init( VULKAN_HPP_NAMESPACE::Instance instanceCpp ) VULKAN_HPP_NOEXCEPT
    {
      VkInstance instance = static_cast<VkInstance>(instanceCpp);
)";

  str += instanceFunctions;
  str += deviceFunctionsInstance;
  str += "    }\n\n";
  str += "    void init( VULKAN_HPP_NAMESPACE::Device deviceCpp ) VULKAN_HPP_NOEXCEPT\n    {\n";
  str += "      VkDevice device = static_cast<VkDevice>(deviceCpp);\n";
  str += deviceFunctions;
  str += R"(    }
  };

)";
}

void VulkanHppGenerator::appendDispatchLoaderStatic( std::string & str )
{
  str += R"(
#if !defined( VK_NO_PROTOTYPES )
  class DispatchLoaderStatic
  {
  public:)";

  std::set<std::string> listedCommands;
  for ( auto const & feature : m_features )
  {
    str += "\n    //=== " + feature.first + " ===\n";
    for ( auto const & command : feature.second.commands )
    {
      assert( listedCommands.find( command ) == listedCommands.end() );
      listedCommands.insert( command );

      auto commandIt = m_commands.find( command );
      assert( commandIt != m_commands.end() );
      str += "\n";
      appendStaticCommand( str, *commandIt );
    }
  }

  for ( auto const & extIt : m_extensionsByNumber )
  {
    if ( !extIt.second->second.commands.empty() )
    {
      std::string firstCommandName = *extIt.second->second.commands.begin();
      auto        commandIt        = m_commands.find( firstCommandName );
      assert( commandIt != m_commands.end() );
      std::string referencedIn = commandIt->second.referencedIn;

      std::string enter, leave;
      std::tie( enter, leave ) = generateProtection( referencedIn );
      if ( !enter.empty() )
      {
        enter = "\n" + enter;
      }
      str += enter + "\n    //=== " + extIt.second->first + " ===\n";
      for ( auto const & commandName : extIt.second->second.commands )
      {
        // some commands are listed for multiple extensions !
        if ( listedCommands.find( commandName ) == listedCommands.end() )
        {
          listedCommands.insert( commandName );

          commandIt = m_commands.find( commandName );
          assert( commandIt != m_commands.end() );
          assert( commandIt->second.referencedIn == referencedIn );

          str += "\n";
          appendStaticCommand( str, *commandIt );
        }
      }
      str += leave;
    }
  }

  str += "  };\n#endif\n";
}

void VulkanHppGenerator::appendDispatchLoaderDefault( std::string & str )
{
  str +=
    "\n"
    R"(  class DispatchLoaderDynamic;
#if !defined(VULKAN_HPP_DISPATCH_LOADER_DYNAMIC)
# if defined( VK_NO_PROTOTYPES )
#  define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
# else
#  define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 0
# endif
#endif

#if !defined( VULKAN_HPP_STORAGE_API )
#  if defined( VULKAN_HPP_STORAGE_SHARED )
#    if defined( _MSC_VER )
#      if defined( VULKAN_HPP_STORAGE_SHARED_EXPORT )
#        define VULKAN_HPP_STORAGE_API __declspec( dllexport )
#      else
#        define VULKAN_HPP_STORAGE_API __declspec( dllimport )
#      endif
#    elif defined( __clang__ ) || defined( __GNUC__ )
#      if defined( VULKAN_HPP_STORAGE_SHARED_EXPORT )
#        define VULKAN_HPP_STORAGE_API __attribute__( ( visibility( "default" ) ) )
#      else
#        define VULKAN_HPP_STORAGE_API
#      endif
#    else
#      define VULKAN_HPP_STORAGE_API
#      pragma warning Unknown import / export semantics
#    endif
#  else
#    define VULKAN_HPP_STORAGE_API
#  endif
#endif

#if !defined(VULKAN_HPP_DEFAULT_DISPATCHER)
# if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
#  define VULKAN_HPP_DEFAULT_DISPATCHER ::VULKAN_HPP_NAMESPACE::defaultDispatchLoaderDynamic
#  define VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE namespace VULKAN_HPP_NAMESPACE { VULKAN_HPP_STORAGE_API DispatchLoaderDynamic defaultDispatchLoaderDynamic; }
  extern VULKAN_HPP_STORAGE_API DispatchLoaderDynamic defaultDispatchLoaderDynamic;
# else
#  define VULKAN_HPP_DEFAULT_DISPATCHER ::VULKAN_HPP_NAMESPACE::DispatchLoaderStatic()
#  define VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
# endif
#endif

#if !defined(VULKAN_HPP_DEFAULT_DISPATCHER_TYPE)
# if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
  #define VULKAN_HPP_DEFAULT_DISPATCHER_TYPE ::VULKAN_HPP_NAMESPACE::DispatchLoaderDynamic
# else
#  define VULKAN_HPP_DEFAULT_DISPATCHER_TYPE ::VULKAN_HPP_NAMESPACE::DispatchLoaderStatic
# endif
#endif

#if defined( VULKAN_HPP_NO_DEFAULT_DISPATCHER )
#  define VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT
#  define VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT
#  define VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT
#else
#  define VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT         = {}
#  define VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT = nullptr
#  define VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT       = VULKAN_HPP_DEFAULT_DISPATCHER
#endif
)";
}

bool VulkanHppGenerator::appendCommandVoid( std::string &       str,
                                            std::string const & name,
                                            CommandData const & commandData,
                                            size_t              initialSkipCount,
                                            bool                definition ) const
{
  std::vector<size_t> returnParamIndices = determineNonConstPointerParamIndices( commandData.params );
  switch ( returnParamIndices.size() )
  {
    case 0: return appendCommandVoid0Return( str, name, commandData, initialSkipCount, definition );
    case 1:
      return appendCommandVoid1Return( str, name, commandData, initialSkipCount, definition, returnParamIndices[0] );
    case 2: return appendCommandVoid2Return( str, name, commandData, initialSkipCount, definition, returnParamIndices );
    default: return false;
  }
}

bool VulkanHppGenerator::appendCommandVoid0Return( std::string &       str,
                                                   std::string const & name,
                                                   CommandData const & commandData,
                                                   size_t              initialSkipCount,
                                                   bool                definition ) const
{
  std::map<size_t, size_t> vectorParamIndices       = determineVectorParamIndicesNew( commandData.params );
  std::vector<size_t>      constPointerParamIndices = determineConstPointerParamIndices( commandData.params );
  if ( vectorParamIndices.empty() && std::find_if( constPointerParamIndices.begin(),
                                                   constPointerParamIndices.end(),
                                                   [&commandData]( size_t idx ) {
                                                     return commandData.params[idx].type.type != "void";
                                                   } ) == constPointerParamIndices.end() )
  {
    // no vector paramter and no non-void const-pointer
    // void functions and functions returning some value with no fancy input have just standard call
    appendCommandStandard( str, name, commandData, initialSkipCount, definition );
  }
  else
  {
    // functions with some fancy input have both, standard and enhanced call
    appendCommandStandardAndEnhanced( str, name, commandData, initialSkipCount, definition, vectorParamIndices, {} );
  }
  return true;
}

bool VulkanHppGenerator::appendCommandVoid1Return( std::string &       str,
                                                   std::string const & name,
                                                   CommandData const & commandData,
                                                   size_t              initialSkipCount,
                                                   bool                definition,
                                                   size_t              returnParamIndex ) const
{
  bool                     appendedFunction   = false;
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndicesNew( commandData.params );
  if ( isHandleType( commandData.params[returnParamIndex].type.type ) )
  {
    // get handle(s)
    auto returnVectorParamIt = vectorParamIndices.find( returnParamIndex );
    if ( returnVectorParamIt == vectorParamIndices.end() )
    {
      // the return parameter is not a vector -> get just one handle
      if ( beginsWith( name, "vkGet" ) )
      {
        // it's a handle type, but without construction and destruction function; it's just get
        appendCommandStandardAndEnhanced(
          str, name, commandData, initialSkipCount, definition, vectorParamIndices, { returnParamIndex } );
        appendedFunction = true;
      }
    }
  }
  else if ( isStructureChainAnchor( commandData.params[returnParamIndex].type.type ) )
  {
    auto returnVectorParamIt = vectorParamIndices.find( returnParamIndex );
    if ( returnVectorParamIt == vectorParamIndices.end() )
    {
      // provide standard, enhanced, and chained call
      appendCommandChained(
        str, name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndex );
      appendedFunction = true;
    }
  }
  else
  {
    auto returnVectorParamIt = vectorParamIndices.find( returnParamIndex );
    if ( returnVectorParamIt == vectorParamIndices.end() )
    {
      appendCommandStandardAndEnhanced(
        str, name, commandData, initialSkipCount, definition, vectorParamIndices, { returnParamIndex } );
      appendedFunction = true;
    }
  }
  return appendedFunction;
}

bool VulkanHppGenerator::appendCommandVoid2Return( std::string &               str,
                                                   std::string const &         name,
                                                   CommandData const &         commandData,
                                                   size_t                      initialSkipCount,
                                                   bool                        definition,
                                                   std::vector<size_t> const & returnParamIndices ) const
{
  assert( returnParamIndices.size() == 2 );

  bool                     appendedFunction   = false;
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndicesNew( commandData.params );
  if ( !isHandleType( commandData.params[returnParamIndices[0]].type.type ) &&
       !isStructureChainAnchor( commandData.params[returnParamIndices[0]].type.type ) )
  {
    if ( isStructureChainAnchor( commandData.params[returnParamIndices[1]].type.type ) )
    {
      appendCommandVectorChained(
        str, name, commandData, initialSkipCount, definition, vectorParamIndices, returnParamIndices );
      appendedFunction = true;
    }
    else
    {
      // non of the return parameters is a StructureChain
      // Note: if the vector returned holds handles, the function does not create them, but just gets them
      switch ( vectorParamIndices.size() )
      {
        case 1:
          {
            // two returns but just one vector
            auto vectorParamIndexIt = vectorParamIndices.begin();
            if ( ( vectorParamIndexIt->second == returnParamIndices[0] ) &&
                 ( vectorParamIndexIt->first == returnParamIndices[1] ) )
            {
              // the size is a return value as well -> enumerate the values
              // and the vector data is not of type void
              // provide standard, enhanced, and vector calls
              appendCommandVector(
                str, name, commandData, initialSkipCount, definition, *vectorParamIndexIt, returnParamIndices );
              appendedFunction = true;
            }
          }
          break;
      }
    }
  }
  return appendedFunction;
}

void VulkanHppGenerator::appendDispatchLoaderDynamicCommand( std::string &       str,
                                                             std::string &       emptyFunctions,
                                                             std::string &       deviceFunctions,
                                                             std::string &       deviceFunctionsInstance,
                                                             std::string &       instanceFunctions,
                                                             std::string const & commandName,
                                                             CommandData const & commandData )
{
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandData.referencedIn );
  std::string command      = "    PFN_" + commandName + " " + commandName + " = 0;\n";
  if ( !enter.empty() )
  {
    command = enter + command + "#else\n    PFN_dummy placeholder_dont_call_" + commandName + " = 0;\n" + leave;
  }
  str += command;

  bool isDeviceFunction = !commandData.handle.empty() && !commandData.params.empty() &&
                          ( m_handles.find( commandData.params[0].type.type ) != m_handles.end() ) &&
                          ( commandData.params[0].type.type != "VkInstance" ) &&
                          ( commandData.params[0].type.type != "VkPhysicalDevice" );

  if ( commandData.handle.empty() )
  {
    assert( commandData.alias.empty() );
    emptyFunctions += enter + "      " + commandName + " = PFN_" + commandName + "( vkGetInstanceProcAddr( NULL, \"" +
                      commandName + "\" ) );\n" + leave;
  }
  else if ( isDeviceFunction )
  {
    deviceFunctions += enter + "      " + commandName + " = PFN_" + commandName + "( vkGetDeviceProcAddr( device, \"" +
                       commandName + "\" ) );\n";
    deviceFunctionsInstance += enter + "      " + commandName + " = PFN_" + commandName +
                               "( vkGetInstanceProcAddr( instance, \"" + commandName + "\" ) );\n";
    // if this is an alias'ed function, use it as a fallback for the original one
    if ( !commandData.alias.empty() )
    {
      deviceFunctions += "      if ( !" + commandData.alias + " ) " + commandData.alias + " = " + commandName + ";\n";
      deviceFunctionsInstance +=
        "      if ( !" + commandData.alias + " ) " + commandData.alias + " = " + commandName + ";\n";
    }
    deviceFunctions += leave;
    deviceFunctionsInstance += leave;
  }
  else
  {
    instanceFunctions += enter + "      " + commandName + " = PFN_" + commandName +
                         "( vkGetInstanceProcAddr( instance, \"" + commandName + "\" ) );\n";
    // if this is an alias'ed function, use it as a fallback for the original one
    if ( !commandData.alias.empty() )
    {
      instanceFunctions += "      if ( !" + commandData.alias + " ) " + commandData.alias + " = " + commandName + ";\n";
    }
    instanceFunctions += leave;
  }
}

void VulkanHppGenerator::appendEnum( std::string & str, std::pair<std::string, EnumData> const & enumData ) const
{
  str += "  enum class " + stripPrefix( enumData.first, "Vk" );
  if ( enumData.second.isBitmask )
  {
    auto bitmaskIt =
      std::find_if( m_bitmasks.begin(),
                    m_bitmasks.end(),
                    [&enumData]( auto const & bitmask ) { return bitmask.second.requirements == enumData.first; } );
    assert( bitmaskIt != m_bitmasks.end() );
    str += " : " + bitmaskIt->first;
  }

  std::string enumList, previousEnter, previousLeave;
  for ( auto const & value : enumData.second.values )
  {
    std::string enter, leave;
    if ( !value.extension.empty() )
    {
      std::tie( enter, leave ) = generateProtection( value.extension );
    }
    if ( !leave.empty() )
    {
      assert( leave.back() == '\n' );
      leave.pop_back();
      leave = "\n" + leave;
    }
    enumList += ( ( previousEnter != enter ) ? ( previousLeave + "\n" + enter ) : "\n" ) + "    " + value.vkValue +
                " = " + value.vulkanValue + ",";

    previousEnter = enter;
    previousLeave = leave;
  }
  enumList += previousLeave;
  for ( auto const & alias : enumData.second.aliases )
  {
    // make sure to only list alias values that differ from all non-alias values
    if ( std::find_if( enumData.second.values.begin(),
                       enumData.second.values.end(),
                       [&alias]( EnumValueData const & evd )
                       { return alias.second.vkValue == evd.vkValue; } ) == enumData.second.values.end() )
    {
#if !defined( NDEBUG )
      auto enumIt =
        std::find_if( enumData.second.values.begin(),
                      enumData.second.values.end(),
                      [&alias]( EnumValueData const & evd ) { return alias.second.vulkanValue == evd.vulkanValue; } );
      if ( enumIt == enumData.second.values.end() )
      {
        auto aliasIt = enumData.second.aliases.find( alias.second.vulkanValue );
        assert( aliasIt != enumData.second.aliases.end() );
        enumIt = std::find_if( enumData.second.values.begin(),
                               enumData.second.values.end(),
                               [&aliasIt]( EnumValueData const & evd )
                               { return aliasIt->second.vulkanValue == evd.vulkanValue; } );
      }
      assert( enumIt != enumData.second.values.end() );
      assert( enumIt->extension.empty() || generateProtection( enumIt->extension ).first.empty() );
#endif
      enumList += "\n    " + alias.second.vkValue + " = " + alias.first + ",";
    }
  }
  if ( !enumList.empty() )
  {
    size_t pos = enumList.rfind( ',' );
    assert( pos != std::string::npos );
    enumList.erase( pos, 1 );
  }

  str += "\n  {" + enumList + "\n  };\n";

  if ( !enumData.second.alias.empty() )
  {
    str +=
      "  using " + stripPrefix( enumData.second.alias, "Vk" ) + " = " + stripPrefix( enumData.first, "Vk" ) + ";\n";
  }
}

void VulkanHppGenerator::appendEnums( std::string & str ) const
{
  // start with toHexString, which is used in all the to_string functions here!
  str += R"(
  VULKAN_HPP_INLINE std::string toHexString( uint32_t value )
  {
    std::stringstream stream;
    stream << std::hex << value;
    return stream.str();
  }

  //=============
  //=== ENUMs ===
  //=============
)";

  std::set<std::string> listedEnums;
  for ( auto const & feature : m_features )
  {
    str += "\n  //=== " + feature.first + " ===\n";
    for ( auto const & type : feature.second.types )
    {
      auto enumIt = m_enums.find( type.first );
      if ( enumIt != m_enums.end() )
      {
        assert( listedEnums.find( type.first ) == listedEnums.end() );
        listedEnums.insert( type.first );

        str += "\n";
        appendEnum( str, *enumIt );
        appendEnumToString( str, *enumIt );
      }
    }
  }

  for ( auto const & extIt : m_extensionsByNumber )
  {
    std::vector<std::map<std::string, EnumData>::const_iterator> enumIts;
    for ( auto const & type : extIt.second->second.types )
    {
      auto enumIt = m_enums.find( type );
      if ( enumIt != m_enums.end() )
      {
        enumIts.push_back( enumIt );
      }
    }

    if ( !enumIts.empty() )
    {
      std::string enter, leave;
      std::tie( enter, leave ) = generateProtection( enumIts.front()->first, !enumIts.front()->second.alias.empty() );
      if ( !enter.empty() )
      {
        enter = "\n" + enter;
      }
      str += enter + "\n  //=== " + extIt.second->first + " ===\n";
      for ( auto enumIt : enumIts )
      {
        assert( listedEnums.find( enumIt->first ) == listedEnums.end() );
        listedEnums.insert( enumIt->first );

        str += "\n";
        appendEnum( str, *enumIt );
        appendEnumToString( str, *enumIt );
      }
      str += leave;
    }
  }

  str += R"(
  template<ObjectType value>
  struct cpp_type
  {};
)";
}

void VulkanHppGenerator::appendEnumInitializer( std::string &                      str,
                                                TypeInfo const &                   type,
                                                std::vector<std::string> const &   arraySizes,
                                                std::vector<EnumValueData> const & values ) const
{
  // enum arguments might need special initialization
  assert( type.prefix.empty() && !values.empty() );
  std::string value = "VULKAN_HPP_NAMESPACE::" + stripPrefix( type.type, "Vk" ) + "::" + values.front().vkValue;
  if ( arraySizes.empty() )
  {
    str += value;
  }
  else
  {
    assert( arraySizes.size() == 1 );
    auto constIt = m_constants.find( arraySizes[0] );
    int  count   = std::stoi( ( constIt == m_constants.end() ) ? arraySizes[0] : constIt->second );
    assert( 1 < count );
    str += "{ { " + value;
    for ( int i = 1; i < count; i++ )
    {
      str += ", " + value;
    }
    str += " } }";
  }
}

void VulkanHppGenerator::appendEnumToString( std::string &                            str,
                                             std::pair<std::string, EnumData> const & enumData ) const
{
  std::string enumName = stripPrefix( enumData.first, "Vk" );

  str +=
    "\n"
    "  VULKAN_HPP_INLINE std::string to_string( " +
    enumName + ( enumData.second.values.empty() ? "" : " value" ) +
    " )\n"
    "  {";

  if ( enumData.second.values.empty() )
  {
    str +=
      "\n"
      "    return \"(void)\";\n";
  }
  else
  {
    str +=
      "\n"
      "    switch ( value )\n"
      "    {\n";
    std::string previousEnter, previousLeave;
    for ( auto const & value : enumData.second.values )
    {
      std::string enter, leave;
      if ( !value.extension.empty() )
      {
        std::tie( enter, leave ) = generateProtection( value.extension );
      }
      str += ( ( previousEnter != enter ) ? ( previousLeave + enter ) : "" ) + "      case " + enumName +
             "::" + value.vkValue + " : return \"" + value.vkValue.substr( 1 ) + "\";\n";
      previousEnter = enter;
      previousLeave = leave;
    }
    str +=
      previousLeave +
      "      default: return \"invalid ( \" + VULKAN_HPP_NAMESPACE::toHexString( static_cast<uint32_t>( value ) ) + \" )\";\n"
      "    }\n";
  }

  str += "  }\n";
}

std::string VulkanHppGenerator::appendFunctionBodyEnhancedLocalReturnVariable( std::string &       str,
                                                                               std::string const & indentation,
                                                                               CommandData const & commandData,
                                                                               size_t              returnParamIndex,
                                                                               std::string const & enhancedReturnType,
                                                                               bool                withAllocator ) const
{
  std::string pureReturnType = stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" );
  std::string returnName     = startLowerCase( stripPrefix( commandData.params[returnParamIndex].name, "p" ) );

  // there is a returned parameter -> we need a local variable to hold that value
  assert( stripPrefix( commandData.returnType, "Vk" ) != enhancedReturnType );
  // the returned parameter is somehow enhanced by us
  str += indentation + "  ";
  // in non-singular case, use the enhanced type for the return variable (like vector<...>)
  str += enhancedReturnType + " " + returnName;

  if ( withAllocator )
  {
    str += "( vectorAllocator )";
  }
  str += ";\n";

  return returnName;
}

void VulkanHppGenerator::appendFunctionBodyEnhancedMultiVectorSizeCheck(
  std::string &                    str,
  std::string const &              indentation,
  std::string const &              name,
  CommandData const &              commandData,
  size_t                           initialSkipCount,
  size_t                           returnParamIndex,
  std::map<size_t, size_t> const & vectorParamIndices ) const
{
  std::string const sizeCheckTemplate =
    R"#(#ifdef VULKAN_HPP_NO_EXCEPTIONS
${i}  VULKAN_HPP_ASSERT( ${firstVectorName}.size() == ${secondVectorName}.size() );
#else
${i}  if ( ${firstVectorName}.size() != ${secondVectorName}.size() )
${i}  {
${i}    throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::${className}::${commandName}: ${firstVectorName}.size() != ${secondVectorName}.size()" );
${i}  }
#endif  /*VULKAN_HPP_NO_EXCEPTIONS*/
)#";

  // add some error checks if multiple vectors need to have the same size
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  for ( std::map<size_t, size_t>::const_iterator it0 = vectorParamIndices.begin(); it0 != vectorParamIndices.end();
        ++it0 )
  {
    if ( it0->first != returnParamIndex )
    {
      for ( std::map<size_t, size_t>::const_iterator it1 = std::next( it0 ); it1 != vectorParamIndices.end(); ++it1 )
      {
        if ( ( it1->first != returnParamIndex ) && ( it0->second == it1->second ) )
        {
          str += replaceWithMap(
            sizeCheckTemplate,
            std::map<std::string, std::string>(
              { { "firstVectorName", startLowerCase( stripPrefix( commandData.params[it0->first].name, "p" ) ) },
                { "secondVectorName", startLowerCase( stripPrefix( commandData.params[it1->first].name, "p" ) ) },
                { "className", commandData.params[initialSkipCount - 1].type.type },
                { "commandName", commandName },
                { "i", indentation } } ) );
        }
      }
    }
  }
}

void VulkanHppGenerator::appendFunctionBodyEnhancedReturnResultValue( std::string &       str,
                                                                      std::string const & indentation,
                                                                      std::string const & returnName,
                                                                      std::string const & name,
                                                                      CommandData const & commandData,
                                                                      size_t              initialSkipCount,
                                                                      size_t              returnParamIndex,
                                                                      bool                twoStep ) const
{
  std::string type = ( returnParamIndex != INVALID_INDEX ) ? commandData.params[returnParamIndex].type.type : "";
  std::string returnVectorName = ( returnParamIndex != INVALID_INDEX )
                                   ? stripPostfix( stripPrefix( commandData.params[returnParamIndex].name, "p" ), "s" )
                                   : "";
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );

  assert( commandData.returnType != "void" );

  str += indentation + "  return createResultValue( result, ";

  // if the return type is "Result" or there is at least one success code, create the Result/Value construct to return
  if ( returnParamIndex != INVALID_INDEX )
  {
    // if there's a return parameter, list it in the Result/Value constructor
    str += returnName + ", ";
  }

  // now the function name (with full namespace) as a string
  str += "VULKAN_HPP_NAMESPACE_STRING\"::" +
         ( commandData.handle.empty() ? "" : stripPrefix( commandData.handle, "Vk" ) + "::" ) + commandName + "\"";

  if ( !twoStep && ( 1 < commandData.successCodes.size() ) )
  {
    // and for the single-step algorithms with more than one success code list them all
    str += ", { Result::" + createSuccessCode( commandData.successCodes[0], m_tags );
    for ( size_t i = 1; i < commandData.successCodes.size(); i++ )
    {
      str += ", Result::" + createSuccessCode( commandData.successCodes[i], m_tags );
    }
    str += " }";
  }

  str += " );\n";
}

void VulkanHppGenerator::appendFunctionBodyEnhancedTwoStep( std::string &                    str,
                                                            std::string const &              indentation,
                                                            std::string const &              name,
                                                            CommandData const &              commandData,
                                                            size_t                           returnParamIndex,
                                                            size_t                           templateParamIndex,
                                                            std::map<size_t, size_t> const & vectorParamIndices,
                                                            std::string const &              returnName ) const
{
  assert( ( commandData.returnType == "VkResult" ) || ( commandData.returnType == "void" ) );
  assert( returnParamIndex != INVALID_INDEX );

  // local count variable to hold the size of the vector to fill
  std::map<size_t, size_t>::const_iterator returnit = vectorParamIndices.find( returnParamIndex );
  assert( returnit != vectorParamIndices.end() && ( returnit->second != INVALID_INDEX ) );

  // take the pure type of the size parameter; strip the leading 'p' from its name for its local name
  std::string sizeName = startLowerCase( stripPrefix( commandData.params[returnit->second].name, "p" ) );
  str +=
    indentation + "  " + stripPrefix( commandData.params[returnit->second].type.type, "Vk" ) + " " + sizeName + ";\n";

  std::string const multiSuccessTemplate =
    R"(${i}  Result result;
${i}  do
${i}  {
${i}    result = static_cast<Result>( ${call1} );
${i}    if ( ( result == Result::eSuccess ) && ${sizeName} )
${i}    {
${i}      ${returnName}.resize( ${sizeName} );
${i}      result = static_cast<Result>( ${call2} );
${i}    }
${i}  } while ( result == Result::eIncomplete );
${i}  if ( result == Result::eSuccess )
${i}  {
${i}    VULKAN_HPP_ASSERT( ${sizeName} <= ${returnName}.size() );
${i}    ${returnName}.resize( ${sizeName} );
${i}  }
)";
  std::string const singleSuccessTemplate =
    R"(${i}  Result result = static_cast<Result>( ${call1} );
${i}  if ( ( result == Result::eSuccess ) && ${sizeName} )
${i}  {
${i}    ${returnName}.resize( ${sizeName} );
${i}    result = static_cast<Result>( ${call2} );
${i}  }
)";
  std::string const voidMultiCallTemplate =
    R"(${i}  ${call1};
${i}  ${returnName}.resize( ${sizeName} );
${i}  ${call2};
)";
  std::string const & selectedTemplate =
    ( commandData.returnType == "VkResult" )
      ? ( ( 1 < commandData.successCodes.size() ) ? multiSuccessTemplate : singleSuccessTemplate )
      : voidMultiCallTemplate;

  std::string call1, call2;
  appendCall( call1, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, true, true );
  appendCall( call2, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, true, false );

  str += replaceWithMap( selectedTemplate,
                         { { "sizeName", sizeName },
                           { "returnName", returnName },
                           { "call1", call1 },
                           { "call2", call2 },
                           { "i", indentation } } );
}

bool VulkanHppGenerator::appendFunctionHeaderArgumentEnhanced( std::string &                    str,
                                                               ParamData const &                param,
                                                               size_t                           paramIndex,
                                                               std::map<size_t, size_t> const & vectorParamIndices,
                                                               bool                             skip,
                                                               bool                             argEncountered,
                                                               bool                             isTemplateParam ) const
{
  if ( !skip )
  {
    if ( argEncountered )
    {
      str += ", ";
    }
    std::string strippedParameterName = startLowerCase( stripPrefix( param.name, "p" ) );

    std::map<size_t, size_t>::const_iterator it = vectorParamIndices.find( paramIndex );
    if ( it == vectorParamIndices.end() )
    {
      // the argument ist not a vector
      assert( param.type.postfix.empty() );
      // and its not a pointer -> just use its type and name here
      str += param.type.compose() + " " + param.name + constructCArraySizes( param.arraySizes );
    }
    else
    {
      // the argument is a vector
      appendFunctionHeaderArgumentEnhancedVector(
        str, param, strippedParameterName, it->second != INVALID_INDEX, isTemplateParam );
    }
    argEncountered = true;
  }
  return argEncountered;
}

void VulkanHppGenerator::appendFunctionHeaderArgumentEnhancedVector( std::string &       str,
                                                                     ParamData const &   param,
                                                                     std::string const & strippedParameterName,
                                                                     bool                hasSizeParam,
                                                                     bool                isTemplateParam ) const
{
  assert( param.type.postfix.back() == '*' );
  // it's optional, if it's marked as optional and there's no size specified
  bool optional = param.optional && !hasSizeParam;

  std::string optionalBegin = optional ? "Optional<" : "";
  std::string optionalEnd   = optional ? "> " : "";

  // use our ArrayProxy
  bool isConst = ( param.type.prefix.find( "const" ) != std::string::npos );
  str += optionalBegin + "ArrayProxy<" +
         ( isTemplateParam ? ( isConst ? "const T" : "T" ) : stripPostfix( param.type.compose(), "*" ) ) + "> const &" +
         optionalEnd + strippedParameterName;
}

void VulkanHppGenerator::appendHandle( std::string & str, std::pair<std::string, HandleData> const & handleData )
{
  assert( m_listingTypes.find( handleData.first ) == m_listingTypes.end() );
  m_listingTypes.insert( handleData.first );

  assert( m_listedTypes.find( handleData.first ) == m_listedTypes.end() );

  // first check for any handle that needs to be listed before this one
  for ( auto const & command : handleData.second.commands )
  {
    auto commandIt = m_commands.find( command );
    assert( commandIt != m_commands.end() );
    for ( auto const & parameter : commandIt->second.params )
    {
      if ( handleData.first != parameter.type.type )  // the commands use this handleData type !
      {
        appendType( str, parameter.type.type );
      }
    }
  }

  if ( handleData.first.empty() )
  {
    for ( auto const & command : handleData.second.commands )
    {
      auto commandIt = m_commands.find( command );
      assert( commandIt != m_commands.end() );
      if ( commandIt->first == "vkCreateInstance" )
      {
        // special handling for createInstance, as we need to explicitly place the forward declarations and the
        // deleter classes here
#if !defined( NDEBUG )
        auto handleIt = m_handles.find( "" );
        assert( ( handleIt != m_handles.end() ) && ( handleIt->second.childrenHandles.size() == 2 ) );
        assert( handleIt->second.childrenHandles.find( "VkInstance" ) != handleIt->second.childrenHandles.end() );
#endif

        appendUniqueTypes( str, "", { "VkInstance" } );
      }
      str += "\n";
      appendCommand( str, commandIt->first, commandIt->second, 0, false );
    }
  }
  else
  {
    // then append any forward declaration of Deleters used by this handle
    if ( !handleData.second.childrenHandles.empty() )
    {
      appendUniqueTypes( str, handleData.first, handleData.second.childrenHandles );
    }
    else if ( handleData.first == "VkPhysicalDevice" )
    {
      // special handling for class Device, as it's created from PhysicalDevice, but destroys itself
      appendUniqueTypes( str, "", { "VkDevice" } );
    }

    std::string commands;
    // list all the commands that are mapped to members of this class
    for ( auto const & command : handleData.second.commands )
    {
      auto commandIt = m_commands.find( command );
      assert( commandIt != m_commands.end() );

      std::string commandString;
      std::string commandName = determineCommandName( commandIt->first, commandIt->second.params[0].type.type, m_tags );
      commands += "\n";
      appendCommand( commands, commandIt->first, commandIt->second, 1, false );

      // special handling for destroy functions, filter out alias functions
      if ( commandIt->second.alias.empty() &&
           ( ( ( commandIt->first.substr( 2, 7 ) == "Destroy" ) && ( commandName != "destroy" ) ) ||
             ( commandIt->first.substr( 2, 4 ) == "Free" ) ||
             ( commandIt->first == "vkReleasePerformanceConfigurationINTEL" ) ) )
      {
        std::string destroyCommandString;
        CommandData commandData = commandIt->second;
        assert( ( 1 < commandData.params.size() ) && ( commandData.params[0].type.type == handleData.first ) );
        commandData.params[1].optional =
          false;  // make sure, the object to destroy/free/release is not optional in the shortened version!

        appendCommand( destroyCommandString, commandIt->first, commandData, 1, false );
        std::string shortenedName;
        if ( commandIt->first.substr( 2, 7 ) == "Destroy" )
        {
          shortenedName = "destroy";
        }
        else if ( commandIt->first.substr( 2, 4 ) == "Free" )
        {
          shortenedName = "free";
        }
        else
        {
          assert( commandIt->first == "vkReleasePerformanceConfigurationINTEL" );
          shortenedName = "release";
        }
        size_t pos = destroyCommandString.find( commandName );
        while ( pos != std::string::npos )
        {
          destroyCommandString.replace( pos, commandName.length(), shortenedName );
          pos = destroyCommandString.find( commandName, pos );
        }
        // we need to remove the default argument for the first argument, to prevent ambiguities!
        assert( 1 < commandIt->second.params.size() );
        pos =
          destroyCommandString.find( commandIt->second.params[1].name );  // skip the standard version of the function
        assert( pos != std::string::npos );
        pos = destroyCommandString.find( commandIt->second.params[1].name,
                                         pos + 1 );  // get the argument to destroy in the advanced version
        assert( pos != std::string::npos );
        pos = destroyCommandString.find( " VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT", pos );
        if ( pos != std::string::npos )
        {
          destroyCommandString.erase( pos, strlen( " VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT" ) );
        }
        commands += "\n" + destroyCommandString;
      }
    }

    static const std::string templateString = R"(
${enter}  class ${className}
  {
  public:
    using CType = Vk${className};

    static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::${objTypeEnum};
    static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType = VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::${debugReportObjectType};

  public:
    VULKAN_HPP_CONSTEXPR ${className}() = default;
    VULKAN_HPP_CONSTEXPR ${className}( std::nullptr_t ) VULKAN_HPP_NOEXCEPT
    {}
    VULKAN_HPP_TYPESAFE_EXPLICIT ${className}( Vk${className} ${memberName} ) VULKAN_HPP_NOEXCEPT
      : m_${memberName}( ${memberName} )
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    ${className} & operator=(Vk${className} ${memberName}) VULKAN_HPP_NOEXCEPT
    {
      m_${memberName} = ${memberName};
      return *this;
    }
#endif

    ${className} & operator=( std::nullptr_t ) VULKAN_HPP_NOEXCEPT
    {
      m_${memberName} = {};
      return *this;
    }

#if defined(VULKAN_HPP_HAS_SPACESHIP_OPERATOR)
    auto operator<=>( ${className} const & ) const = default;
#else
    bool operator==( ${className} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_${memberName} == rhs.m_${memberName};
    }

    bool operator!=(${className} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_${memberName} != rhs.m_${memberName};
    }

    bool operator<(${className} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_${memberName} < rhs.m_${memberName};
    }
#endif
${commands}
    VULKAN_HPP_TYPESAFE_EXPLICIT operator Vk${className}() const VULKAN_HPP_NOEXCEPT
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
  static_assert( sizeof( VULKAN_HPP_NAMESPACE::${className} ) == sizeof( Vk${className} ), "handle and wrapper have different size!" );

  template <>
  struct VULKAN_HPP_DEPRECATED("vk::cpp_type is deprecated. Use vk::CppType instead.") cpp_type<ObjectType::${objTypeEnum}>
  {
    using type = VULKAN_HPP_NAMESPACE::${className};
  };

  template <>
  struct CppType<VULKAN_HPP_NAMESPACE::ObjectType, VULKAN_HPP_NAMESPACE::ObjectType::${objTypeEnum}>
  {
    using Type = VULKAN_HPP_NAMESPACE::${className};
  };

${CppTypeFromDebugReportObjectTypeEXT}

  template <>
  struct isVulkanHandleType<VULKAN_HPP_NAMESPACE::${className}>
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool value = true;
  };
)";

    std::string className = stripPrefix( handleData.first, "Vk" );

    auto enumIt = m_enums.find( "VkDebugReportObjectTypeEXT" );
    assert( enumIt != m_enums.end() );
    auto                     valueIt                                     = std::find_if( enumIt->second.values.begin(),
                                 enumIt->second.values.end(),
                                 [&className]( EnumValueData const & evd ) { return evd.vkValue == "e" + className; } );
    static const std::string cppTypeFromDebugReportObjectTypeEXTTemplate = R"(
  template <>
  struct CppType<VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT, VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::e${className}>
  {
    using Type = VULKAN_HPP_NAMESPACE::${className};
  };
)";
    std::string              cppTypeFromDebugReportObjectTypeEXT =
      ( valueIt != enumIt->second.values.end() )
                     ? replaceWithMap( cppTypeFromDebugReportObjectTypeEXTTemplate, { { "className", className } } )
                     : "";
    std::string debugReportObjectType = ( valueIt != enumIt->second.values.end() ) ? valueIt->vkValue : "eUnknown";

    std::string enter, leave;
    std::tie( enter, leave ) = generateProtection( handleData.first, !handleData.second.alias.empty() );

    assert( !handleData.second.objTypeEnum.empty() );
    enumIt = m_enums.find( "VkObjectType" );
    assert( enumIt != m_enums.end() );
    valueIt = std::find_if( enumIt->second.values.begin(),
                            enumIt->second.values.end(),
                            [&handleData]( EnumValueData const & evd )
                            { return evd.vulkanValue == handleData.second.objTypeEnum; } );
    assert( valueIt != enumIt->second.values.end() );

    str += replaceWithMap( templateString,
                           { { "className", className },
                             { "commands", commands },
                             { "CppTypeFromDebugReportObjectTypeEXT", cppTypeFromDebugReportObjectTypeEXT },
                             { "debugReportObjectType", debugReportObjectType },
                             { "enter", enter },
                             { "memberName", startLowerCase( stripPrefix( handleData.first, "Vk" ) ) },
                             { "objTypeEnum", valueIt->vkValue } } );

    if ( !handleData.second.alias.empty() )
    {
      str += "  using " + stripPrefix( handleData.second.alias, "Vk" ) + " = " + stripPrefix( handleData.first, "Vk" ) +
             ";\n";
    }
    str += leave;
  }

  m_listingTypes.erase( handleData.first );
  m_listedTypes.insert( handleData.first );
}

void VulkanHppGenerator::appendHandles( std::string & str )
{
  for ( auto const & handle : m_handles )
  {
    if ( m_listedTypes.find( handle.first ) == m_listedTypes.end() )
    {
      assert( m_listingTypes.empty() );
      appendHandle( str, handle );
      assert( m_listingTypes.empty() );
    }
  }
}

void VulkanHppGenerator::appendHandlesCommandDefinitions( std::string & str ) const
{
  for ( auto const & handle : m_handles )
  {
    // finally the commands, that are member functions of this handle
    for ( auto const & command : handle.second.commands )
    {
      auto commandIt = m_commands.find( command );
      assert( commandIt != m_commands.end() );

      std::string strippedName = startLowerCase( stripPrefix( commandIt->first, "vk" ) );

      str += "\n";
      appendCommand( str, commandIt->first, commandIt->second, handle.first.empty() ? 0 : 1, true );

      // special handling for destroy functions, filter out alias functions
      std::string commandName = determineCommandName( commandIt->first, commandIt->second.params[0].type.type, m_tags );
      if ( commandIt->second.alias.empty() &&
           ( ( ( commandIt->first.substr( 2, 7 ) == "Destroy" ) && ( commandName != "destroy" ) ) ||
             ( commandIt->first.substr( 2, 4 ) == "Free" ) ||
             ( commandIt->first == "vkReleasePerformanceConfigurationINTEL" ) ) )
      {
        std::string destroyCommandString;
        CommandData commandData = commandIt->second;
        assert( ( 1 < commandData.params.size() ) && ( commandData.params[0].type.type == handle.first ) );
        commandData.params[1].optional =
          false;  // make sure, the object to destroy/free/release is not optional in the shortened version!

        appendCommand( destroyCommandString, commandIt->first, commandData, handle.first.empty() ? 0 : 1, true );
        std::string shortenedName;
        if ( commandIt->first.substr( 2, 7 ) == "Destroy" )
        {
          shortenedName = "destroy";
        }
        else if ( commandIt->first.substr( 2, 4 ) == "Free" )
        {
          shortenedName = "free";
        }
        else
        {
          assert( commandIt->first == "vkReleasePerformanceConfigurationINTEL" );
          shortenedName = "release";
        }
        size_t pos = destroyCommandString.find( commandName );
        while ( pos != std::string::npos )
        {
          destroyCommandString.replace( pos, commandName.length(), shortenedName );
          pos = destroyCommandString.find( commandName, pos );
        }
        // we need to remove the default argument for the first argument, to prevent ambiguities!
        assert( 1 < commandIt->second.params.size() );
        pos =
          destroyCommandString.find( commandIt->second.params[1].name );  // skip the standard version of the function
        assert( pos != std::string::npos );
        pos = destroyCommandString.find( commandIt->second.params[1].name,
                                         pos + 1 );  // get the argument to destroy in the advanced version
        assert( pos != std::string::npos );
        pos = destroyCommandString.find( " VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT", pos );
        if ( pos != std::string::npos )
        {
          destroyCommandString.erase( pos, strlen( " VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT" ) );
        }
        str += "\n" + destroyCommandString;
      }
    }
  }
}

void VulkanHppGenerator::appendHashStructures( std::string & str ) const
{
  const std::string hashTemplate = R"(  template <> struct hash<VULKAN_HPP_NAMESPACE::${type}>
  {
    std::size_t operator()(VULKAN_HPP_NAMESPACE::${type} const & ${name}) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<Vk${type}>{}(static_cast<Vk${type}>(${name}));
    }
  };
)";
  for ( auto handle : m_handles )
  {
    if ( !handle.first.empty() )
    {
      std::string enter, leave;
      std::tie( enter, leave ) = generateProtection( handle.first, !handle.second.alias.empty() );

      str += "\n" + enter;
      std::string type = stripPrefix( handle.first, "Vk" );
      std::string name = startLowerCase( type );
      str += replaceWithMap( hashTemplate, { { "name", name }, { "type", type } } );
      str += leave;
    }
  }
}

void VulkanHppGenerator::appendRAIIDispatchers( std::string & str ) const
{
  std::string contextInitializerList, deviceInitAssignments, instanceInitAssignments;
  std::string contextMembers, deviceMembers, instanceMembers;
  std::string previousEnter;
  for ( auto const & command : m_commands )
  {
    std::string enter, leave;
    std::tie( enter, leave ) = generateProtection( command.second.referencedIn );

    if ( command.second.handle.empty() )
    {
      assert( enter.empty() );
      assert( command.second.alias.empty() );
      contextInitializerList +=
        ", " + command.first + "( PFN_" + command.first + "( getProcAddr( NULL, \"" + command.first + "\" ) ) )";
      contextMembers += "      PFN_" + command.first + " " + command.first + " = 0;\n";
    }
    else if ( ( command.second.handle == "VkDevice" ) || hasParentHandle( command.second.handle, "VkDevice" ) )
    {
      deviceInitAssignments += enter + "        " + command.first + " = PFN_" + command.first +
                               "( vkGetDeviceProcAddr( device, \"" + command.first + "\" ) );\n";
      // if this is an alias'ed function, use it as a fallback for the original one
      if ( !command.second.alias.empty() )
      {
        deviceInitAssignments +=
          "        if ( !" + command.second.alias + " ) " + command.second.alias + " = " + command.first + ";\n";
      }
      deviceInitAssignments += leave;

      deviceMembers += enter + "      PFN_" + command.first + " " + command.first + " = 0;\n" + leave;
    }
    else
    {
      assert( ( command.second.handle == "VkInstance" ) || hasParentHandle( command.second.handle, "VkInstance" ) );

      instanceInitAssignments += enter + "        " + command.first + " = PFN_" + command.first +
                                 "( vkGetInstanceProcAddr( instance, \"" + command.first + "\" ) );\n";
      // if this is an alias'ed function, use it as a fallback for the original one
      if ( !command.second.alias.empty() )
      {
        instanceInitAssignments +=
          "        if ( !" + command.second.alias + " ) " + command.second.alias + " = " + command.first + ";\n";
      }
      instanceInitAssignments += leave;

      instanceMembers += enter + "      PFN_" + command.first + " " + command.first + " = 0;\n" + leave;
    }
    previousEnter = enter;
  }

  std::string contextDispatcherTemplate = R"(
    class ContextDispatcher
    {
    public:
      ContextDispatcher( PFN_vkGetInstanceProcAddr getProcAddr )
        : vkGetInstanceProcAddr( getProcAddr )${initializerList}
      {}

    public:
      PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = 0;
${members}
    };
)";

  str += replaceWithMap( contextDispatcherTemplate,
                         { { "initializerList", contextInitializerList }, { "members", contextMembers } } );

  std::string instanceDispatcherTemplate = R"(
    class InstanceDispatcher
    {
    public:
      InstanceDispatcher( PFN_vkGetInstanceProcAddr getProcAddr )
        : vkGetInstanceProcAddr( getProcAddr )
      {}

      void init( VkInstance instance )
      {
${initAssignments}
        vkGetDeviceProcAddr =
          PFN_vkGetDeviceProcAddr( vkGetInstanceProcAddr( instance, "vkGetDeviceProcAddr" ) );
      }

    public:
${members}
      PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = 0;
    };
)";

  str += replaceWithMap( instanceDispatcherTemplate,
                         { { "initAssignments", instanceInitAssignments }, { "members", instanceMembers } } );

  std::string deviceDispatcherTemplate = R"(
    class DeviceDispatcher
    {
      public:
        DeviceDispatcher( PFN_vkGetDeviceProcAddr getProcAddr )
          : vkGetDeviceProcAddr( getProcAddr )
        {}

        void init( VkDevice device )
        {
${initAssignments}
        }

      public:
${members}
    };
)";

  str += replaceWithMap( deviceDispatcherTemplate,
                         { { "initAssignments", deviceInitAssignments }, { "members", deviceMembers } } );
}

void VulkanHppGenerator::appendRAIIHandles( std::string & str, std::string & commandDefinitions )
{
  // Enum -> Type translations are always able to occur.
  str += "\n";

  // filtering out functions that are not usefull on this level of abstraction (like vkGetInstanceProcAddr)
  // and all the construction and destruction functions, as they are used differently

  std::set<std::string> specialFunctions;
  for ( auto & handle : m_handles )
  {
    if ( !handle.first.empty() )
    {
      handle.second.destructorIt = determineRAIIHandleDestructor( handle.first );
      if ( handle.second.destructorIt != m_commands.end() )
      {
        specialFunctions.insert( handle.second.destructorIt->first );
      }
      handle.second.constructorIts =
        determineRAIIHandleConstructors( handle.first, handle.second.destructorIt, specialFunctions );
    }
  }

  distributeSecondLevelCommands( specialFunctions );
  renameFunctionParameters();

  std::set<std::string> listedHandles;
  auto                  handleIt = m_handles.begin();
  assert( handleIt->first.empty() );
  appendRAIIHandleContext( str, commandDefinitions, *handleIt, specialFunctions );
  for ( ++handleIt; handleIt != m_handles.end(); ++handleIt )
  {
    appendRAIIHandle( str, commandDefinitions, *handleIt, listedHandles, specialFunctions );
  }
}

// Intended only for `enum class Result`!
void VulkanHppGenerator::appendResultExceptions( std::string & str ) const
{
  std::string templateString = R"(
${enter}  class ${className} : public SystemError
  {
  public:
    ${className}( std::string const & message )
      : SystemError( make_error_code( ${enumName}::${enumMemberName} ), message ) {}
    ${className}( char const * message )
      : SystemError( make_error_code( ${enumName}::${enumMemberName} ), message ) {}
  };
${leave})";

  auto enumData = m_enums.find( "VkResult" );
  for ( auto const & value : enumData->second.values )
  {
    if ( beginsWith( value.vkValue, "eError" ) )
    {
      std::string enter, leave;
      if ( !value.extension.empty() )
      {
        std::tie( enter, leave ) = generateProtection( value.extension );
      }
      str += replaceWithMap( templateString,
                             { { "className", stripPrefix( value.vkValue, "eError" ) + "Error" },
                               { "enter", enter },
                               { "enumName", stripPrefix( enumData->first, "Vk" ) },
                               { "enumMemberName", value.vkValue },
                               { "leave", leave } } );
    }
  }
}

void VulkanHppGenerator::appendRAIIHandle( std::string &                              str,
                                           std::string &                              commandDefinitions,
                                           std::pair<std::string, HandleData> const & handle,
                                           std::set<std::string> &                    listedHandles,
                                           std::set<std::string> const &              specialFunctions ) const
{
  if ( listedHandles.find( handle.first ) == listedHandles.end() )
  {
    rescheduleRAIIHandle( str, commandDefinitions, handle, listedHandles, specialFunctions );

    std::string enter, leave;
    std::tie( enter, leave ) = generateProtection( handle.first, !handle.second.alias.empty() );
    std::string handleType   = stripPrefix( handle.first, "Vk" );
    std::string handleName   = startLowerCase( handleType );

    std::string singularConstructors, arrayConstructors;
    std::tie( singularConstructors, arrayConstructors ) = constructRAIIHandleConstructors( handle );
    std::string upgradeConstructor = arrayConstructors.empty() ? "" : constructRAIIHandleUpgradeConstructor( handle );
    std::string destructor, destructorCall;
    std::tie( destructor, destructorCall ) =
      ( handle.second.destructorIt == m_commands.end() )
        ? std::make_pair( "", "" )
        : constructRAIIHandleDestructor( handle.first, handle.second.destructorIt, enter );

    std::string getConstructorSuccessCode, memberVariables, moveConstructorInitializerList, moveAssignmentInstructions;
    std::tie( getConstructorSuccessCode, memberVariables, moveConstructorInitializerList, moveAssignmentInstructions ) =
      constructRAIIHandleDetails( handle, destructorCall );

    std::string declarations, definitions;
    std::tie( declarations, definitions ) = constructRAIIHandleMemberFunctions( handle, specialFunctions );
    commandDefinitions += definitions;

    assert( !handle.second.objTypeEnum.empty() );
    auto enumIt = m_enums.find( "VkObjectType" );
    assert( enumIt != m_enums.end() );
    auto valueIt =
      std::find_if( enumIt->second.values.begin(),
                    enumIt->second.values.end(),
                    [&handle]( EnumValueData const & evd ) { return evd.vulkanValue == handle.second.objTypeEnum; } );
    assert( valueIt != enumIt->second.values.end() );
    std::string objTypeEnum = valueIt->vkValue;

    enumIt = m_enums.find( "VkDebugReportObjectTypeEXT" );
    assert( enumIt != m_enums.end() );
    valueIt                           = std::find_if( enumIt->second.values.begin(),
                            enumIt->second.values.end(),
                            [&handleType]( EnumValueData const & evd ) { return evd.vkValue == "e" + handleType; } );
    std::string debugReportObjectType = ( valueIt != enumIt->second.values.end() ) ? valueIt->vkValue : "eUnknown";

    std::string dispatcherType =
      ( ( handle.first == "VkDevice" ) ||
        ( handle.second.constructorIts.front()->second.params.front().type.type == "VkDevice" ) )
        ? "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher"
        : "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher";

    const std::string handleTemplate = R"(${enter}  class ${handleType}
  {
  public:
    using CType = Vk${handleType};

    static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::ObjectType objectType = VULKAN_HPP_NAMESPACE::ObjectType::${objTypeEnum};
    static VULKAN_HPP_CONST_OR_CONSTEXPR VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT debugReportObjectType = VULKAN_HPP_NAMESPACE::DebugReportObjectTypeEXT::${debugReportObjectType};

  public:
${singularConstructors}
${upgradeConstructor}
${destructor}

    ${handleType}() = delete;
    ${handleType}( ${handleType} const & ) = delete;
    ${handleType}( ${handleType} && rhs )
      : ${moveConstructorInitializerList}
    {}
    ${handleType} & operator=( ${handleType} const & ) = delete;
    ${handleType} & operator=( ${handleType} && rhs )
    {
      if ( this != &rhs )
      {
${moveAssignmentInstructions}
      }
      return *this;
    }
${memberFunctionsDeclarations}

    VULKAN_HPP_NAMESPACE::${handleType} const & operator*() const VULKAN_HPP_NOEXCEPT
    {
      return m_${handleName};
    }

${getConstructorSuccessCode}
    ${dispatcherType} const * getDispatcher() const
    {
      return ${getDispatcherReturn}m_dispatcher;
    }

  private:
    ${memberVariables}
  };

${leave})";

    str += replaceWithMap(
      handleTemplate,
      { { "debugReportObjectType", debugReportObjectType },
        { "destructor", destructor },
        { "dispatcherType", dispatcherType },
        { "enter", enter },
        { "getConstructorSuccessCode", getConstructorSuccessCode },
        { "getDispatcherReturn", ( handleType == "Device" ) || ( handleType == "Instance" ) ? "&" : "" },
        { "handleName", handleName },
        { "handleType", handleType },
        { "leave", leave },
        { "memberFunctionsDeclarations", declarations },
        { "memberVariables", memberVariables },
        { "moveAssignmentInstructions", moveAssignmentInstructions },
        { "moveConstructorInitializerList", moveConstructorInitializerList },
        { "objTypeEnum", objTypeEnum },
        { "singularConstructors", singularConstructors },
        { "upgradeConstructor", upgradeConstructor } } );

    if ( !arrayConstructors.empty() )
    {
      // it's a handle class with a friendly handles class
      const std::string handlesTemplate = R"(
${enter}  class ${handleType}s : public std::vector<VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::${handleType}>
  {
  public:
    ${arrayConstructors}

    ${handleType}s() = delete;
    ${handleType}s( ${handleType}s const & ) = delete;
    ${handleType}s( ${handleType}s && rhs ) = default;
    ${handleType}s & operator=( ${handleType}s const & ) = delete;
    ${handleType}s & operator=( ${handleType}s && rhs ) = default;
  };
${leave}
)";

      str += replaceWithMap( handlesTemplate,
                             { { "arrayConstructors", arrayConstructors },
                               { "enter", enter },
                               { "handleType", handleType },
                               { "leave", leave } } );
    }
  }
}

void VulkanHppGenerator::appendRAIIHandleContext( std::string &                              str,
                                                  std::string &                              commandDefinitions,
                                                  std::pair<std::string, HandleData> const & handle,
                                                  std::set<std::string> const &              specialFunctions ) const
{
  assert( commandDefinitions.empty() );

  const std::string contextTemplate = R"(
  class Context
  {
  public:
    Context()
      : m_dispatcher( m_dynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" ) )
    {}

    ~Context() = default;

    Context( Context const & ) = delete;
    Context( Context && rhs )
      : m_dynamicLoader( std::move( rhs.m_dynamicLoader ) )
      , m_dispatcher( std::move( rhs.m_dispatcher ) )
    {}
    Context & operator=( Context const & ) = delete;
    Context & operator=( Context && rhs )
    {
      if ( this != &rhs )
      {
        m_dynamicLoader = std::move( rhs.m_dynamicLoader );
        m_dispatcher    = std::move( rhs.m_dispatcher );
      }
      return *this;
    }

${memberFunctionDeclarations}

    VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ContextDispatcher const * getDispatcher() const
    {
      return &m_dispatcher;
    }

  private:
    VULKAN_HPP_NAMESPACE::DynamicLoader                                m_dynamicLoader;
    VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::ContextDispatcher m_dispatcher;
  };

)";

  std::string declarations;
  std::tie( declarations, commandDefinitions ) = constructRAIIHandleMemberFunctions( handle, specialFunctions );

  str += replaceWithMap( contextTemplate, { { "memberFunctionDeclarations", declarations } } );
}

std::pair<std::string, std::string>
  VulkanHppGenerator::appendStaticCommand( std::string & str, std::pair<std::string, CommandData> const & command )
{
  std::string parameterList, parameters;
  bool        firstParam = true;
  for ( auto param : command.second.params )
  {
    if ( !firstParam )
    {
      parameterList += ", ";
      parameters += ", ";
    }
    parameterList += param.type.prefix + ( param.type.prefix.empty() ? "" : " " ) + param.type.type +
                     param.type.postfix + " " + param.name + constructCArraySizes( param.arraySizes );
    parameters += param.name;
    firstParam = false;
  }
  std::string commandName = stripPrefix( command.first, "vk" );

  str += "    " + command.second.returnType + " vk" + commandName + "( " + parameterList +
         " ) const VULKAN_HPP_NOEXCEPT\n"
         "    {\n"
         "      return ::vk" +
         commandName + "( " + parameters +
         " );\n"
         "    }\n";
  return std::make_pair( parameterList, parameters );
}

void VulkanHppGenerator::appendStruct( std::string & str, std::pair<std::string, StructureData> const & structure )
{
  assert( m_listingTypes.find( structure.first ) == m_listingTypes.end() );
  m_listingTypes.insert( structure.first );

  assert( m_listedTypes.find( structure.first ) == m_listedTypes.end() );

  for ( auto const & member : structure.second.members )
  {
    if ( structure.first != member.type.type )  // some structures hold a pointer to the very same structure type
    {
      appendType( str, member.type.type );
    }
  }

  if ( !structure.second.subStruct.empty() )
  {
    auto structureIt = m_structures.find( structure.second.subStruct );
    if ( ( structureIt != m_structures.end() ) && ( m_listedTypes.find( structureIt->first ) == m_listedTypes.end() ) )
    {
      appendStruct( str, *structureIt );
    }
  }

  if ( structure.second.isUnion )
  {
    appendUnion( str, structure );
  }
  else
  {
    appendStructure( str, structure );
  }

  m_listingTypes.erase( structure.first );
  m_listedTypes.insert( structure.first );
}

void VulkanHppGenerator::appendStructAssignmentOperators( std::string &                                 str,
                                                          std::pair<std::string, StructureData> const & structData,
                                                          std::string const &                           prefix ) const
{
  static const std::string assignmentFromVulkanType = R"(
${prefix}${constexpr_assign}${structName} & operator=( ${structName} const & rhs ) VULKAN_HPP_NOEXCEPT = default;

${prefix}${structName} & operator=( Vk${structName} const & rhs ) VULKAN_HPP_NOEXCEPT
${prefix}{
${prefix}  *this = *reinterpret_cast<VULKAN_HPP_NAMESPACE::${structName} const *>( &rhs );
${prefix}  return *this;
${prefix}}
)";
  str += replaceWithMap( assignmentFromVulkanType,
                         { { "constexpr_assign", constructConstexprString( structData, true ) },
                           { "prefix", prefix },
                           { "structName", stripPrefix( structData.first, "Vk" ) } } );
}

void VulkanHppGenerator::appendStructCompareOperators( std::string &                                 str,
                                                       std::pair<std::string, StructureData> const & structData ) const
{
  static const std::set<std::string> simpleTypes = { "char",      "double",   "DWORD",    "float",   "HANDLE",
                                                     "HINSTANCE", "HMONITOR", "HWND",     "int",     "int8_t",
                                                     "int16_t",   "int32_t",  "int64_t",  "LPCWSTR", "size_t",
                                                     "uint8_t",   "uint16_t", "uint32_t", "uint64_t" };
  // two structs are compared by comparing each of the elements
  std::string compareMembers;
  std::string intro = "";
  for ( size_t i = 0; i < structData.second.members.size(); i++ )
  {
    MemberData const & member = structData.second.members[i];
    auto               typeIt = m_types.find( member.type.type );
    assert( typeIt != m_types.end() );
    if ( ( typeIt->second.category == TypeCategory::Requires ) && member.type.postfix.empty() &&
         ( simpleTypes.find( member.type.type ) == simpleTypes.end() ) )
    {
      // this type might support operator==()... that is, use memcmp
      compareMembers +=
        intro + "( memcmp( &" + member.name + ", &rhs." + member.name + ", sizeof( " + member.type.type + " ) ) == 0 )";
    }
    else
    {
      // for all others, we use the operator== of that type
      compareMembers += intro + "( " + member.name + " == rhs." + member.name + " )";
    }
    intro = "\n          && ";
  }

  static const std::string compareTemplate = R"(
#if defined(VULKAN_HPP_HAS_SPACESHIP_OPERATOR)
    auto operator<=>( ${name} const & ) const = default;
#else
    bool operator==( ${name} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return ${compareMembers};
    }

    bool operator!=( ${name} const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return !operator==( rhs );
    }
#endif
)";

  str += replaceWithMap( compareTemplate,
                         { { "name", stripPrefix( structData.first, "Vk" ) }, { "compareMembers", compareMembers } } );
}

bool VulkanHppGenerator::checkEquivalentSingularConstructor(
  std::vector<std::map<std::string, CommandData>::const_iterator> const & constructorIts,
  std::map<std::string, CommandData>::const_iterator                      constructorIt,
  std::vector<ParamData>::const_iterator                                  lenIt ) const
{
  // check, if there is no singular constructor with the very same arguments as this array constructor
  // (besides the size, of course)
  auto singularCommandIt =
    std::find_if( constructorIts.begin(),
                  constructorIts.end(),
                  [constructorIt, lenIt]( std::map<std::string, CommandData>::const_iterator it )
                  {
                    if ( it->second.params.size() + 1 != constructorIt->second.params.size() )
                    {
                      return false;
                    }
                    size_t lenIdx = std::distance( constructorIt->second.params.begin(), lenIt );
                    for ( size_t i = 0, j = 0; i < it->second.params.size(); ++i, ++j )
                    {
                      assert( j < constructorIt->second.params.size() );
                      if ( j == lenIdx )
                      {
                        ++j;
                      }
                      if ( it->second.params[i].type.type != constructorIt->second.params[j].type.type )
                      {
                        return false;
                      }
                    }
                    return true;
                  } );
  return ( singularCommandIt != constructorIts.end() );
}

std::string VulkanHppGenerator::constructArgumentListEnhanced( std::vector<ParamData> const & params,
                                                               std::set<size_t> const &       skippedParams,
                                                               std::set<size_t> const &       singularParams,
                                                               bool                           definition,
                                                               bool                           withAllocators,
                                                               bool                           structureChain,
                                                               bool                           withDispatcher ) const
{
  size_t defaultStartIndex = withAllocators ? ~0 : determineDefaultStartIndex( params, skippedParams );

  std::string argumentList;
  bool        encounteredArgument = false;
  for ( size_t i = 0; i < params.size(); ++i )
  {
    if ( skippedParams.find( i ) == skippedParams.end() )
    {
      if ( encounteredArgument )
      {
        argumentList += ", ";
      }
      bool hasDefaultAssignment = false;
      if ( singularParams.find( i ) != singularParams.end() )
      {
        assert( !params[i].optional );
        assert( params[i].type.isConstPointer() && !params[i].len.empty() &&
                !isLenByStructMember( params[i].len, params ) && beginsWith( params[i].type.type, "Vk" ) );
        assert( !isHandleType( params[i].type.type ) );
        argumentList += "const VULKAN_HPP_NAMESPACE::" + stripPrefix( params[i].type.type, "Vk" ) + " & " +
                        stripPluralS( startLowerCase( stripPrefix( params[i].name, "p" ) ) );
      }
      else if ( params[i].type.isConstPointer() )
      {
        std::string name = startLowerCase( stripPrefix( params[i].name, "p" ) );
        if ( params[i].len.empty() )
        {
          assert( withDispatcher || !isHandleType( params[i].type.type ) );
          assert( !params[i].type.prefix.empty() && ( params[i].type.postfix == "*" ) );
          assert( params[i].arraySizes.empty() );
          if ( params[i].type.type == "void" )
          {
            assert( !params[i].optional );
            argumentList += params[i].type.compose() + " " + params[i].name;
          }
          else if ( params[i].optional )
          {
            argumentList +=
              "Optional<const " + stripPrefix( params[i].type.type, "Vk" ) + "> " + name +
              ( ( definition || withAllocators ) ? "" : " VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT" );
            hasDefaultAssignment = true;
          }
          else
          {
            argumentList += params[i].type.prefix + " " + stripPrefix( params[i].type.type, "Vk" ) + " & " + name;
          }
        }
        else
        {
          // a const-pointer with a non-empty len is either null-terminated (aka a string) or represented by an
          // ArrayProxy
          assert( params[i].arraySizes.empty() );
          if ( params[i].len == "null-terminated" )
          {
            assert( params[i].type.type == "char" );
            if ( params[i].optional )
            {
              argumentList +=
                "Optional<const std::string> " + name +
                ( ( definition || withAllocators ) ? "" : " VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT" );
              hasDefaultAssignment = true;
            }
            else
            {
              argumentList += "const std::string & " + name;
            }
          }
          else
          {
            // an ArrayProxy also covers no data, so any optiona flag can be ignored here
            assert( endsWith( params[i].type.postfix, "*" ) );
            std::string type = stripPostfix( params[i].type.compose(), "*" );
            size_t      pos  = type.find( "void" );
            if ( pos != std::string::npos )
            {
              type.replace( pos, 4, "T" );
            }
            argumentList += "ArrayProxy<" + type + "> const & " + name;
            if ( params[i].optional && !definition )
            {
              argumentList += " VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT";
              hasDefaultAssignment = true;
            }
          }
        }
      }
      else if ( params[i].type.isNonConstPointer() )
      {
        assert( withDispatcher || !isHandleType( params[i].type.type ) );
        assert( params[i].len.empty() && !params[i].optional );
        if ( !params[i].type.prefix.empty() )
        {
          argumentList += params[i].type.prefix + " ";
        }
        argumentList += params[i].type.type + " & " + params[i].name;
      }
      else
      {
        assert( params[i].type.isValue() );
        argumentList += params[i].type.compose() + " " + params[i].name + constructCArraySizes( params[i].arraySizes );
      }
      argumentList += std::string( !definition && ( defaultStartIndex <= i ) && !hasDefaultAssignment
                                     ? " VULKAN_HPP_DEFAULT_ARGUMENT_ASSIGNMENT"
                                     : "" );
      encounteredArgument = true;
    }
  }
  if ( withAllocators )
  {
    if ( structureChain )
    {
      if ( encounteredArgument )
      {
        argumentList += ", ";
      }
      argumentList += "StructureChainAllocator & structureChainAllocator";
      encounteredArgument = true;
    }
    else
    {
      for ( auto sp : skippedParams )
      {
        if ( !params[sp].len.empty() )
        {
          if ( encounteredArgument )
          {
            argumentList += ", ";
          }
          std::string type = ( params[sp].type.type == "void" )
                               ? "Uint8_t"
                               : startUpperCase( stripPrefix( params[sp].type.type, "Vk" ) );
          argumentList += type + "Allocator & " + startLowerCase( type ) + "Allocator";
          encounteredArgument = true;
        }
      }
    }
  }
  if ( withDispatcher )
  {
    if ( encounteredArgument )
    {
      argumentList += ", ";
    }
    argumentList +=
      std::string( "Dispatch const & d" ) + ( definition ? "" : " VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT" );
  }
  return argumentList;
}

std::string VulkanHppGenerator::constructArgumentListStandard( std::vector<ParamData> const & params,
                                                               std::set<size_t> const &       skippedParams ) const
{
  std::string argumentList;
  for ( size_t i = 0; i < params.size(); ++i )
  {
    if ( skippedParams.find( i ) == skippedParams.end() )
    {
      argumentList +=
        params[i].type.compose() + " " + params[i].name + constructCArraySizes( params[i].arraySizes ) + ", ";
    }
  }
  argumentList += "Dispatch const & d ";
  return argumentList;
}

std::string VulkanHppGenerator::constructCallArgumentEnhanced( std::vector<ParamData> const & params,
                                                               size_t                         paramIndex,
                                                               bool                           nonConstPointerAsNullptr,
                                                               std::set<size_t> const &       singularParams,
                                                               std::vector<size_t> const &    returnParamIndices,
                                                               bool raiiHandleMemberFunction ) const
{
  std::string       argument;
  ParamData const & param = params[paramIndex];
  if ( param.type.isConstPointer() || ( specialPointerTypes.find( param.type.type ) != specialPointerTypes.end() ) )
  {
    std::string name = startLowerCase( stripPrefix( param.name, "p" ) );
    if ( isHandleType( param.type.type ) && param.type.isValue() )
    {
      assert( !param.optional );
      // if at all, this is the first argument, and it's the implicitly provided member handle
      assert( paramIndex == 0 );
      assert( param.arraySizes.empty() && param.len.empty() );
      argument += "m_" + startLowerCase( stripPrefix( param.type.type, "Vk" ) );
    }
    else if ( param.len.empty() )
    {
      // this const-pointer parameter has no length, that is it's a const-pointer to a single value
      if ( param.type.type == "void" )
      {
        assert( !param.optional );
        // use the original name here, as void-pointer are not mapped to some reference
        argument = param.name;
      }
      else if ( param.optional )
      {
        argument = "static_cast<" + param.type.compose() + ">( " + name + " )";
      }
      else
      {
        argument = "&" + name;
      }
      if ( beginsWith( param.type.type, "Vk" ) )
      {
        argument = "reinterpret_cast<const " + param.type.type + " *>( " + argument + " )";
      }
    }
    else if ( param.len == "null-terminated" )
    {
      // this const-pointer parameter is "null-terminated", that is it's a string
      assert( ( param.type.type == "char" ) && param.arraySizes.empty() );
      if ( param.optional )
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
      if ( singularParams.find( paramIndex ) != singularParams.end() )
      {
        assert( !param.optional );
        argument = "&" + stripPluralS( name );
      }
      else
      {
        // this const-parameter is represented by some array, where data() also works with no data (optional)
        argument = name + ".data()";
      }
      if ( beginsWith( param.type.type, "Vk" ) || ( param.type.type == "void" ) )
      {
        argument = "reinterpret_cast<" + param.type.prefix + " " + param.type.type + " " + param.type.postfix + ">( " +
                   argument + " )";
      }
    }
  }
  else if ( param.type.isNonConstPointer() &&
            ( specialPointerTypes.find( param.type.type ) == specialPointerTypes.end() ) )
  {
    // parameter is a non-const pointer (and none of the special pointer types, that are considered const-pointers,
    // even though they are not!)
    std::string name = startLowerCase( stripPrefix( param.name, "p" ) );
    if ( param.len.empty() )
    {
      assert( param.arraySizes.empty() );
      if ( beginsWith( param.type.type, "Vk" ) )
      {
        argument = "reinterpret_cast<" + param.type.type + " *>( &" + name + " )";
      }
      else
      {
        assert( !param.optional );
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
      else if ( beginsWith( param.type.type, "Vk" ) || ( param.type.type == "void" ) )
      {
        if ( singularParams.find( paramIndex ) != singularParams.end() )
        {
          argument = "&" + stripPluralS( name );
        }
        else
        {
          // get the data of the array, which also covers no data -> no need to look at param.optional
          argument = name + ".data()";
        }
        if ( !raiiHandleMemberFunction || !isHandleType( param.type.type ) )
        {
          argument = "reinterpret_cast<" + param.type.type + " *>( " + argument + " )";
        }
      }
      else
      {
        assert( !param.optional );
        argument = name + ".data()";
      }
    }
  }
  else
  {
    assert( param.len.empty() );
    if ( beginsWith( param.type.type, "Vk" ) )
    {
      if ( param.arraySizes.empty() )
      {
        auto pointerIt = std::find_if(
          params.begin(), params.end(), [&param]( ParamData const & pd ) { return pd.len == param.name; } );
        if ( pointerIt != params.end() )
        {
          assert( !param.optional );
          argument = startLowerCase( stripPrefix( pointerIt->name, "p" ) ) + ".size()";
          if ( pointerIt->type.type == "void" )
          {
            argument += " * sizeof( T )";
          }
        }
        else
        {
          argument = "static_cast<" + param.type.type + ">( " + param.name + " )";
        }
      }
      else
      {
        assert( !param.optional );
        assert( param.arraySizes.size() == 1 );
        assert( param.type.prefix == "const" );
        argument = "reinterpret_cast<const " + param.type.type + " *>( " + param.name + " )";
      }
    }
    else
    {
      if ( singularParams.find( paramIndex ) != singularParams.end() )
      {
        assert( !param.optional );
        assert( param.arraySizes.empty() );
        assert( ( param.type.type == "size_t" ) || ( param.type.type == "uint32_t" ) );
        assert( returnParamIndices.size() == 1 );
        if ( params[returnParamIndices[0]].type.type == "void" )
        {
          argument = "sizeof( T )";
        }
        else
        {
          argument = "1";
        }
      }
      else
      {
        auto pointerIt = std::find_if(
          params.begin(), params.end(), [&param]( ParamData const & pd ) { return pd.len == param.name; } );
        if ( pointerIt != params.end() )
        {
          // this parameter is the len of some other -> replace it with that parameter's size
          assert( param.arraySizes.empty() );
          assert( ( param.type.type == "size_t" ) || ( param.type.type == "uint32_t" ) );
          argument = startLowerCase( stripPrefix( pointerIt->name, "p" ) ) + ".size()";
          if ( pointerIt->type.type == "void" )
          {
            argument += " * sizeof( T )";
          }
        }
        else
        {
          assert( !param.optional );
          assert( param.arraySizes.size() <= 1 );
          argument = param.name;
        }
      }
    }
  }
  assert( !argument.empty() );
  return argument;
}

std::string VulkanHppGenerator::constructCallArgumentsEnhanced( std::vector<ParamData> const & params,
                                                                size_t                         initialSkipCount,
                                                                bool                           nonConstPointerAsNullptr,
                                                                std::set<size_t> const &       singularParams,
                                                                std::vector<size_t> const &    returnParamIndices,
                                                                bool raiiHandleMemberFunction ) const
{
  assert( initialSkipCount <= params.size() );
  std::string arguments;
  bool        encounteredArgument = false;
  for ( size_t i = 0; i < initialSkipCount; ++i )
  {
    if ( encounteredArgument )
    {
      arguments += ", ";
    }
    assert( isHandleType( params[i].type.type ) && params[i].type.isValue() );
    assert( params[i].arraySizes.empty() && params[i].len.empty() );
    std::string argument = "m_" + startLowerCase( stripPrefix( params[i].type.type, "Vk" ) );
    if ( raiiHandleMemberFunction )
    {
      argument = "static_cast<" + params[i].type.type + ">( " + argument + " )";
    }
    arguments += argument;
    encounteredArgument = true;
  }
  for ( size_t i = initialSkipCount; i < params.size(); ++i )
  {
    if ( encounteredArgument )
    {
      arguments += ", ";
    }
    arguments += constructCallArgumentEnhanced(
      params, i, nonConstPointerAsNullptr, singularParams, returnParamIndices, raiiHandleMemberFunction );
    encounteredArgument = true;
  }
  return arguments;
}

std::string VulkanHppGenerator::constructCallArgumentsStandard( std::string const &            handle,
                                                                std::vector<ParamData> const & params ) const
{
  std::string arguments;
  bool        encounteredArgument = false;
  for ( auto const & param : params )
  {
    if ( encounteredArgument )
    {
      arguments += ", ";
    }
    if ( ( param.type.type == handle ) && param.type.isValue() )
    {
      assert( param.arraySizes.empty() && param.len.empty() );
      arguments += "m_" + startLowerCase( stripPrefix( param.type.type, "Vk" ) );
    }
    else
    {
      std::string argument = param.name;
      if ( beginsWith( param.type.type, "Vk" ) )
      {
        if ( !param.arraySizes.empty() )
        {
          assert( param.arraySizes.size() == 1 );
          assert( param.type.isValue() );
          assert( param.type.prefix == "const" );
          argument = "reinterpret_cast<const " + param.type.type + "*>( " + argument + " )";
        }
        else if ( param.type.isValue() )
        {
          argument = "static_cast<" + param.type.type + ">( " + argument + " )";
        }
        else
        {
          assert( !param.type.postfix.empty() );
          argument = "reinterpret_cast<" + ( param.type.prefix.empty() ? "" : param.type.prefix ) + " " +
                     param.type.type + " " + param.type.postfix + ">( " + argument + " )";
        }
      }
      arguments += argument;
    }
    encounteredArgument = true;
  }
  return arguments;
}

std::string VulkanHppGenerator::constructCommandBoolGetValue( std::string const & name,
                                                              CommandData const & commandData,
                                                              size_t              initialSkipCount,
                                                              bool                definition,
                                                              size_t              nonConstPointerIndex ) const
{
  assert( commandData.returnType == "VkBool32" );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, {}, { nonConstPointerIndex }, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, false, false, true );
  std::string commandName =
    determineCommandName( name, initialSkipCount ? commandData.params[initialSkipCount - 1].type.type : "", m_tags );
  std::string returnType = stripPostfix( commandData.params[nonConstPointerIndex].type.compose(), "*" );
  assert( !beginsWith( returnType, "Vk" ) );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::pair<Bool32,${returnType}> ${className}${classSeparator}${commandName}( ${argumentList} )${const}
  {
    std::pair<Bool32,${returnType}> result;
    ${returnType} & ${returnValueName} = result.second;
    result.first = static_cast<Bool32>( d.${vkCommand}( ${callArguments} ) );
    return result;
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, { nonConstPointerIndex }, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "const", commandData.handle.empty() ? "" : " const" },
        { "commandName", commandName },
        { "returnValueName", startLowerCase( stripPrefix( commandData.params[nonConstPointerIndex].name, "p" ) ) },
        { "returnType", returnType },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    VULKAN_HPP_NODISCARD std::pair<bool,${returnType}> ${commandName}( ${argumentList} )${const};)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "const", commandData.handle.empty() ? "" : " const" },
                             { "returnType", returnType } } );
  }
}

std::string VulkanHppGenerator::constructCommandResult( std::string const &              name,
                                                        CommandData const &              commandData,
                                                        size_t                           initialSkipCount,
                                                        bool                             definition,
                                                        std::map<size_t, size_t> const & vectorParamIndices ) const
{
  assert( commandData.returnType == "VkResult" );

  std::set<size_t> skippedParameters =
    determineSkippedParams( commandData.params, initialSkipCount, vectorParamIndices, {}, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParameters, {}, definition, false, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard  = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string returnType = ( 1 < commandData.successCodes.size() ) ? "Result" : "typename ResultValueType<void>::type";

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename Dispatch>
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    return createResultValue( result, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}"${successCodeList} );
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced( commandData.params, initialSkipCount, false, {}, {}, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "nodiscard", nodiscard },
        { "returnType", returnType },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ${nodiscard}${returnType} ${commandName}( ${argumentList} ) const;)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType } } );
  }
}

std::string VulkanHppGenerator::constructCommandResultEnumerate( std::string const &               name,
                                                                 CommandData const &               commandData,
                                                                 size_t                            initialSkipCount,
                                                                 bool                              definition,
                                                                 std::pair<size_t, size_t> const & vectorParamIndices,
                                                                 bool                              withAllocator ) const
{
  assert( commandData.returnType == "VkResult" );
  assert( ( commandData.successCodes.size() == 2 ) && ( commandData.successCodes[0] == "VK_SUCCESS" ) &&
          ( commandData.successCodes[1] == "VK_INCOMPLETE" ) );

  std::set<size_t> skippedParams = determineSkippedParams( commandData.params,
                                                           initialSkipCount,
                                                           { vectorParamIndices },
                                                           { vectorParamIndices.second, vectorParamIndices.first },
                                                           false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, withAllocator, false, true );
  std::string commandName =
    determineCommandName( name, initialSkipCount ? commandData.params[initialSkipCount - 1].type.type : "", m_tags );
  std::string nodiscard = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string vectorElementType = ( commandData.params[vectorParamIndices.first].type.type == "void" )
                                    ? "uint8_t"
                                    : stripPrefix( commandData.params[vectorParamIndices.first].type.type, "Vk" );
  std::string allocatorType     = startUpperCase( vectorElementType ) + "Allocator";

  if ( definition )
  {
    const std::string functionTemplate =
      R"(  template <typename ${allocatorType}, typename Dispatch${typenameCheck}>
  ${nodiscard}VULKAN_HPP_INLINE typename ResultValueType<std::vector<${vectorElementType}, ${allocatorType}>>::type ${className}${classSeparator}${commandName}( ${argumentList} )${const}
  {
    std::vector<${vectorElementType}, ${allocatorType}> ${vectorName}${vectorAllocator};
    ${counterType} ${counterName};
    Result result;
    do
    {
      result = static_cast<Result>( d.${vkCommand}( ${firstCallArguments} ) );
      if ( ( result == Result::eSuccess ) && ${counterName} )
      {
        ${vectorName}.resize( ${counterName} );
        result = static_cast<Result>( d.${vkCommand}( ${secondCallArguments} ) );
        VULKAN_HPP_ASSERT( ${counterName} <= ${vectorName}.size() );
      }
    } while ( result == Result::eIncomplete );
    if ( ( result == Result::eSuccess ) && ( ${counterName} < ${vectorName}.size() ) )
    {
      ${vectorName}.resize( ${counterName} );
    }
    return createResultValue( result, ${vectorName}, VULKAN_HPP_NAMESPACE_STRING"::${className}${classSeparator}${commandName}" );
  })";

    std::string typenameCheck = withAllocator
                                  ? ( ", typename B, typename std::enable_if<std::is_same<typename B::value_type, " +
                                      vectorElementType + ">::value, int>::type " )
                                  : "";

    return replaceWithMap(
      functionTemplate,
      { { "allocatorType", allocatorType },
        { "argumentList", argumentList },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "const", commandData.handle.empty() ? "" : " const" },
        { "counterName", startLowerCase( stripPrefix( commandData.params[vectorParamIndices.second].name, "p" ) ) },
        { "counterType", commandData.params[vectorParamIndices.second].type.type },
        { "firstCallArguments",
          constructCallArgumentsEnhanced( commandData.params, initialSkipCount, true, {}, {}, false ) },
        { "nodiscard", nodiscard },
        { "secondCallArguments",
          constructCallArgumentsEnhanced( commandData.params, initialSkipCount, false, {}, {}, false ) },
        { "typenameCheck", typenameCheck },
        { "vectorAllocator", withAllocator ? ( "( " + startLowerCase( allocatorType ) + " )" ) : "" },
        { "vectorElementType", vectorElementType },
        { "vectorName", startLowerCase( stripPrefix( commandData.params[vectorParamIndices.first].name, "p" ) ) },
        { "vkCommand", name } } );
  }
  else
  {
    const std::string functionTemplate =
      R"(    template <typename ${allocatorType} = std::allocator<${vectorElementType}>, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE${typenameCheck}>
    ${nodiscard}typename ResultValueType<std::vector<${vectorElementType}, ${allocatorType}>>::type ${commandName}( ${argumentList} )${const};)";

    std::string typenameCheck = withAllocator ? ( ", typename B = " + allocatorType +
                                                  ", typename std::enable_if<std::is_same<typename B::value_type, " +
                                                  vectorElementType + ">::value, int>::type = 0" )
                                              : "";

    return replaceWithMap( functionTemplate,
                           { { "allocatorType", allocatorType },
                             { "argumentList", argumentList },
                             { "const", commandData.handle.empty() ? "" : " const" },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "typenameCheck", typenameCheck },
                             { "vectorElementType", vectorElementType } } );
  }
}

std::string
  VulkanHppGenerator::constructCommandResultEnumerateChained( std::string const &               name,
                                                              CommandData const &               commandData,
                                                              size_t                            initialSkipCount,
                                                              bool                              definition,
                                                              std::pair<size_t, size_t> const & vectorParamIndex,
                                                              std::vector<size_t> const &       returnParamIndices,
                                                              bool                              withAllocator ) const
{
  assert( commandData.returnType == "VkResult" );
  assert( ( commandData.successCodes.size() == 2 ) && ( commandData.successCodes[0] == "VK_SUCCESS" ) &&
          ( commandData.successCodes[1] == "VK_INCOMPLETE" ) );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, { vectorParamIndex }, returnParamIndices, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, withAllocator, true, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  assert( beginsWith( commandData.params[vectorParamIndex.first].type.type, "Vk" ) );
  std::string vectorElementType =
    "VULKAN_HPP_NAMESPACE::" + stripPrefix( commandData.params[vectorParamIndex.first].type.type, "Vk" );
  std::string allocatorType = startUpperCase( vectorElementType ) + "Allocator";

  if ( definition )
  {
    const std::string functionTemplate =
      R"(  template <typename StructureChain, typename StructureChainAllocator, typename Dispatch${typenameCheck}>
  ${nodiscard}VULKAN_HPP_INLINE typename ResultValueType<std::vector<StructureChain, StructureChainAllocator>>::type ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    std::vector<StructureChain, StructureChainAllocator> returnVector${structureChainAllocator};
    std::vector<${vectorElementType}> ${vectorName};
    ${counterType} ${counterName};
    Result result;
    do
    {
      result = static_cast<Result>( d.${vkCommand}( ${firstCallArguments} ) );
      if ( ( result == Result::eSuccess ) && ${counterName} )
      {
        returnVector.resize( ${counterName} );
        ${vectorName}.resize( ${counterName} );
        for ( ${counterType} i = 0; i < ${counterName}; i++ )
        {
          ${vectorName}[i].pNext =
            returnVector[i].template get<${vectorElementType}>().pNext;
        }
        result = static_cast<Result>( d.${vkCommand}( ${secondCallArguments} ) );
        VULKAN_HPP_ASSERT( ${counterName} <= ${vectorName}.size() );
      }
    } while ( result == Result::eIncomplete );
    if ( ( result == Result::eSuccess ) && ( ${counterName} < ${vectorName}.size() ) )
    {
      returnVector.resize( ${counterName} );
      ${vectorName}.resize( ${counterName} );
    }
    for ( ${counterType} i = 0; i < ${counterName}; i++ )
    {
      returnVector[i].template get<${vectorElementType}>() = ${vectorName}[i];
    }
    return createResultValue( result, returnVector, VULKAN_HPP_NAMESPACE_STRING"::${className}${classSeparator}${commandName}" );
  })";

    std::string vectorName = startLowerCase( stripPrefix( commandData.params[vectorParamIndex.first].name, "p" ) );
    std::string typenameCheck =
      withAllocator
        ? ( ", typename B, typename std::enable_if<std::is_same<typename B::value_type, StructureChain>::value, int>::type" )
        : "";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "counterName", startLowerCase( stripPrefix( commandData.params[vectorParamIndex.second].name, "p" ) ) },
        { "counterType", commandData.params[vectorParamIndex.second].type.type },
        { "firstCallArguments",
          constructCallArgumentsEnhanced( commandData.params, initialSkipCount, true, {}, returnParamIndices, false ) },
        { "nodiscard", nodiscard },
        { "secondCallArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, returnParamIndices, false ) },
        { "structureChainAllocator", withAllocator ? ( "( structureChainAllocator )" ) : "" },
        { "typenameCheck", typenameCheck },
        { "vectorElementType", vectorElementType },
        { "vectorName", vectorName },
        { "vkCommand", name } } );
  }
  else
  {
    const std::string functionTemplate =
      R"(    template <typename StructureChain, typename StructureChainAllocator = std::allocator<StructureChain>, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE${typenameCheck}>
    ${nodiscard}typename ResultValueType<std::vector<StructureChain, StructureChainAllocator>>::type ${commandName}( ${argumentList} ) const;)";

    std::string typenameCheck =
      withAllocator
        ? ( ", typename B = StructureChainAllocator, typename std::enable_if<std::is_same<typename B::value_type, StructureChain>::value, int>::type = 0" )
        : "";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "typenameCheck", typenameCheck } } );
  }
}

std::string
  VulkanHppGenerator::constructCommandResultEnumerateTwoVectors( std::string const &              name,
                                                                 CommandData const &              commandData,
                                                                 size_t                           initialSkipCount,
                                                                 bool                             definition,
                                                                 std::map<size_t, size_t> const & vectorParamIndices,
                                                                 std::vector<size_t> const &      returnParamIndices,
                                                                 bool                             withAllocators ) const
{
  assert( !commandData.handle.empty() && ( commandData.returnType == "VkResult" ) );
  assert( ( commandData.successCodes.size() == 2 ) && ( commandData.successCodes[0] == "VK_SUCCESS" ) &&
          ( commandData.successCodes[1] == "VK_INCOMPLETE" ) );

  auto firstVectorParamIt  = vectorParamIndices.begin();
  auto secondVectorParamIt = std::next( firstVectorParamIt );

  assert( commandData.params[0].type.type == commandData.handle );
  assert( firstVectorParamIt->second == secondVectorParamIt->second );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, vectorParamIndices, returnParamIndices, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, withAllocators, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string templateTypeFirst  = stripPrefix( commandData.params[firstVectorParamIt->first].type.type, "Vk" );
  std::string templateTypeSecond = stripPrefix( commandData.params[secondVectorParamIt->first].type.type, "Vk" );

  if ( definition )
  {
    const std::string functionTemplate =
      R"(  template <typename ${templateTypeFirst}Allocator, typename ${templateTypeSecond}Allocator, typename Dispatch${typenameCheck}>
  ${nodiscard}VULKAN_HPP_INLINE typename ResultValueType<std::pair<std::vector<${templateTypeFirst}, ${templateTypeFirst}Allocator>, std::vector<${templateTypeSecond}, ${templateTypeSecond}Allocator>>>::type ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    std::pair<std::vector<${templateTypeFirst}, ${templateTypeFirst}Allocator>, std::vector<${templateTypeSecond}, ${templateTypeSecond}Allocator>> data${pairConstructor};
    std::vector<${templateTypeFirst}, ${templateTypeFirst}Allocator> & ${firstVectorName} = data.first;
    std::vector<${templateTypeSecond}, ${templateTypeSecond}Allocator> & ${secondVectorName} = data.second;
    ${counterType} ${counterName};
    Result result;
    do
    {
      result = static_cast<Result>( d.${vkCommand}( ${firstCallArguments} ) );
      if ( ( result == Result::eSuccess ) && counterCount )
      {
        ${firstVectorName}.resize( ${counterName} );
        ${secondVectorName}.resize( ${counterName} );
        result = static_cast<Result>( d.${vkCommand}( ${secondCallArguments} ) );
        VULKAN_HPP_ASSERT( ${counterName} <= ${firstVectorName}.size() );
      }
    } while ( result == Result::eIncomplete );
    if ( ( result == Result::eSuccess ) && ( ${counterName} < ${firstVectorName}.size() ) )
    {
      ${firstVectorName}.resize( ${counterName} );
      ${secondVectorName}.resize( ${counterName} );
    }
    return createResultValue( result, data, VULKAN_HPP_NAMESPACE_STRING"::${className}${classSeparator}${commandName}" );
  })";

    std::string pairConstructor =
      withAllocators
        ? ( "( std::piecewise_construct, std::forward_as_tuple( " +
            startLowerCase( stripPrefix( commandData.params[firstVectorParamIt->first].type.type, "Vk" ) ) +
            "Allocator ), std::forward_as_tuple( " +
            startLowerCase( stripPrefix( commandData.params[secondVectorParamIt->first].type.type, "Vk" ) ) +
            "Allocator ) )" )
        : "";
    std::string typenameCheck =
      withAllocators ? ( ", typename B1, typename B2, typename std::enable_if<std::is_same<typename B1::value_type, " +
                         templateTypeFirst + ">::value && std::is_same<typename B2::value_type, " + templateTypeSecond +
                         ">::value, int>::type " )
                     : "";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "counterName",
          startLowerCase( stripPrefix( stripPluralS( commandData.params[firstVectorParamIt->second].name ), "p" ) ) },
        { "counterType", commandData.params[firstVectorParamIt->second].type.type },
        { "firstCallArguments",
          constructCallArgumentsEnhanced( commandData.params, initialSkipCount, true, {}, returnParamIndices, false ) },
        { "firstVectorName", startLowerCase( stripPrefix( commandData.params[firstVectorParamIt->first].name, "p" ) ) },
        { "nodiscard", nodiscard },
        { "pairConstructor", pairConstructor },
        { "secondCallArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, returnParamIndices, false ) },
        { "secondVectorName",
          startLowerCase( stripPrefix( commandData.params[secondVectorParamIt->first].name, "p" ) ) },
        { "templateTypeFirst", templateTypeFirst },
        { "templateTypeSecond", templateTypeSecond },
        { "typenameCheck", typenameCheck },
        { "vkCommand", name } } );
  }
  else
  {
    const std::string functionTemplate =
      R"(    template <typename ${templateTypeFirst}Allocator = std::allocator<${templateTypeFirst}>, typename ${templateTypeSecond}Allocator = std::allocator<${templateTypeSecond}>, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE${typenameCheck}>
    ${nodiscard}typename ResultValueType<std::pair<std::vector<${templateTypeFirst}, ${templateTypeFirst}Allocator>, std::vector<${templateTypeSecond}, ${templateTypeSecond}Allocator>>>::type ${commandName}( ${argumentList} ) const;)";

    std::string typenameCheck =
      withAllocators
        ? ( ", typename B1 = " + templateTypeFirst + "Allocator, typename B2 = " + templateTypeSecond +
            "Allocator, typename std::enable_if<std::is_same<typename B1::value_type, " + templateTypeFirst +
            ">::value && std::is_same<typename B2::value_type, " + templateTypeSecond + ">::value, int>::type = 0" )
        : "";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "templateTypeFirst", templateTypeFirst },
                             { "templateTypeSecond", templateTypeSecond },
                             { "typenameCheck", typenameCheck } } );
  }
}

std::string VulkanHppGenerator::constructCommandResultEnumerateTwoVectorsDeprecated(
  std::string const &              name,
  CommandData const &              commandData,
  size_t                           initialSkipCount,
  bool                             definition,
  std::map<size_t, size_t> const & vectorParamIndices,
  bool                             withAllocators ) const
{
  size_t returnParamIndex = determineReturnParamIndex( commandData, vectorParamIndices, true );

  std::string argumentList = constructFunctionHeaderArgumentsEnhanced(
    commandData, returnParamIndex, returnParamIndex, vectorParamIndices, !definition, withAllocators );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard  = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string returnType = determineEnhancedReturnType( commandData, returnParamIndex, false );
  std::string templateType = stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" );

  if ( definition )
  {
    const std::string functionTemplate =
      R"(  template <typename Allocator, typename Dispatch${typeCheck}>
  VULKAN_HPP_DEPRECATED( "This function is deprecated. Use one of the other flavours of it.")
  ${nodiscard}VULKAN_HPP_INLINE typename ResultValueType<${returnType}>::type ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    ${functionBody}
  })";

    std::string typeCheck = withAllocators
                              ? ", typename B, typename std::enable_if<std::is_same<typename B::value_type, " +
                                  templateType + ">::value, int>::type"
                              : "";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "functionBody",
          constructFunctionBodyEnhanced( "  ",
                                         name,
                                         commandData,
                                         initialSkipCount,
                                         returnParamIndex,
                                         returnParamIndex,
                                         vectorParamIndices,
                                         true,
                                         returnType,
                                         withAllocators ) },
        { "nodiscard", nodiscard },
        { "returnType", returnType },
        { "typeCheck", typeCheck } } );
  }
  else
  {
    const std::string functionTemplate =
      R"(    template <typename Allocator = std::allocator<${templateType}>, typename Dispatch  = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE${typeCheck}>
    ${nodiscard}typename ResultValueType<${returnType}>::type ${commandName}( ${argumentList} ) const;)";

    std::string typeCheck =
      withAllocators ? ", typename B = Allocator, typename std::enable_if<std::is_same<typename B::value_type, " +
                         templateType + ">::value, int>::type = 0"
                     : "";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType },
                             { "templateType", templateType },
                             { "typeCheck", typeCheck } } );
  }
}

std::string VulkanHppGenerator::constructCommandResultGetChain( std::string const & name,
                                                                CommandData const & commandData,
                                                                size_t              initialSkipCount,
                                                                bool                definition,
                                                                size_t              nonConstPointerIndex ) const
{
  assert( !commandData.handle.empty() && ( commandData.returnType == "VkResult" ) && !commandData.errorCodes.empty() );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, {}, { nonConstPointerIndex }, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, false, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  assert( beginsWith( commandData.params[nonConstPointerIndex].type.type, "Vk" ) );
  std::string returnType =
    "VULKAN_HPP_NAMESPACE::" + stripPrefix( commandData.params[nonConstPointerIndex].type.type, "Vk" );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS VULKAN_HPP_INLINE typename ResultValueType<StructureChain<X, Y, Z...>>::type ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    StructureChain<X, Y, Z...> structureChain;
    ${returnType} & ${returnVariable} = structureChain.template get<${returnType}>();
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    return createResultValue( result, structureChain, VULKAN_HPP_NAMESPACE_STRING"::${className}${classSeparator}${commandName}"${successCodeList} );
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, { nonConstPointerIndex }, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "returnVariable", startLowerCase( stripPrefix( commandData.params[nonConstPointerIndex].name, "p" ) ) },
        { "returnType", returnType },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename X, typename Y, typename... Z, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS typename ResultValueType<StructureChain<X, Y, Z...>>::type ${commandName}( ${argumentList} ) const;)";

    return replaceWithMap( functionTemplate, { { "argumentList", argumentList }, { "commandName", commandName } } );
  }
}

std::string VulkanHppGenerator::constructCommandResultGetHandleUnique( std::string const & name,
                                                                       CommandData const & commandData,
                                                                       size_t              initialSkipCount,
                                                                       bool                definition,
                                                                       size_t              nonConstPointerIndex ) const
{
  assert( ( commandData.returnType == "VkResult" ) && ( commandData.successCodes.size() == 1 ) );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, {}, { nonConstPointerIndex }, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, false, false, true );
  std::string commandName =
    determineCommandName( name, initialSkipCount ? commandData.params[initialSkipCount - 1].type.type : "", m_tags );
  std::string nodiscard = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string returnBaseType = commandData.params[nonConstPointerIndex].type.compose();
  assert( endsWith( returnBaseType, "*" ) );
  returnBaseType.pop_back();

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename Dispatch>
  ${nodiscard}VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<${returnBaseType}, Dispatch>>::type ${className}${classSeparator}${commandName}Unique( ${argumentList} )${const}
  {
    ${returnBaseType} ${returnValueName};
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    ${ObjectDeleter}<${parentName}, Dispatch> deleter( ${this}${allocator}d );
    return createResultValue<${returnBaseType}, Dispatch>( result, ${returnValueName}, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}Unique", deleter );
  })";

    std::string objectDeleter, allocator;
    if ( ( name.find( "Acquire" ) != std::string::npos ) || ( name.find( "Get" ) != std::string::npos ) )
    {
      if ( ( name == "vkAcquirePerformanceConfigurationINTEL" ) || ( name == "vkGetRandROutputDisplayEXT" ) ||
           ( name == "vkGetWinrtDisplayNV" ) || ( name == "vkGetDrmDisplayEXT" ))
      {
        objectDeleter = "ObjectRelease";
      }
      else
      {
        throw std::runtime_error( "Found " + name + " which requires special handling for the object deleter" );
      }
    }
    else if ( name.find( "Allocate" ) != std::string::npos )
    {
      objectDeleter = "ObjectFree";
      allocator     = "allocator, ";
    }
    else
    {
      assert( ( name.find( "Create" ) != std::string::npos ) || ( name.find( "Register" ) != std::string::npos ) );
      assert( ( name.find( "Create" ) != std::string::npos ) || ( name == "vkRegisterDeviceEventEXT" ) ||
              ( name == "vkRegisterDisplayEventEXT" ) );
      objectDeleter = "ObjectDestroy";
      allocator     = "allocator, ";
    }
    std::string className =
      initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "";
    std::string parentName =
      ( className.empty() || ( commandData.params[nonConstPointerIndex].type.type == "VkDevice" ) ) ? "NoParent"
                                                                                                    : className;

    return replaceWithMap(
      functionTemplate,
      { { "allocator", allocator },
        { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, { nonConstPointerIndex }, false ) },
        { "className", className },
        { "classSeparator", className.empty() ? "" : "::" },
        { "commandName", commandName },
        { "const", commandData.handle.empty() ? "" : " const" },
        { "nodiscard", nodiscard },
        { "ObjectDeleter", objectDeleter },
        { "parentName", parentName },
        { "returnBaseType", returnBaseType },
        { "returnValueName", startLowerCase( stripPrefix( commandData.params[nonConstPointerIndex].name, "p" ) ) },
        { "this", ( parentName == "NoParent" ) ? "" : "*this, " },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(  template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
  ${nodiscard}VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<${returnBaseType}, Dispatch>>::type ${commandName}Unique( ${argumentList} )${const};)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "const", commandData.handle.empty() ? "" : " const" },
                             { "nodiscard", nodiscard },
                             { "returnBaseType", returnBaseType } } );
  }
}

std::string VulkanHppGenerator::constructCommandResultGetTwoValues(
  std::string const &         name,
  CommandData const &         commandData,
  size_t                      initialSkipCount,
  bool                        definition,
  std::vector<size_t> const & nonConstPointerParamIndices ) const
{
  assert( ( commandData.returnType == "VkResult" ) && ( 1 < commandData.successCodes.size() ) &&
          ( nonConstPointerParamIndices.size() == 2 ) && !commandData.handle.empty() );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, {}, nonConstPointerParamIndices, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, false, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string firstType   = commandData.params[nonConstPointerParamIndices[0]].type.compose();
  assert( endsWith( firstType, "*" ) );
  firstType.pop_back();
  std::string secondType = commandData.params[nonConstPointerParamIndices[1]].type.compose();
  assert( endsWith( secondType, "*" ) );
  secondType.pop_back();
  std::string returnBaseType = "std::pair<" + firstType + ", " + secondType + ">";
  std::string returnType     = constructReturnType( commandData, returnBaseType );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    std::pair<${firstType}, ${secondType}> returnValues;
    ${firstType} & ${firstValueName} = returnValues.first;
    ${secondType} & ${secondValueName} = returnValues.second;
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    return createResultValue( result, returnValues, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}"${successCodeList} );
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, nonConstPointerParamIndices, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "firstType", firstType },
        { "firstValueName",
          startLowerCase( stripPrefix( commandData.params[nonConstPointerParamIndices[0]].name, "p" ) ) },
        { "returnType", returnType },
        { "secondType", secondType },
        { "secondValueName",
          startLowerCase( stripPrefix( commandData.params[nonConstPointerParamIndices[1]].name, "p" ) ) },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    VULKAN_HPP_NODISCARD ${returnType} ${commandName}( ${argumentList} ) const;)";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList }, { "commandName", commandName }, { "returnType", returnType } } );
  }
}

std::string
  VulkanHppGenerator::constructCommandResultGetTwoVectors( std::string const &              name,
                                                           CommandData const &              commandData,
                                                           size_t                           initialSkipCount,
                                                           bool                             definition,
                                                           std::map<size_t, size_t> const & vectorParamIndices ) const
{
  assert( commandData.returnType == "VkResult" );

  assert( commandData.params[0].type.type == commandData.handle );

#if !defined( NDEBUG )
  auto firstVectorParamIt  = vectorParamIndices.begin();
  auto secondVectorParamIt = std::next( firstVectorParamIt );
  assert( firstVectorParamIt->second == secondVectorParamIt->second );
#endif

  std::set<size_t> skippedParameters =
    determineSkippedParams( commandData.params, initialSkipCount, vectorParamIndices, {}, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParameters, {}, definition, false, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck = needsVectorSizeCheck( vectorParamIndices );
  std::string noexceptString = vectorSizeCheck.first ? "VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS" : "VULKAN_HPP_NOEXCEPT";

  if ( definition )
  {
    const std::string functionTemplate =
      R"(  template <typename Dispatch>
  VULKAN_HPP_INLINE Result ${className}${classSeparator}${commandName}( ${argumentList} ) const ${noexcept}
  {${vectorSizeCheck}
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    return createResultValue( result, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}"${successCodeList} );
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced( commandData.params, initialSkipCount, false, {}, {}, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "noexcept", noexceptString },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "vectorSizeCheck",
          vectorSizeCheck.first
            ? constructVectorSizeCheck( name, commandData, initialSkipCount, vectorSizeCheck.second, skippedParameters )
            : "" },
        { "vkCommand", name } } );
  }
  else
  {
    const std::string functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    Result ${commandName}( ${argumentList} ) const ${noexcept};)";

    return replaceWithMap( functionTemplate,
                           {
                             { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "noexcept", noexceptString },
                           } );
  }
}

std::string VulkanHppGenerator::constructCommandResultGetValue( std::string const & name,
                                                                CommandData const & commandData,
                                                                size_t              initialSkipCount,
                                                                bool                definition,
                                                                size_t              nonConstPointerIndex ) const
{
  assert( commandData.returnType == "VkResult" );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, {}, { nonConstPointerIndex }, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, false, false, true );
  std::string commandName =
    determineCommandName( name, initialSkipCount ? commandData.params[initialSkipCount - 1].type.type : "", m_tags );
  std::string nodiscard = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string returnBaseType = commandData.params[nonConstPointerIndex].type.compose();
  assert( endsWith( returnBaseType, "*" ) );
  returnBaseType.pop_back();
  std::string returnType = constructReturnType( commandData, returnBaseType );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename Dispatch>
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} )${const}
  {
    ${returnBaseType} ${returnValueName};
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    return createResultValue( result, ${returnValueName}, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}"${successCodeList} );
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, { nonConstPointerIndex }, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "const", commandData.handle.empty() ? "" : " const" },
        { "commandName", commandName },
        { "returnBaseType", returnBaseType },
        { "returnValueName", startLowerCase( stripPrefix( commandData.params[nonConstPointerIndex].name, "p" ) ) },
        { "nodiscard", nodiscard },
        { "returnType", returnType },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ${nodiscard}${returnType} ${commandName}( ${argumentList} )${const};)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "const", commandData.handle.empty() ? "" : " const" },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType } } );
  }
}

std::string
  VulkanHppGenerator::constructCommandResultGetValueDeprecated( std::string const &              name,
                                                                CommandData const &              commandData,
                                                                size_t                           initialSkipCount,
                                                                bool                             definition,
                                                                std::map<size_t, size_t> const & vectorParamIndices,
                                                                size_t returnParamIndex ) const
{
  assert( commandData.returnType == "VkResult" );
  assert( ( vectorParamIndices.find( returnParamIndex ) == vectorParamIndices.end() ) );

  std::string argumentList = constructFunctionHeaderArgumentsEnhanced(
    commandData, returnParamIndex, INVALID_INDEX, vectorParamIndices, !definition, false );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );

  assert( !beginsWith( commandData.params[returnParamIndex].type.type, "Vk" ) );
  std::string returnType = commandData.params[returnParamIndex].type.type;

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename Dispatch>
  VULKAN_HPP_DEPRECATED( "This function is deprecated. Use one of the other flavours of it.")
  ${nodiscard}VULKAN_HPP_INLINE typename ResultValueType<${returnType}>::type ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    ${functionBody}
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "functionBody",
          constructFunctionBodyEnhanced( "  ",
                                         name,
                                         commandData,
                                         initialSkipCount,
                                         returnParamIndex,
                                         INVALID_INDEX,
                                         vectorParamIndices,
                                         false,
                                         returnType,
                                         false ) },
        { "nodiscard", nodiscard },
        { "returnType", returnType } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ${nodiscard}typename ResultValueType<${returnType}>::type ${commandName}( ${argumentList} ) const;)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType } } );
  }
}

std::string VulkanHppGenerator::constructCommandResultGetVector( std::string const &              name,
                                                                 CommandData const &              commandData,
                                                                 size_t                           initialSkipCount,
                                                                 bool                             definition,
                                                                 std::map<size_t, size_t> const & vectorParamIndices,
                                                                 size_t returnParamIndex ) const
{
  assert( commandData.returnType == "VkResult" );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, vectorParamIndices, { returnParamIndex }, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, false, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard  = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string returnType = constructReturnType( commandData, "std::vector<T,Allocator>" );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename T, typename Allocator, typename Dispatch>
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    VULKAN_HPP_ASSERT( ${dataSize} % sizeof( T ) == 0 );
    std::vector<T,Allocator> ${dataName}( ${dataSize} / sizeof( T ) );
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    return createResultValue( result, ${dataName}, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}"${successCodeList} );
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, { returnParamIndex }, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "dataName", startLowerCase( stripPrefix( commandData.params[returnParamIndex].name, "p" ) ) },
        { "dataSize", commandData.params[returnParamIndex].len },
        { "nodiscard", nodiscard },
        { "returnType", returnType },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename T, typename Allocator = std::allocator<T>, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ${nodiscard}${returnType} ${commandName}( ${argumentList} ) const;)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType } } );
  }
}

std::string VulkanHppGenerator::constructCommandResultGetVector( std::string const &              name,
                                                                 CommandData const &              commandData,
                                                                 size_t                           initialSkipCount,
                                                                 bool                             definition,
                                                                 std::map<size_t, size_t> const & vectorParamIndices,
                                                                 size_t                           returnParamIndex,
                                                                 bool                             withAllocator ) const
{
  assert( !commandData.handle.empty() && ( commandData.returnType == "VkResult" ) );
  assert( vectorParamIndices.size() == 2 );
  assert( vectorParamIndices.find( returnParamIndex ) != vectorParamIndices.end() );
  assert( vectorParamIndices.begin()->second == std::next( vectorParamIndices.begin() )->second );
  assert( commandData.returnType == "VkResult" );

  std::set<size_t> skippedParameters =
    determineSkippedParams( commandData.params, initialSkipCount, vectorParamIndices, { returnParamIndex }, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParameters, {}, definition, withAllocator, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );

  assert( beginsWith( commandData.params[returnParamIndex].type.type, "Vk" ) );
  std::string vectorElementType = stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" );
  std::string allocatorType     = startUpperCase( vectorElementType ) + "Allocator";

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename ${allocatorType}, typename Dispatch${typenameCheck}>
  ${nodiscard}VULKAN_HPP_INLINE typename ResultValueType<std::vector<${vectorElementType}, ${allocatorType}>>::type ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    std::vector<${vectorElementType}, ${allocatorType}> ${vectorName}( ${vectorSize}${allocateInitializer} );
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    return createResultValue( result, ${vectorName}, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}"${successCodeList} );
  })";

    std::string typenameCheck = withAllocator
                                  ? ( ", typename B, typename std::enable_if<std::is_same<typename B::value_type, " +
                                      vectorElementType + ">::value, int>::type " )
                                  : "";

    return replaceWithMap(
      functionTemplate,
      { { "allocateInitializer", withAllocator ? ( ", " + startLowerCase( allocatorType ) ) : "" },
        { "allocatorType", allocatorType },
        { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, { returnParamIndex }, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "nodiscard", nodiscard },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "typenameCheck", typenameCheck },
        { "vectorElementType", vectorElementType },
        { "vectorName", startLowerCase( stripPrefix( commandData.params[returnParamIndex].name, "p" ) ) },
        { "vectorSize",
          startLowerCase( stripPrefix( commandData.params[vectorParamIndices.begin()->first].name, "p" ) ) +
            ".size()" },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename ${allocatorType} = std::allocator<${vectorElementType}>, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE${typenameCheck}>
    ${nodiscard}typename ResultValueType<std::vector<${vectorElementType}, ${allocatorType}>>::type ${commandName}( ${argumentList} ) const;)";

    std::string typenameCheck = withAllocator ? ( ", typename B = " + allocatorType +
                                                  ", typename std::enable_if<std::is_same<typename B::value_type, " +
                                                  vectorElementType + ">::value, int>::type = 0" )
                                              : "";

    return replaceWithMap( functionTemplate,
                           { { "allocatorType", allocatorType },
                             { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "typenameCheck", typenameCheck },
                             { "vectorElementType", vectorElementType } } );
  }
}

std::string
  VulkanHppGenerator::constructCommandResultGetVectorAndValue( std::string const &              name,
                                                               CommandData const &              commandData,
                                                               size_t                           initialSkipCount,
                                                               bool                             definition,
                                                               std::map<size_t, size_t> const & vectorParamIndices,
                                                               std::vector<size_t> const &      returnParamIndices,
                                                               bool                             withAllocator ) const
{
  assert( !commandData.handle.empty() && ( commandData.returnType == "VkResult" ) );
  assert( ( vectorParamIndices.size() == 2 ) && ( returnParamIndices.size() == 2 ) );
  assert( vectorParamIndices.find( returnParamIndices[0] ) != vectorParamIndices.end() );
  assert( vectorParamIndices.find( returnParamIndices[1] ) == vectorParamIndices.end() );
  assert( vectorParamIndices.begin()->second == std::next( vectorParamIndices.begin() )->second );
  assert( commandData.returnType == "VkResult" );

  std::set<size_t> skippedParameters =
    determineSkippedParams( commandData.params, initialSkipCount, vectorParamIndices, returnParamIndices, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParameters, {}, definition, withAllocator, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard  = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string returnType = constructReturnType( commandData, "std::vector<T,Allocator>" );

  assert( !beginsWith( commandData.params[returnParamIndices[0]].type.type, "Vk" ) );
  std::string vectorElementType = commandData.params[returnParamIndices[0]].type.type;
  std::string allocatorType     = startUpperCase( vectorElementType ) + "Allocator";
  assert( !beginsWith( commandData.params[returnParamIndices[1]].type.type, "Vk" ) );
  std::string valueType = commandData.params[returnParamIndices[1]].type.type;

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename ${allocatorType}, typename Dispatch${typenameCheck}>
  ${nodiscard}VULKAN_HPP_INLINE typename ResultValueType<std::pair<std::vector<${vectorElementType}, ${allocatorType}>, ${valueType}>>::type ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    std::pair<std::vector<${vectorElementType}, ${allocatorType}>,${valueType}> data( std::piecewise_construct, std::forward_as_tuple( ${vectorSize}${allocateInitializer} ), std::forward_as_tuple( 0 ) );
    std::vector<${vectorElementType}, ${allocatorType}> & ${vectorName} = data.first;
    ${valueType} & ${valueName} = data.second;
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    return createResultValue( result, data, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}"${successCodeList} );
  })";

    std::string typenameCheck = withAllocator
                                  ? ( ", typename B, typename std::enable_if<std::is_same<typename B::value_type, " +
                                      vectorElementType + ">::value, int>::type " )
                                  : "";

    return replaceWithMap(
      functionTemplate,
      { { "allocateInitializer", withAllocator ? ( ", " + startLowerCase( allocatorType ) ) : "" },
        { "allocatorType", allocatorType },
        { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, returnParamIndices, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "nodiscard", nodiscard },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "typenameCheck", typenameCheck },
        { "valueName", startLowerCase( stripPrefix( commandData.params[returnParamIndices[1]].name, "p" ) ) },
        { "valueType", valueType },
        { "vectorElementType", vectorElementType },
        { "vectorName", startLowerCase( stripPrefix( commandData.params[returnParamIndices[0]].name, "p" ) ) },
        { "vectorSize",
          startLowerCase( stripPrefix( commandData.params[vectorParamIndices.begin()->first].name, "p" ) ) +
            ".size()" },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename ${allocatorType} = std::allocator<${vectorElementType}>, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE${typenameCheck}>
    ${nodiscard}typename ResultValueType<std::pair<std::vector<${vectorElementType}, ${allocatorType}>, ${valueType}>>::type ${commandName}( ${argumentList} ) const;)";

    std::string typenameCheck = withAllocator ? ( ", typename B = " + allocatorType +
                                                  ", typename std::enable_if<std::is_same<typename B::value_type, " +
                                                  vectorElementType + ">::value, int>::type = 0" )
                                              : "";

    return replaceWithMap( functionTemplate,
                           { { "allocatorType", allocatorType },
                             { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "typenameCheck", typenameCheck },
                             { "valueType", valueType },
                             { "vectorElementType", vectorElementType } } );
  }
}

std::string
  VulkanHppGenerator::constructCommandResultGetVectorDeprecated( std::string const &              name,
                                                                 CommandData const &              commandData,
                                                                 size_t                           initialSkipCount,
                                                                 bool                             definition,
                                                                 std::map<size_t, size_t> const & vectorParamIndices,
                                                                 size_t returnParamIndex ) const
{
  assert( commandData.returnType == "VkResult" );

  std::string argumentList = constructFunctionHeaderArgumentsEnhanced(
    commandData, INVALID_INDEX, returnParamIndex, vectorParamIndices, !definition, false );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard  = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string returnType = constructReturnType( commandData, "void" );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename T, typename Dispatch>
  VULKAN_HPP_DEPRECATED( "This function is deprecated. Use one of the other flavours of it.")
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    ${functionBody}
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "functionBody",
          constructFunctionBodyEnhanced( "  ",
                                         name,
                                         commandData,
                                         initialSkipCount,
                                         INVALID_INDEX,
                                         returnParamIndex,
                                         vectorParamIndices,
                                         false,
                                         "void",
                                         false ) },
        { "nodiscard", nodiscard },
        { "returnType", returnType } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename T, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ${nodiscard}${returnType} ${commandName}( ${argumentList} ) const;)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType } } );
  }
}

std::string
  VulkanHppGenerator::constructCommandResultGetVectorOfHandles( std::string const &              name,
                                                                CommandData const &              commandData,
                                                                size_t                           initialSkipCount,
                                                                bool                             definition,
                                                                std::map<size_t, size_t> const & vectorParamIndices,
                                                                size_t                           returnParamIndex,
                                                                bool                             withAllocator ) const
{
  assert( commandData.returnType == "VkResult" );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, vectorParamIndices, { returnParamIndex }, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, withAllocator, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard  = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string handleType = stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" );
  std::string returnType =
    ( commandData.successCodes.size() == 1 )
      ? ( "typename ResultValueType<std::vector<" + handleType + ", " + handleType + "Allocator>>::type" )
      : ( "ResultValue<std::vector<" + handleType + ", " + handleType + "Allocator>>" );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename ${handleType}Allocator, typename Dispatch${typenameCheck}>
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    std::vector<${handleType}, ${handleType}Allocator> ${vectorName}( ${vectorSize}${vectorAllocator} );
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    return createResultValue( result, ${vectorName}, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}"${successCodeList} );
  })";

    std::string typenameCheck = withAllocator
                                  ? ( ", typename B, typename std::enable_if<std::is_same<typename B::value_type, " +
                                      handleType + ">::value, int>::type " )
                                  : "";
    std::string vectorName    = startLowerCase( stripPrefix( commandData.params[returnParamIndex].name, "p" ) );

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, { returnParamIndex }, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "nodiscard", nodiscard },
        { "handleType", handleType },
        { "returnType", returnType },
        { "typenameCheck", typenameCheck },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "vectorAllocator", withAllocator ? ( ", " + startLowerCase( handleType ) + "Allocator" ) : "" },
        { "vectorName", vectorName },
        { "vectorSize", getVectorSize( commandData.params, vectorParamIndices, returnParamIndex ) },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename ${handleType}Allocator = std::allocator<${handleType}>, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE${typenameCheck}>
    ${nodiscard}${returnType} ${commandName}( ${argumentList} ) const;)";

    std::string typenameCheck = withAllocator
                                  ? ( ", typename B = " + handleType +
                                      "Allocator, typename std::enable_if<std::is_same<typename B::value_type, " +
                                      handleType + ">::value, int>::type = 0" )
                                  : "";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "handleType", handleType },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType },
                             { "typenameCheck", typenameCheck } } );
  }
}

std::string VulkanHppGenerator::constructCommandResultGetVectorOfHandlesSingular(
  std::string const &              name,
  CommandData const &              commandData,
  size_t                           initialSkipCount,
  bool                             definition,
  std::map<size_t, size_t> const & vectorParamIndices,
  size_t                           returnParamIndex ) const
{
  assert( ( vectorParamIndices.size() == 2 ) &&
          ( vectorParamIndices.begin()->second == std::next( vectorParamIndices.begin() )->second ) );
  assert( commandData.params[vectorParamIndices.begin()->second].type.isValue() );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, vectorParamIndices, { returnParamIndex }, true );
  std::set<size_t> singularParams = determineSingularParams( returnParamIndex, vectorParamIndices );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, singularParams, definition, false, false, true );
  std::string commandName =
    stripPluralS( determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags ) );
  std::string nodiscard  = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string handleType = stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" );
  std::string returnType = ( commandData.successCodes.size() == 1 )
                             ? ( "typename ResultValueType<" + handleType + ">::type" )
                             : ( "ResultValue<" + handleType + ">" );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename Dispatch>
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    ${handleType} ${handleName};
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    return createResultValue( result, ${handleName}, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}"${successCodeList} );
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, singularParams, { returnParamIndex }, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "nodiscard", nodiscard },
        { "handleName",
          stripPluralS( startLowerCase( stripPrefix( commandData.params[returnParamIndex].name, "p" ) ) ) },
        { "handleType", handleType },
        { "returnType", returnType },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ${nodiscard}${returnType} ${commandName}( ${argumentList} ) const;)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType } } );
  }
}

std::string VulkanHppGenerator::constructCommandResultGetVectorOfHandlesUnique(
  std::string const &              name,
  CommandData const &              commandData,
  size_t                           initialSkipCount,
  bool                             definition,
  std::map<size_t, size_t> const & vectorParamIndices,
  size_t                           returnParamIndex,
  bool                             withAllocator ) const
{
  assert( commandData.returnType == "VkResult" );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, vectorParamIndices, { returnParamIndex }, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, withAllocator, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard  = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string handleType = stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" );
  std::string returnType =
    ( commandData.successCodes.size() == 1 )
      ? ( "typename ResultValueType<std::vector<UniqueHandle<" + handleType + ", Dispatch>, " + handleType +
          "Allocator>>::type" )
      : ( "ResultValue<std::vector<UniqueHandle<" + handleType + ", Dispatch>, " + handleType + "Allocator>>" );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename Dispatch, typename ${handleType}Allocator${typenameCheck}>
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}Unique( ${argumentList} ) const
  {
    std::vector<UniqueHandle<${handleType}, Dispatch>, ${handleType}Allocator> ${uniqueVectorName}${vectorAllocator};
    std::vector<${handleType}> ${vectorName}( ${vectorSize} );
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    if ( ${successCheck} )
    {
      ${uniqueVectorName}.reserve( ${vectorSize} );
      ${deleterDefinition};
      for ( size_t i=0; i < ${vectorSize}; i++ )
      {
        ${uniqueVectorName}.push_back( UniqueHandle<${handleType}, Dispatch>( ${vectorName}[i], deleter ) );
      }
    }
    return createResultValue( result, std::move( ${uniqueVectorName} ), VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}Unique"${successCodeList} );
  })";

    std::string className =
      initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "";

    std::string              deleterDefinition;
    std::vector<std::string> lenParts = tokenize( commandData.params[returnParamIndex].len, "->" );
    switch ( lenParts.size() )
    {
      case 1: deleterDefinition = "ObjectDestroy<" + className + ", Dispatch> deleter( *this, allocator, d )"; break;
      case 2:
        {
          auto vpiIt = vectorParamIndices.find( returnParamIndex );
          assert( vpiIt != vectorParamIndices.end() );
          std::string poolType, poolName;
          std::tie( poolType, poolName ) = getPoolTypeAndName( commandData.params[vpiIt->second].type.type );
          assert( !poolType.empty() );
          poolType = stripPrefix( poolType, "Vk" );
          poolName = startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + poolName;

          deleterDefinition =
            "PoolFree<" + className + ", " + poolType + ", Dispatch> deleter( *this, " + poolName + ", d )";
        }
        break;
    }

    std::string typenameCheck =
      withAllocator ? ( ", typename B, typename std::enable_if<std::is_same<typename B::value_type, UniqueHandle<" +
                        handleType + ", Dispatch>>::value, int>::type " )
                    : "";
    std::string vectorName = startLowerCase( stripPrefix( commandData.params[returnParamIndex].name, "p" ) );

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, { returnParamIndex }, false ) },
        { "className", className },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "deleterDefinition", deleterDefinition },
        { "handleType", handleType },
        { "nodiscard", nodiscard },
        { "returnType", returnType },
        { "successCheck", constructSuccessCheck( commandData.successCodes ) },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "typenameCheck", typenameCheck },
        { "uniqueVectorName", "unique" + stripPrefix( commandData.params[returnParamIndex].name, "p" ) },
        { "vectorAllocator", withAllocator ? ( "( " + startLowerCase( handleType ) + "Allocator )" ) : "" },
        { "vectorName", vectorName },
        { "vectorSize", getVectorSize( commandData.params, vectorParamIndices, returnParamIndex ) },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE, typename ${handleType}Allocator = std::allocator<UniqueHandle<${handleType}, Dispatch>>${typenameCheck}>
    ${nodiscard}${returnType} ${commandName}Unique( ${argumentList} ) const;)";

    std::string typenameCheck =
      withAllocator ? ( ", typename B = " + handleType +
                        "Allocator, typename std::enable_if<std::is_same<typename B::value_type, UniqueHandle<" +
                        handleType + ", Dispatch>>::value, int>::type = 0" )
                    : "";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "handleType", handleType },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType },
                             { "typenameCheck", typenameCheck } } );
  }
}

std::string VulkanHppGenerator::constructCommandResultGetVectorOfHandlesUniqueSingular(
  std::string const &              name,
  CommandData const &              commandData,
  size_t                           initialSkipCount,
  bool                             definition,
  std::map<size_t, size_t> const & vectorParamIndices,
  size_t                           returnParamIndex ) const
{
  assert( ( vectorParamIndices.size() == 2 ) &&
          ( vectorParamIndices.begin()->second == std::next( vectorParamIndices.begin() )->second ) );
  assert( commandData.params[vectorParamIndices.begin()->second].type.isValue() );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, vectorParamIndices, { returnParamIndex }, true );
  std::set<size_t> singularParams = determineSingularParams( returnParamIndex, vectorParamIndices );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, singularParams, definition, false, false, true );
  std::string commandName =
    stripPluralS( determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags ) );
  std::string nodiscard  = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string handleType = stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" );
  std::string returnType = ( commandData.successCodes.size() == 1 )
                             ? ( "typename ResultValueType<UniqueHandle<" + handleType + ", Dispatch>>::type" )
                             : ( "ResultValue<UniqueHandle<" + handleType + ", Dispatch>>" );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename Dispatch>
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}Unique( ${argumentList} ) const
  {
    ${handleType} ${handleName};
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    ObjectDestroy<${className}, Dispatch> deleter( *this, allocator, d );
    return createResultValue<${handleType}, Dispatch>( result, ${handleName}, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}Unique"${successCodeList}, deleter );
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, singularParams, { returnParamIndex }, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "handleName",
          stripPluralS( startLowerCase( stripPrefix( commandData.params[returnParamIndex].name, "p" ) ) ) },
        { "handleType", handleType },
        { "nodiscard", nodiscard },
        { "returnType", returnType },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ${nodiscard}${returnType} ${commandName}Unique( ${argumentList} ) const;)";

    return replaceWithMap( functionTemplate,
                           {
                             { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType },
                           } );
  }
}

std::string VulkanHppGenerator::constructCommandResultGetVectorOfVoidSingular(
  std::string const &              name,
  CommandData const &              commandData,
  size_t                           initialSkipCount,
  bool                             definition,
  std::map<size_t, size_t> const & vectorParamIndices,
  size_t                           returnParamIndex ) const
{
  assert( commandData.returnType == "VkResult" );
  assert( commandData.params[returnParamIndex].type.type == "void" );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, vectorParamIndices, { returnParamIndex }, true );
  std::set<size_t> singularParams = determineSingularParams( returnParamIndex, vectorParamIndices );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, false, false, true );
  std::string commandName =
    stripPluralS( determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags ) );
  std::string nodiscard  = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string returnType = constructReturnType( commandData, "T" );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename T, typename Dispatch>
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    T ${dataName};
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    return createResultValue( result, ${dataName}, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}"${successCodeList} );
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, singularParams, { returnParamIndex }, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "dataName", startLowerCase( stripPrefix( commandData.params[returnParamIndex].name, "p" ) ) },
        { "nodiscard", nodiscard },
        { "returnType", returnType },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename T, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ${nodiscard}${returnType} ${commandName}( ${argumentList} ) const;)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType } } );
  }
}

std::string
  VulkanHppGenerator::constructCommandResultGetVectorSingular( std::string const &              name,
                                                               CommandData const &              commandData,
                                                               size_t                           initialSkipCount,
                                                               bool                             definition,
                                                               std::map<size_t, size_t> const & vectorParamIndices,
                                                               size_t                           returnParamIndex ) const
{
  assert( commandData.returnType == "VkResult" );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, vectorParamIndices, { returnParamIndex }, true );
  std::set<size_t> singularParams = determineSingularParams( returnParamIndex, vectorParamIndices );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, singularParams, definition, false, false, true );
  std::string commandName =
    stripPluralS( determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags ) );
  std::string nodiscard  = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string dataType   = stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" );
  std::string returnType = constructReturnType( commandData, dataType );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename Dispatch>
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    ${dataType} ${dataName};
    Result result = static_cast<Result>( d.${vkCommand}( ${callArguments} ) );
    return createResultValue( result, ${dataName}, VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}"${successCodeList} );
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, singularParams, { returnParamIndex }, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "dataName", stripPluralS( startLowerCase( stripPrefix( commandData.params[returnParamIndex].name, "p" ) ) ) },
        { "dataType", dataType },
        { "nodiscard", nodiscard },
        { "returnType", returnType },
        { "successCodeList", constructSuccessCodeList( commandData.successCodes ) },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ${nodiscard}${returnType} ${commandName}( ${argumentList} ) const;)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType } } );
  }
}

std::string VulkanHppGenerator::constructCommandStandard( std::string const & name,
                                                          CommandData const & commandData,
                                                          size_t              initialSkipCount,
                                                          bool                definition ) const
{
  std::set<size_t> skippedParams = determineSkippedParams( commandData.params, initialSkipCount, {}, {}, false );

  std::string argumentList = constructArgumentListStandard( commandData.params, skippedParams );
  std::string commandName =
    determineCommandName( name, initialSkipCount ? commandData.params[initialSkipCount - 1].type.type : "", m_tags );
  std::string nodiscard  = constructNoDiscardStandard( commandData );
  std::string returnType = stripPrefix( commandData.returnType, "Vk" );

  if ( definition )
  {
    std::string functionBody =
      "d." + name + "( " + constructCallArgumentsStandard( commandData.handle, commandData.params ) + " )";
    if ( beginsWith( commandData.returnType, "Vk" ) )
    {
      functionBody = "return static_cast<" + returnType + ">( " + functionBody + " )";
    }
    else if ( commandData.returnType != "void" )
    {
      functionBody = "return " + functionBody;
    }

    std::string const functionTemplate =
      R"(  template <typename Dispatch>
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} )${const} VULKAN_HPP_NOEXCEPT
  {
    ${functionBody};
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "const", commandData.handle.empty() ? "" : " const" },
        { "functionBody", functionBody },
        { "nodiscard", nodiscard },
        { "returnType", returnType } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ${nodiscard}${returnType} ${commandName}( ${argumentList} VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT )${const} VULKAN_HPP_NOEXCEPT;)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "const", commandData.handle.empty() ? "" : " const" },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType } } );
  }
}

std::string VulkanHppGenerator::constructCommandType( std::string const & name,
                                                      CommandData const & commandData,
                                                      size_t              initialSkipCount,
                                                      bool                definition ) const
{
  assert( ( commandData.returnType != "VkResult" ) && ( commandData.returnType != "void" ) &&
          commandData.successCodes.empty() && commandData.errorCodes.empty() );

  std::set<size_t> skippedParameters = determineSkippedParams( commandData.params, initialSkipCount, {}, {}, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParameters, {}, definition, false, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard  = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string returnType = stripPrefix( commandData.returnType, "Vk" );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename Dispatch>
  ${nodiscard}VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT
  {
    return d.${vkCommand}( ${callArguments} );
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced( commandData.params, initialSkipCount, false, {}, {}, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "nodiscard", nodiscard },
        { "returnType", returnType },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    ${nodiscard}${returnType} ${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT;)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "nodiscard", nodiscard },
                             { "returnType", returnType } } );
  }
}

std::string VulkanHppGenerator::constructCommandVoid( std::string const &              name,
                                                      CommandData const &              commandData,
                                                      size_t                           initialSkipCount,
                                                      bool                             definition,
                                                      std::map<size_t, size_t> const & vectorParamIndices ) const
{
  assert( ( commandData.returnType == "void" ) && commandData.successCodes.empty() && commandData.errorCodes.empty() );

  std::set<size_t> skippedParameters =
    determineSkippedParams( commandData.params, initialSkipCount, vectorParamIndices, {}, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParameters, {}, definition, false, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string typenameT   = ( ( vectorParamIndices.size() == 1 ) &&
                            ( commandData.params[vectorParamIndices.begin()->first].type.type == "void" ) )
                              ? "typename T, "
                              : "";
  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck = needsVectorSizeCheck( vectorParamIndices );
  std::string noexceptString = vectorSizeCheck.first ? "VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS" : "VULKAN_HPP_NOEXCEPT";

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <${typenameT}typename Dispatch>
  VULKAN_HPP_INLINE void ${className}${classSeparator}${commandName}( ${argumentList} ) const ${noexcept}
  {${vectorSizeCheck}
    d.${vkCommand}( ${callArguments} );
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced( commandData.params, initialSkipCount, false, {}, {}, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "noexcept", noexceptString },
        { "typenameT", typenameT },
        { "vectorSizeCheck",
          vectorSizeCheck.first
            ? constructVectorSizeCheck( name, commandData, initialSkipCount, vectorSizeCheck.second, skippedParameters )
            : "" },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <${typenameT}typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    void ${commandName}( ${argumentList} ) const ${noexcept};)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "noexcept", noexceptString },
                             { "typenameT", typenameT } } );
  }
}

std::string VulkanHppGenerator::constructCommandVoidEnumerate( std::string const &               name,
                                                               CommandData const &               commandData,
                                                               size_t                            initialSkipCount,
                                                               bool                              definition,
                                                               std::pair<size_t, size_t> const & vectorParamIndex,
                                                               std::vector<size_t> const &       returnParamIndices,
                                                               bool                              withAllocators ) const
{
  assert( commandData.params[0].type.type == commandData.handle && ( commandData.returnType == "void" ) &&
          commandData.successCodes.empty() && commandData.errorCodes.empty() );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, { vectorParamIndex }, returnParamIndices, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, withAllocators, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string vectorElementType = stripPrefix( commandData.params[vectorParamIndex.first].type.type, "Vk" );

  if ( definition )
  {
    const std::string functionTemplate =
      R"(  template <typename ${vectorElementType}Allocator, typename Dispatch${typenameCheck}>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<${vectorElementType}, ${vectorElementType}Allocator> ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    std::vector<${vectorElementType}, ${vectorElementType}Allocator> ${vectorName}${vectorAllocator};
    ${counterType} ${counterName};
    d.${vkCommand}( ${firstCallArguments} );
    ${vectorName}.resize( ${counterName} );
    d.${vkCommand}( ${secondCallArguments} );
    VULKAN_HPP_ASSERT( ${counterName} <= ${vectorName}.size() );
    return ${vectorName};
  })";

    std::string vectorName    = startLowerCase( stripPrefix( commandData.params[vectorParamIndex.first].name, "p" ) );
    std::string typenameCheck = withAllocators
                                  ? ( ", typename B, typename std::enable_if<std::is_same<typename B::value_type, " +
                                      vectorElementType + ">::value, int>::type " )
                                  : "";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "counterName", startLowerCase( stripPrefix( commandData.params[vectorParamIndex.second].name, "p" ) ) },
        { "counterType", commandData.params[vectorParamIndex.second].type.type },
        { "firstCallArguments",
          constructCallArgumentsEnhanced( commandData.params, initialSkipCount, true, {}, returnParamIndices, false ) },
        { "secondCallArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, returnParamIndices, false ) },
        { "typenameCheck", typenameCheck },
        { "vectorAllocator",
          withAllocators
            ? ( "( " + startLowerCase( stripPrefix( commandData.params[vectorParamIndex.first].type.type, "Vk" ) ) +
                "Allocator )" )
            : "" },
        { "vectorElementType", vectorElementType },
        { "vectorName", vectorName },
        { "vkCommand", name } } );
  }
  else
  {
    const std::string functionTemplate =
      R"(    template <typename ${vectorElementType}Allocator = std::allocator<${vectorElementType}>, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE${typenameCheck}>
    VULKAN_HPP_NODISCARD std::vector<${vectorElementType}, ${vectorElementType}Allocator> ${commandName}( ${argumentList} ) const;)";

    std::string typenameCheck = withAllocators
                                  ? ( ", typename B = " + vectorElementType +
                                      "Allocator, typename std::enable_if<std::is_same<typename B::value_type, " +
                                      vectorElementType + ">::value, int>::type = 0" )
                                  : "";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "typenameCheck", typenameCheck },
                             { "vectorElementType", vectorElementType } } );
  }
}

std::string
  VulkanHppGenerator::constructCommandVoidEnumerateChained( std::string const &               name,
                                                            CommandData const &               commandData,
                                                            size_t                            initialSkipCount,
                                                            bool                              definition,
                                                            std::pair<size_t, size_t> const & vectorParamIndex,
                                                            std::vector<size_t> const &       returnParamIndices,
                                                            bool                              withAllocators ) const
{
  assert( ( commandData.params[0].type.type == commandData.handle ) && ( commandData.returnType == "void" ) &&
          commandData.successCodes.empty() && commandData.errorCodes.empty() );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, { vectorParamIndex }, returnParamIndices, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, withAllocators, true, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  assert( beginsWith( commandData.params[vectorParamIndex.first].type.type, "Vk" ) );
  std::string vectorElementType =
    "VULKAN_HPP_NAMESPACE::" + stripPrefix( commandData.params[vectorParamIndex.first].type.type, "Vk" );

  if ( definition )
  {
    const std::string functionTemplate =
      R"(  template <typename StructureChain, typename StructureChainAllocator, typename Dispatch${typenameCheck}>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<StructureChain, StructureChainAllocator> ${className}${classSeparator}${commandName}( ${argumentList} ) const
  {
    ${counterType} ${counterName};
    d.${vkCommand}( ${firstCallArguments} );
    std::vector<StructureChain, StructureChainAllocator> returnVector( ${counterName}${structureChainAllocator} );
    std::vector<${vectorElementType}> ${vectorName}( ${counterName} );
    for ( ${counterType} i = 0; i < ${counterName}; i++ )
    {
      ${vectorName}[i].pNext =
        returnVector[i].template get<${vectorElementType}>().pNext;
    }
    d.${vkCommand}( ${secondCallArguments} );
    VULKAN_HPP_ASSERT( ${counterName} <= ${vectorName}.size() );
    for ( ${counterType} i = 0; i < ${counterName}; i++ )
    {
      returnVector[i].template get<${vectorElementType}>() = ${vectorName}[i];
    }
    return returnVector;
  })";

    std::string vectorName = startLowerCase( stripPrefix( commandData.params[vectorParamIndex.first].name, "p" ) );
    std::string typenameCheck =
      withAllocators
        ? ( ", typename B, typename std::enable_if<std::is_same<typename B::value_type, StructureChain>::value, int>::type" )
        : "";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "counterName", startLowerCase( stripPrefix( commandData.params[vectorParamIndex.second].name, "p" ) ) },
        { "counterType", commandData.params[vectorParamIndex.second].type.type },
        { "firstCallArguments",
          constructCallArgumentsEnhanced( commandData.params, initialSkipCount, true, {}, returnParamIndices, false ) },
        { "secondCallArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, returnParamIndices, false ) },
        { "structureChainAllocator", withAllocators ? ( ", structureChainAllocator" ) : "" },
        { "typenameCheck", typenameCheck },
        { "vectorElementType", vectorElementType },
        { "vectorName", vectorName },
        { "vkCommand", name } } );
  }
  else
  {
    const std::string functionTemplate =
      R"(    template <typename StructureChain, typename StructureChainAllocator = std::allocator<StructureChain>, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE${typenameCheck}>
    VULKAN_HPP_NODISCARD std::vector<StructureChain, StructureChainAllocator> ${commandName}( ${argumentList} ) const;)";

    std::string typenameCheck =
      withAllocators
        ? ( ", typename B = StructureChainAllocator, typename std::enable_if<std::is_same<typename B::value_type, StructureChain>::value, int>::type = 0" )
        : "";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList }, { "commandName", commandName }, { "typenameCheck", typenameCheck } } );
  }
}

std::string VulkanHppGenerator::constructCommandVoidGetChain( std::string const & name,
                                                              CommandData const & commandData,
                                                              size_t              initialSkipCount,
                                                              bool                definition,
                                                              size_t              nonConstPointerIndex ) const
{
  assert( ( commandData.returnType == "void" ) && commandData.successCodes.empty() && commandData.errorCodes.empty() );

  std::set<size_t> skippedParams =
    determineSkippedParams( commandData.params, initialSkipCount, {}, { nonConstPointerIndex }, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParams, {}, definition, false, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  assert( beginsWith( commandData.params[nonConstPointerIndex].type.type, "Vk" ) );
  std::string returnType =
    "VULKAN_HPP_NAMESPACE::" + stripPrefix( commandData.params[nonConstPointerIndex].type.type, "Vk" );

  if ( definition )
  {
    std::string const functionTemplate =
      R"(  template <typename X, typename Y, typename... Z, typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE StructureChain<X, Y, Z...> ${className}${classSeparator}${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT
  {
    StructureChain<X, Y, Z...> structureChain;
    ${returnType} & ${returnVariable} = structureChain.template get<${returnType}>();
    d.${vkCommand}( ${callArguments} );
    return structureChain;
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, { nonConstPointerIndex }, false ) },
        { "className",
          initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "" },
        { "classSeparator", commandData.handle.empty() ? "" : "::" },
        { "commandName", commandName },
        { "returnVariable", startLowerCase( stripPrefix( commandData.params[nonConstPointerIndex].name, "p" ) ) },
        { "returnType", returnType },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename X, typename Y, typename... Z, typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    VULKAN_HPP_NODISCARD StructureChain<X, Y, Z...> ${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT;)";

    return replaceWithMap( functionTemplate, { { "argumentList", argumentList }, { "commandName", commandName } } );
  }
}

std::string VulkanHppGenerator::constructCommandVoidGetValue( std::string const &              name,
                                                              CommandData const &              commandData,
                                                              size_t                           initialSkipCount,
                                                              bool                             definition,
                                                              std::map<size_t, size_t> const & vectorParamIndices,
                                                              size_t                           returnParamIndex ) const
{
  assert( ( commandData.returnType == "void" ) && commandData.successCodes.empty() && commandData.errorCodes.empty() );
  assert( vectorParamIndices.size() <= 1 );
  assert( vectorParamIndices.empty() || ( vectorParamIndices.find( returnParamIndex ) == vectorParamIndices.end() ) );
  assert( vectorParamIndices.empty() || ( vectorParamIndices.begin()->second != INVALID_INDEX ) );

  std::set<size_t> skippedParameters =
    determineSkippedParams( commandData.params, initialSkipCount, {}, { returnParamIndex }, false );

  std::string argumentList =
    constructArgumentListEnhanced( commandData.params, skippedParameters, {}, definition, false, false, true );
  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );
  std::string nodiscard  = determineNoDiscard( 1 < commandData.successCodes.size(), 1 < commandData.errorCodes.size() );
  std::string returnType = stripPostfix( commandData.params[returnParamIndex].type.compose(), "*" );

  bool needsVectorSizeCheck =
    !vectorParamIndices.empty() && isLenByStructMember( commandData.params[vectorParamIndices.begin()->first].len,
                                                        commandData.params[vectorParamIndices.begin()->second] );
  std::string noexceptString = needsVectorSizeCheck ? "VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS" : "VULKAN_HPP_NOEXCEPT";

  if ( definition )
  {
    std::string className =
      initialSkipCount ? stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) : "";
    std::string classSeparator = commandData.handle.empty() ? "" : "::";

    std::string vectorSizeCheck;
    if ( needsVectorSizeCheck )
    {
      std::string const sizeCheckTemplate =
        R"(
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    VULKAN_HPP_ASSERT( ${vectorName}.size() == ${sizeValue} );
#else
    if ( ${vectorName}.size() != ${sizeValue} )
    {
      throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::${className}${classSeparator}${commandName}: ${vectorName}.size() != ${sizeValue}" );
    }
#endif /*VULKAN_HPP_NO_EXCEPTIONS*/)";
      std::vector<std::string> lenParts = tokenize( commandData.params[vectorParamIndices.begin()->first].len, "->" );
      assert( lenParts.size() == 2 );

      vectorSizeCheck = replaceWithMap(
        sizeCheckTemplate,
        { { "className", className },
          { "classSeparator", classSeparator },
          { "commandName", commandName },
          { "sizeValue", startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + lenParts[1] },
          { "vectorName",
            startLowerCase( stripPrefix( commandData.params[vectorParamIndices.begin()->first].name, "p" ) ) } } );
    }

    std::string const functionTemplate =
      R"(  template <typename Dispatch>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ${returnType} ${className}${classSeparator}${commandName}( ${argumentList} ) const ${noexcept}
  {${vectorSizeCheck}
    ${returnType} ${returnVariable};
    d.${vkCommand}( ${callArguments} );
    return ${returnVariable};
  })";

    return replaceWithMap(
      functionTemplate,
      { { "argumentList", argumentList },
        { "callArguments",
          constructCallArgumentsEnhanced(
            commandData.params, initialSkipCount, false, {}, { returnParamIndex }, false ) },
        { "className", className },
        { "classSeparator", classSeparator },
        { "commandName", commandName },
        { "noexcept", noexceptString },
        { "returnType", returnType },
        { "returnVariable", startLowerCase( stripPrefix( commandData.params[returnParamIndex].name, "p" ) ) },
        { "vectorSizeCheck", vectorSizeCheck },
        { "vkCommand", name } } );
  }
  else
  {
    std::string const functionTemplate =
      R"(    template <typename Dispatch = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
    VULKAN_HPP_NODISCARD ${returnType} ${commandName}( ${argumentList} ) const ${noexcept};)";

    return replaceWithMap( functionTemplate,
                           { { "argumentList", argumentList },
                             { "commandName", commandName },
                             { "noexcept", noexceptString },
                             { "returnType", returnType } } );
  }
}

std::string VulkanHppGenerator::constructConstexprString( std::pair<std::string, StructureData> const & structData,
                                                          bool assignmentOperator ) const
{
  // structs with a union (and VkBaseInStructure and VkBaseOutStructure) can't be a constexpr!
  bool isConstExpression = !containsUnion( structData.first ) && ( structData.first != "VkBaseInStructure" ) &&
                           ( structData.first != "VkBaseOutStructure" );
  return isConstExpression ? ( std::string( "VULKAN_HPP_CONSTEXPR" ) +
                               ( ( containsArray( structData.first ) || assignmentOperator ) ? "_14 " : " " ) )
                           : "";
}

std::string VulkanHppGenerator::constructFailureCheck( std::vector<std::string> const & successCodes ) const
{
  assert( !successCodes.empty() );
  std::string failureCheck = "result != VULKAN_HPP_NAMESPACE::Result::" + createSuccessCode( successCodes[0], m_tags );
  if ( 1 < successCodes.size() )
  {
    failureCheck = "( " + failureCheck + " )";
    for ( size_t i = 1; i < successCodes.size(); ++i )
    {
      failureCheck +=
        "&& ( result != VULKAN_HPP_NAMESPACE::Result::" + createSuccessCode( successCodes[i], m_tags ) + " )";
    }
  }
  return failureCheck;
}

std::string VulkanHppGenerator::constructFunctionBodyEnhanced( std::string const &              indentation,
                                                               std::string const &              name,
                                                               CommandData const &              commandData,
                                                               size_t                           initialSkipCount,
                                                               size_t                           returnParamIndex,
                                                               size_t                           templateParamIndex,
                                                               std::map<size_t, size_t> const & vectorParamIndices,
                                                               bool                             twoStep,
                                                               std::string const &              enhancedReturnType,
                                                               bool                             withAllocator ) const
{
  std::string str;
  if ( 1 < vectorParamIndices.size() )
  {
    appendFunctionBodyEnhancedMultiVectorSizeCheck(
      str, indentation, name, commandData, initialSkipCount, returnParamIndex, vectorParamIndices );
  }

  std::string returnName;
  if ( returnParamIndex != INVALID_INDEX )
  {
    returnName = appendFunctionBodyEnhancedLocalReturnVariable(
      str, indentation, commandData, returnParamIndex, enhancedReturnType, withAllocator );
  }

  if ( twoStep )
  {
    appendFunctionBodyEnhancedTwoStep(
      str, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, returnName );
  }
  else
  {
    str += constructFunctionBodyEnhancedSingleStep(
      indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices );
  }

  if ( ( commandData.returnType == "VkResult" ) || !commandData.successCodes.empty() )
  {
    appendFunctionBodyEnhancedReturnResultValue(
      str, indentation, returnName, name, commandData, initialSkipCount, returnParamIndex, twoStep );
  }
  return str;
}

std::string VulkanHppGenerator::constructFunctionBodyEnhancedSingleStep(
  std::string const &              indentation,
  std::string const &              name,
  CommandData const &              commandData,
  size_t                           returnParamIndex,
  size_t                           templateParamIndex,
  std::map<size_t, size_t> const & vectorParamIndices ) const
{
  std::string str;
  str += indentation + "  ";
  if ( commandData.returnType == "VkResult" )
  {
    str += "Result result = static_cast<Result>( ";
  }
  appendCall( str, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, false, true );
  if ( commandData.returnType == "VkResult" )
  {
    str += " )";
  }
  str += ";\n";
  return str;
}

std::string
  VulkanHppGenerator::constructFunctionHeaderArgumentsEnhanced( CommandData const &              commandData,
                                                                size_t                           returnParamIndex,
                                                                size_t                           templateParamIndex,
                                                                std::map<size_t, size_t> const & vectorParamIndices,
                                                                bool                             withDefaults,
                                                                bool                             withAllocator ) const
{
  std::string str;

  // check if there's at least one argument left to put in here
  std::set<size_t> skippedParams = ::determineSkippedParams( returnParamIndex, vectorParamIndices );
  if ( skippedParams.size() + ( commandData.handle.empty() ? 0 : 1 ) < commandData.params.size() )
  {
    str += " ";
    bool argEncountered = false;
    for ( size_t i = commandData.handle.empty() ? 0 : 1; i < commandData.params.size(); i++ )
    {
      argEncountered = appendFunctionHeaderArgumentEnhanced( str,
                                                             commandData.params[i],
                                                             i,
                                                             vectorParamIndices,
                                                             skippedParams.find( i ) != skippedParams.end(),
                                                             argEncountered,
                                                             ( templateParamIndex == i ) );
    }

    if ( argEncountered )
    {
      str += ", ";
    }
  }
  if ( withAllocator )
  {
    str += "Allocator const & vectorAllocator, ";
  }
  str += "Dispatch const &d";
  if ( withDefaults )
  {
    str += " VULKAN_HPP_DEFAULT_DISPATCHER_ASSIGNMENT";
  }
  str += " ";
  return str;
}

std::string VulkanHppGenerator::constructFunctionPointerCheck( std::string const & function,
                                                               std::string const & referencedIn ) const
{
  std::string functionPointerCheck;
  if ( m_extensions.find( referencedIn ) != m_extensions.end() )
  {
    std::string message  = "Function <" + function + "> needs extension <" + referencedIn + "> enabled!";
    functionPointerCheck = "\n      VULKAN_HPP_ASSERT( getDispatcher()->" + function + " && \"" + message + "\" );\n";
  }
  return functionPointerCheck;
}

std::string VulkanHppGenerator::constructNoDiscardStandard( CommandData const & commandData ) const
{
  return ( 1 < commandData.successCodes.size() + commandData.errorCodes.size() ) ? "VULKAN_HPP_NODISCARD " : "";
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleConstructor(
  std::pair<std::string, HandleData> const &                             handle,
  std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
  std::string const &                                                    enter,
  std::string const &                                                    leave ) const
{
  auto handleParamIt = std::find_if( constructorIt->second.params.begin(),
                                     constructorIt->second.params.end(),
                                     [&handle]( ParamData const & pd ) { return pd.type.type == handle.first; } );
  assert( handleParamIt != constructorIt->second.params.end() && handleParamIt->type.isNonConstPointer() );

  std::string singularConstructor, arrayConstructor, throwDetail;
  bool        constructedConstructor = false;
  if ( handleParamIt->len.empty() )
  {
    if ( constructorIt->second.returnType == "void" )
    {
      singularConstructor    = constructRAIIHandleConstructorVoid( handle, constructorIt, enter, leave );
      constructedConstructor = true;
    }
    else if ( ( constructorIt->second.returnType == "VkResult" ) && ( constructorIt->second.successCodes.size() == 1 ) )
    {
      if ( !constructorIt->second.errorCodes.empty() )
      {
        singularConstructor    = constructRAIIHandleConstructorResult( handle, constructorIt, enter, leave );
        constructedConstructor = true;
      }
      else
      {
        throwDetail = "\n\tReason: no errorcodes provided.";
      }
    }
  }
  else
  {
    auto lenParamIt =
      std::find_if( constructorIt->second.params.begin(),
                    constructorIt->second.params.end(),
                    [&handleParamIt]( ParamData const & pd ) { return pd.name == handleParamIt->len; } );
    if ( ( lenParamIt != constructorIt->second.params.end() ) && lenParamIt->type.isNonConstPointer() &&
         ( constructorIt->second.successCodes.size() == 2 ) &&
         ( constructorIt->second.successCodes[0] == "VK_SUCCESS" ) &&
         ( constructorIt->second.successCodes[1] == "VK_INCOMPLETE" ) )
    {
      arrayConstructor =
        constructRAIIHandleConstructorEnumerate( handle, constructorIt, handleParamIt, lenParamIt, enter, leave );
    }
    else
    {
      arrayConstructor = constructRAIIHandleConstructorVector( handle, constructorIt, handleParamIt, enter, leave );
      if ( ( lenParamIt != constructorIt->second.params.end() ) &&
           !checkEquivalentSingularConstructor( handle.second.constructorIts, constructorIt, lenParamIt ) )
      {
        singularConstructor =
          constructRAIIHandleConstructorVectorSingular( handle, constructorIt, handleParamIt, enter, leave );
      }
    }
    constructedConstructor = true;
  }
  if ( !constructedConstructor )
  {
    throw std::runtime_error( "Never encountered a constructor function like " + constructorIt->first + " !" +
                              throwDetail );
  }
  return std::make_pair( singularConstructor, arrayConstructor );
}

std::string VulkanHppGenerator::constructRAIIHandleConstructorArguments( std::string const &            handleType,
                                                                         std::vector<ParamData> const & params,
                                                                         bool                           singular,
                                                                         bool encounteredArgument ) const
{
  std::string arguments;
  for ( auto param : params )
  {
    if ( param.type.type != handleType )  // filter out the constructed type
    {
      // the specialPointerTypes are considered const-pointers!
      if ( param.type.isNonConstPointer() &&
           ( specialPointerTypes.find( param.type.type ) == specialPointerTypes.end() ) )
      {
        // this is supposed to be the returned size on an enumeration function!
        assert( param.type.type == "uint32_t" );
        auto typeIt = std::find_if(
          params.begin(), params.end(), [&handleType]( ParamData const & pd ) { return pd.type.type == handleType; } );
        assert( typeIt != params.end() );
        assert( typeIt->len == param.name );
        continue;
      }
      else if ( std::find_if( params.begin(),
                              params.end(),
                              [&param]( ParamData const & pd ) { return pd.len == param.name; } ) != params.end() )
      {
        // this is the len of an other parameter, which will be mapped to an ArrayProxy
        assert( param.type.isValue() && ( param.type.type == "uint32_t" ) );
        assert( param.arraySizes.empty() && param.len.empty() && !param.optional );
        continue;
      }
      if ( encounteredArgument )
      {
        arguments += ", ";
      }
      if ( param.type.isConstPointer() )
      {
        assert( beginsWith( param.type.type, "Vk" ) );
        assert( beginsWith( param.name, "p" ) );
        std::string argumentName = startLowerCase( stripPrefix( param.name, "p" ) );
        std::string argumentType = "VULKAN_HPP_NAMESPACE::" + stripPrefix( param.type.type, "Vk" );
        if ( param.optional )
        {
          assert( param.len.empty() );
          arguments += "VULKAN_HPP_NAMESPACE::Optional<const " + argumentType + "> " + argumentName + " = nullptr";
        }
        else if ( param.len.empty() )
        {
          arguments += argumentType + " const & " + argumentName;
        }
        else
        {
          assert( std::find_if( params.begin(),
                                params.end(),
                                [&param]( ParamData const & pd ) { return pd.name == param.len; } ) != params.end() );
          if ( singular )
          {
            arguments += argumentType + " const & " + stripPluralS( argumentName );
          }
          else
          {
            arguments += "VULKAN_HPP_NAMESPACE::ArrayProxy<" + argumentType + "> const & " + argumentName;
          }
        }
      }
      else if ( specialPointerTypes.find( param.type.type ) != specialPointerTypes.end() )
      {
        assert( !param.optional );
        assert( param.type.isNonConstPointer() );
        arguments += param.type.type + " & " + param.name;
      }
      else if ( ( param.type.isValue() ) && isHandleType( param.type.type ) )
      {
        std::string argument =
          "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::" + stripPrefix( param.type.type, "Vk" );
        if ( param.optional )
        {
          argument = "VULKAN_HPP_NAMESPACE::Optional<const " + argument + ">";
        }
        arguments += argument + " const & " + param.name;
      }
      else
      {
        assert( !param.optional );
        arguments += param.type.compose() + " " + param.name;
      }
      encounteredArgument = true;
    }
  }
  return arguments;
}

std::string
  VulkanHppGenerator::constructRAIIHandleConstructorCallArguments( std::string const &            handleType,
                                                                   std::vector<ParamData> const & params,
                                                                   bool                        nonConstPointerAsNullptr,
                                                                   std::set<size_t> const &    singularParams,
                                                                   std::vector<size_t> const & returnParamIndices,
                                                                   bool allocatorIsMemberVariable ) const
{
  std::string arguments;
  bool        encounteredArgument = false;
  for ( size_t i = 0; i < params.size(); ++i )
  {
    ParamData const & param = params[i];
    if ( encounteredArgument )
    {
      arguments += ", ";
    }
    if ( param.type.type == handleType )
    {
      assert( param.type.isNonConstPointer() && param.arraySizes.empty() );
      if ( param.len.empty() || !singularParams.empty() )
      {
        assert( !param.optional );
        assert( singularParams.empty() || ( param.len == params[*singularParams.begin()].name ) );
        arguments +=
          "reinterpret_cast<" + handleType + "*>( &m_" + startLowerCase( stripPrefix( handleType, "Vk" ) ) + " )";
      }
      else if ( nonConstPointerAsNullptr )
      {
        arguments += "nullptr";
      }
      else
      {
        arguments += startLowerCase( stripPrefix( param.name, "p" ) ) + ".data()";
      }
    }
    else if ( param.type.type == "VkAllocationCallbacks" )
    {
      assert( param.optional );
      if ( allocatorIsMemberVariable )
      {
        // VkAllocationCallbacks is stored as a member of the handle class !
        arguments += "m_allocator";
      }
      else
      {
        arguments +=
          "reinterpret_cast<const VkAllocationCallbacks *>(static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )";
      }
    }
    else if ( m_handles.find( param.type.type ) != m_handles.end() )
    {
      assert( param.type.isValue() && param.arraySizes.empty() && param.len.empty() );
      if ( param.optional )
      {
        arguments += param.name + " ? static_cast<" + param.type.type + ">( **" + param.name + " ) : 0";
      }
      else
      {
        arguments += "static_cast<" + param.type.type + " >( *" + param.name + " )";
      }
    }
    else
    {
      assert( !param.optional );
      arguments +=
        constructCallArgumentEnhanced( params, i, nonConstPointerAsNullptr, singularParams, returnParamIndices, true );
    }
    encounteredArgument = true;
  }
  return arguments;
}

std::string VulkanHppGenerator::constructRAIIHandleConstructorEnumerate(
  std::pair<std::string, HandleData> const &                             handle,
  std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
  std::vector<ParamData>::const_iterator                                 handleParamIt,
  std::vector<ParamData>::const_iterator                                 lenParamIt,
  std::string const &                                                    enter,
  std::string const &                                                    leave ) const
{
  std::string handleConstructorArguments = constructRAIIHandleSingularConstructorArguments( handle, constructorIt );
  std::string handleType                 = stripPrefix( handle.first, "Vk" );
  std::string dispatcherType             = hasParentHandle( handle.first, "VkDevice" )
                                             ? "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher"
                                             : "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher";

  const std::string constructorTemplate =
    R"(
${enter}    ${handleType}s( ${constructorArguments} )
    {
      ${dispatcherType} const * dispatcher = ${parentName}.getDispatcher();
      std::vector<${vectorElementType}> ${vectorName};
      ${counterType} ${counterName};
      VULKAN_HPP_NAMESPACE::Result result;
      do
      {
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( dispatcher->${constructorCall}( ${firstCallArguments} ) );
        if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && ${counterName} )
        {
          ${vectorName}.resize( ${counterName} );
          result = static_cast<VULKAN_HPP_NAMESPACE::Result>( dispatcher->${constructorCall}( ${secondCallArguments} ) );
          VULKAN_HPP_ASSERT( ${counterName} <= ${vectorName}.size() );
        }
      } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
      if ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess )
      {
        this->reserve( ${counterName} );
        for ( auto const & ${handleName} : ${vectorName} )
        {
          this->emplace_back( ${handleConstructorArguments}, dispatcher );
        }
      }
      else
      {
        throwResultException( result, "${constructorCall}" );
      }
    }
${leave})";

  return replaceWithMap(
    constructorTemplate,
    { { "constructorArguments",
        constructRAIIHandleConstructorArguments( handle.first, constructorIt->second.params, false, false ) },
      { "constructorCall", constructorIt->first },
      { "counterName", startLowerCase( stripPrefix( lenParamIt->name, "p" ) ) },
      { "counterType", lenParamIt->type.type },
      { "dispatcherType", dispatcherType },
      { "enter", enter },
      { "firstCallArguments",
        constructRAIIHandleConstructorCallArguments( handle.first, constructorIt->second.params, true, {}, {}, true ) },
      { "handleConstructorArguments", handleConstructorArguments },
      { "handleName", startLowerCase( handleType ) },
      { "handleType", handleType },
      { "leave", leave },
      { "parentName", constructorIt->second.params.front().name },
      { "secondCallArguments",
        constructRAIIHandleConstructorCallArguments(
          handle.first, constructorIt->second.params, false, {}, {}, true ) },
      { "vectorElementType", handleParamIt->type.type },
      { "vectorName", startLowerCase( stripPrefix( handleParamIt->name, "p" ) ) } } );
}

std::string VulkanHppGenerator::constructRAIIHandleConstructorInitializationList(
  std::string const &                                                    handleType,
  std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
  std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator destructorIt,
  bool                                                                   hasSecondLevelCommands ) const
{
  std::string initializationList;
  if ( destructorIt != m_commands.end() )
  {
    for ( auto destructorParam : destructorIt->second.params )
    {
      if ( destructorParam.type.type != handleType )
      {
        if ( isHandleType( destructorParam.type.type ) )
        {
          assert( destructorParam.type.isValue() && destructorParam.arraySizes.empty() && destructorParam.len.empty() &&
                  !destructorParam.optional );
          initializationList += "m_" + startLowerCase( stripPrefix( destructorParam.type.type, "Vk" ) ) + "( ";
          auto constructorParamIt = std::find_if( constructorIt->second.params.begin(),
                                                  constructorIt->second.params.end(),
                                                  [&destructorParam]( ParamData const & pd )
                                                  { return pd.type.type == destructorParam.type.type; } );
          if ( constructorParamIt != constructorIt->second.params.end() )
          {
            assert( constructorParamIt->type.isValue() && constructorParamIt->arraySizes.empty() &&
                    constructorParamIt->len.empty() && !constructorParamIt->optional );
            initializationList += "*" + constructorParamIt->name;
          }
          else
          {
#if !defined( NDEBUG )
            bool found = false;
#endif
            for ( auto constructorParam : constructorIt->second.params )
            {
              auto structureIt = m_structures.find( constructorParam.type.type );
              if ( structureIt != m_structures.end() )
              {
                auto structureMemberIt = std::find_if( structureIt->second.members.begin(),
                                                       structureIt->second.members.end(),
                                                       [&destructorParam]( MemberData const & md )
                                                       { return md.type.type == destructorParam.type.type; } );
                if ( structureMemberIt != structureIt->second.members.end() )
                {
                  assert( constructorParam.type.isConstPointer() && constructorParam.arraySizes.empty() &&
                          constructorParam.len.empty() && !constructorParam.optional );
                  initializationList +=
                    startLowerCase( stripPrefix( constructorParam.name, "p" ) ) + "." + structureMemberIt->name;
#if !defined( NDEBUG )
                  found = true;
#endif
                  break;
                }
              }
            }
            assert( found );
          }
          initializationList += " ), ";
        }
        else if ( destructorParam.type.type == "VkAllocationCallbacks" )
        {
          assert( destructorParam.type.isConstPointer() && destructorParam.arraySizes.empty() &&
                  destructorParam.len.empty() && destructorParam.optional );
          initializationList +=
            "m_allocator( reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) ) ), ";
        }
        else
        {
          // we can ignore all other parameters here !
        }
      }
    }
  }
  else if ( hasSecondLevelCommands )
  {
    auto const & param = constructorIt->second.params.front();
    initializationList = "m_" + startLowerCase( stripPrefix( param.type.type, "Vk" ) ) + "( *" + param.name + " ), ";
  }
  return initializationList;
}

std::string VulkanHppGenerator::constructRAIIHandleConstructorResult(
  std::pair<std::string, HandleData> const &                             handle,
  std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
  std::string const &                                                    enter,
  std::string const &                                                    leave ) const
{
  std::string callArguments = constructRAIIHandleConstructorCallArguments(
    handle.first, constructorIt->second.params, false, {}, {}, handle.second.destructorIt != m_commands.end() );
  std::string constructorArguments, dispatcherArgument, dispatcherInit, getDispatcher;
  if ( handle.first == "VkInstance" )
  {
    constructorArguments = "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Context const & context";
    dispatcherArgument   = "context.getDispatcher()->vkGetInstanceProcAddr";
    getDispatcher        = "context.getDispatcher()";
  }
  else
  {
    dispatcherArgument = constructorIt->second.params[0].name + ".getDispatcher()";
    if ( handle.first == "VkDevice" )
    {
      dispatcherArgument = dispatcherArgument + "->vkGetDeviceProcAddr";
      getDispatcher      = "physicalDevice.getDispatcher()";
    }
    else
    {
      getDispatcher = "getDispatcher()";
    }
  }
  constructorArguments += constructRAIIHandleConstructorArguments(
    handle.first, constructorIt->second.params, false, handle.first == "VkInstance" );
  if ( ( handle.first == "VkDevice" ) || ( handle.first == "VkInstance" ) )
  {
    dispatcherInit = "\n    m_dispatcher.init( static_cast<" + handle.first + ">( m_" +
                     startLowerCase( stripPrefix( handle.first, "Vk" ) ) + " ) );";
  }
  std::string initializationList = constructRAIIHandleConstructorInitializationList(
    handle.first, constructorIt, handle.second.destructorIt, !handle.second.secondLevelCommands.empty() );

  const std::string constructorTemplate =
    R"(
${enter}    ${handleType}( ${constructorArguments} )
      : ${initializationList}m_dispatcher( ${dispatcherArgument} )
    {
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( ${getDispatcher}->${constructorCall}( ${callArguments} ) );
      if ( ${failureCheck} )
      {
        throwResultException( result, "${constructorCall}" );
      }${dispatcherInit}
    }
${leave})";

  return replaceWithMap( constructorTemplate,
                         { { "callArguments", callArguments },
                           { "constructorArguments", constructorArguments },
                           { "constructorCall", constructorIt->first },
                           { "dispatcherArgument", dispatcherArgument },
                           { "dispatcherInit", dispatcherInit },
                           { "enter", enter },
                           { "failureCheck", constructFailureCheck( constructorIt->second.successCodes ) },
                           { "getDispatcher", getDispatcher },
                           { "leave", leave },
                           { "handleType", stripPrefix( handle.first, "Vk" ) },
                           { "initializationList", initializationList } } );
}

std::string VulkanHppGenerator::constructRAIIHandleConstructorTakeOwnership(
  std::pair<std::string, HandleData> const & handle ) const
{
  std::string handleType = stripPrefix( handle.first, "Vk" );
  std::string paramType;
  std::string constructorArguments, dispatcherArgument, initializationList;
  if ( handle.first == "VkInstance" )
  {
    constructorArguments = "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::Context const & context";
    dispatcherArgument   = "context.getDispatcher()->vkGetInstanceProcAddr";
  }
  else
  {
    assert( !handle.second.constructorIts.empty() && !handle.second.constructorIts.front()->second.params.empty() );
    auto param = handle.second.constructorIts.front()->second.params.begin();
    assert( isHandleType( param->type.type ) && param->type.isValue() );
    paramType = stripPrefix( param->type.type, "Vk" );
    constructorArguments =
      "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::" + paramType + " const & " + startLowerCase( paramType );
    dispatcherArgument = startLowerCase( paramType ) + ".getDispatcher()";
    if ( handle.first == "VkDevice" )
    {
      dispatcherArgument = dispatcherArgument + "->vkGetDeviceProcAddr";
    }
  }
  std::string handleName = startLowerCase( handleType );
  constructorArguments += ", " + handle.first + " " + handleName;
  initializationList += "m_" + handleName + "( " + handleName + " ), ";
  if ( handle.second.destructorIt != m_commands.end() )
  {
    std::vector<ParamData> params  = handle.second.destructorIt->second.params;
    auto                   paramIt = std::find_if(
      params.begin(), params.end(), [&paramType]( ParamData const & pd ) { return pd.type.type == "Vk" + paramType; } );
    if ( paramIt != params.end() )
    {
      params.erase( paramIt );
    }
    constructorArguments += constructRAIIHandleConstructorArguments( handle.first, params, false, true );
    initializationList +=
      constructRAIIHandleConstructorInitializationList( handle.first,
                                                        handle.second.destructorIt,
                                                        handle.second.destructorIt,
                                                        !handle.second.secondLevelCommands.empty() );
  }
  else if ( !handle.second.secondLevelCommands.empty() )
  {
    initializationList += "m_" + startLowerCase( paramType ) + "( *" + startLowerCase( paramType ) + " ), ";
  }

  std::string dispatcherInit;
  if ( ( handle.first == "VkDevice" ) || ( handle.first == "VkInstance" ) )
  {
    dispatcherInit = "\n      m_dispatcher.init( static_cast<" + handle.first + ">( m_" +
                     startLowerCase( stripPrefix( handle.first, "Vk" ) ) + " ) );\n    ";
  }

  const std::string constructorTemplate =
    R"(
    ${handleType}( ${constructorArguments} )
      : ${initializationList}m_dispatcher( ${dispatcherArgument} )
    {${dispatcherInit}}
)";

  return replaceWithMap( constructorTemplate,
                         { { "constructorArguments", constructorArguments },
                           { "dispatcherArgument", dispatcherArgument },
                           { "dispatcherInit", dispatcherInit },
                           { "handleType", handleType },
                           { "initializationList", initializationList } } );
}

std::pair<std::string, std::string>
  VulkanHppGenerator::constructRAIIHandleConstructors( std::pair<std::string, HandleData> const & handle ) const
{
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( handle.first, !handle.second.alias.empty() );

  std::string singularConstructors, arrayConstructors;
  for ( auto constructorIt : handle.second.constructorIts )
  {
    // there is a non-const parameter with handle type : the to-be-constructed handle

    // check for additional enter/leave guards for the constructors
    std::string constructorEnter, constructorLeave;
    std::tie( constructorEnter, constructorLeave ) = generateProtection( constructorIt->second.referencedIn );
    if ( constructorEnter == enter )
    {
      constructorEnter.clear();
      constructorLeave.clear();
    }

    std::string arrayConstructor, singularConstructor;
    std::tie( singularConstructor, arrayConstructor ) =
      constructRAIIHandleConstructor( handle, constructorIt, constructorEnter, constructorLeave );
    arrayConstructors += arrayConstructor;
    singularConstructors += singularConstructor;
  }
  singularConstructors += constructRAIIHandleConstructorTakeOwnership( handle );
  return std::make_pair( singularConstructors, arrayConstructors );
}

std::string VulkanHppGenerator::constructRAIIHandleConstructorVector(
  std::pair<std::string, HandleData> const &                             handle,
  std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
  std::vector<ParamData>::const_iterator                                 handleParamIt,
  std::string const &                                                    enter,
  std::string const &                                                    leave ) const
{
  std::string vectorSize;
  auto        lenIt = std::find_if( constructorIt->second.params.begin(),
                             constructorIt->second.params.end(),
                             [&handleParamIt]( ParamData const & pd ) { return pd.name == handleParamIt->len; } );
  if ( lenIt == constructorIt->second.params.end() )
  {
    std::vector<std::string> lenParts = tokenize( handleParamIt->len, "->" );
    assert( lenParts.size() == 2 );
    lenIt = std::find_if( constructorIt->second.params.begin(),
                          constructorIt->second.params.end(),
                          [&lenParts]( ParamData const & pd ) { return pd.name == lenParts[0]; } );
    assert( lenIt != constructorIt->second.params.end() );
    auto structureIt = m_structures.find( lenIt->type.type );
    assert( structureIt != m_structures.end() );
    assert( std::find_if( structureIt->second.members.begin(),
                          structureIt->second.members.end(),
                          [&lenParts]( MemberData const & md )
                          { return md.name == lenParts[1]; } ) != structureIt->second.members.end() );
    assert( constructorIt->second.successCodes.size() == 1 );
    vectorSize = startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + lenParts[1];
  }
  else
  {
    auto arrayIt = std::find_if( constructorIt->second.params.begin(),
                                 constructorIt->second.params.end(),
                                 [&lenIt, &handleParamIt]( ParamData const & pd )
                                 { return ( pd.len == lenIt->name ) && ( pd.name != handleParamIt->name ); } );
    assert( arrayIt != constructorIt->second.params.end() );
    vectorSize = startLowerCase( stripPrefix( arrayIt->name, "p" ) ) + ".size()";
  }

  std::string handleConstructorArguments = constructRAIIHandleSingularConstructorArguments( handle, constructorIt );
  std::string handleType                 = stripPrefix( handle.first, "Vk" );
  std::string successCodePassToElement   = ( 1 < constructorIt->second.successCodes.size() ) ? "result," : "";

  const std::string constructorTemplate =
    R"(
${enter}    ${handleType}s( ${constructorArguments} )
    {
      VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * dispatcher = ${parentName}.getDispatcher();
      std::vector<${vectorElementType}> ${vectorName}( ${vectorSize} );
      VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( dispatcher->${constructorCall}( ${callArguments} ) );
      if ( ${successCheck} )
      {
        this->reserve( ${vectorSize} );
        for ( auto const & ${handleName} : ${vectorName} )
        {
          this->emplace_back( ${handleConstructorArguments}, ${successCodePassToElement} dispatcher );
        }
      }
      else
      {
        throwResultException( result, "${constructorCall}" );
      }
    }
${leave})";

  return replaceWithMap(
    constructorTemplate,
    { { "callArguments",
        constructRAIIHandleConstructorCallArguments(
          handle.first, constructorIt->second.params, false, {}, {}, false ) },
      { "constructorArguments",
        constructRAIIHandleConstructorArguments( handle.first, constructorIt->second.params, false, false ) },
      { "constructorCall", constructorIt->first },
      { "enter", enter },
      { "handleConstructorArguments", handleConstructorArguments },
      { "handleName", startLowerCase( handleType ) },
      { "handleType", handleType },
      { "leave", leave },
      { "parentName", constructorIt->second.params.front().name },
      { "successCheck", constructSuccessCheck( constructorIt->second.successCodes ) },
      { "successCodePassToElement", successCodePassToElement },
      { "vectorElementType", handleParamIt->type.type },
      { "vectorName", startLowerCase( stripPrefix( handleParamIt->name, "p" ) ) },
      { "vectorSize", vectorSize } } );
}

std::string VulkanHppGenerator::constructRAIIHandleConstructorVectorSingular(
  std::pair<std::string, HandleData> const &                             handle,
  std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
  std::vector<ParamData>::const_iterator                                 handleParamIt,
  std::string const &                                                    enter,
  std::string const &                                                    leave ) const
{
  size_t returnParamIndex = static_cast<size_t>( std::distance( constructorIt->second.params.begin(), handleParamIt ) );
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndicesNew( constructorIt->second.params );
  std::set<size_t>         singularParams     = determineSingularParams( returnParamIndex, vectorParamIndices );

  std::string callArguments = constructRAIIHandleConstructorCallArguments(
    handle.first, constructorIt->second.params, false, singularParams, { returnParamIndex }, true );
  std::string initializationList = constructRAIIHandleConstructorInitializationList(
    handle.first, constructorIt, handle.second.destructorIt, !handle.second.secondLevelCommands.empty() );
  std::string failureCheck = constructFailureCheck( constructorIt->second.successCodes );
  replaceAll( failureCheck, "result", "m_constructorSuccessCode" );

  const std::string singularConstructorTemplate =
    R"(
${enter}    ${handleType}( ${constructorArguments} )
      : ${initializationList}m_dispatcher( ${firstArgument}.getDispatcher() )
    {
      m_constructorSuccessCode = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${constructorCall}( ${callArguments} ) );
      if ( ${failureCheck} )
      {
        throwResultException( m_constructorSuccessCode, "${constructorCall}" );
      }
    }
${leave})";

  return replaceWithMap(
    singularConstructorTemplate,
    { { "initializationList", initializationList },
      { "callArguments", callArguments },
      { "constructorArguments",
        constructRAIIHandleConstructorArguments( handle.first, constructorIt->second.params, true, false ) },
      { "constructorCall", constructorIt->first },
      { "enter", enter },
      { "firstArgument", constructorIt->second.params[0].name },
      { "failureCheck", failureCheck },
      { "leave", leave },
      { "handleType", stripPrefix( handle.first, "Vk" ) } } );
}

std::string VulkanHppGenerator::constructRAIIHandleConstructorVoid(
  std::pair<std::string, HandleData> const &                             handle,
  std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
  std::string const &                                                    enter,
  std::string const &                                                    leave ) const
{
  std::string callArguments =
    constructRAIIHandleConstructorCallArguments( handle.first, constructorIt->second.params, false, {}, {}, true );
  std::string constructorArguments =
    constructRAIIHandleConstructorArguments( handle.first, constructorIt->second.params, false, false );
  std::string initializationList = constructRAIIHandleConstructorInitializationList(
    handle.first, constructorIt, handle.second.destructorIt, !handle.second.secondLevelCommands.empty() );

  const std::string constructorTemplate =
    R"(
${enter}    ${handleType}( ${constructorArguments} )
      : ${initializationList}m_dispatcher( ${firstArgument}.getDispatcher() )
    {
      getDispatcher()->${constructorCall}( ${callArguments} );
    }
${leave})";

  return replaceWithMap( constructorTemplate,
                         { { "callArguments", callArguments },
                           { "constructorArguments", constructorArguments },
                           { "constructorCall", constructorIt->first },
                           { "enter", enter },
                           { "firstArgument", constructorIt->second.params[0].name },
                           { "leave", leave },
                           { "handleType", stripPrefix( handle.first, "Vk" ) },
                           { "initializationList", initializationList } } );
}

std::pair<std::string, std::string>
  VulkanHppGenerator::constructRAIIHandleDestructor( std::string const &                                handleType,
                                                     std::map<std::string, CommandData>::const_iterator destructorIt,
                                                     std::string const &                                enter ) const
{
  std::string destructorEnter, destructorLeave;
  std::tie( destructorEnter, destructorLeave ) = generateProtection( destructorIt->second.referencedIn );
  bool doProtect                               = !destructorEnter.empty() && ( destructorEnter != enter );
  if ( !doProtect )
  {
    destructorEnter.clear();
    destructorLeave.clear();
  }
  std::string destructorCall = destructorIt->first + "( " +
                               constructRAIIHandleDestructorCallArguments( handleType, destructorIt->second.params ) +
                               " )";

  const std::string destructorTemplate = R"(
${enter}~${handleType}()
    {
      if ( m_${handleName} )
      {
        getDispatcher()->${destructorCall};
      }
    }
${leave})";

  std::string destructor = replaceWithMap( destructorTemplate,
                                           { { "destructorCall", destructorCall },
                                             { "enter", destructorEnter },
                                             { "handleName", startLowerCase( stripPrefix( handleType, "Vk" ) ) },
                                             { "handleType", stripPrefix( handleType, "Vk" ) },
                                             { "leave", destructorLeave } } );
  return std::make_pair( destructor, destructorCall );
}

std::string
  VulkanHppGenerator::constructRAIIHandleDestructorCallArguments( std::string const &            handleType,
                                                                  std::vector<ParamData> const & params ) const
{
  std::string arguments;
  bool        encounteredArgument = false;
  for ( auto param : params )
  {
    if ( encounteredArgument )
    {
      arguments += ", ";
    }
    if ( param.type.type == "VkAllocationCallbacks" )
    {
      // VkAllocationCallbacks is stored as a member of the handle class
      arguments += "m_allocator";
    }
    else if ( isHandleType( param.type.type ) )
    {
      assert( param.arraySizes.empty() );
      std::string argument = "m_" + startLowerCase( stripPrefix( param.type.type, "Vk" ) );
      if ( param.type.isValue() )
      {
        if ( param.type.type == handleType )
        {
          argument = "static_cast<" + param.type.type + ">( " + argument + " )";
        }
        arguments += argument;
      }
      else
      {
        assert( param.type.isConstPointer() );
        assert( !param.len.empty() &&
                ( std::find_if( params.begin(),
                                params.end(),
                                [&param]( ParamData const & pd ) { return pd.name == param.len; } ) != params.end() ) );
        arguments += "reinterpret_cast<" + param.type.type + " const *>( &" + argument + " )";
      }
    }
    else
    {
      assert( ( param.type.type == "uint32_t" ) && param.type.isValue() && param.arraySizes.empty() &&
              param.len.empty() && !param.optional );
      assert( std::find_if( params.begin(),
                            params.end(),
                            [&param]( ParamData const & pd ) { return pd.len == param.name; } ) != params.end() );
      arguments += "1";
    }
    encounteredArgument = true;
  }
  return arguments;
}

std::tuple<std::string, std::string, std::string, std::string>
  VulkanHppGenerator::constructRAIIHandleDetails( std::pair<std::string, HandleData> const & handle,
                                                  std::string const &                        destructorCall ) const
{
  std::string getConstructorSuccessCode, memberVariables, moveConstructorInitializerList, moveAssignmentInstructions;
  bool        multiSuccessCodeContructor = isMultiSuccessCodeConstructor( handle.second.constructorIts );
  if ( multiSuccessCodeContructor )
  {
    getConstructorSuccessCode = R"(
    VULKAN_HPP_NAMESPACE::Result getConstructorSuccessCode() const
    {
      return m_constructorSuccessCode;
    }
)";
  }

  std::string handleType = stripPrefix( handle.first, "Vk" );
  std::string handleName = startLowerCase( handleType );

  memberVariables = "    VULKAN_HPP_NAMESPACE::" + handleType + " m_" + handleName + ";";
  moveConstructorInitializerList =
    "m_" + handleName + "( VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_" + handleName + ", {} ) )";
  moveAssignmentInstructions = "        m_" + handleName +
                               " = VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::exchange( rhs.m_" + handleName +
                               ", {} );";
  if ( handle.second.destructorIt != m_commands.end() )
  {
    moveAssignmentInstructions = "        getDispatcher()->" + destructorCall + ";\n" + moveAssignmentInstructions;
    for ( auto const & destructorParam : handle.second.destructorIt->second.params )
    {
      if ( ( destructorParam.type.type != handle.first ) &&
           ( std::find_if( handle.second.destructorIt->second.params.begin(),
                           handle.second.destructorIt->second.params.end(),
                           [&destructorParam]( ParamData const & pd ) { return pd.len == destructorParam.name; } ) ==
             handle.second.destructorIt->second.params.end() ) )
      {
        std::string name = destructorParam.name;
        if ( !destructorParam.type.isValue() )
        {
          name = startLowerCase( stripPrefix( name, "p" ) );
        }
        memberVariables += "\n    " + destructorParam.type.prefix + " " + destructorParam.type.type + " " +
                           destructorParam.type.postfix + " m_" + name + ";";
        moveConstructorInitializerList += ", m_" + name + "( rhs.m_" + name + " )";
        moveAssignmentInstructions += "\n        m_" + name + " = rhs.m_" + name + ";";
      }
    }
  }
  else if ( !handle.second.secondLevelCommands.empty() )
  {
    assert( !handle.second.constructorIts.empty() );
    assert( !handle.second.constructorIts.front()->second.params.empty() );
    auto const & parentType = handle.second.constructorIts.front()->second.params.front().type;
    assert( isHandleType( parentType.type ) );
    memberVariables += "\n    VULKAN_HPP_NAMESPACE::" + stripPrefix( parentType.type, "Vk" ) + " m_" +
                       startLowerCase( stripPrefix( parentType.type, "Vk" ) ) + ";";
  }
  if ( multiSuccessCodeContructor )
  {
    memberVariables += "\n    VULKAN_HPP_NAMESPACE::Result m_constructorSuccessCode;";
  }

  if ( handle.first == "VkInstance" )
  {
    memberVariables += "\n    VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher m_dispatcher;";
  }
  else if ( handle.first == "VkDevice" )
  {
    memberVariables += "\n    VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher m_dispatcher;";
  }
  else if ( handle.second.constructorIts.front()->second.params.front().type.type == "VkDevice" )
  {
    memberVariables += "\n    VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher const * m_dispatcher;";
  }
  else
  {
    memberVariables +=
      "\n    VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher const * m_dispatcher;";
  }

  moveConstructorInitializerList += ", m_dispatcher( rhs.m_dispatcher )";
  moveAssignmentInstructions += "\n        m_dispatcher = rhs.m_dispatcher;";
  return std::make_tuple(
    getConstructorSuccessCode, memberVariables, moveConstructorInitializerList, moveAssignmentInstructions );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultEnumerate(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string commandName = determineCommandName(
    commandIt->first, initialSkipCount ? commandIt->second.params[initialSkipCount - 1].type.type : "", m_tags );
  std::string counterName =
    startLowerCase( stripPrefix( commandIt->second.params[vectorParamIndices.begin()->second].name, "p" ) );
  std::string enter, leave;
  std::tie( enter, leave )       = generateProtection( commandIt->second.referencedIn );
  std::string firstCallArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, true, {}, nonConstPointerParamIndices, true );
  std::string secondCallArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true );
  std::string vectorElementType = commandIt->second.params[vectorParamIndices.begin()->first].type.type;
  if ( !isHandleType( vectorElementType ) )
  {
    assert( commandIt->second.params[vectorParamIndices.begin()->first].type.isNonConstPointer() );
    vectorElementType =
      ( vectorElementType == "void" )
        ? "uint8_t"
        : stripPostfix( commandIt->second.params[vectorParamIndices.begin()->first].type.compose(), "*" );
  }
  std::string vectorName =
    startLowerCase( stripPrefix( commandIt->second.params[vectorParamIndices.begin()->first].name, "p" ) );

  std::string const declarationTemplate =
    R"(
${enter}    VULKAN_HPP_NODISCARD std::vector<${vectorElementType}> ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            {
                                              { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave },
                                              { "vectorElementType", vectorElementType },
                                            } );

  const std::string definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<${vectorElementType}> ${className}::${commandName}( ${argumentList} ) const
  {${functionPointerCheck}
    std::vector<${vectorElementType}> ${vectorName};
    ${counterType} ${counterName};
    VULKAN_HPP_NAMESPACE::Result result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${firstCallArguments} ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && ${counterName} )
      {
        ${vectorName}.resize( ${counterName} );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${secondCallArguments} ) );
        VULKAN_HPP_ASSERT( ${counterName} <= ${vectorName}.size() );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && ( ${counterName} < ${vectorName}.size() ) )
    {
      ${vectorName}.resize( ${counterName} );
    }
    if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return ${vectorName};
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "className",
        initialSkipCount ? stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) : "Context" },
      { "commandName", commandName },
      { "counterName", counterName },
      { "counterType", commandIt->second.params[vectorParamIndices.begin()->second].type.type },
      { "enter", enter },
      { "firstCallArguments", firstCallArguments },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "secondCallArguments", secondCallArguments },
      { "vectorElementType", vectorElementType },
      { "vectorName", vectorName },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultEnumerateTwoVectors(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  auto        firstVectorParamIt  = vectorParamIndices.begin();
  auto        secondVectorParamIt = std::next( firstVectorParamIt );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string counterName =
    startLowerCase( stripPrefix( stripPluralS( commandIt->second.params[firstVectorParamIt->second].name ), "p" ) );
  std::string enter, leave;
  std::tie( enter, leave )       = generateProtection( commandIt->second.referencedIn );
  std::string firstCallArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, true, {}, nonConstPointerParamIndices, true );
  std::string firstType = stripPrefix( commandIt->second.params[firstVectorParamIt->first].type.type, "Vk" );
  std::string firstVectorName =
    startLowerCase( stripPrefix( commandIt->second.params[firstVectorParamIt->first].name, "p" ) );
  std::string secondCallArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true );
  std::string secondType = stripPrefix( commandIt->second.params[secondVectorParamIt->first].type.type, "Vk" );
  std::string secondVectorName =
    startLowerCase( stripPrefix( commandIt->second.params[secondVectorParamIt->first].name, "p" ) );

  std::string const declarationTemplate =
    R"(
${enter}    VULKAN_HPP_NODISCARD std::pair<std::vector<${firstType}>, std::vector<${secondType}>> ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "firstType", firstType },
                                              { "leave", leave },
                                              { "secondType", secondType } } );

  const std::string definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::pair<std::vector<${firstType}>, std::vector<${secondType}>> ${className}::${commandName}( ${argumentList} ) const
  {${functionPointerCheck}
    std::pair<std::vector<${firstType}>, std::vector<${secondType}>> data;
    std::vector<${firstType}> & ${firstVectorName} = data.first;
    std::vector<${secondType}> & ${secondVectorName} = data.second;
    ${counterType} ${counterName};
    VULKAN_HPP_NAMESPACE::Result result;
    do
    {
      result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${firstCallArguments} ) );
      if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && counterCount )
      {
        ${firstVectorName}.resize( ${counterName} );
        ${secondVectorName}.resize( ${counterName} );
        result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${secondCallArguments} ) );
        VULKAN_HPP_ASSERT( ${counterName} <= ${firstVectorName}.size() );
      }
    } while ( result == VULKAN_HPP_NAMESPACE::Result::eIncomplete );
    if ( ( result == VULKAN_HPP_NAMESPACE::Result::eSuccess ) && ( ${counterName} < ${firstVectorName}.size() ) )
    {
      ${firstVectorName}.resize( ${counterName} );
      ${secondVectorName}.resize( ${counterName} );
    }
    if ( result != VULKAN_HPP_NAMESPACE::Result::eSuccess )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return data;
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "counterName", counterName },
      { "counterType", commandIt->second.params[firstVectorParamIt->second].type.type },
      { "enter", enter },
      { "firstCallArguments", firstCallArguments },
      { "firstType", firstType },
      { "firstVectorName", firstVectorName },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "secondCallArguments", secondCallArguments },
      { "secondType", secondType },
      { "secondVectorName", secondVectorName },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultGetTwoValues(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  assert( nonConstPointerParamIndices.size() == 2 );
  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string commandName = determineCommandName(
    commandIt->first, initialSkipCount ? commandIt->second.params[initialSkipCount - 1].type.type : "", m_tags );
  std::string enter, leave;
  std::tie( enter, leave )  = generateProtection( commandIt->second.referencedIn );
  std::string callArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true );
  std::string firstReturnName =
    startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[0]].name, "p" ) );
  std::string firstReturnType =
    stripPostfix( commandIt->second.params[nonConstPointerParamIndices[0]].type.compose(), "*" );
  std::string secondReturnName =
    startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[1]].name, "p" ) );
  std::string secondReturnType =
    stripPostfix( commandIt->second.params[nonConstPointerParamIndices[1]].type.compose(), "*" );

  std::string const declarationTemplate =
    R"(
${enter}    VULKAN_HPP_NODISCARD std::tuple<VULKAN_HPP_NAMESPACE::Result,${firstReturnType},${secondReturnType}> ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "firstReturnType", firstReturnType },
                                              { "leave", leave },
                                              { "secondReturnType", secondReturnType } } );

  const std::string definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::tuple<VULKAN_HPP_NAMESPACE::Result,${firstReturnType},${secondReturnType}> ${className}::${commandName}( ${argumentList} ) const
  {${functionPointerCheck}
    std::tuple<VULKAN_HPP_NAMESPACE::Result,${firstReturnType},${secondReturnType}> data;
    VULKAN_HPP_NAMESPACE::Result & result = std::get<0>( data );
    ${firstReturnType} & ${firstReturnName} = std::get<1>( data );
    ${secondReturnType} & ${secondReturnName} = std::get<2>( data );
    result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    if ( ${failureCheck} )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return data;
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className",
        initialSkipCount ? stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) : "Context" },
      { "commandName", commandName },
      { "enter", enter },
      { "failureCheck", constructFailureCheck( commandIt->second.successCodes ) },
      { "firstReturnName", firstReturnName },
      { "firstReturnType", firstReturnType },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "secondReturnName", secondReturnName },
      { "secondReturnType", secondReturnType },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultMulti(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices ) const
{
  std::set<size_t> skippedParameters =
    determineSkippedParams( commandIt->second.params, initialSkipCount, vectorParamIndices, {}, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments =
    constructCallArgumentsEnhanced( commandIt->second.params, initialSkipCount, false, {}, {}, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );

  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck = needsVectorSizeCheck( vectorParamIndices );
  std::string                                            vectorSizeCheckString =
    vectorSizeCheck.first
                                                 ? constructRAIIHandleVectorSizeCheck(
          commandIt->first, commandIt->second, initialSkipCount, vectorSizeCheck.second, skippedParameters )
                                                 : "";

  std::string const declarationTemplate =
    R"(
${enter}    VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::Result ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave } } );

  std::string const definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result ${className}::${commandName}( ${argumentList} ) const
  {${functionPointerCheck}${vectorSizeCheck}
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    if ( ${failureCheck} )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return result;
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "failureCheck", constructFailureCheck( commandIt->second.successCodes ) },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "vectorSizeCheck", vectorSizeCheckString },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultMultiGetValue(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );
  std::string returnType = stripPostfix( commandIt->second.params[nonConstPointerParamIndices[0]].type.compose(), "*" );
  std::string valueName =
    startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[0]].name, "p" ) );

  std::string const declarationTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD std::pair<VULKAN_HPP_NAMESPACE::Result, ${returnType}> ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            {
                                              { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave },
                                              { "returnType", returnType },
                                            } );

  std::string const definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::pair<VULKAN_HPP_NAMESPACE::Result, ${returnType}> ${className}::${commandName}( ${argumentList} ) const
  {${functionPointerCheck}
    ${returnType} ${valueName};
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    if ( ${failureCheck} )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return std::make_pair( result, ${valueName} );
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "failureCheck", constructFailureCheck( commandIt->second.successCodes ) },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "valueName", valueName },
      { "returnType", returnType },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultMultiGetVectorOfVoid(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string dataName =
    startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[0]].name, "p" ) );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );

  std::string const declarationTemplate =
    R"(
${enter}    template <typename T>
    VULKAN_HPP_NODISCARD std::pair<VULKAN_HPP_NAMESPACE::Result, std::vector<T>> ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave } } );

  std::string const definitionTemplate =
    R"(
${enter}  template <typename T>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::pair<VULKAN_HPP_NAMESPACE::Result, std::vector<T>> ${className}::${commandName}( ${argumentList} ) const
  {${functionPointerCheck}
    VULKAN_HPP_ASSERT( ${dataSize} % sizeof( T ) == 0 );
    std::vector<T> ${dataName}( ${dataSize} / sizeof( T ) );
    Result result = static_cast<Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    if ( ${failureCheck} )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return std::make_pair( result, ${dataName} );
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "dataName", dataName },
      { "dataSize", commandIt->second.params[nonConstPointerParamIndices[0]].len },
      { "enter", enter },
      { "failureCheck", constructFailureCheck( commandIt->second.successCodes ) },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string>
  VulkanHppGenerator::constructRAIIHandleMemberFunctionResultMultiGetVectorOfVoidSingular(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  assert( nonConstPointerParamIndices.size() == 1 );
  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, true );
  std::set<size_t> singularParams = determineSingularParams( nonConstPointerParamIndices[0], vectorParamIndices );
  std::string      argumentListDeclaration = constructArgumentListEnhanced(
    commandIt->second.params, skippedParameters, singularParams, false, false, false, false );
  std::string argumentListDefinition = constructArgumentListEnhanced(
    commandIt->second.params, skippedParameters, singularParams, true, false, false, false );
  std::string callArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, singularParams, nonConstPointerParamIndices, true );
  std::string commandName = stripPluralS(
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags ) );
  std::string dataName =
    startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[0]].name, "p" ) );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );

  std::string const singularDeclarationTemplate =
    R"(
${enter}    template <typename T>
    VULKAN_HPP_NODISCARD std::pair<VULKAN_HPP_NAMESPACE::Result, T> ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( singularDeclarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave } } );

  std::string const singularDefinitionTemplate =
    R"(
${enter}  template <typename T>
  VULKAN_HPP_NODISCARD std::pair<VULKAN_HPP_NAMESPACE::Result, T> ${className}::${commandName}( ${argumentList} ) const
  {
    T ${dataName};
    Result result = static_cast<Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    if ( ${failureCheck} )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return std::make_pair( result, ${dataName} );
  }
${leave})";

  std::string definition =
    replaceWithMap( singularDefinitionTemplate,
                    { { "argumentList", argumentListDefinition },
                      { "callArguments", callArguments },
                      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
                      { "commandName", commandName },
                      { "dataName", dataName },
                      { "enter", enter },
                      { "failureCheck", constructFailureCheck( commandIt->second.successCodes ) },
                      { "leave", leave },
                      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultMultiNoErrors(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices ) const
{
  std::set<size_t> skippedParameters =
    determineSkippedParams( commandIt->second.params, initialSkipCount, vectorParamIndices, {}, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments =
    constructCallArgumentsEnhanced( commandIt->second.params, initialSkipCount, false, {}, {}, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );

  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck = needsVectorSizeCheck( vectorParamIndices );
  std::string                                            vectorSizeCheckString =
    vectorSizeCheck.first
                                                 ? constructRAIIHandleVectorSizeCheck(
          commandIt->first, commandIt->second, initialSkipCount, vectorSizeCheck.second, skippedParameters )
                                                 : "";

  std::string const declarationTemplate =
    R"(
${enter}    VULKAN_HPP_NODISCARD VULKAN_HPP_NAMESPACE::Result ${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave } } );

  std::string const definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE VULKAN_HPP_NAMESPACE::Result ${className}::${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT
  {${functionPointerCheck}${vectorSizeCheck}
    return static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "vectorSizeCheck", vectorSizeCheckString },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultSingle(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices ) const
{
  std::set<size_t> skippedParameters =
    determineSkippedParams( commandIt->second.params, initialSkipCount, vectorParamIndices, {}, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments =
    constructCallArgumentsEnhanced( commandIt->second.params, initialSkipCount, false, {}, {}, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );

  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck = needsVectorSizeCheck( vectorParamIndices );
  std::string                                            vectorSizeCheckString =
    vectorSizeCheck.first
                                                 ? constructRAIIHandleVectorSizeCheck(
          commandIt->first, commandIt->second, initialSkipCount, vectorSizeCheck.second, skippedParameters )
                                                 : "";

  std::string const declarationTemplate =
    R"(
${enter}    void ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave } } );

  std::string const definitionTemplate =
    R"(
${enter}  VULKAN_HPP_INLINE void ${className}::${commandName}( ${argumentList} ) const
  {${functionPointerCheck}${vectorSizeCheck}
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    if ( ${failureCheck} )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "failureCheck", constructFailureCheck( commandIt->second.successCodes ) },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "vectorSizeCheck", vectorSizeCheckString },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultSingleGetChain(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );
  std::string returnType = stripPostfix( commandIt->second.params[nonConstPointerParamIndices[0]].type.compose(), "*" );
  std::string returnVariable =
    startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[0]].name, "p" ) );

  std::string const declarationTemplate =
    R"(
${enter}  template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD StructureChain<X, Y, Z...> ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave } } );

  std::string const functionTemplate =
    R"(
${enter}  template <typename X, typename Y, typename... Z>
  VULKAN_HPP_NODISCARD StructureChain<X, Y, Z...> ${className}::${commandName}( ${argumentList} ) const
  {
    StructureChain<X, Y, Z...> structureChain;
    ${returnType} & ${returnVariable} = structureChain.template get<${returnType}>();
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    if ( ${failureCheck} )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return structureChain;
  }
${leave})";

  std::string definition =
    replaceWithMap( functionTemplate,
                    { { "argumentList", argumentListDefinition },
                      { "callArguments", callArguments },
                      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
                      { "commandName", commandName },
                      { "enter", enter },
                      { "failureCheck", constructFailureCheck( commandIt->second.successCodes ) },
                      { "leave", leave },
                      { "returnVariable", returnVariable },
                      { "returnType", returnType },
                      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultSingleGetVector(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  size_t                                             returnParamIndex ) const
{
  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, { returnParamIndex }, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments =
    constructCallArgumentsEnhanced( commandIt->second.params, initialSkipCount, false, {}, { returnParamIndex }, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave )      = generateProtection( commandIt->second.referencedIn );
  std::string vectorElementType = commandIt->second.params[returnParamIndex].type.type;
  std::string vectorName        = startLowerCase( stripPrefix( commandIt->second.params[returnParamIndex].name, "p" ) );
  std::string vectorSize =
    startLowerCase( stripPrefix( commandIt->second.params[vectorParamIndices.begin()->first].name, "p" ) ) + ".size()";

  std::string const declarationTemplate =
    R"(
${enter}    VULKAN_HPP_NODISCARD std::vector<${vectorElementType}> ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            {
                                              { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave },
                                              { "vectorElementType", vectorElementType },
                                            } );

  std::string const definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<${vectorElementType}> ${className}::${commandName}( ${argumentList} ) const
  {${functionPointerCheck}
    std::vector<${vectorElementType}> ${vectorName}( ${vectorSize} );
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    if ( ${failureCheck} )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return ${vectorName};
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "failureCheck", constructFailureCheck( commandIt->second.successCodes ) },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "vectorElementType", vectorElementType },
      { "vectorName", vectorName },
      { "vectorSize", vectorSize },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultSingleGetVectorAndValue(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );
  std::string valueName =
    startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[1]].name, "p" ) );
  std::string valueType         = commandIt->second.params[nonConstPointerParamIndices[1]].type.type;
  std::string vectorElementType = commandIt->second.params[nonConstPointerParamIndices[0]].type.type;
  std::string vectorName =
    startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[0]].name, "p" ) );
  std::string vectorSize =
    startLowerCase( stripPrefix( commandIt->second.params[vectorParamIndices.begin()->first].name, "p" ) ) + ".size()";

  std::string const declarationTemplate =
    R"(
${enter}    VULKAN_HPP_NODISCARD std::pair<std::vector<${vectorElementType}>, ${valueType}> ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            {
                                              { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave },
                                              { "valueType", valueType },
                                              { "vectorElementType", vectorElementType },
                                            } );

  std::string const definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::pair<std::vector<${vectorElementType}>, ${valueType}> ${className}::${commandName}( ${argumentList} ) const
  {${functionPointerCheck}
    std::pair<std::vector<${vectorElementType}>, ${valueType}> data( std::piecewise_construct, std::forward_as_tuple( ${vectorSize} ), std::forward_as_tuple( 0 ) );
    std::vector<${vectorElementType}> & ${vectorName} = data.first;
    ${valueType} & ${valueName} = data.second;
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    if ( ${failureCheck} )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return data;
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "failureCheck", constructFailureCheck( commandIt->second.successCodes ) },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "valueName", valueName },
      { "valueType", valueType },
      { "vectorElementType", vectorElementType },
      { "vectorName", vectorName },
      { "vectorSize", vectorSize },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultSingleGetVectorOfVoid(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  assert( nonConstPointerParamIndices.size() == 1 );
  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string dataName =
    startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[0]].name, "p" ) );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );

  std::string const declarationTemplate =
    R"(
${enter}    template <typename T>
    VULKAN_HPP_NODISCARD std::vector<T> ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave } } );

  std::string const definitionTemplate =
    R"(
${enter}  template <typename T>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<T> ${className}::${commandName}( ${argumentList} ) const
  {${functionPointerCheck}
    VULKAN_HPP_ASSERT( ${dataSize} % sizeof( T ) == 0 );
    std::vector<T> ${dataName}( ${dataSize} / sizeof( T ) );
    Result result = static_cast<Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    if ( ${failureCheck} )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return ${dataName};
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "dataName", dataName },
      { "dataSize", commandIt->second.params[nonConstPointerParamIndices[0]].len },
      { "enter", enter },
      { "failureCheck", constructFailureCheck( commandIt->second.successCodes ) },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string>
  VulkanHppGenerator::constructRAIIHandleMemberFunctionResultSingleGetVectorOfVoidSingular(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  assert( nonConstPointerParamIndices.size() == 1 );

  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, true );
  std::set<size_t> singularParams = determineSingularParams( nonConstPointerParamIndices[0], vectorParamIndices );
  std::string      argumentListDeclaration = constructArgumentListEnhanced(
    commandIt->second.params, skippedParameters, singularParams, false, false, false, false );
  std::string argumentListDefinition = constructArgumentListEnhanced(
    commandIt->second.params, skippedParameters, singularParams, true, false, false, false );
  std::string callArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, singularParams, nonConstPointerParamIndices, true );
  std::string commandName = stripPluralS(
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags ) );
  std::string dataName =
    startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[0]].name, "p" ) );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );

  std::string const singularDeclarationTemplate =
    R"(
${enter}    template <typename T>
    VULKAN_HPP_NODISCARD T ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( singularDeclarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave } } );

  std::string const singularDefinitionTemplate =
    R"(
${enter}  template <typename T>
  VULKAN_HPP_NODISCARD T ${className}::${commandName}( ${argumentList} ) const
  {
    T ${dataName};
    Result result = static_cast<Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    if ( ${failureCheck} )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return ${dataName};
  }
${leave})";

  std::string definition =
    replaceWithMap( singularDefinitionTemplate,
                    { { "argumentList", argumentListDefinition },
                      { "callArguments", callArguments },
                      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
                      { "commandName", commandName },
                      { "dataName", dataName },
                      { "enter", enter },
                      { "failureCheck", constructFailureCheck( commandIt->second.successCodes ) },
                      { "leave", leave },
                      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultSingleGetVectorSingular(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  size_t                                             returnParamIndex ) const
{
  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, { returnParamIndex }, true );
  std::set<size_t> singularParams          = determineSingularParams( returnParamIndex, vectorParamIndices );
  std::string      argumentListDeclaration = constructArgumentListEnhanced(
    commandIt->second.params, skippedParameters, singularParams, false, false, false, false );
  std::string argumentListDefinition = constructArgumentListEnhanced(
    commandIt->second.params, skippedParameters, singularParams, true, false, false, false );
  std::string callArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, singularParams, { returnParamIndex }, true );
  std::string commandName = stripPluralS(
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags ) );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );
  std::string vectorElementName =
    stripPluralS( startLowerCase( stripPrefix( commandIt->second.params[returnParamIndex].name, "p" ) ) );
  std::string vectorElementType = commandIt->second.params[returnParamIndex].type.type;

  std::string const declarationTemplate =
    R"(
${enter}    VULKAN_HPP_NODISCARD ${vectorElementType} ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            {
                                              { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave },
                                              { "vectorElementType", vectorElementType },
                                            } );

  std::string const definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ${vectorElementType} ${className}::${commandName}( ${argumentList} ) const
  {${functionPointerCheck}
    ${vectorElementType} ${vectorElementName};
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    if ( ${failureCheck} )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return ${vectorElementName};
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "failureCheck", constructFailureCheck( commandIt->second.successCodes ) },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "vectorElementName", vectorElementName },
      { "vectorElementType", vectorElementType },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultSingleNoErrors(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices ) const
{
  std::set<size_t> skippedParameters =
    determineSkippedParams( commandIt->second.params, initialSkipCount, vectorParamIndices, {}, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments =
    constructCallArgumentsEnhanced( commandIt->second.params, initialSkipCount, false, {}, {}, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );

  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck = needsVectorSizeCheck( vectorParamIndices );
  std::string                                            vectorSizeCheckString =
    vectorSizeCheck.first
                                                 ? constructRAIIHandleVectorSizeCheck(
          commandIt->first, commandIt->second, initialSkipCount, vectorSizeCheck.second, skippedParameters )
                                                 : "";

  std::string const declarationTemplate =
    R"(
${enter}    void ${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave } } );

  std::string const definitionTemplate =
    R"(
${enter}  VULKAN_HPP_INLINE void ${className}::${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT
  {${functionPointerCheck}${vectorSizeCheck}
    getDispatcher()->${vkCommand}( ${callArguments} );
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "vectorSizeCheck", vectorSizeCheckString },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResultSingleGetValue(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  assert( nonConstPointerParamIndices.size() == 1 );

  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true );
  std::string commandName = determineCommandName(
    commandIt->first, initialSkipCount ? commandIt->second.params[initialSkipCount - 1].type.type : "", m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );
  std::string returnType = stripPostfix( commandIt->second.params[nonConstPointerParamIndices[0]].type.compose(), "*" );
  std::string valueName =
    startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[0]].name, "p" ) );

  std::string const declarationTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD ${returnType} ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            {
                                              { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave },
                                              { "returnType", returnType },
                                            } );

  std::string const definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ${returnType} ${className}::${commandName}( ${argumentList} ) const
  {${functionPointerCheck}
    ${returnType} ${valueName};
    VULKAN_HPP_NAMESPACE::Result result = static_cast<VULKAN_HPP_NAMESPACE::Result>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    if ( ${failureCheck} )
    {
      throwResultException( result, VULKAN_HPP_NAMESPACE_STRING"::${className}::${commandName}" );
    }
    return ${valueName};
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className",
        initialSkipCount ? stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) : "Context" },
      { "commandName", commandName },
      { "enter", enter },
      { "failureCheck", constructFailureCheck( commandIt->second.successCodes ) },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "valueName", valueName },
      { "returnType", returnType },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

void VulkanHppGenerator::constructRAIIHandleMemberFunction( std::string &                 functionDeclarations,
                                                            std::string &                 functionDefinitions,
                                                            std::string const &           command,
                                                            size_t                        initialSkipCount,
                                                            std::set<std::string> const & specialFunctions ) const
{
  if ( specialFunctions.find( command ) == specialFunctions.end() )
  {
    bool commandConstructed = false;
    auto commandIt          = m_commands.find( command );
    assert( commandIt != m_commands.end() );

    if ( commandIt->second.returnType == "VkResult" )
    {
      commandConstructed = constructRAIIHandleMemberFunctionResult(
        functionDeclarations, functionDefinitions, commandIt, initialSkipCount );
    }
    else
    {
      assert( commandIt->second.successCodes.empty() && commandIt->second.errorCodes.empty() );
      if ( commandIt->second.returnType == "void" )
      {
        commandConstructed = constructRAIIHandleMemberFunctionVoid(
          functionDeclarations, functionDefinitions, commandIt, initialSkipCount );
      }
      else
      {
        commandConstructed = constructRAIIHandleMemberFunctionValue(
          functionDeclarations, functionDefinitions, commandIt, initialSkipCount );
      }
    }

    if ( !commandConstructed )
    {
      throw std::runtime_error( "Never encountered a function like <" + commandIt->first + "> !" );
    }
  }
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionBoolGetValue(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  assert( nonConstPointerParamIndices.size() == 1 );

  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );
  std::string returnType = stripPostfix( commandIt->second.params[nonConstPointerParamIndices[0]].type.compose(), "*" );
  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck = needsVectorSizeCheck( vectorParamIndices );
  std::string noexceptString = vectorSizeCheck.first ? "VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS" : "VULKAN_HPP_NOEXCEPT";

  std::string const declarationTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD std::pair<Bool32,${returnType}> ${commandName}( ${argumentList} ) const ${noexcept};
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            {
                                              { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave },
                                              { "noexcept", noexceptString },
                                              { "returnType", returnType },
                                            } );

  std::string const definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::pair<Bool32,${returnType}> ${className}::${commandName}( ${argumentList} ) const ${noexcept}
  {${functionPointerCheck}${vectorSizeCheck}
    std::pair<Bool32,${returnType}> result;
    ${returnType} & ${returnVariable} = result.second;
    result.first = static_cast<Bool32>( getDispatcher()->${vkCommand}( ${callArguments} ) );
    return result;
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments",
        constructCallArgumentsEnhanced(
          commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true ) },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "noexcept", noexceptString },
      { "vectorSizeCheck",
        vectorSizeCheck.first
          ? constructVectorSizeCheck(
              commandIt->first, commandIt->second, initialSkipCount, vectorSizeCheck.second, skippedParameters )
          : "" },
      { "returnType", returnType },
      { "returnVariable",
        startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[0]].name, "p" ) ) },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

bool VulkanHppGenerator::constructRAIIHandleMemberFunctionResult(
  std::string &                                      functionDeclarations,
  std::string &                                      functionDefinitions,
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount ) const
{
  // as the returnType is "VkResult", there has to be at least one success code
  assert( !commandIt->second.successCodes.empty() );

  bool                commandConstructed = false;
  std::vector<size_t> returnParamIndices = determineNonConstPointerParamIndices( commandIt->second.params );
  std::string         declaration, definition;
  switch ( returnParamIndices.size() )
  {
    case 0:
      std::tie( commandConstructed, declaration, definition ) =
        constructRAIIHandleMemberFunctionResult0Return( commandIt, initialSkipCount );
      break;
    case 1:
      std::tie( commandConstructed, declaration, definition ) =
        constructRAIIHandleMemberFunctionResult1Return( commandIt, initialSkipCount, returnParamIndices[0] );
      break;
    case 2:
      std::tie( commandConstructed, declaration, definition ) =
        constructRAIIHandleMemberFunctionResult2Return( commandIt, initialSkipCount, returnParamIndices );
      break;
    case 3:
      std::tie( commandConstructed, declaration, definition ) =
        constructRAIIHandleMemberFunctionResult3Return( commandIt, initialSkipCount, returnParamIndices );
      break;
  }
  functionDeclarations += declaration;
  functionDefinitions += definition;
  return commandConstructed;
}

std::tuple<bool, std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResult0Return(
  std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount ) const
{
  std::string              declaration, definition;
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndicesNew( commandIt->second.params );
  if ( commandIt->second.errorCodes.empty() )
  {
    if ( commandIt->second.successCodes.size() == 1 )
    {
      std::tie( declaration, definition ) =
        constructRAIIHandleMemberFunctionResultSingleNoErrors( commandIt, initialSkipCount, vectorParamIndices );
    }
    else
    {
      std::tie( declaration, definition ) =
        constructRAIIHandleMemberFunctionResultMultiNoErrors( commandIt, initialSkipCount, vectorParamIndices );
    }
  }
  else
  {
    if ( commandIt->second.successCodes.size() == 1 )
    {
      std::tie( declaration, definition ) =
        constructRAIIHandleMemberFunctionResultSingle( commandIt, initialSkipCount, vectorParamIndices );
    }
    else
    {
      std::tie( declaration, definition ) =
        constructRAIIHandleMemberFunctionResultMulti( commandIt, initialSkipCount, vectorParamIndices );
    }
  }
  return std::make_tuple( true, declaration, definition );
}

std::tuple<bool, std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResult1Return(
  std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount, size_t returnParamIndex ) const
{
  // one return parameter (and it's not a handle, that would be a constructor)
  assert( !isHandleType( commandIt->second.params[returnParamIndex].type.type ) );

  bool                     commandConstructed = false;
  std::string              declaration, definition;
  std::map<size_t, size_t> vectorParamIndices  = determineVectorParamIndicesNew( commandIt->second.params );
  auto                     returnVectorParamIt = vectorParamIndices.find( returnParamIndex );
  if ( returnVectorParamIt == vectorParamIndices.end() )
  {
    // the return parameter is not a vector -> return a value
    // as the returnType is "VkResult", there has to be at least one success code
    assert( !commandIt->second.successCodes.empty() );
    if ( !commandIt->second.errorCodes.empty() )
    {
      if ( commandIt->second.successCodes.size() == 1 )
      {
        std::tie( declaration, definition ) = constructRAIIHandleMemberFunctionResultSingleGetValue(
          commandIt, initialSkipCount, vectorParamIndices, { returnParamIndex } );
      }
      else
      {
        std::tie( declaration, definition ) = constructRAIIHandleMemberFunctionResultMultiGetValue(
          commandIt, initialSkipCount, vectorParamIndices, { returnParamIndex } );
      }
      commandConstructed = true;
    }
  }
  else
  {
    // the return parameter is a vector -> return a vector
    // as the returnType is "VkResult", there has to be at least one success code
    assert( !commandIt->second.successCodes.empty() );
    if ( !commandIt->second.errorCodes.empty() &&
         ( commandIt->second.params[returnVectorParamIt->second].type.isValue() ) )
    {
      // there are error codes, and the size is give by value
      if ( commandIt->second.params[returnVectorParamIt->first].type.type == "void" )
      {
        // the return parameter is of type void
        if ( commandIt->second.successCodes.size() == 1 )
        {
          std::tie( declaration, definition ) = constructRAIIHandleMemberFunctionResultSingleGetVectorOfVoid(
            commandIt, initialSkipCount, vectorParamIndices, { returnParamIndex } );

          std::string singularDeclaration, singularDefinition;
          std::tie( singularDeclaration, singularDefinition ) =
            constructRAIIHandleMemberFunctionResultSingleGetVectorOfVoidSingular(
              commandIt, initialSkipCount, vectorParamIndices, { returnParamIndex } );
          declaration += singularDeclaration;
          definition += singularDefinition;
        }
        else
        {
          std::tie( declaration, definition ) = constructRAIIHandleMemberFunctionResultMultiGetVectorOfVoid(
            commandIt, initialSkipCount, vectorParamIndices, { returnParamIndex } );

          std::string singularDeclaration, singularDefinition;
          std::tie( singularDeclaration, singularDefinition ) =
            constructRAIIHandleMemberFunctionResultMultiGetVectorOfVoidSingular(
              commandIt, initialSkipCount, vectorParamIndices, { returnParamIndex } );
          declaration += singularDeclaration;
          definition += singularDefinition;
        }
        commandConstructed = true;
      }
      else
      {
        // the return parameter is of a concrete type
        if ( commandIt->second.successCodes.size() == 1 )
        {
          std::tie( declaration, definition ) = constructRAIIHandleMemberFunctionResultSingleGetVector(
            commandIt, initialSkipCount, vectorParamIndices, returnParamIndex );

          std::string singularDeclaration, singularDefinition;
          std::tie( singularDeclaration, singularDefinition ) =
            constructRAIIHandleMemberFunctionResultSingleGetVectorSingular(
              commandIt, initialSkipCount, vectorParamIndices, returnParamIndex );
          declaration += singularDeclaration;
          definition += singularDefinition;
          commandConstructed = true;
        }
      }
    }
  }
  if ( isStructureChainAnchor( commandIt->second.params[returnParamIndex].type.type ) )
  {
    // for StructureChain returns, add functions returning a StructureChain
    commandConstructed = false;
    if ( returnVectorParamIt == vectorParamIndices.end() )
    {
      // as the returnType is "VkResult", there has to be at least one success code
      assert( !commandIt->second.successCodes.empty() );
      if ( !commandIt->second.errorCodes.empty() )
      {
        if ( commandIt->second.successCodes.size() == 1 )
        {
          std::string chainDeclaration, chainDefinition;
          std::tie( chainDeclaration, chainDefinition ) = constructRAIIHandleMemberFunctionResultSingleGetChain(
            commandIt, initialSkipCount, vectorParamIndices, { returnParamIndex } );
          declaration += chainDeclaration;
          definition += chainDefinition;
          commandConstructed = true;
        }
      }
    }
  }

  return std::make_tuple( commandConstructed, declaration, definition );
}

std::tuple<bool, std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResult2Return(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::vector<size_t> const &                        returnParamIndices ) const
{
  // two return parameters (and the first one is not a handle, that would be a constructor)
  assert( returnParamIndices.size() == 2 );
  assert( !isHandleType( commandIt->second.params[returnParamIndices[0]].type.type ) );

  bool                     commandConstructed = false;
  std::string              declaration, definition;
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndicesNew( commandIt->second.params );
  switch ( vectorParamIndices.size() )
  {
    case 1:
      {
        // two returns but just one vector
        auto vectorParamIndexIt = vectorParamIndices.begin();
        if ( ( vectorParamIndexIt->second == returnParamIndices[0] ) &&
             ( vectorParamIndexIt->first == returnParamIndices[1] ) )
        {
          // the size is a return value as well -> enumerate the values
          // as the returnType is "VkResult", there has to be at least one success code
          assert( !commandIt->second.successCodes.empty() );
          if ( !commandIt->second.errorCodes.empty() )
          {
            if ( ( commandIt->second.successCodes.size() == 2 ) &&
                 ( commandIt->second.successCodes[0] == "VK_SUCCESS" ) &&
                 ( commandIt->second.successCodes[1] == "VK_INCOMPLETE" ) )
            {
              std::tie( declaration, definition ) = constructRAIIHandleMemberFunctionResultEnumerate(
                commandIt, initialSkipCount, vectorParamIndices, returnParamIndices );
              commandConstructed = true;
            }
          }
        }
      }
      break;
    case 2:
      // two returns and two vectors
      if ( ( vectorParamIndices.find( returnParamIndices[0] ) != vectorParamIndices.end() ) &&
           ( vectorParamIndices.find( returnParamIndices[1] ) == vectorParamIndices.end() ) )
      {
        // two returns and two vectors! But one input vector, one output vector of the same size, and one output
        // value
        // as the returnType is "VkResult", there has to be at least one success code
        assert( !commandIt->second.successCodes.empty() );
        if ( !commandIt->second.errorCodes.empty() )
        {
          if ( commandIt->second.successCodes.size() == 1 )
          {
            std::tie( declaration, definition ) = constructRAIIHandleMemberFunctionResultSingleGetVectorAndValue(
              commandIt, initialSkipCount, vectorParamIndices, returnParamIndices );
            commandConstructed = true;
          }
        }
      }
      break;
  }
  if ( isStructureChainAnchor( commandIt->second.params[returnParamIndices[0]].type.type ) )
  {
    commandConstructed = false;
  }
  return std::make_tuple( commandConstructed, declaration, definition );
}

std::tuple<bool, std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionResult3Return(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::vector<size_t> const &                        returnParamIndices ) const
{
  // three return parameters
  assert( returnParamIndices.size() == 3 );

  bool                     commandConstructed = false;
  std::string              declaration, definition;
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndicesNew( commandIt->second.params );
  if ( vectorParamIndices.size() == 2 )
  {
    // two vector parameters
    auto firstVectorParamIt  = vectorParamIndices.begin();
    auto secondVectorParamIt = std::next( firstVectorParamIt );
    if ( ( firstVectorParamIt->second == returnParamIndices[0] ) &&
         ( firstVectorParamIt->first == returnParamIndices[1] ) &&
         ( secondVectorParamIt->first == returnParamIndices[2] ) &&
         ( firstVectorParamIt->second == secondVectorParamIt->second ) )
    {
      // the two vectors use the very same size parameter
      // both vectors, as well as the size parameter are non-const pointer that is output parameters
      assert( commandIt->second.params[firstVectorParamIt->first].type.isNonConstPointer() &&
              commandIt->second.params[secondVectorParamIt->first].type.isNonConstPointer() &&
              commandIt->second.params[firstVectorParamIt->second].type.isNonConstPointer() );
      // the size is a return value as well -> enumerate the values
      // as the returnType is "VkResult", there has to be at least one success code
      assert( !commandIt->second.successCodes.empty() );
      if ( !commandIt->second.errorCodes.empty() )
      {
        if ( ( commandIt->second.successCodes.size() == 2 ) && ( commandIt->second.successCodes[0] == "VK_SUCCESS" ) &&
             ( commandIt->second.successCodes[1] == "VK_INCOMPLETE" ) )
        {
          std::tie( declaration, definition ) = constructRAIIHandleMemberFunctionResultEnumerateTwoVectors(
            commandIt, initialSkipCount, vectorParamIndices, returnParamIndices );
          commandConstructed = true;
        }
      }
    }
  }
  if ( isStructureChainAnchor( commandIt->second.params[returnParamIndices[0]].type.type ) ||
       isStructureChainAnchor( commandIt->second.params[returnParamIndices[1]].type.type ) ||
       isStructureChainAnchor( commandIt->second.params[returnParamIndices[2]].type.type ) )
  {
    commandConstructed = true;
  }
  return std::make_tuple( commandConstructed, declaration, definition );
}

std::pair<std::string, std::string>
  VulkanHppGenerator::constructRAIIHandleMemberFunctions( std::pair<std::string, HandleData> const & handle,
                                                          std::set<std::string> const & specialFunctions ) const
{
  std::string functionDeclarations, functionDefinitions;
  for ( auto const & command : handle.second.commands )
  {
    constructRAIIHandleMemberFunction(
      functionDeclarations, functionDefinitions, command, handle.first.empty() ? 0 : 1, specialFunctions );
  }
  for ( auto const & command : handle.second.secondLevelCommands )
  {
    assert( !handle.first.empty() );
    constructRAIIHandleMemberFunction( functionDeclarations, functionDefinitions, command, 2, specialFunctions );
  }
  return std::make_pair( functionDeclarations, functionDefinitions );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionType(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices ) const
{
  std::set<size_t> skippedParameters =
    determineSkippedParams( commandIt->second.params, initialSkipCount, vectorParamIndices, {}, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments =
    constructCallArgumentsEnhanced( commandIt->second.params, initialSkipCount, false, {}, {}, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );
  std::string                                            returnType = namespacedType( commandIt->second.returnType );
  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck = needsVectorSizeCheck( vectorParamIndices );
  std::string                                            vectorSizeCheckString =
    vectorSizeCheck.first
                                                 ? constructVectorSizeCheck(
          commandIt->first, commandIt->second, initialSkipCount, vectorSizeCheck.second, skippedParameters )
                                                 : "";

  std::string const declarationTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD ${returnType} ${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            {
                                              { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave },
                                              { "returnType", returnType },
                                            } );

  std::string const definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ${returnType} ${className}::${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT
  {${functionPointerCheck}${vectorSizeCheck}
    return getDispatcher()->${vkCommand}( ${callArguments} );
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "returnType", returnType },
      { "vectorSizeCheck", vectorSizeCheckString },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionVkType(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices ) const
{
  std::set<size_t> skippedParameters =
    determineSkippedParams( commandIt->second.params, initialSkipCount, vectorParamIndices, {}, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments =
    constructCallArgumentsEnhanced( commandIt->second.params, initialSkipCount, false, {}, {}, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );
  std::string returnType   = namespacedType( commandIt->second.returnType );

  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck = needsVectorSizeCheck( vectorParamIndices );
  std::string                                            vectorSizeCheckString =
    vectorSizeCheck.first
                                                 ? constructRAIIHandleVectorSizeCheck(
          commandIt->first, commandIt->second, initialSkipCount, vectorSizeCheck.second, skippedParameters )
                                                 : "";

  std::string const declarationTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD ${returnType} ${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            {
                                              { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave },
                                              { "returnType", returnType },
                                            } );

  std::string const definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ${returnType} ${className}::${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT
  {${functionPointerCheck}${vectorSizeCheck}
    return static_cast<${returnType}>( getDispatcher()->${vkCommand}( ${callArguments} ) );
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "returnType", returnType },
      { "vectorSizeCheck", vectorSizeCheckString },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

bool VulkanHppGenerator::constructRAIIHandleMemberFunctionValue(
  std::string &                                      functionDeclarations,
  std::string &                                      functionDefinitions,
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount ) const
{
  // as the return type is not "VkResult", there are no success or error codes allowed
  assert( commandIt->second.successCodes.empty() && commandIt->second.errorCodes.empty() );

  bool                commandConstructed = false;
  std::string         declaration, definition;
  std::vector<size_t> returnParamIndices = determineNonConstPointerParamIndices( commandIt->second.params );
  if ( returnParamIndices.empty() )
  {
    std::tie( commandConstructed, declaration, definition ) =
      constructRAIIHandleMemberFunctionValue0Return( commandIt, initialSkipCount );
  }

  functionDeclarations += declaration;
  functionDefinitions += definition;
  return commandConstructed;
}

std::tuple<bool, std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionValue0Return(
  std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount ) const
{
  std::string              declaration, definition;
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndicesNew( commandIt->second.params );
  if ( beginsWith( commandIt->second.returnType, "Vk" ) )
  {
    std::tie( declaration, definition ) =
      constructRAIIHandleMemberFunctionVkType( commandIt, initialSkipCount, vectorParamIndices );
  }
  else
  {
    std::tie( declaration, definition ) =
      constructRAIIHandleMemberFunctionType( commandIt, initialSkipCount, vectorParamIndices );
  }
  return std::make_tuple( true, declaration, definition );
}

bool VulkanHppGenerator::constructRAIIHandleMemberFunctionVoid(
  std::string &                                      functionDeclarations,
  std::string &                                      functionDefinitions,
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount ) const
{
  // as the return type is not "VkResult", there are no success or error codes allowed
  assert( commandIt->second.successCodes.empty() && commandIt->second.errorCodes.empty() );

  bool                commandConstructed = false;
  std::string         declaration, definition;
  std::vector<size_t> returnParamIndices = determineNonConstPointerParamIndices( commandIt->second.params );
  switch ( returnParamIndices.size() )
  {
    case 0:
      std::tie( commandConstructed, declaration, definition ) =
        constructRAIIHandleMemberFunctionVoid0Return( commandIt, initialSkipCount );
      break;
    case 1:
      std::tie( commandConstructed, declaration, definition ) =
        constructRAIIHandleMemberFunctionVoid1Return( commandIt, initialSkipCount, returnParamIndices[0] );
      break;
    case 2:
      std::tie( commandConstructed, declaration, definition ) =
        constructRAIIHandleMemberFunctionVoid2Return( commandIt, initialSkipCount, returnParamIndices );
      break;
  }

  functionDeclarations += declaration;
  functionDefinitions += definition;
  return commandConstructed;
}

std::tuple<bool, std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionVoid0Return(
  std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount ) const
{
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndicesNew( commandIt->second.params );

  std::set<size_t> skippedParameters =
    determineSkippedParams( commandIt->second.params, initialSkipCount, vectorParamIndices, {}, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments =
    constructCallArgumentsEnhanced( commandIt->second.params, initialSkipCount, false, {}, {}, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );
  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck = needsVectorSizeCheck( vectorParamIndices );
  std::string noexceptString = vectorSizeCheck.first ? "VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS" : "VULKAN_HPP_NOEXCEPT";
  std::string vectorSizeCheckString =
    vectorSizeCheck.first
      ? constructVectorSizeCheck(
          commandIt->first, commandIt->second, initialSkipCount, vectorSizeCheck.second, skippedParameters )
      : "";

  std::string templateString = ( ( vectorParamIndices.size() == 1 ) &&
                                 ( commandIt->second.params[vectorParamIndices.begin()->first].type.type == "void" ) )
                                 ? "template <typename T>\n"
                                 : "";

  std::string const declarationTemplate =
    R"(
${enter}    ${template}void ${commandName}( ${argumentList} ) const ${noexcept};
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave },
                                              { "noexcept", noexceptString },
                                              { "template", templateString } } );

  std::string const definitionTemplate =
    R"(
${enter}  ${template}VULKAN_HPP_INLINE void ${className}::${commandName}( ${argumentList} ) const ${noexcept}
  {${functionPointerCheck}${vectorSizeCheck}
    getDispatcher()->${vkCommand}( ${callArguments} );
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "noexcept", noexceptString },
      { "template", templateString },
      { "vectorSizeCheck", vectorSizeCheckString },
      { "vkCommand", commandIt->first } } );

  return std::make_tuple( true, declaration, definition );
}

std::tuple<bool, std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionVoid1Return(
  std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount, size_t returnParamIndex ) const
{
  // one return parameter (and it's not a handle, that would be a constructor)
  assert( !isHandleType( commandIt->second.params[returnParamIndex].type.type ) );

  bool                     commandConstructed = false;
  std::string              declaration, definition;
  std::map<size_t, size_t> vectorParamIndices  = determineVectorParamIndicesNew( commandIt->second.params );
  auto                     returnVectorParamIt = vectorParamIndices.find( returnParamIndex );
  if ( returnVectorParamIt == vectorParamIndices.end() )
  {
    // the return parameter is not a vector -> return a value
    // as the return type is not "VkResult", there are no success or error codes allowed
    assert( commandIt->second.successCodes.empty() && commandIt->second.errorCodes.empty() );
    std::tie( declaration, definition ) = constructRAIIHandleMemberFunctionVoidGetValue(
      commandIt, initialSkipCount, vectorParamIndices, { returnParamIndex } );
    commandConstructed = true;
  }
  if ( isStructureChainAnchor( commandIt->second.params[returnParamIndex].type.type ) )
  {
    // for StructureChain returns, add functions returning a StructureChain
    commandConstructed = false;
    if ( returnVectorParamIt == vectorParamIndices.end() )
    {
      // as the return type is not "VkResult", there are no success or error codes allowed
      assert( commandIt->second.successCodes.empty() && commandIt->second.errorCodes.empty() );
      std::string chainDeclaration, chainDefinition;
      std::tie( chainDeclaration, chainDefinition ) = constructRAIIHandleMemberFunctionVoidGetChain(
        commandIt, initialSkipCount, vectorParamIndices, { returnParamIndex } );
      declaration += chainDeclaration;
      definition += chainDefinition;
      commandConstructed = true;
    }
  }
  return std::make_tuple( commandConstructed, declaration, definition );
}

std::tuple<bool, std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionVoid2Return(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::vector<size_t> const &                        returnParamIndices ) const
{
  // two return parameters (and the first one is not a handle, that would be a constructor)
  assert( returnParamIndices.size() == 2 );
  assert( !isHandleType( commandIt->second.params[returnParamIndices[0]].type.type ) );

  bool                     commandConstructed = false;
  std::string              declaration, definition;
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndicesNew( commandIt->second.params );
  switch ( vectorParamIndices.size() )
  {
    case 1:
      {
        // two returns but just one vector
        auto vectorParamIndexIt = vectorParamIndices.begin();
        if ( ( vectorParamIndexIt->second == returnParamIndices[0] ) &&
             ( vectorParamIndexIt->first == returnParamIndices[1] ) )
        {
          // the size is a return value as well -> enumerate the values
          std::tie( declaration, definition ) = constructRAIIHandleMemberFunctionVoidEnumerate(
            commandIt, initialSkipCount, vectorParamIndices, returnParamIndices );
          commandConstructed = true;
        }
      }
      break;
  }
  if ( isStructureChainAnchor( commandIt->second.params[returnParamIndices[0]].type.type ) )
  {
    commandConstructed = false;
  }
  if ( isStructureChainAnchor( commandIt->second.params[returnParamIndices[1]].type.type ) )
  {
    commandConstructed = false;
    if ( vectorParamIndices.size() == 1 )
    {
      // two returns but just one vector
      auto vectorParamIndexIt = vectorParamIndices.begin();
      if ( ( vectorParamIndexIt->second == returnParamIndices[0] ) &&
           ( vectorParamIndexIt->first == returnParamIndices[1] ) )
      {
        // the size is a return value as well -> enumerate the values
        std::string vectorElementType = commandIt->second.params[vectorParamIndices.begin()->first].type.type;
        if ( commandIt->second.returnType == "void" )
        {
          std::string chainDeclaration, chainDefinition;
          std::tie( chainDeclaration, chainDefinition ) = constructRAIIHandleMemberFunctionVoidEnumerateChain(
            commandIt, initialSkipCount, vectorParamIndices, returnParamIndices );
          declaration += chainDeclaration;
          definition += chainDefinition;
          commandConstructed = true;
        }
      }
    }
  }
  return std::make_tuple( commandConstructed, declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionVoidEnumerate(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  assert( nonConstPointerParamIndices.size() == 2 );
  assert( vectorParamIndices.size() == 1 );

  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );
  std::string vectorElementType =
    stripPostfix( commandIt->second.params[vectorParamIndices.begin()->first].type.compose(), "*" );
  std::string counterName =
    startLowerCase( stripPrefix( commandIt->second.params[vectorParamIndices.begin()->second].name, "p" ) );
  std::string vectorName =
    startLowerCase( stripPrefix( commandIt->second.params[vectorParamIndices.begin()->first].name, "p" ) );
  std::string firstCallArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, true, {}, nonConstPointerParamIndices, true );
  std::string secondCallArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true );

  std::string const declarationTemplate =
    R"(
${enter}    VULKAN_HPP_NODISCARD std::vector<${vectorElementType}> ${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave },
                                              { "vectorElementType", vectorElementType } } );

  const std::string definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<${vectorElementType}> ${className}::${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT
  {${functionPointerCheck}
    ${counterType} ${counterName};
    getDispatcher()->${vkCommand}( ${firstCallArguments} );
    std::vector<${vectorElementType}> ${vectorName}( ${counterName} );
    getDispatcher()->${vkCommand}( ${secondCallArguments} );
    VULKAN_HPP_ASSERT( ${counterName} <= ${vectorName}.size() );
    return ${vectorName};
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "counterName", counterName },
      { "counterType", commandIt->second.params[vectorParamIndices.begin()->second].type.type },
      { "enter", enter },
      { "firstCallArguments", firstCallArguments },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "secondCallArguments", secondCallArguments },
      { "vectorElementType", vectorElementType },
      { "vectorName", vectorName },
      { "vkCommand", commandIt->first } } );
  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionVoidEnumerateChain(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string counterName =
    startLowerCase( stripPrefix( commandIt->second.params[vectorParamIndices.begin()->second].name, "p" ) );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave )       = generateProtection( commandIt->second.referencedIn );
  std::string firstCallArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, true, {}, nonConstPointerParamIndices, true );
  std::string secondCallArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true );
  std::string vectorElementType =
    stripPostfix( commandIt->second.params[vectorParamIndices.begin()->first].type.compose(), "*" );
  std::string vectorName =
    startLowerCase( stripPrefix( commandIt->second.params[vectorParamIndices.begin()->first].name, "p" ) );

  std::string const declarationTemplate =
    R"(
${enter}    template <typename StructureChain>
    VULKAN_HPP_NODISCARD std::vector<StructureChain> ${commandName}( ${argumentList} ) const;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave } } );

  const std::string definitionTemplate =
    R"(
${enter}  template <typename StructureChain>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE std::vector<StructureChain> ${className}::${commandName}( ${argumentList} ) const
  {${functionPointerCheck}
    ${counterType} ${counterName};
    getDispatcher()->${vkCommand}( ${firstCallArguments} );
    std::vector<StructureChain> returnVector( ${counterName} );
    std::vector<${vectorElementType}> ${vectorName}( ${counterName} );
    for ( ${counterType} i = 0; i < ${counterName}; i++ )
    {
      ${vectorName}[i].pNext = returnVector[i].template get<${vectorElementType}>().pNext;
    }
    getDispatcher()->${vkCommand}( ${secondCallArguments} );
    VULKAN_HPP_ASSERT( ${counterName} <= ${vectorName}.size() );
    for ( ${counterType} i = 0; i < ${counterName}; i++ )
    {
      returnVector[i].template get<${vectorElementType}>() = ${vectorName}[i];
    }
    return returnVector;
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "counterName", counterName },
      { "counterType", commandIt->second.params[vectorParamIndices.begin()->second].type.type },
      { "enter", enter },
      { "firstCallArguments", firstCallArguments },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "secondCallArguments", secondCallArguments },
      { "vectorElementType", vectorElementType },
      { "vectorName", vectorName },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionVoidGetChain(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string callArguments = constructCallArgumentsEnhanced(
    commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );
  std::string returnType = stripPostfix( commandIt->second.params[nonConstPointerParamIndices[0]].type.compose(), "*" );
  std::string returnVariable =
    startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[0]].name, "p" ) );

  std::string const declarationTemplate =
    R"(
${enter}  template <typename X, typename Y, typename... Z>
    VULKAN_HPP_NODISCARD StructureChain<X, Y, Z...> ${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT;
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            { { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave } } );

  std::string const definitionTemplate =
    R"(
${enter}  template <typename X, typename Y, typename... Z>
  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE StructureChain<X, Y, Z...> ${className}::${commandName}( ${argumentList} ) const VULKAN_HPP_NOEXCEPT
  {${functionPointerCheck}
    StructureChain<X, Y, Z...> structureChain;
    ${returnType} & ${returnVariable} = structureChain.template get<${returnType}>();
    getDispatcher()->${vkCommand}( ${callArguments} );
    return structureChain;
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments", callArguments },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "returnVariable", returnVariable },
      { "returnType", returnType },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::pair<std::string, std::string> VulkanHppGenerator::constructRAIIHandleMemberFunctionVoidGetValue(
  std::map<std::string, CommandData>::const_iterator commandIt,
  size_t                                             initialSkipCount,
  std::map<size_t, size_t> const &                   vectorParamIndices,
  std::vector<size_t> const &                        nonConstPointerParamIndices ) const
{
  assert( nonConstPointerParamIndices.size() == 1 );

  std::set<size_t> skippedParameters = determineSkippedParams(
    commandIt->second.params, initialSkipCount, vectorParamIndices, nonConstPointerParamIndices, false );
  std::string argumentListDeclaration =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, false, false, false, false );
  std::string argumentListDefinition =
    constructArgumentListEnhanced( commandIt->second.params, skippedParameters, {}, true, false, false, false );
  std::string commandName =
    determineCommandName( commandIt->first, commandIt->second.params[initialSkipCount - 1].type.type, m_tags );
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( commandIt->second.referencedIn );
  std::string returnType = stripPostfix( commandIt->second.params[nonConstPointerParamIndices[0]].type.compose(), "*" );
  std::pair<bool, std::map<size_t, std::vector<size_t>>> vectorSizeCheck = needsVectorSizeCheck( vectorParamIndices );
  std::string noexceptString = vectorSizeCheck.first ? "VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS" : "VULKAN_HPP_NOEXCEPT";

  std::string const declarationTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD ${returnType} ${commandName}( ${argumentList} ) const ${noexcept};
${leave})";

  std::string declaration = replaceWithMap( declarationTemplate,
                                            {
                                              { "argumentList", argumentListDeclaration },
                                              { "commandName", commandName },
                                              { "enter", enter },
                                              { "leave", leave },
                                              { "noexcept", noexceptString },
                                              { "returnType", returnType },
                                            } );

  std::string const definitionTemplate =
    R"(
${enter}  VULKAN_HPP_NODISCARD VULKAN_HPP_INLINE ${returnType} ${className}::${commandName}( ${argumentList} ) const ${noexcept}
  {${functionPointerCheck}${vectorSizeCheck}
    ${returnType} ${returnVariable};
    getDispatcher()->${vkCommand}( ${callArguments} );
    return ${returnVariable};
  }
${leave})";

  std::string definition = replaceWithMap(
    definitionTemplate,
    { { "argumentList", argumentListDefinition },
      { "callArguments",
        constructCallArgumentsEnhanced(
          commandIt->second.params, initialSkipCount, false, {}, nonConstPointerParamIndices, true ) },
      { "className", stripPrefix( commandIt->second.params[initialSkipCount - 1].type.type, "Vk" ) },
      { "commandName", commandName },
      { "enter", enter },
      { "functionPointerCheck", constructFunctionPointerCheck( commandIt->first, commandIt->second.referencedIn ) },
      { "leave", leave },
      { "noexcept", noexceptString },
      { "vectorSizeCheck",
        vectorSizeCheck.first
          ? constructVectorSizeCheck(
              commandIt->first, commandIt->second, initialSkipCount, vectorSizeCheck.second, skippedParameters )
          : "" },
      { "returnType", returnType },
      { "returnVariable",
        startLowerCase( stripPrefix( commandIt->second.params[nonConstPointerParamIndices[0]].name, "p" ) ) },
      { "vkCommand", commandIt->first } } );

  return std::make_pair( declaration, definition );
}

std::string VulkanHppGenerator::constructRAIIHandleSingularConstructorArguments(
  std::pair<std::string, HandleData> const &         handle,
  std::map<std::string, CommandData>::const_iterator constructorIt ) const
{
  std::string arguments = startLowerCase( stripPrefix( handle.first, "Vk" ) );
  if ( handle.second.destructorIt != m_commands.end() )
  {
    for ( auto const & destructorParam : handle.second.destructorIt->second.params )
    {
      if ( ( destructorParam.type.type != handle.first ) &&
           ( std::find_if( handle.second.destructorIt->second.params.begin(),
                           handle.second.destructorIt->second.params.end(),
                           [&destructorParam]( ParamData const & pd ) { return pd.len == destructorParam.name; } ) ==
             handle.second.destructorIt->second.params.end() ) )
      {
        if ( std::find_if( constructorIt->second.params.begin(),
                           constructorIt->second.params.end(),
                           [&destructorParam]( ParamData const & pd ) {
                             return pd.type.type == destructorParam.type.type;
                           } ) != constructorIt->second.params.end() )
        {
          if ( isHandleType( destructorParam.type.type ) )
          {
            assert( destructorParam.type.isValue() );
            arguments += ", static_cast<" + destructorParam.type.type + ">( *" + destructorParam.name + " )";
          }
          else
          {
            assert( destructorParam.type.type == "VkAllocationCallbacks" );
            arguments +=
              ", reinterpret_cast<const VkAllocationCallbacks *>( static_cast<const VULKAN_HPP_NAMESPACE::AllocationCallbacks *>( allocator ) )";
          }
        }
        else
        {
#if !defined( NDEBUG )
          bool found = false;
#endif
          for ( auto const & constructorParam : constructorIt->second.params )
          {
            auto structureIt = m_structures.find( constructorParam.type.type );
            if ( structureIt != m_structures.end() )
            {
              auto memberIt = std::find_if( structureIt->second.members.begin(),
                                            structureIt->second.members.end(),
                                            [&destructorParam]( MemberData const & md )
                                            { return md.type.type == destructorParam.type.type; } );
              if ( memberIt != structureIt->second.members.end() )
              {
#if !defined( NDEBUG )
                found = true;
#endif
                assert( !constructorParam.type.isValue() );
                std::string argument =
                  startLowerCase( stripPrefix( constructorParam.name, "p" ) ) + "." + memberIt->name;
                if ( isHandleType( memberIt->type.type ) )
                {
                  argument = "static_cast<" + memberIt->type.type + ">( " + argument + " )";
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

std::string VulkanHppGenerator::constructRAIIHandleVectorSizeCheck(
  std::string const &                           name,
  CommandData const &                           commandData,
  size_t                                        initialSkipCount,
  std::map<size_t, std::vector<size_t>> const & countToVectorMap,
  std::set<size_t> const &                      skippedParams ) const
{
  std::string const throwTemplate =
    R"#(    if ( ${zeroSizeCheck}${firstVectorName}.size() != ${secondVectorName}.size() )
  {
    throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::${className}::${commandName}: ${firstVectorName}.size() != ${secondVectorName}.size()" );
  })#";

  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );

  std::string sizeChecks;
  for ( auto const & cvm : countToVectorMap )
  {
    assert( !commandData.params[cvm.second[0]].optional );

    size_t      defaultStartIndex = determineDefaultStartIndex( commandData.params, skippedParams );
    std::string firstVectorName   = startLowerCase( stripPrefix( commandData.params[cvm.second[0]].name, "p" ) );

    for ( size_t i = 1; i < cvm.second.size(); i++ )
    {
      std::string secondVectorName = startLowerCase( stripPrefix( commandData.params[cvm.second[i]].name, "p" ) );
      bool withZeroSizeCheck = commandData.params[cvm.second[i]].optional && ( defaultStartIndex <= cvm.second[i] );
      sizeChecks +=
        replaceWithMap( throwTemplate,
                        { { "firstVectorName", firstVectorName },
                          { "className", stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) },
                          { "commandName", commandName },
                          { "secondVectorName", secondVectorName },
                          { "zeroSizeCheck", withZeroSizeCheck ? ( "!" + secondVectorName + ".empty() && " ) : "" } } );
      if ( i + 1 < cvm.second.size() )
      {
        sizeChecks += "\n";
      }
    }
  }
  if ( !sizeChecks.empty() )
  {
    sizeChecks += "\n";
  }

  return sizeChecks;
}

std::string
  VulkanHppGenerator::constructRAIIHandleUpgradeConstructor( std::pair<std::string, HandleData> const & handle ) const
{
  std::string typeName                      = startLowerCase( stripPrefix( handle.first, "Vk" ) );
  std::string constructorArguments          = handle.first + " " + typeName;
  std::string constructorInitializationList = "m_" + typeName + "( " + typeName + " )";
  if ( handle.second.destructorIt != m_commands.end() )
  {
    for ( auto const & destructorParam : handle.second.destructorIt->second.params )
    {
      if ( ( destructorParam.type.type != handle.first ) &&
           ( std::find_if( handle.second.destructorIt->second.params.begin(),
                           handle.second.destructorIt->second.params.end(),
                           [&destructorParam]( ParamData const & pd ) { return pd.len == destructorParam.name; } ) ==
             handle.second.destructorIt->second.params.end() ) )
      {
        if ( isHandleType( destructorParam.type.type ) )
        {
          assert( destructorParam.type.isValue() );
          std::string name = destructorParam.name;
          constructorArguments += ", " + destructorParam.type.type + " " + name;
          constructorInitializationList += ", m_" + name + "( " + name + " )";
        }
        else
        {
          assert( destructorParam.type.type == "VkAllocationCallbacks" );
          constructorArguments += ", VkAllocationCallbacks const * allocator";
          constructorInitializationList += ", m_allocator( allocator )";
        }
      }
    }
  }
  if ( isMultiSuccessCodeConstructor( handle.second.constructorIts ) )
  {
    constructorArguments += ", VULKAN_HPP_NAMESPACE::Result successCode";
    constructorInitializationList += ", m_constructorSuccessCode( successCode )";
  }
  std::string dispatcherType = ( handle.second.constructorIts.front()->second.params.front().type.type == "VkDevice" )
                                 ? "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::DeviceDispatcher"
                                 : "VULKAN_HPP_NAMESPACE::VULKAN_HPP_RAII_NAMESPACE::InstanceDispatcher";

  const std::string protectedConstructorTemplate = R"(  
    ${handleType}( ${arguments}, ${dispatcherType} const * dispatcher )
      : ${initializationList}, m_dispatcher( dispatcher )
    {})";

  return replaceWithMap( protectedConstructorTemplate,
                         { { "arguments", constructorArguments },
                           { "dispatcherType", dispatcherType },
                           { "handleType", stripPrefix( handle.first, "Vk" ) },
                           { "initializationList", constructorInitializationList } } );
}

std::string VulkanHppGenerator::constructReturnType( CommandData const & commandData,
                                                     std::string const & baseType ) const
{
  return ( 1 < commandData.successCodes.size() )
           ? ( ( baseType == "void" ) ? "Result" : ( "ResultValue<" + baseType + ">" ) )
           : ( "typename ResultValueType<" + baseType + ">::type" );
}

std::string VulkanHppGenerator::constructSuccessCheck( std::vector<std::string> const & successCodes ) const
{
  assert( !successCodes.empty() );
  std::string successCheck = "result == VULKAN_HPP_NAMESPACE::Result::" + createSuccessCode( successCodes[0], m_tags );
  if ( 1 < successCodes.size() )
  {
    successCheck = "( " + successCheck + " )";
    for ( size_t i = 1; i < successCodes.size(); ++i )
    {
      successCheck +=
        "|| ( result == VULKAN_HPP_NAMESPACE::Result::" + createSuccessCode( successCodes[i], m_tags ) + " )";
    }
  }
  return successCheck;
}

std::string VulkanHppGenerator::constructSuccessCodeList( std::vector<std::string> const & successCodes ) const
{
  std::string successCodeList;
  if ( 1 < successCodes.size() )
  {
    successCodeList = ", { VULKAN_HPP_NAMESPACE::Result::" + createSuccessCode( successCodes[0], m_tags );
    for ( size_t i = 1; i < successCodes.size(); ++i )
    {
      successCodeList += ", VULKAN_HPP_NAMESPACE::Result::" + createSuccessCode( successCodes[i], m_tags );
    }
    successCodeList += " }";
  }
  return successCodeList;
}

std::string
  VulkanHppGenerator::constructVectorSizeCheck( std::string const &                           name,
                                                CommandData const &                           commandData,
                                                size_t                                        initialSkipCount,
                                                std::map<size_t, std::vector<size_t>> const & countToVectorMap,
                                                std::set<size_t> const &                      skippedParams ) const
{
  std::string str;

  std::string const assertTemplate =
    "    VULKAN_HPP_ASSERT( ${zeroSizeCheck}${firstVectorName}.size() == ${secondVectorName}.size() );";
  std::string const throwTemplate =
    R"#(    if ( ${zeroSizeCheck}${firstVectorName}.size() != ${secondVectorName}.size() )
  {
    throw LogicError( VULKAN_HPP_NAMESPACE_STRING "::${className}::${commandName}: ${firstVectorName}.size() != ${secondVectorName}.size()" );
  })#";

  std::string commandName = determineCommandName( name, commandData.params[initialSkipCount - 1].type.type, m_tags );

  std::string assertions, throws;
  for ( auto const & cvm : countToVectorMap )
  {
    assert( !commandData.params[cvm.second[0]].optional );

    size_t      defaultStartIndex = determineDefaultStartIndex( commandData.params, skippedParams );
    std::string firstVectorName   = startLowerCase( stripPrefix( commandData.params[cvm.second[0]].name, "p" ) );

    for ( size_t i = 1; i < cvm.second.size(); i++ )
    {
      std::string secondVectorName = startLowerCase( stripPrefix( commandData.params[cvm.second[i]].name, "p" ) );
      bool withZeroSizeCheck = commandData.params[cvm.second[i]].optional && ( defaultStartIndex <= cvm.second[i] );
      assertions +=
        replaceWithMap( assertTemplate,
                        { { "firstVectorName", firstVectorName },
                          { "secondVectorName", secondVectorName },
                          { "zeroSizeCheck", withZeroSizeCheck ? ( secondVectorName + ".empty() || " ) : "" } } );
      throws +=
        replaceWithMap( throwTemplate,
                        { { "firstVectorName", firstVectorName },
                          { "className", stripPrefix( commandData.params[initialSkipCount - 1].type.type, "Vk" ) },
                          { "commandName", commandName },
                          { "secondVectorName", secondVectorName },
                          { "zeroSizeCheck", withZeroSizeCheck ? ( "!" + secondVectorName + ".empty() && " ) : "" } } );
      if ( i + 1 < cvm.second.size() )
      {
        assertions += "\n";
        throws += "\n";
      }
    }
  }

  std::string const sizeCheckTemplate =
    R"#(
#ifdef VULKAN_HPP_NO_EXCEPTIONS
${assertions}
#else
${throws}
#endif  /*VULKAN_HPP_NO_EXCEPTIONS*/
)#";

  str = replaceWithMap( sizeCheckTemplate, { { "assertions", assertions }, { "throws", throws } } );

  return str;
}

template <class InputIt, class UnaryPredicate>
std::vector<InputIt> findAll( InputIt first, InputIt last, UnaryPredicate p )
{
  std::vector<InputIt> result;
  while ( first != last )
  {
    if ( p( *first ) )
    {
      result.push_back( first );
    }
    ++first;
  }
  return result;
}

void VulkanHppGenerator::appendStructConstructors( std::string &                                 str,
                                                   std::pair<std::string, StructureData> const & structData,
                                                   std::string const &                           prefix ) const
{
  // the constructor with all the elements as arguments, with defaults
  // and the simple copy constructor from the corresponding vulkan structure
  static const std::string constructors = R"(
${prefix}${constexpr}${structName}(${arguments}) VULKAN_HPP_NOEXCEPT
${prefix}${initializers}
${prefix}{}

${prefix}${constexpr}${structName}( ${structName} const & rhs ) VULKAN_HPP_NOEXCEPT = default;

${prefix}${structName}( Vk${structName} const & rhs ) VULKAN_HPP_NOEXCEPT
${prefix}  : ${structName}( *reinterpret_cast<${structName} const *>( &rhs ) )
${prefix}{}
)";

  std::string arguments, initializers;
  bool        listedArgument = false;
  bool        firstArgument  = true;
  for ( auto const & member : structData.second.members )
  {
    // gather the arguments
    listedArgument = appendStructConstructorArgument( arguments, listedArgument, member, true );

    // gather the initializers; skip member 'pNext' and constant members
    if ( ( member.name != "pNext" ) && ( member.values.size() != 1 ) )
    {
      initializers += ( firstArgument ? ":" : "," ) + std::string( " " ) + member.name + "( " + member.name + "_ )";
      firstArgument = false;
    }
  }

  str += replaceWithMap( constructors,
                         { { "arguments", arguments },
                           { "constexpr", constructConstexprString( structData, false ) },
                           { "initializers", initializers },
                           { "prefix", prefix },
                           { "structName", stripPrefix( structData.first, "Vk" ) } } );

  appendStructConstructorsEnhanced( str, structData, prefix );
}

void VulkanHppGenerator::appendStructConstructorsEnhanced( std::string &                                 str,
                                                           std::pair<std::string, StructureData> const & structData,
                                                           std::string const &                           prefix ) const
{
  auto memberIts = findAll( structData.second.members.begin(),
                            structData.second.members.end(),
                            []( MemberData const & md )
                            { return !md.len.empty() && ( ignoreLens.find( md.len.front() ) == ignoreLens.end() ); } );
  if ( !memberIts.empty() )
  {
    // maximal one member to be handled by an ArrayProxyNoTemporaries is of type void
    assert( std::count_if( memberIts.begin(), memberIts.end(), []( auto it ) { return it->type.type == "void"; } ) <=
            1 );

    // map from len-members to all the array members using that len
    std::map<std::vector<MemberData>::const_iterator, std::vector<std::vector<MemberData>::const_iterator>> lenIts;
    for ( auto const & mit : memberIts )
    {
      std::string lenName =
        ( mit->len.front() == R"(latexmath:[\textrm{codeSize} \over 4])" ) ? "codeSize" : mit->len.front();
      auto lenIt = std::find_if(
        structData.second.members.begin(), mit, [&lenName]( MemberData const & md ) { return md.name == lenName; } );
      assert( lenIt != mit );
      lenIts[lenIt].push_back( mit );
    }

    std::string arguments, initializers;
    bool        listedArgument = false;
    bool        firstArgument  = true;
    bool        arrayListed    = false;
    std::string templateHeader, sizeChecks;
    for ( auto mit = structData.second.members.begin(); mit != structData.second.members.end(); ++mit )
    {
      // gather the initializers; skip member 'pNext' and constant members
      if ( ( mit->name != "pNext" ) && ( mit->values.size() != 1 ) )
      {
        auto litit = lenIts.find( mit );
        if ( litit != lenIts.end() )
        {
          // len arguments just have an initalizer, from the ArrayProxyNoTemporaries size
          initializers += ( firstArgument ? ": " : ", " ) + mit->name + "( " +
                          generateLenInitializer( mit, litit, structData.second.mutualExclusiveLens ) + " )";
          sizeChecks += generateSizeCheck(
            litit->second, stripPrefix( structData.first, "Vk" ), prefix, structData.second.mutualExclusiveLens );
        }
        else if ( std::find( memberIts.begin(), memberIts.end(), mit ) != memberIts.end() )
        {
          assert( beginsWith( mit->name, "p" ) );
          std::string argumentName = startLowerCase( stripPrefix( mit->name, "p" ) ) + "_";

          assert( endsWith( mit->type.postfix, "*" ) );
          std::string argumentType = stripPostfix( mit->type.compose(), "*" );
          if ( mit->type.type == "void" )
          {
            templateHeader = prefix + "template <typename T>\n";

            size_t pos = argumentType.find( "void" );
            assert( pos != std::string::npos );
            argumentType.replace( pos, strlen( "void" ), "T" );
          }

          arguments += listedArgument ? ", " : "";
          arguments += "VULKAN_HPP_NAMESPACE::ArrayProxyNoTemporaries<" + argumentType + "> const & " + argumentName;
          if ( arrayListed )
          {
            arguments += " = {}";
          }
          listedArgument = true;
          arrayListed    = true;

          initializers += ( firstArgument ? ": " : ", " ) + mit->name + "( " + argumentName + ".data() )";
        }
        else
        {
          listedArgument = appendStructConstructorArgument( arguments, listedArgument, *mit, arrayListed );
          initializers += ( firstArgument ? ": " : ", " ) + mit->name + "( " + mit->name + "_ )";
        }
        firstArgument = false;
      }
    }
    static const std::string constructorTemplate = R"(
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
${templateHeader}${prefix}${structName}( ${arguments} )
${prefix}${initializers}
${prefix}{${sizeChecks}}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

    str += replaceWithMap( constructorTemplate,
                           { { "arguments", arguments },
                             { "initializers", initializers },
                             { "prefix", prefix },
                             { "sizeChecks", sizeChecks },
                             { "structName", stripPrefix( structData.first, "Vk" ) },
                             { "templateHeader", templateHeader } } );
  }
}

bool VulkanHppGenerator::appendStructConstructorArgument( std::string &      str,
                                                          bool               listedArgument,
                                                          MemberData const & memberData,
                                                          bool               withDefault ) const
{
  // skip members 'pNext' and members with a single value, as they are never explicitly set
  if ( ( memberData.name != "pNext" ) && ( memberData.values.size() != 1 ) )
  {
    str += ( listedArgument ? ( ", " ) : "" );
    if ( memberData.arraySizes.empty() )
    {
      str += memberData.type.compose() + " ";
    }
    else
    {
      str += constructStandardArray( memberData.type.compose(), memberData.arraySizes ) + " const & ";
    }
    str += memberData.name + "_";

    if ( withDefault )
    {
      str += " = ";
      auto enumIt = m_enums.find( memberData.type.type );
      if ( enumIt != m_enums.end() && memberData.type.postfix.empty() )
      {
        appendEnumInitializer( str, memberData.type, memberData.arraySizes, enumIt->second.values );
      }
      else
      {
        assert( memberData.values.empty() );
        // all the rest can be initialized with just {}
        str += "{}";
      }
    }

    listedArgument = true;
  }
  return listedArgument;
}

std::string VulkanHppGenerator::appendStructMembers( std::string &                                 str,
                                                     std::pair<std::string, StructureData> const & structData,
                                                     std::string const &                           prefix ) const
{
  std::string sTypeValue;
  for ( auto const & member : structData.second.members )
  {
    str += prefix;
    if ( !member.bitCount.empty() && beginsWith( member.type.type, "Vk" ) )
    {
      assert( member.type.prefix.empty() && member.type.postfix.empty() );  // never encounterd a different case
      str += member.type.type;
    }
    else if ( member.arraySizes.empty() )
    {
      str += member.type.compose();
    }
    else
    {
      assert( member.type.prefix.empty() && member.type.postfix.empty() );
      str += constructStandardArrayWrapper( member.type.compose(), member.arraySizes );
    }
    str += " " + member.name;
    if ( !member.values.empty() )
    {
      // special handling for members with legal value: arbitrarily use the first one as the default
      auto enumIt = m_enums.find( member.type.type );
      assert( enumIt != m_enums.end() );
      {
        std::string enumValue = member.values.front();
        auto        nameIt =
          std::find_if( enumIt->second.values.begin(),
                        enumIt->second.values.end(),
                        [&enumValue]( EnumValueData const & evd ) { return enumValue == evd.vulkanValue; } );
        assert( nameIt != enumIt->second.values.end() );
        str += " = " + stripPrefix( member.type.type, "Vk" ) + "::" + nameIt->vkValue;
        if ( member.name == "sType" )
        {
          sTypeValue = nameIt->vkValue;
        }
      }
    }
    else
    {
      // as we don't have any meaningful default initialization values, everything can be initialized by just '{}' !
      assert( member.arraySizes.empty() || member.bitCount.empty() );
      if ( !member.bitCount.empty() )
      {
        str += " : " + member.bitCount;  // except for bitfield members, where no default member initializatin is
                                         // supported (up to C++20)
      }
      else
      {
        str += " = ";
        auto enumIt = m_enums.find( member.type.type );
        if ( member.arraySizes.empty() && ( enumIt != m_enums.end() ) && member.type.postfix.empty() )
        {
          appendEnumInitializer( str, member.type, member.arraySizes, enumIt->second.values );
        }
        else
        {
          str += "{}";
        }
      }
    }
    str += ";\n";
  }
  return sTypeValue;
}

void VulkanHppGenerator::appendStructs( std::string & str )
{
  for ( auto const & structure : m_structures )
  {
    if ( m_listedTypes.find( structure.first ) == m_listedTypes.end() )
    {
      assert( m_listingTypes.empty() );
      appendStruct( str, structure );
      assert( m_listingTypes.empty() );
    }
  }
}

void VulkanHppGenerator::appendStructSetter( std::string &                   str,
                                             std::string const &             structureName,
                                             std::vector<MemberData> const & memberData,
                                             size_t                          index ) const
{
  MemberData const & member = memberData[index];
  if ( member.type.type != "VkStructureType" )  // filter out StructureType, which is supposed to be immutable !
  {
    static const std::string templateString = R"(
    ${structureName} & set${MemberName}( ${memberType} ${reference}${memberName}_ ) VULKAN_HPP_NOEXCEPT
    {
      ${assignment};
      return *this;
    }
)";

    std::string memberType = member.arraySizes.empty()
                               ? member.type.compose()
                               : constructStandardArray( member.type.compose(), member.arraySizes );
    std::string assignment;
    if ( !member.bitCount.empty() && beginsWith( member.type.type, "Vk" ) )
    {
      assignment = member.name + " = " + "*reinterpret_cast<" + member.type.type + "*>(&" + member.name + "_)";
    }
    else
    {
      assignment = member.name + " = " + member.name + "_";
    }

    str += replaceWithMap(
      templateString,
      { { "assignment", assignment },
        { "memberName", member.name },
        { "MemberName", startUpperCase( member.name ) },
        { "memberType", memberType },
        { "reference",
          ( member.type.postfix.empty() && ( m_structures.find( member.type.type ) != m_structures.end() ) )
            ? "const & "
            : "" },
        { "structureName", structureName } } );

    if ( !member.len.empty() && ( ignoreLens.find( member.len[0] ) == ignoreLens.end() ) )
    {
      assert( member.name.front() == 'p' );
      std::string arrayName = startLowerCase( stripPrefix( member.name, "p" ) );

      std::string lenName, lenValue;
      if ( member.len[0] == R"(latexmath:[\textrm{codeSize} \over 4])" )
      {
        lenName  = "codeSize";
        lenValue = arrayName + "_.size() * 4";
      }
      else
      {
        lenName  = member.len[0];
        lenValue = arrayName + "_.size()";
      }

      assert( memberType.back() == '*' );
      memberType.pop_back();

      std::string templateHeader;
      if ( member.type.type == "void" )
      {
        templateHeader = "template <typename T>\n    ";

        size_t pos = memberType.find( "void" );
        assert( pos != std::string::npos );
        memberType.replace( pos, strlen( "void" ), "T" );

        lenValue += " * sizeof(T)";
      }

      auto lenMember = std::find_if(
        memberData.begin(), memberData.end(), [&lenName]( MemberData const & md ) { return md.name == lenName; } );
      assert( lenMember != memberData.end() && lenMember->type.prefix.empty() && lenMember->type.postfix.empty() );
      if ( lenMember->type.type != "size_t" )
      {
        lenValue = "static_cast<" + lenMember->type.type + ">( " + lenValue + " )";
      }

      static const std::string setArrayTemplate = R"(
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
    ${templateHeader}${structureName} & set${ArrayName}( VULKAN_HPP_NAMESPACE::ArrayProxyNoTemporaries<${memberType}> const & ${arrayName}_ ) VULKAN_HPP_NOEXCEPT
    {
      ${lenName} = ${lenValue};
      ${memberName} = ${arrayName}_.data();
      return *this;
    }
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

      str += replaceWithMap( setArrayTemplate,
                             { { "arrayName", arrayName },
                               { "ArrayName", startUpperCase( arrayName ) },
                               { "lenName", lenName },
                               { "lenValue", lenValue },
                               { "memberName", member.name },
                               { "memberType", memberType },
                               { "structureName", structureName },
                               { "templateHeader", templateHeader } } );
    }
  }
}

void VulkanHppGenerator::appendStructSubConstructor( std::string &                                 str,
                                                     std::pair<std::string, StructureData> const & structData,
                                                     std::string const &                           prefix ) const
{
  if ( !structData.second.subStruct.empty() )
  {
    auto const & subStruct = m_structures.find( structData.second.subStruct );
    assert( subStruct != m_structures.end() );

    std::string subStructArgumentName = startLowerCase( stripPrefix( subStruct->first, "Vk" ) );
    std::string ctorOpening           = prefix + "explicit " + stripPrefix( structData.first, "Vk" ) + "( ";
    std::string indentation           = std::string( ctorOpening.size(), ' ' );

    std::string subCopies;
    bool        firstArgument = true;
    for ( size_t i = 0; i < subStruct->second.members.size(); i++ )
    {
      assert( structData.second.members[i].arraySizes.empty() );
      subCopies += prefix + "  " + ( firstArgument ? ":" : "," ) + " " + structData.second.members[i].name + "( " +
                   subStructArgumentName + "." + subStruct->second.members[i].name + " )\n";
      firstArgument = false;
    }

    std::string subArguments;
    bool        listedArgument = true;
    for ( size_t i = subStruct->second.members.size(); i < structData.second.members.size(); i++ )
    {
      listedArgument =
        appendStructConstructorArgument( subArguments, listedArgument, structData.second.members[i], true );

      assert( structData.second.members[i].arraySizes.empty() );
      subCopies +=
        prefix + "  , " + structData.second.members[i].name + "( " + structData.second.members[i].name + "_ )\n";
    }

    str +=
      "\n"
      "    explicit " +
      stripPrefix( structData.first, "Vk" ) + "( " + stripPrefix( subStruct->first, "Vk" ) + " const & " +
      subStructArgumentName + subArguments + " )\n" + subCopies + "    {}\n";
  }
}

void VulkanHppGenerator::appendStructure( std::string &                                 str,
                                          std::pair<std::string, StructureData> const & structure ) const
{
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( structure.first, !structure.second.aliases.empty() );

  str += "\n" + enter;

  std::string constructorAndSetters;
  constructorAndSetters += "#if !defined( VULKAN_HPP_NO_STRUCT_CONSTRUCTORS )";
  appendStructConstructors( constructorAndSetters, structure, "    " );
  appendStructSubConstructor( constructorAndSetters, structure, "    " );
  constructorAndSetters += "#endif /*VULKAN_HPP_NO_STRUCT_CONSTRUCTORS*/\n";
  appendStructAssignmentOperators( constructorAndSetters, structure, "    " );
  if ( !structure.second.returnedOnly )
  {
    // only structs that are not returnedOnly get setters!
    for ( size_t i = 0; i < structure.second.members.size(); i++ )
    {
      appendStructSetter( constructorAndSetters, stripPrefix( structure.first, "Vk" ), structure.second.members, i );
    }
  }

  // operator==() and operator!=()
  // only structs without a union as a member can have a meaningfull == and != operation; we filter them out
  std::string compareOperators;
  if ( !containsUnion( structure.first ) )
  {
    appendStructCompareOperators( compareOperators, structure );
  }

  // the member variables
  std::string members    = "\n  public:\n";
  std::string sTypeValue = appendStructMembers( members, structure, "    " );

  static const std::string structureTemplate = R"(  struct ${structureName}
  {
${allowDuplicate}
${structureType}
${constructorAndSetters}

    operator ${vkName} const &() const VULKAN_HPP_NOEXCEPT
    {
      return *reinterpret_cast<const ${vkName}*>( this );
    }

    operator ${vkName} &() VULKAN_HPP_NOEXCEPT
    {
      return *reinterpret_cast<${vkName}*>( this );
    }

${compareOperators}

${members}
  };
  static_assert( sizeof( ${structureName} ) == sizeof( ${vkName} ), "struct and wrapper have different size!" );
  static_assert( std::is_standard_layout<${structureName}>::value, "struct wrapper is not a standard layout!" );
)";

  std::string structureName = stripPrefix( structure.first, "Vk" );
  std::string allowDuplicate, structureType;
  if ( !sTypeValue.empty() )
  {
    allowDuplicate = std::string( "    static const bool allowDuplicate = " ) +
                     ( structure.second.allowDuplicate ? "true;" : "false;" );
    structureType =
      "    static VULKAN_HPP_CONST_OR_CONSTEXPR StructureType structureType = StructureType::" + sTypeValue + ";\n";
  }
  str += replaceWithMap( structureTemplate,
                         { { "allowDuplicate", allowDuplicate },
                           { "structureName", structureName },
                           { "structureType", structureType },
                           { "constructorAndSetters", constructorAndSetters },
                           { "vkName", structure.first },
                           { "compareOperators", compareOperators },
                           { "members", members } } );

  if ( !sTypeValue.empty() )
  {
    std::string cppTypeTemplate = R"(
  template <>
  struct CppType<StructureType, StructureType::${sTypeValue}>
  {
    using Type = ${structureName};
  };
)";
    str += replaceWithMap( cppTypeTemplate, { { "sTypeValue", sTypeValue }, { "structureName", structureName } } );
  }

  for ( std::string const & alias : structure.second.aliases )
  {
    str += "  using " + stripPrefix( alias, "Vk" ) + " = " + stripPrefix( structure.first, "Vk" ) + ";\n";
  }

  str += leave;
}

void VulkanHppGenerator::appendStructureChainValidation( std::string & str )
{
  // append all template functions for the structure pointer chain validation
  for ( auto const & structure : m_structures )
  {
    if ( !structure.second.structExtends.empty() )
    {
      std::string enter, leave;
      std::tie( enter, leave ) = generateProtection( structure.first, !structure.second.aliases.empty() );

      str += enter;
      // append out allowed structure chains
      for ( auto extendName : structure.second.structExtends )
      {
        std::map<std::string, StructureData>::const_iterator itExtend = m_structures.find( extendName );
        if ( itExtend == m_structures.end() )
        {
          // look if the extendName acutally is an alias of some other structure
          itExtend = std::find_if( m_structures.begin(),
                                   m_structures.end(),
                                   [extendName]( auto const & sd )
                                   { return sd.second.aliases.find( extendName ) != sd.second.aliases.end(); } );
        }
        if ( itExtend == m_structures.end() )
        {
          std::string errorString;
          errorString = "<" + extendName + "> does not specify a struct in structextends field.";

          // check if symbol name is an alias to a struct
          auto itAlias =
            std::find_if( m_structures.begin(),
                          m_structures.end(),
                          [&extendName]( std::pair<std::string, StructureData> const & it ) -> bool {
                            return std::find( it.second.aliases.begin(), it.second.aliases.end(), extendName ) !=
                                   it.second.aliases.end();
                          } );
          if ( itAlias != m_structures.end() )
          {
            errorString += " The symbol is an alias and maps to <" + itAlias->first + ">.";
          }
          check( false, structure.second.xmlLine, errorString );
        }

        std::string subEnter, subLeave;
        std::tie( subEnter, subLeave ) = generateProtection( itExtend->first, !itExtend->second.aliases.empty() );

        if ( enter != subEnter )
        {
          str += subEnter;
        }

        str += "  template <> struct StructExtends<" + stripPrefix( structure.first, "Vk" ) + ", " +
               stripPrefix( extendName, "Vk" ) + ">{ enum { value = true }; };\n";

        if ( leave != subLeave )
        {
          str += subLeave;
        }
      }
      str += leave;
    }
  }
}

void VulkanHppGenerator::appendThrowExceptions( std::string & str ) const
{
  auto enumData = m_enums.find( "VkResult" );

  str +=
    "\n"
    "  [[noreturn]] static void throwResultException( Result result, char const * message )\n"
    "  {\n"
    "    switch ( result )\n"
    "    {\n";
  for ( auto const & value : enumData->second.values )
  {
    if ( beginsWith( value.vkValue, "eError" ) )
    {
      std::string enter, leave;
      if ( !value.extension.empty() )
      {
        std::tie( enter, leave ) = generateProtection( value.extension );
      }

      str += enter + "      case Result::" + value.vkValue + ": throw " + stripPrefix( value.vkValue, "eError" ) +
             "Error( message );\n" + leave;
    }
  }
  str +=
    "      default: throw SystemError( make_error_code( result ) );\n"
    "    }\n"
    "  }\n";
}

void VulkanHppGenerator::appendType( std::string & str, std::string const & typeName )
{
  if ( m_listedTypes.find( typeName ) == m_listedTypes.end() )
  {
    auto typeIt = m_types.find( typeName );
    assert( typeIt != m_types.end() );
    switch ( typeIt->second.category )
    {
      case TypeCategory::Handle:
        {
          auto handleIt = m_handles.find( typeName );
          if ( handleIt == m_handles.end() )
          {
            handleIt = std::find_if( m_handles.begin(),
                                     m_handles.end(),
                                     [&typeName]( std::pair<std::string, HandleData> const & hd )
                                     { return hd.second.alias == typeName; } );
            assert( handleIt != m_handles.end() );
            if ( m_listedTypes.find( handleIt->first ) == m_listedTypes.end() )
            {
              appendHandle( str, *handleIt );
            }
          }
          else
          {
            appendHandle( str, *handleIt );
          }
        }
        break;
      case TypeCategory::Struct:
      case TypeCategory::Union:
        {
          auto structIt = m_structures.find( typeName );
          if ( structIt == m_structures.end() )
          {
            structIt = std::find_if( m_structures.begin(),
                                     m_structures.end(),
                                     [&typeName]( std::pair<std::string, StructureData> const & sd )
                                     { return sd.second.aliases.find( typeName ) != sd.second.aliases.end(); } );
            assert( structIt != m_structures.end() );
            if ( m_listedTypes.find( structIt->first ) == m_listedTypes.end() )
            {
              appendStruct( str, *structIt );
            }
          }
          else
          {
            appendStruct( str, *structIt );
          }
        }
        break;
      default: m_listedTypes.insert( typeIt->first ); break;
    }
  }
}

void VulkanHppGenerator::appendUnion( std::string & str, std::pair<std::string, StructureData> const & structure ) const
{
  std::string enter, leave;
  std::tie( enter, leave ) = generateProtection( structure.first, !structure.second.aliases.empty() );

  str += "\n" + enter;
  std::string unionName = stripPrefix( structure.first, "Vk" );
  str += "  union " + unionName +
         "\n"
         "  {\n"
         "    " +
         unionName + "( VULKAN_HPP_NAMESPACE::" + unionName +
         " const & rhs ) VULKAN_HPP_NOEXCEPT\n"
         "    {\n"
         "      memcpy( static_cast<void*>( this ), &rhs, sizeof( VULKAN_HPP_NAMESPACE::" +
         unionName +
         " ) );\n"
         "    }\n";

  bool firstMember = true;
  for ( auto const & member : structure.second.members )
  {
    // VkBool32 is aliased to uint32_t. Don't create a VkBool32 constructor if the union also contains a uint32_t
    // constructor.
    auto compareBool32Alias = []( MemberData const & member )
    {
      return member.type.type == std::string( "uint32_t" );
    };
    if ( member.type.type == "VkBool32" )
    {
      if ( std::find_if( structure.second.members.begin(), structure.second.members.end(), compareBool32Alias ) !=
           structure.second.members.end() )
      {
        continue;
      }
    }

    static const std::string constructorTemplate = R"(
    ${unionName}( ${memberType} ${memberName}_${defaultAssignment} )
      : ${memberName}( ${memberName}_ )
    {}
)";

    std::string memberType =
      ( member.arraySizes.empty() )
        ? member.type.compose()
        : ( "const " + constructStandardArray( member.type.compose(), member.arraySizes ) + "&" );
    str += replaceWithMap( constructorTemplate,
                           { { "defaultAssignment", firstMember ? " = {}" : "" },
                             { "memberName", member.name },
                             { "memberType", memberType },
                             { "unionName", stripPrefix( structure.first, "Vk" ) } } );
    firstMember = false;
  }

  // one setter per union element
  for ( size_t i = 0; i < structure.second.members.size(); i++ )
  {
    appendStructSetter( str, stripPrefix( structure.first, "Vk" ), structure.second.members, i );
  }

  // assignment operator
  static const std::string operatorsTemplate = R"(
    VULKAN_HPP_NAMESPACE::${unionName} & operator=( VULKAN_HPP_NAMESPACE::${unionName} const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      memcpy( static_cast<void*>( this ), &rhs, sizeof( VULKAN_HPP_NAMESPACE::${unionName} ) );
      return *this;
    }

    operator Vk${unionName} const &() const
    {
      return *reinterpret_cast<const Vk${unionName}*>( this );
    }

    operator Vk${unionName} &()
    {
      return *reinterpret_cast<Vk${unionName}*>( this );
    }

)";
  str += replaceWithMap( operatorsTemplate, { { "unionName", stripPrefix( structure.first, "Vk" ) } } );

  // the union member variables
  // if there's at least one Vk... type in this union, check for unrestricted unions support
  bool needsUnrestrictedUnions = ( std::find_if( structure.second.members.begin(),
                                                 structure.second.members.end(),
                                                 []( MemberData const & member ) {
                                                   return beginsWith( member.type.type, "Vk" );
                                                 } ) != structure.second.members.end() );
  if ( needsUnrestrictedUnions )
  {
    str += "#ifdef VULKAN_HPP_HAS_UNRESTRICTED_UNIONS\n";
  }
  for ( auto const & member : structure.second.members )
  {
    str += "    " +
           ( member.arraySizes.empty() ? member.type.compose()
                                       : constructStandardArrayWrapper( member.type.compose(), member.arraySizes ) ) +
           " " + member.name + ";\n";
  }
  if ( needsUnrestrictedUnions )
  {
    str += "#else\n";
    for ( auto const & member : structure.second.members )
    {
      str += "    " + member.type.prefix + ( member.type.prefix.empty() ? "" : " " ) + member.type.type +
             member.type.postfix + " " + member.name + constructCArraySizes( member.arraySizes ) + ";\n";
    }
    str += "#endif  /*VULKAN_HPP_HAS_UNRESTRICTED_UNIONS*/\n";
  }
  str += "  };\n" + leave;
}

void VulkanHppGenerator::appendUniqueTypes( std::string &                 str,
                                            std::string const &           parentType,
                                            std::set<std::string> const & childrenTypes ) const
{
  str +=
    "\n"
    "#ifndef VULKAN_HPP_NO_SMART_HANDLE\n";
  if ( !parentType.empty() )
  {
    str += "  class " + stripPrefix( parentType, "Vk" ) + ";\n";
  }

  for ( auto const & childType : childrenTypes )
  {
    auto handleIt = m_handles.find( childType );
    assert( handleIt != m_handles.end() );

    std::string type          = stripPrefix( childType, "Vk" );
    std::string deleterType   = handleIt->second.deletePool.empty() ? "Object" : "Pool";
    std::string deleterAction = ( handleIt->second.deleteCommand.substr( 2, 4 ) == "Free" ) ? "Free" : "Destroy";
    std::string deleterParent = parentType.empty() ? "NoParent" : stripPrefix( parentType, "Vk" );
    std::string deleterPool =
      handleIt->second.deletePool.empty() ? "" : ", " + stripPrefix( handleIt->second.deletePool, "Vk" );

    std::string enter, leave;
    std::tie( enter, leave ) = generateProtection( handleIt->first, !handleIt->second.alias.empty() );

    str += enter + "  template <typename Dispatch> class UniqueHandleTraits<" + type +
           ", Dispatch> { public: using deleter = " + deleterType + deleterAction + "<" + deleterParent + deleterPool +
           ", Dispatch>; };\n"
           "  using Unique" +
           type + " = UniqueHandle<" + type + ", VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;\n";

    if ( !handleIt->second.alias.empty() )
    {
      str += "  using Unique" + stripPrefix( handleIt->second.alias, "Vk" ) + " = UniqueHandle<" + type +
             ", VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;\n";
    }
    str += leave;
  }
  str += "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/\n";
}

void VulkanHppGenerator::EnumData::addEnumAlias( int                 line,
                                                 std::string const & name,
                                                 std::string const & aliasName,
                                                 std::string const & vkName )
{
  auto aliasIt = aliases.find( name );
  check( ( aliasIt == aliases.end() ) || ( aliasIt->second.vulkanValue == aliasName ),
         line,
         "enum alias <" + name + "> already listed for a different enum value" );

  // only list aliases that map to different vkNames
  aliasIt = std::find_if( aliases.begin(),
                          aliases.end(),
                          [&vkName]( std::pair<std::string, EnumAliasData> const & aliasEntry )
                          { return vkName == aliasEntry.second.vkValue; } );
  if ( aliasIt == aliases.end() )
  {
    aliases.insert( std::make_pair( name, EnumAliasData( aliasName, vkName, line ) ) );
  }
}

void VulkanHppGenerator::EnumData::addEnumValue( int                 line,
                                                 std::string const & valueName,
                                                 bool                bitmask,
                                                 bool                bitpos,
                                                 std::string const & prefix,
                                                 std::string const & postfix,
                                                 std::string const & extension,
                                                 std::string const & tag )
{
  std::string translatedName = createEnumValueName( valueName, prefix, postfix, bitmask, tag );

  auto it = std::find_if( values.begin(),
                          values.end(),
                          [&translatedName]( EnumValueData const & evd ) { return evd.vkValue == translatedName; } );
  if ( it == values.end() )
  {
    values.emplace_back( line, valueName, translatedName, extension, bitpos );
  }
  else
  {
    check( it->vulkanValue == valueName,
           line,
           "enum value <" + valueName + "> maps to same C++-name as <" + it->vulkanValue + ">" );
  }
}

void VulkanHppGenerator::checkCorrectness()
{
  check( !m_vulkanLicenseHeader.empty(), -1, "missing license header" );

  // baseType checks
  for ( auto const & baseType : m_baseTypes )
  {
    check( m_types.find( baseType.second.type ) != m_types.end(),
           baseType.second.xmlLine,
           "basetype type <" + baseType.second.type + "> not specified" );
  }

  // bitmask checks
  for ( auto const & bitmask : m_bitmasks )
  {
    if ( !bitmask.second.requirements.empty() )
    {
      check( m_enums.find( bitmask.second.requirements ) != m_enums.end(),
             bitmask.second.xmlLine,
             "bitmask requires unknown <" + bitmask.second.requirements + ">" );
    }
  }

  // prepare command checks
  auto resultIt = m_enums.find( "VkResult" );
  assert( resultIt != m_enums.end() );
  std::set<std::string> resultCodes;
  for ( auto rc : resultIt->second.values )
  {
    resultCodes.insert( rc.vulkanValue );
  }
  for ( auto rc : resultIt->second.aliases )
  {
    resultCodes.insert( rc.first );
  }

  // command checks
  for ( auto const & command : m_commands )
  {
    check( !command.second.referencedIn.empty(),
           command.second.xmlLine,
           "command <" + command.first + "> not listed in any feature or extension" );
    for ( auto const & ec : command.second.errorCodes )
    {
      check( resultCodes.find( ec ) != resultCodes.end(),
             command.second.xmlLine,
             "command uses unknown error code <" + ec + ">" );
    }
    for ( auto const & sc : command.second.successCodes )
    {
      check( resultCodes.find( sc ) != resultCodes.end(),
             command.second.xmlLine,
             "command uses unknown success code <" + sc + ">" );
    }
    // check that functions returning a VkResult specify successcodes
    check( ( command.second.returnType != "VkResult" ) || !command.second.successCodes.empty(),
           command.second.xmlLine,
           "missing successcodes on command <" + command.first + "> returning VkResult!" );

    for ( auto const & p : command.second.params )
    {
      check( m_types.find( p.type.type ) != m_types.end(),
             p.xmlLine,
             "comand uses parameter of unknown type <" + p.type.type + ">" );
    }
    check( m_types.find( command.second.returnType ) != m_types.end(),
           command.second.xmlLine,
           "command uses unknown return type <" + command.second.returnType + ">" );
  }

  // enum checks
  for ( auto const & e : m_enums )
  {
    auto typeIt = m_types.find( e.first );
    assert( typeIt != m_types.end() );
    warn( !typeIt->second.referencedIn.empty(),
          e.second.xmlLine,
          "enum <" + e.first + "> not listed in any feature or extension" );

    // check that the aliasNames are known enum values or known aliases
    for ( auto const & alias : e.second.aliases )
    {
      check( ( std::find_if( e.second.values.begin(),
                             e.second.values.end(),
                             [&alias]( EnumValueData const & evd )
                             { return evd.vulkanValue == alias.second.vulkanValue; } ) != e.second.values.end() ) ||
               ( e.second.aliases.find( alias.second.vulkanValue ) != e.second.aliases.end() ),
             alias.second.xmlLine,
             "unknown enum alias <" + alias.second.vulkanValue + ">" );
    }
  }

  // extension checks
  for ( auto const & extension : m_extensions )
  {
    if ( !extension.second.deprecatedBy.empty() )
    {
      check( ( m_extensions.find( extension.second.deprecatedBy ) != m_extensions.end() ) ||
               ( m_features.find( extension.second.deprecatedBy ) != m_features.end() ),
             extension.second.xmlLine,
             "extension deprecated by to unknown extension/version <" + extension.second.promotedTo + ">" );
    }
    if ( !extension.second.obsoletedBy.empty() )
    {
      check( ( m_extensions.find( extension.second.obsoletedBy ) != m_extensions.end() ) ||
               ( m_features.find( extension.second.obsoletedBy ) != m_features.end() ),
             extension.second.xmlLine,
             "extension obsoleted by unknown extension/version <" + extension.second.promotedTo + ">" );
    }
    if ( !extension.second.promotedTo.empty() )
    {
      check( ( m_extensions.find( extension.second.promotedTo ) != m_extensions.end() ) ||
               ( m_features.find( extension.second.promotedTo ) != m_features.end() ),
             extension.second.xmlLine,
             "extension promoted to unknown extension/version <" + extension.second.promotedTo + ">" );
    }
    for ( auto const & require : extension.second.requirements )
    {
      check( m_extensions.find( require.first ) != m_extensions.end(),
             require.second,
             "extension <" + extension.first + "> lists an unknown require extension <" + require.first + ">" );
    }
  }

  // funcPointer checks
  for ( auto const & funcPointer : m_funcPointers )
  {
    if ( !funcPointer.second.requirements.empty() )
    {
      check( m_types.find( funcPointer.second.requirements ) != m_types.end(),
             funcPointer.second.xmlLine,
             "funcpointer requires unknown <" + funcPointer.second.requirements + ">" );
    }
  }

  // handle checks
  auto objectTypeIt = m_enums.find( "VkObjectType" );
  assert( objectTypeIt != m_enums.end() );
  for ( auto const & handle : m_handles )
  {
    check( m_handles.find( handle.second.parent ) != m_handles.end(),
           handle.second.xmlLine,
           "handle <" + handle.first + "> with unknown parent <" + handle.second.parent + ">" );

    if ( !handle.first.empty() )
    {
      assert( !handle.second.objTypeEnum.empty() );
      check( std::find_if( objectTypeIt->second.values.begin(),
                           objectTypeIt->second.values.end(),
                           [&handle]( EnumValueData const & evd ) {
                             return evd.vulkanValue == handle.second.objTypeEnum;
                           } ) != objectTypeIt->second.values.end(),
             handle.second.xmlLine,
             "handle <" + handle.first + "> specifies unknown \"objtypeenum\" <" + handle.second.objTypeEnum + ">" );
    }
  }
  for ( auto const & objectTypeValue : objectTypeIt->second.values )
  {
    if ( objectTypeValue.vkValue != "eUnknown" )
    {
      check( std::find_if( m_handles.begin(),
                           m_handles.end(),
                           [&objectTypeValue]( std::pair<std::string, HandleData> const & hd )
                           { return hd.second.objTypeEnum == objectTypeValue.vulkanValue; } ) != m_handles.end(),
             objectTypeValue.xmlLine,
             "VkObjectType value <" + objectTypeValue.vulkanValue +
               "> not specified as \"objtypeenum\" for any handle" );
    }
  }

  // structure checks
  std::set<std::string> sTypeValues;
  for ( auto const & structure : m_structures )
  {
    auto typeIt = m_types.find( structure.first );
    assert( typeIt != m_types.end() );
    check( !typeIt->second.referencedIn.empty(),
           structure.second.xmlLine,
           "structure <" + structure.first + "> not listed in any feature or extension" );

    for ( auto const & extend : structure.second.structExtends )
    {
      check(
        m_types.find( extend ) != m_types.end(), structure.second.xmlLine, "struct extends unknown <" + extend + ">" );
    }
    for ( auto const & member : structure.second.members )
    {
      if ( !member.selector.empty() )
      {
        std::string const & selector   = member.selector;
        auto                selectorIt = std::find_if( structure.second.members.begin(),
                                        structure.second.members.end(),
                                        [&selector]( MemberData const & md ) { return md.name == selector; } );
        assert( selectorIt != structure.second.members.end() );
        auto enumIt = m_enums.find( selectorIt->type.type );
        assert( enumIt != m_enums.end() );
        auto dataIt = m_structures.find( member.type.type );
        assert( ( dataIt != m_structures.end() ) && dataIt->second.isUnion );
        for ( auto const & data : dataIt->second.members )
        {
          assert( !data.selection.empty() );
          std::string const & selection = data.selection;
          check( std::find_if( enumIt->second.values.begin(),
                               enumIt->second.values.end(),
                               [&selection]( EnumValueData const & evd )
                               { return evd.vulkanValue == selection; } ) != enumIt->second.values.end(),
                 data.xmlLine,
                 "union member <" + data.name + "> uses selection <" + selection +
                   "> that is not part of the selector type <" + selectorIt->type.type + ">" );
        }
      }
      check( m_types.find( member.type.type ) != m_types.end(),
             member.xmlLine,
             "struct member uses unknown type <" + member.type.type + ">" );
      if ( !member.usedConstant.empty() )
      {
        check( m_constants.find( member.usedConstant ) != m_constants.end(),
               member.xmlLine,
               "struct member array size uses unknown constant <" + member.usedConstant + ">" );
      }
      if ( !member.values.empty() )
      {
        auto enumIt = m_enums.find( member.type.type );
        if ( enumIt != m_enums.end() )
        {
          for ( auto const & enumValue : member.values )
          {
            check( std::find_if( enumIt->second.values.begin(),
                                 enumIt->second.values.end(),
                                 [&enumValue]( auto const & evd )
                                 { return enumValue == evd.vulkanValue; } ) != enumIt->second.values.end(),
                   member.xmlLine,
                   "value <" + enumValue + "> for member <" + member.name + "> in structure <" + structure.first +
                     "> of enum type <" + member.type.type + "> not listed" );
          }
          // special handling for sType: no value should appear more than once
          if ( member.name == "sType" )
          {
            for ( auto const & enumValue : member.values )
            {
              check( sTypeValues.insert( enumValue ).second,
                     member.xmlLine,
                     "sType value <" + enumValue + "> has been used before" );
            }
          }
        }
        else if ( member.type.type == "uint32_t" )
        {
          for ( auto const & value : member.values )
          {
            check( value.find_first_not_of( "0123456789" ) == std::string::npos,
                   member.xmlLine,
                   "value <" + value + "> for member <" + member.name + "> in structure <" + structure.first +
                     "> of type <" + member.type.type + "> is not a number" );
          }
        }
        else
        {
          check( false,
                 member.xmlLine,
                 "member <" + member.name + "> in structure <" + structure.first + "> holds values <" +
                   ::toString( member.values ) + "> for an unhandled type <" + member.type.type + ">" );
        }
      }
    }
  }

  // enum checks
  // enum VkStructureType checks (need to be after structure checks)
  auto structureTypeIt = m_enums.find( "VkStructureType" );
  assert( structureTypeIt != m_enums.end() );
  for ( auto const & enumValue : structureTypeIt->second.values )
  {
    if ( ( enumValue.vulkanValue == "VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO" ) ||
         ( enumValue.vulkanValue == "VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO" ) )
    {
      check( sTypeValues.find( enumValue.vulkanValue ) == sTypeValues.end(),
             enumValue.xmlLine,
             "Reserved VkStructureType enum value <" + enumValue.vulkanValue + "> is used" );
    }
    else
    {
      check( sTypeValues.erase( enumValue.vulkanValue ) == 1,
             enumValue.xmlLine,
             "VkStructureType enum value <" + enumValue.vulkanValue + "> never used" );
    }
  }
  assert( sTypeValues.empty() );
}

bool VulkanHppGenerator::containsArray( std::string const & type ) const
{
  // a simple recursive check if a type is or contains an array
  auto structureIt = m_structures.find( type );
  bool found       = false;
  if ( structureIt != m_structures.end() )
  {
    for ( auto memberIt = structureIt->second.members.begin(); memberIt != structureIt->second.members.end() && !found;
          ++memberIt )
    {
      found = !memberIt->arraySizes.empty() || containsArray( memberIt->type.type );
    }
  }
  return found;
}

bool VulkanHppGenerator::containsUnion( std::string const & type ) const
{
  // a simple recursive check if a type is or contains a union
  auto structureIt = m_structures.find( type );
  bool found       = ( structureIt != m_structures.end() );
  if ( found )
  {
    found = structureIt->second.isUnion;
    for ( auto memberIt = structureIt->second.members.begin(); memberIt != structureIt->second.members.end() && !found;
          ++memberIt )
    {
      found = memberIt->type.prefix.empty() && memberIt->type.postfix.empty() && containsUnion( memberIt->type.type );
    }
  }
  return found;
}

size_t VulkanHppGenerator::determineDefaultStartIndex( std::vector<ParamData> const & params,
                                                       std::set<size_t> const &       skippedParams ) const
{
  size_t defaultStartIndex = INVALID_INDEX;
  for ( int i = static_cast<int>( params.size() ) - 1;
        ( 0 <= i ) && ( params[i].optional || ( skippedParams.find( i ) != skippedParams.end() ) );
        --i )
  {
    defaultStartIndex = i;
  }
  return defaultStartIndex;
}

std::string VulkanHppGenerator::determineEnhancedReturnType( CommandData const & commandData,
                                                             size_t              returnParamIndex,
                                                             bool                isStructureChain ) const
{
  assert( ( returnParamIndex == INVALID_INDEX ) || ( returnParamIndex < commandData.params.size() ) );

  std::string enhancedReturnType;
  assert( returnParamIndex != INVALID_INDEX );
  // if there is a return parameter, we think returnType is always "void" or "VkResult"
  // -> we can return that parameter
  assert( ( commandData.returnType == "void" ) || ( commandData.returnType == "VkResult" ) );
  assert( commandData.successCodes.empty() || ( commandData.successCodes[0] == "VK_SUCCESS" ) );
  return ( commandData.params[returnParamIndex].type.type == "void" )
           ? "std::vector<uint8_t,Allocator>"  // the return parameter is a vector-type parameter
         : isStructureChain ? "std::vector<StructureChain,Allocator>"  // for structureChain returns, it's just
                                                                       // a vector of StrutureChains
                            : "std::vector<" + stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" ) +
                                ",Allocator>";  // for the other parameters, we use a vector of the pure type
}

std::vector<std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator>
  VulkanHppGenerator::determineRAIIHandleConstructors( std::string const &                                handleType,
                                                       std::map<std::string, CommandData>::const_iterator destructorIt,
                                                       std::set<std::string> & specialFunctions ) const
{
  std::vector<std::map<std::string, CommandData>::const_iterator> constructorIts;
  for ( auto commandIt = m_commands.begin(); commandIt != m_commands.end(); )
  {
    commandIt =
      std::find_if( commandIt,
                    m_commands.end(),
                    [&handleType]( std::pair<std::string, CommandData> const & cd )
                    {
                      return std::find_if( cd.second.params.begin(),
                                           cd.second.params.end(),
                                           [&handleType]( ParamData const & pd ) {
                                             return ( pd.type.type == handleType ) && pd.type.isNonConstPointer();
                                           } ) != cd.second.params.end();
                    } );
    if ( commandIt != m_commands.end() )
    {
      // only commands that provide all information needed for the destructor can be considered a constructor!
      bool valid = true;
      if ( destructorIt != m_commands.end() )
      {
        auto paramIt = std::find_if( destructorIt->second.params.begin(),
                                     destructorIt->second.params.end(),
                                     [&handleType]( ParamData const & pd ) { return pd.type.type == handleType; } );
        assert( paramIt != destructorIt->second.params.end() );
        for ( auto pit = destructorIt->second.params.begin(); valid && pit != destructorIt->second.params.end(); ++pit )
        {
          valid = ( pit->name == paramIt->len ) || ( std::find_if( commandIt->second.params.begin(),
                                                                   commandIt->second.params.end(),
                                                                   [&pit]( ParamData const & pd ) {
                                                                     return pd.type.type == pit->type.type;
                                                                   } ) != commandIt->second.params.end() );
          for ( auto cit = commandIt->second.params.begin(); !valid && cit != commandIt->second.params.end(); ++cit )
          {
            auto structureIt = m_structures.find( cit->type.type );
            if ( structureIt != m_structures.end() )
            {
              valid = std::find_if( structureIt->second.members.begin(),
                                    structureIt->second.members.end(),
                                    [&pit]( MemberData const & md )
                                    { return md.type.type == pit->type.type; } ) != structureIt->second.members.end();
            }
          }
        }
      }
      if ( valid )
      {
        specialFunctions.insert( commandIt->first );
        // filter out alias functions
        if ( commandIt->second.alias.empty() )
        {
          constructorIts.push_back( commandIt );
        }
      }
      ++commandIt;
    }
  }
  assert( !constructorIts.empty() );
  return constructorIts;
}

std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator
  VulkanHppGenerator::determineRAIIHandleDestructor( std::string const & handleType ) const
{
  std::string type         = stripPrefix( handleType, "Vk" );
  auto        destructorIt = m_commands.find( "vkDestroy" + type );
  if ( destructorIt == m_commands.end() )
  {
    destructorIt = m_commands.find( "vkFree" + type + "s" );
    if ( destructorIt == m_commands.end() )
    {
      destructorIt = m_commands.find( "vkRelease" + type );
      if ( destructorIt == m_commands.end() )
      {
        if ( handleType == "VkDeviceMemory" )
        {
          // special handling for vkDeviceMemory
          destructorIt = m_commands.find( "vkFreeMemory" );
          assert( destructorIt != m_commands.end() );
        }
        else if ( handleType == "VkDisplayKHR" )
        {
          // special handling for VkDisplayKHR
          destructorIt = m_commands.find( "vkReleaseDisplayEXT" );
          assert( destructorIt != m_commands.end() );
        }
        else
        {
          assert( ( handleType == "VkDisplayModeKHR" ) || ( handleType == "VkPhysicalDevice" ) ||
                  ( handleType == "VkQueue" ) );
        }
      }
    }
  }
  return destructorIt;
}

size_t VulkanHppGenerator::determineReturnParamIndex( CommandData const &              commandData,
                                                      std::map<size_t, size_t> const & vectorParamIndices,
                                                      bool                             twoStep ) const
{
#if !defined( NDEBUG )
  for ( auto vpi : vectorParamIndices )
  {
    assert( ( vpi.first != vpi.second ) && ( vpi.first < commandData.params.size() ) &&
            ( ( vpi.second == INVALID_INDEX ) || ( vpi.second < commandData.params.size() ) ) );
  }
#endif

  size_t returnParamIndex = INVALID_INDEX;
  // for return types of type VkResult or void, we can determine a parameter to return
  if ( ( commandData.returnType == "VkResult" ) || ( commandData.returnType == "void" ) )
  {
    size_t index = commandData.params.size() - 1;
    if ( ( commandData.params[index].type.postfix.find( '*' ) != std::string::npos ) &&
         ( ( commandData.params[index].type.type != "void" ) || twoStep ||
           ( commandData.params[index].type.postfix.find( "**" ) != std::string::npos ) ) &&
         ( commandData.params[index].type.prefix.find( "const" ) == std::string::npos ) )
    {
      // it's a non-const pointer
      // assert that it's not a vector-size parameter
      assert( std::find_if( vectorParamIndices.begin(),
                            vectorParamIndices.end(),
                            [index]( std::pair<size_t, size_t> const & vpi )
                            { return vpi.second == index; } ) == vectorParamIndices.end() );

      std::map<size_t, size_t>::const_iterator vpit = vectorParamIndices.find( index );
      if ( ( vpit == vectorParamIndices.end() ) || twoStep || ( vectorParamIndices.size() > 1 ) ||
           ( vpit->second == INVALID_INDEX ) )
      {
        // it's not a vector parameter, or a two-step process, or there is at least one more vector parameter, or
        // the size argument of this vector parameter is not an argument
        // -> return the index of the selcted parameter
        returnParamIndex = index;
      }
    }
  }

  return returnParamIndex;
}

std::set<size_t> VulkanHppGenerator::determineSkippedParams( std::vector<ParamData> const &   params,
                                                             size_t                           initialSkipCount,
                                                             std::map<size_t, size_t> const & vectorParamIndices,
                                                             std::vector<size_t> const &      returnParamIndices,
                                                             bool                             singular ) const
{
  assert( initialSkipCount <= params.size() );
  std::set<size_t> skippedParams;
  for ( size_t i = 0; i < initialSkipCount; ++i )
  {
    skippedParams.insert( i );
  }

  for ( auto const & vpi : vectorParamIndices )
  {
    assert( !params[vpi.first].len.empty() );
    if ( ( ( std::find_if( returnParamIndices.begin(),
                           returnParamIndices.end(),
                           [&vpi]( size_t rpi ) { return vpi.first == rpi; } ) == returnParamIndices.end() ) &&
           isParam( params[vpi.first].len, params ) ) ||
         ( singular && params[vpi.second].type.isValue() ) )
    {
      skippedParams.insert( vpi.second );
    }
  }

  skippedParams.insert( returnParamIndices.begin(), returnParamIndices.end() );

  return skippedParams;
}

std::string VulkanHppGenerator::determineSubStruct( std::pair<std::string, StructureData> const & structure ) const
{
  for ( auto const & s : m_structures )
  {
    if ( ( s.first != structure.first ) && ( s.second.members.size() < structure.second.members.size() ) &&
         ( s.second.members[0].name != "sType" ) )
    {
      bool equal = true;
      for ( size_t i = 0; i < s.second.members.size() && equal; i++ )
      {
        equal = ( s.second.members[i].type == structure.second.members[i].type ) &&
                ( s.second.members[i].name == structure.second.members[i].name );
      }
      if ( equal )
      {
        return s.first;
      }
    }
  }
  return "";
}

std::vector<size_t> VulkanHppGenerator::determineConstPointerParamIndices( std::vector<ParamData> const & params ) const
{
  std::vector<size_t> constPointerParamIndices;

  for ( size_t i = 0; i < params.size(); i++ )
  {
    // very special handling for some types, which come in as non-const pointers, but are meant as const-pointers
    if ( params[i].type.isConstPointer() ||
         ( params[i].type.isNonConstPointer() &&
           ( specialPointerTypes.find( params[i].type.type ) != specialPointerTypes.end() ) ) )
    {
      constPointerParamIndices.push_back( i );
    }
  }
  return constPointerParamIndices;
}

std::vector<size_t>
  VulkanHppGenerator::determineNonConstPointerParamIndices( std::vector<ParamData> const & params ) const
{
  std::vector<size_t> nonConstPointerParamIndices;

  for ( size_t i = 0; i < params.size(); i++ )
  {
    // very special handling of parameters of some types, which always come as a non-const pointer but are not meant
    // to be a potential return value!
    if ( params[i].type.isNonConstPointer() &&
         ( specialPointerTypes.find( params[i].type.type ) == specialPointerTypes.end() ) )
    {
      nonConstPointerParamIndices.push_back( i );
    }
  }
  return nonConstPointerParamIndices;
}

std::map<size_t, size_t>
  VulkanHppGenerator::determineVectorParamIndicesNew( std::vector<ParamData> const & params ) const
{
  std::map<size_t, size_t> vectorParamIndices;

  // look for the parameters whose len equals the name of an other parameter
  for ( auto it = params.begin(); it != params.end(); ++it )
  {
    if ( !it->len.empty() )
    {
      auto findIt = std::find_if( params.begin(),
                                  it,
                                  [this, &it]( ParamData const & pd )
                                  { return ( pd.name == it->len ) || isLenByStructMember( it->len, pd ); } );

      if ( findIt < it )
      {
        // add this parameter as a vector parameter, using the len-name parameter as the second value
        vectorParamIndices.insert(
          std::make_pair( std::distance( params.begin(), it ), std::distance( params.begin(), findIt ) ) );
      }
    }
  }
  return vectorParamIndices;
}

void VulkanHppGenerator::appendIndexTypeTraits( std::string & str ) const
{
  auto indexType = m_enums.find( "VkIndexType" );
  assert( indexType != m_enums.end() );

  str += R"(
  template<typename T>
  struct IndexTypeValue
  {};
)";

  std::set<std::string> seenCppTypes;
  for ( auto const & value : indexType->second.values )
  {
    std::string cppType;
    if ( beginsWith( value.vkValue, "eUint8" ) )
    {
      cppType = "uint8_t";
    }
    else if ( beginsWith( value.vkValue, "eUint16" ) )
    {
      cppType = "uint16_t";
    }
    else if ( beginsWith( value.vkValue, "eUint32" ) )
    {
      cppType = "uint32_t";
    }
    else if ( beginsWith( value.vkValue, "eUint64" ) )
    {
      cppType = "uint64_t";  // No extension for this currently
    }
    else
    {
      assert( beginsWith( value.vkValue, "eNone" ) );
    }

    if ( !cppType.empty() )
    {
      if ( seenCppTypes.insert( cppType ).second )
      {
        // IndexType traits aren't necessarily invertible.
        // The Type -> Enum translation will only occur for the first prefixed enum value.
        // A hypothetical extension to this enum with a conflicting prefix will use the core spec value.
        str +=
          "\n"
          "  template <>\n"
          "  struct IndexTypeValue<" +
          cppType +
          ">\n"
          "  {\n"
          "    static VULKAN_HPP_CONST_OR_CONSTEXPR IndexType value = IndexType::" +
          value.vkValue +
          ";\n"
          "  };\n";
      }
      // Enum -> Type translations are always able to occur.
      str +=
        "\n"
        "  template <>\n"
        "  struct CppType<IndexType, IndexType::" +
        value.vkValue +
        ">\n"
        "  {\n"
        "    using Type = " +
        cppType +
        ";\n"
        "  };\n";
    }
  }
}

void VulkanHppGenerator::distributeSecondLevelCommands( std::set<std::string> const & specialFunctions )
{
  for ( auto & handle : m_handles )
  {
    if ( !handle.first.empty() )
    {
      for ( auto command = handle.second.commands.begin(); command != handle.second.commands.end(); )
      {
        bool foundCommand = false;
        if ( specialFunctions.find( *command ) == specialFunctions.end() )
        {
          auto commandIt = m_commands.find( *command );
          assert( commandIt != m_commands.end() );
          assert( commandIt->second.params.front().type.type == handle.first );
          if ( ( 1 < commandIt->second.params.size() ) && ( isHandleType( commandIt->second.params[1].type.type ) ) &&
               !commandIt->second.params[1].optional )
          {
            auto handleIt = m_handles.find( commandIt->second.params[1].type.type );
            assert( handleIt != m_handles.end() );
            assert( !handleIt->second.constructorIts.empty() );
            if ( ( *handleIt->second.constructorIts.begin() )->second.handle == handle.first )
            {
              assert( std::find_if( handleIt->second.constructorIts.begin(),
                                    handleIt->second.constructorIts.end(),
                                    [&handle]( auto const & constructorIt ) {
                                      return constructorIt->second.handle != handle.first;
                                    } ) == handleIt->second.constructorIts.end() );
              handleIt->second.secondLevelCommands.insert( *command );
              command      = handle.second.commands.erase( command );
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
}

std::string const & VulkanHppGenerator::getTypesafeCheck() const
{
  return m_typesafeCheck;
}

std::string const & VulkanHppGenerator::getVersion() const
{
  return m_version;
}

std::string const & VulkanHppGenerator::getVulkanLicenseHeader() const
{
  return m_vulkanLicenseHeader;
}

std::string VulkanHppGenerator::generateLenInitializer(
  std::vector<MemberData>::const_iterator                                        mit,
  std::map<std::vector<MemberData>::const_iterator,
           std::vector<std::vector<MemberData>::const_iterator>>::const_iterator litit,
  bool                                                                           mutualExclusiveLens ) const
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
    assert( ( arrayIt->len.front() == litit->first->name ) ||
            ( ( arrayIt->len.front() == R"(latexmath:[\textrm{codeSize} \over 4])" ) &&
              ( litit->first->name == "codeSize" ) ) );

    assert( beginsWith( arrayIt->name, "p" ) );
    std::string argumentName = startLowerCase( stripPrefix( arrayIt->name, "p" ) ) + "_";

    assert( mit->type.prefix.empty() && mit->type.postfix.empty() );
    initializer = argumentName + ".size()";
    if ( arrayIt->len.front() == R"(latexmath:[\textrm{codeSize} \over 4])" )
    {
      initializer += " * 4";
    }
    if ( arrayIt->type.type == "void" )
    {
      initializer += " * sizeof(T)";
    }
  }
  if ( mit->type.type != "size_t" )
  {
    initializer = "static_cast<" + mit->type.type + ">( " + initializer + " )";
  }
  return initializer;
}

std::pair<std::string, std::string> VulkanHppGenerator::generateProtection( std::string const & referencedIn ) const
{
  if ( !referencedIn.empty() && m_features.find( referencedIn ) == m_features.end() )
  {
    auto extensionIt = m_extensions.find( referencedIn );
    assert( extensionIt != m_extensions.end() );
    if ( !extensionIt->second.platform.empty() )
    {
      auto platformIt = m_platforms.find( extensionIt->second.platform );
      assert( platformIt != m_platforms.end() );
      std::string const & protect = platformIt->second.protect;
      if ( !protect.empty() )
      {
        return std::make_pair( "#if defined( " + protect + " )\n", "#endif /*" + protect + "*/\n" );
      }
    }
  }
  return std::make_pair( "", "" );
}

std::pair<std::string, std::string> VulkanHppGenerator::generateProtection( std::string const & type,
                                                                            bool                isAliased ) const
{
  if ( isAliased )
  {
    return std::make_pair( "", "" );
  }
  else
  {
    auto typeIt = m_types.find( type );
    assert( typeIt != m_types.end() );
    return generateProtection( typeIt->second.referencedIn );
  }
}

std::string
  VulkanHppGenerator::generateSizeCheck( std::vector<std::vector<MemberData>::const_iterator> const & arrayIts,
                                         std::string const &                                          structName,
                                         std::string const &                                          prefix,
                                         bool mutualExclusiveLens ) const
{
  std::string sizeCheck;
  if ( 1 < arrayIts.size() )
  {
    std::string assertionText, throwText;
    if ( mutualExclusiveLens )
    {
      // exactly one of the arrays has to be non-empty
      std::string sum;
      for ( size_t first = 0; first + 1 < arrayIts.size(); ++first )
      {
        sum += "!" + startLowerCase( stripPrefix( arrayIts[first]->name, "p" ) ) + "_.empty() + ";
      }
      sum += "!" + startLowerCase( stripPrefix( arrayIts.back()->name, "p" ) ) + "_.empty()";
      assertionText += prefix + "  VULKAN_HPP_ASSERT( ( " + sum + " ) <= 1);\n";
      throwText += prefix + "  if ( 1 < ( " + sum + " ) )\n";
      throwText += prefix + "  {\n";
      throwText += prefix + "    throw LogicError( VULKAN_HPP_NAMESPACE_STRING \"::" + structName + "::" + structName +
                   ": 1 < ( " + sum + " )\" );\n";
      throwText += prefix + "  }\n";
    }
    else
    {
      for ( size_t first = 0; first + 1 < arrayIts.size(); ++first )
      {
        assert( beginsWith( arrayIts[first]->name, "p" ) );
        std::string firstName = startLowerCase( stripPrefix( arrayIts[first]->name, "p" ) ) + "_";
        for ( auto second = first + 1; second < arrayIts.size(); ++second )
        {
          assert( beginsWith( arrayIts[second]->name, "p" ) );
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
          assertionText += prefix + "  VULKAN_HPP_ASSERT( " + assertionCheck + " );\n";
          throwText += prefix + "  if ( " + throwCheck + " )\n";
          throwText += prefix + "  {\n";
          throwText += prefix + "    throw LogicError( VULKAN_HPP_NAMESPACE_STRING \"::" + structName +
                       "::" + structName + ": " + throwCheck + "\" );\n";
          throwText += prefix + "  }\n";
        }
      }
    }
    sizeCheck += "\n#ifdef VULKAN_HPP_NO_EXCEPTIONS\n" + assertionText + "#else\n" + throwText +
                 "#endif /*VULKAN_HPP_NO_EXCEPTIONS*/\n" + prefix;
  }
  return sizeCheck;
}

std::string VulkanHppGenerator::getEnumPrefix( int line, std::string const & name, bool bitmask ) const
{
  std::string prefix;
  if ( name == "VkResult" )
  {
    prefix = "VK_";
  }
  else if ( bitmask )
  {
    // for a bitmask enum, start with "VK", cut off the trailing "FlagBits", and convert that name to upper case
    // end that with "Bit"
    size_t pos = name.find( "FlagBits" );
    check( pos != std::string::npos, line, "bitmask <" + name + "> does not contain <FlagBits>" );
    std::string shortenedName = name;
    shortenedName.erase( pos, strlen( "FlagBits" ) );
    std::string tag = findTag( m_tags, shortenedName );
    prefix          = toUpperCase( stripPostfix( shortenedName, tag ) ) + "_";
  }
  else
  {
    // for a non-bitmask enum, convert the name to upper case
    prefix = toUpperCase( name ) + "_";
  }
  return prefix;
}

std::string VulkanHppGenerator::getPlatform( std::string const & extension ) const
{
  auto extensionIt = m_extensions.find( extension );
  assert( extensionIt != m_extensions.end() );
  return extensionIt->second.platform;
}

std::pair<std::string, std::string> VulkanHppGenerator::getPoolTypeAndName( std::string const & type ) const
{
  auto structIt = m_structures.find( type );
  assert( structIt != m_structures.end() );
  auto memberIt = std::find_if( structIt->second.members.begin(),
                                structIt->second.members.end(),
                                []( MemberData const & md ) { return md.name.find( "Pool" ) != std::string::npos; } );
  assert( memberIt != structIt->second.members.end() );
  assert( std::find_if( std::next( memberIt ),
                        structIt->second.members.end(),
                        []( MemberData const & md )
                        { return md.name.find( "Pool" ) != std::string::npos; } ) == structIt->second.members.end() );
  return std::make_pair( memberIt->type.type, memberIt->name );
}

std::string VulkanHppGenerator::getVectorSize( std::vector<ParamData> const &   params,
                                               std::map<size_t, size_t> const & vectorParamIndices,
                                               size_t                           returnParamIndex ) const
{
  std::vector<std::string> lenParts = tokenize( params[returnParamIndex].len, "->" );
  switch ( lenParts.size() )
  {
    case 1:
      {
        std::string const & len    = lenParts[0];
        size_t              lenIdx = std::distance(
          params.begin(),
          std::find_if( params.begin(), params.end(), [&len]( ParamData const & pd ) { return pd.name == len; } ) );
        assert( lenIdx < params.size() );
        auto lenVectorParamIt =
          std::find_if( vectorParamIndices.begin(),
                        vectorParamIndices.end(),
                        [&lenIdx]( std::pair<size_t, size_t> const & vpi ) { return vpi.second == lenIdx; } );
        return ( lenVectorParamIt == vectorParamIndices.end() )
                 ? lenParts[0]
                 : ( startLowerCase( stripPrefix( params[lenVectorParamIt->first].name, "p" ) ) + ".size()" );
      }
      break;
    case 2: return startLowerCase( stripPrefix( lenParts[0], "p" ) ) + "." + lenParts[1]; break;
    default: assert( false ); return "";
  }
}

bool VulkanHppGenerator::hasParentHandle( std::string const & handle, std::string const & parent ) const
{
  std::string candidate = handle;
  while ( !candidate.empty() )
  {
    auto const & handleIt = m_handles.find( candidate );
    assert( handleIt != m_handles.end() );
    if ( handleIt->second.parent == parent )
    {
      return true;
    }
    else
    {
      candidate = handleIt->second.parent;
    }
  }
  return false;
}

bool VulkanHppGenerator::isHandleType( std::string const & type ) const
{
  if ( beginsWith( type, "Vk" ) )
  {
    auto it = m_handles.find( type );
    if ( it == m_handles.end() )
    {
      it = std::find_if( m_handles.begin(),
                         m_handles.end(),
                         [&type]( std::pair<std::string, HandleData> const & hd ) { return hd.second.alias == type; } );
    }
    return ( it != m_handles.end() );
  }
  return false;
}

bool VulkanHppGenerator::isLenByStructMember( std::string const & name, std::vector<ParamData> const & params ) const
{
  // check if name specifies a member of a struct
  std::vector<std::string> nameParts = tokenize( name, "->" );
  if ( nameParts.size() == 1 )
  {
    // older versions of vk.xml used the notation parameter::member
    nameParts = tokenize( name, "::" );
  }
  if ( nameParts.size() == 2 )
  {
    auto paramIt = std::find_if(
      params.begin(), params.end(), [&n = nameParts[0]]( ParamData const & pd ) { return pd.name == n; } );
    if ( paramIt != params.end() )
    {
#if !defined( NDEBUG )
      auto structureIt = m_structures.find( paramIt->type.type );
      assert( structureIt != m_structures.end() );
      assert( std::find_if( structureIt->second.members.begin(),
                            structureIt->second.members.end(),
                            [&n = nameParts[1]]( MemberData const & md )
                            { return md.name == n; } ) != structureIt->second.members.end() );
#endif
      return true;
    }
  }
  return false;
}

bool VulkanHppGenerator::isLenByStructMember( std::string const & name, ParamData const & param ) const
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
    auto structureIt = m_structures.find( param.type.type );
    assert( structureIt != m_structures.end() );
    assert( std::find_if( structureIt->second.members.begin(),
                          structureIt->second.members.end(),
                          [&n = nameParts[1]]( MemberData const & md )
                          { return md.name == n; } ) != structureIt->second.members.end() );
#endif
    return true;
  }
  return false;
}

bool VulkanHppGenerator::isMultiSuccessCodeConstructor(
  std::vector<std::map<std::string, CommandData>::const_iterator> const & constructorIts ) const
{
  bool ok = !constructorIts.empty();
  if ( ok )
  {
    auto constructorIt = constructorIts.begin();
    ok                 = ( 2 < ( *constructorIt )->second.successCodes.size() ) ||
         ( ( ( *constructorIt )->second.successCodes.size() == 2 ) &&
           ( ( *constructorIt )->second.successCodes[1] != "VK_INCOMPLETE" ) );
#if !defined( NDEBUG )
    for ( constructorIt = std::next( constructorIt ); constructorIt != constructorIts.end(); ++constructorIt )
    {
      assert( ok == ( 2 < ( *constructorIt )->second.successCodes.size() ) ||
              ( ( ( *constructorIt )->second.successCodes.size() == 2 ) &&
                ( ( *constructorIt )->second.successCodes[1] != "VK_INCOMPLETE" ) ) );
    }
#endif
  }
  return ok;
}

bool VulkanHppGenerator::isParam( std::string const & name, std::vector<ParamData> const & params ) const
{
  return std::find_if( params.begin(), params.end(), [&name]( ParamData const & pd ) { return pd.name == name; } ) !=
         params.end();
}

bool VulkanHppGenerator::isStructureChainAnchor( std::string const & type ) const
{
  if ( beginsWith( type, "Vk" ) )
  {
    auto it = m_structures.find( type );
    if ( it == m_structures.end() )
    {
      it = std::find_if( m_structures.begin(),
                         m_structures.end(),
                         [&type]( std::pair<std::string, StructureData> const & sd )
                         { return sd.second.aliases.find( type ) != sd.second.aliases.end(); } );
    }
    if ( it != m_structures.end() )
    {
      return m_extendedStructs.find( it->first ) != m_extendedStructs.end();
    }
  }
  return false;
}

std::pair<bool, std::map<size_t, std::vector<size_t>>>
  VulkanHppGenerator::needsVectorSizeCheck( std::map<size_t, size_t> const & vectorParamIndices ) const
{
  std::map<size_t, std::vector<size_t>> countToVectorMap;
  for ( auto const & vpi : vectorParamIndices )
  {
    if ( vpi.second != INVALID_INDEX )
    {
      countToVectorMap[vpi.second].push_back( vpi.first );
    }
  }
  return std::make_pair( std::find_if( countToVectorMap.begin(),
                                       countToVectorMap.end(),
                                       []( auto const & cvm )
                                       { return 1 < cvm.second.size(); } ) != countToVectorMap.end(),
                         countToVectorMap );
}

void VulkanHppGenerator::readBaseType( tinyxml2::XMLElement const *               element,
                                       std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "basetype" } } }, {} );

  NameData nameData;
  TypeInfo typeInfo;
  std::tie( nameData, typeInfo ) = readNameAndType( element );

  check( nameData.arraySizes.empty(), line, "name <" + nameData.name + "> with unsupported arraySizes" );
  check( typeInfo.type.empty() || ( typeInfo.prefix == "typedef" ),
         line,
         "unexpected type prefix <" + typeInfo.prefix + ">" );
  check( typeInfo.prefix.empty() || ( typeInfo.prefix == "typedef" ),
         line,
         "unexpected type prefix <" + typeInfo.prefix + ">" );
  check( typeInfo.postfix.empty(), line, "unexpected type postfix <" + typeInfo.postfix + ">" );

  if ( !typeInfo.type.empty() )
  {
    check( m_baseTypes.insert( std::make_pair( nameData.name, BaseTypeData( typeInfo.type, line ) ) ).second,
           line,
           "basetype <" + nameData.name + "> already specified" );
  }
  check( m_types.insert( std::make_pair( nameData.name, TypeCategory::BaseType ) ).second,
         line,
         "basetype <" + nameData.name + "> already specified as a type" );
}

void VulkanHppGenerator::readBitmask( tinyxml2::XMLElement const *               element,
                                      std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();

  auto aliasIt = attributes.find( "alias" );
  if ( aliasIt != attributes.end() )
  {
    readBitmaskAlias( element, attributes );
  }
  else
  {
    checkAttributes( line, attributes, { { "category", { "bitmask" } } }, { { "bitvalues", {} }, { "requires", {} } } );

    std::string bitvalues, requirements;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "bitvalues" )
      {
        bitvalues = attribute.second;
      }
      else if ( attribute.first == "requires" )
      {
        requirements = attribute.second;
      }
    }

    NameData nameData;
    TypeInfo typeInfo;
    std::tie( nameData, typeInfo ) = readNameAndType( element );

    check( beginsWith( nameData.name, "Vk" ), line, "name <" + nameData.name + "> does not begin with <Vk>" );
    check( nameData.arraySizes.empty(), line, "name <" + nameData.name + "> with unsupported arraySizes" );
    warn( ( typeInfo.type == "VkFlags" ) || ( typeInfo.type == "VkFlags64" ),
          line,
          "unexpected bitmask type <" + typeInfo.type + ">" );
    check( typeInfo.prefix == "typedef", line, "unexpected type prefix <" + typeInfo.prefix + ">" );
    check( typeInfo.postfix.empty(), line, "unexpected type postfix <" + typeInfo.postfix + ">" );
    check(
      bitvalues.empty() || requirements.empty(), line, "attributes <bitvalues> and <requires> are both specified" );
    check( ( typeInfo.type != "VkFlags64" ) || !bitvalues.empty(),
           line,
           "bitmask of type <VkFlags64> needs attribute bitvalues to be set" );

    if ( !bitvalues.empty() )
    {
      requirements = bitvalues;
    }
    m_bitmasks.insert( std::make_pair( nameData.name, BitmaskData( requirements, typeInfo.type, line ) ) );
    check( m_types.insert( std::make_pair( nameData.name, TypeCategory::Bitmask ) ).second,
           line,
           "bitmask <" + nameData.name + "> already specified as a type" );
  }
}

void VulkanHppGenerator::readBitmaskAlias( tinyxml2::XMLElement const *               element,
                                           std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "alias", {} }, { "category", { "bitmask" } }, { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

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

  auto bitmasksIt = m_bitmasks.find( alias );
  check( bitmasksIt != m_bitmasks.end(), line, "missing alias <" + alias + ">." );
  check( bitmasksIt->second.alias.empty(),
         line,
         "alias for bitmask <" + bitmasksIt->first + "> already specified as <" + bitmasksIt->second.alias + ">" );
  bitmasksIt->second.alias = name;
  check( m_types.insert( std::make_pair( name, TypeCategory::Bitmask ) ).second,
         line,
         "aliased bitmask <" + name + "> already specified as a type" );
}

void VulkanHppGenerator::readCommand( tinyxml2::XMLElement const * element )
{
  std::map<std::string, std::string> attributes = getAttributes( element );
  auto                               aliasIt    = attributes.find( "alias" );
  if ( aliasIt != attributes.end() )
  {
    readCommandAlias( element, attributes );
  }
  else
  {
    readCommand( element, attributes );
  }
}

void VulkanHppGenerator::readCommand( tinyxml2::XMLElement const *               element,
                                      std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line,
                   attributes,
                   {},
                   { { "cmdbufferlevel", { "primary", "secondary" } },
                     { "comment", {} },
                     { "errorcodes", {} },
                     { "pipeline", { "compute", "graphics", "transfer" } },
                     { "queues", { "compute", "decode", "encode", "graphics", "sparse_binding", "transfer" } },
                     { "renderpass", { "both", "inside", "outside" } },
                     { "successcodes", {} } } );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "param", false }, { "proto", true } }, { "implicitexternsyncparams" } );

  CommandData commandData( line );
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "errorcodes" )
    {
      commandData.errorCodes = tokenize( attribute.second, "," );
      // errorCodes are checked in checkCorrectness after complete reading
    }
    else if ( attribute.first == "successcodes" )
    {
      commandData.successCodes = tokenize( attribute.second, "," );
      // successCodes are checked in checkCorrectness after complete reading
    }
  }

  std::string name;
  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "param" )
    {
      commandData.params.push_back( readCommandParam( child, commandData.params ) );
    }
    else if ( value == "proto" )
    {
      std::tie( name, commandData.returnType ) = readCommandProto( child );
    }
  }
  assert( !name.empty() );
  check( ( commandData.returnType == "VkResult" ) || commandData.errorCodes.empty(),
         line,
         "command <" + name + "> does not return a VkResult but specifies errorcodes" );
  check( ( commandData.returnType == "VkResult" ) || commandData.successCodes.empty(),
         line,
         "command <" + name + "> does not return a VkResult but specifies successcodes" );

  registerDeleter( name, std::make_pair( name, commandData ) );
  addCommand( name, commandData );
}

void VulkanHppGenerator::readCommandAlias( tinyxml2::XMLElement const *               element,
                                           std::map<std::string, std::string> const & attributes )
{
  // for command aliases, create a copy of the aliased command
  int line = element->GetLineNum();
  checkAttributes( line,
                   attributes,
                   {},
                   {
                     { "alias", {} },
                     { "name", {} },
                   } );
  checkElements( line, getChildElements( element ), {} );

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
      check( beginsWith( name, "vk" ), line, "name <" + name + "> should begin with <vk>" );
    }
  }

  auto commandIt = m_commands.find( alias );
  check( commandIt != m_commands.end(), line, "missing command <" + alias + ">" );
  CommandData commandData = commandIt->second;
  commandData.alias       = alias;
  commandData.xmlLine     = line;
  addCommand( name, commandData );
}

VulkanHppGenerator::ParamData VulkanHppGenerator::readCommandParam( tinyxml2::XMLElement const *   element,
                                                                    std::vector<ParamData> const & params )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes(
    line,
    attributes,
    {},
    { { "externsync", {} }, { "len", {} }, { "noautovalidity", { "true" } }, { "optional", { "false", "true" } } } );

  ParamData paramData( line );
  for ( auto attribute : attributes )
  {
    if ( attribute.first == "len" )
    {
      paramData.len = attribute.second;
      check( ( paramData.len == "null-terminated" ) || isParam( paramData.len, params ) ||
               isLenByStructMember( paramData.len, params ),
             line,
             "command param len <" + paramData.len + "> is not recognized as a valid len value" );
    }
    else if ( attribute.first == "optional" )
    {
      paramData.optional = ( attribute.second == "true" );
    }
  }

  NameData nameData;
  std::tie( nameData, paramData.type ) = readNameAndType( element );

  check( m_types.find( paramData.type.type ) != m_types.end(), line, "unknown type <" + paramData.type.type + ">" );
  check( paramData.type.prefix.empty() || ( paramData.type.prefix == "const" ) ||
           ( paramData.type.prefix == "const struct" ) || ( paramData.type.prefix == "struct" ),
         line,
         "unexpected type prefix <" + paramData.type.prefix + ">" );
  check( paramData.type.postfix.empty() || ( paramData.type.postfix == "*" ) || ( paramData.type.postfix == "**" ) ||
           ( paramData.type.postfix == "* const *" ),
         line,
         "unexpected type postfix <" + paramData.type.postfix + ">" );
  check( std::find_if( params.begin(),
                       params.end(),
                       [&name = nameData.name]( ParamData const & pd ) { return pd.name == name; } ) == params.end(),
         line,
         "command param <" + nameData.name + "> already used" );
  paramData.name       = nameData.name;
  paramData.arraySizes = nameData.arraySizes;

  return paramData;
}

std::pair<std::string, std::string> VulkanHppGenerator::readCommandProto( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );

  NameData nameData;
  TypeInfo typeInfo;
  std::tie( nameData, typeInfo ) = readNameAndType( element );

  check( beginsWith( nameData.name, "vk" ), line, "name <" + nameData.name + "> does not begin with <vk>" );
  check( nameData.arraySizes.empty(), line, "name <" + nameData.name + "> with unsupported arraySizes" );
  check( m_types.find( typeInfo.type ) != m_types.end(), line, "unknown type <" + typeInfo.type + ">" );
  check( typeInfo.prefix.empty(), line, "unexpected type prefix <" + typeInfo.prefix + ">" );
  check( typeInfo.postfix.empty(), line, "unexpected type postfix <" + typeInfo.postfix + ">" );
  check(
    m_commands.find( nameData.name ) == m_commands.end(), line, "command <" + nameData.name + "> already specified" );

  return std::make_pair( nameData.name, typeInfo.type );
}

void VulkanHppGenerator::readCommands( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, { { "comment", {} } } );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "command", false } } );
  for ( auto child : children )
  {
    readCommand( child );
  }
}

std::string VulkanHppGenerator::readComment( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  checkElements( line, getChildElements( element ), {} );

  return element->GetText();
}

void VulkanHppGenerator::readDefine( tinyxml2::XMLElement const *               element,
                                     std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "define" } } }, { { "name", {} }, { "requires", {} } } );

  std::string name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "requires" )
    {
      check( m_defines.find( attribute.second ) != m_defines.end(),
             line,
             "using undefined requires <" + attribute.second + ">" );
    }
  }

  if ( !name.empty() )
  {
    check( !element->FirstChildElement(), line, "unknown formatting of type category=define name <" + name + ">" );
    check( element->LastChild() && element->LastChild()->ToText() && element->LastChild()->ToText()->Value(),
           line,
           "unknown formatting of type category=define named <" + name + ">" );

    // filter out the check for the different types of VK_DEFINE_NON_DISPATCHABLE_HANDLE
    if ( name == "VK_USE_64_BIT_PTR_DEFINES" )
    {
      m_typesafeCheck = "#if ( VK_USE_64_BIT_PTR_DEFINES == 1 )";
    }
    else if ( ( name == "VK_DEFINE_NON_DISPATCHABLE_HANDLE" ) && ( m_typesafeCheck.empty() ) )
    {
      std::string text  = element->LastChild()->ToText()->Value();
      size_t      start = text.find( "#if defined(__LP64__)" );
      check( start != std::string::npos, line, "unexpected text in type category=define named <" + name + ">" );
      size_t end = text.find_first_of( "\r\n", start + 1 );
      check( end != std::string::npos, line, "unexpected text in type category=define named <" + name + ">" );
      m_typesafeCheck = text.substr( start, end - start );
    }
  }
  else if ( element->GetText() )
  {
    std::string text = element->GetText();
    if ( ( text.find( "class" ) != std::string::npos ) || ( text.find( "struct" ) != std::string::npos ) )
    {
      // here are a couple of structs as defines, which really are types!
      tinyxml2::XMLElement const * child = element->FirstChildElement();
      check( child && ( strcmp( child->Value(), "name" ) == 0 ) && child->GetText(),
             line,
             "unexpected formatting of type category=define" );
      name = child->GetText();
      check( m_types.insert( std::make_pair( name, TypeCategory::Define ) ).second,
             line,
             "type <" + name + "> has already been speficied" );
    }
    else
    {
      tinyxml2::XMLElement const * child = element->FirstChildElement();
      check( child && !child->FirstAttribute() && ( strcmp( child->Value(), "name" ) == 0 ) && child->GetText(),
             line,
             "unknown formatting of type category define" );
      name = trim( child->GetText() );
      if ( name == "VK_HEADER_VERSION" )
      {
        m_version = trimEnd( element->LastChild()->ToText()->Value() );
      }
      // ignore all the other defines
      warn( !child->NextSiblingElement() ||
              ( child->NextSiblingElement() && !child->NextSiblingElement()->FirstAttribute() &&
                ( strcmp( child->NextSiblingElement()->Value(), "type" ) == 0 ) &&
                !child->NextSiblingElement()->NextSiblingElement() ),
            line,
            "unknown formatting of type category define" );
    }
  }

  assert( !name.empty() );
  check( m_defines.insert( name ).second, line, "define <" + name + "> has already been specified" );
}

void VulkanHppGenerator::readEnum( tinyxml2::XMLElement const * element,
                                   EnumData &                   enumData,
                                   bool                         bitmask,
                                   std::string const &          prefix,
                                   std::string const &          postfix )
{
  std::map<std::string, std::string> attributes = getAttributes( element );
  auto                               aliasIt    = attributes.find( "alias" );
  if ( aliasIt != attributes.end() )
  {
    readEnumAlias( element, attributes, enumData, bitmask, prefix, postfix );
  }
  else
  {
    readEnum( element, attributes, enumData, bitmask, prefix, postfix );
  }
}

void VulkanHppGenerator::readEnum( tinyxml2::XMLElement const *               element,
                                   std::map<std::string, std::string> const & attributes,
                                   EnumData &                                 enumData,
                                   bool                                       bitmask,
                                   std::string const &                        prefix,
                                   std::string const &                        postfix )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "name", {} } }, { { "bitpos", {} }, { "comment", {} }, { "value", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string alias, bitpos, name, value;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "bitpos" )
    {
      bitpos = attribute.second;
      check( !bitpos.empty(), line, "enum with empty bitpos" );
    }
    else if ( attribute.first == "name" )
    {
      name = attribute.second;
      check( !name.empty(), line, "enum with empty name" );
    }
    else if ( attribute.first == "value" )
    {
      value = attribute.second;
      check( !value.empty(), line, "enum with empty value" );
    }
  }
  assert( !name.empty() );

  std::string tag = findTag( m_tags, name, postfix );

  check( bitpos.empty() ^ value.empty(), line, "invalid set of attributes for enum <" + name + ">" );
  enumData.addEnumValue( line, name, bitmask, !bitpos.empty(), prefix, postfix, "", tag );
}

void VulkanHppGenerator::readEnumAlias( tinyxml2::XMLElement const *               element,
                                        std::map<std::string, std::string> const & attributes,
                                        EnumData &                                 enumData,
                                        bool                                       bitmask,
                                        std::string const &                        prefix,
                                        std::string const &                        postfix )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "alias", {} }, { "name", {} } }, { { "comment", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string alias, bitpos, name, value;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      alias = attribute.second;
      check( !alias.empty(), line, "enum with empty alias" );
    }
    else if ( attribute.first == "name" )
    {
      name = attribute.second;
      check( !name.empty(), line, "enum with empty name" );
    }
  }
  assert( !name.empty() );

  std::string tag = findTag( m_tags, name, postfix );
  enumData.addEnumAlias( line, name, alias, createEnumValueName( name, prefix, postfix, bitmask, tag ) );
}

void VulkanHppGenerator::readEnumConstant( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes(
    line, attributes, { { "name", {} } }, { { "alias", {} }, { "comment", {} }, { "type", {} }, { "value", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string alias, name, value;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      check( m_constants.find( attribute.second ) != m_constants.end(),
             line,
             "unknown enum constant alias <" + attribute.second + ">" );
      alias = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      check( m_constants.find( attribute.second ) == m_constants.end(),
             line,
             "already specified enum constant <" + attribute.second + ">" );
      name = attribute.second;
    }
    else if ( attribute.first == "value" )
    {
      check( !attribute.second.empty(), line, "value of enum constant is empty" );
      value = attribute.second;
    }
  }
  check( alias.empty() != value.empty(), line, "for enum <" + name + "> either alias or value need to be specified" );
  m_constants[name] = alias.empty() ? value : m_constants[alias];
}

void VulkanHppGenerator::readEnums( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line,
                   attributes,
                   { { "name", {} } },
                   { { "bitwidth", { "64" } }, { "comment", {} }, { "type", { "bitmask", "enum" } } } );
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

  if ( name == "API Constants" )
  {
    checkElements( line, children, { { "enum", false } }, {} );
    for ( auto const & child : children )
    {
      readEnumConstant( child );
    }
  }
  else
  {
    checkElements( line, children, {}, { "comment", "enum", "unused" } );
    check( !type.empty(), line, "enum without type" );

    // get the EnumData entry in enum map
    std::map<std::string, EnumData>::iterator it = m_enums.find( name );
    check( it != m_enums.end(), line, "enum <" + name + "> is not listed as enum in the types section" );
    check( it->second.values.empty(), line, "enum <" + name + "> already holds values" );

    // mark it as a bitmask, if it is one
    bool bitmask = ( type == "bitmask" );
    if ( bitmask )
    {
      auto bitmaskIt = std::find_if( m_bitmasks.begin(),
                                     m_bitmasks.end(),
                                     [&name]( auto const & bitmask ) { return bitmask.second.requirements == name; } );
      check( bitmaskIt != m_bitmasks.end(),
             line,
             "enum <" + name + "> is not listed as an requires or bitvalues for any bitmask in the types section" );
      check( ( bitwidth != "64" ) || ( bitmaskIt->second.type == "VkFlags64" ),
             line,
             "enum <" + name + "> is marked with bitwidth <64> but corresponding bitmask <" + bitmaskIt->first +
               "> is not of type <VkFlags64>" );
    }
    it->second.isBitmask = bitmask;

    std::string prefix  = getEnumPrefix( line, name, bitmask );
    std::string postfix = getEnumPostfix( name, m_tags, prefix );
    // read the names of the enum values
    for ( auto child : children )
    {
      std::string value = child->Value();
      if ( value == "comment" )
      {
        readComment( child );
      }
      else if ( value == "enum" )
      {
        readEnum( child, it->second, bitmask, prefix, postfix );
      }
    }
  }
}

void VulkanHppGenerator::readExtension( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line,
                   attributes,
                   { { "name", {} }, { "number", {} }, { "supported", { "disabled", "enabled", "vulkan" } } },
                   { { "author", {} },
                     { "comment", {} },
                     { "contact", {} },
                     { "deprecatedby", {} },
                     { "obsoletedby", {} },
                     { "platform", {} },
                     { "promotedto", {} },
                     { "provisional", { "true" } },
                     { "requires", {} },
                     { "requiresCore", {} },
                     { "sortorder", { "1" } },
                     { "specialuse", { "cadsupport", "d3demulation", "debugging", "devtools", "glemulation" } },
                     { "type", { "device", "instance" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "require" } );

  std::string              deprecatedBy, name, number, obsoletedBy, platform, promotedTo, supported;
  std::vector<std::string> requirements;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "deprecatedby" )
    {
      deprecatedBy = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "number" )
    {
      number = attribute.second;
    }
    else if ( attribute.first == "obsoletedby" )
    {
      obsoletedBy = attribute.second;
    }
    else if ( attribute.first == "platform" )
    {
      platform = attribute.second;
      check( m_platforms.find( platform ) != m_platforms.end(), line, "unknown platform <" + platform + ">" );
    }
    else if ( attribute.first == "promotedto" )
    {
      promotedTo = attribute.second;
    }
    else if ( attribute.first == "provisional" )
    {
      if ( platform.empty() )
      {
        // for now, having the attribute provisional="true" implies attribute platform="provisional" to get stuff
        // protected by VK_ENABLE_BETA_EXTENSIONS
        platform = "provisional";
      }
      check(
        platform == "provisional",
        line,
        "while attribute <provisional> is set to \"true\", attribute <platform> is not set to \"provisional\" but to \"" +
          platform + "\"" );
    }
    else if ( attribute.first == "requires" )
    {
      requirements = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "requiresCore" )
    {
      std::string const & requiresCore = attribute.second;
      check( std::find_if( m_features.begin(),
                           m_features.end(),
                           [&requiresCore]( std::pair<std::string, FeatureData> const & feature )
                           { return feature.second.number == requiresCore; } ) != m_features.end(),
             line,
             "unknown feature number <" + attribute.second + ">" );
    }
    else if ( attribute.first == "supported" )
    {
      supported = attribute.second;
    }
  }

  if ( supported == "disabled" )
  {
    // kick out all the disabled stuff we've read before !!
    for ( auto const & child : children )
    {
      readExtensionDisabledRequire( child );
    }
  }
  else
  {
    auto pitb = m_extensions.insert(
      std::make_pair( name, ExtensionData( line, deprecatedBy, number, obsoletedBy, platform, promotedTo ) ) );
    check( pitb.second, line, "already encountered extension <" + name + ">" );
    for ( auto const & r : requirements )
    {
      check( pitb.first->second.requirements.insert( std::make_pair( r, line ) ).second,
             line,
             "required extension <" + r + "> already listed" );
    }

    // extract the tag from the name, which is supposed to look like VK_<tag>_<other>
    size_t tagStart = name.find( '_' );
    check( tagStart != std::string::npos, line, "name <" + name + "> is missing an underscore '_'" );
    size_t tagEnd = name.find( '_', tagStart + 1 );
    check( tagEnd != std::string::npos, line, "name <" + name + "> is missing an underscore '_'" );
    std::string tag = name.substr( tagStart + 1, tagEnd - tagStart - 1 );
    check( m_tags.find( tag ) != m_tags.end(), line, "name <" + name + "> is using an unknown tag <" + tag + ">" );

    for ( auto child : children )
    {
      readExtensionRequire( child, pitb.first, tag );
    }
  }
}

void VulkanHppGenerator::readExtensionDisabledCommand( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name = attributes.find( "name" )->second;

  // first unlink the command from its class
  auto commandIt = m_commands.find( name );
  if ( commandIt != m_commands.end() )
  {
    auto handleIt = m_handles.find( commandIt->second.handle );
    check( handleIt != m_handles.end(), line, "cannot find handle corresponding to command <" + name + ">" );
    handleIt->second.commands.erase( commandIt->first );

    // then erase the command from the command list
    m_commands.erase( commandIt );
  }
}

void VulkanHppGenerator::readExtensionDisabledRequire( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "enum", false } }, { "command", "comment", "type" } );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "command" )
    {
      readExtensionDisabledCommand( child );
    }
    else if ( value == "comment" )
    {
      readComment( child );
    }
    else if ( value == "enum" )
    {
      // just skip enums of disabled extensions
    }
    else
    {
      assert( value == "type" );
      readExtensionDisabledType( child );
    }
  }
}

void VulkanHppGenerator::readExtensionDisabledType( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name = attributes.find( "name" )->second;

  auto typeIt = m_types.find( name );
  if ( typeIt != m_types.end() )
  {
    switch ( typeIt->second.category )
    {
      case TypeCategory::Bitmask:
        {
          auto bitmasksIt = m_bitmasks.find( name );
          check( bitmasksIt != m_bitmasks.end(), line, "trying to remove unknown bitmask <" + name + ">" );
          check( bitmasksIt->second.alias.empty(),
                 line,
                 "trying to remove disabled bitmask <" + name + "> which has alias <" + bitmasksIt->second.alias +
                   ">" );
          m_bitmasks.erase( bitmasksIt );
        }
        break;
      case TypeCategory::Enum:
        {
          auto enumIt = m_enums.find( name );
          check( enumIt != m_enums.end(), line, "trying to remove unknown enum <" + name + ">" );
          check( enumIt->second.alias.empty(),
                 line,
                 "trying to remove disabled enum <" + name + "> which has alias <" + enumIt->second.alias + ">" );
          m_enums.erase( enumIt );
        }
        break;
      case TypeCategory::Struct:
        {
          auto structIt = m_structures.find( name );
          check( structIt != m_structures.end(), line, "trying to remove unknown struct <" + name + ">" );
          check( structIt->second.aliases.empty(),
                 line,
                 "trying to remove disabled structure <" + name + "> which has " +
                   std::to_string( structIt->second.aliases.size() ) + "aliases" );
          m_structures.erase( structIt );
        }
        break;
      default:
        check( false,
               line,
               "trying to remove <" + name + "> of unhandled type <" + toString( typeIt->second.category ) + ">" );
        break;
    }
  }
}

void VulkanHppGenerator::readExtensionRequire( tinyxml2::XMLElement const *                   element,
                                               std::map<std::string, ExtensionData>::iterator extensionIt,
                                               std::string const &                            tag )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, { { "extension", {} }, { "feature", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "command", "comment", "enum", "type" } );

  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "extension" )
    {
      check( extensionIt->second.requirements.insert( std::make_pair( attribute.second, line ) ).second,
             line,
             "required extension <" + attribute.second + "> already listed" );
    }
    else
    {
      assert( attribute.first == "feature" );
      check(
        m_features.find( attribute.second ) != m_features.end(), line, "unknown feature <" + attribute.second + ">" );
    }
  }

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "command" )
    {
      readExtensionRequireCommand( child, extensionIt );
    }
    else if ( value == "comment" )
    {
      readComment( child );
    }
    else if ( value == "enum" )
    {
      readRequireEnum( child, extensionIt->first, tag );
    }
    else if ( value == "type" )
    {
      readExtensionRequireType( child, extensionIt );
    }
  }
}

void VulkanHppGenerator::readExtensionRequireCommand( tinyxml2::XMLElement const *                   element,
                                                      std::map<std::string, ExtensionData>::iterator extensionIt )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
  }
  assert( !name.empty() );

  // mark this command be part of this extension
  auto commandIt = m_commands.find( name );
  assert( commandIt != m_commands.end() );
  if ( commandIt->second.referencedIn.empty() )
  {
    commandIt->second.referencedIn = extensionIt->first;
  }
  else
  {
    check( getPlatform( commandIt->second.referencedIn ) == getPlatform( extensionIt->first ),
           line,
           "command <" + name + "> is referenced in extensions <" + commandIt->second.referencedIn + "> and <" +
             extensionIt->first + "> and thus protected by different platforms <" +
             getPlatform( commandIt->second.referencedIn ) + "> and <" + getPlatform( extensionIt->first ) + ">!" );
  }
  if ( std::find( extensionIt->second.commands.begin(), extensionIt->second.commands.end(), name ) ==
       extensionIt->second.commands.end() )
  {
    // some commands are listed more than once under different additional requirements
    extensionIt->second.commands.push_back( name );
  }
}

void VulkanHppGenerator::readExtensionRequireType( tinyxml2::XMLElement const *                   element,
                                                   std::map<std::string, ExtensionData>::iterator extensionIt )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
  }
  assert( !name.empty() );

  auto typeIt = m_types.find( name );
  check( typeIt != m_types.end(), line, "failed to find required type <" + name + ">" );
  if ( typeIt->second.referencedIn.empty() )
  {
    typeIt->second.referencedIn = extensionIt->first;
    assert( std::find( extensionIt->second.types.begin(), extensionIt->second.types.end(), name ) ==
            extensionIt->second.types.end() );
    extensionIt->second.types.push_back( name );
  }
  else
  {
    check( getPlatform( typeIt->second.referencedIn ) == getPlatform( extensionIt->first ),
           line,
           "type <" + name + "> is referenced in extensions <" + typeIt->second.referencedIn + "> and <" +
             extensionIt->first + "> and thus protected by different platforms <" +
             getPlatform( typeIt->second.referencedIn ) + "> and <" + getPlatform( extensionIt->first ) + ">!" );
  }
}

void VulkanHppGenerator::readExtensions( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "extension", false } } );

  for ( auto child : children )
  {
    readExtension( child );
  }
}

void VulkanHppGenerator::readFeature( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes(
    line, attributes, { { "api", { "vulkan" } }, { "comment", {} }, { "name", {} }, { "number", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "require", false } } );

  std::string name, number, modifiedNumber;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "number" )
    {
      number         = attribute.second;
      modifiedNumber = number;
      std::replace( modifiedNumber.begin(), modifiedNumber.end(), '.', '_' );
    }
  }
  assert( !name.empty() && !number.empty() );
  check( name == "VK_VERSION_" + modifiedNumber, line, "unexpected formatting of name <" + name + ">" );
  check( m_features.find( name ) == m_features.end(), line, "already specified feature <" + name + ">" );

  auto featureIt = m_features.insert( std::make_pair( name, number ) ).first;
  for ( auto child : children )
  {
    readFeatureRequire( child, featureIt );
  }
}

void VulkanHppGenerator::readFeatureRequire( tinyxml2::XMLElement const *                 element,
                                             std::map<std::string, FeatureData>::iterator featureIt )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, { { "comment", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "command", "comment", "enum", "type" } );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "command" )
    {
      readFeatureRequireCommand( child, featureIt );
    }
    else if ( value == "comment" )
    {
      readComment( child );
    }
    else if ( value == "enum" )
    {
      readRequireEnum( child, "", "" );
    }
    else if ( value == "type" )
    {
      readFeatureRequireType( child, featureIt );
    }
  }
}

void VulkanHppGenerator::readFeatureRequireCommand( tinyxml2::XMLElement const *                 element,
                                                    std::map<std::string, FeatureData>::iterator featureIt )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, { { "name", {} } } );
  std::string name      = attributes.find( "name" )->second;
  auto        commandIt = m_commands.find( name );
  check( commandIt != m_commands.end(), line, "feature requires unknown command <" + name + ">" );
  check( commandIt->second.referencedIn.empty(),
         line,
         "command <" + name + "> already listed with feature <" + commandIt->second.referencedIn + ">" );
  commandIt->second.referencedIn = featureIt->first;
  assert( std::find( featureIt->second.commands.begin(), featureIt->second.commands.end(), name ) ==
          featureIt->second.commands.end() );
  featureIt->second.commands.push_back( name );
}

void VulkanHppGenerator::readFeatureRequireType( tinyxml2::XMLElement const *                 element,
                                                 std::map<std::string, FeatureData>::iterator featureIt )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, { { "comment", {} }, { "name", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string name          = attributes.find( "name" )->second;
  auto        featureTypeIt = std::find_if( featureIt->second.types.begin(),
                                     featureIt->second.types.end(),
                                     [&name]( std::pair<std::string, int> const & typeLine )
                                     { return ( typeLine.first == name ) && ( typeLine.second != 0 ); } );
  check( featureTypeIt == featureIt->second.types.end(),
         line,
         "type <" + name + "> already listed for this feature on line " +
           ( ( featureTypeIt == featureIt->second.types.end() ) ? "" : std::to_string( featureTypeIt->second ) ) +
           " !" );

  // some types are in fact includes (like vk_platform) or defines (like VK_API_VERSION)
  if ( ( m_defines.find( name ) == m_defines.end() ) && ( m_includes.find( name ) == m_includes.end() ) )
  {
    auto typeIt = m_types.find( name );
    check( typeIt != m_types.end(), line, "feature requires unknown type <" + name + ">" );
    check( typeIt->second.referencedIn.empty() || ( typeIt->second.referencedIn == featureIt->first ),
           line,
           "type <" + name + "> already listed on feature <" + typeIt->second.referencedIn + ">" );
    typeIt->second.referencedIn = featureIt->first;

    // add missing FlagBits here!
    if ( ( name != "VkFlags" ) && endsWith( name, "Flags" ) )
    {
      assert( !featureIt->second.types.empty() );
      std::string flagBits = name.substr( 0, name.length() - 5 ) + "FlagBits";
      if ( featureIt->second.types.back().first != flagBits )
      {
        assert( std::find_if( featureIt->second.types.begin(),
                              featureIt->second.types.end(),
                              [&flagBits]( std::pair<std::string, int> const & typeLine )
                              { return ( typeLine.first == flagBits ); } ) == featureIt->second.types.end() );
        featureIt->second.types.push_back( std::make_pair( flagBits, 0 ) );
      }
    }

    // filter out FlagBits that are listed after their Flags!
    // (and therefore have been added right before that Flags, above)
    bool skipIt = false;
    if ( endsWith( name, "FlagBits" ) )
    {
      assert( !featureIt->second.types.empty() );
      std::string flags = name.substr( 0, name.length() - 4 ) + "s";
      if ( featureIt->second.types.back().first == flags )
      {
        skipIt          = true;
        auto flagBitsIt = std::find_if( featureIt->second.types.begin(),
                                        featureIt->second.types.end(),
                                        [&name]( std::pair<std::string, int> const & typeLine )
                                        { return ( typeLine.first == name ); } );
        assert( flagBitsIt != featureIt->second.types.end() );
        assert( flagBitsIt->second == 0 );
        flagBitsIt->second = line;
      }
      else
      {
        assert( std::find_if( featureIt->second.types.begin(),
                              featureIt->second.types.end(),
                              [&flags]( std::pair<std::string, int> const & typeLine )
                              { return ( typeLine.first == flags ); } ) == featureIt->second.types.end() );
      }
    }
    if ( !skipIt )
    {
      featureIt->second.types.push_back( std::make_pair( name, line ) );
    }
  }
}

void VulkanHppGenerator::readFuncpointer( tinyxml2::XMLElement const *               element,
                                          std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "funcpointer" } } }, { { "requires", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "name", true } }, { "type" } );

  std::string requirements;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "requires" )
    {
      requirements = attribute.second;
    }
  }

  for ( auto const & child : children )
  {
    std::string value     = child->Value();
    int         childLine = child->GetLineNum();
    if ( value == "name" )
    {
      std::string name = child->GetText();
      check( !name.empty(), childLine, "funcpointer with empty name" );
      check( m_funcPointers.insert( std::make_pair( name, FuncPointerData( requirements, line ) ) ).second,
             childLine,
             "funcpointer <" + name + "> already specified" );
      check( m_types.insert( std::make_pair( name, TypeCategory::FuncPointer ) ).second,
             childLine,
             "funcpointer <" + name + "> already specified as a type" );
    }
    else if ( value == "type" )
    {
      std::string type = child->GetText();
      check( !type.empty(), childLine, "funcpointer argument with empty type" );
      check( ( m_types.find( type ) != m_types.end() ) || ( type == requirements ),
             childLine,
             "funcpointer argument of unknown type <" + type + ">" );
    }
  }
}

void VulkanHppGenerator::readHandle( tinyxml2::XMLElement const *               element,
                                     std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();

  auto aliasIt = attributes.find( "alias" );
  if ( aliasIt != attributes.end() )
  {
    checkAttributes( line, attributes, { { "alias", {} }, { "category", { "handle" } }, { "name", {} } }, {} );
    checkElements( line, getChildElements( element ), {} );

    auto handlesIt = m_handles.find( aliasIt->second );
    check( handlesIt != m_handles.end(), line, "using unspecified alias <" + aliasIt->second + ">." );
    check( handlesIt->second.alias.empty(),
           line,
           "handle <" + handlesIt->first + "> already has an alias <" + handlesIt->second.alias + ">" );
    handlesIt->second.alias = attributes.find( "name" )->second;
    check( m_types.insert( std::make_pair( handlesIt->second.alias, TypeCategory::Handle ) ).second,
           line,
           "handle alias <" + handlesIt->second.alias + "> already specified as a type" );
  }
  else
  {
    checkAttributes( line, attributes, { { "category", { "handle" } } }, { { "objtypeenum", {} }, { "parent", {} } } );

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

    NameData nameData;
    TypeInfo typeInfo;
    std::tie( nameData, typeInfo ) = readNameAndType( element );

    check( beginsWith( nameData.name, "Vk" ), line, "name <" + nameData.name + "> does not begin with <Vk>" );
    check( nameData.arraySizes.empty(), line, "name <" + nameData.name + "> with unsupported arraySizes" );
    check( ( typeInfo.type == "VK_DEFINE_HANDLE" ) || ( typeInfo.type == "VK_DEFINE_NON_DISPATCHABLE_HANDLE" ),
           line,
           "handle with invalid type <" + typeInfo.type + ">" );
    check( typeInfo.prefix.empty(), line, "unexpected type prefix <" + typeInfo.prefix + ">" );
    check( typeInfo.postfix == "(", line, "unexpected type postfix <" + typeInfo.postfix + ">" );
    check( !objTypeEnum.empty(), line, "handle <" + nameData.name + "> does not specify attribute \"objtypeenum\"" );

    check(
      parent.find( ',' ) == std::string::npos, line, "mulitple parents specified for handle <" + nameData.name + ">" );
    check( m_handles.insert( std::make_pair( nameData.name, HandleData( parent, objTypeEnum, line ) ) ).second,
           line,
           "handle <" + nameData.name + "> already specified" );
    check( m_types.insert( std::make_pair( nameData.name, TypeCategory::Handle ) ).second,
           line,
           "handle <" + nameData.name + "> already specified as a type" );
  }
}

std::pair<VulkanHppGenerator::NameData, VulkanHppGenerator::TypeInfo>
  VulkanHppGenerator::readNameAndType( tinyxml2::XMLElement const * element )
{
  int                                       line     = element->GetLineNum();
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "name", true } }, { { "type" } } );

  NameData nameData;
  TypeInfo typeInfo;
  for ( auto child : children )
  {
    line = child->GetLineNum();
    checkAttributes( line, getAttributes( child ), {}, {} );
    checkElements( line, getChildElements( child ), {} );

    std::string value = child->Value();
    if ( value == "name" )
    {
      nameData.name = child->GetText();
      std::string bitCount;
      std::tie( nameData.arraySizes, bitCount ) = readModifiers( child->NextSibling() );
      check( bitCount.empty(), line, "name <" + nameData.name + "> with unsupported bitCount <" + bitCount + ">" );
    }
    else if ( value == "type" )
    {
      typeInfo.prefix  = readTypePrefix( child->PreviousSibling() );
      typeInfo.type    = child->GetText();
      typeInfo.postfix = readTypePostfix( child->NextSibling() );
    }
  }
  return std::make_pair( nameData, typeInfo );
}

void VulkanHppGenerator::readPlatform( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "comment", {} }, { "name", {} }, { "protect", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name, protect;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
      check( !name.empty(), line, "attribute <name> is empty" );
    }
    else if ( attribute.first == "protect" )
    {
      protect = attribute.second;
      check( !protect.empty(), line, "attribute <protect> is empty" );
    }
  }
  assert( !name.empty() && !protect.empty() );

  check( std::find_if( m_platforms.begin(),
                       m_platforms.end(),
                       [&protect]( std::pair<std::string, PlatformData> const & p )
                       { return p.second.protect == protect; } ) == m_platforms.end(),
         line,
         "platform protect <" + protect + "> already specified" );
  check( m_platforms.insert( std::make_pair( name, PlatformData( protect ) ) ).second,
         line,
         "platform name <" + name + "> already specified" );
}

void VulkanHppGenerator::readPlatforms( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "platform", false } } );

  for ( auto child : children )
  {
    readPlatform( child );
  }
}

void VulkanHppGenerator::readRegistry( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );

  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line,
                 children,
                 { { "commands", true },
                   { "comment", false },
                   { "enums", false },
                   { "extensions", true },
                   { "feature", false },
                   { "platforms", true },
                   { "spirvcapabilities", true },
                   { "spirvextensions", true },
                   { "tags", true },
                   { "types", true } } );
  for ( auto child : children )
  {
    const std::string value = child->Value();
    if ( value == "commands" )
    {
      readCommands( child );
    }
    else if ( value == "comment" )
    {
      std::string comment = readComment( child );
      if ( comment.find( "\nCopyright" ) == 0 )
      {
        setVulkanLicenseHeader( child->GetLineNum(), comment );
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
    else if ( value == "feature" )
    {
      readFeature( child );
    }
    else if ( value == "platforms" )
    {
      readPlatforms( child );
    }
    else if ( value == "spirvcapabilities" )
    {
      readSPIRVCapabilities( child );
    }
    else if ( value == "spirvextensions" )
    {
      readSPIRVExtensions( child );
    }
    else if ( value == "tags" )
    {
      readTags( child );
    }
    else if ( value == "types" )
    {
      readTypes( child );
    }
  }
}

void VulkanHppGenerator::readRequireEnum( tinyxml2::XMLElement const * element,
                                          std::string const &          extension,
                                          std::string const &          tag )
{
  std::map<std::string, std::string> attributes = getAttributes( element );
  if ( attributes.find( "alias" ) != attributes.end() )
  {
    readRequireEnumAlias( element, attributes, tag );
  }
  else
  {
    readRequireEnum( element, attributes, extension, tag );
  }
}

void VulkanHppGenerator::readRequireEnum( tinyxml2::XMLElement const *               element,
                                          std::map<std::string, std::string> const & attributes,
                                          std::string const &                        extension,
                                          std::string const &                        tag )
{
  int line = element->GetLineNum();
  checkAttributes( line,
                   attributes,
                   { { "name", {} } },
                   { { "bitpos", {} },
                     { "comment", {} },
                     { "extends", {} },
                     { "dir", { "-" } },
                     { "extnumber", {} },
                     { "offset", {} },
                     { "protect", { "VK_ENABLE_BETA_EXTENSIONS" } },
                     { "value", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string bitpos, name, extends, extnumber, offset, value;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "bitpos" )
    {
      bitpos = attribute.second;
    }
    else if ( attribute.first == "extends" )
    {
      extends = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
    else if ( attribute.first == "offset" )
    {
      offset = attribute.second;
    }
    else if ( attribute.first == "protect" )
    {
      // for now, attribute "protect" is, if set at all, set to "VK_ENABLE_BETA_EXTENSIONS"
      // and it's redundant with the platform set for this extension!
      auto extIt = m_extensions.find( extension );
      assert( extIt != m_extensions.end() );
      check(
        extIt->second.platform == "provisional",
        line,
        "attribute <protect> is \"VK_ENABLE_BETA_EXTENSIONS\", but the extensions platform is not \"provisional\" but \"" +
          extIt->second.platform + "\"" );
    }
    else if ( attribute.first == "value" )
    {
      value = attribute.second;
    }
  }

  if ( !extends.empty() )
  {
    auto enumIt = m_enums.find( extends );
    check( enumIt != m_enums.end(), line, "feature extends unknown enum <" + extends + ">" );

    std::string prefix  = getEnumPrefix( element->GetLineNum(), enumIt->first, enumIt->second.isBitmask );
    std::string postfix = getEnumPostfix( enumIt->first, m_tags, prefix );

    // add this enum name to the list of values
    check( bitpos.empty() + offset.empty() + value.empty() == 2,
           line,
           "exactly one out of bitpos = <" + bitpos + ">, offset = <" + offset + ">, and value = <" + value +
             "> are supposed to be empty" );
    enumIt->second.addEnumValue(
      element->GetLineNum(), name, enumIt->second.isBitmask, !bitpos.empty(), prefix, postfix, extension, tag );
  }
  else if ( value.empty() )
  {
    check( m_constants.find( name ) != m_constants.end(), line, "unknown required enum <" + name + ">" );
  }
}

void VulkanHppGenerator::readRequireEnumAlias( tinyxml2::XMLElement const *               element,
                                               std::map<std::string, std::string> const & attributes,
                                               std::string const &                        tag )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "alias", {} }, { "extends", {} }, { "name", {} } }, { { "comment", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string alias, bitpos, name, extends, extnumber, offset, value;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      alias = attribute.second;
    }
    else if ( attribute.first == "extends" )
    {
      extends = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
  }

  auto enumIt = m_enums.find( extends );
  check( enumIt != m_enums.end(), line, "feature extends unknown enum <" + extends + ">" );

  std::string prefix  = getEnumPrefix( element->GetLineNum(), enumIt->first, enumIt->second.isBitmask );
  std::string postfix = getEnumPostfix( enumIt->first, m_tags, prefix );

  // add this enum name to the list of aliases
  std::string valueName = createEnumValueName( name, prefix, postfix, enumIt->second.isBitmask, tag );
  if ( !enumIt->second.alias.empty() )
  {
    prefix  = getEnumPrefix( element->GetLineNum(), enumIt->second.alias, enumIt->second.isBitmask );
    postfix = getEnumPostfix( enumIt->second.alias, m_tags, prefix );
    if ( endsWith( name, postfix ) )
    {
      valueName = createEnumValueName( name, prefix, postfix, enumIt->second.isBitmask, tag );
    }
  }

  enumIt->second.addEnumAlias( line, name, alias, valueName );
}

void VulkanHppGenerator::readRequires( tinyxml2::XMLElement const *               element,
                                       std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "name", {} }, { "requires", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  for ( auto attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      check( m_types.insert( std::make_pair( attribute.second, TypeCategory::Requires ) ).second,
             line,
             "type named <" + attribute.second + "> already specified" );
    }
    else
    {
      assert( attribute.first == "requires" );
      check( m_includes.find( attribute.second ) != m_includes.end(),
             line,
             "type requires unknown include <" + attribute.second + ">" );
    }
  }
}

void VulkanHppGenerator::readSPIRVCapability( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "enable" } );

  for ( auto child : children )
  {
    std::string value = child->Value();
    assert( value == "enable" );
    readSPIRVCapabilityEnable( child );
  }
}

void VulkanHppGenerator::readSPIRVCapabilityEnable( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkElements( line, getChildElements( element ), {}, {} );

  if ( attributes.find( "extension" ) != attributes.end() )
  {
    readSPIRVCapabilityEnableExtension( line, attributes );
  }
  else if ( attributes.find( "property" ) != attributes.end() )
  {
    readSPIRVCapabilityEnableProperty( line, attributes );
  }
  else if ( attributes.find( "struct" ) != attributes.end() )
  {
    readSPIRVCapabilityEnableStruct( line, attributes );
  }
  else if ( attributes.find( "version" ) != attributes.end() )
  {
    readSPIRVCapabilityEnableVersion( line, attributes );
  }
  else
  {
    check( false, line, "unknown set of attributes specified for SPIR-V capability" );
  }
}

void VulkanHppGenerator::readSPIRVCapabilityEnableExtension( int                                        xmlLine,
                                                             std::map<std::string, std::string> const & attributes )
{
  checkAttributes( xmlLine, attributes, { { "extension", {} } }, {} );

  check( attributes.size() == 1,
         xmlLine,
         "unexpected attributes in addition to <extension> specified for SPIR-V capability" );
  for ( auto const & attribute : attributes )
  {
    assert( attribute.first == "extension" );
    check( m_extensions.find( attribute.second ) != m_extensions.end(),
           xmlLine,
           "unknown extension <" + attribute.second + "> specified for SPIR-V capability" );
  }
}

void VulkanHppGenerator::readSPIRVCapabilityEnableProperty( int                                        xmlLine,
                                                            std::map<std::string, std::string> const & attributes )
{
  checkAttributes(
    xmlLine, attributes, { { "member", {} }, { "property", {} }, { "requires", {} }, { "value", {} } }, {} );

  std::string member, property, value;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "member" )
    {
      member = attribute.second;
    }
    else if ( attribute.first == "property" )
    {
      property = attribute.second;
    }
    if ( attribute.first == "requires" )
    {
      std::vector<std::string>
      requires = tokenize( attribute.second, "," );
      for ( auto const & r : requires )
      {
        check( ( m_features.find( r ) != m_features.end() ) || ( m_extensions.find( r ) != m_extensions.end() ),
               xmlLine,
               "unknown requires <" + r + "> specified for SPIR-V capability" );
      }
    }
    else if ( attribute.first == "value" )
    {
      value = attribute.second;
    }
  }
  assert( !member.empty() && !property.empty() && !value.empty() );

  auto propertyIt = m_structures.find( property );
  check(
    propertyIt != m_structures.end(), xmlLine, "unknown property <" + property + "> specified for SPIR-V capability" );
  auto memberIt = std::find_if( propertyIt->second.members.begin(),
                                propertyIt->second.members.end(),
                                [&member]( MemberData const & md ) { return md.name == member; } );
  check( memberIt != propertyIt->second.members.end(),
         xmlLine,
         "unknown member <" + member + "> specified for SPIR-V capability" );
  if ( memberIt->type.type == "VkBool32" )
  {
    check( ( value == "VK_FALSE" ) || ( value == "VK_TRUE" ),
           xmlLine,
           "unknown value <" + value + "> for boolean member <" + member + "> specified for SPIR-V capability" );
  }
  else
  {
    auto bitmaskIt = m_bitmasks.find( memberIt->type.type );
    check( bitmaskIt != m_bitmasks.end(),
           xmlLine,
           "attribute member = <" + member + "> specified for SPIR-V capability is not a bitmask" );
    assert( !bitmaskIt->second.requirements.empty() );
    auto enumIt = m_enums.find( bitmaskIt->second.requirements );
    check( enumIt != m_enums.end(),
           xmlLine,
           "attribute member = <" + member + "> specified for SPIR-V capability requires an unknown enum <" +
             bitmaskIt->second.requirements + ">" );
    auto valueIt = std::find_if( enumIt->second.values.begin(),
                                 enumIt->second.values.end(),
                                 [&value]( EnumValueData const & evd ) { return evd.vulkanValue == value; } );
    check( valueIt != enumIt->second.values.end(),
           xmlLine,
           "unknown attribute value = <" + value + "> specified for SPIR-V capability" );
  }
}

void VulkanHppGenerator::readSPIRVCapabilityEnableStruct( int                                        xmlLine,
                                                          std::map<std::string, std::string> const & attributes )
{
  checkAttributes(
    xmlLine, attributes, { { "feature", {} }, { "struct", {} } }, { { "alias", {} }, { "requires", {} } } );

  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "requires" )
    {
      std::vector<std::string>
      requires = tokenize( attribute.second, "," );
      for ( auto const & r : requires )
      {
        check( ( m_features.find( r ) != m_features.end() ) || ( m_extensions.find( r ) != m_extensions.end() ),
               xmlLine,
               "unknown requires <" + r + "> specified for SPIR-V capability" );
      }
    }
    else if ( attribute.first == "struct" )
    {
      check( m_structures.find( attribute.second ) != m_structures.end(),
             xmlLine,
             "unknown structure <" + attribute.second + "> specified for SPIR-V capability" );
      check( attributes.find( "feature" ) != attributes.end(),
             xmlLine,
             "missing feature attribute for SPIR-V capability specified with struct <" + attribute.second + ">" );
    }
    else
    {
      assert( ( attribute.first == "alias" ) || ( attribute.first == "feature" ) );
    }
  }
}

void VulkanHppGenerator::readSPIRVCapabilityEnableVersion( int                                        xmlLine,
                                                           std::map<std::string, std::string> const & attributes )
{
  checkAttributes( xmlLine, attributes, { { "version", {} } }, {} );

  check(
    attributes.size() == 1, xmlLine, "unexpected attributes in addition to <version> specified for SPIR-V capability" );
  for ( auto const & attribute : attributes )
  {
    assert( attribute.first == "version" );
    check( beginsWith( attribute.second, "VK_API_VERSION_" ),
           xmlLine,
           "unknown version <" + attribute.second + "> specified for SPIR-V capability" );
    std::string feature = attribute.second;
    feature.erase( 3, 4 );  // remove "API_" from the version -> VK_VERSION_x_y
    check( m_features.find( feature ) != m_features.end(),
           xmlLine,
           "unknown version <" + attribute.second + "> specified for SPIR-V capability" );
  }
}

void VulkanHppGenerator::readSPIRVCapabilities( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "spirvcapability" } );

  for ( auto child : children )
  {
    std::string value = child->Value();
    assert( value == "spirvcapability" );
    readSPIRVCapability( child );
  }
}

void VulkanHppGenerator::readSPIRVExtension( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "enable" } );

  for ( auto child : children )
  {
    std::string value = child->Value();
    assert( value == "enable" );
    readSPIRVExtensionEnable( child );
  }
}

void VulkanHppGenerator::readSPIRVExtensionEnable( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, { { "extension", {} }, { "version", {} } } );
  checkElements( line, getChildElements( element ), {}, {} );

  check( !attributes.empty(), line, "no version or extension specified for SPIR-V extension" );
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "extension" )
    {
      check( m_extensions.find( attribute.second ) != m_extensions.end(),
             line,
             "unknown extension <" + attribute.second + "> specified for SPIR-V extension" );
    }
    else
    {
      assert( attribute.first == "version" );
      check( beginsWith( attribute.second, "VK_API_VERSION_" ),
             line,
             "unknown version <" + attribute.second + "> specified for SPIR-V extension" );
      std::string feature = attribute.second;
      feature.erase( 3, 4 );  // remove "API_" from the version -> VK_VERSION_x_y
      check( m_features.find( feature ) != m_features.end(),
             line,
             "unknown version <" + attribute.second + "> specified for SPIR-V extension" );
    }
  }
}

void VulkanHppGenerator::readSPIRVExtensions( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, {}, { "spirvextension" } );

  for ( auto child : children )
  {
    std::string value = child->Value();
    assert( value == "spirvextension" );
    readSPIRVExtension( child );
  }
}

void VulkanHppGenerator::readStruct( tinyxml2::XMLElement const *               element,
                                     bool                                       isUnion,
                                     std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();

  if ( attributes.find( "alias" ) != attributes.end() )
  {
    readStructAlias( element, attributes );
  }
  else
  {
    checkAttributes( line,
                     attributes,
                     { { "category", { isUnion ? "union" : "struct" } }, { "name", {} } },
                     { { "allowduplicate", { "true" } },
                       { "comment", {} },
                       { "returnedonly", { "true" } },
                       { "structextends", {} } } );
    std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
    checkElements( line, children, {}, { "member", "comment" } );

    std::string              category, name;
    std::vector<std::string> structExtends;
    bool                     allowDuplicate = false;
    bool                     returnedOnly   = false;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "allowduplicate" )
      {
        assert( attribute.second == "true" );
        allowDuplicate = true;
      }
      else if ( attribute.first == "category" )
      {
        category = attribute.second;
      }
      else if ( attribute.first == "name" )
      {
        name = attribute.second;
      }
      else if ( attribute.first == "returnedonly" )
      {
        check(
          attribute.second == "true", line, "unknown value for attribute returnedonly: <" + attribute.second + ">" );
        returnedOnly = true;
      }
      else if ( attribute.first == "structextends" )
      {
        structExtends = tokenize( attribute.second, "," );
      }
    }
    assert( !name.empty() );
    // make this warn a check, as soon as vk.xml has been fixed on attribute "allowduplicate" !
    warn( !allowDuplicate || !structExtends.empty(),
          line,
          "attribute <allowduplicate> is true, but no structures are listed in <structextends>" );

    check( m_structures.find( name ) == m_structures.end(), line, "struct <" + name + "> already specfied" );
    std::map<std::string, StructureData>::iterator it =
      m_structures.insert( std::make_pair( name, StructureData( structExtends, line ) ) ).first;
    it->second.allowDuplicate = allowDuplicate;
    it->second.isUnion        = isUnion;
    it->second.returnedOnly   = returnedOnly;

    for ( auto child : children )
    {
      std::string value = child->Value();
      if ( value == "comment" )
      {
        readComment( child );
      }
      else if ( value == "member" )
      {
        readStructMember( child, it->second.members, isUnion );
      }
    }
    it->second.subStruct = determineSubStruct( *it );

    // check if multiple structure members use the very same (not empty) len attribute
    // Note: even though the arrays are not marked as optional, they still might be mutually exclusive (like in
    // VkWriteDescriptorSet)! That is, there's not enough information available in vk.xml to decide on that, so we
    // need this external knowledge!
    static std::set<std::string> mutualExclusiveStructs = { "VkAccelerationStructureBuildGeometryInfoKHR",
                                                            "VkWriteDescriptorSet" };
    static std::set<std::string> multipleLenStructs     = { "VkImageConstraintsInfoFUCHSIA",
                                                        "VkIndirectCommandsLayoutTokenNV",
                                                        "VkPresentInfoKHR",
                                                        "VkSemaphoreWaitInfo",
                                                        "VkSubmitInfo",
                                                        "VkSubpassDescription",
                                                        "VkSubpassDescription2",
                                                        "VkWin32KeyedMutexAcquireReleaseInfoKHR",
                                                        "VkWin32KeyedMutexAcquireReleaseInfoNV" };
    bool                         warned                 = false;
    for ( auto m0It = it->second.members.begin(); !warned && ( m0It != it->second.members.end() ); ++m0It )
    {
      if ( !m0It->len.empty() && ( m0It->len.front() != "null-terminated" ) )
      {
        for ( auto m1It = std::next( m0It ); !warned && ( m1It != it->second.members.end() ); ++m1It )
        {
          if ( !m1It->len.empty() && ( m0It->len.front() == m1It->len.front() ) )
          {
            if ( mutualExclusiveStructs.find( it->first ) != mutualExclusiveStructs.end() )
            {
              it->second.mutualExclusiveLens = true;
            }
            else
            {
              warn(
                multipleLenStructs.find( it->first ) != multipleLenStructs.end(),
                line,
                "Encountered structure <" + it->first +
                  "> with multiple members referencing the same member for len. Need to be checked if they are supposed to be mutually exclusive." );
              warned = true;
            }
          }
        }
      }
    }

    m_extendedStructs.insert( structExtends.begin(), structExtends.end() );
    check(
      m_types.insert( std::make_pair( name, ( category == "struct" ) ? TypeCategory::Struct : TypeCategory::Union ) )
        .second,
      line,
      "struct <" + name + "> already specified as a type" );  // log type and alias in m_types
  }
}

void VulkanHppGenerator::readStructAlias( tinyxml2::XMLElement const *               element,
                                          std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();

  checkAttributes( line, attributes, { { "alias", {} }, { "category", { "struct" } }, { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {}, {} );

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

  auto structIt = m_structures.find( alias );
  check( structIt != m_structures.end(), line, "missing alias <" + alias + ">." );
  check(
    structIt->second.aliases.insert( name ).second, line, "struct <" + alias + "> already uses alias <" + name + ">" );
  check( m_structureAliases.insert( std::make_pair( name, alias ) ).second,
         line,
         "structure alias <" + name + "> already used" );
  check( m_types.insert( std::make_pair( name, TypeCategory::Struct ) ).second,
         line,
         "struct <" + name + "> already specified as a type" );
}

void VulkanHppGenerator::readStructMember( tinyxml2::XMLElement const * element,
                                           std::vector<MemberData> &    members,
                                           bool                         isUnion )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line,
                   attributes,
                   {},
                   { { "altlen", {} },
                     { "externsync", { "true" } },
                     { "len", {} },
                     { "limittype", { "bitmask", "max", "min", "noauto", "range", "struct" } },
                     { "noautovalidity", { "true" } },
                     { "optional", { "false", "true" } },
                     { "selection", {} },
                     { "selector", {} },
                     { "values", {} } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "name", true }, { "type", true } }, { "comment", "enum" } );

  MemberData memberData( line );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "enum" )
    {
      readStructMemberEnum( child, memberData );
    }
    else if ( value == "name" )
    {
      readStructMemberName( child, memberData, members );
    }
    else if ( value == "type" )
    {
      readStructMemberType( child, memberData );
    }
  }

  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "len" )
    {
      memberData.len = tokenize( attribute.second, "," );
      check( !memberData.len.empty() && ( memberData.len.size() <= 2 ),
             line,
             "member attribute <len> holds unknown number of data: " + std::to_string( memberData.len.size() ) );
      std::string const & len = memberData.len[0];
      auto                lenMember =
        std::find_if( members.begin(), members.end(), [&len]( MemberData const & md ) { return ( md.name == len ); } );
      check( ( lenMember != members.end() ) || ( ignoreLens.find( len ) != ignoreLens.end() ) ||
               ( len == R"(latexmath:[\textrm{codeSize} \over 4])" ),
             line,
             "member attribute <len> holds unknown value <" + len + ">" );
      if ( lenMember != members.end() )
      {
        check( lenMember->type.prefix.empty() && lenMember->type.postfix.empty(),
               line,
               "member attribute <len> references a member of unexpected type <" + lenMember->type.compose() + ">" );
      }
      if ( 1 < memberData.len.size() )
      {
        check( ( memberData.len[1] == "1" ) || ( memberData.len[1] == "null-terminated" ),
               line,
               "member attribute <len> holds unknown second value <" + memberData.len[1] + ">" );
      }
    }
    else if ( attribute.first == "noautovalidity" )
    {
      memberData.noAutoValidity = ( attribute.second == "true" );
    }
    else if ( attribute.first == "optional" )
    {
      std::vector<std::string> optional = tokenize( attribute.second, "," );
      memberData.optional.reserve( optional.size() );
      for ( auto const & o : optional )
      {
        memberData.optional.push_back( o == "true" );
      }
    }
    else if ( attribute.first == "selection" )
    {
      check( isUnion, line, "attribute <selection> is used with a non-union structure." );
      memberData.selection = attribute.second;
    }
    else if ( attribute.first == "selector" )
    {
      memberData.selector            = attribute.second;
      std::string const & selector   = memberData.selector;
      auto                selectorIt = std::find_if(
        members.begin(), members.end(), [selector]( MemberData const & md ) { return md.name == selector; } );
      check( selectorIt != members.end(), line, "member attribute <selector> holds unknown value <" + selector + ">" );
      check( m_enums.find( selectorIt->type.type ) != m_enums.end(),
             line,
             "member attribute <selector> references unknown enum type <" + selectorIt->type.type + ">" );
    }
    else if ( attribute.first == "values" )
    {
      memberData.values = tokenize( attribute.second, "," );
    }
  }

  members.push_back( memberData );
}

void VulkanHppGenerator::readStructMemberEnum( tinyxml2::XMLElement const * element, MemberData & memberData )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  checkElements( line, getChildElements( element ), {}, {} );

  std::string enumString = element->GetText();

  check( element->PreviousSibling() && ( strcmp( element->PreviousSibling()->Value(), "[" ) == 0 ) &&
           element->NextSibling() && ( strcmp( element->NextSibling()->Value(), "]" ) == 0 ),
         line,
         std::string( "structure member array specifiation is ill-formatted: <" ) + enumString + ">" );

  memberData.arraySizes.push_back( enumString );
  check( memberData.usedConstant.empty(), line, "struct already holds a constant <" + memberData.usedConstant + ">" );
  memberData.usedConstant = enumString;
}

void VulkanHppGenerator::readStructMemberName( tinyxml2::XMLElement const *    element,
                                               MemberData &                    memberData,
                                               std::vector<MemberData> const & members )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  checkElements( line, getChildElements( element ), {}, {} );

  std::string name = element->GetText();
  check( std::find_if( members.begin(), members.end(), [&name]( MemberData const & md ) { return md.name == name; } ) ==
           members.end(),
         line,
         "structure member name <" + name + "> already used" );

  memberData.name                                        = name;
  std::tie( memberData.arraySizes, memberData.bitCount ) = readModifiers( element->NextSibling() );
}

void VulkanHppGenerator::readStructMemberType( tinyxml2::XMLElement const * element, MemberData & memberData )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), {}, {} );
  checkElements( line, getChildElements( element ), {}, {} );

  memberData.type.prefix  = readTypePrefix( element->PreviousSibling() );
  memberData.type.type    = element->GetText();
  memberData.type.postfix = readTypePostfix( element->NextSibling() );
}

void VulkanHppGenerator::readTag( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "author", {} }, { "contact", {} }, { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      check( m_tags.find( attribute.second ) == m_tags.end(),
             line,
             "tag named <" + attribute.second + "> has already been specified" );
      m_tags.insert( attribute.second );
    }
    else
    {
      check( ( attribute.first == "author" ) || ( attribute.first == "contact" ),
             line,
             "unknown attribute <" + attribute.first + ">" );
    }
  }
}

void VulkanHppGenerator::readTags( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "tag", false } } );

  for ( auto child : children )
  {
    readTag( child );
  }
}

void VulkanHppGenerator::readType( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );

  auto categoryIt = attributes.find( "category" );
  if ( categoryIt != attributes.end() )
  {
    if ( categoryIt->second == "basetype" )
    {
      readBaseType( element, attributes );
    }
    else if ( categoryIt->second == "bitmask" )
    {
      readBitmask( element, attributes );
    }
    else if ( categoryIt->second == "define" )
    {
      readDefine( element, attributes );
    }
    else if ( categoryIt->second == "enum" )
    {
      readTypeEnum( element, attributes );
    }
    else if ( categoryIt->second == "funcpointer" )
    {
      readFuncpointer( element, attributes );
    }
    else if ( categoryIt->second == "handle" )
    {
      readHandle( element, attributes );
    }
    else if ( categoryIt->second == "include" )
    {
      readTypeInclude( element, attributes );
    }
    else if ( categoryIt->second == "struct" )
    {
      readStruct( element, false, attributes );
    }
    else
    {
      check(
        categoryIt->second == "union", element->GetLineNum(), "unknown type category <" + categoryIt->second + ">" );
      readStruct( element, true, attributes );
    }
  }
  else
  {
    auto requiresIt = attributes.find( "requires" );
    if ( requiresIt != attributes.end() )
    {
      readRequires( element, attributes );
    }
    else
    {
      check( ( attributes.size() == 1 ) && ( attributes.begin()->first == "name" ) &&
               ( attributes.begin()->second == "int" ),
             line,
             "unknown type" );
      check( m_types.insert( std::make_pair( attributes.begin()->second, TypeCategory::Unknown ) ).second,
             line,
             "type <" + attributes.begin()->second + "> already specified" );
    }
  }
}

void VulkanHppGenerator::readTypeEnum( tinyxml2::XMLElement const *               element,
                                       std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "enum" } }, { "name", {} } }, { { "alias", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string alias, name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      alias = attribute.second;
      check( !alias.empty(), line, "enum with empty alias" );
    }
    else if ( attribute.first == "name" )
    {
      name = attribute.second;
      check( !name.empty(), line, "enum with empty name" );
      check( m_enums.find( name ) == m_enums.end(), line, "enum <" + name + "> already specified" );
    }
  }
  assert( !name.empty() );

  if ( alias.empty() )
  {
    check( m_enums.insert( std::make_pair( name, EnumData( line ) ) ).second,
           line,
           "enum <" + name + "> already specified" );
  }
  else
  {
    auto enumIt = m_enums.find( alias );
    check( enumIt != m_enums.end(), line, "enum with unknown alias <" + alias + ">" );
    check( enumIt->second.alias.empty(),
           line,
           "enum <" + enumIt->first + "> already has an alias <" + enumIt->second.alias + ">" );
    enumIt->second.alias = name;
  }
  check( m_types.insert( std::make_pair( name, TypeCategory::Enum ) ).second,
         line,
         "enum <" + name + "> already specified as a type" );
}

void VulkanHppGenerator::readTypeInclude( tinyxml2::XMLElement const *               element,
                                          std::map<std::string, std::string> const & attributes )
{
  int line = element->GetLineNum();
  checkAttributes( line, attributes, { { "category", { "include" } }, { "name", {} } }, {} );
  checkElements( line, getChildElements( element ), {} );

  std::string name = attributes.find( "name" )->second;
  check( m_includes.insert( name ).second, element->GetLineNum(), "include named <" + name + "> already specified" );
}

void VulkanHppGenerator::readTypes( tinyxml2::XMLElement const * element )
{
  int line = element->GetLineNum();
  checkAttributes( line, getAttributes( element ), { { "comment", {} } }, {} );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );
  checkElements( line, children, { { "comment", false }, { "type", false } } );

  for ( auto child : children )
  {
    std::string value = child->Value();
    if ( value == "comment" )
    {
      readComment( child );
    }
    else
    {
      assert( value == "type" );
      readType( child );
    }
  }
}

void VulkanHppGenerator::registerDeleter( std::string const &                         name,
                                          std::pair<std::string, CommandData> const & commandData )
{
  if ( ( commandData.first.substr( 2, 7 ) == "Destroy" ) || ( commandData.first.substr( 2, 4 ) == "Free" ) )
  {
    std::string key;
    size_t      valueIndex;
    switch ( commandData.second.params.size() )
    {
      case 2:
      case 3:
        assert( commandData.second.params.back().type.type == "VkAllocationCallbacks" );
        key        = ( commandData.second.params.size() == 2 ) ? "" : commandData.second.params[0].type.type;
        valueIndex = commandData.second.params.size() - 2;
        break;
      case 4:
        key        = commandData.second.params[0].type.type;
        valueIndex = 3;
        assert( m_handles.find( commandData.second.params[valueIndex].type.type ) != m_handles.end() );
        m_handles.find( commandData.second.params[valueIndex].type.type )->second.deletePool =
          commandData.second.params[1].type.type;
        break;
      default: assert( false ); valueIndex = 0;
    }
    auto keyHandleIt = m_handles.find( key );
    assert( ( keyHandleIt != m_handles.end() ) &&
            ( keyHandleIt->second.childrenHandles.find( commandData.second.params[valueIndex].type.type ) ==
              keyHandleIt->second.childrenHandles.end() ) );
    keyHandleIt->second.childrenHandles.insert( commandData.second.params[valueIndex].type.type );

    auto handleIt = m_handles.find( commandData.second.params[valueIndex].type.type );
    assert( handleIt != m_handles.end() );
    handleIt->second.deleteCommand = name;
  }
}

void VulkanHppGenerator::renameFunctionParameters()
{
  // we rename a couple of function parameters to prevent this warning, treated as an error:
  // warning C4458: declaration of 'objectType' hides class member
  for ( auto & command : m_commands )
  {
    for ( auto & param : command.second.params )
    {
      if ( param.name == "objectType" )
      {
        param.name += "_";
      }
    }
  }
}

void VulkanHppGenerator::rescheduleRAIIHandle( std::string &                              str,
                                               std::string &                              commandDefinitions,
                                               std::pair<std::string, HandleData> const & handle,
                                               std::set<std::string> &                    listedHandles,
                                               std::set<std::string> const &              specialFunctions ) const
{
  listedHandles.insert( handle.first );
  if ( !handle.second.parent.empty() && ( listedHandles.find( handle.second.parent ) == listedHandles.end() ) )
  {
    auto parentIt = m_handles.find( handle.second.parent );
    assert( parentIt != m_handles.end() );
    appendRAIIHandle( str, commandDefinitions, *parentIt, listedHandles, specialFunctions );
  }

  for ( auto constructorIt : handle.second.constructorIts )
  {
    for ( auto const & param : constructorIt->second.params )
    {
      auto handleIt = m_handles.find( param.type.type );
      if ( handleIt != m_handles.end() && ( listedHandles.find( param.type.type ) == listedHandles.end() ) )
      {
        appendRAIIHandle( str, commandDefinitions, *handleIt, listedHandles, specialFunctions );
      }
    }
  }
}

void VulkanHppGenerator::setVulkanLicenseHeader( int line, std::string const & comment )
{
  check( m_vulkanLicenseHeader.empty(), line, "second encounter of a Copyright comment" );
  m_vulkanLicenseHeader = comment;

  // replace any '\n' with "\n// "
  for ( size_t pos = m_vulkanLicenseHeader.find( '\n' ); pos != std::string::npos;
        pos        = m_vulkanLicenseHeader.find( '\n', pos + 1 ) )
  {
    m_vulkanLicenseHeader.replace( pos, 1, "\n// " );
  }
  // replace any " \n" with "\n"
  for ( size_t pos = m_vulkanLicenseHeader.find( " \n" ); pos != std::string::npos;
        pos        = m_vulkanLicenseHeader.find( " \n", pos ) )
  {
    m_vulkanLicenseHeader.replace( pos, 2, "\n" );
  }
  // remove any trailing spaces
  m_vulkanLicenseHeader = trimEnd( m_vulkanLicenseHeader );

  // and add a little message on our own
  m_vulkanLicenseHeader += "\n\n// This header is generated from the Khronos Vulkan XML API Registry.";
  m_vulkanLicenseHeader = trim( m_vulkanLicenseHeader ) + "\n";
}

std::string VulkanHppGenerator::toString( TypeCategory category )
{
  switch ( category )
  {
    case TypeCategory::Bitmask: return "bitmask";
    case TypeCategory::BaseType: return "basetype";
    case TypeCategory::Define: return "define";
    case TypeCategory::Enum: return "enum";
    case TypeCategory::FuncPointer: return "funcpointer";
    case TypeCategory::Handle: return "handle";
    case TypeCategory::Requires: return "requires";
    case TypeCategory::Struct: return "struct";
    case TypeCategory::Union: return "union";
    case TypeCategory::Unknown: return "unkown";
    default: assert( false ); return "";
  }
}

std::string VulkanHppGenerator::TypeInfo::compose( bool inNamespace ) const
{
  return prefix + ( prefix.empty() ? "" : " " ) +
         ( inNamespace
             ? ( ( ( type.substr( 0, 2 ) == "Vk" ) ? "VULKAN_HPP_NAMESPACE::" : "" ) + stripPrefix( type, "Vk" ) )
             : type ) +
         postfix;
}

std::string to_string( tinyxml2::XMLError error )
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

int main( int argc, char ** argv )
{
  static const std::string classArrayProxy = R"(
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
  template <typename T>
  class ArrayProxy
  {
  public:
    VULKAN_HPP_CONSTEXPR ArrayProxy() VULKAN_HPP_NOEXCEPT
      : m_count( 0 )
      , m_ptr( nullptr )
    {}

    VULKAN_HPP_CONSTEXPR ArrayProxy( std::nullptr_t ) VULKAN_HPP_NOEXCEPT
      : m_count( 0 )
      , m_ptr( nullptr )
    {}

    ArrayProxy( T & value ) VULKAN_HPP_NOEXCEPT
      : m_count( 1 )
      , m_ptr( &value )
    {}

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxy( typename std::remove_const<T>::type & value ) VULKAN_HPP_NOEXCEPT
      : m_count( 1 )
      , m_ptr( &value )
    {}

    ArrayProxy( uint32_t count, T * ptr ) VULKAN_HPP_NOEXCEPT
      : m_count( count )
      , m_ptr( ptr )
    {}

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxy( uint32_t count, typename std::remove_const<T>::type * ptr ) VULKAN_HPP_NOEXCEPT
      : m_count( count )
      , m_ptr( ptr )
    {}

#  if __GNUC__ >= 9
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Winit-list-lifetime"
#  endif

    ArrayProxy( std::initializer_list<T> const & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {}

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxy( std::initializer_list<typename std::remove_const<T>::type> const & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {}

    ArrayProxy( std::initializer_list<T> & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {}

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxy( std::initializer_list<typename std::remove_const<T>::type> & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {}

#  if __GNUC__ >= 9
#    pragma GCC diagnostic pop
#  endif

    template <size_t N>
    ArrayProxy( std::array<T, N> const & data ) VULKAN_HPP_NOEXCEPT
      : m_count( N )
      , m_ptr( data.data() )
    {}

    template <size_t N, typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxy( std::array<typename std::remove_const<T>::type, N> const & data ) VULKAN_HPP_NOEXCEPT
      : m_count( N )
      , m_ptr( data.data() )
    {}

    template <size_t N>
    ArrayProxy( std::array<T, N> & data ) VULKAN_HPP_NOEXCEPT
      : m_count( N )
      , m_ptr( data.data() )
    {}

    template <size_t N, typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxy( std::array<typename std::remove_const<T>::type, N> & data ) VULKAN_HPP_NOEXCEPT
      : m_count( N )
      , m_ptr( data.data() )
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxy( std::vector<T, Allocator> const & data ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( data.size() ) )
      , m_ptr( data.data() )
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>,
              typename B      = T,
              typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxy( std::vector<typename std::remove_const<T>::type, Allocator> const & data ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( data.size() ) )
      , m_ptr( data.data() )
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxy( std::vector<T, Allocator> & data ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( data.size() ) )
      , m_ptr( data.data() )
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>,
              typename B      = T,
              typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxy( std::vector<typename std::remove_const<T>::type, Allocator> & data ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( data.size() ) )
      , m_ptr( data.data() )
    {}

    const T * begin() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }

    const T * end() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr + m_count;
    }

    const T & front() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_count && m_ptr );
      return *m_ptr;
    }

    const T & back() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_count && m_ptr );
      return *( m_ptr + m_count - 1 );
    }

    bool empty() const VULKAN_HPP_NOEXCEPT
    {
      return ( m_count == 0 );
    }

    uint32_t size() const VULKAN_HPP_NOEXCEPT
    {
      return m_count;
    }

    T * data() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }

  private:
    uint32_t m_count;
    T *      m_ptr;
  };

  template <typename T>
  class ArrayProxyNoTemporaries
  {
  public:
    VULKAN_HPP_CONSTEXPR ArrayProxyNoTemporaries() VULKAN_HPP_NOEXCEPT
      : m_count( 0 )
      , m_ptr( nullptr )
    {}

    VULKAN_HPP_CONSTEXPR ArrayProxyNoTemporaries( std::nullptr_t ) VULKAN_HPP_NOEXCEPT
      : m_count( 0 )
      , m_ptr( nullptr )
    {}

    ArrayProxyNoTemporaries( T & value ) VULKAN_HPP_NOEXCEPT
      : m_count( 1 )
      , m_ptr( &value )
    {}

    ArrayProxyNoTemporaries( T && value ) = delete;

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( typename std::remove_const<T>::type & value ) VULKAN_HPP_NOEXCEPT
      : m_count( 1 )
      , m_ptr( &value )
    {}

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( typename std::remove_const<T>::type && value ) = delete;

    ArrayProxyNoTemporaries( uint32_t count, T * ptr ) VULKAN_HPP_NOEXCEPT
      : m_count( count )
      , m_ptr( ptr )
    {}

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( uint32_t count, typename std::remove_const<T>::type * ptr ) VULKAN_HPP_NOEXCEPT
      : m_count( count )
      , m_ptr( ptr )
    {}

    ArrayProxyNoTemporaries( std::initializer_list<T> const & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {}

    ArrayProxyNoTemporaries( std::initializer_list<T> const && list ) = delete;

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::initializer_list<typename std::remove_const<T>::type> const & list )
      VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {}

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::initializer_list<typename std::remove_const<T>::type> const && list ) = delete;

    ArrayProxyNoTemporaries( std::initializer_list<T> & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {}

    ArrayProxyNoTemporaries( std::initializer_list<T> && list ) = delete;

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::initializer_list<typename std::remove_const<T>::type> & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {}

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::initializer_list<typename std::remove_const<T>::type> && list ) = delete;

    template <size_t N>
    ArrayProxyNoTemporaries( std::array<T, N> const & data ) VULKAN_HPP_NOEXCEPT
      : m_count( N )
      , m_ptr( data.data() )
    {}

    template <size_t N>
    ArrayProxyNoTemporaries( std::array<T, N> const && data ) = delete;

    template <size_t N, typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::array<typename std::remove_const<T>::type, N> const & data ) VULKAN_HPP_NOEXCEPT
      : m_count( N )
      , m_ptr( data.data() )
    {}

    template <size_t N, typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::array<typename std::remove_const<T>::type, N> const && data ) = delete;

    template <size_t N>
    ArrayProxyNoTemporaries( std::array<T, N> & data ) VULKAN_HPP_NOEXCEPT
      : m_count( N )
      , m_ptr( data.data() )
    {}

    template <size_t N>
    ArrayProxyNoTemporaries( std::array<T, N> && data ) = delete;

    template <size_t N, typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::array<typename std::remove_const<T>::type, N> & data ) VULKAN_HPP_NOEXCEPT
      : m_count( N )
      , m_ptr( data.data() )
    {}

    template <size_t N, typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::array<typename std::remove_const<T>::type, N> && data ) = delete;

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxyNoTemporaries( std::vector<T, Allocator> const & data ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( data.size() ) )
      , m_ptr( data.data() )
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxyNoTemporaries( std::vector<T, Allocator> const && data ) = delete;

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>,
              typename B      = T,
              typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::vector<typename std::remove_const<T>::type, Allocator> const & data )
      VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( data.size() ) )
      , m_ptr( data.data() )
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>,
              typename B      = T,
              typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::vector<typename std::remove_const<T>::type, Allocator> const && data ) = delete;

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxyNoTemporaries( std::vector<T, Allocator> & data ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( data.size() ) )
      , m_ptr( data.data() )
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxyNoTemporaries( std::vector<T, Allocator> && data ) = delete;

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>,
              typename B      = T,
              typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::vector<typename std::remove_const<T>::type, Allocator> & data ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( data.size() ) )
      , m_ptr( data.data() )
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>,
              typename B      = T,
              typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::vector<typename std::remove_const<T>::type, Allocator> && data ) = delete;

    const T * begin() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }

    const T * end() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr + m_count;
    }

    const T & front() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_count && m_ptr );
      return *m_ptr;
    }

    const T & back() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_count && m_ptr );
      return *( m_ptr + m_count - 1 );
    }

    bool empty() const VULKAN_HPP_NOEXCEPT
    {
      return ( m_count == 0 );
    }

    uint32_t size() const VULKAN_HPP_NOEXCEPT
    {
      return m_count;
    }

    T * data() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }

  private:
    uint32_t m_count;
    T *      m_ptr;
  };
#endif
)";

  static const std::string classArrayWrapper = R"(
  template <typename T, size_t N>
  class ArrayWrapper1D : public std::array<T, N>
  {
  public:
    VULKAN_HPP_CONSTEXPR ArrayWrapper1D() VULKAN_HPP_NOEXCEPT
      : std::array<T, N>()
    {}

    VULKAN_HPP_CONSTEXPR ArrayWrapper1D( std::array<T, N> const & data ) VULKAN_HPP_NOEXCEPT
      : std::array<T, N>( data )
    {}

#if defined( _WIN32 ) && !defined( _WIN64 )
    VULKAN_HPP_CONSTEXPR T const & operator[]( int index ) const VULKAN_HPP_NOEXCEPT
    {
      return std::array<T, N>::operator[]( index );
    }

    T & operator[]( int index ) VULKAN_HPP_NOEXCEPT
    {
      return std::array<T, N>::operator[]( index );
    }
#endif

    operator T const * () const VULKAN_HPP_NOEXCEPT
    {
      return this->data();
    }

    operator T * () VULKAN_HPP_NOEXCEPT
    {
      return this->data();
    }

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    operator std::string() const
    {
      return std::string( this->data() );
    }

#if 17 <= VULKAN_HPP_CPP_VERSION
    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    operator std::string_view() const
    {
      return std::string_view( this->data() );
    }
#endif

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    bool operator<( ArrayWrapper1D<char, N> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return *static_cast<std::array<char, N> const *>( this ) < *static_cast<std::array<char, N> const *>( &rhs );
    }

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    bool operator<=( ArrayWrapper1D<char, N> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return *static_cast<std::array<char, N> const *>( this ) <= *static_cast<std::array<char, N> const *>( &rhs );
    }

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    bool operator>( ArrayWrapper1D<char, N> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return *static_cast<std::array<char, N> const *>( this ) > *static_cast<std::array<char, N> const *>( &rhs );
    }

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    bool operator>=( ArrayWrapper1D<char, N> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return *static_cast<std::array<char, N> const *>( this ) >= *static_cast<std::array<char, N> const *>( &rhs );
    }

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    bool operator==( ArrayWrapper1D<char, N> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return *static_cast<std::array<char, N> const *>( this ) == *static_cast<std::array<char, N> const *>( &rhs );
    }

    template <typename B = T, typename std::enable_if<std::is_same<B, char>::value, int>::type = 0>
    bool operator!=( ArrayWrapper1D<char, N> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return *static_cast<std::array<char, N> const *>( this ) != *static_cast<std::array<char, N> const *>( &rhs );
    }
  };

  // specialization of relational operators between std::string and arrays of chars
  template <size_t N>
  bool operator<( std::string const & lhs, ArrayWrapper1D<char, N> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs < rhs.data();
  }

  template <size_t N>
  bool operator<=( std::string const & lhs, ArrayWrapper1D<char, N> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs <= rhs.data();
  }

  template <size_t N>
  bool operator>( std::string const & lhs, ArrayWrapper1D<char, N> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs > rhs.data();
  }

  template <size_t N>
  bool operator>=( std::string const & lhs, ArrayWrapper1D<char, N> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs >= rhs.data();
  }

  template <size_t N>
  bool operator==( std::string const & lhs, ArrayWrapper1D<char, N> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs == rhs.data();
  }

  template <size_t N>
  bool operator!=( std::string const & lhs, ArrayWrapper1D<char, N> const & rhs ) VULKAN_HPP_NOEXCEPT
  {
    return lhs != rhs.data();
  }

  template <typename T, size_t N, size_t M>
  class ArrayWrapper2D : public std::array<ArrayWrapper1D<T, M>, N>
  {
  public:
    VULKAN_HPP_CONSTEXPR ArrayWrapper2D() VULKAN_HPP_NOEXCEPT
      : std::array<ArrayWrapper1D<T, M>, N>()
    {}

    VULKAN_HPP_CONSTEXPR ArrayWrapper2D( std::array<std::array<T, M>, N> const & data ) VULKAN_HPP_NOEXCEPT
      : std::array<ArrayWrapper1D<T, M>, N>( *reinterpret_cast<std::array<ArrayWrapper1D<T, M>, N> const *>( &data ) )
    {}
  };
)";

  static const std::string classFlags = R"(
  template <typename FlagBitsType>
  struct FlagTraits
  {
    enum
    {
      allFlags = 0
    };
  };

  template <typename BitType>
  class Flags
  {
  public:
    using MaskType = typename std::underlying_type<BitType>::type;

    // constructors
    VULKAN_HPP_CONSTEXPR Flags() VULKAN_HPP_NOEXCEPT
      : m_mask( 0 )
    {}

    VULKAN_HPP_CONSTEXPR Flags( BitType bit ) VULKAN_HPP_NOEXCEPT
      : m_mask( static_cast<MaskType>( bit ) )
    {}

    VULKAN_HPP_CONSTEXPR Flags( Flags<BitType> const & rhs ) VULKAN_HPP_NOEXCEPT = default;

    VULKAN_HPP_CONSTEXPR explicit Flags( MaskType flags ) VULKAN_HPP_NOEXCEPT
      : m_mask( flags )
    {}

    // relational operators
#if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
    auto operator<=>( Flags<BitType> const & ) const = default;
#else
    VULKAN_HPP_CONSTEXPR bool operator<( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask < rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator<=( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask <= rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator>( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask > rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator>=( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask >= rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator==( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask == rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator!=( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask != rhs.m_mask;
    }
#endif

    // logical operator
    VULKAN_HPP_CONSTEXPR bool operator!() const VULKAN_HPP_NOEXCEPT
    {
      return !m_mask;
    }

    // bitwise operators
    VULKAN_HPP_CONSTEXPR Flags<BitType> operator&( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return Flags<BitType>( m_mask & rhs.m_mask );
    }

    VULKAN_HPP_CONSTEXPR Flags<BitType> operator|( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return Flags<BitType>( m_mask | rhs.m_mask );
    }

    VULKAN_HPP_CONSTEXPR Flags<BitType> operator^( Flags<BitType> const & rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return Flags<BitType>( m_mask ^ rhs.m_mask );
    }

    VULKAN_HPP_CONSTEXPR Flags<BitType> operator~() const VULKAN_HPP_NOEXCEPT
    {
      return Flags<BitType>( m_mask ^ FlagTraits<BitType>::allFlags );
    }

    // assignment operators
    VULKAN_HPP_CONSTEXPR_14 Flags<BitType> & operator=( Flags<BitType> const & rhs ) VULKAN_HPP_NOEXCEPT = default;

    VULKAN_HPP_CONSTEXPR_14 Flags<BitType> & operator|=( Flags<BitType> const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      m_mask |= rhs.m_mask;
      return *this;
    }

    VULKAN_HPP_CONSTEXPR_14 Flags<BitType> & operator&=( Flags<BitType> const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      m_mask &= rhs.m_mask;
      return *this;
    }

    VULKAN_HPP_CONSTEXPR_14 Flags<BitType> & operator^=( Flags<BitType> const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      m_mask ^= rhs.m_mask;
      return *this;
    }

    // cast operators
    explicit VULKAN_HPP_CONSTEXPR operator bool() const VULKAN_HPP_NOEXCEPT
    {
      return !!m_mask;
    }

    explicit VULKAN_HPP_CONSTEXPR operator MaskType() const VULKAN_HPP_NOEXCEPT
    {
      return m_mask;
    }

#if defined( VULKAN_HPP_FLAGS_MASK_TYPE_AS_PUBLIC )
  public:
#else
  private:
#endif
    MaskType m_mask;
  };

#if !defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
  // relational operators only needed for pre C++20
  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator<( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator>( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator<=( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator>=( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator>( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator<( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator>=( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator<=( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator==( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator==( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator!=( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator!=( bit );
  }
#endif

  // bitwise operators
  template <typename BitType>
  VULKAN_HPP_CONSTEXPR Flags<BitType> operator&( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator&( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR Flags<BitType> operator|( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator|( bit );
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR Flags<BitType> operator^( BitType bit, Flags<BitType> const & flags ) VULKAN_HPP_NOEXCEPT
  {
    return flags.operator^( bit );
  }
)";

  static const std::string classObjectDestroy = R"(
  struct AllocationCallbacks;

  template <typename OwnerType, typename Dispatch>
  class ObjectDestroy
  {
  public:
    ObjectDestroy() = default;

    ObjectDestroy( OwnerType owner,
                   Optional<const AllocationCallbacks> allocationCallbacks
                                    VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                   Dispatch const & dispatch = VULKAN_HPP_DEFAULT_DISPATCHER ) VULKAN_HPP_NOEXCEPT
      : m_owner( owner )
      , m_allocationCallbacks( allocationCallbacks )
      , m_dispatch( &dispatch )
    {}

    OwnerType getOwner() const VULKAN_HPP_NOEXCEPT { return m_owner; }
    Optional<const AllocationCallbacks> getAllocator() const VULKAN_HPP_NOEXCEPT { return m_allocationCallbacks; }

  protected:
    template <typename T>
    void destroy(T t) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_owner && m_dispatch );
      m_owner.destroy( t, m_allocationCallbacks, *m_dispatch );
    }

  private:
    OwnerType                           m_owner               = {};
    Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
    Dispatch const *                    m_dispatch            = nullptr;
  };

  class NoParent;

  template <typename Dispatch>
  class ObjectDestroy<NoParent, Dispatch>
  {
  public:
    ObjectDestroy() = default;

    ObjectDestroy( Optional<const AllocationCallbacks> allocationCallbacks,
                   Dispatch const &                    dispatch = VULKAN_HPP_DEFAULT_DISPATCHER ) VULKAN_HPP_NOEXCEPT
      : m_allocationCallbacks( allocationCallbacks )
      , m_dispatch( &dispatch )
    {}

    Optional<const AllocationCallbacks> getAllocator() const VULKAN_HPP_NOEXCEPT { return m_allocationCallbacks; }

  protected:
    template <typename T>
    void destroy(T t) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_dispatch );
      t.destroy( m_allocationCallbacks, *m_dispatch );
    }

  private:
    Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
    Dispatch const *                    m_dispatch            = nullptr;
  };
)";

  static const std::string classObjectFree = R"(
  template <typename OwnerType, typename Dispatch>
  class ObjectFree
  {
  public:
    ObjectFree() = default;

    ObjectFree( OwnerType                                               owner,
                Optional<const AllocationCallbacks> allocationCallbacks VULKAN_HPP_DEFAULT_ARGUMENT_NULLPTR_ASSIGNMENT,
                Dispatch const & dispatch = VULKAN_HPP_DEFAULT_DISPATCHER ) VULKAN_HPP_NOEXCEPT
      : m_owner( owner )
      , m_allocationCallbacks( allocationCallbacks )
      , m_dispatch( &dispatch )
    {}

    OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
    {
      return m_owner;
    }

    Optional<const AllocationCallbacks> getAllocator() const VULKAN_HPP_NOEXCEPT
    {
      return m_allocationCallbacks;
    }

  protected:
    template <typename T>
    void destroy( T t ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_owner && m_dispatch );
      m_owner.free( t, m_allocationCallbacks, *m_dispatch );
    }

  private:
    OwnerType                           m_owner               = {};
    Optional<const AllocationCallbacks> m_allocationCallbacks = nullptr;
    Dispatch const *                    m_dispatch            = nullptr;
  };
)";

  static const std::string classObjectRelease = R"(
  template <typename OwnerType, typename Dispatch>
  class ObjectRelease
  {
  public:
    ObjectRelease() = default;

    ObjectRelease( OwnerType owner, Dispatch const & dispatch = VULKAN_HPP_DEFAULT_DISPATCHER ) VULKAN_HPP_NOEXCEPT
      : m_owner( owner )
      , m_dispatch( &dispatch )
    {}

    OwnerType getOwner() const VULKAN_HPP_NOEXCEPT
    {
      return m_owner;
    }

  protected:
    template <typename T>
    void destroy( T t ) VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( m_owner && m_dispatch );
      m_owner.release( t, *m_dispatch );
    }

  private:
    OwnerType        m_owner    = {};
    Dispatch const * m_dispatch = nullptr;
  };
)";

  static const std::string classOptional = R"(
  template <typename RefType>
  class Optional
  {
  public:
    Optional( RefType & reference ) VULKAN_HPP_NOEXCEPT
    {
      m_ptr = &reference;
    }
    Optional( RefType * ptr ) VULKAN_HPP_NOEXCEPT
    {
      m_ptr = ptr;
    }
    Optional( std::nullptr_t ) VULKAN_HPP_NOEXCEPT
    {
      m_ptr = nullptr;
    }

    operator RefType *() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }
    RefType const * operator->() const VULKAN_HPP_NOEXCEPT
    {
      return m_ptr;
    }
    explicit operator bool() const VULKAN_HPP_NOEXCEPT
    {
      return !!m_ptr;
    }

  private:
    RefType * m_ptr;
  };
)";

  static const std::string classPoolFree = R"(
  template <typename OwnerType, typename PoolType, typename Dispatch>
  class PoolFree
  {
    public:
      PoolFree() = default;

      PoolFree( OwnerType        owner,
                PoolType         pool,
                Dispatch const & dispatch = VULKAN_HPP_DEFAULT_DISPATCHER ) VULKAN_HPP_NOEXCEPT
        : m_owner( owner )
        , m_pool( pool )
        , m_dispatch( &dispatch )
      {}

      OwnerType getOwner() const VULKAN_HPP_NOEXCEPT { return m_owner; }
      PoolType getPool() const VULKAN_HPP_NOEXCEPT { return m_pool; }

    protected:
      template <typename T>
      void destroy(T t) VULKAN_HPP_NOEXCEPT
      {
        m_owner.free( m_pool, t, *m_dispatch );
      }

    private:
      OwnerType        m_owner    = OwnerType();
      PoolType         m_pool     = PoolType();
      Dispatch const * m_dispatch = nullptr;
  };
)";

  static const std::string classStructureChain = R"(
  template <typename X, typename Y>
  struct StructExtends
  {
    enum
    {
      value = false
    };
  };

  template <typename Type, class...>
  struct IsPartOfStructureChain
  {
    static const bool valid = false;
  };

  template <typename Type, typename Head, typename... Tail>
  struct IsPartOfStructureChain<Type, Head, Tail...>
  {
    static const bool valid = std::is_same<Type, Head>::value || IsPartOfStructureChain<Type, Tail...>::valid;
  };

  template <size_t Index, typename T, typename... ChainElements>
  struct StructureChainContains
  {
    static const bool value =
      std::is_same<T, typename std::tuple_element<Index, std::tuple<ChainElements...>>::type>::value ||
      StructureChainContains<Index - 1, T, ChainElements...>::value;
  };

  template <typename T, typename... ChainElements>
  struct StructureChainContains<0, T, ChainElements...>
  {
    static const bool value =
      std::is_same<T, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value;
  };

  template <size_t Index, typename... ChainElements>
  struct StructureChainValidation
  {
    using TestType = typename std::tuple_element<Index, std::tuple<ChainElements...>>::type;
    static const bool valid =
      StructExtends<TestType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value &&
      ( TestType::allowDuplicate || !StructureChainContains<Index - 1, TestType, ChainElements...>::value ) &&
      StructureChainValidation<Index - 1, ChainElements...>::valid;
  };

  template <typename... ChainElements>
  struct StructureChainValidation<0, ChainElements...>
  {
    static const bool valid = true;
  };

  template <typename... ChainElements>
  class StructureChain : public std::tuple<ChainElements...>
  {
  public:
    StructureChain() VULKAN_HPP_NOEXCEPT
    {
      static_assert( StructureChainValidation<sizeof...( ChainElements ) - 1, ChainElements...>::valid,
                     "The structure chain is not valid!" );
      link<sizeof...( ChainElements ) - 1>();
    }

    StructureChain( StructureChain const & rhs ) VULKAN_HPP_NOEXCEPT : std::tuple<ChainElements...>( rhs )
    {
      static_assert( StructureChainValidation<sizeof...( ChainElements ) - 1, ChainElements...>::valid,
                     "The structure chain is not valid!" );
      link<sizeof...( ChainElements ) - 1>();
    }

    StructureChain( StructureChain && rhs ) VULKAN_HPP_NOEXCEPT
      : std::tuple<ChainElements...>( std::forward<std::tuple<ChainElements...>>( rhs ) )
    {
      static_assert( StructureChainValidation<sizeof...( ChainElements ) - 1, ChainElements...>::valid,
                     "The structure chain is not valid!" );
      link<sizeof...( ChainElements ) - 1>();
    }

    StructureChain( ChainElements const &... elems ) VULKAN_HPP_NOEXCEPT : std::tuple<ChainElements...>( elems... )
    {
      static_assert( StructureChainValidation<sizeof...( ChainElements ) - 1, ChainElements...>::valid,
                     "The structure chain is not valid!" );
      link<sizeof...( ChainElements ) - 1>();
    }

    StructureChain & operator=( StructureChain const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      std::tuple<ChainElements...>::operator=( rhs );
      link<sizeof...( ChainElements ) - 1>();
      return *this;
    }

    StructureChain & operator=( StructureChain && rhs ) = delete;

    template <typename T = typename std::tuple_element<0, std::tuple<ChainElements...>>::type, size_t Which = 0>
    T & get() VULKAN_HPP_NOEXCEPT
    {
      return std::get<ChainElementIndex<0, T, Which, void, ChainElements...>::value>(
        static_cast<std::tuple<ChainElements...> &>( *this ) );
    }

    template <typename T = typename std::tuple_element<0, std::tuple<ChainElements...>>::type, size_t Which = 0>
    T const & get() const VULKAN_HPP_NOEXCEPT
    {
      return std::get<ChainElementIndex<0, T, Which, void, ChainElements...>::value>(
        static_cast<std::tuple<ChainElements...> const &>( *this ) );
    }

    template <typename T0, typename T1, typename... Ts>
    std::tuple<T0 &, T1 &, Ts &...> get() VULKAN_HPP_NOEXCEPT
    {
      return std::tie( get<T0>(), get<T1>(), get<Ts>()... );
    }

    template <typename T0, typename T1, typename... Ts>
    std::tuple<T0 const &, T1 const &, Ts const &...> get() const VULKAN_HPP_NOEXCEPT
    {
      return std::tie( get<T0>(), get<T1>(), get<Ts>()... );
    }

    template <typename ClassType, size_t Which = 0>
    typename std::enable_if<
      std::is_same<ClassType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value &&
        ( Which == 0 ),
      bool>::type
      isLinked() const VULKAN_HPP_NOEXCEPT
    {
      return true;
    }

    template <typename ClassType, size_t Which = 0>
    typename std::enable_if<
      !std::is_same<ClassType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value ||
        ( Which != 0 ),
      bool>::type
      isLinked() const VULKAN_HPP_NOEXCEPT
    {
      static_assert( IsPartOfStructureChain<ClassType, ChainElements...>::valid,
                     "Can't unlink Structure that's not part of this StructureChain!" );
      return isLinked( reinterpret_cast<VkBaseInStructure const *>( &get<ClassType, Which>() ) );
    }

    template <typename ClassType, size_t Which = 0>
    typename std::enable_if<
      !std::is_same<ClassType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value ||
        ( Which != 0 ),
      void>::type relink() VULKAN_HPP_NOEXCEPT
    {
      static_assert( IsPartOfStructureChain<ClassType, ChainElements...>::valid,
                     "Can't relink Structure that's not part of this StructureChain!" );
      auto pNext = reinterpret_cast<VkBaseInStructure *>( &get<ClassType, Which>() );
      VULKAN_HPP_ASSERT( !isLinked( pNext ) );
      auto & headElement = std::get<0>( static_cast<std::tuple<ChainElements...> &>( *this ) );
      pNext->pNext       = reinterpret_cast<VkBaseInStructure const *>( headElement.pNext );
      headElement.pNext  = pNext;
    }

    template <typename ClassType, size_t Which = 0>
    typename std::enable_if<
      !std::is_same<ClassType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value ||
        ( Which != 0 ),
      void>::type unlink() VULKAN_HPP_NOEXCEPT
    {
      static_assert( IsPartOfStructureChain<ClassType, ChainElements...>::valid,
                     "Can't unlink Structure that's not part of this StructureChain!" );
      unlink( reinterpret_cast<VkBaseOutStructure const *>( &get<ClassType, Which>() ) );
    }

  private:
    template <int Index, typename T, int Which, typename, class First, class... Types>
    struct ChainElementIndex : ChainElementIndex<Index + 1, T, Which, void, Types...>
    {};

    template <int Index, typename T, int Which, class First, class... Types>
    struct ChainElementIndex<Index,
                             T,
                             Which,
                             typename std::enable_if<!std::is_same<T, First>::value, void>::type,
                             First,
                             Types...> : ChainElementIndex<Index + 1, T, Which, void, Types...>
    {};

    template <int Index, typename T, int Which, class First, class... Types>
    struct ChainElementIndex<Index,
                             T,
                             Which,
                             typename std::enable_if<std::is_same<T, First>::value, void>::type,
                             First,
                             Types...> : ChainElementIndex<Index + 1, T, Which - 1, void, Types...>
    {};

    template <int Index, typename T, class First, class... Types>
    struct ChainElementIndex<Index,
                             T,
                             0,
                             typename std::enable_if<std::is_same<T, First>::value, void>::type,
                             First,
                             Types...> : std::integral_constant<int, Index>
    {};

    bool isLinked( VkBaseInStructure const * pNext ) const VULKAN_HPP_NOEXCEPT
    {
      VkBaseInStructure const * elementPtr = reinterpret_cast<VkBaseInStructure const *>(
        &std::get<0>( static_cast<std::tuple<ChainElements...> const &>( *this ) ) );
      while ( elementPtr )
      {
        if ( elementPtr->pNext == pNext )
        {
          return true;
        }
        elementPtr = elementPtr->pNext;
      }
      return false;
    }

    template <size_t Index>
    typename std::enable_if<Index != 0, void>::type link() VULKAN_HPP_NOEXCEPT
    {
      auto & x = std::get<Index - 1>( static_cast<std::tuple<ChainElements...> &>( *this ) );
      x.pNext  = &std::get<Index>( static_cast<std::tuple<ChainElements...> &>( *this ) );
      link<Index - 1>();
    }

    template <size_t Index>
    typename std::enable_if<Index == 0, void>::type link() VULKAN_HPP_NOEXCEPT
    {}

    void unlink( VkBaseOutStructure const * pNext ) VULKAN_HPP_NOEXCEPT
    {
      VkBaseOutStructure * elementPtr =
        reinterpret_cast<VkBaseOutStructure *>( &std::get<0>( static_cast<std::tuple<ChainElements...> &>( *this ) ) );
      while ( elementPtr && ( elementPtr->pNext != pNext ) )
      {
        elementPtr = elementPtr->pNext;
      }
      if ( elementPtr )
      {
        elementPtr->pNext = pNext->pNext;
      }
      else
      {
        VULKAN_HPP_ASSERT( false );  // fires, if the ClassType member has already been unlinked !
      }
    }
  };
)";

  static const std::string classUniqueHandle = R"(
#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
  template <typename Type, typename Dispatch>
  class UniqueHandleTraits;

  template <typename Type, typename Dispatch>
  class UniqueHandle : public UniqueHandleTraits<Type, Dispatch>::deleter
  {
  private:
    using Deleter = typename UniqueHandleTraits<Type, Dispatch>::deleter;

  public:
    using element_type = Type;

    UniqueHandle()
      : Deleter()
      , m_value()
    {}

    explicit UniqueHandle( Type const & value, Deleter const & deleter = Deleter() ) VULKAN_HPP_NOEXCEPT
      : Deleter( deleter )
      , m_value( value )
    {}

    UniqueHandle( UniqueHandle const & ) = delete;

    UniqueHandle( UniqueHandle && other ) VULKAN_HPP_NOEXCEPT
      : Deleter( std::move( static_cast<Deleter &>( other ) ) )
      , m_value( other.release() )
    {}

    ~UniqueHandle() VULKAN_HPP_NOEXCEPT
    {
      if ( m_value )
      {
        this->destroy( m_value );
      }
    }

    UniqueHandle & operator=( UniqueHandle const & ) = delete;

    UniqueHandle & operator=( UniqueHandle && other ) VULKAN_HPP_NOEXCEPT
    {
      reset( other.release() );
      *static_cast<Deleter *>( this ) = std::move( static_cast<Deleter &>( other ) );
      return *this;
    }

    explicit operator bool() const VULKAN_HPP_NOEXCEPT
    {
      return m_value.operator bool();
    }

    Type const * operator->() const VULKAN_HPP_NOEXCEPT
    {
      return &m_value;
    }

    Type * operator->() VULKAN_HPP_NOEXCEPT
    {
      return &m_value;
    }

    Type const & operator*() const VULKAN_HPP_NOEXCEPT
    {
      return m_value;
    }

    Type & operator*() VULKAN_HPP_NOEXCEPT
    {
      return m_value;
    }

    const Type & get() const VULKAN_HPP_NOEXCEPT
    {
      return m_value;
    }

    Type & get() VULKAN_HPP_NOEXCEPT
    {
      return m_value;
    }

    void reset( Type const & value = Type() ) VULKAN_HPP_NOEXCEPT
    {
      if ( m_value != value )
      {
        if ( m_value )
        {
          this->destroy( m_value );
        }
        m_value = value;
      }
    }

    Type release() VULKAN_HPP_NOEXCEPT
    {
      Type value = m_value;
      m_value    = nullptr;
      return value;
    }

    void swap( UniqueHandle<Type, Dispatch> & rhs ) VULKAN_HPP_NOEXCEPT
    {
      std::swap( m_value, rhs.m_value );
      std::swap( static_cast<Deleter &>( *this ), static_cast<Deleter &>( rhs ) );
    }

  private:
    Type m_value;
  };

  template <typename UniqueType>
  VULKAN_HPP_INLINE std::vector<typename UniqueType::element_type>
                    uniqueToRaw( std::vector<UniqueType> const & handles )
  {
    std::vector<typename UniqueType::element_type> newBuffer( handles.size() );
    std::transform(
      handles.begin(), handles.end(), newBuffer.begin(), []( UniqueType const & handle ) { return handle.get(); } );
    return newBuffer;
  }

  template <typename Type, typename Dispatch>
  VULKAN_HPP_INLINE void swap( UniqueHandle<Type, Dispatch> & lhs,
                               UniqueHandle<Type, Dispatch> & rhs ) VULKAN_HPP_NOEXCEPT
  {
    lhs.swap( rhs );
  }
#endif
)";

  static const std::string defines = R"(
// <tuple> includes <sys/sysmacros.h> through some other header
// this results in major(x) being resolved to gnu_dev_major(x)
// which is an expression in a constructor initializer list.
#if defined( major )
#  undef major
#endif
#if defined( minor )
#  undef minor
#endif

// Windows defines MemoryBarrier which is deprecated and collides
// with the VULKAN_HPP_NAMESPACE::MemoryBarrier struct.
#if defined( MemoryBarrier )
#  undef MemoryBarrier
#endif

#if !defined( VULKAN_HPP_HAS_UNRESTRICTED_UNIONS )
#  if defined( __clang__ )
#    if __has_feature( cxx_unrestricted_unions )
#      define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
#    endif
#  elif defined( __GNUC__ )
#    define GCC_VERSION ( __GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__ )
#    if 40600 <= GCC_VERSION
#      define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
#    endif
#  elif defined( _MSC_VER )
#    if 1900 <= _MSC_VER
#      define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
#    endif
#  endif
#endif

#if !defined( VULKAN_HPP_INLINE )
#  if defined( __clang__ )
#    if __has_attribute( always_inline )
#      define VULKAN_HPP_INLINE __attribute__( ( always_inline ) ) __inline__
#    else
#      define VULKAN_HPP_INLINE inline
#    endif
#  elif defined( __GNUC__ )
#    define VULKAN_HPP_INLINE __attribute__( ( always_inline ) ) __inline__
#  elif defined( _MSC_VER )
#    define VULKAN_HPP_INLINE inline
#  else
#    define VULKAN_HPP_INLINE inline
#  endif
#endif

#if defined( VULKAN_HPP_TYPESAFE_CONVERSION )
#  define VULKAN_HPP_TYPESAFE_EXPLICIT
#else
#  define VULKAN_HPP_TYPESAFE_EXPLICIT explicit
#endif

#if defined( __cpp_constexpr )
#  define VULKAN_HPP_CONSTEXPR constexpr
#  if __cpp_constexpr >= 201304
#    define VULKAN_HPP_CONSTEXPR_14 constexpr
#  else
#    define VULKAN_HPP_CONSTEXPR_14
#  endif
#  define VULKAN_HPP_CONST_OR_CONSTEXPR constexpr
#else
#  define VULKAN_HPP_CONSTEXPR
#  define VULKAN_HPP_CONSTEXPR_14
#  define VULKAN_HPP_CONST_OR_CONSTEXPR const
#endif

#if !defined( VULKAN_HPP_NOEXCEPT )
#  if defined( _MSC_VER ) && ( _MSC_VER <= 1800 )
#    define VULKAN_HPP_NOEXCEPT
#  else
#    define VULKAN_HPP_NOEXCEPT     noexcept
#    define VULKAN_HPP_HAS_NOEXCEPT 1
#    if defined( VULKAN_HPP_NO_EXCEPTIONS )
#      define VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS noexcept
#    else
#      define VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS
#    endif
#  endif
#endif

#if 14 <= VULKAN_HPP_CPP_VERSION
#  define VULKAN_HPP_DEPRECATED( msg ) [[deprecated( msg )]]
#else
#  define VULKAN_HPP_DEPRECATED( msg )
#endif

#if ( 17 <= VULKAN_HPP_CPP_VERSION ) && !defined( VULKAN_HPP_NO_NODISCARD_WARNINGS )
#  define VULKAN_HPP_NODISCARD [[nodiscard]]
#  if defined( VULKAN_HPP_NO_EXCEPTIONS )
#    define VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS [[nodiscard]]
#  else
#    define VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS
#  endif
#else
#  define VULKAN_HPP_NODISCARD
#  define VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS
#endif

#if !defined( VULKAN_HPP_NAMESPACE )
#  define VULKAN_HPP_NAMESPACE vk
#endif

#define VULKAN_HPP_STRINGIFY2( text ) #text
#define VULKAN_HPP_STRINGIFY( text )  VULKAN_HPP_STRINGIFY2( text )
#define VULKAN_HPP_NAMESPACE_STRING   VULKAN_HPP_STRINGIFY( VULKAN_HPP_NAMESPACE )
)";

  static const std::string exceptions = R"(
  class ErrorCategoryImpl : public std::error_category
  {
    public:
    virtual const char* name() const VULKAN_HPP_NOEXCEPT override { return VULKAN_HPP_NAMESPACE_STRING"::Result"; }
    virtual std::string message(int ev) const override { return to_string(static_cast<Result>(ev)); }
  };

  class Error
  {
    public:
    Error() VULKAN_HPP_NOEXCEPT = default;
    Error(const Error&) VULKAN_HPP_NOEXCEPT = default;
    virtual ~Error() VULKAN_HPP_NOEXCEPT = default;

    virtual const char* what() const VULKAN_HPP_NOEXCEPT = 0;
  };

  class LogicError : public Error, public std::logic_error
  {
    public:
    explicit LogicError( const std::string& what )
      : Error(), std::logic_error(what) {}
    explicit LogicError( char const * what )
      : Error(), std::logic_error(what) {}

    virtual const char* what() const VULKAN_HPP_NOEXCEPT { return std::logic_error::what(); }
  };

  class SystemError : public Error, public std::system_error
  {
    public:
    SystemError( std::error_code ec )
      : Error(), std::system_error(ec) {}
    SystemError( std::error_code ec, std::string const & what )
      : Error(), std::system_error(ec, what) {}
    SystemError( std::error_code ec, char const * what )
      : Error(), std::system_error(ec, what) {}
    SystemError( int ev, std::error_category const & ecat )
      : Error(), std::system_error(ev, ecat) {}
    SystemError( int ev, std::error_category const & ecat, std::string const & what)
      : Error(), std::system_error(ev, ecat, what) {}
    SystemError( int ev, std::error_category const & ecat, char const * what)
      : Error(), std::system_error(ev, ecat, what) {}

    virtual const char* what() const VULKAN_HPP_NOEXCEPT { return std::system_error::what(); }
  };

  VULKAN_HPP_INLINE const std::error_category& errorCategory() VULKAN_HPP_NOEXCEPT
  {
    static ErrorCategoryImpl instance;
    return instance;
  }

  VULKAN_HPP_INLINE std::error_code make_error_code(Result e) VULKAN_HPP_NOEXCEPT
  {
    return std::error_code(static_cast<int>(e), errorCategory());
  }

  VULKAN_HPP_INLINE std::error_condition make_error_condition(Result e) VULKAN_HPP_NOEXCEPT
  {
    return std::error_condition(static_cast<int>(e), errorCategory());
  }
)";

  static const std::string includes = R"(
#ifndef VULKAN_HPP
#define VULKAN_HPP

#if defined( _MSVC_LANG )
#  define VULKAN_HPP_CPLUSPLUS _MSVC_LANG
#else
#  define VULKAN_HPP_CPLUSPLUS __cplusplus
#endif

#if 201703L < VULKAN_HPP_CPLUSPLUS
#  define VULKAN_HPP_CPP_VERSION 20
#elif 201402L < VULKAN_HPP_CPLUSPLUS
#  define VULKAN_HPP_CPP_VERSION 17
#elif 201103L < VULKAN_HPP_CPLUSPLUS
#  define VULKAN_HPP_CPP_VERSION 14
#elif 199711L < VULKAN_HPP_CPLUSPLUS
#  define VULKAN_HPP_CPP_VERSION 11
#else
#  error "vulkan.hpp needs at least c++ standard version 11"
#endif

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <sstream>
#include <string>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <vulkan/vulkan.h>

#if 17 <= VULKAN_HPP_CPP_VERSION
#  include <string_view>
#endif

#if defined( VULKAN_HPP_DISABLE_ENHANCED_MODE )
#  if !defined( VULKAN_HPP_NO_SMART_HANDLE )
#    define VULKAN_HPP_NO_SMART_HANDLE
#  endif
#else
#  include <memory>
#  include <vector>
#endif

#if !defined( VULKAN_HPP_ASSERT )
#  include <cassert>
#  define VULKAN_HPP_ASSERT assert
#endif

#if !defined( VULKAN_HPP_ASSERT_ON_RESULT )
#  define VULKAN_HPP_ASSERT_ON_RESULT VULKAN_HPP_ASSERT
#endif

#if !defined( VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL )
#  define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 1
#endif

#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL == 1
#  if defined( __linux__ ) || defined( __APPLE__ ) || defined( __QNXNTO__ ) || defined(__Fuchsia__)
#    include <dlfcn.h>
#  elif defined( _WIN32 )
typedef struct HINSTANCE__ * HINSTANCE;
#    if defined( _WIN64 )
typedef int64_t( __stdcall * FARPROC )();
#    else
typedef int( __stdcall * FARPROC )();
#    endif
extern "C" __declspec( dllimport ) HINSTANCE __stdcall LoadLibraryA( char const * lpLibFileName );
extern "C" __declspec( dllimport ) int __stdcall FreeLibrary( HINSTANCE hLibModule );
extern "C" __declspec( dllimport ) FARPROC __stdcall GetProcAddress( HINSTANCE hModule, const char * lpProcName );
#  endif
#endif

#if !defined( __has_include )
#  define __has_include( x ) false
#endif

#if ( 201711 <= __cpp_impl_three_way_comparison ) && __has_include( <compare> ) && !defined( VULKAN_HPP_NO_SPACESHIP_OPERATOR )
#  define VULKAN_HPP_HAS_SPACESHIP_OPERATOR
#endif
#if defined( VULKAN_HPP_HAS_SPACESHIP_OPERATOR )
#  include <compare>
#endif

)";

  static const std::string is_error_code_enum = R"(
#ifndef VULKAN_HPP_NO_EXCEPTIONS
namespace std
{
  template <>
  struct is_error_code_enum<VULKAN_HPP_NAMESPACE::Result> : public true_type
  {};
}
#endif
)";

  static const std::string structResultValue = R"(
  template <typename T> void ignore(T const &) VULKAN_HPP_NOEXCEPT {}

  template <typename T>
  struct ResultValue
  {
#ifdef VULKAN_HPP_HAS_NOEXCEPT
    ResultValue( Result r, T & v ) VULKAN_HPP_NOEXCEPT(VULKAN_HPP_NOEXCEPT(T(v)))
#else
    ResultValue( Result r, T & v )
#endif
      : result( r )
      , value( v )
    {}

#ifdef VULKAN_HPP_HAS_NOEXCEPT
    ResultValue( Result r, T && v ) VULKAN_HPP_NOEXCEPT(VULKAN_HPP_NOEXCEPT(T(std::move(v))))
#else
    ResultValue( Result r, T && v )
#endif
      : result( r )
      , value( std::move( v ) )
    {}

    Result  result;
    T       value;

    operator std::tuple<Result&, T&>() VULKAN_HPP_NOEXCEPT { return std::tuple<Result&, T&>(result, value); }

#if !defined(VULKAN_HPP_DISABLE_IMPLICIT_RESULT_VALUE_CAST)
    VULKAN_HPP_DEPRECATED("Implicit-cast operators on vk::ResultValue are deprecated. Explicitly access the value as member of ResultValue.")
    operator T const & () const & VULKAN_HPP_NOEXCEPT
    {
      return value;
    }

    VULKAN_HPP_DEPRECATED("Implicit-cast operators on vk::ResultValue are deprecated. Explicitly access the value as member of ResultValue.")
    operator T& () & VULKAN_HPP_NOEXCEPT
    {
      return value;
    }

    VULKAN_HPP_DEPRECATED("Implicit-cast operators on vk::ResultValue are deprecated. Explicitly access the value as member of ResultValue.")
    operator T const && () const && VULKAN_HPP_NOEXCEPT
    {
      return std::move( value );
    }

    VULKAN_HPP_DEPRECATED("Implicit-cast operators on vk::ResultValue are deprecated. Explicitly access the value as member of ResultValue.")
    operator T&& () && VULKAN_HPP_NOEXCEPT
    {
      return std::move( value );
    }
#endif
  };

#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
  template <typename Type, typename Dispatch>
  struct ResultValue<UniqueHandle<Type, Dispatch>>
  {
#ifdef VULKAN_HPP_HAS_NOEXCEPT
    ResultValue(Result r, UniqueHandle<Type, Dispatch> && v) VULKAN_HPP_NOEXCEPT
#else
    ResultValue(Result r, UniqueHandle<Type, Dispatch> && v)
#endif
      : result(r)
      , value(std::move(v))
    {}

    std::tuple<Result, UniqueHandle<Type, Dispatch>> asTuple()
    {
      return std::make_tuple( result, std::move( value ) );
    }

#  if !defined(VULKAN_HPP_DISABLE_IMPLICIT_RESULT_VALUE_CAST)
    VULKAN_HPP_DEPRECATED("Implicit-cast operators on vk::ResultValue are deprecated. Explicitly access the value as member of ResultValue.")
    operator UniqueHandle<Type, Dispatch>& () & VULKAN_HPP_NOEXCEPT
    {
      return value;
    }

    VULKAN_HPP_DEPRECATED("Implicit-cast operators on vk::ResultValue are deprecated. Explicitly access the value as member of ResultValue.")
    operator UniqueHandle<Type, Dispatch>() VULKAN_HPP_NOEXCEPT
    {
      return std::move(value);
    }
#  endif

    Result                        result;
    UniqueHandle<Type, Dispatch>  value;
  };

  template <typename Type, typename Dispatch>
  struct ResultValue<std::vector<UniqueHandle<Type, Dispatch>>>
  {
#  ifdef VULKAN_HPP_HAS_NOEXCEPT
    ResultValue( Result r, std::vector<UniqueHandle<Type, Dispatch>> && v ) VULKAN_HPP_NOEXCEPT
#  else
    ResultValue( Result r, std::vector<UniqueHandle<Type, Dispatch>> && v )
#  endif
      : result( r )
      , value( std::move( v ) )
    {}

    std::tuple<Result, std::vector<UniqueHandle<Type, Dispatch>>> asTuple()
    {
      return std::make_tuple( result, std::move( value ) );
    }

    Result                                    result;
    std::vector<UniqueHandle<Type, Dispatch>> value;

#  if !defined(VULKAN_HPP_DISABLE_IMPLICIT_RESULT_VALUE_CAST)
    VULKAN_HPP_DEPRECATED(
      "Implicit-cast operators on vk::ResultValue are deprecated. Explicitly access the value as member of ResultValue." )
    operator std::tuple<Result &, std::vector<UniqueHandle<Type, Dispatch>> &>() VULKAN_HPP_NOEXCEPT
    {
      return std::tuple<Result &, std::vector<UniqueHandle<Type, Dispatch>> &>( result, value );
    }
#  endif
  };
#endif

  template <typename T>
  struct ResultValueType
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    typedef ResultValue<T>  type;
#else
    typedef T               type;
#endif
  };

  template <>
  struct ResultValueType<void>
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    typedef Result type;
#else
    typedef void   type;
#endif
  };

  VULKAN_HPP_INLINE ResultValueType<void>::type createResultValue( Result result, char const * message )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore(message);
    VULKAN_HPP_ASSERT_ON_RESULT( result == Result::eSuccess );
    return result;
#else
    if ( result != Result::eSuccess )
    {
      throwResultException( result, message );
    }
#endif
  }

  template <typename T>
  VULKAN_HPP_INLINE typename ResultValueType<T>::type createResultValue( Result result, T & data, char const * message )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore(message);
    VULKAN_HPP_ASSERT_ON_RESULT( result == Result::eSuccess );
    return ResultValue<T>( result, std::move( data ) );
#else
    if ( result != Result::eSuccess )
    {
      throwResultException( result, message );
    }
    return std::move( data );
#endif
  }

  VULKAN_HPP_INLINE Result createResultValue( Result result, char const * message, std::initializer_list<Result> successCodes )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore(message);
    ignore(successCodes);   // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
    VULKAN_HPP_ASSERT_ON_RESULT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#else
    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
    {
      throwResultException( result, message );
    }
#endif
    return result;
  }

  template <typename T>
  VULKAN_HPP_INLINE ResultValue<T> createResultValue( Result result, T & data, char const * message, std::initializer_list<Result> successCodes )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore(message);
    ignore(successCodes);   // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
    VULKAN_HPP_ASSERT_ON_RESULT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#else
    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
    {
      throwResultException( result, message );
    }
#endif
    return ResultValue<T>( result, std::move( data ) );
  }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename T, typename D>
  VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<T,D>>::type createResultValue( Result result, T & data, char const * message, typename UniqueHandleTraits<T,D>::deleter const & deleter )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore(message);
    VULKAN_HPP_ASSERT_ON_RESULT( result == Result::eSuccess );
    return ResultValue<UniqueHandle<T,D>>( result, UniqueHandle<T,D>(data, deleter ) );
#else
    if ( result != Result::eSuccess )
    {
      throwResultException( result, message );
    }
    return UniqueHandle<T,D>(data, deleter );
#endif
  }

  template <typename T, typename D>
  VULKAN_HPP_INLINE ResultValue<UniqueHandle<T, D>>
                    createResultValue( Result                                             result,
                                       T &                                                data,
                                       char const *                                       message,
                                       std::initializer_list<Result>                      successCodes,
                                       typename UniqueHandleTraits<T, D>::deleter const & deleter )
  {
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore( message );
    ignore(successCodes);   // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
    VULKAN_HPP_ASSERT_ON_RESULT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#  else
    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
    {
      throwResultException( result, message );
    }
#  endif
    return ResultValue<UniqueHandle<T, D>>( result, UniqueHandle<T, D>( data, deleter ) );
  }

  template <typename T, typename D>
  VULKAN_HPP_INLINE typename ResultValueType<std::vector<UniqueHandle<T, D>>>::type
    createResultValue( Result result, std::vector<UniqueHandle<T, D>> && data, char const * message )
  {
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore( message );
    VULKAN_HPP_ASSERT_ON_RESULT( result == Result::eSuccess );
    return ResultValue<std::vector<UniqueHandle<T, D>>>( result, std::move( data ) );
#  else
    if ( result != Result::eSuccess )
    {
      throwResultException( result, message );
    }
    return std::move( data );
#  endif
  }

  template <typename T, typename D>
  VULKAN_HPP_INLINE ResultValue<std::vector<UniqueHandle<T, D>>>
                    createResultValue( Result                             result,
                                       std::vector<UniqueHandle<T, D>> && data,
                                       char const *                       message,
                                       std::initializer_list<Result>      successCodes )
  {
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore( message );
    ignore(successCodes);   // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
    VULKAN_HPP_ASSERT_ON_RESULT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#  else
    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
    {
      throwResultException( result, message );
    }
#  endif
    return ResultValue<std::vector<UniqueHandle<T, D>>>( result, std::move( data ) );
  }
#endif
)";

  static const std::string typeTraits = R"(
  template <typename EnumType, EnumType value>
  struct CppType
  {};

  template <typename Type>
  struct isVulkanHandleType
  {
    static VULKAN_HPP_CONST_OR_CONSTEXPR bool value = false;
  };
)";

  try
  {
    tinyxml2::XMLDocument doc;

    std::string filename = ( argc == 1 ) ? VK_SPEC : argv[1];

    std::cout << "VulkanHppGenerator: Loading " << filename << std::endl;
    tinyxml2::XMLError error = doc.LoadFile( filename.c_str() );
    if ( error != tinyxml2::XML_SUCCESS )
    {
      std::cout << "VulkanHppGenerator: failed to load file " << filename << " with error <" << to_string( error )
                << ">" << std::endl;
      return -1;
    }

    std::cout << "VulkanHppGenerator: Parsing " << filename << std::endl;
    VulkanHppGenerator generator( doc );

    std::cout << "VulkanHppGenerator: Generating " << VULKAN_HPP_FILE << std::endl;
    std::string         str;
    static const size_t estimatedLength = 4 * 1024 * 1024;
    str.reserve( estimatedLength );
    str += generator.getVulkanLicenseHeader() + includes + "\n";
    str += "static_assert( VK_HEADER_VERSION == " + generator.getVersion() +
           " , \"Wrong VK_HEADER_VERSION!\" );\n"
           "\n";
    str +=
      "// 32-bit vulkan is not typesafe for handles, so don't allow copy constructors on this platform by default.\n"
      "// To enable this feature on 32-bit platforms please define VULKAN_HPP_TYPESAFE_CONVERSION\n" +
      generator.getTypesafeCheck() +
      "\n"
      "#  if !defined( VULKAN_HPP_TYPESAFE_CONVERSION )\n"
      "#    define VULKAN_HPP_TYPESAFE_CONVERSION\n"
      "#  endif\n"
      "#endif\n";
    str += defines + "\n" + "namespace VULKAN_HPP_NAMESPACE\n" + "{" + classArrayProxy + classArrayWrapper +
           classFlags + classOptional + classStructureChain + classUniqueHandle;
    generator.appendDispatchLoaderStatic( str );
    generator.appendDispatchLoaderDefault( str );
    str += classObjectDestroy + classObjectFree + classObjectRelease + classPoolFree + "\n";
    generator.appendBaseTypes( str );
    str += typeTraits;
    generator.appendEnums( str );
    generator.appendIndexTypeTraits( str );
    generator.appendBitmasks( str );
    str += "} // namespace VULKAN_HPP_NAMESPACE\n" + is_error_code_enum + "\n" + "namespace VULKAN_HPP_NAMESPACE\n" +
           "{\n" + "#ifndef VULKAN_HPP_NO_EXCEPTIONS" + exceptions;
    generator.appendResultExceptions( str );
    generator.appendThrowExceptions( str );
    str += "#endif\n" + structResultValue;
    generator.appendStructs( str );
    generator.appendHandles( str );
    generator.appendHandlesCommandDefinitions( str );
    generator.appendStructureChainValidation( str );
    generator.appendDispatchLoaderDynamic( str );
    str +=
      "} // namespace VULKAN_HPP_NAMESPACE\n"
      "\n"
      "namespace std\n"
      "{\n";
    generator.appendHashStructures( str );
    str +=
      "} // namespace std\n"
      "#endif\n";

    std::ofstream ofs( VULKAN_HPP_FILE );
    assert( !ofs.fail() );
    ofs << str;
    ofs.close();

#if defined( CLANG_FORMAT_EXECUTABLE )
    int ret = std::system( "\"" CLANG_FORMAT_EXECUTABLE "\" --version" );
    assert( ret == 0 );
    std::cout << "VulkanHppGenerator: Formatting " << VULKAN_HPP_FILE << " using clang-format..." << std::endl;
    ret = std::system( "\"" CLANG_FORMAT_EXECUTABLE "\" -i --style=file " VULKAN_HPP_FILE );
    if ( ret != 0 )
    {
      std::cout << "VulkanHppGenerator: failed to format file " << VULKAN_HPP_FILE << " with error <" << ret << ">\n";
      return -1;
    }
#endif

    std::cout << "VulkanHppGenerator: Generating " << VULKAN_RAII_HPP_FILE << std::endl;
    str.clear();
    str = generator.getVulkanLicenseHeader() + R"(
#ifndef VULKAN_RAII_HPP
#  define VULKAN_RAII_HPP

#include <vulkan/vulkan.hpp>

#if !defined( VULKAN_HPP_RAII_NAMESPACE )
#  define VULKAN_HPP_RAII_NAMESPACE raii
#endif

namespace VULKAN_HPP_NAMESPACE
{
  namespace VULKAN_HPP_RAII_NAMESPACE
  {
#if !defined( VULKAN_HPP_DISABLE_ENHANCED_MODE ) && !defined(VULKAN_HPP_NO_EXCEPTIONS)

    template <class T, class U = T>
    VULKAN_HPP_CONSTEXPR_14 VULKAN_HPP_INLINE T exchange( T & obj, U && newValue )
    {
#  if ( 14 <= VULKAN_HPP_CPP_VERSION )
      return std::exchange<T>( obj, std::forward<U>( newValue ) );
#  else
      T oldValue = std::move( obj );
      obj        = std::forward<U>( newValue );
      return oldValue;
#  endif
    }

)";

    generator.appendRAIIDispatchers( str );

    std::string raiiHandlesCommandDefinitions;
    generator.appendRAIIHandles( str, raiiHandlesCommandDefinitions );
    str += raiiHandlesCommandDefinitions;
    str += R"(
#endif
  } // namespace VULKAN_HPP_RAII_NAMESPACE
}   // namespace VULKAN_HPP_NAMESPACE
#endif
)";

    ofs.open( VULKAN_RAII_HPP_FILE );
    assert( !ofs.fail() );
    ofs << str;
    ofs.close();

#if defined( CLANG_FORMAT_EXECUTABLE )
    std::cout << "VulkanHppGenerator: Formatting " << VULKAN_RAII_HPP_FILE << " using clang-format..." << std::endl;
    ret = std::system( "\"" CLANG_FORMAT_EXECUTABLE "\" -i --style=file " VULKAN_RAII_HPP_FILE );
    if ( ret != 0 )
    {
      std::cout << "VulkanHppGenerator: failed to format file " << VULKAN_RAII_HPP_FILE << " with error <" << ret
                << ">\n";
      return -1;
    }
#else
    std::cout
      << "VulkanHppGenerator: could not find clang-format. The generated files will not be formatted accordingly.\n";
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
}
