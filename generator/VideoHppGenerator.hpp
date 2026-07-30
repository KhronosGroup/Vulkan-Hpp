// SPDX-FileCopyrightText: 2023-2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "VideoXMLParser.hpp"
#include "XMLHelper.hpp"

#include <map>
#include <string>
#include <tinyxml2.h>

class VideoHppGenerator
{
public:
  VideoHppGenerator( VideoXML && videoXML );

  void generateCppmFile() const;
  void generateHppFile() const;

private:
  struct ConstantData
  {
    std::string type    = {};
    std::string value   = {};
    int         xmlLine = {};
  };

  struct EnumValueData
  {
    std::vector<std::pair<std::string, int>> aliases = {};
    std::string                              name    = {};
    std::string                              value   = {};
    int                                      xmlLine = {};
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
    std::string number      = {};
    std::string protect     = {};
    RequireData requireData = {};
    int         xmlLine     = 0;
  };

private:
  void addImplicitlyRequiredTypes();
  std::vector<std::string>::iterator
       addImplicitlyRequiredTypes( std::map<std::string, TypeData>::iterator typeIt, ExtensionData & extensionData, std::vector<std::string>::iterator reqIt );
  void checkCorrectness() const;
  void checkForError( bool condition, int line, std::string const & message ) const;
  void checkForWarning( bool condition, int line, std::string const & message ) const;
  std::string generateConstants() const;
  std::string generateConstants( ExtensionData const & extensionData ) const;
  std::string generateEnum( std::pair<std::string, EnumData> const & enumData ) const;
  std::string generateEnums() const;
  std::string generateEnums( ExtensionData const & extensionData ) const;
  std::string generateIncludes() const;
  std::string generateStruct( CategoryStruct const & categoryStruct ) const;
  std::string generateStructCompareOperators( CategoryStruct const & categoryStruct ) const;
  std::string generateStructMembers( CategoryStruct const & categoryStruct ) const;
  std::string generateStructs() const;
  std::string generateStructs( ExtensionData const & extensionData ) const;
  void        sortStructs();

private:
  std::map<std::string, EnumData> m_enums;
  std::vector<ExtensionData>      m_extensions;
  std::map<std::string, TypeData> m_types;
  VideoXML                        m_videoXML;
};
