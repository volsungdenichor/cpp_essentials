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

static constexpr detail::sgn_fn sgn = {};
static constexpr detail::abs_fn abs = {};
static constexpr detail::pow_fn pow = {};
static constexpr detail::sqr_fn sqr = {};
static constexpr detail::sqrt_fn sqrt = {};
static constexpr detail::exp_fn exp = {};
static constexpr detail::sin_fn sin = {};
static constexpr detail::cos_fn cos = {};
static constexpr detail::tan_fn tan = {};
static constexpr detail::cot_fn cot = {};
static constexpr detail::acos_fn acos = {};
static constexpr detail::atan2_fn atan2 = {};
static constexpr detail::floor_fn floor = {};
static constexpr detail::round_fn round = {};
static constexpr detail::rad_fn rad = {};
static constexpr detail::deg_fn deg = {};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_FUNCTORS_HPP_ */


