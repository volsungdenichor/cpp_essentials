#ifndef CPP_ESSENTIALS_CORE_DETAIL_FLATTEN_ITERATOR_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_FLATTEN_ITERATOR_HPP_

#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/cc/cc.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Outer, class Inner = cc::range_iter<cc::iter_ref<Outer>>>
class flatten_iterator
    : public core::iterator_facade
        < flatten_iterator<Outer, Inner>
        , std::forward_iterator_tag
        , cc::iter_ref<Inner>>
{
public:
    using base_type = core::iterator_facade
        < flatten_iterator<Outer, Inner>
        , std::forward_iterator_tag
        , cc::iter_ref<Inner>>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    flatten_iterator() = default;

    flatten_iterator(Outer outer, Outer outer_end)
        : _outer{ outer }
        , _outer_end{ outer_end }
    {
        if (_outer == _outer_end)
        {
            return;
        }

        _inner = std::begin(*_outer);
        update();
    }

    reference ref() const
    {
        return *_inner;
    }

    void inc()
    {
        ++_inner;
        if (_inner == std::end(*_outer))
        {
            update();
        }
    }

    bool is_equal(const flatten_iterator& other) const
    {
        return _outer == other._outer
            && (_outer == _outer_end || other._outer == other._outer_end || _inner == other._inner);
    }

private:
    void update()
    {
        while (_outer != _outer_end && _inner == std::end(*_outer))
        {
            ++_outer;
            if (_outer != _outer_end)
            {
                _inner = std::begin(*_outer);
            }
        }
    }
    Outer _outer;
    Outer _outer_end;
    Inner _inner;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_FLATTEN_ITERATOR_HPP_ */
