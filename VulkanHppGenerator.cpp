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
#include <regex>
#include <iterator>

#include <tinyxml2.h>

const std::string vkNamespace = R"(
#if !defined(VULKAN_HPP_NAMESPACE)
#define VULKAN_HPP_NAMESPACE vk
#endif

namespace VULKAN_HPP_NAMESPACE
{
)";

const std::string exceptionHeader = R"(
#if defined(_MSC_VER) && (_MSC_VER == 1800)
# define noexcept _NOEXCEPT
#endif

  class ErrorCategoryImpl : public std::error_category
  {
    public:
    virtual const char* name() const noexcept override { return "VULKAN_HPP_NAMESPACE::Result"; }
    virtual std::string message(int ev) const override { return to_string(static_cast<Result>(ev)); }
  };

#if defined(_MSC_VER) && (_MSC_VER == 1800)
# undef noexcept
#endif

  VULKAN_HPP_INLINE const std::error_category& errorCategory()
  {
    static ErrorCategoryImpl instance;
    return instance;
  }

  VULKAN_HPP_INLINE std::error_code make_error_code(Result e)
  {
    return std::error_code(static_cast<int>(e), errorCategory());
  }

  VULKAN_HPP_INLINE std::error_condition make_error_condition(Result e)
  {
    return std::error_condition(static_cast<int>(e), errorCategory());
  }
)";

const std::string exceptionClassesHeader = R"(
#if defined(_MSC_VER) && (_MSC_VER == 1800)
# define noexcept _NOEXCEPT
#endif

  class Error
  {
    public:
    virtual ~Error() = default;

    virtual const char* what() const noexcept = 0;
  };

  class LogicError : public Error, public std::logic_error
  {
    public:
    explicit LogicError( const std::string& what )
      : Error(), std::logic_error(what) {}
    explicit LogicError( char const * what )
      : Error(), std::logic_error(what) {}
    virtual ~LogicError() = default;

    virtual const char* what() const noexcept { return std::logic_error::what(); }
  };

  class SystemError : public Error, public std::system_error
  {
    public:
    SystemError( std::error_code ec )
      : Error(), std::system_error(ec) {}
    SystemError( std::error_code ec, std::string const& what )
      : Error(), std::system_error(ec, what) {}
    SystemError( std::error_code ec, char const * what )
      : Error(), std::system_error(ec, what) {}
    SystemError( int ev, std::error_category const& ecat )
      : Error(), std::system_error(ev, ecat) {}
    SystemError( int ev, std::error_category const& ecat, std::string const& what)
      : Error(), std::system_error(ev, ecat, what) {}
    SystemError( int ev, std::error_category const& ecat, char const * what)
      : Error(), std::system_error(ev, ecat, what) {}
    virtual ~SystemError() = default;

    virtual const char* what() const noexcept { return std::system_error::what(); }
  };

#if defined(_MSC_VER) && (_MSC_VER == 1800)
# undef noexcept
#endif

)";

const std::string flagsHeader = R"(
  template <typename FlagBitsType> struct FlagTraits
  {
    enum { allFlags = 0 };
  };

  template <typename BitType, typename MaskType = VkFlags>
  class Flags
  {
  public:
    Flags()
      : m_mask(0)
    {
    }

    Flags(BitType bit)
      : m_mask(static_cast<MaskType>(bit))
    {
    }

    Flags(Flags<BitType> const& rhs)
      : m_mask(rhs.m_mask)
    {
    }

    Flags<BitType> & operator=(Flags<BitType> const& rhs)
    {
      m_mask = rhs.m_mask;
      return *this;
    }

    Flags<BitType> & operator|=(Flags<BitType> const& rhs)
    {
      m_mask |= rhs.m_mask;
      return *this;
    }

    Flags<BitType> & operator&=(Flags<BitType> const& rhs)
    {
      m_mask &= rhs.m_mask;
      return *this;
    }

    Flags<BitType> & operator^=(Flags<BitType> const& rhs)
    {
      m_mask ^= rhs.m_mask;
      return *this;
    }

    Flags<BitType> operator|(Flags<BitType> const& rhs) const
    {
      Flags<BitType> result(*this);
      result |= rhs;
      return result;
    }

    Flags<BitType> operator&(Flags<BitType> const& rhs) const
    {
      Flags<BitType> result(*this);
      result &= rhs;
      return result;
    }

    Flags<BitType> operator^(Flags<BitType> const& rhs) const
    {
      Flags<BitType> result(*this);
      result ^= rhs;
      return result;
    }

    bool operator!() const
    {
      return !m_mask;
    }

    Flags<BitType> operator~() const
    {
      Flags<BitType> result(*this);
      result.m_mask ^= FlagTraits<BitType>::allFlags;
      return result;
    }

    bool operator==(Flags<BitType> const& rhs) const
    {
      return m_mask == rhs.m_mask;
    }

    bool operator!=(Flags<BitType> const& rhs) const
    {
      return m_mask != rhs.m_mask;
    }

    explicit operator bool() const
    {
      return !!m_mask;
    }

    explicit operator MaskType() const
    {
        return m_mask;
    }

  private:
    MaskType  m_mask;
  };

  template <typename BitType>
  Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags)
  {
    return flags | bit;
  }

  template <typename BitType>
  Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags)
  {
    return flags & bit;
  }

  template <typename BitType>
  Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags)
  {
    return flags ^ bit;
  }

)";

const std::string optionalClassHeader = R"(
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

const std::string arrayProxyHeader = R"(
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
  template <typename T>
  class ArrayProxy
  {
  public:
    ArrayProxy(std::nullptr_t)
      : m_count(0)
      , m_ptr(nullptr)
    {}

    ArrayProxy(T & ptr)
      : m_count(1)
      , m_ptr(&ptr)
    {}

    ArrayProxy(uint32_t count, T * ptr)
      : m_count(count)
      , m_ptr(ptr)
    {}

    template <size_t N>
    ArrayProxy(std::array<typename std::remove_const<T>::type, N> & data)
      : m_count(N)
      , m_ptr(data.data())
    {}

    template <size_t N>
    ArrayProxy(std::array<typename std::remove_const<T>::type, N> const& data)
      : m_count(N)
      , m_ptr(data.data())
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxy(std::vector<typename std::remove_const<T>::type, Allocator> & data)
      : m_count(static_cast<uint32_t>(data.size()))
      , m_ptr(data.data())
    {}

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    ArrayProxy(std::vector<typename std::remove_const<T>::type, Allocator> const& data)
      : m_count(static_cast<uint32_t>(data.size()))
      , m_ptr(data.data())
    {}

    ArrayProxy(std::initializer_list<T> const& data)
      : m_count(static_cast<uint32_t>(data.end() - data.begin()))
      , m_ptr(data.begin())
    {}

    const T * begin() const
    {
      return m_ptr;
    }

    const T * end() const
    {
      return m_ptr + m_count;
    }

    const T & front() const
    {
      assert(m_count && m_ptr);
      return *m_ptr;
    }

    const T & back() const
    {
      assert(m_count && m_ptr);
      return *(m_ptr + m_count - 1);
    }

    bool empty() const
    {
      return (m_count == 0);
    }

    uint32_t size() const
    {
      return m_count;
    }

    T * data() const
    {
      return m_ptr;
    }

  private:
    uint32_t  m_count;
    T *       m_ptr;
  };
#endif
)";

const std::string structureChainHeader = R"(
  template <typename X, typename Y> struct isStructureChainValid { enum { value = false }; };

  template <class Element>
  class StructureChainElement
  {
  public:
    explicit operator Element&() { return value; }
    explicit operator const Element&() const { return value; }
  private:
    Element value;
  };

  template<typename ...StructureElements>
  class StructureChain : private StructureChainElement<StructureElements>...
  {
  public:
    StructureChain()
    {
      link<StructureElements...>();  
    }

    StructureChain(StructureChain const &rhs)
    {
      linkAndCopy<StructureElements...>(rhs);
    }

    StructureChain& operator=(StructureChain const &rhs)
    {
      linkAndCopy<StructureElements...>(rhs);
      return *this;
    }

    template<typename ClassType> ClassType& get() { return static_cast<ClassType&>(*this);}

  private:
    template<typename X>
    void link()
    {
    }

    template<typename X, typename Y, typename ...Z>
    void link()
    {
      static_assert(isStructureChainValid<X,Y>::value, "The structure chain is not valid!");
      X& x = static_cast<X&>(*this);
      Y& y = static_cast<Y&>(*this);
      x.pNext = &y;
      link<Y, Z...>();
    }

    template<typename X>
    void linkAndCopy(StructureChain const &rhs)
    {
      static_cast<X&>(*this) = static_cast<X const &>(rhs);
    }

    template<typename X, typename Y, typename ...Z>
    void linkAndCopy(StructureChain const &rhs)
    {
      static_assert(isStructureChainValid<X,Y>::value, "The structure chain is not valid!");
      X& x = static_cast<X&>(*this);
      Y& y = static_cast<Y&>(*this);
      x = static_cast<X const &>(rhs);
      x.pNext = &y;
      linkAndCopy<Y, Z...>(rhs);
    }

};
)";

const std::string versionCheckHeader = R"(
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
)";

const std::string inlineHeader = R"(
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
)";

const std::string explicitHeader = R"(
#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
# define VULKAN_HPP_TYPESAFE_EXPLICIT
#else
# define VULKAN_HPP_TYPESAFE_EXPLICIT explicit
#endif
)";

const std::string resultValueHeader = R"(
  template <typename T>
  struct ResultValue
  {
    ResultValue( Result r, T & v )
      : result( r )
      , value( v )
    {}

    Result  result;
    T       value;

    operator std::tuple<Result&, T&>() { return std::tuple<Result&, T&>(result, value); }
  };

  template <typename T>
  struct ResultValueType
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    typedef ResultValue<T>  type;
#else
    typedef T              type;
#endif
  };

  template <>
  struct ResultValueType<void>
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    typedef Result type;
#else
    typedef void   type;
#endif
  };
)";

const std::string createResultValueHeader = R"(
  VULKAN_HPP_INLINE ResultValueType<void>::type createResultValue( Result result, char const * message )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    assert( result == Result::eSuccess );
    return result;
#else
    if ( result != Result::eSuccess )
    {
      throwResultException( result, message );
    }
#endif
  }

  template <typename T>
  VULKAN_HPP_INLINE typename ResultValueType<T>::type createResultValue( Result result, T & data, char const * message )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    assert( result == Result::eSuccess );
    return ResultValue<T>( result, data );
#else
    if ( result != Result::eSuccess )
    {
      throwResultException( result, message );
    }
    return data;
#endif
  }

  VULKAN_HPP_INLINE Result createResultValue( Result result, char const * message, std::initializer_list<Result> successCodes )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    assert( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#else
    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
    {
      throwResultException( result, message );
    }
#endif
    return result;
  }

  template <typename T>
  VULKAN_HPP_INLINE ResultValue<T> createResultValue( Result result, T & data, char const * message, std::initializer_list<Result> successCodes )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    assert( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#else
    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
    {
      throwResultException( result, message );
    }
#endif
    return ResultValue<T>( result, data );
  }

)";

const std::string uniqueHandleHeader = R"(
#ifndef VULKAN_HPP_NO_SMART_HANDLE

  template <typename Type> class UniqueHandleTraits;

  template <typename Type>
  class UniqueHandle : public UniqueHandleTraits<Type>::deleter
  {
  private:
    using Deleter = typename UniqueHandleTraits<Type>::deleter;
  public:
    explicit UniqueHandle( Type const& value = Type(), Deleter const& deleter = Deleter() )
      : Deleter( deleter)
      , m_value( value )
    {}

    UniqueHandle( UniqueHandle const& ) = delete;

    UniqueHandle( UniqueHandle && other )
      : Deleter( std::move( static_cast<Deleter&>( other ) ) )
      , m_value( other.release() )
    {}

    ~UniqueHandle()
    {
      this->destroy( m_value );
    }

    UniqueHandle & operator=( UniqueHandle const& ) = delete;

    UniqueHandle & operator=( UniqueHandle && other )
    {
      reset( other.release() );
      *static_cast<Deleter*>(this) = std::move( static_cast<Deleter&>(other) );
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

    Type * operator->()
    {
      return &m_value;
    }

    Type const& operator*() const
    {
      return m_value;
    }

    Type & operator*()
    {
      return m_value;
    }

    const Type & get() const
    {
      return m_value;
    }
    
    Type & get()
    {
      return m_value;
    }

    void reset( Type const& value = Type() )
    {
      if ( m_value != value )
      {
        this->destroy( m_value );
        m_value = value;
      }
    }

    Type release()
    {
      Type value = m_value;
      m_value = nullptr;
      return value;
    }

    void swap( UniqueHandle<Type> & rhs )
    {
      std::swap(m_value, rhs.m_value);
      std::swap(static_cast<Deleter&>(*this), static_cast<Deleter&>(rhs));
    }

  private:
    Type    m_value;
  };

  template <typename Type>
  VULKAN_HPP_INLINE void swap( UniqueHandle<Type> & lhs, UniqueHandle<Type> & rhs )
  {
    lhs.swap( rhs );
  }
#endif

)";

