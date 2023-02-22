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

const size_t INVALID_INDEX = (size_t)~0;

template <typename BitType>
class Flags
{
public:
  using MaskType = typename std::underlying_type<BitType>::type;

  constexpr Flags() noexcept : m_mask( 0 ) {}

  constexpr Flags( BitType bit ) noexcept : m_mask( static_cast<MaskType>( bit ) ) {}

  constexpr explicit Flags( MaskType flags ) noexcept : m_mask( flags ) {}

  constexpr bool operator!() const noexcept
  {
    return !m_mask;
  }

  constexpr bool operator&( BitType const & rhs ) const noexcept
  {
    return m_mask & static_cast<MaskType>( rhs );
  }

  constexpr Flags<BitType> operator&( Flags<BitType> const & rhs ) const noexcept
  {
    return Flags<BitType>( m_mask & rhs.m_mask );
  }

  constexpr Flags<BitType> operator|( Flags<BitType> const & rhs ) const noexcept
  {
    return Flags<BitType>( m_mask | rhs.m_mask );
  }

private:
  MaskType m_mask;
};

enum class CommandFlavourFlagBits : uint8_t
{
  enhanced      = 1 << 0,
  chained       = 1 << 1,
  singular      = 1 << 2,
  unique        = 1 << 3,
  withAllocator = 1 << 4
};
using CommandFlavourFlags = Flags<CommandFlavourFlagBits>;

constexpr CommandFlavourFlags operator|( CommandFlavourFlagBits const & lhs, CommandFlavourFlagBits const & rhs ) noexcept
{
  return CommandFlavourFlags( lhs ) | CommandFlavourFlags( rhs );
}

class VulkanHppGenerator
{
public:
  VulkanHppGenerator( tinyxml2::XMLDocument const & document );

  void generateVulkanEnumsHppFile() const;
  void generateVulkanFormatTraitsHppFile() const;
  void generateVulkanFuncsHppFile() const;
  void generateVulkanHandlesHppFile() const;
  void generateVulkanHashHppFile() const;
  void generateVulkanHppFile() const;
  void generateVulkanRAIIHppFile() const;
  void generateVulkanStaticAssertionsHppFile() const;
  void generateVulkanStructsHppFile() const;
  void generateVulkanToStringHppFile() const;
  void prepareRAIIHandles();
  void prepareVulkanFuncs();

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
    std::string              stride;
    int                      xmlLine;
  };

  struct CommandData
  {
    CommandData( int line ) : xmlLine( line ) {}

    std::string              alias;
    std::vector<std::string> errorCodes;
    std::string              handle;
    std::vector<ParamData>   params;
    std::set<std::string>    requiredIn;
    std::string              returnType;
    std::vector<std::string> successCodes;
    int                      xmlLine;
  };

  struct DefineData
  {
    DefineData( std::string const & require_, int line ) : require( require_ ), xmlLine( line ) {}

    std::string require;
    int         xmlLine;
  };

  struct EnumAliasData
  {
    EnumAliasData( std::string const & name_, int line ) : name( name_ ), xmlLine( line ) {}

    std::string name;
    int         xmlLine;
  };

  struct EnumValueData
  {
    EnumValueData( int                              line,
                   std::string const &              name_,
                   std::string const &              protect_,
                   std::string const &              extension_,
                   std::vector<std::string> const & depends_,
                   bool                             singleBit_ )
      : name( name_ ), depends( depends_ ), extension( extension_ ), protect( protect_ ), singleBit( singleBit_ ), xmlLine( line )
    {
    }

    std::string              name;
    std::vector<std::string> depends;
    std::string              extension;
    std::string              protect;
    bool                     singleBit;
    int                      xmlLine;
  };

  struct EnumData
  {
    void addEnumAlias( int line, std::string const & name, std::string const & alias );
    void addEnumValue( int                              line,
                       std::string const &              valueName,
                       std::string const &              protect,
                       bool                             bitpos,
                       std::string const &              extension,
                       std::vector<std::string> const & depends );

    std::string                          alias     = {};  // alias for this enum
    std::map<std::string, EnumAliasData> aliases   = {};  // aliases for the values
    std::string                          bitwidth  = {};
    bool                                 isBitmask = false;
    std::vector<EnumValueData>           values    = {};
    int                                  xmlLine   = 0;
  };

  struct RequireData
  {
    RequireData( int line, std::vector<std::string> const & depends_ );

    std::vector<std::string> depends;
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
    std::set<std::string>    depends;
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
    HandleData( std::string const & p, std::string const & objType, bool isDispatchable, int line )
      : objTypeEnum( objType ), parent( p ), isDispatchable( isDispatchable ), xmlLine( line )
    {
    }

    std::string           alias;
    std::set<std::string> childrenHandles;
    std::set<std::string> commands;
    std::string           deleteCommand;
    std::string           deleteParent;
    std::string           deletePool;
    std::string           objTypeEnum;
    std::string           parent;
    std::set<std::string> secondLevelCommands;
    bool                  isDispatchable;
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
    std::vector<std::string> selection;
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
    TypeCategory          category   = TypeCategory::Unknown;
    std::set<std::string> requiredIn = {};
  };

  struct VectorParamData
  {
    size_t lenParam    = INVALID_INDEX;
    size_t strideParam = INVALID_INDEX;
  };

