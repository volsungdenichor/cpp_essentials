#ifndef CPP_ESSENTIALS_CORE_ALGORITHM_HPP_
#define CPP_ESSENTIALS_CORE_ALGORITHM_HPP_

#pragma once

#include <algorithm>
#include <numeric>

#include <cpp_essentials/concepts/concepts.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/return_policy.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct accumulate_t : adaptable<accumulate_t>
{
    using adaptable::operator();

    template
        < class Range
        , class T
        , class BinaryFunc = std::plus<>
        , CONCEPT_IF(concepts::InputRange<Range>)>
    auto operator ()(Range&& range, T init, BinaryFunc&& func = {}) const -> T
    {
        return std::accumulate(std::begin(range), std::end(range), init, std::move(func));
    }
};

struct adjacent_difference_t
{
    template
        < class Range
        , class OutputIter
        , class BinaryFunc = std::minus<>
        , CONCEPT_IF(concepts::InputRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range&& range, OutputIter output, BinaryFunc&& func = {}) const -> OutputIter
    {
        return std::adjacent_difference(std::begin(range), std::end(range), output, std::move(func));
    }
};

struct all_of_t : adaptable<all_of_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT_IF(concepts::InputRange<Range>)>
    auto operator ()(Range&& range, UnaryPred&& pred) const -> bool
    {
        return std::all_of(std::begin(range), std::end(range), std::move(pred));
    }
};

struct any_of_t : adaptable<any_of_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT_IF(concepts::InputRange<Range>)>
    auto operator ()(Range&& range, UnaryPred&& pred) const -> bool
    {
        return std::any_of(std::begin(range), std::end(range), std::move(pred));
    }
};

struct copy_t : adaptable<copy_t>
{
    using adaptable::operator();

    template
        < class Range
        , class OutputIter
        , CONCEPT_IF(concepts::InputRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range&& range, OutputIter output) const -> OutputIter
    {
        return std::copy(std::begin(range), std::end(range), output);
    }
};

struct copy_if_t : adaptable<copy_if_t>
{
    using adaptable::operator();

    template
        < class Range
        , class OutputIter
        , class UnaryPred
        , CONCEPT_IF(concepts::InputRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range&& range, OutputIter output, UnaryPred&& pred) const -> OutputIter
    {
        return std::copy_if(std::begin(range), std::end(range), output, std::move(pred));
    }
};

struct copy_n_t : adaptable<copy_n_t>
{
    using adaptable::operator();

    template
        < class Range
        , class Size
        , class OutputIter
        , CONCEPT_IF(concepts::InputRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range&& range, Size size, OutputIter output) const -> OutputIter
    {
        return std::copy_n(std::begin(range), size, output);
    }
};

struct count_t : adaptable<count_t>
{
    using adaptable::operator();

    template
        < class Range
        , class T
        , CONCEPT_IF(concepts::InputRange<Range>)>
    auto operator ()(Range&& range, T value) const
    {
        return std::count(std::begin(range), std::end(range), value);
    }
};

struct count_if_t : adaptable<count_if_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT_IF(concepts::InputRange<Range>)>
    auto operator ()(Range&& range, UnaryPred&& pred) const
    {
        return std::count_if(std::begin(range), std::end(range), std::move(pred));
    }
};

struct equal_t
{
    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , CONCEPT_IF(concepts::InputRange<Range1>)
        , CONCEPT_IF(concepts::InputRange<Range2>)>
    auto operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const -> bool
    {
        return std::equal(std::begin(range1), std::end(range1), std::begin(range2), std::end(range2), std::move(pred));
    }
};

struct equal_range_t
{
    template
        < class Range
        , class T
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    auto operator ()(Range&& range, const T& value, Compare&& compare = {}) const
    {
        return make_range(std::equal_range(std::begin(range), std::end(range), value, std::move(compare)));
    }
};

struct fill_t
{
    template
        < class Range
        , class T
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    void operator ()(Range&& range, const T& value) const
    {
        std::fill(std::begin(range), std::end(range), value);
    }
};

