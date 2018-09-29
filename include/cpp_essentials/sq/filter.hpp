#ifndef CPP_ESSENTIALS_SQ_FILTER_HPP_
#define CPP_ESSENTIALS_SQ_FILTER_HPP_

#pragma once

#include <cpp_essentials/core/filter.hpp>
#include <cpp_essentials/sq/detail/filter_iterator.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

template <class Iter, class UnaryPred>
auto make_filtered_range(Iter begin, Iter end, UnaryPred pred)
{
    return core::make_range(
        filter_iterator{ begin, pred, end },
        filter_iterator{ end, pred, end });
}

struct take_if_t : core::adaptable<take_if_t>
{
    using adaptable::operator();

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

struct drop_if_t : core::adaptable<drop_if_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred pred) const
    {
        return make_filtered_range(std::begin(range), std::end(range), core::logical_negation(std::move(pred)));
    }
};

struct partition_t : core::adaptable<partition_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred pred) const
    {
        auto take_if = take_if_t{}(range, pred);
        auto drop_if = drop_if_t{}(range, pred);
        return std::make_tuple(take_if, drop_if);
    }
};

} /* namespace detail */

static constexpr detail::take_if_t take_if = {};
static constexpr detail::drop_if_t drop_if = {};
static constexpr detail::partition_t partition = {};

using core::take_while;
using core::drop_while;
using core::take_until;
using core::drop_until;

using core::take_back_while;
using core::drop_back_while;
using core::take_back_until;
using core::drop_back_until;

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_FILTER_HPP_ */
