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

#include <algorithm>
#include <cassert>
#include <exception>
#include <fstream>
#include <functional>
#include <iterator>
#include <regex>

void             appendArgumentCount( std::string &       str,
                                      size_t              vectorIndex,
                                      std::string const & vectorName,
                                      std::string const & counterName,
                                      size_t              returnParamIndex,
                                      size_t              templateParamIndex,
                                      bool                twoStep,
                                      bool                singular );
std::string      appendFunctionBodyEnhancedLocalReturnVariableSingular( std::string &       str,
                                                                        std::string const & indentation,
                                                                        std::string const & returnName,
                                                                        std::string const & typeName,
                                                                        bool                isStructureChain );
void             appendReinterpretCast( std::string &       str,
                                        bool                leadingConst,
                                        std::string const & type,
                                        bool                trailingPointerToConst );
void             appendTypesafeStuff( std::string & str, std::string const & typesafeCheck );
void             appendVersionCheck( std::string & str, std::string const & version );
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
void             cleanup( std::stringstream & ss );
std::string      constructArraySize( std::vector<std::string> const & sizes );
std::string      constructStandardArray( std::string const & type, std::vector<std::string> const & sizes );
std::string      createEnumValueName( std::string const & name,
                                      std::string const & prefix,
                                      std::string const & postfix,
                                      bool                bitmask,
                                      std::string const & tag );
std::string      createSuccessCode( std::string const & code, std::set<std::string> const & tags );
std::string      determineCommandName( std::string const & vulkanCommandName, std::string const & firstArgumentType );
std::set<size_t> determineSkippedParams( size_t returnParamIndex, std::map<size_t, size_t> const & vectorParamIndices );
bool             determineStructureChaining( std::string const &                        structType,
                                             std::set<std::string> const &              extendedStructs,
                                             std::map<std::string, std::string> const & structureAliases );
std::string      extractTag( int line, std::string const & name, std::set<std::string> const & tags );
std::string findTag( std::set<std::string> const & tags, std::string const & name, std::string const & postfix = "" );
std::pair<bool, std::string>       generateFunctionBodyStandardReturn( std::string const & returnType );
std::map<std::string, std::string> getAttributes( tinyxml2::XMLElement const * element );
template <typename ElementContainer>
std::vector<tinyxml2::XMLElement const *> getChildElements( ElementContainer const * element );
std::string getEnumPostfix( std::string const & name, std::set<std::string> const & tags, std::string & prefix );
std::string getEnumPrefix( int line, std::string const & name, bool bitmask );
std::string readTypePostfix( tinyxml2::XMLNode const * node );
std::string readTypePrefix( tinyxml2::XMLNode const * node );
std::string replaceWithMap( std::string const & input, std::map<std::string, std::string> replacements );
std::string startLowerCase( std::string const & input );
std::string startUpperCase( std::string const & input );
std::string stripPostfix( std::string const & value, std::string const & postfix );
std::string stripPluralS( std::string const & name );
std::string stripPrefix( std::string const & value, std::string const & prefix );
std::string toCamelCase( std::string const & value );
std::string toString( std::vector<std::string> const & strings );
std::string toUpperCase( std::string const & name );
std::vector<std::string> tokenize( std::string const & tokenString, std::string const & separator );
std::string              trim( std::string const & input );
std::string              trimEnd( std::string const & input );
std::string              trimStars( std::string const & input );
void                     warn( bool condition, int line, std::string const & message );

const std::set<std::string> ignoreLens = { "null-terminated",
                                           R"(latexmath:[\lceil{\mathit{rasterizationSamples} \over 32}\rceil])",
                                           "2*VK_UUID_SIZE",
                                           "2*ename:VK_UUID_SIZE" };

void appendArgumentCount( std::string &       str,
                          size_t              vectorIndex,
                          std::string const & vectorName,
                          std::string const & counterName,
                          size_t              returnParamIndex,
                          size_t              templateParamIndex,
                          bool                twoStep,
                          bool                singular )
{
  // this parameter is a count parameter for a vector parameter
  if ( ( returnParamIndex == vectorIndex ) && twoStep )
  {
    // the corresponding vector parameter is the return parameter and it's a two-step algorithm
    // -> use the pointer to a local variable named like the counter parameter without leading 'p'
    assert( ( counterName[0] == 'p' ) && isupper( counterName[1] ) );
    str += "&" + startLowerCase( stripPrefix( counterName, "p" ) );
  }
  else
  {
    // the corresponding vector parameter is not the return parameter, or it's not a two-step algorithm
    if ( singular )
    {
      // for the singular version, the count is just 1.
      str += "1 ";
    }
    else
    {
      // for the non-singular version, the count is the size of the vector parameter
      // -> use the vector parameter name without leading 'p' to get the size (in number of elements, not in bytes)
      assert( vectorName[0] == 'p' );
      str += startLowerCase( stripPrefix( vectorName, "p" ) ) + ".size() ";
    }
    if ( templateParamIndex == vectorIndex )
    {
      // if the vector parameter is templatized -> multiply by the size of that type to get the size in bytes
      str += "* sizeof( T ) ";
    }
  }
}

std::string appendFunctionBodyEnhancedLocalReturnVariableSingular( std::string &       str,
                                                                   std::string const & indentation,
                                                                   std::string const & returnName,
                                                                   std::string const & typeName,
                                                                   bool                isStructureChain )
{
  std::string strippedReturnName = stripPluralS( returnName );
  if ( isStructureChain )
  {
    // For StructureChains use the template parameters
    str += "StructureChain<X, Y, Z...> structureChain;\n" + indentation + "  " + typeName + "& " + strippedReturnName +
           " = structureChain.template get<" + typeName + ">()";
    strippedReturnName = "structureChain";
  }
  else
  {
    // in singular case, just use the return parameters pure type for the return variable
    str += typeName + " " + strippedReturnName;
  }
  return strippedReturnName;
}

void appendReinterpretCast( std::string &       str,
                            bool                leadingConst,
                            std::string const & type,
                            bool                trailingPointerToConst )
{
  str += "reinterpret_cast<";
  if ( leadingConst )
  {
    str += "const ";
  }
  str += type;
  if ( trailingPointerToConst )
  {
    str += "* const";
  }
  str += "*>";
}

void appendTypesafeStuff( std::string & str, std::string const & typesafeCheck )
{
  str +=
    "// 32-bit vulkan is not typesafe for handles, so don't allow copy constructors on this platform by default.\n"
    "// To enable this feature on 32-bit platforms please define VULKAN_HPP_TYPESAFE_CONVERSION\n" +
    typesafeCheck +
    "\n"
    "# if !defined( VULKAN_HPP_TYPESAFE_CONVERSION )\n"
    "#  define VULKAN_HPP_TYPESAFE_CONVERSION\n"
    "# endif\n"
    "#endif\n";
}

void appendVersionCheck( std::string & str, std::string const & version )
{
  str += "static_assert( VK_HEADER_VERSION == " + version +
         " , \"Wrong VK_HEADER_VERSION!\" );\n"
         "\n";
}

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
    throw std::runtime_error( "Spec error on line " + std::to_string( line ) + ": " + message );
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

void cleanup( std::string & str )
{
  std::map<std::string, std::string> replacements = { { "\n\n\n", "\n\n" },
                                                      { "{\n\n", "{\n" },
                                                      { "\n\n  }", "\n  }" } };
  for ( auto const & repl : replacements )
  {
    std::string::size_type pos = str.find( repl.first );
    while ( pos != std::string::npos )
    {
      str.replace( pos, repl.first.length(), repl.second );
      pos = str.find( repl.first, pos );
    }
  }
}

