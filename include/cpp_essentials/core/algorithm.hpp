#ifndef CPP_ESSENTIALS_CORE_ALGORITHM_HPP_
#define CPP_ESSENTIALS_CORE_ALGORITHM_HPP_

#pragma once

#include <algorithm>
#include <numeric>

#include <cpp_essentials/concepts/concepts.hpp>
#include <cpp_essentials/core/return_policy.hpp>

namespace cpp_essentials::core
{

template
    < class Range
    , class T
    , class BinaryFunc = std::plus<>
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto accumulate(Range&& range, T init, BinaryFunc&& func = {}) -> T
{
    return std::accumulate(std::begin(range), std::end(range), init, std::move(func));
}

template
    < class Range
    , class OutputIter
    , class BinaryFunc = std::minus<>
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto adjacent_difference(Range&& range, OutputIter output, BinaryFunc&& func = {}) -> OutputIter
{
    return std::adjacent_difference(std::begin(range), std::end(range), output, std::move(func));
}

template
    < class Range
    , class UnaryPred
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto all_of(Range&& range, UnaryPred&& pred) -> bool
{
    return std::all_of(std::begin(range), std::end(range), std::move(pred));
}

template
    < class Range
    , class UnaryPred
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto any_of(Range&& range, UnaryPred&& pred) -> bool
{
    return std::any_of(std::begin(range), std::end(range), std::move(pred));
}

template
    < class Range1
    , class Range2
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)>
auto contains(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) -> bool
{
    auto b1 = std::begin(range1);
    auto e1 = std::end(range1);

    return std::search(b1, e1, std::begin(range2), std::end(range2), std::move(pred)) != e1;
}

template
    < class Range
    , class OutputIter
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto copy(Range&& range, OutputIter output) -> OutputIter
{
    return std::copy(std::begin(range), std::end(range), output);
}

template
    < class Range
    , class OutputIter
    , class UnaryPred
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto copy_if(Range&& range, OutputIter output, UnaryPred&& pred) -> OutputIter
{
    return std::copy_if(std::begin(range), std::end(range), output, std::move(pred));
}

template
    < class Range
    , class Size
    , class OutputIter
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto copy_n(Range&& range, Size size, OutputIter output) -> OutputIter
{
    return std::copy_n(std::begin(range), size, output);
}

template
    < class Range
    , class OutputIter
    , class UnaryPred
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto copy_while(Range&& range, OutputIter output, UnaryPred&& pred) -> OutputIter
{
    auto end = std::end(range);
    for (auto it = std::begin(range); it != end && pred(*it); ++it)
    {
        *output = *it;
        ++output;
    }
    return output;
}

template
    < class Range
    , class OutputIter
    , class UnaryPred
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto copy_until(Range&& range, OutputIter output, UnaryPred&& pred) -> OutputIter
{
    auto end = std::end(range);
    for (auto it = std::begin(range); it != end && !pred(*it); ++it)
    {
        *output = *it;
        ++output;
    }
    return output;
}

template
    < class Range
    , class T
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto count(Range&& range, const T& value)
{
    return std::count(std::begin(range), std::end(range), value);
}

template
    < class Range
    , class UnaryPred
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto count_if(Range&& range, UnaryPred&& pred)
{
    return std::count_if(std::begin(range), std::end(range), std::move(pred));
}

template
    < class Range1
    , class Range2
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)>
auto ends_with(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) -> bool
{
    auto b1 = std::begin(range1);
    auto e1 = std::end(range1);

    auto b2 = std::begin(range2);
    auto e2 = std::end(range2);

    auto size1 = std::distance(b1, e1);
    auto size2 = std::distance(b2, e2);

    return size1 >= size2 && std::equal(b2, e2, std::next(b1, size1 - size2), std::move(pred));
}

template
    < class Range1
    , class Range2
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)>
auto equal(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) -> bool
{
    return std::equal(std::begin(range1), std::end(range1), std::begin(range2), std::end(range2), std::move(pred));
}

template
    < class Range
    , class T
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
auto equal_range(Range&& range, const T& value, Compare&& compare = {})
{
    return make_range(std::equal_range(std::begin(range), std::end(range), value, std::move(compare)));
}

