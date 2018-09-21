#ifndef CPP_ESSENTIALS_CORE_FUNCTORS_HPP_
#define CPP_ESSENTIALS_CORE_FUNCTORS_HPP_

#pragma once

#include <algorithm>
#include <functional>
#include <tuple>

#include <cpp_essentials/core/math_constants.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct identity_t
{
    template <class T>
    const T& operator ()(const T& value) const
    {
        return value;
    }
};

struct logical_negation_t
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

struct approx_equal_to_t
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

struct clamp_t
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

struct between_t
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

struct inclusive_between_t
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

struct min_t
{
    template <class T>
    const T& operator ()(const T& lhs, const T& rhs) const
    {
        return std::min(lhs, rhs);
    }
};

struct max_t
{
    template <class T>
    const T& operator ()(const T& lhs, const T& rhs) const
    {
        return std::max(lhs, rhs);
    }
};

struct lerp_t
{
    template <class R, class T, class U>
    auto operator ()(const R& ratio, const T& a, const U& b) const
    {
        return ((one.as<R>() - ratio) * a) + (ratio * b);
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

struct make_pair_t
{
    template <class L, class R>
    auto operator ()(L&& lhs, R&& rhs) const
    {
        return std::make_pair(std::forward<L>(lhs), std::forward<R>(rhs));
    }
};

struct make_tuple_t
{
    template <class... Args>
    auto operator ()(Args&&... args) const
    {
        return std::make_tuple(std::forward<Args>(args)...);
    }
};

struct tie_t
{
    template <class... Args>
    auto operator ()(Args&&... args) const
    {
        return std::tie(std::forward<Args>(args)...);
    }
};

template <std::size_t Index>
struct get_t
{
    template <class T>
    decltype(auto) operator ()(T&& arg) const
    {
        return std::get<Index>(std::forward<T>(arg));
    }
};

struct dereference_t
{
    template <class T>
    decltype(auto) operator ()(T&& arg) const
    {
        return *arg;
    }
};

} /* namespace detail */

static constexpr detail::identity_t identity = {};
static constexpr detail::approx_equal_to_t approx_equal_to = {};
static constexpr detail::clamp_t clamp = {};
static constexpr detail::between_t between = {};
static constexpr detail::inclusive_between_t inclusive_between = {};
static constexpr detail::min_t min = {};
static constexpr detail::max_t max = {};
static constexpr detail::lerp_t lerp = {};

static constexpr detail::logical_negation_t logical_negation = {};

static constexpr detail::dereference_t dereference = {};

static constexpr detail::make_pair_t make_pair = {};
static constexpr detail::make_tuple_t make_tuple = {};
static constexpr detail::tie_t tie = {};

template <std::size_t Index>
static constexpr detail::get_t<Index> get = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_FUNCTORS_HPP_ */
