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

struct length_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& item) const
    {
        return math::length(item);
    }

    template <class T, size_t D>
    auto operator ()(const segment<T, D>& item) const
    {
        return math::distance(item[0], item[1]);
    }
};

} /* namespace detail */

static constexpr auto length = detail::length_fn{};
static constexpr auto distance = detail::distance_fn{};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DISTANCE_HPP_ */