template
    < class Range
    , class T
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
void fill(Range&& range, const T& value)
{
    std::fill(std::begin(range), std::end(range), value);
}

template
    < class Policy
    , class Range
    , class T
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::InputRange<Range>)>
decltype(auto) find(Policy policy, Range&& range, const T& value)
{
    auto b = std::begin(range);
    auto e = std::end(range);
    return policy(b, std::find(b, e, value), e);
}

template   
    < class Range
    , class T
    , CONCEPT_IF(concepts::InputRange<Range>)>
decltype(auto) find(Range&& range, const T& value)
{
    return find(return_default, range, value);
}

template
    < class Policy
    , class Range
    , class UnaryPred
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::InputRange<Range>)>
decltype(auto) find_if(Policy policy, Range&& range, UnaryPred&& pred)
{
    auto b = std::begin(range);
    auto e = std::end(range);
    return policy(b, std::find_if(b, e, std::move(pred)), e);
}

template
    < class Range
    , class UnaryPred
    , CONCEPT_IF(concepts::InputRange<Range>)>
decltype(auto) find_if(Range&& range, UnaryPred&& pred)
{
    return find_if(return_default, range, std::move(pred));
}

template
    < class Policy
    , class Range
    , class UnaryPred
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::InputRange<Range>)>
decltype(auto) find_if_not(Policy policy, Range&& range, UnaryPred&& pred)
{
    auto b = std::begin(range);
    auto e = std::end(range);
    return policy(b, std::find_if_not(b, e, std::move(pred)), e);
}

template
    < class Range
    , class UnaryPred   
    , CONCEPT_IF(concepts::InputRange<Range>)>
decltype(auto) find_if_not(Range&& range, UnaryPred&& pred)
{
    return find_if_not(return_default, range, std::move(pred));
}

template
    < class Policy
    , class Range1
    , class Range2
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range1>)
    , CONCEPT_IF(concepts::ForwardRange<Range2>)>
decltype(auto) find_end(Policy policy, Range1&& range1, Range2&& range2)
{
    auto b = std::begin(range1);
    auto e = std::end(range1);
    return policy(b, std::find_end(b, e, std::begin(range2), std::end(range2)), e);
}

template
    < class Range1
    , class Range2
    , CONCEPT_IF(concepts::ForwardRange<Range1>)
    , CONCEPT_IF(concepts::ForwardRange<Range2>)>
decltype(auto) find_end(Range1&& range1, Range2&& range2)
{
    return find_end(return_default, range1, range2);
}

template
    < class Policy
    , class Range1
    , class Range2
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::ForwardRange<Range2>)>
decltype(auto) find_first_of(Policy policy, Range1&& range1, Range2&& range2, BinaryPred&& pred = {})
{
    auto b = std::begin(range1);
    auto e = std::end(range1);
    return policy(b, std::find_first_of(b, e, std::begin(range2), std::end(range2), std::move(pred)), e);
}

template
    < class Range1
    , class Range2
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::ForwardRange<Range2>)>
decltype(auto) find_first_of(Range1&& range1, Range2&& range2, BinaryPred&& pred = {})
{
    return find_first_of(return_default, range1, range2, std::move(pred));
}

template
    < class Range
    , class UnaryFunc
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto for_each(Range&& range, UnaryFunc&& func)
{
    return std::for_each(std::begin(range), std::end(range), std::move(func));
}



template
    < class Range
    , class Generator
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
void generate(Range&& range, Generator&& generator)
{
    std::generate(std::begin(range), std::end(range), std::move(generator));
}



template
    < class OutputIter
    , class Size
    , class Generator
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
void generate_n(OutputIter output, Size size, Generator&& generator)
{
    std::generate_n(output, size, std::move(generator));
}

template
    < class Range1
    , class Range2
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)>
auto includes(Range1&& range1, Range2&& range2, Compare&& compare = {}) -> bool
{
    return std::includes(
        std::begin(range1),
        std::end(range1),
        std::begin(range2),
        std::end(range2),
        std::move(compare));
}

