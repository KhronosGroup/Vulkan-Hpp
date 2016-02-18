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

#define VK_PROTOTYPES

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

  void addEnum(std::string const & name);
};

struct CommandData
{
  std::string             returnType;
  std::vector<MemberData> arguments;
};

struct ExtensionData
{
  std::string               name;
  std::string               protect;
  std::vector<std::string>  elements;
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
  size_t                extension;
};

void createDefaults( std::vector<DependencyData> const& dependencies, std::map<std::string,EnumData> const& enums, std::map<std::string,std::string> & defaultValues );
size_t findComplexIndex(CommandData const& commandData, std::vector<std::pair<size_t, size_t>> const& lenParameters);
size_t findReturnIndex(CommandData const& commandData, std::vector<std::pair<size_t, size_t>> const& lenParameters);
std::string getEnumName(std::string const& name); // get vkcpp enum name from vk enum name
std::vector<std::pair<size_t, size_t>> getLenParameters(CommandData const& commandData);
bool noDependencies(std::set<std::string> const& dependencies, std::map<std::string, std::string> & listedTypes);
void readCommandParam( tinyxml2::XMLElement * element, DependencyData & typeData, std::vector<MemberData> & arguments );
CommandData & readCommandProto( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,CommandData> & commands );
void readCommands( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,CommandData> & commands );
void readCommandsCommand( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,CommandData> & commands );
void readEnums( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,EnumData> & enums, std::set<std::string> & vkTypes );
void readEnumsEnum( tinyxml2::XMLElement * element, EnumData & enumData );
void readExtensionRequire( tinyxml2::XMLElement * element, std::vector<std::string> & elements, std::map<std::string, EnumData> & enums );
void readExtensions( tinyxml2::XMLElement * element, std::vector<ExtensionData> & extensions, std::map<std::string, EnumData> & enums );
void readExtensionsExtension(tinyxml2::XMLElement * element, std::vector<ExtensionData> & extensions, std::map<std::string, EnumData> & enums);
void readTypeBasetype( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies );
void readTypeBitmask( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::set<std::string> & flags, std::set<std::string> & vkTypes );
void readTypeDefine( tinyxml2::XMLElement * element, std::string & version );
void readTypeFuncpointer( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies );
void readTypeHandle( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies );
void readTypeStruct( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,StructData> & structs, std::set<std::string> & vkTypes );
void readTypeStructMember( tinyxml2::XMLElement * element, std::vector<MemberData> & members, std::set<std::string> & dependencies );
void readTypeUnion( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,StructData> & structs, std::set<std::string> & vkTypes );
void readTypeUnionMember( tinyxml2::XMLElement * element, std::vector<MemberData> & members, std::set<std::string> & dependencies );
void readTypes( tinyxml2::XMLElement * element, std::string & version, std::list<DependencyData> & dependencies, std::set<std::string> & flags, std::map<std::string,StructData> & structs, std::set<std::string> & vkTypes );
void sortDependencies( std::list<DependencyData> & dependencies, std::vector<ExtensionData> const& extensions, std::vector<std::vector<DependencyData>> & sortedDependencies );
std::string reduceName(std::string const& name);
std::string strip( std::string const& value, std::string const& prefix );
std::string toCamelCase(std::string const& value);
std::string toUpperCase(std::string const& name);
void writeMemberData(std::ofstream & ofs, MemberData const& memberData, std::set<std::string> const& vkTypes);
void writeStructConstructor( std::ofstream & ofs, std::string const& name, std::string const& memberName, StructData const& structData, std::set<std::string> const& vkTypes, std::map<std::string,std::string> const& defaultValues );
void writeStructGetter( std::ofstream & ofs, MemberData const& memberData, std::string const& memberName, std::set<std::string> const& vkTypes );
void writeStructSetter( std::ofstream & ofs, std::string const& name, MemberData const& memberData, std::string const& memberName, std::set<std::string> const& vkTypes, std::map<std::string,StructData> const& structs );
void writeTypeCommand( std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes );
void writeTypeCommandEnhanced(std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes);
void writeTypeCommandStandard(std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes);
void writeTypeCommandComplexBody(std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::map<std::string,std::string> const& nameMap, std::vector<std::pair<size_t, size_t>> const& lenParameters, std::set<size_t> const& argIndices, size_t complexIndex, size_t returnIndex);
void writeTypeCommandSimpleBody(std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::map<std::string,std::string> const& nameMap, std::set<size_t> const& argIndices, std::map<size_t,std::vector<size_t>> const& sizeIndices);
void writeTypeEnum(std::ofstream & ofs, DependencyData const& dependencyData, EnumData const& enumData);
void writeTypeFlags( std::ofstream & ofs, DependencyData const& dependencyData );
void writeTypeHandle( std::ofstream & ofs, DependencyData const& dependencyData );
void writeTypeScalar( std::ofstream & ofs, DependencyData const& dependencyData );
void writeTypeStruct( std::ofstream & ofs, DependencyData const& dependencyData, std::set<std::string> const& vkTypes, std::map<std::string,StructData> const& structs, std::map<std::string,std::string> const& defaultValues );
void writeTypeUnion( std::ofstream & ofs, DependencyData const& dependencyData, StructData const& unionData, std::set<std::string> const& vkTypes, std::map<std::string,StructData> const& structs, std::map<std::string,std::string> const& defaultValues );
void writeTypes( std::ofstream & ofs, std::vector<DependencyData> const& dependencies, std::map<std::string,CommandData> const& commands, std::map<std::string,EnumData> const& enums, std::map<std::string,StructData> const& structs, std::map<std::string,std::string> const& defaultValues, std::set<std::string> const& vkTypes );
void writeVersionCheck( std::ofstream & ofs, std::string const& version );