template <class Policy = default_return_policy_t>
struct find_t : adaptable<find_t<Policy>>
{
    using adaptable<find_t<Policy>>::operator();

    static constexpr Policy policy = {};
    
    template <class P>
    static constexpr find_t<P> as = {};

    template
        < class Range
        , class T
        , CONCEPT_IF(concepts::InputRange<Range>)>
    decltype(auto) operator ()(Range&& range, T&& value) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return policy(b, std::find(b, e, value), e);
    }
};

template <class Policy = default_return_policy_t>
struct find_if_t : adaptable<find_if_t<Policy>>
{
    using adaptable<find_if_t<Policy>>::operator();

    static constexpr Policy policy = {};

    template <class P>
    static constexpr find_if_t<P> as = {};

    template
        < class Range
        , class UnaryPred
        , CONCEPT_IF(concepts::InputRange<Range>)>
    decltype(auto) operator ()(Range&& range, UnaryPred&& pred) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return policy(b, std::find_if(b, e, std::move(pred)), e);
    }
};

template <class Policy = default_return_policy_t>
struct find_if_not_t : adaptable<find_if_not_t<Policy>>
{
    using adaptable<find_if_not_t<Policy>>::operator();

    static constexpr Policy policy = {};

    template <class P>
    static constexpr find_if_not_t<P> as = {};

    template
        < class Range
        , class UnaryPred
        , CONCEPT_IF(concepts::InputRange<Range>)>
    decltype(auto) operator ()(Range&& range, UnaryPred&& pred) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return policy(b, std::find_if_not(b, e, std::move(pred)), e);
    }
};

template <class Policy = default_return_policy_t>
struct find_end_t : adaptable<find_end_t<Policy>>
{
    using adaptable<find_end_t<Policy>>::operator();

    static constexpr Policy policy = {};

    template <class P>
    static constexpr find_end_t<P> as = {};

    template
        < class Range1
        , class Range2
        , CONCEPT_IF(concepts::ForwardRange<Range1>)
        , CONCEPT_IF(concepts::ForwardRange<Range2>)>
    decltype(auto) operator ()(Range1&& range1, Range2&& range2) const
    {
        auto b = std::begin(range1);
        auto e = std::end(range1);
        return policy(b, std::find_end(b, e, std::begin(range2), std::end(range2)), e);
    }
};

template <class Policy = default_return_policy_t>
struct find_first_of_t : adaptable<find_first_of_t<Policy>>
{
    using adaptable<find_first_of_t<Policy>>::operator();

    static constexpr Policy policy = {};

    template <class P>
    static constexpr find_first_of_t<P> as = {};

    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , CONCEPT_IF(concepts::InputRange<Range1>)
        , CONCEPT_IF(concepts::ForwardRange<Range2>)>
    decltype(auto) operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        auto b = std::begin(range1);
        auto e = std::end(range1);
        return policy(b, std::find_first_of(b, e, std::begin(range2), std::end(range2), std::move(pred)), e);
    }
};

struct for_each_t : adaptable<for_each_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryFunc
        , CONCEPT_IF(concepts::InputRange<Range>)>
    auto operator ()(Range&& range, UnaryFunc&& func) const
    {
        return std::for_each(std::begin(range), std::end(range), std::move(func));
    }
};

struct generate_t
{
    template
        < class Range
        , class Generator
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    void operator ()(Range&& range, Generator&& generator) const
    {
        std::generate(std::begin(range), std::end(range), std::move(generator));
    }
};

struct generate_n_t
{
    template
        < class OutputIter
        , class Size
        , class Generator
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    void operator ()(OutputIter output, Size size, Generator&& generator) const
    {
        std::generate_n(output, size, std::move(generator));
    }
};

struct includes_t
{
    template
        < class Range1
        , class Range2
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::InputRange<Range1>)
        , CONCEPT_IF(concepts::InputRange<Range2>)>
    auto operator ()(Range1&& range1, Range2&& range2, Compare&& compare = {}) const -> bool
    {
        return std::includes(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            std::move(compare));
    }
};

