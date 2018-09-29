#ifndef CPP_ESSENTIALS_CORE_FORMAT_HPP_
#define CPP_ESSENTIALS_CORE_FORMAT_HPP_

#include <ostream>
#include <iomanip>
#include <functional>

#include <cpp_essentials/core/string_views.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/slice.hpp>
#include <cpp_essentials/core/filter.hpp>
#include <cpp_essentials/core/containers.hpp>
#include <cpp_essentials/core/serialization.hpp>

namespace cpp_essentials::core
{

class format_error : public std::runtime_error
{
public:
    format_error(const std::string& message)
        : std::runtime_error { std::string { "format: " } + message }
    {
    }
};

namespace detail
{

struct locale_guard
{
    explicit locale_guard(std::ostream& os)
        : _os{ os }
        , _locale{ _os.getloc() }
    {
    }

    ~locale_guard()
    {
        _os.imbue(_locale);
    }

    std::ostream& _os;
    std::locale _locale;
};

struct format_guard
{
    explicit format_guard(std::ostream& os)
        : _os{ os }
        , _format{ _os.flags() }
    {
    }

    ~format_guard()
    {
        _os.flags(_format);
    }

    std::ostream& _os;
    std::ios::fmtflags _format;
};

using key_value_func = std::function<void(cstring_view, cstring_view)>;

void parse_term(cstring_view text, key_value_func func)
{    
    if (!text)
    {
        return;
    }

    auto sep = find(text, '=');
    if (sep)
    {
        auto key = core::make_range(text.begin(), sep.begin()) | core::trim();
        auto value = core::make_range(sep.begin() + 1, text.end()) | core::trim();
        func(key, value);
    }
    else
    {
        func(text | core::trim(), {});
    }
}

void parse_format(cstring_view text, key_value_func func)
{
    auto prev = text.begin();

    for (auto it = text.begin(); it != text.end(); ++it)
    {
        if (*it == '|')
        {
            parse_term(core::make_range(prev, it) | core::trim(), func);
            prev = it + 1;
        }
    }

    parse_term(core::make_range(prev, text.end()) | core::trim(), func);
}

struct format_modifier_handler
{
    std::ostream& os;

    format_modifier_handler(std::ostream& os)
        : os{ os }
    {
    }

    void format(cstring_view value) const
    {
        const auto is_placeholder = [](char ch) { return ch == '0'; };
        const auto is_dot = [](char ch) { return ch == '.'; };

        const auto integer = value | core::take_while(is_placeholder);
        value = value | core::drop(integer.size());

        const auto sep = value | core::take_while(is_dot);
        value = value | core::drop(sep.size());

        const auto fractional = value | core::take_while(is_placeholder);
        value = value | core::drop(fractional.size());

        EXPECTS(value.empty(), "extra format specifier");
        EXPECTS(sep.size() <= 1, "unexpected dot");

        if (sep.empty())
        {
            os
                << std::fixed << std::internal
                << std::setfill('0')
                << std::setw(integer.size());
        }
        else
        {
            os
                << std::fixed << std::internal
                << std::setfill('0')
                << std::setw(integer.size() + fractional.size() + 1)
                << std::setprecision(fractional.size());
        }
    }

