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

#pragma once

#include <map>
#include <iostream>
#include <set>
#include <vector>
#include <tinyxml2.h>

class VulkanHppGenerator
{
  public:
    VulkanHppGenerator(tinyxml2::XMLDocument const& document);

    void appendBaseTypes(std::string & str) const;
    void appendBitmasks(std::string & str) const;
    void appendDispatchLoaderDynamic(std::string & str); // use vkGet*ProcAddress to get function pointers
    void appendDispatchLoaderStatic(std::string & str);  // use exported symbols from loader
    void appendDispatchLoaderDefault(std::string & str);  // typedef to DispatchLoaderStatic or undefined type, based on VK_NO_PROTOTYPES
    void appendEnums(std::string & str) const;
    void appendForwardDeclarations(std::string & str) const;
    void appendHandles(std::string & str) const;
    void appendHandlesCommandDefintions(std::string & str) const;
    void appendResultExceptions(std::string & str) const;
    void appendStructs(std::string & str) const;
    void appendStructureChainValidation(std::string & str);
    void appendThrowExceptions(std::string & str) const;
    std::string const& getTypesafeCheck() const;
    std::string const& getVersion() const;
    std::string const& getVulkanLicenseHeader() const;

  private:
    struct BaseTypeData
    {
      BaseTypeData(std::string const& type_, int line)
        : type(type_)
        , xmlLine(line)
      {}

      std::string type;
      int         xmlLine;
    };

    struct BitmaskData
    {
      BitmaskData(std::string const& r, std::string const& t, int line)
        : requirements(r)
        , type(t)
        , xmlLine(line)
      {}

      std::string requirements;
      std::string type;
      std::string platform;
      std::string alias;
      int         xmlLine;
    };

    struct NameData
    {
      std::string               name;
      std::vector<std::string>  arraySizes;
      std::string               bitCount;
    };

    struct TypeData
    {
      std::string compose() const;

      bool operator==(TypeData const& rhs) const
      {
        return (prefix == rhs.prefix) && (type == rhs.type) && (postfix == rhs.postfix);
      }

      std::string prefix;
      std::string type;
      std::string postfix;
    };

    struct ParamData
    {
      ParamData()
        : optional(false)
      {}

      TypeData                  type;
      std::string               name;
      std::vector<std::string>  arraySizes;
      std::string               len;
      bool                      optional;
    };

    struct CommandData
    {
      CommandData(int line)
        : xmlLine(line)
      {}

      std::vector<ParamData>    params;
      std::string               platform;
      std::string               returnType;
      std::vector<std::string>  successCodes;
      std::vector<std::string>  errorCodes;
      std::set<std::string>     aliases;
      int                       xmlLine;
    };

    struct EnumValueData
    {
      EnumValueData(std::string const& vulkan, std::string const& vk, bool singleBit_)
        : vulkanValue(vulkan)
        , vkValue(vk)
        , singleBit(singleBit_)
      {}

      std::string vulkanValue;
      std::string vkValue;
      bool        singleBit;
    };

    struct EnumData
    {
      void addEnumValue(int line, std::string const& valueName, bool bitmask, bool bitpos, std::string const& prefix, std::string const& postfix, std::string const& tag);

      std::string                                       alias;    // alias for this enum
      std::vector<std::pair<std::string, std::string>>  aliases;  // pairs of vulkan enum value and corresponding vk::-namespace enum value
      bool                                              isBitmask = false;
      std::string                                       platform;
      std::vector<EnumValueData>                        values;
    };

    struct ExtensionData
    {
      ExtensionData(int line)
        : xmlLine(line)
      {}

      std::string                 deprecatedBy;
      std::string                 obsoletedBy;
      std::string                 promotedTo;
      std::map<std::string, int>  requirements;
      int                         xmlLine;
    };

    struct FuncPointerData
    {
      FuncPointerData(std::string const& r, int line)
        : requirements(r)
        , xmlLine(line)
      {}

      std::string requirements;
      int         xmlLine;
    };

    struct HandleData
    {
      HandleData(std::vector<std::string> const& p, int line)
        : parents(p)
        , xmlLine(line)
      {}

      std::string                         alias;
      std::set<std::string>               childrenHandles;
      std::map<std::string, CommandData>  commands;
      std::string                         deleteCommand;
      std::string                         deletePool;
      std::string                         platform;
      std::vector<std::string>            parents;
      int                                 xmlLine;
    };

    struct MemberData
    {
      MemberData(int line)
        : xmlLine(line)
      {}

