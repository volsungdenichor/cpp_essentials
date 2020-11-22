#pragma once

#include <millrind/core/iterator_facade.hpp>
#include <millrind/core/functors.hpp>

namespace millrind::core
{

template <class Pred, class Iter>
class filter_iterator : public iterator_facade<filter_iterator<Pred, Iter>>
{
public:
    filter_iterator() = default;

    filter_iterator(Pred pred, Iter iter, Iter end)
        : _pred{ std::move(pred) }
        , _iter{ std::move(iter) }
        , _end{ std::move(end) }
    {
        update();
    }

    filter_iterator(const filter_iterator&) = default;

    filter_iterator(filter_iterator&&) = default;

    decltype(auto) deref() const
    {
        return *_iter;
    }

    void inc()
    {
        ++_iter;
        update();
    }

    template <class It = Iter, class = BidirectionalIterator<It>>
    void dec()
    {
        --_iter;

        while (!invoke_func(_pred, *_iter))
        {
            --_iter;
        }
    }

    bool is_equal(const filter_iterator& other) const
    {
        return _iter == other._iter;
    }

    template <class It = Iter, class = RandomAccessIterator<It>>
    bool is_less(const filter_iterator& other) const
    {
        return _iter < other._iter;
    }

private:
    void update()
    {
        while (_iter != _end && !invoke_func(_pred, *_iter))
        {
            ++_iter;
        }
    }

    detail::default_constructible_func<Pred> _pred;
    Iter _iter;
    Iter _end;
};

} // namespace millrind::core

CORE_ITERATOR_TRAIRS(::millrind::core::filter_iterator)