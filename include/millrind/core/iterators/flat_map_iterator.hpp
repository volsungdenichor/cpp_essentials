#pragma once

#include <millrind/core/iterator_facade.hpp>
#include <millrind/core/functors.hpp>

namespace millrind::core
{

template <class Func, class Outer, class Inner = range_iter<std::invoke_result_t<Func, iter_ref<Outer>>>>

class flat_map_iterator : public iterator_facade<flat_map_iterator<Func, Outer, Inner>>
{
public:
    flat_map_iterator() = default;

    flat_map_iterator(Func func, Outer outer, Outer outer_end)
        : _func{ std::move(func) }
        , _outer{ std::move(outer) }
        , _outer_end{ std::move(outer_end) }
        , _inner{}
        , _inner_end{}
    {
        if (_outer != _outer_end)
        {
            auto res = invoke_func(_func, *_outer);
            _inner = std::begin(res);
            _inner_end = std::end(res);
            update();
        }
    }

    flat_map_iterator(const flat_map_iterator&) = default;

    flat_map_iterator(flat_map_iterator&&) = default;

    decltype(auto) deref() const
    {
        return *_inner;
    }

    void inc()
    {
        if (++_inner == _inner_end)
        {
            update();
        }
    }

    bool is_equal(const flat_map_iterator& other) const
    {
        return _outer == other._outer
            && (_outer == _outer_end || other._outer == other._outer_end || _inner == other._inner);
    }

private:
    void update()
    {
        while (_outer != _outer_end && _inner == _inner_end)
        {
            if (++_outer != _outer_end)
            {
                auto res = invoke_func(_func, *_outer);
                _inner = std::begin(res);
                _inner_end = std::end(res);
            }
        }
    }

    detail::default_constructible_func<Func> _func;
    Outer _outer;
    Outer _outer_end;
    Inner _inner;
    Inner _inner_end;
};

} // namespace millrind::core

CORE_ITERATOR_TRAIRS(::millrind::core::flat_map_iterator)