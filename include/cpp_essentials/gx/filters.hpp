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

inline rgb_color apply(const rgb_color& lhs, const rgb_color& rhs, const core::function<int, int, int>& func)
{
    rgb_color result;
    core::transform(lhs.data._data, rhs.data._data, result.data._data.begin(), func);
    return result;
}

struct blend
{
    blend(float ratio)
        : _ratio{ ratio }
    {
    }

    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        return core::lerp(_ratio, lhs, rhs);
    }

    float _ratio;
};

struct normal
{
    rgb_color operator ()(const rgb_color& /*lhs*/, const rgb_color& rhs) const
    {
        return rhs;
    }
};

struct darker
{
    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        return apply(lhs, rhs, core::min);
    }
};

struct lighter
{
    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        return apply(lhs, rhs, core::max);
    }
};

struct multiply
{
    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        return apply(lhs, rhs, [](int lt, int rt) { return lt * rt / 255; });
    }
};

struct screen
{
    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        return apply(lhs, rhs, [](int lt, int rt) { return 255 - (255 - lt) * (255 - rt) / 255; });
    }
};

struct difference
{
    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        return apply(lhs, rhs, [](int lt, int rt) { return math::abs(lt - rt); });
    }
};

struct overlay
{
    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        return apply(lhs, rhs, [](int lt, int rt)
        {
            return lt > 128
                ? 255 - 2 * (255 - lt) * (255 - rt) / 255
                : 2 * lt * rt / 255;
        });
    }
};

struct add
{
    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        return lhs + rhs;
    }
};

struct subtract
{
    rgb_color operator ()(const rgb_color& lhs, const rgb_color& rhs) const
    {
        return apply(lhs, rhs, [](int lt, int rt) { return lt + rt - 255; });
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

