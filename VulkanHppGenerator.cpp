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

  void addEnumMember(std::string const& name, std::string const& tag);

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
void leaveProtect(std::ostream &os, std::string const& protect);
void linkCommandToHandle(VkData & vkData, CommandData & commandData);
std::string readArraySize(tinyxml2::XMLNode * node, std::string& name);
bool readCommandParam( tinyxml2::XMLElement * element, std::set<std::string> & dependencies, std::vector<ParamData> & params );
void readCommandParams(tinyxml2::XMLElement* element, std::set<std::string> & dependencies, CommandData & commandData);
tinyxml2::XMLNode* readCommandParamType(tinyxml2::XMLNode* node, ParamData& param);
CommandData& readCommandProto(tinyxml2::XMLElement * element, VkData & vkData);
void readCommands( tinyxml2::XMLElement * element, VkData & vkData );
void readCommandsCommand(tinyxml2::XMLElement * element, VkData & vkData);
std::vector<std::string> readCommandSuccessCodes(tinyxml2::XMLElement* element, std::set<std::string> const& tags);
void readComment(tinyxml2::XMLElement * element, std::string & header);
void readEnums( tinyxml2::XMLElement * element, VkData & vkData );
void readEnumsEnum( tinyxml2::XMLElement * element, EnumData & enumData );
void readDisabledExtensionRequire(tinyxml2::XMLElement * element, VkData & vkData);
void readExtensionCommand(tinyxml2::XMLElement * element, std::map<std::string, CommandData> & commands, std::string const& protect);
void readExtensionEnum(tinyxml2::XMLElement * element, std::map<std::string, EnumData> & enums, std::string const& tag);
void readExtensionRequire(tinyxml2::XMLElement * element, VkData & vkData, std::string const& protect, std::string const& tag);
void readExtensions( tinyxml2::XMLElement * element, VkData & vkData );
void readExtensionsExtension(tinyxml2::XMLElement * element, VkData & vkData);
void readExtensionType(tinyxml2::XMLElement * element, VkData & vkData, std::string const& protect);
tinyxml2::XMLNode* readType(tinyxml2::XMLNode* element, std::string & type, std::string & pureType);
void readTypeBasetype( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies );
void readTypeBitmask( tinyxml2::XMLElement * element, VkData & vkData);
void readTypeDefine( tinyxml2::XMLElement * element, VkData & vkData );
void readTypeFuncpointer( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies );
void readTypeHandle(tinyxml2::XMLElement * element, VkData & vkData);
void readTypeStruct( tinyxml2::XMLElement * element, VkData & vkData, bool isUnion );
void readTypeStructMember( tinyxml2::XMLElement * element, std::vector<MemberData> & members, std::set<std::string> & dependencies );
void readTags(tinyxml2::XMLElement * element, std::set<std::string> & tags);
void readTypes(tinyxml2::XMLElement * element, VkData & vkData);
std::string reduceName(std::string const& name, bool singular = false);
void registerDeleter(VkData & vkData, CommandData const& commandData);
std::string startLowerCase(std::string const& input);
std::string startUpperCase(std::string const& input);
void sortDependencies( std::list<DependencyData> & dependencies );
std::string strip(std::string const& value, std::string const& prefix, std::string const& postfix = std::string());
std::string stripPluralS(std::string const& name);
std::string toCamelCase(std::string const& value);
std::string toUpperCase(std::string const& name);
std::string trimEnd(std::string const& input);
void writeCall(std::ostream & os, CommandData const& commandData, std::set<std::string> const& vkTypes, bool firstCall, bool singular);
void writeCallCountParameter(std::ostream & os, CommandData const& commandData, bool singular, std::map<size_t, size_t>::const_iterator it);
void writeCallParameter(std::ostream & os, ParamData const& paramData, std::set<std::string> const& vkTypes);
void writeCallPlainTypeParameter(std::ostream & os, ParamData const& paramData);
void writeCallVectorParameter(std::ostream & os, CommandData const& commandData, std::set<std::string> const& vkTypes, bool firstCall, bool singular, std::map<size_t, size_t>::const_iterator it);
void writeCallVulkanTypeParameter(std::ostream & os, ParamData const& paramData);
void writeDeleterClasses(std::ofstream & ofs, std::pair<std::string, std::set<std::string>> const& deleterTypes, std::map<std::string, DeleterData> const& deleterData);
void writeDeleterForwardDeclarations(std::ofstream &ofs, std::pair<std::string, std::set<std::string>> const& deleterTypes, std::map<std::string, DeleterData> const& deleterData);
void writeEnumsToString(std::ofstream & ofs, EnumData const& enumData);
void writeFlagsToString(std::ofstream & ofs, std::string const& flagsName, EnumData const &enumData);
void writeFunction(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool definition, bool enhanced, bool singular, bool unique);
void writeFunctionBodyEnhanced(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool singular);
void writeFunctionBodyEnhancedCall(std::ostream &os, std::string const& indentation, std::set<std::string> const& vkTypes, CommandData const& commandData, bool singular);
void writeFunctionBodyEnhancedCallResult(std::ostream &os, std::string const& indentation, std::set<std::string> const& vkTypes, CommandData const& commandData, bool singular);
void writeFunctionBodyEnhancedCallTwoStep(std::ostream & os, std::string const& indentation, std::set<std::string> const& vkTypes, std::string const& returnName, std::string const& sizeName, CommandData const& commandData);
void writeFunctionBodyEnhancedCallTwoStepChecked(std::ostream & os, std::string const& indentation, std::set<std::string> const& vkTypes, std::string const& returnName, std::string const& sizeName, CommandData const& commandData);
void writeFunctionBodyEnhancedCallTwoStepIterate(std::ostream & os, std::string const& indentation, std::set<std::string> const& vkTypes, std::string const& returnName, std::string const& sizeName, CommandData const& commandData);
void writeFunctionBodyEnhancedLocalCountVariable(std::ostream & os, std::string const& indentation, CommandData const& commandData);
std::string writeFunctionBodyEnhancedLocalReturnVariable(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool singular);
void writeFunctionBodyEnhancedMultiVectorSizeCheck(std::ostream & os, std::string const& indentation, CommandData const& commandData);
void writeFunctionBodyEnhancedReturnResultValue(std::ostream & os, std::string const& indentation, std::string const& returnName, CommandData const& commandData, bool singular);
void writeFunctionBodyStandard(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData);
void writeFunctionBodyUnique(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool singular);
void writeFunctionHeaderArguments(std::ostream & os, VkData const& vkData, CommandData const& commandData, bool enhanced, bool singular, bool withDefaults);
void writeFunctionHeaderArgumentsEnhanced(std::ostream & os, VkData const& vkData, CommandData const& commandData, bool singular, bool withDefaults);
void writeFunctionHeaderArgumentsStandard(std::ostream & os, CommandData const& commandData);
void writeFunctionHeaderName(std::ostream & os, std::string const& name, bool singular, bool unique);
void writeFunctionHeaderReturnType(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool enhanced, bool singular, bool unique);
void writeFunctionHeaderTemplate(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool withDefault);
void writeReinterpretCast(std::ostream & os, bool leadingConst, bool vulkanType, std::string const& type, bool trailingPointerToConst);
void writeStandardOrEnhanced(std::ofstream & ofs, std::string const& standard, std::string const& enhanced);
void writeStructConstructor( std::ofstream & ofs, std::string const& name, StructData const& structData, std::set<std::string> const& vkTypes, std::map<std::string,std::string> const& defaultValues );
void writeStructSetter( std::ofstream & ofs, std::string const& structureName, MemberData const& memberData, std::set<std::string> const& vkTypes, std::map<std::string,StructData> const& structs );
void writeTypeCommand(std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData);
void writeTypeCommand(std::ofstream &ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool definition);
void writeTypeEnum(std::ofstream & ofs, EnumData const& enumData);
void writeTypeFlags(std::ofstream & ofs, std::string const& flagsName, FlagData const& flagData, EnumData const& enumData);
void writeTypeHandle(std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, HandleData const& handle, std::list<DependencyData> const& dependencies);
void writeTypeScalar( std::ofstream & ofs, DependencyData const& dependencyData );
void writeTypeStruct( std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, std::map<std::string,std::string> const& defaultValues );
void writeTypeUnion( std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, std::map<std::string,std::string> const& defaultValues );
void writeTypes(std::ofstream & ofs, VkData const& vkData, std::map<std::string, std::string> const& defaultValues);
void writeVersionCheck(std::ofstream & ofs, std::string const& version);
void writeTypesafeCheck(std::ofstream & ofs, std::string const& typesafeCheck);

void EnumData::addEnumMember(std::string const &name, std::string const& tag)
{
  NameValue nv;
  nv.name = "e" + toCamelCase(strip(name, prefix, postfix));
  nv.value = name;
  if (bitmask)
  {
    size_t pos = nv.name.find("Bit");
    if (pos != std::string::npos)
    {
      nv.name.erase(pos, 3);
    }
  }
  if (!tag.empty() && (nv.name.substr(nv.name.length() - tag.length()) == toCamelCase(tag)))
  {
    nv.name = nv.name.substr(0, nv.name.length() - tag.length()) + tag;
  }
  members.push_back(nv);
}

