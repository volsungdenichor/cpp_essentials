#pragma once

#include <millrind/core/iterator_facade.hpp>
#include <millrind/core/functors.hpp>
#if 0
#include "../optional.hpp"

namespace millrind::core
{

template <class Func, class Iter>
class filter_map_iterator : public iterator_facade<filter_map_iterator<Func, Iter>>
{
private:
    using optional_type = std::invoke_result_t<Func, core::iter_ref<Iter>>;

public:
    filter_map_iterator() = default;

    filter_map_iterator(Func func, Iter iter, Iter end)
        : _func{ std::move(func) }
        , _iter{ std::move(iter) }
        , _end{ std::move(end) }
        , _current{}
    {
        update();
    }

    filter_map_iterator(const filter_map_iterator&) = default;

    filter_map_iterator(filter_map_iterator&&) = default;

    decltype(auto) deref() const
    {
        return *_current;
    }

    void inc()
    {
        ++_iter;
        update();
    }

    bool is_equal(const filter_map_iterator& other) const
    {
        return _iter == other._iter;
    }

private:
    void update()
    {
        while (_iter != _end)
        {
            if (_current = _func(*_iter); _current)
            {
                return;
            }
            ++_iter;
        }
    }

    detail::default_constructible_func <Func> _func;
    Iter _iter;
    Iter _end;
    optional_type _current;
};

} // namespace millrind::core

CORE_ITERATOR_TRAIRS(::millrind::core::filter_map_iterator)
#endif