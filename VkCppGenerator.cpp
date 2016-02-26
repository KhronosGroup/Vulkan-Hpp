// Copyright(c) 2015-2016, NVIDIA CORPORATION. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <cassert>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <tinyxml2.h>

const std::string licenseHeader(
"// Copyright(c) 2015-2016, NVIDIA CORPORATION. All rights reserved.\n"
"//\n"
"// Redistribution and use in source and binary forms, with or without\n"
"// modification, are permitted provided that the following conditions\n"
"// are met:\n"
"//  * Redistributions of source code must retain the above copyright\n"
"//    notice, this list of conditions and the following disclaimer.\n"
"//  * Redistributions in binary form must reproduce the above copyright\n"
"//    notice, this list of conditions and the following disclaimer in the\n"
"//    documentation and/or other materials provided with the distribution.\n"
"//  * Neither the name of NVIDIA CORPORATION nor the names of its\n"
"//    contributors may be used to endorse or promote products derived\n"
"//    from this software without specific prior written permission.\n"
"//\n"
"// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY\n"
"// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n"
"// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n"
"// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR\n"
"// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,\n"
"// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,\n"
"// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR\n"
"// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY\n"
"// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n"
"// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE\n"
"// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
);

const std::string exceptionHeader(
  "  enum class Result;\n"
  "\n"
  "  class Exception : public std::runtime_error\n"
  "  {\n"
  "  public:\n"
  "    Exception(Result result, std::string const& what)\n"
  "      : std::runtime_error(what)\n"
  "      , m_error(result)\n"
  "    {}\n"
  "\n"
  "  private:\n"
  "    Result m_error;\n"
  "  };\n"
  "\n"
  );

const std::string flagsHeader(
"  template <typename BitType, typename MaskType = uint32_t>\n"
"  class Flags\n"
"  {\n"
"  public:\n"
"    Flags()\n"
"      : m_mask(0)\n"
"    {\n"
"    }\n"
"\n"
"    Flags(BitType bit)\n"
"      : m_mask(static_cast<uint32_t>(bit))\n"
"    {\n"
"    }\n"
"\n"
"    Flags(Flags<BitType> const& rhs)\n"
"      : m_mask(rhs.m_mask)\n"
"    {\n"
"    }\n"
"\n"
"    Flags<BitType> & operator=(Flags<BitType> const& rhs)\n"
"    {\n"
"      m_mask = rhs.m_mask;\n"
"      return *this;\n"
"    }\n"
"\n"
"    Flags<BitType> & operator|=(Flags<BitType> const& rhs)\n"
"    {\n"
"      m_mask |= rhs.m_mask;\n"
"      return *this;\n"
"    }\n"
"\n"
"    Flags<BitType> & operator&=(Flags<BitType> const& rhs)\n"
"    {\n"
"      m_mask &= rhs.m_mask;\n"
"      return *this;\n"
"    }\n"
"\n"
"    Flags<BitType> & operator^=(Flags<BitType> const& rhs)\n"
"    {\n"
"      m_mask ^= rhs.m_mask;\n"
"      return *this;\n"
"    }\n"
"\n"
"    Flags<BitType> operator|(Flags<BitType> const& rhs) const\n"
"    {\n"
"      Flags<BitType> result(*this);\n"
"      result |= rhs;\n"
"      return result;\n"
"    }\n"
"\n"
"    Flags<BitType> operator&(Flags<BitType> const& rhs) const\n"
"    {\n"
"      Flags<BitType> result(*this);\n"
"      result &= rhs;\n"
"      return result;\n"
"    }\n"
"\n"
"    Flags<BitType> operator^(Flags<BitType> const& rhs) const\n"
"    {\n"
"      Flags<BitType> result(*this);\n"
"      result ^= rhs;\n"
"      return result;\n"
"    }\n"
"\n"
"    bool operator!() const\n"
"    {\n"
"      return !m_mask;\n"
"    }\n"
"\n"
"    bool operator==(Flags<BitType> const& rhs) const\n"
"    {\n"
"      return m_mask == rhs.m_mask;\n"
"    }\n"
"\n"
"    bool operator!=(Flags<BitType> const& rhs) const\n"
"    {\n"
"      return m_mask != rhs.m_mask;\n"
"    }\n"
"\n"
"    operator bool() const\n"
"    {\n"
"      return !!m_mask;\n"
"    }\n"
"\n"
"    explicit operator MaskType() const\n"
"    {\n"
"        return m_mask;\n"
"    }\n"
"\n"
"  private:\n"
"    MaskType  m_mask;\n"
"  };\n"
"  \n"
"  template <typename BitType>\n"
"  Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags)\n"
"  {\n"
"    return flags | bit;\n"
"  }\n"
"  \n"
"  template <typename BitType>\n"
"  Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags)\n"
"  {\n"
"    return flags & bit;\n"
"  }\n"
"  \n"
"  template <typename BitType>\n"
"  Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags)\n"
"  {\n"
"    return flags ^ bit;\n"
"  }\n"
"\n"
);

// trim from end
std::string trimEnd(std::string const& input)
{
  std::string result = input;
  result.erase(std::find_if(result.rbegin(), result.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), result.end());
  return result;
}

struct MemberData
{
  std::string type;
  std::string name;
  std::string arraySize;
  std::string pureType;
  std::string len;
};

struct StructData
{
  bool                    returnedOnly;
  std::vector<MemberData> members;
  std::string             protect;
};

struct NameValue
{
  std::string name;
  std::string value;
};

struct EnumData
{
  bool                    bitmask;
  std::string             prefix;
  std::string             postfix;
  std::vector<NameValue>  members;
  std::string             protect;

  void addEnum(std::string const & name, std::string const& tag);
};

struct CommandData
{
  CommandData()
    : handleCommand(false)
    , twoStep(false)
  {}

  std::string               returnType;
  std::vector<MemberData>   arguments;
  std::vector<std::string>  successCodes;
  std::string               protect;
  bool                      handleCommand;
  bool                      twoStep;
};

struct HandleData
{
  std::vector<std::string>  commands;
};

struct FlagData
{
  std::string protect;
};

struct ScalarData
{
  std::string protect;
};

struct DependencyData
{
  enum class Category
  {
    COMMAND,
    ENUM,
    FLAGS,
    FUNC_POINTER,
    HANDLE,
    REQUIRED,
    SCALAR,
    STRUCT,
    UNION
  };

  DependencyData( Category c, std::string const& n )
    : category(c)
    , name(n)
  {}

  Category              category;
  std::string           name;
  std::set<std::string> dependencies;
};

void createDefaults( std::vector<DependencyData> const& dependencies, std::map<std::string,EnumData> const& enums, std::map<std::string,std::string> & defaultValues );
std::string determineFunctionName(std::string const& name, CommandData const& commandData);
std::string determineReturnType(CommandData const& commandData, size_t returnIndex, bool isVector = false);
void enterProtect(std::ofstream &ofs, std::string const& protect);
std::string extractTag(std::string const& name);
size_t findReturnIndex(CommandData const& commandData, std::map<size_t,size_t> const& vectorParameters);
std::string findTag(std::string const& name, std::set<std::string> const& tags);
size_t findTemplateIndex(CommandData const& commandData, std::map<size_t, size_t> const& vectorParameters);
std::string generateEnumNameForFlags(std::string const& name);
std::map<size_t, size_t> getVectorParameters(CommandData const& commandData);
bool hasPointerArguments(CommandData const& commandData);
bool isVectorSizeParameter(std::map<size_t, size_t> const& vectorParameters, size_t idx);
void leaveProtect(std::ofstream &ofs, std::string const& protect);
bool noDependencies(std::set<std::string> const& dependencies, std::map<std::string, std::string> & listedTypes);
bool readCommandParam( tinyxml2::XMLElement * element, DependencyData & typeData, std::vector<MemberData> & arguments );
std::map<std::string, CommandData>::iterator readCommandProto(tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string, CommandData> & commands);
void readCommands( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,CommandData> & commands, std::map<std::string,HandleData> & handles, std::set<std::string> const& tags );
void readCommandsCommand(tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string, CommandData> & commands, std::map<std::string, HandleData> & handles, std::set<std::string> const& tags);
void readEnums( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,EnumData> & enums, std::set<std::string> const& tags, std::set<std::string> & vkTypes );
void readEnumsEnum( tinyxml2::XMLElement * element, EnumData & enumData, std::string const& tag );
void readExtensionRequire(tinyxml2::XMLElement * element, std::string const& protect, std::string const& tag, std::map<std::string, CommandData> & commands, std::map<std::string, EnumData> & enums, std::map<std::string, FlagData> & flags, std::map<std::string, ScalarData> & scalars, std::map<std::string, StructData> & structs);
void readExtensions( tinyxml2::XMLElement * element, std::map<std::string,CommandData> & commands, std::map<std::string,EnumData> & enums, std::map<std::string,FlagData> & flags, std::map<std::string,ScalarData> & scalars, std::map<std::string,StructData> & structs, std::set<std::string> const& tags );
void readExtensionsExtension(tinyxml2::XMLElement * element, std::map<std::string, CommandData> & commands, std::map<std::string, EnumData> & enums, std::map<std::string, FlagData> & flags, std::map<std::string, ScalarData> & scalars, std::map<std::string, StructData> & structs, std::set<std::string> const& tags);
void readTypeBasetype( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies );
void readTypeBitmask( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,FlagData> & flags, std::map<std::string,ScalarData> & scalars, std::set<std::string> & vkTypes, std::map<std::string, EnumData> & enums );
void readTypeDefine( tinyxml2::XMLElement * element, std::string & version, std::string & typesafeCheck );
void readTypeFuncpointer( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies );
void readTypeHandle(tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::set<std::string> & vkTypes, std::map<std::string,HandleData> & handles);
void readTypeStruct( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,StructData> & structs, std::set<std::string> & vkTypes );
void readTypeStructMember( tinyxml2::XMLElement * element, std::vector<MemberData> & members, std::set<std::string> & dependencies );
void readTypeUnion( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,StructData> & structs, std::set<std::string> & vkTypes );
void readTypeUnionMember( tinyxml2::XMLElement * element, std::vector<MemberData> & members, std::set<std::string> & dependencies );
void readTags(tinyxml2::XMLElement * element, std::set<std::string> & tags);
void readTypes(tinyxml2::XMLElement * element, std::string & version, std::string & typesafeCheck, std::list<DependencyData> & dependencies, std::map<std::string, FlagData> & flags, std::map<std::string, ScalarData> & scalars, std::map<std::string, StructData> & structs, std::set<std::string> & vkTypes, std::map<std::string, HandleData> & handles, std::map<std::string, EnumData> & enums);
void sortDependencies( std::list<DependencyData> & dependencies, std::vector<DependencyData> & sortedDependencies );
std::string reduceName(std::string const& name);
std::string strip(std::string const& value, std::string const& prefix, std::string const& tag = std::string());
std::string stripCommand(std::string const& value);
std::string toCamelCase(std::string const& value);
std::string toUpperCase(std::string const& name);
void writeCall(std::ofstream & ofs, std::string const& name, size_t templateIndex, CommandData const& commandData, std::set<std::string> const& vkTypes, std::map<size_t, size_t> const& vectorParameters = std::map<size_t, size_t>(), size_t specialIndex = ~0, std::string const& specialArgument = "");
void writeExceptionCheck(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, std::vector<std::string> const& successCodes);
void writeFunctionHeader(std::ofstream & ofs, std::string const& indentation, std::string const& returnType, std::string const& name, CommandData const& commandData, size_t returnIndex, size_t templateIndex, std::map<size_t, size_t> const& vectorParameters);
void writeMemberData(std::ofstream & ofs, MemberData const& memberData, std::set<std::string> const& vkTypes);
void writeStructConstructor( std::ofstream & ofs, std::string const& name, std::string const& memberName, StructData const& structData, std::set<std::string> const& vkTypes, std::map<std::string,std::string> const& defaultValues );
void writeStructGetter( std::ofstream & ofs, MemberData const& memberData, std::string const& memberName, std::set<std::string> const& vkTypes );
void writeStructSetter( std::ofstream & ofs, std::string const& name, MemberData const& memberData, std::string const& memberName, std::set<std::string> const& vkTypes, std::map<std::string,StructData> const& structs );
void writeTypeCommand( std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes );
void writeTypeCommandEnhanced(std::ofstream & ofs, std::string const& indentation, std::string const& className, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes);
void writeTypeCommandEnhanced(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, size_t templateIndex, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes, std::map<size_t, size_t> const& vectorParameters);
void writeTypeCommandEnhancedSingleStep(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, std::string const& returnType, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes, size_t returnIndex, size_t templateIndex, std::map<size_t, size_t> const& vectorParameters);
void writeTypeCommandEnhancedTwoStep(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, std::string const& returnType, size_t templateIndex, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes, size_t returnIndex, std::map<size_t, size_t> const& vectorParameters);
void writeTypeCommandEnhancedReplaceReturn(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, std::string const& returnType, size_t templateIndex, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes, size_t returnIndex, std::map<size_t, size_t> const& vectorParameters);
void writeTypeCommandStandard(std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes);
void writeTypeCommandComplexBody(std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::map<std::string,std::string> const& nameMap, std::map<size_t, size_t> const& vectorParameters, std::set<size_t> const& argIndices, size_t complexIndex, size_t returnIndex);
void writeTypeCommandSimpleBody(std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::map<std::string, std::string> const& nameMap, std::map<size_t, size_t> const& vectorParameters, std::set<size_t> const& argIndices, std::map<size_t, std::vector<size_t>> const& sizeIndices, size_t returnIndex);
void writeTypeEnum(std::ofstream & ofs, DependencyData const& dependencyData, EnumData const& enumData);
void writeTypeFlags( std::ofstream & ofs, DependencyData const& dependencyData, FlagData const& flagData );
void writeTypeHandle(std::ofstream & ofs, DependencyData const& dependencyData, HandleData const& handle, std::map<std::string, CommandData> const& commands, std::vector<DependencyData> const& dependencies, std::set<std::string> const& vkTypes);
void writeTypeScalar( std::ofstream & ofs, DependencyData const& dependencyData );
void writeTypeStruct( std::ofstream & ofs, DependencyData const& dependencyData, std::set<std::string> const& vkTypes, std::map<std::string,StructData> const& structs, std::map<std::string,std::string> const& defaultValues );
void writeTypeUnion( std::ofstream & ofs, DependencyData const& dependencyData, StructData const& unionData, std::set<std::string> const& vkTypes, std::map<std::string,StructData> const& structs, std::map<std::string,std::string> const& defaultValues );
void writeTypes(std::ofstream & ofs, std::vector<DependencyData> const& dependencies, std::map<std::string, CommandData> const& commands, std::map<std::string, EnumData> const& enums, std::map<std::string, FlagData> const& flags, std::map<std::string, HandleData> const& handles, std::map<std::string, StructData> const& structs, std::map<std::string, std::string> const& defaultValues, std::set<std::string> const& vkTypes);
void writeVersionCheck(std::ofstream & ofs, std::string const& version);
void writeTypesafeCheck(std::ofstream & ofs, std::string const& typesafeCheck);

