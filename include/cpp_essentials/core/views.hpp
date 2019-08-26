#ifndef CPP_ESSENTIALS_CORE_VIEWS_HPP_
#define CPP_ESSENTIALS_CORE_VIEWS_HPP_

#pragma once

#include <string>
#include <locale>
#include <algorithm>
#include <cpp_essentials/core/iterator_range.hpp>

namespace cpp_essentials::core
{

template <class T>
using span = iterator_range<T*>;

template <class T>
using const_span = iterator_range<const T*>;

template <class T>
using cspan = const_span<T>;


template <class Container>
using view = iterator_range<cc::range_iter<Container>>;

template <class Container>
using const_view = iterator_range<cc::range_iter<const Container>>;

template <class Container>
using cview = const_view<Container>;

} /* namespace cpp_essentials/core */

#endif /* CPP_ESSENTIALS_CORE_VIEWS_HPP_ */