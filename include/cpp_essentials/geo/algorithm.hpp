#ifndef CPP_ESSENTIALS_GEO_ALGORITHM_HPP_
#define CPP_ESSENTIALS_GEO_ALGORITHM_HPP_

#pragma once

#include <cpp_essentials/core/function_defs.hpp>
#include <cpp_essentials/geo/vertex_array.hpp>
#include <cpp_essentials/geo/vertex_container.hpp>
#include <cpp_essentials/geo/orientation.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct douglas_peucker_fn
{
    template <class T, class E>
    void operator ()(const polyline_2d<T>& polyline, E tolerance, const core::action<vector_2d<T>>& output) const
    {
        impl(core::make_range(polyline._data), tolerance, output);
    }

    template <class T, class E>
    polyline_2d<T> operator ()(const polyline_2d<T>& polyline, E tolerance) const
    {
        std::vector<vector_2d<T>> result;
        impl(core::make_range(polyline._data), tolerance, [&](const vector_2d<T>& item) { result.push_back(item); });
        core::erase(result, core::unique(result));
        return polyline_2d<T>{ std::move(result) };
    }

private:
    template <class Iter, class E, class Func>
    static void impl(core::iterator_range<Iter> range, E tolerance, Func&& output)
    {
        if (range.empty())
        {
            return;
        }

        const auto line = make_line(range.front(), range.back());

        const auto distance_to_line_squared = [&](const auto& p)
        {
            return norm(p - projection(p, line).value_or(p));
        };

        const auto max = sq::max_element(range, [&](const auto& lt, const auto& rt)
        {
            return distance_to_line_squared(lt) < distance_to_line_squared(rt);
        });

        if (!max.empty())
        {
            const auto b = max.begin();
            const auto dist_squared = distance_to_line_squared(*b);
            if (dist_squared > math::sqr(tolerance))
            {
                impl(range | sq::split_before(std::next(b)), tolerance, output);
                impl(range | sq::split_after(b), tolerance, output);
            }
            else
            {
                output(line[0]);
                output(line[1]);
            }
        }
    }
};

struct sutherland_hodgman_fn
{
    template <class T>
    void operator()(const polygon_2d<T>& polygon, const polygon_2d<T>& clip_polygon, const core::action<vector_2d<T>>& output) const
    {
        auto result = impl(polygon, clip_polygon);
        result | sq::for_each(output);
    }

    template <class T>
    polygon_2d<T> operator ()(const polygon_2d<T>& polygon, const polygon_2d<T>& clip_polygon) const
    {
        return impl(polygon, clip_polygon);
    }

private:
    template <class T>
    static polygon_2d<T> impl(const polygon_2d<T>& polygon, const polygon_2d<T>& clip_polygon)
    {
        EXPECTS(polygon.size() >= 3 && clip_polygon.size() >= 3);

        auto result = polygon;

        for (const auto clip_halfplane : get_segments(clip_polygon) | sq::map(make_line))
        {
            const auto input = result;
            result._data.clear();

            for (const auto subject_edge : get_segments(input))
            {
                const auto start_inside = orientation(subject_edge[0], clip_halfplane) >= 0;
                const auto end_inside = orientation(subject_edge[1], clip_halfplane) >= 0;

                if (start_inside && end_inside)
                {
                    result._data.push_back(subject_edge[1]);
                }
                else if (start_inside && !end_inside)
                {
                    result._data.push_back(*intersection(subject_edge, clip_halfplane));
                }
                else if (!start_inside && end_inside)
                {
                    result._data.push_back(*intersection(subject_edge, clip_halfplane));
                    result._data.push_back(subject_edge[1]);
                }
            }
        }


        return { std::move(result) };
    }

};

} /* namespace detail */

static constexpr auto douglas_peucker = detail::douglas_peucker_fn{};
static constexpr auto sutherland_hodgman = detail::sutherland_hodgman_fn{};

} /* cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_ALGORITHM_HPP_ */
