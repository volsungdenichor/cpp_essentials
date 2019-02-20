#ifndef CPP_ESSENTIALS_MATH_DETAIL_MATRIX_ACCESS_HPP_
#define CPP_ESSENTIALS_MATH_DETAIL_MATRIX_ACCESS_HPP_

#pragma once

#include <cpp_essentials/math/detail/matrix.base.hpp>

namespace cpp_essentials::math
{

template <class T, size_t R, size_t C>
const auto& get(const matrix<T, R, C>& item, size_t r, size_t c)
{
    return item(r, c);
}

template <class T, size_t R, size_t C>
auto& get(matrix<T, R, C>& item, size_t r, size_t c)
{
    return item(r, c);
}



template <class T, size_t R, size_t C>
const auto& get(const matrix<T, R, C>& item, size_t index)
{
    return item[index];
}

template <class T, size_t R, size_t C>
auto& get(matrix<T, R, C>& item, size_t index)
{
    return item[index];
}



template <size_t Row, size_t Col, class T, size_t R, size_t C>
const auto& get(const matrix<T, R, C>& item)
{
    static_assert(Row < R, "matrix get: invalid row");
    static_assert(Col < C, "matrix get: invalid col");

    static const size_t index = Row * C + Col;

    return std::get<index>(item._data);
}

template <size_t Row, size_t Col, class T, size_t R, size_t C>
auto& get(matrix<T, R, C>& item)
{
    static_assert(Row < R, "matrix get: invalid row");
    static_assert(Col < C, "matrix get: invalid col");

    static const size_t index = Row * C + Col;

    return std::get<index>(item._data);
}



template <size_t Index, class T, size_t R, size_t C>
const auto& get(const matrix<T, R, C>& item)
{
    static_assert(Index < (R * C), "matrix get: invalid index");

    return std::get<Index>(item._data);
}

template <size_t Index, class T, size_t R, size_t C>
auto& get(matrix<T, R, C>& item)
{
    static_assert(Index < (R * C), "matrix get: invalid index");

    return std::get<Index>(item._data);
}

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_DETAIL_MATRIX_ACCESS_HPP_ */