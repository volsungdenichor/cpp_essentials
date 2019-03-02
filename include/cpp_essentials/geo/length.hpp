#ifndef CPP_ESSENTIALS_GEO_LENGTH_HPP_
#define CPP_ESSENTIALS_GEO_LENGTH_HPP_

#pragma once

#include <cpp_essentials/geo/distance.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct length_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& item) const
    {
        return math::length(item);
    }

    template <class T, size_t D>
    auto operator ()(const segment<T, D>& item) const
    {
        return distance(item[0], item[1]);
    }
};

} /* namespace detail */

static constexpr detail::length_fn length = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_LENGTH_HPP_ */