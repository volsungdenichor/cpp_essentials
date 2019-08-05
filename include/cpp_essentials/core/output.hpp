#ifndef CPP_ESSENTIALS_CORE_OUTPUT_HPP_
#define CPP_ESSENTIALS_CORE_OUTPUT_HPP_

#pragma once

#include <iostream>
#include <functional>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/algorithm.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct ostream_manipulator
{
    using function_type = std::function<void(std::ostream&)>;
    
    function_type func;

    friend std::ostream& operator <<(std::ostream& os, const ostream_manipulator& item)
    {
        item.func(os);
        return os;
    }
};

inline auto make_ostream_manipulator(ostream_manipulator::function_type func) -> ostream_manipulator
{
    return { std::move(func) };
}


template <class Func>
class output_iterator_t
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using reference = void;
    using pointer = void;
    using difference_type = void;

    struct proxy_t
    {
        Func _func;

        template <class T>
        void operator ()(const T& item) const
        {
            _func(item);
        }

        template <class T>
        void operator =(const T& item) const
        {
            (*this)(item);
        }
    };

    output_iterator_t(Func func)
        : _proxy{ std::move(func) }
    {
    }

    output_iterator_t& operator =(const output_iterator_t&)
    {
        return *this;
    }

    output_iterator_t& operator ++()
    {
        return *this;
    }

    output_iterator_t& operator ++(int)
    {
        return *this;
    }

    proxy_t& operator *()
    {
        return _proxy;
    }

    template <class T>
    void operator ()(const T& item) const
    {
        _proxy(item);
    }

private:
    proxy_t _proxy;
};

struct make_output_iterator_fn
{
    template <class Func>
    auto operator ()(Func func) const -> output_iterator_t<Func>
    {
        return { std::move(func) };
    }
};

static constexpr make_output_iterator_fn make_output_iterator = {};


template <class C, class T>
struct output_t
{
    output_t(std::basic_ostream<C>& os, const C* separator)
        : os{ &os }
        , separator{ separator }
    {
    }

    void operator ()(const T& value) const
    {
        *os << value;
        if (separator) *os << separator;
    }

    std::basic_ostream<C>* os;
    const C* separator;
};

template <class C>
struct output_t<C, void>
{
    output_t(std::basic_ostream<C>& os, const C* separator)
        : os{ &os }
        , separator{ separator }
    {
    }

    template <class T>
    void operator ()(const T& value) const
    {
        *os << value;
        if (separator) *os << separator;
    }

    std::basic_ostream<C>* os;
    const C* separator;
};

template <class Iter, class C>
ostream_manipulator delimit(Iter begin, Iter end, const C* separator)
{
    return make_ostream_manipulator([=](std::ostream& os)
    {
        for (auto it = begin; it != end; ++it)
        {
            if (it != begin && separator != nullptr)
            {
                os << separator;
            }

            os << *it;
        }
    });
}

struct output_fn
{
    template <class T = void, class C>
    auto operator ()(std::basic_ostream<C>& os, const C* separator = nullptr) const
    {
        return make_output_iterator(output_t<C, T>{ os, separator });
    }
};

struct write_fn
{
    template <class C, class Range, CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, std::basic_ostream<C>& os, const C* separator = nullptr) const
    {
        static constexpr copy_fn _copy = {};
        static constexpr output_fn _output = {};
        return _copy(range, _output(os, separator));
    }
};

struct delimit_fn
{
    template
        < class Range
        , class C
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, const C* separator) const -> ostream_manipulator
    {
        return delimit(std::begin(range), std::end(range), separator);
    }
};

} /* namespace detail */

using detail::make_output_iterator;

using detail::ostream_manipulator;
using detail::make_ostream_manipulator;

static constexpr auto output = detail::output_fn{};
static constexpr auto delimit = detail::delimit_fn{};

static constexpr auto write = adaptable{ detail::write_fn{} };

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_OUTPUT_HPP_ */