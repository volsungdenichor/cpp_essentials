
#ifndef CPP_ESSENTIALS_SQ_NUMERIC_HPP_
#define CPP_ESSENTIALS_SQ_NUMERIC_HPP_

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/sq/detail/numeric_iterator.hpp>

#pragma once

namespace cpp_essentials::sq
{

namespace detail
{

struct range_t
{   
    auto operator ()(int first, int last) const
    {
        int step = first < last ? +1 : -1;

        return core::make_range(
            numeric_iterator<int> { first, step },
            numeric_iterator<int> { last, step });
    }

    auto operator ()(int last) const
    {
        return (*this)(0, last);
    }
};

struct inclusive_range_t
{
    auto operator ()(int first, int last) const
    {
        int step = first < last ? +1 : -1;

        return core::make_range(
            numeric_iterator<int> { first, step },
            numeric_iterator<int> { last + step, step });
    }

    auto operator ()(int last) const
    {
        return (*this)(0, last);
    }
};

} /* namespace detail */

static constexpr detail::range_t range = {};
static constexpr detail::inclusive_range_t inclusive_range = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_NUMERIC_HPP_ */
