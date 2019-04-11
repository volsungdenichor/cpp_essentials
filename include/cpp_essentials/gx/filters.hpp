#ifndef CPP_ESSENTIALS_GX_COLOR_FILTERS_HPP_
#define CPP_ESSENTIALS_GX_COLOR_FILTERS_HPP_

#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/math/functors.hpp>
#include <cpp_essentials/gx/color_models.hpp>

#pragma once

namespace cpp_essentials::gx
{

struct heatmap
{
    heatmap()
    {
        for (size_t i = 0; i < 256; ++i)
        {
            auto gray = i / 255.0;

            rgb_color result;

            result.red() = to_byte(std::min(gray / 0.4, 1.0) * 255 + 0.5);

            if (gray > 0.4)
            {
                result.green() = to_byte(std::min((gray - 0.4) / 0.4, 1.0) * 255 + 0.5);
            }

            if (gray > 0.8)
            {
                result.blue() = to_byte(std::min((gray - 0.8) / 0.2, 1.0) * 255 + 0.5);
            }

            values[i] = result;
        }
    }

    rgb_color operator ()(byte value) const
    {
        return values[value];
    }

    std::array<rgb_color, 256> values;
};

struct sepia
{
    rgb_color operator ()(const rgb_color& color) const
    {
        static const std::array<geo::vector<double, 3>, 3> weights =
        {
            geo::vector<double, 3> { 0.393, 0.769, 0.189 },
            geo::vector<double, 3> { 0.349, 0.686, 0.168 },
            geo::vector<double, 3> { 0.272, 0.534, 0.131 },
        };

        rgb_color result;

        for (size_t i = 0; i < 3; ++i)
        {
            result[i] = to_byte(core::inner_product(color.data._data, weights[i]._data, 0.0));
        }

        return result;
    }
};

struct grayscale
{
    rgb_color operator ()(const rgb_color& color) const
    {
        return color.gray();
    }
};

namespace filters
{

template <class Self>
struct uniform_color_filter
{
    using self_type = Self;

    byte operator ()(byte lhs, byte rhs) const
    {
        return to_byte(self().apply(lhs, rhs));
    }
    
    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        rgb_color result;
        core::transform(lhs.data._data, rhs.data._data, result.data._data.begin(), [&](int lt, int rt) { return to_byte(self().apply(lt, rt)); });
        return result;
    }

private:
    const self_type& self() const
    {
        return static_cast<const self_type&>(*this);
    }
};

struct blend : uniform_color_filter<blend>
{
    blend(float ratio)
        : _ratio{ ratio }
    {
    }

    int apply(int lhs, int rhs) const
    {
        return static_cast<int>(core::lerp(_ratio, lhs, rhs));
    }   

    float _ratio;
};

struct normal : uniform_color_filter<normal>
{
    int apply(int lhs, int rhs) const
    {
        return rhs;
    }
};

struct darker : uniform_color_filter<darker>
{
    int apply(int lhs, int rhs) const
    {
        return core::min(lhs, rhs);
    }
};

struct lighter : uniform_color_filter<lighter>
{
    int apply(int lhs, int rhs) const
    {
        return core::min(lhs, rhs);
    }
};

struct multiply : uniform_color_filter<multiply>
{
    int apply(int lhs, int rhs) const
    {
        return lhs * rhs / 255;
    }
};

struct screen : uniform_color_filter<screen>
{
    int apply(int lhs, int rhs) const
    {
        return 255 - (255 - lhs) * (255 - rhs) / 255;
    }
};

struct difference : uniform_color_filter<difference>
{
    int apply(int lhs, int rhs) const
    {
        return math::abs(lhs - rhs);
    }
};

struct overlay : uniform_color_filter<overlay>
{
    int apply(int lhs, int rhs) const
    {
        return lhs > 128
            ? 255 - 2 * (255 - lhs) * (255 - rhs) / 255
            : 2 * lhs * rhs / 255;
    }
};

struct add : uniform_color_filter<add>
{
    int apply(int lhs, int rhs) const
    {
        return lhs + rhs;
    }
};

struct subtract : uniform_color_filter<subtract>
{
    int apply(int lhs, int rhs) const
    {
        return lhs + rhs - 255;
    }
};

struct hue
{
    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        auto lt = to_hsv(lhs);
        auto rt = to_hsv(rhs);

        return to_rgb(hsv_color{ rt.h, lt.s, lt.v });
    }
};

struct saturation
{
    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        auto lt = to_hsv(lhs);
        auto rt = to_hsv(rhs);

        return to_rgb(hsv_color{ lt.h, rt.s, lt.v });
    }
};

struct color
{
    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        auto lt = to_hsv(lhs);
        auto rt = to_hsv(rhs);

        return to_rgb(hsv_color{ rt.h, rt.s, lt.v });
    }
};

struct luminosity
{
    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        auto lt = to_hsv(lhs);
        auto rt = to_hsv(rhs);

        return to_rgb(hsv_color{ lt.h, lt.s, rt.v });
    }
};

} /* namespace filters */

} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_COLOR_FILTERS_HPP_ */

