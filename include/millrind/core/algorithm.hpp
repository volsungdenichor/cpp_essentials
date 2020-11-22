#pragma once

#include <algorithm>
#include <numeric>

#include <millrind/core/macros.hpp>
#include <millrind/core/type_traits.hpp>
#include <millrind/core/detail/return_policy.hpp>

namespace millrind::core
{
namespace detail
{
struct accumulate_fn
{
    template
        < class Range
        , class T
        , class BinaryFunc = std::plus<>
        , class = InputRange<Range>
        , class = BinaryFunction<BinaryFunc, T, range_ref<Range>>>
        auto operator ()(Range&& range, T init, BinaryFunc&& func = {}) const -> T
    {
        return std::accumulate(std::begin(range), std::end(range), init, wrap_func(FORWARD(func)));
    }
};

struct adjacent_difference_fn
{
    template
        < class Range
        , class OutputIter
        , class BinaryFunc = std::minus<>
        , class = InputRange<Range>
        , class = OutputIterator<OutputIter>
        , class = BinaryFunction<BinaryFunc, range_ref<Range>, range_ref<Range>>>
        auto operator ()(Range&& range, OutputIter output, BinaryFunc&& func = {}) const -> OutputIter
    {
        return std::adjacent_difference(std::begin(range), std::end(range), output, wrap_func(FORWARD(func)));
    }
};

struct all_of_fn
{
    template
        < class Range
        , class UnaryPred
        , class = InputRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        auto operator ()(Range&& range, UnaryPred&& pred) const -> bool
    {
        return std::all_of(std::begin(range), std::end(range), wrap_func(FORWARD(pred)));
    }
};

struct any_of_fn
{
    template
        < class Range
        , class UnaryPred
        , class = InputRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        auto operator ()(Range&& range, UnaryPred&& pred) const -> bool
    {
        return std::any_of(std::begin(range), std::end(range), wrap_func(FORWARD(pred)));
    }
};

struct copy_fn
{
    template
        < class Range
        , class OutputIter
        , class = InputRange<Range>
        , class = OutputIterator<OutputIter>>
        auto operator ()(Range&& range, OutputIter output) const -> OutputIter
    {
        return std::copy(std::begin(range), std::end(range), output);
    }
};

struct copy_if_fn
{
    template
        < class Range
        , class OutputIter
        , class UnaryPred
        , class = InputRange<Range>
        , class = OutputIterator<OutputIter>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        auto operator ()(Range&& range, OutputIter output, UnaryPred&& pred) const -> OutputIter
    {
        return std::copy_if(std::begin(range), std::end(range), output, wrap_func(FORWARD(pred)));
    }
};

struct copy_n_fn
{
    template
        < class Range
        , class Size
        , class OutputIter
        , class = InputRange<Range>
        , class = Integral<Size>
        , class = OutputIterator<OutputIter>>
        auto operator ()(Range&& range, Size size, OutputIter output) const -> OutputIter
    {
        return std::copy_n(std::begin(range), size, output);
    }
};

struct count_fn
{
    template
        < class Range
        , class T
        , class = InputRange<Range>>
        auto operator ()(Range&& range, T&& value) const
    {
        return std::count(std::begin(range), std::end(range), value);
    }
};

struct count_if_fn
{
    template
        < class Range
        , class UnaryPred
        , class = InputRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return std::count_if(std::begin(range), std::end(range), wrap_func(FORWARD(pred)));
    }
};

struct equal_fn
{
    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , class = InputRange<Range1>
        , class = InputRange<Range2>
        , class = BinaryPredicate<BinaryPred, range_ref<Range1>, range_ref<Range2>>>
        auto operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const -> bool
    {
        return std::equal(std::begin(range1), std::end(range1), std::begin(range2), std::end(range2), FORWARD(pred));
    }
};

struct equal_range_fn
{
    template
        < class Range
        , class T
        , class Compare = std::less<>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        auto operator ()(Range&& range, const T& value, Compare&& compare = {}) const
    {
        return make_range(std::equal_range(std::begin(range), std::end(range), value, FORWARD(compare)));
    }
};

struct fill_fn
{
    template
        < class Range
        , class T
        , class = InputRange<Range>
        , class = Assignable<range_ref<Range>, T>>
        void operator ()(Range&& range, const T& value) const
    {
        std::fill(std::begin(range), std::end(range), value);
    }
};

struct find_fn
{
    template
        < class Policy
        , class Range
        , class T
        , class = ReturnPolicy<Policy>
        , class = InputRange<Range>
        , class = EqualityCompare<range_ref<Range>, T>>
        decltype(auto) operator ()(Policy policy, Range&& range, const T& value) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::find(b, e, value), e);
    }

    template
        < class Range
        , class T
        , class = InputRange<Range>
        , class = EqualityCompare<range_ref<Range>, T>>
        decltype(auto) operator ()(Range&& range, T&& value) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), FORWARD(value));
    }
};

