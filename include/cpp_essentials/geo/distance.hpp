#ifndef CPP_ESSENTIALS_GEO_DISTANCE_HPP_
#define CPP_ESSENTIALS_GEO_DISTANCE_HPP_

#pragma once

#include <cpp_essentials/geo/projection.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct distance_fn
{
    template <class T, class U, size_t D >
    auto operator ()(const vector<T, D>& lhs, const vector<U, D>& rhs) const
    {
        return math::distance(lhs, rhs);
    }

    template <class T, size_t D>
    auto operator ()(const vector<T, D>& point, const line<T, D>& shape) const
    {
        return (*this)(point, projection(point, shape).value_or(point));
    }
};

} /* namespace detail */

static constexpr detail::distance_fn distance = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DISTANCE_HPP_ 
*/