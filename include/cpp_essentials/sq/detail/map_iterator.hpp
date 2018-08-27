#ifndef CPP_ESSENTIALS_SQ_DETAIL_MAP_ITERATOR_HPP_
#define CPP_ESSENTIALS_SQ_DETAIL_MAP_ITERATOR_HPP_

#pragma once

#include <cpp_essentials/concepts/concepts.hpp>

#include <cpp_essentials/core/iterator_facade.hpp>

#include <cpp_essentials/sq/detail/iterator_func_helper.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

template <class Iter, class Func>
class map_iterator
    : public core::iterator_facade
        < map_iterator<Iter, Func>
        , concepts::iterator_category<Iter>
        , typename iterator_func_helper<Func, Iter>::type
        , concepts::iterator_difference<Iter>>
{
public:
    using base_type = core::iterator_facade
        < map_iterator<Iter, Func>
        , concepts::iterator_category<Iter>
        , typename iterator_func_helper<Func, Iter>::type
        , concepts::iterator_difference<Iter>>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    map_iterator(Iter iter, Func func)
        : _iter{ iter }
        , _func{ func }
    {
    }

    reference ref() const
    {
        return _func(*_iter);
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

    bool is_equal(const map_iterator& other) const
    {
        return _iter == other._iter;
    }

    bool is_less(const map_iterator& other) const
    {
        return _iter < other._iter;
    }

    difference_type distance(const map_iterator& other) const
    {
        return std::distance(_iter, other._iter);
    }

private:
    Iter _iter;
    Func _func;
};

} /* namespace detail */

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_DETAIL_MAP_ITERATOR_HPP_ */