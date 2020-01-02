#ifndef CPP_ESSENTIALS_GEO_INTERSECTION_HPP_
#define CPP_ESSENTIALS_GEO_INTERSECTION_HPP_

#pragma once

#include <cpp_essentials/geo/interpolate.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct intersection_fn
{
    template <class T, size_t D, class Tag1, class Tag2, class E = T>
    auto operator ()(
        const linear_shape<T, D, Tag1>& lhs,
        const linear_shape<T, D, Tag2>& rhs,
        E epsilon = {}) const -> core::optional<vector<T, D>>
    {
        using lhs_traits = linear_shape_traits<linear_shape<T, D, Tag1>>;
        using rhs_traits = linear_shape_traits<linear_shape<T, D, Tag2>>;

        auto par = get_line_intersection_parameters(lhs[0], lhs[1], rhs[0], rhs[1], epsilon);

        return par && lhs_traits::contains_parameter(std::get<0>(*par)) && rhs_traits::contains_parameter(std::get<1>(*par))
            ? interpolate(lhs[0], lhs[1], std::get<0>(*par))
            : core::optional<vector<T, D>>{ core::none };
    }
};

} /* namespace detail */

static constexpr auto intersection = detail::intersection_fn{};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_INTERSECTION_HPP_ */