struct inner_product_t
{
    template
        < class Range1
        , class Range2
        , class T
        , class BinaryFunc1 = std::plus<>
        , class BinaryFunc2 = std::multiplies<>
        , CONCEPT_IF(concepts::InputRange<Range1>)
        , CONCEPT_IF(concepts::InputRange<Range2>)>
    auto operator ()(Range1&& range1, Range2&& range2, T init, BinaryFunc1&& func1 = {}, BinaryFunc2&& func2 = {}) const -> T
    {
        return std::inner_product(std::begin(range1), std::end(range1), std::begin(range2), init, std::move(func1), std::move(func2));
    }
};

struct iota_t
{
    template
        < class Range
        , class T
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    void operator ()(Range&& range, T value) const
    {
        std::iota(std::begin(range), std::end(range), value);
    }
};

struct is_heap_t : adaptable<is_heap_t>
{
    using adaptable::operator();

    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
    auto operator ()(Range&& range, Compare&& compare = {}) const -> bool
    {
        return std::is_heap(std::begin(range), std::end(range), std::move(compare));
    }
};

template <class Policy = default_return_policy_t>
struct is_heap_until_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr is_heap_until_t<P> as = {};

    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
    decltype(auto) operator ()(Range&& range, Compare&& compare = {}) const
    {
        auto b = std::begin(range);
        auto e = std::begin(range);
        return policy(b, std::is_heap_until(b, e, std::move(compare)), e);
    }
};

struct is_partitioned_t : adaptable<is_partitioned_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryFunc
        , CONCEPT_IF(concepts::InputRange<Range>)>
    auto operator ()(Range&& range, UnaryFunc&& func) const -> bool
    {
        return std::is_partitioned(std::begin(range), std::end(range), std::move(func));
    }
};

struct is_permutation_t : adaptable<is_permutation_t>
{
    using adaptable::operator();

    template
        < class Range1
        , class Range2
        , CONCEPT_IF(concepts::ForwardRange<Range1>)
        , CONCEPT_IF(concepts::ForwardRange<Range2>)>
    auto operator ()(Range1&& range1, Range2&& range2) const -> bool
    {
        return std::is_permutation(std::begin(range1), std::end(range1), std::begin(range2));
    }
};

struct is_sorted_t : adaptable<is_sorted_t>
{
    using adaptable::operator();

    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    auto operator ()(Range&& range, Compare&& compare = {}) const -> bool
    {
        return std::is_sorted(std::begin(range), std::end(range), std::move(compare));
    }
};

template <class Policy = default_return_policy_t>
struct is_sorted_until_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr is_sorted_until_t<P> as = {};

    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    decltype(auto) operator ()(Range&& range, Compare&& compare = {}) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return policy(b, std::is_sorted_until(b, e, std::move(compare)), e);
    }
};

struct lexicographical_compare_t
{
    template
        < class Range1
        , class Range2
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::InputRange<Range1>)
        , CONCEPT_IF(concepts::InputRange<Range2>)>
    auto operator ()(Range1&& range1, Range2&& range2, Compare&& compare = {}) const -> bool
    {
        return std::lexicographical_compare(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            std::move(compare));
    }
};

template <class Policy = default_return_policy_t>
struct lower_bound_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr lower_bound_t<P> as = {};

    template
        < class Range
        , class T
        , class Compare = std::plus<>
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    decltype(auto) operator ()(Range&& range, const T& value, Compare&& compare = {}) const
    {
        auto b = std::begin(range);
        auto e = std::begin(range);
        return policy(b, std::lower_bound(b, e, value, std::move(compare)), e);
    }
};

struct make_heap_t
{
    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
    void operator ()(Range&& range, Compare&& compare = {}) const
    {
        std::make_heap(std::begin(range), std::end(range), std::move(compare));
    }
};

template <class Policy = default_return_policy_t>
struct max_element_t : adaptable<max_element_t<Policy>>
{
    using adaptable<max_element_t<Policy>>::operator();