std::string replaceWithMap(std::string const &input, std::map<std::string, std::string> replacements)
{
  // This will match ${someVariable} and contain someVariable in match group 1
  std::regex re(R"(\$\{([^\}]+)\})");
  auto it = std::sregex_iterator(input.begin(), input.end(), re);
  auto end = std::sregex_iterator();

  // No match, just return the original string
  if (it == end)
  {
    return input;
  }

  std::string result = "";
  while (it != end)
  {
    std::smatch match = *it;
    auto itReplacement = replacements.find(match[1].str());
    assert(itReplacement != replacements.end());

    result += match.prefix().str() + ((itReplacement != replacements.end()) ? itReplacement->second : match[0].str());
    ++it;

    // we've passed the last match. Append the rest of the orignal string
    if (it == end)
    {
      result += match.suffix().str();
    }
  }
  return result;
}

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

struct EnumValueData
{
  std::string name;
  std::string value;
  std::string alias;
};

struct EnumData
{
  EnumData(std::string const& n, bool b = false)
    : name(n)
    , bitmask(b)
  {}

  void addEnumValue(std::string const& name, std::string const& tag, std::map<std::string, std::string> & nameMap);

  std::string                 name;
  std::string                 prefix;
  std::string                 postfix;
  std::vector<EnumValueData>  values;
  std::string                 protect;
  bool                        bitmask;
  std::string                 alias;
};

struct FlagData
{
  std::string protect;
  std::string alias;
};

struct HandleData
{
  std::vector<std::string>  commands;
  std::string               protect;
  std::string               alias;
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
  std::string values;
};

struct StructData
{
  StructData()
    : returnedOnly(false)
  {}

  bool                      returnedOnly;
  bool                      isUnion;
  std::vector<MemberData>   members;
  std::string               protect;
  std::vector<std::string>  structExtends;
  std::string               alias;
};

struct DeleterData
{
  std::string pool;
  std::string call;
};

#if !defined(NDEBUG)
struct ExtensionData
{
  std::string               protect;
  std::vector<std::string>  requires;
};

struct VendorIDData
{
  std::string name;
  std::string id;
  std::string comment;
};
#endif

struct VkData
{
  std::map<std::string, CommandData>            commands;
  std::map<std::string, std::string>            constants;
  std::set<std::string>                         defines;
  std::list<DependencyData>                     dependencies;
  std::map<std::string, DeleterData>            deleters;     // map from child types to corresponding deleter data
  std::map<std::string, std::set<std::string>>  deleterTypes; // map from parent type to set of child types
  std::map<std::string, EnumData>               enums;
  std::map<std::string, FlagData>               flags;
  std::map<std::string, HandleData>             handles;
  std::map<std::string, std::string>            nameMap;
  std::map<std::string, ScalarData>             scalars;
  std::map<std::string, StructData>             structs;
  std::set<std::string>                         extendedStructs; // structs which are referenced by the structextends tag
  std::set<std::string>                         tags;
  std::string                                   typesafeCheck;
  std::string                                   version;
  std::set<std::string>                         vkTypes;
  std::string                                   vulkanLicenseHeader;
#if !defined(NDEBUG)
  std::map<std::string, ExtensionData>          extensions;
  std::vector<VendorIDData>                     vendorIDs;
#endif
};

void aliasEnum(VkData & vkData, std::map<std::string, EnumData>::iterator enumsIt, std::string const& newName, std::string const& protect);
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
void readEnumsConstant(tinyxml2::XMLElement * element, std::map<std::string, std::string> & constants);
void readEnumsEnum( tinyxml2::XMLElement * element, EnumData & enumData, std::map<std::string, std::string> & nameMap);
void readDisabledExtensionRequire(tinyxml2::XMLElement * element, VkData & vkData);
void readExtensionAlias(tinyxml2::XMLElement * element, VkData & vkData, std::string const& protect, std::string const& tag);
void readExtensionCommand(tinyxml2::XMLElement * element, std::map<std::string, CommandData> & commands, std::string const& protect);
void readExtensionEnum(tinyxml2::XMLElement * element, std::map<std::string, EnumData> & enums, std::string const& tag, std::map<std::string, std::string> & nameMap);
void readExtensionRequire(tinyxml2::XMLElement * element, VkData & vkData, std::string const& protect, std::string const& tag);
void readExtensions( tinyxml2::XMLElement * element, VkData & vkData );
void readExtensionsExtension(tinyxml2::XMLElement * element, VkData & vkData);
void readExtensionType(tinyxml2::XMLElement * element, VkData & vkData, std::string const& protect);
void readFeature(tinyxml2::XMLElement * element, std::map<std::string, EnumData> & enums, std::map<std::string, std::string> & nameMap);
void readFeatureRequire(tinyxml2::XMLElement * element, std::map<std::string, EnumData> & enums, std::map<std::string, std::string> & nameMap);
void readFeatureRequireEnum(tinyxml2::XMLElement * element, std::map<std::string, EnumData> & enums, std::map<std::string, std::string> & nameMap);
tinyxml2::XMLNode* readType(tinyxml2::XMLNode* element, std::string & type, std::string & pureType);
void readTypeBasetype( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies );
void readTypeBitmask( tinyxml2::XMLElement * element, VkData & vkData);
void readTypeDefine( tinyxml2::XMLElement * element, VkData & vkData );
void readTypeFuncpointer( tinyxml2::XMLElement * element, std::list<DependencyData> & dependencies );
void readTypeHandle(tinyxml2::XMLElement * element, VkData & vkData);
void readTypeStruct( tinyxml2::XMLElement * element, VkData & vkData, bool isUnion );
void readTypeStructMember( tinyxml2::XMLElement * element, VkData & vkData, StructData & structData );
void readTag(tinyxml2::XMLElement * element, std::set<std::string> & tags);
void readTags(tinyxml2::XMLElement * element, std::set<std::string> & tags);
void readType(tinyxml2::XMLElement * element, VkData & vkData);
void readTypeName(tinyxml2::XMLElement * element, std::map<std::string, std::string> const& attributes, std::list<DependencyData> & dependencies);
void readTypes(tinyxml2::XMLElement * element, VkData & vkData);
std::string reduceName(std::string const& name, bool singular = false);
void registerDeleter(VkData & vkData, CommandData const& commandData);
std::string startLowerCase(std::string const& input);
std::string startUpperCase(std::string const& input);
void sortDependencies( std::list<DependencyData> & dependencies );
std::string strip(std::string const& value, std::string const& prefix, std::string const& postfix = std::string());
std::string stripPluralS(std::string const& name);
std::string toCamelCase(std::string const& value);
std::vector<std::string> tokenize(std::string tokenString, char separator);
std::string toUpperCase(std::string const& name);
std::string trim(std::string const& input);
std::string trimEnd(std::string const& input);
void writeCall(std::ostream & os, CommandData const& commandData, std::set<std::string> const& vkTypes, bool firstCall, bool singular);
std::string generateCall(CommandData const& commandData, std::set<std::string> const& vkTypes, bool firstCall, bool singular);
void writeCallCountParameter(std::ostream & os, CommandData const& commandData, bool singular, std::map<size_t, size_t>::const_iterator it);
void writeCallParameter(std::ostream & os, ParamData const& paramData, std::set<std::string> const& vkTypes);
void writeCallPlainTypeParameter(std::ostream & os, ParamData const& paramData);
void writeCallVectorParameter(std::ostream & os, CommandData const& commandData, std::set<std::string> const& vkTypes, bool firstCall, bool singular, std::map<size_t, size_t>::const_iterator it);
void writeCallVulkanTypeParameter(std::ostream & os, ParamData const& paramData);
void writeDeleterClasses(std::ostream & os, std::pair<std::string, std::set<std::string>> const& deleterTypes, std::map<std::string, DeleterData> const& deleters);
void writeDeleterForwardDeclarations(std::ostream &os, std::pair<std::string, std::set<std::string>> const& deleterTypes, std::map<std::string, DeleterData> const& deleters);
void writeEnumsToString(std::ostream & os, EnumData const& enumData);
void writeFlagsToString(std::ostream & os, std::string const& flagsName, EnumData const &enumData);
void writeFunction(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool definition, bool enhanced, bool singular, bool unique, bool isStructureChain);
void writeFunctionBodyEnhanced(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool singular, bool isStructureChain);
void writeFunctionBodyEnhancedCall(std::ostream &os, std::string const& indentation, std::set<std::string> const& vkTypes, CommandData const& commandData, bool singular);
void writeFunctionBodyEnhancedCallResult(std::ostream &os, std::string const& indentation, std::set<std::string> const& vkTypes, CommandData const& commandData, bool singular);
void writeFunctionBodyEnhancedCallTwoStep(std::ostream & os, std::string const& indentation, std::set<std::string> const& vkTypes, std::string const& returnName, std::string const& sizeName, CommandData const& commandData);
void writeFunctionBodyEnhancedCallTwoStepChecked(std::ostream & os, std::string const& indentation, std::set<std::string> const& vkTypes, std::string const& returnName, std::string const& sizeName, CommandData const& commandData);
void writeFunctionBodyEnhancedCallTwoStepIterate(std::ostream & os, std::string const& indentation, std::set<std::string> const& vkTypes, std::string const& returnName, std::string const& sizeName, CommandData const& commandData);
void writeFunctionBodyEnhancedLocalCountVariable(std::ostream & os, std::string const& indentation, CommandData const& commandData);
std::string writeFunctionBodyEnhancedLocalReturnVariable(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool singular, bool isStructureChain);
void writeFunctionBodyEnhancedMultiVectorSizeCheck(std::ostream & os, std::string const& indentation, CommandData const& commandData);
void writeFunctionBodyEnhancedReturnResultValue(std::ostream & os, std::string const& indentation, std::string const& returnName, CommandData const& commandData, bool singular);
void writeFunctionBodyStandard(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData);
void writeFunctionBodyUnique(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool singular);
void writeFunctionHeaderArguments(std::ostream & os, VkData const& vkData, CommandData const& commandData, bool enhanced, bool singular, bool withDefaults);
void writeFunctionHeaderArgumentsEnhanced(std::ostream & os, VkData const& vkData, CommandData const& commandData, bool singular, bool withDefaults);
void writeFunctionHeaderArgumentsStandard(std::ostream & os, CommandData const& commandData);
void writeFunctionHeaderName(std::ostream & os, std::string const& name, bool singular, bool unique);
void writeFunctionHeaderReturnType(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool enhanced, bool singular, bool unique, bool isStructureChain);
void writeFunctionHeaderTemplate(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool withDefault, bool isStructureChain);
void writeReinterpretCast(std::ostream & os, bool leadingConst, bool vulkanType, std::string const& type, bool trailingPointerToConst);
void writeStandardOrEnhanced(std::ostream & os, std::string const& standard, std::string const& enhanced);
void writeStructConstructor( std::ostream & os, std::string const& name, StructData const& structData, std::set<std::string> const& vkTypes, std::map<std::string, std::string> const& nameMap, std::map<std::string,std::string> const& defaultValues );
void writeStructSetter( std::ostream & os, std::string const& structureName, MemberData const& memberData, std::set<std::string> const& vkTypes, std::map<std::string,StructData> const& structs );
void writeTypeCommand(std::ostream & os, VkData const& vkData, DependencyData const& dependencyData);
void writeTypeCommand(std::ostream &os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool definition);
void writeTypeEnum(std::ostream & os, EnumData const& enumData);
bool isErrorEnum(std::string const& enumName);
std::string stripErrorEnumPrefix(std::string const& enumName);
void writeExceptionsForEnum(std::ostream & os, EnumData const& enumData);
void writeThrowExceptions(std::ostream& os, EnumData const& enumData);
void writeTypeFlags(std::ostream & os, std::string const& flagsName, FlagData const& flagData, EnumData const& enumData);
void writeTypeHandle(std::ostream & os, VkData const& vkData, DependencyData const& dependencyData, HandleData const& handle, std::list<DependencyData> const& dependencies);
void writeTypeScalar( std::ostream & os, DependencyData const& dependencyData );
void writeTypeStruct( std::ostream & os, VkData const& vkData, DependencyData const& dependencyData, std::map<std::string,std::string> const& defaultValues );
void writeTypeUnion( std::ostream & os, VkData const& vkData, DependencyData const& dependencyData, std::map<std::string,std::string> const& defaultValues );
void writeTypes(std::ostream & os, VkData const& vkData, std::map<std::string, std::string> const& defaultValues);
void writeVersionCheck(std::ostream & os, std::string const& version);
void writeTypesafeCheck(std::ostream & os, std::string const& typesafeCheck);
#if !defined(NDEBUG)
void skipTypeInclude(tinyxml2::XMLElement * element, std::map<std::string, std::string> const& attributes);
void skipVendorID(tinyxml2::XMLElement * element, std::vector<VendorIDData> & vendorIDs);
void skipVendorIDs(tinyxml2::XMLElement * element, std::vector<VendorIDData> & vendorIDs);
#endif