std::string constructArraySize( std::vector<std::string> const & sizes )
{
  std::string arraySize;
  for ( auto const & s : sizes )
  {
    arraySize += s + " * ";
  }
  return arraySize.substr( 0, arraySize.length() - 3 );
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

std::string determineCommandName( std::string const & vulkanCommandName, std::string const & firstArgumentType )
{
  std::string commandName( startLowerCase( stripPrefix( vulkanCommandName, "vk" ) ) );
  std::string searchName = stripPrefix( firstArgumentType, "Vk" );
  size_t      pos        = commandName.find( searchName );
  if ( ( pos == std::string::npos ) && isupper( searchName[0] ) )
  {
    searchName[0] = static_cast<char>( tolower( searchName[0] ) );
    pos           = commandName.find( searchName );
  }
  if ( pos != std::string::npos )
  {
    commandName.erase( pos, searchName.length() );
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
  return commandName;
}

std::set<size_t> determineSkippedParams( size_t returnParamIndex, std::map<size_t, size_t> const & vectorParamIndices )
{
  std::set<size_t> skippedParams;

  // the size-parameters of vector parameters are not explicitly used in the enhanced API
  std::for_each(
    vectorParamIndices.begin(), vectorParamIndices.end(), [&skippedParams]( std::pair<size_t, size_t> const & vp ) {
      if ( vp.second != INVALID_INDEX )
        skippedParams.insert( vp.second );
    } );

  // and the return parameter is also skipped
  if ( returnParamIndex != INVALID_INDEX )
  {
    skippedParams.insert( returnParamIndex );
  }
  return skippedParams;
}

bool determineStructureChaining( std::string const &                        structType,
                                 std::set<std::string> const &              extendedStructs,
                                 std::map<std::string, std::string> const & structureAliases )
{
  bool isStructureChained = ( extendedStructs.find( structType ) != extendedStructs.end() );
  if ( !isStructureChained )
  {
    auto aliasIt = structureAliases.find( structType );
    if ( ( aliasIt != structureAliases.end() ) )
    {
      isStructureChained = ( extendedStructs.find( aliasIt->second ) != extendedStructs.end() );
    }
  }
  return isStructureChained;
}

std::string findTag( std::set<std::string> const & tags, std::string const & name, std::string const & postfix )
{
  auto tagIt = std::find_if(
    tags.begin(), tags.end(), [&name, &postfix]( std::string const & t ) { return endsWith( name, t + postfix ); } );
  return ( tagIt != tags.end() ) ? *tagIt : "";
}

std::pair<bool, std::string> generateFunctionBodyStandardReturn( std::string const & returnType )
{
  bool        castReturn = false;
  std::string ret;
  if ( returnType != "void" )
  {
    // there's something to return...
    ret = "return ";

    castReturn = beginsWith( returnType, "Vk" );
    if ( castReturn )
    {
      // the return-type is a vulkan type -> need to cast to VULKAN_HPP_NAMESPACE-type
      ret += "static_cast<" + stripPrefix( returnType, "Vk" ) + ">( ";
    }
  }
  return std::make_pair( castReturn, ret );
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

std::string getEnumPrefix( int line, std::string const & name, bool bitmask )
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
    prefix = toUpperCase( name.substr( 0, pos ) ) + "_";
  }
  else
  {
    // for a non-bitmask enum, convert the name to upper case
    prefix = toUpperCase( name ) + "_";
  }
  return prefix;
}

std::string extractTag( int line, std::string const & name, std::set<std::string> const & tags )
{
  // extract the tag from the name, which is supposed to look like VK_<tag>_<other>
  size_t tagStart = name.find( '_' );
  check( tagStart != std::string::npos, line, "name <" + name + "> is missing an underscore '_'" );
  size_t tagEnd = name.find( '_', tagStart + 1 );
  check( tagEnd != std::string::npos, line, "name <" + name + "> is missing an underscore '_'" );
  std::string tag = name.substr( tagStart + 1, tagEnd - tagStart - 1 );
  check( tags.find( tag ) != tags.end(), line, "name <" + name + "> is using an unknown tag <" + tag + ">" );
  return tag;
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
  ;
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

std::string replaceWithMap( std::string const & input, std::map<std::string, std::string> replacements )
{
  // This will match ${someVariable} and contain someVariable in match group 1
  std::regex re( R"(\$\{([^\}]+)\})" );
  auto       it  = std::sregex_iterator( input.begin(), input.end(), re );
  auto       end = std::sregex_iterator();

  // No match, just return the original string
  if ( it == end )
  {
    return input;
  }

  std::string result = "";
  while ( it != end )
  {
    std::smatch match         = *it;
    auto        itReplacement = replacements.find( match[1].str() );
    assert( itReplacement != replacements.end() );

    result +=
      match.prefix().str() + ( ( itReplacement != replacements.end() ) ? itReplacement->second : match[0].str() );
    ++it;

    // we've passed the last match. Append the rest of the orignal string
    if ( it == end )
    {
      result += match.suffix().str();
    }
  }
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
  assert( pos != std::string::npos );
  strippedName.erase( pos, 1 );
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

std::string toString( std::vector<std::string> const & strings )
{
  std::string str;
  if ( !strings.empty() )
  {
    str = strings[0];
    for ( size_t i = 1; i < strings.size(); i++ )
    {
      str += ", " + strings[i];
    }
  }
  return str;
}

std::string toUpperCase( std::string const & name )
{
  std::string convertedName;
  convertedName.reserve( name.size() );

  bool lowerOrDigit = false;
  for ( auto c : name )
  {
    if ( islower( c ) || isdigit( c ) )
    {
      lowerOrDigit = true;
    }
    else if ( lowerOrDigit )
    {
      convertedName.push_back( '_' );
      lowerOrDigit = false;
    }
    convertedName.push_back( static_cast<char>( toupper( c ) ) );
  }

  return convertedName;
}

std::vector<std::string> tokenize( std::string const & tokenString, std::string const & separator )
{
  std::vector<std::string> tokens;
  size_t                   start = 0, end;
  do
  {
    end = tokenString.find( separator, start );
    if ( start != end )
    {
      tokens.push_back( trim( tokenString.substr( start, end - start ) ) );
    }
    start = end + separator.length();
  } while ( end != std::string::npos );
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
    std::cerr << "Spec warning on line " << std::to_string( line ) << " " << message << "!" << std::endl;
  }
}

VulkanHppGenerator::VulkanHppGenerator( tinyxml2::XMLDocument const & document )
{
  m_handles.insert( std::make_pair(
    "", HandleData( {}, 0 ) ) );  // insert the default "handle" without class (for createInstance, and such)

  int                                       line     = document.GetLineNum();
  std::vector<tinyxml2::XMLElement const *> elements = getChildElements( &document );
  checkElements( line, elements, { { "registry", true } } );
  check( elements.size() == 1,
         line,
         "encountered " + std::to_string( elements.size() ) + " elments named <registry> but only one is allowed" );
  readRegistry( elements[0] );
  checkCorrectness();
}

void VulkanHppGenerator::appendArgumentPlainType( std::string & str, ParamData const & paramData ) const
{
  // this parameter is just a plain type
  if ( !paramData.type.postfix.empty() )
  {
    assert( paramData.type.postfix.back() == '*' );
    // it's a pointer
    std::string parameterName = startLowerCase( stripPrefix( paramData.name, "p" ) );
    if ( paramData.type.prefix.find( "const" ) != std::string::npos )
    {
      // it's a const pointer
      if ( paramData.type.type == "char" )
      {
        // it's a const pointer to char -> it's a string -> get the data via c_str()
        str += parameterName + ( paramData.optional ? ( " ? " + parameterName + "->c_str() : nullptr" ) : ".c_str()" );
      }
      else
      {
        // it's const pointer to something else -> just use the name
        assert( !paramData.optional );
        str += paramData.name;
      }
    }
    else
    {
      // it's a non-const pointer, and char is the only type that occurs -> use the address of the parameter
      assert( paramData.type.type.find( "char" ) == std::string::npos );
      str += "&" + parameterName;
    }
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
                                          bool                             singular,
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
      appendArgumentVector( str,
                            it->first,
                            commandData.params[it->first],
                            returnParamIndex,
                            templateParamIndex,
                            twoStep,
                            firstCall,
                            singular );
    }
    else
    {
      it = find_if( vectorParamIndices.begin(), vectorParamIndices.end(), [i]( std::pair<size_t, size_t> const & vpi ) {
        return vpi.second == i;
      } );
      if ( it != vectorParamIndices.end() )
      {
        appendArgumentCount( str,
                             it->first,
                             commandData.params[it->first].name,
                             commandData.params[it->second].name,
                             returnParamIndex,
                             templateParamIndex,
                             twoStep,
                             singular );
      }
      else if ( beginsWith( commandData.params[i].type.type, "Vk" ) )
      {
        appendArgumentVulkanType( str, commandData.params[i] );
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
                                               bool              firstCall,
                                               bool              singular ) const
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
      appendReinterpretCast( str,
                             paramData.type.prefix.find( "const" ) == 0,
                             paramData.type.type,
                             paramData.type.postfix.rfind( "* const" ) != std::string::npos );
      str += "( " + ( singular ? ( "&" + stripPluralS( parameterName ) ) : ( parameterName + ".data()" ) ) + " )";
    }
    else if ( paramData.type.type == "char" )
    {
      // the parameter is a vector to char -> it might be optional
      // besides that, the parameter now is a std::string -> get the pointer via c_str()
      str += parameterName + ( paramData.optional ? ( " ? " + parameterName + "->c_str() : nullptr" ) : ".c_str()" );
    }
    else
    {
      // this parameter is just a vetor -> get the pointer to its data
      str += parameterName + ".data()";
    }
  }
}

void VulkanHppGenerator::appendArgumentVulkanType( std::string & str, ParamData const & paramData ) const
{
  // this parameter is a vulkan type
  if ( !paramData.type.postfix.empty() || !paramData.arraySizes.empty() )
  {
    assert( ( paramData.type.postfix.empty() || ( paramData.type.postfix.back() == '*' ) ) &&
            ( paramData.arraySizes.empty() || ( paramData.arraySizes.size() == 1 ) ) );
    // it's a pointer -> needs a reinterpret cast to the vulkan type
    std::string parameterName = startLowerCase( stripPrefix( paramData.name, "p" ) );
    appendReinterpretCast(
      str, paramData.type.prefix.find( "const" ) != std::string::npos, paramData.type.type, false );
    str += "( ";
    if ( paramData.optional )
    {
      // for an optional parameter, we need also a static_cast from optional type to const-pointer to pure type
      str += "static_cast<const " + stripPrefix( paramData.type.type, "Vk" ) + "*>( " + parameterName + " )";
    }
    else
    {
      // other parameters can just use the pointer
      str += ( paramData.arraySizes.empty() ? "&" : "" ) + parameterName;
    }
    str += " )";
  }
  else
  {
    // a non-pointer parameter needs a static_cast from VULKAN_HPP_NAMESPACE-type to vulkan type
    str += "static_cast<" + paramData.type.type + ">( " + paramData.name + " )";
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
  for ( auto const & bitmask : m_bitmasks )
  {
    auto bitmaskBits = m_enums.find( bitmask.second.requirements );
    bool hasBits     = ( bitmaskBits != m_enums.end() );
    check( bitmask.second.requirements.empty() || hasBits,
           bitmask.second.xmlLine,
           "bitmask <" + bitmask.first + "> references the undefined requires <" + bitmask.second.requirements + ">" );

    std::string strippedBitmaskName = stripPrefix( bitmask.first, "Vk" );
    std::string strippedEnumName    = hasBits ? stripPrefix( bitmaskBits->first, "Vk" ) : "";

    std::string enter, leave;
    std::tie( enter, leave ) = generateProtection( bitmask.first, !bitmask.second.alias.empty() );

    str += "\n" + enter;
    appendBitmask( str,
                   strippedBitmaskName,
                   bitmask.second.type,
                   bitmask.second.alias,
                   strippedEnumName,
                   hasBits ? bitmaskBits->second.values : std::vector<EnumValueData>() );
    appendBitmaskToStringFunction(
      str, strippedBitmaskName, strippedEnumName, hasBits ? bitmaskBits->second.values : std::vector<EnumValueData>() );
    str += leave;
  }
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
      const std::string templateString = R"x(  enum class ${enumName} : ${bitmaskType}
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
  str +=
    "\n"
    "  using " +
    bitmaskName + " = Flags<" + name + ">;\n";

  if ( !enumValues.empty() )
  {
    std::string allFlags;
    for ( auto const & value : enumValues )
    {
      if ( !allFlags.empty() )
      {
        allFlags += " | ";
      }
      allFlags += bitmaskType + "(" + enumName + "::" + value.vkValue + ")";
    }

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
    str += "\n    return \"{}\";\n";
  }
  else
  {
    // 'or' together all the bits in the value
    str +=
      "\n"
      "    if ( !value ) return \"{}\";\n"
      "    std::string result;\n";
    for ( auto const & evd : enumValues )
    {
      if ( evd.singleBit )
      {
        str +=
          "\n"
          "    if ( value & " +
          enumName + "::" + evd.vkValue + " ) result += \"" + evd.vkValue.substr( 1 ) + " | \";";
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
                                     bool                             firstCall,
                                     bool                             singular ) const
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
                   singular,
                   commandData.handle.empty() ? 0 : 1,
                   commandData.params.size() );
  str += " )";
}

void VulkanHppGenerator::appendCommand( std::string &       str,
                                        std::string const & indentation,
                                        std::string const & name,
                                        CommandData const & commandData,
                                        bool                definition ) const
{
  bool                     twoStep            = isTwoStepAlgorithm( commandData.params );
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndices( commandData.params );

  size_t returnParamIndex = determineReturnParamIndex( commandData, vectorParamIndices, twoStep );
  bool   isStructureChain =
    ( returnParamIndex != INVALID_INDEX ) &&
    determineStructureChaining( commandData.params[returnParamIndex].type.type, m_extendedStructs, m_structureAliases );
  std::string enhancedReturnType =
    determineEnhancedReturnType( commandData,
                                 returnParamIndex,
                                 vectorParamIndices,
                                 false );  // get the enhanced return type without structureChain

  size_t templateParamIndex = determineTemplateParamIndex( commandData.params, vectorParamIndices );

  // first create the standard version of the function
  std::string standard;
  appendFunction( standard,
                  indentation,
                  name,
                  commandData,
                  returnParamIndex,
                  templateParamIndex,
                  vectorParamIndices,
                  twoStep,
                  enhancedReturnType,
                  definition,
                  false,
                  false,
                  false,
                  false,
                  false );

  // then the enhanced version, composed by up to eight parts
  std::string enhanced;
  appendFunction( enhanced,
                  indentation,
                  name,
                  commandData,
                  returnParamIndex,
                  templateParamIndex,
                  vectorParamIndices,
                  twoStep,
                  enhancedReturnType,
                  definition,
                  true,
                  false,
                  false,
                  false,
                  false );

  if ( enhancedReturnType.find( "Allocator" ) != std::string::npos )
  {
    appendFunction( enhanced,
                    indentation,
                    name,
                    commandData,
                    returnParamIndex,
                    templateParamIndex,
                    vectorParamIndices,
                    twoStep,
                    enhancedReturnType,
                    definition,
                    true,
                    false,
                    false,
                    false,
                    true );
  }

  if ( isStructureChain )
  {
    std::string enhancedReturnTypeWithStructureChain =
      determineEnhancedReturnType( commandData, returnParamIndex, vectorParamIndices, true );
    appendFunction( enhanced,
                    indentation,
                    name,
                    commandData,
                    returnParamIndex,
                    templateParamIndex,
                    vectorParamIndices,
                    twoStep,
                    enhancedReturnTypeWithStructureChain,
                    definition,
                    true,
                    false,
                    false,
                    true,
                    false );

    if ( enhancedReturnTypeWithStructureChain.find( "Allocator" ) != std::string::npos )
    {
      appendFunction( enhanced,
                      indentation,
                      name,
                      commandData,
                      returnParamIndex,
                      templateParamIndex,
                      vectorParamIndices,
                      twoStep,
                      enhancedReturnTypeWithStructureChain,
                      definition,
                      true,
                      false,
                      false,
                      true,
                      true );
    }
  }

  // then a singular version, if a sized vector would be returned
  std::map<size_t, size_t>::const_iterator returnVector = vectorParamIndices.find( returnParamIndex );
  bool singular = ( returnVector != vectorParamIndices.end() ) && ( returnVector->second != INVALID_INDEX ) &&
                  ( commandData.params[returnVector->first].type.type != "void" ) &&
                  ( commandData.params[returnVector->second].type.postfix.empty() ||
                    ( commandData.params[returnVector->second].type.postfix.back() != '*' ) );
  if ( singular )
  {
    appendFunction( enhanced,
                    indentation,
                    name,
                    commandData,
                    returnParamIndex,
                    templateParamIndex,
                    vectorParamIndices,
                    twoStep,
                    enhancedReturnType,
                    definition,
                    true,
                    true,
                    false,
                    false,
                    false );
  }

  // special handling for createDevice and createInstance !
  bool specialWriteUnique = ( name == "vkCreateDevice" ) || ( name == "vkCreateInstance" );

  // and then the same for the Unique* versions (a deleteCommand is available for the commandData's class, and the
  // function starts with 'allocate' or 'create')
  auto handleIt = m_handles.find( commandData.handle );
  assert( handleIt != m_handles.end() );
  if ( ( !handleIt->second.deleteCommand.empty() || specialWriteUnique ) &&
       ( ( name.substr( 2, 8 ) == "Allocate" ) || ( name.substr( 2, 6 ) == "Create" ) ||
         ( ( name.substr( 2, 8 ) == "Register" ) && ( returnParamIndex + 1 == commandData.params.size() ) ) ) )
  {
    enhanced += "#ifndef VULKAN_HPP_NO_SMART_HANDLE\n";
    appendFunction( enhanced,
                    indentation,
                    name,
                    commandData,
                    returnParamIndex,
                    templateParamIndex,
                    vectorParamIndices,
                    twoStep,
                    enhancedReturnType,
                    definition,
                    true,
                    false,
                    true,
                    false,
                    false );

    if ( enhancedReturnType.find( "Allocator" ) != std::string::npos )
    {
      appendFunction( enhanced,
                      indentation,
                      name,
                      commandData,
                      returnParamIndex,
                      templateParamIndex,
                      vectorParamIndices,
                      twoStep,
                      enhancedReturnType,
                      definition,
                      true,
                      false,
                      true,
                      false,
                      true );
    }

    if ( singular )
    {
      appendFunction( enhanced,
                      indentation,
                      name,
                      commandData,
                      returnParamIndex,
                      templateParamIndex,
                      vectorParamIndices,
                      twoStep,
                      enhancedReturnType,
                      definition,
                      true,
                      true,
                      true,
                      false,
                      false );
    }
    enhanced += "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/\n";
  }

  // and append one or both of them
  if ( standard == enhanced )
  {
    // standard and enhanced string are equal -> just use one of them and we're done
    str += standard;
  }
  else
  {
    // standard and enhanced string differ -> use both, wrapping the enhanced by !VULKAN_HPP_DISABLE_ENHANCED_MODE
    // determine the argument list of that standard, and compare it with that of the enhanced
    // if they are equal -> need to have just one; if they differ -> need to have both
    size_t standardStart = standard.find( '(' );
    size_t standardCount = standard.find( ')', standardStart ) - standardStart;
    size_t enhancedStart = enhanced.find( '(' );
    bool   unchangedInterface =
      ( standard.substr( standardStart, standardCount ) == enhanced.substr( enhancedStart, standardCount ) );
    if ( unchangedInterface )
    {
      str += "#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE\n";
    }
    str += standard + ( unchangedInterface ? "#else" : "#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE" ) + "\n" + enhanced +
           "#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/\n";
  }
}

void VulkanHppGenerator::appendDispatchLoaderDynamic( std::string & str )
{
  str += R"(
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
  class DynamicLoader
  {
  public:
#  ifdef VULKAN_HPP_NO_EXCEPTIONS
    DynamicLoader( std::string const & vulkanLibraryName = {} ) VULKAN_HPP_NOEXCEPT : m_success( false )
#  else
    DynamicLoader( std::string const & vulkanLibraryName = {} ) : m_success( false )
#  endif
    {
      if ( !vulkanLibraryName.empty() )
      {
#  if defined( __linux__ ) || defined( __APPLE__ )
        m_library = dlopen( vulkanLibraryName.c_str(), RTLD_NOW | RTLD_LOCAL );
#  elif defined( _WIN32 )
        m_library = ::LoadLibraryA( vulkanLibraryName.c_str() );
#  else
#    error unsupported platform
#  endif
      }
      else
      {
#  if defined( __linux__ )
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

      m_success = (m_library != nullptr);
#ifndef VULKAN_HPP_NO_EXCEPTIONS
      if ( !m_success )
      {
        // NOTE there should be an InitializationFailedError, but msvc insists on the symbol does not exist within the scope of this function.
        throw std::runtime_error( "Failed to load vulkan library!" );
      }
#endif
    }

    DynamicLoader( DynamicLoader const& ) = delete;

    DynamicLoader( DynamicLoader && other ) VULKAN_HPP_NOEXCEPT
      : m_success(other.m_success)
      , m_library(other.m_library)
    {
      other.m_library = nullptr;
    }

    DynamicLoader &operator=( DynamicLoader const& ) = delete;

    DynamicLoader &operator=( DynamicLoader && other ) VULKAN_HPP_NOEXCEPT
    {
      m_success = other.m_success;
      std::swap(m_library, other.m_library);
      return *this;
    }

    ~DynamicLoader() VULKAN_HPP_NOEXCEPT
    {
      if ( m_library )
      {
#  if defined( __linux__ ) || defined( __APPLE__ )
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
#  if defined( __linux__ ) || defined( __APPLE__ )
      return (T)dlsym( m_library, function );
#  elif defined( _WIN32 )
      return (T)::GetProcAddress( m_library, function );
#  else
#    error unsupported platform
#  endif
    }

    bool success() const VULKAN_HPP_NOEXCEPT { return m_success; }

  private:
    bool m_success;
#  if defined( __linux__ ) || defined( __APPLE__ )
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
)";

  for ( auto const & handle : m_handles )
  {
    for ( auto const & command : handle.second.commands )
    {
      auto commandIt = m_commands.find( command );
      assert( commandIt != m_commands.end() );
      std::string enter, leave;
      std::tie( enter, leave ) = generateProtection( commandIt->second.feature, commandIt->second.extensions );
      str += enter + "    PFN_" + commandIt->first + " " + commandIt->first + " = 0;\n" + leave;
    }
  }

  std::string emptyFunctions;
  std::string strDeviceFunctions;
  std::string strDeviceFunctionsInstance;
  std::string strInstanceFunctions;
  for ( auto const & handle : m_handles )
  {
    for ( auto const & command : handle.second.commands )
    {
      if ( ( command != "vkGetInstanceProcAddr" ) )
      {
        auto commandIt = m_commands.find( command );
        assert( commandIt != m_commands.end() );

        std::string enter, leave;
        std::tie( enter, leave ) = generateProtection( commandIt->second.feature, commandIt->second.extensions );

        if ( handle.first.empty() )
        {
          assert( commandIt->second.alias.empty() );
          emptyFunctions += enter + "      " + commandIt->first + " = PFN_" + commandIt->first +
                            "( vkGetInstanceProcAddr( NULL, \"" + commandIt->first + "\" ) );\n" + leave;
        }
        else if ( !commandIt->second.params.empty() &&
                  m_handles.find( commandIt->second.params[0].type.type ) != m_handles.end() &&
                  commandIt->second.params[0].type.type != "VkInstance" &&
                  commandIt->second.params[0].type.type != "VkPhysicalDevice" )
        {
          strDeviceFunctions += enter + "      " + commandIt->first + " = PFN_" + commandIt->first +
                                "( vkGetDeviceProcAddr( device, \"" + commandIt->first + "\" ) );\n" + leave;

          strDeviceFunctionsInstance += enter + "      " + commandIt->first + " = PFN_" + commandIt->first +
                                        "( vkGetInstanceProcAddr( instance, \"" + commandIt->first + "\" ) );\n" +
                                        leave;
        }
        else
        {
          strInstanceFunctions += enter + "      " + commandIt->first + " = PFN_" + commandIt->first +
                                  "( vkGetInstanceProcAddr( instance, \"" + commandIt->first + "\" ) );\n" + leave;
        }
      }
    }
  }

  // append initialization function to fetch function pointers
  str += R"(
  public:
    DispatchLoaderDynamic() VULKAN_HPP_NOEXCEPT = default;

#if !defined(VK_NO_PROTOTYPES)
    // This interface is designed to be used for per-device function pointers in combination with a linked vulkan library.
    template <typename DynamicLoader>
    void init(VULKAN_HPP_NAMESPACE::Instance const& instance, VULKAN_HPP_NAMESPACE::Device const& device, DynamicLoader const& dl) VULKAN_HPP_NOEXCEPT
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
    void init(VULKAN_HPP_NAMESPACE::Instance const& instance, VULKAN_HPP_NAMESPACE::Device const& device) VULKAN_HPP_NOEXCEPT
    {
      static DynamicLoader dl;
      init(instance, device, dl);
    }
#endif // !defined(VK_NO_PROTOTYPES)

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
    DispatchLoaderDynamic( VkInstance instance, PFN_vkGetInstanceProcAddr getInstanceProcAddr, VkDevice device = VK_NULL_HANDLE, PFN_vkGetDeviceProcAddr getDeviceProcAddr = nullptr ) VULKAN_HPP_NOEXCEPT
    {
      init( instance, getInstanceProcAddr, device, getDeviceProcAddr );
    }

    // This interface does not require a linked vulkan library.
    void init( VkInstance instance, PFN_vkGetInstanceProcAddr getInstanceProcAddr, VkDevice device = VK_NULL_HANDLE, PFN_vkGetDeviceProcAddr /*getDeviceProcAddr*/ = nullptr ) VULKAN_HPP_NOEXCEPT
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

  str += strInstanceFunctions;
  str += strDeviceFunctionsInstance;
  str += "    }\n\n";
  str += "    void init( VULKAN_HPP_NAMESPACE::Device deviceCpp ) VULKAN_HPP_NOEXCEPT\n    {\n";
  str += "      VkDevice device = static_cast<VkDevice>(deviceCpp);\n";
  str += strDeviceFunctions;
  str += R"(    }
  };

)";
}

void VulkanHppGenerator::appendDispatchLoaderStatic( std::string & str )
{
  str += R"(
#if !defined(VK_NO_PROTOTYPES)
  class DispatchLoaderStatic
  {
  public:)";

  for ( auto const & handle : m_handles )
  {
    for ( auto const & command : handle.second.commands )
    {
      auto commandIt = m_commands.find( command );
      assert( commandIt != m_commands.end() );

      std::string parameterList, parameters;
      bool        firstParam = true;
      for ( auto param : commandIt->second.params )
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
      std::string commandName = stripPrefix( commandIt->first, "vk" );

      str += "\n";
      std::string enter, leave;
      std::tie( enter, leave ) = generateProtection( commandIt->second.feature, commandIt->second.extensions );
      str += enter + "    " + commandIt->second.returnType + " vk" + commandName + "( " + parameterList +
             " ) const VULKAN_HPP_NOEXCEPT\n"
             "    {\n"
             "      return ::vk" +
             commandName + "( " + parameters +
             " );\n"
             "    }\n" +
             leave;
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
# if defined(VK_NO_PROTOTYPES)
#  define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
# else
#  define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 0
# endif
#endif

#if defined(_WIN32) && defined(VULKAN_HPP_STORAGE_SHARED)
#  ifdef VULKAN_HPP_STORAGE_SHARED_EXPORT
#    define VULKAN_HPP_STORAGE_API __declspec( dllexport )
#  else
#    define VULKAN_HPP_STORAGE_API __declspec( dllimport )
#  endif
#else
#  define VULKAN_HPP_STORAGE_API
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
)";
}

void VulkanHppGenerator::appendEnum( std::string & str, std::pair<std::string, EnumData> const & enumData ) const
{
  str += "  enum class " + stripPrefix( enumData.first, "Vk" );
  if ( enumData.second.isBitmask )
  {
    auto bitmaskIt = std::find_if( m_bitmasks.begin(), m_bitmasks.end(), [&enumData]( auto const & bitmask ) {
      return bitmask.second.requirements == enumData.first;
    } );
    assert( bitmaskIt != m_bitmasks.end() );
    str += " : " + bitmaskIt->first;
  }
  str +=
    "\n"
    "  {";

  bool first = true;
  for ( auto const & value : enumData.second.values )
  {
    if ( !first )
    {
      str += ",";
    }
    str += "\n    " + value.vkValue + " = " + value.vulkanValue;
    first = false;
  }
  for ( auto const & alias : enumData.second.aliases )
  {
    // make sure to only list alias values that differ from all non-alias values
    if ( std::find_if(
           enumData.second.values.begin(), enumData.second.values.end(), [&alias]( EnumValueData const & evd ) {
             return alias.second.second == evd.vkValue;
           } ) == enumData.second.values.end() )
    {
      if ( !first )
      {
        str += ",";
      }
      str += "\n    " + alias.second.second + " = " + alias.first;
      first = false;
    }
  }
  if ( !first )
  {
    str += "\n  ";
  }

  str += "};\n";

  if ( !enumData.second.alias.empty() )
  {
    str +=
      "  using " + stripPrefix( enumData.second.alias, "Vk" ) + " = " + stripPrefix( enumData.first, "Vk" ) + ";\n";
  }
}

void VulkanHppGenerator::appendEnums( std::string & str ) const
{
  for ( auto const & e : m_enums )
  {
    std::string enter, leave;
    std::tie( enter, leave ) = generateProtection( e.first, !e.second.alias.empty() );

    str += "\n" + enter;
    appendEnum( str, e );
    appendEnumToString( str, e );
    if ( e.first == "VkObjectType" )
    {
      str += R"(
  template<ObjectType value>
  struct cpp_type
  {};
)";
    }
    str += leave;
  }
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
    int count = std::stoi( arraySizes[0] );
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
    for ( auto const & value : enumData.second.values )
    {
      str += "      case " + enumName + "::" + value.vkValue + " : return \"" + value.vkValue.substr( 1 ) + "\";\n";
    }
    str +=
      "      default: return \"invalid\";\n"
      "    }\n";
  }

  str += "  }\n";
}

void VulkanHppGenerator::appendForwardDeclarations( std::string & str ) const
{
  str += "\n";
  for ( auto const & structure : m_structures )
  {
    std::string enter, leave;
    std::tie( enter, leave ) = generateProtection( structure.first, !structure.second.aliases.empty() );

    str += enter + "  " + ( structure.second.isUnion ? "union" : "struct" ) + " " +
           stripPrefix( structure.first, "Vk" ) + ";\n";
    for ( std::string const & alias : structure.second.aliases )
    {
      str += "  using " + stripPrefix( alias, "Vk" ) + " = " + stripPrefix( structure.first, "Vk" ) + ";\n";
    }
    str += leave;
  }
}

bool needsMultiVectorSizeCheck( size_t returnParamIndex, std::map<size_t, size_t> const & vectorParamIndices )
{
  for ( std::map<size_t, size_t>::const_iterator it0 = vectorParamIndices.begin(); it0 != vectorParamIndices.end();
        ++it0 )
  {
    if ( it0->first != returnParamIndex )
    {
      for ( std::map<size_t, size_t>::const_iterator it1 = std::next( it0 ); it1 != vectorParamIndices.end(); ++it1 )
      {
        if ( ( it1->first != returnParamIndex ) && ( it0->second == it1->second ) )
        {
          return true;
        }
      }
    }
  }
  return false;
}

void VulkanHppGenerator::appendFunction( std::string &                    str,
                                         std::string const &              indentation,
                                         std::string const &              name,
                                         CommandData const &              commandData,
                                         size_t                           returnParamIndex,
                                         size_t                           templateParamIndex,
                                         std::map<size_t, size_t> const & vectorParamIndices,
                                         bool                             twoStep,
                                         std::string const &              enhancedReturnType,
                                         bool                             definition,
                                         bool                             enhanced,
                                         bool                             singular,
                                         bool                             unique,
                                         bool                             isStructureChain,
                                         bool                             withAllocator ) const
{
  appendFunctionHeaderTemplate( str,
                                indentation,
                                returnParamIndex,
                                templateParamIndex,
                                enhancedReturnType,
                                enhanced,
                                singular,
                                unique,
                                !definition,
                                isStructureChain );

  str += indentation;

  if ( ( 1 < commandData.successCodes.size() ) || ( !enhanced && ( 1 < commandData.errorCodes.size() ) ) )
  {
    str += "VULKAN_HPP_NODISCARD ";
  }
  else if ( enhanced && ( 1 < commandData.errorCodes.size() ) )
  {
    str += "VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS ";
  }

  if ( definition )
  {
    str += "VULKAN_HPP_INLINE ";
  }

  appendFunctionHeaderReturnType( str,
                                  commandData,
                                  returnParamIndex,
                                  vectorParamIndices,
                                  enhancedReturnType,
                                  enhanced,
                                  twoStep,
                                  singular,
                                  unique,
                                  isStructureChain );

  if ( definition && !commandData.handle.empty() )
  {
    str += stripPrefix( commandData.handle, "Vk" ) + "::";
  }

  // append the function header name
  std::string commandName = determineCommandName( name, commandData.params[0].type.type );
  str += ( singular ? stripPluralS( commandName ) : commandName );
  if ( unique )
  {
    str += "Unique";
  }

  appendFunctionHeaderArguments( str,
                                 commandData,
                                 returnParamIndex,
                                 templateParamIndex,
                                 vectorParamIndices,
                                 enhanced,
                                 singular,
                                 !definition,
                                 withAllocator );

  // Any function that originally does not return VkResult can be marked noexcept,
  // if it is enhanced it must not include anything with an Allocator or needs size checks on multiple vectors
  bool hasAllocator = enhancedReturnType.find( "Allocator" ) != std::string::npos;
  if ( !enhanced ||
       ( commandData.returnType != "VkResult" &&
         !( enhanced && ( hasAllocator || needsMultiVectorSizeCheck( returnParamIndex, vectorParamIndices ) ) ) ) )
  {
    str += " VULKAN_HPP_NOEXCEPT";
  }

  str += std::string( definition ? "" : ";" ) + "\n";

  if ( definition )
  {
    // append the function body
    str += indentation + "{\n";
    if ( enhanced )
    {
      appendFunctionBodyEnhanced( str,
                                  indentation,
                                  name,
                                  commandData,
                                  returnParamIndex,
                                  templateParamIndex,
                                  vectorParamIndices,
                                  twoStep,
                                  enhancedReturnType,
                                  singular,
                                  unique,
                                  isStructureChain,
                                  withAllocator );
    }
    else
    {
      appendFunctionBodyStandard( str, indentation, name, commandData );
    }
    str += indentation + "}\n";
  }
}

void VulkanHppGenerator::appendFunctionBodyEnhanced( std::string &                    str,
                                                     std::string const &              indentation,
                                                     std::string const &              name,
                                                     CommandData const &              commandData,
                                                     size_t                           returnParamIndex,
                                                     size_t                           templateParamIndex,
                                                     std::map<size_t, size_t> const & vectorParamIndices,
                                                     bool                             twoStep,
                                                     std::string const &              enhancedReturnType,
                                                     bool                             singular,
                                                     bool                             unique,
                                                     bool                             isStructureChain,
                                                     bool                             withAllocator ) const
{
  if ( unique && !singular &&
       ( vectorParamIndices.find( returnParamIndex ) != vectorParamIndices.end() ) )  // returns a vector of UniqueStuff
  {
    appendFunctionBodyEnhancedVectorOfUniqueHandles( str,
                                                     indentation,
                                                     name,
                                                     commandData,
                                                     returnParamIndex,
                                                     templateParamIndex,
                                                     vectorParamIndices,
                                                     twoStep,
                                                     singular,
                                                     withAllocator );
  }
  else if ( isStructureChain && ( vectorParamIndices.find( returnParamIndex ) != vectorParamIndices.end() ) )
  {
    appendFunctionBodyEnhancedVectorOfStructureChain(
      str, indentation, name, commandData, returnParamIndex, vectorParamIndices, withAllocator );
  }
  else
  {
    if ( 1 < vectorParamIndices.size() )
    {
      appendFunctionBodyEnhancedMultiVectorSizeCheck(
        str, indentation, name, commandData, returnParamIndex, vectorParamIndices );
    }

    std::string returnName;
    if ( returnParamIndex != INVALID_INDEX )
    {
      returnName = appendFunctionBodyEnhancedLocalReturnVariable( str,
                                                                  indentation,
                                                                  commandData,
                                                                  returnParamIndex,
                                                                  vectorParamIndices,
                                                                  twoStep,
                                                                  enhancedReturnType,
                                                                  singular,
                                                                  isStructureChain,
                                                                  withAllocator );
    }

    if ( twoStep )
    {
      appendFunctionBodyEnhancedTwoStep( str,
                                         indentation,
                                         name,
                                         commandData,
                                         returnParamIndex,
                                         templateParamIndex,
                                         vectorParamIndices,
                                         singular,
                                         returnName );
    }
    else
    {
      appendFunctionBodyEnhancedSingleStep(
        str, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, singular );
    }

    if ( ( commandData.returnType == "VkResult" ) || !commandData.successCodes.empty() )
    {
      appendFunctionBodyEnhancedReturnResultValue(
        str, indentation, returnName, name, commandData, returnParamIndex, twoStep, singular, unique );
    }
    else if ( ( returnParamIndex != INVALID_INDEX ) &&
              ( stripPrefix( commandData.returnType, "Vk" ) != enhancedReturnType ) )
    {
      // for the other returning cases, when the return type is somhow enhanced, just return the local returnVariable
      str += indentation + "  return " + returnName + ";\n";
    }
  }
}

std::string VulkanHppGenerator::appendFunctionBodyEnhancedLocalReturnVariable(
  std::string &                    str,
  std::string const &              indentation,
  CommandData const &              commandData,
  size_t                           returnParamIndex,
  std::map<size_t, size_t> const & vectorParamIndices,
  bool                             twoStep,
  std::string const &              enhancedReturnType,
  bool                             singular,
  bool                             isStructureChain,
  bool                             withAllocator ) const
{
  std::string pureReturnType = stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" );
  std::string returnName     = startLowerCase( stripPrefix( commandData.params[returnParamIndex].name, "p" ) );

  // there is a returned parameter -> we need a local variable to hold that value
  if ( stripPrefix( commandData.returnType, "Vk" ) != enhancedReturnType )
  {
    // the returned parameter is somehow enhanced by us
    str += indentation + "  ";
    if ( singular )
    {
      returnName = appendFunctionBodyEnhancedLocalReturnVariableSingular(
        str, indentation, returnName, pureReturnType, isStructureChain );
    }
    else
    {
      // in non-singular case, use the enhanced type for the return variable (like vector<...>)
      if ( isStructureChain && vectorParamIndices.empty() )
      {
        // For StructureChains use the template parameters
        str += "StructureChain<X, Y, Z...> structureChain;\n" + indentation + "  " + enhancedReturnType + "& " +
               returnName + " = structureChain.template get<" + enhancedReturnType + ">()";
        returnName = "structureChain";
      }
      else
      {
        str += enhancedReturnType + " " + returnName;
      }

      std::map<size_t, size_t>::const_iterator vpiIt = vectorParamIndices.find( returnParamIndex );
      if ( vpiIt != vectorParamIndices.end() && !twoStep )
      {
        appendFunctionBodyEnhancedLocalReturnVariableVectorSize(
          str, commandData.params, *vpiIt, returnParamIndex, vectorParamIndices, withAllocator );
      }
      else if ( withAllocator )
      {
        str += "( vectorAllocator )";
      }
    }
    str += ";\n";
  }
  else
  {
    // the return parameter is not enhanced -> the type is supposed to be a Result and there are more than one success
    // codes!
    assert( ( commandData.returnType == "VkResult" ) && ( 1 < commandData.successCodes.size() ) );
    str += indentation + "  " + pureReturnType + " " + returnName + ";\n";
  }

  return returnName;
}

void VulkanHppGenerator::appendFunctionBodyEnhancedLocalReturnVariableVectorSize(
  std::string &                     str,
  std::vector<ParamData> const &    params,
  std::pair<size_t, size_t> const & vectorParamIndex,
  size_t                            returnParamIndex,
  std::map<size_t, size_t> const &  vectorParamIndices,
  bool                              withAllocator ) const
{
  // if the return parameter is a vector parameter, and not part of a two-step algorithm, initialize its size
  std::string size;
  if ( vectorParamIndex.second == INVALID_INDEX )
  {
    assert( !params[returnParamIndex].len.empty() );
    // the size of the vector is not given by an other parameter, but by some member of a parameter, described as
    // 'parameter->member'
    // -> replace the '->' by '.' and filter out the leading 'p' to access that value
    size       = startLowerCase( stripPrefix( params[returnParamIndex].len, "p" ) );
    size_t pos = size.find( "->" );
    // older versions of the vk.xml used the notation parameter::member !
    if ( pos == std::string::npos )
    {
      pos = size.find( "::" );
    }
    assert( pos != std::string::npos );
    size.replace( pos, 2, "." );
  }
  else
  {
    // the size of the vector is given by an other parameter
    // first check, if that size has become the size of some other vector parameter
    // -> look for it and get it's actual size
    for ( auto const & vpi : vectorParamIndices )
    {
      if ( ( vpi.first != vectorParamIndex.first ) && ( vpi.second == vectorParamIndex.second ) )
      {
        size = startLowerCase( stripPrefix( params[vpi.first].name, "p" ) ) + ".size()";
        break;
      }
    }
    if ( size.empty() )
    {
      // otherwise, just use that parameter
      size = params[vectorParamIndex.second].name;
    }
  }
  assert( !size.empty() );
  str += "( " + size + ( withAllocator ? ", vectorAllocator" : "" ) + " )";
}

void VulkanHppGenerator::appendFunctionBodyEnhancedMultiVectorSizeCheck(
  std::string &                    str,
  std::string const &              indentation,
  std::string const &              name,
  CommandData const &              commandData,
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
  std::string commandName = determineCommandName( name, commandData.params[0].type.type );
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
                { "className", commandData.handle },
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
                                                                      size_t              returnParamIndex,
                                                                      bool                twoStep,
                                                                      bool                singular,
                                                                      bool                unique ) const
{
  std::string type = ( returnParamIndex != INVALID_INDEX ) ? commandData.params[returnParamIndex].type.type : "";
  std::string returnVectorName = ( returnParamIndex != INVALID_INDEX )
                                   ? stripPostfix( stripPrefix( commandData.params[returnParamIndex].name, "p" ), "s" )
                                   : "";
  std::string commandName = determineCommandName( name, commandData.params[0].type.type );

  if ( commandData.returnType == "void" )
  {
    std::cerr << "warning: skipping appendFunctionBodyEnhancedReturnResultValue for function " << commandName
              << " because the returnType is void";
    return;
  }

  if ( unique )
  {
    // the unique version needs a Deleter object for destruction of the newly created stuff
    // get the DeleterData corresponding to the returned type
    // special handling for "createDevice", as Device is created from PhysicalDevice, but destroyed on its own
    bool noParent = commandData.handle.empty() || ( name == "vkCreateDevice" );
    str += "\n" + indentation + ( ( name == "vkAllocateMemory" ) ? "  ObjectFree<" : "  ObjectDestroy<" ) +
           ( noParent ? "NoParent" : stripPrefix( commandData.handle, "Vk" ) ) + ",Dispatch> deleter( " +
           ( noParent ? "" : "*this, " ) + "allocator, d );\n" + indentation + "  return createResultValue<" +
           stripPrefix( type, "Vk" ) + ",Dispatch>( result, ";
  }
  else
  {
    str += indentation + "  return createResultValue( result, ";
  }

  // if the return type is "Result" or there is at least one success code, create the Result/Value construct to return
  if ( returnParamIndex != INVALID_INDEX )
  {
    // if there's a return parameter, list it in the Result/Value constructor
    str += returnName + ", ";
  }

  // now the function name (with full namespace) as a string
  str += "VULKAN_HPP_NAMESPACE_STRING\"::" +
         ( commandData.handle.empty() ? "" : stripPrefix( commandData.handle, "Vk" ) + "::" ) +
         ( singular ? stripPluralS( commandName ) : commandName ) + ( unique ? "Unique" : "" ) + "\"";

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

  if ( unique )
  {
    str += ", deleter";
  }

  str += " );\n";
}

void VulkanHppGenerator::appendFunctionBodyEnhancedSingleStep( std::string &                    str,
                                                               std::string const &              indentation,
                                                               std::string const &              name,
                                                               CommandData const &              commandData,
                                                               size_t                           returnParamIndex,
                                                               size_t                           templateParamIndex,
                                                               std::map<size_t, size_t> const & vectorParamIndices,
                                                               bool                             singular ) const
{
  str += indentation + "  ";
  if ( commandData.returnType == "VkResult" )
  {
    str += "Result result = static_cast<Result>( ";
  }
  else if ( commandData.returnType != "void" )
  {
    str += "return ";
  }
  appendCall( str, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, false, true, singular );
  if ( commandData.returnType == "VkResult" )
  {
    str += " )";
  }
  str += ";\n";
}

void VulkanHppGenerator::appendFunctionBodyEnhancedTwoStep( std::string &                    str,
                                                            std::string const &              indentation,
                                                            std::string const &              name,
                                                            CommandData const &              commandData,
                                                            size_t                           returnParamIndex,
                                                            size_t                           templateParamIndex,
                                                            std::map<size_t, size_t> const & vectorParamIndices,
                                                            bool                             singular,
                                                            std::string const &              returnName ) const
{
  assert( !singular );
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
  appendCall( call1, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, true, true, false );
  appendCall( call2, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, true, false, false );

  str += replaceWithMap( selectedTemplate,
                         { { "sizeName", sizeName },
                           { "returnName", returnName },
                           { "call1", call1 },
                           { "call2", call2 },
                           { "i", indentation } } );
}

void VulkanHppGenerator::appendFunctionBodyEnhancedVectorOfStructureChain(
  std::string &                    str,
  std::string const &              indentation,
  std::string const &              name,
  CommandData const &              commandData,
  size_t                           returnParamIndex,
  std::map<size_t, size_t> const & vectorParamIndices,
  bool                             withAllocator ) const
{
  std::string const stringTemplate =
    R"(${i}  std::vector<StructureChain,Allocator> ${returnName}${vectorAllocator};
${i}  uint32_t ${sizeName};
${i}  d.${commandName}( m_${handleName}, &${sizeName}, nullptr );
${i}  ${returnName}.resize( ${sizeName} );
${i}  std::vector<VULKAN_HPP_NAMESPACE::${returnType}> localVector( ${sizeName} );
${i}  for ( uint32_t i = 0; i < ${sizeName} ; i++ )
${i}  {
${i}    localVector[i].pNext = ${returnName}[i].template get<VULKAN_HPP_NAMESPACE::${returnType}>().pNext;
${i}  }
${i}  d.${commandName}( m_${handleName}, &${sizeName}, reinterpret_cast<${VkReturnType}*>( localVector.data() ) );
${i}  for ( uint32_t i = 0; i < ${sizeName} ; i++ )
${i}  {
${i}    ${returnName}[i].template get<VULKAN_HPP_NAMESPACE::${returnType}>() = localVector[i];
${i}  }
${i}  return ${returnName};
)";

  // local count variable to hold the size of the vector to fill
  std::map<size_t, size_t>::const_iterator returnit = vectorParamIndices.find( returnParamIndex );
  assert( returnit != vectorParamIndices.end() && ( returnit->second != INVALID_INDEX ) );

  assert( commandData.handle == commandData.params[0].type.type );
  assert( commandData.params.size() ==
          3 );  // make sure, there are three args: the handle, the pointer to size, and the data pointer

  str +=
    replaceWithMap( stringTemplate,
                    { { "commandName", name },
                      { "handleName", startLowerCase( stripPrefix( commandData.params[0].type.type, "Vk" ) ) },
                      { "i", indentation },
                      { "returnName", startLowerCase( stripPrefix( commandData.params[returnParamIndex].name, "p" ) ) },
                      { "returnType", stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" ) },
                      { "sizeName", startLowerCase( stripPrefix( commandData.params[returnit->second].name, "p" ) ) },
                      { "vectorAllocator", withAllocator ? "( vectorAllocator )" : "" },
                      { "VkReturnType", commandData.params[returnParamIndex].type.type } } );
}

void VulkanHppGenerator::appendFunctionBodyEnhancedVectorOfUniqueHandles(
  std::string &                    str,
  std::string const &              indentation,
  std::string const &              name,
  CommandData const &              commandData,
  size_t                           returnParamIndex,
  size_t                           templateParamIndex,
  std::map<size_t, size_t> const & vectorParamIndices,
  bool                             twoStep,
  bool                             singular,
  bool                             withAllocator ) const
{
  std::string const stringTemplate =
    R"(${i}  std::vector<UniqueHandle<${type}, Dispatch>, Allocator> ${uniqueTypeVariable}s${allocator};
${i}  std::vector<${type}> ${typeVariable}s( ${vectorSize} );
${i}  Result result = static_cast<Result>( d.vk${command}( m_device, ${arguments}, reinterpret_cast<Vk${type}*>(${typeVariable}s.data()) ) );
${i}  if ( ${successChecks} )
${i}  {
${i}    ${uniqueTypeVariable}s.reserve( ${vectorSize} );
${i}    ${Deleter}<${DeleterTemplate},Dispatch> deleter( *this, ${deleterArg}, d );
${i}    for ( size_t i=0 ; i<${vectorSize} ; i++ )
${i}    {
${i}      ${uniqueTypeVariable}s.push_back( UniqueHandle<${type}, Dispatch>( ${typeVariable}s[i], deleter ) );
${i}    }
${i}  }

${i}  return createResultValue( result, ${uniqueTypeVariable}s, VULKAN_HPP_NAMESPACE_STRING "::${class}::${commandName}Unique"${successCodes} );
)";

  std::string type = ( returnParamIndex != INVALID_INDEX ) ? commandData.params[returnParamIndex].type.type : "";
  std::string typeVariable       = startLowerCase( stripPrefix( type, "Vk" ) );
  std::string uniqueTypeVariable = "unique" + stripPrefix( type, "Vk" );
  std::string arguments;
  appendArguments( arguments,
                   commandData,
                   returnParamIndex,
                   templateParamIndex,
                   vectorParamIndices,
                   twoStep,
                   true,
                   singular,
                   1,
                   commandData.params.size() - 1 );

  auto handleIt = m_handles.find( type );
  assert( handleIt != m_handles.end() );

  assert( !commandData.successCodes.empty() );
  std::string successChecks =
    "result == VULKAN_HPP_NAMESPACE::Result::" + createSuccessCode( commandData.successCodes[0], m_tags );
  std::string successCodes;
  if ( 1 < commandData.successCodes.size() )
  {
    successChecks = "( " + successChecks + " )";
    successCodes  = ", { VULKAN_HPP_NAMESPACE::Result::" + createSuccessCode( commandData.successCodes[0], m_tags );
    for ( size_t i = 1; i < commandData.successCodes.size(); i++ )
    {
      successChecks +=
        " || ( result == VULKAN_HPP_NAMESPACE::Result::" + createSuccessCode( commandData.successCodes[i], m_tags ) +
        " )";
      successCodes += ", VULKAN_HPP_NAMESPACE::Result::" + createSuccessCode( commandData.successCodes[i], m_tags );
    }
    successCodes += " }";
  }

  std::string commandName      = determineCommandName( name, commandData.params[0].type.type );
  bool        isCreateFunction = ( name.substr( 2, 6 ) == "Create" );
  str += replaceWithMap(
    stringTemplate,
    std::map<std::string, std::string>{
      { "allocator", withAllocator ? "( vectorAllocator )" : "" },
      { "arguments", arguments },
      { "class", stripPrefix( commandData.handle, "Vk" ) },
      { "command", stripPrefix( name, "vk" ) },
      { "commandName", commandName },
      { "Deleter", handleIt->second.deletePool.empty() ? "ObjectDestroy" : "PoolFree" },
      { "deleterArg",
        handleIt->second.deletePool.empty()
          ? "allocator"
          : "allocateInfo." + startLowerCase( stripPrefix( handleIt->second.deletePool, "Vk" ) ) },
      { "DeleterTemplate",
        stripPrefix( commandData.handle, "Vk" ) +
          ( handleIt->second.deletePool.empty() ? "" : "," + stripPrefix( handleIt->second.deletePool, "Vk" ) ) },
      { "i", indentation },
      { "successChecks", successChecks },
      { "successCodes", successCodes },
      { "type", stripPrefix( type, "Vk" ) },
      { "typeVariable", typeVariable },
      { "uniqueTypeVariable", uniqueTypeVariable },
      { "vectorSize", isCreateFunction ? "createInfos.size()" : "allocateInfo." + typeVariable + "Count" } } );
}

void VulkanHppGenerator::appendFunctionBodyStandard( std::string &       str,
                                                     std::string const & indentation,
                                                     std::string const & commandName,
                                                     CommandData const & commandData ) const
{
  std::pair<bool, std::string> returnData = generateFunctionBodyStandardReturn( commandData.returnType );

  assert( commandData.handle.empty() || ( commandData.handle == commandData.params[0].type.type ) );

  str += indentation + "  " + returnData.second + "d." + commandName + "( " +
         ( commandData.handle.empty() ? "" : ( "m_" + startLowerCase( stripPrefix( commandData.handle, "Vk" ) ) ) );
  for ( size_t i = commandData.handle.empty() ? 0 : 1; i < commandData.params.size(); i++ )
  {
    if ( 0 < i )
    {
      str += ", ";
    }
    appendFunctionBodyStandardArgument(
      str, commandData.params[i].type, commandData.params[i].name, commandData.params[i].arraySizes );
  }
  str += std::string( " )" ) + ( returnData.first ? " )" : "" ) + ";\n";
}

void VulkanHppGenerator::appendFunctionBodyStandardArgument( std::string &                    str,
                                                             TypeInfo const &                 typeInfo,
                                                             std::string const &              name,
                                                             std::vector<std::string> const & arraySizes ) const
{
  if ( beginsWith( typeInfo.type, "Vk" ) )
  {
    // the parameter is a vulkan type
    if ( !typeInfo.postfix.empty() || !arraySizes.empty() )
    {
      assert( ( typeInfo.postfix.empty() || ( typeInfo.postfix.back() == '*' ) ) &&
              ( arraySizes.empty() || ( arraySizes.size() == 1 ) ) );
      // it's a pointer -> need to reinterpret_cast it
      appendReinterpretCast( str,
                             typeInfo.prefix.find( "const" ) == 0,
                             typeInfo.type,
                             typeInfo.postfix.find( "* const" ) != std::string::npos );
    }
    else
    {
      // it's a value -> need to static_cast it
      str += "static_cast<" + typeInfo.type + ">";
    }
    str += "( " + name + " )";
  }
  else
  {
    // it's a non-vulkan type -> just use it
    str += name;
  }
}

bool VulkanHppGenerator::appendFunctionHeaderArgumentEnhanced( std::string &                    str,
                                                               ParamData const &                param,
                                                               size_t                           paramIndex,
                                                               std::map<size_t, size_t> const & vectorParamIndices,
                                                               bool                             skip,
                                                               bool                             argEncountered,
                                                               bool                             isTemplateParam,
                                                               bool                             isLastArgument,
                                                               bool                             singular,
                                                               bool                             withDefaults,
                                                               bool                             withAllocator ) const
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
      if ( param.type.postfix.empty() )
      {
        // and its not a pointer -> just use its type and name here
        appendFunctionHeaderArgumentEnhancedSimple( str, param, isLastArgument, withDefaults, withAllocator );
      }
      else
      {
        // the argument is not a vector, but a pointer
        assert( param.type.postfix.back() == '*' );
        appendFunctionHeaderArgumentEnhancedPointer( str, param, strippedParameterName, withDefaults, withAllocator );
      }
    }
    else
    {
      // the argument is a vector
      appendFunctionHeaderArgumentEnhancedVector( str,
                                                  param,
                                                  strippedParameterName,
                                                  it->second != INVALID_INDEX,
                                                  isTemplateParam,
                                                  singular,
                                                  withDefaults,
                                                  withAllocator );
    }
    argEncountered = true;
  }
  return argEncountered;
}

