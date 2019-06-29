#ifndef CPP_ESSENTIALS_CORE_DETAIL_RUST_ITERATOR_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_RUST_ITERATOR_HPP_

#pragma once

#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/core/optional.hpp>

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
    {
    }

    rust_iterator(Func func)
        : _func{ std::move(func) }
    {
        _current = _func();
    }

    void inc()
    {
        _current = _func();
    }

    reference ref() const
    {
        return *_current;
    }

    bool is_equal(const rust_iterator&) const
    {
        return !_current.has_value();
    }

private:
    core::detail::default_constructible_func<Func> _func;
    mutable core::optional<reference> _current;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_RUST_ITERATOR_HPP_ */
