#pragma once

#include <millrind/core/iterator_facade.hpp>
#include <millrind/core/functors.hpp>

namespace millrind::core
{

template <class Iter>
class iterate_iterator : public iterator_facade<iterate_iterator<Iter>>
{
public:
    iterate_iterator() = default;

    iterate_iterator(Iter iter)
        : _iter{ std::move(iter) }
    {
    }

    iterate_iterator(const iterate_iterator&) = default;

    iterate_iterator(iterate_iterator&&) = default;

    auto deref() const
    {
        return _iter;
    }

    void inc()
    {
        ++_iter;
    }

    template <class It = Iter, class = BidirectionalIterator<It>>
    void dec()
    {
        --_iter;
    }

    bool is_equal(const iterate_iterator& other) const
    {
        return _iter == other._iter;
    }

    template <class It = Iter, class = RandomAccessIterator<It>>
    bool is_less(const iterate_iterator& other) const
    {
        return _iter < other._iter;
    }

    template <class It = Iter, class = RandomAccessIterator<It>>
    void advance(std::ptrdiff_t offset)
    {
        _iter += offset;
    }

    template <class It = Iter, class = RandomAccessIterator<It>>
    auto distance_to(const iterate_iterator& other) const
    {
        return other._iter - _iter;
    }

private:
    Iter _iter;
};

} // namespace millrind::core

CORE_ITERATOR_TRAIRS(::millrind::core::iterate_iterator)
