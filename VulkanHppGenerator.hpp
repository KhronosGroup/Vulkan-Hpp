// Copyright(c) 2015-2016, NVIDIA CORPORATION. All rights reserved.
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

#include <list>
#include <map>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <tinyxml2.h>

class VulkanHppGenerator
{
  public:
    VulkanHppGenerator()
    {
      m_handles.insert(std::make_pair("", HandleData()));   // insert the default "handle" without class (for createInstance, and such)
    }

    std::map<std::string, std::string> createDefaults();
    std::string const& getTypesafeCheck() const;
    std::string const& getVersion() const;
    std::string const& getVulkanLicenseHeader() const;
    void readCommands(tinyxml2::XMLElement const* element);
    void readComment(tinyxml2::XMLElement const* element);
    void readEnums(tinyxml2::XMLElement const* element);
    void readExtensions(tinyxml2::XMLElement const* element);
    void readFeature(tinyxml2::XMLElement const* element);
    void readTags(tinyxml2::XMLElement const* element);
    void readTypes(tinyxml2::XMLElement const* element);
    void sortDependencies();
    void writeResultEnum(std::ostream & os);
    void writeStructureChainValidation(std::ostream & os);
    void writeToStringFunctions(std::ostream & os);
    void writeTypes(std::ostream & os, std::map<std::string, std::string> const& defaultValues);
    void writeDelegationClassStatic(std::ostream &os);  // use exported symbols from loader
    void writeDelegationClassDynamic(std::ostream &os); // use vkGet*ProcAddress to get function pointers
#if !defined(NDEBUG)
    void checkExtensionRequirements();
    void skipVendorIDs(tinyxml2::XMLElement const* element);
#endif

  private:
    struct BitmaskData
    {
      std::string protect;
      std::string alias;
    };

    struct DependencyData
    {
      enum class Category
      {
        BITMASK,
        COMMAND,
        ENUM,
        FUNC_POINTER,
        HANDLE,
        REQUIRED,
        SCALAR,
        STRUCT,
        UNION
      };

      DependencyData(Category c, std::string const& n)
        : category(c)
        , name(n)
      {}

      Category              category;
      std::string           name;
      std::set<std::string> dependencies;
      std::set<std::string> forwardDependencies;
    };

    struct ParamData
    {
      std::string type;
      std::string unchangedType;
      std::string name;
      std::string arraySize;
      std::string pureType;
      std::string len;
      bool        optional;
    };

    struct CommandData
    {
      CommandData()
        : returnParam(~0)
        , templateParam(~0)
        , twoStep(false)
        , isAlias(false)
      {}

      std::string               className;
      std::string               enhancedReturnType;
      std::string               fullName;
      std::vector<ParamData>    params;
      std::string               protect;
      std::string               reducedName;
      size_t                    returnParam;
      std::string               returnType;
      std::string               unchangedReturnType;
      std::set<size_t>          skippedParams;
      std::vector<std::string>  successCodes;
      size_t                    templateParam;
      std::map<size_t, size_t>  vectorParams;
      bool                      twoStep;
      bool                      isAlias;
    };

    struct DeleterData
    {
      std::string pool;
      std::string call;
    };

    struct EnumValueData
    {
      std::string name;
      std::string value;
      std::string alias;
    };

    struct EnumData
    {
      EnumData(std::string const& n, bool b = false)
        : name(n)
        , bitmask(b)
      {}

      void addEnumValue(std::string const& name, std::string const& tag, std::map<std::string, std::string> & nameMap);

      std::string                 name;
      std::string                 prefix;
      std::string                 postfix;
      std::vector<EnumValueData>  values;
      std::string                 protect;
      bool                        bitmask;
    };

    struct HandleData
    {
      std::vector<std::string>  commands;
      std::string               protect;
      std::string               alias;
    };

    struct MemberData
    {
      std::string type;
      std::string name;
      std::string arraySize;
      std::string pureType;
      std::string values;
    };

    struct ScalarData
    {
      std::string protect;
    };

    struct StructData
    {
      StructData()
        : returnedOnly(false)
      {}

      bool                      returnedOnly;
      bool                      isUnion;
      std::vector<MemberData>   members;
      std::string               protect;
      std::vector<std::string>  structExtends;
      std::string               alias;
      std::string               subStruct;
    };

#if !defined(NDEBUG)
    struct ExtensionData
    {
      std::string               protect;
      std::vector<std::string>  requires;
    };

    struct VendorIDData
    {
      std::string name;
      std::string id;
      std::string comment;
    };
#endif

