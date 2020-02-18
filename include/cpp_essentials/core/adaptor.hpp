#ifndef CPP_ESSENTIALS_CORE_ADAPTOR_HPP_
#define CPP_ESSENTIALS_CORE_ADAPTOR_HPP_

#pragma once

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/core.hpp>

namespace cpp_essentials::core
{

template <class Func>
struct adaptor_t
{
    Func func;

    template <class T>
    auto operator ()(T&& item) const -> decltype(auto)
    {
        return func(FORWARD(item));
    }
};

template <class T, class Func>
auto operator |(T&& item, const adaptor_t<Func>& adaptor) -> decltype(auto)
{
    return adaptor(FORWARD(item));
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

    const adaptee_type _adaptee;

    constexpr adaptable(adaptee_type adaptee)
        : _adaptee{ std::move(adaptee) }
    {
    }


    template <class... Args>
    constexpr decltype(auto) operator ()(Args&&... args) const
    {
        return make_adaptor([&](auto&& item) -> decltype(auto)
        {
            static_assert(
                std::is_invocable_v<adaptee_type, decltype(item), Args...>,
                "Cannot create adaptor with given arguments");

            return _adaptee(FORWARD(item), FORWARD(args)...);
        });
    }
};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ADAPTOR_HPP_ */