template
    < class Range1
    , class Range2
    , class T
    , class BinaryFunc1 = std::plus<>
    , class BinaryFunc2 = std::multiplies<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)>
auto inner_product(Range1&& range1, Range2&& range2, T init, BinaryFunc1&& func1 = {}, BinaryFunc2&& func2 = {}) -> T
{
    return std::inner_product(std::begin(range1), std::end(range1), std::begin(range2), init, std::move(func1), std::move(func2));
}



template
    < class Range
    , class T
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
void iota(Range&& range, T value)
{
    std::iota(std::begin(range), std::end(range), value);
}

template
    < class Range1
    , class Range2
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)>
auto is_equal(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) -> bool
{
    auto b1 = std::begin(range1);
    auto e1 = std::end(range1);

    auto b2 = std::begin(range2);
    auto e2 = std::end(range2);

    auto size1 = std::distance(b1, e1);
    auto size2 = std::distance(b2, e2);

    return size1 == size2 && std::equal(b1, e1, b2, std::move(pred));
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
auto is_heap(Range&& range, Compare&& compare = {}) -> bool
{
    return std::is_heap(std::begin(range), std::end(range), std::move(compare));
}

template
    < class Policy
    , class Range
    , class Compare = std::less<>
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
decltype(auto) is_heap_until(Policy policy, Range&& range, Compare&& compare = {})
{
    auto b = std::begin(range);
    auto e = std::begin(range);
    return policy(b, std::is_heap_until(b, e, std::move(compare)), e);
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
decltype(auto) is_heap_until(Range&& range, Compare&& compare = {})
{
    return is_heap_until(return_default, range, std::move(compare));
}



template
    < class Range
    , class UnaryFunc
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto is_partitioned(Range&& range, UnaryFunc&& func) -> bool
{
    return std::is_partitioned(std::begin(range), std::end(range), std::move(func));
}

template
    < class Range1
    , class Range2
    , CONCEPT_IF(concepts::ForwardRange<Range1>)
    , CONCEPT_IF(concepts::ForwardRange<Range2>)>
auto is_permutation(Range1&& range1, Range2&& range2) -> bool
{
    return std::is_permutation(std::begin(range1), std::end(range1), std::begin(range2));
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
auto is_sorted(Range&& range, Compare&& compare = {}) -> bool
{
    return std::is_sorted(std::begin(range), std::end(range), std::move(compare));
}

template
    < class Policy
    , class Range
    , class Compare = std::less<>
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) is_sorted_until(Policy policy, Range&& range, Compare&& compare = {})
{
    auto b = std::begin(range);
    auto e = std::end(range);
    return policy(b, std::is_sorted_until(b, e, std::move(compare)), e);
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) is_sorted_until(Range&& range, Compare&& compare = {})
{
    return is_sorted_until(return_default, range, std::move(compare));
}

template
    < class Range1
    , class Range2
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)>
auto lexicographical_compare(Range1&& range1, Range2&& range2, Compare&& compare = {}) -> bool
{
    return std::lexicographical_compare(
        std::begin(range1),
        std::end(range1),
        std::begin(range2),
        std::end(range2),
        std::move(compare));
}

template
    < class Policy
    , class Range
    , class T
    , class Compare = std::plus<>
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) lower_bound(Policy policy, Range&& range, const T& value, Compare&& compare = {})
{
    auto b = std::begin(range);
    auto e = std::begin(range);
    return policy(b, std::lower_bound(b, e, value, std::move(compare)), e);
}

template
    < class Range
    , class T
    , class Compare = std::plus<>
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) lower_bound(Range&& range, const T& value, Compare&& compare = {})
{
    return lower_bound(return_default, range, value, std::move(compare));
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
void make_heap(Range&& range, Compare&& compare = {})
{
    std::make_heap(std::begin(range), std::end(range), std::move(compare));
}

template
    < class Policy
    , class Range
    , class Compare = std::less<>
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) max_element(Policy policy, Range&& range, Compare&& compare = {})
{
    auto b = std::begin(range);
    auto e = std::end(range);
    return policy(b, std::max_element(b, e, std::move(compare)), e);
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) max_element(Range&& range, Compare&& compare = {})
{
    return max_element(return_default, range, std::move(compare));
}