struct find_if_fn
{
    template
        < class Policy
        , class Range
        , class UnaryPred
        , class = ReturnPolicy<Policy>
        , class = InputRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        decltype(auto) operator ()(Policy policy, Range&& range, UnaryPred&& pred) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::find_if(b, e, wrap_func(FORWARD(pred))), e);
    }

    template
        < class Range
        , class UnaryPred
        , class = InputRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        decltype(auto) operator ()(Range&& range, UnaryPred&& pred) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), FORWARD(pred));
    }
};

struct find_if_not_fn
{
    template
        < class Policy
        , class Range
        , class UnaryPred
        , class = ReturnPolicy<Policy>
        , class = InputRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        decltype(auto) operator ()(Policy policy, Range&& range, UnaryPred&& pred) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::find_if_not(b, e, wrap_func(FORWARD(pred))), e);
    }

    template
        < class Range
        , class UnaryPred
        , class = InputRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        decltype(auto) operator ()(Range&& range, UnaryPred&& pred) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), FORWARD(pred));
    }
};

struct find_end_fn
{
    template
        < class Policy
        , class Range1
        , class Range2
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range1>
        , class = ForwardRange<Range2>>
        decltype(auto) operator ()(Policy policy, Range1&& range1, Range2&& range2) const
    {
        auto [b, e] = make_range(range1);
        return policy(b, std::find_end(b, e, std::begin(range2), std::end(range2)), e);
    }

    template
        < class Range1
        , class Range2
        , class = ForwardRange<Range1>
        , class = ForwardRange<Range2>>
        decltype(auto) operator ()(Range1&& range1, Range2&& range2) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range1), FORWARD(range2));
    }
};

struct find_first_of_fn
{
    template
        < class Policy
        , class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , class = ReturnPolicy<Policy>
        , class = InputRange<Range1>
        , class = ForwardRange<Range2>
        , class = BinaryPredicate<BinaryPred, range_ref<Range1>, range_ref<Range2>>>
        decltype(auto) operator ()(Policy policy, Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        auto [b, e] = make_range(range1);
        return policy(b, std::find_first_of(b, e, std::begin(range2), std::end(range2), FORWARD(pred)), e);
    }

    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , class = InputRange<Range1>
        , class = ForwardRange<Range2>
        , class = BinaryPredicate<BinaryPred, range_ref<Range1>, range_ref<Range2>>>
        decltype(auto) operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range1), FORWARD(range2), FORWARD(pred));
    }
};

struct for_each_fn
{
    template
        < class Range
        , class UnaryFunc
        , class = InputRange<Range>
        , class = UnaryFunction<UnaryFunc, range_ref<Range>>>
        auto operator ()(Range&& range, UnaryFunc&& func) const
    {
        return std::for_each(std::begin(range), std::end(range), wrap_func(FORWARD(func)));
    }
};

struct generate_fn
{
    template
        < class Range
        , class Generator
        , class = ForwardRange<Range>
        , class = NullaryFunction<Generator>>
        void operator ()(Range&& range, Generator&& generator) const
    {
        std::generate(std::begin(range), std::end(range), FORWARD(generator));
    }
};

struct generate_n_fn
{
    template
        < class OutputIter
        , class Size
        , class Generator
        , class = OutputIterator<OutputIter>
        , class = Integral<Size>
        , class = NullaryFunction<Generator>>
        void operator ()(OutputIter output, Size size, Generator&& generator) const
    {
        std::generate_n(output, size, FORWARD(generator));
    }
};

struct includes_fn
{
    template
        < class Range1
        , class Range2
        , class Compare = std::less<>
        , class = InputRange<Range1>
        , class = InputRange<Range2>
        , class = BinaryPredicate<Compare, range_ref<Range1>, range_ref<Range2>>>
        auto operator ()(Range1&& range1, Range2&& range2, Compare&& compare = {}) const -> bool
    {
        return std::includes(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            FORWARD(compare));
    }
};

struct inner_product_fn
{
    template
        < class Range1
        , class Range2
        , class T
        , class BinaryFunc1 = std::plus<>
        , class BinaryFunc2 = std::multiplies<>
        , class = InputRange<Range1>
        , class = InputRange<Range2>
        , class = BinaryFunction<BinaryFunc1, T, invoke_result<BinaryFunc2, range_ref<Range1>, range_ref<Range2>>>
        , class = BinaryFunction<BinaryFunc2, range_ref<Range1>, range_ref<Range2>>>
        auto operator ()(Range1&& range1, Range2&& range2, T init, BinaryFunc1&& func1 = {}, BinaryFunc2&& func2 = {}) const -> T
    {
        return std::inner_product(std::begin(range1), std::end(range1), std::begin(range2), init, FORWARD(func1), FORWARD(func2));
    }
};

