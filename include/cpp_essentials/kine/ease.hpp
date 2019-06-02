#pragma once

#include <cpp_essentials/kine/ease.detail.hpp>

namespace cpp_essentials::kine
{

using ease_function = std::function<float(float)>;

namespace ease
{

enum class type
{
    linear,
    quad,
    cubic,
    quart,
    quint,
    sine,
    expo,
    circ,
    back,
    elastic,
    bounce,
};

enum class direction
{
    none,
    in,
    out,
    in_out,
    out_in,
};

template <type Type, direction C>
struct ease_fn_t
{
};


template <direction C>
struct ease_fn_t<type::linear, C>
{
    float operator ()(float t) const
    {
        return t;
    }
};

template <type Type>
struct ease_fn_t<Type, direction::out_in>
{
    float operator ()(float t) const
    {
        static const ease_fn_t<Type, direction::out> out;
        static const ease_fn_t<Type, direction::in> in;

        return t < 0.5F
            ? out(2.F * t) / 2.F
            : in(2.F * t - 1.F) / 2.F + 0.5F;
    }
};


template <>
struct ease_fn_t<type::quad, direction::in>
{
    float operator ()(float t) const
    {
        return math::pow(t, 2);
    }
};

template <>
struct ease_fn_t<type::quad, direction::out>
{
    float operator ()(float t) const
    {
        return -t * (t - 2.F);
    }
};

template <>
struct ease_fn_t<type::quad, direction::in_out>
{
    float operator ()(float t) const
    {
        t *= 2;

        if (t < 1)
        {
            return 0.5F * math::pow(t, 2);
        }

        t -= 1;
        return -0.5F * (t * (t - 2.F) - 1.F);
    }
};


template <>
struct ease_fn_t<type::cubic, direction::in>
{
    float operator ()(float t) const
    {
        return math::pow(t, 3);
    }
};

template <>
struct ease_fn_t<type::cubic, direction::out>
{
    float operator ()(float t) const
    {
        return math::pow(t - 1, 3) + 1;
    }
};

template <>
struct ease_fn_t<type::cubic, direction::in_out>
{
    float operator ()(float t) const
    {
        t *= 2;
        return t < 1
            ? 0.5F * math::pow(t, 3)
            : 0.5F * (math::pow(t - 2, 3) + 2);
    }
};


template <>
struct ease_fn_t<type::quart, direction::in>
{
    float operator ()(float t) const
    {
        return math::pow(t, 4);
    }
};

template <>
struct ease_fn_t<type::quart, direction::out>
{
    float operator ()(float t) const
    {
        return -(math::pow(t - 1, 4) - 1);
    }
};

template <>
struct ease_fn_t<type::quart, direction::in_out>
{
    float operator ()(float t) const
    {
        t *= 2;

        return t < 1
            ? 0.5F * math::pow(t, 4)
            : -0.5F * (math::pow(t - 2, 4) - 2);
    }
};


template <>
struct ease_fn_t<type::quint, direction::in>
{
    float operator ()(float t) const
    {
        return math::pow(t, 5);
    }
};

template <>
struct ease_fn_t<type::quint, direction::out>
{
    float operator ()(float t) const
    {
        return math::pow(t - 1, 5) + 1;
    }
};

template <>
struct ease_fn_t<type::quint, direction::in_out>
{
    float operator ()(float t) const
    {
        t *= 2;

        return t < 1
            ? 0.5F * math::pow(t, 5)
            : 0.5F * (math::pow(t - 2, 5) + 2);
    }
};


template <>
struct ease_fn_t<type::sine, direction::in>
{
    float operator ()(float t) const
    {
        return -std::cos(t * math::half_pi.as<float>()) + 1.F;
    }
};

template <>
struct ease_fn_t<type::sine, direction::out>
{
    float operator ()(float t) const
    {
        return std::sin(t * math::half_pi.as<float>());
    }
};

template <>
struct ease_fn_t<type::sine, direction::in_out>
{
    float operator ()(float t) const
    {
        return -0.5F * (std::cos(math::pi.as<float>() * t) - 1.F);
    }
};


template <>
struct ease_fn_t<type::expo, direction::in>
{
    float operator ()(float t) const
    {
        return t == 0.F
            ? 0.0F
            : math::pow(2.0F, 10 * (t - 1.F));
    }
};

template <>
struct ease_fn_t<type::expo, direction::out>
{
    float operator ()(float t) const
    {
        return t == 1.F
            ? 1.F
            : -math::pow(2.0F, -10 * t) + 1.F;
    }
};

template <>
struct ease_fn_t<type::expo, direction::in_out>
{
    float operator ()(float t) const
    {
        if (t == 0.F) return 0.F;
        if (t == 1.F) return 1.F;

        t *= 2.F;

        return t < 1.F
            ? 0.5F * math::pow(2.0F, 10 * (t - 1.F))
            : 0.5F * (-math::pow(2.0F, -10 * (t - 1.F)) + 2);
    }
};


template <>
struct ease_fn_t<type::circ, direction::in>
{
    float operator ()(float t) const
    {
        return -(math::sqrt(1.F - math::pow(t, 2)) - 1.F);
    }
};

template <>
struct ease_fn_t<type::circ, direction::out>
{
    float operator ()(float t) const
    {
        return math::sqrt(1.F - math::pow(t - 1, 2));
    }
};

template <>
struct ease_fn_t<type::circ, direction::in_out>
{
    float operator ()(float t) const
    {
        t *= 2.F;

        return t < 1.F
            ? -0.5F * (math::sqrt(1.F - math::pow(t, 2)) - 1.F)
            : 0.5F * (math::sqrt(1.F - math::pow(t - 2, 2)) + 1.F);
    }
};


template <>
struct ease_fn_t<type::back, direction::in>
{
    float operator ()(float t) const
    {
        return math::pow(t, 2) * ((s + 1) * t - s);
    }

