#pragma once

#include <type_traits>
#include <functional>

#include <millrind/core/macros.hpp>

namespace millrind::core
{

template <class T, class = std::enable_if_t<!std::is_const_v<T>>>
class mut_ref : public std::reference_wrapper<T>
{
public:
    constexpr explicit mut_ref(T& value)
        : std::reference_wrapper<T>{ value }
    {
    }
};

template <class T>
class ref : public std::reference_wrapper<const T>
{
public:
    constexpr explicit ref(const T& value)
        : std::reference_wrapper<const T>{ value }
    {
    }
};

struct invoke_func_fn
{
    template <class Func, class... Args>
    constexpr decltype(auto) operator()(Func&& func, Args&&... args) const
    {
        return std::invoke(FORWARD(func), FORWARD(args)...);
    }
};

static constexpr auto invoke_func = invoke_func_fn{};

template <class Func>
struct function_wrapper
{
    Func func;

    template <class... Args>
    constexpr decltype(auto) operator()(Args&&... args) const
    {
        return invoke_func(func, FORWARD(args)...);
    }
};

template <class Func>
struct function_wrapper<function_wrapper<Func>> : function_wrapper<Func>
{

};

struct wrap_func_fn
{
    template <class Func>
    constexpr auto operator()(Func func) const -> function_wrapper<Func>
    {
        return { std::move(func) };
    }    
};

static constexpr auto wrap_func = wrap_func_fn{};

} // namespace millrind::core