struct iota_fn
{
    template
        < class Range
        , class T
        , class = ForwardRange<Range>
        , class = Arithmetic<T>>
        void operator ()(Range&& range, T value) const
    {
        std::iota(std::begin(range), std::end(range), value);
    }
};

struct is_heap_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , class = RandomAccessRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        auto operator ()(Range&& range, Compare&& compare = {}) const -> bool
    {
        return std::is_heap(std::begin(range), std::end(range), FORWARD(compare));
    }
};

struct is_heap_until_fn
{
    template
        < class Policy
        , class Range
        , class Compare = std::less<>
        , class = ReturnPolicy<Policy>
        , class = RandomAccessRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        decltype(auto) operator ()(Policy policy, Range&& range, Compare&& compare = {}) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::is_heap_until(b, e, FORWARD(compare)), e);
    }

    template
        < class Range
        , class Compare = std::less<>
        , class = RandomAccessRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        decltype(auto) operator ()(Range&& range, Compare&& compare = {}) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), FORWARD(compare));
    }
};

struct is_partitioned_fn
{
    template
        < class Range
        , class UnaryPred
        , class = InputRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        auto operator ()(Range&& range, UnaryPred&& pred) const -> bool
    {
        return std::is_partitioned(std::begin(range), std::end(range), wrap_func(FORWARD(pred)));
    }
};

struct is_permutation_fn
{
    template
        < class Range1
        , class Range2
        , class = ForwardRange<Range1>
        , class = ForwardRange<Range2>>
        auto operator ()(Range1&& range1, Range2&& range2) const -> bool
    {
        return std::is_permutation(std::begin(range1), std::end(range1), std::begin(range2));
    }
};

struct is_sorted_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        auto operator ()(Range&& range, Compare&& compare = {}) const -> bool
    {
        return std::is_sorted(std::begin(range), std::end(range), FORWARD(compare));
    }
};

struct is_sorted_until_fn
{
    template
        < class Policy
        , class Range
        , class Compare = std::less<>
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        decltype(auto) operator ()(Policy policy, Range&& range, Compare&& compare = {}) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::is_sorted_until(b, e, FORWARD(compare)), e);
    }

    template
        < class Range
        , class Compare = std::less<>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        decltype(auto) operator ()(Range&& range, Compare&& compare = {}) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), FORWARD(compare));
    }
};

struct lexicographical_compare_fn
{
    template
        < class Range1
        , class Range2
        , class Compare = std::less<>
        , class = InputRange<Range1>
        , class = InputRange<Range2>
        , class = BinaryPredicate<Compare, range_ref<Range1>, range_ref<Range2>>>
        auto operator ()(Range1&& range1, Range2&& range2, Compare&& compare = {}) const -> bool
    {
        return std::lexicographical_compare(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            FORWARD(compare));
    }
};

struct lower_bound_fn
{
    template
        < class Policy
        , class Range
        , class T
        , class Compare = std::less<>
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>, T>>
        decltype(auto) operator ()(Policy policy, Range&& range, const T& value, Compare&& compare = {}) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::lower_bound(b, e, value, FORWARD(compare)), e);
    }

    template
        < class Range
        , class T
        , class Compare = std::less<>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>, T>>
        decltype(auto) operator ()(Range&& range, const T& value, Compare&& compare = {}) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), value, FORWARD(compare));
    }
};

struct make_heap_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , class = RandomAccessRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        void operator ()(Range&& range, Compare&& compare = {}) const
    {
        std::make_heap(std::begin(range), std::end(range), FORWARD(compare));
    }
};

struct max_element_fn
{
    template
        < class Policy
        , class Range
        , class Compare = std::less<>
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        decltype(auto) operator ()(Policy policy, Range&& range, Compare&& compare = {}) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::max_element(b, e, FORWARD(compare)), e);
    }

    template
        < class Range
        , class Compare = std::less<>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        decltype(auto) operator ()(Range&& range, Compare&& compare = {}) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), FORWARD(compare));
    }
};

struct merge_fn
{
    template
        < class Range1
        , class Range2
        , class OutputIter
        , class Compare = std::less<>
        , class = InputRange<Range1>
        , class = InputRange<Range2>
        , class = OutputIterator<OutputIter>
        , class = BinaryPredicate<Compare, range_ref<Range1>, range_ref<Range2>>>
        auto operator ()(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) const -> OutputIter
    {
        return std::merge(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            output,
            FORWARD(compare));
    }
};

struct inplace_merge_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , class = BidirectionalRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        void operator ()(Range&& range, range_iter<Range> middle, Compare&& compare = {}) const
    {
        std::inplace_merge(std::begin(range), middle, std::end(range), FORWARD(compare));
    }
};