void createDefaults( VkData const& vkData, std::map<std::string,std::string> & defaultValues )
{
  for (auto dependency : vkData.dependencies)
  {
    assert( defaultValues.find( dependency.name ) == defaultValues.end() );
    switch( dependency.category )
    {
      case DependencyData::Category::COMMAND :    // commands should never be asked for defaults
        break;
      case DependencyData::Category::ENUM :
        {
          assert(vkData.enums.find(dependency.name) != vkData.enums.end());
          EnumData const & enumData = vkData.enums.find(dependency.name)->second;
          if (!enumData.members.empty())
          {
            defaultValues[dependency.name] = dependency.name + "::" + vkData.enums.find(dependency.name)->second.members.front().name;
          }
          else
          {
            defaultValues[dependency.name] = dependency.name + "()";
          }
        }
        break;
      case DependencyData::Category::FLAGS :
      case DependencyData::Category::HANDLE:
      case DependencyData::Category::STRUCT:
      case DependencyData::Category::UNION :        // just call the default constructor for flags, structs, and structs (which are mapped to classes)
        defaultValues[dependency.name] = dependency.name + "()";
        break;
      case DependencyData::Category::FUNC_POINTER : // func_pointers default to nullptr
        defaultValues[dependency.name] = "nullptr";
        break;
      case DependencyData::Category::REQUIRED :     // all required default to "0"
      case DependencyData::Category::SCALAR :       // all scalars default to "0"
        defaultValues[dependency.name] = "0";
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
  // if there is a return parameter of type void or Result, and if it's of type Result it either has just one success code
  // or two success codes, where the second one is of type eIncomplete and it's a two-step process
  // -> we can return that parameter
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
      // the return parameter is a vector-type parameter
      if (commandData.params[commandData.returnParam].pureType == "void")
      {
        // for a vector of void, we use a vector of uint8_t, instead
        commandData.enhancedReturnType = "std::vector<uint8_t,Allocator>";
      }
      else
      {
        // for the other parameters, we use a vector of the pure type
        commandData.enhancedReturnType = "std::vector<" + commandData.params[commandData.returnParam].pureType + ",Allocator>";
      }
    }
    else
    {
      // it's a simple parameter -> get the type and just remove the trailing '*' (originally, it's a pointer)
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
  // for return types of type Result or void, we can replace determine a parameter to return
  if ((commandData.returnType == "Result") || (commandData.returnType == "void"))
  {
    for (size_t i = 0; i < commandData.params.size(); i++)
    {
      if ((commandData.params[i].type.find('*') != std::string::npos)
        && (commandData.params[i].type.find("const") == std::string::npos)
        && std::find_if(commandData.vectorParams.begin(), commandData.vectorParams.end(), [i](std::pair<size_t, size_t> const& vp) { return vp.second == i; }) == commandData.vectorParams.end()
        && ((commandData.vectorParams.find(i) == commandData.vectorParams.end()) || commandData.twoStep || (commandData.successCodes.size() == 1)))
      {
        // it's a non-const pointer, not a vector-size parameter, if it's a vector parameter, its a two-step process or there's just one success code
        // -> look for another non-cost pointer argument
        auto paramIt = std::find_if(commandData.params.begin() + i + 1, commandData.params.end(), [](ParamData const& pd)
        {
          return (pd.type.find('*') != std::string::npos) && (pd.type.find("const") == std::string::npos);
        });
        // if there is another such argument, we can't decide which one to return -> return none (~0)
        // otherwise return the index of the selcted parameter
        commandData.returnParam = paramIt != commandData.params.end() ? ~0 : i;
      }
    }
  }
}

void determineSkippedParams(CommandData & commandData)
{
  // the size-parameters of vector parameters are not explicitly used in the enhanced API
  std::for_each(commandData.vectorParams.begin(), commandData.vectorParams.end(), [&commandData](std::pair<size_t, size_t> const& vp) { if (vp.second != ~0) commandData.skippedParams.insert(vp.second); });
  // and the return parameter is also skipped
  if (commandData.returnParam != ~0)
  {
    commandData.skippedParams.insert(commandData.returnParam);
  }
}

void determineTemplateParam(CommandData & commandData)
{
  for (size_t i = 0; i < commandData.params.size(); i++)
  {
    // any vector parameter on the pure type void is templatized in the enhanced API
    if ((commandData.vectorParams.find(i) != commandData.vectorParams.end()) && (commandData.params[i].pureType == "void"))
    {
#if !defined(NDEBUG)
      for (size_t j = i + 1; j < commandData.params.size(); j++)
      {
        assert((commandData.vectorParams.find(j) == commandData.vectorParams.end()) || (commandData.params[j].pureType != "void"));
      }
#endif
      commandData.templateParam = i;
      break;
    }
  }
  assert((commandData.templateParam == ~0) || (commandData.vectorParams.find(commandData.templateParam) != commandData.vectorParams.end()));
}

void determineVectorParams(CommandData & commandData)
{
  // look for the parameters whose len equals the name of an other parameter
  for (auto it = commandData.params.begin(), begin = it, end = commandData.params.end(); it != end; ++it)
  {
    if (!it->len.empty())
    {
      auto findLambda = [it](ParamData const& pd) { return pd.name == it->len; };
      auto findIt = std::find_if(begin, it, findLambda);                        // look for a parameter named as the len of this parameter
      assert((std::count_if(begin, end, findLambda) == 0) || (findIt < it));    // make sure, there is no other parameter like that
      // add this parameter as a vector parameter, using the len-name parameter as the second value (or ~0 if there is nothing like that)
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
  assert((start != std::string::npos) && (name.substr(0, start) == "VK"));
  size_t end = name.find('_', start + 1);
  assert(end != std::string::npos);
  return name.substr(start + 1, end - start - 1);
}

std::string findTag(std::string const& name, std::set<std::string> const& tags)
{
  // find the tag in a name, return that tag or an empty string
  auto tagIt = std::find_if(tags.begin(), tags.end(), [&name](std::string const& t)
  {
    size_t pos = name.find(t);
    return (pos != std::string::npos) && (pos == name.length() - t.length());
  });
  return tagIt != tags.end() ? *tagIt : "";
}

std::string generateEnumNameForFlags(std::string const& name)
{
  // create a string, where the substring "Flags" is replaced by "FlagBits"
  std::string generatedName = name;
  size_t pos = generatedName.rfind("Flags");
  assert(pos != std::string::npos);
  generatedName.replace(pos, 5, "FlagBits");
  return generatedName;
}

bool hasPointerParam(std::vector<ParamData> const& params)
{
  // check if any of the parameters is a pointer
  auto it = std::find_if(params.begin(), params.end(), [](ParamData const& pd)
  {
    return (pd.type.find('*') != std::string::npos);
  });
  return it != params.end();
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
  // first, find the handle named like the type of the first argument
  // if there is no such handle, look for the unnamed "handle", that gathers all the functions not tied to a specific handle
  assert(!commandData.params.empty());
  std::map<std::string, HandleData>::iterator hit = vkData.handles.find(commandData.params[0].pureType);
  if (hit == vkData.handles.end())
  {
    hit = vkData.handles.find("");
  }
  assert(hit != vkData.handles.end());

  // put the command into the handle's list of commands, and store the handle in the commands className
  hit->second.commands.push_back(commandData.fullName);
  commandData.className = hit->first;

  // add the dependencies of the command to the dependencies of the handle
  DependencyData const& commandDD = vkData.dependencies.back();
  std::list<DependencyData>::iterator handleDD = std::find_if(vkData.dependencies.begin(), vkData.dependencies.end(), [hit](DependencyData const& dd) { return dd.name == hit->first; });
  assert((handleDD != vkData.dependencies.end()) || hit->first.empty());
  if (handleDD != vkData.dependencies.end())
  {
    std::copy_if(commandDD.dependencies.begin(), commandDD.dependencies.end(), std::inserter(handleDD->dependencies, handleDD->dependencies.end()), [hit](std::string const& d) { return d != hit->first; });
  }
}

std::string readArraySize(tinyxml2::XMLNode * node, std::string& name)
{
  std::string arraySize;
  if (name.back() == ']')
  {
    // if the parameter has '[' and ']' in its name, get the stuff inbetween those as the array size and erase that part from the parameter name
    assert(!node->NextSibling());
    size_t pos = name.find('[');
    assert(pos != std::string::npos);
    arraySize = name.substr(pos + 1, name.length() - 2 - pos);
    name.erase(pos);
  }
  else
  {
    // otherwise look for a sibling of this node
    node = node->NextSibling();
    if (node && node->ToText())
    {
      std::string value = node->Value();
      if (value == "[")
      {
        // if this node has '[' as its value, the next node holds the array size, and the node after that needs to hold ']', and there should be no more siblings
        node = node->NextSibling();
        assert(node && node->ToElement() && (strcmp(node->Value(), "enum") == 0));
        arraySize = node->ToElement()->GetText();
        node = node->NextSibling();
        assert(node && node->ToText() && (strcmp(node->Value(), "]") == 0) && !node->NextSibling());
      }
      else
      {
        // otherwise, the node holds '[' and ']', so get the stuff inbetween those as the array size
        assert((value.front() == '[') && (value.back() == ']'));
        arraySize = value.substr(1, value.length() - 2);
        assert(!node->NextSibling());
      }
    }
  }
  return arraySize;
}

bool readCommandParam( tinyxml2::XMLElement * element, std::set<std::string> & dependencies, std::vector<ParamData> & params )
{
  ParamData param;

  if (element->Attribute("len"))
  {
    param.len = element->Attribute("len");
  }

  // get the type of the parameter, and put it into the list of dependencies
  tinyxml2::XMLNode * child = readCommandParamType(element->FirstChild(), param);
  dependencies.insert(param.pureType);

  assert( child->ToElement() && ( strcmp( child->Value(), "name" ) == 0 ) );
  param.name = child->ToElement()->GetText();

  param.arraySize = readArraySize(child, param.name);

  param.optional = element->Attribute("optional") && (strcmp(element->Attribute("optional"), "true") == 0);

  params.push_back(param);

  // an optional parameter with "false,true" value is supposed to be part of a two-step algorithm: first get the size, than use it
  return element->Attribute("optional") && (strcmp(element->Attribute("optional"), "false,true") == 0);
}

void readCommandParams(tinyxml2::XMLElement* element, std::set<std::string> & dependencies, CommandData & commandData)
{
  // iterate over the siblings of the element and read the command parameters
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
      // ignore these values!
      assert((value == "implicitexternsyncparams") || (value == "validity"));
    }
  }
}

tinyxml2::XMLNode* readCommandParamType(tinyxml2::XMLNode* node, ParamData& param)
{
  assert(node);
  if (node->ToText())
  {
    // start type with "const" or "struct", if needed
    std::string value = trimEnd(node->Value());
    assert((value == "const") || (value == "struct"));
    param.type = value + " ";
    node = node->NextSibling();
    assert(node);
  }

  // get the pure type
  assert(node->ToElement() && (strcmp(node->Value(), "type") == 0) && node->ToElement()->GetText());
  std::string type = strip(node->ToElement()->GetText(), "Vk");
  param.type += type;
  param.pureType = type;

  // end with "*", "**", or "* const*", if needed
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

  // get return type and name of the command
  std::string type = strip( typeElement->GetText(), "Vk" );
  std::string name = startLowerCase(strip(nameElement->GetText(), "vk"));

  // add an empty DependencyData to this name
  vkData.dependencies.push_back( DependencyData( DependencyData::Category::COMMAND, name ) );

  // insert an empty CommandData into the commands-map, and return the newly created CommandData
  assert( vkData.commands.find( name ) == vkData.commands.end() );
  return vkData.commands.insert( std::make_pair( name, CommandData(type, name) ) ).first->second;
}

void readCommands(tinyxml2::XMLElement * element, VkData & vkData)
{
  for (tinyxml2::XMLElement* child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    assert(strcmp(child->Value(), "command") == 0);
    readCommandsCommand(child, vkData);
  }
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

    // tokenize the successCodes string, using ',' as the separator
    size_t start = 0, end;
    do
    {
      end = successCodes.find(',', start);
      std::string code = successCodes.substr(start, end - start);
      std::string tag = findTag(code, tags);
      // on each success code: prepend 'e', strip "VK_" and a tag, convert it to camel case, and add the tag again
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

  // erase the part after the Copyright text
  size_t pos = header.find("\n\n-----");
  assert(pos != std::string::npos);
  header.erase(pos);

  // replace any '\n' with "\n// "
  for (size_t pos = header.find('\n'); pos != std::string::npos; pos = header.find('\n', pos + 1))
  {
    header.replace(pos, 1, "\n// ");
  }

  // and add a little message on our own
  header += "\n\n// This header is generated from the Khronos Vulkan XML API Registry.";
}

void readEnums( tinyxml2::XMLElement * element, VkData & vkData )
{
  if (!element->Attribute("name"))
  {
    throw std::runtime_error(std::string("spec error: enums element is missing the name attribute"));
  }
  std::string name = strip(element->Attribute("name"), "Vk");

  if ( name != "API Constants" )    // skip the "API Constants"
  {
    // add an empty DependencyData on this name into the dependencies list
    vkData.dependencies.push_back( DependencyData( DependencyData::Category::ENUM, name ) );

    // ad an empty EnumData on this name into the enums map
    std::map<std::string,EnumData>::iterator it = vkData.enums.insert( std::make_pair( name, EnumData(name) ) ).first;

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
      if (it->second.bitmask)
      {
        // for a bitmask enum, start with "VK", cut off the trailing "FlagBits", and convert that name to upper case
        // end that with "Bit"
        size_t pos = name.find("FlagBits");
        assert(pos != std::string::npos);
        it->second.prefix = "VK" + toUpperCase(name.substr(0, pos)) + "_";
      }
      else
      {
        // for a non-bitmask enum, start with "VK", and convert the name to upper case
        it->second.prefix = "VK" + toUpperCase(name) + "_";
      }

      // if the enum name contains a tag move it from the prefix to the postfix to generate correct enum value names.
      for (std::set<std::string>::const_iterator tit = vkData.tags.begin(); tit != vkData.tags.end(); ++tit)
      {
        if ((tit->length() < it->second.prefix.length()) && (it->second.prefix.substr(it->second.prefix.length() - tit->length() - 1) == (*tit + "_")))
        {
          it->second.prefix.erase(it->second.prefix.length() - tit->length() - 1);
          it->second.postfix = "_" + *tit;
          break;
        }
        else if ((tit->length() < it->second.name.length()) && (it->second.name.substr(it->second.name.length() - tit->length()) == *tit))
        {
          it->second.postfix = "_" + *tit;
          break;
        }
      }
    }

    readEnumsEnum( element, it->second );

    // add this enum to the set of Vulkan data types
    assert( vkData.vkTypes.find( name ) == vkData.vkTypes.end() );
    vkData.vkTypes.insert( name );
  }
}

void readEnumsEnum( tinyxml2::XMLElement * element, EnumData & enumData )
{
  // read the names of the enum values
  tinyxml2::XMLElement * child = element->FirstChildElement();
  while (child)
  {
    if ( child->Attribute( "name" ) )
    {
      enumData.addEnumMember(child->Attribute("name"), "");
    }
    child = child->NextSiblingElement();
  }
}

void readDisabledExtensionRequire(tinyxml2::XMLElement * element, VkData & vkData)
{
  for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    std::string value = child->Value();

    if ((value == "command") || (value == "type"))
    {
      // disable a command or a type !
      assert(child->Attribute("name"));
      std::string name = (value == "command") ? startLowerCase(strip(child->Attribute("name"), "vk")) : strip(child->Attribute("name"), "Vk");

      // search this name in the dependencies list and remove it
      std::list<DependencyData>::const_iterator depIt = std::find_if(vkData.dependencies.begin(), vkData.dependencies.end(), [&name](DependencyData const& dd) { return(dd.name == name); });
      assert(depIt != vkData.dependencies.end());
      vkData.dependencies.erase(depIt);

      // erase it from all dependency sets
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
        // a type simply needs to be removed from the structs and vkTypes sets
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
  }
}

void readExtensionCommand(tinyxml2::XMLElement * element, std::map<std::string, CommandData> & commands, std::string const& protect)
{
  // just add the protect string to the CommandData
  if (!protect.empty())
  {
    assert(element->Attribute("name"));
    std::string name = startLowerCase(strip(element->Attribute("name"), "vk"));
    std::map<std::string, CommandData>::iterator cit = commands.find(name);
    assert(cit != commands.end());
    cit->second.protect = protect;
  }
}

void readExtensionEnum(tinyxml2::XMLElement * element, std::map<std::string, EnumData> & enums, std::string const& tag)
{
  // TODO process enums which don't extend existing enums
  if (element->Attribute("extends"))
  {
    assert(element->Attribute("name"));
    assert(enums.find(strip(element->Attribute("extends"), "Vk")) != enums.end());
    assert(!!element->Attribute("bitpos") + !!element->Attribute("offset") + !!element->Attribute("value") == 1);
    auto enumIt = enums.find(strip(element->Attribute("extends"), "Vk"));
    assert(enumIt != enums.end());
    enumIt->second.addEnumMember(element->Attribute("name"), tag);
  }
}

void readExtensionRequire(tinyxml2::XMLElement * element, VkData & vkData, std::string const& protect, std::string const& tag)
{
  for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    std::string value = child->Value();

    if ( value == "command" )
    {
      readExtensionCommand(child, vkData.commands, protect);
    }
    else if (value == "type")
    {
      readExtensionType(child, vkData, protect);
    }
    else if ( value == "enum")
    {
      readExtensionEnum(child, vkData.enums, tag);
    }
    else
    {
      assert(value=="usage");
    }
  }
}

void readExtensions(tinyxml2::XMLElement * element, VkData & vkData)
{
  for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    assert( strcmp( child->Value(), "extension" ) == 0 );
    readExtensionsExtension( child, vkData );
  }
}

