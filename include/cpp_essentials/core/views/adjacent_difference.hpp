#ifndef CPP_ESSENTIALS_CORE_VIEWS_ADJACENT_DIFFERENCE_HPP_
#define CPP_ESSENTIALS_CORE_VIEWS_ADJACENT_DIFFERENCE_HPP_

#pragma once

#include <cpp_essentials/core/detail/adjacent_difference_iterator.hpp>
#include <cpp_essentials/core/iterator_range.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

struct adjacent_difference_fn
{
    template
        < class Range
        , class BinaryFunc = std::minus<>
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::BinaryFunction<BinaryFunc, cc::range_ref<Range>, cc::range_ref<Range>>>
    auto operator ()(Range&& range, BinaryFunc func = {}) const
    {
        const auto b = std::begin(range);
        const auto e = std::end(range);
        return core::make_range(
            core::detail::adjacent_difference_iterator{ b, func, e },
            core::detail::adjacent_difference_iterator{ e, func, e });
    }
};

} /* namespace detail */

static constexpr auto adjacent_difference = detail::adjacent_difference_fn{};

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_VIEWS_ADJACENT_DIFFERENCE_HPP_ */
