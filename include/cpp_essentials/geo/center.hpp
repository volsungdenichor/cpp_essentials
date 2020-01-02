#ifndef CPP_ESSENTIALS_GEO_CENTER_HPP_
#define CPP_ESSENTIALS_GEO_CENTER_HPP_

#pragma once

#include <cpp_essentials/geo/bounding_box.hpp>
#include <cpp_essentials/geo/linear_shape.hpp>
#include <cpp_essentials/geo/vertex_array.hpp>
#include <cpp_essentials/geo/circular_shape.hpp>

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
    auto operator ()(const segment<T, D>& item) const
    {
        return (item[0] + item[1]) / 2;
    }

    template <class T, size_t D, size_t N>
    auto operator ()(const vertex_array<T, D, N, detail::polygon_tag>& item) const
    {
        return core::accumulate(item._data, vector<T, D>{}) / item.size();
    }

    template <class T, size_t D>
    auto operator ()(const circular_shape<T, D>& item) const
    {
        return item.center();
    }
};

} /* namespace detail */

static constexpr auto center = detail::center_fn{};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_CENTER_HPP_ */