void VulkanHppGenerator::appendFunctionHeaderArgumentEnhancedPointer( std::string &       str,
                                                                      ParamData const &   param,
                                                                      std::string const & strippedParameterName,
                                                                      bool                withDefaults,
                                                                      bool                withAllocator ) const
{
  assert( param.type.postfix.back() == '*' );
  if ( param.optional )
  {
    // for an optional argument, trim the leading 'p' from the name
    str += "Optional<" + param.type.prefix + ( param.type.prefix.empty() ? "" : " " ) +
           stripPrefix( param.type.type, "Vk" ) + "> " + strippedParameterName;
    if ( withDefaults && !withAllocator )
    {
      str += " = nullptr";
    }
  }
  else if ( param.type.type == "void" )
  {
    // for void-pointer, just use type and name
    str += param.type.compose() + " " + param.name;
  }
  else if ( param.type.type != "char" )
  {
    // for non-char-pointer, change to reference
    assert( param.type.postfix == "*" );
    str += param.type.prefix + ( param.type.prefix.empty() ? "" : " " ) + stripPrefix( param.type.type, "Vk" ) + " & " +
           strippedParameterName;
  }
  else
  {
    // for char-pointer, change to const reference to std::string
    str += "const std::string & " + strippedParameterName;
  }
}

