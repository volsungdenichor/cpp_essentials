#ifndef CPP_ESSENTIALS_CORE_ITERATE_HPP_
#define CPP_ESSENTIALS_CORE_ITERATE_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/detail/iterate_iterator.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

struct iterate_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        using cpp_essentials::core::detail::iterate_iterator;
        return make_range(
            iterate_iterator{ std::begin(range) },
            iterate_iterator{ std::end(range) });
    }
};

} /* namespace detail */

static constexpr auto iterate = adaptable{ detail::iterate_fn{} };

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_MAP_HPP_ */
