#ifndef CPP_ESSENTIALS_CORE_DETAIL_ITERATOR_FUNC_HELPER_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_ITERATOR_FUNC_HELPER_HPP_

#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>
#include <optional>

namespace cpp_essentials::core
{

namespace detail
{

template <class Func, class... Iters>
struct iterator_func_helper
{
    using tuple_type = std::tuple<Iters...>;
    using reference = std::tuple<typename std::iterator_traits<Iters>::reference...>;

#if _HAS_CXX17
    using type = std::invoke_result_t<Func, typename std::iterator_traits<Iters>::reference...>;
#else
    using type = std::result_of_t<Func(typename std::iterator_traits<Iters>::reference...)>;
#endif
};

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

    template <class... Args>
    decltype(auto) operator ()(Args&&... args) const
    {
        if constexpr (is_default_constructible)
        {
            return _func(std::forward<Args>(args)...);
        }
        else
        {
            return (*_func)(std::forward<Args>(args)...);
        }
    }

    impl_type _func;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_ITERATOR_FUNC_HELPER_HPP_ */
