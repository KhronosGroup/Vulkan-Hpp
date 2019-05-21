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
    VulkanHppGenerator();

    void checkCorrectness();
    std::string const& getTypesafeCheck() const;
    std::string const& getVersion() const;
    std::string const& getVulkanLicenseHeader() const;
    void readCommands(tinyxml2::XMLElement const* element);
    void readComment(tinyxml2::XMLElement const* element);
    void readEnums(tinyxml2::XMLElement const* element);
    void readExtensions(tinyxml2::XMLElement const* element);
    void readFeature(tinyxml2::XMLElement const* element);
    void readPlatforms(tinyxml2::XMLElement const* element);
    void readTags(tinyxml2::XMLElement const* element);
    void readTypes(tinyxml2::XMLElement const* element);
    void writeBaseTypes(std::ostream & os) const;
    void writeBitmasks(std::ostream & os) const;
    void writeDispatchLoaderDynamic(std::ostream &os); // use vkGet*ProcAddress to get function pointers
    void writeDispatchLoaderStatic(std::ostream &os);  // use exported symbols from loader
    void writeDispatchLoaderDefault(std::ostream &os);  // typedef to DispatchLoaderStatic or undefined type, based on VK_NO_PROTOTYPES
    void writeEnums(std::ostream & os) const;
    void writeForwardDeclarations(std::ostream & os) const;
    void writeHandles(std::ostream & os) const;
    void writeHandlesCommandDefintions(std::ostream & os) const;
    void writeResultExceptions(std::ostream & os) const;
    void writeStructs(std::ostream & os) const;
    void writeStructureChainValidation(std::ostream & os);
    void writeThrowExceptions(std::ostream& os) const;

  private:
    struct BitmaskData
    {
      BitmaskData(std::string const& r)
        : requires(r)
      {}

      std::string requires;   // original vulkan name: VK*FlagBits
      std::string platform;
      std::string alias;      // original vulkan name
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

      TypeData    type;
      std::string name;
      std::string arraySize;
      std::string len;
      bool        optional;
    };

    struct CommandData
    {
      CommandData()
        : isAlias(false)
      {}

      std::vector<ParamData>    params;
      std::string               platform;
      std::string               returnType;
      std::vector<std::string>  successCodes;
      bool                      isAlias;
    };

    struct EnumData
    {
      void addEnumValue(std::string const& valueName, bool bitmask, std::string const& prefix, std::string const& postfix, std::string const& tag);

      std::vector<std::pair<std::string, std::string>>  aliases;  // pairs of vulkan enum value and corresponding vk::-namespace enum value
      std::string                                       platform;
      std::vector<std::pair<std::string, std::string>>  values;   // pairs of vulkan enum value and corresponding vk::-namespace enum value
    };

    struct HandleData
    {
      std::string                         alias;
      std::map<std::string, CommandData>  commands;
      std::string                         deleteCommand;
      std::string                         deletePool;
      std::set<std::string>               childrenHandles;
    };

    struct MemberData
    {
      TypeData    type;
      std::string name;
      std::string arraySize;
      std::string values;
    };

    struct StructureData
    {
      StructureData()
        : returnedOnly(false)
      {}

      bool                      returnedOnly;
      bool                      isUnion;
      std::vector<MemberData>   members;
      std::string               platform;
      std::vector<std::string>  structExtends;
      std::vector<std::string>  aliases;
      std::string               subStruct;
    };

  private:
    bool containsUnion(std::string const& type) const;
    std::string defaultValue(std::string const& type) const;
    std::string determineEnhancedReturnType(CommandData const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool isStructureChain) const;
    size_t determineReturnParamIndex(CommandData const& commandData, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep) const;
    std::string determineSubStruct(std::pair<std::string, StructureData> const& structure) const;
    size_t determineTemplateParamIndex(std::vector<ParamData> const& params, std::map<size_t, size_t> const& vectorParamIndices) const;
    std::map<size_t, size_t> determineVectorParamIndices(std::vector<ParamData> const& params) const;
    std::string generateCall(std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool firstCall, bool singular) const;
    bool isTwoStepAlgorithm(std::vector<ParamData> const& params) const;
    void linkCommandToHandle(std::string const& name, CommandData const& commandData);
    void readBaseType(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readBitmask(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readBitmaskAlias(int lineNum, std::string const& alias, std::map<std::string, std::string> const& attributes, std::vector<tinyxml2::XMLElement const*> const& children);
    void readCommand(tinyxml2::XMLElement const* element);
    void readCommandAlias(int lineNum, std::string const& alias, std::map<std::string, std::string> const& attributes, std::vector<tinyxml2::XMLElement const*> const& children);
    ParamData readCommandParam(tinyxml2::XMLElement const* element);
    std::string readCommandProto(tinyxml2::XMLElement const* element, std::string & returnType);
    std::vector<std::string> readCommandSuccessCodes(std::map<std::string, std::string> const& attributes);
    void readDefine(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readEnum(tinyxml2::XMLElement const* element, EnumData & enumData, bool bitmask, std::string const& prefix, std::string const& postfix);
    void readExtension(tinyxml2::XMLElement const* element);
    void readExtensionDisabled(std::vector<tinyxml2::XMLElement const*> const& children);
    void readExtensionDisabledRequire(tinyxml2::XMLElement const* element);
    void readExtensionRequire(tinyxml2::XMLElement const* element, std::string const& platform, std::string const& tag);
    void readExtensionRequireCommand(tinyxml2::XMLElement const* element, std::string const& platform);
    void readExtensionRequireType(tinyxml2::XMLElement const* element, std::string const& platform);
    void readFeatureRequire(tinyxml2::XMLElement const* element);
    void readFuncpointer(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readHandle(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readPlatform(tinyxml2::XMLElement const* element);
    void readRequireEnum(tinyxml2::XMLElement const* element, std::string const& tag);
    void readStruct(tinyxml2::XMLElement const* element, bool isUnion, std::map<std::string, std::string> const& attributes);
    void readStructAlias(int lineNum, std::string const& name, std::string const& alias, std::map<std::string, std::string> const& attributes);
    MemberData readStructMember(tinyxml2::XMLElement const* element);
    std::vector<MemberData> readStructMembers(std::vector<tinyxml2::XMLElement const*> const& children);
    void readTag(tinyxml2::XMLElement const* element);
    void readType(tinyxml2::XMLElement const* element);
    void registerDeleter(std::string const& name, std::pair<std::string, CommandData> const& commandData);
    void unlinkCommandFromHandle(std::string const& name);
    void writeArgumentPlainType(std::ostream & os, ParamData const& paramData) const;
    void writeArguments(std::ostream & os, CommandData const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool firstCall, bool singular, size_t from, size_t to) const;
    void writeArgumentVector(std::ostream & os, size_t paramIndex, ParamData const& paramData, size_t returnParamIndex, size_t templateParamIndex, bool twoStep, bool firstCall, bool singular) const;
    void writeArgumentVulkanType(std::ostream & os, ParamData const& paramData) const;
    void writeCommand(std::ostream &os, std::string const& indentation, std::string const& name, std::pair<std::string, CommandData> const& commandData, bool definition) const;
    void writeEnum(std::ostream & os, std::pair<std::string,EnumData> const& enumData) const;
    void writeEnumToString(std::ostream & os, std::pair<std::string, EnumData> const& enumData) const;
    void writeFunction(std::ostream & os, std::string const& indentation, std::string const& name, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, std::string const& enhancedReturnType, bool definition, bool enhanced, bool singular, bool unique, bool isStructureChain, bool withAllocator) const;
    void writeFunctionBodyEnhanced(std::ostream & os, std::string const& indentation, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, std::string const& enhancedReturnType, bool singular, bool unique, bool isStructureChain, bool withAllocator) const;
    std::string writeFunctionBodyEnhancedLocalReturnVariable(std::ostream & os, std::string const& indentation, CommandData const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, std::string const& enhancedReturnType, bool singular, bool isStructureChain, bool withAllocator) const;
    void writeFunctionBodyEnhancedLocalReturnVariableVectorSize(std::ostream & os, std::vector<ParamData> const& params, std::pair<size_t, size_t> const& vectorParamIndex, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool withAllocator) const;
    void writeFunctionBodyEnhancedMultiVectorSizeCheck(std::ostream & os, std::string const& indentation, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices) const;
    void writeFunctionBodyEnhancedReturnResultValue(std::ostream & os, std::string const& indentation, std::string const& returnName, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, bool twoStep, bool singular, bool unique) const;
    void writeFunctionBodyEnhancedSingleStep(std::ostream & os, std::string const& indentation, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool singular) const;
    void writeFunctionBodyEnhancedTwoStep(std::ostream & os, std::string const& indentation, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool singular, std::string const& returnName) const;
    void writeFunctionBodyEnhancedVectorOfUniqueHandles(std::ostream & os, std::string const& indentation, std::string const& commandName, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool twoStep, bool singular, bool withAllocator) const;
    void writeFunctionBodyStandard(std::ostream & os, std::string const& indentation, std::pair<std::string, CommandData> const& commandData) const;
    std::string writeFunctionBodyStandardArgument(TypeData const& typeData, std::string const& name) const;
    bool writeFunctionHeaderArgumentEnhanced(std::ostream & os, ParamData const& param, size_t paramIndex, std::map<size_t, size_t> const& vectorParamIndices, bool skip, bool argEncountered, bool isTemplateParam, bool isLastArgument, bool singular, bool withDefaults, bool withAllocator) const;
    void writeFunctionHeaderArgumentEnhancedPointer(std::ostream & os, ParamData const& param, std::string const& strippedParameterName, bool withDefaults, bool withAllocator) const;
    void writeFunctionHeaderArgumentEnhancedSimple(std::ostream & os, ParamData const& param, bool lastArgument, bool withDefaults, bool withAllocator) const;
    void writeFunctionHeaderArgumentEnhancedVector(std::ostream & os, ParamData const& param, std::string const& strippedParameterName, bool hasSizeParam, bool isTemplateParam, bool singular, bool withDefaults, bool withAllocator) const;
    void writeFunctionHeaderArguments(std::ostream & os, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool enhanced, bool singular, bool withDefaults, bool withAllocator) const;
    void writeFunctionHeaderArgumentsEnhanced(std::ostream & os, std::pair<std::string, CommandData> const& commandData, size_t returnParamIndex, size_t templateParamIndex, std::map<size_t, size_t> const& vectorParamIndices, bool singular, bool withDefaults, bool withAllocator) const;
    void writeFunctionHeaderArgumentsStandard(std::ostream & os, std::pair<std::string, CommandData> const& commandData, bool withDefaults) const;
    bool writeFunctionHeaderArgumentStandard(std::ostream & os, ParamData const& param, bool argEncountered, bool isLastArgument, bool withDefaults) const;
    void writeFunctionHeaderReturnType(std::ostream & os, CommandData const& commandData, size_t returnParamIndex, std::map<size_t, size_t> const& vectorParamIndices, std::string const& enhancedReturnType, bool enhanced, bool singular, bool unique, bool isStructureChain) const;
    void writeFunctionHeaderTemplate(std::ostream & os, std::string const& indentation, size_t returnParamIndex, size_t templateParamIndex, std::string const& enhancedReturnType, bool enhanced, bool singular, bool unique, bool withDefault, bool isStructureChain) const;
    void writeHandle(std::ostream & os, std::pair<std::string, HandleData> const& handle, std::set<std::string> & listedHandles) const;
    void writePlatformEnter(std::ostream & os, std::string const& platform) const;
    void writePlatformLeave(std::ostream & os, std::string const& platform) const;
    void writeStruct(std::ostream & os, std::pair<std::string, StructureData> const& structure, std::set<std::string> & listedStructures) const;
    void writeStructCompareOperators(std::ostream & os, std::pair<std::string, StructureData> const& structure) const;
    void writeStructConstructor(std::ostream & os, std::pair<std::string, StructureData> const& structData) const;
    std::string writeStructConstructorArgument(bool & listedArgument, std::string const& indentation, MemberData const& memberData) const;
    void writeStructMembers(std::ostream & os, StructureData const& structData) const;
    void writeStructSetter(std::ostream & os, std::string const& structureName, MemberData const& memberData) const;
    void writeStructure(std::ostream & os, std::pair<std::string, StructureData> const& structure) const;
    void writeUnion(std::ostream & os, std::pair<std::string, StructureData> const& structure) const;
    void writeUniqueTypes(std::ostream &os, std::string const& parentType, std::set<std::string> const& childrenTypes) const;
#if !defined(NDEBUG)
    void readRequires(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
#endif

  private:
    std::map<std::string, std::string>    m_baseTypes;
    std::map<std::string, EnumData>       m_bitmaskBits;
    std::map<std::string, BitmaskData>    m_bitmasks;
    std::map<std::string, std::string>    m_commandToHandle;
    std::map<std::string, EnumData>       m_enums;
    std::set<std::string>                 m_extendedStructs; // structs which are referenced by the structextends tag
    std::map<std::string, HandleData>     m_handles;
    std::map<std::string, std::string>    m_platforms;
    std::map<std::string, std::string>    m_structureAliases;
    std::map<std::string, StructureData>  m_structures;
    std::set<std::string>                 m_tags;
    std::string                           m_typesafeCheck;
    std::string                           m_version;
    std::string                           m_vulkanLicenseHeader;
#if !defined(NDEBUG)
    std::set<std::string>                 m_defaultZeroTypes;
    std::set<std::string>                 m_defines;        // just used for verfication in readExtensionType
#endif
};

const size_t INVALID_INDEX = (size_t)~0;
