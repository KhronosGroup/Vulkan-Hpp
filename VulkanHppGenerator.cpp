// Copyright(c) 2015-2019, NVIDIA CORPORATION. All rights reserved.
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
#include <sstream>
#include <exception>
#include <regex>
#include <iterator>
#include "VulkanHppGenerator.hpp"

bool beginsWith(std::string const& text, std::string const& prefix);
bool endsWith(std::string const& text, std::string const& postfix);
template <typename T> void checkAlias(std::map<std::string, T> const& data, std::string const& name, int line);
void checkAttributes(std::map<std::string, std::string> const& attributes, int line, std::map<std::string, std::set<std::string>> const& required, std::map<std::string, std::set<std::string>> const& optional);
void checkElements(std::vector<tinyxml2::XMLElement const*> const& elements, std::set<std::string> const& values);
void checkEmptyElement(tinyxml2::XMLElement const* element);
void checkOrderedElements(std::vector<tinyxml2::XMLElement const*> const& elements, std::vector<std::string> const& values);
std::string createEnumValueName(std::string const& name, std::string const& prefix, std::string const& postfix, bool bitmask, std::string const& tag);
std::string determineCommandName(std::string const& vulkanCommandName, std::string const& firstArgumentType);
std::set<size_t> determineSkippedParams(size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices);
bool determineStructureChaining(std::string const& structType, std::set<std::string> const& extendedStructs, std::map<std::string, std::string> const& structureAliases);
std::string findTag(std::set<std::string> const& tags, std::string const& name, std::string const& postfix = "");
std::map<std::string, std::string> getAttributes(tinyxml2::XMLElement const* element);
std::vector<tinyxml2::XMLElement const*> getChildElements(tinyxml2::XMLElement const* element);
std::string getEnumPostfix(std::string const& name, std::set<std::string> const& tags, std::string & prefix);
std::string getEnumPrefix(std::string const& name, bool bitmask);
std::string readArraySize(tinyxml2::XMLNode const* node);
void readStructStructExtends(std::map<std::string, std::string> const& attributes, std::vector<std::string> & structExtends, std::set<std::string> & extendedStructs);
std::string readTypePostfix(tinyxml2::XMLNode const* node);
std::string readTypePrefix(tinyxml2::XMLNode const* node);
std::string replaceWithMap(std::string const &input, std::map<std::string, std::string> replacements);
std::string startLowerCase(std::string const& input);
std::string startUpperCase(std::string const& input);
std::string stripPostfix(std::string const& value, std::string const& postfix);
std::string stripPluralS(std::string const& name);
std::string stripPrefix(std::string const& value, std::string const& prefix);
std::string writeReinterpretCast(bool leadingConst, std::string const& type, bool trailingPointerToConst);
std::string toCamelCase(std::string const& value);
std::string toUpperCase(std::string const& name);
std::vector<std::string> tokenize(std::string tokenString, char separator);
std::string trim(std::string const& input);
std::string trimEnd(std::string const& input);
void writeArgumentCount(std::ostream & os, size_t vectorIndex, std::string const& vectorName, std::string const& counterName, size_t returnParamIndex, size_t templateParamIndex, bool twoStep, bool singular);
void writeBitmask(std::ostream & os, std::string const& bitmaskName, std::string const& bitmaskAlias, std::string const& enumName, std::vector<std::pair<std::string, std::string>> const& enumValues);
void writeBitmaskToStringFunction(std::ostream & os, std::string const& flagsName, std::string const& enumName, std::vector<std::pair<std::string, std::string>> const& enumValues);
std::string writeFunctionBodyEnhancedLocalReturnVariableSingular(std::ostream & os, std::string const& indentation, std::string const& typeName, std::string const&returnName, bool isStructureChain);
std::pair<bool, std::string> writeFunctionBodyStandardReturn(std::string const& returnType);

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
    std::stringstream ss;
    ss << line;
    std::string lineNumber = ss.str();

    throw std::runtime_error("Spec error on line " + lineNumber + ": missing alias <" + name + ">");
  }
}

// check the validity of an attributes map
// attributes : the map of name/value pairs of the encountered attributes
// line       : the line in the xml file where the attributes are listed
// required   : the required attributes, with a set of allowed values per attribute
// optional   : the optional attributes, with a set of allowed values per attribute
void checkAttributes(std::map<std::string, std::string> const& attributes, int line, std::map<std::string, std::set<std::string>> const& required, std::map<std::string, std::set<std::string>> const& optional)
{
  std::stringstream ss;
  ss << line;
  std::string lineNumber = ss.str();

  // check if all required attributes are included and if there is a set of allowed values, check if the actual value is part of that set
  for (auto const& r : required)
  {
    auto attributesIt = attributes.find(r.first);
    if (attributesIt == attributes.end())
    {
      throw std::runtime_error("Spec error on line " + lineNumber + ": missing attribute <" + r.first + ">");
    }
    if (!r.second.empty() && (r.second.find(attributesIt->second) == r.second.end()))
    {
      throw std::runtime_error("Spec error on line " + lineNumber + ": unexpected attribute value <" + attributesIt->second + "> in attribute <" + r.first + ">");
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
        std::cerr << "warning: " << "Unknown attribute " + a.first + " in line " + lineNumber + "!" << std::endl;
        continue;
      }
      if (!optionalIt->second.empty())
      {
        std::vector<std::string> values = tokenize(a.second, ',');
        for (auto const& v : values)
        {
          if (optionalIt->second.find(v) == optionalIt->second.end())
          {
            throw std::runtime_error("Spec error on line " + lineNumber + ": unexpected attribute value <" + v + "> in attribute <" + a.first + ">");
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
      std::stringstream ss;
      ss << e->GetLineNum();
      std::string lineNumber = ss.str();
      std::cerr << "warning: Unknown element in spec on line: " << lineNumber << " " << e->Value() << "!" << std::endl;
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
    std::stringstream ss;
    ss << elements[i]->GetLineNum();
    std::string lineNumber = ss.str();

    if (values.size() <= i)
    {
      throw std::runtime_error("Spec error on line " + lineNumber + ": unexpected surplus element <" + elements[i]->Value() + ">");
    }
    if (values[i] != elements[i]->Value())
    {
      throw std::runtime_error("Spec error on line " + lineNumber + ": unexpected element <" + elements[i]->Value() + ">, expected <" + values[i] + ">");
    }
  }
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

std::string readArraySize(tinyxml2::XMLNode const* node)
{
  if (node && node->ToText())
  {
    // following the name there might be some array size
    assert(!node->ToElement());
    std::string arraySize = node->Value();
    if ((arraySize.front() == '[') && (arraySize.back() == ']'))
    {
      return arraySize.substr(1, arraySize.length() - 2);
    }
  }
  return "";
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

std::string writeReinterpretCast(bool leadingConst, std::string const& type, bool trailingPointerToConst)
{
  std::string reinterpretCast = "reinterpret_cast<";
  if (leadingConst)
  {
    reinterpretCast += "const ";
  }
  reinterpretCast += type;
  if (trailingPointerToConst)
  {
    reinterpretCast += "* const";
  }
  reinterpretCast += "*>";
  return reinterpretCast;
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

void writeArgumentCount(std::ostream & os, size_t vectorIndex, std::string const& vectorName, std::string const& counterName, size_t returnParamIndex, size_t templateParamIndex, bool twoStep, bool singular)
{
  // this parameter is a count parameter for a vector parameter
  if ((returnParamIndex == vectorIndex) && twoStep)
  {
    // the corresponding vector parameter is the return parameter and it's a two-step algorithm
    // -> use the pointer to a local variable named like the counter parameter without leading 'p'
    assert((counterName[0] == 'p') && isupper(counterName[1]));
    os << "&" << startLowerCase(stripPrefix(counterName, "p"));
  }
  else
  {
    // the corresponding vector parameter is not the return parameter, or it's not a two-step algorithm
    if (singular)
    {
      // for the singular version, the count is just 1.
      os << "1 ";
    }
    else
    {
      // for the non-singular version, the count is the size of the vector parameter
      // -> use the vector parameter name without leading 'p' to get the size (in number of elements, not in bytes)
      assert(vectorName[0] == 'p');
      os << startLowerCase(stripPrefix(vectorName, "p")) << ".size() ";
    }
    if (templateParamIndex == vectorIndex)
    {
      // if the vector parameter is templatized -> multiply by the size of that type to get the size in bytes
      os << "* sizeof( T ) ";
    }
  }
}

void writeBitmask(std::ostream & os, std::string const& bitmaskName, std::string const& bitmaskAlias, std::string const& enumName, std::vector<std::pair<std::string, std::string>> const& enumValues)
{
  // each Flags class is using the class 'Flags' with the corresponding FlagBits enum as the template parameter
  os << std::endl
    << "  using " << bitmaskName << " = Flags<" << enumName << ", Vk" << bitmaskName << ">;" << std::endl;

  if (!enumValues.empty())
  {
    std::string allFlags;
    for (auto const& value : enumValues)
    {
      if (!allFlags.empty())
      {
        allFlags += " | ";
      }
      allFlags += "VkFlags(" + enumName + "::" + value.second + ")";
    }

    static const std::string bitmaskOperatorsTemplate = R"(
  VULKAN_HPP_INLINE ${bitmaskName} operator|( ${enumName} bit0, ${enumName} bit1 )
  {
    return ${bitmaskName}( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE ${bitmaskName} operator~( ${enumName} bits )
  {
    return ~( ${bitmaskName}( bits ) );
  }

  template <> struct FlagTraits<${enumName}>
  {
    enum
    {
      allFlags = ${allFlags}
    };
  };
)";

    os << replaceWithMap(bitmaskOperatorsTemplate, { { "bitmaskName", bitmaskName },{ "enumName", enumName },{ "allFlags", allFlags } });
  }

  if (!bitmaskAlias.empty())
  {
    os << std::endl
      << "  using " << stripPrefix(bitmaskAlias, "Vk") << " = " << bitmaskName << ";" << std::endl;
  }
}

void writeBitmaskToStringFunction(std::ostream & os, std::string const& bitmaskName, std::string const& enumName, std::vector<std::pair<std::string, std::string>> const& enumValues)
{
  std::string functionBody;
  if (enumValues.empty())
  {
    functionBody = "\n    return \"{}\";";
  }
  else
  {
    // 'or' together all the bits in the value
    static const std::string caseTemplate = R"(
    if ( value & ${typeName}::${value} ) result += "${valueString} | ";)";

    std::string casesString;
    for (auto const& value : enumValues)
    {
      casesString += replaceWithMap(caseTemplate, { { "typeName", enumName },{ "value", value.second },{ "valueString", value.second.substr(1) } });
    }

    static const std::string bodyTemplate = R"(
    if ( !value ) return "{}";
    std::string result;
${cases}
    return "{ " + result.substr(0, result.size() - 3) + " }";)";
    functionBody = replaceWithMap(bodyTemplate, { { "cases", casesString } });
  }

  static const std::string toStringTemplate = R"(
  VULKAN_HPP_INLINE std::string to_string( ${typeName}${argumentName} )
  {${functionBody}
  }
)";
  os << replaceWithMap(toStringTemplate, { { "typeName", bitmaskName },{ "argumentName", enumValues.empty() ? " " : " value " },{ "functionBody", functionBody } });
}

std::string writeFunctionBodyEnhancedLocalReturnVariableSingular(std::ostream & os, std::string const& indentation, std::string const& typeName, std::string const& returnName, bool isStructureChain)
{
  std::string adjustedReturnName = stripPluralS(returnName);
  if (isStructureChain)
  {
    // For StructureChains use the template parameters
    os << "StructureChain<X, Y, Z...> structureChain;" << std::endl;
    os << indentation << "  " << typeName << "& " << adjustedReturnName << " = structureChain.template get<" << typeName << ">()";
    adjustedReturnName = "structureChain";
  }
  else
  {
    // in singular case, just use the return parameters pure type for the return variable
    os << typeName << " " << adjustedReturnName;
  }
  return adjustedReturnName;
}

std::pair<bool, std::string> writeFunctionBodyStandardReturn(std::string const& returnType)
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
      // the return-type is a vulkan type -> need to cast to vk::-type
      ret += "static_cast<" + stripPrefix(returnType, "Vk") + ">( ";
    }
  }
  return std::make_pair(castReturn, ret);
}

VulkanHppGenerator::VulkanHppGenerator()
{
  m_handles.insert(std::make_pair("", HandleData()));   // insert the default "handle" without class (for createInstance, and such)
#if !defined(NDEBUG)
#endif
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

std::string VulkanHppGenerator::defaultValue(std::string const& type) const
{
  if (beginsWith(type, "PFN_vk") || (type == "LPCWSTR"))
  {
    return "nullptr";
  }
  else if (beginsWith(type, "Vk"))
  {
    auto const baseTypeIt = m_baseTypes.find(type);
    if (baseTypeIt != m_baseTypes.end())
    {
      return "0";
    }
    else
    {
      auto const& bitmaskBitIt = m_bitmaskBits.find(type);
      if (bitmaskBitIt != m_bitmaskBits.end())
      {
        return stripPrefix(type, "Vk") + (bitmaskBitIt->second.values.empty() ? "()" : ("::" + bitmaskBitIt->second.values.front().second));
      }
      else
      {
        auto const& enumIt = m_enums.find(type);
        if (enumIt != m_enums.end())
        {
          return stripPrefix(type, "Vk") + (enumIt->second.values.empty() ? "()" : ("::" + enumIt->second.values.front().second));
        }
        else
        {
          assert((m_bitmasks.find(type) != m_bitmasks.end()) || (m_handles.find(type) != m_handles.end()) || (m_structures.find(type) != m_structures.end()));
          return stripPrefix(type, "Vk") + "()";
        }
      }
    }
  }
  else
  {
    assert(m_defaultZeroTypes.find(type) != m_defaultZeroTypes.end());
    return "0";
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
          || ((commandData.successCodes.size() == 2)
            && (commandData.successCodes[1] == "eIncomplete")
            && twoStep)))))
  {
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

std::string VulkanHppGenerator::generateCall(std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool firstCall, bool singular) const
{
  std::ostringstream call;

  // the original function call
  call << "d." << commandData.first << "( ";

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;
  if (!handle.empty())
  {
    // if it's member of a class -> the first argument is the member variable, starting with "m_"
    assert(handle == commandData.second.params[0].type.type);
    call << "m_" << startLowerCase(stripPrefix(handle, "Vk"));
    if (1 < commandData.second.params.size())
    {
      call << ", ";
    }
  }

  writeArguments(call, commandData.second, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, firstCall, singular, handle.empty() ? 0 : 1, commandData.second.params.size());
  call << " )";
  return call.str();
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

    std::string requires;
    auto requiresIt = attributes.find("requires");
    if (requiresIt != attributes.end())
    {
      requires = requiresIt->second;
    }
    else
    {
      // Generate FlagBits name and add it to the list of enums and vulkan types
      requires = name;
      size_t pos = requires.rfind("Flags");
      assert(pos != std::string::npos);
      requires.replace(pos, 5, "FlagBits");

      assert(m_bitmaskBits.find(requires) == m_bitmaskBits.end());
      m_bitmaskBits.insert(std::make_pair(requires, EnumData()));
    }

    m_bitmasks.insert(std::make_pair(name, BitmaskData(requires)));
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
      paramData.arraySize = readArraySize(child->NextSibling());
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
    m_vulkanLicenseHeader = trim(m_vulkanLicenseHeader);
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
    auto enumIt = std::find_if(enumData.values.begin(), enumData.values.end(), [&aliasIt](std::pair<std::string, std::string> const& value) { return value.first == aliasIt->second; });
    assert(enumIt != enumData.values.end());
    enumData.aliases.push_back(std::make_pair(name, createEnumValueName(name, prefix, postfix, bitmask, tag)));
  }
  else
  {
    enumData.addEnumValue(name, bitmask, prefix, postfix, tag);
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

    // ad an empty EnumData on this name into the enums map
    EnumData enumData;
    std::string type = attributes.find("type")->second;
    bool bitmask = (type == "bitmask");
    std::map<std::string, EnumData>::iterator it = bitmask ? m_bitmaskBits.insert(std::make_pair(name, enumData)).first : m_enums.insert(std::make_pair(name, enumData)).first;
    assert(it->second.values.empty());

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
        assert(m_structures.find(nameAttribute->second) != m_structures.end());
        m_structures.erase(nameAttribute->second);
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
  if (!platform.empty())
  {
    std::string name = attributes.find("name")->second;

    auto bmit = m_bitmasks.find(name);
    if (bmit != m_bitmasks.end())
    {
      assert(bmit->second.platform.empty());
      bmit->second.platform = platform;
      assert(m_bitmaskBits.find(bmit->second.requires) != m_bitmaskBits.end());
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
    bool bitmask = false;
    std::string extends = extendsIt->second;
    auto enumIt = m_enums.find(extends);
    if (enumIt == m_enums.end())
    {
      enumIt = m_bitmaskBits.find(extends);
      assert(enumIt != m_bitmaskBits.end());
      bitmask = true;
    }

    std::string prefix = getEnumPrefix(enumIt->first, bitmask);
    std::string postfix = getEnumPostfix(enumIt->first, m_tags, prefix);

    auto nameIt = attributes.find("name");
    assert(nameIt != attributes.end());

    auto aliasIt = attributes.find("alias");
    if (aliasIt != attributes.end())
    {
      checkAttributes(attributes, element->GetLineNum(), { { "alias",{} },{ "extends",{} },{ "name",{} } }, { { "comment",{} } });

      // look for the aliased enum value
      std::string alias = aliasIt->second;
      auto valueIt = std::find_if(enumIt->second.values.begin(), enumIt->second.values.end(), [&alias](std::pair<std::string, std::string> const& value) { return value.first == alias; });
      if (valueIt == enumIt->second.values.end())
      {
        // if the aliased enum value is not found in the values, look in the aliases as well!
        valueIt = std::find_if(enumIt->second.aliases.begin(), enumIt->second.aliases.end(), [&alias](std::pair<std::string, std::string> const& value) { return value.first == alias; });
        assert(valueIt != enumIt->second.aliases.end());
      }

      std::string name = createEnumValueName(nameIt->second, prefix, postfix, bitmask, tag);
      if (valueIt->second != name)
      {
        // only add an alias if it's different from the aliased name
        enumIt->second.aliases.push_back(std::make_pair(nameIt->second, name));
      }
    }
    else
    {
      assert((attributes.find("bitpos") != attributes.end()) + (attributes.find("offset") != attributes.end()) + (attributes.find("value") != attributes.end()) == 1);
      enumIt->second.addEnumValue(nameIt->second, bitmask, prefix, postfix, tag);
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
  assert(m_defaultZeroTypes.find(nameIt->second) == m_defaultZeroTypes.end());
  m_defaultZeroTypes.insert(nameIt->second);
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
      memberData.arraySize = child->GetText();
    }
    else if (value == "name")
    {
      memberData.name = child->GetText();
      memberData.arraySize = readArraySize(child->NextSibling());
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
    else if ((categoryIt->second != "enum") && (categoryIt->second != "include"))
    {
      std::stringstream ss;
      ss << element->GetLineNum();
      std::string lineNumber = ss.str();

      throw std::runtime_error("Spec error on line " + lineNumber + ": unknown category <" + categoryIt->second + ">");
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
      m_defaultZeroTypes.insert("int");
    }
  }
#endif
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

void VulkanHppGenerator::writeArgumentPlainType(std::ostream & os, ParamData const& paramData) const
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
        os << parameterName << paramData.optional ? (" ? " + parameterName + "->c_str() : nullptr") : ".c_str()";
      }
      else
      {
        // it's const pointer to something else -> just use the name
        assert(!paramData.optional);
        os << paramData.name;
      }
    }
    else
    {
      // it's a non-const pointer, and char is the only type that occurs -> use the address of the parameter
      assert(paramData.type.type.find("char") == std::string::npos);
      os << "&" << parameterName;
    }
  }
  else
  {
    // it's a plain parameter -> just use its name
    os << paramData.name;
  }
}

