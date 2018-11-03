#ifndef CPP_ESSENTIALS_MATH_DETAIL_MATRIX_ARITHMETICS_HPP_
#define CPP_ESSENTIALS_MATH_DETAIL_MATRIX_ARITHMETICS_HPP_

#pragma once

#include <cpp_essentials/core/arithmetic_functors.hpp>
#include <cpp_essentials/math/detail/matrix.base.hpp>

namespace cpp_essentials::math
{

template <class T, size_t R, size_t C>
auto operator +(const matrix<T, R, C>& item) -> matrix<T, R, C>
{
    return item;
}

template <class T, size_t R, size_t C>
auto operator -(const matrix<T, R, C>& item) -> matrix<T, R, C>
{
    matrix<T, R, C> result;
    core::transform(item._data, result._data.begin(), core::negate);
    return result;
}


template
    < class T
    , class U
    , size_t R
    , size_t C
    , CONCEPT = cc::Add<T, U>>
auto& operator +=(matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs)
{
    core::transform(lhs._data, rhs._data, lhs._data.begin(), core::plus);
    return lhs;
}

template
    < class T
    , class U
    , size_t R
    , size_t C
    , CONCEPT = cc::Add<T, U>>
auto operator +(const matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs) -> matrix<cc::Add<T, U>, R, C>
{
    matrix<cc::Add<T, U>, R, C> result;
    core::transform(lhs._data, rhs._data, result._data.begin(), core::plus);
    return result;
}


template
    < class T
    , class U
    , size_t R
    , size_t C
    , CONCEPT = cc::Subtract<T, U>>
auto& operator -=(matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs)
{
    core::transform(lhs._data, rhs._data, lhs._data.begin(), core::minus);
    return lhs;
}

template
    < class T
    , class U
    , size_t R
    , size_t C
    , CONCEPT = cc::Subtract<T, U>>
auto operator -(const matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs) -> matrix<cc::Subtract<T, U>, R, C>
{
    matrix<cc::Subtract<T, U>, R, C> result;
    core::transform(lhs._data, rhs._data, result._data.begin(), core::minus);
    return result;
}


template
    < class T
    , class U
    , size_t R
    , size_t C
    , CONCEPT = cc::Multiply<T, U>>
auto& operator *=(matrix<T, R, C>& lhs, U rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::multiplies(rhs));
    return lhs;
}

template
    < class T
    , class U
    , size_t R
    , size_t C
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(const matrix<T, R, C>& lhs, U rhs) -> matrix<cc::Multiply<T, U>, R, C>
{
    matrix<cc::Multiply<T, U>, R, C> result;
    core::transform(lhs._data, result._data.begin(), core::multiplies(rhs));
    return result;
}

template
    < class T
    , class U
    , size_t R
    , size_t C
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(T lhs, const matrix<U, R, C>& rhs) -> matrix<cc::Multiply<U, T>, R, C>
{
    return rhs * lhs;
}


template
    < class T
    , class U
    , size_t R
    , size_t C
    , CONCEPT = cc::Divide<T, U>>
auto& operator /=(matrix<T, R, C>& lhs, U rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::divides(rhs));
    return lhs;
}

template
    < class T
    , class U
    , size_t R
    , size_t C
    , CONCEPT = cc::Divide<T, U>>
auto operator /(const matrix<T, R, C>& lhs, U rhs) -> matrix<cc::Divide<T, U>, R, C>
{
    matrix<cc::Divide<T, U>, R, C> result;
    core::transform(lhs._data, result._data.begin(), core::divides(rhs));
    return result;
}


template
    < class T
    , class U
    , size_t R
    , size_t D
    , size_t C
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(const matrix<T, R, D>& lhs, const matrix<U, D, C>& rhs) -> matrix<cc::Multiply<T, U>, R, C>
{
    using result_type = matrix<cc::Multiply<T, U>, R, C>;

    result_type result;

    for (size_t r = 0; r < lhs.row_count(); ++r)
    {
        for (size_t c = 0; c < rhs.col_count(); ++c)
        {
            typename result_type::value_type sum = {};

            for (size_t i = 0; i < D; ++i)
            {
                sum += lhs(r, i) * rhs(i, c);
            }
            
            result(r, c) = sum;
        }
    }

    return result;
}

template
    < class T
    , class U
    , size_t D
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(const vector<T, D>& lhs, const square_matrix<U, D + 1>& rhs) -> vector<cc::Multiply<T, U>, D>
{
    using result_type = vector<cc::Multiply<T, U>, D>;

    result_type result;

    for (size_t d = 0; d < lhs.size(); ++d)
    {
        auto sum = static_cast<typename result_type::value_type>(rhs(D, d));

        for (size_t i = 0; i < D; ++i)
        { 
            sum += lhs[i] * rhs(i, d);
        }
        
        result[d] = sum;
    }

    return result;
}

template
    < class T
    , class U
    , size_t D
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(const square_matrix<T, D + 1>& lhs, const vector<U, D>& rhs) -> vector<cc::Multiply<T, U>, D>
{
    return rhs * lhs;
}

template
    < class T
    , class U
    , size_t D
    , CONCEPT = cc::Multiply<T, U>>
auto& operator *=(vector<T, D>& lhs, const square_matrix<U, D + 1>& rhs)
{
    return lhs = lhs * rhs;   
}


template <class T, class U, size_t R, size_t C>
bool operator ==(const matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs)
{
    return core::equal(lhs._data, rhs._data);
}

template <class T, class U, size_t R, size_t C>
bool operator !=(const matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs)
{
    return !(lhs == rhs);
}

namespace detail
{

struct elementwise_multiply_t
{
    template
        < class T
        , class U
        , size_t R
        , size_t C
        , CONCEPT = cc::Multiply<T, U>>
    auto operator ()(const matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs) const -> matrix<cc::Multiply<T, U>, R, C>
    {
        matrix<cc::Multiply<T, U>, R, C> result;
        core::transform(lhs._data, rhs._data, result._data.begin(), core::multiplies);
        return result;
    }
};

struct elementwise_divide_t
{
    template
        < class T
        , class U
        , size_t R
        , size_t C
        , CONCEPT = cc::Divide<T, U>>
    auto operator ()(const matrix<T, R, C>& lhs, const matrix<U, R, C>& rhs) const -> matrix<cc::Divide<T, U>, R, C>
    {
        matrix<cc::Divide<T, U>, R, C> result;
        core::transform(lhs._data, rhs._data, result._data.begin(), core::divides);
        return result;
    }
};

} /* namespace detail */

static constexpr detail::elementwise_multiply_t elementwise_multiply = {};
static constexpr detail::elementwise_divide_t elementwise_divide = {};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_DETAIL_MATRIX_ARITHMETICS_HPP_ */