// SPDX-FileCopyrightText: 2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "XMLHelper.hpp"

#include <map>
#include <string>
#include <tinyxml2.h>
#include <vector>

struct CategoryDefine
{
  std::string name    = {};
  std::string require = {};
  std::string type    = {};
  int         xmlLine = {};
};

struct EnumValue
{
  std::string name    = {};
  std::string value   = {};
  std::string comment = {};
  std::string alias   = {};
  int         xmlLine = {};
};

struct Enum
{
  std::string            name    = {};
  std::vector<EnumValue> values  = {};
  int                    xmlLine = {};
};

struct StructMember
{
  Type                     type       = {};
  std::string              name       = {};
  std::vector<std::string> arraySizes = {};
  std::string              bitCount   = {};
  std::string              len        = {};
  std::string              optional   = {};
  int                      xmlLine    = {};
};

struct CategoryStruct
{
  std::string               name    = {};
  std::string               comment = {};
  std::vector<StructMember> members = {};
  std::string               require = {};
  int                       xmlLine = {};
};

struct Constant
{
  std::string name    = {};
  std::string type    = {};
  std::string value   = {};
  int         xmlLine = {};
};

struct Require
{
  std::vector<Constant>    enums   = {};
  NameElement              include = {};
  std::vector<NameElement> types   = {};
  int                      xmlLine = {};
};

struct Extension
{
  std::string name      = {};
  std::string number    = {};
  std::string protect   = {};
  Require     require   = {};
  std::string supported = {};
  int         xmlLine   = {};
};

struct Text
{
  std::string text    = {};
  int         xmlLine = {};
};

struct Types
{
  std::vector<CategoryDefine> defines   = {};
  std::vector<CategoryEnum>   enums     = {};
  std::vector<TypeExternal>   externals = {};
  std::vector<TypeInclude>    includes  = {};
  std::vector<CategoryStruct> structs   = {};
  int                         xmlLine   = {};
};

struct VideoXML
{
  std::string            copyrightMessage = {};
  std::vector<Enum>      enums            = {};
  std::vector<Extension> extensions       = {};
  Types                  types            = {};
  int                    xmlLine          = {};
};

VideoXML parseVideoXML( tinyxml2::XMLDocument const & document );
