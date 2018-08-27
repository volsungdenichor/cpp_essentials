#ifndef CPP_ESSENTIALS_CORE_ITERATOR_FACADE_HPP_
#define CPP_ESSENTIALS_CORE_ITERATOR_FACADE_HPP_

#pragma once

#include <iterator>
#include <type_traits>

namespace cpp_essentials::core
{
    
template 
    < class Self
    , class IteratorCategory
    , class Reference
    , class DifferenceType = std::ptrdiff_t
    , class ValueType = std::decay_t<Reference>
    , class Pointer = std::add_pointer_t<Reference>>
class iterator_facade
{
private:
    using self_type = Self;

public:
    using iterator_category = IteratorCategory;
    using reference = Reference;
    using value_type = ValueType;
    using pointer = Pointer;
    using difference_type = DifferenceType;

    reference operator *() const
    {
        return self().ref();
    }

    reference operator [](difference_type offset) const
    {
        self_type temp(self());
        temp.advance(offset);
        return temp.ref();
    }

    pointer operator ->() const
    {
        return self().ptr();
    }



    self_type& operator ++()
    {
        self().inc();
        return self();
    }

    self_type operator ++(int)
    {
        self_type result(self());
        self().inc();
        return result;
    }

    self_type& operator --()
    {
        self().dec();
        return self();
    }

    self_type operator --(int)
    {
        self_type result(self());
        self().dec();
        return result;
    }



    self_type& operator +=(difference_type offset)
    {
        self().advance(offset);
        return self();
    }

    self_type operator +(difference_type offset) const
    {
        self_type result(self());
        result.advance(offset);
        return result;
    }

    self_type& operator -=(difference_type offset)
    {
        self().advance(-offset);
        return self();
    }

    self_type operator -(difference_type offset) const
    {
        self_type result(self());
        result.advance(-offset);
        return result;
    }



    bool operator ==(const iterator_facade& other) const
    {
        return self().is_equal(other.self());
    }

    bool operator !=(const iterator_facade& other) const
    {
        return !(*this == other);
    }



    bool operator <(const iterator_facade& other) const
    {
        return self().is_less(other.self());
    }

    bool operator >(const iterator_facade& other) const
    {
        return *this < other;
    }

    bool operator <=(const iterator_facade& other) const
    {
        return !(*this > other);
    }

    bool operator >=(const iterator_facade& other) const
    {
        return !(*this < other);
    }



    difference_type operator -(const iterator_facade& other) const
    {
        return other.self().distance(self());
    }

protected:
    self_type& self()
    {
        return static_cast<self_type&>(*this);
    }

    const self_type& self() const
    {
        return static_cast<const self_type&>(*this);
    }
};

#define INHERIT_ITERATOR_FACADE_TYPES(BASE_CLASS)                       \
    using iterator_category = typename BASE_CLASS::iterator_category;   \
    using reference = typename BASE_CLASS::reference;                   \
    using value_type = typename BASE_CLASS::value_type;                 \
    using pointer = typename BASE_CLASS::pointer;                       \
    using difference_type = typename BASE_CLASS::difference_type;       \

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ITERATOR_FACADE_HPP_ */