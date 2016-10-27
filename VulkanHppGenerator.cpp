// Copyright(c) 2015-2016, NVIDIA CORPORATION. All rights reserved.
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

#include <cassert>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <exception>

#include <tinyxml2.h>

const std::string exceptionHeader(
  "#if defined(_MSC_VER) && (_MSC_VER == 1800)\n"
  "# define noexcept _NOEXCEPT\n"
  "#endif\n"
  "\n"
  "  class ErrorCategoryImpl : public std::error_category\n"
  "  {\n"
  "    public:\n"
  "    virtual const char* name() const noexcept override { return \"vk::Result\"; }\n"
  "    virtual std::string message(int ev) const override { return to_string(static_cast<Result>(ev)); }\n"
  "  };\n"
  "\n"
  "#if defined(_MSC_VER) && (_MSC_VER == 1800)\n"
  "# undef noexcept\n"
  "#endif\n"
  "\n"
  "  VULKAN_HPP_INLINE const std::error_category& errorCategory()\n"
  "  {\n"
  "    static ErrorCategoryImpl instance;\n"
  "    return instance;\n"
  "  }\n"
  "\n"
  "  VULKAN_HPP_INLINE std::error_code make_error_code(Result e)\n"
  "  {\n"
  "    return std::error_code(static_cast<int>(e), errorCategory());\n"
  "  }\n"
  "\n"
  "  VULKAN_HPP_INLINE std::error_condition make_error_condition(Result e)\n"
  "  {\n"
  "    return std::error_condition(static_cast<int>(e), errorCategory());\n"
  "  }\n"
  "\n"
  );

const std::string flagsHeader(
"  template <typename FlagBitsType> struct FlagTraits\n"
"  {\n"
"    enum { allFlags = 0 };\n"
"  };\n"
"\n"
"  template <typename BitType, typename MaskType = VkFlags>\n"
"  class Flags\n"
"  {\n"
"  public:\n"
"    Flags()\n"
"      : m_mask(0)\n"
"    {\n"
"    }\n"
"\n"
"    Flags(BitType bit)\n"
"      : m_mask(static_cast<MaskType>(bit))\n"
"    {\n"
"    }\n"
"\n"
"    Flags(Flags<BitType> const& rhs)\n"
"      : m_mask(rhs.m_mask)\n"
"    {\n"
"    }\n"
"\n"
"    Flags<BitType> & operator=(Flags<BitType> const& rhs)\n"
"    {\n"
"      m_mask = rhs.m_mask;\n"
"      return *this;\n"
"    }\n"
"\n"
"    Flags<BitType> & operator|=(Flags<BitType> const& rhs)\n"
"    {\n"
"      m_mask |= rhs.m_mask;\n"
"      return *this;\n"
"    }\n"
"\n"
"    Flags<BitType> & operator&=(Flags<BitType> const& rhs)\n"
"    {\n"
"      m_mask &= rhs.m_mask;\n"
"      return *this;\n"
"    }\n"
"\n"
"    Flags<BitType> & operator^=(Flags<BitType> const& rhs)\n"
"    {\n"
"      m_mask ^= rhs.m_mask;\n"
"      return *this;\n"
"    }\n"
"\n"
"    Flags<BitType> operator|(Flags<BitType> const& rhs) const\n"
"    {\n"
"      Flags<BitType> result(*this);\n"
"      result |= rhs;\n"
"      return result;\n"
"    }\n"
"\n"
"    Flags<BitType> operator&(Flags<BitType> const& rhs) const\n"
"    {\n"
"      Flags<BitType> result(*this);\n"
"      result &= rhs;\n"
"      return result;\n"
"    }\n"
"\n"
"    Flags<BitType> operator^(Flags<BitType> const& rhs) const\n"
"    {\n"
"      Flags<BitType> result(*this);\n"
"      result ^= rhs;\n"
"      return result;\n"
"    }\n"
"\n"
"    bool operator!() const\n"
"    {\n"
"      return !m_mask;\n"
"    }\n"
"\n"
"    Flags<BitType> operator~() const\n"
"    {\n"
"      Flags<BitType> result(*this);\n"
"      result.m_mask ^= FlagTraits<BitType>::allFlags;\n"
"      return result;\n"
"    }\n"
"\n"
"    bool operator==(Flags<BitType> const& rhs) const\n"
"    {\n"
"      return m_mask == rhs.m_mask;\n"
"    }\n"
"\n"
"    bool operator!=(Flags<BitType> const& rhs) const\n"
"    {\n"
"      return m_mask != rhs.m_mask;\n"
"    }\n"
"\n"
"    explicit operator bool() const\n"
"    {\n"
"      return !!m_mask;\n"
"    }\n"
"\n"
"    explicit operator MaskType() const\n"
"    {\n"
"        return m_mask;\n"
"    }\n"
"\n"
"  private:\n"
"    MaskType  m_mask;\n"
"  };\n"
"  \n"
"  template <typename BitType>\n"
"  Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags)\n"
"  {\n"
"    return flags | bit;\n"
"  }\n"
"  \n"
"  template <typename BitType>\n"
"  Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags)\n"
"  {\n"
"    return flags & bit;\n"
"  }\n"
"  \n"
"  template <typename BitType>\n"
"  Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags)\n"
"  {\n"
"    return flags ^ bit;\n"
"  }\n"
"\n"
);

std::string const optionalClassHeader = R"(
  template <typename RefType>
  class Optional
  {
  public:
    Optional(RefType & reference) { m_ptr = &reference; }
    Optional(RefType * ptr) { m_ptr = ptr; }
    Optional(std::nullptr_t) { m_ptr = nullptr; }

    operator RefType*() const { return m_ptr; }
    RefType const* operator->() const { return m_ptr; }
    explicit operator bool() const { return !!m_ptr; }

  private:
    RefType *m_ptr;
  };

)";

std::string const arrayProxyHeader = (
  "#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE\n"
  "  template <typename T>\n"
  "  class ArrayProxy\n"
  "  {\n"
  "  public:\n"
  "    ArrayProxy(std::nullptr_t)\n"
  "      : m_count(0)\n"
  "      , m_ptr(nullptr)\n"
  "    {}\n"
  "\n"
  "    ArrayProxy(T & ptr)\n"
  "      : m_count(1)\n"
  "      , m_ptr(&ptr)\n"
  "    {}\n"
  "\n"
  "    ArrayProxy(uint32_t count, T * ptr)\n"
  "      : m_count(count)\n"
  "      , m_ptr(ptr)\n"
  "    {}\n"
  "\n"
  "    template <size_t N>\n"
  "    ArrayProxy(std::array<typename std::remove_const<T>::type, N> & data)\n"
  "      : m_count(N)\n"
  "      , m_ptr(data.data())\n"
  "    {}\n"
  "\n"
  "    template <size_t N>\n"
  "    ArrayProxy(std::array<typename std::remove_const<T>::type, N> const& data)\n"
  "      : m_count(N)\n"
  "      , m_ptr(data.data())\n"
  "    {}\n"
  "\n"
  "    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>\n"
  "    ArrayProxy(std::vector<typename std::remove_const<T>::type, Allocator> & data)\n"
  "      : m_count(static_cast<uint32_t>(data.size()))\n"
  "      , m_ptr(data.data())\n"
  "    {}\n"
  "\n"
  "    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>\n"
  "    ArrayProxy(std::vector<typename std::remove_const<T>::type, Allocator> const& data)\n"
  "      : m_count(static_cast<uint32_t>(data.size()))\n"
  "      , m_ptr(data.data())\n"
  "    {}\n"
  "\n"
  "    ArrayProxy(std::initializer_list<T> const& data)\n"
  "      : m_count(static_cast<uint32_t>(data.end() - data.begin()))\n"
  "      , m_ptr(data.begin())\n"
  "    {}\n"
  "\n"
  "    const T * begin() const\n"
  "    {\n"
  "      return m_ptr;\n"
  "    }\n"
  "\n"
  "    const T * end() const\n"
  "    {\n"
  "      return m_ptr + m_count;\n"
  "    }\n"
  "\n"
  "    const T & front() const\n"
  "    {\n"
  "      assert(m_count && m_ptr);\n"
  "      return *m_ptr;\n"
  "    }\n"
  "\n"
  "    const T & back() const\n"
  "    {\n"
  "      assert(m_count && m_ptr);\n"
  "      return *(m_ptr + m_count - 1);\n"
  "    }\n"
  "\n"
  "    bool empty() const\n"
  "    {\n"
  "      return (m_count == 0);\n"
  "    }\n"
  "\n"
  "    uint32_t size() const\n"
  "    {\n"
  "      return m_count;\n"
  "    }\n"
  "\n"
  "    T * data() const\n"
  "    {\n"
  "      return m_ptr;\n"
  "    }\n"
  "\n"
  "  private:\n"
  "    uint32_t  m_count;\n"
  "    T *       m_ptr;\n"
  "  };\n"
  "#endif\n"
  "\n"
);

std::string const versionCheckHeader = (
  "#if !defined(VULKAN_HPP_HAS_UNRESTRICTED_UNIONS)\n"
  "# if defined(__clang__)\n"
  "#  if __has_feature(cxx_unrestricted_unions)\n"
  "#   define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS\n"
  "#  endif\n"
  "# elif defined(__GNUC__)\n"
  "#  define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)\n"
  "#  if 40600 <= GCC_VERSION\n"
  "#   define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS\n"
  "#  endif\n"
  "# elif defined(_MSC_VER)\n"
  "#  if 1900 <= _MSC_VER\n"
  "#   define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS\n"
  "#  endif\n"
  "# endif\n"
  "#endif\n"
  "\n"
  );

std::string const inlineHeader = {R"(
#if !defined(VULKAN_HPP_INLINE)
# if defined(__clang___)
#  if __has_attribute(always_inline)
#   define VULKAN_HPP_INLINE __attribute__((always_inline)) __inline__
#  else
#    define VULKAN_HPP_INLINE inline
#  endif
# elif defined(__GNUC__)
#  define VULKAN_HPP_INLINE __attribute__((always_inline)) __inline__
# elif defined(_MSC_VER)
#  define VULKAN_HPP_INLINE __forceinline
# else
#  define VULKAN_HPP_INLINE inline
# endif
#endif

)"
};

std::string const resultValueHeader = (
  "  template <typename T>\n"
  "  struct ResultValue\n"
  "  {\n"
  "    ResultValue( Result r, T & v )\n"
  "      : result( r )\n"
  "      , value( v )\n"
  "    {}\n"
  "\n"
  "    Result  result;\n"
  "    T       value;\n"
  "\n"
  "    operator std::tuple<Result&, T&>() { return std::tuple<Result&, T&>(result, value); }\n"
  "  };\n"
  "\n"
  "  template <typename T>\n"
  "  struct ResultValueType\n"
  "  {\n"
  "#ifdef VULKAN_HPP_NO_EXCEPTIONS\n"
  "    typedef ResultValue<T>  type;\n"
  "#else\n"
  "    typedef T              type;\n"
  "#endif\n"
  "  };\n"
  "\n"
  "  template <>"
  "  struct ResultValueType<void>\n"
  "  {\n"
  "#ifdef VULKAN_HPP_NO_EXCEPTIONS\n"
  "    typedef Result type;\n"
  "#else\n"
  "    typedef void   type;\n"
  "#endif\n"
  "  };\n"
  "\n"
  );

std::string const createResultValueHeader = (
  "  VULKAN_HPP_INLINE ResultValueType<void>::type createResultValue( Result result, char const * message )\n"
  "  {\n"
  "#ifdef VULKAN_HPP_NO_EXCEPTIONS\n"
  "    assert( result == Result::eSuccess );\n"
  "    return result;\n"
  "#else\n"
  "    if ( result != Result::eSuccess )\n"
  "    {\n"
  "      throw std::system_error( result, message );\n"
  "    }\n"
  "#endif\n"
  "  }\n"
  "\n"
  "  template <typename T>\n"
  "  VULKAN_HPP_INLINE typename ResultValueType<T>::type createResultValue( Result result, T & data, char const * message )\n"
  "  {\n"
  "#ifdef VULKAN_HPP_NO_EXCEPTIONS\n"
  "    assert( result == Result::eSuccess );\n"
  "    return ResultValue<T>( result, data );\n"
  "#else\n"
  "    if ( result != Result::eSuccess )\n"
  "    {\n"
  "      throw std::system_error( result, message );\n"
  "    }\n"
  "    return data;\n"
  "#endif\n"
  "  }\n"
  "\n"
  "  VULKAN_HPP_INLINE Result createResultValue( Result result, char const * message, std::initializer_list<Result> successCodes )\n"
  "  {\n"
  "#ifdef VULKAN_HPP_NO_EXCEPTIONS\n"
  "    assert( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );\n"
  "#else\n"
  "    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )\n"
  "    {\n"
  "      throw std::system_error( result, message );\n"
  "    }\n"
  "#endif\n"
  "    return result;\n"
  "  }\n"
  "\n"
  "  template <typename T>\n"
  "  VULKAN_HPP_INLINE ResultValue<T> createResultValue( Result result, T & data, char const * message, std::initializer_list<Result> successCodes )\n"
  "  {\n"
  "#ifdef VULKAN_HPP_NO_EXCEPTIONS\n"
  "    assert( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );\n"
  "#else\n"
  "    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )\n"
  "    {\n"
  "      throw std::system_error( result, message );\n"
  "    }\n"
  "#endif\n"
  "    return ResultValue<T>( result, data );\n"
  "  }\n"
  "\n"
  );

