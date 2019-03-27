#ifndef CPP_ESSENTIALS_CORE_SLICE_HPP_
#define CPP_ESSENTIALS_CORE_SLICE_HPP_

#pragma once

#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/nil.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct slice_fn
{
    std::ptrdiff_t adjust_index(std::ptrdiff_t index, std::ptrdiff_t size) const
    {
        return clamp(index >= 0 ? index : index + size, std::ptrdiff_t{ 0 }, size);
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
    auto operator ()(Range&& range, std::ptrdiff_t begin_index, nil_t /* end_index */) const
    {
        const auto r = make_range(range);

        if (begin_index >= 0)
        {
            return make_range(advance(r.begin(), r.end(), begin_index), r.end());
        }
        
        const auto size = r.size();

        begin_index = adjust_index(begin_index, size);

        return begin_index >= 0
            ? make_range(std::next(r.begin(), begin_index), r.end())
            : make_range(r.end(), r.end());
    }

    template
        < class Range
        , CONCEPT = cc::ForwardRange<Range>>
    auto operator ()(Range&& range, nil_t /* begin_index */, std::ptrdiff_t end_index) const
    {
        const auto r = make_range(range);

        if (end_index >= 0)
        {
            return make_range(r.begin(), advance(r.begin(), r.end(), end_index));
        }

        const auto size = r.size();

        end_index = adjust_index(end_index, size);

        return end_index >= 0
            ? make_range(r.begin(), std::next(r.begin(), end_index))
            : make_range(r.end(), r.end());
    }

    template
        < class Range
        , CONCEPT = cc::ForwardRange<Range>>
    auto operator ()(Range&& range, std::ptrdiff_t begin_index, std::ptrdiff_t end_index) const
    {
        const auto r = make_range(range);

        if (begin_index >= 0 && end_index >= 0 && begin_index <= end_index)
        {
            auto b = advance(r.begin(), r.end(), begin_index);
            auto e = advance(b, r.end(), end_index - begin_index);
            return make_range(b, e);
        }

        const auto size = r.size();

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

struct take_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, std::ptrdiff_t count) const
    {
        auto[b, e] = make_range(range);
        return make_range(b, advance(b, e, count));
    }
};

struct drop_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, std::ptrdiff_t count) const
    {
        auto[b, e] = make_range(range);
        return make_range(advance(b, e, count), e);
    }
};

struct take_back_fn
{
    template
        < class Range
        , CONCEPT = cc::BidirectionalRange<Range>>
    auto operator ()(Range&& range, std::ptrdiff_t count) const
    {
        auto[b, e] = make_range(range);
        return make_range(advance_back(b, e, count), e);
    }
};

struct drop_back_fn
{
    template
        < class Range
        , CONCEPT = cc::BidirectionalRange<Range>>
    auto operator ()(Range&& range, std::ptrdiff_t count) const
    {
        auto[b, e] = make_range(range);
        return make_range(b, advance_back(b, e, count));
    }
};

struct take_exactly_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, std::ptrdiff_t count) const
    {
        auto[b, e] = make_range(range);
        return make_range(b, advance_exactly(b, e, count));
    }
};

struct drop_exactly_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, std::ptrdiff_t count) const
    {
        auto[b, e] = make_range(range);
        return make_range(advance_exactly(b, e, count), e);
    }
};

struct take_back_exactly_fn
{
    template
        < class Range
        , CONCEPT = cc::BidirectionalRange<Range>>
    auto operator ()(Range&& range, std::ptrdiff_t count) const
    {
        auto[b, e] = make_range(range);
        return make_range(advance_back_exactly(b, e, count), e);
    }
};

struct drop_back_exactly_fn
{
    template
        < class Range
        , CONCEPT = cc::BidirectionalRange<Range>>
    auto operator ()(Range&& range, std::ptrdiff_t count) const
    {
        auto[b, e] = make_range(range);
        return make_range(b, advance_back_exactly(b, e, count));
    }
};

} /* namespace detail */

static constexpr detail::slice_fn slice = {};
static constexpr detail::take_fn take = {};
static constexpr detail::drop_fn drop = {};
static constexpr detail::take_back_fn take_back = {};
static constexpr detail::drop_back_fn drop_back = {};
static constexpr detail::take_exactly_fn take_exactly = {};
static constexpr detail::drop_exactly_fn drop_exactly = {};
static constexpr detail::take_back_exactly_fn take_back_exactly = {};
static constexpr detail::drop_back_exactly_fn drop_back_exactly = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_SLICE_HPP_ */