void EnumData::addEnum(std::string const & name, std::string const& tag)
{
  assert(tag.empty() || (name.find(tag) != std::string::npos));
  members.push_back(NameValue());
  members.back().name = "e" + toCamelCase(strip(name, prefix, tag));
  members.back().value = name;
  if (!postfix.empty())
  {
    size_t pos = members.back().name.find(postfix);
    if (pos != std::string::npos)
    {
      members.back().name.erase(pos);
    }
  }
  if (!tag.empty())
  {
    members.back().name += tag;
  }
}

void createDefaults( std::vector<DependencyData> const& dependencies, std::map<std::string,EnumData> const& enums, std::map<std::string,std::string> & defaultValues )
{
  for ( std::vector<DependencyData>::const_iterator it = dependencies.begin() ; it != dependencies.end() ; ++it )
  {
    assert( defaultValues.find( it->name ) == defaultValues.end() );
    switch( it->category )
    {
      case DependencyData::Category::COMMAND :    // commands should never be asked for defaults
        break;
      case DependencyData::Category::ENUM :
        {
          assert(enums.find(it->name) != enums.end());
          EnumData const & enumData = enums.find(it->name)->second;
          if (!enumData.members.empty())
          {
            defaultValues[it->name] = it->name + "::" + enums.find(it->name)->second.members.front().name;
          }
          else
          {
            defaultValues[it->name] = it->name + "()";
          }
        }
        break;
      case DependencyData::Category::FLAGS :
      case DependencyData::Category::HANDLE:
      case DependencyData::Category::STRUCT:
      case DependencyData::Category::UNION :        // just call the default constructor for flags, structs, and structs (which are mapped to classes)
        defaultValues[it->name] = it->name + "()";
        break;
      case DependencyData::Category::FUNC_POINTER : // func_pointers explicitly have no default!
        defaultValues[it->name];
        break;
      case DependencyData::Category::REQUIRED :     // all required default to "0"
      case DependencyData::Category::SCALAR :       // all scalars default to "0"
        defaultValues[it->name] = "0";
        break;
      default :
        assert( false );
        break;
    }
  }
}

std::string determineFunctionName(std::string const& name, CommandData const& commandData)
{
  if (commandData.handleCommand)
  {
    std::string strippedName = name;
    std::string searchName = commandData.arguments[0].pureType;
    size_t pos = name.find(searchName);
    if (pos == std::string::npos)
    {
      assert(isupper(searchName[0]));
      searchName[0] = tolower(searchName[0]);
      pos = name.find(searchName);
    }
    if (pos != std::string::npos)
    {
      strippedName.erase(pos, commandData.arguments[0].pureType.length());
    }
    else if ((commandData.arguments[0].pureType == "CommandBuffer") && (name.find("cmd") == 0))
    {
      strippedName.erase(0, 3);
      pos = 0;
    }
    if (pos == 0)
    {
      assert(isupper(strippedName[0]));
      strippedName[0] = tolower(strippedName[0]);
    }
    return strippedName;
  }
  return name;
}

std::string determineReturnType(CommandData const& commandData, size_t returnIndex, bool isVector)
{
  std::string returnType;
  if (returnIndex != ~0)
  {
    if ((commandData.returnType == "Result") && (1 < commandData.successCodes.size()))
    {
      return("Result");
    }
    else if (isVector)
    {
      if (commandData.arguments[returnIndex].pureType == "void")
      {
        returnType = "std::vector<uint8_t>";
      }
      else
      {
        returnType = "std::vector<" + commandData.arguments[returnIndex].pureType + ">";
      }
    }
    else
    {
      assert(commandData.arguments[returnIndex].type.back() == '*');
      assert(commandData.arguments[returnIndex].type.find("const") == std::string::npos);
      returnType = commandData.arguments[returnIndex].type;
      returnType.pop_back();
    }
  }
  else if ((commandData.returnType == "Result") && (commandData.successCodes.size() < 2))
  {
    returnType = "void";
  }
  else
  {
    returnType = commandData.returnType;
  }
  return std::move(returnType);
}

void enterProtect(std::ofstream &ofs, std::string const& protect)
{
  if (!protect.empty())
  {
    ofs << "#ifdef " << protect << std::endl;
  }
}

std::string extractTag(std::string const& name)
{
  // the name is supposed to look like: VK_<tag>_<other>
  size_t start = name.find('_');
  assert(start != std::string::npos);
  size_t end = name.find('_', start + 1);
  assert(end != std::string::npos);
  return name.substr(start + 1, end - start - 1);
}

size_t findReturnIndex(CommandData const& commandData, std::map<size_t,size_t> const& vectorParameters)
{
  for (size_t i = 0; i < commandData.arguments.size(); i++)
  {
    if ((commandData.arguments[i].type.find('*') != std::string::npos) && (commandData.arguments[i].type.find("const") == std::string::npos) && !isVectorSizeParameter(vectorParameters, i))
    {
#if !defined(NDEBUG)
      for (size_t j = i + 1; j < commandData.arguments.size(); j++)
      {
        assert((commandData.arguments[j].type.find('*') == std::string::npos) || (commandData.arguments[j].type.find("const") != std::string::npos));
      }
#endif
      return i;
    }
  }
  return ~0;
}

std::string findTag(std::string const& name, std::set<std::string> const& tags)
{
  for (std::set<std::string>::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    size_t pos = name.find(*it);
    if ((pos != std::string::npos) && (pos == name.length() - it->length()))
    {
      return *it;
    }
  }
  return "";
}

size_t findTemplateIndex(CommandData const& commandData, std::map<size_t, size_t> const& vectorParameters)
{
  for (size_t i = 0; i < commandData.arguments.size(); i++)
  {
    if (commandData.arguments[i].name == "pData")
    {
      assert(vectorParameters.find(i) != vectorParameters.end());
      return i;
    }
  }
  return ~0;
}

std::string getEnumName(std::string const& name) // get vkcpp enum name from vk enum name
{
  return strip(name, "Vk");
}

bool hasPointerArguments(CommandData const& commandData)
{
  for (size_t i = 0; i < commandData.arguments.size(); i++)
  {
    size_t pos = commandData.arguments[i].type.find('*');
    if ((pos != std::string::npos) && (commandData.arguments[i].type.find('*', pos + 1) == std::string::npos))
    {
      return true;
    }
  }
  return false;
}

std::string generateEnumNameForFlags(std::string const& name)
{
  std::string generatedName = name;
  size_t pos = generatedName.rfind("Flags");
  assert(pos != std::string::npos);
  generatedName.replace(pos, 5, "FlagBits");
  return generatedName;
}

std::map<size_t, size_t> getVectorParameters(CommandData const& commandData)
{
  std::map<size_t,size_t> lenParameters;
  for (size_t i = 0; i < commandData.arguments.size(); i++)
  {
    if (!commandData.arguments[i].len.empty())
    {
      lenParameters.insert(std::make_pair(i, ~0));
      for (size_t j = 0; j < commandData.arguments.size(); j++)
      {
        if (commandData.arguments[i].len == commandData.arguments[j].name)
        {
          lenParameters[i] = j;
        }
      }
      assert(   (lenParameters[i] != ~0)
             || (commandData.arguments[i].len == "dataSize/4")
             || (commandData.arguments[i].len == "null-terminated")
             || (commandData.arguments[i].len == "pAllocateInfo->descriptorSetCount")
             || (commandData.arguments[i].len == "pAllocateInfo->commandBufferCount"));
      assert((lenParameters[i] == ~0) || (lenParameters[i] < i));
    }
  }
  return std::move(lenParameters);
}

bool isVectorSizeParameter(std::map<size_t, size_t> const& vectorParameters, size_t idx)
{
  for (std::map<size_t, size_t>::const_iterator it = vectorParameters.begin(); it != vectorParameters.end(); ++it)
  {
    if (it->second == idx)
    {
      return true;
    }
  }
  return false;
}

void leaveProtect(std::ofstream &ofs, std::string const& protect)
{
  if (!protect.empty())
  {
    ofs << "#endif /*" << protect << "*/" << std::endl;
  }
}

bool noDependencies(std::set<std::string> const& dependencies, std::set<std::string> & listedTypes)
{
  bool ok = true;
  for ( std::set<std::string>::const_iterator it = dependencies.begin() ; it != dependencies.end() && ok ; ++it )
  {
    ok = ( listedTypes.find( *it ) != listedTypes.end() );
  }
  return( ok );
}

bool readCommandParam( tinyxml2::XMLElement * element, DependencyData & typeData, std::vector<MemberData> & arguments )
{
  arguments.push_back( MemberData() );
  MemberData & arg = arguments.back();

  if (element->Attribute("len"))
  {
    arg.len = element->Attribute("len");
  }

  tinyxml2::XMLNode * child = element->FirstChild();
  assert( child );
  if ( child->ToText() )
  {
    std::string value = trimEnd(child->Value());
    assert( (value == "const") || (value == "struct") );
    arg.type = value + " ";
    child = child->NextSibling();
    assert( child );
  }

  assert( child->ToElement() );
  assert( ( strcmp( child->Value(), "type" ) == 0 ) && child->ToElement() && child->ToElement()->GetText() );
  std::string type = strip( child->ToElement()->GetText(), "Vk" );
  typeData.dependencies.insert( type );
  arg.type += type;
  arg.pureType = type;

  child = child->NextSibling();
  assert( child );
  if ( child->ToText() )
  {
    std::string value = trimEnd(child->Value());
    assert( ( value == "*" ) || ( value == "**" ) || ( value == "* const*" ) );
    arg.type += value;
    child = child->NextSibling();
  }

  assert( child->ToElement() && ( strcmp( child->Value(), "name" ) == 0 ) );
  arg.name = child->ToElement()->GetText();

  if ( arg.name.back() == ']' )
  {
    assert( !child->NextSibling() );
    size_t pos = arg.name.find( '[' );
    assert( pos != std::string::npos );
    arg.arraySize = arg.name.substr( pos + 1, arg.name.length() - 2 - pos );
    arg.name.erase( pos );
  }

  child = child->NextSibling();
  if ( child )
  {
    if ( child->ToText() )
    {
      std::string value = child->Value();
      if ( value == "[" )
      {
        child = child->NextSibling();
        assert( child );
        assert( child->ToElement() && ( strcmp( child->Value(), "enum" ) == 0 ) );
        arg.arraySize = child->ToElement()->GetText();
        child = child->NextSibling();
        assert( child );
        assert( child->ToText() );
        assert( strcmp( child->Value(), "]" ) == 0 );
        assert( !child->NextSibling() );
      }
      else
      {
        assert( ( value.front() == '[' ) && ( value.back() == ']' ) );
        arg.arraySize = value.substr( 1, value.length() - 2 );
        assert( !child->NextSibling() );
      }
    }
  }

  return element->Attribute("optional") && (strcmp(element->Attribute("optional"), "false,true") == 0);
}

