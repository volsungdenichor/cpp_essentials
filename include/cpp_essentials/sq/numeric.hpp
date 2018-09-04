
#ifndef CPP_ESSENTIALS_SQ_NUMERIC_HPP_
#define CPP_ESSENTIALS_SQ_NUMERIC_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/sq/detail/numeric_iterator.hpp>

namespace cpp_essentials::sq
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

struct iota_t
{
    template <class T>
    auto operator ()(T start) const
    {
        return core::make_range(
            iota_iterator<T>{ start },
            iota_iterator<T>{ });
    }

    auto operator ()() const
    {
        return (*this)(0);
    }
};

} /* namespace detail */

static constexpr detail::range_t range = {};
static constexpr detail::inclusive_range_t inclusive_range = {};
static constexpr detail::iota_t iota = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_NUMERIC_HPP_ */