    void operator ()(cstring_view key, cstring_view value) const
    {
        using namespace sq;

        if (key == "format"_str)
        {
            format(value);
        }
        else if (key == "width"_str)
        {
            os << std::setw(parse<int>(value));
        }
        else if (key == "fill"_str)
        {
            os << std::setfill(*value);
        }
        else if (key == "precision"_str)
        {
            os << std::setprecision(parse<int>(value));
        }
        else if (key == "left"_str)
        {
            os << std::left;
        }
        else if (key == "right"_str)
        {
            os << std::right;
        }
        else if (key == "internal"_str)
        {
            os << std::internal;
        }
        else if (key == "fixed"_str)
        {
            os << std::fixed;
        }
        else if (key == "scientific"_str)
        {
            os << std::scientific;
        }
        else if (key == "dec"_str)
        {
            os << std::dec;
        }
        else if (key == "hex"_str)
        {
            os << std::hex;
        }
        else if (key == "oct"_str)
        {
            os << std::oct;
        }
        else if (key == "boolalpha"_str)
        {
            os << std::boolalpha;
        }
        else if (key == "uppercase"_str)
        {
            os << std::uppercase;
        }
        else if (key == "showpoint"_str)
        {
            os << std::showpoint;
        }
        else if (key == "showpos"_str)
        {
            os << std::showpos;
        }
        else if (key == "showbase"_str)
        {
            os << std::showbase;
        }
        else
        {
            THROW(format_error{ "unrecognized format modifier" });
        }
    }
};

template <class T>
void write_value(std::ostream& os, cstring_view fmt, const T& value)
{
    const auto save_format = format_guard{ os };

    if (fmt)
    {
        parse_format(fmt, format_modifier_handler{ os });
    }

    os << value;
}


template <class Arg>
void write_argument(std::ostream& os, cstring_view fmt, int index, const Arg& arg)
{
    if (index == 0)
    {
        write_value(os, fmt, arg);
    }
    else
    {
        THROW(format_error { "argument index out of range" });
    }
}

template <class Arg, class... Args>
void write_argument(std::ostream& os, cstring_view fmt, int index, const Arg& arg, const Args&... args)
{
    if (index == 0)
    {
        write_value(os, fmt, arg);
    }
    else
    {
        write_argument(os, fmt, index - 1, args...);
    }
}

inline void write_argument(std::ostream& /*os*/, cstring_view /*fmt*/, int /*index*/)
{
    THROW(format_error { "argument index out of range" });
}

inline std::tuple<int, cstring_view> parse_field(cstring_view fmt)
{
    auto digits_fmt = fmt
        | core::drop_while([](char ch) { return std::isblank(ch, std::locale {});})
        | core::take_while([](char ch) { return std::isdigit(ch, std::locale {}); });

    int index = !digits_fmt.empty()
        ? core::parse<int>(digits_fmt)
        : -1;

    return std::make_tuple(index, core::find.as<core::return_next_end>(fmt, ':'));
}

template <class... Args>
void handle_item(std::ostream& os, int argument_index, cstring_view fmt, const Args&... args)
{
    auto [field_index, field_fmt] = parse_field(fmt);

    if (field_index == -1)
    {
        field_index = argument_index;
    }

    write_argument(os, field_fmt, field_index, args...);
}

template <class... Args>
void format(std::ostream& os, cstring_view fmt, const Args&... args)
{    
    int argument_index = 0;

    for (auto it = fmt.begin(); it != fmt.end(); ++it)
    {
        if (it[0] == '{')
        {
            if (it[1] == '{')
            {
                os << '{';
                ++it;
            }
            else
            {
                auto e = std::find(it + 1, fmt.end(), '}');

                if (e == fmt.end())
                {
                    THROW(format_error { "unclosed bracket" });
                }

                handle_item(os, argument_index++, cstring_view(it + 1, e), args...);

                it = e;
            }
        }
        else if (it[0] == '}')
        {
            if (it[1] != '}')
            {
                THROW(format_error { "unexpected closing bracket" });
            }

            os << '}';
            ++it;
        }
        else
        {
            os << *it;
        }

    }
}

} /* namespace detail */

template <class... Args>
std::ostream& format(std::ostream& os, const std::locale& locale, const char* fmt, const Args&... args)
{
    const auto save_locale = detail::locale_guard{ os };

    os.imbue(locale);
    detail::format(os, c_str(fmt), args...);
    return os;
}

template <class... Args>
std::ostream& format(std::ostream& os, const char* fmt, const Args&... args)
{
    detail::format(os, c_str(fmt), args...);
    return os;
}



template <class... Args>
std::string format(const std::locale& locale, const char* fmt, const Args&... args)
{
    std::stringstream ss;
    format(ss, locale, fmt, args...);
    return ss.str();
}

template <class... Args>
std::string format(const char* fmt, const Args&... args)
{
    std::stringstream ss;
    format(ss, fmt, args...);
    return ss.str();
}



template <class... Args>
std::ostream& print(const std::locale& locale, const char* fmt, const Args&... args)
{
    return format(std::cout, locale, fmt, args...);
}

template <class... Args>
std::ostream& print(const char* fmt, const Args&... args)
{
    return format(std::cout, fmt, args...);
}

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_FORMAT_HPP_ */

