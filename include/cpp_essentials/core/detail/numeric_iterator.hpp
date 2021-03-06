#ifndef CPP_ESSENTIALS_CORE_DETAIL_NUMERIC_ITERATOR_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_NUMERIC_ITERATOR_HPP_

#pragma once

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/core/detail/iterator_func_helper.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class T, class U>
class numeric_iterator
    : public core::iterator_facade
        < numeric_iterator<T, U>
        , std::random_access_iterator_tag
        , T>
{
public:
    using base_type = core::iterator_facade
        < numeric_iterator<T, U>
        , std::random_access_iterator_tag
        , T>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    numeric_iterator() = default;

    numeric_iterator(T value, U step)
        : _value{ value }
        , _step{ step }
    {
    }

    reference ref() const
    {
        return _value;
    }

    void inc()
    {
        advance(+1);
    }

    void dec()
    {
        advance(-1);
    }

    void advance(difference_type offset)
    {
        _value += _step * (int)offset;
    }

    bool is_equal(const numeric_iterator& other) const
    {
        return _value == other._value;
    }

    bool is_less(const numeric_iterator& other) const
    {
        return (_step > 0 && _value < other._value) || (_step < 0 && _value > other._value);
    }

    difference_type distance(const numeric_iterator& other) const
    {
        return difference_type((other._value - _value) / _step);
    }

private:
    T _value;
    U _step;
};

template <class T>
class infinite_numeric_iterator
    : public core::iterator_facade
    < infinite_numeric_iterator<T>
    , std::forward_iterator_tag
    , T>
{
public:
    using base_type = core::iterator_facade
        < infinite_numeric_iterator<T>
        , std::forward_iterator_tag
        , T>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    infinite_numeric_iterator()
        : _value{ std::numeric_limits<T>::min() }
    {
    }

    infinite_numeric_iterator(T value)
        : _value{ value }
    {
    }

    reference ref() const
    {
        return _value;
    }

    void inc()
    {
        ++_value;
    }

    bool is_equal(const infinite_numeric_iterator& other) const
    {
        return _value == other._value;
    }

private:
    T _value;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_NUMERIC_ITERATOR_HPP_ */
