#pragma once

#include <type_traits>
#include <optional>

namespace millrind::core
{

namespace detail
{

template <class Func>
struct default_constructible_func
{
    static constexpr bool is_default_constructible = std::is_default_constructible_v<Func>;

    using impl_type = std::conditional_t<is_default_constructible, Func, std::optional<Func>>;

    default_constructible_func() = default;

    default_constructible_func(Func func)
        : _func{ std::move(func) }
    {
    }

    default_constructible_func(const default_constructible_func&) = default;

    default_constructible_func(default_constructible_func&&) = default;

    default_constructible_func& operator =(default_constructible_func other)
    {
        _func.~impl_type();
        new(&_func) impl_type{ std::move(other._func) };
        return *this;
    }

    template <class... Args>
    decltype(auto) operator ()(Args&& ... args) const
    {
        if constexpr (is_default_constructible)
        {
            return _func(FORWARD(args)...);
        }
        else
        {
            return (*_func)(FORWARD(args)...);
        }
    }

    mutable impl_type _func;
};

} // namespace detail

} // namespace millrind::core