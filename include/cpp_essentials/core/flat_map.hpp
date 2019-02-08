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
        using type = std::invoke_result_t<UnaryFunc, cc::range_ref<Range>>;
        using underlying_type = std::decay_t<underlying_type_t<type>>;

        static_assert(is_optional_v<type>, "flat_map func: optional return type required");

        const auto f = make_func(func);

        std::vector<underlying_type> result;

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