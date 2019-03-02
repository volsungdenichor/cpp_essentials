#ifndef CPP_ESSENTIALS_GEO_CENTER_HPP_
#define CPP_ESSENTIALS_GEO_CENTER_HPP_

#pragma once

#include <cpp_essentials/geo/bounding_box.hpp>
#include <cpp_essentials/geo/linear_shape.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct center_fn
{
    template <class T, size_t D>
    auto operator ()(const bounding_box<T, D>& item) const
    {
        return item.location() + item.size() / 2;
    }

    template <class T, size_t D>
    auto operator ()(const segment<T, D>& value) const
    {
        return (value[0] + value[1]) / 2;
    }
};

} /* namespace detail */

static constexpr detail::center_fn center = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_CENTER_HPP_ */