#ifndef CPP_ESSENTIALS_GX_COLOR_MODELS_HPP_
#define CPP_ESSENTIALS_GX_COLOR_MODELS_HPP_

#pragma once

#include <iomanip>

#include <cpp_essentials/geo/matrix.hpp>
#include <cpp_essentials/core/algorithm.hpp>

#include <cpp_essentials/gx/core.hpp>
#include <cpp_essentials/core/map.hpp>

namespace cpp_essentials::gx
{

using rgb_proxy = geo::vector<double, 3>;

namespace detail
{

struct to_byte_fn
{
    template <class T>
    byte operator ()(T v) const
    {
        return static_cast<byte>(core::clamp(v, T(0), T(255)));
    };
};

} /* namespace detail */

static constexpr auto to_byte = detail::to_byte_fn{};


struct rgb_color
{
    using proxy = rgb_proxy;
    using const_reference = const byte&;
    using reference = byte & ;

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
        core::transform(proxy._data, data._data.begin(), to_byte);
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

inline std::ostream& operator <<(std::ostream& os, const rgb_color& item)
{
    return os << core::delimit(core::views::map(item.data._data, core::cast<int>), ",");
}




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


template <typename T, CONCEPT = cc::Arithmetic<T>>
rgb_proxy operator *(const rgb_color& lhs, T rhs)
{
    return static_cast<rgb_proxy>(lhs) * rhs;
}

template <typename T, CONCEPT = cc::Arithmetic<T>>
rgb_proxy operator *(T lhs, const rgb_color& rhs)
{
    return rhs * lhs;
}

template <typename T, CONCEPT = cc::Arithmetic<T>>
rgb_color& operator *=(rgb_color& lhs, T rhs)
{
    lhs = lhs * rhs;
    return lhs;
}

template <typename T, CONCEPT = cc::Arithmetic<T>>
rgb_proxy operator /(const rgb_color& lhs, T rhs)
{
    return static_cast<rgb_proxy>(lhs) / rhs;
}

template <typename T, CONCEPT = cc::Arithmetic<T>>
rgb_color& operator /=(rgb_color& lhs, T rhs)
{
    lhs = lhs / rhs;
    return lhs;
}


struct rgba_color
{
    using const_reference = const byte&;
    using reference = byte & ;

    rgba_color(const rgb_color& color, byte a = 255)
        : data{ color[0], color[1], color[2], a }
    {
    }

    rgba_color(byte r, byte g, byte b, byte a)
        : data{ r, g, b, a }
    {
    }

    rgba_color()
        : rgba_color(rgb_color{})
    {
    }

    rgb_color to_rgb() const
    {
        return { data[0], data[1], data[2] };
    }

    operator rgb_color() const
    {
        return to_rgb();
    }

