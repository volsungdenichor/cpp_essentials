#ifndef CPP_ESSENTIALS_SQ_CHUNK_HPP_
#define CPP_ESSENTIALS_SQ_CHUNK_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/sq/detail/chunk_iterator.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

template <class Iter>
auto chunk(Iter b, Iter e, int size, int step)
{
    return core::make_range(
        chunk_iterator{ b, core::advance(b, e, size), step, e },
        chunk_iterator{ e, e, step, e });
}

struct chunk_t : core::adaptable<chunk_t>
{
    using adaptable::operator();

    template
        < class Range
        , CONCEPT_IF(concepts::InputRange<Range>)>
    auto operator ()(Range&& range, int size, int step) const
    {
        return chunk(std::begin(range), std::end(range), size, step);
    }
};

struct slide_t : core::adaptable<slide_t>
{
    using adaptable::operator();

    template
        < class Range
        , CONCEPT_IF(concepts::InputRange<Range>)>
        auto operator ()(Range&& range, int size) const
    {
        return chunk(std::begin(range), std::end(range), size, 1);
    }
};

struct group_t : core::adaptable<group_t>
{
    using adaptable::operator();

    template
        < class Range
        , CONCEPT_IF(concepts::InputRange<Range>)>
    auto operator ()(Range&& range, int size) const
    {
        return chunk(std::begin(range), std::end(range), size, size);
    }
};

struct pairwise_t : core::adaptable<pairwise_t>
{
    using adaptable::operator();

    template
        < class Range
        , CONCEPT_IF(concepts::InputRange<Range>)>
    auto operator ()(Range&& range) const
    {
        return chunk(std::begin(range), std::end(range), 2, 2);
    }
};

} /* namespace detail */

static constexpr detail::chunk_t chunk = {};
static constexpr detail::slide_t slide = {};
static constexpr detail::group_t group = {};
static constexpr detail::pairwise_t pairwise = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_CHUNK_HPP_ */
