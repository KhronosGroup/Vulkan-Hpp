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
#include <iterator>
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

std::string const versionCheckHeader = { R"(
#if !defined(VULKAN_HPP_HAS_UNRESTRICTED_UNIONS)
# if defined(__clang__)
#  if __has_feature(cxx_unrestricted_unions)
#   define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
#  endif
# elif defined(__GNUC__)
#  define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#  if 40600 <= GCC_VERSION
#   define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
#  endif
# elif defined(_MSC_VER)
#  if 1900 <= _MSC_VER
#   define VULKAN_HPP_HAS_UNRESTRICTED_UNIONS
#  endif
# endif
#endif
)"
};

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

std::string const explicitHeader = { R"(
#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
# define VULKAN_HPP_TYPESAFE_EXPLICIT
#else
# define VULKAN_HPP_TYPESAFE_EXPLICIT explicit
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

std::string const uniqueHandleHeader = { R"(
#if defined(VULKAN_HPP_NO_EXCEPTIONS) && !defined(VULKAN_HPP_NO_SMART_HANDLE)
#  define VULKAN_HPP_NO_SMART_HANDLE
#endif

#ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename Type, typename Deleter>
  class UniqueHandle
  {
  public:
    explicit UniqueHandle( Type const& value = Type(), Deleter const& deleter = Deleter() )
      : m_value( value )
      , m_deleter( deleter )
    {}

    UniqueHandle( UniqueHandle const& ) = delete;

    UniqueHandle( UniqueHandle && other )
      : m_value( other.release() )
      , m_deleter( std::move( other.m_deleter ) )
    {}

    ~UniqueHandle()
    {
      destroy();
    }

    UniqueHandle & operator=( UniqueHandle const& ) = delete;

    UniqueHandle & operator=( UniqueHandle && other )
    {
      reset( other.release() );
      m_deleter = std::move( other.m_deleter );
      return *this;
    }

    explicit operator bool() const
    {
      return m_value.operator bool();
    }

    Type const* operator->() const
    {
      return &m_value;
    }

    Type const& operator*() const
    {
      return m_value;
    }

    Type get() const
    {
      return m_value;
    }

    Deleter & getDeleter()
    {
      return m_deleter;
    }

    Deleter const& getDeleter() const
    {
      return m_deleter;
    }

    void reset( Type const& value = Type() )
    {
      if ( m_value != value )
      {
        destroy();
        m_value = value;
      }
    }

    Type release()
    {
      Type value = m_value;
      m_value = nullptr;
      return value;
    }

    void swap( UniqueHandle<Type, Deleter> & rhs )
    {
      std::swap(m_value, rhs.m_value);
      std::swap(m_deleter, rhs.m_deleter);
    }

  private:
    void destroy()
    {
      if ( m_value )
      {
        m_deleter( m_value );
      }
    }

  private:
    Type    m_value;
    Deleter m_deleter;
  };

  template <typename Type, typename Deleter>
  VULKAN_HPP_INLINE void swap( UniqueHandle<Type,Deleter> & lhs, UniqueHandle<Type,Deleter> & rhs )
  {
    lhs.swap( rhs );
  }
#endif

)"
};

struct ParamData
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
  CommandData(std::string const& t, std::string const& fn)
    : returnType(t)
    , fullName(fn)
    , returnParam(~0)
    , templateParam(~0)
    , twoStep(false)
  {}

  std::string               className;
  std::string               enhancedReturnType;
  std::string               fullName;
  std::vector<ParamData>    params;
  std::string               protect;
  std::string               reducedName;
  size_t                    returnParam;
  std::string               returnType;
  std::set<size_t>          skippedParams;
  std::vector<std::string>  successCodes;
  size_t                    templateParam;
  bool                      twoStep;
  std::map<size_t, size_t>  vectorParams;
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
  std::set<std::string> forwardDependencies;
};

struct NameValue
{
  std::string name;
  std::string value;
};

struct EnumData
{
  EnumData(std::string const& n, bool b = false)
    : name(n)
    , bitmask(b)
  {}

  void addEnumMember(std::string const & name, std::string const& tag, bool appendTag);

  std::string             name;
  std::string             prefix;
  std::string             postfix;
  std::vector<NameValue>  members;
  std::string             protect;
  bool                    bitmask;
};

struct FlagData
{
  std::string protect;
};

struct HandleData
{
  std::vector<std::string>  commands;
  std::string               protect;
};

struct ScalarData
{
  std::string protect;
};

