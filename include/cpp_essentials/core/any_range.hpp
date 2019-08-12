#ifndef CPP_ESSENTIALS_CORE_ANY_RANGE_HPP_
#define CPP_ESSENTIALS_CORE_ANY_RANGE_HPP_

#pragma once

#include <cpp_essentials/core/detail/any_iterator.hpp>
#include <cpp_essentials/core/iterator_range.hpp>

namespace cpp_essentials::core
{

template <class T>
using any_range = iterator_range<detail::any_iterator<T>>;

template <class T>
using iterable = any_range<T>;

} /* cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ANY_RANGE_HPP_ */
