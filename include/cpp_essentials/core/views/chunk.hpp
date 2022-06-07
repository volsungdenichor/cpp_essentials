#ifndef CPP_ESSENTIALS_CORE_VIEWS_CHUNK_HPP_
#define CPP_ESSENTIALS_CORE_VIEWS_CHUNK_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/detail/chunk_iterator.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

template <class Iter>
auto chunk(Iter b, Iter e, int size, int step)
{
    using cpp_essentials::core::detail::chunk_iterator;
    return core::make_range(
        chunk_iterator{ b, advance(b, e, size), step, e },
        chunk_iterator{ e, e, step, e });
}

struct chunk_fn
{
    template
        < class Range
        , class = cc::InputRange<Range>>
    auto operator ()(Range&& range, int size, int step) const
    {
        return chunk(std::begin(range), std::end(range), size, step);
    }
};

struct slide_fn
{
    template
        < class Range
        , class = cc::InputRange<Range>>
    auto operator ()(Range&& range, int size) const
    {
        return chunk(std::begin(range), std::end(range), size, 1);
    }
};

struct group_fn
{
    template
        < class Range
        , class = cc::InputRange<Range>>
    auto operator ()(Range&& range, int size) const
    {
        return chunk(std::begin(range), std::end(range), size, size);
    }
};

struct pairwise_fn
{
    template
        < class Range
        , class = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        return chunk(std::begin(range), std::end(range), 2, 2);
    }
};

} /* namespace detail */

static constexpr auto chunk = detail::chunk_fn{};
static constexpr auto slide = detail::slide_fn{};
static constexpr auto group = detail::group_fn{};
static constexpr auto pairwise = detail::pairwise_fn{};

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_VIEWS_CHUNK_HPP_ */