std::map<std::string, CommandData>::iterator readCommandProto(tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string, CommandData> & commands)
{
  tinyxml2::XMLElement * typeElement = element->FirstChildElement();
  assert( typeElement && ( strcmp( typeElement->Value(), "type" ) == 0 ) );
  tinyxml2::XMLElement * nameElement = typeElement->NextSiblingElement();
  assert( nameElement && ( strcmp( nameElement->Value(), "name" ) == 0 ) );
  assert( !nameElement->NextSiblingElement() );

  std::string type = strip( typeElement->GetText(), "Vk" );
  std::string name = stripCommand( nameElement->GetText() );

  dependencies.push_back( DependencyData( DependencyData::Category::COMMAND, name ) );
  assert( commands.find( name ) == commands.end() );
  std::map<std::string,CommandData>::iterator it = commands.insert( std::make_pair( name, CommandData() ) ).first;
  it->second.returnType = type;

  return it;
}

void readCommands(tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string, CommandData> & commands, std::map<std::string, HandleData> & handles, std::set<std::string> const& tags)
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child );
  do
  {
    assert( strcmp( child->Value(), "command" ) == 0 );
    readCommandsCommand( child, dependencies, commands, handles, tags );
  } while ( child = child->NextSiblingElement() );
}

void readCommandsCommand(tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string, CommandData> & commands, std::map<std::string, HandleData> & handles, std::set<std::string> const& tags)
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child && ( strcmp( child->Value(), "proto" ) == 0 ) );

  std::map<std::string, CommandData>::iterator it = readCommandProto(child, dependencies, commands);

  if (element->Attribute("successcodes"))
  {
    std::string successCodes = element->Attribute("successcodes");
    size_t start = 0, end;
    do
    {
      end = successCodes.find(',', start);
      std::string code = successCodes.substr(start, end - start);
      std::string tag = findTag(code, tags);
      it->second.successCodes.push_back("e" + toCamelCase(strip(code, "VK_", tag)) + tag);
      start = end + 1;
    } while (end != std::string::npos);
  }

  // HACK: the current vk.xml misses to specify successcodes on command vkCreateDebugReportCallbackEXT!
  if (it->first == "createDebugReportCallbackEXT")
  {
    it->second.successCodes.clear();
    it->second.successCodes.push_back("eSuccess");
  }

  while ( child = child->NextSiblingElement() )
  {
    std::string value = child->Value();
    if ( value == "param" )
    {
      it->second.twoStep |= readCommandParam(child, dependencies.back(), it->second.arguments);
    }
    else
    {
      assert( ( value == "implicitexternsyncparams" ) || ( value == "validity" ) );
    }
  }

  // HACK: the current vk.xml misses to specify <optional="false,true"> on param pSparseMemoryRequirementCount on command vkGetImageSparseMemoryRequirements!
  if (it->first == "getImageSparseMemoryRequirements")
  {
    it->second.twoStep = true;
  }

  assert(!it->second.arguments.empty());
  std::map<std::string, HandleData>::iterator hit = handles.find(it->second.arguments[0].pureType);
  if (hit != handles.end())
  {
    hit->second.commands.push_back(it->first);
    it->second.handleCommand = true;
    DependencyData const& dep = dependencies.back();
    std::list<DependencyData>::iterator dit = std::find_if(dependencies.begin(), dependencies.end(), [hit](DependencyData const& dd) { return dd.name == hit->first; });
    for (std::set<std::string>::const_iterator depit = dep.dependencies.begin(); depit != dep.dependencies.end(); ++depit)
    {
      if (*depit != hit->first)
      {
        dit->dependencies.insert(*depit);
      }
    }
  }
}

void readEnums( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,EnumData> & enums, std::set<std::string> const& tags, std::set<std::string> & vkTypes )
{
  assert( element->Attribute( "name" ) );
  std::string name = getEnumName(element->Attribute("name"));
  if ( name != "API Constants" )
  {
    dependencies.push_back( DependencyData( DependencyData::Category::ENUM, name ) );
    std::map<std::string,EnumData>::iterator it = enums.insert( std::make_pair( name, EnumData() ) ).first;
    std::string tag;

    if (name == "Result")
    {
      // special handling for VKResult, as its enums just have VK_ in common
      it->second.prefix = "VK_";
    }
    else
    {
      assert(element->Attribute("type"));
      std::string type = element->Attribute("type");
      assert((type == "bitmask") || (type == "enum"));
      it->second.bitmask = (type == "bitmask");
      std::string prefix, postfix;
      if (it->second.bitmask)
      {
        size_t pos = name.find("FlagBits");
        assert(pos != std::string::npos);
        it->second.prefix = "VK" + toUpperCase(name.substr(0, pos)) + "_";
        it->second.postfix = "Bit";
      }
      else
      {
        it->second.prefix = "VK" + toUpperCase(name) + "_";
      }

      // if the enum name contains a tag remove it from the prefix to generate correct enum value names.
      for (std::set<std::string>::const_iterator tit = tags.begin(); tit != tags.end(); ++tit)
      {
        size_t pos = it->second.prefix.find(*tit);
        if ((pos != std::string::npos) && (pos == it->second.prefix.length() - tit->length() - 1))
        {
          it->second.prefix.erase(pos);
          tag = *tit;
          break;
        }
      }
    }

    readEnumsEnum( element, it->second, tag );

    assert( vkTypes.find( name ) == vkTypes.end() );
    vkTypes.insert( name );
  }
}

void readEnumsEnum( tinyxml2::XMLElement * element, EnumData & enumData, std::string const& tag )
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    if ( child->Attribute( "name" ) )
    {
      enumData.addEnum(child->Attribute("name"), tag);
    }
  } while ( child = child->NextSiblingElement() );
}

void readExtensionRequire(tinyxml2::XMLElement * element, std::string const& protect, std::string const& tag, std::map<std::string, CommandData> & commands, std::map<std::string, EnumData> & enums, std::map<std::string, FlagData> & flags, std::map<std::string, ScalarData> & scalars, std::map<std::string, StructData> & structs)
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    std::string value = child->Value();
    assert( child->Attribute( "name" ) );

    if ( value == "command" )
    {
      std::string name = stripCommand(child->Attribute("name"));
      std::map<std::string, CommandData>::iterator cit = commands.find(name);
      assert(cit != commands.end());
      cit->second.protect = protect;
    }
    else if (value == "type")
    {
      std::string name = strip(child->Attribute("name"), "Vk");
      std::map<std::string, EnumData>::iterator eit = enums.find(name);
      if (eit != enums.end())
      {
        eit->second.protect = protect;
      }
      else
      {
        std::map<std::string, FlagData>::iterator fit = flags.find(name);
        if (fit != flags.end())
        {
          fit->second.protect = protect;

          // if the enum of this flags is auto-generated, protect it as well
          std::string enumName = generateEnumNameForFlags(name);
          std::map<std::string, EnumData>::iterator eit = enums.find(enumName);
          assert(eit != enums.end());
          if (eit->second.members.empty())
          {
            eit->second.protect = protect;
          }
        }
        else
        {
          std::map<std::string, ScalarData>::iterator scit = scalars.find(name);
          if (scit != scalars.end())
          {
            scit->second.protect = protect;
          }
          else
          {
            std::map<std::string, StructData>::iterator stit = structs.find(name);
            assert(stit != structs.end() && stit->second.protect.empty());
            stit->second.protect = protect;
          }
        }
      }
    }
    else if ( value == "enum")
    {
      // TODO process enums which don't extend existing enums
      if (child->Attribute("extends"))
      {
        assert(enums.find(getEnumName(child->Attribute("extends"))) != enums.end());
        enums[getEnumName(child->Attribute("extends"))].addEnum(child->Attribute("name"), tag);
      }
    }
    else
    {
        assert("unknown attribute, check me");
    }
  } while ( child = child->NextSiblingElement() );
}

void readExtensions(tinyxml2::XMLElement * element, std::map<std::string, CommandData> & commands, std::map<std::string, EnumData> &enums, std::map<std::string, FlagData> & flags, std::map<std::string, ScalarData> & scalars, std::map<std::string, StructData> & structs, std::set<std::string> const& tags)
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child );
  do
  {
    assert( strcmp( child->Value(), "extension" ) == 0 );
    readExtensionsExtension( child, commands, enums, flags, scalars, structs, tags );
  } while ( child = child->NextSiblingElement() );
}

void readExtensionsExtension(tinyxml2::XMLElement * element, std::map<std::string, CommandData> & commands, std::map<std::string, EnumData> & enums, std::map<std::string, FlagData> & flags, std::map<std::string, ScalarData> & scalars, std::map<std::string, StructData> & structs, std::set<std::string> const& tags)
{
  assert( element->Attribute( "name" ) );
  std::string tag = extractTag(element->Attribute("name"));
  assert(tags.find(tag) != tags.end());

  // don't parse disabled extensions
  if (strcmp(element->Attribute("supported"), "disabled") == 0)
  {
    return;
  }

  std::string protect;
  if (element->Attribute("protect"))
  {
    protect = element->Attribute( "protect" );
  }

  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child && ( strcmp( child->Value(), "require" ) == 0 ) && !child->NextSiblingElement() );
  readExtensionRequire( child, protect, tag, commands, enums, flags, scalars, structs );
}

void readTypeBasetype( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies )
{
  tinyxml2::XMLElement * typeElement = element->FirstChildElement();
  assert( typeElement && ( strcmp( typeElement->Value(), "type" ) == 0 ) && typeElement->GetText() );
  std::string type = typeElement->GetText();
  assert( ( type == "uint32_t" ) || ( type == "uint64_t" ) );

  tinyxml2::XMLElement * nameElement = typeElement->NextSiblingElement();
  assert( nameElement && ( strcmp( nameElement->Value(), "name" ) == 0 ) && nameElement->GetText() );
  std::string name = strip( nameElement->GetText(), "Vk" );

  // skip "Flags",
  if ( name != "Flags" )
  {
    dependencies.push_back( DependencyData( DependencyData::Category::SCALAR, name ) );
    dependencies.back().dependencies.insert( type );
  }
  else
  {
    assert( type == "uint32_t" );
  }
}

void readTypeBitmask(tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string, FlagData> & flags, std::map<std::string, ScalarData> & scalars, std::set<std::string> & vkTypes, std::map<std::string, EnumData> & enums)
{
  tinyxml2::XMLElement * typeElement = element->FirstChildElement();
  assert( typeElement && ( strcmp( typeElement->Value(), "type" ) == 0 ) && typeElement->GetText() && ( strcmp( typeElement->GetText(), "VkFlags" ) == 0 ) );
  std::string type = typeElement->GetText();

  tinyxml2::XMLElement * nameElement = typeElement->NextSiblingElement();
  assert( nameElement && ( strcmp( nameElement->Value(), "name" ) == 0 ) && nameElement->GetText() );
  std::string name = strip( nameElement->GetText(), "Vk" );

  assert( !nameElement->NextSiblingElement() );

  std::string requires;
  if (element->Attribute("requires"))
  {
    requires = strip(element->Attribute("requires"), "Vk");
  }
  else {
    // Generate FlagBits name
    requires = generateEnumNameForFlags(name);
    dependencies.push_back(DependencyData(DependencyData::Category::ENUM, requires));
    std::map<std::string, EnumData>::iterator it = enums.insert(std::make_pair(requires, EnumData())).first;
    it->second.bitmask = true;
    vkTypes.insert(requires);
  }

  dependencies.push_back( DependencyData( DependencyData::Category::FLAGS, name ) );
  dependencies.back().dependencies.insert( requires );
  flags.insert(std::make_pair(name, FlagData()));

  assert( vkTypes.find( name ) == vkTypes.end() );
  vkTypes.insert( name );
}