    static constexpr Policy policy = {};

    template <class P>
    static constexpr max_element_t<P> as = {};

    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    decltype(auto) operator ()(Range&& range, Compare&& compare = {}) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return policy(b, std::max_element(b, e, std::move(compare)), e);
    }
};

struct merge_t
{
    template
        < class Range1
        , class Range2
        , class OutputIter
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::InputRange<Range1>)
        , CONCEPT_IF(concepts::InputRange<Range2>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) const -> OutputIter
    {
        return std::merge(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            output,
            std::move(compare));
    }
};

struct inplace_merge_t
{
    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::BidirectionalRange<Range>)>
    void operator ()(Range&& range, concepts::range_iterator<Range> middle, Compare&& compare = {}) const
    {
        std::inplace_merge(std::begin(range), middle, std::end(range), std::move(compare));
    }
};

template <class Policy = default_return_policy_t>
struct minmax_element_t : adaptable<minmax_element_t<Policy>>
{
    using adaptable<minmax_element_t<Policy>>::operator();

    static constexpr Policy policy = {};

    template <class P>
    static constexpr minmax_element_t<P> as = {};

    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    decltype(auto) operator ()(Range&& range, Compare&& compare = {}) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);

        auto result = std::minmax_element(b, e, std::move(compare));

        return std::make_pair(policy(b, std::get<0>(result), e), policy(b, std::get<1>(result), e));
    }
};

template <class Policy = default_return_policy_t>
struct min_element_t : adaptable<min_element_t<Policy>>
{
    using adaptable<min_element_t<Policy>>::operator();

    static constexpr Policy policy = {};

    template <class P>
    static constexpr min_element_t<P> as = {};

    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    decltype(auto) operator ()(Range&& range, Compare&& compare = {}) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return policy(b, std::min_element(b, e, std::move(compare)), e);
    }
};

template <class Policy = default_return_policy_t>
struct mismatch_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr mismatch_t<P> as = {};

    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , CONCEPT_IF(detail::is_return_policy<Policy>::value)
        , CONCEPT_IF(concepts::InputRange<Range1>)
        , CONCEPT_IF(concepts::InputRange<Range2>)>
    auto operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        auto b1 = std::begin(range1);
        auto e1 = std::end(range1);

        auto b2 = std::begin(range2);
        auto e2 = std::end(range2);

        auto result = std::mismatch(b1, e1, b2, std::move(pred));

        return std::make_pair(policy(b1, std::get<0>(result), e1), policy(b2, std::get<1>(result), e2));
    }
};

struct move_t : adaptable<move_t>
{
    using adaptable::operator();

    template
        < class Range
        , class OutputIter
        , CONCEPT_IF(concepts::InputRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range&& range, OutputIter output) const -> OutputIter
    {
        return std::move(std::begin(range), std::end(range), output);
    }
};

struct next_permutation_t
{
    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::BidirectionalRange<Range>)>
    auto operator ()(Range&& range, Compare&& compare = {}) const -> bool
    {
        return std::next_permutation(std::begin(range), std::end(range), std::move(compare));
    }
};

struct none_of_t : adaptable<none_of_t>
{
    using adaptable::operator();

    template
        < class Range
        , class UnaryPred
        , CONCEPT_IF(concepts::InputRange<Range>)>
    auto operator ()(Range&& range, UnaryPred&& pred) const -> bool
    {
        return std::none_of(std::begin(range), std::end(range), std::move(pred));
    }
};

struct nth_element_t
{
    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
    void operator ()(Range&& range, concepts::range_iterator<Range> middle, Compare&& compare = {}) const
    {
        std::nth_element(std::begin(range), middle, std::end(range), std::move(compare));
    }
};

struct partial_sort_t
{
    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
    void operator ()(Range&& range, concepts::range_iterator<Range> middle, Compare&& compare = {}) const
    {
        std::partial_sort(std::begin(range), middle, std::end(range), std::move(compare));
    }
};