// trim from start
std::string trim(std::string const& input)
{
  std::string result = input;
  result.erase(result.begin(), std::find_if(result.begin(), result.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  return result;
}

// trim from end
std::string trimEnd(std::string const& input)
{
  std::string result = input;
  result.erase(std::find_if(result.rbegin(), result.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), result.end());
  return result;
}

struct MemberData
{
  std::string type;
  std::string name;
  std::string arraySize;
  std::string pureType;
  std::string len;
  bool        optional;
};

struct CommandData
{
  CommandData()
    : handleCommand(false)
    , twoStep(false)
  {}

  std::string               returnType;
  std::vector<MemberData>   arguments;
  std::vector<std::string>  successCodes;
  std::string               protect;
  bool                      handleCommand;
  bool                      twoStep;
};

struct DependencyData
{
  enum class Category
  {
    COMMAND,
    ENUM,
    FLAGS,
    FUNC_POINTER,
    HANDLE,
    REQUIRED,
    SCALAR,
    STRUCT,
    UNION
  };

  DependencyData(Category c, std::string const& n)
    : category(c)
    , name(n)
  {}

  Category              category;
  std::string           name;
  std::set<std::string> dependencies;
};

struct NameValue
{
  std::string name;
  std::string value;
};

struct EnumData
{
  bool                    bitmask;
  std::string             prefix;
  std::string             postfix;
  std::vector<NameValue>  members;
  std::string             protect;

  void addEnum(std::string const & name, std::string const& tag, bool appendTag);
};

struct FlagData
{
  std::string protect;
};

struct HandleData
{
  std::vector<std::string>  commands;
};

struct ScalarData
{
  std::string protect;
};

struct StructData
{
  StructData()
    : returnedOnly(false)
  {}

  bool                    returnedOnly;
  bool                    isUnion;
  std::vector<MemberData> members;
  std::string             protect;
};

struct VkData
{
  std::map<std::string, CommandData>  commands;
  std::list<DependencyData>           dependencies;
  std::map<std::string, EnumData>     enums;
  std::map<std::string, FlagData>     flags;
  std::map<std::string, HandleData>   handles;
  std::map<std::string, ScalarData>   scalars;
  std::map<std::string, StructData>   structs;
  std::set<std::string>               tags;
  std::string                         typesafeCheck;
  std::string                         version;
  std::set<std::string>               vkTypes;
  std::string                         vulkanLicenseHeader;
};

void createDefaults( VkData const& vkData, std::map<std::string,std::string> & defaultValues );
std::string determineFunctionName(std::string const& name, CommandData const& commandData);
std::string determineReturnType(CommandData const& commandData, size_t returnIndex, bool isVector = false);
void enterProtect(std::ofstream &ofs, std::string const& protect);
std::string extractTag(std::string const& name);
size_t findReturnIndex(CommandData const& commandData, std::map<size_t,size_t> const& vectorParameters);
std::string findTag(std::string const& name, std::set<std::string> const& tags);
size_t findTemplateIndex(CommandData const& commandData, std::map<size_t, size_t> const& vectorParameters);
std::string generateEnumNameForFlags(std::string const& name);
std::map<size_t, size_t> getVectorParameters(CommandData const& commandData);
bool hasPointerArguments(CommandData const& commandData);
bool isVectorSizeParameter(std::map<size_t, size_t> const& vectorParameters, size_t idx);
void leaveProtect(std::ofstream &ofs, std::string const& protect);
bool noDependencies(std::set<std::string> const& dependencies, std::map<std::string, std::string> & listedTypes);
bool readCommandParam( tinyxml2::XMLElement * element, DependencyData & typeData, std::vector<MemberData> & arguments );
std::map<std::string, CommandData>::iterator readCommandProto(tinyxml2::XMLElement * element, VkData & vkData);
void readCommands( tinyxml2::XMLElement * element, VkData & vkData );
void readCommandsCommand(tinyxml2::XMLElement * element, VkData & vkData);
void readComment(tinyxml2::XMLElement * element, std::string & header);
void readEnums( tinyxml2::XMLElement * element, VkData & vkData );
void readEnumsEnum( tinyxml2::XMLElement * element, EnumData & enumData, std::string const& tag );
void readExtensionRequire(tinyxml2::XMLElement * element, VkData & vkData, std::string const& protect, std::string const& tag);
void readExtensions( tinyxml2::XMLElement * element, VkData & vkData );
void readExtensionsExtension(tinyxml2::XMLElement * element, VkData & vkData);
void readTypeBasetype( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies );
void readTypeBitmask( tinyxml2::XMLElement * element, VkData & vkData);
void readTypeDefine( tinyxml2::XMLElement * element, VkData & vkData );
void readTypeFuncpointer( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies );
void readTypeHandle(tinyxml2::XMLElement * element, VkData & vkData);
void readTypeStruct( tinyxml2::XMLElement * element, VkData & vkData );
void readTypeStructMember( tinyxml2::XMLElement * element, std::vector<MemberData> & members, std::set<std::string> & dependencies );
void readTypeUnion( tinyxml2::XMLElement * element, VkData & vkData );
void readTypeUnionMember( tinyxml2::XMLElement * element, std::vector<MemberData> & members, std::set<std::string> & dependencies );
void readTags(tinyxml2::XMLElement * element, std::set<std::string> & tags);
void readTypes(tinyxml2::XMLElement * element, VkData & vkData);
void sortDependencies( std::list<DependencyData> & dependencies );
std::string reduceName(std::string const& name, bool singular = false);
std::string strip(std::string const& value, std::string const& prefix, std::string const& tag = std::string());
std::string stripCommand(std::string const& value);
std::string toCamelCase(std::string const& value);
std::string toUpperCase(std::string const& name);
void writeCall(std::ofstream & ofs, std::string const& name, size_t templateIndex, CommandData const& commandData, std::set<std::string> const& vkTypes, std::map<size_t, size_t> const& vectorParameters, size_t returnIndex, bool firstCall, bool singular);
void writeEnumsToString(std::ofstream & ofs, VkData const& vkData);
void writeExceptionCheck(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, std::vector<std::string> const& successCodes);
void writeFunctionBody(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, std::string const& returnType, size_t templateIndex, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes, size_t returnIndex, std::map<size_t, size_t> const& vectorParameters, bool singular);
void writeFunctionHeader(std::ofstream & ofs, VkData const& vkData, std::string const& indentation, std::string const& returnType, std::string const& name, CommandData const& commandData, size_t returnIndex, size_t templateIndex, std::map<size_t, size_t> const& vectorParameters, bool singular);
void writeMemberData(std::ofstream & ofs, MemberData const& memberData, std::set<std::string> const& vkTypes);
void writeStructConstructor( std::ofstream & ofs, std::string const& name, StructData const& structData, std::set<std::string> const& vkTypes, std::map<std::string,std::string> const& defaultValues );
void writeStructSetter( std::ofstream & ofs, std::string const& name, MemberData const& memberData, std::set<std::string> const& vkTypes, std::map<std::string,StructData> const& structs );
void writeTypeCommand(std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData);
void writeTypeCommandEnhanced(std::ofstream & ofs, VkData const& vkData, std::string const& indentation, std::string const& className, std::string const& functionName, DependencyData const& dependencyData, CommandData const& commandData);
void writeTypeCommandStandard(std::ofstream & ofs, std::string const& indentation, std::string const& functionName, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes);
void writeTypeEnum(std::ofstream & ofs, DependencyData const& dependencyData, EnumData const& enumData);
void writeTypeFlags(std::ofstream & ofs, DependencyData const& dependencyData, FlagData const& flagData, std::map<std::string, EnumData>::const_iterator enumData);
void writeTypeHandle(std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, HandleData const& handle, std::list<DependencyData> const& dependencies);
void writeTypeScalar( std::ofstream & ofs, DependencyData const& dependencyData );
void writeTypeStruct( std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, std::map<std::string,std::string> const& defaultValues );
void writeTypeUnion( std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, StructData const& unionData, std::map<std::string,std::string> const& defaultValues );
void writeTypes(std::ofstream & ofs, VkData const& vkData, std::map<std::string, std::string> const& defaultValues);
void writeVersionCheck(std::ofstream & ofs, std::string const& version);
void writeTypesafeCheck(std::ofstream & ofs, std::string const& typesafeCheck);

void EnumData::addEnum(std::string const & name, std::string const& tag, bool appendTag)
{
  assert(tag.empty() || (name.find(tag) != std::string::npos));
  members.push_back(NameValue());
  members.back().name = "e" + toCamelCase(strip(name, prefix, tag));
  members.back().value = name;
  if (!postfix.empty())
  {
    size_t pos = members.back().name.find(postfix);
    if (pos != std::string::npos)
    {
      members.back().name.erase(pos);
    }
  }
  if (appendTag && !tag.empty())
  {
    members.back().name += tag;
  }
}

void createDefaults( VkData const& vkData, std::map<std::string,std::string> & defaultValues )
{
  for ( std::list<DependencyData>::const_iterator it = vkData.dependencies.begin() ; it != vkData.dependencies.end() ; ++it )
  {
    assert( defaultValues.find( it->name ) == defaultValues.end() );
    switch( it->category )
    {
      case DependencyData::Category::COMMAND :    // commands should never be asked for defaults
        break;
      case DependencyData::Category::ENUM :
        {
          assert(vkData.enums.find(it->name) != vkData.enums.end());
          EnumData const & enumData = vkData.enums.find(it->name)->second;
          if (!enumData.members.empty())
          {
            defaultValues[it->name] = it->name + "::" + vkData.enums.find(it->name)->second.members.front().name;
          }
          else
          {
            defaultValues[it->name] = it->name + "()";
          }
        }
        break;
      case DependencyData::Category::FLAGS :
      case DependencyData::Category::HANDLE:
      case DependencyData::Category::STRUCT:
      case DependencyData::Category::UNION :        // just call the default constructor for flags, structs, and structs (which are mapped to classes)
        defaultValues[it->name] = it->name + "()";
        break;
      case DependencyData::Category::FUNC_POINTER : // func_pointers default to nullptr
        defaultValues[it->name] = "nullptr";
        break;
      case DependencyData::Category::REQUIRED :     // all required default to "0"
      case DependencyData::Category::SCALAR :       // all scalars default to "0"
        defaultValues[it->name] = "0";
        break;
      default :
        assert( false );
        break;
    }
  }
}

std::string determineFunctionName(std::string const& name, CommandData const& commandData)
{
  if (commandData.handleCommand)
  {
    std::string strippedName = name;
    std::string searchName = commandData.arguments[0].pureType;
    size_t pos = name.find(searchName);
    if (pos == std::string::npos)
    {
      assert(isupper(searchName[0]));
      searchName[0] = tolower(searchName[0]);
      pos = name.find(searchName);
    }
    if (pos != std::string::npos)
    {
      strippedName.erase(pos, commandData.arguments[0].pureType.length());
    }
    else if ((commandData.arguments[0].pureType == "CommandBuffer") && (name.find("cmd") == 0))
    {
      strippedName.erase(0, 3);
      pos = 0;
    }
    if (pos == 0)
    {
      assert(isupper(strippedName[0]));
      strippedName[0] = tolower(strippedName[0]);
    }
    return strippedName;
  }
  return name;
}

std::string determineReturnType(CommandData const& commandData, size_t returnIndex, bool isVector)
{
  std::string returnType;
  if (    (returnIndex != ~0)
      &&  (   (commandData.returnType == "void")
          ||  (   (commandData.returnType == "Result")
              &&  (   (commandData.successCodes.size() == 1)
                  ||  (   (commandData.successCodes.size() == 2)
                      &&  (commandData.successCodes[1] == "eIncomplete")
                      &&  commandData.twoStep)))))
  {
    if (isVector)
    {
      if (commandData.arguments[returnIndex].pureType == "void")
      {
        returnType = "std::vector<uint8_t,Allocator>";
      }
      else
      {
        returnType = "std::vector<" + commandData.arguments[returnIndex].pureType + ",Allocator>";
      }
    }
    else
    {
      assert(commandData.arguments[returnIndex].type.back() == '*');
      assert(commandData.arguments[returnIndex].type.find("const") == std::string::npos);
      returnType = commandData.arguments[returnIndex].type;
      returnType.pop_back();
    }
  }
  else if ((commandData.returnType == "Result") && (commandData.successCodes.size() == 1))
  {
    // an original return of type "Result" with less just one successCode is changed to void, errors throw an exception
    returnType = "void";
  }
  else
  {
    // the return type just stays the original return type
    returnType = commandData.returnType;
  }
  return returnType;
}

void enterProtect(std::ofstream &ofs, std::string const& protect)
{
  if (!protect.empty())
  {
    ofs << "#ifdef " << protect << std::endl;
  }
}

std::string extractTag(std::string const& name)
{
  // the name is supposed to look like: VK_<tag>_<other>
  size_t start = name.find('_');
  assert(start != std::string::npos);
  size_t end = name.find('_', start + 1);
  assert(end != std::string::npos);
  return name.substr(start + 1, end - start - 1);
}

size_t findReturnIndex(CommandData const& commandData, std::map<size_t,size_t> const& vectorParameters)
{
  if ((commandData.returnType == "Result") || (commandData.returnType == "void"))
  {
    for (size_t i = 0; i < commandData.arguments.size(); i++)
    {
      if ((commandData.arguments[i].type.find('*') != std::string::npos) && (commandData.arguments[i].type.find("const") == std::string::npos) && !isVectorSizeParameter(vectorParameters, i)
          && ((vectorParameters.find(i) == vectorParameters.end()) || commandData.twoStep || (commandData.successCodes.size() == 1)))
      {
#if !defined(NDEBUG)
        for (size_t j = i + 1; j < commandData.arguments.size(); j++)
        {
          assert((commandData.arguments[j].type.find('*') == std::string::npos) || (commandData.arguments[j].type.find("const") != std::string::npos));
        }
#endif
        return i;
      }
    }
  }
  return ~0;
}

std::string findTag(std::string const& name, std::set<std::string> const& tags)
{
  for (std::set<std::string>::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    size_t pos = name.find(*it);
    if ((pos != std::string::npos) && (pos == name.length() - it->length()))
    {
      return *it;
    }
  }
  return "";
}

size_t findTemplateIndex(CommandData const& commandData, std::map<size_t, size_t> const& vectorParameters)
{
  for (size_t i = 0; i < commandData.arguments.size(); i++)
  {
    if ((commandData.arguments[i].name == "pData") || (commandData.arguments[i].name == "pValues"))
    {
      assert(vectorParameters.find(i) != vectorParameters.end());
      return i;
    }
  }
  return ~0;
}

std::string getEnumName(std::string const& name) // get vulkan.hpp enum name from vk enum name
{
  return strip(name, "Vk");
}

std::string generateEnumNameForFlags(std::string const& name)
{
  std::string generatedName = name;
  size_t pos = generatedName.rfind("Flags");
  assert(pos != std::string::npos);
  generatedName.replace(pos, 5, "FlagBits");
  return generatedName;
}

std::map<size_t, size_t> getVectorParameters(CommandData const& commandData)
{
  std::map<size_t,size_t> lenParameters;
  for (size_t i = 0; i < commandData.arguments.size(); i++)
  {
    if (!commandData.arguments[i].len.empty())
    {
      lenParameters.insert(std::make_pair(i, ~0));
      for (size_t j = 0; j < commandData.arguments.size(); j++)
      {
        if (commandData.arguments[i].len == commandData.arguments[j].name)
        {
          lenParameters[i] = j;
        }
      }
      assert(   (lenParameters[i] != ~0)
             || (commandData.arguments[i].len == "dataSize/4")
             || (commandData.arguments[i].len == "latexmath:[$dataSize \\over 4$]")
             || (commandData.arguments[i].len == "null-terminated")
             || (commandData.arguments[i].len == "pAllocateInfo->descriptorSetCount")
             || (commandData.arguments[i].len == "pAllocateInfo->commandBufferCount")
             || (commandData.arguments[i].len == "pAllocateInfo::descriptorSetCount")
             || (commandData.arguments[i].len == "pAllocateInfo::commandBufferCount"));
        assert((lenParameters[i] == ~0) || (lenParameters[i] < i));
    }
  }
  return lenParameters;
}

bool hasPointerArguments(CommandData const& commandData)
{
  for (size_t i = 0; i < commandData.arguments.size(); i++)
  {
    size_t pos = commandData.arguments[i].type.find('*');
    if ((pos != std::string::npos) && (commandData.arguments[i].type.find('*', pos + 1) == std::string::npos))
    {
      return true;
    }
  }
  return false;
}

bool isVectorSizeParameter(std::map<size_t, size_t> const& vectorParameters, size_t idx)
{
  for (std::map<size_t, size_t>::const_iterator it = vectorParameters.begin(); it != vectorParameters.end(); ++it)
  {
    if (it->second == idx)
    {
      return true;
    }
  }
  return false;
}

void leaveProtect(std::ofstream &ofs, std::string const& protect)
{
  if (!protect.empty())
  {
    ofs << "#endif /*" << protect << "*/" << std::endl;
  }
}

bool noDependencies(std::set<std::string> const& dependencies, std::set<std::string> & listedTypes)
{
  bool ok = true;
  for ( std::set<std::string>::const_iterator it = dependencies.begin() ; it != dependencies.end() && ok ; ++it )
  {
    ok = ( listedTypes.find( *it ) != listedTypes.end() );
  }
  return( ok );
}

bool readCommandParam( tinyxml2::XMLElement * element, DependencyData & typeData, std::vector<MemberData> & arguments )
{
  arguments.push_back( MemberData() );
  MemberData & arg = arguments.back();

  if (element->Attribute("len"))
  {
    arg.len = element->Attribute("len");
  }

  tinyxml2::XMLNode * child = element->FirstChild();
  assert( child );
  if ( child->ToText() )
  {
    std::string value = trimEnd(child->Value());
    assert( (value == "const") || (value == "struct") );
    arg.type = value + " ";
    child = child->NextSibling();
    assert( child );
  }

  assert( child->ToElement() );
  assert( ( strcmp( child->Value(), "type" ) == 0 ) && child->ToElement() && child->ToElement()->GetText() );
  std::string type = strip( child->ToElement()->GetText(), "Vk" );
  typeData.dependencies.insert( type );
  arg.type += type;
  arg.pureType = type;

  child = child->NextSibling();
  assert( child );
  if ( child->ToText() )
  {
    std::string value = trimEnd(child->Value());
    assert( ( value == "*" ) || ( value == "**" ) || ( value == "* const*" ) );
    arg.type += value;
    child = child->NextSibling();
  }

  assert( child->ToElement() && ( strcmp( child->Value(), "name" ) == 0 ) );
  arg.name = child->ToElement()->GetText();

  if ( arg.name.back() == ']' )
  {
    assert( !child->NextSibling() );
    size_t pos = arg.name.find( '[' );
    assert( pos != std::string::npos );
    arg.arraySize = arg.name.substr( pos + 1, arg.name.length() - 2 - pos );
    arg.name.erase( pos );
  }

  child = child->NextSibling();
  if ( child )
  {
    if ( child->ToText() )
    {
      std::string value = child->Value();
      if ( value == "[" )
      {
        child = child->NextSibling();
        assert( child );
        assert( child->ToElement() && ( strcmp( child->Value(), "enum" ) == 0 ) );
        arg.arraySize = child->ToElement()->GetText();
        child = child->NextSibling();
        assert( child );
        assert( child->ToText() );
        assert( strcmp( child->Value(), "]" ) == 0 );
        assert( !child->NextSibling() );
      }
      else
      {
        assert( ( value.front() == '[' ) && ( value.back() == ']' ) );
        arg.arraySize = value.substr( 1, value.length() - 2 );
        assert( !child->NextSibling() );
      }
    }
  }

  arg.optional = element->Attribute("optional") && (strcmp(element->Attribute("optional"), "true") == 0);

  return element->Attribute("optional") && (strcmp(element->Attribute("optional"), "false,true") == 0);
}

std::map<std::string, CommandData>::iterator readCommandProto(tinyxml2::XMLElement * element, VkData & vkData)
{
  tinyxml2::XMLElement * typeElement = element->FirstChildElement();
  assert( typeElement && ( strcmp( typeElement->Value(), "type" ) == 0 ) );
  tinyxml2::XMLElement * nameElement = typeElement->NextSiblingElement();
  assert( nameElement && ( strcmp( nameElement->Value(), "name" ) == 0 ) );
  assert( !nameElement->NextSiblingElement() );

  std::string type = strip( typeElement->GetText(), "Vk" );
  std::string name = stripCommand( nameElement->GetText() );

  vkData.dependencies.push_back( DependencyData( DependencyData::Category::COMMAND, name ) );
  assert( vkData.commands.find( name ) == vkData.commands.end() );
  std::map<std::string,CommandData>::iterator it = vkData.commands.insert( std::make_pair( name, CommandData() ) ).first;
  it->second.returnType = type;

  return it;
}

void readCommands(tinyxml2::XMLElement * element, VkData & vkData)
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child );
  do
  {
    assert( strcmp( child->Value(), "command" ) == 0 );
    readCommandsCommand( child, vkData );
  } while ( child = child->NextSiblingElement() );
}

