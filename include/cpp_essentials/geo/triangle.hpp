#ifndef CPP_ESSENTIALS_GEO_TRIANGLE_HPP_
#define CPP_ESSENTIALS_GEO_TRIANGLE_HPP_

#include <cpp_essentials/geo/vertex_array.hpp>
#include <cpp_essentials/geo/vertex_container.hpp>
#include <cpp_essentials/geo/length.hpp>
#include <cpp_essentials/geo/center.hpp>
#include <cpp_essentials/geo/perpendicular.hpp>

#pragma once

namespace cpp_essentials::geo
{

namespace detail
{

struct altitude_fn
{
    template <typename T>
    auto operator ()(const triangle_2d<T>& value, size_t index) const -> segment_2d<T>
    {
        static const T epsilon = T(0.1);

        auto v = value[(index + 0) % 3];

        auto p = projection(v, make_line(value[(index + 1) % 3], value[(index + 2) % 3]), epsilon);

        return{ v, *p };
    }
};

struct centroid_fn
{
    template <typename T>
    auto operator ()(const triangle_2d<T>& value) const -> vector_2d<T>
    {
        return core::accumulate(value._data, vector_2d<T>{}) / 3;
    }
};

struct orthocenter_fn
{
    template <typename T>
    auto operator ()(const triangle_2d<T>& value) const -> vector_2d<T>
    {
        static const T epsilon = T(0.0001);
        static constexpr altitude_fn _altitude = {};

        return *intersection(make_line(_altitude(value, 0)), make_line(_altitude(value, 1)), epsilon);
    }
};

struct circumcenter_fn
{
    template <typename T>
    auto operator ()(const triangle_2d<T>& value) const -> vector_2d<T>
    {
        static T epsilon = T(0.0001);

        auto s0 = get_segment(value, 0);
        auto s1 = get_segment(value, 1);

        return *intersection(make_line(perpendicular(s0, center(s0))), make_line(perpendicular(s1, center(s1))), epsilon);
    }
};

struct incenter_fn
{
    template <typename T>
    auto operator ()(const triangle_2d<T>& value) const -> vector_2d<T>
    {
        T sum = T(0);

        vector_2d<T> result;

        for (size_t i = 0; i < 3; ++i)
        {
            auto side_length = length(get_segment(value, i));

            result += side_length * value[(i + 2) % 3];

            sum += side_length;
        }

        return result / sum;
    }
};

} /* namespace detail */

static constexpr detail::altitude_fn altitude = {};
static constexpr detail::centroid_fn centroid = {};
static constexpr detail::orthocenter_fn orthocenter = {};
static constexpr detail::circumcenter_fn circumcenter = {};
static constexpr detail::incenter_fn incenter = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_TRIANGLE_HPP_ */