void VulkanHppGenerator::writeArguments(std::ostream & os, CommandData const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool firstCall, bool singular, size_t from, size_t to) const
{
  assert(from <= to);

  bool encounteredArgument = false;
  for (size_t i = from; i < to; i++)
  {
    if (encounteredArgument)
    {
      os << ", ";
    }

    auto it = vectorParamIndices.find(i);
    if (it != vectorParamIndices.end())
    {
      writeArgumentVector(os, it->first, commandData.params[it->first], returnParamIndex, templateParamIndex, twoStep, firstCall, singular);
    }
    else
    {
      it = find_if(vectorParamIndices.begin(), vectorParamIndices.end(), [i](std::pair<size_t, size_t> const& vpi) { return vpi.second == i; });
      if (it != vectorParamIndices.end())
      {
        writeArgumentCount(os, it->first, commandData.params[it->first].name, commandData.params[it->second].name, returnParamIndex, templateParamIndex, twoStep, singular);
      }
      else if (beginsWith(commandData.params[i].type.type, "Vk"))
      {
        writeArgumentVulkanType(os, commandData.params[i]);
      }
      else
      {
        writeArgumentPlainType(os, commandData.params[i]);
      }
    }
    encounteredArgument = true;
  }
}

void VulkanHppGenerator::writeArgumentVector(std::ostream & os, size_t paramIndex, ParamData const& paramData, size_t returnParamIndex, size_t templateParamIndex, bool twoStep, bool firstCall, bool singular) const
{
  // this parameter is a vector parameter
  assert(paramData.type.postfix.back() == '*');
  if ((returnParamIndex == paramIndex) && twoStep && firstCall)
  {
    // this parameter is the return parameter, and it's the first call of a two-step algorithm -> just just nullptr
    os << "nullptr";
  }
  else
  {
    std::string parameterName = startLowerCase(stripPrefix(paramData.name, "p"));
    if (beginsWith(paramData.type.type, "Vk") || (paramIndex == templateParamIndex))
    {
      // CHECK for !commandData.params[it->first].optional

      // this parameter is a vulkan type or a templated type -> need to reinterpret cast
      os << writeReinterpretCast(paramData.type.prefix.find("const") == 0, paramData.type.type, paramData.type.postfix.rfind("* const") != std::string::npos);
      os << "( " << (singular ? ("&" + stripPluralS(parameterName)) : (parameterName + ".data()")) << " )";
    }
    else if (paramData.type.type == "char")
    {
      // the parameter is a vector to char -> it might be optional
      // besides that, the parameter now is a std::string -> get the pointer via c_str()
      os << parameterName << (paramData.optional ? (" ? " + parameterName + "->c_str() : nullptr") : ".c_str()");
    }
    else
    {
      // this parameter is just a vetor -> get the pointer to its data
      os << parameterName << ".data()";
    }
  }
}

void VulkanHppGenerator::writeArgumentVulkanType(std::ostream & os, ParamData const& paramData) const
{
  // this parameter is a vulkan type
  if (!paramData.type.postfix.empty())
  {
    assert(paramData.type.postfix.back() == '*');
    // it's a pointer -> needs a reinterpret cast to the vulkan type
    std::string parameterName = startLowerCase(stripPrefix(paramData.name, "p"));
    os << writeReinterpretCast(paramData.type.prefix.find("const") != std::string::npos, paramData.type.type, false);
    os << "( ";
    if (paramData.optional)
    {
      // for an optional parameter, we need also a static_cast from optional type to const-pointer to pure type
      os << "static_cast<const " << stripPrefix(paramData.type.type, "Vk") << "*>( " << parameterName << " )";
    }
    else
    {
      // other parameters can just use the pointer
      os << "&" << parameterName;
    }
    os << " )";
  }
  else
  {
    // a non-pointer parameter needs a static_cast from vk::-type to vulkan type
    os << "static_cast<" << paramData.type.type << ">( " << paramData.name << " )";
  }
}

void VulkanHppGenerator::writeBaseTypes(std::ostream & os) const
{
  assert(!m_baseTypes.empty());
  for (auto const& baseType : m_baseTypes)
  {
    if (baseType.first != "VkFlags")    // filter out VkFlags
    {
      os << "  using " << stripPrefix(baseType.first, "Vk") << " = " << baseType.second << ";" << std::endl;
    }
  }
}

void VulkanHppGenerator::writeBitmasks(std::ostream & os) const
{
  for (auto const& bitmask : m_bitmasks)
  {
    auto bitmaskBits = m_bitmaskBits.find(bitmask.second.requires);
    assert(bitmaskBits != m_bitmaskBits.end());

    os << std::endl;
    writePlatformEnter(os, bitmask.second.platform);
    writeEnum(os, *bitmaskBits);
    writeEnumToString(os, *bitmaskBits);
    std::string strippedBitmaskName = stripPrefix(bitmask.first, "Vk");
    std::string strippedEnumName = stripPrefix(bitmaskBits->first, "Vk");
    writeBitmask(os, strippedBitmaskName, bitmask.second.alias, strippedEnumName, bitmaskBits->second.values);
    writeBitmaskToStringFunction(os, strippedBitmaskName, strippedEnumName, bitmaskBits->second.values);
    writePlatformLeave(os, bitmask.second.platform);
  }
}

