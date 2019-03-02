#ifndef CPP_ESSENTIALS_GEO_PROJECTION_HPP_
#define CPP_ESSENTIALS_GEO_PROJECTION_HPP_

#pragma once

#include <cpp_essentials/geo/linear_shape.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct projection_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<T, D>& rhs) const
    {
        return math::projection(lhs, rhs);
    }

    template <class T, size_t D, class Tag, class E = T>
    auto operator ()(
        const vector<T, D>& point,
        const linear_shape<T, D, Tag>& shape,
        E epsilon = {}) const -> core::optional<vector<T, D>>
    {
        using traits = linear_shape_traits<linear_shape<T, D, Tag>>;

        auto p0 = shape[0];
        auto p1 = shape[1];

        auto result = p0 + (*this)(point - p0, p1 - p0);

        auto t = get_line_intersection_parameter(p0, p1, result, epsilon);

        return core::make_optional(t && traits::contains_parameter(*t), result);
    }
};

} /* namespace detail */

static constexpr detail::projection_fn projection = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_PROJECTION_HPP_ */