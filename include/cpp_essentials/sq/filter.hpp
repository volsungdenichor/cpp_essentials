#ifndef CPP_ESSENTIALS_SQ_FILTER_HPP_
#define CPP_ESSENTIALS_SQ_FILTER_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/sq/detail/filter_iterator.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

template <class Iter, class UnaryPred>
auto make_filtered_range(Iter begin, Iter end, UnaryPred pred)
{
    return core::make_range(
        filter_iterator{ begin, pred, end },
        filter_iterator{ end, pred, end });
}

template <class Iter, class UnaryPred>
auto advance_while(Iter it, Iter end, UnaryPred pred) -> Iter
{
    while (it != end && pred(*it))
    {
        ++it;
    }
    return it;
}

template <class Iter, class UnaryPred>
auto take_while(Iter begin, Iter end, UnaryPred pred)
{
    auto it = advance_while(begin, end, pred);
    return core::make_range(begin, it);
}

template <class Iter, class UnaryPred>
auto drop_while(Iter begin, Iter end, UnaryPred pred)
{
    auto it = advance_while(begin, end, pred);
    return core::make_range(it, end);
}

struct take_if_t : core::adaptable<take_if_t>
{
    using adaptable::operator();

    template <class Range, class UnaryPred>
    auto operator ()(Range&& range, UnaryPred pred) const
    {
        return make_filtered_range(std::begin(range), std::end(range), std::move(pred));
    }
};

struct drop_if_t : core::adaptable<drop_if_t>
{
    using adaptable::operator();

    template <class Range, class UnaryPred>
    auto operator ()(Range&& range, UnaryPred pred) const
    {
        return make_filtered_range(std::begin(range), std::end(range), core::logical_negation(std::move(pred)));
    }
};

struct take_while_t : core::adaptable<take_while_t>
{
    using adaptable::operator();

    template <class Range, class UnaryPred>
    auto operator ()(Range&& range, UnaryPred pred) const
    {
        return take_while(std::begin(range), std::end(range), pred);
    }
};

struct drop_while_t : core::adaptable<drop_while_t>
{
    using adaptable::operator();

    template <class Range, class UnaryPred>
    auto operator ()(Range&& range, UnaryPred pred) const
    {
        return drop_while(std::begin(range), std::end(range), pred);
    }
};

struct take_until_t : core::adaptable<take_until_t>
{
    using adaptable::operator();

    template <class Range, class UnaryPred>
    auto operator ()(Range&& range, UnaryPred pred) const
    {
        return take_while(std::begin(range), std::end(range), core::logical_negation(pred));
    }
};

struct drop_until_t : core::adaptable<drop_until_t>
{
    using adaptable::operator();

    template <class Range, class UnaryPred>
    auto operator ()(Range&& range, UnaryPred pred) const
    {
        return drop_while(std::begin(range), std::end(range), core::logical_negation(pred));
    }
};

struct partition_t : core::adaptable<partition_t>
{
    using adaptable::operator();

    template <class Range, class UnaryPred>
    auto operator ()(Range&& range, UnaryPred pred) const
    {
        auto take_if = take_if_t{}(range, pred);
        auto drop_if = drop_if_t{}(range, pred);
        return std::make_tuple(take_if, drop_if);
    }
};

} /* namespace detail */

static constexpr detail::take_if_t take_if = {};
static constexpr detail::drop_if_t drop_if = {};
static constexpr detail::take_while_t take_while = {};
static constexpr detail::drop_while_t drop_while = {};
static constexpr detail::take_until_t take_until = {};
static constexpr detail::drop_until_t drop_until = {};
static constexpr detail::partition_t partition = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_FILTER_HPP_ */
