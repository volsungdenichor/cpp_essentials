#ifndef CPP_ESSENTIALS_CORE_SERIALIZATION_HPP_
#define CPP_ESSENTIALS_CORE_SERIALIZATION_HPP_

#pragma once

#include <sstream>
#include <stdexcept>

#include <cpp_essentials/core/views.hpp>
#include <cpp_essentials/core/optional.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class T>
struct try_parse_t
{
    template <class Range>
    core::optional<T> operator ()(const Range& text) const
    {
        std::istringstream is{ text };
        T result;
        is >> result;
        return eval_optional(!is.fail(), [&]() { return result; });
    }
};

template <class T>
struct parse_t
{
    template <class Range>
    T operator ()(const Range& text) const
    {
        auto result = try_parse_t<T>{}(text);
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


struct serialize_t
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

struct to_string_t
{
    template <class... Args>
    std::string operator ()(const Args&... args) const
    {
        std::ostringstream ss;
        serialize_t{}(ss, args...);
        return ss.str();
    }
};

} /* namespace detail */

template <class T>
static constexpr detail::try_parse_t<T> try_parse = {};

template <class T>
static constexpr detail::parse_t<T> parse = {};

static constexpr detail::serialize_t serialize = {};
static constexpr detail::to_string_t to_string = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_SERIALIZATION_HPP_ */
