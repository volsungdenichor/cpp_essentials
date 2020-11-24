#pragma once

#include <functional>
#include <millrind/core/type_traits.hpp>
#include <millrind/core/macros.hpp>

namespace millrind::core
{

namespace detail
{

struct identity_fn
{
    template <class T>
    constexpr T&& operator ()(T&& value) const
    {
        return FORWARD(value);
    }

    template <class T>
    constexpr T& operator ()(T& value) const
    {
        return FORWARD(value);
    }
};

struct logical_negation_fn
{
    template <class Func>
    constexpr auto operator ()(Func func) const
    {
        return [func](auto&&... args)
        {
            return !invoke_func(func, FORWARD(args)...);
        };
    }
};

} /* namespace detail */

static constexpr auto identity = detail::identity_fn{};
static constexpr auto logical_negation = detail::logical_negation_fn{};

} /* namespace millrind::core */
