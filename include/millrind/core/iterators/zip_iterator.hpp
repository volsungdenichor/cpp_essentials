#pragma once

#include <millrind/core/iterator_facade.hpp>
#include <millrind/core/functors.hpp>

namespace millrind::core
{

template <class Func, class... Iters>
class zip_iterator : public iterator_facade<zip_iterator<Func, Iters...>>
{
private:
    using index_seq = std::index_sequence_for<Iters...>;

public:
    zip_iterator() = default;

    zip_iterator(Func func, Iters... iters)
        : _func{ std::move(func) }
        , _iters{ std::move(iters)... }
    {
    }

    zip_iterator(const zip_iterator&) = default;

    zip_iterator(zip_iterator&&) = default;

    decltype(auto) deref() const
    {
        return deref(index_seq{});
    }

    void inc()
    {
        inc(index_seq{});
    }

    void dec()
    {
        dec(index_seq{});
    }

    bool is_equal(const zip_iterator& other) const
    {
        return std::get<0>(_iters) == std::get<0>(other._iters);
    }

private:
    template <std::size_t... I>
    decltype(auto) deref(std::index_sequence<I...>) const
    {
        return invoke_func(_func, *std::get<I>(_iters)...);
    }

    template <std::size_t... I>
    void inc(std::index_sequence<I...>)
    {
        ignore(++std::get<I>(_iters)...);
    }

    template <std::size_t... I>
    void dec(std::index_sequence<I...>)
    {
        ignore(--std::get<I>(_iters)...);
    }

    template <class... T>
    void ignore(T&&...)
    {
    }

    std::tuple<Iters...> _iters;
    detail::default_constructible_func <Func> _func;
};

} // namespace millrind::core

CORE_ITERATOR_TRAIRS(::millrind::core::zip_iterator)