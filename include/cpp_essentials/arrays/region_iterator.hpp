#ifndef CPP_ESSENTIALS_ARRAYS_REGION_ITERATOR_HPP_
#define CPP_ESSENTIALS_ARRAYS_REGION_ITERATOR_HPP_

#pragma once

#include <cpp_essentials/core/iterator_facade.hpp>

namespace cpp_essentials::arrays
{

namespace detail
{

template <class SubType>
class region_iterator
    : public core::iterator_facade
    < region_iterator<SubType>
    , std::forward_iterator_tag
    , SubType>
{
private:
    using base_type = core::iterator_facade
        < region_iterator<SubType>
        , std::forward_iterator_tag
        , SubType>;

public:
    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    using iterator = typename SubType::iterator;
    using size_type = typename SubType::size_type;
    using stride_type = typename SubType::stride_type;
    using location_type = typename SubType::location_type;
    using bounds_type = typename SubType::region_type;

    region_iterator(iterator iter, const size_type& size, const stride_type& stride)
        : _iter(iter)
        , _size(size)
        , _stride(stride)
    {
    }

    reference ref() const
    {
        return value_type(&(*_iter), _size, _stride);
    }

    void inc()
    {
        ++_iter;
    }

    bool is_equal(const region_iterator& other) const
    {
        return _iter == other._iter;
    }

    const location_type& location() const
    {
        return _iter.location();
    }

    const size_type& size() const
    {
        return _size;
    }

    bounds_type bounds() const
    {
        return { location(), location() + size() };
    }

private:
    iterator _iter;
    size_type _size;
    stride_type _stride;
};

} /* namespace detail */

} /* namespace cpp_essentials::arrays */

#endif /* CPP_ESSENTIALS_ARRAYS_REGION_ITERATOR_HPP_ */