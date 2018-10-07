#ifndef CPP_ESSENTIALS_CORE_ITERATE_HPP_
#define CPP_ESSENTIALS_CORE_ITERATE_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/detail/iterate_iterator.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct iterate_t
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        return make_range(
            iterate_iterator{ std::begin(range) },
            iterate_iterator{ std::end(range) });
    }
};

} /* namespace detail */

static constexpr detail::iterate_t iterate = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_MAP_HPP_ */