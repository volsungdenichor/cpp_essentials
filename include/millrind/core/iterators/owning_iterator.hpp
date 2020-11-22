#pragma once

#include <memory>

#include <millrind/core/iterator_facade.hpp>
#include <millrind/core/functors.hpp>

namespace millrind::core
{

template <class Iter, class Owner>
class owning_iterator : public iterator_facade<owning_iterator<Iter, Owner>>
{
public:
    owning_iterator() = default;

    owning_iterator(Iter iter, std::shared_ptr<Owner> owner)
        : _iter{ iter }
        , _owner{ std::move(owner) }
    {
    }

    owning_iterator(const owning_iterator&) = default;

    owning_iterator(owning_iterator&&) = default;

    owning_iterator& operator=(owning_iterator other)
    {
        std::swap(_iter, other._iter);
        std::swap(_owner, other._owner);
        return *this;
    }

    decltype(auto) deref() const
    {
        return *_iter;
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

    bool is_equal(const owning_iterator& other) const
    {
        return _iter == other._iter;
    }

    template <class It = Iter, class = RandomAccessIterator<It>>
    bool is_less(const owning_iterator& other) const
    {
        return _iter < other._iter;
    }

    template <class It = Iter, class = RandomAccessIterator<It>>
    void advance(std::ptrdiff_t offset)
    {
        _iter += offset;
    }

    template <class It = Iter, class = RandomAccessIterator<It>>
    auto distance_to(const owning_iterator& other) const
    {
        return other._iter - _iter;
    }

private:
    Iter _iter;
    std::shared_ptr<Owner> _owner;
};

} // namespace millrind::core

CORE_ITERATOR_TRAIRS(::millrind::core::owning_iterator)
