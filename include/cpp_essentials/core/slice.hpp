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

struct slice_t
{
    int adjust_index(int index, int size) const
    {
        return clamp(index >= 0 ? index : index + size, 0, size);
    }

    template
        < class Range
        , CONCEPT = cc::ForwardRange<Range>>
    auto operator ()(Range&& range, nil_t /* begin_index */, nil_t /* end_index */) const
    {
        return make_range(range);        
    }

    template
        < class Range
        , CONCEPT = cc::ForwardRange<Range>>
    auto operator ()(Range&& range, int begin_index, nil_t /* end_index */) const
    {
        const auto r = make_range(range);

        if (begin_index >= 0)
        {
            return make_range(advance(r.begin(), r.end(), begin_index), r.end());
        }
        
        const auto size = static_cast<int>(r.size());

        begin_index = adjust_index(begin_index, size);

        return begin_index >= 0
            ? make_range(std::next(r.begin(), begin_index), r.end())
            : make_range(r.end(), r.end());
    }

    template
        < class Range
        , CONCEPT = cc::ForwardRange<Range>>
    auto operator ()(Range&& range, nil_t /* begin_index */, int end_index) const
    {
        const auto r = make_range(range);

        if (end_index >= 0)
        {
            return make_range(r.begin(), advance(r.begin(), r.end(), end_index));
        }

        const auto size = static_cast<int>(r.size());

        end_index = adjust_index(end_index, size);

        return end_index >= 0
            ? make_range(r.begin(), std::next(r.begin(), end_index))
            : make_range(r.end(), r.end());
    }

    template
        < class Range
        , CONCEPT = cc::ForwardRange<Range>>
    auto operator ()(Range&& range, int begin_index, int end_index) const
    {
        const auto r = make_range(range);

        if (begin_index >= 0 && end_index >= 0 && begin_index <= end_index)
        {
            auto b = advance(r.begin(), r.end(), begin_index);
            auto e = advance(b, r.end(), end_index - begin_index);
            return make_range(b, e);
        }

        const auto size = static_cast<int>(r.size());

        begin_index = adjust_index(begin_index, size);
        end_index = adjust_index(end_index, size);

        if (begin_index >= 0 && end_index >= 0 && begin_index < end_index)
        {
            auto b = std::next(r.begin(), begin_index);
            auto e = std::next(b, end_index - begin_index);
            return make_range(b, e);
        }

        return make_range(r.end(), r.end());
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

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_SLICE_HPP_ */
