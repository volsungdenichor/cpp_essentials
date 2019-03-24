#ifndef CPP_ESSENTIALS_CORE_FLAT_MAP_HPP_
#define CPP_ESSENTIALS_CORE_FLAT_MAP_HPP_

#pragma once

#include <cpp_essentials/core/detail/flat_map_iterator.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct flat_map_fn
{
    template
        < class Range
        , class UnaryFunc
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryFunction<UnaryFunc, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryFunc&& func) const
    {
        auto f = make_func(std::move(func));
        auto b = std::begin(range);
        auto e = std::end(range);

        return make_range(
            flat_map_iterator{ b, f, e },
            flat_map_iterator{ e, f, e });       
    }
};

} /* namespace detail */

static constexpr detail::flat_map_fn flat_map = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_FLAT_MAP_HPP_ */