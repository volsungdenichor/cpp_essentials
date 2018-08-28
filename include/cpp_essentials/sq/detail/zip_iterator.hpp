#ifndef CPP_ESSENTIALS_SQ_DETAIL_ZIP_ITERATOR_HPP_
#define CPP_ESSENTIALS_SQ_DETAIL_ZIP_ITERATOR_HPP_

#pragma once

#include <tuple>

#include <cpp_essentials/concepts/concepts.hpp>
#include <cpp_essentials/core/iterator_facade.hpp>
#include <cpp_essentials/sq/detail/iterator_func_helper.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

template <int...>
struct sequence { };

template <int N, int... S>
struct gens : gens<N - 1, N - 1, S...> { };

template <int... S>
struct gens<0, S...>
{
    using type = sequence<S...>;
};

template <class Func, class... Iters>
class zip_iterator
    : public core::iterator_facade
        < zip_iterator<Func, Iters...>
        , std::bidirectional_iterator_tag
        , typename iterator_func_helper<Func, Iters...>::type>
{
private:
    using sequence_type = typename gens<sizeof...(Iters)>::type;

    static constexpr sequence_type seq = sequence_type {};

public:
    using base_type = core::iterator_facade
        < zip_iterator<Func, Iters...>
        , std::bidirectional_iterator_tag
        , typename iterator_func_helper<Func, Iters...>::type>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    zip_iterator(Func func, const Iters&... args)
        : _func{ func }
        , _current{ args... }
    {
    }

    reference ref() const
    {
        return deref(seq);
    }

    void inc()
    {
        inc(seq);
    }

    void dec()
    {
        dec(seq);
    }

    bool is_equal(const zip_iterator& other) const
    {
        return std::get<0>(_current) == std::get<0>(other._current);
    }

private:
    template <class... T>
    void ignore(T&&...)
    {
    }

    template <int... S>
    reference deref(sequence<S...>) const
    {
        return _func(*std::get<S>(_current)...);
    }

    template <int... S>
    void inc(sequence<S...>)
    {
        ignore(++std::get<S>(_current)...);
    }

    template <int... S>
    void dec(sequence<S...>)
    {
        ignore(--std::get<S>(_current)...);
    }

    Func _func;
    std::tuple<Iters...> _current;
};



template <class Func, class... Iters>
auto make_zip_iterator(Func&& func, const Iters&... args)
{
    return zip_iterator<std::remove_reference_t<Func>, Iters...> { func, args... };
}

template <typename Func, typename... Ranges>
auto zip(Func&& func, Ranges&&... ranges)
{
    return make_range(
        make_zip_iterator(func, std::begin(ranges)...),
        make_zip_iterator(func, std::end(ranges)...));
}

} /* namespace detail */

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_DETAIL_ZIP_ITERATOR_HPP_ */