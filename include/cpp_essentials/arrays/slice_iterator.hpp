#ifndef CPP_ESSENTIALS_ARRAYS_SLICE_ITERATOR_HPP_
#define CPP_ESSENTIALS_ARRAYS_SLICE_ITERATOR_HPP_

#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/arrays/detail/vector.creation.hpp>

namespace cpp_essentials::arrays
{

namespace detail
{

template <class SubType>
class slice_iterator
    : public core::iterator_facade
        < slice_iterator<SubType>
        , std::random_access_iterator_tag
        , SubType
        , stride_value_type
        , SubType>
{
private:
    using base_type = core::iterator_facade
        < slice_iterator<SubType>
        , std::random_access_iterator_tag
        , SubType
        , stride_value_type
        , SubType>;

public:
    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    using pointer_type = typename SubType::pointer;
    using size_type = typename SubType::size_type;
    using stride_type = typename SubType::stride_type;

    slice_iterator(pointer_type ptr, const size_type& size, const stride_type& stride, difference_type step)
        : _ptr(byte_pointer(ptr))
        , _size(size)
        , _stride(stride)
        , _step(step)
    {
    }

    reference ref() const
    {
        return value_type { pointer_type(_ptr), _size, _stride };
    }

    void inc()
    {
        advance(1);
    }

    void dec()
    {
        advance(-1);
    }

    void advance(difference_type offset)
    {
        _ptr += _step * offset;
    }

    bool is_equal(const slice_iterator& other) const
    {
        return _ptr == other._ptr;
    }

    bool is_less(const slice_iterator& other) const
    {
        return (_step > 0 && _ptr < other._ptr) || (_step < 0 && _ptr > other._ptr);
    }

    difference_type distance(const slice_iterator& other) const
    {
        return (other._ptr - _ptr) / _step;
    }

private:
    byte_pointer _ptr;
    size_type _size;
    stride_type _stride;
    difference_type _step;
};

} /* namespace detail */

} /* namespace cpp_essentials::arrays */

#endif /* CPP_ESSENTIALS_ARRAYS_SLICE_ITERATOR_HPP_ */