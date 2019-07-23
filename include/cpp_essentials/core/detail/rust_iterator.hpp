#ifndef CPP_ESSENTIALS_CORE_DETAIL_RUST_ITERATOR_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_RUST_ITERATOR_HPP_

#pragma once

#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/core/optional.hpp>

#include <cpp_essentials/core/detail/iterator_func_helper.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Func>
using rust_iterator_ref = core::underlying_type_t<std::invoke_result_t<Func>>;

template <class Func>
class rust_iterator : public core::iterator_facade<rust_iterator<Func>, std::forward_iterator_tag, rust_iterator_ref<Func>>
{
public:
    using base_type = core::iterator_facade<
        rust_iterator<Func>,
        std::forward_iterator_tag,
        rust_iterator_ref<Func>>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type);

    rust_iterator()
        : _func{}
        , _current{}
        , _counter{ std::numeric_limits<difference_type>::max() }
    {
    }

    rust_iterator(Func func)
        : _func{ std::move(func) }
        , _current{ _func() }
        , _counter{}
    {

    }

    void inc()
    {
        _current = _func();
        ++_counter;
    }

    reference ref() const
    {
        return *_current;
    }

    bool is_equal(const rust_iterator& other) const
    {
        return !_current.has_value() || _counter == other._counter;
    }

private:
    core::detail::default_constructible_func<Func> _func;
    mutable core::optional<reference> _current;
    difference_type _counter;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_RUST_ITERATOR_HPP_ */