void readTypeDefine( tinyxml2::XMLElement * element, std::string & version, std::string & typedefCheck )
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  if (child && (strcmp(child->GetText(), "VK_API_VERSION") == 0))
  {
    version = element->LastChild()->ToText()->Value();
  }
  else if (element->Attribute("name") && strcmp(element->Attribute("name"), "VK_DEFINE_NON_DISPATCHABLE_HANDLE") == 0)
  {
    std::string text = element->LastChild()->ToText()->Value();
    size_t start = text.find('#');
    size_t end = text.find_first_of("\r\n", start + 1);
    typedefCheck = text.substr(start, end - start);
  }
}

void readTypeFuncpointer( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies )
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child && ( strcmp( child->Value(), "name" ) == 0 ) && child->GetText() );
  dependencies.push_back( DependencyData( DependencyData::Category::FUNC_POINTER, child->GetText() ) );
}

void readTypeHandle(tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::set<std::string> & vkTypes, std::map<std::string,HandleData> & handles)
{
  tinyxml2::XMLElement * typeElement = element->FirstChildElement();
  assert( typeElement && ( strcmp( typeElement->Value(), "type" ) == 0 ) && typeElement->GetText() );
#if !defined(NDEBUG)
  std::string type = typeElement->GetText();
  assert( type.find( "VK_DEFINE" ) == 0 );
#endif

  tinyxml2::XMLElement * nameElement = typeElement->NextSiblingElement();
  assert( nameElement && ( strcmp( nameElement->Value(), "name" ) == 0 ) && nameElement->GetText() );
  std::string name = strip( nameElement->GetText(), "Vk" );

  dependencies.push_back( DependencyData( DependencyData::Category::HANDLE, name ) );

  assert(vkTypes.find(name) == vkTypes.end());
  vkTypes.insert(name);
  assert(handles.find(name) == handles.end());
  handles[name];    // add this to the handles map
}

void readTypeStructMember( tinyxml2::XMLElement * element, std::vector<MemberData> & members, std::set<std::string> & dependencies )
{
  members.push_back( MemberData() );
  MemberData & member = members.back();

  tinyxml2::XMLNode * child = element->FirstChild();
  assert( child );
  if ( child->ToText())
  {
    std::string value = trimEnd(child->Value());
    assert( (value == "const") || (value == "struct") );
    member.type = value + " ";
    child = child->NextSibling();
    assert( child );
  }

  assert( child->ToElement() );
  assert( ( strcmp( child->Value(), "type" ) == 0 ) && child->ToElement() && child->ToElement()->GetText() );
  std::string type = strip( child->ToElement()->GetText(), "Vk" );
  dependencies.insert( type );
  member.type += type;
  member.pureType = type;

  child = child->NextSibling();
  assert( child );
  if ( child->ToText())
  {
    std::string value = trimEnd(child->Value());
    assert( ( value == "*" ) || ( value == "**" ) || ( value == "* const*" ) );
    member.type += value;
    child = child->NextSibling();
  }

  assert( ( child->ToElement() && strcmp( child->Value(), "name" ) == 0 ));
  member.name = child->ToElement()->GetText();

  if ( member.name.back() == ']' )
  {
    assert( !child->NextSibling() );
    size_t pos = member.name.find( '[' );
    assert( pos != std::string::npos );
    member.arraySize = member.name.substr( pos + 1, member.name.length() - 2 - pos );
    member.name.erase( pos );
  }

  child = child->NextSibling();
  if ( child )
  {
    assert( member.arraySize.empty() );
    if ( child->ToText() )
    {
      std::string value = child->Value();
      if ( value == "[" )
      {
        child = child->NextSibling();
        assert( child );
        assert( child->ToElement() && ( strcmp( child->Value(), "enum" ) == 0 ) );
        member.arraySize = child->ToElement()->GetText();
        child = child->NextSibling();
        assert( child );
        assert( child->ToText() );
        assert( strcmp( child->Value(), "]" ) == 0 );
        assert( !child->NextSibling() );
      }
      else
      {
        assert( ( value.front() == '[' ) && ( value.back() == ']' ) );
        member.arraySize = value.substr( 1, value.length() - 2 );
        assert( !child->NextSibling() );
      }
    }
  }
}

void readTypeStruct( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,StructData> & structs, std::set<std::string> & vkTypes )
{
  assert( !element->Attribute( "returnedonly" ) || ( strcmp( element->Attribute( "returnedonly" ), "true" ) == 0 ) );

  assert( element->Attribute( "name" ) );
  std::string name = strip( element->Attribute( "name" ), "Vk" );

  if ( name == "Rect3D" )
  {
    return;
  }

  dependencies.push_back( DependencyData( DependencyData::Category::STRUCT, name ) );

  assert( structs.find( name ) == structs.end() );
  std::map<std::string,StructData>::iterator it = structs.insert( std::make_pair( name, StructData() ) ).first;
  it->second.returnedOnly = !!element->Attribute( "returnedonly" );

  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    assert( child->Value() );
    std::string value = child->Value();
    if ( value == "member" )
    {
      readTypeStructMember( child, it->second.members, dependencies.back().dependencies );
    }
    else
    {
      assert( value == "validity" );
    }
  } while ( child = child->NextSiblingElement() );

  assert( vkTypes.find( name ) == vkTypes.end() );
  vkTypes.insert( name );
}

void readTypeUnionMember( tinyxml2::XMLElement * element, std::vector<MemberData> & members, std::set<std::string> & dependencies )
{
  members.push_back( MemberData() );
  MemberData & member = members.back();

  tinyxml2::XMLNode * child = element->FirstChild();
  assert( child );
  if ( child->ToText() )
  {
    assert( ( strcmp( child->Value(), "const" ) == 0 ) || ( strcmp( child->Value(), "struct" ) == 0 ) );
    member.type = std::string( child->Value() ) + " ";
    child = child->NextSibling();
    assert( child );
  }

  assert( child->ToElement() );
  assert( ( strcmp( child->Value(), "type" ) == 0 ) && child->ToElement() && child->ToElement()->GetText() );
  std::string type = strip( child->ToElement()->GetText(), "Vk" );
  dependencies.insert( type );
  member.type += type;
  member.pureType = type;

  child = child->NextSibling();
  assert( child );
  if ( child->ToText() )
  {
    std::string value = child->Value();
    assert( ( value == "*" ) || ( value == "**" ) || ( value == "* const*" ) );
    member.type += value;
    child = child->NextSibling();
  }

  assert( child->ToElement() && ( strcmp( child->Value(), "name" ) == 0 ) );
  member.name = child->ToElement()->GetText();

  if ( member.name.back() == ']' )
  {
    assert( !child->NextSibling() );
    size_t pos = member.name.find( '[' );
    assert( pos != std::string::npos );
    member.arraySize = member.name.substr( pos + 1, member.name.length() - 2 - pos );
    member.name.erase( pos );
  }

  child = child->NextSibling();
  if ( child )
  {
    if ( child->ToText() )
    {
      std::string value = child->Value();
      if ( value == "[" )
      {
        child = child->NextSibling();
        assert( child );
        assert( child->ToElement() && ( strcmp( child->Value(), "enum" ) == 0 ) );
        member.arraySize = child->ToElement()->GetText();
        child = child->NextSibling();
        assert( child );
        assert( child->ToText() );
        assert( strcmp( child->Value(), "]" ) == 0 );
        assert( !child->NextSibling() );
      }
      else
      {
        assert( ( value.front() == '[' ) && ( value.back() == ']' ) );
        member.arraySize = value.substr( 1, value.length() - 2 );
        assert( !child->NextSibling() );
      }
    }
  }
}

void readTypeUnion( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,StructData> & structs, std::set<std::string> & vkTypes )
{
  assert( element->Attribute( "name" ) );
  std::string name = strip( element->Attribute( "name" ), "Vk" );

  dependencies.push_back( DependencyData( DependencyData::Category::UNION, name ) );

  assert( structs.find( name ) == structs.end() );
  std::map<std::string,StructData>::iterator it = structs.insert( std::make_pair( name, StructData() ) ).first;

  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    assert( strcmp( child->Value(), "member" ) == 0 );
    readTypeUnionMember( child, it->second.members, dependencies.back().dependencies );
  } while ( child = child->NextSiblingElement() );

  assert( vkTypes.find( name ) == vkTypes.end() );
  vkTypes.insert( name );
}

void readTags(tinyxml2::XMLElement * element, std::set<std::string> & tags)
{
  tags.insert("EXT");
  tags.insert("KHR");
  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    assert(child->Attribute("name"));
    tags.insert(child->Attribute("name"));
  } while (child = child->NextSiblingElement());
}

void readTypes(tinyxml2::XMLElement * element, std::string & version, std::string & typedefCheck, std::list<DependencyData> & dependencies, std::map<std::string, FlagData> & flags, std::map<std::string, ScalarData> & scalars, std::map<std::string, StructData> & structs, std::set<std::string> & vkTypes, std::map<std::string, HandleData> & handles, std::map<std::string, EnumData> & enums)
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    assert( strcmp( child->Value(), "type" ) == 0 );
    std::string type = child->Value();
    assert( type == "type" );
    if ( child->Attribute( "category" ) )
    {
      std::string category = child->Attribute( "category" );
      if ( category == "basetype" )
      {
        readTypeBasetype( child, dependencies );
      }
      else if ( category == "bitmask" )
      {
        readTypeBitmask( child, dependencies, flags, scalars, vkTypes, enums );
      }
      else if ( category == "define" )
      {
        readTypeDefine( child, version, typedefCheck );
      }
      else if ( category == "funcpointer" )
      {
        readTypeFuncpointer( child, dependencies );
      }
      else if ( category == "handle" )
      {
        readTypeHandle( child, dependencies, vkTypes, handles );
      }
      else if ( category == "struct" )
      {
        readTypeStruct( child, dependencies, structs, vkTypes );
      }
      else if ( category == "union" )
      {
        readTypeUnion( child, dependencies, structs, vkTypes );
      }
      else
      {
        assert( ( category == "enum" ) || ( category == "include" ) );
      }
    }
    else
    {
      assert( child->Attribute( "requires" ) && child->Attribute( "name" ) );
      dependencies.push_back( DependencyData( DependencyData::Category::REQUIRED, child->Attribute( "name" ) ) );
    }
  } while ( child = child->NextSiblingElement() );
}

void sortDependencies( std::list<DependencyData> & dependencies, std::vector<DependencyData> & sortedDependencies )
{
  std::set<std::string> listedTypes = { "VkFlags" };

  sortedDependencies.reserve( dependencies.size() );

  while ( !dependencies.empty() )
  {
#if !defined(NDEBUG)
    bool ok = false;
#endif
    for ( std::list<DependencyData>::iterator it = dependencies.begin() ; it != dependencies.end() ; ++it )
    {
      if ( noDependencies( it->dependencies, listedTypes ) )
      {
        sortedDependencies.push_back( *it );
        listedTypes.insert( it->name );
        dependencies.erase( it );
#if !defined(NDEBUG)
        ok = true;
#endif
        break;
      }
    }
    assert( ok );
  }
}

std::string reduceName(std::string const& name)
{
  assert(1 < name.length());
  if ((name[0] == 'p') && (isupper(name[1]) || name[1] == 'p'))
  {
    std::string reducedName = strip(name, "p");
    reducedName[0] = tolower(reducedName[0]);
    return reducedName;
  }
  return name;
}

std::string strip(std::string const& value, std::string const& prefix, std::string const& postfix)
{
  std::string strippedValue = value;
  if (strippedValue.find(prefix) == 0)
  {
    strippedValue.erase(0, prefix.length());
  }
  if (!postfix.empty())
  {
    size_t pos = strippedValue.find(postfix);
    assert(pos != std::string::npos);
    strippedValue.erase(pos);
  }
  return strippedValue;
}

std::string stripCommand(std::string const& value)
{
  std::string stripped = strip(value, "vk");
  assert(isupper(stripped[0]));
  stripped[0] = tolower(stripped[0]);
  return stripped;
}

std::string toCamelCase(std::string const& value)
{
  assert(!value.empty() && (isupper(value[0]) || isdigit(value[0])));
  std::string result;
  result.reserve(value.size());
  result.push_back(value[0]);
  for (size_t i = 1; i < value.size(); i++)
  {
    if (value[i] != '_')
    {
      if ((value[i - 1] == '_') || isdigit(value[i-1]))
      {
        result.push_back(value[i]);
      }
      else
      {
        result.push_back(tolower(value[i]));
      }
    }
  }
  return result;
}

