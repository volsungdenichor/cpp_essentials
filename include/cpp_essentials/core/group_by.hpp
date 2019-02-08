#ifndef CPP_ESSENTIALS_CORE_GROUP_BY_HPP_
#define CPP_ESSENTIALS_CORE_GROUP_BY_HPP_

#pragma once

#include <unordered_map>
#include <cpp_essentials/core/ref_vector.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Iter, class Func>
auto group_by(Iter begin, Iter end, Func&& func)
{
    using key_type = std::decay_t<decltype(func(*begin))>;
    using value_type = std::conditional_t
        < std::is_reference_v<cc::iter_ref<Iter>>
        , core::ref_vector<std::remove_reference_t<cc::iter_ref<Iter>>>
        , std::vector<cc::iter_val<Iter>>>;

    std::unordered_map<key_type, value_type> result;

    for (auto it = begin; it != end; ++it)
    {
        auto&& value = *it;
        result[func(value)].push_back(value);
    }

    return result;
}

struct group_by_fn
{
    template
        < class Range
        , class UnaryFunc
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryFunction<UnaryFunc, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryFunc func) const
    {
        return group_by(std::begin(range), std::end(range), make_func(func));
    }
};

} /* namespace detail */

static constexpr detail::group_by_fn group_by = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_GROUP_BY_HPP_ */