void VulkanHppGenerator::writeCommand(std::ostream & os, std::string const& indentation, std::string const& name, std::pair<std::string, CommandData> const& commandData, bool definition) const
{
  bool twoStep = isTwoStepAlgorithm(commandData.second.params);
  std::map<size_t, size_t> vectorParamIndices = determineVectorParamIndices(commandData.second.params);

  size_t returnParamIndex = determineReturnParamIndex(commandData.second, vectorParamIndices, twoStep);
  bool isStructureChain = (returnParamIndex != INVALID_INDEX) && determineStructureChaining(commandData.second.params[returnParamIndex].type.type, m_extendedStructs, m_structureAliases);
  std::string enhancedReturnType = determineEnhancedReturnType(commandData.second, returnParamIndex, vectorParamIndices, twoStep, false);   // get the enhanced return type without structureChain

  size_t templateParamIndex = determineTemplateParamIndex(commandData.second.params, vectorParamIndices);

  // first create the standard version of the function
  std::ostringstream standard;
  writeFunction(standard, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, false, false, false, false, false);

  // then the enhanced version, composed by up to eight parts
  std::ostringstream enhanced;
  writeFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, true, false, false, false, false);

  if (enhancedReturnType.find("Allocator") != std::string::npos)
  {
    writeFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, true, false, false, false, true);
  }

  if (isStructureChain)
  {
    std::string enhancedReturnTypeWithStructureChain = determineEnhancedReturnType(commandData.second, returnParamIndex, vectorParamIndices, twoStep, true);
    writeFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnTypeWithStructureChain, definition, true, false, false, true, false);

    if (enhancedReturnTypeWithStructureChain.find("Allocator") != std::string::npos)
    {
      writeFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnTypeWithStructureChain, definition, true, false, false, true, true);
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
    writeFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, true, true, false, false, false);
  }

  // special handling for createDevice and createInstance !
  bool specialWriteUnique = (commandData.first == "vkCreateDevice") || (commandData.first == "vkCreateInstance");

  // and then the same for the Unique* versions (a deleteCommand is available for the commandData's class, and the function starts with 'allocate' or 'create')
  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  auto handleIt = m_handles.find(m_commandToHandle.find(commandData.first)->second);
  assert(handleIt != m_handles.end());
  if ((!handleIt->second.deleteCommand.empty() || specialWriteUnique) && ((commandData.first.substr(2, 8) == "Allocate") || (commandData.first.substr(2, 6) == "Create")))
  {
    enhanced << "#ifndef VULKAN_HPP_NO_SMART_HANDLE" << std::endl;
    writeFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, true, false, true, false, false);

    if (enhancedReturnType.find("Allocator") != std::string::npos)
    {
      writeFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, true, false, true, false, true);
    }

    if (singular)
    {
      writeFunction(enhanced, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, definition, true, true, true, false, false);
    }
    enhanced << "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/" << std::endl;
  }

  // and write one or both of them
  std::string standardString = standard.str();
  std::string enhancedString = enhanced.str();
  if (standardString == enhancedString)
  {
    // standard and enhanced string are equal -> just use one of them and we're done
    os << standardString;
  }
  else
  {
    // standard and enhanced string differ -> use both, wrapping the enhanced by !VULKAN_HPP_DISABLE_ENHANCED_MODE
    // determine the argument list of that standard, and compare it with that of the enhanced
    // if they are equal -> need to have just one; if they differ -> need to have both
    size_t standardStart = standardString.find('(');
    size_t standardCount = standardString.find(')', standardStart) - standardStart;
    size_t enhancedStart = enhancedString.find('(');
    bool unchangedInterface = (standardString.substr(standardStart, standardCount) == enhancedString.substr(enhancedStart, standardCount));
    if (unchangedInterface)
    {
      os << "#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE" << std::endl;
    }
    os << standardString
      << (unchangedInterface ? "#else" : "#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE") << std::endl
      << enhancedString
      << "#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/" << std::endl;
  }
}

void VulkanHppGenerator::writeDispatchLoaderDynamic(std::ostream &os)
{
  os << std::endl
    << "  class DispatchLoaderDynamic" << std::endl
    << "  {" << std::endl
    << "  public:" << std::endl;

  for (auto const& handle : m_handles)
  {
    for (auto const& command : handle.second.commands)
    {
      writePlatformEnter(os, command.second.platform);
      os << "    PFN_" << command.first << " " << command.first << " = 0;" << std::endl;
      writePlatformLeave(os, command.second.platform);
    }
  }

  // write initialization function to fetch function pointers
  os << R"(
  public:
    DispatchLoaderDynamic() = default;

#if !defined(VK_NO_PROTOTYPES)
    // This interface is designed to be used for per-device function pointers in combination with a linked vulkan library.
    DispatchLoaderDynamic(vk::Instance const& instance, vk::Device const& device = {})
    {
      init(instance, device);
    }

    // This interface is designed to be used for per-device function pointers in combination with a linked vulkan library.
    void init(vk::Instance const& instance, vk::Device const& device = {})
    {
      init(instance, ::vkGetInstanceProcAddr, device, device ? ::vkGetDeviceProcAddr : nullptr);
    }
#endif // !defined(VK_NO_PROTOTYPES)

    // This interface does not require a linked vulkan library.
    DispatchLoaderDynamic( VkInstance instance, PFN_vkGetInstanceProcAddr getInstanceProcAddr, VkDevice device = VK_NULL_HANDLE, PFN_vkGetDeviceProcAddr getDeviceProcAddr = nullptr )
    {
      init( instance, getInstanceProcAddr, device, getDeviceProcAddr );
    }

    // This interface does not require a linked vulkan library.
    void init( VkInstance instance, PFN_vkGetInstanceProcAddr getInstanceProcAddr, VkDevice device = VK_NULL_HANDLE, PFN_vkGetDeviceProcAddr getDeviceProcAddr = nullptr )
    {
      assert(instance && getInstanceProcAddr);
      assert(!!device == !!getDeviceProcAddr);
      vkGetInstanceProcAddr = getInstanceProcAddr;
      vkGetDeviceProcAddr = getDeviceProcAddr ? getDeviceProcAddr : PFN_vkGetDeviceProcAddr( vkGetInstanceProcAddr( instance, "vkGetDeviceProcAddr") );
)";

  for (auto const& handle : m_handles)
  {
    for (auto const& command : handle.second.commands)
    {
      if ((command.first != "vkGetDeviceProcAddr") && (command.first != "vkGetInstanceProcAddr"))
      {
        writePlatformEnter(os, command.second.platform);
        if (!command.second.params.empty()
          && m_handles.find(command.second.params[0].type.type) != m_handles.end()
          && command.second.params[0].type.type != "VkInstance"
          && command.second.params[0].type.type != "VkPhysicalDevice")
        {
          os << "      " << command.first << " = PFN_" << command.first
            << "( device ? vkGetDeviceProcAddr( device, \"" << command.first << "\" ) : vkGetInstanceProcAddr( instance, \"" << command.first << "\" ) );" << std::endl;
        }
        else
        {
          os << "      " << command.first << " = PFN_" << command.first << "( vkGetInstanceProcAddr( instance, \"" << command.first << "\" ) );" << std::endl;
        }
        writePlatformLeave(os, command.second.platform);
      }
    }
  }
  os << "    }" << std::endl;
  os << "  };\n";
}

void VulkanHppGenerator::writeDispatchLoaderStatic(std::ostream &os)
{
  static const std::string commandTemplate =
    R"(    ${returnType} vk${commandName}( ${parameterList} ) const
    {
      return ::vk${commandName}( ${parameters} );
    }
)";

  std::ostringstream commands;
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
        parameterList += param.type.prefix + (param.type.prefix.empty() ? "" : " ") + param.type.type + param.type.postfix + " " + param.name;
        if (!param.arraySize.empty())
        {
          parameterList += "[" + param.arraySize + "]";
        }
        parameters += param.name;
        firstParam = false;
      }

      commands << std::endl;
      writePlatformEnter(commands, command.second.platform);
      commands << replaceWithMap(commandTemplate,
      {
        { "returnType", command.second.returnType },
        { "commandName", stripPrefix(command.first, "vk") },
        { "parameterList", parameterList },
        { "parameters", parameters },
      });
      writePlatformLeave(commands, command.second.platform);
    }
  }

  static const std::string dispatchTemplate = R"(
  class DispatchLoaderStatic
  {
  public:${commands}  };
)";

  os << replaceWithMap(dispatchTemplate, { { "commands", commands.str() } });
}

void VulkanHppGenerator::writeDispatchLoaderDefault(std::ostream &os)
{
  os << std::endl
    << "#if !defined(VK_NO_PROTOTYPES)";
  writeDispatchLoaderStatic(os);
  os << R"(
  typedef DispatchLoaderStatic DispatchLoaderDefault;
#else // !defined(VK_NO_PROTOTYPES)
  class NeedExplicitDispatchLoader;
  typedef NeedExplicitDispatchLoader DispatchLoaderDefault;
#endif
)";
}

void VulkanHppGenerator::writeEnum(std::ostream & os, std::pair<std::string,EnumData> const& enumData) const
{
  std::string values;
  for (auto const& value : enumData.second.values)
  {
    if (!values.empty())
    {
      values += ",";
    }
    values += "\n    " + value.second + " = " + value.first;
  }
  for (auto const& value : enumData.second.aliases)
  {
    if (!values.empty())
    {
      values += ",";
    }
    values += "\n    " + value.second + " = " + value.first;
  }
  if (!values.empty())
  {
    values += "\n  ";
  }

  static const std::string enumTemplate =
R"(  enum class ${name}
  {${values}};
)";

  os << replaceWithMap(enumTemplate,
  {
    { "name", stripPrefix(enumData.first, "Vk") },
    { "values", values },
  });
}

void VulkanHppGenerator::writeEnums(std::ostream & os) const
{
  for (auto const& e : m_enums)
  {
    os << std::endl;
    writePlatformEnter(os, e.second.platform);
    writeEnum(os, e);
    writeEnumToString(os, e);
    writePlatformLeave(os, e.second.platform);
  }
}

void VulkanHppGenerator::writeEnumToString(std::ostream & os, std::pair<std::string, EnumData> const& enumData) const
{
  std::string enumName = stripPrefix(enumData.first, "Vk");
  std::string functionBody;
  if (enumData.second.values.empty())
  {
    functionBody = "\n    return \"(void)\";";
  }
  else
  {
    static const std::string caseTemplate = R"(      case ${type}::${value} : return "${valueText}";)";
    std::ostringstream casesString;
    for (auto const& value : enumData.second.values)
    {
      casesString << replaceWithMap(caseTemplate, { { "type", enumName },{ "value", value.second },{ "valueText", value.second.substr(1) } }) << std::endl;
    }

    static const std::string switchTemplate = R"(
    switch ( value )
    {
${cases}      default: return "invalid";
    })";
    functionBody = replaceWithMap(switchTemplate, { { "cases", casesString.str() } });
  }

  static const std::string enumToString = R"(
  VULKAN_HPP_INLINE std::string to_string( ${typeName}${argumentName} )
  {${functionBody}
  }
)";
  os << replaceWithMap(enumToString,
  {
    { "typeName", enumName },
    { "argumentName", enumData.second.values.empty() ? "" : " value" },
    { "functionBody", functionBody }
  });
}

void VulkanHppGenerator::writeForwardDeclarations(std::ostream & os) const
{
  os << std::endl;
  for (auto const& structure : m_structures)
  {
    writePlatformEnter(os, structure.second.platform);
    os << "  " << (structure.second.isUnion ? "union" : "struct") << " " << stripPrefix(structure.first, "Vk") << ";" << std::endl;
    for (std::string const& alias : structure.second.aliases)
    {
      os << "  using " << stripPrefix(alias, "Vk") << " = " << stripPrefix(structure.first, "Vk") << ";" << std::endl;
    }
    writePlatformLeave(os, structure.second.platform);
  }
}

void VulkanHppGenerator::writeFunction(std::ostream & os, std::string const& indentation, std::string const& name, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, std::string const& enhancedReturnType, bool definition, bool enhanced, bool singular, bool unique, bool isStructureChain, bool withAllocator) const
{
  writeFunctionHeaderTemplate(os, indentation, returnParamIndex, templateParamIndex, enhancedReturnType, enhanced, singular, unique, !definition, isStructureChain);

  os << indentation << (definition ? "VULKAN_HPP_INLINE " : "");
  writeFunctionHeaderReturnType(os, commandData.second, returnParamIndex, vectorParamIndices, enhancedReturnType, enhanced, singular, unique, isStructureChain);
  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;
  if (definition && !handle.empty())
  {
    os << stripPrefix(handle, "Vk") << "::";
  }

  // write the function header name
  os << (singular ? stripPluralS(name) : name);
  if (unique)
  {
    os << "Unique";
  }

  writeFunctionHeaderArguments(os, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, enhanced, singular, !definition, withAllocator);
  os << (definition ? "" : ";") << std::endl;

  if (definition)
  {
    // write the function body
    os << indentation << "{" << std::endl;
    if (enhanced)
    {
      writeFunctionBodyEnhanced(os, indentation, name, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, enhancedReturnType, singular, unique, isStructureChain, withAllocator);
    }
    else
    {
      writeFunctionBodyStandard(os, indentation, commandData);
    }
    os << indentation << "}" << std::endl;
  }
}

void VulkanHppGenerator::writeFunctionBodyEnhanced(std::ostream & os, std::string const& indentation, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, std::string const& enhancedReturnType, bool singular, bool unique, bool isStructureChain, bool withAllocator) const
{
  if (unique && !singular && (vectorParamIndices.find(returnParamIndex) != vectorParamIndices.end()))    // returns a vector of UniqueStuff
  {
    writeFunctionBodyEnhancedVectorOfUniqueHandles(os, indentation, commandName, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, singular, withAllocator);
  }
  else
  {
    if (1 < vectorParamIndices.size())
    {
      writeFunctionBodyEnhancedMultiVectorSizeCheck(os, indentation, commandName, commandData, returnParamIndex, vectorParamIndices);
    }

    std::string returnName;
    if (returnParamIndex != INVALID_INDEX)
    {
      returnName = writeFunctionBodyEnhancedLocalReturnVariable(os, indentation, commandData.second, returnParamIndex, vectorParamIndices, twoStep, enhancedReturnType, singular, isStructureChain, withAllocator);
    }

    if (twoStep)
    {
      writeFunctionBodyEnhancedTwoStep(os, indentation, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, singular, returnName);
    }
    else
    {
      writeFunctionBodyEnhancedSingleStep(os, indentation, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, singular);
    }

    if ((commandData.second.returnType == "VkResult") || !commandData.second.successCodes.empty())
    {
      writeFunctionBodyEnhancedReturnResultValue(os, indentation, returnName, commandName, commandData, returnParamIndex, twoStep, singular, unique);
    }
    else if ((returnParamIndex != INVALID_INDEX) && (stripPrefix(commandData.second.returnType, "Vk") != enhancedReturnType))
    {
      // for the other returning cases, when the return type is somhow enhanced, just return the local returnVariable
      os << indentation << "  return " << returnName << ";" << std::endl;
    }
  }
}