std::string createEnumValueName(std::string const& name, std::string const& prefix, std::string const& postfix, bool bitmask, std::string const& tag)
{
  std::string result = "e" + toCamelCase(strip(name, prefix, postfix));
  if (bitmask)
  {
    size_t pos = result.find("Bit");
    if (pos != std::string::npos)
    {
      result.erase(pos, 3);
    }
  }
  if (!tag.empty() && (result.substr(result.length() - tag.length()) == toCamelCase(tag)))
  {
    result = result.substr(0, result.length() - tag.length()) + tag;
  }
  return result;
}

void EnumData::addEnumValue(std::string const &name, std::string const& tag, std::map<std::string, std::string> & nameMap)
{
  EnumValueData evd;
  evd.name = createEnumValueName(name, prefix, postfix, bitmask, tag);
  evd.value = name;

  auto it = std::find_if(values.begin(), values.end(), [&evd](EnumValueData const& _evd) { return _evd.name == evd.name; });
  if (it == values.end())
  {
    values.push_back(evd);
    assert(nameMap.find(name) == nameMap.end());
    nameMap[name] = this->name + "::" + evd.name;
  }
  else
  {
    assert(it->value == evd.value);
  }
}

void aliasEnum(VkData & vkData, std::map<std::string, EnumData>::iterator enumsIt, std::string const& newName, std::string const& protect)
{
  assert(vkData.enums.find(newName) == vkData.enums.end());
  assert(enumsIt->second.protect == protect);
  assert(enumsIt->second.alias.empty());
  enumsIt->second.alias = newName;
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
          if (!enumData.values.empty())
          {
            defaultValues[dependency.name] = dependency.name + "::" + vkData.enums.find(dependency.name)->second.values.front().name;
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
      std::string value = trimEnd(node->Value());
      if (value == "[")
      {
        // if this node has '[' as its value, the next node holds the array size, and the node after that needs to hold ']', and there should be no more siblings
        node = node->NextSibling();
        assert(node && node->ToElement() && (strcmp(node->Value(), "enum") == 0));
        arraySize = node->ToElement()->GetText();
        node = node->NextSibling();
        assert(node && node->ToText() && (trimEnd(node->Value()) == "]"));
      }
      else
      {
        // otherwise, the node holds '[' and ']', so get the stuff in between those as the array size
        assert((value.front() == '[') && (value.back() == ']'));
        arraySize = value.substr(1, value.length() - 2);
      }
      assert(!node->NextSibling() || ((strcmp(node->NextSibling()->Value(), "comment") == 0) && !node->NextSibling()->NextSibling()));
    }
  }
  return arraySize;
}

bool readCommandParam( tinyxml2::XMLElement * element, std::set<std::string> & dependencies, std::vector<ParamData> & params )
{
  ParamData param;

  bool isTwoStep = false;
  if (element->Attribute("len"))
  {
    param.len = element->Attribute("len");
    auto pit = std::find_if(params.begin(), params.end(), [&param](ParamData const& pd) { return param.len == pd.name; });
    if (pit != params.end())
    {
      isTwoStep = (pit->type.find('*') != std::string::npos);
    }
  }

  // get the type of the parameter, and put it into the list of dependencies
  tinyxml2::XMLNode * child = readCommandParamType(element->FirstChild(), param);
  dependencies.insert(param.pureType);

  assert( child->ToElement() && ( strcmp( child->Value(), "name" ) == 0 ) );
  param.name = child->ToElement()->GetText();

  param.arraySize = readArraySize(child, param.name);

  param.optional = element->Attribute("optional") && (strcmp(element->Attribute("optional"), "true") == 0);

  params.push_back(param);

  assert(!isTwoStep || (param.type.substr(0, 6) != "const "));
  return isTwoStep;
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
    std::string value = trim(node->Value());
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

    results = tokenize(successCodes, ',');
    for (auto & code : results)
    {
      std::string tag = findTag(code, tags);
      // on each success code: prepend 'e', strip "VK_" and a tag, convert it to camel case, and add the tag again
      code = std::string("e") + toCamelCase(strip(code, "VK_", tag)) + tag;
    }
  }
  return results;
}

void readComment(tinyxml2::XMLElement * element, std::string & header)
{
  assert(!element->FirstAttribute() && !element->FirstChildElement());
  assert(element->GetText());

  std::string text = element->GetText();
  if (text.find("\nCopyright") == 0)
  {
    assert(header.empty());
    header = text;

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
}

void readEnums( tinyxml2::XMLElement * element, VkData & vkData )
{
  if (!element->Attribute("name"))
  {
    throw std::runtime_error(std::string("spec error: enums element is missing the name attribute"));
  }
  std::string name = strip(element->Attribute("name"), "Vk");

  if (name == "API Constants")
  {
    for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
    {
      readEnumsConstant(child, vkData.constants);
    }
  }
  else
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

    readEnumsEnum( element, it->second, vkData.nameMap );

    // add this enum to the set of Vulkan data types
    assert( vkData.vkTypes.find( name ) == vkData.vkTypes.end() );
    vkData.vkTypes.insert( name );
  }
}

void readEnumsConstant(tinyxml2::XMLElement * element, std::map<std::string, std::string> & constants)
{
  assert((strcmp(element->Value(), "enum") == 0) && element->Attribute("value") && element->Attribute("name"));
  std::string name = element->Attribute("name");
  assert(constants.find(name) == constants.end());
  constants[name] = element->Attribute("value");
}

