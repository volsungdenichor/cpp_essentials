#pragma once

#include <array>

#include <cpp_essentials/carto/coords.hpp>
#include <cpp_essentials/core/algorithm.hpp>

namespace cpp_essentials::carto
{

template <class T>
class segment_base
{
public:
    using value_type = coords_base<T>;
    using size_type = std::size_t;

    using const_reference = const value_type&;
    using reference = value_type & ;

    segment_base()
    {
        core::fill(_data, value_type{});
    }

    segment_base(const value_type& p0, const value_type& p1)
        : _data{ p0, p1 }
    {
    }

    const_reference operator [](size_type index) const
    {
        return _data[index];
    }

    reference operator [](size_type index)
    {
        return _data[index];
    }

    std::array<value_type, 2> _data;
};

template <class T>
std::ostream& operator <<(std::ostream& os, const segment_base<T>& item)
{
    return os << "segment { start:" << item[0] << ", end:" << item[1] << " }";
}


namespace detail
{

struct polygon_tag { };
struct polyline_tag { };

} /* namespace detail */


template <class T, class Tag>
class coords_array
{
public:
    using value_type = coords_base<T>;
    using size_type = std::size_t;

    using const_reference = const value_type&;
    using reference = value_type&;

    using data_type = std::vector<value_type>;

    coords_array()
    {
        core::fill(_data, value_type{});
    }

    coords_array(size_type size)
    {
        resize(size);
        core::fill(_data, value_type{});
    }

    coords_array(std::initializer_list<value_type> init)
    {
        resize(init.size());
        core::copy(init, _data.begin());
    }

    coords_array(data_type data)
        : _data{ std::move(data) }
    {
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
        _data.resize(size);
    }

    data_type _data;
};

template <class T>
using shape_base = coords_array<T, detail::polygon_tag>;

template <class T>
using path_base = coords_array<T, detail::polyline_tag>;


namespace detail
{

struct segment_count_fn
{
    template <class T>
    auto operator ()(const shape_base<T>& shape) const -> size_t
    {
        return shape.size();
    }

    template <class T>
    auto operator ()(const path_base<T>& shape) const -> size_t
    {
        return shape.size() - 1;
    }
};

struct vertex_count_fn
{
    template <class T, class Tag>
    auto operator ()(const coords_array<T, Tag>& shape) const -> size_t
    {
        return shape.size();
    }
};

struct get_segment_fn
{
    template <class T>
    auto operator ()(const shape_base<T>& shape, size_t index) const -> segment_base<T>
    {
        return { shape[index], shape[(index + 1) % shape.size()] };
    }

    template <class T>
    auto operator ()(const path_base<T>& shape, size_t index) const -> segment_base<T>
    {
        return { shape[index + 0], shape[index + 1] };
    }
};

struct get_vertex_fn
{
    template <class T, class Tag>
    auto operator ()(const coords_array<T, Tag>& shape, size_t index) const -> coords_base<T>
    {
        return shape[index];
    }
};

} /* namespace detail */

using segment = segment_base<double>;
using shape = shape_base<double>;
using path = path_base<double>;


static constexpr auto segment_count = detail::segment_count_fn{};
static constexpr auto vertex_count = detail::vertex_count_fn{};
static constexpr auto get_segment = detail::get_segment_fn{};
static constexpr auto get_vertex = detail::get_vertex_fn{};

} /* namespace cpp_essentials::carto */
