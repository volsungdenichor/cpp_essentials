#ifndef CPP_ESSENTIALS_SQ_ITERATE_HPP_
#define CPP_ESSENTIALS_SQ_ITERATE_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/sq/detail/iterate_iterator.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

struct iterate_t : core::adaptable<iterate_t>
{
    using adaptable::operator();

    template
        < class Range
        , CONCEPT_IF(concepts::InputRange<Range>)>
    auto operator ()(Range&& range) const
    {
        return core::make_range(
            iterate_iterator{ std::begin(range) },
            iterate_iterator{ std::end(range) });
    }
};

} /* namespace detail */

static constexpr detail::iterate_t iterate = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_MAP_HPP_ */