void readEnumsEnum( tinyxml2::XMLElement * element, EnumData & enumData, std::map<std::string, std::string> & nameMap)
{
  // read the names of the enum values
  tinyxml2::XMLElement * child = element->FirstChildElement();
  while (child)
  {
    if ( child->Attribute( "name" ) )
    {
      enumData.addEnumValue(child->Attribute("name"), "", nameMap);
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

void readExtensionAlias(tinyxml2::XMLElement * element, VkData & vkData, std::string const& protect, std::string const& tag)
{
  assert(element->Attribute("name") && element->Attribute("value"));
  std::string name = element->Attribute("name");
  std::string value = element->Attribute("value");

  auto commandsIt = vkData.commands.find(startLowerCase(strip(value, "vk")));
  if (commandsIt != vkData.commands.end())
  {
    // the alias is on a command -> add a command with that name
    CommandData commandData = commandsIt->second;
    commandData.fullName = startLowerCase(strip(name, "vk"));
    assert(vkData.commands.find(commandData.fullName) == vkData.commands.end());

    determineReducedName(commandData);
    vkData.commands.insert(std::make_pair(commandData.fullName, commandData));
  }
  else
  {
    auto constantsIt = vkData.constants.find(value);
    if (constantsIt != vkData.constants.end())
    {
      // alias on a constant -> just add it to the set of constants... we're doing nothing with them
      auto it = vkData.constants.find(name);
      assert((vkData.constants.find(name) == vkData.constants.end()) || (vkData.constants.find(name)->second == constantsIt->second));
      vkData.constants[name] = constantsIt->second;
    }
    else
    {
      std::string strippedValue = strip(value, "Vk");
      std::string strippedName = strip(name, "Vk");

      auto enumsIt = vkData.enums.find(strippedValue);
      if (enumsIt != vkData.enums.end())
      {
        // the alias is on an enum -> set the alias, which will map to a using directive
        aliasEnum(vkData, enumsIt, strippedName, protect);
      }
      else
      {
        auto flagsIt = vkData.flags.find(strippedValue);
        if (flagsIt != vkData.flags.end())
        {
          // the alias is on a flags -> set the alias, which will map to a using directive
          assert(vkData.flags.find(strippedName) == vkData.flags.end());
          assert(flagsIt->second.alias.empty());
          flagsIt->second.alias = strippedName;

          // adjust the generated enum name as well, if it's empty (and therefore auto-generated)
          std::string enumName = generateEnumNameForFlags(strippedValue);
          std::map<std::string, EnumData>::iterator enumsIt = vkData.enums.find(enumName);
          assert(enumsIt != vkData.enums.end());
          if (enumsIt->second.values.empty())
          {
            aliasEnum(vkData, enumsIt, generateEnumNameForFlags(flagsIt->second.alias), protect);
          }
        }
        else
        {
          auto handlesIt = vkData.handles.find(strippedValue);
          if (handlesIt != vkData.handles.end())
          {
            assert(vkData.handles.find(strippedName) == vkData.handles.end());
            assert(handlesIt->second.protect == protect);
            assert(handlesIt->second.alias.empty());
            handlesIt->second.alias = strippedName;
          }
          else
          {
            auto structsIt = vkData.structs.find(strippedValue);
            if (structsIt != vkData.structs.end())
            {
              // the alias is on a structure -> set the alias, which will map to a using directive
              assert(vkData.structs.find(strippedName) == vkData.structs.end());
              assert(structsIt->second.protect == protect);
              assert(structsIt->second.alias.empty() || (structsIt->second.alias == strippedName));
              structsIt->second.alias = strippedName;
            }
            else
            {
              // final catch: it has to be an enum value
              bool found = false;
              for (auto & e : vkData.enums)
              {
                auto valueIt = std::find_if(e.second.values.begin(), e.second.values.end(), [&value](EnumValueData const& evd) { return evd.value == value; });
                if (valueIt != e.second.values.end())
                {
                  assert(std::find_if(e.second.values.begin(), e.second.values.end(), [&name](EnumValueData const& evd) {return evd.value == name; }) == e.second.values.end());
                  assert(valueIt->alias.empty());
                  valueIt->alias = createEnumValueName(name, e.second.prefix, e.second.postfix, e.second.bitmask, tag);
                  found = true;
                  break;
                }
              }
              assert(found);
            }
          }
        }
      }
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

void readExtensionEnum(tinyxml2::XMLElement * element, std::map<std::string, EnumData> & enums, std::string const& tag, std::map<std::string, std::string> & nameMap)
{
  // TODO process enums which don't extend existing enums
  if (element->Attribute("extends"))
  {
    assert(element->Attribute("name"));
    std::string extends = strip(element->Attribute("extends"), "Vk");
    assert(enums.find(extends) != enums.end());
    assert(!!element->Attribute("bitpos") + !!element->Attribute("offset") + !!element->Attribute("value") == 1);
    auto enumIt = enums.find(extends);
    assert(enumIt != enums.end());
    enumIt->second.addEnumValue(element->Attribute("name"), tag, nameMap);
  }
}

void readExtensionRequire(tinyxml2::XMLElement * element, VkData & vkData, std::string const& protect, std::string const& tag)
{
  for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    std::string value = child->Value();

    if (value == "alias")
    {
      readExtensionAlias(child, vkData, protect, tag);
    }
    else if ( value == "command" )
    {
      readExtensionCommand(child, vkData.commands, protect);
    }
    else if (value == "type")
    {
      readExtensionType(child, vkData, protect);
    }
    else if ( value == "enum")
    {
      readExtensionEnum(child, vkData.enums, tag, vkData.nameMap);
    }
    else
    {
      assert((value == "comment") || (value=="usage"));
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
  if (strcmp(element->Attribute("supported"), "disabled") == 0)
  {
    // kick out all the disabled stuff we've read before !!
    for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
    {
      assert(strcmp(child->Value(), "require") == 0);
      readDisabledExtensionRequire(child, vkData);
    }
  }
  else
  {
    assert( element->Attribute( "name" ) );
    std::string name = element->Attribute("name");

    std::string tag = extractTag(name);
    assert(vkData.tags.find(tag) != vkData.tags.end());

    std::string protect;
    if (element->Attribute("protect"))
    {
      protect = element->Attribute("protect");
    }
    else if (element->Attribute("platform"))
    {
      assert(element->Attribute("author"));
      protect = "VK_USE_PLATFORM_" + toUpperCase(element->Attribute("platform")) + "_" + element->Attribute("author");
    }

#if !defined(NDEBUG)
    assert(vkData.extensions.find(name) == vkData.extensions.end());
    ExtensionData & extension = vkData.extensions.insert(std::make_pair(name, ExtensionData())).first->second;
    extension.protect = protect;
    if (element->Attribute("requires"))
    {
      extension.requires = tokenize(element->Attribute("requires"), ',');
    }
#endif

    for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
    {
      assert(strcmp(child->Value(), "require") == 0);
      readExtensionRequire(child, vkData, protect, tag);
    }
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
        if (eit->second.values.empty())
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
            if (stit != vkData.structs.end())
            {
              stit->second.protect = protect;
            }
            else
            {
              assert(vkData.defines.find(name) != vkData.defines.end());
            }
          }
        }
      }
    }
  }
}

void readFeature(tinyxml2::XMLElement * element, std::map<std::string, EnumData> & enums, std::map<std::string, std::string> & nameMap)
{
  assert(element->Attribute("api") && (strcmp(element->Attribute("api"), "vulkan") == 0));

  for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    assert(child->Value());
    std::string value = child->Value();
    assert(value == "require");
    readFeatureRequire(child, enums, nameMap);
  }
}

void readFeatureRequire(tinyxml2::XMLElement * element, std::map<std::string, EnumData> & enums, std::map<std::string, std::string> & nameMap)
{
  for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    assert(child->Value());
    std::string value = child->Value();
    if (value == "enum")
    {
      readFeatureRequireEnum(child, enums, nameMap);
    }
    else
    {
      assert((value == "command") || (value == "type"));
    }
  }
}

void readFeatureRequireEnum(tinyxml2::XMLElement * element, std::map<std::string, EnumData> & enums, std::map<std::string, std::string> & nameMap)
{
  if (element->Attribute("extends"))
  {
    assert((strncmp(element->Attribute("extends"), "Vk", 2) == 0) && element->Attribute("name"));
    std::string extends = strip(element->Attribute("extends"), "Vk");
    auto enumIt = enums.find(extends);
    assert(enumIt != enums.end());
    enumIt->second.addEnumValue(element->Attribute("name"), "", nameMap);
  }
}

tinyxml2::XMLNode* readType(tinyxml2::XMLNode* element, std::string & type, std::string & pureType)
{
  assert(element);
  if (element->ToText())
  {
    std::string value = trim(element->Value());
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
  else if (element->GetText() && (trim(element->GetText()) == "struct"))
  {
    assert(child && (strcmp(child->Value(), "name") == 0) && child->GetText());
    vkData.defines.insert(child->GetText());
    vkData.dependencies.push_back(DependencyData(DependencyData::Category::REQUIRED, child->GetText()));
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

  vkData.dependencies.push_back( DependencyData( isUnion ? DependencyData::Category::UNION : DependencyData::Category::STRUCT, name ) );

  assert( vkData.structs.find( name ) == vkData.structs.end() );
  std::map<std::string,StructData>::iterator it = vkData.structs.insert( std::make_pair( name, StructData() ) ).first;
  it->second.returnedOnly = !!element->Attribute( "returnedonly" );
  it->second.isUnion = isUnion;

  if (element->Attribute("structextends"))
  {
    std::vector<std::string> structExtends = tokenize(element->Attribute("structextends"), ',');
    for (auto const& s : structExtends)
    {
      assert(s.substr(0, 2) == "Vk");
      std::string strippedName = s.substr(2);
      it->second.structExtends.push_back(strippedName);
      vkData.extendedStructs.insert(strippedName);
    }
    assert(!it->second.structExtends.empty());
  }

  for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    assert( child->Value() );
    std::string value = child->Value();
    assert((value == "comment") || (value == "member"));
    if (value == "member")
    {
      readTypeStructMember(child, vkData, it->second);
    }
  }

  assert( vkData.vkTypes.find( name ) == vkData.vkTypes.end() );
  vkData.vkTypes.insert( name );
}

void readTypeStructMember(tinyxml2::XMLElement * element, VkData & vkData, StructData & structData)
{
  structData.members.push_back(MemberData());
  MemberData & member = structData.members.back();

  char const* values = element->Attribute("values");
  if (values)
  {
    member.values = values;
  }

  tinyxml2::XMLNode* child = readType(element->FirstChild(), member.type, member.pureType);
  vkData.dependencies.back().dependencies.insert(member.pureType);

  assert((child->ToElement() && strcmp(child->Value(), "name") == 0));
  member.name = child->ToElement()->GetText();

  member.arraySize = readArraySize(child, member.name);
}

void readTag(tinyxml2::XMLElement * element, std::set<std::string> & tags)
{
  for (auto attribute = element->FirstAttribute(); attribute; attribute = attribute->Next())
  {
    std::string name = attribute->Name();
    if (name == "name")
    {
      std::string value = attribute->Value();
      tags.insert(value);
    }
    else
    {
      assert((name == "author") || (name == "contact"));
    }
  }

  assert(!element->FirstChildElement());
}

void readTags(tinyxml2::XMLElement * element, std::set<std::string> & tags)
{
  assert(element->FirstAttribute() && !element->FirstAttribute()->Next());
  assert(strcmp(element->FirstAttribute()->Name(), "comment") == 0);

  for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    std::string value = child->Value();
    assert(value == "tag");
    readTag(child, tags);
  }
}

void readType(tinyxml2::XMLElement * element, VkData & vkData)
{
  std::map<std::string, std::string> attributes;
  for (auto attribute = element->FirstAttribute(); attribute; attribute = attribute->Next())
  {
    assert(attributes.find(attribute->Name()) == attributes.end());
    attributes[attribute->Name()] = attribute->Value();
  }

  auto categoryIt = attributes.find("category");
  if (categoryIt != attributes.end())
  {
    if (categoryIt->second == "basetype")
    {
      readTypeBasetype(element, vkData.dependencies);
    }
    else if (categoryIt->second == "bitmask")
    {
      readTypeBitmask(element, vkData);
    }
    else if (categoryIt->second == "define")
    {
      readTypeDefine(element, vkData);
    }
    else if (categoryIt->second == "funcpointer")
    {
      readTypeFuncpointer(element, vkData.dependencies);
    }
    else if (categoryIt->second == "handle")
    {
      readTypeHandle(element, vkData);
    }
#if !defined(NDEBUG)
    else if (categoryIt->second == "include")
    {
      skipTypeInclude(element, attributes);
    }
#endif
    else if (categoryIt->second == "struct")
    {
      readTypeStruct(element, vkData, false);
    }
    else if (categoryIt->second == "union")
    {
      readTypeStruct(element, vkData, true);
    }
    else
    {
      assert(categoryIt->second == "enum");
    }
  }
  else
  {
    auto nameIt = attributes.find("name");
    assert(nameIt != attributes.end());
    readTypeName(element, attributes, vkData.dependencies);
  }
}

void readTypeName(tinyxml2::XMLElement * element, std::map<std::string, std::string> const& attributes, std::list<DependencyData> & dependencies)
{
  assert((attributes.size() == 1) || ((attributes.size() == 2) && (attributes.find("requires") != attributes.end())));
  assert(!element->FirstChildElement());

  auto nameIt = attributes.find("name");
  assert(nameIt != attributes.end());
  dependencies.push_back(DependencyData(DependencyData::Category::REQUIRED, nameIt->second));
}

void readTypes(tinyxml2::XMLElement * element, VkData & vkData)
{
  assert(element->FirstAttribute() && !element->FirstAttribute()->Next());
  assert(strcmp(element->FirstAttribute()->Name(), "comment") == 0);

  for (tinyxml2::XMLElement * child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    assert(child->Value());
    std::string value = child->Value();
    if (value == "type")
    {
      readType(child, vkData);
    }
    else
    {
      assert(value == "comment");
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
      assert(vkData.deleters.find(commandData.params[valueIndex].pureType) == vkData.deleters.end());
      vkData.deleters[commandData.params[valueIndex].pureType].pool = commandData.params[1].pureType;
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
    vkData.deleters[commandData.params[valueIndex].pureType].call = commandData.reducedName;
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

bool beginsWith(std::string const& text, std::string const& prefix)
{
  return !prefix.empty() && text.substr(0, prefix.length()) == prefix;
}

bool endsWith(std::string const& text, std::string const& postfix)
{
  return !postfix.empty() && (postfix.length() < text.length()) && (text.substr(text.length() - postfix.length()) == postfix);
}

std::string strip(std::string const& value, std::string const& prefix, std::string const& postfix)
{
  std::string strippedValue = value;
  if (beginsWith(strippedValue, prefix))
  {
    strippedValue.erase(0, prefix.length());
  }
  if (endsWith(strippedValue, postfix))
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

std::vector<std::string> tokenize(std::string tokenString, char separator)
{
  std::vector<std::string> tokens;
  size_t start = 0, end;
  do
  {
    end = tokenString.find(separator, start);
    tokens.push_back(tokenString.substr(start, end - start));
    start = end + 1;
  } while (end != std::string::npos);
  return tokens;
}

std::string toUpperCase(std::string const& name)
{
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

std::string trim(std::string const& input)
{
  std::string result = input;
  result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](char c) { return !std::isspace(c); }));
  result.erase(std::find_if(result.rbegin(), result.rend(), [](char c) { return !std::isspace(c); }).base(), result.end());
  return result;
}

// trim from end
std::string trimEnd(std::string const& input)
{
  std::string result = input;
  result.erase(std::find_if(result.rbegin(), result.rend(), [](char c) { return !std::isspace(c); }).base(), result.end());
  return result;
}

std::string generateCall(CommandData const& commandData, std::set<std::string> const& vkTypes, bool firstCall, bool singular)
{
  std::ostringstream call;
  writeCall(call, commandData, vkTypes, firstCall, singular);
  return call.str();
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
    else if (vkTypes.find(commandData.params[i].pureType) != vkTypes.end())
    {
      writeCallVulkanTypeParameter(os, commandData.params[i]);
    }
    else
    {
      writeCallPlainTypeParameter(os, commandData.params[i]);
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

void writeFunction(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool definition, bool enhanced, bool singular, bool unique, bool isStructureChain)
{
  if (enhanced && (!singular || isStructureChain))
  {
    writeFunctionHeaderTemplate(os, indentation, commandData, !definition, isStructureChain);
  }
  os << indentation << (definition ? "VULKAN_HPP_INLINE " : "");
  writeFunctionHeaderReturnType(os, indentation, commandData, enhanced, singular, unique, isStructureChain);
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
        writeFunctionBodyEnhanced(os, indentation, vkData, commandData, singular, isStructureChain);
      }
    }
    else
    {
      writeFunctionBodyStandard(os, indentation, vkData, commandData);
    }
    os << indentation << "}" << std::endl;
  }
}

void writeFunctionBodyEnhanced(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool singular, bool isStructureChain)
{
  if (1 < commandData.vectorParams.size())
  {
    writeFunctionBodyEnhancedMultiVectorSizeCheck(os, indentation, commandData);
  }

  std::string returnName;
  if (commandData.returnParam != ~0)
  {
    returnName = writeFunctionBodyEnhancedLocalReturnVariable(os, indentation, commandData, singular, isStructureChain);
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

void writeFunctionBodyEnhanced(std::ostream &os, std::string const& templateString, std::string const& indentation, std::set<std::string> const& vkTypes, CommandData const& commandData, bool singular)
{
  os << replaceWithMap(templateString, {
    { "call", generateCall(commandData, vkTypes, true, singular) },
    { "i", indentation }
  });

}

void writeFunctionBodyEnhancedCall(std::ostream &os, std::string const& indentation, std::set<std::string> const& vkTypes, CommandData const& commandData, bool singular)
{
  std::string const templateString = "${i}  return ${call};\n";
  std::string const templateStringVoid = "${i}  ${call};\n";
  writeFunctionBodyEnhanced(os, commandData.returnType == "void" ? templateStringVoid : templateString, indentation, vkTypes, commandData, singular);
}

void writeFunctionBodyEnhancedCallResult(std::ostream &os, std::string const& indentation, std::set<std::string> const& vkTypes, CommandData const& commandData, bool singular)
{
  std::string const templateString = "${i}  Result result = static_cast<Result>( ${call} );\n";
  writeFunctionBodyEnhanced(os, templateString, indentation, vkTypes, commandData, singular);
}

void writeFunctionBodyTwoStep(std::ostream & os, std::string const &templateString, std::string const& indentation, std::set<std::string> const& vkTypes, std::string const& returnName, std::string const& sizeName, CommandData const& commandData)
{
  std::map<std::string, std::string> replacements = {
    { "sizeName", sizeName },
    { "returnName", returnName },
    { "call1", generateCall(commandData, vkTypes, true, false) },
    { "call2", generateCall(commandData, vkTypes, false, false) },
    { "i", indentation }
  };

  os << replaceWithMap(templateString, replacements);
}

void writeFunctionBodyEnhancedCallTwoStep(std::ostream & os, std::string const& indentation, std::set<std::string> const& vkTypes, std::string const& returnName, std::string const& sizeName, CommandData const& commandData)
{
  std::string const templateString = 
R"(${i}  ${call1};
${i}  ${returnName}.resize( ${sizeName} );
${i}  ${call2};
)";
  writeFunctionBodyTwoStep(os, templateString, indentation, vkTypes, returnName, sizeName, commandData);
}

void writeFunctionBodyEnhancedCallTwoStepChecked(std::ostream & os, std::string const& indentation, std::set<std::string> const& vkTypes, std::string const& returnName, std::string const& sizeName, CommandData const& commandData)
{
  std::string const templateString =
R"(${i}  Result result = static_cast<Result>( ${call1} );
${i}  if ( ( result == Result::eSuccess ) && ${sizeName} )
${i}  {
${i}    ${returnName}.resize( ${sizeName} );
${i}    result = static_cast<Result>( ${call2} );
${i}  }
)";
  writeFunctionBodyTwoStep(os, templateString, indentation, vkTypes, returnName, sizeName, commandData);
}

void writeFunctionBodyEnhancedCallTwoStepIterate(std::ostream & os, std::string const& indentation, std::set<std::string> const& vkTypes, std::string const& returnName, std::string const& sizeName, CommandData const& commandData)
{
  std::string const templateString = 
R"(${i}  Result result;
${i}  do
${i}  {
${i}    result = static_cast<Result>( ${call1} );
${i}    if ( ( result == Result::eSuccess ) && ${sizeName} )
${i}    {
${i}      ${returnName}.resize( ${sizeName} );
${i}      result = static_cast<Result>( ${call2} );
${i}    }
${i}  } while ( result == Result::eIncomplete );
${i}  assert( ${sizeName} <= ${returnName}.size() );
${i}  ${returnName}.resize( ${sizeName} );
)";
  writeFunctionBodyTwoStep(os, templateString, indentation, vkTypes, returnName, sizeName, commandData);
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

std::string writeFunctionBodyEnhancedLocalReturnVariable(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool singular, bool isStructureChain)
{
  std::string returnName = startLowerCase(strip(commandData.params[commandData.returnParam].name, "p"));

  // there is a returned parameter -> we need a local variable to hold that value
  if (commandData.returnType != commandData.enhancedReturnType)
  {
    // the returned parameter is somehow enhanced by us
    os << indentation << "  ";
    if (singular)
    {
      if (isStructureChain)
      {
        std::string const &pureType = commandData.params[commandData.returnParam].pureType;
        // For StructureChains use the template parameters
        os << "StructureChain<T...> structureChain;" << std::endl;
        returnName = stripPluralS(returnName);
        os << indentation << "  " << pureType << "& " << returnName << " = structureChain.template get<" << pureType << ">()";
        returnName = "structureChain";
      }
      else
      {
        // in singular case, just use the return parameters pure type for the return variable
        returnName = stripPluralS(returnName);
        os << commandData.params[commandData.returnParam].pureType << " " << returnName;
      }
    }
    else
    {
      // in non-singular case, use the enhanced type for the return variable (like vector<...>)
      if (isStructureChain)
      {
        std::string const &returnType = commandData.enhancedReturnType;
        // For StructureChains use the template parameters
        os << "StructureChain<T...> structureChain;" << std::endl;
        os << indentation << "  " << returnType << "& " << returnName << " = structureChain.template get<" << returnType << ">()";
        returnName = "structureChain";
      }
      else
      {
        os << commandData.enhancedReturnType << " " << returnName;
      }

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
  std::string const templateString = 
R"#(#ifdef VULKAN_HPP_NO_EXCEPTIONS
${i}  assert( ${firstVectorName}.size() == ${secondVectorName}.size() );
#else
${i}  if ( ${firstVectorName}.size() != ${secondVectorName}.size() )
${i}  {
${i}    throw LogicError( "VULKAN_HPP_NAMESPACE::${className}::${reducedName}: ${firstVectorName}.size() != ${secondVectorName}.size()" );
${i}  }
#endif  // VULKAN_HPP_NO_EXCEPTIONS
)#";


  // add some error checks if multiple vectors need to have the same size
  for (std::map<size_t, size_t>::const_iterator it0 = commandData.vectorParams.begin(); it0 != commandData.vectorParams.end(); ++it0)
  {
    if (it0->first != commandData.returnParam)
    {
      for (std::map<size_t, size_t>::const_iterator it1 = std::next(it0); it1 != commandData.vectorParams.end(); ++it1)
      {
        if ((it1->first != commandData.returnParam) && (it0->second == it1->second))
        {
          os << replaceWithMap(templateString, std::map<std::string, std::string>( {
            { "firstVectorName", startLowerCase(strip(commandData.params[it0->first].name, "p")) },
            { "secondVectorName", startLowerCase(strip(commandData.params[it1->first].name, "p")) },
            { "className", commandData.className },
            { "reducedName", commandData.reducedName},
            { "i", indentation}
          }));
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
  os << "\"VULKAN_HPP_NAMESPACE::" << (commandData.className.empty() ? "" : commandData.className + "::") << (singular ? stripPluralS(commandData.reducedName) : commandData.reducedName) << "\"";

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
  if (vkData.deleters.find(commandData.className) != vkData.deleters.end())
  {
    // if the Deleter is specific to the command's class, add '*this' to the deleter
    os << "*this, ";
  }

  // get the DeleterData corresponding to the returned type
  std::map<std::string, DeleterData>::const_iterator ddit = vkData.deleters.find(type);
  assert(ddit != vkData.deleters.end());
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
    std::string const stringTemplate = R"(;
${i}  std::vector<Unique${type}> unique${type}s;
${i}  unique${type}s.reserve( ${typeValue}s.size() );
${i}  for ( auto ${typeValue} : ${typeValue}s )
${i}  {
${i}    unique${type}s.push_back( Unique${type}( ${typeValue}, deleter ) );
${i}  }
${i}  return unique${type}s;
)";
    os << replaceWithMap(stringTemplate, std::map<std::string, std::string>{
      { "i", indentation },
      { "type", type },
      { "typeValue", typeValue }
    });
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
                if (enumIt->second.values.empty())
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

void writeFunctionHeaderReturnType(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool enhanced, bool singular, bool unique, bool isStructureChain)
{
  std::string templateString;
  std::string returnType;
  if (enhanced)
  {
    // the enhanced function might return some pretty complex return stuff
    if (unique)
    {
      // the unique version returns something prefixed with 'Unique'; potentially a vector of that stuff
      // it's a vector, if it's not the singular version and the return parameter is a vector parameter
      bool returnsVector = !singular && (commandData.vectorParams.find(commandData.returnParam) != commandData.vectorParams.end());

      templateString = returnsVector ? "std::vector<Unique${returnType}> " : "Unique${returnType} ";
      returnType = isStructureChain ? "StructureChain<T...>" : commandData.params[commandData.returnParam].pureType;
    }
    else if ((commandData.enhancedReturnType != commandData.returnType) && (commandData.returnType != "void"))
    {
      // if the enhanced return type differs from the original return type, and it's not void, we return a ResultValueType<...>::type
      if (isStructureChain || (!singular && (commandData.enhancedReturnType.find("Allocator") != std::string::npos)))
      {
        // for the non-singular case with allocation, we need to prepend with 'typename' to keep compilers happy
        templateString = "typename ";
      }
      templateString += "ResultValueType<${returnType}>::type ";

      assert(commandData.returnType == "Result");
      // in singular case, we create the ResultValueType from the pure return type, otherwise from the enhanced return type
      if (isStructureChain)
      {
        returnType = "StructureChain<T...>";
      }
      else
      {
        returnType = singular ? commandData.params[commandData.returnParam].pureType : commandData.enhancedReturnType;
      }
    }
    else if ((commandData.returnParam != ~0) && (1 < commandData.successCodes.size()))
    {
      // if there is a return parameter at all, and there are multiple success codes, we return a ResultValue<...> with the pure return type
      assert(commandData.returnType == "Result");
      templateString = "ResultValue<${returnType}> ";
      returnType = isStructureChain ? "StructureChain<T...>" : commandData.params[commandData.returnParam].pureType;
    }
    else
    {
      // and in every other case, we just return the enhanced return type.
      templateString = "${returnType} ";
      returnType = isStructureChain ? "StructureChain<T...>" : commandData.enhancedReturnType;
    }
  }
  else
  {
    // the non-enhanced function just uses the return type
    templateString = "${returnType} ";
    returnType = commandData.returnType;
  }
  os << replaceWithMap(templateString, { { "returnType", returnType } });
}

void writeFunctionHeaderTemplate(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool withDefault, bool isStructureChain)
{
  if (isStructureChain)
  {
    os << indentation << "template <typename ...T>" << std::endl;
  }
  else if ((commandData.templateParam != ~0) && ((commandData.templateParam != commandData.returnParam) || (commandData.enhancedReturnType == "Result")))
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

void writeStandardOrEnhanced(std::ostream & os, std::string const& standard, std::string const& enhanced)
{
  if (standard == enhanced)
  {
    // standard and enhanced string are equal -> just use one of them and we're done
    os << standard;
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
      os << "#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE" << std::endl;
    }
    os << standard
      << (unchangedInterface ? "#else" : "#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE") << std::endl
      << enhanced
      << "#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/" << std::endl;
  }
}

void writeStructConstructor( std::ostream & os, std::string const& name, StructData const& structData, std::set<std::string> const& vkTypes,
                             std::map<std::string,std::string> const& nameMap, std::map<std::string,std::string> const& defaultValues )
{
  // the constructor with all the elements as arguments, with defaults
  os << "    " << name << "( ";
  bool listedArgument = false;
  for (size_t i = 0; i<structData.members.size(); i++)
  {
    if (listedArgument)
    {
      os << ", ";
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
        os << structData.members[i].type << " " << structData.members[i].name << "_ = " << (structData.members[i].type.back() == '*' ? "nullptr" : defaultIt->second);
      }
      else
      {
        // array members are provided as const reference to a std::array
        // the arguments name get a trailing '_', to distinguish them from the actual struct members
        // list as many default values as there are elements in the array
        os << "std::array<" << structData.members[i].type << "," << structData.members[i].arraySize << "> const& " << structData.members[i].name << "_ = { { " << defaultIt->second;
        size_t n = atoi(structData.members[i].arraySize.c_str());
        assert(0 < n);
        for (size_t j = 1; j < n; j++)
        {
          os << ", " << defaultIt->second;
        }
        os << " } }";
      }
      listedArgument = true;
    }
  }
  os << " )" << std::endl;

  // copy over the simple arguments
  bool firstArgument = true;
  for (size_t i = 0; i < structData.members.size(); i++)
  {
    if (structData.members[i].arraySize.empty())
    {
      // here, we can only handle non-array arguments
      std::string templateString = "      ${sep} ${member}( ${value} )\n";
      std::string sep = firstArgument ? ":" : ",";
      std::string member = structData.members[i].name;
      std::string value;

      // 'pNext' and 'sType' don't get an argument, use nullptr and the correct StructureType enum value to initialize them
      if (structData.members[i].name == "pNext")
      {
        value = "nullptr";
      }
      else if (structData.members[i].name == "sType")
      {
        assert(!structData.members[i].values.empty());
        auto nameIt = nameMap.find(structData.members[i].values);
        assert(nameIt != nameMap.end());
        value = nameIt->second;
      }
      else
      {
        // the other elements are initialized by the corresponding argument (with trailing '_', as mentioned above)
        value = structData.members[i].name + "_";
      }
      os << replaceWithMap(templateString, { {"sep", sep}, {"member", member}, {"value", value} });
      firstArgument = false;
    }
  }

  // the body of the constructor, copying over data from argument list into wrapped struct
  os << "    {" << std::endl;
  for ( size_t i=0 ; i<structData.members.size() ; i++ )
  {
    if (!structData.members[i].arraySize.empty())
    {
      // here we can handle the arrays, copying over from argument (with trailing '_') to member
      // size is arraySize times sizeof type
      std::string member = structData.members[i].name;
      std::string arraySize = structData.members[i].arraySize;
      std::string type = structData.members[i].type;
      os << replaceWithMap("      memcpy( &${member}, ${member}_.data(), ${arraySize} * sizeof( ${type} ) );\n",
                            { {"member", member}, {"arraySize", arraySize }, {"type", type} });
    }
  }
  os << "    }" << std::endl
      << std::endl;

  std::string templateString = 
R"(    ${name}( Vk${name} const & rhs )
    {
      memcpy( this, &rhs, sizeof( ${name} ) );
    }

    ${name}& operator=( Vk${name} const & rhs )
    {
      memcpy( this, &rhs, sizeof( ${name} ) );
      return *this;
    }
)";

  os << replaceWithMap(templateString, { {"name", name } } );
}

void writeStructSetter( std::ostream & os, std::string const& structureName, MemberData const& memberData, std::set<std::string> const& vkTypes )
{
  if (memberData.type != "StructureType") // filter out StructureType, which is supposed to be immutable !
  {
    // the setters return a reference to the structure
    os << "    " << structureName << "& set" << startUpperCase(memberData.name) << "( ";
    if (memberData.arraySize.empty())
    {
      os << memberData.type << " ";
    }
    else
    {
      os << "std::array<" << memberData.type << "," << memberData.arraySize << "> ";
    }
    // add a trailing '_' to the argument to distinguish it from the structure member
    os << memberData.name << "_ )" << std::endl
      << "    {" << std::endl;
    // copy over the argument, either by assigning simple data, or by memcpy array data
    if (memberData.arraySize.empty())
    {
      os << "      " << memberData.name << " = " << memberData.name << "_";
    }
    else
    {
      os << "      memcpy( &" << memberData.name << ", " << memberData.name << "_.data(), " << memberData.arraySize << " * sizeof( " << memberData.type << " ) )";
    }
    os << ";" << std::endl
      << "      return *this;" << std::endl
      << "    }" << std::endl
      << std::endl;
  }
}

void writeTypeCommand(std::ostream & os, VkData const& vkData, DependencyData const& dependencyData)
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

      writeDeleterForwardDeclarations(os, *deleterTypesIt, vkData.deleters);
      writeTypeCommand(os, "  ", vkData, commandData, false);
      writeDeleterClasses(os, *deleterTypesIt, vkData.deleters);
    }
    else
    {
      writeTypeCommand(os, "  ", vkData, commandData, false);
    }
    writeTypeCommand(os, "  ", vkData, commandData, true);
    os << std::endl;
  }
}

void writeTypeCommand(std::ostream & os, std::string const& indentation, VkData const& vkData, CommandData const& commandData, bool definition)
{
  enterProtect(os, commandData.protect);

  bool isStructureChain = vkData.extendedStructs.find(commandData.enhancedReturnType) != vkData.extendedStructs.end();

  // first create the standard version of the function
  std::ostringstream standard;
  writeFunction(standard, indentation, vkData, commandData, definition, false, false, false, false);

  // then the enhanced version, composed by up to five parts
  std::ostringstream enhanced;
  writeFunction(enhanced, indentation, vkData, commandData, definition, true, false, false, false);

  if (isStructureChain)
  {
    writeFunction(enhanced, indentation, vkData, commandData, definition, true, false, false, true);
  }

  // then a singular version, if a sized vector would be returned
  std::map<size_t, size_t>::const_iterator returnVector = commandData.vectorParams.find(commandData.returnParam);
  bool singular = (returnVector != commandData.vectorParams.end()) && (returnVector->second != ~0) && (commandData.params[returnVector->second].type.back() != '*');
  if (singular)
  {
    writeFunction(enhanced, indentation, vkData, commandData, definition, true, true, false, false);
  }

  // special handling for createDevice and createInstance !
  bool specialWriteUnique = (commandData.reducedName == "createDevice") || (commandData.reducedName == "createInstance");

  // and then the same for the Unique* versions (a Deleter is available for the commandData's class, and the function starts with 'allocate' or 'create')
  if (((vkData.deleters.find(commandData.className) != vkData.deleters.end()) || specialWriteUnique) && ((commandData.reducedName.substr(0, 8) == "allocate") || (commandData.reducedName.substr(0, 6) == "create")))
  {
    enhanced << "#ifndef VULKAN_HPP_NO_SMART_HANDLE" << std::endl;
    writeFunction(enhanced, indentation, vkData, commandData, definition, true, false, true, false);

    if (singular)
    {
      writeFunction(enhanced, indentation, vkData, commandData, definition, true, true, true, false);
    }
    enhanced << "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/" << std::endl;
  }

  // and write one or both of them
  writeStandardOrEnhanced(os, standard.str(), enhanced.str());
  leaveProtect(os, commandData.protect);
  os << std::endl;
}

void writeTypeEnum( std::ostream & os, EnumData const& enumData )
{
  // a named enum per enum, listing all its values by setting them to the original Vulkan names
  enterProtect(os, enumData.protect);
  os << "  enum class " << enumData.name << std::endl
      << "  {" << std::endl;
  for ( size_t i=0 ; i<enumData.values.size() ; i++ )
  {
    os << "    " << enumData.values[i].name << " = " << enumData.values[i].value;
    if (!enumData.values[i].alias.empty())
    {
      os << "," << std::endl
        << "    " << enumData.values[i].alias << " = " << enumData.values[i].value;
    }
    if (i < enumData.values.size() - 1)
    {
      os << ",";
    }
    os << std::endl;
  }
  os << "  };" << std::endl;

  if (!enumData.alias.empty())
  {
    os << std::endl
      << "  using " << enumData.alias << " = " << enumData.name << ";" << std::endl;
  }

  leaveProtect(os, enumData.protect);
  os << std::endl;
}

bool isErrorEnum(std::string const& enumName)
{
  return (enumName.substr(0, 6) == "eError");
}

std::string stripErrorEnumPrefix(std::string const& enumName)
{
  assert(isErrorEnum(enumName));
  return strip(enumName, "eError");
}

// Intended only for `enum class Result`!
void writeExceptionsForEnum( std::ostream & os, EnumData const& enumData)
{
  std::string templateString =
R"(  class ${className} : public SystemError
  {
  public:
    ${className}( std::string const& message )
      : SystemError( make_error_code( ${enumName}::${enumMemberName} ), message ) {}
    ${className}( char const * message )
      : SystemError( make_error_code( ${enumName}::${enumMemberName} ), message ) {}
  };
)";
  
  enterProtect(os, enumData.protect);
  for (size_t i = 0; i < enumData.values.size(); i++)
  {
    if (!isErrorEnum(enumData.values[i].name))
    {
      continue;
    }
    os << replaceWithMap(templateString,
    { { "className", stripErrorEnumPrefix(enumData.values[i].name) + "Error"},
      { "enumName", enumData.name},
      { "enumMemberName", enumData.values[i].name}
    });
  }
  leaveProtect(os, enumData.protect);
  os << std::endl;
}

void writeThrowExceptions( std::ostream & os, EnumData const& enumData)
{
  enterProtect(os, enumData.protect);
  os << 
R"(  VULKAN_HPP_INLINE void throwResultException( Result result, char const * message )
  {
    assert ( static_cast<long long int>(result) < 0 );
    switch ( result )
    {
)";
  for ( size_t i=0 ; i<enumData.values.size() ; i++ )
  {
    if (!isErrorEnum(enumData.values[i].name))
    {
      continue;
    }
    const std::string strippedExceptionName = stripErrorEnumPrefix(enumData.values[i].name);
    os << "    case " << enumData.name << "::" << enumData.values[i].name << ": "
        << "throw " << strippedExceptionName << "Error ( message );" << std::endl;
  }
  os <<
R"(    default: throw SystemError( make_error_code( result ) );
    }
  }
)";
  leaveProtect(os, enumData.protect);
}

