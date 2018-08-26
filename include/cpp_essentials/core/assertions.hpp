#ifndef CPP_ESSENTIALS_CORE_ASSERTIONS_HPP
#define CPP_ESSENTIALS_CORE_ASSERTIONS_HPP

#pragma once

#include <stdexcept>
#include <sstream>
#include <cpp_essentials/core/exceptions.hpp>

namespace cpp_essentials::core
{

namespace detail
{

inline void not_implemented(const char* file, int line, const char* function, const char* message = nullptr)
{
    std::stringstream ss;

    ss << "not implemented:" << std::endl;
    ss << "  message    : " << (message ? message : "");

    std::string what = ss.str();

    detail::throw_exception(std::runtime_error { what }, file, line, function);
}

inline void handle(bool condition, const char* expression, const char* file, int line, const char* function, const char* message = nullptr)
{
    if (condition)
    {
        return;
    }

    std::stringstream ss;
    ss << "expectation failure:" << std::endl;
    ss << "  message    : " << (message ? message : "") << std::endl;
    ss << "  condition  : " << expression;

    std::string what = ss.str();

    detail::throw_exception(std::runtime_error { what }, file, line, function);
}

} /* namespace detail */

} /* namespace cpp_essentials::core */

#if defined(__MINGW32__)
#  define NOT_IMPLEMENTED(...) ::core::detail::not_implemented(__FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#  define EXPECTS(COND, ...) ::core::detail::handle(COND, #COND, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#  define ENSURES(COND, ...) ::core::detail::handle(COND, #COND, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#elif defined(__GNUC__)
#  define NOT_IMPLEMENTED(...) ::core::detail::not_implemented(__FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#  define EXPECTS(COND, ...) ::core::detail::handle(COND, #COND, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#  define ENSURES(COND, ...) ::core::detail::handle(COND, #COND, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#elif defined(_MSC_VER)
#  define NOT_IMPLEMENTED(...) ::core::detail::not_implemented(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#  define EXPECTS(COND, ...) ::core::detail::handle(COND, #COND, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#  define ENSURES(COND, ...) ::core::detail::handle(COND, #COND, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#elif defined(__clang__)
#  define NOT_IMPLEMENTED(...) ::core::detail::not_implemented(__FILE__, __LINE__, __FUNCSIG__, __VA_ARGS__)
#  define EXPECTS(COND, ...) ::core::detail::handle(COND, #COND, __FILE__, __LINE__, __FUNCSIG__, __VA_ARGS__)
#  define ENSURES(COND, ...) ::core::detail::handle(COND, #COND, __FILE__, __LINE__, __FUNCSIG__, __VA_ARGS__)
#endif

#endif /* CPP_ESSENTIALS_CORE_ASSERTIONS_HPP */
