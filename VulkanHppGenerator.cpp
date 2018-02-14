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
#include <iterator>
#include <sstream>
#include <exception>
#include <regex>
#include <iterator>
#include "VulkanHppGenerator.hpp"

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

    explicit Flags(MaskType flags)
      : m_mask(flags)
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
      VULKAN_HPP_ASSERT(m_count && m_ptr);
      return *m_ptr;
    }

    const T & back() const
    {
      VULKAN_HPP_ASSERT(m_count && m_ptr);
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

    ResultValue( Result r, T && v )
      : result( r )
      , value( std::move( v ) )
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
    typedef T               type;
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
    VULKAN_HPP_ASSERT( result == Result::eSuccess );
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
    VULKAN_HPP_ASSERT( result == Result::eSuccess );
    return ResultValue<T>( result, data );
#else
    if ( result != Result::eSuccess )
    {
      throwResultException( result, message );
    }
    return std::move( data );
#endif
  }

  VULKAN_HPP_INLINE Result createResultValue( Result result, char const * message, std::initializer_list<Result> successCodes )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    VULKAN_HPP_ASSERT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
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
    VULKAN_HPP_ASSERT( std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end() );
#else
    if ( std::find( successCodes.begin(), successCodes.end(), result ) == successCodes.end() )
    {
      throwResultException( result, message );
    }
#endif
    return ResultValue<T>( result, data );
  }

#ifndef VULKAN_HPP_NO_SMART_HANDLE
  template <typename T>
  VULKAN_HPP_INLINE typename ResultValueType<UniqueHandle<T>>::type createResultValue( Result result, T & data, char const * message, typename UniqueHandleTraits<T>::deleter const& deleter )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    VULKAN_HPP_ASSERT( result == Result::eSuccess );
    return ResultValue<UniqueHandle<T>>( result, UniqueHandle<T>(data, deleter) );
#else
    if ( result != Result::eSuccess )
    {
      throwResultException( result, message );
    }
    return UniqueHandle<T>(data, deleter);
#endif
  }
#endif

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
      if ( m_value ) this->destroy( m_value );
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
        if ( m_value ) this->destroy( m_value );
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

const std::string deleterClassString = R"(
  struct AllocationCallbacks;

  template <typename OwnerType>
  class ObjectDeleter
  {
    public:
      ObjectDeleter(OwnerType owner = OwnerType(), Optional<const AllocationCallbacks> allocator = nullptr)
        : m_owner(owner)
        , m_allocator(allocator)
      {}

      OwnerType getOwner() const { return m_owner; }
      Optional<const AllocationCallbacks> getAllocator() const { return m_allocator; }

    protected:
      template <typename T>
      void destroy(T t)
      {
        m_owner.destroy(t, m_allocator);
      }

    private:
      OwnerType m_owner;
      Optional<const AllocationCallbacks> m_allocator;
  };

  class NoParent;

  template <>
  class ObjectDeleter<NoParent>
  {
  public:
    ObjectDeleter( Optional<const AllocationCallbacks> allocator = nullptr )
      : m_allocator( allocator )
    {}

    Optional<const AllocationCallbacks> getAllocator() const { return m_allocator; }

  protected:
    template <typename T>
    void destroy(T t)
    {
      t.destroy( m_allocator );
    }

  private:
    Optional<const AllocationCallbacks> m_allocator;
  };

  template <typename OwnerType, typename PoolType>
  class PoolDeleter
  {
    public:
      PoolDeleter(OwnerType owner = OwnerType(), PoolType pool = PoolType())
        : m_owner(owner)
        , m_pool(pool)
      {}

      OwnerType getOwner() const { return m_owner; }
      PoolType getPool() const { return m_pool; }

    protected:
      template <typename T>
      void destroy(T t)
      {
        m_owner.free(m_pool, t);
      }

    private:
      OwnerType m_owner;
      PoolType m_pool;
  };

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


bool beginsWith(std::string const& text, std::string const& prefix);
void checkAttributes(std::map<std::string, std::string> const& attributes, int line, std::map<std::string, std::set<std::string>> const& required, std::map<std::string, std::set<std::string>> const& optional);
void checkElements(std::vector<tinyxml2::XMLElement const*> const& elements, std::set<std::string> const& values);
void checkEmptyElement(tinyxml2::XMLElement const* element);
void checkOrderedElements(std::vector<tinyxml2::XMLElement const*> const& elements, std::vector<std::string> const& values);
std::string createEnumValueName(std::string const& name, std::string const& prefix, std::string const& postfix, bool bitmask, std::string const& tag);
bool endsWith(std::string const& text, std::string const& postfix);
void enterProtect(std::ostream &os, std::string const& protect);
std::string extractTag(std::string const& name);
std::string findTag(std::string const& name, std::set<std::string> const& tags);
std::string generateEnumNameForFlags(std::string const& name);
std::map<std::string, std::string> getAttributes(tinyxml2::XMLElement const* element);
std::vector<tinyxml2::XMLElement const*> getChildElements(tinyxml2::XMLElement const* element);
bool isErrorEnum(std::string const& enumName);
void leaveProtect(std::ostream &os, std::string const& protect);
std::string readArraySize(tinyxml2::XMLNode const* node, std::string& name);
std::string startUpperCase(std::string const& input);
std::string startLowerCase(std::string const& input);
std::string strip(std::string const& value, std::string const& prefix, std::string const& postfix = std::string());
std::string stripErrorEnumPrefix(std::string const& enumName);
std::string stripPluralS(std::string const& name);
std::vector<std::string> tokenize(std::string tokenString, char separator);
std::string trim(std::string const& input);
std::string trimEnd(std::string const& input);
std::string toCamelCase(std::string const& value);
std::string toUpperCase(std::string const& name);
void writeFunctionHeaderName(std::ostream & os, std::string const& name, bool singular, bool unique);
void writeReinterpretCast(std::ostream & os, bool leadingConst, bool vulkanType, std::string const& type, bool trailingPointerToConst);
void writeStandardOrEnhanced(std::ostream & os, std::string const& standard, std::string const& enhanced);
void writeTypesafeCheck(std::ostream & os, std::string const& typesafeCheck);
void writeVersionCheck(std::ostream & os, std::string const& version);
#if !defined(NDEBUG)
void skipFeatureRequire(tinyxml2::XMLElement const* element);
void skipImplicitExternSyncParams(tinyxml2::XMLElement const* element);
void skipTypeEnum(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
void skipTypeInclude(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes);
#endif

bool beginsWith(std::string const& text, std::string const& prefix)
{
  return !prefix.empty() && text.substr(0, prefix.length()) == prefix;
}

// check the validity of an attributes map
// attributes : the map of name/value pairs of the encountered attributes
// line       : the line in the xml file where the attributes are listed
// required   : the required attributes, with a set of allowed values per attribute
// optional   : the optional attributes, with a set of allowed values per attribute
void checkAttributes(std::map<std::string, std::string> const& attributes, int line, std::map<std::string, std::set<std::string>> const& required, std::map<std::string, std::set<std::string>> const& optional)
{
  std::stringstream ss;
  ss << line;
  std::string lineNumber = ss.str();

  // check if all required attributes are included and if there is a set of allowed values, check if the actual value is part of that set
  for (auto const& r : required)
  {
    auto attributesIt = attributes.find(r.first);
    if (attributesIt == attributes.end())
    {
      assert(false);
      throw std::runtime_error("Spec error on line " + lineNumber + ": missing attribute <" + r.first + ">");
    }
    if (!r.second.empty() && (r.second.find(attributesIt->second) == r.second.end()))
    {
      assert(false);
      throw std::runtime_error("Spec error on line " + lineNumber + ": unexpected attribute value <" + attributesIt->second + "> in attribute <" + r.first + ">");
    }
  }
  // check if all not required attributes or optional, and if there is a set of allowed values, check if the actual value is part of that set
  for (auto const& a : attributes)
  {
    if (required.find(a.first) == required.end())
    {
      auto optionalIt = optional.find(a.first);
      if (optionalIt == optional.end())
      {
        assert(false);
        throw std::runtime_error("Spec error on line " + lineNumber + ": unexpected attribute <" + a.first + ">");
      }
      if (!optionalIt->second.empty())
      {
        std::vector<std::string> values = tokenize(a.second, ',');
        for (auto const& v : values)
        {
          if (optionalIt->second.find(v) == optionalIt->second.end())
          {
            assert(false);
            throw std::runtime_error("Spec error on line " + lineNumber + ": unexpected attribute value <" + v + "> in attribute <" + a.first + ">");
          }
        }
      }
    }
  }
}

void checkElements(std::vector<tinyxml2::XMLElement const*> const& elements, std::set<std::string> const& values)
{
  for (auto e : elements)
  {
    if (values.find(e->Value()) == values.end())
    {
      std::stringstream ss;
      ss << e->GetLineNum();
      std::string lineNumber = ss.str();
      assert(false);
      throw std::runtime_error("Spec error on line " + lineNumber + ": unexpected element value <" + e->Value() + ">");
    }
  }
}

void checkEmptyElement(tinyxml2::XMLElement const* element)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), {}, {});
  checkElements(getChildElements(element), {});
}

void checkOrderedElements(std::vector<tinyxml2::XMLElement const*> const& elements, std::vector<std::string> const& values)
{
  for (size_t i = 0; i < elements.size(); i++)
  {
    std::stringstream ss;
    ss << elements[i]->GetLineNum();
    std::string lineNumber = ss.str();

    if (values.size() <= i)
    {
      assert(false);
      throw std::runtime_error("Spec error on line " + lineNumber + ": unexpected surplus element <" + elements[i]->Value() + ">");
    }
    if (values[i] != elements[i]->Value())
    {
      assert(false);
      throw std::runtime_error("Spec error on line " + lineNumber + ": unexpected element <" + elements[i]->Value() + ">, expected <" + values[i] + ">");
    }
  }
}

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

bool endsWith(std::string const& text, std::string const& postfix)
{
  return !postfix.empty() && (postfix.length() < text.length()) && (text.substr(text.length() - postfix.length()) == postfix);
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

std::map<std::string, std::string> getAttributes(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes;
  for (auto attribute = element->FirstAttribute(); attribute; attribute = attribute->Next())
  {
    assert(attributes.find(attribute->Name()) == attributes.end());
    attributes[attribute->Name()] = attribute->Value();
  }
  return attributes;
}

std::vector<tinyxml2::XMLElement const*> getChildElements(tinyxml2::XMLElement const* element)
{
  std::vector<tinyxml2::XMLElement const*> childElements;
  for (tinyxml2::XMLElement const* childElement = element->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement())
  {
    childElements.push_back(childElement);
  }
  return childElements;
}

bool isErrorEnum(std::string const& enumName)
{
  return (enumName.substr(0, 6) == "eError");
}

void leaveProtect(std::ostream &os, std::string const& protect)
{
  if (!protect.empty())
  {
    os << "#endif /*" << protect << "*/" << std::endl;
  }
}

std::string readArraySize(tinyxml2::XMLNode const* node, std::string& name)
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
      assert(node->Value());
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

std::string startUpperCase(std::string const& input)
{
  return static_cast<char>(toupper(input[0])) + input.substr(1);
}

std::string startLowerCase(std::string const& input)
{
  return input.empty() ? "" : static_cast<char>(tolower(input[0])) + input.substr(1);
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

std::string stripErrorEnumPrefix(std::string const& enumName)
{
  assert(isErrorEnum(enumName));
  return strip(enumName, "eError");
}

std::string stripPluralS(std::string const& name)
{
  std::string strippedName(name);
  size_t pos = strippedName.rfind('s');
  assert(pos != std::string::npos);
  strippedName.erase(pos, 1);
  return strippedName;
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

std::string trim(std::string const& input)
{
  std::string result = input;
  result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](char c) { return !std::isspace(c); }));
  result.erase(std::find_if(result.rbegin(), result.rend(), [](char c) { return !std::isspace(c); }).base(), result.end());
  return result;
}

std::string trimEnd(std::string const& input)
{
  std::string result = input;
  result.erase(std::find_if(result.rbegin(), result.rend(), [](char c) { return !std::isspace(c); }).base(), result.end());
  return result;
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
      if ((value[i - 1] == '_') || isdigit(value[i - 1]))
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
  std::string convertedName;

  for (size_t i = 0; i<name.length(); i++)
  {
    if (isupper(name[i]) && ((i == 0) || islower(name[i - 1]) || isdigit(name[i - 1])))
    {
      convertedName.push_back('_');
    }
    convertedName.push_back(toupper(name[i]));
  }
  return convertedName;
}

void writeFunctionHeaderName(std::ostream & os, std::string const& name, bool singular, bool unique)
{
  os << (singular ? stripPluralS(name) : name);
  if (unique)
  {
    os << "Unique";
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

void writeVersionCheck(std::ostream & os, std::string const& version)
{
  os << "static_assert( VK_HEADER_VERSION == " << version << " , \"Wrong VK_HEADER_VERSION!\" );" << std::endl
    << std::endl;
}

#if !defined(NDEBUG)
void skipFeatureRequire(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), {}, { { "name",{} } });
  checkElements(getChildElements(element), {});
}

