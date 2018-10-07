#ifndef CPP_ESSENTIALS_CORE_BACKWARDS_HPP_
#define CPP_ESSENTIALS_CORE_BACKWARDS_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/detail/zip_iterator.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct backwards_t
{
    template <class Iter>
    auto make_iterator(Iter iter) const
    {
        return std::make_reverse_iterator(iter);
    }

    template <class Iter>
    auto make_iterator(std::reverse_iterator<Iter> iter) const
    {
        return iter.base();
    }

    template
        < class Range
        , CONCEPT = cc::BidirectionalRange<Range>>
    auto operator ()(Range&& range) const
    {
        return make_range(
            make_iterator(std::end(range)),
            make_iterator(std::begin(range)));
    }
};

} /* namespace detail */

static constexpr detail::backwards_t backwards = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_BACKWARDS_HPP_ */