void VulkanHppGenerator::appendFunctionHeaderArgumentEnhancedSimple(
  std::string & str, ParamData const & param, bool lastArgument, bool withDefaults, bool withAllocator ) const
{
  str += param.type.compose() + " " + param.name + constructCArraySizes( param.arraySizes );

  if ( withDefaults && lastArgument && !withAllocator )
  {
    // check if the very last argument is a flag without any bits -> provide some empty default for it
    std::map<std::string, BitmaskData>::const_iterator bitmasksIt = m_bitmasks.find( param.type.type );
    if ( bitmasksIt != m_bitmasks.end() )
    {
      // get the enum corresponding to this flag, to check if it's empty
      std::string                                     strippedBitmaskName = stripPrefix( bitmasksIt->first, "Vk" );
      std::map<std::string, EnumData>::const_iterator enumIt = m_enums.find( bitmasksIt->second.requirements );
      assert( ( enumIt == m_enums.end() ) || ( enumIt->second.isBitmask ) );
      if ( ( enumIt == m_enums.end() ) || ( enumIt->second.values.empty() ) )
      {
        // there are no bits in this flag -> provide the default
        str += " = " + stripPrefix( param.type.type, "Vk" ) + "()";
      }
    }
  }
}

void VulkanHppGenerator::appendFunctionHeaderArgumentEnhancedVector( std::string &       str,
                                                                     ParamData const &   param,
                                                                     std::string const & strippedParameterName,
                                                                     bool                hasSizeParam,
                                                                     bool                isTemplateParam,
                                                                     bool                singular,
                                                                     bool                withDefaults,
                                                                     bool                withAllocator ) const
{
  assert( param.type.postfix.back() == '*' );
  // it's optional, if it's marked as optional and there's no size specified
  bool optional = param.optional && !hasSizeParam;

  bool        useString     = ( param.type.type.find( "char" ) != std::string::npos );
  std::string optionalBegin = optional ? "Optional<" : "";
  std::string optionalEnd   = optional ? "> " : ( useString ? " & " : "" );

  if ( useString )
  {
    // it's a char-vector -> use a std::string
    assert( param.type.prefix.find( "const" ) != std::string::npos );
    str += optionalBegin + "const std::string" + optionalEnd + strippedParameterName;
    if ( optional && withDefaults && !withAllocator )
    {
      str += " = nullptr";
    }
  }
  else if ( singular )
  {
    // in singular case, change from pointer to reference
    assert( !optional );  // never encounterd such a case
    str += param.type.prefix + ( param.type.prefix.empty() ? "" : " " ) + stripPrefix( param.type.type, "Vk" ) + " & " +
           stripPluralS( strippedParameterName );
  }
  else
  {
    // otherwise, use our ArrayProxy
    bool isConst = ( param.type.prefix.find( "const" ) != std::string::npos );
    str += optionalBegin + "ArrayProxy<" +
           ( isTemplateParam ? ( isConst ? "const T" : "T" ) : stripPostfix( param.type.compose(), "*" ) ) +
           "> const &" + optionalEnd + strippedParameterName;
  }
}

void VulkanHppGenerator::appendFunctionHeaderArguments( std::string &                    str,
                                                        CommandData const &              commandData,
                                                        size_t                           returnParamIndex,
                                                        size_t                           templateParamIndex,
                                                        std::map<size_t, size_t> const & vectorParamIndices,
                                                        bool                             enhanced,
                                                        bool                             singular,
                                                        bool                             withDefaults,
                                                        bool                             withAllocator ) const
{
  str += "(";
  if ( enhanced )
  {
    appendFunctionHeaderArgumentsEnhanced( str,
                                           commandData,
                                           returnParamIndex,
                                           templateParamIndex,
                                           vectorParamIndices,
                                           singular,
                                           withDefaults,
                                           withAllocator );
  }
  else
  {
    appendFunctionHeaderArgumentsStandard( str, commandData, withDefaults );
  }
  str += ")";

  if ( !commandData.handle.empty() )
  {
    str += " const";
  }
}

void VulkanHppGenerator::appendFunctionHeaderArgumentsEnhanced( std::string &                    str,
                                                                CommandData const &              commandData,
                                                                size_t                           returnParamIndex,
                                                                size_t                           templateParamIndex,
                                                                std::map<size_t, size_t> const & vectorParamIndices,
                                                                bool                             singular,
                                                                bool                             withDefaults,
                                                                bool                             withAllocator ) const
{
  // check if there's at least one argument left to put in here
  std::set<size_t> skippedParams = determineSkippedParams( returnParamIndex, vectorParamIndices );
  if ( skippedParams.size() + ( commandData.handle.empty() ? 0 : 1 ) < commandData.params.size() )
  {
    // determine the last argument, where we might provide some default for
    size_t lastArgument = INVALID_INDEX;
    for ( size_t i = commandData.params.size() - 1; i < commandData.params.size(); i-- )
    {
      if ( skippedParams.find( i ) == skippedParams.end() )
      {
        lastArgument = i;
        break;
      }
    }

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
                                                             ( templateParamIndex == i ),
                                                             ( lastArgument == i ),
                                                             singular,
                                                             withDefaults,
                                                             withAllocator );
    }

    if ( argEncountered )
    {
      str += ", ";
    }
  }
  if ( withAllocator )
  {
    str += "Allocator const& vectorAllocator, ";
  }
  str += "Dispatch const &d";
  if ( withDefaults && !withAllocator )
  {
    str += " = VULKAN_HPP_DEFAULT_DISPATCHER";
  }
  str += " ";
}

void VulkanHppGenerator::appendFunctionHeaderArgumentsStandard( std::string &       str,
                                                                CommandData const & commandData,
                                                                bool                withDefaults ) const
{
  // for the standard case, just list all the arguments as we've got them
  // determine the last argument, where we might provide some default for
  size_t lastArgument = commandData.params.size() - 1;

  bool argEncountered = false;
  for ( size_t i = commandData.handle.empty() ? 0 : 1; i < commandData.params.size(); i++ )
  {
    argEncountered = appendFunctionHeaderArgumentStandard(
      str, commandData.params[i], argEncountered, lastArgument == i, withDefaults );
  }
  if ( argEncountered )
  {
    str += ", ";
  }

  str += "Dispatch const &d";
  if ( withDefaults )
  {
    str += " = VULKAN_HPP_DEFAULT_DISPATCHER ";
  }
}

bool VulkanHppGenerator::appendFunctionHeaderArgumentStandard(
  std::string & str, ParamData const & param, bool argEncountered, bool isLastArgument, bool withDefaults ) const
{
  if ( argEncountered )
  {
    str += ",";
  }

  str += " " + param.type.compose() + " " + param.name + constructCArraySizes( param.arraySizes );

  if ( withDefaults && isLastArgument )
  {
    // check if the very last argument is a flag without any bits -> provide some empty default for it
    std::map<std::string, BitmaskData>::const_iterator bitmasksIt = m_bitmasks.find( param.type.type );
    if ( bitmasksIt != m_bitmasks.end() )
    {
      // get the enum corresponding to this flag, to check if it's empty
      std::string                                     strippedBitmaskName = stripPrefix( bitmasksIt->first, "Vk" );
      std::map<std::string, EnumData>::const_iterator enumIt = m_enums.find( bitmasksIt->second.requirements );
      assert( ( enumIt == m_enums.end() ) || ( enumIt->second.isBitmask ) );
      if ( ( enumIt == m_enums.end() ) || ( enumIt->second.values.empty() ) )
      {
        // there are no bits in this flag -> provide the default
        str += " = " + stripPrefix( param.type.type, "Vk" ) + "()";
      }
    }
  }
  return true;
}

void VulkanHppGenerator::appendFunctionHeaderReturnType( std::string &                    str,
                                                         CommandData const &              commandData,
                                                         size_t                           returnParamIndex,
                                                         std::map<size_t, size_t> const & vectorParamIndices,
                                                         std::string const &              enhancedReturnType,
                                                         bool                             enhanced,
                                                         bool                             twoStep,
                                                         bool                             singular,
                                                         bool                             unique,
                                                         bool                             isStructureChain ) const
{
  if ( enhanced )
  {
    bool useTypename =
      ( ( commandData.successCodes.size() == 1 ) ||
        ( ( commandData.successCodes.size() == 2 ) && ( commandData.successCodes[1] == "VK_INCOMPLETE" ) && twoStep ) );
    // the enhanced function might return some pretty complex return stuff
    bool isVector = ( enhancedReturnType.find( "Allocator" ) != std::string::npos );
    if ( unique )
    {
      // the unique version returns something prefixed with 'Unique'; potentially a vector of that stuff
      // it's a vector, if it's not the singular version and the return parameter is a vector parameter
      bool returnsVector = !singular && ( vectorParamIndices.find( returnParamIndex ) != vectorParamIndices.end() );

      std::string returnType = isStructureChain ? "StructureChain<X, Y, Z...>"
                                                : stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" );
      str += useTypename ? "typename ResultValueType<" : "ResultValue<";
      str += returnsVector ? "std::vector<UniqueHandle<" + returnType + ",Dispatch>,Allocator>>"
                           : "UniqueHandle<" + returnType + ",Dispatch>>";
      str += useTypename ? "::type " : " ";
    }
    else if ( ( enhancedReturnType != stripPrefix( commandData.returnType, "Vk" ) ) &&
              ( commandData.returnType != "void" ) )
    {
      // if the enhanced return type differs from the original return type, and it's not void, we return a
      // ResultValueType<...>::type
      assert( commandData.returnType == "VkResult" );
      // in singular case, we create the ResultValueType from the pure return type, otherwise from the enhanced return
      // type
      std::string returnType =
        isStructureChain
          ? "StructureChain<X, Y, Z...>"
          : ( singular ? stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" ) : enhancedReturnType );
      // for the non-singular case with allocation, we need to prepend with 'typename' to keep compilers happy
      str += ( useTypename ? "typename ResultValueType<" : "ResultValue<" ) + returnType + ">" +
             ( useTypename ? "::type " : " " );
    }
    else if ( ( returnParamIndex != INVALID_INDEX ) && ( 1 < commandData.successCodes.size() ) )
    {
      // if there is a return parameter at all, and there are multiple success codes, we return a ResultValue<...>
      // with the pure return type
      assert( commandData.returnType == "VkResult" );
      str += "ResultValue<" +
             ( isStructureChain ? "StructureChain<X, Y, Z...>"
                                : stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" ) ) +
             "> ";
    }
    else
    {
      // and in every other case, we just return the enhanced return type.
      str += ( isStructureChain && !isVector ? "StructureChain<X, Y, Z...>" : enhancedReturnType ) + " ";
    }
  }
  else
  {
    // the non-enhanced function just uses the return type
    str += stripPrefix( commandData.returnType, "Vk" ) + " ";
  }
}

void VulkanHppGenerator::appendFunctionHeaderTemplate( std::string &       str,
                                                       std::string const & indentation,
                                                       size_t              returnParamIndex,
                                                       size_t              templateParamIndex,
                                                       std::string const & enhancedReturnType,
                                                       bool                enhanced,
                                                       bool                singular,
                                                       bool                unique,
                                                       bool                withDefault,
                                                       bool                isStructureChain ) const
{
  bool withAllocator = ( enhancedReturnType.find( "Allocator" ) != std::string::npos );
  str += indentation + "template<";
  if ( enhanced )
  {
    if ( isStructureChain )
    {
      str += std::string( "typename " ) + ( withAllocator ? "StructureChain" : "X, typename Y, typename ...Z" ) + ", ";
    }
    else if ( ( templateParamIndex != INVALID_INDEX ) &&
              ( ( templateParamIndex != returnParamIndex ) || ( enhancedReturnType == "Result" ) ) )
    {
      assert( !withAllocator );
      str += "typename T, ";
    }
    if ( !singular && withAllocator )
    {
      // otherwise, if there's an Allocator used in the enhanced return type, we templatize on that Allocator
      assert( ( enhancedReturnType.substr( 0, 12 ) == "std::vector<" ) &&
              ( enhancedReturnType.find( ',' ) != std::string::npos ) && ( 12 < enhancedReturnType.find( ',' ) ) );
      str += "typename Allocator";
      if ( withDefault )
      {
        // for the default type get the type from the enhancedReturnType, which is of the form
        // 'std::vector<Type,Allocator>'
        assert( !isStructureChain || !unique );
        str += " = std::allocator<" +
               ( isStructureChain ? "StructureChain"
                                  : ( unique ? "Unique" : "" ) +
                                      enhancedReturnType.substr( 12, enhancedReturnType.find( ',' ) - 12 ) ) +
               ">";
      }
      str += ", ";
    }
  }
  str += std::string( "typename Dispatch" ) + ( withDefault ? " = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE" : "" ) + ">\n";
}

void VulkanHppGenerator::appendHandle( std::string &                              str,
                                       std::pair<std::string, HandleData> const & handleData,
                                       std::set<std::string> &                    listedHandles ) const
{
  if ( listedHandles.find( handleData.first ) == listedHandles.end() )
  {
    listedHandles.insert( handleData.first );

    // first check for any handle that needs to be listed before this one
    for ( auto const & command : handleData.second.commands )
    {
      auto commandIt = m_commands.find( command );
      assert( commandIt != m_commands.end() );
      for ( auto const & parameter : commandIt->second.params )
      {
        std::string typeName  = parameter.type.type;
        auto        handlesIt = m_handles.find( typeName );
        if ( ( handlesIt != m_handles.end() ) && ( listedHandles.find( typeName ) == listedHandles.end() ) )
        {
          appendHandle( str, *handlesIt, listedHandles );
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
        std::string enter, leave;
        std::tie( enter, leave ) = generateProtection( commandIt->second.feature, commandIt->second.extensions );
        str += enter;
        appendCommand( str, "  ", commandIt->first, commandIt->second, false );
        str += leave;
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

        std::string enter, leave;
        std::tie( enter, leave ) = generateProtection( commandIt->second.feature, commandIt->second.extensions );

        std::string commandString;
        commands += "\n" + enter;
        std::string commandName = determineCommandName( commandIt->first, commandIt->second.params[0].type.type );
        appendCommand( commands, "    ", commandIt->first, commandIt->second, false );

        // special handling for destroy functions
        bool platformLeft = false;
        if ( commandIt->second.alias.empty() &&
             ( ( ( commandIt->first.substr( 2, 7 ) == "Destroy" ) && ( commandName != "destroy" ) ) ||
               ( commandIt->first.substr( 2, 4 ) == "Free" ) ) )
        {
          assert( 1 < commandIt->second.params.size() );
          auto handleIt = m_handles.find( commandIt->second.params[1].type.type );
          assert( handleIt != m_handles.end() );
          if ( !handleIt->second.alias.empty() )
          {
            commands += leave;
            platformLeft = true;
          }

          std::string destroyCommandString;
          appendCommand( destroyCommandString, "    ", commandIt->first, commandIt->second, false );
          std::string shortenedName = ( commandIt->first.substr( 2, 7 ) == "Destroy" ) ? "destroy" : "free";
          size_t      pos           = destroyCommandString.find( commandName );
          while ( pos != std::string::npos )
          {
            destroyCommandString.replace( pos, commandName.length(), shortenedName );
            pos = destroyCommandString.find( commandName, pos );
          }
          commands += "\n" + destroyCommandString;
        }
        if ( !platformLeft )
        {
          commands += leave;
        }
      }

      static const std::string templateString = R"(
${enter}  class ${className}
  {
  public:
    using CType = Vk${className};

    static VULKAN_HPP_CONST_OR_CONSTEXPR ObjectType objectType = ObjectType::e${className};

  public:
    VULKAN_HPP_CONSTEXPR ${className}() VULKAN_HPP_NOEXCEPT
      : m_${memberName}(VK_NULL_HANDLE)
    {}

    VULKAN_HPP_CONSTEXPR ${className}( std::nullptr_t ) VULKAN_HPP_NOEXCEPT
      : m_${memberName}(VK_NULL_HANDLE)
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
      m_${memberName} = VK_NULL_HANDLE;
      return *this;
    }

#if defined(VULKAN_HPP_HAS_SPACESHIP_OPERATOR)
    auto operator<=>( ${className} const& ) const = default;
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
    Vk${className} m_${memberName};
  };
  static_assert( sizeof( ${className} ) == sizeof( Vk${className} ), "handle and wrapper have different size!" );

  template <>
  struct VULKAN_HPP_DEPRECATED("vk::cpp_type is deprecated. Use vk::CppType instead.") cpp_type<ObjectType::e${className}>
  {
    using type = ${className};
  };

  template <>
  struct CppType<ObjectType, ObjectType::e${className}>
  {
    using Type = ${className};
  };
)";

      std::string enter, leave;
      std::tie( enter, leave ) = generateProtection( handleData.first, !handleData.second.alias.empty() );
      str += replaceWithMap( templateString,
                             { { "className", stripPrefix( handleData.first, "Vk" ) },
                               { "commands", commands },
                               { "enter", enter },
                               { "memberName", startLowerCase( stripPrefix( handleData.first, "Vk" ) ) } } );

      if ( !handleData.second.alias.empty() )
      {
        str += "  using " + stripPrefix( handleData.second.alias, "Vk" ) + " = " +
               stripPrefix( handleData.first, "Vk" ) + ";\n";
      }
      str += leave;
    }
  }
}

void VulkanHppGenerator::appendHandles( std::string & str ) const
{
  std::set<std::string> listedHandles;
  for ( auto const & handle : m_handles )
  {
    appendHandle( str, handle, listedHandles );
  }
}

void VulkanHppGenerator::appendHandlesCommandDefintions( std::string & str ) const
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
      std::string enter, leave;
      std::tie( enter, leave ) = generateProtection( commandIt->second.feature, commandIt->second.extensions );
      str += enter;
      appendCommand( str, "  ", commandIt->first, commandIt->second, true );

      // special handling for destroy functions
      std::string commandName = determineCommandName( commandIt->first, commandIt->second.params[0].type.type );
      if ( commandIt->second.alias.empty() &&
           ( ( ( commandIt->first.substr( 2, 7 ) == "Destroy" ) && ( commandName != "destroy" ) ) ||
             ( commandIt->first.substr( 2, 4 ) == "Free" ) ) )
      {
        std::string destroyCommandString;
        appendCommand( destroyCommandString, "  ", commandIt->first, commandIt->second, true );
        std::string shortenedName = ( commandIt->first.substr( 2, 7 ) == "Destroy" ) ? "destroy" : "free";
        size_t      pos           = destroyCommandString.find( commandName );
        while ( pos != std::string::npos )
        {
          destroyCommandString.replace( pos, commandName.length(), shortenedName );
          pos = destroyCommandString.find( commandName, pos );
        }

        bool extended = false;
        // check if this command is protected and is aliased
        if ( !enter.empty() )
        {
          std::string                                                     alias = commandIt->first;
          std::vector<std::map<std::string, CommandData>::const_iterator> aliasIts;
          std::map<std::string, CommandData>::const_iterator              aliasIt = m_commands.begin();
          do
          {
            aliasIt =
              std::find_if( aliasIt, m_commands.end(), [&alias]( std::pair<std::string, CommandData> const & cd ) {
                return alias == cd.second.alias;
              } );
            if ( aliasIt != m_commands.end() )
            {
              aliasIts.push_back( aliasIt );
              ++aliasIt;
            }
          } while ( aliasIt != m_commands.end() );

          if ( !aliasIts.empty() )
          {
            assert( aliasIts.size() == 1 );
            aliasIt = aliasIts.front();

            std::string aliasEnter, aliasLeave;
            std::tie( aliasEnter, aliasLeave ) =
              generateProtection( aliasIt->second.feature, aliasIt->second.extensions );
            assert( aliasEnter.empty() );

            pos = destroyCommandString.find( commandIt->first );
            while ( pos != std::string::npos )
            {
              assert( ( 6 < pos ) && ( destroyCommandString.substr( pos - 6, 6 ) == "    d." ) );
              size_t endPos = destroyCommandString.find( ';', pos );
              assert( endPos != std::string::npos );
              std::string originalCall = destroyCommandString.substr( pos - 6, endPos - pos + 7 );
              std::string aliasCall    = originalCall;
              aliasCall.replace( 6, commandIt->first.length(), aliasIt->first );
              destroyCommandString.replace(
                pos - 6, endPos - pos + 7, enter + originalCall + "\n#else\n" + aliasCall + "\n" + leave );
              pos      = destroyCommandString.find( commandIt->first, endPos );
              extended = true;
            }
          }
        }
        if ( extended )
        {
          str += leave + "\n" + destroyCommandString + "\n";
        }
        else
        {
          str += "\n" + destroyCommandString + leave;
        }
      }
      else
      {
        str += leave + "\n";
      }
    }
  }
}

