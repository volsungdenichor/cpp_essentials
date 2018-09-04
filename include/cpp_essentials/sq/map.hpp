#ifndef CPP_ESSENTIALS_SQ_MAP_HPP_
#define CPP_ESSENTIALS_SQ_MAP_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/sq/detail/map_iterator.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

struct map_t : core::adaptable<map_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryFunc
        , CONCEPT_IF(concepts::InputRange<Range>)>
    auto operator ()(Range&& range, UnaryFunc func) const
    {
        return core::make_range(
            map_iterator{ std::begin(range), func },
            map_iterator{ std::end(range), func });
    }
};

} /* namespace detail */

static constexpr detail::map_t map = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_MAP_HPP_ */
