
#ifndef CPP_ESSENTIALS_CORE_NUMERIC_HPP_
#define CPP_ESSENTIALS_CORE_NUMERIC_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/detail/numeric_iterator.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct range_t
{   
    template <class T>
    auto operator ()(T first, T last) const
    {
        int step = first < last ? +1 : -1;

        return core::make_range(
            numeric_iterator{ first, step },
            numeric_iterator{ last, step });
    }

    template <class T>
    auto operator ()(T last) const
    {
        return (*this)(T{}, last);
    }
};

struct inclusive_range_t
{
    template <class T>
    auto operator ()(T first, T last) const
    {
        int step = first < last ? +1 : -1;

        return core::make_range(
            numeric_iterator{ first, step },
            numeric_iterator{ T(last + step), step });
    }

    template <class T>
    auto operator ()(T last) const
    {
        return (*this)(T{}, last);
    }
};

} /* namespace detail */

static constexpr detail::range_t range = {};
static constexpr detail::inclusive_range_t inclusive_range = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_NUMERIC_HPP_ */
