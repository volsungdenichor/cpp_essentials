#ifndef CPP_ESSENTIALS_GEO_BOUNDING_BOX_HPP_
#define CPP_ESSENTIALS_GEO_BOUNDING_BOX_HPP_

#pragma once

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/geo/matrix.hpp>
#include <cpp_essentials/geo/interval.hpp>
#include <cpp_essentials/geo/vertex_array.hpp>

namespace cpp_essentials::geo
{

template <class T, size_t D>
class bounding_box
{
public:
    using interval_type = interval<T>;
    using vector_type = vector<T, D>;

    bounding_box()
    {
        core::fill(_data, interval_type {});
    }

    bounding_box(const vector_type& lower, const vector_type& upper)
    {
        for (size_t d = 0; d < D; ++d)
        {
            _data[d] = interval_type { lower[d], upper[d] };
        }
    }

    template <class U>
    bounding_box(const vector<U, D>& lower, const vector<U, D>& upper)
        : bounding_box { static_cast<vector_type>(lower), static_cast<vector_type>(upper) }
    {
    }

    template <class U>
    bounding_box(const bounding_box<U, D>& other)
        : bounding_box { other.lower(), other.upper() }
    {
    }   


    template <class U>
    bounding_box<U, D> as() const
    {
        return *this;
    }


    vector_type lower() const
    {
        vector_type result;
        core::transform(_data, result._data.begin(), [](auto&& b) { return b.lower(); });
        return result;
    }

    vector_type upper() const
    {
        vector_type result;
        core::transform(_data, result._data.begin(), [](auto&& b) { return b.upper(); });
        return result;
    }

    vector_type min() const
    {
        vector_type result;
        core::transform(_data, result._data.begin(), [](auto&& b) { return b.min(); });
        return result;
    }

    vector_type max() const
    {
        vector_type result;
        core::transform(_data, result._data.begin(), [](auto&& b) { return b.max(); });
        return result;
    }

    vector_type location() const
    {
        return lower();
    }

    vector_type size() const
    {
        return upper() - lower();
    }

    bool empty() const
    {
        return core::any_of(_data, [](auto&& b) { return b.empty(); });
    }

    const interval_type& operator [](size_t index) const
    {
        return _data.at(index);
    }    

    template <class U = T, size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    quad<U, 2> as_quad() const
    {
        const auto lo = min();
        const auto hi = max();
        return
        {
            { lo[0], lo[1] },
            { hi[0], lo[1] },
            { hi[0], hi[1] },
            { lo[0], hi[1] }
        };
    }

    template <class U, size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    operator quad<U, 2>() const
    {
        return as_quad<U>();
    }

    std::array<interval_type, D> _data;
};

template <class T>
using rect = bounding_box<T, 2>;

template <class T>
using cuboid = bounding_box<T, 3>;

template <class T>
using rect_2d = rect<T>;


template
    < class T
    , class U
    , size_t D
    , CONCEPT = cc::Add<T, U>>
auto& operator +=(bounding_box<T, D>& lhs, const vector<U, D>& rhs)
{
    for (size_t d = 0; d < D; ++d)
    {
        lhs._data[d] += rhs[d];
    }
    return lhs;
}

template
    < class T
    , class U
    , size_t D
    , CONCEPT = cc::Add<T, U>>
auto operator +(const bounding_box<T, D>& lhs, const vector<U, D>& rhs) -> bounding_box<cc::Add<T, U>, D>
{
    bounding_box<cc::Add<T, U>, D> result;
    for (size_t d = 0; d < D; ++d)
    {
        result._data[d] = lhs._data[d] + rhs[d];
    }
    return result;
}

template
< class T
    , class U
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(const rect_2d<T>& lhs, const square_matrix_2d<U>& rhs) -> quad_2d<cc::Multiply<T, U>>
{
    return static_cast<quad<T, 2>>(lhs) * rhs;
}

template
    < class T
    , class U
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(const square_matrix_2d<T>& lhs, const rect_2d<U>& rhs) -> quad_2d<cc::Multiply<T, U>>
{
    return rhs * lhs;
}




template
    < class T
    , class U
    , size_t D
    , CONCEPT = cc::Subtract<T, U>>
auto& operator -=(bounding_box<T, D>& lhs, const vector<U, D>& rhs)
{
    for (size_t d = 0; d < D; ++d)
    {
        lhs._data[d] -= rhs[d];
    }
    return lhs;
}

template
    < class T
    , class U
    , size_t D
    , CONCEPT = cc::Subtract<T, U>>
auto operator -(const bounding_box<T, D>& lhs, const vector<U, D>& rhs) -> bounding_box<cc::Subtract<T, U>, D>
{
    bounding_box<cc::Subtract<T, U>, D> result;
    for (size_t d = 0; d < D; ++d)
    {
        result._data[d] = lhs._data[d] - rhs[d];
    }
    return result;
}

template
    < class T
    , class U
    , size_t D>
bool operator ==(const bounding_box<T, D>& lhs, const bounding_box<U, D>& rhs)
{
    return core::equal(lhs._data, rhs._data);
}

template
    < class T
    , class U
    , size_t D>
bool operator !=(const bounding_box<T, D>& lhs, const bounding_box<U, D>& rhs)
{
    return !(lhs == rhs);
}

namespace detail
{

template <size_t D, size_t E>
struct get_rect_side_fn
{
    template <class T>
    auto operator ()(const rect_2d<T>& rect) const -> T
    {
        return E == 0 ? rect._data[D].min() : rect._data[D].max();
    }
};

template <size_t H, size_t V>
struct get_rect_vertex_fn
{
    template <class T>
    auto operator ()(const rect_2d<T>& rect) const -> vector_2d<T>
    {
        static constexpr get_rect_side_fn<0, H> h = {};
        static constexpr get_rect_side_fn<1, V> v = {};

        return { h(rect), v(rect) };
    }
};

} /* namespace detail */

static constexpr detail::get_rect_vertex_fn<0, 0> top_left = {};
static constexpr detail::get_rect_vertex_fn<1, 0> top_right = {};
static constexpr detail::get_rect_vertex_fn<0, 1> bottom_left = {};
static constexpr detail::get_rect_vertex_fn<1, 1> bottom_right = {};
static constexpr detail::get_rect_side_fn<0, 0> left = {};
static constexpr detail::get_rect_side_fn<0, 1> right = {};
static constexpr detail::get_rect_side_fn<1, 0> top = {};
static constexpr detail::get_rect_side_fn<1, 1> bottom = {};

template <class T, size_t D>
std::ostream& operator <<(std::ostream& os, const bounding_box<T, D>& bb)
{
    os << "bounding_box { ";
    os << "lower:" << bb.lower();
    os << ", ";
    os << "upper:" << bb.upper();
    os << " }";

    return os;
}

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_BOUNDING_BOX_HPP_ */