void readCommandsCommand(tinyxml2::XMLElement * element, VkData & vkData)
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child && ( strcmp( child->Value(), "proto" ) == 0 ) );

  std::map<std::string, CommandData>::iterator it = readCommandProto(child, vkData);

  if (element->Attribute("successcodes"))
  {
    std::string successCodes = element->Attribute("successcodes");
    size_t start = 0, end;
    do
    {
      end = successCodes.find(',', start);
      std::string code = successCodes.substr(start, end - start);
      std::string tag = findTag(code, vkData.tags);
      it->second.successCodes.push_back("e" + toCamelCase(strip(code, "VK_", tag)) + tag);
      start = end + 1;
    } while (end != std::string::npos);
  }

  // HACK: the current vk.xml misses to specify successcodes on command vkCreateDebugReportCallbackEXT!
  if (it->first == "createDebugReportCallbackEXT")
  {
    it->second.successCodes.clear();
    it->second.successCodes.push_back("eSuccess");
  }

  while ( child = child->NextSiblingElement() )
  {
    std::string value = child->Value();
    if ( value == "param" )
    {
      it->second.twoStep |= readCommandParam(child, vkData.dependencies.back(), it->second.arguments);
    }
    else
    {
      assert( ( value == "implicitexternsyncparams" ) || ( value == "validity" ) );
    }
  }

  // HACK: the current vk.xml misses to specify <optional="false,true"> on param pSparseMemoryRequirementCount on command vkGetImageSparseMemoryRequirements!
  if (it->first == "getImageSparseMemoryRequirements")
  {
    it->second.twoStep = true;
  }

  assert(!it->second.arguments.empty());
  std::map<std::string, HandleData>::iterator hit = vkData.handles.find(it->second.arguments[0].pureType);
  if (hit != vkData.handles.end())
  {
    hit->second.commands.push_back(it->first);
    it->second.handleCommand = true;
    DependencyData const& dep = vkData.dependencies.back();
    std::list<DependencyData>::iterator dit = std::find_if(vkData.dependencies.begin(), vkData.dependencies.end(), [hit](DependencyData const& dd) { return dd.name == hit->first; });
    for (std::set<std::string>::const_iterator depit = dep.dependencies.begin(); depit != dep.dependencies.end(); ++depit)
    {
      if (*depit != hit->first)
      {
        dit->dependencies.insert(*depit);
      }
    }
  }
}

void readComment(tinyxml2::XMLElement * element, std::string & header)
{
  assert(element->GetText());
  assert(header.empty());
  header = element->GetText();
  assert(header.find("\nCopyright") == 0);

  size_t pos = header.find("\n\n-----");
  assert(pos != std::string::npos);
  header.erase(pos);

  for (size_t pos = header.find('\n'); pos != std::string::npos; pos = header.find('\n', pos + 1))
  {
    header.replace(pos, 1, "\n// ");
  }

  header += "\n\n// This header is generated from the Khronos Vulkan XML API Registry.";
}

void readEnums( tinyxml2::XMLElement * element, VkData & vkData )
{
  if (!element->Attribute("name"))
  {
    throw std::runtime_error(std::string("spec error: enums element is missing the name attribute"));
  }

  std::string name = getEnumName(element->Attribute("name"));
  if ( name != "API Constants" )
  {
    vkData.dependencies.push_back( DependencyData( DependencyData::Category::ENUM, name ) );
    std::map<std::string,EnumData>::iterator it = vkData.enums.insert( std::make_pair( name, EnumData() ) ).first;
    std::string tag;

    if (name == "Result")
    {
      // special handling for VKResult, as its enums just have VK_ in common
      it->second.prefix = "VK_";
    }
    else
    {
      if (!element->Attribute("type"))
      {
        throw std::runtime_error(std::string("spec error: enums name=\"" + name + "\" is missing the type attribute"));
      }

      std::string type = element->Attribute("type");

      if (type != "bitmask" && type != "enum")
      {
        throw std::runtime_error(std::string("spec error: enums name=\"" + name + "\" has unknown type " + type));
      }

      it->second.bitmask = (type == "bitmask");
      std::string prefix, postfix;
      if (it->second.bitmask)
      {
        size_t pos = name.find("FlagBits");
        assert(pos != std::string::npos);
        it->second.prefix = "VK" + toUpperCase(name.substr(0, pos)) + "_";
        it->second.postfix = "Bit";
      }
      else
      {
        it->second.prefix = "VK" + toUpperCase(name) + "_";
      }

      // if the enum name contains a tag remove it from the prefix to generate correct enum value names.
      for (std::set<std::string>::const_iterator tit = vkData.tags.begin(); tit != vkData.tags.end(); ++tit)
      {
        size_t pos = it->second.prefix.find(*tit);
        if ((pos != std::string::npos) && (pos == it->second.prefix.length() - tit->length() - 1))
        {
          it->second.prefix.erase(pos);
          tag = *tit;
          break;
        }
      }
    }

    readEnumsEnum( element, it->second, tag );

    assert( vkData.vkTypes.find( name ) == vkData.vkTypes.end() );
    vkData.vkTypes.insert( name );
  }
}

void readEnumsEnum( tinyxml2::XMLElement * element, EnumData & enumData, std::string const& tag )
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    if ( child->Attribute( "name" ) )
    {
      enumData.addEnum(child->Attribute("name"), tag, false);
    }
  } while ( child = child->NextSiblingElement() );
}

void readExtensionRequire(tinyxml2::XMLElement * element, VkData & vkData, std::string const& protect, std::string const& tag)
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    std::string value = child->Value();

    if ( value == "command" )
    {
      assert(child->Attribute("name"));
      std::string name = stripCommand(child->Attribute("name"));
      std::map<std::string, CommandData>::iterator cit = vkData.commands.find(name);
      assert(cit != vkData.commands.end());
      cit->second.protect = protect;
    }
    else if (value == "type")
    {
      assert(child->Attribute("name"));
      std::string name = strip(child->Attribute("name"), "Vk");
      std::map<std::string, EnumData>::iterator eit = vkData.enums.find(name);
      if (eit != vkData.enums.end())
      {
        eit->second.protect = protect;
      }
      else
      {
        std::map<std::string, FlagData>::iterator fit = vkData.flags.find(name);
        if (fit != vkData.flags.end())
        {
          fit->second.protect = protect;

          // if the enum of this flags is auto-generated, protect it as well
          std::string enumName = generateEnumNameForFlags(name);
          std::map<std::string, EnumData>::iterator eit = vkData.enums.find(enumName);
          assert(eit != vkData.enums.end());
          if (eit->second.members.empty())
          {
            eit->second.protect = protect;
          }
        }
        else
        {
          std::map<std::string, ScalarData>::iterator scit = vkData.scalars.find(name);
          if (scit != vkData.scalars.end())
          {
            scit->second.protect = protect;
          }
          else
          {
            std::map<std::string, StructData>::iterator stit = vkData.structs.find(name);
            assert(stit != vkData.structs.end() && stit->second.protect.empty());
            stit->second.protect = protect;
          }
        }
      }
    }
    else if ( value == "enum")
    {
      // TODO process enums which don't extend existing enums
      if (child->Attribute("extends"))
      {
        assert(child->Attribute("name"));
        assert(vkData.enums.find(getEnumName(child->Attribute("extends"))) != vkData.enums.end());
        assert(!!child->Attribute("bitpos") + !!child->Attribute("offset") + !!child->Attribute("value") == 1);
        vkData.enums[getEnumName(child->Attribute("extends"))].addEnum(child->Attribute("name"), child->Attribute("value") ? "" : tag, true );
      }
    }
    else
    {
      assert(value=="usage");
    }
  } while ( child = child->NextSiblingElement() );
}

