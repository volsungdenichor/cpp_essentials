#ifndef CPP_ESSENTIALS_CORE_GROUP_BY_HPP_
#define CPP_ESSENTIALS_CORE_GROUP_BY_HPP_

#pragma once

#include <map>
#include <cpp_essentials/core/ref_vector.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <template <class, class> class Map, class Iter, class Func>
auto group_by(Iter begin, Iter end, Func&& func)
{
    using key_type = std::decay_t<decltype(func(*begin))>;
    using value_type = core::vector<cc::iter_ref<Iter>>;
    using result_type = Map<key_type, value_type>;

    result_type result;

    for (auto it = begin; it != end; ++it)
    {
        auto&& value = *it;
        result[func(value)].push_back(value);
    }

    return result;
}

template <template <class, class> class Map>
struct group_by_fn
{
    template
        < class Range
        , class UnaryFunc
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryFunction<UnaryFunc, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryFunc func) const
    {
        return group_by<Map>(std::begin(range), std::end(range), make_func(func));
    }
};

} /* namespace detail */

template <template <class, class> class Map>
static constexpr auto group_by_as = detail::group_by_fn<Map>{};

static constexpr auto group_by = group_by_as<std::unordered_map>;

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_GROUP_BY_HPP_ */