#ifndef CPP_ESSENTIALS_CORE_ASSERTIONS_HPP
#define CPP_ESSENTIALS_CORE_ASSERTIONS_HPP

#pragma once

#include <stdexcept>
#include <sstream>
#include <string_view>

#include <cpp_essentials/core/exceptions.hpp>
#include <cpp_essentials/core/function_defs.hpp>

namespace cpp_essentials::core
{

namespace detail
{

inline void not_implemented(const code_location& loc, std::string_view message = {})
{
    std::stringstream ss;

    ss << "not implemented:" << std::endl;
    ss << "  message    : " << message;

    std::string what = ss.str();

    detail::throw_exception(std::runtime_error { what }, loc);
}

inline void handle(bool condition, std::string_view expression, const code_location& loc, std::string_view message = {})
{
    if (condition)
    {
        return;
    }

    std::stringstream ss;
    ss << "expectation failure:" << std::endl;
    ss << "  message    : " << message << std::endl;
    ss << "  condition  : " << expression;

    std::string what = ss.str();

    detail::throw_exception(std::runtime_error { what }, loc);
}

struct ensures_fn
{
    void operator ()(bool condition, std::string message) const
    {
        if (condition)
        {
            return;
        }

        throw std::runtime_error{ std::move(message) };
    }

    void operator ()(bool condition, const function<std::string>& message_builder) const
    {
        if (condition)
        {
            return;
        }

        throw std::runtime_error{ message_builder() };
    }

    void operator ()(bool condition) const
    {
        (*this)(condition, "condition not met");
    }
};

} /* namespace detail */

static constexpr detail::ensures_fn expects = {};
static constexpr detail::ensures_fn ensures = {};

} /* namespace cpp_essentials::core */


#define NOT_IMPLEMENTED(...) ::cpp_essentials::core::detail::not_implemented(CODE_LOCATION, __VA_ARGS__)
#define EXPECTS(COND, ...) ::cpp_essentials::core::detail::handle(COND, #COND, CODE_LOCATION, __VA_ARGS__)
#define ENSURES(COND, ...) ::cpp_essentials::core::detail::handle(COND, #COND, CODE_LOCATION, __VA_ARGS__)

#endif /* CPP_ESSENTIALS_CORE_ASSERTIONS_HPP */