void EnumData::addEnum(std::string const & name)
{
  members.push_back(NameValue());
  members.back().name = "e" + toCamelCase(strip(name, prefix));
  members.back().value = name;
  if (!postfix.empty())
  {
    size_t pos = members.back().name.find(postfix);
    if (pos != std::string::npos)
    {
        members.back().name.erase(pos);
    }
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
        assert( enums.find( it->name ) != enums.end() );
        defaultValues[it->name] = it->name + "::" + enums.find( it->name )->second.members.front().name;
        break;
      case DependencyData::Category::FLAGS :
      case DependencyData::Category::STRUCT :
      case DependencyData::Category::UNION :        // just call the default constructor for flags, structs, and structs (which are mapped to classes)
        defaultValues[it->name] = it->name + "()";
        break;
      case DependencyData::Category::FUNC_POINTER : // func_pointers explicitly have no default!
        defaultValues[it->name];
        break;
      case DependencyData::Category::HANDLE :       // handles are pointers
        defaultValues[it->name] = it->name + "()";
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

size_t findComplexIndex(CommandData const& commandData, std::vector<std::pair<size_t, size_t>> const& lenParameters)
{
  for (size_t i = 0; i < lenParameters.size(); i++)
  {
    if ((lenParameters[i].second != ~0) && (commandData.arguments[lenParameters[i].second].type.find("*") != std::string::npos))
    {
#if !defined(NDEBUG)
      for (size_t j = i + 1; j < lenParameters.size(); j++)
      {
        assert((lenParameters[i].second == ~0) || (commandData.arguments[lenParameters[i].second].type.find("*") == std::string::npos));
      }
#endif
      return i;
    }
  }
  return ~0;
}

size_t findReturnIndex(CommandData const& commandData, std::vector<std::pair<size_t, size_t>> const& lenParameters)
{
  for (size_t i = 0; i < lenParameters.size(); i++)
  {
    if (commandData.arguments[lenParameters[i].first].type.find("const") == std::string::npos)
    {
#if !defined(NDEBUG)
      for (size_t j = i + 1; j < lenParameters.size(); j++)
      {
        assert(commandData.arguments[lenParameters[j].first].type.find("const") == 0);
      }
#endif
      return i;
    }
  }
  return ~0;
}

std::string getEnumName(std::string const& name) // get vkcpp enum name from vk enum name
{
  return strip(name, "Vk");
}

std::vector<std::pair<size_t, size_t>> getLenParameters(CommandData const& commandData)
{
  std::vector<std::pair<size_t,size_t>> lenParameters;
  for (size_t i = 0; i < commandData.arguments.size(); i++)
  {
    if (!commandData.arguments[i].len.empty())
    {
      lenParameters.push_back(std::make_pair(i, ~0));
      for (size_t j = 0; j < commandData.arguments.size(); j++)
      {
        if (commandData.arguments[i].len == commandData.arguments[j].name)
        {
          lenParameters.back().second = j;
        }
      }
      assert(   (lenParameters.back().second != ~0)
             || (commandData.arguments[i].len == "dataSize/4")
             || (commandData.arguments[i].len == "null-terminated")
             || (commandData.arguments[i].len == "pAllocateInfo->descriptorSetCount")
             || (commandData.arguments[i].len == "pAllocateInfo->commandBufferCount"));
      assert((lenParameters.back().second == ~0) || (lenParameters.back().second < lenParameters.back().first));
    }
  }
  return std::move(lenParameters);
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

void readCommandParam( tinyxml2::XMLElement * element, DependencyData & typeData, std::vector<MemberData> & arguments )
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
}

CommandData & readCommandProto( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,CommandData> & commands )
{
  tinyxml2::XMLElement * typeElement = element->FirstChildElement();
  assert( typeElement && ( strcmp( typeElement->Value(), "type" ) == 0 ) );
  tinyxml2::XMLElement * nameElement = typeElement->NextSiblingElement();
  assert( nameElement && ( strcmp( nameElement->Value(), "name" ) == 0 ) );
  assert( !nameElement->NextSiblingElement() );

  std::string type = strip( typeElement->GetText(), "Vk" );

  std::string name = strip( nameElement->GetText(), "vk" );
  assert( isupper( name[0] ) );
  name[0] = tolower( name[0] );

  dependencies.push_back( DependencyData( DependencyData::Category::COMMAND, name ) );
  assert( commands.find( name ) == commands.end() );
  std::map<std::string,CommandData>::iterator it = commands.insert( std::make_pair( name, CommandData() ) ).first;
  it->second.returnType = type;

  return it->second;
}

void readCommands( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,CommandData> & commands )
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child );
  do
  {
    assert( strcmp( child->Value(), "command" ) == 0 );
    readCommandsCommand( child, dependencies, commands );
  } while ( child = child->NextSiblingElement() );
}