template <class Policy = default_return_policy_t>
struct partial_sort_copy_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr partial_sort_copy_t<P> as = {};

    template
        < class Range1
        , class Range2
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::RandomAccessRange<Range1>)
        , CONCEPT_IF(concepts::RandomAccessRange<Range2>)>
    decltype(auto) operator ()(Range1&& range1, Range2&& range2, Compare&& compare = {}) const
    {
        auto b2 = std::begin(range2);
        auto e2 = std::end(range2);
        return policy(b2, std::partial_sort_copy(std::begin(range1), std::end(range1), b2, e2, std::move(compare)), e2);
    }
};

struct partial_sum_t
{
    template
        < class Range
        , class OutputIter
        , class BinaryFunc = std::plus<>
        , CONCEPT_IF(concepts::InputRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range&& range, OutputIter output, BinaryFunc&& func = {}) const -> OutputIter
    {
        return std::partial_sum(std::begin(range), std::end(range), output, std::move(func));
    }
};

template <class Policy = default_return_policy_t>
struct partition_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr partition_t<P> as = {};

    template
        < class Range
        , class UnaryPred
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    decltype(auto) operator ()(Range&& range, UnaryPred&& pred) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return policy(b, std::partition(b, e, std::move(pred)), e);
    }
};

struct partition_copy_t
{
    template
        < class Range
        , class OutputIter1
        , class OutputIter2
        , class UnaryPred
        , CONCEPT_IF(concepts::InputRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter1>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter2>)>
    auto operator ()(Range&& range, OutputIter1 result_true, OutputIter2 result_false, UnaryPred&& pred) const
    {
        return std::partition_copy(std::begin(range), std::end(range), result_true, result_false, std::move(pred));
    }
};

template <class Policy = default_return_policy_t>
struct stable_partition_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr stable_partition_t<P> as = {};

    template
        < class Range
        , class UnaryPred
        , CONCEPT_IF(concepts::BidirectionalRange<Range>)>
    decltype(auto) operator ()(Range&& range, UnaryPred&& pred) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return policy(b, std::stable_partition(b, e, std::move(pred)), e);
    }
};

struct prev_permutation_t
{
    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::BidirectionalRange<Range>)>
    auto operator ()(Range&& range, Compare&& compare = {}) const -> bool
    {
        return std::prev_permutation(std::begin(range), std::end(range), std::move(compare));
    }
};

struct push_heap_t
{
    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
    void operator ()(Range&& range, Compare&& compare = {}) const
    {
        std::push_heap(std::begin(range), std::end(range), std::move(compare));
    }
};

template <class Policy = default_return_policy_t>
struct remove_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr remove_t<P> as = {};

    template
        < class Range
        , class T
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    decltype(auto) operator ()(Range&& range, const T& value) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return policy(b, std::remove(b, e, value), e);
    }
};

template <class Policy = default_return_policy_t>
struct remove_if_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr remove_if_t<P> as = {};

    template
        < class Range
        , class UnaryPred
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    decltype(auto) operator ()(Range&& range, UnaryPred&& pred) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return policy(b, std::remove_if(b, e, std::move(pred)), e);
    }
};

struct remove_copy_t
{
    template
        < class Range
        , class OutputIter
        , class T
        , CONCEPT_IF(concepts::InputRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range&& range, OutputIter output, const T& value) const -> OutputIter
    {
        return std::remove_copy(std::begin(range), std::end(range), output, value);
    }
};

struct remove_copy_if_t
{
    template
        < class Range
        , class OutputIter
        , class UnaryPred
        , CONCEPT_IF(concepts::InputRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range&& range, OutputIter output, UnaryPred&& pred) const -> OutputIter
    {
        return std::remove_copy_if(std::begin(range), std::end(range), output, std::move(pred));
    }
};

struct replace_t
{
    template
        < class Range
        , class T
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    void operator ()(Range&& range, const T& old_value, const T& new_value) const
    {
        std::replace(std::begin(range), std::end(range), old_value, new_value);
    }
};

