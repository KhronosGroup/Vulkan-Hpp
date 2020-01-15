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

#include <cassert>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <exception>
#include <regex>
#include <iterator>
#include "VulkanHppGenerator.hpp"

void appendArgumentCount(std::string & str, size_t vectorIndex, std::string const& vectorName, std::string const& counterName, size_t returnParamIndex, size_t templateParamIndex, bool twoStep, bool singular);
std::string appendFunctionBodyEnhancedLocalReturnVariableSingular(std::string & str, std::string const& indentation, std::string const& returnName, std::string const& typeName, bool isStructureChain);
void appendReinterpretCast(std::string & str, bool leadingConst, std::string const& type, bool trailingPointerToConst);
void appendTypesafeStuff(std::string &str, std::string const& typesafeCheck);
void appendVersionCheck(std::string & str, std::string const& version);
bool beginsWith(std::string const& text, std::string const& prefix);
bool endsWith(std::string const& text, std::string const& postfix);
template <typename T> void checkAlias(std::map<std::string, T> const& data, std::string const& name, int line);
void checkAttributes(std::map<std::string, std::string> const& attributes, int line, std::map<std::string, std::set<std::string>> const& required, std::map<std::string, std::set<std::string>> const& optional);
void checkElements(std::vector<tinyxml2::XMLElement const*> const& elements, std::set<std::string> const& values);
void checkEmptyElement(tinyxml2::XMLElement const* element);
void checkOrderedElements(std::vector<tinyxml2::XMLElement const*> const& elements, std::vector<std::string> const& values);
void cleanup(std::stringstream & ss);
std::string constructArraySize(std::vector<std::string> const& sizes);
std::string constructStandardArray(std::string const& type, std::vector<std::string> const& sizes);
std::string createEnumValueName(std::string const& name, std::string const& prefix, std::string const& postfix, bool bitmask, std::string const& tag);
std::string determineCommandName(std::string const& vulkanCommandName, std::string const& firstArgumentType);
std::set<size_t> determineSkippedParams(size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices);
bool determineStructureChaining(std::string const& structType, std::set<std::string> const& extendedStructs, std::map<std::string, std::string> const& structureAliases);
std::string findTag(std::set<std::string> const& tags, std::string const& name, std::string const& postfix = "");
std::pair<bool, std::string> generateFunctionBodyStandardReturn(std::string const& returnType);
std::map<std::string, std::string> getAttributes(tinyxml2::XMLElement const* element);
std::vector<tinyxml2::XMLElement const*> getChildElements(tinyxml2::XMLElement const* element);
std::string getEnumPostfix(std::string const& name, std::set<std::string> const& tags, std::string & prefix);
std::string getEnumPrefix(std::string const& name, bool bitmask);
std::pair<std::vector<std::string>, std::string> readModifier(tinyxml2::XMLNode const* node);
void readStructStructExtends(std::map<std::string, std::string> const& attributes, std::vector<std::string> & structExtends, std::set<std::string> & extendedStructs);
std::string readTypePostfix(tinyxml2::XMLNode const* node);
std::string readTypePrefix(tinyxml2::XMLNode const* node);
std::string replaceWithMap(std::string const &input, std::map<std::string, std::string> replacements);
std::string startLowerCase(std::string const& input);
std::string startUpperCase(std::string const& input);
std::string stripPostfix(std::string const& value, std::string const& postfix);
std::string stripPluralS(std::string const& name);
std::string stripPrefix(std::string const& value, std::string const& prefix);
std::string toCamelCase(std::string const& value);
std::string toUpperCase(std::string const& name);
std::vector<std::string> tokenize(std::string tokenString, char separator);
std::string trim(std::string const& input);
std::string trimEnd(std::string const& input);

const std::set<std::string> nonConstSTypeStructs = { "VkBaseInStructure", "VkBaseOutStructure" };

void appendArgumentCount(std::string & str, size_t vectorIndex, std::string const& vectorName, std::string const& counterName, size_t returnParamIndex, size_t templateParamIndex, bool twoStep, bool singular)
{
  // this parameter is a count parameter for a vector parameter
  if ((returnParamIndex == vectorIndex) && twoStep)
  {
    // the corresponding vector parameter is the return parameter and it's a two-step algorithm
    // -> use the pointer to a local variable named like the counter parameter without leading 'p'
    assert((counterName[0] == 'p') && isupper(counterName[1]));
    str += "&" + startLowerCase(stripPrefix(counterName, "p"));
  }
  else
  {
    // the corresponding vector parameter is not the return parameter, or it's not a two-step algorithm
    if (singular)
    {
      // for the singular version, the count is just 1.
      str += "1 ";
    }
    else
    {
      // for the non-singular version, the count is the size of the vector parameter
      // -> use the vector parameter name without leading 'p' to get the size (in number of elements, not in bytes)
      assert(vectorName[0] == 'p');
      str += startLowerCase(stripPrefix(vectorName, "p")) + ".size() ";
    }
    if (templateParamIndex == vectorIndex)
    {
      // if the vector parameter is templatized -> multiply by the size of that type to get the size in bytes
      str += "* sizeof( T ) ";
    }
  }
}

std::string appendFunctionBodyEnhancedLocalReturnVariableSingular(std::string & str, std::string const& indentation, std::string const& returnName, std::string const& typeName, bool isStructureChain)
{
  std::string strippedReturnName = stripPluralS(returnName);
  if (isStructureChain)
  {
    // For StructureChains use the template parameters
    str +=
      "StructureChain<X, Y, Z...> structureChain;\n" +
      indentation + "  " + typeName + "& " + strippedReturnName + " = structureChain.template get<" + typeName + ">()";
    strippedReturnName = "structureChain";
  }
  else
  {
    // in singular case, just use the return parameters pure type for the return variable
    str += typeName + " " + strippedReturnName;
  }
  return strippedReturnName;
}

void appendReinterpretCast(std::string & str, bool leadingConst, std::string const& type, bool trailingPointerToConst)
{
  str += "reinterpret_cast<";
  if (leadingConst)
  {
    str += "const ";
  }
  str += type;
  if (trailingPointerToConst)
  {
    str += "* const";
  }
  str += "*>";
}

void appendTypesafeStuff(std::string & str, std::string const& typesafeCheck)
{
  str += "// 32-bit vulkan is not typesafe for handles, so don't allow copy constructors on this platform by default.\n"
    "// To enable this feature on 32-bit platforms please define VULKAN_HPP_TYPESAFE_CONVERSION\n"
    + typesafeCheck + "\n"
    "# if !defined( VULKAN_HPP_TYPESAFE_CONVERSION )\n"
    "#  define VULKAN_HPP_TYPESAFE_CONVERSION\n"
    "# endif\n"
    "#endif\n";
}

void appendVersionCheck(std::string & str, std::string const& version)
{
  str += "static_assert( VK_HEADER_VERSION == " + version + " , \"Wrong VK_HEADER_VERSION!\" );\n"
    "\n";
}

bool beginsWith(std::string const& text, std::string const& prefix)
{
  return !prefix.empty() && text.substr(0, prefix.length()) == prefix;
}

bool endsWith(std::string const& text, std::string const& postfix)
{
  return !postfix.empty() && (postfix.length() <= text.length()) && (text.substr(text.length() - postfix.length()) == postfix);
}

template <typename T>
void checkAlias(std::map<std::string, T> const& data, std::string const& name, int line)
{
  if (data.find(name) == data.end())
  {
    throw std::runtime_error("Spec error on line " + std::to_string(line) + ": missing alias <" + name + ">");
  }
}

// check the validity of an attributes map
// attributes : the map of name/value pairs of the encountered attributes
// line       : the line in the xml file where the attributes are listed
// required   : the required attributes, with a set of allowed values per attribute
// optional   : the optional attributes, with a set of allowed values per attribute
void checkAttributes(std::map<std::string, std::string> const& attributes, int line, std::map<std::string, std::set<std::string>> const& required, std::map<std::string, std::set<std::string>> const& optional)
{
  // check if all required attributes are included and if there is a set of allowed values, check if the actual value is part of that set
  for (auto const& r : required)
  {
    auto attributesIt = attributes.find(r.first);
    if (attributesIt == attributes.end())
    {
      throw std::runtime_error("Spec error on line " + std::to_string(line) + ": missing attribute <" + r.first + ">");
    }
    if (!r.second.empty() && (r.second.find(attributesIt->second) == r.second.end()))
    {
      throw std::runtime_error("Spec error on line " + std::to_string(line) + ": unexpected attribute value <" + attributesIt->second + "> in attribute <" + r.first + ">");
    }
  }
  // check if all not required attributes or optional, and if there is a set of allowed values, check if the actual value is part of that set
  for (auto const& a : attributes)
  {
    if (required.find(a.first) == required.end())
    {
      auto optionalIt = optional.find(a.first);
      if (optionalIt == optional.end())
      {
        std::cerr << "warning: " << "Unknown attribute " + a.first + " in line " + std::to_string(line) + "!" << std::endl;
        continue;
      }
      if (!optionalIt->second.empty())
      {
        std::vector<std::string> values = tokenize(a.second, ',');
        for (auto const& v : values)
        {
          if (optionalIt->second.find(v) == optionalIt->second.end())
          {
            throw std::runtime_error("Spec error on line " + std::to_string(line) + ": unexpected attribute value <" + v + "> in attribute <" + a.first + ">");
          }
        }
      }
    }
  }
}

void checkElements(std::vector<tinyxml2::XMLElement const*> const& elements, std::set<std::string> const& values)
{
  for (auto e : elements)
  {
    if (values.find(e->Value()) == values.end())
    {
      std::cerr << "warning: Unknown element in spec on line: " << std::to_string(e->GetLineNum()) << " " << e->Value() << "!" << std::endl;
    }
  }
}

void checkEmptyElement(tinyxml2::XMLElement const* element)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), {}, {});
  checkElements(getChildElements(element), {});
}

void checkOrderedElements(std::vector<tinyxml2::XMLElement const*> const& elements, std::vector<std::string> const& values)
{
  for (size_t i = 0; i < elements.size(); i++)
  {
    if (values.size() <= i)
    {
      throw std::runtime_error("Spec error on line " + std::to_string(elements[i]->GetLineNum()) + ": unexpected surplus element <" + elements[i]->Value() + ">");
    }
    if (values[i] != elements[i]->Value())
    {
      throw std::runtime_error("Spec error on line " + std::to_string(elements[i]->GetLineNum()) + ": unexpected element <" + elements[i]->Value() + ">, expected <" + values[i] + ">");
    }
  }
}

void cleanup(std::string &str)
{
  std::map<std::string,std::string> replacements =
  {
    { "\n\n\n",   "\n\n" },
    { "{\n\n",    "{\n" },
    { "\n\n  }",  "\n  }" }
  };
  for (auto const& repl : replacements)
  {
    std::string::size_type pos = str.find(repl.first);
    while (pos != std::string::npos)
    {
      str.replace(pos, repl.first.length(), repl.second);
      pos = str.find(repl.first, pos);
    }
  }
}

std::string constructArraySize(std::vector<std::string> const& sizes)
{
  std::string arraySize;
  for (auto const& s : sizes)
  {
    arraySize += s + " * ";
  }
  return arraySize.substr(0, arraySize.length() - 3);
}

std::string constructCArraySizes(std::vector<std::string> const& sizes)
{
  std::string arraySizes;
  for (auto const& s : sizes)
  {
    arraySizes += "[" + s + "]";
  }
  return arraySizes;
}

std::string constructStandardArray(std::string const& type, std::vector<std::string> const& sizes)
{
  std::string arrayString = "std::array<" + type + "," + sizes.back() + ">";
  for (size_t i = sizes.size() - 2; i < sizes.size(); i--)
  {
    arrayString = "std::array<" + arrayString + "," + sizes[i] + ">";
  }
  return arrayString;
}

std::string createEnumValueName(std::string const& name, std::string const& prefix, std::string const& postfix, bool bitmask, std::string const& tag)
{
  std::string result = "e" + toCamelCase(stripPostfix(stripPrefix(name, prefix), postfix));
  if (bitmask)
  {
    size_t pos = result.find("Bit");
    if (pos != std::string::npos)
    {
      result.erase(pos, 3);
    }
  }
  if (!tag.empty() && (result.substr(result.length() - tag.length()) == toCamelCase(tag)))
  {
    result = result.substr(0, result.length() - tag.length()) + tag;
  }
  return result;
}

std::string determineCommandName(std::string const& vulkanCommandName, std::string const& firstArgumentType)
{
  std::string commandName(startLowerCase(stripPrefix(vulkanCommandName, "vk")));
  std::string searchName = stripPrefix(firstArgumentType, "Vk");
  size_t pos = commandName.find(searchName);
  if ((pos == std::string::npos) && isupper(searchName[0]))
  {
    searchName[0] = static_cast<char>(tolower(searchName[0]));
    pos = commandName.find(searchName);
  }
  if (pos != std::string::npos)
  {
    commandName.erase(pos, searchName.length());
  }
  else if ((searchName == "commandBuffer") && beginsWith(commandName, "cmd"))
  {
    commandName.erase(0, 3);
    pos = 0;
  }
  if ((pos == 0) && isupper(commandName[0]))
  {
    commandName[0] = static_cast<char>(tolower(commandName[0]));
  }
  return commandName;
}

std::set<size_t> determineSkippedParams(size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices)
{
  std::set<size_t> skippedParams;

  // the size-parameters of vector parameters are not explicitly used in the enhanced API
  std::for_each(vectorParamIndices.begin(), vectorParamIndices.end(), [&skippedParams](std::pair<size_t, size_t> const& vp) { if (vp.second != INVALID_INDEX) skippedParams.insert(vp.second); });

  // and the return parameter is also skipped
  if (returnParamIndex != INVALID_INDEX)
  {
    skippedParams.insert(returnParamIndex);
  }
  return skippedParams;
}

bool determineStructureChaining(std::string const& structType, std::set<std::string> const& extendedStructs, std::map<std::string, std::string> const& structureAliases)
{
  bool isStructureChained = (extendedStructs.find(structType) != extendedStructs.end());
  if (!isStructureChained)
  {
    auto aliasIt = structureAliases.find(structType);
    if ((aliasIt != structureAliases.end()))
    {
      isStructureChained = (extendedStructs.find(aliasIt->second) != extendedStructs.end());
    }
  }
  return isStructureChained;
}

std::string findTag(std::set<std::string> const& tags, std::string const& name, std::string const& postfix)
{
  auto tagIt = std::find_if(tags.begin(), tags.end(), [&name, &postfix](std::string const& t) { return endsWith(name, t + postfix); });
  return (tagIt != tags.end()) ? *tagIt : "";
}

std::pair<bool, std::string> generateFunctionBodyStandardReturn(std::string const& returnType)
{
  bool castReturn = false;
  std::string ret;
  if (returnType != "void")
  {
    // there's something to return...
    ret = "return ";

    castReturn = beginsWith(returnType, "Vk");
    if (castReturn)
    {
      // the return-type is a vulkan type -> need to cast to VULKAN_HPP_NAMESPACE-type
      ret += "static_cast<" + stripPrefix(returnType, "Vk") + ">( ";
    }
  }
  return std::make_pair(castReturn, ret);
}

std::map<std::string, std::string> getAttributes(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes;
  for (auto attribute = element->FirstAttribute(); attribute; attribute = attribute->Next())
  {
    assert(attributes.find(attribute->Name()) == attributes.end());
    attributes[attribute->Name()] = attribute->Value();
  }
  return attributes;
}

std::vector<tinyxml2::XMLElement const*> getChildElements(tinyxml2::XMLElement const* element)
{
  std::vector<tinyxml2::XMLElement const*> childElements;
  for (tinyxml2::XMLElement const* childElement = element->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement())
  {
    childElements.push_back(childElement);
  }
  return childElements;
}

std::string getEnumPostfix(std::string const& name, std::set<std::string> const& tags, std::string & prefix)
{
  std::string postfix;
  if (name != "VkResult")
  {
    // if the enum name contains a tag move it from the prefix to the postfix to generate correct enum value names.
    for (auto const& tag : tags)
    {
      if (endsWith(prefix, tag + "_"))
      {
        prefix.erase(prefix.length() - tag.length() - 1);
        postfix = "_" + tag;
        break;
      }
      else if (endsWith(name, tag))
      {
        postfix = "_" + tag;
        break;
      }
    }
  }
  return postfix;
}

std::string getEnumPrefix(std::string const& name, bool bitmask)
{
  std::string prefix;
  if (name == "VkResult")
  {
    prefix = "VK_";
  }
  else if (bitmask)
  {
    // for a bitmask enum, start with "VK", cut off the trailing "FlagBits", and convert that name to upper case
    // end that with "Bit"
    size_t pos = name.find("FlagBits");
    assert(pos != std::string::npos);
    prefix = toUpperCase(name.substr(0, pos)) + "_";
  }
  else
  {
    // for a non-bitmask enum, convert the name to upper case
    prefix = toUpperCase(name) + "_";
  }
  return prefix;
}

std::string extractTag(std::string const& name, std::set<std::string> const& tags)
{
  // extract the tag from the name, which is supposed to look like VK_<tag>_<other>
  size_t tagStart = name.find('_');
  assert(tagStart != std::string::npos);
  size_t tagEnd = name.find('_', tagStart + 1);
  assert(tagEnd != std::string::npos);
  std::string tag = name.substr(tagStart + 1, tagEnd - tagStart - 1);
  assert(tags.find(tag) != tags.end());
  return tag;
}

std::pair<std::vector<std::string>, std::string> readModifiers(tinyxml2::XMLNode const* node)
{
  std::vector<std::string> arraySizes;
  std::string bitCount;
  if (node && node->ToText())
  {
    // following the name there might be some array size
    assert(!node->ToElement());
    std::string value = node->Value();
    assert(!value.empty());
    if (value[0] == '[')
    {
      std::string::size_type endPos = 0;
      while (endPos + 1 != value.length())
      {
        std::string::size_type startPos = value.find('[', endPos);
        assert(startPos != std::string::npos);
        endPos = value.find(']', startPos);
        assert(endPos != std::string::npos);
        assert(startPos + 2 <= endPos);
        arraySizes.push_back(value.substr(startPos + 1, endPos - startPos - 1));
      }
    }
    else
    {
      assert(value[0] == ':');
      bitCount = value.substr(1);
    }
  }
  return std::make_pair(arraySizes, bitCount);;
}

void readStructStructExtends(std::map<std::string, std::string> const& attributes, std::vector<std::string> & structExtends, std::set<std::string> & extendedStructs)
{
  auto attributesIt = attributes.find("structextends");
  if (attributesIt != attributes.end())
  {
    std::vector<std::string> extends = tokenize(attributesIt->second, ',');
    structExtends.insert(structExtends.end(), extends.begin(), extends.end());
    extendedStructs.insert(extends.begin(), extends.end());
    assert(!structExtends.empty());
  }
}

std::string readTypePostfix(tinyxml2::XMLNode const* node)
{
  std::string postfix;
  if (node && node->ToText())
  {
    postfix = trimEnd(node->Value());
    assert((postfix == "*") || (postfix == "**") || (postfix == "* const*"));
  }
  return postfix;
}

std::string readTypePrefix(tinyxml2::XMLNode const* node)
{
  std::string prefix;
  if (node && node->ToText())
  {
    prefix = trim(node->Value());
    assert((prefix == "const") || (prefix == "struct") || (prefix == "const struct"));
  }
  return prefix;
}

std::string replaceWithMap(std::string const &input, std::map<std::string, std::string> replacements)
{
  // This will match ${someVariable} and contain someVariable in match group 1
  std::regex re(R"(\$\{([^\}]+)\})");
  auto it = std::sregex_iterator(input.begin(), input.end(), re);
  auto end = std::sregex_iterator();

  // No match, just return the original string
  if (it == end)
  {
    return input;
  }

  std::string result = "";
  while (it != end)
  {
    std::smatch match = *it;
    auto itReplacement = replacements.find(match[1].str());
    assert(itReplacement != replacements.end());

    result += match.prefix().str() + ((itReplacement != replacements.end()) ? itReplacement->second : match[0].str());
    ++it;

    // we've passed the last match. Append the rest of the orignal string
    if (it == end)
    {
      result += match.suffix().str();
    }
  }
  return result;
}

std::string startLowerCase(std::string const& input)
{
  return input.empty() ? "" : static_cast<char>(tolower(input[0])) + input.substr(1);
}

std::string startUpperCase(std::string const& input)
{
  return input.empty() ? "" : static_cast<char>(toupper(input[0])) + input.substr(1);
}

std::string stripPostfix(std::string const& value, std::string const& postfix)
{
  std::string strippedValue = value;
  if (endsWith(strippedValue, postfix))
  {
    strippedValue.erase(strippedValue.length() - postfix.length());
  }
  return strippedValue;
}

std::string stripPluralS(std::string const& name)
{
  std::string strippedName(name);
  size_t pos = strippedName.rfind('s');
  assert(pos != std::string::npos);
  strippedName.erase(pos, 1);
  return strippedName;
}

std::string stripPrefix(std::string const& value, std::string const& prefix)
{
  std::string strippedValue = value;
  if (beginsWith(strippedValue, prefix))
  {
    strippedValue.erase(0, prefix.length());
  }
  return strippedValue;
}

