#ifndef CPP_ESSENTIALS_GEO_DETAIL_POLY_SHAPE_ARITHMETICS_HPP_
#define CPP_ESSENTIALS_GEO_DETAIL_POLY_SHAPE_ARITHMETICS_HPP_

#include <cpp_essentials/geo/detail/poly_shape.base.hpp>

namespace cpp_essentials::geo
{

template <class T, size_t D, class Tag>
auto operator +(const poly_shape_base<T, D, Tag>& value) -> poly_shape_base<T, D, Tag>
{
    return value;
}

template <class T, size_t D, class Tag>
auto operator -(const poly_shape_base<T, D, Tag>& value) -> poly_shape_base<T, D, Tag>
{
    poly_shape_base<T, D, Tag> result;
    core::transform(value._data, std::back_inserter(result._data), core::negate);
    return result;
}



template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Add<T, U>>
auto& operator +=(poly_shape_base<T, D, Tag>& lhs, const vector<U, D>& rhs)
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
auto operator +(const poly_shape_base<T, D, Tag>& lhs, const vector<U, D>& rhs) -> poly_shape_base<cc::Add<T, U>, D, Tag>
{
    poly_shape_base<cc::Add<T, U>, D, Tag> result;
    core::transform(lhs._data, std::back_inserter(result._data), core::plus(rhs));
    return result;
}



template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Subtract<T, U>>
auto& operator -=(poly_shape_base<T, D, Tag>& lhs, const vector<U, D>& rhs)
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
auto operator -(const poly_shape_base<T, D, Tag>& lhs, const vector<U, D>& rhs) -> poly_shape_base<cc::Subtract<T, U>, D, Tag>
{
    poly_shape_base<cc::Subtract<T, U>, D, Tag> result;
    core::transform(lhs._data, std::back_inserter(result._data), core::minus(rhs));
    return result;
}



template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Multiply<T, U>>
auto& operator *=(poly_shape_base<T, D, Tag>& lhs, const square_matrix<U, D>& rhs)
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
auto operator *(const poly_shape_base<T, D, Tag>& lhs, const square_matrix<U, D>& rhs) -> poly_shape_base<cc::Multiply<T, U>, D, Tag>
{
    poly_shape_base<cc::Multiply<T, U>, D, Tag> result;
    core::transform(lhs._data, std::back_inserter(result._data), core::multiplies(rhs));
    return result;
}

template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(const square_matrix<T, D>& lhs, const poly_shape_base<U, D, Tag>& rhs) -> poly_shape_base<cc::Multiply<T, U>, D, Tag>
{
    return rhs * lhs;
}



template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Multiply<T, U>>
auto& operator *=(poly_shape_base<T, D, Tag>& lhs, const square_matrix<U, D + 1>& rhs)
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
auto operator *(const poly_shape_base<T, D, Tag>& lhs, const square_matrix<U, D + 1>& rhs) -> poly_shape_base<cc::Multiply<T, U>, D, Tag>
{
    poly_shape_base<cc::Multiply<T, U>, D, Tag> result;
    core::transform(lhs._data, std::back_inserter(result._data), core::multiplies(rhs));
    return result;
}

template
    < class T
    , class U
    , size_t D
    , class Tag
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(const square_matrix<T, D + 1>& lhs, const poly_shape_base<U, D, Tag>& rhs) -> poly_shape_base<cc::Multiply<T, U>, D, Tag>
{
    return rhs * lhs;
}

template <class T, class U, size_t D, class Tag>
bool operator ==(const poly_shape_base<T, D, Tag>& lhs, const poly_shape_base<U, D, Tag>& rhs)
{
    return core::equal(lhs._data, rhs._data);
}

template <class T, class U, size_t D, class Tag>
bool operator !=(const poly_shape_base<T, D, Tag>& lhs, const poly_shape_base<U, D, Tag>& rhs)
{
    return !(lhs == rhs);
}

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DETAIL_POLY_SHAPE_ARITHMETICS_HPP_ */
