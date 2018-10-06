#ifndef CPP_ESSENTIALS_CORE_ELEMENTWISE_HPP_
#define CPP_ESSENTIALS_CORE_ELEMENTWISE_HPP_

#include <functional>

#include <cpp_essentials/math/arithmetic_functors.hpp>
#include <cpp_essentials/cc/cc.hpp>

#include <cpp_essentials/core/zip.hpp>
#include <cpp_essentials/core/map.hpp>

namespace cpp_essentials::core
{

template
    < class Iter1
    , class Iter2
    , CONCEPT = cc::Add<cc::iter_ref<Iter1>, cc::iter_ref<Iter2>>>
auto operator +(const core::iterator_range<Iter1>& lhs, const core::iterator_range<Iter2>& rhs)
{
    return zip(lhs, rhs, math::plus());
}

template
    < class Iter1
    , class Iter2
    , CONCEPT = cc::Subtract<cc::iter_ref<Iter1>, cc::iter_ref<Iter2>>>
auto operator -(const core::iterator_range<Iter1>& lhs, const core::iterator_range<Iter2>& rhs)
{
    return zip(lhs, rhs, math::minus());
}

template
< class Iter1
    , class Iter2
    , CONCEPT = cc::Multiply<cc::iter_ref<Iter1>, cc::iter_ref<Iter2>>>
auto operator *(const core::iterator_range<Iter1>& lhs, const core::iterator_range<Iter2>& rhs)
{
    return zip(lhs, rhs, math::multiplies());
}

template
    < class Iter1
    , class Iter2
    , CONCEPT = cc::Divide<cc::iter_ref<Iter1>, cc::iter_ref<Iter2>>>
auto operator /(const core::iterator_range<Iter1>& lhs, const core::iterator_range<Iter2>& rhs)
{
    return zip(lhs, rhs, math::divides());
}


template
    < class Iter
    , class T
    , CONCEPT = cc::Add<cc::iter_ref<Iter>, T>>
auto operator +(const core::iterator_range<Iter>& lhs, T rhs)
{
    return map(lhs, math::plus(rhs));
}

template
    < class T
    , class Iter
    , CONCEPT = cc::Add<T, cc::iter_ref<Iter>>>
auto operator +(T lhs, const core::iterator_range<Iter>& rhs)
{
    return rhs + lhs;
}


template
    < class Iter
    , class T
    , CONCEPT = cc::Subtract<cc::iter_ref<Iter>, T>>
auto operator -(const core::iterator_range<Iter>& lhs, T rhs)
{
    return map(lhs, math::minus(rhs));
}

template
    < class T
    , class Iter
    , CONCEPT = cc::Subtract<T, cc::iter_ref<Iter>>>
auto operator -(T lhs, const core::iterator_range<Iter>& rhs)
{
    return map(rhs, math::minus.bind_left(lhs));
}


template
    < class Iter
    , class T
    , CONCEPT = cc::Multiply<cc::iter_ref<Iter>, T>>
auto operator *(const core::iterator_range<Iter>& lhs, T rhs)
{
    return map(lhs, math::multiplies(rhs));
}

template
    < class T
    , class Iter
    , CONCEPT = cc::Multiply<T, cc::iter_ref<Iter>>>
auto operator *(T lhs, const core::iterator_range<Iter>& rhs)
{
    return rhs * lhs;
}


template
    < class Iter
    , class T
    , CONCEPT = cc::Divide<cc::iter_ref<Iter>, T>>
auto operator /(const core::iterator_range<Iter>& lhs, T rhs)
{
    return map(lhs, math::divides(rhs));
}

template
    < class T
    , class Iter
    , CONCEPT = cc::Divide<T, cc::iter_ref<Iter>>>
auto operator /(T lhs, const core::iterator_range<Iter>& rhs)
{
    return map(rhs, math::divides.bind_left(lhs));
}


template <class Iter>
auto operator +(const core::iterator_range<Iter>& item)
{
    return item;
}

template <class Iter>
auto operator -(const core::iterator_range<Iter>& item)
{
    return map(item, math::negate());
}

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ELEMENTWISE_HPP_ */