std::string toCamelCase(std::string const& value)
{
  assert(!value.empty() && (isupper(value[0]) || isdigit(value[0])));
  std::string result;
  result.reserve(value.size());
  bool keepUpper = true;
  for (auto c : value)
  {
    if (c == '_')
    {
      keepUpper = true;
    }
    else if (isdigit(c))
    {
      keepUpper = true;
      result.push_back(c);
    }
    else if (keepUpper)
    {
      result.push_back(c);
      keepUpper = false;
    }
    else
    {
      result.push_back(static_cast<char>(tolower(c)));
    }
  }
  return result;
}

std::string toUpperCase(std::string const& name)
{
  std::string convertedName;
  convertedName.reserve(name.size());

  bool lowerOrDigit = false;
  for (auto c : name)
  {
    if (islower(c) || isdigit(c))
    {
      lowerOrDigit = true;
    }
    else if (lowerOrDigit)
    {
      convertedName.push_back('_');
      lowerOrDigit = false;
    }
    convertedName.push_back(static_cast<char>(toupper(c)));
  }

  return convertedName;
}

std::vector<std::string> tokenize(std::string tokenString, char separator)
{
  std::vector<std::string> tokens;
  size_t start = 0, end;
  do
  {
    end = tokenString.find(separator, start);
    tokens.push_back(tokenString.substr(start, end - start));
    start = end + 1;
  } while (end != std::string::npos);
  return tokens;
}

std::string trim(std::string const& input)
{
  std::string result = input;
  result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](char c) { return !std::isspace(c); }));
  result.erase(std::find_if(result.rbegin(), result.rend(), [](char c) { return !std::isspace(c); }).base(), result.end());
  return result;
}

std::string trimEnd(std::string const& input)
{
  std::string result = input;
  result.erase(std::find_if(result.rbegin(), result.rend(), [](char c) { return !std::isspace(c); }).base(), result.end());
  return result;
}

VulkanHppGenerator::VulkanHppGenerator()
{
  m_handles.insert(std::make_pair("", HandleData()));   // insert the default "handle" without class (for createInstance, and such)
#if !defined(NDEBUG)
#endif
}

void VulkanHppGenerator::appendArgumentPlainType(std::string & str, ParamData const& paramData) const
{
  // this parameter is just a plain type
  if (!paramData.type.postfix.empty())
  {
    assert(paramData.type.postfix.back() == '*');
    // it's a pointer
    std::string parameterName = startLowerCase(stripPrefix(paramData.name, "p"));
    if (paramData.type.prefix.find("const") != std::string::npos)
    {
      // it's a const pointer
      if (paramData.type.type == "char")
      {
        // it's a const pointer to char -> it's a string -> get the data via c_str()
        str += parameterName + (paramData.optional ? (" ? " + parameterName + "->c_str() : nullptr") : ".c_str()");
      }
      else
      {
        // it's const pointer to something else -> just use the name
        assert(!paramData.optional);
        str += paramData.name;
      }
    }
    else
    {
      // it's a non-const pointer, and char is the only type that occurs -> use the address of the parameter
      assert(paramData.type.type.find("char") == std::string::npos);
      str += "&" + parameterName;
    }
  }
  else
  {
    // it's a plain parameter -> just use its name
    str += paramData.name;
  }
}

void VulkanHppGenerator::appendArguments(std::string & str, CommandData const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool firstCall, bool singular, size_t from, size_t to) const
{
  assert(from <= to);

  bool encounteredArgument = false;
  for (size_t i = from; i < to; i++)
  {
    if (encounteredArgument)
    {
      str += ", ";
    }

    auto it = vectorParamIndices.find(i);
    if (it != vectorParamIndices.end())
    {
      appendArgumentVector(str, it->first, commandData.params[it->first], returnParamIndex, templateParamIndex, twoStep, firstCall, singular);
    }
    else
    {
      it = find_if(vectorParamIndices.begin(), vectorParamIndices.end(), [i](std::pair<size_t, size_t> const& vpi) { return vpi.second == i; });
      if (it != vectorParamIndices.end())
      {
        appendArgumentCount(str, it->first, commandData.params[it->first].name, commandData.params[it->second].name, returnParamIndex, templateParamIndex, twoStep, singular);
      }
      else if (beginsWith(commandData.params[i].type.type, "Vk"))
      {
        appendArgumentVulkanType(str, commandData.params[i]);
      }
      else
      {
        appendArgumentPlainType(str, commandData.params[i]);
      }
    }
    encounteredArgument = true;
  }
}

void VulkanHppGenerator::appendArgumentVector(std::string & str, size_t paramIndex, ParamData const& paramData, size_t returnParamIndex, size_t templateParamIndex, bool twoStep, bool firstCall, bool singular) const
{
  // this parameter is a vector parameter
  assert(paramData.type.postfix.back() == '*');
  if ((returnParamIndex == paramIndex) && twoStep && firstCall)
  {
    // this parameter is the return parameter, and it's the first call of a two-step algorithm -> just just nullptr
    str += "nullptr";
  }
  else
  {
    std::string parameterName = startLowerCase(stripPrefix(paramData.name, "p"));
    if (beginsWith(paramData.type.type, "Vk") || (paramIndex == templateParamIndex))
    {
      // CHECK for !commandData.params[it->first].optional

      // this parameter is a vulkan type or a templated type -> need to reinterpret cast
      appendReinterpretCast(str, paramData.type.prefix.find("const") == 0, paramData.type.type, paramData.type.postfix.rfind("* const") != std::string::npos);
      str += "( " + (singular ? ("&" + stripPluralS(parameterName)) : (parameterName + ".data()")) + " )";
    }
    else if (paramData.type.type == "char")
    {
      // the parameter is a vector to char -> it might be optional
      // besides that, the parameter now is a std::string -> get the pointer via c_str()
      str += parameterName + (paramData.optional ? (" ? " + parameterName + "->c_str() : nullptr") : ".c_str()");
    }
    else
    {
      // this parameter is just a vetor -> get the pointer to its data
      str += parameterName + ".data()";
    }
  }
}

void VulkanHppGenerator::appendArgumentVulkanType(std::string & str, ParamData const& paramData) const
{
  // this parameter is a vulkan type
  if (!paramData.type.postfix.empty())
  {
    assert(paramData.type.postfix.back() == '*');
    // it's a pointer -> needs a reinterpret cast to the vulkan type
    std::string parameterName = startLowerCase(stripPrefix(paramData.name, "p"));
    appendReinterpretCast(str, paramData.type.prefix.find("const") != std::string::npos, paramData.type.type, false);
    str += "( ";
    if (paramData.optional)
    {
      // for an optional parameter, we need also a static_cast from optional type to const-pointer to pure type
      str += "static_cast<const " + stripPrefix(paramData.type.type, "Vk") + "*>( " + parameterName + " )";
    }
    else
    {
      // other parameters can just use the pointer
      str += "&" + parameterName;
    }
    str += " )";
  }
  else
  {
    // a non-pointer parameter needs a static_cast from VULKAN_HPP_NAMESPACE-type to vulkan type
    str += "static_cast<" + paramData.type.type + ">( " + paramData.name + " )";
  }
}

void VulkanHppGenerator::appendBaseTypes(std::string & str) const
{
  assert(!m_baseTypes.empty());
  for (auto const& baseType : m_baseTypes)
  {
    if (baseType.first != "VkFlags")    // filter out VkFlags
    {
      str += "  using " + stripPrefix(baseType.first, "Vk") + " = " + baseType.second + ";\n";
    }
  }
}

void VulkanHppGenerator::appendBitmasks(std::string & str) const
{
  for (auto const& bitmask : m_bitmasks)
  {
    auto bitmaskBits = m_enums.find(bitmask.second.requirement);
    bool hasBits = (bitmaskBits != m_enums.end());
    if (!bitmask.second.requirement.empty() && !hasBits)
    {
      throw std::runtime_error(bitmask.first + " references the undefined requirement '" + bitmask.second.requirement + "'");
    }

    std::string strippedBitmaskName = stripPrefix(bitmask.first, "Vk");
    std::string strippedEnumName = hasBits ? stripPrefix(bitmaskBits->first, "Vk") : "";

    str += "\n";
    appendPlatformEnter(str, bitmask.second.platform);
    appendBitmask(str, strippedBitmaskName, bitmask.second.alias, strippedEnumName, hasBits ? bitmaskBits->second.values : std::vector<EnumValueData>());
    appendBitmaskToStringFunction(str, strippedBitmaskName, strippedEnumName, hasBits ? bitmaskBits->second.values : std::vector<EnumValueData>());
    appendPlatformLeave(str, bitmask.second.platform);
  }
}

void VulkanHppGenerator::appendBitmask(std::string & str, std::string const& bitmaskName, std::string const& bitmaskAlias, std::string const& enumName, std::vector<EnumValueData> const& enumValues) const
{
  // each Flags class is using the class 'Flags' with the corresponding FlagBits enum as the template parameter
  // if there's no enum for the FlagBits, introduce an artificial empty one
  std::string emptyEnumName;
  if (enumName.empty())
  {
    emptyEnumName = bitmaskName;
    size_t pos = emptyEnumName.rfind("Flags");
    assert(pos != std::string::npos);
    emptyEnumName.replace(pos, 5, "FlagBits");

    // if this emptyEnumName is not in the list of enums, list it here
    if (m_enums.find("Vk" + emptyEnumName) == m_enums.end())
    {
      const std::string templateString = R"x(  enum class ${enumName}
  {};

  VULKAN_HPP_INLINE std::string to_string( ${enumName} )
  {
    return "(void)";
  }
)x";

      str += replaceWithMap(templateString, { { "enumName", emptyEnumName } });
    }
  }
  str += "\n"
    "  using " + bitmaskName + " = Flags<" + (enumName.empty() ? emptyEnumName : enumName) + ", Vk" + bitmaskName + ">;\n";

  if (!enumValues.empty())
  {
    std::string allFlags;
    for (auto const& value : enumValues)
    {
      if (!allFlags.empty())
      {
        allFlags += " | ";
      }
      allFlags += "VkFlags(" + enumName + "::" + value.vkValue + ")";
    }

    static const std::string bitmaskOperatorsTemplate = R"(
  template <> struct FlagTraits<${enumName}>
  {
    enum
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

    str += replaceWithMap(bitmaskOperatorsTemplate, { { "bitmaskName", bitmaskName },{ "enumName", enumName },{ "allFlags", allFlags } });
  }

  if (!bitmaskAlias.empty())
  {
    str += "\n"
      "  using " + stripPrefix(bitmaskAlias, "Vk") + " = " + bitmaskName + ";\n";
  }
}

void VulkanHppGenerator::appendBitmaskToStringFunction(std::string & str, std::string const& bitmaskName, std::string const& enumName, std::vector<EnumValueData> const& enumValues) const
{
  str += "\n"
    "  VULKAN_HPP_INLINE std::string to_string( " + bitmaskName + (enumValues.empty() ? " " : " value ") + " )\n"
    "  {\n";

  if (enumValues.empty())
  {
    str += "\n    return \"{}\";\n";
  }
  else
  {
    // 'or' together all the bits in the value
    str += "\n"
      "    if ( !value ) return \"{}\";\n"
      "    std::string result;\n";
    for (auto const& evd : enumValues)
    {
      if (evd.singleBit)
      {
        str += "\n"
          "    if ( value & " + enumName + "::" + evd.vkValue + " ) result += \"" + evd.vkValue.substr(1) + " | \";";
      }
    }
    str += "\n"
      "    return \"{ \" + result.substr(0, result.size() - 3) + \" }\";\n";
  }

  str += "  }\n";
}

void VulkanHppGenerator::appendCall(std::string & str, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool firstCall, bool singular) const
{
  // the original function call
  str += "d." + commandData.first + "( ";

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;
  if (!handle.empty())
  {
    // if it's member of a class -> the first argument is the member variable, starting with "m_"
    assert(handle == commandData.second.params[0].type.type);
    str += "m_" + startLowerCase(stripPrefix(handle, "Vk"));
    if (1 < commandData.second.params.size())
    {
      str += ", ";
    }
  }

  appendArguments(str, commandData.second, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, firstCall, singular, handle.empty() ? 0 : 1, commandData.second.params.size());
  str += " )";
}

void VulkanHppGenerator::appendCommand(std::string & str, std::string const& indentation, std::string const& name, std::pair<std::string, CommandData> const& commandData, bool definition) const
{
  bool twoStep = isTwoStepAlgorithm(commandData.second.params);
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndices(commandData.second.params);

  size_t returnParamIndex = determineReturnParamIndex(commandData.second, vectorParamIndices, twoStep);
  bool isStructureChain = (returnParamIndex != INVALID_INDEX) && determineStructureChaining(commandData.second.params[returnParamIndex].type.type, m_extendedStructs, m_structureAliases);
  std::string enhancedReturnType = determineEnhancedReturnType(commandData.second, returnParamIndex, vectorParamIndices, twoStep, false);   // get the enhanced return type without structureChain

  size_t templateParamIndex = determineTemplateParamIndex(commandData.second.params, vectorParamIndices);

  // first create the standard version of the function
  std::string standard;
  appendFunction(standard, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, false, false, false, false, false);

  // then the enhanced version, composed by up to eight parts
  std::string enhanced;
  appendFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, true, false, false, false, false);

  if (enhancedReturnType.find("Allocator") != std::string::npos)
  {
    appendFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, true, false, false, false, true);
  }

  if (isStructureChain)
  {
    std::string enhancedReturnTypeWithStructureChain = determineEnhancedReturnType(commandData.second, returnParamIndex, vectorParamIndices, twoStep, true);
    appendFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnTypeWithStructureChain, definition, true, false, false, true, false);

    if (enhancedReturnTypeWithStructureChain.find("Allocator") != std::string::npos)
    {
      appendFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnTypeWithStructureChain, definition, true, false, false, true, true);
    }
  }

  // then a singular version, if a sized vector would be returned
  std::map<size_t, size_t>::const_iterator returnVector = vectorParamIndices.find(returnParamIndex);
  bool singular = (returnVector != vectorParamIndices.end()) &&
    (returnVector->second != INVALID_INDEX) &&
    (commandData.second.params[returnVector->first].type.type != "void") &&
    (commandData.second.params[returnVector->second].type.postfix.empty() || (commandData.second.params[returnVector->second].type.postfix.back() != '*'));
  if (singular)
  {
    appendFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, true, true, false, false, false);
  }

  // special handling for createDevice and createInstance !
  bool specialWriteUnique = (commandData.first == "vkCreateDevice") || (commandData.first == "vkCreateInstance");

  // and then the same for the Unique* versions (a deleteCommand is available for the commandData's class, and the function starts with 'allocate' or 'create')
  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  auto handleIt = m_handles.find(m_commandToHandle.find(commandData.first)->second);
  assert(handleIt != m_handles.end());
  if ((!handleIt->second.deleteCommand.empty() || specialWriteUnique)
    && ((commandData.first.substr(2, 8) == "Allocate") || (commandData.first.substr(2, 6) == "Create")
      || ((commandData.first.substr(2, 8) == "Register") && (returnParamIndex + 1 == commandData.second.params.size()))))
  {
    enhanced += "#ifndef VULKAN_HPP_NO_SMART_HANDLE\n";
    appendFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, true, false, true, false, false);

    if (enhancedReturnType.find("Allocator") != std::string::npos)
    {
      appendFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, true, false, true, false, true);
    }

    if (singular)
    {
      appendFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, true, true, true, false, false);
    }
    enhanced += "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/\n";
  }

  // and append one or both of them
  if (standard == enhanced)
  {
    // standard and enhanced string are equal -> just use one of them and we're done
    str += standard;
  }
  else
  {
    // standard and enhanced string differ -> use both, wrapping the enhanced by !VULKAN_HPP_DISABLE_ENHANCED_MODE
    // determine the argument list of that standard, and compare it with that of the enhanced
    // if they are equal -> need to have just one; if they differ -> need to have both
    size_t standardStart = standard.find('(');
    size_t standardCount = standard.find(')', standardStart) - standardStart;
    size_t enhancedStart = enhanced.find('(');
    bool unchangedInterface = (standard.substr(standardStart, standardCount) == enhanced.substr(enhancedStart, standardCount));
    if (unchangedInterface)
    {
      str += "#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE\n";
    }
    str += standard
      + (unchangedInterface ? "#else" : "#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE") + "\n"
      + enhanced
      + "#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/\n";
  }
}

