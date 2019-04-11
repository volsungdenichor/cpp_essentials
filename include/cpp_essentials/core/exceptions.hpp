#ifndef CPP_ESSENTIALS_CORE_EXCEPTIONS_HPP_
#define CPP_ESSENTIALS_CORE_EXCEPTIONS_HPP_

#pragma once

#include <stdexcept>
#include <sstream>

#include <cpp_essentials/cc/cc.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template
    < class Exception
    , CONCEPT = cc::BaseOf<std::exception, Exception>>
class exception_wrapper : public Exception
{
public:
    using inner_type = Exception;

    exception_wrapper(const inner_type& inner, std::string_view file, int line, std::string_view function)
        : inner_type { inner }
    {
        std::stringstream ss;
        ss << inner_type::what();
        ss << "\n    " << file << "(" << line << "): " << function;
        _what = ss.str();
    }

    const char* what() const noexcept override
    {
        return _what.c_str();
    }

private:
    std::string _what;
};

template <class Exception, class... Args>
void throw_exception(const Exception& ex, std::string_view file, int line, std::string_view function)
{
    throw exception_wrapper<Exception>(ex, file, line, function);
}

} /* namespace detail */

} /* namespace cpp_essentials::core */

#if defined(__MINGW32__)
#  define THROW(...) ::cpp_essentials::core::detail::throw_exception(__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#elif defined(__GNUC__)
#  define THROW(...) ::cpp_essentials::core::detail::throw_exception(__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#elif defined(_MSC_VER)
#  define THROW(...) ::cpp_essentials::core::detail::throw_exception(__VA_ARGS__, __FILE__, __LINE__, __FUNCSIG__)
#endif

#endif /* CPP_ESSENTIALS_CORE_EXCEPTIONS_HPP_ */
