#ifndef CPP_ESSENTIALS_CORE_FLAT_MAP_HPP_
#define CPP_ESSENTIALS_CORE_FLAT_MAP_HPP_

#pragma once

#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/functors.hpp>

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
        const auto f = make_func(func);
        using type = std::decay_t<decltype(*f(*std::begin(range)))>;

        std::vector<type> result;

        for (auto&& item : range)
        {
            auto value = f(item);
            if (has_value(value))
            {
                result.push_back(dereference(value));
            }
        }
        return result;
    }
};

} /* namespace detail */

static constexpr detail::flat_map_fn flat_map = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_FLAT_MAP_HPP_ */