void readCommandsCommand( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,CommandData> & commands )
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child && ( strcmp( child->Value(), "proto" ) == 0 ) );

  CommandData & commandData = readCommandProto( child, dependencies, commands );

  while ( child = child->NextSiblingElement() )
  {
    std::string value = child->Value();
    if ( value == "param" )
    {
      readCommandParam(child, dependencies.back(), commandData.arguments);
    }
    else
    {
      assert( ( value == "implicitexternsyncparams" ) || ( value == "validity" ) );
    }
  }
}

void readEnums( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::map<std::string,EnumData> & enums, std::set<std::string> & vkTypes )
{
  assert( element->Attribute( "name" ) );
  std::string name = getEnumName(element->Attribute("name"));
  if ( name != "API Constants" )
  {
    dependencies.push_back( DependencyData( DependencyData::Category::ENUM, name ) );
    std::map<std::string,EnumData>::iterator it = enums.insert( std::make_pair( name, EnumData() ) ).first;

    assert( element->Attribute( "type" ) );
    std::string type = element->Attribute( "type" );
    assert( ( type == "bitmask" ) || ( type == "enum" ) );
    it->second.bitmask = ( type == "bitmask" );
    std::string prefix, postfix;
    if ( it->second.bitmask )
    {
      size_t pos = name.find( "FlagBits" );
      assert( pos != std::string::npos );
      it->second.prefix = "VK" + toUpperCase( name.substr( 0, pos ) ) + "_";
      it->second.postfix = "Bit";
    }
    else
    {
      it->second.prefix = "VK" + toUpperCase( name ) + "_";
    }

    readEnumsEnum( element, it->second );

    assert( vkTypes.find( name ) == vkTypes.end() );
    vkTypes.insert( name );
  }
}