template
    < class Range1
    , class Range2
    , class OutputIter
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto merge(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) -> OutputIter
{
    return std::merge(
        std::begin(range1),
        std::end(range1),
        std::begin(range2),
        std::end(range2),
        output,
        std::move(compare));
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::BidirectionalRange<Range>)>
void inplace_merge(Range&& range, concepts::range_iterator<Range> middle, Compare&& compare = {})
{
    std::inplace_merge(std::begin(range), middle, std::end(range), std::move(compare));
}

template
    < class Policy
    , class Range
    , class Compare = std::less<>
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) minmax_element(Policy policy, Range&& range, Compare&& compare = {})
{
    auto b = std::begin(range);
    auto e = std::end(range);

    auto result = std::minmax_element(b, e, std::move(compare));

    return std::make_pair(policy(b, std::get<0>(result), e), policy(b, std::get<1>(result), e));
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) minmax_element(Range&& range, Compare&& compare = {})
{
    return minmax_element(return_default, range, std::move(compare));
}

template
    < class Policy
    , class Range
    , class Compare = std::less<>
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) min_element(Policy policy, Range&& range, Compare&& compare = {})
{
    auto b = std::begin(range);
    auto e = std::end(range);
    return policy(b, std::min_element(b, e, std::move(compare)), e);
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) min_element(Range&& range, Compare&& compare = {})
{
    return min_element(return_default, range, std::move(compare));
}

template
    < class Policy
    , class Range1
    , class Range2
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)>
auto mismatch(Policy policy, Range1&& range1, Range2&& range2, BinaryPred&& pred = {})
{
    auto b1 = std::begin(range1);
    auto e1 = std::end(range1);

    auto b2 = std::begin(range2);
    auto e2 = std::end(range2);

    auto result = std::mismatch(b1, e1, b2, std::move(pred));

    return std::make_pair(policy(b1, std::get<0>(result), e1), policy(b2, std::get<1>(result), e2));
}

template
    < class Range1
    , class Range2
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)>
auto mismatch(Range1&& range1, Range2&& range2, BinaryPred&& pred = {})
{
    return mismatch(return_default, range1, range2, std::move(pred));
}

template
    < class Range
    , class OutputIter
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto move(Range&& range, OutputIter output) -> OutputIter
{
    return std::move(std::begin(range), std::end(range), output);
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::BidirectionalRange<Range>)>
auto next_permutation(Range&& range, Compare&& compare = {}) -> bool
{
    return std::next_permutation(std::begin(range), std::end(range), std::move(compare));
}

template
    < class Range
    , class UnaryPred
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto none_of(Range&& range, UnaryPred&& pred) -> bool
{
    return std::none_of(std::begin(range), std::end(range), std::move(pred));
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
void nth_element(Range&& range, concepts::range_iterator<Range> middle, Compare&& compare = {})
{
    std::nth_element(std::begin(range), middle, std::end(range), std::move(compare));
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
void partial_sort(Range&& range, concepts::range_iterator<Range> middle, Compare&& compare = {})
{
    std::partial_sort(std::begin(range), middle, std::end(range), std::move(compare));
}

template
    < class Policy
    , class Range1
    , class Range2
    , class Compare = std::less<>
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::RandomAccessRange<Range1>)
    , CONCEPT_IF(concepts::RandomAccessRange<Range2>)>
decltype(auto) partial_sort_copy(Policy policy, Range1&& range1, Range2&& range2, Compare&& compare = {})
{
    auto b2 = std::begin(range2);
    auto e2 = std::end(range2);
    return policy(b2, std::partial_sort_copy(std::begin(range1), std::end(range1), b2, e2, std::move(compare)), e2);
}

template
    < class Range1
    , class Range2
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::RandomAccessRange<Range1>)
    , CONCEPT_IF(concepts::RandomAccessRange<Range2>)>
