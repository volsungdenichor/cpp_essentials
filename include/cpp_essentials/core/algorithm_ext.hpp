#ifndef CPP_ESSENTIALS_CORE_ALGORITHM_EXT_HPP_
#define CPP_ESSENTIALS_CORE_ALGORITHM_EXT_HPP_

#pragma once

#include <algorithm>
#include <numeric>

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/return_policy.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Iter, class OutputIter, class UnaryPred>
Iter copy_while(Iter first, Iter last, OutputIter output, UnaryPred pred)
{
    while (first != last && pred(*first))
    {
        *output = *first;
        ++output;
        ++first;
    }
    return first;
}

template <class Iter>
bool is_not_empty(Iter b, Iter e)
{
    return b != e;
}

template <class Iter>
bool is_single(Iter b, Iter e)
{
    return b != e && std::next(b) == e;
}

struct front_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    decltype(auto) operator ()(Range&& range) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        EXPECTS(is_not_empty(b, e), "empty range");
        return *b;
    }
};

struct front_or_throw_fn
{
    template
        < class Range
        , class Exception
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::BaseOf<std::exception, Exception>>
    decltype(auto) operator ()(Range&& range, const Exception& exception) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        if (!is_not_empty(b, e))
        {
            throw exception;
        }
        return *b;
    }

    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    decltype(auto) operator ()(Range&& range, const std::string& message) const
    {
        return (*this)(range, std::runtime_error{ message });
    }
};

struct front_or_fn
{
    template
        < class Range
        , class T
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, const T& default_value) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return is_not_empty(b, e) ? *b : default_value;
    }
};

struct front_or_default_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return is_not_empty(b, e) ? *b : cc::range_val<Range>{};
    }
};

struct front_or_eval_fn
{
    template
        < class Range
        , class Func
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::NullaryFunction<Func>>
    auto operator ()(Range&& range, Func func) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return is_not_empty(b, e) ? *b : func();
    }
};

struct front_or_none_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const -> decltype(auto)
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return eval_optional(is_not_empty(b, e), [&]() -> decltype(auto) { return *b; });
    }
};

struct single_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    decltype(auto) operator ()(Range&& range) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        EXPECTS(is_single(b, e), "zero or more than one element");
        return *b;
    }
};

struct single_or_throw_fn
{
    template
        < class Range
        , class Exception
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::BaseOf<std::exception, Exception>>
    decltype(auto) operator ()(Range&& range, const Exception& exception) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        if (!is_single(b, e))
        {
            throw exception;
        }
        return *b;
    }

    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    decltype(auto) operator ()(Range&& range, const std::string& message) const
    {
        return (*this)(range, std::runtime_error{ message });
    }
};

struct single_or_fn
{
    template
        < class Range
        , class T
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, const T& default_value) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return is_single(b, e) ? *b : default_value;
    }
};

struct single_or_default_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return is_single(b, e) ? *b : cc::range_val<Range>{};
    }
};

struct single_or_eval_fn
{
    template
        < class Range
        , class Func
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::NullaryFunction<Func>>
    auto operator ()(Range&& range, Func func) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return is_single(b, e) ? *b : func();
    }
};

struct single_or_none_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const -> decltype(auto)
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return eval_optional(is_single(b, e), [&]() -> decltype(auto) { return *b; });
    }
};

struct size_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        return std::distance(std::begin(range), std::end(range));
    }
};

struct empty_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        return std::begin(range) == std::end(range);
    }
};

struct non_empty_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        return std::begin(range) != std::end(range);
    }
};

struct copy_while_fn
{
    template
        < class Range
        , class OutputIter
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::OutputIterator<OutputIter>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, OutputIter output, UnaryPred&& pred) const
    {
        return copy_while(std::begin(range), std::end(range), output, std::move(pred));
    }
};

struct copy_until_fn
{
    template
        < class Range
        , class OutputIter
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::OutputIterator<OutputIter>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, OutputIter output, UnaryPred&& pred) const
    {
        return copy_while(std::begin(range), std::end(range), output, logical_negation(pred));
    }
};

struct starts_with_fn
{
    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , CONCEPT = cc::InputRange<Range1>
        , CONCEPT = cc::InputRange<Range2>
        , CONCEPT = cc::BinaryPredicate<BinaryPred, cc::range_ref<Range1>, cc::range_ref<Range2>>>
    auto operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        auto b1 = std::begin(range1);
        auto e1 = std::end(range1);

        return std::search(b1, e1, std::begin(range2), std::end(range2), std::move(pred)) == b1;
    }
};

struct ends_with_fn
{
    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , CONCEPT = cc::InputRange<Range1>
        , CONCEPT = cc::InputRange<Range2>
        , CONCEPT = cc::BinaryPredicate<BinaryPred, cc::range_ref<Range1>, cc::range_ref<Range2>>>
    auto operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        auto b1 = std::begin(range1);
        auto e1 = std::end(range1);

        auto b2 = std::begin(range2);
        auto e2 = std::end(range2);

        auto size1 = std::distance(b1, e1);
        auto size2 = std::distance(b2, e2);

        return size1 >= size2 && std::equal(b2, e2, std::next(b1, size1 - size2), std::move(pred));
    }
};

struct contains_fn
{
    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , CONCEPT = cc::InputRange<Range1>
        , CONCEPT = cc::InputRange<Range2>
        , CONCEPT = cc::BinaryPredicate<BinaryPred, cc::range_ref<Range1>, cc::range_ref<Range2>>>
    auto operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        auto b1 = std::begin(range1);
        auto e1 = std::end(range1);

        return std::search(b1, e1, std::begin(range2), std::end(range2), std::move(pred)) != e1;
    }
};

struct sum_fn
{
    template <class Range>
    auto operator ()(Range&& range) const
    {
        using result_type = cc::Add<cc::range_val<Range>, cc::range_val<Range>>;
        return std::accumulate(std::begin(range), std::end(range), result_type{});
    }
};

} /* namespace detail */

static constexpr detail::front_fn front = {};
static constexpr detail::front_or_throw_fn front_or_throw = {};
static constexpr detail::front_or_fn front_or = {};
static constexpr detail::front_or_default_fn front_or_default = {};
static constexpr detail::front_or_eval_fn front_or_eval = {};
static constexpr detail::front_or_none_fn front_or_none = {};
static constexpr detail::single_fn single = {};
static constexpr detail::single_or_throw_fn single_or_throw = {};
static constexpr detail::single_or_fn single_or = {};
static constexpr detail::single_or_default_fn single_or_default = {};
static constexpr detail::single_or_eval_fn single_or_eval = {};
static constexpr detail::single_or_none_fn single_or_none = {};
static constexpr detail::size_fn size = {};
static constexpr detail::empty_fn empty = {};
static constexpr detail::non_empty_fn non_empty = {};
static constexpr detail::copy_while_fn copy_while = {};
static constexpr detail::copy_until_fn copy_until = {};
static constexpr detail::starts_with_fn starts_with = {};
static constexpr detail::ends_with_fn ends_with = {};
static constexpr detail::contains_fn contains = {};
static constexpr detail::sum_fn sum = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ALGORITHM_EXT_HPP_ */