void readExtensions(tinyxml2::XMLElement * element, VkData & vkData)
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child );
  do
  {
    assert( strcmp( child->Value(), "extension" ) == 0 );
    readExtensionsExtension( child, vkData );
  } while ( child = child->NextSiblingElement() );
}

void readExtensionsExtension(tinyxml2::XMLElement * element, VkData & vkData)
{
  assert( element->Attribute( "name" ) );
  std::string tag = extractTag(element->Attribute("name"));
  assert(vkData.tags.find(tag) != vkData.tags.end());

  // don't parse disabled extensions
  if (strcmp(element->Attribute("supported"), "disabled") == 0)
  {
    return;
  }

  std::string protect;
  if (element->Attribute("protect"))
  {
    protect = element->Attribute( "protect" );
  }

  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child && ( strcmp( child->Value(), "require" ) == 0 ) && !child->NextSiblingElement() );
  readExtensionRequire( child, vkData, protect, tag );
}

void readTypeBasetype( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies )
{
  tinyxml2::XMLElement * typeElement = element->FirstChildElement();
  assert( typeElement && ( strcmp( typeElement->Value(), "type" ) == 0 ) && typeElement->GetText() );
  std::string type = typeElement->GetText();
  assert( ( type == "uint32_t" ) || ( type == "uint64_t" ) );

  tinyxml2::XMLElement * nameElement = typeElement->NextSiblingElement();
  assert( nameElement && ( strcmp( nameElement->Value(), "name" ) == 0 ) && nameElement->GetText() );
  std::string name = strip( nameElement->GetText(), "Vk" );

  // skip "Flags",
  if ( name != "Flags" )
  {
    dependencies.push_back( DependencyData( DependencyData::Category::SCALAR, name ) );
    dependencies.back().dependencies.insert( type );
  }
  else
  {
    assert( type == "uint32_t" );
  }
}

void readTypeBitmask(tinyxml2::XMLElement * element, VkData & vkData)
{
  tinyxml2::XMLElement * typeElement = element->FirstChildElement();
  assert( typeElement && ( strcmp( typeElement->Value(), "type" ) == 0 ) && typeElement->GetText() && ( strcmp( typeElement->GetText(), "VkFlags" ) == 0 ) );
  std::string type = typeElement->GetText();

  tinyxml2::XMLElement * nameElement = typeElement->NextSiblingElement();
  assert( nameElement && ( strcmp( nameElement->Value(), "name" ) == 0 ) && nameElement->GetText() );
  std::string name = strip( nameElement->GetText(), "Vk" );

  assert( !nameElement->NextSiblingElement() );

  std::string requires;
  if (element->Attribute("requires"))
  {
    requires = strip(element->Attribute("requires"), "Vk");
  }
  else {
    // Generate FlagBits name
    requires = generateEnumNameForFlags(name);
    vkData.dependencies.push_back(DependencyData(DependencyData::Category::ENUM, requires));
    std::map<std::string, EnumData>::iterator it = vkData.enums.insert(std::make_pair(requires, EnumData())).first;
    it->second.bitmask = true;
    vkData.vkTypes.insert(requires);
  }

  vkData.dependencies.push_back( DependencyData( DependencyData::Category::FLAGS, name ) );
  vkData.dependencies.back().dependencies.insert( requires );
  vkData.flags.insert(std::make_pair(name, FlagData()));

  assert( vkData.vkTypes.find( name ) == vkData.vkTypes.end() );
  vkData.vkTypes.insert( name );
}

void readTypeDefine( tinyxml2::XMLElement * element, VkData & vkData )
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  if (child && (strcmp(child->GetText(), "VK_HEADER_VERSION") == 0))
  {
    vkData.version = element->LastChild()->ToText()->Value();
  }
  else if (element->Attribute("name") && strcmp(element->Attribute("name"), "VK_DEFINE_NON_DISPATCHABLE_HANDLE") == 0)
  {
    std::string text = element->LastChild()->ToText()->Value();
    size_t start = text.find("#if defined(__LP64__)");
    size_t end = text.find_first_of("\r\n", start + 1);
    vkData.typesafeCheck = text.substr(start, end - start);
  }
}

void readTypeFuncpointer( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies )
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert( child && ( strcmp( child->Value(), "name" ) == 0 ) && child->GetText() );
  dependencies.push_back( DependencyData( DependencyData::Category::FUNC_POINTER, child->GetText() ) );
}

void readTypeHandle(tinyxml2::XMLElement * element, VkData & vkData)
{
  tinyxml2::XMLElement * typeElement = element->FirstChildElement();
  assert( typeElement && ( strcmp( typeElement->Value(), "type" ) == 0 ) && typeElement->GetText() );
#if !defined(NDEBUG)
  std::string type = typeElement->GetText();
  assert( type.find( "VK_DEFINE" ) == 0 );
#endif

  tinyxml2::XMLElement * nameElement = typeElement->NextSiblingElement();
  assert( nameElement && ( strcmp( nameElement->Value(), "name" ) == 0 ) && nameElement->GetText() );
  std::string name = strip( nameElement->GetText(), "Vk" );

  vkData.dependencies.push_back( DependencyData( DependencyData::Category::HANDLE, name ) );

  assert(vkData.vkTypes.find(name) == vkData.vkTypes.end());
  vkData.vkTypes.insert(name);
  assert(vkData.handles.find(name) == vkData.handles.end());
  vkData.handles[name];    // add this to the handles map
}

void readTypeStructMember( tinyxml2::XMLElement * element, std::vector<MemberData> & members, std::set<std::string> & dependencies )
{
  members.push_back( MemberData() );
  MemberData & member = members.back();

  tinyxml2::XMLNode * child = element->FirstChild();
  assert( child );
  if ( child->ToText())
  {
    std::string value = trimEnd(child->Value());
    assert( (value == "const") || (value == "struct") );
    member.type = value + " ";
    child = child->NextSibling();
    assert( child );
  }

  assert( child->ToElement() );
  assert( ( strcmp( child->Value(), "type" ) == 0 ) && child->ToElement() && child->ToElement()->GetText() );
  std::string type = strip( child->ToElement()->GetText(), "Vk" );
  dependencies.insert( type );
  member.type += type;
  member.pureType = type;

  child = child->NextSibling();
  assert( child );
  if ( child->ToText())
  {
    std::string value = trimEnd(child->Value());
    assert( ( value == "*" ) || ( value == "**" ) || ( value == "* const*" ) );
    member.type += value;
    child = child->NextSibling();
  }

  assert( ( child->ToElement() && strcmp( child->Value(), "name" ) == 0 ));
  member.name = child->ToElement()->GetText();

  if ( member.name.back() == ']' )
  {
    assert( !child->NextSibling() );
    size_t pos = member.name.find( '[' );
    assert( pos != std::string::npos );
    member.arraySize = member.name.substr( pos + 1, member.name.length() - 2 - pos );
    member.name.erase( pos );
  }

  child = child->NextSibling();
  if ( child )
  {
    assert( member.arraySize.empty() );
    if ( child->ToText() )
    {
      std::string value = child->Value();
      if ( value == "[" )
      {
        child = child->NextSibling();
        assert( child );
        assert( child->ToElement() && ( strcmp( child->Value(), "enum" ) == 0 ) );
        member.arraySize = child->ToElement()->GetText();
        child = child->NextSibling();
        assert( child );
        assert( child->ToText() );
        assert( strcmp( child->Value(), "]" ) == 0 );
        assert( !child->NextSibling() );
      }
      else
      {
        assert( ( value.front() == '[' ) && ( value.back() == ']' ) );
        member.arraySize = value.substr( 1, value.length() - 2 );
        assert( !child->NextSibling() );
      }
    }
  }
}

void readTypeStruct( tinyxml2::XMLElement * element, VkData & vkData )
{
  assert( !element->Attribute( "returnedonly" ) || ( strcmp( element->Attribute( "returnedonly" ), "true" ) == 0 ) );

  assert( element->Attribute( "name" ) );
  std::string name = strip( element->Attribute( "name" ), "Vk" );

  if ( name == "Rect3D" )
  {
    return;
  }

  vkData.dependencies.push_back( DependencyData( DependencyData::Category::STRUCT, name ) );

  assert( vkData.structs.find( name ) == vkData.structs.end() );
  std::map<std::string,StructData>::iterator it = vkData.structs.insert( std::make_pair( name, StructData() ) ).first;
  it->second.returnedOnly = !!element->Attribute( "returnedonly" );
  it->second.isUnion = false;

  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    assert( child->Value() );
    std::string value = child->Value();
    if ( value == "member" )
    {
      readTypeStructMember( child, it->second.members, vkData.dependencies.back().dependencies );
    }
    else
    {
      assert( value == "validity" );
    }
  } while ( child = child->NextSiblingElement() );

  assert( vkData.vkTypes.find( name ) == vkData.vkTypes.end() );
  vkData.vkTypes.insert( name );
}

void readTypeUnionMember( tinyxml2::XMLElement * element, std::vector<MemberData> & members, std::set<std::string> & dependencies )
{
  members.push_back( MemberData() );
  MemberData & member = members.back();

  tinyxml2::XMLNode * child = element->FirstChild();
  assert( child );
  if ( child->ToText() )
  {
    assert( ( strcmp( child->Value(), "const" ) == 0 ) || ( strcmp( child->Value(), "struct" ) == 0 ) );
    member.type = std::string( child->Value() ) + " ";
    child = child->NextSibling();
    assert( child );
  }

  assert( child->ToElement() );
  assert( ( strcmp( child->Value(), "type" ) == 0 ) && child->ToElement() && child->ToElement()->GetText() );
  std::string type = strip( child->ToElement()->GetText(), "Vk" );
  dependencies.insert( type );
  member.type += type;
  member.pureType = type;

  child = child->NextSibling();
  assert( child );
  if ( child->ToText() )
  {
    std::string value = child->Value();
    assert( ( value == "*" ) || ( value == "**" ) || ( value == "* const*" ) );
    member.type += value;
    child = child->NextSibling();
  }

  assert( child->ToElement() && ( strcmp( child->Value(), "name" ) == 0 ) );
  member.name = child->ToElement()->GetText();

  if ( member.name.back() == ']' )
  {
    assert( !child->NextSibling() );
    size_t pos = member.name.find( '[' );
    assert( pos != std::string::npos );
    member.arraySize = member.name.substr( pos + 1, member.name.length() - 2 - pos );
    member.name.erase( pos );
  }

  child = child->NextSibling();
  if ( child )
  {
    if ( child->ToText() )
    {
      std::string value = child->Value();
      if ( value == "[" )
      {
        child = child->NextSibling();
        assert( child );
        assert( child->ToElement() && ( strcmp( child->Value(), "enum" ) == 0 ) );
        member.arraySize = child->ToElement()->GetText();
        child = child->NextSibling();
        assert( child );
        assert( child->ToText() );
        assert( strcmp( child->Value(), "]" ) == 0 );
        assert( !child->NextSibling() );
      }
      else
      {
        assert( ( value.front() == '[' ) && ( value.back() == ']' ) );
        member.arraySize = value.substr( 1, value.length() - 2 );
        assert( !child->NextSibling() );
      }
    }
  }
}

void readTypeUnion( tinyxml2::XMLElement * element, VkData & vkData )
{
  assert( element->Attribute( "name" ) );
  std::string name = strip( element->Attribute( "name" ), "Vk" );

  vkData.dependencies.push_back( DependencyData( DependencyData::Category::UNION, name ) );

  assert( vkData.structs.find( name ) == vkData.structs.end() );
  std::map<std::string,StructData>::iterator it = vkData.structs.insert( std::make_pair( name, StructData() ) ).first;
  it->second.isUnion = true;

  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    assert( strcmp( child->Value(), "member" ) == 0 );
    readTypeUnionMember( child, it->second.members, vkData.dependencies.back().dependencies );
  } while ( child = child->NextSiblingElement() );

  assert( vkData.vkTypes.find( name ) == vkData.vkTypes.end() );
  vkData.vkTypes.insert( name );
}

void readTags(tinyxml2::XMLElement * element, std::set<std::string> & tags)
{
  tags.insert("EXT");
  tags.insert("KHR");
  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    assert(child->Attribute("name"));
    tags.insert(child->Attribute("name"));
  } while (child = child->NextSiblingElement());
}

void readTypes(tinyxml2::XMLElement * element, VkData & vkData)
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    assert( strcmp( child->Value(), "type" ) == 0 );
    std::string type = child->Value();
    assert( type == "type" );
    if ( child->Attribute( "category" ) )
    {
      std::string category = child->Attribute( "category" );
      if ( category == "basetype" )
      {
        readTypeBasetype( child, vkData.dependencies );
      }
      else if ( category == "bitmask" )
      {
        readTypeBitmask( child, vkData);
      }
      else if ( category == "define" )
      {
        readTypeDefine( child, vkData );
      }
      else if ( category == "funcpointer" )
      {
        readTypeFuncpointer( child, vkData.dependencies );
      }
      else if ( category == "handle" )
      {
        readTypeHandle( child, vkData );
      }
      else if ( category == "struct" )
      {
        readTypeStruct( child, vkData );
      }
      else if ( category == "union" )
      {
        readTypeUnion( child, vkData );
      }
      else
      {
        assert( ( category == "enum" ) || ( category == "include" ) );
      }
    }
    else
    {
      assert( child->Attribute( "requires" ) && child->Attribute( "name" ) );
      vkData.dependencies.push_back( DependencyData( DependencyData::Category::REQUIRED, child->Attribute( "name" ) ) );
    }
  } while ( child = child->NextSiblingElement() );
}

