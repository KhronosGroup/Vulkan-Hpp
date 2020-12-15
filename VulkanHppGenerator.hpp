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

  void appendBaseTypes( std::string & str ) const;
  void appendBitmasks( std::string & str ) const;
  void appendDispatchLoaderDynamic( std::string & str );  // use vkGet*ProcAddress to get function pointers
  void appendDispatchLoaderStatic( std::string & str );   // use exported symbols from loader
  void appendDispatchLoaderDefault(
    std::string & str );  // typedef to DispatchLoaderStatic or undefined type, based on VK_NO_PROTOTYPES
  void                appendEnums( std::string & str ) const;
  void                appendHandles( std::string & str );
  void                appendHandlesCommandDefinitions( std::string & str ) const;
  void                appendHashStructures( std::string & str ) const;
  void                appendResultExceptions( std::string & str ) const;
  void                appendStructs( std::string & str );
  void                appendStructureChainValidation( std::string & str );
  void                appendThrowExceptions( std::string & str ) const;
  void                appendIndexTypeTraits( std::string & str ) const;
  std::string const & getTypesafeCheck() const;
  std::string const & getVersion() const;
  std::string const & getVulkanLicenseHeader() const;

private:
  struct BaseTypeData
  {
    BaseTypeData( std::string const & type_, int line ) : type( type_ ), xmlLine( line ) {}

    std::string type;
    int         xmlLine;
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
    std::string              bitCount;
  };

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

  struct CommandAliasData
  {
    CommandAliasData( int line ) : xmlLine( line ) {}

    std::set<std::string> extensions;
    std::string           feature;
    int                   xmlLine;
  };

  struct CommandData
  {
    CommandData( int line ) : xmlLine( line ) {}

    std::map<std::string, CommandAliasData> aliasData;
    std::vector<std::string>                errorCodes;
    std::set<std::string>                   extensions;
    std::string                             feature;
    std::string                             handle;
    std::vector<ParamData>                  params;
    std::string                             returnType;
    std::vector<std::string>                successCodes;
    int                                     xmlLine;
  };

  struct EnumValueData
  {
    EnumValueData( int line, std::string const & vulkan, std::string const & vk, bool singleBit_ )
      : vulkanValue( vulkan ), vkValue( vk ), singleBit( singleBit_ ), xmlLine( line )
    {}

    std::string vulkanValue;
    std::string vkValue;
    bool        singleBit;
    int         xmlLine;
  };

  struct EnumData
  {
    void addEnumAlias( int line, std::string const & name, std::string const & alias, std::string const & vkName );
    void addEnumValue( int                 line,
                       std::string const & valueName,
                       bool                bitmask,
                       bool                bitpos,
                       std::string const & prefix,
                       std::string const & postfix,
                       std::string const & tag );

    std::string                                                alias;    // alias for this enum
    std::map<std::string, std::pair<std::string, std::string>> aliases;  // map from name to alias and vk-name
    bool                                                       isBitmask = false;
    std::vector<EnumValueData>                                 values;
  };

  struct ExtensionData
  {
    ExtensionData( int                 line,
                   std::string const & deprecatedBy_,
                   std::string const & obsoletedBy_,
                   std::string const & platform_,
                   std::string const & promotedTo_ )
      : deprecatedBy( deprecatedBy_ )
      , obsoletedBy( obsoletedBy_ )
      , platform( platform_ )
      , promotedTo( promotedTo_ )
      , xmlLine( line )
    {}

    std::string                deprecatedBy;
    std::string                obsoletedBy;
    std::string                platform;
    std::string                promotedTo;
    std::map<std::string, int> requirements;
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
    HandleData( std::vector<std::string> const & p, std::string const & objType, int line )
      : objTypeEnum( objType ), parents( p ), xmlLine( line )
    {}

    std::string              alias;
    std::set<std::string>    childrenHandles;
    std::set<std::string>    commands;
    std::string              deleteCommand;
    std::string              deletePool;
    std::string              objTypeEnum;
    std::vector<std::string> parents;
    int                      xmlLine;
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

    TypeCategory          category;
    std::set<std::string> extensions;
    std::string           feature;
  };

