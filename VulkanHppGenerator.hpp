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
  std::string         generateFormatTraits() const;
  std::string         generateHandles() const;
  std::string         generateHandleHashStructures() const;
  std::string         generateIndexTypeTraits() const;
  std::string         generateRAIICommandDefinitions() const;
  std::string         generateRAIIDispatchers() const;
  std::string         generateRAIIHandles() const;
  std::string         generateResultExceptions() const;
  std::string         generateStructExtendsStructs() const;
  std::string         generateStructForwardDeclarations() const;
  std::string         generateStructHashStructures() const;
  std::string         generateStructs() const;
  std::string         generateThrowResultException() const;
  std::string const & getTypesafeCheck() const;
  std::string const & getVersion() const;
  std::string const & getVulkanLicenseHeader() const;
  void                prepareRAIIHandles();

private:
  struct TypeInfo
  {
    std::string compose( std::string const & nameSpace ) const;

    bool operator==( TypeInfo const & rhs ) const
    {
      return ( prefix == rhs.prefix ) && ( type == rhs.type ) && ( postfix == rhs.postfix );
    }

    bool operator!=( TypeInfo const & rhs ) const
    {
      return !operator==( rhs );
    }

    bool operator<( TypeInfo const & rhs ) const
    {
      return ( prefix < rhs.prefix ) || ( ( prefix == rhs.prefix ) && ( ( type < rhs.type ) || ( ( type == rhs.type ) && ( postfix < rhs.postfix ) ) ) );
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
    BitmaskData( std::string const & r, std::string const & t, int line ) : requirements( r ), type( t ), xmlLine( line ) {}

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
    EnumValueData( int line, std::string const & name_, std::string const & protect_, std::string const & extension_, bool singleBit_ )
      : name( name_ ), extension( extension_ ), protect( protect_ ), singleBit( singleBit_ ), xmlLine( line )
    {
    }

    std::string name;
    std::string extension;
    std::string protect;
    bool        singleBit;
    int         xmlLine;
  };

  struct EnumData
  {
    EnumData( int line, bool isBitmask_ = false ) : isBitmask( isBitmask_ ), xmlLine( line ) {}
    void addEnumAlias( int line, std::string const & name, std::string const & alias );
    void addEnumValue( int line, std::string const & valueName, std::string const & protect, bool bitpos, std::string const & extension );

    std::string                          alias;    // alias for this enum
    std::map<std::string, EnumAliasData> aliases;  // aliases for the values
    std::string                          bitwidth;
    bool                                 isBitmask;
    std::vector<EnumValueData>           values;
    int                                  xmlLine;
  };

  struct RequireData
  {
    RequireData( int line, std::string const & title_ ) : title( title_ ), xmlLine( line ) {}

    std::string              title;
    std::vector<std::string> commands;
    std::vector<std::string> types;
    int                      xmlLine;
  };

  struct FeatureData
  {
    FeatureData( std::string const & number_ ) : number( number_ ) {}

    std::string              number;
    std::vector<RequireData> requireData;
  };

  struct ExtensionData
  {
    ExtensionData( int                 line,
                   std::string const & deprecatedBy_,
                   std::string const & number_,
                   std::string const & obsoletedBy_,
                   std::string const & platform_,
                   std::string const & promotedTo_ )
      : deprecatedBy( deprecatedBy_ ), number( number_ ), obsoletedBy( obsoletedBy_ ), platform( platform_ ), promotedTo( promotedTo_ ), xmlLine( line )
    {
    }

    std::string              deprecatedBy;
    std::string              number;
    std::string              obsoletedBy;
    std::string              platform;
    std::string              promotedTo;
    std::set<std::string>    requiresAttribute;
    std::vector<RequireData> requireData;
    int                      xmlLine;
  };

  struct ComponentData
  {
    ComponentData( int line ) : xmlLine( line ) {}

    std::string bits;
    std::string name;
    std::string numericFormat;
    std::string planeIndex;
    int         xmlLine;
  };

  struct PlaneData
  {
    PlaneData( int line ) : xmlLine( line ) {}

    std::string compatible;
    std::string heightDivisor;
    std::string widthDivisor;
    int         xmlLine;
  };

  struct FormatData
  {
    FormatData( int line ) : xmlLine( line ) {}

    std::string                blockExtent;
    std::string                blockSize;
    std::string                chroma;
    std::string                classAttribute;
    std::vector<ComponentData> components;
    std::string                compressed;
    std::string                packed;
    std::vector<PlaneData>     planes;
    std::string                spirvImageFormat;
    std::string                texelsPerBlock;
    int                        xmlLine;
  };

  struct FuncPointerArgumentData
  {
    FuncPointerArgumentData( std::string const & t, int line ) : type( t ), xmlLine( line ) {}

    std::string type;
    int         xmlLine;
  };

  struct FuncPointerData
  {
    FuncPointerData( std::string const & r, int line ) : requirements( r ), xmlLine( line ) {}

    std::vector<FuncPointerArgumentData> arguments;
    std::string                          requirements;
    int                                  xmlLine;
  };

  struct HandleData
  {
    HandleData( std::string const & p, std::string const & objType, int line ) : objTypeEnum( objType ), parent( p ), xmlLine( line ) {}

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
    std::string              value;
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
    TypeData( TypeCategory category_, std::string const & referencedIn_ = "" ) : category( category_ ), referencedIn( referencedIn_ ) {}

    TypeCategory category;
    std::string  referencedIn;
  };

private:
  void        addCommand( std::string const & name, CommandData & commandData );
  void        addMissingFlagBits( std::vector<RequireData> & requireData, std::string const & referencedIn );
  std::string addTitleAndProtection( std::string const & title, std::string const & strIf, std::string const & strElse = {} ) const;
  bool        allVectorSizesSupported( std::vector<ParamData> const & params, std::map<size_t, size_t> const & vectorParams ) const;
  void        appendDispatchLoaderDynamicCommands( std::vector<RequireData> const & requireData,
                                                   std::set<std::string> &          listedCommands,
                                                   std::string const &              title,
                                                   std::string &                    commandMembers,
                                                   std::string &                    initialCommandAssignments,
                                                   std::string &                    instanceCommandAssignments,
                                                   std::string &                    deviceCommandAssignments ) const;
  void        appendRAIIDispatcherCommands( std::vector<RequireData> const & requireData,
                                            std::set<std::string> &          listedCommands,
                                            std::string const &              title,
                                            std::string &                    contextInitializers,
                                            std::string &                    contextMembers,
                                            std::string &                    deviceAssignments,
                                            std::string &                    deviceMembers,
                                            std::string &                    instanceAssignments,
                                            std::string &                    instanceMembers ) const;
  void        checkBitmaskCorrectness() const;
  void        checkCommandCorrectness() const;
  void        checkCorrectness() const;
  void        checkEnumCorrectness() const;
  void        checkEnumCorrectness( std::vector<RequireData> const & requireData ) const;
  bool        checkEquivalentSingularConstructor( std::vector<std::map<std::string, CommandData>::const_iterator> const & constructorIts,
                                                  std::map<std::string, CommandData>::const_iterator                      constructorIt,
                                                  std::vector<ParamData>::const_iterator                                  lenIt ) const;
  void        checkExtensionCorrectness() const;
  void        checkFuncPointerCorrectness() const;
  void        checkHandleCorrectness() const;
  void        checkStructCorrectness() const;
  void checkStructMemberCorrectness( std::string const & structureName, std::vector<MemberData> const & members, std::set<std::string> & sTypeValues ) const;
  bool containsArray( std::string const & type ) const;
  bool containsFloatingPoints( std::vector<MemberData> const & members ) const;
  bool containsUnion( std::string const & type ) const;
  std::vector<size_t> determineConstPointerParams( std::vector<ParamData> const & params ) const;
  size_t              determineDefaultStartIndex( std::vector<ParamData> const & params, std::set<size_t> const & skippedParams ) const;
  size_t              determineInitialSkipCount( std::string const & command ) const;
  std::vector<size_t> determineReturnParamIndices( std::vector<ParamData> const & params ) const;
  std::vector<std::map<std::string, CommandData>::const_iterator>
    determineRAIIHandleConstructors( std::string const & handleType, std::map<std::string, CommandData>::const_iterator destructorIt ) const;
  std::map<std::string, CommandData>::const_iterator determineRAIIHandleDestructor( std::string const & handleType ) const;
  std::set<size_t>                                   determineSkippedParams( std::vector<ParamData> const &   params,
                                                                             size_t                           initialSkipCount,
                                                                             std::map<size_t, size_t> const & vectorParamIndices,
                                                                             std::vector<size_t> const &      returnParam,
                                                                             bool                             singular ) const;
  std::string                                        determineSubStruct( std::pair<std::string, StructureData> const & structure ) const;
  std::map<size_t, size_t>                           determineVectorParams( std::vector<ParamData> const & params ) const;
  std::set<size_t>                                   determineVoidPointerParams( std::vector<ParamData> const & params ) const;
  void                                               distributeSecondLevelCommands( std::set<std::string> const & specialFunctions );
  std::string                                        findBaseName( std::string aliasName, std::map<std::string, EnumAliasData> const & aliases ) const;
  std::vector<MemberData>::const_iterator            findStructMemberIt( std::string const & name, std::vector<MemberData> const & memberData ) const;
  std::vector<MemberData>::const_iterator            findStructMemberItByType( std::string const & type, std::vector<MemberData> const & memberData ) const;
  std::string                                        generateArgumentListEnhanced( std::vector<ParamData> const & params,
                                                                                   std::set<size_t> const &       skippedParams,
                                                                                   std::set<size_t> const &       singularParams,
                                                                                   std::set<size_t> const &       templatedParams,
                                                                                   bool                           definition,
                                                                                   bool                           withAllocators,
                                                                                   bool                           structureChain,
                                                                                   bool                           withDispatcher ) const;
  std::string generateArgumentListStandard( std::vector<ParamData> const & params, std::set<size_t> const & skippedParams ) const;
  std::string generateArgumentTemplates( std::vector<ParamData> const & params, std::set<size_t> const & templatedParams, bool complete ) const;
  std::string generateBitmask( std::map<std::string, BitmaskData>::const_iterator bitmaskIt ) const;
  std::string generateBitmasks( std::vector<RequireData> const & requireData, std::set<std::string> & listedBitmasks, std::string const & title ) const;
  std::string generateCallArgumentsEnhanced( CommandData const &      commandData,
                                             size_t                   initialSkipCount,
                                             bool                     nonConstPointerAsNullptr,
                                             std::set<size_t> const & singularParams,
                                             std::set<size_t> const & templatedParams,
                                             bool                     raiiHandleMemberFunction ) const;
  std::string generateCallArgumentsRAIIFactory( std::vector<ParamData> const & params,
                                                size_t                         initialSkipCount,
                                                std::set<size_t> const &       skippedParams,
                                                std::set<size_t> const &       singularParams ) const;
  std::string generateCallArgumentsStandard( std::string const & handle, std::vector<ParamData> const & params ) const;
  std::string generateCallArgumentEnhanced( std::vector<ParamData> const & params,
                                            size_t                         paramIndex,
                                            bool                           nonConstPointerAsNullptr,
                                            std::set<size_t> const &       singularParams,
                                            std::set<size_t> const &       templatedParams,
                                            bool                           raiiHandleMemberFunction ) const;
  std::string generateCallArgumentEnhancedConstPointer( ParamData const &        param,
                                                        size_t                   paramIndex,
                                                        std::set<size_t> const & singularParams,
                                                        std::set<size_t> const & templatedParams ) const;
  std::string generateCallArgumentEnhancedNonConstPointer(
    ParamData const & param, size_t paramIndex, bool nonConstPointerAsNullptr, std::set<size_t> const & singularParams, bool raiiHandleMemberFunction ) const;
  std::string generateCallArgumentEnhancedValue( std::vector<ParamData> const & params, size_t paramIndex, std::set<size_t> const & singularParams ) const;
  std::string generateCommand( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const;
  std::string
    generateCommandDefinitions( std::vector<RequireData> const & requireData, std::set<std::string> & listedCommands, std::string const & title ) const;
  std::string generateCommandDefinitions( std::string const & command, std::string const & handle ) const;
  std::string generateCommandName( std::string const &            vulkanCommandName,
                                   std::vector<ParamData> const & params,
                                   size_t                         initialSkipCount,
                                   std::set<std::string> const &  tags ) const;
  std::string generateCommandResult( std::string const &              name,
                                     CommandData const &              commandData,
                                     size_t                           initialSkipCount,
                                     bool                             definition,
                                     std::map<size_t, size_t> const & vectorParamIndices,
                                     std::vector<size_t> const &      returnParams,
                                     bool                             singular ) const;
  std::string generateCommandResultEnumerate( std::string const &               name,
                                              CommandData const &               commandData,
                                              size_t                            initialSkipCount,
                                              bool                              definition,
                                              std::pair<size_t, size_t> const & vectorParam,
                                              bool                              withAllocators ) const;
  std::string generateCommandResultEnumerateChained( std::string const &               name,
                                                     CommandData const &               commandData,
                                                     size_t                            initialSkipCount,
                                                     bool                              definition,
                                                     std::pair<size_t, size_t> const & vectorParamIndices,
                                                     bool                              withAllocators ) const;
  std::string generateCommandResultEnumerateTwoVectors( std::string const &              name,
                                                        CommandData const &              commandData,
                                                        size_t                           initialSkipCount,
                                                        bool                             definition,
                                                        std::map<size_t, size_t> const & vectorParamIndices,
                                                        std::vector<size_t> const &      returnParamIndices,
                                                        bool                             withAllocators ) const;
  std::string generateCommandResultGetChain(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const;
  std::string generateCommandResultGetHandleUnique(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const;
  std::string generateCommandResultGetSingularAndValue( std::string const &              name,
                                                        CommandData const &              commandData,
                                                        size_t                           initialSkipCount,
                                                        bool                             definition,
                                                        std::vector<size_t> const &      returnParamIndices,
                                                        std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string generateCommandResultGetVector( std::string const &              name,
                                              CommandData const &              commandData,
                                              size_t                           initialSkipCount,
                                              bool                             definition,
                                              std::map<size_t, size_t> const & vectorParamIndices,
                                              size_t                           returnParam ) const;
  std::string generateCommandResultGetVectorAndValue( std::string const &              name,
                                                      CommandData const &              commandData,
                                                      size_t                           initialSkipCount,
                                                      bool                             definition,
                                                      std::map<size_t, size_t> const & vectorParamIndices,
                                                      std::vector<size_t> const &      returnParam,
                                                      bool                             withAllocator ) const;
  std::string generateCommandResultGetVectorOfHandlesOrValues( std::string const &              name,
                                                               CommandData const &              commandData,
                                                               size_t                           initialSkipCount,
                                                               bool                             definition,
                                                               std::map<size_t, size_t> const & vectorParamIndices,
                                                               size_t                           returnParam,
                                                               bool                             withAllocator ) const;
  std::string generateCommandResultGetVectorOfHandlesUnique( std::string const &              name,
                                                             CommandData const &              commandData,
                                                             size_t                           initialSkipCount,
                                                             bool                             definition,
                                                             std::map<size_t, size_t> const & vectorParamIndices,
                                                             size_t                           returnParam,
                                                             bool                             withAllocator ) const;
  std::string generateCommandResultGetVectorOfHandlesUniqueSingular( std::string const &              name,
                                                                     CommandData const &              commandData,
                                                                     size_t                           initialSkipCount,
                                                                     bool                             definition,
                                                                     std::map<size_t, size_t> const & vectorParamIndices,
                                                                     size_t                           returnParam ) const;
  std::string
    generateCommandResultMultiSuccessNoErrors( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const;
  std::string
    generateCommandResultMultiSuccessWithErrors( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const;
  std::string generateCommandResultMultiSuccessWithErrors1Return(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const;
  std::string generateCommandResultMultiSuccessWithErrors2Return(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, std::vector<size_t> const & returnParamIndices ) const;
  std::string generateCommandResultMultiSuccessWithErrors3Return(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, std::vector<size_t> const & returnParamIndices ) const;
  std::string
    generateCommandResultSingleSuccessNoErrors( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const;
  std::string
    generateCommandResultSingleSuccessWithErrors( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const;
  std::string generateCommandResultSingleSuccessWithErrors1Return(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const;
  std::string generateCommandResultSingleSuccessWithErrors1ReturnChain(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const;
  std::string generateCommandResultSingleSuccessWithErrors1ReturnHandle(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const;
  std::string generateCommandResultSingleSuccessWithErrors1ReturnHandle1Vector( std::string const &               name,
                                                                                CommandData const &               commandData,
                                                                                size_t                            initialSkipCount,
                                                                                bool                              definition,
                                                                                size_t                            returnParam,
                                                                                std::pair<size_t, size_t> const & vectorParamIndex ) const;
  std::string generateCommandResultSingleSuccessWithErrors1ReturnHandle2Vector( std::string const &              name,
                                                                                CommandData const &              commandData,
                                                                                size_t                           initialSkipCount,
                                                                                bool                             definition,
                                                                                size_t                           returnParam,
                                                                                std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string generateCommandResultSingleSuccessWithErrors1ReturnValue(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const;
  std::string generateCommandResultSingleSuccessWithErrors1ReturnValue2Vectors( std::string const &              name,
                                                                                CommandData const &              commandData,
                                                                                size_t                           initialSkipCount,
                                                                                bool                             definition,
                                                                                size_t                           returnParam,
                                                                                std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string generateCommandResultSingleSuccessWithErrors1ReturnVoid(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const;
  std::string generateCommandResultSingleSuccessWithErrors2Return(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, std::vector<size_t> const & returnParamIndices ) const;
  std::string
    generateCommandResultWithErrors0Return( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const;
  std::string generateCommandSetStandard( std::string const & standard ) const;
  std::string generateCommandSetStandardEnhanced( bool definition, std::string const & standard, std::string const & enhanced ) const;
  std::string generateCommandSetStandardEnhancedChained( bool                definition,
                                                         std::string const & standard,
                                                         std::string const & enhanced,
                                                         std::string const & enhancedChained ) const;
  std::string generateCommandSetStandardEnhancedSingular( bool                definition,
                                                          std::string const & standard,
                                                          std::string const & enhanced,
                                                          std::string const & enhancedSingular ) const;
  std::string generateCommandSetStandardEnhancedUnique( bool                definition,
                                                        std::string const & standard,
                                                        std::string const & enhanced,
                                                        std::string const & enhancedUnique ) const;
  std::string generateCommandSetStandardEnhancedWithAllocator( bool                definition,
                                                               std::string const & standard,
                                                               std::string const & enhanced,
                                                               std::string const & enhancedWithAllocator ) const;
  std::string generateCommandSetStandardEnhancedWithAllocatorChained( bool                definition,
                                                                      std::string const & standard,
                                                                      std::string const & enhanced,
                                                                      std::string const & enhancedWithAllocator,
                                                                      std::string const & enhancedChained,
                                                                      std::string const & enhancedChainedWithAllocator ) const;
  std::string generateCommandSetStandardEnhancedWithAllocatorSingular( bool                definition,
                                                                       std::string const & standard,
                                                                       std::string const & enhanced,
                                                                       std::string const & enhancedWithAllocator,
                                                                       std::string const & enhancedSingular ) const;
  std::string generateCommandSetStandardEnhancedWithAllocatorSingularUnique( bool                definition,
                                                                             std::string const & standard,
                                                                             std::string const & enhanced,
                                                                             std::string const & enhancedWithAllocator,
                                                                             std::string const & enhancedSingular,
                                                                             std::string const & enhancedUnique,
                                                                             std::string const & enhancedUniqueWithAllocator,
                                                                             std::string const & enhancedUniqueSingular ) const;
  std::string generateCommandSetStandardEnhancedWithAllocatorUnique( bool                definition,
                                                                     std::string const & standard,
                                                                     std::string const & enhanced,
                                                                     std::string const & enhancedWithAllocator,
                                                                     std::string const & enhancedUnique,
                                                                     std::string const & enhancedUniqueWithAllocator ) const;
  std::string generateCommandSetStandardOrEnhanced( std::string const & standard, std::string const & enhanced ) const;
  std::string generateCommandSingle( std::string const &              name,
                                     CommandData const &              commandData,
                                     size_t                           initialSkipCount,
                                     bool                             definition,
                                     std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string generateCommandStandard( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const;
  std::string generateCommandValue( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const;
  std::string generateCommandVoid0Return( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const;
  std::string
    generateCommandVoid1Return( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam ) const;
  std::string generateCommandVoid2Return(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, std::vector<size_t> const & returnParamIndices ) const;
  std::string generateCommandVoidEnumerate( std::string const &               name,
                                            CommandData const &               commandData,
                                            size_t                            initialSkipCount,
                                            bool                              definition,
                                            std::pair<size_t, size_t> const & vectorParamIndex,
                                            std::vector<size_t> const &       returnParamIndices,
                                            bool                              withAllocators ) const;
  std::string generateCommandVoidEnumerateChained( std::string const &               name,
                                                   CommandData const &               commandData,
                                                   size_t                            initialSkipCount,
                                                   bool                              definition,
                                                   std::pair<size_t, size_t> const & vectorParamIndex,
                                                   std::vector<size_t> const &       returnParamIndices,
                                                   bool                              withAllocators ) const;
  std::string generateCommandVoidGetValue( std::string const &              name,
                                           CommandData const &              commandData,
                                           size_t                           initialSkipCount,
                                           bool                             definition,
                                           std::map<size_t, size_t> const & vectorParamIndices,
                                           size_t                           returnParam,
                                           bool                             chained ) const;
  std::string generateConstexprString( std::string const & structName ) const;
  std::string generateDestroyCommand( std::string const & name, CommandData const & commandData ) const;
  std::string
    generateDispatchLoaderDynamicCommandAssignment( std::string const & commandName, CommandData const & commandData, std::string const & firstArg ) const;
  std::string generateDispatchLoaderStaticCommands( std::vector<RequireData> const & requireData,
                                                    std::set<std::string> &          listedCommands,
                                                    std::string const &              title ) const;
  std::string generateEnhancedReturnType( CommandData const & commandData, size_t returnParam, bool isStructureChain ) const;
  std::string generateEnum( std::pair<std::string, EnumData> const & enumData ) const;
  std::string generateEnums( std::vector<RequireData> const & requireData, std::set<std::string> & listedEnums, std::string const & title ) const;
  std::string generateEnumInitializer( TypeInfo const &                   type,
                                       std::vector<std::string> const &   arraySizes,
                                       std::vector<EnumValueData> const & values,
                                       bool                               bitmask ) const;
  std::string generateEnumToString( std::pair<std::string, EnumData> const & enumData ) const;
  std::string generateFailureCheck( std::vector<std::string> const & successCodes ) const;
  std::string generateFunctionPointerCheck( std::string const & function, std::string const & referencedIn ) const;
  std::string generateHandle( std::pair<std::string, HandleData> const & handle, std::set<std::string> & listedHandles ) const;
  std::string generateHandleCommandDeclarations( std::set<std::string> const & commands ) const;
  std::string generateHandleDependencies( std::pair<std::string, HandleData> const & handle, std::set<std::string> & listedHandles ) const;
  std::string generateHandleEmpty( HandleData const & handleData ) const;
  std::string generateHandleHashStructures( std::vector<RequireData> const & requireData, std::string const & title ) const;
  std::string
                                      generateLenInitializer( std::vector<MemberData>::const_iterator                                                                                 mit,
                                                              std::map<std::vector<MemberData>::const_iterator, std::vector<std::vector<MemberData>::const_iterator>>::const_iterator litit,
                                                              bool mutualExclusiveLens ) const;
  std::string                         generateName( TypeInfo const & typeInfo ) const;
  std::pair<std::string, std::string> generateProtection( std::string const & referencedIn, std::string const & protect ) const;
  std::pair<std::string, std::string> generateProtection( std::string const & type, bool isAliased ) const;
  std::string
    generateRAIICommandDefinitions( std::vector<RequireData> const & requireData, std::set<std::string> & listedCommands, std::string const & title ) const;
  std::string generateRAIIHandle( std::pair<std::string, HandleData> const & handle,
                                  std::set<std::string> &                    listedHandles,
                                  std::set<std::string> const &              specialFunctions ) const;
  std::string generateRAIIHandleCommand( std::string const & command, size_t initialSkipCount, bool definition ) const;
  std::string generateRAIIHandleCommandDeclarations( std::pair<std::string, HandleData> const & handle, std::set<std::string> const & specialFunctions ) const;
  std::string generateRAIIHandleCommandFactory( std::map<std::string, CommandData>::const_iterator commandIt,
                                                size_t                                             initialSkipCount,
                                                size_t                                             returnParam,
                                                bool                                               definition ) const;
  std::string generateRAIIHandleCommandFactoryArgumentList( std::vector<ParamData> const & params,
                                                            std::set<size_t> const &       skippedParams,
                                                            bool                           definition,
                                                            bool                           singular ) const;
  std::string generateRAIIHandleCommandFactorySingular( std::map<std::string, CommandData>::const_iterator commandIt,
                                                        size_t                                             initialSkipCount,
                                                        std::vector<size_t> const &                        returnParamIndices,
                                                        std::map<size_t, size_t> const &                   vectorParamIndices,
                                                        bool                                               definition ) const;
  std::string generateRAIIHandleCommandFactoryVector( std::map<std::string, CommandData>::const_iterator commandIt,
                                                      size_t                                             initialSkipCount,
                                                      std::vector<size_t> const &                        returnParamIndices,
                                                      std::map<size_t, size_t> const &                   vectorParamIndices,
                                                      bool                                               definition ) const;
  std::string generateRAIIHandleCommandResult( std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount, bool definition ) const;
  std::string generateRAIIHandleCommandResultMultiSuccessNoErrors( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                   size_t                                             initialSkipCount,
                                                                   bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultMultiSuccessNoErrors0Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                          size_t                                             initialSkipCount,
                                                                          std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                          bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultMultiSuccessWithErrors( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                     size_t                                             initialSkipCount,
                                                                     bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultMultiSuccessWithErrors0Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                            size_t                                             initialSkipCount,
                                                                            std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                            bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultMultiSuccessWithErrors1Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                            size_t                                             initialSkipCount,
                                                                            bool                                               definition,
                                                                            size_t                                             returnParam ) const;
  std::string generateRAIIHandleCommandResultMultiSuccessWithErrors1ReturnValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                 size_t                                             initialSkipCount,
                                                                                 std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                                 std::vector<size_t> const &                        returnParamIndices,
                                                                                 bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultMultiSuccessWithErrors1ReturnVoidSingular( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                        size_t                                             initialSkipCount,
                                                                                        std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                                        std::vector<size_t> const &                        returnParamIndices,
                                                                                        bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultMultiSuccessWithErrors1ReturnVoidVector( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                      size_t                                             initialSkipCount,
                                                                                      std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                                      std::vector<size_t> const &                        returnParamIndices,
                                                                                      bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultMultiSuccessWithErrors2Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                            size_t                                             initialSkipCount,
                                                                            bool                                               definition,
                                                                            std::vector<size_t> const &                        returnParamIndices ) const;
  std::string generateRAIIHandleCommandResultMultiSuccessWithErrors2Return1VectorEnumerate( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                            size_t                                             initialSkipCount,
                                                                                            std::map<size_t, size_t> const & vectorParamIndices,
                                                                                            std::vector<size_t> const &      returnParamIndices,
                                                                                            bool                             definition ) const;
  std::string generateRAIIHandleCommandResultMultiSuccessWithErrors2Return1VectorEnumerateChain( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                                 size_t                           initialSkipCount,
                                                                                                 std::map<size_t, size_t> const & vectorParamIndices,
                                                                                                 std::vector<size_t> const &      returnParamIndices,
                                                                                                 bool                             definition ) const;
  std::string generateRAIIHandleCommandResultMultiSuccessWithErrors3Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                            size_t                                             initialSkipCount,
                                                                            bool                                               definition,
                                                                            std::vector<size_t> const &                        returnParamIndices ) const;
  std::string generateRAIIHandleCommandResultMultiSuccessWithErrors3Return2VectorEnumerate( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                            size_t                                             initialSkipCount,
                                                                                            std::map<size_t, size_t> const & vectorParamIndices,
                                                                                            std::vector<size_t> const &      returnParamIndices,
                                                                                            bool                             definition ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessNoErrors( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                    size_t                                             initialSkipCount,
                                                                    bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessNoErrors0Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                           size_t                                             initialSkipCount,
                                                                           std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                           bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessWithErrors( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                      size_t                                             initialSkipCount,
                                                                      bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessWithErrors0Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                             size_t                                             initialSkipCount,
                                                                             std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                             bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessWithErrors1ReturnChain( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                  size_t                                             initialSkipCount,
                                                                                  std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                                  std::vector<size_t> const &                        returnParamIndices,
                                                                                  bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessWithErrors1Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                             size_t                                             initialSkipCount,
                                                                             bool                                               definition,
                                                                             size_t                                             returnParam ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessWithErrors1ReturnValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                  size_t                                             initialSkipCount,
                                                                                  std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                                  size_t                                             returnParam,
                                                                                  bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessWithErrors1ReturnValueSingular( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                          size_t                                             initialSkipCount,
                                                                                          size_t                                             returnParam,
                                                                                          std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                                          bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessWithErrors1ReturnValueVector( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                        size_t                                             initialSkipCount,
                                                                                        size_t                                             returnParam,
                                                                                        std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                                        bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessWithErrors1ReturnVoidSingular( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                         size_t                                             initialSkipCount,
                                                                                         std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                                         size_t                                             returnParam,
                                                                                         bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessWithErrors1ReturnVoidVector( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                       size_t                                             initialSkipCount,
                                                                                       std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                                       size_t                                             returnParam,
                                                                                       bool                                               definition ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessWithErrors2Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                             size_t                                             initialSkipCount,
                                                                             bool                                               definition,
                                                                             std::vector<size_t> const &                        returnParamIndices ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessWithErrors2ReturnValueSingularValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                               size_t                           initialSkipCount,
                                                                                               bool                             definition,
                                                                                               std::vector<size_t> const &      returnParamIndices,
                                                                                               std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string generateRAIIHandleCommandResultSingleSuccessWithErrors2ReturnValueVectorValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                             size_t                           initialSkipCount,
                                                                                             std::map<size_t, size_t> const & vectorParamIndices,
                                                                                             std::vector<size_t> const &      returnParamIndices,
                                                                                             bool                             definition ) const;
  std::string generateRAIIHandleCommandValue( std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount, bool definition ) const;
  std::string generateRAIIHandleCommandValue0Return0VectorType( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                size_t                                             initialSkipCount,
                                                                std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                bool                                               definition ) const;
  std::string generateRAIIHandleCommandValue0Return0VectorVkType( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                  size_t                                             initialSkipCount,
                                                                  std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                  bool                                               definition ) const;
  std::string generateRAIIHandleCommandVoid( std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount, bool definition ) const;
  std::string
    generateRAIIHandleCommandVoid0Return( std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount, bool definition ) const;
  std::string                         generateRAIIHandleCommandVoid1ReturnChain( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                 size_t                                             initialSkipCount,
                                                                                 std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                                 size_t                                             returnParam,
                                                                                 bool                                               definition ) const;
  std::string                         generateRAIIHandleCommandVoid1ReturnValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                 size_t                                             initialSkipCount,
                                                                                 std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                                 size_t                                             returnParam,
                                                                                 bool                                               definition ) const;
  std::string                         generateRAIIHandleCommandVoid2ReturnEnumerateChain( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                          size_t                                             initialSkipCount,
                                                                                          std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                                          std::vector<size_t> const &                        returnParamIndices,
                                                                                          bool                                               definition ) const;
  std::string                         generateRAIIHandleCommandVoid2ReturnEnumerateValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                                                          size_t                                             initialSkipCount,
                                                                                          std::map<size_t, size_t> const &                   vectorParamIndices,
                                                                                          std::vector<size_t> const &                        returnParamIndices,
                                                                                          bool                                               definition ) const;
  std::pair<std::string, std::string> generateRAIIHandleConstructor( std::pair<std::string, HandleData> const &         handle,
                                                                     std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                     std::string const &                                enter,
                                                                     std::string const &                                leave ) const;
  std::pair<std::string, std::string> generateRAIIHandleConstructor1Return2Vector( std::pair<std::string, HandleData> const &         handle,
                                                                                   std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                                   std::string const &                                enter,
                                                                                   std::string const &                                leave,
                                                                                   size_t                                             returnParam,
                                                                                   std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::pair<std::string, std::string> generateRAIIHandleConstructors( std::pair<std::string, HandleData> const & handle ) const;
  std::string generateRAIIHandleConstructorArgument( ParamData const & param, bool definition, bool singular, bool takesOwnership ) const;
  std::string generateRAIIHandleConstructorArguments( std::pair<std::string, HandleData> const &                             handle,
                                                      std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
                                                      bool                                                                   singular,
                                                      bool                                                                   takesOwnership ) const;
  std::string generateRAIIHandleConstructorCallArguments( std::pair<std::string, HandleData> const &                             handle,
                                                          std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
                                                          bool                                                                   nonConstPointerAsNullptr,
                                                          std::set<size_t> const &                                               singularParams,
                                                          bool allocatorIsMemberVariable ) const;
  std::string generateRAIIHandleConstructorEnumerate( std::pair<std::string, HandleData> const &         handle,
                                                      std::map<std::string, CommandData>::const_iterator constructorIt,
                                                      std::vector<ParamData>::const_iterator             handleParamIt,
                                                      std::vector<ParamData>::const_iterator             lenParamIt,
                                                      std::string const &                                enter,
                                                      std::string const &                                leave ) const;
  std::string generateRAIIHandleConstructorInitializationList( std::pair<std::string, HandleData> const &         handle,
                                                               std::map<std::string, CommandData>::const_iterator constructorIt,
                                                               std::map<std::string, CommandData>::const_iterator destructorIt,
                                                               bool                                               takesOwnership ) const;
  std::string generateRAIIHandleConstructorParamName( std::string const & type, std::map<std::string, CommandData>::const_iterator destructorIt ) const;
  std::pair<std::string, std::string> generateRAIIHandleConstructorResult( std::pair<std::string, HandleData> const &         handle,
                                                                           std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                           std::string const &                                enter,
                                                                           std::string const &                                leave ) const;
  std::string generateRAIIHandleConstructorResultSingleSuccessWithErrors1Return0Vector( std::pair<std::string, HandleData> const &         handle,
                                                                                        std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                                        std::string const &                                enter,
                                                                                        std::string const &                                leave ) const;
  std::string generateRAIIHandleConstructorTakeOwnership( std::pair<std::string, HandleData> const & handle ) const;
  std::string generateRAIIHandleConstructorVector( std::pair<std::string, HandleData> const &         handle,
                                                   std::map<std::string, CommandData>::const_iterator constructorIt,
                                                   std::vector<ParamData>::const_iterator             handleParamIt,
                                                   std::string const &                                enter,
                                                   std::string const &                                leave ) const;
  std::string generateRAIIHandleConstructorVectorSingular( std::pair<std::string, HandleData> const &         handle,
                                                           std::map<std::string, CommandData>::const_iterator constructorIt,
                                                           std::vector<ParamData>::const_iterator             handleParamIt,
                                                           std::string const &                                enter,
                                                           std::string const &                                leave ) const;
  std::pair<std::string, std::string> generateRAIIHandleConstructorVoid( std::pair<std::string, HandleData> const &         handle,
                                                                         std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                         std::string const &                                enter,
                                                                         std::string const &                                leave ) const;
  std::string                         generateRAIIHandleConstructorVoid1Return0Vector( std::pair<std::string, HandleData> const &         handle,
                                                                                       std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                                       std::string const &                                enter,
                                                                                       std::string const &                                leave ) const;
  std::string generateRAIIHandleContext( std::pair<std::string, HandleData> const & handle, std::set<std::string> const & specialFunctions ) const;
  std::string generateRAIIHandleDestructorCallArguments( std::string const &                                handleType,
                                                         std::map<std::string, CommandData>::const_iterator destructorIt ) const;
  std::tuple<std::string, std::string, std::string, std::string, std::string, std::string>
              generateRAIIHandleDetails( std::pair<std::string, HandleData> const & handle ) const;
  std::string generateRAIIHandleForwardDeclarations( std::vector<RequireData> const & requireData, std::string const & title ) const;
  std::string generateRAIIHandleSingularConstructorArguments( std::pair<std::string, HandleData> const &         handle,
                                                              std::map<std::string, CommandData>::const_iterator constructorIt ) const;
  std::string generateRAIIHandleVectorSizeCheck( std::string const &                           name,
                                                 CommandData const &                           commandData,
                                                 size_t                                        initialSkipCount,
                                                 std::map<size_t, std::vector<size_t>> const & countToVectorMap,
                                                 std::set<size_t> const &                      skippedParams ) const;
  std::string
    generateSizeCheck( std::vector<std::vector<MemberData>::const_iterator> const & arrayIts, std::string const & structName, bool mutualExclusiveLens ) const;
  std::string generateStruct( std::pair<std::string, StructureData> const & structure, std::set<std::string> & listedStructs ) const;
  std::string generateStructAssignmentOperators( std::pair<std::string, StructureData> const & structure ) const;
  std::string generateStructCompareOperators( std::pair<std::string, StructureData> const & structure ) const;
  std::string generateStructConstructors( std::pair<std::string, StructureData> const & structData ) const;
  std::string generateStructConstructorsEnhanced( std::pair<std::string, StructureData> const & structData ) const;
  std::string generateStructConstructorArgument( bool listedArgument, MemberData const & memberData, bool withDefault ) const;
  std::string generateStructHashStructure( std::pair<std::string, StructureData> const & structure, std::set<std::string> & listedStructs ) const;
  std::string generateStructHashSum( std::string const & structName, std::vector<MemberData> const & members ) const;
  std::string generateStructure( std::pair<std::string, StructureData> const & structure ) const;
  std::string
    generateStructExtendsStructs( std::vector<RequireData> const & requireData, std::set<std::string> & listedStructs, std::string const & title ) const;
  std::string generateStructForwardDeclarations( std::vector<RequireData> const & requireData, std::string const & title ) const;
  std::tuple<std::string, std::string, std::string, std::string> generateStructMembers( std::pair<std::string, StructureData> const & structData ) const;
  std::string                         generateStructSetter( std::string const & structureName, std::vector<MemberData> const & memberData, size_t index ) const;
  std::string                         generateStructSubConstructor( std::pair<std::string, StructureData> const & structData ) const;
  std::string                         generateSuccessCheck( std::vector<std::string> const & successCodes ) const;
  std::string                         generateSuccessCodeList( std::vector<std::string> const & successCodes ) const;
  std::string                         generateUnion( std::pair<std::string, StructureData> const & structure ) const;
  std::string                         generateUniqueTypes( std::string const & parentType, std::set<std::string> const & childrenTypes ) const;
  std::string                         generateVectorSizeCheck( std::string const &                           name,
                                                               CommandData const &                           commandData,
                                                               size_t                                        initialSkipCount,
                                                               std::map<size_t, std::vector<size_t>> const & countToVectorMap,
                                                               std::set<size_t> const &                      skippedParams,
                                                               bool                                          onlyThrows ) const;
  std::pair<std::string, std::string> getParentTypeAndName( std::pair<std::string, HandleData> const & handle ) const;
  std::string                         getPlatform( std::string const & title ) const;
  std::pair<std::string, std::string> getPoolTypeAndName( std::string const & type ) const;
  std::string getVectorSize( std::vector<ParamData> const & params, std::map<size_t, size_t> const & vectorParamIndices, size_t returnParam ) const;
  bool        hasParentHandle( std::string const & handle, std::string const & parent ) const;
  bool        isDeviceCommand( CommandData const & commandData ) const;
  bool        isHandleType( std::string const & type ) const;
  bool        isLenByStructMember( std::string const & name, std::vector<ParamData> const & params ) const;
  bool        isLenByStructMember( std::string const & name, ParamData const & param ) const;
  bool        isMultiSuccessCodeConstructor( std::vector<std::map<std::string, CommandData>::const_iterator> const & constructorIts ) const;
  bool        isParam( std::string const & name, std::vector<ParamData> const & params ) const;
  bool        isStructMember( std::string const & name, std::vector<MemberData> const & memberData ) const;
  bool        isStructureChainAnchor( std::string const & type ) const;
  std::pair<bool, std::map<size_t, std::vector<size_t>>> needsVectorSizeCheck( std::vector<ParamData> const &   params,
                                                                               std::map<size_t, size_t> const & vectorParamIndices,
                                                                               std::set<size_t> const &         singularParams ) const;
  void                                                   readCommands( tinyxml2::XMLElement const * element );
  void                                                   readCommandsCommand( tinyxml2::XMLElement const * element );
  ParamData                           readCommandsCommandParam( tinyxml2::XMLElement const * element, std::vector<ParamData> const & params );
  std::pair<std::string, std::string> readCommandsCommandProto( tinyxml2::XMLElement const * element );
  std::string                         readComment( tinyxml2::XMLElement const * element );
  void                                readEnums( tinyxml2::XMLElement const * element );
  void                                readEnumsConstant( tinyxml2::XMLElement const * element );
  void                                readEnumsEnum( tinyxml2::XMLElement const * element, std::map<std::string, EnumData>::iterator enumIt );
  void                                readExtensions( tinyxml2::XMLElement const * element );
  void                                readExtensionsExtension( tinyxml2::XMLElement const * element );
  void                                readExtensionsExtensionDisabledRequire( tinyxml2::XMLElement const * element );
  void                                readExtensionsExtensionDisabledRequireCommand( tinyxml2::XMLElement const * element );
  void                                readExtensionsExtensionDisabledRequireType( tinyxml2::XMLElement const * element );
  void readExtensionsExtensionRequire( tinyxml2::XMLElement const * element, std::map<std::string, ExtensionData>::iterator extensionIt );
  void readExtensionsExtensionRequireCommand( tinyxml2::XMLElement const * element, std::string const & extensionName, RequireData & requireData );
  void readExtensionsExtensionRequireType( tinyxml2::XMLElement const * element, std::string const & extensionName, RequireData & requireData );
  void readFeature( tinyxml2::XMLElement const * element );
  void readFeatureRequire( tinyxml2::XMLElement const * element, std::map<std::string, FeatureData>::iterator featureIt );
  void readFeatureRequireCommand( tinyxml2::XMLElement const * element, std::map<std::string, FeatureData>::iterator featureIt, RequireData & requireData );
  void readFeatureRequireType( tinyxml2::XMLElement const * element, std::map<std::string, FeatureData>::iterator featureIt, RequireData & requireData );
  void readFormats( tinyxml2::XMLElement const * element );
  void readFormatsFormat( tinyxml2::XMLElement const * element );
  void readFormatsFormatComponent( tinyxml2::XMLElement const * element, FormatData & formatData );
  void readFormatsFormatPlane( tinyxml2::XMLElement const * element, FormatData & formatData );
  void readFormatsFormatSPIRVImageFormat( tinyxml2::XMLElement const * element, FormatData & formatData );
  std::pair<NameData, TypeInfo> readNameAndType( tinyxml2::XMLElement const * elements );
  void                          readPlatforms( tinyxml2::XMLElement const * element );
  void                          readPlatformsPlatform( tinyxml2::XMLElement const * element );
  void                          readRegistry( tinyxml2::XMLElement const * element );
  void                          readRequireEnum( tinyxml2::XMLElement const * element, std::string const & extensionName );
  void                          readSPIRVCapabilities( tinyxml2::XMLElement const * element );
  void                          readSPIRVCapabilitiesSPIRVCapability( tinyxml2::XMLElement const * element );
  void                          readSPIRVCapabilitiesSPIRVCapabilityEnable( tinyxml2::XMLElement const * element );
  void                          readSPIRVCapabilitiesSPIRVCapabilityEnableExtension( int xmlLine, std::map<std::string, std::string> const & attributes );
  void                          readSPIRVCapabilitiesSPIRVCapabilityEnableProperty( int xmlLine, std::map<std::string, std::string> const & attributes );
  void                          readSPIRVCapabilitiesSPIRVCapabilityEnableStruct( int xmlLine, std::map<std::string, std::string> const & attributes );
  void                          readSPIRVCapabilitiesSPIRVCapabilityEnableVersion( int xmlLine, std::map<std::string, std::string> const & attributes );
  void                          readSPIRVExtensions( tinyxml2::XMLElement const * element );
  void                          readSPIRVExtensionsExtension( tinyxml2::XMLElement const * element );
  void                          readSPIRVExtensionsExtensionEnable( tinyxml2::XMLElement const * element );
  void                          readTags( tinyxml2::XMLElement const * element );
  void                          readTagsTag( tinyxml2::XMLElement const * element );
  void                          readTypes( tinyxml2::XMLElement const * element );
  void                          readTypesType( tinyxml2::XMLElement const * element );
  void                          readTypesTypeBasetype( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void                          readTypesTypeBitmask( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void                          readTypesTypeDefine( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void                          readTypesTypeEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void                          readTypesTypeFuncpointer( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void                          readTypesTypeHandle( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void                          readTypesTypeInclude( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void                          readTypesTypeRequires( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void     readTypesTypeStruct( tinyxml2::XMLElement const * element, bool isUnion, std::map<std::string, std::string> const & attributes );
  void     readTypesTypeStructMember( tinyxml2::XMLElement const * element, std::vector<MemberData> & members, bool isUnion );
  void     readTypesTypeStructMemberEnum( tinyxml2::XMLElement const * element, MemberData & memberData );
  void     readTypesTypeStructMemberName( tinyxml2::XMLElement const * element, MemberData & memberData, std::vector<MemberData> const & members );
  void     readTypesTypeStructMemberType( tinyxml2::XMLElement const * element, MemberData & memberData );
  TypeInfo readTypeInfo( tinyxml2::XMLElement const * element ) const;
  void     registerDeleter( std::string const & name, std::pair<std::string, CommandData> const & commandData );
  void     rescheduleRAIIHandle( std::string &                              str,
                                 std::pair<std::string, HandleData> const & handle,
                                 std::set<std::string> &                    listedHandles,
                                 std::set<std::string> const &              specialFunctions ) const;
  std::vector<std::string> selectCommandsByHandle( std::vector<RequireData> const & requireData,
                                                   std::set<std::string> const &    handleCommands,
                                                   std::set<std::string> &          listedCommands ) const;
  void                     setVulkanLicenseHeader( int line, std::string const & comment );
  bool                     skipLeadingGrandParent( std::pair<std::string, HandleData> const & handle ) const;
  std::string              toString( TypeCategory category );

private:
  std::map<std::string, BaseTypeData>                                 m_baseTypes;
  std::map<std::string, BitmaskData>                                  m_bitmasks;
  std::map<std::string, CommandData>                                  m_commands;
  std::map<std::string, std::string>                                  m_constants;
  std::set<std::string>                                               m_defines;
  std::map<std::string, EnumData>                                     m_enums;
  std::set<std::string>                                               m_extendedStructs;  // structs which are referenced by the structextends tag
  std::map<std::string, ExtensionData>                                m_extensions;
  std::map<int, std::map<std::string, ExtensionData>::const_iterator> m_extensionsByNumber;
  std::map<std::string, FeatureData>                                  m_features;
  std::map<std::string, FormatData>                                   m_formats;
  std::map<std::string, FuncPointerData>                              m_funcPointers;
  std::map<std::string, HandleData>                                   m_handles;
  std::set<std::string>                                               m_includes;
  std::map<std::string, PlatformData>                                 m_platforms;
  std::set<std::string>                                               m_RAIISpecialFunctions;
  std::map<std::string, StructureData>                                m_structures;
  std::map<std::string, StructureAliasData>                           m_structureAliases;
  std::map<std::string, std::set<std::string>>                        m_structureAliasesInverse;
  std::set<std::string>                                               m_tags;
  std::map<std::string, TypeData>                                     m_types;
  std::string                                                         m_typesafeCheck;
  std::string                                                         m_version;
  std::string                                                         m_vulkanLicenseHeader;
};

const size_t INVALID_INDEX = (size_t)~0;
