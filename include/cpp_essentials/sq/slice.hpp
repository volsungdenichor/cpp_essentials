#ifndef CPP_ESSENTIALS_SQ_SLICE_HPP_
#define CPP_ESSENTIALS_SQ_SLICE_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

struct slice_t : core::adaptable<slice_t>
{
    using adaptable::operator();

    template <class Range>
    auto operator ()(Range&& range, int begin_index, int end_index) const
    {
        static_assert(concepts::InputRange<Range>, "slice: input range required");

        auto b = std::begin(range);
        auto e = std::end(range);

        auto count = end_index - begin_index;
        if (count <= 0)
        {
            return core::make_range(e, e);
        }

        auto begin = core::advance(b, e, begin_index);
        auto end = core::advance(begin, e, count);
        return core::make_range(begin, end);
    }
};

struct take_t : core::adaptable<take_t>
{
    using adaptable::operator();

    template <class Range>
    auto operator ()(Range&& range, int count) const
    {
        static_assert(concepts::InputRange<Range>, "take: input range required");

        auto b = std::begin(range);
        auto e = std::end(range);
        return core::make_range(b, core::advance(b, e, count));
    }
};

struct drop_t : core::adaptable<drop_t>
{
    using adaptable::operator();

    template <class Range>
    auto operator ()(Range&& range, int count) const
    {
        static_assert(concepts::InputRange<Range>, "drop: input range required");

        auto b = std::begin(range);
        auto e = std::end(range);
        return core::make_range(core::advance(b, e, count), e);
    }
};

} /* namespace detail */

static constexpr detail::slice_t slice = {};
static constexpr detail::take_t take = {};
static constexpr detail::drop_t drop = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_SLICE_HPP_ */
