#ifndef CPP_ESSENTIALS_CORE_SERIALIZATION_HPP_
#define CPP_ESSENTIALS_CORE_SERIALIZATION_HPP_

#pragma once

#include <sstream>
#include <stdexcept>

#include <cpp_essentials/core/views.hpp>
#include <cpp_essentials/core/string_views.hpp>
#include <cpp_essentials/core/optional.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class T>
std::string str(const T& item)
{
    std::ostringstream os;
    os << item;
    return os.str();
}

inline const std::string& str(const std::string& item)
{
    return item;
}

template <class T>
struct try_parse_fn
{
    template <class String>
    core::optional<T> operator ()(const String& text) const
    {
        std::istringstream is{ str(text) };
        T result;
        is >> result;
        return eval_optional(!is.fail(), [&]() { return result; });
    }
};

template <class T>
struct parse_fn
{
    template <class String>
    T operator ()(const String& text) const
    {
        static constexpr try_parse_fn<T> _try_parse = {};
        auto result = _try_parse(text);
        if (!result)
        {
            std::stringstream ss;
            ss << "could not parse '" << text << "'";
            throw std::runtime_error{ ss.str() };
        }
        return *result;
    }
};



struct ostream_guard
{
    std::ostream& _os;
    std::ios_base::fmtflags _flags;

    ostream_guard() = delete;
    ostream_guard(const ostream_guard&) = delete;
    ostream_guard(ostream_guard&&) = delete;

    ostream_guard& operator =(const ostream_guard&) = delete;
    ostream_guard& operator =(ostream_guard&&) = delete;

    ostream_guard(std::ostream& os)
        : _os{ os }
        , _flags{ os.flags() }
    {
    }

    ~ostream_guard()
    {
        _os.flags(_flags);
    }
};


struct serialize_fn
{
    template <class... Args>
    std::ostream& operator ()(std::ostream& os, const Args&... args) const
    {
        ostream_guard guard{ os };
        (void)guard;

        const auto apply = [&](const auto& item) { os << item; };
        (void)(apply(args), ...);
        return os;
    }
};

struct stringify_fn
{
    template <class... Args>
    std::string operator ()(const Args&... args) const
    {
        static constexpr serialize_fn _serialize = {};
        std::ostringstream ss;
        _serialize(ss, args...);
        return ss.str();
    }
};

} /* namespace detail */

template <class T>
static constexpr auto try_parse = adaptable{ detail::try_parse_fn<T>{} };

template <class T>
static constexpr auto parse = adaptable{ detail::parse_fn<T>{} };

static constexpr auto serialize = adaptable{ detail::serialize_fn{} };
static constexpr auto stringify = adaptable{ detail::stringify_fn{} };
static constexpr auto str = adaptable{ detail::stringify_fn{} };

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_SERIALIZATION_HPP_ */
