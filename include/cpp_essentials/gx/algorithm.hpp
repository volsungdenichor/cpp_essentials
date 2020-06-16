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
    const core::predicate<point>& output)
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
        const point& start,
        const point& end,
        const core::action<point>& output) const
    {       
        const auto direction = end - start;

        vector dist;
        vector dir;

        core::transform(direction._data, dist._data.begin(), math::abs);
        core::transform(direction._data, dir._data.begin(), math::sgn);

        bresenham(
            start,
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

struct bresenham_circle_fn
{
    void operator ()(
        const point& center,
        int radius,
        const core::action<point>& output) const
    {
        int x = radius;
        int y = 0;
        int err = 0;

        int x0 = center.x();
        int y0 = center.y();

        while (x >= y)
        {
            output(geo::make_vector(x0 + x, y0 + y));
            output(geo::make_vector(x0 + y, y0 + x));
            output(geo::make_vector(x0 - y, y0 + x));
            output(geo::make_vector(x0 - x, y0 + y));
            output(geo::make_vector(x0 - x, y0 - y));
            output(geo::make_vector(x0 - y, y0 - x));
            output(geo::make_vector(x0 + y, y0 - x));
            output(geo::make_vector(x0 + x, y0 - y));

            if (err <= 0)
            {
                y += 1;
                err += 2 * y + 1;
            }

            if (err > 0)
            {
                x -= 1;
                err -= 2 * x + 1;
            }
        }
    }
};

struct xiaolin_wu_line_fn
{
    void operator ()(
        const point& start,
        const point& end,
        const core::action<point, float>& output) const
    {
        auto plot = [&](auto x, auto y, auto c)
        {
            output(geo::make_vector(x, y), static_cast<float>(c));
        };

        auto fpart = [&](auto x) { return x - math::floor(x); };

        auto x0 = start.x();
        auto y0 = start.y();
        auto x1 = end.x();
        auto y1 = end.y();
        auto steep = math::abs(y1 - y0) > math::abs(x1 - x0);

        if (steep)
        {
            std::swap(x0, y0);
            std::swap(x1, y1);
        }

        if (x0 > x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        auto dx = x1 - x0;
        auto dy = y1 - y0;
        auto gradient = static_cast<float>(dy) / dx;
        if (dx == 0)
        {
            gradient = 1.F;
        }

        auto xend = math::round(x0);
        auto yend = y0 + gradient * (xend - x0);
        auto xgap = 1.F - math::round(x0);
        auto xpxl1 = xend;
        auto ypxl1 = math::floor(yend);
        if (steep)
        {
            plot(ypxl1 + 0, xpxl1, (1.F - fpart(yend)) * xgap);
            plot(ypxl1 + 1, xpxl1, (0.F + fpart(yend)) * xgap);
        }
        else
        {
            plot(xpxl1, ypxl1 + 0, (1.F - fpart(yend)) * xgap);
            plot(xpxl1, ypxl1 + 1, (0.F + fpart(yend)) * xgap);
        }

        auto intery = yend + gradient;

        xend = math::round(x1);
        yend = y1 + gradient * (xend - x1);
        xgap = fpart(x1 + 0.5F);

        auto xpxl2 = xend;
        auto ypxl2 = math::floor(yend);

        if (steep)
        {
            plot(ypxl2 + 0, xpxl2, (1.F - fpart(yend)) * xgap);
            plot(ypxl2 + 1, xpxl2, (0.F + fpart(yend)) * xgap);

            for (auto x = xpxl1 + 1; x < xpxl2; ++x, intery += gradient)
            {
                auto alpha = fpart(intery);
                plot(math::floor(intery) + 0, x, (1.F - alpha));
                plot(math::floor(intery) + 1, x, (0.F + alpha));
            }
        }
        else
        {
            plot(xpxl2, ypxl2 + 0, (1.F - fpart(yend)) * xgap);
            plot(xpxl2, ypxl2 + 1, (0.F + fpart(yend)) * xgap);

            for (auto x = xpxl1 + 1; x < xpxl2; ++x, intery += gradient)
            {
                auto alpha = fpart(intery);
                plot(x, math::floor(intery) + 0, (1.F - alpha));
                plot(x, math::floor(intery) + 1, (0.F + alpha));
            }
        }
    }
};

} /* namespace detail */

static constexpr auto bresenham_line = detail::bresenham_line_fn{};
static constexpr auto bresenham_circle = detail::bresenham_circle_fn{};
static constexpr auto xiaolin_wu_line = detail::xiaolin_wu_line_fn{};

} /* namespace cpp_essentials::gx */

#endif /* CPP_ESSENTIALS_GX_ALGORITHM_HPP__ */