struct MemberData
{
  std::string type;
  std::string name;
  std::string arraySize;
  std::string pureType;
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

struct DeleterData
{
  std::string pool;
  std::string call;
};

struct VkData
{
  std::map<std::string, CommandData>            commands;
  std::list<DependencyData>                     dependencies;
  std::map<std::string, std::set<std::string>>  deleterTypes; // map from parent type to set of child types
  std::map<std::string, DeleterData>            deleterData;  // map from child types to corresponding deleter data
  std::map<std::string, EnumData>               enums;
  std::map<std::string, FlagData>               flags;
  std::map<std::string, HandleData>             handles;
  std::map<std::string, ScalarData>             scalars;
  std::map<std::string, StructData>             structs;
  std::set<std::string>                         tags;
  std::string                                   typesafeCheck;
  std::string                                   version;
  std::set<std::string>                         vkTypes;
  std::string                                   vulkanLicenseHeader;
};

void createDefaults( VkData const& vkData, std::map<std::string,std::string> & defaultValues );
void determineEnhancedReturnType(CommandData & commandData);
void determineReducedName(CommandData & commandData);
void determineReturnParam(CommandData & commandData);
void determineSkippedParams(CommandData & commandData);
void determineTemplateParam(CommandData & commandData);
void determineVectorParams(CommandData & commandData);
void enterProtect(std::ostream &os, std::string const& protect);
std::string extractTag(std::string const& name);
std::string findTag(std::string const& name, std::set<std::string> const& tags);
std::string generateEnumNameForFlags(std::string const& name);
bool hasPointerParam(std::vector<ParamData> const& params);
bool isVectorSizeParam(std::map<size_t, size_t> const& vectorParams, size_t idx);
void leaveProtect(std::ostream &os, std::string const& protect);
void linkCommandToHandle(VkData & vkData, CommandData & commandData);
bool noDependencies(std::set<std::string> const& dependencies, std::map<std::string, std::string> & listedTypes);
bool readCommandParam( tinyxml2::XMLElement * element, std::set<std::string> & dependencies, std::vector<ParamData> & params );
void readCommandParamArraySize(tinyxml2::XMLNode * node, ParamData & param);
void readCommandParams(tinyxml2::XMLElement* element, std::set<std::string> & dependencies, CommandData & commandData);
tinyxml2::XMLNode* readCommandParamType(tinyxml2::XMLNode* node, ParamData& param);
CommandData& readCommandProto(tinyxml2::XMLElement * element, VkData & vkData);
void readCommands( tinyxml2::XMLElement * element, VkData & vkData );
void readCommandsCommand(tinyxml2::XMLElement * element, VkData & vkData);
std::vector<std::string> readCommandSuccessCodes(tinyxml2::XMLElement* element, std::set<std::string> const& tags);
void readComment(tinyxml2::XMLElement * element, std::string & header);
void readEnums( tinyxml2::XMLElement * element, VkData & vkData );
void readEnumsEnum( tinyxml2::XMLElement * element, EnumData & enumData, std::string const& tag );
void readDisabledExtensionRequire(tinyxml2::XMLElement * element, VkData & vkData);
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
std::string reduceName(std::string const& name, bool singular = false);
void registerDeleter(VkData & vkData, CommandData const& commandData);
void sortDependencies( std::list<DependencyData> & dependencies );
std::string strip(std::string const& value, std::string const& prefix, std::string const& tag = std::string());
std::string stripCommand(std::string const& value);
std::string toCamelCase(std::string const& value);
std::string toUpperCase(std::string const& name);
std::string trimStart(std::string const& input);
std::string trimEnd(std::string const& input);
void writeCall(std::ostream & os, CommandData const& commandData, std::set<std::string> const& vkTypes, bool firstCall, bool singular);
void writeDeleterClasses(std::ofstream & ofs, std::pair<std::string, std::set<std::string>> const& deleterTypes, std::map<std::string, DeleterData> const& deleterData);
void writeDeleterForwardDeclarations(std::ofstream &ofs, std::pair<std::string, std::set<std::string>> const& deleterTypes, std::map<std::string, DeleterData> const& deleterData);
void writeEnumsToString(std::ofstream & ofs, EnumData const& enumData);
void writeEnumsToString(std::ofstream & ofs, VkData const& vkData);
void writeFlagsToString(std::ofstream & ofs, std::string const& flagsName, EnumData const &enumData);
void writeExceptionCheck(std::ofstream & ofs, std::string const& indentation, std::string const& className, std::string const& functionName, std::vector<std::string> const& successCodes);
void writeFunctionBody(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData, bool enhanced, bool singular, bool unique);
void writeFunctionBodyEnhanced(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData, bool singular);
void writeFunctionBodyStandard(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData);
void writeFunctionBodyUnique(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData, bool singular);
void writeFunctionDeclaration(std::ofstream & ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool enhanced, bool singular, bool unique);
void writeFunctionDefinition(std::ofstream & ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData, bool enhanced, bool singular, bool unique);
void writeFunctionHeaderArguments(std::ostream & os, VkData const& vkData, CommandData const& commandData, bool enhanced, bool singular, bool withDefaults);
void writeFunctionHeaderName(std::ostream & os, std::string const& name, bool singular, bool unique);
void writeFunctionHeaderReturnType(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool enhanced, bool singular, bool unique);
void writeFunctionHeaderTemplate(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool singular, bool withDefault);
void writeStructConstructor( std::ofstream & ofs, std::string const& name, StructData const& structData, std::set<std::string> const& vkTypes, std::map<std::string,std::string> const& defaultValues );
void writeStructSetter( std::ofstream & ofs, std::string const& name, MemberData const& memberData, std::set<std::string> const& vkTypes, std::map<std::string,StructData> const& structs );
void writeTypeCommand(std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData);
void writeTypeCommandDeclaration(std::ofstream &ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData);
void writeTypeCommandDeclarationEnhanced(std::ofstream & ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData);
void writeTypeCommandDeclarationStandard(std::ofstream & ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData);
void writeTypeCommandDefinition(std::ofstream &ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData);
void writeTypeCommandDefinitionEnhanced(std::ofstream &ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData);
void writeTypeCommandDefinitionStandard(std::ofstream &ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData);
void writeTypeCommandParam(std::ostream & os, ParamData const& param, std::set<std::string> const& vkTypes);
void writeTypeEnum(std::ofstream & ofs, EnumData const& enumData);
void writeTypeFlags(std::ofstream & ofs, std::string const& flagsName, FlagData const& flagData, EnumData const& enumData);
void writeTypeHandle(std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, HandleData const& handle, std::list<DependencyData> const& dependencies);
void writeTypeScalar( std::ofstream & ofs, DependencyData const& dependencyData );
void writeTypeStruct( std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, std::map<std::string,std::string> const& defaultValues );
void writeTypeUnion( std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, StructData const& unionData, std::map<std::string,std::string> const& defaultValues );
void writeTypes(std::ofstream & ofs, VkData const& vkData, std::map<std::string, std::string> const& defaultValues);
void writeVersionCheck(std::ofstream & ofs, std::string const& version);
void writeTypesafeCheck(std::ofstream & ofs, std::string const& typesafeCheck);

void EnumData::addEnumMember(std::string const & name, std::string const& tag, bool appendTag)
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
  for ( auto it = vkData.dependencies.begin(), end = vkData.dependencies.end() ; it != end ; ++it )
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

void determineReducedName(CommandData & commandData)
{
  commandData.reducedName = commandData.fullName;
  std::string searchName = commandData.params[0].pureType;
  size_t pos = commandData.fullName.find(searchName);
  if ((pos == std::string::npos) && isupper(searchName[0]))
  {
    searchName[0] = tolower(searchName[0]);
    pos = commandData.fullName.find(searchName);
  }
  if (pos != std::string::npos)
  {
    commandData.reducedName.erase(pos, searchName.length());
  }
  else if ((searchName == "commandBuffer") && (commandData.fullName.find("cmd") == 0))
  {
    commandData.reducedName.erase(0, 3);
    pos = 0;
  }
  if ((pos == 0) && isupper(commandData.reducedName[0]))
  {
    commandData.reducedName[0] = tolower(commandData.reducedName[0]);
  }
}

void determineEnhancedReturnType(CommandData & commandData)
{
  std::string returnType;
  if ((commandData.returnParam != ~0)
    && ((commandData.returnType == "void")
      || ((commandData.returnType == "Result")
        && ((commandData.successCodes.size() == 1)
          || ((commandData.successCodes.size() == 2)
            && (commandData.successCodes[1] == "eIncomplete")
            && commandData.twoStep)))))
  {
    if (commandData.vectorParams.find(commandData.returnParam) != commandData.vectorParams.end())
    {
      if (commandData.params[commandData.returnParam].pureType == "void")
      {
        commandData.enhancedReturnType = "std::vector<uint8_t,Allocator>";
      }
      else
      {
        commandData.enhancedReturnType = "std::vector<" + commandData.params[commandData.returnParam].pureType + ",Allocator>";
      }
    }
    else
    {
      assert(commandData.params[commandData.returnParam].type.back() == '*');
      assert(commandData.params[commandData.returnParam].type.find("const") == std::string::npos);
      commandData.enhancedReturnType = commandData.params[commandData.returnParam].type;
      commandData.enhancedReturnType.pop_back();
    }
  }
  else if ((commandData.returnType == "Result") && (commandData.successCodes.size() == 1))
  {
    // an original return of type "Result" with just one successCode is changed to void, errors throw an exception
    commandData.enhancedReturnType = "void";
  }
  else
  {
    // the return type just stays the original return type
    commandData.enhancedReturnType = commandData.returnType;
  }
}

void determineReturnParam(CommandData & commandData)
{
  if ((commandData.returnType == "Result") || (commandData.returnType == "void"))
  {
    for (size_t i = 0; i < commandData.params.size(); i++)
    {
      if ((commandData.params[i].type.find('*') != std::string::npos)
        && (commandData.params[i].type.find("const") == std::string::npos)
        && !isVectorSizeParam(commandData.vectorParams, i)
        && ((commandData.vectorParams.find(i) == commandData.vectorParams.end()) || commandData.twoStep || (commandData.successCodes.size() == 1)))
      {
        auto paramIt = std::find_if(commandData.params.begin() + i + 1, commandData.params.end(), [](ParamData const& pd)
        {
          return (pd.type.find('*') != std::string::npos) && (pd.type.find("const") == std::string::npos);
        });
        commandData.returnParam = paramIt != commandData.params.end() ? ~0 : i;
      }
    }
  }
}

void determineSkippedParams(CommandData & commandData)
{
  std::for_each(commandData.vectorParams.begin(), commandData.vectorParams.end(), [&commandData](std::pair<size_t,size_t> const& vp) { if (vp.second != ~0) commandData.skippedParams.insert(vp.second); });
  if (commandData.returnParam != ~0)
  {
    commandData.skippedParams.insert(commandData.returnParam);
  }
}

void determineTemplateParam(CommandData & commandData)
{
  for (size_t i = 0; i < commandData.params.size(); i++)
  {
    if (((commandData.params[i].name == "pData") || (commandData.params[i].name == "pValues"))
      && (commandData.vectorParams.find(i) != commandData.vectorParams.end()))
    {
      commandData.templateParam = i;
    }
  }
  assert((commandData.templateParam == ~0) || (commandData.vectorParams.find(commandData.templateParam) != commandData.vectorParams.end()));
}

void determineVectorParams(CommandData & commandData)
{
  for (auto it = commandData.params.begin(), begin = it, end = commandData.params.end(); it != end; ++it)
  {
    if (!it->len.empty())
    {
      auto findLambda = [it](ParamData const& pd) { return pd.name == it->len; };
      auto findIt = std::find_if(begin, it, findLambda);
      assert((std::count_if(begin, end, findLambda) == 0) || (findIt < it));
      commandData.vectorParams.insert(std::make_pair(std::distance(begin, it), findIt < it ? std::distance(begin, findIt) : ~0));
      assert((commandData.vectorParams[std::distance(begin, it)] != ~0)
        || (it->len == "null-terminated")
        || (it->len == "pAllocateInfo::descriptorSetCount")
        || (it->len == "pAllocateInfo::commandBufferCount"));
    }
  }
}

void enterProtect(std::ostream &os, std::string const& protect)
{
  if (!protect.empty())
  {
    os << "#ifdef " << protect << std::endl;
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

std::string findTag(std::string const& name, std::set<std::string> const& tags)
{
  auto tagIt = std::find_if(tags.begin(), tags.end(), [&name](std::string const& t)
  {
    size_t pos = name.find(t);
    return (pos != std::string::npos) && (pos == name.length() - t.length());
  });
  return tagIt != tags.end() ? *tagIt : "";
}

std::string generateEnumNameForFlags(std::string const& name)
{
  std::string generatedName = name;
  size_t pos = generatedName.rfind("Flags");
  assert(pos != std::string::npos);
  generatedName.replace(pos, 5, "FlagBits");
  return generatedName;
}

bool hasPointerParam(std::vector<ParamData> const& params)
{
  auto it = std::find_if(params.begin(), params.end(), [](ParamData const& pd)
  {
    return (pd.type.find('*') != std::string::npos);
  });
  return it != params.end();
}

bool isVectorSizeParam(std::map<size_t, size_t> const& vectorParams, size_t idx)
{
  auto it = std::find_if(vectorParams.begin(), vectorParams.end(), [idx](std::pair<size_t, size_t> const& vp) { return vp.second == idx; });
  return it != vectorParams.end();
}

void leaveProtect(std::ostream &os, std::string const& protect)
{
  if (!protect.empty())
  {
    os << "#endif /*" << protect << "*/" << std::endl;
  }
}

void linkCommandToHandle(VkData & vkData, CommandData & commandData)
{
  assert(!commandData.params.empty());
  std::map<std::string, HandleData>::iterator hit = vkData.handles.find(commandData.params[0].pureType);
  if (hit == vkData.handles.end())
  {
    hit = vkData.handles.find("");
  }
  assert(hit != vkData.handles.end());

  hit->second.commands.push_back(commandData.fullName);
  commandData.className = hit->first;

  DependencyData const& commandDD = vkData.dependencies.back();
  std::list<DependencyData>::iterator handleDD = std::find_if(vkData.dependencies.begin(), vkData.dependencies.end(), [hit](DependencyData const& dd) { return dd.name == hit->first; });
  assert((handleDD != vkData.dependencies.end()) || hit->first.empty());
  if (handleDD != vkData.dependencies.end())
  {
    std::copy_if(commandDD.dependencies.begin(), commandDD.dependencies.end(), std::inserter(handleDD->dependencies, handleDD->dependencies.end()), [hit](std::string const& d) { return d != hit->first; });
  }
}

bool noDependencies(std::set<std::string> const& dependencies, std::set<std::string> & listedTypes)
{
  // false, if any of the dependencies is not in listedTypes; true if all dependencies are in listedTypes
  auto it = std::find_if(dependencies.begin(), dependencies.end(), [&listedTypes](std::string const& d) { return listedTypes.find(d) == listedTypes.end(); });
  return it == dependencies.end();
}

bool readCommandParam( tinyxml2::XMLElement * element, std::set<std::string> & dependencies, std::vector<ParamData> & params )
{
  ParamData param;

  if (element->Attribute("len"))
  {
    param.len = element->Attribute("len");
  }

  tinyxml2::XMLNode * child = readCommandParamType(element->FirstChild(), param);
  dependencies.insert(param.pureType);

  assert( child->ToElement() && ( strcmp( child->Value(), "name" ) == 0 ) );
  param.name = child->ToElement()->GetText();

  readCommandParamArraySize(child, param);

  param.optional = element->Attribute("optional") && (strcmp(element->Attribute("optional"), "true") == 0);

  params.push_back(param);

  return element->Attribute("optional") && (strcmp(element->Attribute("optional"), "false,true") == 0);
}

void readCommandParamArraySize(tinyxml2::XMLNode * node, ParamData & param)
{
  if (param.name.back() == ']')
  {
    assert(!node->NextSibling());
    size_t pos = param.name.find('[');
    assert(pos != std::string::npos);
    param.arraySize = param.name.substr(pos + 1, param.name.length() - 2 - pos);
    param.name.erase(pos);
  }
  else
  {
    tinyxml2::XMLNode * child = node->NextSibling();
    if (child)
    {
      if (child->ToText())
      {
        std::string value = child->Value();
        if (value == "[")
        {
          child = child->NextSibling();
          assert(child && child->ToElement() && (strcmp(child->Value(), "enum") == 0));
          param.arraySize = child->ToElement()->GetText();
          child = child->NextSibling();
          assert(child && child->ToText() && (strcmp(child->Value(), "]") == 0) && !child->NextSibling());
        }
        else
        {
          assert((value.front() == '[') && (value.back() == ']'));
          param.arraySize = value.substr(1, value.length() - 2);
          assert(!child->NextSibling());
        }
      }
    }
  }
}

void readCommandParams(tinyxml2::XMLElement* element, std::set<std::string> & dependencies, CommandData & commandData)
{
  assert(element);
  while (element = element->NextSiblingElement())
  {
    std::string value = element->Value();
    if (value == "param")
    {
      commandData.twoStep |= readCommandParam(element, dependencies, commandData.params);
    }
    else
    {
      assert((value == "implicitexternsyncparams") || (value == "validity"));
    }
  }
}

tinyxml2::XMLNode* readCommandParamType(tinyxml2::XMLNode* node, ParamData& param)
{
  assert(node);
  if (node->ToText())
  {
    std::string value = trimEnd(node->Value());
    assert((value == "const") || (value == "struct"));
    param.type = value + " ";
    node = node->NextSibling();
    assert(node);
  }

  assert(node->ToElement() && (strcmp(node->Value(), "type") == 0) && node->ToElement()->GetText());
  std::string type = strip(node->ToElement()->GetText(), "Vk");
  param.type += type;
  param.pureType = type;

  node = node->NextSibling();
  assert(node);
  if (node->ToText())
  {
    std::string value = trimEnd(node->Value());
    assert((value == "*") || (value == "**") || (value == "* const*"));
    param.type += value;
    node = node->NextSibling();
  }

  return node;
}

CommandData& readCommandProto(tinyxml2::XMLElement * element, VkData & vkData)
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
  std::map<std::string,CommandData>::iterator it = vkData.commands.insert( std::make_pair( name, CommandData(type, name) ) ).first;

  return it->second;
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

  CommandData& commandData = readCommandProto(child, vkData);
  commandData.successCodes = readCommandSuccessCodes(element, vkData.tags);
  readCommandParams(child, vkData.dependencies.back().dependencies, commandData);
  determineReducedName(commandData);
  linkCommandToHandle(vkData, commandData);
  registerDeleter(vkData, commandData);
  determineVectorParams(commandData);
  determineReturnParam(commandData);
  determineTemplateParam(commandData);
  determineEnhancedReturnType(commandData);
  determineSkippedParams(commandData);
}

std::vector<std::string> readCommandSuccessCodes(tinyxml2::XMLElement* element, std::set<std::string> const& tags)
{
  std::vector<std::string> results;
  if (element->Attribute("successcodes"))
  {
    std::string successCodes = element->Attribute("successcodes");
    size_t start = 0, end;
    do
    {
      end = successCodes.find(',', start);
      std::string code = successCodes.substr(start, end - start);
      std::string tag = findTag(code, tags);
      results.push_back(std::string("e") + toCamelCase(strip(code, "VK_", tag)) + tag);
      start = end + 1;
    } while (end != std::string::npos);
  }
  return results;
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

  std::string name = strip(element->Attribute("name"), "Vk");
  if ( name != "API Constants" )
  {
    vkData.dependencies.push_back( DependencyData( DependencyData::Category::ENUM, name ) );
    std::map<std::string,EnumData>::iterator it = vkData.enums.insert( std::make_pair( name, EnumData(name) ) ).first;
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
  while (child)
  {
    if ( child->Attribute( "name" ) )
    {
      enumData.addEnumMember(child->Attribute("name"), tag, false);
    }
    child = child->NextSiblingElement();
  }
}

void readDisabledExtensionRequire(tinyxml2::XMLElement * element, VkData & vkData)
{
  tinyxml2::XMLElement * child = element->FirstChildElement();
  do
  {
    std::string value = child->Value();

    if ((value == "command") || (value == "type"))
    {
      assert(child->Attribute("name"));
      std::string name = (value == "command") ? stripCommand(child->Attribute("name")) : strip(child->Attribute("name"), "Vk");
      std::list<DependencyData>::const_iterator depIt = std::find_if(vkData.dependencies.begin(), vkData.dependencies.end(), [&name](DependencyData const& dd) { return(dd.name == name); });
      assert(depIt != vkData.dependencies.end());
      vkData.dependencies.erase(depIt);

      for (auto & dep : vkData.dependencies)
      {
        dep.dependencies.erase(name);
      }

      if (value == "command")
      {
        // first unlink the command from its class
        auto commandsIt = vkData.commands.find(name);
        assert(commandsIt != vkData.commands.end());
        assert(!commandsIt->second.className.empty());
        auto handlesIt = vkData.handles.find(commandsIt->second.className);
        assert(handlesIt != vkData.handles.end());
        auto it = std::find(handlesIt->second.commands.begin(), handlesIt->second.commands.end(), name);
        assert(it != handlesIt->second.commands.end());
        handlesIt->second.commands.erase(it);

        // then remove the command
        vkData.commands.erase(name);
      }
      else
      {
        assert((vkData.structs.find(name) != vkData.structs.end()) && (vkData.vkTypes.find(name) != vkData.vkTypes.end()));
        vkData.structs.erase(name);
        vkData.vkTypes.erase(name);
      }
    }
    else
    {
      // nothing to do for enums, no other values ever encountered
      assert(value == "enum");
    }
  } while (child = child->NextSiblingElement());
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
          std::map<std::string, HandleData>::iterator hait = vkData.handles.find(name);
          if (hait != vkData.handles.end())
          {
            hait->second.protect = protect;
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
    }
    else if ( value == "enum")
    {
      // TODO process enums which don't extend existing enums
      if (child->Attribute("extends"))
      {
        assert(child->Attribute("name"));
        assert(vkData.enums.find(strip(child->Attribute("extends"), "Vk")) != vkData.enums.end());
        assert(!!child->Attribute("bitpos") + !!child->Attribute("offset") + !!child->Attribute("value") == 1);
        auto enumIt = vkData.enums.find(strip(child->Attribute("extends"), "Vk"));
        assert(enumIt != vkData.enums.end());
        enumIt->second.addEnumMember(child->Attribute("name"), child->Attribute("value") ? "" : tag, true );
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
  //assert(vkData.tags.find(tag) != vkData.tags.end());

  tinyxml2::XMLElement * child = element->FirstChildElement();
  assert(child && (strcmp(child->Value(), "require") == 0) && !child->NextSiblingElement());

  if (strcmp(element->Attribute("supported"), "disabled") == 0)
  {
    // kick out all the disabled stuff we've read before !!
    readDisabledExtensionRequire(child, vkData);
  }
  else
  {
    std::string protect;
    if (element->Attribute("protect"))
    {
      protect = element->Attribute("protect");
    }

    readExtensionRequire(child, vkData, protect, tag);
  }
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
    vkData.enums.insert(std::make_pair(requires, EnumData(requires, true)));
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
      assert( child->Attribute( "name" ) );
      vkData.dependencies.push_back( DependencyData( DependencyData::Category::REQUIRED, child->Attribute( "name" ) ) );
    }
  } while ( child = child->NextSiblingElement() );
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

void registerDeleter(VkData & vkData, CommandData const& commandData)
{
  if ((commandData.fullName.substr(0, 7) == "destroy") || (commandData.fullName.substr(0, 4) == "free"))
  {
    std::string key;
    size_t valueIndex;
    switch (commandData.params.size())
    {
    case 2:
    case 3:
      assert(commandData.params.back().pureType == "AllocationCallbacks");
      key = (commandData.params.size() == 2) ? "" : commandData.params[0].pureType;
      valueIndex = commandData.params.size() - 2;
      break;
    case 4:
      key = commandData.params[0].pureType;
      valueIndex = 3;
      assert(vkData.deleterData.find(commandData.params[valueIndex].pureType) == vkData.deleterData.end());
      vkData.deleterData[commandData.params[valueIndex].pureType].pool = commandData.params[1].pureType;
      break;
    default:
      assert(false);
    }
    if (commandData.fullName == "destroyDevice")
    {
      key = "PhysicalDevice";
    }
    assert(vkData.deleterTypes[key].find(commandData.params[valueIndex].pureType) == vkData.deleterTypes[key].end());
    vkData.deleterTypes[key].insert(commandData.params[valueIndex].pureType);
    vkData.deleterData[commandData.params[valueIndex].pureType].call = commandData.reducedName;
  }
}

void sortDependencies( std::list<DependencyData> & dependencies )
{
  std::set<std::string> listedTypes = { "VkFlags" };
  std::list<DependencyData> sortedDependencies;

  while ( !dependencies.empty() )
  {
    bool found = false;
    for ( std::list<DependencyData>::iterator it = dependencies.begin() ; it != dependencies.end() ; ++it )
    {
      if ( noDependencies( it->dependencies, listedTypes ) )
      {
        sortedDependencies.push_back( *it );
        listedTypes.insert( it->name );
        dependencies.erase( it );
        found = true;
        break;
      }
    }
    if (!found)
    {
      // resolve direct circular dependencies
      for (std::list<DependencyData>::iterator it = dependencies.begin(); !found && it != dependencies.end(); ++it)
      {
        for (std::set<std::string>::const_iterator dit = it->dependencies.begin(); dit != it->dependencies.end(); ++dit)
        {
          std::list<DependencyData>::const_iterator depIt = std::find_if(dependencies.begin(), dependencies.end(), [&dit](DependencyData const& dd) { return(dd.name == *dit); });
          if (depIt != dependencies.end())
          {
            if (depIt->dependencies.find(it->name) != depIt->dependencies.end())
            {
              // we only have just one case, for now!
              assert((it->category == DependencyData::Category::HANDLE) && (depIt->category == DependencyData::Category::STRUCT));
              it->forwardDependencies.insert(*dit);
              it->dependencies.erase(*dit);
              found = true;
              break;
            }
          }
#if !defined(NDEBUG)
          else
          {
            assert(std::find_if(sortedDependencies.begin(), sortedDependencies.end(), [&dit](DependencyData const& dd) { return(dd.name == *dit); }) != sortedDependencies.end());
          }
#endif
        }
      }
    }
    assert( found );
  }

  dependencies.swap(sortedDependencies);
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

// trim from start
std::string trimStart(std::string const& input)
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

void writeCall(std::ostream & os, CommandData const& commandData, std::set<std::string> const& vkTypes, bool firstCall, bool singular)
{
  std::map<size_t,size_t> countIndices;
  for (std::map<size_t, size_t>::const_iterator it = commandData.vectorParams.begin(); it != commandData.vectorParams.end(); ++it)
  {
    countIndices.insert(std::make_pair(it->second, it->first));
  }
  if ((commandData.vectorParams.size() == 1)
      && ((commandData.params[commandData.vectorParams.begin()->first].len == "dataSize/4") || (commandData.params[commandData.vectorParams.begin()->first].len == "latexmath:[$dataSize \\over 4$]")))
  {
    assert(commandData.params[3].name == "dataSize");
    countIndices.insert(std::make_pair(3, commandData.vectorParams.begin()->first));
  }

  assert(islower(commandData.fullName[0]));
  os << "vk" << static_cast<char>(toupper(commandData.fullName[0])) << commandData.fullName.substr(1) << "( ";
  size_t i = 0;
  if (!commandData.className.empty())
  {
    os << "m_" << commandData.params[0].name;
    i++;
  }
  for (; i < commandData.params.size(); i++)
  {
    if (0 < i)
    {
      os << ", ";
    }
    std::map<size_t, size_t>::const_iterator it = countIndices.find(i);
    if (it != countIndices.end())
    {
      if ((commandData.returnParam == it->second) && commandData.twoStep)
      {
        os << "&" << reduceName(commandData.params[it->first].name);
      }
      else
      {
        if (singular)
        {
          os << "1 ";
        }
        else
        {
          os << reduceName(commandData.params[it->second].name) << ".size() ";
        }
        if (commandData.templateParam == it->second)
        {
          os << "* sizeof( T ) ";
        }
      }
    }
    else
    {
      it = commandData.vectorParams.find(i);
      if (it != commandData.vectorParams.end())
      {
        assert(commandData.params[it->first].type.back() == '*');
        if ((commandData.returnParam == it->first) && commandData.twoStep && firstCall)
        {
          os << "nullptr";
        }
        else
        {
          std::set<std::string>::const_iterator vkit = vkTypes.find(commandData.params[it->first].pureType);
          if ((vkit != vkTypes.end()) || (it->first == commandData.templateParam))
          {
            os << "reinterpret_cast<";
            if (commandData.params[it->first].type.find("const") == 0)
            {
              os << "const ";
            }
            if (vkit != vkTypes.end())
            {
              os << "Vk";
            }
            os << commandData.params[it->first].pureType;
            if (commandData.params[it->first].type.rfind("* const") != std::string::npos)
            {
              os << "* const";
            }
            os << "*>( " << (singular ? "&" : "") << reduceName(commandData.params[it->first].name, singular) << (singular ? "" : ".data()") << " )";
          }
          else if (commandData.params[it->first].pureType == "char")
          {
            os << reduceName(commandData.params[it->first].name);
            if (commandData.params[it->first].optional)
            {
              os << " ? " << reduceName(commandData.params[it->first].name) << "->c_str() : nullptr";
            }
            else
            {
              os << ".c_str()";
            }
          }
          else
          {
            os << reduceName(commandData.params[it->first].name) << ".data()";
          }
        }
      }
      else if (vkTypes.find(commandData.params[i].pureType) != vkTypes.end())
      {
        if (commandData.params[i].type.back() == '*')
        {
          if (commandData.params[i].type.find("const") != std::string::npos)
          {
            os << "reinterpret_cast<const Vk" << commandData.params[i].pureType << "*>( ";
            if (commandData.params[i].optional)
            {
              os << "static_cast<const " << commandData.params[i].pureType << "*>( ";
            }
            else
            {
              os << "&";
            }
            os << reduceName(commandData.params[i].name) << (commandData.params[i].optional ? "))" : " )");
          }
          else
          {
            assert(!commandData.params[i].optional);
            os << "reinterpret_cast<Vk" << commandData.params[i].pureType << "*>( &" << reduceName(commandData.params[i].name) << " )";
          }
        }
        else
        {
          os << "static_cast<Vk" << commandData.params[i].pureType << ">( " << commandData.params[i].name << " )";
        }
      }
      else
      {
        if (commandData.params[i].type.back() == '*')
        {
          if (commandData.params[i].type.find("const") != std::string::npos)
          {
            if (commandData.params[i].pureType == "char")
            {
              os << reduceName(commandData.params[i].name);
              if (commandData.params[i].optional)
              {
                os << " ? " << reduceName(commandData.params[i].name) << "->c_str() : nullptr";
              }
              else
              {
                os << ".c_str()";
              }
            }
            else
            {
              assert((commandData.params[i].pureType == "void") && !commandData.params[i].optional);
              os << commandData.params[i].name;
            }
          }
          else
          {
            assert(commandData.params[i].type.find("char") == std::string::npos);
            os << "&" << reduceName(commandData.params[i].name);
          }
        }
        else
        {
          os << commandData.params[i].name;
        }
      }
    }
  }
  os << " )";
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

void writeFunctionBody(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData, bool enhanced, bool singular, bool unique)
{
  os << indentation << "{" << std::endl;

  if (enhanced)
  {
    if (unique)
    {
      writeFunctionBodyUnique(os, indentation, vkData, commandData, dependencyData, singular);
    }
    else
    {
      writeFunctionBodyEnhanced(os, indentation, vkData, commandData, dependencyData, singular);
    }
  }
  else
  {
    writeFunctionBodyStandard(os, indentation, vkData, commandData, dependencyData);
  }

  os << indentation << "}" << std::endl;
}

void writeFunctionBodyEnhanced(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData, bool singular)
{
  // add a static_assert if a type is templated and its size needs to be some multiple of the original size
  if ((commandData.templateParam != ~0) && (commandData.params[commandData.templateParam].pureType != "void"))
  {
    os << indentation << "  static_assert( sizeof( T ) % sizeof( " << commandData.params[commandData.templateParam].pureType << " ) == 0, \"wrong size of template type T\" );" << std::endl;
  }

  // add some error checks if multiple vectors need to have the same size
  if (1 < commandData.vectorParams.size())
  {
    for (std::map<size_t, size_t>::const_iterator it0 = commandData.vectorParams.begin(); it0 != commandData.vectorParams.end(); ++it0)
    {
      if (it0->first != commandData.returnParam)
      {
        for (std::map<size_t, size_t>::const_iterator it1 = std::next(it0); it1 != commandData.vectorParams.end(); ++it1)
        {
          if ((it1->first != commandData.returnParam) && (it0->second == it1->second))
          {
            os << "#ifdef VULKAN_HPP_NO_EXCEPTIONS" << std::endl
              << indentation << "  assert( " << reduceName(commandData.params[it0->first].name) << ".size() == " << reduceName(commandData.params[it1->first].name) << ".size() );" << std::endl
              << "#else" << std::endl
              << indentation << "  if ( " << reduceName(commandData.params[it0->first].name) << ".size() != " << reduceName(commandData.params[it1->first].name) << ".size() )" << std::endl
              << indentation << "  {" << std::endl
              << indentation << "    throw std::logic_error( \"vk::" << commandData.className << "::" << commandData.reducedName << ": " << reduceName(commandData.params[it0->first].name) << ".size() != " << reduceName(commandData.params[it1->first].name) << ".size()\" );" << std::endl
              << indentation << "  }" << std::endl
              << "#endif  // VULKAN_HPP_NO_EXCEPTIONS" << std::endl;
          }
        }
      }
    }
  }

  // write the local variable to hold a returned value
  if (commandData.returnParam != ~0)
  {
    if (commandData.returnType != commandData.enhancedReturnType)
    {
      os << indentation << "  " << (singular ? commandData.params[commandData.returnParam].pureType : commandData.enhancedReturnType) << " " << reduceName(commandData.params[commandData.returnParam].name, singular);

      if (!singular)
      {
        std::map<size_t, size_t>::const_iterator it = commandData.vectorParams.find(commandData.returnParam);
        if (it != commandData.vectorParams.end() && !commandData.twoStep)
        {
          std::string size;
          if ((it->second == ~0) && !commandData.params[commandData.returnParam].len.empty())
          {
            size = reduceName(commandData.params[commandData.returnParam].len);
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
            for (std::map<size_t, size_t>::const_iterator sit = commandData.vectorParams.begin(); sit != commandData.vectorParams.end(); ++sit)
            {
              if ((sit->first != commandData.returnParam) && (sit->second == it->second))
              {
                size = reduceName(commandData.params[sit->first].name) + ".size()";
                break;
              }
            }
          }
          assert(!size.empty());
          os << "( " << size << " )";
        }
      }
      os << ";" << std::endl;
    }
    else if (1 < commandData.successCodes.size())
    {
      os << indentation << "  " << commandData.params[commandData.returnParam].pureType << " " << reduceName(commandData.params[commandData.returnParam].name) << ";" << std::endl;
    }
  }

  // local count variable to hold the size of the vector to fill
  if (commandData.twoStep)
  {
    assert(commandData.returnParam != ~0);

    std::map<size_t, size_t>::const_iterator returnit = commandData.vectorParams.find(commandData.returnParam);
    assert(returnit != commandData.vectorParams.end() && (returnit->second != ~0));
    assert((commandData.returnType == "Result") || (commandData.returnType == "void"));

    os << indentation << "  " << commandData.params[returnit->second].pureType << " " << reduceName(commandData.params[returnit->second].name) << ";" << std::endl;
  }

  // write the function call
  os << indentation << "  ";
  std::string localIndentation = "  ";
  if (commandData.returnType == "Result")
  {
    os << "Result result";
    if (commandData.twoStep && (1 < commandData.successCodes.size()))
    {
      os << ";" << std::endl
        << indentation << "  do" << std::endl
        << indentation << "  {" << std::endl
        << indentation << "    result";
      localIndentation += "  ";
    }
    os << " = static_cast<Result>( ";
  }
  else if (commandData.returnType != "void")
  {
    assert(!commandData.twoStep);
    os << "return ";
  }
  assert(dependencyData.name == commandData.fullName);
  writeCall(os, commandData, vkData.vkTypes, true, singular);
  if (commandData.returnType == "Result")
  {
    os << " )";
  }
  os << ";" << std::endl;

  if (commandData.twoStep)
  {
    std::map<size_t, size_t>::const_iterator returnit = commandData.vectorParams.find(commandData.returnParam);

    if (commandData.returnType == "Result")
    {
      os << indentation << localIndentation << "if ( ( result == Result::eSuccess ) && " << reduceName(commandData.params[returnit->second].name) << " )" << std::endl
        << indentation << localIndentation << "{" << std::endl
        << indentation << localIndentation << "  ";
    }
    else
    {
      os << indentation << "  ";
    }

    // resize the vector to hold the data according to the result from the first call
    os << reduceName(commandData.params[returnit->first].name) << ".resize( " << reduceName(commandData.params[returnit->second].name) << " );" << std::endl;

    // write the function call a second time
    if (commandData.returnType == "Result")
    {
      os << indentation << localIndentation << "  result = static_cast<Result>( ";
    }
    else
    {
      os << indentation << "  ";
    }
    assert(!singular);
    assert(dependencyData.name == commandData.fullName);
    writeCall(os, commandData, vkData.vkTypes, false, singular);
    if (commandData.returnType == "Result")
    {
      os << " )";
    }
    os << ";" << std::endl;
    if (commandData.returnType == "Result")
    {
      os << indentation << localIndentation << "}" << std::endl;
      if (1 < commandData.successCodes.size())
      {
        os << indentation << "  } while ( result == Result::eIncomplete );" << std::endl
          << indentation << "  assert( " << reduceName(commandData.params[returnit->second].name) << " <= " << reduceName(commandData.params[returnit->first].name) << ".size() ); " << std::endl
          << indentation << "  " << reduceName(commandData.params[returnit->first].name) << ".resize( " << reduceName(commandData.params[returnit->second].name) << " ); " << std::endl;
      }
    }
  }

  if ((commandData.returnType == "Result") || !commandData.successCodes.empty())
  {
    os << indentation << "  return createResultValue( result, ";
    if (commandData.returnParam != ~0)
    {
      os << reduceName(commandData.params[commandData.returnParam].name, singular) << ", ";
    }
    os << "\"vk::" << (commandData.className.empty() ? "" : commandData.className + "::") << reduceName(commandData.reducedName, singular) << "\"";
    if (1 < commandData.successCodes.size() && !commandData.twoStep)
    {
      os << ", { Result::" << commandData.successCodes[0];
      for (size_t i = 1; i < commandData.successCodes.size(); i++)
      {
        os << ", Result::" << commandData.successCodes[i];
      }
      os << " }";
    }
    os << " );" << std::endl;
  }
  else if ((commandData.returnParam != ~0) && (commandData.returnType != commandData.enhancedReturnType))
  {
    os << indentation << "  return " << reduceName(commandData.params[commandData.returnParam].name) << ";" << std::endl;
  }
}

void writeFunctionBodyStandard(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData)
{
  os << indentation << "  ";
  bool castReturn = false;
  if (commandData.returnType != "void")
  {
    os << "return ";
    castReturn = (vkData.vkTypes.find(commandData.returnType) != vkData.vkTypes.end());
    if (castReturn)
    {
      os << "static_cast<" << commandData.returnType << ">( ";
    }
  }

  std::string callName(dependencyData.name);
  assert(islower(callName[0]));
  callName[0] = toupper(callName[0]);

  os << "vk" << callName << "( ";
  if (!commandData.className.empty())
  {
    os << "m_" << commandData.params[0].name;
  }
  bool argEncountered = false;
  for (size_t i = commandData.className.empty() ? 0 : 1; i < commandData.params.size(); i++)
  {
    if (0 < i)
    {
      os << ", ";
    }
    writeTypeCommandParam(os, commandData.params[i], vkData.vkTypes);
  }
  os << " )";
  if (castReturn)
  {
    os << " )";
  }
  os << ";" << std::endl;
}

void writeFunctionBodyUnique(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData, bool singular)
{
  std::string type = commandData.params[commandData.returnParam].pureType;
  std::string typeValue = char(tolower(type[0])) + type.substr(1);
  os << indentation << "  " << type << "Deleter deleter( ";
  if (vkData.deleterData.find(commandData.className) != vkData.deleterData.end())
  {
    os << "*this, ";
  }
  std::map<std::string, DeleterData>::const_iterator ddit = vkData.deleterData.find(type);
  assert(ddit != vkData.deleterData.end());
  if (ddit->second.pool.empty())
  {
    os << "allocator";
  }
  else
  {
    os << reduceName(commandData.params[1].name) << "." << char(tolower(ddit->second.pool[0])) << ddit->second.pool.substr(1);
  }
  os << " );" << std::endl;
  bool returnsVector = !singular && (commandData.vectorParams.find(commandData.returnParam) != commandData.vectorParams.end());
  if (returnsVector)
  {
    os << indentation << "  std::vector<" << type << ",Allocator> " << typeValue << "s = ";
  }
  else
  {
    os << indentation << "  return Unique" << type << "( ";
  }
  os << reduceName(commandData.fullName, singular) << "( ";
  bool argEncountered = false;
  for (size_t i = commandData.className.empty() ? 0 : 1; i < commandData.params.size(); i++)
  {
    if (commandData.skippedParams.find(i) == commandData.skippedParams.end())
    {
      if (argEncountered)
      {
        os << ", ";
      }
      argEncountered = true;

      os << reduceName(commandData.params[i].name, singular && (commandData.vectorParams.find(i) != commandData.vectorParams.end()));
    }
  }
  os << " )";
  if (returnsVector)
  {
    os << ";" << std::endl
      << indentation << "  std::vector<Unique" << type << "> unique" << type << "s;" << std::endl
      << indentation << "  unique" << type << "s.reserve( " << typeValue << "s.size() );" << std::endl
      << indentation << "  for ( auto " << typeValue << " : " << typeValue << "s )" << std::endl
      << indentation << "  {" << std::endl
      << indentation << "    unique" << type << "s.push_back( Unique" << type << "( " << typeValue << ", deleter ) );" << std::endl
      << indentation << "  }" << std::endl
      << indentation << "  return unique" << type << "s;" << std::endl;
  }
  else
  {
    os << ", deleter );" << std::endl;
  }
}

void writeFunctionDeclaration(std::ofstream & ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool enhanced, bool singular, bool unique)
{
  if (enhanced)
  {
    writeFunctionHeaderTemplate(ofs, indentation, commandData, singular, true);
  }
  ofs << indentation;
  writeFunctionHeaderReturnType(ofs, indentation, commandData, enhanced, singular, unique);
  writeFunctionHeaderName(ofs, commandData.reducedName, singular, unique);
  writeFunctionHeaderArguments(ofs, vkData, commandData, enhanced, singular, true);
  ofs << ";" << std::endl;
}

void writeFunctionDefinition(std::ofstream & ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData, bool enhanced, bool singular, bool unique)
{
  if (enhanced)
  {
    writeFunctionHeaderTemplate(ofs, indentation, commandData, singular, false);
  }
  ofs << indentation << "VULKAN_HPP_INLINE ";
  writeFunctionHeaderReturnType(ofs, indentation, commandData, enhanced, singular, unique);
  if (!commandData.className.empty())
  {
    ofs << commandData.className << "::";
  }
  writeFunctionHeaderName(ofs, commandData.reducedName, singular, unique);
  writeFunctionHeaderArguments(ofs, vkData, commandData, enhanced, singular, false);
  ofs << std::endl;
  writeFunctionBody(ofs, indentation, vkData, commandData, dependencyData, enhanced, singular, unique);
}

void writeFunctionHeaderArguments(std::ostream & os, VkData const& vkData, CommandData const& commandData, bool enhanced, bool singular, bool withDefaults)
{
  os << "(";
  if (enhanced)
  {
    if (commandData.skippedParams.size() + (commandData.className.empty() ? 0 : 1) < commandData.params.size())
    {
      size_t lastArgument = ~0;
      for (size_t i = commandData.params.size() - 1; i < commandData.params.size(); i--)
      {
        if (commandData.skippedParams.find(i) == commandData.skippedParams.end())
        {
          lastArgument = i;
          break;
        }
      }

      os << " ";
      bool argEncountered = false;
      for (size_t i = commandData.className.empty() ? 0 : 1; i < commandData.params.size(); i++)
      {
        if (commandData.skippedParams.find(i) == commandData.skippedParams.end())
        {
          if (argEncountered)
          {
            os << ", ";
          }

          std::map<size_t, size_t>::const_iterator it = commandData.vectorParams.find(i);
          size_t pos = commandData.params[i].type.rfind('*');
          if (it == commandData.vectorParams.end())
          {
            if (pos == std::string::npos)
            {
              os << commandData.params[i].type << " " << reduceName(commandData.params[i].name);
              if (!commandData.params[i].arraySize.empty())
              {
                os << "[" << commandData.params[i].arraySize << "]";
              }
              if (lastArgument == i)
              {
                std::map<std::string, FlagData>::const_iterator flagIt = vkData.flags.find(commandData.params[i].pureType);
                if (flagIt != vkData.flags.end())
                {
                  std::list<DependencyData>::const_iterator depIt = std::find_if(vkData.dependencies.begin(), vkData.dependencies.end(), [&flagIt](DependencyData const& dd) { return(dd.name == flagIt->first); });
                  assert(depIt != vkData.dependencies.end());
                  assert(depIt->dependencies.size() == 1);
                  if (withDefaults)
                  {
                    std::map<std::string, EnumData>::const_iterator enumIt = vkData.enums.find(*depIt->dependencies.begin());
                    assert(enumIt != vkData.enums.end());
                    if (enumIt->second.members.empty())
                    {
                      os << " = " << commandData.params[i].pureType << "()";
                    }
                  }
                }
              }
            }
            else
            {
              assert(commandData.params[i].type[pos] == '*');
              if (commandData.params[i].optional)
              {
                os << "Optional<" << trimEnd(commandData.params[i].type.substr(0, pos)) << "> " << reduceName(commandData.params[i].name);
                if (withDefaults)
                {
                  os << " = nullptr";
                }
              }
              else if (commandData.params[i].pureType == "void")
              {
                os << commandData.params[i].type << " " << commandData.params[i].name;
              }
              else if (commandData.params[i].pureType != "char")
              {
                os << trimEnd(commandData.params[i].type.substr(0, pos)) << " & " << reduceName(commandData.params[i].name);
              }
              else
              {
                os << "const std::string & " << reduceName(commandData.params[i].name);
              }
            }
          }
          else
          {
            bool optional = commandData.params[i].optional && ((it == commandData.vectorParams.end()) || (it->second == ~0));
            assert(pos != std::string::npos);
            assert(commandData.params[i].type[pos] == '*');
            if (commandData.params[i].type.find("char") != std::string::npos)
            {
              if (optional)
              {
                os << "Optional<const std::string> " << reduceName(commandData.params[i].name);
                if (withDefaults)
                {
                  os << " = nullptr";
                }
              }
              else
              {
                os << "const std::string & " << reduceName(commandData.params[i].name);
              }
            }
            else
            {
              assert(!optional);
              if (singular)
              {
                os << trimEnd(commandData.params[i].type.substr(0, pos)) << " & " << reduceName(commandData.params[i].name, true);
              }
              else
              {
                bool isConst = (commandData.params[i].type.find("const") != std::string::npos);
                os << "ArrayProxy<" << ((commandData.templateParam == i) ? (isConst ? "const T" : "T") : trimEnd(commandData.params[i].type.substr(0, pos))) << "> " << reduceName(commandData.params[i].name);
              }
            }
          }
          argEncountered = true;
        }
      }
      os << " ";
    }
  }
  else
  {
    bool argEncountered = false;
    for (size_t i = commandData.className.empty() ? 0 : 1; i < commandData.params.size(); i++)
    {
      if (argEncountered)
      {
        os << ",";
      }

      os << " " << commandData.params[i].type << " " << commandData.params[i].name;
      if (!commandData.params[i].arraySize.empty())
      {
        os << "[" << commandData.params[i].arraySize << "]";
      }
      argEncountered = true;
    }
    if (argEncountered)
    {
      os << " ";
    }
  }
  os << ")";
  if (!commandData.className.empty())
  {
    os << " const";
  }
}

void writeFunctionHeaderName(std::ostream & os, std::string const& name, bool singular, bool unique)
{
  os << reduceName(name, singular);
  if (unique)
  {
    os << "Unique";
  }
}

void writeFunctionHeaderReturnType(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool enhanced, bool singular, bool unique)
{
  if (enhanced && !singular && (commandData.enhancedReturnType.find("Allocator") != std::string::npos))
  {
    if (!unique && (commandData.enhancedReturnType != commandData.returnType) && (commandData.returnType != "void"))
    {
      os << "typename ";
    }
  }
  if (enhanced)
  {
    if (unique)
    {
      bool returnsVector = !singular && (commandData.vectorParams.find(commandData.returnParam) != commandData.vectorParams.end());
      if (returnsVector)
      {
        os << "std::vector<";
      }
      os << "Unique" << commandData.params[commandData.returnParam].pureType;
      if (returnsVector)
      {
        os << ">";
      }
      os << " ";
    }
    else if ((commandData.enhancedReturnType != commandData.returnType) && (commandData.returnType != "void"))
    {
      assert(commandData.returnType == "Result");
      os << "ResultValueType<" << (singular ? commandData.params[commandData.returnParam].pureType : commandData.enhancedReturnType) << ">::type ";
    }
    else if ((commandData.returnParam != ~0) && (1 < commandData.successCodes.size()))
    {
      assert(commandData.returnType == "Result");
      os << "ResultValue<" << commandData.params[commandData.returnParam].pureType << "> ";
    }
    else
    {
      os << commandData.enhancedReturnType << " ";
    }
  }
  else
  {
    os << commandData.returnType << " ";
  }
}

void writeFunctionHeaderTemplate(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool singular, bool withDefault)
{
  if (!singular && (commandData.templateParam != ~0) && ((commandData.templateParam != commandData.returnParam) || (commandData.enhancedReturnType == "Result")))
  {
    assert(commandData.enhancedReturnType.find("Allocator") == std::string::npos);
    os << indentation << "template <typename T>" << std::endl;
  }
  else if (!singular && (commandData.enhancedReturnType.find("Allocator") != std::string::npos))
  {
    assert((commandData.enhancedReturnType.substr(0, 12) == "std::vector<") && (commandData.enhancedReturnType.find(',') != std::string::npos) && (12 < commandData.enhancedReturnType.find(',')));
    os << indentation << "template <typename Allocator";
    if (withDefault)
    {
      os << " = std::allocator<" << commandData.enhancedReturnType.substr(12, commandData.enhancedReturnType.find(',') - 12) << ">";
    }
    os << "> " << std::endl;
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
  if (memberData.type != "StructureType") // filter out StructureType, which is supposed to be immutable !
  {
    ofs << "    " << name << "& set" << static_cast<char>(toupper(memberData.name[0])) << memberData.name.substr(1) << "( ";
    if (memberData.arraySize.empty())
    {
      ofs << memberData.type << " ";
    }
    else
    {
      ofs << "std::array<" << memberData.type << "," << memberData.arraySize << "> ";
    }
    ofs << memberData.name << "_ )" << std::endl
      << "    {" << std::endl;
    if (!memberData.arraySize.empty())
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
}

void writeTypeCommand(std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData)
{
  assert(vkData.commands.find(dependencyData.name) != vkData.commands.end());
  CommandData const& commandData = vkData.commands.find(dependencyData.name)->second;
  if (commandData.className.empty())
  {
    if (commandData.fullName == "createInstance")
    {
      auto deleterTypesIt = vkData.deleterTypes.find("");
      assert((deleterTypesIt != vkData.deleterTypes.end()) && (deleterTypesIt->second.size() == 1));
      writeDeleterForwardDeclarations(ofs, *deleterTypesIt, vkData.deleterData);
    }
    writeTypeCommandDeclaration(ofs, "  ", vkData, commandData);
    if (commandData.fullName == "createInstance")
    {
      auto deleterTypesIt = vkData.deleterTypes.find("");
      assert((deleterTypesIt != vkData.deleterTypes.end()) && (deleterTypesIt->second.size() == 1));
      writeDeleterClasses(ofs, *deleterTypesIt, vkData.deleterData);
    }
    writeTypeCommandDefinition(ofs, "  ", vkData, commandData, dependencyData);
    ofs << std::endl;
  }
}

void writeTypeCommandDeclaration(std::ofstream & ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData)
{
  enterProtect(ofs, commandData.protect);
  bool unchangedInterface = !hasPointerParam(commandData.params);
  if ((commandData.enhancedReturnType == commandData.returnType) && commandData.vectorParams.empty() && unchangedInterface && (commandData.successCodes.size() <= 1))
  {
    writeTypeCommandDeclarationStandard(ofs, "    ", vkData, commandData);
  }
  else
  {
    if (unchangedInterface)
    {
      ofs << "#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE" << std::endl;
    }
    writeTypeCommandDeclarationStandard(ofs, "    ", vkData, commandData);
    ofs << (unchangedInterface ? "#else" : "#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE") << std::endl;
    writeTypeCommandDeclarationEnhanced(ofs, "    ", vkData, commandData);
    ofs << "#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/" << std::endl;
  }
  leaveProtect(ofs, commandData.protect);
  ofs << std::endl;
}

void writeTypeCommandDeclarationEnhanced(std::ofstream & ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData)
{
  writeFunctionDeclaration(ofs, indentation, vkData, commandData, true, false, false);

  // determine candidates for singular version of function
  std::map<size_t, size_t>::const_iterator returnVector = commandData.vectorParams.find(commandData.returnParam);
  bool singular = (returnVector != commandData.vectorParams.end()) && (returnVector->second != ~0) && (commandData.params[returnVector->second].type.back() != '*');
  if (singular)
  {
    writeFunctionDeclaration(ofs, indentation, vkData, commandData, true, true, false);
  }

  // special handling for createDevice and createInstance !
  bool specialWriteUnique = (commandData.reducedName == "createDevice") || (commandData.reducedName == "createInstance");

  if (((vkData.deleterData.find(commandData.className) != vkData.deleterData.end()) || specialWriteUnique) && ((commandData.reducedName.substr(0, 8) == "allocate") || (commandData.reducedName.substr(0, 6) == "create")))
  {
    ofs << "#ifndef VULKAN_HPP_NO_SMART_HANDLE" << std::endl;
    writeFunctionDeclaration(ofs, indentation, vkData, commandData, true, false, true);

    if (singular)
    {
      writeFunctionDeclaration(ofs, indentation, vkData, commandData, true, true, true);
    }
    ofs << "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/" << std::endl;
  }
}

void writeTypeCommandDeclarationStandard(std::ofstream & ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData)
{
  writeFunctionDeclaration(ofs, indentation, vkData, commandData, false, false, false);
}

void writeTypeCommandDefinition(std::ofstream &ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData)
{
  enterProtect(ofs, commandData.protect);
  bool unchangedInterface = !hasPointerParam(commandData.params);
  if ((commandData.enhancedReturnType == commandData.returnType) && commandData.vectorParams.empty() && unchangedInterface && (commandData.successCodes.size() <= 1))
  {
    writeTypeCommandDefinitionStandard(ofs, indentation, vkData, commandData, dependencyData);
  }
  else
  {
    if (unchangedInterface)
    {
      ofs << "#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE" << std::endl;
    }
    writeTypeCommandDefinitionStandard(ofs, indentation, vkData, commandData, dependencyData);
    ofs << (unchangedInterface ? "#else" : "#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE") << std::endl;
    writeTypeCommandDefinitionEnhanced(ofs, indentation, vkData, commandData, dependencyData);
    ofs << "#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/" << std::endl;
  }
  leaveProtect(ofs, commandData.protect);
}

void writeTypeCommandDefinitionEnhanced(std::ofstream &ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData)
{
  writeFunctionDefinition(ofs, indentation, vkData, commandData, dependencyData, true, false, false);

  // determine candidates for singular version of function
  std::map<size_t, size_t>::const_iterator returnVector = commandData.vectorParams.find(commandData.returnParam);
  bool singular = (returnVector != commandData.vectorParams.end()) && (returnVector->second != ~0) && (commandData.params[returnVector->second].type.back() != '*');
  if (singular)
  {
    writeFunctionDefinition(ofs, indentation, vkData, commandData, dependencyData, true, true, false);
  }

  // special handling for createDevice and createInstance !
  bool specialWriteUnique = (commandData.reducedName == "createDevice") || (commandData.reducedName == "createInstance");

  if (((vkData.deleterData.find(commandData.className) != vkData.deleterData.end()) || specialWriteUnique) && ((commandData.reducedName.substr(0, 8) == "allocate") || (commandData.reducedName.substr(0, 6) == "create")))
  {
    ofs << "#ifndef VULKAN_HPP_NO_SMART_HANDLE" << std::endl;
    writeFunctionDefinition(ofs, indentation, vkData, commandData, dependencyData, true, false, true);

    if (singular)
    {
      writeFunctionDefinition(ofs, indentation, vkData, commandData, dependencyData, true, true, true);
    }
    ofs << "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/" << std::endl;
  }
}

void writeTypeCommandDefinitionStandard(std::ofstream &ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData, DependencyData const& dependencyData)
{
  writeFunctionDefinition(ofs, indentation, vkData, commandData, dependencyData, false, false, false);
}

void writeTypeCommandParam(std::ostream & os, ParamData const& param, std::set<std::string> const& vkTypes)
{
  if (vkTypes.find(param.pureType) != vkTypes.end())
  {
    if (param.type.back() == '*')
    {
      os << "reinterpret_cast<";
      if (param.type.find("const") == 0)
      {
        os << "const ";
      }
      os << "Vk" << param.pureType;
      if (param.type.find("* const") != std::string::npos)
      {
        os << "* const";
      }
      os << '*';
    }
    else
    {
      os << "static_cast<Vk" << param.pureType;
    }
    os << ">( " << param.name << " )";
  }
  else
  {
    os << param.name;
  }
}

void writeTypeEnum( std::ofstream & ofs, EnumData const& enumData )
{
  enterProtect(ofs, enumData.protect);
  ofs << "  enum class " << enumData.name << std::endl
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

void writeDeleterClasses(std::ofstream & ofs, std::pair<std::string, std::set<std::string>> const& deleterTypes, std::map<std::string, DeleterData> const& deleterData)
{
  ofs << "#ifndef VULKAN_HPP_NO_SMART_HANDLE" << std::endl;
  bool first = true;
  std::string firstName = deleterTypes.first.empty() ? "" : (char(tolower(deleterTypes.first[0])) + deleterTypes.first.substr(1));
  for (auto const& dt : deleterTypes.second)
  {
    std::string value = char(tolower(dt[0])) + dt.substr(1);

    if (!first)
    {
      ofs << std::endl;
    }
    first = false;

    ofs << "  class " << dt << "Deleter" << std::endl
      << "  {" << std::endl
      << "  public:" << std::endl
      << "    " << dt << "Deleter( ";
    if (!deleterTypes.first.empty() && (dt != "Device"))
    {
      ofs << deleterTypes.first << " " << firstName << " = " << deleterTypes.first << "(), ";
    }
    auto const& dd = deleterData.find(dt);
    assert(dd != deleterData.end());
    std::string poolName = (dd->second.pool.empty() ? "" : (char(tolower(dd->second.pool[0])) + dd->second.pool.substr(1)));
    if (poolName.empty())
    {
      ofs << "Optional<const AllocationCallbacks> allocator = nullptr )" << std::endl;
    }
    else
    {
      assert(!dd->second.pool.empty());
      poolName = char(tolower(dd->second.pool[0])) + dd->second.pool.substr(1);
      ofs << dd->second.pool << " " << poolName << " = " << dd->second.pool << "() )" << std::endl;
    }
    ofs << "      : ";
    if (!deleterTypes.first.empty() && (dt != "Device"))
    {
      ofs << "m_" << firstName << "( " << firstName << " )" << std::endl
        << "      , ";
    }
    if (poolName.empty())
    {
      ofs << "m_allocator( allocator )" << std::endl;
    }
    else
    {
      ofs << "m_" << poolName << "( " << poolName << " )" << std::endl;
    }
    ofs << "    {}" << std::endl
      << std::endl
      << "    void operator()( " << dt << " " << char(tolower(dt[0])) + dt.substr(1) << " )" << std::endl
      << "    {" << std::endl;
    if (dt == "Device")
    {
      ofs << "      device.";
    }
    else if (deleterTypes.first.empty())
    {
      ofs << "      " << value << ".";
    }
    else
    {
      ofs << "      m_" << char(tolower(deleterTypes.first[0])) << deleterTypes.first.substr(1) << ".";
    }
    ofs << dd->second.call << "( ";
    if (!dd->second.pool.empty())
    {
      ofs << "m_" << char(tolower(dd->second.pool[0])) << dd->second.pool.substr(1) << ", ";
    }
    if (!deleterTypes.first.empty() && (dt != "Device"))
    {
      ofs << value;
    }
    if (dd->second.pool.empty())
    {
      if (!deleterTypes.first.empty() && (dt != "Device"))
      {
        ofs << ", ";
      }
      ofs << "m_allocator";
    }
    ofs << " );" << std::endl
      << "    }" << std::endl
      << std::endl
      << "  private:" << std::endl;
    if (!deleterTypes.first.empty() && (dt != "Device"))
    {
      ofs << "    " << deleterTypes.first << " m_" << firstName << ";" << std::endl;
    }
    if (poolName.empty())
    {
      ofs << "    Optional<const AllocationCallbacks> m_allocator;" << std::endl;
    }
    else
    {
      ofs << "    " << dd->second.pool << " m_" << poolName << ";" << std::endl;
    }
    ofs << "  };" << std::endl;
  }
  ofs << "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/" << std::endl
    << std::endl;
}

void writeDeleterForwardDeclarations(std::ofstream &ofs, std::pair<std::string, std::set<std::string>> const& deleterTypes, std::map<std::string, DeleterData> const& deleterData)
{
  ofs << "#ifndef VULKAN_HPP_NO_SMART_HANDLE" << std::endl;
  bool first = true;
  std::string firstName = deleterTypes.first.empty() ? "" : (char(tolower(deleterTypes.first[0])) + deleterTypes.first.substr(1));
  for (auto const& dt : deleterTypes.second)
  {
    ofs << "  class " << dt << "Deleter;" << std::endl;
    ofs << "  using Unique" << dt << " = UniqueHandle<" << dt << ", " << dt << "Deleter>;" << std::endl;
  }
  ofs << "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/" << std::endl
    << std::endl;
}

void writeEnumsToString(std::ofstream & ofs, EnumData const& enumData)
{
  enterProtect(ofs, enumData.protect);
  ofs << "  VULKAN_HPP_INLINE std::string to_string(" << enumData.name << (enumData.members.empty() ? ")" : " value)") << std::endl
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
      ofs << "    case " << enumData.name << "::" << itMember->name << ": return \"" << itMember->name.substr(1) << "\";" << std::endl;
    }
    ofs << "    default: return \"invalid\";" << std::endl
        << "    }" << std::endl;
  }
  ofs << "  }" << std::endl;
  leaveProtect(ofs, enumData.protect);
  ofs << std::endl;
}

void writeFlagsToString(std::ofstream & ofs, std::string const& flagsName, EnumData const &enumData)
{
  enterProtect(ofs, enumData.protect);
  ofs << "  VULKAN_HPP_INLINE std::string to_string(" << flagsName << (enumData.members.empty() ? ")" : " value)") << std::endl
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
      ofs << "    if (value & " << enumData.name << "::" << itMember->name << ") result += \"" << itMember->name.substr(1) << " | \";" << std::endl;
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
      writeEnumsToString(ofs, vkData.enums.find(it->name)->second);
      break;
    case DependencyData::Category::FLAGS:
      writeFlagsToString(ofs, it->name, vkData.enums.find(*it->dependencies.begin())->second);
      break;
    }
  }
}

void writeTypeFlags(std::ofstream & ofs, std::string const& flagsName, FlagData const& flagData, EnumData const& enumData)
{
  enterProtect(ofs, flagData.protect);
  ofs << "  using " << flagsName << " = Flags<" << enumData.name << ", Vk" << flagsName << ">;" << std::endl
      << std::endl
      << "  VULKAN_HPP_INLINE " << flagsName << " operator|( " << enumData.name << " bit0, " << enumData.name << " bit1 )" << std::endl
      << "  {" << std::endl
      << "    return " << flagsName << "( bit0 ) | bit1;" << std::endl
      << "  }" << std::endl;
  if (!enumData.members.empty())
  {
    ofs << std::endl
        << "  VULKAN_HPP_INLINE " << flagsName << " operator~( " << enumData.name << " bits )" << std::endl
        << "  {" << std::endl
        << "    return ~( " << flagsName << "( bits ) );" << std::endl
        << "  }" << std::endl
        << std::endl
        << "  template <> struct FlagTraits<" << enumData.name << ">" << std::endl
        << "  {" << std::endl
        << "    enum" << std::endl
        << "    {" << std::endl
        << "      allFlags = ";
    for (size_t i = 0; i < enumData.members.size(); i++)
    {
      if (i != 0)
      {
        ofs << " | ";
      }
      ofs << "VkFlags(" << enumData.name << "::" << enumData.members[i].name << ")";
    }
    ofs << std::endl
        << "    };" << std::endl
        << "  };" << std::endl;
  }
  leaveProtect(ofs, flagData.protect);
  ofs << std::endl;
}

void writeTypeHandle(std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, HandleData const& handleData, std::list<DependencyData> const& dependencies)
{
  std::string memberName = dependencyData.name;
  assert(isupper(memberName[0]));
  memberName[0] = tolower(memberName[0]);

  enterProtect(ofs, handleData.protect);

  if (!dependencyData.forwardDependencies.empty())
  {
    ofs << "  // forward declarations" << std::endl;
    for (std::set<std::string>::const_iterator it = dependencyData.forwardDependencies.begin(); it != dependencyData.forwardDependencies.end(); ++it)
    {
      assert(vkData.structs.find(*it) != vkData.structs.end());
      ofs << "  struct " << *it << ";" << std::endl;
    }
    ofs << std::endl;
  }

  std::map<std::string, std::set<std::string>>::const_iterator deleterTypesIt = vkData.deleterTypes.find(dependencyData.name);
  if (deleterTypesIt != vkData.deleterTypes.end())
  {
    writeDeleterForwardDeclarations(ofs, *deleterTypesIt, vkData.deleterData);
  }

  ofs << "  class " << dependencyData.name << std::endl
      << "  {" << std::endl
      << "  public:" << std::endl
      << "    " << dependencyData.name << "()" << std::endl
      << "      : m_" << memberName << "(VK_NULL_HANDLE)" << std::endl
      << "    {}" << std::endl
      << std::endl
      << "    " << dependencyData.name << "( nullptr_t )" << std::endl
      << "      : m_" << memberName << "(VK_NULL_HANDLE)" << std::endl
      << "    {}" << std::endl
      << std::endl
      // construct from native handle
      << "    VULKAN_HPP_TYPESAFE_EXPLICIT " << dependencyData.name << "(Vk" << dependencyData.name << " " << memberName << ")" << std::endl
      << "       : m_" << memberName << "("  << memberName << ")" << std::endl
      << "    {}" << std::endl
      << std::endl
      // assignment from native handle
      << "#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)" << std::endl
      << "    " << dependencyData.name << "& operator=(Vk" << dependencyData.name << " " << memberName << ")" << std::endl
      << "    {" << std::endl
      << "      m_" << memberName << " = " << memberName << ";" << std::endl
      << "      return *this;" << std::endl
      << "    }" << std::endl
      << "#endif\n"
      << std::endl
      // assignment from nullptr_t
      << "    " << dependencyData.name << "& operator=( nullptr_t )" << std::endl
      << "    {" << std::endl
      << "      m_" << memberName << " = VK_NULL_HANDLE;" << std::endl
      << "      return *this;" << std::endl
      << "    }" << std::endl
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

  for (size_t i = 0; i < handleData.commands.size(); i++)
  {
    std::string commandName = handleData.commands[i];
    std::map<std::string, CommandData>::const_iterator cit = vkData.commands.find(commandName);
    assert((cit != vkData.commands.end()) && !cit->second.className.empty());

    writeTypeCommandDeclaration(ofs, "    ", vkData, cit->second);
  }

  ofs << "    VULKAN_HPP_TYPESAFE_EXPLICIT operator Vk" << dependencyData.name << "() const" << std::endl
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

  deleterTypesIt = vkData.deleterTypes.find(dependencyData.name);
  if (deleterTypesIt != vkData.deleterTypes.end())
  {
    writeDeleterClasses(ofs, *deleterTypesIt, vkData.deleterData);
  }

  for (size_t i = 0; i < handleData.commands.size(); i++)
  {
    std::string commandName = handleData.commands[i];
    std::map<std::string, CommandData>::const_iterator cit = vkData.commands.find(commandName);
    assert((cit != vkData.commands.end()) && !cit->second.className.empty());
    std::list<DependencyData>::const_iterator dep = std::find_if(dependencies.begin(), dependencies.end(), [commandName](DependencyData const& dd) { return dd.name == commandName; });
    assert(dep != dependencies.end() && (dep->name == cit->second.fullName));

    writeTypeCommandDefinition(ofs, "  ", vkData, cit->second, *dep);

    if (i < handleData.commands.size() - 1)
    {
      ofs << std::endl;
    }
  }

  leaveProtect(ofs, handleData.protect);
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
        writeTypeEnum( ofs, vkData.enums.find( it->name )->second );
        break;
      case DependencyData::Category::FLAGS :
        assert(vkData.flags.find(it->name) != vkData.flags.end());
        writeTypeFlags( ofs, it->name, vkData.flags.find( it->name)->second, vkData.enums.find(generateEnumNameForFlags(it->name))->second );
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
      << "# if !defined( VULKAN_HPP_TYPESAFE_CONVERSION )" << std::endl
      << "#  define VULKAN_HPP_TYPESAFE_CONVERSION" << std::endl
      << "# endif" << std::endl
      << "#endif" << std::endl;
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
    vkData.handles[""];   // insert the default "handle" without class (for createInstance, and such)

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
        vkData.vulkanLicenseHeader = trimStart(vkData.vulkanLicenseHeader);
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
      << explicitHeader
      << std::endl
      << "namespace vk" << std::endl
      << "{" << std::endl
      << flagsHeader
      << optionalClassHeader
      << arrayProxyHeader
      << uniqueHandleHeader;

    // first of all, write out vk::Result and the exception handling stuff
    std::list<DependencyData>::const_iterator it = std::find_if(vkData.dependencies.begin(), vkData.dependencies.end(), [](DependencyData const& dp) { return dp.name == "Result"; });
    assert(it != vkData.dependencies.end());
    writeTypeEnum(ofs, vkData.enums.find(it->name)->second);
    writeEnumsToString(ofs, vkData.enums.find(it->name)->second);
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

    assert(vkData.deleterTypes.find("") != vkData.deleterTypes.end());
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
