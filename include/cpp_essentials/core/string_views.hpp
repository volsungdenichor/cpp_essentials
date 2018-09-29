#ifndef CPP_ESSENTIALS_CORE_STRING_VIEWS_HPP_
#define CPP_ESSENTIALS_CORE_STRING_VIEWS_HPP_

#pragma once

#include <string>
#include <locale>
#include <cpp_essentials/core/views.hpp>
#include <cpp_essentials/core/filter.hpp>

namespace cpp_essentials::core
{

using cstring_mut_view = mut_span<char>;
using cstring_view = span<char>;

using string_mut_view = mut_view<std::string>;
using string_view = view<std::string>;


using cwstring_mut_view = mut_span<wchar_t>;
using cwstring_view = span<wchar_t>;

using wstring_mut_view = mut_view<std::wstring>;
using wstring_view = view<std::wstring>;


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
#if 0
    std::_Fnv1a_hasher hasher;
    auto b = &(*begin);
    auto e = &(*end);
    return hasher._Add_bytes((const unsigned char*)b, (const unsigned char*)e);
#else
    return 0;
#endif
}

namespace detail
{

struct trim_t : adaptable<trim_t>
{
    using adaptable::operator();

    template <class Range, CONCEPT = cc::BidirectionalRange<Range>>
    auto operator ()(Range&& range, const std::locale& locale = {}) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);

        while (b != e && std::isspace(*b, locale))
        {
            ++b;
        }

        while (e != b && std::isspace(*std::prev(e), locale))
        {
            --e;
        }

        return make_range(b, e);
    }
};

struct c_str_t
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

static constexpr detail::trim_t trim = {};
static constexpr detail::c_str_t c_str = {};

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