void skipImplicitExternSyncParams(tinyxml2::XMLElement const* element)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), {}, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkOrderedElements(children, { "param" });
  checkEmptyElement(children[0]);
}

void skipTypeEnum(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { { "category",{ "enum" } } }, { { "alias", {} }, { "name",{} } });
  checkElements(getChildElements(element), {});
}

void skipTypeInclude(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), { { "category",{ "include" } } }, { { "name",{} } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "name" });

  for (auto child : children)
  {
    checkEmptyElement(child);
  }
}
#endif

template <typename T>
void VulkanHppGenerator::checkAlias(std::map<std::string, T> const& data, std::string const& name, int line)
{
  if (data.find(name) == data.end())
  {
    std::stringstream ss;
    ss << line;
    std::string lineNumber = ss.str();

    assert(false);
    throw std::runtime_error("Spec error on line " + lineNumber + ": missing alias <" + name + ">");
  }
}

bool VulkanHppGenerator::containsUnion(std::string const& type, std::map<std::string, StructData> const& structs)
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

std::map<std::string, std::string> VulkanHppGenerator::createDefaults()
{
  std::map<std::string, std::string> defaultValues;
  for (auto dependency : m_dependencies)
  {
    assert(defaultValues.find(dependency.name) == defaultValues.end());
    switch (dependency.category)
    {
    case DependencyData::Category::BITMASK:
    case DependencyData::Category::HANDLE:
    case DependencyData::Category::STRUCT:
    case DependencyData::Category::UNION:        // just call the default constructor for bitmasks, handles, structs, and unions (which are mapped to classes)
      defaultValues[dependency.name] = dependency.name + "()";
      break;
    case DependencyData::Category::COMMAND:    // commands should never be asked for defaults
      break;
    case DependencyData::Category::ENUM:
      assert(m_enums.find(dependency.name) != m_enums.end());
      setDefault(dependency.name, defaultValues, m_enums.find(dependency.name)->second);
      break;
    case DependencyData::Category::FUNC_POINTER: // func_pointers default to nullptr
      defaultValues[dependency.name] = "nullptr";
      break;
    case DependencyData::Category::REQUIRED:     // all required default to "0"
    case DependencyData::Category::SCALAR:       // all scalars default to "0"
      defaultValues[dependency.name] = "0";
      break;
    default:
      assert(false);
      break;
    }
  }
  return defaultValues;
}

void VulkanHppGenerator::determineEnhancedReturnType(CommandData & commandData)
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

void VulkanHppGenerator::determineReducedName(CommandData & commandData)
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

void VulkanHppGenerator::determineReturnParam(CommandData & commandData)
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

void VulkanHppGenerator::determineSkippedParams(CommandData & commandData)
{
  // the size-parameters of vector parameters are not explicitly used in the enhanced API
  std::for_each(commandData.vectorParams.begin(), commandData.vectorParams.end(), [&commandData](std::pair<size_t, size_t> const& vp) { if (vp.second != ~0) commandData.skippedParams.insert(vp.second); });
  // and the return parameter is also skipped
  if (commandData.returnParam != ~0)
  {
    commandData.skippedParams.insert(commandData.returnParam);
  }
}

void VulkanHppGenerator::determineTemplateParam(CommandData & commandData)
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

void VulkanHppGenerator::determineVectorParams(CommandData & commandData)
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

std::string VulkanHppGenerator::generateCall(CommandData const& commandData, bool firstCall, bool singular)
{
  std::ostringstream call;
  writeCall(call, commandData, firstCall, singular);
  return call.str();
}

std::string const& VulkanHppGenerator::getTypesafeCheck() const
{
  return m_typesafeCheck;
}

std::string const& VulkanHppGenerator::getVersion() const
{
  return m_version;
}

std::string const& VulkanHppGenerator::getVulkanLicenseHeader() const
{
  return m_vulkanLicenseHeader;
}

void VulkanHppGenerator::linkCommandToHandle(CommandData & commandData)
{
  // first, find the handle named like the type of the first argument
  // if there is no such handle, look for the unnamed "handle", that gathers all the functions not tied to a specific handle
  assert(!commandData.params.empty());
  std::map<std::string, HandleData>::iterator hit = m_handles.find(commandData.params[0].pureType);
  if (hit == m_handles.end())
  {
    hit = m_handles.find("");
  }
  assert(hit != m_handles.end());

  // put the command into the handle's list of commands, and store the handle in the commands className
  hit->second.commands.push_back(commandData.fullName);
  commandData.className = hit->first;

  // add the dependencies of the command to the dependencies of the handle
  DependencyData const& commandDD = m_dependencies.back();
  std::list<DependencyData>::iterator handleDD = std::find_if(m_dependencies.begin(), m_dependencies.end(), [hit](DependencyData const& dd) { return dd.name == hit->first; });
  assert((handleDD != m_dependencies.end()) || hit->first.empty());
  if (handleDD != m_dependencies.end())
  {
    std::copy_if(commandDD.dependencies.begin(), commandDD.dependencies.end(), std::inserter(handleDD->dependencies, handleDD->dependencies.end()), [hit](std::string const& d) { return d != hit->first; });
  }
}

bool VulkanHppGenerator::readCommandParam(tinyxml2::XMLElement const* element, std::set<std::string> & dependencies, std::vector<ParamData> & params)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), {}, { { "externsync",{} },{ "len",{} },{ "noautovalidity",{ "true" } },{ "optional",{ "false", "true" } } });
  checkElements(getChildElements(element), { "name", "type" });

  ParamData param;

  bool isTwoStep = false;
  auto lenAttribute = attributes.find("len");
  if (lenAttribute != attributes.end())
  {
    param.len = lenAttribute->second;
    auto pit = std::find_if(params.begin(), params.end(), [&param](ParamData const& pd) { return param.len == pd.name; });
    if (pit != params.end())
    {
      isTwoStep = (pit->type.find('*') != std::string::npos);
    }
  }

  // get the type of the parameter, and put it into the list of dependencies
  tinyxml2::XMLNode const* child = readCommandParamType(element->FirstChild(), param);
  dependencies.insert(param.pureType);

  assert(child->ToElement());
  tinyxml2::XMLElement const* nameElement = child->ToElement();
  checkEmptyElement(nameElement);
  param.name = child->ToElement()->GetText();

  param.arraySize = readArraySize(child, param.name);

  auto optionalAttribute = attributes.find("optional");
  param.optional = (optionalAttribute != attributes.end()) && (optionalAttribute->second == "true");

  params.push_back(param);

  assert(!isTwoStep || (param.type.substr(0, 6) != "const "));
  return isTwoStep;
}

tinyxml2::XMLNode const* VulkanHppGenerator::readCommandParamType(tinyxml2::XMLNode const* node, ParamData& param)
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
  assert(node->ToElement());
  tinyxml2::XMLElement const* typeElement = node->ToElement();
  checkEmptyElement(typeElement);
  std::string type = strip(node->ToElement()->GetText(), "Vk");
  param.unchangedType = param.type + node->ToElement()->GetText();
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
    param.unchangedType += value;
    node = node->NextSibling();
  }

  return node;
}

void VulkanHppGenerator::readCommands(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), {}, { { "comment",{} } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "command" });

  for (auto child : children)
  {
    readCommandsCommand(child);
  }
}

void VulkanHppGenerator::readCommandsCommand(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), {},
  { { "alias", {} },
    { "cmdbufferlevel",{ "primary", "secondary" } },
    { "comment",{} },
    { "errorcodes",{} },
    { "name", {} },
    { "pipeline",{ "compute", "graphics", "transfer" } },
    { "queues",{ "compute", "graphics", "sparse_binding", "transfer" } },
    { "renderpass",{ "both", "inside", "outside" } },
    { "successcodes",{} }
  });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);

  CommandData commandData;
  auto aliasIt = attributes.find("alias");
  if (aliasIt != attributes.end())
  {
    // for command aliases, create a copy of the aliased command
    checkAttributes(attributes, element->GetLineNum(), { { "alias",{} },{ "name",{} } }, {});    // re-check on alias type!
    checkElements(children, {});

    std::string alias = startLowerCase(strip(aliasIt->second, "vk"));
    checkAlias(m_commands, alias, element->GetLineNum());

    auto commandsIt = m_commands.find(alias);
    assert(commandsIt != m_commands.end());
    commandData = commandsIt->second;
    commandData.fullName = startLowerCase(strip(attributes.find("name")->second, "vk"));
    determineReducedName(commandData);
    linkCommandToHandle(commandData);

    // add a DependencyData to this name
    m_dependencies.push_back(DependencyData(DependencyData::Category::COMMAND, commandData.fullName));
    m_dependencies.back().dependencies.insert(alias);
  }
  else
  {
    checkElements(children, { "implicitexternsyncparams", "param", "proto" });

    // read the success codes
    auto successcodesAttribute = attributes.find("successcodes");
    if (successcodesAttribute != attributes.end())
    {
      commandData.successCodes = tokenize(successcodesAttribute->second, ',');
      for (auto & code : commandData.successCodes)
      {
        std::string tag = findTag(code, m_tags);
        // on each success code: prepend 'e', strip "VK_" and a tag, convert it to camel case, and add the tag again
        code = std::string("e") + toCamelCase(strip(code, "VK_", tag)) + tag;
      }
    }

    for (auto child : children)
    {
      std::string value = child->Value();
      if (value == "param")
      {
        commandData.twoStep |= readCommandParam(child, m_dependencies.back().dependencies, commandData.params);
      }
      else if (value == "proto")
      {
        readCommandProto(child, commandData.returnType, commandData.unchangedReturnType, commandData.fullName);
      }
#if !defined(NDEBUG)
      else
      {
        assert(value == "implicitexternsyncparams");
        skipImplicitExternSyncParams(child);
      }
#endif
    }

    determineReducedName(commandData);
    linkCommandToHandle(commandData);
    registerDeleter(commandData);
    determineVectorParams(commandData);
    determineReturnParam(commandData);
    determineTemplateParam(commandData);
    determineEnhancedReturnType(commandData);
    determineSkippedParams(commandData);
  }

  // insert the commandData into the commands-map,
  assert(m_commands.find(commandData.fullName) == m_commands.end());
  m_commands.insert(std::make_pair(commandData.fullName, commandData));
}

void VulkanHppGenerator::readCommandProto(tinyxml2::XMLElement const* element, std::string & returnType, std::string & unchangedReturnType, std::string & fullName)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), {}, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkOrderedElements(children, { "type", "name" });

  // get return type and name of the command
  returnType = strip(children[0]->GetText(), "Vk");
  unchangedReturnType = children[0]->GetText();
  fullName = startLowerCase(strip(children[1]->GetText(), "vk"));

  // add an empty DependencyData to this name
  m_dependencies.push_back(DependencyData(DependencyData::Category::COMMAND, fullName));
}

void VulkanHppGenerator::readComment(tinyxml2::XMLElement const* element)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), {}, {});
  checkElements(getChildElements(element), {});

  assert(element->GetText());
  std::string text = element->GetText();
  if (text.find("\nCopyright") == 0)
  {
    assert(m_vulkanLicenseHeader.empty());
    m_vulkanLicenseHeader = text;

    // erase the part after the Copyright text
    size_t pos = m_vulkanLicenseHeader.find("\n\n-----");
    assert(pos != std::string::npos);
    m_vulkanLicenseHeader.erase(pos);

    // replace any '\n' with "\n// "
    for (size_t pos = m_vulkanLicenseHeader.find('\n'); pos != std::string::npos; pos = m_vulkanLicenseHeader.find('\n', pos + 1))
    {
      m_vulkanLicenseHeader.replace(pos, 1, "\n// ");
    }

    // and add a little message on our own
    m_vulkanLicenseHeader += "\n\n// This header is generated from the Khronos Vulkan XML API Registry.";
  }

  m_vulkanLicenseHeader.erase(m_vulkanLicenseHeader.begin(), std::find_if(m_vulkanLicenseHeader.begin(), m_vulkanLicenseHeader.end(), [](char c) { return !std::isspace(c); }));
}