void VulkanHppGenerator::appendDispatchLoaderDynamic(std::string & str)
{
  str += R"(
#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
  class DynamicLoader
  {
  public:
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    DynamicLoader() VULKAN_HPP_NOEXCEPT : m_success( false )
#else
    DynamicLoader() : m_success( false )
#endif
    {
#if defined(__linux__)
      m_library = dlopen( "libvulkan.so", RTLD_NOW | RTLD_LOCAL );
#elif defined(__APPLE__)
      m_library = dlopen( "libvulkan.dylib", RTLD_NOW | RTLD_LOCAL );
#elif defined(_WIN32)
      m_library = LoadLibrary( TEXT( "vulkan-1.dll" ) );
#else
      assert( false && "unsupported platform" );
#endif

      m_success = m_library != 0;
#ifndef VULKAN_HPP_NO_EXCEPTIONS
      if ( !m_success )
      {
        // NOTE there should be an InitializationFailedError, but msvc insists on the symbol does not exist within the scope of this function.
        throw std::runtime_error( "Failed to load vulkan library!" );
      }
#endif
    }

    ~DynamicLoader() VULKAN_HPP_NOEXCEPT
    {
      if ( m_library )
      {
#if defined(__linux__) || defined(__APPLE__)
        dlclose( m_library );
#elif defined(_WIN32)
        FreeLibrary( m_library );
#endif
      }
    }

    template <typename T>
    T getProcAddress( const char* function ) const VULKAN_HPP_NOEXCEPT
    {
#if defined(__linux__) || defined(__APPLE__)
      return (T)dlsym( m_library, function );
#elif defined(_WIN32)
      return (T)GetProcAddress( m_library, function );
#endif
    }

    bool success() const VULKAN_HPP_NOEXCEPT { return m_success; }

  private:
    bool m_success;
#if defined(__linux__) || defined(__APPLE__)
    void *m_library;
#elif defined(_WIN32)
    HMODULE m_library;
#else
#error unsupported platform
#endif
  };
#endif

)";
  str += R"(
  class DispatchLoaderDynamic
  {
  public:
)";

  for (auto const& handle : m_handles)
  {
    for (auto const& command : handle.second.commands)
    {
      std::string enter, leave;
      appendPlatformEnter(enter, command.second.platform);
      appendPlatformLeave(leave, command.second.platform);

      str += enter + "    PFN_" + command.first + " " + command.first + " = 0;\n" + leave;
    }
  }

  std::string emptyFunctions;
  std::string strDeviceFunctions;
  std::string strDeviceFunctionsInstance;
  std::string strInstanceFunctions;
  for (auto const& handle : m_handles)
  {
    for (auto const& command : handle.second.commands)
    {
      if ((command.first != "vkGetInstanceProcAddr"))
      {
        std::string enter, leave;
        appendPlatformEnter(enter, command.second.platform);
        appendPlatformLeave(leave, command.second.platform);

        if (handle.first.empty())
        {
          emptyFunctions += enter;
          emptyFunctions += "      " + command.first + " = PFN_" + command.first + "( vkGetInstanceProcAddr( NULL, \"" + command.first + "\" ) );\n";
          emptyFunctions += leave;
        }
        else if (!command.second.params.empty()
          && m_handles.find(command.second.params[0].type.type) != m_handles.end()
          && command.second.params[0].type.type != "VkInstance"
          && command.second.params[0].type.type != "VkPhysicalDevice")
        {
          strDeviceFunctions += enter;
          strDeviceFunctions += "      " + command.first + " = PFN_" + command.first + "( vkGetDeviceProcAddr( device, \"" + command.first + "\" ) );\n";
          strDeviceFunctions += leave;

          strDeviceFunctionsInstance += enter;
          strDeviceFunctionsInstance += "      " + command.first + " = PFN_" + command.first + "( vkGetInstanceProcAddr( instance, \"" + command.first + "\" ) );\n";
          strDeviceFunctionsInstance += leave;
        }
        else
        {
          strInstanceFunctions += enter;
          strInstanceFunctions += "      " + command.first + " = PFN_" + command.first + "( vkGetInstanceProcAddr( instance, \"" + command.first + "\" ) );\n";
          strInstanceFunctions += leave;
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
    DispatchLoaderDynamic(VULKAN_HPP_NAMESPACE::Instance const& instance, VULKAN_HPP_NAMESPACE::Device const& device) VULKAN_HPP_NOEXCEPT
    {
      init(instance, device);
    }

    // This interface is designed to be used for per-device function pointers in combination with a linked vulkan library.
    void init(VULKAN_HPP_NAMESPACE::Instance const& instance, VULKAN_HPP_NAMESPACE::Device const& device) VULKAN_HPP_NOEXCEPT
    {
      static vk::DynamicLoader dl;
      PFN_vkGetInstanceProcAddr getInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
      PFN_vkGetDeviceProcAddr getDeviceProcAddr = dl.getProcAddress<PFN_vkGetDeviceProcAddr>("vkGetDeviceProcAddr");
      init(static_cast<VkInstance>(instance), getInstanceProcAddr, static_cast<VkDevice>(device), device ? getDeviceProcAddr : nullptr);
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

void VulkanHppGenerator::appendDispatchLoaderStatic(std::string & str)
{
  str += R"(
#if !defined(VK_NO_PROTOTYPES)
  class DispatchLoaderStatic
  {
  public:)";

  for (auto const& handle : m_handles)
  {
    for (auto const& command : handle.second.commands)
    {
      std::string parameterList, parameters;
      bool firstParam = true;
      for (auto param : command.second.params)
      {
        if (!firstParam)
        {
          parameterList += ", ";
          parameters += ", ";
        }
        parameterList += param.type.prefix + (param.type.prefix.empty() ? "" : " ") + param.type.type + param.type.postfix + " " + param.name + constructCArraySizes(param.arraySizes);
        parameters += param.name;
        firstParam = false;
      }
      std::string commandName = stripPrefix(command.first, "vk");

      str += "\n";
      appendPlatformEnter(str, command.second.platform);
      str += "    " + command.second.returnType + " vk" + commandName + "( " + parameterList + " ) const VULKAN_HPP_NOEXCEPT\n"
        "    {\n"
        "      return ::vk" + commandName + "( " + parameters + " );\n"
        "    }\n";
      appendPlatformLeave(str, command.second.platform);
    }
  }
  str += "  };\n#endif\n";
}

void VulkanHppGenerator::appendDispatchLoaderDefault(std::string & str)
{
  str += "\n"
    R"(  class DispatchLoaderDynamic;
#if !defined(VULKAN_HPP_DISPATCH_LOADER_DYNAMIC)
# if defined(VK_NO_PROTOTYPES)
#  define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
# else
#  define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 0
# endif
#endif

#if !defined(VULKAN_HPP_DEFAULT_DISPATCHER)
# if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
#  define VULKAN_HPP_DEFAULT_DISPATCHER ::VULKAN_HPP_NAMESPACE::defaultDispatchLoaderDynamic
#  define VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE namespace VULKAN_HPP_NAMESPACE { DispatchLoaderDynamic defaultDispatchLoaderDynamic; }
  extern DispatchLoaderDynamic defaultDispatchLoaderDynamic;
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

void VulkanHppGenerator::appendEnum(std::string & str, std::pair<std::string, EnumData> const& enumData) const
{
  str += "  enum class " + stripPrefix(enumData.first, "Vk") + "\n"
    "  {";

  bool first = true;
  for (auto const& value : enumData.second.values)
  {
    if (!first)
    {
      str += ",";
    }
    str += "\n    " + value.vkValue + " = " + value.vulkanValue;
    first = false;
  }
  for (auto const& value : enumData.second.aliases)
  {
    // make sure to only list alias values that differ from all non-alias values
    if (std::find_if(enumData.second.values.begin(), enumData.second.values.end(), [&value](EnumValueData const& evd) { return value.second == evd.vkValue; }) == enumData.second.values.end())
    {
      if (!first)
      {
        str += ",";
      }
      str += "\n    " + value.second + " = " + value.first;
      first = false;
    }
  }
  if (!first)
  {
    str += "\n  ";
  }

  str += "};\n";

  if (!enumData.second.alias.empty())
  {
    str += "  using " + stripPrefix(enumData.second.alias, "Vk") + " = " + stripPrefix(enumData.first, "Vk") + ";\n";
  }
}

void VulkanHppGenerator::appendEnums(std::string & str) const
{
  for (auto const& e : m_enums)
  {
    str += "\n";
    appendPlatformEnter(str, e.second.platform);
    appendEnum(str, e);
    appendEnumToString(str, e);
    appendPlatformLeave(str, e.second.platform);
  }
}

void VulkanHppGenerator::appendEnumToString(std::string & str, std::pair<std::string, EnumData> const& enumData) const
{
  std::string enumName = stripPrefix(enumData.first, "Vk");

  str += "\n"
    "  VULKAN_HPP_INLINE std::string to_string( " + enumName + (enumData.second.values.empty() ? "" : " value") + " )\n"
    "  {";

  if (enumData.second.values.empty())
  {
    str += "\n"
      "    return \"(void)\";\n";
  }
  else
  {
    str += "\n"
      "    switch ( value )\n"
      "    {\n";
    for (auto const& value : enumData.second.values)
    {
      str += "      case " + enumName + "::" + value.vkValue + " : return \"" + value.vkValue.substr(1) + "\";\n";
    }
    str += "      default: return \"invalid\";\n"
      "    }\n";
  }

  str += "  }\n";
}

void VulkanHppGenerator::appendForwardDeclarations(std::string & str) const
{
  str += "\n";
  for (auto const& structure : m_structures)
  {
    appendPlatformEnter(str, structure.second.platform);
    str += std::string("  ") + (structure.second.isUnion ? "union" : "struct") + " " + stripPrefix(structure.first, "Vk") + ";\n";
    for (std::string const& alias : structure.second.aliases)
    {
      str += "  using " + stripPrefix(alias, "Vk") + " = " + stripPrefix(structure.first, "Vk") + ";\n";
    }
    appendPlatformLeave(str, structure.second.platform);
  }
}

bool needsMultiVectorSizeCheck(size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices)
{
  for (std::map<size_t, size_t>::const_iterator it0 = vectorParamIndices.begin(); it0 != vectorParamIndices.end(); ++it0)
  {
    if (it0->first != returnParamIndex)
    {
      for (std::map<size_t, size_t>::const_iterator it1 = std::next(it0); it1 != vectorParamIndices.end(); ++it1)
      {
        if ((it1->first != returnParamIndex) && (it0->second == it1->second))
        {
          return true;
        }
      }
    }
  }
  return false;
}

void VulkanHppGenerator::appendFunction(std::string & str, std::string const& indentation, std::string const& name, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, std::string const& enhancedReturnType, bool definition, bool enhanced, bool singular, bool unique, bool isStructureChain, bool withAllocator) const
{
  appendFunctionHeaderTemplate(str, indentation, returnParamIndex, templateParamIndex, enhancedReturnType, enhanced, singular, unique, !definition, isStructureChain);

  str += indentation + (definition ? "VULKAN_HPP_INLINE " : "");

  appendFunctionHeaderReturnType(str, commandData.second, returnParamIndex, vectorParamIndices, enhancedReturnType, enhanced, twoStep, singular, unique, isStructureChain);

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;
  if (definition && !handle.empty())
  {
    str += stripPrefix(handle, "Vk") + "::";
  }

  // append the function header name
  str += (singular ? stripPluralS(name) : name);
  if (unique)
  {
    str += "Unique";
  }

  appendFunctionHeaderArguments(str, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, enhanced, singular, !definition, withAllocator);

  // Any function that originally does not return VkResult can be marked noexcept,
  // if it is enhanced it must not include anything with an Allocator or needs size checks on multiple vectors
  bool hasAllocator = enhancedReturnType.find("Allocator") != std::string::npos;
  if (!enhanced || (commandData.second.returnType != "VkResult" && !(enhanced && (hasAllocator || needsMultiVectorSizeCheck(returnParamIndex, vectorParamIndices)))))
  {
    str += " VULKAN_HPP_NOEXCEPT";
  }

  str += std::string(definition ? "" : ";") + "\n";

  if (definition)
  {
    // append the function body
    str += indentation + "{\n";
    if (enhanced)
    {
      appendFunctionBodyEnhanced(str, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, singular, unique, isStructureChain, withAllocator);
    }
    else
    {
      appendFunctionBodyStandard(str, indentation, commandData);
    }
    str += indentation + "}\n";
  }
}

void VulkanHppGenerator::appendFunctionBodyEnhanced(std::string & str, std::string const& indentation, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, std::string const& enhancedReturnType, bool singular, bool unique, bool isStructureChain, bool withAllocator) const
{
  if (unique && !singular && (vectorParamIndices.find(returnParamIndex) != vectorParamIndices.end()))    // returns a vector of UniqueStuff
  {
    appendFunctionBodyEnhancedVectorOfUniqueHandles(str, indentation, commandName, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, singular, withAllocator);
  }
  else if (isStructureChain && (vectorParamIndices.find(returnParamIndex) != vectorParamIndices.end()))
  {
    appendFunctionBodyEnhancedVectorOfStructureChain(str, indentation, commandData, returnParamIndex, vectorParamIndices, withAllocator);
  }
  else
  {
    if (1 < vectorParamIndices.size())
    {
      appendFunctionBodyEnhancedMultiVectorSizeCheck(str, indentation, commandName, commandData, returnParamIndex, vectorParamIndices);
    }

    std::string returnName;
    if (returnParamIndex != INVALID_INDEX)
    {
      returnName = appendFunctionBodyEnhancedLocalReturnVariable(str, indentation, commandData.second, returnParamIndex, vectorParamIndices, twoStep, enhancedReturnType, singular, isStructureChain, withAllocator);
    }

    if (twoStep)
    {
      appendFunctionBodyEnhancedTwoStep(str, indentation, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, singular, returnName);
    }
    else
    {
      appendFunctionBodyEnhancedSingleStep(str, indentation, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, singular);
    }

    if ((commandData.second.returnType == "VkResult") || !commandData.second.successCodes.empty())
    {
      appendFunctionBodyEnhancedReturnResultValue(str, indentation, returnName, commandName, commandData, returnParamIndex, twoStep, singular, unique);
    }
    else if ((returnParamIndex != INVALID_INDEX) && (stripPrefix(commandData.second.returnType, "Vk") != enhancedReturnType))
    {
      // for the other returning cases, when the return type is somhow enhanced, just return the local returnVariable
      str += indentation + "  return " + returnName + ";\n";
    }
  }
}

std::string VulkanHppGenerator::appendFunctionBodyEnhancedLocalReturnVariable(std::string & str, std::string const& indentation, CommandData const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, std::string const& enhancedReturnType, bool singular, bool isStructureChain, bool withAllocator) const
{
  std::string pureReturnType = stripPrefix(commandData.params[returnParamIndex].type.type, "Vk");
  std::string returnName = startLowerCase(stripPrefix(commandData.params[returnParamIndex].name, "p"));

  // there is a returned parameter -> we need a local variable to hold that value
  if (stripPrefix(commandData.returnType, "Vk") != enhancedReturnType)
  {
    // the returned parameter is somehow enhanced by us
    str += indentation + "  ";
    if (singular)
    {
      returnName = appendFunctionBodyEnhancedLocalReturnVariableSingular(str, indentation, returnName, pureReturnType, isStructureChain);
    }
    else
    {
      // in non-singular case, use the enhanced type for the return variable (like vector<...>)
      if (isStructureChain && vectorParamIndices.empty())
      {
        // For StructureChains use the template parameters
        str += "StructureChain<X, Y, Z...> structureChain;\n"
          + indentation + "  " + enhancedReturnType + "& " + returnName + " = structureChain.template get<" + enhancedReturnType + ">()";
        returnName = "structureChain";
      }
      else
      {
        str += enhancedReturnType + " " + returnName;
      }

      std::map<size_t, size_t>::const_iterator vpiIt = vectorParamIndices.find(returnParamIndex);
      if (vpiIt != vectorParamIndices.end() && !twoStep)
      {
        appendFunctionBodyEnhancedLocalReturnVariableVectorSize(str, commandData.params, *vpiIt, returnParamIndex, vectorParamIndices, withAllocator);
      }
      else if (withAllocator)
      {
        str += "( vectorAllocator )";
      }
    }
    str += ";\n";
  }
  else
  {
    // the return parameter is not enhanced -> the type is supposed to be a Result and there are more than one success codes!
    assert((commandData.returnType == "VkResult") && (1 < commandData.successCodes.size()));
    str += indentation + "  " + pureReturnType + " " + returnName + ";\n";
  }

  return returnName;
}

void VulkanHppGenerator::appendFunctionBodyEnhancedLocalReturnVariableVectorSize(std::string & str, std::vector<ParamData> const& params, std::pair<size_t, size_t> const& vectorParamIndex, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool withAllocator) const
{
  // if the return parameter is a vector parameter, and not part of a two-step algorithm, initialize its size
  std::string size;
  if (vectorParamIndex.second == INVALID_INDEX)
  {
    assert(!params[returnParamIndex].len.empty());
    // the size of the vector is not given by an other parameter, but by some member of a parameter, described as 'parameter::member'
    // -> replace the '::' by '.' and filter out the leading 'p' to access that value
    size = startLowerCase(stripPrefix(params[returnParamIndex].len, "p"));
    size_t pos = size.find("::");
    assert(pos != std::string::npos);
    size.replace(pos, 2, ".");
  }
  else
  {
    // the size of the vector is given by an other parameter
    // first check, if that size has become the size of some other vector parameter
    // -> look for it and get it's actual size
    for (auto const& vpi : vectorParamIndices)
    {
      if ((vpi.first != vectorParamIndex.first) && (vpi.second == vectorParamIndex.second))
      {
        size = startLowerCase(stripPrefix(params[vpi.first].name, "p")) + ".size()";
        break;
      }
    }
    if (size.empty())
    {
      // otherwise, just use that parameter
      size = params[vectorParamIndex.second].name;
    }
  }
  assert(!size.empty());
  str += "( " + size + (withAllocator ? ", vectorAllocator" : "") + " )";
}

void VulkanHppGenerator::appendFunctionBodyEnhancedMultiVectorSizeCheck(std::string & str, std::string const& indentation, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices) const
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

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;

  // add some error checks if multiple vectors need to have the same size
  for (std::map<size_t, size_t>::const_iterator it0 = vectorParamIndices.begin(); it0 != vectorParamIndices.end(); ++it0)
  {
    if (it0->first != returnParamIndex)
    {
      for (std::map<size_t, size_t>::const_iterator it1 = std::next(it0); it1 != vectorParamIndices.end(); ++it1)
      {
        if ((it1->first != returnParamIndex) && (it0->second == it1->second))
        {
          str += replaceWithMap(sizeCheckTemplate, std::map<std::string, std::string>({
            { "firstVectorName", startLowerCase(stripPrefix(commandData.second.params[it0->first].name, "p")) },
            { "secondVectorName", startLowerCase(stripPrefix(commandData.second.params[it1->first].name, "p")) },
            { "className", handle },
            { "commandName", commandName },
            { "i", indentation }
            }));
        }
      }
    }
  }
}

void VulkanHppGenerator::appendFunctionBodyEnhancedReturnResultValue(std::string & str, std::string const& indentation, std::string const& returnName, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, bool twoStep, bool singular, bool unique) const
{
  std::string type = (returnParamIndex != INVALID_INDEX) ? commandData.second.params[returnParamIndex].type.type : "";
  std::string returnVectorName = (returnParamIndex != INVALID_INDEX) ? stripPostfix(stripPrefix(commandData.second.params[returnParamIndex].name, "p"), "s") : "";

  if (commandData.second.returnType == "void") {
    std::cerr << "warning: skipping appendFunctionBodyEnhancedReturnResultValue for function " << commandName << " because the returnType is void";
    return;
  }

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;

  if (unique)
  {
    // the unique version needs a Deleter object for destruction of the newly created stuff
    // get the DeleterData corresponding to the returned type
    // special handling for "createDevice", as Device is created from PhysicalDevice, but destroyed on its own
    bool noParent = handle.empty() || (commandData.first == "vkCreateDevice");
    str += "\n"
      + indentation + ((commandData.first == "vkAllocateMemory") ? "  ObjectFree<" : "  ObjectDestroy<") + (noParent ? "NoParent" : stripPrefix(handle, "Vk")) + ",Dispatch> deleter( " + (noParent ? "" : "*this, ") + "allocator, d );\n"
      + indentation + "  return createResultValue<" + stripPrefix(type, "Vk") + ",Dispatch>( result, ";
  }
  else
  {
    str += indentation + "  return createResultValue( result, ";
  }

  // if the return type is "Result" or there is at least one success code, create the Result/Value construct to return
  if (returnParamIndex != INVALID_INDEX)
  {
    // if there's a return parameter, list it in the Result/Value constructor
    str += returnName + ", ";
  }

  // now the function name (with full namespace) as a string
  str += "VULKAN_HPP_NAMESPACE_STRING\"::" + (handle.empty() ? "" : stripPrefix(handle, "Vk") + "::") + (singular ? stripPluralS(commandName) : commandName) + (unique ? "Unique" : "") + "\"";

  if (!twoStep && (1 < commandData.second.successCodes.size()))
  {
    // and for the single-step algorithms with more than one success code list them all
    str += ", { Result::" + commandData.second.successCodes[0];
    for (size_t i = 1; i < commandData.second.successCodes.size(); i++)
    {
      str += ", Result::" + commandData.second.successCodes[i];
    }
    str += " }";
  }

  if (unique)
  {
    str += ", deleter";
  }

  str += " );\n";
}

void VulkanHppGenerator::appendFunctionBodyEnhancedSingleStep(std::string & str, std::string const& indentation, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool singular) const
{
  str += indentation + "  ";
  if (commandData.second.returnType == "VkResult")
  {
    str += "Result result = static_cast<Result>( ";
  }
  else if (commandData.second.returnType != "void")
  {
    str += "return ";
  }
  appendCall(str, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, false, true, singular);
  if (commandData.second.returnType == "VkResult")
  {
    str += " )";
  }
  str += ";\n";
}

void VulkanHppGenerator::appendFunctionBodyEnhancedTwoStep(std::string & str, std::string const& indentation, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool singular, std::string const& returnName) const
{
  assert(!singular);
  assert((commandData.second.returnType == "VkResult") || (commandData.second.returnType == "void"));
  assert(returnParamIndex != INVALID_INDEX);

  // local count variable to hold the size of the vector to fill
  std::map<size_t, size_t>::const_iterator returnit = vectorParamIndices.find(returnParamIndex);
  assert(returnit != vectorParamIndices.end() && (returnit->second != INVALID_INDEX));

  // take the pure type of the size parameter; strip the leading 'p' from its name for its local name
  std::string sizeName = startLowerCase(stripPrefix(commandData.second.params[returnit->second].name, "p"));
  str += indentation + "  " + stripPrefix(commandData.second.params[returnit->second].type.type, "Vk") + " " + sizeName + ";\n";

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
  std::string const& selectedTemplate = (commandData.second.returnType == "VkResult") ? ((1 < commandData.second.successCodes.size()) ? multiSuccessTemplate : singleSuccessTemplate) : voidMultiCallTemplate;

  std::string call1, call2;
  appendCall(call1, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, true, true, false);
  appendCall(call2, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, true, false, false);

  str += replaceWithMap(selectedTemplate,
    {
      { "sizeName", sizeName },
      { "returnName", returnName },
      { "call1", call1 },
      { "call2", call2 },
      { "i", indentation }
    });
}

void VulkanHppGenerator::appendFunctionBodyEnhancedVectorOfStructureChain(std::string & str, std::string const& indentation, std::pair<std::string,CommandData> const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool withAllocator) const
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
  std::map<size_t, size_t>::const_iterator returnit = vectorParamIndices.find(returnParamIndex);
  assert(returnit != vectorParamIndices.end() && (returnit->second != INVALID_INDEX));

  assert(m_commandToHandle.find(commandData.first)->second == commandData.second.params[0].type.type); // make sure, the first argument is the handle
  assert(commandData.second.params.size() == 3);    // make sure, there are three args: the handle, the pointer to size, and the data pointer

  str += replaceWithMap(stringTemplate,
    {
      { "commandName", commandData.first},
      { "handleName",  startLowerCase(stripPrefix(commandData.second.params[0].type.type, "Vk")) },
      { "i", indentation },
      { "returnName", startLowerCase(stripPrefix(commandData.second.params[returnParamIndex].name, "p")) },
      { "returnType", stripPrefix(commandData.second.params[returnParamIndex].type.type, "Vk")},
      { "sizeName", startLowerCase(stripPrefix(commandData.second.params[returnit->second].name, "p"))},
      { "vectorAllocator", withAllocator ? "( vectorAllocator )" : "" },
      { "VkReturnType", commandData.second.params[returnParamIndex].type.type}
    });
}

void VulkanHppGenerator::appendFunctionBodyEnhancedVectorOfUniqueHandles(std::string & str, std::string const& indentation, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool singular, bool withAllocator) const
{
  std::string const stringTemplate =
    R"(${i}  static_assert( sizeof( ${type} ) <= sizeof( UniqueHandle<${type}, Dispatch> ), "${type} is greater than UniqueHandle<${type}, Dispatch>!" );
${i}  std::vector<UniqueHandle<${type}, Dispatch>, Allocator> ${typeVariable}s${allocator};
${i}  ${typeVariable}s.reserve( ${vectorSize} );
${i}  ${type}* buffer = reinterpret_cast<${type}*>( reinterpret_cast<char*>( ${typeVariable}s.data() ) + ${vectorSize} * ( sizeof( UniqueHandle<${type}, Dispatch> ) - sizeof( ${type} ) ) );
${i}  Result result = static_cast<Result>(d.vk${command}( m_device, ${arguments}, reinterpret_cast<Vk${type}*>( buffer ) ) );
${i}  if ( ${successChecks} )
${i}  {
${i}    ${Deleter}<${DeleterTemplate},Dispatch> deleter( *this, ${deleterArg}, d );
${i}    for ( size_t i=0 ; i<${vectorSize} ; i++ )
${i}    {
${i}      ${typeVariable}s.push_back( UniqueHandle<${type}, Dispatch>( buffer[i], deleter ) );
${i}    }
${i}  }

${i}  return createResultValue( result, ${typeVariable}s, VULKAN_HPP_NAMESPACE_STRING "::${class}::${commandName}Unique"${successCodes} );
)";

  std::string type = (returnParamIndex != INVALID_INDEX) ? commandData.second.params[returnParamIndex].type.type : "";
  std::string typeVariable = startLowerCase(stripPrefix(type, "Vk"));
  std::string arguments;
  appendArguments(arguments, commandData.second, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, true, singular, 1, commandData.second.params.size() - 1);

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;

  auto handleIt = m_handles.find(type);
  assert(handleIt != m_handles.end());

  assert(!commandData.second.successCodes.empty());
  std::string successChecks = "result == VULKAN_HPP_NAMESPACE::Result::" + commandData.second.successCodes[0];
  std::string successCodes;
  if (1 < commandData.second.successCodes.size())
  {
    successChecks = "( " + successChecks + " )";
    successCodes = ", { VULKAN_HPP_NAMESPACE::Result::" + commandData.second.successCodes[0];
    for (size_t i = 1; i < commandData.second.successCodes.size(); i++)
    {
      successChecks += " || ( result == VULKAN_HPP_NAMESPACE::Result::" + commandData.second.successCodes[i] + " )";
      successCodes += ", VULKAN_HPP_NAMESPACE::Result::" + commandData.second.successCodes[i];
    }
    successCodes += " }";
  }

  bool isCreateFunction = (commandData.first.substr(2, 6) == "Create");
  str += replaceWithMap(stringTemplate, std::map<std::string, std::string>
  {
    { "allocator", withAllocator ? "( vectorAllocator )" : "" },
    { "arguments", arguments },
    { "class", stripPrefix(handle, "Vk") },
    { "command", stripPrefix(commandData.first, "vk") },
    { "commandName", commandName },
    { "Deleter", handleIt->second.deletePool.empty() ? "ObjectDestroy" : "PoolFree" },
    { "deleterArg", handleIt->second.deletePool.empty() ? "allocator" : "allocateInfo." + startLowerCase(stripPrefix(handleIt->second.deletePool, "Vk")) },
    { "DeleterTemplate", stripPrefix(handle, "Vk") + (handleIt->second.deletePool.empty() ? "" : "," + stripPrefix(handleIt->second.deletePool, "Vk")) },
    { "i", indentation },
    { "successChecks", successChecks },
    { "successCodes", successCodes },
    { "type", stripPrefix(type, "Vk") },
    { "typeVariable", typeVariable },
    { "vectorSize", isCreateFunction ? "createInfos.size()" : "allocateInfo." + typeVariable + "Count" }
  });
}

void VulkanHppGenerator::appendFunctionBodyStandard(std::string & str, std::string const& indentation, std::pair<std::string, CommandData> const& commandData) const
{
  std::pair<bool, std::string> returnData = generateFunctionBodyStandardReturn(commandData.second.returnType);

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;
  assert(handle.empty() || (handle == commandData.second.params[0].type.type));

  str += indentation + "  " + returnData.second + "d." + commandData.first + "( " + (handle.empty() ? "" : ("m_" + startLowerCase(stripPrefix(handle, "Vk"))));
  for (size_t i = handle.empty() ? 0 : 1; i < commandData.second.params.size(); i++)
  {
    if (0 < i)
    {
      str += ", ";
    }
    appendFunctionBodyStandardArgument(str, commandData.second.params[i].type, commandData.second.params[i].name);
  }
  str += std::string(" )") + (returnData.first ? " )" : "") + ";\n";
}

void VulkanHppGenerator::appendFunctionBodyStandardArgument(std::string & str, TypeData const& typeData, std::string const& name) const
{
  if (beginsWith(typeData.type, "Vk"))
  {
    // the parameter is a vulkan type
    if (!typeData.postfix.empty())
    {
      assert(typeData.postfix.back() == '*');
      // it's a pointer -> need to reinterpret_cast it
      appendReinterpretCast(str, typeData.prefix.find("const") == 0, typeData.type, typeData.postfix.find("* const") != std::string::npos);
    }
    else
    {
      // it's a value -> need to static_cast ist
      str += "static_cast<" + typeData.type + ">";
    }
    str += "( " + name + " )";
  }
  else
  {
    // it's a non-vulkan type -> just use it
    str += name;
  }
}

bool VulkanHppGenerator::appendFunctionHeaderArgumentEnhanced(std::string & str, ParamData const& param, size_t paramIndex, std::map<size_t, size_t> const& vectorParamIndices, bool skip, bool argEncountered, bool isTemplateParam, bool isLastArgument, bool singular, bool withDefaults, bool withAllocator) const
{
  if (!skip)
  {
    if (argEncountered)
    {
      str += ", ";
    }
    std::string strippedParameterName = startLowerCase(stripPrefix(param.name, "p"));

    std::map<size_t, size_t>::const_iterator it = vectorParamIndices.find(paramIndex);
    if (it == vectorParamIndices.end())
    {
      // the argument ist not a vector
      if (param.type.postfix.empty())
      {
        // and its not a pointer -> just use its type and name here
        appendFunctionHeaderArgumentEnhancedSimple(str, param, isLastArgument, withDefaults, withAllocator);
      }
      else
      {
        // the argument is not a vector, but a pointer
        assert(param.type.postfix.back() == '*');
        appendFunctionHeaderArgumentEnhancedPointer(str, param, strippedParameterName, withDefaults, withAllocator);
      }
    }
    else
    {
      // the argument is a vector
      appendFunctionHeaderArgumentEnhancedVector(str, param, strippedParameterName, it->second != INVALID_INDEX, isTemplateParam, singular, withDefaults, withAllocator);
    }
    argEncountered = true;
  }
  return argEncountered;
}

void VulkanHppGenerator::appendFunctionHeaderArgumentEnhancedPointer(std::string & str, ParamData const& param, std::string const& strippedParameterName, bool withDefaults, bool withAllocator) const
{
  assert(param.type.postfix.back() == '*');
  if (param.optional)
  {
    // for an optional argument, trim the leading 'p' from the name
    str += "Optional<" + param.type.prefix + (param.type.prefix.empty() ? "" : " ") + stripPrefix(param.type.type, "Vk") + "> " + strippedParameterName;
    if (withDefaults && !withAllocator)
    {
      str += " = nullptr";
    }
  }
  else if (param.type.type == "void")
  {
    // for void-pointer, just use type and name
    str += param.type.compose() + " " + param.name;
  }
  else if (param.type.type != "char")
  {
    // for non-char-pointer, change to reference
    assert(param.type.postfix == "*");
    str += param.type.prefix + (param.type.prefix.empty() ? "" : " ") + stripPrefix(param.type.type, "Vk") + " & " + strippedParameterName;
  }
  else
  {
    // for char-pointer, change to const reference to std::string
    str += "const std::string & " + strippedParameterName;
  }
}

void VulkanHppGenerator::appendFunctionHeaderArgumentEnhancedSimple(std::string & str, ParamData const& param, bool lastArgument, bool withDefaults, bool withAllocator) const
{
  str += param.type.compose() + " " + param.name + constructCArraySizes(param.arraySizes);

  if (withDefaults && lastArgument && !withAllocator)
  {
    // check if the very last argument is a flag without any bits -> provide some empty default for it
    std::map<std::string, BitmaskData>::const_iterator bitmasksIt = m_bitmasks.find(param.type.type);
    if (bitmasksIt != m_bitmasks.end())
    {
      // get the enum corresponding to this flag, to check if it's empty
      std::string strippedBitmaskName = stripPrefix(bitmasksIt->first, "Vk");
      std::map<std::string, EnumData>::const_iterator enumIt = m_enums.find(bitmasksIt->second.requirement);
      assert((enumIt == m_enums.end()) || (enumIt->second.isBitmask));
      if ((enumIt == m_enums.end()) || (enumIt->second.values.empty()))
      {
        // there are no bits in this flag -> provide the default
        str += " = " + stripPrefix(param.type.type, "Vk") + "()";
      }
    }
  }
}

void VulkanHppGenerator::appendFunctionHeaderArgumentEnhancedVector(std::string & str, ParamData const& param, std::string const& strippedParameterName, bool hasSizeParam, bool isTemplateParam, bool singular, bool withDefaults, bool withAllocator) const
{
  assert(param.type.postfix.back() == '*');

  // it's optional, if it's marked as optional and there's no size specified
  bool optional = param.optional && !hasSizeParam;
  if (param.type.type.find("char") != std::string::npos)
  {
    // it's a char-vector -> use a std::string (either optional or a const-reference
    if (optional)
    {
      str += "Optional<const std::string> " + strippedParameterName;
      if (withDefaults && !withAllocator)
      {
        str += " = nullptr";
      }
    }
    else
    {
      str += "const std::string & " + strippedParameterName;
    }
  }
  else
  {
    // it's a non-char vector (they are never optional)
    assert(!optional);
    if (singular)
    {
      // in singular case, change from pointer to reference
      str += param.type.prefix + (param.type.prefix.empty() ? "" : " ") + stripPrefix(param.type.type, "Vk") + " & " + stripPluralS(strippedParameterName);
    }
    else
    {
      // otherwise, use our ArrayProxy
      bool isConst = (param.type.prefix.find("const") != std::string::npos);
      str += "ArrayProxy<" + (isTemplateParam ? (isConst ? "const T" : "T") : stripPostfix(param.type.compose(), "*")) + "> " + strippedParameterName;
    }
  }
}

void VulkanHppGenerator::appendFunctionHeaderArguments(std::string & str, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool enhanced, bool singular, bool withDefaults, bool withAllocator) const
{
  str += "(";
  if (enhanced)
  {
    appendFunctionHeaderArgumentsEnhanced(str, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, singular, withDefaults, withAllocator);
  }
  else
  {
    appendFunctionHeaderArgumentsStandard(str, commandData, withDefaults);
  }
  str += ")";

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  if (!m_commandToHandle.find(commandData.first)->second.empty())
  {
    str += " const";
  }
}

void VulkanHppGenerator::appendFunctionHeaderArgumentsEnhanced(std::string & str, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool singular, bool withDefaults, bool withAllocator) const
{
  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;

  // check if there's at least one argument left to put in here
  std::set<size_t> skippedParams = determineSkippedParams(returnParamIndex, vectorParamIndices);
  if (skippedParams.size() + (handle.empty() ? 0 : 1) < commandData.second.params.size())
  {
    // determine the last argument, where we might provide some default for
    size_t lastArgument = INVALID_INDEX;
    for (size_t i = commandData.second.params.size() - 1; i < commandData.second.params.size(); i--)
    {
      if (skippedParams.find(i) == skippedParams.end())
      {
        lastArgument = i;
        break;
      }
    }

    str += " ";
    bool argEncountered = false;
    for (size_t i = handle.empty() ? 0 : 1; i < commandData.second.params.size(); i++)
    {
      argEncountered = appendFunctionHeaderArgumentEnhanced(str, commandData.second.params[i], i, vectorParamIndices, skippedParams.find(i) != skippedParams.end(), argEncountered,
        (templateParamIndex == i), (lastArgument == i), singular, withDefaults, withAllocator);
    }

    if (argEncountered)
    {
      str += ", ";
    }
  }
  if (withAllocator)
  {
    str += "Allocator const& vectorAllocator, ";
  }
  str += "Dispatch const &d";
  if (withDefaults && !withAllocator)
  {
    str += " = VULKAN_HPP_DEFAULT_DISPATCHER";
  }
  str += " ";
}

void VulkanHppGenerator::appendFunctionHeaderArgumentsStandard(std::string & str, std::pair<std::string, CommandData> const& commandData, bool withDefaults) const
{
  // for the standard case, just list all the arguments as we've got them
  // determine the last argument, where we might provide some default for
  size_t lastArgument = commandData.second.params.size() - 1;

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;

  bool argEncountered = false;
  for (size_t i = handle.empty() ? 0 : 1; i < commandData.second.params.size(); i++)
  {
    argEncountered = appendFunctionHeaderArgumentStandard(str, commandData.second.params[i], argEncountered, lastArgument == i, withDefaults);
  }
  if (argEncountered)
  {
    str += ", ";
  }

  str += "Dispatch const &d";
  if (withDefaults)
  {
    str += " = VULKAN_HPP_DEFAULT_DISPATCHER ";
  }
}

bool VulkanHppGenerator::appendFunctionHeaderArgumentStandard(std::string & str, ParamData const& param, bool argEncountered, bool isLastArgument, bool withDefaults) const
{
  if (argEncountered)
  {
    str += ",";
  }

  str += " " + param.type.compose() + " " + param.name + constructCArraySizes(param.arraySizes);

  if (withDefaults && isLastArgument)
  {
    // check if the very last argument is a flag without any bits -> provide some empty default for it
    std::map<std::string, BitmaskData>::const_iterator bitmasksIt = m_bitmasks.find(param.type.type);
    if (bitmasksIt != m_bitmasks.end())
    {
      // get the enum corresponding to this flag, to check if it's empty
      std::string strippedBitmaskName = stripPrefix(bitmasksIt->first, "Vk");
      std::map<std::string, EnumData>::const_iterator enumIt = m_enums.find(bitmasksIt->second.requirement);
      assert((enumIt == m_enums.end()) || (enumIt->second.isBitmask));
      if ((enumIt == m_enums.end()) || (enumIt->second.values.empty()))
      {
        // there are no bits in this flag -> provide the default
        str += " = " + stripPrefix(param.type.type, "Vk") + "()";
      }
    }
  }
  return true;
}

void VulkanHppGenerator::appendFunctionHeaderReturnType(std::string & str, CommandData const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, std::string const& enhancedReturnType, bool enhanced, bool twoStep, bool singular, bool unique, bool isStructureChain) const
{
  if (enhanced)
  {
    bool useTypename = ((commandData.successCodes.size() == 1) || ((commandData.successCodes.size() == 2) && (commandData.successCodes[1] == "eIncomplete") && twoStep));
    // the enhanced function might return some pretty complex return stuff
    bool isVector = (enhancedReturnType.find("Allocator") != std::string::npos);
    if (unique)
    {
      // the unique version returns something prefixed with 'Unique'; potentially a vector of that stuff
      // it's a vector, if it's not the singular version and the return parameter is a vector parameter
      bool returnsVector = !singular && (vectorParamIndices.find(returnParamIndex) != vectorParamIndices.end());

      std::string returnType = isStructureChain ? "StructureChain<X, Y, Z...>" : stripPrefix(commandData.params[returnParamIndex].type.type, "Vk");
      str += useTypename ? "typename ResultValueType<" : "ResultValue<";
      str += returnsVector ? "std::vector<UniqueHandle<" + returnType + ",Dispatch>,Allocator>>" : "UniqueHandle<" + returnType + ",Dispatch>>";
      str += useTypename ? "::type " : " ";
    }
    else if ((enhancedReturnType != stripPrefix(commandData.returnType, "Vk")) && (commandData.returnType != "void"))
    {
      // if the enhanced return type differs from the original return type, and it's not void, we return a ResultValueType<...>::type
      assert(commandData.returnType == "VkResult");
      // in singular case, we create the ResultValueType from the pure return type, otherwise from the enhanced return type
      std::string returnType = isStructureChain ? "StructureChain<X, Y, Z...>" : (singular ? stripPrefix(commandData.params[returnParamIndex].type.type, "Vk") : enhancedReturnType);
      // for the non-singular case with allocation, we need to prepend with 'typename' to keep compilers happy
      str += (useTypename ? "typename ResultValueType<" : "ResultValue<") + returnType + ">" + (useTypename ? "::type " : " ");
    }
    else if ((returnParamIndex != INVALID_INDEX) && (1 < commandData.successCodes.size()))
    {
      // if there is a return parameter at all, and there are multiple success codes, we return a ResultValue<...> with the pure return type
      assert(commandData.returnType == "VkResult");
      str += "ResultValue<" + (isStructureChain ? "StructureChain<X, Y, Z...>" : stripPrefix(commandData.params[returnParamIndex].type.type, "Vk")) + "> ";
    }
    else
    {
      // and in every other case, we just return the enhanced return type.
      str += (isStructureChain && !isVector ? "StructureChain<X, Y, Z...>" : enhancedReturnType) + " ";
    }
  }
  else
  {
    // the non-enhanced function just uses the return type
    str += stripPrefix(commandData.returnType, "Vk") + " ";
  }
}

void VulkanHppGenerator::appendFunctionHeaderTemplate(std::string & str, std::string const& indentation, size_t returnParamIndex, size_t templateParamIndex, std::string const& enhancedReturnType, bool enhanced, bool singular, bool unique, bool withDefault, bool isStructureChain) const
{
  bool withAllocator = (enhancedReturnType.find("Allocator") != std::string::npos);
  str += indentation + "template<";
  if (enhanced)
  {
    if (isStructureChain)
    {
      str += std::string("typename ") + (withAllocator ? "StructureChain" : "X, typename Y, typename ...Z") + ", ";
    }
    else if ((templateParamIndex != INVALID_INDEX) && ((templateParamIndex != returnParamIndex) || (enhancedReturnType == "Result")))
    {
      assert(!withAllocator);
      str += "typename T, ";
    }
    if (!singular && withAllocator)
    {
      // otherwise, if there's an Allocator used in the enhanced return type, we templatize on that Allocator
      assert((enhancedReturnType.substr(0, 12) == "std::vector<") && (enhancedReturnType.find(',') != std::string::npos) && (12 < enhancedReturnType.find(',')));
      str += "typename Allocator";
      if (withDefault)
      {
        // for the default type get the type from the enhancedReturnType, which is of the form 'std::vector<Type,Allocator>'
        assert(!isStructureChain || !unique);
        str += " = std::allocator<" + (isStructureChain ? "StructureChain" : (unique ? "Unique" : "") + enhancedReturnType.substr(12, enhancedReturnType.find(',') - 12)) + ">";
      }
      str += ", ";
    }
  }
  str += std::string("typename Dispatch") + (withDefault ? " = VULKAN_HPP_DEFAULT_DISPATCHER_TYPE" : "") + ">\n";
}

void VulkanHppGenerator::appendHandle(std::string & str, std::pair<std::string, HandleData> const& handleData, std::set<std::string> & listedHandles) const
{
  if (listedHandles.find(handleData.first) == listedHandles.end())
  {
    listedHandles.insert(handleData.first);

    // first check for any handle that needs to be listed before this one
    for (auto const& command : handleData.second.commands)
    {
      for (auto const& parameter : command.second.params)
      {
        std::string typeName = parameter.type.type;
        auto handlesIt = m_handles.find(typeName);
        if ((handlesIt != m_handles.end()) && (listedHandles.find(typeName) == listedHandles.end()))
        {
          appendHandle(str, *handlesIt, listedHandles);
        }
      }
    }

    if (handleData.first.empty())
    {
      for (auto const& command : handleData.second.commands)
      {
        std::string commandName = determineCommandName(command.first, command.second.params[0].type.type);
        if (command.first == "vkCreateInstance")
        {
          // special handling for createInstance, as we need to explicitly place the forward declarations and the deleter classes here
#if !defined(NDEBUG)
          auto handleIt = m_handles.find("");
          assert((handleIt != m_handles.end()) && (handleIt->second.childrenHandles.size() == 2));
          assert(handleIt->second.childrenHandles.find("VkInstance") != handleIt->second.childrenHandles.end());
#endif

          appendUniqueTypes(str, "", { "VkInstance" });
        }
        str += "\n";
        appendPlatformEnter(str, command.second.platform);
        appendCommand(str, "  ", commandName, command, false);
        appendPlatformLeave(str, command.second.platform);
      }
    }
    else
    {
      // then append any forward declaration of Deleters used by this handle
      if (!handleData.second.childrenHandles.empty())
      {
        appendUniqueTypes(str, handleData.first, handleData.second.childrenHandles);
      }
      else if (handleData.first == "VkPhysicalDevice")
      {
        // special handling for class Device, as it's created from PhysicalDevice, but destroys itself
        appendUniqueTypes(str, "", { "VkDevice" });
      }

      std::string commands;
      // list all the commands that are mapped to members of this class
      for (auto const& command : handleData.second.commands)
      {
        std::string enter, leave, commandString;
        appendPlatformEnter(enter, command.second.platform);
        appendPlatformLeave(leave, command.second.platform);
        std::string commandName = determineCommandName(command.first, command.second.params[0].type.type);
        appendCommand(commandString, "    ", commandName, command, false);
        commands += "\n" + enter + commandString;

        // special handling for destroy functions which are not aliased.
        if (!command.second.isAlias && (((command.first.substr(2, 7) == "Destroy") && (commandName != "destroy")) || (command.first.substr(2, 4) == "Free")))
        {
          commandName = (command.first.substr(2, 7) == "Destroy") ? "destroy" : "free";
          std::string destroyCommandString;
          appendCommand(destroyCommandString, "    ", commandName, command, false);
          commands += "\n" + destroyCommandString;
        }
        commands += leave;
      }

      static const std::string templateString = R"(
  class ${className}
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
  struct cpp_type<ObjectType::e${className}>
  {
    using type = ${className};
  };
)";

      str += replaceWithMap(templateString, {
        { "className", stripPrefix(handleData.first, "Vk") },
        { "memberName", startLowerCase(stripPrefix(handleData.first, "Vk")) },
        { "commands", commands }
        });

      if (!handleData.second.alias.empty())
      {
        str += "  using " + stripPrefix(handleData.second.alias, "Vk") + " = " + stripPrefix(handleData.first, "Vk") + ";\n";
      }
    }
  }
}

void VulkanHppGenerator::appendHandles(std::string & str) const
{
  std::set<std::string> listedHandles;
  for (auto const& handle : m_handles)
  {
    appendHandle(str, handle, listedHandles);
  }
}

void VulkanHppGenerator::appendHandlesCommandDefintions(std::string & str) const
{
  for (auto const& handle : m_handles)
  {
    // finally the commands, that are member functions of this handle
    for (auto const& command : handle.second.commands)
    {
      std::string commandName = determineCommandName(command.first, command.second.params[0].type.type);
      std::string strippedName = startLowerCase(stripPrefix(command.first, "vk"));

      str += "\n";
      appendPlatformEnter(str, command.second.platform);
      appendCommand(str, "  ", commandName, command, true);

      // special handling for destroy functions
      if (!command.second.isAlias && (((command.first.substr(2, 7) == "Destroy") && (commandName != "destroy")) || (command.first.substr(2, 4) == "Free")))
      {
        commandName = (command.first.substr(2, 7) == "Destroy") ? "destroy" : "free";
        str += "\n";
        appendCommand(str, "  ", commandName, command, true);
      }
      appendPlatformLeave(str, command.second.platform);
    }
  }
  str += "\n";
}

// Intended only for `enum class Result`!
void VulkanHppGenerator::appendResultExceptions(std::string & str) const
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

  auto enumData = m_enums.find("VkResult");
  for (auto const& value : enumData->second.values)
  {
    if (beginsWith(value.vkValue, "eError"))
    {
      str += replaceWithMap(templateString,
        {
          { "className", stripPrefix(value.vkValue, "eError") + "Error" },
          { "enumName", stripPrefix(enumData->first, "Vk") },
          { "enumMemberName", value.vkValue }
        });
    }
  }
  str += "\n";
}

void VulkanHppGenerator::appendPlatformEnter(std::string & str, std::string const& platform) const
{
  if (!platform.empty())
  {
    auto it = m_platforms.find(platform);
    assert((it != m_platforms.end()) && !it->second.empty());
    str += "#ifdef " + it->second + "\n";
  }
}

void VulkanHppGenerator::appendPlatformLeave(std::string & str, std::string const& platform) const
{
  if (!platform.empty())
  {
    auto it = m_platforms.find(platform);
    assert((it != m_platforms.end()) && !it->second.empty());
    str += "#endif /*" + it->second + "*/\n";
  }
}

void VulkanHppGenerator::appendStruct(std::string & str, std::pair<std::string, StructureData> const& structure, std::set<std::string> & listedStructures) const
{
  if (listedStructures.find(structure.first) == listedStructures.end())
  {
    listedStructures.insert(structure.first);
    for (auto const& member : structure.second.members)
    {
      auto structureIt = m_structures.find(member.type.type);
      if ((structureIt != m_structures.end()) && (listedStructures.find(member.type.type) == listedStructures.end()))
      {
        appendStruct(str, *structureIt, listedStructures);
      }
    }
    if (!structure.second.subStruct.empty())
    {
      auto structureIt = m_structures.find(structure.second.subStruct);
      if ((structureIt != m_structures.end()) && (listedStructures.find(structureIt->first) == listedStructures.end()))
      {
        appendStruct(str, *structureIt, listedStructures);
      }
    }

    if (structure.second.isUnion)
    {
      appendUnion(str, structure);
    }
    else
    {
      appendStructure(str, structure);
    }
  }
}

void VulkanHppGenerator::appendStructCompareOperators(std::string & str, std::pair<std::string, StructureData> const& structData) const
{
  // two structs are compared by comparing each of the elements
  std::string compareMembers;
  std::string intro = "";
  for (size_t i = 0; i < structData.second.members.size(); i++)
  {
    MemberData const& member = structData.second.members[i];
    compareMembers += intro;
    if (member.arraySizes.empty())
    {
      compareMembers += "( " + member.name + " == rhs." + member.name + " )";
    }
    else
    {
      std::string arraySize = constructArraySize(member.arraySizes);
      if ((0 < i) && ((stripPostfix(member.name, "s") + "Count") == structData.second.members[i - 1].name))
      {
        assert(structData.second.members[i - 1].type.type == "uint32_t");   // make sure, it's an unsigned type, so we don't need to clamp here
        arraySize = "std::min<" + structData.second.members[i-1].type.type + ">( " + arraySize + ", " + structData.second.members[i - 1].name + " )";
      }
      compareMembers += "( memcmp( " + member.name + ", rhs." + member.name + ", " + arraySize + " * sizeof( " + member.type.compose() + " ) ) == 0 )";
    }
    intro = "\n          && ";
  }

  static const std::string compareTemplate = R"(
    bool operator==( ${name} const& rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return ${compareMembers};
    }

    bool operator!=( ${name} const& rhs ) const VULKAN_HPP_NOEXCEPT
    {
      return !operator==( rhs );
    }
)";

  str += replaceWithMap(compareTemplate, { { "name", stripPrefix(structData.first, "Vk") }, { "compareMembers", compareMembers } });
}