std::string VulkanHppGenerator::writeFunctionBodyEnhancedLocalReturnVariable(std::ostream & os, std::string const& indentation, CommandData const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, std::string const& enhancedReturnType, bool singular, bool isStructureChain, bool withAllocator) const
{
  std::string pureReturnType = stripPrefix(commandData.params[returnParamIndex].type.type, "Vk");
  std::string returnName = startLowerCase(stripPrefix(commandData.params[returnParamIndex].name, "p"));

  // there is a returned parameter -> we need a local variable to hold that value
  if (stripPrefix(commandData.returnType, "Vk") != enhancedReturnType)
  {
    // the returned parameter is somehow enhanced by us
    os << indentation << "  ";
    if (singular)
    {
      returnName = writeFunctionBodyEnhancedLocalReturnVariableSingular(os, indentation, pureReturnType, returnName, isStructureChain);
    }
    else
    {
      // in non-singular case, use the enhanced type for the return variable (like vector<...>)
      if (isStructureChain && vectorParamIndices.empty())
      {
        // For StructureChains use the template parameters
        std::string const returnVariableTemplate =
R"(StructureChain<X, Y, Z...> structureChain;
${i}  ${enhancedReturnType}& ${returnName} = structureChain.template get<${enhancedReturnType}>())";

        os << replaceWithMap(returnVariableTemplate, { { "i", indentation }, { "enhancedReturnType", enhancedReturnType }, { "returnName", returnName } });
        returnName = "structureChain";
      }
      else
      {
        os << enhancedReturnType << " " << returnName;
      }

      std::map<size_t, size_t>::const_iterator vpiIt = vectorParamIndices.find(returnParamIndex);
      if (vpiIt != vectorParamIndices.end() && !twoStep)
      {
        writeFunctionBodyEnhancedLocalReturnVariableVectorSize(os, commandData.params, *vpiIt, returnParamIndex, vectorParamIndices, withAllocator);
      }
      else if (withAllocator)
      {
        os << "( vectorAllocator )";
      }
    }
    os << ";" << std::endl;
  }
  else
  {
    // the return parameter is not enhanced -> the type is supposed to be a Result and there are more than one success codes!
    assert((commandData.returnType == "VkResult") && (1 < commandData.successCodes.size()));
    os << indentation << "  " << pureReturnType << " " << returnName << ";" << std::endl;
  }

  return returnName;
}

void VulkanHppGenerator::writeFunctionBodyEnhancedLocalReturnVariableVectorSize(std::ostream & os, std::vector<ParamData> const& params, std::pair<size_t, size_t> const& vectorParamIndex, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool withAllocator) const
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
      if ((vpi.first != vectorParamIndex.first) || (vpi.second != vectorParamIndex.second))
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
  os << "( " << size << (withAllocator ? ", vectorAllocator" : "") << " )";
}

void VulkanHppGenerator::writeFunctionBodyEnhancedMultiVectorSizeCheck(std::ostream & os, std::string const& indentation, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices) const
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
          os << replaceWithMap(sizeCheckTemplate, std::map<std::string, std::string>({
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

void VulkanHppGenerator::writeFunctionBodyEnhancedReturnResultValue(std::ostream & os, std::string const& indentation, std::string const& returnName, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, bool twoStep, bool singular, bool unique) const
{
  std::string type = (returnParamIndex != INVALID_INDEX) ? commandData.second.params[returnParamIndex].type.type : "";
  std::string returnVectorName = (returnParamIndex != INVALID_INDEX) ? stripPostfix(stripPrefix(commandData.second.params[returnParamIndex].name, "p"), "s") : "";

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;

  if (unique)
  {
    // the unique version needs a Deleter object for destruction of the newly created stuff
    // get the DeleterData corresponding to the returned type
    // special handling for "createDevice", as Device is created from PhysicalDevice, but destroyed on its own
    bool noParent = handle.empty() || (commandData.first == "vkCreateDevice");
    os << std::endl
      << indentation << ((commandData.first == "vkAllocateMemory") ? "  ObjectFree<" : "  ObjectDestroy<") << (noParent ? "NoParent" : stripPrefix(handle, "Vk")) << ",Dispatch> deleter( " << (noParent ? "" : "*this, ") << "allocator, d );" << std::endl
      << indentation << "  return createResultValue<" << stripPrefix(type, "Vk") << ",Dispatch>( result, ";
  }
  else
  {
    os << indentation << "  return createResultValue( result, ";
  }

  // if the return type is "Result" or there is at least one success code, create the Result/Value construct to return
  if (returnParamIndex != INVALID_INDEX)
  {
    // if there's a return parameter, list it in the Result/Value constructor
    os << returnName << ", ";
  }

  // now the function name (with full namespace) as a string
  os << "VULKAN_HPP_NAMESPACE_STRING\"::" << (handle.empty() ? "" : stripPrefix(handle, "Vk") + "::") << (singular ? stripPluralS(commandName) : commandName) << (unique ? "Unique" : "") << "\"";

  if (!twoStep && (1 < commandData.second.successCodes.size()))
  {
    // and for the single-step algorithms with more than one success code list them all
    os << ", { Result::" << commandData.second.successCodes[0];
    for (size_t i = 1; i < commandData.second.successCodes.size(); i++)
    {
      os << ", Result::" << commandData.second.successCodes[i];
    }
    os << " }";
  }

  if (unique)
  {
    os << ", deleter";
  }

  os << " );" << std::endl;
}

void VulkanHppGenerator::writeFunctionBodyEnhancedSingleStep(std::ostream & os, std::string const& indentation, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool singular) const
{
  std::string const resultCallTemplate = "${i}  Result result = static_cast<Result>( ${call} );\n";
  std::string const returnCallTemplate = "${i}  return ${call};\n";
  std::string const voidCallTemplate = "${i}  ${call};\n";
  std::string const& selectedTemplate = (commandData.second.returnType == "VkResult") ? resultCallTemplate : (commandData.second.returnType == "void") ? voidCallTemplate : returnCallTemplate;
  os << replaceWithMap(selectedTemplate,
  {
    { "call", generateCall(commandData, returnParamIndex, templateParamIndex, vectorParamIndices, false, true, singular) },
    { "i", indentation }
  });
}

void VulkanHppGenerator::writeFunctionBodyEnhancedTwoStep(std::ostream & os, std::string const& indentation, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool singular, std::string const& returnName) const
{
  assert(!singular);
  assert((commandData.second.returnType == "VkResult") || (commandData.second.returnType == "void"));
  assert(returnParamIndex != INVALID_INDEX);

  // local count variable to hold the size of the vector to fill
  std::map<size_t, size_t>::const_iterator returnit = vectorParamIndices.find(returnParamIndex);
  assert(returnit != vectorParamIndices.end() && (returnit->second != INVALID_INDEX));

  // take the pure type of the size parameter; strip the leading 'p' from its name for its local name
  std::string sizeName = startLowerCase(stripPrefix(commandData.second.params[returnit->second].name, "p"));
  os << indentation << "  " << stripPrefix(commandData.second.params[returnit->second].type.type, "Vk") << " " << sizeName << ";" << std::endl;

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
  os << replaceWithMap(selectedTemplate,
  {
    { "sizeName", sizeName },
    { "returnName", returnName },
    { "call1", generateCall(commandData, returnParamIndex, templateParamIndex, vectorParamIndices, true, true, false) },
    { "call2", generateCall(commandData, returnParamIndex, templateParamIndex, vectorParamIndices, true, false, false) },
    { "i", indentation }
  });
}

void VulkanHppGenerator::writeFunctionBodyEnhancedVectorOfUniqueHandles(std::ostream & os, std::string const& indentation, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool singular, bool withAllocator) const
{
  std::string const stringTemplate =
    R"(${i}  static_assert( sizeof( ${type} ) <= sizeof( UniqueHandle<${type}, Dispatch> ), "${type} is greater than UniqueHandle<${type}, Dispatch>!" );
${i}  std::vector<UniqueHandle<${type}, Dispatch>, Allocator> ${typeVariable}s${allocator};
${i}  ${typeVariable}s.reserve( ${vectorSize} );
${i}  ${type}* buffer = reinterpret_cast<${type}*>( reinterpret_cast<char*>( ${typeVariable}s.data() ) + ${vectorSize} * ( sizeof( UniqueHandle<${type}, Dispatch> ) - sizeof( ${type} ) ) );
${i}  Result result = static_cast<Result>(d.vk${command}( m_device, ${arguments}, reinterpret_cast<Vk${type}*>( buffer ) ) );
${i}  if (result == vk::Result::eSuccess)
${i}  {
${i}    ${Deleter}<${DeleterTemplate},Dispatch> deleter( *this, ${deleterArg}, d );
${i}    for ( size_t i=0 ; i<${vectorSize} ; i++ )
${i}    {
${i}      ${typeVariable}s.push_back( UniqueHandle<${type}, Dispatch>( buffer[i], deleter ) );
${i}    }
${i}  }

${i}  return createResultValue( result, ${typeVariable}s, VULKAN_HPP_NAMESPACE_STRING "::${class}::${commandName}Unique" );
)";

  std::string type = (returnParamIndex != INVALID_INDEX) ? commandData.second.params[returnParamIndex].type.type : "";
  std::string typeVariable = startLowerCase(stripPrefix(type, "Vk"));
  std::ostringstream arguments;
  writeArguments(arguments, commandData.second, returnParamIndex, templateParamIndex, vectorParamIndices, twoStep, true, singular, 1, commandData.second.params.size() - 1);

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;

  auto handleIt = m_handles.find(type);
  assert(handleIt != m_handles.end());

  bool isCreateFunction = (commandData.first.substr(2, 6) == "Create");
  os << replaceWithMap(stringTemplate, std::map<std::string, std::string>
  {
    { "i", indentation },
    { "type", stripPrefix(type, "Vk") },
    { "typeVariable", typeVariable },
    { "allocator", withAllocator ? "( vectorAllocator )" : "" },
    { "vectorSize", isCreateFunction ? "createInfos.size()" : "allocateInfo." + typeVariable + "Count" },
    { "command", stripPrefix(commandData.first, "vk") },
    { "arguments", arguments.str() },
    { "Deleter", handleIt->second.deletePool.empty() ? "ObjectDestroy" : "PoolFree" },
    { "DeleterTemplate", stripPrefix(handle, "Vk") + (handleIt->second.deletePool.empty() ? "" : "," + stripPrefix(handleIt->second.deletePool, "Vk")) },
    { "deleterArg", handleIt->second.deletePool.empty() ? "allocator" : "allocateInfo." + startLowerCase(stripPrefix(handleIt->second.deletePool, "Vk")) },
    { "class", stripPrefix(handle, "Vk") },
    { "commandName", commandName }
  });
}

void VulkanHppGenerator::writeFunctionBodyStandard(std::ostream & os, std::string const& indentation, std::pair<std::string, CommandData> const& commandData) const
{
  std::pair<bool, std::string> returnData = writeFunctionBodyStandardReturn(commandData.second.returnType);

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;
  assert(handle.empty() || (handle == commandData.second.params[0].type.type));

  std::string arguments;
  for (size_t i = handle.empty() ? 0 : 1; i < commandData.second.params.size(); i++)
  {
    if (0 < i)
    {
      arguments += ", ";
    }
    arguments += writeFunctionBodyStandardArgument(commandData.second.params[i].type, commandData.second.params[i].name);
  }

  std::string const standardTemplate =
    R"(${i}  ${return}d.${command}( ${handleArg}${arguments} )${castClose};
)";

  os << replaceWithMap(standardTemplate,
  {
    { "i", indentation },
    { "return", returnData.second },
    { "command", commandData.first },
    { "handleArg", handle.empty() ? "" : ("m_" + startLowerCase(stripPrefix(handle, "Vk"))) },
    { "arguments", arguments },
    { "castClose", returnData.first ? " )" : "" }
  });
}

std::string VulkanHppGenerator::writeFunctionBodyStandardArgument(TypeData const& typeData, std::string const& name) const
{
  std::string argument;
  if (beginsWith(typeData.type, "Vk"))
  {
    // the parameter is a vulkan type
    if (!typeData.postfix.empty())
    {
      assert(typeData.postfix.back() == '*');
      // it's a pointer -> need to reinterpret_cast it
      argument += writeReinterpretCast(typeData.prefix.find("const") == 0, typeData.type, typeData.postfix.find("* const") != std::string::npos);
    }
    else
    {
      // it's a value -> need to static_cast ist
      argument = "static_cast<" + typeData.type + ">";
    }
    argument += "( " + name + " )";
  }
  else
  {
    // it's a non-vulkan type -> just use it
    argument = name;
  }
  return argument;
}

bool VulkanHppGenerator::writeFunctionHeaderArgumentEnhanced(std::ostream & os, ParamData const& param, size_t paramIndex, std::map<size_t, size_t> const& vectorParamIndices, bool skip, bool argEncountered, bool isTemplateParam, bool isLastArgument, bool singular, bool withDefaults, bool withAllocator) const
{
  if (!skip)
  {
    if (argEncountered)
    {
      os << ", ";
    }
    std::string strippedParameterName = startLowerCase(stripPrefix(param.name, "p"));

    std::map<size_t, size_t>::const_iterator it = vectorParamIndices.find(paramIndex);
    if (it == vectorParamIndices.end())
    {
      // the argument ist not a vector
      if (param.type.postfix.empty())
      {
        // and its not a pointer -> just use its type and name here
        writeFunctionHeaderArgumentEnhancedSimple(os, param, isLastArgument, withDefaults, withAllocator);
      }
      else
      {
        // the argument is not a vector, but a pointer
        assert(param.type.postfix.back() == '*');
        writeFunctionHeaderArgumentEnhancedPointer(os, param, strippedParameterName, withDefaults, withAllocator);
      }
    }
    else
    {
      // the argument is a vector
      writeFunctionHeaderArgumentEnhancedVector(os, param, strippedParameterName, it->second != INVALID_INDEX, isTemplateParam, singular, withDefaults, withAllocator);
    }
    argEncountered = true;
  }
  return argEncountered;
}

void VulkanHppGenerator::writeFunctionHeaderArgumentEnhancedPointer(std::ostream & os, ParamData const& param, std::string const& strippedParameterName, bool withDefaults, bool withAllocator) const
{
  assert(param.type.postfix.back() == '*');
  if (param.optional)
  {
    // for an optional argument, trim the leading 'p' from the name
    os << "Optional<" << param.type.prefix << (param.type.prefix.empty() ? "" : " ") << stripPrefix(param.type.type, "Vk") << "> " << strippedParameterName;
    if (withDefaults && !withAllocator)
    {
      os << " = nullptr";
    }
  }
  else if (param.type.type == "void")
  {
    // for void-pointer, just use type and name
    os << param.type.compose() << " " << param.name;
  }
  else if (param.type.type != "char")
  {
    // for non-char-pointer, change to reference
    assert(param.type.postfix == "*");
    os << param.type.prefix << (param.type.prefix.empty() ? "" : " ") << stripPrefix(param.type.type, "Vk") << " & " << strippedParameterName;
  }
  else
  {
    // for char-pointer, change to const reference to std::string
    os << "const std::string & " << strippedParameterName;
  }
}

void VulkanHppGenerator::writeFunctionHeaderArgumentEnhancedSimple(std::ostream & os, ParamData const& param, bool lastArgument, bool withDefaults, bool withAllocator) const
{
  os << param.type.compose() << " " << param.name;
  if (!param.arraySize.empty())
  {
    os << "[" << param.arraySize << "]";
  }

  if (withDefaults && lastArgument && !withAllocator)
  {
    // check if the very last argument is a flag without any bits -> provide some empty default for it
    std::map<std::string, BitmaskData>::const_iterator bitmasksIt = m_bitmasks.find(param.type.type);
    if (bitmasksIt != m_bitmasks.end())
    {
      // get the enum corresponding to this flag, to check if it's empty
      std::string strippedBitmaskName = stripPrefix(bitmasksIt->first, "Vk");
      std::map<std::string, EnumData>::const_iterator enumIt = m_bitmaskBits.find(bitmasksIt->second.requires);
      assert(enumIt != m_bitmaskBits.end());
      if (enumIt->second.values.empty())
      {
        // there are no bits in this flag -> provide the default
        os << " = " << stripPrefix(param.type.type, "Vk") << "()";
      }
    }
  }
}

void VulkanHppGenerator::writeFunctionHeaderArgumentEnhancedVector(std::ostream & os, ParamData const& param, std::string const& strippedParameterName, bool hasSizeParam, bool isTemplateParam, bool singular, bool withDefaults, bool withAllocator) const
{
  assert(param.type.postfix.back() == '*');

  // it's optional, if it's marked as optional and there's no size specified
  bool optional = param.optional && !hasSizeParam;
  if (param.type.type.find("char") != std::string::npos)
  {
    // it's a char-vector -> use a std::string (either optional or a const-reference
    if (optional)
    {
      os << "Optional<const std::string> " << strippedParameterName;
      if (withDefaults && !withAllocator)
      {
        os << " = nullptr";
      }
    }
    else
    {
      os << "const std::string & " << strippedParameterName;
    }
  }
  else
  {
    // it's a non-char vector (they are never optional)
    assert(!optional);
    if (singular)
    {
      // in singular case, change from pointer to reference
      os << param.type.prefix << (param.type.prefix.empty() ? "" : " ") << stripPrefix(param.type.type, "Vk") << " & " << stripPluralS(strippedParameterName);
    }
    else
    {
      // otherwise, use our ArrayProxy
      bool isConst = (param.type.prefix.find("const") != std::string::npos);
      os << "ArrayProxy<" << (isTemplateParam ? (isConst ? "const T" : "T") : stripPostfix(param.type.compose(), "*")) << "> " << strippedParameterName;
    }
  }
}

void VulkanHppGenerator::writeFunctionHeaderArguments(std::ostream & os, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool enhanced, bool singular, bool withDefaults, bool withAllocator) const
{
  os << "(";
  if (enhanced)
  {
    writeFunctionHeaderArgumentsEnhanced(os, commandData, returnParamIndex, templateParamIndex, vectorParamIndices, singular, withDefaults, withAllocator);
  }
  else
  {
    writeFunctionHeaderArgumentsStandard(os, commandData, withDefaults);
  }
  os << ")";

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  if (!m_commandToHandle.find(commandData.first)->second.empty())
  {
    os << " const";
  }
}

void VulkanHppGenerator::writeFunctionHeaderArgumentsEnhanced(std::ostream & os, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool singular, bool withDefaults, bool withAllocator) const
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

    os << " ";
    bool argEncountered = false;
    for (size_t i = handle.empty() ? 0 : 1; i < commandData.second.params.size(); i++)
    {
      argEncountered = writeFunctionHeaderArgumentEnhanced(os, commandData.second.params[i], i, vectorParamIndices, skippedParams.find(i) != skippedParams.end(), argEncountered, (templateParamIndex == i), (lastArgument == i), singular, withDefaults, withAllocator);
    }

    if (argEncountered)
    {
      os << ", ";
    }
  }
  if (withAllocator)
  {
    os << "Allocator const& vectorAllocator, ";
  }
  os << "Dispatch const &d";
  if (withDefaults && !withAllocator)
  {
    os << " = Dispatch()";
  }

  os << " ";
}

