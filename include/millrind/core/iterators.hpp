#pragma once

#include <millrind/core/iterators/any_iterator.hpp>
#include <millrind/core/iterators/map_iterator.hpp>
#include <millrind/core/iterators/filter_iterator.hpp>
#include <millrind/core/iterators/iterate_iterator.hpp>
#include <millrind/core/iterators/chain_iterator.hpp>
#include <millrind/core/iterators/filter_map_iterator.hpp>
#include <millrind/core/iterators/flat_map_iterator.hpp>
#include <millrind/core/iterators/generating_iterator.hpp>
#include <millrind/core/iterators/zip_iterator.hpp>
#include <millrind/core/iterators/repeat_iterator.hpp>
#include <millrind/core/iterators/owning_iterator.hpp>
#include <millrind/core/iterators/numeric_iterator.hpp>
#include <millrind/core/iterators/any_iterator.hpp>

namespace millrind::core
{

struct make_map_iterator_fn
{
    template <class Func, class Iter>
    auto operator()(Func func, Iter iter) const
    {
        using result_type = map_iterator<Func, Iter>;
        return result_type{ std::move(func), std::move(iter) };
    }    
};

static constexpr auto make_map_iterator = make_map_iterator_fn{};

struct make_filter_iterator_fn
{
    template <class Pred, class Iter>
    auto operator()(Pred pred, Iter iter, Iter end) const
    {
        using result_type = filter_iterator<Pred, Iter>;
        return result_type{ std::move(pred), std::move(iter), std::move(end) };
    }
};

static constexpr auto make_filter_iterator = make_filter_iterator_fn{};

struct make_iterate_iterator_fn
{
    template <class Iter>
    auto operator()(Iter iter) const
    {
        using result_type = iterate_iterator<Iter>;
        return result_type{ std::move(iter) };
    }
};

static constexpr auto make_iterate_iterator = make_iterate_iterator_fn{};

struct make_flat_map_iterator_fn
{
    template <class Func, class Iter>
    auto operator()(Func func, Iter iter, Iter end) const
    {
        using result_type = flat_map_iterator<Func, Iter>;
        return result_type{ std::move(func), std::move(iter), std::move(end) };
    }
};

static constexpr auto make_flat_map_iterator = make_flat_map_iterator_fn{};

struct make_filter_map_iterator_fn
{
    template <class Func, class Iter>
    auto operator()(Func func, Iter iter, Iter end) const
    {
        using result_type = filter_map_iterator<Func, Iter>;
        return result_type{ std::move(func), std::move(iter), std::move(end) };
    }
};

static constexpr auto make_filter_map_iterator = make_filter_map_iterator_fn{};

struct make_chain_iterator_fn
{
    template <class Iter1, class Iter2>
    auto operator()(Iter1 iter1, Iter2 iter2, Iter1 end1, Iter2 begin2) const
    {
        using result_type = chain_iterator<Iter1, Iter2>;
        return result_type{ std::move(iter1), std::move(iter2), std::move(end1), std::move(begin2) };
    }
};

static constexpr auto make_chain_iterator = make_chain_iterator_fn{};

struct make_zip_iterator_fn
{

    template <class Func, class... Iters>
    auto operator()(Func func, Iters... args) const
    {
        using result_type = zip_iterator<Func, Iters...>;
        return result_type{ std::move(func), std::move(args)... };
    }
};

static constexpr auto make_zip_iterator = make_zip_iterator_fn{};

struct make_owning_iterator_fn
{
    template <class Container, class Iter>
    auto operator()(std::shared_ptr<Container> container, Iter iter) const
    {
        using result_type = owning_iterator<Iter, Container>;
        return result_type{ std::move(iter), std::move(container) };
    }
};

static constexpr auto make_owning_iterator = make_owning_iterator_fn{};

struct make_reverse_iterator_fn
{
    template <class Iter>
    auto operator()(Iter iter) const
    {
        using result_type = std::reverse_iterator<Iter>;
        return result_type{ iter };
    }

    template <class Iter>
    auto operator()(std::reverse_iterator<Iter> iter) const
    {
        using result_type = Iter;
        return result_type{ iter.base() };
    }
};

static constexpr auto make_reverse_iterator = make_reverse_iterator_fn{};

struct make_numeric_iterator_fn
{
    template <class T>
    auto operator()(T value) const
    {
        using result_type = numeric_iterator<T>;
        return result_type{ value };
    }
};

static constexpr auto make_numeric_iterator = make_numeric_iterator_fn{};

struct make_repeat_iterator_fn
{
    template <class T>
    auto operator()(T value, std::ptrdiff_t index) const
    {
        using result_type = repeat_iterator<T>;
        return result_type{ std::move(value), index };
    }
};

static constexpr auto make_repeat_iterator = make_repeat_iterator_fn{};

struct make_generating_iterator_fn
{
    template <class Func>
    auto operator()(Func func) const
    {
        using result_type = generating_iterator<Func>;
        return result_type{ std::move(func) };
    }
};

static constexpr auto make_generating_iterator = make_generating_iterator_fn{};

} // namespace millrind::core
