#ifndef CPP_ESSENTIALS_CORE_FILTER_HPP_
#define CPP_ESSENTIALS_CORE_FILTER_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/detail/filter_iterator.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

template <class Iter, class UnaryPred>
auto make_filtered_range(Iter begin, Iter end, UnaryPred pred)
{
    using cpp_essentials::core::detail::filter_iterator;
    return make_range(
        filter_iterator{ begin, pred, end },
        filter_iterator{ end, pred, end });
}

struct take_if_fn
{
    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred pred) const
    {
        return make_filtered_range(std::begin(range), std::end(range), make_func(pred));
    }
};

struct drop_if_fn
{
    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred pred) const
    {
        return make_filtered_range(std::begin(range), std::end(range), logical_negation(make_func(pred)));
    }
};

} /* namespace detail */

static constexpr auto take_if = adaptable{ detail::take_if_fn{} };
static constexpr auto drop_if = adaptable{ detail::drop_if_fn{} };

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_FILTER_HPP_ */