void readEnumsEnum( tinyxml2::XMLElement * element, EnumData & enumData )
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    if ( child->Attribute( "name" ) )
    {
      enumData.addEnum(child->Attribute("name"));
    }
  } while ( child = child->NextSiblingElement() );
}

void readExtensionRequire(tinyxml2::XMLElement * element, std::vector<std::string> & elements, std::map<std::string, EnumData> & enums)
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    std::string value = child->Value();
    assert( child->Attribute( "name" ) );

    if ( value == "command" )
    {
      elements.push_back( strip( child->Attribute( "name" ), "vk" ) );
      assert( isupper( elements.back()[0] ) );
      elements.back()[0] = tolower( elements.back()[0] );
    }
    else if ( value == "type" )
    {
      elements.push_back( strip( child->Attribute( "name" ), "Vk" ) );
    }
    else if ( value == "enum")
    {
      // TODO process enums which don't extend existing enums
      if (child->Attribute("extends"))
      {
        assert(enums.find(getEnumName(child->Attribute("extends"))) != enums.end());
        enums[getEnumName(child->Attribute("extends"))].addEnum(child->Attribute("name"));
      }
    }
    else
    {
        assert("unknown attribute, check me");
    }
  } while ( child = child->NextSiblingElement() );
}

void readExtensions(tinyxml2::XMLElement * element, std::vector<ExtensionData> & extensions, std::map<std::string, EnumData> & enums)
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child );
  do
  {
    assert( strcmp( child->Value(), "extension" ) == 0 );
    readExtensionsExtension( child, extensions, enums );
  } while ( child = child->NextSiblingElement() );
}

void readExtensionsExtension(tinyxml2::XMLElement * element, std::vector<ExtensionData> & extensions, std::map<std::string, EnumData> & enums )
{
  extensions.push_back( ExtensionData() );
  ExtensionData & ext = extensions.back();

  assert( element->Attribute( "name" ) );
  ext.name = element->Attribute( "name" );

  // don't parse disabled extensions
  if (strcmp(element->Attribute("supported"), "disabled") == 0)
  {
    return;
  }

  if ( element->Attribute( "protect" ) )
  {
    ext.protect = element->Attribute( "protect" );
  }

  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child && ( strcmp( child->Value(), "require" ) == 0 ) && !child->NextSiblingElement() );
  readExtensionRequire( child, ext.elements, enums);
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

void readTypeBitmask( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies, std::set<std::string> & flags, std::set<std::string> & vkTypes )
{
  tinyxml2::XMLElement * typeElement = element->FirstChildElement();
  assert( typeElement && ( strcmp( typeElement->Value(), "type" ) == 0 ) && typeElement->GetText() && ( strcmp( typeElement->GetText(), "VkFlags" ) == 0 ) );
  std::string type = typeElement->GetText();

  tinyxml2::XMLElement * nameElement = typeElement->NextSiblingElement();
  assert( nameElement && ( strcmp( nameElement->Value(), "name" ) == 0 ) && nameElement->GetText() );
  std::string name = strip( nameElement->GetText(), "Vk" );

  assert( !nameElement->NextSiblingElement() );

  if ( element->Attribute( "requires" ) )
  {
    std::string requires = strip( element->Attribute( "requires" ), "Vk" );
    dependencies.push_back( DependencyData( DependencyData::Category::FLAGS, name ) );
    dependencies.back().dependencies.insert( requires );
    flags.insert( name );

    assert( vkTypes.find( name ) == vkTypes.end() );
    vkTypes.insert( name );
  }
  else
  {
    dependencies.push_back( DependencyData( DependencyData::Category::SCALAR, name ) );
    dependencies.back().dependencies.insert( type );
  }
}