std::string toUpperCase(std::string const& name)
{
  assert(isupper(name.front()));
  std::string convertedName;

  for (size_t i = 0; i<name.length(); i++)
  {
    if (isupper(name[i]) && ((i == 0) || islower(name[i - 1]) || isdigit(name[i-1])))
    {
      convertedName.push_back('_');
    }
    convertedName.push_back(toupper(name[i]));
  }
  return convertedName;
}

void writeCall(std::ofstream & ofs, std::string const& name, size_t templateIndex, CommandData const& commandData, std::set<std::string> const& vkTypes, std::map<size_t, size_t> const& vectorParameters, size_t specialIndex, std::string const& specialArgument)
{
  std::map<size_t,size_t> countIndices;
  for (std::map<size_t, size_t>::const_iterator it = vectorParameters.begin(); it != vectorParameters.end(); ++it)
  {
    countIndices.insert(std::make_pair(it->second, it->first));
  }
  if ((vectorParameters.size() == 1) && (commandData.arguments[vectorParameters.begin()->first].len == "dataSize/4"))
  {
    assert(commandData.arguments[3].name == "dataSize");
    countIndices.insert(std::make_pair(3, vectorParameters.begin()->first));
  }

  assert(islower(name[0]));
  ofs << "vk" << static_cast<char>(toupper(name[0])) << name.substr(1) << "( ";
  size_t i = 0;
  if (commandData.handleCommand)
  {
    ofs << "m_" << commandData.arguments[0].name;
    i++;
  }
  for (; i < commandData.arguments.size(); i++)
  {
    if (0 < i)
    {
      ofs << ", ";
    }
    std::map<size_t, size_t>::const_iterator it = countIndices.find(i);
    if (it != countIndices.end())
    {
      if (specialIndex == it->second)
      {
        ofs << "&" << reduceName(commandData.arguments[it->first].name);
      }
      else
      {
        ofs << "static_cast<" << commandData.arguments[it->first].pureType << ">( " << reduceName(commandData.arguments[it->second].name) << ".size() ";
        if (templateIndex == it->second)
        {
          ofs << "* sizeof( T ) ";
        }
        ofs << ")";
      }
    }
    else
    {
      it = vectorParameters.find(i);
      if (it != vectorParameters.end())
      {
        assert(commandData.arguments[it->first].type.back() == '*');
        std::set<std::string>::const_iterator vkit = vkTypes.find(commandData.arguments[it->first].pureType);
        if ((vkit != vkTypes.end()) || (it->first == templateIndex))
        {
          ofs << "reinterpret_cast<";
          if (commandData.arguments[it->first].type.find("const") != std::string::npos)
          {
            ofs << "const ";
          }
          if (vkit != vkTypes.end())
          {
            ofs << "Vk";
          }
          ofs << commandData.arguments[it->first].pureType << "*>( " << reduceName(commandData.arguments[it->first].name) << ".data() )";
        }
        else
        {
          ofs << reduceName(commandData.arguments[it->first].name) << ".data()";
        }
      }
      else if (vkTypes.find(commandData.arguments[i].pureType) != vkTypes.end())
      {
        if (commandData.arguments[i].type.back() == '*')
        {
          if (commandData.arguments[i].type.find("const") != std::string::npos)
          {
            ofs << "reinterpret_cast<const Vk" << commandData.arguments[i].pureType << "*>( &" << reduceName(commandData.arguments[i].name) << " )";
          }
          else
          {
            ofs << "reinterpret_cast<Vk" << commandData.arguments[i].pureType << "*>( &" << reduceName(commandData.arguments[i].name) << " )";
          }
        }
        else
        {
          ofs << "static_cast<Vk" << commandData.arguments[i].pureType << ">( " << commandData.arguments[i].name << " )";
        }
      }
      else
      {
        if (commandData.arguments[i].type.back() == '*')
        {
          if (commandData.arguments[i].type.find("const") != std::string::npos)
          {
            assert(commandData.arguments[i].type.find("char") != std::string::npos);
            ofs << reduceName(commandData.arguments[i].name) << ".c_str()";
          }
          else
          {
            assert(commandData.arguments[i].type.find("char") == std::string::npos);
            ofs << "&" << reduceName(commandData.arguments[i].name);
          }
        }
        else
        {
          ofs << commandData.arguments[i].name;
        }
      }
    }
  }
  ofs << " )";
}

void writeExceptionCheck(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, std::vector<std::string> const& successCodes)
{
  if (successCodes.size() == 1)
  {
    assert(successCodes.front() == "eSuccess");
    ofs << indentation << "  if ( result != Result::eSuccess )" << std::endl;
  }
  else
  {
    assert(successCodes.size() == 2);
    ofs << indentation << "  if ( ( result != Result::" << successCodes[0] << " ) && ( result != Result::" << successCodes[1] << " ) )" << std::endl;
  }
  ofs << indentation << "  {" << std::endl
    << indentation << "    throw Exception( result, \"vk::";
  if (!className.empty())
  {
    ofs << className << "::";
  }
  ofs << functionName << "\" );" << std::endl
      << indentation << "  }" << std::endl;
}

void writeFunctionHeader(std::ofstream & ofs, std::string const& indentation, std::string const& returnType, std::string const& name, CommandData const& commandData, size_t returnIndex, size_t templateIndex, std::map<size_t, size_t> const& vectorParameters)
{
  std::set<size_t> skippedArguments;
  for (std::map<size_t, size_t>::const_iterator it = vectorParameters.begin(); it != vectorParameters.end(); ++it)
  {
    skippedArguments.insert(it->second);
  }
  if ((vectorParameters.size() == 1) && (commandData.arguments[vectorParameters.begin()->first].len == "dataSize/4"))
  {
    assert(commandData.arguments[3].name == "dataSize");
    skippedArguments.insert(3);
  }
  if (commandData.successCodes.size() < 2)
  {
    skippedArguments.insert(returnIndex);
  }

  ofs << indentation;
  if ((templateIndex != ~0) && ((templateIndex != returnIndex) || (returnType == "Result")))
  {
    ofs << "template <typename T>" << std::endl
        << indentation;
  }
  else if (!commandData.handleCommand)
  {
    ofs << "inline ";
  }
  ofs << returnType << " " << name << "( ";
  bool argEncountered = false;
  for (size_t i = commandData.handleCommand ? 1 : 0; i < commandData.arguments.size(); i++)
  {
    if (skippedArguments.find(i) == skippedArguments.end())
    {
      if (argEncountered)
      {
        ofs << ", ";
      }
      if (vectorParameters.find(i) == vectorParameters.end())
      {
        size_t pos = commandData.arguments[i].type.find('*');
        if (pos == std::string::npos)
        {
          ofs << commandData.arguments[i].type << " " << commandData.arguments[i].name;
          if (!commandData.arguments[i].arraySize.empty())
          {
            ofs << "[" << commandData.arguments[i].arraySize << "]";
          }
        }
        else
        {
          std::string type = commandData.arguments[i].type;
          if (type.find("char") != std::string::npos)
          {
            type = "std::string const&";
          }
          else
          {
            assert(type[pos] == '*');
            type[pos] = '&';
          }
          ofs << type << " " << reduceName(commandData.arguments[i].name);
        }
      }
      else
      {
        if (templateIndex == i)
        {
          ofs << "std::vector<T> ";
        }
        else if (commandData.arguments[i].pureType == "char")
        {
          ofs << "std::string ";
        }
        else if (commandData.arguments[i].pureType == "void")
        {
          ofs << "std::vector<uint8_t> ";
        }
        else
        {
          ofs << "std::vector<" << commandData.arguments[i].pureType << "> ";
        }
        if (commandData.arguments[i].type.find("const") != std::string::npos)
        {
          ofs << "const";
        }
        ofs << "& " << reduceName(commandData.arguments[i].name);
      }
      argEncountered = true;
    }
  }
  ofs << " )";
  if (commandData.handleCommand)
  {
    ofs << " const";
  }
  ofs << std::endl;
}

void writeMemberData(std::ofstream & ofs, MemberData const& memberData, std::set<std::string> const& vkTypes)
{
  if ( vkTypes.find( memberData.pureType ) != vkTypes.end() )
  {
    if ( memberData.type.back() == '*' )
    {
      ofs << "reinterpret_cast<";
      if ( memberData.type.find( "const" ) == 0 )
      {
        ofs << "const ";
      }
      ofs << "Vk" << memberData.pureType << '*';
    }
    else
    {
      ofs << "static_cast<Vk" << memberData.pureType;
    }
    ofs << ">( " << memberData.name << " )";
  }
  else
  {
    ofs << memberData.name;
  }
}

void writeStructConstructor( std::ofstream & ofs, std::string const& name, std::string const& memberName, StructData const& structData, std::set<std::string> const& vkTypes, std::map<std::string,std::string> const& defaultValues )
{
  // check if there is a member element with no default available
  bool noDefault = false;
  for (size_t i = 0; i < structData.members.size() && !noDefault; i++)
  {
    std::map<std::string, std::string>::const_iterator it = defaultValues.find(structData.members[i].pureType);
    assert(it != defaultValues.end());
    noDefault = it->second.empty();
  }

  if (!noDefault)
  {
    // if there's a default for all memeber, provide a default constructor
    ofs << "    " << name << "()" << std::endl
      << "      : " << name << "( ";
    bool listedArgument = false;
    for (size_t i = 0; i < structData.members.size(); i++)
    {
      if (listedArgument)
      {
        ofs << ", ";
      }
      if ((structData.members[i].name != "pNext") && (structData.members[i].name != "sType"))
      {
        if (structData.members[i].type.back() == '*')
        {
          ofs << "nullptr";
        }
        else
        {
          std::map<std::string, std::string>::const_iterator it = defaultValues.find(structData.members[i].pureType);
          assert((it != defaultValues.end()) && !it->second.empty());

          if (structData.members[i].arraySize.empty())
          {
            ofs << it->second;
          }
          else
          {
            ofs << "{ " << it->second << " }";
          }
        }
        listedArgument = true;
      }
    }
    ofs << " )" << std::endl
        << "    {}" << std::endl
        << std::endl;
  }

  // the constructor with all the elements as arguments
  ofs << "    " << name << "( ";
  std::vector<std::string> noDefaultArgs, defaultArgs;
  bool listedArgument = false;
  for (size_t i = 0; i<structData.members.size(); i++)
  {
    if (listedArgument)
    {
      ofs << ", ";
    }
    if ((structData.members[i].name != "pNext") && (structData.members[i].name != "sType"))
    {
      if (structData.members[i].arraySize.empty())
      {
        ofs << structData.members[i].type + " " + structData.members[i].name;
      }
      else
      {
        ofs << "std::array<" + structData.members[i].type + "," + structData.members[i].arraySize + "> const& " + structData.members[i].name;
      }
      listedArgument = true;
    }
  }
  ofs << ")" << std::endl;

  // now the body of the constructor, copying over data from argument list into wrapped struct
  ofs << "    {" << std::endl;
  for ( size_t i=0 ; i<structData.members.size() ; i++ )
  {
    if ( !structData.members[i].arraySize.empty() )
    {
      ofs << "      memcpy( &" << memberName << "." << structData.members[i].name << ", " << structData.members[i].name << ".data(), " << structData.members[i].arraySize << " * sizeof( " << structData.members[i].type << " ) )";
    }
    else
    {
      ofs << "      " << memberName << "." << structData.members[i].name << " = ";
      if ( structData.members[i].name == "pNext" )
      {
        ofs << "nullptr";
      }
      else if ( structData.members[i].name == "sType" )
      {
        // HACK: we need to read <enum extends="VkStructureType"> here for the correct name. In this case the 'generic' rule to create the enums doesn't work
        if (name == "DebugReportCallbackCreateInfoEXT")
        {
          ofs << "VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT";
        }
        else
        {
          ofs << "VK_STRUCTURE_TYPE" << toUpperCase(name);
        }
      }
      else
      {
        writeMemberData( ofs, structData.members[i], vkTypes );
      }
    }
    ofs << ";" << std::endl;
  }
  ofs << "    }" << std::endl
      << std::endl;

  // now write the copy constructor
  ofs << "    " << name << "(Vk" << name << " const & rhs)" << std::endl
      << "      : " << memberName << "(rhs)" << std::endl
      << "    {" << std::endl
      << "    }" << std::endl
      << std::endl;

  // now write the assignment operator
  ofs << "    " << name << "& operator=(Vk" << name << " const & rhs)" << std::endl
    << "    {" << std::endl
    << "      " << memberName << " = rhs;" << std::endl
    << "      return *this;" << std::endl
    << "    }" << std::endl
    << std::endl;

}

