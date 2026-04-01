  class ErrorCategoryImpl : public std::error_category
  {
    public:
    virtual char const * name() const VULKAN_HPP_NOEXCEPT override { return VULKAN_HPP_NAMESPACE_STRING"::Result"; }
    virtual std::string message(int ev) const override
    {
#if defined( VULKAN_HPP_NO_TO_STRING )
      return std::to_string( ev );
#else
      return to_string(static_cast<Result>(ev));
#endif
    }
  };

  class Error
  {
    public:
    Error() VULKAN_HPP_NOEXCEPT              = default;
    Error(Error const &) VULKAN_HPP_NOEXCEPT = default;
    virtual ~Error() VULKAN_HPP_NOEXCEPT     = default;

    virtual char const * what() const VULKAN_HPP_NOEXCEPT = 0;
  };

  class LogicError : public Error, public std::logic_error
  {
    public:
    explicit LogicError( std::string const & what )
      : Error(), std::logic_error(what) {}
    explicit LogicError( char const * what )
      : Error(), std::logic_error(what) {}

    virtual char const * what() const VULKAN_HPP_NOEXCEPT { return std::logic_error::what(); }
  };

  class SystemError : public Error, public std::system_error
  {
    public:
    SystemError( std::error_code ec )
      : Error(), std::system_error(ec) {}
    SystemError( std::error_code ec, std::string const & what )
      : Error(), std::system_error(ec, what) {}
    SystemError( std::error_code ec, char const * what )
      : Error(), std::system_error(ec, what) {}
    SystemError( int ev, std::error_category const & ecat )
      : Error(), std::system_error(ev, ecat) {}
    SystemError( int ev, std::error_category const & ecat, std::string const & what)
      : Error(), std::system_error(ev, ecat, what) {}
    SystemError( int ev, std::error_category const & ecat, char const * what)
      : Error(), std::system_error(ev, ecat, what) {}

    virtual char const * what() const VULKAN_HPP_NOEXCEPT { return std::system_error::what(); }
  };

  VULKAN_HPP_INLINE std::error_category const & errorCategory() VULKAN_HPP_NOEXCEPT
  {
    static ErrorCategoryImpl instance;
    return instance;
  }

  VULKAN_HPP_INLINE std::error_code make_error_code(Result e) VULKAN_HPP_NOEXCEPT
  {
    return std::error_code(static_cast<int>(e), errorCategory());
  }

  VULKAN_HPP_INLINE std::error_condition make_error_condition(Result e) VULKAN_HPP_NOEXCEPT
  {
    return std::error_condition(static_cast<int>(e), errorCategory());
  }
