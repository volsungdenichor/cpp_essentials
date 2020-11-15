#pragma once

#include "../iterator_facade.hpp"
#include "../functors.hpp"

namespace core
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

    template <class It = Iter, class = bidirectional_iter <It>>
    void dec()
    {
        --_iter;

        while (!_pred(*_iter))
        {
            --_iter;
        }
    }

    bool is_equal(const filter_iterator& other) const
    {
        return _iter == other._iter;
    }

    template <class It = Iter, class = random_access_iter <It>>
    bool is_less(const filter_iterator& other) const
    {
        return _iter < other._iter;
    }

private:
    void update()
    {
        while (_iter != _end && !_pred(*_iter))
        {
            ++_iter;
        }
    }

    detail::default_constructible_func <Pred> _pred;
    Iter _iter;
    Iter _end;
};

} // namespace core

CORE_ITERATOR_TRAIRS(::core::filter_iterator)