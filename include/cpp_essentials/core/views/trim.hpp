#ifndef CPP_ESSENTIALS_CORE_VIEWS_TRIM_HPP_
#define CPP_ESSENTIALS_CORE_VIEWS_TRIM_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/functors.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

template <class Iter, class UnaryPred>
auto advance_while(Iter begin, Iter end, UnaryPred&& pred) -> Iter
{
    while (begin != end && pred(*begin))
    {
        ++begin;
    }
    return begin;
}

template <class Iter, class UnaryPred>
auto advance_back_while(Iter begin, Iter end, UnaryPred&& pred) -> Iter
{
    while (begin != end && pred(*std::prev(end)))
    {
        --end;
    }
    return end;
}

template <class Iter, class UnaryPred>
auto take_while(Iter begin, Iter end, UnaryPred&& pred)
{
    return make_range(begin, advance_while(begin, end, pred));
}

template <class Iter, class UnaryPred>
auto drop_while(Iter begin, Iter end, UnaryPred&& pred)
{
    return make_range(advance_while(begin, end, pred), end);
}

template <class Iter, class UnaryPred>
auto take_back_while(Iter begin, Iter end, UnaryPred&& pred)
{
    return make_range(advance_back_while(begin, end, pred), end);
}

template <class Iter, class UnaryPred>
auto drop_back_while(Iter begin, Iter end, UnaryPred&& pred)
{
    return make_range(begin, advance_back_while(begin, end, pred));
}

template <class Iter, class UnaryPred>
auto trim_while(Iter begin, Iter end, UnaryPred&& pred)
{
    auto b = advance_while(begin, end, pred);
    auto e = advance_back_while(b, end, pred);
    return make_range(b, e);
}

struct take_while_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::InputRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return take_while(std::begin(range), std::end(range), make_func(pred));
    }
};

struct drop_while_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::InputRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return drop_while(std::begin(range), std::end(range), make_func(pred));
    }
};

struct take_until_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::InputRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return take_while(std::begin(range), std::end(range), logical_negation(make_func(pred)));
    }
};

struct drop_until_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::InputRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return drop_while(std::begin(range), std::end(range), logical_negation(make_func(pred)));
    }
};

struct take_back_while_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::BidirectionalRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return take_back_while(std::begin(range), std::end(range), make_func(pred));
    }
};

struct drop_back_while_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::BidirectionalRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return drop_back_while(std::begin(range), std::end(range), make_func(pred));
    }
};

struct take_back_until_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::BidirectionalRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return take_back_while(std::begin(range), std::end(range), logical_negation(make_func(pred)));
    }
};

struct drop_back_until_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::BidirectionalRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return drop_back_while(std::begin(range), std::end(range), logical_negation(make_func(pred)));
    }
};

struct trim_while_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::BidirectionalRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return trim_while(std::begin(range), std::end(range), make_func(pred));
    }
};

struct trim_until_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::BidirectionalRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return trim_while(std::begin(range), std::end(range), logical_negation(make_func(pred)));
    }
};

struct split_while_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::InputRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        auto[b, e] = make_range(range);
        auto m = advance_while(b, e, make_func(pred));
        return make_range_pair(b, e, m);
    }
};

struct split_until_fn
{
    template
        < class Range
        , class UnaryPred
        , class = cc::InputRange<Range>
        , class = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        auto[b, e] = make_range(range);
        auto m = advance_while(b, e, logical_negation(make_func(pred)));
        return make_range_pair(b, e, m);
    }
};

} /* namespace detail */

static constexpr auto take_while = detail::take_while_fn{};
static constexpr auto drop_while = detail::drop_while_fn{};
static constexpr auto take_until = detail::take_until_fn{};
static constexpr auto drop_until = detail::drop_until_fn{};

static constexpr auto take_back_while = detail::take_back_while_fn{};
static constexpr auto drop_back_while = detail::drop_back_while_fn{};
static constexpr auto take_back_until = detail::take_back_until_fn{};
static constexpr auto drop_back_until = detail::drop_back_until_fn{};

static constexpr auto trim_while = detail::trim_while_fn{};
static constexpr auto trim_until = detail::trim_until_fn{};

static constexpr auto split_while = detail::split_while_fn{};
static constexpr auto split_until = detail::split_until_fn{};

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_VIEWS_TRIM_HPP_ */
