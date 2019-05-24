#ifndef CPP_ESSENTIALS_CORE_REPLACE_HPP_
#define CPP_ESSENTIALS_CORE_REPLACE_HPP_

#pragma once

#include <cpp_essentials/core/map.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

struct replace_if_fn
{
    template
        < class Range
        , class UnaryPred
        , class T
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryFunction<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred pred, const T& new_value) const
    {
        return map(range, [=](auto&& item) { return pred(item) ? new_value : item; });
    }
};

struct replace_fn
{
    template
        < class Range
        , class T
        , class U
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, const T& old_value, const U& new_value) const
    {
        return map(range, [=](auto&& item) { return item == old_value ? new_value : item; });
    }
};

} /* namespace detail */

static constexpr auto replace_if = adaptable{ detail::replace_if_fn{} };
static constexpr auto replace = adaptable{ detail::replace_fn{} };

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_REPLACE_HPP_ */