decltype(auto) partial_sort_copy(Range1&& range1, Range2&& range2, Compare&& compare = {})
{
    return partial_sort_copy(return_default, range1, range2, std::move(compare));
}

template
    < class Range
    , class OutputIter
    , class BinaryFunc = std::plus<>
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto partial_sum(Range&& range, OutputIter output, BinaryFunc&& func = {}) -> OutputIter
{
    return std::partial_sum(std::begin(range), std::end(range), output, std::move(func));
}

template
    < class Policy
    , class Range
    , class UnaryPred
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) partition(Policy policy, Range&& range, UnaryPred&& pred)
{
    auto b = std::begin(range);
    auto e = std::end(range);
    return policy(b, std::partition(b, e, std::move(pred)), e);
}

template
    < class Range
    , class UnaryPred
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) partition(Range&& range, UnaryPred&& pred)
{
    return partition(return_default, range, std::move(pred));
}

template
    < class Range
    , class OutputIter1
    , class OutputIter2
    , class UnaryPred
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter1>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter2>)>
auto partition_copy(Range&& range, OutputIter1 result_true, OutputIter2 result_false, UnaryPred&& pred)
{
    return std::partition_copy(std::begin(range), std::end(range), result_true, result_false, std::move(pred));
}

template
    < class Policy
    , class Range
    , class UnaryPred
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::BidirectionalRange<Range>)>
decltype(auto) stable_partition(Policy policy, Range&& range, UnaryPred&& pred)
{
    auto b = std::begin(range);
    auto e = std::end(range);
    return policy(b, std::stable_partition(b, e, std::move(pred)), e);
}

template
    < class Range
    , class UnaryPred
    , CONCEPT_IF(concepts::BidirectionalRange<Range>)>
decltype(auto) stable_partition(Range&& range, UnaryPred&& pred)
{
    return stable_partition(return_default, range, std::move(pred));
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::BidirectionalRange<Range>)>
auto prev_permutation(Range&& range, Compare&& compare = {}) -> bool
{
    return std::prev_permutation(std::begin(range), std::end(range), std::move(compare));
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
void push_heap(Range&& range, Compare&& compare = {})
{
    std::push_heap(std::begin(range), std::end(range), std::move(compare));
}

template
    < class Range
    , class T
    , class BinaryFunc = std::plus<>
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto reduce(Range&& range, T init, BinaryFunc&& func = {})
{
    return std::accumulate(std::begin(range), std::end(range), init, std::move(func));
}

template
    < class Range
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto reduce(Range&& range)
{
    return reduce(range, concepts::range_value<Range>{});
}

template
    < class Policy
    , class Range
    , class T
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) remove(Policy policy, Range&& range, const T& value)
{
    auto b = std::begin(range);
    auto e = std::end(range);
    return policy(b, std::remove(b, e, value), e);
}

template
    < class Range
    , class T
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) remove(Range&& range, const T& value)
{
    return remove(return_default, range, value);
}

template
    < class Policy
    , class Range
    , class UnaryPred
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) remove_if(Policy policy, Range&& range, UnaryPred&& pred)
{
    auto b = std::begin(range);
    auto e = std::end(range);
    return policy(b, std::remove_if(b, e, std::move(pred)), e);
}

template
    < class Range
    , class UnaryPred    
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) remove_if(Range&& range, UnaryPred&& pred)
{
    return remove_if(return_default, range, std::move(pred));
}

template
    < class Range
    , class OutputIter
    , class T
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto remove_copy(Range&& range, OutputIter output, const T& value) -> OutputIter
{
    return std::remove_copy(std::begin(range), std::end(range), output, value);
}

template
    < class Range
    , class OutputIter
    , class UnaryPred
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto remove_copy_if(Range&& range, OutputIter output, UnaryPred&& pred) -> OutputIter
{
    return std::remove_copy_if(std::begin(range), std::end(range), output, std::move(pred));
}

template
    < class Range
    , class T
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
void replace(Range&& range, const T& old_value, const T& new_value)
{
    std::replace(std::begin(range), std::end(range), old_value, new_value);
}

