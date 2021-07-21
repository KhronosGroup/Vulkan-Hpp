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

#include <iostream>
#include <map>
#include <set>
#include <tinyxml2.h>
#include <vector>

class VulkanHppGenerator
{
public:
  VulkanHppGenerator( tinyxml2::XMLDocument const & document );

  std::string         generateBaseTypes() const;
  std::string         generateBitmasks() const;
  std::string         generateCommandDefinitions() const;
  std::string         generateDispatchLoaderDynamic() const;  // uses vkGet*ProcAddress to get function pointers
  std::string         generateDispatchLoaderStatic() const;   // uses exported symbols from loader
  std::string         generateEnums() const;
  std::string         generateHandles() const;
  std::string         generateHashStructures() const;
  std::string         generateIndexTypeTraits() const;
  std::string         generateRAIICommandDefinitions() const;
  std::string         generateRAIIDispatchers() const;
  std::string         generateRAIIHandles() const;
  std::string         generateResultExceptions() const;
  std::string         generateStructExtendsStructs() const;
  std::string         generateStructs() const;
  std::string         generateThrowResultException() const;
  std::string const & getTypesafeCheck() const;
  std::string const & getVersion() const;
  std::string const & getVulkanLicenseHeader() const;
  void                prepareRAIIHandles();

private:
  struct TypeInfo
  {
    std::string compose( bool inNamespace = true ) const;

    bool operator==( TypeInfo const & rhs ) const
    {
      return ( prefix == rhs.prefix ) && ( type == rhs.type ) && ( postfix == rhs.postfix );
    }

    bool isConstPointer() const
    {
      return ( prefix.find( "const" ) != std::string::npos ) && ( postfix.find( '*' ) != std::string::npos );
    }

    bool isNonConstPointer() const
    {
      return ( prefix.find( "const" ) == std::string::npos ) && ( postfix.find( '*' ) != std::string::npos );
    }

    bool isValue() const
    {
      return ( ( prefix.find( '*' ) == std::string::npos ) && ( postfix.find( '*' ) == std::string::npos ) );
    }

    std::string prefix;
    std::string type;
    std::string postfix;
  };

  struct BaseTypeData
  {
    BaseTypeData( TypeInfo const & typeInfo_, int line ) : typeInfo( typeInfo_ ), xmlLine( line ) {}

    TypeInfo typeInfo;
    int      xmlLine;
  };

  struct BitmaskData
  {
    BitmaskData( std::string const & r, std::string const & t, int line )
      : requirements( r ), type( t ), xmlLine( line )
    {}

    std::string requirements;
    std::string type;
    std::string alias;
    int         xmlLine;
  };

  struct NameData
  {
    std::string              name;
    std::vector<std::string> arraySizes;
  };

  struct ParamData
  {
    ParamData( int line ) : optional( false ), xmlLine( line ) {}

    TypeInfo                 type;
    std::string              name;
    std::vector<std::string> arraySizes;
    std::string              len;
    bool                     optional;
    int                      xmlLine;
  };

  struct CommandData
  {
    CommandData( int line ) : xmlLine( line ) {}

    std::string              alias;
    std::vector<std::string> errorCodes;
    std::string              handle;
    std::vector<ParamData>   params;
    std::string              referencedIn;
    std::string              returnType;
    std::vector<std::string> successCodes;
    int                      xmlLine;
  };

  struct EnumAliasData
  {
    EnumAliasData( std::string const & name_, int line ) : name( name_ ), xmlLine( line ) {}

    std::string name;
    int         xmlLine;
  };

  struct EnumValueData
  {
    EnumValueData( int                 line,
                   std::string const & name_,
                   std::string const & protect_,
                   std::string const & extension_,
                   bool                singleBit_ )
      : name( name_ ), extension( extension_ ), protect( protect_ ), singleBit( singleBit_ ), xmlLine( line )
    {}

    std::string name;
    std::string extension;
    std::string protect;
    bool        singleBit;
    int         xmlLine;
  };

  struct EnumData
  {
    EnumData( int line ) : xmlLine( line ) {}
    void addEnumAlias( int line, std::string const & name, std::string const & alias );
    void addEnumValue( int                 line,
                       std::string const & valueName,
                       std::string const & protect,
                       bool                bitpos,
                       std::string const & extension );

    std::string                          alias;    // alias for this enum
    std::map<std::string, EnumAliasData> aliases;  // aliases for the values
    std::string                          bitwidth;
    bool                                 isBitmask = false;
    std::vector<EnumValueData>           values;
    int                                  xmlLine;
  };

  struct FeatureData
  {
    FeatureData( std::string const & number_ ) : number( number_ ) {}

    std::vector<std::string> commands;
    std::string              number;
    std::vector<std::string> types;
  };

  struct ExtensionData
  {
    ExtensionData( int                 line,
                   std::string const & deprecatedBy_,
                   std::string const & number_,
                   std::string const & obsoletedBy_,
                   std::string const & platform_,
                   std::string const & promotedTo_ )
      : deprecatedBy( deprecatedBy_ )
      , number( number_ )
      , obsoletedBy( obsoletedBy_ )
      , platform( platform_ )
      , promotedTo( promotedTo_ )
      , xmlLine( line )
    {}

    std::vector<std::string>   commands;
    std::string                deprecatedBy;
    std::string                number;
    std::string                obsoletedBy;
    std::string                platform;
    std::string                promotedTo;
    std::map<std::string, int> requirements;
    std::vector<std::string>   types;
    int                        xmlLine;
  };

  struct FuncPointerData
  {
    FuncPointerData( std::string const & r, int line ) : requirements( r ), xmlLine( line ) {}

    std::string requirements;
    int         xmlLine;
  };

  struct HandleData
  {
    HandleData( std::string const & p, std::string const & objType, int line )
      : objTypeEnum( objType ), parent( p ), xmlLine( line )
    {}

    std::string           alias;
    std::set<std::string> childrenHandles;
    std::set<std::string> commands;
    std::string           deleteCommand;
    std::string           deletePool;
    std::string           objTypeEnum;
    std::string           parent;
    std::set<std::string> secondLevelCommands;
    int                   xmlLine;

    // RAII data
    std::map<std::string, CommandData>::const_iterator              destructorIt;
    std::vector<std::map<std::string, CommandData>::const_iterator> constructorIts;
  };

  struct MemberData
  {
    MemberData( int line ) : xmlLine( line ) {}

    TypeInfo                 type;
    std::string              name;
    std::vector<std::string> arraySizes;
    std::string              bitCount;
    std::vector<std::string> len;
    bool                     noAutoValidity = false;
    std::vector<bool>        optional;
    std::string              selection;
    std::string              selector;
    std::vector<std::string> values;
    std::string              usedConstant;
    int                      xmlLine;
  };

  struct PlatformData
  {
    PlatformData( std::string const & protect_ ) : protect( protect_ ) {}

    std::string protect;
  };

