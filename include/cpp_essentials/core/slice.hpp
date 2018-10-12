#ifndef CPP_ESSENTIALS_CORE_SLICE_HPP_
#define CPP_ESSENTIALS_CORE_SLICE_HPP_

#pragma once

#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/detail/nil.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct py_slice_t
{
    template
        < class Range
        , CONCEPT = cc::ForwardRange<Range>>
    auto operator ()(Range&& range, int begin_index, int end_index) const
    {
        const auto r = make_range(range);
        const auto size = static_cast<int>(r.size());

        const auto adjust_index = [](auto index, auto size)
        {
            return clamp(index >= 0 ? index : index + size, 0, size);
        };

        begin_index = begin_index == nil ? 0 : adjust_index(begin_index, size);
        end_index = end_index == nil ? size : adjust_index(end_index, size);

        if (begin_index >= 0 && end_index >= 0 && begin_index < end_index)
        {
            auto b = std::next(r.begin(), begin_index);
            auto e = std::next(b, end_index - begin_index);
            return make_range(b, e);
        }

        return make_range(r.end(), r.end());

    }
};

struct slice_t
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int begin_index, int end_index) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);

        auto count = end_index - begin_index;
        if (count <= 0)
        {
            return make_range(e, e);
        }

        auto begin = advance(b, e, begin_index);
        auto end = advance(begin, e, count);
        return make_range(begin, end);
    }
};

struct take_t
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int count) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return make_range(b, advance(b, e, count));
    }
};

struct drop_t
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int count) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return make_range(advance(b, e, count), e);
    }
};

struct take_back_t
{
    template
        < class Range
        , CONCEPT = cc::BidirectionalRange<Range>>
    auto operator ()(Range&& range, int count) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return make_range(advance_back(b, e, count), e);
    }
};

struct drop_back_t
{
    template
        < class Range
        , CONCEPT = cc::BidirectionalRange<Range>>
    auto operator ()(Range&& range, int count) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return make_range(b, advance_back(b, e, count));
    }
};

struct take_exactly_t
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int count) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return make_range(b, advance_exactly(b, e, count));
    }
};

struct drop_exactly_t
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int count) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return make_range(advance_exactly(b, e, count), e);
    }
};

struct take_back_exactly_t
{
    template
        < class Range
        , CONCEPT = cc::BidirectionalRange<Range>>
    auto operator ()(Range&& range, int count) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return make_range(advance_back_exactly(b, e, count), e);
    }
};

struct drop_back_exactly_t
{
    template
        < class Range
        , CONCEPT = cc::BidirectionalRange<Range>>
    auto operator ()(Range&& range, int count) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return make_range(b, advance_back_exactly(b, e, count));
    }
};

} /* namespace detail */

static constexpr detail::slice_t slice = {};
static constexpr detail::take_t take = {};
static constexpr detail::drop_t drop = {};
static constexpr detail::take_back_t take_back = {};
static constexpr detail::drop_back_t drop_back = {};
static constexpr detail::take_exactly_t take_exactly = {};
static constexpr detail::drop_exactly_t drop_exactly = {};
static constexpr detail::take_back_exactly_t take_back_exactly = {};
static constexpr detail::drop_back_exactly_t drop_back_exactly = {};

static constexpr detail::py_slice_t py_slice = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_SLICE_HPP_ */