void sortDependencies( std::list<DependencyData> & dependencies )
{
  std::set<std::string> listedTypes = { "VkFlags" };
  std::list<DependencyData> sortedDependencies;

  while ( !dependencies.empty() )
  {
#if !defined(NDEBUG)
    bool ok = false;
#endif
    for ( std::list<DependencyData>::iterator it = dependencies.begin() ; it != dependencies.end() ; ++it )
    {
      if ( noDependencies( it->dependencies, listedTypes ) )
      {
        sortedDependencies.push_back( *it );
        listedTypes.insert( it->name );
        dependencies.erase( it );
#if !defined(NDEBUG)
        ok = true;
#endif
        break;
      }
    }
    assert( ok );
  }

  dependencies.swap(sortedDependencies);
}

std::string reduceName(std::string const& name, bool singular)
{
  std::string reducedName;
  if ((name[0] == 'p') && (1 < name.length()) && (isupper(name[1]) || name[1] == 'p'))
  {
    reducedName = strip(name, "p");
    reducedName[0] = tolower(reducedName[0]);
  }
  else
  {
    reducedName = name;
  }
  if (singular)
  {
    size_t pos = reducedName.rfind('s');
    assert(pos != std::string::npos);
    reducedName.erase(pos, 1);
  }

  return reducedName;
}

std::string strip(std::string const& value, std::string const& prefix, std::string const& postfix)
{
  std::string strippedValue = value;
  if (strippedValue.find(prefix) == 0)
  {
    strippedValue.erase(0, prefix.length());
  }
  if (!postfix.empty())
  {
    size_t pos = strippedValue.rfind(postfix);
    assert(pos != std::string::npos);
    strippedValue.erase(pos);
  }
  return strippedValue;
}

std::string stripCommand(std::string const& value)
{
  std::string stripped = strip(value, "vk");
  assert(isupper(stripped[0]));
  stripped[0] = tolower(stripped[0]);
  return stripped;
}

std::string toCamelCase(std::string const& value)
{
  assert(!value.empty() && (isupper(value[0]) || isdigit(value[0])));
  std::string result;
  result.reserve(value.size());
  result.push_back(value[0]);
  for (size_t i = 1; i < value.size(); i++)
  {
    if (value[i] != '_')
    {
      if ((value[i - 1] == '_') || isdigit(value[i-1]))
      {
        result.push_back(value[i]);
      }
      else
      {
        result.push_back(tolower(value[i]));
      }
    }
  }
  return result;
}

std::string toUpperCase(std::string const& name)
{
  assert(isupper(name.front()));
  std::string convertedName;

  for (size_t i = 0; i<name.length(); i++)
  {
    if (isupper(name[i]) && ((i == 0) || islower(name[i - 1]) || isdigit(name[i-1])))
    {
      convertedName.push_back('_');
    }
    convertedName.push_back(toupper(name[i]));
  }
  return convertedName;
}

void writeCall(std::ofstream & ofs, std::string const& name, size_t templateIndex, CommandData const& commandData, std::set<std::string> const& vkTypes, std::map<size_t, size_t> const& vectorParameters, size_t returnIndex, bool firstCall, bool singular)
{
  std::map<size_t,size_t> countIndices;
  for (std::map<size_t, size_t>::const_iterator it = vectorParameters.begin(); it != vectorParameters.end(); ++it)
  {
    countIndices.insert(std::make_pair(it->second, it->first));
  }
  if ((vectorParameters.size() == 1)
      && ((commandData.arguments[vectorParameters.begin()->first].len == "dataSize/4") || (commandData.arguments[vectorParameters.begin()->first].len == "latexmath:[$dataSize \\over 4$]")))
  {
    assert(commandData.arguments[3].name == "dataSize");
    countIndices.insert(std::make_pair(3, vectorParameters.begin()->first));
  }

  assert(islower(name[0]));
  ofs << "vk" << static_cast<char>(toupper(name[0])) << name.substr(1) << "( ";
  size_t i = 0;
  if (commandData.handleCommand)
  {
    ofs << "m_" << commandData.arguments[0].name;
    i++;
  }
  for (; i < commandData.arguments.size(); i++)
  {
    if (0 < i)
    {
      ofs << ", ";
    }
    std::map<size_t, size_t>::const_iterator it = countIndices.find(i);
    if (it != countIndices.end())
    {
      if ((returnIndex == it->second) && commandData.twoStep)
      {
        ofs << "&" << reduceName(commandData.arguments[it->first].name);
      }
      else
      {
        if (singular)
        {
          ofs << "1 ";
        }
        else
        {
          ofs << reduceName(commandData.arguments[it->second].name) << ".size() ";
        }
        if (templateIndex == it->second)
        {
          ofs << "* sizeof( T ) ";
        }
      }
    }
    else
    {
      it = vectorParameters.find(i);
      if (it != vectorParameters.end())
      {
        assert(commandData.arguments[it->first].type.back() == '*');
        if ((returnIndex == it->first) && commandData.twoStep && firstCall)
        {
          ofs << "nullptr";
        }
        else
        {
          std::set<std::string>::const_iterator vkit = vkTypes.find(commandData.arguments[it->first].pureType);
          if ((vkit != vkTypes.end()) || (it->first == templateIndex))
          {
            ofs << "reinterpret_cast<";
            if (commandData.arguments[it->first].type.find("const") != std::string::npos)
            {
              ofs << "const ";
            }
            if (vkit != vkTypes.end())
            {
              ofs << "Vk";
            }
            ofs << commandData.arguments[it->first].pureType << "*>( " << (singular ? "&" : "") << reduceName(commandData.arguments[it->first].name, singular) << (singular ? "" : ".data()") << " )";
          }
          else if (commandData.arguments[it->first].pureType == "char")
          {
            ofs << reduceName(commandData.arguments[it->first].name);
            if (commandData.arguments[it->first].optional)
            {
              ofs << " ? " << reduceName(commandData.arguments[it->first].name) << "->c_str() : nullptr";
            }
            else
            {
              ofs << ".c_str()";
            }
          }
          else
          {
            ofs << reduceName(commandData.arguments[it->first].name) << ".data()";
          }
        }
      }
      else if (vkTypes.find(commandData.arguments[i].pureType) != vkTypes.end())
      {
        if (commandData.arguments[i].type.back() == '*')
        {
          if (commandData.arguments[i].type.find("const") != std::string::npos)
          {
            ofs << "reinterpret_cast<const Vk" << commandData.arguments[i].pureType << "*>( ";
            if (commandData.arguments[i].optional)
            {
              ofs << "static_cast<const " << commandData.arguments[i].pureType << "*>( ";
            }
            else
            {
              ofs << "&";
            }
            ofs << reduceName(commandData.arguments[i].name) << (commandData.arguments[i].optional ? "))" : " )");
          }
          else
          {
            assert(!commandData.arguments[i].optional);
            ofs << "reinterpret_cast<Vk" << commandData.arguments[i].pureType << "*>( &" << reduceName(commandData.arguments[i].name) << " )";
          }
        }
        else
        {
          ofs << "static_cast<Vk" << commandData.arguments[i].pureType << ">( " << commandData.arguments[i].name << " )";
        }
      }
      else
      {
        if (commandData.arguments[i].type.back() == '*')
        {
          if (commandData.arguments[i].type.find("const") != std::string::npos)
          {
            assert(commandData.arguments[i].type.find("char") != std::string::npos);
            ofs << reduceName(commandData.arguments[i].name);
            if (commandData.arguments[i].optional)
            {
              ofs << " ? " << reduceName(commandData.arguments[i].name) << "->c_str() : nullptr";
            }
            else
            {
              ofs << ".c_str()";
            }
          }
          else
          {
            assert(commandData.arguments[i].type.find("char") == std::string::npos);
            ofs << "&" << reduceName(commandData.arguments[i].name);
          }
        }
        else
        {
          ofs << commandData.arguments[i].name;
        }
      }
    }
  }
  ofs << " )";
}

void writeExceptionCheck(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, std::vector<std::string> const& successCodes)
{
  assert(!successCodes.empty());
  ofs << indentation << "  if (";
  if (successCodes.size() == 1)
  {
    assert(successCodes.front() == "eSuccess");
    ofs << " result != Result::eSuccess";
  }
  else
  {
    for (size_t i = 0; i < successCodes.size() - 1; i++)
    {
      ofs << " ( result != Result::" << successCodes[i] << " ) &&";
    }
    ofs << " ( result != Result::" << successCodes.back() << " )";
  }
  ofs << " )" << std::endl;
  ofs << indentation << "  {" << std::endl
    << indentation << "    throw std::system_error( result, \"vk::";
  if (!className.empty())
  {
    ofs << className << "::";
  }
  ofs << functionName << "\" );" << std::endl
      << indentation << "  }" << std::endl;
}

void writeFunctionBody(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, std::string const& returnType, size_t templateIndex, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes, size_t returnIndex, std::map<size_t, size_t> const& vectorParameters, bool singular)
{
  ofs << indentation << "{" << std::endl;

  // add a static_assert if a type is templated and its size needs to be some multiple of the original size
  if ((templateIndex != ~0) && (commandData.arguments[templateIndex].pureType != "void"))
  {
    ofs << indentation << "  static_assert( sizeof( T ) % sizeof( " << commandData.arguments[templateIndex].pureType << " ) == 0, \"wrong size of template type T\" );" << std::endl;
  }

  // add some error checks if multiple vectors need to have the same size
  if (1 < vectorParameters.size())
  {
    for (std::map<size_t, size_t>::const_iterator it0 = vectorParameters.begin(); it0 != vectorParameters.end(); ++it0)
    {
      if (it0->first != returnIndex)
      {
        for (std::map<size_t, size_t>::const_iterator it1 = std::next(it0); it1 != vectorParameters.end(); ++it1)
        {
          if ((it1->first != returnIndex) && (it0->second == it1->second))
          {
            ofs << "#ifdef VULKAN_HPP_NO_EXCEPTIONS" << std::endl
                << indentation << "  assert( " << reduceName(commandData.arguments[it0->first].name) << ".size() == " << reduceName(commandData.arguments[it1->first].name) << ".size() );" << std::endl
                << "#else" << std::endl
                << indentation << "  if ( " << reduceName(commandData.arguments[it0->first].name) << ".size() != " << reduceName(commandData.arguments[it1->first].name) << ".size() )" << std::endl
                << indentation << "  {" << std::endl
                << indentation << "    throw std::logic_error( \"vk::" << className << "::" << functionName << ": " << reduceName(commandData.arguments[it0->first].name) << ".size() != " << reduceName(commandData.arguments[it1->first].name) << ".size()\" );" << std::endl
                << indentation << "  }" << std::endl
                << "#endif  // VULKAN_HPP_NO_EXCEPTIONS" << std::endl;
          }
        }
      }
    }
  }

  // write the local variable to hold a returned value
  if (returnIndex != ~0)
  {
    if (commandData.returnType != returnType)
    {
      ofs << indentation << "  " << (singular ? commandData.arguments[returnIndex].pureType : returnType) << " " << reduceName(commandData.arguments[returnIndex].name, singular);

      if ( !singular)
      {
        std::map<size_t, size_t>::const_iterator it = vectorParameters.find(returnIndex);
        if (it != vectorParameters.end() && !commandData.twoStep)
        {
          std::string size;
          if ((it->second == ~0) && !commandData.arguments[returnIndex].len.empty())
          {
            size = reduceName(commandData.arguments[returnIndex].len);
            size_t pos = size.find("->");
            if (pos == std::string::npos)
            {
              pos = size.find("::");
            }
            assert(pos != std::string::npos);
            size.replace(pos, 2, ".");
          }
          else
          {
            for (std::map<size_t, size_t>::const_iterator sit = vectorParameters.begin(); sit != vectorParameters.end(); ++sit)
            {
              if ((sit->first != returnIndex) && (sit->second == it->second))
              {
                size = reduceName(commandData.arguments[sit->first].name) + ".size()";
                break;
              }
            }
          }
          assert(!size.empty());
          ofs << "( " << size << " )";
        }
      }
      ofs << ";" << std::endl;
    }
    else if (1 < commandData.successCodes.size())
    {
      ofs << indentation << "  " << commandData.arguments[returnIndex].pureType << " " << reduceName(commandData.arguments[returnIndex].name) << ";" << std::endl;
    }
  }

  // local count variable to hold the size of the vector to fill
  if (commandData.twoStep)
  {
    assert(returnIndex != ~0);

    std::map<size_t, size_t>::const_iterator returnit = vectorParameters.find(returnIndex);
    assert(returnit != vectorParameters.end() && (returnit->second != ~0));
    assert((commandData.returnType == "Result") || (commandData.returnType == "void"));

    ofs << indentation << "  " << commandData.arguments[returnit->second].pureType << " " << reduceName(commandData.arguments[returnit->second].name) << ";" << std::endl;
  }

  // write the function call
  ofs << indentation << "  ";
  std::string localIndentation = "  ";
  if (commandData.returnType == "Result")
  {
    ofs << "Result result";
    if (commandData.twoStep && (1 < commandData.successCodes.size()))
    {
      ofs << ";" << std::endl
          << indentation << "  do" << std::endl
          << indentation << "  {" << std::endl
          << indentation << "    result";
      localIndentation += "  ";
    }
    ofs << " = static_cast<Result>( ";
  }
  else if (commandData.returnType != "void")
  {
    assert(!commandData.twoStep);
    ofs << "return ";
  }
  writeCall(ofs, dependencyData.name, templateIndex, commandData, vkTypes, vectorParameters, returnIndex, true, singular);
  if (commandData.returnType == "Result")
  {
    ofs << " )";
  }
  ofs << ";" << std::endl;

  if (commandData.twoStep)
  {
    std::map<size_t, size_t>::const_iterator returnit = vectorParameters.find(returnIndex);

    if (commandData.returnType == "Result")
    {
      ofs << indentation << localIndentation << "if ( ( result == Result::eSuccess ) && " << reduceName(commandData.arguments[returnit->second].name) << " )" << std::endl
          << indentation << localIndentation << "{" << std::endl
          << indentation << localIndentation << "  ";
    }
    else
    {
      ofs << indentation << "  ";
    }

    // resize the vector to hold the data according to the result from the first call
    ofs << reduceName(commandData.arguments[returnit->first].name) << ".resize( " << reduceName(commandData.arguments[returnit->second].name) << " );" << std::endl;

    // write the function call a second time
    if (commandData.returnType == "Result")
    {
      ofs << indentation << localIndentation << "  result = static_cast<Result>( ";
    }
    else
    {
      ofs << indentation << "  ";
    }
    assert(!singular);
    writeCall(ofs, dependencyData.name, templateIndex, commandData, vkTypes, vectorParameters, returnIndex, false, singular);
    if (commandData.returnType == "Result")
    {
      ofs << " )";
    }
    ofs << ";" << std::endl;
    if (commandData.returnType == "Result")
    {
      ofs << indentation << localIndentation << "}" << std::endl;
      if (1 < commandData.successCodes.size())
      {
        ofs << indentation << "  } while ( result == Result::eIncomplete );" << std::endl
            << indentation << "  assert( " << reduceName(commandData.arguments[returnit->second].name) << " <= " << reduceName(commandData.arguments[returnit->first].name) << ".size() ); " << std::endl
            << indentation << "  " << reduceName(commandData.arguments[returnit->first].name) << ".resize( " << reduceName(commandData.arguments[returnit->second].name) << " ); " << std::endl;
      }
    }
  }
  
  if ((commandData.returnType == "Result") || !commandData.successCodes.empty())
  {
    ofs << indentation << "  return createResultValue( result, ";
    if (returnIndex != ~0)
    {
      ofs << reduceName(commandData.arguments[returnIndex].name, singular) << ", ";
    }
    ofs << "\"vk::" << (className.empty() ? "" : className + "::") << reduceName(functionName, singular) << "\"";
    if (1 < commandData.successCodes.size() && !commandData.twoStep)
    {
      ofs << ", { Result::" << commandData.successCodes[0];
      for (size_t i = 1; i < commandData.successCodes.size(); i++)
      {
        ofs << ", Result::" << commandData.successCodes[i];
      }
      ofs << " }";
    }
    ofs << " );" << std::endl;
  }
  else if ((returnIndex != ~0) && (commandData.returnType != returnType))
  {
    ofs << indentation << "  return " << reduceName(commandData.arguments[returnIndex].name) << ";" << std::endl;
  }

  ofs << indentation << "}" << std::endl;
}