void writeDeleterClasses(std::ostream & os, std::pair<std::string, std::set<std::string>> const& deleterTypes, std::map<std::string, DeleterData> const& deleters)
{
  // A Deleter class for each of the Unique* classes... but only if smart handles are not switched off
  os << "#ifndef VULKAN_HPP_NO_SMART_HANDLE" << std::endl;
  bool first = true;

  // get type and name of the parent (holder) type
  std::string parentType = deleterTypes.first;
  std::string parentName = parentType.empty() ? "" : startLowerCase(parentType);

  // iterate over the deleter types parented by this type
  for (auto const& deleterType : deleterTypes.second)
  {
    std::string deleterName = startLowerCase(deleterType);
    bool standardDeleter = !parentType.empty() && (deleterType != "Device");    // this detects the 'standard' case for a deleter
                                                                                // if this Deleter is pooled, make such a pool the last argument, otherwise an Optional allocator
    auto const& dd = deleters.find(deleterType);
    assert(dd != deleters.end());
    std::string poolName = (dd->second.pool.empty() ? "" : startLowerCase(dd->second.pool));

    if (!first)
    {
      os << std::endl;
    }
    first = false;

    os << "  class " << deleterType << "Deleter" << std::endl
       << "  {" << std::endl
       << "  public:" << std::endl
       << "    " << deleterType << "Deleter( ";
    if (standardDeleter)
    {
      // the standard deleter gets a parent type in the constructor
      os << parentType << " " << parentName << " = " << parentType << "(), ";
    }

    if (poolName.empty())
    {
      os << "Optional<const AllocationCallbacks> allocator = nullptr )" << std::endl;
    }
    else
    {
      assert(!dd->second.pool.empty());
      os << dd->second.pool << " " << poolName << " = " << dd->second.pool << "() )" << std::endl;
    }

    // now the initializer list of the Deleter constructor
    os << "      : ";
    if (standardDeleter)
    {
      // the standard deleter has a parent type as a member
      os << "m_" << parentName << "( " << parentName << " )" << std::endl
        << "      , ";
    }
    if (poolName.empty())
    {
      // non-pooled deleter have an allocator as a member
      os << "m_allocator( allocator )" << std::endl;
    }
    else
    {
      // pooled deleter have a pool as a member
      os << "m_" << poolName << "( " << poolName << " )" << std::endl;
    }

    // besides that, the constructor is empty
    os << "    {}" << std::endl
      << std::endl;

    // getter for the parent type
    if (standardDeleter)
    {
      os << "    " << parentType << " get" << parentType << "() const { return m_" << parentName << "; }\n";
    }

    // getter for pool
    if (!poolName.empty())
    {
      os << "    " << dd->second.pool << " get" << dd->second.pool << "() const { return m_" << poolName << "; }\n";
    }
    else // getter for allocator
    {
      os << "    Optional<const AllocationCallbacks> getAllocator() const { return m_allocator; }\n";
    }

    os << "\n";

    // the operator() calls the delete/destroy function
    os << "  protected:\n"
       << "    void destroy( " << deleterType << " " << deleterName << " )\n"
      << "    {\n";

    // the delete/destroy function is either part of the parent member of the deleter argument
    if (standardDeleter)
    {
      os << "      m_" << parentName << ".";
    }
    else
    {
      os << "      " << deleterName << ".";
    }

    os << dd->second.call << "( ";

    if (!poolName.empty())
    {
      // pooled Deleter gets the pool as the first argument
      os << "m_" << poolName << ", ";
    }

    if (standardDeleter)
    {
      // the standard deleter gets the deleter argument as an argument
      os << deleterName;
    }

    // the non-pooled deleter get the allocate as an argument (potentially after the deleterName
    if (poolName.empty())
    {
      if (standardDeleter)
      {
        os << ", ";
      }
      os << "m_allocator";
    }
    os << " );" << std::endl
      << "    }" << std::endl
      << std::endl;

    // now the members of the Deleter class
    os << "  private:" << std::endl;
    if (standardDeleter)
    {
      // the parentType for the standard deleter
      os << "    " << parentType << " m_" << parentName << ";" << std::endl;
    }

    // the allocator for the non-pooled deleters, the pool for the pooled ones
    if (poolName.empty())
    {
      os << "    Optional<const AllocationCallbacks> m_allocator;" << std::endl;
    }
    else
    {
      os << "    " << dd->second.pool << " m_" << poolName << ";" << std::endl;
    }
    os << "  };" << std::endl;
  }

  os << "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/" << std::endl
    << std::endl;
}