struct minmax_element_fn
{
    template
        < class Policy
        , class Range
        , class Compare = std::less<>
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        decltype(auto) operator ()(Policy policy, Range&& range, Compare&& compare = {}) const
    {
        auto [b, e] = make_range(range);

        auto [min, max] = std::minmax_element(b, e, FORWARD(compare));

        return std::make_pair(policy(b, min, e), policy(b, max, e));
    }

    template
        < class Range
        , class Compare = std::less<>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        decltype(auto) operator ()(Range&& range, Compare&& compare = {}) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), FORWARD(compare));
    }
};

struct min_element_fn
{
    template
        < class Policy
        , class Range
        , class Compare = std::less<>
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        decltype(auto) operator ()(Policy policy, Range&& range, Compare&& compare = {}) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::min_element(b, e, FORWARD(compare)), e);
    }

    template
        < class Range
        , class Compare = std::less<>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        decltype(auto) operator ()(Range&& range, Compare&& compare = {}) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), FORWARD(compare));
    }
};

struct mismatch_fn
{
    template
        < class Policy
        , class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , class = ReturnPolicy<Policy>
        , class = InputRange<Range1>
        , class = InputRange<Range2>
        , class = BinaryPredicate<BinaryPred, range_ref<Range1>, range_ref<Range2>>>
        decltype(auto) operator ()(Policy policy, Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        auto [b1, e1] = make_range(range1);
        auto [b2, e2] = make_range(range2);

        auto [b, e] = std::mismatch(b1, e1, b2, FORWARD(pred));

        return std::make_pair(policy(b1, b, e1), policy(b2, e, e2));
    }

    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , class = InputRange<Range1>
        , class = InputRange<Range2>
        , class = BinaryPredicate<BinaryPred, range_ref<Range1>, range_ref<Range2>>>
        decltype(auto) operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range1), FORWARD(range2), FORWARD(pred));
    }
};

struct move_fn
{
    template
        < class Range
        , class OutputIter
        , class = InputRange<Range>
        , class = OutputIterator<OutputIter>>
        auto operator ()(Range&& range, OutputIter output) const -> OutputIter
    {
        return std::move(std::begin(range), std::end(range), output);
    }
};

struct next_permutation_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , class = BidirectionalRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        auto operator ()(Range&& range, Compare&& compare = {}) const -> bool
    {
        return std::next_permutation(std::begin(range), std::end(range), FORWARD(compare));
    }
};

struct none_of_fn
{
    template
        < class Range
        , class UnaryPred
        , class = InputRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        auto operator ()(Range&& range, UnaryPred&& pred) const -> bool
    {
        return std::none_of(std::begin(range), std::end(range), wrap_func(FORWARD(pred)));
    }
};

struct nth_element_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , class = RandomAccessRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        void operator ()(Range&& range, range_iter<Range> middle, Compare&& compare = {}) const
    {
        std::nth_element(std::begin(range), middle, std::end(range), FORWARD(compare));
    }
};

struct partial_sort_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , class = RandomAccessRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        void operator ()(Range&& range, range_iter<Range> middle, Compare&& compare = {}) const
    {
        std::partial_sort(std::begin(range), middle, std::end(range), FORWARD(compare));
    }
};

struct partial_sort_copy_fn
{
    template
        < class Policy
        , class Range1
        , class Range2
        , class Compare = std::less<>
        , class = ReturnPolicy<Policy>
        , class = RandomAccessRange<Range1>
        , class = RandomAccessRange<Range2>
        , class = BinaryPredicate<Compare, range_ref<Range1>, range_ref<Range2>>>
        decltype(auto) operator ()(Policy policy, Range1&& range1, Range2&& range2, Compare&& compare = {}) const
    {
        auto [b2, e2] = make_range(range2);
        return policy(b2, std::partial_sort_copy(std::begin(range1), std::end(range1), b2, e2, FORWARD(compare)), e2);
    }

    template
        < class Range1
        , class Range2
        , class Compare = std::less<>
        , class = RandomAccessRange<Range1>
        , class = RandomAccessRange<Range2>
        , class = BinaryPredicate<Compare, range_ref<Range1>, range_ref<Range2>>>
        decltype(auto) operator ()(Range1&& range1, Range2&& range2, Compare&& compare = {}) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range1), FORWARD(range2), FORWARD(compare));
    }
};

struct partial_sum_fn
{
    template
        < class Range
        , class OutputIter
        , class BinaryFunc = std::plus<>
        , class = InputRange<Range>
        , class = OutputIterator<OutputIter>
        , class = BinaryFunction<BinaryFunc, range_val<Range>, range_ref<Range>>>
        auto operator ()(Range&& range, OutputIter output, BinaryFunc&& func = {}) const -> OutputIter
    {
        return std::partial_sum(std::begin(range), std::end(range), output, FORWARD(func));
    }
};