void VulkanHppGenerator::readDisabledExtensionRequire(tinyxml2::XMLElement const* element)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), {}, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "command", "enum", "type" });

  for (auto child : children)
  {
    checkElements(getChildElements(child), {});

    std::string value = child->Value();
    if ((value == "command") || (value == "type"))
    {
      std::map<std::string, std::string> attributes = getAttributes(child);
      checkAttributes(attributes, element->GetLineNum(), { { "name",{} } }, {});

      // disable a command or a type !
      auto nameAttribute = attributes.find("name");
      std::string name = (value == "command") ? startLowerCase(strip(nameAttribute->second, "vk")) : strip(nameAttribute->second, "Vk");

      // search this name in the dependencies list and remove it
      std::list<DependencyData>::const_iterator depIt = std::find_if(m_dependencies.begin(), m_dependencies.end(), [&name](DependencyData const& dd) { return(dd.name == name); });
      assert(depIt != m_dependencies.end());
      m_dependencies.erase(depIt);

      // erase it from all dependency sets
      for (auto & dep : m_dependencies)
      {
        dep.dependencies.erase(name);
      }

      if (value == "command")
      {
        // first unlink the command from its class
        auto commandsIt = m_commands.find(name);
        assert(commandsIt != m_commands.end());
        assert(!commandsIt->second.className.empty());
        auto handlesIt = m_handles.find(commandsIt->second.className);
        assert(handlesIt != m_handles.end());
        auto it = std::find(handlesIt->second.commands.begin(), handlesIt->second.commands.end(), name);
        assert(it != handlesIt->second.commands.end());
        handlesIt->second.commands.erase(it);

        // then remove the command
        m_commands.erase(name);
      }
      else
      {
        // a type simply needs to be removed from the structs and vkTypes sets
        assert((m_structs.find(name) != m_structs.end()) && (m_vkTypes.find(name) != m_vkTypes.end()));
        m_structs.erase(name);
        m_vkTypes.erase(name);
      }
    }
    else
    {
      assert(value == "enum");
      std::map<std::string, std::string> attributes = getAttributes(child);
      checkAttributes(attributes, child->GetLineNum(), { { "name",{} } }, { { "extends",{} },{ "offset",{} },{ "value",{} } });
    }
  }
}

void VulkanHppGenerator::readEnums(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "name",{} } }, { { "comment",{} },{ "type",{ "bitmask", "enum" } } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "comment", "enum", "unused" });

  std::string name = strip(attributes.find("name")->second, "Vk");

  if (name == "API Constants")
  {
    for (auto child : children)
    {
      assert(strcmp(child->Value(), "enum") == 0);
      readEnumsConstant(child);
    }
  }
  else
  {
    checkAttributes(attributes, element->GetLineNum(), { { "name",{} },{ "type",{ "bitmask", "enum" } } }, { { "comment",{} } });   // re-check with type as required

                                                                                                                                    // add an empty DependencyData on this name into the dependencies list
    m_dependencies.push_back(DependencyData(DependencyData::Category::ENUM, name));

    // ad an empty EnumData on this name into the enums map
    std::map<std::string, EnumData>::iterator it = m_enums.insert(std::make_pair(name, EnumData(name))).first;

    if (name == "Result")
    {
      // special handling for VKResult, as its enums just have VK_ in common
      it->second.prefix = "VK_";
    }
    else
    {
      std::string type = attributes.find("type")->second;
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
      for (std::set<std::string>::const_iterator tit = m_tags.begin(); tit != m_tags.end(); ++tit)
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

    // read the names of the enum values
    for (auto child : children)
    {
      std::string value = child->Value();
      if (value == "enum")
      {
        readEnumsEnum(child, it->second, "");
      }
#if !defined(NDEBUG)
      else
      {
        assert((value == "comment") || (value == "unused"));
      }
#endif
    }

    // add this enum to the set of Vulkan data types
    assert(m_vkTypes.find(name) == m_vkTypes.end());
    m_vkTypes.insert(name);
  }
}

void VulkanHppGenerator::readEnumsEnum(tinyxml2::XMLElement const* element, EnumData & enumData, std::string const& tag)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "name",{} } }, { { "bitpos",{} },{ "comment",{} },{ "value",{} } });
  assert((attributes.find("bitpos") != attributes.end()) + (attributes.find("value") != attributes.end()) == 1);
  checkElements(getChildElements(element), {});
  enumData.addEnumValue(attributes.find("name")->second, tag, m_nameMap);
}

void VulkanHppGenerator::readEnumsConstant(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "name",{} } }, { { "alias", {}}, { "comment",{} }, { "value",{} } });
  checkElements(getChildElements(element), {});
  std::string name = attributes.find("name")->second;
  assert(m_constants.find(name) == m_constants.end());

  auto aliasIt = attributes.find("alias");
  if (aliasIt != attributes.end())
  {
    checkAttributes(attributes, element->GetLineNum(), { {"alias", {}}, { "name", {}} }, {});    // re-check on alias type
    checkAlias(m_constants, aliasIt->second, element->GetLineNum());
    m_constants[name] = m_constants.find(aliasIt->second)->second;
  }
  else
  {
    checkAttributes(attributes, element->GetLineNum(), { { "name",{} }, { "value", {}} }, { {"comment", {} } });    // re-check on non-alias type
    m_constants[name] = attributes.find("value")->second;
  }
}

void VulkanHppGenerator::readExtensionCommand(tinyxml2::XMLElement const* element, std::string const& protect)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "name",{} } }, {});
  checkElements(getChildElements(element), {});

  // just add the protect string to the CommandData
  if (!protect.empty())
  {
    std::string name = startLowerCase(strip(attributes.find("name")->second, "vk"));
    std::map<std::string, CommandData>::iterator cit = m_commands.find(name);
    assert(cit != m_commands.end());
    cit->second.protect = protect;
  }
}

void VulkanHppGenerator::readExtensionEnum(tinyxml2::XMLElement const* element, std::string const& tag)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(),
  {
    { "name", {} }
  },
  {
    { "alias", {} },
    { "bitpos", {} },
    { "comment", {} },
    { "dir", { "-" } },
    { "extends", {} },
    { "extnumber", {} },
    { "offset", {} },
    { "value", {} }
  });
  checkElements(getChildElements(element), {});

  // TODO process enums which don't extend existing enums
  auto extendsIt = attributes.find("extends");
  if (extendsIt != attributes.end())
  {
    std::string extends = strip(extendsIt->second, "Vk");
    auto enumIt = m_enums.find(extends);
    assert(enumIt != m_enums.end());

    auto aliasIt = attributes.find("alias");
    if (aliasIt != attributes.end())
    {
      checkAttributes(attributes, element->GetLineNum(), { { "alias", {} }, { "extends", {} }, { "name", {} } }, {});
      std::string alias = createEnumValueName(aliasIt->second, enumIt->second.prefix, enumIt->second.postfix, enumIt->second.bitmask, tag);
      auto evdIt = std::find_if(enumIt->second.values.begin(), enumIt->second.values.end(), [&alias](EnumValueData const& evd) { return evd.name == alias; });
      assert(evdIt != enumIt->second.values.end());
      evdIt->alias = createEnumValueName(attributes.find("name")->second, enumIt->second.prefix, enumIt->second.postfix, enumIt->second.bitmask, tag);
    }
    else
    {
      assert((attributes.find("bitpos") != attributes.end()) + (attributes.find("offset") != attributes.end()) + (attributes.find("value") != attributes.end()) == 1);
      enumIt->second.addEnumValue(attributes.find("name")->second, tag, m_nameMap);
    }
  }
}

void VulkanHppGenerator::readExtensionRequire(tinyxml2::XMLElement const* element, std::string const& protect, std::string const& tag)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), {}, { { "extension",{} },{ "feature",{} } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "command", "comment", "enum", "type" });

  for (auto child : children)
  {
    std::string value = child->Value();

    if (value == "command")
    {
      readExtensionCommand(child, protect);
    }
    else if (value == "enum")
    {
      readExtensionEnum(child, tag);
    }
    else if (value == "type")
    {
      readExtensionType(child, protect);
    }
#if !defined(NDEBUG)
    else
    {
      assert(value == "comment");
      checkEmptyElement(child);
    }
#endif
  }
}

void VulkanHppGenerator::readExtensions(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "comment",{} } }, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "extension" });

  for (auto child : children)
  {
    readExtensionsExtension(child);
  }
}

void VulkanHppGenerator::readExtensionsExtension(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(),
  {
    { "name",{} },
    { "number",{} },
    { "supported",{ "disabled", "vulkan" } }
  },
  {
    { "author",{} },
    { "comment", {} },
    { "contact",{} },
    { "platform",{} },
    { "protect",{} },
    { "requires",{} },
    { "type",{ "device", "instance" } }
  });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "require" });

  if (attributes.find("supported")->second == "disabled")
  {
    // kick out all the disabled stuff we've read before !!
    for (tinyxml2::XMLElement const* child = element->FirstChildElement(); child; child = child->NextSiblingElement())
    {
      assert(strcmp(child->Value(), "require") == 0);
      readDisabledExtensionRequire(child);
    }
  }
  else
  {
    std::string name = attributes.find("name")->second;
    std::string tag = extractTag(name);
    assert(m_tags.find(tag) != m_tags.end());

    auto protectAttribute = attributes.find("protect");
    auto platformAttribute = attributes.find("platform");
    std::string protect;
    if (protectAttribute != attributes.end())
    {
      protect = protectAttribute->second;
    }
    else if (platformAttribute != attributes.end())
    {
      auto authorAttribute = attributes.find("author");
      assert(authorAttribute != attributes.end());
      protect = "VK_USE_PLATFORM_" + toUpperCase(platformAttribute->second) + "_" + authorAttribute->second;
    }

#if !defined(NDEBUG)
    assert(m_extensions.find(name) == m_extensions.end());
    ExtensionData & extension = m_extensions.insert(std::make_pair(name, ExtensionData())).first->second;
    extension.protect = protect;
    auto requiresAttribute = attributes.find("requires");
    if (requiresAttribute != attributes.end())
    {
      extension.requires = tokenize(requiresAttribute->second, ',');
    }
#endif

    for (auto child : children)
    {
      readExtensionRequire(child, protect, tag);
    }
  }
}

void VulkanHppGenerator::readExtensionType(tinyxml2::XMLElement const* element, std::string const& protect)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "name",{} } }, {});
  checkElements(getChildElements(element), {});

  // add the protect-string to the appropriate type: enum, flag, handle, scalar, or struct
  if (!protect.empty())
  {
    std::string name = strip(attributes.find("name")->second, "Vk");
    std::map<std::string, BitmaskData>::iterator bitmasksIt = m_bitmasks.find(name);
    if (bitmasksIt != m_bitmasks.end())
    {
      bitmasksIt->second.protect = protect;

      // if the enum of this flags is auto-generated, protect it as well
      std::string enumName = generateEnumNameForFlags(name);
      std::map<std::string, EnumData>::iterator enumsIt = m_enums.find(enumName);
      assert(enumsIt != m_enums.end());
      if (enumsIt->second.values.empty())
      {
        enumsIt->second.protect = protect;
      }
    }
    else
    {
      std::map<std::string, EnumData>::iterator eit = m_enums.find(name);
      if (eit != m_enums.end())
      {
        eit->second.protect = protect;
      }
      else
      {
        std::map<std::string, HandleData>::iterator hait = m_handles.find(name);
        if (hait != m_handles.end())
        {
          hait->second.protect = protect;
        }
        else
        {
          std::map<std::string, ScalarData>::iterator scit = m_scalars.find(name);
          if (scit != m_scalars.end())
          {
            scit->second.protect = protect;
          }
          else
          {
            std::map<std::string, StructData>::iterator stit = m_structs.find(name);
            if (stit != m_structs.end())
            {
              stit->second.protect = protect;
            }
            else
            {
              assert(m_defines.find(name) != m_defines.end());
            }
          }
        }
      }
    }
  }
}

void VulkanHppGenerator::readFeature(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "api",{ "vulkan" } },{ "comment",{} },{ "name",{} },{ "number",{} } }, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "require" });

  for (auto child : children)
  {
    readFeatureRequire(child);
  }
}

void VulkanHppGenerator::readFeatureRequire(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), {}, { { "comment",{} } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "command", "comment", "enum", "type" });

  for (auto child : children)
  {
    std::string value = child->Value();
    if (value == "enum")
    {
      readFeatureRequireEnum(child);
    }
#if !defined(NDEBUG)
    else
    {
      assert((value == "command") || (value == "comment") || (value == "type"));
      skipFeatureRequire(child);
    }
#endif
  }
}

void VulkanHppGenerator::readFeatureRequireEnum(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(),
  {
    { "name",{} }
  },
  {
    { "bitpos",{} },
    { "comment",{} },
    { "dir", { "-" } },
    { "extends",{} },
    { "extnumber", {} },
    { "offset", {} },
    { "value",{} }
  });
  checkElements(getChildElements(element), {});

  auto extendsAttribute = attributes.find("extends");
  if (extendsAttribute != attributes.end())
  {
    assert(strncmp(extendsAttribute->second.c_str(), "Vk", 2) == 0);
    std::string extends = strip(extendsAttribute->second, "Vk");
    auto enumIt = m_enums.find(extends);
    assert(enumIt != m_enums.end());
    enumIt->second.addEnumValue(attributes.find("name")->second, "", m_nameMap);
  }
}

