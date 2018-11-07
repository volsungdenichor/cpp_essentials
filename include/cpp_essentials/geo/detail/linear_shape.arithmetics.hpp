#ifndef CPP_ESSENTIALS_GEO_DETAIL_LINEAR_SHAPE_ARITHMETICS_HPP_
#define CPP_ESSENTIALS_GEO_DETAIL_LINEAR_SHAPE_ARITHMETICS_HPP_

#pragma once

#include <cpp_essentials/geo/detail/linear_shape.base.hpp>

namespace cpp_essentials::geo
{

template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Add<T, U>>
auto& operator +=(linear_shape<T, D, Tag>& lhs, const vector<U, D>& rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::plus(rhs));
    return lhs;
}

template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Add<T, U>>
auto operator +(const linear_shape<T, D, Tag>& lhs, const vector<U, D>& rhs) -> linear_shape<cc::Add<T, U>, D, Tag>
{
    linear_shape<cc::Add<T, U>, D, Tag> result;
    core::transform(lhs._data, result._data.begin(), core::plus(rhs));
    return result;
}

template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Add<T, U>>
auto operator +(const vector<T, D>& lhs, const linear_shape<U, D, Tag>& rhs) -> linear_shape<cc::Add<T, U>, D, Tag>
{
    return rhs + lhs;
}



template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Subtract<T, U>>
auto& operator -=(linear_shape<T, D, Tag>& lhs, const vector<U, D>& rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::minus(rhs));
    return lhs;
}

template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Subtract<T, U>>
auto operator -(const linear_shape<T, D, Tag>& lhs, const vector<U, D>& rhs) -> linear_shape<cc::Subtract<T, U>, D, Tag>
{
    linear_shape<cc::Subtract<T, U>, D, Tag> result;
    core::transform(lhs._data, result._data.begin(), core::minus(rhs));
    return result;
}



template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Multiply<T, U>>
auto& operator *=(linear_shape<T, D, Tag>& lhs, const square_matrix<U, D>& rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::multiplies(rhs));
    return lhs;
}

template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(const linear_shape<T, D, Tag>& lhs, const square_matrix<U, D>& rhs) -> linear_shape<cc::Multiply<T, U>, D, Tag>
{
    linear_shape<cc::Multiply<T, U>, D, Tag> result;
    core::transform(lhs._data, result._data.begin(), core::multiplies(rhs));
    return result;
}

template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(const square_matrix<U, D>& lhs, const linear_shape<T, D, Tag>& rhs) -> linear_shape<cc::Multiply<T, U>, D, Tag>
{
    return rhs * lhs;
}



template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Multiply<T, U>>
auto& operator *=(linear_shape<T, D, Tag>& lhs, const square_matrix<U, D + 1>& rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::multiplies(rhs));
    return lhs;
}

template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(const linear_shape<T, D, Tag>& lhs, const square_matrix<U, D + 1>& rhs) -> linear_shape<cc::Multiply<T, U>, D, Tag>
{
    linear_shape<cc::Multiply<T, U>, D, Tag> result;
    core::transform(lhs._data, result._data.begin(), core::multiplies(rhs));
    return result;
}

template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(const square_matrix<U, D + 1>& lhs, const linear_shape<T, D, Tag>& rhs) -> linear_shape<cc::Multiply<T, U>, D, Tag>
{
    return rhs * lhs;
}


template <class T, class U, size_t D, class Tag>
bool operator ==(const linear_shape<T, D, Tag>& lhs, const linear_shape<U, D, Tag>& rhs)
{
    return core::equal(lhs._data, rhs._data);
}

template <class T, class U, size_t D, class Tag>
bool operator !=(const linear_shape<T, D, Tag>& lhs, const linear_shape<U, D, Tag>& rhs)
{
    return !(lhs == rhs);
}

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DETAIL_LINEAR_SHAPE_ARITHMETICS_HPP_ */