struct partition_fn
{
    template
        < class Policy
        , class Range
        , class UnaryPred
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        decltype(auto) operator ()(Policy policy, Range&& range, UnaryPred&& pred) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::partition(b, e, wrap_func(FORWARD(pred))), e);
    }

    template
        < class Range
        , class UnaryPred
        , class = ForwardRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        decltype(auto) operator ()(Range&& range, UnaryPred&& pred) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), FORWARD(pred));
    }
};

struct partition_copy_fn
{
    template
        < class Range
        , class OutputIter1
        , class OutputIter2
        , class UnaryPred
        , class = InputRange<Range>
        , class = OutputIterator<OutputIter1>
        , class = OutputIterator<OutputIter2>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        auto operator ()(Range&& range, OutputIter1 result_true, OutputIter2 result_false, UnaryPred&& pred) const
    {
        return std::partition_copy(std::begin(range), std::end(range), result_true, result_false, wrap_func(FORWARD(pred)));
    }
};

struct stable_partition_fn
{
    template
        < class Policy
        , class Range
        , class UnaryPred
        , class = ReturnPolicy<Policy>
        , class = BidirectionalRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        decltype(auto) operator ()(Policy policy, Range&& range, UnaryPred&& pred) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::stable_partition(b, e, wrap_func(FORWARD(pred))), e);
    }

    template
        < class Range
        , class UnaryPred
        , class = BidirectionalRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        decltype(auto) operator ()(Range&& range, UnaryPred&& pred) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), FORWARD(pred));
    }
};

struct prev_permutation_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , class = BidirectionalRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        auto operator ()(Range&& range, Compare&& compare = {}) const -> bool
    {
        return std::prev_permutation(std::begin(range), std::end(range), FORWARD(compare));
    }
};

struct push_heap_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , class = RandomAccessRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        void operator ()(Range&& range, Compare&& compare = {}) const
    {
        std::push_heap(std::begin(range), std::end(range), FORWARD(compare));
    }
};

struct remove_fn
{
    template
        < class Policy
        , class Range
        , class T
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range>
        , class = EqualityCompare<range_ref<Range>, T>>
        decltype(auto) operator ()(Policy policy, Range&& range, const T& value) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::remove(b, e, value), e);
    }

    template
        < class Range
        , class T
        , class = ForwardRange<Range>
        , class = EqualityCompare<range_ref<Range>, T>>
        decltype(auto) operator ()(Range&& range, const T& value) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), value);
    }
};

struct remove_if_fn
{
    template
        < class Policy
        , class Range
        , class UnaryPred
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        decltype(auto) operator ()(Policy policy, Range&& range, UnaryPred&& pred) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::remove_if(b, e, wrap_func(FORWARD(pred))), e);
    }

    template
        < class Range
        , class UnaryPred
        , class = ForwardRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        decltype(auto) operator ()(Range&& range, UnaryPred&& pred) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), FORWARD(pred));
    }
};

struct remove_copy_fn
{
    template
        < class Range
        , class OutputIter
        , class T
        , class = InputRange<Range>
        , class = OutputIterator<OutputIter>
        , class = EqualityCompare<range_ref<Range>, T>>
        auto operator ()(Range&& range, OutputIter output, const T& value) const -> OutputIter
    {
        return std::remove_copy(std::begin(range), std::end(range), output, value);
    }
};

struct remove_copy_if_fn
{
    template
        < class Range
        , class OutputIter
        , class UnaryPred
        , class = InputRange<Range>
        , class = OutputIterator<OutputIter>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>>
        auto operator ()(Range&& range, OutputIter output, UnaryPred&& pred) const -> OutputIter
    {
        return std::remove_copy_if(std::begin(range), std::end(range), output, wrap_func(FORWARD(pred)));
    }
};

struct replace_fn
{
    template
        < class Range
        , class T
        , class = ForwardRange<Range>
        , class = EqualityCompare<range_ref<Range>, T>>
        void operator ()(Range&& range, const T& old_value, const T& new_value) const
    {
        std::replace(std::begin(range), std::end(range), old_value, new_value);
    }
};

struct replace_if_fn
{
    template
        < class Range
        , class UnaryPred
        , class T
        , class = ForwardRange<Range>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>
        , class = Assignable<range_ref<Range>, T>>
        void operator ()(Range&& range, UnaryPred&& pred, const T& new_value) const
    {
        std::replace_if(std::begin(range), std::end(range), wrap_func(FORWARD(pred)), new_value);
    }
};

struct replace_copy_fn
{
    template
        < class Range
        , class OutputIter
        , class T
        , class = InputRange<Range>
        , class = OutputIterator<OutputIter>
        , class = EqualityCompare<range_ref<Range>, T>>
        auto operator ()(Range&& range, OutputIter output, const T& old_value, const T& new_value) const -> OutputIter
    {
        return std::replace_copy(std::begin(range), std::end(range), output, old_value, new_value);
    }
};