void writeFunctionHeader(std::ofstream & ofs, VkData const& vkData, std::string const& indentation, std::string const& returnType, std::string const& name, CommandData const& commandData, size_t returnIndex, size_t templateIndex, std::map<size_t, size_t> const& vectorParameters, bool singular)
{
  std::set<size_t> skippedArguments;
  for (std::map<size_t, size_t>::const_iterator it = vectorParameters.begin(); it != vectorParameters.end(); ++it)
  {
    if (it->second != ~0)
    {
      skippedArguments.insert(it->second);
    }
  }
  if ((vectorParameters.size() == 1)
      && ((commandData.arguments[vectorParameters.begin()->first].len == "dataSize/4") || (commandData.arguments[vectorParameters.begin()->first].len == "latexmath:[$dataSize \\over 4$]")))
  {
    assert(commandData.arguments[3].name == "dataSize");
    skippedArguments.insert(3);
  }
  if (returnIndex != ~0)
  {
    skippedArguments.insert(returnIndex);
  }

  ofs << indentation;
  if ( !singular && (templateIndex != ~0) && ((templateIndex != returnIndex) || (returnType == "Result")))
  {
    assert(returnType.find("Allocator") == std::string::npos);
    ofs << "template <typename T>" << std::endl
        << indentation;
  }
  else if (!singular && (returnType.find("Allocator") != std::string::npos))
  {
    assert((returnType.substr(0, 12) == "std::vector<") && (returnType.find(',') != std::string::npos) && (12 < returnType.find(',')));
    ofs << "template <typename Allocator = std::allocator<" << returnType.substr(12, returnType.find(',') - 12) << ">>" << std::endl
        << indentation;
    if ((returnType != commandData.returnType) && (commandData.returnType != "void"))
    {
      ofs << "typename ";
    }
  }
  else if (!commandData.handleCommand)
  {
    ofs << "VULKAN_HPP_INLINE ";
  }
  if ((returnType != commandData.returnType) && (commandData.returnType != "void"))
  {
    assert(commandData.returnType == "Result");
    ofs << "ResultValueType<" << (singular ? commandData.arguments[returnIndex].pureType : returnType) << ">::type ";
  }
  else if ((returnIndex != ~0) && (1 < commandData.successCodes.size()))
  {
    assert(commandData.returnType == "Result");
    ofs << "ResultValue<" << commandData.arguments[returnIndex].pureType << "> ";
  }
  else
  {
    ofs << returnType << " ";
  }
  ofs << reduceName(name, singular) << "(";
  if (skippedArguments.size() + (commandData.handleCommand ? 1 : 0) < commandData.arguments.size())
  {
    size_t lastArgument = ~0;
    for (size_t i = commandData.arguments.size() - 1; i < commandData.arguments.size(); i--)
    {
      if (skippedArguments.find(i) == skippedArguments.end())
      {
        lastArgument = i;
        break;
      }
    }

    ofs << " ";
    bool argEncountered = false;
    for (size_t i = commandData.handleCommand ? 1 : 0; i < commandData.arguments.size(); i++)
    {
      if (skippedArguments.find(i) == skippedArguments.end())
      {
        if (argEncountered)
        {
          ofs << ", ";
        }

        std::map<size_t, size_t>::const_iterator it = vectorParameters.find(i);
        size_t pos = commandData.arguments[i].type.find('*');
        if (it == vectorParameters.end())
        {
          if (pos == std::string::npos)
          {
            ofs << commandData.arguments[i].type << " " << reduceName(commandData.arguments[i].name);
            if (!commandData.arguments[i].arraySize.empty())
            {
              ofs << "[" << commandData.arguments[i].arraySize << "]";
            }
            if (lastArgument == i)
            {
              std::map<std::string, FlagData>::const_iterator flagIt = vkData.flags.find(commandData.arguments[i].pureType);
              if (flagIt != vkData.flags.end())
              {
                std::list<DependencyData>::const_iterator depIt = std::find_if(vkData.dependencies.begin(), vkData.dependencies.end(), [&flagIt](DependencyData const& dd) { return(dd.name == flagIt->first); });
                assert(depIt != vkData.dependencies.end());
                assert(depIt->dependencies.size() == 1);
                std::map<std::string, EnumData>::const_iterator enumIt = vkData.enums.find(*depIt->dependencies.begin());
                assert(enumIt != vkData.enums.end());
                if (enumIt->second.members.empty())
                {
                  ofs << " = " << commandData.arguments[i].pureType << "()";
                }
              }
            }
          }
          else
          {
            assert(commandData.arguments[i].type[pos] == '*');
            if (commandData.arguments[i].optional)
            {
              ofs << "Optional<" << trimEnd(commandData.arguments[i].type.substr(0, pos)) << "> " << reduceName(commandData.arguments[i].name) << " = nullptr";
            }
            else if (commandData.arguments[i].type.find("char") == std::string::npos)
            {
              ofs << trimEnd(commandData.arguments[i].type.substr(0, pos)) << " & " << reduceName(commandData.arguments[i].name);
            }
            else
            {
              ofs << "const std::string & " << reduceName(commandData.arguments[i].name);
            }
          }
        }
        else
        {
          bool optional = commandData.arguments[i].optional && ((it == vectorParameters.end()) || (it->second == ~0));
          assert(pos != std::string::npos);
          assert(commandData.arguments[i].type[pos] == '*');
          if (commandData.arguments[i].type.find("char") != std::string::npos)
          {
            if (optional)
            {
              ofs << "Optional<const std::string> " << reduceName(commandData.arguments[i].name) << " = nullptr";
            }
            else
            {
              ofs << "const std::string & " << reduceName(commandData.arguments[i].name);
            }
          }
          else
          {
            assert(!optional);
            if (singular)
            {
              ofs << trimEnd(commandData.arguments[i].type.substr(0, pos)) << " & " << reduceName(commandData.arguments[i].name, true);
            }
            else
            {
              bool isConst = (commandData.arguments[i].type.find("const") != std::string::npos);
              ofs << "ArrayProxy<" << ((templateIndex == i) ? (isConst ? "const T" : "T") : trimEnd(commandData.arguments[i].type.substr(0, pos))) << "> " << reduceName(commandData.arguments[i].name);
            }
          }
        }
        argEncountered = true;
      }
    }
    ofs << " ";
  }
  ofs << ")";
  if (commandData.handleCommand)
  {
    ofs << " const";
  }
  ofs << std::endl;
}

void writeMemberData(std::ofstream & ofs, MemberData const& memberData, std::set<std::string> const& vkTypes)
{
  if ( vkTypes.find( memberData.pureType ) != vkTypes.end() )
  {
    if ( memberData.type.back() == '*' )
    {
      ofs << "reinterpret_cast<";
      if ( memberData.type.find( "const" ) == 0 )
      {
        ofs << "const ";
      }
      ofs << "Vk" << memberData.pureType << '*';
    }
    else
    {
      ofs << "static_cast<Vk" << memberData.pureType;
    }
    ofs << ">( " << memberData.name << " )";
  }
  else
  {
    ofs << memberData.name;
  }
}

void writeStructConstructor( std::ofstream & ofs, std::string const& name, StructData const& structData, std::set<std::string> const& vkTypes, std::map<std::string,std::string> const& defaultValues )
{
  // the constructor with all the elements as arguments, with defaults
  ofs << "    " << name << "( ";
  bool listedArgument = false;
  for (size_t i = 0; i<structData.members.size(); i++)
  {
    if (listedArgument)
    {
      ofs << ", ";
    }
    if ((structData.members[i].name != "pNext") && (structData.members[i].name != "sType"))
    {
      std::map<std::string, std::string>::const_iterator defaultIt = defaultValues.find(structData.members[i].pureType);
      assert(defaultIt != defaultValues.end());
      if (structData.members[i].arraySize.empty())
      {
        ofs << structData.members[i].type + " " + structData.members[i].name << "_ = " << (structData.members[i].type.back() == '*' ? "nullptr" : defaultIt->second);
      }
      else
      {
        ofs << "std::array<" + structData.members[i].type + "," + structData.members[i].arraySize + "> const& " + structData.members[i].name << "_ = { { " << defaultIt->second;
        size_t n = atoi(structData.members[i].arraySize.c_str());
        assert(0 < n);
        for (size_t j = 1; j < n; j++)
        {
          ofs << ", " << defaultIt->second;
        }
        ofs << " } }";
      }
      listedArgument = true;
    }
  }
  ofs << " )" << std::endl;

  // copy over the simple arguments
  bool firstArgument = true;
  for (size_t i = 0; i < structData.members.size(); i++)
  {
    if (structData.members[i].arraySize.empty())
    {
      ofs << "      " << (firstArgument ? ":" : ",") << " " << structData.members[i].name << "( ";
      if (structData.members[i].name == "pNext")
      {
        ofs << "nullptr";
      }
      else if (structData.members[i].name == "sType")
      {
        ofs << "StructureType::e" << name;
      }
      else
      {
        ofs << structData.members[i].name << "_";
      }
      ofs << " )" << std::endl;
      firstArgument = false;
    }
  }

  // the body of the constructor, copying over data from argument list into wrapped struct
  ofs << "    {" << std::endl;
  for ( size_t i=0 ; i<structData.members.size() ; i++ )
  {
    if ( !structData.members[i].arraySize.empty() )
    {
      ofs << "      memcpy( &" << structData.members[i].name << ", " << structData.members[i].name << "_.data(), " << structData.members[i].arraySize << " * sizeof( " << structData.members[i].type << " ) );" << std::endl;
    }
  }
  ofs << "    }" << std::endl
      << std::endl;

  // the copy constructor from a native struct (Vk...)
  ofs << "    " << name << "( Vk" << name << " const & rhs )" << std::endl
      << "    {" << std::endl
      << "      memcpy( this, &rhs, sizeof(" << name << ") );" << std::endl
      << "    }" << std::endl
      << std::endl;

  // the assignment operator from a native sturct (Vk...)
  ofs << "    " << name << "& operator=( Vk" << name << " const & rhs )" << std::endl
    << "    {" << std::endl
    << "      memcpy( this, &rhs, sizeof(" << name << ") );" << std::endl
    << "      return *this;" << std::endl
    << "    }" << std::endl
    << std::endl;
}

void writeStructSetter( std::ofstream & ofs, std::string const& name, MemberData const& memberData, std::set<std::string> const& vkTypes )
{
  ofs << "    " << name << "& set" << static_cast<char>(toupper(memberData.name[0])) << memberData.name.substr(1) << "( ";
  if ( memberData.arraySize.empty() )
  {
    ofs << memberData.type << " ";
  }
  else
  {
    ofs << "std::array<" << memberData.type << "," << memberData.arraySize << "> ";
  }
  ofs << memberData.name << "_ )" << std::endl
      << "    {" << std::endl;
  if ( !memberData.arraySize.empty() )
  {
    ofs << "      memcpy( &" << memberData.name << ", " << memberData.name << "_.data(), " << memberData.arraySize << " * sizeof( " << memberData.type << " ) )";
  }
  else
  {
    ofs << "      " << memberData.name << " = " << memberData.name << "_";
  }
  ofs << ";" << std::endl
      << "      return *this;" << std::endl
      << "    }" << std::endl
      << std::endl;
}

void writeTypeCommand(std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData)
{
  assert(vkData.commands.find(dependencyData.name) != vkData.commands.end());
  CommandData const& commandData = vkData.commands.find(dependencyData.name)->second;
  if (!commandData.handleCommand)
  {
    writeTypeCommandStandard(ofs, "  ", dependencyData.name, dependencyData, commandData, vkData.vkTypes);

    ofs << std::endl
        << "#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE" << std::endl;
    writeTypeCommandEnhanced(ofs, vkData, "  ", "", dependencyData.name, dependencyData, commandData);
    ofs << "#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/" << std::endl
        << std::endl;
  }
}

