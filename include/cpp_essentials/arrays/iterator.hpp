#ifndef CPP_ESSENTIALS_ARRAYS_ITERATOR_HPP_
#define CPP_ESSENTIALS_ARRAYS_ITERATOR_HPP_

#pragma once

#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/arrays/detail/vector.creation.hpp>

namespace cpp_essentials::arrays
{

namespace detail
{

template <size_t D, size_t Dim, class Iterator>
struct forward_advancer
{    
    void operator ()(Iterator& item) const
    {
        using namespace geo;

        static forward_advancer<D, Dim + 1, Iterator> next;

        item._ptr += item._step.template get<Dim>();
        ++item._location.template get<Dim>();

        if (location_value_type(item._location.template get<Dim>()) == location_value_type(item._size.template get<Dim>()))
        {
            item._location.template get<Dim>() = 0;

            next(item);
        }
    }
};

template <size_t D, class Iterator>
struct forward_advancer<D, D, Iterator>
{    
    void operator ()(Iterator& item) const { }
};



template <size_t D, size_t Dim, class Iterator>
struct backward_advancer
{    
    void operator ()(Iterator& item) const
    {
        using namespace geo;

        static backward_advancer<D, Dim + 1, Iterator> next;

        if (item._location.template get<Dim>() == 0)
        {
            item._location.template get<Dim>() = item._size.template get<Dim>();

            next(item);
        }

        item._ptr -= item._step.template get<Dim>();
        --item._location.template get<Dim>();
    }
};

template <size_t D, class Iterator>
struct backward_advancer<D, D, Iterator>
{
    void operator ()(Iterator& item) const { }
};

template <class SubType, size_t D>
class array_iterator
    : public core::iterator_facade
        < array_iterator<SubType, D>
        , typename std::conditional<(D == 1), std::random_access_iterator_tag, std::bidirectional_iterator_tag>::type
        , typename SubType::reference
        , stride_value_type
        , typename SubType::pointer>
{
private:
    using base_type = core::iterator_facade
        < array_iterator<SubType, D>
        , typename std::conditional<(D == 1), std::random_access_iterator_tag, std::bidirectional_iterator_tag>::type
        , typename SubType::reference
        , stride_value_type
        , typename SubType::pointer>;

public:
    using pointer_type = typename SubType::pointer;
    using size_type = typename SubType::size_type;
    using stride_type = typename SubType::stride_type;
    using location_type = typename SubType::location_type;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    array_iterator() = default;

    array_iterator(
        pointer_type ptr,
        const size_type& size,
        const stride_type& step,
        const location_type& location,
        difference_type counter)
        : _ptr(byte_pointer(ptr))
        , _size(size)
        , _step(step)
        , _location(location)
        , _counter(counter)
    {
    }

    pointer ptr() const
    {
        return reinterpret_cast<pointer>(_ptr);
    }

    reference ref() const
    {
        return *ptr();
    }

    void inc()
    {
        static forward_advancer<D, 0, array_iterator> next;

        ++_counter;
        next(*this);
    }

    void dec()
    {
        static backward_advancer<D, 0, array_iterator> next;

        --_counter;
        next(*this);
    }

    bool is_equal(const array_iterator& other) const
    {
        return _counter == other._counter;
    }

    bool is_less(const array_iterator& other) const
    {
        return _counter < other._counter;
    }

    difference_type distance(const array_iterator& other) const
    {
        return other._counter - _counter;
    }

    location_type location() const
    {
        return _location;
    }

    void advance(difference_type offset)
    {
        _counter += offset;

        while (offset < 0)
        {
            ++offset;
            dec();
        }

        while (offset > 0)
        {
            --offset;
            inc();
        }
    }

    byte_pointer _ptr;
    size_type _size;
    stride_type _step;
    location_type _location;
    difference_type _counter;
};

} /* namespace detail */

} /* cpp_essentials::arrays */

#endif /* CPP_ESSENTIALS_ARRAYS_ITERATOR_HPP_ */
