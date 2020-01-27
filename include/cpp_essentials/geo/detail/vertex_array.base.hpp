#ifndef CPP_ESSENTIALS_GEO_DETAIL_VERTEX_ARRAY_BASE_HPP_
#define CPP_ESSENTIALS_GEO_DETAIL_VERTEX_ARRAY_BASE_HPP_

#pragma once

#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/geo/matrix.hpp>
#include <cpp_essentials/core/output.hpp>

#include <vector>
#include <array>

namespace cpp_essentials::geo
{

namespace detail
{

struct polygon_tag { };
struct polyline_tag { };

} /* namespace detail */

template <class T, size_t D, size_t N, class Tag>
class vertex_array
{
public:
    using vector_type = vector<T, D>;
    using value_type = vector_type;
    using size_type = std::size_t;

    using const_reference = const vector_type&;
    using reference = vector_type&;

    using data_type = std::conditional_t<N == 0, std::vector<value_type>, std::array<value_type, N>>;

    vertex_array()
    {
        core::fill(_data, vector_type{});
    }

    vertex_array(size_type size)
    {
        resize(size);
        core::fill(_data, vector_type{});
    }

    vertex_array(std::initializer_list<vector_type> init)
    {
        resize(init.size());
        core::copy(init, _data.begin());
    }

    vertex_array(data_type data)
        : _data{ std::move(data) }
    {
    }

    template <class U>
    vertex_array(std::initializer_list<vector<U, D>> init)
    {
        resize(init.size());
        core::copy(init, _data.begin());
    }

    template <class U>
    vertex_array(const vertex_array<U, D, N, Tag>& other)
    {
        resize(other.size());
        core::copy(other._data, _data.begin());
    }

    template <class U, size_t M, CONCEPT = std::enable_if_t<N == 0>>
    vertex_array(const vertex_array<U, D, M, Tag>& other)
    {
        resize(other.size());
        core::copy(other._data, _data.begin());
    }


    template <class U>
    vertex_array<U, D, N, Tag> as() const
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

    void resize(size_type size)
    {
        if constexpr (N == 0)
        {
            _data.resize(size);
        }
        else
        {
            EXPECTS(size == _data.size());
        }
    }

    data_type _data;
};


template <class T, size_t D>
using triangle = vertex_array<T, D, 3, detail::polygon_tag>;

template <class T, size_t D>
using quad = vertex_array<T, D, 4, detail::polygon_tag>;


template <class T>
using triangle_2d = triangle<T, 2>;

template <class T>
using quad_2d = quad<T, 2>;

template <class T, size_t D>
using polygon = vertex_array<T, D, 0, detail::polygon_tag>;

template <class T, size_t D>
using polyline = vertex_array<T, D, 0, detail::polyline_tag>;


template <class T>
using polygon_2d = polygon<T, 2>;

template <class T>
using polyline_2d = polyline<T, 2>;


template <class T, size_t D, size_t N, class Tag>
std::ostream& operator <<(std::ostream& os, const vertex_array<T, D, N, Tag>& item)
{
    return os << "{ " << core::delimit(item._data, " ") << " }";
}

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DETAIL_VERTEX_ARRAY_BASE_HPP_ */
