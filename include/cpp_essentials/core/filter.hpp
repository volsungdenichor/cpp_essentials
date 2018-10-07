#ifndef CPP_ESSENTIALS_CORE_FILTER_HPP_
#define CPP_ESSENTIALS_CORE_FILTER_HPP_

#pragma once

#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/detail/filter_iterator.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Iter, class UnaryPred>
auto make_filtered_range(Iter begin, Iter end, UnaryPred pred)
{
    return make_range(
        filter_iterator{ begin, pred, end },
        filter_iterator{ end, pred, end });
}

struct take_if_t
{
    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred pred) const
    {
        return make_filtered_range(std::begin(range), std::end(range), std::move(pred));
    }
};

struct drop_if_t
{
    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred pred) const
    {
        return make_filtered_range(std::begin(range), std::end(range), logical_negation(std::move(pred)));
    }
};

} /* namespace detail */

static constexpr detail::take_if_t take_if = {};
static constexpr detail::drop_if_t drop_if = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_FILTER_HPP_ */