private:
  void        addCommand( std::string const & name, CommandData & commandData );
  void        addMissingFlagBits( std::vector<RequireData> & requireData, std::string const & requiredIn );
  std::string addTitleAndProtection( std::string const & title, std::string const & strIf, std::string const & strElse = {} ) const;
  bool        allVectorSizesSupported( std::vector<ParamData> const & params, std::map<size_t, VectorParamData> const & vectorParams ) const;
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
  void        checkDefineCorrectness() const;
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
  std::string              combineDataTypes( std::map<size_t, VectorParamData> const & vectorParams,
                                             std::vector<size_t> const &               returnParams,
                                             bool                                      enumerating,
                                             std::vector<std::string> const &          dataTypes,
                                             CommandFlavourFlags                       flavourFlags,
                                             bool                                      raii ) const;
  bool                     containsArray( std::string const & type ) const;
  bool                     containsFuncPointer( std::string const & type ) const;
  bool                     containsFloatingPoints( std::vector<MemberData> const & members ) const;
  bool                     containsUnion( std::string const & type ) const;
  std::vector<size_t>      determineConstPointerParams( std::vector<ParamData> const & params ) const;
  std::vector<std::string> determineDataTypes( std::vector<VulkanHppGenerator::ParamData> const & params,
                                               std::map<size_t, VectorParamData> const &          vectorParams,
                                               std::vector<size_t> const &                        returnParams,
                                               std::set<size_t> const &                           templatedParams ) const;
  size_t                   determineDefaultStartIndex( std::vector<ParamData> const & params, std::set<size_t> const & skippedParams ) const;
  bool                     determineEnumeration( std::map<size_t, VectorParamData> const & vectorParams, std::vector<size_t> const & returnParams ) const;
  size_t                   determineInitialSkipCount( std::string const & command ) const;
  std::vector<size_t>      determineReturnParams( std::vector<ParamData> const & params ) const;
  std::vector<std::map<std::string, CommandData>::const_iterator>
    determineRAIIHandleConstructors( std::string const & handleType, std::map<std::string, CommandData>::const_iterator destructorIt ) const;
  std::map<std::string, CommandData>::const_iterator determineRAIIHandleDestructor( std::string const & handleType ) const;
  std::set<size_t>                        determineSingularParams( size_t returnParam, std::map<size_t, VectorParamData> const & vectorParams ) const;
  std::set<size_t>                        determineSkippedParams( std::vector<ParamData> const &            params,
                                                                  size_t                                    initialSkipCount,
                                                                  std::map<size_t, VectorParamData> const & vectorParams,
                                                                  std::vector<size_t> const &               returnParam,
                                                                  bool                                      singular ) const;
  std::string                             determineSubStruct( std::pair<std::string, StructureData> const & structure ) const;
  std::map<size_t, VectorParamData>       determineVectorParams( std::vector<ParamData> const & params ) const;
  std::set<size_t>                        determineVoidPointerParams( std::vector<ParamData> const & params ) const;
  void                                    distributeSecondLevelCommands( std::set<std::string> const & specialFunctions );
  std::string                             findBaseName( std::string aliasName, std::map<std::string, EnumAliasData> const & aliases ) const;
  std::vector<MemberData>::const_iterator findStructMemberIt( std::string const & name, std::vector<MemberData> const & memberData ) const;
  std::vector<MemberData>::const_iterator findStructMemberItByType( std::string const & type, std::vector<MemberData> const & memberData ) const;
  std::pair<std::string, std::string>     generateAllocatorTemplates( std::vector<size_t> const &               returnParams,
                                                                      std::vector<std::string> const &          returnDataTypes,
                                                                      std::map<size_t, VectorParamData> const & vectorParams,
                                                                      CommandFlavourFlags                       flavourFlags,
                                                                      bool                                      definition ) const;
  std::string                             generateArgumentListEnhanced( std::vector<ParamData> const &            params,
                                                                        std::vector<size_t> const &               returnParams,
                                                                        std::map<size_t, VectorParamData> const & vectorParams,
                                                                        std::set<size_t> const &                  skippedParams,
                                                                        std::set<size_t> const &                  singularParams,
                                                                        std::set<size_t> const &                  templatedParams,
                                                                        bool                                      definition,
                                                                        CommandFlavourFlags                       flavourFlags,
                                                                        bool                                      withDispatcher ) const;
  std::string                             generateArgumentListStandard( std::vector<ParamData> const & params, std::set<size_t> const & skippedParams ) const;
  std::string                             generateArgumentTemplates( std::vector<ParamData> const &            params,
                                                                     std::vector<size_t> const &               returnParams,
                                                                     std::map<size_t, VectorParamData> const & vectorParams,
                                                                     std::set<size_t> const &                  templatedParams,
                                                                     CommandFlavourFlags                       flavourFlags,
                                                                     bool                                      raii ) const;
  std::string                             generateBaseTypes() const;
  std::string generateBitmask( std::map<std::string, BitmaskData>::const_iterator bitmaskIt, std::string const & surroundingProtect ) const;
  std::string generateBitmasksToString() const;
  std::string generateBitmasksToString( std::vector<RequireData> const & requireData, std::set<std::string> & listedBitmasks, std::string const & title ) const;
  std::string generateBitmaskToString( std::map<std::string, BitmaskData>::const_iterator bitmaskIt ) const;
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
                                            std::set<size_t> const &       templatedParams ) const;
  std::string generateCallArgumentEnhancedConstPointer( ParamData const &        param,
                                                        size_t                   paramIndex,
                                                        std::set<size_t> const & singularParams,
                                                        std::set<size_t> const & templatedParams ) const;
  std::string generateCallArgumentEnhancedNonConstPointer( ParamData const &        param,
                                                           size_t                   paramIndex,
                                                           bool                     nonConstPointerAsNullptr,
                                                           std::set<size_t> const & singularParams ) const;
  std::string generateCallArgumentEnhancedValue( std::vector<ParamData> const & params, size_t paramIndex, std::set<size_t> const & singularParams ) const;
  std::string generateCallSequence( std::string const &                       name,
                                    CommandData const &                       commandData,
                                    std::vector<size_t> const &               returnParams,
                                    std::map<size_t, VectorParamData> const & vectorParams,
                                    size_t                                    initialSkipCount,
                                    std::set<size_t> const &                  singularParams,
                                    std::set<size_t> const &                  templatedParams,
                                    CommandFlavourFlags                       flavourFlags,
                                    bool                                      raii ) const;
  std::string generateChainTemplates( std::vector<size_t> const & returnParams, bool chained ) const;
  std::string generateCommand( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const;
  std::string generateCommandDefinitions() const;
  std::string
    generateCommandDefinitions( std::vector<RequireData> const & requireData, std::set<std::string> & listedCommands, std::string const & title ) const;
  std::string generateCommandDefinitions( std::string const & command, std::string const & handle ) const;
  std::string generateCommandEnhanced( std::string const &                       name,
                                       CommandData const &                       commandData,
                                       size_t                                    initialSkipCount,
                                       bool                                      definition,
                                       std::map<size_t, VectorParamData> const & vectorParams,
                                       std::vector<size_t> const &               returnParams,
                                       CommandFlavourFlags                       flavourFlags = {} ) const;
  std::string generateCommandName( std::string const &            vulkanCommandName,
                                   std::vector<ParamData> const & params,
                                   size_t                         initialSkipCount,
                                   std::set<std::string> const &  tags,
                                   CommandFlavourFlags            flavourFlags = {} ) const;
  std::string generateCommandResult( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const;
  std::string generateCommandResultMultiSuccessNoErrors(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const;
  std::string generateCommandResultMultiSuccessWithErrors(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const;
  std::string generateCommandResultMultiSuccessWithErrors1Return(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const;
  std::string generateCommandResultMultiSuccessWithErrors2Return( std::string const &         name,
                                                                  CommandData const &         commandData,
                                                                  size_t                      initialSkipCount,
                                                                  bool                        definition,
                                                                  std::vector<size_t> const & returnParamIndices,
                                                                  bool                        raii ) const;
  std::string generateCommandResultMultiSuccessWithErrors3Return( std::string const &         name,
                                                                  CommandData const &         commandData,
                                                                  size_t                      initialSkipCount,
                                                                  bool                        definition,
                                                                  std::vector<size_t> const & returnParamIndices,
                                                                  bool                        raii ) const;
  std::string
    generateCommandResultSingleSuccess( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const;
  std::string generateCommandResultSingleSuccessNoErrors(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const;
  std::string generateCommandResultSingleSuccessWithErrors(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const;
  std::string generateCommandResultSingleSuccessWithErrors1Return(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const;
  std::string generateCommandResultSingleSuccessWithErrors1ReturnChain(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const;
  std::string generateCommandResultSingleSuccessWithErrors1ReturnHandle(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const;
  std::string generateCommandResultSingleSuccessWithErrors1ReturnValue(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const;
  std::string generateCommandResultSingleSuccessWithErrors1ReturnVoid(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const;
  std::string generateCommandResultSingleSuccessWithErrors2Return( std::string const &         name,
                                                                   CommandData const &         commandData,
                                                                   size_t                      initialSkipCount,
                                                                   bool                        definition,
                                                                   std::vector<size_t> const & returnParamIndices,
                                                                   bool                        raii ) const;
  std::string generateCommandResultWithErrors0Return(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const;
  std::string generateCommandSet( bool                             definition,
                                  std::string const &              standard,
                                  std::vector<std::string> const & enhanced = {},
                                  std::vector<std::string> const & unique   = {} ) const;
  std::string generateCommandSet( std::string const & standard, std::string const & enhanced ) const;
  std::string generateCommandSetInclusive( std::string const &                      name,
                                           CommandData const &                      commandData,
                                           size_t                                   initialSkipCount,
                                           bool                                     definition,
                                           std::vector<size_t>                      returnParams,
                                           std::map<size_t, VectorParamData>        vectorParams,
                                           bool                                     unique,
                                           std::vector<CommandFlavourFlags> const & flags,
                                           bool                                     raii,
                                           bool                                     raiiFactory,
                                           std::vector<CommandFlavourFlags> const & raiiFlags ) const;
  std::string
    generateCommandSetExclusive( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const;
  std::string generateCommandStandard( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition ) const;
  std::string generateCommandVoid( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const;
  std::string generateCommandValue( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const;
  std::string
    generateCommandVoid0Return( std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, bool raii ) const;
  std::string generateCommandVoid1Return(
    std::string const & name, CommandData const & commandData, size_t initialSkipCount, bool definition, size_t returnParam, bool raii ) const;
  std::string generateCommandVoid2Return( std::string const &         name,
                                          CommandData const &         commandData,
                                          size_t                      initialSkipCount,
                                          bool                        definition,
                                          std::vector<size_t> const & returnParamIndices,
                                          bool                        raii ) const;
  std::string generateConstexprString( std::string const & structName ) const;
  std::string generateDataDeclarations( CommandData const &                       commandData,
                                        std::vector<size_t> const &               returnParams,
                                        std::map<size_t, VectorParamData> const & vectorParams,
                                        std::set<size_t> const &                  templatedParams,
                                        CommandFlavourFlags                       flavourFlags,
                                        bool                                      raii,
                                        std::vector<std::string> const &          dataTypes,
                                        std::string const &                       dataType,
                                        std::string const &                       returnType,
                                        std::string const &                       returnVariable ) const;
  std::string generateDataDeclarations1Return( CommandData const &                       commandData,
                                               std::vector<size_t> const &               returnParams,
                                               std::map<size_t, VectorParamData> const & vectorParams,
                                               std::set<size_t> const &                  templatedParams,
                                               CommandFlavourFlags                       flavourFlags,
                                               std::vector<std::string> const &          dataTypes,
                                               std::string const &                       dataType,
                                               std::string const &                       returnType,
                                               std::string const &                       returnVariable ) const;
  std::string generateDataDeclarations2Returns( CommandData const &                       commandData,
                                                std::vector<size_t> const &               returnParams,
                                                std::map<size_t, VectorParamData> const & vectorParams,
                                                CommandFlavourFlags                       flavourFlags,
                                                bool                                      raii,
                                                std::vector<std::string> const &          dataTypes,
                                                std::string const &                       dataType,
                                                std::string const &                       returnVariable ) const;
  std::string generateDataDeclarations3Returns( CommandData const &              commandData,
                                                std::vector<size_t> const &      returnParams,
                                                CommandFlavourFlags              flavourFlags,
                                                bool                             raii,
                                                std::vector<std::string> const & dataTypes ) const;
  std::string generateDataPreparation( CommandData const &                       commandData,
                                       size_t                                    initialSkipCount,
                                       std::vector<size_t> const &               returnParams,
                                       std::map<size_t, VectorParamData> const & vectorParams,
                                       std::set<size_t> const &                  templatedParams,
                                       CommandFlavourFlags                       flavourFlags,
                                       bool                                      enumerating ) const;
  std::string generateDataSizeChecks( CommandData const &                       commandData,
                                      std::vector<size_t> const &               returnParams,
                                      std::vector<std::string> const &          returnParamTypes,
                                      std::map<size_t, VectorParamData> const & vectorParams,
                                      std::set<size_t> const &                  templatedParams,
                                      bool                                      singular ) const;
  std::string generateDispatchLoaderDynamic() const;  // uses vkGet*ProcAddress to get function pointers
  std::string generateDispatchLoaderStatic() const;   // uses exported symbols from loader
  std::string generateDestroyCommand( std::string const & name, CommandData const & commandData ) const;
  std::string
    generateDispatchLoaderDynamicCommandAssignment( std::string const & commandName, CommandData const & commandData, std::string const & firstArg ) const;
  std::string generateDispatchLoaderStaticCommands( std::vector<RequireData> const & requireData,
                                                    std::set<std::string> &          listedCommands,
                                                    std::string const &              title ) const;
  std::string generateEnum( std::pair<std::string, EnumData> const & enumData, std::string const & surroundingProtect ) const;
  std::string generateEnums() const;
  std::string generateEnums( std::vector<RequireData> const & requireData, std::set<std::string> & listedEnums, std::string const & title ) const;
  std::string generateEnumsToString() const;
  std::string generateEnumsToString( std::vector<RequireData> const & requireData, std::set<std::string> & listedEnums, std::string const & title ) const;
  std::string generateEnumInitializer( TypeInfo const &                   type,
                                       std::vector<std::string> const &   arraySizes,
                                       std::vector<EnumValueData> const & values,
                                       bool                               bitmask ) const;
  std::string generateEnumToString( std::pair<std::string, EnumData> const & enumData ) const;
  std::string generateFailureCheck( std::vector<std::string> const & successCodes ) const;
  std::string generateFormatTraits() const;
  std::string generateFunctionPointerCheck( std::string const & function, std::set<std::string> const & requiredIn ) const;
  std::string generateHandle( std::pair<std::string, HandleData> const & handle, std::set<std::string> & listedHandles ) const;
  std::string generateHandleCommandDeclarations( std::set<std::string> const & commands ) const;
  std::string generateHandleDependencies( std::pair<std::string, HandleData> const & handle, std::set<std::string> & listedHandles ) const;
  std::string generateHandleEmpty( HandleData const & handleData ) const;
  std::string generateHandleForwardDeclarations() const;
  std::string generateHandleForwardDeclarations( std::vector<RequireData> const & requireData, std::string const & title ) const;
  std::string generateHandleHashStructures( std::vector<RequireData> const & requireData, std::string const & title ) const;
  std::string generateHandleHashStructures() const;
  std::string generateHandles() const;
  std::string generateIndexTypeTraits() const;
  std::string
              generateLenInitializer( std::vector<MemberData>::const_iterator                                                                                 mit,
                                      std::map<std::vector<MemberData>::const_iterator, std::vector<std::vector<MemberData>::const_iterator>>::const_iterator litit,
                                      bool mutualExclusiveLens ) const;
  std::string generateName( TypeInfo const & typeInfo ) const;
  std::string generateNoExcept( std::vector<std::string> const &          errorCodes,
                                std::vector<size_t> const &               returnParams,
                                std::map<size_t, VectorParamData> const & vectorParams,
                                CommandFlavourFlags                       flavourFlags,
                                bool                                      vectorSizeCheck,
                                bool                                      raii ) const;
  std::string generateObjectDeleter( std::string const & commandName, CommandData const & commandData, size_t initialSkipCount, size_t returnParam ) const;
  std::pair<std::string, std::string> generateProtection( std::string const & protect ) const;
  std::string                         generateRAIICommandDefinitions() const;
  std::string
    generateRAIICommandDefinitions( std::vector<RequireData> const & requireData, std::set<std::string> & listedCommands, std::string const & title ) const;
  std::string generateRAIIDispatchers() const;
  std::string generateRAIIHandle( std::pair<std::string, HandleData> const & handle,
                                  std::set<std::string> &                    listedHandles,
                                  std::set<std::string> const &              specialFunctions ) const;
  std::string generateRAIIHandleCommand( std::string const & command, size_t initialSkipCount, bool definition ) const;
  std::string generateRAIIHandleCommandDeclarations( std::pair<std::string, HandleData> const & handle, std::set<std::string> const & specialFunctions ) const;
  std::string generateRAIIHandleCommandEnhanced( std::string const &                       name,
                                                 CommandData const &                       commandData,
                                                 size_t                                    initialSkipCount,
                                                 std::vector<size_t> const &               returnParams,
                                                 std::map<size_t, VectorParamData> const & vectorParamIndices,
                                                 bool                                      definition,
                                                 CommandFlavourFlags                       flavourFlags = {} ) const;
  std::string generateRAIIHandleCommandFactory( std::string const &                       name,
                                                CommandData const &                       commandData,
                                                size_t                                    initialSkipCount,
                                                std::vector<size_t> const &               returnParams,
                                                std::map<size_t, VectorParamData> const & vectorParams,
                                                bool                                      definition,
                                                CommandFlavourFlags                       flavourFlags = {} ) const;
  std::string generateRAIIHandleCommandFactoryArgumentList( std::vector<ParamData> const & params,
                                                            std::set<size_t> const &       skippedParams,
                                                            bool                           definition,
                                                            bool                           singular ) const;
  std::pair<std::string, std::string> generateRAIIHandleConstructor( std::pair<std::string, HandleData> const &         handle,
                                                                     std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                     std::string const &                                enter,
                                                                     std::string const &                                leave ) const;
  std::pair<std::string, std::string> generateRAIIHandleConstructor1Return2Vector( std::pair<std::string, HandleData> const &         handle,
                                                                                   std::map<std::string, CommandData>::const_iterator constructorIt,
                                                                                   std::string const &                                enter,
                                                                                   std::string const &                                leave,
                                                                                   size_t                                             returnParam,
                                                                                   std::map<size_t, VectorParamData> const & vectorParamIndices ) const;
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
  std::tuple<std::string, std::string, std::string, std::string, std::string, std::string, std::string>
              generateRAIIHandleDetails( std::pair<std::string, HandleData> const & handle ) const;
  std::string generateRAIIHandleForwardDeclarations( std::vector<RequireData> const & requireData, std::string const & title ) const;
  std::string generateRAIIHandles() const;
  std::string generateRAIIHandleSingularConstructorArguments( std::pair<std::string, HandleData> const &         handle,
                                                              std::map<std::string, CommandData>::const_iterator constructorIt ) const;
  std::string generateRAIIHandleVectorSizeCheck( std::string const &                           name,
                                                 CommandData const &                           commandData,
                                                 size_t                                        initialSkipCount,
                                                 std::map<size_t, std::vector<size_t>> const & countToVectorMap,
                                                 std::set<size_t> const &                      skippedParams ) const;
  std::string generateResultAssignment( CommandData const & commandData ) const;
  std::string generateResultCheck(
    CommandData const & commandData, std::string const & className, std::string const & classSeparator, std::string commandName, bool enumerating ) const;
  std::string generateResultExceptions() const;
  std::string generateReturnStatement( std::string const & commandName,
                                       CommandData const & commandData,
                                       std::string const & returnVariable,
                                       std::string const & returnType,
                                       std::string const & dataType,
                                       size_t              initialSkipCount,
                                       size_t              returnParam,
                                       CommandFlavourFlags flavourFlags,
                                       bool                enumerating,
                                       bool                raii ) const;
  std::string generateReturnType( CommandData const &                       commandData,
                                  std::vector<size_t> const &               returnParams,
                                  std::map<size_t, VectorParamData> const & vectorParams,
                                  CommandFlavourFlags                       flavourFlags,
                                  bool                                      raii,
                                  std::string const &                       dataType ) const;
  std::string generateReturnVariable( CommandData const &                       commandData,
                                      std::vector<size_t> const &               returnParams,
                                      std::map<size_t, VectorParamData> const & vectorParams,
                                      CommandFlavourFlags                       flavourFlags ) const;
  std::string
    generateSizeCheck( std::vector<std::vector<MemberData>::const_iterator> const & arrayIts, std::string const & structName, bool mutualExclusiveLens ) const;
  std::string generateStaticAssertions() const;
  std::string generateStaticAssertions( std::vector<RequireData> const & requireData, std::string const & title, std::set<std::string> & listedStructs ) const;
  std::string generateStruct( std::pair<std::string, StructureData> const & structure, std::set<std::string> & listedStructs ) const;
  std::string generateStructCompareOperators( std::pair<std::string, StructureData> const & structure ) const;
  std::string generateStructConstructors( std::pair<std::string, StructureData> const & structData ) const;
  std::string generateStructConstructorsEnhanced( std::pair<std::string, StructureData> const & structData ) const;
  std::string generateStructConstructorArgument( bool listedArgument, MemberData const & memberData, bool withDefault ) const;
  std::string generateStructHashStructure( std::pair<std::string, StructureData> const & structure, std::set<std::string> & listedStructs ) const;
  std::string generateStructHashStructures() const;
  std::string generateStructHashSum( std::string const & structName, std::vector<MemberData> const & members ) const;
  std::string generateStructs() const;
  std::string generateStructure( std::pair<std::string, StructureData> const & structure ) const;
  std::string generateStructExtendsStructs() const;
  std::string
    generateStructExtendsStructs( std::vector<RequireData> const & requireData, std::set<std::string> & listedStructs, std::string const & title ) const;
  std::string generateStructForwardDeclarations() const;
  std::string
    generateStructForwardDeclarations( std::vector<RequireData> const & requireData, std::string const & title, std::set<std::string> & listedStructs ) const;
  std::tuple<std::string, std::string, std::string, std::string> generateStructMembers( std::pair<std::string, StructureData> const & structData ) const;
  std::string                         generateStructSetter( std::string const & structureName, std::vector<MemberData> const & memberData, size_t index ) const;
  std::string                         generateStructSubConstructor( std::pair<std::string, StructureData> const & structData ) const;
  std::string                         generateSuccessCheck( std::vector<std::string> const & successCodes ) const;
  std::string                         generateSuccessCodeList( std::vector<std::string> const & successCodes, bool enumerating ) const;
  std::string                         generateThrowResultException() const;
  std::string                         generateTypenameCheck( std::vector<size_t> const &               returnParams,
                                                             std::map<size_t, VectorParamData> const & vectorParams,
                                                             bool                                      definition,
                                                             std::vector<std::string> const &          dataTypes,
                                                             CommandFlavourFlags                       flavourFlags ) const;
  std::string                         generateUnion( std::pair<std::string, StructureData> const & structure ) const;
  std::string                         generateUniqueHandle( std::pair<std::string, HandleData> const & handleData ) const;
  std::string                         generateUniqueHandle( std::vector<RequireData> const & requireData, std::string const & title ) const;
  std::string                         generateUniqueHandles() const;
  std::string                         generateVectorSizeCheck( std::string const &                           name,
                                                               CommandData const &                           commandData,
                                                               size_t                                        initialSkipCount,
                                                               std::map<size_t, std::vector<size_t>> const & countToVectorMap,
                                                               std::set<size_t> const &                      skippedParams,
                                                               bool                                          onlyThrows ) const;
  std::pair<std::string, std::string> getParentTypeAndName( std::pair<std::string, HandleData> const & handle ) const;
  std::string                         getPlatform( std::string const & title ) const;
  std::pair<std::string, std::string> getPoolTypeAndName( std::string const & type ) const;
  std::string                         getProtect( EnumValueData const & evd ) const;
  std::string                         getProtectFromPlatform( std::string const & platform ) const;
  std::string                         getProtectFromTitle( std::string const & title ) const;
  std::string                         getProtectFromTitles( std::set<std::string> const & titles ) const;
  std::string                         getProtectFromType( std::string const & type ) const;
  std::string                         getVectorSize( std::vector<ParamData> const &            params,
                                                     std::map<size_t, VectorParamData> const & vectorParamIndices,
                                                     size_t                                    returnParam,
                                                     std::string const &                       returnParamType,
                                                     std::set<size_t> const &                  templatedParams ) const;
  bool                                hasLen( std::vector<MemberData> const & members, MemberData const & md ) const;
  bool                                hasParentHandle( std::string const & handle, std::string const & parent ) const;
  bool                                isDeviceCommand( CommandData const & commandData ) const;
  bool                                isHandleType( std::string const & type ) const;
  bool                                isLenByStructMember( std::string const & name, std::vector<ParamData> const & params ) const;
  bool                                isLenByStructMember( std::string const & name, ParamData const & param ) const;
  bool isMultiSuccessCodeConstructor( std::vector<std::map<std::string, CommandData>::const_iterator> const & constructorIts ) const;
  bool isParam( std::string const & name, std::vector<ParamData> const & params ) const;
  bool isStructMember( std::string const & name, std::vector<MemberData> const & memberData ) const;
  bool isStructureChainAnchor( std::string const & type ) const;
  std::pair<bool, std::map<size_t, std::vector<size_t>>> needsVectorSizeCheck( std::vector<ParamData> const &            params,
                                                                               std::map<size_t, VectorParamData> const & vectorParams,
                                                                               std::vector<size_t> const &               returnParams,
                                                                               std::set<size_t> const &                  singularParams ) const;
  void                                                   readCommands( tinyxml2::XMLElement const * element );
  void                                                   readCommandsCommand( tinyxml2::XMLElement const * element );
  std::pair<bool, ParamData>          readCommandsCommandParam( tinyxml2::XMLElement const * element, std::vector<ParamData> const & params );
  std::pair<std::string, std::string> readCommandsCommandProto( tinyxml2::XMLElement const * element );
  std::string                         readComment( tinyxml2::XMLElement const * element );
  void                                readEnums( tinyxml2::XMLElement const * element );
  void                                readEnumsConstant( tinyxml2::XMLElement const * element );
  void                                readEnumsEnum( tinyxml2::XMLElement const * element, std::map<std::string, EnumData>::iterator enumIt );
  void                                readExtensions( tinyxml2::XMLElement const * element );
  void                                readExtensionsExtension( tinyxml2::XMLElement const * element );
  void readExtensionsExtensionRequire( tinyxml2::XMLElement const * element, std::map<std::string, ExtensionData>::iterator extensionIt );
  void readExtensionsExtensionRequireRemove( tinyxml2::XMLElement const * element );
  void readExtensionsExtensionRequireType( tinyxml2::XMLElement const * element, std::string const & extensionName, RequireData & requireData );
  void readFeature( tinyxml2::XMLElement const * element );
  void readFeatureRequire( tinyxml2::XMLElement const * element, std::map<std::string, FeatureData>::iterator featureIt );
  void readFeatureRequireCommandRemove( tinyxml2::XMLElement const * element );
  void readFeatureRequireRemove( tinyxml2::XMLElement const * element );
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
  void                          readRequireCommand( tinyxml2::XMLElement const * element, std::string const & title, RequireData & requireData );
  void                          readRequireCommandRemove( tinyxml2::XMLElement const * element );
  void     readRequireEnum( tinyxml2::XMLElement const * element, std::string const & extensionName, std::vector<std::string> const & depends );
  void     readRequireEnumRemove( tinyxml2::XMLElement const * element );
  void     readRequireTypeRemove( tinyxml2::XMLElement const * element );
  void     readSPIRVCapabilities( tinyxml2::XMLElement const * element );
  void     readSPIRVCapabilitiesSPIRVCapability( tinyxml2::XMLElement const * element );
  void     readSPIRVCapabilitiesSPIRVCapabilityEnable( tinyxml2::XMLElement const * element );
  void     readSPIRVCapabilitiesSPIRVCapabilityEnableExtension( int xmlLine, std::map<std::string, std::string> const & attributes );
  void     readSPIRVCapabilitiesSPIRVCapabilityEnableProperty( int xmlLine, std::map<std::string, std::string> const & attributes );
  void     readSPIRVCapabilitiesSPIRVCapabilityEnableStruct( int xmlLine, std::map<std::string, std::string> const & attributes );
  void     readSPIRVCapabilitiesSPIRVCapabilityEnableVersion( int xmlLine, std::map<std::string, std::string> const & attributes );
  void     readSPIRVExtensions( tinyxml2::XMLElement const * element );
  void     readSPIRVExtensionsExtension( tinyxml2::XMLElement const * element );
  void     readSPIRVExtensionsExtensionEnable( tinyxml2::XMLElement const * element );
  void     readTags( tinyxml2::XMLElement const * element );
  void     readTagsTag( tinyxml2::XMLElement const * element );
  void     readTypes( tinyxml2::XMLElement const * element );
  void     readTypesType( tinyxml2::XMLElement const * element );
  void     readTypesTypeBasetype( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void     readTypesTypeBitmask( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void     readTypesTypeDefine( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void     readTypesTypeEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void     readTypesTypeFuncpointer( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void     readTypesTypeHandle( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void     readTypesTypeInclude( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void     readTypesTypeRequires( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
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
  std::map<std::string, DefineData>                                   m_defines;
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
  std::set<std::string>                                               m_removedCommands;
  std::map<std::string, EnumData>                                     m_removedEnums;
  std::set<std::string>                                               m_removedFeatures;
  std::set<std::string>                                               m_removedStructs;
  std::map<std::string, TypeData>                                     m_removedTypes;
  std::map<std::string, StructureData>                                m_structs;
  std::map<std::string, StructureAliasData>                           m_structsAliases;
  std::map<std::string, std::set<std::string>>                        m_structsAliasesInverse;
  std::set<std::string>                                               m_tags;
  std::map<std::string, TypeData>                                     m_types;
  std::string                                                         m_typesafeCheck;
  std::string                                                         m_version;
  std::string                                                         m_vulkanLicenseHeader;
};
