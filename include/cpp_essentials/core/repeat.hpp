#ifndef CPP_ESSENTIALS_CORE_REPEAT_HPP_
#define CPP_ESSENTIALS_CORE_REPEAT_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/detail/repeat_iterator.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <int Count = 0>
struct repeat_fn
{
    template <class T>
    auto operator ()(const T& value) const
    {
        return make_range(
            repeat_iterator{ value, 0 },
            repeat_iterator{ value, Count });
    }
};

template <>
struct repeat_fn<0>
{
    template <class T>
    auto operator ()(const T& value, int count) const
    {
        return make_range(
            repeat_iterator{ value, 0 },
            repeat_iterator{ value, count });
    }

    template <class T>
    auto operator ()(const T& value) const
    {
        return (*this)(value, std::numeric_limits<int>::min());
    }
};

} /* namespace detail */

static constexpr auto repeat = detail::repeat_fn<>{};
static constexpr auto once = detail::repeat_fn<1>{};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_REPEAT_HPP_ */