struct replace_copy_if_fn
{
    template
        < class Range
        , class OutputIter
        , class UnaryPred
        , class T
        , class = InputRange<Range>
        , class = OutputIterator<OutputIter>
        , class = UnaryPredicate<UnaryPred, range_ref<Range>>
        , class = EqualityCompare<range_ref<Range>, T>>
        auto operator ()(Range&& range, OutputIter output, UnaryPred&& pred, const T& new_value) const -> OutputIter
    {
        return std::replace_copy_if(std::begin(range), std::end(range), output, wrap_func(FORWARD(pred)), new_value);
    }
};

struct reverse_fn
{
    template
        < class Range
        , class = BidirectionalRange<Range>>
        void operator ()(Range&& range) const
    {
        std::reverse(std::begin(range), std::end(range));
    }
};

struct reverse_copy_fn
{
    template
        < class Range
        , class OutputIter
        , class = BidirectionalRange<Range>
        , class = OutputIterator<OutputIter>>
        auto operator()(Range&& range, OutputIter output) const -> OutputIter
    {
        return std::reverse_copy(std::begin(range), std::end(range), output);
    }
};

struct rotate_fn
{
    template
        < class Policy
        , class Range
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range>>
        decltype(auto) operator ()(Policy policy, Range&& range, range_iter<Range> middle) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::rotate(b, middle, e), e);
    }

    template
        < class Range
        , class = ForwardRange<Range>>
        decltype(auto) operator ()(Range&& range, range_iter<Range> middle) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), middle);
    }
};

struct rotate_copy_fn
{
    template
        < class Range
        , class OutputIter
        , class = ForwardRange<Range>
        , class = OutputIterator<OutputIter>>
        auto operator ()(Range&& range, range_iter<Range> middle, OutputIter output) const -> OutputIter
    {
        return std::rotate_copy(std::begin(range), middle, std::end(range), output);
    }
};

struct search_fn
{
    template
        < class Policy
        , class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range1>
        , class = ForwardRange<Range2>
        , class = BinaryPredicate<BinaryPred, range_ref<Range1>, range_ref<Range2>>>
        decltype(auto) operator ()(Policy policy, Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        auto [b, e] = make_range(range1);
        return policy(b, std::search(b, e, std::begin(range2), std::end(range2), FORWARD(pred)), e);
    }

    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , class = ForwardRange<Range1>
        , class = ForwardRange<Range2>
        , class = BinaryPredicate<BinaryPred, range_ref<Range1>, range_ref<Range2>>>
        decltype(auto) operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range1), FORWARD(range2), FORWARD(pred));
    }
};

struct search_n_fn
{
    template
        < class Policy
        , class Range
        , class Size
        , class T
        , class BinaryPred = std::equal_to<>
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range>
        , class = Integral<Size>
        , class = BinaryPredicate<BinaryPred, range_ref<Range>, T>>
        decltype(auto) operator ()(Policy policy, Range&& range, Size size, const T& value, BinaryPred&& pred = {}) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::search_n(b, e, size, value, FORWARD(pred)), e);
    }

    template
        < class Range
        , class Size
        , class T
        , class BinaryPred = std::equal_to<>
        , class = ForwardRange<Range>
        , class = Integral<Size>
        , class = BinaryPredicate<BinaryPred, range_ref<Range>, T>>
        decltype(auto) operator ()(Range&& range, Size size, const T& value, BinaryPred&& pred = {}) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), size, value, FORWARD(pred));
    }
};

struct set_difference_fn
{
    template
        < class Range1
        , class Range2
        , class OutputIter
        , class Compare = std::less<>
        , class = InputRange<Range1>
        , class = InputRange<Range2>
        , class = OutputIterator<OutputIter>
        , class = BinaryPredicate<Compare, range_ref<Range1>, range_ref<Range2>>>
        auto operator ()(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) const -> OutputIter
    {
        return std::set_difference(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            output,
            FORWARD(compare));
    }
};

struct set_intersection_fn
{
    template
        < class Range1
        , class Range2
        , class OutputIter
        , class Compare = std::less<>
        , class = InputRange<Range1>
        , class = InputRange<Range2>
        , class = OutputIterator<OutputIter>
        , class = BinaryPredicate<Compare, range_ref<Range1>, range_ref<Range2>>>
        auto operator ()(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) const -> OutputIter
    {
        return std::set_intersection(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            output,
            FORWARD(compare));
    }
};

