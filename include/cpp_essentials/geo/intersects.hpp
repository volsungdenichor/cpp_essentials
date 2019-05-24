#ifndef CPP_ESSENTIALS_GEO_INTERSECTS_HPP_
#define CPP_ESSENTIALS_GEO_INTERSECTS_HPP_

#pragma once

#include <cpp_essentials/geo/bounding_box.hpp>
#include <cpp_essentials/geo/linear_shape.hpp>
#include <cpp_essentials/geo/detail/linear_shape.traits.hpp>
#include <cpp_essentials/core/zip.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct intersects_fn
{
    template <class T, class U>
    bool operator ()(const interval<T>& item, const interval<U>& other) const
    {
        return core::any_of(other._data, core::inclusive_between(item.lower(), item.upper()))
            || core::any_of(item._data, core::inclusive_between(other.lower(), other.upper()));
    }

    template <class T, size_t D>
    bool operator ()(const bounding_box<T, D>& item, const bounding_box<T, D>& other) const
    {
        return core::all_of(
            core::views::zip(
                item._data,
                other._data,
                [this](const auto& this_interval, const auto& other_interval) { return (*this)(this_interval, other_interval); }),
            core::equal_to(true));
    }

    template <class T, size_t D, class Tag1, class Tag2, class E = T>
    bool operator ()(
        const linear_shape<T, D, Tag1>& item,
        const linear_shape<T, D, Tag2>& other,
        E epsilon = {}) const
    {
        using lhs_traits = linear_shape_traits<linear_shape<T, D, Tag1>>;
        using rhs_traits = linear_shape_traits<linear_shape<T, D, Tag2>>;

        const auto par = get_line_intersection_parameters(item[0], item[1], other[0], other[1], epsilon);

        return par
            && lhs_traits::contains_parameter(std::get<0>(*par))
            && rhs_traits::contains_parameter(std::get<1>(*par));
    }
};

} /* namespace detail */

static constexpr detail::intersects_fn intersects = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_INTERSECTS_HPP_ */
