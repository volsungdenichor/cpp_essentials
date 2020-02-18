#ifndef CPP_ESSENTIALS_CORE_FILTER_MAP_HPP_
#define CPP_ESSENTIALS_CORE_FILTER_MAP_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/detail/flat_map_iterator.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

struct filter_map_fn
{
    template
        < class Range
        , class UnaryFunc
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryFunction<UnaryFunc, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryFunc&& func) const
    {
        using cpp_essentials::core::detail::flat_map_iterator;
        
        auto f = make_func(std::move(func));
        auto b = std::begin(range);
        auto e = std::end(range);

        return make_range(
            flat_map_iterator{ b, f, e },
            flat_map_iterator{ e, f, e });
    }
};

} /* namespace detail */

static constexpr auto filter_map = detail::filter_map_fn{};

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_FILTER_MAP_HPP_ */