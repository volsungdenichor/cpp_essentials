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

struct pow_t
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

struct sqr_t
{
    template <class T>
    auto operator ()(const T& v) const
    {
        return v * v;
    }
};

struct sqrt_t
{
    template <class T>
    auto operator ()(const T& v) const
    {
        return std::sqrt(v);
    }
};

struct exp_t
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::exp(x);
    }
};

struct sin_t
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::sin(x);
    }
};

struct cos_t
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::cos(x);
    }
};

struct tan_t
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::tan(x);
    }
};

struct cot_t
{
    template <class T>
    auto operator ()(T x) const
    {
        return T(1) / std::tan(x);
    }
};

struct acos_t
{
    template <class T>
    auto operator ()(T x) const
    {
        return std::acos(x);
    }
};

struct atan2_t
{
    template <class T>
    auto operator ()(T y, T x) const
    {
        return std::atan2(y, x);
    }
};

struct rad_t
{
    template <class T>
    auto operator ()(T d) const
    {
        static const T multiplier = pi.as<T>() / T(180);
        return d * multiplier;
    }
};

struct deg_t
{
    template <class T>
    auto operator ()(T r) const
    {
        static const T multiplier = T(180) / pi.as<T>();
        return r * multiplier;
    }
};

} /* namespace detail */

static constexpr detail::pow_t pow = {};
static constexpr detail::sqr_t sqr = {};
static constexpr detail::sqrt_t sqrt = {};
static constexpr detail::exp_t exp = {};
static constexpr detail::sin_t sin = {};
static constexpr detail::cos_t cos = {};
static constexpr detail::tan_t tan = {};
static constexpr detail::cot_t cot = {};
static constexpr detail::acos_t acos = {};
static constexpr detail::atan2_t atan2 = {};
static constexpr detail::rad_t rad = {};
static constexpr detail::deg_t deg = {};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_FUNCTORS_HPP_ */