private:
  void appendArgumentPlainType( std::string & str, ParamData const & paramData ) const;
  void appendArguments( std::string &                    str,
                        CommandData const &              commandData,
                        size_t                           returnParamIndex,
                        size_t                           templateParamIndex,
                        std::map<size_t, size_t> const & vectorParamIndices,
                        bool                             twoStep,
                        bool                             firstCall,
                        size_t                           from,
                        size_t                           to ) const;
  void appendArgumentVector( std::string &     str,
                             size_t            paramIndex,
                             ParamData const & paramData,
                             size_t            returnParamIndex,
                             size_t            templateParamIndex,
                             bool              twoStep,
                             bool              firstCall ) const;
  void appendBitmask( std::string &                      os,
                      std::string const &                bitmaskName,
                      std::string const &                bitmaskType,
                      std::string const &                bitmaskAlias,
                      std::string const &                enumName,
                      std::vector<EnumValueData> const & enumValues ) const;
  void appendBitmaskToStringFunction( std::string &                      str,
                                      std::string const &                flagsName,
                                      std::string const &                enumName,
                                      std::vector<EnumValueData> const & enumValues ) const;
  void appendCall( std::string &                    str,
                   std::string const &              name,
                   CommandData const &              commandData,
                   size_t                           returnParamIndex,
                   size_t                           templateParamIndex,
                   std::map<size_t, size_t> const & vectorParamIndices,
                   bool                             twoStep,
                   bool                             firstCall ) const;
  void appendCommand( std::string &       str,
                      std::string const & name,
                      CommandData const & commandData,
                      bool                definition ) const;
  void appendCommandChained( std::string &                    str,
                             std::string const &              name,
                             CommandData const &              commandData,
                             bool                             definition,
                             std::map<size_t, size_t> const & vectorParamIndices,
                             size_t                           nonConstPointerIndex ) const;
  void appendCommandSingular( std::string &                    str,
                              std::string const &              name,
                              CommandData const &              commandData,
                              bool                             definition,
                              std::map<size_t, size_t> const & vectorParamIndices,
                              size_t                           returnParamIndex ) const;
  void appendCommandStandard( std::string &       str,
                              std::string const & name,
                              CommandData const & commandData,
                              bool                definition ) const;
  void appendCommandStandardAndEnhanced( std::string &                    str,
                                         std::string const &              name,
                                         CommandData const &              commandData,
                                         bool                             definition,
                                         std::map<size_t, size_t> const & vectorParamIndices,
                                         std::vector<size_t> const &      nonConstPointerParamIndices ) const;
  void
              appendCommandStandardEnhancedDeprecatedAllocator( std::string &                    str,
                                                                std::string const &              name,
                                                                CommandData const &              commandData,
                                                                bool                             definition,
                                                                std::map<size_t, size_t> const & vectorParamIndices,
                                                                std::vector<size_t> const & nonConstPointerParamIndices ) const;
  void        appendCommandStandardOrEnhanced( std::string &       str,
                                               std::string const & name,
                                               CommandData const & commandData,
                                               bool                definition ) const;
  void        appendCommandUnique( std::string &       str,
                                   std::string const & name,
                                   CommandData const & commandData,
                                   size_t              nonConstPointerIndex,
                                   bool                definition ) const;
  void        appendCommandVector( std::string &                     str,
                                   std::string const &               name,
                                   CommandData const &               commandData,
                                   bool                              definition,
                                   std::pair<size_t, size_t> const & vectorParamIndex,
                                   std::vector<size_t> const &       returnParamIndices ) const;
  void        appendCommandVectorChained( std::string &                    str,
                                          std::string const &              name,
                                          CommandData const &              commandData,
                                          bool                             definition,
                                          std::map<size_t, size_t> const & vectorParamIndices,
                                          std::vector<size_t> const &      returnParamIndices ) const;
  void        appendCommandVectorDeprecated( std::string &                    str,
                                             std::string const &              name,
                                             CommandData const &              commandData,
                                             std::map<size_t, size_t> const & vectorParamIndices,
                                             std::vector<size_t> const &      returnParamIndices,
                                             bool                             definition ) const;
  void        appendCommandVectorSingularUnique( std::string &                    str,
                                                 std::string const &              name,
                                                 CommandData const &              commandData,
                                                 std::map<size_t, size_t> const & vectorParamIndices,
                                                 size_t                           returnParamIndex,
                                                 bool                             definition ) const;
  void        appendCommandVectorUnique( std::string &                    str,
                                         std::string const &              name,
                                         CommandData const &              commandData,
                                         std::map<size_t, size_t> const & vectorParamIndices,
                                         size_t                           returnParamIndex,
                                         bool                             definition ) const;
  void        appendDispatchLoaderDynamicCommand( std::string &       str,
                                                  std::string &       emptyFunctions,
                                                  std::string &       deviceFunctions,
                                                  std::string &       deviceFunctionsInstance,
                                                  std::string &       instanceFunctions,
                                                  std::string const & commandName,
                                                  CommandData const & commandData );
  void        appendEnum( std::string & str, std::pair<std::string, EnumData> const & enumData ) const;
  void        appendEnumInitializer( std::string &                      str,
                                     TypeInfo const &                   type,
                                     std::vector<std::string> const &   arraySizes,
                                     std::vector<EnumValueData> const & values ) const;
  void        appendEnumToString( std::string & str, std::pair<std::string, EnumData> const & enumData ) const;
  std::string appendFunctionBodyEnhancedLocalReturnVariable( std::string &       str,
                                                             std::string const & indentation,
                                                             CommandData const & commandData,
                                                             size_t              returnParamIndex,
                                                             std::string const & enhancedReturnType,
                                                             bool                withAllocator ) const;
  void        appendFunctionBodyEnhancedMultiVectorSizeCheck( std::string &                    str,
                                                              std::string const &              indentation,
                                                              std::string const &              name,
                                                              CommandData const &              commandData,
                                                              size_t                           returnParamIndex,
                                                              std::map<size_t, size_t> const & vectorParamIndices ) const;
  void        appendFunctionBodyEnhancedReturnResultValue( std::string &       str,
                                                           std::string const & indentation,
                                                           std::string const & returnName,
                                                           std::string const & name,
                                                           CommandData const & commandData,
                                                           size_t              returnParamIndex,
                                                           bool                twoStep ) const;
  void        appendFunctionBodyEnhancedTwoStep( std::string &                    str,
                                                 std::string const &              indentation,
                                                 std::string const &              name,
                                                 CommandData const &              commandData,
                                                 size_t                           returnParamIndex,
                                                 size_t                           templateParamIndex,
                                                 std::map<size_t, size_t> const & vectorParamIndices,
                                                 std::string const &              returnName ) const;
  bool        appendFunctionHeaderArgumentEnhanced( std::string &                    str,
                                                    ParamData const &                param,
                                                    size_t                           paramIndex,
                                                    std::map<size_t, size_t> const & vectorParamIndices,
                                                    bool                             skip,
                                                    bool                             argEncountered,
                                                    bool                             isTemplateParam ) const;
  void        appendFunctionHeaderArgumentEnhancedVector( std::string &       str,
                                                          ParamData const &   param,
                                                          std::string const & strippedParameterName,
                                                          bool                hasSizeParam,
                                                          bool                isTemplateParam ) const;
  void        appendHandle( std::string & str, std::pair<std::string, HandleData> const & handle );
  void        appendStruct( std::string & str, std::pair<std::string, StructureData> const & structure );
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
  void        appendType( std::string & str, std::string const & typeName );
  void        appendUnion( std::string & str, std::pair<std::string, StructureData> const & structure ) const;
  void        appendUniqueTypes( std::string &                 str,
                                 std::string const &           parentType,
                                 std::set<std::string> const & childrenTypes ) const;
  std::string constructArgumentListEnhanced( std::vector<ParamData> const & params,
                                             std::set<size_t> const &       skippedParams,
                                             size_t                         singularParam,
                                             bool                           definition,
                                             bool                           withAllocators,
                                             bool                           structureChain ) const;
  std::string constructArgumentListStandard( std::vector<ParamData> const & params,
                                             std::set<size_t> const &       skippedParams ) const;
  std::string constructCallArgumentsEnhanced( std::string const &            handle,
                                              std::vector<ParamData> const & params,
                                              bool                           nonConstPointerAsNullptr,
                                              size_t                         singularParamIndex ) const;
  std::string constructCallArgumentsStandard( std::string const & handle, std::vector<ParamData> const & params ) const;
  std::string constructCommandResult( std::string const &              name,
                                      CommandData const &              commandData,
                                      bool                             definition,
                                      std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string constructCommandResultEnumerate( std::string const &               name,
                                               CommandData const &               commandData,
                                               bool                              definition,
                                               std::pair<size_t, size_t> const & vectorParamIndices,
                                               bool                              withAllocators ) const;
  std::string constructCommandResultEnumerateChained( std::string const &               name,
                                                      CommandData const &               commandData,
                                                      bool                              definition,
                                                      std::pair<size_t, size_t> const & vectorParamIndex,
                                                      std::vector<size_t> const &       returnParamIndices,
                                                      bool                              withAllocator ) const;
  std::string constructCommandResultEnumerateTwoVectors( std::string const &              name,
                                                         CommandData const &              commandData,
                                                         bool                             definition,
                                                         std::map<size_t, size_t> const & vectorParamIndices,
                                                         std::vector<size_t> const &      returnParamIndices,
                                                         bool                             withAllocators ) const;
  std::string constructCommandResultEnumerateTwoVectorsDeprecated( std::string const &              name,
                                                                   CommandData const &              commandData,
                                                                   bool                             definition,
                                                                   std::map<size_t, size_t> const & vectorParamIndices,
                                                                   bool withAllocators ) const;
  std::string constructCommandResultGetChain( std::string const & name,
                                              CommandData const & commandData,
                                              bool                definition,
                                              size_t              nonConstPointerIndex ) const;
  std::string constructCommandResultGetHandleUnique( std::string const & name,
                                                     CommandData const & commandData,
                                                     bool                definition,
                                                     size_t              nonConstPointerIndex ) const;
  std::string constructCommandResultGetTwoVectors( std::string const &              name,
                                                   CommandData const &              commandData,
                                                   bool                             definition,
                                                   std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string constructCommandResultGetValue( std::string const & name,
                                              CommandData const & commandData,
                                              bool                definition,
                                              size_t              nonConstPointerIndex ) const;
  std::string constructCommandResultGetValueDeprecated( std::string const &              name,
                                                        CommandData const &              commandData,
                                                        bool                             definition,
                                                        std::map<size_t, size_t> const & vectorParamIndices,
                                                        size_t                           returnParamIndex ) const;
  std::string constructCommandResultGetVector( std::string const &              name,
                                               CommandData const &              commandData,
                                               bool                             definition,
                                               std::map<size_t, size_t> const & vectorParamIndices,
                                               size_t                           returnParamIndex ) const;
  std::string constructCommandResultGetVectorAndValue( std::string const &              name,
                                                       CommandData const &              commandData,
                                                       bool                             definition,
                                                       std::map<size_t, size_t> const & vectorParamIndices,
                                                       std::vector<size_t> const &      returnParamIndex,
                                                       bool                             withAllocator ) const;
  std::string constructCommandResultGetVectorDeprecated( std::string const &              name,
                                                         CommandData const &              commandData,
                                                         bool                             definition,
                                                         std::map<size_t, size_t> const & vectorParamIndices,
                                                         size_t                           returnParamIndex ) const;
  std::string constructCommandResultGetVectorOfHandles( std::string const &              name,
                                                        CommandData const &              commandData,
                                                        bool                             definition,
                                                        std::map<size_t, size_t> const & vectorParamIndices,
                                                        size_t                           returnParamIndex,
                                                        bool                             withAllocator ) const;
  std::string constructCommandResultGetVectorOfHandlesSingular( std::string const &              name,
                                                                CommandData const &              commandData,
                                                                bool                             definition,
                                                                std::map<size_t, size_t> const & vectorParamIndices,
                                                                size_t returnParamIndex ) const;
  std::string constructCommandResultGetVectorOfHandlesUnique( std::string const &              name,
                                                              CommandData const &              commandData,
                                                              bool                             definition,
                                                              std::map<size_t, size_t> const & vectorParamIndices,
                                                              size_t                           returnParamIndex,
                                                              bool                             withAllocator ) const;
  std::string
              constructCommandResultGetVectorOfHandlesUniqueSingular( std::string const &              name,
                                                                      CommandData const &              commandData,
                                                                      bool                             definition,
                                                                      std::map<size_t, size_t> const & vectorParamIndices,
                                                                      size_t                           returnParamIndex ) const;
  std::string constructCommandResultGetVectorSingular( std::string const &              name,
                                                       CommandData const &              commandData,
                                                       bool                             definition,
                                                       std::map<size_t, size_t> const & vectorParamIndices,
                                                       size_t                           returnParamIndex ) const;
  std::string
    constructCommandStandard( std::string const & name, CommandData const & commandData, bool definition ) const;
  std::string constructCommandType( std::string const & name, CommandData const & commandData, bool definition ) const;
  std::string constructCommandVoid( std::string const &              name,
                                    CommandData const &              commandData,
                                    bool                             definition,
                                    std::map<size_t, size_t> const & vectorParamIndices ) const;
  std::string constructCommandVoidEnumerate( std::string const &               name,
                                             CommandData const &               commandData,
                                             bool                              definition,
                                             std::pair<size_t, size_t> const & vectorParamIndex,
                                             std::vector<size_t> const &       returnParamIndices,
                                             bool                              withAllocators ) const;
  std::string constructCommandVoidEnumerateChained( std::string const &               name,
                                                    CommandData const &               commandData,
                                                    bool                              definition,
                                                    std::pair<size_t, size_t> const & vectorParamIndex,
                                                    std::vector<size_t> const &       returnParamIndices,
                                                    bool                              withAllocators ) const;
  std::string constructCommandVoidGetChain( std::string const & name,
                                            CommandData const & commandData,
                                            bool                definition,
                                            size_t              nonConstPointerIndex ) const;
  std::string constructCommandVoidGetValue( std::string const &              name,
                                            CommandData const &              commandData,
                                            bool                             definition,
                                            std::map<size_t, size_t> const & vectorParamIndices,
                                            size_t                           returnParamIndex ) const;
  std::string constructConstexprString( std::pair<std::string, StructureData> const & structData ) const;
  std::string constructFunctionBodyEnhanced( std::string const &              indentation,
                                             std::string const &              name,
                                             CommandData const &              commandData,
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
  std::string constructNoDiscardStandard( CommandData const & commandData ) const;
  std::string constructReturnType( CommandData const & commandData, std::string const & baseType ) const;
  std::string constructSuccessCheck( std::vector<std::string> const & successCodes ) const;
  std::string constructSuccessCodeList( std::vector<std::string> const & successCodes ) const;
  std::string constructVectorSizeCheck( std::string const &                           name,
                                        CommandData const &                           commandData,
                                        std::map<size_t, std::vector<size_t>> const & countToVectorMap,
                                        std::set<size_t> const &                      skippedParams ) const;
  void        checkCorrectness();
  bool        containsArray( std::string const & type ) const;
  bool        containsUnion( std::string const & type ) const;
  size_t      determineDefaultStartIndex( std::vector<ParamData> const & params,
                                          std::set<size_t> const &       skippedParams ) const;
  std::string determineEnhancedReturnType( CommandData const & commandData,
                                           size_t              returnParamIndex,
                                           bool                isStructureChain ) const;
  size_t      determineReturnParamIndex( CommandData const &              commandData,
                                         std::map<size_t, size_t> const & vectorParamIndices,
                                         bool                             twoStep ) const;
  std::set<size_t>         determineSkippedParams( std::string const &              handleType,
                                                   std::vector<ParamData> const &   params,
                                                   std::map<size_t, size_t> const & vectorParamIndices,
                                                   std::vector<size_t> const &      returnParamIndex,
                                                   bool                             singular ) const;
  std::string              determineSubStruct( std::pair<std::string, StructureData> const & structure ) const;
  std::vector<size_t>      determineConstPointerParamIndices( std::vector<ParamData> const & params ) const;
  std::vector<size_t>      determineNonConstPointerParamIndices( std::vector<ParamData> const & params ) const;
  std::map<size_t, size_t> determineVectorParamIndicesNew( std::vector<ParamData> const & params ) const;
  std::string
                                      generateLenInitializer( std::vector<MemberData>::const_iterator                                        mit,
                                                              std::map<std::vector<MemberData>::const_iterator,
                                     std::vector<std::vector<MemberData>::const_iterator>>::const_iterator litit,
                                                              bool mutualExclusiveLens ) const;
  std::pair<std::string, std::string> generateProtection( std::string const &           feature,
                                                          std::set<std::string> const & extension ) const;
  std::pair<std::string, std::string> generateProtection( std::string const & type, bool isAliased ) const;
  std::string           generateSizeCheck( std::vector<std::vector<MemberData>::const_iterator> const & arrayIts,
                                           std::string const &                                          structName,
                                           std::string const &                                          prefix,
                                           bool mutualExclusiveLens ) const;
  std::set<std::string> getPlatforms( std::set<std::string> const & extensions ) const;
  std::pair<std::string, std::string> getPoolTypeAndName( std::string const & type ) const;
  std::string                         getVectorSize( std::vector<ParamData> const &   params,
                                                     std::map<size_t, size_t> const & vectorParamIndices,
                                                     size_t                           returnParamIndex ) const;
  bool                                isHandleType( std::string const & type ) const;
  bool isLenByStructMember( std::string const & name, std::vector<ParamData> const & params ) const;
  bool isLenByStructMember( std::string const & name, ParamData const & param ) const;
  bool isParam( std::string const & name, std::vector<ParamData> const & params ) const;
  bool isStructureChainAnchor( std::string const & type ) const;
  bool needsComplexBody( CommandData const & commandData ) const;
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
  void readEnum( tinyxml2::XMLElement const * element,
                 EnumData &                   enumData,
                 bool                         bitmask,
                 std::string const &          prefix,
                 std::string const &          postfix );
  void readEnum( tinyxml2::XMLElement const *               element,
                 std::map<std::string, std::string> const & attributes,
                 EnumData &                                 enumData,
                 bool                                       bitmask,
                 std::string const &                        prefix,
                 std::string const &                        postfix );
  void readEnumAlias( tinyxml2::XMLElement const *               element,
                      std::map<std::string, std::string> const & attributes,
                      EnumData &                                 enumData,
                      bool                                       bitmask,
                      std::string const &                        prefix,
                      std::string const &                        postfix );
  void readEnumConstant( tinyxml2::XMLElement const * element );
  void readEnums( tinyxml2::XMLElement const * element );
  void readExtension( tinyxml2::XMLElement const * element );
  void readExtensionDisabledCommand( tinyxml2::XMLElement const * element );
  void readExtensionDisabledEnum( std::string const & extensionName, tinyxml2::XMLElement const * element );
  void readExtensionDisabledRequire( std::string const & extensionName, tinyxml2::XMLElement const * element );
  void readExtensionDisabledType( tinyxml2::XMLElement const * element );
  void readExtensionRequire( tinyxml2::XMLElement const * element,
                             std::string const &          extension,
                             std::string const &          tag,
                             std::map<std::string, int> & requirements );
  void readExtensionRequireCommand( tinyxml2::XMLElement const * element, std::string const & extension );
  void readExtensionRequireType( tinyxml2::XMLElement const * element, std::string const & extension );
  void readExtensions( tinyxml2::XMLElement const * element );
  void readFeature( tinyxml2::XMLElement const * element );
  void readFeatureRequire( tinyxml2::XMLElement const * element, std::string const & feature );
  void readFeatureRequireCommand( tinyxml2::XMLElement const * element, std::string const & feature );
  void readFeatureRequireType( tinyxml2::XMLElement const * element, std::string const & feature );
  void readFuncpointer( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void readHandle( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  std::pair<NameData, TypeInfo> readNameAndType( tinyxml2::XMLElement const * elements );
  void                          readPlatform( tinyxml2::XMLElement const * element );
  void                          readPlatforms( tinyxml2::XMLElement const * element );
  void                          readRegistry( tinyxml2::XMLElement const * element );
  void                          readRequireEnum( tinyxml2::XMLElement const * element, std::string const & tag );
  void                          readRequireEnum( tinyxml2::XMLElement const *               element,
                                                 std::map<std::string, std::string> const & attributes,
                                                 std::string const &                        tag );
  void                          readRequireEnumAlias( tinyxml2::XMLElement const *               element,
                                                      std::map<std::string, std::string> const & attributes,
                                                      std::string const &                        tag );
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
  void readTypes( tinyxml2::XMLElement const * element );
  void registerDeleter( std::string const & name, std::pair<std::string, CommandData> const & commandData );
  void setVulkanLicenseHeader( int line, std::string const & comment );
  std::string toString( TypeCategory category );

private:
  std::map<std::string, BaseTypeData>    m_baseTypes;
  std::map<std::string, BitmaskData>     m_bitmasks;
  std::map<std::string, CommandData>     m_commands;
  std::set<std::string>                  m_constants;
  std::set<std::string>                  m_defines;
  std::map<std::string, EnumData>        m_enums;
  std::set<std::string>                  m_extendedStructs;  // structs which are referenced by the structextends tag
  std::map<std::string, ExtensionData>   m_extensions;
  std::map<std::string, std::string>     m_features;
  std::map<std::string, FuncPointerData> m_funcPointers;
  std::map<std::string, HandleData>      m_handles;
  std::set<std::string>                  m_includes;
  std::set<std::string>                  m_listedTypes;
  std::set<std::string>                  m_listingTypes;
  std::map<std::string, PlatformData>    m_platforms;
  std::map<std::string, std::string>     m_structureAliases;
  std::map<std::string, StructureData>   m_structures;
  std::set<std::string>                  m_tags;
  std::map<std::string, TypeData>        m_types;
  std::string                            m_typesafeCheck;
  std::string                            m_version;
  std::string                            m_vulkanLicenseHeader;
};

const size_t INVALID_INDEX = (size_t)~0;