template
    < class Range
    , class UnaryPred
    , class T
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
void replace_if(Range&& range, UnaryPred pred, const T& new_value)
{
    std::replace_if(std::begin(range), std::end(range), pred, new_value);
}

template
    < class Range
    , class OutputIter
    , class T
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto replace_copy(Range&& range, OutputIter output, const T& old_value, const T& new_value) -> OutputIter
{
    return std::replace_copy(std::begin(range), std::end(range), output, old_value, new_value);
}

template
    < class Range
    , class OutputIter
    , class UnaryPred
    , class T
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto replace_copy_if(Range&& range, OutputIter output, UnaryPred&& pred, const T& new_value) -> OutputIter
{
    return std::replace_copy_if(std::begin(range), std::end(range), output, std::move(pred), new_value);
}

template
    < class Range
    , CONCEPT_IF(concepts::BidirectionalRange<Range>)>
void reverse(Range&& range)
{
    std::reverse(std::begin(range), std::end(range));
}

template
    < class Range
    , class OutputIter
    , CONCEPT_IF(concepts::BidirectionalRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto reverse_copy(Range&& range, OutputIter output) -> OutputIter
{
    return std::reverse_copy(std::begin(range), std::end(range), output);
}

template
    < class Policy
    , class Range
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) rotate(Policy policy, Range&& range, concepts::range_iterator<Range> middle)
{
    auto b = std::begin(range);
    auto e = std::end(range);
    return policy(b, std::rotate(b, middle, e), e);
}

template
    < class Range
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) rotate(Range&& range, concepts::range_iterator<Range> middle)
{
    return rotate(return_default, range, middle);
}

template
    < class Range
    , class OutputIter
    , CONCEPT_IF(concepts::ForwardRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto rotate_copy(Range&& range, concepts::range_iterator<Range> middle, OutputIter output) -> OutputIter
{
    return std::rotate_copy(std::begin(range), middle, std::end(range), output);
}

template
    < class Policy
    , class Range1
    , class Range2
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range1>)
    , CONCEPT_IF(concepts::ForwardRange<Range2>)>
decltype(auto) search(Policy policy, Range1&& range1, Range2&& range2, BinaryPred&& pred = {})
{    
    auto b = std::begin(range1);
    auto e = std::end(range1);
    return policy(b, std::search(b, e, std::begin(range2), std::end(range2), std::move(pred)), e);
}

template
    < class Range1
    , class Range2
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(concepts::ForwardRange<Range1>)
    , CONCEPT_IF(concepts::ForwardRange<Range2>)>
decltype(auto) search(Range1&& range1, Range2&& range2, BinaryPred&& pred = {})
{
    return search(return_default, range1, range2, std::move(pred));
}

template
    < class Policy
    , class Range
    , class Size
    , class T
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) search_n(Policy policy, Range&& range, Size size, const T& value, BinaryPred&& pred = {})
{
    auto b = std::begin(range);
    auto e = std::end(range);
    return policy(b, std::search_n(b, e, size, value, std::move(pred)), e);
}

template
    < class Range
    , class Size
    , class T
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) search_n(Range&& range, Size size, const T& value, BinaryPred&& pred = {})
{
    return search_n(return_default, range, size, value, std::move(pred));
}

template
    < class Range1
    , class Range2
    , class OutputIter
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto set_difference(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) -> OutputIter
{
    return std::set_difference(
        std::begin(range1),
        std::end(range1),
        std::begin(range2),
        std::end(range2),
        output,
        std::move(compare));
}

template
    < class Range1
    , class Range2
    , class OutputIter
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto set_intersection(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) -> OutputIter
{
    return std::set_intersection(
        std::begin(range1),
        std::end(range1),
        std::begin(range2),
        std::end(range2),
        output,
        std::move(compare));
}

template
    < class Range1
    , class Range2
    , class OutputIter
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto set_symmetric_difference(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) -> OutputIter
{
    return std::set_symmetric_difference(
        std::begin(range1),
        std::end(range1),
        std::begin(range2),
        std::end(range2),
        output,
        std::move(compare));
}