struct set_symmetric_difference_fn
{
    template
        < class Range1
        , class Range2
        , class OutputIter
        , class Compare = std::less<>
        , class = InputRange<Range1>
        , class = InputRange<Range2>
        , class = OutputIterator<OutputIter>
        , class = BinaryPredicate<Compare, range_ref<Range1>, range_ref<Range2>>>
        auto operator ()(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) const -> OutputIter
    {
        return std::set_symmetric_difference(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            output,
            FORWARD(compare));
    }
};

struct set_union_fn
{
    template
        < class Range1
        , class Range2
        , class OutputIter
        , class Compare = std::less<>
        , class = InputRange<Range1>
        , class = InputRange<Range2>
        , class = OutputIterator<OutputIter>
        , class = BinaryPredicate<Compare, range_ref<Range1>, range_ref<Range2>>>
        auto operator ()(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) const -> OutputIter
    {
        return std::set_union(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            output,
            FORWARD(compare));
    }
};

struct shuffle_fn
{
    template
        < class Range
        , class RandomNumberGenerator
        , class = RandomAccessRange<Range>
        , class = NullaryFunction<RandomNumberGenerator>>
        void operator ()(Range&& range, RandomNumberGenerator&& generator) const
    {
        std::shuffle(std::begin(range), std::end(range), FORWARD(generator));
    }
};

struct sort_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , class = RandomAccessRange<Range>
        , class = BinaryPredicate<Compare, range_ref<Range>>>
        void operator ()(Range&& range, Compare&& compare = {}) const
    {
        std::sort(std::begin(range), std::end(range), FORWARD(compare));
    }
};

struct stable_sort_fn
{
    template
        < class Range
        , class Compare = std::less<>
        , class = RandomAccessRange<Range>
        , class = BinaryFunction<Compare, range_ref<Range>>>
        void operator ()(Range&& range, Compare&& compare = {}) const
    {
        std::stable_sort(std::begin(range), std::end(range), FORWARD(compare));
    }
};

struct transform_fn
{
    template
        < class Range
        , class OutputIter
        , class UnaryFunc
        , class = InputRange<Range>
        , class = OutputIterator<OutputIter>
        , class = UnaryFunction<UnaryFunc, range_ref<Range>>>
        auto operator ()(Range&& range, OutputIter output, UnaryFunc&& func) const -> OutputIter
    {
        return std::transform(std::begin(range), std::end(range), output, wrap_func(FORWARD(func)));
    }

    template
        < class Range1
        , class Range2
        , class OutputIter
        , class BinaryFunc
        , class = InputRange<Range1>
        , class = InputRange<Range2>
        , class = OutputIterator<OutputIter>
        , class = BinaryFunction<BinaryFunc, range_ref<Range1>, range_ref<Range2>>>
        auto operator ()(Range1&& range1, Range2&& range2, OutputIter output, BinaryFunc&& func) const -> OutputIter
    {
        return std::transform(std::begin(range1), std::end(range1), std::begin(range2), output, FORWARD(func));
    }
};

struct unique_fn
{
    template
        < class Policy
        , class Range
        , class BinaryPred = std::equal_to<>
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<BinaryPred, range_ref<Range>>>
        decltype(auto) operator ()(Policy policy, Range&& range, BinaryPred&& pred = {}) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::unique(b, e, FORWARD(pred)), e);
    }

    template
        < class Range
        , class BinaryPred = std::equal_to<>
        , class = ForwardRange<Range>
        , class = BinaryPredicate<BinaryPred, range_ref<Range>>>
        decltype(auto) operator ()(Range&& range, BinaryPred&& pred = {}) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), FORWARD(pred));
    }
};

struct unique_copy_fn
{
    template
        < class Range
        , class OutputIter
        , class BinaryPred = std::equal_to<>
        , class = InputRange<Range>
        , class = OutputIterator<OutputIter>
        , class = BinaryPredicate<BinaryPred, range_ref<Range>>>
        auto operator ()(Range&& range, OutputIter output, BinaryPred&& pred = {}) const -> OutputIter
    {
        return std::unique_copy(std::begin(range), std::end(range), output, FORWARD(pred));
    }
};

struct upper_bound_fn
{
    template
        < class Policy
        , class Range
        , class T
        , class Compare = std::less<>
        , class = ReturnPolicy<Policy>
        , class = ForwardRange<Range>
        , class = BinaryFunction<Compare, range_ref<Range>, T>>
        decltype(auto) operator ()(Policy policy, Range&& range, const T& value, Compare&& compare = {}) const
    {
        auto [b, e] = make_range(range);
        return policy(b, std::upper_bound(b, e, value, FORWARD(compare)), e);
    }

    template
        < class Range
        , class T
        , class Compare = std::less<>
        , class = ForwardRange<Range>
        , class = BinaryFunction<Compare, range_ref<Range>, T>>
        decltype(auto) operator ()(Range&& range, const T& value, Compare&& compare = {}) const
    {
        return (*this)(default_return_policy_t{}, FORWARD(range), value, FORWARD(compare));
    }
};

} /* namespace detail */