std::string VulkanHppGenerator::appendStructConstructor(std::pair<std::string, StructureData> const& structData, std::string const& prefix) const
{
  // the constructor with all the elements as arguments, with defaults
  // returnedOnly structs and structs with a union (and VkBaseInStructure and VkBaseOutStructure) can't be a constexpr!
  bool isConstExpression = !structData.second.returnedOnly && !containsUnion(structData.first) && (structData.first != "VkBaseInStructure") && (structData.first != "VkBaseOutStructure");
  std::string constexprString = isConstExpression ? (std::string("VULKAN_HPP_CONSTEXPR") + (containsArray(structData.first) ? "_14 " : " ")) : "";
  std::string ctorOpening = prefix + constexprString + stripPrefix(structData.first, "Vk");
  std::string indentation(ctorOpening.size() + 2, ' ');

  std::string arguments, initializers, copyOps;
  bool listedArgument = false;
  bool firstArgument = true;
  for (auto const& member : structData.second.members)
  {
    // gather the arguments
    listedArgument = appendStructConstructorArgument(arguments, listedArgument, indentation, member);

    // gather the initializers; skip members 'pNext' and 'sType', they are directly set by initializers
    if ((member.name != "pNext") && (member.name != "sType"))
    {
      if (member.arraySizes.empty())
      {
        // here, we can only handle non-array arguments
        initializers += prefix + "  " + (firstArgument ? ":" : ",") + " " + member.name + "( " + member.name + "_ )\n";
        firstArgument = false;
      }
      else
      {
        // here we can handle the arrays, copying over from argument (with trailing '_') to member
        initializers += prefix + "  " + (firstArgument ? ":" : ",") + " " + member.name + "{}\n"; // need to initialize the array
        firstArgument = false;

        std::string type = (member.type.type.substr(0, 2) == "Vk") ? ("VULKAN_HPP_NAMESPACE::" + stripPrefix(member.type.type, "Vk")) : member.type.type;
        std::string arraySizes;
        for (auto const& as : member.arraySizes)
        {
          arraySizes += "," + as;
        }
        copyOps += prefix + "  VULKAN_HPP_NAMESPACE::ConstExpression" + std::to_string(member.arraySizes.size()) + "DArrayCopy<" + type + arraySizes + arraySizes + ">::copy( " + member.name + ", " + member.name + "_ );\n";
      }
    }
  }

  std::string structConstructor = ctorOpening + (arguments.empty() ? "()" : std::string("( " + arguments + " )")) + " VULKAN_HPP_NOEXCEPT\n";
  structConstructor += initializers;
  if (copyOps.empty())
  {
    structConstructor += prefix + "{}\n";
  }
  else
  {
    structConstructor += prefix + "{\n" + copyOps + prefix + "}\n";
  }

  if (!structData.second.subStruct.empty())
  {
    auto const& subStruct = m_structures.find(structData.second.subStruct);
    assert(subStruct != m_structures.end());

    std::string subStructArgumentName = startLowerCase(stripPrefix(subStruct->first, "Vk"));
    ctorOpening = prefix + "explicit " + stripPrefix(structData.first, "Vk") + "( ";
    indentation = std::string(ctorOpening.size(), ' ');

    std::string subCopies;
    firstArgument = true;
    for (size_t i = 0; i < subStruct->second.members.size(); i++)
    {
      assert(structData.second.members[i].arraySizes.empty());
      subCopies += prefix + "  " + (firstArgument ? ":" : ",") + " " + structData.second.members[i].name + "( " + subStructArgumentName + "." + subStruct->second.members[i].name + " )\n";
      firstArgument = false;
    }

    std::string subArguments;
    listedArgument = true;
    for (size_t i = subStruct->second.members.size(); i < structData.second.members.size(); i++)
    {
      listedArgument = appendStructConstructorArgument(subArguments, listedArgument, indentation, structData.second.members[i]);

      assert(structData.second.members[i].arraySizes.empty());
      subCopies += prefix + "  , " + structData.second.members[i].name + "( " + structData.second.members[i].name + "_ )\n";
    }

    structConstructor += "\n"
      "    explicit " + stripPrefix(structData.first, "Vk") + "( " + stripPrefix(subStruct->first, "Vk") + " const& " + subStructArgumentName + subArguments + " )\n"
      + subCopies +
      "    {}\n";
  }

  // we need a copy constructor if there is constant sType in this struct
  if ((nonConstSTypeStructs.find(structData.first) == nonConstSTypeStructs.end()) && !structData.second.members.empty() && (structData.second.members.front().name == "sType"))
  {
    assert((2 <= structData.second.members.size()) && (structData.second.members[1].name == "pNext"));

    static const std::string stringTemplate = R"(
${prefix}VULKAN_HPP_NAMESPACE::${structName} & operator=( VULKAN_HPP_NAMESPACE::${structName} const & rhs ) VULKAN_HPP_NOEXCEPT
${prefix}{
${prefix}  memcpy( &pNext, &rhs.pNext, sizeof( VULKAN_HPP_NAMESPACE::${structName} ) - offsetof( ${structName}, pNext ) );
${prefix}  return *this;
${prefix}}
)";
    structConstructor += replaceWithMap(stringTemplate, { {"prefix", prefix }, { "structName", stripPrefix(structData.first, "Vk") } });
  }

  return structConstructor;
}