void VulkanHppGenerator::readTags(tinyxml2::XMLElement const* element)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), { { "comment",{} } }, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "tag" });

  for (auto child : children)
  {
    std::string value = child->Value();
    assert(value == "tag");
    readTag(child);
  }
}

void VulkanHppGenerator::readTag(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "author",{} },{ "contact",{} },{ "name",{} } }, {});
  checkElements(getChildElements(element), {});

  for (auto const& attribute : attributes)
  {
    std::string name = attribute.first;
    if (name == "name")
    {
      std::string value = attribute.second;
      m_tags.insert(value);
    }
    else
    {
      assert((name == "author") || (name == "contact"));
    }
  }
}

void VulkanHppGenerator::readType(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);

  auto categoryIt = attributes.find("category");
  if (categoryIt != attributes.end())
  {
    if (categoryIt->second == "basetype")
    {
      readTypeBasetype(element, attributes);
    }
    else if (categoryIt->second == "bitmask")
    {
      readTypeBitmask(element, attributes);
    }
    else if (categoryIt->second == "define")
    {
      readTypeDefine(element, attributes);
    }
    else if (categoryIt->second == "funcpointer")
    {
      readTypeFuncpointer(element, attributes);
    }
    else if (categoryIt->second == "handle")
    {
      readTypeHandle(element, attributes);
    }
    else if (categoryIt->second == "struct")
    {
      readTypeStruct(element, false, attributes);
    }
    else if (categoryIt->second == "union")
    {
      readTypeStruct(element, true, attributes);
    }
#if !defined(NDEBUG)
    else if (categoryIt->second == "enum")
    {
      skipTypeEnum(element, attributes);
    }
    else if (categoryIt->second == "include")
    {
      skipTypeInclude(element, attributes);
    }
    else
#else
    else if ((categoryIt->second != "enum") && (categoryIt->second != "include"))
#endif
    {
      std::stringstream ss;
      ss << element->GetLineNum();
      std::string lineNumber = ss.str();

      assert(false);
      throw std::runtime_error("Spec error on line " + lineNumber + ": unknown category <" + categoryIt->second + ">");
    }
  }
  else
  {
    assert(attributes.find("name") != attributes.end());
    readTypeName(element, attributes);
  }
}

void VulkanHppGenerator::readTypeBasetype(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { { "category",{ "basetype" } } }, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkOrderedElements(children, { "type", "name" });
  checkEmptyElement(children[0]);
  checkEmptyElement(children[1]);

  std::string type = children[0]->GetText();
  assert((type == "uint32_t") || (type == "uint64_t"));

  std::string name = strip(children[1]->GetText(), "Vk");

  // skip "Flags",
  if (name != "Flags")
  {
    m_dependencies.push_back(DependencyData(DependencyData::Category::SCALAR, name));
    m_dependencies.back().dependencies.insert(type);
  }
  else
  {
    assert(type == "uint32_t");
  }
}

void VulkanHppGenerator::readTypeBitmask(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { { "category", { "bitmask" } } }, { { "alias", {} }, { "name", {}}, { "requires", {} } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);

  auto aliasIt = attributes.find("alias");
  if (aliasIt != attributes.end())
  {
    checkAttributes(attributes, element->GetLineNum(), { { "alias", {} }, { "category", {"bitmask"} }, { "name", {} } }, {});   // re-check on alias type!
    checkElements(children, {});

    std::string alias = strip(aliasIt->second, "Vk");
    checkAlias(m_bitmasks, alias, element->GetLineNum());

    std::string name = strip(attributes.find("name")->second, "Vk");

    auto bitmasksIt = m_bitmasks.find(alias);
    assert((bitmasksIt != m_bitmasks.end()) && bitmasksIt->second.alias.empty());
    bitmasksIt->second.alias = name;
  }
  else
  {
    checkOrderedElements(children, { "type", "name" });
    checkEmptyElement(children[0]);
    checkEmptyElement(children[1]);

    assert(strcmp(children[0]->GetText(), "VkFlags") == 0);

    std::string name = strip(children[1]->GetText(), "Vk");

    std::string requires;
    auto requiresIt = attributes.find("requires");
    if (requiresIt != attributes.end())
    {
      requires = strip(requiresIt->second, "Vk");
    }
    else
    {
      // Generate FlagBits name, add a DependencyData for that name, and add it to the list of enums and vulkan types
      requires = generateEnumNameForFlags(name);
      m_dependencies.push_back(DependencyData(DependencyData::Category::ENUM, requires));
      m_enums.insert(std::make_pair(requires, EnumData(requires, true)));
      m_vkTypes.insert(requires);
    }

    // add a DependencyData for the bitmask name, with the required type as its first dependency
    m_dependencies.push_back(DependencyData(DependencyData::Category::BITMASK, name));
    m_dependencies.back().dependencies.insert(requires);

    m_bitmasks.insert(std::make_pair(name, BitmaskData()));

    assert(m_vkTypes.find(name) == m_vkTypes.end());
    m_vkTypes.insert(name);
  }
}

void VulkanHppGenerator::readTypeDefine(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { { "category",{ "define" } } }, { { "name",{} } });

  auto nameIt = attributes.find("name");
  if (nameIt != attributes.end())
  {
    assert(!element->FirstChildElement());
    assert(nameIt->second == "VK_DEFINE_NON_DISPATCHABLE_HANDLE");

    // filter out the check for the different types of VK_DEFINE_NON_DISPATCHABLE_HANDLE
    std::string text = element->LastChild()->ToText()->Value();
    size_t start = text.find("#if defined(__LP64__)");
    size_t end = text.find_first_of("\r\n", start + 1);
    m_typesafeCheck = text.substr(start, end - start);
  }
  else if (element->GetText() && (trim(element->GetText()) == "struct"))
  {
    tinyxml2::XMLElement const* child = element->FirstChildElement();
    assert(child && (strcmp(child->Value(), "name") == 0) && child->GetText());
    m_defines.insert(child->GetText());
    m_dependencies.push_back(DependencyData(DependencyData::Category::REQUIRED, child->GetText()));
  }
  else
  {
    tinyxml2::XMLElement const* child = element->FirstChildElement();
    assert(child && !child->FirstAttribute() && (strcmp(child->Value(), "name") == 0) && child->GetText());
    std::string text = trim(child->GetText());
    if (text == "VK_HEADER_VERSION")
    {
      m_version = element->LastChild()->ToText()->Value();
    }
    // ignore all the other defines
    assert(!child->NextSiblingElement() || (child->NextSiblingElement() && !child->NextSiblingElement()->FirstAttribute() && (strcmp(child->NextSiblingElement()->Value(), "type") == 0) && !child->NextSiblingElement()->NextSiblingElement()));
  }
}

void VulkanHppGenerator::readTypeFuncpointer(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { { "category",{ "funcpointer" } } }, { { "requires",{} } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "name", "type" });
  assert(!children.empty());
  checkEmptyElement(children[0]);

  assert((strcmp(children[0]->Value(), "name") == 0) && children[0]->GetText());
  m_dependencies.push_back(DependencyData(DependencyData::Category::FUNC_POINTER, children[0]->GetText()));

#if !defined(NDEBUG)
  for (size_t i = 1; i < children.size(); i++)
  {
    checkEmptyElement(children[i]);
  }
#endif
}

void VulkanHppGenerator::readTypeHandle(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { { "category",{ "handle" } } }, { { "alias",{} }, { "name",{} }, { "parent",{} } });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);

  auto aliasIt = attributes.find("alias");
  if (aliasIt != attributes.end())
  {
    checkAttributes(attributes, element->GetLineNum(), { { "alias",{} },{ "category",{ "handle" } },{ "name",{} } }, {});   // re-check on alias type!
    checkElements(children, {});

    std::string alias = strip(aliasIt->second, "Vk");
    checkAlias(m_handles, alias, element->GetLineNum());

    std::string name = strip(attributes.find("name")->second, "Vk");

    auto handlesIt = m_handles.find(alias);
    assert((handlesIt != m_handles.end()) && handlesIt->second.alias.empty());
    handlesIt->second.alias = name;
  }
  else
  {
    checkOrderedElements(children, { "type", "name" });
    checkEmptyElement(children[0]);
    checkEmptyElement(children[1]);

#if !defined(NDEBUG)
    std::string type = children[0]->GetText();
    assert((type.find("VK_DEFINE_HANDLE") == 0) || (type.find("VK_DEFINE_NON_DISPATCHABLE_HANDLE") == 0));
#endif

    std::string name = strip(children[1]->GetText(), "Vk");

    m_dependencies.push_back(DependencyData(DependencyData::Category::HANDLE, name));

    assert(m_vkTypes.find(name) == m_vkTypes.end());
    m_vkTypes.insert(name);
    assert(m_handles.find(name) == m_handles.end());
    m_handles.insert(std::make_pair(name, HandleData()));
  }
}

void VulkanHppGenerator::readTypeName(tinyxml2::XMLElement const* element, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(), { { "name",{} } }, { { "requires",{} } });
  checkElements(getChildElements(element), {});

  auto nameIt = attributes.find("name");
  assert(nameIt != attributes.end());
  m_dependencies.push_back(DependencyData(DependencyData::Category::REQUIRED, nameIt->second));
}

void VulkanHppGenerator::readTypes(tinyxml2::XMLElement const* element)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), { { "comment",{} } }, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "comment", "type" });

  for (auto child : children)
  {
    std::string value = child->Value();
    if (value == "type")
    {
      readType(child);
    }
#if !defined(NDEBUG)
    else
    {
      assert(value == "comment");
      checkEmptyElement(child);
    }
#endif
  }
}

void VulkanHppGenerator::readTypeStruct(tinyxml2::XMLElement const* element, bool isUnion, std::map<std::string, std::string> const& attributes)
{
  checkAttributes(attributes, element->GetLineNum(),
  {
    { "category",{ isUnion ? "union" : "struct" } },
    { "name",{} }
  },
  {
    { "alias", {} },
    { "comment",{} },
    { "returnedonly",{ "true" } },
    { "structextends",{} }
  });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "comment", "member" });

  auto aliasIt = attributes.find("alias");
  if (aliasIt != attributes.end())
  {
    checkAttributes(attributes, element->GetLineNum(), { { "alias", {}}, {"category", {"struct"}}, { "name", {}} }, {});    // re-check on alias type!

    std::string alias = strip(aliasIt->second, "Vk");
    checkAlias(m_structs, alias, element->GetLineNum());

    std::string name = strip(attributes.find("name")->second, "Vk");

    auto structsIt = m_structs.find(alias);
    assert((structsIt != m_structs.end()) && structsIt->second.alias.empty());
    structsIt->second.alias = name;
  }
  else
  {
    std::string name = strip(attributes.find("name")->second, "Vk");

    m_dependencies.push_back(DependencyData(isUnion ? DependencyData::Category::UNION : DependencyData::Category::STRUCT, name));

    assert(m_structs.find(name) == m_structs.end());
    std::map<std::string, StructData>::iterator it = m_structs.insert(std::make_pair(name, StructData())).first;
    it->second.returnedOnly = (attributes.find("returnedonly") != attributes.end());
    it->second.isUnion = isUnion;

    auto attributesIt = attributes.find("structextends");
    if (attributesIt != attributes.end())
    {
      std::vector<std::string> structExtends = tokenize(attributesIt->second, ',');
      for (auto const& s : structExtends)
      {
        assert(s.substr(0, 2) == "Vk");
        std::string strippedName = s.substr(2);
        it->second.structExtends.push_back(strippedName);
        m_extendedStructs.insert(strippedName);
      }
      assert(!it->second.structExtends.empty());
    }

    for (auto child : children)
    {
      assert(child->Value());
      std::string value = child->Value();
      if (value == "member")
      {
        readTypeStructMember(child, it->second);
      }
#if !defined(NDEBUG)
      else
      {
        assert(value == "comment");
        checkEmptyElement(child);
      }
#endif
    }

    assert(m_vkTypes.find(name) == m_vkTypes.end());
    m_vkTypes.insert(name);
  }
}