    float s;
};

template <>
struct ease_fn_t<type::back, direction::out>
{
    float operator ()(float t) const
    {
        t -= 1;
        return math::pow(t, 2) * ((s + 1) * t + s) + 1;
    }

    float s;
};

template <>
struct ease_fn_t<type::back, direction::in_out>
{
    float operator ()(float t) const
    {
        auto c = s * 1.525f;

        t *= 2;

        if (t < 1)
        {
            return 0.5F * (t * t * ((c + 1) * t - c));
        }
        else
        {
            t -= 2;
            return 0.5F * (t * t * ((c + 1) * t + c) + 2);
        }
    }

    float s;
};

template <>
struct ease_fn_t<type::back, direction::out_in>
{
    float operator ()(float t) const
    {
        const ease_fn_t<type::back, direction::out> out{ s };
        const ease_fn_t<type::back, direction::in> in{ s };

        return t < 0.5F
            ? out(2.F * t) / 2.F
            : in(2.F * t - 1.F) / 2.F + 0.5F;
    }

    float s;
};




template <>
struct ease_fn_t<type::elastic, direction::in>
{
    float operator ()(float t) const
    {
        return detail::elastic_in(t, 0, 1, 1, a, p);
    }

    float a;
    float p;
};

template <>
struct ease_fn_t<type::elastic, direction::out>
{
    float operator ()(float t) const
    {
        return detail::elastic_out(t, 1, a, p);
    }

    float a;
    float p;
};

template <>
struct ease_fn_t<type::elastic, direction::in_out>
{
    float operator ()(float t) const
    {
        auto amplitude = a;
        if (t == 0) return 0;
        t *= 2;
        if (t == 2) return 1;

        float s;
        if (amplitude < 1)
        {
            amplitude = 1;
            s = p / 4;
        }
        else
        {
            s = p / (math::two_pi.as<float>()) * std::asin(1 / amplitude);
        }

        if (t < 1)
        {
            return -0.5F * (amplitude * math::pow(2.0f, 10 * (t - 1)) * std::sin((t - 1 - s) * math::two_pi.as<float>() / p));
        }

        return amplitude * math::pow(2.0f, -10 * (t - 1)) * std::sin((t - 1 - s) * math::two_pi.as<float>() / p) * 0.5F + 1;
    }

    float a;
    float p;
};

template <>
struct ease_fn_t<type::elastic, direction::out_in>
{
    float operator ()(float t) const
    {
        const ease_fn_t<type::elastic, direction::out> out{ a, p };
        const ease_fn_t<type::elastic, direction::in> in{ a, p };

        return t < 0.5F
            ? out(2.F * t) / 2.F
            : in(2.F * t - 1.F) / 2.F + 0.5F;
    }

    float a;
    float p;
};


template <>
struct ease_fn_t<type::bounce, direction::in>
{
    float operator ()(float t) const
    {
        return 1 - detail::bounce_out(1 - t, 1, a);
    }

    float a;
};

template <>
struct ease_fn_t<type::bounce, direction::out>
{
    float operator ()(float t) const
    {
        return detail::bounce_out(t, 1, a);
    }

    float a;
};

template <>
struct ease_fn_t<type::bounce, direction::in_out>
{
    float operator ()(float t) const
    {
        const ease_fn_t<type::bounce, direction::out> out{ a };
        const ease_fn_t<type::bounce, direction::in> in{ a };

        if (t < 0.5F)
        {
            return in(2 * t) / 2;
        }
        else
        {
            return t == 1
                ? 1
                : out(2 * t - 1) / 2 + 0.5F;
        }
    }