static constexpr auto accumulate = detail::accumulate_fn{};
static constexpr auto adjacent_difference = detail::adjacent_difference_fn{};
static constexpr auto all_of = detail::all_of_fn{};
static constexpr auto any_of = detail::any_of_fn{};
static constexpr auto copy = detail::copy_fn{};
static constexpr auto copy_if = detail::copy_if_fn{};
static constexpr auto copy_n = detail::copy_n_fn{};
static constexpr auto count = detail::count_fn{};
static constexpr auto count_if = detail::count_if_fn{};
static constexpr auto equal = detail::equal_fn{};
static constexpr auto equal_range = detail::equal_range_fn{};
static constexpr auto fill = detail::fill_fn{};
static constexpr auto find = detail::find_fn{};
static constexpr auto find_if = detail::find_if_fn{};
static constexpr auto find_if_not = detail::find_if_not_fn{};
static constexpr auto find_end = detail::find_end_fn{};
static constexpr auto find_first_of = detail::find_first_of_fn{};
static constexpr auto for_each = detail::for_each_fn{};
static constexpr auto generate = detail::generate_fn{};
static constexpr auto generate_n = detail::generate_n_fn{};
static constexpr auto includes = detail::includes_fn{};
static constexpr auto inner_product = detail::inner_product_fn{};
static constexpr auto iota = detail::iota_fn{};
static constexpr auto is_heap = detail::is_heap_fn{};
static constexpr auto is_heap_until = detail::is_heap_until_fn{};
static constexpr auto is_partitioned = detail::is_partitioned_fn{};
static constexpr auto is_permutation = detail::is_permutation_fn{};
static constexpr auto is_sorted = detail::is_sorted_fn{};
static constexpr auto is_sorted_until = detail::is_sorted_until_fn{};
static constexpr auto lexicographical_compare = detail::lexicographical_compare_fn{};
static constexpr auto lower_bound = detail::lower_bound_fn{};
static constexpr auto make_heap = detail::make_heap_fn{};
static constexpr auto max_element = detail::max_element_fn{};
static constexpr auto merge = detail::merge_fn{};
static constexpr auto inplace_merge = detail::inplace_merge_fn{};
static constexpr auto minmax_element = detail::minmax_element_fn{};
static constexpr auto min_element = detail::min_element_fn{};
static constexpr auto mismatch = detail::mismatch_fn{};
static constexpr auto move = detail::move_fn{};
static constexpr auto next_permutation = detail::next_permutation_fn{};
static constexpr auto none_of = detail::none_of_fn{};
static constexpr auto nth_element = detail::nth_element_fn{};
static constexpr auto partial_sort = detail::partial_sort_fn{};
static constexpr auto partial_sort_copy = detail::partial_sort_copy_fn{};
static constexpr auto partial_sum = detail::partial_sum_fn{};
static constexpr auto partition = detail::partition_fn{};
static constexpr auto partition_copy = detail::partition_copy_fn{};
static constexpr auto stable_partition = detail::stable_partition_fn{};
static constexpr auto prev_permutation = detail::prev_permutation_fn{};
static constexpr auto push_heap = detail::push_heap_fn{};
static constexpr auto remove = detail::remove_fn{};
static constexpr auto remove_if = detail::remove_if_fn{};
static constexpr auto remove_copy = detail::remove_copy_fn{};
static constexpr auto remove_copy_if = detail::remove_copy_if_fn{};
static constexpr auto replace = detail::replace_fn{};
static constexpr auto replace_if = detail::replace_if_fn{};
static constexpr auto replace_copy = detail::replace_copy_fn{};
static constexpr auto replace_copy_if = detail::replace_copy_if_fn{};
static constexpr auto reverse = detail::reverse_fn{};
static constexpr auto reverse_copy = detail::reverse_copy_fn{};
static constexpr auto rotate = detail::rotate_fn{};
static constexpr auto rotate_copy = detail::rotate_copy_fn{};
static constexpr auto search = detail::search_fn{};
static constexpr auto search_n = detail::search_n_fn{};
static constexpr auto set_difference = detail::set_difference_fn{};
static constexpr auto set_intersection = detail::set_intersection_fn{};
static constexpr auto set_symmetric_difference = detail::set_symmetric_difference_fn{};
static constexpr auto set_union = detail::set_union_fn{};
static constexpr auto shuffle = detail::shuffle_fn{};
static constexpr auto sort = detail::sort_fn{};
static constexpr auto stable_sort = detail::stable_sort_fn{};
static constexpr auto transform = detail::transform_fn{};
static constexpr auto unique = detail::unique_fn{};
static constexpr auto unique_copy = detail::unique_copy_fn{};
static constexpr auto upper_bound = detail::upper_bound_fn{};

} // namespace millrind::core


