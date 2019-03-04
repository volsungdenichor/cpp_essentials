#ifndef CPP_ESSENTIALS_GEO_CIRCLE_HPP_
#define CPP_ESSENTIALS_GEO_CIRCLE_HPP_

#pragma once

#include <cpp_essentials/geo/circular_shape.hpp>
#include <cpp_essentials/geo/triangle.hpp>
#include <cpp_essentials/geo/distance.hpp>
#include <cpp_essentials/geo/projection.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct incircle_fn
{
    template <class T>
    circle<T> operator ()(const triangle_2d<T>& triangle) const
    {
        static const T epsilon = T(0.1);

        auto center = incenter(triangle);
        auto radius = distance(center, *projection(center, get_segment(triangle, 0), epsilon));

        return circle<T> { center, radius };
    }
};

struct circumcircle_fn
{
    template <class T>
    circle<T> operator ()(const triangle_2d<T>& triangle) const
    {
        auto center = circumcenter(triangle);
        auto radius = distance(center, get_vertex(triangle, 0));

        return circle<T> { center, radius };
    }
};

} /* namespace detail */

static constexpr detail::incircle_fn incircle = {};
static constexpr detail::circumcircle_fn circumcircle = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_CIRCLE_HPP_ */