void VulkanHppGenerator::writeFunctionHeaderArgumentsStandard(std::ostream & os, std::pair<std::string, CommandData> const& commandData, bool withDefaults) const
{
  // for the standard case, just list all the arguments as we've got them
  // determine the last argument, where we might provide some default for
  size_t lastArgument = commandData.second.params.size() - 1;

  assert(m_commandToHandle.find(commandData.first) != m_commandToHandle.end());
  std::string const& handle = m_commandToHandle.find(commandData.first)->second;

  bool argEncountered = false;
  for (size_t i = handle.empty() ? 0 : 1; i < commandData.second.params.size(); i++)
  {
    argEncountered = writeFunctionHeaderArgumentStandard(os, commandData.second.params[i], argEncountered, lastArgument == i, withDefaults);
  }
  if (argEncountered)
  {
    os << ", ";
  }

  os << "Dispatch const &d";
  if (withDefaults)
  {
    os << " = Dispatch() ";
  }
}

bool VulkanHppGenerator::writeFunctionHeaderArgumentStandard(std::ostream & os, ParamData const& param, bool argEncountered, bool isLastArgument, bool withDefaults) const
{
  if (argEncountered)
  {
    os << ",";
  }

  os << " " << param.type.compose() << " " << param.name;
  if (!param.arraySize.empty())
  {
    os << "[" << param.arraySize << "]";
  }

  if (withDefaults && isLastArgument)
  {
    // check if the very last argument is a flag without any bits -> provide some empty default for it
    std::map<std::string, BitmaskData>::const_iterator bitmasksIt = m_bitmasks.find(param.type.type);
    if (bitmasksIt != m_bitmasks.end())
    {
      // get the enum corresponding to this flag, to check if it's empty
      std::string strippedBitmaskName = stripPrefix(bitmasksIt->first, "Vk");
      std::map<std::string, EnumData>::const_iterator enumIt = m_bitmaskBits.find(bitmasksIt->second.requires);
      assert(enumIt != m_bitmaskBits.end());
      if (enumIt->second.values.empty())
      {
        // there are no bits in this flag -> provide the default
        os << " = " << stripPrefix(param.type.type, "Vk") << "()";
      }
    }
  }
  return true;
}

void VulkanHppGenerator::writeFunctionHeaderReturnType(std::ostream & os, CommandData const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, std::string const& enhancedReturnType, bool enhanced, bool singular, bool unique, bool isStructureChain) const
{
  std::string templateString;
  std::string returnType;
  if (enhanced)
  {
    // the enhanced function might return some pretty complex return stuff
    bool isVector = (enhancedReturnType.find("Allocator") != std::string::npos);
    if (isStructureChain || (!singular && isVector))
    {
      // for the non-singular case with allocation, we need to prepend with 'typename' to keep compilers happy
      templateString = "typename ";
    }
    if (unique)
    {
      // the unique version returns something prefixed with 'Unique'; potentially a vector of that stuff
      // it's a vector, if it's not the singular version and the return parameter is a vector parameter
      bool returnsVector = !singular && (vectorParamIndices.find(returnParamIndex) != vectorParamIndices.end());

      templateString += returnsVector ? "ResultValueType<std::vector<UniqueHandle<${returnType},Dispatch>,Allocator>>::type " : "typename ResultValueType<UniqueHandle<${returnType},Dispatch>>::type ";
      returnType = isStructureChain ? "StructureChain<X, Y, Z...>" : stripPrefix(commandData.params[returnParamIndex].type.type, "Vk");
    }
    else if ((enhancedReturnType != stripPrefix(commandData.returnType, "Vk")) && (commandData.returnType != "void"))
    {
      // if the enhanced return type differs from the original return type, and it's not void, we return a ResultValueType<...>::type
      templateString += "ResultValueType<${returnType}>::type ";

      assert(commandData.returnType == "VkResult");
      // in singular case, we create the ResultValueType from the pure return type, otherwise from the enhanced return type
      returnType = isStructureChain ? "StructureChain<X, Y, Z...>" : (singular ? stripPrefix(commandData.params[returnParamIndex].type.type, "Vk") : enhancedReturnType);
    }
    else if ((returnParamIndex != INVALID_INDEX) && (1 < commandData.successCodes.size()))
    {
      // if there is a return parameter at all, and there are multiple success codes, we return a ResultValue<...> with the pure return type
      assert(commandData.returnType == "VkResult");
      templateString = "ResultValue<${returnType}> ";
      returnType = isStructureChain ? "StructureChain<X, Y, Z...>" : stripPrefix(commandData.params[returnParamIndex].type.type, "Vk");
    }
    else
    {
      // and in every other case, we just return the enhanced return type.
      templateString = "${returnType} ";
      returnType = isStructureChain && !isVector ? "StructureChain<X, Y, Z...>" : enhancedReturnType;
    }
  }
  else
  {
    // the non-enhanced function just uses the return type
    templateString = "${returnType} ";
    returnType = stripPrefix(commandData.returnType, "Vk");
  }
  os << replaceWithMap(templateString, { { "returnType", returnType } });
}

void VulkanHppGenerator::writeFunctionHeaderTemplate(std::ostream & os, std::string const& indentation, size_t returnParamIndex, size_t templateParamIndex, std::string const& enhancedReturnType, bool enhanced, bool singular, bool unique, bool withDefault, bool isStructureChain) const
{
  bool withAllocator = (enhancedReturnType.find("Allocator") != std::string::npos);
  os << indentation << "template<";
  if (enhanced)
  {
    if (isStructureChain)
    {
      os << "typename " << (withAllocator ? "StructureChain" : "X, typename Y, typename ...Z") << ", ";
    }
    else if ((templateParamIndex != INVALID_INDEX) && ((templateParamIndex != returnParamIndex) || (enhancedReturnType == "Result")))
    {
      assert(!withAllocator);
      os << "typename T, ";
    }
    if (!singular && withAllocator)
    {
      // otherwise, if there's an Allocator used in the enhanced return type, we templatize on that Allocator
      assert((enhancedReturnType.substr(0, 12) == "std::vector<") && (enhancedReturnType.find(',') != std::string::npos) && (12 < enhancedReturnType.find(',')));
      os << "typename Allocator";
      if (withDefault)
      {
        // for the default type get the type from the enhancedReturnType, which is of the form 'std::vector<Type,Allocator>'
        assert(!isStructureChain || !unique);
        os << " = std::allocator<" << (isStructureChain ? "StructureChain" : (unique ? "Unique" : "") + enhancedReturnType.substr(12, enhancedReturnType.find(',') - 12)) << ">";
      }
      os << ", ";
    }
  }
  os << "typename Dispatch" << (withDefault ? " = DispatchLoaderDefault" : "") << ">" << std::endl;
}

