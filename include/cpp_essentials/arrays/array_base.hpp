#ifndef CPP_ESSENTIALS_ARRAYS_ARRAY_BASE_HPP_
#define CPP_ESSENTIALS_ARRAYS_ARRAY_BASE_HPP_

#pragma once

#include <cpp_essentials/arrays/iterator.hpp>
#include <cpp_essentials/core/nil.hpp>

namespace cpp_essentials::arrays
{

inline void adjust_slice(
    stride_value_type slice_begin,
    stride_value_type slice_end,
    stride_value_type slice_stride,
    size_value_type size,
    stride_value_type stride,
    location_value_type& new_start,
    size_value_type& new_size,
    stride_value_type& new_stride)
{

    auto adjust_index = [](stride_value_type index, stride_value_type stride, stride_value_type size)
    {
        if (index < 0) index += size;
        if (index < 0) index = stride > 0 ? 0 : -1;
        if (index >= size) index = stride > 0 ? size : size - 1;

        return index;
    };

    if (slice_stride == 0)
    {
        throw std::invalid_argument { "stride must not be zero!" };
    }

#if 0
    if (slice_stride == nil_t::value)
    {
        slice_stride = 1;
    }

    slice_begin = slice_begin != nil_t::value
        ? adjust_index(slice_begin, slice_stride, size)
        : slice_stride > 0 ? 0 : size - 1;

    slice_end = slice_end != nil_t::value
        ? adjust_index(slice_end, slice_stride, size)
        : slice_stride > 0 ? size : -1;

    if ((slice_stride < 0 && slice_begin <= slice_end) || (slice_stride > 0 && slice_begin >= slice_end))
    {
        new_start = 0;
        new_size = 0;
        new_stride = stride;
    }
#endif

    new_start = slice_begin;
    new_size = (slice_end - slice_begin + (slice_stride > 0 ? -1 : +1)) / slice_stride + 1;
    new_stride = stride * slice_stride;
}


template <size_t D>
using array_stride_t = geo::vector<stride_value_type, D>;

template <size_t D>
using array_size_t = geo::vector<size_value_type, D>;

template <size_t D>
using array_location_t = geo::vector<location_value_type, D>;

template <size_t D>
using array_bounds_t = geo::bounding_box<size_value_type, D>;

template <size_t D>
using array_region_t = geo::bounding_box<location_value_type, D>;



template <size_t D>
class array_base
{
public:
    using stride_type = array_stride_t<D>;
    using size_type = array_size_t<D>;
    using location_type = array_location_t<D>;
    using bounds_type = array_bounds_t<D>;
    using region_type = array_region_t<D>;

protected:
    array_base(const size_type& size, const stride_type& stride)
        : _size(size)
        , _stride(stride)
    {
    }

    array_base()
        : array_base(size_type {}, stride_type {})
    {
    }


public:
    size_value_type volume() const
    {
        return core::accumulate(_size._data, size_value_type(1), core::multiplies);
    }

    bool empty() const
    {
        return volume() == 0;
    }

    const size_type& size() const
    {
        return _size;
    }

    const stride_type& stride() const
    {
        return _stride;
    }

    bounds_type bounds() const
    {
        return { size_type {}, size() };
    }

    template <size_t Dim = D, class = std::enable_if_t<(Dim == 2)>>
    size_value_type width() const
    {
        return _size.x();
    }

    template <size_t Dim = D, class = std::enable_if_t<(Dim == 2)>>
    size_value_type height() const
    {
        return _size.y();
    }

    template <size_t Dim = D, class = std::enable_if_t<(Dim == 2)>>
    size_value_type slice_count() const
    {
        return _size.template get<Dim>();
    }

    template <size_t Dim = D, class = std::enable_if_t<(Dim == 2)>>
    size_value_type column_count() const
    {
        return slice_count<0>();
    }

    template <size_t Dim = D, class = std::enable_if_t<(Dim == 2)>>
    size_value_type row_count() const
    {
        return slice_count<1>();
    }

    void swap(array_base& other)
    {
        std::swap(_size, other._size);
        std::swap(_stride, other._stride);
    }

protected:
    location_type begin_location() const
    {
        return geo::zeros;
    }

    location_type end_location() const
    {
        location_type result = geo::zeros;
        result[D - 1]  = _size[D - 1];
        return result;
    }

    stride_value_type get_offset(const location_type& location) const
    {
        return core::inner_product(location._data, _stride._data, stride_value_type {});
    }

    template <size_t N>
    location_type get_slice_offset(location_value_type index) const
    {
        location_type result = geo::zeros;
        result[N] = index;
        return result;
    }

    static stride_type adjust_stride(size_value_type element_size, const size_type& size)
    {
        stride_type result;
        result[0] = element_size;

        for (size_t i = 1; i < result.size(); ++i)
        {
            result[i] = result[i - 1] * size[i - 1];
        }

        return result;
    }

    stride_type get_iterator_step() const
    {
        stride_type result;

        result[0] = _stride[0];

        for (size_t i = 1; i < result.size(); ++i)
        {
            result[i] = _stride[i] - _size[i - 1] * _stride[i - 1];
        }

        return result;
    }

    size_type _size;
    stride_type _stride;
};

#define INHERIT_ARRAY_BASE_TYPES(BASE_CLASS)                    \
    using stride_type = typename BASE_CLASS::stride_type;       \
    using size_type = typename BASE_CLASS::size_type;           \
    using location_type = typename BASE_CLASS::location_type;   \
    using bounds_type = typename BASE_CLASS::bounds_type;       \
    using region_type = typename BASE_CLASS::region_type;       \

} /* namespace cpp_essentials::arrays */ 

#endif /* CPP_ESSENTIALS_ARRAYS_ARRAY_BASE_HPP_ */
