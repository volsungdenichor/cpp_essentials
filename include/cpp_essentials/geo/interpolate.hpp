#ifndef CPP_ESSENTIALS_GEO_INTERPOLATE_HPP_
#define CPP_ESSENTIALS_GEO_INTERPOLATE_HPP_

#pragma once

#include <cpp_essentials/geo/linear_shape.hpp>
#include <cpp_essentials/geo/bounding_box.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct interpolate_fn
{
    template <class T, class U, size_t D, class R>
    auto operator ()(const vector<T, D>& lhs, const vector<U, D>& rhs, R ratio) const
    {
        return core::lerp(ratio, lhs, rhs);
    }

    template <class T, size_t D, class R>
    auto operator ()(const segment<T, D>& value, R ratio) const
    {
        return (*this)(value[0], value[1], ratio);
    }

    template <class T, class R>
    auto operator ()(const interval<T>& item, R ratio) const
    {
        return core::lerp(ratio, item.lower(), item.upper());
    }
};

} /* namespace detail */

static constexpr detail::interpolate_fn interpolate = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_INTERPOLATE_HPP_ */
