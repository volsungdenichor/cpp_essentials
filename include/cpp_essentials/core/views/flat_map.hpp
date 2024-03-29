#ifndef CPP_ESSENTIALS_CORE_VIEWS_FLAT_MAP_HPP_
#define CPP_ESSENTIALS_CORE_VIEWS_FLAT_MAP_HPP_

#pragma once

#include <cpp_essentials/core/views/map.hpp>
#include <cpp_essentials/core/views/flatten.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

struct flat_map_fn
{
    template
        < class Range
        , class UnaryFunc
        , class = cc::InputRange<Range>
        , class = cc::UnaryFunction<UnaryFunc, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryFunc&& func) const
    {
        static constexpr auto _map = map_fn{};
        static constexpr auto _flatten = flatten_fn{};
        return _flatten(_map(FORWARD(range), std::move(func)));
    }
};

} /* namespace detail */

static constexpr auto flat_map = detail::flat_map_fn{};

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_VIEWS_FLAT_MAP_HPP_ */