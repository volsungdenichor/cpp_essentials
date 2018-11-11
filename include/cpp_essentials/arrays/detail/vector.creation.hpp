#ifndef CPP_ESSENTIALS_ARRAYS_DETAIL_VECTOR_CREATION_HPP_
#define CPP_ESSENTIALS_ARRAYS_DETAIL_VECTOR_CREATION_HPP_

#pragma once

#include <cpp_essentials/geo/matrix.hpp>
#include <cpp_essentials/geo/bounding_box.hpp>

namespace cpp_essentials::arrays
{

using byte = unsigned char;
using byte_pointer = byte*;

template <class T, size_t D>
using vector = geo::vector<T, D>;

using size_value_type = std::size_t;
using stride_value_type = std::ptrdiff_t;
using location_value_type = stride_value_type;

namespace detail
{

template <class T, size_t D>
auto insert(const vector<T, D>& item, size_t index, T value) -> vector<T, D + 1>
{
    vector<T, D + 1> result;

    std::copy(item._data.begin(), item._data.begin() + index, result._data.begin());

    result[index] = value;

    std::copy(item._data.begin() + index, item._data.end(), result._data.begin() + index + 1);

    return result;
}

template <size_t Index, class T, size_t D>
auto insert(const vector<T, D>& item, T value) -> vector<T, D + 1>
{
    static_assert(Index <= D, "insert: invalid index");
    return insert(item, Index, value);
}

template <class T, size_t D>
auto push_front(const vector<T, D>& item, T value) -> vector<T, D + 1>
{
    return insert<0>(item, value);
}

template <class T, size_t D>
auto push_back(const vector<T, D>& item, T value) -> vector<T, D + 1>
{
    return insert<D>(item, value);
}

template <class T, size_t D>
auto erase(const vector<T, D>& item, size_t index) -> vector<T, D - 1>
{
    vector<T, D - 1> result;

    std::copy(item._data.begin(), item._data.begin() + index, result._data.begin());
    std::copy(item._data.begin() + index + 1, item._data.end(), result._data.begin() + index);

    return result;
}

template <size_t Index, class T, size_t D>
auto erase(const vector<T, D>& item) -> vector<T, D - 1>
{
    static_assert(Index < D, "erase: invalid index");
    return erase(item, Index);
}

template <class T, size_t D>
auto pop_front(const vector<T, D>& item) -> vector<T, D - 1>
{
    return erase<0>(item);
}

template <class T, size_t D>
auto pop_back(const vector<T, D>& item) -> vector<T, D - 1>
{
    return erase<D - 1>(item);
}

template <class T, size_t D>
auto reverse(const vector<T, D>& item) -> vector<T, D>
{
    vector<T, D> result;
    std::copy(item._data.begin(), item._data.end(), std::make_reverse_iterator(result._data.end()));
    return result;
}

} /* namespace detail */

} /* namespace cpp_essentials::arrays */

#endif /* CPP_ESSENTIALS_ARRAYS_DETAIL_VECTOR_CREATION_HPP_ */
