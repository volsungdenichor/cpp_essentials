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

class cstring_mut_view : public mut_span<char>
{
public:
    using base_type = mut_span<char>;

    using iterator_range::iterator_range;

    cstring_mut_view(char* text)
        : base_type{ base_type::iterator(text), base_type::size_type(std::strlen(text)) }
    {
    }

    cstring_mut_view(std::string& text)
        : base_type{ base_type::iterator(&text[0]), base_type::size_type(text.size()) }
    {
    }

    cstring_mut_view(mut_view<std::string> text)
        : base_type{ base_type::iterator(&text[0]), base_type::size_type(text.size()) }
    {
    }    
};

class cstring_view : public span<char>
{
public:
    using base_type = span<char>;

    using iterator_range::iterator_range;

    cstring_view(char* text)
        : base_type{ base_type::iterator(text), base_type::size_type(std::strlen(text)) }
    {
    }

    cstring_view(const char* text)
        : base_type{ base_type::iterator(text), base_type::size_type(std::strlen(text)) }
    {
    }

    cstring_view(std::string& text)
        : base_type{ base_type::iterator(&text[0]), base_type::size_type(text.size()) }
    {
    }

    cstring_view(const std::string& text)
        : base_type{ base_type::iterator(&text[0]), base_type::size_type(text.size()) }
    {
    }

    cstring_view(mut_view<std::string> text)
        : base_type{ base_type::iterator(&text[0]), base_type::size_type(text.size()) }
    {
    }

    cstring_view(view<std::string> text)
        : base_type{ base_type::iterator(&text[0]), base_type::size_type(text.size()) }
    {
    }

    cstring_view(std::string_view text)
        : base_type{ base_type::iterator(&text[0]), base_type::size_type(text.size()) }
    {
    }
};

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

inline std::ostream& operator <<(std::ostream& os, const cstring_mut_view::base_type& item)
{
    std::copy(item.begin(), item.end(), std::ostream_iterator<char>{ os });
    return os;
}

inline std::ostream& operator <<(std::ostream& os, const cstring_view::base_type& item)
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

    cstring_mut_view operator ()(std::string& text) const
    {
        return { &(*text.begin()), &(*text.end()) };
    }

    cstring_view operator ()(const std::string& text) const
    {
        return { &(*text.begin()), &(*text.end()) };
    }

    cstring_mut_view operator ()(string_mut_view text) const
    {
        return { &(*text.begin()), &(*text.end()) };
    }

    cstring_view operator ()(string_view text) const
    {
        return { &(*text.begin()), &(*text.end()) };
    }
};

} /* namespace detail */

static constexpr detail::trim_fn trim = {};
static constexpr detail::c_str_fn c_str = {};

inline cstring_view operator ""_str(const char* text, size_t length)
{
    return { text, text + length };
}

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
