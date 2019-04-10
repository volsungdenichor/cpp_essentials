#ifndef CPP_ESSENTIALS_GEO_INTERVAL_OPERATIONS_HPP_
#define CPP_ESSENTIALS_GEO_INTERVAL_OPERATIONS_HPP_

#pragma once

#include <cpp_essentials/geo/bounding_box.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct make_intersection_tag {};
struct make_union_tag {};

template <class Tag>
struct make_intersection_fn
{
    template <class T>
    interval<T> operator ()(const interval<T>& lhs, const interval<T>& rhs) const
    {
        static constexpr Tag tag = {};
        return apply(lhs, rhs, tag);
    }

    template <class T, size_t D>
    bounding_box<T, D> operator ()(const bounding_box<T, D>& lhs, const bounding_box<T, D>& rhs) const
    {
        bounding_box<T, D> result;
        for (size_t d = 0; d < D; ++d)
        {
            result._data[d] = (*this)(lhs._data[d], rhs._data[d]);
        }
        return result;
    }

private:
    template <class T>
    interval<T> apply(const interval<T>& lhs, const interval<T>& rhs, make_intersection_tag) const
    {
        if (lhs.empty() || rhs.empty())
        {
            return {};
        }

        return { std::max(lhs.lower(), rhs.lower()), std::min(lhs.upper(), rhs.upper()) };
    }

    template <class T>
    interval<T> apply(const interval<T>& lhs, const interval<T>& rhs, make_union_tag) const
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
};

} /* namespace detail */

static constexpr detail::make_intersection_fn<detail::make_intersection_tag> make_intersection = { };
static constexpr detail::make_intersection_fn<detail::make_union_tag> make_union = { };

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_INTERVAL_OPERATIONS_HPP_ */