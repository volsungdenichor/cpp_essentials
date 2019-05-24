#ifndef CPP_ESSENTIALS_CORE_ALGORITHM_EXT_HPP_
#define CPP_ESSENTIALS_CORE_ALGORITHM_EXT_HPP_

#pragma once

#include <algorithm>
#include <numeric>

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/return_policy.hpp>
#include <cpp_essentials/core/functors.hpp>

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

template <class Iter1, class Iter2>
Iter1 overwrite(Iter1 s_begin, Iter1 s_end, Iter2 d_begin, Iter2 d_end)
{
    for (; s_begin != s_end && d_begin != d_end; ++s_begin, ++d_begin)
    {
        *d_begin = *s_begin;
    }
    return s_begin;
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
        return copy_while(std::begin(range), std::end(range), output, make_func(pred));
    }
};

struct overwrite_fn
{
    template
        < class Range
        , class Dest
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::Range<Dest>>
    auto operator ()(Range&& range, Dest&& dest) const
    {
        return overwrite(std::begin(range), std::end(range), std::begin(dest), std::end(dest));
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
        return copy_while(std::begin(range), std::end(range), output, logical_negation(make_func(pred)));
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
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        using result_type = cc::Add<cc::range_val<Range>, cc::range_val<Range>>;
        return std::accumulate(std::begin(range), std::end(range), result_type{});
    }
};

template <class Iter, class Compare>
auto extreme_value(Iter b, Iter e, Compare&& compare)
{
    auto result = *b;

    for (; b != e; ++b)
    {
        if (compare(*b, result))
        {
            result = *b;
        }
    }

    return result;
}

template <class Iter, class Compare>
auto extreme_values(Iter b, Iter e, Compare&& compare)
{
    auto min = *b;
    auto max = *b;

    for (; b != e; ++b)
    {
        if (compare(*b, min))
        {
            min = *b;
        }
        if (!compare(*b, max))
        {
            max = *b;
        }
    }

    return std::make_pair(min, max);
}

struct min_value_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::BinaryPredicate<Compare, cc::range_ref<Range>, cc::range_ref<Range>>>
    auto operator ()(Range&& range, Compare compare = {}) const
    {
        return extreme_value(std::begin(range), std::end(range), compare);
    }
};

struct max_value_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::BinaryPredicate<Compare, cc::range_ref<Range>, cc::range_ref<Range>>>
    auto operator ()(Range&& range, Compare compare = {}) const
    {
        return extreme_value(std::begin(range), std::end(range), logical_negation(compare));
    }
};

struct minmax_value_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::BinaryPredicate<Compare, cc::range_ref<Range>, cc::range_ref<Range>>>
    auto operator ()(Range&& range, Compare compare = {}) const
    {
        return extreme_values(std::begin(range), std::end(range), compare);
    }
};

} /* namespace detail */

static constexpr auto front = detail::front_fn{};
static constexpr auto front_or_throw = detail::front_or_throw_fn{};
static constexpr auto front_or = detail::front_or_fn{};
static constexpr auto front_or_default = detail::front_or_default_fn{};
static constexpr auto front_or_eval = detail::front_or_eval_fn{};
static constexpr auto front_or_none = detail::front_or_none_fn{};
static constexpr auto single = detail::single_fn{};
static constexpr auto single_or_throw = detail::single_or_throw_fn{};
static constexpr auto single_or = detail::single_or_fn{};
static constexpr auto single_or_default = detail::single_or_default_fn{};
static constexpr auto single_or_eval = detail::single_or_eval_fn{};
static constexpr auto single_or_none = detail::single_or_none_fn{};
static constexpr auto size = detail::size_fn{};
static constexpr auto empty = detail::empty_fn{};
static constexpr auto non_empty = detail::non_empty_fn{};
static constexpr auto copy_while = detail::copy_while_fn{};
static constexpr auto copy_until = detail::copy_until_fn{};
static constexpr auto overwrite = detail::overwrite_fn{};
static constexpr auto starts_with = detail::starts_with_fn{};
static constexpr auto ends_with = detail::ends_with_fn{};
static constexpr auto contains = detail::contains_fn{};
static constexpr auto sum = detail::sum_fn{};
static constexpr auto min_value = detail::min_value_fn{};
static constexpr auto max_value = detail::max_value_fn{};
static constexpr auto minmax_value = detail::minmax_value_fn{};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ALGORITHM_EXT_HPP_ */

