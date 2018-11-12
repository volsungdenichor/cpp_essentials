#ifndef CPP_ESSENTIALS_GX_ALGORITHM_HPP_
#define CPP_ESSENTIALS_GX_ALGORITHM_HPP_

#pragma once

#include <cpp_essentials/core/function_defs.hpp>
#include <cpp_essentials/geo/linear_shape.hpp>

#include <cpp_essentials/gx/defs.hpp>

namespace cpp_essentials::gx
{

namespace detail
{

inline void bresenham(
    point cur,
    const vector& dir,
    const vector& dist,
    int err,
    core::predicate<point> output)
{
    while (true)
    {
        if (!output(cur))
        {
            break;
        }

        auto e = err;

        if (e > -dist[0])
        {
            err -= dist[1];
            cur[0] += dir[0];
        }

        if (e < dist[1])
        {
            err += dist[0];
            cur[1] += dir[1];
        }
    }
}

struct bresenham_line_fn
{

    void operator ()(
        const segment& line,
        core::action<point> output) const
    {
        const auto& [start, end] = line._data;
        const auto direction = end - start;

        vector dist;
        vector dir;

        core::transform(direction._data, dist._data.begin(), math::abs);
        core::transform(direction._data, dir._data.begin(), math::sgn);

        bresenham(
            line[0],
            dir,
            dist,
            (dist[0] > dist[1] ? dist[0] : -dist[1]) / 2,
            [&](const auto& loc)
            {
                output(loc);
                return loc != end;
            });
    }
};

} /* namespace detail */

static constexpr detail::bresenham_line_fn bresenham_line = {};

} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_ALGORITHM_HPP__ */