  private:
    template <typename T> void checkAlias(std::map<std::string, T> const& data, std::string const& name, int line);
    bool containsUnion(std::string const& type, std::map<std::string, StructData> const& structs);
    void determineEnhancedReturnType(CommandData & commandData);
    void determineReducedName(CommandData & commandData);
    void determineReturnParam(CommandData & commandData);
    void determineSkippedParams(CommandData & commandData);
    void determineTemplateParam(CommandData & commandData);
    void determineVectorParams(CommandData & commandData);
    std::string generateCall(CommandData const& commandData, bool firstCall, bool singular);
    bool isSubStruct(std::pair<std::string, StructData> const& nsd, std::string const& name, StructData const& structData);
    void linkCommandToHandle(CommandData & commandData);
    bool readCommandParam(tinyxml2::XMLElement const* element, std::set<std::string> & dependencies, std::vector<ParamData> & params);
    tinyxml2::XMLNode const* readCommandParamType(tinyxml2::XMLNode const* node, ParamData& param);
    void readCommandsCommand(tinyxml2::XMLElement const* element);
    void readCommandProto(tinyxml2::XMLElement const* element, std::string & returnType, std::string & unchangedReturnType, std::string & fullName);
    void readDisabledExtensionRequire(tinyxml2::XMLElement const* element);
    void readEnumsEnum(tinyxml2::XMLElement const* element, EnumData & enumData, std::string const& tag);
    void readEnumsConstant(tinyxml2::XMLElement const* element);
    void readExtensionCommand(tinyxml2::XMLElement const* element, std::string const& protect);
    void readExtensionEnum(tinyxml2::XMLElement const* element, std::string const& tag);
    void readExtensionRequire(tinyxml2::XMLElement const* element, std::string const& protect, std::string const& tag);
    void readExtensionsExtension(tinyxml2::XMLElement const* element);
    void readExtensionType(tinyxml2::XMLElement const* element, std::string const& protect);
    void readFeatureRequire(tinyxml2::XMLElement const* element);
    void readFeatureRequireEnum(tinyxml2::XMLElement const* element);
    void readTag(tinyxml2::XMLElement const* element);
    void readType(tinyxml2::XMLElement const* element);
    void readTypeBasetype(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readTypeBitmask(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readTypeDefine(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readTypeFuncpointer(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readTypeHandle(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readTypeName(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
    void readTypeStruct(tinyxml2::XMLElement const* element, bool isUnion, std::map<std::string, std::string> const& attributes);
    void readTypeStructMember(tinyxml2::XMLElement const* element, StructData & structData);
    void registerDeleter(CommandData const& commandData);
    void setDefault(std::string const& name, std::map<std::string, std::string> & defaultValues, EnumData const& enumData);
    void writeArguments(std::ostream & os, CommandData const& commandData, bool firstCall, bool singular, size_t from, size_t to);
    void writeBitmaskToString(std::ostream & os, std::string const& flagsName, EnumData const &enumData);
    void writeCall(std::ostream & os, CommandData const& commandData, bool firstCall, bool singular);
    void writeCallCountParameter(std::ostream & os, CommandData const& commandData, bool singular, std::map<size_t, size_t>::const_iterator it);
    void writeCallPlainTypeParameter(std::ostream & os, ParamData const& paramData);
    void writeCallVectorParameter(std::ostream & os, CommandData const& commandData, bool firstCall, bool singular, std::map<size_t, size_t>::const_iterator it);
    void writeCallVulkanTypeParameter(std::ostream & os, ParamData const& paramData);
    void writeEnumsToString(std::ostream & os, EnumData const& enumData);
    void writeExceptionsForEnum(std::ostream & os, EnumData const& enumData);
    void writeFunction(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool definition, bool enhanced, bool singular, bool unique, bool isStructureChain);
    void writeFunctionBodyEnhanced(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool singular, bool unique, bool isStructureChain);
    void writeFunctionBodyEnhanced(std::ostream & os, std::string const& templateString, std::string const& indentation, CommandData const& commandData, bool singular);
    void writeFunctionBodyTwoStep(std::ostream & os, std::string const &templateString, std::string const& indentation, std::string const& returnName, std::string const& sizeName, CommandData const& commandData);
    std::string writeFunctionBodyEnhancedLocalReturnVariable(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool singular, bool isStructureChain);
    void writeFunctionBodyEnhancedCall(std::ostream &os, std::string const& indentation, CommandData const& commandData, bool singular);
    void writeFunctionBodyEnhancedCallResult(std::ostream &os, std::string const& indentation, CommandData const& commandData, bool singular);
    void writeFunctionBodyEnhancedCallTwoStep(std::ostream & os, std::string const& indentation, std::string const& returnName, std::string const& sizeName, CommandData const& commandData);
    void writeFunctionBodyEnhancedCallTwoStepIterate(std::ostream & os, std::string const& indentation, std::string const& returnName, std::string const& sizeName, CommandData const& commandData);
    void writeFunctionBodyEnhancedCallTwoStepChecked(std::ostream & os, std::string const& indentation, std::string const& returnName, std::string const& sizeName, CommandData const& commandData);
    void writeFunctionBodyEnhancedLocalCountVariable(std::ostream & os, std::string const& indentation, CommandData const& commandData);
    void writeFunctionBodyEnhancedMultiVectorSizeCheck(std::ostream & os, std::string const& indentation, CommandData const& commandData);
    void writeFunctionBodyEnhancedReturnResultValue(std::ostream & os, std::string const& indentation, std::string const& returnName, CommandData const& commandData, bool singular, bool unique);
    void writeFunctionBodyStandard(std::ostream & os, std::string const& indentation, CommandData const& commandData);
    void writeFunctionHeaderArguments(std::ostream & os, CommandData const& commandData, bool enhanced, bool singular, bool withDefaults);
    void writeFunctionHeaderArgumentsEnhanced(std::ostream & os, CommandData const& commandData, bool singular, bool withDefaults);
    void writeFunctionHeaderArgumentsStandard(std::ostream & os, CommandData const& commandData, bool withDefaults);
    void writeFunctionHeaderReturnType(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool enhanced, bool singular, bool unique, bool isStructureChain);
    void writeFunctionHeaderTemplate(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool enhanced, bool unique, bool withDefault, bool isStructureChain);
    void writeIndentation(std::ostream & os, size_t indentLength);
    void writeStructConstructor(std::ostream & os, std::string const& name, StructData const& structData, std::map<std::string, std::string> const& defaultValues);
    bool writeStructConstructorArgument(std::ostream & os, bool listedArgument, size_t indentLength, MemberData const& memberData, std::map<std::string, std::string> const& defaultValues);
    void writeStructSetter(std::ostream & os, std::string const& structureName, MemberData const& memberData);
    void writeStructureChainValidation(std::ostream & os, DependencyData const& dependencyData);
    void writeThrowExceptions(std::ostream& os, EnumData const& enumData);
    void writeTypeBitmask(std::ostream & os, std::string const& flagsName, BitmaskData const& bitmaskData, EnumData const& enumData);
    void writeTypeCommand(std::ostream & os, DependencyData const& dependencyData);
    void writeTypeCommand(std::ostream &os, std::string const& indentation, CommandData const& commandData, bool definition);
    void writeTypeEnum(std::ostream & os, EnumData const& enumData);
    void writeTypeHandle(std::ostream & os, DependencyData const& dependencyData, HandleData const& handle);
    void writeTypeScalar(std::ostream & os, DependencyData const& dependencyData);
    void writeTypeStruct(std::ostream & os, DependencyData const& dependencyData, std::map<std::string, std::string> const& defaultValues);
    void writeTypeUnion(std::ostream & os, DependencyData const& dependencyData, std::map<std::string, std::string> const& defaultValues);
    void writeUniqueTypes(std::ostream &os, std::pair<std::string, std::set<std::string>> const& deleterTypes);
#if !defined(NDEBUG)
    void skipVendorID(tinyxml2::XMLElement const* element);
#endif

  private:
    std::map<std::string, BitmaskData>            m_bitmasks;
    std::map<std::string, CommandData>            m_commands;
    std::map<std::string, std::string>            m_constants;
    std::set<std::string>                         m_defines;
    std::map<std::string, DeleterData>            m_deleters;     // map from child types to corresponding deleter data
    std::map<std::string, std::set<std::string>>  m_deleterTypes; // map from parent type to set of child types
    std::list<DependencyData>                     m_dependencies;
    std::map<std::string, EnumData>               m_enums;
    std::set<std::string>                         m_extendedStructs; // structs which are referenced by the structextends tag
    std::map<std::string, HandleData>             m_handles;
    std::map<std::string, std::string>            m_nameMap;
    std::map<std::string, ScalarData>             m_scalars;
    std::map<std::string, StructData>             m_structs;
    std::set<std::string>                         m_tags;
    std::string                                   m_typesafeCheck;
    std::string                                   m_version;
    std::set<std::string>                         m_vkTypes;
    std::string                                   m_vulkanLicenseHeader;
#if !defined(NDEBUG)
    std::map<std::string, ExtensionData>          m_extensions;
    std::vector<VendorIDData>                     m_vendorIDs;
#endif
};