#ifndef CPP_ESSENTIALS_CORE_VIEWS_REPLACE_HPP_
#define CPP_ESSENTIALS_CORE_VIEWS_REPLACE_HPP_

#pragma once

#include <cpp_essentials/core/views/map.hpp>

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
        , class = cc::InputRange<Range>
        , class = cc::UnaryFunction<UnaryPred, cc::range_ref<Range>>
        , class = cc::Convertible<T, cc::range_val<Range>>>
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
        , class = cc::InputRange<Range>
        , class = cc::EqualityCompare<cc::range_val<Range>, T>
        , class = cc::Convertible<U, cc::range_val<Range>>>
    auto operator ()(Range&& range, const T& old_value, const U& new_value) const
    {
        static constexpr auto _replace_if = replace_if_fn{};
        return _replace_if(range, core::equal_to(old_value), new_value);
    }
};

} /* namespace detail */

static constexpr auto replace_if = detail::replace_if_fn{};
static constexpr auto replace = detail::replace_fn{};

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_VIEWS_REPLACE_HPP_ */
