#ifndef CPP_ESSENTIALS_GEO_DETAIL_VERTEX_ARRAY_BASE_HPP_
#define CPP_ESSENTIALS_GEO_DETAIL_VERTEX_ARRAY_BASE_HPP_

#pragma once

#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/geo/matrix.hpp>
#include <cpp_essentials/core/output.hpp>

namespace cpp_essentials::geo
{

template <class T, size_t D, size_t N>
class vertex_array
{
public:
    using vector_type = vector<T, D>;
    using value_type = vector_type;
    using size_type = std::size_t;

    using const_reference = const vector_type&;
    using reference = vector_type&;

    using const_iterator = typename std::array<vector_type, N>::const_iterator;
    using iterator = typename std::array<vector_type, N>::iterator;

    vertex_array()
    {
        core::fill(_data, vector_type {});
    }

    vertex_array(std::initializer_list<vector_type> init)
    {
        EXPECTS(init.size() == N, "invalid initializer size");

        core::copy(init, _data.begin());
    }

    template <class U>
    vertex_array(std::initializer_list<vector<U, D>> init)
    {
        EXPECTS(init.size() == N, "invalid initializer size");
        
        core::copy(init, _data.begin());
    }

    template <class U>
    vertex_array(const vertex_array<U, D, N>& other)
    {
        core::copy(other._data, _data.begin());
    }


    template <class U>
    vertex_array<U, D, N> as() const
    {
        return *this;
    }


    size_type size() const
    {
        return _data.size();
    }


    const_reference operator [](size_type index) const
    {
        return _data[index];
    }

    reference operator [](size_type index)
    {
        return _data[index];
    }

    std::array<vector_type, N> _data;
};



template <class T, size_t D>
using triangle = vertex_array<T, D, 3>;

template <class T, size_t D>
using quad = vertex_array<T, D, 4>;


template <class T>
using triangle_2d = triangle<T, 2>;

template <class T>
using quad_2d = quad<T, 2>;


template <class T, size_t D, size_t N>
std::ostream& operator <<(std::ostream& os, const vertex_array<T, D, N>& item)
{
    return os << "{ " << core::delimit(item._data, " ") << " }";
}

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DETAIL_VERTEX_ARRAY_BASE_HPP_ */