    byte gray() const
    {
        return to_rgb().gray();
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

    const_reference alpha() const
    {
        return data[3];
    }

    reference alpha()
    {
        return data[3];
    }

    geo::vector<byte, 4> data;
};

inline std::ostream& operator <<(std::ostream& os, const rgba_color& item)
{
    return os << core::delimit(core::views::map(item.data._data, core::cast<int>), ",");
}

inline bool operator ==(const rgba_color& lhs, const rgba_color& rhs)
{
    return lhs.data == rhs.data;
}

inline bool operator !=(const rgba_color& lhs, const rgba_color& rhs)
{
    return !(lhs == rhs);
}


struct hsv_color
{
    double h, s, v;
};

struct hsl_color
{
    double h, s, l;
};

struct cmyk_color
{
    double c, m, y, k;
};

struct yiq_color
{
    double y, i, q;
};

struct yuv_color
{
    double y, u, v;
};


namespace detail
{

struct to_hsv_fn
{
    hsv_color operator ()(const rgb_color& color) const
    {
        auto rgb = color.data / 255.0;

        auto[min, max] = core::minmax_element(core::return_value, rgb._data);

        auto span = max - min;

        auto h = 0.0;

        if (max == min)
        {
            h = 0.0;
        }
        else if (max == rgb[0] && rgb[1] >= rgb[2])
        {
            h = 60 * (rgb[1] - rgb[2]) / span;
        }
        else if (max == rgb[0] && rgb[1] < rgb[2])
        {
            h = 60 * (rgb[1] - rgb[2]) / span + 360;
        }
        else if (max == rgb[1])
        {
            h = 60 * (rgb[2] - rgb[0]) / span + 120;
        }
        else if (max == rgb[2])
        {
            h = 60 * (rgb[0] - rgb[1]) / span + 240;
        }

        double s = (max == 0) ? 0.0 : (1.0 - (min / max));

        return { h, s, max };
    }
};

struct to_hsl_fn
{
    hsl_color operator ()(const rgb_color& color) const
    {
        double h = 0.0, s = 0.0, l = 0.0;

        auto rgb = color.data / 255.0;

        auto[min, max] = core::minmax_element(core::return_value, rgb._data);

        auto span = max - min;

        if (min == max)
        {
            h = 0;
        }
        else if (max == rgb[0] && rgb[1] >= rgb[2])
        {
            h = 60.0 * (rgb[1] - rgb[2]) / span;
        }
        else if (max == rgb[0] && rgb[1] < rgb[2])
        {
            h = 60.0 * (rgb[1] - rgb[2]) / span + 360.0;
        }
        else if (max == rgb[1])
        {
            h = 60.0 * (rgb[2] - rgb[0]) / span + 120.0;
        }
        else if (max == rgb[2])
        {
            h = 60.0 * (rgb[0] - rgb[1]) / span + 240.0;
        }

        l = (max + min) / 2.0;

        if (l == 0 || max == min)
        {
            s = 0;
        }
        else if (0 < l && l <= 0.5)
        {
            s = (max - min) / (max + min);
        }
        else if (l > 0.5)
        {
            s = (max - min) / (2 - (max + min));
        }

        return { h, s, l };
    }
};

struct to_cmyk_fn
{
    cmyk_color operator ()(const rgb_color& color) const
    {
        auto c = (255.0 - color.red()) / 255.0;
        auto m = (255.0 - color.green()) / 255.0;
        auto y = (255.0 - color.blue()) / 255.0;
        auto k = std::min(c, std::min(m, y));

        if (k == 1.0)
        {
            return { 0, 0, 0, 1 };
        }
        else
        {
            return { (c - k) / (1 - k), (m - k) / (1 - k), (y - k) / (1 - k), k };
        }
    }
};

static const geo::square_matrix<double, 3> yiq_matrix =
{
    { +0.299, +0.587, +0.114 },
    { +0.596, -0.274, -0.322 },
    { +0.211, -0.523, +0.312 },
};

static const geo::square_matrix<double, 3> yuv_matrix =
{
    { +0.29900, +0.58700, +0.11400 },
    { -0.14713, -0.28886, +0.43600 },
    { +0.61500, -0.51499, -0.10001 },
};

struct to_yiq_fn
{
    yiq_color operator ()(const rgb_color& color) const
    {
        static const auto weights = yiq_matrix;

        auto result = color.data * weights;

        return yiq_color{ result[0], result[1], result[2] };
    }
};

struct to_yuv_fn
{
    yuv_color operator ()(const rgb_color& color) const
    {
        static const auto weights = yuv_matrix;

        auto result = color.data * weights;

        return yuv_color{ result[0], result[1], result[2] };
    }
};

struct to_rgb_fn
{
    rgb_color operator ()(const hsv_color& color) const
    {
        geo::vector<double, 3> result;

        if (core::approx_equal_to(color.s, 0.0, std::numeric_limits<double>::epsilon()))
        {
            core::fill(result._data, color.v);
        }
        else
        {
            auto sector_pos = color.h / 60.0;
            auto sector_index = int(math::floor(sector_pos));
            auto fractional_sector = sector_pos - sector_index;

            auto p = color.v * (1.0 - color.s);
            auto q = color.v * (1.0 - (color.s * fractional_sector));
            auto t = color.v * (1.0 - (color.s * (1 - fractional_sector)));

            switch (sector_index % 6)
            {
                case 0:
                    result = { color.v, t, p };
                    break;

                case 1:
                    result = { q, color.v, p };
                    break;

                case 2:
                    result = { p, color.v, t };
                    break;

                case 3:
                    result = { p, q, color.v };
                    break;

                case 4:
                    result = { t, p, color.v };
                    break;

                case 5:
                    result = { color.v, p, q };
                    break;

                default:
                    break;
            }
        }

        return 255.0 * result;
    }

    rgb_color operator ()(const hsl_color& color) const
    {
        geo::vector<double, 3> result;

        if (color.s == 0.0)
        {
            core::fill(result._data, color.l * 255.0);
        }
        else
        {
            auto q = (color.l < 0.5) ? (color.l * (1.0 + color.s)) : (color.l + color.s - (color.l * color.s));
            auto p = (2.0 * color.l) - q;

            auto hk = color.h / 360.0;
            result = { hk + (1.0 / 3.0), hk, hk - (1.0 / 3.0) };

            for (int i = 0; i < 3; i++)
            {
                if (result[i] < 0) result[i] += 1.0;
                if (result[i] > 1) result[i] -= 1.0;

                if ((result[i] * 6) < 1)
                {
                    result[i] = p + ((q - p) * 6.0 * result[i]);
                }
                else if ((result[i] * 2.0) < 1)
                {
                    result[i] = q;
                }
                else if ((result[i] * 3.0) < 2)
                {
                    result[i] = p + (q - p) * ((2.0 / 3.0) - result[i]) * 6.0;
                }
                else
                {
                    result[i] = p;
                }
            }
        }

        return 255.0 * result;
    }

    rgb_color operator ()(const cmyk_color& color) const
    {
        auto r = (1.0 - color.c) * (1.0 - color.k) * 255.0;
        auto g = (1.0 - color.m) * (1.0 - color.k) * 255.0;
        auto b = (1.0 - color.y) * (1.0 - color.k) * 255.0;

        return rgb_proxy{ r, g, b };
    }

    rgb_color operator ()(const yiq_color& color) const
    {
        static const auto weights = geo::invert(yiq_matrix).value();

        geo::vector<double, 3> vect = { color.y, color.i, color.q };

        return vect * weights;
    }

    rgb_color operator ()(const yuv_color& color) const
    {
        static const auto weights = geo::invert(yuv_matrix).value();

        geo::vector<double, 3> vect = { color.y, color.u, color.v };

        return vect * weights;
    }
};

} /* namespace detail */

static constexpr auto to_hsv = detail::to_hsv_fn{};
static constexpr auto to_hsl = detail::to_hsl_fn{};
static constexpr auto to_cmyk = detail::to_cmyk_fn{};
static constexpr auto to_yiq = detail::to_yiq_fn{};
static constexpr auto to_yuv = detail::to_yuv_fn{};
static constexpr auto to_rgb = detail::to_rgb_fn{};


} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_COLOR_MODELS_HPP_*/