bool VulkanHppGenerator::appendStructConstructorArgument(std::string & str, bool listedArgument, std::string const& indentation, MemberData const& memberData) const
{
  // skip members 'pNext' and 'sType', as they are never explicitly set
  if ((memberData.name != "pNext") && (memberData.name != "sType"))
  {
    str += (listedArgument ? (",\n" + indentation) : "");
    if (memberData.arraySizes.empty())
    {
      str += memberData.type.compose() + " ";
    }
    else
    {
      str += constructStandardArray(memberData.type.compose(), memberData.arraySizes) + " const& ";
    }
    str += memberData.name + "_ = {}";
    listedArgument = true;
  }
  return listedArgument;
}

void VulkanHppGenerator::appendStructCopyConstructors(std::string & str, std::string const& name) const
{
  static const std::string templateString = R"(
    ${name}( Vk${name} const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      *this = rhs;
    }

    ${name}& operator=( Vk${name} const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      *this = *reinterpret_cast<VULKAN_HPP_NAMESPACE::${name} const *>(&rhs);
      return *this;
    }
)";

  str += replaceWithMap(templateString, { { "name", name } });
}

void VulkanHppGenerator::appendStructMembers(std::string & str, std::pair<std::string,StructureData> const& structData, std::string const& prefix) const
{
  for (auto const& member : structData.second.members)
  {
    str += prefix;
    if ((member.name == "sType") && (nonConstSTypeStructs.find(structData.first) == nonConstSTypeStructs.end()))  // special handling for sType and some nasty little structs that don't get a const sType
    {
      str += "const ";
    }
    if (!member.bitCount.empty() && beginsWith(member.type.type, "Vk"))
    {
      assert(member.type.prefix.empty() && member.type.postfix.empty());    // never encounterd a different case
      str += member.type.type;
    }
    else
    {
      str += member.type.compose();
    }
    str += " " + member.name;
    if (member.name == "sType")           // special handling for sType
    {
      auto enumIt = m_enums.find("VkStructureType");
      assert(enumIt != m_enums.end());
      if (!member.values.empty())
      {
        assert(beginsWith(member.values, "VK_STRUCTURE_TYPE"));
        auto nameIt = std::find_if(enumIt->second.values.begin(), enumIt->second.values.end(), [&member](EnumValueData const& evd) { return member.values == evd.vulkanValue; });
        assert(nameIt != enumIt->second.values.end());
        str += " = StructureType::" + nameIt->vkValue;
      }
      else
      {
        // special handling for those nasty structs with an unspecified value for sType
        str += " = {}";
      }
    }
    else
    {
      // as we don't have any meaningful default initialization values, everything can be initialized by just '{}' !
      assert(member.arraySizes.empty() || member.bitCount.empty());
      if (!member.bitCount.empty())
      {
        str += " : " + member.bitCount;   // except for bitfield members, where no default member initializatin is supported (up to C++20)
      }
      else
      {
        str += constructCArraySizes(member.arraySizes) + " = {}";
      }
    }
    str += ";\n";
  }
}

void VulkanHppGenerator::appendStructs(std::string & str) const
{
  std::set<std::string> listedStructures;
  for (auto const& structure : m_structures)
  {
    appendStruct(str, structure, listedStructures);
  }
}

void VulkanHppGenerator::appendStructSetter(std::string & str, std::string const& structureName, MemberData const& memberData) const
{
  if (memberData.type.type != "VkStructureType") // filter out StructureType, which is supposed to be immutable !
  {
    std::string memberType;
    if (memberData.arraySizes.empty())
    {
      memberType = memberData.type.compose();
    }
    else
    {
      memberType = constructStandardArray(memberData.type.compose(), memberData.arraySizes);
    }

    // copy over the argument, either by assigning simple data, or by memcpy array data
    str += "\n"
      "    " + structureName + " & set" + startUpperCase(memberData.name) + "( " + memberType + " " + memberData.name + "_ ) VULKAN_HPP_NOEXCEPT\n"
      "    {\n"
      "      ";
    if (memberData.arraySizes.empty())
    {
      if (!memberData.bitCount.empty() && beginsWith(memberData.type.type, "Vk"))
      {
        str += memberData.name + " = " + "*reinterpret_cast<" + memberData.type.type + "*>(&" + memberData.name + "_)";
      }
      else
      {
        str += memberData.name + " = " + memberData.name + "_";
      }
    }
    else
    {
      str += "memcpy( " + memberData.name + ", " + memberData.name + "_.data(), " + constructArraySize(memberData.arraySizes) + " * sizeof( " + memberData.type.compose() + " ) )";
    }
    str += ";\n"
      "      return *this;\n"
      "    }\n";
  }
}

void VulkanHppGenerator::appendStructure(std::string & str, std::pair<std::string, StructureData> const& structure) const
{
  str += "\n";
  appendPlatformEnter(str, structure.second.platform);

  std::string constructorAndSetters = appendStructConstructor(structure, "    ");
  appendStructCopyConstructors(constructorAndSetters, stripPrefix(structure.first, "Vk"));
  if (!structure.second.returnedOnly)
  {
    // only structs that are not returnedOnly get setters!
    for (auto const& member : structure.second.members)
    {
      appendStructSetter(constructorAndSetters, stripPrefix(structure.first, "Vk"), member);
    }
  }

  // operator==() and operator!=()
  // only structs without a union as a member can have a meaningfull == and != operation; we filter them out
  std::string compareOperators;
  if (!containsUnion(structure.first))
  {
    appendStructCompareOperators(compareOperators, structure);
  }

  // the member variables
  std::string members = "\n  public:\n";
  appendStructMembers(members, structure, "    ");

  static const std::string structureTemplate = R"(
  struct ${name}
  {
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
  static_assert( sizeof( ${name} ) == sizeof( ${vkName} ), "struct and wrapper have different size!" );
  static_assert( std::is_standard_layout<${name}>::value, "struct wrapper is not a standard layout!" );
)";

  str += replaceWithMap(structureTemplate,
    {
      { "name",                   stripPrefix(structure.first, "Vk") },
      { "constructorAndSetters",  constructorAndSetters },
      { "vkName",                 structure.first },
      { "compareOperators",       compareOperators },
      { "members",                members }
    });
  appendPlatformLeave(str, structure.second.platform);
}

void VulkanHppGenerator::appendStructureChainValidation(std::string & str)
{
  // append all template functions for the structure pointer chain validation
  for (auto const& structure : m_structures)
  {
    if (!structure.second.structExtends.empty())
    {
      appendPlatformEnter(str, structure.second.platform);

      // append out allowed structure chains
      for (auto extendName : structure.second.structExtends)
      {
        std::map<std::string, StructureData>::const_iterator itExtend = m_structures.find(extendName);
        if (itExtend == m_structures.end())
        {
          std::string errorString;
          errorString = extendName + " does not specify a struct in structextends field.";

          // check if symbol name is an alias to a struct
          auto itAlias = std::find_if(m_structures.begin(), m_structures.end(), [&extendName](std::pair<std::string, StructureData> const &it) -> bool {return std::find(it.second.aliases.begin(), it.second.aliases.end(), extendName) != it.second.aliases.end(); });
          if (itAlias != m_structures.end())
          {
            errorString += " The symbol is an alias and maps to " + itAlias->first + ".";
          }

          errorString += "\n";
          throw std::runtime_error(errorString);
        }
        if (structure.second.platform != itExtend->second.platform)
        {
          appendPlatformEnter(str, itExtend->second.platform);
        }

        str += "  template <> struct isStructureChainValid<" + stripPrefix(extendName, "Vk") + ", " + stripPrefix(structure.first, "Vk") + ">{ enum { value = true }; };\n";

        if (structure.second.platform != itExtend->second.platform)
        {
          appendPlatformLeave(str, itExtend->second.platform);
        }
      }
      appendPlatformLeave(str, structure.second.platform);
    }
  }
}

void VulkanHppGenerator::appendThrowExceptions(std::string & str) const
{
  auto enumData = m_enums.find("VkResult");

  str += "\n"
    "  [[noreturn]] static void throwResultException( Result result, char const * message )\n"
    "  {\n"
    "    switch ( result )\n"
    "    {\n";
  for (auto const& value : enumData->second.values)
  {
    if (beginsWith(value.vkValue, "eError"))
    {
      str += "      case Result::" + value.vkValue + ": throw " + stripPrefix(value.vkValue, "eError") + "Error( message );\n";
    }
  }
  str += "      default: throw SystemError( make_error_code( result ) );\n"
    "    }\n"
    "  }\n";
}

void VulkanHppGenerator::appendUnion(std::string & str, std::pair<std::string, StructureData> const& structure) const
{
  str += "\n"
    "  union " + stripPrefix(structure.first, "Vk") + "\n"
    "  {\n";

  if (!structure.second.returnedOnly)
  {
    bool firstMember = true;
    for (auto const& member : structure.second.members)
    {
      // VkBool32 is aliased to uint32_t. Don't create a VkBool32 constructor if the union also contains a uint32_t constructor.
      auto compareBool32Alias = [](MemberData const& member) { return member.type.type == std::string("uint32_t"); };
      if (member.type.type == "VkBool32") {
        if (std::find_if(structure.second.members.begin(), structure.second.members.end(), compareBool32Alias) != structure.second.members.end())
        {
          continue;
        }
      }

      static const std::string constructorTemplate = R"(
    ${unionName}( ${memberType} ${memberName}_${defaultAssignment} )
    {
      ${memberAssignment};
    }
)";
      std::string memberAssignment, memberType;
      if (member.arraySizes.empty())
      {
        memberAssignment = member.name + " = " + member.name + "_";
        memberType = member.type.compose();
      }
      else
      {
        std::string arraySize = constructArraySize(member.arraySizes);
        memberAssignment = "memcpy( " + member.name + ", " + member.name + "_.data(), " + arraySize + " * sizeof( " + member.type.compose() + " ) )";
        memberType = "const " + constructStandardArray(member.type.compose(), member.arraySizes) + "&";
      }

      str += replaceWithMap(constructorTemplate,
        {
          { "defaultAssignment",  firstMember ? " = {}" : "" },
          { "memberAssignment",   memberAssignment },
          { "memberName",         member.name },
          { "memberType",         memberType },
          { "unionName",          stripPrefix(structure.first, "Vk") }
        });
      firstMember = false;
    }

    // one setter per union element
    for (auto const& member : structure.second.members)
    {
      appendStructSetter(str, stripPrefix(structure.first, "Vk"), member);
    }
  }

  // assignment operator
  static const std::string operatorsTemplate = R"(
    VULKAN_HPP_NAMESPACE::${unionName} & operator=( VULKAN_HPP_NAMESPACE::${unionName} const & rhs ) VULKAN_HPP_NOEXCEPT
    {
      memcpy( this, &rhs, sizeof( VULKAN_HPP_NAMESPACE::${unionName} ) );
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
  str += replaceWithMap(operatorsTemplate, { { "unionName", stripPrefix(structure.first, "Vk") } });

  // the union member variables
  // if there's at least one Vk... type in this union, check for unrestricted unions support
  bool needsUnrestrictedUnions = (std::find_if(structure.second.members.begin(), structure.second.members.end(), [](MemberData const& member) { return beginsWith(member.type.type, "Vk"); }) != structure.second.members.end());
  if (needsUnrestrictedUnions)
  {
    str += "#ifdef VULKAN_HPP_HAS_UNRESTRICTED_UNIONS\n";
  }
  for (auto const& member : structure.second.members)
  {
    str += "    " + member.type.compose() + " " + member.name + constructCArraySizes(member.arraySizes) + ";\n";
  }
  if (needsUnrestrictedUnions)
  {
    str += "#else\n";
    for (auto const& member : structure.second.members)
    {
      str += "    " + member.type.prefix + (member.type.prefix.empty() ? "" : " ") + member.type.type + member.type.postfix + " " + member.name + constructCArraySizes(member.arraySizes) + ";\n";
    }
    str += "#endif  /*VULKAN_HPP_HAS_UNRESTRICTED_UNIONS*/\n";
  }
  str += "  };\n";
}

