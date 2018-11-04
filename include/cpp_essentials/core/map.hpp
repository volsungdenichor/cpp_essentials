#ifndef CPP_ESSENTIALS_CORE_MAP_HPP_
#define CPP_ESSENTIALS_CORE_MAP_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/detail/map_iterator.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct map_fn
{
    template
        < class Range
        , class UnaryFunc
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryFunction<UnaryFunc, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryFunc func) const
    {
        return core::make_range(
            map_iterator{ std::begin(range), func },
            map_iterator{ std::end(range), func });
    }

    template
        < class Range
        , class Type
        , class T
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::BaseOf<T, cc::range_val<Range>>>
    auto operator ()(Range&& range, Type T::*field) const
    {
        return (*this)(range, std::mem_fn(field));
    }
};

} /* namespace detail */

static constexpr detail::map_fn map = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_MAP_HPP_ */
