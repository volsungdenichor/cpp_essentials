#ifndef CPP_ESSENTIALS_ARRAYS_ARRAY_VIEW_HPP_
#define CPP_ESSENTIALS_ARRAYS_ARRAY_VIEW_HPP_

#include <memory>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <array>

#include <cassert>

#include <cpp_essentials/arrays/array_base.hpp>
#include <cpp_essentials/arrays/iterator.hpp>
#include <cpp_essentials/arrays/region_iterator.hpp>
#include <cpp_essentials/arrays/slice_iterator.hpp>

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/geo/contains.hpp>

namespace cpp_essentials::arrays
{

namespace detail
{

template <class T>
struct dummy
{
    using pointer = T*;
    using size_type = int;
    using stride_type = int;
};

} /* namespace detail */


template <class T, size_t D = 1>
class array_view : public array_base<D>
{
private:
    using base_type = array_base<D>;

public:
    INHERIT_ARRAY_BASE_TYPES(base_type)

    using value_type = std::remove_const_t<T>;
    using reference = T&;
    using pointer = T*;

    using iterator = detail::array_iterator<array_view<T, D>, D>;

    using slice_type = std::conditional_t<(D > 1), array_view<T, D - 1>, detail::dummy<T>>;

    using slice_iterator = detail::slice_iterator<slice_type>;
    using region_iterator = detail::region_iterator<array_view>;

    using slice_range_type = core::iterator_range<slice_iterator>;
    using region_range_type = core::iterator_range<region_iterator>;

    array_view(pointer ptr, const size_type& size, const stride_type& stride)
        : array_base<D>(size, stride)
        , _ptr(byte_pointer(ptr))
    {
    }

    array_view(pointer ptr, const size_type& size)
        : array_view(ptr, size, base_type::adjust_stride(sizeof(T), size))
    {
    }

    array_view()
        : array_view(nullptr, size_type {})
    {
    }

    array_view(const array_view& other)
        : array_view(other.data(), other.size(), other.stride())
    {
    }
   
    void operator =(const value_type& value)
    {
        static_assert(!std::is_const_v<T>, "array_view: cannot assign value to const array");
        core::fill(*this, value);
    }

    pointer data() const
    {
        return pointer(_ptr);
    }

    pointer data(const location_type& location) const
    {
        return pointer(_ptr + base_type::get_offset(location));
    }

    reference operator [](const location_type& location) const
    {
        return *data(location);
    }

    reference at(const location_type& location) const
    {
        EXPECTS(geo::contains(base_type::bounds(), location), "location out of bounds");
        return (*this)[location];
    }    

    value_type at(const location_type& location, const value_type& default_value) const
    {
        return geo::contains(base_type::bounds(), location)
            ? (*this)[location]
            : default_value;
    }

    iterator begin() const
    {
        return make_iterator(base_type::begin_location(), 0);
    }

    iterator end() const
    {
        return make_iterator(base_type::end_location(), base_type::volume());
    }

    template <size_t N, size_t Dim = D, CONCEPT = std::enable_if_t<(Dim > 1)>>
    slice_range_type slice_range() const
    {
        static const size_t dim = D - 1 - N;
        return slice_range<N>(size_value_type(0), base_type::size().template get<dim>());
    }

    template <size_t N, size_t Dim = D, CONCEPT = std::enable_if_t<(Dim > 1)>>
    slice_type slice(location_value_type index) const
    {
        return slice_range<N>()[index];
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    slice_range_type row_range() const
    {
        return slice_range<0>();
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    slice_type row(location_value_type index) const
    {
        return slice<0>(index);
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    slice_range_type column_range() const
    {
        return slice_range<1>();
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    slice_type column(location_value_type index) const
    {
        return slice<1>(index);
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim > 1)>>
    slice_type operator [](location_value_type index) const
    {
        return slice<0>(index);
    }

    array_view region(const region_type& region) const
    {
        return array_view(data(region.lower()), region.size(), base_type::stride());
    }

#if 0
    array_view sub(const stride_type& sub_begin, const stride_type& sub_end = nil, const stride_type& sub_stride = nil) const
    {
        location_type new_start;
        size_type new_size;
        stride_type new_stride;

        for (size_t i = 0; i < D; ++i)
        {
            adjust_slice(
                sub_begin[i], sub_end[i], sub_stride[i],
                base_type::size(i), base_type::stride(i),
                new_start[i], new_size[i], new_stride[i]);
        }

        return array_view(data(new_start), new_size, new_stride);
    }
#endif

    region_range_type region_range(const size_type& kernel_size) const
    {
        auto valid_region = region({ location_type(), base_type::size() - kernel_size + geo::ones.make<D, size_value_type>() });

        return core::make_range(
            region_iterator{ valid_region.begin(), kernel_size, base_type::stride() },
            region_iterator{ valid_region.end(), kernel_size, base_type::stride() });
    }

    void swap(array_view& other)
    {
        array_base<D>::swap(other);
        std::swap(_ptr, other._ptr);
    }

private:
    template <size_t N, size_t Dim = D, CONCEPT = std::enable_if_t<(Dim > 1)>>
    slice_range_type slice_range(location_value_type begin, location_value_type end) const
    {
        static_assert(N < D, "slice: invalid dimension");

        static const size_t dim = D - 1 - N;

        auto sub_size = detail::erase<dim>(base_type::size());
        auto sub_stride = detail::erase<dim>(base_type::stride());
        auto sub_step = begin < end ? base_type::stride().template get<dim>() : -base_type::stride().template get<dim>();

        return core::make_range(
            slice_iterator{ data(base_type::template get_slice_offset<dim>(begin)), sub_size, sub_stride, sub_step },
            slice_iterator{ data(base_type::template get_slice_offset<dim>(end)), sub_size, sub_stride, sub_step });
    }

    iterator make_iterator(const location_type& loc, stride_value_type counter) const
    {
        return iterator(data(loc), base_type::size(), base_type::get_iterator_step(), loc, counter);
    }

    byte_pointer _ptr;
};

template <class T, size_t D = 1>
using const_array_view = array_view<const T, D>;


namespace detail
{

struct from_fn
{
    template <class T, size_t S0>
    auto operator ()(T(&a)[S0]) const -> array_view<T, 1>
    {
        return { &a[0], { S0 } };
    }

    template <class T, size_t S0, size_t S1>
    auto operator ()(T(&a)[S0][S1]) const -> array_view<T, 2>
    {
        return { &a[0][0], { S1, S0 } };
    }

    template <class T, size_t S0, size_t S1, size_t S2>
    auto operator ()(T(&a)[S0][S1][S2]) const -> array_view<T, 3>
    {
        return { &a[0][0][0], { S2, S1, S0 } };
    }

    template <class T, size_t S0, size_t S1, size_t S2, size_t S3>
    auto operator ()(T(&a)[S0][S1][S2][S3]) const -> array_view<T, 4>
    {
        return { &a[0][0][0][0], { S3, S2, S1, S0 } };
    }
};

} /* namespace detail */

static constexpr detail::from_fn from = {};

} /* namespace cpp_essentials::arrays */

#endif /* CPP_ESSENTIALS_ARRAYS_ARRAY_VIEW_HPP_ */
