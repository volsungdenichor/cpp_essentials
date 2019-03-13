#ifndef CPP_ESSENTIALS_CORE_DETAIL_CHUNK_ITERATOR_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_CHUNK_ITERATOR_HPP_

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/core/iterator_range.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Iter>
class chunk_iterator
    : public core::iterator_facade
        < chunk_iterator<Iter>
        , std::forward_iterator_tag
        , core::iterator_range<Iter>
        , cc::iter_diff<Iter>>
{
public:
    using base_type = core::iterator_facade
        < chunk_iterator<Iter>
        , std::forward_iterator_tag
        , core::iterator_range<Iter>
        , cc::iter_diff<Iter>>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    chunk_iterator() = default;

    chunk_iterator(Iter begin, Iter end, difference_type step, Iter last)
        : _begin{ begin }
        , _end{ end }
        , _step{ step }
        , _last{ last }
    {
    }

    reference ref() const
    {
        return core::make_range(_begin, _end);
    }

    void inc()
    {
        _begin = core::advance(_begin, _last, _step);
        _end = core::advance(_end, _last, _step);
    }

    bool is_equal(const chunk_iterator& other) const
    {
        return _begin == other._begin;
    }

private:
    Iter _begin;
    Iter _end;
    difference_type _step;
    Iter _last;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_CHUNK_ITERATOR_HPP_ */
