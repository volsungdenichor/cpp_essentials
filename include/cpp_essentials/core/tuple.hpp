#ifndef CPP_ESSENTIALS_CORE_TUPLE_HPP_
#define CPP_ESSENTIALS_CORE_TUPLE_HPP_

#pragma once

#include <iostream>
#include <tuple>

namespace cpp_essentials::core
{

namespace detail
{

template <size_t Index, class T, class Func>
void visit_value(T&& value, Func&& func)
{
    if constexpr (std::is_invocable_v<Func, T, size_t>)
    {
        func(value, Index);
    }
    else
    {
        func(value);
    }
}

template <class Tuple, class Func, size_t... Index>
void visit(Tuple&& tuple, Func&& func, std::index_sequence<Index...>)
{
    auto dummy = { 0, (visit_value<Index>(std::get<Index>(tuple), func), 0)... };
    (void)dummy;
}

struct visit_fn
{
    template <class Tuple, class Func>
    void operator ()(Tuple&& tuple, Func&& func) const
    {
        visit(tuple, func, std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{});
    }
};

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

static constexpr detail::visit_fn visit = {};

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

