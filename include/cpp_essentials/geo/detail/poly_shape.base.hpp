#ifndef CPP_ESSENTIALS_GEO_DETAIL_POLY_SHAPE_BASE_HPP_
#define CPP_ESSENTIALS_GEO_DETAIL_POLY_SHAPE_BASE_HPP_

#pragma once

#include <vector>

#include <cpp_essentials/geo/matrix.hpp>
#include <cpp_essentials/core/output.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct polygon_tag { };
struct polyline_tag { };

} /* namespace detail */

template <class T, size_t D, class Tag>
class poly_shape_base
{
public:
    using vector_type = vector<T, D>;
    using value_type = vector_type;   

    using const_reference = const vector_type&;
    using reference = vector_type&;

    poly_shape_base() = default;

    poly_shape_base(std::initializer_list<vector_type> init)
        : _data{ init }
    {
    }

    template <class Iter, CONCEPT = cc::InputIterator<Iter>>
    poly_shape_base(Iter begin, Iter end)
        : _data{ begin, end }
    {
    }

    template <class Iter, CONCEPT = cc::InputIterator<Iter>>
    poly_shape_base(core::iterator_range<Iter> range)
        : _data{ range }
    {
    }

    poly_shape_base(const poly_shape_base&) = default;
    poly_shape_base(poly_shape_base&&) = default;

    size_t size() const
    {
        return _data.size();
    }

    std::vector<vector_type> _data;
};


template <class T, size_t D>
using polygon = poly_shape_base<T, D, detail::polygon_tag>;

template <class T, size_t D>
using polyline = poly_shape_base<T, D, detail::polyline_tag>;


template <class T>
using polygon_2d = polygon<T, 2>;

template <class T>
using polyline_2d = polyline<T, 2>;


template <class T, size_t D>
std::ostream& operator <<(std::ostream& os, const polygon<T, D>& value)
{
    return os << "{ " << core::delimit(value._data, " ") << " }";
}

template <class T, size_t D>
std::ostream& operator <<(std::ostream& os, const polyline<T, D>& value)
{
    return os << "{ " << core::delimit(value._data, " ") << " }";
}

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DETAIL_POLY_SHAPE_BASE_HPP_ */