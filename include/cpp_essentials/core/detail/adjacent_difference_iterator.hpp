#pragma once

#ifndef CPP_ESSENTIALS_CORE_DETAIL_ADJACENT_DIFFERENCE_ITERATOR_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_ADJACENT_DIFFERENCE_ITERATOR_HPP_

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/core/detail/iterator_func_helper.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Iter, class Func>
class adjacent_difference_iterator : public core::iterator_facade
    < adjacent_difference_iterator<Iter, Func>
    , std::forward_iterator_tag
    , std::invoke_result_t<Func, cc::iter_val<Iter>, cc::iter_val<Iter>>>
{
public:
    using base_type = core::iterator_facade
        < adjacent_difference_iterator<Iter, Func>
        , std::forward_iterator_tag
        , std::invoke_result_t<Func, cc::iter_val<Iter>, cc::iter_val<Iter>>>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    adjacent_difference_iterator() = default;

    adjacent_difference_iterator(Iter iter, Func func, Iter end)
        : _iter{ iter }
        , _end{ end }
        , _func{ func }
        , _prev{}
    {
    }

    reference ref() const
    {
        return _prev
            ? _func(*_iter, *_prev)
            : *_iter;
    }

    void inc()
    {
        update();
        ++_iter;
    }

    bool is_equal(const adjacent_difference_iterator& other) const
    {
        return _iter == other._iter;
    }

private:
    void update()
    {
        if (_iter != _end)
        {
            _prev = *_iter;
        }
    }
    
    Iter _iter;
    Iter _end;
    default_constructible_func<Func> _func;
    core::optional<cc::iter_val<Iter>> _prev;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_ADJACENT_DIFFERENCE_ITERATOR_HPP_ */