      TypeData                  type;
      std::string               name;
      std::vector<std::string>  arraySizes;
      std::string               bitCount;
      std::string               values;
      std::string               usedConstant;
      int                       xmlLine;
    };

    struct StructureData
    {
      StructureData(std::vector<std::string> const& extends, int line)
        : returnedOnly(false)
        , isUnion(false)
        , structExtends(extends)
        , xmlLine(line)
      {}

      bool                      returnedOnly;
      bool                      isUnion;
      std::vector<MemberData>   members;
      std::string               platform;
      std::vector<std::string>  structExtends;
      std::set<std::string>     aliases;
      std::string               subStruct;
      int                       xmlLine;
    };

  private:
    void appendArgumentPlainType(std::string & str, ParamData const& paramData) const;
    void appendArguments(std::string & str, CommandData const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool firstCall, bool singular, size_t from, size_t to) const;
    void appendArgumentVector(std::string & str, size_t paramIndex, ParamData const& paramData, size_t returnParamIndex, size_t templateParamIndex, bool twoStep, bool firstCall, bool singular) const;
    void appendArgumentVulkanType(std::string & str, ParamData const& paramData) const;
    void appendBitmask(std::string & os, std::string const& bitmaskName, std::string const& bitmaskType, std::string const& bitmaskAlias, std::string const& enumName, std::vector<EnumValueData> const& enumValues) const;
    void appendBitmaskToStringFunction(std::string & str, std::string const& flagsName, std::string const& enumName, std::vector<EnumValueData> const& enumValues) const;
    void appendCall(std::string &str, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool firstCall, bool singular) const;
    void appendCommand(std::string & str, std::string const& indentation, std::string const& name, std::pair<std::string, CommandData> const& commandData, bool definition) const;
    void appendEnum(std::string & str, std::pair<std::string, EnumData> const& enumData) const;
    void appendEnumToString(std::string & str, std::pair<std::string, EnumData> const& enumData) const;
    void appendFunction(std::string & str, std::string const& indentation, std::string const& name, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, std::string const& enhancedReturnType, bool definition, bool enhanced, bool singular, bool unique, bool isStructureChain, bool withAllocator) const;
    void appendFunctionBodyEnhanced(std::string & str, std::string const& indentation, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, std::string const& enhancedReturnType, bool singular, bool unique, bool isStructureChain, bool withAllocator) const;
    std::string appendFunctionBodyEnhancedLocalReturnVariable(std::string & str, std::string const& indentation, CommandData const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, std::string const& enhancedReturnType, bool singular, bool isStructureChain, bool withAllocator) const;
    void appendFunctionBodyEnhancedLocalReturnVariableVectorSize(std::string & str, std::vector<ParamData> const& params, std::pair<size_t, size_t> const& vectorParamIndex, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool withAllocator) const;
    void appendFunctionBodyEnhancedMultiVectorSizeCheck(std::string & str, std::string const& indentation, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices) const;
    void appendFunctionBodyEnhancedReturnResultValue(std::string & str, std::string const& indentation, std::string const& returnName, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, bool twoStep, bool singular, bool unique) const;
    void appendFunctionBodyEnhancedSingleStep(std::string & str, std::string const& indentation, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool singular) const;
    void appendFunctionBodyEnhancedTwoStep(std::string & str, std::string const& indentation, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool singular, std::string const& returnName) const;
    void appendFunctionBodyEnhancedVectorOfStructureChain(std::string & str, std::string const& indentation, std::pair<std::string,CommandData> const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool withAllocator) const;
    void appendFunctionBodyEnhancedVectorOfUniqueHandles(std::string & str, std::string const& indentation, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool singular, bool withAllocator) const;
    void appendFunctionBodyStandard(std::string & str, std::string const& indentation, std::pair<std::string, CommandData> const& commandData) const;
    void appendFunctionBodyStandardArgument(std::string & str, TypeData const& typeData, std::string const& name) const;
    bool appendFunctionHeaderArgumentEnhanced(std::string & str, ParamData const& param, size_t paramIndex, std::map<size_t, size_t> const& vectorParamIndices, bool skip, bool argEncountered, bool isTemplateParam, bool isLastArgument, bool singular, bool withDefaults, bool withAllocator) const;
    void appendFunctionHeaderArgumentEnhancedPointer(std::string & str, ParamData const& param, std::string const& strippedParameterName, bool withDefaults, bool withAllocator) const;
    void appendFunctionHeaderArgumentEnhancedSimple(std::string & str, ParamData const& param, bool lastArgument, bool withDefaults, bool withAllocator) const;
    void appendFunctionHeaderArgumentEnhancedVector(std::string & str, ParamData const& param, std::string const& strippedParameterName, bool hasSizeParam, bool isTemplateParam, bool singular, bool withDefaults, bool withAllocator) const;
    void appendFunctionHeaderArguments(std::string & str, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool enhanced, bool singular, bool withDefaults, bool withAllocator) const;
    void appendFunctionHeaderArgumentsEnhanced(std::string & str, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool singular, bool withDefaults, bool withAllocator) const;
    void appendFunctionHeaderArgumentsStandard(std::string & str, std::pair<std::string, CommandData> const& commandData, bool withDefaults) const;
    bool appendFunctionHeaderArgumentStandard(std::string & str, ParamData const& param, bool argEncountered, bool isLastArgument, bool withDefaults) const;
    void appendFunctionHeaderReturnType(std::string & str, CommandData const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, std::string const& enhancedReturnType, bool enhanced, bool twoStep, bool singular, bool unique, bool isStructureChain) const;
    void appendFunctionHeaderTemplate(std::string & str, std::string const& indentation, size_t returnParamIndex, size_t templateParamIndex, std::string const& enhancedReturnType, bool enhanced, bool singular, bool unique, bool withDefault, bool isStructureChain) const;
    void appendHandle(std::string & str, std::pair<std::string, HandleData> const& handle, std::set<std::string> & listedHandles) const;
    void appendPlatformEnter(std::string & str, bool isAliased, std::string const& platform) const;
    void appendPlatformLeave(std::string & str, bool isAliased, std::string const& platform) const;
    void appendStruct(std::string & str, std::pair<std::string, StructureData> const& structure, std::set<std::string> & listedStructures) const;
    void appendStructAssignmentOperator(std::string &str, std::pair<std::string, StructureData> const& structure, std::string const& prefix) const;
    void appendStructCompareOperators(std::string & str, std::pair<std::string, StructureData> const& structure) const;
    void appendStructConstructor(std::string &str, std::pair<std::string, StructureData> const& structData, std::string const& prefix) const;
    bool appendStructConstructorArgument(std::string & str, bool listedArgument, std::string const& indentation, MemberData const& memberData) const;
    void appendStructCopyConstructor(std::string &str, std::pair<std::string, StructureData> const& structData, std::string const& prefix) const;
    void appendStructCopyConstructors(std::string & str, std::string const& vkName) const;
    void appendStructMembers(std::string & str, std::pair<std::string,StructureData> const& structData, std::string const& prefix) const;
    void appendStructSetter(std::string & str, std::string const& structureName, MemberData const& memberData) const;
    void appendStructSubConstructor(std::string &str, std::pair<std::string, StructureData> const& structData, std::string const& prefix) const;
    void appendStructure(std::string & str, std::pair<std::string, StructureData> const& structure) const;
    void appendUnion(std::string & str, std::pair<std::string, StructureData> const& structure) const;
    void appendUniqueTypes(std::string &str, std::string const& parentType, std::set<std::string> const& childrenTypes) const;
    std::string constructConstexprString(std::pair<std::string, StructureData> const& structData) const;
    void checkCorrectness();
    bool checkLenAttribute(std::string const& len, std::vector<ParamData> const& params);
    bool containsArray(std::string const& type) const;
    bool containsUnion(std::string const& type) const;
    std::string determineEnhancedReturnType(CommandData const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool isStructureChain) const;
    size_t determineReturnParamIndex(CommandData const& commandData, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep) const;
    std::string determineSubStruct(std::pair<std::string, StructureData> const& structure) const;
    size_t determineTemplateParamIndex(std::vector<ParamData> const& params, std::map<size_t, size_t> const& vectorParamIndices) const;
    std::map<size_t, size_t> determineVectorParamIndices(std::vector<ParamData> const& params) const;
    bool isTwoStepAlgorithm(std::vector<ParamData> const& params) const;
    void linkCommandToHandle(int line, std::string const& name, CommandData const& commandData);
    void readBaseType(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readBitmask(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readBitmaskAlias(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readCommand(tinyxml2::XMLElement const* element);
    void readCommand(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributess);
    void readCommandAlias(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    ParamData readCommandParam(tinyxml2::XMLElement const* element, std::vector<ParamData> const& params);
    std::pair<std::string, std::string> readCommandProto(tinyxml2::XMLElement const* element);
    void readCommands(tinyxml2::XMLElement const* element);
    std::string readComment(tinyxml2::XMLElement const* element);
    void readDefine(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readEnum(tinyxml2::XMLElement const* element, EnumData & enumData, bool bitmask, std::string const& prefix, std::string const& postfix);
    void readEnum(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes, EnumData & enumData, bool bitmask, std::string const& prefix, std::string const& postfix);
    void readEnumAlias(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes, EnumData & enumData, bool bitmask, std::string const& prefix, std::string const& postfix);
    void readEnumConstant(tinyxml2::XMLElement const* element);
    void readEnums(tinyxml2::XMLElement const* element);
    void readExtension(tinyxml2::XMLElement const* element);
    void readExtensionDisabledCommand(tinyxml2::XMLElement const* element);
    void readExtensionDisabledEnum(std::string const& extensionName, tinyxml2::XMLElement const* element);
    void readExtensionDisabledRequire(std::string const& extensionName, tinyxml2::XMLElement const* element);
    void readExtensionDisabledType(tinyxml2::XMLElement const* element);
    void readExtensionRequire(tinyxml2::XMLElement const* element, std::string const& platform, std::string const& tag, std::map<std::string, int> & requirements);
    void readExtensionRequireCommand(tinyxml2::XMLElement const* element, std::string const& platform);
    void readExtensionRequireType(tinyxml2::XMLElement const* element, std::string const& platform);
    void readExtensions(tinyxml2::XMLElement const* element);
    void readFeature(tinyxml2::XMLElement const* element);
    void readFeatureRequire(tinyxml2::XMLElement const* element);
    void readFuncpointer(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readHandle(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    std::pair<NameData, TypeData> readNameAndType(tinyxml2::XMLElement const* elements);
    void readPlatform(tinyxml2::XMLElement const* element);
    void readPlatforms(tinyxml2::XMLElement const* element);
    void readRegistry(tinyxml2::XMLElement const* element);
    void readRequireCommand(tinyxml2::XMLElement const* element);
    void readRequireEnum(tinyxml2::XMLElement const* element, std::string const& tag);
    void readRequireEnum(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes, std::string const& tag);
    void readRequireEnumAlias(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes, std::string const& tag);
    void readRequires(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readRequireType(tinyxml2::XMLElement const* element);
    void readStruct(tinyxml2::XMLElement const* element, bool isUnion, std::map<std::string, std::string> const& attributes);
    void readStructAlias(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readStructMember(tinyxml2::XMLElement const* element, std::vector<MemberData> & members);
    void readStructMemberEnum(tinyxml2::XMLElement const* element, MemberData & memberData);
    void readStructMemberName(tinyxml2::XMLElement const* element, MemberData & memberData, std::vector<MemberData> const& members);
    void readStructMemberType(tinyxml2::XMLElement const* element, MemberData & memberData);
    void readTag(tinyxml2::XMLElement const* element);
    void readTags(tinyxml2::XMLElement const* element);
    void readType(tinyxml2::XMLElement const* element);
    void readTypeEnum(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readTypeInclude(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readTypes(tinyxml2::XMLElement const* element);
    void registerDeleter(std::string const& name, std::pair<std::string, CommandData> const& commandData);
    void setVulkanLicenseHeader(int line, std::string const& comment);

  private:
    std::map<std::string, BaseTypeData>     m_baseTypes;
    std::map<std::string, BitmaskData>      m_bitmasks;
    std::map<std::string, std::string>      m_commandToHandle;
    std::set<std::string>                   m_constants;
    std::map<std::string, EnumData>         m_enums;
    std::set<std::string>                   m_extendedStructs; // structs which are referenced by the structextends tag
    std::map<std::string, ExtensionData>    m_extensions;
    std::map<std::string, std::string>      m_features;
    std::map<std::string, FuncPointerData>  m_funcPointers;
    std::map<std::string, HandleData>       m_handles;
    std::set<std::string>                   m_includes;
    std::map<std::string, std::string>      m_platforms;
    std::map<std::string, std::string>      m_structureAliases;
    std::map<std::string, StructureData>    m_structures;
    std::set<std::string>                   m_tags;
    std::set<std::string>                   m_types;
    std::string                             m_typesafeCheck;
    std::string                             m_version;
    std::string                             m_vulkanLicenseHeader;
};

const size_t INVALID_INDEX = (size_t)~0;
