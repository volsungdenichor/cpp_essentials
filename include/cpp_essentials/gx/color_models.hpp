#ifndef CPP_ESSENTIALS_GX_COLOR_MODELS_HPP_
#define CPP_ESSENTIALS_GX_COLOR_MODELS_HPP_

#pragma once

#include <cpp_essentials/geo/matrix.hpp>
#include <cpp_essentials/core/algorithm.hpp>

#include <cpp_essentials/gx/core.hpp>

namespace cpp_essentials::gx
{

using rgb_proxy = geo::vector<double, 3>;

struct rgb_color
{
    using const_reference = const byte&;
    using reference = byte&;

    rgb_color()
        : rgb_color(0, 0, 0)
    {
    }

    rgb_color(byte r, byte g, byte b)
        : data({ { r, g, b } })
    {
    }

    rgb_color(byte value)
        : rgb_color(value, value, value)
    {
    }

    rgb_color(const rgb_proxy& proxy)
    {
        core::transform(proxy._data, data._data.begin(), [](auto v) { return static_cast<byte>(core::clamp(v, 0.0, 255.0)); });
    }

    operator byte() const
    {
        return gray();
    }

    operator rgb_proxy() const
    {
        return data;
    }

    byte gray() const
    {
        static const geo::vector<double, 3> weights = { 0.2989, 0.5870, 0.1140 };
        return static_cast<byte>(core::inner_product(data._data, weights._data, 0.0));
    }

    const_reference operator [](size_t index) const
    {
        return data[index];
    }

    reference operator [](size_t index)
    {
        return data[index];
    }

    const_reference red() const
    {
        return data[0];
    }

    reference red()
    {
        return data[0];
    }

    const_reference green() const
    {
        return data[1];
    }

    reference green()
    {
        return data[1];
    }

    const_reference blue() const
    {
        return data[2];
    }

    reference blue()
    {
        return data[2];
    }

    geo::vector<byte, 3> data;
};



inline bool operator ==(const rgb_color& lhs, const rgb_color& rhs)
{
    return lhs.data == rhs.data;
}

inline bool operator !=(const rgb_color& lhs, const rgb_color& rhs)
{
    return !(lhs == rhs);
}



inline rgb_proxy operator +(const rgb_color& lhs, const rgb_color& rhs)
{
    return static_cast<rgb_proxy>(lhs) + static_cast<rgb_proxy>(rhs);
}

inline rgb_color& operator +=(rgb_color& lhs, const rgb_proxy& rhs)
{
    lhs = lhs + rhs;
    return lhs;
}

inline rgb_proxy operator -(const rgb_color& lhs, const rgb_color& rhs)
{
    return static_cast<rgb_proxy>(lhs) - static_cast<rgb_proxy>(rhs);
}

inline rgb_color& operator -=(rgb_color& lhs, const rgb_proxy& rhs)
{
    lhs = lhs - rhs;
    return lhs;
}



template <typename T, CONCEPT = cc::Multiply<byte, T>>
rgb_proxy operator *(const rgb_color& lhs, T rhs)
{
    return static_cast<rgb_proxy>(lhs) * rhs;
}

template <typename T, CONCEPT = cc::Multiply<T, byte>>
rgb_proxy operator *(T lhs, const rgb_color& rhs)
{
    return rhs * lhs;
}

template <typename T, CONCEPT = cc::Multiply<T, byte>>
rgb_color& operator *=(rgb_color& lhs, T rhs)
{
    lhs = lhs * rhs;
    return lhs;
}

template <typename T, CONCEPT = cc::Divide<byte, T>>
rgb_proxy operator /(const rgb_color& lhs, T rhs)
{
    return static_cast<rgb_proxy>(lhs) / rhs;
}

template <typename T, CONCEPT = cc::Divide<byte, T>>
rgb_color& operator /=(rgb_color& lhs, T rhs)
{
    lhs = lhs / rhs;
    return lhs;
}

} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_COLOR_MODELS_HPP_*/