void VulkanHppGenerator::appendUniqueTypes(std::string & str, std::string const& parentType, std::set<std::string> const& childrenTypes) const
{
  str += "\n"
    "#ifndef VULKAN_HPP_NO_SMART_HANDLE";
  if (!parentType.empty())
  {
    str += "\n"
      "  class " + stripPrefix(parentType, "Vk") + ";";
  }

  for (auto const& childType : childrenTypes)
  {
    auto handleIt = m_handles.find(childType);
    assert(handleIt != m_handles.end());

    std::string type = stripPrefix(childType, "Vk");
    std::string deleterType = handleIt->second.deletePool.empty() ? "Object" : "Pool";
    std::string deleterAction = (handleIt->second.deleteCommand.substr(2, 4) == "Free") ? "Free" : "Destroy";
    std::string deleterParent = parentType.empty() ? "NoParent" : stripPrefix(parentType, "Vk");
    std::string deleterPool = handleIt->second.deletePool.empty() ? "" : ", " + stripPrefix(handleIt->second.deletePool, "Vk");
    str += "\n"
      "  template <typename Dispatch> class UniqueHandleTraits<" + type + ", Dispatch> { public: using deleter = " + deleterType + deleterAction + "<" + deleterParent + deleterPool + ", Dispatch>; };\n"
      "  using Unique" + type + " = UniqueHandle<" + type + ", VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;";

    if (!handleIt->second.alias.empty())
    {
      str += "\n"
        "  using Unique" + stripPrefix(handleIt->second.alias, "Vk") + " = UniqueHandle<" + type + ", VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;";
    }
  }
  str += "\n"
    "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/\n";
}

void VulkanHppGenerator::EnumData::addEnumValue(std::string const &valueName, bool bitmask, bool bitpos, std::string const& prefix, std::string const& postfix, std::string const& tag)
{
  std::string translatedName = createEnumValueName(valueName, prefix, postfix, bitmask, tag);

  auto it = std::find_if(values.begin(), values.end(), [&translatedName](EnumValueData const& evd) { return evd.vkValue == translatedName; });
  if (it == values.end())
  {
    values.push_back(EnumValueData(valueName, translatedName, bitpos));
  }
  else
  {
    assert(it->vulkanValue == valueName);
  }
}

bool VulkanHppGenerator::containsArray(std::string const& type) const
{
  // a simple recursive check if a type is or contains an array
  auto structureIt = m_structures.find(type);
  bool found = false;
  if (structureIt != m_structures.end())
  {
    for (auto memberIt = structureIt->second.members.begin(); memberIt != structureIt->second.members.end() && !found; ++memberIt)
    {
      found = !memberIt->arraySizes.empty();
    }
  }
  return found;
}

bool VulkanHppGenerator::containsUnion(std::string const& type) const
{
  // a simple recursive check if a type is or contains a union
  auto structureIt = m_structures.find(type);
  bool found = (structureIt != m_structures.end());
  if (found)
  {
    found = structureIt->second.isUnion;
    for (auto memberIt = structureIt->second.members.begin(); memberIt != structureIt->second.members.end() && !found; ++memberIt)
    {
      found = memberIt->type.prefix.empty() && memberIt->type.postfix.empty() && containsUnion(memberIt->type.type);
    }
  }
  return found;
}

void VulkanHppGenerator::checkCorrectness()
{
  for (auto const& handle : m_handles)
  {
    for (auto const& command: handle.second.commands)
    {
      // check that functions returning a VkResult specify successcodes
      if ((command.second.returnType == "VkResult") && command.second.successCodes.empty())
      {
        throw std::runtime_error("Spec error on command Vk" + startUpperCase(command.first) + " : missing successcodes on command returning VkResult!");
      }
    }
  }
}

std::string VulkanHppGenerator::determineEnhancedReturnType(CommandData const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool isStructureChain) const
{
  assert((returnParamIndex == INVALID_INDEX) || (returnParamIndex < commandData.params.size()));
  for (auto vpi : vectorParamIndices)
  {
    assert((vpi.first != vpi.second) && (vpi.first < commandData.params.size()) && ((vpi.second == INVALID_INDEX) || (vpi.second < commandData.params.size())));
  }

  std::string enhancedReturnType;
  // if there is a return parameter of type void or Result, and if it's of type Result it either has just one success code
  // or two success codes, where the second one is of type eIncomplete and it's a two-step process
  // -> we can return that parameter
  if ((returnParamIndex != INVALID_INDEX)
    && ((commandData.returnType == "void")
      || ((commandData.returnType == "VkResult")
        && ((commandData.successCodes.size() == 1)
          || ((commandData.successCodes.size() == 2) && (commandData.successCodes[1] == "eIncomplete") && twoStep)
          || ((commandData.successCodes.size() == 3) && (commandData.successCodes[1] == "eOperationDeferredKHR") && (commandData.successCodes[2] == "eOperationNotDeferredKHR"))))))
  {
    assert(commandData.successCodes.empty() || (commandData.successCodes[0] == "eSuccess"));
    if (vectorParamIndices.find(returnParamIndex) != vectorParamIndices.end())
    {
      enhancedReturnType = (commandData.params[returnParamIndex].type.type == "void")
        ? "std::vector<uint8_t,Allocator>"                                                                      // the return parameter is a vector-type parameter
        : isStructureChain
          ? "std::vector<StructureChain,Allocator>"                                                             // for structureChain returns, it's just a vector of StrutureChains
          : "std::vector<" + stripPrefix(commandData.params[returnParamIndex].type.type, "Vk") + ",Allocator>"; // for the other parameters, we use a vector of the pure type
    }
    else
    {
      // it's a simple parameter -> get the type and just remove the trailing '*' (originally, it's a pointer)
      assert(commandData.params[returnParamIndex].type.postfix.back() == '*');
      assert((commandData.params[returnParamIndex].type.prefix.find("const") == std::string::npos) && (commandData.params[returnParamIndex].type.postfix.find("const") == std::string::npos));
      enhancedReturnType = stripPostfix(commandData.params[returnParamIndex].type.compose(), "*");
    }
  }
  else if ((commandData.returnType == "VkResult") && (commandData.successCodes.size() == 1))
  {
    // an original return of type "Result" with just one successCode is changed to void, errors throw an exception
    enhancedReturnType = "void";
  }
  else
  {
    // the return type just stays the original return type
    enhancedReturnType = stripPrefix(commandData.returnType, "Vk");
  }
  return enhancedReturnType;
}

size_t VulkanHppGenerator::determineReturnParamIndex(CommandData const& commandData, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep) const
{
  for (auto vpi : vectorParamIndices)
  {
    assert((vpi.first != vpi.second) && (vpi.first < commandData.params.size()) && ((vpi.second == INVALID_INDEX) || (vpi.second < commandData.params.size())));
  }

  size_t returnParamIndex = INVALID_INDEX;
  // for return types of type VkResult or void, we can determine a parameter to return
  if ((commandData.returnType == "VkResult") || (commandData.returnType == "void"))
  {
    for (size_t i = 0; i < commandData.params.size(); i++)
    {
      if ((commandData.params[i].type.postfix.find('*') != std::string::npos)
        && ((commandData.params[i].type.type != "void") || twoStep || (commandData.params[i].type.postfix.find("**") != std::string::npos))
        && (commandData.params[i].type.prefix.find("const") == std::string::npos)
        && std::find_if(vectorParamIndices.begin(), vectorParamIndices.end(), [i](std::pair<size_t, size_t> const& vpi) { return vpi.second == i; }) == vectorParamIndices.end())
      {
        // it's a non-const pointer and not a vector-size parameter
        std::map<size_t, size_t>::const_iterator vpit = vectorParamIndices.find(i);
        if ((vpit == vectorParamIndices.end()) || twoStep || (vectorParamIndices.size() > 1) || (vpit->second == INVALID_INDEX) || (commandData.params[vpit->second].type.postfix.find('*') != std::string::npos))
        {
          // it's not a vector parameter, or a two-step process, or there is at least one more vector parameter, or the size argument of this vector parameter is not an argument, or the size argument of this vector parameter is provided by a pointer
          // -> look for another non-cost pointer argument
          auto paramIt = std::find_if(commandData.params.begin() + i + 1, commandData.params.end(), [](ParamData const& pd)
          {
            return (pd.type.postfix.find('*') != std::string::npos) && (pd.type.postfix.find("const") == std::string::npos);
          });
          // if there is another such argument, we can't decide which one to return -> return INVALID_INDEX
          // otherwise return the index of the selcted parameter
          returnParamIndex = paramIt != commandData.params.end() ? INVALID_INDEX : i;
        }
      }
    }
  }
  return returnParamIndex;
}

std::string VulkanHppGenerator::determineSubStruct(std::pair<std::string, StructureData> const& structure) const
{
  for (auto const& s : m_structures)
  {
    if ((s.first != structure.first) && (s.second.members.size() < structure.second.members.size()) && (s.second.members[0].name != "sType"))
    {
      bool equal = true;
      for (size_t i = 0; i < s.second.members.size() && equal; i++)
      {
        equal = (s.second.members[i].type == structure.second.members[i].type) && (s.second.members[i].name == structure.second.members[i].name);
      }
      if (equal)
      {
        return s.first;
      }
    }
  }
  return "";
}

size_t VulkanHppGenerator::determineTemplateParamIndex(std::vector<ParamData> const& params, std::map<size_t, size_t> const& vectorParamIndices) const
{
  size_t templateParamIndex = INVALID_INDEX;

  for (size_t i = 0; i < params.size(); i++)
  {
    // any vector parameter on the pure type void is templatized in the enhanced API
    if ((vectorParamIndices.find(i) != vectorParamIndices.end()) && (params[i].type.type == "void"))
    {
#if !defined(NDEBUG)
      for (size_t j = i + 1; j < params.size(); j++)
      {
        assert((vectorParamIndices.find(j) == vectorParamIndices.end()) || (params[j].type.type != "void"));
      }
#endif
      templateParamIndex = i;
      break;
    }
  }
  assert((templateParamIndex == INVALID_INDEX) || (vectorParamIndices.find(templateParamIndex) != vectorParamIndices.end()));
  return templateParamIndex;
}

std::map<size_t, size_t> VulkanHppGenerator::determineVectorParamIndices(std::vector<ParamData> const& params) const
{
  std::map<size_t, size_t> vectorParamIndices;

  // look for the parameters whose len equals the name of an other parameter
  for (auto it = params.begin(); it != params.end(); ++it)
  {
    if (!it->len.empty())
    {
      auto findLambda = [it](ParamData const& pd) { return pd.name == it->len; };
      auto findIt = std::find_if(params.begin(), it, findLambda);                        // look for a parameter named as the len of this parameter
      assert((std::count_if(params.begin(), params.end(), findLambda) == 0) || (findIt < it));    // make sure, there is no other parameter like that

      // add this parameter as a vector parameter, using the len-name parameter as the second value (or INVALID_INDEX if there is nothing like that)
      vectorParamIndices.insert(std::make_pair(std::distance(params.begin(), it), (findIt < it) ? std::distance(params.begin(), findIt) : INVALID_INDEX));
      assert((vectorParamIndices[std::distance(params.begin(), it)] != INVALID_INDEX)
        || (it->len == "null-terminated")
        || (it->len == "pAllocateInfo::descriptorSetCount")
        || (it->len == "pAllocateInfo::commandBufferCount"));
    }
  }
  return vectorParamIndices;
}

std::string const& VulkanHppGenerator::getTypesafeCheck() const
{
  return m_typesafeCheck;
}

std::string const& VulkanHppGenerator::getVersion() const
{
  return m_version;
}

std::string const& VulkanHppGenerator::getVulkanLicenseHeader() const
{
  return m_vulkanLicenseHeader;
}

bool VulkanHppGenerator::isTwoStepAlgorithm(std::vector<ParamData> const& params) const
{
  // we generate a two-step algorithm for functions returning a vector of stuff, where the length is specified as a pointer as well
  // for those functions, the size can be queried first, and then used
  bool isTwoStep = false;
  for (auto paramIt = params.begin(); paramIt != params.end() && !isTwoStep; ++paramIt)
  {
    if (!paramIt->len.empty())
    {
      auto lenIt = std::find_if(params.begin(), paramIt, [paramIt](ParamData const& pd) { return paramIt->len == pd.name; });
      if (lenIt != paramIt)
      {
        isTwoStep = (lenIt->type.postfix.find('*') != std::string::npos);
      }
    }
  }
  return isTwoStep;
}

void VulkanHppGenerator::linkCommandToHandle(std::string const& name, CommandData const& commandData)
{
  // first, find the handle named like the type of the first argument
  // if there is no such handle, look for the unnamed "handle", that gathers all the functions not tied to a specific handle
  assert(!commandData.params.empty());
  std::map<std::string, HandleData>::iterator handleIt = m_handles.find(commandData.params[0].type.type);
  if (handleIt == m_handles.end())
  {
    handleIt = m_handles.find("");
  }
  assert(handleIt != m_handles.end());

  // put the command into the handle's list of commands
  assert(handleIt->second.commands.find(name) == handleIt->second.commands.end());
  handleIt->second.commands[name] = commandData;

  // and store the handle in the command-to-handle map
  assert(m_commandToHandle.find(name) == m_commandToHandle.end());
  m_commandToHandle[name] = handleIt->first;
}

void VulkanHppGenerator::readBaseType(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { { "category",{ "basetype" } } }, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkOrderedElements(children, { "type", "name" });
  checkEmptyElement(children[0]);
  checkEmptyElement(children[1]);

  std::string type = children[0]->GetText();
  std::string name = children[1]->GetText();
#if !defined(NDEBUG)
  assert((type == "uint32_t") || (type == "uint64_t"));
#endif

  assert(m_baseTypes.find(name) == m_baseTypes.end());
  m_baseTypes[name] = type;
}

void VulkanHppGenerator::readBitmask(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { { "category",{ "bitmask" } } }, { { "alias",{} },{ "name",{} },{ "requires",{} } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);

  auto aliasIt = attributes.find("alias");
  if (aliasIt != attributes.end())
  {
    readBitmaskAlias(element->GetLineNum(), aliasIt->second, attributes, children);
  }
  else
  {
    checkOrderedElements(children, { "type", "name" });
    checkEmptyElement(children[0]);
    checkEmptyElement(children[1]);

    assert(strcmp(children[0]->GetText(), "VkFlags") == 0);

    std::string name = children[1]->GetText();

    std::string requirement;
    auto requiresIt = attributes.find("requires");
    if (requiresIt != attributes.end())
    {
      requirement = requiresIt->second;
    }

    m_bitmasks.insert(std::make_pair(name, BitmaskData(requirement)));
  }
}

void VulkanHppGenerator::readBitmaskAlias(int lineNum, std::string const& alias, std::map<std::string, std::string> const& attributes, std::vector<tinyxml2::XMLElement const*> const& children)
{
  checkAttributes(attributes, lineNum, { { "alias",{} },{ "category",{ "bitmask" } },{ "name",{} } }, {});   // re-check on alias type!
  checkElements(children, {});

  checkAlias(m_bitmasks, alias, lineNum);

  auto bitmasksIt = m_bitmasks.find(alias);
  assert((bitmasksIt != m_bitmasks.end()) && bitmasksIt->second.alias.empty());
  bitmasksIt->second.alias = attributes.find("name")->second;
}

void VulkanHppGenerator::readCommand(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), {},
  {
    { "alias",{} },
    { "cmdbufferlevel",{ "primary", "secondary" } },
    { "comment",{} },
    { "errorcodes",{} },
    { "name",{} },
    { "pipeline",{ "compute", "graphics", "transfer" } },
    { "queues",{ "compute", "graphics", "sparse_binding", "transfer" } },
    { "renderpass",{ "both", "inside", "outside" } },
    { "successcodes",{} }
  });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);

  std::string name;
  CommandData commandData;
  auto aliasIt = attributes.find("alias");
  if (aliasIt != attributes.end())
  {
    readCommandAlias(element->GetLineNum(), aliasIt->second, attributes, children);
  }
  else
  {
    checkElements(children, { "implicitexternsyncparams", "param", "proto" });

    commandData.successCodes = readCommandSuccessCodes(attributes);

    for (auto child : children)
    {
      std::string value = child->Value();
      if (value == "param")
      {
        commandData.params.push_back(readCommandParam(child));
      }
      else if (value == "proto")
      {
        name = readCommandProto(child, commandData.returnType);
      }
    }

    registerDeleter(name, std::make_pair(name, commandData));
    linkCommandToHandle(name, commandData);
  }
}

void VulkanHppGenerator::readCommandAlias(int lineNum, std::string const& alias, std::map<std::string, std::string> const& attributes, std::vector<tinyxml2::XMLElement const*> const& children)
{
  // for command aliases, create a copy of the aliased command
  checkAttributes(attributes, lineNum, { { "alias",{} },{ "name",{} } }, {});    // re-check on alias type!
  checkElements(children, {});

  std::string name = attributes.find("name")->second;
  checkAlias(m_commandToHandle, alias, lineNum);
  auto handleIt = m_handles.find(m_commandToHandle.find(alias)->second);
  assert(handleIt != m_handles.end());
  auto commandsIt = handleIt->second.commands.find(alias);
  assert(commandsIt != handleIt->second.commands.end());

  // create a copy of the found command, mark that as an alias and link it to a handle
  CommandData commandData = commandsIt->second;
  commandData.isAlias = true;
  linkCommandToHandle(name, commandData);
}

VulkanHppGenerator::ParamData VulkanHppGenerator::readCommandParam(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), {}, { { "externsync",{} },{ "len",{} },{ "noautovalidity",{ "true" } },{ "optional",{ "false", "true" } } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "name", "type" });

  ParamData paramData;
  for (auto attribute : attributes)
  {
    if (attribute.first == "len")
    {
      paramData.len = attribute.second;
    }
    else if (attribute.first == "optional")
    {
      paramData.optional = (attribute.second == "true");
    }
  }

  for (auto child : children)
  {
    checkEmptyElement(child);
    std::string value = child->Value();
    if (value == "name")
    {
      paramData.name = child->GetText();
      std::string bitCount;
      std::tie(paramData.arraySizes, bitCount) = readModifiers(child->NextSibling());
      assert(bitCount.empty());
    }
    else if (value == "type")
    {
      paramData.type.prefix = readTypePrefix(child->PreviousSibling());
      paramData.type.type = child->GetText();
      paramData.type.postfix = readTypePostfix(child->NextSibling());
    }
  }

  return paramData;
}

std::string VulkanHppGenerator::readCommandProto(tinyxml2::XMLElement const* element, std::string & returnType)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), {}, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkOrderedElements(children, { "type", "name" });

  // get return type and name of the command
  returnType = children[0]->GetText();

  // add an empty DependencyData to this name
  std::string name = children[1]->GetText();
  return name;
}

void VulkanHppGenerator::readCommands(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), {}, { { "comment",{} } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "command" });

  for (auto child : children)
  {
    readCommand(child);
  }
}

std::vector<std::string> VulkanHppGenerator::readCommandSuccessCodes(std::map<std::string, std::string> const& attributes)
{
  // read the success codes
  std::vector<std::string> successCodes;
  auto successcodesAttribute = attributes.find("successcodes");
  if (successcodesAttribute != attributes.end())
  {
    successCodes = tokenize(successcodesAttribute->second, ',');
    for (auto & code : successCodes)
    {
      std::string tag = findTag(m_tags, code);
      // on each success code: prepend 'e', strip "VK_" and a tag, convert it to camel case, and add the tag again
      code = std::string("e") + toCamelCase(stripPostfix(stripPrefix(code, "VK_"), tag)) + tag;
    }
  }
  return successCodes;
}

void VulkanHppGenerator::readComment(tinyxml2::XMLElement const* element)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), {}, {});
  checkElements(getChildElements(element), {});

  assert(element->GetText());
  std::string text = element->GetText();
  if (text.find("\nCopyright") == 0)
  {
    assert(m_vulkanLicenseHeader.empty());
    m_vulkanLicenseHeader = text;

    // replace any '\n' with "\n// "
    for (size_t pos = m_vulkanLicenseHeader.find('\n'); pos != std::string::npos; pos = m_vulkanLicenseHeader.find('\n', pos + 1))
    {
      m_vulkanLicenseHeader.replace(pos, 1, "\n// ");
    }

    // and add a little message on our own
    m_vulkanLicenseHeader += "\n\n// This header is generated from the Khronos Vulkan XML API Registry.";
    m_vulkanLicenseHeader = trim(m_vulkanLicenseHeader) + "\n";
  }
}