void readExtensionsExtension(tinyxml2::XMLElement * element, VkData & vkData)
{
  assert( element->Attribute( "name" ) );
  std::string tag = extractTag(element->Attribute("name"));
  assert(vkData.tags.find(tag) != vkData.tags.end());

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

void readExtensionType(tinyxml2::XMLElement * element, VkData & vkData, std::string const& protect)
{
  // add the protect-string to the appropriate type: enum, flag, handle, scalar, or struct
  if (!protect.empty())
  {
    assert(element->Attribute("name"));
    std::string name = strip(element->Attribute("name"), "Vk");
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
}

tinyxml2::XMLNode* readType(tinyxml2::XMLNode* element, std::string & type, std::string & pureType)
{
	assert(element);
	if (element->ToText())
	{
		std::string value = trimEnd(element->Value());
		assert((value == "const") || (value == "struct"));
		type = value + " ";
		element = element->NextSibling();
		assert(element);
	}

	assert(element->ToElement());
	assert((strcmp(element->Value(), "type") == 0) && element->ToElement() && element->ToElement()->GetText());
	pureType = strip(element->ToElement()->GetText(), "Vk");
	type += pureType;

	element = element->NextSibling();
	assert(element);
	if (element->ToText())
	{
		std::string value = trimEnd(element->Value());
		assert((value == "*") || (value == "**") || (value == "* const*"));
		type += value;
		element = element->NextSibling();
	}
	return element;
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
  else
  {
    // Generate FlagBits name, add a DependencyData for that name, and add it to the list of enums and vulkan types
    requires = generateEnumNameForFlags(name);
    vkData.dependencies.push_back(DependencyData(DependencyData::Category::ENUM, requires));
    vkData.enums.insert(std::make_pair(requires, EnumData(requires, true)));
    vkData.vkTypes.insert(requires);
  }

  // add a DependencyData for the bitmask name, with the required type as its first dependency
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
    // filter out the check for the different types of VK_DEFINE_NON_DISPATCHABLE_HANDLE
    std::string text = element->LastChild()->ToText()->Value();
    size_t start = text.find("#if defined(__LP64__)");
    size_t end = text.find_first_of("\r\n", start + 1);
    vkData.typesafeCheck = text.substr(start, end - start);
  }
  // ignore all the other defines
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
  assert((type.find("VK_DEFINE_HANDLE") == 0) || (type.find("VK_DEFINE_NON_DISPATCHABLE_HANDLE") == 0));
#endif

  tinyxml2::XMLElement * nameElement = typeElement->NextSiblingElement();
  assert( nameElement && ( strcmp( nameElement->Value(), "name" ) == 0 ) && nameElement->GetText() );
  std::string name = strip( nameElement->GetText(), "Vk" );

  vkData.dependencies.push_back( DependencyData( DependencyData::Category::HANDLE, name ) );
  assert(vkData.vkTypes.find(name) == vkData.vkTypes.end());
  vkData.vkTypes.insert(name);
  assert(vkData.handles.find(name) == vkData.handles.end());
  vkData.handles[name];
}

void readTypeStruct( tinyxml2::XMLElement * element, VkData & vkData, bool isUnion )
{
  assert( !element->Attribute( "returnedonly" ) || ( strcmp( element->Attribute( "returnedonly" ), "true" ) == 0 ) );

  assert( element->Attribute( "name" ) );
  std::string name = strip( element->Attribute( "name" ), "Vk" );

  if ( name == "Rect3D" )
  {
    // for whatever reason, VkRect3D is listed in vk.xml, but does not appear in vulkan.h!!
    return;
  }

  vkData.dependencies.push_back( DependencyData( isUnion ? DependencyData::Category::UNION : DependencyData::Category::STRUCT, name ) );

  assert( vkData.structs.find( name ) == vkData.structs.end() );
  std::map<std::string,StructData>::iterator it = vkData.structs.insert( std::make_pair( name, StructData() ) ).first;
  it->second.returnedOnly = !!element->Attribute( "returnedonly" );
  it->second.isUnion = isUnion;

  for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    assert( child->Value() );
    std::string value = child->Value();
    assert(value == "member");
    readTypeStructMember( child, it->second.members, vkData.dependencies.back().dependencies );
  }

  assert( vkData.vkTypes.find( name ) == vkData.vkTypes.end() );
  vkData.vkTypes.insert( name );
}

void readTypeStructMember(tinyxml2::XMLElement * element, std::vector<MemberData> & members, std::set<std::string> & dependencies)
{
  members.push_back(MemberData());
  MemberData & member = members.back();

  tinyxml2::XMLNode* child = readType(element->FirstChild(), member.type, member.pureType);
  dependencies.insert(member.pureType);

  assert((child->ToElement() && strcmp(child->Value(), "name") == 0));
  member.name = child->ToElement()->GetText();

  member.arraySize = readArraySize(child, member.name);
}

void readTags(tinyxml2::XMLElement * element, std::set<std::string> & tags)
{
  tags.insert("EXT");
  tags.insert("KHR");
  for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    assert(child->Attribute("name"));
    tags.insert(child->Attribute("name"));
  }
}

void readTypes(tinyxml2::XMLElement * element, VkData & vkData)
{
  for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
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
        readTypeStruct( child, vkData, false );
      }
      else if ( category == "union" )
      {
        readTypeStruct( child, vkData, true );
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
  }
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
      if (std::find_if(it->dependencies.begin(), it->dependencies.end(), [&listedTypes](std::string const& d) { return listedTypes.find(d) == listedTypes.end(); }) == it->dependencies.end())
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

