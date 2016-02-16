# Copyright(c) 2015-2016, NVIDIA CORPORATION. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  * Neither the name of NVIDIA CORPORATION nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.2)

project(VkCppGenerator)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/tinyxml/tinyxml.cpp")
  message(FATAL_ERROR, "TinyXML is missing. Please download the latest version from http://www.grinninglizard.com/tinyxml/index.html and put the sources in a folder named 'tinyxml'")
endif()

set(HEADERS
)

set(SOURCES
  main.cpp
)

set(TINYXML_SOURCES
  tinyxml/tinyxml.cpp
  tinyxml/tinyxmlerror.cpp
  tinyxml/tinyxmlparser.cpp
  tinyxml/tinystr.cpp
)

set(TINYXML_HEADERS
  tinyxml/tinyxml.h
  tinyxml/tinystr.h
)

source_group(headers FILES ${HEADERS})
source_group(sources FILES ${SOURCES})

source_group(TinyXML\\headers FILES ${TINYXML_HEADERS})
source_group(TinyXML\\sources FILES ${TINYXML_SOURCES})

add_executable(VkCppGenerator
  ${HEADERS}
  ${SOURCES}
  ${TINYXML_SOURCES}
  ${TINYXML_HEADERS}
)

set_property(TARGET VkCppGenerator PROPERTY CXX_STANDARD 11)

target_include_directories(VkCppGenerator PRIVATE "${CMAKE_SOURCE_DIR}/tinyxml")
