#ifndef CPP_ESSENTIALS_CORE_STRING_VIEWS_HPP_
#define CPP_ESSENTIALS_CORE_STRING_VIEWS_HPP_

#pragma once

#include <string>
#include <cstring>
#include <locale>
#include <string_view>

#include <cpp_essentials/core/views.hpp>
#include <cpp_essentials/core/trim.hpp>

namespace cpp_essentials::core
{

using cstring_mut_view = mut_span<char>;
using cstring_view = span<char>;

using string_mut_view = mut_view<std::string>;
using string_view = view<std::string>;

inline std::ostream& operator <<(std::ostream& os, const string_mut_view& item)
{
    std::copy(item.begin(), item.end(), std::ostream_iterator<char>{ os });
    return os;
}

inline std::ostream& operator <<(std::ostream& os, const string_view& item)
{
    std::copy(item.begin(), item.end(), std::ostream_iterator<char>{ os });
    return os;
}

inline std::ostream& operator <<(std::ostream& os, const cstring_mut_view& item)
{
    std::copy(item.begin(), item.end(), std::ostream_iterator<char>{ os });
    return os;
}

inline std::ostream& operator <<(std::ostream& os, const cstring_view& item)
{
    std::copy(item.begin(), item.end(), std::ostream_iterator<char>{ os });
    return os;
}

template <class Iter>
std::size_t string_hash(Iter begin, Iter end)
{
    auto b = &(*begin);
    auto e = &(*end);
#if _MSC_VER
    return std::_Hash_array_representation(b, e - b);
#else
    return std::_Hash_impl::hash(b, e - b);
#endif
}

namespace detail
{

struct trim_fn
{
    template
        < class Range
        , CONCEPT = cc::BidirectionalRange<Range>>
    auto operator ()(Range&& range, const std::locale& locale = {}) const
    {
        return core::trim_while(range, [&](auto ch) { return std::isspace(ch, locale); });
    }
};

struct c_str_fn
{
    cstring_mut_view operator ()(char* text) const
    {
        EXPECTS(text != nullptr);

        return { text, text + std::strlen(text) };
    }

    cstring_view operator ()(const char* text) const
    {
        EXPECTS(text != nullptr);

        return { text, text + std::strlen(text) };
    }

    cstring_mut_view operator ()(cstring_mut_view text) const
    {
        return text;
    }

    cstring_view operator ()(cstring_view text) const
    {
        return text;
    }

    cstring_mut_view operator ()(std::string& text) const
    {
        return { cstring_mut_view::iterator(&text[0]), cstring_mut_view::size_type(text.size()) };
    }

    cstring_view operator ()(const std::string& text) const
    {
        return { cstring_view::iterator(&text[0]), cstring_mut_view::size_type(text.size()) };
    }

    cstring_mut_view operator ()(string_mut_view text) const
    {
        return { cstring_mut_view::iterator(&text[0]), cstring_mut_view::size_type(text.size()) };
    }

    cstring_view operator ()(string_view text) const
    {
        return { cstring_view::iterator(&text[0]), cstring_view::size_type(text.size()) };
    }

    cstring_view operator ()(std::string_view text) const
    {
        return { cstring_view::iterator(&text[0]), cstring_view::size_type(text.size()) };
    }
};

} /* namespace detail */

static constexpr detail::trim_fn trim = {};
static constexpr detail::c_str_fn c_str = {};

namespace literals
{

inline cstring_view operator ""_s(const char* text, size_t length)
{
    return { text, text + length };
}

} /* namespace literals */

} /* namespace cpp_essentials::core */

namespace std
{

template <>
struct hash<::cpp_essentials::core::cstring_view>
{
    using argument_type = ::cpp_essentials::core::cstring_view;
    using result_type = size_t;

    result_type operator ()(const argument_type& item) const
    {
        return ::cpp_essentials::core::string_hash(item.begin(), item.end());
    }
};

template <>
struct hash<::cpp_essentials::core::cstring_mut_view>
{
    using argument_type = ::cpp_essentials::core::cstring_mut_view;
    using result_type = size_t;

    result_type operator ()(const argument_type& item) const
    {
        return ::cpp_essentials::core::string_hash(item.begin(), item.end());
    }
};

template <>
struct hash<::cpp_essentials::core::string_view>
{
    using argument_type = ::cpp_essentials::core::string_view;
    using result_type = size_t;

    result_type operator ()(const argument_type& item) const
    {
        return ::cpp_essentials::core::string_hash(item.begin(), item.end());
    }
};

template <>
struct hash<::cpp_essentials::core::string_mut_view>
{
    using argument_type = ::cpp_essentials::core::string_mut_view;
    using result_type = size_t;

    result_type operator ()(const argument_type& item) const
    {
        return ::cpp_essentials::core::string_hash(item.begin(), item.end());
    }
};

} /* namespace std */

#endif /* CPP_ESSENTIALS_CORE_STRING_VIEWS_HPP_ */
