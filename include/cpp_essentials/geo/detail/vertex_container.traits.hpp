#ifndef CPP_ESSENTIALS_GEO_DETAIL_VERTEX_ARRAY_TRAITS_HPP_
#define CPP_ESSENTIALS_GEO_DETAIL_VERTEX_ARRAY_TRAITS_HPP_

#pragma once

#include <cpp_essentials/geo/detail/vertex_array.base.hpp>
#include <cpp_essentials/geo/detail/linear_shape.base.hpp>
#include <cpp_essentials/geo/detail/poly_shape.base.hpp>
#include <cpp_essentials/geo/bounding_box.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

template <class Shape>
struct vertex_container_traits { };

template <class Shape>
using vertex_type = typename vertex_container_traits<Shape>::vertex_type;

template <class Shape>
using segment_type = typename vertex_container_traits<Shape>::segment_type;


template <class T, size_t D>
struct vertex_container_traits<segment<T, D>>
{
    using container_type = segment<T, D>;

    using value_type = T;

    using vertex_type = vector<T, D>;

    using segment_type = segment<T, D>;

    static size_t vertex_count(const container_type& container)
    {
        return 2;
    }

    static vertex_type get_vertex(const container_type& container, size_t index)
    {
        return container[index];
    }

    static size_t segment_count(const container_type& container)
    {
        return 1;
    }

    static segment_type get_segment(const container_type& container, size_t index)
    {
        return container;
    }
};

template <class T, size_t D>
struct vertex_container_traits<polygon<T, D>>
{
    using container_type = polygon<T, D>;

    using value_type = T;

    using vertex_type = vector<T, D>;

    using segment_type = segment<T, D>;

    static size_t vertex_count(const container_type& container)
    {
        return container.size();
    }

    static vertex_type get_vertex(const container_type& container, size_t index)
    {
        return container._data[index];
    }

    static size_t segment_count(const container_type& container)
    {
        return container.size();
    }

    static segment_type get_segment(const container_type& container, size_t index)
    {
        return { container._data[index], container._data[(index + 1) % container.size()] };
    }
};



template <class T, size_t D>
struct vertex_container_traits<polyline<T, D>>
{
    using container_type = polyline<T, D>;

    using value_type = T;

    using vertex_type = vector<T, D>;

    using segment_type = segment<T, D>;

    static size_t vertex_count(const container_type& container)
    {
        return container.size();
    }

    static vertex_type get_vertex(const container_type& container, size_t index)
    {
        return container._data[index];
    }

    static size_t segment_count(const container_type& container)
    {
        return container.size() - 1;
    }

    static segment_type get_segment(const container_type& container, size_t index)
    {
        return { container._data[index], container._data[(index + 1) % container.size()] };
    }
};

template <class T, size_t D, size_t N>
struct vertex_container_traits<vertex_array<T, D, N>>
{
    using container_type = vertex_array<T, D, N>;

    using value_type = T;

    using vertex_type = vector<T, D>;

    using segment_type = segment<T, D>;

    static size_t vertex_count(const container_type&)
    {
        return N;
    }

    static vertex_type get_vertex(const container_type& container, size_t index)
    {
        return container._data[index];
    }

    static size_t segment_count(const container_type&)
    {
        return N;
    }

    static segment_type get_segment(const container_type& container, size_t index)
    {
        return { container[index], container[(index + 1) % N] };
    }
};

template <class T>
struct vertex_container_traits<rect<T>>
{
    using container_type = rect<T>;

    using value_type = T;

    using vertex_type = vector<T, 2>;

    using segment_type = segment<T, 2>;

    static size_t vertex_count(const container_type&)
    {
        return 4;
    }

    static vertex_type get_vertex(const container_type& container, size_t index)
    {
        switch (index)
        {
            case 0:
                return top_left(container);
            case 1:
                return top_right(container);
            case 2:
                return bottom_right(container);
            case 3:
                return bottom_left(container);

            default:
                throw std::invalid_argument{ "index" };
        }
    }

    static size_t segment_count(const container_type&)
    {
        return 4;
    }

    static segment_type get_segment(const container_type& container, size_t index)
    {
        return { get_vertex(container, index), get_vertex(container, (index + 1) % 4) };
    }
};

} /* namespace detail */

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DETAIL_VERTEX_ARRAY_TRAITS_HPP_ */
