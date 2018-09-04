#ifndef CPP_ESSENTIALS_SQ_DETAIL_CHAIN_ITERATOR_HPP_
#define CPP_ESSENTIALS_SQ_DETAIL_CHAIN_ITERATOR_HPP_

#pragma once

#include <cpp_essentials/concepts/concepts.hpp>
#include <cpp_essentials/core/iterator_facade.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

template <class Iter1, class Iter2>
struct chain_iterator_helper
{
private:
    using result1_type = concepts::iterator_reference<Iter1>;
    using result2_type = concepts::iterator_reference<Iter2>;

    using value1_type = std::remove_const_t<std::remove_reference_t<result1_type>>;
    using value2_type = std::remove_const_t<std::remove_reference_t<result2_type>>;

    static_assert(std::is_convertible<value1_type, value2_type>::value, "chain_iterator: invalid type");

public:
    using type = std::common_type_t<result1_type, result2_type>;
};



template <class Iter1, class Iter2>
class chain_iterator
    : public core::iterator_facade
        < chain_iterator<Iter1, Iter2>
        , std::bidirectional_iterator_tag
        , typename chain_iterator_helper<Iter1, Iter2>::type>
{
public:
    using base_type = core::iterator_facade
        < chain_iterator<Iter1, Iter2>
        , std::bidirectional_iterator_tag
        , typename chain_iterator_helper<Iter1, Iter2>::type>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    chain_iterator(Iter1 iter1, Iter2 iter2, Iter1 end, Iter2 begin)
        : _iter1{ iter1 }
        , _iter2{ iter2 }
        , range1_end{ end }
        , range2_begin{ begin }
    {
    }

    void inc()
    {
        if (_iter1 != range1_end)
        {
            ++_iter1;
        }
        else
        {
            ++_iter2;
        }
    }

    void dec()
    {
        if (_iter2 != range2_begin)
        {
            --_iter2;
        }
        else
        {
            --_iter1;
        }
    }

    reference ref() const
    {
        return _iter1 != range1_end
            ? value_type(*_iter1)
            : value_type(*_iter2);
    }

    bool is_equal(const chain_iterator& other) const
    {
        return _iter1 == other._iter1 && _iter2 == other._iter2;
    }

private:
    Iter1 _iter1;
    Iter2 _iter2;

    Iter1 range1_end;
    Iter2 range2_begin;
};

} /* namespace detail */

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_DETAIL_CHAIN_ITERATOR_HPP_ */