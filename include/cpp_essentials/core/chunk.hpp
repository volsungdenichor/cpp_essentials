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

struct chunk_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int size, int step) const
    {
        return chunk(std::begin(range), std::end(range), size, step);
    }
};

struct slide_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int size) const
    {
        return chunk(std::begin(range), std::end(range), size, 1);
    }
};

struct group_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int size) const
    {
        return chunk(std::begin(range), std::end(range), size, size);
    }
};

struct pairwise_fn
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

static constexpr detail::chunk_fn chunk = {};
static constexpr detail::slide_fn slide = {};
static constexpr detail::group_fn group = {};
static constexpr detail::pairwise_fn pairwise = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_CHUNK_HPP_ */
