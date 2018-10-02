#ifndef CPP_ESSENTIALS_GEO_DETAIL_MATRIX_ACCESS_HPP_
#define CPP_ESSENTIALS_GEO_DETAIL_MATRIX_ACCESS_HPP_

#include <cpp_essentials/geo/detail/matrix.base.hpp>

namespace cpp_essentials::geo
{

template <class T, std::size_t R, std::size_t C>
const auto& get(const matrix<T, R, C>& item, std::size_t r, std::size_t c)
{
    return item(r, c);
}

template <class T, std::size_t R, std::size_t C>
auto& get(matrix<T, R, C>& item, std::size_t r, std::size_t c)
{
    return item(r, c);
}


template <class T, std::size_t R, std::size_t C>
const auto& get(const matrix<T, R, C>& item, std::size_t index)
{
    return item[index];
}

template <class T, std::size_t R, std::size_t C>
auto& get(matrix<T, R, C>& item, std::size_t index)
{
    return item[index];
}


template <size_t Row, std::size_t Col, class T, std::size_t R, std::size_t C>
const auto& get(const matrix<T, R, C>& item)
{
    static_assert(Row < R, "matrix get: invalid row");
    static_assert(Col < C, "matrix get: invalid col");

    static const std::size_t index = Row * C + Col;

    return std::get<index>(item._data);
}

template <size_t Row, std::size_t Col, class T, std::size_t R, std::size_t C>
auto& get(matrix<T, R, C>& item)
{
    static_assert(Row < R, "matrix get: invalid row");
    static_assert(Col < C, "matrix get: invalid col");

    static const std::size_t index = Row * C + Col;

    return std::get<index>(item._data);
}


template <size_t Index, class T, std::size_t R, std::size_t C>
const auto& get(const matrix<T, R, C>& item)
{
    static_assert(Index < (R * C), "matrix get: invalid index");

    return std::get<Index>(item._data);
}

template <size_t Index, class T, std::size_t R, std::size_t C>
auto& get(matrix<T, R, C>& item)
{
    static_assert(Index < (R * C), "matrix get: invalid index");

    return std::get<Index>(item._data);
}

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DETAIL_MATRIX_ACCESS_HPP_ */