#pragma once

#include <millrind/core/iterator_facade.hpp>
#include <millrind/core/functors.hpp>

namespace millrind::core
{

template <class Func>
class generating_iterator : public iterator_facade<generating_iterator<Func>>
{
private:
    using optional_type = std::invoke_result_t<Func>;

public:
    generating_iterator()
        : _func{ }
        , _current{ }
        , _index{ std::numeric_limits<std::ptrdiff_t>::max() }
    {
    }

    generating_iterator(Func func)
        : _func{ std::move(func) }
        , _current{ invoke_func(_func) }
        , _index{ 0 }
    {
    }

    generating_iterator(const generating_iterator&) = default;

    generating_iterator(generating_iterator&&) = default;    

    auto deref() const -> typename optional_type::const_reference
    {
        return *_current;
    }

    void inc()
    {
        _current = _func();
        ++_index;
    }

    bool is_equal(const generating_iterator& other) const
    {
        return !_current || _index == other._index;
    }

private:
    detail::default_constructible_func<Func> _func;
    std::ptrdiff_t _index;
    optional_type _current;
};

} // namespace millrind::core

CORE_ITERATOR_TRAIRS(::millrind::core::generating_iterator)