std::string startLowerCase(std::string const& input)
{
  return static_cast<char>(tolower(input[0])) + input.substr(1);
}

std::string startUpperCase(std::string const& input)
{
  return static_cast<char>(toupper(input[0])) + input.substr(1);
}

std::string strip(std::string const& value, std::string const& prefix, std::string const& postfix)
{
  std::string strippedValue = value;
  if (strippedValue.substr(0, prefix.length()) == prefix)
  {
    strippedValue.erase(0, prefix.length());
  }
  if (!postfix.empty() && (strippedValue.substr(strippedValue.length() - postfix.length()) == postfix))
  {
    strippedValue.erase(strippedValue.length() - postfix.length());
  }
  return strippedValue;
}

std::string stripPluralS(std::string const& name)
{
  std::string strippedName(name);
  size_t pos = strippedName.rfind('s');
  assert(pos != std::string::npos);
  strippedName.erase(pos, 1);
  return strippedName;
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

// trim from end
std::string trimEnd(std::string const& input)
{
  std::string result = input;
  result.erase(std::find_if(result.rbegin(), result.rend(), [](char c) { return !std::isspace(c); }).base(), result.end());
  return result;
}

void writeCall(std::ostream & os, CommandData const& commandData, std::set<std::string> const& vkTypes, bool firstCall, bool singular)
{
  // get the parameter indices of the counter for vector parameters
  std::map<size_t,size_t> countIndices;
  for (std::map<size_t, size_t>::const_iterator it = commandData.vectorParams.begin(); it != commandData.vectorParams.end(); ++it)
  {
    countIndices.insert(std::make_pair(it->second, it->first));
  }

  // the original function call
  os << "vk" << startUpperCase(commandData.fullName) << "( ";

  if (!commandData.className.empty())
  {
    // if it's member of a class -> add the first parameter with "m_" as prefix
    os << "m_" << commandData.params[0].name;
  }

  for (size_t i=commandData.className.empty() ? 0 : 1; i < commandData.params.size(); i++)
  {
    if (0 < i)
    {
      os << ", ";
    }

    std::map<size_t, size_t>::const_iterator it = countIndices.find(i);
    if (it != countIndices.end())
    {
      writeCallCountParameter(os, commandData, singular, it);
    }
    else if ((it = commandData.vectorParams.find(i)) != commandData.vectorParams.end())
    {
      writeCallVectorParameter(os, commandData, vkTypes, firstCall, singular, it);
    }
    else 
    {
      if (vkTypes.find(commandData.params[i].pureType) != vkTypes.end())
      {
        writeCallVulkanTypeParameter(os, commandData.params[i]);
      }
      else
      {
        writeCallPlainTypeParameter(os, commandData.params[i]);
      }
    }
  }
  os << " )";
}

void writeCallCountParameter(std::ostream & os, CommandData const& commandData, bool singular, std::map<size_t, size_t>::const_iterator it)
{
  // this parameter is a count parameter for a vector parameter
  if ((commandData.returnParam == it->second) && commandData.twoStep)
  {
    // the corresponding vector parameter is the return parameter and it's a two-step algorithm
    // -> use the pointer to a local variable named like the counter parameter without leading 'p'
    os << "&" << startLowerCase(strip(commandData.params[it->first].name, "p"));
  }
  else
  {
    // the corresponding vector parameter is not the return parameter, or it's not a two-step algorithm
    if (singular)
    {
      // for the singular version, the count is just 1.
      os << "1 ";
    }
    else
    {
      // for the non-singular version, the count is the size of the vector parameter
      // -> use the vector parameter name without leading 'p' to get the size (in number of elements, not in bytes)
      os << startLowerCase(strip(commandData.params[it->second].name, "p")) << ".size() ";
    }
    if (commandData.templateParam == it->second)
    {
      // if the vector parameter is templatized -> multiply by the size of that type to get the size in bytes
      os << "* sizeof( T ) ";
    }
  }
}

void writeCallPlainTypeParameter(std::ostream & os, ParamData const& paramData)
{
  // this parameter is just a plain type
  if (paramData.type.back() == '*')
  {
    // it's a pointer
    std::string parameterName = startLowerCase(strip(paramData.name, "p"));
    if (paramData.type.find("const") != std::string::npos)
    {
      // it's a const pointer
      if (paramData.pureType == "char")
      {
        // it's a const pointer to char -> it's a string -> get the data via c_str()
        os << parameterName;
        if (paramData.optional)
        {
          // it's optional -> might use nullptr
          os << " ? " << parameterName << "->c_str() : nullptr";
        }
        else
        {
          os << ".c_str()";
        }
      }
      else
      {
        // it's const pointer to void (only other type that occurs) -> just use the name
        assert((paramData.pureType == "void") && !paramData.optional);
        os << paramData.name;
      }
    }
    else
    {
      // it's a non-const pointer, and char is the only type that occurs -> use the address of the parameter
      assert(paramData.type.find("char") == std::string::npos);
      os << "&" << parameterName;
    }
  }
  else
  {
    // it's a plain parameter -> just use its name
    os << paramData.name;
  }
}

void writeCallVectorParameter(std::ostream & os, CommandData const& commandData, std::set<std::string> const& vkTypes, bool firstCall, bool singular, std::map<size_t, size_t>::const_iterator it)
{
  // this parameter is a vector parameter
  assert(commandData.params[it->first].type.back() == '*');
  if ((commandData.returnParam == it->first) && commandData.twoStep && firstCall)
  {
    // this parameter is the return parameter, and it's the first call of a two-step algorithm -> just just nullptr
    os << "nullptr";
  }
  else
  {
    std::string parameterName = startLowerCase(strip(commandData.params[it->first].name, "p"));
    std::set<std::string>::const_iterator vkit = vkTypes.find(commandData.params[it->first].pureType);
    if ((vkit != vkTypes.end()) || (it->first == commandData.templateParam))
    {
      // CHECK for !commandData.params[it->first].optional

      // this parameter is a vulkan type or a templated type -> need to reinterpret cast
      writeReinterpretCast(os, commandData.params[it->first].type.find("const") == 0, vkit != vkTypes.end(), commandData.params[it->first].pureType,
        commandData.params[it->first].type.rfind("* const") != std::string::npos);
      os << "( ";
      if (singular)
      {
        // in singular case, strip the plural-S from the name, and use the pointer to that thing
        os << "&" << stripPluralS(parameterName);
      }
      else
      {
        // in plural case, get the pointer to the data
        os << parameterName << ".data()";
      }
      os << " )";
    }
    else if (commandData.params[it->first].pureType == "char")
    {
      // the parameter is a vector to char -> it might be optional
      // besides that, the parameter now is a std::string -> get the pointer via c_str()
      os << parameterName;
      if (commandData.params[it->first].optional)
      {
        os << " ? " << parameterName << "->c_str() : nullptr";
      }
      else
      {
        os << ".c_str()";
      }
    }
    else
    {
      // this parameter is just a vetor -> get the pointer to its data
      os << parameterName << ".data()";
    }
  }
}

void writeCallVulkanTypeParameter(std::ostream & os, ParamData const& paramData)
{
  // this parameter is a vulkan type
  if (paramData.type.back() == '*')
  {
    // it's a pointer -> needs a reinterpret cast to the vulkan type
    std::string parameterName = startLowerCase(strip(paramData.name, "p"));
    writeReinterpretCast(os, paramData.type.find("const") != std::string::npos, true, paramData.pureType, false);
    os << "( ";
    if (paramData.optional)
    {
      // for an optional parameter, we need also a static_cast from optional type to const-pointer to pure type
      os << "static_cast<const " << paramData.pureType << "*>( " << parameterName << " )";
    }
    else
    {
      // other parameters can just use the pointer
      os << "&" << parameterName;
    }
    os << " )";
  }
  else
  {
    // a non-pointer parameter needs a static_cast from vk::-type to vulkan type
    os << "static_cast<Vk" << paramData.pureType << ">( " << paramData.name << " )";
  }
}

void writeFunction(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool definition, bool enhanced, bool singular, bool unique)
{
  if (enhanced && !singular)
  {
    writeFunctionHeaderTemplate(os, indentation, commandData, !definition);
  }
  os << indentation << (definition ? "VULKAN_HPP_INLINE " : "");
  writeFunctionHeaderReturnType(os, indentation, commandData, enhanced, singular, unique);
  if (definition && !commandData.className.empty())
  {
    os << commandData.className << "::";
  }
  writeFunctionHeaderName(os, commandData.reducedName, singular, unique);
  writeFunctionHeaderArguments(os, vkData, commandData, enhanced, singular, !definition);
  os << (definition ? "" : ";") << std::endl;

  if (definition)
  {
    // write the function body
    os << indentation << "{" << std::endl;
    if (enhanced)
    {
      if (unique)
      {
        writeFunctionBodyUnique(os, indentation, vkData, commandData, singular);
      }
      else
      {
        writeFunctionBodyEnhanced(os, indentation, vkData, commandData, singular);
      }
    }
    else
    {
      writeFunctionBodyStandard(os, indentation, vkData, commandData);
    }
    os << indentation << "}" << std::endl;
  }
}

void writeFunctionBodyEnhanced(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool singular)
{
  if (1 < commandData.vectorParams.size())
  {
    writeFunctionBodyEnhancedMultiVectorSizeCheck(os, indentation, commandData);
  }

  std::string returnName;
  if (commandData.returnParam != ~0)
  {
    returnName = writeFunctionBodyEnhancedLocalReturnVariable(os, indentation, commandData, singular);
  }

  if (commandData.twoStep)
  {
    assert(!singular);
    writeFunctionBodyEnhancedLocalCountVariable(os, indentation, commandData);

    // we now might have to check the result, resize the returned vector accordingly, and call the function again
    std::map<size_t, size_t>::const_iterator returnit = commandData.vectorParams.find(commandData.returnParam);
    assert(returnit != commandData.vectorParams.end() && (returnit->second != ~0));
    std::string sizeName = startLowerCase(strip(commandData.params[returnit->second].name, "p"));

    if (commandData.returnType == "Result")
    {
      if (1 < commandData.successCodes.size())
      {
        writeFunctionBodyEnhancedCallTwoStepIterate(os, indentation, vkData.vkTypes, returnName, sizeName, commandData);
      }
      else
      {
        writeFunctionBodyEnhancedCallTwoStepChecked(os, indentation, vkData.vkTypes, returnName, sizeName, commandData);
      }
    }
    else
    {
      writeFunctionBodyEnhancedCallTwoStep(os, indentation, vkData.vkTypes, returnName, sizeName, commandData);
    }
  }
  else
  {
    if (commandData.returnType == "Result")
    {
      writeFunctionBodyEnhancedCallResult(os, indentation, vkData.vkTypes, commandData, singular);
    }
    else
    {
      writeFunctionBodyEnhancedCall(os, indentation, vkData.vkTypes, commandData, singular);
    }
  }

  if ((commandData.returnType == "Result") || !commandData.successCodes.empty())
  {
    writeFunctionBodyEnhancedReturnResultValue(os, indentation, returnName, commandData, singular);
  }
  else if ((commandData.returnParam != ~0) && (commandData.returnType != commandData.enhancedReturnType))
  {
    // for the other returning cases, when the return type is somhow enhanced, just return the local returnVariable
    os << indentation << "  return " << returnName << ";" << std::endl;
  }
}

void writeFunctionBodyEnhancedCall(std::ostream &os, std::string const& indentation, std::set<std::string> const& vkTypes, CommandData const& commandData, bool singular)
{
  os << indentation << "  ";

  if (commandData.returnType != "void")
  {
    os << "return ";
  }
  writeCall(os, commandData, vkTypes, true, singular);
  os << ";" << std::endl;
}

void writeFunctionBodyEnhancedCallResult(std::ostream &os, std::string const& indentation, std::set<std::string> const& vkTypes, CommandData const& commandData, bool singular)
{
  os << indentation << "  Result result = static_cast<Result>( ";
  writeCall(os, commandData, vkTypes, true, singular);
  os << " );" << std::endl;
}

void writeFunctionBodyEnhancedCallTwoStep(std::ostream & os, std::string const& indentation, std::set<std::string> const& vkTypes, std::string const& returnName, std::string const& sizeName, CommandData const& commandData)
{
  os << indentation << "  ";
  writeCall(os, commandData, vkTypes, true, false);
  os << ";" << std::endl;

  // resize the vector to hold the data according to the result from the first call
  os << indentation << "  " << returnName << ".resize( " << sizeName << " );" << std::endl
    << indentation << "  ";
  writeCall(os, commandData, vkTypes, false, false);
  os << ";" << std::endl;
}

void writeFunctionBodyEnhancedCallTwoStepChecked(std::ostream & os, std::string const& indentation, std::set<std::string> const& vkTypes, std::string const& returnName, std::string const& sizeName, CommandData const& commandData)
{
  os << indentation << "  Result result = static_cast<Result>( ";
  writeCall(os, commandData, vkTypes, true, false);
  os << " );" << std::endl
    << indentation << "  if ( ( result == Result::eSuccess ) && " << sizeName << " )" << std::endl
    << indentation << "  {" << std::endl
    << indentation << "    " << returnName << ".resize( " << sizeName << " );" << std::endl
    << indentation << "    result = static_cast<Result>( ";
  writeCall(os, commandData, vkTypes, false, false);
  os << " );" << std::endl
    << indentation << "  }" << std::endl;
}

void writeFunctionBodyEnhancedCallTwoStepIterate(std::ostream & os, std::string const& indentation, std::set<std::string> const& vkTypes, std::string const& returnName, std::string const& sizeName, CommandData const& commandData)
{
  // we need a local variable of type 'Result' to get the results of the calls
  os << indentation << "  Result result;" << std::endl
    << indentation << "  do" << std::endl
    << indentation << "  {" << std::endl
    << indentation << "    result = static_cast<Result>( ";
  writeCall(os, commandData, vkTypes, true, false);
  os << " );" << std::endl;

  // check if the result was success and the size != 0
  os << indentation << "    if ( ( result == Result::eSuccess ) && " << sizeName << " )" << std::endl
    << indentation << "    {" << std::endl
    // resize the vector to hold the data according to the result from the first call
    << indentation << "      " << returnName << ".resize( " << sizeName << " );" << std::endl
    // we need a static cast again
    << indentation << "      result = static_cast<Result>( ";
  writeCall(os, commandData, vkTypes, false, false);
  os << " );" << std::endl
    << indentation << "    }" << std::endl
    // close the loop until we got complete data
    << indentation << "  } while ( result == Result::eIncomplete );" << std::endl
    << indentation << "  assert( " << sizeName << " <= " << returnName << ".size() ); " << std::endl
    << indentation << "  " << returnName << ".resize( " << sizeName << " ); " << std::endl;
}

void writeFunctionBodyEnhancedLocalCountVariable(std::ostream & os, std::string const& indentation, CommandData const& commandData)
{
  // local count variable to hold the size of the vector to fill
  assert(commandData.returnParam != ~0);

  std::map<size_t, size_t>::const_iterator returnit = commandData.vectorParams.find(commandData.returnParam);
  assert(returnit != commandData.vectorParams.end() && (returnit->second != ~0));
  assert((commandData.returnType == "Result") || (commandData.returnType == "void"));

  // take the pure type of the size parameter; strip the leading 'p' from its name for its local name
  os << indentation << "  " << commandData.params[returnit->second].pureType << " " << startLowerCase(strip(commandData.params[returnit->second].name, "p")) << ";" << std::endl;
}

std::string writeFunctionBodyEnhancedLocalReturnVariable(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool singular)
{
  std::string returnName = startLowerCase(strip(commandData.params[commandData.returnParam].name, "p"));

  // there is a returned parameter -> we need a local variable to hold that value
  if (commandData.returnType != commandData.enhancedReturnType)
  {
    // the returned parameter is somehow enanced by us
    os << indentation << "  ";
    if (singular)
    {
      // in singular case, just use the return parameters pure type for the return variable
      returnName = stripPluralS(returnName);
      os << commandData.params[commandData.returnParam].pureType << " " << returnName;
    }
    else
    {
      // in non-singular case, use the enhanced type for the return variable (like vector<...>)
      os << commandData.enhancedReturnType << " " << returnName;

      std::map<size_t, size_t>::const_iterator it = commandData.vectorParams.find(commandData.returnParam);
      if (it != commandData.vectorParams.end() && !commandData.twoStep)
      {
        // if the return parameter is a vector parameter, and not part of a two-step algorithm, initialize its size
        std::string size;
        if (it->second == ~0)
        {
          assert(!commandData.params[commandData.returnParam].len.empty());
          // the size of the vector is not given by an other parameter, but by some member of a parameter, described as 'parameter::member'
          // -> replace the '::' by '.' and filter out the leading 'p' to access that value
          size = startLowerCase(strip(commandData.params[commandData.returnParam].len, "p"));
          size_t pos = size.find("::");
          assert(pos != std::string::npos);
          size.replace(pos, 2, ".");
        }
        else
        {
          // the size of the vector is given by an other parameter
          // that means (as this is not a two-step algorithm) it's size is determined by some other vector parameter!
          // -> look for it and get it's actual size
          for (auto const& vectorParam : commandData.vectorParams)
          {
            if ((vectorParam.first != commandData.returnParam) && (vectorParam.second == it->second))
            {
              size = startLowerCase(strip(commandData.params[vectorParam.first].name, "p")) + ".size()";
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
  else
  {
    // the return parameter is not enhanced -> the type is supposed to be a Result and there are more than one success codes!
    assert((commandData.returnType == "Result") && (1 < commandData.successCodes.size()));
    os << indentation << "  " << commandData.params[commandData.returnParam].pureType << " " << returnName << ";" << std::endl;
  }

  return returnName;
}

void writeFunctionBodyEnhancedMultiVectorSizeCheck(std::ostream & os, std::string const& indentation, CommandData const& commandData)
{
  // add some error checks if multiple vectors need to have the same size
  for (std::map<size_t, size_t>::const_iterator it0 = commandData.vectorParams.begin(); it0 != commandData.vectorParams.end(); ++it0)
  {
    if (it0->first != commandData.returnParam)
    {
      for (std::map<size_t, size_t>::const_iterator it1 = std::next(it0); it1 != commandData.vectorParams.end(); ++it1)
      {
        if ((it1->first != commandData.returnParam) && (it0->second == it1->second))
        {
          std::string firstVectorName = startLowerCase(strip(commandData.params[it0->first].name, "p"));
          std::string secondVectorName = startLowerCase(strip(commandData.params[it1->first].name, "p"));
          os << "#ifdef VULKAN_HPP_NO_EXCEPTIONS" << std::endl
            << indentation << "  assert( " << firstVectorName << ".size() == " << secondVectorName << ".size() );" << std::endl
            << "#else" << std::endl
            << indentation << "  if ( " << firstVectorName << ".size() != " << secondVectorName << ".size() )" << std::endl
            << indentation << "  {" << std::endl
            << indentation << "    throw std::logic_error( \"vk::" << commandData.className << "::" << commandData.reducedName << ": " << firstVectorName << ".size() != " << secondVectorName << ".size()\" );" << std::endl
            << indentation << "  }" << std::endl
            << "#endif  // VULKAN_HPP_NO_EXCEPTIONS" << std::endl;
        }
      }
    }
  }
}

void writeFunctionBodyEnhancedReturnResultValue(std::ostream & os, std::string const& indentation, std::string const& returnName, CommandData const& commandData, bool singular)
{
  // if the return type is "Result" or there is at least one success code, create the Result/Value construct to return
  os << indentation << "  return createResultValue( result, ";
  if (commandData.returnParam != ~0)
  {
    // if there's a return parameter, list it in the Result/Value constructor
    os << returnName << ", ";
  }

  // now the function name (with full namespace) as a string
  os << "\"vk::" << (commandData.className.empty() ? "" : commandData.className + "::") << (singular ? stripPluralS(commandData.reducedName) : commandData.reducedName) << "\"";

  if (!commandData.twoStep && (1 < commandData.successCodes.size()))
  {
    // and for the single-step algorithms with more than one success code list them all
    os << ", { Result::" << commandData.successCodes[0];
    for (size_t i = 1; i < commandData.successCodes.size(); i++)
    {
      os << ", Result::" << commandData.successCodes[i];
    }
    os << " }";
  }
  os << " );" << std::endl;
}

void writeFunctionBodyStandard(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData)
{
  os << indentation << "  ";
  bool castReturn = false;
  if (commandData.returnType != "void")
  {
    // there's something to return...
    os << "return ";

    castReturn = (vkData.vkTypes.find(commandData.returnType) != vkData.vkTypes.end());
    if (castReturn)
    {
      // the return-type is a vulkan type -> need to cast to vk::-type
      os << "static_cast<" << commandData.returnType << ">( ";
    }
  }

  // call the original function
  os << "vk" << startUpperCase(commandData.fullName) << "( ";

  if (!commandData.className.empty())
  {
    // the command is part of a class -> the first argument is the member variable, starting with "m_"
    os << "m_" << commandData.params[0].name;
  }

  // list all the arguments
  for (size_t i = commandData.className.empty() ? 0 : 1; i < commandData.params.size(); i++)
  {
    if (0 < i)
    {
      os << ", ";
    }

    if (vkData.vkTypes.find(commandData.params[i].pureType) != vkData.vkTypes.end())
    {
      // the parameter is a vulkan type
      if (commandData.params[i].type.back() == '*')
      {
        // it's a pointer -> need to reinterpret_cast it
        writeReinterpretCast(os, commandData.params[i].type.find("const") == 0, true, commandData.params[i].pureType, commandData.params[i].type.find("* const") != std::string::npos);
      }
      else
      {
        // it's a value -> need to static_cast ist
        os << "static_cast<Vk" << commandData.params[i].pureType << ">";
      }
      os << "( " << commandData.params[i].name << " )";
    }
    else
    {
      // it's a non-vulkan type -> just use it
      os << commandData.params[i].name;
    }
  }
  os << " )";

  if (castReturn)
  {
    // if we cast the return -> close the static_cast
    os << " )";
  }
  os << ";" << std::endl;
}

void writeFunctionBodyUnique(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool singular)
{
  // the unique version needs a Deleter object for destruction of the newly created stuff
  std::string type = commandData.params[commandData.returnParam].pureType;
  std::string typeValue = startLowerCase(type);
  os << indentation << "  " << type << "Deleter deleter( ";
  if (vkData.deleterData.find(commandData.className) != vkData.deleterData.end())
  {
    // if the Deleter is specific to the command's class, add '*this' to the deleter
    os << "*this, ";
  }

  // get the DeleterData corresponding to the returned type
  std::map<std::string, DeleterData>::const_iterator ddit = vkData.deleterData.find(type);
  assert(ddit != vkData.deleterData.end());
  if (ddit->second.pool.empty())
  {
    // if this type isn't pooled, use the allocator (provided as a function argument)
    os << "allocator";
  }
  else
  {
    // otherwise use the pool, which always is a member of the second argument
    os << startLowerCase(strip(commandData.params[1].name, "p")) << "." << startLowerCase(ddit->second.pool);
  }
  os << " );" << std::endl;

  bool returnsVector = !singular && (commandData.vectorParams.find(commandData.returnParam) != commandData.vectorParams.end());
  if (returnsVector)
  {
    // if a vector of data is returned, use a local variable to hold the returned data from the non-unique function call
    os << indentation << "  std::vector<" << type << ",Allocator> " << typeValue << "s = ";
  }
  else
  {
    // otherwise create a Unique stuff out of the returned data from the non-unique function call
    os << indentation << "  return Unique" << type << "( ";
  }

  // the call to the non-unique function
  os << (singular ? stripPluralS(commandData.fullName) : commandData.fullName) << "( ";
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

      // strip off the leading 'p' for pointer arguments
      std::string argumentName = (commandData.params[i].type.back() == '*') ? startLowerCase(strip(commandData.params[i].name, "p")) : commandData.params[i].name;
      if (singular && (commandData.vectorParams.find(i) != commandData.vectorParams.end()))
      {
        // and strip off the plural 's' if appropriate
        argumentName = stripPluralS(argumentName);
      }
      os << argumentName;
    }
  }
  os << " )";
  if (returnsVector)
  {
    // for vector returns, copy over the results from the non-unique function into a vector of Unique stuff
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
    // for non-vector returns, just add the deleter (local variable) to the Unique-stuff constructor
    os << ", deleter );" << std::endl;
  }
}

void writeFunctionHeaderArguments(std::ostream & os, VkData const& vkData, CommandData const& commandData, bool enhanced, bool singular, bool withDefaults)
{
  os << "(";
  if (enhanced)
  {
    writeFunctionHeaderArgumentsEnhanced(os, vkData, commandData, singular, withDefaults);
  }
  else
  {
    writeFunctionHeaderArgumentsStandard(os, commandData);
  }
  os << ")";
  if (!commandData.className.empty())
  {
    os << " const";
  }
}

void writeFunctionHeaderArgumentsEnhanced(std::ostream & os, VkData const& vkData, CommandData const& commandData, bool singular, bool withDefaults)
{
  // check if there's at least one argument left to put in here
  if (commandData.skippedParams.size() + (commandData.className.empty() ? 0 : 1) < commandData.params.size())
  {
    // determine the last argument, where we might provide some default for
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
        std::string strippedParameterName = startLowerCase(strip(commandData.params[i].name, "p"));

        std::map<size_t, size_t>::const_iterator it = commandData.vectorParams.find(i);
        size_t rightStarPos = commandData.params[i].type.rfind('*');
        if (it == commandData.vectorParams.end())
        {
          // the argument ist not a vector
          if (rightStarPos == std::string::npos)
          {
            // and its not a pointer -> just use its type and name here
            os << commandData.params[i].type << " " << commandData.params[i].name;
            if (!commandData.params[i].arraySize.empty())
            {
              os << "[" << commandData.params[i].arraySize << "]";
            }

            if (withDefaults && (lastArgument == i))
            {
              // check if the very last argument is a flag without any bits -> provide some empty default for it
              std::map<std::string, FlagData>::const_iterator flagIt = vkData.flags.find(commandData.params[i].pureType);
              if (flagIt != vkData.flags.end())
              {
                // get the enum corresponding to this flag, to check if it's empty
                std::list<DependencyData>::const_iterator depIt = std::find_if(vkData.dependencies.begin(), vkData.dependencies.end(), [&flagIt](DependencyData const& dd) { return(dd.name == flagIt->first); });
                assert((depIt != vkData.dependencies.end()) && (depIt->dependencies.size() == 1));
                std::map<std::string, EnumData>::const_iterator enumIt = vkData.enums.find(*depIt->dependencies.begin());
                assert(enumIt != vkData.enums.end());
                if (enumIt->second.members.empty())
                {
                  // there are no bits in this flag -> provide the default
                  os << " = " << commandData.params[i].pureType << "()";
                }
              }
            }
          }
          else
          {
            // the argument is not a vector, but a pointer
            assert(commandData.params[i].type[rightStarPos] == '*');
            if (commandData.params[i].optional)
            {
              // for an optional argument, trim the trailing '*' from the type, and the leading 'p' from the name
              os << "Optional<" << trimEnd(commandData.params[i].type.substr(0, rightStarPos)) << "> " << strippedParameterName;
              if (withDefaults)
              {
                os << " = nullptr";
              }
            }
            else if (commandData.params[i].pureType == "void")
            {
              // for void-pointer, just use type and name
              os << commandData.params[i].type << " " << commandData.params[i].name;
            }
            else if (commandData.params[i].pureType != "char")
            {
              // for non-char-pointer, change to reference
              os << trimEnd(commandData.params[i].type.substr(0, rightStarPos)) << " & " << strippedParameterName;
            }
            else
            {
              // for char-pointer, change to const reference to std::string
              os << "const std::string & " << strippedParameterName;
            }
          }
        }
        else
        {
          // the argument is a vector
          // it's optional, if it's marked as optional and there's no size specified
          bool optional = commandData.params[i].optional && (it->second == ~0);
          assert((rightStarPos != std::string::npos) && (commandData.params[i].type[rightStarPos] == '*'));
          if (commandData.params[i].type.find("char") != std::string::npos)
          {
            // it's a char-vector -> use a std::string (either optional or a const-reference
            if (optional)
            {
              os << "Optional<const std::string> " << strippedParameterName;
              if (withDefaults)
              {
                os << " = nullptr";
              }
            }
            else
            {
              os << "const std::string & " << strippedParameterName;
            }
          }
          else
          {
            // it's a non-char vector (they are never optional)
            assert(!optional);
            if (singular)
            {
              // in singular case, change from pointer to reference
              os << trimEnd(commandData.params[i].type.substr(0, rightStarPos)) << " & " << stripPluralS(strippedParameterName);
            }
            else
            {
              // otherwise, use our ArrayProxy
              bool isConst = (commandData.params[i].type.find("const") != std::string::npos);
              os << "ArrayProxy<" << ((commandData.templateParam == i) ? (isConst ? "const T" : "T") : trimEnd(commandData.params[i].type.substr(0, rightStarPos))) << "> " << strippedParameterName;
            }
          }
        }
        argEncountered = true;
      }
    }
    os << " ";
  }
}

void writeFunctionHeaderArgumentsStandard(std::ostream & os, CommandData const& commandData)
{
  // for the standard case, just list all the arguments as we've got them
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

void writeFunctionHeaderName(std::ostream & os, std::string const& name, bool singular, bool unique)
{
  os << (singular ? stripPluralS(name) : name);
  if (unique)
  {
    os << "Unique";
  }
}

void writeFunctionHeaderReturnType(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool enhanced, bool singular, bool unique)
{
  if (enhanced)
  {
    // the enhanced function might return some pretty complex return stuff
    if (unique)
    {
      // the unique version returns something prefixed with 'Unique'; potentially a vector of that stuff
      // it's a vector, if it's not the singular version and the return parameter is a vector parameter
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
      // if the enhanced return type differs from the original return type, and it's not void, we return a ResultValueType<...>::type
      if (!singular && (commandData.enhancedReturnType.find("Allocator") != std::string::npos))
      {
        // for the non-singular case with allocation, we need to prepend with 'typename' to keep compilers happy
        os << "typename ";
      }
      assert(commandData.returnType == "Result");
      // in singular case, we create the ResultValueType from the pure return type, otherwise from the enhanced return type
      os << "ResultValueType<" << (singular ? commandData.params[commandData.returnParam].pureType : commandData.enhancedReturnType) << ">::type ";
    }
    else if ((commandData.returnParam != ~0) && (1 < commandData.successCodes.size()))
    {
      // if there is a return parameter at all, and there are multiple success codes, we return a ResultValue<...> with the pure return type 
      assert(commandData.returnType == "Result");
      os << "ResultValue<" << commandData.params[commandData.returnParam].pureType << "> ";
    }
    else
    {
      // and in every other case, we just return the enhanced return type.
      os << commandData.enhancedReturnType << " ";
    }
  }
  else
  {
    // the non-enhanced function just uses the return type
    os << commandData.returnType << " ";
  }
}

void writeFunctionHeaderTemplate(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool withDefault)
{
  if ((commandData.templateParam != ~0) && ((commandData.templateParam != commandData.returnParam) || (commandData.enhancedReturnType == "Result")))
  {
    // if there's a template parameter, not being the return parameter or where the enhanced return type is 'Result' -> templatize on type 'T'
    assert(commandData.enhancedReturnType.find("Allocator") == std::string::npos);
    os << indentation << "template <typename T>" << std::endl;
  }
  else if ((commandData.enhancedReturnType.find("Allocator") != std::string::npos))
  {
    // otherwise, if there's an Allocator used in the enhanced return type, we templatize on that Allocator
    assert((commandData.enhancedReturnType.substr(0, 12) == "std::vector<") && (commandData.enhancedReturnType.find(',') != std::string::npos) && (12 < commandData.enhancedReturnType.find(',')));
    os << indentation << "template <typename Allocator";
    if (withDefault)
    {
      // for the default type get the type from the enhancedReturnType, which is of the form 'std::vector<Type,Allocator>'
      os << " = std::allocator<" << commandData.enhancedReturnType.substr(12, commandData.enhancedReturnType.find(',') - 12) << ">";
    }
    os << "> " << std::endl;
  }
}

void writeReinterpretCast(std::ostream & os, bool leadingConst, bool vulkanType, std::string const& type, bool trailingPointerToConst)
{
  os << "reinterpret_cast<";
  if (leadingConst)
  {
    os << "const ";
  }
  if (vulkanType)
  {
    os << "Vk";
  }
  os << type;
  if (trailingPointerToConst)
  {
    os << "* const";
  }
  os << "*>";
}

void writeStandardOrEnhanced(std::ofstream & ofs, std::string const& standard, std::string const& enhanced)
{
  if (standard == enhanced)
  {
    // standard and enhanced string are equal -> just use one of them and we're done
    ofs << standard;
  }
  else
  {
    // standard and enhanced string differ -> use both, wrapping the enhanced by !VULKAN_HPP_DISABLE_ENHANCED_MODE
    // determine the argument list of that standard, and compare it with that of the enhanced
    // if they are equal -> need to have just one; if they differ -> need to have both
    size_t standardStart = standard.find('(');
    size_t standardCount = standard.find(')', standardStart) - standardStart;
    size_t enhancedStart = enhanced.find('(');
    bool unchangedInterface = (standard.substr(standardStart, standardCount) == enhanced.substr(enhancedStart, standardCount));
    if (unchangedInterface)
    {
      ofs << "#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE" << std::endl;
    }
    ofs << standard
      << (unchangedInterface ? "#else" : "#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE") << std::endl
      << enhanced
      << "#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/" << std::endl;
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
    // skip members 'pNext' and 'sType', as they are never explicitly set
    if ((structData.members[i].name != "pNext") && (structData.members[i].name != "sType"))
    {
      // find a default value for the given pure type
      std::map<std::string, std::string>::const_iterator defaultIt = defaultValues.find(structData.members[i].pureType);
      assert(defaultIt != defaultValues.end());

      if (structData.members[i].arraySize.empty())
      {
        // the arguments name get a trailing '_', to distinguish them from the actual struct members
        // pointer arguments get a nullptr as default
        ofs << structData.members[i].type << " " << structData.members[i].name << "_ = " << (structData.members[i].type.back() == '*' ? "nullptr" : defaultIt->second);
      }
      else
      {
        // array members are provided as const reference to a std::array
        // the arguments name get a trailing '_', to distinguish them from the actual struct members
        // list as many default values as there are elements in the array
        ofs << "std::array<" << structData.members[i].type << "," << structData.members[i].arraySize << "> const& " << structData.members[i].name << "_ = { { " << defaultIt->second;
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
      // here, we can only handle non-array arguments
      ofs << "      " << (firstArgument ? ":" : ",") << " " << structData.members[i].name << "( ";

      // 'pNext' and 'sType' don't get an argument, use nullptr and the correct StructureType enum value to initialize them
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
        // the other elements are initialized by the corresponding argument (with trailing '_', as mentioned above)
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
      // here we can handle the arrays, copying over from argument (with trailing '_') to member
      // size is arraySize times sizeof type
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

void writeStructSetter( std::ofstream & ofs, std::string const& structureName, MemberData const& memberData, std::set<std::string> const& vkTypes )
{
  if (memberData.type != "StructureType") // filter out StructureType, which is supposed to be immutable !
  {
    // the setters return a reference to the structure
    ofs << "    " << structureName << "& set" << startUpperCase(memberData.name) << "( ";
    if (memberData.arraySize.empty())
    {
      ofs << memberData.type << " ";
    }
    else
    {
      ofs << "std::array<" << memberData.type << "," << memberData.arraySize << "> ";
    }
    // add a trailing '_' to the argument to distinguish it from the structure member
    ofs << memberData.name << "_ )" << std::endl
      << "    {" << std::endl;
    // copy over the argument, either by assigning simple data, or by memcpy array data
    if (memberData.arraySize.empty())
    {
      ofs << "      " << memberData.name << " = " << memberData.name << "_";
    }
    else
    {
      ofs << "      memcpy( &" << memberData.name << ", " << memberData.name << "_.data(), " << memberData.arraySize << " * sizeof( " << memberData.type << " ) )";
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
      // special handling for createInstance, as we need to explicitly place the forward declarations and the deleter classes here
      auto deleterTypesIt = vkData.deleterTypes.find("");
      assert((deleterTypesIt != vkData.deleterTypes.end()) && (deleterTypesIt->second.size() == 1));

      writeDeleterForwardDeclarations(ofs, *deleterTypesIt, vkData.deleterData);
      writeTypeCommand(ofs, "  ", vkData, commandData, false);
      writeDeleterClasses(ofs, *deleterTypesIt, vkData.deleterData);
    }
    else
    {
      writeTypeCommand(ofs, "  ", vkData, commandData, false);
    }
    writeTypeCommand(ofs, "  ", vkData, commandData, true);
    ofs << std::endl;
  }
}

void writeTypeCommand(std::ofstream & ofs, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool definition)
{
  enterProtect(ofs, commandData.protect);

  // first create the standard version of the function
  std::ostringstream standard;
  writeFunction(standard, indentation, vkData, commandData, definition, false, false, false);

  // then the enhanced version, composed by up to four parts
  std::ostringstream enhanced;
  writeFunction(enhanced, indentation, vkData, commandData, definition, true, false, false);

  // then a singular version, if a sized vector would be returned
  std::map<size_t, size_t>::const_iterator returnVector = commandData.vectorParams.find(commandData.returnParam);
  bool singular = (returnVector != commandData.vectorParams.end()) && (returnVector->second != ~0) && (commandData.params[returnVector->second].type.back() != '*');
  if (singular)
  {
    writeFunction(enhanced, indentation, vkData, commandData, definition, true, true, false);
  }

  // special handling for createDevice and createInstance !
  bool specialWriteUnique = (commandData.reducedName == "createDevice") || (commandData.reducedName == "createInstance");

  // and then the same for the Unique* versions (a Deleter is available for the commandData's class, and the function starts with 'allocate' or 'create')
  if (((vkData.deleterData.find(commandData.className) != vkData.deleterData.end()) || specialWriteUnique) && ((commandData.reducedName.substr(0, 8) == "allocate") || (commandData.reducedName.substr(0, 6) == "create")))
  {
    enhanced << "#ifndef VULKAN_HPP_NO_SMART_HANDLE" << std::endl;
    writeFunction(enhanced, indentation, vkData, commandData, definition, true, false, true);

    if (singular)
    {
      writeFunction(enhanced, indentation, vkData, commandData, definition, true, true, true);
    }
    enhanced << "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/" << std::endl;
  }

  // and write one or both of them
  writeStandardOrEnhanced(ofs, standard.str(), enhanced.str());
  leaveProtect(ofs, commandData.protect);
  ofs << std::endl;
}

void writeTypeEnum( std::ofstream & ofs, EnumData const& enumData )
{
  // a named enum per enum, listing all its values by setting them to the original Vulkan names
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
  // A Deleter class for each of the Unique* classes... but only if smart handles are not switched off
  ofs << "#ifndef VULKAN_HPP_NO_SMART_HANDLE" << std::endl;
  bool first = true;

  // get type and name of the parent (holder) type
  std::string parentType = deleterTypes.first;
  std::string parentName = parentType.empty() ? "" : startLowerCase(parentType);

  // iterate over the deleter types parented by this type
  for (auto const& deleterType : deleterTypes.second)
  {
    std::string deleterName = startLowerCase(deleterType);
    bool standardDeleter = !parentType.empty() && (deleterType != "Device");    // this detects the 'standard' case for a deleter

    if (!first)
    {
      ofs << std::endl;
    }
    first = false;

    ofs << "  class " << deleterType << "Deleter" << std::endl
      << "  {" << std::endl
      << "  public:" << std::endl
      << "    " << deleterType << "Deleter( ";
    if (standardDeleter)
    {
      // the standard deleter gets a parent type in the constructor
      ofs << parentType << " " << parentName << " = " << parentType << "(), ";
    }

    // if this Deleter is pooled, make such a pool the last argument, otherwise an Optional allocator
    auto const& dd = deleterData.find(deleterType);
    assert(dd != deleterData.end());
    std::string poolName = (dd->second.pool.empty() ? "" : startLowerCase(dd->second.pool));
    if (poolName.empty())
    {
      ofs << "Optional<const AllocationCallbacks> allocator = nullptr )" << std::endl;
    }
    else
    {
      assert(!dd->second.pool.empty());
      ofs << dd->second.pool << " " << poolName << " = " << dd->second.pool << "() )" << std::endl;
    }

    // now the initializer list of the Deleter constructor
    ofs << "      : ";
    if (standardDeleter)
    {
      // the standard deleter has a parent type as a member
      ofs << "m_" << parentName << "( " << parentName << " )" << std::endl
        << "      , ";
    }
    if (poolName.empty())
    {
      // non-pooled deleter have an allocator as a member
      ofs << "m_allocator( allocator )" << std::endl;
    }
    else
    {
      // pooled deleter have a pool as a member
      ofs << "m_" << poolName << "( " << poolName << " )" << std::endl;
    }

    // besides that, the constructor is empty
    ofs << "    {}" << std::endl
      << std::endl;

    // the operator() calls the delete/destroy function
    ofs << "    void operator()( " << deleterType << " " << deleterName << " )" << std::endl
      << "    {" << std::endl;

    // the delete/destroy function is either part of the parent member of the deleter argument
    if (standardDeleter)
    {
      ofs << "      m_" << parentName << ".";
    }
    else
    {
      ofs << "      " << deleterName << ".";
    }

    ofs << dd->second.call << "( ";

    if (!poolName.empty())
    {
      // pooled Deleter gets the pool as the first argument
      ofs << "m_" << poolName << ", ";
    }

    if (standardDeleter)
    {
      // the standard deleter gets the deleter argument as an argument
      ofs << deleterName;
    }

    // the non-pooled deleter get the allocate as an argument (potentially after the deleterName
    if (poolName.empty())
    {
      if (standardDeleter)
      {
        ofs << ", ";
      }
      ofs << "m_allocator";
    }
    ofs << " );" << std::endl
      << "    }" << std::endl
      << std::endl;

    // now the members of the Deleter class
    ofs << "  private:" << std::endl;
    if (standardDeleter)
    {
      // the parentType for the standard deleter
      ofs << "    " << parentType << " m_" << parentName << ";" << std::endl;
    }

    // the allocator for the non-pooled deleters, the pool for the pooled ones
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
  // if smart handles are supported, all the Deleter classes need to be forward declared
  ofs << "#ifndef VULKAN_HPP_NO_SMART_HANDLE" << std::endl;
  bool first = true;
  std::string firstName = deleterTypes.first.empty() ? "" : startLowerCase(deleterTypes.first);
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
  // the helper functions to make strings out of enum values
  enterProtect(ofs, enumData.protect);
  ofs << "  VULKAN_HPP_INLINE std::string to_string(" << enumData.name << (enumData.members.empty() ? ")" : " value)") << std::endl
      << "  {" << std::endl;
  if (enumData.members.empty())
  {
    // no enum values in this enum -> return "(void)"
    ofs << "    return \"(void)\";" << std::endl;
  }
  else
  {
    // otherwise switch over the value and return the a stringized version of that value (without leading 'e')
    ofs << "    switch (value)" << std::endl
        << "    {" << std::endl;
    for (auto const& member : enumData.members)
    {
      ofs << "    case " << enumData.name << "::" << member.name << ": return \"" << member.name.substr(1) << "\";" << std::endl;
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
  // the helper functions to make strings out of flag values
  enterProtect(ofs, enumData.protect);
  ofs << "  VULKAN_HPP_INLINE std::string to_string(" << flagsName << (enumData.members.empty() ? ")" : " value)") << std::endl
      << "  {" << std::endl;
  if (enumData.members.empty())
  {
    // no flags values in this enum -> return "{}"
    ofs << "    return \"{}\";" << std::endl;
  }
  else
  {
    ofs << "    if (!value) return \"{}\";" << std::endl
        << "    std::string result;" << std::endl;

    // 'or' together all the bits in the value
    for (auto itMember = enumData.members.begin(); itMember != enumData.members.end(); ++itMember)
    {
      ofs << "    if (value & " << enumData.name << "::" << itMember->name << ") result += \"" << itMember->name.substr(1) << " | \";" << std::endl;
    }
    // cut off the last three characters from the result (being " | ")
    ofs << "    return \"{\" + result.substr(0, result.size() - 3) + \"}\";" << std::endl;
  }
  ofs << "  }" << std::endl;
  leaveProtect(ofs, enumData.protect);
  ofs << std::endl;
}

void writeTypeFlags(std::ofstream & ofs, std::string const& flagsName, FlagData const& flagData, EnumData const& enumData)
{
  enterProtect(ofs, flagData.protect);
  // each Flags class is using on the class 'Flags' with the corresponding FlagBits enum as the template parameter
  ofs << "  using " << flagsName << " = Flags<" << enumData.name << ", Vk" << flagsName << ">;" << std::endl;

  if (!enumData.members.empty())
  {
    ofs << std::endl;

    // the operator| of two FlagBits into one Flags is mapped to the operator| of the Flags class
    ofs << "  VULKAN_HPP_INLINE " << flagsName << " operator|( " << enumData.name << " bit0, " << enumData.name << " bit1 )" << std::endl
      << "  {" << std::endl
      << "    return " << flagsName << "( bit0 ) | bit1;" << std::endl
      << "  }" << std::endl
      << std::endl;

    // the operator~ on a FlagBits is mapped to the operator~ of the Flags class
    ofs << "  VULKAN_HPP_INLINE " << flagsName << " operator~( " << enumData.name << " bits )" << std::endl
      << "  {" << std::endl
      << "    return ~( " << flagsName << "( bits ) );" << std::endl
      << "  }" << std::endl
      << std::endl;

    // the FlagTraits on FlagBits hold a value allFlags with all the defined bits set
    ofs << "  template <> struct FlagTraits<" << enumData.name << ">" << std::endl
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
  enterProtect(ofs, handleData.protect);

  // check if there are any forward dependenices for this handle -> list them first
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

  // then write any forward declaration of Deleters used by this handle
  std::map<std::string, std::set<std::string>>::const_iterator deleterTypesIt = vkData.deleterTypes.find(dependencyData.name);
  if (deleterTypesIt != vkData.deleterTypes.end())
  {
    writeDeleterForwardDeclarations(ofs, *deleterTypesIt, vkData.deleterData);
  }

  std::string memberName = startLowerCase(dependencyData.name);
  ofs << "  class " << dependencyData.name << std::endl
      << "  {" << std::endl
      << "  public:" << std::endl
      << "    " << dependencyData.name << "()" << std::endl
      << "      : m_" << memberName << "(VK_NULL_HANDLE)" << std::endl
      << "    {}" << std::endl
      << std::endl
      << "    " << dependencyData.name << "( std::nullptr_t )" << std::endl
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
      // assignment from std::nullptr_t
      << "    " << dependencyData.name << "& operator=( std::nullptr_t )" << std::endl
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

  // now list all the commands that are mapped to members of this class
  for (size_t i = 0; i < handleData.commands.size(); i++)
  {
    std::string commandName = handleData.commands[i];
    std::map<std::string, CommandData>::const_iterator cit = vkData.commands.find(commandName);
    assert((cit != vkData.commands.end()) && !cit->second.className.empty());
    writeTypeCommand(ofs, "    ", vkData, cit->second, false);
  }

  // finally list the cast operators to the corresponding Vk-type, and to bool (explicit), and the operator!
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
    << std::endl;

  // and last but not least: the member variable: a corresponding Vk-type
  ofs << "  private:" << std::endl
    << "    Vk" << dependencyData.name << " m_" << memberName << ";" << std::endl
    << "  };" << std::endl;

  // and a little safety check, that our class has the same size as the corresponding Vk-type
  ofs << "  static_assert( sizeof( " << dependencyData.name << " ) == sizeof( Vk" << dependencyData.name << " ), \"handle and wrapper have different size!\" );" << std::endl
    << std::endl;

  // then the actual Deleter classes can be listet
  deleterTypesIt = vkData.deleterTypes.find(dependencyData.name);
  if (deleterTypesIt != vkData.deleterTypes.end())
  {
    writeDeleterClasses(ofs, *deleterTypesIt, vkData.deleterData);
  }

  // and finally the commands, that are member functions of this handle
  for (size_t i = 0; i < handleData.commands.size(); i++)
  {
    std::string commandName = handleData.commands[i];
    std::map<std::string, CommandData>::const_iterator cit = vkData.commands.find(commandName);
    assert((cit != vkData.commands.end()) && !cit->second.className.empty());
    std::list<DependencyData>::const_iterator dep = std::find_if(dependencies.begin(), dependencies.end(), [commandName](DependencyData const& dd) { return dd.name == commandName; });
    assert(dep != dependencies.end() && (dep->name == cit->second.fullName));
    writeTypeCommand(ofs, "  ", vkData, cit->second, true);
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
  // a simple recursive check if a type is or contains a union
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
    // two structs are compared by comparing each of the elements
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

void writeTypeUnion( std::ofstream & ofs, VkData const& vkData, DependencyData const& dependencyData, std::map<std::string,std::string> const& defaultValues )
{
  std::map<std::string, StructData>::const_iterator it = vkData.structs.find(dependencyData.name);
  assert(it != vkData.structs.end());

  std::ostringstream oss;
  ofs << "  union " << dependencyData.name << std::endl
      << "  {" << std::endl;

  for ( size_t i=0 ; i<it->second.members.size() ; i++ )
  {
    // one constructor per union element
    ofs << "    " << dependencyData.name << "( ";
    if ( it->second.members[i].arraySize.empty() )
    {
      ofs << it->second.members[i].type << " ";
    }
    else
    {
      ofs << "const std::array<" << it->second.members[i].type << "," << it->second.members[i].arraySize << ">& ";
    }
    ofs << it->second.members[i].name << "_";

    // just the very first constructor gets default arguments
    if ( i == 0 )
    {
      std::map<std::string,std::string>::const_iterator defaultIt = defaultValues.find( it->second.members[i].pureType );
      assert(defaultIt != defaultValues.end() );
      if ( it->second.members[i].arraySize.empty() )
      {
        ofs << " = " << defaultIt->second;
      }
      else
      {
        ofs << " = { {" << defaultIt->second << "} }";
      }
    }
    ofs << " )" << std::endl
        << "    {" << std::endl
        << "      ";
    if ( it->second.members[i].arraySize.empty() )
    {
      ofs << it->second.members[i].name << " = " << it->second.members[i].name << "_";
    }
    else
    {
      ofs << "memcpy( &" << it->second.members[i].name << ", " << it->second.members[i].name << "_.data(), " << it->second.members[i].arraySize << " * sizeof( " << it->second.members[i].type << " ) )";
    }
    ofs << ";" << std::endl
        << "    }" << std::endl
        << std::endl;
    }

  for (size_t i = 0; i<it->second.members.size(); i++)
  {
    // one setter per union element
    assert(!it->second.returnedOnly);
    writeStructSetter(ofs, dependencyData.name, it->second.members[i], vkData.vkTypes);
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
  for (size_t i = 0; i < it->second.members.size() && !needsUnrestrictedUnions; i++)
  {
    needsUnrestrictedUnions = (vkData.vkTypes.find(it->second.members[i].type) != vkData.vkTypes.end());
  }
  if (needsUnrestrictedUnions)
  {
    ofs << "#ifdef VULKAN_HPP_HAS_UNRESTRICTED_UNIONS" << std::endl;
    for (size_t i = 0; i < it->second.members.size(); i++)
    {
      ofs << "    " << it->second.members[i].type << " " << it->second.members[i].name;
      if (!it->second.members[i].arraySize.empty())
      {
        ofs << "[" << it->second.members[i].arraySize << "]";
      }
      ofs << ";" << std::endl;
    }
    ofs << "#else" << std::endl;
  }
  for (size_t i = 0; i < it->second.members.size(); i++)
  {
    ofs << "    ";
    if (vkData.vkTypes.find(it->second.members[i].type) != vkData.vkTypes.end())
    {
      ofs << "Vk";
    }
    ofs << it->second.members[i].type << " " << it->second.members[i].name;
    if (!it->second.members[i].arraySize.empty())
    {
      ofs << "[" << it->second.members[i].arraySize << "]";
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
        writeTypeUnion( ofs, vkData, *it, defaultValues );
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
    vkData.handles[""];         // insert the default "handle" without class (for createInstance, and such)
    vkData.tags.insert("KHX");  // insert a non-listed tag

    for (tinyxml2::XMLElement * child = registryElement->FirstChildElement(); child; child = child->NextSiblingElement())
    {
      assert(child->Value());
      const std::string value = child->Value();
      if (value == "commands")
      {
        readCommands(child, vkData);
      }
      else if (value == "comment")
      {
        // get the vulkan license header and skip any leading spaces
        readComment(child, vkData.vulkanLicenseHeader);
        vkData.vulkanLicenseHeader.erase(vkData.vulkanLicenseHeader.begin(), std::find_if(vkData.vulkanLicenseHeader.begin(), vkData.vulkanLicenseHeader.end(), [](char c) { return !std::isspace(c); }));
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
    }

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
      << "#include <cstddef>" << std::endl
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

    // write all the to_string functions for enums and flags
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
