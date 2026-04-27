// Copyright(c) 2026, NVIDIA CORPORATION. All rights reserved.
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

#include "XMLHelper.hpp"

#include <map>
#include <regex>
#include <string>
#include <tinyxml2.h>
#include <vector>

struct BaseType
{
  Type type    = {};
  int  xmlLine = {};
};

struct Bitmask
{
  std::map<std::string, int> aliases = {};
  std::string                require = {};
  Type                       type    = {};
  int                        xmlLine = {};
};

struct CategoryAlias
{
  std::string alias   = {};
  std::string name    = {};
  int         xmlLine = {};
};

struct Param
{
  std::string              altLen         = {};
  std::vector<std::string> api            = {};
  std::vector<std::string> arraySizes     = {};
  std::string              externSync     = {};
  std::string              len            = {};
  std::string              name           = {};
  std::string              noAutoValidity = {};
  std::string              objectType     = {};
  std::vector<std::string> optional       = {};
  std::string              stride         = {};
  Type                     type           = {};
  std::string              validStructs   = {};
  int                      xmlLine        = {};
};

struct Command
{
  std::string                allowNoQueues        = {};
  std::map<std::string, int> aliases              = {};
  std::vector<std::string>   api                  = {};
  std::vector<std::string>   cmdBufferLevel       = {};
  std::string                conditionalRendering = {};
  std::vector<std::string>   errorCodes           = {};
  std::vector<std::string>   export_              = {};
  std::vector<Param>         params               = {};
  std::vector<std::string>   queues               = {};
  std::string                renderPass           = {};
  Type                       returnType           = {};
  std::vector<std::string>   successCodes         = {};
  std::vector<std::string>   tasks                = {};
  std::string                videoCoding          = {};
  int                        xmlLine              = {};
};

struct Constant
{
  std::string type    = {};
  std::string value   = {};
  int         xmlLine = {};
};

struct Comment
{
  std::string text    = {};
  int         xmlLine = {};
};

struct Define
{
  std::vector<std::string> macro   = {};
  std::string              require = {};
  int                      xmlLine = {};
};

struct EnumValueAlias
{
  std::string api        = {};
  std::string deprecated = {};
  int         xmlLine    = {};
};

struct EnumValue
{
  std::map<std::string, EnumValueAlias> aliases = {};
  std::string                           bitpos  = {};
  std::string                           name    = {};
  std::string                           value   = {};
  int                                   xmlLine = {};
};

struct Enum
{
  std::map<std::string, int> aliases  = {};
  std::string                bitwidth = {};
  std::string                type     = {};
  std::vector<EnumValue>     values   = {};
  int                        xmlLine  = {};
};

struct EnumValues
{
  std::string            bitwidth = {};
  std::string            type     = {};
  std::vector<EnumValue> values   = {};
  int                    xmlLine  = {};
};

struct Enums
{
  std::map<std::string, Constant>   constants  = {};
  std::map<std::string, EnumValues> enumValues = {};
};

struct ExternalType
{
  std::string includedBy = {};
  int         xmlLine    = {};
};

struct FuncPointerParam
{
  std::string name    = {};
  Type        type    = {};
  int         xmlLine = {};
};

struct FuncPointer
{
  std::vector<FuncPointerParam> params     = {};
  std::string                   require    = {};
  Type                          returnType = {};
  int                           xmlLine    = {};
};

struct Handle
{
  std::map<std::string, int> aliases     = {};
  std::string                objTypeEnum = {};
  std::string                parent      = {};
  Type                       type        = {};
  int                        xmlLine     = {};
};

struct MacroVisitor final : tinyxml2::XMLVisitor
{
  // comments, then name, then parameters and definition together, because that's how they appear in the xml!
  // guaranteed to be 3 elements long
  std::vector<std::string> macro;

  bool Visit( tinyxml2::XMLText const & text ) override
  {
    if ( auto const nodeText = text.Value(); nodeText != nullptr )
    {
      macro.emplace_back( nodeText );
    }
    return true;
  }
};

struct Platform
{
  std::string comment = {};
  std::string protect = {};
  int         xmlLine = {};
};

struct StructMember
{
  std::string              altLen         = {};
  std::vector<std::string> api            = {};
  std::vector<std::string> arraySizes     = {};
  std::string              bitCount       = {};
  std::string              deprecated     = {};
  std::string              externSync     = {};
  std::string              featureLink    = {};
  std::vector<std::string> len            = {};
  std::vector<std::string> limitType      = {};
  std::string              name           = {};
  std::string              noAutoValidity = {};
  std::string              objectType     = {};
  std::vector<std::string> optional       = {};
  std::string              selector       = {};
  Type                     type           = {};
  std::string              values         = {};
  int                      xmlLine        = {};
};

struct Struct
{
  std::map<std::string, int> aliases           = {};
  std::string                allowDuplicate    = {};
  std::string                requiredLimitType = {};
  std::string                returnedOnly      = {};
  std::vector<StructMember>  members           = {};
  std::vector<std::string>   structExtends     = {};
  int                        xmlLine           = {};
};

struct Tag
{
  std::string author  = {};
  std::string contact = {};
  int         xmlLine = {};
};

struct UnionMember
{
  std::vector<std::string> arraySizes     = {};
  std::string              len            = {};
  std::string              name           = {};
  std::string              noAutoValidity = {};
  std::string              optional       = {};
  std::vector<std::string> selection      = {};
  Type                     type           = {};
  int                      xmlLine        = {};
};

struct Union
{
  std::vector<UnionMember> members      = {};
  std::string              returnedOnly = {};
  int                      xmlLine      = {};
};

struct Types
{
  std::map<std::string, BaseType>     baseTypes     = {};
  std::map<std::string, Bitmask>      bitmasks      = {};
  std::map<std::string, Define>       defines       = {};
  std::map<std::string, Enum>         enums         = {};
  std::map<std::string, ExternalType> externalTypes = {};
  std::map<std::string, FuncPointer>  funcPointers  = {};
  std::map<std::string, Handle>       handles       = {};
  std::map<std::string, int>          includes      = {};
  std::map<std::string, Struct>       structs       = {};
  std::map<std::string, Union>        unions        = {};

  std::vector<CategoryAlias> structAliases = {};
};

struct Vkxml
{
  std::map<std::string, BaseType>     baseTypes     = {};
  std::map<std::string, Bitmask>      bitmasks      = {};
  std::map<std::string, Command>      commands      = {};  // lists only commands of the requested export
  std::map<std::string, Constant>     constants     = {};
  Comment                             copyright     = {};
  std::map<std::string, Define>       defines       = {};
  std::map<std::string, Enum>         enums         = {};
  std::map<std::string, ExternalType> externalTypes = {};
  std::map<std::string, FuncPointer>  funcPointers  = {};
  std::map<std::string, Handle>       handles       = {};
  std::map<std::string, int>          includes      = {};
  std::map<std::string, Platform>     platforms     = {};
  std::map<std::string, Struct>       structs       = {};
  std::map<std::string, Tag>          tags          = {};
  std::map<std::string, Union>        unions        = {};
};

Vkxml parseVkXml( tinyxml2::XMLDocument const & document, std::string const & api );