struct replace_if_t
{
    template
        < class Range
        , class UnaryPred
        , class T
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    void operator ()(Range&& range, UnaryPred pred, const T& new_value) const
    {
        std::replace_if(std::begin(range), std::end(range), pred, new_value);
    }
};

struct replace_copy_t
{
    template
        < class Range
        , class OutputIter
        , class T
        , CONCEPT_IF(concepts::InputRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range&& range, OutputIter output, const T& old_value, const T& new_value) const -> OutputIter
    {
        return std::replace_copy(std::begin(range), std::end(range), output, old_value, new_value);
    }
};

struct replace_copy_if_t
{
    template
        < class Range
        , class OutputIter
        , class UnaryPred
        , class T
        , CONCEPT_IF(concepts::InputRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range&& range, OutputIter output, UnaryPred&& pred, const T& new_value) const -> OutputIter
    {
        return std::replace_copy_if(std::begin(range), std::end(range), output, std::move(pred), new_value);
    }
};

struct reverse_t
{
    template
        < class Range
        , CONCEPT_IF(concepts::BidirectionalRange<Range>)>
    void operator ()(Range&& range) const
    {
        std::reverse(std::begin(range), std::end(range));
    }
};

struct reverse_copy_t
{
    template
        < class Range
        , class OutputIter
        , CONCEPT_IF(concepts::BidirectionalRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator()(Range&& range, OutputIter output) const -> OutputIter
    {
        return std::reverse_copy(std::begin(range), std::end(range), output);
    }
};

template <class Policy = default_return_policy_t>
struct rotate_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr rotate_t<P> as = {};

    template
        < class Range
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    decltype(auto) operator ()(Range&& range, concepts::range_iterator<Range> middle) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return policy(b, std::rotate(b, middle, e), e);
    }
};

struct rotate_copy_t
{
    template
        < class Range
        , class OutputIter
        , CONCEPT_IF(concepts::ForwardRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range&& range, concepts::range_iterator<Range> middle, OutputIter output) const -> OutputIter
    {
        return std::rotate_copy(std::begin(range), middle, std::end(range), output);
    }
};

template <class Policy = default_return_policy_t, CONCEPT_IF(is_return_policy<Policy>::value)>
struct search_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr search_t<P> as = {};

    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , CONCEPT_IF(concepts::ForwardRange<Range1>)
        , CONCEPT_IF(concepts::ForwardRange<Range2>)>
    decltype(auto) operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        auto b = std::begin(range1);
        auto e = std::end(range1);
        return policy(b, std::search(b, e, std::begin(range2), std::end(range2), std::move(pred)), e);
    }
};

template <class Policy = default_return_policy_t, CONCEPT_IF(is_return_policy<Policy>::value)>
struct search_n_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr search_n_t<P> as = {};

    template
        < class Range
        , class Size
        , class T
        , class BinaryPred = std::equal_to<>
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    decltype(auto) operator ()(Range&& range, Size size, const T& value, BinaryPred&& pred = {}) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return policy(b, std::search_n(b, e, size, value, std::move(pred)), e);
    }
};

struct set_difference_t
{
    template
        < class Range1
        , class Range2
        , class OutputIter
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::InputRange<Range1>)
        , CONCEPT_IF(concepts::InputRange<Range2>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) const -> OutputIter
    {
        return std::set_difference(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            output,
            std::move(compare));
    }
};

struct set_intersection_t
{
    template
        < class Range1
        , class Range2
        , class OutputIter
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::InputRange<Range1>)
        , CONCEPT_IF(concepts::InputRange<Range2>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) const -> OutputIter
    {
        return std::set_intersection(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            output,
            std::move(compare));
    }
};

struct set_symmetric_difference_t
{
    template
        < class Range1
        , class Range2
        , class OutputIter
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::InputRange<Range1>)
        , CONCEPT_IF(concepts::InputRange<Range2>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) const -> OutputIter
    {
        return std::set_symmetric_difference(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            output,
            std::move(compare));
    }
};

