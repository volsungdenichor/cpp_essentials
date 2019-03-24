#ifndef CPP_ESSENTIALS_CORE_DETAIL_flat_map_iterator_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_flat_map_iterator_HPP_

#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/optional.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Iter, class Func>
class flat_map_iterator
    : public core::iterator_facade
    < flat_map_iterator<Iter, Func>
    , std::forward_iterator_tag
    , underlying_type_t<typename iterator_func_helper<Func, Iter>::type>>
{
public:   
    using base_type = core::iterator_facade
        < flat_map_iterator<Iter, Func>
        , std::forward_iterator_tag
        , underlying_type_t<typename iterator_func_helper<Func, Iter>::type>>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    flat_map_iterator() = default;

    flat_map_iterator(Iter first, Func func, Iter last)
        : _iter{ first }
        , _func{ func }
        , _last{ last }
    {
        update();
    }

    reference ref() const
    {
        return *_current;
    }

    void inc()
    {       
        ++_iter;
        update();
    }

    bool is_equal(const flat_map_iterator& other) const
    {
        return _iter == other._iter;
    }

private:
    void update()
    {
        while (_iter != _last)
        {
            _current = _func(*_iter);
            if (_current)
            {
                return;
            }
            ++_iter;
        }
    }

    Iter _iter;
    default_constructible_func<Func> _func;
    Iter _last;
    core::optional<reference> _current;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_flat_map_iterator_HPP_ */