void VulkanHppGenerator::readTypeStructMember(tinyxml2::XMLElement const* element, StructData & structData)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), {},
  {
    { "altlen",{} },
    { "externsync",{ "true" } },
    { "len",{} },
    { "noautovalidity",{ "true" } },
    { "optional",{ "false", "true" } },
    { "values",{} }
  });
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "comment", "enum", "name", "type" });
  for (auto child : children)
  {
    checkEmptyElement(child);
  }

  structData.members.push_back(MemberData());
  MemberData & member = structData.members.back();

  auto valuesAttribute = attributes.find("values");
  if (valuesAttribute != attributes.end())
  {
    member.values = valuesAttribute->second;
  }

  tinyxml2::XMLNode const* child = element->FirstChild();
  assert(child);
  if (child->ToText())
  {
    std::string value = trim(child->Value());
    assert((value == "const") || (value == "struct"));
    member.type = value + " ";
    child = child->NextSibling();
    assert(child);
  }

  assert(child->ToElement());
  tinyxml2::XMLElement const* typeElement = child->ToElement();
  assert((strcmp(typeElement->Value(), "type") == 0) && typeElement->GetText());
  member.pureType = strip(typeElement->GetText(), "Vk");
  member.type += member.pureType;

  child = typeElement->NextSibling();
  assert(child);
  if (child->ToText())
  {
    std::string value = trimEnd(child->Value());
    assert((value == "*") || (value == "**") || (value == "* const*"));
    member.type += value;
    child = child->NextSibling();
  }

  m_dependencies.back().dependencies.insert(member.pureType);

  assert(child->ToElement());
  tinyxml2::XMLElement const* nameElement = child->ToElement();
  assert((strcmp(nameElement->Value(), "name") == 0) && nameElement->GetText());
  member.name = nameElement->GetText();

  member.arraySize = readArraySize(nameElement, member.name);
}

void VulkanHppGenerator::registerDeleter(CommandData const& commandData)
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
      assert(m_deleters.find(commandData.params[valueIndex].pureType) == m_deleters.end());
      m_deleters[commandData.params[valueIndex].pureType].pool = commandData.params[1].pureType;
      break;
    default:
      assert(false);
    }
    if (commandData.fullName == "destroyDevice")
    {
      key = "PhysicalDevice";
    }
    assert(m_deleterTypes[key].find(commandData.params[valueIndex].pureType) == m_deleterTypes[key].end());
    m_deleterTypes[key].insert(commandData.params[valueIndex].pureType);
    m_deleters[commandData.params[valueIndex].pureType].call = commandData.reducedName;
  }
}

void VulkanHppGenerator::setDefault(std::string const& name, std::map<std::string, std::string> & defaultValues, EnumData const& enumData)
{
  defaultValues[name] = name + (enumData.values.empty() ? "()" : ("::" + enumData.values.front().name));
}

void VulkanHppGenerator::sortDependencies()
{
  std::set<std::string> listedTypes = { "VkFlags" };
  std::list<DependencyData> sortedDependencies;

  while (!m_dependencies.empty())
  {
    bool found = false;
    for (std::list<DependencyData>::iterator it = m_dependencies.begin(); it != m_dependencies.end(); ++it)
    {
      if (std::find_if(it->dependencies.begin(), it->dependencies.end(), [&listedTypes](std::string const& d) { return listedTypes.find(d) == listedTypes.end(); }) == it->dependencies.end())
      {
        sortedDependencies.push_back(*it);
        listedTypes.insert(it->name);
        m_dependencies.erase(it);
        found = true;
        break;
      }
    }
    if (!found)
    {
      // resolve direct circular dependencies
      for (std::list<DependencyData>::iterator it = m_dependencies.begin(); !found && it != m_dependencies.end(); ++it)
      {
        for (std::set<std::string>::const_iterator dit = it->dependencies.begin(); dit != it->dependencies.end(); ++dit)
        {
          std::list<DependencyData>::const_iterator depIt = std::find_if(m_dependencies.begin(), m_dependencies.end(), [&dit](DependencyData const& dd) { return(dd.name == *dit); });
          if (depIt != m_dependencies.end())
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
    assert(found);
  }

  m_dependencies.swap(sortedDependencies);
}

void VulkanHppGenerator::writeArguments(std::ostream & os, CommandData const& commandData, bool firstCall, bool singular, size_t from, size_t to)
{
  assert(from <= to);

  // get the parameter indices of the counter for vector parameters
  std::map<size_t, size_t> countIndices;
  for (std::map<size_t, size_t>::const_iterator it = commandData.vectorParams.begin(); it != commandData.vectorParams.end(); ++it)
  {
    countIndices.insert(std::make_pair(it->second, it->first));
  }

  bool encounteredArgument = false;
  for (size_t i = from; i < to; i++)
  {
    if (encounteredArgument)
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
      writeCallVectorParameter(os, commandData, firstCall, singular, it);
    }
    else if (m_vkTypes.find(commandData.params[i].pureType) != m_vkTypes.end())
    {
      writeCallVulkanTypeParameter(os, commandData.params[i]);
    }
    else
    {
      writeCallPlainTypeParameter(os, commandData.params[i]);
    }
    encounteredArgument = true;
  }
}

void VulkanHppGenerator::writeBitmaskToString(std::ostream & os, std::string const& bitmaskName, EnumData const &enumData)
{
  // the helper functions to make strings out of flag values
  enterProtect(os, enumData.protect);
  os << "  VULKAN_HPP_INLINE std::string to_string(" << bitmaskName << (enumData.values.empty() ? ")" : " value)") << std::endl
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

void VulkanHppGenerator::writeCall(std::ostream & os, CommandData const& commandData, bool firstCall, bool singular)
{
  // the original function call
  os << "d.vk" << startUpperCase(commandData.fullName) << "( ";

  if (!commandData.className.empty())
  {
    // if it's member of a class -> add the first parameter with "m_" as prefix
    os << "m_" << commandData.params[0].name;
    if (1 < commandData.params.size())
    {
      os << ", ";
    }
  }

  writeArguments(os, commandData, firstCall, singular, commandData.className.empty() ? 0 : 1, commandData.params.size());
  os << " )";
}

void VulkanHppGenerator::writeCallCountParameter(std::ostream & os, CommandData const& commandData, bool singular, std::map<size_t, size_t>::const_iterator it)
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

void VulkanHppGenerator::writeCallPlainTypeParameter(std::ostream & os, ParamData const& paramData)
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

void VulkanHppGenerator::writeCallVectorParameter(std::ostream & os, CommandData const& commandData, bool firstCall, bool singular, std::map<size_t, size_t>::const_iterator it)
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
    std::set<std::string>::const_iterator vkit = m_vkTypes.find(commandData.params[it->first].pureType);
    if ((vkit != m_vkTypes.end()) || (it->first == commandData.templateParam))
    {
      // CHECK for !commandData.params[it->first].optional

      // this parameter is a vulkan type or a templated type -> need to reinterpret cast
      writeReinterpretCast(os, commandData.params[it->first].type.find("const") == 0, vkit != m_vkTypes.end(), commandData.params[it->first].pureType, commandData.params[it->first].type.rfind("* const") != std::string::npos);
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

void VulkanHppGenerator::writeCallVulkanTypeParameter(std::ostream & os, ParamData const& paramData)
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

void VulkanHppGenerator::writeEnumsToString(std::ostream & os, EnumData const& enumData)
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

// Intended only for `enum class Result`!
void VulkanHppGenerator::writeExceptionsForEnum(std::ostream & os, EnumData const& enumData)
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
    { { "className", stripErrorEnumPrefix(enumData.values[i].name) + "Error" },
    { "enumName", enumData.name },
    { "enumMemberName", enumData.values[i].name }
    });
  }
  leaveProtect(os, enumData.protect);
  os << std::endl;
}

void VulkanHppGenerator::writeFunction(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool definition, bool enhanced, bool singular, bool unique, bool isStructureChain)
{
  writeFunctionHeaderTemplate(os, indentation, commandData, enhanced, unique, !definition, isStructureChain);

  os << indentation << (definition ? "VULKAN_HPP_INLINE " : "");
  writeFunctionHeaderReturnType(os, indentation, commandData, enhanced, singular, unique, isStructureChain);
  if (definition && !commandData.className.empty())
  {
    os << commandData.className << "::";
  }
  writeFunctionHeaderName(os, commandData.reducedName, singular, unique);
  writeFunctionHeaderArguments(os, commandData, enhanced, singular, !definition);
  os << (definition ? "" : ";") << std::endl;

  if (definition)
  {
    // write the function body
    os << indentation << "{" << std::endl;
    if (enhanced)
    {
      writeFunctionBodyEnhanced(os, indentation, commandData, singular, unique, isStructureChain);
    }
    else
    {
      writeFunctionBodyStandard(os, indentation, commandData);
    }
    os << indentation << "}" << std::endl;
  }
}

void VulkanHppGenerator::writeFunctionBodyEnhanced(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool singular, bool unique, bool isStructureChain)
{
  if (unique && !singular && (commandData.vectorParams.find(commandData.returnParam) != commandData.vectorParams.end()))    // returns a vector of UniqueStuff
  {
    std::string const stringTemplate =
R"(${i}  static_assert( sizeof( ${type} ) <= sizeof( Unique${type} ), "${type} is greater than Unique${type}!" );
${i}  std::vector<Unique${type}, Allocator> ${typeVariable}s;
${i}  ${typeVariable}s.reserve( ${vectorSize} );
${i}  ${type}* buffer = reinterpret_cast<${type}*>( reinterpret_cast<char*>( ${typeVariable}s.data() ) + ${vectorSize} * ( sizeof( Unique${type} ) - sizeof( ${type} ) ) );
${i}  Result result = static_cast<Result>(d.vk${command}( m_device, ${arguments}, reinterpret_cast<Vk${type}*>( buffer ) ) );

${i}  ${Deleter}<${DeleterTemplate}> deleter( *this, ${deleterArg} );
${i}  for ( size_t i=0 ; i<${vectorSize} ; i++ )
${i}  {
${i}    ${typeVariable}s.push_back( Unique${type}( buffer[i], deleter ) );
${i}  }

${i}  return createResultValue( result, ${typeVariable}s, "VULKAN_HPP_NAMESPACE::${class}::${function}Unique" );
)";

    std::string type = (commandData.returnParam != ~0) ? commandData.params[commandData.returnParam].pureType : "";
    std::string typeVariable = startLowerCase(type);
    std::ostringstream arguments;
    writeArguments(arguments, commandData, true, singular, 1, commandData.params.size() - 1);

    std::map<std::string, DeleterData>::const_iterator ddit = m_deleters.find(type);
    assert(ddit != m_deleters.end());

    bool isCreateFunction = (commandData.fullName.substr(0, 6) == "create");
    os << replaceWithMap(stringTemplate, std::map<std::string, std::string>
    {
      { "i", indentation },
      { "type", type },
      { "typeVariable", typeVariable },
      { "vectorSize", isCreateFunction ? "createInfos.size()" : "allocateInfo." + typeVariable + "Count" },
      { "command", startUpperCase(commandData.fullName) },
      { "arguments", arguments.str() },
      { "Deleter", ddit->second.pool.empty() ? "ObjectDeleter" : "PoolDeleter" },
      { "DeleterTemplate", ddit->second.pool.empty() ? type : commandData.className + "," + ddit->second.pool },
      { "deleterArg", ddit->second.pool.empty() ? "allocator" : "allocateInfo." + startLowerCase(ddit->second.pool) },
      { "class", commandData.className },
      { "function", commandData.reducedName }
    });
  }
  else
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
          writeFunctionBodyEnhancedCallTwoStepIterate(os, indentation, returnName, sizeName, commandData);
        }
        else
        {
          writeFunctionBodyEnhancedCallTwoStepChecked(os, indentation, returnName, sizeName, commandData);
        }
      }
      else
      {
        writeFunctionBodyEnhancedCallTwoStep(os, indentation, returnName, sizeName, commandData);
      }
    }
    else
    {
      if (commandData.returnType == "Result")
      {
        writeFunctionBodyEnhancedCallResult(os, indentation, commandData, singular);
      }
      else
      {
        writeFunctionBodyEnhancedCall(os, indentation, commandData, singular);
      }
    }

    if ((commandData.returnType == "Result") || !commandData.successCodes.empty())
    {
      writeFunctionBodyEnhancedReturnResultValue(os, indentation, returnName, commandData, singular, unique);
    }
    else if ((commandData.returnParam != ~0) && (commandData.returnType != commandData.enhancedReturnType))
    {
      // for the other returning cases, when the return type is somhow enhanced, just return the local returnVariable
      os << indentation << "  return " << returnName << ";" << std::endl;
    }
  }
}

void VulkanHppGenerator::writeFunctionBodyEnhanced(std::ostream &os, std::string const& templateString, std::string const& indentation, CommandData const& commandData, bool singular)
{
  os << replaceWithMap(templateString, {
    { "call", generateCall(commandData, true, singular) },
    { "i", indentation }
  });
}

void VulkanHppGenerator::writeFunctionBodyTwoStep(std::ostream & os, std::string const &templateString, std::string const& indentation, std::string const& returnName, std::string const& sizeName, CommandData const& commandData)
{
  std::map<std::string, std::string> replacements = {
    { "sizeName", sizeName },
    { "returnName", returnName },
    { "call1", generateCall(commandData, true, false) },
    { "call2", generateCall(commandData, false, false) },
    { "i", indentation }
  };

  os << replaceWithMap(templateString, replacements);
}

std::string VulkanHppGenerator::writeFunctionBodyEnhancedLocalReturnVariable(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool singular, bool isStructureChain)
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

