#ifndef CPP_ESSENTIALS_CORE_PREDICATES_HPP_
#define CPP_ESSENTIALS_CORE_PREDICATES_HPP_

#pragma once

#include <cpp_essentials/core/arithmetic_functors.hpp>

namespace cpp_essentials::core
{

template <class Pred>
struct predicate_t
{
    Pred pred;

    template <class T>
    constexpr bool operator ()(const T& value) const
    {
        if constexpr (std::is_invocable_v<Pred, T>)
        {
            return pred(value);
        }
        else
        {
            return value == pred;
        }
    }
};

template <class Pred>
constexpr predicate_t<Pred> make_predicate(Pred pred)
{
    return { std::move(pred) };
}

template <class Pred>
constexpr predicate_t<Pred> make_predicate(predicate_t<Pred> pred)
{
    return std::move(pred);
}

constexpr auto make_placeholder()
{
    return make_predicate([](const auto&) { return true; });
}

template <class P>
constexpr auto operator !(predicate_t<P> item)
{
    return make_predicate([=](const auto& value)
    {
        return !item(value);
    });
}

template <class L, class R>
constexpr auto operator &&(predicate_t<L> lhs, predicate_t<R> rhs)
{
    return make_predicate([=](const auto& value)
    {
        return lhs(value) && rhs(value);
    });
}

template <class L, class R>
constexpr auto operator ||(predicate_t<L> lhs, predicate_t<R> rhs)
{
    return make_predicate([=](const auto& value)
    {
        return lhs(value) || rhs(value);
    });
}

template <class P, class T>
constexpr auto operator ==(predicate_t<P> lhs, T rhs)
{
    return std::move(lhs) && make_predicate(core::equal_to(std::move(rhs)));
}

template <class P, class T>
constexpr auto operator !=(predicate_t<P> lhs, T rhs)
{
    return std::move(lhs) && make_predicate(core::not_equal_to(std::move(rhs)));
}

template <class P, class T>
constexpr auto operator <(predicate_t<P> lhs, T rhs)
{
    return std::move(lhs) && make_predicate(core::less(std::move(rhs)));
}

template <class P, class T>
constexpr auto operator >(predicate_t<P> lhs, T rhs)
{
    return std::move(lhs) && make_predicate(core::greater(std::move(rhs)));
}

template <class P, class T>
constexpr auto operator <=(predicate_t<P> lhs, T rhs)
{
    return std::move(lhs) && make_predicate(core::less_equal(std::move(rhs)));
}

template <class P, class T>
constexpr auto operator >=(predicate_t<P> lhs, T rhs)
{
    return std::move(lhs) && make_predicate(core::greater_equal(std::move(rhs)));
}

template <class T, class P>
constexpr auto operator ==(T lhs, predicate_t<P> rhs)
{
    return std::move(rhs) == std::move(lhs);
}

template <class T, class P>
constexpr auto operator !=(T lhs, predicate_t<P> rhs)
{
    return std::move(rhs) != std::move(lhs);
}

template <class T, class P>
constexpr auto operator <(T lhs, predicate_t<P> rhs)
{
    return std::move(rhs) > std::move(lhs);
}

template <class T, class P>
constexpr auto operator >(T lhs, predicate_t<P> rhs)
{
    return std::move(rhs) < std::move(lhs);
}

template <class T, class P>
constexpr auto operator <=(T lhs, predicate_t<P> rhs)
{
    return std::move(rhs) >= std::move(lhs);
}

template <class T, class P>
constexpr auto operator >=(T lhs, predicate_t<P> rhs)
{
    return std::move(rhs) <= std::move(lhs);
}

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_PREDICATES_HPP_ */