#ifndef CPP_ESSENTIALS_CORE_DETAIL_FILTER_ITERATOR_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_FILTER_ITERATOR_HPP_

#pragma once

#include <cpp_essentials/cc/cc.hpp>

#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/core/iterator_range.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Iter, class Pred>
class filter_iterator
    : public core::iterator_facade
        < filter_iterator<Iter, Pred>
        , std::bidirectional_iterator_tag
        , cc::iter_ref<Iter>
        , cc::iter_diff<Iter>>
{
public:
    using base_type = core::iterator_facade
        < filter_iterator<Iter, Pred>
        , std::bidirectional_iterator_tag
        , cc::iter_ref<Iter>
        , cc::iter_diff<Iter>>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    filter_iterator(Iter iter, Pred pred, Iter last)
        : _iter{ iter }
        , _pred{ pred }
        , _last{ last }
    {
        update();
    }

    reference ref() const
    {
        return *_iter;
    }

    void inc()
    {
        ++_iter;
        update();
    }

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

    bool is_less(const filter_iterator& other) const
    {
        return _iter < other._iter;
    }

private:
    void update()
    {
        while (_iter != _last && !_pred(*_iter))
        {
            ++_iter;
        }
    }

    Iter _iter;
    Pred _pred;
    Iter _last;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_FILTER_ITERATOR_HPP_ */
