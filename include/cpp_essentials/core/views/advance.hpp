#ifndef CPP_ESSENTIALS_CORE_VIEWS_ADVANCE_HPP_
#define CPP_ESSENTIALS_CORE_VIEWS_ADVANCE_HPP_

#pragma once

#include <cpp_essentials/core/views/slice.hpp>
#include <cpp_essentials/core/views/trim.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

struct advance_fn
{
    template
        < class Range
        , class = cc::InputRange<Range>>
    auto operator ()(Range&& range, std::ptrdiff_t count) const
    {
        const auto b = std::begin(range);
        const auto e = std::end(range);
        const auto it = advance(b, e, count);
        
        
        return std::pair{
            make_range(b, it),
            make_range(it, e)
        };
    }
};

struct advance_while_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::InputRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        const auto b = std::begin(range);
        const auto e = std::end(range);
        const auto it = advance_while(b, e, make_func(pred));

        return std::pair{
            make_range(b, it),
            make_range(it, e)
        };
    }
};

struct advance_until_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::InputRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        const auto b = std::begin(range);
        const auto e = std::end(range);
        const auto it = advance_while(b, e, logical_negation(make_func(pred)));

        return std::pair{
            make_range(b, it),
            make_range(it, e)
        };
    }
};

} /* namespace detail */

static constexpr auto advance = detail::advance_fn{};
static constexpr auto advance_while = detail::advance_while_fn{};
static constexpr auto advance_until = detail::advance_until_fn{};

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_VIEWS_ADVANCE_HPP_ */