void readTypeDefine( tinyxml2::XMLElement * element, std::string & version )
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  if ( child && ( strcmp( child->GetText(), "VK_API_VERSION" ) == 0 ) )
  {
    version = element->LastChild()->ToText()->Value();
  }
}

void readTypeFuncpointer( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies )
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child && ( strcmp( child->Value(), "name" ) == 0 ) && child->GetText() );
  dependencies.push_back( DependencyData( DependencyData::Category::FUNC_POINTER, child->GetText() ) );
}

void readTypeHandle( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies )
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

void readTypes( tinyxml2::XMLElement * element, std::string & version, std::list<DependencyData> & dependencies, std::set<std::string> & flags, std::map<std::string,StructData> & structs, std::set<std::string> & vkTypes )
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
        readTypeBitmask( child, dependencies, flags, vkTypes );
      }
      else if ( category == "define" )
      {
        readTypeDefine( child, version );
      }
      else if ( category == "funcpointer" )
      {
        readTypeFuncpointer( child, dependencies );
      }
      else if ( category == "handle" )
      {
        readTypeHandle( child, dependencies );
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

void sortDependencies( std::list<DependencyData> & dependencies, std::vector<ExtensionData> const& extensions, std::vector<std::vector<DependencyData>> & sortedDependencies )
{
  std::set<std::string> listedTypes = { "VkFlags" };

  std::map<std::string,size_t> extensionMap;
  for ( size_t i=0 ; i<extensions.size() ; i++ )
  {
    for ( size_t j=0 ; j<extensions[i].elements.size() ; j++ )
    {
      assert( extensionMap.find( extensions[i].elements[j] ) == extensionMap.end() );
      extensionMap[extensions[i].elements[j]] = i + 1;
    }
  }

  sortedDependencies.resize( extensions.size() + 1 );

  while ( !dependencies.empty() )
  {
#if !defined(NDEBUG)
    bool ok = false;
#endif
    for ( std::list<DependencyData>::iterator it = dependencies.begin() ; it != dependencies.end() ; ++it )
    {
      if ( noDependencies( it->dependencies, listedTypes ) )
      {
        std::map<std::string,size_t>::iterator extMapIt = extensionMap.find( it->name );
        size_t idx = ( extMapIt != extensionMap.end() ) ? extMapIt->second : 0;
        sortedDependencies[idx].push_back( *it );
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
  assert((1 < name.length()) && (name[0] == 'p') && (isupper(name[1])));
  std::string reducedName = strip(name, "p");
  reducedName[0] = tolower(reducedName[0]);
  return reducedName;
}

std::string strip( std::string const& value, std::string const& prefix )
{
  if ( value.find( prefix ) == 0 )
  {
    return value.substr( prefix.length() );
  }
  return value;
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
      ofs << "Vk" << memberData.pureType << "*";
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
        // HACK we need to read <enum extends="VkStructureType"> here for the correct name. In this case the 'generic' rule to create the enums doesn't work
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
    ofs << "const " << memberData.type << ( memberData.arraySize.empty() ? "&" : "*" );
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
  writeTypeCommandStandard(ofs, dependencyData, commandData, vkTypes);
  writeTypeCommandEnhanced(ofs, dependencyData, commandData, vkTypes);
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

void writeTypeCommandEnhanced(std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes)
{
  if (hasPointerArguments( commandData ))
  {
    std::vector<std::pair<size_t, size_t>> lenParameters = getLenParameters(commandData);
    ofs << "#ifdef VKCPP_ENHANCED_MODE" << std::endl
        << "  inline ";
    size_t returnIndex = (commandData.returnType == "void") ? findReturnIndex(commandData, lenParameters) : ~0;
    if (returnIndex == ~0)
    {
      ofs << commandData.returnType;
    }
    else
    {
      ofs << "std::vector<" << commandData.arguments[lenParameters[returnIndex].first].pureType << ">";
      ofs << std::flush;
    }
    ofs << " " << dependencyData.name << "( ";

    std::set<size_t> argIndices;
    std::map<size_t, std::vector<size_t>> sizeIndices;
    for (size_t i = 0; i < lenParameters.size(); i++)
    {
      argIndices.insert(lenParameters[i].first);
      if (lenParameters[i].second != ~0)
      {
        sizeIndices[lenParameters[i].second].push_back( lenParameters[i].first );
      }
    }

    bool argEncounterd = false;
    std::map<std::string, std::string> nameMap;
    for (size_t i = 0; i < commandData.arguments.size(); i++)
    {
      nameMap[commandData.arguments[i].name] = commandData.arguments[i].name;

      if ((sizeIndices.find(i) != sizeIndices.end()) || ((returnIndex != ~0) && (i == lenParameters[returnIndex].first)))
      {
        // skip size arguments and the returning arguments
      }
      else
      {
        if (argEncounterd)
        {
          ofs << ", ";
        }
        if (argIndices.find(i) != argIndices.end())
        {
          // take a vector or a string here
          if (commandData.arguments[i].len == "null-terminated")
          {
            assert(commandData.arguments[i].pureType == "char");
            ofs << "std::string const& ";
          }
          else
          {
            std::string dataType = (commandData.arguments[i].pureType == "void") ? "uint8_t" : commandData.arguments[i].pureType;
            ofs << "std::vector<" << dataType << "> ";
            if (commandData.arguments[i].type.find("const") == 0)
            {
              ofs << "const";
            }
            ofs << "&";
          }
          nameMap[commandData.arguments[i].name] = reduceName(commandData.arguments[i].name);
        }
        else
        {
          assert(commandData.arguments[i].arraySize.empty());
          std::string type = commandData.arguments[i].type;
          size_t pos = type.find('*');
          if ((pos != std::string::npos) && (type.find('*', pos + 1) == std::string::npos) && (commandData.arguments[i].name[0] == 'p') && (isupper(commandData.arguments[i].name[1])))
          {
            type[pos] = '&';
            assert(type.find('*') == std::string::npos);
            nameMap[commandData.arguments[i].name] = reduceName(commandData.arguments[i].name);
          }
          ofs << type;
        }
        ofs << " " << nameMap[commandData.arguments[i].name];
        argEncounterd = true;
      }
    }
    ofs << " )" << std::endl
      << "  {" << std::endl;

    size_t complexIndex = findComplexIndex(commandData, lenParameters);
    if (complexIndex == ~0)
    {
      writeTypeCommandSimpleBody(ofs, dependencyData, commandData, nameMap, argIndices, sizeIndices);
    }
    else
    {
      writeTypeCommandComplexBody(ofs, dependencyData, commandData, nameMap, lenParameters, argIndices, complexIndex, returnIndex);
    }

    ofs << "  }" << std::endl
      << "#endif    // VKCPP_ENHANCED_MODE" << std::endl
      << std::endl;
  }
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

void writeTypeCommandComplexBody(std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::map<std::string,std::string> const& nameMap, std::vector<std::pair<size_t, size_t>> const& lenParameters, std::set<size_t> const& argIndices, size_t complexIndex, size_t returnIndex)
{
  assert(lenParameters[complexIndex].second != ~0);
  ofs << "    " << commandData.arguments[lenParameters[complexIndex].second].pureType << " " << nameMap.find(commandData.arguments[lenParameters[complexIndex].second].name)->second << " = 0;" << std::endl;
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
    if (i == lenParameters[complexIndex].first)
    {
      ofs << "nullptr";
    }
    else if (i == lenParameters[complexIndex].second)
    {
      ofs << "&" << argit->second;
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
    ofs << "    if ( result == Result::eVkSuccess )" << std::endl
      << "    {" << std::endl
      << "  ";
  }
  ofs << "    ";
  if (returnIndex == ~0)
  {
    ofs << nameMap.find(commandData.arguments[lenParameters[complexIndex].first].name)->second << ".resize( " << nameMap.find(commandData.arguments[lenParameters[complexIndex].second].name)->second << " );" << std::endl;
  }
  else
  {
    ofs << "std::vector<" << commandData.arguments[lenParameters[returnIndex].first].pureType << "> " << nameMap.find(commandData.arguments[lenParameters[returnIndex].first].name)->second << "( " << nameMap.find(commandData.arguments[lenParameters[returnIndex].second].name)->second << " );" << std::endl;
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
    if (i == lenParameters[complexIndex].first)
    {
      ofs << argit->second << ".data()";
    }
    else if (i == lenParameters[complexIndex].second)
    {
      ofs << "&" << argit->second;
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
    assert(returnIndex == ~0);
    ofs << "    }" << std::endl
      << "    return result;" << std::endl;
  }
  else if (returnIndex != ~0)
  {
    ofs << "    return std::move( " << nameMap.find(commandData.arguments[lenParameters[returnIndex].first].name)->second << " );" << std::endl;
  }
}

void writeTypeCommandSimpleBody(std::ofstream & ofs, DependencyData const& dependencyData, CommandData const& commandData, std::map<std::string,std::string> const& nameMap, std::set<size_t> const& argIndices, std::map<size_t, std::vector<size_t>> const& sizeIndices)
{
  for (std::map<size_t, std::vector<size_t>>::const_iterator it = sizeIndices.begin(); it != sizeIndices.end(); ++it)
  {
    if (1 < it->second.size())
    {
      assert(it->second.size() == 2);
      ofs << "    assert( " << nameMap.find(commandData.arguments[it->second[0]].name)->second << ".size() <= " << nameMap.find(commandData.arguments[it->second[1]].name)->second << ".size() );" << std::endl;
    }
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
        ofs << "&" << argit->second;
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
  ofs << "  };" << std::endl
      << std::endl;
}

void writeTypeFlags( std::ofstream & ofs, DependencyData const& dependencyData )
{
  assert( dependencyData.dependencies.size() == 1 );
  ofs << "  typedef Flags<" << *dependencyData.dependencies.begin() << ", Vk" << dependencyData.name << "> " << dependencyData.name << ";" << std::endl
      << std::endl
      << "  inline " << dependencyData.name << " operator|( " << *dependencyData.dependencies.begin() << " bit0, " << *dependencyData.dependencies.begin() << " bit1 )" << std::endl
      << "  {" << std::endl
      << "    return " << dependencyData.name << "( bit0 ) | bit1;" << std::endl
      << "  }" << std::endl
      << std::endl;
}

void writeTypeHandle( std::ofstream & ofs, DependencyData const& dependencyData )
{
  assert( dependencyData.dependencies.empty() );
  ofs << "  typedef Vk" << dependencyData.name << " " << dependencyData.name << ";" << std::endl;
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

  // the cast-operator to the wrapped struct, and the struct itself as a private member variable
  ofs << "    operator const Vk" << dependencyData.name << "&() const" << std::endl
      << "    {" << std::endl
      << "      return " << memberName << ";" << std::endl
      << "    }" << std::endl
      << std::endl
      << "  private:" << std::endl
      << "    Vk" << dependencyData.name << " " << memberName << ";" << std::endl
      << "  };" << std::endl
#if 0
      << "  static_assert( sizeof( " << dependencyData.name << " ) == sizeof( Vk" << dependencyData.name << " ), \"struct and wrapper have different size!\" );" << std::endl
#endif
      << std::endl;
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

void writeTypes( std::ofstream & ofs, std::vector<DependencyData> const& dependencies, std::map<std::string,CommandData> const& commands, std::map<std::string,EnumData> const& enums, std::map<std::string,StructData> const& structs, std::map<std::string,std::string> const& defaultValues, std::set<std::string> const& vkTypes )
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
        writeTypeFlags( ofs, *it );
        break;
      case DependencyData::Category::FUNC_POINTER :
      case DependencyData::Category::REQUIRED :
        // skip FUNC_POINTER and REQUIRED, they just needed to be in the dependencies list to resolve dependencies
        break;
      case DependencyData::Category::HANDLE :
        writeTypeHandle( ofs, *it );
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

void writeVersionCheck( std::ofstream & ofs, std::string const& version )
{
  ofs << "static_assert( VK_MAKE_VERSION" << version << " == VK_API_VERSION, \"Wrong VK_API_VERSION!\" );" << std::endl
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

  std::string                       version;
  std::list<DependencyData>         dependencies;
  std::map<std::string,EnumData>    enums;
  std::vector<ExtensionData>        extensions;
  std::set<std::string>             flags;
  std::map<std::string,StructData>  structs;
  std::map<std::string,CommandData> commands;
  std::set<std::string>             vkTypes;

  tinyxml2::XMLElement * child = registryElement->FirstChildElement();
  do
  {
    assert( child->Value() );
    const std::string value = child->Value();
    if ( value == "commands" )
    {
      readCommands( child, dependencies, commands );
    }
    else if ( value == "enums" )
    {
      readEnums( child, dependencies, enums, vkTypes );
    }
    else if ( value == "extensions" )
    {
      readExtensions( child, extensions, enums );
    }
    else if ( value == "types" )
    {
      readTypes( child, version, dependencies, flags, structs, vkTypes );
    }
    else
    {
      assert( ( value == "comment" ) || ( value == "feature" ) || ( value == "tags" ) || ( value == "vendorids" ) );
    }
  } while ( child = child->NextSiblingElement() );

  std::vector<std::vector<DependencyData>> sortedDependencies;
  sortDependencies( dependencies, extensions, sortedDependencies );

  std::map<std::string,std::string> defaultValues;
  for ( size_t i=0 ; i<sortedDependencies.size() ; i++ )
  {
    createDefaults( sortedDependencies[i], enums, defaultValues );
  }

  std::ofstream ofs( "vk_cpp.h" );
  ofs << licenseHeader << std::endl;

  ofs << std::endl << "#pragma once" << std::endl;

  ofs << "#include <array>" << std::endl
      << "#include <cassert>" << std::endl
      << "#include <cstdint>" << std::endl
      << "#include <cstring>" << std::endl
      << "#include <vulkan/vulkan.h>" << std::endl
      << "#ifdef VKCPP_ENHANCED_MODE" << std::endl
      << "# include <vector>" << std::endl
      << "#endif    // VKCPP_ENHANCED_MODE" << std::endl
      << std::endl;
  writeVersionCheck( ofs, version );
  ofs << "namespace vk" << std::endl
      << "{" << std::endl;

  ofs << flagsHeader;

  for ( size_t i=0 ; i<sortedDependencies.size() ; i++ )
  {
    if ( ( 0 < i ) && !extensions[i-1].protect.empty() )
    {
      ofs << "#ifdef " << extensions[i-1].protect << std::endl
          << std::endl;
    }
    writeTypes( ofs, sortedDependencies[i], commands, enums, structs, defaultValues, vkTypes );
    if ( ( 0 < i ) && !extensions[i-1].protect.empty() )
    {
      ofs << "#endif /* " << extensions[i-1].protect << " */" << std::endl
          << std::endl;
    }
  }

  ofs << "}" << std::endl;
}