void VulkanHppGenerator::appendHashStructures( std::string & str ) const
{
  str +=
    "\n"
    "namespace std\n"
    "{\n";

  const std::string hashTemplate = R"(  template <> struct hash<VULKAN_HPP_NAMESPACE::${type}>
  {
    std::size_t operator()(VULKAN_HPP_NAMESPACE::${type} const& ${name}) const VULKAN_HPP_NOEXCEPT
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

  str += "}\n";
}

// Intended only for `enum class Result`!
void VulkanHppGenerator::appendResultExceptions( std::string & str ) const
{
  std::string templateString = R"(
  class ${className} : public SystemError
  {
  public:
    ${className}( std::string const& message )
      : SystemError( make_error_code( ${enumName}::${enumMemberName} ), message ) {}
    ${className}( char const * message )
      : SystemError( make_error_code( ${enumName}::${enumMemberName} ), message ) {}
  };
)";

  auto enumData = m_enums.find( "VkResult" );
  for ( auto const & value : enumData->second.values )
  {
    if ( beginsWith( value.vkValue, "eError" ) )
    {
      str += replaceWithMap( templateString,
                             { { "className", stripPrefix( value.vkValue, "eError" ) + "Error" },
                               { "enumName", stripPrefix( enumData->first, "Vk" ) },
                               { "enumMemberName", value.vkValue } } );
    }
  }
  str += "\n";
}

void VulkanHppGenerator::appendStruct( std::string &                                 str,
                                       std::pair<std::string, StructureData> const & structure,
                                       std::set<std::string> &                       listedStructures ) const
{
  if ( listedStructures.find( structure.first ) == listedStructures.end() )
  {
    listedStructures.insert( structure.first );
    for ( auto const & member : structure.second.members )
    {
      auto structureIt = m_structures.find( member.type.type );
      if ( ( structureIt != m_structures.end() ) &&
           ( listedStructures.find( member.type.type ) == listedStructures.end() ) )
      {
        appendStruct( str, *structureIt, listedStructures );
      }
    }
    if ( !structure.second.subStruct.empty() )
    {
      auto structureIt = m_structures.find( structure.second.subStruct );
      if ( ( structureIt != m_structures.end() ) &&
           ( listedStructures.find( structureIt->first ) == listedStructures.end() ) )
      {
        appendStruct( str, *structureIt, listedStructures );
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
  }
}

void VulkanHppGenerator::appendStructAssignmentOperators( std::string &                                 str,
                                                          std::pair<std::string, StructureData> const & structData,
                                                          std::string const &                           prefix ) const
{
  static const std::string assignmentFromVulkanType = R"(
${prefix}${structName} & operator=( Vk${structName} const & rhs ) VULKAN_HPP_NOEXCEPT
${prefix}{
${prefix}  *this = *reinterpret_cast<VULKAN_HPP_NAMESPACE::${structName} const *>( &rhs );
${prefix}  return *this;
${prefix}}
)";
  str += replaceWithMap( assignmentFromVulkanType,
                         { { "prefix", prefix }, { "structName", stripPrefix( structData.first, "Vk" ) } } );

  // we need an assignment operator if there is constant member in this struct
  if ( std::find_if( structData.second.members.begin(), structData.second.members.end(), []( MemberData const & md ) {
         return md.values.size() == 1;
       } ) != structData.second.members.end() )
  {
    std::string assignments;
    for ( auto member : structData.second.members )
    {
      if ( member.values.size() != 1 )
      {
        assignments += prefix + member.name + " = rhs." + member.name + ";\n";
      }
    }

    static const std::string assignmentTemplate = R"(
${prefix}${structName} & operator=( ${structName} const & rhs ) VULKAN_HPP_NOEXCEPT
${prefix}{
${assignments}
${prefix}  return *this;
${prefix}}
)";
    str += replaceWithMap( assignmentTemplate,
                           { { "assignments", assignments },
                             { "prefix", prefix },
                             { "structName", stripPrefix( structData.first, "Vk" ) } } );
  }
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
    auto operator<=>( ${name} const& ) const = default;
#else
    bool operator==( ${name} const& rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return ${compareMembers};
    }

    bool operator!=( ${name} const& rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return !operator==( rhs );
    }
#endif
)";

  str += replaceWithMap( compareTemplate,
                         { { "name", stripPrefix( structData.first, "Vk" ) }, { "compareMembers", compareMembers } } );
}

std::string
  VulkanHppGenerator::constructConstexprString( std::pair<std::string, StructureData> const & structData ) const
{
  // structs with a union (and VkBaseInStructure and VkBaseOutStructure) can't be a constexpr!
  bool isConstExpression = !containsUnion( structData.first ) && ( structData.first != "VkBaseInStructure" ) &&
                           ( structData.first != "VkBaseOutStructure" );
  return isConstExpression
           ? ( std::string( "VULKAN_HPP_CONSTEXPR" ) + ( containsArray( structData.first ) ? "_14 " : " " ) )
           : "";
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

${prefix}${structName}( Vk${structName} const & rhs ) VULKAN_HPP_NOEXCEPT
${prefix}{
${prefix}  *this = rhs;
${prefix}}
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
                           { "constexpr", constructConstexprString( structData ) },
                           { "initializers", initializers },
                           { "prefix", prefix },
                           { "structName", stripPrefix( structData.first, "Vk" ) } } );

  appendStructConstructorsEnhanced( str, structData, prefix );
}