void writeStructGetter( std::ofstream & ofs, MemberData const& memberData, std::string const& memberName, std::set<std::string> const& vkTypes )
{
  ofs << "    ";
  if ( memberData.type.back() == '*' )
  {
    if ( memberData.type.find( "const" ) != 0 )
    {
      ofs << "const ";
    }
    ofs << memberData.type;
  }
  else
  {
    ofs << "const " << memberData.type << ( memberData.arraySize.empty() ? '&' : '*' );
  }

  ofs << " " << memberData.name << "() const" << std::endl
      << "    {" << std::endl
      << "      return ";
  if ( ! memberData.arraySize.empty() )
  {
    ofs << "reinterpret_cast<const " << memberData.type << "*>( " << memberName << "." << memberData.name << " )";
  }
  else if ( memberData.type.back() == '*' )
  {
    ofs << "reinterpret_cast<" << memberData.type << ">( " << memberName << "." << memberData.name << " )";
  }
  else if ( vkTypes.find( memberData.pureType ) != vkTypes.end() )
  {
    ofs << "reinterpret_cast<const " << memberData.pureType << "&>( " << memberName << "." << memberData.name << " )";
  }
  else
  {
    ofs << memberName << "." << memberData.name;
  }
  ofs << ";" << std::endl
      << "    }" << std::endl
      << std::endl;
}

void writeStructSetter( std::ofstream & ofs, std::string const& name, MemberData const& memberData, std::string const& memberName, std::set<std::string> const& vkTypes )
{
  ofs << "    " << name << "& " << memberData.name << "( ";
  if ( memberData.arraySize.empty() )
  {
    ofs << memberData.type << " ";
  }
  else
  {
    ofs << "std::array<" << memberData.type << "," << memberData.arraySize << "> ";
  }
  ofs << memberData.name << " )" << std::endl
      << "    {" << std::endl;
  if ( !memberData.arraySize.empty() )
  {
    ofs << "      memcpy( &" << memberName << "." << memberData.name << ", " << memberData.name << ".data(), " << memberData.arraySize << " * sizeof( " << memberData.type << " ) )";
  }
  else
  {
    ofs << "      " << memberName << "." << memberData.name << " = ";
    writeMemberData( ofs, memberData, vkTypes );
  }
  ofs << ";" << std::endl
      << "      return *this;" << std::endl
      << "    }" << std::endl
      << std::endl;
}

void writeTypeCommand( std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes )
{
  enterProtect(ofs, commandData.protect);
  writeTypeCommandStandard(ofs, dependencyData, commandData, vkTypes);
  leaveProtect(ofs, commandData.protect);
  if (!commandData.handleCommand)
  {
    ofs << "#ifdef VKCPP_ENHANCED_MODE" << std::endl;
    writeTypeCommandEnhanced(ofs, "  ", "", dependencyData, commandData, vkTypes);
    ofs << "#endif /*VKCPP_ENHANCED_MODE*/" << std::endl;
  }
  ofs << std::endl;
}

void writeTypeCommandEnhanced(std::ofstream & ofs, std::string const& indentation, std::string const& className, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes)
{
  enterProtect(ofs, commandData.protect);
  std::map<size_t, size_t> vectorParameters = getVectorParameters(commandData);
  size_t returnIndex = findReturnIndex(commandData, vectorParameters);
  size_t templateIndex = findTemplateIndex(commandData, vectorParameters);
  std::map<size_t, size_t>::const_iterator returnVector = vectorParameters.find(returnIndex);
  std::string returnType = determineReturnType(commandData, returnIndex, returnVector != vectorParameters.end());
  std::string functionName = determineFunctionName(dependencyData.name, commandData);

  writeFunctionHeader(ofs, indentation, returnType, functionName, commandData, returnIndex, templateIndex, vectorParameters);

  if (commandData.twoStep)
  {
    writeTypeCommandEnhancedTwoStep(ofs, indentation, className, functionName, returnType, templateIndex, dependencyData, commandData, vkTypes, returnIndex, vectorParameters);
  }
  else
  {
    writeTypeCommandEnhancedSingleStep(ofs, indentation, className, functionName, returnType, dependencyData, commandData, vkTypes, returnIndex, templateIndex, vectorParameters);
  }
  leaveProtect(ofs, commandData.protect);
}

void writeTypeCommandEnhanced(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, size_t templateIndex, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes, std::map<size_t, size_t> const& vectorParameters)
{
  ofs << indentation << "{" << std::endl;
  if (vectorParameters.size() == 2)
  {
    std::map<size_t, size_t>::const_iterator it1 = vectorParameters.begin();
    std::map<size_t, size_t>::const_iterator it0 = it1++;
    if ( it0->second == it1->second )
    {
      ofs << indentation << "  if ( " << reduceName(commandData.arguments[it0->first].name) << ".size() != " << reduceName(commandData.arguments[it1->first].name) << ".size() )" << std::endl
          << indentation << "  {" << std::endl
          << indentation << "    throw std::logic_error( \"vk::" << className << "::" << functionName << ": " << reduceName(commandData.arguments[it0->first].name) << ".size() != " << reduceName(commandData.arguments[it1->first].name) << ".size()\" );" << std::endl
          << indentation << "  }" << std::endl;
    }
  }
  if ((templateIndex != ~0) && (commandData.arguments[templateIndex].pureType != "void"))
  {
    ofs << indentation << "  static_assert( sizeof( T ) % sizeof( " << commandData.arguments[templateIndex].pureType << " ) == 0, \"wrong size of template type T\" );" << std::endl;
  }
  ofs << indentation << "  ";
  if (commandData.returnType == "Result")
  {
    ofs << "Result result = static_cast<Result>( ";
  }
  else if (commandData.returnType != "void")
  {
    ofs << "return ";
  }
  writeCall(ofs, dependencyData.name, templateIndex, commandData, vkTypes, vectorParameters);
  if (commandData.returnType == "Result")
  {
    ofs << " );" << std::endl;
    writeExceptionCheck(ofs, indentation, className, functionName, commandData.successCodes);
    if (1 < commandData.successCodes.size())
    {
      ofs << indentation << "  return result;" << std::endl;
    }
  }
  else
  {
    ofs << ";" << std::endl;
  }
  ofs << indentation << "}" << std::endl;
}

void writeTypeCommandEnhancedSingleStep(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, std::string const& returnType, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes, size_t returnIndex, size_t templateIndex, std::map<size_t, size_t> const& vectorParameters)
{
  if (1 < commandData.successCodes.size())
  {
    returnIndex = ~0;
  }
  if ((commandData.returnType == "Result") || (commandData.returnType == "void"))
  {
    // is allowed to return some input parameter
    if (returnIndex == ~0)
    {
      writeTypeCommandEnhanced(ofs, indentation, className, functionName, templateIndex, dependencyData, commandData, vkTypes, vectorParameters);
    }
    else
    {
      std::map<size_t, size_t>::const_iterator it = vectorParameters.find(returnIndex);
      if (it == vectorParameters.end() || (it->second == ~0) || ((vectorParameters.size() == 2) && (vectorParameters.begin()->second == (++vectorParameters.begin())->second)))
      {
        writeTypeCommandEnhancedReplaceReturn(ofs, indentation, className, functionName, returnType, templateIndex, dependencyData, commandData, vkTypes, returnIndex, vectorParameters);
      }
      else
      {
        assert(false);
      }
    }
  }
  else
  {
    writeTypeCommandEnhanced(ofs, indentation, className, functionName, templateIndex, dependencyData, commandData, vkTypes, std::map<size_t, size_t>());
  }
}

void writeTypeCommandEnhancedTwoStep(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, std::string const& returnType, size_t templateIndex, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes, size_t returnIndex, std::map<size_t, size_t> const& vectorParameters)
{
  assert(returnIndex != ~0);
  std::map<size_t, size_t>::const_iterator it = vectorParameters.find(returnIndex);
  assert((it != vectorParameters.end()) && (it->second != ~0));

  ofs << indentation << "{" << std::endl;
  if (commandData.successCodes.size() < 2)
  {
    ofs << indentation << "  " << returnType << " " << reduceName(commandData.arguments[it->first].name) << ";" << std::endl;
  }
  ofs << indentation << "  " << commandData.arguments[it->second].pureType << " " << reduceName(commandData.arguments[it->second].name) << ";" << std::endl
      << indentation << "  ";
  if (commandData.returnType == "Result")
  {
    ofs << "Result result = static_cast<Result>( ";
    writeCall(ofs, dependencyData.name, templateIndex, commandData, vkTypes, vectorParameters, returnIndex, "nullptr");
    ofs << " );" << std::endl;
    writeExceptionCheck(ofs, indentation, className, functionName, commandData.successCodes);
  }
  else
  {
    writeCall(ofs, dependencyData.name, templateIndex, commandData, vkTypes, vectorParameters, returnIndex, "nullptr");
    ofs << ";" << std::endl;
  }
  ofs << indentation << "  " << reduceName(commandData.arguments[it->first].name) << ".resize( " << reduceName(commandData.arguments[it->second].name) << " );" << std::endl
      << indentation << "  ";
  if (commandData.returnType == "Result")
  {
    ofs << "result = static_cast<Result>( ";
    writeCall(ofs, dependencyData.name, templateIndex, commandData, vkTypes, vectorParameters, returnIndex, reduceName(commandData.arguments[returnIndex].name) + ".data()");
    ofs << " );" << std::endl;
    writeExceptionCheck(ofs, indentation, className, functionName, commandData.successCodes);
  }
  else
  {
    writeCall(ofs, dependencyData.name, templateIndex, commandData, vkTypes, vectorParameters, returnIndex, reduceName(commandData.arguments[returnIndex].name) + ".data()");
    ofs << ";" << std::endl;
  }
  if (commandData.successCodes.size() < 2)
  {
    ofs << indentation << "  return std::move( " << reduceName(commandData.arguments[returnIndex].name) << " );" << std::endl;
  }
  else if (commandData.returnType == "Result")
  {
    ofs << indentation << "  return result;" << std::endl;
  }
  ofs << indentation << "}" << std::endl;
}

void writeTypeCommandEnhancedReplaceReturn(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, std::string const& returnType, size_t templateIndex, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes, size_t returnIndex, std::map<size_t, size_t> const& vectorParameters)
{
  assert(returnIndex < commandData.arguments.size());

  ofs << indentation << "{" << std::endl
      << indentation << "  " << returnType << " " << reduceName(commandData.arguments[returnIndex].name);
  std::map<size_t, size_t>::const_iterator it = vectorParameters.find(returnIndex);
  if (it != vectorParameters.end())
  {
    std::string size;
    if ((it->second == ~0) && !commandData.arguments[returnIndex].len.empty())
    {
      size = reduceName(commandData.arguments[returnIndex].len);
      size_t pos = size.find("->");
      assert(pos != std::string::npos);
      size.replace(pos, 2, ".");
      size += "()";
    }
    else
    {
      for (std::map<size_t, size_t>::const_iterator sit = vectorParameters.begin(); sit != vectorParameters.end(); ++sit)
      {
        if ((sit->first != returnIndex) && (sit->second == it->second))
        {
          size = reduceName(commandData.arguments[sit->first].name) + ".size()";
          break;
        }
      }
    }
    assert(!size.empty());
    ofs << "( " << size << " )";
  }
  ofs << ";" << std::endl
      << indentation << "  ";
  if (commandData.returnType == "Result")
  {
    ofs << "Result result = static_cast<Result>( ";
  }
  writeCall(ofs, dependencyData.name, templateIndex, commandData, vkTypes, vectorParameters);
  if (commandData.returnType == "Result")
  {
    ofs << " )";
  }
  ofs << ";" << std::endl;
  if (commandData.returnType == "Result")
  {
    writeExceptionCheck(ofs, indentation, className, functionName, commandData.successCodes);
  }
  ofs << indentation << "  return ";
  if (it != vectorParameters.end())
  {
    ofs << "std::move( ";
  }
  ofs << reduceName(commandData.arguments[returnIndex].name);
  if (it != vectorParameters.end())
  {
    ofs << " )";
  }
  ofs << ";" << std::endl
      << indentation << "}" << std::endl;
}

