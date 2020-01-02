#ifndef CPP_ESSENTIALS_MATH_FUNCTORS_HPP_
#define CPP_ESSENTIALS_MATH_FUNCTORS_HPP_

#pragma once

#include <cmath>
#include <functional>

#include <cpp_essentials/math/constants.hpp>

namespace cpp_essentials::math
{

namespace detail
{

struct abs_fn
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::abs(x);
    }
};

struct sgn_fn
{
    template <class T>
    auto operator ()(T x) const -> int
    {
        if (x > zero)
        {
            return +1;
        }
        else if (x < zero)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
};

struct pow_fn
{
    template <class X, class Y>
    auto operator ()(X x, Y y) const
    {
        return std::pow(x, y);
    }

    template <class Y>
    auto operator ()(Y y) const
    {
        return [this, y](auto x)
        {
            return (*this)(x, y);
        };
    }
};

struct sqr_fn
{
    template <class T>
    auto operator ()(const T& v) const
    {
        return v * v;
    }
};

struct sqrt_fn
{
    template <class T>
    auto operator ()(const T& v) const
    {
        return std::sqrt(v);
    }
};

struct exp_fn
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::exp(x);
    }
};

struct sin_fn
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::sin(x);
    }
};

struct cos_fn
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::cos(x);
    }
};

struct tan_fn
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::tan(x);
    }
};

struct cot_fn
{
    template <class T>
    auto operator ()(T x) const
    {
        return T(1) / std::tan(x);
    }
};

struct asin_fn
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::asin(x);
    }
};

struct acos_fn
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::acos(x);
    }
};

struct atan2_fn
{
    template <class T>
    auto operator ()(T y, T x) const
    {
        return std::atan2(y, x);
    }
};

struct floor_fn
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::floor(x);
    }
};

struct ceil_fn
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::ceil(x);
    }
};

struct round_fn
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::round(x);
    }
};

struct rad_fn
{
    template <class T>
    auto operator ()(T d) const
    {
        static const T multiplier = pi.as<T>() / T(180);
        return d * multiplier;
    }
};

struct deg_fn
{
    template <class T>
    auto operator ()(T r) const
    {
        static const T multiplier = T(180) / pi.as<T>();
        return r * multiplier;
    }
};

} /* namespace detail */

static constexpr auto sgn = detail::sgn_fn{};
static constexpr auto abs = detail::abs_fn{};
static constexpr auto pow = detail::pow_fn{};
static constexpr auto sqr = detail::sqr_fn{};
static constexpr auto sqrt = detail::sqrt_fn{};
static constexpr auto exp = detail::exp_fn{};
static constexpr auto sin = detail::sin_fn{};
static constexpr auto cos = detail::cos_fn{};
static constexpr auto tan = detail::tan_fn{};
static constexpr auto cot = detail::cot_fn{};
static constexpr auto asin = detail::asin_fn{};
static constexpr auto acos = detail::acos_fn{};
static constexpr auto atan2 = detail::atan2_fn{};
static constexpr auto floor = detail::floor_fn{};
static constexpr auto ceil = detail::ceil_fn{};
static constexpr auto round = detail::round_fn{};
static constexpr auto rad = detail::rad_fn{};
static constexpr auto deg = detail::deg_fn{};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_FUNCTORS_HPP_ */


