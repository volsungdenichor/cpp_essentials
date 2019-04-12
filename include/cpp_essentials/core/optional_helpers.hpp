#ifndef CPP_ESSENTIALS_CORE_OPTIONAL_HELPERS_HPP_
#define CPP_ESSENTIALS_CORE_OPTIONAL_HELPERS_HPP_

#pragma once

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/optional.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct then_fn
{
    template
        < class T
        , class Func
        , CONCEPT = cc::Optional<T>>
    decltype(auto) operator ()(T&& item, Func func) const
    {
        using result_type = decltype(func(*item));
        using optional_type = optional<result_type>;
        return item ? optional_type{ func(*item) } : optional_type{};
    }
};

struct unwrap_fn
{
    template <class T, CONCEPT = cc::Optional<T>>
    decltype(auto) operator ()(T&& item) const
    {
        ensures(static_cast<bool>(item), "unwrapping null value");
        return *item;
    }
};

struct unwrap_or_throw_fn
{
    template
        < class T
        , class Exception
        , CONCEPT = cc::Optional<T>
        , CONCEPT = cc::BaseOf<std::exception, Exception>>
    decltype(auto) operator ()(T&& item, const Exception& ex) const
    {
        if (!item)
        {
            throw ex;
        }
        return *item;
    }

    template <class T, CONCEPT = cc::Optional<T>>
    decltype(auto) operator ()(T&& item, const std::string& message) const
    {
        return (*this)(std::forward<T>(item), std::runtime_error{ message });
    }
};

struct unwrap_or_fn
{
    template <class T, class V, CONCEPT = cc::Optional<T>>
    decltype(auto) operator ()(T&& item, V default_value) const
    {
        return item ? *item : default_value;
    }
};

struct unwrap_or_default_fn
{
    template <class T, CONCEPT = cc::Optional<T>>
    decltype(auto) operator ()(T&& item) const
    {
        using result_type = std::decay_t<decltype(*item)>;
        return item ? *item : result_type{};
    }
};

struct unwrap_or_eval_fn
{
    template <class T, class Func, CONCEPT = cc::Optional<T>>
    decltype(auto) operator ()(T&& item, Func func) const
    {
        return item ? *item : func();
    }
};

} /* namespace detail */

static constexpr auto then = adaptable{ detail::then_fn{} };
static constexpr auto unwrap = adaptable{ detail::unwrap_fn{} };
static constexpr auto unwrap_or_throw = adaptable{ detail::unwrap_or_throw_fn{} };
static constexpr auto unwrap_or = adaptable{ detail::unwrap_or_fn{} };
static constexpr auto unwrap_or_default = adaptable{ detail::unwrap_or_default_fn{} };
static constexpr auto unwrap_or_eval = adaptable{ detail::unwrap_or_eval_fn{} };

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_OPTIONAL_HELPERS_HPP_ */