#ifndef CPP_ESSENTIALS_CORE_DETAIL_REPEAT_ITERATOR_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_REPEAT_ITERATOR_HPP_

#pragma once

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/core/detail/iterator_func_helper.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class T>
class repeat_iterator
    : public core::iterator_facade
    < repeat_iterator<T>
    , std::random_access_iterator_tag
    , const T&>
{
public:
    using base_type = core::iterator_facade
        < repeat_iterator<T>
        , std::random_access_iterator_tag
        , const T&>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    repeat_iterator() = default;

    repeat_iterator(T value, int index)
        : _value{ std::move(value) }
        , _index{ index }
    {
    }

    reference ref() const
    {
        return _value;
    }

    pointer ptr() const
    {
        return &_value;
    }

    void inc()
    {
        ++_index;
    }

    void dec()
    {
        --_index;
    }

    void advance(difference_type offset)
    {
        _index += offset;
    }

    bool is_equal(const repeat_iterator& other) const
    {
        return _index == other._index;
    }

    bool is_less(const repeat_iterator& other) const
    {
        return _index < other._index;
    }

    difference_type distance(const repeat_iterator& other) const
    {
        return other._index - _index;
    }

private:
    T _value;
    int _index;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_REPEAT_ITERATOR_HPP_ */