template
    < class Range1
    , class Range2
    , class OutputIter
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto set_union(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) -> OutputIter
{
    return std::set_union(
        std::begin(range1),
        std::end(range1),
        std::begin(range2),
        std::end(range2),
        output,
        std::move(compare));
}

template
    < class Range
    , class RandomNumberGenerator
    , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
void shuffle(Range&& range, RandomNumberGenerator&& generator)
{
    std::shuffle(std::begin(range), std::end(range), std::move(generator));
}

template
    < class Range
    , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
void random_shuffle(Range&& range)
{
    std::random_shuffle(std::begin(range), std::end(range));
}

template
    < class Range
    , class RandomNumberGenerator
    , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
void random_shuffle(Range&& range, RandomNumberGenerator&& generator)
{
    std::random_shuffle(std::begin(range), std::end(range), std::move(generator));
}

template
    < typename Range
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto size(Range&& range) -> size_t
{
    return static_cast<size_t>(std::distance(std::begin(range), std::end(range)));
}

template
    < typename Range
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto empty(Range&& range) -> bool
{
    return std::begin(range) == std::end(range);
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
void sort(Range&& range, Compare&& compare = {})
{
    std::sort(std::begin(range), std::end(range), std::move(compare));
}

template
    < class Range
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
void stable_sort(Range&& range, Compare&& compare = {})
{
    std::stable_sort(std::begin(range), std::end(range), std::move(compare));
}

template
    < class Range1
    , class Range2
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)>
auto starts_with(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) -> bool
{
    auto b1 = std::begin(range1);
    auto e1 = std::end(range1);

    return std::search(b1, e1, std::begin(range2), std::end(range2), std::move(pred)) == b1;
}



template
    < class Range
    , class UnaryFunc
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto transform(Range&& range, UnaryFunc&& func)
{
    return std::transform(std::begin(range), std::end(range), std::begin(range), std::move(func));
}

template
    < class Range
    , class OutputIter
    , class UnaryFunc
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto transform(Range&& range, OutputIter output, UnaryFunc&& func) -> OutputIter
{
    return std::transform(std::begin(range), std::end(range), output, std::move(func));
}

template
    < class Range1
    , class Range2
    , class OutputIter
    , class BinaryFunc
    , CONCEPT_IF(concepts::InputRange<Range1>)
    , CONCEPT_IF(concepts::InputRange<Range2>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto transform(Range1&& range1, Range2&& range2, OutputIter output, BinaryFunc&& func) -> OutputIter
{
    return std::transform(std::begin(range1), std::end(range1), std::begin(range2), output, std::move(func));
}

template
    < class Policy
    , class Range
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) unique(Policy policy, Range&& range, BinaryPred&& pred = {})
{    
    auto b = std::begin(range);
    auto e = std::end(range);
    return policy(b, std::unique(b, e, std::move(pred)), e);
}

template    
    < class Range
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) unique(Range&& range, BinaryPred&& pred = {})
{
    return unique(return_default, range, std::move(pred));
}

template
    < class Range
    , class OutputIter
    , class BinaryPred = std::equal_to<>
    , CONCEPT_IF(concepts::InputRange<Range>)
    , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
auto unique_copy(Range&& range, OutputIter output, BinaryPred&& pred = {}) -> OutputIter
{
    return std::unique_copy(std::begin(range), std::end(range), output, std::move(pred));
}

template
    < class Policy
    , class Range
    , class T
    , class Compare = std::less<>
    , CONCEPT_IF(detail::is_return_policy<Policy>::value)
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) upper_bound(Policy policy, Range&& range, const T& value, Compare&& compare = { })
{    
    auto b = std::begin(range);
    auto e = std::begin(range);
    return policy(b, std::upper_bound(b, e, value, std::move(compare)), e);
}

template
    < class Range
    , class T
    , class Compare = std::less<>
    , CONCEPT_IF(concepts::ForwardRange<Range>)>
decltype(auto) upper_bound(Range&& range, const T& value, Compare&& compare = {})
{
    return upper_bound(return_default, range, value, std::move(compare));
}

} /* cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ALGORITHM_HPP_ */
