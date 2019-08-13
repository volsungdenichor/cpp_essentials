#ifndef CPP_ESSENTIALS_CORE_MATCH_HPP_
#define CPP_ESSENTIALS_CORE_MATCH_HPP_

#pragma once

#include <variant>

namespace cpp_essentials::core
{

namespace detail
{

template <class... T>
struct overloaded : T...
{
    template <class ...Matchers>
    overloaded(Matchers&& ...matchers)
        : T{ std::forward<Matchers>(matchers) }...
    {
    }

    using T::operator()...;
};

template <class... Matchers>
auto make_overload(Matchers&&... matchers) -> overloaded<std::remove_reference_t<Matchers>...>
{
    return { std::forward<Matchers>(matchers)... };
}

struct match_fn
{
    template <class T, class... Matchers>
    decltype(auto) operator ()(T&& item, Matchers&&... matchers) const
    {
        return std::visit(make_overload(std::forward<Matchers>(matchers)...), std::forward<T>(item));
    }
};

} /* namespace detail */

static constexpr detail::match_fn match = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_MATCH_HPP_ */
