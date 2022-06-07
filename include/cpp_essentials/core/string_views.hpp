#ifndef CPP_ESSENTIALS_CORE_STRING_VIEWS_HPP_
#define CPP_ESSENTIALS_CORE_STRING_VIEWS_HPP_

#pragma once

#include <string>
#include <cstring>
#include <locale>
#include <string_view>

#include <cpp_essentials/core/views.hpp>
#include <cpp_essentials/core/views/trim.hpp>

namespace cpp_essentials::core
{

using cstr_view = span<char>;
using cstr_cview = cspan<char>;

using string_view = view<std::string>;
using string_cview = cview<std::string>;

inline std::ostream& operator <<(std::ostream& os, const string_view& item)
{
    std::copy(item.begin(), item.end(), std::ostream_iterator<char>{ os });
    return os;
}

inline std::ostream& operator <<(std::ostream& os, const string_cview& item)
{
    std::copy(item.begin(), item.end(), std::ostream_iterator<char>{ os });
    return os;
}

inline std::ostream& operator <<(std::ostream& os, const cstr_view& item)
{
    std::copy(item.begin(), item.end(), std::ostream_iterator<char>{ os });
    return os;
}

inline std::ostream& operator <<(std::ostream& os, const cstr_cview& item)
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
        , class = cc::BidirectionalRange<Range>>
    auto operator ()(Range&& range, const std::locale& locale = {}) const
    {
        return views::trim_while(range, [&](auto ch) { return std::isspace(ch, locale); });
    }
};

struct c_str_fn
{
    cstr_view operator ()(char* text) const
    {
        EXPECTS(text != nullptr);

        return { text, text + std::strlen(text) };
    }

    cstr_cview operator ()(const char* text) const
    {
        EXPECTS(text != nullptr);

        return { text, text + std::strlen(text) };
    }

    cstr_view operator ()(cstr_view text) const
    {
        return text;
    }

    cstr_cview operator ()(cstr_cview text) const
    {
        return text;
    }

    cstr_view operator ()(std::string& text) const
    {
        return { cstr_view::iterator(&text[0]), cstr_view::size_type(text.size()) };
    }

    cstr_cview operator ()(const std::string& text) const
    {
        return { cstr_cview::iterator(&text[0]), cstr_view::size_type(text.size()) };
    }

    cstr_view operator ()(string_view text) const
    {
        return { cstr_view::iterator(&text[0]), cstr_view::size_type(text.size()) };
    }

    cstr_cview operator ()(string_cview text) const
    {
        return { cstr_cview::iterator(&text[0]), cstr_cview::size_type(text.size()) };
    }

    cstr_cview operator ()(std::string_view text) const
    {
        return { cstr_cview::iterator(&text[0]), cstr_cview::size_type(text.size()) };
    }
};

} /* namespace detail */

static constexpr auto trim = detail::trim_fn{};
static constexpr auto c_str = detail::c_str_fn{};

namespace literals
{

inline cstr_cview operator ""_s(const char* text, size_t length)
{
    return { text, text + length };
}

} /* namespace literals */

} /* namespace cpp_essentials::core */

namespace std
{

template <>
struct hash<::cpp_essentials::core::cstr_cview>
{
    using argument_type = ::cpp_essentials::core::cstr_cview;
    using result_type = size_t;

    result_type operator ()(const argument_type& item) const
    {
        return ::cpp_essentials::core::string_hash(item.begin(), item.end());
    }
};

template <>
struct hash<::cpp_essentials::core::cstr_view>
{
    using argument_type = ::cpp_essentials::core::cstr_view;
    using result_type = size_t;

    result_type operator ()(const argument_type& item) const
    {
        return ::cpp_essentials::core::string_hash(item.begin(), item.end());
    }
};

template <>
struct hash<::cpp_essentials::core::string_cview>
{
    using argument_type = ::cpp_essentials::core::string_cview;
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

} /* namespace std */

#endif /* CPP_ESSENTIALS_CORE_STRING_VIEWS_HPP_ */
