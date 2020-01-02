#ifndef CPP_ESSENTIALS_GEO_PERPENDICULAR_HPP_
#define CPP_ESSENTIALS_GEO_PERPENDICULAR_HPP_

#pragma once

#include <cpp_essentials/geo/linear_shape.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct perpendicular_fn
{    
    template <class T>
    auto operator ()(const vector<T, 2>& value) const -> vector<T, 2>
    {
        return math::perpendicular(value);
    }

    template <class T, class Tag>
    auto operator ()(const linear_shape<T, 2, Tag>& value, const vector<T, 2>& origin) const -> linear_shape<T, 2, Tag>
    {
        return{ origin, origin + (*this)(value[1] - value[0]) };
    }

    template <class T, class Tag>
    auto operator ()(const linear_shape<T, 2, Tag>& value) const -> linear_shape<T, 2, Tag>
    {
        return (*this)(value, value[0]);
    }
};

} /* namespace detail */

static constexpr auto perpendicular = detail::perpendicular_fn{};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_PERPENDICULAR_HPP_ */
