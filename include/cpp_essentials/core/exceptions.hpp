#ifndef CPP_ESSENTIALS_CORE_EXCEPTIONS_HPP_
#define CPP_ESSENTIALS_CORE_EXCEPTIONS_HPP_

#pragma once

#include <stdexcept>
#include <sstream>

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/debug_utils.hpp>

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

    exception_wrapper(inner_type inner, const code_location& loc)
        : inner_type { std::move(inner) }
    {
        std::stringstream ss;
        ss << inner_type::what() << "\n    " << loc;
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
void throw_exception(const Exception& ex, const code_location& loc)
{
    throw exception_wrapper<Exception>(ex, loc);
}

} /* namespace detail */

} /* namespace cpp_essentials::core */

#define THROW(...) ::cpp_essentials::core::detail::throw_exception(__VA_ARGS__, CODE_LOCATION)

#endif /* CPP_ESSENTIALS_CORE_EXCEPTIONS_HPP_ */
