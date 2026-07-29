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

struct Text
{
  std::string text    = {};
  int         xmlLine = {};
};

struct Types
{
  std::vector<CategoryDefine>  defines       = {};
  std::vector<CategoryEnum>    enums         = {};
  std::vector<ExternalType>    externalTypes = {};
  std::vector<CategoryInclude> includes      = {};
  std::vector<CategoryStruct>  structs       = {};
  int                          xmlLine       = {};
};

struct VideoXML
{
  std::string copyrightMessage = {};
  Types       types            = {};
  int         xmlLine          = {};
};

Types parseTypes( tinyxml2::XMLElement const * element );

VideoXML parseVideoXML( tinyxml2::XMLDocument const & document );
