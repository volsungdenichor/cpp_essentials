#ifndef CPP_ESSENTIALS_CORE_VIEWS_MAP_HPP_
#define CPP_ESSENTIALS_CORE_VIEWS_MAP_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/detail/map_iterator.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

struct map_fn
{
    template
        < class Range
        , class UnaryFunc
        , class = cc::InputRange<Range>
        , class = cc::UnaryFunction<UnaryFunc, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryFunc&& func) const
    {
        using cpp_essentials::core::detail::map_iterator;
        auto f = make_func(FORWARD(func));
        return core::make_range(
            map_iterator{ std::begin(range), f },
            map_iterator{ std::end(range), f });
    }    
};

} /* namespace detail */

static constexpr auto transform = detail::map_fn{};
static constexpr auto map = detail::map_fn{};

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_VIEWS_MAP_HPP_ */