void VulkanHppGenerator::writeFunctionBodyEnhancedCall(std::ostream &os, std::string const& indentation, CommandData const& commandData, bool singular)
{
  std::string const templateString = "${i}  return ${call};\n";
  std::string const templateStringVoid = "${i}  ${call};\n";
  writeFunctionBodyEnhanced(os, commandData.returnType == "void" ? templateStringVoid : templateString, indentation, commandData, singular);
}

void VulkanHppGenerator::writeFunctionBodyEnhancedCallResult(std::ostream &os, std::string const& indentation, CommandData const& commandData, bool singular)
{
  std::string const templateString = "${i}  Result result = static_cast<Result>( ${call} );\n";
  writeFunctionBodyEnhanced(os, templateString, indentation, commandData, singular);
}

void VulkanHppGenerator::writeFunctionBodyEnhancedCallTwoStep(std::ostream & os, std::string const& indentation, std::string const& returnName, std::string const& sizeName, CommandData const& commandData)
{
  std::string const templateString =
    R"(${i}  ${call1};
${i}  ${returnName}.resize( ${sizeName} );
${i}  ${call2};
)";
  writeFunctionBodyTwoStep(os, templateString, indentation, returnName, sizeName, commandData);
}

void VulkanHppGenerator::writeFunctionBodyEnhancedCallTwoStepIterate(std::ostream & os, std::string const& indentation, std::string const& returnName, std::string const& sizeName, CommandData const& commandData)
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
${i}  VULKAN_HPP_ASSERT( ${sizeName} <= ${returnName}.size() );
${i}  ${returnName}.resize( ${sizeName} );
)";
  writeFunctionBodyTwoStep(os, templateString, indentation, returnName, sizeName, commandData);
}

void VulkanHppGenerator::writeFunctionBodyEnhancedCallTwoStepChecked(std::ostream & os, std::string const& indentation, std::string const& returnName, std::string const& sizeName, CommandData const& commandData)
{
  std::string const templateString =
    R"(${i}  Result result = static_cast<Result>( ${call1} );
${i}  if ( ( result == Result::eSuccess ) && ${sizeName} )
${i}  {
${i}    ${returnName}.resize( ${sizeName} );
${i}    result = static_cast<Result>( ${call2} );
${i}  }
)";
  writeFunctionBodyTwoStep(os, templateString, indentation, returnName, sizeName, commandData);
}

void VulkanHppGenerator::writeFunctionBodyEnhancedLocalCountVariable(std::ostream & os, std::string const& indentation, CommandData const& commandData)
{
  // local count variable to hold the size of the vector to fill
  assert(commandData.returnParam != ~0);

  std::map<size_t, size_t>::const_iterator returnit = commandData.vectorParams.find(commandData.returnParam);
  assert(returnit != commandData.vectorParams.end() && (returnit->second != ~0));
  assert((commandData.returnType == "Result") || (commandData.returnType == "void"));

  // take the pure type of the size parameter; strip the leading 'p' from its name for its local name
  os << indentation << "  " << commandData.params[returnit->second].pureType << " " << startLowerCase(strip(commandData.params[returnit->second].name, "p")) << ";" << std::endl;
}

void VulkanHppGenerator::writeFunctionBodyEnhancedMultiVectorSizeCheck(std::ostream & os, std::string const& indentation, CommandData const& commandData)
{
  std::string const templateString =
    R"#(#ifdef VULKAN_HPP_NO_EXCEPTIONS
${i}  VULKAN_HPP_ASSERT( ${firstVectorName}.size() == ${secondVectorName}.size() );
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
          os << replaceWithMap(templateString, std::map<std::string, std::string>({
            { "firstVectorName", startLowerCase(strip(commandData.params[it0->first].name, "p")) },
            { "secondVectorName", startLowerCase(strip(commandData.params[it1->first].name, "p")) },
            { "className", commandData.className },
            { "reducedName", commandData.reducedName },
            { "i", indentation }
          }));
        }
      }
    }
  }
}

void VulkanHppGenerator::writeFunctionBodyEnhancedReturnResultValue(std::ostream & os, std::string const& indentation, std::string const& returnName, CommandData const& commandData, bool singular, bool unique)
{
  std::string type = (commandData.returnParam != ~0) ? commandData.params[commandData.returnParam].pureType : "";
  std::string returnVectorName = (commandData.returnParam != ~0) ? strip(commandData.params[commandData.returnParam].name, "p", "s") : "";

  if (unique)
  {
    // the unique version needs a Deleter object for destruction of the newly created stuff
    // get the DeleterData corresponding to the returned type
    std::map<std::string, DeleterData>::const_iterator ddit = m_deleters.find(type);
    assert(ddit != m_deleters.end() && ddit->second.pool.empty());

    os << std::endl
      << indentation << "  ObjectDeleter<" << (commandData.className.empty() ? "NoParent" : commandData.className) << "> deleter( " << (commandData.className.empty() ? "" : "*this, ") << "allocator );" << std::endl;
  }

  // if the return type is "Result" or there is at least one success code, create the Result/Value construct to return
  os << indentation << "  return createResultValue( result, ";
  if (commandData.returnParam != ~0)
  {
    // if there's a return parameter, list it in the Result/Value constructor
    os << returnName << ", ";
  }

  // now the function name (with full namespace) as a string
  os << "\"VULKAN_HPP_NAMESPACE::" << (commandData.className.empty() ? "" : commandData.className + "::") << (singular ? stripPluralS(commandData.reducedName) : commandData.reducedName) << (unique ? "Unique" : "") << "\"";

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

  if (unique)
  {
    os << ", deleter";
  }

  os << " );" << std::endl;
}

void VulkanHppGenerator::writeFunctionBodyStandard(std::ostream & os, std::string const& indentation, CommandData const& commandData)
{
  os << indentation << "  ";
  bool castReturn = false;
  if (commandData.returnType != "void")
  {
    // there's something to return...
    os << "return ";

    castReturn = (m_vkTypes.find(commandData.returnType) != m_vkTypes.end());
    if (castReturn)
    {
      // the return-type is a vulkan type -> need to cast to vk::-type
      os << "static_cast<" << commandData.returnType << ">( ";
    }
  }

  // call the original function
  os << "d.vk" << startUpperCase(commandData.fullName) << "( ";

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

    if (m_vkTypes.find(commandData.params[i].pureType) != m_vkTypes.end())
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

void VulkanHppGenerator::writeFunctionHeaderArguments(std::ostream & os, CommandData const& commandData, bool enhanced, bool singular, bool withDefaults)
{
  os << "(";
  if (enhanced)
  {
    writeFunctionHeaderArgumentsEnhanced(os, commandData, singular, withDefaults);
  }
  else
  {
    writeFunctionHeaderArgumentsStandard(os, commandData, withDefaults);
  }
  os << ")";
  if (!commandData.className.empty())
  {
    os << " const";
  }
}

void VulkanHppGenerator::writeFunctionHeaderArgumentsEnhanced(std::ostream & os, CommandData const& commandData, bool singular, bool withDefaults)
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
              std::map<std::string, BitmaskData>::const_iterator bitmasksIt = m_bitmasks.find(commandData.params[i].pureType);
              if (bitmasksIt != m_bitmasks.end())
              {
                // get the enum corresponding to this flag, to check if it's empty
                std::list<DependencyData>::const_iterator depIt = std::find_if(m_dependencies.begin(), m_dependencies.end(), [&bitmasksIt](DependencyData const& dd) { return(dd.name == bitmasksIt->first); });
                assert((depIt != m_dependencies.end()) && (depIt->dependencies.size() == 1));
                std::map<std::string, EnumData>::const_iterator enumIt = m_enums.find(*depIt->dependencies.begin());
                assert(enumIt != m_enums.end());
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

    if (argEncountered)
    {
      os << ", ";
    }
  }
  os << "Dispatch const &d";
  if (withDefaults)
  {
    os << " = Dispatch()";
  }

  os << " ";
}

void VulkanHppGenerator::writeFunctionHeaderArgumentsStandard(std::ostream & os, CommandData const& commandData, bool withDefaults)
{
  // for the standard case, just list all the arguments as we've got them
  bool argEncountered = false;

  // determine the last argument, where we might provide some default for
  size_t lastArgument = commandData.params.size() - 1;

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

    if (withDefaults && (lastArgument == i))
    {
      // check if the very last argument is a flag without any bits -> provide some empty default for it
      std::map<std::string, BitmaskData>::const_iterator flagIt = m_bitmasks.find(commandData.params[i].pureType);
      if (flagIt != m_bitmasks.end())
      {
        // get the enum corresponding to this flag, to check if it's empty
        std::list<DependencyData>::const_iterator depIt = std::find_if(m_dependencies.begin(), m_dependencies.end(), [&flagIt](DependencyData const& dd) { return(dd.name == flagIt->first); });
        assert((depIt != m_dependencies.end()) && (depIt->dependencies.size() == 1));
        std::map<std::string, EnumData>::const_iterator enumIt = m_enums.find(*depIt->dependencies.begin());
        assert(enumIt != m_enums.end());
        if (enumIt->second.values.empty())
        {
          // there are no bits in this flag -> provide the default
          os << " = " << commandData.params[i].pureType << "()";
        }
      }
    }
    argEncountered = true;
  }
  if (argEncountered)
  {
    os << ", ";
  }

  os << "Dispatch const &d";
  if (withDefaults)
  {
    os << " = Dispatch() ";
  }
}

