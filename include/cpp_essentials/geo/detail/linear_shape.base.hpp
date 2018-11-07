#ifndef CPP_ESSENTIALS_GEO_DETAIL_LINEAR_SHAPE_BASE_HPP_
#define CPP_ESSENTIALS_GEO_DETAIL_LINEAR_SHAPE_BASE_HPP_

#pragma once

#include <cpp_essentials/geo/matrix.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct ray_tag { };
struct line_tag { };
struct segment_tag { };

} /* namespace detail */


template <class T, size_t D, class Tag>
class linear_shape
{
public:
    using vector_type = vector<T, D>;
    using value_type = vector_type;
    using size_type = std::size_t;

    using const_reference = const vector_type&;
    using reference = vector_type&;

    linear_shape()
    {
        core::fill(_data, vector_type {});
    }

    linear_shape(const vector_type& p0, const vector_type& p1)
        : _data{ p0, p1 }
    {
    }

    template <class U>
    linear_shape(const vector<U, D>& p0, const vector<U, D>& p1)
        : linear_shape { vector_type(p0), vector_type(p1) }
    {
    }

    template <class U>
    linear_shape<U, D, Tag> as() const
    {
        return *this;
    }   

    const_reference operator [](size_type index) const
    {
        return _data[index];
    }

    reference operator [](size_type index)
    {
        return _data[index];
    }

    std::array<vector_type, 2> _data;
};

template <class T, size_t D>
using line = linear_shape<T, D, detail::line_tag>;

template <class T, size_t D>
using ray = linear_shape<T, D, detail::ray_tag>;

template <class T, size_t D>
using segment = linear_shape<T, D, detail::segment_tag>;


template <class T>
using line_2d = line<T, 2>;

template <class T>
using ray_2d = ray<T, 2>;

template <class T>
using segment_2d = segment<T, 2>;


namespace detail
{

struct make_segment_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& start, const vector<T, D>& end) const -> segment<T, D>
    {
        return{ start, end };
    }
};

struct make_ray_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& start, const vector<T, D>& end) const -> ray<T, D>
    {
        return{ start, end };
    }
};

struct make_line_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& start, const vector<T, D>& end) const -> line<T, D>
    {
        return{ start, end };
    }
};

} /* namespace detail */

static constexpr detail::make_segment_fn make_segment = {};
static constexpr detail::make_ray_fn make_ray = {};
static constexpr detail::make_line_fn make_line = {};

template <class T, size_t D>
std::ostream& operator <<(std::ostream& os, const segment<T, D>& item)
{
    return os << "segment { start:" << item[0] << ", end:" << item[1] << " }";
}

template <class T, size_t D>
std::ostream& operator <<(std::ostream& os, const ray<T, D>& item)
{
    return os << "ray { start:" << item[0] << ", dir:" << (item[1] - item[0]) << " }";
}

template <class T, size_t D>
std::ostream& operator <<(std::ostream& os, const line<T, D>& item)
{
    return os << "line { start:" << item[0] << ", dir:" << (item[1] - item[0]) << " }";
}

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DETAIL_LINEAR_SHAPE_BASE_HPP_ */