void VulkanHppGenerator::appendStructConstructorsEnhanced( std::string &                                 str,
                                                           std::pair<std::string, StructureData> const & structData,
                                                           std::string const &                           prefix ) const
{
  auto memberIts =
    findAll( structData.second.members.begin(), structData.second.members.end(), []( MemberData const & md ) {
      return !md.len.empty() && ( ignoreLens.find( md.len.front() ) == ignoreLens.end() );
    } );
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
          assert( ( litit->second.size() == 1 ) || !litit->second.front()->optional );
          initializers +=
            ( firstArgument ? ": " : ", " ) + mit->name + "( " + generateLenInitializer( mit, litit ) + " )";
          sizeChecks += generateSizeCheck( litit->second, stripPrefix( structData.first, "Vk" ), prefix );
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
#if !defined(VULKAN_HPP_DISABLE_ENHANCED_MODE)
${templateHeader}${prefix}${structName}( ${arguments} )
${prefix}${initializers}
${prefix}{${sizeChecks}}
#endif  // !defined(VULKAN_HPP_DISABLE_ENHANCED_MODE)
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
      str += constructStandardArray( memberData.type.compose(), memberData.arraySizes ) + " const& ";
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
      else if ( !memberData.values.empty() )
      {
        // member with multiple values (those with just one don't get here) arbitrarily get the first value as their
        // default
        str += memberData.values.front();
      }
      else
      {
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
    if ( member.values.size() == 1 )
    {
      // members with just one allowed value are set to be const
      str += "const ";
    }
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
        if ( enumIt != m_enums.end() && member.type.postfix.empty() )
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

void VulkanHppGenerator::appendStructs( std::string & str ) const
{
  std::set<std::string> listedStructures;
  for ( auto const & structure : m_structures )
  {
    appendStruct( str, structure, listedStructures );
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
#if !defined(VULKAN_HPP_DISABLE_ENHANCED_MODE)
    ${templateHeader}${structureName} & set${ArrayName}( VULKAN_HPP_NAMESPACE::ArrayProxyNoTemporaries<${memberType}> const & ${arrayName}_ ) VULKAN_HPP_NOEXCEPT
    {
      ${lenName} = ${lenValue};
      ${memberName} = ${arrayName}_.data();
      return *this;
    }
#endif  // !defined(VULKAN_HPP_DISABLE_ENHANCED_MODE)
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
      stripPrefix( structData.first, "Vk" ) + "( " + stripPrefix( subStruct->first, "Vk" ) + " const& " +
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
  constructorAndSetters += "#endif // !defined( VULKAN_HPP_NO_STRUCT_CONSTRUCTORS )\n";
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

    operator ${vkName} const&() const VULKAN_HPP_NOEXCEPT
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
          itExtend = std::find_if( m_structures.begin(), m_structures.end(), [extendName]( auto const & sd ) {
            return sd.second.aliases.find( extendName ) != sd.second.aliases.end();
          } );
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
      str += "      case Result::" + value.vkValue + ": throw " + stripPrefix( value.vkValue, "eError" ) +
             "Error( message );\n";
    }
  }
  str +=
    "      default: throw SystemError( make_error_code( result ) );\n"
    "    }\n"
    "  }\n";
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
         " const& rhs ) VULKAN_HPP_NOEXCEPT\n"
         "    {\n"
         "      memcpy( static_cast<void*>(this), &rhs, sizeof( VULKAN_HPP_NAMESPACE::" +
         unionName +
         " ) );\n"
         "    }\n";

  bool firstMember = true;
  for ( auto const & member : structure.second.members )
  {
    // VkBool32 is aliased to uint32_t. Don't create a VkBool32 constructor if the union also contains a uint32_t
    // constructor.
    auto compareBool32Alias = []( MemberData const & member ) {
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
      memcpy( static_cast<void*>(this), &rhs, sizeof( VULKAN_HPP_NAMESPACE::${unionName} ) );
      return *this;
    }

    operator Vk${unionName} const&() const
    {
      return *reinterpret_cast<const Vk${unionName}*>(this);
    }

    operator Vk${unionName} &()
    {
      return *reinterpret_cast<Vk${unionName}*>(this);
    }

)";
  str += replaceWithMap( operatorsTemplate, { { "unionName", stripPrefix( structure.first, "Vk" ) } } );

  // the union member variables
  // if there's at least one Vk... type in this union, check for unrestricted unions support
  bool needsUnrestrictedUnions =
    ( std::find_if( structure.second.members.begin(), structure.second.members.end(), []( MemberData const & member ) {
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
  // check that the aliasName is either a known enum value or at least a known alias
  check( ( std::find_if( values.begin(),
                         values.end(),
                         [&aliasName]( EnumValueData const & evd ) { return evd.vulkanValue == aliasName; } ) !=
           values.end() ) ||
           ( aliases.find( aliasName ) != aliases.end() ),
         line,
         "unknown enum alias <" + aliasName + ">" );

  auto aliasIt = aliases.find( name );
  check( ( aliasIt == aliases.end() ) || ( aliasIt->second.first == aliasName ),
         line,
         "enum alias <" + name + "> already listed for a different enum value" );

  // only list aliases that map to different vkNames
  aliasIt = std::find_if( aliases.begin(),
                          aliases.end(),
                          [&vkName]( std::pair<std::string, std::pair<std::string, std::string>> const & aliasEntry ) {
                            return vkName == aliasEntry.second.second;
                          } );
  if ( aliasIt == aliases.end() )
  {
    aliases.insert( std::make_pair( name, std::make_pair( aliasName, vkName ) ) );
  }
}

void VulkanHppGenerator::EnumData::addEnumValue( int                 line,
                                                 std::string const & valueName,
                                                 bool                bitmask,
                                                 bool                bitpos,
                                                 std::string const & prefix,
                                                 std::string const & postfix,
                                                 std::string const & tag )
{
  std::string translatedName = createEnumValueName( valueName, prefix, postfix, bitmask, tag );

  auto it = std::find_if( values.begin(), values.end(), [&translatedName]( EnumValueData const & evd ) {
    return evd.vkValue == translatedName;
  } );
  if ( it == values.end() )
  {
    values.push_back( EnumValueData( line, valueName, translatedName, bitpos ) );
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
  for ( auto const & baseType : m_baseTypes )
  {
    check( m_types.find( baseType.second.type ) != m_types.end(),
           baseType.second.xmlLine,
           "basetype type <" + baseType.second.type + "> not specified" );
  }
  for ( auto const & bitmask : m_bitmasks )
  {
    if ( !bitmask.second.requirements.empty() )
    {
      check( m_enums.find( bitmask.second.requirements ) != m_enums.end(),
             bitmask.second.xmlLine,
             "bitmask requires unknown <" + bitmask.second.requirements + ">" );
    }
  }
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
      warn( m_extensions.find( require.first ) != m_extensions.end(),
            require.second,
            "unknown extension requires <" + require.first + ">" );
    }
  }
  for ( auto const & funcPointer : m_funcPointers )
  {
    if ( !funcPointer.second.requirements.empty() )
    {
      check( m_types.find( funcPointer.second.requirements ) != m_types.end(),
             funcPointer.second.xmlLine,
             "funcpointer requires unknown <" + funcPointer.second.requirements + ">" );
    }
  }

  std::set<std::string> sTypeValues;
  for ( auto const & structure : m_structures )
  {
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
                               [&selection]( EnumValueData const & evd ) { return evd.vulkanValue == selection; } ) !=
                   enumIt->second.values.end(),
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
                                 [&enumValue]( auto const & evd ) { return enumValue == evd.vulkanValue; } ) !=
                     enumIt->second.values.end(),
                   member.xmlLine,
                   "value <" + enumValue + "> for member <" + member.name + "> in structure <" + structure.first +
                     " of enum type <" + member.type.type + "> not listed" );
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
      warn( sTypeValues.erase( enumValue.vulkanValue ) == 1,
            enumValue.xmlLine,
            "VkStructureType enum value <" + enumValue.vulkanValue + "> never used" );
    }
  }
  assert( sTypeValues.empty() );

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

  for ( auto const & command : m_commands )
  {
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

  for ( auto const & handle : m_handles )
  {
    for ( auto const & parent : handle.second.parents )
    {
      check( m_handles.find( parent ) != m_handles.end(),
             handle.second.xmlLine,
             "handle with unknown parent <" + parent + ">" );
    }
  }
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

std::string VulkanHppGenerator::determineEnhancedReturnType( CommandData const &              commandData,
                                                             size_t                           returnParamIndex,
                                                             std::map<size_t, size_t> const & vectorParamIndices,
                                                             bool                             isStructureChain ) const
{
  assert( ( returnParamIndex == INVALID_INDEX ) || ( returnParamIndex < commandData.params.size() ) );
  for ( auto vpi : vectorParamIndices )
  {
    assert( ( vpi.first != vpi.second ) && ( vpi.first < commandData.params.size() ) &&
            ( ( vpi.second == INVALID_INDEX ) || ( vpi.second < commandData.params.size() ) ) );
  }

  std::string enhancedReturnType;
  if ( returnParamIndex != INVALID_INDEX )
  {
    // if there is a return parameter, we think returnType is always "void" or "VkResult"
    // -> we can return that parameter
    assert( ( commandData.returnType == "void" ) || ( commandData.returnType == "VkResult" ) );
    assert( commandData.successCodes.empty() || ( commandData.successCodes[0] == "VK_SUCCESS" ) );
    if ( vectorParamIndices.find( returnParamIndex ) != vectorParamIndices.end() )
    {
      enhancedReturnType =
        ( commandData.params[returnParamIndex].type.type == "void" )
          ? "std::vector<uint8_t,Allocator>"  // the return parameter is a vector-type parameter
          : isStructureChain ? "std::vector<StructureChain,Allocator>"  // for structureChain returns, it's just
                                                                        // a vector of StrutureChains
                             : "std::vector<" + stripPrefix( commandData.params[returnParamIndex].type.type, "Vk" ) +
                                 ",Allocator>";  // for the other parameters, we use a vector of the pure type
    }
    else
    {
      // it's a simple parameter -> get the type and just remove the trailing '*' (originally, it's a pointer)
      assert( commandData.params[returnParamIndex].type.postfix.back() == '*' );
      assert( ( commandData.params[returnParamIndex].type.prefix.find( "const" ) == std::string::npos ) &&
              ( commandData.params[returnParamIndex].type.postfix.find( "const" ) == std::string::npos ) );
      enhancedReturnType = stripPostfix( commandData.params[returnParamIndex].type.compose(), "*" );
    }
  }
  else if ( ( commandData.returnType == "VkResult" ) && ( commandData.successCodes.size() == 1 ) )
  {
    // an original return of type "Result" with just one successCode is changed to void, errors throw an exception
    enhancedReturnType = "void";
  }
  else
  {
    // the return type just stays the original return type
    enhancedReturnType = stripPrefix( commandData.returnType, "Vk" );
  }
  return enhancedReturnType;
}

size_t VulkanHppGenerator::determineReturnParamIndex( CommandData const &              commandData,
                                                      std::map<size_t, size_t> const & vectorParamIndices,
                                                      bool                             twoStep ) const
{
  for ( auto vpi : vectorParamIndices )
  {
    assert( ( vpi.first != vpi.second ) && ( vpi.first < commandData.params.size() ) &&
            ( ( vpi.second == INVALID_INDEX ) || ( vpi.second < commandData.params.size() ) ) );
  }

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
      assert( std::find_if(
                vectorParamIndices.begin(), vectorParamIndices.end(), [index]( std::pair<size_t, size_t> const & vpi ) {
                  return vpi.second == index;
                } ) == vectorParamIndices.end() );

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

size_t VulkanHppGenerator::determineTemplateParamIndex( std::vector<ParamData> const &   params,
                                                        std::map<size_t, size_t> const & vectorParamIndices ) const
{
  size_t templateParamIndex = INVALID_INDEX;

  for ( size_t i = 0; i < params.size(); i++ )
  {
    // any vector parameter on the pure type void is templatized in the enhanced API
    if ( ( vectorParamIndices.find( i ) != vectorParamIndices.end() ) && ( params[i].type.type == "void" ) )
    {
#if !defined( NDEBUG )
      for ( size_t j = i + 1; j < params.size(); j++ )
      {
        assert( ( vectorParamIndices.find( j ) == vectorParamIndices.end() ) || ( params[j].type.type != "void" ) );
      }
#endif
      templateParamIndex = i;
      break;
    }
  }
  assert( ( templateParamIndex == INVALID_INDEX ) ||
          ( vectorParamIndices.find( templateParamIndex ) != vectorParamIndices.end() ) );
  return templateParamIndex;
}

std::map<size_t, size_t> VulkanHppGenerator::determineVectorParamIndices( std::vector<ParamData> const & params ) const
{
  std::map<size_t, size_t> vectorParamIndices;

  // look for the parameters whose len equals the name of an other parameter
  for ( auto it = params.begin(); it != params.end(); ++it )
  {
    if ( !it->len.empty() )
    {
      auto findLambda = [it]( ParamData const & pd ) {
        return pd.name == it->len;
      };
      auto findIt =
        std::find_if( params.begin(), it, findLambda );  // look for a parameter named as the len of this parameter
      assert( ( std::count_if( params.begin(), params.end(), findLambda ) == 0 ) ||
              ( findIt < it ) );  // make sure, there is no other parameter like that

      // add this parameter as a vector parameter, using the len-name parameter as the second value (or
      // INVALID_INDEX if there is nothing like that)
      vectorParamIndices.insert(
        std::make_pair( std::distance( params.begin(), it ),
                        ( findIt < it ) ? std::distance( params.begin(), findIt ) : INVALID_INDEX ) );
      assert( ( vectorParamIndices[std::distance( params.begin(), it )] != INVALID_INDEX ) ||
              ( it->len == "null-terminated" ) || isParamIndirect( it->len, params ) );
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
           std::vector<std::vector<MemberData>::const_iterator>>::const_iterator litit ) const
{
  std::string initializer;
  if ( ( 1 < litit->second.size() ) &&
       ( std::find_if( litit->second.begin(), litit->second.end(), []( std::vector<MemberData>::const_iterator it ) {
           return !it->noAutoValidity;
         } ) == litit->second.end() ) )
  {
    // there are multiple arrays related to this len, all marked with noautovalidity
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

std::pair<std::string, std::string>
  VulkanHppGenerator::generateProtection( std::string const & feature, std::set<std::string> const & extensions ) const
{
  if ( feature.empty() && !extensions.empty() )
  {
    assert( getPlatforms( extensions ).size() == 1 );
    std::string platform = *getPlatforms( extensions ).begin();
    if ( !platform.empty() )
    {
      auto platformIt = m_platforms.find( platform );
      assert( platformIt != m_platforms.end() );
      std::string const & protect = platformIt->second.protect;
      if ( !protect.empty() )
      {
        return std::make_pair( "#ifdef " + protect + "\n", "#endif /*" + protect + "*/\n" );
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
    return generateProtection( typeIt->second.feature, typeIt->second.extensions );
  }
}

std::string
  VulkanHppGenerator::generateSizeCheck( std::vector<std::vector<MemberData>::const_iterator> const & arrayIts,
                                         std::string const &                                          structName,
                                         std::string const &                                          prefix ) const
{
  std::string sizeCheck;
  if ( 1 < arrayIts.size() )
  {
    std::string assertionText, throwText;
    if ( std::find_if( arrayIts.begin(), arrayIts.end(), []( std::vector<MemberData>::const_iterator it ) {
           return !it->noAutoValidity;
         } ) == arrayIts.end() )
    {
      // all the arrays are marked with noautovalidity -> exactly one of them has to be non-empty
      std::string sum;
      for ( size_t first = 0; first + 1 < arrayIts.size(); ++first )
      {
        sum += "!" + startLowerCase( stripPrefix( arrayIts[first]->name, "p" ) ) + "_.empty() + ";
      }
      sum += "!" + startLowerCase( stripPrefix( arrayIts.back()->name, "p" ) ) + "_.empty()";
      assertionText += prefix + "  VULKAN_HPP_ASSERT( ( " + sum + " ) == 1 );\n";
      throwText += prefix + "  if ( ( " + sum + " ) != 1 )\n";
      throwText += prefix + "  {\n";
      throwText += prefix + "    throw LogicError( VULKAN_HPP_NAMESPACE_STRING \"::" + structName + "::" + structName +
                   ": ( " + sum + " ) != 1\" );\n";
      throwText += prefix + "  }\n";
    }
    else
    {
      // none of the arrays should be marked with noautovalidity !
      assert( std::find_if( arrayIts.begin(), arrayIts.end(), []( std::vector<MemberData>::const_iterator it ) {
                return it->noAutoValidity;
              } ) == arrayIts.end() );
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
          if ( arrayIts[first]->optional || arrayIts[second]->optional )
          {
            assertionCheck = "( " + assertionCheck + " )";
            throwCheck     = "( " + throwCheck + " )";
            if ( arrayIts[second]->optional )
            {
              assertionCheck = secondName + ".empty() || " + assertionCheck;
              throwCheck     = "!" + secondName + ".empty() && " + throwCheck;
            }
            if ( arrayIts[first]->optional )
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

std::set<std::string> VulkanHppGenerator::getPlatforms( std::set<std::string> const & extensions ) const
{
  std::set<std::string> platforms;
  for ( auto const & e : extensions )
  {
    auto extensionIt = m_extensions.find( e );
    assert( extensionIt != m_extensions.end() );
    platforms.insert( extensionIt->second.platform );
  }
  return platforms;
}

bool VulkanHppGenerator::isParam( std::string const & name, std::vector<ParamData> const & params ) const
{
  return std::find_if( params.begin(), params.end(), [&name]( ParamData const & pd ) { return pd.name == name; } ) !=
         params.end();
}

bool VulkanHppGenerator::isParamIndirect( std::string const & name, std::vector<ParamData> const & params ) const
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
      auto structureIt = m_structures.find( paramIt->type.type );
      return ( structureIt != m_structures.end() ) && ( std::find_if( structureIt->second.members.begin(),
                                                                      structureIt->second.members.end(),
                                                                      [&n = nameParts[1]]( MemberData const & md ) {
                                                                        return md.name == n;
                                                                      } ) != structureIt->second.members.end() );
    }
  }
  return false;
}

bool VulkanHppGenerator::isTwoStepAlgorithm( std::vector<ParamData> const & params ) const
{
  // we generate a two-step algorithm for functions returning a vector of stuff, where the length is specified as a
  // pointer as well for those functions, the size can be queried first, and then used
  bool isTwoStep = false;
  for ( auto paramIt = params.begin(); paramIt != params.end() && !isTwoStep; ++paramIt )
  {
    if ( !paramIt->len.empty() )
    {
      auto lenIt =
        std::find_if( params.begin(), paramIt, [paramIt]( ParamData const & pd ) { return paramIt->len == pd.name; } );
      if ( lenIt != paramIt )
      {
        isTwoStep = ( lenIt->type.postfix.find( '*' ) != std::string::npos );
      }
    }
  }
  return isTwoStep;
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
  check( nameData.bitCount.empty(),
         line,
         "name <" + nameData.name + "> with unsupported bitCount <" + nameData.bitCount + ">" );
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
    checkAttributes( line, attributes, { { "category", { "bitmask" } } }, { { "requires", {} } } );

    std::string requirements;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "requires" )
      {
        requirements = attribute.second;
      }
    }

    NameData nameData;
    TypeInfo typeInfo;
    std::tie( nameData, typeInfo ) = readNameAndType( element );

    check( beginsWith( nameData.name, "Vk" ), line, "name <" + nameData.name + "> does not begin with <Vk>" );
    check( nameData.arraySizes.empty(), line, "name <" + nameData.name + "> with unsupported arraySizes" );
    check( nameData.bitCount.empty(),
           line,
           "name <" + nameData.name + "> with unsupported bitCount <" + nameData.bitCount + ">" );
    warn( ( typeInfo.type == "VkFlags" ) || ( typeInfo.type == "VkFlags64" ),
          line,
          "unexpected bitmask type <" + typeInfo.type + ">" );
    check( typeInfo.prefix == "typedef", line, "unexpected type prefix <" + typeInfo.prefix + ">" );
    check( typeInfo.postfix.empty(), line, "unexpected type postfix <" + typeInfo.postfix + ">" );

    check(
      m_commands.find( nameData.name ) == m_commands.end(), line, "command <" + nameData.name + "> already specified" );

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
                     { "queues", { "compute", "graphics", "sparse_binding", "transfer" } },
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

  registerDeleter( name, std::make_pair( name, commandData ) );

  // find the handle this command is going to be associated to
  check( !commandData.params.empty(), line, "command <" + name + "> with no params" );
  std::map<std::string, HandleData>::iterator handleIt = m_handles.find( commandData.params[0].type.type );
  if ( handleIt == m_handles.end() )
  {
    handleIt = m_handles.find( "" );
  }
  check( handleIt != m_handles.end(), line, "could not find a handle to hold command <" + name + ">" );
  commandData.handle = handleIt->first;

  // add this command to the list of commands
  check( m_commands.insert( std::make_pair( name, commandData ) ).second,
         line,
         "already encountered command <" + name + ">" );

  // put the command into the handle's list of commands
  check( handleIt->second.commands.insert( name ).second,
         line,
         "command list of handle <" + handleIt->first + "> already holds a commnand <" + name + ">" );
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

  // add the very same command as an alias
  bool inserted;
  std::tie( commandIt, inserted ) = m_commands.insert( std::make_pair( name, commandIt->second ) );
  check( inserted, line, "alias <" + name + "> for command <" + alias + "> already specified" );
  commandIt->second.alias   = alias;
  commandIt->second.xmlLine = line;

  // and add it to a handle!
  auto handleIt = m_handles.find( commandIt->second.handle );
  check( handleIt != m_handles.end(),
         line,
         "trying to add alias command <" + name + "> to an unknown handle <" + commandIt->second.handle + ">" );
  check( handleIt->second.commands.insert( name ).second,
         line,
         "alias command <" + name + "> already registered with handle <" + handleIt->first + ">" );
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
               isParamIndirect( paramData.len, params ),
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

  check( nameData.bitCount.empty(),
         line,
         "name <" + nameData.name + "> with unsupported bitCount <" + nameData.bitCount + ">" );
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
  check( nameData.bitCount.empty(),
         line,
         "name <" + nameData.name + "> with unsupported bitCount <" + nameData.bitCount + ">" );
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
    check( name == "VK_DEFINE_NON_DISPATCHABLE_HANDLE", line, "unknown type category=define name <" + name + ">" );
    check( element->LastChild() && element->LastChild()->ToText() && element->LastChild()->ToText()->Value(),
           line,
           "unknown formatting of type category=define named <" + name + ">" );

    // filter out the check for the different types of VK_DEFINE_NON_DISPATCHABLE_HANDLE
    std::string text  = element->LastChild()->ToText()->Value();
    size_t      start = text.find( "#if defined(__LP64__)" );
    check( start != std::string::npos, line, "unexpected text in type category=define named <" + name + ">" );
    size_t end = text.find_first_of( "\r\n", start + 1 );
    check( end != std::string::npos, line, "unexpected text in type category=define named <" + name + ">" );
    m_typesafeCheck = text.substr( start, end - start );
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
  enumData.addEnumValue( line, name, bitmask, !bitpos.empty(), prefix, postfix, tag );
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
  checkAttributes( line, attributes, { { "name", {} } }, { { "alias", {} }, { "comment", {} }, { "value", {} } } );
  checkElements( line, getChildElements( element ), {} );

  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "alias" )
    {
      check( m_constants.find( attribute.second ) != m_constants.end(),
             line,
             "unknown enum constant alias <" + attribute.second + ">" );
    }
    else if ( attribute.first == "name" )
    {
      check( m_constants.insert( attribute.second ).second,
             line,
             "already specified enum constant <" + attribute.second + ">" );
    }
  }
}

void VulkanHppGenerator::readEnums( tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, { { "name", {} } }, { { "comment", {} }, { "type", { "bitmask", "enum" } } } );
  std::vector<tinyxml2::XMLElement const *> children = getChildElements( element );

  std::string name, type;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "name" )
    {
      name = attribute.second;
      check( !name.empty(), line, "enum with empty name" );
    }
    else if ( attribute.first == "type" )
    {
      type = attribute.second;
      check( !type.empty(), line, "enum with empty type" );
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
    if ( it == m_enums.end() )
    {
      // well, some enums are not listed in the <types> section
      warn( false, line, "enum <" + name + "> is not listed as enum in the types section" );
      it = m_enums.insert( std::make_pair( name, EnumData() ) ).first;

      // add some "dummy" type in the types map
      assert( m_types.find( name ) == m_types.end() );
      m_types.insert( std::make_pair( name, TypeData( TypeCategory::Enum ) ) );
    }
    check( it->second.values.empty(), line, "enum <" + name + "> already holds values" );

    // mark it as a bitmask, if it is one
    bool bitmask         = ( type == "bitmask" );
    it->second.isBitmask = bitmask;
    if ( bitmask )
    {
      // look for the corresponding bitmask and set the requirements if needed!
      auto bitmaskIt = std::find_if( m_bitmasks.begin(), m_bitmasks.end(), [&name]( auto const & bitmask ) {
        return bitmask.second.requirements == name;
      } );
      if ( bitmaskIt == m_bitmasks.end() )
      {
        warn( false, line, "enum <" + name + "> is not listed as an requires for any bitmask in the types section" );

        std::string bitmaskName = name;
        size_t      pos         = bitmaskName.rfind( "FlagBits" );
        check( pos != std::string::npos, line, "enum <" + name + "> does not contain <FlagBits> as substring" );
        bitmaskName.replace( pos, 8, "Flags" );

        bitmaskIt = m_bitmasks.find( bitmaskName );
        check( bitmaskIt != m_bitmasks.end(),
               line,
               "enum <" + name + "> has not corresponding bitmask <" + bitmaskName + "> listed in the types section" );
        assert( bitmaskIt->second.requirements.empty() );
        bitmaskIt->second.requirements = name;
      }
    }

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

  std::string              deprecatedBy, name, obsoletedBy, platform, promotedTo, supported;
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
    else if ( attribute.first == "requires" )
    {
      requirements = tokenize( attribute.second, "," );
    }
    else if ( attribute.first == "requiresCore" )
    {
      std::string const & requiresCore = attribute.second;
      check( std::find_if( m_features.begin(),
                           m_features.end(),
                           [&requiresCore]( std::pair<std::string, std::string> const & nameNumber ) {
                             return nameNumber.second == requiresCore;
                           } ) != m_features.end(),
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
      readExtensionDisabledRequire( name, child );
    }
  }
  else
  {
    auto pitb = m_extensions.insert(
      std::make_pair( name, ExtensionData( line, deprecatedBy, obsoletedBy, platform, promotedTo ) ) );
    check( pitb.second, line, "already encountered extension <" + name + ">" );
    for ( auto const & r : requirements )
    {
      check( pitb.first->second.requirements.insert( std::make_pair( r, line ) ).second,
             line,
             "required extension <" + r + "> already listed" );
    }

    std::string tag = extractTag( line, name, m_tags );
    for ( auto child : children )
    {
      readExtensionRequire( child, name, tag, pitb.first->second.requirements );
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
  check( commandIt != m_commands.end(), line, "try to remove unknown command <" + name + ">" );
  auto handleIt = m_handles.find( commandIt->second.handle );
  check( handleIt != m_handles.end(), line, "cannot find handle corresponding to command <" + name + ">" );
  handleIt->second.commands.erase( commandIt->first );

  // then erase the command from the command list
  m_commands.erase( commandIt );
}

void VulkanHppGenerator::readExtensionDisabledEnum( std::string const &          extensionName,
                                                    tinyxml2::XMLElement const * element )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes(
    line,
    attributes,
    { { "name", {} } },
    { { "alias", {} }, { "bitpos", {} }, { "extends", {} }, { "extnumber", {} }, { "offset", {} }, { "value", {} } } );
  checkElements( line, getChildElements( element ), {} );

  std::string extends, name;
  for ( auto const & attribute : attributes )
  {
    if ( attribute.first == "extends" )
    {
      extends = attribute.second;
    }
    else if ( attribute.first == "name" )
    {
      name = attribute.second;
    }
  }

  if ( !extends.empty() )
  {
    auto enumIt = m_enums.find( extends );
    check( enumIt != m_enums.end(),
           line,
           "disabled extension <" + extensionName + "> references unknown enum <" + extends + ">" );
    check( std::find_if( enumIt->second.values.begin(),
                         enumIt->second.values.end(),
                         [&name]( EnumValueData const & evd ) { return evd.vulkanValue == name; } ) ==
             enumIt->second.values.end(),
           line,
           "disabled extension <" + extensionName + "> references known enum value <" + name + ">" );
  }
}

void VulkanHppGenerator::readExtensionDisabledRequire( std::string const &          extensionName,
                                                       tinyxml2::XMLElement const * element )
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
      readExtensionDisabledEnum( extensionName, child );
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
  check( typeIt != m_types.end(), line, "trying to remove unknown type <" + name + ">" );

  switch ( typeIt->second.category )
  {
    case TypeCategory::Bitmask:
    {
      auto bitmasksIt = m_bitmasks.find( name );
      check( bitmasksIt != m_bitmasks.end(), line, "trying to remove unknown bitmask <" + name + ">" );
      check( bitmasksIt->second.alias.empty(),
             line,
             "trying to remove disabled bitmask <" + name + "> which has alias <" + bitmasksIt->second.alias + ">" );
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

void VulkanHppGenerator::readExtensionRequire( tinyxml2::XMLElement const * element,
                                               std::string const &          extension,
                                               std::string const &          tag,
                                               std::map<std::string, int> & requirements )
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
      check( requirements.insert( std::make_pair( attribute.second, line ) ).second,
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
      readExtensionRequireCommand( child, extension );
    }
    else if ( value == "comment" )
    {
      readComment( child );
    }
    else if ( value == "enum" )
    {
      readRequireEnum( child, tag );
    }
    else if ( value == "type" )
    {
      readExtensionRequireType( child, extension );
    }
  }
}

void VulkanHppGenerator::readExtensionRequireCommand( tinyxml2::XMLElement const * element,
                                                      std::string const &          extension )
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
  check( commandIt != m_commands.end(), line, "extension <" + extension + "> requires unknown command <" + name + ">" );
  commandIt->second.extensions.insert( extension );
}

void VulkanHppGenerator::readExtensionRequireType( tinyxml2::XMLElement const * element, std::string const & extension )
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
  typeIt->second.extensions.insert( extension );
  check( getPlatforms( typeIt->second.extensions ).size() == 1,
         line,
         "type <" + name + "> is protected by more than one platform" );
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
  check( m_features.insert( std::make_pair( name, number ) ).second, line, "already specified feature <" + name + ">" );

  for ( auto child : children )
  {
    readFeatureRequire( child, name );
  }
}

void VulkanHppGenerator::readFeatureRequire( tinyxml2::XMLElement const * element, std::string const & feature )
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
      readFeatureRequireCommand( child, feature );
    }
    else if ( value == "comment" )
    {
      readComment( child );
    }
    else if ( value == "enum" )
    {
      readRequireEnum( child, "" );
    }
    else if ( value == "type" )
    {
      readFeatureRequireType( child, feature );
    }
  }
}