    float a;
};

template <>
struct ease_fn_t<type::bounce, direction::out_in>
{
    float operator ()(float t) const
    {
        const ease_fn_t<type::bounce, direction::out> out{ a };
        const ease_fn_t<type::bounce, direction::in> in{ a };

        return t < 0.5F
            ? out(2.F * t) / 2.F
            : in(2.F * t - 1.F) / 2.F + 0.5F;
    }

    float a;
};


static const ease_function none = ease_fn_t<type::linear, direction::none>{};

static const ease_function quad_in = ease_fn_t<type::quad, direction::in>{};
static const ease_function quad_out = ease_fn_t<type::quad, direction::out>{};
static const ease_function quad_in_out = ease_fn_t<type::quad, direction::in_out>{};
static const ease_function quad_out_in = ease_fn_t<type::quad, direction::out_in>{};

static const ease_function cubic_in = ease_fn_t<type::cubic, direction::in>{};
static const ease_function cubic_out = ease_fn_t<type::cubic, direction::out>{};
static const ease_function cubic_in_out = ease_fn_t<type::cubic, direction::in_out>{};
static const ease_function cubic_out_in = ease_fn_t<type::cubic, direction::out_in>{};

static const ease_function quart_in = ease_fn_t<type::quart, direction::in>{};
static const ease_function quart_out = ease_fn_t<type::quart, direction::out>{};
static const ease_function quart_in_out = ease_fn_t<type::quart, direction::in_out>{};
static const ease_function quart_out_in = ease_fn_t<type::quart, direction::out_in>{};

static const ease_function quint_in = ease_fn_t<type::quint, direction::in>{};
static const ease_function quint_out = ease_fn_t<type::quint, direction::out>{};
static const ease_function quint_in_out = ease_fn_t<type::quint, direction::in_out>{};
static const ease_function quint_out_in = ease_fn_t<type::quint, direction::out_in>{};

static const ease_function sine_in = ease_fn_t<type::sine, direction::in>{};
static const ease_function sine_out = ease_fn_t<type::sine, direction::out>{};
static const ease_function sine_in_out = ease_fn_t<type::sine, direction::in_out>{};
static const ease_function sine_out_in = ease_fn_t<type::sine, direction::out_in>{};

static const ease_function expo_in = ease_fn_t<type::expo, direction::in>{};
static const ease_function expo_out = ease_fn_t<type::expo, direction::out>{};
static const ease_function expo_in_out = ease_fn_t<type::expo, direction::in_out>{};
static const ease_function expo_out_in = ease_fn_t<type::expo, direction::out_in>{};

static const ease_function circ_in = ease_fn_t<type::circ, direction::in>{};
static const ease_function circ_out = ease_fn_t<type::circ, direction::out>{};
static const ease_function circ_in_out = ease_fn_t<type::circ, direction::in_out>{};
static const ease_function circ_out_in = ease_fn_t<type::circ, direction::out_in>{};

constexpr float default_overshoot = 1.70158F;


inline ease_function back_in(float s = default_overshoot)
{
    return ease_fn_t<type::back, direction::in>{ s };
}

inline ease_function back_out(float s = default_overshoot)
{
    return ease_fn_t<type::back, direction::out>{ s };
}

inline ease_function back_in_out(float s = default_overshoot)
{
    return ease_fn_t<type::back, direction::in_out>{ s };
}

inline ease_function back_out_in(float s = default_overshoot)
{
    return ease_fn_t<type::back, direction::out_in>{ s };
}


inline ease_function elastic_in(float a, float p)
{
    return ease_fn_t<type::elastic, direction::in>{ a, p };
}

inline ease_function elastic_out(float a, float p)
{
    return ease_fn_t<type::elastic, direction::out>{ a, p };
}

inline ease_function elastic_in_out(float a, float p)
{
    return ease_fn_t<type::elastic, direction::in_out>{ a, p };
}

inline ease_function elastic_out_in(float a, float p)
{
    return ease_fn_t<type::elastic, direction::out_in>{ a, p };
}


inline ease_function bounce_in(float a = default_overshoot)
{
    return ease_fn_t<type::bounce, direction::in>{ a };
}

inline ease_function bounce_out(float a = default_overshoot)
{
    return ease_fn_t<type::bounce, direction::out>{ a };
}

auto bounce_in_out(float a = default_overshoot)
{
    return ease_fn_t<type::bounce, direction::in_out>{ a };
}

inline ease_function bounce_out_in(float a = default_overshoot)
{
    return ease_fn_t<type::bounce, direction::out_in>{ a };
}

} /* namespace ease */

} /* namespace cpp_essentials::kine */
