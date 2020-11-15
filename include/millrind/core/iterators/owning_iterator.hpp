#pragma once

#include <memory>

#include "../iterator_facade.hpp"
#include "../functors.hpp"


namespace core
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

    template <class It = Iter, class = bidirectional_iter<It>>
    void dec()
    {
        --_iter;
    }

    bool is_equal(const owning_iterator& other) const
    {
        return _iter == other._iter;
    }

    template <class It = Iter, class = random_access_iter<It>>
    bool is_less(const owning_iterator& other) const
    {
        return _iter < other._iter;
    }

    template <class It = Iter, class = random_access_iter<It>>
    void advance(std::ptrdiff_t offset)
    {
        _iter += offset;
    }

    template <class It = Iter, class = random_access_iter<It>>
    auto distance_to(const owning_iterator& other) const
    {
        return other._iter - _iter;
    }

private:
    Iter _iter;
    std::shared_ptr<Owner> _owner;
};

} // namespace core

CORE_ITERATOR_TRAIRS(::core::owning_iterator)
