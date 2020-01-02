#ifndef CPP_ESSENTIALS_GEO_CLAMP_HPP_
#define CPP_ESSENTIALS_GEO_CLAMP_HPP_

#pragma once

#include <cpp_essentials/geo/bounding_box.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct clamp_fn
{
    template <class T>
    T operator ()(T value, const interval<T>& item) const
    {
        return core::clamp(value, item.min(), item.max());
    }

    template <class T, size_t D>
    vector<T, D> operator ()(const vector<T, D>& value, const bounding_box<T, D>& item) const
    {
        vector<T, D> result = value;
        for (size_t i = 0; i < D; ++i)
        {
            result[i] = (*this)(value[i], item._data[i]);
        }
        return result;
    }
};

} /* namespace detail */

static constexpr auto clamp = detail::clamp_fn{};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_CLAMP_HPP_ */
