#ifndef CPP_ESSENTIALS_CORE_TUPLE_HPP_
#define CPP_ESSENTIALS_CORE_TUPLE_HPP_

#pragma once

#include <iostream>
#include <tuple>

namespace cpp_essentials::core
{

namespace detail
{

template <class T, std::size_t Index, std::size_t Last>
struct tuple_printer
{
    static void print(std::ostream& os, const T& value, const char* separator)
    {
        os << std::get<Index>(value) << separator;
        tuple_printer<T, Index + 1, Last>::print(os, value, separator);
    }
};

template <class T, std::size_t Index>
struct tuple_printer<T, Index, Index>
{
    static void print(std::ostream& os, const T& value, const char* separator)
    {
        os << std::get<Index>(value);
    }
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

namespace std
{

template <class... Args>
std::ostream& operator <<(std::ostream& os, const std::tuple<Args...>& value)
{
    os << "(";

    cpp_essentials::core::detail::tuple_printer<std::tuple<Args...>, 0, sizeof...(Args) - 1>::print(os, value, ";");

    os << ")";

    return os;
}

template <class T1, class T2>
std::ostream& operator <<(std::ostream& os, const std::pair<T1, T2>& value)
{
    os << "(";

    cpp_essentials::core::detail::tuple_printer<std::pair<T1, T2>, 0, 1>::print(os, value, ";");

    os << ")";

    return os;
}

} /* namespace std */

#endif /* CPP_ESSENTIALS_CORE_TUPLE_HPP_ */

