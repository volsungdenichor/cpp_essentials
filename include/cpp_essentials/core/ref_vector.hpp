#ifndef CPP_ESSENTIALS_CORE_REF_VECTOR_HPP_
#define CPP_ESSENTIALS_CORE_REF_VECTOR_HPP_

#pragma once

#include <cpp_essentials/concepts/concepts.hpp>
#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/core/algorithm.hpp>

namespace cpp_essentials::core
{

namespace detail
{
template <class Iter, class T>
class dereferenced_iterator
    : public iterator_facade
    < dereferenced_iterator<Iter, T>
    , concepts::iterator_category<Iter>
    , T
    , concepts::iterator_difference<Iter>>
{
public:
    using base_type = core::iterator_facade
        < dereferenced_iterator<Iter, T>
        , concepts::iterator_category<Iter>
        , T
        , concepts::iterator_difference<Iter>>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    dereferenced_iterator(Iter iter)
        : _iter{ iter }
    {
    }

    reference ref() const
    {
        return **_iter;
    }

    void inc()
    {
        ++_iter;
    }

    void dec()
    {
        --_iter;
    }

    void advance(difference_type offset)
    {
        std::advance(_iter, offset);
    }

    bool is_equal(const dereferenced_iterator& other) const
    {
        return _iter == other._iter;
    }

    bool is_less(const dereferenced_iterator& other) const
    {
        return _iter < other._iter;
    }

    difference_type distance(const dereferenced_iterator& other) const
    {
        return std::distance(_iter, other._iter);
    }

private:
    Iter _iter;
};

} /* namespace detail */

template <class T>
class ref_vector
{
public:
    using value_type = std::remove_const_t<T>;
    using reference = T&;
    using pointer = T*;

    using inner_const_iter = typename std::vector<pointer>::const_iterator;
    using inner_iter = typename std::vector<pointer>::iterator;
    
    using iterator = detail::dereferenced_iterator<inner_iter, reference>;
    using const_iterator = detail::dereferenced_iterator<inner_const_iter, reference>;
    using size_type = std::size_t;

    template <class Container>
    using is_iterator_constructible = detail::is_iterator_constructible<Container, const_iterator>;

    ref_vector() = default;        
    ref_vector(const ref_vector&) = default;
    ref_vector(ref_vector&&) = default;

    template <class Container, CONCEPT_IF(std::is_reference<concepts::range_reference<Container>>::value)>
    ref_vector(Container&& container)
    {
        std::transform(std::begin(container), std::end(container), std::back_inserter(_vect), [](auto&& item) { return pointer(&item); });
    }

    template <class Container, CONCEPT_IF(is_iterator_constructible<Container>::value)>
    operator Container() const
    {
        return { begin(), end() };
    }

    iterator begin()
    {
        return { _vect.begin() };
    }

    iterator end()
    {
        return { _vect.end() };
    }

    const_iterator begin() const
    {
        return { _vect.begin() };
    }

    const_iterator end() const
    {
        return { _vect.end() };
    }

    bool empty() const
    {
        return _vect.empty();
    }

    size_type size() const
    {
        return _vect.size();
    }

    size_type capacity() const
    {
        return _vect.capacity();
    }

    void resize(size_type size)
    {
        _vect.resize(size);
    }

    void reserve(size_type size)
    {
        _vect.reserve(size);
    }

    void clear()
    {
        _vect.clear();
    }

    void push_back(reference item)
    {
        _vect.push_back(&item);
    }

    void insert(iterator pos, reference item)
    {
        _vect.insert(to_inner_iter(pos), &item);
    }

    void erase(iterator b, iterator e)
    {
        _vect.erase(to_inner_iter(b), to_inner_iter(e));
    }

    void erase(iterator pos)
    {
        _vect.erase(to_inner_iter(pos));
    }

    reference front() const
    {
        return *begin();
    }

    reference back() const
    {
        return *std::prev(end());
    }

    reference operator [](size_type index) const
    {
        return *std::next(begin(), index);
    }

    reference at(size_type index) const
    {
        return (*this)[index];
    }

private:
    inner_const_iter to_inner_iter(iterator iter)
    {
        return std::next(_vect.begin(), std::distance(begin(), iter));
    }

    std::vector<pointer> _vect;
};

template <class T>
using cref_vector = ref_vector<const T>;

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_REF_VECTOR_HPP_ */
