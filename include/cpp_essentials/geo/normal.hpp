#pragma once

#include <cpp_essentials/geo/perpendicular.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct normal_fn
{
    template <class T, class Tag>
    auto operator ()(const linear_shape<T, 2, Tag>& value) const -> vector<T, 2>
    {
        return perpendicular(direction(value));
    }
};

} /* namespace detail */

static constexpr detail::normal_fn normal = {};

} /* namespace cpp_essentials::geo */