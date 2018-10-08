#ifndef CPP_ESSENTIALS_CORE_ADAPTOR_HPP_
#define CPP_ESSENTIALS_CORE_ADAPTOR_HPP_

#pragma once

#include <cpp_essentials/cc/cc.hpp>

namespace cpp_essentials::core
{

template <class Func>
struct adaptor_t
{
    Func func;

    template <class T>
    auto operator ()(T&& item) const -> decltype(auto)
    {
        return func(std::forward<T>(item));
    }
};

template <class T, class Func>
auto operator |(T&& item, const adaptor_t<Func>& adaptor) -> decltype(auto)
{
    return adaptor(std::forward<T>(item));
}

template <class Func>
auto make_adaptor(Func func) -> adaptor_t<Func>
{
    return { std::move(func) };
}

template <class F1, class F2>
auto operator >>(const adaptor_t<F1>& lhs, const adaptor_t<F2>& rhs)
{
    return make_adaptor([=](auto&& item)
    {
        return item | lhs | rhs;
    });
}


template <class Adaptee>
struct adaptable
{
    using adaptee_type = Adaptee;

    static_assert(
        std::is_default_constructible_v<adaptee_type>,
        "Adaptee must be default constructibe");

    static constexpr adaptee_type _adaptee = {};

    template <class... Args>
    auto operator ()(Args&&... args) const
    {
        if constexpr (std::is_invocable_v<adaptee_type, Args...>)
        {
            return _adaptee(std::forward<Args>(args)...);
        }
        else
        {
            return make_adaptor([&](auto&& item) -> decltype(auto)
            {
                static_assert(
                    std::is_invocable_v<adaptee_type, decltype(item), Args...>,
                    "Cannot create adaptor with given arguments");

                return _adaptee(std::forward<decltype(item)>(item), args...);
            });
        }
    }
};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ADAPTOR_HPP_ */