void VulkanHppGenerator::writeHandle(std::ostream & os, std::pair<std::string, HandleData> const& handleData, std::set<std::string> & listedHandles) const
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
          writeHandle(os, *handlesIt, listedHandles);
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

          writeUniqueTypes(os, "", { "VkInstance" });
        }
        os << std::endl;
        writePlatformEnter(os, command.second.platform);
        writeCommand(os, "  ", commandName, command, false);
        writePlatformLeave(os, command.second.platform);
      }
    }
    else
    {
      // then write any forward declaration of Deleters used by this handle
      if (!handleData.second.childrenHandles.empty())
      {
        writeUniqueTypes(os, handleData.first, handleData.second.childrenHandles);
      }
      else if (handleData.first == "VkPhysicalDevice")
      {
        // special handling for class Device, as it's created from PhysicalDevice, but destroys itself
        writeUniqueTypes(os, "", { "VkDevice" });
      }

      std::ostringstream commands;
      // list all the commands that are mapped to members of this class
      for (auto const& command : handleData.second.commands)
      {
        commands << std::endl;
        writePlatformEnter(commands, command.second.platform);
        std::string commandName = determineCommandName(command.first, command.second.params[0].type.type);
        writeCommand(commands, "    ", commandName, command, false);

        // special handling for destroy functions which are not aliased.
        if (!command.second.isAlias && (((command.first.substr(2, 7) == "Destroy") && (commandName != "destroy")) || (command.first.substr(2, 4) == "Free")))
        {
          commandName = (command.first.substr(2, 7) == "Destroy") ? "destroy" : "free";
          commands << std::endl;
          writeCommand(commands, "    ", commandName, command, false);
        }
        writePlatformLeave(commands, command.second.platform);
      }

      static const std::string templateString = R"(
  class ${className}
  {
  public:
    VULKAN_HPP_CONSTEXPR ${className}()
      : m_${memberName}(VK_NULL_HANDLE)
    {}

    VULKAN_HPP_CONSTEXPR ${className}( std::nullptr_t )
      : m_${memberName}(VK_NULL_HANDLE)
    {}

    VULKAN_HPP_TYPESAFE_EXPLICIT ${className}( Vk${className} ${memberName} )
      : m_${memberName}( ${memberName} )
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    ${className} & operator=(Vk${className} ${memberName})
    {
      m_${memberName} = ${memberName};
      return *this; 
    }
#endif

    ${className} & operator=( std::nullptr_t )
    {
      m_${memberName} = VK_NULL_HANDLE;
      return *this;
    }

    bool operator==( ${className} const & rhs ) const
    {
      return m_${memberName} == rhs.m_${memberName};
    }

    bool operator!=(${className} const & rhs ) const
    {
      return m_${memberName} != rhs.m_${memberName};
    }

    bool operator<(${className} const & rhs ) const
    {
      return m_${memberName} < rhs.m_${memberName};
    }
${commands}
    VULKAN_HPP_TYPESAFE_EXPLICIT operator Vk${className}() const
    {
      return m_${memberName};
    }

    explicit operator bool() const
    {
      return m_${memberName} != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_${memberName} == VK_NULL_HANDLE;
    }

  private:
    Vk${className} m_${memberName};
  };
  static_assert( sizeof( ${className} ) == sizeof( Vk${className} ), "handle and wrapper have different size!" );
)";

      os << replaceWithMap(templateString, {
        { "className", stripPrefix(handleData.first, "Vk") },
        { "memberName", startLowerCase(stripPrefix(handleData.first, "Vk")) },
        { "commands", commands.str() }
      });

      if (!handleData.second.alias.empty())
      {
        os << "  using " << stripPrefix(handleData.second.alias, "Vk") << " = " << stripPrefix(handleData.first, "Vk") << ";" << std::endl;
      }
    }
  }
}

void VulkanHppGenerator::writeHandles(std::ostream & os) const
{
  std::set<std::string> listedHandles;
  for (auto const& handle : m_handles)
  {
    writeHandle(os, handle, listedHandles);
  }
}

void VulkanHppGenerator::writeHandlesCommandDefintions(std::ostream & os) const
{
  for (auto const& handle : m_handles)
  {
    // finally the commands, that are member functions of this handle
    for (auto const& command : handle.second.commands)
    {
      os << std::endl;
      writePlatformEnter(os, command.second.platform);
      std::string commandName = determineCommandName(command.first, command.second.params[0].type.type);
      std::string strippedName = startLowerCase(stripPrefix(command.first, "vk"));
      writeCommand(os, "  ", commandName, command, true);

      // special handling for destroy functions
      if (!command.second.isAlias && (((command.first.substr(2, 7) == "Destroy") && (commandName != "destroy")) || (command.first.substr(2, 4) == "Free")))
      {
        commandName = (command.first.substr(2, 7) == "Destroy") ? "destroy" : "free";
        os << std::endl;
        writeCommand(os, "  ", commandName, command, true);
      }
      writePlatformLeave(os, command.second.platform);
    }
  }
  os << std::endl;
}

// Intended only for `enum class Result`!
void VulkanHppGenerator::writeResultExceptions(std::ostream & os) const
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
    if (beginsWith(value.second, "eError"))
    {
      os << replaceWithMap(templateString,
      {
        { "className", stripPrefix(value.second, "eError") + "Error" },
        { "enumName", stripPrefix(enumData->first, "Vk") },
        { "enumMemberName", value.second }
      });
    }
  }
  os << std::endl;
}

void VulkanHppGenerator::writePlatformEnter(std::ostream & os, std::string const& platform) const
{
  if (!platform.empty())
  {
    auto it = m_platforms.find(platform);
    assert((it != m_platforms.end()) && !it->second.empty());
    os << "#ifdef " << it->second << std::endl;
  }
}

void VulkanHppGenerator::writePlatformLeave(std::ostream & os, std::string const& platform) const
{
  if (!platform.empty())
  {
    auto it = m_platforms.find(platform);
    assert((it != m_platforms.end()) && !it->second.empty());
    os << "#endif /*" << it->second << "*/" << std::endl;
  }
}

void VulkanHppGenerator::writeStruct(std::ostream & os, std::pair<std::string, StructureData> const& structure, std::set<std::string> & listedStructures) const
{
  if (listedStructures.find(structure.first) == listedStructures.end())
  {
    listedStructures.insert(structure.first);
    for (auto const& member : structure.second.members)
    {
      auto structureIt = m_structures.find(member.type.type);
      if ((structureIt != m_structures.end()) && (listedStructures.find(member.type.type) == listedStructures.end()))
      {
        writeStruct(os, *structureIt, listedStructures);
      }
    }
    if (!structure.second.subStruct.empty())
    {
      auto structureIt = m_structures.find(structure.second.subStruct);
      if ((structureIt != m_structures.end()) && (listedStructures.find(structureIt->first) == listedStructures.end()))
      {
        writeStruct(os, *structureIt, listedStructures);
      }
    }

    if (structure.second.isUnion)
    {
      writeUnion(os, structure);
    }
    else
    {
      writeStructure(os, structure);
    }
  }
}

void VulkanHppGenerator::writeStructCompareOperators(std::ostream & os, std::pair<std::string, StructureData> const& structData) const
{
  // two structs are compared by comparing each of the elements
  std::string compareMembers;
  std::string intro = "";
  for (auto const& member : structData.second.members)
  {
    const std::string compareMemberTemplate = std::string("${intro}") + (member.arraySize.empty() ? "( ${name} == rhs.${name} )" : "( memcmp( ${name}, rhs.${name}, ${size} * sizeof( ${type} ) ) == 0 )");
    compareMembers += replaceWithMap(compareMemberTemplate,
    {
      { "intro", intro },
      { "name", member.name },
      { "size", member.arraySize },
      { "type", member.type.compose() }
    });
    intro = "\n          && ";
  }

  static const std::string compareTemplate = R"(
    bool operator==( ${name} const& rhs ) const
    {
      return ${compareMembers};
    }

    bool operator!=( ${name} const& rhs ) const
    {
      return !operator==( rhs );
    }
)";

  os << replaceWithMap(compareTemplate, { { "name", stripPrefix(structData.first, "Vk") }, { "compareMembers", compareMembers } });
}

void VulkanHppGenerator::writeStructConstructor(std::ostream & os, std::pair<std::string, StructureData> const& structData) const
{
  // the constructor with all the elements as arguments, with defaults
  std::string ctorOpening = "    " + stripPrefix(structData.first, "Vk") + "( ";
  std::string indentation(ctorOpening.size(), ' ');

  std::string arguments, initializers, copyOps;
  bool listedArgument = false;
  bool firstArgument = true;
  for (auto const& member : structData.second.members)
  {
    // gather the arguments
    arguments += writeStructConstructorArgument(listedArgument, indentation, member);

    // gather the initializers; skip members 'pNext' and 'sType', they are directly set by initializers
    if ((member.name != "pNext") && (member.name != "sType"))
    {
      if (member.arraySize.empty())
      {
        // here, we can only handle non-array arguments
        static const std::string templateString = "      ${sep} ${memberName}( ${value} )\n";
        initializers += replaceWithMap(templateString, { { "sep", firstArgument ? ":" : "," },{ "memberName", member.name },{ "value", member.name + "_" } });
        firstArgument = false;
      }
      else
      {
        // here we can handle the arrays, copying over from argument (with trailing '_') to member
        // size is arraySize times sizeof type
        static const std::string templateString = "\n      memcpy( &${member}, ${member}_.data(), ${arraySize} * sizeof( ${type} ) );";
        copyOps += replaceWithMap(templateString, { { "member", member.name },{ "arraySize", member.arraySize },{ "type", member.type.compose() } });
      }
    }
  }
  if (!copyOps.empty())
  {
    copyOps += "\n    ";
  }

  std::ostringstream subStructConstructor;
  if (!structData.second.subStruct.empty())
  {
    auto const& subStruct = m_structures.find(structData.second.subStruct);
    assert(subStruct != m_structures.end());

    std::string subStructArgumentName = startLowerCase(stripPrefix(subStruct->first, "Vk"));
    ctorOpening = "    explicit " + stripPrefix(structData.first, "Vk") + "( ";
    indentation = std::string(ctorOpening.size(), ' ');

    static const std::string copyTemplate = "      ${sep} ${member}( ${value} )\n";
    std::string subCopies;
    firstArgument = true;
    for (size_t i = 0; i < subStruct->second.members.size(); i++)
    {
      assert(structData.second.members[i].arraySize.empty());
      subCopies += replaceWithMap(copyTemplate,
      {
        { "sep", firstArgument ? ":" : "," },
        { "member", structData.second.members[i].name },
        { "value", subStructArgumentName + "." + subStruct->second.members[i].name }
      });
      firstArgument = false;
    }

    std::string subArguments;
    listedArgument = true;
    for (size_t i = subStruct->second.members.size(); i < structData.second.members.size(); i++)
    {
      subArguments += writeStructConstructorArgument(listedArgument, indentation, structData.second.members[i]);

      assert(structData.second.members[i].arraySize.empty());
      subCopies += replaceWithMap(copyTemplate, { { "sep", "," }, { "member", structData.second.members[i].name }, { "value", structData.second.members[i].name + "_" } });
    }

    std::string constructorTemplate = R"(
    explicit ${type}( ${subType} const& ${subTypeName}${subArguments} )
${subCopies}    {}
)";
    subStructConstructor << replaceWithMap(constructorTemplate,
    {
      { "type", stripPrefix(structData.first, "Vk") },
      { "subType", stripPrefix(subStruct->first, "Vk") },
      { "subTypeName", subStructArgumentName },
      { "subArguments", subArguments },
      { "subCopies", subCopies }
    });
  }

  static std::string constructorTemplate = R"(    ${name}( ${arguments} )
${initializers}    {${copyOps}}
${subStructConstructor}
    ${name}( Vk${name} const & rhs )
    {
      memcpy( this, &rhs, sizeof( ${name} ) );
    }

    ${name}& operator=( Vk${name} const & rhs )
    {
      memcpy( this, &rhs, sizeof( ${name} ) );
      return *this;
    }
)";

  os << replaceWithMap(constructorTemplate,
  {
    { "name", stripPrefix(structData.first, "Vk") },
    { "arguments", arguments },
    { "initializers", initializers },
    { "copyOps", copyOps },
    { "subStructConstructor", subStructConstructor.str() }
  });
}

std::string VulkanHppGenerator::writeStructConstructorArgument(bool & listedArgument, std::string const& indentation, MemberData const& memberData) const
{
  std::string argument;

  // skip members 'pNext' and 'sType', as they are never explicitly set
  if ((memberData.name != "pNext") && (memberData.name != "sType"))
  {
    static const std::string simpleArgumentTemplate = "${type} ${name} = ${value}";
    static const std::string arrayArgumentTemplate = "std::array<${type},${size}> const& ${name} = { { ${value} } }";
    const std::string argumentTemplate = "${intro}" + (memberData.arraySize.empty() ? simpleArgumentTemplate : arrayArgumentTemplate);
    argument = replaceWithMap(argumentTemplate,
    {
      { "intro", listedArgument ? (",\n" + indentation) : "" },
      { "type", memberData.type.compose() },
      { "size", memberData.arraySize },
      { "name", memberData.name + "_" },
      { "value", (memberData.arraySize.empty() && (!memberData.type.postfix.empty() && memberData.type.postfix.back() == '*')) ? "nullptr" : defaultValue(memberData.type.type) }
    });

    listedArgument = true;
  }
  return argument;
}

void VulkanHppGenerator::writeStructMembers(std::ostream & os, StructureData const& structData) const
{
  auto enumIt = m_enums.find("VkStructureType");
  assert(enumIt != m_enums.end());
  for (auto const& member : structData.members)
  {
    if (member.type.type == "VkStructureType")
    {
      assert(member.name == "sType");
      if (!member.values.empty())
      {
        assert(!member.values.empty() && beginsWith(member.values, "VK_STRUCTURE_TYPE"));
        auto nameIt = std::find_if(enumIt->second.values.begin(), enumIt->second.values.end(), [&member](std::pair<std::string, std::string> const& value) { return member.values == value.first; });
        assert(nameIt != enumIt->second.values.end());
        os << "  private:" << std::endl
          << "    StructureType sType = StructureType::" << nameIt->second << ";" << std::endl
          << std::endl
          << "  public:" << std::endl;
      }
      else
      {
        os << "    StructureType sType;" << std::endl;
      }
    }
    else
    {
      os << "    " << member.type.compose() << " " << member.name;
      if (member.name == "pNext")
      {
        os << " = nullptr";
      }
      else if (!member.arraySize.empty())
      {
        os << "[" << member.arraySize << "]";
      }
      os << ";" << std::endl;
    }
  }
}

void VulkanHppGenerator::writeStructs(std::ostream & os) const
{
  std::set<std::string> listedStructures;
  for (auto const& structure : m_structures)
  {
    writeStruct(os, structure, listedStructures);
  }
}

