#ifndef CPP_ESSENTIALS_CORE_VIEW_HPP_
#define CPP_ESSENTIALS_CORE_VIEW_HPP_

#include <cpp_essentials/core/iterator_range.hpp>

namespace cpp_essentials::core
{

template <class T>
using mut_span = iterator_range<T*>;

template <class T>
using span = iterator_range<const T*>;



template <class Container>
using mut_view = iterator_range<typename Container::iterator>;

template <class Container>
using view = iterator_range<typename Container::const_iterator>;

} /* cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_VIEW_HPP_ */