void writeDeleterForwardDeclarations(std::ostream &os, std::pair<std::string, std::set<std::string>> const& deleterTypes, std::map<std::string, DeleterData> const& deleters)
{
  // if smart handles are supported, all the Deleter classes need to be forward declared
  os << "#ifndef VULKAN_HPP_NO_SMART_HANDLE" << std::endl;
  bool first = true;
  std::string firstName = deleterTypes.first.empty() ? "" : startLowerCase(deleterTypes.first);
  for (auto const& dt : deleterTypes.second)
  {
    os << "  class " << dt << "Deleter;" << std::endl;
    os << "  template <> class UniqueHandleTraits<" << dt << "> {public: using deleter = " << dt << "Deleter; };\n";
    os << "  using Unique" << dt << " = UniqueHandle<" << dt << ">;" << std::endl;
  }
  os << "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/" << std::endl
    << std::endl;
}

void writeEnumsToString(std::ostream & os, EnumData const& enumData)
{
  // the helper functions to make strings out of enum values
  enterProtect(os, enumData.protect);
  os << "  VULKAN_HPP_INLINE std::string to_string(" << enumData.name << (enumData.values.empty() ? ")" : " value)") << std::endl
      << "  {" << std::endl;
  if (enumData.values.empty())
  {
    // no enum values in this enum -> return "(void)"
    os << "    return \"(void)\";" << std::endl;
  }
  else
  {
    // otherwise switch over the value and return the a stringized version of that value (without leading 'e')
    os << "    switch (value)" << std::endl
        << "    {" << std::endl;
    for (auto const& value : enumData.values)
    {
      os << "    case " << enumData.name << "::" << value.name << ": return \"" << value.name.substr(1) << "\";" << std::endl;
    }
    os << "    default: return \"invalid\";" << std::endl
        << "    }" << std::endl;
  }
  os << "  }" << std::endl;
  leaveProtect(os, enumData.protect);
  os << std::endl;
}

