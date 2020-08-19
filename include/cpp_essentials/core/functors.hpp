#ifndef CPP_ESSENTIALS_CORE_FUNCTORS_HPP_
#define CPP_ESSENTIALS_CORE_FUNCTORS_HPP_

#pragma once

#include <algorithm>
#include <functional>
#include <tuple>

#include <cpp_essentials/core/core.hpp>
#include <cpp_essentials/math/constants.hpp>
#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/core/tuple.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct identity_fn
{
    template <class T>
    T&& operator ()(T&& value) const
    {
        return FORWARD(value);
    }
};

struct logical_negation_fn
{
    template <class Func>
    auto operator ()(Func func) const
    {
        return [this, func](auto&&... args)
        {
            return !func(FORWARD(args)...);
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
        using common = std::common_type_t<std::common_type_t<V, T>, U>;
        return (common)lower <= (common)value && (common)value < (common)upper;
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
        return std::make_pair(FORWARD(lhs), FORWARD(rhs));
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
        return std::tie(FORWARD(args)...);
    }
};

template <std::size_t Index>
struct get_fn
{   
    template <class T>
    decltype(auto) operator ()(T&& arg) const
    {
        return std::get<Index>(FORWARD(arg));
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

struct pipe_fn
{
    template <class Func, class... Tail>
    auto operator ()(Func&& func, Tail&&... tail) const
    {
        if constexpr (sizeof...(tail) == 0)
        {
            return func;
        }
        else
        {
            return [=](auto&& item) { return (*this)(std::move(tail)...)(func(item)); };
        }
    }
};

struct compare_by_fn
{
    template <class Func, class Compare = std::less<>>
    auto operator ()(Func func, Compare compare = {}) const
    {
        auto f = cc::make_func(func);
        return [=](const auto& lhs, const auto& rhs)
        {
            return compare(f(lhs), f(rhs));
        };
    }
};

struct tie_members_fn
{
    template <class... F>
    auto operator ()(F... fields) const
    {
        return [=](const auto& item)
        {
            return std::tie(cc::make_func(fields)(item)...);
        };
    }
};

template <class Compare>
struct compare_by_members_fn
{
    template <class... F>
    auto operator ()(F... fields) const
    {
        static constexpr auto _compare = Compare{};
        static constexpr auto _tie_members = tie_members_fn{};
        static constexpr auto _compare_by = compare_by_fn{};
        return _compare_by(_tie_members(std::move(fields)...), _compare);
    }
};

struct hash_fn
{
    template <class T>
    size_t operator ()(const T& item) const
    {       
        return std::hash<T>{}(item);
    }
};

struct all_fn
{
    template <class... Preds>
    auto operator ()(Preds&&... preds) const
    {
        auto t = std::tuple<Preds...>{ FORWARD(preds)... };
        return [=](auto&& item)
        {
            bool result = true;
            core::visit(t, [&](auto&& pred)
            {
                result &= pred(item);
            });
            return result;
        };
    }
};

struct any_fn
{
    template <class... Preds>
    auto operator ()(Preds&&... preds) const
    {
        auto t = std::tuple<Preds...>{ FORWARD(preds)... };
        return [=](auto&& item)
        {
            bool result = false;
            core::visit(t, [&](auto&& pred)
            {
                result |= pred(item);
            });
            return result;
        };
    }
};

struct unpack_fn
{
    template <class BinaryFunc>
    auto operator ()(BinaryFunc func) const
    {
        return [=](auto&& tuple) -> decltype(auto)
        {
            return std::apply(func, tuple);
        };
    }
};

} /* namespace detail */

static constexpr auto identity = detail::identity_fn{};
static constexpr auto approx_equal_to = detail::approx_equal_to_fn{};
static constexpr auto clamp = detail::clamp_fn{};
static constexpr auto between = detail::between_fn{};
static constexpr auto inclusive_between = detail::inclusive_between_fn{};
static constexpr auto min = detail::min_fn{};
static constexpr auto max = detail::max_fn{};
static constexpr auto lerp = detail::lerp_fn{};

static constexpr auto logical_negation = detail::logical_negation_fn{};

static constexpr auto has_value = detail::has_value_fn{};
static constexpr auto dereference = detail::dereference_fn{};

static constexpr auto make_pair = detail::make_pair_fn{};
static constexpr auto make_tuple = detail::make_tuple_fn{};
static constexpr auto tie = detail::tie_fn{};

template <std::size_t Index>
static constexpr auto get = detail::get_fn<Index>{};

static constexpr auto get_key = detail::get_fn<0>{};
static constexpr auto get_value = detail::get_fn<1>{};

template <class Type>
static constexpr auto cast = detail::cast_fn<Type>{};

static constexpr auto offset_of = detail::offset_of_fn{};
static constexpr auto pipe = detail::pipe_fn{};
static constexpr auto compare_by = detail::compare_by_fn{};
static constexpr auto tie_members = detail::tie_members_fn{};

static constexpr auto asc_by = detail::compare_by_members_fn<std::less<>>{};
static constexpr auto desc_by = detail::compare_by_members_fn<std::greater<>>{};

static constexpr auto hash = detail::hash_fn{};

static constexpr auto all = detail::all_fn{};
static constexpr auto any = detail::any_fn{};

static constexpr auto unpack = detail::unpack_fn{};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_FUNCTORS_HPP_ */
