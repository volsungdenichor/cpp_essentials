#ifndef CPP_ESSENTIALS_SQ_SLICE_HPP_
#define CPP_ESSENTIALS_SQ_SLICE_HPP_

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>

#pragma once

namespace cpp_essentials::sq
{

namespace detail
{

template <class Iter>
auto advance(Iter it, Iter end, int n) -> Iter
{
    while (it != end && n > 0)
    {
        ++it;
        --n;
    }
    return it;
}

struct slice_t : core::adaptable<slice_t>
{
    using adaptable::operator();

    template <class Range>
    auto operator ()(Range&& range, int begin_index, int end_index) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        auto begin = advance(b, e, begin_index);
        auto end = advance(begin, e, end_index - begin_index);
        return core::make_range(begin, end);
    }
};

struct take_t : core::adaptable<take_t>
{
    using adaptable::operator();

    template <class Range>
    auto operator ()(Range&& range, int count) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return core::make_range(b, advance(b, e, count));
    }
};

struct drop_t : core::adaptable<drop_t>
{
    using adaptable::operator();

    template <class Range>
    auto operator ()(Range&& range, int count) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return core::make_range(advance(b, e, count), e);
    }
};

} /* namespace detail */

static constexpr detail::slice_t slice = {};
static constexpr detail::take_t take = {};
static constexpr detail::drop_t drop = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_SLICE_HPP_ */