void writeFlagsToString(std::ostream & os, std::string const& flagsName, EnumData const &enumData)
{
  // the helper functions to make strings out of flag values
  enterProtect(os, enumData.protect);
  os << "  VULKAN_HPP_INLINE std::string to_string(" << flagsName << (enumData.values.empty() ? ")" : " value)") << std::endl
      << "  {" << std::endl;
  if (enumData.values.empty())
  {
    // no flags values in this enum -> return "{}"
    os << "    return \"{}\";" << std::endl;
  }
  else
  {
    os << "    if (!value) return \"{}\";" << std::endl
        << "    std::string result;" << std::endl;

    // 'or' together all the bits in the value
    for (auto valuesIt = enumData.values.begin(); valuesIt != enumData.values.end(); ++valuesIt)
    {
      os << "    if (value & " << enumData.name << "::" << valuesIt->name << ") result += \"" << valuesIt->name.substr(1) << " | \";" << std::endl;
    }
    // cut off the last three characters from the result (being " | ")
    os << "    return \"{\" + result.substr(0, result.size() - 3) + \"}\";" << std::endl;
  }
  os << "  }" << std::endl;
  leaveProtect(os, enumData.protect);
  os << std::endl;
}

void writeTypeFlags(std::ostream & os, std::string const& flagsName, FlagData const& flagData, EnumData const& enumData)
{
  enterProtect(os, flagData.protect);
  // each Flags class is using on the class 'Flags' with the corresponding FlagBits enum as the template parameter
  os << "  using " << flagsName << " = Flags<" << enumData.name << ", Vk" << flagsName << ">;" << std::endl;

  std::stringstream allFlags;
  for (size_t i = 0; i < enumData.values.size(); i++)
  {
    if (i != 0)
    {
      allFlags << " | ";
    }
    allFlags << "VkFlags(" << enumData.name << "::" << enumData.values[i].name << ")";
  }

  if (!enumData.values.empty())
  {
    const std::string templateString = R"(
  VULKAN_HPP_INLINE ${flagsName} operator|( ${enumName} bit0, ${enumName} bit1 )
  {
    return ${flagsName}( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE ${flagsName} operator~( ${enumName} bits )
  {
    return ~( ${flagsName}( bits ) );
  }

  template <> struct FlagTraits<${enumName}>
  {
    enum
    {
      allFlags = ${allFlags}
    };
  };
)";
    os << replaceWithMap(templateString, { { "flagsName", flagsName}, { "enumName", enumData.name }, { "allFlags", allFlags.str() } } );
  }

  if (!flagData.alias.empty())
  {
    os << std::endl
      << "  using " << flagData.alias << " = " << flagsName << ";" << std::endl;
  }

  leaveProtect(os, flagData.protect);
  os << std::endl;
}

void writeTypeHandle(std::ostream & os, VkData const& vkData, DependencyData const& dependencyData, HandleData const& handleData, std::list<DependencyData> const& dependencies)
{
  enterProtect(os, handleData.protect);

  // check if there are any forward dependenices for this handle -> list them first
  if (!dependencyData.forwardDependencies.empty())
  {
    os << "  // forward declarations" << std::endl;
    for (std::set<std::string>::const_iterator it = dependencyData.forwardDependencies.begin(); it != dependencyData.forwardDependencies.end(); ++it)
    {
      assert(vkData.structs.find(*it) != vkData.structs.end());
      os << "  struct " << *it << ";" << std::endl;
    }
    os << std::endl;
  }

  // then write any forward declaration of Deleters used by this handle
  std::map<std::string, std::set<std::string>>::const_iterator deleterTypesIt = vkData.deleterTypes.find(dependencyData.name);
  if (deleterTypesIt != vkData.deleterTypes.end())
  {
    writeDeleterForwardDeclarations(os, *deleterTypesIt, vkData.deleters);
  }

  const std::string memberName = startLowerCase(dependencyData.name);
  const std::string templateString = 
R"(  class ${className}
  {
  public:
    ${className}()
      : m_${memberName}(VK_NULL_HANDLE)
    {}

    ${className}( std::nullptr_t )
      : m_${memberName}(VK_NULL_HANDLE)
    {}

    VULKAN_HPP_TYPESAFE_EXPLICIT ${className}( Vk${className} ${memberName} )
       : m_${memberName}( ${memberName} )
    {}

#if defined(VULKAN_HPP_TYPESAFE_CONVERSION)
    ${className} & operator=(Vk${className} ${memberName})
    {
      m_${memberName} = ${memberName};
      return *this; 
    }
#endif

    ${className} & operator=( std::nullptr_t )
    {
      m_${memberName} = VK_NULL_HANDLE;
      return *this;
    }

    bool operator==( ${className} const & rhs ) const
    {
      return m_${memberName} == rhs.m_${memberName};
    }

    bool operator!=(${className} const & rhs ) const
    {
      return m_${memberName} != rhs.m_${memberName};
    }

    bool operator<(${className} const & rhs ) const
    {
      return m_${memberName} < rhs.m_${memberName};
    }

${commands}

    VULKAN_HPP_TYPESAFE_EXPLICIT operator Vk${className}() const
    {
      return m_${memberName};
    }

    explicit operator bool() const
    {
      return m_${memberName} != VK_NULL_HANDLE;
    }

    bool operator!() const
    {
      return m_${memberName} == VK_NULL_HANDLE;
    }

  private:
    Vk${className} m_${memberName};
  };

  static_assert( sizeof( ${className} ) == sizeof( Vk${className} ), "handle and wrapper have different size!" );

)";

  std::ostringstream commands;
  // now list all the commands that are mapped to members of this class
  for (size_t i = 0; i < handleData.commands.size(); i++)
  {
    std::string commandName = handleData.commands[i];
    std::map<std::string, CommandData>::const_iterator cit = vkData.commands.find(commandName);
    assert((cit != vkData.commands.end()) && !cit->second.className.empty());
    writeTypeCommand(commands, "    ", vkData, cit->second, false);
  }

  os << replaceWithMap(templateString, {
    { "className", dependencyData.name },
    { "memberName", memberName },
    { "commands", commands.str() }
  });

  if (!handleData.alias.empty())
  {
    os << std::endl
      << "  using " << handleData.alias << " = " << dependencyData.name << ";" << std::endl;
  }

  // then the actual Deleter classes can be listed
  deleterTypesIt = vkData.deleterTypes.find(dependencyData.name);
  if (deleterTypesIt != vkData.deleterTypes.end())
  {
    writeDeleterClasses(os, *deleterTypesIt, vkData.deleters);
  }

  // and finally the commands, that are member functions of this handle
  for (size_t i = 0; i < handleData.commands.size(); i++)
  {
    std::string commandName = handleData.commands[i];
    std::map<std::string, CommandData>::const_iterator cit = vkData.commands.find(commandName);
    assert((cit != vkData.commands.end()) && !cit->second.className.empty());
    std::list<DependencyData>::const_iterator dep = std::find_if(dependencies.begin(), dependencies.end(), [commandName](DependencyData const& dd) { return dd.name == commandName; });
    assert(dep != dependencies.end() && (dep->name == cit->second.fullName));
    writeTypeCommand(os, "  ", vkData, cit->second, true);
  }

  leaveProtect(os, handleData.protect);
}

void writeTypeScalar( std::ostream & os, DependencyData const& dependencyData )
{
  assert( dependencyData.dependencies.size() == 1 );
  os << "  using " << dependencyData.name << " = " << *dependencyData.dependencies.begin() << ";" << std::endl
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

void writeTypeStruct( std::ostream & os, VkData const& vkData, DependencyData const& dependencyData, std::map<std::string,std::string> const& defaultValues )
{
  std::map<std::string,StructData>::const_iterator it = vkData.structs.find( dependencyData.name );
  assert( it != vkData.structs.end() );

  enterProtect(os, it->second.protect);
  os << "  struct " << dependencyData.name << std::endl
      << "  {" << std::endl;

  // only structs that are not returnedOnly get a constructor!
  if ( !it->second.returnedOnly )
  {
    writeStructConstructor( os, dependencyData.name, it->second, vkData.vkTypes, vkData.nameMap, defaultValues );
  }

  // create the setters
  if (!it->second.returnedOnly)
  {
    for (size_t i = 0; i<it->second.members.size(); i++)
    {
      writeStructSetter( os, dependencyData.name, it->second.members[i], vkData.vkTypes );
    }
  }

  // the cast-operator to the wrapped struct
  os << "    operator const Vk" << dependencyData.name << "&() const" << std::endl
      << "    {" << std::endl
      << "      return *reinterpret_cast<const Vk" << dependencyData.name << "*>(this);" << std::endl
      << "    }" << std::endl
      << std::endl;

  // operator==() and operator!=()
  // only structs without a union as a member can have a meaningfull == and != operation; we filter them out
  if (!containsUnion(dependencyData.name, vkData.structs))
  {
    // two structs are compared by comparing each of the elements
    os << "    bool operator==( " << dependencyData.name << " const& rhs ) const" << std::endl
        << "    {" << std::endl
        << "      return ";
    for (size_t i = 0; i < it->second.members.size(); i++)
    {
      if (i != 0)
      {
        os << std::endl << "          && ";
      }
      if (!it->second.members[i].arraySize.empty())
      {
        os << "( memcmp( " << it->second.members[i].name << ", rhs." << it->second.members[i].name << ", " << it->second.members[i].arraySize << " * sizeof( " << it->second.members[i].type << " ) ) == 0 )";
      }
      else
      {
        os << "( " << it->second.members[i].name << " == rhs." << it->second.members[i].name << " )";
      }
    }
    os << ";" << std::endl
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
      os << "  private:" << std::endl
          << "    StructureType sType;" << std::endl
          << std::endl
          << "  public:" << std::endl;
    }
    else
    {
      os << "    " << it->second.members[i].type << " " << it->second.members[i].name;
      if (!it->second.members[i].arraySize.empty())
      {
        os << "[" << it->second.members[i].arraySize << "]";
      }
      os << ";" << std::endl;
    }
  }
  os << "  };" << std::endl
      << "  static_assert( sizeof( " << dependencyData.name << " ) == sizeof( Vk" << dependencyData.name << " ), \"struct and wrapper have different size!\" );" << std::endl;

  if (!it->second.alias.empty())
  {
    os << std::endl
      << "  using " << it->second.alias << " = " << it->first << ";" << std::endl;
  }

  leaveProtect(os, it->second.protect);
  os << std::endl;
}

