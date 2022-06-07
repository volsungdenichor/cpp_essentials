#ifndef CPP_ESSENTIALS_CORE_VIEWS_ELEMENTWISE_HPP_
#define CPP_ESSENTIALS_CORE_VIEWS_ELEMENTWISE_HPP_

#include <functional>

#include <cpp_essentials/cc/cc.hpp>

#include <cpp_essentials/core/views/zip.hpp>
#include <cpp_essentials/core/views/map.hpp>
#include <cpp_essentials/core/arithmetic_functors.hpp>

namespace cpp_essentials::core::views
{

template
    < class Iter1
    , class Iter2
    , class = cc::Add<cc::iter_ref<Iter1>, cc::iter_ref<Iter2>>>
auto operator +(const iterator_range<Iter1>& lhs, const iterator_range<Iter2>& rhs)
{
    return zip(lhs, rhs, plus());
}

template
    < class Iter1
    , class Iter2
    , class = cc::Subtract<cc::iter_ref<Iter1>, cc::iter_ref<Iter2>>>
auto operator -(const iterator_range<Iter1>& lhs, const iterator_range<Iter2>& rhs)
{
    return zip(lhs, rhs, minus());
}

template
< class Iter1
    , class Iter2
    , class = cc::Multiply<cc::iter_ref<Iter1>, cc::iter_ref<Iter2>>>
auto operator *(const iterator_range<Iter1>& lhs, const iterator_range<Iter2>& rhs)
{
    return zip(lhs, rhs, multiplies());
}

template
    < class Iter1
    , class Iter2
    , class = cc::Divide<cc::iter_ref<Iter1>, cc::iter_ref<Iter2>>>
auto operator /(const iterator_range<Iter1>& lhs, const iterator_range<Iter2>& rhs)
{
    return zip(lhs, rhs, divides());
}


template
    < class Iter
    , class T
    , class = cc::Add<cc::iter_ref<Iter>, T>>
auto operator +(const iterator_range<Iter>& lhs, T rhs)
{
    return map(lhs, plus(rhs));
}

template
    < class T
    , class Iter
    , class = cc::Add<T, cc::iter_ref<Iter>>>
auto operator +(T lhs, const iterator_range<Iter>& rhs)
{
    return rhs + lhs;
}


template
    < class Iter
    , class T
    , class = cc::Subtract<cc::iter_ref<Iter>, T>>
auto operator -(const iterator_range<Iter>& lhs, T rhs)
{
    return map(lhs, minus(rhs));
}

template
    < class T
    , class Iter
    , class = cc::Subtract<T, cc::iter_ref<Iter>>>
auto operator -(T lhs, const iterator_range<Iter>& rhs)
{
    return map(rhs, minus.bind_left(lhs));
}


template
    < class Iter
    , class T
    , class = cc::Multiply<cc::iter_ref<Iter>, T>>
auto operator *(const iterator_range<Iter>& lhs, T rhs)
{
    return map(lhs, multiplies(rhs));
}

template
    < class T
    , class Iter
    , class = cc::Multiply<T, cc::iter_ref<Iter>>>
auto operator *(T lhs, const iterator_range<Iter>& rhs)
{
    return rhs * lhs;
}


template
    < class Iter
    , class T
    , class = cc::Divide<cc::iter_ref<Iter>, T>>
auto operator /(const iterator_range<Iter>& lhs, T rhs)
{
    return map(lhs, divides(rhs));
}

template
    < class T
    , class Iter
    , class = cc::Divide<T, cc::iter_ref<Iter>>>
auto operator /(T lhs, const iterator_range<Iter>& rhs)
{
    return map(rhs, divides.bind_left(lhs));
}


template <class Iter>
auto operator +(const iterator_range<Iter>& item)
{
    return item;
}

template <class Iter>
auto operator -(const iterator_range<Iter>& item)
{
    return map(item, negate());
}

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_VIEWS_ELEMENTWISE_HPP_ */
