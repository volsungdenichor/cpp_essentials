#ifndef CPP_ESSENTIALS_CORE_CHAIN_HPP_
#define CPP_ESSENTIALS_CORE_CHAIN_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/detail/chain_iterator.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Iter1, class Iter2>
auto chain(Iter1 b1, Iter1 e1, Iter2 b2, Iter2 e2)
{
    return make_range(
        chain_iterator{ b1, b2, e1, e2 },
        chain_iterator{ e1, e2, e1, b2 });
}

struct append_fn
{
    template
        < class Range1
        , class Range2
        , CONCEPT = cc::InputRange<Range1>
        , CONCEPT = cc::InputRange<Range2>>
    auto operator ()(Range1&& range1, Range2&& range2) const
    {
        return chain(std::begin(range1), std::end(range1), std::begin(range2), std::end(range2));
    }
};

struct prepend_fn
{
    template
        < class Range1
        , class Range2
        , CONCEPT = cc::InputRange<Range1>
        , CONCEPT = cc::InputRange<Range2>>
    auto operator ()(Range1&& range1, Range2&& range2) const
    {
        return chain(std::begin(range2), std::end(range2), std::begin(range1), std::end(range1));
    }
};

} /* namespace chain */

static constexpr detail::append_fn chain = {};
static constexpr detail::append_fn append = {};
static constexpr detail::prepend_fn prepend = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_CHAIN_HPP_ */