struct set_union_t
{
    template
        < class Range1
        , class Range2
        , class OutputIter
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::InputRange<Range1>)
        , CONCEPT_IF(concepts::InputRange<Range2>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range1&& range1, Range2&& range2, OutputIter output, Compare&& compare = {}) const -> OutputIter
    {
        return std::set_union(
            std::begin(range1),
            std::end(range1),
            std::begin(range2),
            std::end(range2),
            output,
            std::move(compare));
    }
};

struct shuffle_t
{
    template
        < class Range
        , class RandomNumberGenerator
        , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
    void operator ()(Range&& range, RandomNumberGenerator&& generator) const
    {
        std::shuffle(std::begin(range), std::end(range), std::move(generator));
    }
};

struct random_shuffle_t
{
    template
        < class Range
        , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
    void operator ()(Range&& range) const
    {
        std::random_shuffle(std::begin(range), std::end(range));
    }

    template
        < class Range
        , class RandomNumberGenerator
        , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
    void operator ()(Range&& range, RandomNumberGenerator&& generator) const
    {
        std::random_shuffle(std::begin(range), std::end(range), std::move(generator));
    }
};

struct sort_t
{
    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
    void operator ()(Range&& range, Compare&& compare = {}) const
    {
        std::sort(std::begin(range), std::end(range), std::move(compare));
    }
};

struct stable_sort_t
{
    template
        < class Range
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::RandomAccessRange<Range>)>
    void operator ()(Range&& range, Compare&& compare = {}) const
    {
        std::stable_sort(std::begin(range), std::end(range), std::move(compare));
    }
};

struct transform_t
{
    template
        < class Range
        , class OutputIter
        , class UnaryFunc
        , CONCEPT_IF(concepts::InputRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range&& range, OutputIter output, UnaryFunc&& func) const -> OutputIter
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
    auto operator ()(Range1&& range1, Range2&& range2, OutputIter output, BinaryFunc&& func) const -> OutputIter
    {
        return std::transform(std::begin(range1), std::end(range1), std::begin(range2), output, std::move(func));
    }
};

template <class Policy = default_return_policy_t, CONCEPT_IF(is_return_policy<Policy>::value)>
struct unique_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr unique_t<P> as = {};

    template
        < class Range
        , class BinaryPred = std::equal_to<>
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    decltype(auto) operator ()(Range&& range, BinaryPred&& pred = {}) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return policy(b, std::unique(b, e, std::move(pred)), e);
    }
};

struct unique_copy_t
{
    template
        < class Range
        , class OutputIter
        , class BinaryPred = std::equal_to<>
        , CONCEPT_IF(concepts::InputRange<Range>)
        , CONCEPT_IF(concepts::OutputIterator<OutputIter>)>
    auto operator ()(Range&& range, OutputIter output, BinaryPred&& pred = {}) const -> OutputIter
    {
        return std::unique_copy(std::begin(range), std::end(range), output, std::move(pred));
    }
};

template <class Policy = default_return_policy_t, CONCEPT_IF(detail::is_return_policy<Policy>::value)>
struct upper_bound_t
{
    static constexpr Policy policy = {};

    template <class P>
    static constexpr upper_bound_t<P> as = {};

    template
        < class Range
        , class T
        , class Compare = std::less<>
        , CONCEPT_IF(concepts::ForwardRange<Range>)>
    decltype(auto) operator ()(Range&& range, const T& value, Compare&& compare = {}) const
    {
        auto b = std::begin(range);
        auto e = std::begin(range);
        return policy(b, std::upper_bound(b, e, value, std::move(compare)), e);
    }
};

} /* namespace detail */