  struct StructureAliasData
  {
    StructureAliasData( std::string const & alias_, int line ) : alias( alias_ ), xmlLine( line ) {}

    std::string alias;
    int         xmlLine;
  };

  struct StructureData
  {
    StructureData( std::vector<std::string> const & extends, int line ) : structExtends( extends ), xmlLine( line ) {}

    bool                     allowDuplicate      = false;
    bool                     isUnion             = false;
    bool                     returnedOnly        = false;
    bool                     mutualExclusiveLens = false;
    std::vector<MemberData>  members;
    std::vector<std::string> structExtends;
    std::set<std::string>    aliases;
    std::string              subStruct;
    int                      xmlLine;
  };

  enum class TypeCategory
  {
    Bitmask,
    BaseType,
    Define,
    Enum,
    FuncPointer,
    Handle,
    Requires,
    Struct,
    Union,
    Unknown
  };

  struct TypeData
  {
    TypeData( TypeCategory category_ ) : category( category_ ) {}

    TypeCategory category;
    std::string  referencedIn;
  };

private:
  void        addCommand( std::string const & name, CommandData & commandData );
  void        addMissingFlagBits( std::vector<std::string> & types, std::string const & referencedIn );
  std::string addTitleAndProtection( std::string const & str, std::string const & title ) const;
  void        appendDispatchLoaderDynamicCommands( std::vector<std::string> const & commands,
                                                   std::set<std::string> &          listedCommands,
                                                   std::string const &              title,
                                                   std::string &                    commandMembers,
                                                   std::string &                    initialCommandAssignments,
                                                   std::string &                    instanceCommandAssignments,
                                                   std::string &                    deviceCommandAssignments ) const;
  void        appendFunctionHeaderArgumentEnhancedVector( std::string &       str,
                                                          ParamData const &   param,
                                                          std::string const & strippedParameterName,
                                                          bool                hasSizeParam,
                                                          bool                isTemplateParam ) const;
  void        appendRAIIHandle( std::string &                              str,
                                std::pair<std::string, HandleData> const & handle,
                                std::set<std::string> &                    listedHandles,
                                std::set<std::string> const &              specialFunctions ) const;
  void        appendRAIIHandleContext( std::string &                              str,
                                       std::pair<std::string, HandleData> const & handle,
                                       std::set<std::string> const &              specialFunctions ) const;
  void        appendStruct( std::string &                                 str,
                            std::pair<std::string, StructureData> const & structure,
                            std::set<std::string> &                       listedStructs ) const;
  void        appendStructAssignmentOperators( std::string &                                 str,
                                               std::pair<std::string, StructureData> const & structure,
                                               std::string const &                           prefix ) const;
  void appendStructCompareOperators( std::string & str, std::pair<std::string, StructureData> const & structure ) const;
  void appendStructConstructors( std::string &                                 str,
                                 std::pair<std::string, StructureData> const & structData,
                                 std::string const &                           prefix ) const;
  void appendStructConstructorsEnhanced( std::string &                                 str,
                                         std::pair<std::string, StructureData> const & structData,
                                         std::string const &                           prefix ) const;
  bool appendStructConstructorArgument( std::string &      str,
                                        bool               listedArgument,
                                        MemberData const & memberData,
                                        bool               withDefault ) const;
  std::string appendStructMembers( std::string &                                 str,
                                   std::pair<std::string, StructureData> const & structData,
                                   std::string const &                           prefix ) const;
  void        appendStructSetter( std::string &                   str,
                                  std::string const &             structureName,
                                  std::vector<MemberData> const & memberData,
                                  size_t                          index ) const;
  void        appendStructSubConstructor( std::string &                                 str,
                                          std::pair<std::string, StructureData> const & structData,
                                          std::string const &                           prefix ) const;
  void        appendStructure( std::string & str, std::pair<std::string, StructureData> const & structure ) const;
  void        appendType( std::string & str, std::string const & typeName, std::set<std::string> & listedTypes ) const;
  void        appendUnion( std::string & str, std::pair<std::string, StructureData> const & structure ) const;
  void        appendUniqueTypes( std::string &                 str,
                                 std::string const &           parentType,
                                 std::set<std::string> const & childrenTypes ) const;
  bool        checkEquivalentSingularConstructor(
           std::vector<std::map<std::string, CommandData>::const_iterator> const & constructorIts,
           std::map<std::string, CommandData>::const_iterator                      constructorIt,
           std::vector<ParamData>::const_iterator                                  lenIt ) const;
  std::string constructArgumentListEnhanced( std::vector<ParamData> const & params,
                                             std::set<size_t> const &       skippedParams,
                                             std::set<size_t> const &       singularParams,
                                             bool                           definition,
                                             bool                           withAllocators,
                                             bool                           structureChain,
                                             bool                           withDispatcher ) const;
  std::string constructArgumentListStandard( std::vector<ParamData> const & params,
                                             std::set<size_t> const &       skippedParams ) const;
  std::string constructCallArgumentEnhanced( std::vector<ParamData> const & params,
                                             size_t                         paramIndex,
                                             bool                           nonConstPointerAsNullptr,
                                             std::set<size_t> const &       singularParams,
                                             std::vector<size_t> const &    returnParamIndices,
                                             bool                           raiiHandleMemberFunction ) const;
  std::string constructCallArgumentsEnhanced( std::vector<ParamData> const & params,
                                              size_t                         initialSkipCount,
                                              bool                           nonConstPointerAsNullptr,
                                              std::set<size_t> const &       singularParams,
                                              std::vector<size_t> const &    returnParamIndices,
                                              bool                           raiiHandleMemberFunction ) const;
  std::string constructCallArgumentsStandard( std::string const & handle, std::vector<ParamData> const & params ) const;
  std::string constructCommandBoolGetValue( std::string const & name,
                                            CommandData const & commandData,
                                            size_t              initialSkipCount,
                                            bool                definition,
                                            size_t              nonConstPointerIndex ) const;
  std::string constructCommandResult( std::string const &              name,
                                      CommandData const &              commandData,
                                      size_t                           initialSkipCount,
                                      bool                             definition,
                                      std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string constructCommandResultEnumerate( std::string const &               name,
                                               CommandData const &               commandData,
                                               size_t                            initialSkipCount,
                                               bool                              definition,
                                               std::pair<size_t, size_t> const & vectorParamIndices,
                                               bool                              withAllocators ) const;
  std::string constructCommandResultEnumerateChained( std::string const &               name,
                                                      CommandData const &               commandData,
                                                      size_t                            initialSkipCount,
                                                      bool                              definition,
                                                      std::pair<size_t, size_t> const & vectorParamIndex,
                                                      std::vector<size_t> const &       returnParamIndices,
                                                      bool                              withAllocator ) const;
  std::string constructCommandResultEnumerateTwoVectors( std::string const &              name,
                                                         CommandData const &              commandData,
                                                         size_t                           initialSkipCount,
                                                         bool                             definition,
                                                         std::map<size_t, size_t> const & vectorParamIndices,
                                                         std::vector<size_t> const &      returnParamIndices,
                                                         bool                             withAllocators ) const;
  std::string constructCommandResultEnumerateTwoVectorsDeprecated( std::string const &              name,
                                                                   CommandData const &              commandData,
                                                                   size_t                           initialSkipCount,
                                                                   bool                             definition,
                                                                   std::map<size_t, size_t> const & vectorParamIndices,
                                                                   bool withAllocators ) const;
  std::string constructCommandResultGetChain( std::string const & name,
                                              CommandData const & commandData,
                                              size_t              initialSkipCount,
                                              bool                definition,
                                              size_t              nonConstPointerIndex ) const;
  std::string constructCommandResultGetHandleUnique( std::string const & name,
                                                     CommandData const & commandData,
                                                     size_t              initialSkipCount,
                                                     bool                definition,
                                                     size_t              nonConstPointerIndex ) const;
  std::string constructCommandResultGetTwoValues( std::string const &         name,
                                                  CommandData const &         commandData,
                                                  size_t                      initialSkipCount,
                                                  bool                        definition,
                                                  std::vector<size_t> const & nonConstPointerParamIndices ) const;
  std::string constructCommandResultGetTwoVectors( std::string const &              name,
                                                   CommandData const &              commandData,
                                                   size_t                           initialSkipCount,
                                                   bool                             definition,
                                                   std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string constructCommandResultGetValue( std::string const & name,
                                              CommandData const & commandData,
                                              size_t              initialSkipCount,
                                              bool                definition,
                                              size_t              nonConstPointerIndex ) const;
  std::string constructCommandResultGetValueDeprecated( std::string const &              name,
                                                        CommandData const &              commandData,
                                                        size_t                           initialSkipCount,
                                                        bool                             definition,
                                                        std::map<size_t, size_t> const & vectorParamIndices,
                                                        size_t                           returnParamIndex ) const;
  std::string constructCommandResultGetVector( std::string const &              name,
                                               CommandData const &              commandData,
                                               size_t                           initialSkipCount,
                                               bool                             definition,
                                               std::map<size_t, size_t> const & vectorParamIndices,
                                               size_t                           returnParamIndex ) const;
  std::string constructCommandResultGetVector( std::string const &              name,
                                               CommandData const &              commandData,
                                               size_t                           initialSkipCount,
                                               bool                             definition,
                                               std::map<size_t, size_t> const & vectorParamIndices,
                                               size_t                           returnParamIndex,
                                               bool                             withAllocator ) const;
  std::string constructCommandResultGetVectorAndValue( std::string const &              name,
                                                       CommandData const &              commandData,
                                                       size_t                           initialSkipCount,
                                                       bool                             definition,
                                                       std::map<size_t, size_t> const & vectorParamIndices,
                                                       std::vector<size_t> const &      returnParamIndex,
                                                       bool                             withAllocator ) const;
  std::string constructCommandResultGetVectorDeprecated( std::string const &              name,
                                                         CommandData const &              commandData,
                                                         size_t                           initialSkipCount,
                                                         bool                             definition,
                                                         std::map<size_t, size_t> const & vectorParamIndices,
                                                         size_t                           returnParamIndex ) const;
  std::string constructCommandResultGetVectorOfHandles( std::string const &              name,
                                                        CommandData const &              commandData,
                                                        size_t                           initialSkipCount,
                                                        bool                             definition,
                                                        std::map<size_t, size_t> const & vectorParamIndices,
                                                        size_t                           returnParamIndex,
                                                        bool                             withAllocator ) const;
  std::string constructCommandResultGetVectorOfHandlesSingular( std::string const &              name,
                                                                CommandData const &              commandData,
                                                                size_t                           initialSkipCount,
                                                                bool                             definition,
                                                                std::map<size_t, size_t> const & vectorParamIndices,
                                                                size_t returnParamIndex ) const;
  std::string constructCommandResultGetVectorOfHandlesUnique( std::string const &              name,
                                                              CommandData const &              commandData,
                                                              size_t                           initialSkipCount,
                                                              bool                             definition,
                                                              std::map<size_t, size_t> const & vectorParamIndices,
                                                              size_t                           returnParamIndex,
                                                              bool                             withAllocator ) const;
  std::string
              constructCommandResultGetVectorOfHandlesUniqueSingular( std::string const &              name,
                                                                      CommandData const &              commandData,
                                                                      size_t                           initialSkipCount,
                                                                      bool                             definition,
                                                                      std::map<size_t, size_t> const & vectorParamIndices,
                                                                      size_t                           returnParamIndex ) const;
  std::string constructCommandResultGetVectorOfVoidSingular( std::string const &              name,
                                                             CommandData const &              commandData,
                                                             size_t                           initialSkipCount,
                                                             bool                             definition,
                                                             std::map<size_t, size_t> const & vectorParamIndices,
                                                             size_t                           returnParamIndex ) const;
  std::string constructCommandResultGetVectorSingular( std::string const &              name,
                                                       CommandData const &              commandData,
                                                       size_t                           initialSkipCount,
                                                       bool                             definition,
                                                       std::map<size_t, size_t> const & vectorParamIndices,
                                                       size_t                           returnParamIndex ) const;
  std::string constructCommandStandard( std::string const & name,
                                        CommandData const & commandData,
                                        size_t              initialSkipCount,
                                        bool                definition ) const;
  std::string constructCommandType( std::string const & name,
                                    CommandData const & commandData,
                                    size_t              initialSkipCount,
                                    bool                definition ) const;
  std::string constructCommandVoid( std::string const &              name,
                                    CommandData const &              commandData,
                                    size_t                           initialSkipCount,
                                    bool                             definition,
                                    std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string constructCommandVoidEnumerate( std::string const &               name,
                                             CommandData const &               commandData,
                                             size_t                            initialSkipCount,
                                             bool                              definition,
                                             std::pair<size_t, size_t> const & vectorParamIndex,
                                             std::vector<size_t> const &       returnParamIndices,
                                             bool                              withAllocators ) const;
  std::string constructCommandVoidEnumerateChained( std::string const &               name,
                                                    CommandData const &               commandData,
                                                    size_t                            initialSkipCount,
                                                    bool                              definition,
                                                    std::pair<size_t, size_t> const & vectorParamIndex,
                                                    std::vector<size_t> const &       returnParamIndices,
                                                    bool                              withAllocators ) const;
  std::string constructCommandVoidGetChain( std::string const & name,
                                            CommandData const & commandData,
                                            size_t              initialSkipCount,
                                            bool                definition,
                                            size_t              nonConstPointerIndex ) const;
  std::string constructCommandVoidGetValue( std::string const &              name,
                                            CommandData const &              commandData,
                                            size_t                           initialSkipCount,
                                            bool                             definition,
                                            std::map<size_t, size_t> const & vectorParamIndices,
                                            size_t                           returnParamIndex ) const;
  std::string constructConstexprString( std::pair<std::string, StructureData> const & structData,
                                        bool                                          assignmentOperator ) const;
  std::string constructFailureCheck( std::vector<std::string> const & successCodes ) const;
  std::string constructFunctionBodyEnhanced( std::string const &              indentation,
                                             std::string const &              name,
                                             CommandData const &              commandData,
                                             size_t                           initialSkipCount,
                                             size_t                           returnParamIndex,
                                             size_t                           templateParamIndex,
                                             std::map<size_t, size_t> const & vectorParamIndices,
                                             bool                             twoStep,
                                             std::string const &              enhancedReturnType,
                                             bool                             withAllocator ) const;
  std::string constructFunctionBodyEnhancedSingleStep( std::string const &              indentation,
                                                       std::string const &              name,
                                                       CommandData const &              commandData,
                                                       size_t                           returnParamIndex,
                                                       size_t                           templateParamIndex,
                                                       std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string constructFunctionHeaderArgumentsEnhanced( CommandData const &              commandData,
                                                        size_t                           returnParamIndex,
                                                        size_t                           templateParamIndex,
                                                        std::map<size_t, size_t> const & vectorParamIndices,
                                                        bool                             withDefaults,
                                                        bool                             withAllocator ) const;
  std::string constructFunctionPointerCheck( std::string const & function, std::string const & referencedIn ) const;
  std::string constructNoDiscardStandard( CommandData const & commandData ) const;
  std::pair<std::string, std::string> constructRAIIHandleConstructor(
    std::pair<std::string, HandleData> const &                             handle,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
    std::string const &                                                    enter,
    std::string const &                                                    leave ) const;
  std::string constructRAIIHandleConstructorArguments( std::string const &            handleType,
                                                       std::vector<ParamData> const & params,
                                                       bool                           singular,
                                                       bool                           encounteredArgument ) const;
  std::string constructRAIIHandleMemberFunctionResultSingleGetVector(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    size_t                                             returnParamIndex,
    bool                                               definition ) const;
  std::string constructRAIIHandleConstructorCallArguments( std::string const &            handleType,
                                                           std::vector<ParamData> const & params,
                                                           bool                           nonConstPointerAsNullptr,
                                                           std::set<size_t> const &       singularParams,
                                                           std::vector<size_t> const &    returnParamIndices,
                                                           bool allocatorIsMemberVariable ) const;
  std::string constructRAIIHandleConstructorEnumerate(
    std::pair<std::string, HandleData> const &                             handle,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
    std::vector<ParamData>::const_iterator                                 handleParamIt,
    std::vector<ParamData>::const_iterator                                 lenParamIt,
    std::string const &                                                    enter,
    std::string const &                                                    leave ) const;
  std::string constructRAIIHandleConstructorInitializationList(
    std::string const &                                                    handleType,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator destructorIt,
    bool                                                                   hasSecondLevelCommands ) const;
  std::string constructRAIIHandleConstructorResult(
    std::pair<std::string, HandleData> const &                             handle,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
    std::string const &                                                    enter,
    std::string const &                                                    leave ) const;
  std::string constructRAIIHandleConstructorTakeOwnership( std::pair<std::string, HandleData> const & handle ) const;
  std::pair<std::string, std::string>
              constructRAIIHandleConstructors( std::pair<std::string, HandleData> const & handle ) const;
  std::string constructRAIIHandleConstructorVector(
    std::pair<std::string, HandleData> const &                             handle,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
    std::vector<ParamData>::const_iterator                                 handleParamIt,
    std::string const &                                                    enter,
    std::string const &                                                    leave ) const;
  std::string constructRAIIHandleConstructorVectorSingular(
    std::pair<std::string, HandleData> const &                             handle,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
    std::vector<ParamData>::const_iterator                                 handleParamIt,
    std::string const &                                                    enter,
    std::string const &                                                    leave ) const;
  std::string constructRAIIHandleConstructorVoid(
    std::pair<std::string, HandleData> const &                             handle,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
    std::string const &                                                    enter,
    std::string const &                                                    leave ) const;
  std::pair<std::string, std::string>
              constructRAIIHandleDestructor( std::string const &                                handleType,
                                             std::map<std::string, CommandData>::const_iterator destructorIt,
                                             std::string const &                                enter ) const;
  std::string constructRAIIHandleDestructorCallArguments( std::string const &            handleType,
                                                          std::vector<ParamData> const & params ) const;
  std::tuple<std::string, std::string, std::string, std::string>
              constructRAIIHandleDetails( std::pair<std::string, HandleData> const & handle,
                                          std::string const &                        destructorCall ) const;
  std::string constructRAIIHandleMemberFunction( std::string const &           command,
                                                 size_t                        initialSkipCount,
                                                 std::set<std::string> const & specialFunctions,
                                                 bool                          definition ) const;
  std::string
    constructRAIIHandleMemberFunctionResultEnumerate( std::map<std::string, CommandData>::const_iterator commandIt,
                                                      size_t                           initialSkipCount,
                                                      std::map<size_t, size_t> const & vectorParamIndices,
                                                      std::vector<size_t> const &      nonConstPointerParamIndices,
                                                      bool                             definition ) const;
  std::string constructRAIIHandleMemberFunctionResultEnumerateTwoVectors(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices,
    bool                                               definition ) const;
  std::pair<std::string, std::string> constructRAIIHandleMemberFunctionResultGetTwoValues(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices ) const;
  std::string
    constructRAIIHandleMemberFunctionResultMulti( std::map<std::string, CommandData>::const_iterator commandIt,
                                                  size_t                                             initialSkipCount,
                                                  std::map<size_t, size_t> const &                   vectorParamIndices,
                                                  bool                                               definition ) const;
  std::string
    constructRAIIHandleMemberFunctionResultMultiGetValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                          size_t                           initialSkipCount,
                                                          std::map<size_t, size_t> const & vectorParamIndices,
                                                          std::vector<size_t> const &      nonConstPointerParamIndices,
                                                          bool                             definition ) const;
  std::string constructRAIIHandleMemberFunctionResultMultiGetVectorOfVoid(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices,
    bool                                               definition ) const;
  std::string constructRAIIHandleMemberFunctionResultMultiGetVectorOfVoidSingular(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices,
    bool                                               definition ) const;
  std::string
    constructRAIIHandleMemberFunctionResultMultiNoErrors( std::map<std::string, CommandData>::const_iterator commandIt,
                                                          size_t                           initialSkipCount,
                                                          std::map<size_t, size_t> const & vectorParamIndices,
                                                          bool                             definition ) const;
  std::string
    constructRAIIHandleMemberFunctionResultSingle( std::map<std::string, CommandData>::const_iterator commandIt,
                                                   size_t                                             initialSkipCount,
                                                   std::map<size_t, size_t> const & vectorParamIndices,
                                                   bool                             definition ) const;
  std::string
    constructRAIIHandleMemberFunctionResultSingleGetChain( std::map<std::string, CommandData>::const_iterator commandIt,
                                                           size_t                           initialSkipCount,
                                                           std::map<size_t, size_t> const & vectorParamIndices,
                                                           std::vector<size_t> const &      nonConstPointerParamIndices,
                                                           bool                             definition ) const;
  std::string constructRAIIHandleMemberFunctionResultSingleGetVectorAndValue(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices,
    bool                                               definition ) const;
  std::string constructRAIIHandleMemberFunctionResultSingleGetVectorOfVoid(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices,
    bool                                               definition ) const;
  std::string constructRAIIHandleMemberFunctionResultSingleGetVectorSingular(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    size_t                                             returnParamIndex,
    bool                                               definition ) const;
  std::string constructRAIIHandleMemberFunctionResultSingleGetVectorOfVoidSingular(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices,
    bool                                               definition ) const;
  std::string
    constructRAIIHandleMemberFunctionResultSingleNoErrors( std::map<std::string, CommandData>::const_iterator commandIt,
                                                           size_t                           initialSkipCount,
                                                           std::map<size_t, size_t> const & vectorParamIndices,
                                                           bool                             definition ) const;
  std::string
    constructRAIIHandleMemberFunctionResultSingleGetValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                           size_t                           initialSkipCount,
                                                           std::map<size_t, size_t> const & vectorParamIndices,
                                                           std::vector<size_t> const &      nonConstPointerParamIndices,
                                                           bool                             definition ) const;
  std::pair<std::string, std::string>
    constructRAIIHandleMemberFunctionBoolGetValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                   size_t                                             initialSkipCount,
                                                   std::map<size_t, size_t> const & vectorParamIndices,
                                                   std::vector<size_t> const &      nonConstPointerParamIndices ) const;
  std::string constructRAIIHandleMemberFunctionResult( std::map<std::string, CommandData>::const_iterator commandIt,
                                                       size_t initialSkipCount,
                                                       bool   definition ) const;
  std::string constructRAIIHandleMemberFunctionResult0Return(
    std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount, bool definition ) const;
  std::string
    constructRAIIHandleMemberFunctionResult1Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                    size_t                                             initialSkipCount,
                                                    size_t                                             returnParamIndex,
                                                    bool definition ) const;
  std::string
    constructRAIIHandleMemberFunctionResult2Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                    size_t                                             initialSkipCount,
                                                    std::vector<size_t> const & returnParamIndices,
                                                    bool                        definition ) const;
  std::string
    constructRAIIHandleMemberFunctionResult3Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                    size_t                                             initialSkipCount,
                                                    std::vector<size_t> const & returnParamIndices,
                                                    bool                        definition ) const;
  std::string constructRAIIHandleMemberFunctionDeclarations( std::pair<std::string, HandleData> const & handle,
                                                             std::set<std::string> const & specialFunctions ) const;
  std::string constructRAIIHandleMemberFunctionType( std::map<std::string, CommandData>::const_iterator commandIt,
                                                     size_t                           initialSkipCount,
                                                     std::map<size_t, size_t> const & vectorParamIndices,
                                                     bool                             definition ) const;
  std::string constructRAIIHandleMemberFunctionVkType( std::map<std::string, CommandData>::const_iterator commandIt,
                                                       size_t                           initialSkipCount,
                                                       std::map<size_t, size_t> const & vectorParamIndices,
                                                       bool                             definition ) const;
  std::string constructRAIIHandleMemberFunctionValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                      size_t initialSkipCount,
                                                      bool   definition ) const;
  std::string constructRAIIHandleMemberFunctionValue0Return(
    std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount, bool definition ) const;
  std::string constructRAIIHandleMemberFunctionVoid( std::map<std::string, CommandData>::const_iterator commandIt,
                                                     size_t initialSkipCount,
                                                     bool   definition ) const;
  std::string constructRAIIHandleMemberFunctionVoid0Return(
    std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount, bool definition ) const;
  std::string
    constructRAIIHandleMemberFunctionVoid1Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                  size_t                                             initialSkipCount,
                                                  size_t                                             returnParamIndex,
                                                  bool                                               definition ) const;
  std::string
    constructRAIIHandleMemberFunctionVoid2Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                  size_t                                             initialSkipCount,
                                                  std::vector<size_t> const &                        returnParamIndices,
                                                  bool                                               definition ) const;
  std::string
    constructRAIIHandleMemberFunctionVoidEnumerate( std::map<std::string, CommandData>::const_iterator commandIt,
                                                    size_t                                             initialSkipCount,
                                                    std::map<size_t, size_t> const & vectorParamIndices,
                                                    std::vector<size_t> const &      nonConstPointerParamIndices,
                                                    bool                             definition ) const;
  std::string
    constructRAIIHandleMemberFunctionVoidEnumerateChain( std::map<std::string, CommandData>::const_iterator commandIt,
                                                         size_t                           initialSkipCount,
                                                         std::map<size_t, size_t> const & vectorParamIndices,
                                                         std::vector<size_t> const &      nonConstPointerParamIndices,
                                                         bool                             definition ) const;
  std::string
    constructRAIIHandleMemberFunctionVoidGetChain( std::map<std::string, CommandData>::const_iterator commandIt,
                                                   size_t                                             initialSkipCount,
                                                   std::map<size_t, size_t> const & vectorParamIndices,
                                                   std::vector<size_t> const &      nonConstPointerParamIndices,
                                                   bool                             definition ) const;
  std::string
    constructRAIIHandleMemberFunctionVoidGetValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                   size_t                                             initialSkipCount,
                                                   std::map<size_t, size_t> const & vectorParamIndices,
                                                   std::vector<size_t> const &      nonConstPointerParamIndices,
                                                   bool                             definition ) const;
  std::string constructRAIIHandleSingularConstructorArguments(
    std::pair<std::string, HandleData> const &         handle,
    std::map<std::string, CommandData>::const_iterator constructorIt ) const;
  std::string constructRAIIHandleVectorSizeCheck( std::string const &                           name,
                                                  CommandData const &                           commandData,
                                                  size_t                                        initialSkipCount,
                                                  std::map<size_t, std::vector<size_t>> const & countToVectorMap,
                                                  std::set<size_t> const &                      skippedParams ) const;
  std::string constructRAIIHandleUpgradeConstructor( std::pair<std::string, HandleData> const & handle ) const;
  std::string constructReturnType( CommandData const & commandData, std::string const & baseType ) const;
  std::string constructSuccessCheck( std::vector<std::string> const & successCodes ) const;
  std::string constructSuccessCodeList( std::vector<std::string> const & successCodes ) const;
  std::string constructVectorSizeCheck( std::string const &                           name,
                                        CommandData const &                           commandData,
                                        size_t                                        initialSkipCount,
                                        std::map<size_t, std::vector<size_t>> const & countToVectorMap,
                                        std::set<size_t> const &                      skippedParams ) const;
  void        checkCorrectness();
  void        checkEnumCorrectness( std::vector<std::string> const & types ) const;
  bool        containsArray( std::string const & type ) const;
  bool        containsUnion( std::string const & type ) const;
  size_t      determineDefaultStartIndex( std::vector<ParamData> const & params,
                                          std::set<size_t> const &       skippedParams ) const;
  std::string determineEnhancedReturnType( CommandData const & commandData,
                                           size_t              returnParamIndex,
                                           bool                isStructureChain ) const;
  size_t      determineInitialSkipCount( std::string const & command ) const;
  std::vector<std::map<std::string, CommandData>::const_iterator>
    determineRAIIHandleConstructors( std::string const &                                handleType,
                                     std::map<std::string, CommandData>::const_iterator destructorIt,
                                     std::set<std::string> &                            specialFunctions ) const;
  std::map<std::string, CommandData>::const_iterator
                           determineRAIIHandleDestructor( std::string const & handleType ) const;
  size_t                   determineReturnParamIndex( CommandData const &              commandData,
                                                      std::map<size_t, size_t> const & vectorParamIndices,
                                                      bool                             twoStep ) const;
  std::set<size_t>         determineSkippedParams( std::vector<ParamData> const &   params,
                                                   size_t                           initialSkipCount,
                                                   std::map<size_t, size_t> const & vectorParamIndices,
                                                   std::vector<size_t> const &      returnParamIndex,
                                                   bool                             singular ) const;
  std::string              determineSubStruct( std::pair<std::string, StructureData> const & structure ) const;
  std::vector<size_t>      determineConstPointerParamIndices( std::vector<ParamData> const & params ) const;
  std::vector<size_t>      determineNonConstPointerParamIndices( std::vector<ParamData> const & params ) const;
  std::map<size_t, size_t> determineVectorParamIndicesNew( std::vector<ParamData> const & params ) const;
  void                     distributeSecondLevelCommands( std::set<std::string> const & specialFunctions );
  std::string findBaseName( std::string aliasName, std::map<std::string, EnumAliasData> const & aliases ) const;
  std::string generateBitmask( std::map<std::string, BitmaskData>::const_iterator bitmaskIt ) const;
  std::string generateBitmasks( std::vector<std::string> const & types,
                                std::set<std::string> &          listedBitmasks,
                                std::string const &              title ) const;
  std::string generateCommand( std::string const & name,
                               CommandData const & commandData,
                               size_t              initialSkipCount,
                               bool                definition ) const;
  std::string generateCommandDefinitions( std::vector<std::string> const & commands,
                                          std::set<std::string> &          listedCommands,
                                          std::string const &              title ) const;
  std::string generateCommandDefinitions( std::string const & command, std::string const & handle ) const;
  std::string generateCommandName( std::string const &            vulkanCommandName,
                                   std::vector<ParamData> const & params,
                                   size_t                         initialSkipCount,
                                   std::set<std::string> const &  tags ) const;
  std::string generateCommandResult( std::string const & name,
                                     CommandData const & commandData,
                                     size_t              initialSkipCount,
                                     bool                definition ) const;
  std::string generateCommandResult0Return( std::string const & name,
                                            CommandData const & commandData,
                                            size_t              initialSkipCount,
                                            bool                definition ) const;
  std::string generateCommandResult1Return( std::string const & name,
                                            CommandData const & commandData,
                                            size_t              initialSkipCount,
                                            bool                definition,
                                            size_t              returnParamIndex ) const;
  std::string generateCommandResult2Return( std::string const &         name,
                                            CommandData const &         commandData,
                                            size_t                      initialSkipCount,
                                            bool                        definition,
                                            std::vector<size_t> const & returnParamIndices ) const;
  std::string generateCommandResult3Return( std::string const &         name,
                                            CommandData const &         commandData,
                                            size_t                      initialSkipCount,
                                            bool                        definition,
                                            std::vector<size_t> const & returnParamIndices ) const;
  std::string generateCommandSetStandard( std::string const & name,
                                          CommandData const & commandData,
                                          size_t              initialSkipCount,
                                          bool                definition ) const;
  std::string generateCommandSetStandardAndEnhanced( std::string const &              name,
                                                     CommandData const &              commandData,
                                                     size_t                           initialSkipCount,
                                                     bool                             definition,
                                                     std::map<size_t, size_t> const & vectorParamIndices,
                                                     std::vector<size_t> const & nonConstPointerParamIndices ) const;
  std::string generateCommandSetStandardEnhancedDeprecatedAllocator(
    std::string const &              name,
    CommandData const &              commandData,
    size_t                           initialSkipCount,
    bool                             definition,
    std::map<size_t, size_t> const & vectorParamIndices,
    std::vector<size_t> const &      nonConstPointerParamIndices ) const;
  std::string generateCommandSetStandardOrEnhanced( std::string const & name,
                                                    CommandData const & commandData,
                                                    size_t              initialSkipCount,
                                                    bool                definition ) const;
  std::string generateCommandSetUnique( std::string const & name,
                                        CommandData const & commandData,
                                        size_t              initialSkipCount,
                                        size_t              nonConstPointerIndex,
                                        bool                definition ) const;
  std::string generateCommandSetVector( std::string const &               name,
                                        CommandData const &               commandData,
                                        size_t                            initialSkipCount,
                                        bool                              definition,
                                        std::pair<size_t, size_t> const & vectorParamIndex,
                                        std::vector<size_t> const &       returnParamIndices ) const;
  std::string generateCommandSetVectorChained( std::string const &              name,
                                               CommandData const &              commandData,
                                               size_t                           initialSkipCount,
                                               bool                             definition,
                                               std::map<size_t, size_t> const & vectorParamIndices,
                                               std::vector<size_t> const &      returnParamIndices ) const;
  std::string generateCommandSetVectorDeprecated( std::string const &              name,
                                                  CommandData const &              commandData,
                                                  size_t                           initialSkipCount,
                                                  std::map<size_t, size_t> const & vectorParamIndices,
                                                  std::vector<size_t> const &      returnParamIndices,
                                                  bool                             definition ) const;
  std::string generateCommandSetVectorSingular( std::string const &              name,
                                                CommandData const &              commandData,
                                                size_t                           initialSkipCount,
                                                std::map<size_t, size_t> const & vectorParamIndices,
                                                size_t                           returnParamIndex,
                                                bool                             definition ) const;
  std::string generateCommandSetVectorSingularUnique( std::string const &              name,
                                                      CommandData const &              commandData,
                                                      size_t                           initialSkipCount,
                                                      std::map<size_t, size_t> const & vectorParamIndices,
                                                      size_t                           returnParamIndex,
                                                      bool                             definition ) const;
  std::string generateCommandSetVectorSingularVoid( std::string const &              name,
                                                    CommandData const &              commandData,
                                                    size_t                           initialSkipCount,
                                                    std::map<size_t, size_t> const & vectorParamIndices,
                                                    size_t                           returnParamIndex,
                                                    bool                             definition ) const;
  std::string generateCommandSetVectorUnique( std::string const &              name,
                                              CommandData const &              commandData,
                                              size_t                           initialSkipCount,
                                              std::map<size_t, size_t> const & vectorParamIndices,
                                              size_t                           returnParamIndex,
                                              bool                             definition ) const;
  std::string generateCommandsStandardEnhancedChained( std::string const &              name,
                                                       CommandData const &              commandData,
                                                       size_t                           initialSkipCount,
                                                       bool                             definition,
                                                       std::map<size_t, size_t> const & vectorParamIndices,
                                                       size_t                           nonConstPointerIndex ) const;
  std::string generateCommandValue( std::string const & name,
                                    CommandData const & commandData,
                                    size_t              initialSkipCount,
                                    bool                definition ) const;
  std::string generateCommandValue0Return( std::string const & name,
                                           CommandData const & commandData,
                                           size_t              initialSkipCount,
                                           bool                definition ) const;
  std::string generateCommandVoid( std::string const & name,
                                   CommandData const & commandData,
                                   size_t              initialSkipCount,
                                   bool                definition ) const;
  std::string generateCommandVoid0Return( std::string const & name,
                                          CommandData const & commandData,
                                          size_t              initialSkipCount,
                                          bool                definition ) const;
  std::string generateCommandVoid1Return( std::string const & name,
                                          CommandData const & commandData,
                                          size_t              initialSkipCount,
                                          bool                definition,
                                          size_t              returnParamIndex ) const;
  std::string generateCommandVoid2Return( std::string const &         name,
                                          CommandData const &         commandData,
                                          size_t                      initialSkipCount,
                                          bool                        definition,
                                          std::vector<size_t> const & returnParamIndices ) const;
  std::string generateDestroyCommand( std::string const & name, CommandData const & commandData ) const;
  std::string generateDispatchLoaderDynamicCommandAssignment( std::string const & commandName,
                                                              CommandData const & commandData,
                                                              std::string const & firstArg ) const;
  std::string generateDispatchLoaderStaticCommands( std::vector<std::string> const & commands,
                                                    std::set<std::string> &          listedCommands,
                                                    std::string const &              title ) const;
  std::string generateEnum( std::pair<std::string, EnumData> const & enumData ) const;
  std::string generateEnumInitializer( TypeInfo const &                   type,
                                       std::vector<std::string> const &   arraySizes,
                                       std::vector<EnumValueData> const & values,
                                       bool                               bitmask ) const;
  std::string generateEnums( std::vector<std::string> const & enums,
                             std::set<std::string> &          listedEnums,
                             std::string const &              title ) const;
  std::string generateEnumToString( std::pair<std::string, EnumData> const & enumData ) const;
  std::string generateFunctionBodyEnhancedLocalReturnVariable( std::string const & indentation,
                                                               CommandData const & commandData,
                                                               size_t              returnParamIndex,
                                                               std::string const & enhancedReturnType,
                                                               bool                withAllocator ) const;
  std::string
              generateFunctionBodyEnhancedMultiVectorSizeCheck( std::string const &              indentation,
                                                                std::string const &              name,
                                                                CommandData const &              commandData,
                                                                size_t                           initialSkipCount,
                                                                size_t                           returnParamIndex,
                                                                std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string generateFunctionBodyEnhancedReturnResultValue( std::string const & indentation,
                                                             std::string const & returnName,
                                                             std::string const & name,
                                                             CommandData const & commandData,
                                                             size_t              initialSkipCount,
                                                             size_t              returnParamIndex,
                                                             bool                twoStep ) const;
  std::string generateFunctionBodyEnhancedTwoStep( std::string const &              indentation,
                                                   std::string const &              name,
                                                   CommandData const &              commandData,
                                                   size_t                           returnParamIndex,
                                                   size_t                           templateParamIndex,
                                                   std::map<size_t, size_t> const & vectorParamIndices,
                                                   std::string const &              returnName ) const;
  std::string generateFunctionCall( std::string const &              name,
                                    CommandData const &              commandData,
                                    size_t                           returnParamIndex,
                                    size_t                           templateParamIndex,
                                    std::map<size_t, size_t> const & vectorParamIndices,
                                    bool                             twoStep,
                                    bool                             firstCall ) const;
  std::string generateFunctionHeaderArgumentEnhanced( ParamData const &                param,
                                                      size_t                           paramIndex,
                                                      std::map<size_t, size_t> const & vectorParamIndices,
                                                      bool                             skip,
                                                      bool                             isTemplateParam ) const;
  std::string generateHandle( std::pair<std::string, HandleData> const & handle,
                              std::set<std::string> &                    listedHandles ) const;
  std::string generateHashStructures( std::vector<std::string> const & types, std::string const & title ) const;
  std::string
                                      generateLenInitializer( std::vector<MemberData>::const_iterator                                        mit,
                                                              std::map<std::vector<MemberData>::const_iterator,
                                     std::vector<std::vector<MemberData>::const_iterator>>::const_iterator litit,
                                                              bool mutualExclusiveLens ) const;
  std::pair<std::string, std::string> generateProtection( std::string const & referencedIn,
                                                          std::string const & protect ) const;
  std::pair<std::string, std::string> generateProtection( std::string const & type, bool isAliased ) const;
  std::string                         generateRAIICommandDefinitions( std::vector<std::string> const & commands,
                                                                      std::set<std::string> &          listedCommands,
                                                                      std::string const &              title ) const;
  std::string generateSizeCheck( std::vector<std::vector<MemberData>::const_iterator> const & arrayIts,
                                 std::string const &                                          structName,
                                 std::string const &                                          prefix,
                                 bool mutualExclusiveLens ) const;
  std::string generateStructExtendsStructs( std::vector<std::string> const & types,
                                            std::set<std::string> &          listedStructs,
                                            std::string const &              title ) const;
  std::string getPlatform( std::string const & extension ) const;
  std::pair<std::string, std::string> getPoolTypeAndName( std::string const & type ) const;
  std::string                         getVectorSize( std::vector<ParamData> const &   params,
                                                     std::map<size_t, size_t> const & vectorParamIndices,
                                                     size_t                           returnParamIndex ) const;
  bool                                hasParentHandle( std::string const & handle, std::string const & parent ) const;
  bool                                isDeviceCommand( CommandData const & commandData ) const;
  bool                                isHandleType( std::string const & type ) const;
  bool isLenByStructMember( std::string const & name, std::vector<ParamData> const & params ) const;
  bool isLenByStructMember( std::string const & name, ParamData const & param ) const;
  bool isMultiSuccessCodeConstructor(
    std::vector<std::map<std::string, CommandData>::const_iterator> const & constructorIts ) const;
  bool isParam( std::string const & name, std::vector<ParamData> const & params ) const;
  bool isStructureChainAnchor( std::string const & type ) const;
  std::pair<bool, std::map<size_t, std::vector<size_t>>>
       needsVectorSizeCheck( std::map<size_t, size_t> const & vectorParamIndices ) const;
  void readBaseType( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void readBitmask( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void readBitmaskAlias( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void readCommand( tinyxml2::XMLElement const * element );
  void readCommand( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributess );
  void readCommandAlias( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  ParamData readCommandParam( tinyxml2::XMLElement const * element, std::vector<ParamData> const & params );
  std::pair<std::string, std::string> readCommandProto( tinyxml2::XMLElement const * element );
  void                                readCommands( tinyxml2::XMLElement const * element );
  std::string                         readComment( tinyxml2::XMLElement const * element );
  void readDefine( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void readEnum( tinyxml2::XMLElement const * element, std::map<std::string, EnumData>::iterator enumIt );
  void readEnum( tinyxml2::XMLElement const *               element,
                 std::map<std::string, std::string> const & attributes,
                 std::map<std::string, EnumData>::iterator  enumIt );
  void readEnumAlias( tinyxml2::XMLElement const *               element,
                      std::map<std::string, std::string> const & attributes,
                      EnumData &                                 enumData );
  void readEnumConstant( tinyxml2::XMLElement const * element );
  void readEnums( tinyxml2::XMLElement const * element );
  void readExtension( tinyxml2::XMLElement const * element );
  void readExtensionDisabledCommand( tinyxml2::XMLElement const * element );
  void readExtensionDisabledRequire( tinyxml2::XMLElement const * element );
  void readExtensionDisabledType( tinyxml2::XMLElement const * element );
  void readExtensionRequire( tinyxml2::XMLElement const *                   element,
                             std::map<std::string, ExtensionData>::iterator extensionIt );
  void readExtensionRequireCommand( tinyxml2::XMLElement const *                   element,
                                    std::map<std::string, ExtensionData>::iterator extensionIt );
  void readExtensionRequireType( tinyxml2::XMLElement const *                   element,
                                 std::map<std::string, ExtensionData>::iterator extensionIt );
  void readExtensions( tinyxml2::XMLElement const * element );
  void readFeature( tinyxml2::XMLElement const * element );
  void readFeatureRequire( tinyxml2::XMLElement const *                 element,
                           std::map<std::string, FeatureData>::iterator featureIt );
  void readFeatureRequireCommand( tinyxml2::XMLElement const *                 element,
                                  std::map<std::string, FeatureData>::iterator featureIt );
  void readFeatureRequireType( tinyxml2::XMLElement const *                 element,
                               std::map<std::string, FeatureData>::iterator featureIt );
  void readFuncpointer( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void readHandle( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  std::pair<NameData, TypeInfo> readNameAndType( tinyxml2::XMLElement const * elements );
  void                          readPlatform( tinyxml2::XMLElement const * element );
  void                          readPlatforms( tinyxml2::XMLElement const * element );
  void                          readRegistry( tinyxml2::XMLElement const * element );
  void                          readRequireEnum( tinyxml2::XMLElement const *                   element,
                                                 std::map<std::string, ExtensionData>::iterator extensionIt );
  void                          readRequireEnum( tinyxml2::XMLElement const *                   element,
                                                 std::map<std::string, std::string> const &     attributes,
                                                 std::map<std::string, ExtensionData>::iterator extensionIt );
  void                          readRequireEnumAlias( tinyxml2::XMLElement const *               element,
                                                      std::map<std::string, std::string> const & attributes );
  void readRequires( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void readSPIRVCapability( tinyxml2::XMLElement const * element );
  void readSPIRVCapabilityEnable( tinyxml2::XMLElement const * element );
  void readSPIRVCapabilityEnableExtension( int xmlLine, std::map<std::string, std::string> const & attributes );
  void readSPIRVCapabilityEnableProperty( int xmlLine, std::map<std::string, std::string> const & attributes );
  void readSPIRVCapabilityEnableStruct( int xmlLine, std::map<std::string, std::string> const & attributes );
  void readSPIRVCapabilityEnableVersion( int xmlLine, std::map<std::string, std::string> const & attributes );
  void readSPIRVCapabilities( tinyxml2::XMLElement const * element );
  void readSPIRVExtension( tinyxml2::XMLElement const * element );
  void readSPIRVExtensionEnable( tinyxml2::XMLElement const * element );
  void readSPIRVExtensions( tinyxml2::XMLElement const * element );
  void readStruct( tinyxml2::XMLElement const *               element,
                   bool                                       isUnion,
                   std::map<std::string, std::string> const & attributes );
  void readStructAlias( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void readStructMember( tinyxml2::XMLElement const * element, std::vector<MemberData> & members, bool isUnion );
  void readStructMemberEnum( tinyxml2::XMLElement const * element, MemberData & memberData );
  void readStructMemberName( tinyxml2::XMLElement const *    element,
                             MemberData &                    memberData,
                             std::vector<MemberData> const & members );
  void readStructMemberType( tinyxml2::XMLElement const * element, MemberData & memberData );
  void readTag( tinyxml2::XMLElement const * element );
  void readTags( tinyxml2::XMLElement const * element );
  void readType( tinyxml2::XMLElement const * element );
  void readTypeEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void readTypeInclude( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  TypeInfo    readTypeInfo( tinyxml2::XMLElement const * element ) const;
  void        readTypes( tinyxml2::XMLElement const * element );
  void        registerDeleter( std::string const & name, std::pair<std::string, CommandData> const & commandData );
  void        renameFunctionParameters();
  void        rescheduleRAIIHandle( std::string &                              str,
                                    std::pair<std::string, HandleData> const & handle,
                                    std::set<std::string> &                    listedHandles,
                                    std::set<std::string> const &              specialFunctions ) const;
  void        setVulkanLicenseHeader( int line, std::string const & comment );
  std::string toString( TypeCategory category );

private:
  std::map<std::string, BaseTypeData>  m_baseTypes;
  std::map<std::string, BitmaskData>   m_bitmasks;
  std::map<std::string, CommandData>   m_commands;
  std::map<std::string, std::string>   m_constants;
  std::set<std::string>                m_defines;
  std::map<std::string, EnumData>      m_enums;
  std::set<std::string>                m_extendedStructs;  // structs which are referenced by the structextends tag
  std::map<std::string, ExtensionData> m_extensions;
  std::map<int, std::map<std::string, ExtensionData>::const_iterator> m_extensionsByNumber;
  std::map<std::string, FeatureData>                                  m_features;
  std::map<std::string, FuncPointerData>                              m_funcPointers;
  std::map<std::string, HandleData>                                   m_handles;
  std::set<std::string>                                               m_includes;
  std::map<std::string, PlatformData>                                 m_platforms;
  std::set<std::string>                                               m_RAIISpecialFunctions;
  std::map<std::string, StructureAliasData>                           m_structureAliases;
  std::map<std::string, StructureData>                                m_structures;
  std::set<std::string>                                               m_tags;
  std::map<std::string, TypeData>                                     m_types;
  std::string                                                         m_typesafeCheck;
  std::string                                                         m_version;
  std::string                                                         m_vulkanLicenseHeader;
};

const size_t INVALID_INDEX = (size_t)~0;
