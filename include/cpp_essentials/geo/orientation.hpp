#ifndef CPP_ESSENTIALS_GEO_ORIENTATION_HPP_
#define CPP_ESSENTIALS_GEO_ORIENTATION_HPP_

#pragma once

#include <cpp_essentials/geo/linear_shape.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct orientation_fn
{
    template <class T, class U>
    auto operator ()(const vector_2d<T>& point, const vector_2d<U>& start, const vector_2d<U>& end) const
    {
        return cross(end - start, point - start);
    }

    template <class T, class U, class Tag>
    auto operator ()(const vector_2d<T>& point, const linear_shape<U, 2, Tag>& shape) const
    {
        return (*this)(point, shape[0], shape[1]);
    }
};

} /* namespace detail */

static constexpr auto orientation = detail::orientation_fn{};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_ORIENTATION_HPP_ */
