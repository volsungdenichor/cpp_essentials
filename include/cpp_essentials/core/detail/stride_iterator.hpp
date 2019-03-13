#ifndef CPP_ESSENTIALS_CORE_DETAIL_STRIDE_ITERATOR_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_STRIDE_ITERATOR_HPP_

#include <cpp_essentials/cc/cc.hpp>

#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/core/iterator_range.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Iter>
class stride_iterator
    : public core::iterator_facade
        < stride_iterator<Iter>
        , std::input_iterator_tag
        , cc::iter_ref<Iter>
        , cc::iter_diff<Iter>>
{
public:
    using base_type = core::iterator_facade
        < stride_iterator<Iter>
        , std::input_iterator_tag
        , cc::iter_ref<Iter>
        , cc::iter_diff<Iter>>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    stride_iterator() = default;

    stride_iterator(Iter iter, difference_type step, Iter last)
        : _iter{ iter }
        , _step{ step }
        , _last{ last }
    {
    }

    reference ref() const
    {
        return *_iter;
    }

    void inc()
    {
        _iter = core::advance(_iter, _last, _step);
    }

    bool is_equal(const stride_iterator& other) const
    {
        return _iter == other._iter;
    }

private:
    Iter _iter;
    difference_type _step;
    Iter _last;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_STRIDE_ITERATOR_HPP_ */
