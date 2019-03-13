#ifndef CPP_ESSENTIALS_ARRAYS_ARRAY_HPP_
#define CPP_ESSENTIALS_ARRAYS_ARRAY_HPP_

#include <vector>
#include <cpp_essentials/arrays/array_view.hpp>

namespace cpp_essentials::arrays
{

template <class T, size_t D = 1, size_t ElementSize = sizeof(T)>
class array : public array_base<D>
{
private:
    using base_type = array_base<D>;

public:
    INHERIT_ARRAY_BASE_TYPES(base_type)

    using value_type = T;

    using mut_view_type = array_view<T, D>;
    using view_type = array_view<const T, D>;

    using reference = typename mut_view_type::reference;
    using const_reference = typename view_type::reference;

    using pointer = typename mut_view_type::pointer;
    using const_pointer = typename view_type::pointer;

    using iterator = typename mut_view_type::iterator;
    using const_iterator = typename view_type::iterator;

    using slice_type = typename mut_view_type::slice_type;
    using const_slice_type = typename view_type::slice_type;

    using slice_range_type = typename mut_view_type::slice_range_type;
    using const_slice_range_type = typename view_type::slice_range_type;

    using region_range_type = typename mut_view_type::region_range_type;
    using const_region_range_type = typename view_type::region_range_type;

    array(const size_type& size, const stride_type& stride)
        : array_base<D>(size, stride)
    {
        auto total_size = base_type::get_offset(base_type::end_location());

        _data.resize(total_size);
        
        core::fill(*this, value_type {});
    }

    explicit array(const size_type& size)
        : array(size, base_type::adjust_stride(ElementSize, size))
    {
    }

    array()
        : array(size_type{})
    {
    }

    array(const array& other) = default;

    array(array&& other) = default;

    template <class U>
    array(const array_view<U, D>& other)
        : array(other.size())
    {
        core::copy(other, begin());
    }

    template <class U>
    array(const array<U, D>& other)
        : array(other.view())
    {
    }

    array& operator =(array other)
    {
        swap(other);
        return *this;
    }

    view_type view() const
    {
        return{ const_pointer(_data.data()), base_type::size(), base_type::stride() };
    }

    mut_view_type view()
    {
        return{ pointer(_data.data()), base_type::size(), base_type::stride() };
    }

    view_type cview() const
    {
        return view();
    }

    mut_view_type mut_view()
    {
        return view();
    }

    operator view_type() const
    {
        return view();
    }

    operator mut_view_type()
    {
        return view();
    }

    void operator =(const value_type& value)
    {
        view() = value;
    }

    const_pointer data() const
    {
        return view().data();
    }

    pointer data()
    {
        return view().data();
    }

    const_pointer data(const location_type& location) const
    {
        return view().data();
    }

    pointer data(const location_type& location)
    {
        return view().data();
    }

    const_reference operator [](const location_type& location) const
    {
        return view()[location];
    }

    reference operator [](const location_type& location)
    {
        return view()[location];
    }

    const_reference at(const location_type& location) const
    {
        return view().at(location);
    }

    reference at(const location_type& location)
    {
        return view().at(location);
    }
    
    value_type at(const location_type& location, const value_type& default_value) const
    {
        return view().at(location, default_value);
    }

    const_iterator begin() const
    {
        return view().begin();
    }

    iterator begin()
    {
        return view().begin();
    }

    const_iterator end() const
    {
        return view().end();
    }

    iterator end()
    {
        return view().end();
    }

    const_iterator cbegin() const
    {
        return begin();
    }

    const_iterator cend() const
    {
        return end();
    }

    view_type region(const region_type& region) const
    {
        return view().region(region);
    }

    mut_view_type region(const region_type& region)
    {
        return view().region(region);
    }

#if 0
    view_type sub(const stride_type& sub_begin, const stride_type& sub_end = nil, const stride_type& sub_stride = nil) const
    {
        return view().sub(sub_begin, sub_end, sub_stride);
    }

    mut_view_type sub(const stride_type& sub_begin, const stride_type& sub_end = nil, const stride_type& sub_stride = nil)
    {
        return view().sub(sub_begin, sub_end, sub_stride);
    }
#endif

    template <size_t N, size_t Dim = D, CONCEPT = std::enable_if_t<(Dim > 1)>>
    const_slice_type slice(location_value_type index) const
    {
        return view().slice<N>(index);
    }

    template <size_t N, size_t Dim = D, CONCEPT = std::enable_if_t<(Dim > 1)>>
    slice_type slice(location_value_type index)
    {
        return view().slice<N>(index);
    }

    template <size_t N, size_t Dim = D, CONCEPT = std::enable_if_t<(Dim > 1)>>
    const_slice_range_type slice_range() const
    {
        return view().slice_range<N>();
    }

    template <size_t N, size_t Dim = D, CONCEPT = std::enable_if_t<(Dim > 1)>>
    slice_range_type slice_range()
    {
        return view().slice_range<N>();
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim > 1)>>
    const_slice_type operator [](location_value_type index) const
    {
        return view()[index];
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim > 1)>>
    slice_type operator [](location_value_type index)
    {
        return view()[index];
    }

    const_region_range_type region_range(const size_type& size) const
    {
        return view().region_range(size);
    }

    region_range_type region_range(const size_type& size)
    {
        return view().region_range(size);
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    const_slice_type row(location_value_type index) const
    {
        return view().row(index);
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    slice_type row(location_value_type index)
    {
        return view().row(index);
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    const_slice_range_type row_range() const
    {
        return view().row_range();
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    slice_range_type row_range()
    {
        return view().row_range();
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    const_slice_type column(location_value_type index) const
    {
        return view().column(index);
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    slice_type column(location_value_type index)
    {
        return view().column(index);
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    const_slice_range_type column_range() const
    {
        return view().column_range();
    }

    template <size_t Dim = D, CONCEPT = std::enable_if_t<(Dim == 2)>>
    slice_range_type column_range()
    {
        return view().column_range();
    }

    void swap(array& other)
    {
        array_base<D>::swap(other);
        std::swap(_data, other._data);
    }

private:
    std::vector<byte> _data;    
};

} /* namespace cpp_essentials::arrays */

#endif /* CPP_ESSENTIALS_ARRAYS_ARRAY_HPP_ */