void VulkanHppGenerator::readFeatureRequireCommand( tinyxml2::XMLElement const * element, std::string const & feature )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, { { "name", {} } } );
  std::string command   = attributes.find( "name" )->second;
  auto        commandIt = m_commands.find( command );
  check( commandIt != m_commands.end(), line, "feature requires unknown command <" + command + ">" );
  check( commandIt->second.feature.empty(),
         line,
         "command <" + commandIt->first + "> already listed with feature <" + commandIt->second.feature + ">" );
  commandIt->second.feature = feature;
}

void VulkanHppGenerator::readFeatureRequireType( tinyxml2::XMLElement const * element, std::string const & feature )
{
  int                                line       = element->GetLineNum();
  std::map<std::string, std::string> attributes = getAttributes( element );
  checkAttributes( line, attributes, {}, { { "comment", {} }, { "name", {} } } );
  checkElements( line, getChildElements( element ), {} );
  std::string type = attributes.find( "name" )->second;

  // some types are in fact includes (like vk_platform) or defines (like VK_API_VERSION)
  if ( ( m_defines.find( type ) == m_defines.end() ) && ( m_includes.find( type ) == m_includes.end() ) )
  {
    auto typeIt = m_types.find( type );
    check( typeIt != m_types.end(), line, "feature requires unknown type <" + type + ">" );
    check( typeIt->second.feature.empty() || ( typeIt->second.feature == feature ),
           line,
           "type <" + type + "> already listed on feature <" + typeIt->second.feature + ">" );
    typeIt->second.feature = feature;
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
    checkAttributes( line, attributes, { { "category", { "handle" } } }, { { "parent", {} } } );

    std::string parent;
    for ( auto const & attribute : attributes )
    {
      if ( attribute.first == "parent" )
      {
        parent = attribute.second;
        check( !parent.empty(), line, "handle with empty parent" );
      }
    }

    NameData nameData;
    TypeInfo typeInfo;
    std::tie( nameData, typeInfo ) = readNameAndType( element );

    check( beginsWith( nameData.name, "Vk" ), line, "name <" + nameData.name + "> does not begin with <Vk>" );
    check( nameData.arraySizes.empty(), line, "name <" + nameData.name + "> with unsupported arraySizes" );
    check( nameData.bitCount.empty(),
           line,
           "name <" + nameData.name + "> with unsupported bitCount <" + nameData.bitCount + ">" );
    check( ( typeInfo.type == "VK_DEFINE_HANDLE" ) || ( typeInfo.type == "VK_DEFINE_NON_DISPATCHABLE_HANDLE" ),
           line,
           "handle with invalid type <" + typeInfo.type + ">" );
    check( typeInfo.prefix.empty(), line, "unexpected type prefix <" + typeInfo.prefix + ">" );
    check( typeInfo.postfix == "(", line, "unexpected type postfix <" + typeInfo.postfix + ">" );

    check( m_handles.insert( std::make_pair( nameData.name, HandleData( tokenize( parent, "," ), line ) ) ).second,
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
      nameData.name                                      = child->GetText();
      std::tie( nameData.arraySizes, nameData.bitCount ) = readModifiers( child->NextSibling() );
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
                       [&protect]( std::pair<std::string, PlatformData> const & p ) {
                         return p.second.protect == protect;
                       } ) == m_platforms.end(),
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

void VulkanHppGenerator::readRequireEnum( tinyxml2::XMLElement const * element, std::string const & tag )
{
  std::map<std::string, std::string> attributes = getAttributes( element );
  if ( attributes.find( "alias" ) != attributes.end() )
  {
    readRequireEnumAlias( element, attributes, tag );
  }
  else
  {
    readRequireEnum( element, attributes, tag );
  }
}

void VulkanHppGenerator::readRequireEnum( tinyxml2::XMLElement const *               element,
                                          std::map<std::string, std::string> const & attributes,
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
      element->GetLineNum(), name, enumIt->second.isBitmask, !bitpos.empty(), prefix, postfix, tag );
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
      memberData.optional = ( attribute.second == "true" );
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
    check( m_enums.insert( std::make_pair( name, EnumData() ) ).second, line, "enum <" + name + "> already specified" );
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

std::string VulkanHppGenerator::TypeInfo::compose() const
{
  return prefix + ( prefix.empty() ? "" : " " ) + ( ( type.substr( 0, 2 ) == "Vk" ) ? "VULKAN_HPP_NAMESPACE::" : "" ) +
         stripPrefix( type, "Vk" ) + postfix;
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
#if !defined(VULKAN_HPP_DISABLE_ENHANCED_MODE)
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

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( typename std::remove_const<T>::type & value ) VULKAN_HPP_NOEXCEPT
      : m_count( 1 )
      , m_ptr( &value )
    {}

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

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::initializer_list<typename std::remove_const<T>::type> const & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {}

    ArrayProxyNoTemporaries( std::initializer_list<T> & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {}

    template <typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::initializer_list<typename std::remove_const<T>::type> & list ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( list.size() ) )
      , m_ptr( list.begin() )
    {}

    ArrayProxyNoTemporaries( std::initializer_list<T> const && list ) VULKAN_HPP_NOEXCEPT = delete;
    ArrayProxyNoTemporaries( std::initializer_list<T> && list ) VULKAN_HPP_NOEXCEPT       = delete;

    template <size_t N>
    ArrayProxyNoTemporaries( std::array<T, N> const & data ) VULKAN_HPP_NOEXCEPT
      : m_count( N )
      , m_ptr( data.data() )
    {}

    template <size_t N, typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::array<typename std::remove_const<T>::type, N> const & data ) VULKAN_HPP_NOEXCEPT
      : m_count( N )
      , m_ptr( data.data() )
    {}

    template <size_t N>
    ArrayProxyNoTemporaries( std::array<T, N> & data ) VULKAN_HPP_NOEXCEPT
      : m_count( N )
      , m_ptr( data.data() )
    {}

    template <size_t N, typename B = T, typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::array<typename std::remove_const<T>::type, N> & data ) VULKAN_HPP_NOEXCEPT
      : m_count( N )
      , m_ptr( data.data() )
    {}

    template <size_t N>
    ArrayProxyNoTemporaries( std::array<T, N> const && data ) VULKAN_HPP_NOEXCEPT = delete;
    template <size_t N>
    ArrayProxyNoTemporaries( std::array<T, N> && data ) VULKAN_HPP_NOEXCEPT       = delete;

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxyNoTemporaries( std::vector<T, Allocator> const & data ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( data.size() ) )
      , m_ptr( data.data() )
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>,
              typename B      = T,
              typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::vector<typename std::remove_const<T>::type, Allocator> const & data ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( data.size() ) )
      , m_ptr( data.data() )
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxyNoTemporaries( std::vector<T, Allocator> & data ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( data.size() ) )
      , m_ptr( data.data() )
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>,
              typename B      = T,
              typename std::enable_if<std::is_const<B>::value, int>::type = 0>
    ArrayProxyNoTemporaries( std::vector<typename std::remove_const<T>::type, Allocator> & data ) VULKAN_HPP_NOEXCEPT
      : m_count( static_cast<uint32_t>( data.size() ) )
      , m_ptr( data.data() )
    {}

    ArrayProxyNoTemporaries( std::vector<T> const && data ) VULKAN_HPP_NOEXCEPT = delete;
    ArrayProxyNoTemporaries( std::vector<T> && data ) VULKAN_HPP_NOEXCEPT       = delete;

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
  class ArrayWrapper1D : public std::array<T,N>
  {
  public:
    VULKAN_HPP_CONSTEXPR ArrayWrapper1D() VULKAN_HPP_NOEXCEPT
      : std::array<T, N>()
    {}

    VULKAN_HPP_CONSTEXPR ArrayWrapper1D(std::array<T,N> const& data) VULKAN_HPP_NOEXCEPT
      : std::array<T, N>(data)
    {}

#if defined(_WIN32) && !defined(_WIN64)
    VULKAN_HPP_CONSTEXPR T const& operator[](int index) const VULKAN_HPP_NOEXCEPT
    {
      return std::array<T, N>::operator[](index);
    }

    VULKAN_HPP_CONSTEXPR T & operator[](int index) VULKAN_HPP_NOEXCEPT
    {
      return std::array<T, N>::operator[](index);
    }
#endif

    operator T const* () const VULKAN_HPP_NOEXCEPT
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
  bool operator<(std::string const& lhs, ArrayWrapper1D<char, N> const& rhs) VULKAN_HPP_NOEXCEPT
  {
    return lhs < rhs.data();
  }

  template <size_t N>
  bool operator<=(std::string const& lhs, ArrayWrapper1D<char, N> const& rhs) VULKAN_HPP_NOEXCEPT
  {
    return lhs <= rhs.data();
  }

  template <size_t N>
  bool operator>(std::string const& lhs, ArrayWrapper1D<char, N> const& rhs) VULKAN_HPP_NOEXCEPT
  {
    return lhs > rhs.data();
  }

  template <size_t N>
  bool operator>=(std::string const& lhs, ArrayWrapper1D<char, N> const& rhs) VULKAN_HPP_NOEXCEPT
  {
    return lhs >= rhs.data();
  }

  template <size_t N>
  bool operator==(std::string const& lhs, ArrayWrapper1D<char, N> const& rhs) VULKAN_HPP_NOEXCEPT
  {
    return lhs == rhs.data();
  }

  template <size_t N>
  bool operator!=(std::string const& lhs, ArrayWrapper1D<char, N> const& rhs) VULKAN_HPP_NOEXCEPT
  {
    return lhs != rhs.data();
  }

  template <typename T, size_t N, size_t M>
  class ArrayWrapper2D : public std::array<ArrayWrapper1D<T,M>,N>
  {
  public:
    VULKAN_HPP_CONSTEXPR ArrayWrapper2D() VULKAN_HPP_NOEXCEPT
      : std::array<ArrayWrapper1D<T,M>, N>()
    {}

    VULKAN_HPP_CONSTEXPR ArrayWrapper2D(std::array<std::array<T,M>,N> const& data) VULKAN_HPP_NOEXCEPT
      : std::array<ArrayWrapper1D<T,M>, N>(*reinterpret_cast<std::array<ArrayWrapper1D<T,M>,N> const*>(&data))
    {}
  };
)";

  static const std::string classFlags = R"(
  template <typename FlagBitsType> struct FlagTraits
  {
    enum { allFlags = 0 };
  };

  template <typename BitType>
  class Flags
  {
  public:
    using MaskType = typename std::underlying_type<BitType>::type;

    // constructors
    VULKAN_HPP_CONSTEXPR Flags() VULKAN_HPP_NOEXCEPT
      : m_mask(0)
    {}

    VULKAN_HPP_CONSTEXPR Flags(BitType bit) VULKAN_HPP_NOEXCEPT
      : m_mask(static_cast<MaskType>(bit))
    {}

    VULKAN_HPP_CONSTEXPR Flags(Flags<BitType> const& rhs) VULKAN_HPP_NOEXCEPT
      : m_mask(rhs.m_mask)
    {}

    VULKAN_HPP_CONSTEXPR explicit Flags(MaskType flags) VULKAN_HPP_NOEXCEPT
      : m_mask(flags)
    {}

    // relational operators
#if defined(VULKAN_HPP_HAS_SPACESHIP_OPERATOR)
    auto operator<=>(Flags<BitType> const&) const = default;
#else
    VULKAN_HPP_CONSTEXPR bool operator<(Flags<BitType> const& rhs) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask < rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator<=(Flags<BitType> const& rhs) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask <= rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator>(Flags<BitType> const& rhs) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask > rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator>=(Flags<BitType> const& rhs) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask >= rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator==(Flags<BitType> const& rhs) const VULKAN_HPP_NOEXCEPT
    {
      return m_mask == rhs.m_mask;
    }

    VULKAN_HPP_CONSTEXPR bool operator!=(Flags<BitType> const& rhs) const VULKAN_HPP_NOEXCEPT
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
    VULKAN_HPP_CONSTEXPR Flags<BitType> operator&(Flags<BitType> const& rhs) const VULKAN_HPP_NOEXCEPT
    {
      return Flags<BitType>(m_mask & rhs.m_mask);
    }

    VULKAN_HPP_CONSTEXPR Flags<BitType> operator|(Flags<BitType> const& rhs) const VULKAN_HPP_NOEXCEPT
    {
      return Flags<BitType>(m_mask | rhs.m_mask);
    }

    VULKAN_HPP_CONSTEXPR Flags<BitType> operator^(Flags<BitType> const& rhs) const VULKAN_HPP_NOEXCEPT
    {
      return Flags<BitType>(m_mask ^ rhs.m_mask);
    }

    VULKAN_HPP_CONSTEXPR Flags<BitType> operator~() const VULKAN_HPP_NOEXCEPT
    {
      return Flags<BitType>(m_mask ^ FlagTraits<BitType>::allFlags);
    }

    // assignment operators
    VULKAN_HPP_CONSTEXPR_14 Flags<BitType> & operator=(Flags<BitType> const& rhs) VULKAN_HPP_NOEXCEPT
    {
      m_mask = rhs.m_mask;
      return *this;
    }

    VULKAN_HPP_CONSTEXPR_14 Flags<BitType> & operator|=(Flags<BitType> const& rhs) VULKAN_HPP_NOEXCEPT
    {
      m_mask |= rhs.m_mask;
      return *this;
    }

    VULKAN_HPP_CONSTEXPR_14 Flags<BitType> & operator&=(Flags<BitType> const& rhs) VULKAN_HPP_NOEXCEPT
    {
      m_mask &= rhs.m_mask;
      return *this;
    }

    VULKAN_HPP_CONSTEXPR_14 Flags<BitType> & operator^=(Flags<BitType> const& rhs) VULKAN_HPP_NOEXCEPT
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

  private:
    MaskType  m_mask;
  };

