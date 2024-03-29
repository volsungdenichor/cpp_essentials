#ifndef CPP_ESSENTIALS_CC_CC_HPP_
#define CPP_ESSENTIALS_CC_CC_HPP_

#include <type_traits>
#include <functional>
#include <iterator>
#include <iostream>

#pragma once

namespace cc
{

template <class T>
using Reference = std::enable_if_t<std::is_reference_v<T>>;

template <class T, class U>
using Same = std::enable_if_t<std::is_same_v<T, U>>;

template <class T, class U>
using Convertible = std::enable_if_t<std::is_convertible_v<T, U>>;

template <class T>
using Arithmetic = std::enable_if_t<std::is_arithmetic_v<T>>;

template <class T>
using Integral = std::enable_if_t<std::is_integral_v<T>>;

template <class T>
using FloatingPoint = std::enable_if_t<std::is_floating_point_v<T>>;

template <class T>
using Enum = std::enable_if_t<std::is_enum_v<T>>;

template <class T, class U>
using BaseOf = std::enable_if_t<std::is_base_of_v<T, U>>;

template <class T, class U>
using Assignable = std::enable_if_t<std::is_assignable_v<T, U>>;

template <class T, class... Args>
using Constructible = std::enable_if_t<std::is_constructible_v<T, Args...>>;


namespace detail
{

template <class...>
struct void_impl
{
    using type = void;
};

} /* namespace detail */

template <class... T>
using void_t = typename detail::void_impl<T...>::type;

namespace detail
{

template <class AlwaysVoid, template<class...> class Op, class... Args>
struct detector
{
    using value_t = std::false_type;
};


template <template<class...> class Op, class... Args>
struct detector<void_t<Op<Args...>>, Op, Args...>
{
    using value_t = std::true_type;
};

} /* namespace detail */


template <template<class...> class Op, class... Args>
using is_detected = typename detail::detector<void, Op, Args...>::value_t;

template <template<class...> class Op, class... Args>
static constexpr bool is_detected_v = is_detected<Op, Args...>::value;

template <class T>
using Optional = decltype(*std::declval<T&>(), static_cast<bool>(std::declval<T&>()));

template <class T>
using Increment = decltype(++std::declval<T&>());

template <class T>
using Decrement = decltype(--std::declval<T&>());


template <class T, class U>
using Add = decltype(std::declval<T&>() + std::declval<U&>());

template <class T, class U>
using Subtract = decltype(std::declval<T&>() - std::declval<U&>());

template <class T, class U>
using Multiply = decltype(std::declval<T&>() * std::declval<U&>());

template <class T, class U>
using Divide = decltype(std::declval<T&>() / std::declval<U&>());


template <class T, class U = T>
using EqualityCompare = decltype(std::declval<T&>() == std::declval<U&>());

template <class T, class U = T>
using LessThanCompare = decltype(std::declval<T&>() < std::declval<U&>());

template <class T, class U = T>
using Compare = std::conjunction<EqualityCompare<T, U>, LessThanCompare<T, U>>;


template <class T>
using HasOstreamOperator = decltype(std::declval<std::ostream&>() << std::declval<const T&>());

template <class T>
using HasIstreamIterator = decltype(std::declval<std::istream&>() >> std::declval<T&>());


template <class T>
using Iterator = void_t
    < typename std::iterator_traits<T>::value_type
    , typename std::iterator_traits<T>::reference
    , typename std::iterator_traits<T>::pointer
    , typename std::iterator_traits<T>::iterator_category
    , typename std::iterator_traits<T>::difference_type>;

template <class T>
using Range = void_t
    < std::enable_if_t<is_detected_v<Iterator, decltype(std::begin(std::declval<T&>()))>>
    , std::enable_if_t<is_detected_v<Iterator, decltype(std::end(std::declval<T&>()))>>>;

namespace detail
{

template <class T>
using range_iterator = decltype(std::begin(std::declval<T&>()));


template <class Category, class T>
using iterator_of_category = std::enable_if_t<std::is_base_of_v<Category, typename std::iterator_traits<T>::iterator_category>>;

template <class Category, class T>
using range_of_category = iterator_of_category<Category, range_iterator<T>>;

template <typename T>
constexpr bool _is_non_const_reference = std::is_lvalue_reference<T>::value && !std::is_const<std::remove_reference_t<T>>::value;

} /* namespace detail */

template <class Type, class T>
auto make_func(Type T::*ptr)
{
    return std::mem_fn(ptr);
}

template <class Func>
auto make_func(Func func)
{
    return func;
}


template <class T>
using InputIterator = detail::iterator_of_category<std::input_iterator_tag, T>;

template <class T>
using ForwardIterator = detail::iterator_of_category<std::forward_iterator_tag, T>;

template <class T>
using BidirectionalIterator = detail::iterator_of_category<std::bidirectional_iterator_tag, T>;

template <class T>
using RandomAccessIterator = detail::iterator_of_category<std::random_access_iterator_tag, T>;

template <class T>
using OutputIterator = void_t
    < Iterator<T>
    , std::enable_if_t<detail::_is_non_const_reference<decltype(*std::declval<T&>())>>>;


template <class T>
using InputRange = detail::range_of_category<std::input_iterator_tag, T>;

template <class T>
using ForwardRange = detail::range_of_category<std::forward_iterator_tag, T>;

template <class T>
using BidirectionalRange = detail::range_of_category<std::bidirectional_iterator_tag, T>;

template <class T>
using RandomAccessRange = detail::range_of_category<std::random_access_iterator_tag, T>;


template <class T>
using NullaryFunction = decltype(std::declval<T&>()());

template <class T, class A>
using UnaryFunction = decltype(make_func(std::declval<T&>())(std::declval<A&>()));

template <class T, class A, class B = A>
using BinaryFunction = decltype(make_func(std::declval<T&>())(std::declval<A&>(), std::declval<B&>()));

template <class T, class A>
using UnaryPredicate = std::enable_if_t<std::is_convertible_v<UnaryFunction<T, A>, bool>>;

template <class T, class A, class B = A>
using BinaryPredicate = std::enable_if_t<std::is_convertible_v<BinaryFunction<T, A, B>, bool>>;


template <class T>
using iter_val = typename std::iterator_traits<T>::value_type;

template <class T>
using iter_ref = typename std::iterator_traits<T>::reference;

template <class T>
using iter_cat = typename std::iterator_traits<T>::iterator_category;

template <class T>
using iter_diff = typename std::iterator_traits<T>::difference_type;


template <class T>
using range_iter = detail::range_iterator<T>;

template <class T>
using range_val = iter_val<range_iter<T>>;

template <class T>
using range_ref = iter_ref<range_iter<T>>;

template <class T>
using range_cat = iter_cat<range_iter<T>>;

} /* namespace cc */

#endif /* CPP_ESSENTIALS_CC_CC_HPP_ */