void VulkanHppGenerator::readDefine(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { { "category",{ "define" } } }, { { "name",{} } });

  auto nameIt = attributes.find("name");
  if (nameIt != attributes.end())
  {
    assert(!element->FirstChildElement());
    assert(nameIt->second == "VK_DEFINE_NON_DISPATCHABLE_HANDLE");

    // filter out the check for the different types of VK_DEFINE_NON_DISPATCHABLE_HANDLE
    std::string text = element->LastChild()->ToText()->Value();
    size_t start = text.find("#if defined(__LP64__)");
    size_t end = text.find_first_of("\r\n", start + 1);
    m_typesafeCheck = text.substr(start, end - start);
  }
  else if (element->GetText() && (trim(element->GetText()) == "struct"))
  {
#if !defined(NDEBUG)
    tinyxml2::XMLElement const* child = element->FirstChildElement();
    assert(child && (strcmp(child->Value(), "name") == 0) && child->GetText());
    m_defines.insert(child->GetText());
#endif
  }
  else
  {
    tinyxml2::XMLElement const* child = element->FirstChildElement();
    assert(child && !child->FirstAttribute() && (strcmp(child->Value(), "name") == 0) && child->GetText());
    std::string text = trim(child->GetText());
    if (text == "VK_HEADER_VERSION")
    {
      m_version = trimEnd(element->LastChild()->ToText()->Value());
    }
    // ignore all the other defines
    assert(!child->NextSiblingElement() || (child->NextSiblingElement() && !child->NextSiblingElement()->FirstAttribute() && (strcmp(child->NextSiblingElement()->Value(), "type") == 0) && !child->NextSiblingElement()->NextSiblingElement()));
  }
}

void VulkanHppGenerator::readEnum(tinyxml2::XMLElement const* element, EnumData & enumData, bool bitmask, std::string const& prefix, std::string const& postfix)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "name",{} } }, { { "alias",{} },{ "bitpos",{} },{ "comment",{} },{ "value",{} } });
  assert((attributes.find("alias") != attributes.end()) + (attributes.find("bitpos") != attributes.end()) + (attributes.find("value") != attributes.end()) == 1);
  checkElements(getChildElements(element), {});

  std::string name = attributes.find("name")->second;
  std::string tag = findTag(m_tags, name, postfix);

  auto aliasIt = attributes.find("alias");
  if (aliasIt != attributes.end())
  {
    auto enumIt = std::find_if(enumData.values.begin(), enumData.values.end(), [&aliasIt](EnumValueData const& evd) { return evd.vulkanValue == aliasIt->second; });
    assert(enumIt != enumData.values.end());
    enumData.aliases.push_back(std::make_pair(name, createEnumValueName(name, prefix, postfix, bitmask, tag)));
  }
  else
  {
    enumData.addEnumValue(name, bitmask, attributes.find("bitpos") != attributes.end(), prefix, postfix, tag);
  }
}

void VulkanHppGenerator::readEnums(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "name",{} } }, { { "comment",{} },{ "type",{ "bitmask", "enum" } } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "comment", "enum", "unused" });

  std::string name = attributes.find("name")->second;

  if (name != "API Constants")
  {
    checkAttributes(attributes, element->GetLineNum(), { { "name",{} },{ "type",{ "bitmask", "enum" } } }, { { "comment",{} } });   // re-check with type as required

    // get the EnumData entry in enum map
    std::map<std::string, EnumData>::iterator it = m_enums.find(name);
    if (it == m_enums.end())
    {
      // well, some enums are not listed in the <types> section
      it = m_enums.insert(std::make_pair(name, EnumData())).first;
    }
    assert(it->second.values.empty());

    // mark it as a bitmap, if it is one
    std::string type = attributes.find("type")->second;
    bool bitmask = (type == "bitmask");
    it->second.isBitmask = bitmask;

    std::string prefix = getEnumPrefix(name, bitmask);
    std::string postfix = getEnumPostfix(name, m_tags, prefix);
    // read the names of the enum values
    for (auto child : children)
    {
      std::string value = child->Value();
      if (value == "enum")
      {
        readEnum(child, it->second, bitmask, prefix, postfix);
      }
#if !defined(NDEBUG)
      else
      {
        assert((value == "comment") || (value == "unused"));
      }
#endif
    }
  }
}

void VulkanHppGenerator::readExtension(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(),
  {
    { "name",{} },
    { "number",{} },
    { "supported",{ "disabled", "vulkan" } }
  },
  {
    { "author",{} },
    { "comment",{} },
    { "contact",{} },
    { "deprecatedby",{} },
    { "obsoletedby",{} },
    { "platform",{} },
    { "promotedto",{} },
    { "provisional",{} },
    { "requires",{} },
    { "requiresCore",{} },
    { "specialuse", {} },
    { "type",{ "device", "instance" } }
  });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "require" });

  if (attributes.find("supported")->second == "disabled")
  {
    readExtensionDisabled(children);
  }
  else
  {
    std::string name = attributes.find("name")->second;
    assert(beginsWith(name, "VK"));

    std::string tag = extractTag(name, m_tags);

    auto platformAttribute = attributes.find("platform");
    std::string platform = (platformAttribute != attributes.end()) ? platformAttribute->second : "";
    assert(platform.empty() || (m_platforms.find(platform) != m_platforms.end()));

    for (auto child : children)
    {
      assert(strcmp(child->Value(), "require") == 0);
      readExtensionRequire(child, platform, tag);
    }
  }
}

void VulkanHppGenerator::readExtensionDisabled(std::vector<tinyxml2::XMLElement const*> const& children)
{
  // kick out all the disabled stuff we've read before !!
  for (auto const& child : children)
  {
    assert(strcmp(child->Value(), "require") == 0);
    readExtensionDisabledRequire(child);
  }
}

void VulkanHppGenerator::readExtensionDisabledRequire(tinyxml2::XMLElement const* element)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), {}, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "command", "comment", "enum", "type" });

  for (auto child : children)
  {
    checkElements(getChildElements(child), {});

    std::string value = child->Value();
    if ((value == "command") || (value == "type"))
    {
      std::map<std::string, std::string> attributes = getAttributes(child);
      checkAttributes(attributes, element->GetLineNum(), { { "name",{} } }, {});

      // disable a command or a type !
      auto nameAttribute = attributes.find("name");
      std::string name = nameAttribute->second;
      std::string strippedName = (value == "command") ? startLowerCase(stripPrefix(name, "vk")) : stripPrefix(name, "Vk");

      if (value == "command")
      {
        // first unlink the command from its class
        unlinkCommandFromHandle(name);

        // then remove the command
        m_commandToHandle.erase(name);
      }
      else
      {
        // a type simply needs to be removed from the structs and vkTypes sets
        assert(m_structures.find(nameAttribute->second) != m_structures.end() ||
               m_bitmasks.find(nameAttribute->second) != m_bitmasks.end() ||
               m_enums.find(nameAttribute->second) != m_enums.end());
        m_structures.erase(nameAttribute->second);
        m_bitmasks.erase(nameAttribute->second);
        m_enums.erase(nameAttribute->second);
      }
    }
    else if (value == "enum")
    {
      std::map<std::string, std::string> attributes = getAttributes(child);
      checkAttributes(attributes, child->GetLineNum(), { { "name",{} } }, { { "bitpos",{} },{ "extends",{} },{ "offset",{} },{ "value",{} } });
    }
    else
    {
      assert(value == "comment");
    }
  }
}

void VulkanHppGenerator::readExtensionRequire(tinyxml2::XMLElement const* element, std::string const& platform, std::string const& tag)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), {}, { { "extension",{} },{ "feature",{} } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "command", "comment", "enum", "type" });

  for (auto child : children)
  {
    std::string value = child->Value();

    if (value == "command")
    {
      readExtensionRequireCommand(child, platform);
    }
    else if (value == "enum")
    {
      readRequireEnum(child, tag);
    }
    else if (value == "type")
    {
      readExtensionRequireType(child, platform);
    }
#if !defined(NDEBUG)
    else
    {
      assert(value == "comment");
      checkEmptyElement(child);
    }
#endif
  }
}

void VulkanHppGenerator::readExtensionRequireCommand(tinyxml2::XMLElement const* element, std::string const& platform)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "name",{} } }, {});
  checkElements(getChildElements(element), {});

  // just add the protect string to the CommandData
  if (!platform.empty())
  {
    assert(m_platforms.find(platform) != m_platforms.end());
    std::string name = attributes.find("name")->second;
    assert(m_commandToHandle.find(name) != m_commandToHandle.end());
    auto const& handlesIt = m_handles.find(m_commandToHandle.find(name)->second);
    assert(handlesIt != m_handles.end());
    auto const& commandsIt = handlesIt->second.commands.find(name);
    assert(commandsIt != handlesIt->second.commands.end());
    commandsIt->second.platform = platform;
  }
}

void VulkanHppGenerator::readExtensionRequireType(tinyxml2::XMLElement const* element, std::string const& platform)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "name",{} } }, {});
  checkElements(getChildElements(element), {});

  // add the protect-string to the appropriate type: enum, flag, handle, scalar, or struct
  std::string name = attributes.find("name")->second;

  auto handleIt = m_handles.find(name);
  if (handleIt != m_handles.end())
  {
    assert(beginsWith(name, "Vk"));
    auto objectTypeIt = m_enums.find("VkObjectType");
    assert(objectTypeIt != m_enums.end());
    std::string objectTypeName = "e" + stripPrefix(handleIt->first, "Vk");
    if (std::find_if(objectTypeIt->second.values.begin(), objectTypeIt->second.values.end(), [objectTypeName](EnumValueData const& evd) {return evd.vkValue == objectTypeName; }) == objectTypeIt->second.values.end())
    {
      throw std::runtime_error("Spec error on line " + std::to_string(element->GetLineNum()) + ": missing entry in VkObjectType enum for handle <" + name + ">.");
    }
  }

  if (!platform.empty())
  {
    auto bmit = m_bitmasks.find(name);
    if (bmit != m_bitmasks.end())
    {
      assert(bmit->second.platform.empty());
      bmit->second.platform = platform;
      assert((m_enums.find(bmit->second.requirement) == m_enums.end()) || (m_enums.find(bmit->second.requirement)->second.isBitmask));
    }
    else
    {
      auto eit = m_enums.find(name);
      if (eit != m_enums.end())
      {
        assert(eit->second.platform.empty());
        eit->second.platform = platform;
      }
      else
      {
        auto stit = m_structures.find(name);
        if (stit != m_structures.end())
        {
          assert(m_handles.find(name) == m_handles.end());
          assert(stit->second.platform.empty());
          stit->second.platform = platform;
        }
        else
        {
          assert((m_defines.find(name) != m_defines.end()));
        }
      }
    }
  }
}

void VulkanHppGenerator::readExtensions(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "comment",{} } }, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "extension" });

  for (auto child : children)
  {
    readExtension(child);
  }
}

void VulkanHppGenerator::readFeature(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "api",{ "vulkan" } },{ "comment",{} },{ "name",{} },{ "number",{} } }, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "require" });

  for (auto child : children)
  {
    readFeatureRequire(child);
  }
}

void VulkanHppGenerator::readFeatureRequire(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), {}, { { "comment",{} } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "command", "comment", "enum", "type" });

  for (auto child : children)
  {
    std::string value = child->Value();
    if (value == "enum")
    {
      readRequireEnum(child, "");
    }
  }
}

void VulkanHppGenerator::readFuncpointer(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { { "category",{ "funcpointer" } } }, { { "requires",{} } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "name", "type" });
  assert(!children.empty());
  checkEmptyElement(children[0]);

  assert((strcmp(children[0]->Value(), "name") == 0) && children[0]->GetText());
#if !defined(NDEBUG)
  for (size_t i = 1; i < children.size(); i++)
  {
    checkEmptyElement(children[i]);
  }
#endif
}

void VulkanHppGenerator::readHandle(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { { "category",{ "handle" } } }, { { "alias",{} },{ "name",{} },{ "parent",{} } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);

  auto aliasIt = attributes.find("alias");
  if (aliasIt != attributes.end())
  {
    checkAttributes(attributes, element->GetLineNum(), { { "alias",{} },{ "category",{ "handle" } },{ "name",{} } }, {});   // re-check on alias type!
    checkElements(children, {});
    checkAlias(m_handles, aliasIt->second, element->GetLineNum());

    auto handlesIt = m_handles.find(aliasIt->second);
    assert((handlesIt != m_handles.end()) && handlesIt->second.alias.empty());
    handlesIt->second.alias = attributes.find("name")->second;
  }
  else
  {
    checkOrderedElements(children, { "type", "name" });
    checkEmptyElement(children[0]);
    checkEmptyElement(children[1]);

#if !defined(NDEBUG)
    std::string type = children[0]->GetText();
    assert((type.find("VK_DEFINE_HANDLE") == 0) || (type.find("VK_DEFINE_NON_DISPATCHABLE_HANDLE") == 0));
#endif

    std::string name = children[1]->GetText();

    assert(m_handles.find(name) == m_handles.end());
    m_handles.insert(std::make_pair(name, HandleData()));
  }
}

void VulkanHppGenerator::readPlatform(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "comment",{} },{ "name",{} },{ "protect",{} } }, {});
  checkElements(getChildElements(element), {});

  std::string name, protect;
  for (auto const& attribute : attributes)
  {
    if (attribute.first == "name")
    {
      name = attribute.second;
    }
    else if (attribute.first == "protect")
    {
      protect = attribute.second;
    }
    else
    {
      assert(attribute.first == "comment");
    }
  }
  assert(!name.empty() && !protect.empty());
  assert(m_platforms.find(name) == m_platforms.end());
  m_platforms[name] = protect;
}

void VulkanHppGenerator::readPlatforms(tinyxml2::XMLElement const* element)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), { { "comment",{} } }, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "platform" });

  // init m_platforms with "" -> ""
  assert(m_platforms.empty());
  m_platforms[""] = "";

  for (auto child : children)
  {
    readPlatform(child);
  }
}

void VulkanHppGenerator::readRequireEnum(tinyxml2::XMLElement const* element, std::string const& tag)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(),
  {
    { "name",{} }
  },
  {
    { "alias",{} },
    { "bitpos",{} },
    { "comment",{} },
    { "dir",{ "-" } },
    { "extends",{} },
    { "extnumber",{} },
    { "offset",{} },
    { "value",{} }
  });
  checkElements(getChildElements(element), {});

  // TODO process enums which don't extend existing enums
  auto extendsIt = attributes.find("extends");
  if (extendsIt != attributes.end())
  {
    std::string extends = extendsIt->second;
    auto enumIt = m_enums.find(extends);
    assert(enumIt != m_enums.end());

    std::string prefix = getEnumPrefix(enumIt->first, enumIt->second.isBitmask);
    std::string postfix = getEnumPostfix(enumIt->first, m_tags, prefix);

    auto nameIt = attributes.find("name");
    assert(nameIt != attributes.end());

    auto aliasIt = attributes.find("alias");
    if (aliasIt != attributes.end())
    {
      // add this enum name to the list of aliases
      checkAttributes(attributes, element->GetLineNum(), { { "alias",{} },{ "extends",{} },{ "name",{} } }, { { "comment",{} } });
      std::string valueName = createEnumValueName(nameIt->second, prefix, postfix, enumIt->second.isBitmask, tag);
      if (!enumIt->second.alias.empty())
      {
        prefix = getEnumPrefix(enumIt->second.alias, enumIt->second.isBitmask);
        postfix = getEnumPostfix(enumIt->second.alias, m_tags, prefix);
        if (endsWith(nameIt->second, postfix))
        {
          valueName = createEnumValueName(nameIt->second, prefix, postfix, enumIt->second.isBitmask, tag);
        }
      }
      assert(std::find_if(enumIt->second.aliases.begin(), enumIt->second.aliases.end(), [&valueName](std::pair<std::string, std::string> const& aliasPair) { return valueName == aliasPair.second; }) == enumIt->second.aliases.end());
      if (std::find_if(enumIt->second.values.begin(), enumIt->second.values.end(), [&valueName](EnumValueData const& evd) { return evd.vkValue == valueName; }) == enumIt->second.values.end())
      {
        enumIt->second.aliases.push_back(std::make_pair(nameIt->second, valueName));
      }
    }
    else
    {
      // add this enum name to the list of values
      assert((attributes.find("bitpos") != attributes.end()) + (attributes.find("offset") != attributes.end()) + (attributes.find("value") != attributes.end()) == 1);
      enumIt->second.addEnumValue(nameIt->second, enumIt->second.isBitmask, attributes.find("bitpos") != attributes.end(), prefix, postfix, tag);
    }
  }
}

#if !defined(NDEBUG)
void VulkanHppGenerator::readRequires(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { {"name", {}}, { "requires", {}} }, {});
  checkElements(getChildElements(element), {});

  auto nameIt = attributes.find("name");
  assert(nameIt != attributes.end());
}
#endif

void VulkanHppGenerator::readStruct(tinyxml2::XMLElement const* element, bool isUnion, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(),
  {
    { "category",{ isUnion ? "union" : "struct" } },
    { "name",{} }
  },
  {
    { "alias",{} },
    { "comment",{} },
    { "returnedonly",{ "true" } },
    { "structextends",{} }
  });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "comment", "member" });

  std::string name = attributes.find("name")->second;

  auto aliasIt = attributes.find("alias");
  if (aliasIt != attributes.end())
  {
    readStructAlias(element->GetLineNum(), name, aliasIt->second, attributes);
  }
  else
  {
    assert(m_structures.find(name) == m_structures.end());
    std::map<std::string, StructureData>::iterator it = m_structures.insert(std::make_pair(name, StructureData())).first;
    it->second.returnedOnly = (attributes.find("returnedonly") != attributes.end());
    it->second.isUnion = isUnion;

    readStructStructExtends(attributes, it->second.structExtends, m_extendedStructs);
    it->second.members = readStructMembers(children);
    it->second.subStruct = determineSubStruct(*it);
  }
}

void VulkanHppGenerator::readStructAlias(int lineNum, std::string const& name, std::string const& alias, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, lineNum, { { "alias",{} },{ "category",{ "struct" } },{ "name",{} } }, {});    // re-check on alias type!
  checkAlias(m_structures, alias, lineNum);

  auto structsIt = m_structures.find(alias);
  assert((structsIt != m_structures.end()) && (std::find(structsIt->second.aliases.begin(), structsIt->second.aliases.end(), name) == structsIt->second.aliases.end()));
  structsIt->second.aliases.push_back(name);

  assert(m_structureAliases.find(name) == m_structureAliases.end());
  m_structureAliases[name] = alias;
}

VulkanHppGenerator::MemberData VulkanHppGenerator::readStructMember(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), {},
  {
    { "altlen",{} },
    { "externsync",{ "true" } },
    { "len",{} },
    { "noautovalidity",{ "true" } },
    { "optional",{ "false", "true" } },
    { "values",{} }
  });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "comment", "enum", "name", "type" });

  MemberData memberData;
  auto valuesIt = attributes.find("values");
  if (valuesIt != attributes.end())
  {
    memberData.values = valuesIt->second;
  }

  for (auto child : children)
  {
    checkEmptyElement(child);
    assert(child->Value());
    std::string value = child->Value();
    assert(child->GetText());
    if (value == "enum")
    {
      assert(child->PreviousSibling() && (strcmp(child->PreviousSibling()->Value(), "[") == 0)
        && child->NextSibling() && (strcmp(child->NextSibling()->Value(), "]") == 0));
      memberData.arraySizes.push_back(child->GetText());
    }
    else if (value == "name")
    {
      memberData.name = child->GetText();
      std::tie(memberData.arraySizes, memberData.bitCount) = readModifiers(child->NextSibling());
    }
    else if (value == "type")
    {
      memberData.type.prefix = readTypePrefix(child->PreviousSibling());
      memberData.type.type = child->GetText();
      memberData.type.postfix = readTypePostfix(child->NextSibling());
    }
  }

  return memberData;
}

std::vector<VulkanHppGenerator::MemberData> VulkanHppGenerator::readStructMembers(std::vector<tinyxml2::XMLElement const*> const& children)
{
  std::vector<MemberData> members;
  for (auto child : children)
  {
    assert(child->Value());
    std::string value = child->Value();
    if (value == "member")
    {
      members.push_back(readStructMember(child));
    }
#if !defined(NDEBUG)
    else
    {
      assert(value == "comment");
      checkEmptyElement(child);
    }
#endif
  }
  return members;
}

void VulkanHppGenerator::readTags(tinyxml2::XMLElement const* element)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), { { "comment",{} } }, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "tag" });

  for (auto child : children)
  {
    readTag(child);
  }
}

void VulkanHppGenerator::readTag(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "author",{} },{ "contact",{} },{ "name",{} } }, {});
  checkElements(getChildElements(element), {});

  for (auto const& attribute : attributes)
  {
    std::string name = attribute.first;
    if (name == "name")
    {
      std::string value = attribute.second;
      assert(m_tags.find(value) == m_tags.end());
      m_tags.insert(value);
    }
    else
    {
      assert((name == "author") || (name == "contact"));
    }
  }
}

void VulkanHppGenerator::readType(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);

  auto categoryIt = attributes.find("category");
  if (categoryIt != attributes.end())
  {
    if (categoryIt->second == "basetype")
    {
      readBaseType(element, attributes);
    }
    else if (categoryIt->second == "bitmask")
    {
      readBitmask(element, attributes);
    }
    else if (categoryIt->second == "define")
    {
      readDefine(element, attributes);
    }
    else if (categoryIt->second == "enum")
    {
      readTypeEnum(element, attributes);
    }
    else if (categoryIt->second == "funcpointer")
    {
      readFuncpointer(element, attributes);
    }
    else if (categoryIt->second == "handle")
    {
      readHandle(element, attributes);
    }
    else if (categoryIt->second == "struct")
    {
      readStruct(element, false, attributes);
    }
    else if (categoryIt->second == "union")
    {
      readStruct(element, true, attributes);
    }
    else if ( categoryIt->second != "include")
    {
      throw std::runtime_error("Spec error on line " + std::to_string(element->GetLineNum()) + ": unknown category <" + categoryIt->second + ">");
    }
  }
