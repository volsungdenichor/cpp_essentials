#ifndef CPP_ESSENTIALS_CORE_SLICE_HPP_
#define CPP_ESSENTIALS_CORE_SLICE_HPP_

#pragma once

#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/nil.hpp>

namespace cpp_essentials::core::views
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

struct split_before_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, cc::range_iter<Range> it) const
    {
        return make_range(std::begin(range), it);
    }
};

struct split_after_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, cc::range_iter<Range> it) const
    {
        return make_range(it, std::end(range));
    }
};

struct split_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, cc::range_iter<Range> it) const
    {
        return make_range_pair(FORWARD(range), it);
    }

    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, std::ptrdiff_t count) const
    {
        auto[b, e] = make_range(range);
        return (*this)(range, advance(b, e, count));
    }
};

} /* namespace detail */

static constexpr auto slice = detail::slice_fn{};
static constexpr auto take = detail::take_fn{};
static constexpr auto drop = detail::drop_fn{};
static constexpr auto take_back = detail::take_back_fn{};
static constexpr auto drop_back = detail::drop_back_fn{};
static constexpr auto take_exactly = detail::take_exactly_fn{};
static constexpr auto drop_exactly = detail::drop_exactly_fn{};
static constexpr auto take_back_exactly = detail::take_back_exactly_fn{};
static constexpr auto drop_back_exactly = detail::drop_back_exactly_fn{};
static constexpr auto split_before = detail::split_before_fn{};
static constexpr auto split_after = detail::split_after_fn{};
static constexpr auto split = detail::split_fn{};

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_SLICE_HPP_ */