static constexpr detail::accumulate_t accumulate = {};
static constexpr detail::adjacent_difference_t adjacent_difference = {};
static constexpr detail::all_of_t all_of = {};
static constexpr detail::any_of_t any_of = {};
static constexpr detail::copy_t copy = {};
static constexpr detail::copy_if_t copy_if = {};
static constexpr detail::copy_n_t copy_n = {};
static constexpr detail::count_t count = {};
static constexpr detail::count_if_t count_if = {};
static constexpr detail::equal_t equal = {};
static constexpr detail::equal_range_t equal_range = {};
static constexpr detail::fill_t fill = {};
static constexpr detail::find_t<> find = {};
static constexpr detail::find_if_t<> find_if = {};
static constexpr detail::find_if_not_t<> find_if_not = {};
static constexpr detail::find_end_t<> find_end = {};
static constexpr detail::find_first_of_t<> find_first_of = {};
static constexpr detail::for_each_t for_each = {};
static constexpr detail::generate_t generate = {};
static constexpr detail::generate_n_t generate_n = {};
static constexpr detail::includes_t includes = {};
static constexpr detail::inner_product_t inner_product = {};
static constexpr detail::iota_t iota = {};
static constexpr detail::is_heap_t is_heap = {};
static constexpr detail::is_heap_until_t<> is_heap_until = {};
static constexpr detail::is_partitioned_t is_partitioned = {};
static constexpr detail::is_permutation_t is_permutation = {};
static constexpr detail::is_sorted_t is_sorted = {};
static constexpr detail::is_sorted_until_t<> is_sorted_until = {};
static constexpr detail::lexicographical_compare_t lexicographical_compare = {};
static constexpr detail::lower_bound_t<> lower_bound = {};
static constexpr detail::make_heap_t make_heap = {};
static constexpr detail::max_element_t<> max_element = {};
static constexpr detail::merge_t merge = {};
static constexpr detail::inplace_merge_t inplace_merge = {};
static constexpr detail::minmax_element_t<> minmax_element = {};
static constexpr detail::min_element_t<> min_element = {};
static constexpr detail::mismatch_t<> mismatch = {};
static constexpr detail::move_t move = {};
static constexpr detail::next_permutation_t next_permutation = {};
static constexpr detail::none_of_t none_of = {};
static constexpr detail::nth_element_t nth_element = {};
static constexpr detail::partial_sort_t partial_sort = {};
static constexpr detail::partial_sort_copy_t<> partial_sort_copy = {};
static constexpr detail::partial_sum_t partial_sum = {};
static constexpr detail::partition_t<> partition = {};
static constexpr detail::partition_copy_t partition_copy = {};
static constexpr detail::stable_partition_t<> stable_partition = {};
static constexpr detail::prev_permutation_t prev_permutation = {};
static constexpr detail::push_heap_t push_heap = {};
static constexpr detail::remove_t<> remove = {};
static constexpr detail::remove_if_t<> remove_if = {};
static constexpr detail::remove_copy_t remove_copy = {};
static constexpr detail::remove_copy_if_t remove_copy_if = {};
static constexpr detail::replace_t replace = {};
static constexpr detail::replace_if_t replace_if = {};
static constexpr detail::replace_copy_t replace_copy = {};
static constexpr detail::replace_copy_if_t replace_copy_if = {};
static constexpr detail::reverse_t reverse = {};
static constexpr detail::reverse_copy_t reverse_copy = {};
static constexpr detail::rotate_t<> rotate = {};
static constexpr detail::rotate_copy_t rotate_copy = {};
static constexpr detail::search_t<> search = {};
static constexpr detail::search_n_t<> search_n = {};
static constexpr detail::set_difference_t set_difference = {};
static constexpr detail::set_intersection_t set_intersection = {};
static constexpr detail::set_symmetric_difference_t set_symmetric_difference = {};
static constexpr detail::set_union_t set_union = {};
static constexpr detail::shuffle_t shuffle = {};
static constexpr detail::random_shuffle_t random_shuffle = {};
static constexpr detail::sort_t sort = {};
static constexpr detail::stable_sort_t stable_sort = {};
static constexpr detail::transform_t transform = {};
static constexpr detail::unique_t<> unique = {};
static constexpr detail::unique_copy_t unique_copy = {};
static constexpr detail::upper_bound_t<> upper_bound = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ALGORITHM_HPP_ */

