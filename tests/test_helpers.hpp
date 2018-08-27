#pragma once

#include <vector>

template <class T>
void insert(std::vector<T>& result, const T& value)
{
    result.push_back(value);
}

template <class T, class... Args>
void insert(std::vector<T>& result, const T& value, const Args&... args)
{
    result.push_back(value);
    insert(result, args...);
}

template <class T, class... Args>
std::vector<T> vec(const T& value, const Args&... args)
{
    std::vector<T> result;
    insert(result, value, args...);
    return result;
}

template <class T>
std::vector<T> empty_vec()
{
    return {};
}

template <class T>
struct vector_builder : std::vector<T>
{
    using iterator_category = std::output_iterator_tag;

    vector_builder& operator *()
    {
        return *this;
    }

    vector_builder& operator ++()
    {
        return *this;
    }

    vector_builder& operator ++(int)
    {
        return *this;
    }

    vector_builder& operator =(const T& item)
    {
        std::vector<T>::push_back(item);
        return *this;
    }

    static std::vector<T> empty()
    {
        return {};
    }
};
