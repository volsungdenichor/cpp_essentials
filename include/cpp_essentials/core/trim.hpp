#ifndef CPP_ESSENTIALS_CORE_TRIM_HPP_
#define CPP_ESSENTIALS_CORE_TRIM_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/functors.hpp>

namespace cpp_essentials::core
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

struct take_while_t : adaptable<take_while_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return take_while(std::begin(range), std::end(range), pred);
    }
};

struct drop_while_t : adaptable<drop_while_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return drop_while(std::begin(range), std::end(range), pred);
    }
};

struct take_until_t : adaptable<take_until_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return take_while(std::begin(range), std::end(range), logical_negation(pred));
    }
};

struct drop_until_t : adaptable<drop_until_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return drop_while(std::begin(range), std::end(range), logical_negation(pred));
    }
};

struct take_back_while_t : adaptable<take_back_while_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::BidirectionalRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return take_back_while(std::begin(range), std::end(range), pred);
    }
};

struct drop_back_while_t : adaptable<drop_back_while_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::BidirectionalRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return drop_back_while(std::begin(range), std::end(range), pred);
    }
};

struct take_back_until_t : adaptable<take_back_until_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::BidirectionalRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return take_back_while(std::begin(range), std::end(range), logical_negation(pred));
    }
};

struct drop_back_until_t : adaptable<drop_back_until_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::BidirectionalRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return drop_back_while(std::begin(range), std::end(range), logical_negation(pred));
    }
};

struct trim_while_t : adaptable<trim_while_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::BidirectionalRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return trim_while(std::begin(range), std::end(range), pred);
    }
};

struct trim_until_t : adaptable<trim_until_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT = cc::BidirectionalRange<Range>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
        auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return trim_while(std::begin(range), std::end(range), logical_negation(pred));
    }
};

} /* namespace detail */

static constexpr detail::take_while_t take_while = {};
static constexpr detail::drop_while_t drop_while = {};
static constexpr detail::take_until_t take_until = {};
static constexpr detail::drop_until_t drop_until = {};

static constexpr detail::take_back_while_t take_back_while = {};
static constexpr detail::drop_back_while_t drop_back_while = {};
static constexpr detail::take_back_until_t take_back_until = {};
static constexpr detail::drop_back_until_t drop_back_until = {};

static constexpr detail::trim_while_t trim_while = {};
static constexpr detail::trim_until_t trim_until = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_TRIM_HPP_ */
