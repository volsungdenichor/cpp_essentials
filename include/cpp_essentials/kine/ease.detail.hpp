#pragma once

#include <cpp_essentials/math/functors.hpp>

namespace cpp_essentials::kine
{

namespace detail
{

inline float elastic_in(float t, float b, float c, float d, float a, float p)
{
    if (t == 0) return b;
    float t_adj = t / d;
    if (t_adj == 1) return b + c;

    float s;
    if (a < std::abs(c))
    {
        a = c;
        s = p / 4.0f;
    }
    else
    {
        s = p / (math::two_pi.as<float>()) * std::asin(c / a);
    }

    t_adj -= 1;
    return -(a * std::pow(2.0f, 10 * t_adj) * std::sin((t_adj * d - s) * (math::two_pi.as<float>()) / p)) + b;
}

inline float elastic_out(float t, float c, float a, float p)
{
    if (t == 0) return 0;
    if (t == 1) return c;

    float s;
    if (a < c)
    {
        a = c;
        s = p / 4;
    }
    else
    {
        s = p / (math::two_pi.as<float>()) * std::asin(c / a);
    }

    return a * std::pow(2.0f, -10 * t) * std::sin((t - s) * (math::two_pi.as<float>()) / p) + c;
}

inline float bounce_out(float t, float c, float a)
{
    static const float coeff = 7.5625f;
    if (t == 1) return c;
    if (t < (4 / 11.0f))
    {
        return c * (coeff * t * t);
    }
    else if (t < (8 / 11.0f))
    {
        t -= (6 / 11.0f);
        return -a * (1 - (coeff * t * t + 0.75f)) + c;
    }
    else if (t < (10 / 11.0f))
    {
        t -= (9 / 11.0f);
        return -a * (1 - (coeff * t * t + 0.9375f)) + c;
    }
    else
    {
        t -= (21 / 22.0f);
        return -a * (1 - (coeff * t * t + 0.984375f)) + c;
    }
}

} /* namespace detail */

} /* namespace cpp_essentials::kine */
