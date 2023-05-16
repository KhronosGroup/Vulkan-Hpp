// Copyright(c) 2023, NVIDIA CORPORATION. All rights reserved.
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
#include <string>
#include <tinyxml2.h>

class VideoHppGenerator
{
public:
  VideoHppGenerator( tinyxml2::XMLDocument const & document );

  void generateHppFile() const;

private:
  struct ConstantData
  {
    std::string value   = {};
    int         xmlLine = {};
  };

  struct DefineData
  {
    std::string require = {};
    int         xmlLine = {};
  };

  struct EnumValueData
  {
    std::string name    = {};
    std::string value   = {};
    int         xmlLine = {};
  };

  struct EnumData
  {
    std::vector<EnumValueData> values  = {};
    int                        xmlLine = {};
  };

  struct RequireData
  {
    std::map<std::string, ConstantData> constants = {};
    std::vector<std::string>            types     = {};
    int                                 xmlLine   = {};
  };

  struct ExtensionData
  {
    std::string depends     = {};
    std::string name        = {};
    RequireData requireData = {};
    int         xmlLine     = 0;
  };

  struct MemberData
  {
    TypeInfo                 type       = {};
    std::string              name       = {};
    std::vector<std::string> arraySizes = {};
    std::string              bitCount   = {};
    int                      xmlLine    = {};
  };

  struct StructureData
  {
    std::vector<MemberData> members = {};
    int                     xmlLine = {};
  };

private:
  void addImplicitlyRequiredTypes();
  std::vector<std::string>::iterator
       addImplicitlyRequiredTypes( std::map<std::string, TypeData>::iterator typeIt, ExtensionData & extensionData, std::vector<std::string>::iterator reqIt );
  void checkCorrectness() const;
  std::string generateEnum( std::pair<std::string, EnumData> const & enumData ) const;
  std::string generateEnums() const;
  std::string generateEnums( RequireData const & requireData, std::string const & title ) const;
  std::string generateStruct( std::pair<std::string, StructureData> const & structData ) const;
  std::string generateStructCompareOperators( std::pair<std::string, StructureData> const & structData ) const;
  std::string generateStructMembers( std::pair<std::string, StructureData> const & structData ) const;
  std::string generateStructs() const;
  std::string generateStructs( RequireData const & requireData, std::string const & title ) const;
  bool        isExtension( std::string const & name ) const;
  void        readEnums( tinyxml2::XMLElement const * element );
  void        readEnumsEnum( tinyxml2::XMLElement const * element, std::map<std::string, EnumData>::iterator enumIt );
  void        readExtension( tinyxml2::XMLElement const * element );
  void        readExtensionRequire( tinyxml2::XMLElement const * element, ExtensionData & extensionData );
  void        readExtensions( tinyxml2::XMLElement const * element );
  void        readRegistry( tinyxml2::XMLElement const * element );
  void        readRequireEnum( tinyxml2::XMLElement const * element, std::map<std::string, ConstantData> & constants );
  void        readRequireType( tinyxml2::XMLElement const * element, ExtensionData & extensionData );
  void        readStructMember( tinyxml2::XMLElement const * element, std::vector<MemberData> & members );
  void        readTypeDefine( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void        readTypeEnum( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void        readTypeInclude( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void        readTypeRequires( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void        readTypes( tinyxml2::XMLElement const * element );
  void        readTypeStruct( tinyxml2::XMLElement const * element, std::map<std::string, std::string> const & attributes );
  void        readTypesType( tinyxml2::XMLElement const * element );
  void        sortStructs();

private:
  std::string                             m_copyrightMessage;
  std::map<std::string, DefineData>       m_defines;
  std::map<std::string, EnumData>         m_enums;
  std::vector<ExtensionData>              m_extensions;
  std::map<std::string, ExternalTypeData> m_externalTypes;
  std::map<std::string, IncludeData>      m_includes;
  std::map<std::string, StructureData>    m_structs;
  std::map<std::string, TypeData>         m_types;
};