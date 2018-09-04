#ifndef CPP_ESSENTIALS_CORE_VIEW_HPP_
#define CPP_ESSENTIALS_CORE_VIEW_HPP_

#pragma once

#include <string>
#include <algorithm>
#include <cpp_essentials/core/iterator_range.hpp>

namespace cpp_essentials::core
{

template <class T>
using mut_span = iterator_range<T*>;

template <class T>
using span = iterator_range<const T*>;


template <class Container>
using mut_view = iterator_range<typename Container::iterator>;

template <class Container>
using view = iterator_range<typename Container::const_iterator>;


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
    std::_Fnv1a_hasher hasher;
    auto b = &(*begin);
    auto e = &(*end);
    return hasher._Add_bytes((const unsigned char*)b, (const unsigned char*)e);
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

#endif /* CPP_ESSENTIALS_CORE_VIEW_HPP_ */