#if !defined(NDEBUG)
  else
  {
    auto requiresIt = attributes.find("requires");
    if (requiresIt != attributes.end())
    {
      readRequires(element, attributes);
    }
    else
    {
      assert((attributes.size() == 1) && (attributes.begin()->first == "name") && (attributes.begin()->second == "int"));
    }
  }
#endif
}

void VulkanHppGenerator::readTypeEnum(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { { "category",{ "enum" } }, { "name",{} } }, { { "alias",{} }});

  std::string name = attributes.find("name")->second;

  auto aliasIt = attributes.find("alias");
  if ((aliasIt != attributes.end()))
  {
    auto enumIt = m_enums.find(aliasIt->second);
    assert(enumIt != m_enums.end());
    assert(enumIt->second.alias.empty());
    enumIt->second.alias = name;
  }
  else
  {
    assert(m_enums.find(name) == m_enums.end());
    m_enums[name];    // insert an empty EnumData
  }
}

void VulkanHppGenerator::readTypes(tinyxml2::XMLElement const* element)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), { { "comment",{} } }, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "comment", "type" });

  for (auto child : children)
  {
    std::string value = child->Value();
    if (value == "type")
    {
      readType(child);
    }
#if !defined(NDEBUG)
    else
    {
      assert(value == "comment");
      checkEmptyElement(child);
    }
#endif
  }
}

void VulkanHppGenerator::registerDeleter(std::string const& name, std::pair<std::string, CommandData> const& commandData)
{
  if ((commandData.first.substr(2, 7) == "Destroy") || (commandData.first.substr(2, 4) == "Free"))
  {
    std::string key;
    size_t valueIndex;
    switch (commandData.second.params.size())
    {
    case 2:
    case 3:
      assert(commandData.second.params.back().type.type == "VkAllocationCallbacks");
      key = (commandData.second.params.size() == 2) ? "" : commandData.second.params[0].type.type;
      valueIndex = commandData.second.params.size() - 2;
      break;
    case 4:
      key = commandData.second.params[0].type.type;
      valueIndex = 3;
      assert(m_handles.find(commandData.second.params[valueIndex].type.type) != m_handles.end());
      m_handles.find(commandData.second.params[valueIndex].type.type)->second.deletePool = commandData.second.params[1].type.type;
      break;
    default:
      assert(false);
      valueIndex = 0;
    }
    auto keyHandleIt = m_handles.find(key);
    assert((keyHandleIt != m_handles.end()) && (keyHandleIt->second.childrenHandles.find(commandData.second.params[valueIndex].type.type) == keyHandleIt->second.childrenHandles.end()));
    keyHandleIt->second.childrenHandles.insert(commandData.second.params[valueIndex].type.type);

    auto handleIt = m_handles.find(commandData.second.params[valueIndex].type.type);
    assert(handleIt != m_handles.end());
    handleIt->second.deleteCommand = name;
  }
}

void VulkanHppGenerator::unlinkCommandFromHandle(std::string const& name)
{
  assert(m_commandToHandle.find(name) != m_commandToHandle.end());
  auto handlesIt = m_handles.find(m_commandToHandle.find(name)->second);
  assert(handlesIt != m_handles.end());
  auto it = handlesIt->second.commands.find(name);
  assert(it != handlesIt->second.commands.end());
  handlesIt->second.commands.erase(it);
}

std::string VulkanHppGenerator::TypeData::compose() const
{
  return prefix + (prefix.empty() ? "" : " ") + ((type.substr(0, 2) == "Vk") ? "VULKAN_HPP_NAMESPACE::" : "") + stripPrefix(type, "Vk") + postfix;
}

int main(int argc, char **argv)
{
  static const std::string classArrayProxy = R"(
#if !defined(VULKAN_HPP_DISABLE_ENHANCED_MODE)
  template <typename T>
  class ArrayProxy
  {
  public:
    VULKAN_HPP_CONSTEXPR ArrayProxy(std::nullptr_t) VULKAN_HPP_NOEXCEPT
      : m_count(0)
      , m_ptr(nullptr)
    {}

    ArrayProxy(typename std::remove_reference<T>::type & ptr) VULKAN_HPP_NOEXCEPT
      : m_count(1)
      , m_ptr(&ptr)
    {}

    ArrayProxy(uint32_t count, T * ptr) VULKAN_HPP_NOEXCEPT
      : m_count(count)
      , m_ptr(ptr)
    {}

    template <size_t N>
    ArrayProxy(std::array<typename std::remove_const<T>::type, N> & data) VULKAN_HPP_NOEXCEPT
      : m_count(N)
      , m_ptr(data.data())
    {}

    template <size_t N>
    ArrayProxy(std::array<typename std::remove_const<T>::type, N> const& data) VULKAN_HPP_NOEXCEPT
      : m_count(N)
      , m_ptr(data.data())
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxy(std::vector<typename std::remove_const<T>::type, Allocator> & data) VULKAN_HPP_NOEXCEPT
      : m_count(static_cast<uint32_t>(data.size()))
      , m_ptr(data.data())
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxy(std::vector<typename std::remove_const<T>::type, Allocator> const& data) VULKAN_HPP_NOEXCEPT
      : m_count(static_cast<uint32_t>(data.size()))
      , m_ptr(data.data())
    {}

    ArrayProxy(std::initializer_list<typename std::remove_reference<T>::type> const& data) VULKAN_HPP_NOEXCEPT
      : m_count(static_cast<uint32_t>(data.end() - data.begin()))
      , m_ptr(data.begin())
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
      VULKAN_HPP_ASSERT(m_count && m_ptr);
      return *m_ptr;
    }

    const T & back() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT(m_count && m_ptr);
      return *(m_ptr + m_count - 1);
    }

    bool empty() const VULKAN_HPP_NOEXCEPT
    {
      return (m_count == 0);
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
    uint32_t  m_count;
    T *       m_ptr;
  };
#endif
)";

  static const std::string classFlags = R"(
  template <typename FlagBitsType> struct FlagTraits
  {
    enum { allFlags = 0 };
  };

  template <typename BitType, typename MaskType = VkFlags>
  class Flags
  {
  public:
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
    Flags<BitType> & operator=(Flags<BitType> const& rhs) VULKAN_HPP_NOEXCEPT
    {
      m_mask = rhs.m_mask;
      return *this;
    }

    Flags<BitType> & operator|=(Flags<BitType> const& rhs) VULKAN_HPP_NOEXCEPT
    {
      m_mask |= rhs.m_mask;
      return *this;
    }

    Flags<BitType> & operator&=(Flags<BitType> const& rhs) VULKAN_HPP_NOEXCEPT
    {
      m_mask &= rhs.m_mask;
      return *this;
    }

    Flags<BitType> & operator^=(Flags<BitType> const& rhs) VULKAN_HPP_NOEXCEPT
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

  // relational operators
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
        assert( m_owner && m_dispatch );
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
        assert( m_dispatch );
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
      ObjectFree()
        : m_owner()
        , m_allocationCallbacks( nullptr )
        , m_dispatch( nullptr )
      {}

      ObjectFree( OwnerType owner, Optional<const AllocationCallbacks> allocationCallbacks, Dispatch const &dispatch ) VULKAN_HPP_NOEXCEPT
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
        assert( m_owner && m_dispatch );
        m_owner.free( t, m_allocationCallbacks, *m_dispatch );
      }

    private:
      OwnerType m_owner;
      Optional<const AllocationCallbacks> m_allocationCallbacks;
      Dispatch const* m_dispatch;
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
  template <typename X, typename Y> struct isStructureChainValid { enum { value = false }; };

  template <typename P, typename T>
  struct TypeList
  {
    using list = P;
    using last = T;
  };

  template <typename List, typename X>
  struct extendCheck
  {
    static const bool valid = isStructureChainValid<typename List::last, X>::value || extendCheck<typename List::list,X>::valid;
  };

  template <typename T, typename X>
  struct extendCheck<TypeList<void,T>,X>
  {
    static const bool valid = isStructureChainValid<T, X>::value;
  };

  template <typename X>
  struct extendCheck<void,X>
  {
    static const bool valid = true;
  };

  template<typename Type, class...>
  struct isPartOfStructureChain
  {
    static const bool valid = false;
  };

  template<typename Type, typename Head, typename... Tail>
  struct isPartOfStructureChain<Type, Head, Tail...>
  {
    static const bool valid = std::is_same<Type, Head>::value || isPartOfStructureChain<Type, Tail...>::valid;
  };

  template <class Element>
  class StructureChainElement
  {
  public:
    explicit operator Element&() VULKAN_HPP_NOEXCEPT { return value; }
    explicit operator const Element&() const VULKAN_HPP_NOEXCEPT { return value; }
  private:
    Element value;
  };

  template<typename ...StructureElements>
  class StructureChain : private StructureChainElement<StructureElements>...
  {
  public:
    StructureChain() VULKAN_HPP_NOEXCEPT
    {
      link<void, StructureElements...>();
    }

    StructureChain(StructureChain const &rhs) VULKAN_HPP_NOEXCEPT
    {
      linkAndCopy<void, StructureElements...>(rhs);
    }

    StructureChain(StructureElements const &... elems) VULKAN_HPP_NOEXCEPT
    {
      linkAndCopyElements<void, StructureElements...>(elems...);
    }

    StructureChain& operator=(StructureChain const &rhs) VULKAN_HPP_NOEXCEPT
    {
      linkAndCopy<void, StructureElements...>(rhs);
      return *this;
    }

    template<typename ClassType> ClassType& get() VULKAN_HPP_NOEXCEPT { return static_cast<ClassType&>(*this);}

    template<typename ClassTypeA, typename ClassTypeB, typename ...ClassTypes>
    std::tuple<ClassTypeA, ClassTypeB, ClassTypes...> get()
    {
        return std::tuple_cat(
            std::make_tuple(get<ClassTypeA>(),get<ClassTypeB>()),
            std::make_tuple(get<ClassTypes>()...)
        );
    }

    template<typename ClassType>
    void unlink() VULKAN_HPP_NOEXCEPT
    {
      static_assert(isPartOfStructureChain<ClassType, StructureElements...>::valid, "Can't unlink Structure that's not part of this StructureChain!");
      static_assert(!std::is_same<ClassType, typename std::tuple_element<0, std::tuple<StructureElements...>>::type>::value, "It's not allowed to unlink the first element!");
      VkBaseOutStructure * ptr = reinterpret_cast<VkBaseOutStructure*>(&get<ClassType>());
      assert(ptr != nullptr);
      VkBaseOutStructure ** ppNext = &(reinterpret_cast<VkBaseOutStructure*>(this)->pNext);
      assert(*ppNext != nullptr);
      while (*ppNext != ptr)
      {
        ppNext = &(*ppNext)->pNext;
        assert(*ppNext != nullptr);   // fires, if the ClassType member has already been unlinked !
      }
      assert(*ppNext == ptr);
      *ppNext = (*ppNext)->pNext;
    }

    template <typename ClassType>
    void relink() VULKAN_HPP_NOEXCEPT
    {
      static_assert(isPartOfStructureChain<ClassType, StructureElements...>::valid, "Can't relink Structure that's not part of this StructureChain!");
      static_assert(!std::is_same<ClassType, typename std::tuple_element<0, std::tuple<StructureElements...>>::type>::value, "It's not allowed to have the first element unlinked!");
      VkBaseOutStructure * ptr = reinterpret_cast<VkBaseOutStructure*>(&get<ClassType>());
      assert(ptr != nullptr);
      VkBaseOutStructure ** ppNext = &(reinterpret_cast<VkBaseOutStructure*>(this)->pNext);
      assert(*ppNext != nullptr);
#if !defined(NDEBUG)
      while (*ppNext)
      {
        assert(*ppNext != ptr);   // fires, if the ClassType member has not been unlinked before
        ppNext = &(*ppNext)->pNext;
      }
      ppNext = &(reinterpret_cast<VkBaseOutStructure*>(this)->pNext);
#endif
      ptr->pNext = *ppNext;
      *ppNext = ptr;
    }

  private:
    template<typename List, typename X>
    void link() VULKAN_HPP_NOEXCEPT
    {
      static_assert(extendCheck<List, X>::valid, "The structure chain is not valid!");
    }

    template<typename List, typename X, typename Y, typename ...Z>
    void link() VULKAN_HPP_NOEXCEPT
    {
      static_assert(extendCheck<List,X>::valid, "The structure chain is not valid!");
      X& x = static_cast<X&>(*this);
      Y& y = static_cast<Y&>(*this);
      x.pNext = &y;
      link<TypeList<List, X>, Y, Z...>();
    }

    template<typename List, typename X>
    void linkAndCopy(StructureChain const &rhs) VULKAN_HPP_NOEXCEPT
    {
      static_assert(extendCheck<List, X>::valid, "The structure chain is not valid!");
      static_cast<X&>(*this) = static_cast<X const &>(rhs);
    }

    template<typename List, typename X, typename Y, typename ...Z>
    void linkAndCopy(StructureChain const &rhs) VULKAN_HPP_NOEXCEPT
    {
      static_assert(extendCheck<List, X>::valid, "The structure chain is not valid!");
      X& x = static_cast<X&>(*this);
      Y& y = static_cast<Y&>(*this);
      x = static_cast<X const &>(rhs);
      x.pNext = &y;
      linkAndCopy<TypeList<List, X>, Y, Z...>(rhs);
    }

    template<typename List, typename X>
    void linkAndCopyElements(X const &xelem) VULKAN_HPP_NOEXCEPT
    {
      static_assert(extendCheck<List, X>::valid, "The structure chain is not valid!");
      static_cast<X&>(*this) = xelem;
    }

    template<typename List, typename X, typename Y, typename ...Z>
    void linkAndCopyElements(X const &xelem, Y const &yelem, Z const &... zelem) VULKAN_HPP_NOEXCEPT
    {
      static_assert(extendCheck<List, X>::valid, "The structure chain is not valid!");
      X& x = static_cast<X&>(*this);
      Y& y = static_cast<Y&>(*this);
      x = xelem;
      x.pNext = &y;
      linkAndCopyElements<TypeList<List, X>, Y, Z...>(yelem, zelem...);
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

static const std::string constExpressionArrayCopy = R"(
  template <typename T, size_t N, size_t I>
  class ConstExpression1DArrayCopy
  {
    public:
      VULKAN_HPP_CONSTEXPR_14 static void copy(T dst[N], std::array<T,N> const& src) VULKAN_HPP_NOEXCEPT
      {
        dst[I-1] = src[I-1];
        ConstExpression1DArrayCopy<T, N, I - 1>::copy(dst, src);
      }
  };

  template <typename T, size_t N>
  class ConstExpression1DArrayCopy<T, N, 0>
  {
    public:
      VULKAN_HPP_CONSTEXPR_14 static void copy(T /*dst*/[N], std::array<T,N> const& /*src*/) VULKAN_HPP_NOEXCEPT {}
  };

  template <typename T, size_t N, size_t M, size_t I, size_t J>
  class ConstExpression2DArrayCopy
  {
  public:
    VULKAN_HPP_CONSTEXPR_14 static void copy(T dst[N][M], std::array<std::array<T,M>, N> const& src) VULKAN_HPP_NOEXCEPT
    {
      dst[I - 1][J - 1] = src[I - 1][J - 1];
      ConstExpression2DArrayCopy<T, N, M, I, J - 1>::copy(dst, src);
    }
  };

  template <typename T, size_t N, size_t M, size_t I>
  class ConstExpression2DArrayCopy<T, N, M, I, 0>
  {
  public:
    VULKAN_HPP_CONSTEXPR_14 static void copy(T dst[N][M], std::array<std::array<T, M>, N> const& src) VULKAN_HPP_NOEXCEPT
    {
      ConstExpression2DArrayCopy<T, N, M, I - 1, M>::copy(dst, src);
    }
  };

  template <typename T, size_t N, size_t M>
  class ConstExpression2DArrayCopy<T, N, M, 0, 0>
  {
  public:
    VULKAN_HPP_CONSTEXPR_14 static void copy(T /*dst*/[N][M], std::array<std::array<T, M>, N> const& /*src*/) VULKAN_HPP_NOEXCEPT {}
  };
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

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <string>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <vulkan/vulkan.h>

#if !defined(VULKAN_HPP_DISABLE_ENHANCED_MODE)
# include <memory>
# include <vector>
#endif

#if !defined(VULKAN_HPP_ASSERT)
# include <cassert>
# define VULKAN_HPP_ASSERT   assert
#endif

#if !defined(VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL)
# define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 1
#endif

#if VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL == 1
#  if defined(__linux__) || defined(__APPLE__)
#   include <dlfcn.h>
#  endif

#  if defined(_WIN32)
#   include <windows.h>
#  endif
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
  };

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
    VULKAN_HPP_ASSERT( result == Result::eSuccess );
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
    VULKAN_HPP_ASSERT( result == Result::eSuccess );
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
    VULKAN_HPP_ASSERT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
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
    VULKAN_HPP_ASSERT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
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
    VULKAN_HPP_ASSERT( result == Result::eSuccess );
    return ResultValue<UniqueHandle<T,D>>( result, UniqueHandle<T,D>(data, deleter) );
#else
    if ( result != Result::eSuccess )
    {
      throwResultException( result, message );
    }
    return UniqueHandle<T,D>(data, deleter);
#endif
  }
#endif
)";

  static const std::string typeTraits = R"(
  template<ObjectType value>
  struct cpp_type
  {
  };
)";

  try {
    tinyxml2::XMLDocument doc;

    std::string filename = (argc == 1) ? VK_SPEC : argv[1];
    std::cout << "Loading vk.xml from " << filename << std::endl;
    std::cout << "Writing vulkan.hpp to " << VULKAN_HPP_FILE << std::endl;

    tinyxml2::XMLError error = doc.LoadFile(filename.c_str());
    if (error != tinyxml2::XML_SUCCESS)
    {
      std::cout << "VkGenerate: failed to load file " << filename << " . Error code: " << error << std::endl;
      return -1;
    }

    VulkanHppGenerator generator;

    tinyxml2::XMLElement const* registryElement = doc.FirstChildElement();
    checkAttributes(getAttributes(registryElement), registryElement->GetLineNum(), {}, {});
    assert(strcmp(registryElement->Value(), "registry") == 0);
    assert(!registryElement->NextSiblingElement());

    std::vector<tinyxml2::XMLElement const*> children = getChildElements(registryElement);
    checkElements(children, { "commands", "comment", "enums", "extensions", "feature", "tags", "types", "vendorids", "platforms" });
    for (auto child : children)
    {
      const std::string value = child->Value();
      if (value == "commands")
      {
        generator.readCommands(child);
      }
      else if (value == "comment")
      {
        generator.readComment(child);
      }
      else if (value == "enums")
      {
        generator.readEnums(child);
      }
      else if (value == "extensions")
      {
        generator.readExtensions(child);
      }
      else if (value == "feature")
      {
        generator.readFeature(child);
      }
      else if (value == "platforms")
      {
        generator.readPlatforms(child);
      }
      else if (value == "tags")
      {
        generator.readTags(child);
      }
      else if (value == "types")
      {
        generator.readTypes(child);
      }
      else
      {
        std::cerr << "warning: Unhandled tag " << value << " at line number: " << std::to_string(child->GetLineNum()) << "!" << std::endl;
      }
    }

    generator.checkCorrectness();

    std::string str;
    static const size_t estimatedLength = 4 * 1024 * 1024;
    str.reserve(estimatedLength);
    str += generator.getVulkanLicenseHeader()
      + includes
      + "\n";
    appendVersionCheck(str, generator.getVersion());
    appendTypesafeStuff(str, generator.getTypesafeCheck());
    str += defines
      + "\n"
      + "namespace VULKAN_HPP_NAMESPACE\n"
      + "{\n"
      + classArrayProxy
      + classFlags
      + classOptional
      + classStructureChain
      + classUniqueHandle;
    generator.appendDispatchLoaderStatic(str);
    generator.appendDispatchLoaderDefault(str);
    str += classObjectDestroy
      + classObjectFree
      + classPoolFree
      + constExpressionArrayCopy
      + "\n";
    generator.appendBaseTypes(str);
    generator.appendEnums(str);
    str += typeTraits;
    generator.appendBitmasks(str);
    str += "} // namespace VULKAN_HPP_NAMESPACE\n"
      + is_error_code_enum
      + "\n"
      + "namespace VULKAN_HPP_NAMESPACE\n"
      + "{\n"
      + "#ifndef VULKAN_HPP_NO_EXCEPTIONS"
      + exceptions;
    generator.appendResultExceptions(str);
    generator.appendThrowExceptions(str);
    str += "#endif\n"
      + structResultValue;
    generator.appendForwardDeclarations(str);
    generator.appendHandles(str);
    generator.appendStructs(str);
    generator.appendHandlesCommandDefintions(str);
    generator.appendStructureChainValidation(str);
    generator.appendDispatchLoaderDynamic(str);
    str += "} // namespace VULKAN_HPP_NAMESPACE\n"
      "#endif\n";

    assert(str.length() < estimatedLength);
    cleanup(str);

    std::ofstream ofs(VULKAN_HPP_FILE);
    assert(!ofs.fail());
    ofs << str;
    ofs.close();
  }
  catch (std::exception const& e)
  {
    std::cout << "caught exception: " << e.what() << std::endl;
    return -1;
  }
  catch (...)
  {
    std::cout << "caught unknown exception" << std::endl;
    return -1;
  }
}