void VulkanHppGenerator::writeStructSetter(std::ostream & os, std::string const& structureName, MemberData const& memberData) const
{
  if (memberData.type.type != "VkStructureType") // filter out StructureType, which is supposed to be immutable !
  {
    // copy over the argument, either by assigning simple data, or by memcpy array data
    const std::string copyOpTemplate = memberData.arraySize.empty() ? "${name} = ${name}_" : "memcpy( ${name}, ${name}_.data(), ${size} * sizeof( ${type} ) )";
    std::string copyOp = replaceWithMap(copyOpTemplate, { { "name", memberData.name }, { "size", memberData.arraySize }, { "type", memberData.type.compose() } });

    // the setters return a reference to the structure
    static const std::string setterTemplate = R"(
    ${structureName} & ${functionName}( ${memberType} ${memberName}_ )
    {
      ${copyOp};
      return *this;
    }
)";

    os << replaceWithMap(setterTemplate,
    {
      { "structureName", structureName },
      { "functionName", "set" + startUpperCase(memberData.name) },
      { "memberType", memberData.arraySize.empty() ? memberData.type.compose() : "std::array<" + memberData.type.compose() + "," + memberData.arraySize + ">" },
      { "memberName", memberData.name },
      { "copyOp", copyOp }
    });
  }
}

void VulkanHppGenerator::writeStructure(std::ostream & os, std::pair<std::string, StructureData> const& structure) const
{
  // only structs that are not returnedOnly get a constructor!
  std::ostringstream constructorAndSetters;
  if (!structure.second.returnedOnly)
  {
    constructorAndSetters << std::endl;
    writeStructConstructor(constructorAndSetters, structure);
    for (auto const& member : structure.second.members)
    {
      writeStructSetter(constructorAndSetters, stripPrefix(structure.first, "Vk"), member);
    }
  }

  // operator==() and operator!=()
  // only structs without a union as a member can have a meaningfull == and != operation; we filter them out
  std::ostringstream compareOperators;
  if (!containsUnion(structure.first))
  {
    writeStructCompareOperators(compareOperators, structure);
  }

  // the member variables
  std::ostringstream members;
  writeStructMembers(members, structure.second);

  static const std::string structureTemplate = 
    R"(  struct ${name}
  {${constructorAndSetters}
    operator ${vkName} const&() const
    {
      return *reinterpret_cast<const ${vkName}*>( this );
    }

    operator ${vkName} &()
    {
      return *reinterpret_cast<${vkName}*>( this );
    }
${compareOperators}
${members}  };
  static_assert( sizeof( ${name} ) == sizeof( ${vkName} ), "struct and wrapper have different size!" );
)";

  os << std::endl;
  writePlatformEnter(os, structure.second.platform);
  os << replaceWithMap(structureTemplate,
  {
    { "name", stripPrefix(structure.first, "Vk") },
    { "constructorAndSetters", constructorAndSetters.str() },
    { "vkName", structure.first },
    { "compareOperators", compareOperators.str() },
    { "members", members.str() },
  });
  writePlatformLeave(os, structure.second.platform);
}

void VulkanHppGenerator::writeStructureChainValidation(std::ostream & os)
{
  // write all template functions for the structure pointer chain validation
  for (auto const& structure : m_structures)
  {
    if (!structure.second.structExtends.empty())
    {
      writePlatformEnter(os, structure.second.platform);

      // write out allowed structure chains
      for (auto extendName : structure.second.structExtends)
      {
        std::map<std::string, StructureData>::const_iterator itExtend = m_structures.find(extendName);
        if (itExtend == m_structures.end())
        {
          std::stringstream errorString;
          errorString << extendName << " does not specify a struct in structextends field.";

          // check if symbol name is an alias to a struct
          auto itAlias = std::find_if(m_structures.begin(), m_structures.end(), [&extendName](std::pair<std::string, StructureData> const &it) -> bool {return std::find(it.second.aliases.begin(), it.second.aliases.end(), extendName) != it.second.aliases.end(); });
          if (itAlias != m_structures.end())
          {
            errorString << " The symbol is an alias and maps to " << itAlias->first << ".";
          }

          errorString << std::endl;
          throw std::runtime_error(errorString.str());
        }
        if (structure.second.platform != itExtend->second.platform)
        {
          writePlatformEnter(os, itExtend->second.platform);
        }

        os << "  template <> struct isStructureChainValid<" << stripPrefix(extendName, "Vk") << ", " << stripPrefix(structure.first, "Vk") << ">{ enum { value = true }; };" << std::endl;

        if (structure.second.platform != itExtend->second.platform)
        {
          writePlatformLeave(os, itExtend->second.platform);
        }
      }
      writePlatformLeave(os, structure.second.platform);
    }
  }
}

void VulkanHppGenerator::writeThrowExceptions(std::ostream & os) const
{
  auto enumData = m_enums.find("VkResult");

  std::string casesString;
  for (auto const& value : enumData->second.values)
  {
    if (beginsWith(value.second, "eError"))
    {
      casesString += "      case Result::" + value.second + ": throw " + stripPrefix(value.second, "eError") + "Error( message );\n";
    }
  }

  static const std::string exceptionTemplate = R"(
  VULKAN_HPP_INLINE void throwResultException( Result result, char const * message )
  {
    switch ( result )
    {
${cases}      default: throw SystemError( make_error_code( result ) );
    }
  }
)";
  os << replaceWithMap(exceptionTemplate, { { "cases", casesString } });
}

void VulkanHppGenerator::writeUnion(std::ostream & os, std::pair<std::string, StructureData> const& structure) const
{
  std::ostringstream oss;
  os << std::endl
    << "  union " << stripPrefix(structure.first, "Vk") << std::endl
    << "  {" << std::endl;

  bool firstTime = true;
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

    // one constructor per union element
    os << "    " << stripPrefix(structure.first, "Vk") << "( " << (member.arraySize.empty() ? (member.type.compose() + " ") : ("const std::array<" + member.type.compose() + "," + member.arraySize + ">& ")) << member.name << "_";

    // just the very first constructor gets default arguments
    if (firstTime)
    {
      std::string value = defaultValue(member.type.type);
      os << (member.arraySize.empty() ? (" = " + value) : (" = { { " + value + " } }"));
      firstTime = false;
    }
    os << " )" << std::endl
      << "    {" << std::endl
      << "      " << (member.arraySize.empty() ? (member.name + " = " + member.name + "_") : ("memcpy( " + member.name + ", " + member.name + "_.data(), " + member.arraySize + " * sizeof( " + member.type.compose() + " ) )")) << ";" << std::endl
      << "    }" << std::endl
      << std::endl;
  }

  // one setter per union element
  assert(!structure.second.returnedOnly);
  for (auto const& member : structure.second.members)
  {
    writeStructSetter(os, stripPrefix(structure.first, "Vk"), member);
  }

  // the implicit cast operators to the native type
  os << "    operator " << structure.first << " const&() const" << std::endl
    << "    {" << std::endl
    << "      return *reinterpret_cast<const " << structure.first << "*>(this);" << std::endl
    << "    }" << std::endl
    << std::endl
    << "    operator " << structure.first << " &()" << std::endl
    << "    {" << std::endl
    << "      return *reinterpret_cast<" << structure.first << "*>(this);" << std::endl
    << "    }" << std::endl
    << std::endl;

  // the union member variables
  // if there's at least one Vk... type in this union, check for unrestricted unions support
  bool needsUnrestrictedUnions = (std::find_if(structure.second.members.begin(), structure.second.members.end(), [](MemberData const& member) { return beginsWith(member.type.type, "Vk"); }) != structure.second.members.end());
  if (needsUnrestrictedUnions)
  {
    os << "#ifdef VULKAN_HPP_HAS_UNRESTRICTED_UNIONS" << std::endl;
    for (auto const& member : structure.second.members)
    {
      os << "    " << member.type.compose() << " " << member.name << (member.arraySize.empty() ? "" : ("[" + member.arraySize + "]")) << ";" << std::endl;
    }
    os << "#else" << std::endl;
  }
  for (auto const& member : structure.second.members)
  {
    os << "    " << (beginsWith(member.type.type, "Vk") ? "Vk" : "") << member.type.compose() << " " << member.name << (member.arraySize.empty() ? "" : ("[" + member.arraySize + "]")) << ";" << std::endl;
  }
  if (needsUnrestrictedUnions)
  {
    os << "#endif  /*VULKAN_HPP_HAS_UNRESTRICTED_UNIONS*/" << std::endl;
  }
  os << "  };" << std::endl;
}

void VulkanHppGenerator::writeUniqueTypes(std::ostream &os, std::string const& parentType, std::set<std::string> const& childrenTypes) const
{
  os << std::endl
    << "#ifndef VULKAN_HPP_NO_SMART_HANDLE";
  if (!parentType.empty())
  {
    os << std::endl
      << "  class " << stripPrefix(parentType, "Vk") << ";";
  }

  for (auto const& childType : childrenTypes)
  {
    auto handleIt = m_handles.find(childType);
    assert(handleIt != m_handles.end());

    static const std::string uniqueTypesTemplate = R"(
  template <typename Dispatch> class UniqueHandleTraits<${type}, Dispatch> { public: using deleter = ${deleterType}${deleterAction}<${deleterParent}${deleterPool}, Dispatch>; };
  using Unique${type} = UniqueHandle<${type}, DispatchLoaderDefault>;)";

    os << replaceWithMap(uniqueTypesTemplate,
    {
      { "type", stripPrefix(childType, "Vk") },
      { "deleterType", handleIt->second.deletePool.empty() ? "Object" : "Pool" },
      { "deleterAction", (handleIt->second.deleteCommand.substr(2, 4) == "Free") ? "Free" : "Destroy" },
      { "deleterParent", parentType.empty() ? "NoParent" : stripPrefix(parentType, "Vk") },
      { "deleterPool", handleIt->second.deletePool.empty() ? "" : ", " + stripPrefix(handleIt->second.deletePool, "Vk") }
    });
  }
  os << std::endl
    << "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/" << std::endl;
}

void VulkanHppGenerator::EnumData::addEnumValue(std::string const &valueName, bool bitmask, std::string const& prefix, std::string const& postfix, std::string const& tag)
{
  std::string translatedName = createEnumValueName(valueName, prefix, postfix, bitmask, tag);

  auto it = std::find_if(values.begin(), values.end(), [&translatedName](std::pair<std::string, std::string> const& value) { return value.second == translatedName; });
  if (it == values.end())
  {
    values.push_back(std::make_pair(valueName, translatedName));
  }
  else
  {
    assert(it->first == valueName);
  }
}

std::string VulkanHppGenerator::TypeData::compose() const
{
  return prefix + (prefix.empty() ? "" : " ") + stripPrefix(type, "Vk") + postfix;
}