void writeTypeCommandEnhanced(std::ofstream & ofs, VkData const& vkData, std::string const& indentation, std::string const& className, std::string const& functionName, DependencyData const& dependencyData, CommandData const& commandData)
{
  enterProtect(ofs, commandData.protect);
  std::map<size_t, size_t> vectorParameters = getVectorParameters(commandData);
  size_t returnIndex = findReturnIndex(commandData, vectorParameters);
  size_t templateIndex = findTemplateIndex(commandData, vectorParameters);
  std::map<size_t, size_t>::const_iterator returnVector = vectorParameters.find(returnIndex);
  std::string returnType = determineReturnType(commandData, returnIndex, returnVector != vectorParameters.end());

  writeFunctionHeader(ofs, vkData, indentation, returnType, functionName, commandData, returnIndex, templateIndex, vectorParameters, false);
  writeFunctionBody(ofs, indentation, className, functionName, returnType, templateIndex, dependencyData, commandData, vkData.vkTypes, returnIndex, vectorParameters, false);

  // determine candidates for singular version of function
  if ((returnVector != vectorParameters.end()) && (returnVector->second != ~0) && (commandData.arguments[returnVector->second].type.back() != '*'))
  {
    ofs << std::endl;
    writeFunctionHeader(ofs, vkData, indentation, returnType, functionName, commandData, returnIndex, templateIndex, vectorParameters, true);
    writeFunctionBody(ofs, indentation, className, functionName, returnType, templateIndex, dependencyData, commandData, vkData.vkTypes, returnIndex, vectorParameters, true);
  }
  leaveProtect(ofs, commandData.protect);
}

void writeTypeCommandStandard(std::ofstream & ofs, std::string const& indentation, std::string const& functionName, DependencyData const& dependencyData, CommandData const& commandData, std::set<std::string> const& vkTypes)
{
  enterProtect(ofs, commandData.protect);
  ofs << indentation;
  if (!commandData.handleCommand)
  {
    ofs << "VULKAN_HPP_INLINE ";
  }
  ofs << commandData.returnType << " " << functionName << "( ";
  bool argEncountered = false;
  for (size_t i = commandData.handleCommand ? 1 : 0; i < commandData.arguments.size(); i++)
  {
    if (argEncountered)
    {
      ofs << ", ";
    }
    ofs << commandData.arguments[i].type << " " << commandData.arguments[i].name;
    if (!commandData.arguments[i].arraySize.empty())
    {
      ofs << "[" << commandData.arguments[i].arraySize << "]";
    }
    argEncountered = true;
  }
  ofs << " )";
  if (commandData.handleCommand)
  {
    ofs << " const";
  }
  ofs << std::endl
      << indentation << "{" << std::endl
      << indentation << "  ";
  bool castReturn = false;
  if (commandData.returnType != "void")
  {
    ofs << "return ";
    castReturn = (vkTypes.find(commandData.returnType) != vkTypes.end());
    if (castReturn)
    {
      ofs << "static_cast<" << commandData.returnType << ">( ";
    }
  }

  std::string callName(dependencyData.name);
  assert(islower(callName[0]));
  callName[0] = toupper(callName[0]);

  ofs << "vk" << callName << "( ";
  if (commandData.handleCommand)
  {
    ofs << "m_" << commandData.arguments[0].name;
  }
  argEncountered = false;
  for (size_t i = commandData.handleCommand ? 1 : 0; i < commandData.arguments.size(); i++)
  {
    if (0 < i)
    {
      ofs << ", ";
    }
    writeMemberData(ofs, commandData.arguments[i], vkTypes);
  }
  ofs << " )";
  if (castReturn)
  {
    ofs << " )";
  }
  ofs << ";" << std::endl
      << indentation << "}" << std::endl;
  leaveProtect(ofs, commandData.protect);
}

void writeTypeEnum( std::ofstream & ofs, DependencyData const& dependencyData, EnumData const& enumData )
{
  enterProtect(ofs, enumData.protect);
  ofs << "  enum class " << dependencyData.name << std::endl
      << "  {" << std::endl;
  for ( size_t i=0 ; i<enumData.members.size() ; i++ )
  {
    ofs << "    " << enumData.members[i].name << " = " << enumData.members[i].value;
    if ( i < enumData.members.size() - 1 )
    {
      ofs << ",";
    }
    ofs << std::endl;
  }
  ofs << "  };" << std::endl;
  leaveProtect(ofs, enumData.protect);
  ofs << std::endl;
}

void writeEnumsToString(std::ofstream & ofs, DependencyData const& dependencyData, EnumData const& enumData)
{
  enterProtect(ofs, enumData.protect);
  ofs << "  VULKAN_HPP_INLINE std::string to_string(" << dependencyData.name << (enumData.members.empty() ? ")" : " value)") << std::endl
      << "  {" << std::endl;
  if (enumData.members.empty())
  {
    ofs << "    return \"(void)\";" << std::endl;
  }
  else
  {
    ofs << "    switch (value)" << std::endl
        << "    {" << std::endl;
    for (auto itMember = enumData.members.begin(); itMember != enumData.members.end(); ++itMember)
    {
      ofs << "    case " << dependencyData.name << "::" << itMember->name << ": return \"" << itMember->name.substr(1) << "\";" << std::endl;
    }
    ofs << "    default: return \"invalid\";" << std::endl
        << "    }" << std::endl;
  }
  ofs << "  }" << std::endl;
  leaveProtect(ofs, enumData.protect);
  ofs << std::endl;
}

void writeFlagsToString(std::ofstream & ofs, DependencyData const& dependencyData, EnumData const &enumData)
{
  enterProtect(ofs, enumData.protect);
  std::string enumPrefix = *dependencyData.dependencies.begin() + "::";
  ofs << "  VULKAN_HPP_INLINE std::string to_string(" << dependencyData.name << (enumData.members.empty() ? ")" : " value)") << std::endl
      << "  {" << std::endl;
  if (enumData.members.empty())
  {
    ofs << "    return \"{}\";" << std::endl;
  }
  else
  {
    ofs << "    if (!value) return \"{}\";" << std::endl
        << "    std::string result;" << std::endl;

    for (auto itMember = enumData.members.begin(); itMember != enumData.members.end(); ++itMember)
    {
      ofs << "    if (value & " << enumPrefix + itMember->name << ") result += \"" << itMember->name.substr(1) << " | \";" << std::endl;
    }
    ofs << "    return \"{\" + result.substr(0, result.size() - 3) + \"}\";" << std::endl;
  }
  ofs << "  }" << std::endl;
  leaveProtect(ofs, enumData.protect);
  ofs << std::endl;
}

void writeEnumsToString(std::ofstream & ofs, VkData const& vkData)
{
  for (auto it = vkData.dependencies.begin(); it != vkData.dependencies.end(); ++it)
  {
    switch (it->category)
    {
    case DependencyData::Category::ENUM:
      assert(vkData.enums.find(it->name) != vkData.enums.end());
      writeEnumsToString(ofs, *it, vkData.enums.find(it->name)->second);
      break;
    case DependencyData::Category::FLAGS:
      writeFlagsToString(ofs, *it, vkData.enums.find(*it->dependencies.begin())->second);
      break;
    }
  }
}

void writeTypeFlags(std::ofstream & ofs, DependencyData const& dependencyData, FlagData const& flagData, std::map<std::string, EnumData>::const_iterator enumData)
{
  assert( dependencyData.dependencies.size() == 1 );
  enterProtect(ofs, flagData.protect);
  ofs << "  using " << dependencyData.name << " = Flags<" << *dependencyData.dependencies.begin() << ", Vk" << dependencyData.name << ">;" << std::endl
      << std::endl
      << "  VULKAN_HPP_INLINE " << dependencyData.name << " operator|( " << *dependencyData.dependencies.begin() << " bit0, " << *dependencyData.dependencies.begin() << " bit1 )" << std::endl
      << "  {" << std::endl
      << "    return " << dependencyData.name << "( bit0 ) | bit1;" << std::endl
      << "  }" << std::endl;
  if (!enumData->second.members.empty())
  {
    ofs << std::endl
        << "  VULKAN_HPP_INLINE " << dependencyData.name << " operator~( " << *dependencyData.dependencies.begin() << " bits )" << std::endl
        << "  {" << std::endl
        << "    return ~( " << dependencyData.name << "( bits ) );" << std::endl
        << "  }" << std::endl
        << std::endl
        << "  template <> struct FlagTraits<" << *dependencyData.dependencies.begin() << ">" << std::endl
        << "  {" << std::endl
        << "    enum" << std::endl
        << "    {" << std::endl
        << "      allFlags = ";
    for (size_t i = 0; i < enumData->second.members.size(); i++)
    {
      if (i != 0)
      {
        ofs << " | ";
      }
      ofs << "VkFlags(" << *dependencyData.dependencies.begin() << "::" << enumData->second.members[i].name << ")";
    }
    ofs << std::endl
        << "    };" << std::endl
        << "  };" << std::endl;
  }
  leaveProtect(ofs, flagData.protect);
  ofs << std::endl;
}

void writeTypeHandle(std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, HandleData const& handle, std::list<DependencyData> const& dependencies)
{
  std::string memberName = dependencyData.name;
  assert(isupper(memberName[0]));
  memberName[0] = tolower(memberName[0]);

  ofs << "  class " << dependencyData.name << std::endl
      << "  {" << std::endl
      << "  public:" << std::endl
      << "    " << dependencyData.name << "()" << std::endl
      << "      : m_" << memberName << "(VK_NULL_HANDLE)" << std::endl
      << "    {}" << std::endl
      << std::endl
      << "#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)" << std::endl
      // construct from native handle
      << "    " << dependencyData.name << "(Vk" << dependencyData.name << " " << memberName << ")" << std::endl
      << "       : m_" << memberName << "("  << memberName << ")" << std::endl
      << "    {}" << std::endl
      << std::endl
      // assignment from native handle
      << "    " << dependencyData.name << "& operator=(Vk" << dependencyData.name << " " << memberName << ")" << std::endl
      << "    {" << std::endl
      << "      m_" << memberName << " = " << memberName << ";" << std::endl
      << "      return *this;" << std::endl
      << "    }" << std::endl
      << "#endif\n"
      << std::endl
      // operator==
      << "    bool operator==" << "(" << dependencyData.name << " const &rhs) const" << std::endl
      << "    {" << std::endl
      << "      return m_" << memberName << " == rhs.m_" << memberName << ";" << std::endl
      << "    }" << std::endl
      << std::endl
      // operator!=
      << "    bool operator!=" << "(" << dependencyData.name << " const &rhs) const" << std::endl
      << "    {" << std::endl
      << "      return m_" << memberName << " != rhs.m_" << memberName << ";" << std::endl
      << "    }" << std::endl
      << std::endl
      // operator<
      << "    bool operator<" << "(" << dependencyData.name << " const &rhs) const" << std::endl
      << "    {" << std::endl
      << "      return m_" << memberName << " < rhs.m_" << memberName << ";" << std::endl
      << "    }" << std::endl
      << std::endl;

  if (!handle.commands.empty())
  {
    for (size_t i = 0; i < handle.commands.size(); i++)
    {
      std::string commandName = handle.commands[i];
      std::map<std::string, CommandData>::const_iterator cit = vkData.commands.find(commandName);
      assert((cit != vkData.commands.end()) && cit->second.handleCommand);
      std::list<DependencyData>::const_iterator dep = std::find_if(dependencies.begin(), dependencies.end(), [commandName](DependencyData const& dd) { return dd.name == commandName; });
      assert(dep != dependencies.end());
      std::string className = dependencyData.name;
      std::string functionName = determineFunctionName(dep->name, cit->second);

      bool hasPointers = hasPointerArguments(cit->second);
      if (!hasPointers)
      {
        ofs << "#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE" << std::endl;
      }
      writeTypeCommandStandard(ofs, "    ", functionName, *dep, cit->second, vkData.vkTypes);
      if (!hasPointers)
      {
        ofs << "#endif /*!VULKAN_HPP_DISABLE_ENHANCED_MODE*/" << std::endl;
      }

      ofs << std::endl
          << "#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE" << std::endl;
      writeTypeCommandEnhanced(ofs, vkData, "    ", className, functionName, *dep, cit->second);
      ofs << "#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/" << std::endl;

      if (i < handle.commands.size() - 1)
      {
        ofs << std::endl;
      }
    }
    ofs << std::endl;
  }
  ofs << "#if !defined(VULKAN_HPP_TYPESAFE_CONVERSION)" << std::endl
      << "    explicit" << std::endl
      << "#endif" << std::endl
      << "    operator Vk" << dependencyData.name << "() const" << std::endl
      << "    {" << std::endl
      << "      return m_" << memberName << ";" << std::endl
      << "    }" << std::endl
      << std::endl
      << "    explicit operator bool() const" << std::endl
      << "    {" << std::endl
      << "      return m_" << memberName << " != VK_NULL_HANDLE;" << std::endl
      << "    }" << std::endl
      << std::endl
      << "    bool operator!() const" << std::endl
      << "    {" << std::endl
      << "      return m_" << memberName << " == VK_NULL_HANDLE;" << std::endl
      << "    }" << std::endl
      << std::endl
      << "  private:" << std::endl
      << "    Vk" << dependencyData.name << " m_" << memberName << ";" << std::endl
      << "  };" << std::endl
#if 1
      << "  static_assert( sizeof( " << dependencyData.name << " ) == sizeof( Vk" << dependencyData.name << " ), \"handle and wrapper have different size!\" );" << std::endl
#endif
      << std::endl;
}

void writeTypeScalar( std::ofstream & ofs, DependencyData const& dependencyData )
{
  assert( dependencyData.dependencies.size() == 1 );
  ofs << "  using " << dependencyData.name << " = " << *dependencyData.dependencies.begin() << ";" << std::endl
      << std::endl;
}

bool containsUnion(std::string const& type, std::map<std::string, StructData> const& structs)
{
  std::map<std::string, StructData>::const_iterator sit = structs.find(type);
  bool found = (sit != structs.end());
  if (found)
  {
    found = sit->second.isUnion;
    for (std::vector<MemberData>::const_iterator mit = sit->second.members.begin(); mit != sit->second.members.end() && !found; ++mit)
    {
      found = (mit->type == mit->pureType) && containsUnion(mit->type, structs);
    }
  }
  return found;
}

