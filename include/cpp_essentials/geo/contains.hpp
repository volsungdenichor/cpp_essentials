#ifndef CPP_ESSENTIALS_GEO_CONTAINS_HPP_
#define CPP_ESSENTIALS_GEO_CONTAINS_HPP_

#pragma once

#include <cpp_essentials/geo/bounding_box.hpp>
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
            core::zip(
                item._data,
                point._data,
                [this](const auto& this_interval, const auto& p) { return (*this)(this_interval, p); }),
            core::equal_to(true));
    }

    template <class T, size_t D>
    bool operator ()(const bounding_box<T, D>& item, const bounding_box<T, D>& other) const
    {
        return core::all_of(
            core::zip(
                item._data,
                other._data,
                [this](const auto& this_interval, const auto& other_interval) { return (*this)(this_interval, other_interval); }),
            core::equal_to(true));
    }
};

} /* namespace detail */

static constexpr detail::contains_fn contains = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_CONTAINS_HPP_ */