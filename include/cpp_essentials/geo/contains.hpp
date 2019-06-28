#ifndef CPP_ESSENTIALS_GEO_CONTAINS_HPP_
#define CPP_ESSENTIALS_GEO_CONTAINS_HPP_

#pragma once

#include <cpp_essentials/geo/bounding_box.hpp>
#include <cpp_essentials/geo/circle.hpp>
#include <cpp_essentials/geo/vertex_array.hpp>
#include <cpp_essentials/geo/orientation.hpp>
#include <cpp_essentials/geo/vertex_container.hpp>
#include <cpp_essentials/core/zip.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct contains_fn
{
    template <class T, class U>
    bool operator ()(const interval<T>& item, U value) const
    {
        return core::between(value, item.lower(), item.upper());
    }

    template <class T, class U>
    bool operator ()(const interval<T>& item, const interval<U>& other) const
    {
        return core::all_of(other._data, core::inclusive_between(item.lower(), item.upper()));
    }

    template <class T, class U, size_t D>
    bool operator ()(const bounding_box<T, D>& item, const vector<U, D>& point) const
    {
        return core::all_of(
            core::views::zip(
                item._data,
                point._data,
                [this](const auto& this_interval, const auto& p) { return (*this)(this_interval, p); }),
            core::equal_to(true));
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

    template <class T, class U, size_t D>
    bool operator ()(const circular_shape<T, D>& item, const vector<U, D>& other) const
    {
        return geo::norm(other - item.center()) <= math::sqr(item.radius());
    }

    template <class T, class U>
    bool operator ()(const triangle<T, 2>& item, const vector<U, 2>& other) const
    {
        static const auto same_sign = [](int a, int b)
        {
            return (a <= 0 && b <= 0) || (a >= 0 && b >= 0);
        };
        
        int result[3];

        for (size_t i = 0; i < 3; ++i)
        {
            result[i] = math::sgn(geo::orientation(other, get_segment(item, i)));
        }

        return same_sign(result[0], result[1])
            && same_sign(result[0], result[2])
            && same_sign(result[1], result[2]);
    }    
};

} /* namespace detail */

static constexpr detail::contains_fn contains = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_CONTAINS_HPP_ */