void writeTypeStruct( std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, std::map<std::string,std::string> const& defaultValues )
{
  std::map<std::string,StructData>::const_iterator it = vkData.structs.find( dependencyData.name );
  assert( it != vkData.structs.end() );

  enterProtect(ofs, it->second.protect);
  ofs << "  struct " << dependencyData.name << std::endl
      << "  {" << std::endl;

  // only structs that are not returnedOnly get a constructor!
  if ( !it->second.returnedOnly )
  {
    writeStructConstructor( ofs, dependencyData.name, it->second, vkData.vkTypes, defaultValues );
  }

  // create the setters
  if (!it->second.returnedOnly)
  {
    for (size_t i = 0; i<it->second.members.size(); i++)
    {
      writeStructSetter( ofs, dependencyData.name, it->second.members[i], vkData.vkTypes );
    }
  }

  // the cast-operator to the wrapped struct
  ofs << "    operator const Vk" << dependencyData.name << "&() const" << std::endl
      << "    {" << std::endl
      << "      return *reinterpret_cast<const Vk" << dependencyData.name << "*>(this);" << std::endl
      << "    }" << std::endl
      << std::endl;

  // operator==() and operator!=()
  // only structs without a union as a member can have a meaningfull == and != operation; we filter them out
  if (!containsUnion(dependencyData.name, vkData.structs))
  {
    ofs << "    bool operator==( " << dependencyData.name << " const& rhs ) const" << std::endl
        << "    {" << std::endl
        << "      return ";
    for (size_t i = 0; i < it->second.members.size(); i++)
    {
      if (i != 0)
      {
        ofs << std::endl << "          && ";
      }
      if (!it->second.members[i].arraySize.empty())
      {
        ofs << "( memcmp( " << it->second.members[i].name << ", rhs." << it->second.members[i].name << ", " << it->second.members[i].arraySize << " * sizeof( " << it->second.members[i].type << " ) ) == 0 )";
      }
      else
      {
        ofs << "( " << it->second.members[i].name << " == rhs." << it->second.members[i].name << " )";
      }
    }
    ofs << ";" << std::endl
        << "    }" << std::endl
        << std::endl
        << "    bool operator!=( " << dependencyData.name << " const& rhs ) const" << std::endl
        << "    {" << std::endl
        << "      return !operator==( rhs );" << std::endl
        << "    }" << std::endl
        << std::endl;
  }

  // the member variables
  for (size_t i = 0; i < it->second.members.size(); i++)
  {
    if (it->second.members[i].type == "StructureType")
    {
      assert((i == 0) && (it->second.members[i].name == "sType"));
      ofs << "  private:" << std::endl
          << "    StructureType sType;" << std::endl
          << std::endl
          << "  public:" << std::endl;
    }
    else
    {
      ofs << "    " << it->second.members[i].type << " " << it->second.members[i].name;
      if (!it->second.members[i].arraySize.empty())
      {
        ofs << "[" << it->second.members[i].arraySize << "]";
      }
      ofs << ";" << std::endl;
    }
  }
  ofs << "  };" << std::endl
      << "  static_assert( sizeof( " << dependencyData.name << " ) == sizeof( Vk" << dependencyData.name << " ), \"struct and wrapper have different size!\" );" << std::endl;

  leaveProtect(ofs, it->second.protect);
  ofs << std::endl;
}

void writeTypeUnion( std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, StructData const& unionData, std::map<std::string,std::string> const& defaultValues )
{
  std::ostringstream oss;
  ofs << "  union " << dependencyData.name << std::endl
      << "  {" << std::endl;

  for ( size_t i=0 ; i<unionData.members.size() ; i++ )
  {
    // one constructor per union element
    ofs << "    " << dependencyData.name << "( ";
    if ( unionData.members[i].arraySize.empty() )
    {
      ofs << unionData.members[i].type << " ";
    }
    else
    {
      ofs << "const std::array<" << unionData.members[i].type << "," << unionData.members[i].arraySize << ">& ";
    }
    ofs << unionData.members[i].name << "_";

    // just the very first constructor gets default arguments
    if ( i == 0 )
    {
      std::map<std::string,std::string>::const_iterator it = defaultValues.find( unionData.members[i].pureType );
      assert( it != defaultValues.end() );
      if ( unionData.members[i].arraySize.empty() )
      {
        ofs << " = " << it->second;
      }
      else
      {
        ofs << " = { {" << it->second << "} }";
      }
    }
    ofs << " )" << std::endl
        << "    {" << std::endl
        << "      ";
    if ( unionData.members[i].arraySize.empty() )
    {
      ofs << unionData.members[i].name << " = " << unionData.members[i].name << "_";
    }
    else
    {
      ofs << "memcpy( &" << unionData.members[i].name << ", " << unionData.members[i].name << "_.data(), " << unionData.members[i].arraySize << " * sizeof( " << unionData.members[i].type << " ) )";
    }
    ofs << ";" << std::endl
        << "    }" << std::endl
        << std::endl;
    }

  for (size_t i = 0; i<unionData.members.size(); i++)
  {
    // one setter per union element
    assert(!unionData.returnedOnly);
    writeStructSetter(ofs, dependencyData.name, unionData.members[i], vkData.vkTypes);
  }

  // the implicit cast operator to the native type
  ofs << "    operator Vk" << dependencyData.name << " const& () const" << std::endl
      << "    {" << std::endl
      << "      return *reinterpret_cast<const Vk" << dependencyData.name << "*>(this);" << std::endl
      << "    }" << std::endl
      << std::endl;

  // the union member variables
  // if there's at least one Vk... type in this union, check for unrestricted unions support
  bool needsUnrestrictedUnions = false;
  for (size_t i = 0; i < unionData.members.size() && !needsUnrestrictedUnions; i++)
  {
    needsUnrestrictedUnions = (vkData.vkTypes.find(unionData.members[i].type) != vkData.vkTypes.end());
  }
  if (needsUnrestrictedUnions)
  {
    ofs << "#ifdef VULKAN_HPP_HAS_UNRESTRICTED_UNIONS" << std::endl;
    for (size_t i = 0; i < unionData.members.size(); i++)
    {
      ofs << "    " << unionData.members[i].type << " " << unionData.members[i].name;
      if (!unionData.members[i].arraySize.empty())
      {
        ofs << "[" << unionData.members[i].arraySize << "]";
      }
      ofs << ";" << std::endl;
    }
    ofs << "#else" << std::endl;
  }
  for (size_t i = 0; i < unionData.members.size(); i++)
  {
    ofs << "    ";
    if (vkData.vkTypes.find(unionData.members[i].type) != vkData.vkTypes.end())
    {
      ofs << "Vk";
    }
    ofs << unionData.members[i].type << " " << unionData.members[i].name;
    if (!unionData.members[i].arraySize.empty())
    {
      ofs << "[" << unionData.members[i].arraySize << "]";
    }
    ofs << ";" << std::endl;
  }
  if (needsUnrestrictedUnions)
  {
    ofs << "#endif  // VULKAN_HPP_HAS_UNRESTRICTED_UNIONS" << std::endl;
  }
  ofs << "  };" << std::endl
      << std::endl;
}

void writeTypes(std::ofstream & ofs, VkData const& vkData, std::map<std::string, std::string> const& defaultValues)
{
  for ( std::list<DependencyData>::const_iterator it = vkData.dependencies.begin() ; it != vkData.dependencies.end() ; ++it )
  {
    switch( it->category )
    {
      case DependencyData::Category::COMMAND :
        writeTypeCommand( ofs, vkData, *it );
        break;
      case DependencyData::Category::ENUM :
        assert( vkData.enums.find( it->name ) != vkData.enums.end() );
        writeTypeEnum( ofs, *it, vkData.enums.find( it->name )->second );
        break;
      case DependencyData::Category::FLAGS :
        assert(vkData.flags.find(it->name) != vkData.flags.end());
        writeTypeFlags( ofs, *it, vkData.flags.find( it->name)->second, vkData.enums.find(generateEnumNameForFlags(it->name)) );
        break;
      case DependencyData::Category::FUNC_POINTER :
      case DependencyData::Category::REQUIRED :
        // skip FUNC_POINTER and REQUIRED, they just needed to be in the dependencies list to resolve dependencies
        break;
      case DependencyData::Category::HANDLE :
        assert(vkData.handles.find(it->name) != vkData.handles.end());
        writeTypeHandle(ofs, vkData, *it, vkData.handles.find(it->name)->second, vkData.dependencies);
        break;
      case DependencyData::Category::SCALAR :
        writeTypeScalar( ofs, *it );
        break;
      case DependencyData::Category::STRUCT :
        writeTypeStruct( ofs, vkData, *it, defaultValues );
        break;
      case DependencyData::Category::UNION :
        assert( vkData.structs.find( it->name ) != vkData.structs.end() );
        writeTypeUnion( ofs, vkData, *it, vkData.structs.find( it->name )->second, defaultValues );
        break;
      default :
        assert( false );
        break;
    }
  }
}

void writeVersionCheck(std::ofstream & ofs, std::string const& version)
{
  ofs << "static_assert( VK_HEADER_VERSION == " << version << " , \"Wrong VK_HEADER_VERSION!\" );" << std::endl
      << std::endl;
}

void writeTypesafeCheck(std::ofstream & ofs, std::string const& typesafeCheck)
{
  ofs << "// 32-bit vulkan is not typesafe for handles, so don't allow copy constructors on this platform by default." << std::endl
      << "// To enable this feature on 32-bit platforms please define VULKAN_HPP_TYPESAFE_CONVERSION" << std::endl
      << typesafeCheck << std::endl
      << "#define VULKAN_HPP_TYPESAFE_CONVERSION 1" << std::endl
      << "#endif" << std::endl
      << std::endl;
}

int main( int argc, char **argv )
{
  try {
    tinyxml2::XMLDocument doc;

    std::string filename = (argc == 1) ? VK_SPEC : argv[1];
    std::cout << "Loading vk.xml from " << filename << std::endl;
    std::cout << "Writing vulkan.hpp to " << VULKAN_HPP << std::endl;

    tinyxml2::XMLError error = doc.LoadFile(filename.c_str());
    if (error != tinyxml2::XML_SUCCESS)
    {
      std::cout << "VkGenerate: failed to load file " << argv[1] << " . Error code: " << error << std::endl;
      return -1;
    }

    tinyxml2::XMLElement * registryElement = doc.FirstChildElement();
    assert(strcmp(registryElement->Value(), "registry") == 0);
    assert(!registryElement->NextSiblingElement());

    VkData vkData;

    tinyxml2::XMLElement * child = registryElement->FirstChildElement();
    do
    {
      assert(child->Value());
      const std::string value = child->Value();
      if (value == "commands")
      {
        readCommands(child, vkData);
      }
      else if (value == "comment")
      {
        readComment(child, vkData.vulkanLicenseHeader);
        vkData.vulkanLicenseHeader = trim(vkData.vulkanLicenseHeader);
      }
      else if (value == "enums")
      {
        readEnums(child, vkData);
      }
      else if (value == "extensions")
      {
        readExtensions(child, vkData);
      }
      else if (value == "tags")
      {
        readTags(child, vkData.tags);
      }
      else if (value == "types")
      {
        readTypes(child, vkData);
      }
      else
      {
        assert((value == "feature") || (value == "vendorids"));
      }
    } while (child = child->NextSiblingElement());

    sortDependencies(vkData.dependencies);

    std::map<std::string, std::string> defaultValues;
    createDefaults(vkData, defaultValues);

    std::ofstream ofs(VULKAN_HPP);
    ofs << vkData.vulkanLicenseHeader << std::endl
      << std::endl
      << std::endl
      << "#ifndef VULKAN_HPP" << std::endl
      << "#define VULKAN_HPP" << std::endl
      << std::endl
      << "#include <algorithm>" << std::endl
      << "#include <array>" << std::endl
      << "#include <cassert>" << std::endl
      << "#include <cstdint>" << std::endl
      << "#include <cstring>" << std::endl
      << "#include <initializer_list>" << std::endl
      << "#include <string>" << std::endl
      << "#include <system_error>" << std::endl
      << "#include <tuple>" << std::endl
      << "#include <type_traits>" << std::endl
      << "#include <vulkan/vulkan.h>" << std::endl
      << "#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE" << std::endl
      << "# include <memory>" << std::endl
      << "# include <vector>" << std::endl
      << "#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/" << std::endl
      << std::endl;

    writeVersionCheck(ofs, vkData.version);
    writeTypesafeCheck(ofs, vkData.typesafeCheck);
    ofs << versionCheckHeader
        << inlineHeader
        << "namespace vk" << std::endl
        << "{" << std::endl
        << flagsHeader
        << optionalClassHeader
        << arrayProxyHeader;

    // first of all, write out vk::Result and the exception handling stuff
    std::list<DependencyData>::const_iterator it = std::find_if(vkData.dependencies.begin(), vkData.dependencies.end(), [](DependencyData const& dp) { return dp.name == "Result"; });
    assert(it != vkData.dependencies.end());
    writeTypeEnum(ofs, *it, vkData.enums.find(it->name)->second);
    writeEnumsToString(ofs, *it, vkData.enums.find(it->name)->second);
    vkData.dependencies.erase(it);
    ofs << exceptionHeader;

    ofs << "} // namespace vk" << std::endl
      << std::endl
      << "namespace std" << std::endl
      << "{" << std::endl
      << "  template <>" << std::endl
      << "  struct is_error_code_enum<vk::Result> : public true_type" << std::endl
      << "  {};" << std::endl
      << "}" << std::endl
      << std::endl
      << "namespace vk" << std::endl
      << "{" << std::endl
      << resultValueHeader
      << createResultValueHeader;

    writeTypes(ofs, vkData, defaultValues);
    writeEnumsToString(ofs, vkData);

    ofs << "} // namespace vk" << std::endl
      << std::endl
      << "#endif" << std::endl;
  }
  catch (std::exception const& e)
  {
    std::cout << "caught exception: " << e.what() << std::endl;
    return -1;
  }
  catch (...)
  {
    std::cout << "caught unknown exception" << std::endl;
    return -1;
  }
}
