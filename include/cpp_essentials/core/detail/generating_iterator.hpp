#ifndef CPP_ESSENTIALS_CORE_DETAIL_GENERATING_ITERATOR_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_GENERATING_ITERATOR_HPP_

#pragma once

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/iterator_facade.hpp>

#include <cpp_essentials/core/detail/iterator_func_helper.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Func>
class generating_iterator
    : public core::iterator_facade
    < generating_iterator<Func>
    , std::forward_iterator_tag
    , std::invoke_result_t<Func>>
{
public:
    using base_type = core::iterator_facade
        < generating_iterator<Func>
        , std::forward_iterator_tag
        , std::invoke_result_t<Func>>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    generating_iterator()
        : _func{}
        , _counter{ std::numeric_limits<difference_type>::max() }
    {
    }

    generating_iterator(Func func)
        : _func{ std::move(func) }
        , _counter{}
    {
    }

    void inc()
    {
        ++_counter;
    }

    reference ref() const
    {
        return _func();
    }

    bool is_equal(const generating_iterator& other) const
    {
        return _counter == other._counter;
    }

private:
    core::detail::default_constructible_func<Func> _func;
    difference_type _counter;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_GENERATING_ITERATOR_HPP_ */
