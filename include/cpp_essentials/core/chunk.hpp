#ifndef CPP_ESSENTIALS_CORE_CHUNK_HPP_
#define CPP_ESSENTIALS_CORE_CHUNK_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/detail/chunk_iterator.hpp>

namespace cpp_essentials::core
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

struct chunk_t
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int size, int step) const
    {
        return chunk(std::begin(range), std::end(range), size, step);
    }
};

struct slide_t
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int size) const
    {
        return chunk(std::begin(range), std::end(range), size, 1);
    }
};

struct group_t
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int size) const
    {
        return chunk(std::begin(range), std::end(range), size, size);
    }
};

struct pairwise_t
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
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

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_CHUNK_HPP_ */