#if !defined(VULKAN_HPP_HAS_SPACESHIP_OPERATOR)
  // relational operators only needed for pre C++20
  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator<(BitType bit, Flags<BitType> const& flags) VULKAN_HPP_NOEXCEPT
  {
    return flags > bit;
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator<=(BitType bit, Flags<BitType> const& flags) VULKAN_HPP_NOEXCEPT
  {
    return flags >= bit;
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator>(BitType bit, Flags<BitType> const& flags) VULKAN_HPP_NOEXCEPT
  {
    return flags < bit;
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator>=(BitType bit, Flags<BitType> const& flags) VULKAN_HPP_NOEXCEPT
  {
    return flags <= bit;
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator==(BitType bit, Flags<BitType> const& flags) VULKAN_HPP_NOEXCEPT
  {
    return flags == bit;
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR bool operator!=(BitType bit, Flags<BitType> const& flags) VULKAN_HPP_NOEXCEPT
  {
    return flags != bit;
  }
#endif

  // bitwise operators
  template <typename BitType>
  VULKAN_HPP_CONSTEXPR Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags) VULKAN_HPP_NOEXCEPT
  {
    return flags & bit;
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags) VULKAN_HPP_NOEXCEPT
  {
    return flags | bit;
  }

  template <typename BitType>
  VULKAN_HPP_CONSTEXPR Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags) VULKAN_HPP_NOEXCEPT
  {
    return flags ^ bit;
  }
)";

  static const std::string classObjectDestroy = R"(
  struct AllocationCallbacks;

  template <typename OwnerType, typename Dispatch>
  class ObjectDestroy
  {
    public:
      ObjectDestroy()
        : m_owner()
        , m_allocationCallbacks( nullptr )
        , m_dispatch( nullptr )
      {}

      ObjectDestroy( OwnerType owner, Optional<const AllocationCallbacks> allocationCallbacks = nullptr, Dispatch const &dispatch = VULKAN_HPP_DEFAULT_DISPATCHER ) VULKAN_HPP_NOEXCEPT
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
      OwnerType m_owner;
      Optional<const AllocationCallbacks> m_allocationCallbacks;
      Dispatch const* m_dispatch;
  };

  class NoParent;

  template <typename Dispatch>
  class ObjectDestroy<NoParent,Dispatch>
  {
    public:
      ObjectDestroy()
        : m_allocationCallbacks( nullptr )
        , m_dispatch( nullptr )
      {}

      ObjectDestroy( Optional<const AllocationCallbacks> allocationCallbacks, Dispatch const &dispatch = VULKAN_HPP_DEFAULT_DISPATCHER ) VULKAN_HPP_NOEXCEPT
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
      Optional<const AllocationCallbacks> m_allocationCallbacks;
      Dispatch const* m_dispatch;
  };
)";

  static const std::string classObjectFree = R"(
  template <typename OwnerType, typename Dispatch>
  class ObjectFree
  {
  public:
    ObjectFree() : m_owner(), m_allocationCallbacks( nullptr ), m_dispatch( nullptr ) {}

    ObjectFree( OwnerType                           owner,
                Optional<const AllocationCallbacks> allocationCallbacks = nullptr,
                Dispatch const &                    dispatch = VULKAN_HPP_DEFAULT_DISPATCHER ) VULKAN_HPP_NOEXCEPT
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
    OwnerType                           m_owner;
    Optional<const AllocationCallbacks> m_allocationCallbacks;
    Dispatch const *                    m_dispatch;
  };
)";

  static const std::string classOptional = R"(
  template <typename RefType>
  class Optional
  {
  public:
    Optional(RefType & reference) VULKAN_HPP_NOEXCEPT { m_ptr = &reference; }
    Optional(RefType * ptr) VULKAN_HPP_NOEXCEPT { m_ptr = ptr; }
    Optional(std::nullptr_t) VULKAN_HPP_NOEXCEPT { m_ptr = nullptr; }

    operator RefType*() const VULKAN_HPP_NOEXCEPT { return m_ptr; }
    RefType const* operator->() const VULKAN_HPP_NOEXCEPT { return m_ptr; }
    explicit operator bool() const VULKAN_HPP_NOEXCEPT { return !!m_ptr; }

  private:
    RefType *m_ptr;
  };
)";

  static const std::string classPoolFree = R"(
  template <typename OwnerType, typename PoolType, typename Dispatch>
  class PoolFree
  {
    public:
      PoolFree( OwnerType owner = OwnerType(), PoolType pool = PoolType(), Dispatch const &dispatch = VULKAN_HPP_DEFAULT_DISPATCHER ) VULKAN_HPP_NOEXCEPT
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
      OwnerType m_owner;
      PoolType m_pool;
      Dispatch const* m_dispatch;
  };
)";

  static const std::string classStructureChain = R"(
  template <typename X, typename Y> struct StructExtends { enum { value = false }; };

  template<typename Type, class...>
  struct IsPartOfStructureChain
  {
    static const bool valid = false;
  };

  template<typename Type, typename Head, typename... Tail>
  struct IsPartOfStructureChain<Type, Head, Tail...>
  {
    static const bool valid = std::is_same<Type, Head>::value || IsPartOfStructureChain<Type, Tail...>::valid;
  };

  template <size_t Index, typename T, typename... ChainElements>
  struct StructureChainContains
  {
    static const bool value = std::is_same<T, typename std::tuple_element<Index, std::tuple<ChainElements...>>::type>::value ||
                              StructureChainContains<Index - 1, T, ChainElements...>::value;
  };

  template <typename T, typename... ChainElements>
  struct StructureChainContains<0, T, ChainElements...>
  {
    static const bool value = std::is_same<T, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value;
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

    template <typename T, size_t Which = 0>
    T & get() VULKAN_HPP_NOEXCEPT
    {
      return std::get<ChainElementIndex<0, T, Which, void, ChainElements...>::value>( *this );
    }

    template <typename T, size_t Which = 0>
    T const & get() const VULKAN_HPP_NOEXCEPT
    {
      return std::get<ChainElementIndex<0, T, Which, void, ChainElements...>::value>( *this );
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
    void relink() VULKAN_HPP_NOEXCEPT
    {
      static_assert( IsPartOfStructureChain<ClassType, ChainElements...>::valid,
                     "Can't relink Structure that's not part of this StructureChain!" );
      static_assert(
        !std::is_same<ClassType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value || (Which != 0),
        "It's not allowed to have the first element unlinked!" );

      auto pNext = reinterpret_cast<VkBaseInStructure *>( &get<ClassType, Which>() );
      VULKAN_HPP_ASSERT( !isLinked( pNext ) );
      auto & headElement = std::get<0>( *this );
      pNext->pNext       = reinterpret_cast<VkBaseInStructure const*>(headElement.pNext);
      headElement.pNext  = pNext;
    }

    template <typename ClassType, size_t Which = 0>
    void unlink() VULKAN_HPP_NOEXCEPT
    {
      static_assert( IsPartOfStructureChain<ClassType, ChainElements...>::valid,
                     "Can't unlink Structure that's not part of this StructureChain!" );
      static_assert(
        !std::is_same<ClassType, typename std::tuple_element<0, std::tuple<ChainElements...>>::type>::value || (Which != 0),
        "It's not allowed to unlink the first element!" );

      unlink<sizeof...( ChainElements ) - 1>( reinterpret_cast<VkBaseOutStructure const *>( &get<ClassType, Which>() ) );
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

    bool isLinked( VkBaseInStructure const * pNext )
    {
      VkBaseInStructure const * elementPtr = reinterpret_cast<VkBaseInStructure const*>(&std::get<0>( *this ));
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
      auto & x = std::get<Index - 1>( *this );
      x.pNext  = &std::get<Index>( *this );
      link<Index - 1>();
    }

    template <size_t Index>
    typename std::enable_if<Index == 0, void>::type link() VULKAN_HPP_NOEXCEPT
    {}

    template <size_t Index>
    typename std::enable_if<Index != 0, void>::type unlink( VkBaseOutStructure const * pNext ) VULKAN_HPP_NOEXCEPT
    {
      auto & element = std::get<Index>( *this );
      if ( element.pNext == pNext )
      {
        element.pNext = pNext->pNext;
      }
      else
      {
        unlink<Index - 1>( pNext );
      }
    }

    template <size_t Index>
    typename std::enable_if<Index == 0, void>::type unlink( VkBaseOutStructure const * pNext ) VULKAN_HPP_NOEXCEPT
    {
      auto & element = std::get<0>( *this );
      if ( element.pNext == pNext )
      {
        element.pNext = pNext->pNext;
      }
      else
      {
        VULKAN_HPP_ASSERT( false );  // fires, if the ClassType member has already been unlinked !
      }
    }
  };
)";

  static const std::string classUniqueHandle = R"(
#if !defined(VULKAN_HPP_NO_SMART_HANDLE)
  template <typename Type, typename Dispatch> class UniqueHandleTraits;

  template <typename Type, typename Dispatch>
  class UniqueHandle : public UniqueHandleTraits<Type,Dispatch>::deleter
  {
  private:
    using Deleter = typename UniqueHandleTraits<Type,Dispatch>::deleter;

  public:
    using element_type = Type;

    UniqueHandle()
      : Deleter()
      , m_value()
    {}

    explicit UniqueHandle( Type const& value, Deleter const& deleter = Deleter() ) VULKAN_HPP_NOEXCEPT
      : Deleter( deleter)
      , m_value( value )
    {}

    UniqueHandle( UniqueHandle const& ) = delete;

    UniqueHandle( UniqueHandle && other ) VULKAN_HPP_NOEXCEPT
      : Deleter( std::move( static_cast<Deleter&>( other ) ) )
      , m_value( other.release() )
    {}

    ~UniqueHandle() VULKAN_HPP_NOEXCEPT
    {
      if ( m_value ) this->destroy( m_value );
    }

    UniqueHandle & operator=( UniqueHandle const& ) = delete;

    UniqueHandle & operator=( UniqueHandle && other ) VULKAN_HPP_NOEXCEPT
    {
      reset( other.release() );
      *static_cast<Deleter*>(this) = std::move( static_cast<Deleter&>(other) );
      return *this;
    }

    explicit operator bool() const VULKAN_HPP_NOEXCEPT
    {
      return m_value.operator bool();
    }

    Type const* operator->() const VULKAN_HPP_NOEXCEPT
    {
      return &m_value;
    }

    Type * operator->() VULKAN_HPP_NOEXCEPT
    {
      return &m_value;
    }

    Type const& operator*() const VULKAN_HPP_NOEXCEPT
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

    void reset( Type const& value = Type() ) VULKAN_HPP_NOEXCEPT
    {
      if ( m_value != value )
      {
        if ( m_value ) this->destroy( m_value );
        m_value = value;
      }
    }

    Type release() VULKAN_HPP_NOEXCEPT
    {
      Type value = m_value;
      m_value = nullptr;
      return value;
    }

    void swap( UniqueHandle<Type,Dispatch> & rhs ) VULKAN_HPP_NOEXCEPT
    {
      std::swap(m_value, rhs.m_value);
      std::swap(static_cast<Deleter&>(*this), static_cast<Deleter&>(rhs));
    }

  private:
    Type    m_value;
  };

  template <typename UniqueType>
  VULKAN_HPP_INLINE std::vector<typename UniqueType::element_type> uniqueToRaw(std::vector<UniqueType> const& handles)
  {
    std::vector<typename UniqueType::element_type> newBuffer(handles.size());
    std::transform(handles.begin(), handles.end(), newBuffer.begin(), [](UniqueType const& handle) { return handle.get(); });
    return newBuffer;
  }

  template <typename Type, typename Dispatch>
  VULKAN_HPP_INLINE void swap( UniqueHandle<Type,Dispatch> & lhs, UniqueHandle<Type,Dispatch> & rhs ) VULKAN_HPP_NOEXCEPT
  {
    lhs.swap( rhs );
  }
#endif
)";

  static const std::string defines = R"(
// <tuple> includes <sys/sysmacros.h> through some other header
// this results in major(x) being resolved to gnu_dev_major(x)
// which is an expression in a constructor initializer list.
#if defined(major)
  #undef major
#endif
#if defined(minor)
  #undef minor
#endif

// Windows defines MemoryBarrier which is deprecated and collides
// with the VULKAN_HPP_NAMESPACE::MemoryBarrier struct.
#if defined(MemoryBarrier)
  #undef MemoryBarrier
#endif

#if !defined(VULKAN_HPP_HAS_UNRESTRICTED_UNIONS)
# if defined(__clang__)
#  if __has_feature(cxx_unrestricted_unions)
#   define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
#  endif
# elif defined(__GNUC__)
#  define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#  if 40600 <= GCC_VERSION
#   define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
#  endif
# elif defined(_MSC_VER)
#  if 1900 <= _MSC_VER
#   define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
#  endif
# endif
#endif

#if !defined(VULKAN_HPP_INLINE)
# if defined(__clang__)
#  if __has_attribute(always_inline)
#   define VULKAN_HPP_INLINE __attribute__((always_inline)) __inline__
#  else
#   define VULKAN_HPP_INLINE inline
#  endif
# elif defined(__GNUC__)
#  define VULKAN_HPP_INLINE __attribute__((always_inline)) __inline__
# elif defined(_MSC_VER)
#  define VULKAN_HPP_INLINE inline
# else
#  define VULKAN_HPP_INLINE inline
# endif
#endif

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
# define VULKAN_HPP_TYPESAFE_EXPLICIT
#else
# define VULKAN_HPP_TYPESAFE_EXPLICIT explicit
#endif

#if defined(__cpp_constexpr)
# define VULKAN_HPP_CONSTEXPR constexpr
# if __cpp_constexpr >= 201304
#  define VULKAN_HPP_CONSTEXPR_14  constexpr
# else
#  define VULKAN_HPP_CONSTEXPR_14
# endif
# define VULKAN_HPP_CONST_OR_CONSTEXPR  constexpr
#else
# define VULKAN_HPP_CONSTEXPR
# define VULKAN_HPP_CONSTEXPR_14
# define VULKAN_HPP_CONST_OR_CONSTEXPR  const
#endif

#if !defined(VULKAN_HPP_NOEXCEPT)
# if defined(_MSC_VER) && (_MSC_VER <= 1800)
#  define VULKAN_HPP_NOEXCEPT
# else
#  define VULKAN_HPP_NOEXCEPT noexcept
#  define VULKAN_HPP_HAS_NOEXCEPT 1
# endif
#endif

#if 14 <= VULKAN_HPP_CPP_VERSION
#  define VULKAN_HPP_DEPRECATED( msg ) [[deprecated( msg )]]
#else
#  define VULKAN_HPP_DEPRECATED( msg )
#endif

#if ( 17 <= VULKAN_HPP_CPP_VERSION ) && !defined( VULKAN_HPP_NO_NODISCARD_WARNINGS )
#  define VULKAN_HPP_NODISCARD [[nodiscard]]
#  if defined(VULKAN_HPP_NO_EXCEPTIONS)
#    define VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS [[nodiscard]]
#  else
#    define VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS
#  endif
#else
#  define VULKAN_HPP_NODISCARD
#  define VULKAN_HPP_NODISCARD_WHEN_NO_EXCEPTIONS
#endif

#if !defined(VULKAN_HPP_NAMESPACE)
#define VULKAN_HPP_NAMESPACE vk
#endif

#define VULKAN_HPP_STRINGIFY2(text) #text
#define VULKAN_HPP_STRINGIFY(text) VULKAN_HPP_STRINGIFY2(text)
#define VULKAN_HPP_NAMESPACE_STRING VULKAN_HPP_STRINGIFY(VULKAN_HPP_NAMESPACE)
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
    SystemError( std::error_code ec, std::string const& what )
      : Error(), std::system_error(ec, what) {}
    SystemError( std::error_code ec, char const * what )
      : Error(), std::system_error(ec, what) {}
    SystemError( int ev, std::error_category const& ecat )
      : Error(), std::system_error(ev, ecat) {}
    SystemError( int ev, std::error_category const& ecat, std::string const& what)
      : Error(), std::system_error(ev, ecat, what) {}
    SystemError( int ev, std::error_category const& ecat, char const * what)
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
#include <string>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <vulkan/vulkan.h>

#if 17 <= VULKAN_HPP_CPP_VERSION
#include <string_view>
#endif

#if defined(VULKAN_HPP_DISABLE_ENHANCED_MODE)
# if !defined(VULKAN_HPP_NO_SMART_HANDLE)
#  define VULKAN_HPP_NO_SMART_HANDLE
# endif
#else
# include <memory>
# include <vector>
#endif

#if !defined(VULKAN_HPP_ASSERT)
# include <cassert>
# define VULKAN_HPP_ASSERT   assert
#endif

#if !defined(VULKAN_HPP_ASSERT_ON_RESULT)
# define VULKAN_HPP_ASSERT_ON_RESULT VULKAN_HPP_ASSERT
#endif

#if !defined(VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL)
# define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 1
#endif

#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL == 1
#  if defined( __linux__ ) || defined( __APPLE__ )
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

#if 201711 <= __cpp_impl_three_way_comparison
# define VULKAN_HPP_HAS_SPACESHIP_OPERATOR
#endif
#if defined(VULKAN_HPP_HAS_SPACESHIP_OPERATOR)
# include <compare>
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
  template <typename T> void ignore(T const&) VULKAN_HPP_NOEXCEPT {}

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
    operator T const& () const & VULKAN_HPP_NOEXCEPT
    {
      return value;
    }

    operator T& () & VULKAN_HPP_NOEXCEPT
    {
      return value;
    }

    operator T const&& () const && VULKAN_HPP_NOEXCEPT
    {
      return std::move( value );
    }

    operator T&& () && VULKAN_HPP_NOEXCEPT
    {
      return std::move( value );
    }
#endif
  };

#if !defined(VULKAN_HPP_NO_SMART_HANDLE)
  template <typename Type, typename Dispatch>
  struct ResultValue<UniqueHandle<Type,Dispatch>>
  {
#ifdef VULKAN_HPP_HAS_NOEXCEPT
    ResultValue(Result r, UniqueHandle<Type, Dispatch> && v) VULKAN_HPP_NOEXCEPT
#else
    ResultValue(Result r, UniqueHandle<Type, Dispatch> && v)
#endif
      : result(r)
      , value(std::move(v))
    {}

    Result                        result;
    UniqueHandle<Type, Dispatch>  value;

    operator std::tuple<Result&, UniqueHandle<Type, Dispatch>&>() VULKAN_HPP_NOEXCEPT { return std::tuple<Result&, UniqueHandle<Type, Dispatch>&>(result, value); }

#  if !defined(VULKAN_HPP_DISABLE_IMPLICIT_RESULT_VALUE_CAST)
    operator UniqueHandle<Type, Dispatch>& () & VULKAN_HPP_NOEXCEPT
    {
      return value;
    }

    operator UniqueHandle<Type, Dispatch>() VULKAN_HPP_NOEXCEPT
    {
      return std::move(value);
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
    VULKAN_HPP_ASSERT_ON_RESULT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#else
    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
    {
      throwResultException( result, message );
    }
#endif
    return ResultValue<T>( result, data );
  }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename T, typename D>
  VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<T,D>>::type createResultValue( Result result, T & data, char const * message, typename UniqueHandleTraits<T,D>::deleter const& deleter )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore(message);
    VULKAN_HPP_ASSERT_ON_RESULT( result == Result::eSuccess );
    return ResultValue<UniqueHandle<T,D>>( result, UniqueHandle<T,D>(data, deleter) );
#else
    if ( result != Result::eSuccess )
    {
      throwResultException( result, message );
    }
    return UniqueHandle<T,D>(data, deleter);
#endif
  }

  template <typename T, typename D>
  VULKAN_HPP_INLINE ResultValue<UniqueHandle<T,D>> createResultValue( Result result, T & data, char const * message, std::initializer_list<Result> successCodes, typename UniqueHandleTraits<T,D>::deleter const& deleter )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore(message);
    VULKAN_HPP_ASSERT_ON_RESULT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
    return ResultValue<UniqueHandle<T,D>>( result, UniqueHandle<T,D>(data, deleter) );
#else
    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
    {
      throwResultException( result, message );
    }
    return ResultValue<UniqueHandle<T,D>>( result, UniqueHandle<T,D>(data, deleter) );
#endif
  }
#endif
)";

  static const std::string typeTraits = R"(
  template <typename EnumType, EnumType value>
  struct CppType
  {};
)";

  try
  {
    tinyxml2::XMLDocument doc;

    std::string filename = ( argc == 1 ) ? VK_SPEC : argv[1];
    std::cout << "Loading vk.xml from " << filename << std::endl;
    std::cout << "Writing vulkan.hpp to " << VULKAN_HPP_FILE << std::endl;

    tinyxml2::XMLError error = doc.LoadFile( filename.c_str() );
    if ( error != tinyxml2::XML_SUCCESS )
    {
      std::cout << "VulkanHppGenerator: failed to load file " << filename << " with error <" << to_string( error )
                << ">" << std::endl;
      return -1;
    }

    VulkanHppGenerator generator( doc );

    std::string         str;
    static const size_t estimatedLength = 4 * 1024 * 1024;
    str.reserve( estimatedLength );
    str += generator.getVulkanLicenseHeader() + includes + "\n";
    appendVersionCheck( str, generator.getVersion() );
    appendTypesafeStuff( str, generator.getTypesafeCheck() );
    str += defines + "\n" + "namespace VULKAN_HPP_NAMESPACE\n" + "{\n" + classArrayProxy + classArrayWrapper +
           classFlags + classOptional + classStructureChain + classUniqueHandle;
    generator.appendDispatchLoaderStatic( str );
    generator.appendDispatchLoaderDefault( str );
    str += classObjectDestroy + classObjectFree + classPoolFree + "\n";
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
    generator.appendForwardDeclarations( str );
    generator.appendHandles( str );
    generator.appendStructs( str );
    generator.appendHandlesCommandDefintions( str );
    generator.appendStructureChainValidation( str );
    generator.appendDispatchLoaderDynamic( str );
    str += "} // namespace VULKAN_HPP_NAMESPACE\n";
    generator.appendHashStructures( str );
    str += "#endif\n";

    std::ofstream ofs( VULKAN_HPP_FILE );
    assert( !ofs.fail() );
    ofs << str;
    ofs.close();

#if defined( CLANG_FORMAT_EXECUTABLE )
    std::cout << "VulkanHppGenerator: formatting vulkan.hpp using clang-format...";
    int ret = std::system( "\"" CLANG_FORMAT_EXECUTABLE "\" -i --style=file " VULKAN_HPP_FILE );
    if ( ret != 0 )
    {
      std::cout << "VulkanHppGenerator: failed to format file " << filename << " with error <" << ret << ">\n";
      return -1;
    }
#else
    std::cout
      << "VulkanHppGenerator: could not find clang-format. The generated vulkan.hpp will not be formatted accordingly.\n";
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
