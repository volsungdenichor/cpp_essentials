#ifndef CPP_ESSENTIALS_GEO_INTERVAL_OPERATIONS_HPP_
#define CPP_ESSENTIALS_GEO_INTERVAL_OPERATIONS_HPP_

#pragma once

#include <cpp_essentials/geo/bounding_box.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct make_union_fn
{
    template <class T>
    auto operator ()(const interval<T>& lhs, const interval<T>& rhs) const -> interval<T>
    {
        if (lhs.empty())
        {
            return rhs;
        }

        if (rhs.empty())
        {
            return lhs;
        }

        return { std::min(lhs.lower(), rhs.lower()), std::max(lhs.upper(), rhs.upper()) };
    }

    template <class T, size_t D>
    auto operator ()(const bounding_box<T, D>& lhs, const bounding_box<T, D>& rhs) const -> bounding_box<T, D>
    {
        bounding_box<T, D> result;

        for (size_t d = 0; d < D; ++d)
        {
            result._data[d] = (*this)(lhs._data[d], rhs._data[d]);
        }

        return result;
    }
};

struct make_intersection_fn
{
    template <class T>
    auto operator ()(const interval<T>& lhs, const interval<T>& rhs) const -> interval<T>
    {
        if (lhs.empty() || rhs.empty())
        {
            return {};
        }

        return { std::max(lhs.lower(), rhs.lower()), std::min(lhs.upper(), rhs.upper()) };
    }

    template <class T, size_t D>
    auto operator ()(const bounding_box<T, D>& lhs, const bounding_box<T, D>& rhs) const -> bounding_box<T, D>
    {
        bounding_box<T, D> result;

        for (size_t d = 0; d < D; ++d)
        {
            result._data[d] = (*this)(lhs._data[d], rhs._data[d]);
        }

        return result;
    }
};

} /* namespace detail */

static constexpr detail::make_union_fn make_union = {};
static constexpr detail::make_intersection_fn make_intersection = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_INTERVAL_OPERATIONS_HPP_ */