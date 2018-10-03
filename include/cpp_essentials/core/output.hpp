#ifndef CPP_ESSENTIALS_CORE_OUTPUT_HPP_
#define CPP_ESSENTIALS_CORE_OUTPUT_HPP_

#pragma once

#include <iostream>

namespace cpp_essentials::core
{

namespace detail
{

template <class Func>
struct manip_t
{
    Func func;
};

template <class Func>
std::ostream& operator <<(std::ostream& os, const manip_t<Func>& item)
{
    item.func(os);
    return os;
}

template <class Func>
auto manip(Func func) -> manip_t<Func>
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

template <class Func>
auto make_output_iterator(Func func) -> output_iterator_t<Func>
{
    return { std::move(func) };
}


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
auto delimit(Iter begin, Iter end, const C* separator)
{
    return manip([=](auto& os)
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

} /* namespace detail */

struct output_t
{
    template <class T = void, class C>
    auto operator ()(std::basic_ostream<C>& os, const C* separator = nullptr) const
    {
        return detail::make_output_iterator(detail::output_t<C, T>{ os, separator });
    }
};

static constexpr output_t output = {};


struct delimit_t
{
    template
        < class Range
        , class C
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, const C* separator) const
    {
        return detail::delimit(std::begin(range), std::end(range), separator);
    }
};

static constexpr delimit_t delimit = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_OUTPUT_HPP_ */