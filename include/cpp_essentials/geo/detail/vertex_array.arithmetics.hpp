#ifndef CPP_ESSENTIALS_GEO_DETAIL_VERTEX_ARRAY_ARITHMETICS_HPP_
#define CPP_ESSENTIALS_GEO_DETAIL_VERTEX_ARRAY_ARITHMETICS_HPP_

#pragma once

#include <cpp_essentials/geo/detail/vertex_array.base.hpp>

namespace cpp_essentials::geo
{

template <class T, size_t D, size_t N, class Tag>
auto operator +(const vertex_array<T, D, N, Tag>& value) -> vertex_array<T, D, N, Tag>
{
    return value;
}

template <class T, size_t D, size_t N, class Tag>
auto operator -(const vertex_array<T, D, N, Tag>& value) -> vertex_array<T, D, N, Tag>
{
    vertex_array<T, D, N, Tag> result(value.size());
    core::transform(value._data, result._data.begin(), core::negate);
    return result;
}


template
    < class T
    , class U
    , size_t D
    , size_t N
    , class Tag
    , class = cc::Add<T, U>>
auto& operator +=(vertex_array<T, D, N, Tag>& lhs, const vector<U, D>& rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::plus(rhs));
    return lhs;
}

template
    < class T
    , class U
    , size_t D
    , size_t N
    , class Tag
    , class = cc::Add<T, U>>
auto operator +(const vertex_array<T, D, N, Tag>& lhs, const vector<U, D>& rhs) -> vertex_array<cc::Add<T, U>, D, N, Tag>
{
    vertex_array<cc::Add<T, U>, D, N, Tag> result(lhs.size());
    core::transform(lhs._data, result._data.begin(), core::plus(rhs));
    return result;
}


template
    < class T
    , class U
    , size_t D
    , size_t N
    , class Tag
    , class = cc::Subtract<T, U>>
auto& operator -=(vertex_array<T, D, N, Tag>& lhs, const vector<U, D>& rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::minus(rhs));
    return lhs;
}

template
    < class T
    , class U
    , size_t D
    , size_t N
    , class Tag
    , class = cc::Subtract<T, U>>
auto operator -(const vertex_array<T, D, N, Tag>& lhs, const vector<U, D>& rhs) -> vertex_array<cc::Subtract<T, U>, D, N, Tag>
{
    vertex_array<cc::Subtract<T, U>, D, N, Tag> result(lhs.size());
    core::transform(lhs._data, result._data.begin(), core::minus(rhs));
    return result;
}


template
    < class T
    , class U
    , size_t D
    , size_t N
    , class Tag
    , class = cc::Multiply<T, U>>
auto& operator *=(vertex_array<T, D, N, Tag>& lhs, const square_matrix<U, D>& rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::multiplies(rhs));
    return lhs;
}

template
    < class T
    , class U
    , size_t D
    , size_t N
    , class Tag
    , class = cc::Multiply<T, U>>
auto operator *(const vertex_array<T, D, N, Tag>& lhs, const square_matrix<U, D>& rhs) -> vertex_array<cc::Multiply<T, U>, D, N, Tag>
{
    vertex_array<cc::Multiply<T, U>, D, N, Tag> result(lhs.size());
    core::transform(lhs._data, result._data.begin(), core::multiplies(rhs));
    return result;
}

template
    < class T
    , class U
    , size_t D
    , size_t N
    , class Tag
    , class = cc::Multiply<T, U>>
auto operator *(const square_matrix<T, D>& lhs, const vertex_array<U, D, N, Tag>& rhs) -> vertex_array<cc::Multiply<T, U>, D, N, Tag>
{
    return rhs * lhs;
}


template
    < class T
    , class U
    , size_t D
    , size_t N
    , class Tag
    , class = cc::Multiply<T, U>>
auto& operator *=(vertex_array<T, D, N, Tag>& lhs, const square_matrix<U, D + 1>& rhs)
{
    core::transform(lhs, lhs._data.begin(), core::multiplies(rhs));
    return lhs;
}

template
    < class T
    , class U
    , size_t D
    , size_t N
    , class Tag
    , class = cc::Multiply<T, U>>
auto operator *(const vertex_array<T, D, N, Tag>& lhs, const square_matrix<U, D + 1>& rhs) -> vertex_array<cc::Multiply<T, U>, D, N, Tag>
{
    vertex_array<cc::Multiply<T, U>, D, N, Tag> result(lhs.size());
    core::transform(lhs._data, result._data.begin(), core::multiplies(rhs));
    return result;
}

template
    < class T
    , class U
    , size_t D
    , size_t N
    , class Tag
    , class = cc::Multiply<T, U>>
auto operator *(const square_matrix<T, D + 1>& lhs, const vertex_array<U, D, N, Tag>& rhs) -> vertex_array<cc::Multiply<T, U>, D, N, Tag>
{
    return rhs * lhs;
}


template <class T, class U, size_t D, size_t N, class Tag>
bool operator ==(const vertex_array<T, D, N, Tag>& lhs, const vertex_array<U, D, N, Tag>& rhs)
{
    return core::equal(lhs._data, rhs._data);
}

template <class T, class U, size_t D, size_t N, class Tag>
bool operator !=(const vertex_array<T, D, N, Tag>& lhs, const vertex_array<U, D, N, Tag>& rhs)
{
    return !(lhs == rhs);
}

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DETAIL_VERTEX_ARRAY_ARITHMETICS_HPP_ */