void VulkanHppGenerator::writeFunctionHeaderReturnType(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool enhanced, bool singular, bool unique, bool isStructureChain)
{
  std::string templateString;
  std::string returnType;
  if (enhanced)
  {
    // the enhanced function might return some pretty complex return stuff
    if (isStructureChain || (!singular && (commandData.enhancedReturnType.find("Allocator") != std::string::npos)))
    {
      // for the non-singular case with allocation, we need to prepend with 'typename' to keep compilers happy
      templateString = "typename ";
    }
    if (unique)
    {
      // the unique version returns something prefixed with 'Unique'; potentially a vector of that stuff
      // it's a vector, if it's not the singular version and the return parameter is a vector parameter
      bool returnsVector = !singular && (commandData.vectorParams.find(commandData.returnParam) != commandData.vectorParams.end());

      templateString += returnsVector ? "ResultValueType<std::vector<Unique${returnType},Allocator>>::type " : "ResultValueType<Unique${returnType}>::type ";
      returnType = isStructureChain ? "StructureChain<T...>" : commandData.params[commandData.returnParam].pureType;
    }
    else if ((commandData.enhancedReturnType != commandData.returnType) && (commandData.returnType != "void"))
    {
      // if the enhanced return type differs from the original return type, and it's not void, we return a ResultValueType<...>::type
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

void VulkanHppGenerator::writeFunctionHeaderTemplate(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool enhanced, bool unique, bool withDefault, bool isStructureChain)
{
  std::string dispatch = withDefault ? std::string("typename Dispatch = DispatchLoaderStatic") : std::string("typename Dispatch");
  if (enhanced && isStructureChain)
  {
    os << indentation << "template <typename ...T, " << dispatch << ">" << std::endl;
  }
  else if (enhanced && (commandData.templateParam != ~0) && ((commandData.templateParam != commandData.returnParam) || (commandData.enhancedReturnType == "Result")))
  {
    // if there's a template parameter, not being the return parameter or where the enhanced return type is 'Result' -> templatize on type 'T'
    assert(commandData.enhancedReturnType.find("Allocator") == std::string::npos);
    os << indentation << "template <typename T, " << dispatch << ">" << std::endl;
  }
  else if (enhanced && (commandData.enhancedReturnType.find("Allocator") != std::string::npos))
  {
    // otherwise, if there's an Allocator used in the enhanced return type, we templatize on that Allocator
    assert((commandData.enhancedReturnType.substr(0, 12) == "std::vector<") && (commandData.enhancedReturnType.find(',') != std::string::npos) && (12 < commandData.enhancedReturnType.find(',')));
    os << indentation << "template <typename Allocator";
    if (withDefault)
    {
      // for the default type get the type from the enhancedReturnType, which is of the form 'std::vector<Type,Allocator>'
      os << " = std::allocator<" << (unique ? "Unique" : "") << commandData.enhancedReturnType.substr(12, commandData.enhancedReturnType.find(',') - 12) << ">";
    }
    os << ", " << dispatch;
    os << "> " << std::endl;
  }
  else
  {
    os << indentation << "template<" << dispatch << ">" << std::endl;
  }
}

void VulkanHppGenerator::writeResultEnum(std::ostream & os)
{
  std::list<DependencyData>::const_iterator it = std::find_if(m_dependencies.begin(), m_dependencies.end(), [](DependencyData const& dp) { return dp.name == "Result"; });
  assert(it != m_dependencies.end());
  writeTypeEnum(os, m_enums.find(it->name)->second);
  writeEnumsToString(os, m_enums.find(it->name)->second);
  os << "#ifndef VULKAN_HPP_NO_EXCEPTIONS";
  os << exceptionHeader;
  os << exceptionClassesHeader;
  writeExceptionsForEnum(os, m_enums.find(it->name)->second);
  writeThrowExceptions(os, m_enums.find(it->name)->second);
  os << "#endif" << std::endl;
  m_dependencies.erase(it);
}

void VulkanHppGenerator::writeStructConstructor(std::ostream & os, std::string const& name, StructData const& structData, std::map<std::string, std::string> const& defaultValues)
{
  // the constructor with all the elements as arguments, with defaults
  os << "    " << name << "( ";
  bool listedArgument = false;
  for (size_t i = 0; i < structData.members.size(); i++)
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
    // skip members 'pNext' and 'sType' are directly set by initializers
    if ((structData.members[i].name != "pNext") && (structData.members[i].name != "sType") && (structData.members[i].arraySize.empty()))
    {
      // here, we can only handle non-array arguments
      std::string templateString = "      ${sep} ${member}( ${value} )\n";
      std::string sep = firstArgument ? ":" : ",";
      std::string member = structData.members[i].name;
      std::string value = structData.members[i].name + "_";   // the elements are initialized by the corresponding argument (with trailing '_', as mentioned above)

      os << replaceWithMap(templateString, { { "sep", sep },{ "member", member },{ "value", value } });
      firstArgument = false;
    }
  }

  // the body of the constructor, copying over data from argument list into wrapped struct
  os << "    {" << std::endl;
  for (size_t i = 0; i < structData.members.size(); i++)
  {
    if (!structData.members[i].arraySize.empty())
    {
      // here we can handle the arrays, copying over from argument (with trailing '_') to member
      // size is arraySize times sizeof type
      std::string member = structData.members[i].name;
      std::string arraySize = structData.members[i].arraySize;
      std::string type = structData.members[i].type;
      os << replaceWithMap("      memcpy( &${member}, ${member}_.data(), ${arraySize} * sizeof( ${type} ) );\n",
      { { "member", member },{ "arraySize", arraySize },{ "type", type } });
    }
  }
  os << "    }\n\n";

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

  os << replaceWithMap(templateString, { { "name", name } });
}

void VulkanHppGenerator::writeStructSetter(std::ostream & os, std::string const& structureName, MemberData const& memberData)
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

void VulkanHppGenerator::writeStructureChainValidation(std::ostream & os)
{
  // write all template functions for the structure pointer chain validation
  for (auto it = m_dependencies.begin(); it != m_dependencies.end(); ++it)
  {
    switch (it->category)
    {
    case DependencyData::Category::STRUCT:
      writeStructureChainValidation(os, *it);
      break;
    }
  }
}

void VulkanHppGenerator::writeStructureChainValidation(std::ostream & os, DependencyData const& dependencyData)
{
  std::map<std::string, StructData>::const_iterator it = m_structs.find(dependencyData.name);
  assert(it != m_structs.end());

  if (!it->second.structExtends.empty()) {
    enterProtect(os, it->second.protect);

    // write out allowed structure chains
    for (auto extendName : it->second.structExtends)
    {
      std::map<std::string, StructData>::const_iterator itExtend = m_structs.find(extendName);
      assert(itExtend != m_structs.end());
      enterProtect(os, itExtend->second.protect);

      os << "  template <> struct isStructureChainValid<" << extendName << ", " << dependencyData.name << ">{ enum { value = true }; };" << std::endl;

      leaveProtect(os, itExtend->second.protect);
    }
    leaveProtect(os, it->second.protect);
  }
}

void VulkanHppGenerator::writeThrowExceptions(std::ostream & os, EnumData const& enumData)
{
  enterProtect(os, enumData.protect);
  os <<
    R"(  VULKAN_HPP_INLINE void throwResultException( Result result, char const * message )
  {
    switch ( result )
    {
)";
  for (size_t i = 0; i<enumData.values.size(); i++)
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

void VulkanHppGenerator::writeToStringFunctions(std::ostream & os)
{
  // write all the to_string functions for enums and flags
  for (auto it = m_dependencies.begin(); it != m_dependencies.end(); ++it)
  {
    switch (it->category)
    {
    case DependencyData::Category::BITMASK:
      writeBitmaskToString(os, it->name, m_enums.find(*it->dependencies.begin())->second);
      break;
    case DependencyData::Category::ENUM:
      assert(m_enums.find(it->name) != m_enums.end());
      writeEnumsToString(os, m_enums.find(it->name)->second);
      break;
    }
  }
}

void VulkanHppGenerator::writeTypeBitmask(std::ostream & os, std::string const& bitmaskName, BitmaskData const& bitmaskData, EnumData const& enumData)
{
  enterProtect(os, bitmaskData.protect);

  // each Flags class is using on the class 'Flags' with the corresponding FlagBits enum as the template parameter
  os << "  using " << bitmaskName << " = Flags<" << enumData.name << ", Vk" << bitmaskName << ">;" << std::endl;

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
  VULKAN_HPP_INLINE ${bitmaskName} operator|( ${enumName} bit0, ${enumName} bit1 )
  {
    return ${bitmaskName}( bit0 ) | bit1;
  }

  VULKAN_HPP_INLINE ${bitmaskName} operator~( ${enumName} bits )
  {
    return ~( ${bitmaskName}( bits ) );
  }

  template <> struct FlagTraits<${enumName}>
  {
    enum
    {
      allFlags = ${allFlags}
    };
  };
)";
    os << replaceWithMap(templateString, { { "bitmaskName", bitmaskName },{ "enumName", enumData.name },{ "allFlags", allFlags.str() } });
  }

  if (!bitmaskData.alias.empty())
  {
    os << std::endl
      << "  using " << bitmaskData.alias << " = " << bitmaskName << ";" << std::endl;
  }

  leaveProtect(os, bitmaskData.protect);
  os << std::endl;
}

void VulkanHppGenerator::writeTypeCommand(std::ostream & os, DependencyData const& dependencyData)
{
  assert(m_commands.find(dependencyData.name) != m_commands.end());
  CommandData const& commandData = m_commands.find(dependencyData.name)->second;
  if (commandData.className.empty())
  {
    if (commandData.fullName == "createInstance")
    {
      // special handling for createInstance, as we need to explicitly place the forward declarations and the deleter classes here
      auto deleterTypesIt = m_deleterTypes.find("");
      assert((deleterTypesIt != m_deleterTypes.end()) && (deleterTypesIt->second.size() == 1));

      writeUniqueTypes(os, *deleterTypesIt);
      writeTypeCommand(os, "  ", commandData, false);
    }
    else
    {
      writeTypeCommand(os, "  ", commandData, false);
    }
    writeTypeCommand(os, "  ", commandData, true);
    os << std::endl;
  }
}

void VulkanHppGenerator::writeTypeCommand(std::ostream & os, std::string const& indentation, CommandData const& commandData, bool definition)
{
  enterProtect(os, commandData.protect);

  bool isStructureChain = m_extendedStructs.find(commandData.enhancedReturnType) != m_extendedStructs.end();

  // first create the standard version of the function
  std::ostringstream standard;
  writeFunction(standard, indentation, commandData, definition, false, false, false, false);

  // then the enhanced version, composed by up to five parts
  std::ostringstream enhanced;
  writeFunction(enhanced, indentation, commandData, definition, true, false, false, false);

  if (isStructureChain)
  {
    writeFunction(enhanced, indentation, commandData, definition, true, false, false, true);
  }

  // then a singular version, if a sized vector would be returned
  std::map<size_t, size_t>::const_iterator returnVector = commandData.vectorParams.find(commandData.returnParam);
  bool singular = (returnVector != commandData.vectorParams.end()) && (returnVector->second != ~0) && (commandData.params[returnVector->second].type.back() != '*');
  if (singular)
  {
    writeFunction(enhanced, indentation, commandData, definition, true, true, false, false);
  }

  // special handling for createDevice and createInstance !
  bool specialWriteUnique = (commandData.reducedName == "createDevice") || (commandData.reducedName == "createInstance");

  // and then the same for the Unique* versions (a Deleter is available for the commandData's class, and the function starts with 'allocate' or 'create')
  if (((m_deleters.find(commandData.className) != m_deleters.end()) || specialWriteUnique) && ((commandData.reducedName.substr(0, 8) == "allocate") || (commandData.reducedName.substr(0, 6) == "create")))
  {
    enhanced << "#ifndef VULKAN_HPP_NO_SMART_HANDLE" << std::endl;
    writeFunction(enhanced, indentation, commandData, definition, true, false, true, false);

    if (singular)
    {
      writeFunction(enhanced, indentation, commandData, definition, true, true, true, false);
    }
    enhanced << "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/" << std::endl;
  }

  // and write one or both of them
  writeStandardOrEnhanced(os, standard.str(), enhanced.str());

  leaveProtect(os, commandData.protect);
  os << std::endl;
}

void VulkanHppGenerator::writeTypeEnum(std::ostream & os, EnumData const& enumData)
{
  // a named enum per enum, listing all its values by setting them to the original Vulkan names
  enterProtect(os, enumData.protect);
  os << "  enum class " << enumData.name << std::endl
    << "  {" << std::endl;
  for (size_t i = 0; i<enumData.values.size(); i++)
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

  leaveProtect(os, enumData.protect);
  os << std::endl;
}

void VulkanHppGenerator::writeTypeHandle(std::ostream & os, DependencyData const& dependencyData, HandleData const& handleData)
{
  enterProtect(os, handleData.protect);

  // check if there are any forward dependenices for this handle -> list them first
  if (!dependencyData.forwardDependencies.empty())
  {
    os << "  // forward declarations" << std::endl;
    for (std::set<std::string>::const_iterator it = dependencyData.forwardDependencies.begin(); it != dependencyData.forwardDependencies.end(); ++it)
    {
      assert(m_structs.find(*it) != m_structs.end());
      os << "  struct " << *it << ";" << std::endl;
    }
    os << std::endl;
  }

  // then write any forward declaration of Deleters used by this handle
  std::map<std::string, std::set<std::string>>::const_iterator deleterTypesIt = m_deleterTypes.find(dependencyData.name);
  if (deleterTypesIt != m_deleterTypes.end())
  {
    writeUniqueTypes(os, *deleterTypesIt);
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
    std::map<std::string, CommandData>::const_iterator cit = m_commands.find(handleData.commands[i]);
    assert((cit != m_commands.end()) && !cit->second.className.empty());
    writeTypeCommand(commands, "    ", cit->second, false);

    // special handling for destroy functions
    if (((cit->second.fullName.substr(0, 7) == "destroy") && (cit->second.reducedName != "destroy")) || (cit->second.fullName.substr(0, 4) == "free"))
    {
      CommandData shortenedCommand = cit->second;
      shortenedCommand.reducedName = (cit->second.fullName.substr(0, 7) == "destroy") ? "destroy" : "free";
      writeTypeCommand(commands, "    ", shortenedCommand, false);
    }
  }

  os << replaceWithMap(templateString, {
    { "className", dependencyData.name },
    { "memberName", memberName },
    { "commands", commands.str() }
  });

  // and finally the commands, that are member functions of this handle
  for (size_t i = 0; i < handleData.commands.size(); i++)
  {
    std::string commandName = handleData.commands[i];
    std::map<std::string, CommandData>::const_iterator cit = m_commands.find(commandName);
    assert((cit != m_commands.end()) && !cit->second.className.empty());
    std::list<DependencyData>::const_iterator dep = std::find_if(m_dependencies.begin(), m_dependencies.end(), [commandName](DependencyData const& dd) { return dd.name == commandName; });
    assert(dep != m_dependencies.end() && (dep->name == cit->second.fullName));
    writeTypeCommand(os, "  ", cit->second, true);

    // special handling for destroy functions
    if (((cit->second.fullName.substr(0, 7) == "destroy") && (cit->second.reducedName != "destroy")) || (cit->second.fullName.substr(0, 4) == "free"))
    {
      CommandData shortenedCommand = cit->second;
      shortenedCommand.reducedName = (cit->second.fullName.substr(0, 7) == "destroy") ? "destroy" : "free";
      writeTypeCommand(os, "  ", shortenedCommand, true);
    }
  }

  if (!handleData.alias.empty())
  {
    os << "  using " << handleData.alias << " = " << dependencyData.name << ";" << std::endl
      << std::endl;
  }

  leaveProtect(os, handleData.protect);
}

void VulkanHppGenerator::writeTypes(std::ostream & os, std::map<std::string, std::string> const& defaultValues)
{
  assert(m_deleterTypes.find("") != m_deleterTypes.end());

  for (std::list<DependencyData>::const_iterator it = m_dependencies.begin(); it != m_dependencies.end(); ++it)
  {
    switch (it->category)
    {
    case DependencyData::Category::BITMASK:
      assert(m_bitmasks.find(it->name) != m_bitmasks.end());
      writeTypeBitmask(os, it->name, m_bitmasks.find(it->name)->second, m_enums.find(generateEnumNameForFlags(it->name))->second);
      break;
    case DependencyData::Category::COMMAND:
      writeTypeCommand(os, *it);
      break;
    case DependencyData::Category::ENUM:
      assert(m_enums.find(it->name) != m_enums.end());
      writeTypeEnum(os, m_enums.find(it->name)->second);
      break;
    case DependencyData::Category::FUNC_POINTER:
    case DependencyData::Category::REQUIRED:
      // skip FUNC_POINTER and REQUIRED, they just needed to be in the dependencies list to resolve dependencies
      break;
    case DependencyData::Category::HANDLE:
      assert(m_handles.find(it->name) != m_handles.end());
      writeTypeHandle(os, *it, m_handles.find(it->name)->second);
      break;
    case DependencyData::Category::SCALAR:
      writeTypeScalar(os, *it);
      break;
    case DependencyData::Category::STRUCT:
      writeTypeStruct(os, *it, defaultValues);
      break;
    case DependencyData::Category::UNION:
      assert(m_structs.find(it->name) != m_structs.end());
      writeTypeUnion(os, *it, defaultValues);
      break;
    default:
      assert(false);
      break;
    }
  }
}

void VulkanHppGenerator::writeTypeScalar(std::ostream & os, DependencyData const& dependencyData)
{
  assert(dependencyData.dependencies.size() == 1);
  os << "  using " << dependencyData.name << " = " << *dependencyData.dependencies.begin() << ";" << std::endl
    << std::endl;
}

void VulkanHppGenerator::writeTypeStruct(std::ostream & os, DependencyData const& dependencyData, std::map<std::string, std::string> const& defaultValues)
{
  std::map<std::string, StructData>::const_iterator it = m_structs.find(dependencyData.name);
  assert(it != m_structs.end());

  enterProtect(os, it->second.protect);
  os << "  struct " << dependencyData.name << std::endl
    << "  {" << std::endl;

  // only structs that are not returnedOnly get a constructor!
  if (!it->second.returnedOnly)
  {
    writeStructConstructor(os, dependencyData.name, it->second, defaultValues);
  }

  // create the setters
  if (!it->second.returnedOnly)
  {
    for (size_t i = 0; i<it->second.members.size(); i++)
    {
      writeStructSetter(os, dependencyData.name, it->second.members[i]);
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
  if (!containsUnion(dependencyData.name, m_structs))
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
      assert(!it->second.members[i].values.empty());
      auto nameIt = m_nameMap.find(it->second.members[i].values);
      assert(nameIt != m_nameMap.end());
      os << "  private:" << std::endl
        << "    StructureType sType = " << nameIt->second << ";" << std::endl
        << std::endl
        << "  public:" << std::endl;
    }
    else
    {
      os << "    " << it->second.members[i].type << " " << it->second.members[i].name;
      if (it->second.members[i].name == "pNext")
      {
        os << " = nullptr";
      }
      else if (!it->second.members[i].arraySize.empty())
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
      << "  using " << it->second.alias << " = " << dependencyData.name << ";" << std::endl;
  }

  leaveProtect(os, it->second.protect);
  os << std::endl;
}

void VulkanHppGenerator::writeUniqueTypes(std::ostream &os, std::pair<std::string, std::set<std::string>> const& deleterTypes)
{
  os << "#ifndef VULKAN_HPP_NO_SMART_HANDLE" << std::endl;
  if (!deleterTypes.first.empty())
  {
    os << "  class " << deleterTypes.first << ";" << std::endl;
  }
  os << std::endl;

  bool first = true;
  for (auto const& dt : deleterTypes.second)
  {
    auto ddit = m_deleters.find(dt);
    assert(ddit != m_deleters.end());

    os << "  template <> class UniqueHandleTraits<" << dt << "> {public: using deleter = " << (ddit->second.pool.empty() ? "Object" : "Pool") << "Deleter<" << (deleterTypes.first.empty() ? "NoParent" : deleterTypes.first) << (ddit->second.pool.empty() ? "" : ", " + ddit->second.pool) << ">; };\n";
    os << "  using Unique" << dt << " = UniqueHandle<" << dt << ">;" << std::endl;
  }
  os << "#endif /*VULKAN_HPP_NO_SMART_HANDLE*/" << std::endl
    << std::endl;
}

void VulkanHppGenerator::writeTypeUnion(std::ostream & os, DependencyData const& dependencyData, std::map<std::string, std::string> const& defaultValues)
{
  std::map<std::string, StructData>::const_iterator it = m_structs.find(dependencyData.name);
  assert(it != m_structs.end());

  std::ostringstream oss;
  os << "  union " << dependencyData.name << std::endl
    << "  {" << std::endl;

  for (size_t i = 0; i<it->second.members.size(); i++)
  {
    // one constructor per union element
    os << "    " << dependencyData.name << "( ";
    if (it->second.members[i].arraySize.empty())
    {
      os << it->second.members[i].type << " ";
    }
    else
    {
      os << "const std::array<" << it->second.members[i].type << "," << it->second.members[i].arraySize << ">& ";
    }
    os << it->second.members[i].name << "_";

    // just the very first constructor gets default arguments
    if (i == 0)
    {
      std::map<std::string, std::string>::const_iterator defaultIt = defaultValues.find(it->second.members[i].pureType);
      assert(defaultIt != defaultValues.end());
      if (it->second.members[i].arraySize.empty())
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
    if (it->second.members[i].arraySize.empty())
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
    writeStructSetter(os, dependencyData.name, it->second.members[i]);
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
    needsUnrestrictedUnions = (m_vkTypes.find(it->second.members[i].type) != m_vkTypes.end());
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
    if (m_vkTypes.find(it->second.members[i].type) != m_vkTypes.end())
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

#if !defined(NDEBUG)
void VulkanHppGenerator::checkExtensionRequirements()
{
  for (auto const& ext : m_extensions)
  {
    for (auto const& req : ext.second.requires)
    {
      auto reqExt = m_extensions.find(req);
      assert(reqExt != m_extensions.end());
      assert(reqExt->second.protect.empty() || (reqExt->second.protect == ext.second.protect));
    }
  }
}

void VulkanHppGenerator::skipVendorID(tinyxml2::XMLElement const* element)
{
  std::map<std::string, std::string> attributes = getAttributes(element);
  checkAttributes(attributes, element->GetLineNum(), { { "comment",{} },{ "id",{} },{ "name",{} } }, {});
  checkElements(getChildElements(element), {});

  VendorIDData vendorID;
  for (auto const& attribute : attributes)
  {
    std::string name = attribute.first;
    if (name == "comment")
    {
      vendorID.comment = attribute.second;
    }
    else if (name == "id")
    {
      vendorID.id = attribute.second;
    }
    else
    {
      assert(name == "name");
      vendorID.name = attribute.second;
    }
  }
  m_vendorIDs.push_back(vendorID);
}

void VulkanHppGenerator::skipVendorIDs(tinyxml2::XMLElement const* element)
{
  checkAttributes(getAttributes(element), element->GetLineNum(), { { "comment",{} } }, {});
  std::vector<tinyxml2::XMLElement const*> children = getChildElements(element);
  checkElements(children, { "vendorid" });

  for (auto child : children)
  {
    skipVendorID(child);
  }
}
#endif

void VulkanHppGenerator::EnumData::addEnumValue(std::string const &name, std::string const& tag, std::map<std::string, std::string> & nameMap)
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

void VulkanHppGenerator::writeDelegationClassStatic(std::ostream &os)
{
  os << "class DispatchLoaderStatic" << std::endl
     << "{"                    << std::endl
     << "public:\n";

  for (auto command : m_commands)
  {
    enterProtect(os, command.second.protect);
    os << "  " << command.second.unchangedReturnType << " vk" << startUpperCase(command.second.fullName) << "( ";
    bool first = true;
    for (auto param : command.second.params)
    {
      if (!first) {
        os << ", ";
      }
      os << param.unchangedType << " " << param.name;
      if (!param.arraySize.empty())
      {
        os << "[" << param.arraySize << "]";
      }
      first = false;
    }
    os << "  ) const\n"
      << "  {\n"
      << "    return ::vk" << startUpperCase(command.second.fullName) << "( ";
    first = true;
    for (auto param : command.second.params)
    {
      if (!first) {
        os << ", ";
      }
      os << param.name;
      first = false;
    }
    os << ");\n";
    os << "  }\n";
    leaveProtect(os, command.second.protect);
  }
  os << "};\n";
}

void VulkanHppGenerator::writeDelegationClassDynamic(std::ostream &os)
{
  os << "  class DispatchLoaderDynamic" << std::endl
     << "  {" << std::endl
     << "  public:" << std::endl;

  for (auto command : m_commands)
  {
    enterProtect(os, command.second.protect);
    os << "    PFN_vk" << startUpperCase(command.second.fullName) << " vk" << startUpperCase(command.second.fullName) << " = 0;" << std::endl;
    leaveProtect(os, command.second.protect);
  }

  // write initialization function to fetch function pointers
  os << "  public:" << std::endl
     << "    DispatchLoaderDynamic(Instance instance = Instance(), Device device = Device())" << std::endl
     << "    {" << std::endl
     << "      if (instance)" << std::endl
     << "      {" << std::endl
     << "        init(instance, device);" << std::endl
     << "      }" << std::endl
     << "    }" << std::endl << std::endl
     << "    void init(Instance instance, Device device = Device())" << std::endl
     << "    {" << std::endl;

  for (auto command : m_commands)
  {
    enterProtect(os, command.second.protect);
    if (!command.second.params.empty() 
      && m_handles.find(command.second.params[0].type) != m_handles.end()
      && command.second.params[0].type != "Instance")
    {
      os << "      vk" << startUpperCase(command.second.fullName) << " = PFN_vk" << startUpperCase(command.second.fullName) 
        << "(device ? device.getProcAddr( \"vk" << startUpperCase(command.second.fullName) << "\") : instance.getProcAddr( \"vk" << startUpperCase(command.second.fullName) << "\"));" << std::endl;
    }
    else {
      os << "      vk" << startUpperCase(command.second.fullName) << " = PFN_vk" << startUpperCase(command.second.fullName) << "(instance.getProcAddr( \"vk" << startUpperCase(command.second.fullName) << "\"));" << std::endl;
    }
    leaveProtect(os, command.second.protect);
  }
  os << "    }" << std::endl;
  os << "  };\n";
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
      std::cout << "VkGenerate: failed to load file " << filename << " . Error code: " << error << std::endl;
      return -1;
    }

    VulkanHppGenerator generator;

    tinyxml2::XMLElement const* registryElement = doc.FirstChildElement();
    checkAttributes(getAttributes(registryElement), registryElement->GetLineNum(), {}, {});
    assert(strcmp(registryElement->Value(), "registry") == 0);
    assert(!registryElement->NextSiblingElement());

    std::vector<tinyxml2::XMLElement const*> children = getChildElements(registryElement);
    checkElements(children, { "commands", "comment", "enums", "extensions", "feature", "tags", "types", "vendorids" });
    for (auto child : children)
    {
      const std::string value = child->Value();
      if (value == "commands")
      {
        generator.readCommands(child);
      }
      else if (value == "comment")
      {
        // get the vulkan license header and skip any leading spaces
        generator.readComment(child);
      }
      else if (value == "enums")
      {
        generator.readEnums(child);
      }
      else if (value == "extensions")
      {
        generator.readExtensions(child);
      }
      else if (value == "feature")
      {
        generator.readFeature(child);
      }
      else if (value == "tags")
      {
        generator.readTags(child);
      }
      else if (value == "types")
      {
        generator.readTypes(child);
      }
      else
      {
        assert(value == "vendorids");
#if !defined(NDEBUG)
        generator.skipVendorIDs(child);
#endif
      }
    }

    generator.sortDependencies();

#if !defined(NDEBUG)
    generator.checkExtensionRequirements();
#endif

    std::map<std::string, std::string> defaultValues = generator.createDefaults();

    std::ofstream ofs(VULKAN_HPP);
    ofs << generator.getVulkanLicenseHeader() << std::endl
      << R"(
#ifndef VULKAN_HPP
#define VULKAN_HPP

#include <algorithm>
#include <array>
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
#if !defined(VULKAN_HPP_ASSERT)
# include <cassert>
# define VULKAN_HPP_ASSERT   assert
#endif
)";

    writeVersionCheck(ofs, generator.getVersion());
    writeTypesafeCheck(ofs, generator.getTypesafeCheck());
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
    generator.writeResultEnum(ofs);

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
      << createResultValueHeader
      << deleterClassString;

    generator.writeDelegationClassStatic(ofs);

    generator.writeTypes(ofs, defaultValues);
    generator.writeStructureChainValidation(ofs);
    generator.writeToStringFunctions(ofs);

    generator.writeDelegationClassDynamic(ofs);

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