void writeTypeCommandStandard(std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes)
{
  ofs << "  inline " << commandData.returnType << " " << dependencyData.name << "( ";
  for (size_t i = 0; i<commandData.arguments.size(); i++)
  {
    if (0 < i)
    {
      ofs << ", ";
    }
    ofs << commandData.arguments[i].type << " " << commandData.arguments[i].name;
    if (!commandData.arguments[i].arraySize.empty())
    {
      ofs << "[" << commandData.arguments[i].arraySize << "]";
    }
  }
  ofs << " )" << std::endl
    << "  {" << std::endl
    << "    ";
  bool castReturn = false;
  if (commandData.returnType != "void")
  {
    ofs << "return ";
    castReturn = (vkTypes.find(commandData.returnType) != vkTypes.end());
    if (castReturn)
    {
      ofs << "static_cast<" << commandData.returnType << ">( ";
    }
  }

  std::string callName(dependencyData.name);
  assert(islower(callName[0]));
  callName[0] = toupper(callName[0]);

  ofs << "vk" << callName << "( ";
  for (size_t i = 0; i<commandData.arguments.size(); i++)
  {
    if (0 < i)
    {
      ofs << ", ";
    }
    writeMemberData(ofs, commandData.arguments[i], vkTypes);
  }
  ofs << " )";
  if (castReturn)
  {
    ofs << " )";
  }
  ofs << ";" << std::endl
    << "  }" << std::endl
    << std::endl;
}

void writeTypeCommandComplexBody(std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::map<std::string,std::string> const& nameMap, std::map<size_t, size_t> const& vectorParameters, std::set<size_t> const& argIndices, size_t complexIndex, size_t returnIndex)
{
  if (returnIndex != ~0)
  {
    assert(vectorParameters.find(returnIndex) != vectorParameters.end());
    ofs << "    std::vector<" << commandData.arguments[returnIndex].pureType << "> " << reduceName(commandData.arguments[returnIndex].name) << ";" << std::endl;
  }

  std::map<size_t, size_t>::const_iterator vpit = vectorParameters.find(complexIndex);
  assert((vpit != vectorParameters.end()) && (vpit->second != ~0));
  ofs << "    " << commandData.arguments[vpit->second].pureType << " " << nameMap.find(commandData.arguments[vpit->second].name)->second << " = 0;" << std::endl;
  ofs << "    ";

  assert((commandData.returnType == "void") || (commandData.returnType == "Result"));
  if (commandData.returnType == "Result")
  {
    ofs << commandData.returnType << " result = ";
  }
  ofs << dependencyData.name << "( ";
  for (size_t i = 0; i<commandData.arguments.size(); i++)
  {
    auto argit = nameMap.find(commandData.arguments[i].name);
    if (0 < i)
    {
      ofs << ", ";
    }
    if (i == vpit->first)
    {
      ofs << "nullptr";
    }
    else if (i == vpit->second)
    {
      ofs << '&' << argit->second;
    }
    else if (argIndices.find(i) != argIndices.end())
    {
      ofs << argit->second << ".data()";
    }
    else
    {
      ofs << argit->second;
    }
  }
  ofs << " );" << std::endl;
  if (commandData.returnType == "Result")
  {
    ofs << "    if ( result == Result::eSuccess )" << std::endl
        << "    {" << std::endl
        << "  ";
  }
  ofs << "    ";
  if (returnIndex == ~0)
  {
    ofs << nameMap.find(commandData.arguments[vpit->first].name)->second << ".resize( " << nameMap.find(commandData.arguments[vpit->second].name)->second << " );" << std::endl;
  }
  else
  {
    ofs << "std::vector<" << commandData.arguments[vpit->first].pureType << "> " << nameMap.find(commandData.arguments[vpit->first].name)->second << "( " << nameMap.find(commandData.arguments[vpit->second].name)->second << " );" << std::endl;
  }
  ofs << "    ";
  if (commandData.returnType == "Result")
  {
    ofs << "  result = ";
  }
  ofs << dependencyData.name << "( ";
  for (size_t i = 0; i<commandData.arguments.size(); i++)
  {
    auto argit = nameMap.find(commandData.arguments[i].name);
    if (0 < i)
    {
      ofs << ", ";
    }
    if (i == vpit->first)
    {
      ofs << argit->second << ".data()";
    }
    else if (i == vpit->second)
    {
      ofs << '&' << argit->second;
    }
    else if (argIndices.find(i) != argIndices.end())
    {
      ofs << argit->second << ".data()";
    }
    else
    {
      ofs << argit->second;
    }
  }
  ofs << " );" << std::endl;
  if (returnIndex != ~0)
  {
    ofs << "    }" << std::endl
        << "    return std::move( " << nameMap.find(commandData.arguments[vpit->first].name)->second << " );" << std::endl;
  }
  else if (commandData.returnType == "Result")
  {
    ofs << "    }" << std::endl
      << "    return result;" << std::endl;
  }
}

void writeTypeCommandSimpleBody(std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::map<std::string, std::string> const& nameMap, std::map<size_t, size_t> const& vectorParameters, std::set<size_t> const& argIndices, std::map<size_t, std::vector<size_t>> const& sizeIndices, size_t returnIndex)
{
  for (std::map<size_t, std::vector<size_t>>::const_iterator it = sizeIndices.begin(); it != sizeIndices.end(); ++it)
  {
    if (1 < it->second.size())
    {
      assert(it->second.size() == 2);
      ofs << "    assert( " << nameMap.find(commandData.arguments[it->second[0]].name)->second << ".size() <= " << nameMap.find(commandData.arguments[it->second[1]].name)->second << ".size() );" << std::endl;
    }
  }

  if (returnIndex != ~0)
  {
    assert(vectorParameters.find(returnIndex) == vectorParameters.end());
    ofs << "    " << commandData.arguments[returnIndex].pureType << " " << reduceName(commandData.arguments[returnIndex].name) << ";" << std::endl;
  }

  ofs << "    ";
  if (commandData.returnType != "void")
  {
    ofs << "return ";
  }
  ofs << dependencyData.name << "( ";
  for (size_t i = 0; i < commandData.arguments.size(); i++)
  {
    auto argit = nameMap.find(commandData.arguments[i].name);
    if (0 < i)
    {
      ofs << ", ";
    }
    if (argIndices.find(i) != argIndices.end())
    {
      ofs << argit->second << ".data()";
    }
    else
    {
      std::map<size_t, std::vector<size_t>>::const_iterator it = sizeIndices.find(i);
      if (it != sizeIndices.end())
      {
        ofs << "static_cast<" << commandData.arguments[i].type << ">( " << nameMap.find(commandData.arguments[it->second.front()].name)->second << ".size() )";
      }
      else if (nameMap.find(commandData.arguments[i].name)->second != commandData.arguments[i].name)
      {
        ofs << '&' << argit->second;
      }
      else
      {
        ofs << argit->second;
      }
    }
  }
  ofs << " );" << std::endl;
}

void writeTypeEnum( std::ofstream & ofs, DependencyData const& dependencyData, EnumData const& enumData )
{
  enterProtect(ofs, enumData.protect);
  ofs << "  enum class " << dependencyData.name << std::endl
      << "  {" << std::endl;
  for ( size_t i=0 ; i<enumData.members.size() ; i++ )
  {
    ofs << "    " << enumData.members[i].name << " = " << enumData.members[i].value;
    if ( i < enumData.members.size() - 1 )
    {
      ofs << ",";
    }
    ofs << std::endl;
  }
  ofs << "  };" << std::endl;
  leaveProtect(ofs, enumData.protect);
  ofs << std::endl;
}

void writeEnumGetString(std::ofstream & ofs, DependencyData const& dependencyData, EnumData const& enumData)
{
  enterProtect(ofs, enumData.protect);
  ofs << "  inline std::string getString(" << dependencyData.name << (enumData.members.empty() ? ")" : " value)") << std::endl
      << "  {" << std::endl;
  if (enumData.members.empty())
  {
    ofs << "    return std::string();" << std::endl;
  }
  else
  {
    ofs << "    switch (value)" << std::endl
        << "    {" << std::endl;
    for (auto itMember = enumData.members.begin(); itMember != enumData.members.end(); ++itMember)
    {
      ofs << "    case " << dependencyData.name << "::" << itMember->name << ": return \"" << itMember->name.substr(1) << "\";" << std::endl;
    }
    ofs << "    default: return \"unknown\";" << std::endl
        << "    }" << std::endl;
  }
  ofs << "  }" << std::endl;
  leaveProtect(ofs, enumData.protect);
  ofs << std::endl;
}

void writeFlagsGetString(std::ofstream & ofs, DependencyData const& dependencyData, EnumData const &enumData)
{
  enterProtect(ofs, enumData.protect);
  std::string enumPrefix = "vk::" + *dependencyData.dependencies.begin() + "::";
  ofs << "  inline std::string getString(" << dependencyData.name << (enumData.members.empty() ? ")" : " value)") << std::endl
      << "  {" << std::endl;
  if (enumData.members.empty())
  {
    ofs << "    return std::string();" << std::endl;
  }
  else
  {
    ofs << "    if (!value) return std::string();" << std::endl
        << "    std::string result;" << std::endl;

    for (auto itMember = enumData.members.begin(); itMember != enumData.members.end(); ++itMember)
    {
      ofs << "    if (value & " << enumPrefix + itMember->name << ") result += \"" << itMember->name.substr(1) << " | \";" << std::endl;
    }
    ofs << "    return result.substr(0, result.size() - 3);" << std::endl;
  }
  ofs << "  }" << std::endl;
  leaveProtect(ofs, enumData.protect);
  ofs << std::endl;
}

void writeEnumsToString(std::ofstream & ofs, std::vector<DependencyData> const& dependencyData, std::map<std::string, EnumData> const& enums)
{
  for (auto it = dependencyData.begin(); it != dependencyData.end(); ++it)
  {
    switch (it->category)
    {
    case DependencyData::Category::ENUM:
      assert(enums.find(it->name) != enums.end());
      writeEnumGetString(ofs, *it, enums.find(it->name)->second);
      break;
    case DependencyData::Category::FLAGS:
        writeFlagsGetString(ofs, *it, enums.find(*it->dependencies.begin())->second);
      break;
    }
  }
}

void writeTypeFlags( std::ofstream & ofs, DependencyData const& dependencyData, FlagData const& flagData )
{
  assert( dependencyData.dependencies.size() == 1 );
  enterProtect(ofs, flagData.protect);
  ofs << "  typedef Flags<" << *dependencyData.dependencies.begin() << ", Vk" << dependencyData.name << "> " << dependencyData.name << ";" << std::endl
      << std::endl
      << "  inline " << dependencyData.name << " operator|( " << *dependencyData.dependencies.begin() << " bit0, " << *dependencyData.dependencies.begin() << " bit1 )" << std::endl
      << "  {" << std::endl
      << "    return " << dependencyData.name << "( bit0 ) | bit1;" << std::endl
      << "  }" << std::endl;
  leaveProtect(ofs, flagData.protect);
  ofs << std::endl;
}

void writeTypeHandle(std::ofstream & ofs, DependencyData const& dependencyData, HandleData const& handle, std::map<std::string, CommandData> const& commands, std::vector<DependencyData> const& dependencies, std::set<std::string> const& vkTypes)
{
  std::string memberName = dependencyData.name;
  assert(isupper(memberName[0]));
  memberName[0] = tolower(memberName[0]);

  ofs << "  class " << dependencyData.name << std::endl
      << "  {" << std::endl
      << "  public:" << std::endl
      << "    " << dependencyData.name << "()" << std::endl
      << "      : m_" << memberName << "(VK_NULL_HANDLE)" << std::endl
      << "    {}" << std::endl
      << std::endl
      << "#if defined(VK_CPP_TYPESAFE_CONVERSION)" << std::endl
      // construct from native handle
      << "    " << dependencyData.name << "(Vk" << dependencyData.name << " " << memberName << ")" << std::endl
      << "       : m_" << memberName << "("  << memberName << ")" << std::endl
      << "    {}" << std::endl
      << std::endl
      << "    " << dependencyData.name << "& operator=(Vk" << dependencyData.name << " " << memberName << ")" << std::endl
      << "    {" << std::endl
      << "      m_" << memberName << " = " << memberName << ";" << std::endl
      << "      return *this;" << std::endl
      << "    }" << std::endl
      << "#endif\n"
      << std::endl;
  if (!handle.commands.empty())
  {
    ofs << "#ifdef VKCPP_ENHANCED_MODE" << std::endl;
    for (size_t i = 0; i < handle.commands.size(); i++)
    {
      std::string commandName = handle.commands[i];
      std::map<std::string, CommandData>::const_iterator cit = commands.find(commandName);
      assert((cit != commands.end()) && cit->second.handleCommand);
      std::vector<DependencyData>::const_iterator dep = std::find_if(dependencies.begin(), dependencies.end(), [commandName](DependencyData const& dd) { return dd.name == commandName; });
      assert(dep != dependencies.end());
      writeTypeCommandEnhanced(ofs, "    ", dependencyData.name, *dep, cit->second, vkTypes);
      if (i < handle.commands.size() - 1)
      {
        ofs << std::endl;
      }
    }
    ofs << "#endif /*VKCPP_ENHANCED_MODE*/" << std::endl
        << std::endl;
  }
  ofs << "#if !defined(VK_CPP_TYPESAFE_CONVERSION)" << std::endl
      << "    explicit" << std::endl
      << "#endif" << std::endl
      << "    operator Vk" << dependencyData.name << "() const" << std::endl
      << "    {" << std::endl
      << "      return m_" << memberName << ";" << std::endl
      << "    }" << std::endl
      << std::endl
      << "    explicit operator bool() const" << std::endl
      << "    {" << std::endl
      << "      return m_" << memberName << " != VK_NULL_HANDLE;" << std::endl
      << "    }" << std::endl
      << std::endl
      << "    bool operator!() const" << std::endl
      << "    {" << std::endl
      << "      return m_" << memberName << " == VK_NULL_HANDLE;" << std::endl
      << "    }" << std::endl
      << std::endl
      << "  private:" << std::endl
      << "    Vk" << dependencyData.name << " m_" << memberName << ";" << std::endl
      << "  };" << std::endl
#if 1
      << "  static_assert( sizeof( " << dependencyData.name << " ) == sizeof( Vk" << dependencyData.name << " ), \"handle and wrapper have different size!\" );" << std::endl
#endif
      << std::endl;
}

