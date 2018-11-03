#ifndef GEO_VECTOR_CREATION_HPP_
#define GEO_VECTOR_CREATION_HPP_

#include <geo/matrix.hpp>
#include <geo/bounding_box.hpp>

namespace geo
{

template <class T, size_t D>
auto insert(const vector<T, D>& item, size_t index, T value) -> vector<T, D + 1>
{
    vector<T, D + 1> result;

    std::copy(item.begin(), item.begin() + index, result.begin());

    result[index] = value;

    std::copy(item.begin() + index, item.end(), result.begin() + index + 1);

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

    std::copy(item.begin(), item.begin() + index, result.begin());
    std::copy(item.begin() + index + 1, item.end(), result.begin() + index);

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

    std::copy(item.begin(), item.end(), std::make_reverse_iterator(result.end()));

    return result;
}

} /* namespace geo */

#endif /* GEO_VECTOR_CREATION_HPP_ */