void writeStructureChainValidation(std::ostream & os, VkData const& vkData, DependencyData const& dependencyData)
{
  std::map<std::string, StructData>::const_iterator it = vkData.structs.find(dependencyData.name);
  assert(it != vkData.structs.end());

  if (!it->second.structExtends.empty()) {
    enterProtect(os, it->second.protect);

    // write out allowed structure chains
    for (auto extendName : it->second.structExtends)
    {
      std::map<std::string, StructData>::const_iterator itExtend = vkData.structs.find(extendName);
      assert(itExtend != vkData.structs.end());
      enterProtect(os, itExtend->second.protect);

      os << "  template <> struct isStructureChainValid<" << extendName << ", " << dependencyData.name << ">{ enum { value = true }; };" << std::endl;

      leaveProtect(os, itExtend->second.protect);
    }
    leaveProtect(os, it->second.protect);
  }
}

void writeTypeUnion( std::ostream & os, VkData const& vkData, DependencyData const& dependencyData, std::map<std::string,std::string> const& defaultValues )
{
  std::map<std::string, StructData>::const_iterator it = vkData.structs.find(dependencyData.name);
  assert(it != vkData.structs.end());

  std::ostringstream oss;
  os << "  union " << dependencyData.name << std::endl
      << "  {" << std::endl;

  for ( size_t i=0 ; i<it->second.members.size() ; i++ )
  {
    // one constructor per union element
    os << "    " << dependencyData.name << "( ";
    if ( it->second.members[i].arraySize.empty() )
    {
      os << it->second.members[i].type << " ";
    }
    else
    {
      os << "const std::array<" << it->second.members[i].type << "," << it->second.members[i].arraySize << ">& ";
    }
    os << it->second.members[i].name << "_";

    // just the very first constructor gets default arguments
    if ( i == 0 )
    {
      std::map<std::string,std::string>::const_iterator defaultIt = defaultValues.find( it->second.members[i].pureType );
      assert(defaultIt != defaultValues.end() );
      if ( it->second.members[i].arraySize.empty() )
      {
        os << " = " << defaultIt->second;
      }
      else
      {
        os << " = { {" << defaultIt->second << "} }";
      }
    }
    os << " )" << std::endl
        << "    {" << std::endl
        << "      ";
    if ( it->second.members[i].arraySize.empty() )
    {
      os << it->second.members[i].name << " = " << it->second.members[i].name << "_";
    }
    else
    {
      os << "memcpy( &" << it->second.members[i].name << ", " << it->second.members[i].name << "_.data(), " << it->second.members[i].arraySize << " * sizeof( " << it->second.members[i].type << " ) )";
    }
    os << ";" << std::endl
        << "    }" << std::endl
        << std::endl;
    }

  for (size_t i = 0; i<it->second.members.size(); i++)
  {
    // one setter per union element
    assert(!it->second.returnedOnly);
    writeStructSetter(os, dependencyData.name, it->second.members[i], vkData.vkTypes);
  }

  // the implicit cast operator to the native type
  os << "    operator Vk" << dependencyData.name << " const& () const" << std::endl
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
    os << "#ifdef VULKAN_HPP_HAS_UNRESTRICTED_UNIONS" << std::endl;
    for (size_t i = 0; i < it->second.members.size(); i++)
    {
      os << "    " << it->second.members[i].type << " " << it->second.members[i].name;
      if (!it->second.members[i].arraySize.empty())
      {
        os << "[" << it->second.members[i].arraySize << "]";
      }
      os << ";" << std::endl;
    }
    os << "#else" << std::endl;
  }
  for (size_t i = 0; i < it->second.members.size(); i++)
  {
    os << "    ";
    if (vkData.vkTypes.find(it->second.members[i].type) != vkData.vkTypes.end())
    {
      os << "Vk";
    }
    os << it->second.members[i].type << " " << it->second.members[i].name;
    if (!it->second.members[i].arraySize.empty())
    {
      os << "[" << it->second.members[i].arraySize << "]";
    }
    os << ";" << std::endl;
  }
  if (needsUnrestrictedUnions)
  {
    os << "#endif  // VULKAN_HPP_HAS_UNRESTRICTED_UNIONS" << std::endl;
  }
  os << "  };" << std::endl
      << std::endl;
}

void writeTypes(std::ostream & os, VkData const& vkData, std::map<std::string, std::string> const& defaultValues)
{
  for ( std::list<DependencyData>::const_iterator it = vkData.dependencies.begin() ; it != vkData.dependencies.end() ; ++it )
  {
    switch( it->category )
    {
      case DependencyData::Category::COMMAND :
        writeTypeCommand( os, vkData, *it );
        break;
      case DependencyData::Category::ENUM :
        assert( vkData.enums.find( it->name ) != vkData.enums.end() );
        writeTypeEnum( os, vkData.enums.find( it->name )->second );
        break;
      case DependencyData::Category::FLAGS :
        assert(vkData.flags.find(it->name) != vkData.flags.end());
        writeTypeFlags( os, it->name, vkData.flags.find( it->name)->second, vkData.enums.find(generateEnumNameForFlags(it->name))->second );
        break;
      case DependencyData::Category::FUNC_POINTER :
      case DependencyData::Category::REQUIRED :
        // skip FUNC_POINTER and REQUIRED, they just needed to be in the dependencies list to resolve dependencies
        break;
      case DependencyData::Category::HANDLE :
        assert(vkData.handles.find(it->name) != vkData.handles.end());
        writeTypeHandle(os, vkData, *it, vkData.handles.find(it->name)->second, vkData.dependencies);
        break;
      case DependencyData::Category::SCALAR :
        writeTypeScalar( os, *it );
        break;
      case DependencyData::Category::STRUCT :
        writeTypeStruct( os, vkData, *it, defaultValues );
        break;
      case DependencyData::Category::UNION :
        assert( vkData.structs.find( it->name ) != vkData.structs.end() );
        writeTypeUnion( os, vkData, *it, defaultValues );
        break;
      default :
        assert( false );
        break;
    }
  }
}

void writeVersionCheck(std::ostream & os, std::string const& version)
{
  os << "static_assert( VK_HEADER_VERSION == " << version << " , \"Wrong VK_HEADER_VERSION!\" );" << std::endl
      << std::endl;
}

void writeTypesafeCheck(std::ostream & os, std::string const& typesafeCheck)
{
  os << "// 32-bit vulkan is not typesafe for handles, so don't allow copy constructors on this platform by default." << std::endl
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
      else if (value == "feature")
      {
        readFeature(child, vkData.enums, vkData.nameMap);
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
        assert(value == "vendorids");
#if !defined(NDEBUG)
        skipVendorIDs(child, vkData.vendorIDs);
#endif
      }
    }

    sortDependencies(vkData.dependencies);

#if !defined(NDEBUG)
    for (auto const& ext : vkData.extensions)
    {
      for (auto const& req : ext.second.requires)
      {
        auto reqExt = vkData.extensions.find(req);
        assert(reqExt != vkData.extensions.end());
        assert(reqExt->second.protect.empty() || (reqExt->second.protect == ext.second.protect));
      }
    }
#endif

    std::map<std::string, std::string> defaultValues;
    createDefaults(vkData, defaultValues);

    std::ofstream ofs(VULKAN_HPP);
    ofs << vkData.vulkanLicenseHeader << std::endl
      << R"(
#ifndef VULKAN_HPP
#define VULKAN_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <string>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <vulkan/vulkan.h>
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
# include <memory>
# include <vector>
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
)";

    writeVersionCheck(ofs, vkData.version);
    writeTypesafeCheck(ofs, vkData.typesafeCheck);
    ofs << versionCheckHeader
      << inlineHeader
      << explicitHeader
      << std::endl
      << vkNamespace
      << flagsHeader
      << optionalClassHeader
      << arrayProxyHeader
      << uniqueHandleHeader
      << structureChainHeader;

    // first of all, write out vk::Result and the exception handling stuff
    std::list<DependencyData>::const_iterator it = std::find_if(vkData.dependencies.begin(), vkData.dependencies.end(), [](DependencyData const& dp) { return dp.name == "Result"; });
    assert(it != vkData.dependencies.end());
    writeTypeEnum(ofs, vkData.enums.find(it->name)->second);
    writeEnumsToString(ofs, vkData.enums.find(it->name)->second);
    ofs << "#ifndef VULKAN_HPP_NO_EXCEPTIONS";
    ofs << exceptionHeader;
    ofs << exceptionClassesHeader;
    writeExceptionsForEnum(ofs, vkData.enums.find(it->name)->second);
    writeThrowExceptions(ofs, vkData.enums.find(it->name)->second);
    ofs << "#endif" << std::endl;
    vkData.dependencies.erase(it);

    ofs << "} // namespace VULKAN_HPP_NAMESPACE" << std::endl
      << std::endl
      << "namespace std" << std::endl
      << "{" << std::endl
      << "  template <>" << std::endl
      << "  struct is_error_code_enum<VULKAN_HPP_NAMESPACE::Result> : public true_type" << std::endl
      << "  {};" << std::endl
      << "}" << std::endl
      << std::endl
      << "namespace VULKAN_HPP_NAMESPACE" << std::endl
      << "{" << std::endl
      << resultValueHeader
      << createResultValueHeader;

    assert(vkData.deleterTypes.find("") != vkData.deleterTypes.end());
    writeTypes(ofs, vkData, defaultValues);

    // write all template functions for the structure pointer chain validation
    for (auto it = vkData.dependencies.begin(); it != vkData.dependencies.end(); ++it)
    {
      switch (it->category)
      {
      case DependencyData::Category::STRUCT:
        writeStructureChainValidation(ofs, vkData, *it);
        break;
      }
    }

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

    ofs << "} // namespace VULKAN_HPP_NAMESPACE" << std::endl
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

#if !defined(NDEBUG)
void skipTypeInclude(tinyxml2::XMLElement * element, std::map<std::string, std::string> const& attributes)
{
  assert((attributes.find("category") != attributes.end()) && (attributes.find("category")->second == "include"));
  assert((attributes.size() == 1) || (attributes.find("name") != attributes.end()));

  auto child = element->FirstChildElement();
  assert(!child || !child->NextSiblingElement());
  if (child)
  {
    assert(!child->FirstAttribute() && !child->FirstChildElement());
    std::string name = child->Name();
    assert(name == "name");
  }
}

void skipVendorID(tinyxml2::XMLElement * element, std::vector<VendorIDData> & vendorIDs)
{
  VendorIDData vendorID;
  for (auto attribute = element->FirstAttribute(); attribute; attribute = attribute->Next())
  {
    std::string name = attribute->Name();
    if (name == "comment")
    {
      vendorID.comment = attribute->Value();
    }
    else if (name == "id")
    {
      vendorID.id = attribute->Value();
    }
    else
    {
      assert(name == "name");
      vendorID.name = attribute->Value();
    }
  }
  vendorIDs.push_back(vendorID);
}

void skipVendorIDs(tinyxml2::XMLElement * element, std::vector<VendorIDData> & vendorIDs)
{
  assert(element->FirstAttribute() && !element->FirstAttribute()->Next() && element->Attribute("comment"));
  for (tinyxml2::XMLElement* child = element->FirstChildElement(); child; child = child->NextSiblingElement())
  {
    std::string value = child->Value();
    assert(value == "vendorid");
    skipVendorID(child, vendorIDs);
  }
}
#endif
