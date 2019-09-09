#ifndef CPP_ESSENTIALS_CORE_DEBUG_UTILS_HPP_
#define CPP_ESSENTIALS_CORE_DEBUG_UTILS_HPP_

#pragma once

#include <string_view>
#include <stdexcept>

namespace cpp_essentials::core
{

struct code_location
{
    std::string_view file;
    int line;
    std::string_view function;

    friend std::ostream& operator <<(std::ostream& os, const code_location& item)
    {
        return os << item.file << ":" << item.line << ":" << item.function;
    }
};

template <class Func>
decltype(auto) try_invoke(Func&& func, const code_location& loc)
{
    try
    {
        return func();
    }
    catch (std::exception& ex)
    {
        throw std::runtime_error{ core::str(ex.what(), " [", loc, "]") };
    }
}

} /* namespace cpp_essentials::core */

#define CODE_LOCATION ::cpp_essentials::core::code_location{ __FILE__, __LINE__, __FUNCTION__ }
#define TRY_INVOKE(...) ::cpp_essentials::core::try_invoke([&]() -> decltype(auto) { return (__VA_ARGS__); }, CODE_LOCATION)

#endif /* CPP_ESSENTIALS_CORE_DEBUG_UTILS_HPP_ */
