#ifndef CPP_ESSENTIALS_GX_LOOKUP_TABLE_HPP_
#define CPP_ESSENTIALS_GX_LOOKUP_TABLE_HPP_

#pragma once

#include <functional>
#include <vector>

#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/gx/colors.hpp>

namespace cpp_essentials::gx
{

class lookup_table
{
public:
    lookup_table(std::vector<byte> lut)
        : _lut(std::move(lut))
    {
        EXPECTS(_lut.size() == 256);
    }

    byte operator ()(byte value) const
    {
        return _lut[value];
    }

    rgb_color operator ()(const rgb_color& value) const
    {
        rgb_color result;

        for (size_t i = 0; i < 3; ++i)
        {
            result[i] = (*this)(value[i]);
        }

        return result;
    }

    rgba_color operator ()(const rgba_color& value) const
    {
        rgba_color result;

        for (size_t i = 0; i < 3; ++i)
        {
            result[i] = (*this)(value[i]);
        }
        result[3] = value[3];

        return result;
    }

private:
    std::vector<byte> _lut;
};


inline lookup_table make_lut(std::function<int(int)> func)
{
    std::vector<byte> result(256);

    for (size_t i = 0; i < result.size(); ++i)
    {
        result[i] = to_byte(func(static_cast<int>(i)));
    }

    return result;
}


inline lookup_table operator *(const lookup_table& lhs, const lookup_table& rhs)
{
    return make_lut([&](int v) { return rhs(lhs(static_cast<byte>(v))); });
}

inline lookup_table identity()
{
    return make_lut([=](int v) { return v; });
}

inline lookup_table brightness(int value)
{
    return make_lut([=](int v) { return v + value; });
}

inline lookup_table threshold(byte value, byte lower, byte upper)
{
    return make_lut([=](int v) { return v < value ? lower : upper; });
}

inline lookup_table threshold(byte value)
{
    return threshold(value, 0, 255);
}

inline lookup_table negative()
{
    return make_lut([](int v) { return 255 - v; });
}

inline lookup_table contrast(float value, int center = 128)
{
    return make_lut([=](int v) { return int(value * (v - center) + center); });
}

inline lookup_table brightness_contrast(int brightness, float contrast, int center = 128)
{
    return make_lut([=](int v) { return int(contrast * (v + brightness - center) + center); });
}

inline lookup_table exposition(float value)
{
    return value < 0
        ? identity()
        : make_lut([=](int v) { return int(v * value); });
}

inline lookup_table levels_adjustment(const geo::interval<byte>& in, const geo::interval<byte>& out, float gamma = 1.F)
{
    return make_lut([=](int v) -> int
    {
        if (v < in.lower())
        {
            return out.lower();
        }

        if (v > in.upper())
        {
            return out.upper();
        }

        return int(out.lower() + out.size() * math::pow(float(v - in.lower()) / in.size(), 1.F / gamma));
    });
}

inline lookup_table gamma(float value)
{
    return levels_adjustment({ 0, 255 }, { 0, 255 }, value);
}

} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_LOOKUP_TABLE_HPP_ */
