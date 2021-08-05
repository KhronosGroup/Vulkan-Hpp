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
  std::string         generateStructForwardDeclarations() const;
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

    bool operator!=( TypeInfo const & rhs ) const
    {
      return !operator==( rhs );
    }

    bool operator<( TypeInfo const & rhs ) const
    {
      return ( prefix < rhs.prefix ) ||
             ( ( prefix == rhs.prefix ) &&
               ( ( type < rhs.type ) || ( ( type == rhs.type ) && ( postfix < rhs.postfix ) ) ) );
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
      : deprecatedBy( deprecatedBy_ )
      , number( number_ )
      , obsoletedBy( obsoletedBy_ )
      , platform( platform_ )
      , promotedTo( promotedTo_ )
      , xmlLine( line )
    {}

    std::string              deprecatedBy;
    std::string              number;
    std::string              obsoletedBy;
    std::string              platform;
    std::string              promotedTo;
    std::set<std::string>    requiresAttribute;
    std::vector<RequireData> requireData;
    int                      xmlLine;
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
    TypeData( TypeCategory category_ ) : category( category_ ) {}

    TypeCategory category;
    std::string  referencedIn;
  };

private:
  void        addCommand( std::string const & name, CommandData & commandData );
  void        addMissingFlagBits( std::vector<RequireData> & requireData, std::string const & referencedIn );
  std::string addTitleAndProtection( std::string const & title,
                                     std::string const & strIf,
                                     std::string const & strElse = {} ) const;
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
  bool        checkEquivalentSingularConstructor(
           std::vector<std::map<std::string, CommandData>::const_iterator> const & constructorIts,
           std::map<std::string, CommandData>::const_iterator                      constructorIt,
           std::vector<ParamData>::const_iterator                                  lenIt ) const;
  void                checkExtensionCorrectness() const;
  void                checkFuncPointerCorrectness() const;
  void                checkHandleCorrectness() const;
  void                checkStructCorrectness() const;
  void                checkStructMemberCorrectness( std::string const &             structureName,
                                                    std::vector<MemberData> const & members,
                                                    std::set<std::string> &         sTypeValues ) const;
  bool                containsArray( std::string const & type ) const;
  bool                containsUnion( std::string const & type ) const;
  std::vector<size_t> determineConstPointerParamIndices( std::vector<ParamData> const & params ) const;
  size_t              determineDefaultStartIndex( std::vector<ParamData> const & params,
                                                  std::set<size_t> const &       skippedParams ) const;
  size_t              determineInitialSkipCount( std::string const & command ) const;
  std::vector<size_t> determineNonConstPointerParamIndices( std::vector<ParamData> const & params ) const;
  std::vector<std::map<std::string, CommandData>::const_iterator>
    determineRAIIHandleConstructors( std::string const &                                handleType,
                                     std::map<std::string, CommandData>::const_iterator destructorIt,
                                     std::set<std::string> &                            specialFunctions ) const;
  std::map<std::string, CommandData>::const_iterator
                           determineRAIIHandleDestructor( std::string const & handleType ) const;
  std::set<size_t>         determineSkippedParams( std::vector<ParamData> const &   params,
                                                   size_t                           initialSkipCount,
                                                   std::map<size_t, size_t> const & vectorParamIndices,
                                                   std::vector<size_t> const &      returnParamIndex,
                                                   bool                             singular ) const;
  std::string              determineSubStruct( std::pair<std::string, StructureData> const & structure ) const;
  std::map<size_t, size_t> determineVectorParamIndices( std::vector<ParamData> const & params ) const;
  void                     distributeSecondLevelCommands( std::set<std::string> const & specialFunctions );
  std::string findBaseName( std::string aliasName, std::map<std::string, EnumAliasData> const & aliases ) const;
  std::string generateArgumentEnhancedConstPointer( ParamData const & param,
                                                    bool              definition,
                                                    bool              withAllocators,
#if !defined( NDEBUG )
                                                    bool withDispatcher,
#endif
                                                    bool & hasDefaultAssignment ) const;
  std::string generateArgumentListEnhanced( std::vector<ParamData> const & params,
                                            std::set<size_t> const &       skippedParams,
                                            std::set<size_t> const &       singularParams,
                                            bool                           definition,
                                            bool                           withAllocators,
                                            bool                           structureChain,
                                            bool                           withDispatcher ) const;
  std::string generateArgumentListStandard( std::vector<ParamData> const & params,
                                            std::set<size_t> const &       skippedParams ) const;
  std::string generateBitmask( std::map<std::string, BitmaskData>::const_iterator bitmaskIt ) const;
  std::string generateBitmasks( std::vector<RequireData> const & requireData,
                                std::set<std::string> &          listedBitmasks,
                                std::string const &              title ) const;
  std::pair<std::string, std::string>
              generateBitmaskValues( std::map<std::string, BitmaskData>::const_iterator bitmaskIt,
                                     std::map<std::string, EnumData>::const_iterator    bitmaskBitsIt ) const;
  std::string generateCallArgumentsEnhanced( std::vector<ParamData> const & params,
                                             size_t                         initialSkipCount,
                                             bool                           nonConstPointerAsNullptr,
                                             std::set<size_t> const &       singularParams,
                                             std::vector<size_t> const &    returnParamIndices,
                                             bool                           raiiHandleMemberFunction ) const;
  std::string generateCallArgumentsStandard( std::string const & handle, std::vector<ParamData> const & params ) const;
  std::string generateCallArgumentEnhanced( std::vector<ParamData> const & params,
                                            size_t                         paramIndex,
                                            bool                           nonConstPointerAsNullptr,
                                            std::set<size_t> const &       singularParams,
                                            std::vector<size_t> const &    returnParamIndices,
                                            bool                           raiiHandleMemberFunction ) const;
  std::string generateCallArgumentEnhancedConstPointer( ParamData const &        param,
                                                        size_t                   paramIndex,
                                                        std::set<size_t> const & singularParams ) const;
  std::string generateCallArgumentEnhancedNonConstPointer( ParamData const &        param,
                                                           size_t                   paramIndex,
                                                           bool                     nonConstPointerAsNullptr,
                                                           std::set<size_t> const & singularParams,
                                                           bool                     raiiHandleMemberFunction ) const;
  std::string generateCallArgumentEnhancedValue( std::vector<ParamData> const & params,
                                                 size_t                         paramIndex,
                                                 std::set<size_t> const &       singularParams,
                                                 std::vector<size_t> const &    returnParamIndices ) const;
  std::string generateCommand( std::string const & name,
                               CommandData const & commandData,
                               size_t              initialSkipCount,
                               bool                definition ) const;
  std::string generateStructForwardDeclarations( std::vector<RequireData> const & requireData,
                                                 std::string const &              title ) const;

  std::string generateCommandBoolGetValue( std::string const & name,
                                           CommandData const & commandData,
                                           size_t              initialSkipCount,
                                           bool                definition,
                                           size_t              nonConstPointerIndex ) const;
  std::string generateCommandDefinitions( std::vector<RequireData> const & requireData,
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
  std::string generateCommandResult( std::string const &              name,
                                     CommandData const &              commandData,
                                     size_t                           initialSkipCount,
                                     bool                             definition,
                                     std::map<size_t, size_t> const & vectorParamIndices ) const;
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
  std::string generateCommandResultEnumerate( std::string const &               name,
                                              CommandData const &               commandData,
                                              size_t                            initialSkipCount,
                                              bool                              definition,
                                              std::pair<size_t, size_t> const & vectorParamIndices,
                                              bool                              withAllocators ) const;
  std::string generateCommandResultEnumerateChained( std::string const &               name,
                                                     CommandData const &               commandData,
                                                     size_t                            initialSkipCount,
                                                     bool                              definition,
                                                     std::pair<size_t, size_t> const & vectorParamIndex,
                                                     std::vector<size_t> const &       returnParamIndices,
                                                     bool                              withAllocator ) const;
  std::string generateCommandResultEnumerateTwoVectors( std::string const &              name,
                                                        CommandData const &              commandData,
                                                        size_t                           initialSkipCount,
                                                        bool                             definition,
                                                        std::map<size_t, size_t> const & vectorParamIndices,
                                                        std::vector<size_t> const &      returnParamIndices,
                                                        bool                             withAllocators ) const;
  std::string generateCommandResultEnumerateTwoVectorsDeprecated( std::string const &              name,
                                                                  CommandData const &              commandData,
                                                                  size_t                           initialSkipCount,
                                                                  bool                             definition,
                                                                  std::map<size_t, size_t> const & vectorParamIndices,
                                                                  bool withAllocators ) const;
  std::string generateCommandResultGetChain( std::string const & name,
                                             CommandData const & commandData,
                                             size_t              initialSkipCount,
                                             bool                definition,
                                             size_t              nonConstPointerIndex ) const;
  std::string generateCommandResultGetHandleUnique( std::string const & name,
                                                    CommandData const & commandData,
                                                    size_t              initialSkipCount,
                                                    bool                definition,
                                                    size_t              nonConstPointerIndex ) const;
  std::string generateCommandResultGetTwoValues( std::string const &         name,
                                                 CommandData const &         commandData,
                                                 size_t                      initialSkipCount,
                                                 bool                        definition,
                                                 std::vector<size_t> const & nonConstPointerParamIndices ) const;
  std::string generateCommandResultGetTwoVectors( std::string const &              name,
                                                  CommandData const &              commandData,
                                                  size_t                           initialSkipCount,
                                                  bool                             definition,
                                                  std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string generateCommandResultGetValue( std::string const & name,
                                             CommandData const & commandData,
                                             size_t              initialSkipCount,
                                             bool                definition,
                                             size_t              nonConstPointerIndex ) const;
  std::string generateCommandResultGetValueDeprecated( std::string const &              name,
                                                       CommandData const &              commandData,
                                                       size_t                           initialSkipCount,
                                                       bool                             definition,
                                                       std::map<size_t, size_t> const & vectorParamIndices,
                                                       size_t                           returnParamIndex ) const;
  std::string generateCommandResultGetVector( std::string const &              name,
                                              CommandData const &              commandData,
                                              size_t                           initialSkipCount,
                                              bool                             definition,
                                              std::map<size_t, size_t> const & vectorParamIndices,
                                              size_t                           returnParamIndex ) const;
  std::string generateCommandResultGetVector( std::string const &              name,
                                              CommandData const &              commandData,
                                              size_t                           initialSkipCount,
                                              bool                             definition,
                                              std::map<size_t, size_t> const & vectorParamIndices,
                                              size_t                           returnParamIndex,
                                              bool                             withAllocator ) const;
  std::string generateCommandResultGetVectorAndValue( std::string const &              name,
                                                      CommandData const &              commandData,
                                                      size_t                           initialSkipCount,
                                                      bool                             definition,
                                                      std::map<size_t, size_t> const & vectorParamIndices,
                                                      std::vector<size_t> const &      returnParamIndex,
                                                      bool                             withAllocator ) const;
  std::string generateCommandResultGetVectorDeprecated( std::string const &              name,
                                                        CommandData const &              commandData,
                                                        size_t                           initialSkipCount,
                                                        bool                             definition,
                                                        std::map<size_t, size_t> const & vectorParamIndices,
                                                        size_t                           returnParamIndex ) const;
  std::string generateCommandResultGetVectorOfHandles( std::string const &              name,
                                                       CommandData const &              commandData,
                                                       size_t                           initialSkipCount,
                                                       bool                             definition,
                                                       std::map<size_t, size_t> const & vectorParamIndices,
                                                       size_t                           returnParamIndex,
                                                       bool                             withAllocator ) const;
  std::string generateCommandResultGetVectorOfHandlesSingular( std::string const &              name,
                                                               CommandData const &              commandData,
                                                               size_t                           initialSkipCount,
                                                               bool                             definition,
                                                               std::map<size_t, size_t> const & vectorParamIndices,
                                                               size_t returnParamIndex ) const;
  std::string generateCommandResultGetVectorOfHandlesUnique( std::string const &              name,
                                                             CommandData const &              commandData,
                                                             size_t                           initialSkipCount,
                                                             bool                             definition,
                                                             std::map<size_t, size_t> const & vectorParamIndices,
                                                             size_t                           returnParamIndex,
                                                             bool                             withAllocator ) const;
  std::string
              generateCommandResultGetVectorOfHandlesUniqueSingular( std::string const &              name,
                                                                     CommandData const &              commandData,
                                                                     size_t                           initialSkipCount,
                                                                     bool                             definition,
                                                                     std::map<size_t, size_t> const & vectorParamIndices,
                                                                     size_t                           returnParamIndex ) const;
  std::string generateCommandResultGetVectorOfVoidSingular( std::string const &              name,
                                                            CommandData const &              commandData,
                                                            size_t                           initialSkipCount,
                                                            bool                             definition,
                                                            std::map<size_t, size_t> const & vectorParamIndices,
                                                            size_t                           returnParamIndex ) const;
  std::string generateCommandResultGetVectorSingular( std::string const &              name,
                                                      CommandData const &              commandData,
                                                      size_t                           initialSkipCount,
                                                      bool                             definition,
                                                      std::map<size_t, size_t> const & vectorParamIndices,
                                                      size_t                           returnParamIndex ) const;
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
  std::string generateCommandStandard( std::string const & name,
                                       CommandData const & commandData,
                                       size_t              initialSkipCount,
                                       bool                definition ) const;
  std::string generateCommandsStandardEnhancedChained( std::string const &              name,
                                                       CommandData const &              commandData,
                                                       size_t                           initialSkipCount,
                                                       bool                             definition,
                                                       std::map<size_t, size_t> const & vectorParamIndices,
                                                       size_t                           nonConstPointerIndex ) const;
  std::string generateCommandType( std::string const & name,
                                   CommandData const & commandData,
                                   size_t              initialSkipCount,
                                   bool                definition ) const;
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
  std::string generateCommandVoid( std::string const &              name,
                                   CommandData const &              commandData,
                                   size_t                           initialSkipCount,
                                   bool                             definition,
                                   std::map<size_t, size_t> const & vectorParamIndices ) const;
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
  std::string generateCommandVoidGetChain( std::string const & name,
                                           CommandData const & commandData,
                                           size_t              initialSkipCount,
                                           bool                definition,
                                           size_t              nonConstPointerIndex ) const;
  std::string generateCommandVoidGetValue( std::string const &              name,
                                           CommandData const &              commandData,
                                           size_t                           initialSkipCount,
                                           bool                             definition,
                                           std::map<size_t, size_t> const & vectorParamIndices,
                                           size_t                           returnParamIndex ) const;
  std::string generateConstexprString( std::pair<std::string, StructureData> const & structData,
                                       bool                                          assignmentOperator ) const;
  std::string generateDestroyCommand( std::string const & name, CommandData const & commandData ) const;
  std::string generateDispatchLoaderDynamicCommandAssignment( std::string const & commandName,
                                                              CommandData const & commandData,
                                                              std::string const & firstArg ) const;
  std::string generateDispatchLoaderStaticCommands( std::vector<RequireData> const & requireData,
                                                    std::set<std::string> &          listedCommands,
                                                    std::string const &              title ) const;
  std::string
    generateEnhancedReturnType( CommandData const & commandData, size_t returnParamIndex, bool isStructureChain ) const;
  std::string generateEnum( std::pair<std::string, EnumData> const & enumData ) const;
  std::string generateEnumInitializer( TypeInfo const &                   type,
                                       std::vector<std::string> const &   arraySizes,
                                       std::vector<EnumValueData> const & values,
                                       bool                               bitmask ) const;
  std::string generateEnums( std::vector<RequireData> const & requireData,
                             std::set<std::string> &          listedEnums,
                             std::string const &              title ) const;
  std::string generateEnumToString( std::pair<std::string, EnumData> const & enumData ) const;
  std::string generateFailureCheck( std::vector<std::string> const & successCodes ) const;
  std::string generateFunctionBodyEnhanced( std::string const &              indentation,
                                            std::string const &              name,
                                            CommandData const &              commandData,
                                            size_t                           initialSkipCount,
                                            size_t                           returnParamIndex,
                                            size_t                           templateParamIndex,
                                            std::map<size_t, size_t> const & vectorParamIndices,
                                            bool                             twoStep,
                                            std::string const &              enhancedReturnType,
                                            bool                             withAllocator ) const;
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
  std::string generateFunctionBodyEnhancedSingleStep( std::string const &              indentation,
                                                      std::string const &              name,
                                                      CommandData const &              commandData,
                                                      size_t                           returnParamIndex,
                                                      size_t                           templateParamIndex,
                                                      std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string generateFunctionBodyEnhancedTwoStep( std::string const &              indentation,
                                                   std::string const &              name,
                                                   CommandData const &              commandData,
                                                   size_t                           returnParamIndex,
                                                   size_t                           templateParamIndex,
                                                   std::map<size_t, size_t> const & vectorParamIndices,
                                                   std::string const &              returnName ) const;
  std::string generateFunctionHeaderArgumentsEnhanced( CommandData const &              commandData,
                                                       size_t                           returnParamIndex,
                                                       size_t                           templateParamIndex,
                                                       size_t                           initialSkipCount,
                                                       std::map<size_t, size_t> const & vectorParamIndices,
                                                       bool                             withDefaults,
                                                       bool                             withAllocator ) const;
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
  std::string generateFunctionHeaderArgumentEnhancedVector( ParamData const &   param,
                                                            std::string const & strippedParameterName,
                                                            bool                hasSizeParam,
                                                            bool                isTemplateParam ) const;
  std::string generateFunctionPointerCheck( std::string const & function, std::string const & referencedIn ) const;
  std::string generateHandle( std::pair<std::string, HandleData> const & handle,
                              std::set<std::string> &                    listedHandles ) const;
  std::string generateHashStructures( std::vector<RequireData> const & requireData, std::string const & title ) const;
  std::string
                                      generateLenInitializer( std::vector<MemberData>::const_iterator                                        mit,
                                                              std::map<std::vector<MemberData>::const_iterator,
                                     std::vector<std::vector<MemberData>::const_iterator>>::const_iterator litit,
                                                              bool mutualExclusiveLens ) const;
  std::string                         generateName( TypeInfo const & typeInfo ) const;
  std::string                         generateNoDiscardStandard( CommandData const & commandData ) const;
  std::pair<std::string, std::string> generateProtection( std::string const & referencedIn,
                                                          std::string const & protect ) const;
  std::pair<std::string, std::string> generateProtection( std::string const & type, bool isAliased ) const;
  std::string                         generateRAIICommandDefinitions( std::vector<RequireData> const & requireData,
                                                                      std::set<std::string> &          listedCommands,
                                                                      std::string const &              title ) const;
  std::string                         generateRAIIHandle( std::pair<std::string, HandleData> const & handle,
                                                          std::set<std::string> &                    listedHandles,
                                                          std::set<std::string> const &              specialFunctions ) const;
  std::pair<std::string, std::string>
    generateRAIIHandleConstructor( std::pair<std::string, HandleData> const &                             handle,
                                   std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
                                   std::string const &                                                    enter,
                                   std::string const &                                                    leave ) const;
  std::pair<std::string, std::string>
              generateRAIIHandleConstructors( std::pair<std::string, HandleData> const & handle ) const;
  std::string generateRAIIHandleConstructorArguments( std::string const &            handleType,
                                                      std::vector<ParamData> const & params,
                                                      bool                           singular,
                                                      bool                           encounteredArgument ) const;
  std::string generateRAIIHandleConstructorCallArguments( std::string const &            handleType,
                                                          std::vector<ParamData> const & params,
                                                          bool                           nonConstPointerAsNullptr,
                                                          std::set<size_t> const &       singularParams,
                                                          std::vector<size_t> const &    returnParamIndices,
                                                          bool allocatorIsMemberVariable ) const;
  std::string generateRAIIHandleConstructorEnumerate(
    std::pair<std::string, HandleData> const &                             handle,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
    std::vector<ParamData>::const_iterator                                 handleParamIt,
    std::vector<ParamData>::const_iterator                                 lenParamIt,
    std::string const &                                                    enter,
    std::string const &                                                    leave ) const;
  std::string generateRAIIHandleConstructorInitializationList(
    std::string const &                                                    handleType,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator destructorIt,
    bool                                                                   hasSecondLevelCommands ) const;
  std::string generateRAIIHandleConstructorResult(
    std::pair<std::string, HandleData> const &                             handle,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
    std::string const &                                                    enter,
    std::string const &                                                    leave ) const;
  std::string generateRAIIHandleConstructorTakeOwnership( std::pair<std::string, HandleData> const & handle ) const;
  std::string generateRAIIHandleConstructorVector(
    std::pair<std::string, HandleData> const &                             handle,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
    std::vector<ParamData>::const_iterator                                 handleParamIt,
    std::string const &                                                    enter,
    std::string const &                                                    leave ) const;
  std::string generateRAIIHandleConstructorVectorSingular(
    std::pair<std::string, HandleData> const &                             handle,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
    std::vector<ParamData>::const_iterator                                 handleParamIt,
    std::string const &                                                    enter,
    std::string const &                                                    leave ) const;
  std::string generateRAIIHandleConstructorVoid(
    std::pair<std::string, HandleData> const &                             handle,
    std::map<std::string, VulkanHppGenerator::CommandData>::const_iterator constructorIt,
    std::string const &                                                    enter,
    std::string const &                                                    leave ) const;
  std::string generateRAIIHandleContext( std::pair<std::string, HandleData> const & handle,
                                         std::set<std::string> const &              specialFunctions ) const;
  std::pair<std::string, std::string>
              generateRAIIHandleDestructor( std::string const &                                handleType,
                                            std::map<std::string, CommandData>::const_iterator destructorIt,
                                            std::string const &                                enter ) const;
  std::string generateRAIIHandleDestructorCallArguments( std::string const &            handleType,
                                                         std::vector<ParamData> const & params ) const;
  std::tuple<std::string, std::string, std::string, std::string>
              generateRAIIHandleDetails( std::pair<std::string, HandleData> const & handle,
                                         std::string const &                        destructorCall ) const;
  std::string generateRAIIHandleMemberFunction( std::string const &           command,
                                                size_t                        initialSkipCount,
                                                std::set<std::string> const & specialFunctions,
                                                bool                          definition ) const;
  std::string generateRAIIHandleMemberFunctionDeclarations( std::pair<std::string, HandleData> const & handle,
                                                            std::set<std::string> const & specialFunctions ) const;
  std::string generateRAIIHandleMemberFunctionResult( std::map<std::string, CommandData>::const_iterator commandIt,
                                                      size_t initialSkipCount,
                                                      bool   definition ) const;
  std::string generateRAIIHandleMemberFunctionResult0Return(
    std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount, bool definition ) const;
  std::string
    generateRAIIHandleMemberFunctionResult1Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                   size_t                                             initialSkipCount,
                                                   size_t                                             returnParamIndex,
                                                   bool definition ) const;
  std::string
    generateRAIIHandleMemberFunctionResult2Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                   size_t                                             initialSkipCount,
                                                   std::vector<size_t> const & returnParamIndices,
                                                   bool                        definition ) const;
  std::string
    generateRAIIHandleMemberFunctionResult3Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                   size_t                                             initialSkipCount,
                                                   std::vector<size_t> const & returnParamIndices,
                                                   bool                        definition ) const;
  std::string
    generateRAIIHandleMemberFunctionResultEnumerate( std::map<std::string, CommandData>::const_iterator commandIt,
                                                     size_t                           initialSkipCount,
                                                     std::map<size_t, size_t> const & vectorParamIndices,
                                                     std::vector<size_t> const &      nonConstPointerParamIndices,
                                                     bool                             definition ) const;
  std::string generateRAIIHandleMemberFunctionResultEnumerateTwoVectors(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices,
    bool                                               definition ) const;
  std::pair<std::string, std::string>
    generateRAIIHandleMemberFunctionResultGetTwoValues( std::map<std::string, CommandData>::const_iterator commandIt,
                                                        size_t                           initialSkipCount,
                                                        std::map<size_t, size_t> const & vectorParamIndices,
                                                        std::vector<size_t> const & nonConstPointerParamIndices ) const;
  std::string generateRAIIHandleMemberFunctionResultMulti( std::map<std::string, CommandData>::const_iterator commandIt,
                                                           size_t                           initialSkipCount,
                                                           std::map<size_t, size_t> const & vectorParamIndices,
                                                           bool                             definition ) const;
  std::string generateRAIIHandleMemberFunctionResultMultiGetTwoValues(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices,
    bool                                               definition ) const;
  std::string
    generateRAIIHandleMemberFunctionResultMultiGetValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                         size_t                           initialSkipCount,
                                                         std::map<size_t, size_t> const & vectorParamIndices,
                                                         std::vector<size_t> const &      nonConstPointerParamIndices,
                                                         bool                             definition ) const;
  std::string generateRAIIHandleMemberFunctionResultMultiGetVectorOfVoid(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices,
    bool                                               definition ) const;
  std::string generateRAIIHandleMemberFunctionResultMultiGetVectorOfVoidSingular(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices,
    bool                                               definition ) const;
  std::string
    generateRAIIHandleMemberFunctionResultMultiNoErrors( std::map<std::string, CommandData>::const_iterator commandIt,
                                                         size_t                           initialSkipCount,
                                                         std::map<size_t, size_t> const & vectorParamIndices,
                                                         bool                             definition ) const;
  std::string
    generateRAIIHandleMemberFunctionResultSingle( std::map<std::string, CommandData>::const_iterator commandIt,
                                                  size_t                                             initialSkipCount,
                                                  std::map<size_t, size_t> const &                   vectorParamIndices,
                                                  bool                                               definition ) const;
  std::string
    generateRAIIHandleMemberFunctionResultSingleGetChain( std::map<std::string, CommandData>::const_iterator commandIt,
                                                          size_t                           initialSkipCount,
                                                          std::map<size_t, size_t> const & vectorParamIndices,
                                                          std::vector<size_t> const &      nonConstPointerParamIndices,
                                                          bool                             definition ) const;
  std::string
    generateRAIIHandleMemberFunctionResultSingleGetVector( std::map<std::string, CommandData>::const_iterator commandIt,
                                                           size_t                           initialSkipCount,
                                                           std::map<size_t, size_t> const & vectorParamIndices,
                                                           size_t                           returnParamIndex,
                                                           bool                             definition ) const;
  std::string generateRAIIHandleMemberFunctionResultSingleGetVectorAndValue(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices,
    bool                                               definition ) const;
  std::string generateRAIIHandleMemberFunctionResultSingleGetVectorOfVoid(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices,
    bool                                               definition ) const;
  std::string generateRAIIHandleMemberFunctionResultSingleGetVectorSingular(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    size_t                                             returnParamIndex,
    bool                                               definition ) const;
  std::string generateRAIIHandleMemberFunctionResultSingleGetVectorOfVoidSingular(
    std::map<std::string, CommandData>::const_iterator commandIt,
    size_t                                             initialSkipCount,
    std::map<size_t, size_t> const &                   vectorParamIndices,
    std::vector<size_t> const &                        nonConstPointerParamIndices,
    bool                                               definition ) const;
  std::string
    generateRAIIHandleMemberFunctionResultSingleNoErrors( std::map<std::string, CommandData>::const_iterator commandIt,
                                                          size_t                           initialSkipCount,
                                                          std::map<size_t, size_t> const & vectorParamIndices,
                                                          bool                             definition ) const;
  std::string
    generateRAIIHandleMemberFunctionResultSingleGetValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                          size_t                           initialSkipCount,
                                                          std::map<size_t, size_t> const & vectorParamIndices,
                                                          std::vector<size_t> const &      nonConstPointerParamIndices,
                                                          bool                             definition ) const;
  std::string generateRAIIHandleMemberFunctionType( std::map<std::string, CommandData>::const_iterator commandIt,
                                                    size_t                                             initialSkipCount,
                                                    std::map<size_t, size_t> const & vectorParamIndices,
                                                    bool                             definition ) const;
  std::string generateRAIIHandleMemberFunctionValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                     size_t initialSkipCount,
                                                     bool   definition ) const;
  std::string generateRAIIHandleMemberFunctionValue0Return(
    std::map<std::string, CommandData>::const_iterator commandIt, size_t initialSkipCount, bool definition ) const;
  std::string generateRAIIHandleMemberFunctionVkType( std::map<std::string, CommandData>::const_iterator commandIt,
                                                      size_t                           initialSkipCount,
                                                      std::map<size_t, size_t> const & vectorParamIndices,
                                                      bool                             definition ) const;
  std::string generateRAIIHandleMemberFunctionVoid( std::map<std::string, CommandData>::const_iterator commandIt,
                                                    size_t                                             initialSkipCount,
                                                    bool definition ) const;
  std::string generateRAIIHandleMemberFunctionVoid0Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                           size_t initialSkipCount,
                                                           bool   definition ) const;
  std::string generateRAIIHandleMemberFunctionVoid1Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                           size_t initialSkipCount,
                                                           size_t returnParamIndex,
                                                           bool   definition ) const;
  std::string generateRAIIHandleMemberFunctionVoid2Return( std::map<std::string, CommandData>::const_iterator commandIt,
                                                           size_t                      initialSkipCount,
                                                           std::vector<size_t> const & returnParamIndices,
                                                           bool                        definition ) const;
  std::string
    generateRAIIHandleMemberFunctionVoidEnumerate( std::map<std::string, CommandData>::const_iterator commandIt,
                                                   size_t                                             initialSkipCount,
                                                   std::map<size_t, size_t> const & vectorParamIndices,
                                                   std::vector<size_t> const &      nonConstPointerParamIndices,
                                                   bool                             definition ) const;
  std::string
    generateRAIIHandleMemberFunctionVoidEnumerateChain( std::map<std::string, CommandData>::const_iterator commandIt,
                                                        size_t                           initialSkipCount,
                                                        std::map<size_t, size_t> const & vectorParamIndices,
                                                        std::vector<size_t> const &      nonConstPointerParamIndices,
                                                        bool                             definition ) const;
  std::string
    generateRAIIHandleMemberFunctionVoidGetChain( std::map<std::string, CommandData>::const_iterator commandIt,
                                                  size_t                                             initialSkipCount,
                                                  std::map<size_t, size_t> const &                   vectorParamIndices,
                                                  std::vector<size_t> const & nonConstPointerParamIndices,
                                                  bool                        definition ) const;
  std::string
    generateRAIIHandleMemberFunctionVoidGetValue( std::map<std::string, CommandData>::const_iterator commandIt,
                                                  size_t                                             initialSkipCount,
                                                  std::map<size_t, size_t> const &                   vectorParamIndices,
                                                  std::vector<size_t> const & nonConstPointerParamIndices,
                                                  bool                        definition ) const;
  std::string generateRAIIHandleSingularConstructorArguments(
    std::pair<std::string, HandleData> const &         handle,
    std::map<std::string, CommandData>::const_iterator constructorIt ) const;
  std::string generateRAIIHandleUpgradeConstructor( std::pair<std::string, HandleData> const & handle ) const;
  std::string generateRAIIHandleVectorSizeCheck( std::string const &                           name,
                                                 CommandData const &                           commandData,
                                                 size_t                                        initialSkipCount,
                                                 std::map<size_t, std::vector<size_t>> const & countToVectorMap,
                                                 std::set<size_t> const &                      skippedParams ) const;
  std::string generateReturnType( CommandData const & commandData, std::string const & baseType ) const;
  std::string generateSizeCheck( std::vector<std::vector<MemberData>::const_iterator> const & arrayIts,
                                 std::string const &                                          structName,
                                 std::string const &                                          prefix,
                                 bool mutualExclusiveLens ) const;
  std::string generateStruct( std::pair<std::string, StructureData> const & structure,
                              std::set<std::string> &                       listedStructs ) const;
  std::string generateStructAssignmentOperators( std::pair<std::string, StructureData> const & structure,
                                                 std::string const &                           prefix ) const;
  std::string generateStructCompareOperators( std::pair<std::string, StructureData> const & structure ) const;
  std::string
    generateStructConstructorArgument( bool listedArgument, MemberData const & memberData, bool withDefault ) const;
  std::string generateStructConstructors( std::pair<std::string, StructureData> const & structData,
                                          std::string const &                           prefix ) const;
  std::string generateStructConstructorsEnhanced( std::pair<std::string, StructureData> const & structData,
                                                  std::string const &                           prefix ) const;
  std::string generateStructExtendsStructs( std::vector<RequireData> const & requireData,
                                            std::set<std::string> &          listedStructs,
                                            std::string const &              title ) const;
  std::pair<std::string, std::string> generateStructMembers( std::pair<std::string, StructureData> const & structData,
                                                             std::string const & prefix ) const;
  std::string                         generateStructSetter( std::string const &             structureName,
                                                            std::vector<MemberData> const & memberData,
                                                            size_t                          index ) const;
  std::string generateStructSubConstructor( std::pair<std::string, StructureData> const & structData,
                                            std::string const &                           prefix ) const;
  std::string generateStructure( std::pair<std::string, StructureData> const & structure ) const;
  std::string generateSuccessCheck( std::vector<std::string> const & successCodes ) const;
  std::string generateSuccessCodeList( std::vector<std::string> const & successCodes ) const;
  std::string generateUnion( std::pair<std::string, StructureData> const & structure ) const;
  std::string generateUniqueTypes( std::string const & parentType, std::set<std::string> const & childrenTypes ) const;
  std::string generateVectorSizeCheck( std::string const &                           name,
                                       CommandData const &                           commandData,
                                       size_t                                        initialSkipCount,
                                       std::map<size_t, std::vector<size_t>> const & countToVectorMap,
                                       std::set<size_t> const &                      skippedParams ) const;
  std::string getPlatform( std::string const & title ) const;
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
  void readExtensionRequireCommand( tinyxml2::XMLElement const * element,
                                    std::string const &          extensionName,
                                    RequireData &                requireData );
  void readExtensionRequireType( tinyxml2::XMLElement const * element,
                                 std::string const &          extensionName,
                                 RequireData &                requireData );
  void readExtensions( tinyxml2::XMLElement const * element );
  void readFeature( tinyxml2::XMLElement const * element );
  void readFeatureRequire( tinyxml2::XMLElement const *                 element,
                           std::map<std::string, FeatureData>::iterator featureIt );
  void readFeatureRequireCommand( tinyxml2::XMLElement const *                 element,
                                  std::map<std::string, FeatureData>::iterator featureIt,
                                  RequireData &                                requireData );
  void readFeatureRequireType( tinyxml2::XMLElement const *                 element,
                               std::map<std::string, FeatureData>::iterator featureIt,
                               RequireData &                                requireData );
  void readFuncpointer( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void readHandle( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  std::pair<NameData, TypeInfo> readNameAndType( tinyxml2::XMLElement const * elements );
  void                          readPlatform( tinyxml2::XMLElement const * element );
  void                          readPlatforms( tinyxml2::XMLElement const * element );
  void                          readRegistry( tinyxml2::XMLElement const * element );
  void readRequireEnum( tinyxml2::XMLElement const * element, std::string const & extensionName );
  void readRequireEnum( tinyxml2::XMLElement const *               element,
                        std::map<std::string, std::string> const & attributes,
                        std::string const &                        extensionName );
  void readRequireEnumAlias( tinyxml2::XMLElement const *               element,
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
  TypeInfo readTypeInfo( tinyxml2::XMLElement const * element ) const;
  void     readTypes( tinyxml2::XMLElement const * element );
  void     registerDeleter( std::string const & name, std::pair<std::string, CommandData> const & commandData );
  void     renameFunctionParameters();
  void     rescheduleRAIIHandle( std::string &                              str,
                                 std::pair<std::string, HandleData> const & handle,
                                 std::set<std::string> &                    listedHandles,
                                 std::set<std::string> const &              specialFunctions ) const;
  std::vector<std::string> selectCommandsByHandle( std::vector<RequireData> const & requireData,
                                                   std::set<std::string> const &    handleCommands,
                                                   std::set<std::string> &          listedCommands ) const;
  void                     setVulkanLicenseHeader( int line, std::string const & comment );
  std::string              toString( TypeCategory category );

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