void writeTypeScalar( std::ofstream & ofs, DependencyData const& dependencyData )
{
  assert( dependencyData.dependencies.size() == 1 );
  ofs << "  typedef " << *dependencyData.dependencies.begin() << " " << dependencyData.name << ";" << std::endl;
}

void writeTypeStruct( std::ofstream & ofs, DependencyData const& dependencyData, std::set<std::string> const& vkTypes, std::map<std::string,StructData> const& structs, std::map<std::string,std::string> const& defaultValues )
{
  std::map<std::string,StructData>::const_iterator it = structs.find( dependencyData.name );
  assert( it != structs.end() );

  enterProtect(ofs, it->second.protect);
  ofs << "  class " << dependencyData.name << std::endl
      << "  {" << std::endl
      << "  public:" << std::endl;

  std::string memberName( dependencyData.name );
  assert( isupper( memberName[0] ) );
  memberName[0] = tolower( memberName[0] );
  memberName = "m_" + memberName;

  // only structs that are not returnedOnly get a constructor!
  if ( !it->second.returnedOnly )
  {
    writeStructConstructor( ofs, dependencyData.name, memberName, it->second, vkTypes, defaultValues );
  }

  // create the getters and setters
  for ( size_t i=0 ; i<it->second.members.size() ; i++ )
  {
    writeStructGetter( ofs, it->second.members[i], memberName, vkTypes );
    if ( !it->second.returnedOnly )
    {
      writeStructSetter( ofs, dependencyData.name, it->second.members[i], memberName, vkTypes );
    }
  }

  // null handle
  ofs << "    static " << dependencyData.name << "& null()" << std::endl
      << "    {" << std::endl
      << "      return *((" << dependencyData.name << "*)(nullptr));" << std::endl
      << "    }" << std::endl
      << std::endl;

  // the cast-operator to the wrapped struct, and the struct itself as a private member variable
  ofs << "    operator const Vk" << dependencyData.name << "&() const" << std::endl
      << "    {" << std::endl
      << "      return " << memberName << ";" << std::endl
      << "    }" << std::endl
      << std::endl
      << "  private:" << std::endl
      << "    Vk" << dependencyData.name << " " << memberName << ";" << std::endl
      << "  };" << std::endl;
#if 1
  ofs << "  static_assert( sizeof( " << dependencyData.name << " ) == sizeof( Vk" << dependencyData.name << " ), \"struct and wrapper have different size!\" );" << std::endl;
#endif
  leaveProtect(ofs, it->second.protect);
  ofs << std::endl;
}

void writeTypeUnion( std::ofstream & ofs, DependencyData const& dependencyData, StructData const& unionData, std::set<std::string> const& vkTypes, std::map<std::string,StructData> const& structs, std::map<std::string,std::string> const& defaultValues )
{
  std::ostringstream oss;
  ofs << "  class " << dependencyData.name << std::endl
      << "  {" << std::endl
      << "  public:" << std::endl;

  std::string memberName( dependencyData.name );
  assert( isupper( memberName[0] ) );
  memberName[0] = tolower( memberName[0] );
  memberName = "m_" + memberName;

  for ( size_t i=0 ; i<unionData.members.size() ; i++ )
  {
    // one constructor per union element
    ofs << "    " << dependencyData.name << "( ";
    if ( unionData.members[i].arraySize.empty() )
    {
      ofs << unionData.members[i].type << " ";
    }
    else
    {
      ofs << "const std::array<" << unionData.members[i].type << "," << unionData.members[i].arraySize << ">& ";
    }
    ofs << unionData.members[i].name;

    // just the very first constructor gets default arguments
    if ( i == 0 )
    {
      std::map<std::string,std::string>::const_iterator it = defaultValues.find( unionData.members[i].pureType );
      assert( it != defaultValues.end() );
      if ( unionData.members[i].arraySize.empty() )
      {
        ofs << " = " << it->second;
      }
      else
      {
        ofs << " = { " << it->second << " }";
      }
    }
    ofs << " )" << std::endl
        << "    {" << std::endl
        << "      ";
    if ( unionData.members[i].arraySize.empty() )
    {
      ofs << memberName << "." << unionData.members[i].name << " = " << unionData.members[i].name;
    }
    else
    {
      ofs << "memcpy( &" << memberName << "." << unionData.members[i].name << ", " << unionData.members[i].name << ".data(), " << unionData.members[i].arraySize << " * sizeof( " << unionData.members[i].type << " ) )";
    }
    ofs << ";" << std::endl
        << "    }" << std::endl
        << std::endl;

    // one getter/setter per union element
    writeStructGetter( ofs, unionData.members[i], memberName, vkTypes );
    writeStructSetter( ofs, dependencyData.name, unionData.members[i], memberName, vkTypes );
  }
  ofs << "    operator Vk" << dependencyData.name << " const& () const" << std::endl
      << "    {" << std::endl
      << "      return " << memberName << ";" << std::endl
      << "    }" << std::endl
      << std::endl
      << "  private:" << std::endl
      << "    Vk" << dependencyData.name << " " << memberName << ";" << std::endl
      << "  };" << std::endl
      << std::endl;
}

void writeTypes(std::ofstream & ofs, std::vector<DependencyData> const& dependencies, std::map<std::string, CommandData> const& commands, std::map<std::string, EnumData> const& enums, std::map<std::string, FlagData> const& flags, std::map<std::string, HandleData> const& handles, std::map<std::string, StructData> const& structs, std::map<std::string, std::string> const& defaultValues, std::set<std::string> const& vkTypes)
{
  for ( std::vector<DependencyData>::const_iterator it = dependencies.begin() ; it != dependencies.end() ; ++it )
  {
    switch( it->category )
    {
      case DependencyData::Category::COMMAND :
        assert( commands.find( it->name ) != commands.end() );
        writeTypeCommand( ofs, *it, commands.find( it->name )->second, vkTypes );
        break;
      case DependencyData::Category::ENUM :
        assert( enums.find( it->name ) != enums.end() );
        writeTypeEnum( ofs, *it, enums.find( it->name )->second );
        break;
      case DependencyData::Category::FLAGS :
        assert(flags.find(it->name) != flags.end());
        writeTypeFlags( ofs, *it, flags.find( it->name)->second );
        break;
      case DependencyData::Category::FUNC_POINTER :
      case DependencyData::Category::REQUIRED :
        // skip FUNC_POINTER and REQUIRED, they just needed to be in the dependencies list to resolve dependencies
        break;
      case DependencyData::Category::HANDLE :
        assert(handles.find(it->name) != handles.end());
        writeTypeHandle( ofs, *it, handles.find( it->name )->second, commands, dependencies, vkTypes );
        break;
      case DependencyData::Category::SCALAR :
        writeTypeScalar( ofs, *it );
        break;
      case DependencyData::Category::STRUCT :
        writeTypeStruct( ofs, *it, vkTypes, structs, defaultValues );
        break;
      case DependencyData::Category::UNION :
        assert( structs.find( it->name ) != structs.end() );
        writeTypeUnion( ofs, *it, structs.find( it->name )->second, vkTypes, structs, defaultValues );
        break;
      default :
        assert( false );
        break;
    }
  }
}

void writeVersionCheck(std::ofstream & ofs, std::string const& version)
{
  ofs << "static_assert( VK_MAKE_VERSION" << version << " == VK_API_VERSION, \"Wrong VK_API_VERSION!\" );" << std::endl
      << std::endl;
}

void writeTypesafeCheck(std::ofstream & ofs, std::string const& typesafeCheck)
{
  ofs << "// 32-bit vulkan is not typesafe for handles, so don't allow copy constructors on this platform by default." << std::endl
      << "// To enable this feature on 32-bit platforms please define VK_CPP_TYPESAFE_CONVERSION" << std::endl
      << typesafeCheck << std::endl
      << "#define VK_CPP_TYPESAFE_CONVERSION 1" << std::endl
      << "#endif" << std::endl
      << std::endl;
}

int main( int argc, char **argv )
{
  tinyxml2::XMLDocument doc;

  tinyxml2::XMLError error = doc.LoadFile( argv[1] );
  if (error != tinyxml2::XML_SUCCESS)
  {
    std::cout << "VkGenerate: failed to load file " << argv[1] << " . Error code: " << error << std::endl;
    return -1;
  }

  tinyxml2::XMLElement * registryElement = doc.FirstChildElement();
  assert( strcmp( registryElement->Value(), "registry" ) == 0 );
  assert( !registryElement->NextSiblingElement() );

  std::string                         version;
  std::string                         typesafeCheck;
  std::list<DependencyData>           dependencies;
  std::map<std::string, CommandData>  commands;
  std::map<std::string, EnumData>     enums;
  std::map<std::string, FlagData>     flags;
  std::map<std::string, HandleData>   handles;
  std::map<std::string, ScalarData>   scalars;
  std::map<std::string, StructData>   structs;
  std::set<std::string>               tags;
  std::set<std::string>               vkTypes;

  tinyxml2::XMLElement * child = registryElement->FirstChildElement();
  do
  {
    assert( child->Value() );
    const std::string value = child->Value();
    if ( value == "commands" )
    {
      readCommands( child, dependencies, commands, handles, tags );
    }
    else if ( value == "enums" )
    {
      readEnums( child, dependencies, enums, tags, vkTypes );
    }
    else if ( value == "extensions" )
    {
      readExtensions( child, commands, enums, flags, scalars, structs, tags );
    }
    else if (value == "tags")
    {
      readTags(child, tags);
    }
    else if ( value == "types" )
    {
      readTypes( child, version, typesafeCheck, dependencies, flags, scalars, structs, vkTypes, handles, enums );
    }
    else
    {
      assert( ( value == "comment" ) || ( value == "feature" ) || ( value == "vendorids" ) );
    }
  } while ( child = child->NextSiblingElement() );

  std::vector<DependencyData> sortedDependencies;
  sortDependencies( dependencies, sortedDependencies );

  std::map<std::string,std::string> defaultValues;
  createDefaults( sortedDependencies, enums, defaultValues );

  std::ofstream ofs( "vk_cpp.h" );
  ofs << licenseHeader << std::endl;

  ofs << std::endl << "#ifndef VK_CPP_H_" << std::endl
                   << "#define VK_CPP_H_" << std::endl
                   << std::endl;

  ofs << "#include <array>" << std::endl
      << "#include <cassert>" << std::endl
      << "#include <cstdint>" << std::endl
      << "#include <cstring>" << std::endl
      << "#include <vulkan/vulkan.h>" << std::endl
      << "#ifdef VKCPP_ENHANCED_MODE" << std::endl
      << "# include <vector>" << std::endl
      << "#endif /*VKCPP_ENHANCED_MODE*/" << std::endl
      << std::endl;
  writeVersionCheck( ofs, version );
  writeTypesafeCheck(ofs, typesafeCheck );
  ofs << "namespace vk" << std::endl
      << "{" << std::endl;

  ofs << exceptionHeader;
  ofs << flagsHeader;

  writeTypes( ofs, sortedDependencies, commands, enums, flags, handles, structs, defaultValues, vkTypes );
  writeEnumsToString(ofs, sortedDependencies, enums);

  ofs << "} // namespace vk" << std::endl
      << std::endl
      << "#endif" << std::endl;
}
