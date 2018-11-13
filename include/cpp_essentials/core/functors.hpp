#ifndef CPP_ESSENTIALS_CORE_FUNCTORS_HPP_
#define CPP_ESSENTIALS_CORE_FUNCTORS_HPP_

#pragma once

#include <algorithm>
#include <functional>
#include <tuple>

#include <cpp_essentials/math/constants.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct identity_fn
{
    template <class T>
    const T& operator ()(const T& value) const
    {
        return value;
    }
};

struct logical_negation_fn
{
    template <class Func>
    auto operator ()(Func func) const
    {
        return [this, func](auto&&... args)
        {
            return !func(std::forward<decltype(args)>(args)...);
        };
    }
};

struct approx_equal_to_fn
{
    template <class L, class R, class E>
    bool operator ()(const L& lhs, const R& rhs, const E& epsilon) const
    {
        return std::abs(lhs - rhs) <= epsilon;
    }

    template <class E>
    auto operator ()(const E& epsilon) const
    {
        return [this, epsilon](const auto& lhs, const auto& rhs)
        {
            return (*this)(lhs, rhs, epsilon);
        };
    }
};

struct clamp_fn
{
    template <class T>
    const T& operator ()(const T& value, const T& lower, const T& upper) const
    {
        return std::clamp(value, lower, upper);
    }

    template <class T>
    auto operator ()(const T& lower, const T& upper) const
    {
        return [this, lower, upper](const auto& value)
        {
            return (*this)(value, lower, upper);
        };
    }
};

struct between_fn
{
    template <class V, class T, class U>
    bool operator ()(const V& value, const T& lower, const U& upper) const
    {
        return lower <= value && value < upper;
    }

    template <class T, class U>
    auto operator ()(T lower, U upper) const
    {
        return [this, lower, upper](const auto& item)
        {
            return (*this)(item, lower, upper);
        };
    }
};

struct inclusive_between_fn
{
    template <class V, class T, class U>
    bool operator ()(const V& value, const T& lower, const U& upper) const
    {
        return lower <= value && value <= upper;
    }

    template <class T, class U>
    auto operator ()(T lower, U upper) const
    {
        return [this, lower, upper](const auto& item)
        {
            return (*this)(item, lower, upper);
        };
    }
};

struct min_fn
{
    template <class T>
    const T& operator ()(const T& lhs, const T& rhs) const
    {
        return std::min(lhs, rhs);
    }
};

struct max_fn
{
    template <class T>
    const T& operator ()(const T& lhs, const T& rhs) const
    {
        return std::max(lhs, rhs);
    }
};

struct lerp_fn
{
    template <class R, class T, class U>
    auto operator ()(const R& ratio, const T& a, const U& b) const
    {
        return ((R(1) - ratio) * a) + (ratio * b);
    }

    template <class R>
    auto operator ()(const R& ratio) const
    {
        return [this, ratio](const auto& a, const auto& b)
        {
            return (*this)(ratio, a, b);
        };
    }
};

struct make_pair_fn
{
    template <class L, class R>
    auto operator ()(L&& lhs, R&& rhs) const
    {
        return std::make_pair(std::forward<L>(lhs), std::forward<R>(rhs));
    }
};

struct make_tuple_fn
{
    template <class... Args>
    auto operator ()(Args&&... args) const
    {
        return std::make_tuple(std::forward<Args>(args)...);
    }
};

struct tie_fn
{
    template <class... Args>
    auto operator ()(Args&&... args) const
    {
        return std::tie(std::forward<Args>(args)...);
    }
};

template <std::size_t Index>
struct get_fn
{
    template <class T>
    decltype(auto) operator ()(T&& arg) const
    {
        return std::get<Index>(std::forward<T>(arg));
    }
};

struct has_value_fn
{
    template <class T>
    bool operator ()(T&& arg) const
    {
        return static_cast<bool>(arg);
    }
};

struct dereference_fn
{
    template <class T>
    decltype(auto) operator ()(T&& arg) const
    {
        return *arg;
    }
};

template <class Type>
struct cast_fn
{
    template <class T>
    decltype(auto) operator ()(const T& arg) const
    {
        return static_cast<Type>(arg);
    }
};

struct offset_of_fn
{
    template <class Type, class T>
    constexpr size_t operator ()(Type T::*ptr) const
    {
        // static_assert(std::is_standard_layout_v<T>, "Only standard layout types supported");
        return (char*)&(static_cast<T*>(nullptr)->*ptr) - (char*)static_cast<T*>(nullptr);
    }
};

} /* namespace detail */

static constexpr detail::identity_fn identity = {};
static constexpr detail::approx_equal_to_fn approx_equal_to = {};
static constexpr detail::clamp_fn clamp = {};
static constexpr detail::between_fn between = {};
static constexpr detail::inclusive_between_fn inclusive_between = {};
static constexpr detail::min_fn min = {};
static constexpr detail::max_fn max = {};
static constexpr detail::lerp_fn lerp = {};

static constexpr detail::logical_negation_fn logical_negation = {};

static constexpr detail::has_value_fn has_value = {};
static constexpr detail::dereference_fn dereference = {};

static constexpr detail::make_pair_fn make_pair = {};
static constexpr detail::make_tuple_fn make_tuple = {};
static constexpr detail::tie_fn tie = {};

template <std::size_t Index>
static constexpr detail::get_fn<Index> get = {};

template <class Type>
static constexpr detail::cast_fn<Type> cast = {};

static constexpr detail::offset_of_fn offset_of = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_FUNCTORS_HPP_ */