int main( int argc, char **argv )
{
  static const std::string classArrayProxy = R"(
#if !defined(VULKAN_HPP_DISABLE_ENHANCED_MODE)
  template <typename T>
  class ArrayProxy
  {
  public:
    VULKAN_HPP_CONSTEXPR ArrayProxy(std::nullptr_t)
      : m_count(0)
      , m_ptr(nullptr)
    {}

    ArrayProxy(T & ptr)
      : m_count(1)
      , m_ptr(&ptr)
    {}

    ArrayProxy(uint32_t count, T * ptr)
      : m_count(count)
      , m_ptr(ptr)
    {}

    template <size_t N>
    ArrayProxy(std::array<typename std::remove_const<T>::type, N> & data)
      : m_count(N)
      , m_ptr(data.data())
    {}

    template <size_t N>
    ArrayProxy(std::array<typename std::remove_const<T>::type, N> const& data)
      : m_count(N)
      , m_ptr(data.data())
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxy(std::vector<typename std::remove_const<T>::type, Allocator> & data)
      : m_count(static_cast<uint32_t>(data.size()))
      , m_ptr(data.data())
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxy(std::vector<typename std::remove_const<T>::type, Allocator> const& data)
      : m_count(static_cast<uint32_t>(data.size()))
      , m_ptr(data.data())
    {}

    ArrayProxy(std::initializer_list<T> const& data)
      : m_count(static_cast<uint32_t>(data.end() - data.begin()))
      , m_ptr(data.begin())
    {}

    const T * begin() const
    {
      return m_ptr;
    }

    const T * end() const
    {
      return m_ptr + m_count;
    }

    const T & front() const
    {
      VULKAN_HPP_ASSERT(m_count && m_ptr);
      return *m_ptr;
    }

    const T & back() const
    {
      VULKAN_HPP_ASSERT(m_count && m_ptr);
      return *(m_ptr + m_count - 1);
    }

    bool empty() const
    {
      return (m_count == 0);
    }

    uint32_t size() const
    {
      return m_count;
    }

    T * data() const
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
    VULKAN_HPP_CONSTEXPR Flags()
      : m_mask(0)
    {
    }

    Flags(BitType bit)
      : m_mask(static_cast<MaskType>(bit))
    {
    }

    Flags(Flags<BitType> const& rhs)
      : m_mask(rhs.m_mask)
    {
    }

    explicit Flags(MaskType flags)
      : m_mask(flags)
    {
    }

    Flags<BitType> & operator=(Flags<BitType> const& rhs)
    {
      m_mask = rhs.m_mask;
      return *this;
    }

    Flags<BitType> & operator|=(Flags<BitType> const& rhs)
    {
      m_mask |= rhs.m_mask;
      return *this;
    }

    Flags<BitType> & operator&=(Flags<BitType> const& rhs)
    {
      m_mask &= rhs.m_mask;
      return *this;
    }

    Flags<BitType> & operator^=(Flags<BitType> const& rhs)
    {
      m_mask ^= rhs.m_mask;
      return *this;
    }

    Flags<BitType> operator|(Flags<BitType> const& rhs) const
    {
      Flags<BitType> result(*this);
      result |= rhs;
      return result;
    }

    Flags<BitType> operator&(Flags<BitType> const& rhs) const
    {
      Flags<BitType> result(*this);
      result &= rhs;
      return result;
    }

    Flags<BitType> operator^(Flags<BitType> const& rhs) const
    {
      Flags<BitType> result(*this);
      result ^= rhs;
      return result;
    }

    bool operator!() const
    {
      return !m_mask;
    }

    Flags<BitType> operator~() const
    {
      Flags<BitType> result(*this);
      result.m_mask ^= FlagTraits<BitType>::allFlags;
      return result;
    }

    bool operator==(Flags<BitType> const& rhs) const
    {
      return m_mask == rhs.m_mask;
    }

    bool operator!=(Flags<BitType> const& rhs) const
    {
      return m_mask != rhs.m_mask;
    }

    explicit operator bool() const
    {
      return !!m_mask;
    }

    explicit operator MaskType() const
    {
        return m_mask;
    }

  private:
    MaskType  m_mask;
  };

  template <typename BitType>
  Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags)
  {
    return flags | bit;
  }

  template <typename BitType>
  Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags)
  {
    return flags & bit;
  }

  template <typename BitType>
  Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags)
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
      ObjectDestroy( OwnerType owner = OwnerType(), Optional<const AllocationCallbacks> allocationCallbacks = nullptr, Dispatch const &dispatch = Dispatch() )
        : m_owner( owner )
        , m_allocationCallbacks( allocationCallbacks )
        , m_dispatch( &dispatch )
      {}

      OwnerType getOwner() const { return m_owner; }
      Optional<const AllocationCallbacks> getAllocator() const { return m_allocationCallbacks; }

    protected:
      template <typename T>
      void destroy(T t)
      {
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
      ObjectDestroy( Optional<const AllocationCallbacks> allocationCallbacks = nullptr, Dispatch const &dispatch = Dispatch() )
        : m_allocationCallbacks( allocationCallbacks )
        , m_dispatch( &dispatch )
      {}

      Optional<const AllocationCallbacks> getAllocator() const { return m_allocationCallbacks; }

    protected:
      template <typename T>
      void destroy(T t)
      {
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
      ObjectFree( OwnerType owner = OwnerType(), Optional<const AllocationCallbacks> allocationCallbacks = nullptr, Dispatch const &dispatch = Dispatch() )
        : m_owner( owner )
        , m_allocationCallbacks( allocationCallbacks )
        , m_dispatch( &dispatch )
      {}

      OwnerType getOwner() const { return m_owner; }
      Optional<const AllocationCallbacks> getAllocator() const { return m_allocationCallbacks; }

    protected:
      template <typename T>
      void destroy(T t)
      {
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
    Optional(RefType & reference) { m_ptr = &reference; }
    Optional(RefType * ptr) { m_ptr = ptr; }
    Optional(std::nullptr_t) { m_ptr = nullptr; }

    operator RefType*() const { return m_ptr; }
    RefType const* operator->() const { return m_ptr; }
    explicit operator bool() const { return !!m_ptr; }

  private:
    RefType *m_ptr;
  };
)";

  static const std::string classPoolFree = R"(
  template <typename OwnerType, typename PoolType, typename Dispatch>
  class PoolFree
  {
    public:
      PoolFree( OwnerType owner = OwnerType(), PoolType pool = PoolType(), Dispatch const &dispatch = Dispatch() )
        : m_owner( owner )
        , m_pool( pool )
        , m_dispatch( &dispatch )
      {}

      OwnerType getOwner() const { return m_owner; }
      PoolType getPool() const { return m_pool; }

    protected:
      template <typename T>
      void destroy(T t)
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

  template <class Element>
  class StructureChainElement
  {
  public:
    explicit operator Element&() { return value; }
    explicit operator const Element&() const { return value; }
  private:
    Element value;
  };

  template<typename ...StructureElements>
  class StructureChain : private StructureChainElement<StructureElements>...
  {
  public:
    StructureChain()
    {
      link<void, StructureElements...>();  
    }

    StructureChain(StructureChain const &rhs)
    {
      linkAndCopy<void, StructureElements...>(rhs);
    }

    StructureChain(StructureElements const &... elems)
    {
      linkAndCopyElements<void, StructureElements...>(elems...);
    }

    StructureChain& operator=(StructureChain const &rhs)
    {
      linkAndCopy<void, StructureElements...>(rhs);
      return *this;
    }

    template<typename ClassType> ClassType& get() { return static_cast<ClassType&>(*this);}

    template<typename ClassTypeA, typename ClassTypeB, typename ...ClassTypes>
    std::tuple<ClassTypeA, ClassTypeB, ClassTypes...> get()
    {
        return std::tuple_cat(
            std::make_tuple(get<ClassTypeA>(),get<ClassTypeB>()),
            std::make_tuple(get<ClassTypes>()...)
        );
    }

  private:
    template<typename List, typename X>
    void link()
    {
      static_assert(extendCheck<List, X>::valid, "The structure chain is not valid!");
    }

    template<typename List, typename X, typename Y, typename ...Z>
    void link()
    {
      static_assert(extendCheck<List,X>::valid, "The structure chain is not valid!");
      X& x = static_cast<X&>(*this);
      Y& y = static_cast<Y&>(*this);
      x.pNext = &y;
      link<TypeList<List, X>, Y, Z...>();
    }

    template<typename List, typename X>
    void linkAndCopy(StructureChain const &rhs)
    {
      static_assert(extendCheck<List, X>::valid, "The structure chain is not valid!");
      static_cast<X&>(*this) = static_cast<X const &>(rhs);
    }

    template<typename List, typename X, typename Y, typename ...Z>
    void linkAndCopy(StructureChain const &rhs)
    {
      static_assert(extendCheck<List, X>::valid, "The structure chain is not valid!");
      X& x = static_cast<X&>(*this);
      Y& y = static_cast<Y&>(*this);
      x = static_cast<X const &>(rhs);
      x.pNext = &y;
      linkAndCopy<TypeList<List, X>, Y, Z...>(rhs);
    }

    template<typename List, typename X>
    void linkAndCopyElements(X const &xelem)
    {
      static_assert(extendCheck<List, X>::valid, "The structure chain is not valid!");
      static_cast<X&>(*this) = xelem;
    }

    template<typename List, typename X, typename Y, typename ...Z>
    void linkAndCopyElements(X const &xelem, Y const &yelem, Z const &... zelem)
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

    explicit UniqueHandle( Type const& value = Type(), Deleter const& deleter = Deleter() )
      : Deleter( deleter)
      , m_value( value )
    {}

    UniqueHandle( UniqueHandle const& ) = delete;

    UniqueHandle( UniqueHandle && other )
      : Deleter( std::move( static_cast<Deleter&>( other ) ) )
      , m_value( other.release() )
    {}

    ~UniqueHandle()
    {
      if ( m_value ) this->destroy( m_value );
    }

    UniqueHandle & operator=( UniqueHandle const& ) = delete;

    UniqueHandle & operator=( UniqueHandle && other )
    {
      reset( other.release() );
      *static_cast<Deleter*>(this) = std::move( static_cast<Deleter&>(other) );
      return *this;
    }

    explicit operator bool() const
    {
      return m_value.operator bool();
    }

    Type const* operator->() const
    {
      return &m_value;
    }

    Type * operator->()
    {
      return &m_value;
    }

    Type const& operator*() const
    {
      return m_value;
    }

    Type & operator*()
    {
      return m_value;
    }

    const Type & get() const
    {
      return m_value;
    }
    
    Type & get()
    {
      return m_value;
    }

    void reset( Type const& value = Type() )
    {
      if ( m_value != value )
      {
        if ( m_value ) this->destroy( m_value );
        m_value = value;
      }
    }

    Type release()
    {
      Type value = m_value;
      m_value = nullptr;
      return value;
    }

    void swap( UniqueHandle<Type,Dispatch> & rhs )
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
  VULKAN_HPP_INLINE void swap( UniqueHandle<Type,Dispatch> & lhs, UniqueHandle<Type,Dispatch> & rhs )
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
// with the vk::MemoryBarrier struct.
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
# if defined(__clang___)
#  if __has_attribute(always_inline)
#   define VULKAN_HPP_INLINE __attribute__((always_inline)) __inline__
#  else
#    define VULKAN_HPP_INLINE inline
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

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
# define VULKAN_HPP_CONSTEXPR
#else
# define VULKAN_HPP_CONSTEXPR constexpr
#endif

#if !defined(VULKAN_HPP_NAMESPACE)
#define VULKAN_HPP_NAMESPACE vk
#endif

#define VULKAN_HPP_STRINGIFY2(text) #text
#define VULKAN_HPP_STRINGIFY(text) VULKAN_HPP_STRINGIFY2(text)
#define VULKAN_HPP_NAMESPACE_STRING VULKAN_HPP_STRINGIFY(VULKAN_HPP_NAMESPACE)
)";

  static const std::string exceptions = R"(
#if defined(_MSC_VER) && (_MSC_VER == 1800)
# define noexcept _NOEXCEPT
#endif

  class ErrorCategoryImpl : public std::error_category
  {
    public:
    virtual const char* name() const noexcept override { return VULKAN_HPP_NAMESPACE_STRING"::Result"; }
    virtual std::string message(int ev) const override { return to_string(static_cast<Result>(ev)); }
  };

  class Error
  {
    public:
    virtual ~Error() = default;

    virtual const char* what() const noexcept = 0;
  };

  class LogicError : public Error, public std::logic_error
  {
    public:
    explicit LogicError( const std::string& what )
      : Error(), std::logic_error(what) {}
    explicit LogicError( char const * what )
      : Error(), std::logic_error(what) {}
    virtual ~LogicError() = default;

    virtual const char* what() const noexcept { return std::logic_error::what(); }
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
    virtual ~SystemError() = default;

    virtual const char* what() const noexcept { return std::system_error::what(); }
  };

#if defined(_MSC_VER) && (_MSC_VER == 1800)
# undef noexcept
#endif

  VULKAN_HPP_INLINE const std::error_category& errorCategory()
  {
    static ErrorCategoryImpl instance;
    return instance;
  }

  VULKAN_HPP_INLINE std::error_code make_error_code(Result e)
  {
    return std::error_code(static_cast<int>(e), errorCategory());
  }

  VULKAN_HPP_INLINE std::error_condition make_error_condition(Result e)
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
)";

  static const std::string is_error_code_enum = R"(
namespace std
{
  template <>
  struct is_error_code_enum<VULKAN_HPP_NAMESPACE::Result> : public true_type
  {};
}
)";

  static const std::string structResultValue = R"(
  template <typename T> void ignore(T const&) {}

  template <typename T>
  struct ResultValue
  {
    ResultValue( Result r, T & v )
      : result( r )
      , value( v )
    {}

    ResultValue( Result r, T && v )
      : result( r )
      , value( std::move( v ) )
    {}

    Result  result;
    T       value;

    operator std::tuple<Result&, T&>() { return std::tuple<Result&, T&>(result, value); }
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
        std::stringstream lineNumber;
        lineNumber << child->GetLineNum();
        std::cerr << "warning: Unhandled tag " << value << " at line number: " << lineNumber.str() << "!" << std::endl;
      }
    }

    generator.checkCorrectness();

    std::ofstream ofs(VULKAN_HPP_FILE);
    ofs << generator.getVulkanLicenseHeader() << std::endl
      << includes
      << std::endl
      << "static_assert( VK_HEADER_VERSION == " << generator.getVersion() << " , \"Wrong VK_HEADER_VERSION!\" );" << std::endl
      << std::endl
      << "// 32-bit vulkan is not typesafe for handles, so don't allow copy constructors on this platform by default." << std::endl
      << "// To enable this feature on 32-bit platforms please define VULKAN_HPP_TYPESAFE_CONVERSION" << std::endl
      << generator.getTypesafeCheck() << std::endl
      << "# if !defined( VULKAN_HPP_TYPESAFE_CONVERSION )" << std::endl
      << "#  define VULKAN_HPP_TYPESAFE_CONVERSION" << std::endl
      << "# endif" << std::endl
      << "#endif" << std::endl
      << defines
      << std::endl
      << "namespace VULKAN_HPP_NAMESPACE" << std::endl
      << "{"
      << classArrayProxy
      << classFlags
      << classOptional
      << classStructureChain
      << classUniqueHandle;
    generator.writeDispatchLoaderDefault(ofs);
    ofs << classObjectDestroy
      << classObjectFree
      << classPoolFree
      << std::endl;
    generator.writeBaseTypes(ofs);
    generator.writeEnums(ofs);
    generator.writeBitmasks(ofs);
    ofs << "} // namespace VULKAN_HPP_NAMESPACE" << std::endl
      << is_error_code_enum
      << std::endl
      << "namespace VULKAN_HPP_NAMESPACE" << std::endl
      << "{" << std::endl
      << "#ifndef VULKAN_HPP_NO_EXCEPTIONS"
      << exceptions;
    generator.writeResultExceptions(ofs);
    generator.writeThrowExceptions(ofs);
    ofs << "#endif" << std::endl;
    ofs << structResultValue;
    generator.writeForwardDeclarations(ofs);
    generator.writeHandles(ofs);
    generator.writeStructs(ofs);
    generator.writeHandlesCommandDefintions(ofs);

    generator.writeStructureChainValidation(ofs);

    generator.writeDispatchLoaderDynamic(ofs);

    ofs << "} // namespace VULKAN_HPP_NAMESPACE" << std::endl
      << "#endif" << std::endl;
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
