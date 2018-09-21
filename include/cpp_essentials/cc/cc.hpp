#ifndef CPP_ESSENTIALS_CC_CC_HPP_
#define CPP_ESSENTIALS_CC_CC_HPP_

#include <type_traits>
#include <iterator>
#include <iostream>

#pragma once

namespace cc
{

template <class T, class U>
using is_same = std::enable_if_t<std::is_same_v<T, U>>;

template <class T, class U>
using is_convertible = std::enable_if_t<std::is_convertible_v<T, U>>;


namespace detail
{

struct any_type
{
    template <class T>
    operator T() const;
};

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


template<template<class...> class Op, class... Args>
using is_detected = typename detail::detector<void, Op, Args...>::value_t;


template <class T>
using incrementable = decltype(++std::declval<T&>());

template <class T>
using decrementable = decltype(--std::declval<T&>());


template <class T, class U>
using addable = decltype(std::declval<T&>() + std::declval<U&>());

template <class T, class U>
using subtractable = decltype(std::declval<T&>() - std::declval<U&>());

template <class T, class U>
using multipliable = decltype(std::declval<T&>() * std::declval<U&>());

template <class T, class U>
using dividable = decltype(std::declval<T&>() / std::declval<U&>());


template <class T, class U = T>
using equality_comparable = decltype(std::declval<T&>() == std::declval<U&>());

template <class T, class U = T>
using less_than_comparable = decltype(std::declval<T&>() < std::declval<U&>());

template <class T, class U = T>
using comparable = std::conjunction<equality_comparable<T, U>, less_than_comparable<T, U>>;


template <class T>
using has_ostream_operator = decltype(std::declval<std::ostream&>() << std::declval<const T&>());


template <class T>
using iterator = void_t
    < typename std::iterator_traits<T>::value_type
    , typename std::iterator_traits<T>::reference
    , typename std::iterator_traits<T>::pointer
    , typename std::iterator_traits<T>::iterator_category
    , typename std::iterator_traits<T>::difference_type>;

template <class T>
using range = void_t
    < std::enable_if_t<is_detected<iterator, decltype(std::begin(std::declval<T&>()))>::value>
    , std::enable_if_t<is_detected<iterator, decltype(std::end(std::declval<T&>()))>::value>>;

namespace detail
{

template <class T>
using range_iterator = decltype(std::begin(std::declval<T&>()));


template <class Category, class T>
using iterator_of_category = std::conjunction<iterator<T>, std::enable_if_t<std::is_base_of_v<Category, typename std::iterator_traits<T>::iterator_category>>>;

template <class Category, class T>
using range_of_category = iterator_of_category<Category, range_iterator<T>>;

} /* namespace detail */


template <class T>
using input_iterator = detail::iterator_of_category<std::input_iterator_tag, T>;

template <class T>
using forward_iterator = detail::iterator_of_category<std::forward_iterator_tag, T>;

template <class T>
using bidirectional_iterator = detail::iterator_of_category<std::bidirectional_iterator_tag, T>;

template <class T>
using random_access_iterator = detail::iterator_of_category<std::random_access_iterator_tag, T>;

template <class T>
using output_iterator = void_t
    < iterator<T>
    , decltype(*std::declval<T&>() = std::declval<detail::any_type&>())>;


template <class T>
using input_range = detail::range_of_category<std::input_iterator_tag, T>;

template <class T>
using forward_range = detail::range_of_category<std::forward_iterator_tag, T>;

template <class T>
using bidirectional_range = detail::range_of_category<std::bidirectional_iterator_tag, T>;

template <class T>
using random_access_range = detail::range_of_category<std::random_access_iterator_tag, T>;


template <class T>
using range_val = typename std::iterator_traits<detail::range_iterator<T>>::value_type;

template <class T>
using range_ref = typename std::iterator_traits<detail::range_iterator<T>>::reference;


template <class T>
using nullary_function = decltype(std::declval<T&>()());

template <class T, class A>
using unary_function = decltype(std::declval<T&>()(std::declval<A&>()));

template <class T, class A, class B>
using binary_function = decltype(std::declval<T&>()(std::declval<A&>(), std::declval<B&>()));

template <class T, class A>
using unary_predicate = std::enable_if_t<std::is_convertible_v<unary_function<T, A>, bool>>;

template <class T, class A, class B>
using binary_predicate = std::enable_if_t<std::is_convertible_v<binary_function<T, A, B>, bool>>;

} /* namespace cc */

#endif /* CPP_ESSENTIALS_CC_CC_HPP_ */
